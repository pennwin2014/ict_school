#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <string.h>
#include <locale.h>
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
#include "dsutl.h"
#include "dsclt.h"
#include "dlfcn.h"
#include "ncportal.h"
#include <iconv.h>
#define FILEPATH "/home/ncmysql/ncsrv/html/ncm_webplacelimit"

typedef struct UserLoginInfo
{
	char username[20];
	int times;
}UserLoginInfo;
UserLoginInfo *pUserLoginInfo;

//�û�����Ƶ��ͳ��
int ncm_web_Place_smslog_count(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	
	
			 utMsgPrintMsg(psMsgHead);
	
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int i;
	 int iret=0;
	 int iNum=0;	 
	 char caUsername[20] = "";
	 char sdate[32] = ""; // ����Ŀ�ʼʱ��
   char edate[32] = ""; // ����Ľ���ʱ��
   char username[300000][20];

	 char is_export[32] = "";
	 int iexport;	
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	   
   unsigned long  lStarttime = 0;
	 unsigned long  lStopday = 0;
	 unsigned long  lAddtime = 0;
	 unsigned long  lStartday = 0;
	 

	 
	 UserLoginInfo loginInfo[20000];
	 	memset(loginInfo , 0 , sizeof(UserLoginInfo) * 20000);
	 	
	 FILE *fp = NULL;
   
   // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 3,"sdate",        UT_TYPE_STRING,  sizeof(sdate)-1,           sdate,
															   "edate",        UT_TYPE_STRING,  sizeof(edate)-1,           edate,
															   "is_export",    UT_TYPE_STRING,  sizeof(is_export)-1,       is_export);
	 

	 
	 
	 lStartday = atol(sdate)/86400;
	 lStopday = atol(edate)/86400;
	 iexport = atoi(is_export);
	 while(lStartday<lStopday)
	 {
	
	 lAddtime = (lStartday+1)*86400;
	 lStarttime = lStartday*86400;
	 sprintf(sql, "select distinct username from ncsrvuserlog where starttime >= %lu and starttime <= %lu and username !='Guest'" ,lStarttime,lAddtime);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur!=NULL)
	 {
    	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	    iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername);
	 	  while(0 == iret || 1405 == iret)
		  {
		  	memset(username[iNum],0,20);
		   	strcpy(username[iNum],caUsername);
			 	iNum++;
			  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername);
		  }
	 		
		} 
	 	lStartday++;
	 	pasDbCloseCursor(psCur);
	} 
	
	
	
	
	int j = 0;
	int k = 0;
	
	int isFound = 0;
	for(i = 0 ; i < iNum ; i++)
	{
		
		j = 0;
		isFound = 0;
		while((j < 200000) && (strlen(loginInfo[j].username) != 0))
		{
				if(strcmp(loginInfo[j].username,username[i])==0)
				{
						isFound = 1;
						loginInfo[j].times++;
						break;	
				}
				j++;
		}
		if(isFound == 0)
		{
			
			strcpy(loginInfo[j].username,username[i]);
			
			loginInfo[j].times = 1;
			
		}
	}
			
	j=0;
	iNum=0;
	
	if(iexport == 0)
	{
		while(j < 200000 &&(strlen(loginInfo[j].username) != 0))
		{
			iNum++;
				 if(iNum > 1)
				 {
					 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
				 }
				 
			 	   utPltPutLoopVar(psDbHead,"username",iNum,loginInfo[j].username);
			 	   utPltPutLoopVarF(psDbHead,"logintimes",iNum,"%d",loginInfo[j].times);
			j++;
		}
			utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_userlog_frequencyreport.htm");
	}
	else
		{
			
			strcpy(caFilename, "Frequencyreport.csv");
			strcpy(caPath, FILEPATH);
  		sprintf(caFile, "%s/%s", caPath, caFilename);
  		
			fp = fopen(caFile, "w");
			
			if(fp == NULL){
		 		 		 pasDbCloseCursor(psCur);
			       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",(char *)ncLang("0546���ݵ�������"),(char *)ncLang("0546���ݵ�������"));
			       return 0;	
			       
		 		 }
		 		 
		 		 //fprintf(fp,"����ϵͳ����ͼ��\n");
		 		 //fprintf(fp,"ʱ��: %s -- %s\n",sdate,edate);
				 ncUtlPrintCsv(fp,2,UT_TYPE_STRING,"�û���",
				 									  UT_TYPE_STRING,"����Ƶ��");
			
					while(j < 200000 &&(strlen(loginInfo[j].username) != 0))
				  {
			
				 		 ncUtlPrintCsv(fp,2,UT_TYPE_STRING, loginInfo[j].username,
		                        		UT_TYPE_LONG,loginInfo[j].times);
								
						j++;
				  }
				
				fclose(fp);
				
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        
        //remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
		}
	return 0;
}

