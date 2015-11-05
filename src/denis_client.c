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

//#include "denis.h"
#include "soapStub.h"

#define  PAS_SRCFILE    7001

static char     caServicecode[32] = "pronetwayshaitest";
static char     caSn[16] = "PN10234454";
static int      iUpdateUserFlags = 2;       //  1--直接更新远程数据库   0--只更新本地    2--通过Pas更新
ncSrvSetUserStatus(utShmHead *psShmHead, uint4 lStype, char *pUserName, int iStatus);
int GetVipCardCallback(struct _tempuri__GetVipCard2Response *resp)
{

    //printf("resp is : %d , msg is : %s \n" , resp->GetVipCard2Result , resp->msg);
    //if (resp->GetVipCard2Result != NULL)
    {

    }
}

int RegisterCallback(struct _tempuri__VipRegisterInfoResponse *resp)
{

    printf("coming \n");
}

/*
int Denis_GetVipCard2(char *url,struct SOAP_ENV__Header *header,struct _tempuri__GetVipCard2 *in,int (*Callback)(struct _tempuri__GetVipCard2Response *resp))
{
 int status = -1;
 struct soap soap;
 struct _tempuri__GetVipCard2Response out;

 soap_init(&soap);
 soap_imode(&soap, SOAP_C_UTFSTRING);
 soap_omode(&soap, SOAP_C_UTFSTRING);

 soap.header = header;
 soap.version = 1;

 status = soap_call___tempuri__GetVipCard2(&soap,url,NULL,in,&out);
 //(*Callback)(&out);

 soap_destroy(&soap);
 soap_end(&soap);
 soap_done(&soap);
 int ret = out.GetVipCard2Result == NULL ? 0 : 1;
 return ret;
}
*/

