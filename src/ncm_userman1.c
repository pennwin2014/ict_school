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

#define FILEPATH "/home/ncmysql/ncm/html/ncm_userman"

static int ncsWebReplace00(char *pHead,char *pVar,short nType,long lLen,char *pValue);
int ncmWebReplace(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);

int ncTimeToLong(char *stime, unsigned long *lTime); // 时间字符串转换成长整形常量
/*{
	 char ctime[20] = "";
	 
	 snprintf(ctime, sizeof(ctime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", stime[0], stime[1], stime[2], stime[3], stime[5], stime[6], stime[8], stime[9], stime[11], stime[12], stime[14], stime[15], stime[17], stime[18]);	
	 *lTime = utTimStrToLong("%Y%m%d%H%M%S", ctime);
	 
	 return 0;
}*/

int ncDaysToLong(unsigned long lTime, int days, unsigned long *lStartTime)  // 给定days天数，计算距离当前时间days天的开始时间，并转换成常量
{
		char sdate[33] = "";
   	strcpy(sdate, utTimFormat("%Y-%m-%d", lTime));//当前日期
   	strcat(sdate, " 00:00:00");
   	*lStartTime = utTimStrToLong("%Y/%m/%d %H:%M:%S", sdate) - (days-1) * 86400;
   	
   	return 0;	
}

int ncNumToTimeE(unsigned long lTime, char *cTime) // 整形常量转换成时间值
{   
		int ss = 1, mi = 0, hh = 0,dd = 0;
		int day = 0, hour = 0, minute = 0, second = 0;
    char caDay[11] = ""; 
    char caHour[11] = "";
    char caMin[11] = "";
    char caSec[11] = "";
    
    mi = ss*60;
    hh = mi*60;
    dd = hh*24;
    day = lTime/dd;
    hour = (lTime - day*dd)/hh;
    minute = (lTime - day*dd - hour*hh)/mi;
    second = (lTime - day*dd - hour*hh - minute*mi)/ss;
  	if(day<10){sprintf(caDay, "0%d", day);}else{sprintf(caDay, "%d", day);}
    if(hour<10){sprintf(caHour, "0%d:", hour);}else{sprintf(caHour, "%d:", hour);}
    if(minute<10){sprintf(caMin, "0%d:", minute);}else{sprintf(caMin, "%d:", minute);}
    if(second<10){sprintf(caSec, "0%d", second);}else{sprintf(caSec, "%d", second);}
		
    strcat(cTime, caDay);
    strcat(cTime, " ");
    strcat(cTime, caHour);
    strcat(cTime, caMin);
    strcat(cTime, caSec); 
     
    return 0;  
}

static char *convert(const char *codefrom, const char *codeto, const char *src)
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

// 时间转化格式
char **ncsUtlGetTable2(char *sdate, char *edate, char *prefix, unsigned long *lStartTime, unsigned long *lEndTime, int *lCount);
/*{
   	char stime[15] = "";
   	char etime[15] = "";
   	
   	snprintf(stime, sizeof(stime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", sdate[0], sdate[1], sdate[2], sdate[3], sdate[5], sdate[6], sdate[8], sdate[9]);
   	snprintf(etime, sizeof(etime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", edate[0], edate[1], edate[2], edate[3], edate[5], edate[6], edate[8], edate[9]);
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
}*/

char **ncsUtlGetTable(unsigned long lTime, int days, char *prefix, unsigned long *lStartTime, int *lCount);
/*{
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
}*/

// 管理中心门店信息
int ncm_web_MC_unitinfo_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 strcpy(sql, "select userid, username from ncmuser where 1=1");
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL) {
				unsigned long	userid = 0;
				char 					username[32] = "";
				
				// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
			  while(0 == (iret = pasDbFetchInto(psCur,
			     UT_TYPE_ULONG,  4,               		&userid,
			     UT_TYPE_STRING, sizeof(username)-1,  username)) || 1405 == iret)
			  {
					 iNum++;
					 if(iNum > 1)
					 {
						 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
					 }
					 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",userid);
					 utPltPutLoopVar(psDbHead,"text",iNum,username);
			  }
		}
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_unitinfo_list.htm");
	  return 0;	
}

// 管理中心组信息
int ncm_web_MC_groupinfo_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 strcpy(sql, "select groupid, groupname from ncmgroup where 1=1");
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL) {
				unsigned long	groupid = 0;
				char 					groupname[32] = "";
				
				// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
			  while(0 == (iret = pasDbFetchInto(psCur,
			     UT_TYPE_ULONG,  4,               		&groupid,
			     UT_TYPE_STRING, sizeof(groupname)-1,  groupname)) || 1405 == iret)
			  {
					 iNum++;
					 if(iNum > 1)
					 {
						 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
					 }
					 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",groupid);
					 utPltPutLoopVar(psDbHead,"name",iNum,groupname);
			  }
		}
		utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_groupinfo_list.htm");
	  return 0;	
}

// 管理中心黑名单
int ncm_web_MC_blackuser_1(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char utype[32] = "";
	 char did[32] = "";
	 //char dname[32] = "";
	 char gid[32] = "";
	 //char gname[32] = "";
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
	 utMsgGetSomeNVar(psMsgHead, 21,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"utype",       UT_TYPE_STRING,  sizeof(utype)-1,          utype,
			"did",         UT_TYPE_STRING,  sizeof(did)-1,            did,
			//"dname",       UT_TYPE_STRING,  sizeof(dname)-1,          dname,
			"gid",         UT_TYPE_STRING,  sizeof(gid)-1,            gid,
			//"gname",       UT_TYPE_STRING,  sizeof(gname)-1,          gname,
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
	 printf("did = %s\n", did);
	 //printf("dname = %s\n", dname);
	 printf("gid = %s\n", gid);
	 //printf("gname = %s\n", gname);
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
   /*if(strlen(dname)>0){
			pasCvtGBK(2,dname,caTemp,30);
			strcpy(dname,caTemp);
   }
   if(strlen(gname)>0){
			pasCvtGBK(2,gname,caTemp,30);
			strcpy(gname,caTemp);
   }*/
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
	 if(strcmp(unit, "d")==0){
	 	  strcpy(unit, "1");	
	 }else if(strcmp(unit, "h")==0){
	 	  strcpy(unit, "2");	
	 }else if(strcmp(unit, "m")==0){
	 	  strcpy(unit, "3");	
	 }else if(strcmp(unit, "s")==0){
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
	 				sprintf(caTemp,"update ncsrvblackuser set utype=%lu,did=%lu,gid=%lu,name='%s',memo='%s',validtime=%lu,systime=%lu where name='%s' ",atol(utype),atol(did),atol(gid),name,memo,iValidTime,lSysTime,caPreid);
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
		    	sprintf(caTemp,"insert into ncsrvblackuser (utype,did,gid,name,memo,validtime,systime) values (%lu,%lu,%lu,'%s','%s',%lu,%lu)",atol(utype),atol(did),atol(gid),name,memo,iValidTime,time(0));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 	  memset(utype, 0, sizeof(utype));
	 	  memset(did, 0, sizeof(did));
	 	  memset(gid, 0, sizeof(gid));
	 	  memset(name, 0, sizeof(name));
	 	  memset(memo, 0, sizeof(memo));
	 	  memset(validtime, 0, sizeof(validtime));
	 	  memset(systime, 0, sizeof(systime));
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
	 if(!utStrIsSpaces(did) && atol(did)>0) // 根据门店
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", atol(did));
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select bu.utype, bu.did, u.username as dname, bu.gid, g.groupname as gname, bu.name, bu.memo, bu.validtime, bu.systime from ncsrvblackuser bu left join ncmuser u on bu.did = u.userid left join ncmgroup g on bu.gid = g.groupid where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and bu.name like '%%%s%%' ", _keyword);
	 }
	 if(!utStrIsSpaces(did) && atol(did)>0) // 根据门店
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and bu.did = %lu", atol(did));
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by bu.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL) {
				unsigned long	utype = 0;
				unsigned long	did = 0;
				char          dname[32] = "";
				unsigned long	gid = 0;
				char          gname[32] = "";
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
			     UT_TYPE_ULONG,  4,               		&did,
			     UT_TYPE_STRING, sizeof(dname)-1,     dname,
			     UT_TYPE_ULONG,  4,               		&gid,
			     UT_TYPE_STRING, sizeof(gname)-1,     gname,
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
				   						strcpy(unit, "d");
				   				}else{
				   						l_validtime = (validtime-systime)/3600;
				   						strcpy(unit, "h");
				   				}
				   		}else{
				   				l_validtime = (validtime-systime)/60;
				   				strcpy(unit, "m");
				   		}
				   }else{
				   		l_validtime = (validtime-systime);
				   		strcpy(unit, "s");	
				   }
				   
				   if(systime>0){
				   		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", systime));
				   }else{
				   		strcpy(s_systime, "");	
				   }
				  
					 utPltPutLoopVar(psDbHead,"utype",iNum,s_utype);
					 utPltPutLoopVarF(psDbHead,"did",iNum,"%lu",did);
					 utPltPutLoopVar(psDbHead,"dname",iNum,dname);
					 utPltPutLoopVarF(psDbHead,"gid",iNum,"%lu",gid);
					 utPltPutLoopVar(psDbHead,"gname",iNum,gname);
					 utPltPutLoopVar(psDbHead,"name",iNum,name);
					 utPltPutLoopVar(psDbHead,"memo",iNum,memo);
					 utPltPutLoopVarF(psDbHead,"validtime",iNum,"%lu",l_validtime);
					 utPltPutLoopVar(psDbHead,"unit",iNum,unit);
					 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
					 
					 memset(s_systime, 0, sizeof(s_systime));
					 memset(s_utype, 0, sizeof(s_utype));
			  }
	 }
	 //utPltShowDb(psDbHead);
	 pasDbCloseCursor(psCur);
	  
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_blackuser_list.htm");
	 return 0;	
}

// 管理中心短信注册
/*int ncm_web_MC_smslog_1(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 int i = 0;
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 char *tmpgbk = NULL;
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 按时间查询变量
	 char time_flag[2] = ""; // 选择的时间标志：
	 int  iTime_flag = 0; // 存放转换后的时间atoi(time_flag)
   int  days = 0; // 存放转换后的时间标志
	 unsigned long lStartTime = 0;  // 开始时间：是根据当前时间 - 选项得到的天数
	 unsigned long lTime = time(0); // 当前时间
	 int lCount = 0; // 根据时间算表的个数
	 char sdate[32] = ""; // 输入的开始时间
   char edate[32] = ""; // 输入的结束时间
   char _sdate[32] = ""; // 格式转换后的开始时间
	 char _edate[32] = ""; // 格式转换后的当前时间
	 
	 char sid[20] = "";
	 char did[20] = "";
	 char gid[20] = "";
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
	 char **ptmp = NULL;
	 struct sttable
	 {
			char tname[32]; // 表名
			unsigned long lCount; // 表中记录的个数
	 };
	 struct sttable tableinfo[120]; // 表的个数
	 unsigned long lCount1 = 0; // 表中记录的个数
	 int num = 0; // 已经有的表的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 unsigned long llogTime = 0;
	 int _break = 0;
	 
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
	 utMsgGetSomeNVar(psMsgHead, 25,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
	    "time_flag",   UT_TYPE_STRING,  sizeof(time_flag)-1,      time_flag,
	    "sdate",       UT_TYPE_STRING,  sizeof(sdate)-1,          sdate,
	    "edate",       UT_TYPE_STRING,  sizeof(edate)-1,          edate,
			"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
			"did",         UT_TYPE_STRING,  sizeof(did)-1,            did,
			"gid",         UT_TYPE_STRING,  sizeof(gid)-1,            gid,
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
	 printf("did = %s\n", did);
	 printf("gid = %s\n", gid);
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
	 
	 // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
	 
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
	 
	 iTime_flag = atoi(time_flag);  // atoi 将字符串转换成长整型数据 和atol一样
	 switch(iTime_flag)
	 {
	   	case 1://今天
	   		days = 1;
	   		break;
	   	case 2://最近三天
	   		days = 3;
	   		break;
	   	case 3://最近一周
	   		days = 7;
	   		break;
	   	case 4://最近一月
	   		days = 30;
	   		break;
	   	case 5://最近三月
	   		days = 90;
	   		break;
	   	case 6://最近半年
	   		days = 180;
	   		break;
	   	default:
	   		days = 1;
	  		break;
	 }
	 
	 // 时间字符串的格式转化
	 if(strlen(sdate) > 0 && strlen(edate) > 0){
		  ptmp = ncsUtlGetTable2(sdate, edate, "ncsrvsmslog_if_", &lStartTime, &lTime, &lCount);
	 }
	 else{
		  ptmp = ncsUtlGetTable(lTime, days, "ncsrvsmslog_if_",  &lStartTime, &lCount);
		  lTime=lTime+2*3600;
	 }
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lStartTime));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lTime));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 printf("lCount = %d\n", lCount);
	 for(i=0; i<lCount; i++){
      printf("ptmp = %s\n", ptmp[i]);
      lCount1 = 0;
			memset(sql, 0, sizeof(sql));
			snprintf(sql, sizeof(sql), "select count(*) from %s where logtime >= %lu and logtime <= %lu ",  ptmp[i], lStartTime, lTime);
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
   		
   		// pasDbOneRecord 单记录查询。　0---正确; !=0---错误代码, 具体含义请参见 数据库的有关资料
			iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
			if(lCount1 > 0 && iReturn!=1146)
			{
					strcpy(tableinfo[num].tname, ptmp[i]);
					tableinfo[num].lCount = lCount1;
			   	num++;
			}
			free(ptmp[i]);
   }
   free(ptmp);	
	 for(i = 0; i < num; i++){
			lTotRec += tableinfo[i].lCount;
	 }
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 // 打印表名和表中的数据
	 for(i = num-1; i >= 0; i--){
			printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
	 }
   
	 iNum = 0;
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 for(i=num-1; i>=0; i--){
	 		memset(sql, 0, sizeof(sql));
			if(tableinfo[i].lCount <= iStart)
			{	
				iStart -= tableinfo[i].lCount;
				continue;
			}
			snprintf(sql, sizeof(sql), "select sid, did, gid, userid, username, usermac, ip, logtime, sptype, areacode, areaname, status from %s where logtime >= %lu and logtime <= %lu", tableinfo[i].tname, lStartTime, lTime);
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
		  snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);
			if(tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit)
			{
				iLimit = iLimit -  (tableinfo[i].lCount-iStart);
				iStart = 0;
			}
			else 
			{
				_break = 1;
			}
			tmpgbk = convert("UTF-8", "GBK", sql);
			if(tmpgbk == NULL)
				continue;
			printf("__line = %d, sql = %s\n", __LINE__, tmpgbk);
			
			psCur = pasDbOpenSql(tmpgbk, 0);
			if(psCur != NULL) {
				unsigned long	sid = 0;
				unsigned long	did = 0;
				unsigned long	gid = 0;
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
			     UT_TYPE_ULONG,  4,               		&did,
			     UT_TYPE_ULONG,  4,               		&gid,
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
					 utPltPutLoopVarF(psDbHead,"did",iNum,"%lu",did);
					 utPltPutLoopVarF(psDbHead,"gid",iNum,"%lu",gid);
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
			utPltShowDb(psDbHead);
		  pasDbCloseCursor(psCur);
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_smslog_list.htm");
	 return 0;	
}*/

