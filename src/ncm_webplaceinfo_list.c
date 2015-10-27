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

#define FILEPATH "/home/ncmysql/ncsrv/html/ncm_webplacelimit"

int ncTimeToLong(char *stime, unsigned long *lTime);
/*{
	 char ctime[20] = "";
	 
	 snprintf(ctime, sizeof(ctime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", stime[0], stime[1], stime[2], stime[3], stime[5], stime[6], stime[8], stime[9], stime[11], stime[12], stime[14], stime[15], stime[17], stime[18]);	
	 *lTime = utTimStrToLong("%Y%m%d%H%M%S", ctime);
	 
	 return 0;
}*/

int ncTimeToStr(char *cTime, char *sTime);
/*{
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
}*/

// 时间格式转换
int ncTimeToDate(char *limtime, char *stime, char *etime)
{
	 char ctime[20] = "";
	 char dtime[20] = "";
	 snprintf(ctime, sizeof(ctime), "%c%c%c%c%c%c%c%c", limtime[0], limtime[1], limtime[2], limtime[3], limtime[4], limtime[5], limtime[6], limtime[7]);	
	 snprintf(dtime, sizeof(dtime), "%c%c%c%c%c%c%c%c", limtime[9], limtime[10], limtime[11], limtime[12], limtime[13], limtime[14], limtime[15], limtime[16]);
	 strcpy(stime, ctime);
	 strcpy(etime, dtime);
	 
	 return 0;
}
// 取出给定时间格式中的时间值
int ncTimeToTime(char *stime, char *etime)
{
	 char ctime[20] = "";
	 unsigned long lTime = 0;
	 
	 snprintf(ctime, sizeof(ctime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", stime[0], stime[1], stime[2], stime[3], stime[5], stime[6], stime[8], stime[9], stime[11], stime[12], stime[14], stime[15], stime[17], stime[18]);	
	 lTime = utTimStrToLong("%Y%m%d%H%M%S", ctime);
	 strcpy(etime, utTimFormat("%H:%M:%S", lTime));
	 
	 return 0;
}

int strrpl(char* pDstOut, char* pSrcIn, const char* pSrcRpl, const char* pDstRpl)
{ 
		char* pi = pSrcIn; 
		char* po = pDstOut; 
		
		int nSrcRplLen = strlen(pSrcRpl); 
		int nDstRplLen = strlen(pDstRpl); 
		
		char *p = NULL; 
		int nLen = 0; 
		
		do{
				// 找到下一个替换点
				p = strstr(pi, pSrcRpl); 
				
				if(p != NULL) { 
						// 拷贝上一个替换点和下一个替换点中间的字符串
						nLen = p - pi; 
						memcpy(po, pi, nLen);
						
						// 拷贝需要替换的字符串
						memcpy(po + nLen, pDstRpl, nDstRplLen); 
				} else { 
						strcpy(po, pi); 
						
						// 如果没有需要拷贝的字符串,说明循环应该结束
						break;
				} 
				
				pi = p + nSrcRplLen; 
				po = po + nLen + nDstRplLen; 
		
		} while (p != NULL); 
		
		return 0;
}

int ncm_web_Place_smslimit_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char sid[20] = "";
	 char localnum[20] = "";
	 char internum[20] = "";
	 char smsflag[20] = "";
	 char stime[64] = "";
	 char etime[64] = "";
	 //char caStime[64] = "";
	 //char caEtime[64] = "";
	 char limtime[64] = "";
	 char systime[32] = "";
	 char flags[20] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long lSid = 0;
 	 unsigned long lLocalnum = 0;
 	 unsigned long lInternum = 0;
 	 unsigned long lSmsflag = 0;
 	 char caLimtime[64] = "";
	 unsigned long lSystime = 0;
	 unsigned long lFlags = 0;
	 char s_smsflag[32] = "";
	 char s_flags[32] = "";
	 char s_systime[32] = "";
	 char s_stime[32] = "";
	 char s_etime[32] = "";
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caSesid[1024] = "";
	 char caTemp[4096] = "";
	 // 关键字
	 char _keyword[33] = "";
	 
	 // 数据表
	 unsigned long lCount1 = 0; // 表中记录的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 18,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
	    "sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
			"localnum",    UT_TYPE_STRING,  sizeof(localnum)-1,       localnum,
			"internum",    UT_TYPE_STRING,  sizeof(internum)-1,       internum,
			"smsflag",     UT_TYPE_STRING,  sizeof(smsflag)-1,        smsflag,
			"stime",       UT_TYPE_STRING,  sizeof(stime)-1,          stime,
			"etime",       UT_TYPE_STRING,  sizeof(etime)-1,          etime,
			"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
			"flags",       UT_TYPE_STRING,  sizeof(flags)-1,          flags,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 // 排序
	 if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
	 // 去除关键字前后的空格
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(stime)>0){
			pasCvtGBK(2,stime,caTemp,30);
			strcpy(stime,caTemp);
   }
	 if(strlen(etime)>0){
			pasCvtGBK(2,etime,caTemp,30);
			strcpy(etime,caTemp);
   }
   if(strlen(smsflag)>0){
			pasCvtGBK(2,smsflag,caTemp,30);
			strcpy(smsflag,caTemp);
   }
   if(strlen(flags)>0){
			pasCvtGBK(2,flags,caTemp,30);
			strcpy(flags,caTemp);
   }
   
   if(strcmp(smsflag, "使用")==0){
   		strcpy(smsflag, "0");	
   }else if(strcmp(smsflag, "不使用")==0){
   		strcpy(smsflag, "1");		
   }
   
   if(strcmp(flags, "使用")==0){
   		strcpy(flags, "0");	
   }else if(strcmp(flags, "不使用")==0){
   		strcpy(flags, "1");		
   }
   
   /*ncTimeToTime(stime, caStime);
   ncTimeToTime(etime, caEtime);
   printf("caStime = %s\n", caStime);
   printf("caEtime = %s\n", caEtime);
   sprintf(limtime, "%s-%s", caStime, caEtime);*/
   sprintf(limtime, "%s-%s", stime, etime);
	 
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
	 		if (atol(sid)>0){
	 				sprintf(caTemp,"update ncmSmslimit set localnum=%lu, internum=%lu, smsflag=%lu, limtime='%s', systime=%lu, flags=%lu where sid=%lu ",atol(localnum),atol(internum),atol(smsflag),limtime,time(0),atol(flags),atol(sid));
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  } else {
		    	sprintf(caTemp,"insert into ncmSmslimit (localnum,internum,smsflag,limtime,systime,flags) values (%lu,%lu,%lu,'%s',%lu,%lu)",atol(localnum),atol(internum),atol(smsflag),limtime,time(0),atol(flags));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from ncmSmslimit where sid in(%s)",caSesid);
    	}
	 }
	 	 
	 // 查询
	 strcpy(sql, "select count(*) from ncmSmslimit where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (localnum like '%%%s%%' or internum like '%%%s%%')", _keyword, _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select sid, localnum, internum, smsflag, limtime, systime, flags from ncmSmslimit where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (localnum like '%%%s%%' or internum like '%%%s%%')", _keyword, _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),       &lSid,
													  	 UT_TYPE_ULONG,  sizeof(long),       &lLocalnum,
													  	 UT_TYPE_ULONG,  sizeof(long),       &lInternum,
													  	 UT_TYPE_ULONG,  sizeof(long),       &lSmsflag,
													  	 UT_TYPE_STRING, sizeof(limtime)-1,  caLimtime,
															 UT_TYPE_ULONG,  sizeof(long),       &lSystime,
															 UT_TYPE_ULONG,  sizeof(long),       &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 
			 switch(lSmsflag){
			 		case 0:strcpy(s_smsflag, "使用");break;	
			 		case 1:strcpy(s_smsflag, "不使用");break;	
			 		default:break;		
			 }
			 switch(lFlags){
			 		case 0:strcpy(s_flags, "使用");break;	
			 		case 1:strcpy(s_flags, "不使用");break;	
			 		default:break;		
			 }
			 
			 ncTimeToDate(caLimtime, s_stime, s_etime);
			 
			 if(lSystime>0){
			 		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
			 }else{
			 		strcpy(s_systime, "");
			 }
		   
		   utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
		   utPltPutLoopVarF(psDbHead,"localnum",iNum,"%lu",lLocalnum);
		   utPltPutLoopVarF(psDbHead,"internum",iNum,"%lu",lInternum);
		   utPltPutLoopVar(psDbHead,"smsflag",iNum,s_smsflag);
		   utPltPutLoopVar(psDbHead,"stime",iNum,s_stime);
			 utPltPutLoopVar(psDbHead,"etime",iNum,s_etime);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVar(psDbHead,"flags",iNum,s_flags);
			 
			 lSid = 0;
		 	 lLocalnum = 0;
		 	 lInternum = 0;
		 	 lSmsflag = 0;
		 	 memset(caLimtime, 0, sizeof(caLimtime));
			 lSystime = 0;
			 lFlags = 0;
			 memset(s_smsflag, 0, sizeof(s_smsflag));
			 memset(s_stime, 0, sizeof(s_stime));
			 memset(s_etime, 0, sizeof(s_etime));
			 memset(s_systime, 0, sizeof(s_systime));
			 memset(s_flags, 0, sizeof(s_flags));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),       &lSid,
															  	 UT_TYPE_ULONG,  sizeof(long),       &lLocalnum,
															  	 UT_TYPE_ULONG,  sizeof(long),       &lInternum,
															  	 UT_TYPE_ULONG,  sizeof(long),       &lSmsflag,
															  	 UT_TYPE_STRING, sizeof(limtime)-1,  caLimtime,
																	 UT_TYPE_ULONG,  sizeof(long),       &lSystime,
																	 UT_TYPE_ULONG,  sizeof(long),       &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_smslimit_list.htm");
	  return 0;	
}

