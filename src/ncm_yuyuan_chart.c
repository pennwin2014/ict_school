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

int ncm_sms_send_chart(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。

	// 初始化数据库
	char sql[4096] = "";
	pasDbCursor *psCur = NULL;
	int iret=0;
	int iNum=0;
	utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板

	// 接收信息变量
	char sdate[32] = "";
	char edate[32] = "";
	char timeV[32] = "";
	char localNum[32] = "";
	char interNum[64] = "";
	char groupcode[32] = "";
	char areaid[32] = "";
	char groupid[32] = "";

	// 存放数据库取出的数据的变量
	unsigned long smsnum = 0;
	char htime[24] = "";

	unsigned long lSdate = 0;
	unsigned long lEdate = time(0);

	char caUpdate[16] = "";
	char del[16] = "";
	char caSesid[1024] = "";
	char caDel[16] = "";
	// 关键字
	char _keyword[33] = "";

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
	char expexcel[32] = "";
	FILE *fp = NULL;
	char caFilename[32] = "";
	char caFile[128] = "";
	char caPath[128] = "";
	char caDispName[32];
	// 商家
	char caStype[8] = "";
	char caOptd[64] = "";
	unsigned long lCid = 0;
	iReturn = dsCltGetSessionValue(1, "cid",	UT_TYPE_LONG,	sizeof(long),			&lCid);
	iReturn = dsCltGetSessionValue(1, "stype",	UT_TYPE_STRING, sizeof(caStype) - 1,	caStype);
	iReturn = dsCltGetSessionValue(1, "opt",	UT_TYPE_STRING, sizeof(caOptd) - 1,		caOptd);
	//iReturn = dsCltGetSessionValue(1, "dispname",UT_TYPE_STRING,	sizeof(caDispName) - 1,	caDispName); 

	// 创建纪录信息结构体
	int i = 0;
	 typedef struct sms_stat{
	 		int num;
	 		char timeV[18];
	 		long localNum;
	 		long interNum;
	 }sms_stat_s;
	 sms_stat_s stat_sms[25];
	 for (i = 0;i < 24;i++){ // 初始化
		 	 stat_sms[i].num = i;
		   sprintf(stat_sms[i].timeV,"%02d:00:00-%02d:59:59",i,i);
		   stat_sms[i].localNum = 0;
		   stat_sms[i].interNum = 0;
		   //printf("num = %d timeV = %s localNum = %d interNum = %d \n",stat_sms[i].num,stat_sms[i].timeV,stat_sms[i].localNum,stat_sms[i].interNum);	
	 }
	 stat_sms[24].num = 24;
   sprintf(stat_sms[24].timeV,"总计");
   stat_sms[24].localNum = 0;
   stat_sms[24].interNum = 0;

	utMsgGetSomeNVar(psMsgHead, 7,	"sdate",        UT_TYPE_STRING,  sizeof(sdate)-1,       sdate,
																	"edate",        UT_TYPE_STRING,  sizeof(edate)-1,       edate,
																	"groupcode",    UT_TYPE_STRING,  sizeof(groupcode)-1,   groupcode,
																	"areaid",       UT_TYPE_STRING,  sizeof(areaid)-1,      areaid,
																	//"groupid",      UT_TYPE_STRING,  sizeof(groupid)-1,     groupid,
																	"expexcel",     UT_TYPE_STRING,  sizeof(expexcel)-1,    expexcel);

  // 时间转换
	sprintf(sdate, "%s %s", sdate, "00:00:00");
	sprintf(edate, "%s %s", edate, "23:59:59");
	ncTimeToLong(sdate,&lSdate);
	ncTimeToLong(edate,&lEdate);

	char _sdate[32] = ""; // 格式转换后的开始时间
	char _edate[32] = ""; // 格式转换后的当前时间
	strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
	strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
	printf("_sdate = %s\n", _sdate);
	printf("_edate = %s\n", _edate);

	memset(sql,0,sizeof(sql)); // 国内短信统计
	sprintf(sql, "select count(*) as smsnum, hour(from_unixtime(logtime)) as htime from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);		                    
	snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (contrycode = '086' or contrycode = '86')");
	strcpy(sql, (char *)ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by htime");
	printf("sql = %s\n", sql);
	psCur = pasDbOpenSql(sql, 0);
	if(psCur==NULL){
		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
	   return 0;	
	}
	
	// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
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

	memset(sql,0,sizeof(sql)); // 国际短信统计
	sprintf(sql, "select count(*) as smsnum, hour(from_unixtime(logtime)) as htime from ncsrvsmslog where logtime>%lu and logtime<%lu", lSdate, lEdate);		                    
	strcpy(sql, (char *)ncmWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
	snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (contrycode != '086' and contrycode != '86')");
	snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by htime");
	printf("sql = %s\n", sql);
	psCur = pasDbOpenSql(sql, 0);
	if(psCur==NULL){
		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
	   return 0;	
	}
	
	smsnum = 0;
	memset(htime, 0, sizeof(htime));
	// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
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
	for(i=0; i<24; i++){
	 		iNum++;
	    if(iNum > 1)
		  {
			   utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
		  }
	 		utPltPutLoopVar(psDbHead,"inum",iNum,stat_sms[i].timeV);
	 		utPltPutLoopVarF(psDbHead,"chart_name",iNum,"%lu",stat_sms[i].localNum);		
	 		utPltPutLoopVarF(psDbHead,"chart_number",iNum,"%lu",stat_sms[i].interNum);
	 		/*if(strlen(expexcel)>0){
					 		ncUtlPrintCsv(fp,3,UT_TYPE_STRING,stat_sms[i].timeV,
					 											UT_TYPE_ULONG,stat_sms[i].localNum,
														 		UT_TYPE_ULONG,stat_sms[i].interNum);
			}	*/
	 }
	/*iNum = 0;
	int count = 25;
	if(strlen(number) > 0)
		count = 24;
	printf("\t\tnumber value: %s  count : %d\n", number, count);
	for(i = 0; i < count; i++)
	{
		iNum++;
		if(iNum > 1)
		{
			utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
		}
		if(count == 24)
		{
			sprintf(stat_int[i].timeV, "%d时", i);
			utPltPutLoopVar(psDbHead,	"time",		iNum,	stat_int[i].timeV);
		}
		else
		{

			utPltPutLoopVar(psDbHead,	"time",		iNum,	stat_int[i].timeV);
		}
		utPltPutLoopVarF(psDbHead,	"number",	iNum,	"%lu",	stat_int[i].number);
		utPltPutLoopVarF(psDbHead,	"traffic",	iNum,	"%.2f", stat_int[i].traffic);
	}*/
	//utPltPutVarF(psDbHead,"TotRec", "%lu", 25);
	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead, "ncm_chart/ncm_sms_send_chart.htm");
	return 0;
}

int Ncm_YuyuanSWW_SetFunName(){
		pasSetTcpFunName("ncm_sms_send_chart", ncm_sms_send_chart, 0);  // 20141031短信发送图表
		
		return 0;	
}
