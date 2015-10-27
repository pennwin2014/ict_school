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

int ncUtlPrintCsv(FILE *fp,int iSum,...)
{
    va_list ap;
    int iType,i;
    long long llValue;
    unsigned long lUvalue;
    long lValue;
    double fValue;
    char *pValue;
   
    va_start(ap,iSum);
    for(i=0;i<iSum;i++) {
        iType = va_arg(ap,int);
        if(i>0) {
            switch(iType) {
                case UT_TYPE_LONG8:
                    llValue = va_arg(ap, long8);
                    fprintf(fp,",%lld",llValue);
                    break;
                case UT_TYPE_LONG:
                    lValue = va_arg(ap, long);
                    fprintf(fp,",%ld",lValue);
                    break;
                case UT_TYPE_ULONG:
                    lUvalue = va_arg(ap, unsigned long);
                    fprintf(fp,",%lu",lUvalue);
                    break;
                case UT_TYPE_FLOAT:
                    fValue = va_arg(ap, double);
                    fprintf(fp,",%10.2f",fValue);
                    break;
                case UT_TYPE_STRING:
                    pValue = va_arg(ap, char *);
                    fprintf(fp,",\"%s\"",pValue);
                    break;
            }
        }
        else {
            switch(iType) {
                case UT_TYPE_LONG8:
                    llValue = va_arg(ap, long8);
                    fprintf(fp,"%lld",llValue);
                    break;
                case UT_TYPE_LONG:
                    lValue = va_arg(ap, long);
                    fprintf(fp,"%ld",lValue);
                    break;
                case UT_TYPE_ULONG:
                    lUvalue = va_arg(ap, unsigned long);
                    fprintf(fp,"%lu",lUvalue);
                    break;
                case UT_TYPE_FLOAT:
                    fValue = va_arg(ap, double);
                    fprintf(fp,"%10.2f",fValue);
                    break;
                case UT_TYPE_STRING:
                    pValue = va_arg(ap, char *);
                    fprintf(fp,"\"%s\"",pValue);
                    break;
            }
        }
    }
    fprintf(fp,"\r\n");
    va_end(ap);
}


int ncTimeToLong(char *stime, unsigned long *lTime);
/*{
	 char ctime[20] = "";
	 
	 snprintf(ctime, sizeof(ctime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", stime[0], stime[1], stime[2], stime[3], stime[5], stime[6], stime[8], stime[9], stime[11], stime[12], stime[14], stime[15], stime[17], stime[18]);	
	 *lTime = utTimStrToLong("%Y%m%d%H%M%S", ctime);
	 
	 return 0;
}*/

int ncNumToTime(unsigned long lTime, char *cTime)
{
      char day[11] = ""; 
      char hour[11] = "";
      char min[11] = "";
      char secd[11] = "";
      snprintf(day, sizeof(day), "%lu", lTime/86400);
      snprintf(hour, sizeof(hour), "%lu", (lTime%86400)/3600);
      snprintf(min, sizeof(min), "%lu", ((lTime%86400)%3600)/60);
      snprintf(secd, sizeof(secd), "%lu", (((lTime%86400)%3600)%60)/60);
  
      strcat(cTime, strcat(day, "天"));
      strcat(hour, "小时");
      strcat(min, "分");
      strcat(secd, "秒");
      strcat(cTime, hour);
      strcat(cTime, min);
      strcat(cTime, secd); 
       
      return 0;  
}

