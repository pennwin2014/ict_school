/* 
    针对环创AP
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

int ncSrvAppGetInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncSrvAppCheck(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncSrvAppLoginAuth(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int proAppGetIp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int proAppGetInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int proAppCheck(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int proAppTermBind(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int proAppTermUnBind(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int proAppLogout(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int proAppGetAuthCode(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int proAppHeat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int proAppTermAuth(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncSrvSetAppFun(utShmHead *psShmHead)
{
    char *p;
    pasSetTcpFunName("ncAppGetInfo",          ncSrvAppGetInfo,     0);         
    pasSetTcpFunName("ncAppCheck",            ncSrvAppCheck,       0);
    pasSetTcpFunNameS("ncSrvAppLoginAuth",       ncSrvAppLoginAuth,      NULL,                                0,PAS_CRYPT_TEA);

    // APP认证功能
    pasSetTcpFunName("proAppGetIp",            proAppGetIp,       0);
    pasSetTcpFunName("proAppGetInfo",          proAppGetInfo,       0);
    pasSetTcpFunName("proAppCheck",            proAppCheck,       0);
    pasSetTcpFunName("proAppTermBind",         proAppTermBind,       0);
    pasSetTcpFunName("proAppTermUnBind",       proAppTermUnBind,       0);
    pasSetTcpFunName("proAppLogout",           proAppLogout,       0);
    pasSetTcpFunName("proAppGetAuthCode",      proAppGetAuthCode,       0);
    pasSetTcpFunName("proAppHeat",             proAppHeat,       0);
    pasSetTcpFunName("proAppTermAuth",         proAppTermAuth,   0);
    
    p = utComGetVar_s(psShmHead,"AppNoCheckCode");
    if(p) {
        proAppSetNoCheckApp(p);
    }
    p = utComGetVar_s(psShmHead,"AppValidAppSrc");
    if(p) {
        proAppIsValidApp(p);
    }
    return 0;
}

// APP登录

int ncSrvAppLoginAuth(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char    caIp[32],caMac[32],caUsername[32],caPassword[32],caGwid[32],caGwip[32],caGwport[16];
    char    caSsid[32],caAcname[32],caSitename[32];
    char    caAcName0[64];
    char    mac[6];
    char    caUrl[256];
    int     iStatus=0,iReturn;
    uint4   lSip;
    ncPortalOnline *psOnline; 
    ncPortalAcInfo *psAc;
    
    utMsgOutMsgToLog(PAS_SRCFILE,1018,psMsgHead,"[ncSrvAppLoginAuth] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead,10,
                    "ip",       UT_TYPE_STRING,31,caIp,
                    "mac",      UT_TYPE_STRING,31,caMac,
                    "username", UT_TYPE_STRING,31,caUsername,
                    "password", UT_TYPE_STRING,31,caPassword,
                    "gwid",     UT_TYPE_STRING,31,caGwid,
                    "gwip",     UT_TYPE_STRING,31,caGwip,
                    "gwport",   UT_TYPE_STRING,15,caGwport,
                    "ssid",     UT_TYPE_STRING,31,caSsid,
                    "acname",   UT_TYPE_STRING,31,caAcname,
                    "sitename", UT_TYPE_STRING,31,caSitename);
    
    pasCvtMacI(caMac,mac);
    lSip = ntohl(pasIpcvtLong(caIp));
    strcpy(caUrl,"\0");
    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByNasidIpA(psShmHead,caSitename,caGwid,lSip,mac,caSsid,caGwip,caGwport,0);
    if(psOnline) {
        
        sprintf(caUrl,"http://%s:%s/pronline/Msg?FunName=ncSrvLoginError&tsid=%llu",
                utComGetVar_sd(psShmHead,"AdminIp","127.0.0.1"),
                utComGetVar_sd(psShmHead,"AdminPort","9080"),
                psOnline->lTsid);
        if(utStrIsSpaces(psOnline->caAcName)) {
            strcpy(psOnline->caAcName,caAcname);
        }
        sprintf(caAcName0,"App_%s",caAcname);
        psAc = (ncPortalAcInfo *)ncSrvGetAcInfo(psShmHead,caAcName0);
        pasLogs(PAS_SRCFILE,1018,"AcName0==%s  ErrUrl:%s  psAc=%s",caAcName0,caUrl,psAc?"OK":"NoFound");
        if(psAc) {
            psOnline->lAcIp = psAc->lAcip;
            psOnline->nAcPort = psAc->nAcPort;
            strcpy(psOnline->caKey,psAc->caKey);
            psOnline->cAuthType = psAc->iAuthType;
            psOnline->psAc = psAc;
            
            pasLogs(PAS_SRCFILE,1018,"AcIp:%s:%u  Type:%s", utComHostIp(htonl(psOnline->lAcIp)),psOnline->nAcPort);
            if(strcasecmp(psAc->caAcType,"Radius")==0) {
                if(psOnline->psAc) {
                    ncPortalUser *psUser;
                    char *p;
                    char caUsername1[64];
                    p = strstr(psOnline->psAc->caDomain,"@");
                    if(p) {
                        if(psOnline->psAc->caDomain[0] == '@') {
                            sprintf(caUsername1,"%s%s",caUsername,psOnline->psAc->caDomain);
                        }
                        else {
                            sprintf(caUsername1,"%s%s",psOnline->psAc->caDomain,caUsername);
                        }
                    }
                    else {
                        strcpy(caUsername1,caUsername);
                    }
                    iReturn = ncSrvLogin_AuthMeru(psShmHead,psOnline,caIp,caUsername1,caPassword);
                    pasLogs(PAS_SRCFILE,1018," Radius Name:%s    %s Ip:%s    iRet=%d ",caUsername1,caUsername,caIp,iReturn); 
                    if(iReturn == 2) {  // 验证通过
                        strcpy(psOnline->caName,caUsername);
                        psUser = (ncPortalUser *)ncSrvSynUserPassword(psShmHead,psOnline->lStype,psOnline->lGroupid,caUsername,caPassword,
                                        "\0",psOnline->caSsid,NCSRV_USERTYPE_APP);
                        if(psUser) {
                            psOnline->psUser = psUser;
                        }          
                        iStatus = 1;        
                        sprintf(caUrl,"http://%s:%s/pronline/Msg?FunName=ncSrvLoginOK&tsid=%llu",
                        utComGetVar_sd(psShmHead,"AdminIp","127.0.0.1"),
                        utComGetVar_sd(psShmHead,"AdminPort","9080"),
                        psOnline->lTsid);
                        ncSrvUpdateOnlineUserByOnline(psShmHead,psOnline,psOnline->caName,psOnline->caName);     
                        psOnline->cAuthWay = NCPORTAL_LOGIN_APPAUTH;
                    }
                    else {  // 验证失败
                        iStatus = 0;
                    }
                }
            }
        }
        
    }
    pasLogs(PAS_SRCFILE,1018,"Response: Status:%d  Url:%s",iStatus,caUrl);
    pasTcpResponse(iFd,psMsgHead,
                    pasGetCurComKey(),                /* 密钥，暂时不用  */
                    2,
                    "status",    UT_TYPE_LONG,  iStatus,
                    "url",  UT_TYPE_STRING,caUrl);
}
//  
/*
  POST /pronline/Msg?FunName@ncAppGetInfo&ip=192.168.20.237&mac=3c:43:8e:d4:53:92&mode=EID&geturl=1 HTTP/1.1<0d><0a>
  Content-Type: application/x-www-form-urlencoded<0d><0a>
  User-Agent: Dalvik/1.6.0 (Linux; U; Android 4.1.2; XT928 Build/6.7.2_GC-383)<0d><0a>
  Host: 192.168.20.29:9080<0d><0a>
  Connection: Keep-Alive<0d><0a>
  Accept-Encoding: gzip<0d><0a>
  Content-Length: 0<0d><0a>
  <0d><0a>
  
  */

int ncSrvAppGetInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead;
    ncPortalOnline *psOnline;
    int iReturn,iUrlFlags;
    char caHtml[1024];
    char caDisp[128],caGroupName[32];
    char caOut[128],caIp[32],caMac[32],caUrlflag[8];
    char    caPassurl[128],caFailurl[128];
    char mac[6];
    uint4 lSip;
	int iStatus = 0;
    utMsgOutMsgToLog(PAS_SRCFILE,1018,psMsgHead,"[ncAppGetInfo] \n");
    strcpy(caIp,"\0");
    iReturn = utMsgGetSomeNVar(psMsgHead,3,
                    "ip",       UT_TYPE_STRING,31,caIp,
                    "mac",      UT_TYPE_STRING,31,caMac,
                    "geturl",   UT_TYPE_STRING,2, caUrlflag);
    lSip =  ntohl(pasIpcvtLong(caIp));
    iUrlFlags = atol(caUrlflag);    
    pasCvtMacI(caMac,mac);
    psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByMac(psShmHead,mac);
    if(psOnline) {
        if(psOnline->login == NCPORTAL_ONLINE_LOGIN && psOnline->lSip == lSip) {
            iStatus = 1;
        }
    }
    if(psOnline && psOnline->psAp && psOnline->psAp->psGroup) {
    	pasGBKToUtf8(psOnline->psAp->psGroup->caDisp,caOut,127);
    	strcpy(caGroupName,psOnline->psAp->psGroup->caGroupName);
    }
    else {
        strcpy(caGroupName,"\0");
        strcpy(caOut,"\0");
    }
    strcpy(caPassurl,utComGetVar_sd(psShmHead,"passurl","\0"));
	strcpy(caFailurl,utComGetVar_sd(psShmHead,"failurl","\0"));

	if(iUrlFlags == 1) {
       	sprintf(caHtml,"{\"bid\":\"%s\",\"bn\":\"%s\",\"ct\":\"2\",\"stat\":\"%d\",\"passurl\":\"%s\"}",
                         caGroupName,caOut,iStatus,caPassurl);
	}
	else if(iUrlFlags == 2) {
       	sprintf(caHtml,"{\"bid\":\"%s\",\"bn\":\"%s\",\"ct\":\"2\",\"stat\":\"%d\",\"failurl\":\"%s\"}",
                         caGroupName,caOut,iStatus,caFailurl);
	}
	else if(iUrlFlags == 9) {
       	sprintf(caHtml,"{\"bid\":\"%s\",\"bn\":\"%s\",\"ct\":\"2\",\"stat\":\"%d\",\"passurl\":\"%s\",\"failurl\":\"%s\"}",
                         caGroupName,caOut,iStatus,caPassurl,caFailurl);
	}
	else {
       	sprintf(caHtml,"{\"bid\":\"%s\",\"bn\":\"%s\",\"ct\":\"2\",\"stat\":\"%d\"}",
                         caGroupName,caOut,iStatus);
    }
    pasLogs(PAS_SRCFILE,1018,"[ncAppGetInfo] Response:%s \n\n",caHtml);
    utComTcpResponse(iFd,psMsgHead,1,
		                       "text",UT_TYPE_STRING,caHtml);
    return 0;
}


/*

POST /pronline/Msg?FunName@ncAppCheck&username=18001625009&password=986495 HTTP/1.1<0d><0a>
Content-Type: application/x-www-form-urlencoded<0d><0a>
User-Agent: Dalvik/1.6.0 (Linux; U; Android 4.1.2; XT928 Build/6.7.2_GC-383)<0d><0a>
Host: 192.168.20.29:9080<0d><0a>
Connection: Keep-Alive<0d><0a>
Accept-Encoding: gzip<0d><0a>
Content-Length: 0<0d><0a>
<0d><0a>

*/
int ncSrvAppCheck(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{

}





