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

static int ictPrint(long isDebug, const char* fmt, ...)
{
    int retCnt = 0;
    if(isDebug)
    {
        va_list ap;
        va_start(ap, fmt);
        retCnt = vprintf(fmt, ap);
        va_end(ap);
    }
    return retCnt;
}


int isVnameRegistered(char* vName)
{

    return 0;
}

static uint8 getTsidByVname(utShmHead *psShmHead, char* vName)
{
    uint8 ltsid = 0;
    pasHashInfo sHashInfo;
    uchar *pHash = NULL;
    ictOnlineUser* psOnline = NULL;
    pHash = (unsigned char *)utShmHashHead(psShmHead, ICT_USER_LOGIN_TSID);
    if(pHash == NULL)
    {
        utShmFreeHash(psShmHead, ICT_USER_LOGIN_TSID);
        utShmHashInit(psShmHead, ICT_USER_LOGIN_TSID, 2000, 2000, sizeof(ictOnlineUser), 0, 8);
    }
    psOnline = (ictOnlineUser*)pasHashFirst(pHash, &sHashInfo);
    while(psOnline)
    {
        if(strcmp(psOnline->vName, vName) == 0)
        {
            return psOnline->tsid;
        }
        psOnline = (ictOnlineUser *)pasHashNextS(&sHashInfo);
    }
    ltsid = time(0);
    ictOnlineUser* psData = (ictOnlineUser*)utShmHashLookA(psShmHead, ICT_USER_LOGIN_TSID, (char*)(&ltsid));
    if(psData)
    {
        psData->tsid = ltsid;
        strcpy(psData->vName, vName);
    }
    return ltsid;

}


static char* getVnameByTsId(utShmHead *psShmHead, uint8 ltsid)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    ictOnlineUser* psOnline;
    static char caVname[32] = "";

    ictOnlineUser* psData = (ictOnlineUser*)utShmHashLookA(psShmHead, ICT_USER_LOGIN_TSID, (char*)(&ltsid));
    if(psData)
    {
        //psData->tsid = ltsid;
        memset(caVname, 0, sizeof(caVname));
        memcpy(caVname, psData->vName, sizeof(psData->vName));
    }
    return caVname;
}

int ict_register(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    //sql语句处理有三种，查询的话用pasDbOneRecord
    //pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);一般是只返回一个数据，将数据存在lCount中
    //insert,delete,update等用pasDbExecSqlF?
    //查询语句用 psCur = pasDbOpenSql(sql, 0);
    long wpDebug = utComGetVar_ld(psShmHead, "wpDebug", 0);
    int iReturn = 0;
    utMsgPrintMsg(psMsgHead);
    char caVname[32 + 1] = "";
    char caDname[24 + 1] = "";
    char caPasswd[24 + 1] = "";
    char caAddress[128 + 1] = "";
    char caMname[24 + 1] = ""; //姓名
    char sqlbuf[1024] = "";
    ulong lCount = 0;
    char caMsg[256] = "";

    iReturn = utMsgGetSomeNVar(psMsgHead, 5,
                               "vname",   UT_TYPE_STRING, sizeof(caVname) - 1, caVname,
                               "dname",   UT_TYPE_STRING, sizeof(caDname) - 1, caDname,
                               "passwd",  UT_TYPE_STRING, sizeof(caPasswd) - 1, caPasswd,
                               "address",   UT_TYPE_STRING, sizeof(caAddress) - 1, caAddress,
                               "mname",   UT_TYPE_STRING, sizeof(caMname) - 1, caMname);
    utStrDelSpaces(caAddress);
    utStrDelSpaces(caVname);
    //utPltDbHead* psDbHead = utPltInitDbHead();
    utPltDbHead* psDbHead = utPltInitDb();
    if(strlen(caVname) > 0)
    {
        memset(sqlbuf, 0, sizeof(sqlbuf));
        //先判断数据库里是否有该caVname，有的话直接报错
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select count(*) from userlib where vname='%s'", caVname);
        ictPrint(wpDebug, "sqlbuf=%s  \n", sqlbuf);
        pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount > 0)
        {
            snprintf(caMsg, sizeof(caMsg) - 1, "该vname[%s]已经存在", caVname);
            utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
            utPltPutVarF(psDbHead, "result", "%d", 1);//表示已经存在该vname
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/register/register.htm");
            return 0;
        }
        //printf();
        //插入数据库
        memset(sqlbuf, 0, sizeof(sqlbuf));
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "insert into userlib (vname,dname,address,passwd,mname) values('%s','%s','%s','%s','%s')",
                 caVname, caDname, caAddress, caPasswd, caMname);
        /*snprintf(sqlbuf, sizeof(sqlbuf) - 1, "insert into userlib (vname,dname,address,passwd,mname) values('%s','%s','%s','%s','%s')",
                utf8convert(psShmHead,caVname), utf8convert(psShmHead,caDname), utf8convert(psShmHead,caPasswd), utf8convert(psShmHead,caAddress), utf8convert(psShmHead,caMname));   */
        //snprintf(sql,sizeof(sql),"insert into help (messageTitle,messageContent)values('%s','%s')",utf8convert(psShmHead,messageTitle),utf8convert(psShmHead,messageContent));
        ictPrint(wpDebug, "insert sql=%s\n", sqlbuf);
        iReturn = pasDbExecSqlF(sqlbuf);
        //pasDbExecSql(sqlbuf, 0);
        ictPrint(wpDebug, "iReturn=%d\n", iReturn);
        if(iReturn != 0)
        {
            snprintf(caMsg, sizeof(caMsg) - 1, "操作数据库失败,错误码=[%d]", iReturn);
            utPltPutVar(psDbHead, "mesg", caMsg);
            //sql语句操作回滚
            //pasDbRollback(NULL);
        }
        else
        {
            // pasDbCommit(NULL);
        }
        utPltPutVarF(psDbHead, "result", "%d", iReturn);
        utPltPutVarF(psDbHead, "tsid", "%llu", getTsidByVname(psShmHead, caVname));
    }
    else
    {
        snprintf(caMsg, sizeof(caMsg) - 1, "vname不可以为空");
        utPltPutVar(psDbHead, "mesg", caMsg);
        utPltPutVarF(psDbHead, "result", "%d", 2);
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/register/register.htm");
    return 0;
}