int ncm_web_Place_placelimit_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char sid[20] = "";
	 char regnum[20] = "";
	 char stime[64] = "";
	 char etime[64] = "";
	 char caStime[64] = "";
	 char caEtime[64] = "";
	 char limtime[64] = "";
	 char systime[32] = "";
	 char flags[20] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long lSid = 0;
 	 unsigned long lRegnum = 0;
 	 char caLimtime[64] = "";
	 unsigned long lSystime = 0;
	 unsigned long lFlags = 0;
	 char s_flags[32] = "";
	 char s_systime[32] = "";
	 char s_stime[32] = "";
	 char s_etime[32] = "";
	 
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
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
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 16,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
	    "sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
			"regnum",      UT_TYPE_STRING,  sizeof(regnum)-1,         regnum,
			"stime",       UT_TYPE_STRING,  sizeof(stime)-1,          stime,
			"etime",       UT_TYPE_STRING,  sizeof(etime)-1,          etime,
			"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
			"flags",       UT_TYPE_STRING,  sizeof(flags)-1,          flags,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 // 排序
	 if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
	 // 去除关键字前后的空格
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(stime)>0){
			pasCvtGBK(2,stime,caTemp,30);
			strcpy(stime,caTemp);
   }
	 if(strlen(etime)>0){
			pasCvtGBK(2,etime,caTemp,30);
			strcpy(etime,caTemp);
   }
   if(strlen(flags)>0){
			pasCvtGBK(2,flags,caTemp,30);
			strcpy(flags,caTemp);
   }
   
   if(strcmp(flags, "使用")==0){
   		strcpy(flags, "0");	
   }else if(strcmp(flags, "不使用")==0){
   		strcpy(flags, "1");		
   }
   
   /*ncTimeToTime(stime, caStime);
   ncTimeToTime(etime, caEtime);
   printf("caStime = %s\n", caStime);
   printf("caEtime = %s\n", caEtime);
   sprintf(limtime, "%s-%s", caStime, caEtime);*/
   sprintf(limtime, "%s-%s", stime, etime);
	 
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
	 		if (atol(sid)>0){
	 				sprintf(caTemp,"update ncmplacelimit set regnum=%lu, limtime='%s', systime=%lu, flags=%lu where sid=%lu ",atol(regnum),limtime,time(0),atol(flags),atol(sid));
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  } else {
		    	sprintf(caTemp,"insert into ncmplacelimit (regnum,limtime,systime,flags) values (%lu,'%s',%lu,%lu)",atol(regnum),limtime,time(0),atol(flags));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from ncmplacelimit where sid in(%s)",caSesid);
    	}
	 }
	 	 
	 // 查询
	 strcpy(sql, "select count(*) from ncmplacelimit where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and regnum like '%%%s%%'", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select sid, regnum, limtime, systime, flags from ncmplacelimit where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and regnum like '%%%s%%'", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),        &lSid,
													  	 UT_TYPE_ULONG,  sizeof(long),        &lRegnum,
													  	 UT_TYPE_STRING, sizeof(caLimtime)-1, caLimtime,
															 UT_TYPE_ULONG,  sizeof(long),        &lSystime,
															 UT_TYPE_ULONG,  sizeof(long),        &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 
			 switch(lFlags){
			 		case 0:strcpy(s_flags, "使用");break;	
			 		case 1:strcpy(s_flags, "不使用");break;	
			 		default:break;		
			 }
			 
			 ncTimeToDate(caLimtime, s_stime, s_etime);
			 
			 if(lSystime>0){
			 		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
			 }else{
			 		strcpy(s_systime, "");
			 }
		   
		   utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
		   utPltPutLoopVarF(psDbHead,"regnum",iNum,"%lu",lRegnum);
		   utPltPutLoopVar(psDbHead,"stime",iNum,s_stime);
			 utPltPutLoopVar(psDbHead,"etime",iNum,s_etime);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVar(psDbHead,"flags",iNum,s_flags);
			 
			 lSid = 0;
		 	 lRegnum = 0;
		 	 memset(caLimtime, 0, sizeof(caLimtime));
			 lSystime = 0;
			 lFlags = 0;
			 memset(s_stime, 0, sizeof(s_stime));
			 memset(s_etime, 0, sizeof(s_etime));
			 memset(s_systime, 0, sizeof(s_systime));
			 memset(s_flags, 0, sizeof(s_flags));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),        &lSid,
															  	 UT_TYPE_ULONG,  sizeof(long),        &lRegnum,
															  	 UT_TYPE_STRING, sizeof(caLimtime)-1, caLimtime,
																	 UT_TYPE_ULONG,  sizeof(long),        &lSystime,
																	 UT_TYPE_ULONG,  sizeof(long),        &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_placelimit_list.htm");
	  return 0;	
}

int ncm_web_Place_timelimit_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char sid[32] = "";
	 char intertime[64] = "";
	 char freetime[64] = "";
	 char idletime[64] = "";
	 char timeout[64] = "";
	 char systime[64] = "";
	 char flags[32] = "";
	 
	 // 存放数据库取出的数据的变量
   unsigned long lSid = 0;
	 unsigned long lIntertime = 0;
	 unsigned long lFreetime = 0;
	 unsigned long lIdletime = 0;
	 unsigned long lTimeout = 0;
	 unsigned long lSystime = 0;
	 unsigned long lFlags = 0;
	 char          s_intertime[32] = "";
	 char          s_freetime[32] = "";
	 char          s_idletime[32] = "";
	 char          s_timeout[32] = "";
	 char          s_systime[32] = "";
	 char          s_flags[32] = "";
	 
	 char caUpdate[16] = "";
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
	 char sInterTime[32] = "";
	 char sFreeTime[32] = "";
	 char sIdleTime[32] = "";
	 char sTimeOut[32] = "";
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 17,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
			"intertime",   UT_TYPE_STRING,  sizeof(intertime)-1,      intertime,
			"freetime",    UT_TYPE_STRING,  sizeof(freetime)-1,       freetime,
			"idletime",    UT_TYPE_STRING,  sizeof(idletime)-1,       idletime,
			"timeout",     UT_TYPE_STRING,  sizeof(timeout)-1,        timeout,
			"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
			"flags",       UT_TYPE_STRING,  sizeof(flags)-1,          flags,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(flags)>0){
			pasCvtGBK(2,flags,caTemp,30);
			strcpy(flags,caTemp);
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
	 	  strcpy(sort,"sid");
	 }
	 
	 if(strcmp(flags, "使用")==0){
   		strcpy(flags, "0");	
   }else if(strcmp(flags, "不使用")==0){
   		strcpy(flags, "1");		
   }
   
   // 时间转换
   ncTimeToStr(intertime, sInterTime);
   ncTimeToStr(freetime, sFreeTime);
	 ncTimeToStr(idletime, sIdleTime);
	 ncTimeToStr(timeout, sTimeOut);
	 ncTimeToLong(systime, &lSystime);
	 
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
			if (strlen(sid)>0){
	 				sprintf(caTemp,"update ncmTimelimit set intertime=%lu,freetime=%lu,idletime=%lu,timeout=%lu,systime=%lu,flags=%lu where sid=%lu ",atol(sInterTime),atol(sFreeTime),atol(sIdleTime),atol(sTimeOut),time(0),atol(flags),atol(sid));
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  } else {
		    	sprintf(caTemp,"insert into ncmTimelimit (intertime,freetime,idletime,timeout,systime,flags) values (%lu,%lu,%lu,%lu,%lu,%lu)",atol(sInterTime),atol(sFreeTime),atol(sIdleTime),atol(sTimeOut),time(0),atol(flags));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    			printf("caSesid = %s\n", caSesid);
    		  pasDbExecSqlF("delete from ncmTimelimit where sid in(%s)",caSesid);
    	}
	 }	 
	 // 查询
	 strcpy(sql, "select count(*) from ncmTimelimit where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and intertime like '%%%s%%'", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select sid, intertime, freetime, idletime, timeout, systime, flags from ncmTimelimit where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and intertime like '%%%s%%' ", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),   &lSid,
															 UT_TYPE_ULONG,  sizeof(long),   &lIntertime,
															 UT_TYPE_ULONG,  sizeof(long),   &lFreetime,
															 UT_TYPE_ULONG,  sizeof(long),   &lIdletime,
															 UT_TYPE_ULONG,  sizeof(long),   &lTimeout,
															 UT_TYPE_ULONG,  sizeof(long),   &lSystime,
															 UT_TYPE_ULONG,  sizeof(long),   &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 if(lSystime>0){
			 		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
			 }else{
			 		strcpy(s_systime, "");
			 }
			 
			 switch(lFlags){
			 		case 0:strcpy(s_flags, "使用");break;	
			 		case 1:strcpy(s_flags, "不使用");break;	
			 		default:break;		
			 }
			 
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVar(psDbHead,"intertime",iNum,utStrLtoF8((long long)lIntertime, s_intertime));
			 utPltPutLoopVar(psDbHead,"freetime",iNum,utStrLtoF8((long long)lFreetime, s_freetime));
			 utPltPutLoopVar(psDbHead,"idletime",iNum,utStrLtoF8((long long)lIdletime, s_idletime));
			 utPltPutLoopVar(psDbHead,"timeout",iNum,utStrLtoF8((long long)lTimeout, s_timeout));
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVar(psDbHead,"flags",iNum,s_flags);
			 
			 
			 lSid = 0;
			 lIntertime = 0;
			 lFreetime = 0;
			 lIdletime = 0;
			 lTimeout = 0;
			 lSystime = 0;
			 lFlags = 0;
			 memset(s_intertime, 0, sizeof(s_intertime));
			 memset(s_freetime, 0, sizeof(s_freetime));
			 memset(s_idletime, 0, sizeof(s_idletime));
			 memset(s_timeout, 0, sizeof(s_timeout));
			 memset(s_systime, 0, sizeof(s_systime));
			 memset(s_flags, 0, sizeof(s_flags));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),   &lSid,
																	 UT_TYPE_ULONG,  sizeof(long),   &lIntertime,
																	 UT_TYPE_ULONG,  sizeof(long),   &lFreetime,
																	 UT_TYPE_ULONG,  sizeof(long),   &lIdletime,
																	 UT_TYPE_ULONG,  sizeof(long),   &lTimeout,
																	 UT_TYPE_ULONG,  sizeof(long),   &lSystime,
																	 UT_TYPE_ULONG,  sizeof(long),   &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_timelimit_list.htm");
	  return 0;	
}

