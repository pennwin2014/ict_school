#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include <pcap.h>
#include <errno.h>

#include <iconv.h>

#include <dirent.h>


#include "ncportalweb.h"



//国内国际短信统计
int ncm_ShowSmsStatisticsReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead,char sdate[32],char edate[32],char makepath[255])
{
		  pasDbCursor *psCur;
	    char sqlbuf[4024];
	    utPltDbHead *psDbHead;
	    unsigned long lTime;
	    unsigned long long lBytes;
	    long iNum;
	    long long lSum;
	    char caTemp[10240];
	    int iReturn,i;
	    char caOrder[16];
	    long lGroupid0;
	    char caExport[12];
	    char caSum[12];
	    char caSdate[16],caEdate[16],caShour[8],caEhour[8],caGroupid[16],caGitem[12];
	    char caSdate_temp[20],caEdate_temp[20],caUnit[16];
	    long lStime,lEtime,lConntime;
	    long lSumShow;
	    struct ncFlowSort_s {
	        unsigned long lId;
	        unsigned long long lBytes;
	        unsigned long lTime;
	        char uname[32];
	        char ip[15];
	    };
	    struct ncFlowSort_s *pFlow;
//	     ncUserCont *psUser;
	    long long lSumbytes;
	    long lSumtime,lGitem,lGroupid,m;
	    char caServ_flag[12],caName[32],caPdf[32];
	    struct piechart_datasetf *pChart;           //用于PDF文档输出
	
	    int rowrow;                     
	    int column; 
	    char caTitle[128];
	    char filename[128];
	    column=4;
	    char **desc;
	    char caStatdate[128];
	    char caUsername[32],caIp[20],caGroupname[32];
	    lTime=time(0);
	    lSum=0;
	    
	    char **ptmp = NULL;	
	    unsigned long lStartTime = 0,gNum=0,index=0,gid=0,iret=0,maxgcou=0;
	    int lCount = 0,days=0,lRecenttime=0;
	    lSum=0;
	    lRecenttime=0;
	    char uName[32]="",ip[15]="",gname[32]="",tmpGname[64]="";
	    struct group_s
		  {
				char gname[32];
				unsigned long gid;
		  };
		  struct group_s *group_str;
		  char trGname[128]="",caTemp_d[256]="",caGroupcode[16]=""; 
		  
		  char *pHash;
		  unsigned char *pHash_dep;
	    pasLHashInfo sHashInfo,sHashInfo2;
	    struct ap_s{
	    	long apid;
	    	char apname[64];
	    };
	    struct ap_s *psData;
	    long hCount=0;
	    
	    //
	    typedef struct sms_stat{
	        int num;
	        char timeV[18];
	        long localNum;
	        long interNum;	
	        long mobileNum;
	        long unicomNum;
	        long telcomNum;
	    }sms_stat_s;
	    sms_stat_s stat_sms[24];
	    sms_stat_s *stat_tmp;
	    
	    
	    typedef struct sms_pronv{
	    	  char pronvence[64];
	    	  long sunmobile;
	    	  long sumunicom;
	    	  long sumtelcom;
	      	sms_stat_s *sms_cont;
	    }sms_pronv_s;
	    sms_pronv_s  *pronv_sms;
	    
	    long sms_num = 0,sms_time = 0,sumlocal = 0;
	    char sms_code[20] = "";
	    
	    int sumForeign = 0, sumMobile = 0, sumUnicom = 0, sumTelecom = 0;
	    
	    iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
	    if(iReturn != 0) lGroupid0 = 0;
      iReturn = pasConnect(psShmHead);

     char mysdate[32] = "", myedate[32] = "", mymakepath[255] = "";
		 printf("xxxxx\n");
//		 strcpy(sdate,"2014-02-24 00:00:00");
//	   strcpy(edate,"2014-02-24 23:59:59");
//	   strcpy(makepath,"/db/applereport/2014-02-24/smsStatisticsReport.csv");
		 
		
		strcpy(mysdate,sdate);
	   strcpy(myedate,edate);
	   strcpy(mymakepath,makepath);
	   printf("mysdate = %s\n",mysdate);
		printf("myedate = %s\n",myedate);
		printf("mymakepath = %s\n",mymakepath);
		 //init 最终数组
		 for (i = 0;i < 24;i++)
		 {
		 	  stat_sms[i].num = i;
		    sprintf(stat_sms[i].timeV,"%02d:00-%02d:59",i,i);
		    stat_sms[i].localNum = 0;
		    stat_sms[i].interNum = 0;	
		    stat_sms[i].mobileNum = 0;	
		    stat_sms[i].unicomNum = 0;	
		    stat_sms[i].telcomNum = 0;	
//		    printf("num = %d timeV = %s localNum = %d interNum = %d \n",stat_sms[i].num,stat_sms[i].timeV,stat_sms[i].localNum,stat_sms[i].interNum);
		 }				
  
	  
  	
     lStime=utTimStrToLong("%Y/%m/%d %H:%M:%S",mysdate);
     lEtime=utTimStrToLong("%Y/%m/%d %H:%M:%S",myedate);
     printf("caSdate=%s  caEdate=%s \n",caSdate,caEdate);
     

	   lGitem=4;                                                
     sprintf(sqlbuf,"select count(*) as smsnum,from_unixtime(logtime,'%H') as atime,servicecode from ncsrvsmslog b where  contrycode = 086 ");	
		 sprintf(sqlbuf+strlen(sqlbuf)," and b.logtime>=%lu and b.logtime<=%lu ",lStime,lEtime);
	   sprintf(sqlbuf+strlen(sqlbuf)," group by atime asc  ");
     

     strcpy(caUnit,"条");
     sprintf(sqlbuf+strlen(sqlbuf)," order by atime desc  ");
//  	 printf("11sqlbuf=%s\n",sqlbuf);
  	
     psCur=pasDbOpenSql(sqlbuf,0);
     
     if(psCur == NULL) {
        return 0;
     }
      
     iReturn = 0;
     sms_num=0;
     sms_time=0;
     memset(sms_code,0,sizeof(sms_code));
     iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
                                    UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
                                    UT_TYPE_STRING,sizeof(sms_code),sms_code); 
                  
	   iNum=0;
	   while(iReturn==0||iReturn==1405)
	   {
		    stat_sms[sms_time].localNum = sms_num;
		    sumlocal += sms_num;  
	     	iNum++;
	     	sms_num=0;
		    sms_time=0;
		    memset(sms_code,0,sizeof(sms_code));
        iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
	                                     UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
	                                     UT_TYPE_STRING,sizeof(sms_code),sms_code);           
	   }
	   pasDbCloseCursor(psCur); 
    
     
		 //国际短信统计
		 sprintf(sqlbuf,"select count(*) as smsnum,from_unixtime(logtime,'%H') as atime,servicecode from ncsrvsmslog b where  contrycode != 086 ");	
		 sprintf(sqlbuf+strlen(sqlbuf)," and b.logtime>=%lu and b.logtime<=%lu ",lStime,lEtime);
	   sprintf(sqlbuf+strlen(sqlbuf)," group by atime asc  ");
	 
     strcpy(caUnit,"条");
     sprintf(sqlbuf+strlen(sqlbuf)," order by atime desc ");
