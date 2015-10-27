/* Radius服务器的处理
     工作在 Radius端，和Radius配合工作   
*/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include "utoall.h"
#include "pasutl.h"
#include "ncportal.h"
#define NCSRV_VERSION    "V1.00.01"
#define NCSRV_RELEASE    "2013/10/12"
int proRadProcessInit(utShmHead *psShmHead);
int proRadServerInit(utShmHead *psShmHead);
#define PAS_SRCFILE  8000

int main(int argc,char **argv)
{
    utShmHead  *psShmHead;
    char caConfFile[64];
    unsigned char *pLogFile;
    int  iReturn,iLogLevel;
    
    printf(" proeim_radius %s  Release %s\n",NCSRV_VERSION,NCSRV_RELEASE);
    strcpy(caConfFile,"proradius.cnf");
    psShmHead = (utShmHead *)pasServerInit(caConfFile);
    if(psShmHead == NULL) {
        printf(" ncsrv  start error ..... \n");
        exit(9);
    }
    pasKillProcess(NULL,"proradius");   /* 自动停掉原来已经运行的进程  */
    pLogFile    = utComGetVar_sd(psShmHead,"LogFile",  "../log/proradius.log");
    pasSetLogPath("../log");
    pasSetLogFile(pLogFile);
    pasSetDebugInfoFromCnf(psShmHead);
    pasLog(PAS_LOG_EVENT,"Start ncsrv ");
    iReturn = proRadServerInit(psShmHead);
    if(iReturn < 0){
        printf(" Start Tcp Server Error \n");
        pasStopProcess(psShmHead);
        utShmRemove(psShmHead);
        exit(9);
    }
    dsCltSetSessionType("email",    UT_TYPE_STRING,63,1);
    dsCltSetSessionType("groupname",UT_TYPE_STRING,31,1);
    dsCltSetSessionType("groupid",  UT_TYPE_LONG,4,1);
    dsCltSetSessionType("usrlevel", UT_TYPE_LONG,4,1);
    dsCltSetSessionType("plate",    UT_TYPE_STRING,63,0);
    dsCltSetSessionType("dispname", UT_TYPE_STRING,32 ,1);
    /*
    iReturn = dsCltInit(psShmHead);
    if(iReturn < 0){
        printf(" Start Dsclt Error ");
        pasStopProcess(psShmHead);
        utShmRemove(psShmHead);
        exit(9);
    } 
    */
    pasUtlInitProcess(psShmHead,59,100);    
    proRadSetProcess(psShmHead);   
    iReturn = pasTcpServer(psShmHead,proRadProcessInit);    /* Tcp处理进程   */
    if(iReturn < 0) {
        pasStopProcess(psShmHead);
        utShmRemove(psShmHead);
        exit(9);
    }
    pasUtlSetCheckLicense(0);
    pasOnTimeDo(psShmHead);  
    utShmClose(psShmHead);
    exit(0);
}



int proRadProcessInit(utShmHead *psShmHead)
{
    int iReturn;
    printf("Connect ... Pid=%d\n",getpid());
    iReturn = pasConnect(psShmHead);
    if(iReturn != 0) {
        printf("Connect Database Error Sqlcode is %d %s\n",iReturn,pasDbErrorMsg(NULL));
        pasLog(PAS_LOG_ERROR,"Connect Database Error Sqlcode is %d %s",iReturn,pasDbErrorMsg(NULL));
        utShmRemove(psShmHead);
        exit(9);
    }
    else {
        printf("Connect DataBase Sucess \n");
    }
    return 0;
}



int proRadServerInit(utShmHead *psShmHead)
{
    char *pPltPath;
    char caAdminIp[32],caAdminPort[16],caSmsIp[32];
    char caSmsType[16];
    char caAcConfig[128];
    char *p;
    uint4 lPort;
    unsigned long lMaxOnline;
    int iReturn,iRet;
    long lTimeOut,lMaxReSend;
    strcpy(caAdminIp,utComGetVar_sd(psShmHead,"AdminIp","192.168.20.1"));
    strcpy(caAdminPort,utComGetVar_sd(psShmHead,"AdminPort","80"));
    iReturn = proRadLoadConfig(psShmHead);
    if(iReturn != 0) {
        pasLog(PAS_LOG_ERROR," proRadLoadConfig Error");
        return (-1);
    }
    pPltPath = utComGetVar_s(psShmHead,"PlatePath");
    if(pPltPath) {
        utPltSetPlatePath(pPltPath);
    }
    proRadSetTcpFun(psShmHead);
    
    return 0;
}