// 短信格式
int ncm_web_Place_smsformat_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char sid[32] = "";
	 char content[65535] = "";
	 char language[32] = "";
	 char caLanguage[32] = "";
	 char systime[64] = "";
	 char flags[32] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long lSid = 0;
	 char          caContent[65535] = "";
	 unsigned long lLanguage = 0;
	 unsigned long lSystime = 0;
	 unsigned long lFlags = 0;
	 char          s_systime[32] = "";
	 char          s_flags[32] = "";
	 char          s_language[32] = "";
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caSesid[1024] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
	 // 关键字
	 char _keyword[33] = "";
	 
	 // 数据表
	 unsigned long lCount1 = 0; // 表中记录的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 15,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
			"content",     UT_TYPE_STRING,  sizeof(content)-1,        content,
			"language",    UT_TYPE_STRING,  sizeof(language)-1,       language,
			"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
			"flags",       UT_TYPE_STRING,  sizeof(flags)-1,          flags,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(content)>0){
			pasCvtGBK(2,content,caTemp,65535);
			strcpy(content,caTemp);
   }
   if(strlen(language)>0){
			pasCvtGBK(2,language,caTemp,30);
			strcpy(language,caTemp);
   }
   if(strlen(flags)>0){
			pasCvtGBK(2,flags,caTemp,30);
			strcpy(flags,caTemp);
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
	 	  strcpy(sort,"sid");
	 }
   
   if(strcmp(flags, "使用")==0){
   		strcpy(flags, "0");	
   }else if(strcmp(flags, "不使用")==0){
   		strcpy(flags, "1");		
   }
   if(strcmp(language, "中文")==0){
   		strcpy(language, "0");	
   }else if(strcmp(language, "英文")==0){
   		strcpy(language, "1");		
   }
   
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
			if (strlen(sid)>0){
	 				sprintf(caTemp,"update ncmSmsformat set content='%s',language=%lu,systime=%lu,flags=%lu where sid=%lu ",content,atol(language),time(0),atol(flags),atol(sid));
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  } else {
		    	sprintf(caTemp,"insert into ncmSmsformat (content,language,systime,flags) values ('%s',%lu,%lu,%lu)",content,atol(language),time(0),atol(flags));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    			printf("caSesid = %s\n", caSesid);
    		  pasDbExecSqlF("delete from ncmSmsformat where sid in(%s)",caSesid);
    	}
	 }	 
	 // 查询
	 strcpy(sql, "select count(*) from ncmSmsformat where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and content like '%%%s%%'", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select sid, content, language, systime, flags from ncmSmsformat where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and content like '%%%s%%' ", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),        &lSid,
															 UT_TYPE_STRING, sizeof(caContent)-1, caContent,
															 UT_TYPE_ULONG,  sizeof(long),        &lLanguage,
															 UT_TYPE_ULONG,  sizeof(long),        &lSystime,
															 UT_TYPE_ULONG,  sizeof(long),        &lFlags);
	
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 
			 if(lSystime>0){
			 		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));	
			 }else{
			 		strcpy(s_systime, "");
			 }
			 switch(lLanguage){
			 		case 0:strcpy(s_language, "中文");break;
			 		case 1:strcpy(s_language, "英文");break;
			 		default:break;	
			 }
			 switch(lFlags){
			 		case 0:strcpy(s_flags, "使用");break;	
			 		case 1:strcpy(s_flags, "不使用");break;	
			 		default:break;		
			 }

			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVar(psDbHead,"content",iNum,caContent);
			 utPltPutLoopVarF(psDbHead,"language",iNum,s_language);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVar(psDbHead,"flags",iNum,s_flags);
			 
			 lSid = 0;
			 memset(caContent, 0, sizeof(caContent));
			 lLanguage = 0;
			 lSystime = 0;
			 lFlags = 0;
			 memset(s_systime, 0, sizeof(s_systime));
			 memset(s_flags, 0, sizeof(s_flags));
			 memset(s_language, 0, sizeof(s_language));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),        &lSid,
																	 UT_TYPE_STRING, sizeof(caContent)-1, caContent,
																	 UT_TYPE_ULONG,  sizeof(long),        &lLanguage,
																	 UT_TYPE_ULONG,  sizeof(long),        &lSystime,
																	 UT_TYPE_ULONG,  sizeof(long),        &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_smsformat_list.htm");
	  
	  return 0;	
}

// 终端操作系统
int ncm_web_Place_os_chart(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
		utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
		
		// 初始化数据库
		char sql[4096] = "";
		pasDbCursor *psCur = NULL;
		int iret=0;
		int iNum=0;
		utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
		
		char sdate[32] = "";
		char edate[32] = "";
		char groupcode[32] = "";
		char areaid[32] = "";
		char groupid[32] = "";
		
		unsigned long lsdate;
	  unsigned long ledate;
	  
	  unsigned long iNumIOS = 0;
	  
	  char chart_name[32] = "";
		unsigned long chart_number = 0;
		double rate = 0;
		double total = 0;
		char rates[32] = "";
	  
	  // 数据表
	  unsigned long lCount1 = 0; // 表中记录的个数
	  long iReturn; // 单记录查询
		
		// 导出
	  char expexcel[32] = "";
	  FILE *fp = NULL;
	  char caFilename[32] = "";
	  char caFile[128] = "";
	  char caPath[128] = "";
	 char caShopids[1024];
	 strcpy(caShopids,getDsShopids());
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
	  
	  // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
		utMsgGetSomeNVar(psMsgHead, 6,"sdate",        UT_TYPE_STRING,  sizeof(sdate)-1,       sdate,
																	"edate",        UT_TYPE_STRING,  sizeof(edate)-1,       edate,
																	"groupcode",    UT_TYPE_STRING,  sizeof(groupcode)-1,   groupcode,
																	"areaid",       UT_TYPE_STRING,  sizeof(areaid)-1,      areaid,
																	//"groupid",      UT_TYPE_STRING,  sizeof(groupid)-1,     groupid,
																	"expexcel",     UT_TYPE_STRING,  sizeof(expexcel)-1,    expexcel);
		
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
	  
	  sprintf(sql,"select count(*) from ncsrvuserlog where starttime >= %lu and endtime <= %lu", lsdate, ledate);
	  	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
	  
//	  strcpy(sql, (char *)ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
		pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	  utPltPutVarF(psDbHead,"TotRec","%lu", lCount1);
	  
	  sprintf(sql,"select count(*) from ncsrvuserlog where starttime >= %lu and endtime <= %lu and dev='iPhone'", lsdate, ledate);	
	  if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
	  strcpy(sql, (char *)ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
		pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &iNumIOS);
	  
	  
	  // 导出
	  if(strlen(expexcel)>0){
				 strcpy(caFilename, "OSchart.csv");
				 strcpy(caPath, FILEPATH);
				 sprintf(caFile, "%s/%s", caPath, caFilename);
		 		 printf("caFile = %s\n", caFile);
		 		 fp = fopen(caFile, "w");
		 		 if(fp == NULL){
		 		 		 pasDbCloseCursor(psCur);
			       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",(char *)ncLang("0546数据导出出错"),(char *)ncLang("0546数据导出出错"));
			       return 0;	
		 		 }
		 		 
		 		 fprintf(fp,"操作系统数量图表\n");
		 		 fprintf(fp,"时间: %s -- %s\n",sdate,edate);
				 ncUtlPrintCsv(fp,4,UT_TYPE_STRING,"序号",
				 									  UT_TYPE_STRING,"操作系统",
														UT_TYPE_STRING,"数量",
									          UT_TYPE_STRING,"比例");
	  }
	  
	  sprintf(sql,"select os, count(os) as number from ncsrvuserlog where starttime >= %lu and endtime <= %lu", lsdate, ledate);	
	  strcpy(sql, (char *)ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	  	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and groupcode in (select groupname from ncsrvgroup where groupid in (%s))",caShopids);
	 }
	  snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by os order by number desc");
		printf("sql = %s\n", sql);
		psCur = pasDbOpenSql(sql,0);
		if(psCur != NULL){
				total = lCount1;
				iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(chart_name)-1,chart_name,
																		   UT_TYPE_LONG,  sizeof(long),        &chart_number);
			  unsigned long iNumAndroid = 0;
			  
		
			  unsigned long iNumOther = 0;
			  char *android = "Android";
			 	char *ios = "IOS";
		
			  char *other = "Other";
			  while(0==iReturn  || 1405==iReturn){
			  	printf("系统名字%s\n",chart_name);
			  	printf("数量%lu\n",chart_number);
			  	char *cname = &chart_name;
			  		if(strstr(cname,android)!=NULL)
			  		{
			  				iNumAndroid += chart_number;

			  		}

			  
					  iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(chart_name)-1,chart_name,
																		       UT_TYPE_LONG,  sizeof(long),        &chart_number);	
      	}
      		printf("total : %lu \n",lCount1);
      		iNumOther = lCount1-iNumAndroid-iNumIOS;
      		printf("iNumOther : %lu \n",iNumOther);
      		printf("iNumAndroid : %lu \n",iNumAndroid);
      		printf("iNumIOS : %lu \n",iNumIOS);
      	
      			iNum++;
      				if(iNum > 1){
								utPltPutLoopVar(psDbHead,"dh",iNum,",");
							}
      		  utPltPutLoopVarF(psDbHead,"chart_number",iNum,"%lu", iNumOther);
						utPltPutLoopVar(psDbHead,"chart_name",iNum,other);
						utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);
						rate = iNumOther/total;
						sprintf(rates,"%0.2f%%",rate*100);
						utPltPutLoopVarF(psDbHead,"rate",iNum,"%s",rates);
						if(strlen(expexcel)>0){
							 ncUtlPrintCsv(fp,4,UT_TYPE_LONG, iNum,
		                          UT_TYPE_STRING,other,
		                          UT_TYPE_LONG,iNumOther,
															UT_TYPE_STRING,rates);
					  } 							
      	
      		iNum++;	
      			if(iNum > 1){
								utPltPutLoopVar(psDbHead,"dh",iNum,",");
						}
      		  utPltPutLoopVarF(psDbHead,"chart_number",iNum,"%lu", iNumAndroid);
						utPltPutLoopVar(psDbHead,"chart_name",iNum,android);
						utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);
						rate = iNumAndroid/total;
						sprintf(rates,"%0.2f%%",rate*100);
						utPltPutLoopVarF(psDbHead,"rate",iNum,"%s",rates);
						if(strlen(expexcel)>0){
							 ncUtlPrintCsv(fp,4,UT_TYPE_LONG, iNum,
		                          UT_TYPE_STRING,android,
		                          UT_TYPE_LONG,iNumAndroid,
															UT_TYPE_STRING,rates);
					  } 

      		iNum++;	
						if(iNum > 1){
								utPltPutLoopVar(psDbHead,"dh",iNum,",");
						}
      		  utPltPutLoopVarF(psDbHead,"chart_number",iNum,"%lu", iNumIOS);
						utPltPutLoopVar(psDbHead,"chart_name",iNum,ios);
						utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);
						rate = iNumIOS/total;
						sprintf(rates,"%0.2f%%",rate*100);
						utPltPutLoopVarF(psDbHead,"rate",iNum,"%s",rates);  
						if(strlen(expexcel)>0){
							 ncUtlPrintCsv(fp,4,UT_TYPE_LONG, iNum,
		                          UT_TYPE_STRING,ios,
		                          UT_TYPE_LONG,iNumIOS,
															UT_TYPE_STRING,rates);
					  }     		
     		   	
		}
		pasDbCloseCursor(psCur);
		//utPltShowDb(psDbHead);
		
		if(strlen(expexcel)>0){
	 			fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	}
	 
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_webplacelimit/ncm_web_Place_os_chart.htm");
	  return 0;
}

