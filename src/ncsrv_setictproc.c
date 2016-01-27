#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utoall.h"
#include "pasutl.h"
#include "pasdb.h"
#include "pasutl.h"
#include "ncportal.h"
#define  PAS_SRCFILE 7000
int checkUserOnline(utShmHead *psShmHead);
int ictStatCount(utShmHead *psShmHead);

static void ncmexecdel_user(utShmHead *psShmHead, char username[32])
{
    /* char tmp[128] = "";
     system("cd /home/ncmysql/ncsrv/bin");
     sprintf(tmp,"./portalonline -t %s",username);
    pasLog(6111,6111,"tmp=%s\n",tmp);
    system(tmp);*/
    ncSrvLetUserOffLineByTsid(psShmHead, ncmWebGetTisdByUname(psShmHead, username));
}

int ncSrvSetIctProcess(utShmHead *psShmHead)
{

    pasUtlSetProcessNameD(psShmHead,
                          "CkUserOn",             // 进程名称
                          "No",                // StartImpLog 的缺省值  Yes -- 启动   No--不启动
                          checkUserOnline,  // 调用的函数名
                          7100,                 // 进程超时时间
                          0);                   //
    //统计表的统计进程
    pasUtlSetProcessNameD(psShmHead, "IctCount", "No", ictStatCount, 68400L, 0);
    return 0;
}

int ictStatUserPerDay(utShmHead *psShmHead, long lStart, long lEnd)
{
    char caTemp[2048] = "";
    strcpy(caTemp, "insert into ictusercount(groupcode,apname,lcount,sdate) select groupcode,apname,count(*),from_unixtime(starttime,'\%Y\%m\%d') aa ");

    sprintf(caTemp + strlen(caTemp), " from  ncsrvuserlog where starttime>=%d and starttime<%d ", lStart, lEnd);

    strcat(caTemp, " group by groupcode,apname,aa ");
    pasLogs(1066, 1601, "caTemp=%s\n", caTemp);
    pasDbExecSql(caTemp, 0);
}


int ictStatPackagePerDay(utShmHead *psShmHead, long lStart, long lEnd)
{
    uint8 lTime = time(0);

    char caTemp[2048] = "";
    strcpy(caTemp, "insert into ictpackagecount(packageid,mark,lcount,sdate) select packageid,mark,count(*),from_unixtime(timeval,'\%Y\%m\%d') aa ");

    sprintf(caTemp + strlen(caTemp), " from  userorderlog_%s where timeval>=%d and timeval<%d ", utTimFormat("%Y%m", lTime), lStart, lEnd);

    strcat(caTemp, " group by mark,packageid,aa ");
    pasLogs(1066, 1601, "caTemp=%s\n", caTemp);
    pasDbExecSql(caTemp, 0);
}

int ictStatChargePerDay(utShmHead *psShmHead, long lStart, long lEnd)
{
    uint8 lTime = time(0);

    char caTemp[2048] = "";
    strcpy(caTemp, "insert into ictchargecount(mtype,mark,lcount,sdate) select mtype,mark,sum(money),from_unixtime(timeval,'\%Y\%m\%d') aa ");

    sprintf(caTemp + strlen(caTemp), " from  rechargelog_%s where timeval>=%d and timeval<%d ", utTimFormat("%Y%m", lTime), lStart, lEnd);

    strcat(caTemp, " group by mark,mtype,aa ");
    pasLogs(1066, 1601, "caTemp=%s\n", caTemp);
    pasDbExecSql(caTemp, 0);
}


