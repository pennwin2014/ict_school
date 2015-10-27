
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utoall.h"
#include "pasutl.h"
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
//#include "ncdef.h"
#include "pasdb.h"
#include    <signal.h>
#include    <sys/wait.h>
#include <netinet/in.h>
#define   NC_SHM_MAXLNK     100
#define NC_LNK_PROCESS      47  /* 进程管理           */
typedef struct ncProcessInfo_s {
    int           iPid;                /* 进程ID                                        */
    unsigned long lStartTime;          /* 启动时间                                      */
    unsigned long lTimeOut;            /* 超时时间                                      */
    unsigned long lOntime;             /* 在每天指定时间重新启动                        */
    unsigned short nPrio;              /* 优先级                                        */
    unsigned short nNum;               /* 启动顺序号                                    */
    unsigned char caControl[28];       /* 控制信息                                      */
    char     caName[32];               /* 功能名称                                      */
    int      (*fFunName)(utShmHead *); /* 函数名                                        */
    unsigned long lLastTime;           /* 进程最后操作时间                              */
    unsigned long lStepTime;           /* 最大间隔时间，通常超过该时间表明进程有问题    */
    int      iErrorNo;                 /* 进程启动出错次数                              */
    int      iFlags;                   /* 0--无  1--内部进程  2--外部   8--进程重启     */
    unsigned long lRunSec;             /* CPU时间 秒                                    */
    unsigned long lRunUsec;            /* CPU时间 豪秒                                  */
    unsigned long lSysSec;             /* 系统时间                                      */
    unsigned long lSysUsec;            /* 占用内存                                      */
    unsigned long lGetTime;            /* 取样时间                                      */
} ncProcessInfo;

/* 进程管理中的第一条记录   */
typedef struct ncProcessHead_s {
    int             iPid;                      /* 本进程的进程号    */
    unsigned long   lStartTime;                /* 启动时间          */
    int             iDo;                       /* 处理标识 0--正常  */
    int             iFlags;                    /* iFlags == 0-清掉在线信息 1--保留在线信息 2---直接恢复 */
    unsigned long   lLastTime;                 /* 进程最后操作时间  */
    
} ncProcessHead;
/* 进程信息     */   
#define NC_PROC_END         2     /* 进程被强迫终止   */
#define NC_PROC_RESET       8     /* 进程需要重启  */
#define NC_PROC_STOP        9     /* 进程需要停止  */

void SysCrmlic(utShmHead *psShmHead);


