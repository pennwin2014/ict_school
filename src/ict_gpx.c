#define  PAS_SRCFILE   7006

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




//#define  PAS_SRCFILE    7001


//static uint8 indexTsid = 0;
static char  caSn[16] = "PHT36BSZ253";
static char caServicecode[32] = "pronetwayshaitest";
static int   iUpdateUserFlags = 0;       //  1--直接更新远程数据库   0--只更新本地    2--通过Pas更新

void  combine_table(char *table_name, char *sdate, char *edate, char *table)
{
    char sql[1024] = "";
    char casyear[12] = "";
    char casmonth[12] = "";
    char caeyear[12] = "";
    char caemonth[12] = "";
    strncpy(casyear, sdate, 4);
    casyear[4] = '\0';
    strncpy(casmonth, sdate + 4, 2);
    casmonth[2] = '\0';
    strncpy(caeyear, edate, 4);
    caeyear[4] = '\0';
    strncpy(caemonth, edate + 4, 2);
    caemonth[2] = '\0';
    int syear = atoi(casyear);
    int smonth = atoi(casmonth);
    int eyear = atoi(caeyear);
    int emonth = atoi(caemonth);


    if(syear == eyear && smonth == emonth)
        snprintf(table_name + strlen(table_name), 1024 - strlen(table_name), "%s%4u%02u as new_table", table, syear, smonth);
    else
    {
        int i, j = smonth, iNum = 0;
        strcat(table_name, "(");

        pasDbCursor *psCur = NULL;
        for(i = syear; i <= eyear && j <= emonth;)
        {
            snprintf(sql, sizeof(sql)-1, "select * from %s%4u%02u", table, i, j);
            psCur = pasDbOpenSql(sql, 0);
            if(psCur != NULL)
            {
                if(iNum > 0)
                {
                    strcat(table_name, " union all ");
                }
                iNum++;
                snprintf(table_name + strlen(table_name), 1024 - strlen(table_name), sql);
                pasDbCloseCursor(psCur);
            }

            if(j > 11)
            {
                i++;
                j = 0;
            }
            j++;
        }
        snprintf(table_name + strlen(table_name), 1024 - strlen(table_name), ")new_table");
    }
}

int checkTableExists(char* tableName)
{
    char sql[1024] = "";
    int ret = 0;
    pasDbCursor *psCur = NULL;
    snprintf(sql, sizeof(sql), "select count(*) from %s", tableName);
    psCur = pasDbOpenSql(sql, 0);
    if(psCur != NULL)
    {
        ret = 1;
        pasDbCloseCursor(psCur);
    }
    return ret;
}

char* getNewLogTable(char* tableOri, char * yearMonth)
{
    static char newTable[64] = "";
    snprintf(newTable, sizeof(newTable) - 1, "%s_%s", tableOri,yearMonth);
	char sql[1024] = "";
	if(!checkTableExists(newTable))
    {
        snprintf(sql, sizeof(sql) - 1, "create table %s like %s", newTable, tableOri);
     //   printf("not exist logTable=%s, do sql=%s\n", newTable, sql);
		//执行sql语句
        pasDbExecSqlF(sql);
    }
    return newTable;
}

static int checkMoney(char money[16])//检测money有效性
{
	int i;
	if(atol(money) == 0 || strlen(money)==0)
	{
		return 0;
	}
	
	for(i=0; i < strlen(money); i++)
	{
		if(money[i] < '0' || money[i] > '9')
			return 0;
	}
		
	return 1;
}



int ict_orderLog_search(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//账单查询
{
    utMsgPrintMsg(psMsgHead);

    char vname[32] = "";
    char name[32] = "";
    ulong money = 0;
	char dtime[24]="";
	char timeFlag[8] = "";
	char sql[1024] = "";
	char sql_count[1024] = "";
	char sqlbuf[1024] = "";
	char caTsid[24] = "";
	char startTime[24] = "";
	char endTime[24] = "";
	char start_in[8] = "";
	char limit_in[8] = "";
	ulong totalCount = 0;
	ulong lCount = 0;
	char sql_tmp[1024] = "";
	char caTime[24] = "";
	char table_name[256] = "";
	char table_name1[256] = "";

    utMsgGetSomeNVar(psMsgHead, 5,
							 "starttime", UT_TYPE_STRING, sizeof(startTime) - 1, startTime,
							 "endtime", UT_TYPE_STRING, sizeof(endTime) - 1, endTime,
							 "start", UT_TYPE_STRING,  sizeof(start_in) - 1, start_in,
							 "limit", UT_TYPE_STRING,  sizeof(limit_in) - 1,  limit_in,
							 "tsid", UT_TYPE_STRING,  sizeof(caTsid) - 1,  caTsid
							);
	
	pasDbCursor *psCur = NULL;
	utPltDbHead *psDbHead = utPltInitDb();
	
	if(checkTsid(psShmHead, psDbHead,psMsgHead, iFd, caTsid, "school/billCheck/bill_check.htm"))
	{
		return 0;
	}
	snprintf(vname, sizeof(vname)-1, "%s", getVnameByTsId(psShmHead, atoll(caTsid)));
	//strcpy(vname, getVnameByTsId(psShmHead, atoll(caTsid)));
	if(strlen(vname) == 0)
	{
        utPltPutVarF(psDbHead, "result", "%d", 3);//用户名空
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/billCheck/bill_check.htm");		
	}
	else
	{
		
		memset(sqlbuf, 0, sizeof(sqlbuf));
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select count(*) from ncsrvuser where username='%s'", vname);
		pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);
		if(lCount <= 0)
        {
            utPltPutVarF(psDbHead, "result", "%d", 4);//用户名不存在
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/billCheck/bill_check.htm");
        }
		else
		{	
			memset(sqlbuf, 0, sizeof(sqlbuf));
			snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select count(*) from ncsrvuser where username='%s'", vname);
			char begin[24] = "";
			char end[24] = "";
			ulong begintime = 0;
			ulong endtime = 0;
			uint8 nowTime=time(0);
			if(strlen(startTime) == 1)
			{
				switch(atol(startTime))
				{
					case 1: snprintf(begin, sizeof(begin)-1, "%s", utTimFormat("%Y%m%d", nowTime-7*24*3600)); break;
					case 2: snprintf(begin, sizeof(begin)-1, "%s", utTimFormat("%Y%m%d", nowTime-30*24*3600));break;
					case 3: snprintf(begin, sizeof(begin)-1, "%s", utTimFormat("%Y%m%d", nowTime-90*24*3600));break;
					case 4: snprintf(begin, sizeof(begin)-1, "%s", utTimFormat("%Y%m%d", nowTime-180*24*3600));break;
					default: break;
				}
				begintime = utTimStrToLong("%Y%m%d %H%M%S", begin);
				endtime = nowTime;
				snprintf(end, sizeof(begin)-1, "%s", utTimFormat("%Y%m%d", nowTime));				
				combine_table(table_name, begin, end, "userorderlog_");	
				combine_table(table_name1, begin, end, "rechargelog_");
			}
			else
			{
				begintime = utTimStrToLong("%Y%m%d %H%M%S", startTime);
				endtime = utTimStrToLong("%Y%m%d %H%M%S", endTime) + 24*3600 - 1;
				combine_table(table_name, startTime, endTime, "userorderlog_");	
				combine_table(table_name1, startTime, endTime, "rechargelog_");	
			}

			snprintf(sql_tmp, sizeof(sql_tmp)-1, "where username='%s' and new_table.timeval>=%lu and new_table.timeval<=%lu", vname, begintime, endtime);		
			snprintf(sql, sizeof(sql)-1, "select money from %s %s", table_name, sql_tmp);
			psCur = pasDbOpenSql(sql, 0);	
			int iret = 0, iNum = 0;  
			
			
			ulong payMoney = 0;
			if(psCur)
			{
				
				while(0 == (iret = pasDbFetchInto(psCur,
												UT_TYPE_LONG, 4, &money)) || 1405 == iret)
				{
					payMoney = payMoney + money;
					iNum++;
				}
				pasDbCloseCursor(psCur);
			}
			
			memset(sql_tmp, 0, sizeof(sql_tmp));
			memset(sql,0,sizeof(sql));
			snprintf(sql_tmp, sizeof(sql_tmp)-1, "where username='%s' and new_table.timeval>=%lu and new_table.timeval<=%lu", vname, begintime, endtime);
			snprintf(sql, sizeof(sql)-1, "select money from %s %s", table_name1, sql_tmp);
			psCur = pasDbOpenSql(sql, 0);
			
			iret = 0;
			iNum = 0;
			ulong reMoney = 0;
			if(psCur)
			{
				while(0 == (iret = pasDbFetchInto(psCur,
									  UT_TYPE_LONG, 4, &money)) || 1405 == iret)
				{
					reMoney = reMoney + money;
					iNum++;
				}
				pasDbCloseCursor(psCur);
			}
				
			memset(sql_tmp, 0, sizeof(sql_tmp));
			memset(sql, 0, sizeof(sql));
			snprintf(sql_tmp, sizeof(sql_tmp)-1, "where username='%s' and new_table.timeval>=%lu and new_table.timeval<=%lu", vname, begintime, endtime);		
			snprintf(sql, sizeof(sql)-1, "select timeval,money,name from %s %s", table_name, sql_tmp);
			char sql1[1024] = "";
			memset(sql_tmp, 0, sizeof(sql_tmp));
			snprintf(sql_tmp, sizeof(sql_tmp)-1, "where username='%s' and new_table.timeval>=%lu and new_table.timeval<=%lu", vname, begintime, endtime);
			snprintf(sql1, sizeof(sql1)-1, "select timeval,money,'' from %s %s", table_name1, sql_tmp);
			
			memset(sql_tmp, 0, sizeof(sql_tmp));
			snprintf(sql_tmp, sizeof(sql_tmp)-1, "select count(*) from (%s union all %s order by timeval desc)new_table", sql, sql1);
			pasDbOneRecord(sql_tmp, 0, UT_TYPE_LONG, 4, &totalCount);
			
			
			memset(sql_tmp, 0, sizeof(sql_tmp));
			snprintf(sql_tmp, sizeof(sql_tmp)-1, "select from_unixtime(timeval),money,name from (%s union all %s)new_table order by timeval desc limit %lu,%lu", sql, sql1, atol(start_in), atol(limit_in));
			psCur = pasDbOpenSql(sql_tmp, 0);	
			//pasLogs(5001,5001,"账单查询 %s\n", sql_tmp);
			printf("********************sql_tmp=%s\n", sql_tmp);
			pasLogs(1234, 1234, "账单查询 %s\n", sql_tmp);
			iret = 0;
			iNum = 0;
			if(psCur)
			{
				
				while(0 == (iret = pasDbFetchInto(psCur,
									  UT_TYPE_STRING, sizeof(dtime) - 1, dtime,  
									  UT_TYPE_LONG, 4, &money,
									  UT_TYPE_STRING, sizeof(name) - 1, name)) || 1405 == iret)
				{
					iNum++;
					if(iNum > 1)
					{
						utPltPutLoopVar(psDbHead, "dh", iNum, ",");
					}
					if(strlen(name) == 0)
					{
						utPltPutLoopVarF(psDbHead, "flag", iNum, "%d", 1);
					    utPltPutLoopVar(psDbHead, "dtime", iNum, dtime);
						snprintf(name, sizeof(name)-1, "充值%.2f元", money/100.0);
					    utPltPutLoopVar(psDbHead, "name", iNum, convert("GBK", "UTF-8", name));
					    utPltPutLoopVarF(psDbHead, "money", iNum, "%lu", money);
					}
					else
					{
						utPltPutLoopVarF(psDbHead, "flag", iNum, "%d", 0);
					    utPltPutLoopVar(psDbHead, "dtime", iNum, dtime);
					    utPltPutLoopVar(psDbHead, "name", iNum, convert("GBK", "UTF-8", name));
					    utPltPutLoopVarF(psDbHead, "money", iNum, "%lu", money);
					}
						
				}
				pasDbCloseCursor(psCur);
			}
			utPltPutVarF(psDbHead, "totalCount", "%lu", totalCount);
			utPltPutVarF(psDbHead, "payMoney", "%lu", payMoney);
			utPltPutVarF(psDbHead, "rechargeMoney", "%lu", reMoney);
			utPltPutVar(psDbHead, "tsid", caTsid);
			utPltPutVarF(psDbHead, "result", "%d", 0);
			utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/billCheck/bill_check.htm");	
		}	
	}
	
    return 0;
}

int ict_rechargelog_search(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//充值记录查询
{

    utMsgPrintMsg(psMsgHead);

    char vname[32] = "";
    char orderno[64] = "";
    char dtime[24] = "";
    ulong money = 0;
    ulong mtype = 0;
    char sql[1024] = "";
    char sql_count[1024] = "";
    char sql_tmp[1024] = "";
    char caTsid[24] = "";
    char startTime[24] = "";
    char endTime[24] = "";
    char start_in[8] = "";
    char limit_in[8] = "";
    ulong totalCount = 0;
    ulong lCount = 0;

    utMsgGetSomeNVar(psMsgHead, 5,
                     "starttime", UT_TYPE_STRING, sizeof(startTime), startTime,
                     "endtime", UT_TYPE_STRING, sizeof(endTime), endTime,
                     "start", UT_TYPE_STRING,  sizeof(start_in) - 1, start_in,
                     "limit", UT_TYPE_STRING,  sizeof(limit_in) - 1,  limit_in,
                     "tsid", UT_TYPE_STRING,  sizeof(caTsid) - 1,  caTsid
                    );

    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();
    if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/payRecord/pay_record.htm"))
    {
        return 0;
    }
    snprintf(vname, sizeof(vname) - 1, "%s", getVnameByTsId(psShmHead, atoll(caTsid)));
    //strcpy(vname, getVnameByTsId(psShmHead, atoll(caTsid)));
    if(strlen(vname) == 0)
    {
        utPltPutVarF(psDbHead, "result", "%d", 3);//用户名空
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/pay_record.htm");
    }
    else
    {
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql) - 1, "select count(*) from ncsrvuser where username='%s'", vname);
        pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount <= 0)
        {
            utPltPutVarF(psDbHead, "result", "%d", 4);//用户名不存在
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/pay_record.htm");
        }
        else
        {
            char begin[24] = "";
            char end[24] = "";
            ulong begintime = 0;
            ulong endtime = 0;
            uint8 nowTime = time(0);
            char table_name[256] = "";
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
                combine_table(table_name, begin, end, "rechargelog_");

            }
            else
            {
                begintime = utTimStrToLong("%Y%m%d %H%M%S", startTime);
                endtime = utTimStrToLong("%Y%m%d %H%M%S", endTime) + 24 * 3600 - 1;
                combine_table(table_name, startTime, endTime, "rechargelog_");

            }

            snprintf(sql_tmp, sizeof(sql_tmp) - 1, " where username='%s' and new_table.timeval>=%lu and new_table.timeval<=%lu", vname, begintime, endtime);


            memset(sql, 0, sizeof(sql));
            snprintf(sql, sizeof(sql) - 1, "select count(*) from %s %s", table_name, sql_tmp);
            //  printf("sql=%s\n", sql);
            pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &totalCount);


            memset(sql, 0, sizeof(sql));
            snprintf(sql, sizeof(sql) - 1, "select orderno,from_unixtime(timeval),money,mtype from %s %s order by timeval desc limit %lu,%lu", table_name, sql_tmp, atol(start_in), atol(limit_in));
           printf("sql = %s\n",sql);
			psCur = pasDbOpenSql(sql, 0);
            int iret = 0, iNum = 0;
            if(psCur)
            {
                while(0 == (iret = pasDbFetchInto(psCur,
                                                  UT_TYPE_STRING, sizeof(orderno) - 1, orderno,
                                                  UT_TYPE_STRING, sizeof(dtime) - 1, dtime,
                                                  UT_TYPE_LONG, 4, &money,
                                                  UT_TYPE_LONG, 4, &mtype)) || 1405 == iret)
                {
                    iNum++;
                    if(iNum > 1)
                    {
                        utPltPutLoopVar(psDbHead, "dh", iNum, ",");
                    }
                    utPltPutLoopVar(psDbHead, "orderno", iNum, orderno);
                    utPltPutLoopVar(psDbHead, "dtime", iNum, dtime);
                    utPltPutLoopVarF(psDbHead, "money", iNum, "%lu", money);
                    utPltPutLoopVarF(psDbHead, "mtype", iNum, "%lu", mtype);
                }
                pasDbCloseCursor(psCur);
            }
            utPltPutVarF(psDbHead, "totalCount", "%lu", totalCount);
            utPltPutVar(psDbHead, "tsid", caTsid);
            utPltPutVarF(psDbHead, "result", "%d", 0);
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/pay_record.htm");
        }
    }

    return 0;
}

