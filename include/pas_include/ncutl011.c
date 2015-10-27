#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utoshm01.h"
#include "ncuser.h"
#include "ncdef.h"

/* �����û�  */
/* �����ݿ��м����û���Ϣ
      iFlags --- 2 ����IPѰַ   3 ����MACѰַ

 */
int ncUtlAdduser(utShmHead *psShmHead,ncUserInfo *psUserInfo,int iFlags)
{
    long lUserid;
    int iReturn;
    int lCount=0;
    char caDate[32];
    unsigned long lTime;
    lTime = time(0);
    strcpy(caDate,utTimFormat("%Y-%m-%d %H:%M:%S",lTime));
    lCount = 0;
    iReturn = 0;

    /* ����û��Ƿ����  */
    psHead = (ncUserContHead *)utShmArray(psShmHead,NC_LNK_ALLUSER);
    
    iReturn = pasDbOneRecord("select count(*) from ncuser where username = :name",1,
                                "name",UT_TYPE_STRING,psUserInfo->caName,
                                UT_TYPE_LONG,4,&lCount);
    if(iReturn == 0 && lCount > 0) {
        sprintf(psUserInfo->caName,"K%lu",ncUtlGenId(psShmHead));
    }
    if(iFlags == NC_NETWORK_IPBASE) {  /* ����IP��ַѰַ  */
         iReturn = pasDbOneRecord("select count(*) from ncuser where ip = :ip",1,
                                "ip",UT_TYPE_STRING,utComHostIp(htonl(psUserInfo->lIp)),
                                UT_TYPE_LONG,4,&lCount);
         if(iReturn == 0 && lCount == 0) {
            if(iFlagsCheckMac) { /* ���MAC��ַ  */
                iReturn = pasDbOneRecord("select count(*) from ncuser where mac = :mac",1,
                                "mac",UT_TYPE_STRING,ncCvtMac(psUserInfo->mac),
                                UT_TYPE_LONG,4,&lCount);
                 
            }
         }
    }
    else if(iFlags == NC_NETWORK_MACBASE) {
         iReturn = pasDbOneRecord("select count(*) from ncuser where mac = :mac",1,
                                "mac",UT_TYPE_STRING,ncCvtMac(psUserInfo->mac),
                                UT_TYPE_LONG,4,&lCount);
    }
    if(iReturn == 0) {
        if(lCount == 0) {
            unsigned long lTimeLimit;
            long8 lFlowLimit;
            lTimeLimit = utComGetVar_ld(psShmHead,"TimeCtlDefault",1440) * 60;
            lFlowLimit = utComGetVar_ld(psShmHead,"FlowCtlDefault",1000) * 1024;
            psUserInfo->lLimitTimes = lTimeLimit;
            psUserInfo->lLimitBytes = lFlowLimit;
            iReturn = pasDbExecSqlF("insert into ncuser (userid,username,password,groupid,dispname, \
                      email,ip,mac,addtime,lasttime,useflags,timelimit,flowlimit) \
                      values (%lu,'%s','***','%lu','%s','%s','%s','%s','%s','%s',0,%lu,%lu)",
                           psUserInfo->lId,psUserInfo->caName,
                           psUserInfo->lGid,
                           psUserInfo->caName,"\0",
                           utComHostIp(htonl(psUserInfo->lIp)),
                           ncCvtMac(psUserInfo->mac),
                           caDate,caDate,
                           lTimeLimit,lFlowLimit);
            if(iReturn == 0) {
                pasDbCommit(NULL);
                return 0;
            }
            else {
                char caMsg[255];
                sprintf(caMsg,"insert into ncuser (userid,username,groupid,dispname, \
                      email,ip,mac,addtime,lasttime,useflags) \
                      values (%lu,'%s','%lu','%s','%s','%s','%s','%s','%s',0)",
                           lUserid,psUserInfo->caName,
                           psUserInfo->lGid,
                           psUserInfo->caName,"\0",
                           utComHostIp(htonl(psUserInfo->lIp)),
                           ncCvtMac(psUserInfo->mac),caDate,caDate);
                pasDbRollback(NULL);
                utSysLog(" Add UserError %d \n",iReturn);
                return (-1);
            }
        }
        else {
            utSysLog(" User Mac Already Exist %s \n",ncCvtMac(psUserInfo->mac));
            return (-2);
        }
    }
    else {
        return (-1);
    }
}
 