/*
int Denis_Register(char *url,struct SOAP_ENV__Header *header,struct _tempuri__VipRegisterInfo *in,int (*Callback)(struct _tempuri__VipRegisterInfoResponse *resp))
{
 int status = -1;
 struct soap soap;
 struct _tempuri__VipRegisterInfoResponse out;

 soap_init(&soap);
 soap_imode(&soap, SOAP_C_UTFSTRING);
 soap_omode(&soap, SOAP_C_UTFSTRING);

 soap.header = header;
 soap.version = 1;

 status = soap_call___tempuri__VipRegisterInfo(&soap,url,NULL,in,&out);
 (*Callback)(&out);

 soap_destroy(&soap);
 soap_end(&soap);
 soap_done(&soap);

 return status;
}
*/
// 注册
int Denis_Register(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    printf("\n 进入Denis_Register \n");
    utMsgPrintMsg(psMsgHead);
    utPltDbHead *psDbHead = utPltInitDbHead();
    char ip[16] = {0};
    char port[8] = {0};
    char getarg[512] = {0};
    char tsid[512] = {0};
    char posturl[1024] = {0};
    char postarg[512] = {0};
    char user_name[32] = {0};
    char id_number[24] = {0};
    char mobile[24] = {0};
    char email[64] = {0};
    char caName[68];
    char caSex[12];
    // 获取注册信息
    utMsgGetSomeNVar(psMsgHead, 11,
                     "ip",  UT_TYPE_STRING,  sizeof(ip) - 1, ip,
                     "port",  UT_TYPE_STRING,  sizeof(8) - 1,    port,
                     "getarg",  UT_TYPE_STRING,  sizeof(getarg) - 1, getarg,
                     "tsid",  UT_TYPE_STRING,  sizeof(tsid) - 1, tsid,
                     "posturl",  UT_TYPE_STRING,  sizeof(posturl) - 1,   posturl,
                     "postarg", UT_TYPE_STRING,  sizeof(postarg) - 1, postarg,
                     "sex",     UT_TYPE_STRING, sizeof(caSex) - 1, caSex,
                     "user_name",  UT_TYPE_STRING,  sizeof(user_name) - 1,   user_name,
                     "id_number",  UT_TYPE_STRING,  sizeof(id_number) - 1,   id_number,
                     "mobile",     UT_TYPE_STRING,  sizeof(mobile) - 1,      mobile,
                     "email",      UT_TYPE_STRING,  sizeof(email) - 1,       email
                    );

    unsigned getShopId = ncSrvGetCid(psShmHead, psMsgHead);
    printf("pid=%d\n", getpid());
    //  sleep(15);



    char caTemp[4096] = {0};
    pasCvtGBK(2, user_name, caTemp, strlen(user_name) + 1);

    strcpy(caName, caTemp);

    pasCvtGBK(2, caSex, caTemp, strlen(caSex) + 1);
    strcpy(caSex, caTemp);

    printf("\n user_name = %s \n", caTemp);
    printf("\n id_number = %s \n", id_number);
    printf("\n mobile = %s \n", mobile);
    printf("\n email = %s \n", email);
    utMsgHead *psMsgHead2;
    long status = -1;
    char server_ip[100] = {0};
    char server_port[100] = {0};

    strcpy(server_ip,  utComGetVar_sd(psShmHead, "verify_ip",  ""));
    strcpy(server_port, utComGetVar_sd(psShmHead, "verify_port", ""));
    printf("\n server_ip = %s \n", server_ip);
    printf("\n server_port = %s \n", server_port);
    unsigned long request_ip = utComHostAddress(server_ip);
    unsigned long request_port = atol(server_port);
    printf("\n port1 = %d \n", request_port);
    request_port = ntohs(request_port);
    printf("\n port2 = %d \n", request_port);
    psMsgHead2 = NULL;
    if(strlen(user_name) > 0 && strlen(id_number) > 0 && strlen(mobile) > 0)
    {
        psMsgHead2 = pasTcpRequest(request_ip, request_port,
                                   0,        /* CheckSum */
                                   0,        /* Encrypt  */
                                   "DenisRegister",
                                   0,       /* Sid */
                                   0,       /* Rid  */
                                   30,      /* 超时  */
                                   NULL,    /* key   */
                                   4,
                                   "user_name", UT_TYPE_STRING, user_name,
                                   "id_number", UT_TYPE_STRING, id_number,
                                   "mobile", UT_TYPE_STRING, mobile,
                                   "email", UT_TYPE_STRING, email
                                  );
        printf("\n psMsgHead2 = %p \n", psMsgHead2);
        status = 1;
        if(psMsgHead2)
        {
            int iReturn = utMsgGetSomeNVar(psMsgHead2, 1,
                                           "status", UT_TYPE_LONG, 4, &status);

            utMsgFree(psMsgHead2);
        }
    }
    else
    {
        status = 0;
    }

    printf("\n status = %d \n", status);
    char caTsid[24] = {0};
    char userip[16] = {0};
    char ssid[32] = {0};
    char usermac[64] = {0};

    utMsgGetSomeNVar(psMsgHead, 1, "PASTSID", UT_TYPE_STRING, 20, caTsid);
    printf("\n caTsid = %s \n", caTsid);
    /*
    printf("\n psShmHead = %p\n", psShmHead);
    ncPortalOnline *online = (ncPortalOnline*)ncSrvGetOnlineUserByStrTsid(psShmHead, caTsid);
    printf("\n online = %p \n", online);
    if (online != NULL)
    {
        strcpy(userip, utComHostIp(online->lSip));
        strcpy(ssid, utComHostIp(online->caSsid));
        strcpy(usermac, pasCvtMac(online->caMac));
    }
    printf("\n lSip = %s \n", userip);
    printf("\n caSsid = %s \n", ssid);
    printf("\n caMac = %s \n", usermac);
    */
    //utPltPutVar(psDbHead, "tsid", caTsid);
    //utPltPutVar(psDbHead, "userip", userip);
    //utPltPutVar(psDbHead, "ssid", ssid);
    //utPltPutVar(psDbHead, "usermac", usermac);
    /*
    char caName0[32]= {0};
    char caPass[32]= {0};
    char caUserIp[32]= {0};
    char caLang[16] = {0};
    char caCode[8] = {0};
    utMsgGetSomeNVar(psMsgHead,5,
            "tsid",     UT_TYPE_STRING,20, caTsid,
            "lang",     UT_TYPE_STRING,15, caLang,
            "charset",  UT_TYPE_STRING,15, caCode,
            "userip",   UT_TYPE_STRING,24, caUserIp,
            "username", UT_TYPE_STRING,31, caName0);

    printf("\n tsid = %s \n", caTsid);
    printf("\n lang = %s \n", caLang);
    printf("\n charset = %s \n", caCode);
    printf("\n userip = %s \n", caUserIp);
    printf("\n username = %s \n", caName0);
    */



    utPltPutVar(psDbHead, "ip", ip);
    utPltPutVar(psDbHead, "port", port);
    utPltPutVar(psDbHead, "getarg", getarg);
    utPltPutVar(psDbHead, "tsid", tsid);
    utPltPutVar(psDbHead, "posturl", posturl);
    utPltPutVar(psDbHead, "postarg", postarg);
    /*
    SurfTheNet(psShmHead, iFd, psMsgHead);
    printf("\n 离开Denis_Register \n");
    return;
    */
    //utMsgGetSomeNVar(psMsgHead,1, "PASTSID",UT_TYPE_STRING,20,caTsid);
    //utPltOutToHtml(iFd,psMsgHead,psDbHead,"http://127.0.0.1:9080/pronline/genal/sms/login_main.htm");
    //utPltOutToHtml(iFd,psMsgHead,psDbHead,"http://192.168.20.188:9080/pronline/Msg?FunName@ncSrvDispLogin");
    if(status == 1)     // 注册成功跳到登录页
    {
        /*
        utPltPutVar(psDbHead, "status", "successful");
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"genal/sms/redirect.htm");
        */


        char caTemp[1024];
        long lCount = 0;
        sprintf(caTemp, "select count(*) from ncmshopuserlog where username='%s' and idno='%s' ", mobile, id_number);
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount == 0)
        {
            pasDbExecSqlF("insert into ncmshopuserlog(username,dispname,did,mobile,idno,email,stime,sex) values('%s','%s',%lu,'%s','%s','%s',%lu,'%s') ",
                          mobile, caName, getShopId, mobile, id_number, email, time(0), caSex);
        }



        printf("\n 注册成功 \n");
        //      SurfTheNet(psShmHead, iFd, psMsgHead);
        char caPass[32];
        long iReturn;
        iReturn = ncSrvSysDnsPass(psShmHead, iFd, psMsgHead, caPass);
        printf("iReturn=%d,pass=%s\n", iReturn, caPass);
        if(iReturn == 0)
        {
            utPltPutVar(psDbHead, "tsid",   caTsid);

            utPltPutVar(psDbHead, "status", "successful");
            //utPltPutVar(psDbHead, "tsid", tsid);
            //  utPltPutVar(psDbHead, "posturl", posturl);
            //  utPltPutVar(psDbHead, "postarg", postarg);
            utPltPutVar(psDbHead, "username", mobile);
            utPltPutVar(psDbHead, "password", caPass);
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "genal/sms/redirect.htm");
        }
        else
        {
            utPltPutVar(psDbHead, "status", "failed");
            //utPltOutToHtml(iFd,psMsgHead,psDbHead,"portal/sms/Register.html");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "genal/sms/redirect.htm");
        }

    }
    else    // 注册失败
    {
        printf("\n 注册失败 \n");
        utPltPutVar(psDbHead, "status", "failed");
        //utPltOutToHtml(iFd,psMsgHead,psDbHead,"portal/sms/Register.html");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "genal/sms/redirect.htm");
    }
    printf("\n 离开Denis_Register \n");
}