//�û��������ͳ��
int ncm_web_Place_smslog_rule(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int i;
	 int iret=0;
	 int iNum=0;
	 
	 int weeknum;
	 int startWeekNum;
	 int exist = 0;
	 
	 FILE *fp = NULL;
	 char is_export[32] = "";
	 int iexport;	
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 char caUsername[20] = "";
	 char sdate[32] = ""; // ����Ŀ�ʼʱ��
   char edate[32] = ""; // ����Ľ���ʱ��
   char binary_week[32] = ""; //��������ڶ�������
   char start_sday[32] = "";
	 char stop_sday[32] = "";
	 char username[300000][20];   
   unsigned long  lStarttime = 0;
	 unsigned long  lStopday = 0;
	 unsigned long  lAddtime = 0;
	 unsigned long  lStartday = 0;
	 int  tStartTime = 0;
	 int  tStopTime = 0;
	 
	 
	 UserLoginInfo loginInfo[20000];
	 	memset(loginInfo , 0 , sizeof(UserLoginInfo) * 20000);
   
   // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 6,"sdate",        UT_TYPE_STRING,  sizeof(sdate)-1,           sdate,
															   "edate",        UT_TYPE_STRING,  sizeof(edate)-1,           edate,
															   "binary_week",  UT_TYPE_STRING,  sizeof(binary_week)-1,     binary_week,
															   "start_sday",   UT_TYPE_STRING,  sizeof(start_sday)-1,      start_sday,
															   "stop_sday",    UT_TYPE_STRING,  sizeof(stop_sday)-1,       stop_sday,
															   "is_export",    UT_TYPE_STRING,  sizeof(is_export)-1,          is_export);
	 
	 
	 lStartday = atol(sdate)/86400;
	 lStopday = atol(edate)/86400;
	 iexport = atoi(is_export);
	 
	
	 tStartTime = atol(start_sday);
	 tStopTime  = atol(stop_sday);
	 if(tStopTime == 0)
	 	{
	 		tStopTime = 86400;	
	 	}
	 
	 while(lStartday<lStopday)
	 {
	 weeknum = (lStartday+3)%7;
	 for(i = 0 ; i < 7 ; i++)
	 {
	 		if(binary_week[i] == '1')
	 		{
	 				if(weeknum == i)
	 				{
	 						exist = 1;
	 				}
	 		}	
	 }
	 
	 if(exist == 1)
	 {
	 	lAddtime = lStartday*86400+tStopTime;
	 	lStarttime = lStartday*86400+tStartTime;
	 sprintf(sql, "select distinct username from ncsrvuserlog where starttime >= %lu and starttime <= %lu and username !='Guest'",lStarttime,lAddtime);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur!=NULL)
	 {
        	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	    iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername);
	  while(0 == iret || 1405 == iret)
	  {
	  
		  
		  	memset(username[iNum],0,20);
		   	strcpy(username[iNum],caUsername);
			 	iNum++;
			  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername);
		  }
	  }
	 		pasDbCloseCursor(psCur);
	 } 
	 	exist = 0;
	 	lStartday++;
	 	
	} 
	
	int j = 0;
	int k = 0;
	
	int isFound = 0;
	for(i = 0 ; i < iNum ; i++)
	{
		
		j = 0;
		isFound = 0;
		while((j < 300000) && (strlen(loginInfo[j].username) != 0))
		{
				if(strcmp(loginInfo[j].username,username[i])==0)
				{
						isFound = 1;
						loginInfo[j].times++;
						break;	
				}
				j++;
		}
		if(isFound == 0)
		{
			
			strcpy(loginInfo[j].username,username[i]);
			
			loginInfo[j].times = 1;
			
		}
	}
			
	j=0;
	iNum=0;
	if(iexport == 0)
	{
		while(j < 300000 &&(strlen(loginInfo[j].username) != 0))
		{
			iNum++;
				 if(iNum > 1)
				 {
					 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
				 }
				 
			 	   utPltPutLoopVar(psDbHead,"username",iNum,loginInfo[j].username);
			 	   utPltPutLoopVarF(psDbHead,"logintimes",iNum,"%d",loginInfo[j].times);
			j++;
		}
			utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_userlog_frequencyreport.htm");
	}
	else
		{
			
			strcpy(caFilename, "Rulereport.csv");
			strcpy(caPath, FILEPATH);
  		sprintf(caFile, "%s/%s", caPath, caFilename);
  		
			fp = fopen(caFile, "w");
			
			if(fp == NULL){
		 		 		 pasDbCloseCursor(psCur);
			       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",(char *)ncLang("0546���ݵ�������"),(char *)ncLang("0546���ݵ�������"));
			       return 0;	
			       
		 		 }
		 		 
		 		 //fprintf(fp,"����ϵͳ����ͼ��\n");
		 		 //fprintf(fp,"ʱ��: %s -- %s\n",sdate,edate);
				 ncUtlPrintCsv(fp,2,UT_TYPE_STRING,"�û���",
				 									  UT_TYPE_STRING,"����Ƶ��");
			
					while(j < 300000 &&(strlen(loginInfo[j].username) != 0))
				  {
			
				 		 ncUtlPrintCsv(fp,2,UT_TYPE_STRING, loginInfo[j].username,
		                        		UT_TYPE_LONG,loginInfo[j].times);
								
						j++;
				  }
				
				fclose(fp);
				
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        
        //remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
		}
	return 0;
}


