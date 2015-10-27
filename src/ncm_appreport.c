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

#define FILEPATH "/home/ncmysql/ncsrv/html/ncm_appreport"

int ncTimeToLong(char *stime, unsigned long *lTime);
/*{
	 char ctime[20] = "";
	 
	 snprintf(ctime, sizeof(ctime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", stime[0], stime[1], stime[2], stime[3], stime[5], stime[6], stime[8], stime[9], stime[11], stime[12], stime[14], stime[15], stime[17], stime[18]);	
	 *lTime = utTimStrToLong("%Y%m%d%H%M%S", ctime);
	 
	 return 0;
}*/

int ncLogTimeToDate(char *stime, char *etime)
{
	 char ctime[10] = "";
	 snprintf(ctime, sizeof(ctime), "%c%c%c%c%c%c%c%c", stime[16], stime[17], stime[18], stime[19], stime[20], stime[21], stime[22], stime[23]);	
	 strcpy(etime, ctime);
	 
	 return 0;
}

int ncDaysToLong(unsigned long lTime, int days, unsigned long *lStartTime);
/*{
		char sdate[33] = "";
   	strcpy(sdate, utTimFormat("%Y-%m-%d", lTime));//��ǰ����
   	strcat(sdate, " 00:00:00");
   	*lStartTime = utTimStrToLong("%Y/%m/%d %H:%M:%S", sdate) - (days-1) * 86400;
   	
   	return 0;	
}*/

// ��������
int ncm_web_App_intDetailReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char sdate[32] = "";
	 char edate[32] = "";
	 char slogdate[64] = "";
	 char elogdate[64] = "";
	 char slogtime[64] = "";
	 char elogtime[64] = "";
	 char username[32] = "";
	 char mac[18] = "";
	 char ip[16] = "";
	 char dev[32] = "";
	 char os[32] = "";
	 char starttime[64] = "";
	 char endtime[64] = "";
	 char traffic[64] = "";
	 char groupcode[32] = "";
	 char areaid[32] = "";
	 char groupid[32] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 char caUsername[32] = "";
   char caMac[18] = "";
   char caIp[16] = "";
   char caDev[24] = "";
   char caOs[16] = "";
   char caStart[32] = "";
   char caEnd[32] = "";
   unsigned long lUbytes = 0;
	 unsigned long lDbytes = 0;
	 double dBytes = 0.0;
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // ��ǰʱ��(Ĭ�Ͻ���ʱ��)
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	  // �ؼ���
	 char _keyword[33] = "";
	 char caSesid[1024] = "";
	 
	 char caShopids[1024];
	 strcpy(caShopids,getDsShopids());
	 
	 // ���ݱ�
	 unsigned long lCount1 = 0; // ���м�¼�ĸ���
	 long iReturn; // ����¼��ѯ
	 char start[17] = ""; // Ĭ�ϼ�¼��0��ʼ
	 char limit[17] = ""; // �޶�һҳ�еļ�¼��40��
	 unsigned long iStart = 0; // ת�����Ĭ�Ͽ�ʼatoi(start)
	 unsigned long iLimit = 0; // ת������޶���¼����atoi(limit)
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
   // ��ҳ
	 unsigned long lTotRec = 0; // ��¼���е����ݵĸ���
   
   // ����
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // �̼�
   char caStype[8] = "";
	 char caOptd[64] = "";
	 unsigned long lCid = 0;
   iReturn = dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	 iReturn = dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStype)-1,caStype);
	 iReturn = dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
	 printf("lCid = %d\n", lCid);
	 printf("*******caStype = %s \n",caStype);
	 printf("*******caOptd = %s \n",caOptd);
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 24,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
															    "slogdate",    UT_TYPE_STRING,  sizeof(slogdate)-1,       slogdate,
															    "elogdate",    UT_TYPE_STRING,  sizeof(elogdate)-1,       elogdate,
															    "slogtime",    UT_TYPE_STRING,  sizeof(slogtime)-1,       slogtime,
															    "elogtime",    UT_TYPE_STRING,  sizeof(elogtime)-1,       elogtime,
																	"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
																	"mac",         UT_TYPE_STRING,  sizeof(mac)-1,            mac,
																	"ip",          UT_TYPE_STRING,  sizeof(ip)-1,             ip,
																	"dev",         UT_TYPE_STRING,  sizeof(dev)-1,       		  dev,
																	"os",          UT_TYPE_STRING,  sizeof(os)-1,             os,
																	"starttime",   UT_TYPE_STRING,  sizeof(starttime)-1,      starttime,
																	"endtime",     UT_TYPE_STRING,  sizeof(endtime)-1,        endtime,
																	"traffic",     UT_TYPE_STRING,  sizeof(traffic)-1,        traffic,
																	"groupcode",   UT_TYPE_STRING,  sizeof(groupcode)-1,      groupcode,
																	"areaid",      UT_TYPE_STRING,  sizeof(areaid)-1,         areaid,
																	//"groupid",     UT_TYPE_STRING,  sizeof(groupid)-1,        groupid,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				 UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",    UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 
	 trim(_keyword);
	 
	 // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"starttime");
	 }
	 
	 // ��ȡ�����ļ�������ʱ��
	 char cnfSdate[32] = "";
	 char cnfEdate[32] = "";
	 char cnfPath[128] = "";
	 char *pConfig;
	 pasConfList *psConfig;
	 
	 pConfig = (char *)pasGetConfigFileName();
	 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
   if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","��ȡ�����ļ�",(char *)ncLang("0427�����ļ�����"));
        return 0;
   }
   //����û�����ã�Ĭ��null��
   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig�����һ������ΪStatisticsStim��Ĭ��ֵ
 	 strcpy(cnfEdate,(char *)pasUtlLookConfig(psConfig,"StatisticsEtime",""));
 	 strcpy(cnfPath,(char *)pasUtlLookConfig(psConfig,"ReportPosition",""));
   printf("cnfSdate = %s\n", cnfSdate);
	 printf("cnfEdate = %s\n", cnfEdate);
	 printf("cnfPath = %s\n", cnfPath);
   pasUtlFreeConfig(psConfig);
	 
	 if(strlen(cnfSdate)>0 && strlen(cnfEdate)>0){
	 		ncTimeToLong(cnfSdate, &lSdate);
			ncTimeToLong(cnfEdate, &lEdate);
			printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	 }else{
	 		// ת��ʱ��
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // Ĭ��ʱ���ǽ���0��--��ǰʱ��
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 // �鿴ʱ���Ƿ���ȷ
	 char _sdate[32] = ""; // ��ʽת����Ŀ�ʼʱ��
	 char _edate[32] = ""; // ��ʽת����ĵ�ǰʱ��
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 sprintf(sql, "select count(*) from ncsrvuserlog where starttime>%lu and endtime<%lu",lSdate, lEdate);
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username != 'Guest'");
	 if(!utStrIsSpaces(username)) // �����û���¼����ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(mac)) // ����mac��ַ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", mac);
	 }
	 if(!utStrIsSpaces(ip)) // ����ip��ַ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 if(!utStrIsSpaces(dev)) // �����豸���Ͳ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev like '%%%s%%'", dev);
	 }
	 if(!utStrIsSpaces(os)) // ���ݲ���ϵͳ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and os like '%%%s%%'", os);
	 }
	 
	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
	 
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 // ����
	 if(strlen(caExport)>0){
	 		 strcpy(caFilename, "intSurfDetailReport.csv");
	 		 if(strlen(cnfPath)>0){
	 		 		strcpy(caPath, cnfPath);
	 		 }else{
	 		 		strcpy(caPath, FILEPATH);
	 		 }
	 		 sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",(char *)ncLang("0546���ݵ�������"),(char *)ncLang("0546���ݵ�������"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"�û�������ϸ����\n");
			 fprintf(fp,"\t\tʱ��:%s--%s\n", sdate, edate);
			 ncUtlPrintCsv(fp,8,UT_TYPE_STRING,"�û��˺�",
			                    UT_TYPE_STRING,"MAC��ַ",
			                    UT_TYPE_STRING,"IP",
			                    UT_TYPE_STRING,"�豸����",
			                    UT_TYPE_STRING,"����ϵͳ",
			                    UT_TYPE_STRING,"��¼ʱ��",
			                    UT_TYPE_STRING,"�˳�ʱ��",
			                    UT_TYPE_STRING,"��������(mb)");
	 }
	 
	 if((lEdate-lSdate)<=86400){ // һ��֮��, ʱ����ʾ(13:23:23)
	 		sprintf(sql, "select username, mac, ip, dev, os, date_format(from_unixtime(starttime), '%T') as start, date_format(from_unixtime(endtime), '%T') as end, ubytes, dbytes from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate);	
	 }else{ // ����һ��, ʱ����ʾ(2013/02/12 13:23:23)
	 		sprintf(sql, "select username, mac, ip, dev, os, from_unixtime(starttime) as start, from_unixtime(endtime) as end, ubytes, dbytes from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate);	                    
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username != 'Guest'");
	 if(!utStrIsSpaces(username)) // �����û���¼����ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(mac)) // ����mac��ַ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", mac);
	 }
	 if(!utStrIsSpaces(ip)) // ����ip��ַ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 if(!utStrIsSpaces(dev)) // �����豸���Ͳ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev like '%%%s%%'", dev);
	 }
	 if(!utStrIsSpaces(os)) // ���ݲ���ϵͳ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and os like '%%%s%%'", os);
	 }
	 
	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
	 
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
												 		   UT_TYPE_STRING, sizeof(caMac)-1,       caMac,
												 		   UT_TYPE_STRING, sizeof(caIp)-1,        caIp,
												 		   UT_TYPE_STRING, sizeof(caDev)-1,  			caDev,
												 		   UT_TYPE_STRING, sizeof(caOs)-1,  			caOs,
												 		   UT_TYPE_STRING, sizeof(caStart)-1,  		caStart,
												 		   UT_TYPE_STRING, sizeof(caEnd)-1,  			caEnd,
															 UT_TYPE_ULONG,  sizeof(long),          &lUbytes,
															 UT_TYPE_ULONG,  sizeof(long),          &lDbytes);
	 
	 while(0 == iret || 1405 == iret){  
  	   iNum++;
  	   if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
			 dBytes = (double)(lUbytes+lDbytes)/(1024*1024);
			 
			 utPltSetCvtHtml(1);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
			 utPltPutLoopVar(psDbHead,"ip",iNum,caIp);
			 utPltPutLoopVar(psDbHead,"dev",iNum,caDev);
			 utPltPutLoopVar(psDbHead,"os",iNum,caOs);
			 utPltSetCvtHtml(0);
			 utPltPutLoopVar(psDbHead,"starttime",iNum,caStart);
			 utPltPutLoopVar(psDbHead,"endtime",iNum,caEnd);
			 utPltPutLoopVarF(psDbHead,"traffic",iNum,"%.2f",dBytes);
			 if(strlen(caExport)>0){
				 		ncUtlPrintCsv(fp,8,UT_TYPE_STRING,caUsername,
													 		UT_TYPE_STRING,caMac,
													 		UT_TYPE_STRING,caIp,
		                          UT_TYPE_STRING,caDev,
		                          UT_TYPE_STRING,caOs,
		                          UT_TYPE_STRING,caStart,
		                          UT_TYPE_STRING,caEnd,
		                          UT_TYPE_FLOAT,dBytes);
			 }
			 
			 memset(caUsername, 0, sizeof(caUsername));
			 memset(caMac, 0, sizeof(caMac));
			 memset(caIp, 0, sizeof(caIp));
			 memset(caDev, 0, sizeof(caDev));
			 memset(caOs, 0, sizeof(caOs));
			 memset(caStart, 0, sizeof(caStart));
			 memset(caEnd, 0, sizeof(caEnd));
		   lUbytes = 0;
			 lDbytes = 0;
			 dBytes = 0.0;
			 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
														 		   UT_TYPE_STRING, sizeof(caMac)-1,       caMac,
														 		   UT_TYPE_STRING, sizeof(caIp)-1,        caIp,
														 		   UT_TYPE_STRING, sizeof(caDev)-1,  			caDev,
														 		   UT_TYPE_STRING, sizeof(caOs)-1,  			caOs,
														 		   UT_TYPE_STRING, sizeof(caStart)-1,  		caStart,
														 		   UT_TYPE_STRING, sizeof(caEnd)-1,  			caEnd,
																	 UT_TYPE_ULONG,  sizeof(long),          &lUbytes,
																	 UT_TYPE_ULONG,  sizeof(long),          &lDbytes);
   }
	 //utPltShowDb(psDbHead); 
	 pasDbCloseCursor(psCur);
	 
	 if(strlen(caExport)>0){
 	    fclose(fp);
      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
      remove(caFile);
      utPltFreeDb(psDbHead);
      return 0;
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_appreport/ncm_web_App_intDetailReport.htm");
	 return 0;	
}

