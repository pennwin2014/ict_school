/*    Portal认证
      最后修改时间  2014/05/11   By Liyunming
      主要针对Portal前端登录的处理
      是
 */
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utoall.h"
#include "pasutl.h"
#include "pasdb.h"
#include "utoplt01.h"
#include "ncportal.h"

#define  PAS_SRCFILE    7001

int ncSrcSendToAcRequestChallenge(char *pIp, uint2 nPort, uint2 nSn, char *caUserIp, uint2 *nReqId, char *pChallenge);
int ncSrcSendToAcRequesChapAuth(char *pIp, uint2 nPort, uint2 nSn, uint2 nReqId, char *caUserIp, char *pUser, char *caChallenge, char *pPass, char *pChapPass);


static int      iAutoUpdateRadius = 1;
static int2     lReqIdNum = 1;
static char     caServicecode[32] = "pronetwayshaitest";
static char     caSn[16] = "PN10234454";
static char     caAdminIp[32] = "192.168.20.38";
static char     caAdminPort[16] = "9080";
static int      iPapChap = 0;   /* 0--Chap  1-PAP  */
static char     caClientType[32] = "Pronetway";
static char     caSmsType[32] = "Pronetway";
static char     caSmsIp[32] = "61.129.34.70";
static uint2    nSmsPort = 7195;
static int      iUpdateUserFlags = 0;       //  1--直接更新远程数据库   0--只更新本地    2--通过Pas更新
static int      iAutoAuthIfExistUser = 0;   //  当用户存在时，可以自动认证
static int      iNasNameFrom = 0;           // NasId的来源  0--不变 1--FixName  2--ACName  3--APName
static uchar    caGNasName[32];             // NasName的来源

int ncSrvSetNasNameFrom(int iFlags, char *pName)
{
    iNasNameFrom = iFlags;
    if(pName)
    {
        strcpy(caGNasName, pName);
    }
    return 0;
}
int ncSrvSetUpdateUserBy(int iFLags)
{
    iUpdateUserFlags = iFLags;
    return 0;
}

int ncSrvGetUpdateUserBy()
{
    return iUpdateUserFlags;
}

int ncSrvSetAutoAuthExist(int iFlags)
{
    iAutoAuthIfExistUser = iFlags;
    return 0;

}


int ncSrvSetAdminServer(char *pIp, char *pPort)
{
    strcpy(caAdminIp, pIp);
    strcpy(caAdminPort, pPort);
    return 0;
}

int ncSrvSetSmsIp(char *pClientType, char *pType, char *pIp, uint2 nPort)
{
    strcpy(caClientType, pClientType);
    strcpy(caSmsType, pType);
    strcpy(caSmsIp, pIp);
    nSmsPort = nPort;
    return 0;
}

int ncSrvSetAuthSynRadius(int iFlags)
{
    iAutoUpdateRadius = iFlags;
}

int ncSrvSetChapFlags(int iFlags)
{
    iPapChap = iFlags;
    return 0;
}

int ncSrvGetChapFlags()
{
    return iPapChap;
}

int ncSrvDispLogin(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    return ncSrvDispLogin0(psShmHead, iFd, psMsgHead, 0);
}

int ncSrvPlate(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    return ncSrvDispLogin0(psShmHead, iFd, psMsgHead, 1);
}