unsigned long  ncUtlGetThatDays(unsigned long lTime)
{
    struct tm  *tm;
    unsigned long lThisDay;
    tm = localtime(&lTime);
    lThisDay = (1900 + tm->tm_year) * 10000 + (tm->tm_mon + 1) * 100 + tm->tm_mday;
    return lThisDay;
}
int ncUtlStartProcess(utShmHead *psShmHead)
{
    ncProcessHead *psFunHead;
    struct tm    *tm;
    unsigned long lThisTime,lThisDayTime,lThisDay,lStepTime;
   	unsigned long lLastLicenseTime=0;
    int iPid,iPid1,iRet,iFlags;
    int iReturn = 0;
    int iError = 0;
    int iSum,i,j,iCount,iStatus,k=0;
    ncProcessInfo *psFun;
    unsigned long lTime=0;
    iPid = fork();
            iPid1 = waitpid(-1,&iStatus,WNOHANG);
         
    if(iPid < 0) {
  //      pasLog1(PAS_LOG_ERROR," Start Main Process  Error");
        return (-1);
    }
    else if(iPid > 0) {
        return iPid;
    }
    lTime = time(0);
    
    while(1) {
        psFun = (ncProcessInfo *)utShmArray(psShmHead,NC_LNK_PROCESS);
        if(psFun == NULL) {
  //          pasLog1(PAS_LOG_EVENT,"[nc]Process Memory not init ");
            return (-1);
        }
        psFun[0].iPid = getpid();
        psFunHead = (ncProcessHead *)psFun;        
        iSum = utShmArrayRecord(psShmHead,NC_LNK_PROCESS);
        if(psFunHead->iDo == NC_PROC_RESET) {
        	iRet = 0;
            printf("Start ncUtlStartProcess --------- Pid:%d psFunHead->iDo=%d \n",
               getpid(),psFunHead->iDo);
             
            psFunHead->iDo = 0;
        	
            for(i=1;i<iSum;i++) {
                if(psFun[i].iFlags == 1 && psFun[i].iPid > 0) {
                    kill(psFun[i].iPid,9);
                    psFun[i].iPid = 0;
                }
            }
            sleep(1);
            iPid1 = 1;
            while(iPid1 > 0) {
                iPid1 = waitpid(-1,&iStatus,WNOHANG);
                if(iPid1 > 0) {
                    iStatus = WEXITSTATUS(iStatus);
                 
                }
            }
            /* 初始化   */
          	iReturn = pasConnect(psShmHead); /* 连接数据库  */
            if(iReturn != 0) {
               
                return (-1);
            }
                    
            pasDbClose(NULL);
            
            ncStartSetAllProcess(psShmHead);  
            psFun = (ncProcessInfo *)utShmArray(psShmHead,NC_LNK_PROCESS);
            if(psFun == NULL) {
                pasLog1(PAS_LOG_EVENT,"[nc]Process Memory not init ");
                return (-1);
            }
            psFunHead = (ncProcessHead *)psFun;        
            iSum = utShmArrayRecord(psShmHead,NC_LNK_PROCESS);
            for(i=1;i<iSum;i++) {
                if(psFun[i].iFlags == 1 || psFun[i].iFlags == 2) {
                	ncProcStartThisProcess(psShmHead,&psFun[i]);
                }
            }
          
            psFunHead->iFlags = 0;
            psFunHead->lLastTime = time(0);
        }
        else if(psFunHead->iDo == NC_PROC_STOP) { /* 停止所有进程  */
            pasLog1(PAS_LOG_EVENT,"Stop All Process\n");
            psFunHead->iDo = 0;
          
            for(i=1;i<iSum;i++) {
                if(psFun[i].iFlags == 1 && psFun[i].iPid > 0) {
                    kill(psFun[i].iPid,9);
                    psFun[i].iPid = 0;
                    psFun[i].iFlags == 2;
                }
            }
            iPid1 = 1;
            while(iPid1 > 0) {
                iPid1 = waitpid(-1,&iStatus,WNOHANG);
                if(iPid1 > 0) {
                    iStatus = WEXITSTATUS(iStatus);
                    pasLog3(PAS_LOG_EVENT,"[nc]Pid %d Exit Status is %d\n",iPid1,iStatus);
                }
            }
            psFunHead->iFlags = 1;
            psFunHead->lLastTime = time(0);
        }
        else  {
        	lThisTime = time(0);
        	tm = localtime(&lThisTime);
        	lThisDayTime = tm->tm_hour * 10000L + tm->tm_min * 100 + tm->tm_sec;
       	lThisDay = (1900 + tm->tm_year) * 10000 + (tm->tm_mon + 1) * 100 + tm->tm_mday;
      	
            for(i=1;i<iSum;i++) {
// printf("name=%s\n",psFun[i].caName);
                if(psFun[i].iFlags == NC_PROC_RESET) {  /* 重置当前进程   */
                    if(psFun[i].iPid > 0)  {
                        kill(psFun[i].iPid,9);
                        psFun[i].iPid = 0;
                    }
                    pasLog2(PAS_LOG_EVENT,"NC_PROC_RESET,Start %s  ",psFun[i].caName);
                    ncProcStartThisProcess(psShmHead,&psFun[i]);
                }
                else if(psFun[i].iFlags == NC_PROC_STOP) {  /* 重置当前进程   */
                    if(psFun[i].iPid > 0)  {
                        kill(psFun[i].iPid,9);
                        psFun[i].iPid = 0;
                        psFun[i].iFlags = 2;
                        
                    }
                    pasLog2(PAS_LOG_EVENT,"NC_PROC_STOP,Stop %s  ",psFun[i].caName);
                }
                else if(psFun[i].iFlags == 1 && psFun[i].iPid == 0) {
                    pasLog2(PAS_LOG_EVENT,"NC_PROC_START,Start %s  ",psFun[i].caName);
                    ncProcStartThisProcess(psShmHead,&psFun[i]);
                }
                else { /* 检查进程状态/超时等   */
                	if(psFun[i].lOntime > 0) {    /* 定时  */
                		unsigned long lThisStartTime=ncUtlGetThatDays(psFun[i].lStartTime);
                    	if(lThisStartTime  <  ncUtlGetThatDays(lThisTime) &&
                        		lThisDayTime >= psFun[i].lOntime && psFun[i].lStartTime > 0)  {
                            pasLog2(PAS_LOG_EVENT,"Process %s is Ontime, Reset by system ",psFun[i].caName);
                            if(psFun[i].iPid > 0) kill(psFun[i].iPid,9);
                            psFun[i].lStartTime = lThisTime;
                    	}
                	}
                	else {
                    	if( lThisTime - psFun[i].lStartTime > psFun[i].lTimeOut 
                    		           && psFun[i].lTimeOut > 0 && psFun[i].lStartTime > 0) {
                        	pasLog2(PAS_LOG_EVENT,"Process %s  is Time out, Reset by system ",psFun[i].caName);
	                        if(psFun[i].iPid > 0) kill(psFun[i].iPid,9);
    	                    psFun[i].lStartTime = lThisTime;
        	            }
            	    }
                }
            }
            psFunHead->lLastTime = time(0);
        }
        iPid1 = 1;
        while(iPid1 > 0) {
            iPid1 = waitpid(-1,&iStatus,WNOHANG);
//            printf("iPid1=%d\n",iPid1);
            if(iPid1 > 0) {
                if(WIFEXITED(iStatus)) {
                    iStatus = WEXITSTATUS(iStatus);
                    pasLog3(PAS_LOG_EVENT,"[nc]Pid %d Exit Status is %d\n",iPid1,iStatus);
                }
                else if(WIFSIGNALED(iStatus)) { /* 异常退出  */
                    iStatus = WTERMSIG(iStatus);
                    pasLog3(PAS_LOG_ERROR,"[nc]Pid %d Exit by term signal %d\n",iPid1,iStatus);
                    iError ++;
                    if(time(0) - lTime > 600) {
                        lTime = time(0);
                        iError = 0;
                    }
                   
                }
                else if(WIFSTOPPED(iStatus)) {
                    iStatus = WSTOPSIG(iStatus);
                    pasLog3(PAS_LOG_ERROR,"[nc]Pid %d Exit by stop signal %d\n",iPid1,iStatus);
                }
                else {
                    pasLog3(PAS_LOG_ERROR,"[nc]Pid %d Exit by abnormally %d\n",iPid1,iStatus);
                }
                if(iStatus != SIGUSR1 && iStatus != SIGALRM) { /* 出错  */
                    for(i=1;i<iSum;i++) {
                        if(psFun[i].iPid == iPid1) {
                            pasLog3(PAS_LOG_ERROR,"Stop Process %s  Pid==%lu \n",psFun[i].caName,psFun[i].iPid);
                            psFun[i].iPid = 0;
                            k = i;
                            if(iPid1 > 0) kill(iPid1,9);
                        }
                    }
                }
            }
          
        }
        sleep(5);
    }
}

