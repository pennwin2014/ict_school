#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include "ncreport.h" 
#include <time.h>
#include "ncmportalweb.h"

static int ncsWebReplace00(char *pHead,char *pVar,short nType,long lLen,char *pValue);
int ncmWebReplace(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);

/* 取星期几的时间*/
int ncmGetWeekDay(unsigned long lTime,int lDay,char *sDate)                                       
{
	printf("ncmGetWeekDay start!\n");
    struct tm *tm;
    unsigned long lTime1,lTime2;
    tm = localtime(&lTime);
    if(tm->tm_wday == 0) tm->tm_wday = 7;
    lTime1 = lTime - (tm->tm_wday -lDay) * 86400;
  
    tm = localtime(&lTime1);
    sprintf(sDate,"%04d/%02d/%02d",1900 + tm->tm_year,
                     tm->tm_mon +1,tm->tm_mday);
   
   printf("ncmGetWeekDay end!\n");
    return 0;
}


//报表分发设置
int ncmReportReset(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	printf("ncmReportReset start!!!!\n");
    char caTemp[2048];  
    utPltDbHead *psDbHead;
   	int iReturn,i;
   	char *p;
    char caUname[32],caMail[64],caReportid[12],caGroupid[12],caRname[32],caSid[16];
    unsigned long lSid,lGroupid,iNum,lReportid;
    char caAdd[16],caModi[16],caDel[16];
    char caGroupname[32];
    char caRtype[12],caSendtime[12],caTtype[12],caOrderby[12];
    unsigned long lTtype,lSendtime,lType,lOrderby;
    long maxsid;
 	  pasDbCursor *psCur;
 	 	
 	 	char u[5];
 	 	char weekre[14];
 	 	long uflg;
 	 	char sendmoment[10];
 	 	char weekend[50];
 	 	long weektime=0;
 	 	long weekcount;
 	 	long daycount;
 	 	long monthcount;
 	 	long dayre=0;
 	 	char nowday[16];
 	 	char nowday2[32];
 	 	long dayflg;
 	 	long daytime=0;
 	 	long rtid;
 	 	char weekmon[16]="";
 	 	char weeksun[16]="";
 	 	char monthfirst[16]="";
 	 	char monthover[16]="";
 	 	long monthtime=0;
 	 	long maxid=0;
 	 	long timemoment;
 	 	char hp_name[32];
 	 	char hp_bbname[32];
 	 	char sendmoment1[5];
 	 	char sendmoment2[5];
 	 	char dj_uname_t[236]="",dj_ip_t[236]="",dj_site_t[299]="",dj_uname[255]="",dj_ip[255]="",dj_url[255]="",dj_tonum[10]="",dj_denum[10]="";
 	 	long ltotal_num=0,ldetail_num=0;
		char selsid[1024]="";
		char selrid[1024]="";
	utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,22,"sid",      UT_TYPE_STRING,10, caSid,
	    																	  "uname",    UT_TYPE_STRING,30, caUname,
																				  "mail",     UT_TYPE_STRING,63, caMail,
																				  "reportid", UT_TYPE_STRING,10, caReportid,
																				  "groupid",  UT_TYPE_STRING,10, caGroupid,
																				  "add",      UT_TYPE_STRING,12, caAdd,
	    																	  "modi",     UT_TYPE_STRING,12, caModi,
	    																	  "del",      UT_TYPE_STRING,12, caDel,
	    																	 "rtype",    UT_TYPE_STRING,10, caRtype,					//按天按周。。
																				  "sendtime", UT_TYPE_STRING,10, caSendtime,
																				  "orderby",  UT_TYPE_STRING,10, caOrderby,				//报表类型
																				  "ttype",    UT_TYPE_STRING,10, caTtype,					//全部，工作时间
																				  "rname",    UT_TYPE_STRING,30, caRname,
																				  "dj_uname", UT_TYPE_STRING,156,dj_uname_t,
																					"dj_ip",    UT_TYPE_STRING,156,dj_ip_t,
																					"dj_site",  UT_TYPE_STRING,299,dj_site_t,
																					"dj_tonum", UT_TYPE_STRING,10, dj_tonum,
																					"dj_denum", UT_TYPE_STRING,10, dj_denum,
																				  "sendmoment",UT_TYPE_STRING,9,sendmoment1,
																				  "sendmoment2",UT_TYPE_STRING,9,sendmoment2,
																				  "selsid",UT_TYPE_STRING,sizeof(selsid)-1,selsid,
																				  "selrid",UT_TYPE_STRING,sizeof(selrid)-1,selrid);
				//printf("groupid=%s caOrderby=%s  dj_tonum=%s dj_denum=%s\n",caGroupid,caOrderby,dj_tonum,dj_denum);
				
				
				pasCvtGBK(2,caUname,caTemp,30);
				strcpy(caUname,caTemp);
				
				pasCvtGBK(2,caRname,caTemp,30);
				strcpy(caRname,caTemp);
				
				if(atoi(caRtype)==1)
					strcpy(sendmoment,sendmoment2);
				else
					strcpy(sendmoment,sendmoment1);
				//printf("== == caSendtime=%s\n",caSendtime);			
			if(strlen(dj_tonum)==0) strcpy(dj_tonum,"0");
				if(strlen(dj_denum)==0) strcpy(dj_denum,"0");
				//2算出，日，周，月的时间值																		==========守护进程执行间隔
		 		memset(caTemp,0,sizeof(caTemp));
		 		sprintf(caTemp,"select day(last_day(now()))");
  						pasDbOneRecord(caTemp,0,UT_TYPE_STRING,4,u);  
		 		/*
		 		daycount=3600*24;
		  	weekcount=3600*24*7;
		 		monthcount=3600*24*atol(u);	*/				
		 	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
		 		//求出开始时间，即设置时间与开始时间差												=========守护进程开始时间差
				sprintf(caTemp,"select date(now())");
		 			 pasDbOneRecord(caTemp,0,UT_TYPE_STRING,15,nowday); 
				if(atoi(caRtype)==1){																		//按天
					 if(strlen(caSendtime)>0)
					 sprintf(nowday2,"%s %s:00",nowday,sendmoment);
		 			 daytime=utTimStrToLong("%Y-%m-%d %H:%M",nowday2);													//推迟时间
		 		}else if(atoi(caRtype)==2){																	 //按周
		  		 sprintf(caTemp,"select weekday(now())");
  				 pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&uflg); 
  				 sprintf(caTemp,"SELECT  concat('20',date_format(date_sub(date(now()),interval %d day),\'%sy/%sm/%sd\'))",uflg,"%","%","%");								//周一
		 			pasDbOneRecord(caTemp,0,UT_TYPE_STRING,15,weekmon);
		 			 sprintf(caTemp,"SELECT  concat('20',date_format(date_add(date(now()),interval %d day),\'%sy/%sm/%sd\'))",7-uflg-1,"%","%","%");						//周日
		 			pasDbOneRecord(caTemp,0,UT_TYPE_STRING,15,weeksun); 
		
			}else if(atoi(caRtype)==3){																		//按月
		  		 sprintf(caTemp,"select day(now())");
  				 pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&uflg);  
  				 sprintf(caTemp,"select concat('20',date_format(concat(substr(last_day(now()),1,8),'01'),\'%sy/%sm/%sd\'))","%","%","%");				
  				 pasDbOneRecord(caTemp,0,UT_TYPE_STRING,15,monthfirst);						  //月初  
  				 sprintf(caTemp,"select  concat('20',date_format(last_day(now()),\'%sy/%sm/%sd\'))","%","%","%");
  				 pasDbOneRecord(caTemp,0,UT_TYPE_STRING,15,monthover);  						//月末
  	
		  	}else{}
		//  	printf("nowday2 = %s, weekmon = %s, weeksun = %s, monthfirst = %s, monthover = %s \n",nowday2,weekmon,weeksun,monthfirst,monthover);
		  	memset(weekend,0,sizeof(weekend));	
		 		
   	 if(!utStrIsSpaces(caAdd)) {  // 增加记录  
	 	//printf("Add start\n");
   	 		 sprintf(caTemp,"select max(sid)+1 from ncreporttype");
  			 iReturn =pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&maxsid);
   	 		 if(iReturn!=0) maxsid=1;
   	 
   	 		 	pasDbExecSqlF("insert into ncreporttype(sid,name,type,sendtime,ttype,orderby,timemoment,btflag,gl_uname,gl_ip,gl_url,gl_totalnum,gl_detail_num) \
                    		 values (%lu,'%s',%lu,%lu,%lu,%lu,%lu,%lu,'%s','%s','%s',%d,%d)",
                   		   maxsid,caRname,atol(caRtype),atol(caSendtime),atol(caTtype),atol(caOrderby),atoi(sendmoment),1,dj_uname_t,dj_ip_t,dj_site_t,atoi(dj_tonum),atoi(dj_denum));
   	 	 
    	   sprintf(caTemp,"insert into ncreportreceiver(name,mail,reportid,groupid) \
                    values ('%s','%s',%d,%d)",
                    caUname,caMail,maxsid,atol(caGroupid));
                    //caUname,caMail,atol(caReportid),atol(caGroupid));
				
        iReturn = pasDbExecSqlF("insert into ncreportreceiver(name,mail,reportid,groupid) \
                    values ('%s','%s',%d,%d)",
                    caUname,caMail,maxsid,atol(caGroupid));
		//printf("Add to ncm75mail_help\n");
	     //插入辅助表?
        sprintf(caTemp,"select max(hp_id)+1 from ncm75mail_help");
  			pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&maxid);
   	 		 if(maxid==0) maxid=1;
        pasDbExecSqlF("insert into ncm75mail_help(hp_id,hp_day,hp_week,hp_month,hp_week_mon,hp_week_sun,hp_month_fir,hp_month_las,hp_flag,hp_reportid,hp_serviceflag,hp_typeflag,hp_ttype,hp_groupid,hp_mail,hp_name,hp_bbname,hp_uname,hp_ip,hp_url,hp_totalnum,hp_detail_num) \
                    values (%lu,%lu,%lu,%lu,'%s','%s','%s','%s',%lu,%lu,%lu,%lu,%lu,%lu,'%s','%s','%s','%s','%s','%s',%d,%d)",
                    maxid,atoi(sendmoment),atoi(caSendtime),atoi(caSendtime),weekmon,weeksun,monthfirst,monthover,1,maxsid,atol(caOrderby),atol(caRtype),atol(caTtype),atol(caGroupid),caMail,caUname,caRname,dj_uname_t,dj_ip_t,dj_site_t,atoi(dj_tonum),atoi(dj_denum));

        if(iReturn != 0) {
            utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0797邮件分类"),ncLang("0311记录增加出错%s"),pasDbErrorMsg(NULL));
            pasDbRollback(NULL);
            return 0;
        }        
        else {
            pasDbCommit(NULL);
        }
    //   printf("Add end\n");
    }else if(!utStrIsSpaces(caDel)) { /* 删除记录  */
	//printf("Del start \n");
      		
	//	printf("selsid = %s \n",selsid);
	//	printf("selrid = %s \n",selrid);
		iReturn = pasDbExecSqlF("delete from ncreportreceiver where sid in (%s) ",selsid);
		
	//	printf("delete from ncreporttype where sid in (%s) \n",selrid);
		iReturn = pasDbExecSqlF("delete from ncreporttype where sid in (%s) ",selrid);
		iReturn = pasDbExecSqlF("delete from ncm75mail_help where hp_reportid in (%s)",selrid);
	//	printf("iReturn = %d \n",iReturn);
	//	printf("Del end\n");
		
		
		
    }else if(!utStrIsSpaces(caModi)) { /* 修改记录  */
	//	printf("modi start\n");
        if(!utStrIsSpaces(caSid)) {
            lSid = atol(caSid);
            iReturn = pasDbExecSqlF("update ncreportreceiver set name='%s',mail='%s',reportid=%d,groupid=%d where sid=%d ",
            	                       caUname,caMail,atol(caReportid),atol(caGroupid),lSid);   
       //     if(atol(caOrderby)==6)
       //     	iReturn = pasDbExecSqlF("update ncreporttype set name='%s',sendtime=%d,type=%d,ttype=%d,orderby=%d,timemoment=%lu,gl_uname='%s',gl_ip='%s',gl_url='%s',btflag=2,gl_totalnum=%d,gl_detail_num=%d where sid=%d  ",
       //     	                       caRname,atol(caSendtime),atol(caRtype),atol(caTtype),atol(caOrderby),atoi(sendmoment),dj_uname_t,dj_ip_t,dj_site_t,atoi(dj_tonum),atoi(dj_denum),atol(caReportid));         
      //      else        
            	iReturn = pasDbExecSqlF("update ncreporttype set name='%s',sendtime=%d,type=%d,ttype=%d,orderby=%d,timemoment=%lu,gl_uname='%s',gl_ip='%s',gl_url='%s',btflag=1,gl_totalnum=%d,gl_detail_num=%d where sid=%d  ",
            	                       caRname,atol(caSendtime),atol(caRtype),atol(caTtype),atol(caOrderby),atoi(sendmoment),dj_uname_t,dj_ip_t,dj_site_t,atoi(dj_tonum),atoi(dj_denum),atol(caReportid));         
           		//按月的插入表里的数据有问题
				iReturn = pasDbExecSqlF("update ncm75mail_help set hp_day=%d,hp_week=%d,hp_month=%d,hp_week_mon='%s',hp_week_sun='%s',hp_month_fir='%s',hp_month_las='%s',hp_flag=%d,hp_serviceflag=%d,hp_typeflag=%d,hp_ttype=%d,hp_groupid=%d,hp_mail='%s',hp_name='%s',hp_bbname='%s',hp_uname='%s',hp_ip='%s',hp_url='%s',hp_totalnum=%d,hp_detail_num=%d where hp_reportid=%d ",
            	                       atoi(sendmoment),atoi(caSendtime),atoi(caSendtime),weekmon,weeksun,monthfirst,monthover,2,atol(caOrderby),atol(caRtype),atol(caTtype),atol(caGroupid),caMail,caUname,caRname,dj_uname_t,dj_ip_t,dj_site_t,atoi(dj_tonum),atoi(dj_denum),atol(caReportid));         
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }else {
                utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0797閭欢鍒嗙被"),ncLang("0305璁板綍淇敼鍑洪敊, %s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
            }
  
        }
	//	printf("modi end\n");
    }  	   	   
   	   	   
   sprintf(caTemp,"select a.sid,a.name,a.mail,a.groupid,a.reportid,b.name,b.sendtime,b.type,b.ttype,b.orderby,b.timemoment,b.gl_uname,b.gl_ip,b.gl_url,gl_totalnum,gl_detail_num from ncreportreceiver a,ncreporttype b where a.reportid=b.sid  order by a.mail ");
//printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0152閮ㄩ棬鏌ヨ"),ncLang("0171鏌ヨ鏁版嵁鍑洪敊"));
        return 0;
    }   
 
    psDbHead = utPltInitDb();
    lSid=0;
    memset(caUname,0,sizeof(caUname));
    memset(caMail,0,sizeof(caMail));
    lGroupid=0;
    lReportid=0;
    memset(caRname,0,sizeof(caRname));   
    lSendtime=0;
    lTtype=0;
    lOrderby=0;
    lType=0;
    memset(sendmoment,0,sizeof(sendmoment));
    timemoment=0;
    
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lSid,
     	                             UT_TYPE_STRING,30,caUname,
     	                             UT_TYPE_STRING,63,caMail,
     	                             UT_TYPE_LONG,4,&lGroupid,
     	                             UT_TYPE_LONG,4,&lReportid,
     	                             UT_TYPE_STRING,30,caRname,
     	                             UT_TYPE_LONG,4,&lSendtime,
     	                             UT_TYPE_LONG,4,&lType,
     	                             UT_TYPE_LONG,4,&lTtype,
    															 UT_TYPE_LONG,4,&lOrderby,
    															 UT_TYPE_LONG,4,&timemoment,
    															 UT_TYPE_STRING,254,dj_uname,
    															 UT_TYPE_STRING,254,dj_ip,
    															 UT_TYPE_STRING,254,dj_url,
    															 UT_TYPE_LONG,4,&ltotal_num,
    															 UT_TYPE_LONG,4,&ldetail_num);
     															
     	                             
   																
   	iNum = 0;
   	
   	while(iReturn == 0||iReturn==1405 ) {
        iNum++;
		
		if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
        utPltPutLoopVarF(psDbHead,"num",iNum,"%d",iNum);
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%d",lSid);
       
       if(lGroupid==999999999)
       		utPltPutLoopVar(psDbHead,"groupname",iNum,ncLang("0446全部"));
       else
       // 	utPltPutLoopVar(psDbHead,"groupname",iNum,ncUtlGetGroupNameByid(psShmHead,lGroupid,"\0"));
		
		//char* caUname_UTF8 = convert("GBK","UTF-8", caUname);
        utPltPutLoopVar(psDbHead,"uname",iNum,caUname);
        utPltPutLoopVar(psDbHead,"mail",iNum,caMail);
        utPltPutLoopVar(psDbHead,"rname",iNum,caRname);
        utPltPutLoopVarF(psDbHead,"groupid",iNum,"%d",lGroupid);
        utPltPutLoopVarF(psDbHead,"rid",iNum,"%d",lReportid);
        if(lOrderby==1)
          utPltPutLoopVar(psDbHead,"type_desc",iNum,ncLang("2047Internet Surfing Detail Report"));
        else if(lOrderby==2)
          utPltPutLoopVar(psDbHead,"type_desc",iNum,ncLang("2048Internet Surfing Statistics Report"));
        else if(lOrderby==3)
         utPltPutLoopVar(psDbHead,"type_desc",iNum,ncLang("2049Device Model Report"));
        else if(lOrderby==4)
         utPltPutLoopVar(psDbHead,"type_desc",iNum,ncLang("2050SMS Detail Report")); 
        else if(lOrderby==5)
         utPltPutLoopVar(psDbHead,"type_desc",iNum,ncLang("2051SMS Statistics Report")); 
        else if(lOrderby==7)
         utPltPutLoopVar(psDbHead,"type_desc",iNum,"上网时间综合图表"); 
        else
         utPltPutLoopVar(psDbHead,"type_desc",iNum,ncLang("1925上网情况报表")); 	
        utPltPutLoopVarF(psDbHead,"type_desc1",iNum,"%d",lOrderby); 
        
        if(lTtype==1)
         utPltPutLoopVar(psDbHead,"ttype_desc",iNum,ncLang("1926工作时间"));
        else if(lTtype==2)
         utPltPutLoopVar(psDbHead,"ttype_desc",iNum,ncLang("1927非工作时间"));
        else if(lTtype==0)
         utPltPutLoopVar(psDbHead,"ttype_desc",iNum,ncLang("0446全部"));
        utPltPutLoopVarF(psDbHead,"ttype_desc1",iNum,"%d",lTtype); 
        
        if(lType==1){ 
        	utPltPutLoopVarF(psDbHead,"timemoment1",iNum,"%d",timemoment); 
        	sprintf(caTemp,ncLang("1928每日 %d:00"),timemoment);
      		utPltPutLoopVar(psDbHead,"sendtime",iNum,caTemp);
        }else if(lType==2){
      		utPltPutLoopVarF(psDbHead,"sendtime1",iNum,"%d",lSendtime); 
      		utPltPutLoopVarF(psDbHead,"timemoment1",iNum,"%d",timemoment); 
      		sprintf(caTemp,ncLang("1929星期%d %d:00"),lSendtime,timemoment);
      		utPltPutLoopVar(psDbHead,"sendtime",iNum,caTemp);
      	}else if(lType==3){
      		utPltPutLoopVarF(psDbHead,"sendtime1",iNum,"%d",lSendtime);
      		utPltPutLoopVarF(psDbHead,"timemoment1",iNum,"%d",timemoment); 
      		sprintf(caTemp,ncLang("1930每月%d日 %d:00"),lSendtime,timemoment);
      		utPltPutLoopVar(psDbHead,"sendtime",iNum,caTemp);
      	}
      
      if(lType==1)
        utPltPutLoopVar(psDbHead,"orderby",iNum,ncLang("1931日报"));
      else if(lType==2)
        utPltPutLoopVar(psDbHead,"orderby",iNum,ncLang("1932周报"));
      else if(lType==3)
        utPltPutLoopVar(psDbHead,"orderby",iNum,ncLang("1933月报")); 
      utPltPutLoopVarF(psDbHead,"orderby1",iNum,"%d",lType); 
      utPltPutLoopVar(psDbHead,"xh_uname",iNum,dj_uname);
      utPltPutLoopVar(psDbHead,"xh_ip",iNum,dj_ip);
      utPltPutLoopVar(psDbHead,"xh_url",iNum,dj_url);
      if(ltotal_num==0)
      	utPltPutLoopVar(psDbHead,"xh_tonum",iNum,"");
      else
      	utPltPutLoopVarF(psDbHead,"xh_tonum",iNum,"%d",ltotal_num); 
      if(ldetail_num==0)
      	utPltPutLoopVar(psDbHead,"xh_denum",iNum,"");
      else
      	utPltPutLoopVarF(psDbHead,"xh_denum",iNum,"%d",ldetail_num); 
    	lSid=0;
   		memset(caUname,0,sizeof(caUname));
   	 	memset(caMail,0,sizeof(caMail));
    	lGroupid=0;
    	lReportid=0;
    	memset(caRname,0,sizeof(caRname));   
    	lSendtime=0;
    	lTtype=0;
    	lOrderby=0;
    	lType=0;
  		timemoment=0;
  		ltotal_num=0;
  		ldetail_num=0;
  		memset(dj_uname,0,sizeof(dj_uname));   
  		memset(dj_ip,0,sizeof(dj_ip));   
  		memset(dj_url,0,sizeof(dj_url));   
    	iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lSid,
     	                             	 UT_TYPE_STRING,30,caUname,
     	                             	 UT_TYPE_STRING,63,caMail,
     	                             	 UT_TYPE_LONG,4,&lGroupid,
     	                             	 UT_TYPE_LONG,4,&lReportid,
     	                             	 UT_TYPE_STRING,30,caRname,
     	                               UT_TYPE_LONG,4,&lSendtime,
     	                             	 UT_TYPE_LONG,4,&lType,
     	                             	 UT_TYPE_LONG,4,&lTtype,
    															 	 UT_TYPE_LONG,4,&lOrderby,
    															 	 UT_TYPE_LONG,4,&timemoment,
    															 	 UT_TYPE_STRING,254,dj_uname,
    															 	 UT_TYPE_STRING,254,dj_ip,
    															 	 UT_TYPE_STRING,254,dj_url,
    															 	 UT_TYPE_LONG,4,&ltotal_num,
    															 	 UT_TYPE_LONG,4,&ldetail_num);
    }
    pasDbCloseCursor(psCur);
	utPltPutVarF(psDbHead,"TotRec","%d",iNum);
  
  
    sprintf(caTemp,"select groupid,groupname from ncgroup ");
    psCur = pasDbOpenSql(caTemp,0);

    if(psCur == NULL) {
         utWebDispMsg(iFd,psMsgHead,"ncm/ncmsg_back.htm",ncLang("0152部门查询"),ncLang("0171查询数据出错"));
        return 0;
    }   
    lGroupid=0;
    memset(caGroupname,0,sizeof(caGroupname));
     
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lGroupid,
     	                               UT_TYPE_STRING,30,caGroupname);
     															   																
   	iNum = 0;
   	
   	while(iReturn == 0||iReturn==1405 ) {
        iNum++;      
        utPltPutLoopVar(psDbHead,"gname",iNum,caGroupname);
        utPltPutLoopVarF(psDbHead,"gid",iNum,"%d",lGroupid);      
	    lGroupid=0;
	    memset(caGroupname,0,sizeof(caGroupname));
	     
	    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lGroupid,
	     	                               UT_TYPE_STRING,30,caGroupname);
    }
    pasDbCloseCursor(psCur);
  
    sprintf(caTemp,"select sid,name from ncreporttype ");
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncm/ncmsg_back.htm",ncLang("0152部门查询"),ncLang("0171查询数据出错"));
        return 0;
    }   
    lSid=0;
    memset(caRname,0,sizeof(caRname));
     
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lSid,
     	                               UT_TYPE_STRING,30,caRname);
     															   																
   	iNum = 0;
   	
   	while(iReturn == 0||iReturn==1405 ) {
        iNum++;      
        utPltPutLoopVar(psDbHead,"rrname",iNum,caRname);
        utPltPutLoopVarF(psDbHead,"rrid",iNum,"%d",lSid);      
		    lSid=0;
		    memset(caRname,0,sizeof(caRname));
		     
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lSid,
		     	                               UT_TYPE_STRING,30,caRname);
    }
    pasDbCloseCursor(psCur);
	//utPltShowDb(psDbHead);
  
     utPltOutToHtml(iFd,psMsgHead,psDbHead,"suning/ncm_send_report_control_list.htm");
	 
	printf("ncmReportReset  end!!!!\n");
    return 0;
}

