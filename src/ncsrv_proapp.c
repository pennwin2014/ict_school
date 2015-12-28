/* 
    APP认证
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
#define PAS_SRCFILE   7112
static char caNoCheckApp[255]="LIANSHANG";
static char caValidApp[255]="PROEIM,LIANSHANG";

int proAppSetNoCheckApp(char *pSrc)
{
    strcpy(caNoCheckApp,pSrc);
    return 0;
}

int proAppIsNoCheckApp(char *pAppSrc)
{
    char *p;
    char caTemp[32];
    p = caNoCheckApp;
    while(p) {
        p = utStrGetWord(p,caTemp,15,",");
        if(strcmp(caTemp,pAppSrc)==0) {
            return 1;
        }
        if(*p) {
            p++;
        }
        else {
            break;
        }
    }
    return 0;
}

int proAppSetValidApp(char *pSrc)
{
    strcpy(caValidApp,pSrc);
    return 0;
}

int proAppIsValidApp(char *pAppSrc)
{
    char *p;
    char caTemp[32];
    p = caValidApp;
    while(p) {
        p = utStrGetWord(p,caTemp,15,",");
        if(strcmp(caTemp,pAppSrc)==0) {
            return 1;
        }
        if(*p) {
            p++;
        }
        else {
            break;
        }
    }
    return 0;
}




// 获取认证服务器地址

int proAppGetIp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char    caHtml[1024];
	int     lSize,iReturn;
    char    caDip[32]="192.168.20.29";
    char    caPort[16]="80";
    char    caMode[16];
    char    caAuthProt[16];
    char    caAuthPath[128];
	char    caAuthMode[32]="";
    
    utMsgOutMsgToLog(PAS_SRCFILE,1008, psMsgHead,"[ncAppGetIp] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead,1,
                    "mode",  UT_TYPE_STRING,15,caMode);
	strcpy(caAuthMode,  utComGetVar_sd(psShmHead, "authmode",  ""));
    if(strcasecmp(caMode,"SHAPP")==0) {                    
        strcpy(caDip,       utComGetVar_sd(psShmHead, "AppAuthHost",    "192.168.0.1"));
        strcpy(caPort,      utComGetVar_sd(psShmHead, "AppAuthPort",  "443"));
        strcpy(caAuthPath,  utComGetVar_sd(psShmHead, "AppAuthPath",  "/appauth"));
        strcpy(caAuthProt,  utComGetVar_sd(psShmHead, "AppAuthProt",  "https"));
        pasStrCvtJson(caHtml,6,
                UT_TYPE_STRING,"protocol",  caAuthProt,
                UT_TYPE_STRING,"ip",        caDip,
                UT_TYPE_STRING,"port",      caPort,
                UT_TYPE_STRING,"authpath",  caAuthPath,
                UT_TYPE_STRING,"authmode",  caAuthMode,
                UT_TYPE_STRING,"data",      "[]");
    }
    else {

        strcpy(caDip,       utComGetVar_sd(psShmHead,  "AppAuthHost",    "192.168.0.1"));
        strcpy(caPort,      utComGetVar_sd(psShmHead,  "AppAuthPort",  "443"));
        strcpy(caAuthPath,  utComGetVar_sd(psShmHead,  "AppAuthPath",  "/appauth"));

        pasStrCvtJson(caHtml,5,
                UT_TYPE_STRING,"ip",        caDip,
                UT_TYPE_STRING,"port",      caPort,
                UT_TYPE_STRING,"mode",      caMode,
                UT_TYPE_STRING,"authmode",  caAuthMode,
                UT_TYPE_STRING,"data",      "[]");
    }
    pasLogs(PAS_SRCFILE,1008,"Response:%s\n\n",caHtml);
    utComTcpResponse(iFd,psMsgHead,1,
		                       "text",UT_TYPE_STRING,caHtml);
    return 0;
}


int ncSrvAppDoGetIp(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead,ncPortalOnline *psOnline)
{
    char    caHtml[1024];
    char    caSid[32];
    int     iStatus = 0;
	char    caAuthMode[32] = "";
    ncPortalSummary *psSumm;
    psSumm = (ncPortalSummary *)ncSrvGetSumm(psShmHead);
    utMsgOutMsgToLog(PAS_SRCFILE,1008, psMsgHead,"[ncSrvAppDoGetIp] \n");
    sprintf(caSid,"%18lld",psOnline->lTsid);
	strcpy(caAuthMode,  utComGetVar_sd(psShmHead, "authmode",  ""));
	pasLogs(1234,1233,"caAuthMode:%s\n\n",caAuthMode);
    pasStrCvtJson(caHtml,9,
                UT_TYPE_STRING,"protocol",  psSumm->sAppAuth.prot,
                UT_TYPE_STRING,"ip",        psSumm->sAppAuth.host,
                UT_TYPE_STRING,"port",      psSumm->sAppAuth.port,
                UT_TYPE_STRING,"authpath",  psSumm->sAppAuth.path,
                UT_TYPE_STRING,"SESSION_ID",caSid,
                UT_TYPE_STRING,"authmode",  caAuthMode,
                UT_TYPE_ULONG, "STATUS",    iStatus,
                UT_TYPE_STRING,"MAC",       pasCvtMac12(psOnline->caMac),
                UT_TYPE_STRING,"data",      "[]");
    pasLogs(PAS_SRCFILE,1008,"Response:%s\n\n",caHtml);
    utComTcpResponse(iFd,psMsgHead,1,
		                       "text",UT_TYPE_STRING,caHtml);
    return 0;
}


// 获取认证服务器地址

int proAppGetSid(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char    caHtml[1024];
	int     lSize,iReturn;
    char    caDip[32]="192.168.20.29";
    char    caPort[16]="80";
    char    caMode[16];
    char    caAuthProt[16];
    char    caAuthPath[128];
    ncPortalOnline *psOnline;
    ncPortalSummary *psSumm;
    psSumm = (ncPortalSummary *)ncSrvGetSumm(psShmHead);
/*
    utMsgOutMsgToLog(PAS_SRCFILE,1008, psMsgHead,"[proAppGetSid] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead,5,
                    "APP_SRC",      UT_TYPE_STRING,16,caAppSrc,
                    "AP_MAC",       UT_TYPE_STRING,17,caApMac,
                    "MAC",          UT_TYPE_STRING,17,caMac,
                    "CLIENT_IP",    UT_TYPE_STRING,15,caSip,
                    "SSID",         UT_TYPE_STRING,31,caSsid);
    psAp = 
    psOnline = (ncPortalOnline *)proAppGetOnlineUser(psShmHead,caMac,caSip,caApMac,caSsid);
    if(psOnline) {
        if(psOnline->psAp && psOnline->psAp->psGroup) {
            utStrncpy(caName,psOnline->psAp->psGroup->caGroupName,14);
            pasGBKToUtf8(psOnline->psAp->psGroup->caDisp,caDisp,127);
        }
        strncpy(psOnline->caSrc,caAppSrc,16);
        if(!utStrIsSpaces(caSsid) && strcmp(psOnline->caSsid,caSsid) != 0) {
            strcpy(psOnline->caSsid,caSsid);
            iMod = 1;
        }
       if(!utStrIsSpaces(caApMac) && strcmp(psOnline->caApName,caApMac) != 0) {
            strcpy(psOnline->caApName,caApMac);
            iMod = 1;
       }
    }
    else {
        
    }
    pasStrCvtJson(caHtml,10,
                UT_TYPE_STRING,"SJ_SRC",        psSumm->sAppAuth.sjname,
                UT_TYPE_STRING,"SJ_IP",         psSumm->sAppAuth.host,
                UT_TYPE_STRING,"SJ_PORT",       psSumm->sAppAuth.port,
                UT_TYPE_STRING,"SJ_URL",        psSumm->sAppAuth.path,
                UT_TYPE_STRING,"SJ_PROTOCOL",   psSumm->sAppAuth.prot,
                UT_TYPE_STRING,"SERVICE_NUM",   caName,
                UT_TYPE_STRING,"SERVICE_NAME",  caDisp,
                UT_TYPE_STRING,"SESSION_ID",    caSid,
                UT_TYPE_ULONG, "STATUS",        iStatus,
                UT_TYPE_STRING,"ERRMSG",        caMsg);
                */
    return 0;
}