int ictStatCount(utShmHead *psShmHead)
{
    pasDbCursor *psCur;
    char caReportname[64], caGname_rp[64], caWorktime_rp[16];
    int iReturn;
    long lLasttime, lTime, lYtime;
    char caNoWH[16], caSdate[20];
    long lDid;
    long long lSid;
    char caYM[12];
    char caTemp[2048];
    iReturn = pasConnect(psShmHead);
    if(iReturn < 0)
    {
        sleep(60);
        return 0;
    }

    while(1)
    {
        lTime = time(0);
        strcpy(caNoWH, utTimFormat("%H", lTime));

        if(strcmp(caNoWH, "15") == 0)
        {
            lYtime = lTime - 3600 * 15;
            sprintf(caSdate, "%s 23:59:59", utTimFormat("%Y/%m/%d", lYtime));
            lYtime = utTimStrToLong("%Y/%m/%d %H:%M:%S", caSdate);
            //统计每天用户数量
            lLasttime = pasGetLastInfo_l("UserCountStatTime", 0);
            pasLogs(1066, 1601, "UserCountStatTime lLasttime=%d", lLasttime);
            if(lYtime > lLasttime)
            {
                ictStatUserPerDay(psShmHead, lLasttime, lYtime);
                pasLogs(1066, 1601, "ready set lYtime=%d", lYtime);
                pasSetLastInfo_l("UserCountStatTime", lYtime);
                pasLogs(1066, 1601, "finish set lYtime=%d", lYtime);
            }
            //统计订单
            lLasttime = pasGetLastInfo_l("PackCountStatTime", 0);
            if(lYtime > lLasttime)
            {
                ictStatPackagePerDay(psShmHead, lLasttime, lYtime);
                pasSetLastInfo_l("PackCountStatTime", lYtime);
            }
            //统计充值
            lLasttime = pasGetLastInfo_l("ChargeCountStatTime", 0);
            if(lYtime > lLasttime)
            {
                ictStatChargePerDay(psShmHead, lLasttime, lYtime);
                pasSetLastInfo_l("ChargeCountStatTime", lYtime);
            }

        }
        sleep(600);
    }
}