//发送生成统计报表
int ncmReportSend(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	printf("ncmReportSend start!\n");
	pasDbCursor *psCur;
	char caTemp[1024];
	char caTemp1[256];
	char sqlbuf[1024];
	int iReturn;
	char caPath[128];
  struct reportsender{
	    	char caName[32];
	    	char caMail[64];
	    	unsigned long lGroupid;
	    	char caRname[32];
	    	long lType;
	    	unsigned long lStime;
	    	unsigned long lEtime;
	    	unsigned long lSendtime;
	    	long lTtype;
	    	long lOrderby;
	    	long ltsid;
	    	char dj_uname[254];
	    	char dj_ip[254];
	    	char dj_url[254];
	    	long dj_totalnum;
	    	long dj_detail_num;
	} sReportsender[100];
	char caName[32],caMail[64],caRname[32];
	char caToday[20];
	long lType;
	unsigned long lStime;
	unsigned long lEtime;
	unsigned long lSendtime;
	long lTtype;
	long lOrderby,iNum,i;
	unsigned long lGroupid;
	long lCount;
	char caTempdate[20];
	char *pSendtime;
	long lSendtime1;
	char caTtime[12];
	char caCond[512],caOrder[128],caFile[128];
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		FILE *fp; 
		time_t t; 
		char caTemp2[556];
		long hp_week,hp_month;
		long hp_flag,hp_reportid,hp_serviceflag,hp_typeflag;
		char hp_week_mon[16],hp_week_sun[16],hp_month_fir[16],hp_month_las[16];
		char u[5];
		long hp_day;
		char nowmoment[18];
		char uflg[5];
		long uuflg;		
		
		
    long hp_ttype,hp_groupid;
    
    struct mailhelp{
    		long ma_hp_day;
    		long ma_hp_week;
    		long ma_hp_month;
	    	char ma_hp_week_mon[16];
	    	char ma_hp_week_sun[16];
	    	char ma_hp_month_fir[16];
	    	char ma_hp_month_las[16];	    	
	    	long ma_hp_flag;
	    	long ma_hp_reportid;
	    	long ma_hp_serviceflag;
	    	long ma_hp_typeflag;
	    	long ma_hp_ttype;
	    	long ma_hp_groupid;
	    	char ma_hp_mail[64];	 
	    	char ma_hp_name[64];
	    	char ma_hp_bbname[64];   
	    	long ma_hp_id;
	    	char dj_uname[254];
	    	char dj_ip[254];
	    	char dj_url[254];
	    	long dj_totalnum;
	    	long dj_detail_num;
	  } mailhp[1000];
	  long iiNum;
	  char hp_name[32];
 	 	char hp_bbname[32];
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		long tsid;
		long hp_id;
		long he_time;
	//	int folder_count=0;
		char folder_count[12]="";
		char dj_uname[254]="",dj_ip[254]="",dj_url[254]="";
		long dj_totalnum=0,dj_detail_num=0;
	  iReturn = pasConnect(psShmHead);
    if(iReturn < 0) {
        //ncSysLog(NC_LOG_ERROR,"Start ReportSend, Database connect error");
        sleep(60);
        return 0;
    }
	
	 char *pConfig;
    char *pFile;
    pasConfList *psConfig;
    utPltDbHead *psDbHead;
    psDbHead = utPltInitDb();
    char ReportPosition[1024] = "";
    char ReportFtp[128] = "";
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","读取配置文件",ncLang("0427配置文件出错"));
        return 0;
    }
    //如若没有配置，默认一楼。
    strcpy(ReportPosition,pasUtlLookConfig(psConfig,"ReportPosition","/home/ncmysql/ncm/db/applereport"));
    strcpy(ReportFtp,pasUtlLookConfig(psConfig,"NcMailFrom","proeim@pronetway.com"));
	printf("ReportPosition = %s\n",ReportPosition);
	printf("ReportFtp = %s\n",ReportFtp);
	
	
   //ncreport杩涚▼鍚姩锛屽垱寤烘枃浠跺す
	 	int k;
		char file_Folder[128];
		char file_Folder2[128];		
		char file_Folder3[128];		
		long re_flag,re_flag2;
		char folder_path[256]="";
		char *p;
		char caOut[128];
		int number=0;
		strcpy(folder_path,ReportPosition); //创建文件夹
		p=strtok(folder_path, "/");
	//	printf("p = %s\n",p);
		sprintf(file_Folder,"%s /%s","cd",p);
	//	printf("file_Folder = %s\n",file_Folder);
		sprintf(file_Folder2,"%s /%s","mkdir",p);
	//	printf("file_Folder2 = %s\n",file_Folder2);
		sprintf(file_Folder3,"%s /%s","chmod 755",p);
	//	printf("file_Folder3 = %s\n",file_Folder3);
		re_flag=system(file_Folder);
		if(re_flag!=0){
			system(file_Folder2); 
			system(file_Folder3);
		}
	  	while(p=strtok(NULL, "/")){
	  			//strcpy(caOut[number],p);
	  			//number++;
	  		//	p=strtok(NULL, "/");
		//		printf("p = %s\n",p);
				snprintf(file_Folder+strlen(file_Folder), sizeof(file_Folder)-strlen(file_Folder), "/%s", p);
		//		printf("file_Folder = %s\n",file_Folder);
				snprintf(file_Folder2+strlen(file_Folder2), sizeof(file_Folder2)-strlen(file_Folder2), "/%s", p);
		//		printf("file_Folder2 = %s\n",file_Folder2);
				snprintf(file_Folder3+strlen(file_Folder3), sizeof(file_Folder3)-strlen(file_Folder3), "/%s", p);
		//		printf("file_Folder3 = %s\n",file_Folder3);
				re_flag=system(file_Folder);
				if(re_flag!=0){
				system(file_Folder2); 
				system(file_Folder3);
				}
	  	}
		strcpy(caToday,utTimFormat("%Y-%m-%d",time(0)));
		snprintf(file_Folder+strlen(file_Folder), sizeof(file_Folder)-strlen(file_Folder), "/%s", caToday);
	//	printf("file_Folder = %s\n",file_Folder);
		snprintf(file_Folder2+strlen(file_Folder2), sizeof(file_Folder2)-strlen(file_Folder2), "/%s", caToday);
	//	printf("file_Folder2 = %s\n",file_Folder2);
		snprintf(file_Folder3+strlen(file_Folder3), sizeof(file_Folder3)-strlen(file_Folder3), "/%s", caToday);
	//	printf("file_Folder3 = %s\n",file_Folder3);
		re_flag=system(file_Folder);  
		if(re_flag!=0){
			system(file_Folder2); 
			system(file_Folder3);
		}
		
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime ); 
	pSendtime=utComGetVar_sd(psShmHead,"SendReporttime","18");
	lSendtime1=atol(pSendtime);
	//while(1){	
		strcpy(caToday,utTimFormat("%Y/%m/%d",time(0)));
		strcpy(caTtime,utTimFormat("%H",time(0)));
		memset(caTemp,0,sizeof(caTemp));
		sprintf(caTemp,"select a.name,a.mail,a.groupid,b.name,b.type,b.stime,b.timemoment,b.sendtime,b.ttype,b.orderby,b.sid,gl_uname,gl_ip,gl_url,gl_totalnum,gl_detail_num from ncreportreceiver a,ncreporttype b where a.reportid=b.sid and btflag=1 limit 0,99");
	
	//	printf("caTemp=%s\n",caTemp);
			  psCur = pasDbOpenSql(caTemp,0);             
		    			if(psCur == NULL) {
		    				  printf("open psCur error\n");
		        			return 0;
		    }
		    memset(caName,0,sizeof(caName));
		    memset(caMail,0,sizeof(caMail));
		    lGroupid=0;
		    memset(caRname,0,sizeof(caRname));
		    lTtype=0;
		    lStime=0;
		    lEtime=0;
		    lSendtime=0;
		    lType=0;
		    tsid=0;
		    memset(dj_uname,0,sizeof(dj_uname));   
  			memset(dj_ip,0,sizeof(dj_ip));   
  			memset(dj_url,0,sizeof(dj_url));   
			  iReturn=pasDbFetchInto(psCur,    UT_TYPE_STRING,30,caName,
			                                   UT_TYPE_STRING,63,caMail,
			                                   UT_TYPE_LONG,4,&lGroupid,
			                                   UT_TYPE_STRING,30,caRname,
			                                   UT_TYPE_LONG,4,&lType,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_LONG,4,&lEtime,
			                                   UT_TYPE_LONG,4,&lSendtime,
			                                   UT_TYPE_LONG,4,&lTtype,
			                                   UT_TYPE_LONG,4,&lOrderby,
			                                   UT_TYPE_LONG,4,&tsid,
			                                   UT_TYPE_STRING,254,dj_uname,
			    															 UT_TYPE_STRING,254,dj_ip,
			    															 UT_TYPE_STRING,254,dj_url,
			    															 UT_TYPE_LONG,4,&dj_totalnum,
			                                   UT_TYPE_LONG,4,&dj_detail_num);
		//	printf("iReturn=%d\n",iReturn);
		 iNum=0;
		 while((iReturn==0)||(iReturn==1405)){
				
			 	strcpy(sReportsender[iNum].caName,caName);
			 	strcpy(sReportsender[iNum].caMail,caMail);
			 	sReportsender[iNum].lGroupid=lGroupid;
			 	strcpy(sReportsender[iNum].caRname,caRname);
			 	sReportsender[iNum].lType=lType;
			 	sReportsender[iNum].lStime=lStime;
			 	sReportsender[iNum].lEtime=lEtime;
			 	sReportsender[iNum].lSendtime=lSendtime;
			 	sReportsender[iNum].lTtype=lTtype;
			 	sReportsender[iNum].lOrderby=lOrderby;
			 	sReportsender[iNum].ltsid=tsid;
		 	  strcpy(sReportsender[iNum].dj_uname,dj_uname);
		 	  strcpy(sReportsender[iNum].dj_ip,dj_ip);
		 	  strcpy(sReportsender[iNum].dj_url,dj_url);
		 	  sReportsender[iNum].dj_totalnum=dj_totalnum;
		 	  sReportsender[iNum].dj_detail_num=dj_detail_num;
		 		memset(caName,0,sizeof(caName));
		    memset(caMail,0,sizeof(caMail));
		    lGroupid=0;
		    memset(caRname,0,sizeof(caRname));
		    lTtype=0;
		    lStime=0;
		    lEtime=0;
		    lSendtime=0;
		    lType=0;
		    iNum++;
		    tsid=0;
		    memset(dj_uname,0,sizeof(dj_uname));   
  			memset(dj_ip,0,sizeof(dj_ip));   
  			memset(dj_url,0,sizeof(dj_url));  
  			dj_totalnum=0;
  			dj_detail_num=0; 
			  iReturn=pasDbFetchInto(psCur,    UT_TYPE_STRING,30,caName,
			                                   UT_TYPE_STRING,63,caMail,
			                                   UT_TYPE_LONG,4,&lGroupid,
			                                   UT_TYPE_STRING,30,caRname,
			                                   UT_TYPE_LONG,4,&lType,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_LONG,4,&lEtime,
			                                   UT_TYPE_LONG,4,&lSendtime,
			                                   UT_TYPE_LONG,4,&lTtype,
			                                   UT_TYPE_LONG,4,&lOrderby,
			                                   UT_TYPE_LONG,4,&tsid,
			                                   UT_TYPE_STRING,254,dj_uname,
			    															 UT_TYPE_STRING,254,dj_ip,
			    															 UT_TYPE_STRING,254,dj_url,
			    															 UT_TYPE_LONG,4,&dj_totalnum,
			                                   UT_TYPE_LONG,4,&dj_detail_num);
		}
    pasDbCloseCursor(psCur); 
	
    for(i=0;i<iNum;i++){
	//	printf("sReportsender[i].lEtime=%lu,  timeinfo->tm_hour = %lu\n",sReportsender[i].lEtime,timeinfo->tm_hour);
	//	sReportsender[i].lEtime=14;
    	if(sReportsender[i].lType==1&&sReportsender[i].lEtime==timeinfo->tm_hour){ //鍒ゆ柇鏄惁鏃ユ姤锛屽彂閫佹椂闂翠笌鐜板湪鏃堕棿灏忔椂鏄惁鍖归厤
	//		printf("sReportsender[i].lType==1\n");
    		sprintf(caTempdate,"%s %02d",caToday,sReportsender[i].lEtime);
    		he_time=utTimStrToLong("%Y/%m/%d %H",caTempdate);
    		lCount=0;																									//鍙戦�佺殑鏃堕棿
    		sprintf(sqlbuf,"select count(*) from ncmreportsendlog where sendtime>=%lu and mail='%s' and rname='%s' and groupid=%lu",he_time-3600,sReportsender[i].caMail,sReportsender[i].caRname,sReportsender[i].lGroupid);
    //		printf("sqlbuf=%s\n",sqlbuf);
    		pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lCount);
	//		printf("lCount = %d\n",lCount);
    		if(lCount>0){
    			continue;			//璺宠繃鏈寰幆鍒癴or
    		}
    		else{
    			sprintf(caCond," and b.stime>=%lu and b.stime<=%lu ",utTimStrToLong("%Y/%m/%d",utTimFormat("%Y/%m/%d",time(0))),utTimStrToLong("%Y/%m/%d",utTimFormat("%Y/%m/%d",time(0)))+86400);
    			if(sReportsender[i].lGroupid!=999999999){
    				sprintf(caCond+strlen(caCond)," and a.groupid=%lu ",sReportsender[i].lGroupid);
    			}
    			strcpy(caOrder," order by ub desc ");

    			sprintf(caPath,"%s/report",utComGetVar_sd(psShmHead,"NCHOME","/home/ncmysql/ncm"));
    		 if(!utFileIsExist(caPath)){
    		 	sprintf(caTemp1,"mkdir %s",caPath);
    		 	system(caTemp1);
    			}
    			sprintf(caFile,"%s/report/Report-%s.htm",utComGetVar_sd(psShmHead,"NCHOME","/home/ncmysql/ncm"),utTimFormat("%Y%m%d%H%M%S",time(0)));
    		//	ncWebGenReportHtml(psShmHead,caCond,caOrder,caFile,sReportsender[i].lTtype,sReportsender[i].lStime,99125,sReportsender[i].caRname,sReportsender[i].caMail);
    			
    		//	ncmWriteReportLog(psShmHead,sReportsender[i].caName,sReportsender[i].caMail,sReportsender[i].caRname,sReportsender[i].lGroupid,sReportsender[i].ltsid,1,"");
				
//    			remove(caFile);
    		}
    	}
    	else if(sReportsender[i].lType==2){
	//	printf("sReportsender[i].lType==2\n");
 //鍛ㄦ姤
 			  char caSendDate[20];
 			  ncmGetWeekDay(time(0),sReportsender[i].lSendtime,caSendDate);
    		if(strcmp(caToday,caSendDate)!=0){
    			continue;
    		}
    		
    		lSendtime=utTimStrToLong("%Y/%m/%d",caSendDate);
    		
    		lCount=0;
    		sprintf(sqlbuf,"select count(*) from ncmreportsendlog where sendtime>=%lu and mail='%s' and rname='%s' and groupid=%lu ",lSendtime-5* 86400,sReportsender[i].caMail,sReportsender[i].caRname,sReportsender[i].lGroupid);
		//	printf("sqlbuf=%s\n",sqlbuf);
    		pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lCount);
		//	printf("lCount = %d\n",lCount);
    		if(lCount>0){
    			continue;
    		}else{
    			sprintf(caCond," and b.stime>=%lu and b.stime<=%lu ",lSendtime-7* 86400,lSendtime);
    			if(sReportsender[i].lGroupid!=999999999){
    				sprintf(caCond+strlen(caCond)," and a.groupid=%lu ",sReportsender[i].lGroupid);
    			}
    			strcpy(caOrder," order by ub desc ");
    			
    			sprintf(caPath,"%s/report",utComGetVar_sd(psShmHead,"NCHOME","/home/ncmysql/ncm"));
    		  if(!utFileIsExist(caPath)){
	    		 	sprintf(caTemp1,"mkdir %s",caPath);
	    		 	system(caTemp1);
    			}
    			sprintf(caFile,"%s/report/Report-%s.htm",utComGetVar_sd(psShmHead,"NCHOME","/home/ncmysql/ncm"),utTimFormat("%Y%m%d%H%M%S",time(0)));
    		//	ncWebGenReportHtml(psShmHead,caCond,caOrder,caFile,sReportsender[i].lTtype,lSendtime-7* 86400,lSendtime,sReportsender[i].caRname,sReportsender[i].caMail);
    			
    		//	ncmWriteReportLog(psShmHead,sReportsender[i].caName,sReportsender[i].caMail,sReportsender[i].caRname,sReportsender[i].lGroupid,sReportsender[i].ltsid,1,"");
//    			remove(caFile);
    		}
    		
    	   		
    	}
    	else if(sReportsender[i].lType==3){
//		printf("sReportsender[i].lType==3\n");
 //鏈堟姤
 			  char caSendDate[20];
 			  //zms
//        sprintf(caSendDate,"%s/%02d",utTimFormat("%Y/%m",time(0)),sReportsender[i].lSendtime);  	
        sprintf(caSendDate,"%s/01",utTimFormat("%Y/%m",time(0)));  	
    		if(strcmp(caToday,caSendDate)!=0){
    			continue;
    		}
    		
    		lSendtime=utTimStrToLong("%Y/%m/%d",caSendDate);
    		
    		lCount=0;//鏌ョ湅鏃ュ織琛ㄩ噷璁剧疆鍙戦�佹椂闂村線鍓嶆帹涓�涓湀鏄惁鍙戣繃閭欢
//    		sprintf(sqlbuf,"select count(*) from ncmreportsendlog where sendtime>=%lu and mail='%s' and rname='%s' and groupid=%lu ",lSendtime-30* 86400,sReportsender[i].caMail,sReportsender[i].caRname,sReportsender[i].lGroupid);
    		//2013.2.5 zms姣忔湀鑷姩鍙?
    		sprintf(sqlbuf,"select count(*) from ncmreportsendlog where sendtime>=%lu and mail='%s' and rname='%s' and groupid=%lu ",lSendtime,sReportsender[i].caMail,sReportsender[i].caRname,sReportsender[i].lGroupid);
    	//	printf("sqlbuf=%s\n",sqlbuf);
    		pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lCount);
    		if(lCount>0){
    			continue;
    		}
    		else{
//    			sprintf(caCond," and b.stime>=%lu and b.stime<=%lu ",lSendtime-27* 86400,lSendtime);
    			sprintf(caCond," and b.stime>=%lu and b.stime<=%lu ",lSendtime,time(0));
    			if(sReportsender[i].lGroupid!=999999999){
    				sprintf(caCond+strlen(caCond)," and a.groupid=%lu ",sReportsender[i].lGroupid);
    			}
    			
    				strcpy(caOrder," order by ub desc ");
    			
    			sprintf(caPath,"%s/report",utComGetVar_sd(psShmHead,"NCHOME","/home/ncmysql/ncm"));
    		 if(!utFileIsExist(caPath)){
    		 	sprintf(caTemp1,"mkdir %s",caPath);
    		 	system(caTemp1);
    			}
    			
    			sprintf(caFile,"%s/report/Report-%s.htm",utComGetVar_sd(psShmHead,"NCHOME","/home/ncmysql/ncm"),utTimFormat("%Y%m%d%H%M%S",time(0)));
//    			ncWebGenReportHtml(psShmHead,caCond,caOrder,caFile,sReportsender[i].lTtype,lSendtime-30* 86400,lSendtime,sReportsender[i].caRname,sReportsender[i].caMail);
    		//	ncWebGenReportHtml(psShmHead,caCond,caOrder,caFile,sReportsender[i].lTtype,lSendtime,lSendtime,sReportsender[i].caRname,sReportsender[i].caMail);
    			
    		//	ncmWriteReportLog(psShmHead,sReportsender[i].caName,sReportsender[i].caMail,sReportsender[i].caRname,sReportsender[i].lGroupid,sReportsender[i].ltsid,1,"");
//    			remove(caFile);
    		}
    	}
    }
    memset(caTemp,0,sizeof(caTemp)); 
    sprintf(caTemp,"select substr(now(),12,2)");	
		pasDbOneRecord(caTemp,0,UT_TYPE_STRING,15,nowmoment);		 		
		memset(caTemp,0,sizeof(caTemp));  
		sprintf(caTemp,"select weekday(now())");
  	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,4,uflg);	
  	memset(caTemp,0,sizeof(caTemp));  
  	sprintf(caTemp,"select day(now())");
  	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,4,u); 
  	char lastday[5]="";
  	sprintf(caTemp,"select substr(last_day(now()),9,10);");
  	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,4,lastday); 
  	memset(caTemp,0,sizeof(caTemp));  
		sprintf(caTemp,"select hp_day,hp_week,hp_month,hp_week_mon,hp_week_sun,hp_month_fir,hp_month_las,hp_flag,hp_reportid,hp_serviceflag,hp_typeflag,hp_ttype,hp_groupid,hp_mail,hp_name,hp_bbname,hp_id,hp_uname,hp_ip,hp_url,hp_totalnum,hp_detail_num from ncm75mail_help ");				