/* 启动当前进程  */
int ncProcStartThisProcess(utShmHead *psShmHead,ncProcessInfo *psFun)
{    
    int iPid2;
    int iReturn;
    iPid2 = fork();
    if(iPid2 == 0) {
    //    pasLog3(PAS_LOG_EVENT,"[nc]Start Process %s  Pid==%lu ",psFun->caName,getpid());
    	printf(" Start Process [%s]  Pid==%lu \n",psFun->caName,getpid());
        if(strcasecmp(utComGetVar_sd(psShmHead,"ProcessDebug","No"),"Yes") == 0) {
            printf(" Waiting %s 20s  Pid is %d \n",psFun->caName,getpid());
            sleep(20);
        }
        if(psFun->nNum > 0) {
        	sleep(psFun->nNum);
        }
        iReturn = psFun->fFunName(psShmHead);
        if(iReturn < 0) {
            printf(" Start Process [%s] Error \n",psFun->caName);
       //     pasLog(PAS_LOG_EVENT,"[nc]Start Process %s  Error %d ",psFun->caName,iReturn);
        }
        exit(0);
    }
    else if(iPid2 > 0) {
        psFun->iPid = iPid2;
        psFun->iFlags = 1;
        psFun->lStartTime = time(0);
        return iPid2;
    }
    else {
    	return (-1);
    }
}


