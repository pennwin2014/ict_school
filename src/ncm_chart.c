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
//#include "gdfontl.h"

#include <iconv.h>

#include <dirent.h>

#include "ncreport.h"
#define   NC_LNK_IPPKG      31

#include "ncportalweb.h"



#include <iconv.h>
//int getCharset(char *, int);


void ncsReplaceStr(char *pInput, char *pOutput, char *pSrc, char *pDst)
{
    char *pi, *po, *p;
    int nSrcLen, nDstLen, nLen;

    // 指向输入字符串的游动指针.
    pi = pInput;
    // 指向输出字符串的游动指针.
    po = pOutput;
    // 计算被替换串和替换串的长度.
    nSrcLen = strlen(pSrc);
    nDstLen = strlen(pDst);

    // 查找pi指向字符串中第一次出现替换串的位置,并返回指针(找不到则返回null).
    p = strstr(pi, pSrc);
    if(p) {
        // 找到.
        while(p) {
              // 计算被替换串前边字符串的长度.
            nLen = (int)(p - pi);
              // 复制到输出字符串.
            memcpy(po, pi, nLen);
            memcpy(po + nLen, pDst, nDstLen);
              // 跳过被替换串.
            pi = p + nSrcLen;
              // 调整指向输出串的指针位置.
            po = po + nLen + nDstLen;
              // 继续查找.
            p = strstr(pi, pSrc);
        }
        // 复制剩余字符串.
       strcpy(po, pi);
    }
    else
    {
        // 没有找到则原样复制.
       strcpy(po, pi);
    }
} 




char *convert_v4(const char *codefrom, const char *codeto, const char *src)
{
        char    to[32] = "";
        char    *out = NULL;
        char    *dst = NULL;

        iconv_t c;
        size_t  n_in  =  strlen(src);
        size_t  n_out =  2*n_in;

        snprintf(to, sizeof(to), "%s//IGNORE", codeto);
        if( (out=(char *)calloc(n_out+1, sizeof(char))) == NULL)
        {
                printf("allocate memory for %d bytes failed\n", n_out);
                return NULL;
        }
        dst=out;

        if((c = iconv_open(to, codefrom))==(iconv_t)(-1))
        {
                printf("codefrom = %s, codeto = %s, failed\n", codefrom, codeto);
                return NULL;
        }
        iconv(c, (char **)&src, &n_in, &dst, &n_out);
        iconv_close(c);
        return out;
}



char **ncsUtlGetTable(unsigned long lTime, int days, char *prefix, unsigned long *lStartTime, int *lCount)
{
   	char sdate[33] = "";
   	strcpy(sdate, utTimFormat("%Y-%m-%d", lTime));//当前日期
   	strcat(sdate, " 00:00:00");
   	*lStartTime = utTimStrToLong("%Y/%m/%d %H:%M:%S", sdate) - (days-1) * 86400;
   	
   	unsigned long lStartTime1 = *lStartTime;
   	
   	int i = 0;
   	
   	char minYear[5] = "";
   	char maxYear[5] = "";
   	char minMon[3] = "";
   	char maxMon[3] = "";
   	
   	strcpy(minYear, utTimFormat("%Y", lStartTime1));
   	strcpy(maxYear, utTimFormat("%Y", lTime));
   	
   	strcpy(minMon, utTimFormat("%m", lStartTime1));
   	strcpy(maxMon, utTimFormat("%m", lTime));
   	
   	int iMin = atoi(minYear) * 12 + atoi(minMon);
   	int iMax = atoi(maxYear) * 12 + atoi(maxMon);
   	int strLen = strlen(prefix) + 7;
   	
   	*lCount = (iMax - iMin) + 1;
   	
   	 int a;
   	 int b;
   	 char **tmp = (char **)malloc(sizeof(char *)*((iMax - iMin) + 1));
   	 if(tmp != NULL)
   	 {
   	 	int j = 0;
   	 	for(i = iMin; i <= iMax; i++)
   	 	{
   	 		tmp[j] = (char *)malloc(strLen);
   	 		if(tmp[j] != NULL)
   	 		{
   	 			a = i/12;
   	 			b = i%12;
   	 			if(b == 0)
   	 			{
   	 				a--;
   	 				b = 12;
   	 			}
   	 			snprintf(tmp[j], strLen, "%s%04d%02d", prefix, a, b);
   	 		}
   	 		j++;
   	 	}	
   	 }
   	 return tmp;
}