/*

 */
int proAppGetInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    ncPortalOnline *psOnline=NULL;
    int iReturn;
    char caHtml[1024];
    char caAppSrc[36],caIp[20],caMac[32],caDisp[20],mac[6];
    char caSid[36],caName[32];
    char caApMac[20],caSsid[36],caUidsn[36],caMsg[128],caUid[32],caUsername[32];
    uint4 lSip;
    uint8 llTsid;
	int4  iStatus = 0,iMod=0,iRet;
    ncPortalSummary *psSumm;
    psSumm = (ncPortalSummary *)ncSrvGetSumm(psShmHead);

    iReturn = utMsgGetSomeNVar(psMsgHead,7,
                    "APP_SRC",      UT_TYPE_STRING,15,caAppSrc,
                    "CLIENT_IP",    UT_TYPE_STRING,15,caIp,
                    "MAC",          UT_TYPE_STRING,17,caMac,
                    "AP_MAC",       UT_TYPE_STRING,17,caApMac,
                    "SSID",         UT_TYPE_STRING,16,caSsid,
                    "UID_SN",       UT_TYPE_STRING,32,caUidsn,
                    "SESSION_ID",   UT_TYPE_STRING,32,caSid);
    utMsgOutMsgToLog(PAS_SRCFILE,1008,psMsgHead,"[proAppGetInfo] \n");
    pasCvtMacI(caMac,mac);
    
    iRet = proAppCheckAppSrc(psShmHead,caAppSrc);   // 检查APP厂家的有效性
    if(iRet != 1) {  // APP SRC无效
        iStatus = NCAPP_STATUS_ERRAPP;
        pasGBKToUtf8("没有手机号码",caMsg,127);
    }
    //
    iRet = proAppCheckUserByUidMac(psShmHead,caUidsn,mac,caUid,caUsername);
    if(iRet == 1) {
        if(!utStrIsSpaces(caSid)) {
            sscanf(caSid,"%llu",&llTsid);
            psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead,llTsid);
            if(psOnline == NULL) {
                   iStatus = NCAPP_STATUS_ERRSID;                 // 不是同一个终端的SessionId, 需要重新获取IP地址
            }
            else {
                if(memcmp(psOnline->caMac,mac,6)==0) {  // 同一个连接
                    if(psOnline->login == NCPORTAL_ONLINE_LOGIN) { // 已经登录
                        iStatus = NCAPP_STATUS_ONLINE;                 // 已经在线
                    }
                    else {
                        iStatus = NCAPP_STATUS_OK;                 // 不在线, 需要重新登录
                    }
                    if(psOnline->psAp && psOnline->psAp->psGroup) {
                        utStrncpy(caName,psOnline->psAp->psGroup->caGroupName,14);
                        pasGBKToUtf8(psOnline->psAp->psGroup->caDisp,caDisp,127);
                    }
                    strncpy(psOnline->caSrc,caAppSrc,16);
                    if(!utStrIsSpaces(caSsid) && strcmp(psOnline->caSsid,caSsid) != 0) {
                        strcpy(psOnline->caSsid,caSsid);
                        iMod = 1;
                    }
                    if(!utStrIsSpaces(caApMac) && strcmp(psOnline->caApName,caApMac) != 0) {
                        strcpy(psOnline->caApName,caApMac);
                        iMod = 1;
                    }
                    if(iMod) {
                        proAppSynOnlineUser(psShmHead,psOnline);
                    }                
                }
                else {
                    iStatus = NCAPP_STATUS_ERRMAC;                 // 不是同一个终端的SessionId, 需要重新获取IP地址
                }
            }
        }
        else { // 无SSID
            iStatus = NCAPP_STATUS_ERRSID;                 // 无SSID
        }
    }
    else if(iRet == 0) {  // Uidsn 不存在
        iStatus = NCAPP_STATUS_ERRUIDSN;                 // 无SSID
        pasGBKToUtf8("UID_SN不存在",caMsg,127);

    }
    else {  // UidSn不正确
        iStatus = NCAPP_STATUS_ERRUIDSN;                 // 无SSID
        pasGBKToUtf8("UID_SN 错误",caMsg,127);
    }
    if(iStatus > 1) {
        pasStrCvtJson(caHtml,11,
                UT_TYPE_STRING,"SJ_SRC",        psSumm->sAppAuth.sjname,
                UT_TYPE_STRING,"SJ_IP",         psSumm->sAppAuth.host,
                UT_TYPE_STRING,"SJ_PORT",       psSumm->sAppAuth.port,
                UT_TYPE_STRING,"SJ_URL",        psSumm->sAppAuth.path,
                UT_TYPE_STRING,"SJ_PROTOCOL",   psSumm->sAppAuth.prot,
                UT_TYPE_STRING,"SERVICE_NUM",   caName,
                UT_TYPE_STRING,"SERVICE_NAME",  caDisp,
                UT_TYPE_STRING,"SESSION_ID",    caSid,
                UT_TYPE_ULONG, "STATUS",        iStatus,
                UT_TYPE_STRING,"ERRMSG",        caMsg,
                UT_TYPE_STRING,"data",      "[]");
    }
    else {
        pasStrCvtJson(caHtml,10,
                UT_TYPE_STRING,"SJ_SRC",        psSumm->sAppAuth.sjname,
                UT_TYPE_STRING,"SJ_IP",         psSumm->sAppAuth.host,
                UT_TYPE_STRING,"SJ_PORT",       psSumm->sAppAuth.port,
                UT_TYPE_STRING,"SJ_URL",        psSumm->sAppAuth.path,
                UT_TYPE_STRING,"SJ_PROTOCOL",   psSumm->sAppAuth.prot,
                UT_TYPE_STRING,"SERVICE_NUM",   caName,
                UT_TYPE_STRING,"SERVICE_NAME",  caDisp,
                UT_TYPE_STRING,"SESSION_ID",    caSid,
                UT_TYPE_ULONG, "STATUS",        iStatus,
                UT_TYPE_STRING,"data",      "[]");
    }
    pasLogs(PAS_SRCFILE,1008,"Response:%s\n\n",caHtml);
    utComTcpResponse(iFd,psMsgHead,1,
		                       "text",UT_TYPE_STRING,caHtml);
    return 0;
}