//		printf("--- caTemp=%s \n",caTemp);
		psCur=pasDbOpenSql(caTemp,0);
		if(psCur == NULL) {
        printf("==psCur is Null==\n");
        return 0;
    }  
	
    hp_day=0;
		hp_week=0;
		hp_month=0;
		memset(hp_week_mon,0,sizeof(hp_week_mon));
		memset(hp_week_sun,0,sizeof(hp_week_sun));
		memset(hp_month_fir,0,sizeof(hp_month_fir));
		memset(hp_month_las,0,sizeof(hp_month_las));
		hp_flag=0;
		hp_reportid=0;
		hp_serviceflag=0;
		hp_ttype=0;
		hp_groupid=0; 
		memset(caMail,0,sizeof(caMail));
		hp_typeflag=0;
		memset(hp_name,0,sizeof(hp_name));
		memset(hp_bbname,0,sizeof(hp_bbname));
		hp_id=0;
		memset(dj_uname,0,sizeof(dj_uname));   
  	memset(dj_ip,0,sizeof(dj_ip));   
  	memset(dj_url,0,sizeof(dj_url));   
  	dj_totalnum=0;
  	dj_detail_num=0; 
		iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&hp_day,
																	 UT_TYPE_LONG,4,&hp_week,
																	 UT_TYPE_LONG,4,&hp_month,
																	 UT_TYPE_STRING,15,hp_week_mon,
																	 UT_TYPE_STRING,15,hp_week_sun,
																	 UT_TYPE_STRING,15,hp_month_fir,
																	 UT_TYPE_STRING,15,hp_month_las,
                                   UT_TYPE_LONG,4,&hp_flag,
                                   UT_TYPE_LONG,4,&hp_reportid,
                                   UT_TYPE_LONG,4,&hp_serviceflag,
                                   UT_TYPE_LONG,4,&hp_typeflag,
                                   UT_TYPE_LONG,4,&hp_ttype,
                                   UT_TYPE_LONG,4,&hp_groupid,
                                   UT_TYPE_STRING,63,caMail,
                                   UT_TYPE_STRING,31,hp_name,
                                   UT_TYPE_STRING,31,hp_bbname,
                                   UT_TYPE_LONG,4,&hp_id,
                                   UT_TYPE_STRING,254,dj_uname,
			    												 UT_TYPE_STRING,254,dj_ip,
			    												 UT_TYPE_STRING,254,dj_url,
			    												 UT_TYPE_LONG,4,&dj_totalnum,
			                             UT_TYPE_LONG,4,&dj_detail_num);
     uuflg=atoi(uflg)+1;
     iiNum=0;
		 while(iReturn == 0) {	
		 		mailhp[iiNum].ma_hp_day=hp_day;
		 		mailhp[iiNum].ma_hp_week=hp_week;
		 		mailhp[iiNum].ma_hp_month=hp_month;
		 		strcpy(mailhp[iiNum].ma_hp_week_mon,hp_week_mon);	
		 		strcpy(mailhp[iiNum].ma_hp_week_sun,hp_week_sun);	
		 		strcpy(mailhp[iiNum].ma_hp_month_fir,hp_month_fir);	
		 		strcpy(mailhp[iiNum].ma_hp_month_las,hp_month_las);
		 		mailhp[iiNum].ma_hp_flag=hp_flag;
		 		mailhp[iiNum].ma_hp_reportid=hp_reportid;
		 		mailhp[iiNum].ma_hp_serviceflag=hp_serviceflag;
		 		mailhp[iiNum].ma_hp_typeflag=hp_typeflag;
		 		mailhp[iiNum].ma_hp_ttype=hp_ttype;
		 		mailhp[iiNum].ma_hp_groupid=hp_groupid;	
		 		strcpy(mailhp[iiNum].ma_hp_mail,caMail);
		 		strcpy(mailhp[iiNum].ma_hp_name,hp_name);
		 		strcpy(mailhp[iiNum].ma_hp_bbname,hp_bbname);
		 		mailhp[iiNum].ma_hp_id=hp_id;	
		 		strcpy(mailhp[iiNum].dj_uname,dj_uname);
		 		strcpy(mailhp[iiNum].dj_ip,dj_ip);
		 		strcpy(mailhp[iiNum].dj_url,dj_url);
		 		mailhp[iiNum].dj_totalnum=dj_totalnum;	
		 		mailhp[iiNum].dj_detail_num=dj_detail_num;	
				iiNum++;
				hp_week=0;
				hp_month=0;
				dj_totalnum=0;
  			dj_detail_num=0; 
  			mailhp[iiNum].ma_hp_flag=hp_flag;
				memset(hp_week_mon,0,sizeof(hp_week_mon));
				memset(hp_week_sun,0,sizeof(hp_week_sun));
				memset(hp_month_fir,0,sizeof(hp_month_fir));
				memset(hp_month_las,0,sizeof(hp_month_las));
				hp_flag=0;
				hp_reportid=0;
				hp_serviceflag=0;
				hp_typeflag=0;
				hp_day=0;
				hp_ttype=0;
				hp_groupid=0; 
				memset(caMail,0,sizeof(caMail));
				memset(hp_name,0,sizeof(hp_name));
				memset(hp_bbname,0,sizeof(hp_bbname));
				hp_id=0;
				memset(dj_uname,0,sizeof(dj_uname));   
		  	memset(dj_ip,0,sizeof(dj_ip));   
		  	memset(dj_url,0,sizeof(dj_url));  
		  	dj_totalnum=0;
  			dj_detail_num=0; 
				iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&hp_day,
																	 		 UT_TYPE_LONG,4,&hp_week,
																	 		 UT_TYPE_LONG,4,&hp_month,
																	 		 UT_TYPE_STRING,15,hp_week_mon,
																	 		 UT_TYPE_STRING,15,hp_week_sun,
																	 		 UT_TYPE_STRING,15,hp_month_fir,
																	 		 UT_TYPE_STRING,15,hp_month_las,
                                   		 UT_TYPE_LONG,4,&hp_flag,
                                   		 UT_TYPE_LONG,4,&hp_reportid,
                                   		 UT_TYPE_LONG,4,&hp_serviceflag,
                                   		 UT_TYPE_LONG,4,&hp_typeflag,
                                   		 UT_TYPE_LONG,4,&hp_ttype,
                                   		 UT_TYPE_LONG,4,&hp_groupid,
                                   		 UT_TYPE_STRING,63,caMail,
                                   		 UT_TYPE_STRING,31,hp_name,
                                   		 UT_TYPE_STRING,31,hp_bbname,
                                   		 UT_TYPE_LONG,4,&hp_id,
                                   		 UT_TYPE_STRING,254,dj_uname,
			    												 		 UT_TYPE_STRING,254,dj_ip,
			    												 		 UT_TYPE_STRING,254,dj_url,
			    												 		 UT_TYPE_LONG,4,&dj_totalnum,
			                             		 UT_TYPE_LONG,4,&dj_detail_num);
			}
		
    	pasDbCloseCursor(psCur);
    	long kCount=0;
    	char tt2_5[20]="";
		//unsigned long sdate1 = time(0)-86400;
		//unsigned long edate1 = time(0);
		
    	for(i=0;i<iiNum;i++){
			char sdate1[32] = "";
			char edate1[32] = "";
			long lSdate =0;
			long lEdate =0;
		//	srand( (unsigned)time( NULL ) );
		//	folder_count = rand()%100;
			strcpy(folder_count,utTimFormat("%Y-%m-%d",time(0)));
		//	printf("folder_count = %s\n",folder_count);
    		if(mailhp[i].ma_hp_typeflag==1){													//控制统计类型标志位 1-按天 2-按周 3-按月			
		//			printf("mailhp[i].ma_hp_typeflag==1\n");
				//	mailhp[i].ma_hp_day=13;
					if(atoi(nowmoment)==mailhp[i].ma_hp_day){							//判断的当前时间和发送时间是否一样
						sprintf(sqlbuf,"select count(*) from ncmreportsendlog where sendtime>=%lu and mail='%s' and rname='%s' and groupid=%lu ",time(0)-3600,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid);
    	//		printf("sqlbuf=%s\n",sqlbuf);
    				pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&kCount);
    				
					if(kCount>0){
    					continue;
    				}else{	
						
		//				printf("mailhp[i].ma_hp_serviceflag = %lu\n",mailhp[i].ma_hp_serviceflag);
					
						strcpy(edate1,utTimFormat("%Y-%m-%d %H:%M:%S", time(0)));
						
						strcpy(sdate1,utTimFormat("%Y-%m-%d 00:00:00", time(0)));
										


							if(mailhp[i].ma_hp_serviceflag==1){								 		
								
								char report_download_path[128] ="";
								char caFilename[64];
								sprintf(caFilename, "intSurfDetailReport%s.csv",utTimFormat("%Y_%m_%d", time(0)));
								char path[128] = "";
								sprintf(path,"%s/%s/%s",ReportPosition,folder_count,caFilename);
								ncm_web_MC_intDetailReport_list(psShmHead, iFd,psMsgHead,sdate1,edate1,"create",path);
								strcpy(report_download_path,path);
					//			printf("report_download_path = %s \n",report_download_path);
								
								ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,report_download_path);	
								//strcpy(caTitle,ncLang("1524网站访问次数分布情况"));
								
								char ma_content[2048]="";
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Hi ");
								strcat(ma_content,mailhp[i].ma_hp_name);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Attachment: ");
								strcat(ma_content,caFilename);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"<br>\n");
							
								ncmMailAttachSend(ReportFtp,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,ma_content, report_download_path,caFilename);

							 }
							  if(mailhp[i].ma_hp_serviceflag==2){ //上网人数流量报表
								
								char report_download_path[128] ="";
								char caFilename[64];
								sprintf(caFilename, "intStatisticsReport%s.csv",utTimFormat("%Y_%m_%d", time(0)));
								char path[128] = "";
								sprintf(path,"%s/%s/%s",ReportPosition,folder_count,caFilename);
								ncm_web_MC_intStatisticsReport_list(psShmHead, iFd,psMsgHead,sdate1,edate1,"create",path);
								strcpy(report_download_path,path);
						//		printf("report_download_path = %s \n",report_download_path);
								
								ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,report_download_path);	
								//strcpy(caTitle,ncLang("1524网站访问次数分布情况"));
								
								char ma_content[2048]="";
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Hi ");
								strcat(ma_content,mailhp[i].ma_hp_name);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Attachment: ");
								strcat(ma_content,caFilename);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"<br>\n");
							
								ncmMailAttachSend(ReportFtp,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,ma_content, report_download_path,caFilename);
							 }
							  if(mailhp[i].ma_hp_serviceflag==3){ //设备类型报表
								char report_download_path[128] ="";
								char caFilename[64];
								sprintf(caFilename, "deviceModelReport%s.csv",utTimFormat("%Y_%m_%d", time(0)));
								char path[128] = "";
								sprintf(path,"%s/%s/%s",ReportPosition,folder_count,caFilename);
								
								ncm_web_MC_intDeviceReport_list(psShmHead, iFd,psMsgHead,sdate1,edate1,"create",path);
								strcpy(report_download_path,path);
						//		printf("report_download_path = %s \n",report_download_path);
								
								ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,report_download_path);	
								//strcpy(caTitle,ncLang("1524网站访问次数分布情况"));
								
								char ma_content[2048]="";
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Hi ");
								strcat(ma_content,mailhp[i].ma_hp_name);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Attachment: ");
								strcat(ma_content,caFilename);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"<br>\n");
							
								ncmMailAttachSend(ReportFtp,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,ma_content, report_download_path,caFilename);
								
							 }
							  if(mailhp[i].ma_hp_serviceflag==4){ //短信发送报表
								char report_download_path[128] ="";
								char caFilename[64];
								sprintf(caFilename, "smsDetailReport%s.csv",utTimFormat("%Y_%m_%d", time(0)));
								char path[128] = "";
								sprintf(path,"%s/%s/%s",ReportPosition,folder_count,caFilename);
								ncm_web_MC_smsDetailReport_list(psShmHead, iFd,psMsgHead,sdate1,edate1,"create",path);
								strcpy(report_download_path,path);
						//		printf("report_download_path = %s \n",report_download_path);
								
								ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,report_download_path);	
								//strcpy(caTitle,ncLang("1524网站访问次数分布情况"));
								
								char ma_content[2048]="";
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Hi ");
								strcat(ma_content,mailhp[i].ma_hp_name);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Attachment: ");
								strcat(ma_content,caFilename);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"<br>\n");
							
								ncmMailAttachSend(ReportFtp,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,ma_content, report_download_path,caFilename);
							 }
							  if(mailhp[i].ma_hp_serviceflag==5){ //SmsStatisticsReport
								
								char report_download_path[128] ="";
								char caFilename[64];
								sprintf(caFilename, "smsStatisticsReport%s.csv",utTimFormat("%Y_%m_%d", time(0)));
								char path[128] = "";
								sprintf(path,"%s/%s/%s",ReportPosition,folder_count,caFilename);
								ncm_ShowSmsStatisticsReport(psShmHead, iFd,psMsgHead,sdate1,edate1,path);
								strcpy(report_download_path,path);
							//	printf("report_download_path = %s \n",report_download_path);
								
								ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,report_download_path);	
								//strcpy(caTitle,ncLang("1524网站访问次数分布情况"));
								
								char ma_content[2048]="";
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Hi ");
								strcat(ma_content,mailhp[i].ma_hp_name);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Attachment: ");
								strcat(ma_content,caFilename);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"<br>\n");
							
								ncmMailAttachSend(ReportFtp,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,ma_content, report_download_path,caFilename);
								
							 }
							//else{}	
							// if(mailhp[i].ma_hp_serviceflag==7){ 						//涓婄綉鏃堕棿缁煎悎鍥捐〃	
								// ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,nc_jingdong_reportall_PDF_outgiving(psShmHead,iFd,mailhp[i].ma_hp_typeflag,"","","","",mailhp[i].ma_hp_ttype,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_mail,folder_count,mailhp[i].dj_uname,mailhp[i].dj_ip,mailhp[i].dj_url,mailhp[i].dj_totalnum,mailhp[i].dj_detail_num));
							// }else{}	
						//	folder_count++;
						//	if(folder_count==99) folder_count=0;
						}
					}																		  
				}else if(mailhp[i].ma_hp_typeflag==2){	//周报						
				//	printf("mailhp[i].ma_hp_typeflag==2\n");
					
				//	uuflg = 7;
				//	mailhp[i].ma_hp_day=15;
				//	printf("mailhp[i].ma_hp_week = %lu, uuflg = %lu\n",mailhp[i].ma_hp_week,uuflg);
					if((uuflg==mailhp[i].ma_hp_week)&&(atoi(nowmoment)==mailhp[i].ma_hp_day)){	
						sprintf(sqlbuf,"select count(*) from ncmreportsendlog where sendtime>=%lu and mail='%s' and rname='%s' and groupid=%lu ",time(0)-5*86400,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid);
 //   			printf("sqlbuf=%s\n",sqlbuf);
 						kCount=0;
    				pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&kCount);
    				if(kCount>0){
    					continue;
    				}else{
					
				//		printf("mailhp[i].ma_hp_serviceflag = %lu\n",mailhp[i].ma_hp_serviceflag);
						
						sprintf(sdate1, "%s 00:00:00", mailhp[i].ma_hp_week_mon);
						//strcpy(edate1,utTimFormat("%Y-%m-%d %H:%M:%S", time(0)));
						sprintf(edate1, "%s 23:59:59",mailhp[i].ma_hp_week_sun);
						
						ncTimeToLong(mailhp[i].ma_hp_week_mon, &lSdate);
						char sSdate[32]="";
						strcpy(sSdate,utTimFormat("%Y_%m_%d", lSdate));
				//		printf("sSdate = %s\n",sSdate);
						ncTimeToLong(mailhp[i].ma_hp_week_sun, &lEdate);
						char sEdate[32]="";
						strcpy(sEdate,utTimFormat("%Y_%m_%d", lEdate));
				//		printf("sEdate = %s\n",sEdate);
						//strcpy(sdate1,utTimFormat("%Y-%m-%d 00:00:00", time(0)));
						
							 if(mailhp[i].ma_hp_serviceflag==1){																		
								char report_download_path[128] ="";
								char caFilename[64]="";
								sprintf(caFilename, "intSurfDetailReport%s-%s.csv",sSdate,sEdate);
								char path[128] = "";
								sprintf(path,"%s/%s/%s",ReportPosition,folder_count,caFilename);
								ncm_web_MC_intDetailReport_list(psShmHead, iFd,psMsgHead,sdate1,edate1,"create",path);
								strcpy(report_download_path,path);
						//		printf("report_download_path = %s \n",report_download_path);
								
								ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,report_download_path);	
								//strcpy(caTitle,ncLang("1524网站访问次数分布情况"));
								
								char ma_content[2048]="";
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Hi ");
								strcat(ma_content,mailhp[i].ma_hp_name);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Attachment: ");
								strcat(ma_content,caFilename);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"<br>\n");
								
								ncmMailAttachSend(ReportFtp,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,ma_content, report_download_path,caFilename);
		
							 }
							  if(mailhp[i].ma_hp_serviceflag==2){
								char report_download_path[128] ="";
								char caFilename[64];
							
								sprintf(caFilename, "intStatisticsReport%s-%s.csv",sSdate,sEdate);
								char path[128] = "";
								sprintf(path,"%s/%s/%s",ReportPosition,folder_count,caFilename);
								ncm_web_MC_intStatisticsReport_list(psShmHead, iFd,psMsgHead,sdate1,edate1,"create",path);
								strcpy(report_download_path,path);
							//	printf("report_download_path = %s \n",report_download_path);
								
								ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,report_download_path);	
								//strcpy(caTitle,ncLang("1524网站访问次数分布情况"));
								
								char ma_content[2048]="";
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Hi ");
								strcat(ma_content,mailhp[i].ma_hp_name);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Attachment: ");
								strcat(ma_content,caFilename);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"<br>\n");
							
								ncmMailAttachSend(ReportFtp,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,ma_content, report_download_path,caFilename);
							 } if(mailhp[i].ma_hp_serviceflag==3){
								char report_download_path[128] ="";
								char caFilename[64];
								sprintf(caFilename, "deviceModelReport%s-%s.csv",sSdate,sEdate);
				
								char path[128] = "";
								sprintf(path,"%s/%s/%s",ReportPosition,folder_count,caFilename);
								ncm_web_MC_intDeviceReport_list(psShmHead, iFd,psMsgHead,sdate1,edate1,"create",path);
								strcpy(report_download_path,path);
							//	printf("report_download_path = %s \n",report_download_path);
								
								ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,report_download_path);	
								//strcpy(caTitle,ncLang("1524网站访问次数分布情况"));
								
								char ma_content[2048]="";
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Hi ");
								strcat(ma_content,mailhp[i].ma_hp_name);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Attachment: ");
								strcat(ma_content,caFilename);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"<br>\n");
							
								ncmMailAttachSend(ReportFtp,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,ma_content, report_download_path,caFilename);
							 } 
							 if(mailhp[i].ma_hp_serviceflag==4){
								char report_download_path[128] ="";
								char caFilename[64];
							
								sprintf(caFilename, "smsDetailReport%s-%s.csv",sSdate,sEdate);
								char path[128] = "";
								sprintf(path,"%s/%s/%s",ReportPosition,folder_count,caFilename);
								ncm_web_MC_smsDetailReport_list(psShmHead, iFd,psMsgHead,sdate1,edate1,"create",path);
								strcpy(report_download_path,path);
							//	printf("report_download_path = %s \n",report_download_path);
								
								ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,report_download_path);	
								//strcpy(caTitle,ncLang("1524网站访问次数分布情况"));
								
								char ma_content[2048]="";
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Hi ");
								strcat(ma_content,mailhp[i].ma_hp_name);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Attachment: ");
								strcat(ma_content,caFilename);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"<br>\n");
							
								ncmMailAttachSend(ReportFtp,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,ma_content, report_download_path,caFilename);
							 } 
							 if(mailhp[i].ma_hp_serviceflag==5){
								char report_download_path[128] ="";
								char caFilename[64];
								sprintf(caFilename, "smsStatisticsReport%s-%s.csv",sSdate,sEdate);
								
								char path[128] = "";
								sprintf(path,"%s/%s/%s",ReportPosition,folder_count,caFilename);
								ncm_ShowSmsStatisticsReport(psShmHead, iFd,psMsgHead,sdate1,edate1,path);
								strcpy(report_download_path,path);
							//	printf("report_download_path = %s \n",report_download_path);
								
								ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,report_download_path);	
								//strcpy(caTitle,ncLang("1524网站访问次数分布情况"));
								
								char ma_content[2048]="";
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Hi ");
								strcat(ma_content,mailhp[i].ma_hp_name);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Attachment: ");
								strcat(ma_content,caFilename);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"<br>\n");
							
								ncmMailAttachSend(ReportFtp,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,ma_content, report_download_path,caFilename);
							 }else{}
							// if(mailhp[i].ma_hp_serviceflag==7){
								// ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,nc_jingdong_reportall_PDF_outgiving(psShmHead,iFd,mailhp[i].ma_hp_typeflag,mailhp[i].ma_hp_week_mon,mailhp[i].ma_hp_week_sun,"","",mailhp[i].ma_hp_ttype,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_mail,folder_count,mailhp[i].dj_uname,mailhp[i].dj_ip,mailhp[i].dj_url,mailhp[i].dj_totalnum,mailhp[i].dj_detail_num));
							// }
						//	folder_count++;
						//	if(folder_count==99) folder_count=0;
						}
					}																			  
				}else if(mailhp[i].ma_hp_typeflag==3){	//月报													
				//	printf("mailhp[i].ma_hp_typeflag==3\n");
					if(atoi(lastday)<=mailhp[i].ma_hp_month)		mailhp[i].ma_hp_month=atoi(lastday);
					sprintf(tt2_5,"%s/01",utTimFormat("%Y/%m",time(0)));
					strcpy(mailhp[i].ma_hp_month_fir,tt2_5);
					memset(tt2_5,0,sizeof(tt2_5));
					sprintf(tt2_5,"%s/%s",utTimFormat("%Y/%m",time(0)),lastday);
					strcpy(mailhp[i].ma_hp_month_las,tt2_5);
//					printf("ma_hp_month_fir=%s ma_hp_month_las=%s\n",mailhp[i].ma_hp_month_fir,mailhp[i].ma_hp_month_las);
					memset(tt2_5,0,sizeof(tt2_5));
				//	printf("u = %d\n",atoi(u));
				//	mailhp[i].ma_hp_month=26;
				//	mailhp[i].ma_hp_day=15;
  				if((atoi(u)==mailhp[i].ma_hp_month)&&(atoi(nowmoment)==mailhp[i].ma_hp_day)){
  					sprintf(sqlbuf,"select count(*) from ncmreportsendlog where sendtime>=%lu and mail='%s' and rname='%s' and groupid=%lu ",utTimStrToLong("%Y/%m/%d",mailhp[i].ma_hp_month_fir),mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid);
    //			printf("sqlbuf=%s\n",sqlbuf);
 						kCount=0;
    				pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&kCount);
    				if(kCount>0){
    					continue;
    				}else{
					
					//	printf("mailhp[i].ma_hp_serviceflag = %lu\n",mailhp[i].ma_hp_serviceflag);
						
						sprintf(sdate1, "%s 00:00:00", mailhp[i].ma_hp_month_fir);
						//strcpy(edate1,utTimFormat("%Y-%m-%d %H:%M:%S", time(0)));
						sprintf(edate1, "%s 23:59:59",mailhp[i].ma_hp_month_las);
						
						ncTimeToLong(mailhp[i].ma_hp_month_fir, &lSdate);
						char sSdate[32]="";
						strcpy(sSdate,utTimFormat("%Y_%m_%d", lSdate));	
					//	printf("sSdate = %s\n",sSdate);
						ncTimeToLong(mailhp[i].ma_hp_month_las, &lEdate);
						char sEdate[32]="";
						strcpy(sEdate,utTimFormat("%Y_%m_%d", lEdate));
					//	printf("sEdate = %s\n",sEdate);
					//	printf("===================\n",sEdate);
						
  						if(mailhp[i].ma_hp_serviceflag==1){		
  							
								char report_download_path[128] ="";
								char caFilename[64]="";
								sprintf(caFilename, "intSurfDetailReport%s-%s.csv",sSdate,sEdate);
								char path[128] = "";
								sprintf(path,"%s/%s/%s",ReportPosition,folder_count,caFilename);
								ncm_web_MC_intDetailReport_list(psShmHead, iFd,psMsgHead,sdate1,edate1,"create",path);
								strcpy(report_download_path,path);
							//	printf("report_download_path = %s \n",report_download_path);
								
								ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,report_download_path);	
								//strcpy(caTitle,ncLang("1524网站访问次数分布情况"));
								
								char ma_content[2048]="";
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Hi ");
								strcat(ma_content,mailhp[i].ma_hp_name);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Attachment: ");
								strcat(ma_content,caFilename);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"<br>\n");
								
								ncmMailAttachSend(ReportFtp,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,ma_content, report_download_path,caFilename);
							
							
  						 } 
						 if(mailhp[i].ma_hp_serviceflag==2){						
								char report_download_path[128] ="";
								char caFilename[64];
							
								sprintf(caFilename, "intStatisticsReport%s-%s.csv",sSdate,sEdate);
								char path[128] = "";
								sprintf(path,"%s/%s/%s",ReportPosition,folder_count,caFilename);
								ncm_web_MC_intStatisticsReport_list(psShmHead, iFd,psMsgHead,sdate1,edate1,"create",path);
								strcpy(report_download_path,path);
							//	printf("report_download_path = %s \n",report_download_path);
								
								ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,report_download_path);	
								//strcpy(caTitle,ncLang("1524网站访问次数分布情况"));
								
								char ma_content[2048]="";
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Hi ");
								strcat(ma_content,mailhp[i].ma_hp_name);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Attachment: ");
								strcat(ma_content,caFilename);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"<br>\n");
							
								ncmMailAttachSend(ReportFtp,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,ma_content, report_download_path,caFilename);
								
							 }
							 if(mailhp[i].ma_hp_serviceflag==3){		
								char report_download_path[128] ="";
								char caFilename[64];
								sprintf(caFilename, "deviceModelReport%s-%s.csv",sSdate,sEdate);
								
								char path[128] = "";
								sprintf(path,"%s/%s/%s",ReportPosition,folder_count,caFilename);
								ncm_web_MC_intDeviceReport_list(psShmHead, iFd,psMsgHead,sdate1,edate1,"create",path);
								strcpy(report_download_path,path);
							//	printf("report_download_path = %s \n",report_download_path);
								
								ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,report_download_path);	
								//strcpy(caTitle,ncLang("1524网站访问次数分布情况"));
								
								char ma_content[2048]="";
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Hi ");
								strcat(ma_content,mailhp[i].ma_hp_name);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Attachment: ");
								strcat(ma_content,caFilename);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"<br>\n");
							
								ncmMailAttachSend(ReportFtp,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,ma_content, report_download_path,caFilename);
							 } 
							 if(mailhp[i].ma_hp_serviceflag==4){
								char report_download_path[128] ="";
								char caFilename[64];
								
								sprintf(caFilename, "smsDetailReport%s-%s.csv",sSdate,sEdate);
								char path[128] = "";
								sprintf(path,"%s/%s/%s",ReportPosition,folder_count,caFilename);
								ncm_web_MC_smsDetailReport_list(psShmHead, iFd,psMsgHead,sdate1,edate1,"create",path);
								strcpy(report_download_path,path);
							//	printf("report_download_path = %s \n",report_download_path);
								
								ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,report_download_path);	
								//strcpy(caTitle,ncLang("1524网站访问次数分布情况"));
								
								char ma_content[2048]="";
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Hi ");
								strcat(ma_content,mailhp[i].ma_hp_name);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Attachment: ");
								strcat(ma_content,caFilename);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"<br>\n");
							
								ncmMailAttachSend(ReportFtp,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,ma_content, report_download_path,caFilename);
								
							 } 
							 if(mailhp[i].ma_hp_serviceflag==5){
								
								char report_download_path[128] ="";
								char caFilename[64];
								sprintf(caFilename, "smsStatisticsReport%s-%s.csv",sSdate,sEdate);
								char path[128] = "";
								sprintf(path,"%s/%s/%s",ReportPosition,folder_count,caFilename);
								ncm_ShowSmsStatisticsReport(psShmHead, iFd,psMsgHead,sdate1,edate1,path);
								strcpy(report_download_path,path);
							//	printf("report_download_path = %s \n",report_download_path);
								
								ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,report_download_path);	
								//strcpy(caTitle,ncLang("1524网站访问次数分布情况"));
								
								char ma_content[2048]="";
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Hi ");
								strcat(ma_content,mailhp[i].ma_hp_name);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"&nbsp;&nbsp;&nbsp;&nbsp;");
								strcat(ma_content,"Attachment: ");
								strcat(ma_content,caFilename);
								strcat(ma_content,"<br>\n");
								strcat(ma_content,"<br>\n");
							
								ncmMailAttachSend(ReportFtp,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,ma_content, report_download_path,caFilename);
								
								
							 }else{}	
							// if(mailhp[i].ma_hp_serviceflag==7){
								// ncmWriteReportLog(psShmHead,mailhp[i].ma_hp_name,mailhp[i].ma_hp_mail,mailhp[i].ma_hp_bbname,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_id,2,nc_jingdong_reportall_PDF_outgiving(psShmHead,iFd,mailhp[i].ma_hp_typeflag,"","",mailhp[i].ma_hp_month_fir,mailhp[i].ma_hp_month_las,mailhp[i].ma_hp_ttype,mailhp[i].ma_hp_groupid,mailhp[i].ma_hp_mail,folder_count,mailhp[i].dj_uname,mailhp[i].dj_ip,mailhp[i].dj_url,mailhp[i].dj_totalnum,mailhp[i].dj_detail_num));
							// }
						//	folder_count++;
						//	if(folder_count==99) folder_count=0;
						}
					}				
				}else{}			
					
				
			}
     	memset(uflg,0,sizeof(uflg));
    	memset(nowmoment,0,sizeof(nowmoment)); 	    	
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~09-9-14~~~~~~~~~~~     														
		sleep(3600);
	printf("ncmReportSend end!\n");	