char **ncsUtlGetTable2(char *sdate, char *edate, char *prefix, unsigned long *lStartTime, unsigned long *lEndTime, int *lCount)
{
   	char stime[15] = "";
   	char etime[15] = "";
   	
   	snprintf(stime, sizeof(stime), "%c%c%c%c%c%c%c%c000000", sdate[0], sdate[1], sdate[2], sdate[3], sdate[5], sdate[6], sdate[8], sdate[9]);
   	snprintf(etime, sizeof(stime), "%c%c%c%c%c%c%c%c235959", edate[0], edate[1], edate[2], edate[3], edate[5], edate[6], edate[8], edate[9]);
   	*lStartTime = utTimStrToLong("%Y%m%d%H%M%S", stime);
   	
   	unsigned long lTime = utTimStrToLong("%Y%m%d%H%M%S", etime);
   	*lEndTime = lTime;
   	
   	unsigned long lStartTime1 = *lStartTime;
   	
   	int i = 0;
   	
   	char minYear[5] = "";
   	char maxYear[5] = "";
   	char minMon[3] = "";
   	char maxMon[3] = "";
   	
   	strcpy(minYear, utTimFormat("%Y", lStartTime1));
   	strcpy(maxYear, utTimFormat("%Y", lTime));
   	
   	strcpy(minMon, utTimFormat("%m", lStartTime1));
   	strcpy(maxMon, utTimFormat("%m", lTime));
   	
   	int iMin = atoi(minYear) * 12 + atoi(minMon);
   	int iMax = atoi(maxYear) * 12 + atoi(maxMon);
   	int strLen = strlen(prefix) + 7;
   	
   	*lCount = (iMax - iMin) + 1;
   	
   	 int a;
   	 int b;
   	 char **tmp = (char **)malloc(sizeof(char *)*((iMax - iMin) + 1));
   	 if(tmp != NULL)
   	 {
   	 	int j = 0;
   	 	for(i = iMin; i <= iMax; i++)
   	 	{
   	 		tmp[j] = (char *)malloc(strLen);
   	 		if(tmp[j] != NULL)
   	 		{
   	 			a = i/12;
   	 			b = i%12;
   	 			if(b == 0)
   	 			{
   	 				a--;
   	 				b = 12;
   	 			}
   	 			snprintf(tmp[j], strLen, "%s%04d%02d", prefix, a, b);
   	 		}
   	 		j++;
   	 	}	
   	 }
   	 return tmp;
}

//排序  按流量排序
static int ncUtlSortOnServiceId(char *p1,char *p2)
{
    int i;
    long long l;
   
    struct ncSflow_s psC1,psC2;
    memcpy(&psC1,p1,sizeof(struct ncSflow_s));
    memcpy(&psC2,p2,sizeof(struct ncSflow_s));    
    l=psC2.lBytes-psC1.lBytes;
    if(l>0) return 1;
    if(l<0) return -1;
    if(l==0) return 0;
  
}


//排序  按服务ID排序
static int ncUtlSortOnServiceId2(char *p1,char *p2)
{
    int i;
    long l;
   
    struct ncSflow_s psC1,psC2;
    memcpy(&psC1,p1,sizeof(struct ncSflow_s));
    memcpy(&psC2,p2,sizeof(struct ncSflow_s));    
    l=psC1.lService-psC2.lService;
    if(l>0) return 1;
    if(l<0) return -1;
    if(l==0) return 0;
  
}

