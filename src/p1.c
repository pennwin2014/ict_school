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
int main(int argc,char **argv)
{
    char argval;
    char caConfFile[64];
    char caName[32];
    int i,iReturn,iO=0,iU=0,iK=0,iT=0,iM=0,iFlags=0,iOpt=0;
    char   *p;
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
            case 'x':
                psShmHead=utComShmOpen(caConfFile);
                ncSrvSynUsers2Db(psShmHead);
                printf("SynUser 2 Db\n");
                return 0;
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
            case 'm':
                iM = 1;
                break;
            case 'u':
                iU = 1;
                break;
            case 's':
                iOpt = 1;
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
            ncSrvShowProcess(psShmHead);
            exit(0);
    }
    if(iO) {
        printf(" Online User \n");
        iReturn = ncSrvShowOnlineUser(psShmHead,iFlags);
    }
    if(iU) {
        printf(" UserInfo  \n");
        ncSrvShowAllUser(psShmHead);
    }
    if(iM) {
        printf(" UserMac  \n");
        ncSrvShowAllUserMac(psShmHead);
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
            psOnline = ncSrvGetOnlineUserByName(psShmHead,caName0);
            if(psOnline) {
                utShmHashDel(psShmHead,NCSRV_LNK_ONLINE,psOnline);
            }
            psUser = (ncPortalUser *)utShmHashLook(psShmHead,NCSRV_LNK_USERS,caName0);
            if(psUser) {
                iReturn = pasDbExecSqlF("delete from ncsrvuser where username = '%s'",
                                 psOnline->caName);
                iReturn = pasDbExecSqlF("delete from ncsrvusermac where username = '%s'",
                                 psOnline->caName);
                utShmHashDel(psShmHead,NCSRV_LNK_USERS,psUser);
            }
            ncSrvDellUserMacByName(psShmHead,caName0);
        }
    }
    if(iT) {
        ncPortalOnline *psOnline;
        printf(" Kick User %s \n",caName);
        psOnline = ncSrvGetOnlineUserByName(psShmHead,caName);
        if(psOnline) {
            if(psOnline->lSip > 0) {
                ncPortalAcInfo *psAc;
                psAc = (ncPortalAcInfo *)ncSrvGetAcInfo(psShmHead,psOnline->caAcName);
                if(psAc) {
                    char caUserIp[32];
                    sprintf(caUserIp,"%s",utComHostIp(htonl(psOnline->lSip)));
                    iReturn = ncSrvSendToAcRequestLogout(psAc->lAcip,psAc->nAcPort,0,0,caUserIp,0);
                    printf(" Kick User %s  Return=%d\n",caName,iReturn);
                }
            }
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
    printf("   portalonline  -i  --- Reload the confige file\n");
    printf("   portalonline  -k username  --- delete user information\n");
    printf("   portalonline  -t username  --- Kick our online user\n");
    return 0;
}

    