//  	 printf("12sqlbuf=%s\n",sqlbuf);
     psCur=pasDbOpenSql(sqlbuf,0);
     if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
        return 0;
     }
     iReturn = 0;
     sms_num=0;
     sms_time=0;
     memset(sms_code,0,sizeof(sms_code));
     iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
                                    UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
                                    UT_TYPE_STRING,sizeof(sms_code),sms_code);                               
	  iNum=0;
	  while(iReturn==0||iReturn==1405)
	  {
		    stat_sms[sms_time].interNum = sms_num;
		    sumForeign += sms_num;
	     	iNum++;
	     	sms_num=0;
		    sms_time=0;
		    memset(sms_code,0,sizeof(sms_code));
        iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
	                                     UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
	                                     UT_TYPE_STRING,sizeof(sms_code),sms_code);           
	  }
	  pasDbCloseCursor(psCur); 
    
    
    //移动
    sprintf(sqlbuf,"select count(*) as smsnum,from_unixtime(logtime,'%H') as atime,servicecode from ncsrvsmslog b where  sptype = 1 ");	
		 sprintf(sqlbuf+strlen(sqlbuf)," and b.logtime>=%lu and b.logtime<=%lu ",lStime,lEtime);
	   sprintf(sqlbuf+strlen(sqlbuf)," group by atime asc  ");
	 
     strcpy(caUnit,"条");
     sprintf(sqlbuf+strlen(sqlbuf)," order by atime desc  ");