//�̶��˿ͷ���
int ncm_web_Place_smslog_custmer(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{		
	printf("�ѽ���ncm_web_Place_smslog_custmer \n");
	 utMsgPrintMsg(psMsgHead);
	
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int i;
	 int iret=0;
	 int iNum=0;	 
	 char caUsername[20] = "";							 // ��ѯ�����û���
	 char sdate[32] = "";                    // ����Ŀ�ʼʱ��
   char edate[32] = ""; 									 // ����Ľ���ʱ��
   char slunch_time[32] = "";  					   // ���յ���Ϳ�ʼʱ��
   char elunch_time[32] = "";              // ���յ���ͽ���ʱ��
   char sdinner_time[32] = "";						 // ���յ���Ϳ�ʼʱ��
   char edinner_time[32] = "";					   // ���յ���ͽ���ʱ��
   char time_online[32] = ""; 					   // ��������Ĳ����ڵ�����ʱ��
   char start_sday[32] = "";							 // �����û��Զ��忪ʼʱ��
	 char stop_sday[32] = "";								 // �����û��Զ������ʱ��
	 
	 char username[300000][20]; 
	 
	 	 FILE *fp = NULL;
	 char is_export[32] = "";
	 int iexport;	
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 
	 char macstr[256] = "";
	 char delims[] = ";";
	 char *result = NULL;
		   
   unsigned long  lStarttime = 0;          // ÿ����Զ��忪ʼʱ��
	 unsigned long  lStopday = 0;						 // ��ʼʱ�������
	 unsigned long  lAddtime = 0;						 // ÿ����Զ������ʱ��
	 unsigned long  lStartday = 0;					 // ����ʱ�������
	 unsigned long  lTime_online = 0;				 // ����ת������û�����ʱ��
	 unsigned long  lslunch_time = 0;				 // ÿ����Ϳ�ʼʱ��
	 unsigned long  lelunch_time = 0;        // ÿ����ͽ���ʱ��
	 unsigned long  lsdinner_time = 0;       // ÿ����Ϳ�ʼʱ��
	 unsigned long  ledinner_time = 0;			 // ÿ����ͽ���ʱ��
	 
	 int  iSlunch_time = 0;									 // ����ת������Ϳ�ʼʱ��
	 int  iElunch_time = 0;									 // ����ת������ͽ���ʱ��
	 int  iSdinner_time = 0;								 // ����ת������Ϳ�ʼʱ��
	 int  iEdinner_time = 0;								 // ����ת������ͽ���ʱ��
	 int  tStartTime = 0;										 // �û��Զ���ÿ��Ŀ�ʼʱ��
	 int  tStopTime = 0;										 // �û��Զ���ÿ��Ľ���ʱ��
	 
	 UserLoginInfo loginInfo[20000];
	 memset(loginInfo , 0 , sizeof(UserLoginInfo) * 20000);
	 
   
   // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 11,"sdate",        UT_TYPE_STRING,  sizeof(sdate)-1,           sdate,
															   "edate",        UT_TYPE_STRING,  sizeof(edate)-1,           edate,
															   "slunch_time",  UT_TYPE_STRING,  sizeof(slunch_time)-1,     slunch_time,
															   "elunch_time",  UT_TYPE_STRING,  sizeof(elunch_time)-1,     elunch_time,
															   "sdinner_time", UT_TYPE_STRING,  sizeof(sdinner_time)-1,    sdinner_time,
															   "edinner_time", UT_TYPE_STRING,  sizeof(edinner_time)-1,    edinner_time,
															   "start_sday",   UT_TYPE_STRING,  sizeof(start_sday)-1,      start_sday,
															   "stop_sday",    UT_TYPE_STRING,  sizeof(stop_sday)-1,       stop_sday,
															   "time_online",  UT_TYPE_STRING,  sizeof(time_online)-1,     time_online,
															   "is_export",    UT_TYPE_STRING,  sizeof(is_export)-1,          is_export,
															   "macstr",       UT_TYPE_STRING,  sizeof(macstr)-1,          macstr);
	
               
	 lStartday = atol(sdate)/86400;
	 lStopday = atol(edate)/86400;
	 lTime_online  = atol(time_online);
	 iexport = atoi(is_export);
	 
	 
	 iSlunch_time = atol(slunch_time);
	 iElunch_time = atol(elunch_time);
	 iSdinner_time = atol(sdinner_time);
	 iEdinner_time = atol(edinner_time);
	 tStartTime = atol(start_sday);
	 tStopTime  = atol(stop_sday);
	 
	 char macsql[1024] = "";
	 	if(strcmp(macstr,"resetmac") !=0)
	 	{	
	 		snprintf(macsql+strlen(macsql), sizeof(macsql)-strlen(macsql), " and(");	
	 		result = strtok( macstr, delims );
	 		int count = 0;
   while( result != NULL ) {
     printf( "result is \"%s\"\n", result );
     count++;
     if(count == 1)
     {
     	snprintf(macsql+strlen(macsql), sizeof(macsql)-strlen(macsql), "(apname = '%s') ", result);
     	
     }
     else
     {
     	snprintf(macsql+strlen(macsql), sizeof(macsql)-strlen(macsql), "or(apname = '%s') ", result);
     }	
     
     result = strtok( NULL, delims );
     if(result == NULL)
     {
     		snprintf(macsql+strlen(macsql), sizeof(macsql)-strlen(macsql), ")");
     		
     }
   }
	}
	
	printf("����ӵ�sql %s\n",macsql);
	while(lStartday<lStopday)
	{
	
	 if(tStopTime == 0&&iEdinner_time==0&&iElunch_time==0)
	 	{
	 			lslunch_time = lStartday*86400;;
	 			lelunch_time = lStartday*86400+86400;
	 			lsdinner_time = lStartday*86400;;
	 			ledinner_time = lStartday*86400+86400;
	 			lAddtime = lStartday*86400+86400;
	 			lStarttime = lStartday*86400;
	 	}
	 else
	 {	
	 lslunch_time = lStartday*86400+iSlunch_time;
	 lelunch_time = lStartday*86400+iElunch_time;
	 lsdinner_time = lStartday*86400+iSdinner_time;
	 ledinner_time = lStartday*86400+iEdinner_time;
	 lAddtime = lStartday*86400+tStopTime;
	 lStarttime = lStartday*86400+tStartTime;
	 }
	 sprintf(sql, "select distinct username from ncsrvuserlog where ((starttime >= %lu and starttime <= %lu) or (starttime >= %lu and starttime <= %lu) or (starttime >= %lu and starttime <= %lu)) and (conntime > %lu) and username!='Guest'" ,lStarttime,lAddtime,lslunch_time,lelunch_time,lsdinner_time,ledinner_time,lTime_online);
	 if(strcmp(macstr,"resetmac") !=0)
	 { 
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), "%s",macsql);	
	}

	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur!=NULL)
	 {
        	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	    iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername);
	  while(0 == iret || 1405 == iret)
	  {
		  	memset(username[iNum],0,20);
		   	strcpy(username[iNum],caUsername);
			 	iNum++;
			  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername);
		  }
	  }
	 pasDbCloseCursor(psCur);	
	 lStartday++;
	 	
	} 
	
	int j = 0;
	int k = 0;
	
	int isFound = 0;
	for(i = 0 ; i < iNum ; i++)
	{
		
		j = 0;
		isFound = 0;
		while((j < 300000) && (strlen(loginInfo[j].username) != 0))
		{
				if(strcmp(loginInfo[j].username,username[i])==0)
				{
						isFound = 1;
						loginInfo[j].times++;
						break;	
				}
				j++;
		}
		if(isFound == 0)
		{
			
			strcpy(loginInfo[j].username,username[i]);
			
			loginInfo[j].times = 1;
			
		}
	}
			
	j=0;
	iNum=0;
	if(iexport == 0)
	{
		while(j < 300000 &&(strlen(loginInfo[j].username) != 0))
		{
			iNum++;
				 if(iNum > 1)
				 {
					 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
				 }
				 
			 	   utPltPutLoopVar(psDbHead,"username",iNum,loginInfo[j].username);
			 	   utPltPutLoopVarF(psDbHead,"logintimes",iNum,"%d",loginInfo[j].times);
			j++;
		}
			utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_userlog_frequencyreport.htm");
	}
	else
		{
			
			strcpy(caFilename, "Custmeranalyze.csv");
			strcpy(caPath, FILEPATH);
  		sprintf(caFile, "%s/%s", caPath, caFilename);
  		
			fp = fopen(caFile, "w");
			
			if(fp == NULL){
		 		 		 pasDbCloseCursor(psCur);
			       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",(char *)ncLang("0546���ݵ�������"),(char *)ncLang("0546���ݵ�������"));
			       return 0;	
			       
		 		 }
		 		 
		 		 //fprintf(fp,"����ϵͳ����ͼ��\n");
		 		 //fprintf(fp,"ʱ��: %s -- %s\n",sdate,edate);
				 ncUtlPrintCsv(fp,2,UT_TYPE_STRING,"�û���",
				 									  UT_TYPE_STRING,"����Ƶ��");
			
					while(j < 300000 &&(strlen(loginInfo[j].username) != 0))
				  {
			
				 		 ncUtlPrintCsv(fp,2,UT_TYPE_STRING, loginInfo[j].username,
		                        		UT_TYPE_LONG,loginInfo[j].times);
								
						j++;
				  }
				
				fclose(fp);
				
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        
        //remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
		}
	return 0;
}