//国内国际短信统计
int ncm_WebShowSmsSendType(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
	    }sms_stat_s;
	    sms_stat_s stat_sms[24];
	    sms_stat_s *stat_tmp;
	    
	    long sms_num = 0,sms_time = 0,sumlocal = 0;
	    char sms_code[20] = "";
	    iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
	    if(iReturn != 0) lGroupid0 = 0;
   
    	utMsgPrintMsg(psMsgHead);
      iReturn = utMsgGetSomeNVar(psMsgHead,11,"order",    UT_TYPE_STRING, 12, caOrder,
																							"exppdf",   UT_TYPE_STRING,8,caPdf,
																							"expexcel", UT_TYPE_STRING, 6,  caExport,
																							"sunrec",   UT_TYPE_STRING,8, caSum,
																							"sdate",    UT_TYPE_STRING,11,caSdate,
																							"edate",    UT_TYPE_STRING,11,caEdate,
																							"shour",    UT_TYPE_STRING,2,caShour,
																							"ehour",    UT_TYPE_STRING,2,caEhour,
																							"groupid",  UT_TYPE_STRING,20,caGroupid,
																							"tr_gname", UT_TYPE_STRING,127,trGname,
																							"gitem",    UT_TYPE_STRING,8,caGitem);
		 //init 最终数组
		 for (i = 0;i < 24;i++)
		 {
		 	  stat_sms[i].num = i;
		    sprintf(stat_sms[i].timeV,"%02d:00-%02d:59",i,i);
		    stat_sms[i].localNum = 0;
		    stat_sms[i].interNum = 0;	
//		    printf("num = %d timeV = %s localNum = %d interNum = %d \n",stat_sms[i].num,stat_sms[i].timeV,stat_sms[i].localNum,stat_sms[i].interNum);
		 }				
  
     
      
	   if(strlen(caSum)==0) strcpy(caSum,"20");
	   lSumShow=atol(caSum);
	   trim(trGname);
	   if(strlen(trGname)>0){
	   		strcpy(caTemp_d,pasCvtGBK(2,trGname,caTemp,128));
	   		strcpy(trGname,caTemp_d);
	   } 
	   if(strlen(caSdate)==0){
	   	lTime=time(0);
	   	ncUtlWeekDate(lTime,caSdate,caEdate);
	
	   }  
	   else if(strlen(caEdate)==0){
	   	lTime=time(0);
	   	strcpy(caEdate,utTimFormat("%Y/%m/%d ",lTime));
	
	   } 
	   if(strlen(caShour)==0) strcpy(caShour,"00");
	   if(strlen(caEhour)==0) strcpy(caEhour,"23");
	   if(strlen(caGitem)==0) strcpy(caGitem,"100");
	   // zms	
	   strcpy(caSdate,"2013/12/23");
     strcpy(caEdate,"2013/12/29");	
	   	
	   sprintf(caSdate_temp,"%s %s",caSdate,caShour);
	   sprintf(caEdate_temp,"%s %s",caEdate,caEhour);         

  	 psDbHead = utPltInitDb();
 
   
	   utPltPutVar(psDbHead,"sunrec",caSum);
	   utPltPutVar(psDbHead,"sdate",caSdate);
	   utPltPutVar(psDbHead,"edate",caEdate);
	   utPltPutVar(psDbHead,"shour",caShour);
	   utPltPutVar(psDbHead,"ehour",caEhour);
	   utPltPutVar(psDbHead,"groupid",caGroupid);
	   if(strlen(caGroupid)>0){
//	    	utPltPutVar(psDbHead,"groupname",ncUtlGetGroupNameByid(psShmHead,atol(caGroupid)));
	   }
	   else{
	    	utPltPutVar(psDbHead,"groupname",ncLang("1889点击选择"));
	   }
	   utPltPutVar(psDbHead,"gitem",caGitem);
	   //读取区域信息
	   sprintf(sqlbuf,"select count(*) from ncgroup  ");
	   pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&maxgcou);
	   group_str=(struct group_s *)malloc((maxgcou+1)*sizeof(struct group_s));
	   sprintf(sqlbuf,"select distinct groupid,groupname from ncgroup order by groupid asc ");
		 printf("sqlbuf=%s\n",sqlbuf);
		 gNum=0;
		 psCur = pasDbOpenSql(sqlbuf, 0);
		 if(psCur != NULL)
		 {
				gid = 0;
				memset(gname,0,sizeof(gname));
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &gid,
				                             UT_TYPE_STRING,32, gname);
				while((iret == 0)||(iret == 1405))
				{
						group_str[gNum].gid=gid;
						strcpy(group_str[gNum].gname,gname);
						gNum++;
						gid = 0;
						memset(gname,0,sizeof(gname));
						iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &gid,
					                             	 UT_TYPE_STRING,32, gname);
				}
				pasDbCloseCursor(psCur);
	   } 
    
     
     lStime=utTimStrToLong("%Y/%m/%d %H:00",caSdate_temp);
     lEtime=utTimStrToLong("%Y/%m/%d %H:59",caEdate_temp);
     printf("caSdate=%s  caEdate=%s \n",caSdate,caEdate);
     
     
	 
		 if(strlen(caSdate) > 0 && strlen(caEdate) > 0){
					ptmp = ncsUtlGetTable2(caSdate, caEdate, "ncsrvsmslog_if_", &lStartTime, &lTime, &lCount);
		 }else{
					ptmp = ncsUtlGetTable(lTime, days, "ncsrvsmslog_if_",  &lStartTime, &lCount);
		 }
	 
	 	 if(strlen(trGname)>0){
		  	sprintf(caTemp,"select groupcode from ncgroup where groupname like '%s%c' ",trGname,'%');
	    	memset(caGroupcode,0,sizeof(caGroupcode));
	    	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,14,caGroupcode);	
	    	if(strlen(caGroupcode)==0){
	    			sprintf(caTemp,"select groupcode from ncwlanhot where hname like '%s%c' ",trGname,'%');
		    		pasDbOneRecord(caTemp,0,UT_TYPE_STRING,14,caGroupcode);	
	    	}
		 }
		 printf("lCount=%d \n",lCount);
	   pFlow=(struct ncFlowSort_s *)malloc(lSumShow*sizeof(struct ncFlowSort_s));
	   if(pFlow==NULL){
	   	    utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0405内存分配出错"),ncLang("0405内存分配出错"));
	        return 0;
	   }
	   lGitem=0;
	   for(i = 0; i < lCount; i++){ 
	      
	  		  lGitem=4;                                                
		  		if(i>0){
							sprintf(sqlbuf+strlen(sqlbuf)," union ");
							sprintf(sqlbuf+strlen(sqlbuf),"select count(*) as smsnum,from_unixtime(logtime,'%H') as atime,servicecode from %s b where  areacode = 086  ",ptmp[i]);
							sprintf(sqlbuf+strlen(sqlbuf)," and b.logtime>=%lu and b.logtime<=%lu ",lStime,lEtime);
					}else{
		  		    sprintf(sqlbuf,"select count(*) as smsnum,from_unixtime(logtime,'%H') as atime,servicecode from %s b where  areacode = 086 ",ptmp[i]);	
		          sprintf(sqlbuf+strlen(sqlbuf)," and b.logtime>=%lu and b.logtime<=%lu ",lStime,lEtime);
		      }
	        if(lGroupid0!=0){
	        		sprintf(sqlbuf+strlen(sqlbuf)," and gid in (%s) ",getPortalGroupid(atol(lGroupid0)));
	        }
	        if(strlen(caGroupid)!=0){
	 //        	  sprintf(sqlbuf+strlen(sqlbuf)," and (a.groupid=%d or a.groupid in (select groupid from ncgroup where pid=%d ))",atol(caGroupid),atol(caGroupid));
	//      			sprintf(sqlbuf+strlen(sqlbuf)," and gid=%s ",caGroupid);
	      			sprintf(sqlbuf+strlen(sqlbuf)," and gid in (%s) ",getPortalGroupid(atol(caGroupid)));
	        }
	  		  sprintf(sqlbuf+strlen(sqlbuf)," group by atime asc  ");
	
	  	}

     strcpy(caUnit,"条");
     strcpy(caOrder,ncLang("0369流量"));
     sprintf(sqlbuf+strlen(sqlbuf)," order by atime desc limit 0,%lu ",lSumShow);