// 显示认证界面
int ncSrvDispLogin0(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead, int iFlags)
{
    ncPortalSsid *psSsid;
    char caNasid[32];
    char caSsid[32];
    char caUserIp[32];
    char caUserMac[32], caUserMac0[32];
    char caUserAgent[256];
    char caClientIp[32];
    char caPostUrl[255];
    char caMsg[512];
    char caRedir[256];
    char caPostArg[512];
    char caGetArg[512];
    char caApMac[64];
    char caStatus[8];
    char *pAcIp;
    char mac[6];
    char caName[32], caIp[32], caPass[32], caAcName[32];
    char caHtml[128], caFunName[128], caUp[64];
    char caUsernameTemp[18];
    char caAreaCode[16];
    char caGwid[32], caGwip[32], caGwport[8];
    ncPortalOnline *psOnline = NULL;
    char caTerm[16], caOs[16], caBro[16], caType[16], caLang[16], caCode[16], caSrvIp[32], caSrvPort[10], caTsid[24];
    char *pPlate;
    char *pFrom;
    char caPlate[128];
    uint2 nPort = 2000;
    uint4 lSip;
    uint8 llTsid;
    char *p;
    int iReturn, iRet;
    utPltDbHead *psDbHead = NULL;
    utShmHand sShm;
    int iSum, iVendor = 0;
    uint4 lLastTime = 0;


    //printf("pid=%d\n",getpid());
    //sleep(15);
    utMsgOutMsgToLog(PAS_SRCFILE, 1008, psMsgHead, "[ncSrvDispLogin] \n");
    strcpy(caRedir, "\0");
    strcpy(caSsid, "\0");
    strcpy(caUserIp, "\0");
    strcpy(caClientIp, "\0");
    strcpy(caUserMac, "\0");
    strcpy(caLang, "\0");
    strcpy(caUserAgent, "\0");
    strcpy(caStatus, "\0");
    strcpy(caApMac, "\0");
    strcpy(caAcName, "\0");
    strcpy(caPostUrl, "\0");
    strcpy(caPostArg, "\0");
    strcpy(caGetArg, "\0");
    strcpy(caGwip, "\0");
    strcpy(caGwport, "\0");
    strcpy(caMsg, "\0");
    strcpy(caUserMac0, "\0");
    memset(caGwid, 0, 32);
    memset(caNasid, 0, 32);
    memset(caTsid, 0, 24);
    iVendor = 0;
    if(pasLogsIsOut(PAS_SRCFILE, 1089, NULL))
    {
        printf(" ncSrvDispLogin Pid==%d\n", getpid());
        sleep(10);
    }
    psOnline = NULL;
    // utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 6,
                               "tsid",        UT_TYPE_STRING, 31,   caTsid,
                               "lang",        UT_TYPE_STRING, 15,   caLang,
                               "usragent",    UT_TYPE_STRING, 255, caUserAgent,
                               "srvname",     UT_TYPE_STRING, 31, caSrvIp,
                               "srvport",     UT_TYPE_STRING, 7,  caSrvPort,
                               "clientip",    UT_TYPE_STRING, 15, caClientIp);
    if(!utStrIsSpaces(caTsid))
    {
        sscanf(caTsid, "%llu", &llTsid);
        psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
        if(psOnline)
        {
            iVendor = psOnline->cFrom;
            sprintf(caGetArg, "tsid@%s", caTsid);
            sprintf(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "tsid", caTsid);
            if(!utStrIsSpaces(caLang))
            {
                utStrAddF(caGetArg, "&lang@%s", caLang);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang",   caLang);
            }
            if(iVendor == NCPORTAL_VENDOR_ARUBA || iVendor == NCPORTAL_VENDOR_MOTO || iVendor == NCPORTAL_VENDOR_CISCO)
            {
                sprintf(caPostUrl, psOnline->caPostUrl);
            }
            else
            {
                if(strcmp(caSrvPort, "443") == 0)
                {
                    sprintf(caPostUrl, "https://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
                }
                else
                {
                    sprintf(caPostUrl, "http://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
                }
            }
            strcpy(caUserMac, pasCvtMac(psOnline->caMac));
            memcpy(mac, psOnline->caMac, 6);
            utStrncpy(caSsid, psOnline->caSsid, 31);
            if(utStrIsSpaces(caLang))
            {
                strcpy(caLang, psOnline->caLang);
            }
        }
    }
    else
    {
        pFrom = utMsgGetVar_s(psMsgHead, "PFrom");
        if(pFrom)
        {
            if(strncasecmp(pFrom, "CiscoV1", 7) == 0) //  Cisco
            {
                /*
                 VarName   Bytes  Type      Content
                login_url  292STRING  [https://n113.network-auth.com/splash/login?mauth=MMYenrF6x9b0A8vHDerEXVICb1W3gxj9RrduBSlt9KkmD5_Z8sz2wg8e_aUh0ozFwQXEEUwIBEF9pGp40d13gZGmwAF0ZBk819aLm6Upk_FN4YPoRh9kHrfhTuKz5ACSoB17M-Iex6AzUFMGoDC9D8hs92AJLH6NooZ4mgzOXmehobEAaMdYutpg&continue_url=http%3A%2F%2Finit-p01st.push.apple.com%2Fbag]
                continue_url  37STRING  [http://init-p01st.push.apple.com/bag]
                ap_mac      18STRING  [00:18:0a:38:be:26]
                ap_name      9STRING  [cnshap01]
                ap_tags     15STRING  [recently-added]
                client_mac  18STRING  [70:14:a6:3d:30:f6]
                client_ip   15STRING  [192.168.10.215]
                srvname     14STRING  [180.169.1.201]
                usragent    22STRING  [iPhone7,2/8.3 (12F70)]
                srvport      5STRING  [9080]
                clientip    14STRING  [180.169.1.201]
                requri     557STRING  [/portal/auth/ncSrvDispLogin.htm?login_url=https%3A%2F%2Fn113.network-auth.com%2Fsplash%2Flogin%3Fmauth%3DMMYenrF6x9b0A8vHDerEXVICb1W3gxj9RrduBSlt9KkmD5_Z8sz2wg8e_aUh0ozFwQXEEUwIBEF9pGp40d13gZGmwAF0ZBk819aLm6Upk_FN4YPoRh9kHrfhTuKz5ACSoB17M-Iex6AzUFMGoDC9D8hs92AJLH6NooZ4mgzOXmehobEAaMdYutpg%26continue_url%3Dhttp%253A%252F%252Finit-p01st.push.apple.com%252Fbag&continue_url=http%3A%2F%2Finit-p01st.push.apple.com%2Fbag&ap_mac=00%3A18%3A0a%3A38%3Abe%3A26&ap_name=cnshap01&ap_tags=recently-added&client_mac=70%3A14%3Aa6%3A3d%3A30%3Af6&client_ip=192.168.10.215]
                */
                strcpy(caSsid, "\0");
                iReturn = utMsgGetSomeNVar(psMsgHead, 15,
                                           "tsid",        UT_TYPE_STRING, 31,   caTsid,
                                           "lang",        UT_TYPE_STRING, 15,   caLang,
                                           "login_url",    UT_TYPE_STRING, 511, caPostUrl,
                                           "ssid",         UT_TYPE_STRING,     31,  caSsid,
                                           "continue_url",    UT_TYPE_STRING,  127,  caRedir,
                                           "usragent",    UT_TYPE_STRING, 255,  caUserAgent,
                                           "client_ip",    UT_TYPE_STRING, 23,   caUserIp,
                                           "srvname",     UT_TYPE_STRING, 31,   caSrvIp,
                                           "srvport",     UT_TYPE_STRING, 7,    caSrvPort,
                                           "ap_mac",      UT_TYPE_STRING, 31,   caApMac,
                                           "ap_name",     UT_TYPE_STRING, 31,   caNasid,
                                           "client_mac",  UT_TYPE_STRING, 17,   caUserMac,
                                           "code",        UT_TYPE_STRING, 15,   caAreaCode,
                                           "usernametmp", UT_TYPE_STRING, 15,   caUsernameTemp,
                                           "statusCode",  UT_TYPE_STRING, 2,    caStatus);


                if(caNasid[0] == 0)
                {
                    strcpy(caNasid, caApMac);
                }
                iVendor = NCPORTAL_VENDOR_CISCO;
                if(!utStrIsSpaces(caUserMac))
                {
                    pasCvtMacI(caUserMac, mac);
                    strcpy(caUserMac, pasCvtMac12(mac));
                }
                lSip = ntohl(pasIpcvtLong(caUserIp));
                if(!utStrIsSpaces(caTsid))
                {
                    sscanf(caTsid, "%llu", &llTsid);
                    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
                    if(psOnline == NULL)
                    {
                        psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caNasid, caApMac, lSip, mac, caSsid, caGwip, caGwport, 0);
                    }
                }
                else
                {
                    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caNasid, caApMac, lSip, mac, caSsid, caGwip, caGwport, 0);
                }
                if(psOnline)
                {
                    sprintf(caTsid, "%llu", psOnline->lTsid);
                    psOnline->cFrom = iVendor;
                    if(strncmp(caRedir, "http://1.2.3.4/getip", 20) == 0 || strstr(caUserAgent, "PROEIM;")) // APP认证
                    {
                        ncSrvAppDoGetIp(psShmHead, iFd, psMsgHead, psOnline);
                        return 0;
                    }
                }
                sprintf(caGetArg, "tsid@%s&ap_mac@%s&client_mac@%s", caTsid, caApMac, caUserMac);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "tsid",    caTsid);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "redirect", caRedir);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "ap_mac", caApMac);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang", caLang);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "login_url", caPostUrl);
            }

            else if(strncasecmp(pFrom, "WIFIDOG", 7) == 0 || strncasecmp(pFrom, "BCTH", 4) == 0) //  WiFiDOG
            {
                /*
                VarName   Bytes  Type      Content
                PFrom        8STRING  [WIFIDOG]
                gw_address  11STRING  [172.16.0.1]
                gw_port      5STRING  [8060]
                gw_id       13STRING  [AC34CB60FFDC]
                ip          12STRING  [172.16.0.24]
                mac         18STRING  [68:A0:F6:00:05:26]
                url         59STRING  [http://74.125.235.229/generate_204??r=5772&mType=netdetect]
                PASTSID     19STRING  [142805569380689504]
                srvname     14STRING  [192.168.20.31]
                usragent   148STRING  [Mozilla/5.0 (Linux; Android 4.4.2; H60-L01 Build/HDH60-L01) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/30.0.0.0 Mobile Safari/537.36]
                srvport      3STRING  [80]
                httpref    213STRING  [http://192.168.20.31/cmps/admin.php/api/login/?gw_address=172.16.0.1&gw_port=8060&gw_id=AC34CB60FFDC&ip=172.16.0.24&mac=68:A0:F6:00:05:26&url=http%3A//74.125.235.229/generate_204%3F%3Fr%3D5772%26mType%3Dnetdetect]
                clientip    15STRING  [192.168.20.212]
                requri     217STRING  [/pronline/Msg?FunName@ncSrvDispLogin&PFrom=WIFIDOG&gw_address=172.16.0.1&gw_port=8060&gw_id=AC34CB60FFDC&ip=172.16.0.24&mac=68:A0:F6:00:05:26&url=http%3A//74.125.235.229/generate_204%3F%3Fr%3D5772%26mType%3Dnetdetect]
                */
                if(strncasecmp(pFrom, "WIFIDOG", 7) == 0)
                {
                    iVendor = NCPORTAL_VENDOR_WIFIDOG;
                }
                else
                {
                    iVendor = NCPORTAL_VENDOR_BCTH;   // 宝创
                }
                strcpy(caSsid, "\0");
                // http://171.8.150.218:9080/pronline/Msg?FunName@ncSrvDispLogin&PFrom=WifiDog&wlanacname=&apmac=&ssid=&gw_address=192.168.20.1&gw_port=8060&gw_id=hotspot2&ip=192.168.20.133&mac=1C:4B:D6:8F:BE:4A&url=http%3A//www.baidu.com/
                iReturn = utMsgGetSomeNVar(psMsgHead, 15,
                                           "tsid",        UT_TYPE_STRING, 20, caTsid,
                                           "lang",        UT_TYPE_STRING, 7,  caLang,
                                           "ip",          UT_TYPE_STRING, 24, caUserIp,
                                           "ssid",        UT_TYPE_STRING, 24, caSsid,
                                           "mac",         UT_TYPE_STRING, 23, caUserMac,
                                           "gw_id",       UT_TYPE_STRING, 31, caGwid,
                                           "gw_address",  UT_TYPE_STRING, 15, caGwip,
                                           "gw_port",     UT_TYPE_STRING, 15, caGwport,
                                           "usragent",    UT_TYPE_STRING, 255, caUserAgent,
                                           "srvname",     UT_TYPE_STRING, 31, caSrvIp,
                                           "srvport",     UT_TYPE_STRING, 7,  caSrvPort,
                                           "code",        UT_TYPE_STRING, 15, caAreaCode,
                                           "usernametmp", UT_TYPE_STRING, 15, caUsernameTemp,
                                           "statusCode",  UT_TYPE_STRING, 7,  caStatus,
                                           "url",         UT_TYPE_STRING, 255, caRedir);

                if(pasCharIsUtf8(caSsid, strlen(caSsid)))
                {
                    char caTemp[32];
                    pasUtf8ToGBK(caSsid, caTemp, 31);
                    strcpy(caSsid, caTemp);
                }

                if(!utStrIsSpaces(caGwid))
                {
                    strcpy(caNasid, caGwid);
                }
                else
                {
                    strcpy(caNasid, "\0");
                }
                strcpy(caAcName, pFrom);
                strcpy(caNasid, caGwid);
                if(!utStrIsSpaces(caUserMac))
                {
                    pasCvtMacI(caUserMac, mac);
                    strcpy(caUserMac, pasCvtMac12(mac));
                }
                lSip = ntohl(pasIpcvtLong(caUserIp));

                if(!utStrIsSpaces(caTsid))
                {
                    sscanf(caTsid, "%llu", &llTsid);
                    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
                    if(psOnline == NULL)
                    {
                        psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caNasid, caGwid, lSip, mac, caSsid, caGwip, caGwport, 0);
                    }
                }
                else
                {
                    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caNasid, caGwid, lSip, mac, caSsid, caGwip, caGwport, 0);
                    if(psOnline)
                    {
                        sprintf(caTsid, "%llu", psOnline->lTsid);
                        psOnline->lGwip = ntohl(pasIpcvtLong(caGwip));
                        psOnline->nGwport = atol(caGwport);
                        psOnline->cFrom = iVendor;
                    }
                }
                if(psOnline)
                {
                    psOnline->cFrom = iVendor;

                    if(strncmp(caRedir, "http://1.2.3.4/getip", 20) == 0 || strstr(caUserAgent, "PROEIM;")) // APP认证
                    {
                        ncSrvAppDoGetIp(psShmHead, iFd, psMsgHead, psOnline);
                        return 0;
                    }
                }
                sprintf(caGetArg, "PFrom=%s&tsid=%s&gw_address=%s&gw_port=%s&gw_id=%s&ip=%s&mac=%s&url=%s",
                        pFrom, caTsid, caGwip, caGwport, caGwid, caUserIp, caUserMac, caRedir);
                strcpy(caPostArg, "\0");
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang",   caLang);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip", caUserIp);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "usermac", caUserMac);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "gw_address", caGwip);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "gw_id",     caGwid);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "gw_port",   caGwport);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "redirect",  caRedir);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "tsid",      caTsid);
                if(strcmp(caSrvPort, "443") == 0)
                {
                    sprintf(caPostUrl, "https://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
                }
                else
                {
                    sprintf(caPostUrl, "http://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
                }
            }
            else if(strncasecmp(pFrom, "Meru", 4) == 0) // Meru
            {
                /*
                [19722-7001.1008][14.12.27 12:19:21][ncSrvDispLogin]
                ======================================================
                 lCode:[990118101]  lBytes:[629]  lMsgId:[199160000] lSendTime:[1419653961]
                 lCheckSum:[0] nVersion:[200] cCrypt:[0] cOrder:[0]
                 nReplyPort:[0]  nFunCode:[1966134858] cFlags:[0] cConfirm:[0]
                 nReverse:[0] l1:[0], l2:[0]
                ========================================================
                 VarName   Bytes  Type      Content
                server_ip   15STRING  [192.168.20.187]
                login_url   17STRING  [vpn/externalauth]
                original_url  22STRING  [http://www.baidu.com/]
                ssid        10STRING  [meru-test]
                station_mac  18STRING  [14:8f:c6:78:f3:0c]
                station_ip  15STRING  [192.168.20.249]
                PFrom       13STRING  [MeruYuyuan01]
                srvname     15STRING  [192.168.20.187]
                usragent   138STRING  [Mozilla/5.0 (iPhone; CPU iPhone OS 7_1_2 like Mac OS X) AppleWebKit/537.51.2 (KHTML, like Gecko) Version/7.0 Mobile/11D257 Safari/9537.53]
                srvport      5STRING  [9080]
                httpref     22STRING  [http://www.baidu.com/]
                clientip    15STRING  [192.168.20.249]
                requri      56STRING  [/pronline/Msg?FunName@ncSrvDispLogin&PFrom@MeruYuyuan01]

                */
                strcpy(caAcName, pFrom + 4);
                strcpy(caUserMac, "\0");
                strcpy(caUserIp, "\0");
                iVendor = NCPORTAL_VENDOR_MERU;
                // http://171.8.150.218:9080/pronline/Msg?FunName@ncSrvDispLogin&PFrom@Meru
                iReturn = utMsgGetSomeNVar(psMsgHead, 12,
                                           "tsid",        UT_TYPE_STRING, 20, caTsid,
                                           "lang",        UT_TYPE_STRING, 7,  caLang,
                                           "station_ip",  UT_TYPE_STRING, 24, caUserIp,
                                           "station_mac", UT_TYPE_STRING, 31, caUserMac,
                                           "usragent",    UT_TYPE_STRING, 255, caUserAgent,
                                           "srvname",     UT_TYPE_STRING, 31, caSrvIp,
                                           "srvport",     UT_TYPE_STRING, 7,  caSrvPort,
                                           "original_url", UT_TYPE_STRING, 127, caRedir,
                                           "ssid",        UT_TYPE_STRING, 31, caSsid,
                                           "code",        UT_TYPE_STRING, 15, caAreaCode,
                                           "usernametmp", UT_TYPE_STRING, 15, caUsernameTemp,
                                           "clientip",    UT_TYPE_STRING, 15, caClientIp,
                                           "statusCode",  UT_TYPE_STRING, 7,  caStatus);
                if(utStrIsSpaces(caUserIp))
                {
                    strcpy(caUserIp, caClientIp);
                }
                lSip = ntohl(pasIpcvtLong(caUserIp));
                if(!utStrIsSpaces(caTsid))
                {
                    sscanf(caTsid, "%llu", &llTsid);
                    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
                    if(psOnline)
                    {
                        if(utStrIsSpaces(psOnline->caAcName))
                        {
                            if(!utStrIsSpaces(caUserMac))
                            {
                                iReturn = ncSrvMeruSshGetStationInfoByMac(psShmHead, caUserMac, caUserIp, caGwid, 5, caAcName);
                            }
                            else
                            {
                                iReturn = ncSrvMeruSshGetStationInfo(psShmHead, caUserIp, caUserMac0, caSsid, caGwid, 5, caAcName);
                                if(iReturn == 1)
                                {
                                    strcpy(caUserMac, caUserMac0);
                                    pasCvtMacI(caUserMac, mac);
                                    strcpy(caUserMac, pasCvtMac12(mac));
                                }
                            }
                            if(iReturn == 1)
                            {
                                strcpy(psOnline->caAcName, caAcName);
                            }
                        }
                        if(psOnline->lSip != lSip)
                        {
                            psOnline = NULL;
                        }
                    }
                }
                if(!utStrIsSpaces(caUserMac))
                {
                    pasCvtMacI(caUserMac, mac);
                    strcpy(caUserMac, pasCvtMac12(mac));
                }
                if(psOnline == NULL)
                {
                    if(!utStrIsSpaces(caUserMac))
                    {
                        iReturn = ncSrvMeruSshGetStationInfoByMac(psShmHead, caUserMac, caUserIp, caGwid, 5, caAcName);
                    }
                    else
                    {
                        iReturn = ncSrvMeruSshGetStationInfo(psShmHead, caUserIp, caUserMac0, caSsid, caGwid, 5, caAcName);
                        if(iReturn == 1)
                        {
                            strcpy(caUserMac, caUserMac0);
                            pasCvtMacI(caUserMac, mac);
                            strcpy(caUserMac, pasCvtMac12(mac));
                        }
                    }
                    pasLogs(PAS_SRCFILE, 1008, "ncSrvMeruSshGetStationInfo UserIp:%s  Mac:%s  Ssid:%s  Gwid:%s Acname:%s iReturn:%d",
                            caUserIp, caUserMac, caSsid, caGwid, caAcName, iReturn);
                    if(iReturn == 1)
                    {
                        psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caAcName, caGwid, lSip, mac, caSsid, "\0", "\0", 0);
                        if(psOnline)
                        {
                            sprintf(caTsid, "%llu", psOnline->lTsid);
                            strcpy(psOnline->caAcName, caAcName);
                        }
                    }
                    else   // 取不到Mac地址
                    {
                        if(!utStrIsSpaces(caUserMac))
                        {
                            pasCvtMacI(caUserMac, mac);
                            strcpy(caUserMac, pasCvtMac12(mac));
                            psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByMac(psShmHead, mac);
                        }
                        if(psOnline)
                        {
                            sprintf(caTsid, "%llu", psOnline->lTsid);
                            strcpy(psOnline->caAcName, caAcName);
                        }
                    }
                }
                if(psOnline)
                {
                    psOnline->cFrom = iVendor;
                    if(strncmp(caRedir, "http://1.2.3.4/getip", 20) == 0 || strstr(caUserAgent, "PROEIM;")) // APP认证
                    {
                        ncSrvAppDoGetIp(psShmHead, iFd, psMsgHead, psOnline);
                        return 0;
                    }
                }
                sprintf(caGetArg, "PFrom=%s&tsid=%s", pFrom, caTsid);
                strcpy(caPostArg, "\0");
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang",   caLang);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip", caUserIp);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "tsid",   caTsid);
                if(strcmp(caSrvPort, "443") == 0)
                {
                    sprintf(caPostUrl, "https://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
                }
                else
                {
                    sprintf(caPostUrl, "http://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
                }
            }
            else if(strncasecmp(pFrom, "MeAC", 4) == 0) // Meru AC
            {
                char *pActionUrl;
                /*
                原来美禄AC
                ========================================================
                 VarName   Bytes  Type      Content
                server_ip   14STRING  [192.168.20.31]
                login_url   17STRING  [vpn/externalauth]
                original_url  22STRING  [http://www.baidu.com/]
                ssid        10STRING  [meru-test]
                station_mac  18STRING  [90:8d:6c:e8:15:58]
                station_ip  15STRING  [192.168.20.201]
                PFrom       13STRING  [MeAcYuyuan01]
                PASTSID     19STRING  [142493366733302400]
                srvname     14STRING  [192.168.20.31]
                usragent   137STRING  [Mozilla/5.0 (iPhone; CPU iPhone OS 8_1_3 like Mac OS X) AppleWebKit/600.1.4 (KHTML, like Gecko) Version/8.0 Mobile/12B466 Safari/600.1.4]
                srvport      5STRING  [9080]
                httpref     22STRING  [http://www.baidu.com/]
                clientip    15STRING  [192.168.20.201]
                requri      56STRING  [/pronline/Msg?FunName@ncSrvDispLogin&PFrom@MeAcYuyuan01]


                奕通网关
                [19654-7001.1008][15.04.08 18:47:07][ncSrvDispLogin]
                ======================================================
                 lCode:[990118101]  lBytes:[818]  lMsgId:[205240000] lSendTime:[1428490027]
                 lCheckSum:[0] nVersion:[200] cCrypt:[0] cOrder:[0]
                 nReplyPort:[0]  nFunCode:[1966134858] cFlags:[0] cConfirm:[0]
                 nReverse:[0] l1:[0], l2:[0]
                ========================================================
                 VarName   Bytes  Type      Content
                PFrom       13STRING  [MeAcYuyuan01]
                actionUrl   27STRING  [http://192.168.11.254:8002]
                clientIp    15STRING  [192.168.11.225]
                clientMac   18STRING  [68:df:dd:63:c8:ed]
                rand         5STRING  [2762]
                devCode     11STRING  [yy20150001]
                zone        12STRING  [yuyuan_test]
                redirurl    70STRING  [http://resolver.gslb.mi-idc.com/v2/user/0/network/bucket?deviceType=0]
                srvname     14STRING  [192.168.20.31]
                usragent    68STRING  [Dalvik/1.6.0 (Linux; U; Android 4.4.4; MI 3W MIUI/V6.4.2.0.KXDCNCB)]
                srvport      5STRING  [9080]
                clientip    15STRING  [192.168.20.224]
                requri     290STRING  [/pronline/Msg?FunName@ncSrvDispLogin&PFrom@MeAcYuyuan01&actionUrl=http://192.168.11.254:8002&clientIp=192.168.11.225&clientMac=68:df:dd:63:c8:ed&rand=2762&devCode=yy20150001&zone=yuyuan_test&redirurl=http%3A%2F%2Fresolver.gslb.mi-idc.com%2Fv2%2Fuser%2F0%2Fnetwork%2Fbucket%3FdeviceType%3D0]

                */

                strcpy(caAcName, pFrom + 4);
                strcpy(caUserMac, "\0");
                strcpy(caUserIp, "\0");
                strcpy(caGwid, "\0");


                pActionUrl = utMsgGetVar_s(psMsgHead, "login_url");
                if(pActionUrl)
                {
                    // http://171.8.150.218:9080/pronline/Msg?FunName@ncSrvDispLogin&PFrom@Meru
                    iReturn = utMsgGetSomeNVar(psMsgHead, 13,
                                               "tsid",        UT_TYPE_STRING, 20, caTsid,
                                               "lang",        UT_TYPE_STRING, 7,  caLang,
                                               "station_ip",  UT_TYPE_STRING, 24, caUserIp,
                                               "station_mac", UT_TYPE_STRING, 31, caUserMac,
                                               "usragent",    UT_TYPE_STRING, 255, caUserAgent,
                                               "srvname",     UT_TYPE_STRING, 31, caSrvIp,
                                               "srvport",     UT_TYPE_STRING, 7,  caSrvPort,
                                               "original_url", UT_TYPE_STRING, 127, caRedir,
                                               "ssid",        UT_TYPE_STRING, 31, caSsid,
                                               "code",        UT_TYPE_STRING, 15, caAreaCode,
                                               "usernametmp", UT_TYPE_STRING, 15, caUsernameTemp,
                                               "clientip",    UT_TYPE_STRING, 15, caClientIp,
                                               "statusCode",  UT_TYPE_STRING, 7,  caStatus);
                    iVendor = NCPORTAL_VENDOR_MERUAC;
                    if(utStrIsSpaces(caUserIp))
                    {
                        strcpy(caUserIp, caClientIp);
                    }
                    lSip = ntohl(pasIpcvtLong(caUserIp));
                    if(!utStrIsSpaces(caTsid))
                    {
                        sscanf(caTsid, "%llu", &llTsid);
                        psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
                        if(psOnline)
                        {
                            if(utStrIsSpaces(psOnline->caAcName))
                            {
                                strcpy(psOnline->caAcName, caAcName);
                            }
                            if(psOnline->lSip != lSip)
                            {
                                psOnline = NULL;
                            }
                        }
                    }
                    if(!utStrIsSpaces(caUserMac))
                    {
                        pasCvtMacI(caUserMac, mac);
                        strcpy(caUserMac, pasCvtMac12(mac));
                    }
                    if(psOnline == NULL)
                    {
                        psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caAcName, caGwid, lSip, mac, caSsid, "\0", "\0", 0);
                        if(psOnline)
                        {
                            sprintf(caTsid, "%llu", psOnline->lTsid);
                            strcpy(psOnline->caAcName, caAcName);
                        }
                    }
                    if(psOnline)
                    {
                        psOnline->cFrom = iVendor;
                        if(strncmp(caRedir, "http://1.2.3.4/getip", 20) == 0 || strstr(caUserAgent, "PROEIM;")) // APP认证
                        {
                            ncSrvAppDoGetIp(psShmHead, iFd, psMsgHead, psOnline);
                            return 0;
                        }
                    }
                    sprintf(caGetArg, "PFrom=%s&tsid=%s", pFrom, caTsid);
                    strcpy(caPostArg, "\0");
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang",    caLang);
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip",  caUserIp);
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "usermac", caUserMac);
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "ssid",    caSsid);
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "redirect", caRedir);
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "tsid",    caTsid);
                    if(strcmp(caSrvPort, "443") == 0)
                    {
                        sprintf(caPostUrl, "https://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
                    }
                    else
                    {
                        sprintf(caPostUrl, "http://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
                    }
                }
                else    // 一通网关
                {
                    pActionUrl = utMsgGetVar_s(psMsgHead, "actionUrl");
                    if(pActionUrl)
                    {
                        char *p;
                        iReturn = utMsgGetSomeNVar(psMsgHead, 13,
                                                   "tsid",        UT_TYPE_STRING, 20, caTsid,
                                                   "lang",        UT_TYPE_STRING, 7,  caLang,
                                                   "clientIp",  UT_TYPE_STRING, 24, caUserIp,
                                                   "clientMac", UT_TYPE_STRING, 31, caUserMac,
                                                   "usragent",    UT_TYPE_STRING, 255, caUserAgent,
                                                   "srvname",     UT_TYPE_STRING, 31, caSrvIp,
                                                   "srvport",     UT_TYPE_STRING, 7,  caSrvPort,
                                                   "redirurl",    UT_TYPE_STRING, 127, caRedir,
                                                   "zone",        UT_TYPE_STRING, 31, caSsid,
                                                   "code",        UT_TYPE_STRING, 15, caAreaCode,
                                                   "usernametmp", UT_TYPE_STRING, 15, caUsernameTemp,
                                                   "clientip",    UT_TYPE_STRING, 15, caClientIp,
                                                   "statusCode",  UT_TYPE_STRING, 7,  caStatus);
                        iVendor = NCPORTAL_VENDOR_MERUAC1;
                        if(utStrIsSpaces(caUserIp))
                        {
                            strcpy(caUserIp, caClientIp);
                        }
                        lSip = ntohl(pasIpcvtLong(caUserIp));
                        if(!utStrIsSpaces(caTsid))
                        {
                            sscanf(caTsid, "%llu", &llTsid);
                            psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
                            if(psOnline)
                            {
                                if(utStrIsSpaces(psOnline->caAcName))
                                {
                                    strcpy(psOnline->caAcName, caAcName);
                                }
                                if(psOnline->lSip != lSip)
                                {
                                    psOnline = NULL;
                                }
                            }
                        }
                        if(!utStrIsSpaces(caUserMac))
                        {
                            pasCvtMacI(caUserMac, mac);
                            strcpy(caUserMac, pasCvtMac12(mac));
                        }
                        if(psOnline == NULL)
                        {
                            psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caAcName, caGwid, lSip, mac, caSsid, "\0", "\0", 0);
                            if(psOnline)
                            {
                                sprintf(caTsid, "%llu", psOnline->lTsid);
                                strcpy(psOnline->caAcName, caAcName);
                            }
                        }
                        if(psOnline)
                        {
                            psOnline->cFrom = iVendor;
                            if(strncmp(caRedir, "http://1.2.3.4/getip", 20) == 0 || strstr(caUserAgent, "PROEIM;")) // APP认证
                            {
                                ncSrvAppDoGetIp(psShmHead, iFd, psMsgHead, psOnline);
                                return 0;
                            }
                        }
                        sprintf(caGetArg, "PFrom=%s&tsid=%s", pFrom, caTsid);
                        strcpy(caPostArg, "\0");
                        p = utComGetVar_s(psShmHead, "LoginOK");
                        if(p)
                        {
                            strcpy(caRedir, p);
                            utStrReplaceWith(caRedir, "[#tsid#]", caTsid);
                        }
                        utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
                        utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang",    caLang);
                        utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip",  caUserIp);
                        utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "usermac", caUserMac);
                        utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "ssid",    caSsid);
                        utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "redirurl", caRedir);
                        utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "tsid",    caTsid);
                        sprintf(caPostUrl, "%s", pActionUrl);
                        if(psOnline)
                        {
                            strcpy(psOnline->caPostUrl, pActionUrl);
                        }
                    }
                }
            }

            else if(strncasecmp(pFrom, "ProEIMV7", 7) == 0) // ProEIMV7
            {
                char caUserMac1[64];
                char caNasid1[64];
                char caApmac1[64];
                char caAcname1[64];
                char caAcname0[64];
                char caSsid0[64];
                /*
                [23198-7001.1008][15.01.06 12:08:19][ncSrvDispLogin]
                ======================================================
                 lCode:[990118101]  lBytes:[645]  lMsgId:[316430000] lSendTime:[1420517299]
                 lCheckSum:[0] nVersion:[200] cCrypt:[0] cOrder:[0]
                 nReplyPort:[0]  nFunCode:[1966134858] cFlags:[0] cConfirm:[0]
                 nReverse:[0] l1:[0], l2:[0]
                ========================================================
                 VarName   Bytes  Type      Content
                PFrom        9STRING  [ProEIMV7]
                ip          15STRING  [192.168.20.201]
                mac         49STRING  [D29F19B64C8663567D671E5CDAE47F2E024487C8736735B6]
                ncip        14STRING  [192.168.20.31]
                ncport      11STRING  [3215284460]
                siteid      17STRING  [B4902CEDE5F911BB]
                gwid        29STRING  [1E5CDAE47F2E024487C8736735B6]
                srvname     14STRING  [192.168.20.29]
                usragent    25STRING  [iPhone3,1/7.1.2 (11D257)]
                srvport      5STRING  [9080]
                clientip    15STRING  [192.168.20.201]
                requri     219STRING  [/pronline/Msg?FunName@ncSrvDispLogin&PFrom=ProEIMV7&ip=192.168.20.201&mac=D29F19B64C8663567D671E5CDAE47F2E024487C8736735B6&ncip=192.168.20.31&ncport=3215284460&siteid=B4902CEDE5F911BB&nasid=1E5CDAE47F2E024487C8736735B6]

                */
                strcpy(caAcname0, "\0");
                iReturn = utMsgGetSomeNVar(psMsgHead, 16,
                                           "tsid",        UT_TYPE_STRING, 20,    caTsid,
                                           "lang",        UT_TYPE_STRING, 15,    caLang,
                                           "ip",          UT_TYPE_STRING, 24,    caUserIp,
                                           "mac",         UT_TYPE_STRING, 63,    caUserMac1,
                                           "ncip",        UT_TYPE_STRING, 15,    caGwip,
                                           "ncport",      UT_TYPE_STRING, 7,     caGwport,
                                           "siteid",      UT_TYPE_STRING, 63,    caNasid1,
                                           "ssid",        UT_TYPE_STRING, 63,    caSsid0,
                                           "gwid",        UT_TYPE_STRING, 63,    caApmac1,
                                           "acname",      UT_TYPE_STRING, 63,    caAcname1,
                                           "pacname",     UT_TYPE_STRING, 63,    caAcname0,    // 这个参数优先于acname
                                           "usragent",    UT_TYPE_STRING, 255,   caUserAgent,
                                           "srvname",     UT_TYPE_STRING, 31,    caSrvIp,
                                           "srvport",     UT_TYPE_STRING, 7,     caSrvPort,
                                           "code",        UT_TYPE_STRING, 15,    caAreaCode,
                                           "statusCode",  UT_TYPE_STRING, 2,     caStatus);
                iVendor = NCPORTAL_VENDOR_PROEIMV7;
                if(!utStrIsSpaces(caAcname0))
                {
                    strcpy(caAcname1, caAcname0);
                }
                pasStrDecodeDesHex(caUserMac1, caUserMac, PAS_DEF_KEY);
                pasStrDecodeDesHex(caNasid1, caNasid, PAS_DEF_KEY);
                pasStrDecodeDesHex(caApmac1, caApMac, PAS_DEF_KEY);
                pasStrDecodeDesHex(caAcname1, caAcName, PAS_DEF_KEY);
                pasStrDecodeDesHex(caSsid0, caSsid, PAS_DEF_KEY);
                pasLogs(PAS_SRCFILE, 1008, " UserMac:%s  nasid:%s  ApMac:%s AcName:%s Ssid=%s ",
                        caUserMac, caNasid, caApMac, caAcName, caSsid);
                pasCvtMacI(caUserMac, mac);
                strcpy(caUserMac, pasCvtMac12(mac));
                lSip = ntohl(pasIpcvtLong(caUserIp));
                if(!utStrIsSpaces(caTsid))
                {
                    sscanf(caTsid, "%llu", &llTsid);
                    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
                }
                if(psOnline == NULL)
                {
                    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caNasid, caApMac, lSip, mac, caSsid, caGwip, caGwport, 0);
                    if(psOnline)
                    {
                        sprintf(caTsid, "%llu", psOnline->lTsid);
                    }
                }
                else
                {
                    sprintf(caTsid, "%llu", psOnline->lTsid);
                    if(!utStrIsSpaces(caAcName))
                    {
                        strcpy(psOnline->caAcName, caAcName);
                    }
                    if(utStrIsSpaces(caSsid))
                    {
                        strcpy(caSsid, psOnline->caSsid);
                    }
                    if(utStrIsSpaces(caUserMac))
                    {
                        strcpy(caUserMac, pasCvtMac(psOnline->caMac));
                    }
                    if(utStrIsSpaces(caApMac))
                    {
                        strcpy(caApMac, pasCvtMac(psOnline->caApName));
                    }
                    pasLogs(PAS_SRCFILE, 1008, " UserMac:%s ApMac:%s Ssid:%s",
                            caUserMac, caApMac, caSsid);
                }
                if(psOnline)
                {
                    psOnline->cFrom = iVendor;
                    if(strncmp(caRedir, "http://1.2.3.4/getip", 20) == 0 || strstr(caUserAgent, "PROEIM;")) // APP认证
                    {
                        ncSrvAppDoGetIp(psShmHead, iFd, psMsgHead, psOnline);
                        return 0;
                    }
                }
                if(strcmp(caSrvPort, "443") == 0)
                {
                    sprintf(caPostUrl, "https://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
                }
                else
                {
                    sprintf(caPostUrl, "http://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
                }
                sprintf(caGetArg, "PFrom=%s&tsid@%s", pFrom, caTsid);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "tsid",    caTsid);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang",    caLang);
            }
        }
    }
    if(iVendor == 0)
    {
        pAcIp = utMsgGetVar_s(psMsgHead, "wlanacname");
        if(pAcIp && !utStrIsSpaces(pAcIp))
        {
            strcpy(caAcName, pAcIp);
        }
        else
        {
            p = utMsgGetVar_s(psMsgHead, "wlanacip");
            if(p && !utStrIsSpaces(p))
            {
                ncPortalAcInfo *psAc0;
                psAc0 = (ncPortalAcInfo *)ncSrvGetAcInfoByIp(psShmHead, p);
                if(psAc0)
                {
                    strcpy(caAcName, psAc0->caName);
                }
            }
        }
        if(!utStrIsSpaces(caAcName))    // 针对寰创AP 三维ac 及 华三AC,华为等遵循移动2.0标准的AC
        {
            char caUserMac1[24];
            char caUserMac2[24];
            /*
                 [5398-7001.1008][14.07.08 17:43:07][ncSrvDispLogin]
                     VarName   Bytes  Type      Content
                     wlanuserip  14STRING  [10.69.240.183]
                     wlanacname  21STRING  [1111.0000.000.00.460]
                     wlanacip    14STRING  [171.8.150.214]
                     ssid         6STRING  [ICPIC]
                     nasid       21STRING  [0000.0000.000.00.460]
                     usermac     18STRING  [F0-5A-09-E8-B9-5A]
                     wlanapmac   18STRING  [00-34-CB-90-F0-18]
                     srvname     14STRING  [171.8.150.210]
                     usragent    13STRING  [Agoo-sdk-2.0]
                     srvport      5STRING  [9080]
                     clientip    14STRING  [123.55.197.78]
                     requri     209STRING  [/pronline/Msg?FunName@ncSrvDispLogin&wlanuserip=10.69.240.183&wlanacname=1111.0000.000.00.460&wlanacip=171.8.150.214&ssid=ICPIC&nasid=0000.0000.000.00.460&usermac=F0-5A-09-E8-B9-5A&wlanapmac=00-34-CB-90-F0-18]


                 锐捷：

            [12513-7001.1008][15.03.17 15:56:29][ncSrvDispLogin]
            ======================================================
            lCode:[990118101]  lBytes:[925]  lMsgId:[131410000] lSendTime:[1426578989]
            lCheckSum:[0] nVersion:[200] cCrypt:[0] cOrder:[0]
            nReplyPort:[0]  nFunCode:[1966134858] cFlags:[0] cConfirm:[0]
            nReverse:[0] l1:[0], l2:[0]
            ========================================================
            VarName   Bytes  Type      Content
            wlanuserip  11STRING  [10.99.3.18]wlanacname  16STRING  [Ac_14144b7c9ad5]
            ssid        14STRING  [hongkongplaza]
            nasip       10STRING  [10.3.40.5]
            mac         13STRING  [ac3c0bc8c308]
            t           18STRING  [wireless-v2-plain]
            url        105STRING  [http://captive.apple.com/MJN2LCfLZVmhR2/FSnKD6FNbpXpDx/2RrHaaRg16dLyi/yuRd47m5FWhzHi/I059B3jZ2Gse93.html]
            srvname     10STRING  [10.3.40.6]
            usragent   101STRING  [Mozilla/5.0 (iPad; CPU OS 8_0_2 like Mac OS X) AppleWebKit/600.1.4 (KHTML, like Gecko) Mobile/12A405]
            srvport      5STRING  [9080]
            httpref    105STRING  [http://captive.apple.com/MJN2LCfLZVmhR2/FSnKD6FNbpXpDx/2RrHaaRg16dLyi/yuRd47m5FWhzHi/I059B3jZ2Gse93.html]
            clientip    11STRING  [10.99.3.18]
            requri     262STRING  [/portal/auth/ncSrvDispLogin.htm?wlanuserip=10.99.3.18&wlanacname=Ac_14144b7c9ad5&ssid=hongkongplaza&nasip=10.3.40.5&mac=ac3c0bc8c308&t=wireless-v2-plain&url=http://captive.apple.com/MJN2LCfLZVmhR2/FSnKD6FNbpXpDx/2RrHaaRg16dLyi/yuRd47m5FWhzHi/I059B3jZ2Gse93.html]
             */
            strcpy(caUserMac2, "\0");
            strcpy(caUserMac1, "\0");
            iReturn = utMsgGetSomeNVar(psMsgHead, 15,
                                       "tsid",        UT_TYPE_STRING, 20, caTsid,
                                       "lang",        UT_TYPE_STRING, 7,  caLang,
                                       "wlanuserip",  UT_TYPE_STRING, 24, caUserIp,
                                       "ssid",        UT_TYPE_STRING, 24, caSsid,
                                       "usermac",     UT_TYPE_STRING, 23, caUserMac,
                                       "wlanapmac",   UT_TYPE_STRING, 23, caApMac,
                                       "user-mac-key", UT_TYPE_STRING, 23, caUserMac1,
                                       "mac",         UT_TYPE_STRING, 23, caUserMac2,
                                       "usragent",    UT_TYPE_STRING, 255, caUserAgent,
                                       "srvname",     UT_TYPE_STRING, 31, caSrvIp,
                                       "srvport",     UT_TYPE_STRING, 7,  caSrvPort,
                                       "code",        UT_TYPE_STRING, 15, caAreaCode,
                                       "usernametmp", UT_TYPE_STRING, 15, caUsernameTemp,
                                       "clientip",    UT_TYPE_STRING, 23, caClientIp,
                                       "statusCode",  UT_TYPE_STRING, 7,  caStatus);
            if(pasCharIsUtf8(caSsid, strlen(caSsid)))
            {
                char caTemp[32];
                pasUtf8ToGBK(caSsid, caTemp, 31);
                strcpy(caSsid, caTemp);
            }
            iVendor = NCPORTAL_VENDOR_GBCOM;
            if(pasIsMacAddress(caApMac))
            {
                pasCvtMac2StdMac(caApMac);
            }

            if(utStrIsSpaces(caUserIp))
            {
                strcpy(caUserIp, caClientIp);
            }
            if(!utStrIsSpaces(caAcName))
            {
                strcpy(caNasid, caAcName);
            }
            else if(!utStrIsSpaces(caApMac))
            {
                char mac1[6];
                pasCvtMacI(caApMac, mac1);
                strcpy(caNasid, (char *)ncSrvCvtApMac(mac1));
            }

            if(!utStrIsSpaces(caUserMac))
            {
                pasCvtMacI(caUserMac, mac);
                strcpy(caUserMac, pasCvtMac12(mac));
            }
            else
            {
                if(!utStrIsSpaces(caUserMac1))
                {
                    pasCvtMacI(caUserMac1, mac);
                    strcpy(caUserMac, pasCvtMac12(mac));
                }
                else if(!utStrIsSpaces(caUserMac2))
                {
                    pasCvtMacI(caUserMac2, mac);
                    strcpy(caUserMac, pasCvtMac12(mac));
                }
                else
                {
                    memset(mac, 0, 6);
                }
            }

            lSip = ntohl(pasIpcvtLong(caUserIp));
            if(!utStrIsSpaces(caTsid))
            {
                sscanf(caTsid, "%llu", &llTsid);
                if(llTsid > 0)
                {
                    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
                    if(psOnline == NULL)
                    {
                        psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caNasid, caApMac, lSip, mac, caSsid, caGwip, caGwport, 0);
                    }
                }
            }
            if(psOnline == NULL)
            {
                pasLogs(PAS_SRCFILE, 1008, "ncSrvGetOnlineUserByNasidIpA UserIp:%s  NasId:%s ", caUserIp, caNasid);
                psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caNasid, caApMac, lSip, mac, caSsid, caGwip, caGwport, 0);
            }
            if(psOnline)
            {
                sprintf(caTsid, "%llu", psOnline->lTsid);
                if(!utStrIsSpaces(caApMac))
                {
                    strcpy(psOnline->caApName, caApMac);
                }
                if(psOnline->lAcIp == 0)
                {
                    ncPortalAcInfo *psAc;
                    psAc = (ncPortalAcInfo *)ncSrvGetAcInfo(psShmHead, caAcName);
                    if(psAc)
                    {
                        psOnline->lAcIp = psAc->lAcip;
                        psOnline->nAcPort = psAc->nAcPort;
                        psOnline->psAc = psAc;
                    }
                }
                psOnline->cFrom = iVendor;
                if(strncmp(caRedir, "http://1.2.3.4/getip", 20) == 0 || strstr(caUserAgent, "PROEIM;")) // APP认证
                {
                    ncSrvAppDoGetIp(psShmHead, iFd, psMsgHead, psOnline);
                    return 0;
                }
            }
            strcpy(caPostArg, "\0");
            sprintf(caGetArg, "userip@%s", caUserIp);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang",   caLang);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip", caUserIp);
            if(!utStrIsSpaces(caTsid))
            {
                utStrAddF(caGetArg, "&tsid@%s", caTsid);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "tsid",   caTsid);
            }
            if(!utStrIsSpaces(caAcName))
            {
                utStrAddF(caGetArg, "&wlanacname@%s", caAcName);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "wlanacname", caAcName);
            }
            if(!utStrIsSpaces(caSsid))
            {
                utStrAddF(caGetArg, "&ssid@%s", caSsid);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "ssid",   caSsid);
            }
            if(!utStrIsSpaces(caUserMac))
            {
                utStrAddF(caGetArg, "&usermac@%s", caUserMac);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "usermac", caUserMac);
            }

            if(!utStrIsSpaces(caApMac))
            {
                utStrAddF(caGetArg, "&wlanapmac@%s", caApMac);
            }
            if(!utStrIsSpaces(caNasid))
            {
                utStrAddF(caGetArg, "&nasid@%s", caNasid);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "nasid",  caNasid);
            }
            if(strcmp(caSrvPort, "443") == 0)
            {
                sprintf(caPostUrl, "https://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
            }
            else
            {
                sprintf(caPostUrl, "http://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
            }
        }
    }

    if(iVendor == 0)
    {
        /* For Cisco
        switch_url  27STRING  [https://1.1.1.1/login.html]
        ap_mac      18STRING  [70:10:5c:a1:36:10]
        client_mac  18STRING  [98:d6:bb:4e:c4:4a]
        wlan        12STRING  [Apple Store]
        redirect    65STRING  [www.airport.us/qCdXxjjnQdTwch/fOjr5QqvHZMMRH/UgK7dBuWuQI8os.html]
        srvname     12STRING  [10.216.80.9]
        usragent   111STRING  [Mozilla/5.0 (iPhone; CPU iPhone OS 7_0_3 like Mac OS X) AppleWebKit/537.51.1 (KHTML, like Gecko) Mobile/11B511]
        srvport      5STRING  [9080]
        httpref     72STRING  [http://www.airport.us/qCdXxjjnQdTwch/fOjr5QqvHZMMRH/UgK7dBuWuQI8os.html]
        clientip    14STRING  [10.216.82.110]
        requri     222STRING  [/pronline/Msg?FunName@ncSrvDispLogin?switch_url=https://1.1.1.1/login.html&ap_mac=70:10:5c:a1:36:10&client_mac=98:d6:bb:4e:c4:4a&wlan=Apple%20Store&redirect=www.airport.us/qCdXxjjnQdTwch/fOjr5QqvHZMMRH/UgK7dBuWuQI8os.html]
        */
        p = utMsgGetVar_s(psMsgHead, "switch_url");
        if(p && !utStrIsSpaces(p))
        {
            char caTemp[256];
            if(strncmp(p, "BB66", 4) == 0) // Base64加密
            {
                pasBase64_setbz('*', '-', 0);
                strcpy(caTemp, p);
                pasBase64_decode(caTemp + 4, caPostUrl);
            }
            else
            {
                strcpy(caPostUrl, p);
            }

            iReturn = utMsgGetSomeNVar(psMsgHead, 13,
                                       "lang",        UT_TYPE_STRING, 15,   caLang,
                                       "wlan",        UT_TYPE_STRING, 24,   caSsid,
                                       "redirect",    UT_TYPE_STRING, 127,  caRedir,
                                       "usragent",    UT_TYPE_STRING, 255,  caUserAgent,
                                       "clientip",    UT_TYPE_STRING, 23,   caClientIp,
                                       "srvname",     UT_TYPE_STRING, 31,   caSrvIp,
                                       "srvport",     UT_TYPE_STRING, 7,    caSrvPort,
                                       "ap_mac",      UT_TYPE_STRING, 31,   caApMac,
                                       "client_mac",  UT_TYPE_STRING, 17,   caUserMac,
                                       "code",        UT_TYPE_STRING, 15,   caAreaCode,
                                       "usernametmp", UT_TYPE_STRING, 15,   caUsernameTemp,
                                       "nasid",       UT_TYPE_STRING, 31,   caNasid,
                                       "statusCode",  UT_TYPE_STRING, 2,    caStatus);
            if(pasCharIsUtf8(caSsid, strlen(caSsid)))
            {
                char caTemp[32];
                pasUtf8ToGBK(caSsid, caTemp, 31);
                strcpy(caSsid, caTemp);
            }
            if(strncmp(caRedir, "BB66", 4) == 0) // Base64加密
            {
                pasBase64_setbz('*', '-', 0);
                pasBase64_decode(caRedir + 4, caTemp);
                strcpy(caRedir, caTemp);
            }
            if(caNasid[0] == 0)
            {
                strcpy(caNasid, caApMac);
            }
            iVendor = NCPORTAL_VENDOR_CISCO;
            strcpy(caUserIp, caClientIp);
            if(!utStrIsSpaces(caUserMac))
            {
                pasCvtMacI(caUserMac, mac);
                strcpy(caUserMac, pasCvtMac12(mac));
            }
            lSip = ntohl(pasIpcvtLong(caUserIp));
            if(!utStrIsSpaces(caTsid))
            {
                sscanf(caTsid, "%llu", &llTsid);
                psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
                if(psOnline == NULL)
                {
                    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caNasid, caApMac, lSip, mac, caSsid, caGwip, caGwport, 0);
                }
            }
            else
            {
                psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caNasid, caApMac, lSip, mac, caSsid, caGwip, caGwport, 0);
            }
            if(psOnline)
            {
                sprintf(caTsid, "%llu", psOnline->lTsid);
                psOnline->cFrom = iVendor;
                if(strncmp(caRedir, "http://1.2.3.4/getip", 20) == 0 || strstr(caUserAgent, "PROEIM;")) // APP认证
                {
                    ncSrvAppDoGetIp(psShmHead, iFd, psMsgHead, psOnline);
                    return 0;
                }
            }
            sprintf(caGetArg, "wlan@%s&switch_url@%s&ap_mac@%s&client_mac@%s", caSsid, caPostUrl, caApMac, caUserMac);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "buttonClicked", "4");
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "redirect", caRedir);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "err_flag", "0");
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "err_msg", "\0");
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "info_msg", "\0");
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "ap_mac", caApMac);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang", caLang);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "switch_url", caPostUrl);
        }
    }

    if(iVendor == 0)
    {
        /* For Aruba

        ======================================================
        lCode:[990118101]  lBytes:[625]  lMsgId:[314130000] lSendTime:[1403686392]
        lCheckSum:[0] nVersion:[200] cCrypt:[0] cOrder:[0]
        nReplyPort:[0]  nFunCode:[1966134858] cFlags:[0] cConfirm:[0]
        nReverse:[0] l1:[0], l2:[0]
        ========================================================
        VarName   Bytes  Type      Content
        cmd          6STRING  [login]
        mac         18STRING  [38:59:f9:e5:cf:54]
        ip          11STRING  [172.16.0.3]
        essid        7STRING  [dannis]
        apname      18STRING  [9c:1c:12:cc:2f:0a]
        apgroup      8STRING  [default]
        url         53STRING  [http://crl.microsoft.com/pki/crl/products/WinPCA.crl]
        srvname     14STRING  [180.169.1.203]
        usragent    24STRING  [Microsoft-CryptoAPI/6.1]
        srvport      5STRING  [9080]
        clientip    16STRING  [123.160.230.159]
        requri     224STRING  [/pronline/Msg?FunName@ncSrvDispLogin&cmd=login&mac=38:59:f9:e5:cf:54&ip=172.16.0.3&essid=dannis&apname=9c%3A1c%3A12%3Acc%3A2f%3A0a&apgroup=default&url=http%3A%2F%2Fcrl%2Emicrosoft%2Ecom%2Fpki%2Fcrl%2Fproducts%2FWinPCA%2Ecrl]

        --- 验证出错
        [19312-7001.1008][14.06.26 10:27:52][ncSrvDispLogin]
        ======================================================
        lCode:[990118101]  lBytes:[1001]  lMsgId:[13960000] lSendTime:[1403749672]
        lCheckSum:[0] nVersion:[200] cCrypt:[0] cOrder:[0]
        nReplyPort:[0]  nFunCode:[1966134858] cFlags:[0] cConfirm:[0]
        nReverse:[0] l1:[0], l2:[0]
        ========================================================
        VarName   Bytes  Type      Content
        errmsg      22STRING  [Authentication failed]
        mac         18STRING  [c4:6a:b7:55:56:fb]
        ip          12STRING  [172.16.0.12]
        essid        7STRING  [dannis]
        apname      18STRING  [9c:1c:12:cc:2f:0a]
        apgroup      8STRING  [default]
        url         40STRING  [http://clients3.google.com/generate_204]
        srvname     14STRING  [180.169.1.203]
        usragent   143STRING  [Mozilla/5.0 (Linux; U; Android 4.0.1; zh-cn; MI-ONE Plus Build/ITL41D) AppleWebKit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30]
        srvport      5STRING  [9080]
        httpref    231STRING  [http://180.169.1.203:9080/pronline/Msg?FunName@ncSrvDispLogin&cmd=login&mac=c4:6a:b7:55:56:fb&ip=172.16.0.12&essid=dannis&apname=9c%3A1c%3A12%3Acc%3A2f%3A0a&apgroup=default&url=http%3A%2F%2Fclients3%2Egoogle%2Ecom%2Fgenerate%5F204]
        clientip    16STRING  [123.160.230.159]
        requri     227STRING  [/pronline/Msg?FunName@ncSrvDispLogin?errmsg=Authentication%20failed&mac=c4:6a:b7:55:56:fb&ip=172.16.0.12&essid=dannis&apname=9c%3A1c%3A12%3Acc%3A2f%3A0a&apgroup=default&url=http%3A%2F%2Fclients3%2Egoogle%2Ecom%2Fgenerate%5F204]

        */
        p = utMsgGetVar_s(psMsgHead, "essid");
        if(p)
        {
            ncPortalAcInfo *psAc;
            char caCmd[16], caErr[64], caAcid[32];
            strcpy(caCmd, "\0");
            strcpy(caErr, "\0");
            strcpy(caSsid, p);
            strcpy(caAcid, "\0");
            iReturn = utMsgGetSomeNVar(psMsgHead, 12,
                                       "cmd",         UT_TYPE_STRING, 15,   caCmd,
                                       "errmsg",      UT_TYPE_STRING, 15,   caErr,
                                       "tsid",        UT_TYPE_STRING, 20,   caTsid,
                                       "lang",        UT_TYPE_STRING, 15,   caLang,
                                       "url",         UT_TYPE_STRING, 127,  caRedir,
                                       "usragent",    UT_TYPE_STRING, 255,  caUserAgent,
                                       "ip",          UT_TYPE_STRING, 23,   caUserIp,
                                       "srvname",     UT_TYPE_STRING, 31,   caSrvIp,
                                       "srvport",     UT_TYPE_STRING, 7,    caSrvPort,
                                       "apname",      UT_TYPE_STRING, 31,   caApMac,
                                       "acid",        UT_TYPE_STRING, 31,   caAcid,
                                       "mac",         UT_TYPE_STRING, 17,   caUserMac);
            if(pasCharIsUtf8(caSsid, strlen(caSsid)))
            {
                char caTemp[32];
                pasUtf8ToGBK(caSsid, caTemp, 31);
                strcpy(caSsid, caTemp);
            }
            if(strcasecmp(caCmd, "login") == 0)
            {
            }
            if(!utStrIsSpaces(caErr))
            {
                strcpy(caStatus, "5");
            }
            if(caNasid[0] == 0)
            {
                strcpy(caNasid, caApMac);
            }
            psAc = (ncPortalAcInfo *)ncSrvGetAcInfo(psShmHead, caAcid);
            if(psAc)
            {
                strcpy(caPostUrl, psAc->caPost);
            }
            if(utStrIsSpaces(caPostUrl))
            {
                strcpy(caPostUrl, "http://securelogin.arubanetworks.com/auth/index.html/u");
            }
            pasLogs(PAS_SRCFILE, 1008, "PostUrl=%s ", caPostUrl);
            iVendor = NCPORTAL_VENDOR_ARUBA;
            if(!utStrIsSpaces(caUserMac))
            {
                pasCvtMacI(caUserMac, mac);
                strcpy(caUserMac, pasCvtMac12(mac));
            }
            lSip = ntohl(pasIpcvtLong(caUserIp));
            if(!utStrIsSpaces(caTsid))
            {
                sscanf(caTsid, "%llu", &llTsid);
                psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
                if(psOnline == NULL)
                {
                    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caNasid, caApMac, lSip, mac, caSsid, "\0", "\0", 0);
                }
                else
                {
                    strcpy(caLang, psOnline->caLang);
                    strcpy(caSsid, psOnline->caSsid);
                }
            }
            else
            {
                if(!utStrIsSpaces(caNasid))
                {
                    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caNasid, caApMac, lSip, mac, caSsid, "\0", "\0", 0);
                }
                else
                {
                    char *pHref, *p;
                    pHref = utMsgGetVar_s(psMsgHead, "httpref");
                    if(pHref)
                    {
                        char caOut[512];
                        pasStrUrlUnEsc(pHref, caOut);
                        strcpy(caTsid, "\0");
                        ncUtlGetWordBetween(caOut, "tsid=", "&", caTsid, 31);
                        if(!utStrIsSpaces(caTsid))
                        {
                            sscanf(caTsid, "%llu", &llTsid);
                            psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
                            pasLogs(PAS_SRCFILE, 1008, "Get Tsid From Href=tsid=%llu ", llTsid);
                        }
                    }
                }
            }
            if(psOnline)
            {
                sprintf(caTsid, "%llu", psOnline->lTsid);
                psOnline->cFrom = iVendor;
                if(!utStrIsSpaces(caPostUrl))
                {
                    strcpy(psOnline->caPostUrl, caPostUrl);
                }
                if(strncmp(caRedir, "http://1.2.3.4/getip", 20) == 0 || strstr(caUserAgent, "PROEIM;")) // APP认证
                {
                    ncSrvAppDoGetIp(psShmHead, iFd, psMsgHead, psOnline);
                    return 0;
                }
            }
            sprintf(caGetArg, "cmd=login&tsid=%llu&essid=%s&acid=%s", psOnline->lTsid, caSsid, caAcid);

            if(!utStrIsSpaces(caErr))
            {
                utStrAddF(caGetArg, "&errmsg=%s", caErr);
            }
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "redirect", caRedir);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang", caLang);
        }
    }

    if(iVendor == 0)
    {
        /* For Moto
           // webpage external login http://192.168.20.198:9080/portal/Moto/ncSrvDispLogin.htm?Pfrom=Moto&client_mac=WING_TAG_CLIENT_MAC&ap_mac=WING_TAG_AP_MAC&
            PFrom        5STRING  [Moto]
            client_ip   14STRING  [192.168.1.145]
            client_mac  18STRING  [3C-43-8E-D4-53-92]
            wlan_ssid    8STRING  [ProMoto]
            ap_mac      18STRING  [B4-C7-99-71-EA-E8]
            srvname     14STRING  [192.168.20.29]
            usragent   143STRING  [Mozilla/5.0 (Linux; U; Android 4.1.2; zh-cn; XT928 Build/6.7.2_GC-383) AppleWebKit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30]
            srvport      5STRING  [9080]
            clientip    15STRING  [192.168.10.155]
            requri     275STRING  [/portal/Moto/ncSrvDispLogin.htm/client_ip=192.168.1.145&client_mac=3C-43-8E-D4-53-92&wlan_ssid=ProMoto&ap_mac=B4-C7-99-71-EA-E8?hs_server=192.168.1.144&Qv=it_qpmjdz=betq@bbb_qpmjdz=uftu@dmjfou_njou=5379:7215@dmjfou_nbd=4D.54.9F.E5.64.:3@ttje=QspNpup@bq_nbd=C5.D8.::.82.FB.F9]
            */
        p = utMsgGetVar_s(psMsgHead, "PFrom");
        if(p && strncasecmp(p, "Moto", 4) == 0)
        {
            ncPortalAcInfo *psAc;
            char caQv[256], caHserver[32];
            char caCmd[16], caErr[64], caAcid[32];
            strcpy(caErr, "\0");
            strcpy(caSsid, p);
            strcpy(caAcid, p);
            iReturn = utMsgGetSomeNVar(psMsgHead, 11,
                                       "tsid",        UT_TYPE_STRING, 20,   caTsid,
                                       "lang",        UT_TYPE_STRING, 15,   caLang,
                                       "srvname",     UT_TYPE_STRING, 31, caSrvIp,
                                       "srvport",     UT_TYPE_STRING, 7,  caSrvPort,
                                       "wlan_ssid",   UT_TYPE_STRING, 127,  caSsid,
                                       "usragent",    UT_TYPE_STRING, 255,  caUserAgent,
                                       "client_ip",   UT_TYPE_STRING, 23,   caUserIp,
                                       "ap_mac",      UT_TYPE_STRING, 31,   caApMac,
                                       "hs_server",   UT_TYPE_STRING, 23,   caHserver,
                                       "Qv",          UT_TYPE_STRING, 255,  caQv,
                                       "client_mac",  UT_TYPE_STRING, 17,   caUserMac);
            strcpy(caNasid, caApMac);
            strcpy(caAcid, p);
            if(pasCharIsUtf8(caSsid, strlen(caSsid)))
            {
                char caTemp[32];
                pasUtf8ToGBK(caSsid, caTemp, 31);
                strcpy(caSsid, caTemp);
            }
            psAc = (ncPortalAcInfo *)ncSrvGetAcInfo(psShmHead, caAcid);
            if(psAc)
            {
                strcpy(caPostUrl, psAc->caPost);
            }

            if(utStrIsSpaces(caPostUrl))
            {
                char caVar[32];
                sprintf(caVar, "AuthUrl_%s", p);
                strcpy(caPostUrl, utComGetVar_sd(psShmHead, caVar, "http://192.168.1.254/cgi-bin/hslogin.cgi"));
            }
            pasLogs(PAS_SRCFILE, 1008, "PostUrl=%s ", caPostUrl);
            iVendor = NCPORTAL_VENDOR_MOTO;
            if(!utStrIsSpaces(caUserMac))
            {
                pasCvtMacI(caUserMac, mac);
                strcpy(caUserMac, pasCvtMac12(mac));
            }
            lSip = ntohl(pasIpcvtLong(caUserIp));
            if(!utStrIsSpaces(caTsid))
            {
                sscanf(caTsid, "%llu", &llTsid);
                if(llTsid > 0)
                {
                    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
                }
                if(psOnline == NULL)
                {
                    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caNasid, caApMac, lSip, mac, caSsid, "\0", "\0", 0);
                }
            }
            if(psOnline == NULL)
            {
                psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caNasid, caApMac, lSip, mac, caSsid, "\0", "\0", 0);
            }
            if(psOnline)
            {
                sprintf(caTsid, "%llu", psOnline->lTsid);
                psOnline->cFrom = iVendor;
                strcpy(psOnline->caPostUrl, caPostUrl);
                if(strncmp(caRedir, "http://1.2.3.4/getip", 20) == 0 || strstr(caUserAgent, "PROEIM;")) // APP认证
                {
                    ncSrvAppDoGetIp(psShmHead, iFd, psMsgHead, psOnline);
                    return 0;
                }
            }
            sprintf(caGetArg, "tsid=%s&wlan_ssid=%s&PFrom=%s&client_ip=%s&client_mac=%s&ap_mac=%s&lang@%s", caTsid, caSsid, caAcid, caUserIp, caUserMac, caApMac, caLang);

            // utStrAddF(caPostArg,"<input type=\"hidden\" name=\"%s\" value=\"%s\">","FunName","ncSrvLogin");
            // utStrAddF(caPostArg,"<input type=\"hidden\" name=\"%s\" value=\"%s\">","tsid",caTsid);
            // utStrAddF(caPostArg,"<input type=\"hidden\" name=\"%s\" value=\"%s\">","lang",caLang);
            // utStrAddF(caPostArg,"<input type=\"hidden\" name=\"%s\" value=\"%s\">","wlan_ssid",caSsid);
            // utStrAddF(caPostArg,"<input type=\"hidden\" name=\"%s\" value=\"%s\">","client_ip",caUserIp);
            // utStrAddF(caPostArg,"<input type=\"hidden\" name=\"%s\" value=\"%s\">","client_mac",caUserMac);
            // utStrAddF(caPostArg,"<input type=\"hidden\" name=\"%s\" value=\"%s\">","ap_mac",caApMac);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "f_Qv", caQv);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "f_hs_server", caHserver);
        }
    }
    if(iVendor == 0)
    {
        if(!utStrIsSpaces(utComGetVar_sd(psShmHead, "DefAcName", "\0")))
        {
            strcpy(caAcName, utComGetVar_sd(psShmHead, "DefAcName", "\0"));
            iReturn = utMsgGetSomeNVar(psMsgHead, 5,
                                       "tsid",        UT_TYPE_STRING, 20, caTsid,
                                       "usragent",    UT_TYPE_STRING, 255, caUserAgent,
                                       "clientip",    UT_TYPE_STRING, 23, caUserIp,
                                       "srvname",     UT_TYPE_STRING, 31, caSrvIp,
                                       "srvport",     UT_TYPE_STRING, 7,  caSrvPort);
            if(utStrIsSpaces(caUserIp))
            {
                strcpy(caUserIp, caClientIp);
            }
            lSip = ntohl(pasIpcvtLong(caUserIp));
            if(!utStrIsSpaces(caTsid))
            {
                sscanf(caTsid, "%llu", &llTsid);
                psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
                if(psOnline == NULL)
                {
                    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caNasid, caApMac, lSip, mac, caSsid, caGwip, caGwport, 0);
                }
            }
            else
            {
                psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead, caNasid, caApMac, lSip, mac, caSsid, caGwip, caGwport, 0);
                if(psOnline)
                {
                    sprintf(caTsid, "%llu", psOnline->lTsid);
                    psOnline->lGwip = ntohl(pasIpcvtLong(caGwip));
                    psOnline->nGwport = atol(caGwport);
                }
            }
            sprintf(caGetArg, "wlanacname=%s&tsid=%s&ssid=%s&gw_address=%s&gw_port=%s&gw_id=%s&ip=%s&mac=%s&url=%s",
                    caAcName,
                    caTsid, caSsid, caGwip, caGwport, caGwid, caUserIp, caUserMac, caRedir);
            strcpy(caPostArg, "\0");
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang",   caLang);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip", caUserIp);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "usermac", caUserMac);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "gw_address", caGwip);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "gw_id",     caGwid);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "gw_port",   caGwport);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "redirect",  caRedir);
            utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "tsid",      caTsid);
            if(strcmp(caSrvPort, "443") == 0)
            {
                sprintf(caPostUrl, "https://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
            }
            else
            {
                sprintf(caPostUrl, "http://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
            }
            iVendor = NCPORTAL_VENDOR_DEFAULT;

        }
    }

    if(iVendor == 0)    // 未知型号
    {
        char *pHref, *p;
        pHref = utMsgGetVar_s(psMsgHead, "httpref");
        if(pHref)
        {
            char caOut[512];
            pasStrUrlUnEsc(pHref, caOut);
            strcpy(caTsid, "\0");
            ncUtlGetWordBetween(caOut, "tsid=", "&", caTsid, 31);
            if(!utStrIsSpaces(caTsid))
            {
                sscanf(caTsid, "%llu", &llTsid);
                psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
                pasLogs(PAS_SRCFILE, 1008, "Get Tsid From Href=tsid=%llu  psOnline=%lu",
                        llTsid, psOnline);
                if(psOnline)
                {
                    iVendor = psOnline->cFrom;
                }
            }
        }
        if(psOnline == NULL)    // 不支持
        {
            return 0;
        }
        else
        {
            ncPortalAcInfo *psAc;
            char caErr[32], caAcid[32];
            iVendor = psOnline->cFrom;
            switch(iVendor)
            {
                case NCPORTAL_VENDOR_GBCOM:
                    sprintf(caGetArg, "wlanacname@%s&wlanuserip@%s&ssid@%s&usermac@%s",
                            psOnline->caAcName,
                            caUserIp,
                            psOnline->caSsid,
                            pasCvtMac(psOnline->caMac));
                    strcpy(caPostArg, "\0");
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang", psOnline->caLang);
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip", caUserIp);
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "usermac", pasCvtMac(psOnline->caMac));
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "ssid", psOnline->caSsid);
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "wlanacname", psOnline->caAcName);
                    strcpy(caPostUrl, psOnline->caPostUrl);
                    break;
                case NCPORTAL_VENDOR_CISCO:
                    sprintf(caGetArg, "wlan@%s&switch_url@%s&ap_mac@%s&client_mac@%s",
                            psOnline->caSsid, psOnline->caPostUrl, psOnline->caApName, pasCvtMac(psOnline->caMac));
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "buttonClicked", "4");
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "err_flag", "0");
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "err_msg", "\0");
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "info_msg", "\0");
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "ap_mac", psOnline->caApName);
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang", psOnline->caLang);
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "switch_url", psOnline->caPostUrl);
                    strcpy(caPostUrl, psOnline->caPostUrl);
                    break;
                case NCPORTAL_VENDOR_ARUBA:
                    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                                               "errmsg",      UT_TYPE_STRING, 15,   caErr,
                                               "acid",        UT_TYPE_STRING, 15,   caAcid);
                    if(!utStrIsSpaces(caErr))
                    {
                        strcpy(caStatus, "5");
                    }
                    psAc = (ncPortalAcInfo *)ncSrvGetAcInfo(psShmHead, caAcid);
                    if(psAc)
                    {
                        strcpy(caPostUrl, psAc->caPost);
                    }
                    if(utStrIsSpaces(caPostUrl))
                    {
                        strcpy(caPostUrl, "http://securelogin.arubanetworks.com/auth/index.html/u");
                    }
                    sprintf(caGetArg, "cmd=login&tsid=%llu", psOnline->lTsid);
                    if(!utStrIsSpaces(psOnline->caSsid))
                    {
                        utStrAddF(caGetArg, "&essid=%s", psOnline->caSsid);
                    }
                    if(!utStrIsSpaces(caErr))
                    {
                        utStrAddF(caGetArg, "&errmsg=%s", caErr);
                    }
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
                    utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "essid", caSsid);
                    break;
            }
        }
    }
    if(iVendor > 0)
    {
        int iCode;
        char caGroup[32];
        ncPortalStype *psType;
        // 检查用户是否在黑名单中
        iCode = 0;
        if(strcasecmp(caUserAgent, "PROEIM") == 0)   // APP认证
        {
            ncSrvAppDoGetIp(psShmHead, iFd, psMsgHead, psOnline);
            return 0;
        }
        if(psOnline)
        {
            if(!utStrIsSpaces(caTsid))
            {
                pasSetCookies("PASTSID", caTsid, "/", NULL, NULL);
            }
            if(!utStrIsSpaces(caLang))
            {
                if(strcmp(caLang, psOnline->caLang) != 0)
                {
                    utStrncpy(psOnline->caLang, caLang, 7);
                }
            }
            else if(!utStrIsSpaces(psOnline->caLang))
            {
                utStrncpy(caLang, psOnline->caLang, 7);
            }

            if(psOnline->psUser == NULL || memcmp(psOnline->caMac, mac, 6) != 0)
            {
                ncPortalUser *psUser;
                memcpy(psOnline->caMac, mac, 6);
                psUser = (ncPortalUser *)ncSrvGetUserByMac(psShmHead, psOnline->lStype, mac);
                if(psUser)
                {
                    psOnline->psUser = psUser;
                    strcpy(psOnline->caName, psUser->caName);
                }
            }

            if(!utStrIsSpaces(caSsid))
            {
                if(strcmp(psOnline->caSsid, caSsid) != 0)
                {
                    psOnline->psSsid = ncSrvGetSsidInfo(psShmHead, psOnline->lGroupid, caSsid);
                    if(psOnline->psSsid)
                    {
                        psOnline->psPar = psOnline->psSsid->psPar;
                    }
                    utStrncpy(psOnline->caSsid, caSsid, 31);
                }
            }
            else
            {
                strcpy(caSsid, psOnline->caSsid);
            }
            if(psOnline->psAp && psOnline->psAp->psGroup)
            {
                strcpy(caGroup, psOnline->psAp->psGroup->caGroupName);
            }
            else
            {
                strcpy(caGroup, "\0");
            }
            psType = (ncPortalStype *)ncSrvGetStypeInfoByName(psShmHead, caGroup, caSsid);
            if(psType)
            {
                psOnline->lStype = psType->stype;
                if(utStrIsSpaces(caGroup))
                {
                    strcpy(caGroup, psType->ucode);
                }
            }
            else
            {
                psOnline->lStype = 0;
            }
            pasLogs(PAS_SRCFILE, 1008, "Mac:%s Ssid %s Group:%s Stype=%u", pasCvtMac(psOnline->caMac), psOnline->caSsid, caGroup, psOnline->lStype);
            if(!utStrIsSpaces(caUserAgent))
            {
                strcpy(caOs, "\0");
                strcpy(caType, "\0");
                strcpy(caBro, "\0");
                strcpy(caTerm, "\0");
                iRet = pasUtlTermAttr(caUserAgent, caTerm, caOs, caBro, caType);
                pasLogs(PAS_SRCFILE, 1009, "[%d %-8s %-8s %-8s %-8s ]%s\n", iRet, caType, caTerm, caOs, caBro, caUserAgent);
                if(!utStrIsSpaces(caTerm))
                {
                    if(strcmp(psOnline->caDev, caTerm) != 0)
                    {
                        utStrncpy(psOnline->caDev, caTerm, 23);
                    }
                }
                if(!utStrIsSpaces(caOs))
                {
                    if(strcmp(psOnline->caOs, caOs) != 0)
                    {
                        utStrncpy(psOnline->caOs, caOs, 15);
                    }
                }
                if(!utStrIsSpaces(caType))
                {
                    if(strcmp(psOnline->caTermType, caType) != 0)
                    {
                        utStrncpy(psOnline->caTermType, caType, 7);
                    }
                }
                pPlate = (char *)ncSrvGetPlateInfo(psShmHead, (char *)ncSrvGetPlateName(psShmHead, psOnline), caType, caLang, &iCode);
                if(caStatus[0] == '5')   // 出错
                {
                    char caFile9[128];
                    if(pPlate)
                    {
                        sprintf(caPlate, "%s/login_error.htm", pPlate);
                        sprintf(caFile9, "%s/%s", utPltGetPlatePath(), caPlate);
                        if(!utFileIsExist(caFile9))
                        {
                            sprintf(caPlate, "%s/login_main.htm", pPlate);
                        }
                    }
                    else
                    {
                        sprintf(caPlate, "default/login_error.htm", pPlate);
                        sprintf(caFile9, "%s/%s", utPltGetPlatePath(), caPlate);
                        if(!utFileIsExist(caFile9))
                        {
                            sprintf(caPlate, "default/login_main.htm", pPlate);
                        }
                    }
                }
                else
                {
                    if(pPlate)
                    {
                        sprintf(caPlate, "%s/login_main.htm", pPlate);
                    }
                    else
                    {
                        sprintf(caPlate, "default/login_main.htm", pPlate);
                    }
                }
                pasLogs(PAS_SRCFILE, 1009, "Plate:%s  Code: %d Online=%lu  User=%lu  Ip=%s",
                        caPlate, iCode, psOnline, psOnline ? psOnline->psUser : 0,
                        caUserIp);
            }
            if(!utStrIsSpaces(caPostUrl))
            {
                if(strcmp(psOnline->caPostUrl, caPostUrl) != 0)
                {
                    utStrncpy(psOnline->caPostUrl, caPostUrl, 63);
                }
            }
            if(!utStrIsSpaces(caAcName))
            {
                if(strcmp(psOnline->caAcName, caAcName) != 0)
                {
                    utStrncpy(psOnline->caAcName, caAcName, 31);
                }
            }
            if(!utStrIsSpaces(caNasid))
            {
                if(utStrIsSpaces(psOnline->caApName))
                {
                    utStrncpy(psOnline->caApName, caNasid, 31);
                }
                if(psOnline->psAp == NULL)
                {
                    char caAp[32];
                    memset(caAp, 0, 32);
                    pasCvtStrMac2Mac12(caNasid, caAp);
                    psOnline->psAp = (ncPortalAp *)utShmHashLook(psShmHead, PORTAL_LNK_GROUPAP, caAp);
                }
                if(psOnline->psAp)
                {
                    psOnline->lGroupid = psOnline->psAp->lGroupid;
                    if(psOnline->psAp->lApIp == 0 && caGwip[0])
                    {
                        psOnline->psAp->lApIp = ntohl(pasIpcvtLong(caGwip));
                        psOnline->psAp->nPort = atol(caGwport);
                    }
                }
            }
            psOnline->cFrom = iVendor;
            psOnline->lasttime = time(0);
            if(!utStrIsNull(psOnline->caMac, 6))
            {
                if(psOnline->cStatus != 1)
                {
                    if(ncSrvPortalIsBlackUser(psShmHead, psOnline->lGroupid, NCPORTAL_BLACK_MAC, psOnline->caMac))
                    {
                        pasLogs(PAS_SRCFILE, 1008, "Mac Is In BlackList %s ", pasCvtMac(psOnline->caMac));
                        psOnline->cStatus = 1;   // 在黑名单中
                    }
                }
            }
            if(caStatus[0] == '5')
            {
                if(strcasecmp(caLang, "zh") == 0)
                {
                    char caTemp[256];
                    sprintf(caTemp, "您输入的账号或密码不正确");
                    pasCharsetCvt(PAS_CCODE_GB, iCode, caTemp, caMsg, 255);
                }
                else
                {
                    sprintf(caMsg, "User name or password Error");
                }
            }
            else if(psOnline->cStatus == 1)      // 在黑名单中
            {
                if(strcasecmp(caLang, "zh") == 0)
                {
                    char caTemp[256];
                    sprintf(caTemp, "Mac %s 地址不被允许访问", pasCvtMac(psOnline->caMac));
                    pasCharsetCvt(PAS_CCODE_GB, iCode, caTemp, caMsg, 255);
                }
                else
                {
                    sprintf(caMsg, " Mac %s n ot allowed ", pasCvtMac(psOnline->caMac));
                }
                sprintf(caStatus, "%d", 3);
                sprintf(caPostUrl, psOnline->caPostUrl);
                strcpy(caPostArg, "\0");
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvDispLogin");
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang", psOnline->caLang);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip", caUserIp);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "usermac", pasCvtMac(psOnline->caMac));
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "ssid", psOnline->caSsid);
                utStrAddF(caPostArg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "wlanacname", psOnline->caAcName);
            }
            else
            {
                if(iFlags == 0)
                {
                    // 微信认证检查
                    // 第一次认证，需要进行手机认证,  若已经过认证，手机号已绑定，则不需要认证
                    // WeixinAuth = "Yes" | "Auto"
                    pasLogs(PAS_SRCFILE, 1008, " Weixin Auth is (-1: NoPar 0:No  1:Yes  2: Auto) %d ", psOnline->psPar ? psOnline->psPar->cWeixinAuth : (-1));
                    if(psOnline->psPar && psOnline->psPar->cWeixinAuth > 0)
                    {
                        /* =======================================================================================================================================================
                            以下来自网络督察
                            [24498-7001.1008][14.07.16 09:28:14][ncSrvDispLogin]
                            ======================================================
                            VarName   Bytes  Type      Content
                            from         7STRING  [proeim]
                            wlanacname   7STRING  [test38]
                            wlanuserip  15STRING  [192.168.20.218]
                            ssid         7STRING  [test38]
                            usermac     18STRING  [3c:43:8e:d4:53:92]
                            srvname     14STRING  [192.168.20.29]
                            usragent   192STRING  [Mozilla/5.0 (Linux; U; Android 4.1.2; zh-cn; XT928 Build/6.7.2_GC-383) AppleWebKit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30 MicroMessenger/5.3.1.51_r733746.462 NetType/WIFI]
                            srvport      5STRING  [9080]
                            httpref    127STRING  [http://www.pronetway.com/?openid@ouPAxt3aF3-4bDyFOAwwnUClX_F8&nickname@%E8%80%81%E6%9D%8E&timestamp@1405474192&echostr@abcdefg]
                            clientip    15STRING  [192.168.20.218]
                            requri     131STRING  [/pronline/Msg?FunName@ncSrvDispLogin&from@proeim&wlanacname@test38&wlanuserip@192.168.20.218&gwid@test38&usermac@3c:43:8e:d4:53:92]


                            以下经过Portal转发
                            [26559-7001.1008][14.09.30 10:07:43][ncSrvDispLogin]
                            ======================================================
                            lCode:[990118101]  lBytes:[954]  lMsgId:[266020000] lSendTime:[1412042863]
                            lCheckSum:[0] nVersion:[200] cCrypt:[0] cOrder:[0]
                            nReplyPort:[0]  nFunCode:[1966134858] cFlags:[0] cConfirm:[0]
                            nReverse:[0] l1:[0], l2:[0]
                            ========================================================
                            VarName   Bytes  Type      Content
                            wlanuserip  12STRING  [172.16.5.10]
                            wlanacname  21STRING  [0000.0000.000.00.465]
                            wlanacip    14STRING  [171.8.149.162]
                            ssid        22STRING  [寰峰~L~V?[1m~V拌?[1m~W娆㈣?[1m~N?[1m~B╙
                            nasid       17STRING  [0000000000000460]
                            usermac     13STRING  [3423ba4ef3ae]
                            wlanapmac   13STRING  [0034cb90f7be]
                            srvname     12STRING  [171.8.1.148]
                            usragent   187STRING  [Mozilla/5.0 (Linux; U; Android 4.3; zh-cn; GT-I9300 Build/JSS15J) AppleWebKit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30 MicroMessenger/5.3.1.50_r732663.462 NetType/WIFI]
                            srvport      5STRING  [9080]
                            httpref    112STRING  [http://www.pronetway.com/?ProOpenid@ocF53jubkWx0w8tQsGsxVELAWnQs&nickname@&timestamp@1412043052&echostr@abcdefg]
                            clientip    14STRING  [171.8.149.162]
                            requri     251STRING  [/pronline/Msg?FunName@ncSrvDispLogin&wlanuserip=172.16.5.10&wlanacname=0000.0000.000.00.465&wlanacip=171.8.149.162&ssid=%E5%BE%B7%E5%8C%96%E6%96%B0%E8%A1%97%E6%AC%A2%E8%BF%8E%E6%82%A8&nasid=0000000000000460&usermac=3423ba4ef3ae&wlanapmac=0034cb90f7be]
                            [26559-7001.1008][14.09.30 10:07:43]ncSrvGetOnlineUserByNasidIpA UserIp:172.16.5.10  NasId:0000000000000460
                            [26559-7001.1009][14.09.30 10:07:43][6 Mobile            Android 4.x Safari   ]Mozilla/5.0 (Linux; U; Android 4.3; zh-cn; GT-I9300 Build/JSS15J) AppleWebKit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30 MicroMessenger/5.3.1.50_r732663.462 NetType/WIFI
                            [26559-7001.1009][14.09.30 10:07:43]Plate:dhxjtmp/sms/login_main.htm  Code: 0 Online=3976978828  User=3841966096  Ip=172.16.5.10
                            [26559-7001.1008][14.09.30 10:07:43] Weixin Auth is (-1: NoPar 0:No  1:Yes  2: Auto) -1
                            [26559-7001.1009][14.09.30 10:07:43]Plate:dhxjtmp/sms/login_main.htm  Code: 0 Stype:0  Gtype:0
                            ===========================================================================================================================================================
                        */
                        int iRet = 0;
                        char caOpenid[32], caName[32], caHostr[32];
                        unsigned long lTime;
                        strcpy(caOpenid, "\0");
                        strcpy(caName, "\0");

                        p = utMsgGetVar_s(psMsgHead, "ProOpenid");
                        if(p)
                        {
                            strcpy(caOpenid, p);
                            strcpy(psOnline->caOpenid, caOpenid);
                            iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                                                       "nickname",         UT_TYPE_STRING, 31,   caName,
                                                       "echostr",          UT_TYPE_STRING, 15,   caHostr);
                            pasLogs(PAS_SRCFILE, 1008, "  iReturn=%d　OpenId=%s nickname = %s  echostr=%s",
                                    iReturn, caOpenid, caName, caHostr);
                        }
                        else
                        {
                            p = utMsgGetVar_s(psMsgHead, "httpref");
                            if(p)
                            {
                                iRet = ncSrvGetWeixinAccount(p, caOpenid, caName, &lTime);
                                pasLogs(PAS_SRCFILE, 1008, "Weixin httpref  iRet=%d OpenId=%s Name:%s Time=%u ", iRet, caOpenid, caName, lTime);
                                if(iRet > 0)
                                {
                                    strcpy(psOnline->caOpenid, caOpenid);
                                }
                            }
                        }

                        if(!utStrIsSpaces(caOpenid))   // 微信认证
                        {
                            char caTemp[32];
                            pasStrUrlUnEsc(caName, caTemp);
                            pasCvtGBK(PAS_CCODE_UTF8, caTemp, caName, 31);
                            pasLogs(PAS_SRCFILE, 1008, "  nickname = %s  ", caName);

                            if(psOnline->psPar->cWeixinAuth == 2)   // 自动认证
                            {
                                iReturn = ncSrvThirdUserAuth(psShmHead, iFd, psMsgHead, psOnline,
                                                             caSrvIp, caSrvPort, NCPORTAL_LOGIN_WEIXINAUTH);
                                if(iReturn == 1)
                                {
                                    return 0;
                                }
                            }
                        }
                    }

                    if(psOnline->psUser && psOnline->psPar && psOnline->psPar->cAutoAuth)    // 二次免认证
                    {
                        if(ncSrvMacIsBindUser(psShmHead, psOnline) && strcmp(psOnline->psUser->caPass, "&*%^G12345SA") != 0 && psOnline->cStatus == 0)
                        {
                            if(!ncSrvPortalIsBlackUser(psShmHead, psOnline->lStype, NCPORTAL_BLACK_USER, psOnline->caName))
                            {
                                char caFile1[128], caFile2[128], caUp[64], caUp0[64];
                                sprintf(caUp, "%s,%s", psOnline->psUser->caName, psOnline->psUser->caPass);
                                psDbHead = utPltInitDbHead();
                                ncSrvDesEncryptA(caUp, caUp0, PAS_DEF_KEY);
                                utPltPutVar(psDbHead, "up", caUp0);
                                if(pPlate)
                                {
                                    sprintf(caFile1, "%s/login_main_auto.htm", pPlate);
                                }
                                else
                                {
                                    sprintf(caFile1, "default/login_main_auto.htm");
                                }
                                sprintf(caFile2, "%s/%s", utPltGetPlatePath(), caFile1);
                                if(utFileIsExist(caFile2))
                                {
                                    strcpy(caPlate, caFile1);
                                }
                                if(iUpdateUserFlags == 2 && iAutoUpdateRadius)
                                {
                                    iReturn = ncSrvSynUserPassword2RadSrv(psOnline->psPar, psOnline->psUser->caName, psOnline->psUser->caPass, psOnline->lStype);
                                    pasLogs(PAS_SRCFILE, 1008, "AutoAuth Update User [%s]  Pass:[%s]  Stype:%u Ret=%d ",
                                            psOnline->psUser->caName, psOnline->psUser->caPass, psOnline->lStype, iReturn);
                                }
                                pasLogs(PAS_SRCFILE, 1008, "User AutoAuth User:[%s] Pass:[%s] ",
                                        psOnline->psUser->caName,
                                        psOnline->psUser->caPass);
                                utPltPutVar(psDbHead, "autoauth", "1");
                                utPltPutVar(psDbHead, "username", psOnline->psUser->caName);
                                utPltPutVar(psDbHead, "password", psOnline->psUser->caPass);
                            }
                            else
                            {
                                pasLogs(PAS_SRCFILE, 1008, "User  %s is in blacklist ", psOnline->caName);
                            }
                        }
                    }
                }
            }
            if(psDbHead == NULL)
            {
                psDbHead = utPltInitDbHead();
            }
            if(iFlags == 1)
            {
                char caFname[32];
                caHtml[0] = 0;
                caFname[0] = 0;
                caUp[0] = 0;
                iReturn = utMsgGetSomeNVar(psMsgHead, 3,
                                           "html",        UT_TYPE_STRING, 127, caHtml,
                                           "fname",       UT_TYPE_STRING, 31,  caFname,
                                           "up",          UT_TYPE_STRING, 63,  caUp);
                if(!utStrIsSpaces(caUp) && pasStrIsHexDigital(caUp, strlen(caUp)))
                {
                    char caUp0[64], *p;
                    utPltPutVar(psDbHead, "up", caUp);
                    ncSrvDesDecryptA(caUp, caUp0);
                    p = strstr(caUp0, ",");
                    if(p && *p)
                    {
                        *p = 0;
                        utPltPutVar(psDbHead, "username", caUp0);
                        utPltPutVar(psDbHead, "password", p + 1);
                    }
                }
                pasLogs(PAS_SRCFILE, 1009, "Plate:%s  Code: %d Stype:%u  Gtype:%u", caPlate, iCode, psOnline->lStype, psOnline->lGroupid);
                if(pPlate)
                {
                    sprintf(caPlate, "%s/%s", pPlate, caHtml);
                }
                else
                {
                    sprintf(caPlate, "default/%s", caHtml);
                }
                if(!utStrIsSpaces(caFname))
                {
                    utStrAddF(caGetArg, "&fname@%s", caFname);
                }
            }
            utPltPutVar(psDbHead, "statusCode", caStatus);
            utPltPutVar(psDbHead, "posturl", caPostUrl);
            utPltPutVar(psDbHead, "postarg", caPostArg);
            utPltPutVar(psDbHead, "getarg", caGetArg);
            utPltPutVar(psDbHead, "lang",    psOnline->caLang);
            utPltPutVar(psDbHead, "ip",      caSrvIp);
            utPltPutVar(psDbHead, "port",    caSrvPort);
            utPltPutVar(psDbHead, "userip",  caUserIp);
            utPltPutVar(psDbHead, "redirurl",  caRedir);
            utPltPutVar(psDbHead, "usermac", (char *)pasCvtMac(psOnline->caMac));
            utPltPutVarF(psDbHead, "tsid",   "%llu", psOnline->lTsid);
            if(!utStrIsSpaces(caAreaCode))
            {
                utPltPutVar(psDbHead, "code",    caAreaCode);
            }
            if(!utStrIsSpaces(caUsernameTemp))
            {
                utPltPutVar(psDbHead, "usernametmp", caUsernameTemp);
            }
            utPltPutVar(psDbHead, "message", caMsg);
            pasLogs(PAS_SRCFILE, 1009, "Plate:%s  Code: %d Stype:%u  Gtype:%u", caPlate, iCode, psOnline->lStype, psOnline->lGroupid);

            //丹尼斯免认证输出帐号密码
            char caUsername[32], caPassword[32];
            utMsgGetSomeNVar(psMsgHead, 2,
                             "username",        UT_TYPE_STRING, 31,   caUsername,
                             "password",        UT_TYPE_STRING, 15,   caPassword);
            utPltPutVar(psDbHead, "username", caUsername);
            utPltPutVar(psDbHead, "password", caPassword);
            utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);
        }
        else    // 错误
        {
            if(!utStrIsSpaces(caRedir))
            {
                if(psDbHead == NULL)
                {
                    psDbHead = utPltInitDbHead();
                }
                pasSetHttpHeadStatus(302, "Found");
                pasSetHttpHeadLocation(caRedir);
                pasLogs(PAS_SRCFILE, 1008, "Error,Redir Url:%s ", caRedir);
                utPltOutToHtmlFromBuf(iFd, psMsgHead, psDbHead, "\0");
            }
        }
    }
    return 0;
}