// 获取验证码
int ncSrvUserGetPass(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    //  utMsgPrintMsg(psMsgHead);
    //  printf("\n 进入ncSrvUserGetPass \n");
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
    //  printf("\n 手机号为 %s \n", username);

    //  printf("\n %s \n", postarg);
    //  printf("posturl=%s\n",caPosturl);
    //  printf("postarg=%s\n",postarg);

    /*
    int iRet = 0;
    struct _tempuri__GetVipCard2 pGetVipCardInput;
    struct SOAP_ENV__Header header;
    struct tempuri__CrmSoapHeader vheader;


    pGetVipCardInput.condType = (int)malloc(sizeof(int));
    pGetVipCardInput.condType = 3;
    pGetVipCardInput.condValue = (char*)malloc(sizeof(char)*20);
    pGetVipCardInput.condValue = username; //   "15900713390";//"11122223333";//"13213037793";
    pGetVipCardInput.cardCodeToCheck = "";
    pGetVipCardInput.verifyCode = "";
    pGetVipCardInput.storeCode = (char*)malloc(sizeof(char)*20);
    pGetVipCardInput.storeCode = "8001";

    vheader.UserId = "BFCRM";
    vheader.Password = "BFCRM";
    vheader.__anyAttribute = NULL;

    header.tempuri__CrmSoapHeader = &vheader;
    */
    //sleep(10);
    //iRet = Denis_GetVipCard("http://dennisapp.chinacloudapp.cn:82/PosWebService.asmx" , &pGetVipCardInput , GetVipCardCallback);
    //iRet = Denis_GetVipCard2(NULL,&header,&pGetVipCardInput,GetVipCardCallback);
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
        printf("lStatus=%d,username=%s,stype=%d \n", lStatus, username, psOnline->lStype);
    }
    else
    {

        lStatus = ncSrvGetUserStatus(psShmHead, 4100021, username);
        printf("_1lStatus=%d,username=%s,stype=%d \n", lStatus, username, 0);
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


    //printf("\n server_ip = %s \n", server_ip);
    //printf("\n server_port = %s \n", server_port);
    unsigned long ip = utComHostAddress(server_ip);
    unsigned long port = atol(server_port);
    //printf("\n port1 = %d \n", port);
    port = ntohs(port);
    //printf("\n port2 = %d \n", port);
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

    //printf("\n status = %d \n", status);
    /*
    ncSrvGetPass(psShmHead, iFd, psMsgHead);
    return;
    */
    if(status == 1)     // 会员直接获取验证码
    {
        printf("\n 会员 \n");
        ncSrvGetPass(psShmHead, iFd, psMsgHead);
    }
    else    // 非会员则跳到注册页
    {
        printf("\n 非会员 \n");
        utPltDbHead *psDbHead = utPltInitDbHead();
        utPltPutVar(psDbHead, "status", "forbidden");
        utPltPutVar(psDbHead, "posturl", caPosturl);
        utPltPutVar(psDbHead, "postarg", postarg);
        utPltPutVar(psDbHead, "ip", caIp);
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "genal/sms/redirect2.htm");
        //utPltOutToHtml(iFd,psMsgHead,psDbHead,"genal/sms/Register.html");
    }
    //  printf("\n 离开ncSrvUserGetPass \n");
}