// ������������
int ncm_web_App_intStatisticsReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char sdate[32] = "";
	 char edate[32] = "";
	 char slogdate[64] = "";
	 char elogdate[64] = "";
	 char slogtime[64] = "";
	 char elogtime[64] = "";
	 char timeV[32] = "";
	 char number[32] = "";
	 char traffic[64] = "";
	 char groupcode[32] = "";
	 char areaid[32] = "";
	 char groupid[32] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long usernum = 0;
	 char ptime[32] = "";
	 unsigned long traf = 0;
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0);
	 
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caSesid[1024] = "";
	 char caDel[16] = "";
	  // �ؼ���
	 char _keyword[33] = "";
	 
	 // ���ݱ�
	 unsigned long lCount1 = 0; // ���м�¼�ĸ���
	 long iReturn; // ����¼��ѯ
	 char start[17] = ""; // Ĭ�ϼ�¼��0��ʼ
	 char limit[17] = ""; // �޶�һҳ�еļ�¼��40��
	 unsigned long iStart = 0; // ת�����Ĭ�Ͽ�ʼatoi(start)
	 unsigned long iLimit = 0; // ת������޶���¼����atoi(limit)
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
   // ��ҳ
	 unsigned long lTotRec = 0; // ��¼���е����ݵĸ���
   
   // ����
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // �̼�
   char caStype[8] = "";
	 char caOptd[64] = "";
	 unsigned long lCid = 0;
   iReturn = dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	 iReturn = dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStype)-1,caStype);
	 iReturn = dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
	 printf("lCid = %d\n", lCid);
	 printf("*******caStype = %s \n",caStype);
	 printf("*******caOptd = %s \n",caOptd);
	 
	 // ������¼��Ϣ�ṹ��
	 int i = 0;
	 typedef struct int_stat{
	 		int num;
	 		char timeV[20];
	 		long number;
	 		double traffic;
	 }int_stat_s;
	 int_stat_s stat_int[25];
	 
	 for (i = 0;i < 24;i++){ // ��ʼ��
		 	 stat_int[i].num = i;
		   sprintf(stat_int[i].timeV,"%02d:00:00-%02d:59:59",i,i);
		   stat_int[i].number = 0;
		   stat_int[i].traffic = 0.0;
		   //printf("num = %d, timeV = %s, number = %d, traffic = %.2f\n", stat_int[i].num, stat_int[i].timeV, stat_int[i].number, stat_int[i].traffic);
	 }
	 stat_int[24].num = 24;
   sprintf(stat_int[24].timeV,"�ܼ�");
   stat_int[24].number = 0;
   stat_int[24].traffic = 0.0;
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 19,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
															    "slogdate",    UT_TYPE_STRING,  sizeof(slogdate)-1,       slogdate,
															    "elogdate",    UT_TYPE_STRING,  sizeof(elogdate)-1,       elogdate,
															    "slogtime",    UT_TYPE_STRING,  sizeof(slogtime)-1,       slogtime,
															    "elogtime",    UT_TYPE_STRING,  sizeof(elogtime)-1,       elogtime,
																	"time",        UT_TYPE_STRING,  sizeof(timeV)-1,          timeV,
																	"number",      UT_TYPE_STRING,  sizeof(number)-1,         number,
																	"traffic",     UT_TYPE_STRING,  sizeof(traffic)-1,        traffic,
																	"groupcode",   UT_TYPE_STRING,  sizeof(groupcode)-1,      groupcode,
																	"areaid",      UT_TYPE_STRING,  sizeof(areaid)-1,         areaid,
																	//"groupid",     UT_TYPE_STRING,  sizeof(groupid)-1,        groupid,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				 UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",    UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 
	 trim(_keyword);
	 
	 // ��ȡ�����ļ�������ʱ��
	 char cnfSdate[32] = "";
	 char cnfEdate[32] = "";
	 char cnfPath[128] = "";
	 char *pConfig;
	 pasConfList *psConfig;
	 
	 pConfig = (char *)pasGetConfigFileName();
	 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
   if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","��ȡ�����ļ�",(char *)ncLang("0427�����ļ�����"));
        return 0;
   }
   //����û�����ã�Ĭ��null��
   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig�����һ������ΪStatisticsStim��Ĭ��ֵ
 	 strcpy(cnfEdate,(char *)pasUtlLookConfig(psConfig,"StatisticsEtime",""));
 	 strcpy(cnfPath,(char *)pasUtlLookConfig(psConfig,"ReportPosition",""));
   printf("cnfSdate = %s\n", cnfSdate);
	 printf("cnfEdate = %s\n", cnfEdate);
	 printf("cnfPath = %s\n", cnfPath);
   pasUtlFreeConfig(psConfig);
	 
	 if(strlen(cnfSdate)>0 && strlen(cnfEdate)>0){
	 		ncTimeToLong(cnfSdate, &lSdate);
			ncTimeToLong(cnfEdate, &lEdate);
			printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	 }else{
	 		// ת��ʱ��
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // Ĭ��ʱ���ǽ���0��--��ǰʱ��
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 char _sdate[32] = ""; // ��ʽת����Ŀ�ʼʱ��
	 char _edate[32] = ""; // ��ʽת����ĵ�ǰʱ��
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 // ����
	 if(strlen(caExport)>0){
	 		 strcpy(caFilename, "intStatisticsReport.csv");
	 		 if(strlen(cnfPath)>0){
	 		 		strcpy(caPath, cnfPath);
	 		 }else{
	 		 		strcpy(caPath, FILEPATH);
	 		 }
	 		 sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",(char *)ncLang("0546���ݵ�������"),(char *)ncLang("0546���ݵ�������"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"����������������\n");
			 fprintf(fp,"\t\tʱ��:%s--%s\n", _sdate, _edate);
			 ncUtlPrintCsv(fp,5,UT_TYPE_STRING,"ʱ��",
			 									  UT_TYPE_STRING,"����",
			                    UT_TYPE_STRING,"����(Mb)");
	 }
	 
	 for(i=0; i<24; i++){
			 memset(sql, 0, sizeof(sql));
			 sprintf(sql, "select count(*) as usernum, %d as ptime from ncsrvuserlog where starttime>%lu and endtime<%lu", i, lSdate, lEdate);		                    
			 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and hour(from_unixtime(starttime)) <= %d and hour(from_unixtime(endtime)) >= %d", i, i);
			 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
			 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by ptime");
			 //printf("sql = %s\n", sql);
			 psCur = pasDbOpenSql(sql, 0);
			 if(psCur==NULL){
			 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
		       return 0;	
			 }
			 
			 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
			 iret = pasDbFetchInto(psCur, UT_TYPE_ULONG,  sizeof(long),    &usernum, 
							 		 									UT_TYPE_STRING, sizeof(ptime)-1, ptime);
			 
			 while(0 == iret || 1405 == iret){
					 stat_int[i].number = usernum;
					 stat_int[24].number += usernum;
					 
					 usernum = 0;
					 memset(ptime, 0, sizeof(ptime));
					 iret = pasDbFetchInto(psCur, UT_TYPE_ULONG,  sizeof(long),    &usernum, 
							 		 											UT_TYPE_STRING, sizeof(ptime)-1, ptime);
			 }
			 pasDbCloseCursor(psCur);
	 }
	 
	 for(i=0; i<24; i++){
	 		 memset(sql, 0, sizeof(sql));
			 sprintf(sql, "select sum(ubytes+dbytes) as traf, %d as ptime from ncsrvuserlog where starttime>%lu and endtime<%lu", i, lSdate, lEdate);		                    
			 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and hour(from_unixtime(starttime)) <= %d and hour(from_unixtime(endtime)) >= %d", i, i);
			 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
			 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by ptime");
			 //printf("sql = %s\n", sql);
			 psCur = pasDbOpenSql(sql, 0);
			 if(psCur==NULL){
			 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
		       return 0;	
			 }
			 
			 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),    &traf,
																	 UT_TYPE_STRING, sizeof(ptime)-1, ptime);
			 
	 		 while(0 == iret || 1405 == iret){
		  		 stat_int[i].traffic = (double)traf/(1024*1024);
		  		 stat_int[24].traffic += (double)traf/(1024*1024);
		  		 
		  		 traf = 0;
		  		 memset(ptime, 0, sizeof(ptime));
		  		 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),    &traf,
																	     UT_TYPE_STRING, sizeof(ptime)-1, ptime);
		   }
	 		 pasDbCloseCursor(psCur);	
	 }
	 
	 for(i=0; i<25; i++){
	 		iNum++;
  	  if(iNum > 1)
			{
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			}
			utPltPutLoopVar(psDbHead,"time",iNum,stat_int[i].timeV);
			utPltPutLoopVarF(psDbHead,"number",iNum,"%lu",stat_int[i].number);
			utPltPutLoopVarF(psDbHead,"traffic",iNum,"%.2f",stat_int[i].traffic);
			if(strlen(caExport)>0){
					 		ncUtlPrintCsv(fp,3,UT_TYPE_STRING,stat_int[i].timeV,
					 											UT_TYPE_ULONG,stat_int[i].number,
														 		UT_TYPE_FLOAT,stat_int[i].traffic);
			}
	 }
	 //utPltShowDb(psDbHead);
	 
	 if(strlen(caExport)>0){
 	    fclose(fp);
      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
      remove(caFile);
      utPltFreeDb(psDbHead);
      return 0;
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_appreport/ncm_web_App_intStatisticsReport.htm");
	 return 0;
}