// 认证
int ncSrvLogin(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    ncPortalAcInfo  *psAc = NULL;
    ncPortalStype   *psStype;
    uint8 llTsid;
    int iEnd = 0;
    char caSsid[32];
    char caName[32];
    char caPass[32];
    char caUserType[16];
    char caUserIp[32];
    char caUserMac[32];
    char caClientIp[32];
    char caChapPass[32], *pPlate;
    char caChallenge[32], caName0[32];
    char caGwid[32], caGwip[32], caGwport[8];
    char caAcName[32], caAcIp[32], caSrvIp[32], caSrvPort[10], caNasid[24], caTsid[24];
    char caUrl[256], caUserAgent[256], caPlate[128], caPostUrl[256], caIp1[32], caUserMac1[32];
    char caMsg[4096], caArea[32];
    uint4 lGroupid;
    int iReturn, iRet, iSp, iArea, iStatus = 5, iGmVip = 0;
    utPltDbHead *psDbHead;
    ncPortalOnline *psOnline;
    ncPortalUser *psUser = NULL;
    char caTerm[16], caOs[16], caBro[16], caType[16], caLang[16], caCode[16], caSpname[16], caAcName0[32];
    long long lSid = 0;
    utShmHand sShm;
    int iSum, iCode;
    uint2 nReqId = 0;
    uint4  nPort = 2000;
    utMsgOutMsgToLog(PAS_SRCFILE, 1008, psMsgHead, "[ncSrvLogin] \n");
    strcpy(caSsid, "\0");
    strcpy(caUserIp, "\0");
    strcpy(caName, "\0");
    strcpy(caPass, "\0");
    strcpy(caClientIp, "\0");
    strcpy(caUserMac, "\0");
    strcpy(caUrl, "\0");
    strcpy(caAcName, "\0");
    strcpy(caAcIp, "\0");
    memset(caTsid, 0, 24);
    memset(caNasid, 0, 32);
    memset(caGwid, 0, 32);
    strcpy(caUserType, "\0");
    strcpy(caAcName0, "\0");
    psOnline = NULL;
    iReturn = utMsgGetSomeNVar(psMsgHead, 21,
                               "tsid",       UT_TYPE_STRING, 20, caTsid,
                               "lang",       UT_TYPE_STRING, 15, caLang,
                               "usertype",   UT_TYPE_STRING, 15, caUserType,    // 用户类别，用于页面个性化处理
                               "ssid",       UT_TYPE_STRING, 24, caSsid,
                               "userip",     UT_TYPE_STRING, 24, caUserIp,
                               "username",   UT_TYPE_STRING, 31, caName0,
                               "password",   UT_TYPE_STRING, 31, caPass,
                               "srvname",    UT_TYPE_STRING, 31, caSrvIp,
                               "srvport",    UT_TYPE_STRING, 7,  caSrvPort,
                               "clientip",   UT_TYPE_STRING, 31, caClientIp,
                               "usermac",    UT_TYPE_STRING, 31, caUserMac,
                               "usragent",   UT_TYPE_STRING, 255, caUserAgent,
                               "wlanacname", UT_TYPE_STRING, 31, caAcName,
                               "wlanacip",   UT_TYPE_STRING, 31, caAcIp,
                               "pacname",    UT_TYPE_STRING, 31, caAcName0,
                               "gw_id",      UT_TYPE_STRING, 31, caGwid,
                               "gw_address", UT_TYPE_STRING, 15, caGwip,
                               "gw_port",    UT_TYPE_STRING, 15, caGwport,
                               "ip",         UT_TYPE_STRING, 24, caIp1,
                               "mac",        UT_TYPE_STRING, 31, caUserMac1,
                               "nasid",      UT_TYPE_STRING, 23, caNasid,
                               "redirect",   UT_TYPE_STRING, 255, caUrl);
    if(utStrIsSpaces(caUserIp))
    {
        strcpy(caUserIp, caIp1);
    }
    if(utStrIsSpaces(caUserIp))
    {
        strcpy(caUserIp, caClientIp);
    }

    if(utStrIsSpaces(caUserMac))
    {
        strcpy(caUserMac, caUserMac1);
    }
    if(!utStrIsSpaces(caAcName0))
    {
        strcpy(caAcName, caAcName0);
    }
    iReturn = ncPortalCheckMobileNumber(caName0, caName, caArea, caSpname, &iArea, &iSp);
    if(iReturn <= 0)
    {
        strcpy(caName, caName0);
    }
    if(caNasid[0] == 0)
    {
        if(caGwid[0] != 0)
        {
            strcpy(caNasid, caGwid);
        }
        else if(caAcName[0] != 0)
        {
            strcpy(caNasid, caAcName);
        }
        else if(caAcIp[0] != 0)
        {
            strcpy(caNasid, caAcIp);
        }
    }

    if(!utStrIsSpaces(caTsid))
    {
        sscanf(caTsid, "%llu", &llTsid);
        psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
    }
    if(psOnline == NULL)
    {
        iStatus = NCSRV_STATUSCODE_PAGEEXPIRE;
        /*
        if(!utStrIsSpaces(caUserIp)) {
            uint4 lSip;
            lSip = ntohl(pasIpcvtLong(caUserIp));
            psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIp(psShmHead,caNasid,lSip);
            pasLogs(PAS_SRCFILE,1008,"ncSrvGetOnlineUserByNasidIp NasId:%s Sip:%s  Return=%lu",caNasid,caUserIp,(unsigned long)psOnline);
            if(psOnline == NULL) {
                char mac[6];
                pasCvtMacI(caUserMac,mac);
                psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead,caNasid,caNasid,lSip,mac,caSsid,"\0","\0",0);
                pasLogs(PAS_SRCFILE,1008,"ncSrvGetOnlineUserByNasidIpA NasId:%s Sip:%s  Return=%lu",caNasid,caUserIp,(unsigned long)psOnline);
            }
        }
        */
    }
    /*
    if(psOnline == NULL) {
        if(!utStrIsSpaces(caUserMac)) {
            char mac[6];
            pasCvtMacI(caUserMac,mac);
            psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByMac(psShmHead,mac);
            pasLogs(PAS_SRCFILE,1008,"ncSrvGetOnlineUserByMac UserMac:%s Online=%lu",caUserMac,(unsigned long)psOnline);
        }
    }
    */
    psDbHead = utPltInitDbHead();
    // 检查黑名单
    iEnd = 0;
    if(psOnline)
    {
        lGroupid = psOnline->lStype;
        pasLogs(PAS_SRCFILE, 1008, "psOnline Stype=%u", lGroupid);
    }
    else
    {
        lGroupid = 0;
        pasLogs(PAS_SRCFILE, 1008, "psOnline is NULL");
    }
    if(psOnline)
    {
        if(utStrIsSpaces(caUserMac))
        {
            strcpy(caUserMac, pasCvtMac(psOnline->caMac));
        }
        psStype = (ncPortalStype *)ncSrvGetStypeById(psShmHead, lGroupid);
        psUser = (ncPortalUser *)ncSrvGetUserByName(psShmHead, caName, lGroupid);
        iReturn = 9;
        if(psUser)
        {
            pasLogs(PAS_SRCFILE, 1008, "ncSrvGetUserByName UserStatus=%d ExcessMac=%d SumMac=%d MaxMac=%d",
                    psUser->cStatus, psUser->psPar->cExceedMac, psUser->cSum, psUser->cMaxMac);

            if(psUser->cStatus != 0)
            {
                iStatus = NCSRV_STATUSCODE_EXPIRE;
            }
            if(psUser->psPar && ncSrvIsTimeFlowCtl())
            {
                if(psUser->psPar->lLimittime > 0 && psUser->lSumConnTime > psUser->psPar->lLimittime)    // 超出每天上网时间
                {
                    iStatus = NCSRV_STATUSCODE_EXCEEDTIME;
                    iEnd = 1;
                    iReturn = 1;
                }
                if(psUser->psPar->limitflow > 0 && psUser->lSumBytes / 1024 > psUser->psPar->limitflow)    // 超出每天上网流量
                {
                    iStatus = NCSRV_STATUSCODE_EXCEEDFLOW;
                    iEnd = 1;
                    iReturn = 1;
                }
                if(psUser->psPar->limitlogin > 0 && psUser->lSumLogin >= psUser->psPar->limitlogin)    // 超出每天上网流量
                {
                    iStatus = NCSRV_STATUSCODE_EXCEEDLOGIN;
                    iEnd = 1;
                    iReturn = 1;
                }
            }
        }
        else
        {
            iStatus = NCSRV_STATUSCODE_NOEXIST;
            iEnd = 1;
            pasLogs(PAS_SRCFILE, 1008, "User %s no exist Stype=%u", caName, lGroupid);
        }
        psAc = NULL;
        if(!iEnd)
        {
            if(ncSrvPortalIsBlackUser(psShmHead, psOnline->lStype, NCPORTAL_BLACK_USER, caName))
            {
                iEnd = 1;
                iReturn = 1;
                iStatus = NCSRV_STATUSCODE_BLACKLIST;
                pasLogs(PAS_SRCFILE, 1008, " User %s is in black list ", caName);
            }
            else
            {
                if(psUser && psOnline->psPar && !psOnline->psPar->cSsidRoam && psUser && !utStrIsSpaces(psUser->caSsid) && !utStrIsSpaces(psOnline->caSsid) && strcasecmp(psUser->caSsid, psOnline->caSsid) != 0)
                {
                    iEnd = 1;
                    iReturn = 1;
                    iStatus = NCSRV_STATUSCODE_FORBITLOGIN;
                    pasLogs(PAS_SRCFILE, 1008, " User %s  Ssid is %s and NoIs  %s", caName, psUser->caSsid, psOnline->caSsid);
                }
            }
        }
        if(!iEnd)
        {
            if(psUser && !ncSrvCheckUserMacIsBind(psShmHead, psUser, psOnline->caMac))
            {
                if(psUser->cMaxMac > 0 && psUser->cSum >= psUser->cMaxMac && psUser->psPar->cExceedMac == 1)  // 超出绑定数
                {
                    int iSum;
                    psUser->cSum = ncSrvCountMacBindByName(psShmHead, psUser->lStype, caName);
                    if(psUser->cSum >= psUser->cMaxMac)
                    {
                        iEnd = 1;
                        iReturn = 1;
                        iStatus = NCSRV_STATUSCODE_EXCEEDMAC;
                        pasLogs(PAS_SRCFILE, 1008, " User %s  Mac Bind  %d Exceed MacMac %d, Forbit", caName, psUser->cSum, psUser->cMaxMac);
                    }
                }
            }
            else
            {
                pasLogs(PAS_SRCFILE, 1008, " User %s  Mac %s Already Bind ", caName, pasCvtMac(psOnline->caMac));
            }
        }
        if(iEnd != 1)
        {
            if(utStrIsSpaces(caAcName))
            {
                strcpy(caAcName, psOnline->caAcName);
            }
            psAc = (ncPortalAcInfo *)ncSrvGetAcInfo(psShmHead, caAcName);
        }
        if(psAc)
        {
            // iStatus = 5;
            if(strcasecmp(psAc->caAcType, "Ac") == 0)
            {
                lReqIdNum = psAc->nSn;
                psAc->nSn++;
                if(iPapChap == 0)
                {
                    iReturn = ncSrvSendToAcRequestChallenge(psAc->lAcip, psAc->nAcPort, lReqIdNum, caUserIp, &nReqId, caChallenge);
                    pasLogsHex(PAS_SRCFILE, 1009, caChallenge, 16, "[ncSrvSendToAcRequestChallenge1]");
                    if(iReturn == 0)
                    {
                        iReturn = ncSrvSendToAcRequesChapAuth(psAc->lAcip, psAc->nAcPort, lReqIdNum, nReqId, caUserIp, caName, caChallenge, caPass, caChapPass);
                    }
                }
                else
                {
                    iReturn = ncSrvSendToAcRequesPapAuth(psAc->lAcip, psAc->nAcPort, lReqIdNum, 0, caUserIp, caName, caPass);
                }
                pasLogs(PAS_SRCFILE, 1008, " ACAuth Name:%s   Ip:%s    iRet=%d ", caName, caUserIp, iReturn);
                iEnd = 1;
            }
            else if(strcasecmp(psAc->caAcType, "ProEIM") == 0)
            {
                if(strcasecmp(caUserType, "shgmvip") == 0)
                {
                    iReturn = ncSrvLoginByShGm(psShmHead, psStype ? psStype->ucode : "\0", caSsid, psAc->lAcip, psAc->nAcPort, caNasid, caUserIp, caUserMac, caName, caPass);
                    pasLogs(PAS_SRCFILE, 1008, " Name:%s  Pass:%s  Ip:%s    iRet=%d ", caName, caPass, caUserIp, iReturn);
                    iEnd = 1;
                }
                else
                {
                    iReturn = ncSrvLoginByProEIM(psShmHead, psOnline ? psOnline->lStype : 0, psAc->lAcip, psAc->nAcPort, caNasid, caUserIp, caUserMac, caName, caPass);
                    pasLogs(PAS_SRCFILE, 1008, " Name:%s   Ip:%s    iRet=%d ", caName, caUserIp, iReturn);
                    iEnd = 1;
                }
            }
            else if(strncasecmp(psAc->caAcType, "Meru", 4) == 0)
            {
                char caUsername1[64];
                char *p;
                p = strstr(psAc->caDomain, "@");
                if(p)
                {
                    if(psAc->caDomain[0] == '@')
                    {
                        sprintf(caUsername1, "%s%s", caName, psAc->caDomain);
                    }
                    else
                    {
                        sprintf(caUsername1, "%s%s", psAc->caDomain, caName);
                    }
                }
                else
                {
                    strcpy(caUsername1, caName);
                }
                iReturn = ncSrvUserAuth_MeruHttps(psAc->caPost, caUsername1, caPass, caUserMac, caUserIp, caMsg);
                pasLogs(PAS_SRCFILE, 1008, "[ncSrvUserAuth_MeruHttps] Name:%s  Pass:%s  UsreMac:%s  Ip:%s iRet=%d  Mesg=%s", caUsername1, caPass, caUserMac, caUserIp, iReturn, caMsg);
                iEnd = 1;
            }
            else if(strcasecmp(psAc->caAcType, "Radius") == 0)
            {
                char caUsername1[64];
                char *p;
                psOnline->lAcIp = psAc->lAcip;
                psOnline->nAcPort = psAc->nAcPort;
                strcpy(psOnline->caKey, psAc->caKey);
                psOnline->cAuthType = psAc->iAuthType;
                p = strstr(psAc->caDomain, "@");
                if(p)
                {
                    if(psAc->caDomain[0] == '@')
                    {
                        sprintf(caUsername1, "%s%s", caName, psAc->caDomain);
                    }
                    else
                    {
                        sprintf(caUsername1, "%s%s", psAc->caDomain, caName);
                    }
                }
                else
                {
                    strcpy(caUsername1, caName);
                }
                iReturn = ncSrvLogin_AuthMeru(psShmHead, psOnline, caUserIp, caUsername1, caPass);
                pasLogs(PAS_SRCFILE, 1008, " Radius Name:%s   Ip:%s    iRet=%d ", caUsername1, caUserIp, iReturn);
                if(iReturn == 2)
                {
                    if(psOnline->cFrom == NCPORTAL_VENDOR_MERU)
                    {
                        iRet = ncSrvMeruSshDoLogin(psShmHead, caUserIp, 5, psOnline->caAcName);
                        strcpy(psOnline->caName, caName);
                        psUser = (ncPortalUser *)ncSrvSynUserPassword(psShmHead, psOnline->lStype, psOnline->lGroupid, caName, caPass, "\0", psOnline->caSsid, NCSRV_USERTYPE_SMS);
                        if(psUser)
                        {
                            psOnline->psUser = psUser;
                        }
                        iStatus = 0;
                        pasLogs(PAS_SRCFILE, 1008, " Radius ncSrvMeruSshDoLogin: Ip:%s AcName=%s iRet=%d ", caUserIp, psOnline->caAcName, iRet);
                    }
                    else if(psOnline->cFrom == NCPORTAL_VENDOR_PROEIMV7)
                    {
                        strcpy(psOnline->caName, caName);
                        psUser = (ncPortalUser *)ncSrvSynUserPassword(psShmHead, psOnline->lStype, psOnline->lGroupid, caName, caPass, "\0", psOnline->caSsid, NCSRV_USERTYPE_SMS);
                        if(psUser)
                        {
                            psOnline->psUser = psUser;
                        }
                        iStatus = 0;
                        pasLogs(PAS_SRCFILE, 1008, " Radius ncSrvDoLogin_ProEIMV7: Ip:%s AcName=%s iRet=%d ", caUserIp, psOnline->caAcName, iRet);
                        iEnd = 1;
                    }
                }
                else
                {
                    iStatus = iReturn;
                }
                iEnd = 1;
            }
            else if(strcasecmp(psAc->caAcType, "CenAuth") == 0)
            {
                psOnline->lAcIp = psAc->lAcip;
                psOnline->nAcPort = psAc->nAcPort;
                strcpy(psOnline->caKey, psAc->caKey);
                psOnline->cAuthType = psAc->iAuthType;
                iReturn = ncSrvLogin_AuthCenter(psShmHead, psOnline, caUserIp, caName, caPass);
                if(iReturn == 2)
                {
                    strcpy(psOnline->caName, caName);
                    psUser = (ncPortalUser *)ncSrvSynUserPassword(psShmHead, psOnline->lStype, psOnline->lGroupid, caName, caPass, "\0", psOnline->caSsid, NCSRV_USERTYPE_SMS);
                    if(psUser)
                    {
                        psOnline->psUser = psUser;
                    }
                    ncSrvUpdateOnlineUserByOnline(psShmHead, psOnline, caName, caName);
                    iStatus = 0;
                }
                else
                {
                    iStatus = iReturn;
                }
                pasLogs(PAS_SRCFILE, 1008, " CenterAuth Name:%s   Ip:%s    iRet=%d ", caName, caUserIp, iReturn);
                iEnd = 1;
            }
        }
        if(iEnd == 0)
        {
            if(psOnline->cFrom == NCPORTAL_VENDOR_GBAP || psOnline->cFrom == NCPORTAL_VENDOR_WIFIDOG || psOnline->cFrom == NCPORTAL_VENDOR_BCTH || psOnline->cFrom == NCPORTAL_VENDOR_PROEIMV7)
            {
                iReturn = ncSrvLoginByLocal(psShmHead, psOnline->lStype, 0, 0, caUserIp, caUserMac, caName, caPass);
                if(iReturn == 2 || iReturn == 0)
                {
                    ncSrvUpdateOnlineUserByOnline(psShmHead, psOnline, caName, caName);
                }
                pasLogs(PAS_SRCFILE, 1008, "【ncSrvLoginByLocal】 Name:%s   Ip:%s    iRet=%d From:%d ", caName, caUserIp, iReturn, psOnline->cFrom);
                iEnd = 1;
            }
        }
        if(iReturn == 0 || iReturn == 2)  // 认证成功
        {
            if(pasLogsIsOut(PAS_SRCFILE, 1088, NULL))
            {
                printf(" Pid==%d\n", getpid());
                sleep(10);
            }
            psOnline->cAuthWay = NCPORTAL_LOGIN_WEBAUTH;
            iStatus = 0;
            utPltPutVar(psDbHead, "lang",    caLang);
            sprintf(caMsg, "tsid@%s&wlanacname@%s&userip@%s&ssid@%s&usermac@%s&username@%s", caTsid, caAcName, caUserIp, caSsid, caUserMac, caName);
            utPltPutVar(psDbHead, "getarg", caMsg);

            utPltPutVar(psDbHead, "ip",    caSrvIp);
            utPltPutVar(psDbHead, "port",  caSrvPort);
            utPltPutVar(psDbHead, "userip", caUserIp);
            utPltPutVar(psDbHead, "usermac", caUserMac);
            utPltPutVar(psDbHead, "username", caName);
            utPltPutVar(psDbHead, "tsid", caTsid);
            utPltPutVar(psDbHead, "wlanacname",  caAcName);
            utPltPutVar(psDbHead, "statusCode", "0");
            strcpy(caMsg, "\0");
            utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
            utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang", caLang);
            utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip", caUserIp);
            utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "usermac", caUserMac);
            utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "ssid", caSsid);
            utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "tsid", caTsid);
            utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "wlanacname", caAcName);
            utPltPutVar(psDbHead, "postarg", caMsg);
            if(psAc && strcasecmp(psAc->caAcType, "Ac") == 0)
            {
                iReturn = ncSrvSendToAcAuthAck(psAc->lAcip, psAc->nAcPort, lReqIdNum, nReqId, caUserIp);
            }
            else if(psOnline->cFrom == NCPORTAL_VENDOR_GBAP)
            {
                char caHost[64], caUrl1[512];
                // http://115.29.172.248/cmps/admin.php/api/quicklogin1?gw_address=192.168.20.1&gw_port=8060&gw_id=hotspot2&ip=192.168.20.133&mac=1C:4B:D6:8F:BE:4A&url=http%3A//www.baidu.com/
                //                sprintf(caHost,"%s:%u",utComHostIp(htonl(psAc->lAcip)),psAc->nAcPort);
                sprintf(caHost, "%s:%u", "www.login-wifi.com", psAc->nAcPort);
                sprintf(caUrl1, "http://%s/cmps/admin.php/api/quicklogin1?gw_address=%s&gw_port=%s&gw_id=%s&ip=%s&mac=%s&url=%s",
                        caHost, caGwip, caGwport, caGwid, caUserIp, caUserMac, caUrl);
                utPltPutVar(psDbHead, "reurl", caUrl1);
                pasLogs(PAS_SRCFILE, 1008, "【GBAPl】 Return=%d Plate:%s",
                        iReturn, "nc/portalredir.htm");
                if(iReturn == 2)
                {
                    utPltOutToHtml(iFd, psMsgHead, psDbHead, "nc/portalredir.htm");
                    return 0;
                }
                //                    ncSrvSendAck2Gbcom(caHost,caUrl1);
            }
            else if(psOnline->cFrom == NCPORTAL_VENDOR_WIFIDOG || psOnline->cFrom == NCPORTAL_VENDOR_BCTH)
            {
                char caToken[48];
                if(psOnline)
                {
                    sprintf(caToken, "%llu", psOnline->lTsid);
                    psOnline->cAuthWay = NCPORTAL_LOGIN_WEBAUTH;
                }
                if(psOnline->psPar && psOnline->psPar->cWeixinAuth == 1 && !utStrIsSpaces(psOnline->caOpenid))
                {
                    if(psOnline->psUser)    //  绑定
                    {
                        if(strcmp(psOnline->psUser->caOpenId, psOnline->caOpenid) != 0)
                        {
                            strcpy(psOnline->psUser->caOpenId, psOnline->caOpenid);
                            pasLogs(PAS_SRCFILE, 1008, "【WIFIDOG】 Bind Weixin %s", psOnline->caOpenid);
                            psOnline->psUser->cMod = 1;
                        }
                    }
                }
                pasLogs(PAS_SRCFILE, 1008, "【WIFIDOG】 Return=%d Token:%s", iReturn, caToken);
                ncSrvGBAuthRedir(psShmHead, iFd, psMsgHead, caToken, psOnline->cFrom);
                utPltFreeDb(psDbHead);
                return 0;
            }
            else if(psOnline->cFrom == NCPORTAL_VENDOR_PROEIMV7)
            {
                char caUrl1[512];
                char caUserIp[32];
                char caTemp[512];
                char caApMac[64], caSsid[64];
                char caRssi[16];
                ncLbsMacLocation *psMac;
                strcpy(caUserIp, utComHostIp(htonl(psOnline->lSip)));
                strcpy(caUserMac, pasStrEncodeDesHex(pasCvtMac(psOnline->caMac), caTemp, PAS_DEF_KEY));
                strcpy(caName, pasStrEncodeDesHex(psOnline->caName, caTemp, PAS_DEF_KEY));
                strcpy(caApMac, pasStrEncodeDesHex(psOnline->caApName, caTemp, PAS_DEF_KEY));
                strcpy(caSsid, pasStrEncodeDesHex(psOnline->caSsid, caTemp, PAS_DEF_KEY));
                if(strcmp(caSrvPort, "443") == 0)
                {
                    sprintf(caUrl, "https://%s:%s/pronline/Msg?FunName=ncSrvLoginOK&tsid=%s", caSrvIp, caSrvPort, caTsid);
                }
                else
                {
                    sprintf(caUrl, "http://%s:%s/pronline/Msg?FunName=ncSrvLoginOK&tsid=%s", caSrvIp, caSrvPort, caTsid);
                }
                pasStrEncodeDesHex(caUrl, caTemp, PAS_DEF_KEY);
                sprintf(caUrl1, "http://%s:%u/proauth/Msg?FunName@ncWebPortalDoLogin&username=%s&userip=%s&mac=%s&apmac=%s&ssid=%s&url=%s",
                        utComHostIp(htonl(psOnline->lGwip)),
                        psOnline->nGwport,
                        caName,
                        caUserIp,
                        caUserMac,
                        caApMac,
                        caSsid,
                        caTemp);
                psMac = (ncLbsMacLocation *)ncSrvGetMacLogByMac(psShmHead, psOnline->caMac);
                if(psMac)
                {
                    utStrAddF(caUrl1, "&rssi=%d", psMac->urssi);
                }
                if(!utStrIsSpaces(psOnline->caOpenid))
                {
                    utStrAddF(caUrl1, "&openid=%s", psOnline->caOpenid);
                }
                pasSetHttpHeadStatus(302, "Found");
                pasSetHttpHeadLocation(caUrl1);
                pasLogs(PAS_SRCFILE, 1008, "User:%s  Red:%s ", psOnline->caName, caUrl1);
                ncSrvUpdateOnlineUserByOnline(psShmHead, psOnline, psOnline->caName, psOnline->caName);
                utPltOutToHtmlFromBuf(iFd, psMsgHead, psDbHead, "\0");
                return 0;
            }

            caTerm[0] = 0;
            caOs[0] = 0;
            caBro[0] = 0;
            caType[0] = 0;
            iRet = pasUtlTermAttr(caUserAgent, caTerm, caOs, caBro, caType);
            pasLogs(PAS_SRCFILE, 1009, "[%d %-8s %-8s %-8s %-8s ]%s\n", iRet, caType, caTerm, caOs, caBro, caUserAgent);
            pPlate = ncSrvGetPlateInfo(psShmHead, ncSrvGetPlateName(psShmHead, psOnline), caType, caLang, &iCode);
            if(pPlate)
            {
                sprintf(caPlate, "%s/login_ok.htm", pPlate);
            }
            else
            {
                sprintf(caPlate, "default/login_ok.htm");
            }
            if(strcmp(caSrvPort, "443") == 0)
            {
                sprintf(caPostUrl, "https://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
            }
            else
            {
                sprintf(caPostUrl, "http://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
            }
            if(psOnline)
            {
                iReturn = ncSrvUpdateOnlineUserByOnline(psShmHead, psOnline, caName, "\0");
                psOnline->login = NCPORTAL_ONLINE_LOGIN;
                psOnline->logintime = time(0);
                psOnline->lasttime = time(0);
                psOnline->cAuthWay = NCPORTAL_LOGIN_WEBAUTH;
                pasLogs(PAS_SRCFILE, 1009, "ncSrvUpdateOnlineUserByOnline Name:%s iReturn=%d\n", caName, iReturn);
            }
            utPltPutVar(psDbHead, "posturl", caPostUrl);
            pasLogs(PAS_SRCFILE, 1009, "Plate=%s\n", caPlate);
            pasUtlOut2DbFile("ncsrvloginlog", 15,
                             "sid",       UT_TYPE_LONG8,  lSid,                            // 自动增加
                             "servicecode", UT_TYPE_STRING, caServicecode,
                             "did",        UT_TYPE_ULONG, 0,
                             "gid",        UT_TYPE_ULONG, 0,
                             "userid",     UT_TYPE_ULONG,  0,                               //
                             "username",   UT_TYPE_STRING, caName,
                             "usermac",    UT_TYPE_STRING, caUserMac,
                             "ip",         UT_TYPE_STRING, caUserIp,
                             "logtime",    UT_TYPE_ULONG,  time(0),
                             "platename",  UT_TYPE_STRING, caPlate,
                             "fun",        UT_TYPE_STRING, "login",
                             "dev",        UT_TYPE_STRING, caTerm,
                             "os",         UT_TYPE_STRING, caOs,
                             "devtype",    UT_TYPE_STRING, caType,
                             "status",     UT_TYPE_ULONG,   1);
            utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);
        }
        else    // 认证失败
        {
            char caFile9[128];
            sprintf(caMsg, "tsid@%s&wlanacname@%s&wlanuserip@%s&ssid@%s&usermac@%s&username@%s", caTsid, caAcName, caUserIp, caSsid, caUserMac, caName);
            utPltPutVar(psDbHead, "getarg", caMsg);
            strcpy(caMsg, "\0");
            utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "FunName", "ncSrvLogin");
            utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang", caLang);
            utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip", caUserIp);
            utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "usermac", caUserMac);
            utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "ssid", caSsid);
            utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "tsid", caTsid);
            if(!utStrIsSpaces(caAcName))
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "wlanacname", caAcName);
            utPltPutVar(psDbHead, "postarg", caMsg);
            utPltPutVar(psDbHead, "lang",  caLang);
            utPltPutVar(psDbHead, "tsid",  caTsid);
            utPltPutVar(psDbHead, "ip",    caSrvIp);
            utPltPutVar(psDbHead, "port",  caSrvPort);
            utPltPutVar(psDbHead, "userip", caUserIp);
            utPltPutVar(psDbHead, "usermac", caUserMac);
            utPltPutVar(psDbHead, "username", caName);
            utPltPutVar(psDbHead, "wlanacname",  caAcName);
            utPltPutVarF(psDbHead, "statusCode", "%d", iStatus);
            iRet = pasUtlTermAttr(caUserAgent, caTerm, caOs, caBro, caType);
            pasLogs(PAS_SRCFILE, 1009, "[%d %-8s %-8s %-8s %-8s ]%s\n", iRet, caType, caTerm, caOs, caBro, caUserAgent);
            pPlate = ncSrvGetPlateInfo(psShmHead, ncSrvGetPlateName(psShmHead, psOnline), caType, caLang, &iCode);

            if(pPlate)
            {
                sprintf(caPlate, "%s/login_error.htm", pPlate);
                sprintf(caFile9, "%s/%s", utPltGetPlatePath(), caPlate);
                if(!utFileIsExist(caFile9))
                {
                    sprintf(caPlate, "%s/login_main.htm", pPlate);
                }
            }
            else
            {
                sprintf(caPlate, "default/login_error.htm", pPlate);
                sprintf(caFile9, "%s/%s", utPltGetPlatePath(), caPlate);
                if(!utFileIsExist(caFile9))
                {
                    sprintf(caPlate, "default/login_main.htm", pPlate);
                }
            }
            if(strcasecmp(caLang, "zh") == 0)
            {
                char caTemp[256];
                sprintf(caTemp, "您输入的账号或密码不正确");
                pasCharsetCvt(PAS_CCODE_GB, iCode, caTemp, caMsg, 255);
            }
            else
            {
                sprintf(caMsg, "User name or password Error");
            }
            if(strcmp(caSrvPort, "443") == 0)
            {
                sprintf(caPostUrl, "https://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
            }
            else
            {
                sprintf(caPostUrl, "http://%s:%s/pronline/Msg", caSrvIp, caSrvPort);
            }
            utPltPutVar(psDbHead, "posturl", caPostUrl);
            utPltPutVar(psDbHead, "message", caMsg);
            pasLogs(PAS_SRCFILE, 1009, "Plate=%s Message=%s\n", caPlate, caMsg);
            pasUtlOut2DbFile("ncsrvloginlog", 15,
                             "sid",       UT_TYPE_LONG8,  lSid,                            // 自动增加
                             "servicecode", UT_TYPE_STRING, caServicecode,
                             "did",        UT_TYPE_ULONG, 0,
                             "gid",        UT_TYPE_ULONG, 0,
                             "userid",     UT_TYPE_ULONG,  0,                               //
                             "username",   UT_TYPE_STRING, caName,
                             "usermac",    UT_TYPE_STRING, caUserMac,
                             "ip",         UT_TYPE_STRING, caUserIp,
                             "logtime",    UT_TYPE_ULONG,  time(0),
                             "platename",  UT_TYPE_STRING, caPlate,
                             "fun",        UT_TYPE_STRING, "login",
                             "dev",        UT_TYPE_STRING, caTerm,
                             "os",         UT_TYPE_STRING, caOs,
                             "devtype",    UT_TYPE_STRING, caType,
                             "status",     UT_TYPE_ULONG,   0);
            utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);
        }
    }
    else
    {
        char caFile9[128];
        strcpy(caMsg, "\0");
        utPltPutVar(psDbHead, "tsid",  caTsid);
        utPltPutVar(psDbHead, "ip",    caAdminIp);
        utPltPutVar(psDbHead, "port",  caAdminPort);
        utPltPutVar(psDbHead, "userip", caUserIp);
        utPltPutVar(psDbHead, "usermac", caUserMac);
        utPltPutVar(psDbHead, "username", caName);
        utPltPutVar(psDbHead, "wlanacname",  caAcName);
        iRet = pasUtlTermAttr(caUserAgent, caTerm, caOs, caBro, caType);
        pasLogs(PAS_SRCFILE, 1009, "11[%d %-8s %-8s %-8s %-8s ]%s\n", iRet, caType, caTerm, caOs, caBro, caUserAgent);
        pPlate = ncSrvGetPlateInfo(psShmHead, ncSrvGetPlateName(psShmHead, psOnline), caType, caLang, &iCode);
        if(pPlate)
        {
            sprintf(caPlate, "%s/login_error.htm", pPlate);
            sprintf(caFile9, "%s/%s", utPltGetPlatePath(), caPlate);
            if(!utFileIsExist(caFile9))
            {
                sprintf(caPlate, "%s/login_main.htm", pPlate);
            }
        }
        else
        {
            sprintf(caPlate, "default/login_error.htm", pPlate);
            sprintf(caFile9, "%s/%s", utPltGetPlatePath(), caPlate);
            if(!utFileIsExist(caFile9))
            {
                sprintf(caPlate, "default/login_main.htm", pPlate);
            }
        }
        if(strcasecmp(caLang, "zh") == 0)
        {
            char caTemp[256];
            sprintf(caTemp, "页面过期或不正确");
            pasCharsetCvt(PAS_CCODE_GB, iCode, caTemp, caMsg, 255);
        }
        else
        {
            sprintf(caMsg, "Page Error");
        }
        utPltPutVarF(psDbHead, "statusCode", "%d", iStatus);
        utPltPutVar(psDbHead, "message", caMsg);
        pasLogs(PAS_SRCFILE, 1009, "11Plate=%s Message=%s\n", caPlate, caMsg);
        pasUtlOut2DbFile("ncsrvloginlog", 15,
                         "sid",       UT_TYPE_LONG8,  lSid,                            // 自动增加
                         "servicecode", UT_TYPE_STRING, caServicecode,
                         "did",        UT_TYPE_ULONG, 0,
                         "gid",        UT_TYPE_ULONG, 0,
                         "userid",     UT_TYPE_ULONG,  0,                               //
                         "username",   UT_TYPE_STRING, caName,
                         "usermac",    UT_TYPE_STRING, caUserMac,
                         "ip",         UT_TYPE_STRING, caUserIp,
                         "logtime",    UT_TYPE_ULONG,  time(0),
                         "platename",  UT_TYPE_STRING, caPlate,
                         "fun",        UT_TYPE_STRING, "login",
                         "dev",        UT_TYPE_STRING, caTerm,
                         "os",         UT_TYPE_STRING, caOs,
                         "devtype",    UT_TYPE_STRING, caType,
                         "status",     UT_TYPE_ULONG,   iStatus);
        utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);
    }
    return 0;
}