static int pasSendHttpGetStr(char *pHost,char *pUrl,int iMaxLen,char *pOut)
{
    char caBuf[4096];
    char caPort[12],caHost[128];
    int iSock,iReturn;
    char *p;

    p = (char *)strchr(pHost,':');
    if(p == NULL) {
        strcpy(caPort,"80");
        strcpy(caHost,pHost);
    }
    else {
        memcpy(caHost,pHost,p-pHost);
        caHost[p-pHost]=0;
        strcpy(caPort,p+1);
    }
    printf("host is : %s \n port is : %s\n  url is : %s \n" , caHost ,caPort,pUrl);
    iSock = utComTcpConnect(caHost,caPort);
    if(iSock < 0) {
        printf("Connect Host %s error \n",pHost);
        return (-1);
    }
    memset(caBuf,0,4096);
    if(pUrl == NULL || utStrIsSpaces(pUrl)) {
        utStrAddF(caBuf,"GET / HTTP/1.1\r\n");
    }
    else {
        utStrAddF(caBuf,"GET %s HTTP/1.1\r\n",pUrl);
    }
    utStrAddF(caBuf,"Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*\r\n");
    utStrAddF(caBuf,"Accept-Language: zh-cn\r\n");
    utStrAddF(caBuf,"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; InfoPath.2)\r\n");
    utStrAddF(caBuf,"Host: %s\r\n\r\n",pHost);
    iReturn = utComTcpWrite(iSock,caBuf,strlen(caBuf));
    if(iReturn < 0) {
        close(iSock);
        return (-1);
    }
    iReturn = read(iSock,pOut,iMaxLen);
    close(iSock);
    return iReturn;
}



