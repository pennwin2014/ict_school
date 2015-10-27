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

static int ncTimeToLong(char *stime, unsigned long *lTime)
{
	 char ctime[20] = "";
	 
	 snprintf(ctime, sizeof(ctime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", stime[0], stime[1], stime[2], stime[3], stime[5], stime[6], stime[8], stime[9], stime[11], stime[12], stime[14], stime[15], stime[17], stime[18]);	
	 *lTime = utTimStrToLong("%Y%m%d%H%M%S", ctime);
	 
	 return 0;
}

int ncTimeToStr(char *cTime, char *sTime){
	 int i = 0;
	 char *p = NULL;
	 int len = strlen(cTime);
	 int flag = 0;
	 for(i=0; i<len; i++){
	 		if(*(cTime+i)==','){
	 				flag = 1;
		  }
	 }
	 if(flag){
	 		p = strtok(cTime, ",");
			while(p){
			 	 strcat(sTime, p);
			 	 p = strtok(NULL, ",");	
			}
	 }else{
	 		strcpy(sTime, cTime);
	 }
	 return 0;
}

int ncmWeb_groupList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char groupname[32] = "";
	 char dispname[32] = "";
	 char sessiontime[64] = "";
	 char idletime[64] = "";
	 char macbindtime[64] = "";
	 char moditime[64] = "";
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
	 unsigned long lCount2 = 0; // 表中记录的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 unsigned long lSessionTime = 0;
	 char sIdleTime[32] = "";
	 char sMacbindTime[32] = "";
	 unsigned long lModiTime = 0;
	 
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
	 utMsgGetSomeNVar(psMsgHead, 16,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"groupname",   UT_TYPE_STRING,  sizeof(groupname)-1,      groupname,
			"dispname",    UT_TYPE_STRING,  sizeof(dispname)-1,       dispname,
			"sessiontime", UT_TYPE_STRING,  sizeof(sessiontime)-1,    sessiontime,
			"idletime",    UT_TYPE_STRING,  sizeof(idletime)-1,       idletime,
			"macbindtime", UT_TYPE_STRING,  sizeof(macbindtime)-1,    macbindtime,
			"moditime",    UT_TYPE_STRING,  sizeof(moditime)-1,       moditime,
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
	 printf("groupname = %s\n", groupname);
	 printf("dispname = %s\n", dispname);
	 printf("sessiontime = %s\n", sessiontime);
	 printf("idletime = %s\n", idletime);
	 printf("macbindtime = %s\n", macbindtime);
	 printf("moditime = %s\n", moditime);
	 printf("insert = %s\n", caInsert);
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
	 if(strlen(groupname)>0){
			pasCvtGBK(2,groupname,caTemp,30);
			strcpy(groupname,caTemp);
   }
   if(strlen(dispname)>0){
			pasCvtGBK(2,dispname,caTemp,30);
			strcpy(dispname,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   if(strlen(sort)>0){
			pasCvtGBK(2,sort,caTemp,30);
			strcpy(sort,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"groupname");
	 }
   
   // 时间转换
   ncTimeToLong(sessiontime, &lSessionTime);
	 ncTimeToStr(idletime, sIdleTime);
	 ncTimeToStr(macbindtime, sMacbindTime);
	 ncTimeToLong(moditime, &lModiTime);
	 
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
	 	  sprintf(caTemp1, "select count(*) from ncsrvgroup where groupname='%s'", groupname);
			pasDbOneRecord(caTemp1, 0, UT_TYPE_ULONG, 4, &lCount2);
			if (lCount2>0){
	 				sprintf(caTemp,"update ncsrvgroup set groupname='%s',dispname='%s',sessiontime=%lu,idletime=%lu,macbindtime=%lu,moditime=%lu where groupname='%s' ",groupname,dispname,lSessionTime,atoi(sIdleTime),atoi(sMacbindTime),time(0),groupname);
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  } else {
		    	sprintf(caTemp,"insert into ncsrvgroup (groupname,dispname,sessiontime,idletime,macbindtime,moditime) values ('%s','%s',%lu,%lu,%lu,%lu)",groupname,dispname,lSessionTime,atoi(sIdleTime),atoi(sMacbindTime),time(0));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    			printf("caSesid = %s\n", caSesid);
    		  pasDbExecSqlF("delete from ncsrvgroup where groupname in(%s)",caSesid);
    	}
	 }	 
	 // 查询
	 strcpy(sql, "select count(*) from ncsrvgroup");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " where (groupname like '%%%s%%' or dispname like '%%%s%%') ", _keyword, _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select groupname, dispname, sessiontime, idletime, macbindtime, moditime from ncsrvgroup");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " where (groupname like '%%%s%%' or dispname like '%%%s%%') ", _keyword, _keyword);
	 }
	 
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL) {
				char 					groupname[32] = "";
				char 					dispname[32] = "";
				unsigned long sessiontime = 0;
				unsigned long idletime = 0;
				unsigned long macbindtime = 0;
				unsigned long moditime = 0;
				char          s_sessiontime[21] = "";
				char          s_idletime[32] = "";
				char          s_macbindtime[32] = "";
				char          s_moditime[21] = "";
				
				// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
			  while(0 == (iret = pasDbFetchInto(psCur,
					 UT_TYPE_STRING, sizeof(groupname)-1, groupname,
					 UT_TYPE_STRING, sizeof(dispname)-1,  dispname,
					 UT_TYPE_ULONG,  4,               		&sessiontime,
					 UT_TYPE_ULONG,  4,               		&idletime,
					 UT_TYPE_ULONG,  4,               		&macbindtime,
					 UT_TYPE_ULONG,  4,               		&moditime)) || 1405 == iret)
			  {
					 iNum++;
					 if(iNum > 1)
					 {
						 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
					 }
				  
				   strcpy(s_sessiontime, utTimFormat("%Y/%m/%d %H:%M:%S", sessiontime));
				   strcpy(s_moditime, utTimFormat("%Y/%m/%d %H:%M:%S", moditime));
				   if(strcmp(s_sessiontime, "1970/01/01 00:00:00") == 0){
				     strcpy(s_sessiontime, "");	
				   }
				   if(strcmp(s_moditime, "1970/01/01 00:00:00") == 0){
				     strcpy(s_moditime, "");	
				   }
				  
					 utPltPutLoopVar(psDbHead,"groupname",iNum,groupname);
					 utPltPutLoopVar(psDbHead,"dispname",iNum,dispname);
					 utPltPutLoopVar(psDbHead,"sessiontime",iNum,s_sessiontime);
					 utPltPutLoopVar(psDbHead,"idletime",iNum,utStrLtoF8((long long)idletime, s_idletime));
					 utPltPutLoopVar(psDbHead,"macbindtime",iNum,utStrLtoF8((long long)macbindtime, s_macbindtime));
					 utPltPutLoopVar(psDbHead,"moditime",iNum,s_moditime);
					 memset(s_idletime, 0, sizeof(s_idletime));
					 memset(s_macbindtime, 0, sizeof(s_macbindtime));
			  }
		}
		utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmgroup/ncm_group_list.htm");
	  return 0;	
}