// �豸����
int ncm_web_App_intDeviceReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 int iNumber = 0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char sdate[32] = "";
	 char edate[32] = "";
	 char slogdate[64] = "";
	 char elogdate[64] = "";
	 char slogtime[64] = "";
	 char elogtime[64] = "";
	 char dev[32] = "";
	 char devtype[32] = "";
	 char num[20] = "";
	 char groupcode[32] = "";
	 char areaid[32] = "";
	 char groupid[32] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long lCount2 = 0;
	 char caDev[24] = "";
	 char caDevtype[8] = "";
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // ��ǰʱ��(Ĭ�Ͻ���ʱ��)
	 
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caSesid[1024] = "";
	 char caDel[16] = "";
	  // �ؼ���
	 char _keyword[33] = "";
	 
	 // ���ݱ�
	 unsigned long lCount1 = 0; // ���м�¼�ĸ���
	 long iReturn; // ����¼��ѯ
	 char start[17] = ""; // Ĭ�ϼ�¼��0��ʼ
	 char limit[17] = ""; // �޶�һҳ�еļ�¼��40��
	 unsigned long iStart = 0; // ת�����Ĭ�Ͽ�ʼatoi(start)
	 unsigned long iLimit = 0; // ת������޶���¼����atoi(limit)
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
   // ��ҳ
	 unsigned long lTotRec = 0; // ��¼���е����ݵĸ���
   
   // ����
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // �̼�
   char caStype[8] = "";
	 char caOptd[64] = "";
	 unsigned long lCid = 0;
	 char caShopids[1024];
	 strcpy(caShopids,getDsShopids());
	 
   iReturn = dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	 iReturn = dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStype)-1,caStype);
	 iReturn = dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
	 printf("lCid = %d\n", lCid);
	 printf("*******caStype = %s \n",caStype);
	 printf("*******caOptd = %s \n",caOptd);
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 19,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
															    "slogdate",    UT_TYPE_STRING,  sizeof(slogdate)-1,       slogdate,
															    "elogdate",    UT_TYPE_STRING,  sizeof(elogdate)-1,       elogdate,
															    "slogtime",    UT_TYPE_STRING,  sizeof(slogtime)-1,       slogtime,
															    "elogtime",    UT_TYPE_STRING,  sizeof(elogtime)-1,       elogtime,
																	"dev",         UT_TYPE_STRING,  sizeof(dev)-1,       		  dev,
																	"devtype",     UT_TYPE_STRING,  sizeof(devtype)-1,        devtype,
																	"num",         UT_TYPE_STRING,  sizeof(num)-1,            num,
																	"groupcode",   UT_TYPE_STRING,  sizeof(groupcode)-1,      groupcode,
																	"areaid",      UT_TYPE_STRING,  sizeof(areaid)-1,         areaid,
																	//"groupid",     UT_TYPE_STRING,  sizeof(groupid)-1,        groupid,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				 UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",    UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 
	 trim(_keyword);
	 
	 // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"starttime");
	 }
	 
	 // ��ȡ�����ļ�������ʱ��
	 char cnfSdate[32] = "";
	 char cnfEdate[32] = "";
	 char cnfPath[128] = "";
	 char *pConfig;
	 pasConfList *psConfig;
	 
	 pConfig = (char *)pasGetConfigFileName();
	 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
   if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","��ȡ�����ļ�",(char *)ncLang("0427�����ļ�����"));
        return 0;
   }
   //����û�����ã�Ĭ��null��
   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig�����һ������ΪStatisticsStim��Ĭ��ֵ
 	 strcpy(cnfEdate,(char *)pasUtlLookConfig(psConfig,"StatisticsEtime",""));
 	 strcpy(cnfPath,(char *)pasUtlLookConfig(psConfig,"ReportPosition",""));
   printf("cnfSdate = %s\n", cnfSdate);
	 printf("cnfEdate = %s\n", cnfEdate);
	 printf("cnfPath = %s\n", cnfPath);
   pasUtlFreeConfig(psConfig);
	 
	 if(strlen(cnfSdate)>0 && strlen(cnfEdate)>0){
	 		ncTimeToLong(cnfSdate, &lSdate);
			ncTimeToLong(cnfEdate, &lEdate);
			printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	 }else{
	 		// ת��ʱ��
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // Ĭ��ʱ���ǽ���0��--��ǰʱ��
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 char _sdate[32] = ""; // ��ʽת����Ŀ�ʼʱ��
	 char _edate[32] = ""; // ��ʽת����ĵ�ǰʱ��
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 sprintf(sql, "select count(*) from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate);
	 if(!utStrIsSpaces(devtype)) // �����豸���Ͳ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and devtype like '%%%s%%'", devtype);
	 }
	 if(!utStrIsSpaces(dev)) // �����豸���Ʋ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev like '%%%s%%'", dev);
	 }
	 
	  if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
	 
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 //lTotRec = lCount1;
	 //utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 // ����
	 if(strlen(caExport)>0){
	 		 strcpy(caFilename, "intDeviceReport.csv");
	 		 if(strlen(cnfPath)>0){
	 		 		strcpy(caPath, cnfPath);
	 		 }else{
	 		 		strcpy(caPath, FILEPATH);
	 		 }
	 		 sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
	 		 		 printf("���ļ�ʧ�ܣ�\n");
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",(char *)ncLang("0546���ݵ�������"),(char *)ncLang("0546���ݵ�������"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"�����豸���ͱ���\n");
			 fprintf(fp,"\t\tʱ��:%s--%s\n", sdate, edate);
			 ncUtlPrintCsv(fp,3,UT_TYPE_STRING,"�豸����",
			                    UT_TYPE_STRING,"�豸����",
			                    UT_TYPE_STRING,"����");
	 }
	 
	 sprintf(sql, "select count(*) as lCount2, dev, devtype from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate);		                    
	 if(!utStrIsSpaces(devtype)) // �����豸���Ͳ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and devtype like '%%%s%%'", devtype);
	 }
	 if(!utStrIsSpaces(dev)) // �����豸���Ʋ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev like '%%%s%%'", dev);
	 }
	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by devtype order by %s %s", sort, dir);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),       &lCount2,
															 UT_TYPE_STRING, sizeof(caDev)-1,  		caDev,
															 UT_TYPE_STRING, sizeof(caDevtype)-1, caDevtype);
				
	 		 while(0 == iret || 1405 == iret){
		  	   iNum++;
		  	   if(iNum > 1)
					 {
						 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
					 }
					 
					 if(strlen(caDevtype)==0){
					 		strcpy(caDevtype, "����");	
					 }
					 if(strlen(caDev)==0){
					 		strcpy(caDev, "����");	
					 }
					 utPltSetCvtHtml(1);
					 utPltPutLoopVar(psDbHead,"dev",iNum,caDev);
					 if(strcmp(caDevtype,"Mobile")==0){strcpy(caDevtype,"�ֻ�");}
					 if(strcmp(caDevtype,"mspc")==0){strcpy(caDevtype,"����");}
					 if(strcmp(caDevtype,"pad")==0){strcpy(caDevtype,"ƽ��");}	
					 utPltPutLoopVar(psDbHead,"devtype",iNum,caDevtype);
					 utPltSetCvtHtml(0);
					 utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",lCount2);
					 if(strlen(caExport)>0){
					 		ncUtlPrintCsv(fp,3,UT_TYPE_STRING,caDevtype,
														 		UT_TYPE_STRING,caDev,
														 		UT_TYPE_ULONG,lCount2);
					 }
					 
					 lCount2 = 0;
					 memset(caDev, 0, sizeof(caDev));
					 memset(caDevtype, 0, sizeof(caDevtype));
					 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),       &lCount2,
																			 UT_TYPE_STRING, sizeof(caDev)-1,  		caDev,
																			 UT_TYPE_STRING, sizeof(caDevtype)-1, caDevtype);
		  }
	 utPltPutVarF(psDbHead,"TotRec", "%lu", iNum);
	 //utPltShowDb(psDbHead); 
	 pasDbCloseCursor(psCur);
	 
	 if(strlen(caExport)>0){
 	    fclose(fp);
      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
      remove(caFile);
      utPltFreeDb(psDbHead);
      return 0;
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_appreport/ncm_web_App_intDeviceReport.htm");
	 return 0;
}