//  	 printf("mobileNum sqlbuf=%s\n",sqlbuf);
  	
     psCur=pasDbOpenSql(sqlbuf,0);
     if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
        return 0;
     }
     iReturn = 0;
     sms_num=0;
     sms_time=0;
     memset(sms_code,0,sizeof(sms_code));
     iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
                                    UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
                                    UT_TYPE_STRING,sizeof(sms_code),sms_code);                               
	  iNum=0;
	  while(iReturn==0||iReturn==1405)
	  {
		    stat_sms[sms_time].mobileNum = sms_num;
		    sumMobile += sms_num;
	     	iNum++;
	     	sms_num=0;
		    sms_time=0;
		    memset(sms_code,0,sizeof(sms_code));
        iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
	                                     UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
	                                     UT_TYPE_STRING,sizeof(sms_code),sms_code);           
	  }
	  pasDbCloseCursor(psCur); 
    
    //联通
    sprintf(sqlbuf,"select count(*) as smsnum,from_unixtime(logtime,'%H') as atime,servicecode from ncsrvsmslog b where  sptype = 2 ");	
		 sprintf(sqlbuf+strlen(sqlbuf)," and b.logtime>=%lu and b.logtime<=%lu ",lStime,lEtime);
	   sprintf(sqlbuf+strlen(sqlbuf)," group by atime asc  ");
	 
     strcpy(caUnit,"条");
     strcpy(caOrder,ncLang("0369流量"));
     sprintf(sqlbuf+strlen(sqlbuf)," order by atime desc  ");
//  	 printf("unicomNum  sqlbuf=%s\n",sqlbuf);
  	
     psCur=pasDbOpenSql(sqlbuf,0);
     if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
        return 0;
     }
     iReturn = 0;
     sms_num=0;
     sms_time=0;
     memset(sms_code,0,sizeof(sms_code));
     iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
                                    UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
                                    UT_TYPE_STRING,sizeof(sms_code),sms_code);                               
	  iNum=0;
	  while(iReturn==0||iReturn==1405)
	  {
		    stat_sms[sms_time].unicomNum = sms_num;
		    sumUnicom += sms_num;
	     	iNum++;
	     	sms_num=0;
		    sms_time=0;
		    memset(sms_code,0,sizeof(sms_code));
        iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
	                                     UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
	                                     UT_TYPE_STRING,sizeof(sms_code),sms_code);           
	  }
	  pasDbCloseCursor(psCur); 
    
    //电信
    sprintf(sqlbuf,"select count(*) as smsnum,from_unixtime(logtime,'%H') as atime,servicecode from ncsrvsmslog b where  sptype = 3 ");	
		 sprintf(sqlbuf+strlen(sqlbuf)," and b.logtime>=%lu and b.logtime<=%lu ",lStime,lEtime);
	   sprintf(sqlbuf+strlen(sqlbuf)," group by atime asc  ");
	 
     strcpy(caUnit,"条");
     sprintf(sqlbuf+strlen(sqlbuf)," order by atime desc  ");