int addUserToShm(utShmHead *psShmHead)
{

    return 0;
}

int isInUserShm(utShmHead *psShmHead)
{

    return 0;
}

//static uint8 getTsIdByVname(char* vname){

//return 12345;

//}
int ict_AuthMobile(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select count(*) from userlib where vname='%s' and passwd='%s'", caVname, caPasswd);
        ictPrint(wpDebug, "sqlbuf=%s  \n", sqlbuf);
        pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount <= 0)
        {
            snprintf(caMsg, sizeof(caMsg) - 1, "该vname[%s]不存在或者密码错误", caVname);
            utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
            utPltPutVarF(psDbHead, "result", "%d", 1);



        }
        else
        {
            //将用户加入共享内存
            addUserToShm(caVname);
            utPltPutVarF(psDbHead, "result", "%d", 0);


            utPltPutVarF(psDbHead, "tsid", "%llu", getTsidByVname(psShmHead, caVname));
        }
    }
    else
    {
        snprintf(caMsg, sizeof(caMsg) - 1, "vname不可以为空");
        utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
        utPltPutVarF(psDbHead, "result", "%d", 2);
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/login/login.htm");
    return 0;
}

int ict_Auth(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select count(*) from userlib where vname='%s' and passwd='%s'", caVname, caPasswd);
        ictPrint(wpDebug, "sqlbuf=%s  \n", sqlbuf);
        pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount <= 0)
        {
            snprintf(caMsg, sizeof(caMsg) - 1, "该vname[%s]不存在或者密码错误", caVname);
            utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
            utPltPutVarF(psDbHead, "result", "%d", 1);
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/main/login_error.htm");
        }
        else
        {
            //将用户加入共享内存
            addUserToShm(caVname);
            utPltPutVarF(psDbHead, "result", "%d", 0);
            utPltPutVarF(psDbHead, "tsid", "%llu", getTsidByVname(psShmHead, caVname));
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/main/index.htm");
        }
    }
    else
    {
        snprintf(caMsg, sizeof(caMsg) - 1, "vname不可以为空");
        utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
        utPltPutVarF(psDbHead, "result", "%d", 2);
        utPltPutVarF(psDbHead, "tsid", "%llu", getTsidByVname(psShmHead, caVname));
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/main/login_error.htm");
    }

    return 0;
}