// 强制离线
int ncSrvLogOut(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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


// 获取短信密码
int ncSrvGetPass(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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

	insertPassToShm(caPass, caName);


    if(psOnline)
    {
        psOnline->psUser = ncSrvSynUserPassword(psShmHead, psOnline->lStype, psOnline->lGroupid, caName, caPass, psOnline->caGroupCode, psOnline->caSsid, NCSRV_USERTYPE_SMS);
        if(iUpdateUserFlags == 2)
        {
            ncSrvSynUserPassword2RadSrv(psOnline->psPar, caName, caPass, psOnline->lStype);
            pasLogs(PAS_SRCFILE, 1008, " Update User [%s]  Pass:[%s] Group:%s Ssid:%s  Stype:%u Ret=%d ",
                    caName, caPass, psOnline->caGroupCode, psOnline->caSsid, psOnline->lStype, iReturn);
        }
        strcpy(caGroupCode, psOnline->caGroupCode);
        if(utStrIsSpaces(caUserIp))
        {
            strcpy(caUserIp, utComHostIp(htonl(psOnline->lSip)));
        }
    }
    else
    {
        ncSrvSynUserPassword(psShmHead, 0, 0, caName, caPass, "\0", utComGetVar_sd(psShmHead, "DefSsid", "\0"), NCSRV_USERTYPE_SMS);
        if(iUpdateUserFlags == 2)
        {
            ncSrvSynUserPassword2RadSrv(psSumm->psPar, caName, caPass, 0);
            pasLogs(PAS_SRCFILE, 1008, " Update User [%s]  Pass:[%s] iReturn=%d ",
                    caName, caPass, iReturn);
        }
    }
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



int ncSrvUpdateUser(utShmHead *psShmHead, char *pUser, char *pPass, char *pGroup)
{
    pasDbConn    *psConn;
    char caDbname[32], caDbpass[32], caDbuser[32];
    char caSmt[256];
    int  iReturn;

    strcpy(caDbname, utComGetVar_sd(psShmHead, "RadDbName", "radius"));
    strcpy(caDbpass, utComGetVar_sd(psShmHead, "RadDbPass", "radius"));
    strcpy(caDbuser, utComGetVar_sd(psShmHead, "RadDbuser", "radius"));

    psConn = pasDbConnect(caDbuser, caDbpass, caDbname);
    if(psConn)
    {
        sprintf(caSmt, "insert into radcheck (username,attribute,op,value) values ('%s','User-Password',':=','%s')",
                pUser, pPass);
        iReturn = pasDbExecSqlF("%s", caSmt);
        if(iReturn != 0)
        {
            sprintf(caSmt, "update radcheck  set value = '%s' where username = '%s' and attribute = 'User-Password'",
                    pPass, pUser);
            iReturn = pasDbExecSqlF("%s", caSmt);
        }
        else
        {
            sprintf(caSmt, "insert into radusergroup (username,groupname) values ('%s','%s')",
                    pUser, "user");
            iReturn = pasDbExecSqlF("%s", caSmt);
        }
        pasLogs(PAS_SRCFILE, 1008, "Update Db iRet=%d", iReturn);
        pasDbClose(psConn);
        return iReturn;
    }
    else
    {
        pasLogs(PAS_SRCFILE, 1008, "Connect DataBase Error, %s", pasDbErrorMsg(NULL));
        return (-1);
    }
}





// 显示测试页面
int ncSrvTestplate(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caPlate[128];
    int iReturn;
    utPltDbHead *psDbHead;

    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "plate",      UT_TYPE_STRING, 100, caPlate);

    psDbHead = utPltInitDbHead();

    utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);

    return 0;
}