int proAppCheck(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iReturn,lTime;
    ncPortalOnline *psOnline;
    char caHtml[1024];
    char caAppSrc[36],caUidSn[36],caUid[36],caUsername[36],caSecr[4],caIp[20],caMac[64],caId[40],caSid[40];
    unsigned long lIp;
    char caApMac[40],caSsid[40],caMsg[128];
    char caUserIp[32];
    uint4 lSip;
    char caAppUrl[128];
    char caLongitude[20],caLatitude[20],caProtocol[16],caMethod[16],caUrl[256],caArgs[256];
    uint8 llTsid;
	int iStatus = 0,iRet,iFlags=0;
	char caUrlflag[12];
	char caPassurl[128],caFailurl[128];
    ncPortalSummary *psSumm;
    strcpy(caProtocol,"\0");
    strcpy(caMethod,"\0");
    strcpy(caArgs,"\0");
    strcpy(caUrl,"\0");
    strcpy(caMsg,"\0");
    strcpy(caIp,"\0");
    psSumm = (ncPortalSummary *)ncSrvGetSumm(psShmHead);
    iReturn = utMsgGetSomeNVar(psMsgHead,4,
                    "SESSION_ID",      UT_TYPE_STRING,32,caSid,
                    "UID_SN",          UT_TYPE_STRING,32,caUidSn,
                    "LONGITUDE",       UT_TYPE_STRING,16,caLongitude,
                    "LATITUDE",        UT_TYPE_STRING,16,caLatitude);
    utMsgOutMsgToLog(PAS_SRCFILE,1008,psMsgHead,"[proAppCheck] \n");
    sscanf(caSid,"%llu",&llTsid);
    strcpy(caMsg,"\0");
    iFlags = 0;
    strcpy(caProtocol,"\0");
    strcpy(caMethod,"\0");
    strcpy(caUrl,"\0");
    strcpy(caArgs,"\0");
    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead,llTsid);
    if(psOnline) {
        iRet = proAppCheckUserByUidMac(psShmHead,caUidSn,psOnline->caMac,caUid,caUsername);
        if(iRet == 1) {  // 验证通过
            iStatus = NCAPP_STATUS_OK;
            strcpy(caIp,utComHostIp(htonl(psOnline->lSip)));
            ncSrvUpdateOnlineUserByOnline(psShmHead,psOnline,caUsername,caUsername);
            iFlags = 1;
            if(psOnline->cFrom == NCPORTAL_VENDOR_PROEIMV7) {
                char caUrl1[512];
                char caUserIp[40];
                char caTemp[512];
                char caApMac[64],caSsid[64];
                char caRssi[32];
                char caUserMac[64],caName[40];
                ncLbsMacLocation *psMac;
                strcpy(caProtocol,"HTTP");
                strcpy(caMethod,"GET");
                strcpy(caUserIp,utComHostIp(htonl(psOnline->lSip)));
                strcpy(caUserMac,pasStrEncodeDesHex(pasCvtMac(psOnline->caMac),caTemp,PAS_DEF_KEY));
                strcpy(caName,pasStrEncodeDesHex(psOnline->caName,caTemp,PAS_DEF_KEY));
                strcpy(caApMac,pasStrEncodeDesHex(psOnline->caApName,caTemp,PAS_DEF_KEY));
                strcpy(caSsid,pasStrEncodeDesHex(psOnline->caSsid,caTemp,PAS_DEF_KEY));

                sprintf(caUrl,"http://%s:%u/proauth/Msg",
                    utComHostIp(htonl(psOnline->lGwip)),
                    psOnline->nGwport);

                sprintf(caArgs,"FunName@ncWebPortalDoLogin&username=%s&userip=%s&mac=%s&apmac=%s&ssid=%s",
                    caName,
                    caUserIp,
                    caUserMac,
                    caApMac,
                    caSsid);
                psMac = (ncLbsMacLocation *)ncSrvGetMacLogByMac(psShmHead,psOnline->caMac);
                if(psMac) {
                    utStrAddF(caArgs,"&rssi=%d",psMac->urssi);
                }
            }
            else if(psOnline->cFrom == NCPORTAL_VENDOR_WIFIDOG) {
                strcpy(caProtocol,"HTTP");
                strcpy(caMethod,"GET");
                sprintf(caUrl,"http://%s:%u/wifidog/auth",
                    utComHostIp(htonl(psOnline->lGwip)),psOnline->nGwport);
                sprintf(caArgs,"token=%s",caSid);
            }
            else if(psOnline->cFrom == NCPORTAL_VENDOR_BCTH) {
                strcpy(caProtocol,"HTTP");
                strcpy(caMethod,"GET");
                
                sprintf(caUrl,"http://%s:%s/msa/main.xp",
                    utComHostIp(htonl(psOnline->lGwip)),psOnline->nGwport);                
                sprintf(caArgs,"Fun=msaLoginForThirdPart&mac=%s&ip=%s&token=%s",pasCvtMac(psOnline->caMac),utComHostIp(htonl(psOnline->lSip)),caSid);
            }
            else if(psOnline->cFrom == NCPORTAL_VENDOR_GBCOM) {
                ncPortalAcInfo *psAc;
                strcpy(caUserIp,utComHostIp(htonl(psOnline->lSip)));
                psAc = (ncPortalAcInfo *)ncSrvGetAcInfo(psShmHead,psOnline->caAcName);
                psOnline->cAuthWay = NCPORTAL_LOGIN_APPAUTH;
                if(psAc) {
                    if(strcasecmp(psAc->caAcType,"Ac")==0) {
                        ncPortalUser *psUser;
                        uint2 nReqId;
                        uint4 lReqIdNum;
                        uchar caChallenge[32],caChapPass[32];
                        psUser = (ncPortalUser *)ncSrvGetUserByName(psShmHead,caUsername,psOnline->lStype);
                        lReqIdNum = rand() % 32000;
                        psAc->nSn++;
                        if(ncSrvGetChapFlags() == 0) {
                            iReturn = ncSrvSendToAcRequestChallenge(psAc->lAcip,psAc->nAcPort,lReqIdNum,caUserIp,&nReqId,caChallenge);
                            pasLogsHex(PAS_SRCFILE,1008,caChallenge,16,"[ncSrvSendToAcRequestChallenge1]iReturn=%d Pass=%s",iReturn,psUser->caPass);
                            if(iReturn == 0) {
                                iReturn = ncSrvSendToAcRequesChapAuth(psAc->lAcip,psAc->nAcPort,lReqIdNum,nReqId,caUserIp,psOnline->caName,caChallenge,psUser->caPass,caChapPass);
                            }
                        }
                        else {
                            iReturn = ncSrvSendToAcRequesPapAuth(psAc->lAcip,psAc->nAcPort,lReqIdNum,0,caUserIp,psUser->caName,psUser->caPass);
                        }
                        pasLogs(PAS_SRCFILE,1008," ACAuth Name:%s   Ip:%s    iRet=%d ",psUser->caName,caUserIp,iReturn);
                        if(iReturn == 0 || iReturn ==2) { // 认证成功
                            iReturn = ncSrvSendToAcAuthAck(psAc->lAcip,psAc->nAcPort,lReqIdNum,nReqId,caUserIp);
                            ncSrvUpdateOnlineUserByUser(psShmHead,psOnline,psUser);
                            pasLogs(PAS_SRCFILE,1008," ACAuth Sucess  UserName:%s   Ip:%s    iRet=%d ",psUser->caName,caUserIp,iReturn);
                        }
                        else {
                            iStatus = NCAPP_STATUS_ERROR;
                        }
                    }
                    else {
                       iStatus = NCAPP_STATUS_ERROR;
                    }
                }
                else {
                    iStatus = NCAPP_STATUS_ERROR;
                }                    
            }
        }
        else {
            iStatus = NCAPP_STATUS_ERROR;
        }
    }
    else {
        iStatus = NCAPP_STATUS_NOONLINE;
    }
    pasStrCvtJson(caHtml,12,
                UT_TYPE_ULONG, "CHECK_TYPE",    iStatus,
                UT_TYPE_ULONG, "TIME",          psSumm->sAppAuth.lSessionTime,
                UT_TYPE_STRING,"NOTE",          caMsg,
                UT_TYPE_STRING,"URL",           psSumm->sAppAuth.reurl,
                UT_TYPE_STRING,"APP_URL",       psSumm->sAppAuth.reapp_url,
                UT_TYPE_ULONG, "NEXT_FLAGS",    iFlags,
                UT_TYPE_STRING,"NEXT_PROTOCOL", caProtocol,
                UT_TYPE_STRING,"NEXT_METHOD",   caMethod,
                UT_TYPE_STRING,"NEXT_URL",      caUrl,
                UT_TYPE_STRING,"NEXT_ARGS",     caArgs,
                UT_TYPE_ULONG, "STATUS",        iStatus,
                UT_TYPE_STRING,"data",      "[]");

    pasLogs(PAS_SRCFILE,1018,"Response:%s\n\n",caHtml);
    utComTcpResponse(iFd,psMsgHead,1,
		                       "text",UT_TYPE_STRING,caHtml);
    return 0;
}