// ��������
int ncm_web_App_smsDetailReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 int iNumber = 0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char sdate[32] = "";
	 char edate[32] = "";
	 char slogdate[64] = "";
	 char elogdate[64] = "";
	 char slogtime[64] = "";
	 char elogtime[64] = "";
	 char contrycode[32] = "";
	 char username[32] = "";
	 char usermac[18] = "";
	 char ip[16] = "";
	 char logtime[64] = "";
	 char sptype[32] = "";
	 char areaname[32] = "";
	 char groupcode[32] = "";
	 char areaid[32] = "";
	 char groupid[32] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 char caUsername[32] = "";
	 char caUsermac[18] = "";
	 char caIp[16] = "";
	 char caTime[32] = "";
	 unsigned long lSptype = 0;
	 unsigned long lContrycode = 0;
	 char caAreaname[32] = "";
	 char caApmac[64] = "";
	 char s_sptype[32] = "";
	 ncPortalAp     *psAp = NULL;
	 char           s_apaddr[255] = ""; 
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // ��ǰʱ��(Ĭ�Ͻ���ʱ��)
	 char caShopids[1024];
	 strcpy(caShopids,getDsShopids());
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caSesid[1024] = "";
	 char caDel[16] = "";
	  // �ؼ���
	 char _keyword[33] = "";
	 
	 // ���ݱ�
	 unsigned long lCount1 = 0; // ���м�¼�ĸ���
	 long iReturn; // ����¼��ѯ
	 char start[17] = ""; // Ĭ�ϼ�¼��0��ʼ
	 char limit[17] = ""; // �޶�һҳ�еļ�¼��40��
	 unsigned long iStart = 0; // ת�����Ĭ�Ͽ�ʼatoi(start)
	 unsigned long iLimit = 0; // ת������޶���¼����atoi(limit)
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
   // ��ҳ
	 unsigned long lTotRec = 0; // ��¼���е����ݵĸ���
   
   // ����
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 // �̼�
   char caStype[8] = "";
	 char caOptd[64] = "";
	 unsigned long lCid = 0;
   iReturn = dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	 iReturn = dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStype)-1,caStype);
	 iReturn = dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
	 printf("lCid = %d\n", lCid);
	 printf("*******caStype = %s \n",caStype);
	 printf("*******caOptd = %s \n",caOptd);
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 24,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
															    "slogdate",    UT_TYPE_STRING,  sizeof(slogdate)-1,       slogdate,
															    "elogdate",    UT_TYPE_STRING,  sizeof(elogdate)-1,       elogdate,
															    "slogtime",    UT_TYPE_STRING,  sizeof(slogtime)-1,       slogtime,
															    "elogtime",    UT_TYPE_STRING,  sizeof(elogtime)-1,       elogtime,
																	"contrycode",  UT_TYPE_STRING,  sizeof(contrycode)-1,     contrycode,
																	"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
																	"usermac",     UT_TYPE_STRING,  sizeof(usermac)-1,        usermac,
																	"ip",          UT_TYPE_STRING,  sizeof(ip)-1,             ip,
																	"logtime",     UT_TYPE_STRING,  sizeof(logtime)-1,        logtime,
																	"sptype",      UT_TYPE_STRING,  sizeof(sptype)-1,         sptype,
																	"areaname",    UT_TYPE_STRING,  sizeof(areaname)-1,       areaname,
																	"groupcode",   UT_TYPE_STRING,  sizeof(groupcode)-1,      groupcode,
																	"areaid",      UT_TYPE_STRING,  sizeof(areaid)-1,         areaid,
																	"groupid",     UT_TYPE_STRING,  sizeof(groupid)-1,        groupid,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				 UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",    UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 
	 trim(_keyword);
	 
	 // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"logtime");
	 }
	 
	 // ��ȡ�����ļ�������ʱ��
	 char cnfSdate[32] = "";
	 char cnfEdate[32] = "";
	 char cnfPath[128] = "";
	 char *pConfig;
	 pasConfList *psConfig;
	 
	 pConfig = (char *)pasGetConfigFileName();
	 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
   if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","��ȡ�����ļ�",(char *)ncLang("0427�����ļ�����"));
        return 0;
   }
   //����û�����ã�Ĭ��null��
   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig�����һ������ΪStatisticsStim��Ĭ��ֵ
 	 strcpy(cnfEdate,(char *)pasUtlLookConfig(psConfig,"StatisticsEtime",""));
 	 strcpy(cnfPath,(char *)pasUtlLookConfig(psConfig,"ReportPosition",""));
   printf("cnfSdate = %s\n", cnfSdate);
	 printf("cnfEdate = %s\n", cnfEdate);
	 printf("cnfPath = %s\n", cnfPath);
   pasUtlFreeConfig(psConfig);
	 
	 if(strlen(cnfSdate)>0 && strlen(cnfEdate)>0){
	 		ncTimeToLong(cnfSdate, &lSdate);
			ncTimeToLong(cnfEdate, &lEdate);
			printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	 }else{
	 		// ת��ʱ��
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // Ĭ��ʱ���ǽ���0��--��ǰʱ��
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 char _sdate[32] = ""; // ��ʽת����Ŀ�ʼʱ��
	 char _edate[32] = ""; // ��ʽת����ĵ�ǰʱ��
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 sprintf(sql, "select count(*) from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);
	 if(!utStrIsSpaces(contrycode)) // ���ݹ��Ҵ����ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and contrycode like '%%%s%%'", contrycode);
	 }
	 if(!utStrIsSpaces(username)) // �����û���¼����ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(usermac)) // ����mac��ַ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and usermac like '%%%s%%'", usermac);
	 }
	 if(!utStrIsSpaces(ip)) // ����ip��ַ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username != 'Guest'");
	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
	 
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 // ����
	 if(strlen(caExport)>0){
	 		 strcpy(caFilename, "smsDetailReport.csv");
	 		 if(strlen(cnfPath)>0){
	 		 		strcpy(caPath, cnfPath);
	 		 }else{
	 		 		strcpy(caPath, FILEPATH);
	 		 }
	 		 sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",(char *)ncLang("0546���ݵ�������"),(char *)ncLang("0546���ݵ�������"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"���ŷ�����ϸ����\n");
			 fprintf(fp,"\t\tʱ��:%s--%s\n", sdate, edate);
			 ncUtlPrintCsv(fp,7,UT_TYPE_STRING,"��������",
			 									  UT_TYPE_STRING,"�û��˺�",
			                    UT_TYPE_STRING,"MAC��ַ",
			                    UT_TYPE_STRING,"IP",
			                    UT_TYPE_STRING,"����ʱ��",
			                    UT_TYPE_STRING,"��Ӫ�����",
	                        UT_TYPE_STRING,"��������");
	 }
	 
	 if((lEdate-lSdate)<=86400){ // һ��֮��, ʱ����ʾ(13:23:23)
	 		sprintf(sql, "select username, usermac, ip, date_format(from_unixtime(logtime), '%T') as time, sptype, contrycode, areaname, apmac from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);
	 }else{ // ����һ��, ʱ����ʾ(2013/02/12 13:23:23)
	 		sprintf(sql, "select username, usermac, ip, from_unixtime(logtime) as time, sptype, contrycode, areaname, apmac from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);	                    
	 }
	 if(!utStrIsSpaces(contrycode)) // ���ݹ��Ҵ����ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and contrycode like '%%%s%%'", contrycode);
	 }
	 if(!utStrIsSpaces(username)) // �����û���¼����ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(usermac)) // ����mac��ַ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and usermac like '%%%s%%'", usermac);
	 }
	 if(!utStrIsSpaces(ip)) // ����ip��ַ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username != 'Guest'");
	  if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
	 
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " ");
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by sptype, areaname desc, %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caUsername)-1,  	caUsername,
															 UT_TYPE_STRING, sizeof(caUsermac)-1,  	  caUsermac,
															 UT_TYPE_STRING, sizeof(caIp)-1,  	      caIp,
															 UT_TYPE_STRING, sizeof(caTime)-1,  			caTime,
															 UT_TYPE_ULONG,  sizeof(long),  	        &lSptype,
															 UT_TYPE_ULONG,  sizeof(long),  	        &lContrycode,
															 UT_TYPE_STRING, sizeof(caAreaname)-1,  	caAreaname,
															 UT_TYPE_STRING, sizeof(caApmac)-1,       caApmac);
	
		while(0 == iret || 1405 == iret){
	     iNum++;
		   if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
			 if(strlen(caApmac)>0){
	  	 			char caapmac[64] = "";
	  	 			printf("caApmac = %s\n", caApmac);
	  	 			strcpy(caapmac, pasCvtMac(caApmac));
	  	 			printf("caapmac = %s\n", caapmac);
	  	 			psAp = (ncPortalAp *)ncSrvGetApByMac(psShmHead,caapmac);
	  	 			printf("psAp->caAddr = %s\n", psAp->caAddr);
	  	 			strcpy(s_apaddr, psAp->caAddr);
	  	 			printf("s_apaddr = %s\n", s_apaddr);
	  	 }
			 
			 switch(lSptype){
	  				case 0:strcpy(s_sptype, "δ֪");break;
	  				case 1:strcpy(s_sptype, "�ƶ�");break;
	  				case 2:strcpy(s_sptype, "��ͨ");break;	
	  				case 3:strcpy(s_sptype, "����");break;
	  				case 8:strcpy(s_sptype, "����");break;
	  				case 9:strcpy(s_sptype, "����");break;
	  				default:break;
	  	 }
			 
			 utPltPutLoopVarF(psDbHead,"contrycode",iNum,"%lu",lContrycode);
			 utPltPutLoopVar(psDbHead,"sptype",iNum,s_sptype);
			 utPltSetCvtHtml(1);
			 utPltPutLoopVar(psDbHead,"areaname",iNum,caAreaname);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVar(psDbHead,"usermac",iNum,caUsermac);
			 utPltPutLoopVar(psDbHead,"apmac",iNum,s_apaddr);
			 utPltPutLoopVar(psDbHead,"ip",iNum,caIp);
			 utPltSetCvtHtml(0);
			 utPltPutLoopVar(psDbHead,"logtime",iNum,caTime);
			 if(strlen(caExport)>0){
				 		ncUtlPrintCsv(fp,7,UT_TYPE_ULONG,lContrycode,
		 											UT_TYPE_STRING,caUsername,
											 		UT_TYPE_STRING,caUsermac,
											 		UT_TYPE_STRING,caIp,
	                        UT_TYPE_STRING,caTime,
	                        UT_TYPE_STRING,s_sptype,
	                        UT_TYPE_STRING,caAreaname);
			 }
			 
			 lContrycode = 0;
			 memset(caUsername, 0, sizeof(caUsername));
			 memset(caUsermac, 0, sizeof(caUsermac));
			 memset(caIp, 0, sizeof(caIp));
			 memset(caTime, 0, sizeof(caTime));
			 lSptype = 0;
			 memset(caAreaname, 0, sizeof(caAreaname));
			 memset(caApmac, 0, sizeof(caApmac));
			 memset(s_sptype, 0, sizeof(s_sptype));
			 memset(s_apaddr, 0, sizeof(s_apaddr));
			 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caUsername)-1,  	caUsername,
																	 UT_TYPE_STRING, sizeof(caUsermac)-1,  	  caUsermac,
																	 UT_TYPE_STRING, sizeof(caIp)-1,  	      caIp,
																	 UT_TYPE_STRING, sizeof(caTime)-1,  			caTime,
																	 UT_TYPE_ULONG,  sizeof(long),  	        &lSptype,
																	 UT_TYPE_ULONG,  sizeof(long),  	        &lContrycode,
																	 UT_TYPE_STRING, sizeof(caAreaname)-1,  	caAreaname,
																	 UT_TYPE_STRING, sizeof(caApmac)-1,       caApmac);
	 }
	 //utPltShowDb(psDbHead); 
	 pasDbCloseCursor(psCur);
	 
	 if(strlen(caExport)>0){
 	    fclose(fp);
      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
      remove(caFile);
      utPltFreeDb(psDbHead);
      return 0;
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_appreport/ncm_web_App_smsDetailReport.htm");
	 return 0;
}

