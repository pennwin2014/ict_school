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

int ict_orderLog_search(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{

    utMsgPrintMsg(psMsgHead);

    ulong id = 0;
    char vname[32] = "";
    char name[32] = "";
    ulong dstatus = 0;
    char caDtime[24] = "";

    utMsgGetSomeNVar(psMsgHead, 5,
                     "id", UT_TYPE_LONG,  sizeof(id), &id,
                     "vname", UT_TYPE_STRING,  sizeof(vname) - 1, vname,
                     "name", UT_TYPE_STRING,  sizeof(name) - 1, name,
                     "dstatus", UT_TYPE_LONG,  sizeof(dstatus), &dstatus,
                     "dtime", UT_TYPE_STRING,  sizeof(caDtime) - 1, caDtime
                    );


    char sql[1024] = "";

    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();

    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select dtime,dstatus,name from orderlog");
    printf("sql:%s\n", sql);
    psCur = pasDbOpenSql(sql, 0);
    uint8 dtime = 0;
    int iret = 0, iNum = 0;
    if(psCur)
    {
        while(0 == (iret = pasDbFetchInto(psCur,
                                          UT_TYPE_LONG8, 8, &dtime,
                                          UT_TYPE_LONG, 4, &dstatus,
                                          UT_TYPE_STRING, sizeof(name) - 1, name)) || 1405 == iret)
        {

            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVarF(psDbHead, "dtime", iNum, "%llu", dtime);
            utPltPutLoopVarF(psDbHead, "dstatus", iNum, "%lu", dstatus);
            utPltPutLoopVar(psDbHead, "name", iNum, name);
        }
        pasDbCloseCursor(psCur);
    }
    utPltPutVarF(psDbHead, "totalCount", "%d", iNum);
    //utPltShowDb(psDbHead);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/billCheck/bill_check.htm");
    return 0;
}


int ict_rechargelog_search(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{

    utMsgPrintMsg(psMsgHead);

    ulong id = 0;
    char vname[32] = "";
    ulong money = 0;
    ulong mtype = 0;
    char caDtime[24] = "";

    utMsgGetSomeNVar(psMsgHead, 5,
                     "id", UT_TYPE_LONG,  sizeof(id), &id,
                     "vname", UT_TYPE_STRING,  sizeof(vname) - 1, vname,
                     "money", UT_TYPE_LONG,  sizeof(money) - 1, &money,
                     "mtype", UT_TYPE_LONG,  sizeof(mtype), &mtype,
                     "timeval", UT_TYPE_STRING,  sizeof(caDtime) - 1, caDtime
                    );


    char sql[1024] = "";

    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();

    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select timeval,money,mtype from rechargelog");
    printf("sql:%s\n", sql);
    psCur = pasDbOpenSql(sql, 0);
    uint8 timeval = 0;
    int iret = 0, iNum = 0;
    if(psCur)
    {
        while(0 == (iret = pasDbFetchInto(psCur,
                                          UT_TYPE_LONG8, 8, &timeval,
                                          UT_TYPE_LONG, 4, &money,
                                          UT_TYPE_LONG, 4, &mtype)) || 1405 == iret)
        {

            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVarF(psDbHead, "timeval", iNum, "%llu", timeval);
            utPltPutLoopVarF(psDbHead, "money", iNum, "%lu", money);
            utPltPutLoopVarF(psDbHead, "mtype", iNum, "%lu", mtype);
        }
        pasDbCloseCursor(psCur);
    }
    utPltPutVarF(psDbHead, "totalCount", "%d", iNum);
    //utPltShowDb(psDbHead);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/payRecord/pay_record.htm");
    return 0;
}

int ictInitWebFun_gpx(utShmHead *psShmHead)
{
    pasSetTcpFunName("ict_orderLog_search", ict_orderLog_search, 0);
    pasSetTcpFunName("ict_rechargelog_search", ict_rechargelog_search, 0);
    return 0;
}




