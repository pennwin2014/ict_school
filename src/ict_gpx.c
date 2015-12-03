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
#include <iconv.h>

static int checkValidity(char dtime[24])//检测时间有效性
{
    int i;
    if(strlen(dtime) != 6)
    {
        return 1;
    }

    for(i = 0; i < 6; i++)
    {
        if(dtime[i] < '0' || dtime[i] > '9')
            return 1;
    }

    return 0;
}


int ict_orderLog_search(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//账单查询
{
    utMsgPrintMsg(psMsgHead);

    char vname[32] = "";
    char name[32] = "";
    ulong dstatus = 0;
    char caDtime[24] = "";
    char sql[1024] = "";
    char sql_count[1024] = "";
    char sqlbuf[1024] = "";
    char caTsid[24] = "";
    char start_in[8] = "";
    char limit_in[8] = "";
    ulong totalCount = 0;
    ulong lCount = 0;

    utMsgGetSomeNVar(psMsgHead, 5,
                     "dtime", UT_TYPE_STRING,  sizeof(caDtime) - 1, caDtime,
                     "start", UT_TYPE_STRING,  sizeof(start_in) - 1, start_in,
                     "limit", UT_TYPE_STRING,  sizeof(limit_in) - 1,  limit_in,
                     "tsid", UT_TYPE_STRING,  sizeof(caTsid) - 1,  caTsid
                    );

    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();
    if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/billCheck/bill_check.htm"))
    {
        return 0;
    }

    strcpy(vname, getVnameByTsId(psShmHead, atoll(caTsid)));
    if(strlen(vname) == 0)
    {
        utPltPutVarF(psDbHead, "result", "%d", 3);//用户名空
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/billCheck/bill_check.htm");
    }
    else
    {

        memset(sqlbuf, 0, sizeof(sqlbuf));
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select count(*) from orderlog where vname='%s'", vname);
        pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount <= 0)
        {
            utPltPutVarF(psDbHead, "result", "%d", 4);//用户名不存在
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/billCheck/bill_check.htm");
        }
        else
        {
            if(checkValidity(caDtime))
            {
                utPltPutVarF(psDbHead, "result", "%d", 5);//日期错误
                utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/billCheck/bill_check.htm");
            }
            else
            {
                memset(sql, 0, sizeof(sql));

                snprintf(sql, sizeof(sql) - 1, "select from_unixtime(dtime),dstatus,name from orderlog,package where orderlog.packageid=package.id and vname='%s' and left(from_unixtime(dtime,'%%Y%%m'),6)='%s' and 1=1 limit %u,%u", vname, caDtime, atoi(start_in), atoi(limit_in));
                snprintf(sql_count, sizeof(sql_count) - 1, "select count(*) from orderlog,package where orderlog.packageid=package.id and vname='%s' and left(from_unixtime(dtime,'%%Y%%m'),6)='%s'", vname, caDtime);
                printf("sql_count: %s\n", sql_count);
                psCur = pasDbOpenSql(sql_count, 0);
                pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &totalCount);
                pasDbCloseCursor(psCur);

                printf("zhaojianjian   totalCount is %lu\n", totalCount);
                psCur = pasDbOpenSql(sql, 0);

                char dtime[24] = "";
                int iret = 0, iNum = 0;
                if(psCur)
                {
                    while(0 == (iret = pasDbFetchInto(psCur,
                                                      UT_TYPE_STRING, sizeof(dtime) - 1, dtime,
                                                      UT_TYPE_LONG, 4, &dstatus,
                                                      UT_TYPE_STRING, sizeof(name) - 1, name)) || 1405 == iret)
                    {

                        iNum++;
                        if(iNum > 1)
                        {
                            utPltPutLoopVar(psDbHead, "dh", iNum, ",");
                        }
                        utPltPutLoopVarF(psDbHead, "dtime", iNum, dtime);
                        utPltPutLoopVarF(psDbHead, "dstatus", iNum, "%lu", dstatus);
                        utPltPutLoopVar(psDbHead, "name", iNum, convert("GBK", "UTF-8", name));
                    }
                    pasDbCloseCursor(psCur);
                }
                utPltPutVarF(psDbHead, "totalCount", "%lu", totalCount);
                utPltPutVarF(psDbHead, "tsid", caTsid);
                utPltPutVarF(psDbHead, "result", "%d", 0);
                utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/billCheck/bill_check.htm");

            }
        }
    }

    return 0;
}