//  	 printf("telcomNum sqlbuf=%s\n",sqlbuf);
  	
     psCur=pasDbOpenSql(sqlbuf,0);
     if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
        return 0;
     }
     iReturn = 0;
     sms_num=0;
     sms_time=0;
     memset(sms_code,0,sizeof(sms_code));
     iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
                                    UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
                                    UT_TYPE_STRING,sizeof(sms_code),sms_code);                               
	  iNum=0;
	  while(iReturn==0||iReturn==1405)
	  {
		    stat_sms[sms_time].telcomNum = sms_num;
		    sumTelecom += sms_num;
	     	iNum++;
	     	sms_num=0;
		    sms_time=0;
		    memset(sms_code,0,sizeof(sms_code));
        iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
	                                     UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
	                                     UT_TYPE_STRING,sizeof(sms_code),sms_code);           
	  }
	  pasDbCloseCursor(psCur); 
    
    //**************************************************各省明细
    int pronCou = 0,t = 0, k = 0;
    char pron_name[64] = "";
    /*从配置行政表来*/
    sprintf(sqlbuf,"select count(*) from ncmareainfo  ");
    pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&pronCou);
	  pronv_sms = (sms_pronv_s *)malloc(sizeof(sms_pronv_s)*pronCou+1);    
	  sprintf(sqlbuf,"select name from ncmareainfo  where  1 = 1 ");
    psCur=pasDbOpenSql(sqlbuf,0);
    if(psCur == NULL) {
        return 0;
    }
    iReturn = 0;
    memset(pron_name,0,sizeof(pron_name));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(pron_name) - 1,pron_name);                               
	  iNum=0;
	  while(iReturn==0||iReturn==1405)
	  {
		    strcpy(pronv_sms[iNum].pronvence,pron_name);
	     	iNum++;
		    memset(pron_name,0,sizeof(pron_name));
        iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(pron_name) - 1,pron_name);               
	  }
	  pasDbCloseCursor(psCur); 
	  
	  
	  
	  
    //省份明细
    for (t = 0; t <pronCou; t++)
    {  
//       printf(" 0**************come here %s \n",pronv_sms[t].pronvence);
       
       pronv_sms[t].sunmobile = 0;
       pronv_sms[t].sumunicom = 0;
       pronv_sms[t].sumtelcom = 0;
       
       pronv_sms[t].sms_cont = (sms_stat_s *)malloc(sizeof(sms_stat_s)*24+1);

       for (i = 0;i < 24;i++)
		   {
			 	  pronv_sms[t].sms_cont[i].num = i;
			    sprintf(pronv_sms[t].sms_cont[i].timeV,"%02d:00-%02d:59",i,i);
			    pronv_sms[t].sms_cont[i].localNum = 0;
			    pronv_sms[t].sms_cont[i].interNum = 0;	
			    pronv_sms[t].sms_cont[i].mobileNum = 0;	
			    pronv_sms[t].sms_cont[i].unicomNum = 0;	
			    pronv_sms[t].sms_cont[i].telcomNum = 0;	
	//		    printf("num = %d timeV = %s localNum = %d interNum = %d \n",stat_sms[i].num,stat_sms[i].timeV,stat_sms[i].localNum,stat_sms[i].interNum);
			 }			
    }
//   printf("pronCou = %d \n",pronCou);
    //填入各省份数据
    for (t = 0; t <pronCou; t++)
    { 
    	 //移动
	     sprintf(sqlbuf,"select count(*) as smsnum,from_unixtime(logtime,'%H') as atime,servicecode from ncsrvsmslog b where  sptype = 1  ");	
			 sprintf(sqlbuf+strlen(sqlbuf)," and b.logtime>=%lu and b.logtime<=%lu ",lStime,lEtime);
			 sprintf(sqlbuf+strlen(sqlbuf)," and areaname = '%s'  ",pronv_sms[t].pronvence);
		   sprintf(sqlbuf+strlen(sqlbuf)," group by atime asc  ");
		 
	     strcpy(caUnit,"条");
	     sprintf(sqlbuf+strlen(sqlbuf)," order by atime desc  ");
//	  	 printf("mobileNum sqlbuf=%s\n",sqlbuf);
	  	
	     psCur=pasDbOpenSql(sqlbuf,0);
	     if(psCur == NULL) {
	        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
	        return 0;
	     }
	     iReturn = 0;
	     sms_num=0;
	     sms_time=0;
	     memset(sms_code,0,sizeof(sms_code));
	     iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
	                                    UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
	                                    UT_TYPE_STRING,sizeof(sms_code),sms_code);                               
		  iNum=0;
		  while(iReturn==0||iReturn==1405)
		  {
			    pronv_sms[t].sms_cont[sms_time].mobileNum = sms_num;
			    pronv_sms[t].sunmobile += sms_num;
		     	iNum++;
		     	sms_num=0;
			    sms_time=0;
			    memset(sms_code,0,sizeof(sms_code));
	        iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
		                                     UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
		                                     UT_TYPE_STRING,sizeof(sms_code),sms_code);           
		  }
		  pasDbCloseCursor(psCur); 
	    
	    //联通
	    sprintf(sqlbuf,"select count(*) as smsnum,from_unixtime(logtime,'%H') as atime,servicecode from ncsrvsmslog b where  sptype = 2 ");	
			sprintf(sqlbuf+strlen(sqlbuf)," and b.logtime>=%lu and b.logtime<=%lu ",lStime,lEtime);
			sprintf(sqlbuf+strlen(sqlbuf)," and areaname = '%s'  ",pronv_sms[t].pronvence);
		  sprintf(sqlbuf+strlen(sqlbuf)," group by atime asc  ");
		 
	    strcpy(caUnit,"条");
	    sprintf(sqlbuf+strlen(sqlbuf)," order by atime desc  ");