// 后台生成验证码，并准备上网。
int SurfTheNet(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    //  printf("\n 进入SurfTheNet \n");
    char caSsid[32] = {0};
    char caName[32] = {0};
    char caName0[32] = {0};
    char caPass[32] = {0};
    char caUserIp[32] = {0};
    char caUserMac[32] = {0};
    char caAcName[32] = {0};
    char caChapPass[32] = {0};
    char caChallenge[32] = {0};
    char caSmsName0[32] = {0};
    char caGroupCode[32] = {0};
    char caSpname[16] = {0};
    char caApMac[32] = {0};
    char caUrl[256] = {0};
    char caMsg[4096] = {0};
    char caLang[16] = {0}, caCode[8] = {0};
    char caTsid[24] = {0};
    char caArea[32] = {0};
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
    strcpy(caName, "\0");
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
    // xky start
    /*
    iReturn = utMsgGetSomeNVar(psMsgHead,5,
            "PASTSID",     UT_TYPE_STRING,20, caTsid,
            "lang",     UT_TYPE_STRING,15, caLang,
            "charset",  UT_TYPE_STRING,15, caCode,
            "userip",   UT_TYPE_STRING,24, caUserIp,
            "user_name", UT_TYPE_STRING,31, caName0);
    */
    char getarg[512] = {0};
    char tsid[512] = {0};
    char posturl[512] = {0};
    char postarg[512] = {0};
    char mobile[24] = {0};
    iReturn = utMsgGetSomeNVar(psMsgHead, 9,
                               "PASTSID",     UT_TYPE_STRING, 20, caTsid,
                               "tsid",       UT_TYPE_STRING, sizeof(tsid) - 1, tsid,
                               "lang",     UT_TYPE_STRING, 15, caLang,
                               "charset",  UT_TYPE_STRING, 15, caCode,
                               "getarg",   UT_TYPE_STRING, 24, getarg,
                               "posturl",  UT_TYPE_STRING, sizeof(posturl) - 1, posturl,
                               "postarg",  UT_TYPE_STRING, sizeof(postarg) - 1, postarg,
                               "user_name", UT_TYPE_STRING, 31, caName0,
                               "mobile",   UT_TYPE_STRING, sizeof(mobile) - 1, mobile);
    char *p = NULL, *q = NULL;
    p = strchr(getarg, '@');
    q = strchr(getarg, '&');
    strncpy(caUserIp, p + 1, q - p - 1);
    strcpy(caName, mobile);
    // xky end
    //  printf("\n caTsid = %s \n", caTsid);
    //  printf("\n tsid = %s \n", tsid);
    //  printf("\n lang = %s \n", caLang);
    //  printf("\n charset = %s \n", caCode);
    //  printf("\n userip = %s \n", caUserIp);
    //  printf("\n getarg = %s \n", getarg);
    //  printf("\n posturl = %s \n", posturl);
    //  printf("\n postarg = %s \n", postarg);
    //  printf("\n username = %s \n", caName0);
    //  printf("\n mobile = %s \n", mobile);

    if(!utStrIsSpaces(caTsid))
    {
        sscanf(caTsid, "%llu", &llTsid);
        psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
        //      printf("\n psOnline = %p \n", psOnline);
        if(psOnline)
        {
            lGroupid = psOnline->lGroupid;
        }
        else
        {
            lGroupid = 0;
        }

    }
    //
    //  printf("\n lGroupid = %d \n", lGroupid);
    //  printf("\n psOnline = %p \n", psOnline);
    if(psOnline == NULL)
    {
        char *pHref, *p;
        pHref = utMsgGetVar_s(psMsgHead, "httpref");
        if(pHref)
        {
            char caOut[512];
            pasStrUrlUnEsc(pHref, caOut);
            //
            //      printf("\n caOut = %s \n", caOut);
            p = strstr(caOut, "switch_url=");
            if(p)    // Cisco AP
            {
                // https://r389.proeim.cn/pronline/Msg?FunName@ncSrvDispLogin?switch_url=https://applechinawifi.apple.com/login.html&ap_mac=fc:fb:fb:d8:26:70&client_mac=3c:ab:8e:5a:4f:cc&wlan=Apple%20Store&redirect=www.airport.us/pcK3qszyU/pwaX9tvIi.html
                //
                //              printf("\n caOut contains switch_url \n");
                ncUtlGetWordBetween(caOut, "ap_mac=", "&", caNasid, 31);
                ncUtlGetWordBetween(caOut, "wlan=", "&", caSsid, 31);
                ncUtlGetWordBetween(caOut, "client_mac=", "&", caUserMac, 31);
                iVendor = NCPORTAL_VENDOR_CISCO;
            }
            if(iVendor == 0 && (p = strstr(caOut, "wlanacname=")))
            {
                // [https://192.168.21.102/pronline/Msg?FunName@ncSrvDispLogin?wlanuserip=10.1.1.6&wlanacname=0000.0000.000.00.460&ssid=apple-https&nasid=0000.0000.000.00.460&usermac=E4-CE-8F-E1-AB-37]
                //               printf("\n wlanacname= \n");
                iVendor = NCPORTAL_VENDOR_GBCOM;
                ncUtlGetWordBetween(caOut, "wlanacname=", "&", caNasid, 31);
                ncUtlGetWordBetween(caOut, "ssid=", "&", caSsid, 31);
                ncUtlGetWordBetween(caOut, "usermac=", "&", caUserMac, 31);
                ncUtlGetWordBetween(caOut, "wlanuserip=", "&", caUserIp, 15);
            }
            if(iVendor == 0 && (p = strstr(caOut, "from=gbap")))
            {
                // [http://171.8.150.218:9080/pronline/Msg?FunName@ncSrvPlate&lang@&code@&usernametmp@&html@/alogin_main.htm&from=gbap&ssid=&gw_address=192.168.20.1&gw_port=8060&gw_id=ZZDXceshi&ip=192.168.20.110&mac=3c:43:8e:d4:53:92&url=http://m.baidu.com/s]
                //              printf("\n from=gbap \n");
                iVendor = NCPORTAL_VENDOR_GBAP;
                ncUtlGetWordBetween(caOut, "gw_id=", "&", caNasid, 31);
                strcpy(caSsid, "\0");
                ncUtlGetWordBetween(caOut, "&mac=", "&", caUserMac, 31);
                ncUtlGetWordBetween(caOut, "&ip=", "&", caUserIp, 15);
            }
            if(iVendor == 0 && (p = strstr(caOut, "from=WIFIDOG")))
            {
                // [http://171.8.150.218:9080/pronline/Msg?FunName@ncSrvPlate&lang@&code@&usernametmp@&html@/alogin_main.htm&from=gbap&ssid=&gw_address=192.168.20.1&gw_port=8060&gw_id=ZZDXceshi&ip=192.168.20.110&mac=3c:43:8e:d4:53:92&url=http://m.baidu.com/s]
                printf("\n from=WIFIDOG \n");
                iVendor = NCPORTAL_VENDOR_WIFIDOG;
                ncUtlGetWordBetween(caOut, "gw_id=", "&", caNasid, 31);
                strcpy(caSsid, "\0");
                ncUtlGetWordBetween(caOut, "&mac=", "&", caUserMac, 31);
                ncUtlGetWordBetween(caOut, "&ip=", "&", caUserIp, 15);
            }
            if(iVendor == 0 && (p = strstr(caOut, "&tsid=")))
            {
                // [http://171.8.150.218:9080/pronline/Msg?FunName@ncSrvPlate&lang@&code@&usernametmp@&html@/alogin_main.htm&from=gbap&ssid=&gw_address=192.168.20.1&gw_port=8060&gw_id=ZZDXceshi&ip=192.168.20.110&mac=3c:43:8e:d4:53:92&url=http://m.baidu.com/s]
                //               printf("\n &tsid= \n");
                ncUtlGetWordBetween(caOut, "&tsid=", "&", caTsid, 31);
                sscanf(caTsid, "%llu", &llTsid);
                psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
                if(psOnline)
                {
                    //                  printf("\n psOnline != NULL \n");
                    lGroupid = psOnline->lGroupid;
                }
            }
            if(iVendor > 0 && psOnline == NULL)
            {
                //              printf("\n iVendor > 0 && psOnline == NULL \n");
                lSip = ntohl(pasIpcvtLong(caUserIp));
                psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIp(psShmHead, caNasid, lSip);
            }
        }
    }


    /*
    if(psOnline && psOnline->psAp && psOnline->psAp->psGroup) {  // 短信计费单位
        strcpy(caSmsName,psOnline->psAp->psGroup->caSmsName);
    }
    else {
        strcpy(caSmsName,utComGetVar_sd(psShmHead,"MySmsName","None"));
    }
    */

    /*  手机号码的有效性检查
        +86-1234567
      */
    // xky start
    /*  暂时不验证手机号
    iAreaCode =  ncPortalCheckMobileNumber(caName0,mobile,caArea,caSpname,&iArea,&iSp);
    printf("\n iAreaCode = %u \n", iAreaCode);
    printf("\n caName = %s \n", caName);
    printf("\n caArea = %s \n", caArea);
    */
    // xky end
    /*
    if(iAreaCode <= 0) {
        pasLogs(PAS_SRCFILE,1008," Username: %s Number Error AreaCode=%d",caName0,iAreaCode);
        psDbHead = utPltInitDbHead();
        utPltPutVar(psDbHead,"statusId",   "1");
        utPltPutVar(psDbHead,"statusCode", "5");
        utPltPutVar(psDbHead,"message", "Invalid Username");
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"login_getpass_return.htm");
        return 0;
    }
    */
    pasLogs(PAS_SRCFILE, 1008, " Check MobileNumber: AreaCode=%u  Name:%s  Area:%s ", iAreaCode, caName, caArea);
    // 从if-else判断中移到此处
    psDbHead = utPltInitDbHead();
    // 检查黑名单
    printf("\n 开始检查黑名单 \n");
    if(psOnline)
    {
        printf("\n psOnline != NULL \n");
        strcpy(caApMac, psOnline->caApName);
        lStype = psOnline->lStype;
        if(ncSrvPortalIsBlackUser(psShmHead, psOnline->lStype, NCPORTAL_BLACK_USER, caName))
        {
            printf("\n Username is in Black \n");
            pasLogs(PAS_SRCFILE, 1008, " Username: %s is in Black", caName);
            //psDbHead = utPltInitDbHead();
            //utPltPutVar(psDbHead,"statusId",   "3");
            //utPltPutVar(psDbHead,"statusCode", "3");
            //utPltPutVar(psDbHead,"message", " Username is in black");
            //utPltOutToHtml(iFd,psMsgHead,psDbHead,"login_getpass_return.htm");
            utPltPutVar(psDbHead, "status", "user_black");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "genal/sms/redirect.htm");
            //return 0;
        }
        if(ncSrvPortalIsBlackUser(psShmHead, psOnline->lStype, NCPORTAL_BLACK_MAC, psOnline->caMac))
        {
            printf("\n User Mac is in Black \n");
            pasLogs(PAS_SRCFILE, 1008, " Username: %s Mac %s is in Black", caName, pasCvtMac(psOnline->caMac));
            //psDbHead = utPltInitDbHead();
            //utPltPutVar(psDbHead,"statusId",   "3");
            //utPltPutVar(psDbHead,"statusCode", "3");
            //utPltPutVar(psDbHead,"message", " Username is in black");
            //utPltOutToHtml(iFd,psMsgHead,psDbHead,"login_getpass_return.htm");
            utPltPutVar(psDbHead, "status", "mac_black");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "genal/sms/redirect.htm");
            //return 0;
        }

        if(utStrIsSpaces(caUserMac))
        {
            printf("\n caUserMac is space \n");
            strcpy(caUserMac, pasCvtMac(psOnline->caMac));
        }
        printf("\n caUserMac = %s \n", caUserMac);
    }
    else
    {
        printf("\n psOnline == NULL \n");
        if(ncSrvPortalIsBlackUser(psShmHead, 0, NCPORTAL_BLACK_USER, caName))
        {
            printf("\n Username is in Black \n");
            pasLogs(PAS_SRCFILE, 1008, " Username: %s is in Black", caName);
            //psDbHead = utPltInitDbHead();
            //utPltPutVar(psDbHead,"statusId",   "3");
            //utPltPutVar(psDbHead,"statusCode", "3");
            //utPltPutVar(psDbHead,"message", " Username is in black");
            //utPltOutToHtml(iFd,psMsgHead,psDbHead,"login_getpass_return.htm");
            utPltPutVar(psDbHead, "status", "user_black");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "genal/sms/redirect.htm");
            //return 0;
        }
    }
    printf("\n 完成检查黑名单 \n");
    srand(time(0));
    lCode = rand() % 999999L;
    sprintf(caPass, "%06lu", lCode);
    caPass[6] = 0;

    /*
    if(iAreaCode == 86) { // 国内短信
        nCurNum = psSumm->nCurSms;
        iReSend = 0;  // 失败重发次数
        while(iReSend < 3) {
            lDip = htonl(psSumm->psSmsServer[nCurNum].lIp);
            nDport = htons(psSumm->psSmsServer[nCurNum].nPort);
            if(psSumm->psSmsServer[nCurNum].nType == NCSRV_SMSTYPE_PRONETWAY) {
                ncSrvGetSmsMsg(psShmHead,psOnline,caLang,caMsg);
                utStrReplaceWith(caMsg,"[#username#]",caName);
                utStrReplaceWith(caMsg,"[#password#]",caPass);
                sprintf(caSmsName0,"%s",psSumm->psSmsServer[nCurNum].caSrvName);
                psMsgHead0 = pasTcpRequest(lDip,nDport,
                         0,
                         0,
                         "ncsSendMobileMesg_cy",
                         0,
                         0,
                         30,
                         NULL,
                         5,
                    "servicecode",UT_TYPE_STRING,psSumm->caPortalName,
                    "SN",          UT_TYPE_STRING,psSumm->caProdSn,
                    "clienttype",  UT_TYPE_STRING,caSmsName,
                    "mobno",UT_TYPE_STRING,caName,
                    "mesg", UT_TYPE_STRING,caMsg);
                pasLogs(PAS_SRCFILE,1008,"Send ncsSendMobileMesg_cy %s:%u  ServiceCode:%s  SN:%s  ClientType:%s Name:%s Message:[%s] iRet=%d",
                                       utComHostIp(lDip),ntohs(nDport),psSumm->caPortalName,psSumm->caProdSn,caSmsName,caName,caMsg,iReturn);
                if(psMsgHead0) {
                    utMsgOutMsgToLog(PAS_SRCFILE,1008,psMsgHead0,"[ncsSendMobileMesg_cy] \n");
                    iReturn = utMsgGetVar_ld(psMsgHead0,"status",0);
                    // 短信服务器返回值   -999 网络不通   0 成功  -1 不成功  -888 禁用
                    utMsgFree(psMsgHead0);
                    if(iReturn == 0) {
                        psSumm->psSmsServer[nCurNum].cStatus = 1;
                        psSumm->psSmsServer[nCurNum].lLastTime = time(0);
                        psSumm->nCurSms = nCurNum;
                        lStatus = 1;
                        break;
                    }
                    else if(iReturn == (-999)) {
                        iReSend ++;
                        if(nCurNum < psSumm->nSumSms - 1) {
                            nCurNum ++;
                        }
                        else {
                            nCurNum = 0;
                        }
                    }
                    else {
                        break;
                    }
                }
                else {
                    iReSend ++;
                    if(nCurNum < psSumm->nSumSms - 1) {
                        nCurNum ++;
                    }
                    else {
                        nCurNum = 0;
                    }
                    lStatus = 0;
                }
            }
            else if(psSumm->psSmsServer[nCurNum].nType == NCSRV_SMSTYPE_PEOPLE) {
                char *p;
                char caPre[128],caSuff[128];
                ncSrvGetSmsMsg(psShmHead,psOnline,caLang,caMsg);
                utStrReplaceWith(caMsg,"[#username#]",caName);
                p = strstr(caMsg,"[#password#]");
                if(p) {
                    *p = 0;
                    strcpy(caPre,caMsg);
                    strcpy(caSuff,p+12);
                }
                else {
                    strcpy(caPre,caMsg);
                    strcpy(caSuff,"\0");
                }
                pasLogs(PAS_SRCFILE,1008,"ncSrvSendSmsByPeople Name:%s Pre:%s Suff:%s",caName,caPre,caSuff);
                iRet = ncSrvSendSmsByPeople(psShmHead,caName,caPre,caSuff);
                break;
            }
            else {
                if(utStrIsMobileNumber(caName)) {
                    ncSrvGetSmsMsg(psShmHead,psOnline,caLang,caMsg);
                    utStrReplaceWith(caMsg,"[#username#]",caName);
                    utStrReplaceWith(caMsg,"[#password#]",caPass);
                    sprintf(caSmsName0,"%s",psSumm->psSmsServer[nCurNum].caSrvName);
                    iRet = ncUtlSendSmsByModem(caName,caMsg);
                    pasLogs(PAS_SRCFILE,1008,"Send ncUtlSendSmsByModem Name:%s  Message:[%s] iRet=%d",
                        caName,caMsg,iRet);
                    lStatus = iRet;
                    if(iRet < 0) {
                        iReSend ++;
                    }
                    else {
                        psSumm->psSmsServer[nCurNum].cStatus = 1;
                        psSumm->psSmsServer[nCurNum].lLastTime = time(0);
                        psSumm->nCurSms = nCurNum;
                        break;
                    }
                }
            }
        }
    }
    else {  // 国际短信
        nCurNum = psSumm->nCurGjSms;
        iReSend = 0;  // 失败重发次数
        while(iReSend < 3) {
            lDip = htonl(psSumm->psSmsGjServer[nCurNum].lIp);
            nDport = htons(psSumm->psSmsGjServer[nCurNum].nPort);
            if(psSumm->psSmsGjServer[nCurNum].nType == NCSRV_SMSTYPE_PRONETWAY) {
                ncSrvGetSmsMsg(psShmHead,psOnline,caLang,caMsg);
                utStrReplaceWith(caMsg,"[#username#]",caName);
                utStrReplaceWith(caMsg,"[#password#]",caPass);
                sprintf(caSmsName0,"%s",psSumm->psSmsGjServer[nCurNum].caSrvName);
                psMsgHead0 = pasTcpRequest(lDip,nDport,
                         0,
                         0,
                         "ncsSendMMesg_sms",
                         0,
                         0,
                         30,
                         NULL,
                         5,
                    "servicecode",UT_TYPE_STRING,psSumm->caPortalName,
                    "SN",          UT_TYPE_STRING,psSumm->caProdSn,
                    "clienttype",  UT_TYPE_STRING,caSmsName,
                    "mobno",UT_TYPE_STRING,caName,
                    "mesg", UT_TYPE_STRING,caMsg);
                pasLogs(PAS_SRCFILE,1008,"Send ncsSendMMesg_sms %s:%u  ServiceCode:%s  SN:%s  ClientType:%s Name:%s Message:[%s] iRet=%d",
                                       utComHostIp(lDip),ntohs(nDport),psSumm->caPortalName,psSumm->caProdSn,caSmsName,caName,caMsg,iReturn);
                if(psMsgHead0) {
                    utMsgOutMsgToLog(PAS_SRCFILE,1008,psMsgHead0,"[ncsSendMobileMesg_cy] \n");
                    iReturn = utMsgGetVar_ld(psMsgHead0,"status",0);
                    // 短信服务器返回值   -999 网络不通   0 成功  -1 不成功  -888 禁用
                    utMsgFree(psMsgHead0);
                    if(iReturn == 0) {
                        psSumm->psSmsGjServer[nCurNum].cStatus = 1;
                        psSumm->psSmsGjServer[nCurNum].lLastTime = time(0);
                        psSumm->nCurGjSms = nCurNum;
                        lStatus = 1;
                        break;
                    }
                    else if(iReturn == (-999)) {
                        iReSend ++;
                        if(nCurNum < psSumm->nSumGjSms - 1) {
                            nCurNum ++;
                        }
                        else {
                            nCurNum = 0;
                        }
                    }
                    else {
                        break;
                    }
                }
                else {
                    iReSend ++;
                    if(nCurNum < psSumm->nSumGjSms - 1) {
                        nCurNum ++;
                    }
                    else {
                        nCurNum = 0;
                    }
                    lStatus = 0;
                }
            }
            else if(psSumm->psSmsGjServer[nCurNum].nType == NCSRV_SMSTYPE_PEOPLE) {
                char *p;
                char caPre[128],caSuff[128];
                ncSrvGetSmsMsg(psShmHead,psOnline,caLang,caMsg);
                utStrReplaceWith(caMsg,"[#username#]",caName);
                p = strstr(caMsg,"[#password#]");
                if(p) {
                    *p = 0;
                    strcpy(caPre,caMsg);
                    strcpy(caSuff,p+12);
                }
                else {
                    strcpy(caPre,caMsg);
                    strcpy(caSuff,"\0");
                }
                iRet = ncSrvSendSmsByPeople(psShmHead,caName,caPre,caSuff);
                break;
            }
            else {
                if(utStrIsMobileNumber(caName)) {
                    ncSrvGetSmsMsg(psShmHead,psOnline,caLang,caMsg);
                    utStrReplaceWith(caMsg,"[#username#]",caName);
                    utStrReplaceWith(caMsg,"[#password#]",caPass);
                    sprintf(caSmsName,"%s.%s",caSmsName,psSumm->psSmsGjServer[nCurNum].caSrvName);
                    iRet = ncUtlSendSmsByModem(caName,caMsg);
                    pasLogs(PAS_SRCFILE,1008,"Send ncUtlSendSmsByModem Name:%s  Message:[%s] iRet=%d",
                        caName,caMsg,iRet);
                    lStatus = iRet;
                    psSumm->psSmsGjServer[nCurNum].cStatus = 1;
                    psSumm->psSmsGjServer[nCurNum].lLastTime = time(0);
                    psSumm->nCurSms = nCurNum;
                }
            }
        }
    }
    */

    //psDbHead = utPltInitDbHead();
    utPltPutVar(psDbHead, "tsid",   caTsid);
    utPltPutVar(psDbHead, "userip", caUserIp);
    utPltPutVar(psDbHead, "username", caName);
    strcpy(caGroupCode, "\0");
    lGroupid = 0;
    printf("\n at the bottom \n");
    printf("\n iUpdateUserFlags = %d \n", iUpdateUserFlags);
    if(psOnline)
    {
        printf("\n psOnline != NULL \n");
        psOnline->psUser = ncSrvSynUserPassword(psShmHead, psOnline->lStype, psOnline->lGroupid, caName, caPass, psOnline->caGroupCode, psOnline->caSsid, NCSRV_USERTYPE_SMS);
        if(iUpdateUserFlags == 2)
        {
            printf("\n iUpdateUserFlags == 2 \n");
            printf("\n psOnline->psPar = %p \n", psOnline->psPar);
            printf("\n caName = %s \n", caName);
            printf("\n caPass = %s \n", caPass);
            printf("\n psOnline->lStype = %d \n", psOnline->lStype);
            int ret = ncSrvSynUserPassword2RadSrv(psOnline->psPar, caName, caPass, psOnline->lStype);
            printf("\n ret = %d \n", ret);
            pasLogs(PAS_SRCFILE, 1008, " Update User [%s]  Pass:[%s] Group:%s Ssid:%s  Stype:%u Ret=%d ",
                    caName, caPass, psOnline->caGroupCode, psOnline->caSsid, psOnline->lStype, iReturn);
        }
        strcpy(caGroupCode, psOnline->caGroupCode);
        if(utStrIsSpaces(caUserIp))
        {
            printf("\n caUserIp is space \n");
            strcpy(caUserIp, utComHostIp(htonl(psOnline->lSip)));
        }
    }
    else
    {
        printf("\n psOnline == NULL \n");
        ncSrvSynUserPassword(psShmHead, 0, 0, caName, caPass, "\0", utComGetVar_sd(psShmHead, "DefSsid", "\0"), NCSRV_USERTYPE_SMS);
        if(iUpdateUserFlags == 2)
        {
            printf("\n iUpdateUserFlags == 2 \n");
            printf("\n psSumm = %p \n", psSumm->psPar);
            printf("\n caName = %s \n", caName);
            printf("\n caPass = %s \n", caPass);
            int ret = ncSrvSynUserPassword2RadSrv(psSumm->psPar, caName, caPass, 0);
            printf("\n ret = %d \n", ret);
            pasLogs(PAS_SRCFILE, 1008, " Update User [%s]  Pass:[%s] iReturn=%d ",
                    caName, caPass, iReturn);
        }
    }

    /*
    sprintf(caHtml,"<statusCode>%s</statusCode><statusId>%s</statusId><message>%s</message>\r\n",
                    "0","1","Invalid Username");
    utComTcpResponse(iFd,psMsgHead,1,
                       "html",UT_TYPE_STRING,caHtml);

    utPltPutVar(psDbHead,"statusId",   "1");
    utPltPutVar(psDbHead,"statusCode", "0");
    utPltPutVar(psDbHead,"message", "Get Password Sucess");
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"login_getpass_return.htm");

    if(!utStrIsSpaces(caGroupCode)) {
        memcpy(caStype,caGroupCode+8,2);
        caStype[2] = 0;
    }
    else {
        caStype[0] = 0;
    }
    */

    utPltPutVar(psDbHead, "status", "successful");
    //utPltPutVar(psDbHead, "tsid", tsid);
    utPltPutVar(psDbHead, "posturl", posturl);
    utPltPutVar(psDbHead, "postarg", postarg);
    utPltPutVar(psDbHead, "username", caName);
    utPltPutVar(psDbHead, "password", caPass);
    printf("\n 已输出successful \n");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "genal/sms/redirect.htm");

    /*
    // 输出到日志文件
    pasUtlOut2DbFile("ncsrvsmslog",22,
       "sid",       UT_TYPE_LONG8,  lSid,                            // 自动增加
       "servicecode",UT_TYPE_STRING,caServicecode,
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
       "contrycode",UT_TYPE_ULONG,   iAreaCode,
       "status",    UT_TYPE_ULONG,  lStatus,
       "groupid",   UT_TYPE_ULONG,  lGroupid,
       "tsid",      UT_TYPE_LONG8,  llTsid,
       "groupcode", UT_TYPE_STRING, caGroupCode,
       "stype",     UT_TYPE_STRING, caStype,
       "utype",     UT_TYPE_ULONG,  lStype,
       "feename",   UT_TYPE_STRING, caSmsName,
       "apmac",     UT_TYPE_STRING, caApMac);
    return 0;
    */
    printf("\n 离开SurfTheNet \n");
}