int ict_create_orderno(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	utMsgPrintMsg(psMsgHead);
	char sql[1024] = "";
	char orderno[32] = "";
	ulong maxId = 0;
	char username[32] = "";
	char caVname[32] = "";
	char money[32] = "";
	char mtype[4] = "";
	char passwd[32] = "";
	char caPasswd[32] = "";
	char mobno[16] = "";
	char mark[128] = "";
	char caTsid[32] = "";
	ulong lCount = 0;
	utMsgGetSomeNVar(psMsgHead, 5,
					 "tsid",  UT_TYPE_STRING,  sizeof(caTsid) - 1, caTsid,
                     "username",  UT_TYPE_STRING,  sizeof(username) - 1, username,
                     "money",  UT_TYPE_STRING,  sizeof(money) - 1, money,
                     "type", UT_TYPE_STRING,  sizeof(mtype) - 1, mtype,
                     "passwd", UT_TYPE_STRING,  sizeof(passwd) - 1, passwd);
	utPltDbHead *psDbHead = utPltInitDb();
	
	if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/payRecord/get_orderno.htm"))
    {
        return 0;
    }
	
	snprintf(caVname, sizeof(caVname) - 1, "%s", getVnameByTsId(psShmHead, atoll(caTsid)));

	if(strlen(username) == 0)
    {
        utPltPutVarF(psDbHead, "result", "%d", 3);//用户名空
		utPltPutVar(psDbHead, "mesg",  convert("GBK", "UTF-8", "用户名不能为空"));
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/get_orderno.htm");
    }
    else
    {
        /*memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql) - 1, "select count(*) from ncsrvuser where username='%s'", username);
        pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount <= 0)
        {
            utPltPutVarF(psDbHead, "result", "%d", 4);//用户名不存在
			utPltPutVar(psDbHead, "mesg",  convert("GBK", "UTF-8", "用户名输入错误"));
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/get_orderno.htm");
        }*/
		if(strcmp(caVname,username) != 0)
		{
			utPltPutVarF(psDbHead, "result", "%d", 4);//用户名输入错误
			utPltPutVar(psDbHead, "mesg",  convert("GBK", "UTF-8", "用户名输入错误"));
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/get_orderno.htm");		
		}
        else
        {		
            if(!checkMoney(money))
            {
                utPltPutVarF(psDbHead, "result", "%d", 5);//输入充值金额格式错误
                utPltPutVar(psDbHead, "mesg",  convert("GBK", "UTF-8", "充值金额必须为大于0的数"));
                utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/get_orderno.htm");
            }	
			else
			{
				memset(sql, 0, sizeof(sql));
				snprintf(sql, sizeof(sql)-1, "select ncsrvuserex.mobno,ncsrvuser.password from ncsrvuserex,ncsrvuser where ncsrvuserex.username='%s' and ncsrvuser.username='%s'", username, username);  
				pasDbOneRecord(sql, 0,  UT_TYPE_STRING, sizeof(mobno)-1, mobno,
										UT_TYPE_STRING, sizeof(caPasswd)-1, caPasswd);
				if(strcmp(passwd,caPasswd) != 0)
				{
					utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", "密码错误"));
					utPltPutVarF(psDbHead, "result", "%d", 6);
					utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/get_orderno.htm");
				}
				else
				{
					snprintf(mark, sizeof(mark) - 1, "%s", utComGetVar_sd(psShmHead, "mark", "0"));
					memset(sql, 0, sizeof(sql));
					snprintf(sql, sizeof(sql)-1, "select ifnull(max(id),0)+1 from rechargeTransdtl");
					pasDbOneRecord(sql, 0,  UT_TYPE_LONG, 4, &maxId);
					uint8 NowTime=time(0);
					snprintf(orderno,sizeof(orderno)-1, "%llu%09u", NowTime, maxId);
					printf("orderno %s\n",orderno);
					memset(sql, 0, sizeof(sql));
					snprintf(sql, sizeof(sql)-1, "insert into rechargeTransdtl(username,mobno,mark,money,mtype,orderno,timeval,orderstatus) values('%s','%s','%s',%lu,%lu,'%s',%llu,1)", username,mobno,mark,atol(money),atol(mtype),orderno,NowTime);
					printf("%s\n",sql);
					pasDbExecSqlF(sql);
					utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", "获取订单号成功"));
					utPltPutVarF(psDbHead, "result", "%d", 0);
					utPltPutVar(psDbHead, "orderno",orderno);
					utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/get_orderno.htm");
				}
			}
		}
	}	
	
	return 0;
}

int ict_recharge(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//账户充值
{
    utMsgPrintMsg(psMsgHead);
    long zjjDebug = utComGetVar_ld(psShmHead, "zjjDebug", 0);

    uint8 timeval = 0;
	char caTime[12] = "";
    int iReturn = 0;
	char sql[1024] = "";
	
	char caTsid[24] = "";
	char sign[64] = "";
	char orderno[64] = "";
	char orderstatus[12] = "";
	char caMsg[128] = "";
	
	char username[24] = "";
	char mobno[16] = "";
	char mark[128] = "";
	ulong lsign = 0;
	ulong money = 0;
	ulong mtype = 0;
    //获取前台数据
	utMsgGetSomeNVar(psMsgHead, 4,
					 "tsid", UT_TYPE_STRING,  sizeof(caTsid) - 1,  caTsid,
                     "orderno", UT_TYPE_STRING, sizeof(orderno)-1, orderno, 
					 "sign",  UT_TYPE_STRING,  sizeof(sign) - 1, sign,
					 "orderstatus", UT_TYPE_STRING, sizeof(orderstatus) - 1, orderstatus
                    );				
    utPltDbHead *psDbHead = utPltInitDb();

    if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/payRecord/pay_result.htm"))
    {
        return 0;
    }
	if(atol(orderstatus) == 2)
	{
		memset(sql, 0, sizeof(sql));
		snprintf(sql, sizeof(sql)-1, "update rechargeTransdtl set orderstatus=2 where orderno='%s'", orderno);
		printf("11 %s\n",sql);
		pasDbExecSqlF(sql);
		
		utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", "充值失败"));
		utPltPutVarF(psDbHead, "result", "%d", 3);//充值失败
		utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/pay_result.htm");
		
	}
	if(atol(orderstatus) == 3)
	{	
		printf("a1\n");
		memset(sql, 0, sizeof(sql));
		snprintf(sql, sizeof(sql)-1, "select username,mobno,mark,money,mtype from rechargeTransdtl where orderno='%s'", orderno);
		pasDbOneRecord(sql, 0,  UT_TYPE_STRING, sizeof(username)-1, username,
								UT_TYPE_STRING, sizeof(mobno)-1, mobno,
								UT_TYPE_STRING, sizeof(mark)-1,	mark,
								UT_TYPE_ULONG, 4, &money,
								UT_TYPE_ULONG, 4, &mtype);								
		
		if(strlen(username)==0)
		{
			utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", "订单号不存在"));
			utPltPutVarF(psDbHead, "result", "%d", 4);//订单号不存在
			utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/pay_result.htm");
			
			return 0;
		}
		 
		lsign=utMd5Code(orderno, strlen(orderno), username);
		if(atol(sign) != lsign)
		{
			;
		}
		 
		memset(sql, 0, sizeof(sql));//更新用户表余额
		snprintf(sql, sizeof(sql) - 1, "update ncsrvuserex set money=money+%lu where username='%s'", money, username);
		pasLogs(5005,5005,"充值语句 %s\n",sql);
		iReturn = pasDbExecSqlF(sql);
		if(iReturn != 0)
		{
			printf("a3\n");
			snprintf(caMsg, sizeof(caMsg) - 1, "数据库操作失败,错误码=[%d]", iReturn);
			utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
			utPltPutVarF(psDbHead, "result", "%d", 5);//充值失败
			utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/pay_result.htm");

		}
		else
		{
			
			timeval = time(0);
			sprintf(caTime, "%s", utTimFormat("%Y%m", timeval));
			printf("a4 %s\n",caTime);
			memset(sql, 0, sizeof(sql));//插入充值记录表
			snprintf(sql, sizeof(sql) - 1, "insert into rechargelog_%s(username,money,mtype,timeval,orderno,mobno,mark) values('%s',%lu,%lu,%llu,'%s','%s','%s')", caTime, username , money, mtype, timeval, orderno, mobno, mark);
			ictPrint(zjjDebug, "sql_rec=%s  \n", sql);
			iReturn = pasDbExecSqlF(sql);
			if(iReturn != 0)
			{
				printf("a5\n");
				snprintf(caMsg, sizeof(caMsg) - 1, "写入充值记录失败,错误码=[%d]", iReturn);
				utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
				utPltPutVarF(psDbHead, "result", "%d", 6);//写入充值记录失败
				utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/pay_result.htm");
				pasDbRollback(NULL);//错误回滚

			}
			else
			{
				printf("a6\n");
				memset(sql, 0, sizeof(sql));
				snprintf(sql, sizeof(sql)-1, "update rechargeTransdtl set orderstatus=3 where orderno='%s'", orderno);
				pasDbExecSqlF(sql);
				
				utPltPutVarF(psDbHead, "tsid", caTsid);
				utPltPutVarF(psDbHead, "result", "%d", 0);
				utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/pay_result.htm");
				pasDbCommit(NULL);//事物提交
			}
		}
	}  
    return 0;
}

int ict_check_user(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//注册时检测用户名是否能用
{
    utMsgPrintMsg(psMsgHead);
    long wpDebug = utComGetVar_ld(psShmHead, "wpDebug", 0);
    int iReturn = 0;
    char caVname[32] = "";
    char sqlbuf[1024] = "";
    char caMsg[256] = "";
    ulong lCount = 0;

    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "vname", UT_TYPE_STRING, sizeof(caVname) - 1, caVname
                              );
    utStrDelSpaces(caVname);
    utPltDbHead* psDbHead = utPltInitDb();

    if(strlen(caVname) > 0)
    {
        memset(sqlbuf, 0, sizeof(sqlbuf));
        //先判断数据库里是否有该caVname，有的话直接报错
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select count(*) from ncsrvuser where username='%s'", caVname);
        ictPrint(wpDebug, "sqlbuf=%s  \n", sqlbuf);
        pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount > 0)
        {
            snprintf(caMsg, sizeof(caMsg) - 1, "用户名已经存在");
            utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
            utPltPutVarF(psDbHead, "result", "%d", 1);//表示已经存在该vname
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/register/register.htm");
        }
        else
        {
            snprintf(caMsg, sizeof(caMsg) - 1, "用户名可以使用");
            utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
            utPltPutVarF(psDbHead, "result", "%d", 3);//表示用户名可以使用
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/register/register.htm");
        }
    }
    else
    {
        snprintf(caMsg, sizeof(caMsg) - 1, "用户名不可以为空");
        utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
        utPltPutVarF(psDbHead, "result", "%d", 2);//表示用户名不可以为空
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/register/register.htm");
    }

    return 0;
}

int ict_register(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//注册

{
    //sql语句处理有三种，查询的话用pasDbOneRecord
    //pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);一般是只返回一个数据，将数据存在lCount中
    //insert,delete,update等用pasDbExecSqlF?
    //查询语句用 psCur = pasDbOpenSql(sql, 0);

    long wpDebug = utComGetVar_ld(psShmHead, "wpDebug", 0);

    int iReturn = 0;
    int iReturn1 = 0;

    utMsgPrintMsg(psMsgHead);

    char caVname[32] = "";
    char caPasswd[24] = "";
    char caPhoto[128] = "";
    char caDname[24] = "";
    char caMname[24] = "";
    char caGender[12] = "";
	//char iGender[4] = "";
    char caCid[24] = "";
    char caStudentid[24] = "";
    char caCollege[128] = "";
    char caDpartment[128] = "";
    char caMobno[24] = "";
    char caEmail[24] = "";
    char caQQ[24] = "";
    char caHobby[128] = "";
    char caAddress[128] = "";
    //   ulong lCount = 0;
    char caMsg[256] = "";
    char sql[1024] = "";
    char sqlbuf[1024] = "";
    char caGroupname_t[64] = "";
    char caTemp[1024] = "";
    char caTsid[32] = "";
	int irett =0;

    iReturn = utMsgGetSomeNVar(psMsgHead, 16,
                               "vname",   UT_TYPE_STRING, sizeof(caVname) - 1, caVname,
                               "passwd",  UT_TYPE_STRING, sizeof(caPasswd) - 1, caPasswd,
                               "photo",   UT_TYPE_STRING, sizeof(caPhoto) - 1, caPhoto,
                               "dname",   UT_TYPE_STRING, sizeof(caDname) - 1, caDname,
                               "mname",  UT_TYPE_STRING, sizeof(caMname) - 1, caMname,
                               "gender",  UT_TYPE_STRING, sizeof(caGender) - 1, caGender,
                               "cid",  UT_TYPE_STRING, sizeof(caCid) - 1, caCid,
                               "studentid",   UT_TYPE_STRING, sizeof(caStudentid) - 1, caStudentid,
                               "college",  UT_TYPE_STRING, sizeof(caCollege) - 1, caCollege,
                               "department",   UT_TYPE_STRING, sizeof(caDpartment) - 1, caDpartment,
                               "mobno",   UT_TYPE_STRING, sizeof(caMobno) - 1, caMobno,
                               "email",  UT_TYPE_STRING, sizeof(caEmail) - 1, caEmail,
                               "qq",   UT_TYPE_STRING, sizeof(caQQ) - 1, caQQ,
                               "hobby",   UT_TYPE_STRING, sizeof(caHobby) - 1, caHobby,
                               "address",  UT_TYPE_STRING, sizeof(caAddress) - 1, caAddress,
                               "PASTSID", UT_TYPE_STRING, sizeof(caTsid) - 1 , caTsid
                              );
	zjj_delete_shm(psShmHead, caMobno);//删除短信验证码内存
    utStrDelSpaces(caVname);
    utPltDbHead* psDbHead = utPltInitDb();
    //插入数据库
    char caMark[20] = "";
    snprintf(caMark, sizeof(caMark) - 1, "%s", utComGetVar_sd(psShmHead, "mark", "0"));
    //strcpy(caMark, utComGetVar_sd(psShmHead, "mark", "0"));
    unsigned long lGroupid = 0;
    unsigned long lSid;
    //memset(caGroupname_t,0,sizeof(caGroupname_t));

    utStrGetSomeWordBy(caMark, "_", 1,
                       UT_TYPE_LONG, 4, &lGroupid);
    sprintf(caTemp, "%d_%s", lGroupid, caVname);

    lSid = utMd5Code(caTemp, strlen(caTemp), "ffff");
    if(lSid > 2000000000) lSid = lSid - 2000000000;

    if(lSid > 2000000000) lSid = lSid - 2000000000;
    memset(sqlbuf, 0, sizeof(sqlbuf));

    snprintf(sqlbuf, sizeof(sqlbuf) - 1, "insert into ncsrvuserex(username,photo,dname,mname,gender,cid,studentid,college,department,mobno,email,qq,hobby,address) values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",

             caVname, caPhoto, convert("UTF-8", "GBK", caDname), convert("UTF-8", "GBK", caMname), caGender, caCid, caStudentid, convert("UTF-8", "GBK", caCollege), convert("UTF-8", "GBK", caDpartment), caMobno, caEmail, caQQ, convert("UTF-8", "GBK", caHobby), convert("UTF-8", "GBK", caAddress));

    ictPrint(wpDebug, "insert sqlbuf=%s\n", sqlbuf);
    iReturn = pasDbExecSqlF(sqlbuf);
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql) - 1, "insert into ncsrvuser(userid,username,password) values('%lu','%s','%s')", lSid, caVname, caPasswd);
    ictPrint(wpDebug, "insert sql=%s\n", sql);
	pasLogs(5003,5003,"sqlbuf=%s\n",sqlbuf);
	pasLogs(5003,5003,"sql=%s\n",sql);
