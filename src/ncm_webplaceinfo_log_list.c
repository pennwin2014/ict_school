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
char *ncmWebSelect(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char *sql, long cid, char *stype, char *opt, char *groupcode, char *areaid, char *groupid)
{
	 if(cid>0)  // ��λ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupid = %lu", cid);
	 }
	 //if(!utStrIsSpaces(stype))  // �̼Ҳ�ѯ
	 //{
			//snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and stype = '%s'", stype);
	 //}
	 if(!utStrIsSpaces(opt))  // �̼Ҳ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupcode like '%s%c'", opt, '%');
	 }
   if(!utStrIsSpaces(groupcode))  // �̼�Ȩ�޲�ѯ
	 {
	 	  char caOpt[64]="";
	 	  sprintf(caOpt,"%s%s",ncmWebAutoDefGet16(psShmHead,iFd,psMsgHead,stype),groupcode);
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupcode like '%s%c'", caOpt, '%');
	 }
	 if(!utStrIsSpaces(areaid))  // �����ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupcode like '%s%c'", areaid, '%');
	 }
	 if(!utStrIsSpaces(groupid))  // ��λ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupid = %lu", atol(groupid));
	 }
	 
	 return sql;	
}

static int ncmWebReplace00(char *pHead,char *pVar,short nType,long lLen,char *pValue);
int ncmWebReplace(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);

// �����˶���ע��
int ncm_web_Place_smslog_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 int i = 0;
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 char *tmpgbk = NULL;
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ��ʱ���ѯ����
	 char time_flag[2] = ""; // ѡ���ʱ���־��
	 int  iTime_flag = 0; // ���ת�����ʱ��atoi(time_flag)
   int  days = 0; // ���ת�����ʱ���־
	 unsigned long lStartTime = 0;  // ��ʼʱ�䣺�Ǹ��ݵ�ǰʱ�� - ѡ��õ�������
	 unsigned long lTime = time(0); // ��ǰʱ��
	 char sdate[32] = ""; // ����Ŀ�ʼʱ��
   char edate[32] = ""; // ����Ľ���ʱ��
   char _sdate[32] = ""; // ��ʽת����Ŀ�ʼʱ��
	 char _edate[32] = ""; // ��ʽת����ĵ�ǰʱ��
	 
	 // ������Ϣ����
	 char sid[20] = "";
	 char servicecode[16] = "";
	 char userid[16] = "";
	 char username[32] = "";
	 char usermac[18] = "";
	 char ip[16] = "";
	 char logtime[32] = "";
	 char smsname[32] = "";
	 char sptype[16] = "";
	 char areacode[16] = "";
	 char areaname[32] = "";
	 char groupcode[32] = "";
	 char contrycode[16] = "";
	 char status[16] = "";
	 char apmac[64] = "";
	 char areaid[32] ="";
	 char groupid[32] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lSid = 0;
	 char 					caServicecode[16] = "";
	 unsigned long	lGroupid = 0;
	 char 					caGroupname[32] = "";
	 unsigned long	lUserid = 0;
	 char 					caUsername[32] = "";
	 char 					caUsermac[18] = "";
	 char 					caIp[16] = "";
	 unsigned long  lLogtime = 0;
	 char 					caSmsname[32] = "";
	 unsigned long  lSptype = 0;
	 unsigned long  lAreacode = 0;
	 char 					caAreaname[32] = "";
	 unsigned long  lContrycode = 0;
	 unsigned long  lStatus = 0;
	 char           caApmac[64] = "";
	 char           s_logtime[32] = "";
	 char           s_status[32] = "";
	 char           s_sptype[32] = "";
	 ncPortalAp     *psAp = NULL;
	 char           s_apaddr[255] = ""; 
	 
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 
	 // ��ҳ
	 unsigned long lTotRec = 0; // ��¼���е����ݵĸ���
	 
	 // ����
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
   // �̼�
   char caStype[8] = "";
	 char caOptd[64] = "";
	 unsigned long lCid = 0;
	 char caShopids[1024];
   iReturn = dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	 iReturn = dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStype)-1,caStype);
	 iReturn = dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
	 printf("lCid = %d\n", lCid);
	 printf("*******caStype = %s \n",caStype);
	 printf("*******caOptd = %s \n",caOptd);
	  strcpy(caShopids,getDsShopids());
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 29,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
															    "time_flag",   UT_TYPE_STRING,  sizeof(time_flag)-1,      time_flag,
															    "sdate",       UT_TYPE_STRING,  sizeof(sdate)-1,          sdate,
															    "edate",       UT_TYPE_STRING,  sizeof(edate)-1,          edate,
																	"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
																	"servicecode", UT_TYPE_STRING,  sizeof(servicecode)-1,    servicecode,
																	"userid",      UT_TYPE_STRING,  sizeof(userid)-1,         userid,
																	"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
																	"usermac",     UT_TYPE_STRING,  sizeof(usermac)-1,        usermac,
																	"ip",          UT_TYPE_STRING,  sizeof(ip)-1,             ip,
																	"logtime",     UT_TYPE_STRING,  sizeof(logtime)-1,        logtime,
																	"smsname",     UT_TYPE_STRING,  sizeof(smsname)-1,        smsname,
																	"sptype",      UT_TYPE_STRING,  sizeof(sptype)-1,         sptype,
																	"areacode",    UT_TYPE_STRING,  sizeof(areacode)-1,       areacode,
																	"areaname",    UT_TYPE_STRING,  sizeof(areaname)-1,       areaname,
																	"contrycode",  UT_TYPE_STRING,  sizeof(contrycode)-1,     contrycode,
																	"groupcode",   UT_TYPE_STRING,  sizeof(groupcode)-1,      groupcode,
																	"status",      UT_TYPE_STRING,  sizeof(status)-1,         status,
																	"apmac",       UT_TYPE_STRING,  sizeof(apmac)-1,          apmac,
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
	 
	 // ����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,32);
			strcpy(username,caTemp);
   }
   if(strlen(usermac)>0){
			pasCvtGBK(2,usermac,caTemp,18);
			strcpy(usermac,caTemp);
   }
   if(strlen(ip)>0){
			pasCvtGBK(2,ip,caTemp,16);
			strcpy(ip,caTemp);
   }
   if(strlen(sptype)>0){
			pasCvtGBK(2,sptype,caTemp,20);
			strcpy(sptype,caTemp);
   }
   if(strlen(areacode)>0){
			pasCvtGBK(2,areacode,caTemp,20);
			strcpy(areacode,caTemp);
   }
   
	 // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
	 
	 iTime_flag = atoi(time_flag);  // atoi ���ַ���ת���ɳ��������� ��atolһ��
	 switch(iTime_flag)
	 {
	 	  case 0:
	 	  	ncTimeToLong(sdate, &lStartTime);
	      ncTimeToLong(edate, &lTime);
	      break;
	   	case 1://����
	   		days = 1;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 2://�������
	   		days = 3;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 3://���һ��
	   		days = 7;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 4://���һ��
	   		days = 30;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 5://�������
	   		days = 90;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 6://�������
	   		days = 180;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	default:
	   		days = 1;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	  		break;
	 }
	 
	 // ʱ���ַ����ĸ�ʽת��
	 /*if(strlen(sdate) > 0 && strlen(edate) > 0){
		  ncTimeToLong(sdate, &lStartTime);
	 		ncTimeToLong(edate, &lTime);
	 }else{
		  ncDaysToLong(lTime, days, &lStartTime);
		  lTime=lTime+2*3600;
	 }*/
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lStartTime));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lTime));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 snprintf(sql, sizeof(sql), "select count(*) from ncsrvsmslog where logtime >= %lu and logtime <= %lu ", lStartTime, lTime);
 	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%' or usermac like '%%%s%%') ",  _keyword, _keyword);
	 }
	 if(!utStrIsSpaces(username))  // �û���
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(usermac))  // MAC
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and usermac like '%%%s%%'", usermac);
	 }
	 if(strlen(groupid)>0){
	 	sprintf(sql+strlen(sql)," and (groupid=%s or groupcode in (select groupname from ncsrvgroup where groupid=%s))",groupid,groupid);
	 }
	 if(!utStrIsSpaces(ip))  // IP
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s)) ",caShopids);
	 	
	 }
	 if(!utStrIsSpaces(sptype))  // ��Ӫ�����
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sptype like '%%%s%%'", sptype);
	 }
	 if(!utStrIsSpaces(areacode))  // ����
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and areacode like '%%%s%%'", areacode);
	 }