// 手机号验证
int proAppGetAuthCode(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iReturn;
    char caHtml[1024];
    char caNum[20],caSid[32],caMsg[128];
	int iStatus = 0;

    utMsgOutMsgToLog(PAS_SRCFILE,1008, psMsgHead,"[proAppGetAuthCode] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead,2,
                    "NUM",          UT_TYPE_STRING,18,caNum,
                    "SESSION_ID",   UT_TYPE_STRING,32,caSid);
    iStatus = proAppDoGetAuthCode(psShmHead,caSid,caNum,caMsg);
    pasStrCvtJson(caHtml,3,
                UT_TYPE_ULONG, "STATUS",         iStatus,
                UT_TYPE_STRING,"ERRMSG",         caMsg,
                UT_TYPE_STRING,"data",      "[]");
                
    pasLogs(PAS_SRCFILE,1008,"Response:%s\n\n",caHtml);
    utComTcpResponse(iFd,psMsgHead,1,
		                       "text",UT_TYPE_STRING,caHtml);

}

// 终端认证
int proAppTermAuth(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{

    int iReturn;
    char caHtml[1024];
    char caMsg1[128];
    char caKey[32];
    char caNum[20],caAppSrc[20],caMac[20],caImei[16],caImsi[16],caOs[20],caSid[32];
    char caTerm[32],caUidsn[36],caUid[36],caMsg[128];
    uint8 llTsid;
	int iStatus = 0,iRet;
	char mac[6],caDisp[32];
	ncPortalOnline *psOnline;
    ncPortalSummary *psSumm;
    psSumm = (ncPortalSummary *)ncSrvGetSumm(psShmHead);
    caUidsn[0] = 0;
    caSid[0] = 0;
    caUid[0] = 0;
    caMsg[0] = 0;
    utMsgOutMsgToLog(PAS_SRCFILE,1008, psMsgHead,"[ncSrvAppDoGetIp] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead,9,
                    "NUM",          UT_TYPE_STRING,18,caNum,
                    "KEY",          UT_TYPE_STRING,31,caKey,
                    "APP_SRC",      UT_TYPE_STRING,16,caAppSrc,
                    "MAC",          UT_TYPE_STRING,17,caMac,
                    "IMEI",         UT_TYPE_STRING,15,caImei,
                    "IMSI",         UT_TYPE_STRING,15,caImsi,
                    "CLIENT_TYPE",  UT_TYPE_STRING,16,caTerm,
                    "OS_TYPE",      UT_TYPE_STRING,16,caOs,
                    "SESSION_ID",   UT_TYPE_STRING,32,caSid);
    utMsgOutMsgToLog(PAS_SRCFILE,1008,psMsgHead,"[proAppTermAuth] \n");
    if(utStrIsSpaces(caNum)) {
        iStatus = NCAPP_STATUS_ERRNUM;
      	pasGBKToUtf8("没有手机号码",caMsg,127);
    }
    else {
        if(!pasIsMacAddress(caMac)) {
            iStatus = NCAPP_STATUS_ERRMAC;
      	    pasGBKToUtf8("Mac地址出错",caMsg,127);
        }
        else {
            if(utStrIsSpaces(caSid)) {
                iStatus = NCAPP_STATUS_ERRSID;
          	    pasGBKToUtf8("SESSION_ID 出错",caMsg,127);
            }
            else {
                pasCvtMacI(caMac,mac);
                sscanf(caSid,"%llu",&llTsid);
                psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead,llTsid);
                if(psOnline) {
                    if(strcmp(psOnline->caKey,caKey)!=0) {
                        iStatus = NCAPP_STATUS_ERRKEY;
                  	    pasGBKToUtf8("密码不正确",caMsg,127);
                    }
                    else if(memcmp(psOnline->caMac,mac,6)!=0) {  // 同一个连接
                        iStatus = NCAPP_STATUS_ERRMAC;
                  	    pasGBKToUtf8("Mac地址不一致",caMsg,127);
                    }
                    else {
                        iStatus = proAppBindTermByMacNum(psShmHead,caNum,caNum,mac,caKey,caAppSrc,"PROEIM",caImei,caImsi,caTerm,caOs,1,caUid,caUidsn,caMsg1);
                        if(iStatus == 0 || iStatus == 1) { // 成功
                            psOnline->psUser = ncSrvSynUserPassword(psShmHead,psOnline->lStype,psOnline->lGroupid,caNum,caKey,psOnline->caGroupCode,psOnline->caSsid,
                                            NCSRV_USERTYPE_SMS);
                            if(ncSrvGetUpdateUserBy() == 2) {
                                iReturn = ncSrvSynUserPassword2RadSrv(psOnline->psPar,caNum,caKey,psOnline->lStype);
                                pasLogs(PAS_SRCFILE,1008," Update User [%s]  Pass:[%s] Group:%s Ssid:%s  Stype:%u Ret=%d ",
                                        caNum,caKey,psOnline->caGroupCode,psOnline->caSsid,psOnline->lStype,iReturn);
                            }                            
                        }
                        else {   // 绑定失败
                  	        pasGBKToUtf8(caMsg1,caMsg,127);
                        }
                    }
                }
                else {
                    iStatus = NCAPP_STATUS_NOONLINE;
               	    pasGBKToUtf8("用户不在线",caMsg,127);
                }                    
            }
        }
    }
    if(iStatus == 0 || iStatus == 1) {
        pasStrCvtJson(caHtml,5,
                UT_TYPE_STRING,"UID_SN",        caUidsn,
                UT_TYPE_STRING,"UID",           caUid,
                UT_TYPE_ULONG, "STATUS",        iStatus,
                UT_TYPE_STRING, "ERRMSG",        "\0",
                UT_TYPE_STRING,"data",      "[]");
                
    }
    else {                
        pasStrCvtJson(caHtml,5,
                UT_TYPE_STRING,"UID_SN",        caUidsn,
                UT_TYPE_STRING,"UID",           caUid,
                UT_TYPE_ULONG, "STATUS",        iStatus,
                UT_TYPE_STRING,"ERRMSG",        caMsg,
                UT_TYPE_STRING,"data",      "[]");
    }
    pasLogs(PAS_SRCFILE,1018,"Response:%s\n\n",caHtml);
    utComTcpResponse(iFd,psMsgHead,1,
		                       "text",UT_TYPE_STRING,caHtml);
    return 0;
}