//    iReturn1 = pasDbExecSqlF(sql);
	pasLogs(1234, 1233, "iReturn=%d\n", iReturn);
	pasLogs(1234, 1233, "iReturn1=%d\n", iReturn1);

    if(iReturn != 0 || iReturn1 != 0)
    {
        snprintf(caMsg, sizeof(caMsg) - 1, "操作数据库失败,错误码=[%d]", iReturn);
        snprintf(caMsg, sizeof(caMsg) - 1, "操作数据库失败,错误码=[%d]", iReturn1);
        utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
        utPltPutVarF(psDbHead, "result", "%d", 4);
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/register/register.htm");
        //sql语句操作回滚
        //pasDbRollback(NULL);
        return 0;
    }

    else
    {
        char caGroupCode[32] = "";
        char caUserIp[32] = "";
        ncPortalSummary *psSumm;
        ncPortalOnline* psOnline = NULL;
        psSumm = (ncPortalSummary *)utShmArray(psShmHead, NCSRV_LNK_SYSINFO);
        psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, atoll(caTsid));
		iUpdateUserFlags = 2;
        if(psOnline)
        {
			pasLogs(1234, 1233, "psOnline not null");
            psOnline->psUser = ncSrvSynUserPassword(psShmHead, psOnline->lStype, psOnline->lGroupid, caVname, caPasswd, psOnline->caGroupCode, psOnline->caSsid, NCSRV_USERTYPE_SMS);
            if(iUpdateUserFlags == 2)
            {
                irett = ncSrvSynUserPassword2RadSrv(psOnline->psPar, caVname, caPasswd, psOnline->lStype);
                pasLogs(PAS_SRCFILE, 1008, " Update User [%s]  Pass:[%s] Group:%s Ssid:%s  Stype:%u Ret=%d ",
                        caVname, caPasswd, psOnline->caGroupCode, psOnline->caSsid, psOnline->lStype, iReturn);
				pasLogs(1234, 1233, "after ncSrvSynUserPassword2RadSrv,ret=%d", irett);
            }
            strcpy(caGroupCode, psOnline->caGroupCode);
            if(utStrIsSpaces(caUserIp))
            {
                strcpy(caUserIp, utComHostIp(htonl(psOnline->lSip)));
            }
			pasLogs(1234, 1233, "after ncSrvSynUserPassword,iUpdateUserFlags=%d", iUpdateUserFlags);
        }
        else
        {
			pasLogs(1234, 1233, "psOnline is null");
            
			ncSrvSynUserPassword(psShmHead, 0, 0, caVname, caPasswd, "\0", utComGetVar_sd(psShmHead, "DefSsid", "\0"), NCSRV_USERTYPE_SMS);
            if(iUpdateUserFlags == 2)
            {
                irett = ncSrvSynUserPassword2RadSrv(psSumm->psPar, caVname, caPasswd, 0);
                pasLogs(PAS_SRCFILE, 1008, " Update User [%s]  Pass:[%s] iReturn=%d ",
                        caVname, caPasswd, iReturn);
				pasLogs(1234, 1233, "after ncSrvSynUserPassword2RadSrv,ret=%d", irett);
            }else{
				pasLogs(1234, 1233, "Error Update User [%s]  Pass:[%s] iReturn=%d ",
                        caVname, caPasswd, iReturn);
			}
			pasLogs(1234, 1233, "after ncSrvSynUserPassword,iUpdateUserFlags=%d", iUpdateUserFlags);
        }

        //indexTsid = getTsidByVname(psShmHead, caVname);
		setOnlineVname(psShmHead, caVname, atoll(caTsid));
        utPltPutVarF(psDbHead, "tsid", "%llu", getTsidByVname(psShmHead, caVname));
        utPltPutVarF(psDbHead, "result", "%d", 0);
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/register/register.htm");
    }


    return 0;

}

/*int ict_AuthMobile(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//手机端登录
{
    int iReturn = 0;
    utMsgPrintMsg(psMsgHead);
    char caVname[32 + 1] = "";
    char caPasswd[24 + 1] = "";
    char sqlbuf[1024] = "";
    ulong lCount = 0;
    char caMsg[256] = "";

    long wpDebug = utComGetVar_ld(psShmHead, "wpDebug", 0);
    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "vname",   UT_TYPE_STRING, sizeof(caVname) - 1, caVname,
                               "passwd",  UT_TYPE_STRING, sizeof(caPasswd) - 1, caPasswd
                               );
    utStrDelSpaces(caVname);
    utPltDbHead* psDbHead = utPltInitDb();
    if(strlen(caVname) > 0)
    {
        memset(sqlbuf, 0, sizeof(sqlbuf));
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select count(*) from ncsrvuser where username='%s' and password='%s'", caVname, caPasswd);
        ictPrint(wpDebug, "sqlbuf=%s  \n", sqlbuf);
        pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount <= 0)
        {
            snprintf(caMsg, sizeof(caMsg) - 1, "用户名不存在或密码错误", caVname);
            utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
            utPltPutVarF(psDbHead, "result", "%d", 1);



        }
        else
        {
            //将用户加入共享内存
            addUserToShm(caVname);
            utPltPutVarF(psDbHead, "result", "%d", 0);
            uint8 llTsid = getTsidByVname(psShmHead, caVname);
            utPltPutVarF(psDbHead, "tsid", "%llu", llTsid);
        }
    }
    else
    {
        snprintf(caMsg, sizeof(caMsg) - 1, "用户名不可以为空");
        utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
        utPltPutVarF(psDbHead, "result", "%d", 2);
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/login/login.htm");
    return 0;
}*/

int ict_Auth(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//pc端登录
{
    int iReturn = 0;
    utMsgPrintMsg(psMsgHead);
    char caVname[32 + 1] = "";
    char caPasswd[24 + 1] = "";
    char sqlbuf[1024] = "";
    ulong lCount = 0;
    char caMsg[256] = "";

    long wpDebug = utComGetVar_ld(psShmHead, "wpDebug", 0);
    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "vname",   UT_TYPE_STRING, sizeof(caVname) - 1, caVname,
                               "passwd",  UT_TYPE_STRING, sizeof(caPasswd) - 1, caPasswd);
    utStrDelSpaces(caVname);
    utPltDbHead* psDbHead = utPltInitDb();
    if(strlen(caVname) > 0)
    {
        memset(sqlbuf, 0, sizeof(sqlbuf));
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select count(*) from ncsrvuser where username='%s' and password='%s'", caVname, caPasswd);
        ictPrint(wpDebug, "sqlbuf=%s  \n", sqlbuf);
        pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount <= 0)
        {

            snprintf(caMsg, sizeof(caMsg) - 1, "用户名不存在或密码错误", caVname);
            utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
            utPltPutVarF(psDbHead, "result", "%d", 1);
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/main/login_error.htm");
        }
        else
        {

            //将用户加入共享内存
            addUserToShm(caVname);
            utPltPutVarF(psDbHead, "result", "%d", 0);
            //indexTsid=getTsidByVname(psShmHead, caVname);
            utPltPutVarF(psDbHead, "tsid", "%llu", getTsidByVname(psShmHead, caVname));
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/login/login_main1.htm");
        }
    }
    else
    {
        snprintf(caMsg, sizeof(caMsg) - 1, "用户名不可以为空");
        utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
        utPltPutVarF(psDbHead, "result", "%d", 2);
        //utPltPutVarF(psDbHead, "tsid", "%llu", getTsidByVname(psShmHead, caVname));
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/main/login_error.htm");
    }

    return 0;
}

int ict_update_userInfo(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//修改用户信息
{
    utMsgPrintMsg(psMsgHead);

    char caTsid[32] = "";
    char caVname[32] = "";
    char caPhoto[128] = "";
    char caDname[24] = "";
    char caMname[24] = "";
    char caGender[12] = "";
    char caCid[24] = "";
    char caStudentid[24] = "";
    char caCollege[128] = "";
    char caDpartment[128] = "";
    char caMobno[24] = "";
    char caEmail[24] = "";
    char caQQ[24] = "";
    char caHobby[128] = "";
    char caAddress[128] = "";
    char sql[1024] = "";
    char caMsg[256] = "";
    int iReturn = 0;

    iReturn = utMsgGetSomeNVar(psMsgHead, 14,
                               "tsid",   UT_TYPE_STRING, sizeof(caTsid) - 1, caTsid,
                               "photo",  UT_TYPE_STRING, sizeof(caPhoto) - 1, caPhoto,
                               "dname",  UT_TYPE_STRING, sizeof(caDname) - 1, caDname,
                               "mname",  UT_TYPE_STRING, sizeof(caMname) - 1, caMname,
                               "gender", UT_TYPE_STRING, sizeof(caGender) - 1, caGender,
                               "cid",    UT_TYPE_STRING, sizeof(caCid) - 1, caCid,
                               "studentid",   UT_TYPE_STRING, sizeof(caStudentid) - 1, caStudentid,
                               "college",  UT_TYPE_STRING, sizeof(caCollege) - 1, caCollege,
                               "department",   UT_TYPE_STRING, sizeof(caDpartment) - 1, caDpartment,
                               "mobno",   UT_TYPE_STRING, sizeof(caMobno) - 1, caMobno,
                               "email",  UT_TYPE_STRING, sizeof(caEmail) - 1, caEmail,
                               "qq",   UT_TYPE_STRING, sizeof(caQQ) - 1, caQQ,
                               "hobby",   UT_TYPE_STRING, sizeof(caHobby) - 1, caHobby,
                               "address",  UT_TYPE_STRING, sizeof(caAddress) - 1, caAddress
                              );
    utStrDelSpaces(caVname);
    utPltDbHead* psDbHead = utPltInitDb();

    if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/updateUserInfo/ict_update_userInfo.htm"))
    {
        return 0;
    }
    snprintf(caVname, sizeof(caVname) - 1, "%s", getVnameByTsId(psShmHead, atoll(caTsid)));
    //  strcpy(caVname, getVnameByTsId(psShmHead, atoll(caTsid)));


    snprintf(sql, sizeof(sql) - 1, "update ncsrvuserex set ");

    if(strlen(caPhoto) > 0)
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "photo='%s',", convert("UTF-8", "GBK", caPhoto));
    }

    if(strlen(caDname) > 0)
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "dname='%s',", convert("UTF-8", "GBK", caDname));
    }

    if(strlen(caMname) > 0)
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "mname='%s',", convert("UTF-8", "GBK", caMname));
    }

    if(strlen(caGender) > 0)
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "gender='%s',", caGender);
    }
    if(strlen(caCid) > 0)
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "cid='%s',", caCid);
    }
    if(strlen(caStudentid) > 0)
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "studentid='%s',", caStudentid);
    }
    if(strlen(caCollege) > 0)
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "college='%s',", convert("UTF-8", "GBK", caCollege));
    }
    if(strlen(caDpartment) > 0)
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "department='%s',", convert("UTF-8", "GBK", caDpartment));
    }
    if(strlen(caMobno) > 0)
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "mobno='%s',", caMobno);
    }
    if(strlen(caEmail) > 0)
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "email='%s',", convert("UTF-8", "GBK", caEmail));
    }
    if(strlen(caQQ) > 0)
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "qq='%s',", caQQ);
    }
    if(strlen(caHobby) > 0)
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "hobby='%s',", convert("UTF-8", "GBK", caHobby));
    }
    if(strlen(caAddress) > 0)
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "address='%s',", convert("UTF-8", "GBK", caAddress));
    }

    snprintf(sql + strlen(sql) - 1, sizeof(sql) - strlen(sql), " where username='%s'", caVname);
    //snprintf(sql, sizeof(sql) - 1, "update ncsrvuserex set photo='%s',dname='%s',mname='%s',gender='%s',cid='%s',studentid='%s',college='%s',department='%s',mobno='%s',email='%s',qq='%s',hobby='%s',address='%s' where username='%s'",
    //             convert("UTF-8", "GBK", caPhoto), convert("UTF-8", "GBK", caDname), convert("UTF-8", "GBK", caMname), convert("UTF-8", "GBK", caGender), caCid, caStudentid, convert("UTF-8", "GBK", caCollege), convert("UTF-8", "GBK", caDpartment), caMobno, convert("UTF-8", "GBK", caEmail), caQQ, convert("UTF-8", "GBK", caHobby), convert("UTF-8", "GBK", caAddress),caVname);

    iReturn = pasDbExecSqlF(sql);
    if(iReturn != 0)
    {
        snprintf(caMsg, sizeof(caMsg) - 1, "修改个人信息失败,错误码=[%d]", iReturn);
        utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
        utPltPutVarF(psDbHead, "result", "%d", 3);//更新用户信息失败
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/updateUserInfo/ict_update_userInfo.htm");
        return 0;
    }
    else
    {
        utPltPutVar(psDbHead, "tsid", caTsid);
        utPltPutVarF(psDbHead, "result", "%d", 0);
        //utPltPutVarF(psDbHead, "result", convert("GBK", "UTF-8", "修改个人信息成功"));
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/updateUserInfo/ict_update_userInfo.htm");

    }

    return 0;
}

int ictSrvUserGetPass(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//获取验证码
{
    long wpDebug = utComGetVar_ld(psShmHead, "wpDebug", 0);
    if(wpDebug)
        utMsgPrintMsg(psMsgHead);
    char caPosturl[128];
    char username[32] = {0};
    char postarg[512] = {0};
    long lStatus;
    char caTsid[24];
    memset(caTsid, 0, sizeof(caTsid));
    memset(username, 0, sizeof(username));
    // 获取手机号
    utMsgGetSomeNVar(psMsgHead, 4,
                     "username",  UT_TYPE_STRING,  sizeof(username) - 1, username,
                     "postarg",   UT_TYPE_STRING,  sizeof(postarg) - 1,  postarg,
                     "tsid",   UT_TYPE_STRING, sizeof(caTsid) - 1, caTsid,
                     "posturl", UT_TYPE_STRING, sizeof(caPosturl) - 1, caPosturl
                    );
    ictPrint(wpDebug, "\n 手机号为 %s \n", username);
    ictPrint(wpDebug, "\n tsid=%s \n", caTsid);
    ictPrint(wpDebug, "posturl=%s\n", caPosturl);
    ictPrint(wpDebug, "postarg=%s\n", postarg);

    utMsgHead *psMsgHead2;
    ncPortalOnline *psOnline = NULL;
    long status = -1;
    char server_ip[100] = {0};
    char server_port[100] = {0};
    char caIp[32];
    unsigned long long llTsid;
    // 获取丹尼斯接口的IP地址和端口号
    strcpy(server_ip,  utComGetVar_sd(psShmHead, "verify_ip",  ""));
    strcpy(server_port, utComGetVar_sd(psShmHead, "verify_port", ""));
    strcpy(caIp, utComGetVar_sd(psShmHead, "AdminIp", ""));

    sscanf(caTsid, "%llu", &llTsid);
    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
    if(psOnline)
    {
        lStatus = ncSrvGetUserStatus(psShmHead, psOnline->lStype, username);
        ictPrint(wpDebug, "lStatus=%d,username=%s,stype=%d \n", lStatus, username, psOnline->lStype);
    }
    else
    {

        lStatus = ncSrvGetUserStatus(psShmHead, 4100021, username);
        ictPrint(wpDebug, "_1lStatus=%d,username=%s,stype=%d \n", lStatus, username, 0);
    }

    if(lStatus == 9)
    {
        utPltDbHead *psDbHead = utPltInitDbHead();
        utPltPutVar(psDbHead, "status", "sforbidden");
        utPltPutVar(psDbHead, "posturl", caPosturl);
        utPltPutVar(psDbHead, "postarg", postarg);
        utPltPutVar(psDbHead, "ip", caIp);
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "genal/sms/redirect2.htm");
        return 0;
    }


    ictPrint(wpDebug, "\n server_ip = %s \n", server_ip);
    ictPrint(wpDebug, "\n server_port = %s \n", server_port);
    unsigned long ip = utComHostAddress(server_ip);
    unsigned long port = atol(server_port);
    ictPrint(wpDebug, "\n port1 = %d \n", port);
    port = ntohs(port);
    ictPrint(wpDebug, "\n port2 = %d \n", port);
    psMsgHead2 = NULL;
    status = 1;
    psMsgHead2 = pasTcpRequest(ip, port,
                               0,        /* CheckSum */
                               0,        /* Encrypt  */
                               "ncSrvGetPassword",
                               0,       /* Sid */
                               0,       /* Rid  */
                               30,      /* 超时  */
                               NULL,    /* key   */
                               1,
                               "username", UT_TYPE_STRING, username);

    if(psMsgHead2)
    {
        int iReturn = utMsgGetSomeNVar(psMsgHead2, 1,
                                       "status", UT_TYPE_LONG, 4, &status);

        utMsgFree(psMsgHead2);
    }

    ictPrint(wpDebug, "\n status = %d \n", status);
    /*
    ncSrvGetPass(psShmHead, iFd, psMsgHead);
    return;
    */
    if(status == 1)     // 会员直接获取验证码
    {
        ictPrint(wpDebug, "\n 会员 \n");
        ncSrvGetPass(psShmHead, iFd, psMsgHead);
    }
    else    // 非会员则跳到注册页
    {
        ictPrint(wpDebug, "\n 非会员 \n");
        utPltDbHead *psDbHead = utPltInitDbHead();
        utPltPutVar(psDbHead, "status", "forbidden");
        utPltPutVar(psDbHead, "posturl", caPosturl);
        utPltPutVar(psDbHead, "postarg", postarg);
        utPltPutVar(psDbHead, "ip", caIp);
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "genal/sms/redirect2.htm");
        //utPltOutToHtml(iFd,psMsgHead,psDbHead,"genal/sms/Register.html");
    }
    //  printf("\n 离开ictSrvUserGetPass \n");
    return 0;
}