//  	 printf("unicomNum  sqlbuf=%s\n",sqlbuf);
	  	
	    psCur=pasDbOpenSql(sqlbuf,0);
	    if(psCur == NULL) {
	        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
	        return 0;
	    }
	    iReturn = 0;
	    sms_num=0;
	    sms_time=0;
	    memset(sms_code,0,sizeof(sms_code));
	    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
	                                   UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
	                                   UT_TYPE_STRING,sizeof(sms_code),sms_code);                               
		  iNum=0;
		  while(iReturn==0||iReturn==1405)
		  {
		  	  pronv_sms[t].sms_cont[sms_time].unicomNum = sms_num;
			    pronv_sms[t].sumunicom += sms_num;
		     	iNum++;
		     	sms_num=0;
			    sms_time=0;
			    memset(sms_code,0,sizeof(sms_code));
	        iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
		                                     UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
		                                     UT_TYPE_STRING,sizeof(sms_code),sms_code);           
		  }
		  pasDbCloseCursor(psCur); 
	   
	    //电信
	    sprintf(sqlbuf,"select count(*) as smsnum,from_unixtime(logtime,'%H') as atime,servicecode from ncsrvsmslog b where  sptype = 3 ");	
			sprintf(sqlbuf+strlen(sqlbuf)," and b.logtime>=%lu and b.logtime<=%lu ",lStime,lEtime);
			sprintf(sqlbuf+strlen(sqlbuf)," and areaname = '%s'  ",pronv_sms[t].pronvence);
		  sprintf(sqlbuf+strlen(sqlbuf)," group by atime asc  ");
		 
	    strcpy(caUnit,"条");
	    sprintf(sqlbuf+strlen(sqlbuf)," order by atime desc  ");
//	  	 printf("333telcomNum sqlbuf=%s\n",sqlbuf);
	  	
	    psCur=pasDbOpenSql(sqlbuf,0);
	    if(psCur == NULL) {
	        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
	        return 0;
	    }
	    iReturn = 0;
	    sms_num=0;
	    sms_time=0;
	    memset(sms_code,0,sizeof(sms_code));
	    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
	                                   UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
	                                   UT_TYPE_STRING,sizeof(sms_code),sms_code);                               
		  iNum=0;
		  while(iReturn==0||iReturn==1405)
		  {
		  	  pronv_sms[t].sms_cont[sms_time].telcomNum = sms_num;
			    pronv_sms[t].sumtelcom += sms_num;
		     	iNum++;
		     	sms_num=0;
			    sms_time=0;
			    memset(sms_code,0,sizeof(sms_code));
	        iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
		                                     UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
		                                     UT_TYPE_STRING,sizeof(sms_code),sms_code);           
		  }
		  pasDbCloseCursor(psCur);  
		 
    }	
    
	  int mNum=24;

		FILE *fp;
	  char caFilename[32];
	  char caFile[128];
	  char caPath[128];
	 
//		printf("mymakepath = %s \n",mymakepath);
		strcpy(caFile,mymakepath);
	  fp = fopen(caFile,"w");
	  if(fp == NULL) {
	            utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","短信查询",ncLang("0546数据导出出错"));
	            return 0;
	  }
//	        for(i=0;i<mNum;i++)
//					{
//						  printf("timeV = %s localNum = %d interNum = %d mobileNum = %d unicomNum = %d telcomNum =%d  \n",stat_sms[i].timeV,stat_sms[i].localNum,stat_sms[i].interNum,stat_sms[i].mobileNum,stat_sms[i].unicomNum,stat_sms[i].telcomNum);
//					}
	        