// 强制离线
int ncSrvLoginOK(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{

    char caUserAgent[256];
    char caTsid[24];
    char caClientIp[32], caOs[32], caType[32], caBro[32], caTerm[32], caLang[16], caPlate[128], caSsid[32];
    char caPasTsid[32];
    int iReturn, iRet, iCode;
    uint8 llTsid;
    char *pPlate;
    ncPortalOnline *psOnline;
    utPltDbHead *psDbHead;

    strcpy(caOs, "\0");
    strcpy(caType, "\0");
    strcpy(caBro, "\0");
    strcpy(caTerm, "\0");
    strcpy(caLang, "\0");
    strcpy(caTsid, "\0");
    strcpy(caPasTsid, "\0");
    utMsgOutMsgToLog(PAS_SRCFILE, 1008, psMsgHead, "[ncSrvLoginOK] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead, 4,
                               "usragent",    UT_TYPE_STRING, 255, caUserAgent,
                               "tsid",        UT_TYPE_STRING, 31, caTsid,
                               "lang",        UT_TYPE_STRING, 15, caLang,
                               "PASTSID",     UT_TYPE_STRING, 31, caPasTsid);

    psDbHead = utPltInitDbHead();
    iRet = pasUtlTermAttr(caUserAgent, caTerm, caOs, caBro, caType);
    pasLogs(PAS_SRCFILE, 1009, "[%d %-8s %-8s %-8s %-8s ]%s\n", iRet, caType, caTerm, caOs, caBro, caUserAgent);
    if(utStrIsSpaces(caTsid))
    {
        strcpy(caTsid, caPasTsid);
    }
    if(!utStrIsSpaces(caTsid))
    {
        sscanf(caTsid, "%llu", &llTsid);
        psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, llTsid);
        if(psOnline)
        {
            strcpy(caSsid, psOnline->caSsid);
            if(utStrIsSpaces(caLang))
            {
                strcpy(caLang, psOnline->caLang);
                utPltPutVar(psDbHead, "mobile", psOnline->caName);
                utPltPutVar(psDbHead, "mac", pasCvtMac(psOnline->caMac));
                char caTemp[164], caOut[64], caMd5[64], caKey[64];
                long iLen;
                strcpy(caKey, utComGetVar_sd(psShmHead, "MD5key", ""));
                sprintf(caTemp, "%s%s%s", caKey, psOnline->caName, pasCvtMac(psOnline->caMac));
                iLen = strlen(caTemp);
                utMd5(caTemp, iLen, caOut);
                pasStrCvtBin2Hex(caOut, 16, caMd5);
                caMd5[16] = 0;
                pasLogs(PAS_SRCFILE, 1009, "[caTemp:%sd md5:%s\n", caTemp, caMd5);
                utPltPutVar(psDbHead, "sign", caMd5);
            }
        }
        else
        {
            strcpy(caSsid, "\0");
        }
    }
    else
    {
        strcpy(caSsid, "\0");
    }
    pPlate = ncSrvGetPlateInfo(psShmHead, ncSrvGetPlateName(psShmHead, psOnline), caType, caLang, &iCode);
    if(pPlate)
    {
        sprintf(caPlate, "%s/login_ok.htm", pPlate);
    }
    else
    {
        sprintf(caPlate, "default/login_ok.htm", pPlate);
    }
    pasLogs(PAS_SRCFILE, 1009, "Plate:%s  ", caPlate);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);
}