// ÿСʱ���Ͷ��ű��� 
int ncm_web_App_smsStatisticsReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 int iNumber = 0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char sdate[32] = "";
	 char edate[32] = "";
	 char slogdate[64] = "";
	 char elogdate[64] = "";
	 char slogtime[64] = "";
	 char elogtime[64] = "";
	 char timeV[32] = "";
	 char localNum[32] = "";
	 char interNum[32] = "";
	 char groupcode[32] = "";
	 char areaid[32] = "";
	 char groupid[32] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long smsnum = 0;
	 char htime[24] = "";
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // ��ǰʱ��(Ĭ�Ͻ���ʱ��)
	 
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caSesid[1024] = "";
	 char caDel[16] = "";
	  // �ؼ���
	 char _keyword[33] = "";
	 
	 // ���ݱ�
	 unsigned long lCount1 = 0; // ���м�¼�ĸ���
	 long iReturn; // ����¼��ѯ
	 char start[17] = ""; // Ĭ�ϼ�¼��0��ʼ
	 char limit[17] = ""; // �޶�һҳ�еļ�¼��40��
	 unsigned long iStart = 0; // ת�����Ĭ�Ͽ�ʼatoi(start)
	 unsigned long iLimit = 0; // ת������޶���¼����atoi(limit)
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
   // ��ҳ
	 unsigned long lTotRec = 0; // ��¼���е����ݵĸ���
   
   // ����
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // �̼�
   char caStype[8] = "";
	 char caOptd[64] = "";
	 unsigned long lCid = 0;
   iReturn = dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	 iReturn = dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStype)-1,caStype);
	 iReturn = dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
	 printf("lCid = %d\n", lCid);
	 printf("*******caStype = %s \n",caStype);
	 printf("*******caOptd = %s \n",caOptd);
	 
	 // ������¼��Ϣ�ṹ��
	 int i = 0;
	 typedef struct sms_stat{
	 		int num;
	 		char timeV[18];
	 		long localNum;
	 		long interNum;
	 }sms_stat_s;
	 sms_stat_s stat_sms[25];
	 for (i = 0;i < 24;i++){ // ��ʼ��
		 	 stat_sms[i].num = i;
		   sprintf(stat_sms[i].timeV,"%02d:00:00-%02d:59:59",i,i);
		   stat_sms[i].localNum = 0;
		   stat_sms[i].interNum = 0;
		   //printf("num = %d timeV = %s localNum = %d interNum = %d \n",stat_sms[i].num,stat_sms[i].timeV,stat_sms[i].localNum,stat_sms[i].interNum);	
	 }
	 stat_sms[24].num = 24;
   sprintf(stat_sms[24].timeV,"�ܼ�");
   stat_sms[24].localNum = 0;
   stat_sms[24].interNum = 0;
	 char caShopids[1024];
	 strcpy(caShopids,getDsShopids());
	 printf("caShopids=%s\n",caShopids);
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 19,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
															    "slogdate",    UT_TYPE_STRING,  sizeof(slogdate)-1,       slogdate,
															    "elogdate",    UT_TYPE_STRING,  sizeof(elogdate)-1,       elogdate,
															    "slogtime",    UT_TYPE_STRING,  sizeof(slogtime)-1,       slogtime,
															    "elogtime",    UT_TYPE_STRING,  sizeof(elogtime)-1,       elogtime,
																	"timeV",       UT_TYPE_STRING,  sizeof(timeV)-1,          timeV,
																	"localNum",    UT_TYPE_STRING,  sizeof(localNum)-1,       localNum,
																	"interNum",    UT_TYPE_STRING,  sizeof(interNum)-1,       interNum,
																	"groupcode",   UT_TYPE_STRING,  sizeof(groupcode)-1,      groupcode,
																	"areaid",      UT_TYPE_STRING,  sizeof(areaid)-1,         areaid,
																	//"groupid",     UT_TYPE_STRING,  sizeof(groupid)-1,        groupid,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				 UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",    UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 
	 trim(_keyword);
	 
	 // ��ȡ�����ļ�������ʱ��
	 char cnfSdate[32] = "";
	 char cnfEdate[32] = "";
	 char cnfPath[128] = "";
	 char *pConfig;
	 pasConfList *psConfig;
	 
	 pConfig = (char *)pasGetConfigFileName();
	 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
   if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","��ȡ�����ļ�",(char *)ncLang("0427�����ļ�����"));
        return 0;
   }
   //����û�����ã�Ĭ��null��
   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig�����һ������ΪStatisticsStim��Ĭ��ֵ
 	 strcpy(cnfEdate,(char *)pasUtlLookConfig(psConfig,"StatisticsEtime",""));
 	 strcpy(cnfPath,(char *)pasUtlLookConfig(psConfig,"ReportPosition",""));
   printf("cnfSdate = %s\n", cnfSdate);
	 printf("cnfPath = %s\n", cnfPath);
   pasUtlFreeConfig(psConfig);
	 
	 if(strlen(cnfSdate)>0 && strlen(cnfEdate)>0){
	 		ncTimeToLong(cnfSdate, &lSdate);
			ncTimeToLong(cnfEdate, &lEdate);
			printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	 }else{
	 		// ת��ʱ��
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // Ĭ��ʱ���ǽ���0��--��ǰʱ��
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 char _sdate[32] = ""; // ��ʽת����Ŀ�ʼʱ��
	 char _edate[32] = ""; // ��ʽת����ĵ�ǰʱ��
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 // ����
	 if(strlen(caExport)>0){
	 		 strcpy(caFilename, "smsStatisticsReport.csv");
	 		 if(strlen(cnfPath)>0){
	 		 		strcpy(caPath, cnfPath);
	 		 }else{
	 		 		strcpy(caPath, FILEPATH);
	 		 }
	 		 sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",(char *)ncLang("0546���ݵ�������"),(char *)ncLang("0546���ݵ�������"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"���Ͷ���ͳ�Ʊ���\n");
			 fprintf(fp,"\t\tʱ��:%s--%s\n", _sdate, _edate);
			 ncUtlPrintCsv(fp,5,UT_TYPE_STRING,"ʱ��",
			 									  UT_TYPE_STRING,"���ڶ�������",
			                    UT_TYPE_STRING,"���ʶ�������");
	 }
	 
	 memset(sql,0,sizeof(sql)); // ���ڶ���ͳ��
	 sprintf(sql, "select count(*) as smsnum, hour(from_unixtime(logtime)) as htime from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);		                    
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (contrycode = '086' or contrycode = '86')");
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by htime");
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  4,                   &smsnum,
															 UT_TYPE_STRING, sizeof(htime)-1,  		htime);
	 
 	 while(0 == iret || 1405 == iret){
	  	 stat_sms[atol(htime)].localNum = smsnum;
	  	 stat_sms[24].localNum += smsnum;
	  	  
	  	 smsnum = 0;
	  	 memset(htime, 0, sizeof(htime));
	  	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  4,                   &smsnum,
															     UT_TYPE_STRING, sizeof(htime)-1,  		htime);
	 }
	 pasDbCloseCursor(psCur);
	 
	 memset(sql,0,sizeof(sql)); // ���ʶ���ͳ��
	 sprintf(sql, "select count(*) as smsnum, hour(from_unixtime(logtime)) as htime from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);		                    
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (contrycode != '086' and contrycode != '86')");
	 	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
	 
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by htime");
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 smsnum = 0;
	 memset(htime, 0, sizeof(htime));
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  4,                   &smsnum,
															 UT_TYPE_STRING, sizeof(htime)-1,  		htime);
	 
	 while(0 == iret || 1405 == iret){
  	   stat_sms[atol(htime)].interNum = smsnum;
  	   stat_sms[24].interNum += smsnum;
  	   
  	   smsnum = 0;
	  	 memset(htime, 0, sizeof(htime));
	  	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  4,                   &smsnum,
															     UT_TYPE_STRING, sizeof(htime)-1,  		htime);
   }
	 pasDbCloseCursor(psCur);
	 
	 for(i=0; i<25; i++){
	 		iNum++;
	    if(iNum > 1)
		  {
			   utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
		  }
	 		utPltPutLoopVar(psDbHead,"timeV",iNum,stat_sms[i].timeV);
	 		utPltPutLoopVarF(psDbHead,"localNum",iNum,"%lu",stat_sms[i].localNum);		
	 		utPltPutLoopVarF(psDbHead,"interNum",iNum,"%lu",stat_sms[i].interNum);
	 		if(strlen(caExport)>0){
					 		ncUtlPrintCsv(fp,3,UT_TYPE_STRING,stat_sms[i].timeV,
					 											UT_TYPE_ULONG,stat_sms[i].localNum,
														 		UT_TYPE_ULONG,stat_sms[i].interNum);
			}	
	 }
	 //utPltShowDb(psDbHead);
	  
	 if(strlen(caExport)>0){
 	    fclose(fp);
      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
      remove(caFile);
      utPltFreeDb(psDbHead);
      return 0;
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_appreport/ncm_web_App_smsStatisticsReport.htm");
	 return 0;
}