int ncStartSomeProcess(utShmHead *psShmHead)
{
    int iCtlPid,iPid1,iPid,iStatus;
    ncProcessHead *psFunHead;
    unsigned long lTime,lStartTime;
    ncStartSetAllProcess(psShmHead);
//  ncUtlStartProcess(psShmHead);
    lStartTime = time(0);    
 //   printf("StartSome Process .....\n");
    iPid = fork();
    if(iPid == 0) {
        iCtlPid = ncUtlStartProcess(psShmHead);
      
        while(1) {
            lTime = time(0);
            if(iCtlPid > 0) {
                iPid1 = 1;
                while(iPid1 > 0) {
                    iPid1 = waitpid(-1,&iStatus,WNOHANG);
                    if(iPid1 == iCtlPid) {
                        iStatus = WEXITSTATUS(iStatus);
                        pasLog(PAS_LOG_EVENT,"[nc]Pid %d Exit Status is %d\n",iPid1,iStatus);
                        printf(" Pid1 %d Exit Status is %d  MyPid=%d\n",iPid1,iStatus,getpid());
                        if(iPid1 > 0) kill(iPid1,9);
                        ncUtlStopProcess(psShmHead);
                        iCtlPid = ncUtlStartProcess(psShmHead);
                    }
                }
                // 检查进程
                psFunHead = (ncProcessHead *)utShmArray(psShmHead,NC_LNK_PROCESS);
                if(psFunHead) {
                    if(lTime > psFunHead->lLastTime + 300 && lTime  > 30 + lStartTime) {
                        pasLog(PAS_LOG_ERROR," Main Process TimeOut, Restart Process  %lu %lu %lu",lTime,psFunHead->lLastTime,lTime - psFunHead->lLastTime);
                        if(iCtlPid > 0) kill(iCtlPid,9);
                        ncUtlStopProcess(psShmHead);
                        iCtlPid = ncUtlStartProcess(psShmHead);
                    }
                }
                sleep(3);
            }
            else {
                sleep(10);
                iCtlPid = ncUtlStartProcess(psShmHead);
            }
        }
        exit(0);
    }
    else {
        return iPid;
    }
    
}