//	}
}

//写报表发送日志?
int ncmWriteReportLog(utShmHead  *psShmHead,char *caName,char *caMail,char *caRname,unsigned long groupid,long midid,int flag,char *down_name){
  printf("ncmWriteReportLog start!\n");	
  int iReturn;
  char down_name2[255];
  strcpy(down_name2,down_name);
	iReturn = pasDbExecSqlF("insert into ncmreportsendlog(name,mail,rname,sendtime,groupid,midid,flag,down_name) \
                    values ('%s','%s','%s',%d,%lu,%lu,%d,'%s')",
                    caName,caMail,caRname,time(0),groupid,midid,flag,down_name2);  
  
  printf("ncmWriteReportLog end!\n");	
  return iReturn;
}


//邮件附件发送
int ncmMailAttachSend(char *pFrom,char *pTo,char *pSub,char *pCon,char *pFile,char *pDispname)
{
	printf("ncmMailAttachSend start!\n");
//	printf("pFrom = %s \n",pFrom);
//	printf("pTo = %s \n",pTo);
//	printf("pSub = %s \n",pSub);
//	printf("pCon = %s \n",pCon);
//	printf("pFile = %s \n",pFile);
//	printf("pDispname = %s \n",pDispname);
    struct mimeMail *psMail;
    char  *msgbuf;
    int   newlen;
    newMailCode(pasDefaultCode());
    psMail = newMail(pFrom,pTo,NULL,pSub,pCon,1,NULL);
    if(psMail) {
		
	//	printf("pFile = %s \n",pFile);
    	  addAttachMentFromFile(psMail,pFile,pDispname,"text/plain");
        msgbuf = createMimeMail(psMail,&newlen);
        if(msgbuf) {        
            pasSendMailCont(msgbuf,pFrom,pTo);
            free(msgbuf);
        }
//        sendMimeMail(psMail,NULL);
        freeMail(psMail);
    }else {
        return (-1);
    }
	printf("ncmMailAttachSend end!\n");
}


//总体运行状况
/*
int ncm_Totalstate(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	printf("ncm_Totalstate start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	char name[32] = "";
	utMsgGetSomeNVar(psMsgHead, 1,
   				    "name",        UT_TYPE_STRING,  sizeof(name)-1,       name);
	
	struct totalstate
    {
        char siteName[20];
		char serialNo[20];
		long ip;
		char running[20];
		char network[20];
		char portal[20];
		char radius[20];
		char sms[20];
		char log[20];
//		struct totalstate *next;
	};

	struct totalstate totalstate[4] = {
	{"Shanghai IFC","PN72SA01",3232240742,"Active","Online","Normal","Normal","Normal","Normal"},
	{"Shanghai IFC","PN72SA02",3232240111,"Standby","Online","Failure","Failure","Normal","Normal"},
	{"Shanghai IFC","PN72SA03",3232240222,"Active","Online","Failure","Normal","Failure","Normal"},
	{"Shanghai IFC","PN72SA04",3232240333,"Standby","Offline","Failure","Normal","Failure","Normal"}
	};
	struct totalstate *p;
	p=totalstate;
	int i = 0;
	int iNum = 0;
	//printf("SiteName,  Serial No.,  IP,  Running,  Network,  Portal,  Radius,  SMS,  Log,\n");
	utPltPutLoopVarF(psDbHead,"sid",iNum,"%d",iNum);
	
	for(i=0;i<4;i++)
	{
	
	printf("%s,%s,%lu,%s,%s,%s,%s,%s,%s,\n",(p+i)->siteName,
												(p+i)->serialNo,
												(p+i)->ip,
												(p+i)->running,
												(p+i)->network,
												(p+i)->portal,
												(p+i)->radius,
												(p+i)->sms,
												(p+i)->log);
												
			iNum++;
			if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
		//	utPltPutLoopVarF(psDbHead,"dh",iNum,"%s",",");
			utPltPutLoopVarF(psDbHead,"sid",iNum,"%d",iNum);
			utPltPutLoopVarF(psDbHead,"siteName",iNum,"%s",(p+i)->siteName);
			utPltPutLoopVarF(psDbHead,"serialNo",iNum,"%s",(p+i)->serialNo);
			utPltPutLoopVarF(psDbHead,"ip",iNum,"%lu",(p+i)->ip);
			utPltPutLoopVarF(psDbHead,"running",iNum,"%s",(p+i)->running);
			utPltPutLoopVarF(psDbHead,"network",iNum,"%s",(p+i)->network);
			utPltPutLoopVarF(psDbHead,"portal",iNum,"%s",(p+i)->portal);
			utPltPutLoopVarF(psDbHead,"radius",iNum,"%s",(p+i)->radius);
			utPltPutLoopVarF(psDbHead,"sms",iNum,"%s",(p+i)->sms);
			utPltPutLoopVarF(psDbHead,"log",iNum,"%s",(p+i)->log);
	}
	utPltPutVarF(psDbHead,"TotRec","%d",i);
	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_totalstate_list.htm");
	printf("ncm_Totalstate  end!!!!\n");
	//free(p);
	
	return 0;	  
}
*/