int ncm_userlog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char sid[20] = "";
	 char portalname[16] = "";
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
	 unsigned long lStartTime = 0;
	 unsigned long lEndTime = 0;
	 unsigned long lConnTime = 0;
	 
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
	 utMsgGetSomeNVar(psMsgHead, 28,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
			"portalname",  UT_TYPE_STRING,  sizeof(portalname)-1,     portalname,
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
	 printf("portalname = %s\n", portalname);
	 printf("userid = %s\n", userid);
	 printf("username = %s\n", username);
	 printf("dispname = %s\n", dispname);
	 printf("ssid = %s\n",ssid);
	 printf("apname = %s\n", apname);
	 printf("mac = %s\n", mac);
	 printf("ip = %s\n", ip);
	 printf("dev = %s\n", dev);
	 printf("os = %s\n", os);
	 printf("devtype = %s\n", devtype);
	 printf("ubytes = %s\n", ubytes);
	 printf("dbytes = %s\n", dbytes);
	 printf("starttime = %s\n", starttime);
	 printf("endtime = %s\n", endtime);
	 printf("cause = %s\n", cause);
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
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
   
   // 时间转换
   ncTimeToLong(starttime, &lStartTime);
   ncTimeToLong(endtime, &lEndTime);
   if(lStartTime>lEndTime){
   		lConnTime = 0;	
   }else{
   		lConnTime = lEndTime - lStartTime;
   }
   
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
			if (strlen(sid)>0){
	 				sprintf(caTemp,"update ncsrvuserlog set portalname='%s',userid=%lu,username='%s',dispname='%s',ssid='%s',apname='%s',mac='%s',ip='%s',dev='%s',os='%s',devtype='%s',ubytes=%lu,dbytes=%lu,starttime=%lu,endtime=%lu,conntime=%lu,cause=%lu where sid=%lu ",portalname,atol(userid),username,dispname,ssid,apname,mac,ip,dev,os,devtype,atol(ubytes),atol(dbytes),lStartTime,lEndTime,lConnTime,atol(cause),atol(sid));
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  } else {
		    	sprintf(caTemp,"insert into ncsrvuserlog (portalname,userid,username,dispname,ssid,apname,mac,ip,dev,os,devtype,ubytes,dbytes,starttime,endtime,conntime,cause) values ('%s',%lu,'%s','%s','%s','%s','%s','%s','%s','%s','%s',%lu,%lu,%lu,%lu,%lu,%lu)",portalname,atol(userid),username,dispname,ssid,apname,mac,ip,dev,os,devtype,atol(ubytes),atol(dbytes),lStartTime,lEndTime,lConnTime,atol(cause));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 	  memset(sid, 0, sizeof(sid));
	 	  memset(portalname, 0, sizeof(portalname));
	 	  memset(userid, 0, sizeof(userid));
	 	  memset(username, 0, sizeof(username));
	 	  memset(dispname, 0, sizeof(dispname));
	 	  memset(ssid, 0, sizeof(ssid));
	 	  memset(apname, 0, sizeof(apname));
	 	  memset(mac, 0, sizeof(mac));
	 	  memset(ip, 0, sizeof(ip));
	 	  memset(dev, 0, sizeof(dev));
	 	  memset(os, 0, sizeof(os));
	 	  memset(devtype, 0, sizeof(devtype));
	 	  memset(ubytes, 0, sizeof(ubytes));
	 	  memset(dbytes, 0, sizeof(dbytes));
	 	  memset(starttime, 0, sizeof(starttime));
	 	  memset(endtime, 0, sizeof(endtime));
	 	  memset(conntime, 0, sizeof(conntime));
	 	  memset(cause, 0, sizeof(cause));
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from ncsrvuserlog where sid in(%s)",caSesid);
    	}
	 }	 
	 // 查询
	 strcpy(sql, "select count(*) from ncsrvuserlog where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (portalname like '%%%s%%' or username like '%%%s%%') ",  _keyword, _keyword);
	 }
	 /*if(!utStrIsSpaces(username))  // 用户名
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
	 }*/
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 //导出
	 /*unsigned long lCount2 = 0; //internation sms
	 unsigned long lCount3 = 0; //local sms
	 int count1[24] = {0}; //internation sms
	 char time1[24][32] = {""};
	 int num = 0; 
	 int count2[24] = {0}; //local sms
	 char time2[24][32] = {""};*/
	
	 if(strlen(caExport)>0){
	 		 strcpy(caFilename, "userlog.csv");
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
	 		 
	 		 fprintf(fp,"设备类型报告\n");
			 ncUtlPrintCsv(fp,3,UT_TYPE_STRING,"设备",
			                    UT_TYPE_STRING,"类型",
			                    UT_TYPE_STRING,"数量");
			 
			 sprintf(caTemp1, "select count(*) as lCount2, dev, devtype from ncsrvuserlog group by dev, devtype");
			 printf("caTemp1 = %s\n", caTemp1);
			 psCur = pasDbOpenSql(caTemp1, 0);
			 if(psCur != NULL){
			 		 unsigned long lCount2 = 0;
			 		 char dev[24] = "";
			 		 char devtype[8] = "";
			 		 while(0 == (iret = pasDbFetchInto(psCur,
						 UT_TYPE_ULONG,  4,                   &lCount2,
						 UT_TYPE_STRING, sizeof(dev)-1,  			dev,
						 UT_TYPE_STRING, sizeof(devtype)-1,  	devtype)) || 1405 == iret)
				  {
				  	 printf("lCount2 = %d\n", lCount2);
				  	 printf("dev = %s\n", dev);
				  	 printf("dentype = %s\n", devtype);
				  	 ncUtlPrintCsv(fp,3,UT_TYPE_STRING,dev,
			                          UT_TYPE_STRING,devtype,
			                          UT_TYPE_LONG,lCount2);	
				  }
			 }
			 pasDbCloseCursor(psCur); 
			 
			 /*fprintf(fp,"上网统计报告\n");
			 ncUtlPrintCsv(fp,3,UT_TYPE_STRING,"时间",
			                    UT_TYPE_STRING,"数量",
			                    UT_TYPE_STRING,"流量");
			                    
			 sprintf(caTemp1, "select count(*) as lCount2, sum(ubytes) as u, sum(dbytes) as d, date_format(from_unixtime(floor(conntime/3600)*3600), '%H') as sms from ncsrvuserlog group by sms");
			 printf("caTemp1 = %s\n", caTemp1);
			 psCur = pasDbOpenSql(caTemp1, 0);
			 if(psCur != NULL){
			 		 unsigned long lCount2 = 0;
			 		 char dev[24] = "";
			 		 char devtype[8] = "";
			 		 while(0 == (iret = pasDbFetchInto(psCur,
						 UT_TYPE_ULONG,  4,                   &lCount2,
						 UT_TYPE_STRING, sizeof(dev)-1,  			dev,
						 UT_TYPE_STRING, sizeof(devtype)-1,  	devtype)) || 1405 == iret)
				  {
				  	 printf("lCount2 = %d\n", lCount2);
				  	 printf("dev = %s\n", dev);
				  	 printf("dentype = %s\n", devtype);
				  	 ncUtlPrintCsv(fp,3,UT_TYPE_STRING,dev,
			                          UT_TYPE_STRING,devtype,
			                          UT_TYPE_LONG,lCount2);	
				  }
			 }
			 pasDbCloseCursor(psCur); */
			 
			 fprintf(fp,"上网详细报告\n");
			 ncUtlPrintCsv(fp,6,UT_TYPE_STRING,"手机号码",
			                    UT_TYPE_STRING,"终端",
			                    UT_TYPE_STRING,"操作类型",
			                    UT_TYPE_STRING,"开始时间",
			                    UT_TYPE_STRING,"结束时间",
			                    UT_TYPE_STRING,"流量");
			                    
			 sprintf(caTemp1, "select username, dev, os, date_format(from_unixtime(floor(starttime/3600)*3600), '%T') as start, date_format(from_unixtime(floor(endtime/3600)*3600), '%T') as end, sum(ubytes) + sum(dbytes) as bytes from ncsrvuserlog group by username");
			 printf("caTemp1 = %s\n", caTemp1);
			 psCur = pasDbOpenSql(caTemp1, 0);
			 if(psCur != NULL){
			 	   char username[32] = "";
			 	   char dev[24] = "";
			 	   char os[16] = "";
			 	   char start[32] = "";
			 	   char end[32] = "";
			 		 unsigned long bytes = 0;
			 		 while(0 == (iret = pasDbFetchInto(psCur,
			 		   UT_TYPE_STRING, sizeof(username)-1,  username,
			 		   UT_TYPE_STRING, sizeof(dev)-1,  			dev,
			 		   UT_TYPE_STRING, sizeof(os)-1,  			os,
			 		   UT_TYPE_STRING, sizeof(start)-1,  		start,
			 		   UT_TYPE_STRING, sizeof(end)-1,  			end,
						 UT_TYPE_ULONG,  4,                   &bytes)) || 1405 == iret)
				  {
				  	 ncUtlPrintCsv(fp,6,UT_TYPE_STRING,username,
			                          UT_TYPE_STRING,dev,
			                          UT_TYPE_STRING,os,
			                          UT_TYPE_STRING,start,
			                          UT_TYPE_STRING,end,
			                          UT_TYPE_LONG,bytes);	
				  }
			 }
			 pasDbCloseCursor(psCur);
	 }
	 
	 strcpy(sql, "select sid, portalname, userid, username, dispname, ssid, apname, mac, ip, dev, os, devtype, ubytes, dbytes, starttime, endtime, conntime, cause from ncsrvuserlog where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (portalname like '%%%s%%' or username like '%%%s%%') ", _keyword, _keyword);
	 }
	 /*if(!utStrIsSpaces(username))  // 用户名
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
	 }*/
	 
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL) {
				unsigned long	sid = 0;
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
				   ncNumToTime(conntime, s_conntime);
				  
					 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
					 utPltPutLoopVar(psDbHead,"portalname",iNum,portalname);
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
					 
					 memset(s_conntime, 0, sizeof(s_conntime));
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
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmlog/ncm_userlog_list.htm");
	  return 0;	
}