int ncm_web_Place_usernum_chart(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
		utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
		
		// 初始化数据库
		char sql[4096] = "";
		pasDbCursor *psCur = NULL;
		int iret=0;
		int iNum=0;
		utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
		
		char sdate[32] = "";
		char edate[32] = "";
		
		unsigned long lsdate;
	  unsigned long ledate;
	  
	  // 数据表
	  unsigned long lCount1 = 0; // 表中记录的个数
	  long iReturn; // 单记录查询
		
		// 导出
	  char expexcel[32] = "";
	  FILE *fp = NULL;
	  char caFilename[32] = "";
	  char caFile[128] = "";
	  char caPath[128] = "";
	  
	  // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
		utMsgGetSomeNVar(psMsgHead, 3,
			"sdate",        UT_TYPE_STRING,  sizeof(sdate)-1,       sdate,
			"edate",        UT_TYPE_STRING,  sizeof(edate)-1,       edate,
			"expexcel",     UT_TYPE_STRING,  10,                    expexcel
		);
		
		ncTimeToLong(sdate,&lsdate);
	  ncTimeToLong(edate,&ledate);
	  sprintf(sql,"select count(*) from ncsrvsmslog where logtime >= %lu and logtime <= %lu", lsdate, ledate);
		pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	  utPltPutVarF(psDbHead,"TotRec","%lu", lCount1);
	  
	  // 导出
	 if(strlen(expexcel)>0){
			 strcpy(caFilename, "usernumchart.csv");
			 strcpy(caPath, FILEPATH);
			 sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",(char *)ncLang("0546数据导出出错"),(char *)ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"用户数量统计\n");
	 		 fprintf(fp,"时间: %s -- %s\n",sdate,edate);
			 ncUtlPrintCsv(fp,4,UT_TYPE_STRING,"序号",
			 									  UT_TYPE_STRING,"类别",
		                      UT_TYPE_STRING,"数量",
								          UT_TYPE_STRING,"比例");
	 }
	  
	  sprintf(sql,"select sptype, count(username) as number from ncsrvsmslog where logtime >= %lu and logtime <= %lu", lsdate, ledate);	
	  snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by sptype order by number desc");
		printf("sql = %s\n", sql);
		psCur = pasDbOpenSql(sql,0);
		if(psCur != NULL){
				char chart_name[32] = "";
				unsigned long chart_number = 0;
				double rate=0;
				double total = lCount1;
				char rates[32]="";
				unsigned long sptype = 0;
				while(0==(iReturn = pasDbFetchInto(psCur,
      	      UT_TYPE_LONG,4,&sptype,
      	      UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
      	{
      			iNum++;
						if(iNum > 1){
								utPltPutLoopVar(psDbHead,"dh",iNum,",");
						}
						switch(sptype){
								case 0:strcpy(chart_name, "不合法");break;
								case 1:strcpy(chart_name, "移动");break;
								case 2:strcpy(chart_name, "联通");break;
								case 3:strcpy(chart_name, "电信");break;
								default:strcpy(chart_name, "其他");break;
						}
						utPltPutLoopVarF(psDbHead,"chart_number",iNum,"%lu", chart_number);
						utPltPutLoopVarF(psDbHead,"chart_name",iNum,"%s", chart_name);
						utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);
						rate = chart_number/total;
						sprintf(rates,"%0.2f%%",rate*100);
						utPltPutLoopVarF(psDbHead,"rate",iNum,"%s",rates);
						
						if(strlen(expexcel)>0){
							 ncUtlPrintCsv(fp,4,UT_TYPE_LONG, iNum,
		                          UT_TYPE_STRING,chart_name,
		                          UT_TYPE_LONG,chart_number,
															UT_TYPE_STRING,rates);
					  } 	
      	}
		}
		/*char chart_name1[32] = "";
		for(; iNum<11; iNum++){
				utPltPutLoopVar(psDbHead,"dh",iNum,",");
				sprintf(chart_name1, "0%d", iNum);
				utPltPutLoopVar(psDbHead,"chart_name",iNum,chart_name1);
		}
		pasDbCloseCursor(psCur);
		utPltShowDb(psDbHead);*/
		
		if(strlen(expexcel)>0){
	 			fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        //remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	}
	 
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_webplacelimit/ncm_web_Place_usernum_chart.htm");
	  return 0;
}

int ncm_web_Place_white_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char pid[20] = "";
	 char ptype[20] = "";
	 char wtype[20] = "";
	 char nametype[20] = "";
	 char allname[64000] = "";
	 char temp[64000] = "";
	 	
	 // 存放数据库取出的数据的变量
	 unsigned long	lPid = 0;
	 unsigned long	lPtype = 0;
	 unsigned long	lWtype = 0;
	 unsigned long	lNametype = 0;
	 char           caName[128] = "";
	 char           caAllname[64000] = "";
	 
	 char caTemp0[200] = "";
	 char caTemp1[200] = "";
	 char caUpdate[16] = "";
	 char caSesid[1024] = "";
	 char caDel[16] = "";
	 char caTemp[1024] = "";
	 char caTempd[1024] = "";
	 char caTempt[1024] = "";
	 unsigned long lCount = 0;
	 // 关键字
	 char _keyword[33] = "";
	 
	 // 数据表
	 unsigned long lCount1 = 0; // 表中记录的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
   
   char *p=NULL;
	 char *q=NULL;
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 15,"keyword",   UT_TYPE_STRING, sizeof(_keyword)-1,  _keyword,
	 																"pid",       UT_TYPE_STRING, sizeof(pid)-1,       pid,
	                                "ptype",     UT_TYPE_STRING, sizeof(ptype)-1,     ptype,
	                                "wtype",     UT_TYPE_STRING, sizeof(wtype)-1,     wtype,
	                                "nametype",  UT_TYPE_STRING, sizeof(nametype)-1,  nametype,
	                                "allname",   UT_TYPE_STRING, sizeof(allname)-1,   allname,
	                                "tmp",       UT_TYPE_STRING, sizeof(temp)-1,      temp,
	                                "start",     UT_TYPE_STRING, sizeof(start)-1,     start,
															   	"limit",     UT_TYPE_STRING, sizeof(limit)-1,     limit,
															   	"sort",      UT_TYPE_STRING, sizeof(sort)-1,      sort,
															   	"dir",       UT_TYPE_STRING, sizeof(dir)-1,       dir,
																	"selsid",    UT_TYPE_STRING, sizeof(caSesid)-1,   caSesid,
																	"del",			 UT_TYPE_STRING, sizeof(caDel)-1,     caDel,
																	"update",    UT_TYPE_STRING, sizeof(caUpdate)-1,  caUpdate,
																	"caExport",  UT_TYPE_STRING, sizeof(caExport)-1,  caExport);
	 
	 // 排序
	 if(strlen(sort)==0){
	 	  strcpy(sort,"name");
	 }
	 // 去除关键字前后的空格
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(allname)>0){
			pasCvtGBK(2,allname,caTemp,64000);
			strcpy(allname,caTemp);
   }
   if(strlen(temp)>0){
			pasCvtGBK(2,temp,caTemp,64000);
			strcpy(temp,caTemp);
   }
	 
   // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
	 		if (strcmp(caUpdate, "update")==0){
	 			  if(strlen(temp) != 0){
	 			  		q =	utStrSkipSpaces(temp);
	 			  		while(q && *q){
	 			  				q = utStrGetWord(q,caTemp0,512,"\n");
	 			  				sprintf(caTempd,"delete from ncsrvwhite where pid=%lu and ptype=%lu and wtype=%lu and nametype=%lu and name='%s'", atol(pid),atol(ptype),atol(wtype),atol(nametype),caTemp0);
					 				//printf("caTempd = %s\n", caTempd);
							 		pasDbExecSqlF(caTempd);
	 			  				q++;	
	 			  		}
	 			  }
	 			  
	 			  if(strlen(allname) != 0){
	 			  		p =	utStrSkipSpaces(allname);
	 			  		while(p && *p){
	 			  				p = utStrGetWord(p,caTemp1,200,"\n");
	 			  				sprintf(caTempt, "select count(*) from ncsrvwhite where pid=%lu and ptype=%lu and wtype=%lu and nametype=%lu and name='%s'", atol(pid),atol(ptype),atol(wtype),atol(nametype),caTemp1);
	 			  				//printf("caTempt = %s\n", caTempt);
	 			  				pasDbOneRecord(caTempt, 0, UT_TYPE_ULONG, 4, &lCount);
	 			  				//printf("lCount = %lu\n", lCount);
	 			  				if(lCount==0){
	 			  						sprintf(caTemp,"insert into ncsrvwhite (pid, ptype, wtype, nametype, name) values (%lu, %lu, %lu, %lu, '%s')", atol(pid),atol(ptype),atol(wtype),atol(nametype),caTemp1);
							 				//printf("caTemp = %s\n", caTemp);
									 		pasDbExecSqlF(caTemp);	
	 			  				}
	 			  				p++;	
	 			  		}
	 			  }
	 	  }
	 }
	 
	 // 查询
	 strcpy(sql, "select count(*) from ncsrvwhite where 1=1");
	 if(!utStrIsSpaces(pid))
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and pid = %lu ", atol(pid));
	 }
	 if(!utStrIsSpaces(ptype))
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ptype = %lu ", atol(ptype));
	 }
	 if(!utStrIsSpaces(wtype))
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and wtype = %lu ", atol(wtype));
	 }
	 if(!utStrIsSpaces(nametype))
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and nametype = %lu ", atol(nametype));
	 }
	 printf("sql = %s\n", sql);
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select pid, ptype, wtype, nametype, name from ncsrvwhite where 1=1");
	 if(!utStrIsSpaces(pid))
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and pid = %lu ", atol(pid));
	 }
	 if(!utStrIsSpaces(ptype))
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ptype = %lu ", atol(ptype));
	 }
	 if(!utStrIsSpaces(wtype))
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and wtype = %lu ", atol(wtype));
	 }
	 if(!utStrIsSpaces(nametype))
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and nametype = %lu ", atol(nametype));
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lPid,
	                             UT_TYPE_ULONG,  sizeof(long),          &lPtype,
													     UT_TYPE_ULONG,  sizeof(long),          &lWtype,
													     UT_TYPE_ULONG,  sizeof(long),          &lNametype,
															 UT_TYPE_STRING, sizeof(caName)-1,      caName);	
		
	  while(0 == iret || 1405 == iret){
			 if(strlen(caAllname)==0){
			 		strcpy(caAllname, caName);	
			 }else{
			 		sprintf(caAllname+strlen(caAllname), ",%s", caName);	
			 }
			 //printf("caAllname = %s\n", caAllname);
			 
			 lPid = 0;
			 lPtype = 0;
			 lWtype = 0;
			 lNametype = 0;
	     memset(caName, 0, sizeof(caName));
	     iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lPid,
			                             UT_TYPE_ULONG,  sizeof(long),          &lPtype,
															     UT_TYPE_ULONG,  sizeof(long),          &lWtype,
															     UT_TYPE_ULONG,  sizeof(long),          &lNametype,
																	 UT_TYPE_STRING, sizeof(caName)-1,      caName);			
	  }
	  pasDbCloseCursor(psCur);
	  
	  //utPltSetCvtHtml(1);
	  utPltPutVarF(psDbHead,"pid","%lu",atol(pid));
	  utPltPutVarF(psDbHead,"ptype","%lu",atol(ptype));	
	  if(!utStrIsSpaces(wtype)){
	  		utPltPutVarF(psDbHead,"wtype","%lu",atol(wtype));	
	  }else{
	  		utPltPutVar(psDbHead,"wtype","");	
	  }
	  if(!utStrIsSpaces(nametype)){
	  	  utPltPutVarF(psDbHead,"nametype","%lu",atol(nametype));
	  }else{
	  		utPltPutVar(psDbHead,"nametype","");	
	  }
	  if(!utStrIsSpaces(wtype) || !utStrIsSpaces(nametype)){
	  		utPltPutVar(psDbHead,"allname",caAllname);	
	  }else{
	  		utPltPutVar(psDbHead,"allname","");	
	  }
	  //utPltSetCvtHtml(0);
	  
	  //utPltShowDb(psDbHead);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_white_list.htm");
	  return 0;	
}