//     sprintf(sqlbuf+strlen(sqlbuf)," );
  	 printf("11sqlbuf=%s\n",sqlbuf);
  	 utPltPutVar(psDbHead,"order",caOrder);
  	
     psCur=pasDbOpenSql(sqlbuf,0);
     if(psCur == NULL) {
    	  if(pHash_dep) free(pHash_dep);
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
    
     for (i = 0; i < lCount; i++)
     { 
		    lGitem=4;                                                 //国际短信统计
				if(i>0){
									sprintf(sqlbuf+strlen(sqlbuf)," union ");
									sprintf(sqlbuf+strlen(sqlbuf),"select count(*) as smsnum,from_unixtime(logtime,'%H') as atime,servicecode from %s b where  areacode != 086  ",ptmp[i]);
									sprintf(sqlbuf+strlen(sqlbuf)," and b.logtime>=%lu and b.logtime<=%lu ",lStime,lEtime);
				}else{
				  		    sprintf(sqlbuf,"select count(*) as smsnum,from_unixtime(logtime,'%H') as atime,servicecode from %s b where  areacode = !086 ",ptmp[i]);	
				          sprintf(sqlbuf+strlen(sqlbuf)," and b.logtime>=%lu and b.logtime<=%lu ",lStime,lEtime);
				}
			  if(lGroupid0!=0){
			        		sprintf(sqlbuf+strlen(sqlbuf)," and gid in (%s) ",getPortalGroupid(atol(lGroupid0)));
			  }
			  if(strlen(caGroupid)!=0){
			      			sprintf(sqlbuf+strlen(sqlbuf)," and gid in (%s) ",getPortalGroupid(atol(caGroupid)));
			  }
			  sprintf(sqlbuf+strlen(sqlbuf)," group by atime asc  ");
	   }
     strcpy(caUnit,"条");
     strcpy(caOrder,ncLang("0369流量"));
     sprintf(sqlbuf+strlen(sqlbuf)," order by atime desc limit 0,%lu ",lSumShow);
  	 printf("12sqlbuf=%s\n",sqlbuf);
  	 utPltPutVar(psDbHead,"order",caOrder);
  	
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
	     	iNum++;
	     	sms_num=0;
		    sms_time=0;
		    memset(sms_code,0,sizeof(sms_code));
        iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long)    ,&sms_num,                             
	                                     UT_TYPE_LONG,  sizeof(long)    ,&sms_time,
	                                     UT_TYPE_STRING,sizeof(sms_code),sms_code);           
	  }
	  pasDbCloseCursor(psCur); 
    
    int tNum=0;  
	  unsigned long long maxbyte=0;
	  int unitflg=0;
	  
	  long maxlocal = 0,maxinter = 0;
	  for (i = 0;i < 24;i++)
	  {
	  	if (stat_sms[i].localNum > maxlocal)
	  	{
	  	   maxlocal	= stat_sms[i].localNum;
	  	}	
	  	if (stat_sms[i].interNum > maxlocal)
	  	{
	  	   maxinter	= stat_sms[i].interNum;
	  	}	
	  }
	  
	  if (maxlocal < 10)
	  {
	     maxlocal = 10;	
	  }
	  else if (maxlocal > 10 && maxlocal < 50)
	  {
	     maxlocal = 50;	
	  }
	  else if (maxlocal > 50 && maxlocal < 100)
	  {
	     maxlocal = 100;	
	  }
	  else
	  {
	     maxlocal = (maxlocal/100+1)*100;		
	  }	
	  int mNum=24;

    
		if(strlen(caExport)>0){                                      //输出到EXEC中
			    FILE *fp;
	        char caFilename[32];
	        char caFile[128];
	        char caPath[128];
	 
  				sprintf(caFilename,"ncsort%d.csv",utStrGetId());
				  strcpy(caPath,MODULEBINPATH);
				  sprintf(caFile,"%s/%s",caPath,caFilename);
	        fp = fopen(caFile,"w");
	        if(fp == NULL) {
	            pasDbCloseCursor(psCur);
	            utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","短信查询",ncLang("0546数据导出出错"));
	            return 0;
	        }
	        fprintf(fp,"国内国际短信发送数量(%s--%s)\n","\0",caSdate_temp,caEdate_temp);
					                                 
					fprintf(fp,"序号,时间,国际短信数量,国内短信数量(%s),百分比\n",caUnit);
					for(i=0;i<mNum;i++)
					{
						 	fprintf(fp,"%d,\"%s\",\"%d\",\"%d\",\"%0.2f\"\n",i+1,stat_sms[i].timeV,stat_sms[i].interNum,stat_sms[i].localNum,(float)stat_sms[i].localNum/sumlocal);
					}
					
	        fclose(fp);
	        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
	        remove(caFile);
	        free(pFlow);
	        utPltFreeDb(psDbHead);	        
			    return 0;
		}
	  
	  if(strlen(caPdf)>0){                         //输出PDF文档
	     		//表格标签   
				desc=malloc(lGitem*sizeof(char*));
				for(i=0;i<lGitem;i++){
				 	desc[i]=malloc(32);
			  }
			  strcpy(desc[0],"时间");
			  strcpy(desc[1],"国际短信数量");
			  strcpy(desc[2],"国内短信数量");
			  strcpy(desc[3],"百分比");
			  iNum = 24;
	     	 //输出到PDF中     
			  pChart=(struct piechart_datasetf*)malloc(sizeof(struct piechart_datasetf)*iNum);
			    if(pChart==NULL){
			  	  utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0405内存分配出错"));
			      return 0;
			  }
			  
		    for(i=0;i<iNum;i++){
		  	  	//输出表格的补充字段
  				  pChart[i].append=malloc((lGitem-3)*sizeof(char*));
  				  for(m=0;m<lGitem-3;m++){
  				  	pChart[i].append[m]=malloc(32);
  				  }
  				 pChart[i].name=strdup(stat_sms[i].timeV);
  				 sprintf(pChart[i].append[0],"%d",stat_sms[i].interNum);
       		 pChart[i].amount = (float)stat_sms[i].localNum;
       		 
	         if(sumlocal==0)
	          	  pChart[i].percent=strdup("0.00");
	         else{
	          		sprintf(caTemp,"%0.2f",(float)100*stat_sms[i].localNum/sumlocal);
	          		pChart[i].percent=strdup(caTemp);
	         }
//	         printf("time = %s localNum = %d interNum = %d sumlocal = %d \n",stat_sms[i].timeV,stat_sms[i].localNum,stat_sms[i].interNum,sumlocal);
//	         printf("name = %s append = %s   \n",pChart[i].name,pChart[i].append[0]);
        }
        
	     	rowrow=iNum;
        sprintf(filename,"%s.PDF","国内国际短信发送数量"); 
				strcpy(caTitle,"国内国际短信发送数量");
				printf("*****000***sumlocal=%d lGitem=%d\n",sumlocal,lGitem);
      	sprintf(caStatdate,ncLang("1897统计时间：从%s %s 到%s %s"),caSdate,caShour,caEdate,caEhour);
        ncmWebShowSMSsendType_PDF(iFd,pChart,desc,rowrow,lGitem,lGitem-3, caTitle,caUnit,caStatdate,filename); 
	     	//释放内存     
		    for(i=0;i<rowrow;i++)
		    {
			      free(pChart[i].name);
			      free(pChart[i].percent);
			      for(m=0;m<lGitem-3;m++){
	      	     free(pChart[i].append[m]);
	          }
	          if(pChart[i].append) free(pChart[i].append);
		    }  
			  if(pChart) free(pChart);
			   
			  for(i=0;i<lGitem;i++){
			   	   free(desc[i]);
			  }
		    if(desc) free(desc);
        free(pFlow);
        utPltFreeDb(psDbHead);
	     	return 0;
	  }
	  index=-1;