// 强制离线
int ncSrvLoginError(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{

    char caSid[32];
    char caSsid[32];
    char caId[32];
    char caName[32];
    char caUserIp[32];
    char caUserMac[32];
    char caAcName[32];
    char caAcIp[32];
    char caClientIp[32];
    char caMsg[4096];
    int iReturn;
    utPltDbHead *psDbHead;
    utShmHand sShm;
    int iSum, lSn;
    uint2 nReqId = 0;
    uint4  nPort = 2000;
    ncPortalAcInfo *psAc;
    utMsgOutMsgToLog(PAS_SRCFILE, 1008, psMsgHead, "[ncSrvLogin] \n");
    strcpy(caSsid, "\0");
    strcpy(caSid, "\0");
    strcpy(caUserIp, "\0");
    strcpy(caName, "\0");
    strcpy(caClientIp, "\0");
    strcpy(caUserMac, "\0");
    strcpy(caId, "\0");
    strcpy(caAcName, "\0");
    strcpy(caAcIp, "\0");

    iReturn = utMsgGetSomeNVar(psMsgHead, 9,
                               "ssid",          UT_TYPE_STRING, 24, caSsid,
                               "userip",        UT_TYPE_STRING, 24, caUserIp,
                               "username",      UT_TYPE_STRING, 31, caName,
                               "usermac",       UT_TYPE_STRING, 31, caUserMac,
                               "SerialNo",      UT_TYPE_STRING, 24, caSid,
                               "ReqId",         UT_TYPE_STRING, 24, caId,
                               "wlanacname",    UT_TYPE_STRING, 31, caAcName,
                               "wlanacip",    UT_TYPE_STRING, 31, caAcIp,
                               "clientip",      UT_TYPE_STRING, 31, caClientIp);
    psDbHead = utPltInitDbHead();
    if(!utStrIsSpaces(caAcName))
    {
        psAc = (ncPortalAcInfo *)ncSrvGetAcInfo(psShmHead, caAcName);
    }
    else
    {
        psAc = (ncPortalAcInfo *)ncSrvGetAcInfoByIp(psShmHead, caAcIp);
    }


    utPltPutVar(psDbHead, "ip",    caAdminIp);
    utPltPutVar(psDbHead, "port",  caAdminPort);
    utPltPutVar(psDbHead, "userip", caUserIp);
    utPltPutVar(psDbHead, "username", caName);
    if(!utStrIsSpaces(caAcName))
    {
        utPltPutVar(psDbHead, "wlanacname", caAcName);
    }
    else
    {
        utPltPutVar(psDbHead, "wlanacip", caAcIp);
    }
    utPltPutVar(psDbHead, "usermac", caUserMac);
    lSn = atol(caSid);
    nReqId = atol(caId);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "login_main.htm");
    return 0;
}