int ict_rechargelog_search(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//充值记录查询
{

    utMsgPrintMsg(psMsgHead);

    char vname[32] = "";
    ulong money = 0;
    ulong mtype = 0;
    char caDtime[24] = "";
    char sql[1024] = "";
    char sql_count[1024] = "";
    char sqlbuf[1024] = "";
    char caTsid[24] = "";
    char start_in[8] = "";
    char limit_in[8] = "";
    ulong totalCount = 0;
    ulong lCount = 0;

    utMsgGetSomeNVar(psMsgHead, 4,
                     "timeval", UT_TYPE_STRING,  sizeof(caDtime) - 1, caDtime,
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

    strcpy(vname, getVnameByTsId(psShmHead, atoll(caTsid)));
    if(strlen(vname) == 0)
    {
        utPltPutVarF(psDbHead, "result", "%d", 3);//用户名空
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/pay_record.htm");
    }
    else
    {

        memset(sqlbuf, 0, sizeof(sqlbuf));
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select count(*) from rechargelog where vname='%s'", vname);
        pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount <= 0)
        {
            utPltPutVarF(psDbHead, "result", "%d", 4);//用户名不存在
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/pay_record.htm");
        }
        else
        {
            if(checkValidity(caDtime))
            {
                utPltPutVarF(psDbHead, "result", "%d", 5);//日期错误
                utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/pay_record.htm");
            }
            else
            {
                memset(sql, 0, sizeof(sql));
                printf("zhaojjjjjjj******%s,%s\n", start_in, limit_in);
                snprintf(sql, sizeof(sql) - 1, "select from_unixtime(timeval),money,mtype from rechargelog where vname='%s' and left(from_unixtime(timeval,'%%Y%%m'),6)='%s' and 1=1 limit %u,%u", vname, caDtime, atoi(start_in), atoi(limit_in));
                snprintf(sql_count, sizeof(sql_count) - 1, "select count(*) from rechargelog where vname='%s' and left(from_unixtime(timeval,'%%Y%%m'),6)='%s'", vname, caDtime);
                printf("sql_count: %s\n", sql_count);
                psCur = pasDbOpenSql(sql_count, 0);
                pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &totalCount);
                pasDbCloseCursor(psCur);
                //pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &totalCount);
                printf("zhaojianjian   totalCount is %lu\n", totalCount);

                psCur = pasDbOpenSql(sql, 0);
                char timeval[24] = "";
                int iret = 0, iNum = 0;
                if(psCur)
                {
                    while(0 == (iret = pasDbFetchInto(psCur,
                                                      UT_TYPE_STRING, sizeof(timeval), timeval,
                                                      UT_TYPE_LONG, 4, &money,
                                                      UT_TYPE_LONG, 4, &mtype)) || 1405 == iret)
                    {

                        iNum++;
                        if(iNum > 1)
                        {
                            utPltPutLoopVar(psDbHead, "dh", iNum, ",");
                        }
                        utPltPutLoopVarF(psDbHead, "timeval", iNum, timeval);
                        utPltPutLoopVarF(psDbHead, "money", iNum, "%lu", money);
                        utPltPutLoopVarF(psDbHead, "mtype", iNum, "%lu", mtype);
                    }
                    pasDbCloseCursor(psCur);
                }
                utPltPutVarF(psDbHead, "totalCount", "%lu", totalCount);
                utPltPutVarF(psDbHead, "tsid", caTsid);
                utPltPutVarF(psDbHead, "result", "%d", 0);
                utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/pay_record.htm");
            }
        }
    }

    return 0;
}

int ict_recharge(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)//账户充值
{
    utMsgPrintMsg(psMsgHead);

    char vname[32] = "";
    char orderno[64] = "";
    char money[8] = "";
    char mtype[4] = "";
    char caTsid[24] = "";
    char sql_user[1024] = "";
    char sql_rec[1024] = "";
    char sqlbuf[1024] = "";
    char caMsg[256] = "";
    uint8 timeval = 0;
    ulong lCount = 0;
    int iReturn = 0;


    utMsgGetSomeNVar(psMsgHead, 4,
                     "orderno", UT_TYPE_STRING,  sizeof(orderno) - 1, orderno,
                     "money", UT_TYPE_STRING,  sizeof(money) - 1, money,
                     "mtype", UT_TYPE_STRING,  sizeof(mtype) - 1, mtype,
                     "tsid", UT_TYPE_STRING,  sizeof(caTsid) - 1,  caTsid
                    );
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();

    if(checkTsid(psShmHead, psDbHead, psMsgHead, iFd, caTsid, "school/payRecord/pay_result.htm"))
    {
        return 0;
    }

    strcpy(vname, getVnameByTsId(psShmHead, atoll(caTsid)));
    if(strlen(vname) == 0)
    {
        utPltPutVarF(psDbHead, "result", "%d", 3);//用户名空
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/pay_record.htm");
    }
    else
    {
        memset(sqlbuf, 0, sizeof(sqlbuf));
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select count(*) from userlib where vname='%s'", vname);
        pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount <= 0)
        {
            utPltPutVarF(psDbHead, "result", "%d", 4);//用户名不存在
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/pay_record/pay_result.htm");
        }
        else
        {
            memset(sql_user, 0, sizeof(sql_user));
            snprintf(sql_user, sizeof(sql_user) - 1, "update userlib set money=money+'%lu' where vname='%s'", atol(money), vname);
            iReturn = pasDbExecSqlF(sql_user);
            if(iReturn != 0)
            {
                snprintf(caMsg, sizeof(caMsg) - 1, "充值失败,错误码=[%d]", iReturn);
                utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
                utPltPutVarF(psDbHead, "result", "%d", 5);//充值失败
                return 0;
            }
            else
            {
                timeval = time(0);
                memset(sql_rec, 0, sizeof(sql_rec));
                snprintf(sql_rec, sizeof(sql_rec) - 1, "insert into rechargelog(vname,money,mtype,timeval,orderno) values('%s',%lu,%lu,%llu,'%s')", vname , atol(money), atol(mtype), timeval, orderno);
                iReturn = pasDbExecSqlF(sql_rec);
                if(iReturn != 0)
                {
                    snprintf(caMsg, sizeof(caMsg) - 1, "写入充值记录失败,错误码=[%d]", iReturn);
                    utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
                    utPltPutVarF(psDbHead, "result", "%d", 6);//写入充值记录失败
                    return 0;
                }
                else
                {
                    utPltPutVarF(psDbHead, "tsid", caTsid);
                    utPltPutVarF(psDbHead, "result", "%d", 0);
                    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/pay_result.htm");
                }
            }
        }
    }
    return 0;
}

int ictInitWebFun_gpx(utShmHead *psShmHead)
{
    pasSetTcpFunName("ict_orderLog_search", ict_orderLog_search, 0);
    pasSetTcpFunName("ict_rechargelog_search", ict_rechargelog_search, 0);
    pasSetTcpFunName("ict_recharge", ict_recharge, 0);
    return 0;
}