// �û�����ͳ�Ʊ���
int ncm_web_App_intSsidReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 int iNumber = 0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 char sdate[32] = "";
	 char edate[32] = "";
	 char ssid[32] = "";
	 char num[20] = "";
	 char groupcode[32] = "";
	 char areaid[32] = "";
	 char groupid[32] = "";
	 
	 unsigned long lCount2 = 0;
	 char caSsid[32] = "";
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // ��ǰʱ��(Ĭ�Ͻ���ʱ��)
	 
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	  // �ؼ���
	 char _keyword[33] = "";
	 char caSesid[1024] = "";
	 
	 // ���ݱ�
	 unsigned long lCount1 = 0; // ���м�¼�ĸ���
	 long iReturn; // ����¼��ѯ
	 char page[17] = "";
	 char start[17] = ""; // Ĭ�ϼ�¼��0��ʼ
	 char limit[17] = ""; // �޶�һҳ�еļ�¼��40��
	 unsigned long lPage = 0;
	 unsigned long lStart = 0; // ת�����Ĭ�Ͽ�ʼatoi(start)
	 unsigned long lLimit = 0; // ת������޶���¼����atoi(limit)
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
   // ��ҳ
	 unsigned long lTotRec = 0; // ��¼���е����ݵĸ���
   
   // ����
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // �̼�
   char caStype[8] = "";
	 char caOptd[64] = "";
	 unsigned long lCid = 0;
   iReturn = dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	 iReturn = dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStype)-1,caStype);
	 iReturn = dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
	 printf("lCid = %d\n", lCid);
	 printf("*******caStype = %s \n",caStype);
	 printf("*******caOptd = %s \n",caOptd);
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 18,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
															    "slogdate",    UT_TYPE_STRING,  sizeof(sdate)-1,          sdate,
															    "elogdate",    UT_TYPE_STRING,  sizeof(edate)-1,          edate,
															    "ssid",        UT_TYPE_STRING,  sizeof(ssid)-1,       		ssid,
																	"num",         UT_TYPE_STRING,  sizeof(num)-1,            num,
																	"groupcode",   UT_TYPE_STRING,  sizeof(groupcode)-1,      groupcode,
																	"areaid",      UT_TYPE_STRING,  sizeof(areaid)-1,         areaid,
																	"page",        UT_TYPE_STRING,  sizeof(page)-1,           page,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				 UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",    UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 
	 trim(_keyword);
	 
	 lLimit = atol(limit);
	 lPage = atol(page);
	 lStart = (lPage - 1) * lLimit;
	 if(lStart <= 0) lStart = 0;
	 
	 // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"starttime");
	 }
	 
	 // ��ȡ�����ļ�������ʱ��
	 char cnfSdate[32] = "";
	 char cnfEdate[32] = "";
	 char cnfPath[128] = "";
	 char *pConfig;
	 pasConfList *psConfig;
	 
	 pConfig = (char *)pasGetConfigFileName();
	 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
   if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","��ȡ�����ļ�",(char *)ncLang("0427�����ļ�����"));
        return 0;
   }
   //����û�����ã�Ĭ��null��
   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig�����һ������ΪStatisticsStim��Ĭ��ֵ
 	 strcpy(cnfEdate,(char *)pasUtlLookConfig(psConfig,"StatisticsEtime",""));
 	 strcpy(cnfPath,(char *)pasUtlLookConfig(psConfig,"ReportPosition",""));
   printf("cnfSdate = %s\n", cnfSdate);
	 printf("cnfEdate = %s\n", cnfEdate);
	 printf("cnfPath = %s\n", cnfPath);
   pasUtlFreeConfig(psConfig);
	 
	 if(strlen(cnfSdate)>0 && strlen(cnfEdate)>0){
	 		ncTimeToLong(cnfSdate, &lSdate);
			ncTimeToLong(cnfEdate, &lEdate);
			printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	 }else{
		  strcat(edate, " 23:59:59");
		  
	 		// ת��ʱ��
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // Ĭ��ʱ���ǽ���0��--��ǰʱ��
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 char _sdate[32] = ""; // ��ʽת����Ŀ�ʼʱ��
	 char _edate[32] = ""; // ��ʽת����ĵ�ǰʱ��
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 sprintf(sql, "select count(*) as lCount2, ssid from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate);		                    
	 if(!utStrIsSpaces(ssid)) // ����ssid��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ssid like '%%%s%%'", ssid);
	 }
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by ssid");
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by starttime Desc");
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),    &lCount2,
															 UT_TYPE_STRING, sizeof(caSsid)-1,caSsid);
						 
	 while(0 == iret || 1405 == iret){
  	   iNum++;
  	   
			 lCount2 = 0;
			 memset(caSsid, 0, sizeof(caSsid));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),    &lCount2,
																	 UT_TYPE_STRING, sizeof(caSsid)-1,caSsid);
   }
	 utPltPutVarF(psDbHead,"TotRec", "%lu", iNum); 
	 pasDbCloseCursor(psCur);
	 
	 // ����
	 if(strlen(caExport)>0){
	 		 strcpy(caFilename, "intSsidReport.csv");
	 		 if(strlen(cnfPath)>0){
	 		 		strcpy(caPath, cnfPath);
	 		 }else{
	 		 		strcpy(caPath, FILEPATH);
	 		 }
	 		 sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
	 		 		 printf("���ļ�ʧ�ܣ�\n");
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",(char *)ncLang("0546���ݵ�������"),(char *)ncLang("0546���ݵ�������"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"SSID Report\n");
			 fprintf(fp,"\t\tTime:%s--%s\n", sdate, edate);
			 ncUtlPrintCsv(fp,2,UT_TYPE_STRING,"SSID",
			                    UT_TYPE_STRING,"Number");
	 }
	 
	 sprintf(sql, "select count(*) as lCount2, ssid from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate);		                    
	 if(!utStrIsSpaces(ssid)) // ����ssid��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ssid like '%%%s%%'", ssid);
	 }
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by ssid");
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, lStart, lLimit);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),    &lCount2,
															 UT_TYPE_STRING, sizeof(caSsid)-1,caSsid);
	 iNum=0;		 
	 while(0 == iret || 1405 == iret){
  	   iNum++;
  	   if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
			 if(strlen(caSsid)==0){
			 		strcpy(caSsid, "����");	
			 }
			 
			 utPltSetCvtHtml(1);
			 utPltPutLoopVar(psDbHead,"ssid",iNum,caSsid);
			 utPltSetCvtHtml(0);
			 utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",lCount2);
			 if(strlen(caExport)>0){
			 		ncUtlPrintCsv(fp,3,UT_TYPE_STRING,caSsid,
												 		 UT_TYPE_ULONG,lCount2);
			 }
			 
			 lCount2 = 0;
			 memset(caSsid, 0, sizeof(caSsid));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),    &lCount2,
																	 UT_TYPE_STRING, sizeof(caSsid)-1,caSsid);
   }
	 //utPltShowDb(psDbHead); 
	 pasDbCloseCursor(psCur);
	 
	 if(strlen(caExport)>0){
 	    fclose(fp);
      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
      remove(caFile);
      utPltFreeDb(psDbHead);
      return 0;
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_appreport/ncm_web_App_intSsidReport.htm");
	 return 0;
}

// �û�����ͳ�Ʊ���
int ncm_web_App_intTrafReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char sdate[32] = "";
	 char edate[32] = "";
	 char slogdate[64] = "";
	 char elogdate[64] = "";
	 char slogtime[64] = "";
	 char elogtime[64] = "";
	 char username[32] = "";
	 char ssid[32] = "";
	 char traffic[64] = "";
	 char groupcode[32] = "";
	 char areaid[32] = "";
	 char groupid[32] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 char caUsername[32] = "";
   char caSsid[32] = "";
   unsigned long lUbytes = 0;
	 unsigned long lDbytes = 0;
	 double dBytes = 0.0;
	 
	 // ��������
	 unsigned long lubytes = 0;
	 unsigned long ldbytes = 0;
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // ��ǰʱ��(Ĭ�Ͻ���ʱ��)
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	  // �ؼ���
	 char _keyword[33] = "";
	 char caSesid[1024] = "";
	 
	 // ���ݱ�
	 unsigned long lCount1 = 0; // ���м�¼�ĸ���
	 long iReturn; // ����¼��ѯ
	 char page[17] = "";
	 char start[17] = ""; // Ĭ�ϼ�¼��0��ʼ
	 char limit[17] = ""; // �޶�һҳ�еļ�¼��40��
	 unsigned long lPage = 0;
	 unsigned long lStart = 0; // ת�����Ĭ�Ͽ�ʼatoi(start)
	 unsigned long lLimit = 0; // ת������޶���¼����atoi(limit)
	 
	 // ����
	 char sort[32] = "";
   char dir[9] = "";
   
   // ��ҳ
	 unsigned long lTotRec = 0; // ��¼���е����ݵĸ���
   
   // ����
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 char code[64]="";
	 char desc_code[64]="";
	 
	 // �̼�
   char caStype[8] = "";
	 char caOptd[64] = "";
	 unsigned long lCid = 0;
   iReturn = dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	 iReturn = dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStype)-1,caStype);
	 iReturn = dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
	 printf("lCid = %d\n", lCid);
	 printf("*******caStype = %s \n",caStype);
	 printf("*******caOptd = %s \n",caOptd);
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 20,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
															    "slogdate",    UT_TYPE_STRING,  sizeof(slogdate)-1,       slogdate,
															    "elogdate",    UT_TYPE_STRING,  sizeof(elogdate)-1,       elogdate,
															    "slogtime",    UT_TYPE_STRING,  sizeof(slogtime)-1,       slogtime,
															    "elogtime",    UT_TYPE_STRING,  sizeof(elogtime)-1,       elogtime,
																	"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
																	"ssid",        UT_TYPE_STRING,  sizeof(ssid)-1,           ssid,
																	"traffic",     UT_TYPE_STRING,  sizeof(traffic)-1,        traffic,
																	"groupcode",   UT_TYPE_STRING,  sizeof(groupcode)-1,      groupcode,
																	"areaid",      UT_TYPE_STRING,  sizeof(areaid)-1,         areaid,
																	//"groupid",     UT_TYPE_STRING,  sizeof(groupid)-1,        groupid,
																	"page",        UT_TYPE_STRING,  sizeof(page)-1,           page,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				 UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",    UT_TYPE_STRING,  sizeof(caExport)-1,       caExport,
																	"code",        UT_TYPE_STRING,  sizeof(code)-1,       code);
	 
	 trim(_keyword);
	 iReturn = utf8_to_gb2312(code, sizeof(code), desc_code, sizeof(desc_code));
	 lLimit = atol(limit);
	 lPage = atol(page);
	 lStart = (lPage - 1) * lLimit;
	 if(lStart <= 0) lStart = 0;
	 
	 // ����
   if(strlen(sort)==0){
	 	  //sprintf(sort,"%s, %s", "ubytes Desc", "dbytes Desc");
	 }
	 
	 // ��ȡ�����ļ�������ʱ��
	 char cnfSdate[32] = "";
	 char cnfEdate[32] = "";
	 char cnfPath[128] = "";
	 char *pConfig;
	 pasConfList *psConfig;
	 
	 pConfig = (char *)pasGetConfigFileName();
	 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
   if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","��ȡ�����ļ�",(char *)ncLang("0427�����ļ�����"));
        return 0;
   }
   //����û�����ã�Ĭ��null��
   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig�����һ������ΪStatisticsStim��Ĭ��ֵ
 	 strcpy(cnfEdate,(char *)pasUtlLookConfig(psConfig,"StatisticsEtime",""));
 	 strcpy(cnfPath,(char *)pasUtlLookConfig(psConfig,"ReportPosition",""));
   printf("cnfSdate = %s\n", cnfSdate);
	 printf("cnfEdate = %s\n", cnfEdate);
	 printf("cnfPath = %s\n", cnfPath);
   pasUtlFreeConfig(psConfig);
	 
	 if(strlen(cnfSdate)>0 && strlen(cnfEdate)>0){
	 		ncTimeToLong(cnfSdate, &lSdate);
			ncTimeToLong(cnfEdate, &lEdate);
			printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	 }else{
	 		// ת��ʱ��
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // Ĭ��ʱ���ǽ���0��--��ǰʱ��
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 // �鿴ʱ���Ƿ���ȷ
	 char _sdate[32] = ""; // ��ʽת����Ŀ�ʼʱ��
	 char _edate[32] = ""; // ��ʽת����ĵ�ǰʱ��
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 switch(atol(ssid)){
	 		case 0:strcpy(ssid, "");break;
	 		case 1:strcpy(ssid, "ICPIC");break;
	 		case 2:strcpy(ssid, "CPICVIP");break;
	 		default: break;
	 }

	
	 sprintf(sql, "select sum(a.ubytes) as u, sum(a.dbytes) as d from ncsrvuserlog as a,ncsrvgroup as b  where starttime>%lu and starttime<%lu ", lSdate, lEdate);
	 if(!utStrIsSpaces(code))
	 	{		
	 			if(strcmp(desc_code,"ALL")==0)
	 			{
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and a.groupid=b.groupid");
	 			}
	 			else
	 			{		
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and a.groupid=b.groupid and b.dispname='%s'", desc_code);
	 			}
	 	}	                    
//	 if(!utStrIsSpaces(username)) // �����û���¼����ѯ
//	 {
//			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and a.username like '%%%s%%'", username);
//	 }
//	 if(!utStrIsSpaces(ssid)) // ����ssid��ѯ
//	 {
//			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and a.ssid = '%s'", ssid);
//	 }
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and a.username != 'Guest' group by username");
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by u Desc limit %lu, %lu", lStart, lLimit);
				
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),  &lubytes,
															 UT_TYPE_ULONG,  sizeof(long),  &ldbytes);
	 
	 while(0 == iret || 1405 == iret){  
  	   iNum++;
  	   
		   lubytes = 0;
			 ldbytes = 0;
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long), &lubytes,
																	 UT_TYPE_ULONG,  sizeof(long), &ldbytes);
	 }
	 utPltPutVarF(psDbHead,"TotRec", "%lu", iNum);
	 pasDbCloseCursor(psCur);
	 
	 // ����
	 if(strlen(caExport)>0){
	 		 strcpy(caFilename, "intUserTrafficReport.csv");
	 		 if(strlen(cnfPath)>0){
	 		 		strcpy(caPath, cnfPath);
	 		 }else{
	 		 		strcpy(caPath, FILEPATH);
	 		 }
	 		 sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",(char *)ncLang("0546���ݵ�������"),(char *)ncLang("0546���ݵ�������"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"�û�����ͳ�Ʊ���\n");
			 fprintf(fp,"\t\tʱ��:%s--%s\n", sdate, edate);
			 ncUtlPrintCsv(fp,3,UT_TYPE_STRING,"�û��˺�",
			                    UT_TYPE_STRING,"SSID",
			                    UT_TYPE_STRING,"��������(mb)");
	 }
	 
	 sprintf(sql, "select username, ssid, sum(ubytes) as u, sum(dbytes) as d from ncsrvuserlog as a,ncsrvgroup as b where starttime>%lu and endtime<%lu", lSdate, lEdate);	
	 if(!utStrIsSpaces(code))
	 	{		
	 			if(strcmp(desc_code,"ALL")==0)
	 			{
	 			}
	 			else
	 			{		
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and a.groupid=b.groupid and b.dispname='%s'", desc_code);
	 			}
	 	}	                                    