int ncm_web_Place_plate_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 int i = 0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char plateid[32] = "";
	 char shopplateid[20] = "";
	 char gcode[32] = "";
	 char groupcode[32] = "";
	 char platename[128] = "";
	 char platepath[128] = "";
	 char addtime[32] = "";
	 char moditime[32] = "";
	 char areaid[32] = "";
	 
	 // 存放数据库取出的数据的变量
	 char          caPlateid[32] = "";
	 unsigned long lShopplateid = 0;
	 char          caGroupcode[32] = "";
	 char          caDispname[32] = "";
	 char          caPlatename[128] = "";
	 char          caPlatepath[128] = "";
	 unsigned long lAddtime = 0;
	 unsigned long lModitime = 0;
	 char          s_addtime[32] = "";
	 char          s_moditime[32] = "";
	 unsigned long lMaxid = 0;
	 unsigned long lpageid = 0;
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caSesid[1024] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
	 // 关键字
	 char _keyword[33] = "";
	 
	 // 数据表
	 unsigned long lCount1 = 0; // 表中记录的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
   
   // 得到商家ID
	 unsigned long lCid = 0;
	 char caStyped[32] = "";
	 char caOptd[32] = "";
	 dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	 dsCltGetSessionValue(1, "opt", UT_TYPE_STRING,sizeof(caOptd)-1, caOptd);
	 dsCltGetSessionValue(1, "stype", UT_TYPE_STRING,sizeof(caStyped)-1, caStyped);
	 printf("lCid = %d\n", lCid);
	 printf("caOptd = %s\n", caOptd);
	 printf("caStyped = %s\n", caStyped);
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 17,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"shopplateid", UT_TYPE_STRING,  sizeof(shopplateid)-1,    shopplateid,
																	"gcode",       UT_TYPE_STRING,  sizeof(gcode)-1,          gcode,
																	"groupcode",   UT_TYPE_STRING,  sizeof(groupcode)-1,      groupcode,
																	"platename",   UT_TYPE_STRING,  sizeof(platename)-1,      platename,
																	"platepath",   UT_TYPE_STRING,  sizeof(platepath)-1,      platepath,
																	"addtime",     UT_TYPE_STRING,  sizeof(addtime)-1,        addtime,
																	"moditime",    UT_TYPE_STRING,  sizeof(moditime)-1,       moditime,
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
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(plateid)>0){
			pasCvtGBK(2,plateid,caTemp,32);
			strcpy(plateid,caTemp);
   }
   if(strlen(platename)>0){
			pasCvtGBK(2,platename,caTemp,128);
			strcpy(platename,caTemp);
   }
   if(strlen(platepath)>0){
			pasCvtGBK(2,platepath,caTemp,128);
			strcpy(platepath,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid));
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"addtime");
	 }
	 
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
	 	  unsigned long lSid = 0;
  	  lSid=utMd5Code(plateid,strlen(plateid),"ffff");
      if(lSid>2000000000) lSid=lSid-2000000000;
      if(lSid>2000000000) lSid=lSid-2000000000;
      printf("lSid = %lu\n",lSid);
      
	 		if (strcmp(caUpdate, "update")==0){
	 			  unsigned long laddtime = 0;
	 			  ncTimeToLong(addtime, &laddtime);
	 			  
	 			  memset(caTemp, 0, sizeof(caTemp));
	 				sprintf(caTemp,"update portalplate set plateid='%s', shopplateid=%lu, groupcode='%s', platename='%s', platepath='%s', addtime=%lu, modtime=%lu where shopplateid='%s' ", plateid, lSid, gcode, platename,platepath,laddtime,time(0),shopplateid);
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  } else if(strcmp(caUpdate, "add")==0) {
	 	  	  memset(caTemp, 0, sizeof(caTemp));
		    	sprintf(caTemp,"insert into portalplate (plateid,shopplateid,groupcode,platename,platepath,addtime,modtime) values ('%s',%lu,'%s','%s','%s',%lu,%lu)", plateid,lSid,gcode,platename,platepath,time(0),time(0));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from portalplate where plateid in(%s)",caSesid);
    		  pasDbExecSqlF("delete from portalprule where plateid in(%s)",caSesid);
    		  
    		  char *p;
    		  p=strtok(caSesid, ",");
    		  while(p){
    		  	  sprintf(caTemp, "select pageid from portalplist where plateid=%s", p);
    		  	  printf("caTemp = %s\n", caTemp);
							psCur = pasDbOpenSql(caTemp, 0);
							if(psCur==NULL){
							 		 return 0;	
							}
							
							iret = pasDbFetchInto(psCur,UT_TYPE_ULONG, sizeof(long), &lpageid);
							iNum=0;
							unsigned long lpid[12] = {0};
							while(0 == iret || 1405 == iret){
								  lpid[iNum] = lpageid;
								  
								  iNum++;
									lpageid = 0;
									iret = pasDbFetchInto(psCur,UT_TYPE_ULONG, sizeof(long), &lpageid);	
							}
							pasDbCloseCursor(psCur);
							
							for(i=0; i<iNum; i++){
									pasDbExecSqlF("delete from portalpage where pageid = %lu",lpid[i]);	
							}
							
    		  		p=strtok(NULL, ",");	
    		  }
    		  
    		  pasDbExecSqlF("delete from portalplist where plateid in(%s)",caSesid);
    	}
	 }
   
	 // 查询
	 strcpy(sql, "select count(*) from portalplate where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid like '%%%s%%' or platename like '%%%s%%' ", _keyword,_keyword);
	 }
	 if(!utStrIsSpaces(caStyped)){
	 	  int len = 0 ;
	 	  len = strlen(caStyped);
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and substring(groupcode, 9, %lu) = '%s'", len, caStyped);
	 }
	 if(!utStrIsSpaces(caOptd))
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupcode like '%s%c'", caOptd, '%');
	 }
	 if(!utStrIsSpaces(groupcode))  // 商家权限查询
	 {
		 	  char caOpt[64]="";
		 	  sprintf(caOpt,"%s%s",ncmWebAutoDefGet16(psShmHead, iFd,psMsgHead,caStyped),groupcode);
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupcode like '%s%c'", caOpt, '%');
	 }
	 if(!utStrIsSpaces(areaid))  // 区域查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupcode like '%s%c'", areaid, '%');
	 }
	 printf("sql = %s\n", sql);
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select p.plateid, p.shopplateid, p.groupcode, g.dispname, p.platename, p.platepath, p.addtime, p.modtime from portalplate p left join ncsrvgroup g on p.groupcode=g.groupname where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and p.plateid like '%%%s%%' or p.platename like '%%%s%%' ", _keyword,_keyword);
	 }
	 if(!utStrIsSpaces(caStyped)){
	 	  int len = 0 ;
	 	  len = strlen(caStyped);
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and substring(groupcode, 9, %lu) = '%s'", len, caStyped);
	 }
	 if(!utStrIsSpaces(caOptd))
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and p.groupcode like '%s%c'", caOptd, '%');
	 }
	 if(!utStrIsSpaces(groupcode))  // 商家权限查询
	 {
		 	  char caOpt[64]="";
		 	  sprintf(caOpt,"%s%s",ncmWebAutoDefGet16(psShmHead, iFd,psMsgHead,caStyped),groupcode);
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and p.groupcode like '%s%c'", caOpt, '%');
	 }
	 if(!utStrIsSpaces(areaid))  // 区域查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupcode like '%s%c'", areaid, '%');
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by p.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caPlateid)-1,   caPlateid,
	                             UT_TYPE_ULONG,  sizeof(long),          &lShopplateid,
	                             UT_TYPE_STRING, sizeof(caGroupcode)-1, caGroupcode,
	                             UT_TYPE_STRING, sizeof(caDispname)-1,  caDispname,
															 UT_TYPE_STRING, sizeof(caPlatename)-1, caPlatename,
															 UT_TYPE_STRING, sizeof(caPlatepath)-1, caPlatepath,
															 UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
															 UT_TYPE_ULONG,  sizeof(long),          &lModitime);
	  iNum = 0;
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
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

			 utPltPutLoopVar(psDbHead,"plateid",iNum,caPlateid);
			 utPltPutLoopVarF(psDbHead,"shopplateid",iNum,"%lu",lShopplateid);
			 utPltPutLoopVar(psDbHead,"groupcode",iNum,caGroupcode);
			 utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
			 utPltPutLoopVar(psDbHead,"platename",iNum,caPlatename);
			 utPltPutLoopVar(psDbHead,"platepath",iNum,caPlatepath);
			 utPltPutLoopVar(psDbHead,"addtime",iNum,s_addtime);
			 utPltPutLoopVar(psDbHead,"moditime",iNum,s_moditime);
			 
			 memset(caPlateid, 0, sizeof(caPlateid));
			 lShopplateid = 0;
			 memset(caGroupcode, 0, sizeof(caGroupcode));
			 memset(caDispname, 0, sizeof(caDispname));
			 memset(caPlatename, 0, sizeof(caPlatename));
			 memset(caPlatepath, 0, sizeof(caPlatepath));
			 lAddtime = 0;
			 lModitime = 0;
			 memset(s_addtime, 0, sizeof(s_addtime));
			 memset(s_moditime, 0, sizeof(s_moditime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caPlateid)-1,   caPlateid,
			                             UT_TYPE_ULONG,  sizeof(long),          &lShopplateid,
			                             UT_TYPE_STRING, sizeof(caGroupcode)-1, caGroupcode,
			                             UT_TYPE_STRING, sizeof(caDispname)-1,  caDispname,
																	 UT_TYPE_STRING, sizeof(caPlatename)-1, caPlatename,
																	 UT_TYPE_STRING, sizeof(caPlatepath)-1, caPlatepath,
																	 UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lModitime);
			  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_plate_list.htm");
	  
	  return 0;	
}