int ict_getUserInfo(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    int iReturn = 0;
    utMsgPrintMsg(psMsgHead);
    long wpDebug = utComGetVar_ld(psShmHead, "wpDebug", 0);
    char caTsid[512] = {0};
    uint8 ltsid = 0;
    char caVname[32 + 1] = "";
    char caBname[32 + 1] = "";
    char caDname[24 + 1] = "";
    char sqlbuf[1024] = "";
    ulong lCount = 0;
    ulong lBtype = 0;
    ulong lMoney = 0;
    char caMsg[256] = "";
    utPltDbHead *psDbHead = utPltInitDbHead();

    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "tsid",   UT_TYPE_STRING, sizeof(caTsid) - 1, caTsid);
    utStrDelSpaces(caTsid);
    ictPrint(wpDebug, "tsid=%s\n", caTsid);
    if(strlen(caTsid) > 0)
    {
        ltsid = atoll(caTsid);
    }
    //暂时把所有信息都返回
    strcpy(caVname, getVnameByTsId(psShmHead, ltsid));
    utPltPutVar(psDbHead, "vname", caVname);
    //查表得到vname对应的信息
    memset(sqlbuf, 0, sizeof(sqlbuf));
    snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select dname,package.id,package.name,userlib.money from userlib,package where userlib.package=package.id and vname='%s'", caVname);
    ictPrint(wpDebug, "sqlbuf=%s  \n", sqlbuf);
    pasDbOneRecord(sqlbuf, 0, UT_TYPE_STRING, sizeof(caDname) - 1, caDname,
                   UT_TYPE_LONG, 4, &lBtype,
                   UT_TYPE_STRING, sizeof(caBname) - 1, caBname,
                   UT_TYPE_LONG, 4, &lMoney);

    utPltPutVarF(psDbHead, "result", "%d", 0);
    utPltPutVar(psDbHead, "username", convert("GBK", "UTF-8", caDname));

    utPltPutVarF(psDbHead, "btype", "%lu", lBtype);
    utPltPutVar(psDbHead, "bname", convert("GBK", "UTF-8", caBname));
    utPltPutVarF(psDbHead, "money", "%lu", lMoney);
	ictPrint(wpDebug, "caBname=%s\n", caBname);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/main/userInfo.htm");
    return 0;
}

// 获取验证码
int ictSrvUserGetPass(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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

int ict_getRecPackage(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utMsgPrintMsg(psMsgHead);
    char caName[32 + 1] = "";
    char caNamedes[128 + 1] = "";
    ulong lPtype = 0;
    ulong lMoney = 0;
    uint8 lId = 0;
    int iret = -1;
    int iNum = 0;
    char start_in[8] = "";
    char limit_in[8] = "";
    utMsgGetSomeNVar(psMsgHead, 2,
                     "start",    UT_TYPE_STRING,  sizeof(start_in) - 1,    start_in,
                     "limit",        UT_TYPE_STRING,      sizeof(limit_in) - 1,    limit_in);
    char sql[1024] = "";
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();
    int db_count = 0;
    snprintf(sql, sizeof(sql) - 1, "select package.id,name,namedes,ptype,money from package,recpackage where package.id=recpackage.id and useflag=1");
    psCur = pasDbOpenSql(sql, 0);
    if(psCur != NULL)
    {
        while(0 == (iret = pasDbFetchInto(psCur,
                                          UT_TYPE_LONG8, 8, &lId,
                                          UT_TYPE_STRING, sizeof(caName) - 1, caName,
                                          UT_TYPE_STRING, sizeof(caNamedes) - 1, caNamedes,
                                          UT_TYPE_ULONG, 4, &lPtype,
                                          UT_TYPE_ULONG, 4, &lMoney)) || 1405 == iret)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVarF(psDbHead, "id", iNum, "%llu", lId);
            utPltPutLoopVar(psDbHead, "name", iNum, convert("GBK", "UTF-8", caName));
            utPltPutLoopVar(psDbHead, "namedes", iNum, convert("GBK", "UTF-8", caNamedes));
            utPltPutLoopVarF(psDbHead, "ptype", iNum, "%lu", lPtype);
            utPltPutLoopVarF(psDbHead, "money", iNum, "%lu", lMoney);
        }
        pasDbCloseCursor(psCur);
    }
    utPltPutVarF(psDbHead, "TotRec", "%lu", iNum);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/package/rec_package.htm");
    return 0;
}

int ictInitWebFun_wp(utShmHead *psShmHead)
{
    pasSetTcpFunName("ict_register", ict_register, 0);
    pasSetTcpFunName("ict_Auth", ict_Auth, 0);
    pasSetTcpFunName("ict_AuthMobile", ict_AuthMobile, 0);
    pasSetTcpFunName("ict_getUserInfo", ict_getUserInfo, 0);
    pasSetTcpFunName("ictSrvUserGetPass", ictSrvUserGetPass, 0);
    pasSetTcpFunName("ict_getRecPackage", ict_getRecPackage, 0);
    return 0;
}

