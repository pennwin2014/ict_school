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
#include "ncportal.h"
#include "ict_zjj_tool.h"
#include "pasutl.h"
#include <iconv.h>

int ict_school_orderLog(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	char sql[1024] = "";
	char sql1[1024] = "";
	char sql_tmp[1024] = "";
    char startTime[24] = "";
    char endTime[24] = "";
	char startTime1[24] = "";
    char endTime1[24] = "";
    char start_in[8] = "";
    char limit_in[8] = "";
	char keyword[24] = "";
	char packageid[8] = "";
	char order_count[8]="";		
	char begin[24] = "";
	char end[24] = "";
	uint8 begintime = 0;
	uint8 endtime = 0;
	uint8 nowTime = time(0);
	char table_name[1024] = "";	
	int totalCount=0;
	utMsgGetSomeNVar(psMsgHead, 9,
							 "starttime", UT_TYPE_STRING, sizeof(startTime) - 1, startTime,
							 "endtime", UT_TYPE_STRING, sizeof(endTime) - 1, endTime,
							 "starttime1", UT_TYPE_STRING, sizeof(startTime1) - 1, startTime1,
							 "endtime1", UT_TYPE_STRING, sizeof(endTime1) - 1, endTime1,
							 "start", UT_TYPE_STRING,  sizeof(start_in) - 1, start_in,
							 "limit", UT_TYPE_STRING,  sizeof(limit_in) - 1,  limit_in,
							 "keyword", UT_TYPE_STRING,  sizeof(keyword) - 1,  keyword,
							 "packageid", UT_TYPE_STRING,  sizeof(packageid) - 1,  packageid,
							 "order_count", UT_TYPE_STRING,  sizeof(order_count) - 1,  order_count
							);
	pasDbCursor *psCur = NULL;
	pasDbCursor *psCur1 = NULL;		
	utPltDbHead *psDbHead = utPltInitDb();	
	utPltDbHead *psDbHead1 = utPltInitDb();	
	if(strlen(startTime) == 1)
	{
		switch(atol(startTime))
		{
			case 1:
				snprintf(begin, sizeof(begin) - 1, "%s", utTimFormat("%Y%m%d", nowTime - 7 * 24 * 3600));
				break;
			case 2:
				snprintf(begin, sizeof(begin) - 1, "%s", utTimFormat("%Y%m%d", nowTime - 30 * 24 * 3600));
				break;
			case 3:
				snprintf(begin, sizeof(begin) - 1, "%s", utTimFormat("%Y%m%d", nowTime - 90 * 24 * 3600));
				break;
			case 4:
				snprintf(begin, sizeof(begin) - 1, "%s", utTimFormat("%Y%m%d", nowTime - 180 * 24 * 3600));
				break;
			default:
				break;
		}
		begintime = utTimStrToLong("%Y%m%d %H%M%S", begin);
		endtime = nowTime;
		snprintf(end, sizeof(begin) - 1, "%s", utTimFormat("%Y%m%d", nowTime));
		pasLogs(8888,8888,"begin=%s end=%s\n", begin,end);
		combine_table(table_name, begin, end, "userorderlog_");

	}
	else
	{
		if(strcmp(startTime,"all")==0)
		{
			combine_table(table_name, startTime, endTime, "userorderlog_");
			pasLogs(8888,8888,"all   %s\n", table_name);
		}
		else
		{
			begintime = utTimStrToLong("%Y%m%d %H%M%S", startTime);
			endtime = utTimStrToLong("%Y%m%d %H%M%S", endTime) + 24 * 3600 - 1;
			pasLogs(8888,8888,"begin=%s end=%s\n", startTime,endTime);
			combine_table(table_name, startTime, endTime, "userorderlog_");
		}	
	}	
	
	if(strcmp(order_count,"count") != 0)
	{
		char orderno_on[64]="";
		char username_on[32]="";
		char mname_on[32]="";
		char mobno_on[16]="";
		char timeval_on[24]="";
		char name_on[64]="";
		char starttime_on[24]="";
		char endtime_on[24]="";
		//long count=0;
		
			//  printf("sql=%s\n", sql);
		pasDbOneRecord(sql_tmp, 0, UT_TYPE_LONG, 4, &totalCount);
		snprintf(sql, sizeof(sql)-1,"select orderno,username,mobno,from_unixtime(timeval),name,from_unixtime(starttime),from_unixtime(endtime) from %s",table_name);
		
		snprintf(sql_tmp+strlen(sql_tmp), sizeof(sql_tmp)-strlen(sql_tmp)-1, " where 1=1");
		if(strlen(startTime) != 0)	
			snprintf(sql_tmp+strlen(sql_tmp), sizeof(sql_tmp)-strlen(sql_tmp)-1, " and timeval<=%llu and timeval>=%llu", endtime, begintime);
		if(strlen(packageid) !=0)
			snprintf(sql_tmp+strlen(sql_tmp), sizeof(sql_tmp)-strlen(sql_tmp)-1, " and packageid=%d",atoi(packageid));
		
		snprintf(sql1, sizeof(sql1)-1, "select count(*) from %s %s",table_name,sql_tmp);
		pasDbOneRecord(sql1, 0, UT_TYPE_LONG, 4, &totalCount);
		pasLogs(8888,8888,"sql1= %s\n", sql1);
		
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql)-1, "%s order by timeval desc limit %d,%d",sql_tmp,atoi(start_in),atoi(limit_in));
		pasLogs(8888,8888,"sql %s\n", sql);
		psCur = pasDbOpenSql(sql, 0);
		
		int iret = 0, iNum = 0;
		if(psCur)
		{
			
			while(0 == (iret = pasDbFetchInto(psCur,
								  UT_TYPE_STRING, sizeof(orderno_on) - 1, orderno_on,  
								  UT_TYPE_STRING, sizeof(username_on) - 1, username_on,
								//  UT_TYPE_STRING, sizeof(mname_on) - 1, mobno_on,
								  UT_TYPE_STRING, sizeof(mobno_on) - 1, mobno_on,  
								  UT_TYPE_STRING, sizeof(timeval_on) - 1, timeval_on,
								  UT_TYPE_STRING, sizeof(name_on) - 1, name_on,
								  UT_TYPE_STRING, sizeof(starttime_on) - 1, starttime_on,
								  UT_TYPE_STRING, sizeof(endtime_on) - 1, endtime_on)) || 1405 == iret)
			{
				iNum++;
				if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead, "dh", iNum, ",");
				}
				memset(sql_tmp, 0, sizeof(sql_tmp));
				snprintf(sql_tmp, sizeof(sql_tmp)-1,"select mname from ncsrvuserex where username='%s'",username_on);
				pasDbOneRecord(sql_tmp, 0, UT_TYPE_STRING, sizeof(mname_on)-1, mname_on);
				pasLogs(8888,8888,"sql_tmp %s    mname=%d\n", sql_tmp,mname_on);
				
				utPltPutLoopVar(psDbHead, "orderno", iNum, orderno_on);
				utPltPutLoopVar(psDbHead, "username", iNum, username_on);
				utPltPutLoopVar(psDbHead, "mname", iNum, convert("GBK", "UTF-8", mname_on));
				utPltPutLoopVar(psDbHead, "mobno", iNum, mobno_on);
				utPltPutLoopVar(psDbHead, "timeval", iNum, timeval_on);
				utPltPutLoopVar(psDbHead, "name", iNum, convert("GBK", "UTF-8", name_on));
				utPltPutLoopVar(psDbHead, "starttime", iNum, starttime_on);
				utPltPutLoopVar(psDbHead, "endtime", iNum, endtime_on);			
			}
			pasDbCloseCursor(psCur);
		}
	}
	
	if(strcmp(order_count,"count")==0)
	{
		long id=0;
		long count=0;
		uint8 money=0;
		uint8 totalmoney=0;
		char namech[64]="";
		memset(sql, 0, sizeof(sql));
		snprintf(sql, sizeof(sql)-1, "select namech,id from package");
		psCur1 = pasDbOpenSql(sql, 0);
		int iret1 = 0, iNum1 = 0;
	//	pasLogs(8888,8888,"111111\n");
		if(psCur1)
		{
		//	pasLogs(8888,8888,"22222\n");
			while(0 == (iret1 = pasDbFetchInto(psCur1,
								  UT_TYPE_STRING, 63, namech, 	
								  UT_TYPE_LONG, 4, &id)) || 1405 == iret1)
			{
				//pasLogs(8888,8888,"3333333\n");	
				iNum1++;
				//iNum++;
				pasLogs(8888,8888,"namech=%s  id=%d\n", namech,id);
				if(iNum1 > 1)
				{
					utPltPutLoopVar(psDbHead, "dh", iNum1, ",");
				}
				memset(sql_tmp, 0, sizeof(sql_tmp));
				snprintf(sql_tmp, sizeof(sql_tmp)-1, "select count(*),sum(money) from %s where packageid=%d", table_name, id);
				pasLogs(8888,8888,"sql=[%s]\n", sql_tmp);
				pasDbOneRecord(sql_tmp, 0, 
										   UT_TYPE_LONG, 4, &count,
										   UT_TYPE_LONG8, 8, &money);
				pasLogs(8888,8888,"%ld %llu\n", count,money);
				utPltPutLoopVar(psDbHead, "namech", iNum1, convert("GBK", "UTF-8", namech));
				utPltPutLoopVarF(psDbHead, "count", iNum1, "%ld", count);
				utPltPutLoopVarF(psDbHead, "money", iNum1, "%llu", money);			
							
			}
			pasDbCloseCursor(psCur1);
		}
		memset(sql_tmp, 0, sizeof(sql_tmp));
		snprintf(sql_tmp, sizeof(sql_tmp)-1, "select sum(money) from %s", table_name);
		pasDbOneRecord(sql_tmp, 0, UT_TYPE_LONG8, 8, &totalmoney);
		utPltPutVarF(psDbHead, "totalmoney", "%llu", totalmoney);
		
	}
	
	utPltPutVarF(psDbHead, "result", "%d", 0);
	utPltPutVarF(psDbHead, "totalCount", "%d", totalCount);
	utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/order/ict_school_order.htm");
	
	return 0;
}	