/* 检查进程是否需要启动   */
int ncUtlProcessIsSet(utShmHead *psShmHead,char *pName,char *pDef)
{
    char caName[48];
    sprintf(caName,"Start%s",pName);
    if(strcasecmp(utComGetVar_sd(psShmHead,caName,pDef),"Yes")==0) {
        return 1;
    }
    else {
        return 0;
    }
}
int ncUtlInitProcess(utShmHead *psShmHead,int iMaxFun)
{
    ncProcessInfo *psFun;
    ncProcessHead *psFunHead;
    int iReturn;
    utShmFreeArray(psShmHead,NC_LNK_PROCESS);
    iReturn = utShmInitArray(psShmHead,NC_LNK_PROCESS,sizeof(ncProcessInfo),iMaxFun);
    if(iReturn < 0) {
        return (-1);
    }
    psFun = (ncProcessInfo *)utShmArray(psShmHead,NC_LNK_PROCESS);
    if(psFun == NULL) {
        return (-1);
    }
    memset(psFun,0,iMaxFun * sizeof(ncProcessInfo));
    psFun[0].iFlags = 1;
	strncpy(psFun[0].caName,"Manager",31);    
    psFunHead = (ncProcessHead *)psFun;
    psFunHead->iPid = getpid();
    psFunHead->lStartTime = time(0);
    psFunHead->iDo = 0;
    return 0;
}
int ncUtlSetProcessName(utShmHead *psShmHead,
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
    
    
    psFun = (ncProcessInfo *)utShmArray(psShmHead,NC_LNK_PROCESS);
    if(psFun == NULL) {
   //     ncSysLog(NC_LOG_ERROR," Memory not init ");
        return (-1);
    }
    iSum = utShmArrayRecord(psShmHead,NC_LNK_PROCESS);
    for(i=1;i<iSum;i++) {
        if( strcasecmp(psFun[i].caName,pName) == 0) {
            if(psFun[i].iPid > 0) {
                kill(psFun[i].iPid,9);
            }
            psFun[i].iFlags = 1;
            psFun[i].lOntime = lOnTime;
            psFun[i].fFunName = fFunName;
            strncpy(psFun[i].caName,pName,31);
            psFun[i].nPrio = 0;
            psFun[i].nNum =  1;
            psFun[i].lTimeOut = lStepTime;
            return i;
        }
    }
    for(i=1;i<iSum;i++) {
        if(psFun[i].iFlags == 0 && utStrIsSpaces(psFun[i].caName)) {
            if(psFun[i].iPid > 0) {
                kill(psFun[i].iPid,9);
            }
            psFun[i].iFlags = 1;
            psFun[i].lOntime = lOnTime;
            psFun[i].fFunName = fFunName;
            strncpy(psFun[i].caName,pName,31);
            psFun[i].lTimeOut = lStepTime;
            psFun[i].nPrio = 0;
            psFun[i].nNum =  1;
            return i;
        }
    }
    return (-1);
}

/* 停止一个进程  */
int ncUtlStopOneProcess(utShmHead *psShmHead,char *pName)
{
    int iSum,i;
    ncProcessInfo *psFun;
    psFun = (ncProcessInfo *)utShmArray(psShmHead,NC_LNK_PROCESS);
    if(psFun == NULL) {
        pasLog1(PAS_LOG_EVENT," NC_LNK_PROCESS not define \n");
        return (-1);
    }
    iSum = utShmArrayRecord(psShmHead,NC_LNK_PROCESS);
    for(i=1;i<iSum;i++) {
        if(strncasecmp(psFun[i].caName,pName,strlen(pName))==0) {
            if(psFun[i].iPid > 0) {
                psFun[i].iFlags = NC_PROC_STOP;
            }
        }
    }            
    return 0;
}
int ncUtlStartOneProcess(utShmHead *psShmHead,char *pName)
{
    int iSum,i;
    ncProcessInfo *psFun;
    psFun = (ncProcessInfo *)utShmArray(psShmHead,NC_LNK_PROCESS);
    if(psFun == NULL) {
        printf(" Process Error \n");
        return (-1);
    }
    iSum = utShmArrayRecord(psShmHead,NC_LNK_PROCESS);
    for(i=1;i<iSum;i++) {
        if(strncasecmp(psFun[i].caName,pName,strlen(pName))==0) {
            psFun[i].iFlags = NC_PROC_RESET;
        }
    }            
    return 0;
}
int ncUtlStopProcess(utShmHead *psShmHead)
{
    int iSum,i;
    ncProcessInfo *psFun;
    psFun = (ncProcessInfo *)utShmArray(psShmHead,NC_LNK_PROCESS);
    if(psFun == NULL) {
        printf(" Process Error \n");
        return (-1);
    }
    iSum = utShmArrayRecord(psShmHead,NC_LNK_PROCESS);
    for(i=1;i<iSum;i++) {
        if(psFun[i].iPid > 0) {
            kill(psFun[i].iPid,9);
            psFun[i].iPid = 0;
        }
    }    
    return 0;        
}
/* 重新启动各种进程  */
int ncUtlRestartProcess(utShmHead *psShmHead,int iFlags)
{
    ncProcessHead *psFunHead = (ncProcessHead *)utShmArray(psShmHead,NC_LNK_PROCESS);;
    if(psFunHead == NULL) {
  //      pasLog1(PAS_LOG_ERROR,"[ncUtlRestartProcess]psFunHead=NULL");
        return (-1);
    }
    if(psFunHead->iDo == NC_PROC_RESET) {
        psFunHead->iFlags = iFlags;
  //      pasLog2(PAS_LOG_ERROR,"[ncUtlRestartProcess]Already iFlags=%d",iFlags);
    }
    else {
        long lTime;
        lTime = utComGetVar_ld(psShmHead,"ResetTimeOut",1);
  //      pasLog2(PAS_LOG_EVENT,"[ncUtlRestartProcess]iFlags=%d",iFlags);
        psFunHead->iDo = NC_PROC_RESET;
        psFunHead->iFlags = iFlags;
        if(lTime > 0) {
            sleep(lTime);   /* 延迟5秒  */
        }
    }
    return 0;
}