//	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
   // pasDbOneRecord ����¼��ѯ����0---��ȷ; !=0---�������, ���庬����μ� ���ݿ���й�����
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
   printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 snprintf(sql, sizeof(sql), "select sid, servicecode, userid, groupid, username, usermac, ip, logtime, smsname, sptype, areacode, areaname, contrycode, status, apmac from ncsrvsmslog where logtime >= %lu and logtime <= %lu", lStartTime, lTime);
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%' or usermac like '%%%s%%') ", _keyword, _keyword);
	 }
	 if(!utStrIsSpaces(username))  // �û���
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(usermac))  // MAC
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and usermac like '%%%s%%'", usermac);
	 }
	 if(!utStrIsSpaces(ip))  // IP
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 	if(strlen(groupid)>0){
	 	sprintf(sql+strlen(sql)," and (groupid=%s or groupcode in (select groupname from ncsrvgroup where groupid=%s))",groupid,groupid);
	 }
	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s)) ",caShopids);
	 	
	 }
	 
	 if(!utStrIsSpaces(sptype))  // ��Ӫ�����
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sptype = %d", atol(sptype));
	 }
	 if(!utStrIsSpaces(areacode))  // ����
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and areacode like '%%%d%%'", atol(areacode));
	 }
//	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lSid,
													     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
													     UT_TYPE_ULONG,  sizeof(long),           &lUserid,
													     UT_TYPE_ULONG,  sizeof(long),           &lGroupid,
															 UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername,
															 UT_TYPE_STRING, sizeof(caUsermac)-1,    caUsermac,
															 UT_TYPE_STRING, sizeof(caIp)-1,         caIp,
															 UT_TYPE_ULONG,  sizeof(long),           &lLogtime,
															 UT_TYPE_STRING, sizeof(caSmsname)-1,    caSmsname,
															 UT_TYPE_ULONG,  sizeof(long),           &lSptype,
															 UT_TYPE_ULONG,  sizeof(long),           &lAreacode,
															 UT_TYPE_STRING, sizeof(caAreaname)-1,   caAreaname,
															 UT_TYPE_ULONG,  sizeof(long),           &lContrycode,
															 UT_TYPE_ULONG,  sizeof(long),           &lStatus,
															 UT_TYPE_STRING, sizeof(caApmac)-1,      caApmac);
			
	 while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
		   if(lLogtime>0){
		   		strcpy(s_logtime, utTimFormat("%Y/%m/%d %H:%M:%S", lLogtime));
		   }else{
		   		strcpy(s_logtime, "");	
		   }
		   
		   switch(lStatus){
		   		case 0:strcpy(s_status, "ʧ��");break;
		   		case 1:strcpy(s_status, "�ɹ�");break;
		   		default:break;
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
	  	 

	  	 
	  	 if(strlen(caApmac)>0){
	  	 			char caapmac[64] = "", aUsermac[32] = "";
	  	 			printf("caApmac = %s\n", caApmac);
	  	 			pasCvtMacI((char *)caApmac,aUsermac);
//	  	 			strcpy(caapmac, pasCvtMac(caApmac));
//	  	 			printf("caapmac = %s\n", aUsermac);
	  	 			psAp = (ncPortalAp *)ncSrvGetApByMac(psShmHead,aUsermac);
	  	 			if (psAp)
	  	 			printf("psAp->caAddr = %s\n", psAp->caAddr);
	  	 			strcpy(s_apaddr, psAp->caAddr);
	  	 			printf("s_apaddr = %s\n", s_apaddr);
	  	 			if(lGroupid==0){
	  	 				lGroupid=psAp->lGroupid;
	  	 			}
	  	 			printf("pgropid=%lu\n",psAp->lGroupid);
	  	 }
	  	 if(lGroupid>0){
	  	 			strcpy(caGroupname, (char *)ncmGetDispNameByGid(psShmHead,lGroupid));
	  	 }
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVarF(psDbHead,"userid",iNum,"%lu",lUserid);
			 utPltPutLoopVarF(psDbHead,"logtime",iNum,s_logtime);
			 utPltPutLoopVar(psDbHead,"sptype",iNum,s_sptype);
			 utPltPutLoopVarF(psDbHead,"areacode",iNum,"%lu",lAreacode);
			 utPltPutLoopVarF(psDbHead,"contrycode",iNum,"%lu",lContrycode);
			 utPltPutLoopVar(psDbHead,"status",iNum,s_status);
			 utPltSetCvtHtml(1);
			 utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
			 utPltPutLoopVar(psDbHead,"usermac",iNum,caUsermac);
			 utPltPutLoopVar(psDbHead,"ip",iNum,caIp);
			 utPltPutLoopVar(psDbHead,"smsname",iNum,caSmsname);
			 utPltPutLoopVar(psDbHead,"areaname",iNum,caAreaname);
			 utPltPutLoopVar(psDbHead,"apmac",iNum,s_apaddr);
			 utPltSetCvtHtml(0);
			 
			 lSid = 0;
			 memset(caServicecode, 0, sizeof(caServicecode));
			 lGroupid = 0;
			 memset(caGroupname, 0, sizeof(caGroupname));
			 lUserid = 0;
			 memset(caUsername, 0, sizeof(caUsername));
			 memset(caUsermac, 0, sizeof(caUsermac));
			 memset(caIp, 0, sizeof(caIp));
			 lLogtime = 0;
			 memset(caSmsname, 0, sizeof(caSmsname));
			 lSptype = 0;
			 lAreacode = 0;
			 memset(caAreaname, 0, sizeof(caAreaname));
			 lContrycode = 0;
			 lStatus = 0;
			 memset(caApmac, 0, sizeof(caApmac));
			 memset(s_apaddr, 0, sizeof(s_apaddr));
			 memset(s_logtime, 0, sizeof(s_logtime));
			 memset(s_status, 0, sizeof(s_status));
			 memset(s_sptype, 0, sizeof(s_sptype));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lSid,
															     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
															     UT_TYPE_ULONG,  sizeof(long),           &lUserid,
															     UT_TYPE_ULONG,  sizeof(long),           &lGroupid,
																	 UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername,
																	 UT_TYPE_STRING, sizeof(caUsermac)-1,    caUsermac,
																	 UT_TYPE_STRING, sizeof(caIp)-1,         caIp,
																	 UT_TYPE_ULONG,  sizeof(long),           &lLogtime,
																	 UT_TYPE_STRING, sizeof(caSmsname)-1,    caSmsname,
																	 UT_TYPE_ULONG,  sizeof(long),           &lSptype,
																	 UT_TYPE_ULONG,  sizeof(long),           &lAreacode,
																	 UT_TYPE_STRING, sizeof(caAreaname)-1,   caAreaname,
																	 UT_TYPE_ULONG,  sizeof(long),           &lContrycode,
																	 UT_TYPE_ULONG,  sizeof(long),           &lStatus,
															 		 UT_TYPE_STRING, sizeof(caApmac)-1,      caApmac);
	 }
	 //utPltShowDb(psDbHead);
	 pasDbCloseCursor(psCur);
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_smslog_list.htm");
	 return 0;	
}

