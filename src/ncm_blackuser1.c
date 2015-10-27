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

int ncTimeToLong(char *stime, unsigned long *lTime);
/*{
	 char ctime[20] = "";
	 
	 snprintf(ctime, sizeof(ctime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", stime[0], stime[1], stime[2], stime[3], stime[5], stime[6], stime[8], stime[9], stime[11], stime[12], stime[14], stime[15], stime[17], stime[18]);	
	 *lTime = utTimStrToLong("%Y%m%d%H%M%S", ctime);
	 
	 return 0;
}*/

int ncm_blackuser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char utype[32] = "";
	 char name[32] = "";
	 char memo[64] = "";
	 char validtime[64] = "";
	 char systime[64] = "";
	 char unit[64] = "";
	 char insert[16] = "";
	 char caInsert[16] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char caPreid[32] = "";
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
	 unsigned long iValidTime = 0;
	 unsigned long lSysTime = 0;
	 
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
	 utMsgGetSomeNVar(psMsgHead, 17,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"utype",       UT_TYPE_STRING,  sizeof(utype)-1,          utype,
			"name",        UT_TYPE_STRING,  sizeof(name)-1,       		name,
			"memo",        UT_TYPE_STRING,  sizeof(memo)-1,           memo,
			"validtime",   UT_TYPE_STRING,  sizeof(validtime)-1,      validtime,
			"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
			"unit",        UT_TYPE_STRING,  sizeof(unit)-1,           unit,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"insert",      UT_TYPE_STRING,  10,                       caInsert,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"preid",       UT_TYPE_STRING,  sizeof(caPreid)-1,        caPreid,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 
	 trim(_keyword);
	 printf("utype = %s\n", utype);
	 printf("name = %s\n", name);
	 printf("memo = %s\n", memo);
	 printf("validtime = %s\n", validtime);
	 printf("unit = %s\n", unit);
	 printf("insert = %s\n", caInsert);
	 printf("del = %s\n", caDel);
	 printf("caUpdate = %s\n", caUpdate);
	 printf("caPreid = %s\n", caPreid);
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
   if(strlen(utype)>0){
			pasCvtGBK(2,utype,caTemp,30);
			strcpy(utype,caTemp);
   }
	 if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,30);
			strcpy(name,caTemp);
   }
   if(strlen(memo)>0){
			pasCvtGBK(2,memo,caTemp,30);
			strcpy(memo,caTemp);
   }
   if(strlen(unit)>0){
			pasCvtGBK(2,unit,caTemp,30);
			strcpy(unit,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   if(strlen(caPreid)>0){
			pasCvtGBK(2,caPreid,caTemp,30);
			strcpy(caPreid,caTemp);
   }
   if(strlen(sort)>0){
			pasCvtGBK(2,sort,caTemp,30);
			strcpy(sort,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"name");
	 }
	 
	 // 转换类别
	 if(strcmp(utype, "Username")==0){
	 		strcpy(utype, "1");	
	 }else if(strcmp(utype, "Mac")==0){
	 	  strcpy(utype, "2");
	 }
	 
	 // 转换时间单位
	 if(strcmp(unit, "天")==0){
	 	  strcpy(unit, "1");	
	 }else if(strcmp(unit, "小时")==0){
	 	  strcpy(unit, "2");	
	 }else if(strcmp(unit, "分")==0){
	 	  strcpy(unit, "3");	
	 }else if(strcmp(unit, "秒")==0){
	 	  strcpy(unit, "4");	
	 }
	 
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
			if(strlen(caPreid)>0){
				  // 时间转换
					ncTimeToLong(systime, &lSysTime);
					switch(atoi(unit)){
						case 1:	iValidTime = atoi(validtime)*24*3600 + lSysTime;break;
						case 2: iValidTime = atoi(validtime)*3600 + lSysTime;break;
						case 3: iValidTime = atoi(validtime)*60 + lSysTime;break;
						case 4: iValidTime = atoi(validtime) + lSysTime;break;
						default:iValidTime = atoi(validtime) + lSysTime;break;
					}
	 				sprintf(caTemp,"update ncsrvblackuser set utype=%lu,name='%s',memo='%s',validtime=%lu where name='%s' ",atol(utype),name,memo,iValidTime,caPreid);
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  } else {
					switch(atoi(unit)){
						case 1:	iValidTime = atoi(validtime)*24*3600 + time(0);break;
						case 2: iValidTime = atoi(validtime)*3600 + time(0);break;
						case 3: iValidTime = atoi(validtime)*60 + time(0);break;
						case 4: iValidTime = atoi(validtime) + time(0);break;
						default:iValidTime = atoi(validtime) + time(0);break;
					}
		    	sprintf(caTemp,"insert into ncsrvblackuser (utype,name,memo,validtime,systime) values (%lu,'%s','%s',%lu,%lu)",atol(utype),name,memo,iValidTime,time(0));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    			printf("caSesid = %s\n", caSesid);
    		  pasDbExecSqlF("delete from ncsrvblackuser where name in(%s)",caSesid);
    	}
	 }	 
	 // 查询
	 strcpy(sql, "select count(*) from ncsrvblackuser where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and name like '%%%s%%' ", _keyword);
	 }
	 printf("sql =%s \n",sql);
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d  lCount1 = %d\n", lTotRec,lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select utype, name, memo, validtime, systime from ncsrvblackuser where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and name like '%%%s%%' ", _keyword);
	 }
	 
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL) {
				unsigned long	utype = 0;
				char 					name[32] = "";
				char 					memo[64] = "";
				unsigned long validtime = 0;
				unsigned long systime = 0;
				char          unit[32] = "";
				unsigned long l_validtime = 0;
				char          s_utype[32] = "";
				char          s_systime[32] = "";
				
				// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
			  while(0 == (iret = pasDbFetchInto(psCur,
			     UT_TYPE_ULONG,  4,               		&utype,
					 UT_TYPE_STRING, sizeof(name)-1,      name,
					 UT_TYPE_STRING, sizeof(memo)-1,      memo,
					 UT_TYPE_ULONG,  4,               		&validtime,
					 UT_TYPE_ULONG,  4,               		&systime)) || 1405 == iret)
			  {
					 iNum++;
					 if(iNum > 1)
					 {
						 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
					 }
					 
					 switch(utype){
					 		case 1:strcpy(s_utype, "Username");break;
					 		case 2:strcpy(s_utype, "Mac");break;
					 		default:break;
					 }
				   
				   memset(unit, 0, sizeof(unit));
				   if((validtime-systime)%60 == 0){
				   		if((validtime-systime)%3600 == 0){
				   				if((validtime-systime)%86400 == 0){
				   						l_validtime = (validtime-systime)/86400;
				   						strcpy(unit, "天");
				   				}else{
				   						l_validtime = (validtime-systime)/3600;
				   						strcpy(unit, "小时");	
				   				}
				   		}else{
				   				l_validtime = (validtime-systime)/60;
				   				strcpy(unit, "分");	
				   		}
				   }else{
				   		l_validtime = (validtime-systime);
				   		strcpy(unit, "秒");	
				   }
				   
				   strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", systime));
				   if(strcmp(s_systime, "1970/01/01 07:59:59") == 0){
				     strcpy(s_systime, "");	
				   }
				  
					 utPltPutLoopVar(psDbHead,"utype",iNum,s_utype);
					 utPltPutLoopVar(psDbHead,"name",iNum,name);
					 utPltPutLoopVar(psDbHead,"memo",iNum,memo);
					 utPltPutLoopVarF(psDbHead,"validtime",iNum,"%lu",l_validtime);
					 utPltPutLoopVar(psDbHead,"unit",iNum,unit);
					 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
					 
					 memset(s_systime, 0, sizeof(s_systime));
					 memset(s_utype, 0, sizeof(s_utype));
			  }
		}
		utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmgroup/ncm_blackuser_list.htm");
	  return 0;	
}