/* 停止各种进程的运行   */
int ncUtlStopAllNcProcess(utShmHead *psShmHead,int iFlags)
{
    ncProcessHead *psFunHead = (ncProcessHead *)utShmArray(psShmHead,NC_LNK_PROCESS);
    if(psFunHead == NULL) {
        return (-1);
    }
    psFunHead->iDo = NC_PROC_STOP;
    psFunHead->iFlags = iFlags;
    return 0;
}


void ncSignal_fatal(int sig)
{
    printf("exit on signal (%d)\n", sig);
    exit(0);
}

void ncSignal_hup(int sig)
{
    return;
}

int ncStartSetAllProcess(utShmHead *psShmHead)
{
    int iReturn,iLicense;

    signal(SIGHUP, ncSignal_hup);
    signal(SIGINT, ncSignal_fatal);
    signal(SIGQUIT,ncSignal_fatal);
    signal(SIGILL, ncSignal_fatal);
    signal(SIGTRAP,ncSignal_fatal);
    signal(SIGIOT, ncSignal_fatal);
    signal(SIGFPE, ncSignal_fatal);
    signal(SIGTERM,ncSignal_fatal);
    signal(SIGPIPE,ncSignal_hup);
    signal(SIGUSR1,ncSignal_hup);
    signal(SIGUSR2,ncSignal_hup);
    
    ncUtlInitProcess(psShmHead,10);
    /* 导入日志  */
 //     ncUtlSetDoProcess(psShmHead,"LoadDb",0,5,LoadFileToDb,"Yes",71000L,0); 
 
 /*连接嘉讯短信服务器数据库*/     
  //     ncUtlSetDoProcess(psShmHead,"SendJxSms",0,5,SendJxSms,"Yes",71000L,0);    
         
 //   ncUtlSetDoProcess(psShmHead,"syscrmlic",0,15, SysCrmlic,"Yes",68400L,0);
 //   ncUtlSetDoProcess(psShmHead,"SynPortal",0,15, ncSynUser2Portal,"No",71000L,0);


    return 0;
}

/* 设置进程启动状态
   替代  ncUtlSetProcessName
 */
