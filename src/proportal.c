/* Portal服务器 ----- Proportal
   
         最后修改时间:   2013/10/26
*/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include "utoall.h"
#include "pasutl.h"
#include "ncportal.h"      
#include "ncportalweb.h"

int ncSrvProcessInit(utShmHead *psShmHead);
//int Ncm_User_SetFunName(utShmHead *psShmHead);
int main(int argc,char **argv)
{
    utShmHead  *psShmHead;
    char caConfFile[64];
    char caVer[32],caRelease[32];
    unsigned char *pLogFile;
    int  iReturn,iLogLevel;
    ncSrvMyVersion(caVer,caRelease);
    printf(" proportal %s  Release %s\n",caVer,caRelease);
    strcpy(caConfFile,"ncsrv.cnf");
    srand(time(0));
    pasKillProcess(NULL,"proportal");   /* 自动停掉原来已经运行的进程  */
    psShmHead = (utShmHead *)pasServerInit(caConfFile);
    if(psShmHead == NULL) {
        printf(" proportal  start error ..... \n");
        exit(9);
    }
    pLogFile    = utComGetVar_sd(psShmHead,"LogFile",  "../log/proportal.log");
    iLogLevel   = utComGetVar_ld(psShmHead,"LogLevel",PAS_LOG_EVENT);
    ncLangInit(psShmHead);
    pasSetLogPath("../log");
    pasSetLogFile(pLogFile);
    pasSetDebugInfoFromCnf(psShmHead);
    pasLog(PAS_LOG_EVENT,"Start Proportal ");
    iReturn = ncSrvServerInit(psShmHead);
    if(iReturn < 0){
        printf(" ncSrvServerInit Error, iReturn=%d \n",iReturn);
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
    dsCltSetSessionType("id",       UT_TYPE_LONG,4,1);  
    dsCltSetSessionType("cid",      UT_TYPE_LONG,4,1);  
    dsCltSetSessionType("opt",      UT_TYPE_STRING,255,1);  
    dsCltSetSessionType("stype",      UT_TYPE_STRING,255,1);    
    iReturn = dsCltInit(psShmHead);
    if(iReturn < 0){
        printf(" Start Dsclt Error iReturn=%d \n",iReturn);
        pasStopProcess(psShmHead);
        utShmRemove(psShmHead);
        exit(9);
    } 
    // iReturn = ncSrvUdpServer00(psShmHead);
    iReturn = pasTcpServer(psShmHead,ncSrvProcessInit);    /* Tcp处理进程   */
    if(iReturn < 0) {
    	printf(" Start Tcp Server Error iReturn=%d \n",iReturn);
        pasStopProcess(psShmHead);
        utShmRemove(psShmHead);
        exit(9);
    }
    
    iReturn = pasConnect(psShmHead);
    if(iReturn != 0) {
        printf("Connect Database Error Sqlcode is %d %s",iReturn,pasDbErrorMsg(NULL));
        pasLog(PAS_LOG_ERROR,"Connect Database Error Sqlcode is %d %s",iReturn,pasDbErrorMsg(NULL));
        utShmRemove(psShmHead);
        exit(9);
    }
    ncmInitWeiXinMem(psShmHead);
	ictInitShm(psShmHead);
//    ncm_tbuserimpS(psShmHead);
    pasDbClose(NULL);
    pasOnTimeDo(psShmHead);     
    utShmClose(psShmHead);
    exit(0);
}

int ncSrvProcessInit(utShmHead *psShmHead)
{
    int iReturn;
    iReturn = pasConnect(psShmHead);
    if(iReturn != 0) {
        pasLog(PAS_LOG_ERROR,"Connect Database Error Sqlcode is %d %s",iReturn,pasDbErrorMsg(NULL));
        utShmRemove(psShmHead);
        exit(9);
    }
    return 0;
}



int ncSrvServerInit(utShmHead *psShmHead)
{
    int iReturn;
    pasUtlInitProcess(psShmHead,NCSRV_LNK_PROCESS,100);
    iReturn = ncSrvSystemInit(psShmHead);
    if(iReturn < 0) {
        pasLog(PAS_LOG_ERROR,"ncSrvServerInit Error");
        return (-1);
    }
    if(strcasecmp(utComGetVar_sd(psShmHead,"UseWeiXin","Yes"),"Yes")==0) {
        ncmInitWXAccessToken(psShmHead);
    }
    ncSrvSetTcpFun(psShmHead);
    ncmInitWebFun(psShmHead);
  //  Ncm_User_SetFunName(psShmHead);
    ncSrvSetProcess(psShmHead);
    iReturn = pasUtlStartProcess(psShmHead);
    
    return 0;
}