// �������û���־
int ncm_web_Place_userlog_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 int i = 0;
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 char *tmpgbk = NULL;
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 char caShopids[1024];
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 strcpy(caShopids,getDsShopids());
	 char auth_flag[2] = "";	//ѡ�����֤��־
	 // ��ʱ���ѯ����
	 char time_flag[2] = ""; // ѡ���ʱ���־��
	 char sort_flag[2] = "";
	 int  iAuth_flag = 0; 		//���ת�������֤��־atoi(auto_flag)
	 int  iTime_flag = 0; 		// ���ת�����ʱ��atoi(time_flag)
   int  days = 0; 					// ���ת�����ʱ���־
	 unsigned long lStartTime = 0;  // ��ʼʱ�䣺�Ǹ��ݵ�ǰʱ�� - ѡ��õ�������
	 unsigned long lTime = time(0); // ��ǰʱ��
	 char sdate[32] = ""; // ����Ŀ�ʼʱ��
   char edate[32] = ""; // ����Ľ���ʱ��
   char _sdate[32] = ""; // ��ʽת����Ŀ�ʼʱ��
	 char _edate[32] = ""; // ��ʽת����ĵ�ǰʱ��
	 
	 // ������Ϣ����
	 char sid[20] = "";
	 char portalname[16] = "";
	 char userid[16] = "";
	 char groupid[16] = "";
	 char username[32] = "";
	 char dispname[32] = "";
	 char ssid[32] = "";
	 char apname[32] = "";
	 char mac[18] = "";
	 char ip[16] = "";
	 char dev[24] = "";
	 char os[16] = "";
	 char bro[16] = "";
	 char devtype[8] = "";
	 char ubytes[32] = "";
	 char dbytes[32] = "";
	 char starttime[32] = "";
	 char endtime[32] = "";
	 char conntime[32] = "";
	 char cause[32] = "";
	 char groupcode[64] = "";
	 char areaid[32] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 long long	    lSid = 0;
	 char 					caPortalname[16] = "";
	 unsigned long	lUserid = 0;
	 unsigned long	lGroupid = 0;
	 char 					caGroupname[32] = "";
	 char 					caUsername[32] = "";
	 char 					caDispname[32] = "";
	 char 					caSsid[32] = "";
	 char 					caApname[32] = "";
	 char 					caMac[18] = "";
	 char 					caIp[16] = "";
	 char 					caDev[24] = "";
	 char 					caOs[16] = "";
	 char 					caBro[16] = "";
	 char 					caDevtype[8] = "";
	 unsigned long  lUbytes = 0;
	 unsigned long  lDbytes = 0;
	 unsigned long  lStarttime = 0;
	 unsigned long  lEndtime = 0;
	 unsigned long  lConntime = 0;
	 unsigned long  lCause = 0;
	 char           caGroupcode[32] = "";
	 char           s_starttime[32] = "";
	 char           s_endtime[32] = "";
	 char           s_conntime[32] = "";
	 double         dUbytes = 0.0;
	 double         dDbytes = 0.0;
	 
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 
	 // ��ҳ
	 unsigned long lTotRec = 0; // ��¼���е����ݵĸ���
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
   // ����
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // ����chartֵ
	 char flag[32] = "";
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
	 utMsgGetSomeNVar(psMsgHead, 37,"flag",        UT_TYPE_STRING,  sizeof(flag)-1,           flag,
															    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
															    "auth_flag",   UT_TYPE_STRING,  sizeof(auth_flag)-1,      auth_flag,
															    "time_flag",   UT_TYPE_STRING,  sizeof(time_flag)-1,      time_flag,
															    "sort_flag",   UT_TYPE_STRING,  sizeof(sort_flag)-1,      sort_flag,
															    "sdate",       UT_TYPE_STRING,  sizeof(sdate)-1,          sdate,
															    "edate",       UT_TYPE_STRING,  sizeof(edate)-1,          edate,
																	"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
																	"portalname",  UT_TYPE_STRING,  sizeof(portalname)-1,     portalname,
																	"userid",      UT_TYPE_STRING,  sizeof(userid)-1,         userid,
																	"groupid",     UT_TYPE_STRING,  sizeof(groupid)-1,        groupid,
																	"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
																	"dispname",    UT_TYPE_STRING,  sizeof(dispname)-1,       dispname,
																	"ssid",    		 UT_TYPE_STRING,  sizeof(ssid)-1,           ssid,
																	"apname",      UT_TYPE_STRING,  sizeof(apname)-1,         apname,
																	"mac",     		 UT_TYPE_STRING,  sizeof(mac)-1,            mac,
																	"ip",          UT_TYPE_STRING,  sizeof(ip)-1,             ip,
																	"dev",         UT_TYPE_STRING,  sizeof(dev)-1,            dev,
																	"os",          UT_TYPE_STRING,  sizeof(os)-1,             os,
																	"bro",         UT_TYPE_STRING,  sizeof(bro)-1,            bro,
																	"devtype",     UT_TYPE_STRING,  sizeof(devtype)-1,        devtype,
																	"ubytes",      UT_TYPE_STRING,  sizeof(ubytes)-1,         ubytes,
																	"dbytes",      UT_TYPE_STRING,  sizeof(dbytes)-1,         dbytes,
																	"starttime",   UT_TYPE_STRING,  sizeof(starttime)-1,      starttime,
																	"endtime",     UT_TYPE_STRING,  sizeof(endtime)-1,        endtime,
																	"conntime",    UT_TYPE_STRING,  sizeof(conntime)-1,       conntime,
																	"cause",       UT_TYPE_STRING,  sizeof(cause)-1,          cause,
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
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(portalname)>0){
			pasCvtGBK(2,portalname,caTemp,32);
			strcpy(portalname,caTemp);
   }
   if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,32);
			strcpy(username,caTemp);
   }
   if(strlen(mac)>0){
			pasCvtGBK(2,mac,caTemp,18);
			strcpy(mac,caTemp);
   }
   if(strlen(ip)>0){
			pasCvtGBK(2,ip,caTemp,16);
			strcpy(ip,caTemp);
   }
	 if(strlen(os)>0){
			pasCvtGBK(2,os,caTemp,16);
			strcpy(os,caTemp);
   }
   if(strlen(bro)>0){
			pasCvtGBK(2,bro,caTemp,16);
			strcpy(bro,caTemp);
   }
   if(strlen(dev)>0){
			pasCvtGBK(2,dev,caTemp,24);
			strcpy(dev,caTemp);
   }
   if(strlen(devtype)>0){
			pasCvtGBK(2,devtype,caTemp,8);
			strcpy(devtype,caTemp);
   }
   if(strlen(groupcode)>0){
			pasCvtGBK(2,groupcode,caTemp,32);
			strcpy(groupcode,caTemp);
   }
   //��֤��־
   iAuth_flag = atoi(auth_flag);
	 // ����
	 switch(atoi(sort_flag)){
	 		case 1:	strcpy(sort,"starttime"); strcpy(dir,"asc");  break;
	 		case 2:	strcpy(sort,"starttime"); strcpy(dir,"desc"); break;
	 		case 3:	strcpy(sort,"endtime");   strcpy(dir,"asc");  break;
	 		case 4:	strcpy(sort,"endtime");   strcpy(dir,"desc"); break;
	 		default:strcpy(sort,"sid");       strcpy(dir,"desc"); break;
	 }
	 
	 iTime_flag = atoi(time_flag);  // atoi ���ַ���ת���ɳ��������� ��atolһ��
	 switch(iTime_flag){
	 	  case 0:
	 	  	ncTimeToLong(sdate, &lStartTime);
	      ncTimeToLong(edate, &lTime);
	      break;
	   	case 1://����
	   		days = 1;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 2://�������
	   		days = 3;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 3://���һ��
	   		days = 7;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 4://���һ��
	   		days = 30;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 5://�������
	   		days = 90;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 6://�������
	   		days = 180;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	default:
	   		days = 1;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	  		break;
	 }
	 
	 // ʱ���ַ����ĸ�ʽת��
	 /*if(strlen(sdate) > 0 && strlen(edate) > 0){
		  ncTimeToLong(sdate, &lStartTime);
	 		ncTimeToLong(edate, &lTime);
	 }
	 else{
		  ncDaysToLong(lTime, days, &lStartTime);
		  lTime=lTime+2*3600;
	 }*/
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lStartTime));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lTime));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 if(iAuth_flag == 0)
		  snprintf(sql, sizeof(sql), "select count(*) from ncsrvuserlog where starttime >= %lu and endtime <= %lu ", lStartTime, lTime);
	 else if(iAuth_flag == 1)
	 		snprintf(sql, sizeof(sql), "select count(*) from ncsrvuserlog where starttime >= %lu and endtime <= %lu and (username != 'Guest' and username != '') ", lStartTime, lTime);
	 else
	 		snprintf(sql, sizeof(sql), "select count(*) from ncsrvuserlog where starttime >= %lu and endtime <= %lu and (username = 'Guest' or username = '') ", lStartTime, lTime);
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (portalname like '%%%s%%' or username like '%%%s%%') ",  _keyword, _keyword);
	 }
      if(strlen(caShopids)>0){
      	sprintf(sql+strlen(sql)," and groupid in (%s) ",caShopids);
      }
      
   if(strlen(groupid)>0){
	 	sprintf(sql+strlen(sql)," and groupid=%s ",groupid);
	 }
      
	 if(!utStrIsSpaces(portalname))  // Portal Name
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and portalname like '%%%s%%'", portalname);
	 }
	 if(!utStrIsSpaces(username))  // �û���
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(mac))  // mac
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", mac);
	 }
	 if(!utStrIsSpaces(ip))  // ip
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 if(!utStrIsSpaces(devtype))  // �豸����
	 {
	 		if(strcmp(devtype, "����")==0){
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and devtype = ''");
	 		}else{
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and devtype = '%s'", devtype);
	 		}
	 }
	 if(!utStrIsSpaces(dev))  // �豸Ʒ��
	 {
	 		if(strcmp(dev, "����")==0){
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev = ''");	
	 		}else{
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev = '%s'", dev);
	 		}
	 }
	 if(!utStrIsSpaces(os))  // ����ϵͳ
	 {
	 		if(strcmp(os, "����")==0){
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and os = ''");	
	 		}else{
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and os = '%s'", os);
	 		}
	 }
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
   // pasDbOneRecord ����¼��ѯ����0---��ȷ; !=0---�������, ���庬����μ� ���ݿ���й�����
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
   
	 iStart = atol(start);
	 iLimit = atol(limit);
	 if(iAuth_flag == 0)
	 		snprintf(sql, sizeof(sql), "select sid, portalname, userid, groupid, username, dispname, ssid, apname, mac, ip, dev, os, bro, devtype, ubytes, dbytes, starttime, endtime, conntime, cause from ncsrvuserlog where starttime >= %lu and endtime <= %lu", lStartTime, lTime);
	 else if(iAuth_flag == 1)
	 		snprintf(sql, sizeof(sql), "select sid, portalname, userid, groupid, username, dispname, ssid, apname, mac, ip, dev, os, bro, devtype, ubytes, dbytes, starttime, endtime, conntime, cause from ncsrvuserlog where starttime >= %lu and endtime <= %lu and (username != 'Guest' and username != '') ", lStartTime, lTime);
	 else
	 		snprintf(sql, sizeof(sql), "select sid, portalname, userid, groupid, username, dispname, ssid, apname, mac, ip, dev, os, bro, devtype, ubytes, dbytes, starttime, endtime, conntime, cause from ncsrvuserlog where starttime >= %lu and endtime <= %lu and (username = 'Guest' or username = '') ", lStartTime, lTime);
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (portalname like '%%%s%%' or username like '%%%s%%') ", _keyword, _keyword);
	 }
	 if(!utStrIsSpaces(portalname))  // Portal Name
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and portalname like '%%%s%%'", portalname);
	 }
	 if(!utStrIsSpaces(username))  // �û���
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(mac))  // mac
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", mac);
	 }
	 if(!utStrIsSpaces(ip))  // ip
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 if(!utStrIsSpaces(devtype))  // �豸����
	 {
	 		if(strcmp(devtype, "����")==0){
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and devtype = ''");
	 		}else{
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and devtype = '%s'", devtype);
	 		}
	 }
	 if(!utStrIsSpaces(dev))  // �豸Ʒ��
	 {
	 		if(strcmp(dev, "����")==0){
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev = ''");	
	 		}else{
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev = '%s'", dev);
	 		}
	 }
	 if(!utStrIsSpaces(os))  // ����ϵͳ
	 {
	 		if(strcmp(os, "����")==0){
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and os = ''");	
	 		}else{
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and os = '%s'", os);
	 		}
	 }
	     if(strlen(caShopids)>0){
      	sprintf(sql+strlen(sql)," and groupid in (%s) ",caShopids);
      }
    if(strlen(groupid)>0){
	 	sprintf(sql+strlen(sql)," and groupid=%s ",groupid);
	 }
      
	 	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_LONG8,  sizeof(long long),     &lSid,
													     UT_TYPE_STRING, sizeof(caPortalname)-1,caPortalname,
													     UT_TYPE_ULONG,  sizeof(long),          &lUserid,
													     UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
															 UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
															 UT_TYPE_STRING, sizeof(caDispname)-1,  caDispname,
															 UT_TYPE_STRING, sizeof(caSsid)-1,      caSsid,
															 UT_TYPE_STRING, sizeof(caApname)-1,    caApname,
															 UT_TYPE_STRING, sizeof(caMac)-1,       caMac,
															 UT_TYPE_STRING, sizeof(caIp)-1,        caIp,
															 UT_TYPE_STRING, sizeof(caDev)-1,       caDev,
															 UT_TYPE_STRING, sizeof(caOs)-1,        caOs,
															 UT_TYPE_STRING, sizeof(caBro)-1,       caBro,
															 UT_TYPE_STRING, sizeof(caDevtype)-1,   caDevtype,
															 UT_TYPE_ULONG,  sizeof(long),          &lUbytes,
															 UT_TYPE_ULONG,  sizeof(long),          &lDbytes,
															 UT_TYPE_ULONG,  sizeof(long),          &lStarttime,
															 UT_TYPE_ULONG,  sizeof(long),          &lEndtime,
															 UT_TYPE_ULONG,  sizeof(long),          &lConntime,
															 UT_TYPE_ULONG,  sizeof(long),          &lCause);
			
	 while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
			 if(lGroupid>0){
	  	 		strcpy(caGroupname, (char *)ncmGetDispNameByGid(psShmHead,lGroupid));
	  	 }
			 
		   if(lStarttime>0){
		   		strcpy(s_starttime, utTimFormat("%Y/%m/%d %H:%M:%S", lStarttime));
		   }else{
		   		strcpy(s_starttime, "");	
		   }
		   if(lEndtime>0){
		   		strcpy(s_endtime, utTimFormat("%Y/%m/%d %H:%M:%S", lEndtime));
		   }else{
		   		strcpy(s_endtime, "");
		   }
		   ncNumToTimeE(lConntime, s_conntime);
		   
		   dUbytes = (double)lUbytes/(1024*1024);
		   dDbytes = (double)lDbytes/(1024*1024);
		   
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVarF(psDbHead,"userid",iNum,"%lu",lUserid);
			 utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
			 utPltPutLoopVarF(psDbHead,"ubytes",iNum,"%.2f",dUbytes);
			 utPltPutLoopVarF(psDbHead,"dbytes",iNum,"%.2f",dDbytes);
			 utPltPutLoopVarF(psDbHead,"starttime",iNum,s_starttime);
			 utPltPutLoopVarF(psDbHead,"endtime",iNum,s_endtime);
			 utPltPutLoopVarF(psDbHead,"conntime",iNum,s_conntime);
			 utPltPutLoopVarF(psDbHead,"cause",iNum,"%lu",lCause);
			 utPltSetCvtHtml(1);
			 utPltPutLoopVar(psDbHead,"portalname",iNum,caPortalname);
			 utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
			 utPltPutLoopVar(psDbHead,"ssid",iNum,caSsid);
			 utPltPutLoopVar(psDbHead,"apname",iNum,caApname);
			 utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
			 utPltPutLoopVar(psDbHead,"ip",iNum,caIp);
			 utPltPutLoopVar(psDbHead,"dev",iNum,caDev);
			 utPltPutLoopVar(psDbHead,"os",iNum,caOs);
			 utPltPutLoopVar(psDbHead,"bro",iNum,caBro);
			 utPltPutLoopVar(psDbHead,"devtype",iNum,caDevtype);
			 utPltSetCvtHtml(0);
			 
			 lSid = 0;
			 memset(caPortalname, 0, sizeof(caPortalname));
			 lUserid = 0;
			 lGroupid = 0;
			 memset(caGroupname, 0, sizeof(caGroupname));
			 memset(caUsername, 0, sizeof(caUsername));
			 memset(caDispname, 0, sizeof(caDispname));
			 memset(caSsid, 0, sizeof(caSsid));
			 memset(caApname, 0, sizeof(caApname));
			 memset(caMac, 0, sizeof(caMac));
			 memset(caIp, 0, sizeof(caIp));
			 memset(caDev, 0, sizeof(caDev));
			 memset(caOs, 0, sizeof(caOs));
			 memset(caBro, 0, sizeof(caBro));
			 memset(caDevtype, 0, sizeof(caDevtype));
			 lUbytes = 0;
			 lDbytes = 0;
			 lStarttime = 0;
			 lEndtime = 0;
			 lConntime = 0;
			 lCause = 0;
			 memset(s_starttime, 0, sizeof(s_starttime));
			 memset(s_endtime, 0, sizeof(s_endtime));
			 memset(s_conntime, 0, sizeof(s_conntime));
			 dUbytes = 0.0;
			 dDbytes = 0.0;
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  4,               		  &lSid,
															     UT_TYPE_STRING, sizeof(caPortalname)-1,caPortalname,
															     UT_TYPE_ULONG,  4,               		  &lUserid,
															     UT_TYPE_ULONG,  4,               		  &lGroupid,
																	 UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
																	 UT_TYPE_STRING, sizeof(caDispname)-1,  caDispname,
																	 UT_TYPE_STRING, sizeof(caSsid)-1,      caSsid,
																	 UT_TYPE_STRING, sizeof(caApname)-1,    caApname,
																	 UT_TYPE_STRING, sizeof(caMac)-1,       caMac,
																	 UT_TYPE_STRING, sizeof(caIp)-1,        caIp,
																	 UT_TYPE_STRING, sizeof(caDev)-1,       caDev,
																	 UT_TYPE_STRING, sizeof(caOs)-1,        caOs,
																	 UT_TYPE_STRING, sizeof(caBro)-1,       caBro,
																	 UT_TYPE_STRING, sizeof(caDevtype)-1,   caDevtype,
																	 UT_TYPE_ULONG,  4,               		  &lUbytes,
																	 UT_TYPE_ULONG,  4,               		  &lDbytes,
																	 UT_TYPE_ULONG,  4,               		  &lStarttime,
																	 UT_TYPE_ULONG,  4,               		  &lEndtime,
																	 UT_TYPE_ULONG,  4,               		  &lConntime,
																	 UT_TYPE_ULONG,  4,               		  &lCause);
	 }
	 // utPltShowDb(psDbHead);
	 pasDbCloseCursor(psCur);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_userlog_list.htm");
	 
	 return 0;	
}
// �������û���־
int ncm_web_Place_alarmlog_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 int i = 0;
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 char *tmpgbk = NULL;
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 char caShopids[1024];
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	  strcpy(caShopids,getDsShopids());
	 // ��ʱ���ѯ����
	 char time_flag[2] = ""; // ѡ���ʱ���־��
	 char sort_flag[2] = "";
	 int  iTime_flag = 0; // ���ת�����ʱ��atoi(time_flag)
   int  days = 0; // ���ת�����ʱ���־
	 unsigned long lStartTime = 0;  // ��ʼʱ�䣺�Ǹ��ݵ�ǰʱ�� - ѡ��õ�������
	 unsigned long lTime = time(0); // ��ǰʱ��
	 char sdate[32] = ""; // ����Ŀ�ʼʱ��
   char edate[32] = ""; // ����Ľ���ʱ��
   char _sdate[32] = ""; // ��ʽת����Ŀ�ʼʱ��
	 char _edate[32] = ""; // ��ʽת����ĵ�ǰʱ��
	 
	 // ������Ϣ����
	 char sid[20] = "";
	 char portalname[16] = "";
	 char userid[16] = "";
	 char groupid[16] = "";
	 char username[32] = "";
	 char dispname[32] = "";
	 char ssid[32] = "";
	 char apname[32] = "";
	 char mac[18] = "";
	 char ip[16] = "";
	 char dev[24] = "";
	 char os[16] = "";
	 char bro[16] = "";
	 char devtype[8] = "";
	 char ubytes[32] = "";
	 char dbytes[32] = "";
	 char starttime[32] = "";
	 char endtime[32] = "";
	 char conntime[32] = "";
	 char cause[32] = "";
	 char groupcode[32] = "";
	 char areaid[32] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 long long	    lSid = 0;
	 char 					caPortalname[16] = "";
	 unsigned long	lUserid = 0;
	 unsigned long	lGroupid = 0;
	 char 					caGroupname[32] = "";
	 char 					caUsername[32] = "";
	 char 					caDispname[32] = "";
	 char 					caSsid[32] = "";
	 char 					caApname[32] = "";
	 char 					caMac[18] = "";
	 char 					caIp[16] = "";
	 char 					caDev[24] = "";
	 char 					caOs[16] = "";
	 char 					caBro[16] = "";
	 char 					caDevtype[8] = "";
	 unsigned long  lUbytes = 0;
	 unsigned long  lDbytes = 0;
	 unsigned long  lStarttime = 0;
	 unsigned long  lEndtime = 0;
	 unsigned long  lConntime = 0;
	 unsigned long  lCause = 0;
	 char           caGroupcode[32] = "";
	 char           s_starttime[32] = "";
	 char           s_endtime[32] = "";
	 char           s_conntime[32] = "";
	 double         dUbytes = 0.0;
	 double         dDbytes = 0.0;
	 
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 
	 // ��ҳ
	 unsigned long lTotRec = 0; // ��¼���е����ݵĸ���
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
   // ����
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // ����chartֵ
	 char flag[32] = "";
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
	 utMsgGetSomeNVar(psMsgHead, 36,"flag",        UT_TYPE_STRING,  sizeof(flag)-1,           flag,
															    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
															    "time_flag",   UT_TYPE_STRING,  sizeof(time_flag)-1,      time_flag,
															    "sort_flag",   UT_TYPE_STRING,  sizeof(sort_flag)-1,      sort_flag,
															    "sdate",       UT_TYPE_STRING,  sizeof(sdate)-1,          sdate,
															    "edate",       UT_TYPE_STRING,  sizeof(edate)-1,          edate,
																	"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
																	"portalname",  UT_TYPE_STRING,  sizeof(portalname)-1,     portalname,
																	"userid",      UT_TYPE_STRING,  sizeof(userid)-1,         userid,
																	"groupid",     UT_TYPE_STRING,  sizeof(groupid)-1,        groupid,
																	"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
																	"dispname",    UT_TYPE_STRING,  sizeof(dispname)-1,       dispname,
																	"ssid",    		 UT_TYPE_STRING,  sizeof(ssid)-1,           ssid,
																	"apname",      UT_TYPE_STRING,  sizeof(apname)-1,         apname,
																	"mac",     		 UT_TYPE_STRING,  sizeof(mac)-1,            mac,
																	"ip",          UT_TYPE_STRING,  sizeof(ip)-1,             ip,
																	"dev",         UT_TYPE_STRING,  sizeof(dev)-1,            dev,
																	"os",          UT_TYPE_STRING,  sizeof(os)-1,             os,
																	"bro",         UT_TYPE_STRING,  sizeof(bro)-1,            bro,
																	"devtype",     UT_TYPE_STRING,  sizeof(devtype)-1,        devtype,
																	"ubytes",      UT_TYPE_STRING,  sizeof(ubytes)-1,         ubytes,
																	"dbytes",      UT_TYPE_STRING,  sizeof(dbytes)-1,         dbytes,
																	"starttime",   UT_TYPE_STRING,  sizeof(starttime)-1,      starttime,
																	"endtime",     UT_TYPE_STRING,  sizeof(endtime)-1,        endtime,
																	"conntime",    UT_TYPE_STRING,  sizeof(conntime)-1,       conntime,
																	"cause",       UT_TYPE_STRING,  sizeof(cause)-1,          cause,
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
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(portalname)>0){
			pasCvtGBK(2,portalname,caTemp,32);
			strcpy(portalname,caTemp);
   }
   if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,32);
			strcpy(username,caTemp);
   }
   if(strlen(mac)>0){
			pasCvtGBK(2,mac,caTemp,18);
			strcpy(mac,caTemp);
   }
   if(strlen(ip)>0){
			pasCvtGBK(2,ip,caTemp,16);
			strcpy(ip,caTemp);
   }
	 if(strlen(os)>0){
			pasCvtGBK(2,os,caTemp,16);
			strcpy(os,caTemp);
   }
   if(strlen(bro)>0){
			pasCvtGBK(2,bro,caTemp,16);
			strcpy(bro,caTemp);
   }
   if(strlen(dev)>0){
			pasCvtGBK(2,dev,caTemp,24);
			strcpy(dev,caTemp);
   }
   if(strlen(devtype)>0){
			pasCvtGBK(2,devtype,caTemp,8);
			strcpy(devtype,caTemp);
   }
   if(strlen(groupcode)>0){
			pasCvtGBK(2,groupcode,caTemp,32);
			strcpy(groupcode,caTemp);
   }
   
	 // ����
	 switch(atoi(sort_flag)){
	 		case 1:	strcpy(sort,"starttime"); strcpy(dir,"asc");  break;
	 		case 2:	strcpy(sort,"starttime"); strcpy(dir,"desc"); break;
	 		case 3:	strcpy(sort,"endtime");   strcpy(dir,"asc");  break;
	 		case 4:	strcpy(sort,"endtime");   strcpy(dir,"desc"); break;
	 		default:strcpy(sort,"sid");       strcpy(dir,"desc"); break;
	 }
	 
	 iTime_flag = atoi(time_flag);  // atoi ���ַ���ת���ɳ��������� ��atolһ��
	 switch(iTime_flag){
	 	  case 0:
	 	  	ncTimeToLong(sdate, &lStartTime);
	      ncTimeToLong(edate, &lTime);
	      break;
	   	case 1://����
	   		days = 1;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 2://�������
	   		days = 3;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 3://���һ��
	   		days = 7;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 4://���һ��
	   		days = 30;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 5://�������
	   		days = 90;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 6://�������
	   		days = 180;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	default:
	   		days = 1;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	  		break;
	 }
	 
	 switch(atol(ssid)){
	 		case 0:strcpy(ssid, "");break;
	 		case 1:strcpy(ssid, "ICPIC");break;
	 		case 2:strcpy(ssid, "CPICVIP");break;
	 		default: break;
	 }
	 
	 // ʱ���ַ����ĸ�ʽת��
	 /*if(strlen(sdate) > 0 && strlen(edate) > 0){
		  ncTimeToLong(sdate, &lStartTime);
	 		ncTimeToLong(edate, &lTime);
	 }
	 else{
		  ncDaysToLong(lTime, days, &lStartTime);
		  lTime=lTime+2*3600;
	 }*/
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lStartTime));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lTime));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 snprintf(sql, sizeof(sql), "select count(*) from ncsrvuserlog where starttime >= %lu and endtime <= %lu ", lStartTime, lTime);
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and devtype = 'mspc'");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (portalname like '%%%s%%' or username like '%%%s%%') ",  _keyword, _keyword);
	 }
	 if(!utStrIsSpaces(portalname))  // Portal Name
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and portalname like '%%%s%%'", portalname);
	 }
	 if(!utStrIsSpaces(username))  // �û���
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(mac))  // mac
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", mac);
	 }
	 if(!utStrIsSpaces(ip))  // ip
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 if(!utStrIsSpaces(ssid))  // ssid
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ssid = '%s'", ssid);
	 }
	 
	  if(strlen(caShopids)>0){
      	sprintf(sql+strlen(sql)," and groupid in (%s) ",caShopids);
      }
    	 if(strlen(groupid)>0){
	 	sprintf(sql+strlen(sql)," and groupid=%s ",groupid);
	 }  
      
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
   // pasDbOneRecord ����¼��ѯ����0---��ȷ; !=0---�������, ���庬����μ� ���ݿ���й�����
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
   
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 snprintf(sql, sizeof(sql), "select sid, portalname, userid, groupid, username, dispname, ssid, apname, mac, ip, dev, os, bro, devtype, ubytes, dbytes, starttime, endtime, conntime, cause from ncsrvuserlog where starttime >= %lu and endtime <= %lu", lStartTime, lTime);
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and devtype = 'mspc'");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (portalname like '%%%s%%' or username like '%%%s%%') ", _keyword, _keyword);
	 }
	 if(!utStrIsSpaces(portalname))  // Portal Name
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and portalname like '%%%s%%'", portalname);
	 }
	 if(!utStrIsSpaces(username))  // �û���
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	  if(strlen(caShopids)>0){
      	sprintf(sql+strlen(sql)," and groupid in (%s) ",caShopids);
      }
      
   if(strlen(groupid)>0){
	 	sprintf(sql+strlen(sql)," and groupid=%s ",groupid);
	 }  
      
      
	 if(!utStrIsSpaces(mac))  // mac
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", mac);
	 }
	 if(!utStrIsSpaces(ip))  // ip
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 if(!utStrIsSpaces(ssid))  // ssid
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ssid = '%s'", ssid);
	 }
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_LONG8,  sizeof(long long),     &lSid,
													     UT_TYPE_STRING, sizeof(caPortalname)-1,caPortalname,
													     UT_TYPE_ULONG,  sizeof(long),          &lUserid,
													     UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
															 UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
															 UT_TYPE_STRING, sizeof(caDispname)-1,  caDispname,
															 UT_TYPE_STRING, sizeof(caSsid)-1,      caSsid,
															 UT_TYPE_STRING, sizeof(caApname)-1,    caApname,
															 UT_TYPE_STRING, sizeof(caMac)-1,       caMac,
															 UT_TYPE_STRING, sizeof(caIp)-1,        caIp,
															 UT_TYPE_STRING, sizeof(caDev)-1,       caDev,
															 UT_TYPE_STRING, sizeof(caOs)-1,        caOs,
															 UT_TYPE_STRING, sizeof(caBro)-1,       caBro,
															 UT_TYPE_STRING, sizeof(caDevtype)-1,   caDevtype,
															 UT_TYPE_ULONG,  sizeof(long),          &lUbytes,
															 UT_TYPE_ULONG,  sizeof(long),          &lDbytes,
															 UT_TYPE_ULONG,  sizeof(long),          &lStarttime,
															 UT_TYPE_ULONG,  sizeof(long),          &lEndtime,
															 UT_TYPE_ULONG,  sizeof(long),          &lConntime,
															 UT_TYPE_ULONG,  sizeof(long),          &lCause);
			
	 while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
			 if(lGroupid>0){
	  	 		strcpy(caGroupname, (char *)ncmGetDispNameByGid(psShmHead,lGroupid));
	  	 }
			 
		   if(lStarttime>0){
		   		strcpy(s_starttime, utTimFormat("%Y/%m/%d %H:%M:%S", lStarttime));
		   }else{
		   		strcpy(s_starttime, "");	
		   }
		   if(lEndtime>0){
		   		strcpy(s_endtime, utTimFormat("%Y/%m/%d %H:%M:%S", lEndtime));
		   }else{
		   		strcpy(s_endtime, "");
		   }
		   ncNumToTimeE(lConntime, s_conntime);
		   
		   dUbytes = (double)lUbytes/(1024*1024);
		   dDbytes = (double)lDbytes/(1024*1024);
		   
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVarF(psDbHead,"userid",iNum,"%lu",lUserid);
			 utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
			 utPltPutLoopVarF(psDbHead,"ubytes",iNum,"%.2f",dUbytes);
			 utPltPutLoopVarF(psDbHead,"dbytes",iNum,"%.2f",dDbytes);
			 utPltPutLoopVarF(psDbHead,"starttime",iNum,s_starttime);
			 utPltPutLoopVarF(psDbHead,"endtime",iNum,s_endtime);
			 utPltPutLoopVarF(psDbHead,"conntime",iNum,s_conntime);
			 utPltPutLoopVarF(psDbHead,"cause",iNum,"%lu",lCause);
			 utPltSetCvtHtml(1);
			 utPltPutLoopVar(psDbHead,"portalname",iNum,caPortalname);
			 utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
			 utPltPutLoopVar(psDbHead,"ssid",iNum,caSsid);
			 utPltPutLoopVar(psDbHead,"apname",iNum,caApname);
			 utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
			 utPltPutLoopVar(psDbHead,"ip",iNum,caIp);
			 utPltPutLoopVar(psDbHead,"dev",iNum,caDev);
			 utPltPutLoopVar(psDbHead,"os",iNum,caOs);
			 utPltPutLoopVar(psDbHead,"bro",iNum,caBro);
			 utPltPutLoopVar(psDbHead,"devtype",iNum,caDevtype);
			 utPltSetCvtHtml(0);
			 
			 lSid = 0;
			 memset(caPortalname, 0, sizeof(caPortalname));
			 lUserid = 0;
			 lGroupid = 0;
			 memset(caGroupname, 0, sizeof(caGroupname));
			 memset(caUsername, 0, sizeof(caUsername));
			 memset(caDispname, 0, sizeof(caDispname));
			 memset(caSsid, 0, sizeof(caSsid));
			 memset(caApname, 0, sizeof(caApname));
			 memset(caMac, 0, sizeof(caMac));
			 memset(caIp, 0, sizeof(caIp));
			 memset(caDev, 0, sizeof(caDev));
			 memset(caOs, 0, sizeof(caOs));
			 memset(caBro, 0, sizeof(caBro));
			 memset(caDevtype, 0, sizeof(caDevtype));
			 lUbytes = 0;
			 lDbytes = 0;
			 lStarttime = 0;
			 lEndtime = 0;
			 lConntime = 0;
			 lCause = 0;
			 memset(s_starttime, 0, sizeof(s_starttime));
			 memset(s_endtime, 0, sizeof(s_endtime));
			 memset(s_conntime, 0, sizeof(s_conntime));
			 dUbytes = 0.0;
			 dDbytes = 0.0;
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  4,               		  &lSid,
															     UT_TYPE_STRING, sizeof(caPortalname)-1,caPortalname,
															     UT_TYPE_ULONG,  4,               		  &lUserid,
															     UT_TYPE_ULONG,  4,               		  &lGroupid,
																	 UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
																	 UT_TYPE_STRING, sizeof(caDispname)-1,  caDispname,
																	 UT_TYPE_STRING, sizeof(caSsid)-1,      caSsid,
																	 UT_TYPE_STRING, sizeof(caApname)-1,    caApname,
																	 UT_TYPE_STRING, sizeof(caMac)-1,       caMac,
																	 UT_TYPE_STRING, sizeof(caIp)-1,        caIp,
																	 UT_TYPE_STRING, sizeof(caDev)-1,       caDev,
																	 UT_TYPE_STRING, sizeof(caOs)-1,        caOs,
																	 UT_TYPE_STRING, sizeof(caBro)-1,       caBro,
																	 UT_TYPE_STRING, sizeof(caDevtype)-1,   caDevtype,
																	 UT_TYPE_ULONG,  4,               		  &lUbytes,
																	 UT_TYPE_ULONG,  4,               		  &lDbytes,
																	 UT_TYPE_ULONG,  4,               		  &lStarttime,
																	 UT_TYPE_ULONG,  4,               		  &lEndtime,
																	 UT_TYPE_ULONG,  4,               		  &lConntime,
																	 UT_TYPE_ULONG,  4,               		  &lCause);
	 }
	 // utPltShowDb(psDbHead);
	 pasDbCloseCursor(psCur);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_alarmlog_list.htm");
	 
	 return 0;	
}