int checkUserOnline(utShmHead *psShmHead)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    ncPortalOnline  *psOnline;
    ulong lCount = 0;
    uint8 nowTime = 0;
    long sleepTime = 60;
    int iReturn = 0;
    ulong count = 0;
    char sql[1024] = "";
    char vname[32] = "";
    char mobno[16] = "";
    ulong packageid = 0;
    char name[64] = "";
    uint8 endtime = 0;
    uint8 minendtime = 0;
    char caMark[128] = "";
    char autoxiding[4] = "";

    pasDbCursor *psCur = NULL;
    iReturn = pasConnect(psShmHead);
    if(iReturn != 0)
    {
        pasLogs(5001, 5001, "connect error iReturn=%d\n", iReturn);
        sleep(5);
        return 0;
    }
    sleepTime = utComGetVar_ld(psShmHead, "sleeptime", 60);
    while(1)
    {
        pHash = (unsigned char *)utShmHashHead(psShmHead, NCSRV_LNK_ONLINE);
        if(pHash == NULL)
        {
            return (-1);
        }
        psOnline = (ncPortalOnline *)pasHashFirst(pHash, &sHashInfo);
        while(psOnline)
        {
            if(strlen(psOnline->caName) == 0)
            {
                psOnline = (ncPortalOnline *)pasHashNextS(&sHashInfo);
                continue;
            }
            nowTime = time(0);
            snprintf(sql, sizeof(sql) - 1, "select count(*) from userorder where username='%s' and status=1", psOnline->caName);
            pasDbOneRecord(sql, 0,  UT_TYPE_ULONG, 4, &count);
            if(count == 0) //无套餐 踢下线
            {
                if(psOnline->login == 1)
                {
                    ncmexecdel_user(psShmHead, psOnline->caName);
                    pasLogs(6111, 6111, "无套餐 剔除 name=%s\n", psOnline->caName);
                }
            }

            if(count == 2) //2个套餐
            {
                snprintf(sql, sizeof(sql) - 1, "select min(endtime) from userorder where username='%s' and status=1 and autoxiding=0", psOnline->caName);
                pasDbOneRecord(sql, 0,  UT_TYPE_LONG8, 8, &minendtime);
                if(minendtime <= nowTime)
                {
                    memset(sql, 0, sizeof(sql));
                    snprintf(sql, sizeof(sql) - 1, "update userorder set status='2' where username='%s' and endtime=%llu", psOnline->caName, minendtime);
                    pasDbExecSqlF(sql);
                    pasLogs(6111, 6111, "套餐一失效 套餐二生效 name=%s\n", psOnline->caName);
                }
            }
            if(count == 1)
            {
                memset(sql, 0, sizeof(sql));
                snprintf(sql, sizeof(sql) - 1, "select mobno,packageid,name,endtime,autoxiding from userorder where username='%s' and status=1", psOnline->caName, psOnline->caName);
                pasDbOneRecord(sql, 0, UT_TYPE_STRING, sizeof(mobno) - 1, mobno,
                               UT_TYPE_ULONG, 4, &packageid,
                               UT_TYPE_STRING, sizeof(name) - 1, name,
                               UT_TYPE_LONG8, 8, &endtime,
                               UT_TYPE_STRING, sizeof(autoxiding) - 1, autoxiding);
                if(endtime <= nowTime)
                {
                    if(autoxiding[0] == '0')
                    {
                        memset(sql, 0, sizeof(sql));
                        snprintf(sql, sizeof(sql) - 1, "update userorder set status='2' where username='%s'",  psOnline->caName);
                        pasDbExecSqlF(sql);
                        if(psOnline->login == 1)
                        {
                            ncmexecdel_user(psShmHead, psOnline->caName);
                            pasLogs(6111, 6111, "套餐结束，但未自动订购 剔除sql=%s name=%s\n", sql, psOnline->caName);
                        }

                    }
                    if(autoxiding[0] == '1')
                    {
                        memset(sql, 0, sizeof(sql));
                        snprintf(sql, sizeof(sql) - 1, "select ncsrvuserex.money,package.money,package.pdays from ncsrvuserex,package where ncsrvuserex.username='%s' and package.id=%lu",  psOnline->caName, packageid);
                        long lmoney = 0;
                        long pmoney = 0;
                        long pdays = 0;
                        pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lmoney,
                                       UT_TYPE_LONG, 4, &pmoney,
                                       UT_TYPE_LONG, 4, &pdays);
                        if(lmoney < pmoney)
                        {
                            //余额不足，无法自动订购
                            memset(sql, 0, sizeof(sql));
                            snprintf(sql, sizeof(sql) - 1, "update userorder set status='2' where username='%s'",  psOnline->caName);
                            pasDbExecSqlF(sql);
                            if(psOnline->login == 1)
                            {
                                ncmexecdel_user(psShmHead, psOnline->caName);
                                pasLogs(6111, 6111, "余额不足无法自动订购，剔除，余额%ld,续订套餐价格%ld\n", lmoney, pmoney);
                            }

                        }
                        else
                        {
                            snprintf(caMark, sizeof(caMark) - 1, "%s", utComGetVar_sd(psShmHead, "mark", "0"));

                            memset(sql, 0, sizeof(sql));
                            snprintf(sql, sizeof(sql) - 1, "update ncsrvuserex set money=money-%ld where username='%s'", pmoney, psOnline->caName);
                            pasDbExecSqlF(sql);
                            pasLogs(6111, 6111, "扣款:%s\n", sql);

                            memset(sql, 0, sizeof(sql));
                            snprintf(sql, sizeof(sql) - 1, "update userorder set status=2 where username='%s'",  psOnline->caName); //原套餐失效
                            pasDbExecSqlF(sql);
                            pasLogs(6111, 6111, "原套餐失效:%s\n", sql);

                            memset(sql, 0, sizeof(sql));
                            //  snprintf(sql, sizeof(sql)-1, "update userorder set starttime=%llu,endtime=%llu,timeval=%llu",nowtime,nowtime+pdays*24*3600,nowtime);
                            snprintf(sql, sizeof(sql) - 1, "insert into userorder(username,mobno,mark,packageid,name,starttime,endtime,autoxiding,status,timeval) values('%s','%s','%s',%lu,'%s',%llu,%llu,'1','1',%llu)", psOnline->caName, mobno, caMark, packageid, name, endtime, endtime + pdays * 24 * 3600, endtime);
                            pasDbExecSqlF(sql);
                            pasLogs(6111, 6111, "写入userorder:%s\n", sql);

                            char tableTime[12] = "";
                            char caName[64] = "";
                            snprintf(caName, sizeof(caName) - 1, "%s(续订)", name);
                            snprintf(tableTime, sizeof(tableTime) - 1, "%s", utTimFormat("%Y%m", nowTime));
                            memset(sql, 0, sizeof(sql));
                            snprintf(sql, sizeof(sql) - 1, "insert into userorderlog_%s(username,mobno,mark,packageid,name,status,money,starttime,endtime,timeval) values('%s','%s','%s',%lu,'%s','2',%lu,%llu,%llu,%llu)", tableTime,  psOnline->caName, mobno, caMark, packageid, caName, pmoney, endtime, endtime + pdays * 24 * 3600, endtime);
                            pasDbExecSqlF(sql);
                            pasLogs(6111, 6111, "写入userorderlog:%s\n", sql);
                        }
                    }
                }
            }
            psOnline = (ncPortalOnline *)pasHashNextS(&sHashInfo);
        }
        sleep(sleepTime);
    }
    return 0;
}

