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

// 广告日志
int ncm_web_Place_advertisement_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
	 
	 // 接收信息变量
	 char sid[20] = "";
	 char refer[128] = "";
	 char url[128] = "";
	 char ncname[36] = "";
	 char username[32] = "";
	 char sip[16] = "";
	 char mac[40] = "";
	 char adcod[32] = "";
	 char apname[32] = "";
	 char apssid[32] = "";
	 char phtype[8] = "";
	 char os[16] = "";
	 char sptype[8] = "";
	 char devtype[32] = "";
	 char browser[32] = "";
	 char areacode[20] = "";
	 char areaname[32] = "";
	 char countrycode[20] = "";
	 char lasttime[20] = "";
	 char lcount[20] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long lSid = 0;
	 char caRefer[128] = "";
	 char caUrl[128] = "";
	 char caNcname[36] = "";
	 char caUsername[32] = "";
	 char caSip[16] = "";
	 char caMac[40] = "";
	 char caAdcod[32] = "";
	 char caApname[32] = "";
	 char caApssid[32] = "";
	 unsigned long lPhtype = 0;
	 char caOs[16] = "";
	 unsigned long lSptype = 0;
	 char caDevtype[32] = "";
	 char caBrowser[32] = "";
	 unsigned long lAreacode = 0;
	 char caAreaname[32] = "";
	 unsigned long lCountrycode = 0;
	 unsigned long lLasttime = 0;
	 unsigned long lLcount = 0;
	 char s_lasttime[32] = "";
	 char s_phtype[32] = "";
	 char s_sptype[32] = "";
	 
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
	 utMsgGetSomeNVar(psMsgHead, 35,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
	    "time_flag",   UT_TYPE_STRING,  sizeof(time_flag)-1,      time_flag,
	    "sdate",       UT_TYPE_STRING,  sizeof(sdate)-1,          sdate,
	    "edate",       UT_TYPE_STRING,  sizeof(edate)-1,          edate,
			"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
			"refer",       UT_TYPE_STRING,  sizeof(refer)-1,          refer,
			"url",         UT_TYPE_STRING,  sizeof(url)-1,            url,
			"ncname",      UT_TYPE_STRING,  sizeof(ncname)-1,         ncname,
			"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
			"sip",         UT_TYPE_STRING,  sizeof(sip)-1,            sip,
			"mac",     		 UT_TYPE_STRING,  sizeof(mac)-1,            mac,
			"adcod",     	 UT_TYPE_STRING,  sizeof(adcod)-1,          adcod,
			"apname",      UT_TYPE_STRING,  sizeof(apname)-1,         apname,
			"apssid",    	 UT_TYPE_STRING,  sizeof(apssid)-1,         apssid,
			"phtype",      UT_TYPE_STRING,  sizeof(phtype)-1,         phtype,
			"os",          UT_TYPE_STRING,  sizeof(os)-1,             os,
			"sptype",      UT_TYPE_STRING,  sizeof(sptype)-1,         sptype,
			"devtype",     UT_TYPE_STRING,  sizeof(devtype)-1,        devtype,
			"browser",     UT_TYPE_STRING,  sizeof(browser)-1,        browser,
			"areacode",    UT_TYPE_STRING,  sizeof(areacode)-1,       areacode,
			"areaname",    UT_TYPE_STRING,  sizeof(areaname)-1,       areaname,
			"countrycode", UT_TYPE_STRING,  sizeof(countrycode)-1,    countrycode,
			"lasttime",    UT_TYPE_STRING,  sizeof(lasttime)-1,       lasttime,
			"lcount",      UT_TYPE_STRING,  sizeof(lcount)-1,         lcount,
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
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,30);
			strcpy(username,caTemp);
   }
   if(strlen(adcod)>0){
			pasCvtGBK(2,adcod,caTemp,30);
			strcpy(adcod,caTemp);
   }
   if(strlen(mac)>0){
			pasCvtGBK(2,mac,caTemp,30);
			strcpy(mac,caTemp);
   }
   if(strlen(sip)>0){
			pasCvtGBK(2,sip,caTemp,30);
			strcpy(sip,caTemp);
   }
   if(strlen(phtype)>0){
			pasCvtGBK(2,phtype,caTemp,30);
			strcpy(phtype,caTemp);
   }
   if(strlen(sptype)>0){
			pasCvtGBK(2,sptype,caTemp,30);
			strcpy(sptype,caTemp);
   }
	 
	 // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
	 
	 iTime_flag = atoi(time_flag);  // atoi 将字符串转换成长整型数据 和atol一样
	 switch(iTime_flag)
	 {
	 	  case 0:
	 	  	ncTimeToLong(sdate, &lStartTime);
	      ncTimeToLong(edate, &lTime);
	      break;
	   	case 1://今天
	   		days = 1;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 2://最近三天
	   		days = 3;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 3://最近一周
	   		days = 7;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 4://最近一月
	   		days = 30;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 5://最近三月
	   		days = 90;
	   		ncDaysToLong(lTime, days, &lStartTime);
		    lTime=lTime+2*3600;
	   		break;
	   	case 6://最近半年
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
	 
	 // 时间字符串的格式转化
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
	 
	 snprintf(sql, sizeof(sql), "select count(*) from ncadlog where lasttime >= %lu and lasttime <= %lu ", lStartTime, lTime);
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%' or adcod like '%%%s%%') ", _keyword, _keyword);
	 }
	 if(!utStrIsSpaces(username))  // 用户名
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(adcod))  // Portal Name
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and adcod like '%%%s%%'", adcod);
	 }
	 if(!utStrIsSpaces(mac))  // mac
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", mac);
	 }
	 if(!utStrIsSpaces(sip))  // ip
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", sip);
	 }
	 if(!utStrIsSpaces(phtype))  // 上网终端
	 {
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and phtype = %lu", atol(phtype));
	 }
	 if(!utStrIsSpaces(sptype))  // 运营商类型
	 {
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sptype = %lu", atol(sptype));
	 }
   // pasDbOneRecord 单记录查询。　0---正确; !=0---错误代码, 具体含义请参见 数据库的有关资料
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
   
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 snprintf(sql, sizeof(sql), "select sid, refer, url, ncname, username, sip, mac, adcod, apname, apssid, phtype, os, sptype, devtype, browser, areacode, areaname, contrycode, lasttime, lcount from ncadlog where lasttime >= %lu and lasttime <= %lu", lStartTime, lTime);
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%' or adcod like '%%%s%%') ", _keyword, _keyword);
	 }
	 if(!utStrIsSpaces(username))  // 用户名
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and username like '%%%s%%'", username);
	 }
	 if(!utStrIsSpaces(adcod))  // Portal Name
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and adcod like '%%%s%%'", adcod);
	 }
	 if(!utStrIsSpaces(mac))  // mac
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", mac);
	 }
	 if(!utStrIsSpaces(sip))  // ip
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", sip);
	 }
	 if(!utStrIsSpaces(phtype))  // 上网终端
	 {
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and phtype = %lu", atol(phtype));
	 }
	 if(!utStrIsSpaces(sptype))  // 运营商类型
	 {
	 		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sptype = %lu", atol(sptype));
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),        &lSid,
													     UT_TYPE_STRING, sizeof(caRefer)-1,   caRefer,
													     UT_TYPE_STRING, sizeof(caUrl)-1,     caUrl,
													     UT_TYPE_STRING, sizeof(caNcname)-1,  caNcname,
															 UT_TYPE_STRING, sizeof(caUsername)-1,caUsername,
															 UT_TYPE_STRING, sizeof(caSip)-1,     caSip,
															 UT_TYPE_STRING, sizeof(caMac)-1,     caMac,
															 UT_TYPE_STRING, sizeof(caAdcod)-1,   caAdcod,
															 UT_TYPE_STRING, sizeof(caApname)-1,  caApname,
															 UT_TYPE_STRING, sizeof(caApssid)-1,  caApssid,
															 UT_TYPE_ULONG,  sizeof(long),        &lPhtype,
															 UT_TYPE_STRING, sizeof(caOs)-1,      caOs,
															 UT_TYPE_ULONG,  sizeof(long),        &lSptype,
															 UT_TYPE_STRING, sizeof(caDevtype)-1, caDevtype,
															 UT_TYPE_STRING, sizeof(caBrowser)-1, caBrowser,
															 UT_TYPE_ULONG,  sizeof(long),        &lAreacode,
															 UT_TYPE_STRING, sizeof(caAreaname)-1,caAreaname,
															 UT_TYPE_ULONG,  sizeof(long),        &lCountrycode,
															 UT_TYPE_ULONG,  sizeof(long),        &lLasttime,
															 UT_TYPE_ULONG,  sizeof(long),        &lLcount);  
		  
	 while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 
		   if(lLasttime>0){
		   		strcpy(s_lasttime, utTimFormat("%Y/%m/%d %H:%M:%S", lLasttime));
		   }else{
		   		strcpy(s_lasttime, "");	
		   }
		   
		   switch(lPhtype){
		   		case 1:strcpy(s_phtype, "平板");break;
		   		case 2:strcpy(s_phtype, "PC");break;	
		   		case 3:strcpy(s_phtype, "手机");break;
		   		default:strcpy(s_phtype, "");break;
		   }
		   
		   switch(lSptype){
		   		case 0:strcpy(s_sptype, "不合法");break;
		   		case 1:strcpy(s_sptype, "移动");break;	
		   		case 2:strcpy(s_sptype, "联通");break;
		   		case 3:strcpy(s_sptype, "电信");break;
		   		case 8:strcpy(s_sptype, "其他");break;	
		   		case 9:strcpy(s_sptype, "国际");break;
		   		default:strcpy(s_sptype, "");break;	
		   }
		   
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVar(psDbHead,"phtype",iNum,s_phtype);
			 utPltPutLoopVar(psDbHead,"sptype",iNum,s_sptype);
			 utPltPutLoopVarF(psDbHead,"areacode",iNum,"%lu",lAreacode);
			 utPltPutLoopVarF(psDbHead,"countrycode",iNum,"%lu",lCountrycode);
			 utPltPutLoopVarF(psDbHead,"lasttime",iNum,s_lasttime);
			 utPltPutLoopVarF(psDbHead,"lcount",iNum,"%lu",lLcount);
			 utPltSetCvtHtml(1);
			 utPltPutLoopVar(psDbHead,"refer",iNum,caRefer);
			 utPltPutLoopVar(psDbHead,"url",iNum,caUrl);
			 utPltPutLoopVar(psDbHead,"ncname",iNum,caNcname);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVar(psDbHead,"sip",iNum,caSip);
			 utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
			 utPltPutLoopVar(psDbHead,"adcod",iNum,caAdcod);
			 utPltPutLoopVar(psDbHead,"apname",iNum,caApname);
			 utPltPutLoopVar(psDbHead,"apssid",iNum,caApssid);
			 utPltPutLoopVar(psDbHead,"os",iNum,caOs);
			 utPltPutLoopVar(psDbHead,"devtype",iNum,caDevtype);
			 utPltPutLoopVar(psDbHead,"browser",iNum,caBrowser);
			 utPltPutLoopVar(psDbHead,"areaname",iNum,caAreaname);
			 utPltSetCvtHtml(0);
			 
			 memset(s_lasttime, 0, sizeof(s_lasttime));
			 lSid = 0;
			 memset(caRefer, 0, sizeof(caRefer));
			 memset(caUrl, 0, sizeof(caUrl));
			 memset(caNcname, 0, sizeof(caNcname));
			 memset(caUsername, 0, sizeof(caUsername));
			 memset(caSip, 0, sizeof(caSip));
			 memset(caMac, 0, sizeof(caMac));
			 memset(caAdcod, 0, sizeof(caAdcod));
			 memset(caApname, 0, sizeof(caApname));
			 memset(caApssid, 0, sizeof(caApssid));
			 lPhtype = 0;
			 memset(caOs, 0, sizeof(caOs));
			 lSptype = 0;
			 memset(caDevtype, 0, sizeof(caDevtype));
			 memset(caBrowser, 0, sizeof(caBrowser));
			 lAreacode = 0;
			 memset(caAreaname, 0, sizeof(caAreaname));
			 lCountrycode = 0;
			 lLasttime = 0;
			 lLcount = 0;
			 memset(s_lasttime, 0, sizeof(s_lasttime));
			 memset(s_phtype, 0, sizeof(s_phtype));
			 memset(s_sptype, 0, sizeof(s_sptype));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),        &lSid,
															     UT_TYPE_STRING, sizeof(caRefer)-1,   caRefer,
															     UT_TYPE_STRING, sizeof(caUrl)-1,     caUrl,
															     UT_TYPE_STRING, sizeof(caNcname)-1,  caNcname,
																	 UT_TYPE_STRING, sizeof(caUsername)-1,caUsername,
																	 UT_TYPE_STRING, sizeof(caSip)-1,     caSip,
																	 UT_TYPE_STRING, sizeof(caMac)-1,     caMac,
																	 UT_TYPE_STRING, sizeof(caAdcod)-1,   caAdcod,
																	 UT_TYPE_STRING, sizeof(caApname)-1,  caApname,
																	 UT_TYPE_STRING, sizeof(caApssid)-1,  caApssid,
																	 UT_TYPE_ULONG,  sizeof(long),        &lPhtype,
																	 UT_TYPE_STRING, sizeof(caOs)-1,      caOs,
																	 UT_TYPE_ULONG,  sizeof(long),        &lSptype,
																	 UT_TYPE_STRING, sizeof(caDevtype)-1, caDevtype,
																	 UT_TYPE_STRING, sizeof(caBrowser)-1, caBrowser,
																	 UT_TYPE_ULONG,  sizeof(long),        &lAreacode,
																	 UT_TYPE_STRING, sizeof(caAreaname)-1,caAreaname,
																	 UT_TYPE_ULONG,  sizeof(long),        &lCountrycode,
																	 UT_TYPE_ULONG,  sizeof(long),        &lLasttime,
																	 UT_TYPE_ULONG,  sizeof(long),        &lLcount);
	 }
	 //utPltShowDb(psDbHead);
	 pasDbCloseCursor(psCur);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_advertisement/ncm_web_Place_advertisement_list.htm");
	 
	 return 0;	
}