//输出到界面	   
	  for(i=0;i<mNum;i++){
	  		 if(i>0){
						 utPltPutLoopVar(psDbHead,"sms_sdh",tNum+1,",");
				 }	
				 tNum++;
				 if(i==0){
//						utPltPutLoopVarF(psDbHead,"maxbyte",tNum,"%llu",maxbyte);
						utPltPutLoopVarF(psDbHead,"maxlocal",tNum,"%lu",maxlocal);
						utPltPutLoopVarF(psDbHead,"maxinter",tNum,"%lu",maxinter);
						
						if(strlen(caSdate)>0)
				 		  utPltPutLoopVar(psDbHead,"sms_sdate",tNum,caSdate);
				 		if(strlen(caEdate)>0)
				 		  utPltPutLoopVar(psDbHead,"sms_edate",tNum,caEdate);
				 }
         utPltPutLoopVarF(psDbHead,"sms_snum",tNum,"%d",i+1);
         utPltPutLoopVarF(psDbHead,"sms_snum1",tNum,"%02d",i+1);
//        utPltPutLoopVarF(psDbHead,"sms_suid",tNum,"%d",i+1);
         utPltPutLoopVarF(psDbHead,"sms_sunit",tNum,"%s",caUnit);
         utPltPutLoopVarF(psDbHead,"sms_tunit",tNum,"%s","小时");
      
         utPltPutLoopVarF(psDbHead,"sms_slocal",tNum,"%lu",stat_sms[i].localNum); 	
         utPltPutLoopVarF(psDbHead,"sms_sinter",tNum,"%lu",stat_sms[i].interNum); 	
         utPltPutLoopVarF(psDbHead,"sms_stime",tNum,"%s",stat_sms[i].timeV);

    }