int ncm_website_visit_type(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	utMsgPrintMsg(psMsgHead);
	utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	char sql[4096] = "";
	pasDbCursor *psCur = NULL;
	
	int iret=0;
	int iNum=0;	 
	
	char sdate[32] = ""; // ����Ŀ�ʼʱ��
	char edate[32] = ""; // ����Ľ���ʱ��
	char pUrl[200] = "";
	char resultAll[8000] = "";
	char* tempStr;
	char* tempStr2;
	char resultJson[8000] = "";
	char hostIp[100] = "";
	char sdate_all[20],edate_all[20];
	long lstime,letime;
	utMsgGetSomeNVar(psMsgHead, 3,"start_time",        UT_TYPE_STRING,  sizeof(sdate)-1,           sdate,
															   "end_time",        UT_TYPE_STRING,  sizeof(edate)-1,           edate,
															   "hostIp",        UT_TYPE_STRING,  sizeof(hostIp)-1,           hostIp);
   
//  sprintf(sdate_all,"%s 00:00",sdate);
//  sprintf(edate_all,"%s 23:59",edate);
//    lstime=utTimStrToLong("%Y/%m/%d %H:%M",sdate_all);
//    letime=utTimStrToLong("%Y/%m/%d %H:%M",edate_all);
	sprintf(pUrl , "/pronline/Msg?FunName@ncWebShowWebClass_hongkong&start_time@%s&end_time@%s" , sdate ,edate);
	printf("PURL is : %s" , pUrl);
	memset(resultAll,0,sizeof(resultAll));
	pasSendHttpGetStr(hostIp,pUrl,7000,resultAll);
	
	ncUtlGetWordBetween(resultAll,"<joson>","</joson>",resultJson,6000);
	
	printf("resultAll=%s\n",resultAll);
/*
	printf("resultAll is :%s \n" ,resultAll);
	tempStr = strchr(resultAll , '{');
	printf("tempStr is :%s \n" ,tempStr);
	tempStr2 = strstr(tempStr , "]}");
	printf("tempStr2 is :%s \n" ,tempStr2);
	strncpy(resultJson , tempStr, tempStr2-tempStr + 2);
*/
  printf("resultJson is :%s \n" ,resultJson);
  

  utPltPutVar(psDbHead,"resultJson",resultJson);
  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_chart/website_visit_type.htm");
	return 0;
}