//	 if(!utStrIsSpaces(username)) // �����û���¼����ѯ
//	 {
//			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and a.username like '%%%s%%'", username);
//	 }
//	 if(!utStrIsSpaces(ssid)) // ����ssid��ѯ
//	 {
//			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and a.ssid = '%s'", ssid);
//	 }
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and a.username != 'Guest' group by username");
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by u Desc, d Desc limit %lu, %lu", lStart, lLimit);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
												 		   UT_TYPE_STRING, sizeof(caSsid)-1,      caSsid,
															 UT_TYPE_ULONG,  sizeof(long),          &lUbytes,
															 UT_TYPE_ULONG,  sizeof(long),          &lDbytes);
	 iNum=0;
	 while(0 == iret || 1405 == iret){  
  	   iNum++;
  	   if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
			 dBytes = (double)(lUbytes+lDbytes)/(1024*1024);
			 
			 utPltSetCvtHtml(1);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVar(psDbHead,"ssid",iNum,caSsid);
			 utPltSetCvtHtml(0);
			 utPltPutLoopVarF(psDbHead,"traffic",iNum,"%.2f",dBytes);
			 if(strlen(caExport)>0){
				 		ncUtlPrintCsv(fp,3,UT_TYPE_STRING,caUsername,
													 		 UT_TYPE_STRING,caSsid,
		                           UT_TYPE_FLOAT,dBytes);
			 }
			 
			 memset(caUsername, 0, sizeof(caUsername));
			 memset(caSsid, 0, sizeof(caSsid));
		   lUbytes = 0;
			 lDbytes = 0;
			 dBytes = 0.0;
			 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
														 		   UT_TYPE_STRING, sizeof(caSsid)-1,      caSsid,
																	 UT_TYPE_ULONG,  sizeof(long),          &lUbytes,
																	 UT_TYPE_ULONG,  sizeof(long),          &lDbytes);
	 }
	 //utPltShowDb(psDbHead); 
	 pasDbCloseCursor(psCur);
	 
	 if(strlen(caExport)>0){
 	    fclose(fp);
      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
      remove(caFile);
      utPltFreeDb(psDbHead);
      return 0;
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_appreport/ncm_web_App_intTrafficReport.htm");
	 return 0;	
}

// ĳһʱ��AP��MAC����(ÿ��MAC�ڲ�ͬ��AP��ֻͳ��һ��)
int ncm_web_App_apMacCount_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{ 
		printf("11111111111111111111\n"); 
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char sdate[32] = "";
	 char slogdate[64] = "";
	 char slogtime[64] = "";
	 char pname[32] = "";
	 char apmac[18] = "";
	 char groupid[32] = "";
	 char groupcode[32] = "";
	 char mark[64] = "";
	 char stime[32] = "";
	 char maccount[32] = "";
	 char areaid[32] = "";
	 
	 
	 // ������ݿ�ȡ�������ݵı���
	 char caPname[32] = "";
   char caApmac[18] = "";
   unsigned long lGroupid = 0;
   char caGroupname[32] = "";
   char caGroupcode[32] = "";
   char caMark[64] = "";
	 unsigned long lStime = 0;
	 unsigned long lMaccount = 0;
	 char s_stime[32] = "";
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // ��ǰʱ��(Ĭ�Ͻ���ʱ��)
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	  // �ؼ���
	 char _keyword[33] = "";
	 char caSesid[1024] = "";
	 
	 // ���ݱ�
	 unsigned long lCount1 = 0; // ���м�¼�ĸ���
	 long iReturn; // ����¼��ѯ
	 char start[17] = ""; // Ĭ�ϼ�¼��0��ʼ
	 char limit[17] = ""; // �޶�һҳ�еļ�¼��40��
	 unsigned long iStart = 0; // ת�����Ĭ�Ͽ�ʼatoi(start)
	 unsigned long iLimit = 0; // ת������޶���¼����atoi(limit)
	 
	 // ����
	 char sort[32] = "";
   char dir[9] = "";
   
   // ��ҳ
	 unsigned long lTotRec = 0; // ��¼���е����ݵĸ���
   
   // ����
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // �̼�
   char caStype[8] = "";
	 char caOptd[64] = "";
	 unsigned long lCid = 0;
   iReturn = dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	 iReturn = dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStype)-1,caStype);
	 iReturn = dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
	 printf("lCid = %d\n", lCid);
	 printf("*******caStype = %s \n",caStype);
	 printf("*******caOptd = %s \n",caOptd);
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 21,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
															    "slogdate",    UT_TYPE_STRING,  sizeof(slogdate)-1,       slogdate,
															    "slogtime",    UT_TYPE_STRING,  sizeof(slogtime)-1,       slogtime,
															    "pname",       UT_TYPE_STRING,  sizeof(pname)-1,          pname,
																	"apmac",       UT_TYPE_STRING,  sizeof(apmac)-1,          apmac,
																	"groupid",     UT_TYPE_STRING,  sizeof(groupid)-1,        groupid,
																	"groupcode",   UT_TYPE_STRING,  sizeof(groupcode)-1,      groupcode,
																	"mark",        UT_TYPE_STRING,  sizeof(mark)-1,           mark,
																	"stime",       UT_TYPE_STRING,  sizeof(stime)-1,          stime,
																	"maccount",    UT_TYPE_STRING,  sizeof(maccount)-1,       maccount,
																	"areaid",      UT_TYPE_STRING,  sizeof(areaid)-1,         areaid,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				 UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",    UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 
	 trim(_keyword);
	 
	 // ����
   if(strlen(sort)==0){
	 	  //sprintf(sort,"%s, %s", "ubytes Desc", "dbytes Desc");
	 }
	 
	 // ת��ʱ��
	 sprintf(sdate, "%s %s", slogdate, slogtime);
	 ncTimeToLong(sdate, &lSdate);
	 
	 // �鿴ʱ���Ƿ���ȷ
	 char _sdate[32] = ""; // ��ʽת����Ŀ�ʼʱ��
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 printf("_sdate = %s\n", _sdate);
	 
	 sprintf(sql, "select count(*) from (select count(distinct a.mac),a.portalname,a.apname,address,max(a.starttime) from ncsrvuserlog a,ncsrvgroupap b where a.starttime<%lu and a.apname=b.gwid group by a.portalname,a.apname,address) as aa", lSdate);
	 
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	  printf("sql = %s\n", sql);
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 // ����
	 if(strlen(caExport)>0){
	 		 strcpy(caFilename, "apMacCountReport.csv");
	 		 strcpy(caPath, FILEPATH);
	 		 sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",(char *)ncLang("0546���ݵ�������"),(char *)ncLang("0546���ݵ�������"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"MAC��������\n");
			 fprintf(fp,"\t\t��ֹʱ��:%s\n", sdate);
			 ncUtlPrintCsv(fp,5,UT_TYPE_STRING,"Portal",
			                    UT_TYPE_STRING,"AP Mac��ַ",
			                    UT_TYPE_STRING,"APλ��",
			                    UT_TYPE_STRING,"Mac����",
			                    UT_TYPE_STRING,"��ֹʱ��");
	 }
	 
	 sprintf(sql, "select count(distinct a.mac),a.portalname,a.apname,address,max(a.starttime) from ncsrvuserlog a,ncsrvgroupap b where a.starttime<%lu and a.apname=b.gwid group by a.portalname,a.apname,address", lSdate);	                    
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by address limit %lu, %lu", iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lMaccount,
	 														 UT_TYPE_STRING, sizeof(caPname)-1,     caPname,
	 														 UT_TYPE_STRING, sizeof(caApmac)-1,     caApmac,
	 														 UT_TYPE_STRING, sizeof(caGroupcode)-1, caGroupcode,
															 UT_TYPE_ULONG,  sizeof(long),          &lStime
															 );
	 
	 while(0 == iret || 1405 == iret){  
  	   iNum++;
  	   if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
			 if(lStime>0){
		   		strcpy(s_stime, utTimFormat("%Y/%m/%d %H:%M:%S", lStime));
		   }else{
		   		strcpy(s_stime, "");	
		   }
			 
			 if(lGroupid>0){
	  	 		strcpy(caGroupname, (char *)ncmGetDispNameByGid(psShmHead,lGroupid));
	  	 }
			 
			 utPltSetCvtHtml(1);
			 utPltPutLoopVar(psDbHead,"pname",iNum,caPname);
			 utPltPutLoopVar(psDbHead,"apmac",iNum,caApmac);
			 utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupcode);
//			 utPltPutLoopVar(psDbHead,"groupcode",iNum,caGroupcode);
//			 utPltPutLoopVar(psDbHead,"mark",iNum,caMark);
			 utPltSetCvtHtml(0);
			 utPltPutLoopVar(psDbHead,"stime",iNum,s_stime);
			 utPltPutLoopVarF(psDbHead,"maccount",iNum,"%lu",lMaccount);
			 if(strlen(caExport)>0){
				 		ncUtlPrintCsv(fp,5,UT_TYPE_STRING,caPname,
													 		 UT_TYPE_STRING,caApmac,
		                           UT_TYPE_STRING,caGroupcode,
		                           UT_TYPE_ULONG,lMaccount,
		                           UT_TYPE_STRING,s_stime);
			 }
			 
			 memset(caPname, 0, sizeof(caPname));
			 memset(caApmac, 0, sizeof(caApmac));
		   lGroupid = 0;
		   memset(caGroupcode, 0, sizeof(caGroupcode));
		   memset(caMark, 0, sizeof(caMark));
			 lStime = 0;
			 lMaccount = 0;
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lMaccount,
	 														 UT_TYPE_STRING, sizeof(caPname)-1,     caPname,
	 														 UT_TYPE_STRING, sizeof(caApmac)-1,     caApmac,
	 														 UT_TYPE_STRING, sizeof(caGroupcode)-1, caGroupcode,
															 UT_TYPE_ULONG,  sizeof(long),          &lStime
															 );
	 }
	 //utPltShowDb(psDbHead); 
	 pasDbCloseCursor(psCur);
	 
	 if(strlen(caExport)>0){
 	    fclose(fp);
      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
      remove(caFile);
      utPltFreeDb(psDbHead);
      return 0;
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_appreport/ncm_web_App_apMacCount.htm");
	 return 0;	
}