// 认证
int ncRadLogin(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    ncPortalAcInfo  *psAc;
    char caSsid[32];
    char caName[32];
    char caPass[32];
    char caUserIp[32];
    char caUserMac[32];
    char caClientIp[32];
    char caChapPass[32], *pPlate;
    char caChallenge[32];
    char caAcName[32], caAcIp[32], caSrvIp[32], caSrvPort[10];;
    char caUrl[256], caUserAgent[256], caPlate[128], caPostUrl[256];
    char caMsg[4096];
    int iReturn, iRet;
    utPltDbHead *psDbHead;
    char caTerm[16], caOs[16], caBro[16], caType[16], caLang[16], caCode[16];

    utShmHand sShm;
    int iSum, iCode;
    uint2 nReqId = 0;
    uint4  nPort = 2000;
    utMsgOutMsgToLog(PAS_SRCFILE, 1008, psMsgHead, "[ncSrvLogin] \n");
    strcpy(caSsid, "\0");
    strcpy(caUserIp, "\0");
    strcpy(caName, "\0");
    strcpy(caPass, "\0");
    strcpy(caClientIp, "\0");
    strcpy(caUserMac, "\0");
    strcpy(caUrl, "\0");
    strcpy(caAcName, "\0");
    strcpy(caAcIp, "\0");

    iReturn = utMsgGetSomeNVar(psMsgHead, 13,
                               "lang",       UT_TYPE_STRING, 15, caLang,
                               "ssid",       UT_TYPE_STRING, 24, caSsid,
                               "userip",     UT_TYPE_STRING, 24, caUserIp,
                               "username",   UT_TYPE_STRING, 31, caName,
                               "password",   UT_TYPE_STRING, 31, caPass,
                               "srvname",     UT_TYPE_STRING, 31, caSrvIp,
                               "srvport",     UT_TYPE_STRING, 7,  caSrvPort,
                               "clientip",   UT_TYPE_STRING, 31, caClientIp,
                               "usermac",    UT_TYPE_STRING, 31, caUserMac,
                               "usragent",   UT_TYPE_STRING, 255, caUserAgent,
                               "wlanacname", UT_TYPE_STRING, 31, caAcName,
                               "wlanacip", UT_TYPE_STRING, 31, caAcIp,
                               "redirect",   UT_TYPE_STRING, 255, caUrl);
    psDbHead = utPltInitDbHead();
    if(!utStrIsSpaces(caAcName))
    {
        psAc = (ncPortalAcInfo *)ncSrvGetAcInfo(psShmHead, caAcName);
    }
    else
    {
        psAc = (ncPortalAcInfo *)ncSrvGetAcInfoByIp(psShmHead, caAcIp);
    }
    if(psAc)
    {
        if(psAc->iType == NCSRV_PORTAL_AC)
        {
            lReqIdNum = psAc->nSn;
            psAc->nSn++;
            if(iPapChap == 0)
            {
                iReturn = ncSrvSendToAcRequestChallenge(psAc->lAcip, psAc->nAcPort, lReqIdNum, caUserIp, &nReqId, caChallenge);
                ncUtlPrintHex("ncSrvSendToAcRequestChallenge1", caChallenge, 16);
                if(iReturn == 0)
                {
                    iReturn = ncSrvSendToAcRequesChapAuth(psAc->lAcip, psAc->nAcPort, lReqIdNum, nReqId, caUserIp, caName, caChallenge, caPass, caChapPass);
                }
            }
            else
            {
                iReturn = ncSrvSendToAcRequesPapAuth(psAc->lAcip, psAc->nAcPort, lReqIdNum, 0, caUserIp, caName, caPass);
            }
            if(iReturn == 0 || iReturn == 2)  // 认证成功
            {
                if(pasLogsIsOut(PAS_SRCFILE, 1088, NULL))
                {
                    printf(" Pid==%d\n", getpid());
                    sleep(10);
                }
                utPltPutVar(psDbHead, "lang",    caLang);
                if(!utStrIsSpaces(caAcName))
                {
                    sprintf(caMsg, "wlanacname@%s&userip@%s&ssid@%s&usermac@%s&username@%s", caAcName, caUserIp, caSsid, caUserMac, caName);
                }
                else if(!utStrIsSpaces(caAcIp))
                {
                    sprintf(caMsg, "wlanacip@%s&userip@%s&ssid@%s&usermac@%s&username@%s", caAcIp, caUserIp, caSsid, caUserMac, caName);
                }
                utPltPutVar(psDbHead, "getarg", caMsg);
                utPltPutVar(psDbHead, "ip",    caAdminIp);
                utPltPutVar(psDbHead, "port",  caAdminPort);
                utPltPutVar(psDbHead, "userip", caUserIp);
                utPltPutVar(psDbHead, "usermac", caUserMac);
                utPltPutVar(psDbHead, "username", caName);
                utPltPutVar(psDbHead, "wlanacname",  caAcName);
                utPltPutVar(psDbHead, "statusCode", "0");
                strcpy(caMsg, "\0");
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang", caLang);
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip", caUserIp);
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "usermac", caUserMac);
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "ssid", caSsid);
                if(!utStrIsSpaces(caAcName))
                {
                    utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "wlanacname", caAcName);
                }
                else
                {
                    utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "wlanacip", caAcIp);
                }

                utPltPutVar(psDbHead, "postarg", caMsg);
                iReturn = ncSrvSendToAcAuthAck(psAc->lAcip, psAc->nAcPort, lReqIdNum, nReqId, caUserIp);
                caTerm[0] = 0;
                caOs[0] = 0;
                caBro[0] = 0;
                caType[0] = 0;
                iRet = pasUtlTermAttr(caUserAgent, caTerm, caOs, caBro, caType);
                pasLogs(PAS_SRCFILE, 1009, "[%d %-8s %-8s %-8s %-8s ]%s\n", iRet, caType, caTerm, caOs, caBro, caUserAgent);
                pPlate = ncSrvGetPlateInfo(psShmHead, ncSrvGetPlateName(psShmHead, NULL), caType, caLang, &iCode);
                if(pPlate)
                {
                    sprintf(caPlate, "%s/login_ok.htm", pPlate);
                }
                else
                {
                    sprintf(caPlate, "default/login_ok.htm");
                }
                pasLogs(PAS_SRCFILE, 1009, "Plate=%s\n", caPlate);
                utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);
            }
            else
            {
                if(!utStrIsSpaces(caAcName))
                {
                    sprintf(caMsg, "wlanacname@%s&wlanuserip@%s&ssid@%s&usermac@%s&username@%s", caAcName, caUserIp, caSsid, caUserMac, caName);
                }
                else
                {
                    sprintf(caMsg, "wlanacip@%s&wlanuserip@%s&ssid@%s&usermac@%s&username@%s", caAcIp, caUserIp, caSsid, caUserMac, caName);
                }

                utPltPutVar(psDbHead, "getarg", caMsg);
                strcpy(caMsg, "\0");
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang", caLang);
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip", caUserIp);
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "usermac", caUserMac);
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "ssid", caSsid);

                if(!utStrIsSpaces(caAcName))
                {
                    utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "wlanacname", caAcName);
                }
                else
                {
                    utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "wlanacip", caAcIp);
                }

                utPltPutVar(psDbHead, "postarg", caMsg);
                utPltPutVar(psDbHead, "lang",  caLang);
                utPltPutVar(psDbHead, "ip",    caAdminIp);
                utPltPutVar(psDbHead, "port",  caAdminPort);
                utPltPutVar(psDbHead, "userip", caUserIp);
                utPltPutVar(psDbHead, "usermac", caUserMac);
                utPltPutVar(psDbHead, "username", caName);
                if(!utStrIsSpaces(caAcName))
                {
                    utPltPutVar(psDbHead, "wlanacname",  caAcName);
                }
                else
                {
                    utPltPutVar(psDbHead, "wlanacip",  caAcIp);
                }

                utPltPutVar(psDbHead, "statusCode", "5");
                iRet = pasUtlTermAttr(caUserAgent, caTerm, caOs, caBro, caType);
                pasLogs(PAS_SRCFILE, 1009, "[%d %-8s %-8s %-8s %-8s ]%s\n", iRet, caType, caTerm, caOs, caBro, caUserAgent);
                pPlate = ncSrvGetPlateInfo(psShmHead, ncSrvGetPlateName(psShmHead, NULL), caType, caLang, &iCode);
                if(pPlate)
                {
                    sprintf(caPlate, "%s/login_main.htm", pPlate);
                }
                else
                {
                    sprintf(caPlate, "default/login_main.htm");
                }
                if(strcasecmp(caLang, "zh") == 0)
                {
                    char caTemp[256];
                    sprintf(caTemp, "您输入的账号或密码不正确");
                    pasCharsetCvt(PAS_CCODE_GB, iCode, caTemp, caMsg, 255);
                }
                else
                {
                    sprintf(caMsg, "User name or password Error");
                }
                utPltPutVar(psDbHead, "message", caMsg);
                pasLogs(PAS_SRCFILE, 1009, "Plate=%s Message=%s\n", caPlate, caMsg);
                utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);
            }
        }
        else if(psAc->iType == NCSRV_PORTAL_RADIUS)
        {
            int lSession = 0, lIdle = 0, iRet;
            char caRadIp[32];
            sprintf(caRadIp, "%s:%u", utComHostIp(htonl(psAc->lAcip)), psAc->nAcPort);
            iRet =  pasRadiusAuth(caRadIp, psAc->caKey, caName, caPass, "CHAP", "\0",
                                  caUserMac, caSsid, &lSession, &lIdle);
            if(iRet == 2)   //
            {
                if(pasLogsIsOut(PAS_SRCFILE, 1088, NULL))
                {
                    printf(" Pid==%d\n", getpid());
                    sleep(10);
                }
                utPltPutVar(psDbHead, "lang",    caLang);
                if(!utStrIsSpaces(caAcName))
                {
                    sprintf(caMsg, "wlanacname@%s&userip@%s&ssid@%s&usermac@%s&username@%s", caAcName, caUserIp, caSsid, caUserMac, caName);
                }
                else
                {
                    sprintf(caMsg, "wlanacip@%s&userip@%s&ssid@%s&usermac@%s&username@%s", caAcIp, caUserIp, caSsid, caUserMac, caName);
                }

                utPltPutVar(psDbHead, "getarg", caMsg);
                utPltPutVar(psDbHead, "ip",    caAdminIp);
                utPltPutVar(psDbHead, "port",  caAdminPort);
                utPltPutVar(psDbHead, "userip", caUserIp);
                utPltPutVar(psDbHead, "usermac", caUserMac);
                utPltPutVar(psDbHead, "username", caName);
                utPltPutVar(psDbHead, "wlanacname",  caAcName);
                utPltPutVar(psDbHead, "statusCode", "5");
                strcpy(caMsg, "\0");
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang", caLang);
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip", caUserIp);
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "usermac", caUserMac);
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "ssid", caSsid);
                if(!utStrIsSpaces(caAcName))
                {
                    utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "wlanacname", caAcName);
                }
                else
                {
                    utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "wlanacip", caAcIp);
                }

                utPltPutVar(psDbHead, "postarg", caMsg);
                iReturn = ncSrvSendToAcAuthAck(psAc->lAcip, psAc->nAcPort, lReqIdNum, nReqId, caUserIp);
                caTerm[0] = 0;
                caOs[0] = 0;
                caBro[0] = 0;
                caType[0] = 0;
                iRet = pasUtlTermAttr(caUserAgent, caTerm, caOs, caBro, caType);
                pasLogs(PAS_SRCFILE, 1009, "[%d %-8s %-8s %-8s %-8s ]%s\n", iRet, caType, caTerm, caOs, caBro, caUserAgent);
                pPlate = ncSrvGetPlateInfo(psShmHead, ncSrvGetPlateName(psShmHead, NULL), caType, caLang, &iCode);
                if(pPlate)
                {
                    sprintf(caPlate, "%s/login_ok.htm", pPlate);
                }
                else
                {
                    sprintf(caPlate, "default/login_ok.htm");
                }
                pasLogs(PAS_SRCFILE, 1009, "Plate=%s\n", caPlate);
                utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);
            }
            else
            {
                if(!utStrIsSpaces(caAcName))
                {
                    sprintf(caMsg, "wlanacname@%s&wlanuserip@%s&ssid@%s&usermac@%s&username@%s", caAcName, caUserIp, caSsid, caUserMac, caName);
                }
                else
                {
                    sprintf(caMsg, "wlanacip@%s&wlanuserip@%s&ssid@%s&usermac@%s&username@%s", caAcIp, caUserIp, caSsid, caUserMac, caName);
                }
                utPltPutVar(psDbHead, "getarg", caMsg);
                strcpy(caMsg, "\0");
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang", caLang);
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip", caUserIp);
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "usermac", caUserMac);
                utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "ssid", caSsid);
                if(!utStrIsSpaces(caAcName))
                {
                    utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "wlanacname", caAcName);
                }
                else
                {
                    utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "wlanacip", caAcIp);
                }

                utPltPutVar(psDbHead, "postarg", caMsg);
                utPltPutVar(psDbHead, "lang",  caLang);
                utPltPutVar(psDbHead, "ip",    caAdminIp);
                utPltPutVar(psDbHead, "port",  caAdminPort);
                utPltPutVar(psDbHead, "userip", caUserIp);
                utPltPutVar(psDbHead, "usermac", caUserMac);
                utPltPutVar(psDbHead, "username", caName);
                if(!utStrIsSpaces(caAcName))
                {
                    utPltPutVar(psDbHead, "wlanacname",  caAcName);
                }
                else
                {
                    utPltPutVar(psDbHead, "wlanacip",  caAcIp);
                }

                utPltPutVar(psDbHead, "statusCode", "5");
                iRet = pasUtlTermAttr(caUserAgent, caTerm, caOs, caBro, caType);
                pasLogs(PAS_SRCFILE, 1009, "[%d %-8s %-8s %-8s %-8s ]%s\n", iRet, caType, caTerm, caOs, caBro, caUserAgent);
                pPlate = ncSrvGetPlateInfo(psShmHead, ncSrvGetPlateName(psShmHead, NULL), caType, caLang, &iCode);
                if(pPlate)
                {
                    sprintf(caPlate, "%s/login_main.htm", pPlate);
                }
                else
                {
                    sprintf(caPlate, "default/login_main.htm");
                }
                if(strcasecmp(caLang, "zh") == 0)
                {
                    char caTemp[256];
                    sprintf(caTemp, "您输入的账号或密码不正确");
                    pasCharsetCvt(PAS_CCODE_GB, iCode, caTemp, caMsg, 255);
                }
                else
                {
                    sprintf(caMsg, "User name or password Error");
                }
                utPltPutVar(psDbHead, "message", caMsg);
                pasLogs(PAS_SRCFILE, 1009, "Plate=%s Message=%s\n", caPlate, caMsg);
                utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);
            }
        }
    }
    else
    {
        if(!utStrIsSpaces(caAcName))
        {
            sprintf(caMsg, "wlanacname@%s&userip@%s&ssid@%s&usermac@%s&username@%s", caAcName, caUserIp, caSsid, caUserMac, caName);
        }
        else
        {
            sprintf(caMsg, "wlanacip@%s&userip@%s&ssid@%s&usermac@%s&username@%s", caAcIp, caUserIp, caSsid, caUserMac, caName);
        }

        utPltPutVar(psDbHead, "getarg", caMsg);
        strcpy(caMsg, "\0");
        utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "lang", caLang);
        utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "userip", caUserIp);
        utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "usermac", caUserMac);
        utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "ssid", caSsid);
        utStrAddF(caMsg, "<input type=\"hidden\" name=\"%s\" value=\"%s\">", "wlanacname", caAcName);
        utPltPutVar(psDbHead, "postarg", caMsg);
        utPltPutVar(psDbHead, "lang",    caLang);
        utPltPutVar(psDbHead, "ip",    caAdminIp);
        utPltPutVar(psDbHead, "port",  caAdminPort);
        utPltPutVar(psDbHead, "userip", caUserIp);
        utPltPutVar(psDbHead, "usermac", caUserMac);
        utPltPutVar(psDbHead, "username", caName);
        if(!utStrIsSpaces(caAcName))
        {
            utPltPutVar(psDbHead, "wlanacname",  caAcName);
        }
        else
        {
            utPltPutVar(psDbHead, "wlanacip",  caAcIp);
        }

        iRet = pasUtlTermAttr(caUserAgent, caTerm, caOs, caBro, caType);
        pasLogs(PAS_SRCFILE, 1009, "[%d %-8s %-8s %-8s %-8s ]%s\n", iRet, caType, caTerm, caOs, caBro, caUserAgent);
        pPlate = ncSrvGetPlateInfo(psShmHead, ncSrvGetPlateName(psShmHead, NULL), caType, caLang, &iCode);
        if(pPlate)
        {
            sprintf(caPlate, "%s/login_main.htm", pPlate);
        }
        else
        {
            sprintf(caPlate, "default/login_main.htm");
        }
        if(strcasecmp(caLang, "zh") == 0)
        {
            char caTemp[256];
            sprintf(caTemp, "您输入的账号或密码不正确");
            pasCharsetCvt(PAS_CCODE_GB, iCode, caTemp, caMsg, 255);
        }
        else
        {
            sprintf(caMsg, "User name or password Error");
        }
        utPltPutVar(psDbHead, "statusCode", "2");
        if(strcmp(caSrvPort, "443") == 0)
        {
            sprintf(caPostUrl, "https://%s:%s/pronline/Msg?FunName@ncSrvLogin", caSrvIp, caSrvPort);
        }
        else
        {
            sprintf(caPostUrl, "http://%s:%s/pronline/Msg?FunName@ncSrvLogin", caSrvIp, caSrvPort);
        }
        utPltPutVar(psDbHead, "posturl", caPostUrl);
        utPltPutVar(psDbHead, "message", caMsg);
        pasLogs(PAS_SRCFILE, 1009, "Plate=%s Message=%s\n", caPlate, caMsg);
        utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);
    }
    return 0;
}