int ncm_web_Place_plist_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 int i=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 typedef struct plist_stat{
	 		char plateid[16];
	 		char termtype[16];
	 		char lang[16];
	 		unsigned long pageid;
	 		char pagename[128];
	 		char pagepath[128];
	 		unsigned long addtime;
	 		unsigned long moditime;
	 }plist_stat_s;
	 plist_stat_s *stat_temp;
	 
	 // 接收信息变量
	 char plateid[32] = "";
	 char platename[128] = "";
	 char termtype[16] = "";
	 char lang[16] = "";
	 char pageid[20] = "";
	 char pagename[128] = "";
	 char pagepath[128] = "";
	 char addtime[32] = "";
	 
	 // 存放数据库取出的数据的变量
	 char          caPlateid[16] = "";
	 char          caPlatename[32] = "";
	 char          caTermtype[16] = "";
	 char          caLang[16] = "";
	 unsigned long lPageid = 0;
	 char          caPagename[128] = "";
	 char          caPagepath[128] = "";
	 unsigned long lAddtime = 0;
	 unsigned long lModitime = 0;
	 char          s_addtime[32] = "";
	 char          s_moditime[32] = "";
	 unsigned long lMaxid = 0;
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caSesid[4096] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
	 // 关键字
	 char _keyword[33] = "";
	 
	 // 数据表
	 unsigned long lCount1 = 0; // 表中记录的个数
	 unsigned long lCount2 = 0;
	 unsigned long lCount3 = 0;
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 16,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"termtype",    UT_TYPE_STRING,  sizeof(termtype)-1,       termtype,
																	"lang",        UT_TYPE_STRING,  sizeof(lang)-1,           lang,
																	"pageid",      UT_TYPE_STRING,  sizeof(pageid)-1,         pageid,
																	"pagename",    UT_TYPE_STRING,  sizeof(pagename)-1,       pagename,
																	"pagepath",    UT_TYPE_STRING,  sizeof(pagepath)-1,       pagepath,
																	"addtime",     UT_TYPE_STRING,  sizeof(addtime)-1,        addtime,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				 UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",    UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(pagename)>0){
			pasCvtGBK(2,pagename,caTemp,sizeof(pagename));
			strcpy(pagename,caTemp);
   }
   if(strlen(pagepath)>0){
			pasCvtGBK(2,pagepath,caTemp,sizeof(pagename));
			strcpy(pagepath,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid));
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"termtype");
	 }
	 
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
	 	  char cpagepath[128] = "";
	 	  if(strlen(pagepath)>0){
	 	  		strcpy(cpagepath,pasDbReplaceQuote(pagepath));
	 	  }
	 	
	 		memset(caTemp, 0, sizeof(caTemp));
		  sprintf(caTemp, "select count(*) from portalplist where plateid='%s' and termtype='%s' and lang='%s'", plateid, termtype, lang);
			pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, sizeof(long), &lCount1);
			printf("lCount1 = %lu\n", lCount1);
			
			memset(caTemp, 0, sizeof(caTemp));
			sprintf(caTemp, "select count(*) from portalpage where pageid=%lu", atol(pageid));
			pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, sizeof(long), &lCount2);
			printf("lCount2 = %lu\n", lCount2);
			if(lCount1>0){
				  if(lCount2>0){
				  	  unsigned long laddtime = 0;
	 						ncTimeToLong(addtime, &laddtime);
	 						
				  		memset(caTemp, 0, sizeof(caTemp));
	 	  	  		sprintf(caTemp,"update portalplist set pageid=%lu where plateid='%s' and termtype='%s' and lang='%s'", atol(pageid),plateid,termtype,lang);
			 				printf("caTemp = %s\n", caTemp);
					 		pasDbExecSqlF(caTemp);
					 		
					 		memset(caTemp, 0, sizeof(caTemp));
					 		sprintf(caTemp, "update portalpage set pagename='%s', pagepath='%s', addtime=%lu, modtime=%lu where pageid=%lu", pagename, cpagepath, laddtime, time(0), atol(pageid));
					 		printf("caTemp = %s\n", caTemp);
					 		pasDbExecSqlF(caTemp);
				  }else{
				  		memset(caTemp, 0, sizeof(caTemp));
	 	  	  		sprintf(caTemp, "select max(pageid) from portalpage where 1=1");
	 	  	  		pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, sizeof(long), &lMaxid);
	 	  	  		
	 	  	  		memset(caTemp, 0, sizeof(caTemp));
	 	  	  		sprintf(caTemp,"update portalplist set pageid=%lu where plateid='%s' and termtype='%s' and lang='%s'", (lMaxid+1),plateid,termtype,lang);
			 				printf("caTemp = %s\n", caTemp);
					 		pasDbExecSqlF(caTemp);
					 		
					 		memset(caTemp, 0, sizeof(caTemp));
					 		sprintf(caTemp, "insert into portalpage (pageid, pagename, pagepath, addtime, modtime) values (%lu, '%s', '%s', %lu, %lu)", (lMaxid+1), pagename, cpagepath, time(0), time(0));
					 		printf("caTemp = %s\n", caTemp);
					 		pasDbExecSqlF(caTemp);		
				  }
	 	  }else{
	 	  	  if(lCount2>0){
	 	  	  	  unsigned long laddtime = 0;
	 						ncTimeToLong(addtime, &laddtime);
	 		
	 	  	  	  memset(caTemp, 0, sizeof(caTemp));
	 	  	  		sprintf(caTemp,"insert into portalplist (plateid,termtype,lang,pageid) values ('%s','%s','%s',%lu)",plateid,termtype,lang,atol(pageid));
					 		printf("caTemp = %s\n", caTemp);
					 		pasDbExecSqlF(caTemp);
					 		
					 		memset(caTemp, 0, sizeof(caTemp));
					 		sprintf(caTemp, "update portalpage set pagename='%s', pagepath='%s', addtime=%lu, modtime=%lu where pageid=%lu", pagename, cpagepath, laddtime, time(0), atol(pageid));
					 		printf("caTemp = %s\n", caTemp);
					 		pasDbExecSqlF(caTemp);
	 	  	  }else{
	 	  	  		memset(caTemp, 0, sizeof(caTemp));
	 	  	  		sprintf(caTemp, "select max(pageid) from portalpage where 1=1");
	 	  	  		pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, sizeof(long), &lMaxid);	
	 	  	  		
	 	  	  		memset(caTemp, 0, sizeof(caTemp));
	 	  	  		sprintf(caTemp,"insert into portalplist (plateid,termtype,lang,pageid) values ('%s','%s','%s',%lu)",plateid,termtype,lang,(lMaxid+1));
					 		printf("caTemp = %s\n", caTemp);
					 		pasDbExecSqlF(caTemp);
					 		
					 		memset(caTemp, 0, sizeof(caTemp));
					 		sprintf(caTemp, "insert into portalpage (pageid, pagename, pagepath, addtime, modtime) values (%lu, '%s', '%s', %lu, %lu)", (lMaxid+1), pagename, cpagepath, time(0), time(0));
					 		printf("caTemp = %s\n", caTemp);
					 		pasDbExecSqlF(caTemp);
	 	  	  }
	 	  }
	 }
	 
	 stat_temp = (plist_stat_s *)malloc(sizeof(plist_stat_s)*12+1);
	 strcpy(stat_temp[0].termtype, "pad");
	 strcpy(stat_temp[0].lang, "eng");
	 strcpy(stat_temp[1].termtype, "pad");
	 strcpy(stat_temp[1].lang, "zh");
	 strcpy(stat_temp[2].termtype, "pad");
	 strcpy(stat_temp[2].lang, "default");
	 strcpy(stat_temp[3].termtype, "mobile");
	 strcpy(stat_temp[3].lang, "eng");
	 strcpy(stat_temp[4].termtype, "mobile");
	 strcpy(stat_temp[4].lang, "zh");
	 strcpy(stat_temp[5].termtype, "mobile");
	 strcpy(stat_temp[5].lang, "default");
	 strcpy(stat_temp[6].termtype, "mspc");
	 strcpy(stat_temp[6].lang, "eng");
	 strcpy(stat_temp[7].termtype, "mspc");
	 strcpy(stat_temp[7].lang, "zh");
	 strcpy(stat_temp[8].termtype, "mspc");
	 strcpy(stat_temp[8].lang, "default");
	 strcpy(stat_temp[9].termtype, "default");
	 strcpy(stat_temp[9].lang, "eng");
	 strcpy(stat_temp[10].termtype, "default");
	 strcpy(stat_temp[10].lang, "zh");
	 strcpy(stat_temp[11].termtype, "default");
	 strcpy(stat_temp[11].lang, "default");
	 for(i=0; i<12; i++){
	 			strcpy(stat_temp[i].plateid, plateid);
	 			stat_temp[i].pageid = 0;
	 			strcpy(stat_temp[i].pagename, "");
	 			strcpy(stat_temp[i].pagepath, "");
	 			stat_temp[i].addtime = 0;
	 			stat_temp[i].moditime = 0;
	 }
   
	 // 查询
	 strcpy(sql, "select l.plateid, l.termtype, l.lang, l.pageid, g.pagename, g.pagepath, g.addtime, g.modtime from portalplist l left join portalpage g on l.pageid=g.pageid where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(plateid)) // 根据模板查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and l.plateid = '%s'", plateid);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by l.%s %s", sort, dir);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caPlateid)-1,   caPlateid,
															 UT_TYPE_STRING, sizeof(caTermtype)-1,  caTermtype,
															 UT_TYPE_STRING, sizeof(caLang)-1,      caLang,
															 UT_TYPE_ULONG,  sizeof(long),          &lPageid,
															 UT_TYPE_STRING, sizeof(caPagename)-1,  caPagename,
															 UT_TYPE_STRING, sizeof(caPagepath)-1,  caPagepath,
															 UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
															 UT_TYPE_ULONG,  sizeof(long),          &lModitime);
	
	  while(0 == iret || 1405 == iret){
			 for(i=0; i<12; i++){
			 		if(strcmp(stat_temp[i].termtype, caTermtype)==0 && strcmp(stat_temp[i].lang, caLang)==0){
			 				strcpy(stat_temp[i].plateid, caPlateid);
			 				strcpy(stat_temp[i].termtype, caTermtype);
			 				strcpy(stat_temp[i].lang, caLang);
			 				stat_temp[i].pageid = lPageid;
			 				strcpy(stat_temp[i].pagename, caPagename);
			 				strcpy(stat_temp[i].pagepath, caPagepath);
			 				stat_temp[i].addtime = lAddtime;
			 				stat_temp[i].moditime = lModitime;		
			 		}	
			 }
			 
			 memset(caPlateid, 0, sizeof(caPlateid));
			 memset(caPlatename, 0, sizeof(caPlatename));
			 memset(caTermtype, 0, sizeof(caTermtype));
			 memset(caLang, 0, sizeof(caLang));
			 lPageid = 0;
			 memset(caPagename, 0, sizeof(caPagename));
			 memset(caPagepath, 0, sizeof(caPagepath));
			 lAddtime = 0;
			 lModitime = 0;
			 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caPlateid)-1,   caPlateid,
																	 UT_TYPE_STRING, sizeof(caTermtype)-1,  caTermtype,
																	 UT_TYPE_STRING, sizeof(caLang)-1,      caLang,
																	 UT_TYPE_ULONG,  sizeof(long),          &lPageid,
																	 UT_TYPE_STRING, sizeof(caPagename)-1,  caPagename,
																	 UT_TYPE_STRING, sizeof(caPagepath)-1,  caPagepath,
																	 UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lModitime);
	  }
	  
	  for(i=0; i<12; i++){
				iNum++;
				if(iNum > 1)
				{
				 		utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
				}
				
				if(stat_temp[i].addtime>0){
						strcpy(s_addtime, utTimFormat("%Y/%m/%d %H:%M:%S", stat_temp[i].addtime));	
			  }else{
			  		strcpy(s_addtime, "");
			  }
			  if(stat_temp[i].moditime>0){
			 		  strcpy(s_moditime, utTimFormat("%Y/%m/%d %H:%M:%S", stat_temp[i].moditime));	
			  }else{
			 		  strcpy(s_moditime, "");
			  }
			 
				utPltPutLoopVar(psDbHead,"plateid",iNum,stat_temp[i].plateid);
				utPltPutLoopVar(psDbHead,"termtype",iNum,stat_temp[i].termtype);
				utPltPutLoopVar(psDbHead,"lang",iNum,stat_temp[i].lang);
				utPltPutLoopVarF(psDbHead,"pageid",iNum,"%lu",stat_temp[i].pageid);
				utPltPutLoopVar(psDbHead,"pagename",iNum,stat_temp[i].pagename);
				utPltPutLoopVar(psDbHead,"pagepath",iNum,stat_temp[i].pagepath);	
				utPltPutLoopVar(psDbHead,"addtime",iNum,s_addtime);
				utPltPutLoopVar(psDbHead,"moditime",iNum,s_moditime);
				memset(s_addtime, 0, sizeof(s_addtime));
			  memset(s_moditime, 0, sizeof(s_moditime));
	  }
	  
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_plist_list.htm");
	  
	  return 0;	
}