//    if(iNum==0) iNum=1;
    printf("iNum  = %d \n",iNum);
    utPltPutVarF(psDbHead,"sms_sTotRec","%lu",24);
//    utPltPutVar(psDbHead,"unit",caUnit);
    free(pFlow);
    utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmchart/ncm_ext_sms_typesend.html");
    return 0;
}

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
     char *tmputf8 = NULL;
     
		 strcpy(mysdate,sdate);
	   strcpy(myedate,edate);
	   strcpy(mymakepath,makepath);
		 
//		 strcpy(mysdate,"2014-02-24 00:00:00");
//	   strcpy(myedate,"2014-02-24 23:59:59");
//	   strcpy(mymakepath,"/db/applereport/2014-02-24/smsStatisticsReport.csv");
		 
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
//     printf("caSdate=%s  caEdate=%s \n",mysdate,myedate);
     

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
    /*从配置行政表来  英文版可扩充
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
	  */
	  //按分组取省名
	  int tmpint = 0;
	  sprintf(sqlbuf,"select count(*) from ncsrvsmslog where 1 = 1 ");
	  sprintf(sqlbuf+strlen(sqlbuf)," and logtime>=%lu and logtime<=%lu and contrycode = 86 group by areaname ",lStime,lEtime);
    psCur=pasDbOpenSql(sqlbuf,0);