int ncUtlSetDoProcess(utShmHead *psShmHead,
                          char *pName,                          // 进程名
                          int  iPrio,                           // 进程执行优先级
                          int  iNum,                            // 进程执行顺序号
                          int (*fFunName)(utShmHead *),         // 执行函数
                          char *pDef,                           // 缺省是否启动
                          unsigned long lStepTime,              // 重新启动时间
                          unsigned long lOnTime)                // 固定启动时间
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
    lStepTime = utComGetVar_ld(psShmHead,caOn,lStepTime);
    sprintf(caOn,"OnSet%s",pName);
    lOnTime = utComGetVar_ld(psShmHead,caOn,lOnTime);
    sprintf(caOn,"Prio%s",pName);
    iPrio = utComGetVar_ld(psShmHead,caOn,iPrio);
    
    psFun = (ncProcessInfo *)utShmArray(psShmHead,NC_LNK_PROCESS);
    if(psFun == NULL) {

        pasLog(PAS_LOG_ERROR," Memory not init ");
        return (-1);
    }
    iSum = utShmArrayRecord(psShmHead,NC_LNK_PROCESS);

    for(i=1;i<iSum;i++) {
        if( strcasecmp(psFun[i].caName,pName) == 0) {
            if(psFun[i].iPid > 0) {
                kill(psFun[i].iPid,9);
            }
            psFun[i].iFlags = 1;
            psFun[i].lOntime = lOnTime;
            psFun[i].fFunName = fFunName;
            strncpy(psFun[i].caName,pName,31);
            psFun[i].lTimeOut = lStepTime;
            psFun[i].nPrio = iPrio;
            psFun[i].nNum =  iNum;
            return i;
        }
    }
    for(i=0;i<iSum;i++) {
        if(psFun[i].iFlags == 0 ) {
            if(psFun[i].iPid > 0) {
                kill(psFun[i].iPid,9);
            }
            psFun[i].iFlags = 1;
            psFun[i].lOntime = lOnTime;
            psFun[i].fFunName = fFunName;
            strncpy(psFun[i].caName,pName,31);
            psFun[i].lTimeOut = lStepTime;
            psFun[i].nPrio = iPrio;
            psFun[i].nNum =  iNum;
            return i;
        }
    }
    return (-1);
}



/* 启动各种进程  */

int StartSomeProcess(utShmHead *psShmHead){
   int iCtlPid,iPid1,iPid,iStatus;
    ncProcessHead *psFunHead;
    unsigned long lTime,lStartTime;
    ncStartSetAllProcess(psShmHead);
//  ncUtlStartProcess(psShmHead);
    lStartTime = time(0);    
	
   //     pasUtlInitProcess(psShmHead,NC_LNK_PROCESS,30);
  //       ncUtlSetDoProcess(psShmHead,"Onlineuser",0,15, SendOnlineuser,"Yes",68400L,0);



    lStartTime = time(0);    
 //   printf("StartSome Process .....\n");
    iPid = fork();
    if(iPid == 0) {
        iCtlPid = ncUtlStartProcess(psShmHead);
        while(1) {
            lTime = time(0);
            if(iCtlPid > 0) {
                iPid1 = 1;
                while(iPid1 > 0) {
                    iPid1 = waitpid(-1,&iStatus,WNOHANG);
                    if(iPid1 == iCtlPid) {
                        iStatus = WEXITSTATUS(iStatus);
                        pasLog(PAS_LOG_EVENT,"[nc]Pid %d Exit Status is %d\n",iPid1,iStatus);
                        printf(" Pid1 %d Exit Status is %d  MyPid=%d\n",iPid1,iStatus,getpid());
                        if(iPid1 > 0) kill(iPid1,9);
                        ncUtlStopProcess(psShmHead);
                        iCtlPid = ncUtlStartProcess(psShmHead);
                    }
                }
                // 检查进程
                psFunHead = (ncProcessHead *)utShmArray(psShmHead,NC_LNK_PROCESS);
                if(psFunHead) {
                    if(lTime > psFunHead->lLastTime + 300 && lTime  > 30 + lStartTime) {
                        pasLog(PAS_LOG_ERROR," Main Process TimeOut, Restart Process  %lu %lu %lu",lTime,psFunHead->lLastTime,lTime - psFunHead->lLastTime);
                        if(iCtlPid > 0) kill(iCtlPid,9);
                        ncUtlStopProcess(psShmHead);
                        iCtlPid = ncUtlStartProcess(psShmHead);
                    }
                }
                sleep(3);
            }
            else {
                sleep(10);
                iCtlPid = ncUtlStartProcess(psShmHead);
            }
        }
        exit(0);
    }
    else {
        return iPid;
    }


}