int ict_getUserInfo(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//获取用户信息

{
    utMsgPrintMsg(psMsgHead);

    long wpDebug = utComGetVar_ld(psShmHead, "wpDebug", 0);
    char caTsid[32] = "";
    char caVname[32] = "";

    char caPhoto[128] = "";
    char caDname[24] = "";
    char caMname[24] = "";
    char caGender[12] = "";
    char caCid[24] = "";
    char caStudentid[24] = "";
    char caCollege[128] = "";
    char caDepartment[128] = "";
    char caMobno[24] = "";
    char caEmail[24] = "";
    char caQQ[24] = "";
    char caHobby[128] = "";
    char caAddress[128] = "";
    char pMoney[24] = "";//余额

    char packageId[4] = "";
    char namech[32] = "";
    char namedes[128] = "";
    char caBname[32] = "";
    char lMoney[24] = "";//套餐价格
    char startTime[12] = "";
    char endTime[12] = "";
    char autoxiding[4] = "";

    char sql[1024] = "";
    char caMsg[256] = "";
    int iReturn = 0;

    utPltDbHead *psDbHead = utPltInitDbHead();
    pasDbCursor *psCur = NULL;

    utMsgGetSomeNVar(psMsgHead, 1,
                     "tsid",   UT_TYPE_STRING, sizeof(caTsid) - 1, caTsid);

    utStrDelSpaces(caTsid);
    ictPrint(wpDebug, "tsid=%s\n", caTsid);
    if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/main/userInfo.htm"))
    {
        return 0;
    }

    //暂时把所有信息都返回
    snprintf(caVname, sizeof(caVname) - 1, "%s", getVnameByTsId(psShmHead, atoll(caTsid)));
    //strcpy(caVname, getVnameByTsId(psShmHead, atoll(caTsid)));

    // utPltPutVar(psDbHead, "vname", caVname);

    //查表得到vname对应的信息

    /* memset(sql, 0, sizeof(sql));

    uint8 nowTime = time(0);
     snprintf(sql, sizeof(sql) - 1, "select ncsrvuserex.photo,ncsrvuserex.dname,ncsrvuserex.mname,ncsrvuserex.gender,ncsrvuserex.cid,ncsrvuserex.studentid,ncsrvuserex.college,ncsrvuserex.department,ncsrvuserex.mobno,ncsrvuserex.email,ncsrvuserex.qq,ncsrvuserex.hobby,ncsrvuserex.address,ncsrvuserex.money,userorder.packageid,package.name,package.namech,package.namedes,package.money,from_unixtime(userorder.starttime,'%%Y-%%m-%%d'),from_unixtime(userorder.endtime,'%%Y-%%m-%%d'),userorder.autoxiding from ncsrvuserex,package,userorder where userorder.packageid=package.id and ncsrvuserex.username='%s' and userorder.username='%s' and userorder.starttime<=%llu and userorder.endtime > %llu", caVname, caVname, nowTime, nowTime);

     ictPrint(wpDebug, "sql=%s  \n", sql);*/
    uint8 nowTime = time(0);
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql) - 1, "select ncsrvuserex.photo,ncsrvuserex.dname,ncsrvuserex.mname,ncsrvuserex.gender,ncsrvuserex.cid,ncsrvuserex.studentid,ncsrvuserex.college,ncsrvuserex.department,ncsrvuserex.mobno,ncsrvuserex.email,ncsrvuserex.qq,ncsrvuserex.hobby,ncsrvuserex.address,ncsrvuserex.money from ncsrvuserex where username='%s'", caVname);
    pasDbOneRecord(sql, 0,   UT_TYPE_STRING, sizeof(caPhoto) - 1, caPhoto,
                   UT_TYPE_STRING, sizeof(caDname) - 1, caDname,
                   UT_TYPE_STRING, sizeof(caMname) - 1, caMname,
                   UT_TYPE_STRING, sizeof(caGender) - 1, caGender,
                   UT_TYPE_STRING, sizeof(caCid) - 1, caCid,
                   UT_TYPE_STRING, sizeof(caStudentid) - 1, caStudentid,
                   UT_TYPE_STRING, sizeof(caCollege) - 1, caCollege,
                   UT_TYPE_STRING, sizeof(caDepartment) - 1, caDepartment,
                   UT_TYPE_STRING, sizeof(caMobno) - 1, caMobno,
                   UT_TYPE_STRING, sizeof(caEmail) - 1, caEmail,
                   UT_TYPE_STRING, sizeof(caQQ) - 1, caQQ,
                   UT_TYPE_STRING, sizeof(caHobby) - 1, caHobby,
                   UT_TYPE_STRING, sizeof(caAddress) - 1, caAddress,
                   UT_TYPE_STRING, sizeof(pMoney) - 1, pMoney
                  );

    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql) - 1, "select userorder.id,package.name,package.namech,package.namedes,package.money,from_unixtime(userorder.starttime,'%%Y-%%m-%%d'),from_unixtime(userorder.endtime,'%%Y-%%m-%%d'),userorder.autoxiding from userorder,package where userorder.packageid=package.id and userorder.username='%s' and userorder.starttime<=%llu and userorder.endtime>%llu and userorder.status=1", caVname, nowTime, nowTime);
    pasDbOneRecord(sql, 0,    UT_TYPE_STRING, sizeof(packageId) - 1, packageId,
                   UT_TYPE_STRING, sizeof(caBname) - 1, caBname,
                   UT_TYPE_STRING, sizeof(namech) - 1, namech,
                   UT_TYPE_STRING, sizeof(namedes) - 1, namedes,
                   UT_TYPE_STRING, sizeof(lMoney) - 1, lMoney,
                   UT_TYPE_STRING, sizeof(startTime) - 1, startTime,
                   UT_TYPE_STRING, sizeof(endTime) - 1, endTime,
                   UT_TYPE_STRING, sizeof(autoxiding) - 1, autoxiding
                  );


    utPltPutVar(psDbHead, "caPhoto", convert("GBK", "UTF-8", caPhoto));
    utPltPutVar(psDbHead, "caDname", convert("GBK", "UTF-8", caDname));
    utPltPutVar(psDbHead, "caMname", convert("GBK", "UTF-8", caMname));
    utPltPutVar(psDbHead, "caGender", caGender);
    utPltPutVar(psDbHead, "caCid", caCid);
    utPltPutVar(psDbHead, "caStudentid", caStudentid);
    utPltPutVar(psDbHead, "caCollege", convert("GBK", "UTF-8", caCollege));
    utPltPutVar(psDbHead, "caDepartment", convert("GBK", "UTF-8", caDepartment));
    utPltPutVar(psDbHead, "caMobno", caMobno);
    utPltPutVar(psDbHead, "caEmail", convert("GBK", "UTF-8", caEmail));
    utPltPutVar(psDbHead, "caQQ", caQQ);
    utPltPutVar(psDbHead, "caHobby", convert("GBK", "UTF-8", caHobby));
    utPltPutVar(psDbHead, "caAddress", convert("GBK", "UTF-8", caAddress));
    utPltPutVar(psDbHead, "pMoney", pMoney);

    utPltPutVar(psDbHead, "packageid", packageId);
    utPltPutVar(psDbHead, "caBname", convert("GBK", "UTF-8", caBname));
    utPltPutVar(psDbHead, "namech", convert("GBK", "UTF-8", namech));
    utPltPutVar(psDbHead, "namedes", convert("GBK", "UTF-8", namedes));
    utPltPutVar(psDbHead, "lMoney", lMoney);
    utPltPutVar(psDbHead, "startTime", startTime);
    utPltPutVar(psDbHead, "endTime", endTime);
    utPltPutVar(psDbHead, "autoxiding", autoxiding);

    utPltPutVar(psDbHead, "caVname", caVname);
    utPltPutVar(psDbHead, "tsid", caTsid);
    utPltPutVarF(psDbHead, "result", "%d", 0);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/main/userInfo.htm");

    return 0;
}


int ict_submit_form(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caPlatePath[64] = "";
    utPltDbHead* psDbHead = utPltInitDb();

    utMsgGetSomeNVar(psMsgHead, 1,
                     "platepath",   UT_TYPE_STRING, sizeof(caPlatePath) - 1, caPlatePath);
    //utPltPutVarF(psDbHead, "tsid", "%llu", indexTsid);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlatePath);

    return 0;
}

int ict_index(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utMsgPrintMsg(psMsgHead);
    char caTsid[32] = "";
    utMsgGetSomeNVar(psMsgHead, 1, "PASTSID", UT_TYPE_STRING,  sizeof(caTsid) - 1,  caTsid
                    );
	printf("zhaojianjian tsid %s\n", caTsid);
    utPltDbHead* psDbHead = utPltInitDb();
    utPltPutVar(psDbHead, "tsid", caTsid);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/main/index.htm");

    return 0;
}

int ict_get_province(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//获取省接口
{
    utMsgPrintMsg(psMsgHead);
    char  sql[1024] = "";
    char caProvince[64] = "";
    char caProvinceCode[16] = "";
    char caCity[64] = "";

    struct data
    {
        char dpname[128];
        char dpcode[16];
        struct data *next;
    };

    struct data *head = NULL;
    struct data *q = NULL;
    struct data *p = NULL;
    struct data *newnode = NULL;

    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();

    snprintf(sql, sizeof(sql) - 1, "select province_name,province_code from tProvince");
    psCur = pasDbOpenSql(sql, 0);


    int iret = 0;
    if(psCur)
    {
        while(0 == (iret = pasDbFetchInto(psCur,
                                          UT_TYPE_STRING, sizeof(caProvince) - 1, caProvince,
                                          UT_TYPE_STRING, sizeof(caProvinceCode) - 1, caProvinceCode)) || 1405 == iret)
        {
            newnode = (struct data *)malloc(sizeof(struct data));
            snprintf(newnode->dpname, sizeof(newnode->dpname) - 1, "%s", caProvince);
            snprintf(newnode->dpcode, sizeof(newnode->dpcode) - 1, "%s", caProvinceCode);
            newnode->next = NULL;
            if(head == NULL)
            {
                head = newnode;
                q = head;
            }
            else
            {
                q->next = newnode;
                q = newnode;
            }
        }
        pasDbCloseCursor(psCur);
    }

    int iret1 = 0;
    int iNum = 0;
    int iNum1 = 0;

    p = head;
    while(p != NULL)
    {
        iNum++;
        if(iNum > 1)
        {
            utPltPutLoopVar(psDbHead, "dh", iNum, ",");
        }

        utPltPutLoopVar(psDbHead, "province", iNum, convert("GBK", "UTF-8", p->dpname));
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql) - 1, "select city_name from tCity where province_code='%s'", p->dpcode);
        psCur = pasDbOpenSql(sql, 0);
        if(psCur)
        {
            iNum1 = 0;
            while(0 == (iret = pasDbFetchInto(psCur,
                                              UT_TYPE_STRING, sizeof(caCity) - 1, caCity)) || 1405 == iret)
            {
                iNum1++;
                if(iNum1 > 1)
                {
                    utPltPutLoopVar2(psDbHead, "dh1", iNum, iNum1, ",");
                }
                utPltPutLoopVar2(psDbHead, "city", iNum, iNum1, convert("GBK", "UTF-8", caCity));
            }
            pasDbCloseCursor(psCur);
        }
        p = p->next;
    }

    p = head;
    while(p != NULL)//释放单链表
    {
        q = p->next;
        free(p);
        p = q;
    }
    p = NULL;
    q = NULL;
    head = NULL;
    newnode = NULL;

    utPltPutVarF(psDbHead, "result", "%d", 0);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/selectAddress/show_province.htm");
}

/*int ict_get_city(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//获取市
{
    utMsgPrintMsg(psMsgHead);
    char  sql[1024] = "";
    char caCity[64] = "";
    char caCityCode[16] = "";
    char province_code[16]="";
    int iret = 0;
    int iNum = 0;
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();

     utMsgGetSomeNVar(psMsgHead, 1,
                     "province_code", UT_TYPE_STRING,  sizeof(province_code) - 1, province_code);

    snprintf(sql, sizeof(sql)-1, "select city_code,city_name from tCity where province_code='%s'",province_code);
    psCur = pasDbOpenSql(sql, 0);

    if(psCur)
    {
        while(0 == (iret = pasDbFetchInto(psCur,
                                                UT_TYPE_STRING, sizeof(caCityCode) - 1, caCityCode,
                                                UT_TYPE_STRING, sizeof(caCity) - 1, caCity)) || 1405 == iret)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVarF(psDbHead, "city_code", iNum,caCityCode);
            utPltPutLoopVarF(psDbHead, "city", iNum,convert("GBK", "UTF-8",caCity));
        }
        pasDbCloseCursor(psCur);
    }
    utPltPutVarF(psDbHead, "totalCount", "%d", iNum);
    utPltPutVarF(psDbHead, "result", "%d", 0);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/selectAddress/show_city.htm");
}*/

int ict_get_college(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//获取学院
{
    utMsgPrintMsg(psMsgHead);
    char  sql[1024] = "";
    char  sql_tmp[1024] = "";
    char caCollege[64] = "";
    char caCollegeCode[16] = "";
    char caDepartment[64] = "";

    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();

    struct data
    {
        char dpname[128];
        char dpcode[16];
        struct data *next;
    };
    struct data *head = NULL;
    struct data *q = NULL;
    struct data *p = NULL;
    struct data *newnode = NULL;

    snprintf(sql, sizeof(sql) - 1, "select depName,depCode from tDepartment where depLevel=0");
    psCur = pasDbOpenSql(sql, 0);

    int iret = 0;

    if(psCur)
    {
        while(0 == (iret = pasDbFetchInto(psCur,
                                          UT_TYPE_STRING, sizeof(caCollege) - 1, caCollege,
                                          UT_TYPE_STRING, sizeof(caCollegeCode) - 1, caCollegeCode)) || 1405 == iret)
        {
            newnode = (struct data*)malloc(sizeof(struct data));
            snprintf(newnode->dpname, sizeof(newnode->dpname) - 1, "%s", caCollege);
            snprintf(newnode->dpcode, sizeof(newnode->dpcode) - 1, "%s", caCollegeCode);
            newnode->next = NULL;//
            if(head == NULL)
            {
                head = newnode;
                q = head;
            }
            else
            {
                q->next = newnode;
                q = newnode;
            }
        }
        pasDbCloseCursor(psCur);
    }

    int iret1 = 0;
    int iNum = 0;
    int iNum1 = 0;

    p = head;
    while(p != NULL)
    {
        iNum++;
        if(iNum > 1)
        {
            utPltPutLoopVar(psDbHead, "dh", iNum, ",");
        }

        utPltPutLoopVar(psDbHead, "college", iNum, convert("GBK", "UTF-8", p->dpname));
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql) - 1, "select depName from tDepartment where depLevel=1 and tDepartment.pid='%s'", p->dpcode);
        psCur = pasDbOpenSql(sql, 0);
        if(psCur)
        {
            iNum1 = 0;
            while(0 == (iret1 = pasDbFetchInto(psCur,
                                               UT_TYPE_STRING, sizeof(caDepartment) - 1, caDepartment)) || 1405 == iret)
            {
                iNum1++;
                if(iNum1 > 1)
                {
                    utPltPutLoopVar2(psDbHead, "dh1", iNum, iNum1, ",");
                }
                utPltPutLoopVar2(psDbHead, "department", iNum, iNum1, convert("GBK", "UTF-8", caDepartment));
            }
            pasDbCloseCursor(psCur);
        }
        p = p->next;
    }
    p = head;
    while(p != NULL)//释放单链表
    {
        q = p->next;
        free(p);
        p = q;
    }
    p = NULL;
    q = NULL;
    head = NULL;
    newnode = NULL;

    utPltPutVarF(psDbHead, "result", "%d", 0);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/selectDpartment/show_college.htm");

    return 0;
}

/*int ict_get_department(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//获取班级
{
    utMsgPrintMsg(psMsgHead);
    char  sql[1024] = "";
    char caDpartment[64] = "";
    char caDpartmentCode[16] = "";
    char collegeCode[16]="";
    int iret = 0;
    int iNum = 0;
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();

     utMsgGetSomeNVar(psMsgHead, 1,
                     "college_code", UT_TYPE_STRING,  sizeof(collegeCode) - 1, collegeCode);

    snprintf(sql, sizeof(sql)-1, "select depName,depCode from tDepartment where depLevel=1 and tDepartment.pid='%s'", collegeCode);
    psCur = pasDbOpenSql(sql, 0);

    if(psCur)
    {
        while(0 == (iret = pasDbFetchInto(psCur,
                                                UT_TYPE_STRING, sizeof(caDpartment) - 1, caDpartment,
                                                UT_TYPE_STRING, sizeof(caDpartmentCode) -1 ,caDpartmentCode)) || 1405 == iret)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVarF(psDbHead, "department", iNum,convert("GBK", "UTF-8",caDpartment));
            utPltPutLoopVarF(psDbHead, "department_code", iNum,convert("GBK", "UTF-8",caDpartmentCode));
        }
        pasDbCloseCursor(psCur);
    }
    utPltPutVarF(psDbHead, "totalCount", "%d", iNum);
    utPltPutVarF(psDbHead, "result", "%d", 0);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/selectDpartment/show_department.htm");
}*/