int ict_school_getPackage(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	char sql[1024]="";
	char namech[64]="";
	long id=0;
	pasDbCursor *psCur = NULL;		
	utPltDbHead *psDbHead = utPltInitDb();	
	
	snprintf(sql, sizeof(sql)-1, "select namech,id from package");
	psCur = pasDbOpenSql(sql, 0);
	int iret = 0, iNum = 0;
	if(psCur)
	{
	//	pasLogs(8888,8888,"22222\n");
		iNum++;
		utPltPutLoopVar(psDbHead, "namech", iNum, convert("GBK", "UTF-8", "全部"));
		while(0 == (iret = pasDbFetchInto(psCur,
							  UT_TYPE_STRING, 63, namech, 	
							  UT_TYPE_LONG, 4, &id)) || 1405 == iret)
		{
			//pasLogs(8888,8888,"3333333\n");	
			iNum++;
			//iNum++;
			pasLogs(8888,8888," getpackage namech=%s  id=%d\n", namech,id);
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead, "dh", iNum, ",");
			}
			//pasLogs(8888,8888,"%ld %llu\n", count,money);
			utPltPutLoopVar(psDbHead, "namech", iNum, convert("GBK", "UTF-8", namech));
			utPltPutLoopVarF(psDbHead, "packageid", iNum, "%ld", id);		
						
		}
		pasDbCloseCursor(psCur);
	}
	utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/order/ict_school_getPackage.htm");
	
	return 0;
}

int ictInitWebFun_school_zjj(utShmHead *psShmHead)
{
    pasSetTcpFunName("ict_school_orderLog", ict_school_orderLog, 0);//账单查询
	pasSetTcpFunName("ict_school_getPackage", ict_school_getPackage, 0);//账单查询
	
    return 0;
}