// 管理中心短信注册
int ncm_web_MC_smslog_1(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 int i = 0;
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 char *tmpgbk = NULL;
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 按时间查询变量
	 char time_flag[2] = ""; // 选择的时间标志：
	 int  iTime_flag = 0; // 存放转换后的时间atoi(time_flag)
   int  days = 0; // 存放转换后的时间标志
	 unsigned long lStartTime = 0;  // 开始时间：是根据当前时间 - 选项得到的天数
	 unsigned long lTime = time(0); // 当前时间
	 char sdate[32] = ""; // 输入的开始时间
   char edate[32] = ""; // 输入的结束时间
   char _sdate[32] = ""; // 格式转换后的开始时间
	 char _edate[32] = ""; // 格式转换后的当前时间
	 
	 char sid[20] = "";
	 char servicecode[16] = "";
	 char did[20] = "";
	 char gid[20] = "";
	 char userid[16] = "";
	 char username[32] = "";
	 char usermac[18] = "";
	 char ip[16] = "";
	 char logtime[32] = "";
	 char smsname[32] = "";
	 char sptype[16] = "";
	 char areacode[16] = "";
	 char areaname[32] = "";
	 char contrycode[16] = "";
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
	 utMsgGetSomeNVar(psMsgHead, 29,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
	    "time_flag",   UT_TYPE_STRING,  sizeof(time_flag)-1,      time_flag,
	    "sdate",       UT_TYPE_STRING,  sizeof(sdate)-1,          sdate,
	    "edate",       UT_TYPE_STRING,  sizeof(edate)-1,          edate,
			"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
			"servicecode", UT_TYPE_STRING,  sizeof(servicecode)-1,    servicecode,
			"did",         UT_TYPE_STRING,  sizeof(did)-1,            did,
			"gid",         UT_TYPE_STRING,  sizeof(gid)-1,            gid,
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
	 printf("did = %s\n", did);
	 printf("gid = %s\n", gid);
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
	 
	 // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
	 
	 iTime_flag = atoi(time_flag);  // atoi 将字符串转换成长整型数据 和atol一样
	 switch(iTime_flag)
	 {
	   	case 1://今天
	   		days = 1;
	   		break;
	   	case 2://最近三天
	   		days = 3;
	   		break;
	   	case 3://最近一周
	   		days = 7;
	   		break;
	   	case 4://最近一月
	   		days = 30;
	   		break;
	   	case 5://最近三月
	   		days = 90;
	   		break;
	   	case 6://最近半年
	   		days = 180;
	   		break;
	   	default:
	   		days = 1;
	  		break;
	 }
	 
	 // 时间字符串的格式转化
	 if(strlen(sdate) > 0 && strlen(edate) > 0){
		  ncTimeToLong(sdate, &lStartTime);
	 		ncTimeToLong(edate, &lTime);
	 }else{
		  ncDaysToLong(lTime, days, &lStartTime);
		  lTime=lTime+2*3600;
	 }
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lStartTime));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lTime));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 snprintf(sql, sizeof(sql), "select count(*) from ncsrvsmslog where logtime >= %lu and logtime <= %lu ", lStartTime, lTime);
 	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%' or usermac like '%%%s%%') ",  _keyword, _keyword);
	 }
	 if(!utStrIsSpaces(did) && atol(did)>0)  // 门店
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", atol(did));
	 }
	 if(!utStrIsSpaces(username))  // 用户名
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
	 if(!utStrIsSpaces(sptype))  // 运营商类别
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sptype like '%%%s%%'", sptype);
	 }
	 if(!utStrIsSpaces(areacode))  // 区号
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and areacode like '%%%s%%'", areacode);
	 }
   		
   // pasDbOneRecord 单记录查询。　0---正确; !=0---错误代码, 具体含义请参见 数据库的有关资料
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
   printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 snprintf(sql, sizeof(sql), "select sl.sid, sl.servicecode, sl.did, u.username as dname, sl.gid, g.groupname as gname, sl.userid, sl.username, sl.usermac, sl.ip, sl.logtime, sl.smsname, sl.sptype, sl.areacode, sl.areaname, sl.contrycode, sl.status from ncsrvsmslog sl left join ncmuser u on sl.did = u.userid left join ncmgroup g on sl.gid = g.groupid where logtime >= %lu and logtime <= %lu", lStartTime, lTime);
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (sl.username like '%%%s%%' or sl.usermac like '%%%s%%') ", _keyword, _keyword);
	 }
	 if(!utStrIsSpaces(did) && atol(did)>0)  // 门店
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sl.did = %lu", atol(did));
	 }
	 if(!utStrIsSpaces(username))  // 用户名
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sl.username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(usermac))  // MAC
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sl.usermac like '%%%s%%'", usermac);
	 }
	 if(!utStrIsSpaces(ip))  // IP
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sl.ip like '%%%s%%'", ip);
	 }
	 if(!utStrIsSpaces(sptype))  // 运营商类别
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sl.sptype = %d", atol(sptype));
	 }
	 if(!utStrIsSpaces(areacode))  // 区号
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sl.areacode like '%%%d%%'", atol(areacode));
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by sl.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL) {
			unsigned long	sid = 0;
			char 					servicecode[16] = "";
			unsigned long	did = 0;
			char 					dname[32] = "";
			unsigned long	gid = 0;
			char 					gname[32] = "";
			unsigned long	userid = 0;
			char 					username[32] = "";
			char 					usermac[18] = "";
			char 					ip[16] = "";
			unsigned long logtime = 0;
			char 					smsname[32] = "";
			unsigned long sptype = 0;
			unsigned long areacode = 0;
			char 					areaname[32] = "";
			unsigned long contrycode = 0;
			unsigned long status = 0;
			char          s_logtime[32] = "";
			char          s_status[32] = "";
			char          caSptype[32] = "";
			
			// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
		  while(0 == (iret = pasDbFetchInto(psCur,
		     UT_TYPE_ULONG,  4,               		&sid,
		     UT_TYPE_STRING, sizeof(servicecode)-1,servicecode,
		     UT_TYPE_ULONG,  4,               		&did,
		     UT_TYPE_STRING, sizeof(dname)-1,     dname,
		     UT_TYPE_ULONG,  4,               		&gid,
		     UT_TYPE_STRING, sizeof(gname)-1,     gname,
		     UT_TYPE_ULONG,  4,               		&userid,
				 UT_TYPE_STRING, sizeof(username)-1,  username,
				 UT_TYPE_STRING, sizeof(usermac)-1,   usermac,
				 UT_TYPE_STRING, sizeof(ip)-1,        ip,
				 UT_TYPE_ULONG,  4,               		&logtime,
				 UT_TYPE_STRING, sizeof(smsname)-1,   smsname,
				 UT_TYPE_ULONG,  4,               		&sptype,
				 UT_TYPE_ULONG,  4,               		&areacode,
				 UT_TYPE_STRING, sizeof(areaname)-1,  areaname,
				 UT_TYPE_ULONG,  4,               		&contrycode,
				 UT_TYPE_ULONG,  4,               		&status)) || 1405 == iret)
		  {
				 iNum++;
				 if(iNum > 1)
				 {
					 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
				 }
			   if(logtime>0){
			   		strcpy(s_logtime, utTimFormat("%Y/%m/%d %H:%M:%S", logtime));
			   }else{
			   		strcpy(s_logtime, "");	
			   }
			   switch(status){
			   		case 0:strcpy(s_status, "failure");break;
			   		case 1:strcpy(s_status, "success");break;
			   		default:break;
			   }
			   switch(sptype){
		  				case 0:strcpy(caSptype, "Local");break;
		  				case 1:strcpy(caSptype, "CM");break;
		  				case 2:strcpy(caSptype, "CU");break;	
		  				case 3:strcpy(caSptype, "CT");break;
		  				case 8:strcpy(caSptype, "Other");break;
		  				case 9:strcpy(caSptype, "Inter");break;
		  				default:break;
		  	 }
			  
				 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
				 utPltPutLoopVarF(psDbHead,"did",iNum,"%lu",did);
				 utPltPutLoopVarF(psDbHead,"gid",iNum,"%lu",gid);
				 utPltPutLoopVarF(psDbHead,"userid",iNum,"%lu",userid);
				 utPltPutLoopVarF(psDbHead,"logtime",iNum,s_logtime);
				 utPltPutLoopVar(psDbHead,"sptype",iNum,caSptype);
				 utPltPutLoopVarF(psDbHead,"areacode",iNum,"%lu",areacode);
				 utPltPutLoopVarF(psDbHead,"contrycode",iNum,"%lu",contrycode);
				 utPltPutLoopVar(psDbHead,"status",iNum,s_status);
				 utPltSetCvtHtml(1);
				 utPltPutLoopVar(psDbHead,"servicecode",iNum,servicecode);
				 utPltPutLoopVar(psDbHead,"dname",iNum,dname);
				 utPltPutLoopVar(psDbHead,"gname",iNum,gname);
				 utPltPutLoopVar(psDbHead,"username",iNum,username);
				 utPltPutLoopVar(psDbHead,"usermac",iNum,usermac);
				 utPltPutLoopVar(psDbHead,"ip",iNum,ip);
				 utPltPutLoopVar(psDbHead,"smsname",iNum,smsname);
				 utPltPutLoopVar(psDbHead,"areaname",iNum,areaname);
				 utPltSetCvtHtml(0);
		  }
	 }
	 //utPltShowDb(psDbHead);
	 pasDbCloseCursor(psCur);
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_smslog_list.htm");
	 return 0;	
}

// 管理中心用户日志
/*int ncm_web_MC_userlog_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 int i = 0;
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 char *tmpgbk = NULL;
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 按时间查询变量
	 char time_flag[2] = ""; // 选择的时间标志：
	 int  iTime_flag = 0; // 存放转换后的时间atoi(time_flag)
   int  days = 0; // 存放转换后的时间标志
	 unsigned long lStartTime = 0;  // 开始时间：是根据当前时间 - 选项得到的天数
	 unsigned long lTime = time(0); // 当前时间
	 int lCount = 0; // 根据时间算表的个数
	 char sdate[32] = ""; // 输入的开始时间
   char edate[32] = ""; // 输入的结束时间
   char _sdate[32] = ""; // 格式转换后的开始时间
	 char _edate[32] = ""; // 格式转换后的当前时间
	 
	 char sid[20] = "";
	 char portalname[16] = "";
	 char dname[32] = "";
	 char gid[16] = "";
	 char userid[16] = "";
	 char username[32] = "";
	 char dispname[32] = "";
	 char ssid[32] = "";
	 char apname[32] = "";
	 char mac[18] = "";
	 char ip[16] = "";
	 char dev[24] = "";
	 char os[16] = "";
	 char devtype[8] = "";
	 char ubytes[32] = "";
	 char dbytes[32] = "";
	 char starttime[32] = "";
	 char endtime[32] = "";
	 char conntime[32] = "";
	 char cause[32] = "";
	 
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
	 char **ptmp = NULL;
	 struct sttable
	 {
			char tname[32]; // 表名
			unsigned long lCount; // 表中记录的个数
	 };
	 struct sttable tableinfo[120]; // 表的个数
	 unsigned long lCount1 = 0; // 表中记录的个数
	 int num = 0; // 已经有的表的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 unsigned long llogTime = 0;
	 int _break = 0;
	 
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
	 utMsgGetSomeNVar(psMsgHead, 33,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
	    "time_flag",   UT_TYPE_STRING,  sizeof(time_flag)-1,      time_flag,
	    "sdate",       UT_TYPE_STRING,  sizeof(sdate)-1,          sdate,
	    "edate",       UT_TYPE_STRING,  sizeof(edate)-1,          edate,
			"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
			"portalname",  UT_TYPE_STRING,  sizeof(portalname)-1,     portalname,
			"dname",       UT_TYPE_STRING,  sizeof(dname)-1,          dname,
			"gid",         UT_TYPE_STRING,  sizeof(gid)-1,            gid,
			"userid",      UT_TYPE_STRING,  sizeof(userid)-1,         userid,
			"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
			"dispname",    UT_TYPE_STRING,  sizeof(dispname)-1,       dispname,
			"ssid",    		 UT_TYPE_STRING,  sizeof(ssid)-1,           ssid,
			"apname",      UT_TYPE_STRING,  sizeof(apname)-1,         apname,
			"mac",     		 UT_TYPE_STRING,  sizeof(mac)-1,            mac,
			"ip",          UT_TYPE_STRING,  sizeof(ip)-1,             ip,
			"dev",         UT_TYPE_STRING,  sizeof(dev)-1,            dev,
			"os",          UT_TYPE_STRING,  sizeof(os)-1,             os,
			"devtype",     UT_TYPE_STRING,  sizeof(devtype)-1,        devtype,
			"ubytes",      UT_TYPE_STRING,  sizeof(ubytes)-1,         ubytes,
			"dbytes",      UT_TYPE_STRING,  sizeof(dbytes)-1,         dbytes,
			"starttime",   UT_TYPE_STRING,  sizeof(starttime)-1,      starttime,
			"endtime",     UT_TYPE_STRING,  sizeof(endtime)-1,        endtime,
			"conntime",    UT_TYPE_STRING,  sizeof(conntime)-1,       conntime,
			"cause",       UT_TYPE_STRING,  sizeof(cause)-1,          cause,
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
	 printf("dname = %s\n", dname);
	 printf("gid = %s\n", gid);
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
	 
	 // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(portalname)>0){
			pasCvtGBK(2,portalname,caTemp,30);
			strcpy(portalname,caTemp);
   }
	 if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,30);
			strcpy(username,caTemp);
   }
   if(strlen(dispname)>0){
			pasCvtGBK(2,dispname,caTemp,30);
			strcpy(dispname,caTemp);
   }
   if(strlen(ssid)>0){
			pasCvtGBK(2,ssid,caTemp,30);
			strcpy(ssid,caTemp);
   }
   if(strlen(apname)>0){
			pasCvtGBK(2,apname,caTemp,30);
			strcpy(apname,caTemp);
   }
   if(strlen(mac)>0){
			pasCvtGBK(2,mac,caTemp,30);
			strcpy(mac,caTemp);
   }
   if(strlen(ip)>0){
			pasCvtGBK(2,ip,caTemp,30);
			strcpy(ip,caTemp);
   }
   if(strlen(dev)>0){
			pasCvtGBK(2,dev,caTemp,30);
			strcpy(dev,caTemp);
   }
   if(strlen(os)>0){
			pasCvtGBK(2,os,caTemp,30);
			strcpy(os,caTemp);
   }
   if(strlen(devtype)>0){
			pasCvtGBK(2,devtype,caTemp,30);
			strcpy(devtype,caTemp);
   }
   if(strlen(conntime)>0){
			pasCvtGBK(2,conntime,caTemp,30);
			strcpy(conntime,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
	 
	 iTime_flag = atoi(time_flag);  // atoi 将字符串转换成长整型数据 和atol一样
	 switch(iTime_flag)
	 {
	   	case 1://今天
	   		days = 1;
	   		break;
	   	case 2://最近三天
	   		days = 3;
	   		break;
	   	case 3://最近一周
	   		days = 7;
	   		break;
	   	case 4://最近一月
	   		days = 30;
	   		break;
	   	case 5://最近三月
	   		days = 90;
	   		break;
	   	case 6://最近半年
	   		days = 180;
	   		break;
	   	default:
	   		days = 1;
	  		break;
	 }
	 
	 // 时间字符串的格式转化
	 if(strlen(sdate) > 0 && strlen(edate) > 0){
		  ptmp = ncsUtlGetTable2(sdate, edate, "ncsrvuserlog_if_", &lStartTime, &lTime, &lCount);
	 }
	 else{
		  ptmp = ncsUtlGetTable(lTime, days, "ncsrvuserlog_if_",  &lStartTime, &lCount);
		  lTime=lTime+2*3600;
	 }
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lStartTime));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lTime));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 printf("lCount = %d\n", lCount);
	 for(i=0; i<lCount; i++){
      printf("ptmp = %s\n", ptmp[i]);
      lCount1 = 0;
			memset(sql, 0, sizeof(sql));
			snprintf(sql, sizeof(sql), "select count(*) from %s where starttime >= %lu and endtime <= %lu ",  ptmp[i], lStartTime, lTime);
   		if(!utStrIsSpaces(_keyword)) // 根据关键字查询
		  {
				 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (portalname like '%%%s%%' or username like '%%%s%%') ",  _keyword, _keyword);
		  }
		  if(!utStrIsSpaces(portalname))  // Portal Name
		  {
				 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and portalname like '%%%s%%'", portalname);
		  }
		  if(!utStrIsSpaces(dname))  // did
		  {
				 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did like '%%%s%%'", dname);
		  }
		  if(!utStrIsSpaces(username))  // 用户名
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
		  if(!utStrIsSpaces(devtype))  // 设备类型
		  {
				 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and devtype like '%%%s%%'", devtype);
		  }
   		
   		// pasDbOneRecord 单记录查询。　0---正确; !=0---错误代码, 具体含义请参见 数据库的有关资料
			iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
			if(lCount1 > 0 && iReturn!=1146)
			{
					strcpy(tableinfo[num].tname, ptmp[i]);
					tableinfo[num].lCount = lCount1;
			   	num++;
			}
			free(ptmp[i]);
   }
   free(ptmp);	
	 for(i = 0; i < num; i++){
			lTotRec += tableinfo[i].lCount;
	 }
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 // 打印表名和表中的数据
	 for(i = num-1; i >= 0; i--){
			printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
	 }
   
	 iNum = 0;
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 for(i=num-1; i>=0; i--){
	 		memset(sql, 0, sizeof(sql));
			if(tableinfo[i].lCount <= iStart)
			{	
				iStart -= tableinfo[i].lCount;
				continue;
			}
			snprintf(sql, sizeof(sql), "select ul.sid, ul.portalname, u.username as dname, ul.gid, ul.userid, ul.username, ul.dispname, ul.ssid, ul.apname, ul.mac, ul.ip, ul.dev, ul.os, ul.devtype, ul.ubytes, ul.dbytes, ul.starttime, ul.endtime, ul.conntime, ul.cause from %s ul left join ncmuser u on ul.did = u.userid where starttime >= %lu and endtime <= %lu", tableinfo[i].tname, lStartTime, lTime);
			// utStrIsSpaces: 判断字符串是否为空。
		  if(!utStrIsSpaces(_keyword)) // 根据关键字查询
		  {
				 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (ul.portalname like '%%%s%%' or ul.username like '%%%s%%') ", _keyword, _keyword);
		  }
		  if(!utStrIsSpaces(portalname))  // Portal Name
		  {
				 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ul.portalname like '%%%s%%'", portalname);
		  }
		  if(!utStrIsSpaces(dname))  // did
		  {
				 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ul.did like '%%%s%%'", dname);
		  }
		  if(!utStrIsSpaces(username))  // 用户名
		  {
				 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ul.username like '%%%s%%'", username);
		  }
		  if(!utStrIsSpaces(mac))  // mac
		  {
				 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ul.mac like '%%%s%%'", mac);
		  }
		  if(!utStrIsSpaces(ip))  // ip
		  {
				 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ul.ip like '%%%s%%'", ip);
		  }
		  if(!utStrIsSpaces(devtype))  // 设备类型
		  {
				 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ul.devtype like '%%%s%%'", devtype);
		  }
		  snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by ul.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);
			if(tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit)
			{
				iLimit = iLimit -  (tableinfo[i].lCount-iStart);
				iStart = 0;
			}
			else 
			{
				_break = 1;
			}
			tmpgbk = convert("UTF-8", "GBK", sql);
			if(tmpgbk == NULL)
				continue;
			printf("__line = %d, sql = %s\n", __LINE__, tmpgbk);
			
			psCur = pasDbOpenSql(tmpgbk, 0);
			if(psCur != NULL) {
				unsigned long	sid = 0;
				char 					dname[32] = "";
				unsigned long	gid = 0;
				char 					portalname[16] = "";
				unsigned long	userid = 0;
				char 					username[32] = "";
				char 					dispname[32] = "";
				char 					ssid[32] = "";
				char 					apname[32] = "";
				char 					mac[18] = "";
				char 					ip[16] = "";
				char 					dev[24] = "";
				char 					os[16] = "";
				char 					devtype[8] = "";
				unsigned long ubytes = 0;
				unsigned long dbytes = 0;
				unsigned long starttime = 0;
				unsigned long endtime = 0;
				unsigned long conntime = 0;
				unsigned long cause = 0;
				char          s_starttime[32] = "";
				char          s_endtime[32] = "";
				char          s_conntime[32] = "";
				
				// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
			  while(0 == (iret = pasDbFetchInto(psCur,
			     UT_TYPE_ULONG,  4,               		&sid,
			     UT_TYPE_STRING, sizeof(portalname)-1,portalname,
			     UT_TYPE_STRING, sizeof(dname)-1,     dname,
			     UT_TYPE_ULONG,  4,               		&gid,
			     UT_TYPE_ULONG,  4,               		&userid,
					 UT_TYPE_STRING, sizeof(username)-1,  username,
					 UT_TYPE_STRING, sizeof(dispname)-1,  dispname,
					 UT_TYPE_STRING, sizeof(ssid)-1,      ssid,
					 UT_TYPE_STRING, sizeof(apname)-1,    apname,
					 UT_TYPE_STRING, sizeof(mac)-1,       mac,
					 UT_TYPE_STRING, sizeof(ip)-1,        ip,
					 UT_TYPE_STRING, sizeof(dev)-1,       dev,
					 UT_TYPE_STRING, sizeof(os)-1,        os,
					 UT_TYPE_STRING, sizeof(devtype)-1,   devtype,
					 UT_TYPE_ULONG,  4,               		&ubytes,
					 UT_TYPE_ULONG,  4,               		&dbytes,
					 UT_TYPE_ULONG,  4,               		&starttime,
					 UT_TYPE_ULONG,  4,               		&endtime,
					 UT_TYPE_ULONG,  4,               		&conntime,
					 UT_TYPE_ULONG,  4,               		&cause)) || 1405 == iret)
			  {
					 iNum++;
					 if(iNum > 1)
					 {
						 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
					 }
				  
				   strcpy(s_starttime, utTimFormat("%Y/%m/%d %H:%M:%S", starttime));
				   if(strcmp(s_starttime, "1970/01/01 07:59:59") == 0){
				     strcpy(s_starttime, "");	
				   }
				   strcpy(s_endtime, utTimFormat("%Y/%m/%d %H:%M:%S", endtime));
				   if(strcmp(s_endtime, "1970/01/01 07:59:59") == 0){
				     strcpy(s_endtime, "");	
				   }
				   ncNumToTimeE(conntime, s_conntime);
				  
					 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
					 utPltPutLoopVar(psDbHead,"portalname",iNum,portalname);
					 utPltPutLoopVar(psDbHead,"dname",iNum,dname);
					 utPltPutLoopVarF(psDbHead,"gid",iNum,"%lu",gid);
					 utPltPutLoopVarF(psDbHead,"userid",iNum,"%lu",userid);
					 utPltPutLoopVar(psDbHead,"username",iNum,username);
					 utPltPutLoopVar(psDbHead,"dispname",iNum,dispname);
					 utPltPutLoopVar(psDbHead,"ssid",iNum,ssid);
					 utPltPutLoopVar(psDbHead,"apname",iNum,apname);
					 utPltPutLoopVar(psDbHead,"mac",iNum,mac);
					 utPltPutLoopVar(psDbHead,"ip",iNum,ip);
					 utPltPutLoopVar(psDbHead,"dev",iNum,dev);
					 utPltPutLoopVar(psDbHead,"os",iNum,os);
					 utPltPutLoopVar(psDbHead,"devtype",iNum,devtype);
					 utPltPutLoopVarF(psDbHead,"ubytes",iNum,"%lu",ubytes);
					 utPltPutLoopVarF(psDbHead,"dbytes",iNum,"%lu",dbytes);
					 utPltPutLoopVarF(psDbHead,"starttime",iNum,s_starttime);
					 utPltPutLoopVarF(psDbHead,"endtime",iNum,s_endtime);
					 utPltPutLoopVarF(psDbHead,"conntime",iNum,s_conntime);
					 utPltPutLoopVarF(psDbHead,"cause",iNum,"%lu",cause);
					 
					 memset(s_starttime, 0, sizeof(s_starttime));
					 memset(s_endtime, 0, sizeof(s_endtime));
					 memset(s_conntime, 0, sizeof(s_conntime));
			  }
			}
			utPltShowDb(psDbHead);
		  pasDbCloseCursor(psCur);
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_userlog_list.htm");
	 return 0;	
}*/