int ict_balance_transfer(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//余额转让
{
    utMsgPrintMsg(psMsgHead);
    char caTsid[24] = "";
    char caVname[32] = "";
    char caMname[32] = "";
    char mname[32] = "";
    char vname[32] = "";
    char caMoney[16] = "";
    char sql[1024] = "";
    ulong lCount = 0;
    long Money = 0;
    int iReturn = 0;
    int iReturn1 = 0;

    utMsgGetSomeNVar(psMsgHead, 4,
                     "tsid", UT_TYPE_STRING,  sizeof(caTsid) - 1, caTsid,
                     "vname", UT_TYPE_STRING, sizeof(caVname) - 1, caVname,
                     "mname", UT_TYPE_STRING, sizeof(caMname) - 1, caMname,
                     "money", UT_TYPE_STRING, sizeof(caMoney) - 1, caMoney);

    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();
    if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/balanceTransfer/ict_balance_transfer.htm"))
    {
        return 0;
    }
    snprintf(vname, sizeof(vname) - 1, "%s", getVnameByTsId(psShmHead, atoll(caTsid)));
    //strcpy(vname, getVnameByTsId(psShmHead, atoll(caTsid)));
    if(strlen(caVname) == 0)
    {
        utPltPutVarF(psDbHead, "result", "%d", 3);//用户名空
        utPltPutVar(psDbHead, "mesg",  convert("GBK", "UTF-8", "用户名为空"));
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/balanceTransfer/ict_balance_transfer.htm");
    }
    else
    {
        snprintf(sql, sizeof(sql) - 1, "select count(*) from ncsrvuserex where username='%s'", caVname);
        pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount <= 0)
        {
            utPltPutVarF(psDbHead, "result", "%d", 4);//用户名不存在
            utPltPutVar(psDbHead, "mesg",  convert("GBK", "UTF-8", "用户名不存在"));
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/balanceTransfer/ict_balance_transfer.htm");
        }
        else
        {
            if(strcmp(vname, caVname) == 0)
            {
                utPltPutVarF(psDbHead, "result", "%d", 5);//双方账户相同
                utPltPutVar(psDbHead, "mesg",  convert("GBK", "UTF-8", "双方账户相同"));
                utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/balanceTransfer/ict_balance_transfer.htm");
            }
            else
            {
                memset(sql, 0, sizeof(sql));
                snprintf(sql, sizeof(sql), "select mname from ncsrvuserex where username='%s'", caVname);
                pasDbOneRecord(sql, 0, UT_TYPE_STRING, sizeof(mname) - 1, mname);
                if(strcmp(convert("UTF-8", "GBK", caMname), mname) != 0)
                {
                    utPltPutVarF(psDbHead, "result", "%d", 6);//账户名和真实姓名不一致
                    utPltPutVar(psDbHead, "mesg",  convert("GBK", "UTF-8", "账户名和真实姓名不一致"));
                    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/balanceTransfer/ict_balance_transfer.htm");
                }
                else
                {
                    if(!checkMoney(caMoney))
                    {
                        utPltPutVarF(psDbHead, "result", "%d", 7);//转账金额必须为大于0的数
                        utPltPutVar(psDbHead, "mesg",  convert("GBK", "UTF-8", "转账金额必须为大于0的数"));
                        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/balanceTransfer/ict_balance_transfer.htm");
                    }
                    else
                    {
                        memset(sql, 0 , sizeof(sql));
                        snprintf(sql, sizeof(sql) - 1, "select money from ncsrvuserex where username='%s'", vname);
                        pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &Money);
                        if(Money < atol(caMoney))
                        {
                            utPltPutVarF(psDbHead, "result", "%d", 8);//余额不足
                            utPltPutVar(psDbHead, "mesg",  convert("GBK", "UTF-8", "账户余额不足"));
                            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/balanceTransfer/ict_balance_transfer.htm");

                        }
                        else
                        {
                            memset(sql, 0 , sizeof(sql));
                            snprintf(sql, sizeof(sql) - 1, "update ncsrvuserex set money=money-'%ld' where username='%s'", atol(caMoney), vname);
                            iReturn = pasDbExecSqlF(sql);
                            pasLogs(PAS_SRCFILE, 2002, "111sql=%s iReturn=%d\n", sql, iReturn);

                            memset(sql, 0 , sizeof(sql));
                            snprintf(sql, sizeof(sql) - 1, "update ncsrvuserex set money=money+'%ld' where username='%s'", atol(caMoney), caVname);
                            iReturn1 = pasDbExecSqlF(sql);
                            pasLogs(PAS_SRCFILE, 2002, "222sql=%s iReturn1=%d\n", sql, iReturn1);

                            if(iReturn != 0 || iReturn != 0)
                            {
                                //操作失败 sql语句回滚
                                utPltPutVar(psDbHead, "tsid", caTsid);
                                utPltPutVarF(psDbHead, "result", "%d", 9);//余额转让成功
                                utPltPutVar(psDbHead, "mesg",  convert("GBK", "UTF-8", "数据库语句操作失败"));
                                utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/balanceTransfer/ict_balance_transfer.htm");
                                pasDbRollback(NULL);//错误回滚

                            }
                            else
                            {
                                utPltPutVar(psDbHead, "tsid", caTsid);
                                utPltPutVarF(psDbHead, "result", "%d", 0);//余额转让成功
                                utPltPutVar(psDbHead, "mesg",  convert("GBK", "UTF-8", "余额转让成功"));
                                utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/balanceTransfer/ict_balance_transfer.htm");
                                pasDbCommit(NULL);
                            }

                        }
                    }
                }
            }
        }
    }

    return 0;
}

int ict_getRecPackage(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//获取推荐套餐
{
    utMsgPrintMsg(psMsgHead);
    ulong packageId = 0;
    char caName[31] = "";
    char caNamedes[127] = "";
    char caPicture[127] = "";
	char caColorPicture[127] = "";
    ulong lPtype = 0;
    ulong lMoney = 0;
    uint8 lId = 0;
    int iret = -1;
    int iNum = 0;
    char caTsid[24] = {0};
    char start_in[8] = "";
    char limit_in[8] = "";
    uint8 pdays = 0;
    utMsgGetSomeNVar(psMsgHead, 3,
                     "start",    UT_TYPE_STRING,  sizeof(start_in) - 1,    start_in,
                     "limit",    UT_TYPE_STRING,      sizeof(limit_in) - 1,    limit_in,
                     "tsid",     UT_TYPE_STRING,      sizeof(caTsid) - 1,    caTsid);

    //printf("ict_getRecPackage*****%s\n",caTsid);
    char sql[1024] = "";
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();
    if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/package/rec_package.htm"))
    {
        return 0;
    }
    int db_count = 0;
    snprintf(sql, sizeof(sql) - 1, "select package.id,package.pdays,package.namech,package.namedes,package.money,package.picture,package.colorpicture from package,recpackage where package.id=recpackage.id and useflag=0");
    psCur = pasDbOpenSql(sql, 0);
    if(psCur != NULL)
    {
        while(0 == (iret = pasDbFetchInto(psCur,
                                          UT_TYPE_ULONG, 4, &packageId,
                                          UT_TYPE_LONG8, 8, &pdays,
                                          UT_TYPE_STRING, sizeof(caName) - 1, caName,
                                          UT_TYPE_STRING, sizeof(caNamedes) - 1, caNamedes,
                                          UT_TYPE_ULONG, 4, &lMoney,
                                          UT_TYPE_STRING, sizeof(caPicture) - 1, caPicture,
										  UT_TYPE_STRING, sizeof(caColorPicture) - 1, caColorPicture)) || 1405 == iret)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVarF(psDbHead, "packageid", iNum, "%lu", packageId);
            utPltPutLoopVarF(psDbHead, "pdays", iNum, "%llu", pdays);
            utPltPutLoopVar(psDbHead, "name", iNum, convert("GBK", "UTF-8", caName));
            utPltPutLoopVar(psDbHead, "namedes", iNum, convert("GBK", "UTF-8", caNamedes));
            utPltPutLoopVarF(psDbHead, "money", iNum, "%lu", lMoney);
            utPltPutLoopVar(psDbHead, "picture", iNum,  caPicture);
			utPltPutLoopVar(psDbHead, "colorpicture", iNum,  caColorPicture);
        }
        pasDbCloseCursor(psCur);
    }
	utPltPutVarF(psDbHead, "tsid", caTsid);
    utPltPutVarF(psDbHead, "result", "%d", 0);
    utPltPutVarF(psDbHead, "TotRec", "%lu", iNum);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/package/rec_package.htm");
    return 0;
}


int ict_check_upgradePackage(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//检查是否存在可升级套餐
{
    utMsgPrintMsg(psMsgHead);
    char caTsid[32] = "";
    char caVname[32] = "";
    ulong caMoney = 0;
    ulong caDays = 0;
    char sql[1024] = "";
    ulong lCount = 0;
    ulong Count = 0;

    utMsgGetSomeNVar(psMsgHead, 1,
                     "tsid",   UT_TYPE_STRING, sizeof(caTsid) - 1, caTsid);
    utPltDbHead *psDbHead = utPltInitDb();

    if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/upgradePackage/ict_check_upgradePackage.htm"))
    {
        return 0;
    }
    snprintf(caVname, sizeof(caVname) - 1, "%s", getVnameByTsId(psShmHead, atoll(caTsid)));

    uint8 nowTime = time(0);
    snprintf(sql, sizeof(sql) - 1, "select count(*) from userorder where username='%s' and starttime<=%llu and endtime>%llu", caVname, nowTime, nowTime);
    pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);

    if(lCount <= 0)
    {
        utPltPutVar(psDbHead, "tsid", caTsid);
        utPltPutVarF(psDbHead, "result", "%d", 3);//不存在可升级
        utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", "当前无生效套餐"));
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/upgradePackage/ict_check_upgradePackage.htm");
    }
    else
    {
        memset(sql, 0 , sizeof(sql));
        uint8 nowTime = time(0);
        snprintf(sql, sizeof(sql) - 1, "select package.money,package.pdays from package,userorder where userorder.packageid=package.id and userorder.username='%s' and userorder.starttime<=%llu and userorder.endtime>%llu", caVname, nowTime, nowTime);
        pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &caMoney,
                       UT_TYPE_LONG, 4, &caDays);


        memset(sql, 0 , sizeof(sql));
        snprintf(sql, sizeof(sql) - 1, "select count(*) from package,userorder where package.money>%lu and package.pdays=%lu", caMoney, caDays);
        pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &Count);
        if(Count <= 0)
        {
            utPltPutVar(psDbHead, "tsid", caTsid);
            utPltPutVarF(psDbHead, "result", "%d", 4);//套餐已是最高级别
            utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", "套餐已是最高级别"));
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/upgradePackage/ict_check_upgradePackage.htm");
        }
        else
        {
            utPltPutVar(psDbHead, "tsid", caTsid);
            utPltPutVarF(psDbHead, "result", "%d", 0);//存在可升级
            utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", "可以升级"));
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/upgradePackage/ict_check_upgradePackage.htm");
        }
    }

    return 0;
}

int ict_show_upgradePackage(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//显示可升级套餐
{
    utMsgPrintMsg(psMsgHead);

    char caVname[32] = "";
    char caTsid[32] = "";
    ulong caMoney = 0;
    ulong caDays = 0;
    ulong packageid = 0;
    char name[32] = "";
    ulong money = 0;
    char namedes[124] = "";
    char sql[1024] = "";

    ulong pdays = 0;
    char picture[128] = "";
    char colorpicture[128] = "";
    ulong begtime = 0;
    ulong endtime = 0;
    ulong timeval = 0;
    char cuser[24] = "";
    char content[128] = "";

    utMsgGetSomeNVar(psMsgHead, 1,
                     "tsid",   UT_TYPE_STRING, sizeof(caTsid) - 1, caTsid);

    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();

    if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/upgradePackage/ict_show_upgradePackage.htm"))
    {
        return 0;
    }
    snprintf(caVname, sizeof(caVname) - 1, "%s", getVnameByTsId(psShmHead, atoll(caTsid)));
    //strcpy(caVname, getVnameByTsId(psShmHead, atoll(caTsid)));
    snprintf(sql, sizeof(sql) - 1, "select package.money,package.pdays from package,userorder where userorder.packageid=package.id and userorder.username='%s' and stautus=1", caVname);

    pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &caMoney,
                   UT_TYPE_LONG, 4, &caDays);

    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql) - 1, "select namech,money,namedes,id,pdays,picture,colorpicture,begtime,endtime,timeval,cuser,content from package where package.money>%lu and package.pdays=%lu", caMoney, caDays);
    psCur = pasDbOpenSql(sql, 0);
    int iret = 0, iNum = 0;
    if(psCur)
    {
        while(0 == (iret = pasDbFetchInto(psCur,
                                          UT_TYPE_STRING, sizeof(name) - 1, name,
                                          UT_TYPE_LONG, 4, &money,
                                          UT_TYPE_STRING, sizeof(namedes) - 1, namedes,
                                          UT_TYPE_LONG, 4, &packageid,
                                          UT_TYPE_LONG, 4, &pdays,
                                          UT_TYPE_STRING, sizeof(picture) - 1, picture,
                                          UT_TYPE_STRING, sizeof(colorpicture) - 1, colorpicture,
                                          UT_TYPE_LONG, 4, &begtime,
                                          UT_TYPE_LONG, 4, &endtime,
                                          UT_TYPE_LONG, 4, &timeval,
                                          UT_TYPE_STRING, sizeof(cuser) - 1, &cuser,
                                          UT_TYPE_STRING, sizeof(content) - 1, content)) || 1405 == iret)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVarF(psDbHead, "packageid", iNum, "%lu", packageid);
            utPltPutLoopVarF(psDbHead, "name", iNum, name);
            utPltPutLoopVarF(psDbHead, "money", iNum, "%lu", money);
            utPltPutLoopVar(psDbHead, "namedes", iNum, convert("GBK", "UTF-8", namedes));
            utPltPutLoopVarF(psDbHead, "pdays", iNum, "%lu", pdays);
            utPltPutLoopVar(psDbHead, "picture", iNum, picture);
            utPltPutLoopVar(psDbHead, "colorpicture", iNum, colorpicture);
            utPltPutLoopVarF(psDbHead, "begtime", iNum, "%lu", begtime);
            utPltPutLoopVarF(psDbHead, "endtime", iNum, "%lu", endtime);
            utPltPutLoopVarF(psDbHead, "timeval", iNum, "%lu", timeval);
            utPltPutLoopVar(psDbHead, "cuser", iNum, cuser);
            utPltPutLoopVar(psDbHead, "content", iNum, content);

        }
        pasDbCloseCursor(psCur);
    }
    utPltPutVar(psDbHead, "tsid", caTsid);
    utPltPutVarF(psDbHead, "result", "%d", 0);
    utPltPutVarF(psDbHead, "totalCount", "%d", iNum);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/upgradePackage/ict_show_upgradePackage.htm");
}