//    printf("sqlbuf = %s \n",sqlbuf); 
    if(psCur == NULL) {
        return 0;
    }
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,sizeof(long) ,&tmpint);                               
	  iNum=0;
	  while(iReturn==0||iReturn==1405)
	  {
	     	pronCou++;
		    tmpint = 0;
        iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,sizeof(long) ,&tmpint);            
	  }
	  pasDbCloseCursor(psCur); 
	  
	  pronv_sms = (sms_pronv_s *)malloc(sizeof(sms_pronv_s)*pronCou+1);    
	  sprintf(sqlbuf,"select areaname from ncsrvsmslog  where  1 = 1 ");
	  sprintf(sqlbuf+strlen(sqlbuf)," and logtime>=%lu and logtime<=%lu and contrycode = 86 group by areaname order by areaname asc",lStime,lEtime);
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
	        
//	  printf(" fuck 1111  sdate = %s  edate = %s  mymakepath = %s\n",mysdate,myedate,mymakepath);
//	  fprintf(fp,ncLang("2037国内国际运营商短信发送数量(%s--%s)\n"),mysdate,myedate);
	  fprintf(fp,"SMS Statistics Report(%s--%s)\n",mysdate,myedate);
		fprintf(fp,"Time,Local,Inter,CM,CU,CT\n");
//		fprintf(fp,"时间,国内短信数量,国际短信数量,移动短信数量,联通短信数量,电信短信数量\n");
		for(i=0;i<mNum;i++)
		{
					fprintf(fp,"\"%s\",%d,%d,%d,%d,%d\n",stat_sms[i].timeV,stat_sms[i].localNum,stat_sms[i].interNum,stat_sms[i].mobileNum,stat_sms[i].unicomNum,stat_sms[i].telcomNum);
		}
		//汇总
//		fprintf(fp,"\"%s\",%d,%d,%d,%d,%d\n","总计",sumlocal,sumForeign,sumMobile,sumUnicom,sumTelecom);
		fprintf(fp,"\"%s\",%d,%d,%d,%d,%d\n","Total",sumlocal,sumForeign,sumMobile,sumUnicom,sumTelecom);
		fprintf(fp,"\n");
		//明细报表    做格式  表头
		for (t = 0; t <pronCou; t++)
    {
//    	printf("pronvence = %s summobile = %d sumunicome = %d suntelcome = %d\n",pronv_sms[t].pronvence,pronv_sms[t].sunmobile,pronv_sms[t].sumunicom,pronv_sms[t].sumtelcom);
    	if (t == 0)
    	{
    	   fprintf(fp,"\t\r,");	
    	}
    	if(strlen(pronv_sms[t].pronvence)<1)
    	{
    		 fprintf(fp,"%s,\t,\t,","Unknown");
    	}else
    	{			
    	tmputf8 = convert("GBK", "UTF-8", pronv_sms[t].pronvence);
//    	fprintf(fp,"%s,\t,\t,",pronv_sms[t].pronvence);
    	fprintf(fp,"%s,\t,\t,",tmputf8);
    	free(tmputf8);tmputf8 = NULL;
      }
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
    
    
    if (pronv_sms)
    	free(pronv_sms);
    for (t = 0; t <pronCou; t++)
    {  
       if(pronv_sms[t].sms_cont)
       	 free(pronv_sms[t].sms_cont);
    }
    return 0;
}