// 管理中心用户日志
int ncm_web_MC_userlog_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 int i = 0;
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 char *tmpgbk = NULL;
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 按时间查询变量
	 char time_flag[2] = ""; // 选择的时间标志：
	 int  iTime_flag = 0; // 存放转换后的时间atoi(time_flag)
   int  days = 0; // 存放转换后的时间标志
	 unsigned long lStartTime = 0;  // 开始时间：是根据当前时间 - 选项得到的天数
	 unsigned long lTime = time(0); // 当前时间
	 char sdate[32] = ""; // 输入的开始时间
   char edate[32] = ""; // 输入的结束时间
   char _sdate[32] = ""; // 格式转换后的开始时间
	 char _edate[32] = ""; // 格式转换后的当前时间
	 
	 char sid[20] = "";
	 char portalname[16] = "";
	 char did[20] = "";
	 char gid[16] = "";
	 char userid[16] = "";
	 char username[32] = "";
	 char dispname[32] = "";
	 char ssid[32] = "";
	 char apname[32] = "";
	 char mac[18] = "";
	 char ip[16] = "";
	 char dev[24] = "";
	 char os[16] = "";
	 char devtype[8] = "";
	 char ubytes[32] = "";
	 char dbytes[32] = "";
	 char starttime[32] = "";
	 char endtime[32] = "";
	 char conntime[32] = "";
	 char cause[32] = "";
	 
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
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
   
   // 导出
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 接收chart值
	 char flag[32] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 34,
	 	"flag",        UT_TYPE_STRING,  sizeof(flag)-1,           flag,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
	    "time_flag",   UT_TYPE_STRING,  sizeof(time_flag)-1,      time_flag,
	    "sdate",       UT_TYPE_STRING,  sizeof(sdate)-1,          sdate,
	    "edate",       UT_TYPE_STRING,  sizeof(edate)-1,          edate,
			"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
			"portalname",  UT_TYPE_STRING,  sizeof(portalname)-1,     portalname,
			"did",         UT_TYPE_STRING,  sizeof(did)-1,            did,
			"gid",         UT_TYPE_STRING,  sizeof(gid)-1,            gid,
			"userid",      UT_TYPE_STRING,  sizeof(userid)-1,         userid,
			"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
			"dispname",    UT_TYPE_STRING,  sizeof(dispname)-1,       dispname,
			"ssid",    		 UT_TYPE_STRING,  sizeof(ssid)-1,           ssid,
			"apname",      UT_TYPE_STRING,  sizeof(apname)-1,         apname,
			"mac",     		 UT_TYPE_STRING,  sizeof(mac)-1,            mac,
			"ip",          UT_TYPE_STRING,  sizeof(ip)-1,             ip,
			"dev",         UT_TYPE_STRING,  sizeof(dev)-1,            dev,
			"os",          UT_TYPE_STRING,  sizeof(os)-1,             os,
			"devtype",     UT_TYPE_STRING,  sizeof(devtype)-1,        devtype,
			"ubytes",      UT_TYPE_STRING,  sizeof(ubytes)-1,         ubytes,
			"dbytes",      UT_TYPE_STRING,  sizeof(dbytes)-1,         dbytes,
			"starttime",   UT_TYPE_STRING,  sizeof(starttime)-1,      starttime,
			"endtime",     UT_TYPE_STRING,  sizeof(endtime)-1,        endtime,
			"conntime",    UT_TYPE_STRING,  sizeof(conntime)-1,       conntime,
			"cause",       UT_TYPE_STRING,  sizeof(cause)-1,          cause,
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
	 printf("flag = %s\n",flag);
	 printf("devtype = %s\n",devtype);
	 printf("sid = %s\n", sid);
	 printf("did = %s\n", did);
	 printf("gid = %s\n", gid);
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
	 
	 // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
	 
	 iTime_flag = atoi(time_flag);  // atoi 将字符串转换成长整型数据 和atol一样
	 switch(iTime_flag)
	 {
	   	case 1://今天
	   		days = 1;
	   		break;
	   	case 2://最近三天
	   		days = 3;
	   		break;
	   	case 3://最近一周
	   		days = 7;
	   		break;
	   	case 4://最近一月
	   		days = 30;
	   		break;
	   	case 5://最近三月
	   		days = 90;
	   		break;
	   	case 6://最近半年
	   		days = 180;
	   		break;
	   	default:
	   		days = 1;
	  		break;
	 }
	 
	 // 时间字符串的格式转化
	 if(strlen(sdate) > 0 && strlen(edate) > 0){
		  ncTimeToLong(sdate, &lStartTime);
	 		ncTimeToLong(edate, &lTime);
	 }
	 else{
		  ncDaysToLong(lTime, days, &lStartTime);
		  lTime=lTime+2*3600;
	 }
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lStartTime));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lTime));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 snprintf(sql, sizeof(sql), "select count(*) from ncsrvuserlog where starttime >= %lu and endtime <= %lu ", lStartTime, lTime);
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (portalname like '%%%s%%' or username like '%%%s%%') ",  _keyword, _keyword);
	 }
	 if(!utStrIsSpaces(portalname))  // Portal Name
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and portalname like '%%%s%%'", portalname);
	 }
	 if(!utStrIsSpaces(did))  // did
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", atol(did));
	 }
	 if(!utStrIsSpaces(username))  // 用户名
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
	 if(!utStrIsSpaces(devtype))  // 设备类型
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and devtype like '%%%s%%'", devtype);
	 }
	 if(!utStrIsSpaces(dev))  // 设备品牌
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev = '%s' ", dev);
	 }
	 if(!utStrIsSpaces(os))  // 操作系统
	 {
	 		if(strcmp(os, "Other")==0){
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and os = ''");	
	 		}else{
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and os like '%%%s%%'", os);
	 		}
	 }
   // pasDbOneRecord 单记录查询。　0---正确; !=0---错误代码, 具体含义请参见 数据库的有关资料
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
   
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 snprintf(sql, sizeof(sql), "select ul.sid, ul.portalname, ul.did, u.username as dname, ul.gid, g.groupname as gname, ul.userid, ul.username, ul.dispname, ul.ssid, ul.apname, ul.mac, ul.ip, ul.dev, ul.os, ul.devtype, ul.ubytes, ul.dbytes, ul.starttime, ul.endtime, ul.conntime, ul.cause from ncsrvuserlog ul left join ncmuser u on ul.did = u.userid left join ncmgroup g on ul.gid = g.groupid where starttime >= %lu and endtime <= %lu", lStartTime, lTime);
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (ul.portalname like '%%%s%%' or ul.username like '%%%s%%') ", _keyword, _keyword);
	 }
	 if(!utStrIsSpaces(portalname))  // Portal Name
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ul.portalname like '%%%s%%'", portalname);
	 }
	 if(!utStrIsSpaces(did))  // did
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ul.did = %lu", atol(did));
	 }
	 if(!utStrIsSpaces(username))  // 用户名
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ul.username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(mac))  // mac
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ul.mac like '%%%s%%'", mac);
	 }
	 if(!utStrIsSpaces(ip))  // ip
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ul.ip like '%%%s%%'", ip);
	 }
	 if(!utStrIsSpaces(devtype))  // 设备类型
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ul.devtype like '%%%s%%'", devtype);
	 }
	 		 if(!utStrIsSpaces(dev))  // 设备品牌
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ul.dev = '%s' ", dev);
	 }
	 if(!utStrIsSpaces(os))  // 操作系统
	 {
	 		if(strcmp(os, "Other")==0){
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and os = ''");	
	 		}else{
	 				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and os like '%%%s%%'", os);
	 		}
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by ul.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL) {
			unsigned long	sid = 0;
			unsigned long	did = 0;
			char 					dname[32] = "";
			unsigned long	gid = 0;
			char 					gname[32] = "";
			char 					portalname[16] = "";
			unsigned long	userid = 0;
			char 					username[32] = "";
			char 					dispname[32] = "";
			char 					ssid[32] = "";
			char 					apname[32] = "";
			char 					mac[18] = "";
			char 					ip[16] = "";
			char 					dev[24] = "";
			char 					os[16] = "";
			char 					devtype[8] = "";
			unsigned long ubytes = 0;
			unsigned long dbytes = 0;
			unsigned long starttime = 0;
			unsigned long endtime = 0;
			unsigned long conntime = 0;
			unsigned long cause = 0;
			char          s_starttime[32] = "";
			char          s_endtime[32] = "";
			char          s_conntime[32] = "";
			double        dUbytes = 0.0;
			double        dDbytes = 0.0;
			
			// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
		  while(0 == (iret = pasDbFetchInto(psCur,
		     UT_TYPE_ULONG,  4,               		&sid,
		     UT_TYPE_STRING, sizeof(portalname)-1,portalname,
		     UT_TYPE_ULONG,  4,               		&did,
		     UT_TYPE_STRING, sizeof(dname)-1,     dname,
		     UT_TYPE_ULONG,  4,               		&gid,
		     UT_TYPE_STRING, sizeof(gname)-1,     gname,
		     UT_TYPE_ULONG,  4,               		&userid,
				 UT_TYPE_STRING, sizeof(username)-1,  username,
				 UT_TYPE_STRING, sizeof(dispname)-1,  dispname,
				 UT_TYPE_STRING, sizeof(ssid)-1,      ssid,
				 UT_TYPE_STRING, sizeof(apname)-1,    apname,
				 UT_TYPE_STRING, sizeof(mac)-1,       mac,
				 UT_TYPE_STRING, sizeof(ip)-1,        ip,
				 UT_TYPE_STRING, sizeof(dev)-1,       dev,
				 UT_TYPE_STRING, sizeof(os)-1,        os,
				 UT_TYPE_STRING, sizeof(devtype)-1,   devtype,
				 UT_TYPE_ULONG,  4,               		&ubytes,
				 UT_TYPE_ULONG,  4,               		&dbytes,
				 UT_TYPE_ULONG,  4,               		&starttime,
				 UT_TYPE_ULONG,  4,               		&endtime,
				 UT_TYPE_ULONG,  4,               		&conntime,
				 UT_TYPE_ULONG,  4,               		&cause)) || 1405 == iret)
		  {
				 iNum++;
				 if(iNum > 1)
				 {
					 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
				 }
				 
			   if(starttime>0){
			   		strcpy(s_starttime, utTimFormat("%Y/%m/%d %H:%M:%S", starttime));
			   }else{
			   		strcpy(s_starttime, "");	
			   }
			   if(endtime>0){
			   		strcpy(s_endtime, utTimFormat("%Y/%m/%d %H:%M:%S", endtime));
			   }else{
			   		strcpy(s_endtime, "");
			   }
			   ncNumToTimeE(conntime, s_conntime);
			   
			   dUbytes = (double)ubytes/(1024*1024);
			   dDbytes = (double)dbytes/(1024*1024);
			  
				 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
				 utPltPutLoopVarF(psDbHead,"did",iNum,"%lu",did);
				 utPltPutLoopVarF(psDbHead,"gid",iNum,"%lu",gid);
				 utPltPutLoopVarF(psDbHead,"userid",iNum,"%lu",userid);
				 utPltPutLoopVarF(psDbHead,"ubytes",iNum,"%.2f",dUbytes);
				 utPltPutLoopVarF(psDbHead,"dbytes",iNum,"%.2f",dDbytes);
				 utPltPutLoopVarF(psDbHead,"starttime",iNum,s_starttime);
				 utPltPutLoopVarF(psDbHead,"endtime",iNum,s_endtime);
				 utPltPutLoopVarF(psDbHead,"conntime",iNum,s_conntime);
				 utPltPutLoopVarF(psDbHead,"cause",iNum,"%lu",cause);
				 utPltSetCvtHtml(1);
				 utPltPutLoopVar(psDbHead,"portalname",iNum,portalname);
				 utPltPutLoopVar(psDbHead,"dname",iNum,dname);
				 utPltPutLoopVar(psDbHead,"gname",iNum,gname);
				 utPltPutLoopVar(psDbHead,"username",iNum,username);
				 utPltPutLoopVar(psDbHead,"dispname",iNum,dispname);
				 utPltPutLoopVar(psDbHead,"ssid",iNum,ssid);
				 utPltPutLoopVar(psDbHead,"apname",iNum,apname);
				 utPltPutLoopVar(psDbHead,"mac",iNum,mac);
				 utPltPutLoopVar(psDbHead,"ip",iNum,ip);
				 utPltPutLoopVar(psDbHead,"dev",iNum,dev);
				 utPltPutLoopVar(psDbHead,"os",iNum,os);
				 utPltPutLoopVar(psDbHead,"devtype",iNum,devtype);
				 utPltSetCvtHtml(0);
				 
				 memset(s_starttime, 0, sizeof(s_starttime));
				 memset(s_endtime, 0, sizeof(s_endtime));
				 memset(s_conntime, 0, sizeof(s_conntime));
		  }
	 }
	// utPltShowDb(psDbHead);
	 pasDbCloseCursor(psCur);
	// printf("111111111111111 flag = %s\n",flag);
	 
	 if(strcmp(devtype,"devtype")==0){
			utPltPutVar(psDbHead,"sdate",sdate);
			utPltPutVar(psDbHead,"edate",edate);
			utPltPutVar(psDbHead,"devtype",flag);
	 		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_userlog_list_flag.htm");
	}else if(strcmp(dev,"dev")==0){
			utPltPutVar(psDbHead,"sdate",sdate);
			utPltPutVar(psDbHead,"edate",edate);
			utPltPutVar(psDbHead,"dev",flag);
	 		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_userlog_list_flag.htm");
			
	}
	 else{
			printf("3333333333333333333\n");
	 		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_userlog_list.htm");
	 }
	 return 0;	
}