// һ����һ�˵����ô���
int ncm_web_App_userCount_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char sdate[32] = "";
	 char edate[32] = "";
	 char slogdate[64] = "";
	 char slogtime[64] = "";
	 char elogdate[64] = "";
	 char elogtime[64] = "";
	 char username[32] = "";
	 char usercount[32] = "";
	 char groupid[32] = "";
	 char starttime[32] = "";
	 char endtime[32] = "";
	 char conntime[32] = "";
	 char groupcode[32] = "";
	 char areaid[32] = "";
	 
	 
	 // ������ݿ�ȡ�������ݵı���
	 char caUsername[32] = "";
   unsigned long lGroupid = 0;
	 unsigned long lConntime = 0;
	 unsigned long lUsercount = 0;
	 char caYear[8] = "";
	 char caMonth[8] = "";
	 char caDay[8] = "";
	 char s_conntime[32] = "";
	 
	 // ��������
	 char causername[32] = "";
	 unsigned long lCount = 0;
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // ��ǰʱ��(Ĭ�Ͻ���ʱ��)
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	  // �ؼ���
	 char _keyword[33] = "";
	 char caSesid[1024] = "";
	 
	 // ���ݱ�
	 unsigned long lCount1 = 0; // ���м�¼�ĸ���
	 long iReturn; // ����¼��ѯ
	 char caPage[17] = "";  // �ڼ�ҳ��
	 char start[17] = ""; // Ĭ�ϼ�¼��0��ʼ
	 char limit[17] = ""; // �޶�һҳ�еļ�¼��40��
	 unsigned long lCurPg = 0; // ��ǰҳ��
	 unsigned long lStart = 0; // ת�����Ĭ�Ͽ�ʼatol(start)
	 unsigned long lLimit = 0; // ת������޶���¼����atol(limit)
	 
	 // ����
	 char sort[32] = "";
   char dir[9] = "";
   
   // ��ҳ
	 unsigned long lTotRec = 0; // ��¼���е����ݵĸ���
   
   // ����
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // �̼�
   char caStype[8] = "";
	 char caOptd[64] = "";
	 unsigned long lCid = 0;
   iReturn = dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	 iReturn = dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStype)-1,caStype);
	 iReturn = dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
	 printf("lCid = %d\n", lCid);
	 printf("*******caStype = %s \n",caStype);
	 printf("*******caOptd = %s \n",caOptd);
	 
	 // ������¼��Ϣ�ṹ��
	 int i = 0;
	 typedef struct count_stat{
	 		char username[32];
	 		char groupname[32];
	 		unsigned long usercount;
	 		unsigned long conntime;
	 }count_stat_s;
	 count_stat_s *stat_count;
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 21,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
															    "slogdate",    UT_TYPE_STRING,  sizeof(slogdate)-1,       slogdate,
															    "slogtime",    UT_TYPE_STRING,  sizeof(slogtime)-1,       slogtime,
															    "elogdate",    UT_TYPE_STRING,  sizeof(elogdate)-1,       elogdate,
															    "elogtime",    UT_TYPE_STRING,  sizeof(elogtime)-1,       elogtime,
															    "username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
															    "usercount",   UT_TYPE_STRING,  sizeof(usercount)-1,      usercount,
																	"groupid",     UT_TYPE_STRING,  sizeof(groupid)-1,        groupid,
																	"groupcode",   UT_TYPE_STRING,  sizeof(groupcode)-1,      groupcode,
																	"areaid",      UT_TYPE_STRING,  sizeof(areaid)-1,         areaid,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"page",        UT_TYPE_STRING,  sizeof(caPage)-1,         caPage,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				 UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",    UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 
	 trim(_keyword);
	 
	 lLimit = atol(limit);
	 lCurPg = atol(caPage);
	 lStart = (lCurPg - 1)*lLimit;
	 if(lStart <= 0) lStart=0;
	 
	 // ����
   if(strlen(sort)==0){
	 	  //sprintf(sort,"%s, %s", "ubytes Desc", "dbytes Desc");
	 }
	 
	 // ת��ʱ��
	 sprintf(sdate, "%s %s", slogdate, slogtime);
	 sprintf(edate, "%s %s", elogdate, elogtime);

	 if(strlen(sdate) > 1 && strlen(edate) > 1){
			 ncTimeToLong(sdate, &lSdate);
			 ncTimeToLong(edate, &lEdate);
			 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	 }else{
		   ncDaysToLong(lEdate, 1, &lSdate); // Ĭ��ʱ���ǽ���0��--��ǰʱ��
		   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	 }
	 
	 // �鿴ʱ���Ƿ���ȷ
	 char _sdate[32] = ""; // ��ʽת����Ŀ�ʼʱ��
	 char _edate[32] = ""; // ��ʽת����Ŀ�ʼʱ��
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 sprintf(sql, "select username from ncsrvuserlog where starttime>%lu and endtime<%lu",lSdate, lEdate);                    
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by username");
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iNum=0;
	 memset(causername, 0, sizeof(causername));
	 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(causername)-1, causername);
	 while(0 == iret || 1405 == iret){  
  	   iNum++;
  	   
			 memset(causername, 0, sizeof(causername));
			 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(causername)-1, causername);
	 }
	 lCount1 = iNum;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lCount1);
	 pasDbCloseCursor(psCur);
	 
	 stat_count = (count_stat_s *)malloc(sizeof(count_stat_s)*lCount1+1);
   for(i=0; i<lCount1; i++){
    		strcpy(stat_count[i].username,"");
   			strcpy(stat_count[i].groupname,"");
		    stat_count[i].usercount = 0;
	 	    stat_count[i].conntime = 0;
   }
	 
	 sprintf(sql, "select username from ncsrvuserlog where starttime>%lu and endtime<%lu",lSdate, lEdate);                    
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by username");
   snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by username desc limit %d, %d", lStart, lLimit);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iNum=0;
	 memset(causername, 0, sizeof(causername));
	 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(causername)-1, causername);
	 while(0 == iret || 1405 == iret){  
  	   strcpy(stat_count[iNum].username, causername);
  	   iNum++;
  	   
			 memset(causername, 0, sizeof(causername));
			 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(causername)-1, causername);
	 }
	 pasDbCloseCursor(psCur);
	 
	 // ����
	 if(strlen(caExport)>0){
	 		 strcpy(caFilename, "userCountReport.csv");
	 		 strcpy(caPath, FILEPATH);
	 		 sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",(char *)ncLang("0546���ݵ�������"),(char *)ncLang("0546���ݵ�������"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"���ʴ�������\n");
			 fprintf(fp,"\t\t��ʼʱ��--����ʱ��:%s--%s\n", sdate,edate);
			 ncUtlPrintCsv(fp,4,UT_TYPE_STRING,"�û�����",
			                    UT_TYPE_STRING,"��λ����",
			                    UT_TYPE_STRING,"���ʴ���",
			                    UT_TYPE_STRING,"ͣ��ʱ��");
	 }
	 
	 sprintf(sql, "select username, groupid, count(username), sum(conntime), year(from_unixtime(starttime)) as y, month(from_unixtime(starttime)) as m, dayofmonth(from_unixtime(starttime)) as d from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate); 
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by username, y, m, d");
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by username, y desc, m desc, d desc");
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
															 UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
															 UT_TYPE_ULONG,  sizeof(long),          &lUsercount,
															 UT_TYPE_ULONG,  sizeof(long),          &lConntime,
															 UT_TYPE_STRING, sizeof(caYear)-1,      caYear,
															 UT_TYPE_STRING, sizeof(caMonth)-1,     caMonth,
															 UT_TYPE_STRING, sizeof(caDay)-1,       caDay);
	 
	 while(0 == iret || 1405 == iret){  
	  	 for(i=0; i<lCount1; i++){
	  	 		if(strcmp(stat_count[i].username, caUsername) == 0){
	  	 				stat_count[i].usercount++;
	  	 				stat_count[i].conntime += lConntime;
	  	 				if(lGroupid>0){
					  	 		strcpy(stat_count[i].groupname, (char *)ncmGetDispNameByGid(psShmHead,lGroupid));
					  	}	
	  	 		}	
	  	 }
			 
			 memset(caUsername, 0, sizeof(caUsername));
			 lGroupid = 0;
			 lUsercount = 0;
			 lConntime = 0;
			 memset(caYear, 0, sizeof(caYear));
			 memset(caMonth, 0, sizeof(caMonth));
			 memset(caDay, 0, sizeof(caDay));
			 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
																	 UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
																	 UT_TYPE_ULONG,  sizeof(long),          &lUsercount,
																	 UT_TYPE_ULONG,  sizeof(long),          &lConntime,
																	 UT_TYPE_STRING, sizeof(caYear)-1,      caYear,
																	 UT_TYPE_STRING, sizeof(caMonth)-1,     caMonth,
																	 UT_TYPE_STRING, sizeof(caDay)-1,       caDay);
	 }
	 //utPltShowDb(psDbHead); 
	 pasDbCloseCursor(psCur);
	 
	 iNum = 0;
	 for(i=0; i<lCount1; i++){
			 if(strlen(stat_count[i].username) > 0 && stat_count[i].usercount >= atol(usercount)){
					 iNum++;
			 	   if(iNum > 1)
					 {
						 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
					 }
					 
					 ncNumToTimeE(stat_count[i].conntime, s_conntime);
					 
					 utPltSetCvtHtml(1);
			 		 utPltPutLoopVar(psDbHead,"username",iNum,stat_count[i].username);
					 utPltPutLoopVar(psDbHead,"groupname",iNum,stat_count[i].groupname);
					 utPltSetCvtHtml(0);
					 utPltPutLoopVar(psDbHead,"conntime",iNum,s_conntime);
					 utPltPutLoopVarF(psDbHead,"usercount",iNum,"%lu",stat_count[i].usercount);	
					 if(strlen(caExport)>0){
						 		ncUtlPrintCsv(fp,4,UT_TYPE_STRING,stat_count[i].username,
															 		 UT_TYPE_STRING,stat_count[i].groupname,
				                           UT_TYPE_ULONG, stat_count[i].usercount,
				                           UT_TYPE_STRING,s_conntime);
					 }
					 
					 memset(s_conntime, 0, sizeof(s_conntime));
			 }
	 }
	 
	 if(strlen(caExport)>0){
 	    fclose(fp);
      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
      remove(caFile);
      utPltFreeDb(psDbHead);
      return 0;
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_appreport/ncm_web_App_userCount.htm");
	 return 0;	
}