// 检查用户是否在黑名单中

int ncSrvCheckBlack(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caName[32];
    char caClientIp[32];
    char caNasid[24];
    char caMsg[127];
    int iReturn, iRet;
    utPltDbHead *psDbHead;
    uint4 lSip;

    utMsgOutMsgToLog(PAS_SRCFILE, 1008, psMsgHead, "[ncSrvCheckBlack] \n");
    strcpy(caName, "\0");
    strcpy(caClientIp, "\0");

    iReturn = utMsgGetSomeNVar(psMsgHead, 3,
                               "nasid",    UT_TYPE_STRING, 23, caNasid,
                               "username", UT_TYPE_STRING, 31, caName,
                               "clientip", UT_TYPE_STRING, 31, caClientIp);
    lSip =   ntohl(pasIpcvtLong(caClientIp));
    iRet = ncSrvCheckBlackUser(psShmHead, caNasid, lSip, caName, caMsg);
    psDbHead = utPltInitDbHead();
    utPltPutVarF(psDbHead, "statusId", "%s", "1");
    utPltPutVarF(psDbHead, "statusCode", "%d", iRet);
    utPltPutVar(psDbHead, "message", caMsg);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "login_getpass_return.htm");
    return 0;
}



/* 检查用户的有效性
   0--正常
   5--无效用户
   3--在黑名单中
   */
int ncSrvCheckBlackUser(utShmHead *psShmHead, char *caNasid, uint4 lIp, char *pUserName, char *pMsg)
{
    int iRet, iSp, iArea;
    char caName0[32], caArea[32], caSpname[16];
    ncPortalOnline *psOnline;
    iRet =  ncPortalCheckMobileNumber(pUserName, caName0, caArea, caSpname, &iArea, &iSp);
    if(iRet <= 0)
    {
        pasLogs(PAS_SRCFILE, 1008, " Username: %s Number Error AreaCode=%d", caName0, iRet);
        sprintf(pMsg, "Username is invalid");
        return 5;
    }
    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIp(psShmHead, caNasid, lIp);
    if(psOnline == NULL)    // 不支持
    {
        pasLogs(PAS_SRCFILE, 1008, " Username: %s is not online,", caName0);
        sprintf(pMsg, "Username is not online,ip=%s", utComHostIp(htonl(lIp)));
        return 5;
    }
    else
    {
        if(ncSrvPortalIsBlackUser(psShmHead, psOnline->lGroupid, NCPORTAL_BLACK_USER, caName0))
        {
            pasLogs(PAS_SRCFILE, 1008, " Username: %s is in Black", caName0);
            sprintf(pMsg, "Username is in black");
            return 3;
        }
        if(ncSrvPortalIsBlackUser(psShmHead, psOnline->lGroupid, NCPORTAL_BLACK_MAC, psOnline->caMac))
        {
            pasLogs(PAS_SRCFILE, 1008, " Mac Address %s is Black", pasCvtMac(psOnline->caMac));
            sprintf(pMsg, "Mac address is in black");
            return 3;
        }
    }
    return 0;
}

int ncSrvDesDecryptA(char *pIn, char *pOut)
{
    char caTemp[256];
    int iLen;
    iLen = pasStrCvtHex2Bin(pIn, caTemp);
    iLen = utDesDecrypt(caTemp, iLen, pOut, PAS_DEF_KEY);
    pOut[iLen] = 0;
    return iLen;
}


int ncSrvDesEncryptA(char *pIn, char *pOut)
{
    char caTemp[256];
    int iLen;
    iLen = utDesEncrypt(pIn, strlen(pIn), caTemp, PAS_DEF_KEY);
    pasStrCvtBin2Hex(caTemp, iLen, pOut);
    return 0;
}



/* 测试进程
   请求
   应答:


 */
int ncSrvTest(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char    caText[256];
    char    caIp[24];
    uint4   lTime, lSum;
    int     iReturn;
    utMsgOutMsgToLog(PAS_SRCFILE, 1008, psMsgHead, "[ncSrvTest] %s\n", utComGetSockIpS(iFd, caIp));

    lTime = time(0);
    lSum = pasDbSumRecord("ncsrvtest");
    if(lSum > 100)
    {
        iReturn = pasDbExecSqlF("delete from ncsrvtest");
        sprintf(caText, "<xml><delsum>%u</delsum><sqlcode>%d</sqlcode></xml>\n", lSum, iReturn);
    }
    else
    {
        iReturn = pasDbExecSqlF("insert into ncsrvtest(lasttime) value (%u)", lTime);
        sprintf(caText, "<xml><addsum>%u</addsum><sqlcode>%d</sqlcode></xml>\n", lSum, iReturn);
    }
    utComTcpResponse(iFd, psMsgHead, 1,
                     "text", UT_TYPE_STRING, caText);
    return 0;
}


// 获取本机的ID
// 对于不正确的Name，系统将不会返回任何东西。

int ncSrvGetMyId(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caName[32];
    char caIp[24];
    int  iReturn, i;
    ncPortalSummary *psSumm;
    utMsgOutMsgToLog(PAS_SRCFILE, 1018, psMsgHead, "[ncSrvGetMyId] %s\n", utComGetSockIpS(iFd, caIp));
    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "myname",       UT_TYPE_STRING, 31, caName);
    if(iReturn == 1)
    {
        iReturn = ncSrvGetClientIdByName(psShmHead, caName);
        if(iReturn > 0)
        {
            pasLogs(PAS_SRCFILE, 1018, " MYname:%s  Id:%u", caName, iReturn);
            pasTcpResponse(iFd, psMsgHead,
                           pasGetCurComKey(),                /* 密钥，暂时不用  */
                           1,
                           "myid",   UT_TYPE_ULONG, iReturn);
        }
        psSumm = (ncPortalSummary *)utShmArray(psShmHead, NCSRV_LNK_SYSINFO);
        if(psSumm)
        {
            for(i = 0; i < psSumm->nSumEim; i++)
            {
                if(strcasecmp(psSumm->psEimServer[i].caSrvName, caName) == 0)
                {
                    psSumm->psEimServer[i].cStatus = 1;
                    psSumm->psEimServer[i].lLastTime = time(0);
                    psSumm->nCurEim = i;
                }
            }
        }
    }
    return 0;
}




//通过众人获取短信验证密码
int ncSrvSendSmsByPeople(utShmHead *psShmHead, char *caNo, char *caPre, char *caSuff)
{

    long lStatus = 0;
    int  iReturn;
    char caMesg[32];
    char caTemp[1024];
    char caHost[256], caHtml[2024], caReturn[32];
    char caId[72], caKey[128], caXml[2048], caXml_utf8[2048];
    char caPre_utf8[64], caSuff_utf8[64];
    char caPre_gbk[64], caSuff_gbk[64];
    char caSha1[64];

    strcpy(caHost, utComGetVar_sd(psShmHead, "PeopleHost", "www.ikeycn.com"));
    pasGBKToUtf8(caPre, caPre_gbk, 60);
    pasGBKToUtf8(caSuff, caSuff_gbk, 60);
    strcpy(caId, utComGetVar_sd(psShmHead, "PeopleName", "AA0219BFC18A738C7BFAB395A4E42E26"));
    strcpy(caKey, utComGetVar_sd(psShmHead, "Peoplekey", "AA0219BFC18A738C7BFAB395A4E42E264288D50FCCF051073DCC5B160254AB70"));
    sprintf(caXml, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
    sprintf(caXml + strlen(caXml), "<TokenProtocol>\r\n");
    sprintf(caXml + strlen(caXml), "<Version>1.0</Version>\r\n");
    sprintf(caXml + strlen(caXml), "<PackageType>115</PackageType>\r\n");
    sprintf(caXml + strlen(caXml), "<EtpsId>%s</EtpsId>\r\n", caId);
    sprintf(caXml + strlen(caXml), "<EtpsKey>%s</EtpsKey>\r\n", caKey);
    sprintf(caXml + strlen(caXml), "<SMSPrefix>%s</SMSPrefix>\r\n", caPre);
    sprintf(caXml + strlen(caXml), "<SMSSuffix>%s</SMSSuffix>\r\n", caSuff);
    sprintf(caXml + strlen(caXml), "<MobileTel>%s</MobileTel>\r\n", caNo);
    memset(caTemp, 0, sizeof(caTemp));
    sprintf(caTemp, "1.0#115#%s#%s#%s#%s#%s", caId, caKey, caPre_gbk, caSuff_gbk, caNo);
    pasSha1(caTemp, caSha1);
    sprintf(caXml + strlen(caXml), "<PackageMac>%s</PackageMac>\r\n", caSha1);
    sprintf(caXml + strlen(caXml), "</TokenProtocol>\r\n");
    pasLogs(PAS_SRCFILE, 1008, "caXml=%s", caXml);
    // pasGBKToUtf8(caXml,caXml_utf8,2000);
    iReturn = pasSendHttpPostXml(caHost, "/peopleooac/sms/getSmsCode.htm", caHtml, 2023, caXml);
    pasLogs(PAS_SRCFILE, 1008, "iReturn=%d Html:[%s]\n", iReturn, caHtml);
    strcpy(caReturn, "-1");
    if(iReturn == 0)
    {
        ncUtlGetWordBetween(caHtml, "<ResponseCode>", "</ResponseCode>", caReturn, 30);
    }
    if(strcmp(caReturn, "0000") == 0)
    {
        return 1;
    }
    else
    {
        return (-1);
    }
}

// 丹尼斯获取短信密码
int ncSrvSysDnsPass(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead, char *pPass)
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
    strcpy(caName, "\0");
    strcpy(caUserMac, "\0");
    strcpy(caSmsName, "\0");
    strcpy(caNasid, "\0");
    memset(caTsid, 0, 24);
    memset(caNasid, 0, 32);
    strcpy(caApMac, "\0");

    lTime = time(0);

    psSumm = (ncPortalSummary *)utShmArray(psShmHead, NCSRV_LNK_SYSINFO);
    strcpy(caTsid, "\0");
    iReturn = utMsgGetSomeNVar(psMsgHead, 5,
                               "tsid",     UT_TYPE_STRING, 20, caTsid,
                               "lang",     UT_TYPE_STRING, 15, caLang,
                               "charset",  UT_TYPE_STRING, 15, caCode,
                               "userip",   UT_TYPE_STRING, 24, caUserIp,
                               "mobile", UT_TYPE_STRING, 31, caName);

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


    if(psOnline && psOnline->psAp && psOnline->psAp->psGroup)    // 短信计费单位
    {
        strcpy(caSmsName, psOnline->psAp->psGroup->caSmsName);
    }
    else
    {
        strcpy(caSmsName, utComGetVar_sd(psShmHead, "MySmsName", "None"));
    }

    // 检查黑名单
    if(psOnline)
    {
        strcpy(caApMac, psOnline->caApName);
        lStype = psOnline->lStype;
        if(ncSrvPortalIsBlackUser(psShmHead, psOnline->lStype, NCPORTAL_BLACK_USER, caName))
        {

            return -1;
        }
        if(ncSrvPortalIsBlackUser(psShmHead, psOnline->lStype, NCPORTAL_BLACK_MAC, psOnline->caMac))
        {
            pasLogs(PAS_SRCFILE, 1008, " Username: %s Mac %s is in Black", caName, pasCvtMac(psOnline->caMac));

            return -2;
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

            return -1;
        }
    }

    srand(time(0));
    lCode = rand() % 999999L;
    sprintf(caPass, "%06lu", lCode);
    caPass[6] = 0;

    strcpy(pPass, caPass);
    strcpy(caGroupCode, "\0");
    lGroupid = 0;

    if(psOnline)
    {
        psOnline->psUser = ncSrvSynUserPassword(psShmHead, psOnline->lStype, psOnline->lGroupid, caName, caPass, psOnline->caGroupCode, psOnline->caSsid, NCSRV_USERTYPE_SMS);
        if(iUpdateUserFlags == 2)
        {
            ncSrvSynUserPassword2RadSrv(psOnline->psPar, caName, caPass, psOnline->lStype);
            pasLogs(PAS_SRCFILE, 1008, " Update User [%s]  Pass:[%s] Group:%s Ssid:%s  Stype:%u Ret=%d ",
                    caName, caPass, psOnline->caGroupCode, psOnline->caSsid, psOnline->lStype, iReturn);
        }
    }
    else
    {
        ncSrvSynUserPassword(psShmHead, 0, 0, caName, caPass, "\0", utComGetVar_sd(psShmHead, "DefSsid", "\0"), NCSRV_USERTYPE_SMS);
        if(iUpdateUserFlags == 2)
        {
            ncSrvSynUserPassword2RadSrv(psSumm->psPar, caName, caPass, 0);
            pasLogs(PAS_SRCFILE, 1008, " Update User [%s]  Pass:[%s] iReturn=%d ",
                    caName, caPass, iReturn);
        }
    }


    return 0;
}

#ifdef A00000

//显示电子身份验证页面
int ncSrvCheckIp_Eid(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caIp[128];
    char caUrl[2560];
    char caPlatePath[64], caPlate[128], caPort[16];
    char caSid[16];
    char caSrvname[64], caRequir[2560];
    char caFlag[8];
    char caUsragent[256];
    char caUrl2[2560];
    char caTemp2[1024];
    char *p;
    int iReturn;
    unsigned long lIp, lSid;
    long lFlag, lLogin;
    char caEng_flag[16];
    char caPath_all[256];
    char caLogincount[16];
    char caUsertype[16];
    char caMac1[20], caMac[20];
    long lMobflag;
    ncUserInfo *psUserInfo = NULL;
    utPltDbHead *psDbHead;
    pasDbCursor *psCur;

    char caTerm[64], caOs[64], caBro[64], caType[64];
    char caDownios[16];
    utMsgOutMsgToLog(PAS_SRCFILE, 1008, psMsgHead, "[ncSrvCheckIp_Eid] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead, 7,
                               "clientip",    UT_TYPE_STRING, 20, caIp,
                               "httpref",     UT_TYPE_STRING, 127, caUrl,
                               "sid",         UT_TYPE_STRING, 11, caSid,
                               "srvname",     UT_TYPE_STRING, 63, caSrvname,
                               "usragent",    UT_TYPE_STRING, 255, caUsragent,
                               "eng_flag",    UT_TYPE_STRING, 8, caEng_flag,
                               "downios", UT_TYPE_STRING, 10, caDownios);
    psDbHead = utPltInitDb();

    lIp = ntohl(pasIpcvtLong(caIp));
    psUserInfo = ncGetUserInfoByIp(psShmHead, lIp);
    if(psUserInfo == NULL)
    {
        printf(" %s  OffLine \n", caIp);
        utPltPutVar(psDbHead, "tmsg", "off");
        if(strcmp(caDownios, "down") == 0)
        {
            utStrToLower(caUsragent);
            printf("usragent=%s\n", caUsragent);
            if(strstr(caUsragent, "apple") || strstr(caUsragent, "iphone"))
            {
                char caUrl1[256];
                char caUserIp[32];
                char caTemp[512];
                strcpy(caUserIp, utComHostIp(htonl(psOnline->lSip)));
                strcpy(caUserMac, pasStrEncodeDesHex(pasCvtMac(psOnline->caMac), caTemp, PAS_DEF_KEY));
                strcpy(caName, pasStrEncodeDesHex(psOnline->caName, caTemp, PAS_DEF_KEY));
                if(strcmp(caSrvPort, "443") == 0)
                {
                    sprintf(caUrl, "https://%s:%s/pronline/Msg?FunName=ncSrvLoginOK&tsid=%s", caSrvIp, caSrvPort, caTsid);
                }
                else
                {
                    sprintf(caUrl, "http://%s:%s/pronline/Msg?FunName=ncSrvLoginOK&tsid=%s", caSrvIp, caSrvPort, caTsid);
                }
                pasStrEncodeDesHex(caUrl, caTemp, PAS_DEF_KEY);
                sprintf(caUrl1, "http://%s:%u/proauth/Msg?FunName@ncWebPortalDoLogin&username=%s&userip=%s&mac=%s&url=%s",
                        utComHostIp(htonl(psOnline->lGwip)),
                        psOnline->nGwport,
                        "downloadios",
                        caUserIp,
                        caUserMac,
                        caTemp);
                pasSetHttpHeadStatus(302, "Found");
                pasSetHttpHeadLocation(caUrl1);
                pasLogs(PAS_SRCFILE, 1008, "User:%s  Red:%s ", psOnline->caName, caUrl1);
                utPltOutToHtmlFromBuf(iFd, psMsgHead, psDbHead, "\0");
                return 0;
            }
        }
    }
    return 0;
}





#endif