// 用户详细上网报表
int ncm_web_MC_intDetailReport_list_1(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // 当前时间(默认结束时间)
	 
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
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 22,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
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
	 printf("slogdate = %s\n", slogdate);
	 printf("elogdate = %s\n", elogdate);
	 printf("slogtime = %s\n", slogtime);
	 printf("elogtime = %s\n", elogtime);
	 printf("username = %s\n", username);
	 printf("dev = %s\n", dev);
	 printf("os = %s\n", os);
	 printf("starttime = %s\n", starttime);
	 printf("endtime = %s\n", endtime);
	 printf("traffic = %s\n", traffic);
	 printf("insert = %s\n", caInsert);
	 printf("selsid = %s\n", caSesid);
	 printf("del = %s\n", caDel);
	 printf("update = %s\n", caUpdate);
	 printf("_keyword = %s\n", _keyword);
	 printf("start = %s\n", start);
	 printf("limit = %s\n", limit);
	 printf("sort = %s\n", sort);
	 printf("dir = %s\n", dir);
	 printf("caExport = %s\n", caExport);
	 
	 // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"starttime");
	 }
	 
	 // 读取配置文件的配置时间
	 char cnfSdate[32] = "";
	 char cnfEdate[32] = "";
	 char cnfPath[128] = "";
	 char *pConfig;
	 pasConfList *psConfig;
	 
	 pConfig = (char *)pasGetConfigFileName();
	 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
   if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","读取配置文件",ncLang("0427配置文件出错"));
        return 0;
   }
   //如若没有配置，默认null。
   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig的最后一个参数为StatisticsStim的默认值
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
	 		// 转换时间
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // 默认时间是今天0点--当前时间
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 // 查看时间是否正确
	 char _sdate[32] = ""; // 格式转换后的开始时间
	 char _edate[32] = ""; // 格式转换后的当前时间
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 sprintf(sql, "select count(*) from ncsrvuserlog where starttime>%lu and endtime<%lu",lSdate, lEdate);
	 if(!utStrIsSpaces(username)) // 根据用户登录名查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(mac)) // 根据mac地址查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", mac);
	 }
	 if(!utStrIsSpaces(ip)) // 根据ip地址查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 if(!utStrIsSpaces(dev)) // 根据设备类型查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev like '%%%s%%'", dev);
	 }
	 if(!utStrIsSpaces(os)) // 根据操作系统查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and os like '%%%s%%'", os);
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
	 		 char caSdate[32] = "";
	 		 
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
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"Internet Surfing Detail Report\n");
			 fprintf(fp,"\t\tTime:%s--%s\n", _sdate, _edate);
			 ncUtlPrintCsv(fp,8,UT_TYPE_STRING,"Mobile Phone",
			                    UT_TYPE_STRING,"MAC",
			                    UT_TYPE_STRING,"IP",
			                    UT_TYPE_STRING,"Model",
			                    UT_TYPE_STRING,"OS",
			                    UT_TYPE_STRING,"Login Time",
			                    UT_TYPE_STRING,"Logout Time",
			                    UT_TYPE_STRING,"Traffic");
	 }
	 
	 if((lEdate-lSdate)<=86400){ // 一天之间, 时间显示(13:23:23)
	 		sprintf(sql, "select username, mac, ip, dev, os, date_format(from_unixtime(starttime), '%T') as start, date_format(from_unixtime(endtime), '%T') as end, ubytes, dbytes from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate);	
	 }else{ // 超过一天, 时间显示(2013/02/12 13:23:23)
	 		sprintf(sql, "select username, mac, ip, dev, os, from_unixtime(starttime) as start, from_unixtime(endtime) as end, ubytes, dbytes from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate);	                    
	 }
	 if(!utStrIsSpaces(username)) // 根据用户登录名查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(mac)) // 根据mac地址查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", mac);
	 }
	 if(!utStrIsSpaces(ip)) // 根据ip地址查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 if(!utStrIsSpaces(dev)) // 根据设备类型查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev like '%%%s%%'", dev);
	 }
	 if(!utStrIsSpaces(os)) // 根据操作系统查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and os like '%%%s%%'", os);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username != 'Guest'");
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL){
	 	   char username[32] = "";
	 	   char mac[18] = "";
	 	   char ip[16] = "";
	 	   char dev[24] = "";
	 	   char os[16] = "";
	 	   char start[32] = "";
	 	   char end[32] = "";
	 	   unsigned long ubytes = 0;
	 		 unsigned long dbytes = 0;
	 		 double dBytes = 0.0;
	 		 
	 		 while(0 == (iret = pasDbFetchInto(psCur,
			 		   UT_TYPE_STRING, sizeof(username)-1,  username,
			 		   UT_TYPE_STRING, sizeof(mac)-1,       mac,
			 		   UT_TYPE_STRING, sizeof(ip)-1,        ip,
			 		   UT_TYPE_STRING, sizeof(dev)-1,  			dev,
			 		   UT_TYPE_STRING, sizeof(os)-1,  			os,
			 		   UT_TYPE_STRING, sizeof(start)-1,  		start,
			 		   UT_TYPE_STRING, sizeof(end)-1,  			end,
						 UT_TYPE_ULONG,  4,                   &ubytes,
						 UT_TYPE_ULONG,  4,                   &dbytes)) || 1405 == iret)
		  {  
		  	   iNum++;
		  	   if(iNum > 1)
					 {
						 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
					 }
					 
					 dBytes = (double)(ubytes+dbytes)/1024;
					 
					 utPltSetCvtHtml(1);
					 utPltPutLoopVar(psDbHead,"username",iNum,username);
					 utPltPutLoopVar(psDbHead,"mac",iNum,mac);
					 utPltPutLoopVar(psDbHead,"ip",iNum,ip);
					 utPltPutLoopVar(psDbHead,"dev",iNum,dev);
					 utPltPutLoopVar(psDbHead,"os",iNum,os);
					 utPltSetCvtHtml(0);
					 utPltPutLoopVar(psDbHead,"starttime",iNum,start);
					 utPltPutLoopVar(psDbHead,"endtime",iNum,end);
					 utPltPutLoopVarF(psDbHead,"traffic",iNum,"%.2f",dBytes);
					 
					 if(strlen(caExport)>0){
					 		ncUtlPrintCsv(fp,8,UT_TYPE_STRING,username,
														 		UT_TYPE_STRING,mac,
														 		UT_TYPE_STRING,ip,
			                          UT_TYPE_STRING,dev,
			                          UT_TYPE_STRING,os,
			                          UT_TYPE_STRING,start,
			                          UT_TYPE_STRING,end,
			                          UT_TYPE_FLOAT,dBytes);
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
      return 0;
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_intDetailReport_list.htm");
	 return 0;	
}

// 每个小时上网人数和上网流量报表 
int ncm_web_MC_intStatisticsReport_list_1(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
	 char slogdate[64] = "";
	 char elogdate[64] = "";
	 char slogtime[64] = "";
	 char elogtime[64] = "";
	 char timeV[32] = "";
	 char number[32] = "";
	 char traffic[64] = "";
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0);
	 
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
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 创建纪录信息结构体
	 int i = 0;
	 typedef struct int_stat{
	 		int num;
	 		char timeV[20];
	 		long number;
	 		double traffic;
	 }int_stat_s;
	 int_stat_s stat_int[24];
	 
	 for (i = 0;i < 24;i++){ // 初始化
		 	 stat_int[i].num = i;
		   sprintf(stat_int[i].timeV,"%02d:00:00-%02d:59:59",i,i);
		   stat_int[i].number = 0;
		   stat_int[i].traffic = 0.0;
		   printf("num = %d, timeV = %s, number = %d, traffic = %.2f\n", stat_int[i].num, stat_int[i].timeV, stat_int[i].number, stat_int[i].traffic);
	 }
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 17,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
	    "slogdate",    UT_TYPE_STRING,  sizeof(slogdate)-1,       slogdate,
	    "elogdate",    UT_TYPE_STRING,  sizeof(elogdate)-1,       elogdate,
	    "slogtime",    UT_TYPE_STRING,  sizeof(slogtime)-1,       slogtime,
	    "elogtime",    UT_TYPE_STRING,  sizeof(elogtime)-1,       elogtime,
			"time",        UT_TYPE_STRING,  sizeof(timeV)-1,          timeV,
			"number",      UT_TYPE_STRING,  sizeof(number)-1,         number,
			"traffic",     UT_TYPE_STRING,  sizeof(traffic)-1,        traffic,
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
	 printf("slogdate = %s\n", slogdate);
	 printf("elogdate = %s\n", elogdate);
	 printf("slogtime = %s\n", slogtime);
	 printf("elogtime = %s\n", elogtime);
	 printf("timeV = %s\n", timeV);
	 printf("number = %s\n", number);
	 printf("traffic = %s\n", traffic);
	 printf("del = %s\n", caDel);
	 printf("caUpdate = %s\n", caUpdate);
	 printf("_keyword = %s\n", _keyword);
	 printf("caExport = %s\n", caExport);
	 
	 // 读取配置文件的配置时间
	 char cnfSdate[32] = "";
	 char cnfEdate[32] = "";
	 char cnfPath[128] = "";
	 char *pConfig;
	 pasConfList *psConfig;
	 
	 pConfig = (char *)pasGetConfigFileName();
	 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
   if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","读取配置文件",ncLang("0427配置文件出错"));
        return 0;
   }
   //如若没有配置，默认null。
   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig的最后一个参数为StatisticsStim的默认值
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
	 		// 转换时间
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // 默认时间是今天0点--当前时间
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 char _sdate[32] = ""; // 格式转换后的开始时间
	 char _edate[32] = ""; // 格式转换后的当前时间
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 // 导出
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
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"Internet surfing Statistics Report\n");
			 fprintf(fp,"\t\tTime:%s--%s\n", _sdate, _edate);
			 ncUtlPrintCsv(fp,5,UT_TYPE_STRING,"Time",
			 									  UT_TYPE_STRING,"Number",
			                    UT_TYPE_STRING,"Traffic(Mb)");
	 }
	 
	 for(i=0; i<24; i++){
			 memset(sql, 0, sizeof(sql));
			 sprintf(sql, "select count(*) as usernum, %d as ptime from ncsrvuserlog where starttime>%lu and endtime<%lu", i, lSdate, lEdate);		                    
			 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and hour(from_unixtime(starttime)) <= %d and hour(from_unixtime(endtime)) >= %d", i, i);
			 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by ptime");
			 printf("sql = %s\n", sql);
			 psCur = pasDbOpenSql(sql, 0);
			 if(psCur != NULL){
			 		 unsigned long usernum = 0;
			 		 char ptime[32] = "";
			 		 while(0 == (iret = pasDbFetchInto(psCur, 
			 		 									UT_TYPE_ULONG, 4,                &usernum, 
			 		 									UT_TYPE_STRING, sizeof(ptime)-1, ptime)) || 1405 == iret)
				  {
				  		 stat_int[i].number = usernum;
				  }
			 }
			pasDbCloseCursor(psCur);
	}
	 
	 for(i=0; i<24; i++){
	 		 memset(sql, 0, sizeof(sql));
			 sprintf(sql, "select sum(ubytes+dbytes) as traf, %d as ptime from ncsrvuserlog where starttime>%lu and endtime<%lu", i, lSdate, lEdate);		                    
			 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and hour(from_unixtime(starttime)) <= %d and hour(from_unixtime(endtime)) >= %d", i, i);
			 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by ptime");
			 printf("sql = %s\n", sql);
			 psCur = pasDbOpenSql(sql, 0);
			 if(psCur != NULL){
			 		 unsigned long traf = 0;
			 		 char ptime[32] = "";
			 		 while(0 == (iret = pasDbFetchInto(psCur,
								 UT_TYPE_ULONG,  4,               &traf,
								 UT_TYPE_STRING, sizeof(ptime)-1, ptime)) || 1405 == iret)
				  {
				  		 stat_int[i].traffic = (double)traf/(1024*1024);
				  		 printf("traffic = %.2f\n", stat_int[i].traffic);
				  }
			 }
	 		 pasDbCloseCursor(psCur);	
	 }
	 
	 for(i=0; i<24; i++){
	 		iNum++;
  	  if(iNum > 1)
			{
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
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
	 utPltShowDb(psDbHead);
	 
	 if(strlen(caExport)>0){
 	    fclose(fp);
      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
      //remove(caFile);
      utPltFreeDb(psDbHead);
      return 0;
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_intStatisticsReport_list.htm");
	 return 0;
}

// 上网设备类型报表
int ncm_web_MC_intDeviceReport_list_1(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 int iNumber = 0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char sdate[32] = "";
	 char edate[32] = "";
	 char slogdate[64] = "";
	 char elogdate[64] = "";
	 char slogtime[64] = "";
	 char elogtime[64] = "";
	 char dev[32] = "";
	 char devtype[32] = "";
	 char num[20] = "";
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // 当前时间(默认结束时间)
	 
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
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 17,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
	    "slogdate",    UT_TYPE_STRING,  sizeof(slogdate)-1,       slogdate,
	    "elogdate",    UT_TYPE_STRING,  sizeof(elogdate)-1,       elogdate,
	    "slogtime",    UT_TYPE_STRING,  sizeof(slogtime)-1,       slogtime,
	    "elogtime",    UT_TYPE_STRING,  sizeof(elogtime)-1,       elogtime,
			"dev",         UT_TYPE_STRING,  sizeof(dev)-1,       		  dev,
			"devtype",     UT_TYPE_STRING,  sizeof(devtype)-1,        devtype,
			"num",         UT_TYPE_STRING,  sizeof(num)-1,            num,
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
	 printf("slogdate = %s\n", slogdate);
	 printf("elogdate = %s\n", elogdate);
	 printf("slogtime = %s\n", slogtime);
	 printf("elogtime = %s\n", elogtime);
	 printf("dev = %s\n", dev);
	 printf("devtype = %s\n", devtype);
	 printf("num = %s\n", num);
	 printf("del = %s\n", caDel);
	 printf("caUpdate = %s\n", caUpdate);
	 printf("_keyword = %s\n", _keyword);
	 
	 // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"starttime");
	 }
	 
	 // 读取配置文件的配置时间
	 char cnfSdate[32] = "";
	 char cnfEdate[32] = "";
	 char cnfPath[128] = "";
	 char *pConfig;
	 pasConfList *psConfig;
	 
	 pConfig = (char *)pasGetConfigFileName();
	 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
   if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","读取配置文件",ncLang("0427配置文件出错"));
        return 0;
   }
   //如若没有配置，默认null。
   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig的最后一个参数为StatisticsStim的默认值
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
	 		// 转换时间
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // 默认时间是今天0点--当前时间
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 char _sdate[32] = ""; // 格式转换后的开始时间
	 char _edate[32] = ""; // 格式转换后的当前时间
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 sprintf(sql, "select count(*) from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate);
	 if(!utStrIsSpaces(devtype)) // 根据设备类型查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and devtype like '%%%s%%'", devtype);
	 }
	 if(!utStrIsSpaces(dev)) // 根据设备名称查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev like '%%%s%%'", dev);
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
	 		 		 printf("打开文件失败！\n");
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"Device Model Report\n");
			 fprintf(fp,"\t\tTime:%s--%s\n", sdate, edate);
			 ncUtlPrintCsv(fp,3,UT_TYPE_STRING,"Type",
			                    UT_TYPE_STRING,"Model",
			                    UT_TYPE_STRING,"Number");
	 }
	 
	 sprintf(sql, "select count(*) as lCount2, dev, devtype from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate);		                    
	 if(!utStrIsSpaces(devtype)) // 根据设备类型查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and devtype like '%%%s%%'", devtype);
	 }
	 if(!utStrIsSpaces(dev)) // 根据设备名称查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev like '%%%s%%'", dev);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by devtype");
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL){
	 		 unsigned long lCount2 = 0;
	 		 char dev[24] = "";
	 		 char date[32] = "";
	 		 char devtype[8] = "";
	 		 while(0 == (iret = pasDbFetchInto(psCur,
						 UT_TYPE_ULONG,  4,                   &lCount2,
						 UT_TYPE_STRING, sizeof(dev)-1,  			dev,
						 UT_TYPE_STRING, sizeof(devtype)-1,  	devtype)) || 1405 == iret)
		  {
		  	   iNum++;
		  	   if(iNum > 1)
					 {
						 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
					 }
					 
					 if(strlen(devtype)==0){
					 		strcpy(devtype, "other");	
					 }
					 if(strlen(dev)==0){
					 		strcpy(dev, "other");	
					 }
					 utPltSetCvtHtml(1);
					 utPltPutLoopVar(psDbHead,"dev",iNum,dev);
					 utPltPutLoopVar(psDbHead,"devtype",iNum,devtype);
					 utPltSetCvtHtml(0);
					 utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",lCount2);
					 if(strlen(caExport)>0){
					 		ncUtlPrintCsv(fp,3,UT_TYPE_STRING,devtype,
														 		UT_TYPE_STRING,dev,
														 		UT_TYPE_ULONG,lCount2);
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
      return 0;
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_intDeviceReport_list.htm");
	 return 0;
}

// 短信发送详细报表
int ncm_web_MC_smsDetailReport_list_1(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 int iNumber = 0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
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
	 
	 char caContrycode[32] = "";
	 char caUsername[32] = "";
	 char caLogtime[64] = "";
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // 当前时间(默认结束时间)
	 
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
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 21,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
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
	 printf("slogdate = %s\n", slogdate);
	 printf("elogdate = %s\n", elogdate);
	 printf("slogtime = %s\n", slogtime);
	 printf("elogtime = %s\n", elogtime);
	 printf("contrycode = %s\n", contrycode);
	 printf("username = %s\n", username);
	 printf("usermac = %s\n", usermac);
	 printf("ip = %s\n", ip);
	 printf("logtime = %s\n", logtime);
	 printf("del = %s\n", caDel);
	 printf("caUpdate = %s\n", caUpdate);
	 printf("_keyword = %s\n", _keyword);
	 
	 // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"logtime");
	 }
	 
	 // 读取配置文件的配置时间
	 char cnfSdate[32] = "";
	 char cnfEdate[32] = "";
	 char cnfPath[128] = "";
	 char *pConfig;
	 pasConfList *psConfig;
	 
	 pConfig = (char *)pasGetConfigFileName();
	 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
   if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","读取配置文件",ncLang("0427配置文件出错"));
        return 0;
   }
   //如若没有配置，默认null。
   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig的最后一个参数为StatisticsStim的默认值
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
	 		// 转换时间
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // 默认时间是今天0点--当前时间
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 char _sdate[32] = ""; // 格式转换后的开始时间
	 char _edate[32] = ""; // 格式转换后的当前时间
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 sprintf(sql, "select count(*) from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);
	 if(!utStrIsSpaces(contrycode)) // 根据国家代码查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and contrycode like '%%%s%%'", contrycode);
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
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"SMS Detail report\n");
			 fprintf(fp,"\t\tTime:%s--%s\n", _sdate, _edate);
			 ncUtlPrintCsv(fp,7,UT_TYPE_STRING,"Country Code",
	 									  UT_TYPE_STRING,"Mobile Phone",
	                    UT_TYPE_STRING,"MAC",
	                    UT_TYPE_STRING,"IP",
	                    UT_TYPE_STRING,"Send Time",
	                    UT_TYPE_STRING,"Operator type",
	                    UT_TYPE_STRING,"Zone Name");
	 }
	 
	 if((lEdate-lSdate)<=86400){ // 一天之间, 时间显示(13:23:23)
	 		sprintf(sql, "select username, usermac, ip, date_format(from_unixtime(logtime), '%T') as time, sptype, contrycode, areaname from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);
	 }else{ // 超过一天, 时间显示(2013/02/12 13:23:23)
	 		sprintf(sql, "select username, usermac, ip, from_unixtime(logtime) as time, sptype, contrycode, areaname from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);	                    
	 }
	 if(!utStrIsSpaces(contrycode)) // 根据区域查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and contrycode like '%%%s%%'", contrycode);
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
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username != 'Guest'");
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by sptype, areaname");
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " limit %lu, %lu", iStart, iLimit < lCount1 ? iLimit : lCount1);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL){
	 		 char username[32] = "";
	 		 char usermac[18] = "";
	 		 char ip[16] = "";
	 		 char time[32] = "";
	 		 unsigned long sptype = 0;
	 		 unsigned long contrycode = 0;
	 		 char areaname[32] = "";
	 		 char caSptype[32] = "";
	 		 
	 		 while(0 == (iret = pasDbFetchInto(psCur,
				 UT_TYPE_STRING, sizeof(username)-1,  	username,
				 UT_TYPE_STRING, sizeof(usermac)-1,  	  usermac,
				 UT_TYPE_STRING, sizeof(ip)-1,  	      ip,
				 UT_TYPE_STRING, sizeof(time)-1,  			time,
				 UT_TYPE_ULONG,  4,  	                  &sptype,
				 UT_TYPE_ULONG,  4,  	                  &contrycode,
				 UT_TYPE_STRING, sizeof(areaname)-1,  	areaname)) || 1405 == iret)
		  {
	       iNum++;
	  	   if(iNum > 1)
				 {
					 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
				 }
				 
				 switch(sptype){
		  				case 0:strcpy(caSptype, "Illegal Phone Number");break;
		  				case 1:strcpy(caSptype, "China Mobile");break;
		  				case 2:strcpy(caSptype, "China Unicom");break;	
		  				case 3:strcpy(caSptype, "China Telecom");break;
		  				case 8:strcpy(caSptype, "Other");break;
		  				case 9:strcpy(caSptype, "International");break;
		  				default:break;
		  	 }
		  		
				 utPltPutLoopVarF(psDbHead,"contrycode",iNum,"%lu",contrycode);
				 utPltPutLoopVar(psDbHead,"sptype",iNum,caSptype);
				 utPltSetCvtHtml(1);
				 utPltPutLoopVar(psDbHead,"areaname",iNum,areaname);
				 utPltPutLoopVar(psDbHead,"username",iNum,username);
				 utPltPutLoopVar(psDbHead,"usermac",iNum,usermac);
				 utPltPutLoopVar(psDbHead,"ip",iNum,ip);
				 utPltSetCvtHtml(0);
				 utPltPutLoopVar(psDbHead,"logtime",iNum,time);
				 if(strlen(caExport)>0){
					 		ncUtlPrintCsv(fp,7,UT_TYPE_ULONG,contrycode,
			 											UT_TYPE_STRING,username,
												 		UT_TYPE_STRING,usermac,
												 		UT_TYPE_STRING,ip,
	                          UT_TYPE_STRING,time,
	                          UT_TYPE_STRING,caSptype,
	                          UT_TYPE_STRING,areaname);
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
      return 0;
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_smsDetailReport_list.htm");
	 return 0;
}

// 每小时发送短信报表 
int ncm_web_MC_smsStatisticsReport_list_1(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 int iNumber = 0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char sdate[32] = "";
	 char edate[32] = "";
	 char slogdate[64] = "";
	 char elogdate[64] = "";
	 char slogtime[64] = "";
	 char elogtime[64] = "";
	 char timeV[32] = "";
	 char localNum[32] = "";
	 char interNum[32] = "";
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // 当前时间(默认结束时间)
	 
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
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 创建纪录信息结构体
	 int i = 0;
	 typedef struct sms_stat{
	 		int num;
	 		char timeV[18];
	 		long localNum;
	 		long interNum;
	 }sms_stat_s;
	 sms_stat_s stat_sms[24];
	 
	 for (i = 0;i < 24;i++){ // 初始化
		 	 stat_sms[i].num = i;
		   sprintf(stat_sms[i].timeV,"%02d:00:00-%02d:59:59",i,i);
		   stat_sms[i].localNum = 0;
		   stat_sms[i].interNum = 0;
		   //printf("num = %d timeV = %s localNum = %d interNum = %d \n",stat_sms[i].num,stat_sms[i].timeV,stat_sms[i].localNum,stat_sms[i].interNum);	
	 }
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 17,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
	    "slogdate",    UT_TYPE_STRING,  sizeof(slogdate)-1,       slogdate,
	    "elogdate",    UT_TYPE_STRING,  sizeof(elogdate)-1,       elogdate,
	    "slogtime",    UT_TYPE_STRING,  sizeof(slogtime)-1,       slogtime,
	    "elogtime",    UT_TYPE_STRING,  sizeof(elogtime)-1,       elogtime,
			"timeV",       UT_TYPE_STRING,  sizeof(timeV)-1,          timeV,
			"localNum",    UT_TYPE_STRING,  sizeof(localNum)-1,       localNum,
			"interNum",    UT_TYPE_STRING,  sizeof(interNum)-1,       interNum,
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
	 printf("slogdate = %s\n", slogdate);
	 printf("elogdate = %s\n", elogdate);
	 printf("slogtime = %s\n", slogtime);
	 printf("elogtime = %s\n", elogtime);
	 printf("timeV = %s\n", timeV);
	 printf("localNum = %s\n", localNum);
	 printf("interNum = %s\n", interNum);
	 printf("del = %s\n", caDel);
	 printf("caUpdate = %s\n", caUpdate);
	 printf("_keyword = %s\n", _keyword);
	 
	 // 读取配置文件的配置时间
	 char cnfSdate[32] = "";
	 char cnfEdate[32] = "";
	 char cnfPath[128] = "";
	 char *pConfig;
	 pasConfList *psConfig;
	 
	 pConfig = (char *)pasGetConfigFileName();
	 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
   if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","读取配置文件",ncLang("0427配置文件出错"));
        return 0;
   }
   //如若没有配置，默认null。
   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig的最后一个参数为StatisticsStim的默认值
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
	 		// 转换时间
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // 默认时间是今天0点--当前时间
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 char _sdate[32] = ""; // 格式转换后的开始时间
	 char _edate[32] = ""; // 格式转换后的当前时间
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 sprintf(sql, "select count(*) from ncsrvsmslog where logtime>%lu and logtime<%lu",lSdate, lEdate);
	  
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 // 导出
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
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"SMS Statistics Report\n");
			 fprintf(fp,"\t\tTime:%s--%s\n", _sdate, _edate);
			 ncUtlPrintCsv(fp,5,UT_TYPE_STRING,"Time",
			 									  UT_TYPE_STRING,"Count of Local SMS",
			                    UT_TYPE_STRING,"Count of International SMS");
	 }
	 
	 memset(sql,0,sizeof(sql)); // 国内短信统计
	 sprintf(sql, "select count(*) as smsnum, hour(from_unixtime(logtime)) as htime from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);		                    
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (contrycode = '086' or contrycode = '86')");
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by htime");
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " limit %lu, %lu", iStart, iLimit < lCount1 ? iLimit : lCount1);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL){
	 		 unsigned long smsnum = 0;
	 		 char htime[24] = "";
	 		 while(0 == (iret = pasDbFetchInto(psCur,
						 UT_TYPE_ULONG,  4,                   &smsnum,
						 UT_TYPE_STRING, sizeof(htime)-1,  		htime)) || 1405 == iret)
		  {
		  	   printf("smsnum = %d, htime = %s\n", smsnum, htime);
		  	   stat_sms[atol(htime)].localNum = smsnum;
		  }
	 }
	 pasDbCloseCursor(psCur);
	 
	 memset(sql,0,sizeof(sql)); // 国际短信统计
	 sprintf(sql, "select count(*) as smsnum, hour(from_unixtime(logtime)) as htime from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);		                    
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (contrycode != '086' and contrycode != '86')");
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by htime");
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " limit %lu, %lu", iStart, iLimit < lCount1 ? iLimit : lCount1);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL){
	 		 unsigned long smsnum = 0;
	 		 char htime[24] = "";
	 		 while(0 == (iret = pasDbFetchInto(psCur,
						 UT_TYPE_ULONG,  4,                   &smsnum,
						 UT_TYPE_STRING, sizeof(htime)-1,  		htime)) || 1405 == iret)
		  {
		  	   printf("smsnum = %d, htime = %s\n", smsnum, htime);
		  	   stat_sms[atol(htime)].interNum = smsnum;
		  }
	 }
	 pasDbCloseCursor(psCur);
	 
	 for(i=0; i<24; i++){
	 		iNum++;
	    if(iNum > 1)
		  {
			   utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
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
	 utPltShowDb(psDbHead);
	  
	 if(strlen(caExport)>0){
 	    fclose(fp);
      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
      //remove(caFile);
      utPltFreeDb(psDbHead);
      return 0;
	 }
	 
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_smsStatisticsReport_list.htm");
	 return 0;
}

/*int ncm_web_MC_smsStatisticsReport_list_1(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 int iNumber = 0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char sdate[32] = "";
	 char edate[32] = "";
	 char slogdate[64] = "";
	 char elogdate[64] = "";
	 char slogtime[64] = "";
	 char elogtime[64] = "";
	 char timeV[32] = "";
	 char localNum[32] = "";
	 char interNum[32] = "";
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // 当前时间(默认结束时间)
	 
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
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 创建纪录信息结构体
	 int i = 0;
	 typedef struct sms_stat{
	 		int num;
	 		char timeV[18];
	 		long localNum;
	 		long interNum;
	 }sms_stat_s;
	 sms_stat_s stat_sms[24];
	 
	 for (i = 0;i < 24;i++){ // 初始化
		 	 stat_sms[i].num = i;
		   sprintf(stat_sms[i].timeV,"%02d:00:00-%02d:59:59",i,i);
		   stat_sms[i].localNum = 0;
		   stat_sms[i].interNum = 0;
		   //printf("num = %d timeV = %s localNum = %d interNum = %d \n",stat_sms[i].num,stat_sms[i].timeV,stat_sms[i].localNum,stat_sms[i].interNum);	
	 }
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 17,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
	    "slogdate",    UT_TYPE_STRING,  sizeof(slogdate)-1,       slogdate,
	    "elogdate",    UT_TYPE_STRING,  sizeof(elogdate)-1,       elogdate,
	    "slogtime",    UT_TYPE_STRING,  sizeof(slogtime)-1,       slogtime,
	    "elogtime",    UT_TYPE_STRING,  sizeof(elogtime)-1,       elogtime,
			"timeV",       UT_TYPE_STRING,  sizeof(timeV)-1,          timeV,
			"localNum",    UT_TYPE_STRING,  sizeof(localNum)-1,       localNum,
			"interNum",    UT_TYPE_STRING,  sizeof(interNum)-1,       interNum,
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
	 printf("slogdate = %s\n", slogdate);
	 printf("elogdate = %s\n", elogdate);
	 printf("slogtime = %s\n", slogtime);
	 printf("elogtime = %s\n", elogtime);
	 printf("timeV = %s\n", timeV);
	 printf("localNum = %s\n", localNum);
	 printf("interNum = %s\n", interNum);
	 printf("del = %s\n", caDel);
	 printf("caUpdate = %s\n", caUpdate);
	 printf("_keyword = %s\n", _keyword);
	 
 		// 转换时间
	  sprintf(sdate, "%s %s", slogdate, slogtime);
	  sprintf(edate, "%s %s", elogdate, elogtime);
	  
	  if(strlen(sdate) > 1 && strlen(edate) > 1){
			 ncTimeToLong(sdate, &lSdate);
			 ncTimeToLong(edate, &lEdate);
			 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	  }else{
	 	   ncDaysToLong(lEdate, 1, &lSdate); // 默认时间是今天0点--当前时间
	 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	  }
	 
	 char _sdate[32] = ""; // 格式转换后的开始时间
	 char _edate[32] = ""; // 格式转换后的当前时间
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 memset(sql,0,sizeof(sql)); // 国内短信统计
	 sprintf(sql, "select count(*) as smsnum, hour(from_unixtime(logtime)) as htime from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);		                    
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (contrycode = '086' or contrycode = '86')");
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by htime");
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL){
	 		 unsigned long smsnum = 0;
	 		 char htime[24] = "";
	 		 while(0 == (iret = pasDbFetchInto(psCur,
						 UT_TYPE_ULONG,  4,                   &smsnum,
						 UT_TYPE_STRING, sizeof(htime)-1,  		htime)) || 1405 == iret)
		  {
		  	   printf("smsnum = %d, htime = %s\n", smsnum, htime);
		  	   stat_sms[atol(htime)].localNum = smsnum;
		  }
	 }
	 pasDbCloseCursor(psCur);
	 
	 memset(sql,0,sizeof(sql)); // 国际短信统计
	 sprintf(sql, "select count(*) as smsnum, hour(from_unixtime(logtime)) as htime from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);		                    
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (contrycode != '086' and contrycode != '86')");
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by htime");
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " limit %lu, %lu", iStart, iLimit < lCount1 ? iLimit : lCount1);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL){
	 		 unsigned long smsnum = 0;
	 		 char htime[24] = "";
	 		 while(0 == (iret = pasDbFetchInto(psCur,
						 UT_TYPE_ULONG,  4,                   &smsnum,
						 UT_TYPE_STRING, sizeof(htime)-1,  		htime)) || 1405 == iret)
		  {
		  	   printf("smsnum = %d, htime = %s\n", smsnum, htime);
		  	   stat_sms[atol(htime)].interNum = smsnum;
		  }
	 }
	 pasDbCloseCursor(psCur);
	 
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
       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
       return 0;	
	 }
	 
	 fprintf(fp,"SMS Statistics Report\n");
	 fprintf(fp,"\t\tTime:%s--%s\n", _sdate, _edate);
	 ncUtlPrintCsv(fp,5,UT_TYPE_STRING,"Time",
	 									  UT_TYPE_STRING,"Count of Local SMS",
	                    UT_TYPE_STRING,"Count of International SMS");
	 
	 for(i=0; i<24; i++){
		 		ncUtlPrintCsv(fp,3,UT_TYPE_STRING,stat_sms[i].timeV,
		 											UT_TYPE_ULONG,stat_sms[i].localNum,
											 		UT_TYPE_ULONG,stat_sms[i].interNum);
	 }
	 utPltShowDb(psDbHead);
	  
   fclose(fp);
   utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
   //remove(caFile);
   utPltFreeDb(psDbHead);
   
   return 0;
}*/



// 用户详细上网报表
int ncm_web_MC_intDetailReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char cnfSdate[32], char cnfEdate[32], char caExport1[32], char cnfPath[128])
{
	 if(strlen(cnfSdate)==0 && strlen(cnfEdate)==0){
	 		utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 }
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
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
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // 当前时间(默认结束时间)
	 
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
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 if(strlen(cnfSdate)==0 && strlen(cnfEdate)==0){
			 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
			 utMsgGetSomeNVar(psMsgHead, 22,
			    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
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
			 printf("slogdate = %s\n", slogdate);
			 printf("elogdate = %s\n", elogdate);
			 printf("slogtime = %s\n", slogtime);
			 printf("elogtime = %s\n", elogtime);
			 printf("username = %s\n", username);
			 printf("dev = %s\n", dev);
			 printf("os = %s\n", os);
			 printf("starttime = %s\n", starttime);
			 printf("endtime = %s\n", endtime);
			 printf("traffic = %s\n", traffic);
			 printf("insert = %s\n", caInsert);
			 printf("selsid = %s\n", caSesid);
			 printf("del = %s\n", caDel);
			 printf("update = %s\n", caUpdate);
			 printf("_keyword = %s\n", _keyword);
			 printf("start = %s\n", start);
			 printf("limit = %s\n", limit);
			 printf("sort = %s\n", sort);
			 printf("dir = %s\n", dir);
			 printf("caExport = %s\n", caExport);
			 
			 // 排序
		   if(strlen(sort)==0){
			 	  strcpy(sort,"starttime");
			 }
			 
			 // 读取配置文件的配置时间
			 char cnfSdate[32] = "";
			 char cnfEdate[32] = "";
			 char cnfPath[128] = "";
			 char *pConfig;
			 pasConfList *psConfig;
			 
			 pConfig = (char *)pasGetConfigFileName();
			 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
		   if(psConfig == NULL) {
		        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","读取配置文件",ncLang("0427配置文件出错"));
		        return 0;
		   }
		   //如若没有配置，默认null。
		   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig的最后一个参数为StatisticsStim的默认值
		 	 strcpy(cnfEdate,(char *)pasUtlLookConfig(psConfig,"StatisticsEtime",""));
		 	 strcpy(cnfPath,(char *)pasUtlLookConfig(psConfig,"ReportPosition",""));
		   printf("cnfSdate = %s\n", cnfSdate);
			 printf("cnfEdate = %s\n", cnfEdate);
			 printf("cnfPath = %s\n", cnfPath);
		   pasUtlFreeConfig(psConfig);
   }
	 
	 if(strlen(cnfSdate)>0 && strlen(cnfEdate)>0){
	 		ncTimeToLong(cnfSdate, &lSdate);
			ncTimeToLong(cnfEdate, &lEdate);
			printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	 }else{
	 		// 转换时间
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // 默认时间是今天0点--当前时间
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 // 查看时间是否正确
	 char _sdate[32] = ""; // 格式转换后的开始时间
	 char _edate[32] = ""; // 格式转换后的当前时间
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 sprintf(sql, "select count(*) from ncsrvuserlog where starttime>%lu and endtime<%lu",lSdate, lEdate);
	 if(!utStrIsSpaces(username)) // 根据用户登录名查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(mac)) // 根据mac地址查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", mac);
	 }
	 if(!utStrIsSpaces(ip)) // 根据ip地址查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 if(!utStrIsSpaces(dev)) // 根据设备类型查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev like '%%%s%%'", dev);
	 }
	 if(!utStrIsSpaces(os)) // 根据操作系统查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and os like '%%%s%%'", os);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 strcpy(caExport, caExport1);
	 // 导出
	 if(strlen(caExport)>0){
	 		 char caSdate[32] = "";
	 		 if(strlen(cnfPath)>0){
	 		 		strcpy(caFile, cnfPath);
	 		 }else{
	 		 		strcpy(caFilename, "intSurfDetailReport.csv");
		 		  if(strlen(cnfPath)>0){
		 		 		 strcpy(caPath, cnfPath);
		 		  }else{
		 		 		 strcpy(caPath, FILEPATH);
		 		  }
		 		  sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 }
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"Internet Surfing Detail Report\n");
			 fprintf(fp,"\t\tTime:%s--%s\n", _sdate, _edate);
			 ncUtlPrintCsv(fp,8,UT_TYPE_STRING,"Mobile Phone",
			                    UT_TYPE_STRING,"MAC",
			                    UT_TYPE_STRING,"IP",
			                    UT_TYPE_STRING,"Model",
			                    UT_TYPE_STRING,"OS",
			                    UT_TYPE_STRING,"Login Time",
			                    UT_TYPE_STRING,"Logout Time",
			                    UT_TYPE_STRING,"Traffic");
	 }
	 
	 if((lEdate-lSdate)<=86400){ // 一天之间, 时间显示(13:23:23)
	 		sprintf(sql, "select username, mac, ip, dev, os, date_format(from_unixtime(starttime), '%T') as start, date_format(from_unixtime(endtime), '%T') as end, ubytes, dbytes from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate);	
	 }else{ // 超过一天, 时间显示(2013/02/12 13:23:23)
	 		sprintf(sql, "select username, mac, ip, dev, os, from_unixtime(starttime) as start, from_unixtime(endtime) as end, ubytes, dbytes from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate);	                    
	 }
	 if(!utStrIsSpaces(username)) // 根据用户登录名查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(mac)) // 根据mac地址查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", mac);
	 }
	 if(!utStrIsSpaces(ip)) // 根据ip地址查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ip like '%%%s%%'", ip);
	 }
	 if(!utStrIsSpaces(dev)) // 根据设备类型查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev like '%%%s%%'", dev);
	 }
	 if(!utStrIsSpaces(os)) // 根据操作系统查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and os like '%%%s%%'", os);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username != 'Guest'");
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL){
	 	   char username[32] = "";
	 	   char mac[18] = "";
	 	   char ip[16] = "";
	 	   char dev[24] = "";
	 	   char os[16] = "";
	 	   char start[32] = "";
	 	   char end[32] = "";
	 	   unsigned long ubytes = 0;
	 		 unsigned long dbytes = 0;
	 		 double dBytes = 0.0;
	 		 
	 		 while(0 == (iret = pasDbFetchInto(psCur,
			 		   UT_TYPE_STRING, sizeof(username)-1,  username,
			 		   UT_TYPE_STRING, sizeof(mac)-1,       mac,
			 		   UT_TYPE_STRING, sizeof(ip)-1,        ip,
			 		   UT_TYPE_STRING, sizeof(dev)-1,  			dev,
			 		   UT_TYPE_STRING, sizeof(os)-1,  			os,
			 		   UT_TYPE_STRING, sizeof(start)-1,  		start,
			 		   UT_TYPE_STRING, sizeof(end)-1,  			end,
						 UT_TYPE_ULONG,  4,                   &ubytes,
						 UT_TYPE_ULONG,  4,                   &dbytes)) || 1405 == iret)
		  {  
		  	   iNum++;
		  	   if(iNum > 1)
					 {
						 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
					 }
					 
					 dBytes = (double)(ubytes+dbytes)/1024;
					 
					 utPltSetCvtHtml(1);
					 utPltPutLoopVar(psDbHead,"username",iNum,username);
					 utPltPutLoopVar(psDbHead,"mac",iNum,mac);
					 utPltPutLoopVar(psDbHead,"ip",iNum,ip);
					 utPltPutLoopVar(psDbHead,"dev",iNum,dev);
					 utPltPutLoopVar(psDbHead,"os",iNum,os);
					 utPltSetCvtHtml(0);
					 utPltPutLoopVar(psDbHead,"starttime",iNum,start);
					 utPltPutLoopVar(psDbHead,"endtime",iNum,end);
					 utPltPutLoopVarF(psDbHead,"traffic",iNum,"%.2f",dBytes);
					 
					 if(strlen(caExport)>0){
					 		ncUtlPrintCsv(fp,8,UT_TYPE_STRING,username,
														 		UT_TYPE_STRING,mac,
														 		UT_TYPE_STRING,ip,
			                          UT_TYPE_STRING,dev,
			                          UT_TYPE_STRING,os,
			                          UT_TYPE_STRING,start,
			                          UT_TYPE_STRING,end,
			                          UT_TYPE_FLOAT,dBytes);
					 }
		  }
	 }
	 //utPltShowDb(psDbHead); 
	 pasDbCloseCursor(psCur);
	 
	 if(strlen(caExport)>0){
 	    fclose(fp);
 	    printf("**************END0***********\n");
      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
      printf("**************END1***********\n");
      //remove(caFile);
      utPltFreeDb(psDbHead);
      return 0;
	 }
	 
	 if(strlen(cnfSdate)==0 && strlen(cnfEdate)==0){
	 		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_intDetailReport_list.htm");
	 }
	 return 0;	
}

