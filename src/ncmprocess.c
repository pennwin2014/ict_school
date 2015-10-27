#define ncmax(a,b) ((a) > (b) ? (a) : (b))
#define ncMin(a,b) ((a) < (b) ? (a) : (b))
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <dirent.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/resource.h>
#include "utoall.h"
#include "pasutl.h"
#include "pasdb.h"
#include "pasutl.h"
#include "ncportal.h"



//void ncSignal_fatal(int sig);
//void ncSignal_hup(int sig);
//void ncReload_sig(int sig);

typedef struct ncProcessInfo_s {
    int           iPid;                /* ����ID                                        */
    unsigned long lStartTime;          /* ����ʱ��                                      */
    unsigned long lTimeOut;            /* ��ʱʱ��                                      */
    unsigned long lOntime;             /* ��ÿ��ָ��ʱ����������                        */
    unsigned short nPrio;              /* ���ȼ�                                        */
    unsigned short nNum;               /* ����˳���                                    */
    unsigned char caControl[28];       /* ������Ϣ                                      */
    char     caName[32];               /* ��������                                      */
    int      (*fFunName)(utShmHead *); /* ������                                        */
    unsigned long lLastTime;           /* ����������ʱ��                              */
    unsigned long lStepTime;           /* �����ʱ�䣬ͨ��������ʱ���������������    */
    int      iErrorNo;                 /* ���������������                              */
    int      iFlags;                   /* 0--��  1--�ڲ�����  2--�ⲿ   8--��������     */
    unsigned long lRunSec;             /* CPUʱ�� ��                                    */
    unsigned long lRunUsec;            /* CPUʱ�� ����                                  */
    unsigned long lSysSec;             /* ϵͳʱ��                                      */
    unsigned long lSysUsec;            /* ռ���ڴ�                                      */
    unsigned long lGetTime;            /* ȡ��ʱ��                                      */
} ncProcessInfo;


int ncmcomSynPortalManInfo(utShmHead *psShmHead);

int pasUtlStartMyProcess(utShmHead *psShmHead)
{
    int iReturn,iLicense;
    char *p,*pName,*pName1,*pName2,*pName3;
	unsigned long lTime;
	char *pProdSn;
ncmUtlStartProcess(psShmHead);(psShmHead);
//    signal(SIGHUP, ncSignal_hup);
//    signal(SIGINT, ncSignal_fatal);
//    signal(SIGQUIT,ncSignal_fatal);
//    signal(SIGILL, ncSignal_fatal);
//    signal(SIGTRAP,ncSignal_fatal);
//    signal(SIGIOT, ncSignal_fatal);
//    signal(SIGFPE, ncSignal_fatal);
//    signal(SIGTERM,ncSignal_fatal);
//    signal(SIGPIPE,ncSignal_hup);
//    signal(SIGUSR1,ncSignal_hup);
//    signal(SIGUSR2,ncSignal_hup);
    
    ncmUtlInitProcess(psShmHead,80);
    
    ncmUtlSetProcessName(psShmHead,"Commucate Center",ncmcomSynPortalManInfo,"Yes",71000L,0);
//    ncmUtlSetProcessName(psShmHead,"ProcessMgr",ncmProcessCheck,"Yes",10000L,0);
    return 0;
}



/* ���̹������   */
int ncmUtlInitProcess(utShmHead *psShmHead,int iMaxFun)
{
    ncProcessInfo *psFun;
    int iReturn;
    utShmFreeArray(psShmHead,NCSRV_LNK_PROCESS);
    iReturn = utShmInitArray(psShmHead,NCSRV_LNK_PROCESS,sizeof(ncProcessInfo),iMaxFun);
    if(iReturn < 0) {
        return (-1);
    }
    psFun = (ncProcessInfo *)utShmArray(psShmHead,NCSRV_LNK_PROCESS);
    if(psFun == NULL) {
        return (-1);
    }
    memset(psFun,0,iMaxFun * sizeof(ncProcessInfo));
    return 0;
}


/* ���ý���   */
int ncmUtlSetProcessName(utShmHead *psShmHead,
              char *pName, int (*fFunName)(utShmHead *),char *pDef,unsigned long lStepTime,unsigned long lOnTime)
{
    int     iSum,i;
    ncProcessInfo *psFun;
    char            caOn[32];
    unsigned long   lTime;
    char *p;
    sprintf(caOn,"Start%s",pName);
    p = utComGetVar_sd(psShmHead,caOn,pDef);
    if(strcasecmp(p,"No")==0) {
        return 0;
    }
    sprintf(caOn,"Reset%s",pName);
    lTime = utComGetVar_ld(psShmHead,caOn,0);
    if(lTime > 0) {
        lStepTime = lTime;
    }
    sprintf(caOn,"OnSet%s",pName);
    lTime = utComGetVar_ld(psShmHead,caOn,0);
    if(lTime > 0) {
        lOnTime = lTime;
    }
    
    
    psFun = (ncProcessInfo *)utShmArray(psShmHead,NCSRV_LNK_PROCESS);
    if(psFun == NULL) {
//        ncSysLog(NC_LOG_ERROR," Memory not init ");
        return (-1);
    }
    iSum = utShmArrayRecord(psShmHead,NCSRV_LNK_PROCESS);
    for(i=0;i<iSum;i++) {
        if( strcasecmp(psFun[i].caName,pName) == 0) {
            if(psFun[i].iPid > 0) {
                kill(psFun[i].iPid,9);
            }
            psFun[i].iFlags = 1;
            psFun[i].lOntime = lOnTime;
            psFun[i].fFunName = fFunName;
            strncpy(psFun[i].caName,pName,31);
            psFun[i].lTimeOut = lStepTime;
            return i;
        }
    }
    for(i=0;i<iSum;i++) {
        if(psFun[i].iFlags == 0 && utStrIsSpaces(psFun[i].caName)) {
            if(psFun[i].iPid > 0) {
                kill(psFun[i].iPid,9);
            }
            psFun[i].iFlags = 1;
            psFun[i].lOntime = lOnTime;
            psFun[i].fFunName = fFunName;
            strncpy(psFun[i].caName,pName,31);
            psFun[i].lTimeOut = lStepTime;
            return i;
        }
    }
    return (-1);
}