// 终端绑定
int proAppTermBind(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{

    int iReturn;
    char caHtml[1024];
    char caMsg1[128];
    char caKey[32];
    char caNum[20],caAppSrc[20],caMac[20],caImei[16],caImsi[16],caOs[20],caSid[32];
    char caTerm[32],caUidsn[36],caUid[36],caMsg[128];
    uint8 llTsid;
	int iStatus = 0,iRet;
	char mac[6],caDisp[32];
	ncPortalOnline *psOnline;
    ncPortalSummary *psSumm;
    psSumm = (ncPortalSummary *)ncSrvGetSumm(psShmHead);
    utMsgOutMsgToLog(PAS_SRCFILE,1008, psMsgHead,"[proAppTermBind] \n");
    strcpy(caKey,"\0");
    caUidsn[0] = 0;
    caUid[0] = 0;
    caMsg[0] = 0;
    iStatus = 0;
    
    iReturn = utMsgGetSomeNVar(psMsgHead,9,
                    "NUM",          UT_TYPE_STRING,18,caNum,
                    "KEY",          UT_TYPE_STRING,31,caKey,
                    "APP_SRC",      UT_TYPE_STRING,16,caAppSrc,
                    "MAC",          UT_TYPE_STRING,17,caMac,
                    "IMEI",         UT_TYPE_STRING,15,caImei,
                    "IMSI",         UT_TYPE_STRING,15,caImsi,
                    "CLIENT_TYPE",  UT_TYPE_STRING,16,caTerm,
                    "OS_TYPE",      UT_TYPE_STRING,16,caOs,
                    "SESSION_ID",   UT_TYPE_STRING,32,caSid);
    if(utStrIsSpaces(caNum)) {
        iStatus = NCAPP_STATUS_ERRNUM;
      	pasGBKToUtf8("没有手机号码",caMsg,127);
    }
    else {
        if(proAppIsValidApp(caAppSrc)) {
            if(!pasIsMacAddress(caMac)) {
                iStatus = NCAPP_STATUS_ERRMAC;
      	        pasGBKToUtf8("Mac地址出错",caMsg,127);
            }
            else {
                if(utStrIsSpaces(caSid)) {
                    iStatus = NCAPP_STATUS_ERRSID;
          	        pasGBKToUtf8("SESSION_ID 出错",caMsg,127);
                }
                else {
                    pasCvtMacI(caMac,mac);
                    sscanf(caSid,"%llu",&llTsid);
                    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead,llTsid);
                    if(psOnline) {
                        if(memcmp(psOnline->caMac,mac,6)!=0) {  // 同一个连接
                            iStatus = NCAPP_STATUS_ERRMAC;
            	            pasGBKToUtf8("Mac地址不一致",caMsg,127);
                        }
                        else {
                            iRet = proAppGetUidByMac(psShmHead,caNum,caMac,caUid,caUidsn);
                            if(iRet == 1) { // 已经存在
                                iStatus = 1 ;
                            }
                            else {
                                if(proAppIsNoCheckApp(caAppSrc)) { // 不需要检查密码
                                    iStatus = proAppBindTermByMacNum(psShmHead,caNum,caNum,mac,caKey,caAppSrc,"PROEIM",caImei,caImsi,caTerm,caOs,1,caUid,caUidsn,caMsg1);
                                    if(iStatus == 0 || iStatus == 1) { // 成功
                        
                                    }
                                    else {   // 绑定失败
                      	                pasGBKToUtf8(caMsg1,caMsg,127);
                                    }
                                }
                                else {
                                    if(utStrIsSpaces(caKey) || strcmp(psOnline->caKey,caKey)!=0) {
                                        iStatus = NCAPP_STATUS_ERRKEY;
                  	                    pasGBKToUtf8("密码不一致",caMsg,127);
                                    }
                                    else {
                                        iStatus = proAppBindTermByMacNum(psShmHead,caNum,caNum,mac,caKey,caAppSrc,"PROEIM",caImei,caImsi,caTerm,caOs,1,caUid,caUidsn,caMsg1);
                                        if(iStatus == 0 || iStatus == 1) { // 成功
                        
                                        }
                                        else {   // 绑定失败
                      	                    pasGBKToUtf8(caMsg1,caMsg,127);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else {
                        iStatus = NCAPP_STATUS_NOONLINE;
               	        pasGBKToUtf8("用户不在线",caMsg,127);
                    }                    
                }
            }
        }
        else {
            iStatus = NCAPP_STATUS_ERRAPP;
            pasGBKToUtf8("非法的APP",caMsg,127);
        }
    }
    if(iStatus == 0 || iStatus == 1) {
        pasStrCvtJson(caHtml,4,
                UT_TYPE_STRING,"UID_SN",        caUidsn,
                UT_TYPE_STRING,"UID",           caUid,
                UT_TYPE_ULONG, "STATUS",        iStatus,
                UT_TYPE_STRING,"data",      "[]");
                
    }
    else {                
        pasStrCvtJson(caHtml,5,
                UT_TYPE_STRING,"UID_SN",        caUidsn,
                UT_TYPE_STRING,"UID",           caUid,
                UT_TYPE_ULONG, "STATUS",        iStatus,
                UT_TYPE_STRING,"ERRMSG",        caMsg,
                UT_TYPE_STRING,"data",      "[]");
    }
    pasLogs(PAS_SRCFILE,1018,"Response:%s\n\n",caHtml);
    utComTcpResponse(iFd,psMsgHead,1,
		                       "text",UT_TYPE_STRING,caHtml);
    return 0;
}

// 心跳信息
int proAppHeat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iReturn;
    char caHtml[1024];
    char caUidsn[32],caSid[32],caMsg[128];
	int iStatus = 0;
	uint8 llTsid;
    ncPortalOnline  *psOnline=NULL;
    caUidsn[0] = 0;
    caSid[0] = 0;
    caMsg[0] = 0;

    utMsgOutMsgToLog(PAS_SRCFILE,1008, psMsgHead,"[proAppHeat] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead,2,
                    "UID_SN",       UT_TYPE_STRING,32,caUidsn,
                    "SESSION_ID",   UT_TYPE_STRING,32,caSid);
                        
    sscanf(caSid,"%llu",&llTsid);
    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead,llTsid);
    if(psOnline == NULL) {
        iStatus = NCAPP_STATUS_NOONLINE;
    }
    else {
        if(psOnline->login == NCPORTAL_ONLINE_LOGIN) {
            iStatus = NCAPP_STATUS_ONLINE;
        }
        else {
            iStatus = NCAPP_STATUS_NOLOGIN;
        }
        psOnline->lasttime = time(0);
    }
    pasStrCvtJson(caHtml,3,
                UT_TYPE_ULONG, "STATUS",         iStatus,
                UT_TYPE_STRING,"ERRMSG",         caMsg,
                UT_TYPE_STRING,"data",      "[]");
    pasLogs(PAS_SRCFILE,1008,"Response:%s\n\n",caHtml);
    utComTcpResponse(iFd,psMsgHead,1,
		                       "text",UT_TYPE_STRING,caHtml);

}

// 解除终端绑定

int proAppTermUnBind(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iReturn;
    char caUidsn[33];
    char caSid[32];
    ncPortalOnline *psOnline;
    ncPortalUser *psUser;
    uint8 llTsid;
    iReturn = utMsgGetSomeNVar(psMsgHead,2,
                    "UID_SN",       UT_TYPE_STRING,32,caUidsn,
                    "SESSION_ID",   UT_TYPE_STRING,32,caSid);
    sscanf(caSid,"%llu",&llTsid);
    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead,llTsid);
    if(psOnline) {
        strcpy(psOnline->caKey,"\0");
         psUser = (ncPortalUser *)ncSrvGetUserByName(psShmHead,psOnline->caName,psOnline->lStype);
        if(psUser) {
            strcpy(psUser->caPass,"\0");
            psUser->cMod = 1;
        }
    }
    iReturn = pasDbExecSqlF("delete from ncmuserbind where uid_sn = '%s'",caUidsn);
    proAppLogout(psShmHead,iFd,psMsgHead);
    return 0;
}

int proAppLogout(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    
    int iReturn;
    char caHtml[1024];
    char caUidsn[32],caSid[32],caMsg[128];
    char caIp[20];
	int iStatus = 0,iFlags=1;
	uint8 llTsid;
    char caProtocol[16],caMethod[16],caUrl[128],caArgs[128];
    ncPortalOnline  *psOnline=NULL;
    int iRet;
    utMsgOutMsgToLog(PAS_SRCFILE,1008, psMsgHead,"[proAppLogout] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead,2,
                    "UID_SN",       UT_TYPE_STRING,32,caUidsn,
                    "SESSION_ID",   UT_TYPE_STRING,32,caSid);
    sscanf(caSid,"%llu",&llTsid);
    strcpy(caProtocol,"HTTP");
    strcpy(caMethod,"GET");
    sprintf(caUrl,"\0");
    sprintf(caArgs,"\0");
    
    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead,llTsid);
    if(psOnline == NULL) {

        iStatus = NCAPP_STATUS_NOONLINE;
        pasStrCvtJson(caHtml,7,
                UT_TYPE_ULONG, "NEXT_FLAGS",    iFlags,
                UT_TYPE_STRING,"NEXT_PROTOCOL", caProtocol,
                UT_TYPE_STRING,"NEXT_METHOD",   caMethod,
                UT_TYPE_STRING,"NEXT_URL",      caUrl,
                UT_TYPE_STRING,"NEXT_ARGS",     caArgs,
                UT_TYPE_ULONG, "STATUS",        iStatus,
                UT_TYPE_STRING,"data",      "[]");
                
    }
    else {
        strcpy(caProtocol,"HTTP");
        strcpy(caMethod,"GET");
        strcpy(caIp,utComHostIp(htonl(psOnline->lSip)));
        
        sprintf(caUrl,"http://%s:%u/wifidog/userlogout?ip=%s&mac=%s",
                utComHostIp(htonl(psOnline->lGwip)),psOnline->nGwport,
                caIp,pasCvtMac(psOnline->caMac));

// http://x.x.x.x:8060/wifidog/userlogout?ip=x.x.x.x&mac=xx:xx:xx:xx:xx:xx
//
// http://x.x.x.x:8060/wifidog/userlogout?ip=x.x.x.x&mac=xx:xx:xx:xx:xx:xx>

        sprintf(caArgs,"logout=1&token=%s",caSid);
        ncSrvUserLogout(psShmHead,psOnline,NCPORTAL_CAUSE_USERREQUEST);
        iStatus = NCAPP_STATUS_ONLINE;
        pasStrCvtJson(caHtml,7,
                UT_TYPE_ULONG, "NEXT_FLAGS",    iFlags,
                UT_TYPE_STRING,"NEXT_PROTOCOL", caProtocol,
                UT_TYPE_STRING,"NEXT_METHOD",   caMethod,
                UT_TYPE_STRING,"NEXT_URL",      caUrl,
                UT_TYPE_STRING,"NEXT_ARGS",     caArgs,
                UT_TYPE_ULONG, "STATUS",        iStatus,
                UT_TYPE_STRING,"data",      "[]");
    }
    pasLogs(PAS_SRCFILE,1008,"Response:%s\n\n",caHtml);
    utComTcpResponse(iFd,psMsgHead,1,
		                       "text",UT_TYPE_STRING,caHtml);
    
}

/*
    检查用户的MAC地址是否正确
    1--正确
    0--不存在
    2--不正确
*/
int proAppCheckUserByUidMac(utShmHead *psShmHead,char *pUidsn,char *pMac,char *pUid,char *pUsername)
{
    int iReturn;
    char caMac16[20];
    char caMac[20];

    strcpy(caMac16,(char *)pasCvtMac12(pMac));
    iReturn = pasDbOneRecord("select username,uid,user_mac from ncmuserbind where uid_sn = :uid_sn",
            1,"uid_sn",UT_TYPE_STRING,pUidsn,
            UT_TYPE_STRING,31,pUsername,
            UT_TYPE_STRING,32,pUid,
            UT_TYPE_STRING,17,caMac);
    if(iReturn == 0) {
        if(strcasecmp(caMac16,caMac)==0) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 9;
    }
}

int proAppGetUidByMac(utShmHead *psShmHead,char *pNum,char *pMac,char *pUid,char *pUidSn)
{
    int iReturn;
    char caMac[20];
    char caUid[36],caUidSn[36];
    iReturn = pasDbOneRecord("select uid from ncmuserinfo where username = :username",
            1,"username",UT_TYPE_STRING,pNum,
            UT_TYPE_STRING,32,caUid);
    if(iReturn == 0) {
        iReturn = pasDbOneRecord("select uid_sn from ncmuserbind where username = :username and user_mac = :mac",
            2,"username",UT_TYPE_STRING,pNum,
              "user_mac",UT_TYPE_STRING,pMac,
            UT_TYPE_STRING,32,caUidSn);
        if(iReturn == 0) {
            strcpy(pUid,caUid);
            strcpy(pUidSn,caUidSn);
            return 1;
        }
        else {
            strcpy(pUid,caUid);
            strcpy(pUidSn,"\0");
            return 0;
        }
    }
    else {
        strcpy(pUid,"\0");
        strcpy(pUidSn,"\0");
        return 9;
    }
}
    
/*
    Mac地址为6位二进制
        
*/
int proAppBindTermByMacNum(utShmHead *psShmHead,char *pNum,char *pDisp,char *pMac,char *pPass,
                char *app_src,char *sj_src,char *pImei,char *pImsi,char *pTerm,char *pOs,int iType,
                char *pUid,char *pUidSn,char *pMsg)
{
    int iReturn,iRet,iCount;
    char caMac[20],caIn[32],caUid[32];
    uint4 lTime;
    lTime = time(0);
    iReturn = pasDbOneRecord("select uid from ncmuserinfo where username = :username",
            1,"username",UT_TYPE_STRING,pNum,
            UT_TYPE_STRING,32,pUid);
    pasLogs(PAS_SRCFILE,1008,"iReturn=%d uid=%s ",iReturn,pUid);
    if(iReturn == 1403) {
        iCount = 0;
        while(iCount < 10) {
            sprintf(caIn,"%s%u",pNum,iCount);
            utMd5Encrypto(caIn,strlen(caIn),caUid);
            iRet = pasDbExecSqlF("insert into ncmuserinfo (uid,username,dispname,password,usertype,addtime,lasttime) value ('%s','%s','%s','%s',%d,%lu,%lu)",
                            caUid,pNum,pDisp,pPass,iType,lTime,lTime);
            if(iRet != 0) {
                pasLogs(PAS_SRCFILE,1008,"iReturn=%d %s ",iReturn,pasDbErrorMsg(0));
                iCount ++;
            }
            else {
                strcpy(pUid,caUid);
                break;
            }
        }
        if(iRet != 0) {
            sprintf(pMsg,"账号信息插入失败");
            return 9;
        }
    }
    else if(iReturn != 0) {
        sprintf(pMsg,"账号数据库出错");
        return 9;
    }

    strcpy(caMac,(char *)pasCvtMac12(pMac));
    iReturn = pasDbOneRecord("select uid_sn from ncmuserbind where username = :username and user_mac = :mac",
            2,"username",UT_TYPE_STRING,pNum,
              "mac",UT_TYPE_STRING,caMac,
            UT_TYPE_STRING,32,pUidSn);
    if(iReturn == 0) { // 
        return 1;
    }
    else if(iReturn == 1403) { // 绑定
        iCount = 0;
        while(iCount < 10) {
            sprintf(caIn,"%s%s%d",pNum,caMac,iCount);
            utMd5Encrypto(caIn,strlen(caIn),pUidSn);
            iRet = pasDbExecSqlF("insert into ncmuserbind (uid_sn,username,dispname,app_src,sj_src,user_mac,imei,imsi,term,os,uid,addtime,lasttime,flags) value ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%u,%u,%d)",
                    pUidSn,pNum,pDisp,app_src,sj_src,caMac,pImei,pImsi,pTerm,pOs,pUid,lTime,lTime,0);
            if(iRet != 0) {
                pasLogs(PAS_SRCFILE,1008,"ncmuserbind iReturn=%d %s ",iReturn,pasDbErrorMsg(0));
                iCount ++;
            }
            else {
                return 0;
            }
        }
        sprintf(pMsg,"账号和终端绑定失败");
        return 9;
    }
    else {
        pasLogs(PAS_SRCFILE,1008,"Error:iReturn=%d %s",iReturn,pasDbErrorMsg(NULL));
        sprintf(pMsg,"账号和终端绑定失败");
        return 9;
    }
}


// 检查App来源的有效性
int proAppCheckAppSrc(utShmHead *psShmHead,char *pAppSrc)
{
    if(strcasecmp(pAppSrc,"PROEIM")==0 || strcasecmp(pAppSrc,"LIANSHANG")==0) {
        return 1;
    }
    else {
        return 0;
    }
}

/*
    同步在线信息
 */
int proAppSynOnlineUser(utShmHead *psShmHead,ncPortalOnline *psOnline)
{
    return 0;
}



// 获取短信密码
int proAppDoGetAuthCode(utShmHead *psShmHead,char *pSid,char *pNum,char *pMsg)
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
    char caLang[16],caCode[8];
    char caTsid[24];
    char caArea[32];
    int iReturn,iRet;
    long long lSid=0;
    int iSum,iAreaCode=0,nCurNum,iReSend=0,iVendor=0;
    uint2 nReqId=0,nDport;
    uint4  nPort=2000,lDip,lCode,lTime,lStatus=0,iSp,iArea,lGroupid=0,lStype=0;
    uint8 llTsid;
    unsigned long lSip;
    int iStatus=0;
    char   caSn[32];
    char caNasid[24];
    char caStype[8];
    ncPortalSummary *psSumm;
    ncPortalOnline  *psOnline=NULL;
    char caSmsName[32];
    char caServicecode[32];
    utMsgHead *psMsgHead0;

    strcpy(caSn,utComGetVar_sd(psShmHead,"ProductSN","\0"));
    lTime = time(0);
    psSumm = (ncPortalSummary *)ncSrvGetSumm(psShmHead);
    sscanf(pSid,"%llu",&llTsid);
    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead,llTsid);
    if(psOnline == NULL) {
        iStatus = NCAPP_STATUS_NOONLINE;
        return iStatus;
    }
    if(psOnline->psAp && psOnline->psAp->psGroup) {  // 短信计费单位
        strcpy(caSmsName,psOnline->psAp->psGroup->caSmsName);
    }
    else {
        strcpy(caSmsName,utComGetVar_sd(psShmHead,"MySmsName","None"));
    }
    /*  手机号码的有效性检查
        +86-1234567
    */
    iAreaCode =  ncPortalCheckMobileNumber(pNum,caName,caArea,caSpname,&iArea,&iSp);
    if(iAreaCode <= 0) {
        pasLogs(PAS_SRCFILE,1008," Username: %s Number Error AreaCode=%d",pNum,iAreaCode);
        iStatus = NCAPP_STATUS_ERRNUM;
        return iStatus;
    }
    strcpy(caApMac,psOnline->caApName);
    lStype = psOnline->lStype;
    if(ncSrvPortalIsBlackUser(psShmHead,psOnline->lStype,NCPORTAL_BLACK_USER,caName)) {
        iStatus = NCAPP_STATUS_BLACKNUM;
        return iStatus;        
    }
    if(ncSrvPortalIsBlackUser(psShmHead,psOnline->lStype,NCPORTAL_BLACK_MAC,psOnline->caMac)) {
        iStatus = NCAPP_STATUS_BLACKMAC;
        return iStatus;        
    }
    srand(time(0));
    lCode = rand() % 999999L;
    sprintf(caPass,"%06lu",lCode);
    caPass[6]=0;
    strcpy(psOnline->caKey,caPass);
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
            else if(psSumm->psSmsServer[nCurNum].nType == NCSRV_SMSTYPE_CENTER) {  // 集中验证，代发短信
                char caFcode[32],caProd[32];
                ncSrvGetSmsMsg(psShmHead,psOnline,caLang,caMsg);
                utStrReplaceWith(caMsg,"[#username#]",caName);
                utStrReplaceWith(caMsg,"[#password#]",caPass);
                strcpy(caProd,utComGetVar_sd(psShmHead,"ProductSN","\0"));
                if(psOnline && psOnline->psAp && psOnline->psAp->psGroup) {
                    strcpy(caServicecode,psOnline->psAp->psGroup->caGroupName);
                    strcpy(caSmsName,psOnline->psAp->psGroup->caSmsName);
                    if(utStrIsSpaces(caServicecode)) {
                        strcpy(caServicecode,utComGetVar_sd(psShmHead,"ServiceCode","00000000000000"));
                    }
                }
                else {
                    strcpy(caServicecode,utComGetVar_sd(psShmHead,"ServiceCode","00000000000000"));
                    strcpy(caSmsName,"pronetway");
                }
                strcpy(caCode,"\0");
                strcpy(caFcode,utComGetVar_sd(psShmHead,"CenterAuthFcode","02"));
                iRet = ncSrvSendSmsByCenter(&psSumm->psSmsServer[nCurNum],caFcode,caSmsName,pasCvtMac(psOnline->caMac),
                                    caServicecode,caCode,caName,caPass,caMsg,caProd);
                pasLogs(PAS_SRCFILE,1008,"ncSrvSendSmsByCenter Msg:%s ServiceCode:%s SmsName:%s  Ret=%d",caMsg,caServicecode,caSmsName,iRet);
                break;
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
            else if(psSumm->psSmsServer[nCurNum].nType == NCSRV_SMSTYPE_CENTER) {  // 集中验证，代发短信
                char caFcode[32],caServiceCode[32],caProd[32];

                ncSrvGetSmsMsg(psShmHead,psOnline,caLang,caMsg);
                utStrReplaceWith(caMsg,"[#username#]",caName);
                utStrReplaceWith(caMsg,"[#password#]",caPass);
                strcpy(caProd,utComGetVar_sd(psShmHead,"ProductSN","\0"));
                if(psOnline && psOnline->psAp && psOnline->psAp->psGroup) {
                    strcpy(caServiceCode,psOnline->psAp->psGroup->caGroupName);
                    strcpy(caSmsName,psOnline->psAp->psGroup->caSmsName);
                    if(utStrIsSpaces(caServicecode)) {
                        strcpy(caServicecode,utComGetVar_sd(psShmHead,"ServiceCode","00000000000000"));
                    }
                }
                else {
                    strcpy(caServicecode,utComGetVar_sd(psShmHead,"ServiceCode","00000000000000"));
                    strcpy(caSmsName,"pronetway");
                }
                pasLogs(PAS_SRCFILE,1008,"ncSrvSendSmsByCenter Msg:%s ServiceCode:%s SmsName:%s",caMsg,caServicecode,caSmsName);
                sprintf(caCode,"%u",iAreaCode);
                strcpy(caFcode,utComGetVar_sd(psShmHead,"CenterAuthFcode","02"));
                iRet = ncSrvSendSmsByCenter(&psSumm->psSmsServer[nCurNum],caFcode,caSmsName,pasCvtMac(psOnline->caMac),
                                        caServiceCode,caCode,caName,caPass,caMsg,caProd);
                break;
            }
        }            
    }
    psOnline->psUser = ncSrvSynUserPassword(psShmHead,psOnline->lStype,psOnline->lGroupid,caName,caPass,psOnline->caGroupCode,psOnline->caSsid,NCSRV_USERTYPE_SMS);
    if(ncSrvGetUpdateUserBy() == 2) {
        ncSrvSynUserPassword2RadSrv(psOnline->psPar,caName,caPass,psOnline->lStype);
        pasLogs(PAS_SRCFILE,1008," Update User [%s]  Pass:[%s] Group:%s Ssid:%s  Stype:%u Ret=%d ",
                          caName,caPass,psOnline->caGroupCode,psOnline->caSsid,psOnline->lStype,iReturn);
    }
    strcpy(caGroupCode,psOnline->caGroupCode);
    if(utStrIsSpaces(caUserIp)) {
        strcpy(caUserIp,utComHostIp(htonl(psOnline->lSip)));
    }
    if(!utStrIsSpaces(caGroupCode)) {
        memcpy(caStype,caGroupCode+8,2);
        caStype[2] = 0;
    }
    else {
        caStype[0] = 0;
    }
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
}