// 每个小时上网人数和上网流量报表 
int ncm_web_MC_intStatisticsReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char cnfSdate[32], char cnfEdate[32], char caExport1[32], char cnfPath[128])
{  
	 if(strlen(cnfSdate)==0 && strlen(cnfEdate)==0){
	 		utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 }
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char sdate[32] = "";
	 char edate[32] = "";
	 char slogdate[64] = "";
	 char elogdate[64] = "";
	 char slogtime[64] = "";
	 char elogtime[64] = "";
	 char timeV[32] = "";
	 char number[32] = "";
	 char traffic[64] = "";
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0);
	 
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
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 创建纪录信息结构体
	 int i = 0;
	 typedef struct int_stat{
	 		int num;
	 		char timeV[18];
	 		long number;
	 		double traffic;
	 }int_stat_s;
	 int_stat_s stat_int[24];
	 
	 for (i = 0;i < 24;i++){ // 初始化
		 	 stat_int[i].num = i;
		   sprintf(stat_int[i].timeV,"%02d:00:00-%02d:59:59",i,i);
		   stat_int[i].number = 0;
		   stat_int[i].traffic = 0.0;
	 }
	 
	 if(strlen(cnfSdate)==0 && strlen(cnfEdate)==0){
			 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
			 utMsgGetSomeNVar(psMsgHead, 17,
			    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			    "slogdate",    UT_TYPE_STRING,  sizeof(slogdate)-1,       slogdate,
			    "elogdate",    UT_TYPE_STRING,  sizeof(elogdate)-1,       elogdate,
			    "slogtime",    UT_TYPE_STRING,  sizeof(slogtime)-1,       slogtime,
			    "elogtime",    UT_TYPE_STRING,  sizeof(elogtime)-1,       elogtime,
					"time",        UT_TYPE_STRING,  sizeof(timeV)-1,          timeV,
					"number",      UT_TYPE_STRING,  sizeof(number)-1,         number,
					"traffic",     UT_TYPE_STRING,  sizeof(traffic)-1,        traffic,
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
			 printf("slogdate = %s\n", slogdate);
			 printf("elogdate = %s\n", elogdate);
			 printf("slogtime = %s\n", slogtime);
			 printf("elogtime = %s\n", elogtime);
			 printf("timeV = %s\n", timeV);
			 printf("number = %s\n", number);
			 printf("traffic = %s\n", traffic);
			 printf("del = %s\n", caDel);
			 printf("caUpdate = %s\n", caUpdate);
			 printf("_keyword = %s\n", _keyword);
			 printf("caExport = %s\n", caExport);
			 
			 // 读取配置文件的配置时间
			 char cnfSdate[32] = "";
			 char cnfEdate[32] = "";
			 char cnfPath[128] = "";
			 char *pConfig;
			 pasConfList *psConfig;
			 
			 pConfig = (char *)pasGetConfigFileName();
			 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
		   if(psConfig == NULL) {
		        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","读取配置文件",ncLang("0427配置文件出错"));
		        return 0;
		   }
		   //如若没有配置，默认null。
		   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig的最后一个参数为StatisticsStim的默认值
		 	 strcpy(cnfEdate,(char *)pasUtlLookConfig(psConfig,"StatisticsEtime",""));
		 	 strcpy(cnfPath,(char *)pasUtlLookConfig(psConfig,"ReportPosition",""));
		   printf("cnfSdate = %s\n", cnfSdate);
			 printf("cnfEdate = %s\n", cnfEdate);
			 printf("cnfPath = %s\n", cnfPath);
		   pasUtlFreeConfig(psConfig);
 	 }
	 
	 if(strlen(cnfSdate)>0 && strlen(cnfEdate)>0){
	 		ncTimeToLong(cnfSdate, &lSdate);
			ncTimeToLong(cnfEdate, &lEdate);
			printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	 }else{
	 		// 转换时间
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // 默认时间是今天0点--当前时间
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 char _sdate[32] = ""; // 格式转换后的开始时间
	 char _edate[32] = ""; // 格式转换后的当前时间
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 strcpy(caExport, caExport1);
	 // 导出
	 if(strlen(caExport)>0){
	 		 if(strlen(cnfPath)>0){
	 		 		strcpy(caFile, cnfPath);
	 		 }else{
	 		 		strcpy(caFilename, "intStatisticsReport.csv");
	 		 		if(strlen(cnfPath)>0){
			 		 		strcpy(caPath, cnfPath);
			 		}else{
			 		 		strcpy(caPath, FILEPATH);
			 		}
			 		sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 }
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"Internet surfing Statistics Report\n");
			 fprintf(fp,"\t\tTime:%s--%s\n", _sdate, _edate);
			 ncUtlPrintCsv(fp,5,UT_TYPE_STRING,"Time",
			 									  UT_TYPE_STRING,"Number",
			                    UT_TYPE_STRING,"Traffic(Mb)");
	 }
	 
	 for(i=0; i<24; i++){
	 		 memset(sql, 0, sizeof(sql));
			 sprintf(sql, "select count(*) as usernum, %d as ptime from ncsrvuserlog where starttime>%lu and endtime<%lu", i, lSdate, lEdate);		                    
			 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and hour(from_unixtime(starttime)) <= %d and hour(from_unixtime(endtime)) >= %d", i, i);
			 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by ptime");
			 printf("sql = %s\n", sql);
			 psCur = pasDbOpenSql(sql, 0);
			 if(psCur != NULL){
			 		 unsigned long usernum = 0;
			 		 char ptime[32] = "";
			 		 while(0 == (iret = pasDbFetchInto(psCur,
								 UT_TYPE_ULONG,  4,               &usernum,
								 UT_TYPE_STRING, sizeof(ptime)-1, ptime)) || 1405 == iret)
				  {
				  		 printf("usernum = %d\n", usernum);
				  		 stat_int[i].number = usernum;
				  }
			 }
	 		 pasDbCloseCursor(psCur);
	 }
	 
	 for(i=0; i<24; i++){
	 		 memset(sql, 0, sizeof(sql));
			 sprintf(sql, "select sum(ubytes+dbytes) as traf, %d as ptime from ncsrvuserlog where starttime>%lu and endtime<%lu", i, lSdate, lEdate);		                    
			 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and hour(from_unixtime(starttime)) <= %d and hour(from_unixtime(endtime)) >= %d", i, i);
			 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by ptime");
			 printf("sql = %s\n", sql);
			 psCur = pasDbOpenSql(sql, 0);
			 if(psCur != NULL){
			 		 unsigned long traf = 0;
			 		 char ptime[32] = "";
			 		 while(0 == (iret = pasDbFetchInto(psCur,
								 UT_TYPE_ULONG,  4,               &traf,
								 UT_TYPE_STRING, sizeof(ptime)-1, ptime)) || 1405 == iret)
				  {
				  		 stat_int[i].traffic = (double)traf/(1024*1024);
				  		 printf("traffic = %.2f\n", stat_int[i].traffic);
				  }
			 }
	 		 pasDbCloseCursor(psCur);	
	 }
	 
	 for(i=0; i<24; i++){
	 		iNum++;
  	  if(iNum > 1)
			{
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
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
      //remove(caFile);
      utPltFreeDb(psDbHead);
      return 0;
	 }
	 if(strlen(cnfSdate)==0 && strlen(cnfEdate)==0){
	 		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_intStatisticsReport_list.htm");
	 }
	 return 0;
}

// 上网设备类型报表
int ncm_web_MC_intDeviceReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char cnfSdate[32], char cnfEdate[32], char caExport1[32], char cnfPath[128])
{  
	 if(strlen(cnfSdate)==0 && strlen(cnfEdate)==0){
	 		utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 }
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 int iNumber = 0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char sdate[32] = "";
	 char edate[32] = "";
	 char slogdate[64] = "";
	 char elogdate[64] = "";
	 char slogtime[64] = "";
	 char elogtime[64] = "";
	 char dev[32] = "";
	 char devtype[32] = "";
	 char num[20] = "";
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // 当前时间(默认结束时间)
	 
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
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 if(strlen(cnfSdate)==0 && strlen(cnfEdate)==0){
			 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
			 utMsgGetSomeNVar(psMsgHead, 17,
			    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			    "slogdate",    UT_TYPE_STRING,  sizeof(slogdate)-1,       slogdate,
			    "elogdate",    UT_TYPE_STRING,  sizeof(elogdate)-1,       elogdate,
			    "slogtime",    UT_TYPE_STRING,  sizeof(slogtime)-1,       slogtime,
			    "elogtime",    UT_TYPE_STRING,  sizeof(elogtime)-1,       elogtime,
					"dev",         UT_TYPE_STRING,  sizeof(dev)-1,       		  dev,
					"devtype",     UT_TYPE_STRING,  sizeof(devtype)-1,        devtype,
					"num",         UT_TYPE_STRING,  sizeof(num)-1,            num,
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
			 printf("slogdate = %s\n", slogdate);
			 printf("elogdate = %s\n", elogdate);
			 printf("slogtime = %s\n", slogtime);
			 printf("elogtime = %s\n", elogtime);
			 printf("dev = %s\n", dev);
			 printf("devtype = %s\n", devtype);
			 printf("num = %s\n", num);
			 printf("del = %s\n", caDel);
			 printf("caUpdate = %s\n", caUpdate);
			 printf("_keyword = %s\n", _keyword);
			 
			 // 排序
		   if(strlen(sort)==0){
			 	  strcpy(sort,"starttime");
			 }
			 
			 // 读取配置文件的配置时间
			 char cnfSdate[32] = "";
			 char cnfEdate[32] = "";
			 char *pConfig;
			 pasConfList *psConfig;
			 
			 pConfig = (char *)pasGetConfigFileName();
			 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
		   if(psConfig == NULL) {
		        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","读取配置文件",ncLang("0427配置文件出错"));
		        return 0;
		   }
		   //如若没有配置，默认null。
		   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig的最后一个参数为StatisticsStim的默认值
		 	 strcpy(cnfEdate,(char *)pasUtlLookConfig(psConfig,"StatisticsEtime",""));
		   printf("cnfSdate = %s\n", cnfSdate);
			 printf("cnfEdate = %s\n", cnfEdate);
		   pasUtlFreeConfig(psConfig);
   }
	 
	 if(strlen(cnfSdate)>0 && strlen(cnfEdate)>0){
	 		ncTimeToLong(cnfSdate, &lSdate);
			ncTimeToLong(cnfEdate, &lEdate);
			printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	 }else{
	 		// 转换时间
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // 默认时间是今天0点--当前时间
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 char _sdate[32] = ""; // 格式转换后的开始时间
	 char _edate[32] = ""; // 格式转换后的当前时间
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 sprintf(sql, "select count(*) from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate);
	 if(!utStrIsSpaces(devtype)) // 根据设备类型查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and devtype like '%%%s%%'", devtype);
	 }
	 if(!utStrIsSpaces(dev)) // 根据设备名称查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev like '%%%s%%'", dev);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 strcpy(caExport, caExport1);
	 // 导出
	 if(strlen(caExport)>0){
	 		 if(strlen(cnfPath)>0){
	 		 		strcpy(caFile, cnfPath);
	 		 }else{
	 		 		strcpy(caFilename, "intDeviceReport.csv");
		 		  if(strlen(cnfPath)>0){
		 		 		 strcpy(caPath, cnfPath);
		 		  }else{
		 		 		 strcpy(caPath, FILEPATH);
		 		  }
		 		  sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 }
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
	 		 		 printf("打开文件失败！\n");
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"Device Model Report\n");
			 fprintf(fp,"\t\tTime:%s--%s\n", _sdate, _edate);
			 ncUtlPrintCsv(fp,3,UT_TYPE_STRING,"Model",
			 										UT_TYPE_STRING,"Type",
			                    UT_TYPE_STRING,"Number");
	 }
	 
	 sprintf(sql, "select count(*) as lCount2, dev, devtype from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate);		                    
	 if(!utStrIsSpaces(devtype)) // 根据设备类型查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and devtype like '%%%s%%'", devtype);
	 }
	 if(!utStrIsSpaces(dev)) // 根据设备名称查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dev like '%%%s%%'", dev);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by devtype");
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL){
	 		 unsigned long lCount2 = 0;
	 		 char dev[24] = "";
	 		 char date[32] = "";
	 		 char devtype[8] = "";
	 		 while(0 == (iret = pasDbFetchInto(psCur,
						 UT_TYPE_ULONG,  4,                   &lCount2,
						 UT_TYPE_STRING, sizeof(dev)-1,  			dev,
						 UT_TYPE_STRING, sizeof(devtype)-1,  	devtype)) || 1405 == iret)
		  {
		  	   iNum++;
		  	   if(iNum > 1)
					 {
						 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
					 }
					 
					 if(strlen(devtype)==0){
					 		strcpy(devtype, "other");	
					 }
					 if(strlen(dev)==0){
					 		strcpy(dev, "other");	
					 }
					 utPltSetCvtHtml(1);
					 utPltPutLoopVar(psDbHead,"dev",iNum,dev);
					 utPltPutLoopVar(psDbHead,"devtype",iNum,devtype);
					 utPltSetCvtHtml(0);
					 utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",lCount2);
					 if(strlen(caExport)>0){
					 		ncUtlPrintCsv(fp,3,UT_TYPE_STRING,dev,
					 											UT_TYPE_STRING,devtype,
														 		UT_TYPE_ULONG,lCount2);
					 }
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
	 if(strlen(cnfSdate)==0 && strlen(cnfEdate)==0){
	 		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_intDeviceReport_list.htm");
	 }
	 return 0;
}

// 短信发送详细报表
int ncm_web_MC_smsDetailReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char cnfSdate[32], char cnfEdate[32], char caExport1[32], char cnfPath[128])
{
	 if(strlen(cnfSdate)==0 && strlen(cnfEdate)==0){
	 		utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 }
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 int iNumber = 0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
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
	 
	 char caContrycode[32] = "";
	 char caUsername[32] = "";
	 char caLogtime[64] = "";
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // 当前时间(默认结束时间)
	 
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
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 if(strlen(cnfSdate)==0 && strlen(cnfEdate)==0){
			 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
			 utMsgGetSomeNVar(psMsgHead, 21,
			    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
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
			 printf("slogdate = %s\n", slogdate);
			 printf("elogdate = %s\n", elogdate);
			 printf("slogtime = %s\n", slogtime);
			 printf("elogtime = %s\n", elogtime);
			 printf("contrycode = %s\n", contrycode);
			 printf("username = %s\n", username);
			 printf("usermac = %s\n", usermac);
			 printf("ip = %s\n", ip);
			 printf("logtime = %s\n", logtime);
			 printf("del = %s\n", caDel);
			 printf("caUpdate = %s\n", caUpdate);
			 printf("_keyword = %s\n", _keyword);
			 
			 // 排序
		   if(strlen(sort)==0){
			 	  strcpy(sort,"logtime");
			 }
			 
			 // 读取配置文件的配置时间
			 char cnfSdate[32] = "";
			 char cnfEdate[32] = "";
			 char cnfPath[128] = "";
			 char *pConfig;
			 pasConfList *psConfig;
			 
			 pConfig = (char *)pasGetConfigFileName();
			 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
		   if(psConfig == NULL) {
		        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","读取配置文件",ncLang("0427配置文件出错"));
		        return 0;
		   }
		   //如若没有配置，默认null。
		   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig的最后一个参数为StatisticsStim的默认值
		 	 strcpy(cnfEdate,(char *)pasUtlLookConfig(psConfig,"StatisticsEtime",""));
		 	 strcpy(cnfPath,(char *)pasUtlLookConfig(psConfig,"ReportPosition",""));
		   printf("cnfSdate = %s\n", cnfSdate);
			 printf("cnfEdate = %s\n", cnfEdate);
			 printf("cnfPath = %s\n", cnfPath);
		   pasUtlFreeConfig(psConfig);
   }
	 
	 if(strlen(cnfSdate)>0 && strlen(cnfEdate)>0){
	 		ncTimeToLong(cnfSdate, &lSdate);
			ncTimeToLong(cnfEdate, &lEdate);
			printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	 }else{
	 		// 转换时间
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // 默认时间是今天0点--当前时间
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 char _sdate[32] = ""; // 格式转换后的开始时间
	 char _edate[32] = ""; // 格式转换后的当前时间
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 sprintf(sql, "select count(*) from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);
	 if(!utStrIsSpaces(contrycode)) // 根据国家代码查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and contrycode like '%%%s%%'", contrycode);
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
	 strcpy(caExport, caExport1);
	 // 导出
	 if(strlen(caExport)>0){
	 		 if(strlen(cnfPath)>0){
	 		 		strcpy(caFile, cnfPath);
	 		 }else{
	 		 		strcpy(caFilename, "smsDetailReport.csv");
		 		  if(strlen(cnfPath)>0){
		 		 		 strcpy(caPath, cnfPath);
		 		  }else{
		 		 		 strcpy(caPath, FILEPATH);
		 		  }
		 		  sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 }
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"SMS Detail report\n");
			 fprintf(fp,"\t\tTime:%s--%s\n", _sdate, _edate);
			 ncUtlPrintCsv(fp,7,UT_TYPE_STRING,"Country Code",
	 									  UT_TYPE_STRING,"Mobile Phone",
	                    UT_TYPE_STRING,"MAC",
	                    UT_TYPE_STRING,"IP",
	                    UT_TYPE_STRING,"Send Time",
	                    UT_TYPE_STRING,"Operator type",
	                    UT_TYPE_STRING,"Zone Name");
	 }
	 
	 if((lEdate-lSdate)<=86400){ // 一天之间, 时间显示(13:23:23)
	 		sprintf(sql, "select username, usermac, ip, date_format(from_unixtime(logtime), '%T') as time, sptype, contrycode, areaname from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);
	 }else{ // 超过一天, 时间显示(2013/02/12 13:23:23)
	 		sprintf(sql, "select username, usermac, ip, from_unixtime(logtime) as time, sptype, contrycode, areaname from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);	                    
	 }
	 if(!utStrIsSpaces(contrycode)) // 根据区域查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and contrycode like '%%%s%%'", contrycode);
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
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username != 'Guest'");
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by sptype, areaname");
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " limit %lu, %lu", iStart, iLimit < lCount1 ? iLimit : lCount1);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL){
	 		 char username[32] = "";
	 		 char usermac[18] = "";
	 		 char ip[16] = "";
	 		 char time[32] = "";
	 		 unsigned long sptype = 0;
	 		 unsigned long contrycode = 0;
	 		 char areaname[32] = "";
	 		 char caSptype[32] = "";
	 		 
	 		 while(0 == (iret = pasDbFetchInto(psCur,
				 UT_TYPE_STRING, sizeof(username)-1,  	username,
				 UT_TYPE_STRING, sizeof(usermac)-1,  	  usermac,
				 UT_TYPE_STRING, sizeof(ip)-1,  	      ip,
				 UT_TYPE_STRING, sizeof(time)-1,  			time,
				 UT_TYPE_ULONG,  4,  	                  &sptype,
				 UT_TYPE_ULONG,  4,  	                  &contrycode,
				 UT_TYPE_STRING, sizeof(areaname)-1,  	areaname)) || 1405 == iret)
		  {
		  		iNum++;
	  	    if(iNum > 1)
				  {
					   utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
				  }
				  
		  		switch(sptype){
		  				case 0:strcpy(caSptype, "Illegal Phone Number");break;
		  				case 1:strcpy(caSptype, "China Mobile");break;
		  				case 2:strcpy(caSptype, "China Unicom");break;	
		  				case 3:strcpy(caSptype, "China Telecom");break;
		  				case 8:strcpy(caSptype, "Other");break;
		  				case 9:strcpy(caSptype, "International");break;
		  				default:break;
		  		}
			 		
			 		utPltPutLoopVarF(psDbHead,"contrycode",iNum,"%lu",contrycode);
				  utPltPutLoopVar(psDbHead,"sptype",iNum,caSptype);
				  utPltSetCvtHtml(1);
				  utPltPutLoopVar(psDbHead,"areaname",iNum,areaname);
				  utPltPutLoopVar(psDbHead,"username",iNum,username);
				  utPltPutLoopVar(psDbHead,"usermac",iNum,usermac);
				  utPltPutLoopVar(psDbHead,"ip",iNum,ip);
				  utPltSetCvtHtml(0);
				  utPltPutLoopVar(psDbHead,"logtime",iNum,time);
				  if(strlen(caExport)>0){
					 		ncUtlPrintCsv(fp,7,UT_TYPE_ULONG,contrycode,
			 											UT_TYPE_STRING,username,
												 		UT_TYPE_STRING,usermac,
												 		UT_TYPE_STRING,ip,
	                          UT_TYPE_STRING,time,
	                          UT_TYPE_STRING,caSptype,
	                          UT_TYPE_STRING,areaname);
				  }
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
	 if(strlen(cnfSdate)==0 && strlen(cnfEdate)==0){
	 		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_smsDetailReport_list.htm");
	 }
	 return 0;
}

// 每小时发送短信报表 
int ncm_web_MC_smsStatisticsReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char cnfSdate[32], char cnfEdate[32], char caExport1[32], char cnfPath[128])
{
	 if(strlen(cnfSdate)==0 && strlen(cnfEdate)==0){
	 		utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 }
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 int iNumber = 0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char sdate[32] = "";
	 char edate[32] = "";
	 char slogdate[64] = "";
	 char elogdate[64] = "";
	 char slogtime[64] = "";
	 char elogtime[64] = "";
	 char timeV[32] = "";
	 char localNum[32] = "";
	 char interNum[32] = "";
	 
	 unsigned long lSdate = 0;
	 unsigned long lEdate = time(0); // 当前时间(默认结束时间)
	 
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
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 创建纪录信息结构体
	 int i = 0;
	 typedef struct sms_stat{
	 		int num;
	 		char timeV[18];
	 		long localNum;
	 		long interNum;
	 }sms_stat_s;
	 sms_stat_s stat_sms[24];
	 
	 for (i = 0;i < 24;i++){ // 初始化
		 	 stat_sms[i].num = i;
		   sprintf(stat_sms[i].timeV,"%02d:00:00-%02d:59:59",i,i);
		   stat_sms[i].localNum = 0;
		   stat_sms[i].interNum = 0;
		   //printf("num = %d timeV = %s localNum = %d interNum = %d \n",stat_sms[i].num,stat_sms[i].timeV,stat_sms[i].localNum,stat_sms[i].interNum);	
	 }
	 
	 if(strlen(cnfSdate)==0 && strlen(cnfEdate)==0){
			 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
			 utMsgGetSomeNVar(psMsgHead, 17,
			    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			    "slogdate",    UT_TYPE_STRING,  sizeof(slogdate)-1,       slogdate,
			    "elogdate",    UT_TYPE_STRING,  sizeof(elogdate)-1,       elogdate,
			    "slogtime",    UT_TYPE_STRING,  sizeof(slogtime)-1,       slogtime,
			    "elogtime",    UT_TYPE_STRING,  sizeof(elogtime)-1,       elogtime,
					"timeV",       UT_TYPE_STRING,  sizeof(timeV)-1,          timeV,
					"localNum",    UT_TYPE_STRING,  sizeof(localNum)-1,       localNum,
					"interNum",    UT_TYPE_STRING,  sizeof(interNum)-1,       interNum,
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
			 printf("slogdate = %s\n", slogdate);
			 printf("elogdate = %s\n", elogdate);
			 printf("slogtime = %s\n", slogtime);
			 printf("elogtime = %s\n", elogtime);
			 printf("timeV = %s\n", timeV);
			 printf("localNum = %s\n", localNum);
			 printf("interNum = %s\n", interNum);
			 printf("del = %s\n", caDel);
			 printf("caUpdate = %s\n", caUpdate);
			 printf("_keyword = %s\n", _keyword);
			 
			 // 读取配置文件的配置时间
			 char cnfSdate[32] = "";
			 char cnfEdate[32] = "";
			 char cnfPath[128] = "";
			 char *pConfig;
			 pasConfList *psConfig;
			 
			 pConfig = (char *)pasGetConfigFileName();
			 psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
		   if(psConfig == NULL) {
		        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","读取配置文件",ncLang("0427配置文件出错"));
		        return 0;
		   }
		   //如若没有配置，默认null。
		   strcpy(cnfSdate,(char *)pasUtlLookConfig(psConfig,"StatisticsStime","")); // pasUtlLookConfig的最后一个参数为StatisticsStim的默认值
		 	 strcpy(cnfEdate,(char *)pasUtlLookConfig(psConfig,"StatisticsEtime",""));
		 	 strcpy(cnfPath,(char *)pasUtlLookConfig(psConfig,"ReportPosition",""));
		   printf("cnfSdate = %s\n", cnfSdate);
			 printf("cnfPath = %s\n", cnfPath);
		   pasUtlFreeConfig(psConfig);
   }
	 
	 if(strlen(cnfSdate)>0 && strlen(cnfEdate)>0){
	 		ncTimeToLong(cnfSdate, &lSdate);
			ncTimeToLong(cnfEdate, &lEdate);
			printf("lEdate-lSdate = %d\n", lEdate-lSdate);
	 }else{
	 		// 转换时间
		  sprintf(sdate, "%s %s", slogdate, slogtime);
		  sprintf(edate, "%s %s", elogdate, elogtime);
		  
		  if(strlen(sdate) > 1 && strlen(edate) > 1){
				 ncTimeToLong(sdate, &lSdate);
				 ncTimeToLong(edate, &lEdate);
				 printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }else{
		 	   ncDaysToLong(lEdate, 1, &lSdate); // 默认时间是今天0点--当前时间
		 	   printf("lEdate-lSdate = %d\n", lEdate-lSdate);
		  }
	 }
	 
	 char _sdate[32] = ""; // 格式转换后的开始时间
	 char _edate[32] = ""; // 格式转换后的当前时间
	 strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	 strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	 printf("_sdate = %s\n", _sdate);
	 printf("_edate = %s\n", _edate);
	 
	 sprintf(sql, "select count(*) from ncsrvsmslog where logtime>%lu and logtime<%lu",lSdate, lEdate);
	  
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 strcpy(caExport, caExport1);
	 // 导出
	 if(strlen(caExport)>0){
	 	   if(strlen(cnfPath)>0){
	 	   		strcpy(caFile, cnfPath);
	 	   }else{
	 	   		strcpy(caFilename, "smsStatisticsReport.csv");
		 		  if(strlen(cnfPath)>0){
		 		 		 strcpy(caPath, cnfPath);
		 		  }else{
		 		 		 strcpy(caPath, FILEPATH);
		 		  }
		 		  sprintf(caFile, "%s/%s", caPath, caFilename);
	 	   }
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"SMS Statistics Report\n");
			 fprintf(fp,"\t\tTime:%s--%s\n", _sdate, _edate);
			 ncUtlPrintCsv(fp,5,UT_TYPE_STRING,"Time",
			 									  UT_TYPE_STRING,"Count of Local SMS",
			                    UT_TYPE_STRING,"Count of International SMS");
	 }
	 
	 memset(sql,0,sizeof(sql)); // 国内短信统计
	 sprintf(sql, "select count(*) as smsnum, hour(from_unixtime(logtime)) as htime from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);		                    
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (contrycode = '086' or contrycode = '86')");
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by htime");
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " limit %lu, %lu", iStart, iLimit < lCount1 ? iLimit : lCount1);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL){
	 		 unsigned long smsnum = 0;
	 		 char htime[24] = "";
	 		 while(0 == (iret = pasDbFetchInto(psCur,
						 UT_TYPE_ULONG,  4,                   &smsnum,
						 UT_TYPE_STRING, sizeof(htime)-1,  		htime)) || 1405 == iret)
		  {
		  	   printf("smsnum = %d, htime = %s\n", smsnum, htime);
		  	   stat_sms[atol(htime)].localNum = smsnum;
		  }
	 }
	 pasDbCloseCursor(psCur);
	 
	 memset(sql,0,sizeof(sql)); // 国际短信统计
	 sprintf(sql, "select count(*) as smsnum, hour(from_unixtime(logtime)) as htime from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);		                    
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (contrycode != '086' and contrycode != '86')");
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by htime");
	 if(strlen(caExport)==0){
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " limit %lu, %lu", iStart, iLimit < lCount1 ? iLimit : lCount1);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL){
	 		 unsigned long smsnum = 0;
	 		 char htime[24] = "";
	 		 while(0 == (iret = pasDbFetchInto(psCur,
						 UT_TYPE_ULONG,  4,                   &smsnum,
						 UT_TYPE_STRING, sizeof(htime)-1,  		htime)) || 1405 == iret)
		  {
		  	   printf("smsnum = %d, htime = %s\n", smsnum, htime);
		  	   stat_sms[atol(htime)].interNum = smsnum;
		  }
	 }
	 pasDbCloseCursor(psCur);
	 
	 for(i=0; i<24; i++){
	 		iNum++;
	    if(iNum > 1)
		  {
			   utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
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
      //remove(caFile);
      utPltFreeDb(psDbHead);
      return 0;
	 }
	 if(strlen(cnfSdate)==0 && strlen(cnfEdate)==0){
	 		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_userman/ncm_web_MC_smsStatisticsReport_list.htm");
	 }
	 return 0;
}


// 操作系统图表
int ncm_web_MC_os_chart(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
	  sprintf(sql,"select count(*) from ncsrvuserlog where starttime >= %lu and endtime <= %lu", lsdate, ledate);
		pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	  utPltPutVarF(psDbHead,"TotRec","%lu", lCount1);
	  
	  // 导出
	 if(strlen(expexcel)>0){
			 strcpy(caFilename, "OSchart.csv");
			 strcpy(caPath, FILEPATH);
			 sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"OS Recount\n");
	 		 fprintf(fp,"From: %s -- To: %s\n",sdate,edate);
			 ncUtlPrintCsv(fp,4,UT_TYPE_STRING,"No.",
			 									  UT_TYPE_STRING,"OS",
		                      UT_TYPE_STRING,"Number",
								          UT_TYPE_STRING,"Percentage");
	 }
	  
	  sprintf(sql,"select os, count(os) as number from ncsrvuserlog where starttime >= %lu and endtime <= %lu", lsdate, ledate);	
	  snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by os order by number desc");
		printf("sql = %s\n", sql);
		psCur = pasDbOpenSql(sql,0);
		if(psCur != NULL)
		{
				char chart_name[32] = "";
				unsigned long chart_number = 0;
				double rate=0;
				double total = lCount1;
				char rates[32]="";
				while(0==(iReturn = pasDbFetchInto(psCur,
      	      UT_TYPE_STRING,sizeof(chart_name),chart_name,
      	      UT_TYPE_LONG,4,&chart_number))  || 1405==iReturn)
      	{
      			iNum++;
						if(iNum > 1){
								utPltPutLoopVar(psDbHead,"dh",iNum,",");
						}
						if(strlen(chart_name)==0){
								strcpy(chart_name, "Other");	
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
		pasDbCloseCursor(psCur);
		utPltShowDb(psDbHead);
		
		if(strlen(expexcel)>0){
	 			fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        //remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	}
	 
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_userman/ncm_web_MC_os_chart.htm");
	  return 0;
}

int Ncm_MaintainCenterSWW_SetFunName(){
    pasSetTcpFunName("ncm_web_MC_blackuser_1", ncm_web_MC_blackuser_1, 0);  // 管理中心黑名单
    
    pasSetTcpFunName("ncm_web_MC_smslog_1", ncm_web_MC_smslog_1, 0);  // 管理中心短信注册-0212
    pasSetTcpFunName("ncm_web_MC_userlog_list", ncm_web_MC_userlog_list, 0);  // 管理中心用户日志-0212
    pasSetTcpFunName("ncm_web_MC_unitinfo_list", ncm_web_MC_unitinfo_list, 0);  // 管理中心单位信息-0212

		pasSetTcpFunName("ncm_web_MC_intDetailReport_list_1", ncm_web_MC_intDetailReport_list_1, 0);  // 用户详细上网报表-0214
		pasSetTcpFunName("ncm_web_MC_intStatisticsReport_list_1", ncm_web_MC_intStatisticsReport_list_1, 0);  // 每个小时上网人数和上网流量报表-0214
		pasSetTcpFunName("ncm_web_MC_intDeviceReport_list_1", ncm_web_MC_intDeviceReport_list_1, 0);  // 上网设备类型报表-0214
		pasSetTcpFunName("ncm_web_MC_smsDetailReport_list_1", ncm_web_MC_smsDetailReport_list_1, 0);  // 短信发送详细报表-0214
		pasSetTcpFunName("ncm_web_MC_smsStatisticsReport_list_1", ncm_web_MC_smsStatisticsReport_list_1, 0);  // 每个小时发送短信报表-0214
		
		pasSetTcpFunName("ncm_web_MC_intDetailReport_list", ncm_web_MC_intDetailReport_list, 0);  // 用户详细上网报表-0225
		pasSetTcpFunName("ncm_web_MC_intStatisticsReport_list", ncm_web_MC_intStatisticsReport_list, 0);  // 每个小时上网人数和上网流量报表-0225
		pasSetTcpFunName("ncm_web_MC_intDeviceReport_list", ncm_web_MC_intDeviceReport_list, 0);  // 上网设备类型报表-0225
		pasSetTcpFunName("ncm_web_MC_smsDetailReport_list", ncm_web_MC_smsDetailReport_list, 0);  // 短信发送详细报表-0225
		pasSetTcpFunName("ncm_web_MC_smsStatisticsReport_list", ncm_web_MC_smsStatisticsReport_list, 0);  // 每个小时发送短信报表-0225
		
		pasSetTcpFunName("ncm_web_MC_groupinfo_list", ncm_web_MC_groupinfo_list, 0); // 组信息-0226
		
		pasSetTcpFunName("ncm_web_MC_os_chart", ncm_web_MC_os_chart, 0); // 操作系统图表-0304
		
		return 0;
}