//报表分发日志
int ncm_ReportSendLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	printf("ncm_ReportSendLog start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	char keywords[32] = "";
	char start[12]="";
	char limit[12]="";
	char sort[12]="";
	char dir[12]="";
	
	utMsgGetSomeNVar(psMsgHead, 5,
   				    "ip1",        UT_TYPE_STRING,  sizeof(keywords)-1,       keywords,
					"start",        UT_TYPE_STRING,  sizeof(start)-1,       start,
					"limit",        UT_TYPE_STRING,  sizeof(limit)-1,       limit,
					"sort",        UT_TYPE_STRING,  sizeof(sort)-1,       sort,
					"dir",        UT_TYPE_STRING,  sizeof(dir)-1,       dir
					);
	
	
	char caTemp[256];
	long lCount1=0;
	sprintf(caTemp,"select count(*) from ncmreportsendlog");
	pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
	utPltPutVarF(psDbHead,"TotRec","%lu", lCount1);
//	printf("lCount1 = %lu\n",lCount1);
	sprintf(caTemp,"select sid, name, mail, rname,sendtime,down_name from ncmreportsendlog");	
	if(!utStrIsSpaces(keywords) )//楂樼骇IP
			{
					snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where name like '%%%s%%' or mail like '%%%s%%' or rname like '%%%s%%'", keywords,keywords,keywords);
			}
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by %s %s", sort, dir);
	
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu", atol(start), atol(limit));
  	printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		printf("pscur is null\n");
		printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
	
		int iReturn=0;
		int iNum=0;
		long id =0;
		char name[32];
		char mail[64] = "";
		char rname[32] = "";
		int sendtime=0;
		char down_name[256]="";
		char _stime[32] = "";
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&id,
      	                     UT_TYPE_STRING,sizeof(name),name,
      	                     UT_TYPE_STRING,sizeof(mail),mail,
      	                     UT_TYPE_STRING,sizeof(rname),rname,
      	                     UT_TYPE_LONG,4,&sendtime,
							 UT_TYPE_STRING,sizeof(down_name),down_name))  || 1405==iReturn)
		{	
			iNum++;
			if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
			utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu", id);
			
			strcpy(_stime, utTimFormat("%y/%m/%d %H:%M:%S", sendtime));
			utPltPutLoopVarF(psDbHead,"sendtime",iNum,"%s", _stime);
			utPltPutLoopVarF(psDbHead,"name",iNum,"%s",name);
			utPltPutLoopVarF(psDbHead,"mail",iNum,"%s",mail);
			utPltPutLoopVarF(psDbHead,"rname",iNum,"%s",rname);
			utPltPutLoopVarF(psDbHead,"down_name",iNum,"%s",down_name);
			
		}
		
		pasDbCloseCursor(psCur);
	}
	
	
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_send_report_log.htm");
	printf("ncm_ReportSendLog  end!!!!\n");
	//free(p);
	return 0;	  
}



int ncm_MailSendSave_par(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	printf("ncm_MailSendSave_par  start!!!!\n");
    char caApp[20];
    char caMsg[256];
    int iReturn;
    char caDbname[32],caDbuser[32],caDbpass[32];
    char *pConfig;
    char *p,*pFile,*pNat,*pRoute;
    FILE *fp;
    pasConfList *psConfig;
 //   if(iDebugLevel == NC_DEBUG_MSGINFO) {
 //	    utSysPrintMsg(psMsgHead,"ncMailSendSave_par"); //打印
 //	}

    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0880自动备份设定"),ncLang("0427配置文件出错"));
        return 0;
    }
    if(p = utMsgGetVar_s(psMsgHead,"MailReportType")) {
        pasUtlModiConfig(psConfig,"MailSmtpAuth",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"ncre_sender")) {
        pasUtlModiConfig(psConfig,"NcMailFrom",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"ncre_account")) {
        pasUtlModiConfig(psConfig,"MailAccount",p);
    }

    if(p = utMsgGetVar_s(psMsgHead,"ncre_passwd")) {
        pasUtlModiConfig(psConfig,"MailPassword",p);
    }
    
    
    if(p = utMsgGetVar_s(psMsgHead,"ncre_addr")) {
        pasUtlModiConfig(psConfig,"MailServer",p);
    }
    
    pasUtlSaveConfig(pConfig,psConfig);
    pasUtlFreeConfig(psConfig);
    p = utMsgGetVar_s(psMsgHead,"app");
    // if(p) {
        // if(!utStrIsSpaces(p)) {
            // ncUtlReReadConFig(psShmHead);
         //  ncUtlReloadConfig(psShmHead,NC_PID_COLLECT,NC_RELOAD_PQCONFIG,1); //刷新
        // }
    // }
    
    ncm_MailSendSet_par(psShmHead,iFd,psMsgHead);
	printf("ncm_MailSendSave_par  end!!!!\n");
    return 0;
}


/*******************邮件分发参数设定  ********************/
int ncm_MailSendSet_par(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{
	printf("ncm_MailSendSet_par  start!!!!\n");
    char *pConfig;
    char *pFile;
    pasConfList *psConfig;
    utPltDbHead *psDbHead;
    int iReturn;
    long lGroupid0=0;
    
        //增加网络权限判断
    iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
    if(lGroupid0>0)
    {
    	utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0510设置网络"),ncLang("0925您所属的管理组无该功能的权限"));
    	return 0;
    }
    psDbHead = utPltInitDb();
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0880自动备份设定"),ncLang("0427配置文件出错"));
        return 0;
    }
	utPltPutVar(psDbHead,"MailReportType",  pasUtlLookConfig(psConfig,"MailSmtpAuth","Yes"));
    utPltPutVar(psDbHead,"ncre_sender",   pasUtlLookConfig(psConfig,"NcMailFrom","proeim@pronetway.com"));
    utPltPutVar(psDbHead,"ncre_account", pasUtlLookConfig(psConfig,"MailAccount","proeim_proxy@pronetway.com"));
    utPltPutVar(psDbHead,"ncre_passwd", pasUtlLookConfig(psConfig,"MailPassword","proeim_proxy"));    
    utPltPutVar(psDbHead,"ncre_addr", pasUtlLookConfig(psConfig,"MailServer","mail.pronetway.com"));
	

	
   pasUtlFreeConfig(psConfig);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_mail_send_setting.htm");
	printf("ncm_MailSendSet_par  end!!!!\n");
    return 0;
}



//PORTAL端机型报表 终端类型
int ncm_DeviceTypeChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead){
	
  printf("ncm_DeviceTypeChart start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	char keywords[32] = "";
	char start[12]="";
	char limit[12]="";
	char expexcel[12]="";
	char sdate[32]="";
	char edate[32]="";
	char groupcode[32]="";
	char type[32]="";
	unsigned long lsdate;
	unsigned long ledate;
	char areaid[32]="";
	char groupid[32]="";
	
	// 商家
  char caStype[8] = "";
  char caOptd[64] = "";
  unsigned long lCid = 0;
  char caShopids[1024];
	 strcpy(caShopids,getDsShopids());
  dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
  dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStype)-1,caStype);
  dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
  printf("lCid = %d\n", lCid);
  printf("*******caStype = %s \n",caStype);
  printf("*******caOptd = %s \n",caOptd);
	
	utMsgGetSomeNVar(psMsgHead, 10,"ip1",         UT_TYPE_STRING,  sizeof(keywords)-1,     keywords,
																"start",        UT_TYPE_STRING,  sizeof(start)-1,      	 start,
																"limit",        UT_TYPE_STRING,  sizeof(limit)-1,     	 limit,
																"sdate",        UT_TYPE_STRING,  sizeof(sdate)-1,     	 sdate,
																"edate",        UT_TYPE_STRING,  sizeof(edate)-1,      	 edate,
																"groupcode",    UT_TYPE_STRING,  sizeof(groupcode)-1,    groupcode,
																"areaid",       UT_TYPE_STRING,  sizeof(areaid)-1,       areaid,
																//"groupid",      UT_TYPE_STRING,  sizeof(groupid)-1,      groupid,
																"expexcel",     UT_TYPE_STRING,  sizeof(expexcel)-1,     expexcel,
																"type",         UT_TYPE_STRING,  sizeof(type)-1,      	 type);		
			
	if(strcmp(type,"1")==0){
			char caTemp[1256];
			long lCount1=0;
			
			// 时间转换
			sprintf(sdate, "%s %s", sdate, "00:00:00");
			sprintf(edate, "%s %s", edate, "23:59:59");
			ncTimeToLong(sdate,&lsdate);
			ncTimeToLong(edate,&ledate);
			
			char _sdate[32] = ""; // 格式转换后的开始时间
			char _edate[32] = ""; // 格式转换后的当前时间
			strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lsdate));
			strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", ledate));
			printf("_sdate = %s\n", _sdate);
			printf("_edate = %s\n", _edate);
			
			sprintf(caTemp,"select count(*) from ncsrvuserlog");
			if(strlen(sdate) > 0 && strlen(edate) > 0)//楂樼骇IP
			{
					snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where starttime >= %lu and endtime <= %lu",lsdate,ledate);
			}
			
		if(strlen(caShopids)>0){
	 	   sprintf(caTemp+strlen(caTemp)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	  }
			
			strcpy(caTemp, ncmWebSelect(psShmHead, iFd, psMsgHead, caTemp, lCid, caStype, caOptd, groupcode, areaid, groupid));
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(start),atol(limit));
			pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
			
			sprintf(caTemp,"select devtype, count(devtype) as number from ncsrvuserlog");	
			if(strlen(sdate) > 0 && strlen(edate) > 0)//楂樼骇IP
			{
					snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where starttime >= %lu and endtime <= %lu",lsdate,ledate);
			}
			if(strlen(caShopids)>0){
	 	    sprintf(caTemp+strlen(caTemp)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	    }
			
			strcpy(caTemp, ncmWebSelect(psShmHead, iFd, psMsgHead, caTemp, lCid, caStype, caOptd, groupcode, areaid, groupid));
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by devtype order by number desc");
			if(strlen(start) > 0 && strlen(limit) > 0)	{	
					snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(start),atol(limit));
			}
	printf("caTemp=%s\n",caTemp);		
			psCur = pasDbOpenSql(caTemp,0);
			float total =0;
			if(psCur == NULL){
					printf("pscur is null\n");
					printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
			}
			if(psCur != NULL){
					int iReturn=0;
					char chart_name[32];
					int chart_number=0;
					while(0==(iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(chart_name),chart_name,
											      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
					{
						total += chart_number;
					}
					pasDbCloseCursor(psCur);
			}
			utPltPutVarF(psDbHead,"TotRec","%f", atol(limit));
			
		  //	printf("caTemp=%s\n",caTemp);
		  psCur = pasDbOpenSql(caTemp,0);
			char caFilename[32];
		  char caFile[128];
			char caPath[128];
			FILE *fp;
			if(strlen(expexcel)>0){
				        sprintf(caFilename,"deviceTypeRcount%d.csv",utStrGetId());
				        sprintf(caPath,"%s/html","/home/ncmysql/ncsrv");
				        sprintf(caFile,"%s/%s",caPath,caFilename);
				    	  fp = fopen(caFile,"w");
				        if(fp == NULL) {
				            pasDbCloseCursor(psCur);
							      printf("导出出错\n");
				            return 0;
				        }
				        
				        fprintf(fp,"设备类型统计\n");
								fprintf(fp,"从: %s 到: %s\n",sdate,edate);
				        ncUtlPrintCsv(fp,4,UT_TYPE_STRING,"序号",
				                           UT_TYPE_STRING,"设备类型",
				                           UT_TYPE_STRING,"数量（个）",
										   						 UT_TYPE_STRING,"百分比");                           
			}
			if(psCur == NULL){
					printf("pscur is null\n");
					printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
			}
			if(psCur != NULL){
				int iReturn=0;
				int iNum=0;
				char chart_name[32];
				int chart_number=0;
				float rate=0;
				char rates[32]="";
				while(0==(iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(chart_name),chart_name,
										      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
				{
				
						float rate=0;
						char rates[32]="";
				
						iNum++;
						if(iNum > 1){
								utPltPutLoopVar(psDbHead,"dh",iNum,",");
						}
						utPltPutLoopVarF(psDbHead,"chart_number",iNum,"%lu", chart_number);
						if(strcmp(chart_name,"")==0){strcpy(chart_name,"其它");}
						if(strcmp(chart_name,"Mobile")==0){strcpy(chart_name,"手机");}
						if(strcmp(chart_name,"mspc")==0){strcpy(chart_name,"电脑");}
						if(strcmp(chart_name,"pad")==0){strcpy(chart_name,"平板");}
						utPltPutLoopVarF(psDbHead,"chart_name",iNum,"%s", chart_name);
						utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);
						//printf("chart_number = %lu, lCount1 = %lu, rate = %f \n",chart_number,lCount1,(chart_number/lCount1));	
						rate = chart_number/total;
						sprintf(rates,"%0.2f%%",rate*100);
						utPltPutLoopVarF(psDbHead,"rate",iNum,"%s",rates);
					
						if(strlen(expexcel)>0){
						 		ncUtlPrintCsv(fp,4, UT_TYPE_LONG, iNum,
					                          UT_TYPE_STRING,chart_name,
					                          UT_TYPE_LONG,chart_number,
																		UT_TYPE_STRING,rates);
						}	
				}
				pasDbCloseCursor(psCur);
			}
			if(strlen(expexcel)>0){
			 			fclose(fp);
		        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
		        remove(caFile);
		        utPltFreeDb(psDbHead);
		        return 0;
			}
			//utPltShowDb(psDbHead);
			utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_chart/ncm_device_type_chart.htm");
			printf("ncm_DeviceTypeChart  end!!!!\n");
	}
	if(strcmp(type,"2")==0){
			printf("ncm_DeviceTypeChart  type2 start!!!!\n");
			char caTemp[256];
			long lCount1=0;
			
			// 时间转换
			sprintf(sdate, "%s %s", sdate, "00:00:00");
			sprintf(edate, "%s %s", edate, "23:59:59");
			ncTimeToLong(sdate,&lsdate);
			ncTimeToLong(edate,&ledate);
			
			char _sdate[32] = ""; // 格式转换后的开始时间
			char _edate[32] = ""; // 格式转换后的当前时间
			strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lsdate));
			strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", ledate));
			printf("_sdate = %s\n", _sdate);
			printf("_edate = %s\n", _edate);
			
			sprintf(caTemp,"select count(*) from ncadlog");
			if(strlen(sdate) > 0 && strlen(edate) > 0)//楂樼骇IP
			{
				snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where lasttime >= %lu and lasttime <= %lu",lsdate,ledate);
			}
			
			if(strlen(caShopids)>0){
	 	    sprintf(caTemp+strlen(caTemp)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	    }
			
			strcpy(caTemp, ncmWebSelect(psShmHead, iFd, psMsgHead, caTemp, lCid, caStype, caOptd, groupcode, areaid, groupid));
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(start),atol(limit));
			pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
			
			sprintf(caTemp,"select phtype, count(phtype) as number from ncadlog");	
			if(strlen(sdate) > 0 && strlen(edate) > 0)//楂樼骇IP
			{
				snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where lasttime >= %lu and lasttime <= %lu",lsdate,ledate);
			}
			
			if(strlen(caShopids)>0){
	 	    sprintf(caTemp+strlen(caTemp)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	    }
			
			strcpy(caTemp, ncmWebSelect(psShmHead, iFd, psMsgHead, caTemp, lCid, caStype, caOptd, groupcode, areaid, groupid));
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by phtype order by number desc");
			if(strlen(start) > 0 && strlen(limit) > 0)	{	
				snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(start),atol(limit));
			}
			printf("caTemp = %s\n",caTemp);
			 psCur = pasDbOpenSql(caTemp,0);
			float total =0;
			if(psCur == NULL){
				printf("pscur is null\n");
				printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
			}
			if(psCur != NULL)
			{
			
				int iReturn=0;
				char chart_name[32];
				int chart_number=0;
				while(0==(iReturn = pasDbFetchInto(psCur,
		      	                     UT_TYPE_STRING,sizeof(chart_name),chart_name,
		      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
				{
					total += chart_number;
				}
				
				pasDbCloseCursor(psCur);
			}
			
			utPltPutVarF(psDbHead,"TotRec","%f", total);
			
		  //	printf("caTemp=%s\n",caTemp);
		    psCur = pasDbOpenSql(caTemp,0);
			char caFilename[32];
				char caFile[128];
				char caPath[128];
				FILE *fp;
			if(strlen(expexcel)>0){
				        sprintf(caFilename,"deviceTypeRcount%d.csv",utStrGetId());
				        sprintf(caPath,"%s/html","/home/ncmysql/ncsrv");
				        sprintf(caFile,"%s/%s",caPath,caFilename);
				    	fp = fopen(caFile,"w");
				        if(fp == NULL) {
								
				            pasDbCloseCursor(psCur);
							printf("导出出错\n");
				           utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
				            return 0;
				        }
				        
				        fprintf(fp,"终端类型统计\n");
						fprintf(fp,"从: %s 到: %s\n",sdate,edate);
				        ncUtlPrintCsv(fp,4,
				                           UT_TYPE_STRING,"序号",
				                           UT_TYPE_STRING,"设备类型",
				                           UT_TYPE_STRING,"数量（个）",
										   UT_TYPE_STRING,"百分比");                           
			}
			if(psCur == NULL){
				printf("pscur is null\n");
				printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
			}
			if(psCur != NULL)
			{
			
				int iReturn=0;
				int iNum=0;
				int chart_name=0;
				int chart_number=0;
				float rate=0;
				char rates[32]="";
				char chart_name1[32]="";
				while(0==(iReturn = pasDbFetchInto(psCur,
		      	                     UT_TYPE_LONG,sizeof(chart_name),&chart_name,
		      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
				{
				
				float rate=0;
				char rates[32]="";
				
					iNum++;
					if(iNum > 1)
						{
							utPltPutLoopVar(psDbHead,"dh",iNum,",");
						}
					utPltPutLoopVarF(psDbHead,"chart_number",iNum,"%lu", chart_number);
					utPltPutLoopVarF(psDbHead,"nets_subytes2",iNum,"%d", chart_name);
					if(chart_name ==1){
					strcpy(chart_name1,"平板");
					//utPltPutLoopVarF(psDbHead,"nets_subytes2",iNum,"%s", "平板");
					}
					else if(chart_name ==2){
					strcpy(chart_name1,"PC");
					//utPltPutLoopVarF(psDbHead,"nets_subytes2",iNum,"%s", "PC");
					}
					else if(chart_name ==3){
					strcpy(chart_name1,"手机");
					//utPltPutLoopVarF(psDbHead,"nets_subytes2",iNum,"%s", "手机");
					}
					else{
					strcpy(chart_name1,"其他");
					//utPltPutLoopVarF(psDbHead,"nets_subytes2",iNum,"%s", "其他");
					}
					utPltPutLoopVarF(psDbHead,"chart_name",iNum,"%s", chart_name1);
					
					utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);
					//printf("chart_number = %lu, lCount1 = %lu, rate = %f \n",chart_number,lCount1,(chart_number/lCount1));	
					rate = chart_number/total;
					sprintf(rates,"%0.2f%%",rate*100);
					utPltPutLoopVarF(psDbHead,"rate",iNum,"%s",rates);
					
					
							if(strlen(expexcel)>0){
					
							 ncUtlPrintCsv(fp,4,
		                          UT_TYPE_LONG, iNum,
		                          UT_TYPE_STRING,chart_name1,
		                          UT_TYPE_LONG,chart_number,
									UT_TYPE_STRING,rates);
							 }	
				}
				
				pasDbCloseCursor(psCur);
			}
				if(strlen(expexcel)>0){
			 	fclose(fp);
		        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
		        remove(caFile);
		        utPltFreeDb(psDbHead);
		        return 0;
			 }
			//utPltShowDb(psDbHead);
			utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_chart/ncm_device_type_chart.htm");
			printf("ncm_DeviceTypeChart type2 end!!!!\n");
	}
	//free(p);
	return 0;	  
}

//广告PORTAL端机型报表
int ncm_Ad_DeviceTypeChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead){ // 去除
	
printf("ncm_Ad_DeviceTypeChart start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	char keywords[32] = "";
	char start[12]="";
	char limit[12]="";
	char expexcel[12]="";
	char sdate[32]="";
	char edate[32]="";
	unsigned long lsdate;
	unsigned long ledate;
	
	utMsgGetSomeNVar(psMsgHead, 6,
   				    "ip1",        UT_TYPE_STRING,  sizeof(keywords)-1,       keywords,
					"start",        UT_TYPE_STRING,  sizeof(start)-1,       start,
					"limit",        UT_TYPE_STRING,  sizeof(limit)-1,       limit,
					"sdate",        UT_TYPE_STRING,  sizeof(sdate)-1,       sdate,
					"edate",        UT_TYPE_STRING,  sizeof(edate)-1,       edate,
					"expexcel",     UT_TYPE_STRING,  sizeof(expexcel)-1,       expexcel
					);
	char opt[32]="";
    dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(opt) - 1,opt);
	printf("opt = %s\n",opt);
	char caTemp[256];
	long lCount1=0;
	ncTimeToLong(sdate,&lsdate);
	ncTimeToLong(edate,&ledate);
	sprintf(caTemp,"select count(*) from ncadlog");
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where lasttime >= %lu and lasttime <= %lu",lsdate,ledate);
	}
	if(strlen(opt) > 0)//编码
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and opt = '%s'",opt);
	}
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(start),atol(limit));
	pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
	
//	printf("lCount1 = %lu\n",lCount1);

	sprintf(caTemp,"select phtype, count(phtype) as number from ncadlog");	


	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where lasttime >= %lu and lasttime <= %lu",lsdate,ledate);
	}
	if(strlen(opt) > 0)//编码
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and opt = '%s'",opt);
	}
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by phtype order by number desc");
	if(strlen(start) > 0 && strlen(limit) > 0)	{	
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(start),atol(limit));
	}
	printf("caTemp = %s\n",caTemp);
	 psCur = pasDbOpenSql(caTemp,0);
	float total =0;
	if(psCur == NULL){
		printf("pscur is null\n");
		printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
	
		int iReturn=0;
		char chart_name[32];
		int chart_number=0;
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_STRING,sizeof(chart_name),chart_name,
      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
		{
			total += chart_number;
		}
		
		pasDbCloseCursor(psCur);
	}
	
	utPltPutVarF(psDbHead,"TotRec","%f", total);
	//printf("total = %f \n",total);
	
	
	
	
	
  //	printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	char caFilename[32];
		char caFile[128];
		char caPath[128];
		FILE *fp;
	if(strlen(expexcel)>0){
		        sprintf(caFilename,"deviceTypeRcount%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncsrv");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
						
		            pasDbCloseCursor(psCur);
					printf("导出出错\n");
		           utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"终端类型统计\n");
				fprintf(fp,"从: %s 到: %s\n",sdate,edate);
		        ncUtlPrintCsv(fp,4,
		                           UT_TYPE_STRING,"序号",
		                           UT_TYPE_STRING,"设备类型",
		                           UT_TYPE_STRING,"数量（个）",
								   UT_TYPE_STRING,"百分比");                           
	}	else{

	//iStart = atol(start);
	//iLimit = atol(limit);
  }
	if(psCur == NULL){
		printf("pscur is null\n");
		printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
	
		int iReturn=0;
		int iNum=0;
		int chart_name=0;
		int chart_number=0;
		float rate=0;
		char rates[32]="";
		char chart_name1[32]="";
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,sizeof(chart_name),&chart_name,
      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
		{
		
		float rate=0;
		char rates[32]="";
		
			iNum++;
			if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
			utPltPutLoopVarF(psDbHead,"chart_number",iNum,"%lu", chart_number);
			
			utPltPutLoopVarF(psDbHead,"chart_name",iNum,"%d", chart_name);
			if(chart_name ==1){
			strcpy(chart_name1,"平板");
			//utPltPutLoopVarF(psDbHead,"nets_subytes2",iNum,"%s", "平板");
			}
			else if(chart_name ==2){
			strcpy(chart_name1,"PC");
			//utPltPutLoopVarF(psDbHead,"nets_subytes2",iNum,"%s", "PC");
			}
			else if(chart_name ==3){
			strcpy(chart_name1,"手机");
			//utPltPutLoopVarF(psDbHead,"nets_subytes2",iNum,"%s", "手机");
			}
			else{
			strcpy(chart_name1,"其他");
			//utPltPutLoopVarF(psDbHead,"nets_subytes2",iNum,"%s", "其他");
			}
			utPltPutLoopVarF(psDbHead,"nets_subytes2",iNum,"%s", chart_name1);
			utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);
			//printf("chart_number = %lu, lCount1 = %lu, rate = %f \n",chart_number,lCount1,(chart_number/lCount1));	
			rate = chart_number/total;
			sprintf(rates,"%0.2f%%",rate*100);
			utPltPutLoopVarF(psDbHead,"rate",iNum,"%s",rates);
			
			
					if(strlen(expexcel)>0){
			
					 ncUtlPrintCsv(fp,4,
                          UT_TYPE_LONG, iNum,
                          UT_TYPE_STRING,chart_name1,
                          UT_TYPE_LONG,chart_number,
							UT_TYPE_STRING,rates);
					 }	
		}
		
		pasDbCloseCursor(psCur);
	}
		if(strlen(expexcel)>0){
	 	fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 }
	//utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_chart/ncm_device_type_chart.htm");
	printf("ncm_Ad_DeviceTypeChart  end!!!!\n");
	//free(p);
	return 0;	  
}