/*int ncm_web_Place_page_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char pageid[32] = "";
	 char pagename[128] = "";
	 char pagepath[128] = "";
	 char addtime[32] = "";
	 char moditime[32] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long lPageid = 0;
	 char          caPagename[128] = "";
	 char          caPagepath[128] = "";
	 unsigned long lAddtime = 0;
	 unsigned long lModitime = 0;
	 char          s_addtime[32] = "";
	 char          s_moditime[32] = "";
	 unsigned long lMaxid = 0;
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caSesid[1024] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
	 // 关键字
	 char _keyword[33] = "";
	 
	 // 数据表
	 unsigned long lCount1 = 0; // 表中记录的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 14,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"pageid",      UT_TYPE_STRING,  sizeof(pageid)-1,         pageid,
																	"pagename",    UT_TYPE_STRING,  sizeof(pagename)-1,       pagename,
																	"pagepath",    UT_TYPE_STRING,  sizeof(pagepath)-1,       pagepath,
																	"addtime",     UT_TYPE_STRING,  sizeof(addtime)-1,        addtime,
																	"moditime",    UT_TYPE_STRING,  sizeof(moditime)-1,       moditime,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				 UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",    UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(pagename)>0){
			pasCvtGBK(2,pagename,caTemp,128);
			strcpy(pagename,caTemp);
   }
   if(strlen(pagepath)>0){
			pasCvtGBK(2,pagepath,caTemp,128);
			strcpy(pagepath,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"addtime");
	 }
	 
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
	 		if (atol(pageid)>0){
	 			  unsigned long laddtime = 0;
	 			  ncTimeToLong(addtime, &laddtime);
	 				sprintf(caTemp,"update portalpage set pagename='%s', pagepath='%s', addtime=%lu, modtime=%lu where pageid=%lu ", pagename,pagepath,laddtime,time(0),atol(pageid));
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  } else {
	 	  	  pasDbOneRecord("select max(pageid) from portalpage", 0, UT_TYPE_ULONG, 4, &lMaxid);
		    	sprintf(caTemp,"insert into portalpage (pageid,pagename,pagepath,addtime,modtime) values (%lu,'%s','%s',%lu,%lu)",(lMaxid+1),pagename,pagepath,time(0),time(0));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from portalpage where pageid in(%s)",caSesid);
    	}
	 }
   
	 // 查询
	 strcpy(sql, "select count(*) from portalpage where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and pagename like '%%%s%%'", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select pageid, pagename, pagepath, addtime, modtime from portalpage where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and pagename like '%%%s%%' ", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lPageid,
															 UT_TYPE_STRING, sizeof(caPagename)-1,  caPagename,
															 UT_TYPE_STRING, sizeof(caPagepath)-1,  caPagepath,
															 UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
															 UT_TYPE_ULONG,  sizeof(long),          &lModitime);
	
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
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

			 utPltPutLoopVarF(psDbHead,"pageid",iNum,"%lu",lPageid);
			 utPltPutLoopVar(psDbHead,"pagename",iNum,caPagename);
			 utPltPutLoopVar(psDbHead,"pagepath",iNum,caPagepath);
			 utPltPutLoopVarF(psDbHead,"addtime",iNum,s_addtime);
			 utPltPutLoopVar(psDbHead,"moditime",iNum,s_moditime);
			 
			 lPageid = 0;
			 memset(caPagename, 0, sizeof(caPagename));
			 memset(caPagepath, 0, sizeof(caPagepath));
			 lAddtime = 0;
			 lModitime = 0;
			 memset(s_addtime, 0, sizeof(s_addtime));
			 memset(s_moditime, 0, sizeof(s_moditime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lPageid,
																	 UT_TYPE_STRING, sizeof(caPagename)-1,  caPagename,
																	 UT_TYPE_STRING, sizeof(caPagepath)-1,  caPagepath,
																	 UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lModitime);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_page_list.htm");
	  
	  return 0;	
}*/