//	  printf(" fuck 1111  sdate = %s  edate = %s\n",mysdate,myedate);
//	  fprintf(fp,ncLang("2037国内国际运营商短信发送数量(%s--%s)\n"),mysdate,myedate);
	  fprintf(fp,"SMS Statistics Report(%s--%s)\n",mysdate,myedate);
		fprintf(fp,"Time,Local,Inter,CM,CU,CT\n");
//		fprintf(fp,"时间,国内短信数量,国际短信数量,移动短信数量,联通短信数量,电信短信数量\n");
		for(i=0;i<mNum;i++)
		{
//				printf("timeV = %s localNum = %d interNum = %d mobileNum = %d unicomNum = %d telcomNum =%d  \n",stat_sms[i].timeV,stat_sms[i].localNum,stat_sms[i].interNum,stat_sms[i].mobileNum,stat_sms[i].unicomNum,stat_sms[i].telcomNum);
					fprintf(fp,"\"%s\",%d,%d,%d,%d,%d\n",stat_sms[i].timeV,stat_sms[i].localNum,stat_sms[i].interNum,stat_sms[i].mobileNum,stat_sms[i].unicomNum,stat_sms[i].telcomNum);
		}
		//汇总
//		fprintf(fp,"\"%s\",%d,%d,%d,%d,%d\n","总计",sumlocal,sumForeign,sumMobile,sumUnicom,sumTelecom);
		fprintf(fp,"\"%s\",%d,%d,%d,%d,%d\n","Total",sumlocal,sumForeign,sumMobile,sumUnicom,sumTelecom);
		fprintf(fp,"\n");
		//明细报表
		//做格式  表头
		for (t = 0; t <pronCou; t++)
    {
//    	printf("pronvence = %s summobile = %d sumunicome = %d suntelcome = %d\n",pronv_sms[t].pronvence,pronv_sms[t].sunmobile,pronv_sms[t].sumunicom,pronv_sms[t].sumtelcom);
    	if (t == 0)
    	{
    	   fprintf(fp,"\t\r,");	
    	}	
    	fprintf(fp,"%s,\t,\t,",pronv_sms[t].pronvence);
    	
    }
    fprintf(fp,"\n");
    for (t = 0; t <pronCou; t++)
    {  
    	 if (t == 0)
    	 {
    	   fprintf(fp,"\t\r,");	
    	 }	
//       fprintf(fp,"移动,联通,电信,");
       fprintf(fp,"CM,CU,CT,");
    }
    fprintf(fp,"\n");
    //内容
	
    for (i = 0;i < 24;i++)
		{
      for (t = 0; t <pronCou; t++)
      {
		  	if (t == 0)
		  	{	
		  	  fprintf(fp,"%s,%d,%d,%d,",pronv_sms[t].sms_cont[i].timeV,pronv_sms[t].sms_cont[i].mobileNum,pronv_sms[t].sms_cont[i].unicomNum,pronv_sms[t].sms_cont[i].telcomNum);
		    }
		    else
		    {
		    	
		    	fprintf(fp,"%d,%d,%d,",pronv_sms[t].sms_cont[i].mobileNum,pronv_sms[t].sms_cont[i].unicomNum,pronv_sms[t].sms_cont[i].telcomNum);
		    }	
//		  	printf("%s moblie = %d unicom = %d telcom = %d \n",pronv_sms[t].sms_cont[i].timeV,pronv_sms[t].sms_cont[i].mobileNum,pronv_sms[t].sms_cont[i].unicomNum,pronv_sms[t].sms_cont[i].telcomNum);
		  }	
		  fprintf(fp,"\n");
		  
    }
    
		//总计
		fprintf(fp,"\n");
		for (t = 0; t <pronCou; t++)
    {
    	  if (t == 0){
    	  	  fprintf(fp,"%s,%d,%d,%d,","Total",pronv_sms[t].sunmobile,pronv_sms[t].sumunicom,pronv_sms[t].sumtelcom);
//    	      fprintf(fp,"%s,%d,%d,%d,","总计",pronv_sms[t].sunmobile,pronv_sms[t].sumunicom,pronv_sms[t].sumtelcom);
        }
        else
        {
        	  
        	  fprintf(fp,"%d,%d,%d,",pronv_sms[t].sunmobile,pronv_sms[t].sumunicom,pronv_sms[t].sumtelcom);
        }	
    }
    fprintf(fp,"\n");
		
	  fclose(fp);

    return 0;
}


