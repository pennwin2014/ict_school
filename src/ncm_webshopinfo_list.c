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
#include <iconv.h>

#define SHOPIMGPATH "/home/ncmysql/ncsrv/html"
#define UPLOADIMGPATH "/home/ncmysql/ncsrv/upload"
#define  PAS_SRCFILE    9001
static int ncmWebReplace00(char *pHead,char *pVar,short nType,long lLen,char *pValue)
{
    utPltDbHead *psDbHead;
    psDbHead = (utPltDbHead *)pHead;
    /*��ģ�����ݻ��������������
			����ԭ��
			#include ��utoplt01.h��
			int utPltPutVar(utPltDbHead *psDbHead,char *pVarName,char *pValue);
			����˵��
					psDbHead----ģ�����ݻ�����
			����pVarName----������
			����pValue------������ֵ
			����ֵ
			����0 ---- ��ȷ
			����-1-----����
    */
    utPltPutVar(psDbHead,pVar,pValue);
    return 0;
}

// ncsWebReplace: ��̬ҳ��ת���ɶ�̬ҳ��
int ncmWebReplace(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead;
    char caPlate[64];
    int iReturn;
    psDbHead = utPltInitDb();
   	iReturn = utMsgGetSomeNVar(psMsgHead, 1,
					"plate",  UT_TYPE_STRING, 63, caPlate);
    if(utStrIsSpaces(caPlate)) {
        return 0;
    }
    /*�����������ݰ��������ݰ��е����ݽ���ָ���Ĵ���
	��������ԭ��
	������	int utMsgAllRecord(utMsgHead *psMsgHead,char *pHead,int utMsgDoRecord(char *pVar,short nType,long lLen,char *Value));
	��������˵��
		������psMsgHead ----��Ϣ�ľ�����ɺ��� utMsgInit������
		������pHead ----------���ڴ��ⲿ�������ݵ�����utMsgDoRecord��ָ�롣
		������utMsgDoRecord----��ÿһ����¼���в����ĺ������ú��������û�ָ����Ҫ��ĵ��÷�ʽ���£�
		������int utMsgDoRecord(char *pHead,char *pVar,short nType,long lLen,char *Value);
		����������:
							pHead --- ͬ�����pHead
		������    pVar ---- ���ݰ��еı���
		    			nType---- ��������
		    			lLen ----- ���ݵ��ֽ���
		    			Value ---- ���ݵ�����
		 			������ֵ�� 0 ʱ����������Ϊ�� 0, ϵͳ���˳����������������ط���ֵ��
	��	����ֵ
		������0----��������
		��������=0 Ϊ�������ķ���ֵ��
    */
    utMsgAllRecord(psMsgHead,(char *)psDbHead,ncmWebReplace00);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
    return 0;
}

int ncTimeToLong(char *stime, unsigned long *lTime)
{
	 char ctime[20] = "";
	 
	 snprintf(ctime, sizeof(ctime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", stime[0], stime[1], stime[2], stime[3], stime[5], stime[6], stime[8], stime[9], stime[11], stime[12], stime[14], stime[15], stime[17], stime[18]);	
	 *lTime = utTimStrToLong("%Y%m%d%H%M%S", ctime);
	 
	 return 0;
}

int ncNumToTime(unsigned long lTime, char *cTime);
/*{
      char day[11] = ""; 
      char hour[11] = "";
      char min[11] = "";
      char secd[11] = "";
      snprintf(day, sizeof(day), "%lu", lTime/86400);
      snprintf(hour, sizeof(hour), "%lu", (lTime%86400)/3600);
      snprintf(min, sizeof(min), "%lu", ((lTime%86400)%3600)/60);
      snprintf(secd, sizeof(secd), "%lu", (((lTime%86400)%3600)%60)/60);
  
      strcat(cTime, strcat(day, "��"));
      strcat(hour, "Сʱ");
      strcat(min, "��");
      strcat(secd, "��");
      strcat(cTime, hour);
      strcat(cTime, min);
      strcat(cTime, secd); 
       
      return 0;  
}*/

// ¥����Ϣ
int ncm_webreadfloor_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char caFlags[32] = "";
	 char caPlateid[32] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lFid = 0;
	 unsigned long	lGroupid = 0;
	 char 					caName[32] = "";
	 char 					caImgpath[128] = "";
	 unsigned long  lPlateid = 0;
	 unsigned long	lFlags = 0;
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 2, "plateid",UT_TYPE_STRING, sizeof(caPlateid)-1,  caPlateid,
	 																"flags",  UT_TYPE_STRING, sizeof(caFlags)-1,    caFlags);
	 
	 sprintf(sql, "select fid, groupid, name, imgpath, flags from ncmmall where 1=1");
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and flags=%lu ", atol(caFlags));
	 if(!utStrIsSpaces(caPlateid)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu", atol(caPlateid));
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lFid,
													     UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
													     UT_TYPE_STRING, sizeof(caName)-1,      caName,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
													     UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags);	
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"fid",iNum,"%lu",lFid);
			 utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVarF(psDbHead,"plateid",iNum,"%lu",lPlateid);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltSetCvtHtml(0);
			 lFid = 0;
			 lGroupid = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lPlateid = 0;
	     lFlags = 0;
	     iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lFid,
															     UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
															     UT_TYPE_STRING, sizeof(caName)-1,      caName,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
															     UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags);	
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webmall_list.htm");
	  return 0;	
}

// ������Ϣ
int ncm_webarea_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char flags[32] = "";
	 char city[32] = "";
	 char province[32] = "";
	 char start[32] = "";
	 char limit[32] = "";
	 
	 unsigned long lCount1 = 0;
	 int iLimit = 0;
	 int iStart = 0;
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lAid = 0;
	 char 					caAname[48] = "";
	 unsigned long	lPid = 0;
	 unsigned long  lSystime = 0;
	 char           caLon[16] = "";
	 char           caLat[16] = "";
	 char           caDescription[128] = "";
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 5, "flags",     UT_TYPE_STRING, sizeof(flags)-1,     flags,
	                                "city",      UT_TYPE_STRING, sizeof(city)-1,      city,
	                                "province",  UT_TYPE_STRING, sizeof(province)-1,  province,
	                                "start",     UT_TYPE_STRING, sizeof(start)-1,     start,
														   	  "limit",     UT_TYPE_STRING, sizeof(limit)-1,     limit);
	 
	 // ��ȡ�����ļ�������ʱ��
	 char cnfProvince[32] = "";
	 char *pConfig;
	 pasConfList *psConfig;
	
	 pConfig = (char *)pasGetConfigFileName();
	 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
	 if(psConfig == NULL) {
	    utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","��ȡ�����ļ�",(char *)ncLang("0427�����ļ�����"));
	    return 0;
	 }
	
	 //����û�����ã�Ĭ��null��
	 strcpy(cnfProvince,(char *)pasUtlLookConfig(psConfig,"NcmProvince","")); // pasUtlLookConfig�����һ������ΪStatisticsStim��Ĭ��ֵ
	 printf("cnfProvince = %s\n", cnfProvince);
	 pasUtlFreeConfig(psConfig);
	 
	 sprintf(sql, "select count(*) from ncmarea where 1=1");
	 if(!utStrIsSpaces(flags)){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and aid<99 and aname='%s'", cnfProvince);
	 }
	 if(!utStrIsSpaces(province)){
	 	  snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and pid=%lu", atol(province));
	 }
	 if(!utStrIsSpaces(city)){
	 	  snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and pid=%lu", atol(city));
	 }
	 printf("sql = %s\n", sql);
	 pasDbOneRecord(sql,0,UT_TYPE_LONG,4,&lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 sprintf(sql, "select aid, aname, pid, systime, lon, lat, description from ncmarea where 1=1");
	 if(!utStrIsSpaces(flags)){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and aid<99 and aname='%s'", cnfProvince);
	 }
	 if(!utStrIsSpaces(province)){
	 	  snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and pid=%lu", atol(province));
	 }
	 if(!utStrIsSpaces(city)){
	 	  snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and pid=%lu", atol(city));
	 }
	 if(!utStrIsSpaces(flags) || !utStrIsSpaces(province) || !utStrIsSpaces(city)){
	 		sprintf(sql+strlen(sql)," limit %d , %d ",iStart, iLimit < lCount1 ? iLimit : lCount1);
	 }else{
	 		sprintf(sql+strlen(sql)," limit 20 ");	
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lAid,
													     UT_TYPE_STRING, sizeof(caAname)-1,      caAname,
													     UT_TYPE_ULONG,  sizeof(long),           &lPid,
													     UT_TYPE_ULONG,  sizeof(long),           &lSystime,
													     UT_TYPE_STRING, sizeof(caLon)-1,        caLon,
													     UT_TYPE_STRING, sizeof(caLat)-1,        caLat,
													     UT_TYPE_STRING, sizeof(caDescription)-1,caDescription);	
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"aid",iNum,"%lu",lAid);
			 utPltPutLoopVar(psDbHead,"aname",iNum,caAname);
			 utPltPutLoopVarF(psDbHead,"pid",iNum,"%lu",lPid);
			 utPltPutLoopVarF(psDbHead,"systime",iNum,"%lu",lSystime);
			 utPltPutLoopVar(psDbHead,"lon",iNum,caLon);
			 utPltPutLoopVar(psDbHead,"lat",iNum,caLat);
			 utPltPutLoopVar(psDbHead,"description",iNum,caDescription);
			 utPltSetCvtHtml(0);
			 lAid = 0;
			 memset(caAname, 0, sizeof(caAname));
			 lPid = 0;
			 lSystime = 0;
			 memset(caLon, 0, sizeof(caLon));
			 memset(caLat, 0, sizeof(caLat));
			 memset(caDescription, 0, sizeof(caDescription));
	     iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lAid,
															     UT_TYPE_STRING, sizeof(caAname)-1,      caAname,
															     UT_TYPE_ULONG,  sizeof(long),           &lPid,
															     UT_TYPE_ULONG,  sizeof(long),           &lSystime,
															     UT_TYPE_STRING, sizeof(caLon)-1,        caLon,
															     UT_TYPE_STRING, sizeof(caLat)-1,        caLat,
															     UT_TYPE_STRING, sizeof(caDescription)-1,caDescription);		
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webarea_list.htm");
	  return 0;	
}

// ��Ʒ����
int ncm_webcomgroup_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char groupid[20] = "";
	 char name[32] = "";
	 char descr[128] = "";
	 char systime[32] = "";
	 char imgpath[128] = "";
	 char flags[32] = "";
	 char plateid[20] = "";
	 char temp_file[128] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lGroupid = 0;
	 char 					caName[32] = "";
	 char 					caDescr[128] = "";
	 unsigned long	lSystime = 0;
	 char 					caImgpath[128] = "";
	 unsigned long	lFlags = 0;
	 unsigned long	lPlateid = 0;
	 char           s_systime[32] = "";
	 
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
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 16,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"groupid",     UT_TYPE_STRING,  sizeof(groupid)-1,        groupid,
																	"name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
																	"descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
																	"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				 UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"caExport",    UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 
	 trim(_keyword);
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,32);
			strcpy(name,caTemp);
   }
	 if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,128);
			strcpy(descr,caTemp);
   }
   if(strlen(systime)>0){
			pasCvtGBK(2,systime,caTemp,32);
			strcpy(systime,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,128);
			strcpy(imgpath,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid));
			strcpy(caSesid,caTemp);
   }
   
   // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"groupid");
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcomgroup where groupid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
				      return 0;	
					}
					
    		  // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					}
					pasDbExecSqlF("delete from ncmcomgroup where groupid in(%s)",caSesid);
    	}
	 }
	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmcomgroup where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
//	 if(!utStrIsSpaces(plateid)) // ���ݹؼ��ֲ�ѯ
//	 {
//			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu", atol(plateid));
//	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select groupid, name, descr, systime, imgpath, flags from ncmcomgroup where 1=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ���ݹؼ��ֲ�ѯ
	 {
//			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu", atol(plateid));
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,
			     UT_TYPE_ULONG,  4,               		&lGroupid,
			     UT_TYPE_STRING, sizeof(caName)-1,    caName,
			     UT_TYPE_STRING, sizeof(caDescr)-1,   caDescr,
			     UT_TYPE_ULONG,  4,               		&lSystime,
					 UT_TYPE_STRING, sizeof(caImgpath)-1, caImgpath,
					 UT_TYPE_ULONG,  4,               		&lFlags);	
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
		   if(lSystime>0){
		   		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
		   }else{
		   		strcpy(s_systime, "");	
		   }
		   utPltSetCvtHtml(1);
		   utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum-1);
			 utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltSetCvtHtml(0);
			 lGroupid = 0;
			 memset(caName, 0, sizeof(caName));
	     memset(caDescr, 0, sizeof(caDescr));
	     lSystime = 0;
	     memset(caImgpath, 0, sizeof(caImgpath));
	     lFlags = 0;
	     memset(s_systime, 0, sizeof(s_systime));
	     iret = pasDbFetchInto(psCur,
			     UT_TYPE_ULONG,  4,               		&lGroupid,
			     UT_TYPE_STRING, sizeof(caName)-1,    caName,
			     UT_TYPE_STRING, sizeof(caDescr)-1,   caDescr,
			     UT_TYPE_ULONG,  4,               		&lSystime,
					 UT_TYPE_STRING, sizeof(caImgpath)-1, caImgpath,
					 UT_TYPE_ULONG,  4,               		&lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomgroup_list.htm");
	  return 0;	
}

int ncm_webcomgroup_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
//#endif  
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 char caSid[32] = "";
	 char caName[32] = "";
	 char caDescr[255] = "";
	 char caImgpath[128] = "";
	 char caKey[32] = "";
	 char caPlateid[20] = "";
	 unsigned long lPlateid=0;
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 3, "sid",     UT_TYPE_STRING, sizeof(caSid)-1,     caSid,
	                                "key",     UT_TYPE_STRING, sizeof(caKey)-1,     caKey,
	                                "plateid", UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid);
	 printf("key = %s\n", caKey);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
			 sprintf(sql, "select name, descr, imgpath, plateid from ncmcomgroup where groupid=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,sizeof(caName)-1,   caName,
		                        UT_TYPE_STRING,sizeof(caDescr)-1,  caDescr,
		                        UT_TYPE_STRING,sizeof(caImgpath)-1,caImgpath,
		                        UT_TYPE_ULONG, sizeof(long),       &lPlateid);
			  utPltSetCvtHtml(1);
			  utPltPutVar(psDbHead,"groupid",caSid);
				utPltPutVar(psDbHead,"name",caName);
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVar(psDbHead,"descr",caDescr);
		    utPltPutVarF(psDbHead,"plateid","%lu",lPlateid);
		    utPltSetCvtHtml(0);
	 }else{
	 			utPltPutVarF(psDbHead,"plateid","%lu",atol(caPlateid));	
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomgroup_dr.htm");
	 return 0;		
}

// �ϴ���Ʒ����ͼƬ
int ncm_webComGroupImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32]="";
   	char caGroupid[16]="";
    char caName[32]="";
    char caSystime[32]="";
    char caDescr[128]="";
    char caImgPath[128]="";
    char caPlateid[20]="";
    char caDispImgPath[128]="";
    char imgname[128]="";
    
    char caImgpath1[128]="";
    char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
 		char caTemp2[1024]="";
 		unsigned long lSysTime=0;
 		unsigned long lMaxid = 0;
 		char *p;
 		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
		
		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",(char *)ncLang("0194����"),(char *)ncLang("0183���ļ�����"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// ����ͼƬ���·��
  	sprintf(caImgPath,"%s/images/ShopImages/groupimages", SHOPIMGPATH);
  	
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        	  printf("����ͼƬ���·������\n");
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);
    
    // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	  utMsgGetSomeNVar(psMsgHead, 6, "key",      UT_TYPE_STRING,  sizeof(caKey)-1,            caKey,
	                               "groupid",    UT_TYPE_STRING,  sizeof(caGroupid)-1,        caGroupid,
	  														 "name",       UT_TYPE_STRING,  sizeof(caName)-1,           caName,
																 "fname",      UT_TYPE_STRING,  sizeof(imp_file)-1,         imp_file,
																 "descr",      UT_TYPE_STRING,  sizeof(caDescr)-1,          caDescr,
																 "plateid",    UT_TYPE_STRING,  sizeof(caPlateid)-1,        caPlateid);
	  
	  if(strlen(caName)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","��Ʒ���Ʋ���Ϊ��!","��Ʒ���Ʋ���Ϊ��!");
        return 0;	
	  }
	                  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*��ָ�����ַ�����ȡһ����
		  	
		  	  ����ԭ�ͣ�
			����char *utStrGetWord(char *p,char *pOut,
			����                   short nMaxLen,char *pTerm);
			����
					����˵��:
			����p �C �����ַ���
			����pOut --- ����ַ���������Ҫ���ȷ���ÿռ�
			����nMaxLen --- ����ַ�������󳤶�
			����pTerm  ----- ��ֹ�������ַ�����ϵͳ��ɨ����ʱ�������ַ���ĳһ���ַ�ʱ������
			����
			��������ֵ��
			���������ַ���ָ�롣*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
				printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
					  memset(caDispImgPath, 0, sizeof(caDispImgPath));
				}else{
						sprintf(imgname, "group_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/groupimages/");
				    strcat(caDispImgPath, imgname);	
				}
	  }
	  
	  if(strcmp(caKey, "update")==0){
	  	  sprintf(caTemp2, "select imgpath from ncmcomgroup where groupid=%lu", atol(caGroupid));
	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  if(strlen(caDispImgPath)==0){ // �Ƿ�ѡ���ϴ�ͼƬ, ��ԭͼƬ����
	  	  	  sprintf(caTemp,"update ncmcomgroup set name='%s',descr='%s',systime=%lu,imgpath='%s',plateid=%lu where groupid=%lu",caName,caDescr,time(0),caImgpath1,atol(caPlateid),atol(caGroupid));
			 			printf("caTemp = %s\n", caTemp);
			 			pasDbExecSqlF(caTemp);
			 			utPltPutVar(psDbHead,"imgpath",caImgpath1);
	  	  }else{ // �ǣ�ԭͼƬɾ������ͼƬ����
			  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
				  	system(temp3_file);
			  	  
			  	  sprintf(caTemp,"update ncmcomgroup set name='%s',descr='%s',systime=%lu,imgpath='%s',plateid=%lu where groupid=%lu",caName,caDescr,time(0),caDispImgPath,atol(caPlateid),atol(caGroupid));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
			  }
	  }else if(strcmp(caKey, "add")==0){
	  	  pasDbOneRecord("select max(groupid) from ncmcomgroup", 0, UT_TYPE_ULONG, 4, &lMaxid);
		    sprintf(caTemp,"insert into ncmcomgroup (groupid,name,descr,systime,imgpath,plateid) values (%lu,'%s','%s',%lu,'%s','%lu')",(lMaxid+1),caName,caDescr,time(0),caDispImgPath,atol(caPlateid));
			 	printf("caTemp = %s\n", caTemp);
			 	pasDbExecSqlF(caTemp);
			 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
	  }
	  utPltPutVar(psDbHead,"mesg","���ݼ��سɹ�");
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomgroup_dr_list.htm");
	  //utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","���ݼ��سɹ�!","���ݼ��سɹ�!");
    return 0;
}

// ��ƷС����Ϣ
int ncm_webcomtype_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char sid[20] = "";
	 char name[32] = "";
	 char groupname[32] = "";
	 char descr[128] = "";
	 char systime[32] = "";
	 char imgpath[128] = "";
	 char flags[32] = "";
	 char plateid[20] = "";
	 char temp_file[128] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lSid = 0;
	 char 					caName[32] = "";
	 char 					caGroupname[32] = "";
	 char 					caDescr[128] = "";
	 unsigned long	lSystime = 0;
	 char 					caImgpath[128] = "";
	 unsigned long	lFlags = 0;
	 unsigned long	lPlateid = 0;
	 char           s_systime[32] = "";
	 
	 char caUpdate[16] = "";
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
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 17,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
																	"name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
																	"groupname",   UT_TYPE_STRING,  sizeof(groupname)-1,      groupname,
																	"descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
																	"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"update",      UT_TYPE_STRING,  10,                       caUpdate,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport);
	 
	 trim(_keyword);
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,32);
			strcpy(name,caTemp);
   }
   if(strlen(groupname)>0){
			pasCvtGBK(2,groupname,caTemp,32);
			strcpy(groupname,caTemp);
   }
	 if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,128);
			strcpy(descr,caTemp);
   }
   if(strlen(systime)>0){
			pasCvtGBK(2,systime,caTemp,32);
			strcpy(systime,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,128);
			strcpy(imgpath,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid));
			strcpy(caSesid,caTemp);
   }
   
   // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcomtype where sid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
				      return 0;	
					}
					
    		  // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					}
					pasDbExecSqlF("delete from ncmcomtype where sid in(%s)",caSesid);
    	}
	 }
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmcomtype where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
//			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "Select t.sid, t.name, g.name, t.descr, t.systime, t.imgpath, t.flags from ncmcomtype as t left join ncmcomgroup as g on t.groupid = g.groupid where 1=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (t.name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
//			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and t.plateid = %lu ", atol(plateid));
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by t.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,
			     UT_TYPE_ULONG,  4,               		 &lSid,
			     UT_TYPE_STRING, sizeof(caName)-1,     caName,
			     UT_TYPE_STRING, sizeof(caGroupname)-1,caGroupname,
			     UT_TYPE_STRING, sizeof(caDescr)-1,    caDescr,
			     UT_TYPE_ULONG,  4,               		 &lSystime,
					 UT_TYPE_STRING, sizeof(caImgpath)-1,  caImgpath,
					 UT_TYPE_ULONG,  4,               		 &lFlags);		
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
		   
		   if(lSystime>0){
		   	  strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
		   }else{
		   	  strcpy(s_systime, "");
		   }
		   utPltSetCvtHtml(1);
		   utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum-1);
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
			 utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltSetCvtHtml(0);
			 lSid = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caGroupname, 0, sizeof(caGroupname));
			 memset(caDescr, 0, sizeof(caDescr));
	     lSystime = 0;
	     memset(caImgpath, 0, sizeof(caImgpath));
	     lFlags = 0;
	     memset(s_systime, 0, sizeof(s_systime));
	     iret = pasDbFetchInto(psCur,
			     UT_TYPE_ULONG,  4,               		 &lSid,
			     UT_TYPE_STRING, sizeof(caName)-1,     caName,
			     UT_TYPE_STRING, sizeof(caGroupname)-1,caGroupname,
			     UT_TYPE_STRING, sizeof(caDescr)-1,    caDescr,
			     UT_TYPE_ULONG,  4,               		 &lSystime,
					 UT_TYPE_STRING, sizeof(caImgpath)-1,  caImgpath,
					 UT_TYPE_ULONG,  4,               		 &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomtype_list.htm");
	  return 0;	
}

int ncm_webcomtype_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
//#endif  
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 char caKey[32] = "";
	 char caSid[32] = "";
	 char caName[32] = "";
	 unsigned long	lGroupid = 0;
	 char caDescr[255] = "";
	 char caImgpath[128] = "";
	 char caPlateid[20] = "";
	 unsigned long lPlateid = 0;
	 
	 char caGroupTemp[4096] = "";
	 char caGroupname[32] = "";
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 3, "key",     UT_TYPE_STRING, sizeof(caKey)-1,     caKey,
	                                "sid",     UT_TYPE_STRING, sizeof(caSid)-1,     caSid,
	                                "plateid", UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
			 sprintf(sql, "select name, groupid, descr, imgpath, plateid from ncmcomtype where sid=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,sizeof(caName)-1,   caName,
			 											UT_TYPE_ULONG, sizeof(long),       &lGroupid,
		                        UT_TYPE_STRING,sizeof(caDescr)-1,  caDescr,
		                        UT_TYPE_STRING,sizeof(caImgpath)-1,caImgpath,
		                        UT_TYPE_ULONG, sizeof(long),       &lPlateid);
			  utPltSetCvtHtml(1);
			  utPltPutVar(psDbHead,"sid",caSid);
				utPltPutVar(psDbHead,"name",caName);
				utPltPutVarF(psDbHead,"groupid","%lu",lGroupid);
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVar(psDbHead,"descr",caDescr);
		    utPltPutVarF(psDbHead,"plateid","%lu",lPlateid);
		    utPltSetCvtHtml(0);
	 }else{
	 			utPltPutVarF(psDbHead,"plateid","%lu",atol(caPlateid));
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomtype_dr.htm");
	 return 0;		
}

// �ϴ���ƷС��ͼƬ
int ncm_webComTypeImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32]="";
   	char caSid[16]="";
    char caName[32]="";
    char caGroupid[32]="";
    char caDescr[128]="";
    char caSystime[32]="";
    char caImgPath[128]="";
    char caPlateid[20]="";
    char caDispImgPath[128]="";
    char imgname[128]="";
    
    char caImgpath1[128]="";
    char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
 		char caTemp2[1024]="";
 		unsigned long lSysTime=0;
 		unsigned long lMaxid = 0;
 		char *p;
 		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
		
		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",(char *)ncLang("0194����"),(char *)ncLang("0183���ļ�����"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// ����ͼƬ���·��
  	sprintf(caImgPath,"%s/images/ShopImages/typeimages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);
        
	  utMsgGetSomeNVar(psMsgHead,7,"key",        UT_TYPE_STRING,  sizeof(caKey)-1,            caKey,
	                               "sid",        UT_TYPE_STRING,  sizeof(caSid)-1,            caSid,
	  														 "name",       UT_TYPE_STRING,  sizeof(caName)-1,           caName,
	  														 "grid",       UT_TYPE_STRING,  sizeof(caGroupid)-1,        caGroupid,
																 "fname",      UT_TYPE_STRING,  sizeof(imp_file)-1,         imp_file,
																 "descr",      UT_TYPE_STRING,  sizeof(caDescr)-1,          caDescr,
																 "plateid",    UT_TYPE_STRING,  sizeof(caPlateid)-1,          caPlateid);
	  
	  if(strlen(caName)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","��ƷС�����Ʋ���Ϊ��!","��ƷС�����Ʋ���Ϊ��!");
        return 0;	
	  }
	                  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*��ָ�����ַ�����ȡһ����
		  	
		  	  ����ԭ�ͣ�
			����char *utStrGetWord(char *p,char *pOut,
			����                   short nMaxLen,char *pTerm);
			����
					����˵��:
			����p �C �����ַ���
			����pOut --- ����ַ���������Ҫ���ȷ���ÿռ�
			����nMaxLen --- ����ַ�������󳤶�
			����pTerm  ----- ��ֹ�������ַ�����ϵͳ��ɨ����ʱ�������ַ���ĳһ���ַ�ʱ������
			����
			��������ֵ��
			���������ַ���ָ�롣*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
				printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
				}else{
						sprintf(imgname, "type_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/typeimages/");
				    strcat(caDispImgPath, imgname);	
				}
	  }
	  
	  if(strcmp(caKey, "update")==0){
	  	  sprintf(caTemp2, "select imgpath from ncmcomtype where sid=%lu", atol(caSid));
	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  if(strlen(caDispImgPath)==0){ // �Ƿ�ѡ���ϴ�ͼƬ, ��ԭͼƬ����
	  	  	  sprintf(caTemp,"update ncmcomtype set name='%s',groupid=%lu,descr='%s',systime=%lu,imgpath='%s',plateid=%lu where sid=%lu",caName,atol(caGroupid),caDescr,time(0),caImgpath1,atol(caPlateid),atol(caSid));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caImgpath1);
	  	  }else{ // �ǣ�ԭͼƬɾ������ͼƬ����
			  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
				  	system(temp3_file);
			  	  
			  	  sprintf(caTemp,"update ncmcomtype set name='%s',groupid=%lu,descr='%s',systime=%lu,imgpath='%s',plateid=%lu where sid=%lu",caName,atol(caGroupid),caDescr,time(0),caDispImgPath,atol(caPlateid),atol(caSid));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
			  }
	  }else if(strcmp(caKey, "add")==0){
				pasDbOneRecord("select max(sid) from ncmcomtype", 0, UT_TYPE_ULONG, 4, &lMaxid);
		    sprintf(caTemp,"insert into ncmcomtype (sid,name,groupid,descr,systime,imgpath,plateid) values (%lu,'%s',%lu,'%s',%lu,'%s','%lu')",(lMaxid+1),caName,atol(caGroupid),caDescr,time(0),caDispImgPath,atol(caPlateid));
			 	printf("caTemp = %s\n", caTemp);
			 	pasDbExecSqlF(caTemp);
			 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
	  }
	  
	  utPltPutVar(psDbHead,"mesg","���ݼ��سɹ�");
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomtype_dr_list.htm");
	  
    return 0;
}

// ̫�������Ϣ
int ncm_webcomindex5_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 int i=0;
	 typedef struct index_stat{
	 	    unsigned long inum;
	 			unsigned long sid;
			  char name[32];
			  unsigned long sgroupid;
			  char descr[128];
			  char imgpath[128];
			  char serno[32];
			  char floor[32];
			  unsigned long level;
			  unsigned long plevel;
			  unsigned long type;
			  unsigned long systime;
			  unsigned long shopid;
			  unsigned long plateid;
			  unsigned long img_long;
			  unsigned long img_width;
			  char label[128];
			  unsigned long flags;
	 }index_stat_s;
	 index_stat_s *stat_all;
	 
	 // ������Ϣ����
	 char sid[20] = "";
	 char name[32] = "";
	 char sgroupid[32] = "";
	 char descr[128] = "";
	 char imgpath[128] = "";
	 char serno[32] = "";
	 char floor[32] = "";
	 char level[20] = "";
	 char plevel[20] = "";
	 char type[20] = "";
	 char systime[32] = "";
	 char shopid[32] = "";
	 char plateid[32] = "";
	 char flags[32] = "";
	 char img_long[20] = "";
	 char img_width[20] = "";
	 char label[128] = "";
	 char mark[128] = "";
	 
	 // ��/��
	 char s_floor[32] = "";
	 unsigned long lCount = 0; // ���м�¼�ĸ���
	 unsigned long lMaxid = 0;
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lSid = 0;
	 char 					caName[32] = "";
	 unsigned long	lSgroupid = 0;
	 char 					caDescr[128] = "";
	 char 					caImagepath[128] = "";
	 char 					caSerno[32] = "";
	 char 					caFloor[32] = "";
	 unsigned long	lLevel = 0;
	 unsigned long	lPlevel = 0;
	 unsigned long	lType = 0;
	 char           caType[32] = "";
	 unsigned long	lSystime = 0;
	 unsigned long	lShopid = 0;
	 unsigned long	lPlateid = 0;
	 unsigned long	lFlags = 0;
	 unsigned long  lImg_long = 0;
	 unsigned long  lImg_width = 0;
	 char           caLabel[128] = "";
	 char           s_systime[32] = "";
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
	 char caTemp2[4096] = "";
	 char caTemp3[4096] = "";
	 char caTemp4[4096] = "";
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
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
   unsigned long lFlag = 0;
	 
	 //ͼƬ����
	 char caImgpath1[128]="";
	 char imgname[128]="";
	 char temp_file[129]="";
	 char temp2_file[129]="";
   
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 27,"keyword",     						 UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"advmanager5_sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
																	"advmanager5_name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
																	"advmanager5_tyid",        UT_TYPE_STRING,  sizeof(sgroupid)-1,       sgroupid,
																	"advmanager5_descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
																	"advmanager5_imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"advmanager5_serno",       UT_TYPE_STRING,  sizeof(serno)-1,          serno,
																	"advmanager5_floor",       UT_TYPE_STRING,  sizeof(floor)-1,          floor,
																	"advmanager5_level",       UT_TYPE_STRING,  sizeof(level)-1,          level,
																	"advmanager5_plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
																	"advmanager5_type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
																	"advmanager5_shid",        UT_TYPE_STRING,  sizeof(shopid)-1,         shopid,
																	"advmanager5_plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"advmanager5_flags",       UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"advmanager5_img_long",    UT_TYPE_STRING,  sizeof(img_long)-1,       img_long,
																	"advmanager5_img_width",   UT_TYPE_STRING,  sizeof(img_width)-1,      img_width,
																	"advmanager5_label",       UT_TYPE_STRING,  sizeof(label)-1,          label,
																	"mark",                    UT_TYPE_STRING,  sizeof(mark)-1,           mark,
																	"start",                   UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",                   UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",                    UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",                     UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",                  UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				             UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"advmanager5_update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",                UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 
	 trim(_keyword);
	 
	 // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,32);
			strcpy(name,caTemp);
   }
	 if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,128);
			strcpy(descr,caTemp);
   }
   if(strlen(serno)>0){
			pasCvtGBK(2,serno,caTemp,32);
			strcpy(serno,caTemp);
   }
   if(strlen(floor)>0){
			pasCvtGBK(2,floor,caTemp,32);
			strcpy(floor,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,255);
			strcpy(imgpath,caTemp);
   }
   if(strlen(label)>0){
			pasCvtGBK(2,label,caTemp,128);
			strcpy(label,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
	 // ����/�޸�
	 //printf("caUpdate = %s\n", caUpdate);
	 if(!utStrIsSpaces(caUpdate)){
			sprintf(caTemp2, "select floor from ncmshop where id = %lu", atol(shopid));
	    pasDbOneRecord(caTemp2,0,UT_TYPE_STRING,sizeof(s_floor)-1,s_floor);
	    //printf("s_floor = %s\n", s_floor);
printf("caTemp2=%s\n",caTemp2);			
			// ɾ��ԭλ�õ�ͼƬ
		  sprintf(caTemp3, "select imgpath from ncmcomindex where sid=%lu", atol(sid));
printf("caTemp3=%s\n",caTemp3);		  
		  
		  pasDbOneRecord(caTemp3, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
			if(atol(sid)>0){
				  if(strcmp(imgpath, caImgpath1)==0){
				  		printf("**************ԭͼƬ����*************\n");
				  		sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', level=%lu, plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu, flags=%lu, img_long=%lu, img_width=%lu, label='%s' where sid=%lu and plateid=%lu",name,atol(sgroupid),descr,imgpath,serno,s_floor,atol(level),atol(plevel),atol(type),time(0),atol(shopid),atol(plateid),atol(flags),atol(img_long),atol(img_width),label,atol(sid),atol(plateid));	
				  		printf("caTemp = %s\n", caTemp);
						 	pasDbExecSqlF(caTemp);
				  }else{
				  		printf("**************ԭͼƬ�ı�*************\n");
				  		sprintf(temp2_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
			  			system(temp2_file);
			  			
			  			sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', level=%lu, plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu, flags=%lu, img_long=%lu, img_width=%lu, label='%s' where sid=%lu and plateid=%lu",name,atol(sgroupid),descr,imgpath,serno,s_floor,atol(level),atol(plevel),atol(type),time(0),atol(shopid),atol(plateid),atol(flags),atol(img_long),atol(img_width),label,atol(sid),atol(plateid));	
				  		printf("caTemp = %s\n", caTemp);
						 	pasDbExecSqlF(caTemp);
				  }
				 	utPltPutVarF(psDbHead,"sid","%lu",atol(sid));
			}else{
  	  	  pasDbOneRecord("select max(sid) from ncmcomindex", 0, UT_TYPE_ULONG, 4, &lMaxid);
			    sprintf(caTemp,"insert into ncmcomindex (sid,name,sgroupid,descr,imgpath,serno,floor,level,plevel,type,systime,shopid,plateid,flags,img_long,img_width,label) values (%lu,'%s',%lu,'%s','%s','%s','%s',%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,'%s')",(lMaxid+1),name,atol(sgroupid),descr,imgpath,serno,s_floor,atol(level),atol(plevel),atol(type),time(0),atol(shopid),atol(plateid),atol(flags),atol(img_long),atol(img_width),label);
				 	printf("caTemp = %s\n", caTemp);
				 	pasDbExecSqlF(caTemp);	
				 	utPltPutVarF(psDbHead,"sid","%lu",(lMaxid+1));
			}	
			utPltSetCvtHtml(1);
			utPltPutVar(psDbHead,"name",name);
			utPltPutVar(psDbHead,"sgroupid",sgroupid);
			utPltPutVar(psDbHead,"descr",descr);
			utPltPutVar(psDbHead,"imgpath",imgpath);
			utPltPutVar(psDbHead,"serno",serno);
			utPltPutVar(psDbHead,"floor",s_floor);
			utPltPutVar(psDbHead,"level",level);
			utPltPutVar(psDbHead,"plevel",plevel);
			utPltPutVar(psDbHead,"type",type);
			utPltPutVar(psDbHead,"shopid",shopid);
			utPltPutVar(psDbHead,"plateid",plateid);
			utPltPutVar(psDbHead,"flags",flags);
			utPltPutVar(psDbHead,"img_long",img_long);
			utPltPutVar(psDbHead,"img_width",img_width);
			utPltPutVar(psDbHead,"label",label);
	 		utPltPutVar(psDbHead,"mesg","���ݼ��سɹ�");
	 		utPltSetCvtHtml(0);
	    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomindex_dr_list.htm");	
	    return 0;
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcomindex where sid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
				      return 0;	
					}
					
    		  // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					}
					pasDbExecSqlF("delete from ncmcomindex where sid in(%s)",caSesid);
    	}
	 }
	 
	 stat_all = (index_stat_s *)malloc(sizeof(index_stat_s)*5 + 1);
	 for(i=0; i<5; i++){
 				stat_all[i].sid = 0;
 				strcpy(stat_all[i].name, "");
 				stat_all[i].sgroupid = 0;
 				strcpy(stat_all[i].descr, "");
 				strcpy(stat_all[i].imgpath, "");
 				strcpy(stat_all[i].serno, "");
 				strcpy(stat_all[i].floor, "");
 				stat_all[i].level = i+1;
 				stat_all[i].plevel = 0;
 				stat_all[i].type = 1;
 				stat_all[i].systime = 0;
 				stat_all[i].shopid = 0;
 				stat_all[i].plateid = atol(plateid);
 				stat_all[i].flags = 0;
 				if(atol(plateid)==7){
		 				switch(i){
		 						case 0:stat_all[i].img_long = 450;stat_all[i].img_width = 607;break;
		 					  case 1:stat_all[i].img_long = 450;stat_all[i].img_width = 607;break;
		 						case 2:stat_all[i].img_long = 450;stat_all[i].img_width = 607;break;
		 						case 3:stat_all[i].img_long = 450;stat_all[i].img_width = 607;break;
		 						case 4:stat_all[i].img_long = 450;stat_all[i].img_width = 607;break;
		 						default:break;
		 				}
 			  }else if(atol(plateid)==8){
 			  		switch(i){
 			  			  case 0:stat_all[i].img_long = 117;stat_all[i].img_width = 791;break;
		 					  case 1:stat_all[i].img_long = 117;stat_all[i].img_width = 791;break;
		 						case 2:stat_all[i].img_long = 117;stat_all[i].img_width = 791;break;
		 						case 3:stat_all[i].img_long = 117;stat_all[i].img_width = 791;break;
		 						case 4:stat_all[i].img_long = 460;stat_all[i].img_width = 420;break;
		 						default:break;
		 				}	
 			  }else{
 			  		switch(i){
		 						case 0:stat_all[i].img_long = 450;stat_all[i].img_width = 607;break;
		 					  case 1:stat_all[i].img_long = 450;stat_all[i].img_width = 607;break;
		 						case 2:stat_all[i].img_long = 450;stat_all[i].img_width = 607;break;
		 						case 3:stat_all[i].img_long = 450;stat_all[i].img_width = 607;break;
		 						case 4:stat_all[i].img_long = 450;stat_all[i].img_width = 607;break;
		 						default:break;
		 				}	
 			  }
 				strcpy(stat_all[i].label, mark);
	 }
	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmcomindex where 1=1 and type=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu", atol(plateid));
	 }
	 if(!utStrIsSpaces(mark)) // ���ݱ�ǩ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and label = '%s'", mark);
	 }
	 printf("sql = %s\n", sql);
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select sid, name, sgroupid, descr, imgpath, serno, floor, level, plevel, type, systime, shopid, plateid, flags, img_long, img_width, label from ncmcomindex where 1=1 and type=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu", atol(plateid));
	 }
	 if(!utStrIsSpaces(mark)) // ���ݱ�ǩ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and label = '%s'", mark);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
													     UT_TYPE_STRING, sizeof(caName)-1,      caName,
													     UT_TYPE_ULONG,  sizeof(long),          &lSgroupid,
													     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
													     UT_TYPE_STRING, sizeof(caImagepath)-1, caImagepath,
													     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
													     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
													     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lType,
													     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
													     UT_TYPE_ULONG,  sizeof(long),          &lShopid,
													     UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags,
															 UT_TYPE_ULONG,  sizeof(long),          &lImg_long,
															 UT_TYPE_ULONG,  sizeof(long),          &lImg_width,
															 UT_TYPE_STRING, sizeof(caLabel)-1,     caLabel);	
				
	  while(0 == iret || 1405 == iret){
			 for(i=0; i<5; i++){
	  	 		if((i+1) == lLevel){
			 				stat_all[i].sid = lSid;
			 				strcpy(stat_all[i].name, caName);
			 				stat_all[i].sgroupid = lSgroupid;
			 				strcpy(stat_all[i].descr, caDescr);
			 				strcpy(stat_all[i].imgpath, caImagepath);
			 				strcpy(stat_all[i].serno, caSerno);
			 				strcpy(stat_all[i].floor, caFloor);
			 				stat_all[i].level = lLevel;
			 				stat_all[i].plevel = lPlevel;
			 				stat_all[i].type = lType;
			 				stat_all[i].systime = lSystime;
			 				stat_all[i].shopid = lShopid;
			 				stat_all[i].plateid = lPlateid;
			 				stat_all[i].flags = lFlags;
			 				stat_all[i].img_long = lImg_long;
			 				stat_all[i].img_width = lImg_width;
			 				strcpy(stat_all[i].label, caLabel);
	  	 		}	
	  	 }
			 
			 lSid = 0;
			 memset(caName, 0, sizeof(caName));
			 lSgroupid = 0;
			 memset(caDescr, 0, sizeof(caDescr));
			 memset(caImagepath, 0, sizeof(caImagepath));
			 memset(caSerno, 0, sizeof(caSerno));
			 memset(caFloor, 0, sizeof(caFloor));
	     lLevel = 0;
	     lPlevel = 0;
	     lType = 0;
	     lSystime = 0;
	     lShopid = 0;
	     lPlateid = 0;
	     lFlags = 0;
	     lImg_long = 0;
	     lImg_width = 0;
	     memset(caLabel, 0, sizeof(caLabel));
	     memset(s_systime, 0, sizeof(s_systime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
															     UT_TYPE_STRING, sizeof(caName)-1,      caName,
															     UT_TYPE_ULONG,  sizeof(long),          &lSgroupid,
															     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
															     UT_TYPE_STRING, sizeof(caImagepath)-1, caImagepath,
															     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
															     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
															     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lType,
															     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
															     UT_TYPE_ULONG,  sizeof(long),          &lShopid,
															     UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags,
																	 UT_TYPE_ULONG,  sizeof(long),          &lImg_long,
																	 UT_TYPE_ULONG,  sizeof(long),          &lImg_width,
																	 UT_TYPE_STRING, sizeof(caLabel)-1,     caLabel);	
	  }
	  pasDbCloseCursor(psCur);
		
		for(i=0; i<5; i++){
			  iNum++;
			  if(iNum > 1)
			  {
				 		utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			  }
			  
			  utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu",stat_all[i].shopid);
			  utPltSetCvtHtml(1);
		    utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
			  utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",stat_all[i].sid);
			  utPltPutLoopVar(psDbHead,"name",iNum,stat_all[i].name);
			  utPltPutLoopVarF(psDbHead,"sgroupid",iNum,"%lu",stat_all[i].sgroupid);
			  utPltPutLoopVar(psDbHead,"descr",iNum,stat_all[i].descr);
			  utPltPutLoopVar(psDbHead,"imgpath",iNum,stat_all[i].imgpath);
			  utPltPutLoopVar(psDbHead,"serno",iNum,stat_all[i].serno);
			  utPltPutLoopVar(psDbHead,"floor",iNum,stat_all[i].floor);
			  utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",stat_all[i].level);
			  utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",stat_all[i].plevel);
			  utPltPutLoopVarF(psDbHead,"type",iNum,"%lu",stat_all[i].type);
			  utPltPutLoopVar(psDbHead,"systime",iNum,utTimFormat("%Y/%m/%d %H:%M:%S", stat_all[i].systime));
			  utPltPutLoopVarF(psDbHead,"plateid",iNum,"%lu",stat_all[i].plateid);
			  utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",stat_all[i].flags);
			  utPltPutLoopVarF(psDbHead,"img_long",iNum,"%lu",stat_all[i].img_long);
			  utPltPutLoopVarF(psDbHead,"img_width",iNum,"%lu",stat_all[i].img_width);
			  utPltPutLoopVar(psDbHead,"label",iNum,stat_all[i].label);
			  utPltSetCvtHtml(0);
		}
	  utPltShowDb(psDbHead);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex5_list.htm");
	  return 0;	
}

int ncm_webcomindex5_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  
	  char caImgPath[128]="";
	  char caDispImgPath[128]="";
	  char imgname[128]="";
	  char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
	  char *p=NULL;
	  char *q=NULL;
    
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",(char *)ncLang("0194����"),(char *)ncLang("0183���ļ�����"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// ����ͼƬ���·��
  	sprintf(caImgPath,"%s/images/ShopImages/indeximages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);

	  //utMsgGetSomeNVar(psMsgHead,1,"fileField", UT_TYPE_STRING, 255, imp_file);
	  utMsgGetSomeNVar(psMsgHead,1,"image_fil", UT_TYPE_STRING, sizeof(imp_file)-1, imp_file);

	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*��ָ�����ַ�����ȡһ����
		  	
		  	  ����ԭ�ͣ�
			����char *utStrGetWord(char *p,char *pOut,
			����                   short nMaxLen,char *pTerm);
			����
					����˵��:
			����p �C �����ַ���
			����pOut --- ����ַ���������Ҫ���ȷ���ÿռ�
			����nMaxLen --- ����ַ�������󳤶�
			����pTerm  ----- ��ֹ�������ַ�����ϵͳ��ɨ����ʱ�������ַ���ĳһ���ַ�ʱ������
			����
			��������ֵ��
			���������ַ���ָ�롣*/
				p=utStrGetWord(p,temp_file,200,";\n");
		  	//printf("temp_file = %s\n", temp_file);
		  	strcpy(temp3_file, temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
				}else{
					  q = strtok(temp3_file, ".");
					  //if(q)printf("%s\n", q);
					  q = strtok(NULL, ".");
					  //if(q)printf("%s\n", q);
					  srand((unsigned int)time(0));
						sprintf(imgname, "index_%lu_%lu.%s", time(0), rand()%100, q);
						//printf("imgname = %s\n", imgname);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
printf("temp2_file = %s\n", temp2_file);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/indeximages/");
				    strcat(caDispImgPath, imgname);
				}
		}
		
	  utPltPutVar(psDbHead,"filepath",caDispImgPath);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex5_closeFile.htm");
    return 0;
}

// Ԥ԰�����Ϣ(��ԥ԰)
int ncm_webcomindex4_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 int i=0;
	 typedef struct index_stat{
	 	    unsigned long flag;
	 	    unsigned long inum;
	 			unsigned long sid;
			  char name[32];
			  unsigned long sgroupid;
			  char descr[128];
			  char imgpath[128];
			  char serno[32];
			  char floor[32];
			  unsigned long level;
			  unsigned long plevel;
			  unsigned long type;
			  unsigned long systime;
			  unsigned long shopid;
			  unsigned long plateid;
			  unsigned long img_long;
			  unsigned long img_width;
			  char label[128];
			  unsigned long flags;
	 }index_stat_s;
	 index_stat_s *stat_temp;
	 index_stat_s *stat_all;
	 
	 // ������Ϣ����
	 char sid[20] = "";
	 char name[32] = "";
	 char sgroupid[32] = "";
	 char descr[128] = "";
	 char imgpath[128] = "";
	 char serno[32] = "";
	 char floor[32] = "";
	 char level[20] = "";
	 char plevel[20] = "";
	 char type[20] = "";
	 char systime[32] = "";
	 char shopid[32] = "";
	 char plateid[32] = "";
	 char flags[32] = "";
	 char img_long[20] = "";
	 char img_width[20] = "";
	 char label[128] = "";
	 char mark[128] = "";
	 
	 // ��/��
	 char s_floor[32] = "";
	 unsigned long lCount = 0; // ���м�¼�ĸ���
	 unsigned long lMaxid = 0;
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lSid = 0;
	 char 					caName[32] = "";
	 unsigned long	lSgroupid = 0;
	 char 					caDescr[128] = "";
	 char 					caImagepath[128] = "";
	 char 					caSerno[32] = "";
	 char 					caFloor[32] = "";
	 unsigned long	lLevel = 0;
	 unsigned long	lPlevel = 0;
	 unsigned long	lType = 0;
	 char           caType[32] = "";
	 unsigned long	lSystime = 0;
	 unsigned long	lShopid = 0;
	 unsigned long	lPlateid = 0;
	 unsigned long	lFlags = 0;
	 unsigned long  lImg_long = 0;
	 unsigned long  lImg_width = 0;
	 char           caLabel[128] = "";
	 char           caScode[16] = "";
	 char           s_systime[32] = "";
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
	 char caTemp2[4096] = "";
	 char caTemp3[4096] = "";
	 char caTemp4[4096] = "";
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
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
   unsigned long lFlag = 0;
	 
	 //ͼƬ����
	 char caImgpath1[128]="";
	 char imgname[128]="";
	 char temp_file[129]="";
	 char temp2_file[129]="";
   
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 27,"keyword",     						 UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"advmanager3_sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
																	"advmanager3_name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
																	"advmanager3_tyid",        UT_TYPE_STRING,  sizeof(sgroupid)-1,       sgroupid,
																	"advmanager3_descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
																	"advmanager3_imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"advmanager3_serno",       UT_TYPE_STRING,  sizeof(serno)-1,          serno,
																	"advmanager3_floor",       UT_TYPE_STRING,  sizeof(floor)-1,          floor,
																	"advmanager3_level",       UT_TYPE_STRING,  sizeof(level)-1,          level,
																	"advmanager3_plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
																	"advmanager3_type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
																	"advmanager3_shid",        UT_TYPE_STRING,  sizeof(shopid)-1,         shopid,
																	"advmanager3_plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"advmanager3_flags",       UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"advmanager3_img_long",    UT_TYPE_STRING,  sizeof(img_long)-1,       img_long,
																	"advmanager3_img_width",   UT_TYPE_STRING,  sizeof(img_width)-1,      img_width,
																	"advmanager3_label",       UT_TYPE_STRING,  sizeof(label)-1,          label,
																	"mark",                    UT_TYPE_STRING,  sizeof(mark)-1,           mark,
																	"start",                   UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",                   UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",                    UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",                     UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",                  UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				             UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"advmanager3_update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",                UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 
	 trim(_keyword);
	 
	 // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,32);
			strcpy(name,caTemp);
   }
	 if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,128);
			strcpy(descr,caTemp);
   }
   if(strlen(serno)>0){
			pasCvtGBK(2,serno,caTemp,32);
			strcpy(serno,caTemp);
   }
   if(strlen(floor)>0){
			pasCvtGBK(2,floor,caTemp,32);
			strcpy(floor,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,255);
			strcpy(imgpath,caTemp);
   }
   if(strlen(label)>0){
			pasCvtGBK(2,label,caTemp,128);
			strcpy(label,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
	 // ����/�޸�
	 //printf("caUpdate = %s\n", caUpdate);
	 if(!utStrIsSpaces(caUpdate)){
			sprintf(caTemp2, "select floor from ncmshop where id = %lu", atol(shopid));
	    pasDbOneRecord(caTemp2,0,UT_TYPE_STRING,sizeof(s_floor)-1,s_floor);
	    //printf("s_floor = %s\n", s_floor);
			
			// ɾ��ԭλ�õ�ͼƬ
		  sprintf(caTemp3, "select imgpath from ncmcomindex where sid=%lu", atol(sid));
		  pasDbOneRecord(caTemp3, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
			if(atol(sid)>0){
				  if(strcmp(imgpath, caImgpath1)==0){
				  		//printf("**************ԭͼƬ����*************\n");
				  		sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', level=%lu, plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu, flags=%lu, img_long=%lu, img_width=%lu, label='%s' where sid=%lu and plateid=%lu",name,atol(sgroupid),descr,imgpath,serno,s_floor,atol(level),atol(plevel),atol(type),time(0),atol(shopid),atol(plateid),atol(flags),atol(img_long),atol(img_width),label,atol(sid),atol(plateid));	
				  		//printf("caTemp = %s\n", caTemp);
						 	pasDbExecSqlF(caTemp);
				  }else{
				  		//printf("**************ԭͼƬ�ı�*************\n");
				  		sprintf(temp2_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
			  			system(temp2_file);
			  			
			  			sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', level=%lu, plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu, flags=%lu, img_long=%lu, img_width=%lu, label='%s' where sid=%lu and plateid=%lu",name,atol(sgroupid),descr,imgpath,serno,s_floor,atol(level),atol(plevel),atol(type),time(0),atol(shopid),atol(plateid),atol(flags),atol(img_long),atol(img_width),label,atol(sid),atol(plateid));	
				  		//printf("caTemp = %s\n", caTemp);
						 	pasDbExecSqlF(caTemp);
				  }
				 	utPltPutVarF(psDbHead,"sid","%lu",atol(sid));
			}else{
  	  	  pasDbOneRecord("select max(sid) from ncmcomindex", 0, UT_TYPE_ULONG, 4, &lMaxid);
			    sprintf(caTemp,"insert into ncmcomindex (sid,name,sgroupid,descr,imgpath,serno,floor,level,plevel,type,systime,shopid,plateid,flags,img_long,img_width,label) values (%lu,'%s',%lu,'%s','%s','%s','%s',%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,'%s')",(lMaxid+1),name,atol(sgroupid),descr,imgpath,serno,s_floor,atol(level),atol(plevel),atol(type),time(0),atol(shopid),atol(plateid),atol(flags),atol(img_long),atol(img_width),label);
				 	//printf("caTemp = %s\n", caTemp);
				 	pasDbExecSqlF(caTemp);	
				 	utPltPutVarF(psDbHead,"sid","%lu",(lMaxid+1));
			}	
			utPltSetCvtHtml(1);
			utPltPutVar(psDbHead,"name",name);
			utPltPutVar(psDbHead,"sgroupid",sgroupid);
			utPltPutVar(psDbHead,"descr",descr);
			utPltPutVar(psDbHead,"imgpath",imgpath);
			utPltPutVar(psDbHead,"serno",serno);
			utPltPutVar(psDbHead,"floor",s_floor);
			utPltPutVar(psDbHead,"level",level);
			utPltPutVar(psDbHead,"plevel",plevel);
			utPltPutVar(psDbHead,"type",type);
			utPltPutVar(psDbHead,"shopid",shopid);
			utPltPutVar(psDbHead,"plateid",plateid);
			utPltPutVar(psDbHead,"flags",flags);
			utPltPutVar(psDbHead,"img_long",img_long);
			utPltPutVar(psDbHead,"img_width",img_width);
			utPltPutVar(psDbHead,"label",label);
	 		utPltPutVar(psDbHead,"mesg","���ݼ��سɹ�");
	 		utPltSetCvtHtml(0);
	    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomindex_dr_list.htm");	
	    return 0;
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcomindex where sid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
				      return 0;	
					}
					
    		  // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					}
					pasDbExecSqlF("delete from ncmcomindex where sid in(%s)",caSesid);
    	}
	 }
	 
	 stat_all = (index_stat_s *)malloc(sizeof(index_stat_s)*21 + 1);
	 for(i=0; i<21; i++){
 				stat_all[i].flag = 0;
 				stat_all[i].sid = 0;
 				strcpy(stat_all[i].name, "");
 				stat_all[i].sgroupid = 0;
 				strcpy(stat_all[i].descr, "");
 				strcpy(stat_all[i].imgpath, "");
 				strcpy(stat_all[i].serno, "");
 				strcpy(stat_all[i].floor, "");
 				stat_all[i].level = i+1;
 				stat_all[i].plevel = 0;
 				stat_all[i].type = 1;
 				stat_all[i].systime = 0;
 				stat_all[i].shopid = 0;
 				stat_all[i].plateid = atol(plateid);
 				stat_all[i].flags = 0;
 				if(atol(plateid)==1949796946){
 						switch(i){
		 					  case 0:stat_all[i].img_long = 173;stat_all[i].img_width = 605;break;
		 					  case 1:stat_all[i].img_long = 173;stat_all[i].img_width = 605;break;
		 					  case 2:stat_all[i].img_long = 173;stat_all[i].img_width = 605;break;
		 					  case 3:stat_all[i].img_long = 173;stat_all[i].img_width = 605;break;
		 					  case 4:stat_all[i].img_long = 173;stat_all[i].img_width = 605;break;
		 					  case 5:stat_all[i].img_long = 173;stat_all[i].img_width = 605;break;
		 					  case 6:stat_all[i].img_long = 173;stat_all[i].img_width = 605;break;
		 					  case 7:stat_all[i].img_long = 173;stat_all[i].img_width = 605;break;
		 					  case 8:stat_all[i].img_long = 173;stat_all[i].img_width = 605;break;
		 					  case 9:stat_all[i].img_long = 173;stat_all[i].img_width = 196;break;
		 					  case 10:stat_all[i].img_long = 173;stat_all[i].img_width = 196;break;
		 					  case 11:stat_all[i].img_long = 173;stat_all[i].img_width = 196;break;
		 					  case 12:stat_all[i].img_long = 357;stat_all[i].img_width = 256;break;
		 					  case 13:stat_all[i].img_long = 172;stat_all[i].img_width = 341;break;
		 					  case 14:stat_all[i].img_long = 172;stat_all[i].img_width = 341;break;
		 					  case 15:stat_all[i].img_long = 112;stat_all[i].img_width = 298;break;
		 					  case 16:stat_all[i].img_long = 112;stat_all[i].img_width = 298;break;
		 					  case 17:stat_all[i].img_long = 112;stat_all[i].img_width = 298;break;
		 					  case 18:stat_all[i].img_long = 112;stat_all[i].img_width = 298;break;
		 					  case 19:stat_all[i].img_long = 112;stat_all[i].img_width = 298;break;
		 					  case 20:stat_all[i].img_long = 112;stat_all[i].img_width = 298;break;
		 					  default:break;
		 				}
 				}else{
		 				switch(i){
		 						case 0:stat_all[i].img_long = 240;stat_all[i].img_width = 640;break;
		 						case 1:stat_all[i].img_long = 240;stat_all[i].img_width = 640;break;
		 					  case 2:stat_all[i].img_long = 240;stat_all[i].img_width = 640;break;
		 					  case 3:stat_all[i].img_long = 240;stat_all[i].img_width = 640;break;
		 					  case 4:stat_all[i].img_long = 130;stat_all[i].img_width = 640;break;
		 					  case 5:stat_all[i].img_long = 240;stat_all[i].img_width = 160;break;
		 					  case 6:stat_all[i].img_long = 240;stat_all[i].img_width = 160;break;
		 					  case 7:stat_all[i].img_long = 240;stat_all[i].img_width = 160;break;
		 					  case 8:stat_all[i].img_long = 240;stat_all[i].img_width = 160;break;
		 					  case 9:stat_all[i].img_long = 240;stat_all[i].img_width = 640;break;
		 					  case 10:stat_all[i].img_long = 100;stat_all[i].img_width = 640;break;
		 					  case 11:stat_all[i].img_long = 133;stat_all[i].img_width = 320;break;
		 					  case 12:stat_all[i].img_long = 133;stat_all[i].img_width = 320;break;
		 					  default:break;
		 				}
 			  }
 				strcpy(stat_all[i].label, mark);
	 }
	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmcomindex where 1=1 and type=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu", atol(plateid));
	 }
	 if(!utStrIsSpaces(mark)) // ���ݱ�ǩ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and label = '%s'", mark);
	 }
	 printf("sql = %s\n", sql);
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select sid, name, sgroupid, descr, imgpath, serno, floor, level, plevel, type, systime, shopid, plateid, flags, img_long, img_width, label from ncmcomindex where 1=1 and type=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu", atol(plateid));
	 }
	 if(!utStrIsSpaces(mark)) // ���ݱ�ǩ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and label = '%s'", mark);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
													     UT_TYPE_STRING, sizeof(caName)-1,      caName,
													     UT_TYPE_ULONG,  sizeof(long),          &lSgroupid,
													     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
													     UT_TYPE_STRING, sizeof(caImagepath)-1, caImagepath,
													     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
													     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
													     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lType,
													     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
													     UT_TYPE_ULONG,  sizeof(long),          &lShopid,
													     UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags,
															 UT_TYPE_ULONG,  sizeof(long),          &lImg_long,
															 UT_TYPE_ULONG,  sizeof(long),          &lImg_width,
															 UT_TYPE_STRING, sizeof(caLabel)-1,     caLabel);	
				
	  while(0 == iret || 1405 == iret){
			 for(i=0; i<21; i++){
	  	 		if((i+1) == lLevel){
	  	 				stat_all[i].flag = 0;
			 				stat_all[i].sid = lSid;
			 				strcpy(stat_all[i].name, caName);
			 				stat_all[i].sgroupid = lSgroupid;
			 				strcpy(stat_all[i].descr, caDescr);
			 				strcpy(stat_all[i].imgpath, caImagepath);
			 				strcpy(stat_all[i].serno, caSerno);
			 				strcpy(stat_all[i].floor, caFloor);
			 				stat_all[i].level = lLevel;
			 				stat_all[i].plevel = lPlevel;
			 				stat_all[i].type = lType;
			 				stat_all[i].systime = lSystime;
			 				stat_all[i].shopid = lShopid;
			 				stat_all[i].plateid = lPlateid;
			 				stat_all[i].flags = lFlags;
			 				stat_all[i].img_long = lImg_long;
			 				stat_all[i].img_width = lImg_width;
			 				strcpy(stat_all[i].label, caLabel);
	  	 		}	
	  	 }
			 
			 lSid = 0;
			 memset(caName, 0, sizeof(caName));
			 lSgroupid = 0;
			 memset(caDescr, 0, sizeof(caDescr));
			 memset(caImagepath, 0, sizeof(caImagepath));
			 memset(caSerno, 0, sizeof(caSerno));
			 memset(caFloor, 0, sizeof(caFloor));
	     lLevel = 0;
	     lPlevel = 0;
	     lType = 0;
	     lSystime = 0;
	     lShopid = 0;
	     lPlateid = 0;
	     lFlags = 0;
	     lImg_long = 0;
	     lImg_width = 0;
	     memset(caLabel, 0, sizeof(caLabel));
	     memset(s_systime, 0, sizeof(s_systime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
															     UT_TYPE_STRING, sizeof(caName)-1,      caName,
															     UT_TYPE_ULONG,  sizeof(long),          &lSgroupid,
															     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
															     UT_TYPE_STRING, sizeof(caImagepath)-1, caImagepath,
															     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
															     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
															     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lType,
															     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
															     UT_TYPE_ULONG,  sizeof(long),          &lShopid,
															     UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags,
																	 UT_TYPE_ULONG,  sizeof(long),          &lImg_long,
																	 UT_TYPE_ULONG,  sizeof(long),          &lImg_width,
																	 UT_TYPE_STRING, sizeof(caLabel)-1,     caLabel);	
	  }
	  pasDbCloseCursor(psCur);
	  
		for(i=0; i<21; i++){
			  iNum++;
			  if(iNum > 1)
			  {
				 		utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			  }
			  
			  utPltPutLoopVarF(psDbHead,"flag",iNum,"%lu",0);
			  utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu",stat_all[i].shopid);
			  utPltSetCvtHtml(1);
		    utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
			  utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",stat_all[i].sid);
			  utPltPutLoopVar(psDbHead,"name",iNum,stat_all[i].name);
			  utPltPutLoopVarF(psDbHead,"sgroupid",iNum,"%lu",stat_all[i].sgroupid);
			  utPltPutLoopVar(psDbHead,"descr",iNum,stat_all[i].descr);
			  utPltPutLoopVar(psDbHead,"imgpath",iNum,stat_all[i].imgpath);
			  utPltPutLoopVar(psDbHead,"serno",iNum,stat_all[i].serno);
			  utPltPutLoopVar(psDbHead,"floor",iNum,stat_all[i].floor);
			  utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",stat_all[i].level);
			  utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",stat_all[i].plevel);
			  utPltPutLoopVarF(psDbHead,"type",iNum,"%lu",stat_all[i].type);
			  utPltPutLoopVar(psDbHead,"systime",iNum,utTimFormat("%Y/%m/%d %H:%M:%S", stat_all[i].systime));
			  utPltPutLoopVarF(psDbHead,"plateid",iNum,"%lu",stat_all[i].plateid);
			  utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",stat_all[i].flags);
			  utPltPutLoopVarF(psDbHead,"img_long",iNum,"%lu",stat_all[i].img_long);
			  utPltPutLoopVarF(psDbHead,"img_width",iNum,"%lu",stat_all[i].img_width);
			  utPltPutLoopVar(psDbHead,"label",iNum,stat_all[i].label);	
			  utPltSetCvtHtml(0);
		}
	  //utPltShowDb(psDbHead);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex4_list.htm");
	  return 0;	
}

int ncm_webcomindex4_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  
	  char caImgPath[128]="";
	  char caDispImgPath[128]="";
	  char imgname[128]="";
	  char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
	  char *p=NULL;
	  char *q=NULL;
    
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",(char *)ncLang("0194����"),(char *)ncLang("0183���ļ�����"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// ����ͼƬ���·��
  	sprintf(caImgPath,"%s/images/ShopImages/indeximages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);

	  //utMsgGetSomeNVar(psMsgHead,1,"fileField", UT_TYPE_STRING, 255, imp_file);
	  utMsgGetSomeNVar(psMsgHead,1,"image_fil", UT_TYPE_STRING, sizeof(imp_file)-1, imp_file);

	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*��ָ�����ַ�����ȡһ����
		  	
		  	  ����ԭ�ͣ�
			����char *utStrGetWord(char *p,char *pOut,
			����                   short nMaxLen,char *pTerm);
			����
					����˵��:
			����p �C �����ַ���
			����pOut --- ����ַ���������Ҫ���ȷ���ÿռ�
			����nMaxLen --- ����ַ�������󳤶�
			����pTerm  ----- ��ֹ�������ַ�����ϵͳ��ɨ����ʱ�������ַ���ĳһ���ַ�ʱ������
			����
			��������ֵ��
			���������ַ���ָ�롣*/
				p=utStrGetWord(p,temp_file,200,";\n");
		  	//printf("temp_file = %s\n", temp_file);
		  	strcpy(temp3_file, temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
				}else{
					  q = strtok(temp3_file, ".");
					  //if(q)printf("%s\n", q);
					  q = strtok(NULL, ".");
					  //if(q)printf("%s\n", q);
					  srand((unsigned int)time(0));
						sprintf(imgname, "index_%lu_%lu.%s", time(0), rand()%100, q);
						//printf("imgname = %s\n", imgname);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
						//printf("temp2_file = %s\n", temp2_file);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/indeximages/");
				    strcat(caDispImgPath, imgname);
				}
		}
		
	  utPltPutVar(psDbHead,"filepath",caDispImgPath);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex4_closeFile.htm");
    return 0;
}

// ���ڶ�����Ϣ(���̳�)
int ncm_webcomindex3_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 int i=0;
	 typedef struct index_stat{
	 	    unsigned long flag;
	 	    unsigned long inum;
	 			unsigned long sid;
			  char name[32];
			  unsigned long sgroupid;
			  char descr[128];
			  char imgpath[128];
			  char serno[32];
			  char floor[32];
			  unsigned long level;
			  unsigned long plevel;
			  unsigned long type;
			  unsigned long systime;
			  unsigned long shopid;
			  unsigned long plateid;
			  unsigned long img_long;
			  unsigned long img_width;
			  unsigned long flags;
			  char scode[16];
	 }index_stat_s;
	 index_stat_s *stat_temp;
	 index_stat_s *stat_all;
	 
	 // ������Ϣ����
	 char sid[20] = "";
	 char name[32] = "";
	 char sgroupid[32] = "";
	 char descr[128] = "";
	 char imgpath[128] = "";
	 char serno[32] = "";
	 char floor[32] = "";
	 char level[20] = "";
	 char plevel[20] = "";
	 char type[20] = "";
	 char systime[32] = "";
	 char shopid[32] = "";
	 char plateid[32] = "";
	 char flags[32] = "";
	 char img_long[20] = "";
	 char img_width[20] = "";
	 
	 // ��/��
	 char s_floor[32] = "";
	 unsigned long lCount = 0; // ���м�¼�ĸ���
	 unsigned long lMaxid = 0;
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lSid = 0;
	 char 					caName[32] = "";
	 unsigned long	lSgroupid = 0;
	 char 					caDescr[128] = "";
	 char 					caImagepath[128] = "";
	 char 					caSerno[32] = "";
	 char 					caFloor[32] = "";
	 unsigned long	lLevel = 0;
	 unsigned long	lPlevel = 0;
	 unsigned long	lType = 0;
	 char           caType[32] = "";
	 unsigned long	lSystime = 0;
	 unsigned long	lShopid = 0;
	 unsigned long	lPlateid = 0;
	 unsigned long	lFlags = 0;
	 unsigned long  lImg_long = 0;
	 unsigned long  lImg_width = 0;
	 char           caScode[16] = "";
	 char           s_systime[32] = "";
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
	 char caTemp2[4096] = "";
	 char caTemp3[4096] = "";
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
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
   unsigned long lFlag = 0;
	 
	 //ͼƬ����
	 char caImgpath1[128]="";
	 char imgname[128]="";
	 char temp_file[129]="";
	 char temp2_file[129]="";
   char caShopids[1024];
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 26,"keyword",     						 UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"advmanager2_sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
																	"advmanager2_name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
																	"advmanager2_tyid",        UT_TYPE_STRING,  sizeof(sgroupid)-1,       sgroupid,
																	"advmanager2_descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
																	"advmanager2_imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"advmanager2_serno",       UT_TYPE_STRING,  sizeof(serno)-1,          serno,
																	"advmanager2_floor",       UT_TYPE_STRING,  sizeof(floor)-1,          floor,
																	"advmanager2_level",       UT_TYPE_STRING,  sizeof(level)-1,          level,
																	"advmanager2_plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
																	"advmanager2_type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
																	"advmanager2_shid",        UT_TYPE_STRING,  sizeof(shopid)-1,         shopid,
																	"advmanager2_plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"advmanager2_flags",       UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"advmanager2_img_long",    UT_TYPE_STRING,  sizeof(img_long)-1,       img_long,
																	"advmanager2_img_width",   UT_TYPE_STRING,  sizeof(img_width)-1,      img_width,
																	"start",                   UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",                   UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",                    UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",                     UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",                  UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				             UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"advmanager2_update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",                UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 strcpy(caShopids,getDsShopids());
	 trim(_keyword);
	 
	 // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,32);
			strcpy(name,caTemp);
   }
	 if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,128);
			strcpy(descr,caTemp);
   }
   if(strlen(serno)>0){
			pasCvtGBK(2,serno,caTemp,32);
			strcpy(serno,caTemp);
   }
   if(strlen(floor)>0){
			pasCvtGBK(2,floor,caTemp,32);
			strcpy(floor,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,255);
			strcpy(imgpath,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
	 // ����/�޸�
	 printf("caUpdate = %s\n", caUpdate);
	 if(!utStrIsSpaces(caUpdate)){
			sprintf(caTemp2, "select floor from ncmshop where id = %lu", atol(shopid));
	    pasDbOneRecord(caTemp2,0,UT_TYPE_STRING,sizeof(s_floor)-1,s_floor);
	    printf("s_floor = %s\n", s_floor);
			
			// ɾ��ԭλ�õ�ͼƬ
		  sprintf(caTemp3, "select imgpath from ncmcomindex where sid=%lu", atol(sid));
		  pasDbOneRecord(caTemp3, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
			if(atol(sid)>0){
				  if(strcmp(imgpath, caImgpath1)==0){
				  		//printf("**************ԭͼƬ����*************\n");
				  		sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', level=%lu, plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu, flags=%lu, img_long=%lu, img_width=%lu where sid=%lu and plateid=%lu",name,atol(sgroupid),descr,imgpath,serno,s_floor,atol(level),atol(plevel),atol(type),time(0),atol(shopid),atol(plateid),atol(flags),atol(img_long),atol(img_width),atol(sid),atol(plateid));	
				  		printf("caTemp = %s\n", caTemp);
						 	pasDbExecSqlF(caTemp);
				  }else{
				  		//printf("**************ԭͼƬ�ı�*************\n");
				  		sprintf(temp2_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
			  			system(temp2_file);
			  			
			  			sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', level=%lu, plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu, flags=%lu, img_long=%lu, img_width=%lu where sid=%lu and plateid=%lu",name,atol(sgroupid),descr,imgpath,serno,s_floor,atol(level),atol(plevel),atol(type),time(0),atol(shopid),atol(plateid),atol(flags),atol(img_long),atol(img_width),atol(sid),atol(plateid));	
				  		printf("caTemp = %s\n", caTemp);
						 	pasDbExecSqlF(caTemp);
				  }
				 	utPltPutVarF(psDbHead,"sid","%lu",atol(sid));
			}else{
  	  	  pasDbOneRecord("select max(sid) from ncmcomindex", 0, UT_TYPE_ULONG, 4, &lMaxid);
			    sprintf(caTemp,"insert into ncmcomindex (sid,name,sgroupid,descr,imgpath,serno,floor,level,plevel,type,systime,shopid,plateid,flags,img_long,img_width) values (%lu,'%s',%lu,'%s','%s','%s','%s',%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu)",(lMaxid+1),name,atol(sgroupid),descr,imgpath,serno,s_floor,atol(level),atol(plevel),atol(type),time(0),atol(shopid),atol(plateid),atol(flags),atol(img_long),atol(img_width));
				 	printf("caTemp = %s\n", caTemp);
				 	pasDbExecSqlF(caTemp);	
				 	utPltPutVarF(psDbHead,"sid","%lu",(lMaxid+1));
			}	
			utPltSetCvtHtml(1);
			utPltPutVar(psDbHead,"name",name);
			utPltPutVar(psDbHead,"sgroupid",sgroupid);
			utPltPutVar(psDbHead,"descr",descr);
			utPltPutVar(psDbHead,"imgpath",imgpath);
			utPltPutVar(psDbHead,"serno",serno);
			utPltPutVar(psDbHead,"floor",s_floor);
			utPltPutVar(psDbHead,"level",level);
			utPltPutVar(psDbHead,"plevel",plevel);
			utPltPutVar(psDbHead,"type",type);
			utPltPutVar(psDbHead,"shopid",shopid);
			utPltPutVar(psDbHead,"plateid",plateid);
			utPltPutVar(psDbHead,"flags",flags);
			utPltPutVar(psDbHead,"img_long",img_long);
			utPltPutVar(psDbHead,"img_width",img_width);
	 		utPltPutVar(psDbHead,"mesg","���ݼ��سɹ�");
	 		utPltSetCvtHtml(0);
	    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomindex_dr_list.htm");	
	    return 0;
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcomindex where sid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
				      return 0;	
					}
					
    		  // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					}
					pasDbExecSqlF("delete from ncmcomindex where sid in(%s)",caSesid);
    	}
	 }
	 
	 stat_all = (index_stat_s *)malloc(sizeof(index_stat_s)*18 + 1);
	 for(i=0; i<18; i++){
 				stat_all[i].flag = 0;
 				stat_all[i].sid = 0;
 				strcpy(stat_all[i].name, "");
 				stat_all[i].sgroupid = 0;
 				strcpy(stat_all[i].descr, "");
 				strcpy(stat_all[i].imgpath, "");
 				strcpy(stat_all[i].serno, "");
 				strcpy(stat_all[i].floor, "");
 				stat_all[i].level = i+1;
 				stat_all[i].plevel = 0;
 				stat_all[i].type = 1;
 				stat_all[i].systime = 0;
 				stat_all[i].shopid = 0;
 				stat_all[i].plateid = atol(plateid);
 				stat_all[i].flags = 0;
 				switch(i){
 						case 0:stat_all[i].img_long = 306;stat_all[i].img_width = 640;break;
 						case 1:stat_all[i].img_long = 306;stat_all[i].img_width = 640;break;
 					  case 2:stat_all[i].img_long = 306;stat_all[i].img_width = 640;break;
 					  case 3:stat_all[i].img_long = 306;stat_all[i].img_width = 640;break;
 					  case 4:stat_all[i].img_long = 199;stat_all[i].img_width = 199;break;
 					  case 5:stat_all[i].img_long = 199;stat_all[i].img_width = 199;break;
 					  case 6:stat_all[i].img_long = 199;stat_all[i].img_width = 199;break;
 					  case 7:stat_all[i].img_long = 199;stat_all[i].img_width = 199;break;
 					  case 8:stat_all[i].img_long = 199;stat_all[i].img_width = 199;break;
 					  case 9:stat_all[i].img_long = 199;stat_all[i].img_width = 199;break;
 					  case 10:stat_all[i].img_long = 199;stat_all[i].img_width = 407;break;
 					  case 11:stat_all[i].img_long = 199;stat_all[i].img_width = 199;break;
 					  case 12:stat_all[i].img_long = 199;stat_all[i].img_width = 199;break;
 					  case 13:stat_all[i].img_long = 199;stat_all[i].img_width = 199;break;
 					  case 14:stat_all[i].img_long = 199;stat_all[i].img_width = 199;break;
 					  case 15:stat_all[i].img_long = 199;stat_all[i].img_width = 199;break;
 					  case 16:stat_all[i].img_long = 199;stat_all[i].img_width = 199;break;
 					  case 17:stat_all[i].img_long = 199;stat_all[i].img_width = 199;break;
 					  case 18:stat_all[i].img_long = 199;stat_all[i].img_width = 199;break;
 					  default:break;
 				}
 				strcpy(stat_all[i].scode, "");
	 }
	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmcomindex where 1=1 and type=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu", atol(plateid));
	 }
	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and shopid in (select id from ncmshop where sgroupid in (%s) )",caShopids);
	 }
	 
	 printf("sql = %s\n", sql);
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select i.sid, i.name, i.sgroupid, i.descr, i.imgpath, i.serno, i.floor, i.level, i.plevel, i.type, i.systime, i.shopid, i.plateid, i.flags, i.img_long, i.img_width, s.service_code from ncmcomindex i left join ncmcomtype t on i.sgroupid = t.sid left join ncmshop s on i.shopid = s.id  where 1=1 and i.type=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (i.name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and i.plateid = %lu", atol(plateid));
	 }
	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and i.shopid in (select id from ncmshop where sgroupid in (%s) )",caShopids);
	 }
	 
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by i.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
													     UT_TYPE_STRING, sizeof(caName)-1,      caName,
													     UT_TYPE_ULONG,  sizeof(long),          &lSgroupid,
													     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
													     UT_TYPE_STRING, sizeof(caImagepath)-1, caImagepath,
													     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
													     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
													     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lType,
													     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
													     UT_TYPE_ULONG,  sizeof(long),          &lShopid,
													     UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags,
															 UT_TYPE_ULONG,  sizeof(long),          &lImg_long,
															 UT_TYPE_ULONG,  sizeof(long),          &lImg_width,
															 UT_TYPE_STRING, sizeof(caScode)-1,     caScode);	
				
	  while(0 == iret || 1405 == iret){
			 for(i=0; i<18; i++){
	  	 		if((i+1) == lLevel){
	  	 				stat_all[i].flag = 0;
			 				stat_all[i].sid = lSid;
			 				strcpy(stat_all[i].name, caName);
			 				stat_all[i].sgroupid = lSgroupid;
			 				strcpy(stat_all[i].descr, caDescr);
			 				strcpy(stat_all[i].imgpath, caImagepath);
			 				strcpy(stat_all[i].serno, caSerno);
			 				strcpy(stat_all[i].floor, caFloor);
			 				stat_all[i].level = lLevel;
			 				stat_all[i].plevel = lPlevel;
			 				stat_all[i].type = lType;
			 				stat_all[i].systime = lSystime;
			 				stat_all[i].shopid = lShopid;
			 				stat_all[i].plateid = lPlateid;
			 				stat_all[i].flags = lFlags;
			 				stat_all[i].img_long = lImg_long;
			 				stat_all[i].img_width = lImg_width;
			 				strcpy(stat_all[i].scode, caScode);
	  	 		}	
	  	 }
			 
			 lSid = 0;
			 memset(caName, 0, sizeof(caName));
			 lSgroupid = 0;
			 memset(caDescr, 0, sizeof(caDescr));
			 memset(caImagepath, 0, sizeof(caImagepath));
			 memset(caSerno, 0, sizeof(caSerno));
			 memset(caFloor, 0, sizeof(caFloor));
	     lLevel = 0;
	     lPlevel = 0;
	     lType = 0;
	     lSystime = 0;
	     lShopid = 0;
	     lPlateid = 0;
	     lFlags = 0;
	     lImg_long = 0;
	     lImg_width = 0;
	     memset(caScode, 0, sizeof(caScode));
	     memset(s_systime, 0, sizeof(s_systime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
															     UT_TYPE_STRING, sizeof(caName)-1,      caName,
															     UT_TYPE_ULONG,  sizeof(long),          &lSgroupid,
															     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
															     UT_TYPE_STRING, sizeof(caImagepath)-1, caImagepath,
															     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
															     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
															     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lType,
															     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
															     UT_TYPE_ULONG,  sizeof(long),          &lShopid,
															     UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags,
																	 UT_TYPE_ULONG,  sizeof(long),          &lImg_long,
																	 UT_TYPE_ULONG,  sizeof(long),          &lImg_width,
																	 UT_TYPE_STRING, sizeof(caScode)-1,     caScode);	
	  }
	  pasDbCloseCursor(psCur);
	  
		/*for(i=0; i<18; i++){
			  printf("stat_all[%d].sid = %lu\n", i, stat_all[i].sid);	
				printf("stat_all[%d].shopid = %lu\n", i, stat_all[i].shopid);
				printf("stat_all[%d].plateid = %lu\n", i, stat_all[i].plateid);
				printf("stat_all[%d].level = %lu\n", i, stat_all[i].level);
				printf("stat_all[%d].flags = %lu\n", i, stat_all[i].flags);					
		}*/
		
		for(i=0; i<18; i++){
			  iNum++;
			  if(iNum > 1)
			  {
				 		utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			  }
			  
			  utPltPutLoopVarF(psDbHead,"flag",iNum,"%lu",0);
			  utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu",stat_all[i].shopid);
			  utPltSetCvtHtml(1);
		    utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
			  utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",stat_all[i].sid);
			  utPltPutLoopVar(psDbHead,"name",iNum,stat_all[i].name);
			  utPltPutLoopVarF(psDbHead,"sgroupid",iNum,"%lu",stat_all[i].sgroupid);
			  utPltPutLoopVar(psDbHead,"descr",iNum,stat_all[i].descr);
			  utPltPutLoopVar(psDbHead,"imgpath",iNum,stat_all[i].imgpath);
			  utPltPutLoopVar(psDbHead,"serno",iNum,stat_all[i].serno);
			  utPltPutLoopVar(psDbHead,"floor",iNum,stat_all[i].floor);
			  utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",stat_all[i].level);
			  utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",stat_all[i].plevel);
			  utPltPutLoopVarF(psDbHead,"type",iNum,"%lu",stat_all[i].type);
			  utPltPutLoopVar(psDbHead,"systime",iNum,utTimFormat("%Y/%m/%d %H:%M:%S", stat_all[i].systime));
			  utPltPutLoopVarF(psDbHead,"plateid",iNum,"%lu",stat_all[i].plateid);
			  utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",stat_all[i].flags);
			  utPltPutLoopVarF(psDbHead,"img_long",iNum,"%lu",stat_all[i].img_long);
			  utPltPutLoopVarF(psDbHead,"img_width",iNum,"%lu",stat_all[i].img_width);
			  utPltPutLoopVar(psDbHead,"scode",iNum,stat_all[i].scode);	
			  utPltSetCvtHtml(0);
		}
//utPltShowDb(psDbHead);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex2_list.htm");
	  return 0;	
}

int ncm_webcomindex3_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  
	  char caImgPath[128]="";
	  char caDispImgPath[128]="";
	  char imgname[128]="";
	  char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
	  char *p=NULL;
	  char *q=NULL;
    
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",(char *)ncLang("0194����"),(char *)ncLang("0183���ļ�����"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// ����ͼƬ���·��
  	sprintf(caImgPath,"%s/images/ShopImages/indeximages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);

	  //utMsgGetSomeNVar(psMsgHead,1,"fileField", UT_TYPE_STRING, 255, imp_file);
	  utMsgGetSomeNVar(psMsgHead,1,"image_fil", UT_TYPE_STRING, sizeof(imp_file)-1, imp_file);

	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*��ָ�����ַ�����ȡһ����
		  	
		  	  ����ԭ�ͣ�
			����char *utStrGetWord(char *p,char *pOut,
			����                   short nMaxLen,char *pTerm);
			����
					����˵��:
			����p �C �����ַ���
			����pOut --- ����ַ���������Ҫ���ȷ���ÿռ�
			����nMaxLen --- ����ַ�������󳤶�
			����pTerm  ----- ��ֹ�������ַ�����ϵͳ��ɨ����ʱ�������ַ���ĳһ���ַ�ʱ������
			����
			��������ֵ��
			���������ַ���ָ�롣*/
				p=utStrGetWord(p,temp_file,200,";\n");
		  	printf("temp_file = %s\n", temp_file);
		  	strcpy(temp3_file, temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
				}else{
					  q = strtok(temp3_file, ".");
					  //if(q)printf("%s\n", q);
					  q = strtok(NULL, ".");
					  //if(q)printf("%s\n", q);
					  srand((unsigned int)time(0));
						sprintf(imgname, "index_%lu_%lu.%s", time(0), rand()%100, q);
						//printf("imgname = %s\n", imgname);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
						//printf("temp2_file = %s\n", temp2_file);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/indeximages/");
				    strcat(caDispImgPath, imgname);
				}
		}
		
	  utPltPutVar(psDbHead,"filepath",caDispImgPath);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex3_closeFile.htm");
    return 0;
}

// ����һ����Ϣ(֣�ݵ���)
int ncm_webcomindex2_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 int i=0;
	 typedef struct index_stat{
	 	    unsigned long flag;
	 	    unsigned long inum;
	 			unsigned long sid;
			  char name[32];
			  unsigned long sgroupid;
			  char descr[128];
			  char imgpath[128];
			  char serno[32];
			  char floor[32];
			  unsigned long level;
			  unsigned long plevel;
			  unsigned long type;
			  unsigned long systime;
			  unsigned long shopid;
			  unsigned long plateid;
			  unsigned long img_long;
			  unsigned long img_width;
			  unsigned long flags;
			  char label[128];
			  char scode[16];
	 }index_stat_s;
	 index_stat_s *stat_temp;
	 index_stat_s *stat_all;
	 
	 // ������Ϣ����
	 char sid[20] = "";
	 char name[32] = "";
	 char sgroupid[32] = "";
	 char descr[128] = "";
	 char imgpath[128] = "";
	 char serno[32] = "";
	 char floor[32] = "";
	 char level[20] = "";
	 char plevel[20] = "";
	 char type[20] = "";
	 char systime[32] = "";
	 char shopid[32] = "";
	 char plateid[32] = "";
	 char flags[32] = "";
	 char img_long[20] = "";
	 char img_width[20] = "";
	 char label[128] = "";
	 
	 // ��/��
	 char s_floor[32] = "";
	 unsigned long lCount = 0; // ���м�¼�ĸ���
	 unsigned long lMaxid = 0;
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lSid = 0;
	 char 					caName[32] = "";
	 unsigned long	lSgroupid = 0;
	 char 					caDescr[128] = "";
	 char 					caImagepath[128] = "";
	 char 					caSerno[32] = "";
	 char 					caFloor[32] = "";
	 unsigned long	lLevel = 0;
	 unsigned long	lPlevel = 0;
	 unsigned long	lType = 0;
	 char           caType[32] = "";
	 unsigned long	lSystime = 0;
	 unsigned long	lShopid = 0;
	 unsigned long	lPlateid = 0;
	 unsigned long	lFlags = 0;
	 unsigned long  lImg_long = 0;
	 unsigned long  lImg_width = 0;
	 char           caLabel[128] = "";
	 char           caScode[16] = "";
	 char           s_systime[32] = "";
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
	 char caTemp2[4096] = "";
	 char caTemp3[4096] = "";
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
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
   unsigned long lFlag = 0;
	 // �õ��̼�ID
	 unsigned long lShopId = 0;
	 dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	 printf("lShopId = %lu\n", lShopId);
	 
	 //ͼƬ����
	 char caImgpath1[128]="";
	 char imgname[128]="";
	 char temp_file[129]="";
	 char temp2_file[129]="";
    
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 26,"keyword",                UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"advmanager_sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
																	"advmanager_name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
																	"advmanager_tyid",        UT_TYPE_STRING,  sizeof(sgroupid)-1,       sgroupid,
																	"advmanager_descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
																	"advmanager_imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"advmanager_serno",       UT_TYPE_STRING,  sizeof(serno)-1,          serno,
																	"advmanager_floor",       UT_TYPE_STRING,  sizeof(floor)-1,          floor,
																	"advmanager_level",       UT_TYPE_STRING,  sizeof(level)-1,          level,
																	"advmanager_plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
																	"advmanager_type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
																	"advmanager_shid",        UT_TYPE_STRING,  sizeof(shopid)-1,         shopid,
																	"advmanager_plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"advmanager_flags",       UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"advmanager_img_long",    UT_TYPE_STRING,  sizeof(img_long)-1,       img_long,
																	"advmanager_img_width",   UT_TYPE_STRING,  sizeof(img_width)-1,      img_width,
																	"advmanager_label",       UT_TYPE_STRING,  sizeof(label)-1,          label,
																	"start",                  UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",                  UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",                   UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         						UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      						UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				 						UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"advmanager_update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",    						UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 
	 trim(_keyword);
	 
	 // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,32);
			strcpy(name,caTemp);
   }
	 if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,128);
			strcpy(descr,caTemp);
   }
   if(strlen(serno)>0){
			pasCvtGBK(2,serno,caTemp,32);
			strcpy(serno,caTemp);
   }
   if(strlen(floor)>0){
			pasCvtGBK(2,floor,caTemp,32);
			strcpy(floor,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,255);
			strcpy(imgpath,caTemp);
   }
   if(strlen(label)>0){
			pasCvtGBK(2,label,caTemp,128);
			strcpy(label,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
	 // ����/�޸�
	 printf("caUpdate = %s\n", caUpdate);
	 if(!utStrIsSpaces(caUpdate)){
			sprintf(caTemp2, "select floor from ncmshop where id = %lu", atol(shopid));
	    pasDbOneRecord(caTemp2,0,UT_TYPE_STRING,sizeof(s_floor)-1,s_floor);
	    printf("s_floor = %s\n", s_floor);
			
			// ɾ��ԭλ�õ�ͼƬ
		  sprintf(caTemp3, "select imgpath from ncmcomindex where sid=%lu", atol(sid));
		  pasDbOneRecord(caTemp3, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
			if(atol(sid)>0){
				  if(strcmp(imgpath, caImgpath1)==0){
				  		//printf("**************ԭͼƬ����*************\n");
				  		sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', level=%lu, plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu, flags=%lu, img_long=%lu, img_width=%lu, label='%s' where sid=%lu and plateid=%lu",name,atol(sgroupid),descr,imgpath,serno,s_floor,atol(level),atol(plevel),atol(type),time(0),atol(shopid),atol(plateid),atol(flags),atol(img_long),atol(img_width),label,atol(sid),atol(plateid));	
				  		printf("caTemp = %s\n", caTemp);
						 	pasDbExecSqlF(caTemp);
				  }else{
				  		//printf("**************ԭͼƬ�ı�*************\n");
				  		sprintf(temp2_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
			  			system(temp2_file);
			  			
			  			sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', level=%lu, plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu, flags=%lu, img_long=%lu, img_width=%lu, label='%s' where sid=%lu and plateid=%lu",name,atol(sgroupid),descr,imgpath,serno,s_floor,atol(level),atol(plevel),atol(type),time(0),atol(shopid),atol(plateid),atol(flags),atol(img_long),atol(img_width),label,atol(sid),atol(plateid));	
				  		printf("caTemp = %s\n", caTemp);
						 	pasDbExecSqlF(caTemp);
				  }
				 	utPltPutVarF(psDbHead,"sid","%lu",atol(sid));
			}else{
  	  	  pasDbOneRecord("select max(sid) from ncmcomindex", 0, UT_TYPE_ULONG, 4, &lMaxid);
			    sprintf(caTemp,"insert into ncmcomindex (sid,name,sgroupid,descr,imgpath,serno,floor,level,plevel,type,systime,shopid,plateid,flags,img_long,img_width,label) values (%lu,'%s',%lu,'%s','%s','%s','%s',%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,'%s')",(lMaxid+1),name,atol(sgroupid),descr,imgpath,serno,s_floor,atol(level),atol(plevel),atol(type),time(0),atol(shopid),atol(plateid),atol(flags),atol(img_long),atol(img_width),label);
				 	printf("caTemp = %s\n", caTemp);
				 	pasDbExecSqlF(caTemp);	
				 	utPltPutVarF(psDbHead,"sid","%lu",(lMaxid+1));
			}	
			utPltSetCvtHtml(1);
			utPltPutVar(psDbHead,"name",name);
			utPltPutVar(psDbHead,"sgroupid",sgroupid);
			utPltPutVar(psDbHead,"descr",descr);
			utPltPutVar(psDbHead,"imgpath",imgpath);
			utPltPutVar(psDbHead,"serno",serno);
			utPltPutVar(psDbHead,"floor",s_floor);
			utPltPutVar(psDbHead,"level",level);
			utPltPutVar(psDbHead,"plevel",plevel);
			utPltPutVar(psDbHead,"type",type);
			utPltPutVar(psDbHead,"shopid",shopid);
			utPltPutVar(psDbHead,"plateid",plateid);
			utPltPutVar(psDbHead,"flags",flags);
			utPltPutVar(psDbHead,"img_long",img_long);
			utPltPutVar(psDbHead,"img_width",img_width);
			utPltPutVar(psDbHead,"label",label);
	 		utPltPutVar(psDbHead,"mesg","���ݼ��سɹ�");
	 		utPltSetCvtHtml(0);
	    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomindex_dr_list.htm");	
	    return 0;
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcomindex where sid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
				      return 0;	
					}
					
    		  // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					}
					pasDbExecSqlF("delete from ncmcomindex where sid in(%s)",caSesid);
    	}
	 }
	 
	 stat_temp = (index_stat_s *)malloc(sizeof(index_stat_s)*15 + 1);
	 for(i=0; i<15; i++){
 				stat_temp[i].flag = 0;
 				stat_temp[i].sid = 0;
 				strcpy(stat_temp[i].name, "");
 				stat_temp[i].sgroupid = 0;
 				strcpy(stat_temp[i].descr, "");
 				strcpy(stat_temp[i].imgpath, "");
 				strcpy(stat_temp[i].serno, "");
 				strcpy(stat_temp[i].floor, "");
 				stat_temp[i].level = 0;
 				stat_temp[i].plevel = 0;
 				stat_temp[i].type = 0;
 				stat_temp[i].systime = 0;
 				stat_temp[i].shopid = 0;
 				stat_temp[i].plateid = 0;
 				stat_temp[i].flags = 1;
 				stat_temp[i].img_long = 0;
 				stat_temp[i].img_width = 0;
 				strcpy(stat_temp[i].label, "");
 				strcpy(stat_temp[i].scode, "");
	 }
			  	 
	 unsigned long	p_lSid = 0;
	 char 					p_caName[32] = "";
	 unsigned long	p_lSgroupid = 0;
	 char 					p_caDescr[128] = "";
	 char 					p_caImagepath[128] = "";
	 char 					p_caSerno[32] = "";
	 char 					p_caFloor[32] = "";
	 unsigned long	p_lLevel = 0;
	 unsigned long	p_lPlevel = 0;
	 unsigned long	p_lType = 0;
	 char           p_caType[32] = "";
	 unsigned long	p_lSystime = 0;
	 unsigned long	p_lShopid = 0;
	 unsigned long	p_lPlateid = 0;
	 unsigned long	p_lFlags = 0;
	 unsigned long	p_lImg_long = 0;
	 unsigned long	p_lImg_width = 0;
	 char           p_caLabel[128] = "";
	 char           p_caScode[16] = "";
	 char           p_s_systime[32] = "";
	 if(lShopId>0){
	 		 strcpy(sql, "select i.sid, i.name, i.sgroupid, i.descr, i.imgpath, i.serno, i.floor, i.level, i.plevel, i.type, i.systime, i.shopid, i.plateid, i.flags, i.img_long, i.img_width, i.label, s.service_code from ncmcomindex i left join ncmcomtype t on i.sgroupid = t.sid left join ncmshop s on i.shopid = s.id  where 1=1 and i.type=1");
			 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
			 {
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and i.plateid = %lu", atol(plateid));
			 }
			 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and i.shopid = %lu", lShopId);
			 printf("sql = %s\n", sql);
			 psCur = pasDbOpenSql(sql, 0);
			 if(psCur==NULL){
			 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
		       return 0;	
			 }
			 
			 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),            &p_lSid,
															     UT_TYPE_STRING, sizeof(p_caName)-1,      p_caName,
															     UT_TYPE_ULONG,  sizeof(long),            &p_lSgroupid,
															     UT_TYPE_STRING, sizeof(p_caDescr)-1,     p_caDescr,
															     UT_TYPE_STRING, sizeof(p_caImagepath)-1, p_caImagepath,
															     UT_TYPE_STRING, sizeof(p_caSerno)-1,     p_caSerno,
															     UT_TYPE_STRING, sizeof(p_caFloor)-1,     p_caFloor,
															     UT_TYPE_ULONG,  sizeof(long),            &p_lLevel,
															     UT_TYPE_ULONG,  sizeof(long),            &p_lPlevel,
															     UT_TYPE_ULONG,  sizeof(long),            &p_lType,
															     UT_TYPE_ULONG,  sizeof(long),            &p_lSystime,
															     UT_TYPE_ULONG,  sizeof(long),            &p_lShopid,
															     UT_TYPE_ULONG,  sizeof(long),            &p_lPlateid,
																	 UT_TYPE_ULONG,  sizeof(long),            &p_lFlags,
																	 UT_TYPE_ULONG,  sizeof(long),            &p_lImg_long,
																	 UT_TYPE_ULONG,  sizeof(long),            &p_lImg_width,
																	 UT_TYPE_STRING, sizeof(p_caLabel)-1,     p_caLabel,
																	 UT_TYPE_STRING, sizeof(p_caScode)-1,     p_caScode);	
						
			  while(0 == iret || 1405 == iret){
			  	 for(i=0; i<15; i++){
			  	 		if((i+1) == p_lLevel){
			  	 				stat_temp[i].flag = 1;
					 				stat_temp[i].sid = p_lSid;
					 				strcpy(stat_temp[i].name, p_caName);
					 				stat_temp[i].sgroupid = p_lSgroupid;
					 				strcpy(stat_temp[i].descr, p_caDescr);
					 				strcpy(stat_temp[i].imgpath, p_caImagepath);
					 				strcpy(stat_temp[i].serno, p_caSerno);
					 				strcpy(stat_temp[i].floor, p_caFloor);
					 				stat_temp[i].level = p_lLevel;
					 				stat_temp[i].plevel = p_lPlevel;
					 				stat_temp[i].type = p_lType;
					 				stat_temp[i].systime = p_lSystime;
					 				stat_temp[i].shopid = p_lShopid;
					 				stat_temp[i].plateid = p_lPlateid;
					 				stat_temp[i].flags = p_lFlags;
					 				stat_temp[i].img_long = p_lImg_long;
					 				stat_temp[i].img_width = p_lImg_width;
					 				strcpy(stat_temp[i].label, p_caLabel);
					 				strcpy(stat_temp[i].scode, p_caScode);
			  	 		}	
			  	 }
	 				 
					 p_lSid = 0;
					 memset(p_caName, 0, sizeof(p_caName));
					 p_lSgroupid = 0;
					 memset(p_caDescr, 0, sizeof(p_caDescr));
					 memset(p_caImagepath, 0, sizeof(p_caImagepath));
					 memset(p_caSerno, 0, sizeof(p_caSerno));
					 memset(p_caFloor, 0, sizeof(p_caFloor));
			     p_lLevel = 0;
			     p_lPlevel = 0;
			     p_lType = 0;
			     p_lSystime = 0;
			     p_lShopid = 0;
			     p_lPlateid = 0;
			     p_lFlags = 0;
			     p_lImg_long = 0;
			     p_lImg_width = 0;
			     memset(p_caLabel, 0, sizeof(p_caLabel));
			     memset(p_caScode, 0, sizeof(p_caScode));
			     memset(p_s_systime, 0, sizeof(p_s_systime));
					 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),            &p_lSid,
																	     UT_TYPE_STRING, sizeof(p_caName)-1,      p_caName,
																	     UT_TYPE_ULONG,  sizeof(long),            &p_lSgroupid,
																	     UT_TYPE_STRING, sizeof(p_caDescr)-1,     p_caDescr,
																	     UT_TYPE_STRING, sizeof(p_caImagepath)-1, p_caImagepath,
																	     UT_TYPE_STRING, sizeof(p_caSerno)-1,     p_caSerno,
																	     UT_TYPE_STRING, sizeof(p_caFloor)-1,     p_caFloor,
																	     UT_TYPE_ULONG,  sizeof(long),            &p_lLevel,
																	     UT_TYPE_ULONG,  sizeof(long),            &p_lPlevel,
																	     UT_TYPE_ULONG,  sizeof(long),            &p_lType,
																	     UT_TYPE_ULONG,  sizeof(long),            &p_lSystime,
																	     UT_TYPE_ULONG,  sizeof(long),            &p_lShopid,
																	     UT_TYPE_ULONG,  sizeof(long),            &p_lPlateid,
																			 UT_TYPE_ULONG,  sizeof(long),            &p_lFlags,
																			 UT_TYPE_ULONG,  sizeof(long),            &p_lImg_long,
																			 UT_TYPE_ULONG,  sizeof(long),            &p_lImg_width,
																			 UT_TYPE_STRING, sizeof(p_caLabel)-1,     p_caLabel,
																			 UT_TYPE_STRING, sizeof(p_caScode)-1,     p_caScode);	
			  }
			  pasDbCloseCursor(psCur);
	 }
	 
	 stat_all = (index_stat_s *)malloc(sizeof(index_stat_s)*15 + 1);
	 for(i=0; i<15; i++){
 				stat_all[i].flag = 0;
 				stat_all[i].sid = 0;
 				strcpy(stat_all[i].name, "");
 				stat_all[i].sgroupid = 0;
 				strcpy(stat_all[i].descr, "");
 				strcpy(stat_all[i].imgpath, "");
 				strcpy(stat_all[i].serno, "");
 				strcpy(stat_all[i].floor, "");
 				stat_all[i].level = i+1;
 				stat_all[i].plevel = 0;
 				stat_all[i].type = 1;
 				stat_all[i].systime = 0;
 				stat_all[i].shopid = 0;
 				stat_all[i].plateid = atol(plateid);
 				stat_all[i].flags = 0;
 				switch(i){
 						case 0:stat_all[i].img_long = 306;stat_all[i].img_width = 640;break;
 						case 1:stat_all[i].img_long = 306;stat_all[i].img_width = 640;break;
 					  case 2:stat_all[i].img_long = 306;stat_all[i].img_width = 640;break;
 					  case 3:stat_all[i].img_long = 306;stat_all[i].img_width = 640;break;
 					  case 4:stat_all[i].img_long = 172;stat_all[i].img_width = 640;break;
 					  case 5:stat_all[i].img_long = 172;stat_all[i].img_width = 640;break;
 					  case 6:stat_all[i].img_long = 126;stat_all[i].img_width = 302;break;
 					  case 7:stat_all[i].img_long = 126;stat_all[i].img_width = 302;break;
 					  case 8:stat_all[i].img_long = 126;stat_all[i].img_width = 302;break;
 					  case 9:stat_all[i].img_long = 126;stat_all[i].img_width = 144;break;
 					  case 10:stat_all[i].img_long = 126;stat_all[i].img_width = 144;break;
 					  case 11:stat_all[i].img_long = 136;stat_all[i].img_width = 136;break;
 					  case 12:stat_all[i].img_long = 136;stat_all[i].img_width = 136;break;
 					  case 13:stat_all[i].img_long = 136;stat_all[i].img_width = 136;break;
 					  case 14:stat_all[i].img_long = 136;stat_all[i].img_width = 136;break;
 					  default:break;
 				}
 				strcpy(stat_all[i].label, "");
 				strcpy(stat_all[i].scode, "");
	 }
	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmcomindex where 1=1 and type=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu", atol(plateid));
	 }
	 //if(lShopId==0){ // ����̼���admin����
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and flags = 0");	
	 		//snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and shopid = %lu", lshopid);	
	 //}
	 printf("sql = %s\n", sql);
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select i.sid, i.name, i.sgroupid, i.descr, i.imgpath, i.serno, i.floor, i.level, i.plevel, i.type, i.systime, i.shopid, i.plateid, i.flags, i.img_long, i.img_width, i.label, s.service_code from ncmcomindex i left join ncmcomtype t on i.sgroupid = t.sid left join ncmshop s on i.shopid = s.id  where 1=1 and i.type=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (i.name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and i.plateid = %lu", atol(plateid));
	 }
	 //if(lShopId==0){ // ����̼���admin
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and i.flags = 0");	
	 		//snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and i.shopid = %lu", lshopid);	
	 //}
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by i.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
													     UT_TYPE_STRING, sizeof(caName)-1,      caName,
													     UT_TYPE_ULONG,  sizeof(long),          &lSgroupid,
													     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
													     UT_TYPE_STRING, sizeof(caImagepath)-1, caImagepath,
													     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
													     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
													     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lType,
													     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
													     UT_TYPE_ULONG,  sizeof(long),          &lShopid,
													     UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags,
															 UT_TYPE_ULONG,  sizeof(long),          &lImg_long,
															 UT_TYPE_ULONG,  sizeof(long),          &lImg_width,
															 UT_TYPE_STRING, sizeof(caLabel)-1,     caLabel,
															 UT_TYPE_STRING, sizeof(caScode)-1,     caScode);	
				
	  while(0 == iret || 1405 == iret){
			 for(i=0; i<15; i++){
	  	 		if((i+1) == lLevel){
	  	 				stat_all[i].flag = 0;
			 				stat_all[i].sid = lSid;
			 				strcpy(stat_all[i].name, caName);
			 				stat_all[i].sgroupid = lSgroupid;
			 				strcpy(stat_all[i].descr, caDescr);
			 				strcpy(stat_all[i].imgpath, caImagepath);
			 				strcpy(stat_all[i].serno, caSerno);
			 				strcpy(stat_all[i].floor, caFloor);
			 				stat_all[i].level = lLevel;
			 				stat_all[i].plevel = lPlevel;
			 				stat_all[i].type = lType;
			 				stat_all[i].systime = lSystime;
			 				stat_all[i].shopid = lShopid;
			 				stat_all[i].plateid = lPlateid;
			 				stat_all[i].flags = lFlags;
			 				stat_all[i].img_long = lImg_long;
			 				stat_all[i].img_width = lImg_width;
			 				strcpy(stat_all[i].label, caLabel);
			 				strcpy(stat_all[i].scode, caScode);
	  	 		}	
	  	 }
			 
			 lSid = 0;
			 memset(caName, 0, sizeof(caName));
			 lSgroupid = 0;
			 memset(caDescr, 0, sizeof(caDescr));
			 memset(caImagepath, 0, sizeof(caImagepath));
			 memset(caSerno, 0, sizeof(caSerno));
			 memset(caFloor, 0, sizeof(caFloor));
	     lLevel = 0;
	     lPlevel = 0;
	     lType = 0;
	     lSystime = 0;
	     lShopid = 0;
	     lPlateid = 0;
	     lFlags = 0;
	     lImg_long = 0;
	     lImg_width = 0;
	     memset(caLabel, 0, sizeof(caLabel));
	     memset(caScode, 0, sizeof(caScode));
	     memset(s_systime, 0, sizeof(s_systime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
															     UT_TYPE_STRING, sizeof(caName)-1,      caName,
															     UT_TYPE_ULONG,  sizeof(long),          &lSgroupid,
															     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
															     UT_TYPE_STRING, sizeof(caImagepath)-1, caImagepath,
															     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
															     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
															     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lType,
															     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
															     UT_TYPE_ULONG,  sizeof(long),          &lShopid,
															     UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags,
																	 UT_TYPE_ULONG,  sizeof(long),          &lImg_long,
																	 UT_TYPE_ULONG,  sizeof(long),          &lImg_width,
																	 UT_TYPE_STRING, sizeof(caLabel)-1,     caLabel,
																	 UT_TYPE_STRING, sizeof(caScode)-1,     caScode);	
	  }
	  pasDbCloseCursor(psCur);
	  
	  if(lShopId>0){
				for(i=0; i<15; i++){
						if( 0==i || 1==i || 2==i || 4==i || 7==i || 11==i || 12==i || 13==i || 14==i){
							 stat_all[i].flag	=	stat_temp[i].flag;
							 stat_all[i].sid =	stat_temp[i].sid;
							 strcpy(stat_all[i].name, stat_temp[i].name);
							 stat_all[i].sgroupid = stat_temp[i].sgroupid;
			 				 strcpy(stat_all[i].descr, stat_temp[i].descr);
			 				 strcpy(stat_all[i].imgpath, stat_temp[i].imgpath);
			 				 strcpy(stat_all[i].serno, stat_temp[i].serno);
			 				 strcpy(stat_all[i].floor, stat_temp[i].floor);
			 				 //stat_all[i].level = stat_temp[i].level;
			 				 stat_all[i].plevel = stat_temp[i].plevel;
			 				 //stat_all[i].type = stat_temp[i].type;
			 				 stat_all[i].systime = stat_temp[i].systime;
			 				 stat_all[i].shopid = stat_temp[i].shopid;
			 				 //stat_all[i].plateid = stat_temp[i].plateid;
			 				 stat_all[i].flags = stat_temp[i].flags;
			 				 //stat_all[i].img_long = stat_temp[i].img_long;
			 				 //stat_all[i].img_width = stat_temp[i].img_width;
			 				 strcpy(stat_all[i].label, stat_temp[i].label);
			 				 strcpy(stat_all[i].scode, stat_temp[i].scode);	
						}	
				}
  	}
		
		for(i=0; i<15; i++){
			  iNum++;
			  if(iNum > 1)
			  {
				 		utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			  }
			  
			  if(lShopId>0){
			  		utPltPutLoopVarF(psDbHead,"flag",iNum,"%lu",1);
			  		utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu",lShopId);
			  }else{
			  	  utPltPutLoopVarF(psDbHead,"flag",iNum,"%lu",0);
			  	  utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu",stat_all[i].shopid);
			  }
			  utPltSetCvtHtml(1);
		    utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
			  utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",stat_all[i].sid);
			  utPltPutLoopVar(psDbHead,"name",iNum,stat_all[i].name);
			  utPltPutLoopVarF(psDbHead,"sgroupid",iNum,"%lu",stat_all[i].sgroupid);
			  utPltPutLoopVar(psDbHead,"descr",iNum,stat_all[i].descr);
			  utPltPutLoopVar(psDbHead,"imgpath",iNum,stat_all[i].imgpath);
			  utPltPutLoopVar(psDbHead,"serno",iNum,stat_all[i].serno);
			  utPltPutLoopVar(psDbHead,"floor",iNum,stat_all[i].floor);
			  utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",stat_all[i].level);
			  utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",stat_all[i].plevel);
			  utPltPutLoopVarF(psDbHead,"type",iNum,"%lu",stat_all[i].type);
			  utPltPutLoopVar(psDbHead,"systime",iNum,utTimFormat("%Y/%m/%d %H:%M:%S", stat_all[i].systime));
			  utPltPutLoopVarF(psDbHead,"plateid",iNum,"%lu",stat_all[i].plateid);
			  utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",stat_all[i].flags);
			  utPltPutLoopVarF(psDbHead,"img_long",iNum,"%lu",stat_all[i].img_long);
			  utPltPutLoopVarF(psDbHead,"img_width",iNum,"%lu",stat_all[i].img_width);
			  utPltPutLoopVar(psDbHead,"label",iNum,stat_all[i].label);	
			  utPltPutLoopVar(psDbHead,"scode",iNum,stat_all[i].scode);	
			  utPltSetCvtHtml(0);
		}
	  //utPltShowDb(psDbHead);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex2_list.htm");
	  return 0;	
}

int ncm_webcomindex2_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  
	  char caImgPath[128]="";
	  char caDispImgPath[128]="";
	  char imgname[128]="";
	  char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
    char temp4_file[129]="";
 		char caTemp[1024]="";
	  char *p=NULL;
	  char *q=NULL;
	  char *r=NULL;
	  char temp[129]="";
    int i=0;
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",(char *)ncLang("0194����"),(char *)ncLang("0183���ļ�����"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// ����ͼƬ���·��
  	sprintf(caImgPath,"%s/images/ShopImages/indeximages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);

	  //utMsgGetSomeNVar(psMsgHead,1,"fileField", UT_TYPE_STRING, 255, imp_file);
	  utMsgGetSomeNVar(psMsgHead,1,"image_fil", UT_TYPE_STRING, sizeof(imp_file)-1, imp_file);

	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*��ָ�����ַ�����ȡһ����
		  	
		  	  ����ԭ�ͣ�
			����char *utStrGetWord(char *p,char *pOut,
			����                   short nMaxLen,char *pTerm);
			����
					����˵��:
			����p �C �����ַ���
			����pOut --- ����ַ���������Ҫ���ȷ���ÿռ�
			����nMaxLen --- ����ַ�������󳤶�
			����pTerm  ----- ��ֹ�������ַ�����ϵͳ��ɨ����ʱ�������ַ���ĳһ���ַ�ʱ������
			����
			��������ֵ��
			���������ַ���ָ�롣*/
				p=utStrGetWord(p,temp_file,200,";\n"); // �õ���һ���ַ���
		  	//printf("temp_file = %s\n", temp_file);
		  	if((*p)==';')
		  	p=utStrGetWord(p+1,temp_file,200,";\n"); // �õ��ڶ����ַ���
		  	//printf("temp_file = %s\n", temp_file);
		  	strcpy(temp4_file, temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
				}else{
					  r = strtok(temp4_file, "/");
					  while( r!=NULL){
					  	  strcpy(temp, r);
					  		r = strtok(NULL, "/");		
					  } 
					  //printf("temp = %s\n", temp);
					  strcpy(temp3_file, temp);
					  //printf("temp3_file = %s\n", temp3_file);
					  q = strtok(temp3_file, ".");
					  //if(q)printf("%s\n", q);
					  q = strtok(NULL, ".");
					  //if(q)printf("%s\n", q);
					  srand((unsigned int)time(0));
						sprintf(imgname, "index_%lu_%lu.%s", time(0), rand()%100, q);
						printf("imgname = %s\n", imgname);
						//sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp, caImgPath, imgname);
						printf("temp2_file = %s\n", temp2_file);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/indeximages/");
				    strcat(caDispImgPath, imgname);
				}
		}
		
	  utPltPutVar(psDbHead,"filepath",caDispImgPath);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex2_closeFile.htm");
    return 0;
}

// �����Ϣ
int ncm_webcomindex_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char sid[20] = "";
	 char name[32] = "";
	 char sgroupname[32] = "";
	 char descr[128] = "";
	 char imgpath[128] = "";
	 char serno[32] = "";
	 char floor[32] = "";
	 char level[20] = "";
	 char plevel[20] = "";
	 char type[20] = "";
	 char systime[32] = "";
	 char shopname[32] = "";
	 char plateid[32] = "";
	 char flags[32] = "";
	 char temp_file[128] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lSid = 0;
	 char 					caName[32] = "";
	 char	          caSgroupname[32] = "";
	 char 					caDescr[128] = "";
	 char 					caImgpath[128] = "";
	 char 					caSerno[32] = "";
	 char 					caFloor[32] = "";
	 unsigned long	lLevel = 0;
	 unsigned long	lPlevel = 0;
	 unsigned long	lType = 0;
	 char           caType[32] = "";
	 unsigned long	lSystime = 0;
	 char	          caShopname[32] = "";
	 unsigned long	lPlateid = 0;
	 unsigned long	lFlags = 0;
	 char           caScode[16] = "";
	 char           s_systime[32] = "";
	 
	 char insert[16] = "";
	 char caInsert[16] = "";
	 char update[16] = "";
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
   
   unsigned long lShopId = 0;
	 dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 25,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
																	"name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
																	"sgroupname",  UT_TYPE_STRING,  sizeof(sgroupname)-1,     sgroupname,
																	"descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"serno",       UT_TYPE_STRING,  sizeof(serno)-1,          serno,
																	"floor",       UT_TYPE_STRING,  sizeof(floor)-1,          floor,
																	"level",       UT_TYPE_STRING,  sizeof(level)-1,          level,
																	"plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
																	"type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
																	"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
																	"shopname",    UT_TYPE_STRING,  sizeof(shopname)-1,       shopname,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"insert",      UT_TYPE_STRING,  10,                       caInsert,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"update",      UT_TYPE_STRING,  10,                       caUpdate,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport);
	 
	 trim(_keyword);
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,32);
			strcpy(name,caTemp);
   }
   if(strlen(sgroupname)>0){
			pasCvtGBK(2,sgroupname,caTemp,32);
			strcpy(sgroupname,caTemp);
   }
	 if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,32);
			strcpy(descr,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,128);
			strcpy(imgpath,caTemp);
   }
   if(strlen(serno)>0){
			pasCvtGBK(2,serno,caTemp,32);
			strcpy(serno,caTemp);
   }
   if(strlen(floor)>0){
			pasCvtGBK(2,floor,caTemp,32);
			strcpy(floor,caTemp);
   }
   if(strlen(systime)>0){
			pasCvtGBK(2,systime,caTemp,32);
			strcpy(systime,caTemp);
   }
   if(strlen(shopname)>0){
			pasCvtGBK(2,shopname,caTemp,32);
			strcpy(shopname,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid));
			strcpy(caSesid,caTemp);
   }
   
   // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcomindex where sid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
				      return 0;	
					}
					
    		  // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					}
					pasDbExecSqlF("delete from ncmcomindex where sid in(%s)",caSesid);
    	}
	 } 
	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmcomindex where 1=1 and type=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu", atol(plateid));
	 }
	 if(lShopId>0) // �����̵��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and shopid = %lu", lShopId);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select i.sid, i.name, t.name, i.descr, i.imgpath, i.serno, i.floor, i.level, i.plevel, i.type, i.systime, s.name, i.plateid, i.flags, s.service_code from ncmcomindex i left join ncmcomtype t on i.sgroupid = t.sid left join ncmshop s on i.shopid = s.id  where 1=1 and i.type=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (i.name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and i.plateid = %lu", atol(plateid));
	 }
	 if(lShopId>0) // �����̵��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and i.shopid = %lu", lShopId);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by i.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
													     UT_TYPE_STRING, sizeof(caName)-1,      caName,
													     UT_TYPE_STRING, sizeof(caSgroupname)-1,caSgroupname,
													     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
													     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
													     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
													     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lType,
													     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
													     UT_TYPE_STRING, sizeof(caShopname)-1,  caShopname,
													     UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags,
															 UT_TYPE_STRING, sizeof(caScode)-1,     caScode);	
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
		   
		   if(lSystime>0){
		   		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
		   }else{
		   		strcpy(s_systime, "");	
		   }
		   
			 switch(lType){
					case 1:strcpy(caType, "���");break;
					case 2:strcpy(caType, "Ʒ��");break;
					case 3:strcpy(caType, "����");break;
					case 4:strcpy(caType, "��Ƶ");break;
					default:break;	
			 }
			 utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"sgroupname",iNum,caSgroupname);
			 utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"serno",iNum,caSerno);
			 utPltPutLoopVar(psDbHead,"floor",iNum,caFloor);
			 utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",lLevel);
			 utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",lPlevel);
			 utPltPutLoopVarF(psDbHead,"type",iNum,caType);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVarF(psDbHead,"shopname",iNum,caShopname);
			 utPltPutLoopVarF(psDbHead,"plateid",iNum,"%lu",lPlateid);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltPutLoopVar(psDbHead,"scode",iNum,caScode);
			 utPltSetCvtHtml(0);
			 lSid = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caSgroupname, 0, sizeof(caSgroupname));
			 memset(caDescr, 0, sizeof(caDescr));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 memset(caSerno, 0, sizeof(caSerno));
			 memset(caFloor, 0, sizeof(caFloor));
	     lLevel = 0;
	     lPlevel = 0;
	     lType = 0;
	     memset(caType, 0, sizeof(caType));
	     lSystime = 0;
	     memset(caShopname, 0, sizeof(caShopname));
	     lPlateid = 0;
	     lFlags = 0;
	     memset(caScode, 0, sizeof(caScode));
	     memset(s_systime, 0, sizeof(s_systime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
															     UT_TYPE_STRING, sizeof(caName)-1,      caName,
															     UT_TYPE_STRING, sizeof(caSgroupname)-1,caSgroupname,
															     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
															     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
															     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
															     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lType,
															     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
															     UT_TYPE_STRING, sizeof(caShopname)-1,  caShopname,
															     UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags,
																	 UT_TYPE_STRING, sizeof(caScode)-1,     caScode);	
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex_list.htm");
	  return 0;	
}

int ncm_webcomindex_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
//#endif  
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iFlag=0;
	 int level=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 char           caKey[32] = "";
	 char           caSid[32] = "";
	 char           caPlateid[32] = "";
	 char 					caName[32] = "";
	 unsigned long	lSgroupid = 0;
	 char 					caImgpath[128] = "";
	 char 					caSerno[32] = "";
	 char 					caFloor[32] = "";
	 unsigned long  lLevel = 0;
	 unsigned long  lPlevel = 0;
	 unsigned long  lType = 0;
	 unsigned long  lShopid = 0;
	 unsigned long  lPlateid = 0;
	 char           caDescr[255] = "";
	 
	 unsigned long lShopId = 0;
	 dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 3, "key",     UT_TYPE_STRING, sizeof(caKey)-1,     caKey,
	                                "sid",     UT_TYPE_STRING, sizeof(caSid)-1,     caSid,
	                                "plateid", UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0&&strlen(caPlateid)>0){
	 	   iFlag = 1;
			 sprintf(sql, "select name, sgroupid, descr, imgpath, serno, floor, level, plevel, type, shopid, plateid from ncmcomindex where sid=%lu and plateid=%lu", atol(caSid),atol(caPlateid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,31,caName,
		                        UT_TYPE_LONG,4,&lSgroupid,
		                        UT_TYPE_STRING,254,caDescr,
		                        UT_TYPE_STRING,127,caImgpath,
		                        UT_TYPE_STRING,31,caSerno,
		                        UT_TYPE_STRING,31,caFloor,
		                        UT_TYPE_LONG,4,&lLevel,
		                        UT_TYPE_LONG,4,&lPlevel,
		                        UT_TYPE_LONG,4,&lType,
		                        UT_TYPE_LONG,4,&lShopid,
		                        UT_TYPE_LONG,4,&lPlateid);
			  utPltSetCvtHtml(1);
			  utPltPutVar(psDbHead,"key",caKey);
			  utPltPutVar(psDbHead,"sid",caSid);
				utPltPutVar(psDbHead,"name",caName);
		    utPltPutVarF(psDbHead,"typeid","%lu",lSgroupid);
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVar(psDbHead,"descr",caDescr);
		    utPltPutVar(psDbHead,"serno",caSerno);
		    utPltPutVar(psDbHead,"floor",caFloor);
		    utPltPutVarF(psDbHead,"level","%lu",lLevel);
		    utPltPutVarF(psDbHead,"plevel","%lu",lPlevel);
		    utPltPutVarF(psDbHead,"type","%lu",lType);
		    utPltPutVarF(psDbHead,"shopid","%lu",lShopid);
		    utPltPutVarF(psDbHead,"plateid","%lu",atol(caPlateid));
		    
		    utPltPutVarF(psDbHead,"flag","%d",iFlag);
		    utPltSetCvtHtml(0);
	 }else if(strcmp(caKey, "add")==0&&strlen(caPlateid)>0){
	 	    utPltPutVar(psDbHead,"key",caKey);
	 	    utPltPutVarF(psDbHead,"plateid","%lu",atol(caPlateid));
	 	    utPltPutVarF(psDbHead,"flag","%d",iFlag);
	 	    if(lShopId>0){
	 	    	 level = 5;
	 	    	 utPltPutVarF(psDbHead,"level","%lu",level);
	 	    }else{
	 	    	 utPltPutVarF(psDbHead,"level","%lu",level);
	 	    }
	 }
	 
	 //utPltShowDb(psDbHead);
	 if(atol(caPlateid)==1){
	 			utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex_dr.htm");	
	 }else if(atol(caPlateid)==2){
	 		  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex_dr1.htm");
	 }else if(atol(caPlateid)==3){
	 			utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex_dr2.htm");	
	 }else{
	 			utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex_dr3.htm");	
	 }
	 
	 return 0;		
}

int ncm_webcomindex_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  
	  int  iFlag=0;
	  char caPos[32]="";
	  char caImgPath[128]="";
	  char caDispImgPath[128]="";
	  char imgname[128]="";
	  char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
 		char caTemp[1024]="";
	  char *p=NULL;
    
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",(char *)ncLang("0194����"),(char *)ncLang("0183���ļ�����"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// ����ͼƬ���·��
  	sprintf(caImgPath,"%s/images/ShopImages/indeximages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);

	  utMsgGetSomeNVar(psMsgHead,2,"fileField", UT_TYPE_STRING, 255, imp_file,
	  														 "position",  UT_TYPE_STRING, sizeof(caPos)-1, caPos);
	  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*��ָ�����ַ�����ȡһ����
		  	
		  	  ����ԭ�ͣ�
			����char *utStrGetWord(char *p,char *pOut,
			����                   short nMaxLen,char *pTerm);
			����
					����˵��:
			����p �C �����ַ���
			����pOut --- ����ַ���������Ҫ���ȷ���ÿռ�
			����nMaxLen --- ����ַ�������󳤶�
			����pTerm  ----- ��ֹ�������ַ�����ϵͳ��ɨ����ʱ�������ַ���ĳһ���ַ�ʱ������
			����
			��������ֵ��
			���������ַ���ָ�롣*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
		  	printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
				}else{
						sprintf(imgname, "index_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/indeximages/");
				    strcat(caDispImgPath, imgname);	
				}
		}
		
		utPltPutVarF(psDbHead,"flag","%d",iFlag);
		utPltPutVar(psDbHead,"position",caPos);
	  utPltPutVar(psDbHead,"filepath",caDispImgPath);
	  printf("position = %s\n", caPos);
	  printf("imgpath = %s\n", caDispImgPath);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex_closeFile.htm");
    return 0;
}

// �ϴ����ͼƬ
int ncm_webcomindex_update_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead = utPltInitDb();
    
    char caKey[32]="";
   	char caSid[16]="";
    char caName[32]="";
    char caSgroupid[32] = "";
    char caDescr[128]="";
    char caImgpath[128]="";
    char caSerno[32]="";
    char caFloor[32]="";
    char caLevel[20]="";
    char caPlevel[20]="";
    char caType[20]="";
    char caShopid[32] = "";
    char caPlateid[16] = "";
    
 		char caTemp[1024]="";
 		char caTemp1[1024]="";
 		char caTemp2[1024]="";
 		char caTemp3[1024]="";
 		char caImgpath1[128]="";
 		char temp_file[129]="";
 		unsigned long lCount = 0;
 		unsigned long lMaxid = 0;
 		
	  utMsgGetSomeNVar(psMsgHead,13,"key",       UT_TYPE_STRING,  sizeof(caKey)-1,            caKey,
	                               "sid",        UT_TYPE_STRING,  sizeof(caSid)-1,            caSid,
	                               "name",       UT_TYPE_STRING,  sizeof(caName)-1,           caName,
	  														 "typeid",     UT_TYPE_STRING,  sizeof(caSgroupid)-1,       caSgroupid,
	  														 "serno",      UT_TYPE_STRING,  sizeof(caSerno)-1,          caSerno,
	  														 "floor",      UT_TYPE_STRING,  sizeof(caFloor)-1,          caFloor,
	  														 "level",      UT_TYPE_STRING,  sizeof(caLevel)-1,          caLevel,
	  														 "plevel",     UT_TYPE_STRING,  sizeof(caPlevel)-1,         caPlevel,
	  														 "type",       UT_TYPE_STRING,  sizeof(caType)-1,           caType,
	  														 "shopid",     UT_TYPE_STRING,  sizeof(caShopid)-1,         caShopid,
	  														 "plateid",    UT_TYPE_STRING,  sizeof(caPlateid)-1,        caPlateid,
																 "filepath",   UT_TYPE_STRING,  sizeof(caImgpath)-1,        caImgpath,
																 "descr",      UT_TYPE_STRING,  sizeof(caDescr)-1,          caDescr);
	  
	  if(strlen(caName)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","�̱����Ʋ���Ϊ��!","�̱����Ʋ���Ϊ��!");
        return 0;	
	  }
	  
	  if(strcmp(caType, "���")==0){
	  		strcpy(caType, "1");	
	  }else if(strcmp(caType, "Ʒ��")==0){
	  		strcpy(caType, "2");	
	  }else if(strcmp(caType, "����")==0){
	  		strcpy(caType, "3");	
	  }else if(strcmp(caType, "��Ƶ")==0){
	  		strcpy(caType, "4");	
	  }else{
	  	  strcpy(caType, "1");	
	  }
	  
	  sprintf(caTemp3, "select floor from ncmshop where id = %lu", atol(caShopid));
	  pasDbOneRecord(caTemp3,0,UT_TYPE_STRING,sizeof(caFloor)-1,caFloor);
	  
	  sprintf(caTemp1, "select count(*) from ncmcomindex where level=%d and shopid=%lu", atol(caLevel), atol(caShopid));
	  pasDbOneRecord(caTemp1,0,UT_TYPE_LONG,4,&lCount);
	  if(strcmp(caKey, "add")==0){
	  	  if(lCount){
	  	  	  // ɾ��ԭλ�õ�ͼƬ
	  	  	  sprintf(caTemp2, "select imgpath from ncmcomindex where level=%lu and shopid=%lu", atol(caLevel),atol(caShopid));
	  	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  	  sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
				  	system(temp_file);
				  	
	  	  	  sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu where level=%lu and plateid=%lu and shopid=%lu",caName,atol(caSgroupid),caDescr,caImgpath,caSerno,caFloor,atol(caPlevel),atol(caType),time(0),atol(caShopid),atol(caPlateid),atol(caLevel),atol(caPlateid),atol(caShopid));	
			  		printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }else{
	  	  	  pasDbOneRecord("select max(sid) from ncmcomindex", 0, UT_TYPE_ULONG, 4, &lMaxid);
				    sprintf(caTemp,"insert into ncmcomindex (sid,name,sgroupid,descr,imgpath,serno,floor,level,plevel,type,systime,shopid,plateid) values (%lu,'%s',%lu,'%s','%s','%s','%s',%lu,%lu,%lu,%lu,%lu,%lu)",(lMaxid+1),caName,atol(caSgroupid),caDescr,caImgpath,caSerno,caFloor,atol(caLevel),atol(caPlevel),atol(caType),time(0),atol(caShopid),atol(caPlateid));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }
	  }else if(strcmp(caKey, "update")==0){
	  	  if(lCount){
	  	  	  // ɾ��ԭλ�õ�ͼƬ
	  	  	  sprintf(caTemp2, "select imgpath from ncmcomindex where level=%lu and shopid=%lu", atol(caLevel),atol(caShopid));
	  	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  	  if(strcmp(caImgpath, caImgpath1) != 0){
			  	  	  sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
						  	system(temp_file);
				    }
				  	
	  	  	  sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu where level=%lu and plateid=%lu and shopid=%lu",caName,atol(caSgroupid),caDescr,caImgpath,caSerno,caFloor,atol(caPlevel),atol(caType),time(0),atol(caShopid),atol(caPlateid),atol(caLevel),atol(caPlateid),atol(caShopid));	
			  		printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }else{
	  	  		sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', level=%lu, plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu where sid=%lu and plateid=%lu",caName,atol(caSgroupid),caDescr,caImgpath,caSerno,caFloor,atol(caLevel),atol(caPlevel),atol(caType),time(0),atol(caShopid),atol(caPlateid),atol(caSid),atol(caPlateid));	
			  		printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }
	  }
	  utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","���ݼ��سɹ�!","���ݼ��سɹ�!");
    return 0;
}

// Ʒ����Ϣ
int ncm_webcombrand_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char sid[20] = "";
	 char name[32] = "";
	 char sgroupname[32] = "";
	 char descr[128] = "";
	 char imgpath[128] = "";
	 char serno[32] = "";
	 char floor[32] = "";
	 char level[20] = "";
	 char plevel[20] = "";
	 char type[20] = "";
	 char systime[32] = "";
	 char shopid[32] = "";
	 char shopname[32] = "";
	 char plateid[20] = "";
	 char flags[32] = "";
	 char temp_file[128] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lSid = 0;
	 char 					caName[32] = "";
	 char 					caSgroupname[32] = "";
	 char 					caDescr[128] = "";
	 char 					caImgpath[128] = "";
	 char 					caSerno[32] = "";
	 char 					caFloor[32] = "";
	 unsigned long	lLevel = 0;
	 unsigned long	lPlevel = 0;
	 unsigned long	lType = 0;
	 unsigned long	lSystime = 0;
	 unsigned long	lShopid = 0;
	 char	          caShopname[32] = "";
	 unsigned long	lPlateid = 0;
	 unsigned long	lFlags = 0;
	 char           s_type[32] = "";
	 char           s_systime[32] = "";
	 
	 char caUpdate[16] = "";
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
	 unsigned long lSysTime = 0;
	 
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
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 24,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
																	"name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
																	"sgroupname",  UT_TYPE_STRING,  sizeof(sgroupname)-1,     sgroupname,
																	"descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"serno",       UT_TYPE_STRING,  sizeof(serno)-1,          serno,
																	"floor",       UT_TYPE_STRING,  sizeof(floor)-1,          floor,
																	"level",       UT_TYPE_STRING,  sizeof(level)-1,          level,
																	"plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
																	"type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
																	"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
																	"shopid",      UT_TYPE_STRING,  sizeof(shopid)-1,         shopid,
																	"shopname",    UT_TYPE_STRING,  sizeof(shopname)-1,       shopname,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"update",      UT_TYPE_STRING,  10,                       caUpdate,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport);
	 
	 trim(_keyword);
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,32);
			strcpy(name,caTemp);
   }
   if(strlen(sgroupname)>0){
			pasCvtGBK(2,sgroupname,caTemp,32);
			strcpy(sgroupname,caTemp);
   }
	 if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,128);
			strcpy(descr,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,128);
			strcpy(imgpath,caTemp);
   }
   if(strlen(serno)>0){
			pasCvtGBK(2,serno,caTemp,32);
			strcpy(serno,caTemp);
   }
   if(strlen(floor)>0){
			pasCvtGBK(2,floor,caTemp,32);
			strcpy(floor,caTemp);
   }
   if(strlen(systime)>0){
			pasCvtGBK(2,systime,caTemp,32);
			strcpy(systime,caTemp);
   }
   if(strlen(shopname)>0){
			pasCvtGBK(2,shopname,caTemp,32);
			strcpy(shopname,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid));
			strcpy(caSesid,caTemp);
   }
   
   // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
    	memset(caTemp, 0, sizeof(caTemp));
    	char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcomindex where sid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
				      return 0;	
					}
					
					// pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
					iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmcomindex where sid in(%s)",caSesid);
    	}
	 } 
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmcomindex where 1=1 and type=2");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(shopid)) // �����̵��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and shopid = %lu ", atol(shopid));
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %lu\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select i.sid, i.name, t.name, i.descr, i.imgpath, i.serno, i.floor, i.level, i.plevel, i.type, i.systime, i.shopid, s.name, i.flags from ncmcomindex i left join ncmcomtype t on i.sgroupid = t.sid left join ncmshop s on i.shopid = s.id  where 1=1 and i.type=2");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (i.name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(shopid)) // �����̵��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and i.shopid = %lu ", atol(shopid));
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and i.plateid = %lu ", atol(plateid));
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by i.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
													     UT_TYPE_STRING, sizeof(caName)-1,      caName,
													     UT_TYPE_STRING, sizeof(caSgroupname)-1,caSgroupname,
													     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
													     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
													     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
													     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lType,
													     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
													     UT_TYPE_ULONG,  sizeof(long),          &lShopid,
													     UT_TYPE_STRING, sizeof(caShopname)-1,  caShopname,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
		   
		   if(lSystime>0){
		   		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
		   }else{
		   	  strcpy(s_systime, "");
		   }
		   
			 switch(lType){
					case 1:strcpy(s_type, "���");break;
					case 2:strcpy(s_type, "Ʒ��");break;
					case 3:strcpy(s_type, "����");break;
					case 4:strcpy(s_type, "��Ƶ");break;
					default:break;	
			 }
			 utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"sgroupname",iNum,caSgroupname);
			 utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"serno",iNum,caSerno);
			 utPltPutLoopVar(psDbHead,"floor",iNum,caFloor);
			 utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",lLevel);
			 utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",lPlevel);
			 utPltPutLoopVarF(psDbHead,"type",iNum,s_type);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu",lShopid);
			 utPltPutLoopVar(psDbHead,"shopname",iNum,caShopname);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltSetCvtHtml(0);
			 lSid = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caSgroupname, 0, sizeof(caSgroupname));
			 memset(caDescr, 0, sizeof(caDescr));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 memset(caSerno, 0, sizeof(caSerno));
			 memset(caFloor, 0, sizeof(caFloor));
			 lLevel = 0;
			 lPlevel = 0;
			 lType = 0;
			 lSystime = 0;
			 lShopid = 0;
			 memset(caShopname, 0, sizeof(caShopname));
			 lFlags = 0;
			 memset(s_type, 0, sizeof(s_type));
			 memset(s_systime, 0, sizeof(s_systime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
															     UT_TYPE_STRING, sizeof(caName)-1,      caName,
															     UT_TYPE_STRING, sizeof(caSgroupname)-1,caSgroupname,
															     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
															     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
															     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
															     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lType,
															     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
															     UT_TYPE_ULONG,  sizeof(long),          &lShopid,
															     UT_TYPE_STRING, sizeof(caShopname)-1,  caShopname,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcombrand_list.htm");
	  return 0;	
}

int ncm_webcombrand_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
//#endif  
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 char           caKey[32] = "";
	 char           caSid[32] = "";
	 char 					caName[32] = "";
	 unsigned long	lSgroupid = 0;
	 char 					caImgpath[128] = "";
	 char 					caSerno[32] = "";
	 char 					caFloor[32] = "";
	 unsigned long  lLevel = 0;
	 unsigned long  lPlevel = 0;
	 unsigned long  lType = 0;
	 unsigned long  lShopid = 0;
	 char           caDescr[255] = "";
	 char           caPlateid[20] = "";
	 unsigned long  lPlateid = 0;
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 3, "key",     UT_TYPE_STRING, sizeof(caKey)-1,     caKey,
	                                "sid",     UT_TYPE_STRING, sizeof(caSid)-1,     caSid,
	                                "plateid", UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
			 sprintf(sql, "select name, sgroupid, descr, imgpath, serno, floor, level, plevel, type, shopid, plateid from ncmcomindex where sid=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,sizeof(caName)-1,caName,
		                        UT_TYPE_LONG,sizeof(long),&lSgroupid,
		                        UT_TYPE_STRING,sizeof(caDescr)-1,caDescr,
		                        UT_TYPE_STRING,sizeof(caImgpath)-1,caImgpath,
		                        UT_TYPE_STRING,sizeof(caSerno)-1,caSerno,
		                        UT_TYPE_STRING,sizeof(caFloor)-1,caFloor,
		                        UT_TYPE_LONG,sizeof(long),&lLevel,
		                        UT_TYPE_LONG,sizeof(long),&lPlevel,
		                        UT_TYPE_LONG,sizeof(long),&lType,
		                        UT_TYPE_LONG,sizeof(long),&lShopid,
		                        UT_TYPE_LONG,sizeof(long),&lPlateid);
			  utPltSetCvtHtml(1);
			  utPltPutVar(psDbHead,"sid",caSid);
				utPltPutVar(psDbHead,"name",caName);
				utPltPutVarF(psDbHead,"sgroupid","%lu",lSgroupid);
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVar(psDbHead,"descr",caDescr);
		    utPltPutVar(psDbHead,"serno",caSerno);
		    utPltPutVar(psDbHead,"floor",caFloor);
		    utPltPutVarF(psDbHead,"level","%lu",lLevel);
		    utPltPutVarF(psDbHead,"plevel","%lu",lPlevel);
		    utPltPutVarF(psDbHead,"type","%lu",lType);
		    utPltPutVarF(psDbHead,"shopid","%lu",lShopid);
		    utPltPutVarF(psDbHead,"plateid","%lu",lPlateid);
		    utPltSetCvtHtml(0);
	 }else{
	 			utPltPutVarF(psDbHead,"plateid","%lu",atol(caPlateid));	
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcombrand_dr.htm");
	 return 0;		
}

// �ϴ�Ʒ����ϢͼƬ
int ncm_webComBrandImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32] = "";
   	char caSid[16] = "";
    char caName[32]="";
    char caSgroupid[32] = "";
    char caDescr[128]="";
    char caImgPath[128]="";
    char caDispImgPath[128]="";
    char imgname[128]="";
    char caSerno[32]="";
    char caFloor[32]="";
    char caLevel[20]="";
    char caPlevel[20]="";
    char caType[20]="";
    char caShopid[32] = "";
    char caPlateid[20] = "";
    
    char caImgpath1[128]="";
    char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
 		char caTemp2[1024]="";
 		unsigned long lSysTime=0;
 		char *p;
 		
 		unsigned long lMaxid = 0;
		char caShopTemp[4096] = "";
		unsigned long lShopid = 0;
 		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
		
		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",(char *)ncLang("0194����"),(char *)ncLang("0183���ļ�����"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// ����ͼƬ���·��
  	sprintf(caImgPath,"%s/images/ShopImages/brandimages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);
        
	  utMsgGetSomeNVar(psMsgHead,13,"key",        UT_TYPE_STRING,  sizeof(caKey)-1,           caKey,
	  														 "sid",        UT_TYPE_STRING,  sizeof(caSid)-1,            caSid,
	                               "name",       UT_TYPE_STRING,  sizeof(caName)-1,           caName,
	  														 "tyid",       UT_TYPE_STRING,  sizeof(caSgroupid)-1,       caSgroupid,
	  														 "serno",      UT_TYPE_STRING,  sizeof(caSerno)-1,          caSerno,
	  														 "floor",      UT_TYPE_STRING,  sizeof(caFloor)-1,          caFloor,
	  														 "level",      UT_TYPE_STRING,  sizeof(caLevel)-1,          caLevel,
	  														 "plevel",     UT_TYPE_STRING,  sizeof(caPlevel)-1,         caPlevel,
	  														 "type",       UT_TYPE_STRING,  sizeof(caType)-1,           caType,
	  														 "shid",       UT_TYPE_STRING,  sizeof(caShopid)-1,         caShopid,
																 "fname",      UT_TYPE_STRING,  sizeof(imp_file)-1,         imp_file,
																 "descr",      UT_TYPE_STRING,  sizeof(caDescr)-1,          caDescr,
																 "plateid",    UT_TYPE_STRING,  sizeof(caPlateid)-1,        caPlateid);
	  
	  if(strlen(caName)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","��ƷС�����Ʋ���Ϊ��!","��ƷС�����Ʋ���Ϊ��!");
        return 0;	
	  }
	                  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*��ָ�����ַ�����ȡһ����
		  	
		  	  ����ԭ�ͣ�
			����char *utStrGetWord(char *p,char *pOut,
			����                   short nMaxLen,char *pTerm);
			����
					����˵��:
			����p �C �����ַ���
			����pOut --- ����ַ���������Ҫ���ȷ���ÿռ�
			����nMaxLen --- ����ַ�������󳤶�
			����pTerm  ----- ��ֹ�������ַ�����ϵͳ��ɨ����ʱ�������ַ���ĳһ���ַ�ʱ������
			����
			��������ֵ��
			���������ַ���ָ�롣*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
				printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
					  memset(caDispImgPath, 0, sizeof(caDispImgPath));
				}else{
						sprintf(imgname, "brand_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/brandimages/");
				    strcat(caDispImgPath, imgname);	
				}
	  }
	  
	  sprintf(caShopTemp, "select floor from ncmshop where id=%lu", atol(caShopid));
	  pasDbOneRecord(caShopTemp,0,UT_TYPE_STRING,sizeof(caFloor)-1,caFloor);
	  
	  if(strcmp(caType, "���")==0){
	  		strcpy(caType, "1");	
	  }else if(strcmp(caType, "Ʒ��")==0){
	  		strcpy(caType, "2");	
	  }else if(strcmp(caType, "����")==0){
	  		strcpy(caType, "3");	
	  }else if(strcmp(caType, "��Ƶ")==0){
	  		strcpy(caType, "4");	
	  }else{
	  	  strcpy(caType, "2");	
	  }
	  
	  if(strcmp(caKey, "update")==0){
	  	  sprintf(caTemp2, "select imgpath from ncmcomindex where sid=%lu", atol(caSid));
	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  if(strlen(caDispImgPath)==0){ // �Ƿ�ѡ���ϴ�ͼƬ, ��ԭͼƬ����
	  	  	  sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', level=%lu, plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu where sid=%lu",caName,atol(caSgroupid),caDescr,caImgpath1,caSerno,caFloor,atol(caLevel),atol(caPlevel),atol(caType),time(0),atol(caShopid),atol(caPlateid),atol(caSid));	
			  		printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caImgpath1);
	  	  }else{ // �ǣ�ԭͼƬɾ������ͼƬ����
			  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
				  	system(temp3_file);
			  	  
			  	  sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', level=%lu, plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu where sid=%lu",caName,atol(caSgroupid),caDescr,caDispImgPath,caSerno,caFloor,atol(caLevel),atol(caPlevel),atol(caType),time(0),atol(caShopid),atol(caPlateid),atol(caSid));	
			  		printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
			  }
	  }else if(strcmp(caKey, "add")==0){
				pasDbOneRecord("select max(sid) from ncmcomindex", 0, UT_TYPE_ULONG, 4, &lMaxid);
		    sprintf(caTemp,"insert into ncmcomindex (sid,name,sgroupid,descr,imgpath,serno,floor,level,plevel,type,systime,shopid,plateid) values (%lu,'%s',%lu,'%s','%s','%s','%s',%lu,%lu,%lu,%lu,%lu,%lu)",(lMaxid+1),caName,atol(caSgroupid),caDescr,caDispImgPath,caSerno,caFloor,atol(caLevel),atol(caPlevel),atol(caType),time(0),atol(caShopid),atol(caPlateid));
			 	printf("caTemp = %s\n", caTemp);
			 	pasDbExecSqlF(caTemp);
			 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
	  }
	  
	  utPltPutVar(psDbHead,"mesg","���ݼ��سɹ�");
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcombrand_dr_list.htm");
	  //utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","���ݼ��سɹ�!","���ݼ��سɹ�!");
    return 0;
}

// ��Ʒ��Ϣ
int ncm_webcomodity_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	// utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char sid[20] = "";
	 char name[128] = "";
	 char dispname[128] = "";
	 char servicecode[16] = "";
	 char indexname[32] = "";
	 char tyname[32] = "";
	 char imgpath[128] = "";
	 char shgroupid[32] = "";
	 char shopname[32] = "";
	 char shimgpath[128] = "";
	 char factory[32] = "";
	 char type[128] = "";
	 char url[255] = "";
	 char floor[32] = "";
	 char fldetail[255] = "";
	 char fee[20] = "";
	 char discount[20] = "";
	 char code[12] = "";
	 char tel[20] = "";
	 char address[255] = "";
	 char reserve[32] = "";
	 char active[64] = "";
	 char plevel[20] = "";
	 char level[20] = "";
	 char descr[255] = "";
	 char detail[4096] = "";
	 char integral[20] = "";
	 char coupon[20] = "";
	 char atype[20] = "";
	 char systime[20] = "";
	 char flags[20] = "";
	 char plateid[20] = "";
	 char temp_file[128] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lSid = 0;
	 char 					caName[128] = "";
	 char 					caDispname[128] = "";
	 char 					caServicecode[16] = "";
	 char 					caIndexname[32] = "";
	 char 					caTyname[32] = "";
	 unsigned long	lShgroupid = 0;
	 char 					caImgpath[128] = "";
	 char 					caShopname[32] = "";
	 char 					caShimgpath[128] = "";
	 char 					caFactory[32] = "";
	 char 					caType[128] = "";
	 char 					caUrl[255] = "";
	 char 					caFloor[32] = "";
	 char 					caFldetail[255] = "";
	 double         dFee = 0;
	 double         dDiscount = 0;
	 char 					caCode[12] = "";
	 unsigned long  lTel = 0;
	 char 					caAddress[255] = "";
	 unsigned long  lReserve = 0;
	 char 					caActive[64] = "";
	 unsigned long	lPlevel = 0;
	 unsigned long  lLevel = 0;
	 char           caDescr[255] = "";
	 char           caDetail[4096] = "";
	 unsigned long  lIntegral = 0;
	 unsigned long  lCoupon = 0;
	 unsigned long  lAtype = 0;
	 unsigned long  lSystime = 0;
	 unsigned long  lFlags = 0;
	 unsigned long  lPlateid = 0;
	 char           s_systime[32] = "";
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caSesid[1024] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
	 // �ؼ���
	 char _keyword[33] = "";
	 
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
   char caShopids[1024];
   // �õ��̵�ID
   unsigned long lShopId = 0;
	 dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	 strcpy(caShopids,getDsShopids());
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 42,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
																	"name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
																	"dispname",    UT_TYPE_STRING,  sizeof(dispname)-1,       dispname,
																	"servicecode", UT_TYPE_STRING,  sizeof(servicecode)-1,    servicecode,
																	"indexname",   UT_TYPE_STRING,  sizeof(indexname)-1,      indexname,
																	"tyname",      UT_TYPE_STRING,  sizeof(tyname)-1,         tyname,
																	"shgroupid",   UT_TYPE_STRING,  sizeof(shgroupid)-1,      shgroupid,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"shopname",    UT_TYPE_STRING,  sizeof(shopname)-1,       shopname,
																	"shimgpath",   UT_TYPE_STRING,  sizeof(shimgpath)-1,      shimgpath,
																	"factory",     UT_TYPE_STRING,  sizeof(factory)-1,        factory,
																	"type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
																	"url",         UT_TYPE_STRING,  sizeof(url)-1,            url,
																	"floor",       UT_TYPE_STRING,  sizeof(floor)-1,          floor,
																	"fldetail",    UT_TYPE_STRING,  sizeof(fldetail)-1,       fldetail,
																	"fee",         UT_TYPE_STRING,  sizeof(fee)-1,            fee,
																	"discount",    UT_TYPE_STRING,  sizeof(discount)-1,       discount,
																	"code",        UT_TYPE_STRING,  sizeof(code)-1,           code,
																	"tel",         UT_TYPE_STRING,  sizeof(tel)-1,            tel,
																	"address",     UT_TYPE_STRING,  sizeof(address)-1,        address,
																	"reserve",     UT_TYPE_STRING,  sizeof(reserve)-1,        reserve,
																	"active",      UT_TYPE_STRING,  sizeof(active)-1,         active,
																	"plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
																	"level",       UT_TYPE_STRING,  sizeof(level)-1,          level,
																	"descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
																	"detail",      UT_TYPE_STRING,  sizeof(detail)-1,         detail,
																	"integral",    UT_TYPE_STRING,  sizeof(integral)-1,       integral,
																	"coupon",      UT_TYPE_STRING,  sizeof(coupon)-1,         coupon,
																	"atype",       UT_TYPE_STRING,  sizeof(atype)-1,          atype,
																	"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
																	"flags",       UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"update",      UT_TYPE_STRING,  10,                       caUpdate,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport);
	 
	 trim(_keyword);
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,128);
			strcpy(name,caTemp);
   }
	 if(strlen(dispname)>0){
			pasCvtGBK(2,dispname,caTemp,128);
			strcpy(dispname,caTemp);
   }
   if(strlen(indexname)>0){
			pasCvtGBK(2,indexname,caTemp,32);
			strcpy(indexname,caTemp);
   }
   if(strlen(tyname)>0){
			pasCvtGBK(2,tyname,caTemp,32);
			strcpy(tyname,caTemp);
   }
   if(strlen(servicecode)>0){
			pasCvtGBK(2,servicecode,caTemp,16);
			strcpy(servicecode,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,128);
			strcpy(imgpath,caTemp);
   }
   if(strlen(shopname)>0){
			pasCvtGBK(2,shopname,caTemp,32);
			strcpy(shopname,caTemp);
   }
   if(strlen(shimgpath)>0){
			pasCvtGBK(2,shimgpath,caTemp,128);
			strcpy(shimgpath,caTemp);
   }
   if(strlen(factory)>0){
			pasCvtGBK(2,factory,caTemp,32);
			strcpy(factory,caTemp);
   }
   if(strlen(type)>0){
			pasCvtGBK(2,type,caTemp,128);
			strcpy(type,caTemp);
   }
   if(strlen(url)>0){
			pasCvtGBK(2,url,caTemp,255);
			strcpy(url,caTemp);
   }
   if(strlen(floor)>0){
			pasCvtGBK(2,floor,caTemp,32);
			strcpy(floor,caTemp);
   }
   if(strlen(fldetail)>0){
			pasCvtGBK(2,fldetail,caTemp,255);
			strcpy(fldetail,caTemp);
   }
   if(strlen(code)>0){
			pasCvtGBK(2,code,caTemp,12);
			strcpy(code,caTemp);
   }
   if(strlen(address)>0){
			pasCvtGBK(2,address,caTemp,255);
			strcpy(address,caTemp);
   }
   if(strlen(active)>0){
			pasCvtGBK(2,active,caTemp,64);
			strcpy(active,caTemp);
   }
   if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,255);
			strcpy(descr,caTemp);
   }
   if(strlen(detail)>0){
			pasCvtGBK(2,detail,caTemp,4096);
			strcpy(detail,caTemp);
   }
   if(strlen(systime)>0){
			pasCvtGBK(2,systime,caTemp,32);
			strcpy(systime,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid));
			strcpy(caSesid,caTemp);
   }
   
   // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcomodity where sid in(%s)", caSesid);
    			printf("caTemp = %s\n", caTemp);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
				      return 0;	
					}
					
					// pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
    		  iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
    		  
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH, imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmcomodity where sid in(%s)",caSesid);
					pasDbExecSqlF("delete from ncmshoptype where sid in(%s)",caSesid);
    	}
	 }
	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmcomodity where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 if(lShopId>0) // �����̵��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and shgroupid = %lu", lShopId);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
//			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and sgroupid in (%s) ",caShopids);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select o.sid, o.name, o.dispname, o.servicecode, i.dispname, t.name, o.shgroupid, o.imgpath, o.shopname, o.shimgpath, o.factory, o.type, o.url, o.floor, o.fldetail, o.fee, o.discount, o.code, o.tel, o.address, o.reserve, o.active, o.plevel, o.level, o.descr, o.detail, o.integral, o.coupon, o.atype, o.systime, o.flags from ncmcomodity o left join ncsrvgroup i on o.sgroupid = i.groupid left join ncmcomtype t on o.groupid = t.sid where 1=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (o.name like '%%%s%%') ", _keyword);
	 }
	 if(lShopId>0) // �����̵��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and o.shgroupid = %lu", lShopId);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
	//		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and o.plateid = %lu ", atol(plateid));
	 }
	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and sgroupid in (%s) ",caShopids);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by o.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),            &lSid,
													     UT_TYPE_STRING, sizeof(caName)-1,        caName,
													     UT_TYPE_STRING, sizeof(caDispname)-1,    caDispname,
													     UT_TYPE_STRING, sizeof(caServicecode)-1, caServicecode,
													     UT_TYPE_STRING, sizeof(caIndexname)-1,   caIndexname,
													     UT_TYPE_STRING, sizeof(caTyname)-1,      caTyname,
													     UT_TYPE_ULONG,  sizeof(long),            &lShgroupid,
															 UT_TYPE_STRING, sizeof(caImgpath)-1,     caImgpath,
															 UT_TYPE_STRING, sizeof(caShopname)-1,    caShopname,
															 UT_TYPE_STRING, sizeof(caShimgpath)-1,   caShimgpath,
															 UT_TYPE_STRING, sizeof(caFactory)-1,     caFactory,
															 UT_TYPE_STRING, sizeof(caType)-1,        caType,
															 UT_TYPE_STRING, sizeof(caUrl)-1,         caUrl,
															 UT_TYPE_STRING, sizeof(caFloor)-1,       caFloor,
															 UT_TYPE_STRING, sizeof(caFldetail)-1,    caFldetail,
															 UT_TYPE_FLOAT,  sizeof(double),          &dFee,
															 UT_TYPE_FLOAT,  sizeof(double),          &dDiscount,
															 UT_TYPE_STRING, sizeof(caCode)-1,        caCode,
															 UT_TYPE_ULONG,  sizeof(long),            &lTel,
															 UT_TYPE_STRING, sizeof(caAddress)-1,     caAddress,
															 UT_TYPE_ULONG,  sizeof(long),            &lReserve,
															 UT_TYPE_STRING, sizeof(caActive)-1,      caActive,
															 UT_TYPE_ULONG,  sizeof(long),            &lPlevel,
															 UT_TYPE_ULONG,  sizeof(long),            &lLevel,
															 UT_TYPE_STRING, sizeof(caDescr)-1,       caDescr,
															 UT_TYPE_STRING, sizeof(caDetail)-1,      caDetail,
															 UT_TYPE_ULONG,  sizeof(long),            &lIntegral,
															 UT_TYPE_ULONG,  sizeof(long),            &lCoupon,
															 UT_TYPE_ULONG,  sizeof(long),            &lAtype,
															 UT_TYPE_ULONG,  sizeof(long),            &lSystime,
															 UT_TYPE_ULONG,  sizeof(long),            &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
		   
		   if(lSystime>0){
		   		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));	
		   }else{
		   		strcpy(s_systime, "");	
		   }
		   
		   utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
			 utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
			 utPltPutLoopVar(psDbHead,"indexname",iNum,caIndexname);
			 utPltPutLoopVar(psDbHead,"tyname",iNum,caTyname);
			 utPltPutLoopVarF(psDbHead,"shgroupid",iNum,"%lu",lShgroupid);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"shopname",iNum,caShopname);
			 utPltPutLoopVar(psDbHead,"shimgpath",iNum,caShimgpath);
			 utPltPutLoopVar(psDbHead,"factory",iNum,caFactory);
			 utPltPutLoopVar(psDbHead,"type",iNum,caType);
			 utPltPutLoopVar(psDbHead,"url",iNum,caUrl);
			 utPltPutLoopVar(psDbHead,"floor",iNum,caFloor);
			 utPltPutLoopVar(psDbHead,"fldetail",iNum,caFldetail);
			 utPltPutLoopVarF(psDbHead,"fee",iNum,"%.2f",dFee);
			 utPltPutLoopVarF(psDbHead,"discount",iNum,"%.2f",dDiscount);
			 utPltPutLoopVar(psDbHead,"code",iNum,caCode);
			 utPltPutLoopVarF(psDbHead,"tel",iNum,"%lu",lTel);
			 utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
			 utPltPutLoopVarF(psDbHead,"reserve",iNum,"%lu",lReserve);
			 utPltPutLoopVar(psDbHead,"active",iNum,caActive);
			 utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",lPlevel);
			 utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",lLevel);
			 utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
			 utPltPutLoopVar(psDbHead,"detail",iNum,caDetail);
			 utPltPutLoopVarF(psDbHead,"integral",iNum,"%lu",lIntegral);
			 utPltPutLoopVarF(psDbHead,"coupon",iNum,"%lu",lCoupon);
			 utPltPutLoopVarF(psDbHead,"atype",iNum,"%lu",lAtype);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltSetCvtHtml(0);
			 
			 lSid = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caDispname, 0, sizeof(caDispname));
			 memset(caServicecode, 0, sizeof(caServicecode));
			 memset(caIndexname, 0, sizeof(caIndexname));
			 memset(caTyname, 0, sizeof(caTyname));
			 lShgroupid = 0;
			 memset(caImgpath, 0, sizeof(caImgpath));
			 memset(caShopname, 0, sizeof(caShopname));
			 memset(caShimgpath, 0, sizeof(caShimgpath));
			 memset(caFactory, 0, sizeof(caFactory));
			 memset(caType, 0, sizeof(caType));
			 memset(caUrl, 0, sizeof(caUrl));
			 memset(caFloor, 0, sizeof(caFloor));
			 memset(caFldetail, 0, sizeof(caFldetail));
			 dFee = 0;
			 dDiscount = 0;
			 memset(caCode, 0, sizeof(caCode));
			 lTel = 0;
			 memset(caAddress, 0, sizeof(caAddress));
			 lReserve = 0;
			 memset(caActive, 0, sizeof(caActive));
			 lPlevel = 0;
			 lLevel = 0;
			 memset(caDescr, 0, sizeof(caDescr));
			 memset(caDetail, 0, sizeof(caDetail));
			 lIntegral = 0;
			 lCoupon = 0;
			 lAtype = 0;
			 lSystime = 0;
			 lFlags = 0;
			 memset(s_systime, 0, sizeof(s_systime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),            &lSid,
															     UT_TYPE_STRING, sizeof(caName)-1,        caName,
															     UT_TYPE_STRING, sizeof(caDispname)-1,    caDispname,
															     UT_TYPE_STRING, sizeof(caServicecode)-1, caServicecode,
															     UT_TYPE_STRING, sizeof(caIndexname)-1,   caIndexname,
															     UT_TYPE_STRING, sizeof(caTyname)-1,      caTyname,
															     UT_TYPE_ULONG,  sizeof(long),            &lShgroupid,
																	 UT_TYPE_STRING, sizeof(caImgpath)-1,     caImgpath,
																	 UT_TYPE_STRING, sizeof(caShopname)-1,    caShopname,
																	 UT_TYPE_STRING, sizeof(caShimgpath)-1,   caShimgpath,
																	 UT_TYPE_STRING, sizeof(caFactory)-1,     caFactory,
																	 UT_TYPE_STRING, sizeof(caType)-1,        caType,
																	 UT_TYPE_STRING, sizeof(caUrl)-1,         caUrl,
																	 UT_TYPE_STRING, sizeof(caFloor)-1,       caFloor,
																	 UT_TYPE_STRING, sizeof(caFldetail)-1,    caFldetail,
																	 UT_TYPE_FLOAT,  sizeof(double),          &dFee,
																	 UT_TYPE_FLOAT,  sizeof(double),          &dDiscount,
																	 UT_TYPE_STRING, sizeof(caCode)-1,        caCode,
																	 UT_TYPE_ULONG,  sizeof(long),            &lTel,
																	 UT_TYPE_STRING, sizeof(caAddress)-1,     caAddress,
																	 UT_TYPE_ULONG,  sizeof(long),            &lReserve,
																	 UT_TYPE_STRING, sizeof(caActive)-1,      caActive,
																	 UT_TYPE_ULONG,  sizeof(long),            &lPlevel,
																	 UT_TYPE_ULONG,  sizeof(long),            &lLevel,
																	 UT_TYPE_STRING, sizeof(caDescr)-1,       caDescr,
																	 UT_TYPE_STRING, sizeof(caDetail)-1,      caDetail,
																	 UT_TYPE_ULONG,  sizeof(long),            &lIntegral,
																	 UT_TYPE_ULONG,  sizeof(long),            &lCoupon,
																	 UT_TYPE_ULONG,  sizeof(long),            &lAtype,
																	 UT_TYPE_ULONG,  sizeof(long),            &lSystime,
																	 UT_TYPE_ULONG,  sizeof(long),            &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomodity_list.htm");
	  return 0;	
}

int ncm_webcomodity_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
//#endif  
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 char caKey[32] = "";
	 char caSid[32] = "";
	 char caPlateid[20] = "";
	 char caName[128] = "";
	 char caDispname[128] = "";
	 char caScode[16] = "";
	 unsigned long lSgroupid = 0;
	 unsigned long lGroupid = 0;
	 unsigned long lShgroupid = 0;
	 char caImgpath[128] = "";
	 char caFactory[32] = "";
	 char caType[128] = "";
	 double dFee = 0.0;
	 double dDiscount = 0.0;
	 unsigned long lReserve = 0;
	 char caActive[64] = "";
	 unsigned long lPlevel = 0;
	 unsigned long lLevel = 0;
	 char caDescr[255] = "";
	 char caDetail[4096] = "";
	 unsigned long lIntegral = 0;
	 unsigned long lCoupon = 0;
	 unsigned long lAtype = 0;
	 unsigned long lPlateid = 0;
	 
	 // �õ��̵�ID
   unsigned long lShopId = 0;
   unsigned long lFlag = 0;
	 dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 3, "key",     UT_TYPE_STRING, sizeof(caKey)-1,     caKey,
	                                "sid",     UT_TYPE_STRING, sizeof(caSid)-1,     caSid,
	                                "plateid", UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
			 sprintf(sql, "select name, dispname, servicecode, sgroupid, groupid, shgroupid, imgpath, factory, type, fee, discount, reserve, active, plevel, level, descr, detail, integral, coupon, atype, plateid from ncmcomodity where sid=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,sizeof(caName)-1,caName,
			                      UT_TYPE_STRING,sizeof(caDispname)-1,caDispname,
			 											UT_TYPE_STRING,sizeof(caScode)-1,caScode,
			 											UT_TYPE_ULONG,sizeof(long),&lSgroupid,
			 											UT_TYPE_ULONG,sizeof(long),&lGroupid,
			 											UT_TYPE_ULONG,sizeof(long),&lShgroupid,
			 											UT_TYPE_STRING,sizeof(caImgpath)-1,caImgpath,
			 											UT_TYPE_STRING,sizeof(caFactory)-1,caFactory,
			 											UT_TYPE_STRING,sizeof(caType)-1,caType,
			 											UT_TYPE_FLOAT,sizeof(double),&dFee,
			 											UT_TYPE_FLOAT,sizeof(double),&dDiscount,
			 											UT_TYPE_ULONG,sizeof(long),&lReserve,
			 											UT_TYPE_STRING,sizeof(caActive)-1,caActive,
			 											UT_TYPE_ULONG,sizeof(long),&lPlevel,
			 											UT_TYPE_ULONG,sizeof(long),&lLevel,
		                        UT_TYPE_STRING,sizeof(caDescr)-1,caDescr,
		                        UT_TYPE_STRING,sizeof(caDetail)-1,caDetail,
		                        UT_TYPE_ULONG,sizeof(long),&lIntegral,
		                        UT_TYPE_ULONG,sizeof(long),&lCoupon,
		                        UT_TYPE_ULONG,sizeof(long),&lAtype,
		                        UT_TYPE_ULONG,sizeof(long),&lPlateid);
		    utPltSetCvtHtml(1);         
			  utPltPutVar(psDbHead,"sid",caSid);
				utPltPutVar(psDbHead,"name",caName);
				utPltPutVar(psDbHead,"dispname",caDispname);
				utPltPutVar(psDbHead,"scode",caScode);
				utPltPutVarF(psDbHead,"indexid","%lu",lSgroupid);
				utPltPutVarF(psDbHead,"typeid","%lu",lGroupid);
				if(lShopId>0){
	 				 lFlag = 1;
					 utPltPutVarF(psDbHead,"shopid","%lu",lShopId);
					 utPltPutVarF(psDbHead,"flag","%lu",lFlag);		
				}else{
					 lFlag = 0;
					 utPltPutVarF(psDbHead,"shopid","%lu",lShgroupid);
					 utPltPutVarF(psDbHead,"flag","%lu",lFlag);	
				}
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVar(psDbHead,"factory",caFactory);
		    utPltPutVar(psDbHead,"type",caType);
		    utPltPutVarF(psDbHead,"fee","%.2f",dFee);
		    utPltPutVarF(psDbHead,"discount","%.2f",dDiscount);
		    utPltPutVarF(psDbHead,"reserve","%lu",lReserve);
		    utPltPutVar(psDbHead,"active",caActive);
		    utPltPutVarF(psDbHead,"plevel","%lu",lPlevel);
		    utPltPutVarF(psDbHead,"level","%lu",lLevel);
		    utPltPutVar(psDbHead,"descr",caDescr);
		    utPltPutVar(psDbHead,"detail",caDetail);
		    utPltPutVarF(psDbHead,"integral","%lu",lIntegral);
		    utPltPutVarF(psDbHead,"coupon","%lu",lCoupon);
		    utPltPutVarF(psDbHead,"atype","%lu",lAtype);
		    utPltPutVarF(psDbHead,"plateid","%lu",lPlateid);
		    utPltSetCvtHtml(0);
	 }else{
	 	    utPltSetCvtHtml(1);
	 			utPltPutVarF(psDbHead,"plateid","%lu",atol(caPlateid));
	 			if(lShopId>0){
	 				 lFlag = 1;
					 utPltPutVarF(psDbHead,"shopid","%lu",lShopId);
					 utPltPutVarF(psDbHead,"flag","%lu",lFlag);		
				}else{
					 lFlag = 0;
					 utPltPutVar(psDbHead,"shopid","");
					 utPltPutVarF(psDbHead,"flag","%lu",lFlag);	
				}
				utPltSetCvtHtml(0);
	 }
	 
	 utPltPutVar(psDbHead,"key",caKey);
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomodity_dr.htm");
	 return 0;		
}

int ncm_webcomodity_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  
	  char caImgPath[128]="";
	  char caDispImgPath[128]="";
	  char imgname[128]="";
	  char imp_file[256]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
	  char *p=NULL;
	  char *q=NULL;
    
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",(char *)ncLang("0194����"),(char *)ncLang("0183���ļ�����"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// ����ͼƬ���·��
  	sprintf(caImgPath,"%s/images/ShopImages/odityimages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);

	  utMsgGetSomeNVar(psMsgHead,1,"fileField", UT_TYPE_STRING, 255, imp_file);
	  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*��ָ�����ַ�����ȡһ����
		  	
		  	  ����ԭ�ͣ�
			����char *utStrGetWord(char *p,char *pOut,
			����                   short nMaxLen,char *pTerm);
			����
					����˵��:
			����p �C �����ַ���
			����pOut --- ����ַ���������Ҫ���ȷ���ÿռ�
			����nMaxLen --- ����ַ�������󳤶�
			����pTerm  ----- ��ֹ�������ַ�����ϵͳ��ɨ����ʱ�������ַ���ĳһ���ַ�ʱ������
			����
			��������ֵ��
			���������ַ���ָ�롣*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
		  	printf("temp_file = %s\n", temp_file);
		  	strcpy(temp3_file, temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
				}else{
					  q = strtok(temp3_file, ".");
					  //if(q)printf("%s\n", q);
					  q = strtok(NULL, ".");
					  //if(q)printf("%s\n", q);
					  srand((unsigned int)time(0));
						sprintf(imgname, "odity_%lu_%lu.%s", time(0), rand()%100, q);
						//printf("imgname = %s\n", imgname);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
						//printf("temp2_file = %s\n", temp2_file);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/odityimages/");
				    strcat(caDispImgPath, imgname);
				}
		}
		
	  utPltPutVar(psDbHead,"filepath",caDispImgPath);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomodity_closeFile.htm");
    return 0;
}

// �ϴ���ƷͼƬ
int ncm_webComOdityImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32]="";
    char caSid[20]="";
    char caName[128]="";
    char caDispname[128]="";
    char caSgroupid[32]="";
    char caGroupid[32]="";
    char caShgroupid[32]="";
    char caFactory[32]="";
    char caType[128]="";
    char caFee[32]="";
    char caDiscount[32]="";
    char caReserve[32]="";
    char caActive[32]="";
    char caPlevel[20]="";
    char caLevel[20]="";
    char caIntegral[20]="";
    char caCoupon[20]="";
    char caAtype[20]="";
    char caImgPath[128]="";
    char caDescr[255]="";
    char caDetail[4096]="";
    char caImgpath[128]="";
    char imgname[128]="";
    char caPlateid[20]="";
    
    char caShopname[32] = "";
    char caScode[16] = "";
    unsigned long lSgroupid = 0;
	  char caShopImgPath[128] = "";
	  char caUrl[255] = "";
	  char caFloor[32] = "";
	  char caCode[32] = "";
	  char caTel[32] = "";
	  char caAddress[255] = "";
	  unsigned long lLevel = 0;
	  char caShopDescr[255] = "";
	  unsigned long lAreaid = 0;
	  
	  char caFloorDetail[255] = "";
    
    char caImgpath1[128]="";
    char temp_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
 		char caTemp2[1024]="";
 		char caTemp3[1024]="";
 		char caTemp4[1024]="";
 		char caCompTemp[1024]="";
 		unsigned long lCount=0;
 		unsigned long lMaxid = 0;
 		unsigned long lMaxcompid = 0;
 		unsigned long lCount1 = 0;
 		unsigned long lId = 0;
 		unsigned long lSid = 0;
 		char *p;
 		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
		
		psDbHead = utPltInitDb();
        
	  utMsgGetSomeNVar(psMsgHead,22,"key",         UT_TYPE_STRING,  sizeof(caKey)-1,            caKey,
	  														  "sid",         UT_TYPE_STRING,  sizeof(caSid)-1,            caSid,
	  														  "name",        UT_TYPE_STRING,  sizeof(caName)-1,           caName,
	  														  "dispname",    UT_TYPE_STRING,  sizeof(caDispname)-1,       caDispname,
	                                "inid",        UT_TYPE_STRING,  sizeof(caSgroupid)-1,       caSgroupid,
	                                "tyid",        UT_TYPE_STRING,  sizeof(caGroupid)-1,        caGroupid,
	                                "shid",        UT_TYPE_STRING,  sizeof(caShgroupid)-1,      caShgroupid,
	                                "factory",     UT_TYPE_STRING,  sizeof(caFactory)-1,        caFactory,
	                                "type",        UT_TYPE_STRING,  sizeof(caType)-1,           caType,
	                                "fee",         UT_TYPE_STRING,  sizeof(caFee)-1,            caFee,
	                                "discount",    UT_TYPE_STRING,  sizeof(caDiscount)-1,       caDiscount,
	                                "reserve",     UT_TYPE_STRING,  sizeof(caReserve)-1,        caReserve,
	                                "active",      UT_TYPE_STRING,  sizeof(caActive)-1,         caActive,
	                                "plevel",      UT_TYPE_STRING,  sizeof(caPlevel)-1,         caPlevel,
	                                "level",       UT_TYPE_STRING,  sizeof(caLevel)-1,          caLevel,
	                                "integral",    UT_TYPE_STRING,  sizeof(caIntegral)-1,       caIntegral,
	                                "coupon",      UT_TYPE_STRING,  sizeof(caCoupon)-1,         caCoupon,
	                                "atype",       UT_TYPE_STRING,  sizeof(caAtype)-1,          caAtype,
																  "imgpath",     UT_TYPE_STRING,  sizeof(caImgpath)-1,        caImgpath,
																  "descr",       UT_TYPE_STRING,  sizeof(caDescr)-1,          caDescr,
																  "detail",      UT_TYPE_STRING,  sizeof(caDetail)-1,         caDetail,
																  "plateid",     UT_TYPE_STRING,  sizeof(caPlateid)-1,        caPlateid);
  //  unsigned long lSgroupid;
    
    
	  // ����ת��
	  if(strlen(caName)>0){
				pasCvtGBK(2,caName,caTemp,128);
				strcpy(caName,caTemp);
    }
    if(strlen(caDispname)>0){
				pasCvtGBK(2,caDispname,caTemp,128);
				strcpy(caDispname,caTemp);
    }
    if(strlen(caFactory)>0){
				pasCvtGBK(2,caFactory,caTemp,32);
				strcpy(caFactory,caTemp);
    } 
    if(strlen(caType)>0){
				pasCvtGBK(2,caType,caTemp,128);
				strcpy(caType,caTemp);
    }
    if(strlen(caActive)>0){
				pasCvtGBK(2,caActive,caTemp,128);
				strcpy(caActive,caTemp);
    }
    if(strlen(caImgpath)>0){
				pasCvtGBK(2,caImgpath,caTemp,128);
				strcpy(caImgpath,caTemp);
    }
    if(strlen(caDescr)>0){
				pasCvtGBK(2,caDescr,caTemp,128);
				strcpy(caDescr,caTemp);
    }
    if(strlen(caDetail)>0){
				pasCvtGBK(2,caDetail,caTemp,4096);
				strcpy(caDetail,caTemp);
    }
    
    strcpy(caName, pasDbReplaceQuote(caName));
    strcpy(caDispname, pasDbReplaceQuote(caDispname));
    //printf("caName = %s\n", caName);
	  
	  sprintf(caTemp,"select name, service_code, sgroupid, imgpath, url, floor, code, tel, address, level, descr, areaid from ncmshop where id = %lu", atol(caShgroupid));
		printf("caTemp = %s\n", caTemp);
		pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, sizeof(caShopname)-1,     caShopname,
														  UT_TYPE_STRING, sizeof(caScode)-1,        caScode,
														  UT_TYPE_ULONG,  sizeof(long),             &lSgroupid,
														  UT_TYPE_STRING, sizeof(caShopImgPath)-1,  caShopImgPath,
														  UT_TYPE_STRING, sizeof(caUrl)-1,          caUrl,
														  UT_TYPE_STRING, sizeof(caFloor)-1,        caFloor,
														  UT_TYPE_STRING, sizeof(caCode)-1,         caCode,
														  UT_TYPE_STRING, sizeof(caTel)-1,          caTel,
														  UT_TYPE_STRING, sizeof(caAddress)-1,      caAddress,
														  UT_TYPE_ULONG,  sizeof(long),             &lLevel,
														  UT_TYPE_STRING, sizeof(caShopDescr)-1,    caShopDescr,
														  UT_TYPE_ULONG,  sizeof(long),             &lAreaid);
												 
		pasDbOneRecord("select detail from ncmmall where name = :caFloor", 1, "caFloor", UT_TYPE_STRING, caFloor, UT_TYPE_STRING, 254, caFloorDetail);
	  
	  
	  sscanf(caSgroupid,"%lu",&lSgroupid);
	  
	  if(strcmp(caKey, "update")==0){
	  	  sprintf(caTemp2, "select imgpath from ncmcomodity where sid=%lu", atol(caSid));
	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  //printf("caImgpath1 = %s\n", caImgpath1);
	  	  if(strcmp(caImgpath, caImgpath1)==0){ // �Ƿ�ѡ���ϴ�ͼƬ, ��ԭͼƬ����
	  	  	  //printf("*****************ԭͼƬ����******************\n");
	  	  	  if(strlen(caDispname)==0){
	  	  	  		sprintf(caTemp,"update ncmcomodity set name='%s',dispname='%s',servicecode='%s',sgroupid=%lu,groupid=%lu,shgroupid=%lu,imgpath='%s',shopname='%s',shimgpath='%s',factory='%s',type='%s',url='%s',floor='%s',fldetail='%s',fee=%.2f,discount=%.2f,code='%s',tel='%s',address='%s',reserve=%lu,active='%s',plevel=%lu,level=%lu,descr='%s',detail='%s',integral=%lu,coupon=%lu,systime=%lu,atype=%lu,plateid=%lu where sid=%lu",caName,caName,caScode,lSgroupid,atol(caGroupid),atol(caShgroupid),caImgpath,caShopname,caShopImgPath,caFactory,caType,caUrl,caFloor,caFloorDetail,atof(caFee),atof(caDiscount),caCode,caTel,caAddress,atol(caReserve),caActive,atol(caPlevel),atol(caLevel),caDescr,caDetail,atol(caIntegral),atol(caCoupon),time(0),atol(caAtype),atol(caPlateid),atol(caSid));
					  }else{
					  		sprintf(caTemp,"update ncmcomodity set name='%s',dispname='%s',servicecode='%s',sgroupid=%lu,groupid=%lu,shgroupid=%lu,imgpath='%s',shopname='%s',shimgpath='%s',factory='%s',type='%s',url='%s',floor='%s',fldetail='%s',fee=%.2f,discount=%.2f,code='%s',tel='%s',address='%s',reserve=%lu,active='%s',plevel=%lu,level=%lu,descr='%s',detail='%s',integral=%lu,coupon=%lu,systime=%lu,atype=%lu,plateid=%lu where sid=%lu",caName,caDispname,caScode,lSgroupid,atol(caGroupid),atol(caShgroupid),caImgpath,caShopname,caShopImgPath,caFactory,caType,caUrl,caFloor,caFloorDetail,atof(caFee),atof(caDiscount),caCode,caTel,caAddress,atol(caReserve),caActive,atol(caPlevel),atol(caLevel),caDescr,caDetail,atol(caIntegral),atol(caCoupon),time(0),atol(caAtype),atol(caPlateid),atol(caSid));	
					  }
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	//utPltPutVar(psDbHead,"imgpath",caImgpath1);
					 	
					 	// ���ձ�
			 			sprintf(caTemp4, "select id from ncmshoptype where sid=%lu", atol(caSid));
			 			pasDbOneRecord(caTemp4, 0, UT_TYPE_ULONG, sizeof(long), &lId);
			 		
			 		  sprintf(caCompTemp, "update ncmshoptype set sgroupid=%lu, sid=%lu, sname='%s', fee=%.2f, discount=%.2f, systime=%lu where id=%lu", atol(caGroupid), atol(caSid), caName, atof(caFee), atof(caDiscount), time(0), lId);
			 			printf("caCompTemp = %s\n", caCompTemp);
			 	    pasDbExecSqlF(caCompTemp);
	  	  }else{ // �ǣ�ԭͼƬɾ������ͼƬ����
	  	  	  //printf("*****************ԭͼƬ�ı�******************\n");
			  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
				  	system(temp3_file);
			  	  
			  	  if(strlen(caDispname)==0){
			  	  		sprintf(caTemp,"update ncmcomodity set name='%s',dispname='%s',servicecode='%s',sgroupid=%lu,groupid=%lu,shgroupid=%lu,imgpath='%s',shopname='%s',shimgpath='%s',factory='%s',type='%s',url='%s',floor='%s',fldetail='%s',fee=%.2f,discount=%.2f,code='%s',tel='%s',address='%s',reserve=%lu,active='%s',plevel=%lu,level=%lu,descr='%s',detail='%s',integral=%lu,coupon=%lu,systime=%lu,atype=%lu,plateid=%lu where sid=%lu",caName,caName,caScode,lSgroupid,atol(caGroupid),atol(caShgroupid),caImgpath,caShopname,caShopImgPath,caFactory,caType,caUrl,caFloor,caFloorDetail,atof(caFee),atof(caDiscount),caCode,caTel,caAddress,atol(caReserve),caActive,atol(caPlevel),atol(caLevel),caDescr,caDetail,atol(caIntegral),atol(caCoupon),time(0),atol(caAtype),atol(caPlateid),atol(caSid));
					  }else{
					  		sprintf(caTemp,"update ncmcomodity set name='%s',dispname='%s',servicecode='%s',sgroupid=%lu,groupid=%lu,shgroupid=%lu,imgpath='%s',shopname='%s',shimgpath='%s',factory='%s',type='%s',url='%s',floor='%s',fldetail='%s',fee=%.2f,discount=%.2f,code='%s',tel='%s',address='%s',reserve=%lu,active='%s',plevel=%lu,level=%lu,descr='%s',detail='%s',integral=%lu,coupon=%lu,systime=%lu,atype=%lu,plateid=%lu where sid=%lu",caName,caDispname,caScode,lSgroupid,atol(caGroupid),atol(caShgroupid),caImgpath,caShopname,caShopImgPath,caFactory,caType,caUrl,caFloor,caFloorDetail,atof(caFee),atof(caDiscount),caCode,caTel,caAddress,atol(caReserve),caActive,atol(caPlevel),atol(caLevel),caDescr,caDetail,atol(caIntegral),atol(caCoupon),time(0),atol(caAtype),atol(caPlateid),atol(caSid));	
					  }
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	//utPltPutVar(psDbHead,"imgpath",caDispImgPath);
					 	
					 	// ���ձ�
			 			sprintf(caTemp4, "select id from ncmshoptype where sid=%lu", atol(caSid));
			 			pasDbOneRecord(caTemp4, 0, UT_TYPE_ULONG, sizeof(long), &lId);
			 		
			 		  sprintf(caCompTemp, "update ncmshoptype set sgroupid=%lu, sid=%lu, sname='%s', fee=%.2f, discount=%.2f, systime=%lu where id=%lu", atol(caGroupid), atol(caSid), caName, atof(caFee), atof(caDiscount), time(0), lId);
			 			printf("caCompTemp = %s\n", caCompTemp);
			 	    pasDbExecSqlF(caCompTemp);
			  }
	  }else if(strcmp(caKey, "add")==0){
	  	  printf("*****************�����ͼƬ******************\n");
				pasDbOneRecord("select max(sid) from ncmcomodity", 0, UT_TYPE_ULONG, 4, &lMaxid);
				if(strlen(caDispname)==0){
						sprintf(caTemp,"insert into ncmcomodity (sid,name,dispname,servicecode,sgroupid,groupid,shgroupid,imgpath,shopname,shimgpath,factory,type,url,floor,fldetail,fee,discount,code,tel,address,reserve,active,plevel,level,descr,detail,integral,coupon,systime,atype,plateid) values (%lu,'%s','%s','%s',%lu,%lu,%lu,'%s','%s','%s','%s','%s','%s','%s','%s',%.2f,%.2f,'%s','%s','%s','%s','%s',%lu,%lu,'%s','%s',%lu,%lu,%lu,%lu,%lu)",(lMaxid+1),caName,caName,caScode,lSgroupid,atol(caGroupid),atol(caShgroupid),caImgpath,caShopname,caShopImgPath,caFactory,caType,caUrl,caFloor,caFloorDetail,atof(caFee),atof(caDiscount),caCode,caTel,caAddress,caReserve,caActive,atol(caPlevel),atol(caLevel),caDescr,caDetail,atol(caIntegral),atol(caCoupon),time(0),atol(caAtype),atol(caPlateid));	
				}else{
						sprintf(caTemp,"insert into ncmcomodity (sid,name,dispname,servicecode,sgroupid,groupid,shgroupid,imgpath,shopname,shimgpath,factory,type,url,floor,fldetail,fee,discount,code,tel,address,reserve,active,plevel,level,descr,detail,integral,coupon,systime,atype,plateid) values (%lu,'%s','%s','%s',%lu,%lu,%lu,'%s','%s','%s','%s','%s','%s','%s','%s',%.2f,%.2f,'%s','%s','%s','%s','%s',%lu,%lu,'%s','%s',%lu,%lu,%lu,%lu,%lu)",(lMaxid+1),caName,caDispname,caScode,lSgroupid,atol(caGroupid),atol(caShgroupid),caImgpath,caShopname,caShopImgPath,caFactory,caType,caUrl,caFloor,caFloorDetail,atof(caFee),atof(caDiscount),caCode,caTel,caAddress,caReserve,caActive,atol(caPlevel),atol(caLevel),caDescr,caDetail,atol(caIntegral),atol(caCoupon),time(0),atol(caAtype),atol(caPlateid));	
				}
			 	printf("caTemp = %s\n", caTemp);
			 	pasDbExecSqlF(caTemp);
			 	//utPltPutVar(psDbHead,"imgpath",caDispImgPath);
			 	
			 	// ���ձ�
			 	pasDbOneRecord("select max(id) from ncmshoptype", 0, UT_TYPE_ULONG, 4, &lMaxcompid);
			 	
	 			sprintf(caTemp4, "select count(sid), sid from ncmshoptype where shopid=%lu group by shopid ", atol(caShgroupid));
	 			pasDbOneRecord(caTemp4, 0, UT_TYPE_ULONG, sizeof(long), &lCount1,
	 			                           UT_TYPE_ULONG, sizeof(long), &lSid);
	 			printf("lCount1 = %lu\n", lCount1);
	 			printf("lSid = %lu\n", lSid);
				if((lSid==0) && (lCount1==1)){
						sprintf(caCompTemp, "update ncmshoptype set sgroupid=%lu, sid=%lu, sname='%s', fee=%.2f, discount=%.2f, systime=%lu where shopid=%lu", atol(caGroupid), (lMaxid+1), caName, atof(caFee), atof(caDiscount), time(0), atol(caShgroupid));
			 			printf("caCompTemp = %s\n", caCompTemp);
			 	    pasDbExecSqlF(caCompTemp);	
				}else{
						sprintf(caCompTemp, "insert into ncmshoptype (id, shopid, shopname, imgpath, sgroupid, sid, sname, fee, discount, descr, systime, plateid, areaid) values (%lu, %lu, '%s', '%s', %lu, %lu, '%s', %.2f, %.2f, '%s', %lu, %lu, %lu)", (lMaxcompid+1), atol(caShgroupid), caShopname, caShopImgPath, atol(caGroupid), (lMaxid+1), caName, atof(caFee), atof(caDiscount), caShopDescr, time(0), atol(caPlateid), lAreaid);
			 			printf("caCompTemp = %s\n", caCompTemp);
			 	    pasDbExecSqlF(caCompTemp);
				}
	  }
	  utPltPutVar(psDbHead,"mesg","���ݼ��سɹ�");
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomodity_dr_list.htm");
	  //utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","���ݼ��سɹ�!","���ݼ��سɹ�!");
    return 0;
}

// ��Ʒ�̵���ձ�
int ncm_webcomcomp_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
		utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
		
		// ��ʼ�����ݿ�
		char sql[4096] = "";
		pasDbCursor *psCur = NULL;
		int iret=0;
		int iNum=0;
		utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��	
		
		// ������Ϣ����
		char id[20] = "";
		char shopid[20] = "";
		char shopname[32] = "";
		char sgroupid[20] = "";
		char level[20] = "";
		char etime[32] = "";
		char sEtime[32] = "";
		char plateid[20] = "";
		char areaid[20] = "";
		
		// ������ݿ�ȡ�������ݵı���
		unsigned long lId = 0;
		unsigned long lShopid = 0;
		char caShopname[32] = "";
		unsigned long lSgroupid = 0;
		char caSgroupname[32] = "";
		unsigned long lLevel = 0;
		unsigned long lEtime = 0;
		char s_etime[32] = "";
		
		// �޸���Ҫ�ı���
		unsigned long uLevel = 0;
		unsigned long uSystime = 0;
		unsigned long uEtime = 0;
		
		char caUpdate[16] = "";
	  char del[16] = "";
	  char caSesid[1024] = "";
	  char caDel[16] = "";
	  char caTemp[1024]="";
	  char caTemp1[1024]="";
	  // �ؼ���
	  char _keyword[33] = "";
	  
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
		
		// ����
		char sort[17] = "";
		char dir[9] = "";
	  
		// utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
		utMsgGetSomeNVar(psMsgHead, 17, "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																		"sid",         UT_TYPE_STRING,  sizeof(id)-1,             id,
																		"shopid",      UT_TYPE_STRING,  sizeof(shopid)-1,         shopid,
																		"shopname",    UT_TYPE_STRING,  sizeof(shopname)-1,       shopname,
																		"sgroupid",    UT_TYPE_STRING,  sizeof(sgroupid)-1,       sgroupid,
																		"level",       UT_TYPE_STRING,  sizeof(level)-1,          level,
																		"etime",       UT_TYPE_STRING,  sizeof(etime)-1,          etime,
																		"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
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
		
		//����ת��
		if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
		}
		if(strlen(shopname)>0){
			pasCvtGBK(2,shopname,caTemp,32);
			strcpy(shopname,caTemp);
		}
		
		// ����
    if(strlen(sort)==0){
	 	   strcpy(sort,"id");
	  }
	  
	  // ʱ��ת��
    ncTimeToStr(etime, sEtime);
   
	  // �޸�
		if(!utStrIsSpaces(caUpdate)) {
				int flag = 1;
			
			  sprintf(caTemp1, "select level, systime, etime from ncmshoptype where sgroupid=%lu", atol(sgroupid));
				printf("caTemp1 = %s\n", caTemp1);
				psCur = pasDbOpenSql(caTemp1, 0);
				if(psCur==NULL){
					 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
				   return 0;	
				}
				
				// pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG,  sizeof(long),            &uLevel,
																     UT_TYPE_ULONG,  sizeof(long),            &uSystime,
																     UT_TYPE_ULONG,  sizeof(long),            &uEtime);
				
				while(0 == iret || 1405 == iret){
						if(uLevel == atol(level)){
								if((uSystime+uEtime)<time(0)){
										flag = 1;	
								}else{
										flag = 0;
								}
						}
						
						uLevel = 0;
						uSystime = 0;
						uEtime = 0;
						iret = pasDbFetchInto(psCur, UT_TYPE_ULONG,  sizeof(long),            &uLevel,
																		     UT_TYPE_ULONG,  sizeof(long),            &uSystime,
																		     UT_TYPE_ULONG,  sizeof(long),            &uEtime);
				}
				pasDbCloseCursor(psCur);
				
				if(flag){
						sprintf(caTemp,"update ncmshoptype set systime=%lu, level=%lu, etime=%lu where id=%lu ",time(0),atol(level),atol(sEtime),atol(id));
						printf("caTemp = %s\n", caTemp);
				 		pasDbExecSqlF(caTemp);
				}else{
					  utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","�˵ȼ��̼��Ѿ�����","�˵ȼ��̼��Ѿ�����");
		   			return 0;
				}
				memset(sgroupid, 0, sizeof(sgroupid));
				memset(shopid, 0, sizeof(shopid));
		}
		
		unsigned long lShopId = 0;
	  dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	  
		// ��ѯ
		strcpy(sql, "select count(*) from ncmshoptype where 1=1");
		if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (shopname like '%%%s%%') ", _keyword);
		}
		if(!utStrIsSpaces(sgroupid)) // ������ƷС���ѯ
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sgroupid = %lu ", atol(sgroupid));
		}
		if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
		}
		if(lShopId>0) // �����̵��ѯ
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and shopid = %lu ", lShopId);
		}
		iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
		lTotRec = lCount1;
		utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
		printf("lTotRec = %d\n", lTotRec);
		printf("lCount1 = %d\n", lCount1);
		
		iStart = atol(start);
		iLimit = atol(limit);
		
		strcpy(sql, "select s.id, s.shopid, s.shopname, s.sgroupid, t.name, s.level, s.etime from ncmshoptype s left join ncmcomtype t on s.sgroupid = t.sid where 1=1");
		// utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
		if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
		{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (s.shopname like '%%%s%%') ", _keyword);
		}
		if(!utStrIsSpaces(sgroupid)) // ������ƷС���ѯ
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and s.sgroupid = %lu ", atol(sgroupid));
		}
		if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and s.plateid = %lu ", atol(plateid));
		}
		if(lShopId>0) // �����̵��ѯ
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and s.shopid = %lu ", lShopId);
		}
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by s.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
		printf("sql = %s\n", sql);
		psCur = pasDbOpenSql(sql, 0);
		if(psCur==NULL){
			 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
		   return 0;	
		}
		
		// pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
		iret = pasDbFetchInto(psCur, UT_TYPE_ULONG,  sizeof(long),            &lId,
																 UT_TYPE_ULONG,  sizeof(long),            &lShopid,
														     UT_TYPE_STRING, sizeof(caShopname)-1,    caShopname,
														     UT_TYPE_ULONG,  sizeof(long),            &lSgroupid,
														     UT_TYPE_STRING, sizeof(caSgroupname)-1,  caSgroupname,
														     UT_TYPE_ULONG,  sizeof(long),            &lLevel,
														     UT_TYPE_ULONG,  sizeof(long),            &lEtime);
														   
		while(0 == iret || 1405 == iret){
				iNum++;
				if(iNum > 1){
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
				}
				utPltSetCvtHtml(1);
				utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
				utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu",lShopid);
			  utPltPutLoopVar(psDbHead,"shopname",iNum,caShopname);
			  utPltPutLoopVarF(psDbHead,"sgroupid",iNum,"%lu",lSgroupid);
			  utPltPutLoopVar(psDbHead,"sgroupname",iNum,caSgroupname);
			  utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",lLevel);
			  utPltPutLoopVar(psDbHead,"etime",iNum,utStrLtoF8((long long)lEtime, s_etime));
			  utPltSetCvtHtml(0);
			  lId = 0;
			  lShopid = 0;
			  memset(caShopname, 0, sizeof(caShopname));
			  lSgroupid = 0;
			  memset(caSgroupname, 0, sizeof(caSgroupname));
				lLevel = 0;
				lEtime = 0;
				memset(s_etime, 0, sizeof(s_etime));
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG,  sizeof(long),            &lId,
																		 UT_TYPE_ULONG,  sizeof(long),            &lShopid,
																     UT_TYPE_STRING, sizeof(caShopname)-1,    caShopname,
																     UT_TYPE_ULONG,  sizeof(long),            &lSgroupid,
																     UT_TYPE_STRING, sizeof(caSgroupname)-1,  caSgroupname,
																     UT_TYPE_ULONG,  sizeof(long),            &lLevel,
																     UT_TYPE_ULONG,  sizeof(long),            &lEtime);
		}
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomcomp_list.htm");
	  return 0;
}

// �̵���Ϣ
int ncm_webcomshop_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char id[20] = "";
	 char name[128] = "";
	 char service_code[16] = "";
	 char sgroupname[32] = "";
	 char imgpath[128] = "";
	 char floor[32] = "";
	 char url[255] = "";
	 char code[12] = "";
	 char tel[32] = "";
	 char address[255] = "";
	 char level[20] = "";
	 char plevel[20] = "";
	 char type[20] = "";
	 char stype[20] = "";
	 char descr[255] = "";
	 char detail[4096] = "";
	 char systime[32] = "";
	 char flags[32] = "";
	 char plateid[11] = "";
	 char areaid[10] = "";
	 char title[128] = "";
	 char temp_file[128] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lId = 0;
	 char 					caName[128] = "";
	 char           caService_code[16] = "";
	 char 					caSgroupname[32] = "";
	 char 					caImgpath[128] = "";
	 char 					caFloor[32] = "";
	 char 					caUrl[255] = "";
	 char 					caCode[12] = "";
	 char 					caTel[32] = "";
	 char 					caAddress[255] = "";
	 unsigned long	lLevel = 0;
	 unsigned long	lPlevel = 0;
	 unsigned long	lType = 0;
	 unsigned long	lStype = 0;
	 char 					caDescr[255] = "";
	 char 					caDetail[4096] = "";
	 unsigned long	lSystime = 0;
	 unsigned long	lFlags = 0;
	 unsigned long	lPlateid = 0;
	 unsigned long	lAreaid = 0;
	 char           caTitle[128] = "";
	 char           s_systime[32] = "";
	 char           caType[32] = "";
	 
	 char caUpdate[16] = "";
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
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   char caShopids[1024];
   char caCid[20];
    strcpy(caShopids,getDsShopids());
   // �õ��̵�ID
   unsigned long lShopId = 0;
	 dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	 printf("lShopId = %lu\n", lShopId);
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 33,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"id",          UT_TYPE_STRING,  sizeof(id)-1,             id,
																	"name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
																	"service_code",UT_TYPE_STRING,  sizeof(service_code)-1,   service_code,
																	"sgroupname",  UT_TYPE_STRING,  sizeof(sgroupname)-1,     sgroupname,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"floor",       UT_TYPE_STRING,  sizeof(floor)-1,          floor,
																	"url",         UT_TYPE_STRING,  sizeof(url)-1,            url,
																	"code",        UT_TYPE_STRING,  sizeof(code)-1,           code,
																	"tel",         UT_TYPE_STRING,  sizeof(tel)-1,            tel,
																	"address",     UT_TYPE_STRING,  sizeof(address)-1,        address,
																	"level",       UT_TYPE_STRING,  sizeof(level)-1,          level,
																	"plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
																	"type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
																	"stype",       UT_TYPE_STRING,  sizeof(stype)-1,          stype,
																	"descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
																	"detail",      UT_TYPE_STRING,  sizeof(detail)-1,         detail,
																	"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
																	"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"areaid",    	 UT_TYPE_STRING,  sizeof(areaid)-1,         areaid,
																	"title",    	 UT_TYPE_STRING,  sizeof(title)-1,          title,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"update",      UT_TYPE_STRING,  10,                       caUpdate,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport,
																	"cid",         UT_TYPE_STRING,  10,caCid);
	 
	 trim(_keyword);
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,128);
			strcpy(name,caTemp);
   }
   if(strlen(service_code)>0){
			pasCvtGBK(2,service_code,caTemp,16);
			strcpy(service_code,caTemp);
   }
   if(strlen(sgroupname)>0){
			pasCvtGBK(2,sgroupname,caTemp,32);
			strcpy(sgroupname,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,128);
			strcpy(imgpath,caTemp);
   }
   if(strlen(floor)>0){
			pasCvtGBK(2,floor,caTemp,32);
			strcpy(floor,caTemp);
   }
   if(strlen(address)>0){
			pasCvtGBK(2,address,caTemp,255);
			strcpy(address,caTemp);
   }
   if(strlen(type)>0){
			pasCvtGBK(2,type,caTemp,30);
			strcpy(type,caTemp);
   }
	 if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,255);
			strcpy(descr,caTemp);
   }
   if(strlen(detail)>0){
			pasCvtGBK(2,detail,caTemp,4096);
			strcpy(detail,caTemp);
   }
   if(strlen(systime)>0){
			pasCvtGBK(2,systime,caTemp,32);
			strcpy(systime,caTemp);
   }
   if(strlen(title)>0){
			pasCvtGBK(2,title,caTemp,128);
			strcpy(title,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
	 
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmshop where id in(%s)", caSesid);
    			printf("caTemp = %s\n", caTemp);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
				      return 0;	
					}
					
    		  // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					}
					pasDbExecSqlF("delete from ncmshop where id in(%s)",caSesid);
					pasDbExecSqlF("delete from ncmshoptype where shopid in(%s)",caSesid);
    	}
	 }
	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmshop where 1=1");
	 if(lShopId>0) // �����̵��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and id = %lu ", lShopId);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 if(!utStrIsSpaces(areaid)) // ���������ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and areaid = %lu ", atol(areaid));
	 }
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and sgroupid in (%s) ",caShopids);
	}
	if(strlen(caCid)>0){
	 	sprintf(sql+strlen(sql)," and sgroupid =%s ",caCid);
	}
	
	 printf("sql = %s\n",sql);
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select s.id, s.name, s.service_code, t.name, s.imgpath, s.floor, s.url, s.code, s.tel, s.address, s.level, s.plevel, s.type, s.stype, s.descr, s.detail, s.systime, s.flags, s.title from ncmshop s left join ncmcomtype t on s.sgroupid = t.sid where 1=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(lShopId>0) // �����̵��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and s.id = %lu ", lShopId);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and s.plateid = %lu ", atol(plateid));
	 }
	 if(!utStrIsSpaces(areaid)) // ���������ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and s.areaid = %lu ", atol(areaid));
	 }
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (s.name like '%%%s%%') ", _keyword);
	 }
	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and sgroupid in (%s) ",caShopids);
	}
	if(strlen(caCid)>0){
	 	sprintf(sql+strlen(sql)," and sgroupid =%s ",caCid);
	}
	
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by s.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),            &lId,
													     UT_TYPE_STRING, sizeof(caName)-1,        caName,
													     UT_TYPE_STRING, sizeof(caService_code)-1,caService_code,
													     UT_TYPE_STRING, sizeof(caSgroupname)-1,  caSgroupname,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,     caImgpath,
													     UT_TYPE_STRING, sizeof(caFloor)-1,       caFloor,
													     UT_TYPE_STRING, sizeof(caUrl)-1,         caUrl,
													     UT_TYPE_STRING, sizeof(caCode)-1,        caCode,
													     UT_TYPE_STRING, sizeof(caTel)-1,         caTel,
													     UT_TYPE_STRING, sizeof(caAddress)-1,     caAddress,
													     UT_TYPE_ULONG,  sizeof(long),            &lLevel,
													     UT_TYPE_ULONG,  sizeof(long),            &lPlevel,
													     UT_TYPE_ULONG,  sizeof(long),            &lType,
													     UT_TYPE_ULONG,  sizeof(long),            &lStype,
													     UT_TYPE_STRING, sizeof(caDescr)-1,       caDescr,
													     UT_TYPE_STRING, sizeof(caDetail)-1,      caDetail,
													     UT_TYPE_ULONG,  sizeof(long),            &lSystime,
															 UT_TYPE_ULONG,  sizeof(long),            &lFlags,
															 UT_TYPE_STRING, sizeof(caTitle)-1,       caTitle);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1){
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
		   if(lSystime>0){
		   		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
		   }else{
		   		strcpy(s_systime, "");	
		   }
		   
		   switch(lType){
					case 1:strcpy(caType, "���");break;
					case 2:strcpy(caType, "Ʒ��");break;
					default:break;	
			 }
			 
		   utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
			 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"service_code",iNum,caService_code);
			 utPltPutLoopVar(psDbHead,"sgroupname",iNum,caSgroupname);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"floor",iNum,caFloor);
			 utPltPutLoopVar(psDbHead,"url",iNum,caUrl);
			 utPltPutLoopVar(psDbHead,"code",iNum,caCode);
			 utPltPutLoopVar(psDbHead,"tel",iNum,caTel);
			 utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
			 utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",lLevel);
			 utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",lPlevel);
			 utPltPutLoopVar(psDbHead,"type",iNum,caType);
			 utPltPutLoopVarF(psDbHead,"stype",iNum,"%lu",lStype);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltPutLoopVar(psDbHead,"title",iNum,caTitle);
			 
			 utPltSetCvtHtml(1);
			 utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
			 utPltPutLoopVar(psDbHead,"detail",iNum,caDetail);
			 utPltSetCvtHtml(0);
			 
			 lId = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caService_code, 0, sizeof(caService_code));
			 memset(caSgroupname, 0, sizeof(caSgroupname));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 memset(caFloor, 0, sizeof(caFloor));
			 memset(caUrl, 0, sizeof(caUrl));
			 memset(caCode, 0, sizeof(caCode));
			 memset(caTel, 0, sizeof(caTel));
			 memset(caAddress, 0, sizeof(caAddress));
	     lLevel = 0;
	     lPlevel = 0;
	     lType = 0;
	     lStype = 0;
	     memset(caDescr, 0, sizeof(caDescr));
	     memset(caDetail, 0, sizeof(caDetail));
	     lSystime = 0;
	     lFlags = 0;
	     memset(s_systime, 0, sizeof(s_systime));
	     memset(caTitle, 0, sizeof(caTitle));
	     memset(caType, 0, sizeof(caType));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),            &lId,
															     UT_TYPE_STRING, sizeof(caName)-1,        caName,
															     UT_TYPE_STRING, sizeof(caService_code)-1,caService_code,
															     UT_TYPE_STRING, sizeof(caSgroupname)-1,  caSgroupname,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,     caImgpath,
															     UT_TYPE_STRING, sizeof(caFloor)-1,       caFloor,
															     UT_TYPE_STRING, sizeof(caUrl)-1,         caUrl,
															     UT_TYPE_STRING, sizeof(caCode)-1,        caCode,
															     UT_TYPE_STRING, sizeof(caTel)-1,         caTel,
															     UT_TYPE_STRING, sizeof(caAddress)-1,     caAddress,
															     UT_TYPE_ULONG,  sizeof(long),            &lLevel,
															     UT_TYPE_ULONG,  sizeof(long),            &lPlevel,
															     UT_TYPE_ULONG,  sizeof(long),            &lType,
															     UT_TYPE_ULONG,  sizeof(long),            &lStype,
															     UT_TYPE_STRING, sizeof(caDescr)-1,       caDescr,
															     UT_TYPE_STRING, sizeof(caDetail)-1,      caDetail,
															     UT_TYPE_ULONG,  sizeof(long),            &lSystime,
																	 UT_TYPE_ULONG,  sizeof(long),            &lFlags,
															     UT_TYPE_STRING, sizeof(caTitle)-1,       caTitle);
	  }
	  //utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomshop_list.htm");
	  return 0;	
}

int ncm_webcomshop_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
//#endif  
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 char temp1[4096] = "";
	 char temp2[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 char caKey[32] = "";
	 char caSid[32] = "";
	 char caPlateid[32] = "";
	 char caName[128] = "";
	 char caScode[16] = "";
	 unsigned long lSgroupid = 0;
	 char caImgpath[128] = "";
	 char caFloor[32] = "";
	 char caUrl[255] = "";
	 char caCode[12] = "";
	 char caTel[32] = "";
	 char caAddress[255] = "";
	 unsigned long lLevel = 0;
	 unsigned long lPlevel = 0;
	 unsigned long lType = 0;
	 unsigned long lStype = 0;
	 char caDescr[255] = "";
	 char caDetail[4096] = "";
	 unsigned long lSystime = 0;
	 unsigned long lPlateid = 0;
	 unsigned long lAreaid = 0;
	 unsigned long lCityid = 0;
	 unsigned long lProvid = 0;
	 char caTitle[128] = "";
	 
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 3, "key", UT_TYPE_STRING,     sizeof(caKey)-1,     caKey,
	                                "sid", UT_TYPE_STRING,     sizeof(caSid)-1,     caSid,
	                                "plateid", UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
			 sprintf(sql, "select name, service_code, sgroupid, imgpath, floor, url, code, tel, address, level, plevel, type, stype, descr, detail, systime, plateid, areaid, title from ncmshop where id=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,sizeof(caName)-1,    caName,
			 											UT_TYPE_STRING,sizeof(caScode)-1,   caScode,
			 											UT_TYPE_ULONG, sizeof(long),        &lSgroupid,
			 											UT_TYPE_STRING,sizeof(caImgpath)-1, caImgpath,
			 											UT_TYPE_STRING,sizeof(caFloor)-1,   caFloor,
			 											UT_TYPE_STRING,sizeof(caUrl)-1,     caUrl,
			 											UT_TYPE_STRING,sizeof(caCode)-1,    caCode,
			 											UT_TYPE_STRING,sizeof(caTel)-1,     caTel,
			 											UT_TYPE_STRING,sizeof(caAddress)-1, caAddress,
			 											UT_TYPE_ULONG, sizeof(long),        &lLevel,
			 											UT_TYPE_ULONG, sizeof(long),        &lPlevel,
			 											UT_TYPE_ULONG, sizeof(long),        &lType,
			 											UT_TYPE_ULONG, sizeof(long),        &lStype,
		                        UT_TYPE_STRING,sizeof(caDescr)-1,   caDescr,
		                        UT_TYPE_STRING,sizeof(caDetail)-1,  caDetail,
		                        UT_TYPE_ULONG, sizeof(long),        &lSystime,
		                        UT_TYPE_ULONG, sizeof(long),        &lPlateid,
		                        UT_TYPE_ULONG, sizeof(long),        &lAreaid,
		                        UT_TYPE_STRING,sizeof(caTitle)-1,   caTitle);
		   
			  utPltPutVar(psDbHead,"id",caSid);
				utPltPutVar(psDbHead,"name",caName);
				utPltPutVar(psDbHead,"scode",caScode);
				utPltPutVarF(psDbHead,"sgroupid","%lu",lSgroupid);
				utPltPutVar(psDbHead,"imgpath",caImgpath);
				utPltPutVar(psDbHead,"floor",caFloor);
				utPltPutVar(psDbHead,"url",caUrl);
		    utPltPutVar(psDbHead,"code",caCode);
		    utPltPutVar(psDbHead,"tel",caTel);
		    utPltPutVar(psDbHead,"address",caAddress);
		    utPltPutVarF(psDbHead,"level","%lu",lLevel);
		    utPltPutVarF(psDbHead,"plevel","%lu",lPlevel);
		    utPltPutVarF(psDbHead,"type","%lu",lType);
		    utPltPutVarF(psDbHead,"stype","%lu",lStype);
		    utPltPutVarF(psDbHead,"systime","%lu",lSystime);
		    utPltPutVarF(psDbHead,"plateid","%lu",lPlateid);
		    utPltPutVarF(psDbHead,"areaid","%lu",lAreaid);
		    utPltPutVar(psDbHead,"title",caTitle);
		    
		    sprintf(temp1, "select pid from ncmarea where aid=%d", lAreaid);
		    pasDbOneRecord(temp1,0,UT_TYPE_ULONG, sizeof(long), &lCityid);
		    printf("lCityid = %lu\n", lCityid);
		    utPltPutVarF(psDbHead,"cityid","%lu",lCityid);
		    
		    sprintf(temp2, "select pid from ncmarea where aid=%d", lCityid);
		    pasDbOneRecord(temp2,0,UT_TYPE_ULONG, sizeof(long), &lProvid);
		    printf("lProvid = %lu\n", lProvid);
		    utPltPutVarF(psDbHead,"provid","%lu",lProvid);
		    
		    utPltSetCvtHtml(1);
		    utPltPutVar(psDbHead,"descr",caDescr);
		    utPltPutVar(psDbHead,"detail",caDetail);
		    utPltSetCvtHtml(0);
	 }else{
	 			utPltPutVarF(psDbHead,"plateid","%lu",atol(caPlateid));	
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomshop_dr.htm");
	 return 0;		
}

int ncm_webcomshop_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  
	  char caImgPath[128]="";
	  char caDispImgPath[128]="";
	  char imgname[128]="";
	  char imp_file[256]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
	  char *p=NULL;
	  char *q=NULL;
    
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",(char *)ncLang("0194����"),(char *)ncLang("0183���ļ�����"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// ����ͼƬ���·��
  	sprintf(caImgPath,"%s/images/ShopImages/shopimages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);

	  utMsgGetSomeNVar(psMsgHead,1,"fileField", UT_TYPE_STRING, 255, imp_file);
	  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*��ָ�����ַ�����ȡһ����
		  	
		  	  ����ԭ�ͣ�
			����char *utStrGetWord(char *p,char *pOut,
			����                   short nMaxLen,char *pTerm);
			����
					����˵��:
			����p �C �����ַ���
			����pOut --- ����ַ���������Ҫ���ȷ���ÿռ�
			����nMaxLen --- ����ַ�������󳤶�
			����pTerm  ----- ��ֹ�������ַ�����ϵͳ��ɨ����ʱ�������ַ���ĳһ���ַ�ʱ������
			����
			��������ֵ��
			���������ַ���ָ�롣*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
		  	printf("temp_file = %s\n", temp_file);
		  	strcpy(temp3_file, temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
				}else{
					  q = strtok(temp3_file, ".");
					  //if(q)printf("%s\n", q);
					  q = strtok(NULL, ".");
					  //if(q)printf("%s\n", q);
					  srand((unsigned int)time(0));
						sprintf(imgname, "shop_%lu_%lu.%s", time(0), rand()%100, q);
						//printf("imgname = %s\n", imgname);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
						//printf("temp2_file = %s\n", temp2_file);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/shopimages/");
				    strcat(caDispImgPath, imgname);
				}
		}
		
	  utPltPutVar(psDbHead,"filepath",caDispImgPath);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomshop_closeFile.htm");
    return 0;
}

// �ϴ��̵�ͼƬ
int ncm_webComShopImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32]="";
   	char caId[16]="";
    char caName[128]="";
    char caScode[16]="";
    char caSgroupid[32]="";
    unsigned long lSgroupid=0;
    char caImgpath[128]="";
    char imgname[128]="";
    char caFloor[32]="";
    char caUrl[255]="";
    char caCode[12]="";
    char caTel[32]="";
    char caAddress[255]="";
    char caLevel[20]="";
    char caEtime[32]="";
    char caPlevel[20]="";
    char caType[20]="";
    char caStype[20]="";
    char caDescr[255]="";
    char caDetail[4096]="";
    char caPlateid[32]="";
    char caAreaid[32]="";
    char caCityid[32]="";
    char caProvid[32]="";
    char caTitle[128]="";
    
    char caImgpath1[128]="";
    char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
 		char caTemp2[1024]="";
 		char caTemp3[1024]="";
 		char caCompTemp[1024]="";
 		unsigned long lCount=0;
 		unsigned long lMaxid = 0;
 		unsigned long lMaxcompid = 0;
 		char *p;
 		unsigned long lGroupid=0;
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
		
		psDbHead = utPltInitDb();
        
	  utMsgGetSomeNVar(psMsgHead,23,"key",       UT_TYPE_STRING,  sizeof(caKey)-1,            caKey,
	                               "id",         UT_TYPE_STRING,  sizeof(caId)-1,             caId,
	  														 "name",       UT_TYPE_STRING,  sizeof(caName)-1,           caName,
	                               "scode",      UT_TYPE_STRING,  sizeof(caScode)-1,          caScode,
	                               "tyid",       UT_TYPE_STRING,  sizeof(caSgroupid)-1,       caSgroupid,
	                               "floor",      UT_TYPE_STRING,  sizeof(caFloor)-1,          caFloor,
	                               "url",        UT_TYPE_STRING,  sizeof(caUrl)-1,            caUrl,
	                               "code",       UT_TYPE_STRING,  sizeof(caCode)-1,           caCode,
	                               "tel",        UT_TYPE_STRING,  sizeof(caTel)-1,            caTel,
	                               "address",    UT_TYPE_STRING,  sizeof(caAddress)-1,        caAddress,
	                               "level",      UT_TYPE_STRING,  sizeof(caLevel)-1,          caLevel,
	                               "etime",      UT_TYPE_STRING,  sizeof(caEtime)-1,          caEtime,
	                               "plevel",     UT_TYPE_STRING,  sizeof(caPlevel)-1,         caPlevel,
	                               "type",       UT_TYPE_STRING,  sizeof(caType)-1,           caType,
	                               "stype",      UT_TYPE_STRING,  sizeof(caStype)-1,          caStype,
																 "imgpath",    UT_TYPE_STRING,  sizeof(caImgpath)-1,        caImgpath,
																 "descr",      UT_TYPE_STRING,  sizeof(caDescr)-1,          caDescr,
																 "detail",     UT_TYPE_STRING,  sizeof(caDetail)-1,         caDetail,
																 "plateid",    UT_TYPE_STRING,  sizeof(caPlateid)-1,        caPlateid,
																 "area",       UT_TYPE_STRING,  sizeof(caAreaid)-1,         caAreaid,
																 "city",       UT_TYPE_STRING,  sizeof(caCityid)-1,         caCityid,
																 "prov",       UT_TYPE_STRING,  sizeof(caProvid)-1,         caProvid,
																 "title",      UT_TYPE_STRING,  sizeof(caTitle)-1,          caTitle);
	  sscanf(caSgroupid,"%lu",&lGroupid);
//	  printf("lGrouid=%lu\n",lGroupid);
	  if(strlen(caName)>0){
				pasCvtGBK(2,caName,caTemp,128);
				strcpy(caName,caTemp);
    }
    if(strlen(caScode)>0){
			  pasCvtGBK(2,caScode,caTemp,16);
			  strcpy(caScode,caTemp);
    }
    if(strlen(caFloor)>0){
			  pasCvtGBK(2,caFloor,caTemp,32);
			  strcpy(caFloor,caTemp);
    }
    if(strlen(caUrl)>0){
				pasCvtGBK(2,caUrl,caTemp,255);
				strcpy(caUrl,caTemp);
    }
    if(strlen(caAddress)>0){
				pasCvtGBK(2,caAddress,caTemp,255);
				strcpy(caAddress,caTemp);
    }
    if(strlen(caImgpath)>0){
				pasCvtGBK(2,caImgpath,caTemp,128);
				strcpy(caImgpath,caTemp);
    }
    if(strlen(caDescr)>0){
				pasCvtGBK(2,caDescr,caTemp,255);
				strcpy(caDescr,caTemp);
    }
    if(strlen(caDetail)>0){
				pasCvtGBK(2,caDetail,caTemp,sizeof(caDetail));
				strcpy(caDetail,caTemp);
    }
    if(strlen(caTitle)>0){
				pasCvtGBK(2,caTitle,caTemp,128);
				strcpy(caTitle,caTemp);
    }
    
    strcpy(caName, pasDbReplaceQuote(caName)); // ת���ַ����д��е� ' �� " 
	  
	  unsigned long lShopId = 0;
	  dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	  
	  if(lShopId>0){
			  if(strcmp(caKey, "update")==0){
			  	  sprintf(caTemp2, "select imgpath from ncmshop where id=%lu", atol(caId));
			  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
			  	  printf("caImgpath1 = %s\n", caImgpath1);
			  	  if(strcmp(caImgpath1, caImgpath)==0){ // �Ƿ�ѡ���ϴ�ͼƬ, ��ԭͼƬ����
			  	  	  printf("****************ԭͼƬ����************\n");
			  	  	  sprintf(caTemp,"update ncmshop set id=%lu, name='%s',service_code='%s',sgroupid=%lu,imgpath='%s',floor='%s',url='%s',code='%s',tel='%s',address='%s',level=%lu,plevel=%lu,type=%lu,stype=%lu,descr='%s',detail='%s',systime=%lu,plateid=%lu,areaid=%lu,title='%s' where id=%lu",lShopId,caName,caScode,lGroupid,caImgpath,caFloor,caUrl,caCode,caTel,caAddress,atol(caLevel),atol(caPlevel),atol(caType),atol(caStype),caDescr,caDetail,time(0),atol(caPlateid),atol(caAreaid),caTitle,atol(caId));
							 	printf("caTemp = %s\n", caTemp);
							 	pasDbExecSqlF(caTemp);
							 	//utPltPutVar(psDbHead,"imgpath",caImgpath);
							 	
							 	// ���ձ�
							 	sprintf(caCompTemp, "update ncmshoptype set shopid=%lu, shopname='%s', imgpath='%s', level=%lu, descr='%s', systime=%lu, plateid=%lu, areaid=%lu where shopid=%lu",lShopId, caName, caImgpath, atol(caLevel), caDescr, time(0), atol(caPlateid), atol(caAreaid), atol(caId));
							 	printf("caCompTemp = %s\n", caCompTemp);
							 	pasDbExecSqlF(caCompTemp);
			  	  }else{ // �ǣ�ԭͼƬɾ������ͼƬ����
			  	  	  printf("****************ԭͼƬ�ı�************\n");
					  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
						  	system(temp3_file);
					  	  
					  	  sprintf(caTemp,"update ncmshop set id=%lu, name='%s',service_code='%s',sgroupid=%lu,imgpath='%s',floor='%s',url='%s',code='%s',tel='%s',address='%s',level=%lu,plevel=%lu,type=%lu,stype=%lu,descr='%s',detail='%s',systime=%lu,plateid=%lu,areaid=%lu,title='%s' where id=%lu",lShopId,caName,caScode,lGroupid,caImgpath,caFloor,caUrl,caCode,caTel,caAddress,atol(caLevel),atol(caPlevel),atol(caType),atol(caStype),caDescr,caDetail,time(0),atol(caPlateid),atol(caAreaid),caTitle,atol(caId));
							 	printf("caTemp = %s\n", caTemp);
							 	pasDbExecSqlF(caTemp);
							 	//utPltPutVar(psDbHead,"imgpath",caImgpath);
							 	
							 	// ���ձ�
							 	sprintf(caCompTemp, "update ncmshoptype set shopid=%lu, shopname='%s', imgpath='%s', level=%lu, descr='%s', systime=%lu, plateid=%lu, areaid=%lu where shopid=%lu",lShopId, caName, caImgpath, atol(caLevel), caDescr, time(0), atol(caPlateid), atol(caAreaid), atol(caId));
							 	printf("caCompTemp = %s\n", caCompTemp);
							 	pasDbExecSqlF(caCompTemp);
					  }
			  }else if(strcmp(caKey, "add")==0){
			  	  printf("****************������ͼƬ************\n");
						//pasDbOneRecord("select max(id) from ncmshop", 0, UT_TYPE_ULONG, 4, &lMaxid);
				    sprintf(caTemp,"insert into ncmshop (id,name,service_code,sgroupid,imgpath,floor,url,code,tel,address,level,plevel,type,stype,descr,detail,systime,plateid,areaid,title) values (%lu,'%s','%s',%lu,'%s','%s','%s','%s','%s','%s',%lu,%lu,%lu,%lu,'%s','%s',%lu,%lu,%lu,'%s')",lShopId,caName,caScode,lGroupid,caImgpath,caFloor,caUrl,caCode,caTel,caAddress,atol(caLevel),atol(caPlevel),atol(caType),atol(caStype),caDescr,caDetail,time(0),atol(caPlateid),atol(caAreaid),caTitle);
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	//utPltPutVar(psDbHead,"imgpath",caDispImgPath);
					 	
					 	// ���ձ�
					 	pasDbOneRecord("select max(id) from ncmshoptype", 0, UT_TYPE_ULONG, 4, &lMaxcompid);
					 	sprintf(caCompTemp, "insert into ncmshoptype (id, shopid, shopname, imgpath, level, descr, systime, plateid, areaid) values (%lu, %lu, '%s', '%s', %lu, '%s', %lu, %lu, %lu)", (lMaxcompid+1), lShopId, caName, caImgpath, atol(caLevel), caDescr, time(0), atol(caPlateid), atol(caAreaid));
			  		printf("caCompTemp = %s\n", caCompTemp);
			  		pasDbExecSqlF(caCompTemp);
			  }
	  }else{
	  	  if(strcmp(caKey, "update")==0){
			  	  sprintf(caTemp2, "select imgpath from ncmshop where id=%lu", atol(caId));
			  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
			  	  //printf("caImgpath1 = %s\n", caImgpath1);
			  	  if(strcmp(caImgpath1, caImgpath)==0){ // �Ƿ�ѡ���ϴ�ͼƬ, ��ԭͼƬ����
			  	  	  //printf("****************ԭͼƬ����************\n");
			  	  	  sprintf(caTemp,"update ncmshop set name='%s',service_code='%s',sgroupid=%lu,imgpath='%s',floor='%s',url='%s',code='%s',tel='%s',address='%s',level=%lu,plevel=%lu,type=%lu,stype=%lu,descr='%s',detail='%s',systime=%lu,plateid=%lu,areaid=%lu,title='%s' where id=%lu",caName,caScode,lGroupid,caImgpath,caFloor,caUrl,caCode,caTel,caAddress,atol(caLevel),atol(caPlevel),atol(caType),atol(caStype),caDescr,caDetail,time(0),atol(caPlateid),atol(caAreaid),caTitle,atol(caId));
							 	printf("caTemp = %s\n", caTemp);
							 	pasDbExecSqlF(caTemp);
							 	//utPltPutVar(psDbHead,"imgpath",caImgpath1);
							 	
							 	// ���ձ�
							 	sprintf(caCompTemp, "update ncmshoptype set shopname='%s', imgpath='%s', level=%lu, descr='%s', systime=%lu, plateid=%lu, areaid=%lu where shopid=%lu", caName, caImgpath, atol(caLevel), caDescr, time(0), atol(caPlateid), atol(caAreaid), atol(caId));
							 	printf("caCompTemp = %s\n", caCompTemp);
							 	pasDbExecSqlF(caCompTemp);
			  	  }else{ // �ǣ�ԭͼƬɾ������ͼƬ����
			  	  	  //printf("****************ԭͼƬ�ı�************\n");
					  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
						  	system(temp3_file);
					  	  
					  	  sprintf(caTemp,"update ncmshop set name='%s',service_code='%s',sgroupid=%lu,imgpath='%s',floor='%s',url='%s',code='%s',tel='%s',address='%s',level=%lu,plevel=%lu,type=%lu,stype=%lu,descr='%s',detail='%s',systime=%lu,plateid=%lu,areaid=%lu,title='%s' where id=%lu",caName,caScode,lGroupid,caImgpath,caFloor,caUrl,caCode,caTel,caAddress,atol(caLevel),atol(caPlevel),atol(caType),atol(caStype),caDescr,caDetail,time(0),atol(caPlateid),atol(caAreaid),caTitle,atol(caId));
							 	printf("caTemp = %s\n", caTemp);
							 	pasDbExecSqlF(caTemp);
							 	//utPltPutVar(psDbHead,"imgpath",caDispImgPath);
							 	
							 	// ���ձ�
							 	sprintf(caCompTemp, "update ncmshoptype set shopname='%s', imgpath='%s', level=%lu, descr='%s', systime=%lu, plateid=%lu, areaid=%lu where shopid=%lu",caName, caImgpath, atol(caLevel), caDescr, time(0), atol(caPlateid), atol(caAreaid), atol(caId));
							 	printf("caCompTemp = %s\n", caCompTemp);
							 	pasDbExecSqlF(caCompTemp);
					  }
			  }else if(strcmp(caKey, "add")==0){
			  	  //printf("****************������ͼƬ************\n");
						pasDbOneRecord("select max(id) from ncmshop", 0, UT_TYPE_ULONG, 4, &lMaxid);
				    sprintf(caTemp,"insert into ncmshop (id,name,service_code,sgroupid,imgpath,floor,url,code,tel,address,level,plevel,type,stype,descr,detail,systime,plateid,areaid,title) values (%lu,'%s','%s',%lu,'%s','%s','%s','%s','%s','%s',%lu,%lu,%lu,%lu,'%s','%s',%lu,%lu,%lu,'%s')",(lMaxid+1),caName,caScode,lGroupid,caImgpath,caFloor,caUrl,caCode,caTel,caAddress,atol(caLevel),atol(caPlevel),atol(caType),atol(caStype),caDescr,caDetail,time(0),atol(caPlateid),atol(caAreaid),caTitle);
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	//utPltPutVar(psDbHead,"imgpath",caDispImgPath);
					 	
					 	// ���ձ�
					 	pasDbOneRecord("select max(id) from ncmshoptype", 0, UT_TYPE_ULONG, 4, &lMaxcompid);
					 	sprintf(caCompTemp, "insert into ncmshoptype (id, shopid, shopname, imgpath, level, descr, systime, plateid, areaid) values (%lu, %lu, '%s', '%s', %lu, '%s', %lu, %lu, %lu)", (lMaxcompid+1), (lMaxid+1), caName, caImgpath, atol(caLevel), caDescr, time(0), atol(caPlateid), atol(caAreaid));
			  		printf("caCompTemp = %s\n", caCompTemp);
			  		pasDbExecSqlF(caCompTemp);
			  }
	  }
	  
	  utPltPutVar(psDbHead,"mesg","���ݼ��سɹ�");
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomodity_dr_list.htm");
    return 0;
}

// �̳��û���Ϣ
int ncm_webcomshopuser_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char uid[20] = "";
	 char username[32] = "";
	 char dispname[32] = "";
	 char groupname[16] = "";
	 char proname[16] = "";
	 char usermac[18] = "";
	 char password[32] = "";
	 char nickname[32] = "";
	 char membernum[32] = "";
	 char integral[20] = "";
	 char coupon[20] = "";
	 char address[128] = "";
	 char decript[4096] = "";
	 char lcount[20] = "";
	 char imgpath[128] = "";
	 char addtime[32] = "";
	 char lasttime[32] = "";
	 char flags[32] = "";
	 char plateid[20] = "";
	 char temp_file[128] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lUid = 0;
	 char 					caUsername[32] = "";
	 char           caDispname[32] = "";
	 char           caGroupname[16] = "";
	 char 					caProname[16] = "";
	 char 					caUsermac[18] = "";
	 char 					caPassword[32] = "";
	 char 					caNickname[32] = "";
	 char 					caMembernum[32] = "";
	 unsigned long	lIntegral = 0;
	 unsigned long	lCoupon = 0;
	 char 					caAddress[128] = "";
	 char 					caDecript[4096] = "";
	 unsigned long	lLcount = 0;
	 char 					caImgpath[128] = "";
	 unsigned long	lAddtime = 0;
	 unsigned long	lLasttime = 0;
	 unsigned long	lFlags = 0;
	 char           s_addtime[32] = "";
	 char           s_lasttime[32] = "";
	 char           s_flags[32] = "";
	 
	 char caUpdate[16] = "";
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
	 unsigned long lAddTime = 0;
	 unsigned long lLastTime = 0;
	 
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
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 28,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"uid",         UT_TYPE_STRING,  sizeof(uid)-1,            uid,
																	"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
																	"dispname",    UT_TYPE_STRING,  sizeof(dispname)-1,       dispname,
																	"groupname",   UT_TYPE_STRING,  sizeof(groupname)-1,      groupname,
																	"proname",     UT_TYPE_STRING,  sizeof(proname)-1,        proname,
																	"usermac",     UT_TYPE_STRING,  sizeof(usermac)-1,        usermac,
																	"password",    UT_TYPE_STRING,  sizeof(password)-1,       password,
																	"nickname",    UT_TYPE_STRING,  sizeof(nickname)-1,       nickname,
																	"membernum",   UT_TYPE_STRING,  sizeof(membernum)-1,      membernum,
																	"integral",    UT_TYPE_STRING,  sizeof(integral)-1,       integral,
																	"coupon",      UT_TYPE_STRING,  sizeof(coupon)-1,         coupon,
																	"address",     UT_TYPE_STRING,  sizeof(address)-1,        address,
																	"decript",     UT_TYPE_STRING,  sizeof(decript)-1,        decript,
																	"lcount",      UT_TYPE_STRING,  sizeof(lcount)-1,         lcount,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"addtime",     UT_TYPE_STRING,  sizeof(addtime)-1,        addtime,
																	"lasttime",    UT_TYPE_STRING,  sizeof(lasttime)-1,       lasttime,
																	"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
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
   if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,32);
			strcpy(username,caTemp);
   }
   if(strlen(dispname)>0){
			pasCvtGBK(2,dispname,caTemp,32);
			strcpy(dispname,caTemp);
   }
   if(strlen(groupname)>0){
			pasCvtGBK(2,groupname,caTemp,32);
			strcpy(groupname,caTemp);
   }
   if(strlen(proname)>0){
			pasCvtGBK(2,proname,caTemp,32);
			strcpy(proname,caTemp);
   }
   if(strlen(nickname)>0){
			pasCvtGBK(2,nickname,caTemp,18);
			strcpy(nickname,caTemp);
   }
   if(strlen(membernum)>0){
			pasCvtGBK(2,membernum,caTemp,32);
			strcpy(membernum,caTemp);
   }
   if(strlen(address)>0){
			pasCvtGBK(2,address,caTemp,128);
			strcpy(address,caTemp);
   }
   if(strlen(decript)>0){
			pasCvtGBK(2,decript,caTemp,sizeof(decript));
			strcpy(decript,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,128);
			strcpy(imgpath,caTemp);
   }
   if(strlen(addtime)>0){
			pasCvtGBK(2,addtime,caTemp,32);
			strcpy(addtime,caTemp);
   }
   if(strlen(lasttime)>0){
			pasCvtGBK(2,lasttime,caTemp,32);
			strcpy(lasttime,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char 	imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmshopuser where uid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
				 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
			       return 0;	
				  }
    		  
					// pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
					iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);		
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);	
				  }
					pasDbExecSqlF("delete from ncmshopuser where uid in(%s)",caSesid);
    	}
	 }
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmshopuser where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select uid, username, dispname, groupname, proname, usermac, password, nickname, membernum, integral, coupon, address, decript, lcount, imgpath, addtime, lasttime, flags from ncmshopuser where 1=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),         &lUid,
													     UT_TYPE_STRING, sizeof(caUsername)-1, caUsername,
													     UT_TYPE_STRING, sizeof(caDispname)-1, caDispname,
													     UT_TYPE_STRING, sizeof(caGroupname)-1,caGroupname,
													     UT_TYPE_STRING, sizeof(caProname)-1,  caProname,
													     UT_TYPE_STRING, sizeof(caUsermac)-1,  caUsermac,
													     UT_TYPE_STRING, sizeof(caPassword)-1, caPassword,
													     UT_TYPE_STRING, sizeof(caNickname)-1, caNickname,
													     UT_TYPE_STRING, sizeof(caMembernum)-1,caMembernum,
													     UT_TYPE_ULONG,  sizeof(long),         &lIntegral,
													     UT_TYPE_ULONG,  sizeof(long),         &lCoupon,
													     UT_TYPE_STRING, sizeof(caAddress)-1,  caAddress,
													     UT_TYPE_STRING, sizeof(caDecript)-1,  caDecript,
													     UT_TYPE_ULONG,  sizeof(long),         &lLcount,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,  caImgpath,
													     UT_TYPE_ULONG,  sizeof(long),         &lAddtime,
													     UT_TYPE_ULONG,  sizeof(long),         &lLasttime,
															 UT_TYPE_ULONG,  sizeof(long),         &lFlags);
	
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
		   
		   if(lAddtime){
		   		strcpy(s_addtime, utTimFormat("%Y/%m/%d %H:%M:%S", lAddtime));
		   }else{
		   		strcpy(s_addtime, "");	
		   }
		   if(lLasttime){
		   	  strcpy(s_lasttime, utTimFormat("%Y/%m/%d %H:%M:%S", lLasttime));
		   }else{
		   		strcpy(s_lasttime, "");	
		   }
		   switch(lFlags){
		   		case 0:strcpy(s_flags, "û�з��ʹ�");break;
		   		case 1:strcpy(s_flags, "�Ѿ����ʹ�");break;
		   		default:break;	
		   }
		   utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
			 utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
			 utPltPutLoopVar(psDbHead,"proname",iNum,caProname);
			 utPltPutLoopVar(psDbHead,"usermac",iNum,caUsermac);
			 utPltPutLoopVar(psDbHead,"password",iNum,caPassword);
			 utPltPutLoopVar(psDbHead,"nickname",iNum,caNickname);
			 utPltPutLoopVar(psDbHead,"membernum",iNum,caMembernum);
			 utPltPutLoopVarF(psDbHead,"integral",iNum,"%lu",lIntegral);
			 utPltPutLoopVarF(psDbHead,"coupon",iNum,"%lu",lCoupon);
			 utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
			 utPltPutLoopVar(psDbHead,"decript",iNum,caDecript);
			 utPltPutLoopVarF(psDbHead,"lcount",iNum,"%lu",lLcount);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"addtime",iNum,s_addtime);
			 utPltPutLoopVar(psDbHead,"lasttime",iNum,s_lasttime);
			 utPltPutLoopVar(psDbHead,"flags",iNum,s_flags);
			 utPltSetCvtHtml(0);
			 lUid = 0;
			 memset(caUsername, 0, sizeof(caUsername));
			 memset(caDispname, 0, sizeof(caDispname));
			 memset(caGroupname, 0, sizeof(caGroupname));
			 memset(caProname, 0, sizeof(caProname));
			 memset(caUsermac, 0, sizeof(caUsermac));
			 memset(caPassword, 0, sizeof(caPassword));
			 memset(caNickname, 0, sizeof(caNickname));
			 memset(caMembernum, 0, sizeof(caMembernum));
			 lIntegral = 0;
			 lCoupon = 0;
			 memset(caAddress, 0, sizeof(caAddress));
			 memset(caDecript, 0, sizeof(caDecript));
			 lLcount = 0;
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lAddtime = 0;
			 lLasttime = 0;
			 lFlags = 0;
			 memset(s_addtime, 0, sizeof(s_addtime));
			 memset(s_lasttime, 0, sizeof(s_lasttime));
			 memset(s_flags, 0, sizeof(s_flags));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),         &lUid,
															     UT_TYPE_STRING, sizeof(caUsername)-1, caUsername,
															     UT_TYPE_STRING, sizeof(caDispname)-1, caDispname,
															     UT_TYPE_STRING, sizeof(caGroupname)-1,caGroupname,
															     UT_TYPE_STRING, sizeof(caProname)-1,  caProname,
															     UT_TYPE_STRING, sizeof(caUsermac)-1,  caUsermac,
															     UT_TYPE_STRING, sizeof(caPassword)-1, caPassword,
															     UT_TYPE_STRING, sizeof(caNickname)-1, caNickname,
															     UT_TYPE_STRING, sizeof(caMembernum)-1,caMembernum,
															     UT_TYPE_ULONG,  sizeof(long),         &lIntegral,
															     UT_TYPE_ULONG,  sizeof(long),         &lCoupon,
															     UT_TYPE_STRING, sizeof(caAddress)-1,  caAddress,
															     UT_TYPE_STRING, sizeof(caDecript)-1,  caDecript,
															     UT_TYPE_ULONG,  sizeof(long),         &lLcount,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,  caImgpath,
															     UT_TYPE_ULONG,  sizeof(long),         &lAddtime,
															     UT_TYPE_ULONG,  sizeof(long),         &lLasttime,
																	 UT_TYPE_ULONG,  sizeof(long),         &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomshopuser_list.htm");
	  return 0;	
}

// �����߸�����Ϣ
int ncm_webcomconsump_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char cid[20] = "";
	 char username[32] = "";
	 char dispname[32] = "";
	 char usermac[18] = "";
	 char sname[64] = "";
	 char sid[20] = "";
	 char fee[20] = "";
	 char date[32] = "";
	 char uid[20] = "";
	 char lasttime[32] = "";
	 char plateid[20] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lCid = 0;
	 char 					caUsername[32] = "";
	 char 					caDispname[32] = "";
	 char 					caUsermac[18] = "";
	 char 					caSname[64] = "";
	 unsigned long	lSid = 0;
	 double	        dFfee = 0.0;
	 unsigned long	lDate = 0;
	 unsigned long	lUid = 0;
	 unsigned long	lLasttime = 0;
	 char           s_lasttime[32] = "";
	 char           s_date[32] = "";
	 
	 char caUpdate[16] = "";
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
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 20,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,     _keyword,
																	"cid",         UT_TYPE_STRING,  sizeof(cid)-1,          cid,
																	"username",    UT_TYPE_STRING,  sizeof(username)-1,     username,
																	"dispname",    UT_TYPE_STRING,  sizeof(dispname)-1,     dispname,
																	"usermac",     UT_TYPE_STRING,  sizeof(usermac)-1,      usermac,
																	"sname",       UT_TYPE_STRING,  sizeof(sname)-1,        sname,
																	"sid",    	   UT_TYPE_STRING,  sizeof(sid)-1,          sid,
																	"fee",    	   UT_TYPE_STRING,  sizeof(fee)-1,          fee,
																	"date",    	   UT_TYPE_STRING,  sizeof(date)-1,         date,
																	"uid",    	   UT_TYPE_STRING,  sizeof(uid)-1,          uid,
																	"lasttime",    UT_TYPE_STRING,  sizeof(lasttime)-1,     lasttime,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,      plateid,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,        start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,        limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,         sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,          dir,
																	"selsid",      UT_TYPE_STRING,  1000,                   caSesid,
																	"del",				 UT_TYPE_STRING,  10,                     caDel,
																	"update",      UT_TYPE_STRING,  10,                     caUpdate,
																	"caExport",    UT_TYPE_STRING,  10,                     caExport);
	 
	 trim(_keyword);
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,32);
			strcpy(username,caTemp);
   }
   if(strlen(dispname)>0){
			pasCvtGBK(2,dispname,caTemp,32);
			strcpy(dispname,caTemp);
   }
	 if(strlen(sname)>0){
			pasCvtGBK(2,sname,caTemp,64);
			strcpy(sname,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid));
			strcpy(caSesid,caTemp);
   }
   
   // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"cid");
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		if(strlen(caSesid)>0){
    			printf("caSesid = %s\n", caSesid);
    		  pasDbExecSqlF("delete from ncmconsump where cid in(%s)",caSesid);
    	}
	 }
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmconsump where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select cid, username, dispname, usermac, sname, sid, fee, date, uid, lasttime from ncmconsump where 1=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),        &lCid,
													     UT_TYPE_STRING, sizeof(caUsername)-1,caUsername,
													     UT_TYPE_STRING, sizeof(caDispname)-1,caDispname,
													     UT_TYPE_STRING, sizeof(caUsermac)-1, caUsermac,
													     UT_TYPE_STRING, sizeof(caSname)-1,   caSname,
													     UT_TYPE_ULONG,  sizeof(long),        &lSid,
													     UT_TYPE_FLOAT,  sizeof(double),      &dFfee,
													     UT_TYPE_ULONG,  sizeof(long),        &lDate,
													     UT_TYPE_ULONG,  sizeof(long),        &lUid,
															 UT_TYPE_ULONG,  sizeof(long),        &lLasttime);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
		   
		   if(lLasttime>0){
		   		strcpy(s_lasttime, utTimFormat("%Y/%m/%d %H:%M:%S", lLasttime));
		   }else{
		   		strcpy(s_lasttime, "");	
		   }
		   if(lDate>0){
		   		strcpy(s_date, utTimFormat("%Y/%m/%d", lDate));
		   }else{
		   		strcpy(s_date, "");
		   }
		   utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"cid",iNum,"%lu",lCid);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
			 utPltPutLoopVar(psDbHead,"usermac",iNum,caUsermac);
			 utPltPutLoopVar(psDbHead,"sname",iNum,caSname);
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVarF(psDbHead,"fee",iNum,"%.2f",dFfee);
			 utPltPutLoopVar(psDbHead,"date",iNum,s_date);
			 utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
			 utPltPutLoopVar(psDbHead,"lasttime",iNum,s_lasttime);
			 utPltSetCvtHtml(0);
			 lCid = 0;
			 memset(caUsername, 0, sizeof(caUsername));
			 memset(caDispname, 0, sizeof(caDispname));
			 memset(caUsermac, 0, sizeof(caUsermac));
			 memset(caSname, 0, sizeof(caSname));
			 lSid = 0;
			 dFfee = 0.0;
			 lDate = 0;
			 lUid = 0;
			 lLasttime = 0;
			 memset(s_lasttime, 0, sizeof(s_lasttime));
			 memset(s_date, 0, sizeof(s_date));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),        &lCid,
															     UT_TYPE_STRING, sizeof(caUsername)-1,caUsername,
															     UT_TYPE_STRING, sizeof(caDispname)-1,caDispname,
															     UT_TYPE_STRING, sizeof(caUsermac)-1, caUsermac,
															     UT_TYPE_STRING, sizeof(caSname)-1,   caSname,
															     UT_TYPE_ULONG,  sizeof(long),        &lSid,
															     UT_TYPE_FLOAT,  sizeof(double),      &dFfee,
															     UT_TYPE_ULONG,  sizeof(long),        &lDate,
															     UT_TYPE_ULONG,  sizeof(long),        &lUid,
																	 UT_TYPE_ULONG,  sizeof(long),        &lLasttime);
	  }
	  //utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomconsump_list.htm");
	  return 0;	
}

// �Ż݄���Ϣ
int ncm_webcomcoupon_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char cid[20] = "";
	 char fee[20] = "";
	 char counum[20] = "";
	 char addtime[32] = "";
	 char imgpath[128] = "";
	 char effetime[32] = "";
	 char plateid[20] = "";
	 char temp_file[128] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lCid = 0;
	 double         dFee = 0.0;
	 unsigned long	lCounum = 0;
	 unsigned long  lAddtime = 0;
	 char 					caImgpath[128] = "";
	 unsigned long  lEffetime = 0;
	 char           s_addtime[32] = "";
	 char           s_effetime[32] = "";
	 
	 char caUpdate[16] = "";
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
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 16,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"cid",         UT_TYPE_STRING,  sizeof(cid)-1,            cid,
																	"fee",         UT_TYPE_STRING,  sizeof(fee)-1,            fee,
																	"counum",      UT_TYPE_STRING,  sizeof(counum)-1,         counum,
																	"addtime",     UT_TYPE_STRING,  sizeof(addtime)-1,        addtime,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"effetime",    UT_TYPE_STRING,  sizeof(effetime)-1,       effetime,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"update",      UT_TYPE_STRING,  10,                       caUpdate,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport);
	 
	 trim(_keyword);
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,128);
			strcpy(imgpath,caTemp);
   }
   if(strlen(addtime)>0){
			pasCvtGBK(2,addtime,caTemp,32);
			strcpy(addtime,caTemp);
   }
   if(strlen(effetime)>0){
			pasCvtGBK(2,effetime,caTemp,32);
			strcpy(effetime,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid));
			strcpy(caSesid,caTemp);
   }
   
   // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"cid");
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char 	imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcoupon where cid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
				 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
			       return 0;	
				  }
				  
				  // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
    		  
    		  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmcoupon where cid in(%s)",caSesid);
    	}
	 }
	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmcoupon where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (fee like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select cid, fee, counum, addtime, imgpath, effetime from ncmcoupon where 1=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (fee like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lCid,
													     UT_TYPE_FLOAT,  sizeof(double),        &dFee,
													     UT_TYPE_ULONG,  sizeof(long),          &lCounum,
															 UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
													     UT_TYPE_ULONG,  sizeof(long),          &lEffetime);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				  utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
			 if(lAddtime>0){
			 		strcpy(s_addtime, utTimFormat("%Y/%m/%d", lAddtime));
			 }else{
			 		strcpy(s_addtime, "");	
			 }
			 if(lEffetime>0){
			 		strcpy(s_effetime, utTimFormat("%Y/%m/%d", lEffetime));	
			 }else{
			 		strcpy(s_effetime, "");
			 }
			 utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"cid",iNum,"%lu",lCid);
			 utPltPutLoopVarF(psDbHead,"fee",iNum,"%.2f",dFee);
			 utPltPutLoopVarF(psDbHead,"counum",iNum,"%lu",lCounum);
			 utPltPutLoopVar(psDbHead,"addtime",iNum,s_addtime);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"effetime",iNum,s_effetime);
			 utPltSetCvtHtml(0);
			 lCid = 0;
			 dFee = 0.0;
			 lCounum = 0;
			 lAddtime = 0;
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lEffetime = 0;
			 memset(s_addtime, 0, sizeof(s_addtime));
			 memset(s_effetime, 0, sizeof(s_effetime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lCid,
															     UT_TYPE_FLOAT,  sizeof(double),        &dFee,
															     UT_TYPE_ULONG,  sizeof(long),          &lCounum,
																	 UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
															     UT_TYPE_ULONG,  sizeof(long),          &lEffetime);
		}
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomcoupon_list.htm");
	  return 0;	
}

int ncm_webcomcoupon_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
//#endif  
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 char caKey[32] = "";
	 char caSid[32] = "";
	 double dFee = 0.0;
	 unsigned long lCounum = 0;
	 unsigned long lAddtime = 0;
	 char caImgpath[128] = "";
	 unsigned long lEffetime = 0;
	 char s_addtime[32] = "";
	 char s_effetime[32] = "";
	 char caPlateid[20] = "";
	 unsigned long lPlateid = 0;
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 3, "key",     UT_TYPE_STRING, sizeof(caKey)-1,     caKey,
	                                "sid",     UT_TYPE_STRING, sizeof(caSid)-1,     caSid,
	                                "plateid", UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
			 sprintf(sql, "select fee, counum, addtime, imgpath, effetime, plateid from ncmcoupon where cid=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_FLOAT,sizeof(double),&dFee,
			 											UT_TYPE_ULONG,sizeof(long),&lCounum,
		                        UT_TYPE_ULONG,sizeof(long),&lAddtime,
		                        UT_TYPE_STRING,sizeof(caImgpath)-1,caImgpath,
		                        UT_TYPE_ULONG,sizeof(long),&lEffetime,
		                        UT_TYPE_ULONG,sizeof(long),&lPlateid);
		                        
			  strcpy(s_addtime, utTimFormat("%Y/%m/%d", lAddtime));
			  strcpy(s_effetime, utTimFormat("%Y/%m/%d", lEffetime));
			  utPltSetCvtHtml(1);
			  utPltPutVar(psDbHead,"cid",caSid);
			  utPltPutVarF(psDbHead,"fee","%.2f",dFee);
			  utPltPutVarF(psDbHead,"counum","%lu",lCounum);
				utPltPutVar(psDbHead,"addtime",s_addtime);
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVar(psDbHead,"effetime",s_effetime);
		    utPltPutVarF(psDbHead,"plateid","%lu",lPlateid);
		    utPltSetCvtHtml(0);
	 }else{
	 			utPltPutVarF(psDbHead,"plateid","%lu",atol(caPlateid));	
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomcoupon_dr.htm");
	 return 0;
}

// �ϴ��Ż݄�ͼƬ
int ncm_webComCouponImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32]="";
   	char caCid[16]="";
    char caFee[20]="";
    char caCounum[20]="";
    char caAddtime[32]="";
    char caEffetime[32]="";
    char caImgPath[128]="";
    char caDispImgPath[128]="";
    char imgname[128]="";
    char caPlateid[20]="";
    
    char caImgpath1[128]="";
    
    char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
 		char caTemp2[1024]="";
 		unsigned long lAddTime=0;
 		unsigned long lEffeTime=0;
 		unsigned long lMaxid = 0;
 		char *p;
 		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
		
		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",(char *)ncLang("0194����"),(char *)ncLang("0183���ļ�����"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// ����ͼƬ���·��
  	sprintf(caImgPath,"%s/images/ShopImages/couponimages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);
        
	  utMsgGetSomeNVar(psMsgHead,8,"key",      UT_TYPE_STRING,  sizeof(caKey)-1,     caKey,
	                               "cid",      UT_TYPE_STRING,  sizeof(caCid)-1,     caCid,
	  														 "fee",      UT_TYPE_STRING,  sizeof(caFee)-1,     caFee,
	  														 "counum",   UT_TYPE_STRING,  sizeof(caCounum)-1,  caCounum,
	                               "addtime",  UT_TYPE_STRING,  sizeof(caAddtime)-1, caAddtime,
	                               "effetime", UT_TYPE_STRING,  sizeof(caEffetime)-1,caEffetime,
																 "fname",    UT_TYPE_STRING,  sizeof(imp_file)-1,  imp_file,
																 "plateid",  UT_TYPE_STRING,  sizeof(caPlateid)-1, caPlateid);
	  
	  if(strlen(caFee)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","�Ż݄���ֵ����Ϊ��!","�Ż݄���ֵ����Ϊ��!");
        return 0;	
	  }
	  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*��ָ�����ַ�����ȡһ����
		  	
		  	  ����ԭ�ͣ�
			����char *utStrGetWord(char *p,char *pOut,
			����                   short nMaxLen,char *pTerm);
			����
					����˵��:
			����p �C �����ַ���
			����pOut --- ����ַ���������Ҫ���ȷ���ÿռ�
			����nMaxLen --- ����ַ�������󳤶�
			����pTerm  ----- ��ֹ�������ַ�����ϵͳ��ɨ����ʱ�������ַ���ĳһ���ַ�ʱ������
			����
			��������ֵ��
			���������ַ���ָ�롣*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
				printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
					  memset(caDispImgPath, 0, sizeof(caDispImgPath));
				}else{
						sprintf(imgname, "coupon_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/couponimages/");
				    strcat(caDispImgPath, imgname);	
				}
	  }
	  
	  ncTimeToLong(caAddtime, &lAddTime);
	  ncTimeToLong(caEffetime, &lEffeTime);
	  
	  if(strcmp(caKey, "update")==0){
	  	  sprintf(caTemp2, "select imgpath from ncmcoupon where cid=%lu", atol(caCid));
	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  if(strlen(caDispImgPath)==0){ // �Ƿ�ѡ���ϴ�ͼƬ, ��ԭͼƬ����
	  	  	  sprintf(caTemp,"update ncmcoupon set fee=%.2f,counum=%lu,addtime=%lu,imgpath='%s',effetime=%lu,plateid=%lu where cid=%lu",atof(caFee),atol(caCounum),lAddTime,caImgpath1,lEffeTime,atol(caPlateid),atol(caCid));
			 			printf("caTemp = %s\n", caTemp);
			 			pasDbExecSqlF(caTemp);
			 			utPltPutVar(psDbHead,"imgpath",caImgpath1);
	  	  }else{ // �ǣ�ԭͼƬɾ������ͼƬ����
			  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1);
				  	system(temp3_file);
			  	  
			  	  sprintf(caTemp,"update ncmcoupon set fee=%.2f,counum=%lu,addtime=%lu,imgpath='%s',effetime=%lu,plateid=%lu where cid=%lu",atof(caFee),atol(caCounum),lAddTime,caDispImgPath,lEffeTime,atol(caPlateid),atol(caCid));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
			  }
	  }else if(strcmp(caKey, "add")==0){
	  	  pasDbOneRecord("select max(cid) from ncmcoupon", 0, UT_TYPE_ULONG, 4, &lMaxid);
		    sprintf(caTemp,"insert into ncmcoupon (cid,fee,counum,addtime,imgpath,effetime,plateid) values (%lu,%.2f,%lu,%lu,'%s',%lu,%lu)",(lMaxid+1),atof(caFee),atol(caCounum),lAddTime,caDispImgPath,lEffeTime,atol(caPlateid));
			 	printf("caTemp = %s\n", caTemp);
			 	pasDbExecSqlF(caTemp);
			 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
	  }
	  utPltPutVar(psDbHead,"mesg","���ݼ��سɹ�");
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomcoupon_dr_list.htm");
	  //utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","���ݼ��سɹ�!","���ݼ��سɹ�!");
    return 0;
}

// �Ż݄�ʹ�ñ���Ϣ
int ncm_webcomcouponuse_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char id[20] = "";
	 char scode[16] = "";
	 char username[32] = "";
	 char aid[20] = "";
	 char aname[32] = "";
	 char serno[32] = "";
	 char usermac[18] = "";
	 char sname[32] = "";
	 char feetime[20] = "";
	 char uid[20] = "";
	 char sid[20] = "";
	 char fee[20] = "";
	 char imgpath[128] = "";
	 char systime[32] = "";
	 char flags[20] = "";
	 char plateid[20] = "";
	 char temp_file[128] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lId = 0;
	 char           caScode[16] = "";
	 char 					caUsername[32] = "";
	 unsigned long  lAid = 0;
	 char           caAname[32] = "";
	 char           caSerno[32] = "";
	 char 					caUsermac[18] = "";
	 char 					caSname[32] = "";
	 unsigned long	lFeetime = 0;
	 unsigned long	lUid = 0;
	 unsigned long	lSid = 0;
	 double         dFee = 0.0;
	 char 					caImgpath[128] = "";
	 unsigned long	lSystime = 0;
	 unsigned long	lFlags = 0;
	 char           s_feetime[32] = "";
	 char           s_systime[32] = "";
	 char           s_flags[32] = "";
	 
	 char caUpdate[16] = "";
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
	 unsigned long lFeeTime = 0;
	 unsigned long lSysTime = 0;
	 
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
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 25,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"id",          UT_TYPE_STRING,  sizeof(id)-1,             id,
																	"scode",       UT_TYPE_STRING,  sizeof(scode)-1,          scode,
																	"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
																	"aid",         UT_TYPE_STRING,  sizeof(aid)-1,            aid,
																	"aname",       UT_TYPE_STRING,  sizeof(aname)-1,          aname,
																	"serno",       UT_TYPE_STRING,  sizeof(serno)-1,          serno,
																	"usermac",     UT_TYPE_STRING,  sizeof(usermac)-1,        usermac,
																	"sname",       UT_TYPE_STRING,  sizeof(sname)-1,          sname,
																	"feetime",     UT_TYPE_STRING,  sizeof(feetime)-1,        feetime,
																	"uid",         UT_TYPE_STRING,  sizeof(uid)-1,            uid,
																	"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
																	"fee",         UT_TYPE_STRING,  sizeof(fee)-1,            fee,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
																	"flags",       UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
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
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(scode)>0){
			pasCvtGBK(2,scode,caTemp,16);
			strcpy(scode,caTemp);
   }
   if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,32);
			strcpy(username,caTemp);
   }
   if(strlen(aname)>0){
			pasCvtGBK(2,aname,caTemp,32);
			strcpy(aname,caTemp);
   }
   if(strlen(sname)>0){
			pasCvtGBK(2,sname,caTemp,64);
			strcpy(sname,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,128);
			strcpy(imgpath,caTemp);
   }
   if(strlen(feetime)>0){
			pasCvtGBK(2,feetime,caTemp,32);
			strcpy(feetime,caTemp);
   }
   if(strlen(systime)>0){
			pasCvtGBK(2,systime,caTemp,32);
			strcpy(systime,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid));
			strcpy(caSesid,caTemp);
   }
   
   // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcouponuse where id in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
				 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
			       return 0;	
				  
				  }
				  
				  // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
							
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmcouponuse where id in(%s)",caSesid);
    	}
	 }
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmcouponuse where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select cu.id, cu.servicecode, cu.username, cu.aid, cu.aname, cu.serno, cu.usermac, cu.sname, cu.feetime, cu.uid, cu.sid, c.fee, cu.imgpath, cu.systime, cu.flags from ncmcouponuse cu left join ncmcoupon c on cu.cid = c.cid where 1=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (cu.username like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and cu.plateid = %lu ", atol(plateid));
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by cu.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lId,
													     UT_TYPE_STRING, sizeof(caScode)-1,     caScode,
													     UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
													     UT_TYPE_ULONG,  sizeof(long),          &lAid,
													     UT_TYPE_STRING, sizeof(caAname)-1,     caAname,
													     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
													     UT_TYPE_STRING, sizeof(caUsermac)-1,   caUsermac,
													     UT_TYPE_STRING, sizeof(caSname)-1,     caSname,
													     UT_TYPE_ULONG,  sizeof(long),          &lFeetime,
															 UT_TYPE_ULONG,  sizeof(long),          &lUid,
															 UT_TYPE_ULONG,  sizeof(long),          &lSid,
															 UT_TYPE_FLOAT,  sizeof(double),        &dFee,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
													     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
													     UT_TYPE_ULONG,  sizeof(long),          &lFlags);
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				  utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
			 if(lFeetime>0){
			 		strcpy(s_feetime, utTimFormat("%Y/%m/%d", lFeetime));	
			 }else{
			 		strcpy(s_feetime, "");	
			 }
			 if(lSystime>0){
			 		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));	
			 }else{
			 		strcpy(s_systime, "");	
			 }
			 switch(lFlags){
			 		case 0:strcpy(s_flags, "����");break;
			 		case 1:strcpy(s_flags, "ɾ��");break;
			 		default:break;	
			 }
			 utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
			 utPltPutLoopVar(psDbHead,"scode",iNum,caScode);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVarF(psDbHead,"aid",iNum,"%lu",lAid);
			 utPltPutLoopVar(psDbHead,"aname",iNum,caAname);
			 utPltPutLoopVar(psDbHead,"sermo",iNum,caSerno);
			 utPltPutLoopVar(psDbHead,"usermac",iNum,caUsermac);
			 utPltPutLoopVar(psDbHead,"sname",iNum,caSname);
			 utPltPutLoopVar(psDbHead,"feetime",iNum,s_feetime);
			 utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVarF(psDbHead,"fee",iNum,"%.2f",dFee);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVar(psDbHead,"flags",iNum,s_flags);
			 utPltSetCvtHtml(0);
			 lId = 0;
			 memset(caScode, 0, sizeof(caScode));
			 memset(caUsername, 0, sizeof(caUsername));
			 lAid = 0;
			 memset(caAname, 0, sizeof(caAname));
			 memset(caSerno, 0, sizeof(caSerno));
			 memset(caUsermac, 0, sizeof(caUsermac));
			 memset(caSname, 0, sizeof(caSname));
			 lFeetime = 0;
			 lUid = 0;
			 lSid = 0;
			 dFee = 0.0;
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lSystime = 0;
			 lFlags = 0;
			 memset(s_feetime, 0, sizeof(s_feetime));
			 memset(s_systime, 0, sizeof(s_systime));
			 memset(s_flags, 0, sizeof(s_flags));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lId,
															     UT_TYPE_STRING, sizeof(caScode)-1,     caScode,
															     UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
															     UT_TYPE_ULONG,  sizeof(long),          &lAid,
															     UT_TYPE_STRING, sizeof(caAname)-1,     caAname,
															     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
															     UT_TYPE_STRING, sizeof(caUsermac)-1,   caUsermac,
															     UT_TYPE_STRING, sizeof(caSname)-1,     caSname,
															     UT_TYPE_ULONG,  sizeof(long),          &lFeetime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lUid,
																	 UT_TYPE_ULONG,  sizeof(long),          &lSid,
																	 UT_TYPE_FLOAT,  sizeof(double),        &dFee,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
															     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
															     UT_TYPE_ULONG,  sizeof(long),          &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomcouponuse_list.htm");
	  return 0;	
}

// �̳��û���ע��Ϣ
int ncm_webcomshopattent_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char id[20] = "";
	 char servicecode[16] = "";
	 char username[32] = "";
	 char uid[20] = "";
	 char activename[32] = "";
	 char indexname[32] = "";
	 char shopname[32] = "";
	 char integral[20] = "";
	 char decript[4096] = "";
	 char imgpath[128] = "";
	 char addtime[32] = "";
	 char lasttime[32] = "";
	 char flags[32] = "";
	 char plateid[20] = "";
	 char temp_file[128] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lId = 0;
	 char           caServicecode[16] = "";
	 char 					caUsername[32] = "";
	 unsigned long	lUid = 0;
	 char 					caActivename[32] = "";
	 char 					caIndexname[32] = "";
	 char 					caShopname[32] = "";
	 unsigned long	lIntegral = 0;
	 char 					caDecript[4096] = "";
	 char 					caImgpath[128] = "";
	 unsigned long	lAddtime = 0;
	 unsigned long	lLasttime = 0;
	 unsigned long	lFlags = 0;
	 char           s_addtime[32] = "";
	 char           s_lasttime[32] = "";
	 char           s_flags[32] = "";
	 
	 char caUpdate[16] = "";
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
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 25,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"id",          UT_TYPE_STRING,  sizeof(id)-1,             id,
																	"servicecode", UT_TYPE_STRING,  sizeof(servicecode)-1,    servicecode,
																	"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
																	"uid",         UT_TYPE_STRING,  sizeof(uid)-1,            uid,
																	"activename",  UT_TYPE_STRING,  sizeof(activename)-1,     activename,
																	"indexname",   UT_TYPE_STRING,  sizeof(indexname)-1,      indexname,
																	"shopname",    UT_TYPE_STRING,  sizeof(shopname)-1,       shopname,
																	"integral",    UT_TYPE_STRING,  sizeof(integral)-1,       integral,
																	"decript",     UT_TYPE_STRING,  sizeof(decript)-1,        decript,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"addtime",     UT_TYPE_STRING,  sizeof(addtime)-1,        addtime,
																	"lasttime",    UT_TYPE_STRING,  sizeof(lasttime)-1,       lasttime,
																	"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
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
   if(strlen(servicecode)>0){
			pasCvtGBK(2,servicecode,caTemp,16);
			strcpy(servicecode,caTemp);
   }
   if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,32);
			strcpy(username,caTemp);
   }
   if(strlen(activename)>0){
			pasCvtGBK(2,activename,caTemp,32);
			strcpy(activename,caTemp);
   }
   if(strlen(indexname)>0){
			pasCvtGBK(2,indexname,caTemp,32);
			strcpy(indexname,caTemp);
   }
   if(strlen(shopname)>0){
			pasCvtGBK(2,shopname,caTemp,32);
			strcpy(shopname,caTemp);
   }
   if(strlen(decript)>0){
			pasCvtGBK(2,decript,caTemp,sizeof(decript));
			strcpy(decript,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,128);
			strcpy(imgpath,caTemp);
   }
   if(strlen(addtime)>0){
			pasCvtGBK(2,addtime,caTemp,32);
			strcpy(addtime,caTemp);
   }
   if(strlen(lasttime)>0){
			pasCvtGBK(2,lasttime,caTemp,32);
			strcpy(lasttime,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid));
			strcpy(caSesid,caTemp);
   }
   
   // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char 	imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmshopattent where id in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
				 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
			       return 0;	
				  }
				  
				  // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmshopattent where id in(%s)",caSesid);
    	}
	 }
	 
	 unsigned long lShopId = 0;
	 dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmshopattent where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 if(lShopId>0) // �����̵��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and shopid = %lu", lShopId);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select sa.id, sa.servicecode, sa.username, sa.uid, a.aname, i.name, s.name, sa.integral, sa.decript, sa.imgpath, sa.addtime, sa.lasttime, sa.flags from ncmshopattent sa left join ncmactive a on sa.aid = a.id left join ncmcomindex i on sa.sid = i.sid left join ncmshop s on sa.shopid = s.id where 1=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (sa.username like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sa.plateid = %lu ", atol(plateid));
	 }
	 if(lShopId>0) // �����̵��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sa.shopid = %lu", lShopId);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by sa.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
													     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
													     UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername,
													     UT_TYPE_ULONG,  sizeof(long),           &lUid,
													     UT_TYPE_STRING, sizeof(caActivename)-1, caActivename,
													     UT_TYPE_STRING, sizeof(caIndexname)-1,  caIndexname,
													     UT_TYPE_STRING, sizeof(caShopname)-1,   caShopname,
													     UT_TYPE_ULONG,  sizeof(long),           &lIntegral,
													     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
													     UT_TYPE_ULONG,  sizeof(long),           &lAddtime,
													     UT_TYPE_ULONG,  sizeof(long),           &lLasttime,
															 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
		   
		   if(lAddtime>0){
		   		strcpy(s_addtime, utTimFormat("%Y/%m/%d %H:%M:%S", lAddtime));	
		   }else{
		   		strcpy(s_addtime, "");	
		   }
		   if(lLasttime>0){
		   		strcpy(s_lasttime, utTimFormat("%Y/%m/%d %H:%M:%S", lLasttime));	
		   }else{
		   		strcpy(s_lasttime, "");	
		   }
		   switch(lFlags){
		   		case 0:strcpy(s_flags, "����");break;
		   		case 1:strcpy(s_flags, "ɾ��");break;
		   		default:break;
		   }
		   utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
			 utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
			 utPltPutLoopVar(psDbHead,"activename",iNum,caActivename);
			 utPltPutLoopVar(psDbHead,"indexname",iNum,caIndexname);
			 utPltPutLoopVar(psDbHead,"shopname",iNum,caShopname);
			 utPltPutLoopVarF(psDbHead,"integral",iNum,"%lu",lIntegral);
			 utPltPutLoopVar(psDbHead,"decript",iNum,caDecript);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"addtime",iNum,s_addtime);
			 utPltPutLoopVar(psDbHead,"lasttime",iNum,s_lasttime);
			 utPltPutLoopVar(psDbHead,"flags",iNum,s_flags);
			 utPltSetCvtHtml(0);
			 lId = 0;
			 memset(caServicecode, 0, sizeof(caServicecode));
			 memset(caUsername, 0, sizeof(caUsername));
			 lUid = 0;
			 memset(caActivename, 0, sizeof(caActivename));
			 memset(caIndexname, 0, sizeof(caIndexname));
			 memset(caShopname, 0, sizeof(caShopname));
			 unsigned long	lIntegral = 0;
			 memset(caDecript, 0, sizeof(caDecript));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lAddtime = 0;
			 lLasttime = 0;
			 lFlags = 0;
			 memset(s_addtime, 0, sizeof(s_addtime));
			 memset(s_lasttime, 0, sizeof(s_lasttime));
			 memset(s_flags, 0, sizeof(s_flags));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
															     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
															     UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername,
															     UT_TYPE_ULONG,  sizeof(long),           &lUid,
															     UT_TYPE_STRING, sizeof(caActivename)-1, caActivename,
															     UT_TYPE_STRING, sizeof(caIndexname)-1,  caIndexname,
															     UT_TYPE_STRING, sizeof(caShopname)-1,   caShopname,
															     UT_TYPE_ULONG,  sizeof(long),           &lIntegral,
															     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
															     UT_TYPE_ULONG,  sizeof(long),           &lAddtime,
															     UT_TYPE_ULONG,  sizeof(long),           &lLasttime,
																	 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomshopattent_list.htm");
	  return 0;	
}

// �̳��û��ղ���Ϣ
int ncm_webcomshopcollect_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char id[20] = "";
	 char servicecode[16] = "";
	 char username[32] = "";
	 char nickname[32] = "";
	 char membernum[32] = "";
	 char uid[20] = "";
	 char activename[32] = "";
	 char odityname[32] = "";
	 char imgpath[128] = "";
	 char decript[4096] = "";
	 char addtime[32] = "";
	 char lasttime[32] = "";
	 char flags[32] = "";
	 char plateid[20] = "";
	 char temp_file[128] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lId = 0;
	 char           caServicecode[16] = "";
	 char 					caUsername[32] = "";
	 char           caNickname[32] = "";
	 char           caMembernum[32] = "";
	 unsigned long	lUid = 0;
	 char           caActivename[32] = "";
	 char           caOdityname[32] = "";
	 char 					caDecript[4096] = "";
	 char 					caImgpath[128] = "";
	 unsigned long	lAddtime = 0;
	 unsigned long	lLasttime = 0;
	 unsigned long	lFlags = 0;
	 char           s_addtime[32] = "";
	 char           s_lasttime[32] = "";
	 char           s_flags[32] = "";
	 
	 char caUpdate[16] = "";
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
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 23,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"id",          UT_TYPE_STRING,  sizeof(id)-1,             id,
																	"servicecode", UT_TYPE_STRING,  sizeof(servicecode)-1,    servicecode,
																	"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
																	"nickname",    UT_TYPE_STRING,  sizeof(nickname)-1,       nickname,
																	"membernum",   UT_TYPE_STRING,  sizeof(membernum)-1,      membernum,
																	"uid",         UT_TYPE_STRING,  sizeof(uid)-1,            uid,
																	"activename",  UT_TYPE_STRING,  sizeof(activename)-1,     activename,
																	"odityname",   UT_TYPE_STRING,  sizeof(odityname)-1,      odityname,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"decript",     UT_TYPE_STRING,  sizeof(decript)-1,        decript,
																	"addtime",     UT_TYPE_STRING,  sizeof(addtime)-1,        addtime,
																	"lasttime",    UT_TYPE_STRING,  sizeof(lasttime)-1,       lasttime,
																	"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
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
   if(strlen(servicecode)>0){
			pasCvtGBK(2,servicecode,caTemp,16);
			strcpy(servicecode,caTemp);
   }
   if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,32);
			strcpy(username,caTemp);
   }
   if(strlen(nickname)>0){
			pasCvtGBK(2,nickname,caTemp,32);
			strcpy(nickname,caTemp);
   }
   if(strlen(membernum)>0){
			pasCvtGBK(2,membernum,caTemp,32);
			strcpy(membernum,caTemp);
   }
   if(strlen(activename)>0){
			pasCvtGBK(2,activename,caTemp,32);
			strcpy(activename,caTemp);
   }
   if(strlen(odityname)>0){
			pasCvtGBK(2,odityname,caTemp,32);
			strcpy(odityname,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,128);
			strcpy(imgpath,caTemp);
   }
   if(strlen(decript)>0){
			pasCvtGBK(2,decript,caTemp,sizeof(decript));
			strcpy(decript,caTemp);
   }
   if(strlen(addtime)>0){
			pasCvtGBK(2,addtime,caTemp,32);
			strcpy(addtime,caTemp);
   }
   if(strlen(lasttime)>0){
			pasCvtGBK(2,lasttime,caTemp,32);
			strcpy(lasttime,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid));
			strcpy(caSesid,caTemp);
   }
   
   // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char 	imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmshopcollect where id in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
				       return 0;	
					}
					
					// pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
					iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmshopcollect where id in(%s)",caSesid);
    	}
	 }
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmshopcollect where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select sc.id, sc.servicecode, sc.username, sc.nickname, sc.membernum, sc.uid, a.aname, o.name, sc.imgpath, sc.decript, sc.addtime, sc.lasttime, sc.flags from ncmshopcollect sc left join ncmactive a on sc.aid = a.id left join ncmcomodity o on sc.sid = o.sid where 1=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (sc.username like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sc.plateid = %lu ", atol(plateid));
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by sc.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
													     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
													     UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername,
													     UT_TYPE_STRING, sizeof(caNickname)-1,   caNickname,
													     UT_TYPE_STRING, sizeof(caMembernum)-1,  caMembernum,
													     UT_TYPE_ULONG,  sizeof(long),           &lUid,
													     UT_TYPE_STRING, sizeof(caActivename)-1, caActivename,
													     UT_TYPE_STRING, sizeof(caOdityname)-1,  caOdityname,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
													     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
													     UT_TYPE_ULONG,  sizeof(long),           &lAddtime,
													     UT_TYPE_ULONG,  sizeof(long),           &lLasttime,
															 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
		   
		   if(lAddtime>0){
		   		strcpy(s_addtime, utTimFormat("%Y/%m/%d %H:%M:%S", lAddtime));
		   }else{
		   		strcpy(s_addtime, "");	
		   }
		   if(lLasttime>0){
		   		strcpy(s_lasttime, utTimFormat("%Y/%m/%d %H:%M:%S", lLasttime));	
		   }else{
		   		strcpy(s_lasttime, "");	
		   }
		   switch(lFlags){
		   		case 0:strcpy(s_flags, "����");break;
		   		case 1:strcpy(s_flags, "ɾ��");break;
		   		default:break;	
		   }
		   utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
			 utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVar(psDbHead,"nickname",iNum,caNickname);
			 utPltPutLoopVar(psDbHead,"membernum",iNum,caMembernum);
			 utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
			 utPltPutLoopVar(psDbHead,"activename",iNum,caActivename);
			 utPltPutLoopVar(psDbHead,"odityname",iNum,caOdityname);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"decript",iNum,caDecript);
			 utPltPutLoopVar(psDbHead,"addtime",iNum,s_addtime);
			 utPltPutLoopVar(psDbHead,"lasttime",iNum,s_lasttime);
			 utPltPutLoopVar(psDbHead,"flags",iNum,s_flags);
			 utPltSetCvtHtml(0);
			 lId = 0;
			 memset(caServicecode, 0, sizeof(caServicecode));
			 memset(caUsername, 0, sizeof(caUsername));
			 memset(caNickname, 0, sizeof(caNickname));
			 memset(caMembernum, 0, sizeof(caMembernum));
			 lUid = 0;
			 memset(caActivename, 0, sizeof(caActivename));
			 memset(caOdityname, 0, sizeof(caOdityname));
			 memset(caDecript, 0, sizeof(caDecript));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lAddtime = 0;
			 lLasttime = 0;
			 lFlags = 0;
			 memset(s_addtime, 0, sizeof(s_addtime));
			 memset(s_lasttime, 0, sizeof(s_lasttime));
			 memset(s_flags, 0, sizeof(s_flags));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
															     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
															     UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername,
															     UT_TYPE_STRING, sizeof(caNickname)-1,   caNickname,
															     UT_TYPE_STRING, sizeof(caMembernum)-1,  caMembernum,
															     UT_TYPE_ULONG,  sizeof(long),           &lUid,
															     UT_TYPE_STRING, sizeof(caActivename)-1, caActivename,
															     UT_TYPE_STRING, sizeof(caOdityname)-1,  caOdityname,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
															     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
															     UT_TYPE_ULONG,  sizeof(long),           &lAddtime,
															     UT_TYPE_ULONG,  sizeof(long),           &lLasttime,
																	 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomshopcollect_list.htm");
	  return 0;	
}

// �̳�һ�����
int ncm_webcomactive_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char id[20] = "";
	 char servicecode[16] = "";
	 char aname[64] = "";
	 char indexname[32] = "";
	 char odityname[32] = "";
	 char couponfee[20] = "";
	 char discount[20] = "";
	 char address[255] = "";
	 char decript[4096] = "";
	 char imgpath[128] = "";
	 char plevel[20] = "";
	 char type[20] = "";
	 char xid[20] = "";
	 char stime[32] = "";
	 char etime[32] = "";
	 char flags[32] = "";
	 char plateid[20] = "";
	 char temp_file[128] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lId = 0;
	 char 					caServicecode[16] = "";
	 char 					caAname[64] = "";
	 char 			    caIndexname[32] = "";
	 char 			    caOdityname[32] = "";
	 double         dCouponfee = 0.0;
	 double	        dDiscount = 0.0;
	 char           caAddress[255] = "";
	 char 					caDecript[4096] = "";
	 char 					caImgpath[128] = "";
	 unsigned long	lPlevel = 0;
	 unsigned long	lType = 0;
	 unsigned long	lXid = 0;
	 unsigned long	lStime = 0;
	 unsigned long	lEtime = 0;
	 unsigned long	lFlags = 0;
	 char           s_stime[32] = "";
	 char           s_etime[32] = "";
	 
	 char caUpdate[16] = "";
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
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 26,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"id",          UT_TYPE_STRING,  sizeof(id)-1,             id,
																	"servicecode", UT_TYPE_STRING,  sizeof(servicecode)-1,    servicecode,
																	"aname",       UT_TYPE_STRING,  sizeof(aname)-1,          aname,
																	"indexname",   UT_TYPE_STRING,  sizeof(indexname)-1,      indexname,
																	"odityname",   UT_TYPE_STRING,  sizeof(odityname)-1,      odityname,
																	"couponfee",   UT_TYPE_STRING,  sizeof(couponfee)-1,      couponfee,
																	"discount",    UT_TYPE_STRING,  sizeof(discount)-1,       discount,
																	"address",     UT_TYPE_STRING,  sizeof(address)-1,        address,
																	"decript",     UT_TYPE_STRING,  sizeof(decript)-1,        decript,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
																	"type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
																	"xid",         UT_TYPE_STRING,  sizeof(xid)-1,            xid,
																	"stime",       UT_TYPE_STRING,  sizeof(stime)-1,          stime,
																	"etime",       UT_TYPE_STRING,  sizeof(etime)-1,          etime,
																	"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
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
   if(strlen(aname)>0){
			pasCvtGBK(2,aname,caTemp,64);
			strcpy(aname,caTemp);
   }
   if(strlen(indexname)>0){
			pasCvtGBK(2,indexname,caTemp,32);
			strcpy(indexname,caTemp);
   }
   if(strlen(odityname)>0){
			pasCvtGBK(2,odityname,caTemp,32);
			strcpy(odityname,caTemp);
   }
   if(strlen(address)>0){
			pasCvtGBK(2,address,caTemp,30);
			strcpy(address,caTemp);
   }
	 if(strlen(decript)>0){
			pasCvtGBK(2,decript,caTemp,sizeof(decript));
			strcpy(decript,caTemp);
   }
   if(strlen(stime)>0){
			pasCvtGBK(2,stime,caTemp,32);
			strcpy(stime,caTemp);
   }
   if(strlen(etime)>0){
			pasCvtGBK(2,etime,caTemp,32);
			strcpy(etime,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,128);
			strcpy(imgpath,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid));
			strcpy(caSesid,caTemp);
   }
   printf("caSesid = %s\n", caSesid);
   
   // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmactive where id in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
				 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
			       return 0;	
				  }
				  
				  // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
				  iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
    		  
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmactive where id in(%s)",caSesid);
    	}
	 }
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmactive where 1=1 and type=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (aname like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select a.id, a.servicecode, a.aname, i.name, o.name, c.fee, a.discount, a.address, a.decript, a.imgpath, a.plevel, a.type, a.xid, a.stime, a.etime, a.flags from ncmactive a left join ncmcomindex i on a.shopid = i.sid left join ncmcomodity o on a.sid = o.sid left join ncmcoupon c on a.cid = c.cid where 1=1 and a.type=1");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (a.aname like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and a.plateid = %lu ", atol(plateid));
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by a.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
													     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
													     UT_TYPE_STRING, sizeof(caAname)-1,      caAname,
													     UT_TYPE_STRING, sizeof(caIndexname)-1,  caIndexname,
													     UT_TYPE_STRING, sizeof(caOdityname)-1,  caOdityname,
													     UT_TYPE_FLOAT,  sizeof(double),         &dCouponfee,
													     UT_TYPE_FLOAT,  sizeof(double),         &dDiscount,
													     UT_TYPE_STRING, sizeof(caAddress)-1,    caAddress,
													     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
													     UT_TYPE_ULONG,  sizeof(long),           &lPlevel,
													     UT_TYPE_ULONG,  sizeof(long),           &lType,
													     UT_TYPE_ULONG,  sizeof(long),           &lXid,
													     UT_TYPE_ULONG,  sizeof(long),           &lStime,
															 UT_TYPE_ULONG,  sizeof(long),           &lEtime,
															 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
		   
		   if(lStime>0){
		   		strcpy(s_stime, utTimFormat("%Y/%m/%d", lStime));	
		   }else{
		   		strcpy(s_stime, "");	
		   }
		   if(lEtime>0){
		   		strcpy(s_etime, utTimFormat("%Y/%m/%d", lEtime));	
		   }else{
		   		strcpy(s_etime, "");	
		   }
		   //utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
			 utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
			 utPltPutLoopVar(psDbHead,"aname",iNum,caAname);
			 utPltPutLoopVar(psDbHead,"indexname",iNum,caIndexname);
			 utPltPutLoopVar(psDbHead,"odityname",iNum,caOdityname);
			 utPltPutLoopVarF(psDbHead,"couponfee",iNum,"%.2f",dCouponfee);
			 utPltPutLoopVarF(psDbHead,"discount",iNum,"%.2f",dDiscount);
			 utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
			 utPltPutLoopVar(psDbHead,"decript",iNum,caDecript);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",lPlevel);
			 utPltPutLoopVarF(psDbHead,"type",iNum,"%lu",lType);
			 utPltPutLoopVarF(psDbHead,"xid",iNum,"%lu",lXid);
			 utPltPutLoopVar(psDbHead,"stime",iNum,s_stime);
			 utPltPutLoopVar(psDbHead,"etime",iNum,s_etime);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 //utPltSetCvtHtml(0);
			 
			 lId = 0;
			 memset(caServicecode, 0, sizeof(caServicecode));
			 memset(caAname, 0, sizeof(caAname));
			 memset(caIndexname, 0, sizeof(caIndexname));
			 memset(caOdityname, 0, sizeof(caOdityname));
			 dCouponfee = 0.0;
			 dDiscount = 0.0;
			 memset(caAddress, 0, sizeof(caAddress));
			 memset(caDecript, 0, sizeof(caDecript));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lPlevel = 0;
			 lType = 0;
			 lXid = 0;
			 lStime = 0;
			 lEtime = 0;
			 lFlags = 0;
			 memset(s_stime, 0, sizeof(s_stime));
			 memset(s_etime, 0, sizeof(s_etime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
															     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
															     UT_TYPE_STRING, sizeof(caAname)-1,      caAname,
															     UT_TYPE_STRING, sizeof(caIndexname)-1,  caIndexname,
															     UT_TYPE_STRING, sizeof(caOdityname)-1,  caOdityname,
															     UT_TYPE_FLOAT,  sizeof(double),         &dCouponfee,
															     UT_TYPE_FLOAT,  sizeof(double),         &dDiscount,
															     UT_TYPE_STRING, sizeof(caAddress)-1,    caAddress,
															     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
															     UT_TYPE_ULONG,  sizeof(long),           &lPlevel,
															     UT_TYPE_ULONG,  sizeof(long),           &lType,
															     UT_TYPE_ULONG,  sizeof(long),           &lXid,
															     UT_TYPE_ULONG,  sizeof(long),           &lStime,
																	 UT_TYPE_ULONG,  sizeof(long),           &lEtime,
																	 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomactive_list.htm");
	  return 0;	
}

int ncm_webcomactive_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
//#endif  
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iFlag=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 char caKey[32] = "";
	 char caSid[32] = "";
	 char caPlateid[20] = "";
	 unsigned long lPlateid = 0;
	 char caServicecode[16] = "";
	 char caAname[64] = "";
	 unsigned long lShopid = 0;
	 unsigned long lSid = 0;
	 unsigned long lCid = 0;
	 double fDiscount = 0.0;
	 char caAddress[255] = "";
	 char caDecript[4096] = "";
	 char caImgpath[128] = "";
	 unsigned long lPlevel = 0;
	 unsigned long lType = 0;
	 unsigned long lXid = 0;
	 unsigned long lStime = 0;
	 unsigned long lEtime = 0;
	 
	 char s_stime[32] = "";
	 char s_etime[32] = "";
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 3,"key",     UT_TYPE_STRING, sizeof(caKey)-1,     caKey,
	 															 "sid",     UT_TYPE_STRING, sizeof(caSid)-1,     caSid,
	 															 "plateid", UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
	 	   iFlag = 1;
			 sprintf(sql, "select servicecode, aname, shopid, sid, cid, discount, address, decript, imgpath, plevel, type, xid, stime, etime, plateid from ncmactive where id=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,sizeof(caServicecode)-1,caServicecode,
			                      UT_TYPE_STRING,sizeof(caAname)-1,caAname,
			                      UT_TYPE_ULONG,sizeof(long),&lShopid,
			                      UT_TYPE_ULONG,sizeof(long),&lSid,
			                      UT_TYPE_ULONG,sizeof(long),&lCid,
			                      UT_TYPE_FLOAT,sizeof(double),&fDiscount,
			                      UT_TYPE_STRING,sizeof(caAddress)-1,caAddress,
		                        UT_TYPE_STRING,sizeof(caDecript)-1,caDecript,
		                        UT_TYPE_STRING,sizeof(caImgpath)-1,caImgpath,
		                        UT_TYPE_ULONG,sizeof(long),&lPlevel,
		                        UT_TYPE_ULONG,sizeof(long),&lType,
		                        UT_TYPE_ULONG,sizeof(long),&lXid,
		                        UT_TYPE_ULONG,sizeof(long),&lStime,
		                        UT_TYPE_ULONG,sizeof(long),&lEtime,
		                        UT_TYPE_ULONG,sizeof(long),&lPlateid);
		    
		    if(lStime>0){
		    		strcpy(s_stime, utTimFormat("%Y/%m/%d", lStime));	
		    }else{
		    		strcpy(s_stime, "");	
		    }
			  if(lEtime>0){
			  		strcpy(s_etime, utTimFormat("%Y/%m/%d", lEtime));	
			  }else{
			  		strcpy(s_etime, "");	
			  }
			  //utPltSetCvtHtml(1);
			  utPltPutVar(psDbHead,"id",caSid);
			  utPltPutVar(psDbHead,"scode",caServicecode);
				utPltPutVar(psDbHead,"aname",caAname);
				utPltPutVarF(psDbHead,"indexid","%lu",lShopid);
				utPltPutVarF(psDbHead,"odityid","%lu",lSid);
				utPltPutVarF(psDbHead,"couponid","%lu",lCid);
				utPltPutVarF(psDbHead,"discount","%.2f",fDiscount);
				utPltPutVar(psDbHead,"address",caAddress);
				utPltPutVar(psDbHead,"decript",caDecript);
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVarF(psDbHead,"plevel","%lu",lPlevel);
		    utPltPutVarF(psDbHead,"type","%lu",lType);
		    utPltPutVarF(psDbHead,"xid","%lu",lXid);
		    utPltPutVar(psDbHead,"stime",s_stime);
		    utPltPutVar(psDbHead,"etime",s_etime);
		    utPltPutVarF(psDbHead,"plateid","%d",lPlateid);
		    
		    utPltPutVarF(psDbHead,"flag","%d",iFlag);
		    //utPltSetCvtHtml(0);
	 }else{
	 	    utPltPutVarF(psDbHead,"plateid","%d",atol(caPlateid));
	 	    utPltPutVarF(psDbHead,"flag","%d",iFlag);
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomactive_dr.htm");
	 return 0;		
}

int ncm_webcomactive_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  
	  char caPos[32]="";
	  char caImgPath[128]="";
	  char caDispImgPath[128]="";
	  char imgname[128]="";
	  char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
 		char caTemp[1024]="";
	  char *p=NULL;
    
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",(char *)ncLang("0194����"),(char *)ncLang("0183���ļ�����"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// ����ͼƬ���·��
  	sprintf(caImgPath,"%s/images/ShopImages/activeimages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);

	  utMsgGetSomeNVar(psMsgHead,2,"fileField", UT_TYPE_STRING, 255, imp_file,
	  														 "position",  UT_TYPE_STRING, sizeof(caPos)-1, caPos);
	  printf("fileField = %s\n", imp_file);
	  printf("position = %s\n", caPos);
	  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*��ָ�����ַ�����ȡһ����
		  	
		  	  ����ԭ�ͣ�
			����char *utStrGetWord(char *p,char *pOut,
			����                   short nMaxLen,char *pTerm);
			����
					����˵��:
			����p �C �����ַ���
			����pOut --- ����ַ���������Ҫ���ȷ���ÿռ�
			����nMaxLen --- ����ַ�������󳤶�
			����pTerm  ----- ��ֹ�������ַ�����ϵͳ��ɨ����ʱ�������ַ���ĳһ���ַ�ʱ������
			����
			��������ֵ��
			���������ַ���ָ�롣*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
		  	printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
				}else{
						sprintf(imgname, "active_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/activeimages/");
				    strcat(caDispImgPath, imgname);	
				}
		}
		
		utPltPutVar(psDbHead,"position",caPos);
	  utPltPutVar(psDbHead,"filepath",caDispImgPath);
	  printf("position = %s\n", caPos);
	  printf("imgpath = %s\n", caDispImgPath);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomactive_closeFile.htm");
    return 0;
}

// �ϴ��̳�һ���ͼƬ
int ncm_webcomactive_update_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32]="";
   	char caId[20]="";
   	char caScode[16]="";
    char caAname[32]="";
    char caIndexid[32]="";
    char caOdityid[32]="";
    char caCouponid[32]="";
    char caDiscount[20]="";
    char caAddress[255]="";
    char caDecript[4096]="";
    char caImgPath[128]="";
    char caPlevel[20] = "";
    char caType[20] = "";
    char caXid[20] = "";
    char caStime[32]="";
    char caEtime[32]="";
    char caPlateid[20]="";
    
    unsigned long lCount = 0;
    char caImgpath1[128]="";
    char temp_file[129]="";
 		char caTemp[1024]="";
 		char caTemp1[1024]="";
 		char caTemp2[1024]="";
 		char caTemp3[1024]="";
 		
 		unsigned long lShopid=0;
 		unsigned long lSid=0;
 		unsigned long lCid=0;
 		unsigned long lStime=0;
 		unsigned long lEtime=0;
 		unsigned long lMaxid = 0;
 		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
    
    psDbHead = utPltInitDb();
    
    // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	  utMsgGetSomeNVar(psMsgHead, 17,"key",      UT_TYPE_STRING,  sizeof(caKey)-1,            caKey,
	                               "id",         UT_TYPE_STRING,  sizeof(caId)-1,             caId,
	                               "scode",      UT_TYPE_STRING,  sizeof(caScode)-1,          caScode,
	  														 "aname",      UT_TYPE_STRING,  sizeof(caAname)-1,          caAname,
	  														 "indexid",    UT_TYPE_STRING,  sizeof(caIndexid)-1,        caIndexid,
	  														 "odityid",    UT_TYPE_STRING,  sizeof(caOdityid)-1,        caOdityid,
	  														 "couponid",   UT_TYPE_STRING,  sizeof(caCouponid)-1,       caCouponid,
	  														 "discount",   UT_TYPE_STRING,  sizeof(caDiscount)-1,       caDiscount,
	  														 "address",    UT_TYPE_STRING,  sizeof(caAddress)-1,        caAddress,
	  														 "decript",    UT_TYPE_STRING,  sizeof(caDecript)-1,        caDecript,
	  														 "filepath",   UT_TYPE_STRING,  sizeof(caImgPath)-1,        caImgPath,
	  														 "plevel",     UT_TYPE_STRING,  sizeof(caPlevel)-1,         caPlevel,
	  														 "type",       UT_TYPE_STRING,  sizeof(caType)-1,           caType,
	  														 "xid",        UT_TYPE_STRING,  sizeof(caXid)-1,            caXid,
																 "stime",      UT_TYPE_STRING,  sizeof(caStime)-1,          caStime,
																 "etime",      UT_TYPE_STRING,  sizeof(caEtime)-1,          caEtime,
																 "plateid",    UT_TYPE_STRING,  sizeof(caPlateid)-1,        caPlateid);
	  
	  if(strlen(caAname)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","����Ʋ���Ϊ��!","����Ʋ���Ϊ��!");
        return 0;	
	  }
	  
	  strcpy(caType, "1");
	  
	  sprintf(caTemp3, "select servicecode from ncmcomodity where sid = %lu", atol(caOdityid));
	  pasDbOneRecord(caTemp3,0,UT_TYPE_STRING,sizeof(caScode)-1,caScode);
	  ncTimeToLong(caStime, &lStime);
	  ncTimeToLong(caEtime, &lEtime);
	  
	  sprintf(caTemp1, "select count(*) from ncmactive where plevel=%d", atol(caPlevel));
	  pasDbOneRecord(caTemp1,0,UT_TYPE_LONG,4,&lCount);                
	  if(strcmp(caKey, "add")==0){ //����
	  	  if(lCount){
	  	  	  // ɾ��ԭλ�õ�ͼƬ
	  	  	  sprintf(caTemp2, "select imgpath from ncmactive where plevel=%lu", atol(caPlevel));
	  	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  	  sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
				  	system(temp_file);
				  	// ��ԭλ�õĻ��Ϣ�滻
					 	sprintf(caTemp,"update ncmactive set servicecode='%s',aname='%s',shopid=%lu,sid=%lu,cid=%lu,discount=%.2f,address='%s',decript='%s',imgpath='%s',type=%lu,xid=%lu,stime=%lu,etime=%lu,plateid=%lu where plevel=%lu",caScode,caAname,atol(caIndexid),atol(caOdityid),atol(caCouponid),atof(caDiscount),caAddress,caDecript,caImgPath,atol(caType),atol(caXid),lStime,lEtime,atol(caPlateid),atol(caPlevel));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }else{
	  	  	  // �����µĻ��Ϣ
	  	  	  pasDbOneRecord("select max(id) from ncmactive", 0, UT_TYPE_ULONG, 4, &lMaxid);
				    sprintf(caTemp,"insert into ncmactive (id,servicecode,aname,shopid,sid,cid,discount,address,decript,imgpath,plevel,type,xid,stime,etime,plateid) values (%lu,'%s','%s',%lu,%lu,%lu,%.2f,'%s','%s','%s',%lu,%lu,%lu,%lu,%lu,%lu)",(lMaxid+1),caScode,caAname,atol(caIndexid),atol(caOdityid),atol(caCouponid),atof(caDiscount),caAddress,caDecript,caImgPath,atol(caPlevel),atol(caType),atol(caXid),lStime,lEtime,atol(caPlateid));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }
	  }else if(strcmp(caKey, "update")==0){ // �޸�
	  	  if(lCount){
	  	  	  // ɾ��ԭλ�õ�ͼƬ
	  	  	  sprintf(caTemp2, "select imgpath from ncmactive where plevel=%lu", atol(caPlevel));
	  	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  	  if(strcmp(caImgPath, caImgpath1) != 0){
			  	  	  sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
						  	system(temp_file);
				    }
				  	
					 	sprintf(caTemp,"update ncmactive set servicecode='%s',aname='%s',shopid=%lu,sid=%lu,cid=%lu,discount=%.2f,address='%s',decript='%s',imgpath='%s',type=%lu,xid=%lu,stime=%lu,etime=%lu,plateid=%lu where plevel=%lu",caScode,caAname,atol(caIndexid),atol(caOdityid),atol(caCouponid),atof(caDiscount),caAddress,caDecript,caImgPath,atol(caType),atol(caXid),lStime,lEtime,atol(caPlateid),atol(caPlevel));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }else{
					 	sprintf(caTemp,"update ncmactive set servicecode='%s',aname='%s',shopid=%lu,sid=%lu,cid=%lu,discount=%.2f,address='%s',decript='%s',imgpath='%s',plevel=%lu,type=%lu,xid=%lu,stime=%lu,etime=%lu,plateid=%lu where id=%lu",caScode,caAname,atol(caIndexid),atol(caOdityid),atol(caCouponid),atof(caDiscount),caAddress,caDecript,caImgPath,atol(caPlevel),atol(caType),atol(caXid),lStime,lEtime,atol(caPlateid),atol(caId));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }
	  }
	  
	  utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","���ݼ��سɹ�!","���ݼ��سɹ�!");
    return 0;
}

// �̳Ƕ������
int ncm_webcomseries_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char id[20] = "";
	 char servicecode[16] = "";
	 char aname[64] = "";
	 char indexname[32] = "";
	 char odityname[32] = "";
	 char couponfee[20] = "";
	 char discount[20] = "";
	 char address[255] = "";
	 char decript[4096] = "";
	 char imgpath[128] = "";
	 char plevel[20] = "";
	 char type[20] = "";
	 char xid[20] = "";
	 char stime[32] = "";
	 char etime[32] = "";
	 char flags[32] = "";
	 char plateid[20] = "";
	 char temp_file[128] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lId = 0;
	 char 					caServicecode[16] = "";
	 char 					caAname[64] = "";
	 char 			    caIndexname[32] = "";
	 char 			    caOdityname[32] = "";
	 double         dCouponfee = 0.0;
	 double	        dDiscount = 0.0;
	 char           caAddress[255] = "";
	 char 					caDecript[4096] = "";
	 char 					caImgpath[128] = "";
	 unsigned long	lPlevel = 0;
	 unsigned long	lType = 0;
	 unsigned long	lXid = 0;
	 unsigned long	lStime = 0;
	 unsigned long	lEtime = 0;
	 unsigned long	lFlags = 0;
	 char           s_stime[32] = "";
	 char           s_etime[32] = "";
	 
	 char caUpdate[16] = "";
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
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 26,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"id",          UT_TYPE_STRING,  sizeof(id)-1,             id,
																	"servicecode", UT_TYPE_STRING,  sizeof(servicecode)-1,    servicecode,
																	"aname",       UT_TYPE_STRING,  sizeof(aname)-1,          aname,
																	"indexname",   UT_TYPE_STRING,  sizeof(indexname)-1,      indexname,
																	"odityname",   UT_TYPE_STRING,  sizeof(odityname)-1,      odityname,
																	"couponfee",   UT_TYPE_STRING,  sizeof(couponfee)-1,      couponfee,
																	"discount",    UT_TYPE_STRING,  sizeof(discount)-1,       discount,
																	"address",     UT_TYPE_STRING,  sizeof(address)-1,        address,
																	"decript",     UT_TYPE_STRING,  sizeof(decript)-1,        decript,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
																	"type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
																	"xid",         UT_TYPE_STRING,  sizeof(xid)-1,            xid,
																	"stime",       UT_TYPE_STRING,  sizeof(stime)-1,          stime,
																	"etime",       UT_TYPE_STRING,  sizeof(etime)-1,          etime,
																	"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"update",      UT_TYPE_STRING,  10,                       caUpdate,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport);
	 
	 trim(_keyword);
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(aname)>0){
			pasCvtGBK(2,aname,caTemp,64);
			strcpy(aname,caTemp);
   }
   if(strlen(indexname)>0){
			pasCvtGBK(2,indexname,caTemp,32);
			strcpy(indexname,caTemp);
   }
   if(strlen(odityname)>0){
			pasCvtGBK(2,odityname,caTemp,32);
			strcpy(odityname,caTemp);
   }
   if(strlen(address)>0){
			pasCvtGBK(2,address,caTemp,255);
			strcpy(address,caTemp);
   }
	 if(strlen(decript)>0){
			pasCvtGBK(2,decript,caTemp,sizeof(decript));
			strcpy(decript,caTemp);
   }
   if(strlen(stime)>0){
			pasCvtGBK(2,stime,caTemp,32);
			strcpy(stime,caTemp);
   }
   if(strlen(etime)>0){
			pasCvtGBK(2,etime,caTemp,32);
			strcpy(etime,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,128);
			strcpy(imgpath,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid));
			strcpy(caSesid,caTemp);
   }
   
   // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmactive where id in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
				 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
			       return 0;	
				  }
    		  
    		  // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
    		  
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmactive where id in(%s)",caSesid);
    	}
	 }
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmactive where 1=1 and type=2");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (aname like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select a.id, a.servicecode, a.aname, i.name, o.name, c.fee, a.discount, a.address, a.decript, a.imgpath, a.plevel, a.type, a.xid, a.stime, a.etime, a.flags from ncmactive a left join ncmcomindex i on a.shopid = i.sid left join ncmcomodity o on a.sid = o.sid left join ncmcoupon c on a.cid = c.cid where 1=1 and a.type=2");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (a.aname like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and a.plateid = %lu ", atol(plateid));
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by a.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
													     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
													     UT_TYPE_STRING, sizeof(caAname)-1,      caAname,
													     UT_TYPE_STRING, sizeof(caIndexname)-1,  caIndexname,
													     UT_TYPE_STRING, sizeof(caOdityname)-1,  caOdityname,
													     UT_TYPE_FLOAT,  sizeof(double),         &dCouponfee,
													     UT_TYPE_FLOAT,  sizeof(double),         &dDiscount,
													     UT_TYPE_STRING, sizeof(caAddress)-1,    caAddress,
													     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
													     UT_TYPE_ULONG,  sizeof(long),           &lPlevel,
													     UT_TYPE_ULONG,  sizeof(long),           &lType,
													     UT_TYPE_ULONG,  sizeof(long),           &lXid,
													     UT_TYPE_ULONG,  sizeof(long),           &lStime,
															 UT_TYPE_ULONG,  sizeof(long),           &lEtime,
															 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
		  
		   if(lStime>0){
		   		strcpy(s_stime, utTimFormat("%Y/%m/%d", lStime));	
		   }else{
		   		strcpy(s_stime, "");	
		   }
		   if(lEtime>0){
		   		strcpy(s_etime, utTimFormat("%Y/%m/%d", lEtime));	
		   }else{
		   		strcpy(s_etime, "");	
		   }
		   utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
			 utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
			 utPltPutLoopVar(psDbHead,"aname",iNum,caAname);
			 utPltPutLoopVar(psDbHead,"indexname",iNum,caIndexname);
			 utPltPutLoopVar(psDbHead,"odityname",iNum,caOdityname);
			 utPltPutLoopVarF(psDbHead,"couponfee",iNum,"%.2f",dCouponfee);
			 utPltPutLoopVarF(psDbHead,"discount",iNum,"%.2f",dDiscount);
			 utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
			 utPltPutLoopVar(psDbHead,"decript",iNum,caDecript);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",lPlevel);
			 utPltPutLoopVarF(psDbHead,"type",iNum,"%lu",lType);
			 utPltPutLoopVarF(psDbHead,"xid",iNum,"%lu",lXid);
			 utPltPutLoopVar(psDbHead,"stime",iNum,s_stime);
			 utPltPutLoopVar(psDbHead,"etime",iNum,s_etime);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltSetCvtHtml(0);
			 lId = 0;
			 memset(caServicecode, 0, sizeof(caServicecode));
			 memset(caAname, 0, sizeof(caAname));
			 memset(caIndexname, 0, sizeof(caIndexname));
			 memset(caOdityname, 0, sizeof(caOdityname));
			 dCouponfee = 0.0;
			 dDiscount = 0.0;
			 memset(caAddress, 0, sizeof(caAddress));
			 memset(caDecript, 0, sizeof(caDecript));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lPlevel = 0;
			 lType = 0;
			 lXid = 0;
			 lStime = 0;
			 lEtime = 0;
			 lFlags = 0;
			 memset(s_stime, 0, sizeof(s_stime));
			 memset(s_etime, 0, sizeof(s_etime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
															     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
															     UT_TYPE_STRING, sizeof(caAname)-1,      caAname,
															     UT_TYPE_STRING, sizeof(caIndexname)-1,  caIndexname,
															     UT_TYPE_STRING, sizeof(caOdityname)-1,  caOdityname,
															     UT_TYPE_FLOAT,  sizeof(double),         &dCouponfee,
															     UT_TYPE_FLOAT,  sizeof(double),         &dDiscount,
															     UT_TYPE_STRING, sizeof(caAddress)-1,    caAddress,
															     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
															     UT_TYPE_ULONG,  sizeof(long),           &lPlevel,
															     UT_TYPE_ULONG,  sizeof(long),           &lType,
															     UT_TYPE_ULONG,  sizeof(long),           &lXid,
															     UT_TYPE_ULONG,  sizeof(long),           &lStime,
																	 UT_TYPE_ULONG,  sizeof(long),           &lEtime,
																	 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomseries_list.htm");
	  return 0;	
}

int ncm_webcomseries_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
//#endif  
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 char caKey[32] = "";
	 char caSid[32] = "";
	 char caPlateid[20] = "";
	 unsigned long lPlateid = 0;
	 char caServicecode[16] = "";
	 char caAname[32] = "";
	 unsigned long lShopid = 0;
	 unsigned long lSid = 0;
	 unsigned long lCid = 0;
	 double fDiscount = 0.0;
	 char caAddress[255] = "";
	 char caDecript[4096] = "";
	 char caImgpath[128] = "";
	 unsigned long lPlevel = 0;
	 unsigned long lType = 0;
	 unsigned long lXid = 0;
	 unsigned long lStime = 0;
	 unsigned long lEtime = 0;
	 
	 char s_stime[32] = "";
	 char s_etime[32] = "";
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 3,"key",     UT_TYPE_STRING, sizeof(caKey)-1,     caKey,
	 															 "sid",     UT_TYPE_STRING, sizeof(caSid)-1,     caSid,
	 															 "plateid", UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
			 sprintf(sql, "select servicecode, aname, shopid, sid, cid, discount, address, decript, imgpath, plevel, type, xid, stime, etime, plateid from ncmactive where id=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,sizeof(caServicecode)-1,caServicecode,
			                      UT_TYPE_STRING,sizeof(caAname)-1,caAname,
			                      UT_TYPE_ULONG,sizeof(long),&lShopid,
			                      UT_TYPE_ULONG,sizeof(long),&lSid,
			                      UT_TYPE_ULONG,sizeof(long),&lCid,
			                      UT_TYPE_FLOAT,sizeof(double),&fDiscount,
			                      UT_TYPE_STRING,sizeof(caAddress)-1,caAddress,
		                        UT_TYPE_STRING,sizeof(caDecript)-1,caDecript,
		                        UT_TYPE_STRING,sizeof(caImgpath)-1,caImgpath,
		                        UT_TYPE_ULONG,sizeof(long),&lPlevel,
		                        UT_TYPE_ULONG,sizeof(long),&lType,
		                        UT_TYPE_ULONG,sizeof(long),&lXid,
		                        UT_TYPE_ULONG,sizeof(long),&lStime,
		                        UT_TYPE_ULONG,sizeof(long),&lEtime,
		                        UT_TYPE_ULONG,sizeof(long),&lPlateid);
		    
		    if(lStime>0){
		    		strcpy(s_stime, utTimFormat("%Y/%m/%d", lStime));	
		    }else{
		    		strcpy(s_stime, "");	
		    }            
			  if(lEtime>0){
			  		strcpy(s_etime, utTimFormat("%Y/%m/%d", lEtime));	
			  }else{
			  		strcpy(s_etime, "");	
			  }
			  utPltSetCvtHtml(1);
			  utPltPutVar(psDbHead,"id",caSid);
			  utPltPutVar(psDbHead,"scode",caServicecode);
				utPltPutVar(psDbHead,"aname",caAname);
				utPltPutVarF(psDbHead,"indexid","%lu",lShopid);
				utPltPutVarF(psDbHead,"odityid","%lu",lSid);
				utPltPutVarF(psDbHead,"couponid","%lu",lCid);
				utPltPutVarF(psDbHead,"discount","%.2f",fDiscount);
				utPltPutVar(psDbHead,"address",caAddress);
				utPltPutVar(psDbHead,"decript",caDecript);
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVarF(psDbHead,"plevel","%lu",lPlevel);
		    utPltPutVarF(psDbHead,"type","%lu",lType);
		    utPltPutVarF(psDbHead,"xid","%lu",lXid);
		    utPltPutVar(psDbHead,"stime",s_stime);
		    utPltPutVar(psDbHead,"etime",s_etime);
		    utPltPutVarF(psDbHead,"plateid","%lu",lPlateid);
		    utPltSetCvtHtml(0);
	 }else{
	 			utPltPutVarF(psDbHead,"plateid","%lu",atol(caPlateid));	
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomseries_dr.htm");
	 return 0;		
}

// �ϴ������ͼƬ
int ncm_webComSeriesImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32]="";
   	char caId[20]="";
   	char caScode[16]="";
    char caAname[32]="";
    char caIndexid[32]="";
    char caOdityid[32]="";
    char caCouponid[20] = "";
    char caDiscount[20]="";
    char caAddress[255]="";
    char caDecript[4096]="";
    char caImgPath[128]="";
    char caDispImgPath[128]="";
    char imgname[128]="";
    char caPlevel[20] = "";
    char caType[20] = "";
    char caXid[20] = "";
    char caStime[32]="";
    char caEtime[32]="";
    char caPlateid[20]="";
    
    char caImgpath1[128]="";
    char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
 		char caTemp2[1024]="";
 		char caTemp3[1024]="";
 		unsigned long lSysTime=0;
 		char *p;
 		
 		unsigned long lMaxid = 0;
		unsigned long lShopid = 0;
		unsigned long lSid=0;
 		unsigned long lCid=0;
 		unsigned long lStime=0;
 		unsigned long lEtime=0;
 		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
		
		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",(char *)ncLang("0194����"),(char *)ncLang("0183���ļ�����"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// ����ͼƬ���·��
  	sprintf(caImgPath,"%s/images/ShopImages/seriesimages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);
    
	  // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	  utMsgGetSomeNVar(psMsgHead, 17,"key",      UT_TYPE_STRING,  sizeof(caKey)-1,            caKey,
	  														 "id",         UT_TYPE_STRING,  sizeof(caId)-1,             caId,
	                               "scode",      UT_TYPE_STRING,  sizeof(caScode)-1,          caScode,
	  														 "aname",      UT_TYPE_STRING,  sizeof(caAname)-1,          caAname,
	  														 "inid",       UT_TYPE_STRING,  sizeof(caIndexid)-1,        caIndexid,
	  														 "odid",       UT_TYPE_STRING,  sizeof(caOdityid)-1,        caOdityid,
	  														 "cpid",       UT_TYPE_STRING,  sizeof(caCouponid)-1,       caCouponid,
	  														 "discount",   UT_TYPE_STRING,  sizeof(caDiscount)-1,       caDiscount,
	  														 "address",    UT_TYPE_STRING,  sizeof(caAddress)-1,        caAddress,
	  														 "decript",    UT_TYPE_STRING,  sizeof(caDecript)-1,        caDecript,
	  														 "fname",      UT_TYPE_STRING,  sizeof(imp_file)-1,         imp_file,
	  														 "plevel",     UT_TYPE_STRING,  sizeof(caPlevel)-1,         caPlevel,
	  														 "type",       UT_TYPE_STRING,  sizeof(caType)-1,           caType,
	  														 "xid",        UT_TYPE_STRING,  sizeof(caXid)-1,            caXid,
																 "stime",      UT_TYPE_STRING,  sizeof(caStime)-1,          caStime,
																 "etime",      UT_TYPE_STRING,  sizeof(caEtime)-1,          caEtime,
																 "plateid",    UT_TYPE_STRING,  sizeof(caPlateid)-1,        caPlateid);
	  
	  if(strlen(caAname)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","����Ʋ���Ϊ��!","����Ʋ���Ϊ��!");
        return 0;	
	  }
	                  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*��ָ�����ַ�����ȡһ����
		  	
		  	  ����ԭ�ͣ�
			����char *utStrGetWord(char *p,char *pOut,
			����                   short nMaxLen,char *pTerm);
			����
					����˵��:
			����p �C �����ַ���
			����pOut --- ����ַ���������Ҫ���ȷ���ÿռ�
			����nMaxLen --- ����ַ�������󳤶�
			����pTerm  ----- ��ֹ�������ַ�����ϵͳ��ɨ����ʱ�������ַ���ĳһ���ַ�ʱ������
			����
			��������ֵ��
			���������ַ���ָ�롣*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
				printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
					  memset(caDispImgPath, 0, sizeof(caDispImgPath));
				}else{
						sprintf(imgname, "series_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/seriesimages/");
				    strcat(caDispImgPath, imgname);	
				}
	  }
	  
	  sprintf(caTemp3, "select servicecode from ncmcomodity where sid = %lu", atol(caOdityid));
	  pasDbOneRecord(caTemp3,0,UT_TYPE_STRING,sizeof(caScode)-1,caScode);
	  ncTimeToLong(caStime, &lStime);
	  ncTimeToLong(caEtime, &lEtime);
	  
	  strcpy(caType, "2");
	  
	  if(strcmp(caKey, "update")==0){
	  	  sprintf(caTemp2, "select imgpath from ncmactive where id=%lu", atol(caId));
	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  if(strlen(caDispImgPath)==0){ // �Ƿ�ѡ���ϴ�ͼƬ, ��ԭͼƬ����
	  	  	  sprintf(caTemp,"update ncmactive set servicecode='%s',aname='%s',shopid=%lu,sid=%lu,cid=%lu,discount=%.2f,address='%s',decript='%s',imgpath='%s',type=%lu,xid=%lu,stime=%lu,etime=%lu,plateid=%lu where id=%lu",caScode,caAname,atol(caIndexid),atol(caOdityid),atol(caCouponid),atof(caDiscount),caAddress,caDecript,caImgpath1,atol(caType),atol(caXid),lStime,lEtime,atol(caPlateid),atol(caId));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caImgpath1);
	  	  }else{ // �ǣ�ԭͼƬɾ������ͼƬ����
			  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
				  	system(temp3_file);
			  	  sprintf(caTemp,"update ncmactive set servicecode='%s',aname='%s',shopid=%lu,sid=%lu,cid=%lu,discount=%.2f,address='%s',decript='%s',imgpath='%s',type=%lu,xid=%lu,stime=%lu,etime=%lu,plateid=%lu where id=%lu",caScode,caAname,atol(caIndexid),atol(caOdityid),atol(caCouponid),atof(caDiscount),caAddress,caDecript,caDispImgPath,atol(caType),atol(caXid),lStime,lEtime,atol(caPlateid),atol(caId));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
			  }
	  }else if(strcmp(caKey, "add")==0){
	  		pasDbOneRecord("select max(id) from ncmactive", 0, UT_TYPE_ULONG, 4, &lMaxid);
		    sprintf(caTemp,"insert into ncmactive (id,servicecode,aname,shopid,sid,cid,discount,address,decript,imgpath,plevel,type,xid,stime,etime,plateid) values (%lu,'%s','%s',%lu,%lu,%lu,%.2f,'%s','%s','%s',%lu,%lu,%lu,%lu,%lu,%lu)",(lMaxid+1),caScode,caAname,atol(caIndexid),atol(caOdityid),atol(caCouponid),atof(caDiscount),caAddress,caDecript,caDispImgPath,atol(caPlevel),atol(caType),atol(caXid),lStime,lEtime,atol(caPlateid));
			 	printf("caTemp = %s\n", caTemp);
			 	pasDbExecSqlF(caTemp);
			 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
	  }
	  
	  utPltPutVar(psDbHead,"mesg","���ݼ��سɹ�");
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomseries_dr_list.htm");
	  //utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","���ݼ��سɹ�!","���ݼ��سɹ�!");
    return 0;
}

// ��λ����
int ncm_websrvgroup_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 char temp1[4096] = "";
	 char temp2[4096] = "";
	 char temp3[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 char caShopids[1024];
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 strcpy(caShopids,getDsShopids());
	 int i=0;
	 typedef struct group_stat{
	 		unsigned long	groupid;
	    char groupname[32];
	 		char dispname[128];
	 		unsigned long	moditime;
			unsigned long	flags;
			unsigned long	areaid;
			unsigned long	cityid;
			unsigned long	provid;
			char s_moditime[32];
			char s_flags[32];
			unsigned long count;	
	 }group_stat_s;
	 group_stat_s *stat_temp;
	 
	 // ������Ϣ����
	 char groupid[32] = "";
	 char groupname[32] = "";
	 char dispname[128] = "";
	 char moditime[32] = "";
	 char flags[32] = "";
	 char provid[32] = "";
	 char cityid[32] = "";
	 char areaid[32] = "";
	 char typedid[32] = "";
	 char shaoid[32] = "";
	 char fdanid[32] = "";
	 char caKey[32] = "";
	 char groupcode[32] = "";
	 char careaid[32] = "";
	 	
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lGroupid = 0;
	 char 					caGroupname[32] = "";
	 char 					caDispname[128] = "";
	 unsigned long	lModitime = 0;
	 unsigned long	lFlags = 0;
	 unsigned long	lAreaid = 0;
	 unsigned long	lProvid = 0;
	 unsigned long	lCityid = 0;
	 char           caProvname[32] = "";
	 char           caCityname[32] = "";
	 char           caAreaname[32] = "";
	 char           s_moditime[32] = "";
	 char           s_flags[32] = "";
	 
	 char caUpdate[16] = "";
	 char caSesid[1024] = "";
	 char caDel[16] = "";
	 char caRefresh[32]="";
	 char caTemp[4096] = "";
	 char caTempd[4096] = "";
	 unsigned long lMax = 0;
	 unsigned long lCount = 0;
	 unsigned long lSid = 0;
	 char temp[32] = "";
	 char left[20] = "";
	 char left1[20] = "";
	 char left2[20] = "";
	 // �ؼ���
	 char _keyword[33] = "";
	 
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
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
   // �õ��̼�stype
	 char caStyped[32] = "";
	 char caOptd[32] = "";
	 unsigned long lCid = 0;
    dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	 dsCltGetSessionValue(1, "stype", UT_TYPE_STRING, sizeof(caStyped)-1, caStyped);
	 dsCltGetSessionValue(1, "opt", UT_TYPE_STRING, sizeof(caOptd)-1, caOptd);
	 printf("lCid = %d\n", lCid);
	 printf("caStyped = %s\n", caStyped);
	 printf("caOptd = %s\n", caOptd);
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 24,"keyword",   UT_TYPE_STRING, sizeof(_keyword)-1,  _keyword,
	                                "groupid",   UT_TYPE_STRING, sizeof(groupid)-1,   groupid,
	                                "groupname", UT_TYPE_STRING, sizeof(groupname)-1, groupname,
	                                "dispname",  UT_TYPE_STRING, sizeof(dispname)-1,  dispname,
	                                "moditime",  UT_TYPE_STRING, sizeof(moditime)-1,  moditime,
	                                "flags",     UT_TYPE_STRING, sizeof(flags)-1,     flags,
	                                "prov",      UT_TYPE_STRING, sizeof(provid)-1,    provid,
	                                "city",      UT_TYPE_STRING, sizeof(cityid)-1,    cityid,
	                                "area",      UT_TYPE_STRING, sizeof(areaid)-1,    areaid,
	                                "type",      UT_TYPE_STRING, sizeof(typedid)-1,   typedid,
	                                "shao",      UT_TYPE_STRING, sizeof(shaoid)-1,    shaoid,
	                                "fdan",      UT_TYPE_STRING, sizeof(fdanid)-1,    fdanid,
	                                "key",       UT_TYPE_STRING, sizeof(caKey)-1,     caKey,
	                                "groupcode", UT_TYPE_STRING, sizeof(groupcode)-1, groupcode,
	                                "areaid",    UT_TYPE_STRING, sizeof(careaid)-1,   careaid,
	                                "start",     UT_TYPE_STRING, sizeof(start)-1,     start,
															   	"limit",     UT_TYPE_STRING, sizeof(limit)-1,     limit,
															   	"sort",      UT_TYPE_STRING, sizeof(sort)-1,      sort,
															   	"dir",       UT_TYPE_STRING, sizeof(dir)-1,       dir,
																	"selsid",    UT_TYPE_STRING, 1000,                caSesid,
																	"del",			 UT_TYPE_STRING, 10,                  caDel,
																	"refresh",	 UT_TYPE_STRING, sizeof(caRefresh)-1, caRefresh,
																	"update",    UT_TYPE_STRING, 10,                  caUpdate,
																	"caExport",  UT_TYPE_STRING, 10,                  caExport);
	 
	 // ����
	 if(strlen(sort)==0){
	 	  strcpy(sort,"groupid");
	 }
	 // ȥ���ؼ���ǰ��Ŀո�
	 trim(_keyword);
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(groupname)>0){
			pasCvtGBK(2,groupname,caTemp,32);
			strcpy(groupname,caTemp);
   }
	 if(strlen(dispname)>0){
			pasCvtGBK(2,dispname,caTemp,128);
			strcpy(dispname,caTemp);
   }
   
   // ����/�޸�
	 if(!utStrIsSpaces(caKey)) {
	 	  if(strlen(typedid)==0){
  	  		strcpy(typedid, "0");	
  	  }
  	  if(strlen(shaoid)==0){
  	  		strcpy(shaoid, "000");	
  	  }
	 		if (strcmp(caKey, "update")==0){
		  	  sprintf(left, "%s%s%s", areaid, typedid, shaoid);
			 	  printf("left = %s\n", left);
			 	  
			 	  sprintf(caTemp,"update ncsrvgroup set groupname='%s', dispname='%s', moditime=%lu, flags=%lu where groupid='%s'",groupname,dispname,time(0),atol(flags),groupid);
			 				printf("caTemp = %s\n", caTemp);
					 		pasDbExecSqlF(caTemp);	
			 	  
	/* 			  
	 	  	  sprintf(caTempd, "select left(groupname, 12), left(groupname, 14) from ncsrvgroup where groupid='%s'", groupid);
	 	  	  pasDbOneRecord(caTempd, 0, UT_TYPE_STRING, sizeof(left1)-1, left1,
	 	  	  													 UT_TYPE_STRING, sizeof(left2)-1, left2);
	 	  	  if(strcmp(left1, left)==0 && strcmp(left2, fdanid)==0){
	 	  	  	  printf("***************����****************\n");
	 	  	  		sprintf(caTemp,"update ncsrvgroup set groupname='%s', dispname='%s', moditime=%lu, flags=%lu where groupid='%s'",groupname,dispname,time(0),atol(flags),groupid);
			 				printf("caTemp = %s\n", caTemp);
					 		pasDbExecSqlF(caTemp);	
	 	  	  }else{
	 	  	  	  printf("***************�ı�****************\n");
	 	  	  	  if(strcmp(left2, fdanid)==0 && strcmp(left1, left)!=0){
	 	  	  	  		sprintf(left, "%s%s%s%s", areaid, typedid, shaoid, "01");	
	 	  	  	  }else{
	 	  	  	  		strcpy(left, fdanid);		
	 	  	  	  }
	 	  	  	  printf("left = %s\n", left);
	 	  	  	  
			 	  		sprintf(caTempd,"select max(substring(groupname, 15, 2)) from ncsrvgroup where left(groupname, 14)='%s' group by '%s' ", left, left);
					 	  pasDbOneRecord(caTempd, 0, UT_TYPE_ULONG, sizeof(long), &lMax);
					 	  sprintf(temp, "%s%02d", left, lMax+1);		
					 	  printf("temp = %s\n", temp);
					 	  
					 	  lSid=utMd5Code(temp,strlen(temp),"ffff");
				   	  if(lSid>2000000000) lSid=lSid-2000000000;
				   	  if(lSid>2000000000) lSid=lSid-2000000000;
				   	  printf("lSid = %d\n",lSid);	
				   	  
				   	  sprintf(caTemp,"update ncsrvgroup set groupid=%lu, groupname='%s', dispname='%s', moditime=%lu, flags=%lu where groupid='%s'",lSid,temp,dispname,time(0),atol(flags),groupid);
			 				printf("caTemp = %s\n", caTemp);
					 		pasDbExecSqlF(caTemp);
	*/
		//	 	  }
	 	  }else if(strcmp(caKey, "add")==0){
	 	  	  if(strlen(fdanid)==0){
		  	  		sprintf(left, "%s%s%s%s", areaid, typedid, shaoid, "01");
		  	  }else{
		  	  		strcpy(left, fdanid);	
		  	  }
			 	  printf("left = %s\n", left);
			 	  long lLenleft=strlen(left);
	 	  	  // sprintf(caTempd,"select max(substring(groupname, 13, 4)) from ncsrvgroup where left(groupname, 12)='%s' group by '%s' ", left, left);
	 	  	   sprintf(caTempd,"select max(substring(groupname, 15, 2)) from ncsrvgroup where left(groupname, %d)='%s' group by '%s' ", lLenleft,left, left);
	 	  	   printf("caTempd=%s\n",caTempd);
			 	  pasDbOneRecord(caTempd, 0, UT_TYPE_ULONG, sizeof(long), &lMax);
			 	  sprintf(temp, "%s%02d", left, lMax+1);
			 	  printf("temp = %s\n", temp);
			 	  
			 	  lSid=utMd5Code(temp,strlen(temp),"ffff");
		   	  if(lSid>2000000000) lSid=lSid-2000000000;
		   	  if(lSid>2000000000) lSid=lSid-2000000000;
		   	  printf("lSid = %d\n",lSid);
	 	  	
		    	sprintf(caTemp,"insert into ncsrvgroup (groupid,groupname,dispname,moditime,flags) values (%lu,'%s','%s',%lu,%lu)",lSid,temp,dispname,time(0),atol(flags));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 	  utPltPutVar(psDbHead,"mesg","���ݼ��سɹ�");
	 	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_websrvgroup_dr_list.htm");
      return 0;
	 }
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from ncsrvgroup where groupid in(%s)",caSesid);
    	}
	 }
	 
	 // ˢ��
	 if(!utStrIsSpaces(caRefresh)) {
    	ncSrvSysReset(psShmHead);
	 }
	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncsrvgroup where 1=1 and flags=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%')", _keyword);
	 }
	 if(lCid>0){  // ���Զ��壬С�̼�
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupid = %lu", lCid);	
	 }
	 if(!utStrIsSpaces(caStyped)){
	 		//snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and substring(groupname, 9, 2) = '%s'", caStyped);
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and substring(groupname, 9, 2) = '%s'", caStyped);
	 }
	 if(!utStrIsSpaces(caOptd)){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupname like '%s%c'", caOptd, '%');
	 }
	 if(!utStrIsSpaces(groupcode))  // �̼�Ȩ�޲�ѯ
	 {
	 	  char caOpt[64]="";
	 	  sprintf(caOpt,"%s%s",ncmWebAutoDefGet16(psShmHead, iFd,psMsgHead,caStyped),groupcode);
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupname like '%s%c'", caOpt, '%');
	 }	
	 if(!utStrIsSpaces(careaid))  // �����ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupname like '%s%c'", careaid, '%');
	 }
	 if(!utStrIsSpaces(groupid))  // ��λ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupid = %lu", atol(groupid));
	 }
	  if(strlen(caShopids)>0){
    	sprintf(sql+strlen(sql)," and groupid in (%s) ",caShopids);
    }
	  pasLogs(PAS_SRCFILE,9001,"sql=%s ",sql);
	 
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 stat_temp = (group_stat_s *)malloc(sizeof(group_stat_s)*lCount1+1);
	 for(i=0; i<lCount1; i++){
	 			stat_temp[i].groupid = 0;
	 			strcpy(stat_temp[i].groupname, "");
	 			strcpy(stat_temp[i].dispname, "");
	 			stat_temp[i].moditime = 0;
	 			stat_temp[i].flags = 0;
	 			stat_temp[i].provid = 0;
	 			stat_temp[i].cityid = 0;
	 			stat_temp[i].areaid = 0;
	 			strcpy(stat_temp[i].s_moditime, "");
	 			strcpy(stat_temp[i].s_flags, "");
	 			stat_temp[i].count = 0;
	 }
	 
	 strcpy(sql, "select groupid, groupname, dispname, moditime, flags, left(groupname, 2), left(groupname, 4), left(groupname, 6) from ncsrvgroup where 1=1 and flags=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%')", _keyword);
	 }
	 if(lCid>0){  // ���Զ��壬С�̼�
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupid = %lu", lCid);	
	 }
	 if(!utStrIsSpaces(caStyped)){
	 		//snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and substring(groupname, 9, 2) = '%s'", caStyped);
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and substring(groupname, 9, 2) = '%s'", caStyped);
	 }
	 if(!utStrIsSpaces(caOptd)){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupname like '%s%c'", caOptd, '%');
	 }
	 if(!utStrIsSpaces(groupcode))  // �̼�Ȩ�޲�ѯ
	 {
	 	  char caOpt[64]="";
	 	  sprintf(caOpt,"%s%s",ncmWebAutoDefGet16(psShmHead, iFd,psMsgHead,caStyped),groupcode);
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupname like '%s%c'", caOpt, '%');
	 }
	 if(!utStrIsSpaces(careaid))  // �����ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupname like '%s%c'", careaid, '%');
	 }
	 if(!utStrIsSpaces(groupid))  // ��λ��ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupid = %lu", atol(groupid));
	 } 
	 printf("caShopids=%s\n",caShopids);
	 	if(strlen(caShopids)>0){
    	sprintf(sql+strlen(sql)," and groupid in (%s) ",caShopids);
    }
	 
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 pasLogs(PAS_SRCFILE,9001,"sql=%s ",sql);
	 
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
															 UT_TYPE_STRING, sizeof(caGroupname)-1, caGroupname,
															 UT_TYPE_STRING, sizeof(caDispname)-1,  caDispname,
													     UT_TYPE_ULONG,  sizeof(long),          &lModitime,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags,
															 UT_TYPE_ULONG,  sizeof(long),          &lProvid,
															 UT_TYPE_ULONG,  sizeof(long),          &lCityid,	
															 UT_TYPE_ULONG,  sizeof(long),          &lAreaid);
				
				
	  while(0 == iret || 1405 == iret){
			 if(lModitime>0){
			 			strcpy(s_moditime, utTimFormat("%Y/%m/%d %H:%M:%S", lModitime));
			 }else{
			 		strcpy(s_moditime, "");
			 }
			 
			 switch(lFlags){
			 		case 0:strcpy(s_flags, "��Ч");break;
			 		case 1:strcpy(s_flags, "��Ч");break;
			 		case 9:strcpy(s_flags, "ɾ��");break;
			 		default:break;
			 }
			 
			 stat_temp[iNum].groupid = lGroupid;
	 		 strcpy(stat_temp[iNum].groupname, caGroupname);
	 		 strcpy(stat_temp[iNum].dispname, caDispname);
	 		 stat_temp[iNum].moditime = lModitime;
	 		 stat_temp[iNum].flags = lFlags;
	 		 stat_temp[iNum].provid = lProvid;
	 		 stat_temp[iNum].cityid = lCityid;
	 		 stat_temp[iNum].areaid = lAreaid;
	 		 strcpy(stat_temp[iNum].s_moditime, s_moditime);
	 		 strcpy(stat_temp[iNum].s_flags, s_flags);
			 
			 iNum++;
			 lGroupid = 0;
			 memset(caGroupname, 0, sizeof(caGroupname));
			 memset(caDispname, 0, sizeof(caDispname));
			 lModitime = 0;
	     lFlags = 0;
	     lAreaid = 0;
	     lCityid = 0;
	     lProvid = 0;
	     memset(caAreaname, 0, sizeof(caAreaname));
	     memset(s_moditime, 0, sizeof(s_moditime));
	     memset(s_flags, 0, sizeof(s_flags));
	     iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
																	 UT_TYPE_STRING, sizeof(caGroupname)-1, caGroupname,
																	 UT_TYPE_STRING, sizeof(caDispname)-1,  caDispname,
															     UT_TYPE_ULONG,  sizeof(long),          &lModitime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags,
																	 UT_TYPE_ULONG,  sizeof(long),          &lProvid,
																	 UT_TYPE_ULONG,  sizeof(long),          &lCityid,	
																	 UT_TYPE_ULONG,  sizeof(long),          &lAreaid);	
	  }
	  pasDbCloseCursor(psCur);
	   
	  int inum = 0;
	  for(i=0; i<iNum; i++){
	  	  inum++;
	  	  if(inum > 1)
			  {
				   utPltPutLoopVar(psDbHead,"dh",inum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			  }
			  
			  sprintf(sql, "select count(*) from ncsrvgroupap where groupid=%lu", stat_temp[i].groupid);
	  		pasDbOneRecord(sql, 0, UT_TYPE_ULONG, sizeof(long), &stat_temp[i].count);
	  		
	  		sprintf(temp1, "select aname from ncmarea where aid = %lu", stat_temp[i].areaid);
	  		pasDbOneRecord(temp1, 0, UT_TYPE_STRING, sizeof(caAreaname)-1, caAreaname);
	  		
	  		sprintf(temp1, "select aname from ncmarea where aid = %lu", stat_temp[i].cityid);
	  		pasDbOneRecord(temp1, 0, UT_TYPE_STRING, sizeof(caCityname)-1, caCityname);
	  	pasLogs(PAS_SRCFILE,9001,"temp1=%s ",temp1);	
	  		sprintf(temp1, "select aname from ncmarea where aid = %lu", stat_temp[i].provid);
	  		pasDbOneRecord(temp1, 0, UT_TYPE_STRING, sizeof(caProvname)-1, caProvname);
			  
			  char tempname[128] = "";
			  sprintf(tempname, "%s%s%s", caProvname,caCityname,caAreaname);
			  
	  		utPltPutLoopVarF(psDbHead,"groupid",inum,"%lu",stat_temp[i].groupid);
			  utPltPutLoopVar(psDbHead,"groupname",inum,stat_temp[i].groupname);
			  utPltPutLoopVar(psDbHead,"dispname",inum,stat_temp[i].dispname);
			  utPltPutLoopVar(psDbHead,"moditime",inum,stat_temp[i].s_moditime);
			  utPltPutLoopVarF(psDbHead,"flags",inum,"%lu",stat_temp[i].flags);
			  utPltPutLoopVar(psDbHead,"areaname",inum,tempname);
			  utPltPutLoopVar(psDbHead,"s_flags",inum,stat_temp[i].s_flags);
			  utPltPutLoopVarF(psDbHead,"count",inum,"%lu",stat_temp[i].count);
			  
			  memset(caProvname, 0, sizeof(caProvname));
			  memset(caCityname, 0, sizeof(caCityname));
			  memset(caAreaname, 0, sizeof(caAreaname));
			  memset(tempname, 0, sizeof(tempname));
	  }
	  //utPltShowDb(psDbHead);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_websrvgroup_list.htm");
	  return 0;	
}

int ncm_websrvgroup_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
//#endif  
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 char caKey[32] = "";
	 char caGroupid[32] = "";
	 
	 char caGroupname[32] = "";
	 char caDispname[128] = "";
	 unsigned long lFlags = 0;
	 unsigned long lAreaid = 0;
	 unsigned long lCityid = 0;
	 unsigned long lProvid = 0;
	 char caTypeid[20] = "";
	 char caShaoid[20] = "";
	 char caFdanid[20] = "";
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 2, "key",     UT_TYPE_STRING,     sizeof(caKey)-1,         caKey,
	                                "groupid", UT_TYPE_STRING,     sizeof(caGroupid)-1,     caGroupid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caGroupid)>0){
			 //sprintf(sql, "select substring(groupname, 13, 4), dispname, flags, left(groupname, 6), left(groupname, 4), left(groupname, 2), substring(groupname, 7, 2), substring(groupname, 9, 2), left(groupname, 12) from ncsrvgroup where groupid=%d", atol(caGroupid));
			 sprintf(sql, "select substring(groupname, 13, 4), dispname, flags, left(groupname, 6), left(groupname, 4), left(groupname, 2), substring(groupname, 7, 2), substring(groupname, 9, 2), left(groupname, 12) from ncsrvgroup where groupid=%d", atol(caGroupid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,sizeof(caGroupname)-1,    caGroupname,
			 											UT_TYPE_STRING,sizeof(caDispname)-1,     caDispname,
			 											UT_TYPE_ULONG, sizeof(long),             &lFlags,
		                        UT_TYPE_ULONG, sizeof(long),             &lAreaid,
		                        UT_TYPE_ULONG, sizeof(long),             &lCityid,
		                        UT_TYPE_ULONG, sizeof(long),             &lProvid,
		                        UT_TYPE_STRING,sizeof(caTypeid)-1,       caTypeid,
		                        UT_TYPE_STRING,sizeof(caShaoid)-1,       caShaoid,
		                        UT_TYPE_STRING,sizeof(caFdanid)-1,       caFdanid);
		                        
		    utPltSetCvtHtml(1);
			  utPltPutVar(psDbHead,"groupid",caGroupid);
				utPltPutVar(psDbHead,"groupname",caGroupname);
				utPltPutVar(psDbHead,"dispname",caDispname);
				utPltPutVarF(psDbHead,"flags","%lu",lFlags);
		    utPltPutVarF(psDbHead,"areaid","%lu",lAreaid);
		    utPltPutVarF(psDbHead,"cityid","%lu",lCityid);
		    utPltPutVarF(psDbHead,"provid","%lu",lProvid);
		    utPltPutVar(psDbHead,"typeid",caTypeid);
		    utPltPutVar(psDbHead,"shaoid",caShaoid);
		    utPltPutVar(psDbHead,"fdanid",caFdanid);
		    utPltSetCvtHtml(0);
	 }else{
	 		  utPltPutVarF(psDbHead,"flags","%lu",1);	
	 }
	 
	 utPltPutVar(psDbHead,"key",caKey);
	 utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_websrvgroup_dr.htm");
	 return 0;		
}

// AP����
int ncm_websrvgroupap_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char pregwid[32] = "";
	 char groupid[32] = "";
	 char gwid[32] = "";
	 char address[255] = "";
	 char longitude[64] = "";
	 char latitude[64] = "";
	 char addtime[32] = "";
	 char flags[32] = "";
	 	
	 // ������ݿ�ȡ�������ݵı���
	 long long	    lGroupid = 0;
	 char 					caGwid[32] = "";
	 char           caAddress[255] = "";
	 double         dLongitude = 0.0;
	 double         dLatitude = 0.0;
	 unsigned long	lAddtime = 0;
	 unsigned long	lFlags = 0;
	 char           s_addtime[32] = "";
	 char           s_flags[32] = "";
	 
	 char caUpdate[16] = "";
	 char caSesid[4096] = "";
	 char caDel[16] = "";
	 char caRefresh[32]="";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
	 unsigned long lCount = 0;
	 
	 unsigned long lSid = 0;
	 char Gwid[32] = "";
	 // �ؼ���
	 char _keyword[33] = "";
	 
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
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
   
   /*unsigned long lShopId = 0;
	 dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	 printf("lShopId = %lu\n", lShopId);*/
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 18,"keyword",   UT_TYPE_STRING, sizeof(_keyword)-1,  _keyword,
	                                "pregwid",   UT_TYPE_STRING, sizeof(pregwid)-1,   pregwid,
	                                "groupid",   UT_TYPE_STRING, sizeof(groupid)-1,   groupid,
	                                "gwid",      UT_TYPE_STRING, sizeof(gwid)-1,      gwid,
	                                "address",   UT_TYPE_STRING, sizeof(address)-1,   address,
	                                "longitude", UT_TYPE_STRING, sizeof(longitude)-1, longitude,
	                                "latitude",  UT_TYPE_STRING, sizeof(latitude)-1,  latitude,
	                                "addtime",   UT_TYPE_STRING, sizeof(addtime)-1,   addtime,
	                                "flags",     UT_TYPE_STRING, sizeof(flags)-1,     flags,
	                                "start",     UT_TYPE_STRING, sizeof(start)-1,     start,
															   	"limit",     UT_TYPE_STRING, sizeof(limit)-1,     limit,
															   	"sort",      UT_TYPE_STRING, sizeof(sort)-1,      sort,
															   	"dir",       UT_TYPE_STRING, sizeof(dir)-1,       dir,
																	"selsid",    UT_TYPE_STRING, sizeof(caSesid)-1,   caSesid,
																	"del",			 UT_TYPE_STRING, sizeof(caDel)-1,     caDel,
																	"refresh",	 UT_TYPE_STRING, sizeof(caRefresh)-1, caRefresh,
																	"update",    UT_TYPE_STRING, sizeof(caUpdate)-1,  caUpdate,
																	"caExport",  UT_TYPE_STRING, sizeof(caExport)-1,  caExport);
	 
	 // ����
	 if(strlen(sort)==0){
	 	  strcpy(sort,"addtime");
	 }
	 // ȥ���ؼ���ǰ��Ŀո�
	 trim(_keyword);
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(gwid)>0){
			pasCvtGBK(2,gwid,caTemp,32);
			strcpy(gwid,caTemp);
   }
   if(strlen(address)>0){
			pasCvtGBK(2,address,caTemp,255);
			strcpy(address,caTemp);
   }
   
   // ����/�޸�
	 if(!utStrIsSpaces(caUpdate)) {
	 		if (strcmp(caUpdate, "update")==0){
	 				sprintf(caTemp,"update ncsrvgroupap set groupid='%s', gwid='%s', address='%s', longitude=%.8f, latitude=%.8f, addtime=%lu, flags=%lu where gwid='%s'",groupid,gwid,address,atof(longitude),atof(latitude),time(0),atol(flags),pregwid);
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }else if(strcmp(caUpdate, "add")==0){
	 	  	  sprintf(caTemp1, "select count(gwid) from ncsrvgroupap where gwid='%s'", gwid);
	 	  	  printf("caTemp1 = %s\n", caTemp1);
	 	  	  pasDbOneRecord(caTemp1,0,UT_TYPE_ULONG,sizeof(long),&lCount);
	 	  	  printf("lCount = %lu\n", lCount);
	 	  	  if(lCount>0){
	 	  	  	sprintf(caTemp,"update ncsrvgroupap set groupid=%s,address='%s',longitude=%.8f,latitude=%.8f,addtime=%d,flags=%d where gwid='%s' ",groupid,address,atof(longitude),atof(latitude),time(0),atol(flags),gwid);
	 	  	    pasDbExecSqlF(caTemp);
//	            utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back_new.htm","AP�Ѵ���!","AP�Ѵ���!");
//	 	  	  		return 0;	
	 	  	  }else{
		 	  	  	sprintf(caTemp,"insert into ncsrvgroupap (groupid,gwid,address,longitude,latitude,addtime,flags) values ('%s','%s','%s',%.8f,%.8f,%lu,%lu)",groupid,gwid,address,atof(longitude),atof(latitude),time(0),atol(flags));
				 		  printf("caTemp = %s\n", caTemp);
				 		  pasDbExecSqlF(caTemp);
	 	  	  }
	 	  }
	 }
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from ncsrvgroupap where gwid in(%s)",caSesid);
    	}
	 }
	 // ˢ��
	 if(!utStrIsSpaces(caRefresh)) {
    	ncSrvSystemInit(psShmHead);
	 }
	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncsrvgroupap where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (gwid like '%%%s%%')", _keyword);
	 }
	 if (!utStrIsSpaces(groupid)) // ÿ���̼��Լ����Լ���
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupid = '%s' ", groupid);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select groupid, gwid, address, longitude, latitude, addtime, flags from ncsrvgroupap where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (gwid like '%%%s%%')", _keyword);
	 }
	 if (!utStrIsSpaces(groupid)) // ÿ���̼��Լ����Լ���
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupid = '%s' ", groupid);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_LONG8,  sizeof(long long),     &lGroupid,
															 UT_TYPE_STRING, sizeof(caGwid)-1,      caGwid,
															 UT_TYPE_STRING, sizeof(caAddress)-1,   caAddress,
															 UT_TYPE_FLOAT,  sizeof(double),        &dLongitude,
															 UT_TYPE_FLOAT,  sizeof(double),        &dLatitude,
													     UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags);	
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
			 if(lAddtime>0){
			 		strcpy(s_addtime, utTimFormat("%Y/%m/%d %H:%M:%S", lAddtime));
			 }else{
			 		strcpy(s_addtime, "");
			 }
			 
			 switch(lFlags){
			 		case 0:strcpy(s_flags, "δ��ͨ");break;
			 		case 1:strcpy(s_flags, "����ʹ��");break;
			 		default:break;
			 }
			 
			 utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
			 utPltPutLoopVar(psDbHead,"gwid",iNum,caGwid);
			 utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
			 utPltPutLoopVarF(psDbHead,"longitude",iNum,"%.8f",dLongitude);
			 utPltPutLoopVarF(psDbHead,"latitude",iNum,"%.8f",dLatitude);
			 utPltPutLoopVar(psDbHead,"addtime",iNum,s_addtime);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltPutLoopVar(psDbHead,"s_flags",iNum,s_flags);
			 
			 lGroupid = 0;
			 memset(caGwid, 0, sizeof(caGwid));
			 memset(caAddress, 0, sizeof(caAddress));
			 dLongitude = 0.0;
			 dLatitude = 0.0;
			 lAddtime = 0;
	     lFlags = 0;
	     memset(s_addtime, 0, sizeof(s_addtime));
	     memset(s_flags, 0, sizeof(s_flags));
	     iret = pasDbFetchInto(psCur,UT_TYPE_LONG8,  sizeof(long long),     &lGroupid,
																	 UT_TYPE_STRING, sizeof(caGwid)-1,      caGwid,
																	 UT_TYPE_STRING, sizeof(caAddress)-1,   caAddress,
																	 UT_TYPE_FLOAT,  sizeof(double),        &dLongitude,
																	 UT_TYPE_FLOAT,  sizeof(double),        &dLatitude,
															     UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags);	
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_websrvgroupap_list.htm");
	  return 0;	
}

// SSID����
int ncm_websrvgroupssid_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char sid[20] = "";
	 char groupid[32] = "";
	 char ssid[32] = "";
	 char addtime[32] = "";
	 char moditime[32] = "";
	 char flags[32] = "";
	 	
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lSid = 0;
	 unsigned long	lGroupid = 0;
	 char         	caGroupname[32] = "";
	 char 					caSsid[32] = "";
	 unsigned long	lAddtime = 0;
	 unsigned long	lModitime = 0;
	 unsigned long	lFlags = 0;
	 char           s_addtime[32] = "";
	 char           s_moditime[32] = "";
	 char           s_flags[32] = "";
	 
	 char caUpdate[16] = "";
	 char caSesid[1024] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 unsigned long	lMaxid = 0;
	 // �ؼ���
	 char _keyword[33] = "";
	 
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
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 15,"keyword",   UT_TYPE_STRING, sizeof(_keyword)-1,  _keyword,
	 																"sid",       UT_TYPE_STRING, sizeof(sid)-1,       sid,
	                                "groupid",   UT_TYPE_STRING, sizeof(groupid)-1,   groupid,
	                                "ssid",      UT_TYPE_STRING, sizeof(ssid)-1,      ssid,
	                                "addtime",   UT_TYPE_STRING, sizeof(addtime)-1,   addtime,
	                                "moditime",  UT_TYPE_STRING, sizeof(moditime)-1,  moditime,
	                                "flags",     UT_TYPE_STRING, sizeof(flags)-1,     flags,
	                                "start",     UT_TYPE_STRING, sizeof(start)-1,     start,
															   	"limit",     UT_TYPE_STRING, sizeof(limit)-1,     limit,
															   	"sort",      UT_TYPE_STRING, sizeof(sort)-1,      sort,
															   	"dir",       UT_TYPE_STRING, sizeof(dir)-1,       dir,
																	"selsid",    UT_TYPE_STRING, sizeof(caSesid)-1,   caSesid,
																	"del",			 UT_TYPE_STRING, sizeof(caDel)-1,     caDel,
																	"update",    UT_TYPE_STRING, sizeof(caUpdate)-1,  caUpdate,
																	"caExport",  UT_TYPE_STRING, sizeof(caExport)-1,  caExport);
	 
	 // ����
	 if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
	 // ȥ���ؼ���ǰ��Ŀո�
	 trim(_keyword);
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(ssid)>0){
			pasCvtGBK(2,ssid,caTemp,32);
			strcpy(ssid,caTemp);
   }
	 
   // ����/�޸�
	 if(!utStrIsSpaces(caUpdate)) {
	 		if (strcmp(caUpdate, "update")==0){
	 				sprintf(caTemp,"update ncsrvgroupssid set groupid=%lu, ssid='%s', addtime=%lu, moditime=%lu, flags=%lu where sid=%lu", atol(groupid),ssid,atol(addtime),time(0),atol(flags),atol(sid));
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }else if(strcmp(caUpdate, "add")==0){
	 	  	  pasDbOneRecord("select max(sid) from ncsrvgroupssid", 0, UT_TYPE_ULONG, 4, &lMaxid);
		    	sprintf(caTemp,"insert into ncsrvgroupssid (sid,groupid,ssid,addtime,moditime,flags) values (%lu,%lu,'%s',%lu,%lu,%lu)",(lMaxid+1),atol(groupid),ssid,time(0),time(0),atol(flags));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from ncsrvgroupssid where sid in(%s)",caSesid);
    	}
	 }
	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncsrvgroupssid where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (ssid like '%%%s%%')", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select s.sid, s.groupid, g.dispname, s.ssid, s.addtime, s.moditime, s.flags from ncsrvgroupssid s left join ncsrvgroup g on s.groupid=g.groupid where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (s.ssid like '%%%s%%')", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by s.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
	                             UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
	                             UT_TYPE_STRING, sizeof(caGroupname)-1, caGroupname,
															 UT_TYPE_STRING, sizeof(caSsid)-1,      caSsid,
													     UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
													     UT_TYPE_ULONG,  sizeof(long),          &lModitime,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags);	
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
			 if(lAddtime>0){
			 		strcpy(s_addtime, utTimFormat("%Y/%m/%d %H:%M:%S", lAddtime));
			 }else{
			 		strcpy(s_addtime, "");
			 }
			 if(lModitime>0){
			 		strcpy(s_moditime, utTimFormat("%Y/%m/%d %H:%M:%S", lModitime));
			 }else{
			 		strcpy(s_moditime, "");
			 }
			 
			 switch(lFlags){
			 		case 0:strcpy(s_flags, "δ��ͨ");break;
			 		case 1:strcpy(s_flags, "����ʹ��");break;
			 		default:break;
			 }
			 
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
			 utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
			 utPltPutLoopVar(psDbHead,"ssid",iNum,caSsid);
			 utPltPutLoopVarF(psDbHead,"addtime",iNum,"%lu",lAddtime);
			 utPltPutLoopVar(psDbHead,"s_addtime",iNum,s_addtime);
			 utPltPutLoopVar(psDbHead,"moditime",iNum,s_moditime);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltPutLoopVar(psDbHead,"s_flags",iNum,s_flags);
			 
			 lSid = 0;
			 lGroupid = 0;
			 memset(caGroupname, 0, sizeof(caGroupname));
			 memset(caSsid, 0, sizeof(caSsid));
			 lAddtime = 0;
			 lModitime = 0;
	     lFlags = 0;
	     memset(s_addtime, 0, sizeof(s_addtime));
	     memset(s_moditime, 0, sizeof(s_moditime));
	     memset(s_flags, 0, sizeof(s_flags));
	     iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
	                                 UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
			                             UT_TYPE_STRING, sizeof(caGroupname)-1, caGroupname,
																	 UT_TYPE_STRING, sizeof(caSsid)-1,      caSsid,
															     UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
															     UT_TYPE_ULONG,  sizeof(long),          &lModitime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags);		
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_websrvgroupssid_list.htm");
	  return 0;	
}

// ��ͼ��ǩ����
int ncm_webcommark_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char sid[20] = "";
	 char mark[16] = "";
	 char mapid[16] = "";
	 char sx[32] = "";
	 char sy[32] = "";
	 char ex[32] = "";
	 char ey[32] = "";
	 char sname[32] = "";
	 	
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lSid = 0;
	 char         	caMark[16] = "";
	 char 					caMapid[16] = "";
	 double	        dSx = 0;
	 double	        dSy = 0;
	 double	        dEx = 0;
	 double	        dEy = 0;
	 char           caSname[32] = "";
	 
	 char caUpdate[16] = "";
	 char caSesid[1024] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 unsigned long	lMaxid = 0;
	 // �ؼ���
	 char _keyword[33] = "";
	 
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
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 17,"keyword",   UT_TYPE_STRING, sizeof(_keyword)-1,  _keyword,
	 																"sid",       UT_TYPE_STRING, sizeof(sid)-1,       sid,
	                                "mark",      UT_TYPE_STRING, sizeof(mark)-1,      mark,
	                                "mapid",     UT_TYPE_STRING, sizeof(mapid)-1,     mapid,
	                                "sx",        UT_TYPE_STRING, sizeof(sx)-1,        sx,
	                                "sy",        UT_TYPE_STRING, sizeof(sy)-1,        sy,
	                                "ex",        UT_TYPE_STRING, sizeof(ex)-1,        ex,
	                                "ey",        UT_TYPE_STRING, sizeof(ey)-1,        ey,
	                                "sname",     UT_TYPE_STRING, sizeof(sname)-1,     sname,
	                                "start",     UT_TYPE_STRING, sizeof(start)-1,     start,
															   	"limit",     UT_TYPE_STRING, sizeof(limit)-1,     limit,
															   	"sort",      UT_TYPE_STRING, sizeof(sort)-1,      sort,
															   	"dir",       UT_TYPE_STRING, sizeof(dir)-1,       dir,
																	"selsid",    UT_TYPE_STRING, sizeof(caSesid)-1,   caSesid,
																	"del",			 UT_TYPE_STRING, sizeof(caDel)-1,     caDel,
																	"update",    UT_TYPE_STRING, sizeof(caUpdate)-1,  caUpdate,
																	"caExport",  UT_TYPE_STRING, sizeof(caExport)-1,  caExport);
	 
	 // ����
	 if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
	 // ȥ���ؼ���ǰ��Ŀո�
	 trim(_keyword);
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(mark)>0){
			pasCvtGBK(2,mark,caTemp,16);
			strcpy(mark,caTemp);
   }
   if(strlen(mapid)>0){
			pasCvtGBK(2,mapid,caTemp,16);
			strcpy(mapid,caTemp);
   }
   if(strlen(sname)>0){
			pasCvtGBK(2,sname,caTemp,32);
			strcpy(sname,caTemp);
   }
	 
   // ����/�޸�
	 if(!utStrIsSpaces(caUpdate)) {
	 		if (strcmp(caUpdate, "update")==0){
	 				sprintf(caTemp,"update portallocmark set mark='%s', mapid='%s', sx=%.2f, sy=%.2f, ex=%.2f, ey=%.2f, sname='%s' where sid=%lu", mark,mapid,atof(sx),atof(sy),atof(ex),atof(ey),sname,atol(sid));
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }else if(strcmp(caUpdate, "add")==0){
	 	  	  strcpy(mapid,(char *)utComGetVar_sd(psShmHead,"MapId","17_1_0"));
	 	  	  sprintf(caTemp,"insert into portallocmark (mark,mapid,sx,sy,ex,ey,sname) values ('%s','%s',%.2f,%.2f,%.2f,%.2f,'%s')",mark,mapid,atof(sx),atof(sy),atof(ex),atof(ey),sname);
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from portallocmark where sid in(%s)",caSesid);
    	}
	 }
	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from portallocmark where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (mark like '%%%s%%')", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select sid, mark, mapid, sx, sy, ex, ey, sname from portallocmark where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (mark like '%%%s%%')", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
	                             UT_TYPE_STRING, sizeof(caMark)-1,      caMark,
															 UT_TYPE_STRING, sizeof(caMapid)-1,     caMapid,
													     UT_TYPE_FLOAT,  sizeof(double),        &dSx,
													     UT_TYPE_FLOAT,  sizeof(double),        &dSy,
													     UT_TYPE_FLOAT,  sizeof(double),        &dEx,
													     UT_TYPE_FLOAT,  sizeof(double),        &dEy,
													     UT_TYPE_STRING, sizeof(caSname)-1,     caSname);
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVar(psDbHead,"mark",iNum,caMark);
			 utPltPutLoopVar(psDbHead,"mapid",iNum,caMapid);
			 utPltPutLoopVarF(psDbHead,"sx",iNum,"%.2f",dSx);
			 utPltPutLoopVarF(psDbHead,"sy",iNum,"%.2f",dSy);
			 utPltPutLoopVarF(psDbHead,"ex",iNum,"%.2f",dEx);
			 utPltPutLoopVarF(psDbHead,"ey",iNum,"%.2f",dEy);
			 utPltPutLoopVar(psDbHead,"sname",iNum,caSname);
			 
			 lSid = 0;
			 memset(caMark, 0, sizeof(caMark));
			 memset(caMapid, 0, sizeof(caMapid));
			 dSx = 0;
			 dSy = 0;
	     dEx = 0;
	     dEy = 0;
	     memset(caSname, 0, sizeof(caSname));
	     iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
			                             UT_TYPE_STRING, sizeof(caMark)-1,      caMark,
																	 UT_TYPE_STRING, sizeof(caMapid)-1,     caMapid,
															     UT_TYPE_FLOAT,  sizeof(double),        &dSx,
															     UT_TYPE_FLOAT,  sizeof(double),        &dSy,
															     UT_TYPE_FLOAT,  sizeof(double),        &dEx,
															     UT_TYPE_FLOAT,  sizeof(double),        &dEy,
															     UT_TYPE_STRING, sizeof(caSname)-1,     caSname);	
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcommark_list.htm");
	  return 0;	
}

int ncm_webshopgtype_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char id[20] = "";
	 char groupname[32] = "";
	 char groupcode[32] = "";
	 char type[20] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lId = 0;
	 char         	caGroupname[32] = "";
	 char 					caGroupcode[32] = "";
	 unsigned long  lSystime = 0;
	 unsigned long  lType = 0;
	 
	 char caUpdate[16] = "";
	 char caSesid[1024] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 unsigned long	lMaxid = 0;
	 // �ؼ���
	 char _keyword[33] = "";
	 
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
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 13,"keyword",   UT_TYPE_STRING, sizeof(_keyword)-1,  _keyword,
	 																"id",        UT_TYPE_STRING, sizeof(id)-1,        id,
	                                "groupname", UT_TYPE_STRING, sizeof(groupname)-1, groupname,
	                                "groupcode", UT_TYPE_STRING, sizeof(groupcode)-1, groupcode,
	                                "type",      UT_TYPE_STRING, sizeof(type)-1,      type,
	                                "start",     UT_TYPE_STRING, sizeof(start)-1,     start,
															   	"limit",     UT_TYPE_STRING, sizeof(limit)-1,     limit,
															   	"sort",      UT_TYPE_STRING, sizeof(sort)-1,      sort,
															   	"dir",       UT_TYPE_STRING, sizeof(dir)-1,       dir,
																	"selsid",    UT_TYPE_STRING, sizeof(caSesid)-1,   caSesid,
																	"del",			 UT_TYPE_STRING, sizeof(caDel)-1,     caDel,
																	"update",    UT_TYPE_STRING, sizeof(caUpdate)-1,  caUpdate,
																	"caExport",  UT_TYPE_STRING, sizeof(caExport)-1,  caExport);
	 
	 // ����
	 if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
	 // ȥ���ؼ���ǰ��Ŀո�
	 trim(_keyword);
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(groupname)>0){
			pasCvtGBK(2,groupname,caTemp,16);
			strcpy(groupname,caTemp);
   }
   if(strlen(groupcode)>0){
			pasCvtGBK(2,groupcode,caTemp,16);
			strcpy(groupcode,caTemp);
   }
	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from ncmshopgtype where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (groupname like '%%%s%%')", _keyword);
	 }
	 if(!utStrIsSpaces(type)) // ��������ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and type='%s'", type);
	 }
	 printf("sql = %s\n", sql);
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select id, groupname, groupcode, systime, type from ncmshopgtype where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (groupname like '%%%s%%')", _keyword);
	 }
	 if(!utStrIsSpaces(type)) // ��������ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and type='%s'", type);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lId,
	                             UT_TYPE_STRING, sizeof(caGroupname)-1, caGroupname,
															 UT_TYPE_STRING, sizeof(caGroupcode)-1, caGroupcode,
															 UT_TYPE_ULONG,  sizeof(long),          &lSystime,
													     UT_TYPE_ULONG,  sizeof(long),          &lType);
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
			 
			 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
			 utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
			 utPltPutLoopVar(psDbHead,"groupcode",iNum,caGroupcode);
			 utPltPutLoopVarF(psDbHead,"systime",iNum,"%lu",lSystime);
			 utPltPutLoopVarF(psDbHead,"type",iNum,"%lu",lType);
			 
			 lId = 0;
			 memset(caGroupname, 0, sizeof(caGroupname));
			 memset(caGroupcode, 0, sizeof(caGroupcode));
			 lSystime = 0;
			 lType = 0;
	     iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lId,
			                             UT_TYPE_STRING, sizeof(caGroupname)-1, caGroupname,
																	 UT_TYPE_STRING, sizeof(caGroupcode)-1, caGroupcode,
																	 UT_TYPE_ULONG,  sizeof(long),          &lSystime,
															     UT_TYPE_ULONG,  sizeof(long),          &lType);	
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webshopgtype_list.htm");
	  return 0;	
}

static int pmUtlGetCsvWord2(char *caBuf,int iNum,char *caName)
{
    char *p;
    int i;
    char caTemp[12000];
    p = caBuf;
    i=0;
    while(p && *p) {
        p = utStrGetWord(p,caTemp,12000,",");
        if(i==iNum) {
            strcpy(caName,caTemp);
            return 0;
        }
        if(*p != ',') {
            strcpy(caName,"\0");
            return 0;
        }
        p++;
        i++;
    }
}

int ncm_weboditydr_next(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
		pasDbCursor *psCur;
		char caTemp[16000];
		FILE *fp;
		int iNum=0;
		utPltDbHead *psDbHead;
		psDbHead = utPltInitDb();
		
		char plateid[32]="";
		char imp_file[129]="";
		char temp_file[129]="";
		char sqlbuf[1024]="";
		char caBuf[16002];
		unsigned long lcount = 0;
		unsigned long lcount1 = 0;
		unsigned long lcount2 = 0;
		unsigned long lShopid = 0;
		unsigned long lMaxid = 0;
		char *p, *pBuf;
		
		char caSid[32] = "";
		char caShopname[128] = "";
		char caName[128] = "";
		char caDispname[128] = "";
		char caDescr[255] = "";
		char caDetail[16002] = "";
		char caCover[128] = "";
		char caFloor[32] = "";
		char caUrl[255] = "";
		char caAddress[255] = "";
		char caImgpath[128] = "";
		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

		if(!utFileIsExist("/home/ncmysql/ncsrv/upload")) {
		    if(mkdir("/home/ncmysql/ncsrv/upload",777)!=0){
				    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","����","���ļ�����");
				    return 0;
		    }
		}
		system("chmod -Rf 777 /home/ncmysql/ncsrv/upload");

		utMsgGetSomeNVar(psMsgHead,2,"fname",  UT_TYPE_STRING, sizeof(imp_file)-1, imp_file,
		                             "plateid",UT_TYPE_STRING, sizeof(plateid)-1,  plateid);
		
		if(strlen(imp_file)!=0){
				p=utStrSkipSpaces(imp_file);
				p=utStrGetWord(p,temp_file,128,";\n");
				if((*p)==';')
				p=utStrGetWord(p+1,temp_file,128,";\n");
				fp=fopen(temp_file,"r");
				printf("temp_file=%s\n",temp_file);
				if(fp == NULL) {
						utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","����","���ļ�����");
						return 0;
				} 
		 	
				/*p = fgets(caBuf,16000,fp);  // ���Ա��� 
				p = fgets(caBuf,16000,fp);
				iNum = 0;
		    while(p && *p) {
		        p = utStrGetWord(p,caTemp,300,",\r\n");
		        if(!utStrIsSpaces(caTemp)) {
		            iNum++;
		            utPltPutLoopVar(psDbHead,"caTemp",iNum,caTemp);
		            utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum-1);
		            
		            if(iNum==1)
		            {
		            utPltPutVar(psDbHead,"sid",caTemp);
		            utPltPutVarF(psDbHead,"sid_num","%lu",iNum-1);
		            }
		            if(iNum==2)
		            {
		            utPltPutVar(psDbHead,"shopname",caTemp);
		            utPltPutVarF(psDbHead,"shopname_num","%lu",iNum-1);
		            }
		            if(iNum==3)
		            {
		            utPltPutVar(psDbHead,"name",caTemp);
		            utPltPutVarF(psDbHead,"name_num","%lu",iNum-1);
		            }
		            if(iNum==4)
		            {
		            utPltPutVar(psDbHead,"dispname",caTemp);
		            utPltPutVarF(psDbHead,"dispname_num","%lu",iNum-1);
		            }
		            if(iNum==5)
		            {
		            utPltPutVar(psDbHead,"descr",caTemp);
		            utPltPutVarF(psDbHead,"descr_num","%lu",iNum-1);
		            }
		            if(iNum==6)
		            {
		            utPltPutVar(psDbHead,"detail",caTemp);
		            utPltPutVarF(psDbHead,"detail_num","%lu",iNum-1);
		            }
		            if(iNum==7)
		            {
		            utPltPutVar(psDbHead,"cover",caTemp);
		            utPltPutVarF(psDbHead,"cover_num","%lu",iNum-1);
		            }
		            if(iNum==8)
		            {
		            utPltPutVar(psDbHead,"address",caTemp);
		            utPltPutVarF(psDbHead,"address_num","%lu",iNum-1);
		            }
		            if(iNum==9)
		            {
		            utPltPutVar(psDbHead,"url",caTemp);
		            utPltPutVarF(psDbHead,"url_num","%lu",iNum-1);
		            }
		            if(iNum==10)
		            {
		            utPltPutVar(psDbHead,"floor",caTemp);
		            utPltPutVarF(psDbHead,"floor_num","%lu",iNum-1);
		            }
		            if(iNum==11)
		            {
		            utPltPutVar(psDbHead,"imgpath",caTemp);
		            utPltPutVarF(psDbHead,"imgpath_num","%lu",iNum-1);
		            }
		        }
		        if(*p != ',') {
		            break;
		        }
		        p++;
		    }*/
				p = fgets(caBuf,16000,fp);  // ���Ե�һ��
				p = fgets(caBuf,16000,fp);  // ���Ե�һ��
				p = fgets(caBuf,16000,fp);
				while(p){
				    pBuf = utStrGetWord(p,caTemp,16000,"\r\n");
		        strcpy(pBuf,caTemp);
		        strcat(pBuf,",");
		        printf("pBuf = %s \n",pBuf);
		        memset(caSid,0,sizeof(caSid));
		        memset(caShopname,0,sizeof(caShopname));
		        memset(caName,0,sizeof(caName));
		        memset(caDispname,0,sizeof(caDispname));
		        memset(caDescr,0,sizeof(caDescr));
		        memset(caDetail,0,sizeof(caDetail));
		        memset(caCover,0,sizeof(caCover));
		        memset(caAddress,0,sizeof(caAddress));
		        memset(caUrl,0,sizeof(caUrl));
		        memset(caFloor,0,sizeof(caFloor));
		        memset(caImgpath,0,sizeof(caImgpath));
		        
		        while(pBuf){
		             iNum++;
		             pBuf = utStrGetWord(pBuf,caTemp,16000,",");
		             //printf("caTemp = %s \n",caTemp);
		             //printf("iNum = %d \n",iNum);
		             if(iNum==1){
		                strcpy(caSid, caTemp);
		                printf("caSid = %s\n", caSid);
		             }
		             else if(iNum==2){
		                strcpy(caShopname,caTemp);
		                printf("caShopname = %s\n", caShopname);
		             }
		             else if(iNum==3){
		                strcpy(caName,caTemp);
		                printf("caName = %s\n", caName);
		             }
		             else if(iNum==4){
		                strcpy(caDispname,caTemp);
		                printf("caDispname = %s\n", caDispname);
		             }
		             else if(iNum==5){
		                strcpy(caDescr,caTemp);
		                printf("caDescr = %s\n", caDescr);
		             }
		             else if(iNum==6){
		                strcpy(caDetail,caTemp);
		                printf("caDetail = %s\n", caDetail);
		             }
		             else if(iNum==7){
		                strcpy(caCover,caTemp);
		                printf("caCover = %s\n", caCover);
		             }
		             else if(iNum==8){
		                strcpy(caAddress,caTemp);
		                printf("caAddress = %s\n", caAddress);
		             }
		             else if(iNum==9){
		                strcpy(caUrl,caTemp);
		                printf("caUrl = %s\n", caUrl);
		             }
		             else if(iNum==10){
		                strcpy(caFloor,caTemp);
		                printf("caFloor = %s\n", caFloor);
		             }
		             else if(iNum==11){
		                strcpy(caImgpath,caTemp);
		                printf("caImgpath = %s\n", caImgpath);
		             }
		             
				         if(*pBuf != ',') {
				             break;
				         }
				         pBuf++;
		        }
		        
		        sprintf(sqlbuf, "select count(*) from ncmmall where name='%s'", caFloor);
		        //printf("sqlbuf = %s\n", sqlbuf);
						pasDbOneRecord(sqlbuf,0,UT_TYPE_ULONG,sizeof(long),&lcount2);
						if(lcount2==0){
								sprintf(sqlbuf, "insert into ncmmall (name, flags, plateid) values ('%s', %lu, %lu)", caFloor, 1, atol(plateid));	
								//printf("sqlbuf = %s\n", sqlbuf);
								pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
								pasDbCommit(NULL);
						}
						
						sprintf(sqlbuf, "select count(*) from ncmshop where name='%s'", caShopname);
						//printf("sqlbuf = %s\n", sqlbuf);
						pasDbOneRecord(sqlbuf,0,UT_TYPE_ULONG,sizeof(long),&lcount);
						if(lcount==0){
							  pasDbOneRecord("select max(id) from ncmshop", 0, UT_TYPE_ULONG, sizeof(long), &lShopid);
								sprintf(sqlbuf, "insert ncmshop (id, name, floor, url, address, plateid) values (%lu, '%s', '%s', '%s', '%s', %lu)", (lShopid+1), caShopname, caFloor, caUrl, caAddress, atol(plateid));			
								//printf("sqlbuf = %s\n", sqlbuf);
								pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
								pasDbCommit(NULL);
								
								sprintf(sqlbuf, "select count(*) from ncmcomodity where name = '%s'", caName);
								//printf("sqlbuf = %s\n", sqlbuf);
								pasDbOneRecord(sqlbuf,0,UT_TYPE_ULONG,sizeof(long),&lcount1);
								//printf("lcount = %d\n", lcount);
								if(lcount1==0){
										pasDbOneRecord("select max(sid) from ncmcomodity", 0, UT_TYPE_ULONG, sizeof(long), &lMaxid);
										sprintf(sqlbuf, "insert into ncmcomodity (sid, name, dispname, shgroupid, shopname, floor, url, address, descr, detail, imgpath, plateid) values (%lu, '%s', '%s', %lu, '%s','%s','%s','%s','%s','%s','%s',%lu)", (lMaxid+1), caName, caDispname, (lShopid+1), caShopname, caFloor, caUrl, caAddress, caDescr, caDetail, caImgpath, atol(plateid));
										//printf("sqlbuf = %s\n", sqlbuf);
										pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
										pasDbCommit(NULL);
								}
						}else{
							  sprintf(sqlbuf, "select id from ncmshop where name='%s'", caShopname);
							  //printf("sqlbuf = %s\n", sqlbuf);
								pasDbOneRecord(sqlbuf,0,UT_TYPE_ULONG,sizeof(long),&lShopid);
								
								sprintf(sqlbuf, "select count(*) from ncmcomodity where name = '%s'", caName);
								//printf("sqlbuf = %s\n", sqlbuf);
								pasDbOneRecord(sqlbuf,0,UT_TYPE_ULONG,sizeof(long),&lcount1);
								//printf("lcount = %d\n", lcount);
								if(lcount1==0){
										pasDbOneRecord("select max(sid) from ncmcomodity", 0, UT_TYPE_ULONG, sizeof(long), &lMaxid);
										sprintf(sqlbuf, "insert into ncmcomodity (sid, name, dispname, shgroupid, shopname, floor, url, address, descr, detail, imgpath, plateid) values (%lu, '%s', '%s', %lu, '%s','%s','%s','%s','%s','%s','%s',%lu)", (lMaxid+1), caName, caDispname, lShopid, caShopname, caFloor, caUrl, caAddress, caDescr, caDetail, caImgpath, atol(plateid));
										//printf("sqlbuf = %s\n", sqlbuf);
										pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
										pasDbCommit(NULL);
								}
						}
						iNum=0;
						p = fgets(caBuf,16000,fp);
				}
				fclose(fp);
		}
		//utPltPutVar(psDbHead,"fname",temp_file);
		//utPltPutVar(psDbHead,"plateid",plateid);
    //utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/odity_import_next.htm");
		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","�������!","�������!");  
		return 0;
}

int ncm_weboditydr_update(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
		pasDbCursor *psCur;
		char caTemp[16000];
		FILE *fp;
		int iNum=0;
		utPltDbHead *psDbHead;
		psDbHead = utPltInitDb();
		
		char plateid[32]="";
		char imp_file[129]="";
		char temp_file[129]="";
		char sqlbuf[1024]="";
		char caBuf[16002];
		unsigned long lcount = 0;
		unsigned long lcount1 = 0;
		unsigned long lcount2 = 0;
		unsigned long lShopid = 0;
		unsigned long lMaxid = 0;
		char *p, *pBuf;
		
		char caSid[32] = "";
		char caShopname[128] = "";
		char caName[128] = "";
		char caDispname[128] = "";
		char caDescr[255] = "";
		char caDetail[16002] = "";
		char caCover[128] = "";
		char caFloor[32] = "";
		char caUrl[255] = "";
		char caAddress[255] = "";
		char caImgpath[128] = "";
		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

		if(!utFileIsExist("/home/ncmysql/ncsrv/upload")) {
		    if(mkdir("/home/ncmysql/ncsrv/upload",777)!=0){
				    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","����","���ļ�����");
				    return 0;
		    }
		}
		system("chmod -Rf 777 /home/ncmysql/ncsrv/upload");

		utMsgGetSomeNVar(psMsgHead,2,"fname",  UT_TYPE_STRING, sizeof(imp_file)-1, imp_file,
		                             "plateid",UT_TYPE_STRING, sizeof(plateid)-1,  plateid);
		
		if(strlen(imp_file)!=0){
				p=utStrSkipSpaces(imp_file);
				p=utStrGetWord(p,temp_file,128,";\n");
				if((*p)==';')
				p=utStrGetWord(p+1,temp_file,128,";\n");
				fp=fopen(temp_file,"r");
				printf("temp_file=%s\n",temp_file);
				if(fp == NULL) {
						utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","����","���ļ�����");
						return 0;
				} 
		 	 
				p = fgets(caBuf,16000,fp);  // ���Ե�һ��
				p = fgets(caBuf,16000,fp);  // ���Ե�һ��
				p = fgets(caBuf,16000,fp);
				while(p){
				    pBuf = utStrGetWord(p,caTemp,16000,"\r\n");
		        strcpy(pBuf,caTemp);
		        strcat(pBuf,",");
		        printf("pBuf = %s \n",pBuf);
		        memset(caSid,0,sizeof(caSid));
		        memset(caShopname,0,sizeof(caShopname));
		        memset(caName,0,sizeof(caName));
		        memset(caDispname,0,sizeof(caDispname));
		        memset(caDescr,0,sizeof(caDescr));
		        memset(caDetail,0,sizeof(caDetail));
		        memset(caCover,0,sizeof(caCover));
		        memset(caAddress,0,sizeof(caAddress));
		        memset(caUrl,0,sizeof(caUrl));
		        memset(caFloor,0,sizeof(caFloor));
		        memset(caImgpath,0,sizeof(caImgpath));
		        
		        while(pBuf){
		             iNum++;
		             pBuf = utStrGetWord(pBuf,caTemp,16000,",");
		             //printf("caTemp = %s \n",caTemp);
		             //printf("iNum = %d \n",iNum);
		             if(iNum==1){
		                strcpy(caSid, caTemp);
		                printf("caSid = %s\n", caSid);
		             }
		             else if(iNum==2){
		                strcpy(caShopname,caTemp);
		                printf("caShopname = %s\n", caShopname);
		             }
		             else if(iNum==3){
		                strcpy(caName,caTemp);
		                printf("caName = %s\n", caName);
		             }
		             else if(iNum==4){
		                strcpy(caDispname,caTemp);
		                printf("caDispname = %s\n", caDispname);
		             }
		             else if(iNum==5){
		                strcpy(caDescr,caTemp);
		                printf("caDescr = %s\n", caDescr);
		             }
		             else if(iNum==6){
		                strcpy(caDetail,caTemp);
		                printf("caDetail = %s\n", caDetail);
		             }
		             else if(iNum==7){
		                strcpy(caCover,caTemp);
		                printf("caCover = %s\n", caCover);
		             }
		             else if(iNum==8){
		                strcpy(caAddress,caTemp);
		                printf("caAddress = %s\n", caAddress);
		             }
		             else if(iNum==9){
		                strcpy(caUrl,caTemp);
		                printf("caUrl = %s\n", caUrl);
		             }
		             else if(iNum==10){
		                strcpy(caFloor,caTemp);
		                printf("caFloor = %s\n", caFloor);
		             }
		             else if(iNum==11){
		                strcpy(caImgpath,caTemp);
		                printf("caImgpath = %s\n", caImgpath);
		             }
		             
				         if(*pBuf != ',') {
				             break;
				         }
				         pBuf++;
		        }
		        
		        sprintf(sqlbuf, "select count(*) from ncmmall where name='%s'", caFloor);
		        //printf("sqlbuf = %s\n", sqlbuf);
						pasDbOneRecord(sqlbuf,0,UT_TYPE_ULONG,sizeof(long),&lcount2);
						if(lcount2==0){
								sprintf(sqlbuf, "insert into ncmmall (name, flags, plateid) values ('%s', %lu, %lu)", caFloor, 1, atol(plateid));	
								//printf("sqlbuf = %s\n", sqlbuf);
								pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
								pasDbCommit(NULL);
						}
						
						sprintf(sqlbuf, "update ncmshop set floor='%s', url='%s', address='%s' where name='%s'", caFloor, caUrl, caAddress, caShopname);
						//printf("sqlbuf = %s\n", sqlbuf);
						pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
						pasDbCommit(NULL);
						
						sprintf(sqlbuf, "update ncmcomodity set dispname='%s', floor='%s', url='%s', address='%s', descr='%s', detail='%s', imgpath='%s' where name='%s'", caDispname, caFloor, caUrl, caAddress, caDescr, caDetail, caImgpath, caName);
						//printf("sqlbuf = %s\n", sqlbuf);
						pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
						pasDbCommit(NULL);
						
						iNum=0;
						p = fgets(caBuf,16000,fp);
				}
				fclose(fp);
		}
		//utPltPutVar(psDbHead,"fname",temp_file);
		//utPltPutVar(psDbHead,"plateid",plateid);
    //utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/odity_import_next.htm");
		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","�������!","�������!");  
		return 0;
}



// �̳��û���Ϣ������˹)
int ncm_webcomshopuser_dns(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 // ������Ϣ����
	 char uid[20] = "";
	 char username[32] = "";
	 char dispname[32] = "";
	 char groupname[16] = "";
	 char proname[16] = "";
	 char usermac[18] = "";
	 char password[32] = "";
	 char nickname[32] = "";
	 char membernum[32] = "";
	 char integral[20] = "";
	 char coupon[20] = "";
	 char address[128] = "";
	 char decript[4096] = "";
	 char lcount[20] = "";
	 char imgpath[128] = "";
	 char addtime[32] = "";
	 char lasttime[32] = "";
	 char flags[32] = "";
	 char plateid[20] = "";
	 char temp_file[128] = "";
	 
	 // ������ݿ�ȡ�������ݵı���
	 unsigned long	lUid = 0;
	 char 					caUsername[32] = "";
	 char           caDispname[32] = "";
	 char           caGroupname[16] = "";
	 char 					caProname[16] = "";
	 char 					caUsermac[18] = "";
	 char 					caPassword[32] = "";
	 char 					caNickname[32] = "";
	 char 					caMembernum[32] = "";
	 unsigned long	lIntegral = 0;
	 unsigned long	lCoupon = 0;
	 char 					caAddress[128] = "";
	 char 					caDecript[4096] = "";
	 unsigned long	lLcount = 0;
	 char 					caImgpath[128] = "";
	 unsigned long	lAddtime = 0;
	 unsigned long	lLasttime = 0;
	 unsigned long	lFlags = 0;
	 char           s_addtime[32] = "";
	 char           s_lasttime[32] = "";
	 char           s_flags[32] = "";
	 
	 char caUpdate[16] = "";
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
	 unsigned long lAddTime = 0;
	 unsigned long lLastTime = 0;
	 
	 // ��ҳ
	 unsigned long lTotRec = 0; // ��¼���е����ݵĸ���
	 
	 // ����
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 char caShopids[1024];
	 unsigned long lDid;
	 char caSex[12],caMobile[20],caIdno[20],caEmail[64],caCname[128];
	 long lStime;
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
	 strcpy(caShopids,getDsShopids());
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
	 utMsgGetSomeNVar(psMsgHead, 9,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
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
  
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // ����
   if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
   
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char 	imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			printf("delete from ncmshopuserlog where sid in(%s)\n",caSesid);
					pasDbExecSqlF("delete from ncmshopuserlog where sid in(%s)",caSesid);
    	}
	 }
	 // ��ѯ
	 strcpy(sql, "select count(distinct username) from ncmshopuserlog where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%' or dispname like '%%%s%%' or mobile like '%%%s%%') ", _keyword, _keyword, _keyword);
	 }
	 if(strlen(caShopids)>0){
	 	  sprintf(sql+strlen(sql)," and did in (%s) ",caShopids);
	 }
	 if(!utStrIsSpaces(plateid)) // ����ģ���ѯ
	 {
	//		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu ", atol(plateid));
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select  distinct username, a.dispname, did, sex, mobile, idno, email, stime, b.dispname from ncmshopuserlog a left join ncsrvgroup b on did=b.groupid where 1=1 ");
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (a.username like '%%%s%%' or a.dispname like '%%%s%%' or mobile like '%%%s%%') ", _keyword, _keyword, _keyword);
	 }
	 if(strlen(caShopids)>0){
	 	  sprintf(sql+strlen(sql)," and did in (%s) ",caShopids);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by stime desc limit %lu, %lu", iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746�û���ѯ"),(char *)ncLang("0171��ѯ���ݳ���"));
       return 0;	
	 }
	 
	 // pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
	 lUid=0;
	 memset(caUsername,0,sizeof(caUsername));
	 memset(caDispname,0,sizeof(caDispname));
	 lDid=0;
	 memset(caSex,0,sizeof(caSex));
	 memset(caMobile,0,sizeof(caMobile));
	 memset(caIdno,0,sizeof(caIdno));
	 memset(caEmail,0,sizeof(caEmail));
	 lStime=0;
	 memset(caCname,0,sizeof(caCname));
	 iret = pasDbFetchInto(psCur,
													     UT_TYPE_STRING, sizeof(caUsername)-1, caUsername,
													     UT_TYPE_STRING, sizeof(caDispname)-1, caDispname,
													     UT_TYPE_ULONG,4,&lDid,
													     UT_TYPE_STRING,8,caSex,
													     UT_TYPE_STRING,sizeof(caMobile)-1,caMobile,
													     UT_TYPE_STRING,sizeof(caIdno)-1,caIdno,
													     UT_TYPE_STRING,sizeof(caEmail)-1,caEmail,
													     UT_TYPE_LONG,4,&lStime,
													     UT_TYPE_STRING,sizeof(caCname)-1,caCname);
													   
	
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
			 }
		   

		   if(lStime){
		   	  strcpy(s_lasttime, utTimFormat("%Y/%m/%d %H:%M:%S", lStime));
		   }else{
		   		strcpy(s_lasttime, "");	
		   }
		 
		   utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
			 utPltPutLoopVar(psDbHead,"groupname",iNum,caCname);
			 utPltPutLoopVar(psDbHead,"mobile",iNum,caMobile);
			 utPltPutLoopVar(psDbHead,"email",iNum,caEmail);
			 utPltPutLoopVar(psDbHead,"cname",iNum,caCname);
			 utPltPutLoopVar(psDbHead,"sex",iNum,caSex);
			 utPltPutLoopVar(psDbHead,"idno",iNum,caIdno);
			 utPltPutLoopVar(psDbHead,"stime",iNum,s_lasttime);
			 utPltSetCvtHtml(0);
		 lUid=0;
		 memset(caUsername,0,sizeof(caUsername));
		 memset(caDispname,0,sizeof(caDispname));
		 lDid=0;
		 memset(caSex,0,sizeof(caSex));
		 memset(caMobile,0,sizeof(caMobile));
		 memset(caIdno,0,sizeof(caIdno));
		 memset(caEmail,0,sizeof(caEmail));
		 lStime=0;
		 memset(caCname,0,sizeof(caCname));
	 iret = pasDbFetchInto(psCur,
													     UT_TYPE_STRING, sizeof(caUsername)-1, caUsername,
													     UT_TYPE_STRING, sizeof(caDispname)-1, caDispname,
													     UT_TYPE_ULONG,4,&lDid,
													     UT_TYPE_STRING,8,caSex,
													     UT_TYPE_STRING,sizeof(caMobile)-1,caMobile,
													     UT_TYPE_STRING,sizeof(caIdno)-1,caIdno,
													     UT_TYPE_STRING,sizeof(caEmail)-1,caEmail,
													     UT_TYPE_LONG,4,&lStime,
													     UT_TYPE_STRING,sizeof(caCname)-1,caCname);
	  }
	utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
 
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_dnsshopuser_list.htm");
	  return 0;	
}




int Ncm_WebShopSWW_SetFunName(utShmHead *psShmHead)
{
		pasSetTcpFunName("ncm_webcomseries_list", ncm_webcomseries_list, 0); // �̳Ƕ������Ϣ-0303
		pasSetTcpFunName("ncm_webcomseries_info", ncm_webcomseries_info, 0);
		pasSetTcpFunName("ncm_webComSeriesImg_dr", ncm_webComSeriesImg_dr, 0);
		
		pasSetTcpFunName("ncm_webcomcomp_list", ncm_webcomcomp_list, 0); // ��Ʒ�̵���ձ�-0421
		
		pasSetTcpFunName("ncm_websrvgroup_list", ncm_websrvgroup_list, 0); // ��λ����-0423
		pasSetTcpFunName("ncm_websrvgroupap_list", ncm_websrvgroupap_list, 0); // AP����-0423
		pasSetTcpFunName("ncm_websrvgroupssid_list", ncm_websrvgroupssid_list, 0); // SSID����-0423
		
		pasSetTcpFunName("ncm_webcomindex2_list", ncm_webcomindex2_list, 0); // ������-0508
		pasSetTcpFunName("ncm_webarea_list", ncm_webarea_list, 0);  // ������Ϣ-0509
		
		pasSetTcpFunName("ncm_webcomshop_dr_images", ncm_webcomshop_dr_images, 0);  // �̵����ͼƬ�ϴ�-0521
		pasSetTcpFunName("ncm_webcomindex2_dr_images", ncm_webcomindex2_dr_images, 0);  // ������ͼƬ�ϴ�-0521
		pasSetTcpFunName("ncm_webcomodity_dr_images", ncm_webcomodity_dr_images, 0);  // ��Ʒ����ͼƬ�ϴ�-0521
		
		pasSetTcpFunName("ncm_webcomindex3_list", ncm_webcomindex3_list, 0); // ���ڶ���-0609
		pasSetTcpFunName("ncm_webcomindex3_dr_images", ncm_webcomindex3_dr_images, 0);
		
		pasSetTcpFunName("ncm_websrvgroup_info", ncm_websrvgroup_info, 0); //��λ������Ϣ-0613
		
		pasSetTcpFunName("ncm_webcomindex4_list", ncm_webcomindex4_list, 0); // Ԥ԰��������-0617
		pasSetTcpFunName("ncm_webcomindex4_dr_images", ncm_webcomindex4_dr_images, 0);
		
		pasSetTcpFunName("ncm_webcommark_list", ncm_webcommark_list, 0); // ��ͼ��ǩ����-0618
		
		pasSetTcpFunName("ncm_webcomindex5_list", ncm_webcomindex5_list, 0); // ̫��������-0620
		pasSetTcpFunName("ncm_webcomindex5_dr_images", ncm_webcomindex5_dr_images, 0);
		
		pasSetTcpFunName("ncm_webshopgtype_list", ncm_webshopgtype_list, 0); // ����̳������-0623
		
		pasSetTcpFunName("ncm_weboditydr_next", ncm_weboditydr_next, 0); // ��Ʒ����-0808
		pasSetTcpFunName("ncm_weboditydr_update", ncm_weboditydr_update, 0);
		pasSetTcpFunName("ncm_webcomshopuser_dns", ncm_webcomshopuser_dns, 0);
		
		return 0;
}