/*
//生成短信发送报表
char *ncm_web_App_smsDetailReportSend(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead,char sdate1[32], char edate1[32],char caExport1[32],int folder_count,char caFilename[64])
{  
	printf("ncm_web_App_smsDetailReportSend start!\n");
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 int iNumber = 0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char sdate[32] = "";
	 char edate[32] = "";
	 char areacode[32] = "";
	 char username[32] = "";
	 char usermac[18] = "";
	 char ip[16] = "";
	 char logtime[64] = "";
	 
	 char caAreacode[32] = "";
	 char caUsername[32] = "";
	 char caLogtime[64] = "";
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = 0;
	 
	 char insert[16] = "";
	 char caInsert[16] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	  // 关键字
	 char _keyword[33] = "";
	 char caSesid[1024] = "";
	 
	 // 数据表
	 unsigned long lCount1 = 0; // 表中记录的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
   
   // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
   
   // 导出
	 char caExport[32] = "";
	 FILE *fp = NULL;
	// char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 
	 trim(_keyword);
	strcpy(caExport,caExport1);
	 
	 // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"logtime");
	 }
	 
	 ncTimeToLong(sdate1, &lSdate);
	 ncTimeToLong(edate1, &lEdate);
	 printf("lSdate = %d\n", lSdate);
	 printf("lEdate = %d\n", lEdate);
	 
	 sprintf(sql, "select count(*) from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);
	 if(!utStrIsSpaces(areacode)) // 根据操作系统查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and areacode like '%%%s%%'", areacode);
	 }
	 if(!utStrIsSpaces(username)) // 根据用户登录名查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(usermac)) // 根据mac地址查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and usermac like '%%%s%%'", usermac);
	 }
	 if(!utStrIsSpaces(ip)) // 根据ip地址查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 // 导出
	 if(strlen(caExport)>0){
	 		// strcpy(caFilename, "smsDetailReport.csv");
	 		 sprintf(caPath, "%s/report/%02d", "/home/ncmysql/ncm",folder_count);
	 		 sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 	pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"短信发送详细报告\n");
			 fprintf(fp,"\t\t时间:%s--%s\n", sdate1, edate1);
			 ncUtlPrintCsv(fp,5,UT_TYPE_STRING,"国家区号",
			 									  UT_TYPE_STRING,"用户账号",
			                    UT_TYPE_STRING,"MAC地址",
			                    UT_TYPE_STRING,"IP",
			                    UT_TYPE_STRING,"发送时间");
	 }
	 
	 sprintf(sql, "select username, usermac, ip, date_format(from_unixtime(logtime), '%T') as time, areacode from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);		                    
	 if(!utStrIsSpaces(areacode)) // 根据操作系统查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and areacode like '%%%s%%'", areacode);
	 }
	 if(!utStrIsSpaces(username)) // 根据用户登录名查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(usermac)) // 根据mac地址查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and usermac like '%%%s%%'", usermac);
	 }
	 if(!utStrIsSpaces(ip)) // 根据ip地址查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by username order by %s %s", sort, dir);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL){
	 		 char username[32] = "";
	 		 char usermac[18] = "";
	 		 char ip[16] = "";
	 		 char time[32] = "";
	 		 char areacode[32] = "";
	 		 
	 		 while(0 == (iret = pasDbFetchInto(psCur,
				 UT_TYPE_STRING, sizeof(username)-1,  	username,
				 UT_TYPE_STRING, sizeof(usermac)-1,  	  usermac,
				 UT_TYPE_STRING, sizeof(ip)-1,  	      ip,
				 UT_TYPE_STRING, sizeof(time)-1,  			time,
				 UT_TYPE_STRING, sizeof(areacode)-1,  	areacode)) || 1405 == iret)
		  {
	       iNum++;
	  	   if(iNum > 1)
				 {
					 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
				 }
				 
				 utPltPutLoopVar(psDbHead,"areacode",iNum,areacode);
				 utPltPutLoopVar(psDbHead,"username",iNum,username);
				 utPltPutLoopVar(psDbHead,"usermac",iNum,usermac);
				 utPltPutLoopVar(psDbHead,"ip",iNum,ip);
				 utPltPutLoopVar(psDbHead,"logtime",iNum,time);
				 if(strlen(caExport)>0){
					 		ncUtlPrintCsv(fp,5,UT_TYPE_STRING,areacode,
					 											UT_TYPE_STRING,username,
														 		UT_TYPE_STRING,usermac,
														 		UT_TYPE_STRING,ip,
			                          UT_TYPE_STRING,time);
					 }
		  }
	 }
	 utPltShowDb(psDbHead); 
	 pasDbCloseCursor(psCur);
	 
	 if(strlen(caExport)>0){
 	    fclose(fp);
      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
      //remove(caFile);
      utPltFreeDb(psDbHead);
      return caFile;
	 }
	 
	// utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_appreport/ncm_web_App_smsDetailReport.htm");
	 printf("ncm_web_App_smsDetailReportSend end!\n");
	 return caFile;
}
*/