// ������MAC��־
int ncm_web_Place_maclog_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 int i = 0;
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 char *tmpgbk = NULL;
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ��ʱ���ѯ����
	 char time_flag[2] = ""; // ѡ���ʱ���־��
	 int  iTime_flag = 0; // ���ת�����ʱ��atoi(time_flag)
   int  days = 0; // ���ת�����ʱ���־
	 unsigned long lStartTime = 0;  // ��ʼʱ�䣺�Ǹ��ݵ�ǰʱ�� - ѡ��õ�������
	 unsigned long lTime = time(0); // ��ǰʱ��
	 char sdate[32] = ""; // ����Ŀ�ʼʱ��
   char edate[32] = ""; // ����Ľ���ʱ��
   char _sdate[32] = ""; // ��ʽת����Ŀ�ʼʱ��
	 char _edate[32] = ""; // ��ʽת����ĵ�ǰʱ��
	 
	 // ������Ϣ����
	 char pname[32] = "";
	 char mac[18] = "";
	 char apmac[18] = "";
	 char groupid[32] = "";
	 char groupcode[32] = "";
	 char mark[64] = "";
	 char lrssi[32] = "";
	 char urssi[32] = "";
	 char starttime[32] = "";
	 char endtime[32] = "";
	 char flag[32] = "";
	 char areaid[32] ="";
	 
	 // ������ݿ�ȡ�������ݵı���
	 char 					caPname[32] = "";
	 char 					caMac[18] = "";
	 char 					caApmac[18] = "";
	 unsigned long	lGroupid = 0;
	 char           caGroupname[32] = "";
	 char 					caGroupcode[32] = "";
	 char 					caMark[64] = "";
	 unsigned long  lLrssi = 0;
	 unsigned long  lUrssi = 0;
	 unsigned long  lStarttime = 0;
	 unsigned long  lEndtime = 0;
	 unsigned long  lFlag = 0;
	 char           s_starttime[32] = "";
	 char           s_endtime[32] = "";
	 
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 
	 // ��ҳ
	 unsigned long lTotRec = 0; // ��¼���е����ݵĸ���
	 
	 // ����
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
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
															    "time_flag",   UT_TYPE_STRING,  sizeof(time_flag)-1,      time_flag,
															    "sdate",       UT_TYPE_STRING,  sizeof(sdate)-1,          sdate,
															    "edate",       UT_TYPE_STRING,  sizeof(edate)-1,          edate,
																	"pname",       UT_TYPE_STRING,  sizeof(pname)-1,          pname,
																	"mac",         UT_TYPE_STRING,  sizeof(mac)-1,            mac,
																	"apmac",       UT_TYPE_STRING,  sizeof(apmac)-1,          apmac,
																	"groupid",     UT_TYPE_STRING,  sizeof(groupid)-1,        groupid,
																	"groupcode",   UT_TYPE_STRING,  sizeof(groupcode)-1,      groupcode,
																	"mark",        UT_TYPE_STRING,  sizeof(mark)-1,           mark,
																	"lrssi",       UT_TYPE_STRING,  sizeof(lrssi)-1,          lrssi,
																	"urssi",       UT_TYPE_STRING,  sizeof(urssi)-1,          urssi,
																	"starttime",   UT_TYPE_STRING,  sizeof(starttime)-1,      starttime,
																	"endtime",     UT_TYPE_STRING,  sizeof(endtime)-1,        endtime,
																	"flag",        UT_TYPE_STRING,  sizeof(flag)-1,           flag,
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
	 
	 // ����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(pname)>0){
			pasCvtGBK(2,pname,caTemp,32);
			strcpy(pname,caTemp);
   }
   if(strlen(mac)>0){
			pasCvtGBK(2,mac,caTemp,18);
			strcpy(mac,caTemp);
   }
   if(strlen(apmac)>0){
			pasCvtGBK(2,apmac,caTemp,18);
			strcpy(apmac,caTemp);
   }
   if(strlen(groupcode)>0){
			pasCvtGBK(2,groupcode,caTemp,32);
			strcpy(groupcode,caTemp);
   }
   if(strlen(mark)>0){
			pasCvtGBK(2,mark,caTemp,64);
			strcpy(mark,caTemp);
   }
   
	 // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"pname");
	 }
	 
	 iTime_flag = atoi(time_flag);  // atoi ���ַ���ת���ɳ��������� ��atolһ��
	 switch(iTime_flag)
	 {
	 	  case 0:
	 	  	ncTimeToLong(sdate, &lStartTime);
	      ncTimeToLong(edate, &lTime);
	      break;
	   	case 1://����
	   		days = 1;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 2://�������
	   		days = 3;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 3://���һ��
	   		days = 7;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 4://���һ��
	   		days = 30;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 5://�������
	   		days = 90;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 6://�������
	   		days = 180;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	default:
	   		days = 1;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	  		break;
	 }
	 
	 // ʱ���ַ����ĸ�ʽת��
	 /*if(strlen(sdate) > 0 && strlen(edate) > 0){
		  ncTimeToLong(sdate, &lStartTime);
	 		ncTimeToLong(edate, &lTime);
	 }else{
		  ncDaysToLong(lTime, days, &lStartTime);
		  lTime=lTime+2*3600;
	 }*/
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lStartTime));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lTime));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 snprintf(sql, sizeof(sql), "select count(*) from ncsrvmaclog where starttime >= %lu and endtime <= %lu ", lStartTime, lTime);
 	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (pname like '%%%s%%' or mac like '%s%%' or apmac like '%s%%') ", _keyword, _keyword, _keyword);
	 }
	 if(!utStrIsSpaces(pname))  // �û���
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and pname like '%%%s%%'", pname);
	 }
	 if(!utStrIsSpaces(mac))  // MAC
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", mac);
	 }
	 if(!utStrIsSpaces(apmac))  // IP
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and apmac like '%%%s%%'", apmac);
	 }
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
   // pasDbOneRecord ����¼��ѯ����0---��ȷ; !=0---�������, ���庬����μ� ���ݿ���й�����
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
   printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 snprintf(sql, sizeof(sql), "select pname, mac, apmac, groupid, groupcode, mark, lrssi, urssi, starttime, endtime, flag from ncsrvmaclog where starttime >= %lu and endtime <= %lu", lStartTime, lTime);
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (pname like '%%%s%%' or mac like '%s%%' or apmac like '%s%%') ", _keyword, _keyword, _keyword);
	 }
	 if(!utStrIsSpaces(pname))  // �û���
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and pname like '%%%s%%'", pname);
	 }
	 if(!utStrIsSpaces(mac))  // MAC
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", mac);
	 }
	 if(!utStrIsSpaces(apmac))  // IP
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and apmac like '%%%s%%'", apmac);
	 }
	 strcpy(sql, ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caPname)-1,      caPname,
															 UT_TYPE_STRING, sizeof(caMac)-1,        caMac,
															 UT_TYPE_STRING, sizeof(caApmac)-1,      caApmac,
															 UT_TYPE_ULONG,  sizeof(long),           &lGroupid,
															 UT_TYPE_STRING, sizeof(caGroupcode)-1,  caGroupcode,
															 UT_TYPE_STRING, sizeof(caMark)-1,       caMark,
															 UT_TYPE_ULONG,  sizeof(long),           &lLrssi,
															 UT_TYPE_ULONG,  sizeof(long),           &lUrssi,
															 UT_TYPE_ULONG,  sizeof(long),           &lStarttime,
															 UT_TYPE_ULONG,  sizeof(long),           &lEndtime,
															 UT_TYPE_ULONG,  sizeof(long),           &lFlag);
			
	 while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
		   if(lStarttime>0){
		   		strcpy(s_starttime, utTimFormat("%Y/%m/%d %H:%M:%S", lStarttime));
		   }else{
		   		strcpy(s_starttime, "");	
		   }
		   if(lEndtime>0){
		   		strcpy(s_endtime, utTimFormat("%Y/%m/%d %H:%M:%S", lEndtime));
		   }else{
		   		strcpy(s_endtime, "");	
		   }
		   
		   if(lGroupid>0){
	  	 		strcpy(caGroupname, (char *)ncmGetDispNameByGid(psShmHead,lGroupid));
	  	 }
	  	 
			 utPltPutLoopVarF(psDbHead,"lrssi",iNum,"%d",lLrssi);
			 utPltPutLoopVarF(psDbHead,"starttime",iNum,s_starttime);
			 utPltPutLoopVar(psDbHead,"endtime",iNum,s_endtime);
			 utPltPutLoopVarF(psDbHead,"urssi",iNum,"%d",lUrssi);
			 utPltPutLoopVarF(psDbHead,"flag",iNum,"%lu",lFlag);
			 utPltSetCvtHtml(1);
			 utPltPutLoopVar(psDbHead,"pname",iNum,caPname);
			 utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
			 utPltPutLoopVar(psDbHead,"apmac",iNum,caApmac);
			 utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
			 utPltPutLoopVar(psDbHead,"groupcode",iNum,caGroupcode);
			 utPltPutLoopVar(psDbHead,"mark",iNum,caMark);
			 utPltSetCvtHtml(0);
			 
			 memset(caPname, 0, sizeof(caPname));
			 memset(caMac, 0, sizeof(caMac));
			 memset(caApmac, 0, sizeof(caApmac));
			 lGroupid = 0;
			 memset(caGroupname, 0, sizeof(caGroupname));
			 memset(caGroupcode, 0, sizeof(caGroupcode));
			 memset(caMark, 0, sizeof(caMark));
			 lLrssi = 0;
			 lUrssi = 0;
			 lStarttime = 0;
			 lEndtime = 0;
			 lFlag = 0;
			 memset(s_starttime, 0, sizeof(s_starttime));
			 memset(s_endtime, 0, sizeof(s_endtime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caPname)-1,      caPname,
																	 UT_TYPE_STRING, sizeof(caMac)-1,        caMac,
																	 UT_TYPE_STRING, sizeof(caApmac)-1,      caApmac,
																	 UT_TYPE_ULONG,  sizeof(long),           &lGroupid,
																	 UT_TYPE_STRING, sizeof(caGroupcode)-1,  caGroupcode,
																	 UT_TYPE_STRING, sizeof(caMark)-1,       caMark,
																	 UT_TYPE_ULONG,  sizeof(long),           &lLrssi,
																	 UT_TYPE_ULONG,  sizeof(long),           &lUrssi,
																	 UT_TYPE_ULONG,  sizeof(long),           &lStarttime,
																	 UT_TYPE_ULONG,  sizeof(long),           &lEndtime,
																	 UT_TYPE_ULONG,  sizeof(long),           &lFlag);
	 }
	 //utPltShowDb(psDbHead);
	 pasDbCloseCursor(psCur);
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_maclog_list.htm");
	 return 0;	
}