//PORTAL端手机品牌报表 终端品牌
int ncm_DeviceBrandChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead){
  printf("ncm_DeviceBrandChart start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	char keywords[32] = "";
	char start[12]="";
	char limit[12]="";
	char expexcel[12]="";
	char sdate[32]="";
	char edate[32]="";
	char groupcode[32]="";
	char type[32]="";
	unsigned long lsdate;
	unsigned long ledate;
	char areaid[32]="";
	char groupid[32]="";
	
	// 商家
  char caStype[8] = "";
  char caOptd[64] = "";
  unsigned long lCid = 0;
  char caShopids[1024];
	strcpy(caShopids,getDsShopids());
  
  dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
  dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStype)-1,caStype);
  dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
  printf("lCid = %d\n", lCid);
  printf("*******caStype = %s \n",caStype);
  printf("*******caOptd = %s \n",caOptd);
	
	utMsgGetSomeNVar(psMsgHead, 11, "ip1",          UT_TYPE_STRING,  sizeof(keywords)-1,    keywords,
																	"start",        UT_TYPE_STRING,  sizeof(start)-1,       start,
																	"limit",        UT_TYPE_STRING,  sizeof(limit)-1,       limit,
																	"sdate",        UT_TYPE_STRING,  sizeof(sdate)-1,       sdate,
																	"edate",        UT_TYPE_STRING,  sizeof(edate)-1,       edate,
																	"expexcel",     UT_TYPE_STRING,  sizeof(expexcel)-1,    expexcel,
																	"groupcode",    UT_TYPE_STRING,  sizeof(groupcode)-1,   groupcode,
																	"areaid",       UT_TYPE_STRING,  sizeof(areaid)-1,      areaid,
																	"groupid",      UT_TYPE_STRING,  sizeof(groupid)-1,     groupid,
																	"type",         UT_TYPE_STRING,  sizeof(type)-1,        type);
	
	if(strcmp(type,"1")==0){
			char caTemp[1256];
			long lCount1=0;
			
			// 时间转换
			sprintf(sdate, "%s %s", sdate, "00:00:00");
			sprintf(edate, "%s %s", edate, "23:59:59");
			ncTimeToLong(sdate,&lsdate);
			ncTimeToLong(edate,&ledate);
			
			char _sdate[32] = ""; // 格式转换后的开始时间
			char _edate[32] = ""; // 格式转换后的当前时间
			strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lsdate));
			strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", ledate));
			printf("_sdate = %s\n", _sdate);
			printf("_edate = %s\n", _edate);
			
			sprintf(caTemp,"select count(*) from ncsrvuserlog");
			if(strlen(sdate) > 0 && strlen(edate) > 0)//楂樼骇IP
			{
					snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where starttime >= %lu and endtime <= %lu",lsdate,ledate);
			}
		if(strlen(caShopids)>0){
	 	sprintf(caTemp+strlen(caTemp)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
			
			strcpy(caTemp, ncmWebSelect(psShmHead, iFd, psMsgHead, caTemp, lCid, caStype, caOptd, groupcode, areaid, groupid));
			pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
			
			sprintf(caTemp,"select dev, count(dev) as number from ncsrvuserlog");	
			if(strlen(sdate) > 0 && strlen(edate) > 0)//楂樼骇IP
			{
					snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where starttime >= %lu and endtime <= %lu",lsdate,ledate);
			}
			strcpy(caTemp, ncmWebSelect(psShmHead, iFd, psMsgHead, caTemp, lCid, caStype, caOptd, groupcode, areaid, groupid));
		if(strlen(caShopids)>0){
	 	sprintf(caTemp+strlen(caTemp)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
			
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by dev order by number desc");
			if(strlen(start) > 0 && strlen(limit) > 0){//楂樼骇IP		
					snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(start),atol(limit));
			}
			printf("caTemp = %s\n",caTemp);
			float total =0 ;
				
			 psCur = pasDbOpenSql(caTemp,0);
			if(psCur == NULL){
				printf("pscur is null\n");
				printf("Do SQL Error, sqlcode is %f \n",  pasDbSqlCode(NULL));
			}
			if(psCur != NULL)
			{
				int iReturn=0;
				char chart_name[32];
				int chart_number=0;
				while(0==(iReturn = pasDbFetchInto(psCur,
		      	                     UT_TYPE_STRING,sizeof(chart_name),chart_name,
		      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
				{	
					total += chart_number;
				}
				
				pasDbCloseCursor(psCur);
			}
			//printf("total = %f\n",total);
			utPltPutVarF(psDbHead,"TotRec","%lu", atol(limit));
		
		 	//printf("caTemp=%s\n",caTemp);
		    psCur = pasDbOpenSql(caTemp,0);
			  char caFilename[32];
				char caFile[128];
				char caPath[128];
				FILE *fp;
			if(strlen(expexcel)>0){
				        sprintf(caFilename,"deviceBrandRcount%d.csv",utStrGetId());
				        sprintf(caPath,"%s/html","/home/ncmysql/ncsrv");
				        sprintf(caFile,"%s/%s",caPath,caFilename);
				    	fp = fopen(caFile,"w");
				        if(fp == NULL) {
		
				            pasDbCloseCursor(psCur);
							printf("导出出错\n");
				           utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
				            return 0;
				        }
				        
				   //     fprintf(fp,"Device Type Recount\n");
				       
				   //    fprintf(fp,"From: %s To: %s\n",sdate,edate);
				      
				        // ncUtlPrintCsv(fp,4,
				                           // UT_TYPE_STRING,"No.",
				                           // UT_TYPE_STRING,"Device Brand",
				                           // UT_TYPE_STRING,"Number",
										   // UT_TYPE_STRING,"Percentage");    
						 fprintf(fp,"设备品牌统计\n");
						 fprintf(fp,"从: %s 到: %s\n",sdate,edate);
						 ncUtlPrintCsv(fp,4,
				                           UT_TYPE_STRING,"序号",
				                           UT_TYPE_STRING,"设备品牌",
				                           UT_TYPE_STRING,"数量（个）",
										   UT_TYPE_STRING,"百分比");    
										   
			}
		
			if(psCur == NULL){
				printf("pscur is null\n");
				printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
			}
			if(psCur != NULL)
			{
			 
				int iReturn=0;
				int iNum=0;
				char chart_name[32];
				int chart_number=0;
				float rate=0;
				//float total = lCount1;
				char rates[32]="";
		
				while(0==(iReturn = pasDbFetchInto(psCur,
		      	                     UT_TYPE_STRING,sizeof(chart_name),chart_name,
		      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
				{	
		
					iNum++;
					if(iNum > 1)
						{
							utPltPutLoopVar(psDbHead,"dh",iNum,",");
						}
					utPltPutLoopVarF(psDbHead,"chart_number",iNum,"%lu", chart_number);
					if(strcmp(chart_name,"")==0){strcpy(chart_name,"其它");}
					utPltPutLoopVarF(psDbHead,"chart_name",iNum,"%s", chart_name);
					utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);
					//printf("chart_number = %lu, lCount1 = %lu, rate = %f \n",chart_number,lCount1,(chart_number/lCount1));	
					rate = chart_number/total;
					sprintf(rates,"%0.2f%%",rate*100);
					utPltPutLoopVarF(psDbHead,"rate",iNum,"%s",rates);
		
					
							if(strlen(expexcel)>0){
		
							 ncUtlPrintCsv(fp,4,
		                          UT_TYPE_LONG, iNum,
		                          UT_TYPE_STRING,chart_name,
		                          UT_TYPE_LONG,chart_number,
									UT_TYPE_STRING,rates);
							 }	
				}
				
				pasDbCloseCursor(psCur);
			}
				if(strlen(expexcel)>0){
			 	fclose(fp);
		        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
		        remove(caFile);
		        utPltFreeDb(psDbHead);
		        return 0;
			 }
		//	utPltShowDb(psDbHead);
			utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_chart/ncm_device_brand_chart.htm");
			printf("ncm_DeviceBrandChart  end!!!!\n");
	}
	
	if(strcmp(type,"2")==0){
			printf("ncm_DeviceBrandChart type2 start!!!!\n");
			char caTemp[256];
			long lCount1=0;
			
			// 时间转换
			sprintf(sdate, "%s %s", sdate, "00:00:00");
			sprintf(edate, "%s %s", edate, "23:59:59");
			ncTimeToLong(sdate,&lsdate);
			ncTimeToLong(edate,&ledate);
			
			char _sdate[32] = ""; // 格式转换后的开始时间
			char _edate[32] = ""; // 格式转换后的当前时间
			strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lsdate));
			strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", ledate));
			printf("_sdate = %s\n", _sdate);
			printf("_edate = %s\n", _edate);
			
			sprintf(caTemp,"select count(*) from ncadlog");
			if(strlen(sdate) > 0 && strlen(edate) > 0)//楂樼骇IP
			{
				snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where lasttime >= %lu and lasttime <= %lu",lsdate,ledate);
			}
			strcpy(caTemp, ncmWebSelect(psShmHead, iFd, psMsgHead, caTemp, lCid, caStype, caOptd, groupcode, areaid, groupid));
			pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
			
			sprintf(caTemp,"select devtype, count(devtype) as number from ncadlog");	
		
			if(strlen(sdate) > 0 && strlen(edate) > 0)//楂樼骇IP
			{
				snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where lasttime >= %lu and lasttime <= %lu",lsdate,ledate);
			}
			strcpy(caTemp, ncmWebSelect(psShmHead, iFd, psMsgHead, caTemp, lCid, caStype, caOptd, groupcode, areaid, groupid));
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by devtype order by number desc");
			if(strlen(start) > 0 && strlen(limit) > 0){//楂樼骇IP		
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(start),atol(limit));
			}
			
			float total =0 ;
			printf("caTemp = %s\n",caTemp);	
			 psCur = pasDbOpenSql(caTemp,0);
			if(psCur == NULL){
				printf("pscur is null\n");
				printf("Do SQL Error, sqlcode is %f \n",  pasDbSqlCode(NULL));
			}
			if(psCur != NULL)
			{
				int iReturn=0;
				char chart_name[32];
				int chart_number=0;
				while(0==(iReturn = pasDbFetchInto(psCur,
		      	                     UT_TYPE_STRING,sizeof(chart_name),chart_name,
		      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
				{	
					total += chart_number;
				}
				
				pasDbCloseCursor(psCur);
			}
			//printf("total = %f\n",total);
			utPltPutVarF(psDbHead,"TotRec","%lu", total);
			
		 	//printf("caTemp=%s\n",caTemp);
		    psCur = pasDbOpenSql(caTemp,0);
			char caFilename[32];
				char caFile[128];
				char caPath[128];
				FILE *fp;
			if(strlen(expexcel)>0){
				        sprintf(caFilename,"deviceBrandRcount%d.csv",utStrGetId());
				        sprintf(caPath,"%s/html","/home/ncmysql/ncsrv");
				        sprintf(caFile,"%s/%s",caPath,caFilename);
				    	fp = fopen(caFile,"w");
				        if(fp == NULL) {
		
				            pasDbCloseCursor(psCur);
							printf("导出出错\n");
				           utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
				            return 0;
				        }
				         
						 fprintf(fp,"终端品牌统计\n");
						 fprintf(fp,"从: %s 到: %s\n",sdate,edate);
						 ncUtlPrintCsv(fp,4,
				                           UT_TYPE_STRING,"序号",
				                           UT_TYPE_STRING,"设备品牌",
				                           UT_TYPE_STRING,"数量（个）",
										   UT_TYPE_STRING,"百分比");    
										   
			}
		
			if(psCur == NULL){
				printf("pscur is null\n");
				printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
			}
			if(psCur != NULL)
			{
				int iReturn=0;
				int iNum=0;
				char chart_name[32];
				int chart_number=0;
				float rate=0;
				//float total = lCount1;
				char rates[32]="";
		
				while(0==(iReturn = pasDbFetchInto(psCur,
		      	                     UT_TYPE_STRING,sizeof(chart_name),chart_name,
		      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
				{	
					iNum++;
					if(iNum > 1)
						{
							utPltPutLoopVar(psDbHead,"dh",iNum,",");
						}
					utPltPutLoopVarF(psDbHead,"chart_number",iNum,"%lu", chart_number);
					if(strcmp(chart_name,"")==0){strcpy(chart_name,"其它");}
					utPltPutLoopVarF(psDbHead,"chart_name",iNum,"%s", chart_name);
					utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);
					//printf("chart_number = %lu, lCount1 = %lu, rate = %f \n",chart_number,lCount1,(chart_number/lCount1));	
					rate = chart_number/total;
					sprintf(rates,"%0.2f%%",rate*100);
					utPltPutLoopVarF(psDbHead,"rate",iNum,"%s",rates);
		
							if(strlen(expexcel)>0){
		
							 ncUtlPrintCsv(fp,4,
		                          UT_TYPE_LONG, iNum,
		                          UT_TYPE_STRING,chart_name,
		                          UT_TYPE_LONG,chart_number,
									UT_TYPE_STRING,rates);
							 }	
				}
				
				pasDbCloseCursor(psCur);
			}
				if(strlen(expexcel)>0){
			 	fclose(fp);
		        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
		        remove(caFile);
		        utPltFreeDb(psDbHead);
		        return 0;
			 }
		//	utPltShowDb(psDbHead);
			utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_chart/ncm_device_type_chart.htm");
			printf("ncm_DeviceBrandChart type2  end!!!!\n");
	}
	//free(p);
	return 0;	  
}

//广告PORTAL端手机品牌报表
int ncm_Ad_DeviceBrandChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead){  // 去除
	
printf("ncm_Ad_DeviceBrandChart start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	char keywords[32] = "";
	char start[12]="";
	char limit[12]="";
	char expexcel[12]="";
	char sdate[32]="";
	char edate[32]="";
	unsigned long lsdate;
	unsigned long ledate;
  char caShopids[1024];
	strcpy(caShopids,getDsShopids());
	utMsgGetSomeNVar(psMsgHead, 6,
   				    "ip1",        UT_TYPE_STRING,  sizeof(keywords)-1,       keywords,
					"start",        UT_TYPE_STRING,  sizeof(start)-1,       start,
					"limit",        UT_TYPE_STRING,  sizeof(limit)-1,       limit,
					"sdate",        UT_TYPE_STRING,  sizeof(sdate)-1,       sdate,
					"edate",        UT_TYPE_STRING,  sizeof(edate)-1,       edate,
					"expexcel",     UT_TYPE_STRING,  sizeof(expexcel)-1,       expexcel
					);
	
	char caTemp[1256];
	long lCount1=0;
	ncTimeToLong(sdate,&lsdate);
	ncTimeToLong(edate,&ledate);
	sprintf(caTemp,"select count(*) from ncadlog");
	if(strlen(sdate) > 0 && strlen(edate) > 0)//楂樼骇IP
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where lasttime >= %lu and lasttime <= %lu",lsdate,ledate);
	}
	
	if(strlen(caShopids)>0){
	 	sprintf(caTemp+strlen(caTemp)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
	
	//snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(start),atol(limit));
	pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
	//utPltPutVarF(psDbHead,"TotRec","%lu", lCount1);
	//printf("lCount1 = %lu\n",lCount1);
	sprintf(caTemp,"select devtype, count(devtype) as number from ncadlog");	

	if(strlen(sdate) > 0 && strlen(edate) > 0)//楂樼骇IP
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where lasttime >= %lu and lasttime <= %lu",lsdate,ledate);
	}
	
	 if(strlen(caShopids)>0){
	 	sprintf(caTemp+strlen(caTemp)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
	
	
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by devtype order by number desc");
	if(strlen(start) > 0 && strlen(limit) > 0){//楂樼骇IP		
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(start),atol(limit));
	}
	
	float total =0 ;
	printf("caTemp = %s\n",caTemp);	
	 psCur = pasDbOpenSql(caTemp,0);
	if(psCur == NULL){
		printf("pscur is null\n");
		printf("Do SQL Error, sqlcode is %f \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		char chart_name[32];
		int chart_number=0;
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_STRING,sizeof(chart_name),chart_name,
      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
		{	
			total += chart_number;
		}
		
		pasDbCloseCursor(psCur);
	}
	//printf("total = %f\n",total);
	utPltPutVarF(psDbHead,"TotRec","%lu", total);
	
	
 	//printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	char caFilename[32];
		char caFile[128];
		char caPath[128];
		FILE *fp;
	if(strlen(expexcel)>0){
		        sprintf(caFilename,"deviceBrandRcount%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncsrv");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {

		            pasDbCloseCursor(psCur);
					printf("导出出错\n");
		           utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		            return 0;
		        }
		        
		   //     fprintf(fp,"Device Type Recount\n");
		       
		   //    fprintf(fp,"From: %s To: %s\n",sdate,edate);
		      
		        // ncUtlPrintCsv(fp,4,
		                           // UT_TYPE_STRING,"No.",
		                           // UT_TYPE_STRING,"Device Brand",
		                           // UT_TYPE_STRING,"Number",
								   // UT_TYPE_STRING,"Percentage");    
				 fprintf(fp,"终端品牌统计\n");
				 fprintf(fp,"从: %s 到: %s\n",sdate,edate);
				 ncUtlPrintCsv(fp,4,
		                           UT_TYPE_STRING,"序号",
		                           UT_TYPE_STRING,"设备品牌",
		                           UT_TYPE_STRING,"数量（个）",
								   UT_TYPE_STRING,"百分比");    
								   
	}

	if(psCur == NULL){
		printf("pscur is null\n");
		printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
	 
		int iReturn=0;
		int iNum=0;
		char chart_name[32];
		int chart_number=0;
		float rate=0;
		//float total = lCount1;
		char rates[32]="";

		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_STRING,sizeof(chart_name),chart_name,
      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
		{	

			iNum++;
			if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
			utPltPutLoopVarF(psDbHead,"chart_number",iNum,"%lu", chart_number);
			if(strcmp(chart_name,"")==0){strcpy(chart_name,"其它");}
			utPltPutLoopVarF(psDbHead,"chart_name",iNum,"%s", chart_name);
			utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);
			//printf("chart_number = %lu, lCount1 = %lu, rate = %f \n",chart_number,lCount1,(chart_number/lCount1));	
			rate = chart_number/total;
			sprintf(rates,"%0.2f%%",rate*100);
			utPltPutLoopVarF(psDbHead,"rate",iNum,"%s",rates);

			
					if(strlen(expexcel)>0){

					 ncUtlPrintCsv(fp,4,
                          UT_TYPE_LONG, iNum,
                          UT_TYPE_STRING,chart_name,
                          UT_TYPE_LONG,chart_number,
							UT_TYPE_STRING,rates);
					 }	
		}
		
		pasDbCloseCursor(psCur);
	}
		if(strlen(expexcel)>0){
	 	fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 }
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_chart/ncm_device_type_chart.htm");
	printf("ncm_Ad_DeviceBrandChart  end!!!!\n");
	//free(p);
	return 0;	  
}

//商家短信TOP10饼图  商家短信
int ncm_GroupSmsChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead){
  printf("ncm_GroupSmsChart start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	
	int iReturn=0;
	char caTemp[256];
	long lCount1=0;
	char keywords[32] = "";
	char start[12]="";
	char limit[12]="";
	char expexcel[12]="";
	char sdate[32]="";
	char edate[32]="";
	char groupcode[32]="";
	char type[32]="";
	unsigned long lsdate;
	unsigned long ledate;
	char areaid[32]="";
	char groupid[32]="";
	
	// 商家
  char caStype[8] = "";
  char caOptd[64] = "";
  unsigned long lCid = 0;
  dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
  dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStype)-1,caStype);
  dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
  printf("lCid = %d\n", lCid);
  printf("*******caStype = %s \n",caStype);
  printf("*******caOptd = %s \n",caOptd);
	
	utMsgGetSomeNVar(psMsgHead, 10, "ip1",          UT_TYPE_STRING,  sizeof(keywords)-1,    keywords,
																	"start",        UT_TYPE_STRING,  sizeof(start)-1,       start,
																	"limit",        UT_TYPE_STRING,  sizeof(limit)-1,       limit,
																	"sdate",        UT_TYPE_STRING,  sizeof(sdate)-1,       sdate,
																	"edate",        UT_TYPE_STRING,  sizeof(edate)-1,       edate,
																	"expexcel",     UT_TYPE_STRING,  sizeof(expexcel)-1,    expexcel,
																	"groupcode",    UT_TYPE_STRING,  sizeof(groupcode)-1,   groupcode,
																	"areaid",       UT_TYPE_STRING,  sizeof(areaid)-1,      areaid,
																	//"groupid",      UT_TYPE_STRING,  sizeof(groupid)-1,   groupid,
																	"type",         UT_TYPE_STRING,  sizeof(type)-1,        type);
	
	// 时间转换
	sprintf(sdate, "%s %s", sdate, "00:00:00");
	sprintf(edate, "%s %s", edate, "23:59:59");
	ncTimeToLong(sdate,&lsdate);
	ncTimeToLong(edate,&ledate);
	
	char _sdate[32] = ""; // 格式转换后的开始时间
	char _edate[32] = ""; // 格式转换后的当前时间
	strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lsdate));
	strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", ledate));
	printf("_sdate = %s\n", _sdate);
	printf("_edate = %s\n", _edate);
	
	sprintf(caTemp,"select count(*) from ncsrvsmslog");
	if(strlen(sdate) > 0 && strlen(edate) > 0)//楂樼骇IP
	{
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where logtime >= %lu and logtime <= %lu",lsdate,ledate);
	}
	strcpy(caTemp, ncmWebSelect(psShmHead, iFd, psMsgHead, caTemp, lCid, caStype, caOptd, groupcode, areaid, groupid));
	pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
	printf("lCount1 = %lu\n",lCount1);
	
	sprintf(caTemp,"select b.groupname, count(a.sid) as number from ncsrvsmslog a,ncsrvgroup b where a.groupid=b.groupid ");	
	if(strlen(sdate) > 0 && strlen(edate) > 0)//楂樼骇IP
	{
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and a.logtime >= %lu and a.logtime <= %lu",lsdate,ledate);
	}
	strcpy(caTemp, ncmWebSelect(psShmHead, iFd, psMsgHead, caTemp, lCid, caStype, caOptd, groupcode, areaid, groupid));
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by a.groupid order by number desc");
	if(strlen(start) > 0 && strlen(limit) > 0){//楂樼骇IP		
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(start),atol(limit));
	}
	printf("caTemp = %s\n",caTemp);
	float total =0 ;
	psCur = pasDbOpenSql(caTemp,0);
	if(psCur == NULL){
		printf("pscur is null\n");
		printf("Do SQL Error, sqlcode is %f \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		char chart_name[32];
		int chart_number=0;
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_STRING,sizeof(chart_name),chart_name,
      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
		{	
			total += chart_number;
		}
		
		pasDbCloseCursor(psCur);
	}
	//printf("total = %f\n",total);
	utPltPutVarF(psDbHead,"TotRec","%lu", atol(limit));
	
	
 	//printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	char caFilename[32];
		char caFile[128];
		char caPath[128];
		FILE *fp;
	if(strlen(expexcel)>0){
		        sprintf(caFilename,"groupSmsRcount%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncsrv");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {

		            pasDbCloseCursor(psCur);
					printf("导出出错\n");
		           utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		            return 0;
		        }
		        
		   //     fprintf(fp,"Device Type Recount\n");
		       
		   //    fprintf(fp,"From: %s To: %s\n",sdate,edate);
		      
		        // ncUtlPrintCsv(fp,4,
		                           // UT_TYPE_STRING,"No.",
		                           // UT_TYPE_STRING,"Device Brand",
		                           // UT_TYPE_STRING,"Number",
								   // UT_TYPE_STRING,"Percentage");    
				 fprintf(fp,"商家短信统计\n");
				 fprintf(fp,"从: %s 到: %s\n",sdate,edate);
				 ncUtlPrintCsv(fp,4,
		                           UT_TYPE_STRING,"序号",
		                           UT_TYPE_STRING,"商家名称",
		                           UT_TYPE_STRING,"短信数量（条）",
								   UT_TYPE_STRING,"百分比");    
								   
	}

	if(psCur == NULL){
		printf("pscur is null\n");
		printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iNum=0;
		char chart_name[32];
		int chart_number=0;
		float rate=0;
		//float total = lCount1;
		char rates[32]="";

		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_STRING,sizeof(chart_name),chart_name,
      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
		{	

			iNum++;
			if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
			utPltPutLoopVarF(psDbHead,"chart_number",iNum,"%lu", chart_number);
			if(strcmp(chart_name,"")==0){strcpy(chart_name,"其它");}
			utPltPutLoopVarF(psDbHead,"chart_name",iNum,"%s", chart_name);
			utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);
			//printf("chart_number = %lu, lCount1 = %lu, rate = %f \n",chart_number,lCount1,(chart_number/lCount1));	
			rate = chart_number/total;
			sprintf(rates,"%0.2f%%",rate*100);
			utPltPutLoopVarF(psDbHead,"rate",iNum,"%s",rates);

			
					if(strlen(expexcel)>0){

					 ncUtlPrintCsv(fp,4,
                          UT_TYPE_LONG, iNum,
                          UT_TYPE_STRING,chart_name,
                          UT_TYPE_LONG,chart_number,
							UT_TYPE_STRING,rates);
					 }	
		}
		
		pasDbCloseCursor(psCur);
	}
		if(strlen(expexcel)>0){
	 	fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 }
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_chart/ncm_device_brand_chart.htm");
	printf("ncm_GroupSmsChart  end!!!!\n");
	
	
	//free(p);
	return 0;	  
}

//加载省市信息
int ncm_areaInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	printf("ncm_areaInfo start!!!!\n");
	 pasDbCursor *psCur;
	 utPltDbHead *psDbHead = utPltInitDb();
	char caTemp[128];
	sprintf(caTemp,"select areaid, name from ncmareainfo");	
  	printf("caTemp=%s\n",caTemp);
	
    psCur = pasDbOpenSql(caTemp,0);
	if(psCur != NULL)
	{
	int iReturn=0;
   	int iNum=0;
	long id =0;
	char name[24];
	while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,8,&id,
      	                     UT_TYPE_STRING,23,name))  || 1405==iReturn)
	 {								   
        iNum++;
      if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
		utPltPutLoopVarF(psDbHead,"name",iNum,"%s",name);
		}
		pasDbCloseCursor(psCur);
	}
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_chart/ncs_area_info_list.htm");
		printf("ncm_areaInfo  end!!!!\n");    
	  return 0;
}