int ncm_web_Place_prule_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char ruleid[32] = "";
	 char priority[32] = "";
	 char plateid[32] = "";
	 char groupid[32] = "";
	 char ssid[32] = "";
	 char nasid[255] = "";
	 char mark[255] = "";
	 char dotime[255] = "";
	 char dateid[32] = "";
	 char starttime[32] = "";
	 char endtime[32] = "";
	 char addtime[32] = "";
	 char moditime[32] = "";
	 char status[32] = "";
	 char groupcode[32] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long lRuleid = 0;
	 unsigned long lPriority = 0;
	 char          caPlateid[32] = "";
	 char          caPlatename[32] = "";
	 unsigned long lGroupid = 0;
	 char          caDispname[32] = "";
	 char          caSsid[32] = "";
	 char          caNasid[255] = "";
	 char          caMark[255] = "";
	 char          caDotime[255] = "";
	 unsigned long lDateid = 0;
	 unsigned long lStarttime = 0;
	 unsigned long lEndtime = 0;
	 unsigned long lAddtime = 0;
	 unsigned long lModitime = 0;
	 unsigned long lStatus = 0;
	 char          caGroupcode[32] = "";
	 char          s_starttime[32] = "";
	 char          s_endtime[32] = "";
	 char          s_addtime[32] = "";
	 char          s_moditime[32] = "";
	 char          s_dateid[255] = "";
	 char          s_status[32] = "";
	 unsigned long lMaxid = 0;
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caSesid[1024] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
	 // 关键字
	 char _keyword[33] = "";
	 
	 // 数据表
	 unsigned long lCount1 = 0; // 表中记录的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 24,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
	 																"ruleid",      UT_TYPE_STRING,  sizeof(ruleid)-1,         ruleid,
	 																"priority",    UT_TYPE_STRING,  sizeof(priority)-1,       priority,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"groupid",     UT_TYPE_STRING,  sizeof(groupid)-1,        groupid,
																	"nasid",       UT_TYPE_STRING,  sizeof(nasid)-1,          nasid,
																	"ssid",        UT_TYPE_STRING,  sizeof(ssid)-1,           ssid,
																	"mark",        UT_TYPE_STRING,  sizeof(mark)-1,           mark,
																	"dateid",      UT_TYPE_STRING,  sizeof(dateid)-1,         dateid,
																	"starttime",   UT_TYPE_STRING,  sizeof(starttime)-1,      starttime,
																	"dotime",      UT_TYPE_STRING,  sizeof(dotime)-1,         dotime,
																	"endtime",     UT_TYPE_STRING,  sizeof(endtime)-1,        endtime,
																	"addtime",     UT_TYPE_STRING,  sizeof(addtime)-1,        addtime,
																	"moditime",    UT_TYPE_STRING,  sizeof(moditime)-1,       moditime,
																	"status",      UT_TYPE_STRING,  sizeof(status)-1,         status,
																	"groupcode",   UT_TYPE_STRING,  sizeof(groupcode)-1,      groupcode,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  sizeof(caSesid)-1,        caSesid,
																	"del",				 UT_TYPE_STRING,  sizeof(caDel)-1,          caDel,
																	"update",      UT_TYPE_STRING,  sizeof(caUpdate)-1,       caUpdate,
																	"caExport",    UT_TYPE_STRING,  sizeof(caExport)-1,       caExport);
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(ssid)>0){
			pasCvtGBK(2,ssid,caTemp,32);
			strcpy(ssid,caTemp);
   }
   if(strlen(nasid)>0){
			pasCvtGBK(2,nasid,caTemp,255);
			strcpy(nasid,caTemp);
   }
   if(strlen(dotime)>0){
			pasCvtGBK(2,dotime,caTemp,255);
			strcpy(dotime,caTemp);
   }
   if(strlen(mark)>0){
			pasCvtGBK(2,mark,caTemp,255);
			strcpy(mark,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid));
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"addtime");
	 }
	 
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
	 	  unsigned long lstarttime = 0;
  	  unsigned long lendtime = 0;
		  ncTimeToLong(starttime, &lstarttime);
		  ncTimeToLong(endtime, &lendtime);
		  //printf("nasid = %s\n",nasid);
		  char cnasid[255]="";
		  if(strlen(nasid)>0){
		  	  char tnasid[255]="";
				  strrpl(tnasid, nasid, "%3F", "?");
				  strrpl(cnasid, tnasid, "%2C", ",");
				  //printf("cnasid = %s\n",cnasid);
		  }
	 		if (atol(ruleid)>0){
	 			  unsigned long laddtime = 0;
	 			  ncTimeToLong(addtime, &laddtime);
	 				sprintf(caTemp,"update portalprule set priority=%lu, plateid='%s', groupid=%lu, ssid='%s', nasid='%s', mark='%s', dotime='%s', dateid=%lu, starttime=%lu, endtime=%lu, addtime=%lu, modtime=%lu, status=%lu, groupcode='%s' where ruleid=%lu ", atol(priority),plateid,atol(groupid),ssid,cnasid,mark,dotime,atol(dateid),lstarttime,lendtime,laddtime,time(0),atol(status),groupcode,atol(ruleid));
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  } else {
	 	  	  pasDbOneRecord("select max(ruleid) from portalprule", 0, UT_TYPE_ULONG, 4, &lMaxid);
		    	sprintf(caTemp,"insert into portalprule (ruleid,priority,plateid,groupid,ssid,nasid,mark,dotime,dateid,starttime,endtime,addtime,modtime,status,groupcode) values (%lu,%lu,'%s',%lu,'%s','%s','%s','%s',%lu,%lu,%lu,%lu,%lu,%lu,'%s')",(lMaxid+1),atol(priority),plateid,atol(groupid),ssid,cnasid,mark,dotime,atol(dateid),lstarttime,lendtime,time(0),time(0),atol(status),groupcode);
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from portalprule where ruleid in(%s)",caSesid);
    	}
	 }
   
	 // 查询
	 strcpy(sql, "select count(*) from portalprule where 1=1");
	 if(!utStrIsSpaces(plateid)) // 根据模板查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = '%s'", plateid);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select p.ruleid, p.priority, p.plateid, pl.platename, p.groupid, g.dispname, p.ssid, p.nasid, p.mark, p.dotime, p.dateid,p.starttime, p.endtime, p.addtime, p.modtime, p.status, p.groupcode from portalprule p left join portalplate pl on p.plateid=pl.plateid left join ncsrvgroup g on p.groupcode=g.groupname where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(plateid)) // 根据模板查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and p.plateid = '%s'", plateid);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by p.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lRuleid,
	 														 UT_TYPE_ULONG,  sizeof(long),          &lPriority,
	 														 UT_TYPE_STRING, sizeof(caPlateid)-1,   caPlateid,
	 														 UT_TYPE_STRING, sizeof(caPlatename)-1, caPlatename,
	                             UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
	                             UT_TYPE_STRING, sizeof(caDispname)-1,  caDispname,
															 UT_TYPE_STRING, sizeof(caSsid)-1,      caSsid,
															 UT_TYPE_STRING, sizeof(caNasid)-1,     caNasid,
															 UT_TYPE_STRING, sizeof(caMark)-1,      caMark,
															 UT_TYPE_STRING, sizeof(caDotime)-1,    caDotime,
															 UT_TYPE_ULONG,  sizeof(long),          &lDateid,
															 UT_TYPE_ULONG,  sizeof(long),          &lStarttime,
															 UT_TYPE_ULONG,  sizeof(long),          &lEndtime,
															 UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
															 UT_TYPE_ULONG,  sizeof(long),          &lModitime,
															 UT_TYPE_ULONG,  sizeof(long),          &lStatus,
															 UT_TYPE_STRING, sizeof(caGroupcode)-1, caGroupcode);
	
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 if(lStarttime>0){
			 		strcpy(s_starttime, utTimFormat("%Y/%m/%d", lStarttime));	
			 }else{
			 		strcpy(s_starttime, "");
			 }
			 if(lEndtime>0){
			 		strcpy(s_endtime, utTimFormat("%Y/%m/%d", lEndtime));	
			 }else{
			 		strcpy(s_endtime, "");
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
			 
			 switch(lDateid){
			 		case 0:strcpy(s_dateid, "每天");break;
			 		case 1:strcpy(s_dateid, "其他");break;
			 		default:break;	
			 }
			 
			 switch(lStatus){
			 		case 0:strcpy(s_status, "停用");break;
			 		case 1:strcpy(s_status, "启用");break;
			 		default:break;	
			 }

			 utPltPutLoopVarF(psDbHead,"ruleid",iNum,"%lu",lRuleid);
			 utPltPutLoopVarF(psDbHead,"priority",iNum,"%lu",lPriority);
			 utPltPutLoopVar(psDbHead,"plateid",iNum,caPlateid);
			 utPltPutLoopVar(psDbHead,"platename",iNum,caPlatename);
			 utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
			 utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
			 utPltPutLoopVar(psDbHead,"ssid",iNum,caSsid);
			 utPltPutLoopVar(psDbHead,"nasid",iNum,caNasid);
			 utPltPutLoopVar(psDbHead,"mark",iNum,caMark);
			 utPltPutLoopVar(psDbHead,"dotime",iNum,caDotime);
			 utPltPutLoopVarF(psDbHead,"dateid",iNum,"%lu",lDateid);
			 utPltPutLoopVarF(psDbHead,"datename",iNum,s_dateid);
			 utPltPutLoopVarF(psDbHead,"starttime",iNum,s_starttime);
			 utPltPutLoopVarF(psDbHead,"endtime",iNum,s_endtime);
			 utPltPutLoopVarF(psDbHead,"addtime",iNum,s_addtime);
			 utPltPutLoopVar(psDbHead,"moditime",iNum,s_moditime);
			 utPltPutLoopVarF(psDbHead,"status",iNum,"%lu",lStatus);
			 utPltPutLoopVar(psDbHead,"s_status",iNum,s_status);
			 utPltPutLoopVar(psDbHead,"groupcode",iNum,caGroupcode);
			 
			 lRuleid = 0;
			 lPriority = 0;
			 memset(caPlateid, 0, sizeof(caPlateid));
			 memset(caPlatename, 0, sizeof(caPlatename));
			 lGroupid = 0;
			 memset(caDispname, 0, sizeof(caDispname));
			 memset(caSsid, 0, sizeof(caSsid));
			 memset(caNasid, 0, sizeof(caNasid));
			 memset(caMark, 0, sizeof(caMark));
			 memset(caDotime, 0, sizeof(caDotime));
			 lDateid = 0;
			 memset(s_dateid, 0, sizeof(s_dateid));
			 lStarttime = 0;
			 lEndtime = 0;
			 lAddtime = 0;
			 lModitime = 0;
			 lStatus = 0;
			 memset(caGroupcode, 0, sizeof(caGroupcode));
			 memset(s_starttime, 0, sizeof(s_starttime));
			 memset(s_endtime, 0, sizeof(s_endtime));
			 memset(s_addtime, 0, sizeof(s_addtime));
			 memset(s_moditime, 0, sizeof(s_moditime));
			 memset(s_status, 0, sizeof(s_status));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lRuleid,
			 														 UT_TYPE_ULONG,  sizeof(long),          &lPriority,
			 														 UT_TYPE_STRING, sizeof(caPlateid)-1,   caPlateid,
			 														 UT_TYPE_STRING, sizeof(caPlatename)-1, caPlatename,
			                             UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
			                             UT_TYPE_STRING, sizeof(caDispname)-1,  caDispname,
																	 UT_TYPE_STRING, sizeof(caSsid)-1,      caSsid,
																	 UT_TYPE_STRING, sizeof(caNasid)-1,     caNasid,
																	 UT_TYPE_STRING, sizeof(caMark)-1,      caMark,
																	 UT_TYPE_STRING, sizeof(caDotime)-1,    caDotime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lDateid,
																	 UT_TYPE_ULONG,  sizeof(long),          &lStarttime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lEndtime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lModitime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lStatus,
																	 UT_TYPE_STRING, sizeof(caGroupcode)-1, caGroupcode);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_webplacelimit/ncm_web_Place_prule_list.htm");
	  
	  return 0;	
}

int Ncm_WebPlaceSWW_SetFunName(utShmHead *psShmHead) // 各户端函数
{
		pasSetTcpFunName("ncm_web_Place_smslimit_list", ncm_web_Place_smslimit_list, 0); // 场所端短信数量限制-0313
		pasSetTcpFunName("ncm_web_Place_placelimit_list", ncm_web_Place_placelimit_list, 0); // 场所端短信数量限制-0313 
		pasSetTcpFunName("ncm_web_Place_timelimit_list", ncm_web_Place_timelimit_list, 0); // 场所端上网时间限制-0313
		pasSetTcpFunName("ncm_web_Place_smsformat_list", ncm_web_Place_smsformat_list, 0); // 场所端上网时间限制-0313
    
    pasSetTcpFunName("ncm_web_Place_os_chart", ncm_web_Place_os_chart, 0); // 场所端操作系统图表-0306
    pasSetTcpFunName("ncm_web_Place_usernum_chart", ncm_web_Place_usernum_chart, 0); // 场所端用户注册人数图表-0306
    
    pasSetTcpFunName("ncm_web_Place_white_list", ncm_web_Place_white_list, 0); // 黑白名单-0604
    
    pasSetTcpFunName("ncm_web_Place_plate_list", ncm_web_Place_plate_list, 0); // 模板-0728
    pasSetTcpFunName("ncm_web_Place_plist_list", ncm_web_Place_plist_list, 0);
    //pasSetTcpFunName("ncm_web_Place_page_list", ncm_web_Place_page_list, 0);
    pasSetTcpFunName("ncm_web_Place_prule_list", ncm_web_Place_prule_list, 0);
    
    return 0;
}