int ict_package_balance(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//获取套餐余额
{
    utMsgPrintMsg(psMsgHead);
    char caVname[32] = "";
    char caTsid[32] = "";
    char sql[1024] = "";
    char packageId[8] = "";
    ulong caDays = 0;
    ulong caMoney = 0;
    ulong lmoney = 0;
    uint8 startTime = 0;

    utMsgGetSomeNVar(psMsgHead, 2,
                     "tsid",   UT_TYPE_STRING, sizeof(caTsid) - 1, caTsid,
                     "packageid",   UT_TYPE_STRING, sizeof(packageId) - 1, packageId);

    utPltDbHead *psDbHead = utPltInitDb();
    if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/upgradePackage/ict_package_balance.htm"))
    {
        return 0;
    }
    snprintf(caVname, sizeof(caVname) - 1, "%s", getVnameByTsId(psShmHead, atoll(caTsid)));
    //strcpy(caVname, getVnameByTsId(psShmHead, atoll(caTsid)));
    uint8 nowTime = time(0);
    snprintf(sql, sizeof(sql) - 1, "select package.pdays,package.money,userorder.starttime from package,userorder where username='%s' and userorder.packageid=package.id and userorder.starttime<=%llu and userorder.endtime>%llu", caVname, nowTime, nowTime);
    pasDbOneRecord(sql, 0,
                   UT_TYPE_LONG, 4, &caDays,
                   UT_TYPE_LONG, 4, &caMoney,
                   UT_TYPE_LONG8, 4, &startTime);
    lmoney = (caDays * 24 * 3600 + startTime - nowTime) * caMoney / (caDays * 24 * 3600);

    utPltPutVar(psDbHead, "tsid", caTsid);
    utPltPutVarF(psDbHead, "money", "%lu", lmoney);
    utPltPutVarF(psDbHead, "time", "%llu", nowTime);
    utPltPutVarF(psDbHead, "result", "%d", 0);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/upgradePackage/ict_package_balance.htm");
}

int ict_upgrade_package(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//升级套餐
{
    utMsgPrintMsg(psMsgHead);
    char caVname[32] = "";
    char caTsid[32] = "";
    char sql[1024] = "";
    char namedes[32] = "";
    uint8 startTime = 0;
    char mobno[16] = "";
    ulong caPackageId = 0;
    char packageId[8] = "";
    ulong caMoney = 0;
    char caTime[24] = "";
    ulong caDays = 0;
    long Money = 0;
    ulong Days = 0;
    long lmoney = 0;
    long money = 0;
	long lCount = 0;
    utMsgGetSomeNVar(psMsgHead, 3,
                     "tsid",   UT_TYPE_STRING, sizeof(caTsid) - 1, caTsid,
                     "packageid",   UT_TYPE_STRING, sizeof(packageId) - 1, packageId,
                     "time", UT_TYPE_STRING, sizeof(caTime) - 1, caTime
                    );

    utPltDbHead *psDbHead = utPltInitDb();
    if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/upgradePackage/ict_upgrade_package.htm"))
    {
        return 0;
    }
	snprintf(sql, sizeof(sql) - 1, "select count(*) where package.id=%lu", atol(packageId));
	pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);
	if(lCount ==0 )
	{
		utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", "传入套餐id错误"));
        utPltPutVarF(psDbHead, "result", "%d", 5);
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/upgradePackage/ict_upgrade_package.htm");
		return 0;
	}
	
	
    snprintf(caVname, sizeof(caVname) - 1, "%s", getVnameByTsId(psShmHead, atoll(caTsid)));


    //strcpy(caVname, getVnameByTsId(psShmHead, atoll(caTsid)));
    uint8 nowTime = time(0);

    if(nowTime>(atoll(caTime) +60))//付款超时不能超过60秒
    {
        utPltPutVar(psDbHead, "tsid", caTsid);
        //utPltPutVarF(psDbHead, "money", "%lu", lmoney);
        utPltPutVarF(psDbHead, "mesg", convert("GBK", "UTF-8", "操作超时"));
        utPltPutVarF(psDbHead, "result", "%d", 3);
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/upgradePackage/ict_upgrade_package.htm");
    }
    else
    {
		memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql) - 1, "select package.pdays,package.money,userorder.starttime,userorder.mobno from package,userorder where username='%s' and userorder.packageid=package.id and userorder.starttime<=%llu and userorder.endtime>%llu", caVname, atoll(caTime), atoll(caTime));
        pasDbOneRecord(sql, 0,
                       UT_TYPE_LONG, 4, &caDays,
                       UT_TYPE_LONG, 4, &caMoney,
                       UT_TYPE_LONG8, 8, &startTime,
                       UT_TYPE_STRING, sizeof(mobno) - 1, mobno);

        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql) - 1, "select namech,pdays,money from package where package.id=%lu", atol(packageId));
        pasDbOneRecord(sql, 0,
                       UT_TYPE_STRING, sizeof(namedes) - 1, namedes,
                       UT_TYPE_LONG, 4, &Days,
                       UT_TYPE_LONG, 4, &Money);
									   
        lmoney = (caDays * 24 * 3600 + startTime - atoll(caTime)) * caMoney / (caDays * 24 * 3600);
        money = Money - lmoney;
        long uMoney = 0;
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql) - 1, "select money from ncsrvuserex where username='%s'", caVname);

        pasDbOneRecord(sql, 0,  UT_TYPE_LONG, 4, &uMoney);
        if(money > uMoney)
        {

            utPltPutVar(psDbHead, "tsid", caTsid);
            //  utPltPutVarF(psDbHead, "money", "%lu", lmoney);
            char caMsg[128] = "";
            snprintf(caMsg, sizeof(caMsg) - 1, "余额不足，差%.2f元，无法升级套餐", (money - uMoney) / 100.0);
            utPltPutVarF(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
            utPltPutVarF(psDbHead, "result", "%d", 4);
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/upgradePackage/ict_upgrade_package.htm");
        }
        else
        {

            memset(sql, 0, sizeof(sql));
            snprintf(sql, sizeof(sql) - 1, "update ncsrvuserex set money=money-%lu where username='%s'", money, caVname);
            pasDbExecSqlF(sql);
            memset(sql, 0, sizeof(sql));
            snprintf(sql, sizeof(sql) - 1, "update userorder set packageid=%lu,name='%s',starttime=%llu,endtime=%llu,timeval=%llu where username='%s' and status=1", atol(packageId), namedes, atoll(caTime), atoll(caTime) + Days * 24 * 3600, atoll(caTime), caVname);
            pasDbExecSqlF(sql);

            char tableTime[12] = "";
            char caMark[20] = "";
            snprintf(caMark, sizeof(caMark) - 1, "%s", utComGetVar_sd(psShmHead, "mark", "0"));
            //strcpy(caMark, utComGetVar_sd(psShmHead, "mark", "0"));
            snprintf(tableTime, sizeof(tableTime) - 1, "%s", utTimFormat("%Y%m", nowTime));
            memset(sql, 0, sizeof(sql));
            snprintf(sql, sizeof(sql) - 1, "insert into userorderlog_%s(username,mobno,mark,packageid,name,status,money,starttime,endtime,timeval) values('%s','%s','%s',%lu,'%s','2',%lu,%lu,%lu,%lu)", tableTime, caVname, mobno, caMark, atol(packageId), namedes, money, atoll(caTime), atoll(caTime) + Days * 24 * 3600, atoll(caTime));
            pasDbExecSqlF(sql);
            utPltPutVar(psDbHead, "tsid", caTsid);
            utPltPutVarF(psDbHead, "money", "%lu", lmoney);
            utPltPutVarF(psDbHead, "mesg", convert("GBK", "UTF-8", "升级套餐成功"));
            utPltPutVarF(psDbHead, "result", "%d", 0);
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/upgradePackage/ict_upgrade_package.htm");
        }
    }
    return 0;
}

/*int checkUserOrder(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead, char *caVname)
{
    utMsgPrintMsg(psMsgHead);
    char sql[1024] = "";
    char vname[32] = "";
    ulong packageid = 0;
    uint8 endtime = 0;
    char autoxiding[4] = "";

    utPltDbHead *psDbHead = utPltInitDb();
    pasDbCursor *psCur = NULL;
    snprintf(sql, sizeof(sql)-1, "select username,packageid,endtime,autoxiding from userorder where status='1'");
    psCur = pasDbOpenSql(sql, 0);

    int iret = 0;
    if(psCur)
        {
            while(0 == (iret = pasDbFetchInto(psCur,
                                  UT_TYPE_STRING, sizeof(vname) - 1, vname,
                                  UT_TYPE_LONG, 4, &packageid,
                                  UT_TYPE_LONG8, 8, &endtime,
                                  UT_TYPE_STRING, sizeof(autoxiding) - 1 , autoxiding)) || 1405 == iret)
            {
                uint8 nowtime = time(0);
                if(endtime <= nowtime))
                {
                    if(autoxiding == '0')
                    {
                        snprintf(sql, sizeof(sql)-1, "update userorder set status='2' where username='%s' and endTime <= %lu and status='1'", vname, nowTime);
                    }
                    else
                    {
                        memset(sql, 0, sizeof(sql));
                        snprintf(sql, sizeof(sql)-1, "select ncsrvuserex.money,package.money.package.pdays from ncsrvuserex,package where ncsrvuserex.username='%s' and package.id=%lu", vname, packageid);
                        ulong lmoney = 0;
                        ulong pmoney = 0;
                        ulong pdays = 0;
                        pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, lmoney,
                                               UT_TYPE_ULONG, 4, pmoney,
                                               UT_TYPE_ULONG, 4, pdays);
                        if(lmoney < pmoney)
                        {
                            //余额不足，无法自动订购
                        }
                        else
                        {
                            memset(sql, 0, sizeof(sql));
                            snprintf(sql, sizeof(sql)-1, "update ncsrvuserex set money=money-%lu where username='%s'", pmoney,vname);
                            pasDbExecSqlF(sql);

                            memset(sql, 0, sizeof(sql));
                            snprintf(sql, sizeof(sql)-1, "update userorder set starttime=%llu,endtime=%llu,timeval=%llu",nowtime,nowtime+pdays*24*3600,nowtime);
                            pasDbExecSqlF(sql);
                        }
                    }
                }


            }
            pasDbCloseCursor(psCur);
        }


    int iret = 0, iNum = 0;
    if(endTime <= time(0))
    {

    }

}*/