//省市国内国际短信数量统计
int ncm_SMSRecountChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{
	printf("ncm_SMSRecountChart start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	char keywords[32] = "";
	char start[12]="";
	char limit[12]="";
	char expexcel[12]="";
	char sdate[32]="";
	char edate[32]="";
	char sptype[32]="";
	char area[32]="";
	char update[32]="";
	char country[32]="";
	char groupcode[32]="";
	unsigned long lsdate;
	unsigned long ledate;
	char areaid[32]="";
	char groupid[32]="";
			 char caShopids[1024];
	 strcpy(caShopids,getDsShopids());
	// 商家
  char caStype[8] = "";
  char caOptd[64] = "";
  unsigned long lCid = 0;
  dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
  dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStype)-1,caStype);
  dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
  printf("lCid = %d\n", lCid);
  printf("*******caStype = %s \n",caStype);
  printf("*******caOptd = %s \n",caOptd);
	
	utMsgGetSomeNVar(psMsgHead, 13, "ip1",          UT_TYPE_STRING,  sizeof(keywords)-1,    keywords,
																	"start",        UT_TYPE_STRING,  sizeof(start)-1,       start,
																	"limit",        UT_TYPE_STRING,  sizeof(limit)-1,       limit,
																	"sdate",        UT_TYPE_STRING,  sizeof(sdate)-1,       sdate,
																	"edate",        UT_TYPE_STRING,  sizeof(edate)-1,       edate,
																	"sptype",       UT_TYPE_STRING,  sizeof(sptype)-1,      sptype,
																	"area",         UT_TYPE_STRING,  sizeof(area)-1,        area,
																	"country",      UT_TYPE_STRING,  sizeof(country)-1,     country,
																	"update",       UT_TYPE_STRING,  sizeof(update)-1,      update,
																	"groupcode",    UT_TYPE_STRING,  sizeof(groupcode)-1,   groupcode,
																	"areaid",       UT_TYPE_STRING,  sizeof(areaid)-1,      areaid,
																	//"groupid",      UT_TYPE_STRING,  sizeof(groupid)-1,     groupid,
																	"expexcel",     UT_TYPE_STRING,  sizeof(expexcel)-1,    expexcel);
	
	char caTemp[256];
	long lCount1=0;
	
	// 时间转换
	sprintf(sdate, "%s %s", sdate, "00:00:00");
	sprintf(edate, "%s %s", edate, "23:59:59");
	ncTimeToLong(sdate,&lsdate);
	ncTimeToLong(edate,&ledate);
	
	char _sdate[32] = ""; // 格式转换后的开始时间
	char _edate[32] = ""; // 格式转换后的当前时间
	strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lsdate));
	strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", ledate));
	printf("_sdate = %s\n", _sdate);
	printf("_edate = %s\n", _edate);
			
	sprintf(caTemp,"select count(*) from ncsrvsmslog");
	if(strlen(sdate) > 0 && strlen(edate) > 0)//时间
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where logtime >= %lu and logtime <= %lu",lsdate,ledate);
	}
	if(!utStrIsSpaces(sptype) )//运营商
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and sptype = %lu",atol(sptype));
	}
	if(!utStrIsSpaces(area) )//省市
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and areacode = %lu",atol(area));
	}
	if(!utStrIsSpaces(country) )//国家
	{
		if(strcmp(country,"86")==0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and  contrycode = %lu",atol(country));
		}
		else{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and  contrycode != 86");
		}
	}
	if(strlen(caShopids)>0){
	 	sprintf(caTemp+strlen(caTemp)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
	
	strcpy(caTemp, ncmWebSelect(psShmHead, iFd, psMsgHead, caTemp, lCid, caStype, caOptd, groupcode, areaid, groupid));
	pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
	utPltPutVarF(psDbHead,"TotRec","%lu", lCount1);
//	printf("lCount1 = %lu\n",lCount1);

	sprintf(caTemp,"select from_unixtime(logtime,'%H') as atime,count(*) as smsnum from ncsrvsmslog");	
	if(strlen(sdate) > 0 && strlen(edate) > 0)//时间
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where logtime >= %lu and logtime <= %lu",lsdate,ledate);
	}
	if(!utStrIsSpaces(sptype) )//运营商
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and sptype = %lu",atol(sptype));
	}
	if(!utStrIsSpaces(area) )//省市
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and areacode = %lu",atol(area));
	}
	if(!utStrIsSpaces(country) )//省市
	{
		if(strcmp(country,"86")==0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and  contrycode = %lu",atol(country));
		}
		else{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and  contrycode != 86");
		}
	}
	strcpy(caTemp, ncmWebSelect(psShmHead, iFd, psMsgHead, caTemp, lCid, caStype, caOptd, groupcode, areaid, groupid));
	
	if(strlen(caShopids)>0){
	 	sprintf(caTemp+strlen(caTemp)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
	
	
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by atime asc order by atime desc");
	
  	printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	char caFilename[32];
		char caFile[128];
		char caPath[128];
		FILE *fp;
	if(strlen(expexcel)>0){
		        sprintf(caFilename,"deviceTypeRcount%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncsrv");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
						
		            pasDbCloseCursor(psCur);
					printf("导出出错\n");
		           utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"短信数量统计\n");
						fprintf(fp,"从: %s 到: %s\n",sdate,edate);
		        ncUtlPrintCsv(fp,4,UT_TYPE_STRING,"序号",
		                           UT_TYPE_STRING,"时间段（小时）",
		                           UT_TYPE_STRING,"短信数量（条）",
								   						 UT_TYPE_STRING,"百分比");                           
	}
	if(psCur == NULL){
		printf("pscur is null\n");
		printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL){
		int iReturn=0;
		int iNum=0;
		long chart_name=0;
		int chart_number=0;
		float rate=0;
		float total = lCount1;
		char rates[32]="";
			typedef struct sms_pronv{
	    	  long chart_name;
	    	  long chart_number;
	    	  long iNum;
	    	  char rates[32];
	    };
	   struct sms_pronv sms_pronv_s[25];
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,sizeof(chart_name),&chart_name,
      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
		{
		  float rate=0;
		  char rates[32]="";
			iNum++;
			rate = chart_number/total;
			sprintf(rates,"%0.2f%%",rate*100);
			sms_pronv_s[iNum-1].chart_number = chart_number;
			sms_pronv_s[iNum-1].chart_name = chart_name;
			strcpy(sms_pronv_s[iNum-1].rates,rates);		
		}
		int i;
		int j;
		
		for (i = 0;i < 24;i++)
		{
			chart_number =0;
			strcpy(rates,"0%");
			for(j=0; j< iNum ; j++){
				if(sms_pronv_s[j].chart_name == i ){
					strcpy(rates, sms_pronv_s[j].rates);
					chart_number = sms_pronv_s[j].chart_number;
				}
			}
			if(i>0){
			utPltPutLoopVar(psDbHead,"dh",i+1,",");
			}
			utPltPutLoopVarF(psDbHead,"chart_name",i+1,"%d", i);
			utPltPutLoopVarF(psDbHead,"inum",i+1,"%lu", i);
			utPltPutLoopVarF(psDbHead,"chart_number",i+1,"%lu",chart_number);
			utPltPutLoopVarF(psDbHead,"rate",i+1,"%s",rates);
			if(strlen(expexcel)>0){
					 ncUtlPrintCsv(fp,4,
                          UT_TYPE_LONG, i,
                          UT_TYPE_LONG,i,
                          UT_TYPE_LONG,chart_number,
							UT_TYPE_STRING,rates);
					 }	
		}
		
		if(strcmp(update,"update_panel") ==0){
			utPltPutLoopVar(psDbHead,"dh",i+1,",");
			utPltPutLoopVarF(psDbHead,"chart_name",i+1,"%s", "总计");
			utPltPutLoopVarF(psDbHead,"inum",i+1,"%lu", i);
			utPltPutLoopVarF(psDbHead,"chart_number",i+1,"%lu",lCount1);
			utPltPutLoopVarF(psDbHead,"rate",i+1,"%s","100%");
			if(strlen(expexcel)>0){
					 ncUtlPrintCsv(fp,4,
                          UT_TYPE_LONG, i,
                          UT_TYPE_STRING,"总计",
                          UT_TYPE_LONG,lCount1,
							UT_TYPE_STRING,"100%");
					 }	
		}
		pasDbCloseCursor(psCur);
	}
		if(strlen(expexcel)>0){
	 	fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
		}
//	utPltShowDb(psDbHead);
	
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_chart/ncm_sms_recount_chart.htm");
	printf("ncm_SMSRecountChart  end!!!!\n");
	//free(p);
	return 0;	  
}

