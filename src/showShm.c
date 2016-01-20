/*  显示当前在线用户
                    Writen by Liyunming  at 2000/10/3
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utoall.h"
#include "pasdef.h"
#include "ncportal.h"

static utShmHead  *psShmHead;

int ictShowPackage(utShmHead  *psShmHead)
{
	pasHashInfo sHashInfo;
	uchar *pHash = NULL;
	ictOrderNo* psOrder = NULL;
	pHash = (unsigned char *)utShmHashHead(psShmHead, ICT_ORDER_PACKAGE_ORDERNO);	
	psOrder = (ictPassUser*)pasHashFirst(pHash, &sHashInfo);
	printf("=======================查询==================\n");
    while(psOrder)
    {
		printf("orderno=[%s],autoxiding=[%s],effectType=[%d],packageid=[%d]\n",psOrder->orderno,psOrder->autoxiding, psOrder->effectType, psOrder->packageid);
		printf("paymentType=[%d],status=[%d]\n",psOrder->paymentType,psOrder->status);
		
        psOrder = (ictOrderNo *)pasHashNextS(&sHashInfo);
	
    }
	printf("=======================查询==================\n\n");
	return 0;
}


int main(int argc,char **argv)
{
    char argval;
    char caConfFile[64];
    char caName[32];
    char caOpt[32];
    int i,iReturn,iO=0,iU=0,iK=0,iT=0,iM=0,iB=0,iFlags=0,iOpt=0;
    char   *p;
    strcpy(caOpt,"\0");
    long lCount,lCount1,lStart,lNum;
    lCount =0 ;
    lCount1 = 0;
    lStart = 1;
    lNum = 999999L;
    printf(" Show User Information ........\n");
    strcpy(caConfFile,"/home/ncmysql/ncsrv/bin/ncsrv.cnf");    
    iO = 0;
    iU = 0;
    argv++;
    argc--;
    while(argc) {
        if(**argv != '-') {
            ncSrvPrintUsge();
            exit(0);
        }
        argval = *(*argv + 1);
        argc--;
        argv++;
        switch(argval) {
            case 'f':
                iFlags = atol(*argv);
                argc--;
                argv++;
                break;
            case 'd':
                strcpy(caConfFile,(*argv));
                argc--;
                argv++;
                break;
            case 'b':
                strcpy(caOpt,(*argv));
                argc--;
                argv++;
                break;
            case 'x':
                psShmHead=utComShmOpen(caConfFile);
                ncSrvSynUsers2Db(psShmHead);
                printf("SynUser 2 Db\n");
                return 0;
            case 'y':
                psShmHead=utComShmOpen(caConfFile);
                if(psShmHead) {
                    ncSrvShowApIndex(psShmHead);
                }
                exit(0);
            case 'i':
                psShmHead=utComShmOpen(caConfFile);
                pasReLoadConfig(psShmHead);
                ncSrvSystemInit(psShmHead);
                printf("Reload Config\n");
                return 0;
            case 'k':
                iK = 1;
                iO = 0;
                strcpy(caName,(*argv));
                argc--;
                argv++;
                break;
            case 't':
                iT = 1;   // 踢出
                iO = 0;
                memset(caName,0,32);
                strcpy(caName,(*argv));
                argc--;
                argv++;
                break;
            case 'o':
                iO = 1;
                break;
            case 'q':
                iO = 2;
                break;
            case 'm':
                iM = 1;
                break;
            case 'u':
                iU = 1;
                break;
            case 'B':
                iB = 1;
                break;
            case 's':
                iOpt = 1;
                break;
            case 'g':
                iOpt = 2;
                break;
            case 'a':
                iOpt = 3;
                break;
            case 'j':
                iOpt = 4;
                break;
            case 'c':
                iOpt = 5;
                break;
            case 'n':
                iOpt = 6;
                break;
            case 'l':
                iOpt = 7;
                break;
            default:
                ncSrvPrintUsge();
                exit(0);
        }
    }
    psShmHead=utComShmOpen(caConfFile);
    if(psShmHead == NULL) {
        printf(" Share Memory not Init \n");
        exit(0);
    }
    switch(iOpt) {
        case 1:
			ictShowPackage(psShmHead);
            //ncSrvShowProcess(psShmHead);
            exit(0);
        case 2:
            ncSrvShowGroup(psShmHead);
            exit(0);
        case 3:
            ncSrvShowGroupAp(psShmHead);
            exit(0);
        case 4:
            {
                ncPortalAp *psAp;
                char caAp[32],nasid[32];
                utStrGet_s("ApId:",caAp);
                memset(nasid,0,32);
                strcpy(nasid,caAp);
                psAp = (ncPortalAp *)utShmHashLook(psShmHead,PORTAL_LNK_GROUPAP,nasid);
                if(psAp) {
                    ncPortalGroup *psGroup;
                    psGroup = (ncPortalGroup *) ncSrvGetGroupInfoById(psShmHead,psAp->lGroupid);
                    if(psGroup) {
                        printf(" GroupId = %u  Name:%s \n",
                              psAp->lGroupid,psGroup->caGroupName);
                    }
                    else {
                        printf(" GroupId = %u  \n",
                              psAp->lGroupid);
                    }                        
                }
                else {
                    printf(" No Find \n");
                }
            }    
            exit(0);
        case 5:
            {
                ncPortalSummary *psSumm;
                psSumm = (ncPortalSummary *)utShmArray(psShmHead,NCSRV_LNK_SYSINFO);
                if(psSumm) {
                    printf(" Name:%s \n",psSumm->caPortalName);
                    printf(" caProdSn:%s \n",psSumm->caProdSn);
                    printf(" SumPlateRule:%u \n",psSumm->lSumPlateRule);
                    if(psSumm->lSumPlateRule > 0) {
                        ncPortalPlateRule *psRule;
                        psRule = (ncPortalPlateRule *)utShmArray(psShmHead,PORTAL_LNK_PLATERULE);
                        if(psRule) {
                            for(i=0;i<psSumm->lSumPlateRule;i++) {
                                printf("id:%u Code:%s  Plate:%s  Ssid:%s  NasId:%s  Mark:%s doTime:%s  DateId:%d StartTime:%u EndTime:%u\n",
                                        psRule[i].ruleid,
                                        psRule[i].groupcode,
                                        psRule[i].platename,
                                        psRule[i].ssid,
                                        psRule[i].nasid,
                                        psRule[i].mark,
                                        psRule[i].dotime,
                                        psRule[i].dateid,
                                        psRule[i].starttime,
                                        psRule[i].endtime);
                            }
                        }
                    }
                    printf(" SumPar:%u \n",psSumm->lSumPar);
                    if(psSumm->lSumPar > 0) {
                        ncPortalPar *psPar;
                        psPar = (ncPortalPar *)utShmArray(psShmHead,PORTAL_LNK_PORTALPAR);
                        for(i=0;i<psSumm->lSumPar;i++) {
                            printf(" Flags:%d  Id:%u    Code:%s    Session:%u  Idel:%u  cExceedMac:%d MaxMac:%d\n",
                                psPar[i].cFlags,
                                psPar[i].lId,
                                psPar[i].caGroupCode,
                                psPar[i].lSesstime,
                                psPar[i].lIdletime,
                                psPar[i].cExceedMac,
                                psPar[i].cSumMac);
                        }
                    }
                    printf(" SumStype:%u \n",psSumm->lSumStype);
                    if(psSumm->lSumStype > 0) {
                        ncPortalStype *psStype;
                        psStype = (ncPortalStype *)utShmArray(psShmHead,PORTAL_LNK_STYPE);
                        for(i=0;i<psSumm->lSumStype;i++) {
                            printf(" GroupCode:%s %d %d  Ssid:%s %d %d  Stype:%u  shopCode:%s NameCode:%s Desc:%s\n",
                                psStype[i].groupcode,
                                psStype[i].groupmatch,
                                psStype[i].grouplen,
                                psStype[i].ssid,
                                psStype[i].ssidmatch,
                                psStype[i].ssidlen,
                                psStype[i].stype,
                                psStype[i].shopcode,
                                psStype[i].namecode,
                                psStype[i].descr);
                        }
                    }
                    
                    
                }
            }    
            exit(0);    
        case 6:
            ncSrvShowAllMac(psShmHead);
            exit(0);
        case 7:
            ncSrvShowMacOnline(psShmHead);
            exit(0);
    }
    if(iO>0) {
        if(iO == 1) {
            printf(" Online User \n");
            iReturn = ncSrvShowOnlineUser(psShmHead,iFlags);
        }
        else {
            printf(" Online User \n");
            iReturn = ncSrvShowOnlineUser1(psShmHead,iFlags);
        }
    }
    if(iU) {
        printf(" UserInfo  \n");
        ncSrvShowAllUser(psShmHead);
    }
    if(iM) {
        printf(" UserMac  \n");
        ncSrvShowAllUserMac(psShmHead);
    }
    if(iB) {
        printf(" Black User  \n");
        ncSrvShowBlackUser(psShmHead);
    }
    
    // 将指定用户清掉
    if(iK) {
        iReturn = pasConnect(psShmHead);
        if(iReturn == 0) {
            ncPortalUser *psUser;
            char caName0[32];
            ncPortalOnline *psOnline;
            printf(" Delete User %s \n",caName);
            memset(caName0,0,32);
            strcpy(caName0,caName);
            psOnline = ncSrvGetOnlineUserByName(psShmHead,0,caName0);
            if(psOnline) {
                utShmHashDel(psShmHead,NCSRV_LNK_ONLINE,psOnline);
                printf(" Delete Online User %s OK \n",caName);
            }
            iReturn = pasDbExecSqlF("delete from ncsrvuser where username = '%s'",caName);
            iReturn = pasDbExecSqlF("delete from ncsrvusermac where username = '%s'",caName);
            ncSrvDelUserByName(psShmHead,psOnline?psOnline->lGroupid:0,caName0);
            ncSrvDelUserMacByName(psShmHead,psOnline?psOnline->lGroupid:0,caName0);
        }
    }
    if(iT) {
        uint8 lTsid;
        uint4 lBase;
        ncPortalOnline *psOnline;
        if(utStrIsDigital(caName)) {
            printf(" Kick User Tid=%s \n",caName);
            sscanf(caName,"%llu",&lTsid);
            lBase = utShmGetBaseAddr(psShmHead);
            psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead,lTsid);
            if(psOnline) {
                psOnline = (ncPortalOnline *) (  (char *)psOnline + lBase);
                if(psOnline->login == NCPORTAL_ONLINE_LOGIN) {
                    psOnline->login = NCPORTAL_ONLINE_LOGOUTING;
                    printf(" Kick User %s  Sucess\n",psOnline->caName);
                    psOnline->lasttime = time(0);
                }
                else {
                    printf("User:%s  Tid %llu not Login %d\n",psOnline->caName,lTsid,psOnline->login);
                }
            }
            else {
                printf(" Tid %llu not exist\n",lTsid);
            }
        }
        else {
            iReturn = ncSrvLetUserOffLineByName(psShmHead,caName);
            if(iReturn == 0) {
                printf(" User %s not exist\n",caName);
            }
            else {
                printf(" Kick User %s  Sucess\n",caName);
            }
        }            
    }
    if(!utStrIsSpaces(caOpt)) {
        if(strcasecmp(caOpt,"synpar2rad")==0 || strcasecmp(caOpt,"systemreset")==0 
            || strcasecmp(caOpt,"synradius")==0 || strcasecmp(caOpt,"synpar2rad")==0 || strcasecmp(caOpt,"synmacbind")==0 ) {
            printf(" Do  %s \n",caOpt);
            ncSrvSetCtlMark(caOpt);
        }
        else {
            printf(" Invalid ARg \n");
        }
    }
    utShmClose(psShmHead);
    exit(0);
}

int ncSrvPrintUsge()
{
    printf("portalonline usage:\n");
    printf("   portalonline      --- Show Online User \n");
    printf("   portalonline  -u  --- Show User info \n");
    printf("   portalonline  -o  --- Show Online User \n");
    printf("   portalonline  -m  --- Show UserMac \n");
    printf("   portalonline  -s  --- Show Process \n");
    printf("   portalonline  -g  --- Show Group \n");
    printf("   portalonline  -a  --- Show GroupAp \n");
    printf("   portalonline  -i  --- Reload the confige file\n");
    printf("   portalonline  -n  --- Show All Mac\n");
    printf("   portalonline  -k username  --- delete user information\n");
    printf("   portalonline  -t username  --- Kick our online user\n");
    printf("   portalonline  -b opt       --- synpar2rad | synuser2rad | systemreset | synradius | synmacbind\n");
    return 0;
}

    