int ict_auto_renewal(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//自动订购
{
    utMsgPrintMsg(psMsgHead);
    char caTsid[24] = "";
    char packageId[8] = "";
    char caVname[32] = "";
    char sql[1024] = "";
    char flag[4] = "";
    utPltDbHead *psDbHead = utPltInitDb();
    utMsgGetSomeNVar(psMsgHead, 3,
                     "tsid",   UT_TYPE_STRING, sizeof(caTsid) - 1, caTsid,
                     "packageid",   UT_TYPE_STRING, sizeof(packageId) - 1, packageId,
                     "flag", UT_TYPE_STRING, sizeof(flag) - 1, flag);
    if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/autoRenewal/ict_auto_renewal.htm"))
    {
        return 0;
    }
    snprintf(caVname, sizeof(caVname) - 1, "%s", getVnameByTsId(psShmHead, atoll(caTsid)));
    //strcpy(caVname, getVnameByTsId(psShmHead, atoll(caTsid)));
    uint8 nowTime = time(0);

    if(atoi(flag) == 0)
    {
        snprintf(sql, sizeof(sql) - 1, "update userorder set autoxiding='1' where username='%s' and starttime<=%llu and endtime>%llu", caVname, nowTime, nowTime);
        pasDbExecSqlF(sql);
        utPltPutVar(psDbHead, "tsid", caTsid);
        utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", "自动续订成功"));
        utPltPutVarF(psDbHead, "result", "%d", 0);
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/autoRenewal/ict_auto_renewal.htm");
    }
    else
    {
        if(atoi(flag) == 1)
        {
            snprintf(sql, sizeof(sql) - 1, "update userorder set autoxiding='0' where username='%s' and starttime<=%llu and endtime>%llu", caVname, nowTime, nowTime);
            pasDbExecSqlF(sql);

            utPltPutVar(psDbHead, "tsid", caTsid);
            utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", "取消自动续订成功"));
            utPltPutVarF(psDbHead, "result", "%d", 0);
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/autoRenewal/ict_auto_renewal.htm");
        }
        else
        {
            utPltPutVar(psDbHead, "tsid", caTsid);
            utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", "参数错误"));
            utPltPutVarF(psDbHead, "result", "%d", 3);
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/autoRenewal/ict_auto_renewal.htm");
        }
    }
    return 0;
}

int ict_disconnect_net(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//断线
{
    utMsgPrintMsg(psMsgHead);
    char caTsid[24] = "";
    utPltDbHead *psDbHead = utPltInitDb();
    utMsgGetSomeNVar(psMsgHead, 1,
                     "tsid",   UT_TYPE_STRING, sizeof(caTsid) - 1, caTsid);
    if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/disconnectNet/ict_disconnect_net.htm"))
    {
        return 0;
    }

    utPltPutVar(psDbHead, "tsid", caTsid);
    utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", "断线成功"));
    utPltPutVarF(psDbHead, "result", "%d", 0);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/disconnectNet/ict_disconnect_net.htm");

    return 0;
}


int ict_safe_exit(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//安全退出
{
    utMsgPrintMsg(psMsgHead);
    char caTsid[24] = "";
    utPltDbHead *psDbHead = utPltInitDb();
    utMsgGetSomeNVar(psMsgHead, 1,
                     "tsid",   UT_TYPE_STRING, sizeof(caTsid) - 1, caTsid);
    if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/safeExit/ict_safe_exit.htm"))
    {
        return 0;
    }
    utPltPutVar(psDbHead, "tsid", caTsid);
    utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", "安全退出成功"));
    utPltPutVarF(psDbHead, "result", "%d", 0);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/safeExit/ict_safe_exit.htm");

    return 0;
}

int ict_ncSrvLogOut(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{

    char caUserIp[32];
    char caUserMac[32];
    char caClientIp[32], *pPlate;
    char caPlate[128];
    char caMsg[512];
    char caSrvPort[16], caSrvIp[32];
    char mac[6];
    int  iReturn, iRet, iCode = 0;
    utPltDbHead *psDbHead;

    ncPortalOnline *psOnline = NULL;

    uint4 lSip, lSn;
    uint2 nReqId = 0;
    char caLang[16], caTerm[16], caOs[16], caBro[16], caType[16], caUserAgent[256], caTsid[24];
    char caGwid[32];
    utMsgOutMsgToLog(PAS_SRCFILE, 1008, psMsgHead, "[ncSrvLogOut] \n");
    strcpy(caUserIp, "\0");
    strcpy(caClientIp, "\0");
    strcpy(caUserMac, "\0");
    strcpy(caTsid, "\0");
    strcpy(caGwid, "\0");
    strcpy(caUserAgent, "\0");
    strcpy(caSrvIp, "\0");
    strcpy(caSrvPort, "\0");
    iReturn = utMsgGetSomeNVar(psMsgHead, 8,
                               "tsid",          UT_TYPE_STRING, 20, caTsid,
                               "userip",        UT_TYPE_STRING, 24, caUserIp,
                               "usermac",       UT_TYPE_STRING, 31, caUserMac,
                               "gw_id",         UT_TYPE_STRING, 31, caGwid,
                               "srvname",     UT_TYPE_STRING, 31, caSrvIp,
                               "srvport",     UT_TYPE_STRING, 7,  caSrvPort,
                               "usragent",      UT_TYPE_STRING, 255, caUserAgent,
                               "clientip",      UT_TYPE_STRING, 31, caClientIp);

    if(!utStrIsSpaces(caTsid))
    {
        psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByStrTsid(psShmHead, caTsid);
    }
    if(psOnline == NULL)
    {
        if(!utStrIsSpaces(caUserMac))
        {
            pasCvtMacI(caUserMac, mac);
            strcpy(caUserMac, pasCvtMac12(mac));
            psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByMac(psShmHead, mac);
        }
    }
    if(psOnline == NULL)
    {
        if(utStrIsSpaces(caUserIp))
        {
            strcpy(caUserIp, caClientIp);
        }
        lSip = ntohl(pasIpcvtLong(caUserIp));
        psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIp(psShmHead, caGwid, lSip);
    }

    psDbHead = utPltInitDbHead();
    sprintf(caMsg, "tsid@%s", caTsid);
    utPltPutVar(psDbHead, "getarg", caMsg);
    caTerm[0] = 0;
    caOs[0] = 0;
    caBro[0] = 0;
    caType[0] = 0;
    if(psOnline)
    {
        ncSrvLetUserLogout(psShmHead, psOnline->lTsid);
        ncSrvUserLogout(psShmHead, psOnline, NCPORTAL_CAUSE_USERREQUEST);
        utPltPutVar(psDbHead, "lang", psOnline->caLang);
        utPltPutVar(psDbHead, "username", psOnline->caName);
        strcpy(caMsg, "\0");
        utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang", psOnline->caLang);
        utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip", utComHostIp(htonl(psOnline->lSip)));
        utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "usermac", pasCvtMac(psOnline->caMac));
        utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%llu\">", "tsid", psOnline->lTsid);
        utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "username", psOnline->caName);
        utPltPutVar(psDbHead, "postarg", caMsg);
        utPltPutVarF(psDbHead, "tsid", "%llu", psOnline->lTsid);
        utPltPutVar(psDbHead, "userip", utComHostIp(htonl(psOnline->lSip)));
        utPltPutVar(psDbHead, "usermac", pasCvtMac(psOnline->caMac));
        pPlate = ncSrvGetPlateInfo(psShmHead, (char *)ncSrvGetPlateName(psShmHead, psOnline), psOnline->caTermType, psOnline->caLang, &iCode);
    }
    else
    {
        iRet = pasUtlTermAttr(caUserAgent, caTerm, caOs, caBro, caType);
        pPlate = ncSrvGetPlateInfo(psShmHead, ncSrvGetPlateName(psShmHead, psOnline), caType, caLang, &iCode);
    }
    utPltPutVar(psDbHead, "ip",      caSrvIp);
    utPltPutVar(psDbHead, "port",    caSrvPort);


    if(pPlate)
    {
        sprintf(caPlate, "%s/logout_ok.htm", pPlate);
    }
    else
    {
        sprintf(caPlate, "default/logout_ok.htm", pPlate);
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);
    return 0;
}


//获取短信
int ictSrvGetPass(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caSsid[32];
    char caName[32];
    char caName0[32];
    char caPass[32];
    char caUserIp[32];
    char caUserMac[32];
    char caAcName[32];
    char caChapPass[32];
    char caChallenge[32];
    char caSmsName0[32];
    char caGroupCode[32];
    char caSpname[16];
    char caApMac[32];
    char caUrl[256];
    char caMsg[4096];
    char caLang[16], caCode[8];
    char caTsid[24];
    char caArea[32];
    FILE *sp;
    int iReturn, iRet;
    long long lSid = 0;
    utPltDbHead *psDbHead;
    utShmHand sShm;
    int iSum, iAreaCode = 0, nCurNum, iReSend = 0, iVendor = 0;
    uint2 nReqId = 0, nDport;
    uint4  nPort = 2000, lDip, lCode, lTime, lStatus = 0, iSp, iArea, lGroupid = 0, lStype = 0;
    uint8 llTsid;
    unsigned long lSip;
    char   caNo[32];
    char caHtml[256];
    char caNasid[24];
    char caStype[8];
    ncPortalSummary *psSumm;
    ncPortalOnline  *psOnline = NULL;
    char caSmsName[32];
    utMsgHead *psMsgHead0;
    utMsgOutMsgToLog(PAS_SRCFILE, 1008, psMsgHead, "[ncSrvGetPass] \n");
    strcpy(caSsid, "\0");
    strcpy(caPass, "\0");
    strcpy(caUserIp, "\0");
    memset(caName, 0, sizeof(caName));
    //strcpy(caName,"\0");
    strcpy(caUserMac, "\0");
    strcpy(caSmsName, "\0");
    strcpy(caNasid, "\0");
    memset(caTsid, 0, 24);
    memset(caNasid, 0, 32);
    strcpy(caApMac, "\0");
    strcpy(caSn, utComGetVar_sd(psShmHead, "ProductSN", "\0"));
    lTime = time(0);

    psSumm = (ncPortalSummary *)utShmArray(psShmHead, NCSRV_LNK_SYSINFO);
    strcpy(caTsid, "\0");
    iReturn = utMsgGetSomeNVar(psMsgHead, 5,
                               "tsid",     UT_TYPE_STRING, 20, caTsid,
                               "lang",     UT_TYPE_STRING, 15, caLang,
                               "charset",  UT_TYPE_STRING, 15, caCode,
                               "userip",   UT_TYPE_STRING, 24, caUserIp,
                               "username", UT_TYPE_STRING, 31, caName0);

    if(!utStrIsSpaces(caTsid))
    {
        sscanf(caTsid, "%llu", &llTsid);
        psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
        if(psOnline)
        {
            lGroupid = psOnline->lGroupid;
        }
        else
        {
            lGroupid = 0;
        }
    }
    if(utStrIsSpaces(caName0))
    {
        pasLogs(PAS_SRCFILE, 1008, " Username Is Null: ");
        psDbHead = utPltInitDbHead();
        utPltPutVar(psDbHead, "statusId",   "3");
        utPltPutVar(psDbHead, "statusCode", "3");
        utPltPutVar(psDbHead, "message", " Username is Null");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "login_getpass_return.htm");
        return 0;
    }
    if(psOnline == NULL)
    {
        char *pHref, *p;
        pHref = utMsgGetVar_s(psMsgHead, "httpref");
        if(pHref)
        {
            char caOut[512];
            pasStrUrlUnEsc(pHref, caOut);
            p = strstr(caOut, "switch_url=");
            if(p)    // Cisco AP
            {
                // https://r389.proeim.cn/pronline/Msg?FunName@ncSrvDispLogin?switch_url=https://applechinawifi.apple.com/login.html&ap_mac=fc:fb:fb:d8:26:70&client_mac=3c:ab:8e:5a:4f:cc&wlan=Apple%20Store&redirect=www.airport.us/pcK3qszyU/pwaX9tvIi.html
                ncUtlGetWordBetween(caOut, "ap_mac=", "&", caNasid, 31);
                ncUtlGetWordBetween(caOut, "wlan=", "&", caSsid, 31);
                ncUtlGetWordBetween(caOut, "client_mac=", "&", caUserMac, 31);
                iVendor = NCPORTAL_VENDOR_CISCO;
            }
            if(iVendor == 0 && (p = strstr(caOut, "wlanacname=")))
            {
                // [https://192.168.21.102/pronline/Msg?FunName@ncSrvDispLogin?wlanuserip=10.1.1.6&wlanacname=0000.0000.000.00.460&ssid=apple-https&nasid=0000.0000.000.00.460&usermac=E4-CE-8F-E1-AB-37]
                iVendor = NCPORTAL_VENDOR_GBCOM;
                ncUtlGetWordBetween(caOut, "wlanacname=", "&", caNasid, 31);
                ncUtlGetWordBetween(caOut, "ssid=", "&", caSsid, 31);
                ncUtlGetWordBetween(caOut, "usermac=", "&", caUserMac, 31);
                ncUtlGetWordBetween(caOut, "wlanuserip=", "&", caUserIp, 15);
            }
            if(iVendor == 0 && (p = strstr(caOut, "from=gbap")))
            {
                // [http://171.8.150.218:9080/pronline/Msg?FunName@ncSrvPlate&lang@&code@&usernametmp@&html@/alogin_main.htm&from=gbap&ssid=&gw_address=192.168.20.1&gw_port=8060&gw_id=ZZDXceshi&ip=192.168.20.110&mac=3c:43:8e:d4:53:92&url=http://m.baidu.com/s]
                iVendor = NCPORTAL_VENDOR_GBAP;
                ncUtlGetWordBetween(caOut, "gw_id=", "&", caNasid, 31);
                strcpy(caSsid, "\0");
                ncUtlGetWordBetween(caOut, "&mac=", "&", caUserMac, 31);
                ncUtlGetWordBetween(caOut, "&ip=", "&", caUserIp, 15);
            }
            if(iVendor == 0 && (p = strstr(caOut, "From=WIFIDOG")))
            {
                // [http://171.8.150.218:9080/pronline/Msg?FunName@ncSrvPlate&lang@&code@&usernametmp@&html@/alogin_main.htm&from=gbap&ssid=&gw_address=192.168.20.1&gw_port=8060&gw_id=ZZDXceshi&ip=192.168.20.110&mac=3c:43:8e:d4:53:92&url=http://m.baidu.com/s]
                iVendor = NCPORTAL_VENDOR_WIFIDOG;
                ncUtlGetWordBetween(caOut, "gw_id=", "&", caNasid, 31);
                strcpy(caSsid, "\0");
                ncUtlGetWordBetween(caOut, "&mac=", "&", caUserMac, 31);
                ncUtlGetWordBetween(caOut, "&ip=", "&", caUserIp, 15);
            }
            if(iVendor == 0 && (p = strstr(caOut, "From=BCTH")))
            {
                // [http://171.8.150.218:9080/pronline/Msg?FunName@ncSrvPlate&lang@&code@&usernametmp@&html@/alogin_main.htm&from=gbap&ssid=&gw_address=192.168.20.1&gw_port=8060&gw_id=ZZDXceshi&ip=192.168.20.110&mac=3c:43:8e:d4:53:92&url=http://m.baidu.com/s]
                iVendor = NCPORTAL_VENDOR_BCTH;
                ncUtlGetWordBetween(caOut, "gw_id=", "&", caNasid, 31);
                strcpy(caSsid, "\0");
                ncUtlGetWordBetween(caOut, "&mac=", "&", caUserMac, 31);
                ncUtlGetWordBetween(caOut, "&ip=", "&", caUserIp, 15);
            }

            if(iVendor == 0 && (p = strstr(caOut, "&tsid=")))
            {
                // [http://171.8.150.218:9080/pronline/Msg?FunName@ncSrvPlate&lang@&code@&usernametmp@&html@/alogin_main.htm&from=gbap&ssid=&gw_address=192.168.20.1&gw_port=8060&gw_id=ZZDXceshi&ip=192.168.20.110&mac=3c:43:8e:d4:53:92&url=http://m.baidu.com/s]
                ncUtlGetWordBetween(caOut, "&tsid=", "&", caTsid, 31);
                sscanf(caTsid, "%llu", &llTsid);
                psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
                if(psOnline)
                {
                    lGroupid = psOnline->lGroupid;
                }
            }
            if(iVendor > 0 && psOnline == NULL)
            {
                lSip = ntohl(pasIpcvtLong(caUserIp));
                psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIp(psShmHead, caNasid, lSip);
            }
        }
    }



    if(psOnline && psOnline->psAp && psOnline->psAp->psGroup)    // 短信计费单位
    {
        strcpy(caSmsName, psOnline->psAp->psGroup->caSmsName);
    }
    else
    {
        strcpy(caSmsName, utComGetVar_sd(psShmHead, "MySmsName", "None"));
    }


    /*  手机号码的有效性检查
        +86-1234567
      */
    iAreaCode =  ncPortalCheckMobileNumber(caName0, caName, caArea, caSpname, &iArea, &iSp);
    if(iAreaCode <= 0)
    {
        pasLogs(PAS_SRCFILE, 1008, " Username: %s Number Error AreaCode=%d", caName0, iAreaCode);
        psDbHead = utPltInitDbHead();
        utPltPutVar(psDbHead, "statusId",   "1");
        utPltPutVar(psDbHead, "statusCode", "5");
        utPltPutVar(psDbHead, "message", "Invalid Username");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "login_getpass_return.htm");
        return 0;
    }
    pasLogs(PAS_SRCFILE, 1008, " Check MobileNumber: AreaCode=%u  Name:%s  Area:%s ", iAreaCode, caName, caArea);
    // 检查黑名单
    if(psOnline)
    {
        strcpy(caApMac, psOnline->caApName);
        lStype = psOnline->lStype;
        if(ncSrvPortalIsBlackUser(psShmHead, psOnline->lStype, NCPORTAL_BLACK_USER, caName))
        {
            pasLogs(PAS_SRCFILE, 1008, " Username: %s is in Black", caName);
            psDbHead = utPltInitDbHead();
            utPltPutVar(psDbHead, "statusId",   "3");
            utPltPutVar(psDbHead, "statusCode", "3");
            utPltPutVar(psDbHead, "message", " Username is in black");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "login_getpass_return.htm");
            return 0;
        }
        if(ncSrvPortalIsBlackUser(psShmHead, psOnline->lStype, NCPORTAL_BLACK_MAC, psOnline->caMac))
        {
            pasLogs(PAS_SRCFILE, 1008, " Username: %s Mac %s is in Black", caName, pasCvtMac(psOnline->caMac));
            psDbHead = utPltInitDbHead();
            utPltPutVar(psDbHead, "statusId",   "3");
            utPltPutVar(psDbHead, "statusCode", "3");
            utPltPutVar(psDbHead, "message", " Username is in black");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "login_getpass_return.htm");
            return 0;
        }
        if(utStrIsSpaces(caUserMac))
        {
            strcpy(caUserMac, pasCvtMac(psOnline->caMac));
        }
    }
    else
    {
        if(ncSrvPortalIsBlackUser(psShmHead, 0, NCPORTAL_BLACK_USER, caName))
        {
            pasLogs(PAS_SRCFILE, 1008, " Username: %s is in Black", caName);
            psDbHead = utPltInitDbHead();
            utPltPutVar(psDbHead, "statusId",   "3");
            utPltPutVar(psDbHead, "statusCode", "3");
            utPltPutVar(psDbHead, "message", " Username is in black");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "login_getpass_return.htm");
            return 0;
        }
    }

    srand(time(0));
    lCode = rand() % 999999L;
    sprintf(caPass, "%06lu", lCode);
    caPass[6] = 0;

    if(iAreaCode == 86)   // 国内短信
    {
        nCurNum = psSumm->nCurSms;
        iReSend = 0;  // 失败重发次数
        while(iReSend < 3)
        {
            lDip = htonl(psSumm->psSmsServer[nCurNum].lIp);
            nDport = htons(psSumm->psSmsServer[nCurNum].nPort);
            if(psSumm->psSmsServer[nCurNum].nType == NCSRV_SMSTYPE_PRONETWAY)
            {
                ncSrvGetSmsMsg(psShmHead, psOnline, caLang, caMsg);
                utStrReplaceWith(caMsg, "[#username#]", caName);
                utStrReplaceWith(caMsg, "[#password#]", caPass);
                sprintf(caSmsName0, "%s", psSumm->psSmsServer[nCurNum].caSrvName);
                psMsgHead0 = pasTcpRequest(lDip, nDport,
                                           0,
                                           0,
                                           "ncsSendMobileMesg_cy",
                                           0,
                                           0,
                                           30,
                                           NULL,
                                           5,
                                           "servicecode", UT_TYPE_STRING, psSumm->caPortalName,
                                           "SN",          UT_TYPE_STRING, psSumm->caProdSn,
                                           "clienttype",  UT_TYPE_STRING, caSmsName,
                                           "mobno", UT_TYPE_STRING, caName,
                                           "mesg", UT_TYPE_STRING, caMsg);
                pasLogs(PAS_SRCFILE, 1008, "Send ncsSendMobileMesg_cy %s:%u  ServiceCode:%s  SN:%s  ClientType:%s Name:%s Message:[%s] iRet=%d",
                        utComHostIp(lDip), ntohs(nDport), psSumm->caPortalName, psSumm->caProdSn, caSmsName, caName, caMsg, iReturn);
                if(psMsgHead0)
                {
                    utMsgOutMsgToLog(PAS_SRCFILE, 1008, psMsgHead0, "[ncsSendMobileMesg_cy] \n");
                    iReturn = utMsgGetVar_ld(psMsgHead0, "status", 0);
                    // 短信服务器返回值   -999 网络不通   0 成功  -1 不成功  -888 禁用
                    utMsgFree(psMsgHead0);
                    if(iReturn == 0)
                    {
                        psSumm->psSmsServer[nCurNum].cStatus = 1;
                        psSumm->psSmsServer[nCurNum].lLastTime = time(0);
                        psSumm->nCurSms = nCurNum;
                        lStatus = 1;
                        break;
                    }
                    else if(iReturn == (-999))
                    {
                        iReSend ++;
                        if(nCurNum < psSumm->nSumSms - 1)
                        {
                            nCurNum ++;
                        }
                        else
                        {
                            nCurNum = 0;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    iReSend ++;
                    if(nCurNum < psSumm->nSumSms - 1)
                    {
                        nCurNum ++;
                    }
                    else
                    {
                        nCurNum = 0;
                    }
                    lStatus = 0;
                }
            }
            else if(psSumm->psSmsServer[nCurNum].nType == NCSRV_SMSTYPE_PEOPLE)
            {
                char *p;
                char caPre[128], caSuff[128];
                ncSrvGetSmsMsg(psShmHead, psOnline, caLang, caMsg);
                utStrReplaceWith(caMsg, "[#username#]", caName);
                p = strstr(caMsg, "[#password#]");
                if(p)
                {
                    *p = 0;
                    strcpy(caPre, caMsg);
                    strcpy(caSuff, p + 12);
                }
                else
                {
                    strcpy(caPre, caMsg);
                    strcpy(caSuff, "\0");
                }
                pasLogs(PAS_SRCFILE, 1008, "ncSrvSendSmsByPeople Name:%s Pre:%s Suff:%s", caName, caPre, caSuff);
                iRet = ncSrvSendSmsByPeople(psShmHead, caName, caPre, caSuff);
                break;
            }
            else if(psSumm->psSmsServer[nCurNum].nType == NCSRV_SMSTYPE_CENTER)    // 集中验证，代发短信
            {
                char caFcode[32], caProd[32];

                ncSrvGetSmsMsg(psShmHead, psOnline, caLang, caMsg);
                utStrReplaceWith(caMsg, "[#username#]", caName);
                utStrReplaceWith(caMsg, "[#password#]", caPass);
                strcpy(caProd, utComGetVar_sd(psShmHead, "ProductSN", "\0"));
                if(psOnline && psOnline->psAp && psOnline->psAp->psGroup)
                {
                    strcpy(caServicecode, psOnline->psAp->psGroup->caGroupName);
                    strcpy(caSmsName, psOnline->psAp->psGroup->caSmsName);
                    if(utStrIsSpaces(caServicecode))
                    {
                        strcpy(caServicecode, utComGetVar_sd(psShmHead, "ServiceCode", "00000000000000"));
                    }
                }
                else
                {
                    strcpy(caServicecode, utComGetVar_sd(psShmHead, "ServiceCode", "00000000000000"));
                    strcpy(caSmsName, "pronetway");
                }
                if(utStrIsSpaces(caSmsName))
                {
                    strcpy(caSmsName, "pronetway");
                }
                strcpy(caCode, "\0");
                pasLogs(PAS_SRCFILE, 1008, "ncSrvSendSmsByCenter Msg:%s ServiceCode:%s SmsName:%s", caMsg, caServicecode, caSmsName);
                strcpy(caFcode, utComGetVar_sd(psShmHead, "CenterAuthFcode", "02"));
                iRet = ncSrvSendSmsByCenter(&psSumm->psSmsServer[nCurNum], caFcode, caSmsName, pasCvtMac(psOnline->caMac),
                                            caServicecode, caCode, caName, caPass, caMsg, caProd);
                break;
            }
            else
            {
                if(utStrIsMobileNumber(caName))
                {
                    ncSrvGetSmsMsg(psShmHead, psOnline, caLang, caMsg);
                    utStrReplaceWith(caMsg, "[#username#]", caName);
                    utStrReplaceWith(caMsg, "[#password#]", caPass);
                    sprintf(caSmsName0, "%s", psSumm->psSmsServer[nCurNum].caSrvName);
                    iRet = ncUtlSendSmsByModem(caName, caMsg);
                    pasLogs(PAS_SRCFILE, 1008, "Send ncUtlSendSmsByModem Name:%s  Message:[%s] iRet=%d",
                            caName, caMsg, iRet);
                    lStatus = iRet;
                    if(iRet < 0)
                    {
                        iReSend ++;
                    }
                    else
                    {
                        psSumm->psSmsServer[nCurNum].cStatus = 1;
                        psSumm->psSmsServer[nCurNum].lLastTime = time(0);
                        psSumm->nCurSms = nCurNum;
                        break;
                    }
                }
            }
        }
    }
    else    // 国际短信
    {
        nCurNum = psSumm->nCurGjSms;
        iReSend = 0;  // 失败重发次数
        while(iReSend < 3)
        {
            lDip = htonl(psSumm->psSmsGjServer[nCurNum].lIp);
            nDport = htons(psSumm->psSmsGjServer[nCurNum].nPort);
            if(psSumm->psSmsGjServer[nCurNum].nType == NCSRV_SMSTYPE_PRONETWAY)
            {
                ncSrvGetSmsMsg(psShmHead, psOnline, caLang, caMsg);
                utStrReplaceWith(caMsg, "[#username#]", caName);
                utStrReplaceWith(caMsg, "[#password#]", caPass);
                sprintf(caSmsName0, "%s", psSumm->psSmsGjServer[nCurNum].caSrvName);
                psMsgHead0 = pasTcpRequest(lDip, nDport,
                                           0,
                                           0,
                                           "ncsSendMMesg_sms",
                                           0,
                                           0,
                                           30,
                                           NULL,
                                           5,
                                           "servicecode", UT_TYPE_STRING, psSumm->caPortalName,
                                           "SN",          UT_TYPE_STRING, psSumm->caProdSn,
                                           "clienttype",  UT_TYPE_STRING, caSmsName,
                                           "mobno", UT_TYPE_STRING, caName,
                                           "mesg", UT_TYPE_STRING, caMsg);
                pasLogs(PAS_SRCFILE, 1008, "Send ncsSendMMesg_sms %s:%u  ServiceCode:%s  SN:%s  ClientType:%s Name:%s Message:[%s] iRet=%d",
                        utComHostIp(lDip), ntohs(nDport), psSumm->caPortalName, psSumm->caProdSn, caSmsName, caName, caMsg, iReturn);
                if(psMsgHead0)
                {
                    utMsgOutMsgToLog(PAS_SRCFILE, 1008, psMsgHead0, "[ncsSendMobileMesg_cy] \n");
                    iReturn = utMsgGetVar_ld(psMsgHead0, "status", 0);
                    // 短信服务器返回值   -999 网络不通   0 成功  -1 不成功  -888 禁用
                    utMsgFree(psMsgHead0);
                    if(iReturn == 0)
                    {
                        psSumm->psSmsGjServer[nCurNum].cStatus = 1;
                        psSumm->psSmsGjServer[nCurNum].lLastTime = time(0);
                        psSumm->nCurGjSms = nCurNum;
                        lStatus = 1;
                        break;
                    }
                    else if(iReturn == (-999))
                    {
                        iReSend ++;
                        if(nCurNum < psSumm->nSumGjSms - 1)
                        {
                            nCurNum ++;
                        }
                        else
                        {
                            nCurNum = 0;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    iReSend ++;
                    if(nCurNum < psSumm->nSumGjSms - 1)
                    {
                        nCurNum ++;
                    }
                    else
                    {
                        nCurNum = 0;
                    }
                    lStatus = 0;
                }
            }
            else if(psSumm->psSmsGjServer[nCurNum].nType == NCSRV_SMSTYPE_PEOPLE)
            {
                char *p;
                char caPre[128], caSuff[128];
                ncSrvGetSmsMsg(psShmHead, psOnline, caLang, caMsg);
                utStrReplaceWith(caMsg, "[#username#]", caName);
                p = strstr(caMsg, "[#password#]");
                if(p)
                {
                    *p = 0;
                    strcpy(caPre, caMsg);
                    strcpy(caSuff, p + 12);
                }
                else
                {
                    strcpy(caPre, caMsg);
                    strcpy(caSuff, "\0");
                }
                iRet = ncSrvSendSmsByPeople(psShmHead, caName, caPre, caSuff);
                break;
            }
            else if(psSumm->psSmsServer[nCurNum].nType == NCSRV_SMSTYPE_CENTER)    // 集中验证，代发短信
            {
                char caFcode[32], caServiceCode[32], caProd[32];
                ncSrvGetSmsMsg(psShmHead, psOnline, caLang, caMsg);
                utStrReplaceWith(caMsg, "[#username#]", caName);
                utStrReplaceWith(caMsg, "[#password#]", caPass);
                strcpy(caProd, utComGetVar_sd(psShmHead, "ProductSN", "\0"));
                if(psOnline && psOnline->psAp && psOnline->psAp->psGroup)
                {
                    strcpy(caServiceCode, psOnline->psAp->psGroup->caGroupName);
                    strcpy(caSmsName, psOnline->psAp->psGroup->caSmsName);
                    if(utStrIsSpaces(caServicecode))
                    {
                        strcpy(caServicecode, utComGetVar_sd(psShmHead, "ServiceCode", "00000000000000"));
                    }
                }
                else
                {
                    strcpy(caServicecode, utComGetVar_sd(psShmHead, "ServiceCode", "00000000000000"));
                    strcpy(caSmsName, "pronetway");
                }
                pasLogs(PAS_SRCFILE, 1008, "ncSrvSendSmsByCenter Msg:%s ServiceCode:%s SmsName:%s", caMsg, caServicecode, caSmsName);
                sprintf(caCode, "%u", iAreaCode);
                strcpy(caFcode, utComGetVar_sd(psShmHead, "CenterAuthFcode", "02"));
                iRet = ncSrvSendSmsByCenter(&psSumm->psSmsServer[nCurNum], caFcode, caSmsName, pasCvtMac(psOnline->caMac),
                                            caServiceCode, caCode, caName, caPass, caMsg, caProd);
                break;
            }
            else
            {
                if(utStrIsMobileNumber(caName))
                {
                    ncSrvGetSmsMsg(psShmHead, psOnline, caLang, caMsg);
                    utStrReplaceWith(caMsg, "[#username#]", caName);
                    utStrReplaceWith(caMsg, "[#password#]", caPass);
                    sprintf(caSmsName, "%s.%s", caSmsName, psSumm->psSmsGjServer[nCurNum].caSrvName);
                    iRet = ncUtlSendSmsByModem(caName, caMsg);
                    pasLogs(PAS_SRCFILE, 1008, "Send ncUtlSendSmsByModem Name:%s  Message:[%s] iRet=%d",
                            caName, caMsg, iRet);
                    lStatus = iRet;
                    psSumm->psSmsGjServer[nCurNum].cStatus = 1;
                    psSumm->psSmsGjServer[nCurNum].lLastTime = time(0);
                    psSumm->nCurSms = nCurNum;
                }
            }
        }
    }
    psDbHead = utPltInitDbHead();
    utPltPutVar(psDbHead, "tsid",   caTsid);
    utPltPutVar(psDbHead, "userip", caUserIp);
    utPltPutVar(psDbHead, "username", caName);
    strcpy(caGroupCode, "\0");
    lGroupid = 0;

    zjj_insert_shm(psShmHead, caName, caPass);
    /*
    if(psOnline) {
        psOnline->psUser = ncSrvSynUserPassword(psShmHead,psOnline->lStype,psOnline->lGroupid,caName,caPass,psOnline->caGroupCode,psOnline->caSsid,NCSRV_USERTYPE_SMS);
        if(iUpdateUserFlags == 2) {
            ncSrvSynUserPassword2RadSrv(psOnline->psPar,caName,caPass,psOnline->lStype);
            pasLogs(PAS_SRCFILE,1008," Update User [%s]  Pass:[%s] Group:%s Ssid:%s  Stype:%u Ret=%d ",
                          caName,caPass,psOnline->caGroupCode,psOnline->caSsid,psOnline->lStype,iReturn);
        }
        strcpy(caGroupCode,psOnline->caGroupCode);
        if(utStrIsSpaces(caUserIp)) {
            strcpy(caUserIp,utComHostIp(htonl(psOnline->lSip)));
        }
    }
    else {
        ncSrvSynUserPassword(psShmHead,0,0,caName,caPass,"\0",utComGetVar_sd(psShmHead,"DefSsid","\0"),NCSRV_USERTYPE_SMS);
        if(iUpdateUserFlags == 2) {
            ncSrvSynUserPassword2RadSrv(psSumm->psPar,caName,caPass,0);
            pasLogs(PAS_SRCFILE,1008," Update User [%s]  Pass:[%s] iReturn=%d ",
                          caName,caPass,iReturn);
        }
    }*/
    /*
        sprintf(caHtml,"<statusCode>%s</statusCode><statusId>%s</statusId><message>%s</message>\r\n",
                        "0","1","Invalid Username");
        utComTcpResponse(iFd,psMsgHead,1,
                           "html",UT_TYPE_STRING,caHtml);
    */
    utPltPutVar(psDbHead, "statusId",   "1");
    utPltPutVar(psDbHead, "statusCode", "0");
    utPltPutVar(psDbHead, "message", "Get Password Sucess");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "login_getpass_return.htm");
    if(!utStrIsSpaces(caGroupCode))
    {
        memcpy(caStype, caGroupCode + 8, 2);
        caStype[2] = 0;
    }
    else
    {
        caStype[0] = 0;
    }
    // 输出到日志文件
    pasUtlOut2DbFile("ncsrvsmslog", 22,
                     "sid",       UT_TYPE_LONG8,  lSid,                            // 自动增加
                     "servicecode", UT_TYPE_STRING, caServicecode,
                     "did",        UT_TYPE_ULONG, 0,
                     "gid",        UT_TYPE_ULONG, 0,
                     "userid",    UT_TYPE_ULONG,  0,                               //
                     "username",  UT_TYPE_STRING, caName,
                     "usermac",   UT_TYPE_STRING, caUserMac,
                     "ip",        UT_TYPE_STRING, caUserIp,
                     "logtime",   UT_TYPE_ULONG,  lTime,
                     "smsname",    UT_TYPE_STRING, caSmsName0,
                     "sptype",    UT_TYPE_ULONG,   iSp,
                     "areacode",  UT_TYPE_ULONG,    iArea,
                     "areaname",  UT_TYPE_STRING, caArea,
                     "contrycode", UT_TYPE_ULONG,   iAreaCode,
                     "status",    UT_TYPE_ULONG,  lStatus,
                     "groupid",   UT_TYPE_ULONG,  lGroupid,
                     "tsid",      UT_TYPE_LONG8,  llTsid,
                     "groupcode", UT_TYPE_STRING, caGroupCode,
                     "stype",     UT_TYPE_STRING, caStype,
                     "utype",     UT_TYPE_ULONG,  lStype,
                     "feename",   UT_TYPE_STRING, caSmsName,
                     "apmac",     UT_TYPE_STRING, caApMac);
    return 0;
}

int ict_check_pass(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//检查手机验证码
{
    utMsgPrintMsg(psMsgHead);
    char caVname[32] = "";
    char pass[12] = "";
	memset(caVname, 0, sizeof(caVname));
    utMsgGetSomeNVar(psMsgHead, 1,
                     "vname", UT_TYPE_STRING,  sizeof(caVname) - 1, caVname);
    utPltDbHead *psDbHead = utPltInitDb();

    snprintf(pass, sizeof(pass)-1, "%s", zjj_find_shm(psShmHead, caVname));

    utPltPutVar(psDbHead, "pass", pass);
    utPltPutVarF(psDbHead, "result", "%d", 0);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/checkPass/check_pass.htm");

    return 0;

}

/*static void ncmexecdel_user(char username[32])
{
    char tmp[128] = "";
    system("cd /home/ncmysql/ncsrv/bin");
    sprintf(tmp,"./portalonline -k %s",username);
    printf("tmp = %s \n",tmp);
	system(tmp);
}

int checkUserOnline(utShmHead *psShmHead)
{
	pasHashInfo sHashInfo;
    uchar *pHash;
    ncPortalOnline  *psOnline;
	ulong lCount=0;
	uint8 nowTime=0;
	long sleepTime = 60; 
    pHash =  (unsigned char *)utShmHashHead(psShmHead,NCSRV_LNK_ONLINE);
    char sql[1024] = "";
	
    if(pHash == NULL) 
    {
        return (-1);
    }
    psOnline = (ncPortalOnline *)pasHashFirst(pHash,&sHashInfo);
    while(psOnline) 
    {
       // psUser = (ncPortalUser *) ( (char *)psOnline->psUser + lBase);
		//printf("mytsid = %llu   tsid = %llu\n",psOnline->lTsid,tsid);
      // if(psOnline->lTsid == tsid)
     //  {
 //      		  printf("platename = %s\n",psOnline->platename);
    //   		  strcpy(plateid,psOnline->platename);
 //      }	 
		nowTime=time(0);
		memset(sql,0,sizeof(sql));		
		snprintf(sql,sizeof(sql)-1,"select count(*) from userorder where username='%s' and status=1 and starttime<=nowTime and endtime>nowTime", psOnline->caName);	
		pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);
		if(lCount == 0)
		{
			ncmexecdel_user(psOnline->caName);		
		}
 
        psOnline = (ncPortalOnline *)pasHashNextS(&sHashInfo);
		sleepTime = utComGetVar_ld(psShmHead, "sleeptime", 60);
		sleep(sleepTime);
    }
	
}*/

int ictInitWebFun_gpx(utShmHead *psShmHead)
{
    pasSetTcpFunName("ict_orderLog_search", ict_orderLog_search, 0);//账单查询
	
    pasSetTcpFunName("ict_rechargelog_search", ict_rechargelog_search, 0);//充值记录查询 ict_create_orderno
	pasSetTcpFunName("ict_create_orderno", ict_create_orderno, 0);
    pasSetTcpFunName("ict_recharge", ict_recharge, 0);//充值接口
    pasSetTcpFunName("ict_check_user", ict_check_user, 0);//注册时检测用户
    pasSetTcpFunName("ict_register", ict_register, 0);//注册接口
    pasSetTcpFunName("ict_Auth", ict_Auth, 0);//pc端登录
    //pasSetTcpFunName("ict_AuthMobile", ict_AuthMobile, 0);//手机端登录 
    pasSetTcpFunName("ict_getUserInfo", ict_getUserInfo, 0);//获取用户信息
    pasSetTcpFunName("ict_update_userInfo", ict_update_userInfo, 0);//修改用户信息
    pasSetTcpFunName("ictSrvUserGetPass", ictSrvUserGetPass, 0);//获取验证码
    pasSetTcpFunName("ict_submit_form", ict_submit_form, 0);//
    pasSetTcpFunName("ict_index", ict_index, 0);//
    pasSetTcpFunName("ict_get_province", ict_get_province, 0);//获取省接口
    //  pasSetTcpFunName("ict_get_city", ict_get_city, 0);//获取市接口
    pasSetTcpFunName("ict_get_college", ict_get_college, 0);//获取学院
    //pasSetTcpFunName("ict_get_department", ict_get_department, 0);//获取班级
    pasSetTcpFunName("ict_balance_transfer", ict_balance_transfer, 0);//余额转让
    pasSetTcpFunName("ict_getRecPackage", ict_getRecPackage, 0);//获取推荐套餐
    pasSetTcpFunName("ict_check_upgradePackage", ict_check_upgradePackage, 0);//检测可升级套餐
    pasSetTcpFunName("ict_show_upgradePackage", ict_show_upgradePackage, 0);//显示可升级套餐
    pasSetTcpFunName("ict_package_balance", ict_package_balance, 0);//获取套餐余额
    pasSetTcpFunName("ict_upgrade_package", ict_upgrade_package, 0);//升级套餐
    pasSetTcpFunName("ict_auto_renewal", ict_auto_renewal, 0);//自动订购
    pasSetTcpFunName("ict_disconnect_net", ict_disconnect_net, 0);//断网
    pasSetTcpFunName("ict_safe_exit", ict_safe_exit, 0);//安全退出
    pasSetTcpFunName("ict_ncSrvLogOut", ict_ncSrvLogOut, 0);//安全退出
    pasSetTcpFunName("ictSrvGetPass", ictSrvGetPass, 0);
    pasSetTcpFunName("ict_check_pass", ict_check_pass, 0);


    return 0;
}