//广告省市国内国际短信数量统计
int ncm_Ad_SMSRecountChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{
	printf("ncm_Ad_SMSRecountChart start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	char keywords[32] = "";
	char start[12]="";
	char limit[12]="";
	char expexcel[12]="";
	char sdate[32]="";
	char edate[32]="";
	char sptype[32]="";
	char area[32]="";
	char update[32]="";
	char groupcode[32]="";
	char country[32]="";
	unsigned long lsdate;
	unsigned long ledate;
	
	utMsgGetSomeNVar(psMsgHead, 11,
   				    "ip1",        UT_TYPE_STRING,  sizeof(keywords)-1,       keywords,
					"start",        UT_TYPE_STRING,  sizeof(start)-1,       start,
					"limit",        UT_TYPE_STRING,  sizeof(limit)-1,       limit,
					"sdate",        UT_TYPE_STRING,  sizeof(sdate)-1,       sdate,
					"edate",        UT_TYPE_STRING,  sizeof(edate)-1,       edate,
					"sptype",        UT_TYPE_STRING,  sizeof(sptype)-1,       sptype,
					"area",        UT_TYPE_STRING,  sizeof(area)-1,       area,
					"country",        UT_TYPE_STRING,  sizeof(country)-1,       country,
					"update",        UT_TYPE_STRING,  sizeof(update)-1,       update,
					"groupcode",        UT_TYPE_STRING,  sizeof(groupcode)-1,       groupcode,
					"expexcel",     UT_TYPE_STRING,  sizeof(expexcel)-1,       expexcel
					);
	char opt[32]="";
    dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(opt) - 1,opt);
	printf("opt = %s\n",opt);
	char caTemp[256];
	long lCount1=0;
	
	// 时间转换
	sprintf(sdate, "%s %s", sdate, "00:00:00");
	sprintf(edate, "%s %s", edate, "23:59:59");
	ncTimeToLong(sdate,&lsdate);
	ncTimeToLong(edate,&ledate);
	
	char _sdate[32] = ""; // 格式转换后的开始时间
	char _edate[32] = ""; // 格式转换后的当前时间
	strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lsdate));
	strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", ledate));
	printf("_sdate = %s\n", _sdate);
	printf("_edate = %s\n", _edate);
	
	sprintf(caTemp,"select count(*) from ncadlog");
	if(strlen(sdate) > 0 && strlen(edate) > 0)//时间
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where lasttime >= %lu and lasttime <= %lu",lsdate,ledate);
	}
	if(!utStrIsSpaces(opt) )//编码
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and groupcode like '%s%%'",opt);
	}
	if(strlen(groupcode) > 0)//编码
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and groupcode like '%%%s%c'",groupcode,'%');
	}
	if(!utStrIsSpaces(sptype) )//运营商
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and sptype = %lu",atol(sptype));
	}
	if(!utStrIsSpaces(area) )//省市
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and areacode = %lu",atol(area));
	}
	if(!utStrIsSpaces(country) )//省市
	{
		if(strcmp(country,"86")==0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and  contrycode = %lu",atol(country));
		}
		else{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and  contrycode != 86");
		}
	}
	pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
	utPltPutVarF(psDbHead,"TotRec","%lu", lCount1);
//	printf("lCount1 = %lu\n",lCount1);

	sprintf(caTemp,"select from_unixtime(lasttime,'%H') as atime,count(*) as smsnum from ncadlog");	


	if(strlen(sdate) > 0 && strlen(edate) > 0)//时间
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where lasttime >= %lu and lasttime <= %lu",lsdate,ledate);
	}
	if(!utStrIsSpaces(opt) )//编码
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and groupcode like '%s%%'",opt);
	}
	if(strlen(groupcode) > 0)//编码
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and groupcode like '%%%s%c'",groupcode,'%');
	}
	if(!utStrIsSpaces(sptype) )//运营商
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and sptype = %lu",atol(sptype));
	}
	if(!utStrIsSpaces(area) )//省市
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and areacode = %lu",atol(area));
	}
	if(!utStrIsSpaces(country) )//省市
	{
		if(strcmp(country,"86")==0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and  contrycode = %lu",atol(country));
		}
		else{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and  contrycode != 86");
		}
	}
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by atime asc order by atime desc");
	
  	printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	char caFilename[32];
		char caFile[128];
		char caPath[128];
		FILE *fp;
	if(strlen(expexcel)>0){
		        sprintf(caFilename,"deviceTypeRcount%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncsrv");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
						
		            pasDbCloseCursor(psCur);
					printf("导出出错\n");
		           utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"时间段访问次数统计\n");
				fprintf(fp,"从: %s 到: %s\n",sdate,edate);
		        ncUtlPrintCsv(fp,4,
		                           UT_TYPE_STRING,"序号",
		                           UT_TYPE_STRING,"时间段（小时）",
		                           UT_TYPE_STRING,"访问数量（次）",
								   UT_TYPE_STRING,"百分比");                           
	}	else{

	//iStart = atol(start);
	//iLimit = atol(limit);
  }
	if(psCur == NULL){
		printf("pscur is null\n");
		printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
	
		int iReturn=0;
		int iNum=0;
		long chart_name=0;
		int chart_number=0;
		float rate=0;
		float total = lCount1;
		char rates[32]="";
			typedef struct sms_pronv{
	    	  long chart_name;
	    	  long chart_number;
	    	  long iNum;
	    	  char rates[32];
	    };
	   struct sms_pronv sms_pronv_s[25];
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,sizeof(chart_name),&chart_name,
      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
		{
		float rate=0;
		char rates[32]="";
		
			iNum++;
			if(iNum > 1)
				{
				//	utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
		//	utPltPutLoopVarF(psDbHead,"chart_number",iNum,"%lu", chart_number);
		//	if(strcmp(chart_name,"")==0){strcpy(chart_name,"others");}
		//	utPltPutLoopVarF(psDbHead,"chart_name",iNum,"%s", chart_name);
		//	utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);
			//printf("chart_number = %lu, lCount1 = %lu, rate = %f \n",chart_number,lCount1,(chart_number/lCount1));	
			rate = chart_number/total;
			sprintf(rates,"%0.2f%%",rate*100);
		//	utPltPutLoopVarF(psDbHead,"rate",iNum,"%s",rates);
			sms_pronv_s[iNum-1].chart_number = chart_number;
			sms_pronv_s[iNum-1].chart_name = chart_name;
			strcpy(sms_pronv_s[iNum-1].rates,rates);
			
					
		}
		int i;
		int j;
		
		for (i = 0;i < 24;i++)
		{
	//		printf("i=%d\n",i);
			
			
			chart_number =0;
			strcpy(rates,"0%");
			for(j=0; j< iNum ; j++){
	//		printf("sms_pronv_s[j].chart_name =%lu\n",sms_pronv_s[j].chart_name);
			if(sms_pronv_s[j].chart_name == i ){
				strcpy(rates, sms_pronv_s[j].rates);
				chart_number = sms_pronv_s[j].chart_number;
			}
			}
			if(i>0){
			utPltPutLoopVar(psDbHead,"dh",i+1,",");
			}
			utPltPutLoopVarF(psDbHead,"chart_name",i+1,"%d", i);
			utPltPutLoopVarF(psDbHead,"inum",i+1,"%lu", i);
			utPltPutLoopVarF(psDbHead,"chart_number",i+1,"%lu",chart_number);
			utPltPutLoopVarF(psDbHead,"rate",i+1,"%s",rates);
			if(strlen(expexcel)>0){
					 ncUtlPrintCsv(fp,4,
                          UT_TYPE_LONG, i,
                          UT_TYPE_LONG,i,
                          UT_TYPE_LONG,chart_number,
							UT_TYPE_STRING,rates);
					 }	
		
		
		}
		
		if(strcmp(update,"update_panel") ==0){
			utPltPutLoopVar(psDbHead,"dh",i+1,",");
			utPltPutLoopVarF(psDbHead,"chart_name",i+1,"%s", "总计");
			utPltPutLoopVarF(psDbHead,"inum",i+1,"%lu", i);
			utPltPutLoopVarF(psDbHead,"chart_number",i+1,"%lu",lCount1);
			utPltPutLoopVarF(psDbHead,"rate",i+1,"%s","100%");
			if(strlen(expexcel)>0){
					 ncUtlPrintCsv(fp,4,
                          UT_TYPE_LONG, i,
                          UT_TYPE_STRING,"总计",
                          UT_TYPE_LONG,lCount1,
							UT_TYPE_STRING,"100%");
					 }	
		}
		
		
		
		pasDbCloseCursor(psCur);
	}
		if(strlen(expexcel)>0){
	 	fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
		}
//	utPltShowDb(psDbHead);
	
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_chart/ncm_sms_recount_chart.htm");
	printf("ncm_Ad_SMSRecountChart  end!!!!\n");
	//free(p);
	return 0;	  
}



//广告在线用户人数
int ncm_Ad_OnlineUserNumber(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	printf("ncm_Ad_OnlineUserNumber start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	char systime[32] = "";
	char groupcode[32] = "";
	char refresh_time[32] = "";
	utMsgGetSomeNVar(psMsgHead, 3,
   				    "systime",      	  UT_TYPE_STRING,  sizeof(systime)-1,       	systime,
   				    "groupcode",      	  UT_TYPE_STRING,  sizeof(groupcode)-1,       	groupcode,
   				    "refresh_time",       UT_TYPE_STRING,  sizeof(refresh_time)-1,      refresh_time
					);
	
	
	char caTemp[256];
	long lCount1=0;
	long lrefresh_time=0;
	char caNow[32]="";
	int iNum = 0;
	iNum++;
	lrefresh_time = atol(refresh_time)/1000;
	// sprintf(caTemp,"select count(*) as number from ncadlog where lasttime>= %lu and lasttime<= %lu",time(0)-lrefresh_time,time(0));	
	// printf("caTemp = %s\n",caTemp);
	// pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
	// strcpy(caNow,utTimFormat("%M:%S",time(0)));
	// utPltPutLoopVarF(psDbHead,"systime",iNum,"%s",caNow);
	// utPltPutLoopVarF(psDbHead,"regcount",iNum,"%d",lCount1);
	/*测试随机数据*/
	struct totalstate
    {
		long systime;
		int regcount;
	};
	srand( (unsigned)time( NULL ) );
	int number1 = rand()%700+200;
	struct totalstate totalstate[4] = {
	{time(0),number1}
	};
	struct totalstate *p;
	p=totalstate;
			printf("regcount = %d\n",p->regcount);
			printf("systime = %lu\n",p->systime);
			strcpy(caNow,utTimFormat("%M:%S",p->systime));
			utPltPutLoopVarF(psDbHead,"systime",iNum,"%s",caNow);
			utPltPutLoopVarF(psDbHead,"regcount",iNum,"%d",p->regcount);
			

	utPltPutVarF(psDbHead,"TotRec","%d",iNum);
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_chart/ncm_online_user_number_list.htm");
	printf("ncm_Ad_OnlineUserNumber  end!!!!\n");
//	free(p);
	return 0;	  
}




//广告点击次数统计 广告点击
int ncm_Ad_AdVisit(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead){
	
  printf("ncm_Ad_AdVisit start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	
	int iReturn = 0;
	char keywords[32] = "";
	char start[12]="";
	char limit[12]="";
	char expexcel[12]="";
	char sdate[32]="";
	char edate[32]="";
	char groupcode[32]="";
	char username[32]="";
	unsigned long lsdate;
	unsigned long ledate;
	char areaid[32]="";
	char groupid[32]="";
	
	// 商家
  char caStype[8] = "";
  char caOptd[64] = "";
  unsigned long lCid = 0;
  iReturn = dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
  iReturn = dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStype)-1,caStype);
  iReturn = dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
  printf("lCid = %d\n", lCid);
  printf("*******caStype = %s \n",caStype);
  printf("*******caOptd = %s \n",caOptd);
	
	utMsgGetSomeNVar(psMsgHead, 10, "ip1",        	UT_TYPE_STRING,  sizeof(keywords)-1,    keywords,
																	"start",        UT_TYPE_STRING,  sizeof(start)-1,       start,
																	"limit",        UT_TYPE_STRING,  sizeof(limit)-1,       limit,
																	"sdate",        UT_TYPE_STRING,  sizeof(sdate)-1,       sdate,
																	"edate",        UT_TYPE_STRING,  sizeof(edate)-1,       edate,
																	"groupcode",    UT_TYPE_STRING,  sizeof(groupcode)-1,   groupcode,
																	"areaid",       UT_TYPE_STRING,  sizeof(areaid)-1,      areaid,
																	//"groupid",      UT_TYPE_STRING,  sizeof(groupid)-1,     groupid,
																	"username",     UT_TYPE_STRING,  sizeof(username)-1,    username,
																	"expexcel",     UT_TYPE_STRING,  sizeof(expexcel)-1,    expexcel);
					
	char caTemp[256];
	long lCount1=0;
	
	// 时间转换
	sprintf(sdate, "%s %s", sdate, "00:00:00");
	sprintf(edate, "%s %s", edate, "23:59:59");
	ncTimeToLong(sdate,&lsdate);
	ncTimeToLong(edate,&ledate);
	
	char _sdate[32] = ""; // 格式转换后的开始时间
	char _edate[32] = ""; // 格式转换后的当前时间
	strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lsdate));
	strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", ledate));
	printf("_sdate = %s\n", _sdate);
	printf("_edate = %s\n", _edate);
			
	sprintf(caTemp,"select adcod, count(adcod) as number from ncadlog");	

	if(strlen(sdate) > 0 && strlen(edate) > 0)//楂樼骇IP
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where lasttime >= %lu and lasttime <= %lu",lsdate,ledate);
	}
	if(strlen(username) > 0)//用户
	{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and username = '%s' ",username);
	}
	strcpy(caTemp, ncmWebSelect(psShmHead, iFd, psMsgHead, caTemp, lCid, caStype, caOptd, groupcode, areaid, groupid));
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by adcod order by number desc");
	if(strlen(start) > 0 && strlen(limit) > 0){//楂樼骇IP		
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(start),atol(limit));
	}
	
	float total =0 ;
	printf("caTemp = %s\n",caTemp);	
	 psCur = pasDbOpenSql(caTemp,0);
	if(psCur == NULL){
		printf("pscur is null\n");
		printf("Do SQL Error, sqlcode is %f \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		char chart_name[32];
		int chart_number=0;
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_STRING,sizeof(chart_name),chart_name,
      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
		{	
			total += chart_number;
		}
		
		pasDbCloseCursor(psCur);
	}
	//printf("total = %f\n",total);
	utPltPutVarF(psDbHead,"TotRec","%lu", total);
	
	
 	//printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	char caFilename[32];
		char caFile[128];
		char caPath[128];
		FILE *fp;
	if(strlen(expexcel)>0){
		        sprintf(caFilename,"adVisit%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncsrv");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {

		            pasDbCloseCursor(psCur);
					printf("导出出错\n");
		           utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		            return 0;
		        }
		        
		   //     fprintf(fp,"Device Type Recount\n");
		       
		   //    fprintf(fp,"From: %s To: %s\n",sdate,edate);
		      
		        // ncUtlPrintCsv(fp,4,
		                           // UT_TYPE_STRING,"No.",
		                           // UT_TYPE_STRING,"Device Brand",
		                           // UT_TYPE_STRING,"Number",
								   // UT_TYPE_STRING,"Percentage");    
				 fprintf(fp,"设备品牌统计\n");
				 fprintf(fp,"从: %s 到: %s\n",sdate,edate);
				 ncUtlPrintCsv(fp,4,
		                           UT_TYPE_STRING,"序号",
		                           UT_TYPE_STRING,"广告",
		                           UT_TYPE_STRING,"访问（次）",
								   UT_TYPE_STRING,"百分比");    
								   
	}

	if(psCur == NULL){
		printf("pscur is null\n");
		printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iNum=0;
		char chart_name[32];
		int chart_number=0;
		float rate=0;
		//float total = lCount1;
		char rates[32]="";

		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_STRING,sizeof(chart_name),chart_name,
      	                     UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
		{	

			iNum++;
			if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
			utPltPutLoopVarF(psDbHead,"chart_number",iNum,"%lu", chart_number);
			if(strcmp(chart_name,"")==0){strcpy(chart_name,"其它");}
			utPltPutLoopVarF(psDbHead,"chart_name",iNum,"%s", chart_name);
			utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);
			//printf("chart_number = %lu, lCount1 = %lu, rate = %f \n",chart_number,lCount1,(chart_number/lCount1));	
			rate = chart_number/total;
			sprintf(rates,"%0.2f%%",rate*100);
			utPltPutLoopVarF(psDbHead,"rate",iNum,"%s",rates);

			
					if(strlen(expexcel)>0){

					 ncUtlPrintCsv(fp,4,
                          UT_TYPE_LONG, iNum,
                          UT_TYPE_STRING,chart_name,
                          UT_TYPE_LONG,chart_number,
						UT_TYPE_STRING,rates);
					 }	
		}
		
		pasDbCloseCursor(psCur);
	}
		if(strlen(expexcel)>0){
	 	fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 }
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_chart/ncm_device_type_chart.htm");
	printf("ncm_Ad_AdVisit  end!!!!\n");
	//free(p);
	return 0;	  
}



//wubin函数注册
int Ncm_MaintainCenterWB_SetFunName()
{
    pasSetTcpFunName("ncmReportReset",ncmReportReset,0); //报表分发设置
//	pasSetTcpFunName("ncm_Totalstate",ncm_Totalstate,0); //总体运行状况
	pasSetTcpFunName("ncmGetWeekDay",ncmGetWeekDay,0); 
	pasSetTcpFunName("ncmWriteReportLog",ncmWriteReportLog,0); //写入报表发送日志

	pasSetTcpFunName("ncmMailAttachSend",ncmMailAttachSend,0); //邮件附件发送
	pasSetTcpFunName("ncm_ReportSendLog",ncm_ReportSendLog,0); //报表分发日志
	pasSetTcpFunName("ncm_ShowSmsStatisticsReport",ncm_ShowSmsStatisticsReport,0); //短信统计报表
	pasSetTcpFunName("ncm_MailSendSave_par",ncm_MailSendSave_par,0); //邮件分发参数设置
	pasSetTcpFunName("ncm_MailSendSet_par",ncm_MailSendSet_par,0); //邮件分发参数设定
	
	pasSetTcpFunName("ncm_areaInfo",ncm_areaInfo,0); //省市信息
	
	pasSetTcpFunName("ncm_DeviceTypeChart",ncm_DeviceTypeChart,0); //PORTAL端机型报表
	pasSetTcpFunName("ncm_DeviceBrandChart",ncm_DeviceBrandChart,0); //PORTAL端手机品牌报表
	pasSetTcpFunName("ncm_SMSRecountChart",ncm_SMSRecountChart,0); //省市短信国际国内短信统计
	pasSetTcpFunName("ncm_GroupSmsChart",ncm_GroupSmsChart,0); //商家短信统计
	
	
	pasSetTcpFunName("ncm_Ad_DeviceTypeChart",ncm_Ad_DeviceTypeChart,0); //广告终端类别
	pasSetTcpFunName("ncm_Ad_DeviceBrandChart",ncm_Ad_DeviceBrandChart,0); //广告终端品牌
	pasSetTcpFunName("ncm_Ad_SMSRecountChart",ncm_Ad_SMSRecountChart,0); //广告时间段点击统计
	pasSetTcpFunName("ncm_Ad_OnlineUserNumber",ncm_Ad_OnlineUserNumber,0); //广告在线用户人数
	pasSetTcpFunName("ncm_Ad_AdVisit",ncm_Ad_AdVisit,0); //广告点击
    return 0;
}