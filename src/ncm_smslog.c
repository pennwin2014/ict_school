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
extern char *pHashNettype;
extern char *pHashWebclass;
extern char *pHashService;
#include <iconv.h>

int ncTimeToLong(char *stime, unsigned long *lTime);
/*{
	 char ctime[20] = "";
	 
	 snprintf(ctime, sizeof(ctime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", stime[0], stime[1], stime[2], stime[3], stime[5], stime[6], stime[8], stime[9], stime[11], stime[12], stime[14], stime[15], stime[17], stime[18]);	
	 *lTime = utTimStrToLong("%Y%m%d%H%M%S", ctime);
	 
	 return 0;
}*/

int postion(char *username){
		int i = 0;
		for(i=0; i<strlen(username); i++){
				if(username[i]=='-'){
						return i;	
				}	
		}
		return -1;
}

int ncUsernameToNumber(char *username, char *code, char *phone){
	 int i = 0;
	 int len = strlen(username);
	 int post = postion(username);
	 printf("post = %d\n", post);
	 for(i=0; i<post; i++){
	 		*(code+i) = *(username+i);
	 }
	 for(i=post+1; i<len; i++){
	 		*(phone+i-post-1) = *(username+i);	
	 }
	 return 0;
}

int ncm_webshowSMSlogList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char sid[20] = "";
	 char userid[16] = "";
	 char username[32] = "";
	 char usermac[18] = "";
	 char ip[16] = "";
	 char logtime[32] = "";
	 char sptype[16] = "";
	 char areacode[16] = "";
	 char areaname[32] = "";
	 char status[16] = "";
	 char insert[16] = "";
	 char caInsert[16] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 unsigned long llogTime = 0;
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 20,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
			"userid",      UT_TYPE_STRING,  sizeof(userid)-1,         userid,
			"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
			"usermac",     UT_TYPE_STRING,  sizeof(usermac)-1,        usermac,
			"ip",          UT_TYPE_STRING,  sizeof(ip)-1,             ip,
			"logtime",     UT_TYPE_STRING,  sizeof(logtime)-1,        logtime,
			"sptype",      UT_TYPE_STRING,  sizeof(sptype)-1,         sptype,
			"areacode",    UT_TYPE_STRING,  sizeof(areacode)-1,       areacode,
			"areaname",    UT_TYPE_STRING,  sizeof(areaname)-1,       areaname,
			"status",      UT_TYPE_STRING,  sizeof(status)-1,         status,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"insert",      UT_TYPE_STRING,  10,                       caInsert,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 
	 trim(_keyword);
	 printf("sid = %s\n", sid);
	 printf("userid = %s\n", userid);
	 printf("username = %s\n", username);
	 printf("usermac = %s\n", usermac);
	 printf("ip = %s\n", ip);
	 printf("logtime = %s\n", logtime);
	 printf("sptype = %s\n", sptype);
	 printf("areacode = %s\n", areacode);
	 printf("areaname = %s\n", areaname);
	 printf("status = %s\n", status);
	 printf("insert = %s\n", caInsert);
	 printf("selsid = %s\n", caSesid);
	 printf("del = %s\n", caDel);
	 printf("update = %s\n", caUpdate);
	 printf("_keyword = %s\n", _keyword);
	 printf("start = %s\n", start);
	 printf("limit = %s\n", limit);
	 printf("sort1 = %s\n", sort);
	 printf("dir = %s\n", dir);
	 printf("caExport = %s\n", caExport);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
	 if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,30);
			strcpy(username,caTemp);
   }
   if(strlen(usermac)>0){
			pasCvtGBK(2,usermac,caTemp,30);
			strcpy(usermac,caTemp);
   }
   if(strlen(areaname)>0){
			pasCvtGBK(2,areaname,caTemp,30);
			strcpy(areaname,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
   
   // 时间转换
   ncTimeToLong(logtime, &llogTime);
   
   // 类别转换
   char caUtype[32] = ""; 
	 
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
			if (strlen(sid)>0){
	 				sprintf(caTemp,"update ncsrvsmslog set userid=%lu,username='%s',usermac='%s',ip='%s',logtime=%lu,sptype=%lu,areacode=%lu,areaname='%s',status=%lu where sid=%lu ",atol(userid),username,usermac,ip,llogTime,atol(sptype),atol(areacode),areaname,atol(status),atol(sid));
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  } else {
		    	sprintf(caTemp,"insert into ncsrvsmslog (userid,username,usermac,ip,logtime,sptype,areacode,areaname,status) values (%lu,'%s','%s','%s',%lu,%lu,%lu,'%s',%lu)",atol(userid),username,usermac,ip,llogTime,atol(sptype),atol(areacode),areaname,atol(status));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 	  memset(sid, 0, sizeof(sid));
	 	  memset(userid, 0, sizeof(userid));
	 	  memset(username, 0, sizeof(username));
	 	  memset(usermac, 0, sizeof(usermac));
	 	  memset(ip, 0, sizeof(ip));
	 	  memset(logtime, 0, sizeof(logtime));
	 	  memset(sptype, 0, sizeof(sptype));
	 	  memset(areacode, 0, sizeof(areacode));
	 	  memset(areaname, 0, sizeof(areaname));
	 	  memset(status, 0, sizeof(status));
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    			printf("caSesid = %s\n", caSesid);
    		  pasDbExecSqlF("delete from ncsrvsmslog where sid in(%s)",caSesid);
    	}
	 }	 
	 // 查询
	 strcpy(sql, "select count(*) from ncsrvsmslog where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%' or usermac like '%%%s%%') ",  _keyword, _keyword);
	 }
	 if(!utStrIsSpaces(username))  // 用户名
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(logtime))  // 日志时间
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and logtime = %lu", llogTime);
	 }
	 if(!utStrIsSpaces(sptype))  // 运营商类别
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sptype like '%%%s%%'", sptype);
	 }
	 if(!utStrIsSpaces(areacode))  // 区号
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and areacode like '%%%s%%'", areacode);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 //导出
	 unsigned long lCount2 = 0; //internation sms
	 unsigned long lCount3 = 0; //local sms
	 int count1[24] = {0}; //internation sms
	 char time1[24][32] = {""};
	 int num = 0; 
	 int count2[24] = {0}; //local sms
	 char time2[24][32] = {""};
	
	 if(strlen(caExport)>0){
	 		 strcpy(caFilename, "smslog.csv");
	 		 sprintf(caPath, "%s/upload", "/home/ncmysql/ncsrv");
	 		 sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
	 		 		 printf("导出出错\n");
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"短信统计报告\n");
			 ncUtlPrintCsv(fp,3,UT_TYPE_STRING,"时间",
			                    UT_TYPE_STRING,"本地短信总数",
			                    UT_TYPE_STRING,"国际短信总数");
			 
			 sprintf(caTemp1, "select count(*) as lCount2, date_format(from_unixtime(floor(logtime/3600)*3600), '%H') as sms from ncsrvsmslog where username like '%%-%%' group by sms order by sms");
			 psCur = pasDbOpenSql(caTemp1, 0);
			 if(psCur != NULL){
			 		 unsigned long lCount2 = 0;
			 		 char sms[32] = "";
			 		 while(0 == (iret = pasDbFetchInto(psCur,
						 UT_TYPE_ULONG,  4,                   &lCount2,
						 UT_TYPE_STRING, sizeof(sms)-1,  			sms)) || 1405 == iret)
				  {
				  	 count1[atoi(sms)] = lCount2;
				  	 sprintf(time1[atoi(sms)], "%s:00-%s:59", sms, sms);
				  	 //printf("count1[%d] = %d\n", atoi(sms), count1[atoi(sms)]);
				  	 //printf("time1[%d] = %s\n", atoi(sms), time1[atoi(sms)]);
				  }
			 }
			 pasDbCloseCursor(psCur);
			 
			 memset(caTemp1, 0, sizeof(caTemp1));
			 sprintf(caTemp1, "select count(*) as lCount3, date_format(from_unixtime(floor(logtime/3600)*3600), '%H') as smst from ncsrvsmslog where username not like '%%-%%' group by smst order by smst");
			 psCur = pasDbOpenSql(caTemp1, 0);
			 if(psCur != NULL){
			 		 unsigned long lCount3 = 0;
			 		 char smst[32] = "";
			 		 while(0 == (iret = pasDbFetchInto(psCur,
						 UT_TYPE_ULONG,  4,                   &lCount3,
						 UT_TYPE_STRING, sizeof(smst)-1,  			smst)) || 1405 == iret)
				  {
				  	 count2[atoi(smst)] = lCount3;
				  	 sprintf(time2[atoi(smst)], "%s:00-%s:59", smst, smst);
				  	 //printf("count2[%d] = %d\n", atoi(smst), count2[atoi(smst)]);
				  	 //printf("time2[%d] = %s\n", atoi(smst), time2[atoi(smst)]);
				  }
			 }
			 pasDbCloseCursor(psCur);
			 
			 for(num=0; num<10; num++){
			 		if(strlen(time1[num])==0){
			 				sprintf(time1[num], "0%d:00-0%d:59", num, num);
			 		}	
			 }
			 for(num=10; num<24; num++){
			 		if(strlen(time1[num])==0){
			 				sprintf(time1[num], "%d:00-%d:59", num, num);
			 		}	
			 }
			 for(num=0; num<24; num++){
			 		printf("time1[%d] = %s\n", num, time1[num]);
			 		ncUtlPrintCsv(fp,3,UT_TYPE_STRING,time1[num],
			                    UT_TYPE_LONG,count2[num],
			                    UT_TYPE_LONG,count1[num]);	
			 }
			 
			 fprintf(fp,"短信详细报告\n");
			 ncUtlPrintCsv(fp,3,UT_TYPE_STRING,"国家代号",
			                    UT_TYPE_STRING,"电话号码",
			                    UT_TYPE_STRING,"短信发送时间"); 
			 
			 memset(caTemp1, 0, sizeof(caTemp1));                  
			 sprintf(caTemp1, "select username, date_format(from_unixtime(logtime), '%T') as time from ncsrvsmslog");
			 printf("caTemp1 = %s\n", caTemp1);
			 psCur = pasDbOpenSql(caTemp1, 0);
			 if(psCur != NULL){
			 		 char username[32] = "";
			 		 char time[32] = "";
			 		 char code[32] = "";
			 		 char phone[32] = "";
			 		 while(0 == (iret = pasDbFetchInto(psCur,
						 UT_TYPE_STRING, sizeof(username)-1,  	username,
						 UT_TYPE_STRING, sizeof(time)-1,  			time)) || 1405 == iret)
				  {
				  	 ncUsernameToNumber(username, code, phone);
				  	 ncUtlPrintCsv(fp,3,UT_TYPE_STRING,code,
			                    			UT_TYPE_STRING,phone,
			                    			UT_TYPE_STRING,time);
			       memset(code, 0, sizeof(code));
			       memset(phone, 0, sizeof(phone));
			       memset(time, 0, sizeof(time));
				  }
			 }
	     pasDbCloseCursor(psCur);  
	 }
	 
	 strcpy(sql, "select sid, userid, username, usermac, ip, logtime, sptype, areacode, areaname, status from ncsrvsmslog where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%' or usermac like '%%%s%%') ", _keyword, _keyword);
	 }
	 if(!utStrIsSpaces(username))  // 用户名
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(logtime))  // 日志时间
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and logtime = %lu", llogTime);
	 }
	 if(!utStrIsSpaces(sptype))  // 运营商类别
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sptype like '%%%s%%'", sptype);
	 }
	 if(!utStrIsSpaces(areacode))  // 区号
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and areacode like '%%%s%%'", areacode);
	 }
	 
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL) {
				unsigned long	sid = 0;
				unsigned long	userid = 0;
				char 					username[32] = "";
				char 					usermac[18] = "";
				char 					ip[16] = "";
				unsigned long logtime = 0;
				unsigned long sptype = 0;
				unsigned long areacode = 0;
				char 					areaname[32] = "";
				unsigned long status = 0;
				char          s_logtime[32] = "";
				
				// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
			  while(0 == (iret = pasDbFetchInto(psCur,
			     UT_TYPE_ULONG,  4,               		&sid,
			     UT_TYPE_ULONG,  4,               		&userid,
					 UT_TYPE_STRING, sizeof(username)-1,  username,
					 UT_TYPE_STRING, sizeof(usermac)-1,   usermac,
					 UT_TYPE_STRING, sizeof(ip)-1,        ip,
					 UT_TYPE_ULONG,  4,               		&logtime,
					 UT_TYPE_ULONG,  4,               		&sptype,
					 UT_TYPE_ULONG,  4,               		&areacode,
					 UT_TYPE_STRING, sizeof(areaname)-1,  areaname,
					 UT_TYPE_ULONG,  4,               		&status)) || 1405 == iret)
			  {
					 iNum++;
					 if(iNum > 1)
					 {
						 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
					 }
				  
				   strcpy(s_logtime, utTimFormat("%Y/%m/%d %H:%M:%S", logtime));
				   if(strcmp(s_logtime, "1970/01/01 00:00:00") == 0){
				     strcpy(s_logtime, "");	
				   }
				  
					 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
					 utPltPutLoopVarF(psDbHead,"userid",iNum,"%lu",userid);
					 utPltPutLoopVar(psDbHead,"username",iNum,username);
					 utPltPutLoopVar(psDbHead,"usermac",iNum,usermac);
					 utPltPutLoopVar(psDbHead,"ip",iNum,ip);
					 utPltPutLoopVarF(psDbHead,"logtime",iNum,s_logtime);
					 utPltPutLoopVarF(psDbHead,"sptype",iNum,"%lu",sptype);
					 utPltPutLoopVarF(psDbHead,"areacode",iNum,"%lu",areacode);
					 utPltPutLoopVar(psDbHead,"areaname",iNum,areaname);
					 utPltPutLoopVarF(psDbHead,"status",iNum,"%lu",status);
			  }
		}
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  if(strlen(caExport)>0){
	 	    fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        //remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	  }
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmgroup_s/ncm_smslog_list.htm");
	  return 0;	
}