/* ��������Ľ���  */
int ncmUtlStartProcess(psShmHead)
{
    int iPid,iPid1;
    int iReturn = 0;
    int iError = 0;
    int iSum,i,iStatus,k=0;
    ncProcessInfo *psFun;
    unsigned long lTime=0;
    iPid = fork();
    if(iPid < 0) {
	    ncUtlCheckLog(" Start Main Process  Error");
        return (-1);
    }
    else if(iPid > 0) {
        return iPid;
    }
    lTime = time(0);
    ncUtlCheckLog("[nc]Begin Start Main Process ");
    while(1) {
        psFun = (ncProcessInfo *)utShmArray(psShmHead,NCSRV_LNK_PROCESS);
        if(psFun == NULL) {
            ncUtlCheckLog("[nc]Process Memory not init ");
            return (-1);
        }
        iSum = utShmArrayRecord(psShmHead,NCSRV_LNK_PROCESS);
        for(i=0;i<iSum;i++) {
            if(psFun[0].iFlags >= 8) { /* ���н�����������   */
                psFun[0].iFlags -= 8;
                ncUtlCheckLog("[nc]Reset all process");
                pasUtlStartMyProcess(psShmHead);
                psFun = (ncProcessInfo *)utShmArray(psShmHead,NCSRV_LNK_PROCESS);
                if(psFun == NULL) {
                    ncUtlCheckLog("[nc]Process Memory not init ");
                    return (-1);
                }
                iSum = utShmArrayRecord(psShmHead,NCSRV_LNK_PROCESS);
                break;
            }
            if(psFun[i].iFlags == 1) {
                if(psFun[i].iPid == 0)  {
									int iPid2;
									iPid2 = fork();
									if(iPid2 == 0) {
										psFun[i].iPid = getpid();
				            psFun[i].lStartTime = time(0);
									  ncUtlCheckLog("[nc]Start Process %s  Pid==%lu ",psFun[i].caName,getpid());
										printf(" Start Process [%s]  Pid==%lu \n",psFun[i].caName,getpid());
										if(strcasecmp(utComGetVar_sd(psShmHead,"ProcessDebug","No"),"Yes") == 0) {
										    printf(" Waiting %s 20s  Pid is %d \n",psFun[i].caName,getpid());
										    sleep(20);
										}
										iReturn = psFun[i].fFunName(psShmHead);
										if(iReturn < 0) {
										    printf(" Start Process [%s] Error \n",psFun[i].caName);
				    					    ncUtlCheckLog("[nc]Start Process %s  ",psFun[i].caName);
				                        }
										exit(0);
									}
                }
            }
        }
        iPid1 = 1;
        while(iPid1 > 0) {
            iPid1 = waitpid(-1,&iStatus,WNOHANG);
            if(iPid1 > 0) {
                if(WIFEXITED(iStatus)) {
                    iStatus = WEXITSTATUS(iStatus);
                    ncUtlCheckLog("[nc]Pid %d Exit Status is %d\n",iPid1,iStatus);
                }
                else if(WIFSIGNALED(iStatus)) { /* �쳣�˳�  */
                    iStatus = WTERMSIG(iStatus);
                    ncUtlCheckLog("[nc]Pid %d Exit by term signal %d\n",iPid1,iStatus);
                    iError ++;
                    if(time(0) - lTime > 600) {
                        lTime = time(0);
                        iError = 0;
                    }
                    if(iError > 300) {
                        ncUtlCheckLog("[nc001_3]Error is too mach,system should restart");
 //                       ncSysRestartNc("Process Error, Rsetart Nc System");
                        iError = 0;
                    }
                }
                else if(WIFSTOPPED(iStatus)) {
                    iStatus = WSTOPSIG(iStatus);
                    ncUtlCheckLog("[nc]Pid %d Exit by stop signal %d\n",iPid1,iStatus);
                }
                else {
                    ncUtlCheckLog("[nc]Pid %d Exit by abnormally %d\n",iPid1,iStatus);
                }
                if(iStatus != SIGUSR1 && iStatus != SIGALRM) { /* ����  */
                    for(i=0;i<iSum;i++) {
                        if(psFun[i].iPid == iPid1) {
    	                    ncUtlCheckLog("Stop Process %s  Pid==%lu \n",psFun[i].caName,psFun[i].iPid);
                            psFun[i].iPid = 0;
                            k = i;
                            kill(iPid1,9);
                        }
                    }
                }
            }
        }
        sleep(5);
    }
}

