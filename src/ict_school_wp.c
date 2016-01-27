#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include "ncportal.h"
#include "portalweb.h"
//微信新增
#include <iconv.h>
#include <ctype.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include "cjson.h"
#include "ict_zjj_tool.h"
#include "ict_wp_tool.h"

#define FILEPATH "/home/ncmysql/ncsrv/html/ncm_appreport"


//根据ID获取单位
char *authGetClientName(utShmHead *psShmHead, unsigned long lId)
{
    authClient *psClient;
    authClientId2Code *psCode;
    static char sKey[64];
    char caServicecode[32];
    memset(caServicecode, 0, sizeof(caServicecode));

    psCode = (authClientId2Code *)utShmHashLook(psShmHead, PROAUTH_LNK_CLIENTIDCODE, (char*)&lId);
    if(psCode)
    {

        strcpy(caServicecode, psCode->code);
    }
    else
    {
        strcpy(sKey, "");
        return &sKey[0];
    }

    psClient = (authClient *)utShmHashLook(psShmHead, PROAUTH_LNK_CLIENT, caServicecode);
    if(psClient)
    {

        strcpy(sKey, psClient->dispname);
    }
    else
    {
        strcpy(sKey, "");
    }

    return &sKey[0];
}

char *authGetGroupNameById(utShmHead *psShmHead, unsigned long lGroupid)
{
    authGroup *psClient;
    static char sGroup[64];
    memset(sGroup, 0, sizeof(sGroup));
    psClient = (authClient *)utShmHashLook(psShmHead, PROAUTH_LNK_GROUP, (char*)(&lGroupid));
    if(psClient)
    {
        strcpy(sGroup, psClient->groupname);
    }
    return &sGroup[0];
}

static char *strrstr(const char *haystack, const char *needle)
{
    char *r = NULL;

    if(!needle[0])
        return (char*)haystack + strlen(haystack);
    while(1)
    {
        char *p = strstr(haystack, needle);
        if(!p)
            return r;
        r = p;
        haystack = p + 1;
    }
}

static int spmUtlUploadFile(char *pIn, char *pLocal, char *pRemote, char *pType)
{
    char *p, *p0;
    strcpy(pLocal, "\0"); //本地上传文件名字
    strcpy(pRemote, "\0"); //服务器存储位置
    strcpy(pType, "\0");
    p = strstr(pIn, ";");
    if(*p == ';')
    {
        memcpy(pLocal, pIn, p - pIn);

        pLocal[p - pIn] = 0;
        p++;
        p0 = strstr(p, ";");
        if(*p0 == ';')
        {
            memcpy(pRemote, p, p0 - p);
            pRemote[p0 - p] = 0;
            strcpy(pType, p0 + 1);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
    return 0;
}

//将逗号分割的字符拆分成一个数组
int sepcharbydh(char *in, char caOut[200][256])
{
    char *p, *p2;
    long lCount;
    long m = 0;
    lCount = 0;
    if(strlen(in) == 0) return 0;
    p = in;
    memset(caOut, 0, 200 * 256);
    while(*p)
    {
        if(*p == ',')
        {
            caOut[lCount][m] = '\0';

            m = 0;
            p++;
            lCount++;
            if(lCount >= 200) return lCount;
        }
        else
        {
            if(m < 35)
            {
                caOut[lCount][m] = *p;
                p++;
                m++;
            }
        }
    }
    return lCount + 1;
}

//将逗号分割的字符拆分成一个数组
static int sepcharbydh2(char *in, char caOut[200][256])
{
    char *p, *p2;
    long lCount;
    long m = 0;
    lCount = 0;
    if(strlen(in) == 0) return 0;
    p = in;
    while(*p)
    {
        //  printf("lCount=%d\n",lCount);
        if(*p == ',')
        {
            caOut[lCount][m] = '\0';
            m = 0;
            p++;
            lCount++;
            if(lCount >= 200) return lCount;
        }
        else
        {
            if(m < 256)
            {
                caOut[lCount][m] = *p;
                p++;
                m++;
            }
        }
    }
    return lCount + 1;
}


int utPltHtmlFileDbhead(int iFd, utMsgHead *psMsgHead, utPltDbHead *psDbHead, char *pFile)
{
    int  iFd1, iReturn, iFileSize;
    struct stat  sStatBuf;
    char *p, *pBuffer;

    iFd1 = open(pFile, O_RDONLY);
    if(iFd1 < 0)
    {
        utSysLog(" File %s Open Error ", pFile);
        return (-1);
    }

    iReturn = fstat(iFd1, &sStatBuf);
    if(iReturn == (-1))
    {
        utSysLog(" fStat Error FileName == %s", pFile);
        close(iFd1);
        return 0;
    }
    iFileSize = sStatBuf.st_size;
    pBuffer = malloc(iFileSize + 1);
    if(pBuffer == NULL)
    {
        utSysLog(" Memoty Alloc Error in utPltFileToHtml");
        close(iFd1);
        return (-1);
    }
    iReturn = read(iFd1, pBuffer, iFileSize);
    close(iFd1);
    if(iReturn != iFileSize)
    {
        free(pBuffer);
        return (-1);
    }
    pBuffer[iFileSize] = 0;

    p = utPltToHtmBuf(pBuffer, psDbHead);
    if(p)
    {
        utComTcpResponse(iFd, psMsgHead, 1,
                         "html", UT_TYPE_STRING, p);
    }
    free(pBuffer);
    utPltFreeDb(psDbHead);
    return 0;
}


int adStrReplaceWith(char *pBuf, char *s, char *d)
{
    char *p;
    int i = strlen(s);
    int j = strlen(d);
    int l;
    int iMax;
    int iCount = 0;
    if(i > j)
    {
        iMax = i;
    }
    else
    {
        iMax = j;
    }
    p = strstr(pBuf, s);
    while(p)
    {
        iCount++;
        if(i != j)
        {
            l = strlen(p);
            memmove(p + j, p + i, l - i + 1);
        }
        memcpy(p, d, j);
        p = strstr(p + iMax, s);
    }
    return iCount;
}



//加载单位信息
int ictProAuthLoadClientsInfo(utShmHead *psShmHead)
{
    unsigned long   lMaxClient;
    int             iReturn, iRet;
    char            *p;
    unsigned long lGroupid;

    pasDbCursor *psCur;
    authClient s, *psData;
    authClientId2Code sc;
    lMaxClient = utComGetVar_ld(psShmHead, "MaxClients", 15000);

    utShmFreeHash(psShmHead, PROAUTH_LNK_CLIENTIDCODE); /* 先释放  */

    iReturn = utShmHashInit(psShmHead, PROAUTH_LNK_CLIENTIDCODE, lMaxClient, lMaxClient, sizeof(authClientId2Code), 0, 4);

    utShmFreeHash(psShmHead, PROAUTH_LNK_CLIENT); /* 先释放  */

    iReturn = utShmHashInit(psShmHead, PROAUTH_LNK_CLIENT, lMaxClient, lMaxClient, sizeof(authClient), 0, 30);
    if(iReturn != 0)
    {
        printf("Share Memory alloc Error at ProAuthLoadClientsInfo\n");
        return (-1);
    }
    /* 将单位资料导入到内存中  */
    psCur = pasDbOpenSqlF("select groupid,groupname,dispname,longitude,latitude from ncsrvgroup ");

    if(psCur == NULL)
    {

        return (-1);
    }

    memset(&s, 0, sizeof(s));
    memset(&sc, 0, sizeof(sc));
    iReturn = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &s.userid,
                             UT_TYPE_STRING, 30, s.servicecode,
                             UT_TYPE_STRING, 127, s.dispname,
                             UT_TYPE_FLOAT, sizeof(double), &s.longitude,
                             UT_TYPE_FLOAT, sizeof(double), &s.latitude);


    char caServicecode[20];
    while(iReturn == 0 || iReturn == 1405)
    {
        // #printf(" LoadUser:%lu %s \n",s.compid,s.compname);
        sc.id = s.userid;
        strcpy(sc.code, s.servicecode);
        utShmHashAdd(psShmHead, PROAUTH_LNK_CLIENTIDCODE, (char*)&sc, 1);

        iRet = utShmHashAdd(psShmHead, PROAUTH_LNK_CLIENT, (char*)&s, 1);
        if(iRet < 0)
        {
            //   printf("fffffffffffffffff\n");
            pasDbCloseCursor(psCur);
            return (-1);
        }
        memset(&s, 0, sizeof(s));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &s.userid,
                                 UT_TYPE_STRING, 30, s.servicecode,
                                 UT_TYPE_STRING, 127, s.dispname,
                                 UT_TYPE_FLOAT, sizeof(double), &s.longitude,
                                 UT_TYPE_FLOAT, sizeof(double), &s.latitude);
    }
    pasDbCloseCursor(psCur);
    return 0;
}

//加载单位组信息
int ictProAuthLoadGroupInfo(utShmHead *psShmHead)
{
    unsigned long   lMaxClient;
    int             iReturn, iRet;
    char            *p;
    unsigned long lGroupid;
    pasDbCursor *psCur;
    authGroup s, *psData;
    lMaxClient = utComGetVar_ld(psShmHead, "MaxGroups", 300);

    utShmFreeHash(psShmHead, PROAUTH_LNK_GROUP); /* 先释放  */

    iReturn = utShmHashInit(psShmHead, PROAUTH_LNK_GROUP, lMaxClient, lMaxClient, sizeof(authGroup), 0, 4);
    if(iReturn != 0)
    {
        printf("Share Memory alloc Error at ProAuthLoadGroupsInfo\n");
        return (-1);
    }
    /* 将单位组资料导入到内存中  */
    psCur = pasDbOpenSqlF("select groupid,groupname,pid from ncsgroup ");
    if(psCur == NULL)
    {

        return (-1);
    }

    memset(&s, 0, sizeof(s));
    iReturn = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &s.groupid,
                             UT_TYPE_STRING, 63, s.groupname,
                             UT_TYPE_ULONG, 4, &s.pid);

    while(iReturn == 0 || iReturn == 1405)
    {
        // #printf(" LoadUser:%lu %s \n",s.compid,s.compname);
        //   printf("key=%s\n",authGetClientKey(psShmHead,caServicecode));
        iRet = utShmHashAdd(psShmHead, PROAUTH_LNK_GROUP, (char*)&s, 1);
        if(iRet < 0)
        {
            pasDbCloseCursor(psCur);
            return (-1);
        }
        memset(&s, 0, sizeof(s));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &s.groupid,
                                 UT_TYPE_STRING, 63, s.groupname,
                                 UT_TYPE_ULONG, 4, &s.pid);
    }
    pasDbCloseCursor(psCur);
    return 0;
}
/*
//广告模板
int ictAdplatelist_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
#if SYSTEM_LOG
    utMsgPrintMsg(psMsgHead);
#endif
    char caPid[16];
    char caTemp[4024];
    char caKeyword[64];
    char caStart[32], caLimit[32], caSort[32], caDir[32];
    unsigned long lSid, lGroupid, lAddtime, lModtime, lCount;
    char caDel[16], caSelsid[1024];
    char caAdcode[32], caAdname[64], caAdurl[128], caAdfile[128], caAdfile_new[128], caDaterang[256], caMark[1024], caApssid[256];
    char caOut[200][256];
    char caPlatename[128], caAdindex[64], caPlatepath[128], caTermtype[32];
    char caSsid[128], caAp[128], caAc[128], caTermfac[64], caMobsp[32], caMobarea[32], caOs[64], caBrower[128], caDatarang[256];
    unsigned long lDid, lWay, lPri, lCid, cCount;
    char caGroups[2048], caCorps[2048];
    char caPlateid[24];
    long lCount_g = 0;
    long lLangflag, lStatus;
    long lPid, i;
    long lTermtype;
    int iNum = 0;
    int iNum_s = 0;
    int iret = -1;
    char caStatus[16];
    char caTemp_d[128];
    struct  stat st, st1;
    struct dirent *d, *d2;
    DIR *dir, *dir2;
    long m;
    char caFilename[256];
    char caTemp_g[2000], caTemp_c[2000];
    char caTemp1[4000];
    struct portalplate_s
    {
        char plateid[16];
        char platename[64];
        char mark[2048];
        char ssid[2048];
        char ap[2048];
        char groups[2048];
        char corps[2048];
    } portalplate;
    char caAdPlate[128];
    char caIpPort[32];
    char caLang[36], caPath[64];
    char caFullfile[128];
    struct portalplate_s *pData;
    sprintf(caIpPort, "%s:%s", utComGetVar_sd(psShmHead, "AdminIp", ""), utComGetVar_sd(psShmHead, "AuthHttpPort", ""));

    strcpy(caAdPlate, utComGetVar_sd(psShmHead, "adplate", "/home/adplate"));
    if(caAdPlate[strlen(caAdPlate) - 1] != '/')
    {
        strcat(caAdPlate, "/");
    }

    long iNum_t = 0;
    long lSum = 0;
    long iReturn;
    utMsgGetSomeNVar(psMsgHead, 7,
                     "pid",  UT_TYPE_STRING,  10,        caPid,
                     "status", UT_TYPE_STRING, 10, caStatus,
                     "del",      UT_TYPE_STRING, 10, caDel,
                     "selsid",   UT_TYPE_STRING, 1000, caSelsid,
                     "keyword",  UT_TYPE_STRING,  sizeof(caKeyword) - 1,       caKeyword,
                     "start",        UT_TYPE_STRING,  sizeof(caStart) - 1,             caStart,
                     "limit",        UT_TYPE_STRING,  sizeof(caLimit) - 1,             caLimit);
    utMsgPrintMsg(psMsgHead);

    if(!utStrIsSpaces(caKeyword))
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
        strcpy(caKeyword, caTemp_d);
    }

    char sql[1024] = "";
    char sql_tmp[1024] = "";
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();


    if(strcmp(caDel, "del") == 0 && strlen(caSelsid) > 0)
    {

    }
    snprintf(sql, sizeof(sql), "select count(*) from portalplate where 1=1  ");
    if(strlen(caKeyword) > 0)
    {
        sprintf(sql + strlen(sql), " and (platename like '%c%s%c' or plateid like '%c%s%c') ", '%', caKeyword, '%', '%', caKeyword, '%');
    }
    pasDbOneRecord(sql, 0, UT_TYPE_LONG, sizeof(long), &lCount);


    pData = (struct portalplate_s *)malloc((lCount + 1) * sizeof(struct portalplate_s));
    if(pData == NULL)
    {
        return -1;
    }

    utPltPutVarF(psDbHead, "TotRec", "%u", lCount);

    sprintf(sql, "select platename,plateid from portalplate where 1=1 ");
    if(strlen(caKeyword) > 0)
    {
        sprintf(sql + strlen(sql), " and (platename like '%c%s%c' or plateid like '%c%s%c') ", '%', caKeyword, '%', '%', caKeyword, '%');
    }

    sprintf(sql + strlen(sql), " order by plateid desc  ");

    printf("sql=%s\n", sql);

    psCur = pasDbOpenSql(sql, 0);

    if(NULL != psCur)
    {

        memset(caPlatename, 0, sizeof(caPlatename));
        memset(caPlateid, 0, sizeof(caPlateid));

        iNum = 0;
        while(0 == (iret = pasDbFetchInto(psCur,
                                          UT_TYPE_STRING, 63, caPlatename,
                                          UT_TYPE_STRING, 16, caPlateid)) || 1405 == iret)
        {

            strcpy(pData[lSum].plateid, caPlateid);
            strcpy(pData[lSum].platename, caPlatename);
            strcpy(pData[lSum].mark, caMark);
            strcpy(pData[lSum].ssid, caSsid);
            strcpy(pData[lSum].ap, caAp);
            strcpy(pData[lSum].groups, caGroups);
            strcpy(pData[lSum].corps, caCorps);
            lSum++;


            memset(caPlatename, 0, sizeof(caPlatename));
            memset(caPlateid, 0, sizeof(caPlateid));
            memset(caMark, 0, sizeof(caMark));
            memset(caSsid, 0, sizeof(caSsid));
            memset(caAp, 0, sizeof(caAp));
            memset(caAc, 0, sizeof(caAc));
            memset(caGroups, 0, sizeof(caGroups));
            memset(caCorps, 0, sizeof(caCorps));


        }
        pasDbCloseCursor(psCur);

        iNum = 0;
        for(i = 0; i < lSum; i++)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            memset(caPlatepath, 0, sizeof(caPlatepath));
            sprintf(caPlatepath, "%s%s", caAdPlate, pData[i].plateid);
            utPltPutLoopVar(psDbHead, "platepath", iNum, caPlatepath);
            utPltPutLoopVarF(psDbHead, "sid", iNum, "%s", pData[i].plateid);
            utPltPutLoopVar(psDbHead, "platename", iNum, pData[i].platename);
            utPltPutLoopVar(psDbHead, "mark", iNum, pData[i].mark);
            utPltPutLoopVar(psDbHead, "ssid", iNum, pData[i].ssid);
            utPltPutLoopVar(psDbHead, "ap", iNum, pData[i].ap);
            utPltPutLoopVar(psDbHead, "groups", iNum, pData[i].groups);
            utPltPutLoopVar(psDbHead, "corps", iNum, pData[i].corps);
            lCount_g = sepcharbydh(pData[i].groups, caOut);
            memset(caTemp_g, 0, sizeof(caTemp_g));
            if(strstr(caGroups, "all"))
            {
                strcpy(caTemp_g, "全部");
            }
            else
            {
                for(m = 0; m < lCount_g; m++)
                {
                    if(m == 0)
                    {
                        strcpy(caTemp_g, authGetGroupNameById(psShmHead, atol(caOut[m])));
                    }
                    else
                    {
                        sprintf(caTemp_g + strlen(caTemp_g), ",%s", authGetGroupNameById(psShmHead, atol(caOut[m])));
                    }
                }
            }

            lCount_g = sepcharbydh(pData[i].corps, caOut);
            memset(caTemp_c, 0, sizeof(caTemp_c));
            printf("corps=%s,count=%d\n", pData[i].corps, lCount_g);
            if(strstr(caCorps, "all"))
            {
                strcpy(caTemp_g, "全部");
            }
            else
            {

                for(m = 0; m < lCount_g; m++)
                {
                    if(m == 0)
                    {
                        strcpy(caTemp_c, authGetClientName(psShmHead, atol(caOut[m])));
                    }
                    else
                    {
                        sprintf(caTemp_c + strlen(caTemp_c), ",%s", authGetClientName(psShmHead, atol(caOut[m])));
                    }
                }
            }
            utPltPutLoopVar(psDbHead, "groupsname", iNum, caTemp_g);
            utPltPutLoopVar(psDbHead, "corpsname", iNum, caTemp_c);
            memset(caTemp, 0, sizeof(caTemp));
            if(strlen(pData[i].mark) > 0)
            {
                sprintf(caTemp + strlen(caTemp), "标签：[%s] ", pData[i].mark);
            }
            if(strlen(pData[i].ssid) > 0)
            {
                sprintf(caTemp + strlen(caTemp), "SSID：[%s] ", pData[i].ssid);
            }
            if(strlen(pData[i].ap) > 0)
            {
                sprintf(caTemp + strlen(caTemp), "AP：[%s] ", pData[i].ap);
            }

            if(strlen(caTemp_g) > 0)
            {
                sprintf(caTemp + strlen(caTemp), "单位组：[%s] ", caTemp_g);
            }
            if(strlen(caTemp_c) > 0)
            {
                sprintf(caTemp + strlen(caTemp), "单位：[%s] ", caTemp_c);
            }
            strcpy(caTemp1, caTemp);
            utPltPutLoopVar(psDbHead, "procy", iNum, caTemp1);
            //      printf("caTemp=%s\n",caTemp);
            sprintf(caTemp, "select sid,lang,termtype,path from portplatetermlang where plateid='%s' order by termtype,lang ", pData[i].plateid);
            printf("caTemp=%s\n", caTemp);
            psCur = pasDbOpenSql(caTemp, 0);
            if(psCur != NULL)
            {
                lSid = 0;
                memset(caLang, 0, sizeof(caLang));
                memset(caTermtype, 0, sizeof(caTermtype));
                memset(caPath, 0, sizeof(caPath));
                iNum_s = 0;
                while(0 == (iret = pasDbFetchInto(psCur,
                                                  UT_TYPE_LONG, 4, &lSid,
                                                  UT_TYPE_STRING, sizeof(caLang) - 1, caLang,
                                                  UT_TYPE_STRING, sizeof(caTermtype) - 1, caTermtype,
                                                  UT_TYPE_STRING, sizeof(caPath) - 1, caPath)) || 1405 == iret)
                {

                    iNum_s++;
                    if(iNum_s > 1)
                    {
                        utPltPutLoopVar2(psDbHead, "dhs", iNum, iNum_s, ",");
                    }
                    utPltPutLoopVar2(psDbHead, "sid", iNum, iNum_s, pData[i].plateid);
                    sprintf(caTemp, "%s_%s", caTermtype, caLang);
                    utPltPutLoopVar2(psDbHead, "platename", iNum, iNum_s, caTemp);
                    sprintf(caTemp, "%s%s", caTermtype, caLang);
                    utPltPutLoopVar(psDbHead, caTemp, iNum, caPath);


                    //                     utPltPutLoopVar2(psDbHead,caTemp,iNum,iNum_s,caPath);

                    memset(caPlatepath, 0, sizeof(caPlatepath));
                    sprintf(caPlatepath, "%s%s/%s", caAdPlate, pData[i].plateid, caPath);
                    utPltPutLoopVar2(psDbHead, "platepath", iNum, iNum_s, caPlatepath);

                    iReturn = stat(caPlatepath, &st);
                    if(iReturn == 0)
                    {
                        if(S_ISDIR(st.st_mode) && (dir = opendir(caPlatepath)))
                        {
                            iNum_t = 0;
                            while(d = readdir(dir))
                            {
                                if(d->d_name[0] == '.') continue;
                                sprintf(caFilename, "%s/%s", caPlatepath, d->d_name);
                                //                              printf("caFilename=%s\n",caFilename);
                                if(stat(caFilename, &st1) == 0)
                                {

                                    if(strstr(d->d_name, ".htm") && (!strstr(d->d_name, "htm.")) && (!strstr(d->d_name, "html.")))
                                    {
                                        iNum_t++;
                                        if(iNum_t > 1)
                                        {
                                            utPltPutLoopVar3(psDbHead, "dht", iNum, iNum_s, iNum_t, ",");
                                        }
                                        utPltPutLoopVar3(psDbHead, "platename", iNum, iNum_s, iNum_t, d->d_name);
                                        utPltPutLoopVar3(psDbHead, "platepath", iNum, iNum_s, iNum_t, caPlatepath);
                                        utPltPutLoopVar3(psDbHead, "ipport", iNum, iNum_s, iNum_t, caIpPort);
                                        sprintf(caFullfile, "%s/%s", caPlatepath, d->d_name);
                                        utPltPutLoopVar3(psDbHead, "pfile1", iNum, iNum_s, iNum_t, caFullfile);
                                        //                                 utPltPutLoopVar3(psDbHead,"procy",iNum,iNum_s,iNum_t,caTemp1);
                                    }
                                }
                            }
                            closedir(dir);
                        }

                    }
                    memset(caLang, 0, sizeof(caLang));
                    memset(caTermtype, 0, sizeof(caTermtype));
                    memset(caPath, 0, sizeof(caPath));

                }
                pasDbCloseCursor(psCur);

            }

        }

    }

    free(pData);

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_adplate_list.htm");
    return 0;
}
*/
int doParseAdvert(utPltDbHead *psDbHead, const char* caPid, const char* caFilename, const char* advertMark, const char* pBuf, long iNum_s, long iNum)
{
    char caIpPort[32] = "";
    char *p = NULL;
    long iReturn = 0, iNum_t = 0;
    long lCount_t = 0;
    char caAd[128] = "";
    char caAdSize[64] = "";
    char caTemp2[1024] = "", caTemp[1024] = "";
    p = strstr(pBuf, advertMark);
    //             printf("pBuf=%s\n",pBuf);
    iNum_t = 0;
    memset(caTemp2, 0, sizeof(caTemp2));
    if(!p)
        return 1;
    while(p)
    {
        ncUtlGetWordBetween(p, "\"", "\"", caAd, 127);
        printf("caAd=%s\n", caAd);
        //adsize_400_400
        ncUtlGetWordBetween(p, "\"adsize_", "\"", caAdSize, 64);
        printf("adsize=%s\n", caAdSize);
        iNum_t++;
        if(iNum_t > 1)
        {
            utPltPutLoopVar3(psDbHead, "dht", iNum, iNum_s, iNum_t, ",");
        }
        if(iNum_t == 1)
        {
            strcpy(caTemp2, caAd);
        }
        else
        {
            sprintf(caTemp2 + strlen(caTemp2), ",%s", caAd);
        }

        utPltPutLoopVar3(psDbHead, "pfile", iNum, iNum_s, iNum_t, (char*)caFilename);
        utPltPutLoopVar3(psDbHead, "plateindex", iNum, iNum_s, iNum_t, caAd);
        utPltPutLoopVar3(psDbHead, "plateid", iNum, iNum_s, iNum_t, (char*)caPid);
        utPltPutLoopVar2(psDbHead, "platepath", iNum, iNum_s, (char*)caFilename);
        utPltPutLoopVar3(psDbHead, "ipport", iNum, iNum_s, iNum_t, caIpPort);

        lCount_t = 0;


        sprintf(caTemp, "select count(*) from portalplateindex where plateid='%s' and plateindex='%s'  ", caPid, caAd);
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lCount_t);

        if(lCount_t > 0)
        {
            pasDbExecSqlF("update portalplateindex set adsize='%s' where plateid='%s' and plateindex='%s' ", caAdSize, caPid, caAd);
        }
        else
        {
            printf("insert into portalplateindex(plateid,plateindex,defsecad,sectitle,secdesc,addtime,modtime,adsize) \
										              		 		values('%s','%s','','','',%d,%d,'%s')", caPid, caAd, time(0), time(0), caAdSize);

            pasDbExecSqlF("insert into portalplateindex(plateid,plateindex,defsecad,sectitle,secdesc,addtime,modtime,adsize) \
										              		 		values('%s','%s','','','',%d,%d,'%s')", caPid, caAd, time(0), time(0), caAdSize);
        }
        p = strstr(p + 1, advertMark);
    }
    if(strlen(caTemp2) > 0)
    {
        utPltPutLoopVar2(psDbHead, "plateindex", iNum, iNum_s, caTemp2);
    }
    return 0;
}


//广告模板广告位
int ictAdindexlist_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caPid[16];
    char caTemp[1024];
    char caKeyword[64];
    char caStart[32], caLimit[32], caSort[32], caDir[32];
    unsigned long lSid, lGroupid, lAddtime, lModtime, lCount;
    char caDel[16], caSelsid[1024];
    char caAdcode[32], caAdname[64], caAdurl[128], caAdfile[128], caAdfile_new[128], caDaterang[256], caMark[1024], caApssid[256];
    char caOut[200][256];
    char caPlatename[128], caAdindex[64], caPlatepath[128], caTermtype[32];
    long lLangflag, lStatus;
    char caFile[128], caPlateindex[128];
    long lPlateid;
    long lPid;
    int iNum = 0;
    int iret = -1;
    char caStatus[16];
    char caPfile[128];
    char caIpPort[32];
    long iReturn, iNum_s, iNum_t;
    struct  stat st, st1;
    struct dirent *d, *d2, *d3;
    DIR *dir, *dir2, *dir3;
    char caAd[128];
    FILE *fp, *fp1;

    char *pPath, *pBuf;
    unsigned long lSize;
    char caAdSize[64] = "";
    long lCount_t;
    char caFilename[256], caFilename3[256];
    char caAdPlate[128];
    sprintf(caIpPort, "%s:%s", utComGetVar_sd(psShmHead, "AdminIp", ""), utComGetVar_sd(psShmHead, "AuthHttpPort", ""));

    utMsgGetSomeNVar(psMsgHead, 6,
                     "pid",  UT_TYPE_STRING,  15,        caPid,
                     "del",      UT_TYPE_STRING, 10, caDel,
                     "selsid",   UT_TYPE_STRING, 1000, caSelsid,
                     "keyword",  UT_TYPE_STRING,  sizeof(caKeyword) - 1,       caKeyword,
                     "start",        UT_TYPE_STRING,  sizeof(caStart) - 1,             caStart,
                     "limit",        UT_TYPE_STRING,  sizeof(caLimit) - 1,             caLimit);
    utMsgPrintMsg(psMsgHead);

    char sql[1024] = "";
    char sql_tmp[1024] = "";
    char caTemp2[1024];
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();


    if(strcmp(caDel, "del") == 0 && strlen(caSelsid) > 0)
    {

        lCount = sepcharbydh2(caSelsid, caOut);
        for(iNum = 0; iNum < lCount; iNum++)
        {
            //     printf("delete from portadisstype where pid=%s \n",caOut[iNum]);
            iret = pasDbExecSqlF("delete from portplateindex where sid=%s ", caOut[iNum]);


        }
    }


    if(strlen(caPid) > 0)
    {
        strcpy(caAdPlate, utComGetVar_sd(psShmHead, "adplate", "/home/adplate"));
        if(caAdPlate[strlen(caAdPlate) - 1] != '/')
        {
            strcat(caAdPlate, "/");
        }
        memset(caPlatepath, 0, sizeof(caPlatepath));
        sprintf(caPlatepath, "%s%s", caAdPlate, caPid);

        //  printf("caPlatepath1=%s\n",caPlatepath);
        iReturn = stat(caPlatepath, &st);
        if(iReturn == 0)
        {
            if(S_ISDIR(st.st_mode) && (dir = opendir(caPlatepath)))
            {
                iNum = 0;
                while(d = readdir(dir))
                {
                    if(d->d_name[0] == '.' || strstr(d->d_name, "tmp") || strstr(d->d_name, "images") || strstr(d->d_name, "css") || strstr(d->d_name, "js")) continue;
                    sprintf(caFilename, "%s/%s", caPlatepath, d->d_name);
                    if(stat(caFilename, &st1) == 0)
                    {
                        if(S_ISDIR(st1.st_mode) && (dir3 = opendir(caFilename)))
                        {

                            //                                  printf("dname=%s\n",d->d_name);
                            iNum++;
                            if(iNum > 1)
                            {
                                utPltPutLoopVar(psDbHead, "dh", iNum, ",");

                            }
                            utPltPutLoopVar(psDbHead, "pfile", iNum, d->d_name);
                            utPltPutLoopVar(psDbHead, "plateid", iNum, caPid);
                            utPltPutLoopVar(psDbHead, "platepath", iNum, caPlatepath);
                            iNum_s = 0;
                            while(d3 = readdir(dir3))
                            {
                                if(d3->d_name[0] == '.') continue;
                                //                   printf("caFileName=%s,caCback=%s\n",caFileName,caCback);
                                if(strstr(d3->d_name, ".htm") && (!strstr(d3->d_name, "htm.")) && (!strstr(d3->d_name, "html.")))
                                {
                                    iNum_s++;
                                    sprintf(caFilename3, "%s/%s", caFilename, d3->d_name);
                                    //    printf("caFileName3=%s\n",caFilename3);
                                    if(iNum_s > 1)
                                    {
                                        utPltPutLoopVar2(psDbHead, "dhs", iNum, iNum_s, ",");
                                    }
                                    utPltPutLoopVar2(psDbHead, "pfile1", iNum, iNum_s, caFilename3);
                                    utPltPutLoopVar2(psDbHead, "pfile", iNum, iNum_s, d3->d_name);
                                    utPltPutLoopVar2(psDbHead, "plateid", iNum, iNum_s, caPid);

                                    utPltPutLoopVar2(psDbHead, "platepath", iNum, iNum_s, caFilename);

                                    utPltPutLoopVar2(psDbHead, "ipport", iNum, iNum_s, caIpPort);
                                    fp = fopen(caFilename3, "r");
                                    if(fp)
                                    {
                                        lSize = utFileSize(caFilename3);

                                        pBuf = malloc(lSize + 4000);
                                        memset(pBuf, 0, lSize + 1);
                                        fread(pBuf, lSize, 1, fp);
                                        //解析login_main页面的广告位
                                        if(doParseAdvert(psDbHead, caPid, caFilename, "id=\"advert_", pBuf, iNum_s, iNum))
                                        {
                                            //未解析出则解析auto页面广告位
                                            if(doParseAdvert(psDbHead, caPid, caFilename, "id=\"advertauto_", pBuf, iNum_s, iNum))
                                            {
                                                //仍未解析出解析ok页面广告位
                                                doParseAdvert(psDbHead, caPid, caFilename, "id=\"advertok_", pBuf, iNum_s, iNum);
                                            }
                                        }
                                        fclose(fp);
                                        fp = fopen(caFilename3, "w");
                                        if(fp)
                                        {
                                            fprintf(fp, pBuf);
                                            fflush(fp);
                                            fclose(fp);
                                        }

                                        free(pBuf);
                                    }
                                }
                            }
                            closedir(dir3);
                        }
                    }
                }
                closedir(dir);
            }
        }
    }
    utPltPutVarF(psDbHead, "TotRec", "%d", iNum_s);

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_adplateindex_list.htm");

    return 0;
}



//上传默认广告位文件
int ictAdDefAd_upload_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead = NULL;
    char caFile[256] = "";
    char sid[17] = "";
    char *p;
    char caLocal[128] = "";
    char caRemote[128] = "";
    char caRemote_new[128] = "";
    char caType[128] = "";
    char caDfile[128];
    long iReturn, lSid;
    char caSid[20];
    char caPath[128];
    char caTemp[256], caPlatepath[256];
    char caFileName[128];
    DIR *dir, *dir2;
    struct  stat st, st1;
    struct dirent *d, *d2;
    char caAd[128], caAdPlate[128], caAdPlateimg[128] = "", caPlatepathtmp[128];
    char caNewfile[128];
    FILE *fp, *fp1;
    unsigned long lSize;
    long index;
    char caPlateid[32], caPlateindex[32];
    unsigned long lTime = time(0);
    memset(caDfile, 0, sizeof(caDfile));

    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 3,
                               "plateid",   UT_TYPE_STRING, sizeof(caPlateid) - 1, caPlateid,
                               "index", UT_TYPE_STRING, sizeof(caPlateindex) - 1, caPlateindex,
                               "adfile", UT_TYPE_STRING, sizeof(caFile) - 1, caFile);



    if(strlen(caFile) > 0)
    {
        char *descr_gbk = (char*)pasConvert("UTF-8", "GBK", caFile);
        if(descr_gbk)
        {
            strcpy(caFile, descr_gbk);
            free(descr_gbk);
        }

    }

    spmUtlUploadFile(caFile, caLocal, caRemote, caType);
    printf(" caFile==[%s]\n", caFile);
    printf("caLocal=%s,caRemote=%s\n", caLocal, caRemote);

    strcpy(caAdPlate, utComGetVar_sd(psShmHead, "adplate", "/home/adplate"));
    strcpy(caAdPlateimg, utComGetVar_sd(psShmHead, "adplateimg", "/home/adplate"));

    if(caAdPlate[strlen(caAdPlate) - 1] == '/')
    {
        caAdPlate[strlen(caAdPlate) - 1] == '\0';
    }

    p = strrstr(caRemote, "/");
    if(p != NULL)
    {
        strcpy(caDfile, ++p);

    }
    // sprintf(caPlatepath, "%s/%s/images", caAdPlate, caPlateid);
    sprintf(caPlatepath, "%s/%s/images", caAdPlateimg, caPlateid);
    if(!utFileIsExist(caPlatepath))
    {
        if(mkdir(caPlatepath, 0755) != 0)
        {
            utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "3创建模板路径出错");
            return 0;
        }

    }

    //将上传的文件重名
    sprintf(caNewfile, "/adplate/%s/images/%s_%d_%s", caPlateid, caPlateindex, lTime, caDfile);
    snprintf(caRemote_new, sizeof(caRemote_new), "%s/%s_%d_%s", caPlatepath, caPlateindex, lTime, caDfile);
    printf("caRemote_new=%s\n", caRemote_new);
    printf("caRemote=%s\n", caRemote);
    rename(caRemote, caRemote_new);

    iReturn = pasDbExecSqlF("update portalplateindex set defad='%s' where plateid='%s' and plateindex='%s' ",
                            caNewfile, caPlateid, caPlateindex);


    psDbHead = utPltInitDbHead();

    if(iReturn == 0)
    {
        utPltPutVar(psDbHead, "titel", "true");

        utPltPutVar(psDbHead, "adfile", caNewfile);
        utPltPutVar(psDbHead, "message", "上传成功");
    }
    else
    {
        utPltPutVar(psDbHead, "titel", "false");

        utPltPutVar(psDbHead, "message", "上传失败");
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_adfile.htm");


}


//保存广告模板终端也语言类型选择
int ictAdPlateTerm_Save_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead = NULL;

    long iReturn, lSid;
    char caPcch[36], caSid[20], caPceng[36], caPcdefault[36];
    char caMobilech[36], caMobileeng[36], caMobiledefault[36], caPadch[36], caPadeng[36], caPaddefault[36];
    char caDefaultch[36], caDefaulteng[36], caPcbig5[32], caMobilebig5[36], caPadbig5[36], caDefaultbig5[36], caDefaultdefault[36];
    char caTemp[1024];
    utMsgPrintMsg(psMsgHead);

    psDbHead = utPltInitDbHead();

    utMsgGetSomeNVar(psMsgHead, 17,
                     "pcch",   UT_TYPE_STRING, sizeof(caPcch) - 1, caPcch,
                     "sid", UT_TYPE_STRING, sizeof(caSid) - 1, caSid,
                     "pceng",  UT_TYPE_STRING, sizeof(caPceng) - 1, caPceng,
                     "pcdefault", UT_TYPE_STRING, sizeof(caPcdefault) - 1, caPcdefault,
                     "mobilech", UT_TYPE_STRING, sizeof(caMobilech) - 1, caMobilech,
                     "mobileeng", UT_TYPE_STRING, sizeof(caMobileeng) - 1, caMobileeng,
                     "mobiledefault", UT_TYPE_STRING, sizeof(caMobiledefault) - 1, caMobiledefault,
                     "padch",  UT_TYPE_STRING, sizeof(caPadch) - 1, caPadch,
                     "padeng", UT_TYPE_STRING, sizeof(caPadeng) - 1, caPadeng,
                     "paddefault", UT_TYPE_STRING, sizeof(caPaddefault) - 1, caPaddefault,
                     "defaultch", UT_TYPE_STRING, sizeof(caDefaultch) - 1, caDefaultch,
                     "defaulteng", UT_TYPE_STRING, sizeof(caDefaulteng) - 1, caDefaulteng,
                     "pcbig5",   UT_TYPE_STRING, sizeof(caPcbig5) - 1, caPcbig5,
                     "mobilebig5", UT_TYPE_STRING, sizeof(caMobilebig5) - 1, caMobilebig5,
                     "padbig5", UT_TYPE_STRING, sizeof(caPadbig5) - 1, caPadbig5,
                     "defaultbig5", UT_TYPE_STRING, sizeof(caDefaultbig5) - 1, caDefaultbig5,
                     "defaultdefault", UT_TYPE_STRING, sizeof(caDefaultdefault) - 1, caDefaultdefault
                    );


    if(strlen(caPcch) > 0)
    {
        pasCvtGBK(2, caPcch, caTemp, 30);
        strcpy(caPcch, caTemp);
    }
    if(strlen(caPceng) > 0)
    {
        pasCvtGBK(2, caPceng, caTemp, 30);
        strcpy(caPceng, caTemp);
    }
    if(strlen(caPcdefault) > 0)
    {
        pasCvtGBK(2, caPcdefault, caTemp, 30);
        strcpy(caPcdefault, caTemp);
    }
    if(strlen(caMobilech) > 0)
    {
        pasCvtGBK(2, caMobilech, caTemp, 30);
        strcpy(caMobilech, caTemp);
    }
    if(strlen(caMobileeng) > 0)
    {
        pasCvtGBK(2, caMobileeng, caTemp, 30);
        strcpy(caMobileeng, caTemp);
    }
    if(strlen(caMobiledefault) > 0)
    {
        pasCvtGBK(2, caMobiledefault, caTemp, 30);
        strcpy(caMobiledefault, caTemp);
    }
    if(strlen(caPadch) > 0)
    {
        pasCvtGBK(2, caPadch, caTemp, 30);
        strcpy(caPadch, caTemp);
    }
    if(strlen(caPadeng) > 0)
    {
        pasCvtGBK(2, caPadeng, caTemp, 30);
        strcpy(caPadeng, caTemp);
    }
    if(strlen(caPaddefault) > 0)
    {
        pasCvtGBK(2, caPaddefault, caTemp, 30);
        strcpy(caPaddefault, caTemp);
    }
    if(strlen(caDefaultch) > 0)
    {
        pasCvtGBK(2, caDefaultch, caTemp, 30);
        strcpy(caDefaultch, caTemp);
    }
    if(strlen(caDefaulteng) > 0)
    {
        pasCvtGBK(2, caDefaulteng, caTemp, 30);
        strcpy(caDefaulteng, caTemp);
    }

    if(strlen(caPcbig5) > 0)
    {
        pasCvtGBK(2, caPcbig5, caTemp, 30);
        strcpy(caPcbig5, caTemp);
    }
    if(strlen(caMobilebig5) > 0)
    {
        pasCvtGBK(2, caMobilebig5, caTemp, 30);
        strcpy(caMobilebig5, caTemp);
    }
    if(strlen(caPadbig5) > 0)
    {
        pasCvtGBK(2, caPadbig5, caTemp, 30);
        strcpy(caPadbig5, caTemp);
    }

    if(strlen(caDefaultbig5) > 0)
    {
        pasCvtGBK(2, caDefaultbig5, caTemp, 30);
        strcpy(caDefaultbig5, caTemp);
    }
    if(strlen(caDefaultdefault) > 0)
    {
        pasCvtGBK(2, caDefaultdefault, caTemp, 30);
        strcpy(caDefaultdefault, caTemp);
    }

    if(!strstr(caPcch, "选择模板") && strlen(caPcch) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='zh' and termtype='mspc' ", caSid);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caPcch, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','zh','mspc','%s')", caSid, caPcch);
        }
    }

    if(!strstr(caPceng, "选择模板") && strlen(caPceng) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='eng' and termtype='mspc' ", caSid);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caPceng, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) valus('%s','eng','mspc','%s')", caSid, caPceng);
        }
    }

    if(!strstr(caPcdefault, "选择模板") && strlen(caPcdefault) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='default' and termtype='mspc' ", caSid);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caPcdefault, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','default','mspc','%s')", caSid, caPcdefault);
        }
    }

    if(!strstr(caMobilech, "选择模板") && strlen(caMobilech) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='zh' and termtype='mobile' ", caSid);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caMobilech, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','zh','mobile','%s')", caSid, caMobilech);
        }
    }

    if(!strstr(caMobileeng, "选择模板") && strlen(caMobileeng) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='eng' and termtype='mobile' ", caSid);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caMobileeng, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','eng','mobile','%s')", caSid, caMobileeng);
        }
    }

    if(!strstr(caMobiledefault, "选择模板") && strlen(caMobiledefault) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='default' and termtype='mobile' ", caSid);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caMobiledefault, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','default','mobile','%s')", caSid, caMobiledefault);
        }
    }

    if(!strstr(caPadch, "选择模板") && strlen(caPadch) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='zh' and termtype='pad' ", caSid);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caPadch, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','zh','pad','%s')", caSid, caPadch);
        }
    }

    if(!strstr(caPadeng, "选择模板") && strlen(caPadeng) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='eng' and termtype='pad' ", caSid);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caPadeng, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','eng','pad','%s')", caSid, caPadeng);
        }
    }

    if(!strstr(caPaddefault, "选择模板") && strlen(caPaddefault) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='default' and termtype='pad' ", caSid);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caPaddefault, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','default','pad','%s')", caSid, caPaddefault);
        }
    }

    if(!strstr(caDefaultch, "选择模板") && strlen(caDefaultch) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='zh' and termtype='default' ", caSid);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caDefaultch, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','zh','default','%s')", caSid, caDefaultch);
        }
    }

    if(!strstr(caDefaulteng, "选择模板") && strlen(caDefaulteng) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='eng' and termtype='default' ", caSid);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caDefaulteng, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','eng','default','%s')", caSid, caDefaulteng);
        }
    }


    if(!strstr(caPcbig5, "选择模板") && strlen(caPcbig5) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='big5' and termtype='mspc' ", caSid);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caPcbig5, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','big5','mspc','%s')", caSid, caPcbig5);
        }
    }

    if(!strstr(caMobilebig5, "选择模板") && strlen(caMobilebig5) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='big5' and termtype='mobile' ", caSid);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caMobilebig5, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','big5','mobile','%s')", caSid, caMobilebig5);
        }
    }

    if(!strstr(caPadbig5, "选择模板") && strlen(caPadbig5) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='big5' and termtype='pad' ", caPadbig5);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caPadbig5, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','big5','pad','%s')", caSid, caPadbig5);
        }
    }

    if(!strstr(caDefaultbig5, "选择模板") && strlen(caDefaultbig5) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='big5' and termtype='default' ", caDefaultbig5);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caDefaultbig5, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','big5','default','%s')", caSid, caDefaultbig5);
        }
    }

    if(!strstr(caDefaultdefault, "选择模板") && strlen(caDefaultdefault) > 0)
    {
        sprintf(caTemp, "select sid from portplatetermlang where plateid='%s' and lang='default' and termtype='default' ", caDefaultdefault);
        lSid = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSid);
        if(lSid > 0)
        {
            iReturn = pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ", caDefaultdefault, lSid);
        }
        else
        {
            iReturn = pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','default','default','%s')", caSid, caDefaultdefault);
        }
    }




    if(iReturn == 0)
    {
        utPltPutVar(psDbHead, "titel", "true");
        utPltPutVarF(psDbHead, "sid", "%s", caSid);

        utPltPutVar(psDbHead, "message", "保存成功");
    }
    else
    {
        utPltPutVar(psDbHead, "titel", "false");
        utPltPutVarF(psDbHead, "sid", "%s", caSid);
        utPltPutVar(psDbHead, "message", "保存失败");
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_adfile.htm");

}


//显示当前模版子目录
int ictAdPlate_dir(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{

    char caPid[20];
    char caTemp[1024];
    char caKeyword[64];
    char caStart[32], caLimit[32], caSort[32], caDir[32];
    unsigned long lSid, lGroupid, lAddtime, lModtime, lCount;
    char caDel[16], caSelsid[1024];
    char caAdcode[32], caAdname[64], caAdurl[128], caAdfile[128], caAdfile_new[128], caDaterang[256], caMark[1024], caApssid[256];
    char caOut[200][256];
    char caPlatename[128], caAdindex[64], caPlatepath[128], caTermtype[32];
    long lLangflag, lStatus;
    char caFile[128], caPlateindex[128];
    long lPlateid;
    long lPid;
    int iNum = 0;
    int iret = -1;
    char caStatus[16];
    char caPfile[128];
    char caIpPort[32];
    long iReturn, iNum_s;
    struct  stat st, st1;
    struct dirent *d, *d2;
    DIR *dir, *dir2;
    char caAd[128];
    FILE *fp, *fp1;
    char *p;
    char *pPath, *pBuf;
    unsigned long lSize;
    char caFilename[256];
    char caAdPlate[128];
    utMsgGetSomeNVar(psMsgHead, 1,
                     "pid",  UT_TYPE_STRING,  16,        caPid);

    utMsgPrintMsg(psMsgHead);

    char sql[1024] = "";
    char sql_tmp[1024] = "";
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();
    iNum = 0;

    if(strlen(caPid) > 0)
    {
        strcpy(caAdPlate, utComGetVar_sd(psShmHead, "adplate", "/home/adplate"));
        if(caAdPlate[strlen(caAdPlate) - 1] != '/')
        {
            strcat(caAdPlate, "/");
        }
        sprintf(caPlatepath, "%s%s", caAdPlate, caPid);
        iNum_s = 0;
        printf("caPlatepath=%s\n", caPlatepath);

        if(strlen(caPlatepath) > 0)
        {
            iReturn = stat(caPlatepath, &st);
            if(iReturn == 0)
            {
                if(S_ISDIR(st.st_mode) && (dir = opendir(caPlatepath)))
                {
                    iNum = 0;
                    while(d = readdir(dir))
                    {
                        if(d->d_name[0] == '.' || strstr(d->d_name, "tmp") || strstr(d->d_name, "images") || strstr(d->d_name, "css") || strstr(d->d_name, "js")) continue;
                        sprintf(caFilename, "%s/%s", caPlatepath, d->d_name);
                        if(stat(caFilename, &st1) == 0)
                        {
                            if(S_ISDIR(st1.st_mode))
                            {
                                printf("caFilename=%s\n", d->d_name);

                                iNum++;
                                //     printf("caFileName=%s\n",caFilename);
                                if(iNum > 1)
                                {
                                    utPltPutLoopVar(psDbHead, "dh", iNum, ",");
                                }
                                utPltPutLoopVar(psDbHead, "text", iNum, d->d_name);




                            }
                        }
                    }



                    closedir(dir);
                }
            }
        }
    }
    utPltPutVarF(psDbHead, "TotRec", "%d", iNum);

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/proauth_addir_list.htm");

    return 0;
}

//预览广告位文件
int ictShowAdIndex_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead = NULL;
    char caSid[16];
    char caTemp[1024];
    unsigned long lPlateid, iReturn;
    char caPlateindex[64], caFile[128], caPfile[128], caPlatepath[128];
    char caPlateid[20];
    psDbHead = utPltInitDbHead();
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 2, "plateid",   UT_TYPE_STRING, 15, caPlateid,
                               "file", UT_TYPE_STRING, 127, caPfile);
    printf("pFile=%s\n", caPfile);
    utPltPutVar(psDbHead, "ip", utComGetVar_sd(psShmHead, "AdminIp", ""));
    utPltPutVar(psDbHead, "port", utComGetVar_sd(psShmHead, "AuthHttpPort", ""));
    /*
    sprintf(caTemp,"select plateid,plateindex,file,pfile,platepath from portplateindex a,portplate b where a.plateid=b.sid and a.sid=%s ",caSid);
    printf("caTemp=%s\n",caTemp);
    pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lPlateid,
                      UT_TYPE_STRING,63,caPlateindex,
                      UT_TYPE_STRING,127,caFile,
                      UT_TYPE_STRING,127,caPfile,
                      UT_TYPE_STRING,127,caPlatepath);
    char caAdpath[256];
    char *pPath;
    pPath=strstr(caPlatepath,"/adplate/");
    if(pPath){
    sprintf(caAdpath,"%s%s",pPath,caFile);
    utPltPutVar(psDbHead,caPlateindex,caAdpath);
    }
    sprintf(caTemp,"%s/html/%s",caPlatepath,caPfile);
    */
    //   printf("caTemp=%s\n",caTemp);
    utPltHtmlFileDbhead(iFd, psMsgHead, psDbHead, caPfile);
    //   utPltOutToHtml(iFd,psMsgHead,psDbHead,caTemp);
    //  utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","true","上传成功");

}

/* 编辑页面代码  */
int proauthShowPlateBm(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead;
    int iReturn;
    char *p, *pFile, *pBuf;
    char caFile[128], caPlateid[16];
    long lSize;
    char caLang[36];
    FILE *fp;
    utMsgGetSomeNVar(psMsgHead, 2,
                     "file", UT_TYPE_STRING,  127,       caFile,
                     "plateid",      UT_TYPE_STRING, 10, caPlateid);
    lSize = 0;
    lSize = utFileSize(caFile);
    if(lSize < 0) lSize = 128;
    psDbHead = utPltInitDb();
    utPltPutVar(psDbHead, "file", caFile);
    utPltPutVar(psDbHead, "plateid", caPlateid);
    fp = fopen(caFile, "r");
    if(fp)
    {
        pBuf = malloc(lSize + 1);
        memset(pBuf, 0, lSize + 1);
        fread(pBuf, lSize, 1, fp);
        fclose(fp);
        memset(caLang, 0, sizeof(caLang));
        ncUtlGetWordBetween(pBuf, "charset=", "\"", caLang, 30);

        if((strcasecmp(caLang, "utf-8") == 0) || strcasecmp(caLang, "utf8") == 0)
        {
            char *pTemp = malloc(lSize * 2 + 1);
            memset(pTemp, 0, sizeof(pTemp));
            pasCvtGBK(2, pBuf, pTemp, lSize * 2);
            utPltPutVar(psDbHead, "keyword", pTemp);
            free(pTemp);
        }
        else
        {
            utPltPutVar(psDbHead, "keyword", pBuf);
        }
        free(pBuf);
    }
    else
    {
        printf(" File %s does not exist\n", caFile);
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_platecode.htm");
    return 0;
}

int ictAdIndex_upload_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead = NULL;
    char caFile[256] = "";
    char sid[17] = "";
    char *p;
    char caLocal[128] = "";
    char caRemote[128] = "";
    char caRemote_new[128] = "";
    char caType[128] = "";
    char caDfile[128];
    long iReturn, lSid;
    char caSid[20];
    char caPath[128];
    char caTemp[256], caPlatepath[256];
    char caFileName[128];
    DIR *dir, *dir2;
    struct  stat st, st1;
    struct dirent *d, *d2;
    char caAd[128], caAdPlate[128], caPlatepathtmp[128];
    FILE *fp, *fp1;
    unsigned long lSize;
    long index;
    unsigned long ltime = time(0);
    memset(caDfile, 0, sizeof(caDfile));

    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "adfile",   UT_TYPE_STRING, sizeof(caFile) - 1, caFile,
                               "sid",  UT_TYPE_STRING, sizeof(sid) - 1, sid);



    if(strlen(caFile) > 0)
    {
        char *descr_gbk = (char*)pasConvert("UTF-8", "GBK", caFile);
        if(descr_gbk)
        {
            strcpy(caFile, descr_gbk);
            free(descr_gbk);
        }

    }
    lSid = atol(sid);
    spmUtlUploadFile(caFile, caLocal, caRemote, caType);
    printf(" caFile==[%s]\n", caFile);
    printf("caLocal=%s,caRemote=%s\n", caLocal, caRemote);

    //获取模板目标路径
    strcpy(caAdPlate, utComGetVar_sd(psShmHead, "adplate", "/home/adplate"));
    if(caAdPlate[strlen(caAdPlate) - 1] != '/')
    {
        strcat(caAdPlate, "/");
    }
    memset(caPlatepath, 0, sizeof(caPlatepath));
    sprintf(caPlatepath, "%s%s", caAdPlate, sid);


    /* 取得文件名*/
    p = strrstr(caRemote, "/");
    if(p != NULL)
    {
        strcpy(caDfile, ++p);

    }

    if(!utFileIsExist(caPlatepath))
    {
        if(mkdir(caPlatepath, 0755) != 0)
        {
            utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "4创建模板路径出错");
            return 0;
        }

    }
    sprintf(caPlatepathtmp, "%s/tmp", caPlatepath);
    if(!utFileIsExist(caPlatepathtmp))
    {
        if(mkdir(caPlatepathtmp, 0755) != 0)
        {
            utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "1创建模板路径出错");
            return 0;
        }

    }
    else
    {
        printf("caPlaatepathtmp=%s\n", caPlatepathtmp);
        if(strstr(caPlatepathtmp, "tmp"))
        {
            sprintf(caTemp, "rm -Rf %s", caPlatepathtmp);
            system(caTemp);
            if(mkdir(caPlatepathtmp, 0755) != 0)
            {
                utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "2创建模板路径出错");
                return 0;
            }
        }
    }
    //将上传的文件拷到临时目录下，检查是否符合目录要求
    snprintf(caRemote_new, sizeof(caRemote_new), "%s/%s", caPlatepathtmp, caDfile);
    printf("caRemote_new=%s\n", caRemote_new);
    rename(caRemote, caRemote_new);
    if(strstr(caRemote_new, ".zip"))
    {
        sprintf(caTemp, "unzip -o %s -d %s", caRemote_new, caPlatepathtmp);
        system(caTemp);
    }
    else if(strstr(caRemote_new, ".tgz"))
    {
        sprintf(caTemp, "tar xvfz %s -C %s", caRemote_new, caPlatepathtmp);
        system(caTemp);
    }
    //校验
    sprintf(caTemp, "%s/%s", caPlatepathtmp, sid);
    printf("caTemp=[%s]\n", caTemp);
    if(!utFileIsExist(caTemp))
    {
        utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "上传文件包里没有包含模板代码的目录");
        return 0;
    }
    if(strstr(caRemote_new, ".zip"))
    {
        sprintf(caTemp, "unzip -o %s -d %s", caRemote_new, caAdPlate);
        system(caTemp);
    }
    else
    {
        sprintf(caTemp, "tar xvfz %s -C %s", caRemote_new, caAdPlate);
        system(caTemp);
    }


    psDbHead = utPltInitDbHead();
    iReturn = 0;
    if(iReturn == 0)
    {
        utPltPutVar(psDbHead, "titel", "true");

        utPltPutVar(psDbHead, "adfile", caDfile);
        utPltPutVar(psDbHead, "message", "上传成功");
    }
    else
    {
        utPltPutVar(psDbHead, "titel", "false");

        utPltPutVar(psDbHead, "message", "上传失败");
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_adfile.htm");
    //  utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","true","上传成功");

}



//保存广告模板
int ictAdPlate_Save_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead = NULL;

    long iReturn, lSid;
    char caAdurl[256], caPlateindex[64];;
    char caAdfile[128], caSid[12], caAdname[64], caAdcode[32], caDaterang[128], caApssid[1024], caGroupid[16];
    unsigned long lTime = time(0);
    char caPlateid[20], caPid[20], caAdindex[64];
    char caPlatename[64], caPlatepath[128], caLang[12], caSsid[1024], caAp[1024];
    char caAc[128], caDid[12], caDatarang[256], caTermfac[64], caMobsp[32], caMark[1024];
    char caMobarea[32], caOs[64], caBrower[128], caWay[16], caPri[16], caCid[16], caLcount[16];
    char caCorpname[128], caTermtype[64];
    char caMbPlate[256];
    char caTemp[1024];
    char caStatus[32] = "";
    char caPriority[32] = "";
    char caGroups[1024], caCorps[1024];
    char caAdPlate[128];
    long lCount;
    unsigned long lShopplateid;
    utMsgPrintMsg(psMsgHead);

    psDbHead = utPltInitDbHead();

    utMsgGetSomeNVar(psMsgHead, 9,
                     "platename1",   UT_TYPE_STRING, sizeof(caPlatename) - 1, caPlatename,
                     "sid", UT_TYPE_STRING, sizeof(caPlateid) - 1, caPlateid,
                     "mark", UT_TYPE_STRING, sizeof(caMark) - 1, caMark,
                     "corps", UT_TYPE_STRING, sizeof(caCorps) - 1, caCorps,
                     "groups", UT_TYPE_STRING, sizeof(caGroups) - 1, caGroups,
                     "ssid", UT_TYPE_STRING, sizeof(caSsid) - 1, caSsid,
                     "ap",  UT_TYPE_STRING, sizeof(caAp) - 1, caAp,
                     "status", UT_TYPE_STRING, sizeof(caStatus) - 1, caStatus,
                     "priority", UT_TYPE_STRING, sizeof(caPriority) - 1, caPriority);

    strcpy(caAdPlate, utComGetVar_sd(psShmHead, "adplate", "/home/adplate"));
    if(caAdPlate[strlen(caAdPlate) - 1] != '/')
    {
        strcat(caAdPlate, "/");
    }
    if(strlen(caPlateid) > 0)
    {
        pasCvtGBK(2, caPlateid, caTemp, 63);
        strcpy(caPlateid, caTemp);


        if(!strstr(caPlateid, "英文字符") && strlen(caPlateid) > 0)
        {

            sprintf(caTemp, "%s%s", caAdPlate, caPlateid);
            if(!utFileIsExist(caTemp))
            {
                char caComm[128];
                sprintf(caComm, "mkdir -p %s", caTemp);
                system(caComm);
            }
        }


    }

    lShopplateid = utMd5Code(caPlateid, strlen(caPlateid), "ffff");
    if(lShopplateid > 2000000000) lShopplateid = lShopplateid - 2000000000;
    if(lShopplateid > 2000000000) lShopplateid = lShopplateid - 2000000000;




    if(strlen(caPlatename) > 0)
    {
        pasCvtGBK(2, caPlatename, caTemp, 63);
        strcpy(caPlatename, caTemp);


    }

    if(strlen(caMark) > 0)
    {
        pasCvtGBK(2, caMark, caTemp, 1000);
        strcpy(caMark, caTemp);

        if(strstr(caMark, "多个"))
        {
            strcpy(caMark, "");
        }

    }


    if(strlen(caSsid) > 0)
    {
        pasCvtGBK(2, caSsid, caTemp, 1000);
        strcpy(caSsid, caTemp);
        if(strstr(caSsid, "多个"))
        {
            strcpy(caSsid, "");
        }

    }
    if(strlen(caAp) > 0)
    {
        pasCvtGBK(2, caAp, caTemp, 1000);
        strcpy(caAp, caTemp);
        if(strstr(caAp, "多个"))
        {
            strcpy(caAp, "");
        }
    }

    int sqlflag = 0;
    sprintf(caTemp, "select count(*) from portalplate where plateid='%s' ", caPlateid);
    lCount = 0;
    pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lCount);
    uint8 lltime = time(0);
    if(lCount > 0)
    {
        printf("update portalplate set platename='%s',modtime=%llu,shopplateid=%lu where plateid='%s' \n",
               caPlatename, lltime, lShopplateid, caPlateid);

        iReturn = pasDbExecSqlF("update portalplate set platename='%s',modtime=%llu,shopplateid=%lu where plateid='%s' \n",
                                caPlatename, lltime, lShopplateid, caPlateid);


    }
    else
    {

        printf("insert into portalplate(plateid,platename,addtime,modtime,shopplateid) \
      	   values('%s','%s',%llu,%llu,%lu) \n", \
               caPlateid, caPlatename, lltime, lltime, lShopplateid);

        iReturn = pasDbExecSqlF("insert into portalplate(plateid,platename,addtime,modtime,shopplateid) \
      	   values('%s','%s',%llu,%llu,%lu) ", \
                                caPlateid, caPlatename, lltime, lltime, lShopplateid);
    }
    sqlflag = 0;
    if(iReturn == 0)
    {
        long lPriority = 0;
        long lStatus = 0;
        if(strlen(caPriority) > 0)
        {
            lPriority = atol(caPriority);
            lStatus = atol(caStatus);
        }
        memset(caTemp, 0, sizeof(caTemp));
        sprintf(caTemp, "select count(*) from portalprule where plateid='%s' ", caPlateid);
        lCount = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount > 0)
        {
            memset(caTemp, 0, sizeof(caTemp));
            snprintf(caTemp, sizeof(caTemp) - 1, "update portalprule set modtime=%llu, status=%lu, priority=%lu where plateid='%s'", lltime, lStatus, lPriority, caPlateid);

        }
        else
        {

            ulong lMaxid = 0;
            pasDbOneRecord("select max(ruleid) from portalprule", 0, UT_TYPE_ULONG, 4, &lMaxid);
            memset(caTemp, 0, sizeof(caTemp));
            snprintf(caTemp, sizeof(caTemp) - 1, "insert into portalprule(ruleid,priority,plateid,addtime,modtime,status) values(%lu,%lu,'%s',%llu,%llu,%lu) ", (lMaxid + 1), lPriority, caPlateid, lltime, lltime, lStatus);
        }
        printf("sql=[%s]\n", caTemp);
        iReturn = pasDbExecSqlF(caTemp);
        if(iReturn != 0)
        {
            sqlflag = 1;
        }

    }
    else
    {
        sqlflag = 1;
    }
    if(sqlflag == 0)
    {
        utPltPutVar(psDbHead, "titel", "true");
        utPltPutVarF(psDbHead, "sid", "%s", caPlateid);
        utPltPutVar(psDbHead, "platepath", caPlateid);
        utPltPutVar(psDbHead, "message", "保存成功");
    }
    else
    {
        utPltPutVar(psDbHead, "titel", "false");
        utPltPutVarF(psDbHead, "sid", "%s", caPlateid);
        utPltPutVar(psDbHead, "message", "保存失败");
    }


    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_adfile.htm");
    return 0;
}


int getSrcByAdindex(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    int iReturn = 0;
    utMsgPrintMsg(psMsgHead);
    char caAdIndex[64] = "";
    char caPlateid[64] = "";
    char caUrl[256] = "";
    char sqlbuf[1024] = "";
    ulong lCount = 0;
    char caMsg[256] = "";

    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "plateid",   UT_TYPE_STRING, sizeof(caPlateid) - 1, caPlateid,
                               "adindex",   UT_TYPE_STRING, sizeof(caAdIndex) - 1, caAdIndex);

    utStrDelSpaces(caAdIndex);
    utPltDbHead* psDbHead = utPltInitDb();
    if(strlen(caAdIndex) > 0)
    {
        memset(sqlbuf, 0, sizeof(sqlbuf));
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select defad from portalplateindex where plateid='%s' and plateindex='%s'", caPlateid, caAdIndex);
        printf("sql=%s\n", sqlbuf);
        pasDbOneRecord(sqlbuf, 0, UT_TYPE_STRING, sizeof(caUrl) - 1, caUrl);
        if(strlen(caUrl) <= 0)
        {
            snprintf(caMsg, sizeof(caMsg) - 1, "用户名不存在或密码错误");
            utPltPutVar(psDbHead, "mesg", (char*)convert("GBK", "UTF-8", caMsg));
            utPltPutVar(psDbHead, "result", "no");
        }
        else
        {
            utPltPutVar(psDbHead, "result", caUrl);
        }
    }
    else
    {
        snprintf(caMsg, sizeof(caMsg) - 1, "adindex不可以为空");
        utPltPutVar(psDbHead, "mesg", (char*)convert("GBK", "UTF-8", caMsg));
        utPltPutVarF(psDbHead, "result", "%d", 2);
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/login/login.htm");
    return 0;
}


int ictGetUserLoginInfo(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    uint8 lTsid;
    uint4 lBase;
    int iReturn = 0;
    char caTsid[128] = "";
    char caMsg[256] = "";
    ncPortalOnline *psOnline;
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "PASTSID",   UT_TYPE_STRING, sizeof(caTsid) - 1, caTsid);
    utPltDbHead* psDbHead = utPltInitDb();
    if(utStrIsDigital(caTsid))
    {
        printf(" get User Tid=%s \n", caTsid);
        sscanf(caTsid, "%llu", &lTsid);
        lBase = utShmGetBaseAddr(psShmHead);
        psOnline = (ncPortalOnline *)ncSrvGetOnlineUserByTsid(psShmHead, lTsid);
        if(psOnline)
        {
            snprintf(caMsg, sizeof(caMsg) - 1, "成功");
            utPltPutVar(psDbHead, "mesg", (char*)convert("GBK", "UTF-8", caMsg));
            utPltPutVarF(psDbHead, "result", "%d", 0);
            utPltPutVarF(psDbHead, "tsid", "%llu", lTsid);

            printf("mac=[%s]\n", pasCvtMac(psOnline->caMac));
            printf("os=[%s]\n", psOnline->caOs);
            //printf("apmac=[%s]\n", psOnline->psAp->mac);
            printf("place=[%s]\n", psOnline->caGroupCode);
            printf("phone=[%s]\n", psOnline->caName);
            utPltPutVar(psDbHead, "mac", pasCvtMac(psOnline->caMac));
            utPltPutVar(psDbHead, "os", psOnline->caOs);
            //utPltPutVar(psDbHead, "apmac",  psOnline->psAp->mac);
            memset(caMsg, 0, sizeof(caMsg));
            snprintf(caMsg, sizeof(caMsg) - 1, "酒店宾馆");
            utPltPutVar(psDbHead, "place", (char*)convert("GBK", "UTF-8", caMsg));
            //utPltPutVar(psDbHead, "place", psOnline->caGroupCode);
            utPltPutVar(psDbHead, "phone", psOnline->caName);

        }
        else
        {
            printf(" Tid %llu not exist\n", lTsid);
            snprintf(caMsg, sizeof(caMsg) - 1, "tsid不存在");
            utPltPutVar(psDbHead, "mesg", (char*)convert("GBK", "UTF-8", caMsg));
            utPltPutVarF(psDbHead, "result", "%d", 2);
        }

    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "yunlian/info.htm");
    return 0;
}

int ictFirstDispAd(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    int iReturn = 0;
    char caTsid[128] = "";
    char caMsg[256] = "";
    char caPlateid[64] = "";
    char caPlateindex[64] = "";
    char caUrl[256] = "";
    char sqlbuf[1024] = "";
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 3,
                               "plateindex", UT_TYPE_STRING, sizeof(caPlateindex) - 1, caPlateindex,
                               "plateid", UT_TYPE_STRING, sizeof(caPlateid) - 1, caPlateid,
                               "tsid",   UT_TYPE_STRING, sizeof(caTsid) - 1, caTsid);
    utPltDbHead* psDbHead = utPltInitDb();
    if(utStrIsDigital(caTsid))
    {
        utStrDelSpaces(caPlateid);

        memset(sqlbuf, 0, sizeof(sqlbuf));
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select defad from portalplateindex where plateid='%s' and plateindex='%s'", caPlateid, caPlateindex);
        printf("sql=%s\n", sqlbuf);
        pasDbOneRecord(sqlbuf, 0, UT_TYPE_STRING, sizeof(caUrl) - 1, caUrl);
        snprintf(caMsg, sizeof(caMsg) - 1, "成功");
        utPltPutVar(psDbHead, "mesg", (char*)convert("GBK", "UTF-8", caMsg));
        utPltPutVarF(psDbHead, "result", "%d", 0);
    }

    utPltPutVar(psDbHead, "path", caUrl);
    //utPltPutVar(psDbHead, "path", "/adplate/ictportal5/images/advert_1_1449567566_4.jpg");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "yunlian/imgPath.htm");
    return 0;
}
int ictGetLoginTsid(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    int iReturn = 0;
    char caTsid[32] = "";
    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "PASTSID", UT_TYPE_STRING, sizeof(caTsid) - 1 , caTsid
                              );
    return 0;
}




char* getNonceStr()
{
    int i = 0;
    static char sstr[36] = "";
    /*
    memset(sstr, 0, sizeof(sstr));
    srand((int)time(0));
    for(i = 0; i < 32; i++)
    {
        switch(rand() % 2)
        {
            case 0:
                sstr[i] = 'A' + rand() % 26;
                break;
            case 1:
                sstr[i] = '0' + rand() % 10;
                break;
                //case 2:
                //sstr[i]='a'+rand()%26;
                //break;
        }

    }*/
    //gen_str(sstr, 32);
    strcpy(sstr, "5K8264ILTKCH16CQ2502SI8ZNMTM67VS");
    return sstr;
}



/**
* 接收微信的支付结果
*
<xml>
    <appid><![CDATA[wx709aadbe9b24f033]]></appid>
    <bank_type><![CDATA[CMBC_CREDIT]]></bank_type>
    <cash_fee><![CDATA[1]]></cash_fee>
    <fee_type><![CDATA[CNY]]></fee_type>
    <is_subscribe><![CDATA[N]]></is_subscribe>
    <mch_id><![CDATA[1283145801]]></mch_id>
    <nonce_str><![CDATA[0fc170ecbb8ff1afb2c6de48ea5343e7]]></nonce_str>
    <openid><![CDATA[odUXywFCZQpDW5OlxmWyA96pIJ04]]></openid>
    <out_trade_no><![CDATA[1451531552100000012]]></out_trade_no>
    <result_code><![CDATA[SUCCESS]]></result_code>
    <return_code><![CDATA[SUCCESS]]></return_code>
    <sign><![CDATA[8DE5F4ADAF7A46F1D5B50C511D43A881]]></sign>
    <time_end><![CDATA[20151231111303]]></time_end>
    <total_fee>1</total_fee>
    <trade_type><![CDATA[APP]]></trade_type>
    <transaction_id><![CDATA[1007120452201512312415960375]]></transaction_id>
</xml>
*/

int receiveWeixinNotify(utShmHead* psShmHead, int iFd, utMsgHead* psMsgHead)
{
    int iReturn = 0;
    char caOut_trade_no[32] = "";
    char caOutXml[2048] = "";
    char caReturn[16] = "";
    pasLogs(1066, 1066, "\n\n===================接收到微信的反馈===========================\n\n");
    utMsgOutMsgToLog(1066, 1066, psMsgHead, "[recieve weixin] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "PASXml", UT_TYPE_STRING, sizeof(caOutXml) - 1 , caOutXml);
    getNodeValue(caOutXml, "<return_code>", "</return_code>", caReturn, 16);
    pasLogs(1066, 1066, "return code=%s\n", caReturn);
    if(strcmp(caReturn, "SUCCESS") == 0)
    {

    }
    utPltDbHead* psDbHead = utPltInitDb();
    utPltPutVar(psDbHead, "ret_flag", "success");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/pay/wechat.htm");
    return 0;
}

//展示的页面
int showAlipayRet(utShmHead* psShmHead, int iFd, utMsgHead* psMsgHead)
{
    int iReturn = 0;
    char caOut_trade_no[32] = "";
    char caOutXml[2048] = "";
    char caReturn[16] = "";
    pasLogs(1066, 1066, "\n\n===================显示支付宝的return_url===========================\n\n");
    utMsgOutMsgToLog(1066, 1066, psMsgHead, "[show  return url] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "requri", UT_TYPE_STRING, sizeof(caOutXml) - 1 , caOutXml);
    ncUtlGetWordBetween(caOutXml, "body=", "&buyer_email=", caReturn, 128);

    pasLogs(1066, 1066, "return code=%s\n", caReturn);

    utPltDbHead* psDbHead = utPltInitDb();
    utPltPutVar(psDbHead, "body", caReturn);
    utPltPutVar(psDbHead, "tradeno", "20150000000");
    utPltPutVar(psDbHead, "subject", convert("GBK", "UTF-8", "充值"));
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/pay/alipayRet.htm");
    return 0;
}

int showWechatRet(utShmHead* psShmHead, int iFd, utMsgHead* psMsgHead)
{
    int iReturn = 0;
    char caOut_trade_no[32] = "";
    char caOutXml[2048] = "";
    char caReturn[16] = "";
    pasLogs(1066, 1066, "\n\n===================显示微信的return_url===========================\n\n");
    utMsgOutMsgToLog(1066, 1066, psMsgHead, "[show weixin return url] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "PASXml", UT_TYPE_STRING, sizeof(caOutXml) - 1 , caOutXml);

    pasLogs(1066, 1066, "return code=%s\n", caReturn);
    if(strcmp(caReturn, "SUCCESS") == 0)
    {

    }
    utPltDbHead* psDbHead = utPltInitDb();
    utPltPutVar(psDbHead, "ret_flag", "return_url wechat");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/pay/wechatRet.htm");
    return 0;
}



/**
* 接收支付宝的支付结果
*
*/
int receiveAlipayNotify(utShmHead* psShmHead, int iFd, utMsgHead* psMsgHead)
{
    int iReturn = 0;
    char caOut_trade_no[32] = "";
    char caOutXml[2048] = "";
    char caBody[16] = {0};
    char caReturn[16] = "";
    pasLogs(1066, 1066, "\n\n===================接收到支付宝的反馈===========================\n\n");
    utMsgOutMsgToLog(1066, 1066, psMsgHead, "[recieve alipay] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "body", UT_TYPE_STRING, sizeof(caBody) - 1 , caBody,
                               "out_trade_no", UT_TYPE_STRING, sizeof(caOut_trade_no) - 1 , caOut_trade_no);
    pasLogs(1066, 1066, "caBody =%s", caBody);
    //处理数据库的数据更新
    if(strcmp(caBody, "order") == 0) //订购套餐
    {
        pasLogs(1066, 1066, "===caBody =%s", caBody);
        ict_complete_package_order_zf(psShmHead, iFd, psMsgHead, caOut_trade_no);
    }
    else if(strcmp(caBody, "charge") == 0) //订购套餐
    {
        ict_complete_recharge(psShmHead, iFd, psMsgHead, caOut_trade_no);
    }
    utPltDbHead* psDbHead = utPltInitDb();
    utPltPutVar(psDbHead, "ret_flag", "success");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/pay/alipay.htm");
    return 0;
}


/**
* 返回支付宝支付首页
*/
int ictAlipayIndex(utShmHead* psShmHead, int iFd, utMsgHead* psMsgHead)
{
    int iReturn = 0;
    char caTsid[64] = "";
    utMsgOutMsgToLog(1066, 1067, psMsgHead, "[alipay index] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "pastsid", UT_TYPE_STRING, sizeof(caTsid) - 1 , caTsid);
    utPltDbHead* psDbHead = utPltInitDb();
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/pay/alipayIndex.htm");
    return 0;
}

/**
   * 建立请求，以表单HTML形式构造，带文件上传功能
   * @param sParaTemp 请求参数数组
   * @param strMethod 提交方式。两个值可选：post、get
   * @param strButtonName 确认按钮显示文字
   * @param strParaFileName 文件上传的参数名
   * @return 提交表单HTML文本
   */
/***
    * 添加其他参数
    * //支付类型
        String payment_type = "1";
        //必填，不能修改
        //服务器异步通知页面路径
        String notify_url = "http://商户网关地址/create_direct_pay_by_user-JAVA-UTF-8/notify_url.jsp";
        //需http://格式的完整路径，不能加?id=123这类自定义参数

        //页面跳转同步通知页面路径
        String return_url = "http://商户网关地址/create_direct_pay_by_user-JAVA-UTF-8/return_url.jsp";
        //需http://格式的完整路径，不能加?id=123这类自定义参数，不能写成http://localhost/

        //商户订单号
        String out_trade_no = new String(request.getParameter("WIDout_trade_no").getBytes("ISO-8859-1"),"UTF-8");
        //商户网站订单系统中唯一订单号，必填

        //订单名称
        String subject = new String(request.getParameter("WIDsubject").getBytes("ISO-8859-1"),"UTF-8");
        //必填

        //付款金额
        String total_fee = new String(request.getParameter("WIDtotal_fee").getBytes("ISO-8859-1"),"UTF-8");
        //必填

        //订单描述

        String body = new String(request.getParameter("WIDbody").getBytes("ISO-8859-1"),"UTF-8");
        //商品展示地址
        String show_url = new String(request.getParameter("WIDshow_url").getBytes("ISO-8859-1"),"UTF-8");
        //需以http://开头的完整路径，例如：http://www.商户网址.com/myorder.html

        //防钓鱼时间戳
        String anti_phishing_key = "";
        //若要使用请调用类文件submit中的query_timestamp函数

        //客户端的IP地址
        String exter_invoke_ip = "";
        //非局域网的外网IP地址，如：221.0.0.1


        //////////////////////////////////////////////////////////////////////////////////

        //把请求参数打包成数组
        Map<String, String> sParaTemp = new HashMap<String, String>();
        sParaTemp.put("service", "create_direct_pay_by_user");
        sParaTemp.put("partner", AlipayConfig.partner);
        sParaTemp.put("seller_email", AlipayConfig.seller_email);
        sParaTemp.put("_input_charset", AlipayConfig.input_charset);
        sParaTemp.put("payment_type", payment_type);
        sParaTemp.put("notify_url", notify_url);
        sParaTemp.put("return_url", return_url);
        sParaTemp.put("out_trade_no", out_trade_no);
        sParaTemp.put("subject", subject);
        sParaTemp.put("total_fee", total_fee);
        sParaTemp.put("body", body);
        sParaTemp.put("show_url", show_url);
        sParaTemp.put("anti_phishing_key", anti_phishing_key);
        sParaTemp.put("exter_invoke_ip", exter_invoke_ip);
    ***/
/**
*   String mysign = "";
    if(AlipayConfig.sign_type.equals("MD5") ) {
        mysign = MD5.sign(prestr, AlipayConfig.key, AlipayConfig.input_charset);
    }
    return mysign;
*/

#define ALIPAY_GATEWAY_NEW "https://mapi.alipay.com/gateway.do?"

int convertToSmall(char* caValue)
{
    int i = 0;
    int gap = 'a' - 'A';
    for(i = 0; i < strlen(caValue); i++)
    {
        if((caValue[i] >= 'A') && (caValue[i] <= 'Z'))
        {
            caValue[i] = caValue[i] + gap;
        }
    }
    return 0;
}

char* alipayBuildRequest(ictAlipayConfig* pAlipayConfig, char* strMethod, char* strButtonName)
{
    //待请求参数数组
    static char caHtml[8024] = "";
    char caPrestr[8024] = "";
    memset(caHtml, 0, sizeof(caHtml));
    snprintf(caHtml, sizeof(caHtml) - 1, "<form id=\"alipaysubmit\" name=\"alipaysubmit\"  enctype=\"multipart/form-data\" action=\"%s", ALIPAY_GATEWAY_NEW);
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "_input_charset=%s", pAlipayConfig->input_charset);
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "\" method=\"%s\">", strMethod);
    //_
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"_input_charset\" value=\"%s\"/>", pAlipayConfig->input_charset);
    snprintf(caPrestr + strlen(caPrestr), sizeof(caPrestr) - 1, "_input_charset=%s&", pAlipayConfig->input_charset);
    //b
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"body\" value=\"%s\"/>", pAlipayConfig->body);
    snprintf(caPrestr + strlen(caPrestr), sizeof(caPrestr) - 1, "body=%s&", pAlipayConfig->body);
    //n
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"notify_url\" value=\"%s\"/>", pAlipayConfig->notify_url);
    snprintf(caPrestr + strlen(caPrestr), sizeof(caPrestr) - 1, "notify_url=%s&", pAlipayConfig->notify_url);
    //o
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"out_trade_no\" value=\"%s\"/>", pAlipayConfig->out_trade_no);
    snprintf(caPrestr + strlen(caPrestr), sizeof(caPrestr) - 1, "out_trade_no=%s&", pAlipayConfig->out_trade_no);
    //p-a-r
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"partner\" value=\"%s\"/>", pAlipayConfig->partner);
    snprintf(caPrestr + strlen(caPrestr), sizeof(caPrestr) - 1, "partner=%s&", pAlipayConfig->partner);
    //p-a-y
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"payment_type\" value=\"%lu\"/>", pAlipayConfig->payment_type);
    snprintf(caPrestr + strlen(caPrestr), sizeof(caPrestr) - 1, "payment_type=%lu&", pAlipayConfig->payment_type);
    //r
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"return_url\" value=\"%s\"/>", pAlipayConfig->return_url);
    snprintf(caPrestr + strlen(caPrestr), sizeof(caPrestr) - 1, "return_url=%s&", pAlipayConfig->return_url);
    //s-e-l
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"seller_email\" value=\"%s\"/>", pAlipayConfig->seller_email);
    snprintf(caPrestr + strlen(caPrestr), sizeof(caPrestr) - 1, "seller_email=%s&", pAlipayConfig->seller_email);
    //s-e-r
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"service\" value=\"create_direct_pay_by_user\"/>");
    snprintf(caPrestr + strlen(caPrestr), sizeof(caPrestr) - 1, "service=create_direct_pay_by_user&");
    //s-h
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"show_url\" value=\"%s\"/>", pAlipayConfig->show_url);
    snprintf(caPrestr + strlen(caPrestr), sizeof(caPrestr) - 1, "show_url=%s&", pAlipayConfig->show_url);
    //s-u
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"subject\" value=\"%s\"/>", pAlipayConfig->subject);
    snprintf(caPrestr + strlen(caPrestr), sizeof(caPrestr) - 1, "subject=%s&", pAlipayConfig->subject);
    //t
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"total_fee\" value=\"%.2f\"/>", pAlipayConfig->total_fee);
    snprintf(caPrestr + strlen(caPrestr), sizeof(caPrestr) - 1, "total_fee=%.2f", pAlipayConfig->total_fee);
    /*
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"anti_phishing_key\" value=\"%llu\"/>", pAlipayConfig->anti_phishing_key);
    snprintf(caPrestr + strlen(caPrestr), sizeof(caPrestr) - 1, "anti_phishing_key=%llu&", pAlipayConfig->anti_phishing_key);
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"exter_invoke_ip\" value=\"%s\"/>", pAlipayConfig->exter_invoke_ip);
    snprintf(caPrestr + strlen(caPrestr), sizeof(caPrestr) - 1, "exter_invoke_ip=%s", pAlipayConfig->exter_invoke_ip);
    */
    /*
    utMd5_Code(unsigned char  *input,unsigned int inlen,unsigned char *pKey,
        unsigned char  *output)
    */
    char caMySign[64] = "";
    char caSignTmp[128] = "";
    pasLogs(1066, 1067, "caPrestr=[%s]", caPrestr);
    pasLogs(1066, 1067, "md5key=[%s]", pAlipayConfig->md5_key);
    snprintf(caPrestr + strlen(caPrestr), sizeof(caPrestr) - 1, "%s", pAlipayConfig->md5_key);
    pasLogs(1066, 1067, "after append key caPrestr=[%s]", caPrestr);

    utMd5(caPrestr, strlen(caPrestr), caSignTmp);
    //utMd5_Code(caPrestr, strlen(caPrestr), pAlipayConfig->md5_key, caSignTmp);
    pasStrCvtBin2Hex(caSignTmp, 16, caMySign);
    convertToSmall(caMySign);
    pasLogs(1066, 1067, "mysign=[%s]", caMySign);
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"sign\" value=\"%s\"/>", caMySign);
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"sign_type\" value=\"%s\"/>", pAlipayConfig->sign_type);
    //submit按钮控件请不要含有name属性
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"submit\" value=\"%s\" style=\"display:none;\"></form>", strButtonName);
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<script>document.forms['alipaysubmit'].submit();</script>");
    pasLogs(1066, 1067, caHtml);
    pasLogs(1066, 1067, caHtml + 1020);
    return caHtml;
}


/**
* 支付宝支付接口
* WIDout_trade_no
* WIDsubject
* WIDtotal_fee
* WIDbody
* WIDshow_url
*/
/*
    // 合作身份者ID，以2088开头由16位纯数字组成的字符串
    public static String partner = "";
    // 收款支付宝账号，一般情况下收款账号就是签约账号
    public static String seller_email = "";
    // 商户的私钥
    public static String key = "";
*/
int ictAlipayapi(utShmHead* psShmHead, int iFd, utMsgHead* psMsgHead)
{
    int iReturn = 0;
    char caTsid[64] = "";
    char caTotal_fee[16] = "";
    char caSubject[256] = "";
    /*
    char caNotify_url[256] = "";
    ulong lPayment_type = 1;
    char caReturn_url[256] = "";
    char caOut_trade_no[128] = "";


    char caBody[256] = "";
    char caShow_url[256] = "";
    char caExter_invoke_ip[64] = "";
    uint8 lAnti_phishing_key = 0;
    */
    ictAlipayConfig alipayConfig;
    memset(&alipayConfig, 0, sizeof(ictAlipayConfig));
    utMsgOutMsgToLog(1066, 1067, psMsgHead, "[alipay finish] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead, 7,
                               "WIDout_trade_no", UT_TYPE_STRING, sizeof(alipayConfig.out_trade_no) - 1, alipayConfig.out_trade_no,
                               "WIDsubject", UT_TYPE_STRING, sizeof(caSubject) - 1, caSubject,
                               "WIDtotal_fee", UT_TYPE_STRING, sizeof(caTotal_fee) - 1, caTotal_fee,
                               "WIDbody", UT_TYPE_STRING, sizeof(alipayConfig.body) - 1, alipayConfig.body,
                               "WIDshow_url", UT_TYPE_STRING, sizeof(alipayConfig.show_url) - 1, alipayConfig.show_url,
                               "clientip", UT_TYPE_STRING, sizeof(alipayConfig.exter_invoke_ip) - 1, alipayConfig.exter_invoke_ip,
                               "pastsid", UT_TYPE_STRING, sizeof(caTsid) - 1 , caTsid);
    if(strlen(caTotal_fee) > 0)
    {
        alipayConfig.total_fee = atof(caTotal_fee);
    }

    strcpy(alipayConfig.notify_url, utComGetVar_sd(psShmHead, "aliNotify_url", "http://180.169.1.201:8006/ictpay/notifyAlipayResult"));
    strcpy(alipayConfig.return_url, utComGetVar_sd(psShmHead, "aliRetUrl", "http://180.169.1.201:8006/ictpay/alipayRetUrl"));
    strcpy(alipayConfig.show_url, utComGetVar_sd(psShmHead, "aliShowUrl", "http://180.169.1.201:8006/ictpay/notifyAlipayResult"));
    alipayConfig.payment_type = utComGetVar_ld(psShmHead, "aliPayType", 1);
    strcpy(alipayConfig.input_charset, "utf-8");
    strcpy(alipayConfig.partner, "2088411881747090");
    strcpy(alipayConfig.seller_email, "liqin@pronetway.com");
    strcpy(alipayConfig.md5_key, "5yqgnewnsj0m08t2l0rt0z6gje0dk18c");
    strcpy(alipayConfig.subject, caSubject);
    /*
        snprintf(alipayConfig.rsa_key + strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key) - 1, "MIICXAIBAAKBgQC41wBqh3c5z+AH1nx4edgiqTFf8Jzre37K6NdjFAt9OI6mHGpL");
        snprintf(alipayConfig.rsa_key + strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key) - 1, "BzQYGysS7se2pmn7ni7rJC4cLG4fy72ATmDSY9itfQYCJp0Vm2NYJOPh6/a4pllc");
        snprintf(alipayConfig.rsa_key + strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key) - 1, "rmsrVQs0ZuDMnZ17LY/iJv9ulA2AsNEv98AnjcoN4peAhnuaKEUSCGjJYQIDAQAB");
        snprintf(alipayConfig.rsa_key + strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key) - 1, "AoGAWjaFO3kWVZ8BZpHmeh5twKz3Li4PSFcj0QDHbtHvsZQS+zvPHQNNB57q7Pih");
        snprintf(alipayConfig.rsa_key + strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key) - 1, "fKHIBEh1+cnUv2dn8BjOSnr2gaRQEmBGQDNlmXyBGjl6wscIWD5x1TKpOgZwj7x7");
        snprintf(alipayConfig.rsa_key + strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key) - 1, "bfH2C/w/8aDFy1DdDYuW5nVUQCe8UUtFhko62V5OokBuS0kCQQDf9nLTOmYMjsnw");
        snprintf(alipayConfig.rsa_key + strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key) - 1, "6wDJm04bx1c5FZC6LkMM+QhF3UtL7y665gaerwaob57t1CApve+ADdeUi8Nj2ueo");
        snprintf(alipayConfig.rsa_key + strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key) - 1, "9vjRIKUPAkEA00fePG40gdzkEVemK59t6Qj0G9IR3mKM4luYWA1tEzi6RQMY+kXG");
        snprintf(alipayConfig.rsa_key + strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key) - 1, "zfwcoVqvsmQS6uJZPEWC98B5cGcMh6YKjwJAavhFI8pyz3Cr11wHRkPdbLsrV+KK");
        snprintf(alipayConfig.rsa_key + strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key) - 1, "+8H42Ithfi0SN7SH91DCvKwQXAbkO+fjbMyBRgJWg2a17k2x9Cvx78DN1QJBALb4");
        snprintf(alipayConfig.rsa_key + strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key) - 1, "DyKAQeKQm8XVBOfUOofMJDKPmr/e0qkYr3bZsbMqtk0boJ7knIU4JscpVOyFM8jL");
        snprintf(alipayConfig.rsa_key + strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key) - 1, "2oGSpRfPTJul8jjq90sCQCEPzEEhRQ+aeu39LY5FHQ8OJcv4bTm4+7X7HDlEXCRq");
        snprintf(alipayConfig.rsa_key + strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key) - 1, "3SByxXfyTepG9DvBFB+ttSD7fx8bAaRWwWYIAFPv490=");
    */
    /**
        "MIICXAIBAAKBgQC41wBqh3c5z+AH1nx4edgiqTFf8Jzre37K6NdjFAt9OI6mHGpL"
            + "BzQYGysS7se2pmn7ni7rJC4cLG4fy72ATmDSY9itfQYCJp0Vm2NYJOPh6/a4pllc"
            + "rmsrVQs0ZuDMnZ17LY/iJv9ulA2AsNEv98AnjcoN4peAhnuaKEUSCGjJYQIDAQAB"
            + "AoGAWjaFO3kWVZ8BZpHmeh5twKz3Li4PSFcj0QDHbtHvsZQS+zvPHQNNB57q7Pih"
            + "fKHIBEh1+cnUv2dn8BjOSnr2gaRQEmBGQDNlmXyBGjl6wscIWD5x1TKpOgZwj7x7"
            + "bfH2C/w/8aDFy1DdDYuW5nVUQCe8UUtFhko62V5OokBuS0kCQQDf9nLTOmYMjsnw"
            + "6wDJm04bx1c5FZC6LkMM+QhF3UtL7y665gaerwaob57t1CApve+ADdeUi8Nj2ueo"
            + "9vjRIKUPAkEA00fePG40gdzkEVemK59t6Qj0G9IR3mKM4luYWA1tEzi6RQMY+kXG"
            + "zfwcoVqvsmQS6uJZPEWC98B5cGcMh6YKjwJAavhFI8pyz3Cr11wHRkPdbLsrV+KK"
            + "+8H42Ithfi0SN7SH91DCvKwQXAbkO+fjbMyBRgJWg2a17k2x9Cvx78DN1QJBALb4"
            + "DyKAQeKQm8XVBOfUOofMJDKPmr/e0qkYr3bZsbMqtk0boJ7knIU4JscpVOyFM8jL"
            + "2oGSpRfPTJul8jjq90sCQCEPzEEhRQ+aeu39LY5FHQ8OJcv4bTm4+7X7HDlEXCRq"
            + "3SByxXfyTepG9DvBFB+ttSD7fx8bAaRWwWYIAFPv490=";
    */
    strcpy(alipayConfig.sign_type, "MD5");
    //strcpy(alipayConfig.sign_type, "RSA");
    alipayConfig.anti_phishing_key = time(0);

    utPltDbHead* psDbHead = utPltInitDb();
    utPltPutVar(psDbHead, "submitForm",  alipayBuildRequest(&alipayConfig, "get", "confirm"));
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/pay/alipayFinish.htm");
    return 0;
}

char *getHttpsContEx(char argv[512])
{
    int sockfd, ret;
    char buffer[1024];
    struct sockaddr_in server_addr;
    struct hostent *host;
    int portnumber, nbytes;
    char host_addr[256];
    char host_file[1024];
    char local_file[256];
    //    FILE *fp;
    char caTemp[1024];
    char request[1024];
    int send, totalsend;
    int i;
    char *pt;
    SSL *ssl;
    SSL_CTX *ctx;

    char tmpch[4] = "";
    static char pCont[10024] = "";

    //    strcpy(argv,"https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=wx086f838720eb4d73&secret=1ba4edc4bc8ebff3541961fc0785c8f8");
    GetHost(argv, host_addr, host_file, &portnumber);        /*分析网址、端口、文件名等 */


    if((host = gethostbyname(host_addr)) == NULL)           /*取得主机IP地址 */
    {

        return NULL;
    }

    /* 客户程序开始建立 sockfd描述符 */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)           /*建立SOCKET连接 */
    {
        return NULL;
    }

    /* 客户程序填充服务端的资料 */
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portnumber);
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);

    /* 客户程序发起连接请求 */
    if(connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)            /*连接网站 */
    {
        return NULL;
    }

    /* SSL初始化 */
    SSL_library_init();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if(ctx == NULL)
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    ssl = SSL_new(ctx);
    if(ssl == NULL)
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    /* 把socket和SSL关联 */
    ret = SSL_set_fd(ssl, sockfd);
    if(ret == 0)
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    RAND_poll();
    while(RAND_status() == 0)
    {
        unsigned short rand_ret = rand() % 65536;
        RAND_seed(&rand_ret, sizeof(rand_ret));
    }

    ret = SSL_connect(ssl);
    if(ret != 1)
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    sprintf(request, "GET /%s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\n\
User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n\
Host: %s:%d\r\nConnection: Close\r\n\r\n", host_file, host_addr,
            portnumber);


    pasLogs(1066, 1066, "before request=[%s]", request);
    /*发送https请求request */
    send = 0;
    totalsend = 0;
    nbytes = strlen(request);
    while(totalsend < nbytes)
    {
        send = SSL_write(ssl, request + totalsend, nbytes - totalsend);
        if(send == -1)
        {
            return NULL;
        }
        totalsend += send;

    }


    i = 0;
    pasLogs(1066, 1066, "after request=[%s]", request);
    /* 连接成功了，接收https响应，response */
    while((nbytes = SSL_read(ssl, buffer, 1)) == 1)
    {
        if(i < 4)
        {
            if(buffer[0] == '\r' || buffer[0] == '\n')
                i++;
            else
                i = 0;

        }
        else
        {

            sprintf(tmpch, "%c", buffer[0]);
            if(i == 4)
            {
                strcpy(pCont, tmpch);
            }
            else
            {
                strcat(pCont, tmpch);
            }
            i++;
            //            if (i % 1024 == 0)
            //                fflush(fp);        /*每1K时存盘一次 */
        }
    }
    pasLogs(1066, 1066, "pCont=[%s]", pCont);

    //    printf("\npCont = %s \n",pCont);
    //    fclose(fp);
    /* 结束通讯 */
    ret = SSL_shutdown(ssl);
    if(ret != 1)
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    close(sockfd);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    ERR_free_strings();
    //    exit(0);
    return pasCvtGBK(2, (char*)pCont, caTemp, 1000);
}


char *getWeiXinAccessToken(char *pUrl)
{
    char caReturn[10024];
    char caUrl[1024];
    static char caToken[640];
    cJSON* pRoot;
    cJSON * pSubJson = NULL;
    memset(caToken, 0, sizeof(caToken));

    //  strcpy(caUrl,"https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=wx086f838720eb4d73&secret=208e00ea7441ef2f58eae8ba4495c2a7");

    strcpy(caUrl, "https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=wx43dfa58334f0ff7e&secret=726532587df2b5fadcb364a4e0b64bf9");

    strcpy(caReturn, getHttpsContEx(caUrl));
    if(strstr(caReturn, "access_token"))
    {
        pRoot = cJSON_Parse(caReturn);
        if(pRoot)
        {
            strcpy(caToken, cJSON_GetObjectItem(pRoot, "access_token") ? cJSON_GetObjectItem(pRoot, "access_token")->valuestring : "\0");
        }
    }
    return caToken;
}


/**
* 请求微信支付预支付订单
* https://api.mch.weixin.qq.com/pay/unifiedorder
*/
int createWeixinPreOrder(utShmHead* psShmHead, int iFd, utMsgHead* psMsgHead)
{
    long lStatus = 0;
    int  iReturn;
    char caMesg[32];
    char caHost[256] = "", caHtml[2024] = "", caXml[2048] = "", caReturn[16] = "";
    char caAppId[32] = "";
    char caMch_id[32] = "";
    char caDevice_info[32] = "";
    char caNonce_str[36] = "";
    char caSign[32] = "";
    char caBody[32] = "";
    long iTotal_fee = 0;
    char caTotal_fee[16] = "";
    char caSpbill_create_ip[16] = "";
    char caNotify_url[256] = "";
    char caOut_trade_no[32] = "";
    char caRetMsg[128] = "";
    char caPrepay_id[64] = "";
    char stringA[2048] = "";
    char caKey[64] = "4390Y61FDO124533xLF33b0c5CKwvymB";
    char caKey2[64] = "d4624c36b6795d1d99dcf0547af5443d";
    char caSignTmp[1024] = "";
    char caTsid[64] = "";
    char cDataLeft[16] = "<![CDATA[";
    char cDataRight[16] = "]]>";
    char caTemp[128] = "";
    iReturn = utMsgGetSomeNVar(psMsgHead, 3,
                               "PASTSID", UT_TYPE_STRING, sizeof(caTsid) - 1 , caTsid,
                               "total_fee", UT_TYPE_STRING, sizeof(caTotal_fee) - 1 , caTotal_fee,
                               "tradeno", UT_TYPE_STRING, sizeof(caOut_trade_no) - 1 , caOut_trade_no);
    //pasLogs(1066, 1066, "get weixin token=[%s]", getWeiXinAccessToken(""));
    //strcpy(caOut_trade_no, createTradeNo());
    utPltDbHead* psDbHead = utPltInitDb();

    strcpy(caAppId, utComGetVar_sd(psShmHead, "WeChatPayAppid", "wx709aadbe9b24f033"));
    strcpy(caBody, utComGetVar_sd(psShmHead, "WeChatPayBody", "shopbody"));
    strcpy(caDevice_info, utComGetVar_sd(psShmHead, "WeChatPayDevice_info", "deviceinfo"));
    //strcpy(caMch_id, utComGetVar_sd(psShmHead, "WeChatPayMch_id", "1283145801"));
    strcpy(caMch_id, utComGetVar_sd(psShmHead, "WeChatPayMch_id", "1283145801"));
    strcpy(caNonce_str, getNonceStr());
    strcpy(caNotify_url, utComGetVar_sd(psShmHead, "payNotify_url", "http://www.weixin.qq.com/wxpay/pay.php"));
    pasLogs(1066, 1066, "\n====================caNotify_url=%s===================", caNotify_url);
    strcpy(caSpbill_create_ip, utComGetVar_sd(psShmHead, "caSpbill_create_ip", "123.12.12.123"));
    iTotal_fee = atol(caTotal_fee);
    sprintf(caXml, "<xml>\r\n");
    sprintf(caXml + strlen(caXml), "<appid>%s</appid>\r\n", caAppId);
    sprintf(stringA + strlen(stringA), "appid=%s&", caAppId);
    sprintf(caXml + strlen(caXml), "<body>%s</body>\r\n", caBody);//商品描述
    sprintf(stringA + strlen(stringA), "body=%s&", caBody);
    sprintf(caXml + strlen(caXml), "<device_info>%s</device_info>\r\n", caDevice_info);
    sprintf(stringA + strlen(stringA), "device_info=%s&", caDevice_info);
    sprintf(caXml + strlen(caXml), "<mch_id>%s</mch_id>\r\n", caMch_id);
    sprintf(stringA + strlen(stringA), "mch_id=%s&", caMch_id);
    sprintf(caXml + strlen(caXml), "<nonce_str>%s</nonce_str>\r\n", caNonce_str);
    sprintf(stringA + strlen(stringA), "nonce_str=%s&", caNonce_str);
    sprintf(caXml + strlen(caXml), "<notify_url>%s</notify_url>\r\n", caNotify_url);
    sprintf(stringA + strlen(stringA), "notify_url=%s&", caNotify_url);
    sprintf(caXml + strlen(caXml), "<out_trade_no>%s</out_trade_no>\r\n", caOut_trade_no);//商品订单号
    sprintf(stringA + strlen(stringA), "out_trade_no=%s&", caOut_trade_no);
    sprintf(caXml + strlen(caXml), "<spbill_create_ip>%s</spbill_create_ip>\r\n", caSpbill_create_ip);//终端IP
    sprintf(stringA + strlen(stringA), "spbill_create_ip=%s&", caSpbill_create_ip);
    sprintf(caXml + strlen(caXml), "<total_fee>%d</total_fee>\r\n", iTotal_fee);//总金额
    sprintf(stringA + strlen(stringA), "total_fee=%d&", iTotal_fee);
    sprintf(caXml + strlen(caXml), "<trade_type>APP</trade_type>\r\n");
    sprintf(stringA + strlen(stringA), "trade_type=APP&");
    pasLogs(1066, 1066, "生成字符串=%s", stringA);
    sprintf(stringA + strlen(stringA), "key=%s", caKey);
    pasLogs(1066, 1066, "连接KEY之后=%s", stringA);
    //将stringA进行md5加密
    utMd5(stringA, strlen(stringA), caSignTmp);
    pasStrCvtBin2Hex(caSignTmp, 16, caSign);
    pasLogs(1066, 1066, "md5加密后=%s", caSign);
    sprintf(caXml + strlen(caXml), "<sign>%s</sign>\r\n", caSign);
    sprintf(caXml + strlen(caXml), "</xml>\r\n", caSign);
    pasLogs(1066, 1066, "最终提交的XML=%s", caXml);
    // pasGBKToUtf8(caXml,caXml_utf8,2000);
    strcpy(caHost, utComGetVar_sd(psShmHead, "WeChatPayHost", "https://api.mch.weixin.qq.com/pay/unifiedorder"));
    // strcpy(caHtml, getXmlConByPost(caHost, caXml));
    strcpy(caHtml, getHttpsXmlConByPost(caHost, caXml));

    //iReturn = pasSendHttpPostXml(caHost, "/pay/unifiedorder", caHtml, 2023, caXml);
    pasLogs(1066, 1066, "iReturn=%d Html:[%s]\n", iReturn, caHtml);

    getNodeValue(caHtml, "<return_code>", "</return_code>", caReturn, 16);
    getNodeValue(caHtml, "<return_msg>", "</return_msg>", caRetMsg, 128);
    pasLogs(1066, 1066, "caRetMsg=[%s]\n", caRetMsg);

    if(strcmp(caReturn, "SUCCESS") == 0)
    {
        getNodeValue(caHtml, "<result_code>", "</result_code>", caReturn, 64);
        if(strcmp(caReturn, "SUCCESS") == 0)
        {
            getNodeValue(caHtml, "<prepay_id>", "</prepay_id>", caPrepay_id, 64);
            //返回预支付订单
            pasLogs(1066, 1066, "最终得到caPrepay_id=[%s]\n", caPrepay_id);
            //生成sign返回给前台
            //统一下单接口返回正常的prepay_id，再按签名规范重新生成签名后，将数据传输给APP。
            //参与签名的字段名为appId，partnerId，prepayId，nonceStr，timeStamp，package
            //注意：package的值格式为Sign=WXPay
            memset(stringA, 0, sizeof(stringA));
            sprintf(stringA + strlen(stringA), "appid=%s&", caAppId);
            memset(caNonce_str, 0, sizeof(caNonce_str));
            strcpy(caNonce_str, getNonceStr());
            char otherNonce_str[36] = "";
            memcpy(otherNonce_str, caNonce_str, 36);
            sprintf(stringA + strlen(stringA), "noncestr=%s&", caNonce_str);
            sprintf(stringA + strlen(stringA), "package=Sign=WXPay&");
            sprintf(stringA + strlen(stringA), "partnerid=%s&", caMch_id);
            sprintf(stringA + strlen(stringA), "prepayid=%s&", caPrepay_id);
            ulong lNow = time(0);
            sprintf(stringA + strlen(stringA), "timestamp=%lu&", lNow);
            sprintf(stringA + strlen(stringA), "key=%s", caKey);
            //sprintf(stringA + strlen(stringA), "key=%s", caKey2);

            pasLogs(1066, 1066, "连接KEY后=[%s]", stringA);
            //将stringA进行md5加密
            memset(caSignTmp, 0, sizeof(caSignTmp));
            memset(caSign, 0, sizeof(caSign));
            utMd5(stringA, strlen(stringA), caSignTmp);
            pasStrCvtBin2Hex(caSignTmp, 16, caSign);
            pasLogs(1066, 1066, "发给前台的sign=%s", caSign);
            utPltPutVar(psDbHead, "sign", caSign);
            utPltPutVar(psDbHead, "app_id", caAppId);
            utPltPutVar(psDbHead, "trade_no", caOut_trade_no);
            utPltPutVar(psDbHead, "prepay_id", caPrepay_id);
            utPltPutVar(psDbHead, "mch_id", caMch_id);
            pasLogs(1066, 1066, "caNonce_str=%s", otherNonce_str);
            utPltPutVar(psDbHead, "nonce_str", otherNonce_str);
            utPltPutVarF(psDbHead, "timestamp", "%lu", lNow);
            utPltPutVar(psDbHead, "result", "0");
        }
        else
        {
            memset(caRetMsg, 0, sizeof(caRetMsg));
            getNodeValue(caHtml, "<err_code_des>", "</err_code_des>", caRetMsg, 128);
            utPltPutVar(psDbHead, "result", "2");
        }

    }
    else
    {
        utPltPutVar(psDbHead, "result", "1");
    }
    utPltPutVar(psDbHead, "mesg", (char*)convert("GBK", "UTF-8", caRetMsg));
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/weixin/getPreOrder.htm");
    return 0;
}

char *ictWebSelect(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead, char *sql, long cid, char *stype, char *opt, char *groupcode, char *areaid, char *groupid)
{
    if(cid > 0) // 单位查询
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and ncsrvuserlog.groupid = %lu", cid);
    }
    if(!utStrIsSpaces(opt))  // 商家查询
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and ncsrvuserlog.groupcode like '%s%c'", opt, '%');
    }
    if(!utStrIsSpaces(groupcode))  // 商家权限查询
    {
        char caOpt[64] = "";
        sprintf(caOpt, "%s%s", ncmWebAutoDefGet16(psShmHead, iFd, psMsgHead, stype), groupcode);
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and ncsrvuserlog.groupcode like '%s%c'", caOpt, '%');
    }
    if(!utStrIsSpaces(areaid))  // 区域查询
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and ncsrvuserlog.groupcode like '%s%c'", areaid, '%');
    }
    if(!utStrIsSpaces(groupid))  // 单位查询
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and ncsrvuserlog.groupid = %lu", atol(groupid));
    }

    return sql;
}


int ictGetTraffic(utShmHead* psShmHead, int iFd, utMsgHead* psMsgHead)
{
    utMsgOutMsgToLog(1066, 1069, psMsgHead, "[ictGetTraffic] \n");
    // 初始化数据库
    char sql[4096] = "";
    pasDbCursor *psCur = NULL;
    int iret = 0;
    int iNum = 0;
    utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板

    // 接收信息变量
    char sdate[32] = "";
    char edate[32] = "";
    char slogdate[64] = "";
    char elogdate[64] = "";
    char slogtime[64] = "";
    char elogtime[64] = "";
    char username[32] = "";
    char mac[18] = "";
    char ip[16] = "";
    char dev[32] = "";
    char os[32] = "";
    char starttime[64] = "";
    char endtime[64] = "";
    char traffic[64] = "";
    char groupcode[32] = "";
    char areaid[32] = "";
    char groupid[32] = "";
    char caGroupname[64] = "";

    // 存放数据库取出的数据的变量
    char caUsername[32] = "";
    char caMac[18] = "";
    char caIp[16] = "";
    char caDev[24] = "";
    char caOs[16] = "";
    char caStart[32] = "";
    char caEnd[32] = "";
    unsigned long lUbytes = 0;
    unsigned long lDbytes = 0;
    double dBytes = 0.0;

    unsigned long lSdate = 0;
    unsigned long lEdate = time(0); // 当前时间(默认结束时间)

    char caUpdate[16] = "";
    char caDel[16] = "";
    // 关键字
    char _keyword[33] = "";
    char caSesid[1024] = "";

    // 数据表
    unsigned long lCount1 = 0; // 表中记录的个数
    long iReturn; // 单记录查询
    char start[17] = ""; // 默认记录从0开始
    char limit[17] = ""; // 限定一页中的记录是40条
    unsigned long iStart = 0; // 转换后的默认开始atoi(start)
    unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)

    // 排序
    char sort[17] = "";
    char dir[9] = "";

    // 翻页
    unsigned long lTotRec = 0; // 记录表中的数据的个数

    // 导出
    char caExport[32] = "";
    FILE *fp = NULL;
    char caFilename[32] = "";
    char caFile[128] = "";
    char caPath[128] = "";

    // 商家
    char caStype[8] = "";
    char caOptd[64] = "";
    unsigned long lCid = 0;
    iReturn = dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
    iReturn = dsCltGetSessionValue(1, "stype", UT_TYPE_STRING, sizeof(caStype) - 1, caStype);
    iReturn = dsCltGetSessionValue(1, "opt", UT_TYPE_STRING, sizeof(caOptd) - 1, caOptd);
    pasLogs(1066, 1069, "lCid = %d\n", lCid);
    pasLogs(1066, 1069, "*******caStype = %s \n", caStype);
    pasLogs(1066, 1069, "*******caOptd = %s \n", caOptd);

    // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
    utMsgGetSomeNVar(psMsgHead, 24, "keyword",     UT_TYPE_STRING,  sizeof(_keyword) - 1,       _keyword,
                     "slogdate",    UT_TYPE_STRING,  sizeof(slogdate) - 1,       slogdate,
                     "elogdate",    UT_TYPE_STRING,  sizeof(elogdate) - 1,       elogdate,
                     "slogtime",    UT_TYPE_STRING,  sizeof(slogtime) - 1,       slogtime,
                     "elogtime",    UT_TYPE_STRING,  sizeof(elogtime) - 1,       elogtime,
                     "username",    UT_TYPE_STRING,  sizeof(username) - 1,       username,
                     "mac",         UT_TYPE_STRING,  sizeof(mac) - 1,            mac,
                     "ip",          UT_TYPE_STRING,  sizeof(ip) - 1,             ip,
                     "dev",         UT_TYPE_STRING,  sizeof(dev) - 1,                dev,
                     "os",          UT_TYPE_STRING,  sizeof(os) - 1,             os,
                     "starttime",   UT_TYPE_STRING,  sizeof(starttime) - 1,      starttime,
                     "endtime",     UT_TYPE_STRING,  sizeof(endtime) - 1,        endtime,
                     "traffic",     UT_TYPE_STRING,  sizeof(traffic) - 1,        traffic,
                     "groupcode",   UT_TYPE_STRING,  sizeof(groupcode) - 1,      groupcode,
                     "areaid",      UT_TYPE_STRING,  sizeof(areaid) - 1,         areaid,
                     "groupid",     UT_TYPE_STRING,  sizeof(groupid) - 1,          groupid,
                     "start",       UT_TYPE_STRING,  sizeof(start) - 1,          start,
                     "limit",       UT_TYPE_STRING,  sizeof(limit) - 1,          limit,
                     "sort",        UT_TYPE_STRING,  sizeof(sort) - 1,           sort,
                     "dir",         UT_TYPE_STRING,  sizeof(dir) - 1,            dir,
                     "selsid",      UT_TYPE_STRING,  sizeof(caSesid) - 1,        caSesid,
                     "del",               UT_TYPE_STRING,  sizeof(caDel) - 1,          caDel,
                     "update",      UT_TYPE_STRING,  sizeof(caUpdate) - 1,       caUpdate,
                     "exp",    UT_TYPE_STRING,  sizeof(caExport) - 1,       caExport);

    trim(_keyword);

    // 排序
    if(strlen(sort) == 0)
    {
        strcpy(sort, "starttime");
    }

    // 读取配置文件的配置时间
    char cnfSdate[32] = "";
    char cnfEdate[32] = "";
    char cnfPath[128] = "";
    char *pConfig;
    pasConfList *psConfig;

    pConfig = (char *)pasGetConfigFileName();
    psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
    if(psConfig == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncmsysman/ncmsg_back.htm", "读取配置文件", (char *)ncLang("0427配置文件出错"));
        return 0;
    }
    //如若没有配置，默认null。
    strcpy(cnfSdate, (char *)pasUtlLookConfig(psConfig, "StatisticsStime", "")); // pasUtlLookConfig的最后一个参数为StatisticsStim的默认值
    strcpy(cnfEdate, (char *)pasUtlLookConfig(psConfig, "StatisticsEtime", ""));
    strcpy(cnfPath, (char *)pasUtlLookConfig(psConfig, "ReportPosition", ""));
    pasLogs(1066, 1069, "cnfSdate = %s\n", cnfSdate);
    pasLogs(1066, 1069, "cnfEdate = %s\n", cnfEdate);
    pasLogs(1066, 1069, "cnfPath = %s\n", cnfPath);
    pasUtlFreeConfig(psConfig);

    if(strlen(cnfSdate) > 0 && strlen(cnfEdate) > 0)
    {
        ncTimeToLong(cnfSdate, &lSdate);
        ncTimeToLong(cnfEdate, &lEdate);
        pasLogs(1066, 1069, "lEdate-lSdate = %d\n", lEdate - lSdate);
    }
    else
    {
        // 转换时间
        sprintf(sdate, "%s %s", slogdate, slogtime);
        sprintf(edate, "%s %s", elogdate, elogtime);

        if(strlen(sdate) > 1 && strlen(edate) > 1)
        {
            ncTimeToLong(sdate, &lSdate);
            ncTimeToLong(edate, &lEdate);
            pasLogs(1066, 1069, "lEdate-lSdate = %d\n", lEdate - lSdate);
        }
        else
        {
            ncDaysToLong(lEdate, 1, &lSdate); // 默认时间是今天0点--当前时间
            pasLogs(1066, 1069, "lEdate-lSdate = %d\n", lEdate - lSdate);
        }
    }

    // 查看时间是否正确
    char _sdate[32] = ""; // 格式转换后的开始时间
    char _edate[32] = ""; // 格式转换后的当前时间
    strcpy(_sdate, utTimFormat("%Y/%m/%d %H:%M:%S", lSdate));
    strcpy(_edate, utTimFormat("%Y/%m/%d %H:%M:%S", lEdate));
    pasLogs(1066, 1069, "_sdate = %s\n", _sdate);
    pasLogs(1066, 1069, "_edate = %s\n", _edate);

    sprintf(sql, "select count(*) from ncsrvuserlog where starttime>%lu and endtime<%lu", lSdate, lEdate);
    snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and username != 'Guest'");
    pasLogs(1066, 1069, "username=[%s]\n", username);
    if(!utStrIsSpaces(username)) // 根据用户登录名查询
    {
        pasLogs(1066, 1069, "in not utStrIsSpaces\n");
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and username like '%%%s%%'", username);
    }
    strcpy(sql, (char*)ictWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
    lTotRec = lCount1;
    utPltPutVarF(psDbHead, "TotRec", "%lu", lTotRec);
    pasLogs(1066, 1069, "lTotRec = %d\n", lTotRec);
    pasLogs(1066, 1069, "lCount1 = %d\n", lCount1);

    iStart = atol(start);
    iLimit = atol(limit);
    // 导出
    if(strlen(caExport) > 0)
    {
        strcpy(caFilename, "netTrafficReport.csv");
        if(strlen(cnfPath) > 0)
        {
            strcpy(caPath, cnfPath);
        }
        else
        {
            strcpy(caPath, FILEPATH);
        }
        sprintf(caFile, "%s/%s", caPath, caFilename);
        pasLogs(1066, 1069, "caFile = %s\n", caFile);
        fp = fopen(caFile, "w");
        if(fp == NULL)
        {
            pasDbCloseCursor(psCur);
            utWebDispMsg(iFd, psMsgHead, "ncs/ncmsg_back.htm", (char *)ncLang("0546数据导出出错"), (char *)ncLang("0546数据导出出错"));
            return 0;
        }

        fprintf(fp, "用户上网流量统计\n");
        fprintf(fp, "\t\t时间:%s--%s\n", sdate, edate);
        ncUtlPrintCsv(fp, 8, UT_TYPE_STRING, "用户账号",
                      UT_TYPE_STRING, "MAC地址",
                      UT_TYPE_STRING, "IP",
                      UT_TYPE_STRING, "设备类型",
                      UT_TYPE_STRING, "操作系统",
                      UT_TYPE_STRING, "登录时间",
                      UT_TYPE_STRING, "退出时间",
                      UT_TYPE_STRING, "上网流量(mb)");
    }

    if((lEdate - lSdate) <= 86400) // 一天之间, 时间显示(13:23:23)
    {
        sprintf(sql, "select username, mac, ip, dev, os, date_format(from_unixtime(starttime), '%T') as start, date_format(from_unixtime(endtime), '%T') as end, ubytes, dbytes,ncsrvgroup.dispname from ncsrvuserlog left join ncsrvgroup on ncsrvuserlog.groupid=ncsrvgroup.groupid where starttime>%lu and endtime<%lu", lSdate, lEdate);
    }
    else   // 超过一天, 时间显示(2013/02/12 13:23:23)
    {
        sprintf(sql, "select username, mac, ip, dev, os, from_unixtime(starttime) as start, from_unixtime(endtime) as end, ubytes, dbytes,ncsrvgroup.dispname from ncsrvuserlog left join ncsrvgroup on ncsrvuserlog.groupid=ncsrvgroup.groupid where starttime>%lu and endtime<%lu", lSdate, lEdate);
    }
    snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and username != 'Guest'");
    if(!utStrIsSpaces(username)) // 根据用户登录名查询
    {
        pasLogs(1066, 1069, "in sel not utStrIsSpaces\n");
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and username like '%%%s%%'", username);
    }

    strcpy(sql, (char*)ictWebSelect(psShmHead, iFd, psMsgHead, sql, lCid, caStype, caOptd, groupcode, areaid, groupid));
    if(strlen(caExport) == 0)
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
    }
    pasLogs(1066, 1069, "sql = %s\n", sql);
    psCur = pasDbOpenSql(sql, 0);
    if(psCur == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncmsysman/ncmsg_back.htm", (char *)ncLang("0746用户查询"), (char *)ncLang("0171查询数据出错"));
        return 0;
    }

    // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
    iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(caUsername) - 1,  caUsername,
                          UT_TYPE_STRING, sizeof(caMac) - 1,       caMac,
                          UT_TYPE_STRING, sizeof(caIp) - 1,        caIp,
                          UT_TYPE_STRING, sizeof(caDev) - 1,             caDev,
                          UT_TYPE_STRING, sizeof(caOs) - 1,              caOs,
                          UT_TYPE_STRING, sizeof(caStart) - 1,       caStart,
                          UT_TYPE_STRING, sizeof(caEnd) - 1,             caEnd,
                          UT_TYPE_ULONG,  sizeof(long),          &lUbytes,
                          UT_TYPE_ULONG,  sizeof(long),          &lDbytes,
                          UT_TYPE_STRING, sizeof(caGroupname) - 1, caGroupname);

    while(0 == iret || 1405 == iret)
    {
        iNum++;
        if(iNum > 1)
        {
            utPltPutLoopVar(psDbHead, "dh", iNum, ","); // utPltPutLoopVar输出一个循环变量的值。
        }

        dBytes = (double)(lUbytes + lDbytes) / (1024 * 1024);

        utPltSetCvtHtml(1);
        utPltPutLoopVar(psDbHead, "username", iNum, caUsername);
        utPltPutLoopVar(psDbHead, "mac", iNum, caMac);
        utPltPutLoopVar(psDbHead, "ip", iNum, caIp);
        utPltPutLoopVar(psDbHead, "dev", iNum, caDev);
        utPltPutLoopVar(psDbHead, "os", iNum, caOs);
        utPltSetCvtHtml(0);
        utPltPutLoopVar(psDbHead, "starttime", iNum, caStart);
        utPltPutLoopVar(psDbHead, "endtime", iNum, caEnd);
        utPltPutLoopVarF(psDbHead, "traffic", iNum, "%.2f", dBytes);
        utPltPutLoopVar(psDbHead, "gname", iNum, convert("GBK", "UTF-8", caGroupname));
        if(strlen(caExport) > 0)
        {
            ncUtlPrintCsv(fp, 8, UT_TYPE_STRING, caUsername,
                          UT_TYPE_STRING, caMac,
                          UT_TYPE_STRING, caIp,
                          UT_TYPE_STRING, caDev,
                          UT_TYPE_STRING, caOs,
                          UT_TYPE_STRING, caStart,
                          UT_TYPE_STRING, caEnd,
                          UT_TYPE_FLOAT, dBytes);
        }

        memset(caUsername, 0, sizeof(caUsername));
        memset(caMac, 0, sizeof(caMac));
        memset(caIp, 0, sizeof(caIp));
        memset(caDev, 0, sizeof(caDev));
        memset(caOs, 0, sizeof(caOs));
        memset(caStart, 0, sizeof(caStart));
        memset(caEnd, 0, sizeof(caEnd));
        lUbytes = 0;
        lDbytes = 0;
        dBytes = 0.0;
        iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(caUsername) - 1,  caUsername,
                              UT_TYPE_STRING, sizeof(caMac) - 1,       caMac,
                              UT_TYPE_STRING, sizeof(caIp) - 1,        caIp,
                              UT_TYPE_STRING, sizeof(caDev) - 1,             caDev,
                              UT_TYPE_STRING, sizeof(caOs) - 1,              caOs,
                              UT_TYPE_STRING, sizeof(caStart) - 1,       caStart,
                              UT_TYPE_STRING, sizeof(caEnd) - 1,             caEnd,
                              UT_TYPE_ULONG,  sizeof(long),          &lUbytes,
                              UT_TYPE_ULONG,  sizeof(long),          &lDbytes,
                              UT_TYPE_STRING, sizeof(caGroupname) - 1, caGroupname);
    }
    //utPltShowDb(psDbHead);
    pasDbCloseCursor(psCur);
    pasLogs(1066, 1069, "caExport=[%s]", caExport);
    if(strlen(caExport) > 0)
    {
        pasLogs(1066, 1069, "in download");
        fclose(fp);
        utPltFileDownload(iFd, "application/text", caPath, caFilename, caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/report/ncm_web_App_intDetailReport.htm");
    return 0;
}


int jwtBindLoginUid(char* username)
{
    /*
    sprintf(caIn,"%s%u",username,iCount);
    utMd5Encrypto(caIn,strlen(caIn),caUid);
    //往绑定表增加一条数据
    jwtuserinfo */
    return 0;
}

int jwtCheckUidSn(char* uidsn)
{
    return 0;
}

int jwtLogin(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    int iReturn = 0;
    utMsgPrintMsg(psMsgHead);
    char caVname[32 + 1] = "";
    char caPasswd[24 + 1] = "";
    char sqlbuf[1024] = "";
    ulong lCount = 0;
    char caMsg[256] = "";
    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "vname",   UT_TYPE_STRING, sizeof(caVname) - 1, caVname,
                               "passwd",  UT_TYPE_STRING, sizeof(caPasswd) - 1, caPasswd);
    utStrDelSpaces(caVname);
    utPltDbHead* psDbHead = utPltInitDb();
    if(strlen(caVname) > 0)
    {
        memset(sqlbuf, 0, sizeof(sqlbuf));
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select count(*) from ncsrvuser where username='%s' and password='%s'", caVname, caPasswd);
        pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount <= 0)
        {
            snprintf(caMsg, sizeof(caMsg) - 1, "用户名不存在或密码错误", caVname);
            utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
            utPltPutVarF(psDbHead, "result", "%d", 1);
        }
        else
        {
            iReturn = jwtBindLoginUid(caVname);
            if(iReturn == 0)
            {
                snprintf(caMsg, sizeof(caMsg) - 1, "登录成功", caVname);
                utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
                utPltPutVarF(psDbHead, "result", "%d", 0);
            }
            else
            {
                //绑定失败

            }
        }
    }
    else
    {
        snprintf(caMsg, sizeof(caMsg) - 1, "用户名不可以为空");
        utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
        utPltPutVarF(psDbHead, "result", "%d", 2);
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/jwt/login.htm");
    return 0;
}

// 单位管理
int ict_websrvgroup_list(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。

    // 初始化数据库
    char sql[4096] = "";
    char temp1[4096] = "";
    char temp2[4096] = "";
    char temp3[4096] = "";
    pasDbCursor *psCur = NULL;
    int iret = 0;
    int iNum = 0;
    char caShopids[1024];
    utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
    strcpy(caShopids, getDsShopids());
    int i = 0;
    typedef struct group_stat
    {
        unsigned long   groupid;
        char groupname[32];
        char dispname[128];
        unsigned long   moditime;
        unsigned long   flags;
        unsigned long   areaid;
        unsigned long   cityid;
        unsigned long   provid;
        char s_moditime[32];
        char s_flags[32];
        unsigned long count;
    } group_stat_s;
    group_stat_s *stat_temp;

    // 接收信息变量
    char groupid[32] = "";
    char groupname[32] = "";
    char dispname[128] = "";
    char moditime[32] = "";
    char flags[32] = "";
    char provid[32] = "";
    char cityid[32] = "";
    char areaid[32] = "";
    char typedid[32] = "";
    char shaoid[32] = "";
    char fdanid[32] = "";
    char caKey[32] = "";
    char groupcode[32] = "";
    char careaid[32] = "";

    // 存放数据库取出的数据的变量
    unsigned long  lGroupid = 0;
    char                   caGroupname[32] = "";
    char                   caDispname[128] = "";
    unsigned long  lModitime = 0;
    unsigned long  lFlags = 0;
    unsigned long  lAreaid = 0;
    unsigned long  lProvid = 0;
    unsigned long  lCityid = 0;
    char           caProvname[32] = "";
    char           caCityname[32] = "";
    char           caAreaname[32] = "";
    char           s_moditime[32] = "";
    char           s_flags[32] = "";

    char caUpdate[16] = "";
    char caSesid[1024] = "";
    char caDel[16] = "";
    char caRefresh[32] = "";
    char caTemp[4096] = "";
    char caTempd[4096] = "";
    unsigned long lMax = 0;
    unsigned long lCount = 0;
    unsigned long lSid = 0;
    char temp[32] = "";
    char left[20] = "";
    char left1[20] = "";
    char left2[20] = "";
    // 关键字
    char _keyword[33] = "";

    // 数据表
    unsigned long lCount1 = 0; // 表中记录的个数
    long iReturn; // 单记录查询
    char start[17] = ""; // 默认记录从0开始
    char limit[17] = ""; // 限定一页中的记录是40条
    unsigned long iStart = 0; // 转换后的默认开始atoi(start)
    unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)

    // 翻页
    unsigned long lTotRec = 0; // 记录表中的数据的个数

    // 导出
    char caExport[32] = "";

    // 排序
    char sort[17] = "";
    char dir[9] = "";

    // 得到商家stype
    char caStyped[32] = "";
    char caOptd[32] = "";
    unsigned long lCid = 0;
    dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
    dsCltGetSessionValue(1, "stype", UT_TYPE_STRING, sizeof(caStyped) - 1, caStyped);
    dsCltGetSessionValue(1, "opt", UT_TYPE_STRING, sizeof(caOptd) - 1, caOptd);
    printf("lCid = %d\n", lCid);
    printf("caStyped = %s\n", caStyped);
    printf("caOptd = %s\n", caOptd);

    // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
    utMsgGetSomeNVar(psMsgHead, 24, "keyword",   UT_TYPE_STRING, sizeof(_keyword) - 1,  _keyword,
                     "groupid",   UT_TYPE_STRING, sizeof(groupid) - 1,   groupid,
                     "groupname", UT_TYPE_STRING, sizeof(groupname) - 1, groupname,
                     "dispname",  UT_TYPE_STRING, sizeof(dispname) - 1,  dispname,
                     "moditime",  UT_TYPE_STRING, sizeof(moditime) - 1,  moditime,
                     "flags",     UT_TYPE_STRING, sizeof(flags) - 1,     flags,
                     "prov",      UT_TYPE_STRING, sizeof(provid) - 1,    provid,
                     "city",      UT_TYPE_STRING, sizeof(cityid) - 1,    cityid,
                     "area",      UT_TYPE_STRING, sizeof(areaid) - 1,    areaid,
                     "type",      UT_TYPE_STRING, sizeof(typedid) - 1,   typedid,
                     "shao",      UT_TYPE_STRING, sizeof(shaoid) - 1,    shaoid,
                     "fdan",      UT_TYPE_STRING, sizeof(fdanid) - 1,    fdanid,
                     "key",       UT_TYPE_STRING, sizeof(caKey) - 1,     caKey,
                     "groupcode", UT_TYPE_STRING, sizeof(groupcode) - 1, groupcode,
                     "areaid",    UT_TYPE_STRING, sizeof(careaid) - 1,   careaid,
                     "start",     UT_TYPE_STRING, sizeof(start) - 1,     start,
                     "limit",     UT_TYPE_STRING, sizeof(limit) - 1,     limit,
                     "sort",      UT_TYPE_STRING, sizeof(sort) - 1,      sort,
                     "dir",       UT_TYPE_STRING, sizeof(dir) - 1,       dir,
                     "selsid",    UT_TYPE_STRING, 1000,                caSesid,
                     "del",           UT_TYPE_STRING, 10,                  caDel,
                     "refresh",   UT_TYPE_STRING, sizeof(caRefresh) - 1, caRefresh,
                     "update",    UT_TYPE_STRING, 10,                  caUpdate,
                     "caExport",  UT_TYPE_STRING, 10,                  caExport);

    // 排序
    if(strlen(sort) == 0)
    {
        strcpy(sort, "groupid");
    }
    // 去除关键字前后的空格
    trim(_keyword);

    //编码转换
    if(strlen(_keyword) > 0)
    {
        pasCvtGBK(2, _keyword, caTemp, 32);
        strcpy(_keyword, caTemp);
    }
    if(strlen(groupname) > 0)
    {
        pasCvtGBK(2, groupname, caTemp, 32);
        strcpy(groupname, caTemp);
    }
    if(strlen(dispname) > 0)
    {
        pasCvtGBK(2, dispname, caTemp, 128);
        strcpy(dispname, caTemp);
    }

    // 增加/修改
    if(!utStrIsSpaces(caKey))
    {
        if(strlen(typedid) == 0)
        {
            strcpy(typedid, "0");
        }
        if(strlen(shaoid) == 0)
        {
            strcpy(shaoid, "000");
        }
        if(strcmp(caKey, "update") == 0)
        {
            sprintf(left, "%s%s%s", areaid, typedid, shaoid);
            printf("left = %s\n", left);

            sprintf(caTemp, "update ncsrvgroup set groupname='%s', dispname='%s', moditime=%lu, flags=%lu where groupid='%s'", groupname, dispname, time(0), atol(flags), groupid);
            printf("caTemp = %s\n", caTemp);
            pasDbExecSqlF(caTemp);

            /*
                      sprintf(caTempd, "select left(groupname, 12), left(groupname, 14) from ncsrvgroup where groupid='%s'", groupid);
                      pasDbOneRecord(caTempd, 0, UT_TYPE_STRING, sizeof(left1)-1, left1,
                                                                         UT_TYPE_STRING, sizeof(left2)-1, left2);
                      if(strcmp(left1, left)==0 && strcmp(left2, fdanid)==0){
                          printf("***************不变****************\n");
                            sprintf(caTemp,"update ncsrvgroup set groupname='%s', dispname='%s', moditime=%lu, flags=%lu where groupid='%s'",groupname,dispname,time(0),atol(flags),groupid);
                                    printf("caTemp = %s\n", caTemp);
                                    pasDbExecSqlF(caTemp);
                      }else{
                          printf("***************改变****************\n");
                          if(strcmp(left2, fdanid)==0 && strcmp(left1, left)!=0){
                                sprintf(left, "%s%s%s%s", areaid, typedid, shaoid, "01");
                          }else{
                                strcpy(left, fdanid);
                          }
                          printf("left = %s\n", left);

                                sprintf(caTempd,"select max(substring(groupname, 15, 2)) from ncsrvgroup where left(groupname, 14)='%s' group by '%s' ", left, left);
                                  pasDbOneRecord(caTempd, 0, UT_TYPE_ULONG, sizeof(long), &lMax);
                                  sprintf(temp, "%s%02d", left, lMax+1);
                                  printf("temp = %s\n", temp);

                                  lSid=utMd5Code(temp,strlen(temp),"ffff");
                              if(lSid>2000000000) lSid=lSid-2000000000;
                              if(lSid>2000000000) lSid=lSid-2000000000;
                              printf("lSid = %d\n",lSid);

                              sprintf(caTemp,"update ncsrvgroup set groupid=%lu, groupname='%s', dispname='%s', moditime=%lu, flags=%lu where groupid='%s'",lSid,temp,dispname,time(0),atol(flags),groupid);
                                    printf("caTemp = %s\n", caTemp);
                                    pasDbExecSqlF(caTemp);
            */
            //        }
        }
        else if(strcmp(caKey, "add") == 0)
        {
            if(strlen(fdanid) == 0)
            {
                sprintf(left, "%s%s%s%s", areaid, typedid, shaoid, "01");
            }
            else
            {
                strcpy(left, fdanid);
            }
            printf("left = %s\n", left);
            long lLenleft = strlen(left);
            // sprintf(caTempd,"select max(substring(groupname, 13, 4)) from ncsrvgroup where left(groupname, 12)='%s' group by '%s' ", left, left);
            sprintf(caTempd, "select max(substring(groupname, 15, 2)) from ncsrvgroup where left(groupname, %d)='%s' group by '%s' ", lLenleft, left, left);
            printf("caTempd=%s\n", caTempd);
            pasDbOneRecord(caTempd, 0, UT_TYPE_ULONG, sizeof(long), &lMax);
            sprintf(temp, "%s%02d", left, lMax + 1);
            printf("temp = %s\n", temp);

            lSid = utMd5Code(temp, strlen(temp), "ffff");
            if(lSid > 2000000000) lSid = lSid - 2000000000;
            if(lSid > 2000000000) lSid = lSid - 2000000000;
            printf("lSid = %d\n", lSid);

            sprintf(caTemp, "insert into ncsrvgroup (groupid,groupname,dispname,moditime,flags) values (%lu,'%s','%s',%lu,%lu)", lSid, temp, dispname, time(0), atol(flags));
            printf("caTemp = %s\n", caTemp);
            pasDbExecSqlF(caTemp);
        }
        utPltPutVar(psDbHead, "mesg", "数据加载成功");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncmshop/webshop/ncm_websrvgroup_dr_list.htm");
        return 0;
    }
    // 删除
    if(!utStrIsSpaces(caDel))
    {
        if(strlen(caSesid) > 0)
        {
            pasDbExecSqlF("delete from ncsrvgroup where groupid in(%s)", caSesid);
        }
    }

    // 刷新
    if(!utStrIsSpaces(caRefresh))
    {
        ncSrvSysReset(psShmHead);
    }

    // 查询
    strcpy(sql, "select count(*) from ncsrvgroup where 1=1 and flags=1");
    if(!utStrIsSpaces(_keyword)) // 根据关键字查询
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and (dispname like '%%%s%%')", _keyword);
    }
    if(lCid > 0) // 不自定义，小商家
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and groupid = %lu", lCid);
    }
    if(!utStrIsSpaces(caStyped))
    {
        //snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and substring(groupname, 9, 2) = '%s'", caStyped);
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and substring(groupname, 9, 2) = '%s'", caStyped);
    }
    if(!utStrIsSpaces(caOptd))
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and groupname like '%s%c'", caOptd, '%');
    }
    if(!utStrIsSpaces(groupcode))  // 商家权限查询
    {
        char caOpt[64] = "";
        sprintf(caOpt, "%s%s", ncmWebAutoDefGet16(psShmHead, iFd, psMsgHead, caStyped), groupcode);
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and groupname like '%s%c'", caOpt, '%');
    }
    if(!utStrIsSpaces(careaid))  // 区域查询
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and groupname like '%s%c'", careaid, '%');
    }
    if(!utStrIsSpaces(groupid))  // 单位查询
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and groupid = %lu", atol(groupid));
    }
    if(strlen(caShopids) > 0)
    {
        sprintf(sql + strlen(sql), " and groupid in (%s) ", caShopids);
    }
    pasLogs(1066, 9001, "sql=%s ", sql);

    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
    lTotRec = lCount1;
    utPltPutVarF(psDbHead, "TotRec", "%lu", lTotRec);
    printf("lTotRec = %d\n", lTotRec);

    iStart = atol(start);
    iLimit = atol(limit);

    stat_temp = (group_stat_s *)malloc(sizeof(group_stat_s) * lCount1 + 1);
    for(i = 0; i < lCount1; i++)
    {
        stat_temp[i].groupid = 0;
        strcpy(stat_temp[i].groupname, "");
        strcpy(stat_temp[i].dispname, "");
        stat_temp[i].moditime = 0;
        stat_temp[i].flags = 0;
        stat_temp[i].provid = 0;
        stat_temp[i].cityid = 0;
        stat_temp[i].areaid = 0;
        strcpy(stat_temp[i].s_moditime, "");
        strcpy(stat_temp[i].s_flags, "");
        stat_temp[i].count = 0;
    }

    strcpy(sql, "select groupid, groupname, dispname, moditime, flags, left(groupname, 2), left(groupname, 4), left(groupname, 6) from ncsrvgroup where 1=1 and flags=1");
    if(!utStrIsSpaces(_keyword)) // 根据关键字查询
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and (dispname like '%%%s%%')", _keyword);
    }
    if(lCid > 0) // 不自定义，小商家
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and groupid = %lu", lCid);
    }
    if(!utStrIsSpaces(caStyped))
    {
        //snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and substring(groupname, 9, 2) = '%s'", caStyped);
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and substring(groupname, 9, 2) = '%s'", caStyped);
    }
    if(!utStrIsSpaces(caOptd))
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and groupname like '%s%c'", caOptd, '%');
    }
    if(!utStrIsSpaces(groupcode))  // 商家权限查询
    {
        char caOpt[64] = "";
        sprintf(caOpt, "%s%s", ncmWebAutoDefGet16(psShmHead, iFd, psMsgHead, caStyped), groupcode);
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and groupname like '%s%c'", caOpt, '%');
    }
    if(!utStrIsSpaces(careaid))  // 区域查询
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and groupname like '%s%c'", careaid, '%');
    }
    if(!utStrIsSpaces(groupid))  // 单位查询
    {
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and groupid = %lu", atol(groupid));
    }
    printf("caShopids=%s\n", caShopids);
    if(strlen(caShopids) > 0)
    {
        sprintf(sql + strlen(sql), " and groupid in (%s) ", caShopids);
    }

    snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
    printf("sql = %s\n", sql);
    pasLogs(1066, 9001, "sql=%s ", sql);

    psCur = pasDbOpenSql(sql, 0);
    if(psCur == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncmsysman/ncmsg_back.htm", (char *)ncLang("0746用户查询"), (char *)ncLang("0171查询数据出错"));
        return 0;
    }

    // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
    iret = pasDbFetchInto(psCur, UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
                          UT_TYPE_STRING, sizeof(caGroupname) - 1, caGroupname,
                          UT_TYPE_STRING, sizeof(caDispname) - 1,  caDispname,
                          UT_TYPE_ULONG,  sizeof(long),          &lModitime,
                          UT_TYPE_ULONG,  sizeof(long),          &lFlags,
                          UT_TYPE_ULONG,  sizeof(long),          &lProvid,
                          UT_TYPE_ULONG,  sizeof(long),          &lCityid,
                          UT_TYPE_ULONG,  sizeof(long),          &lAreaid);


    while(0 == iret || 1405 == iret)
    {
        if(lModitime > 0)
        {
            strcpy(s_moditime, utTimFormat("%Y/%m/%d %H:%M:%S", lModitime));
        }
        else
        {
            strcpy(s_moditime, "");
        }

        switch(lFlags)
        {
            case 0:
                strcpy(s_flags, "无效");
                break;
            case 1:
                strcpy(s_flags, "有效");
                break;
            case 9:
                strcpy(s_flags, "删除");
                break;
            default:
                break;
        }

        stat_temp[iNum].groupid = lGroupid;
        strcpy(stat_temp[iNum].groupname, caGroupname);
        strcpy(stat_temp[iNum].dispname, caDispname);
        stat_temp[iNum].moditime = lModitime;
        stat_temp[iNum].flags = lFlags;
        stat_temp[iNum].provid = lProvid;
        stat_temp[iNum].cityid = lCityid;
        stat_temp[iNum].areaid = lAreaid;
        strcpy(stat_temp[iNum].s_moditime, s_moditime);
        strcpy(stat_temp[iNum].s_flags, s_flags);

        iNum++;
        lGroupid = 0;
        memset(caGroupname, 0, sizeof(caGroupname));
        memset(caDispname, 0, sizeof(caDispname));
        lModitime = 0;
        lFlags = 0;
        lAreaid = 0;
        lCityid = 0;
        lProvid = 0;
        memset(caAreaname, 0, sizeof(caAreaname));
        memset(s_moditime, 0, sizeof(s_moditime));
        memset(s_flags, 0, sizeof(s_flags));
        iret = pasDbFetchInto(psCur, UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
                              UT_TYPE_STRING, sizeof(caGroupname) - 1, caGroupname,
                              UT_TYPE_STRING, sizeof(caDispname) - 1,  caDispname,
                              UT_TYPE_ULONG,  sizeof(long),          &lModitime,
                              UT_TYPE_ULONG,  sizeof(long),          &lFlags,
                              UT_TYPE_ULONG,  sizeof(long),          &lProvid,
                              UT_TYPE_ULONG,  sizeof(long),          &lCityid,
                              UT_TYPE_ULONG,  sizeof(long),          &lAreaid);
    }
    pasDbCloseCursor(psCur);

    int inum = 0;
    for(i = 0; i < iNum; i++)
    {
        inum++;
        if(inum > 1)
        {
            utPltPutLoopVar(psDbHead, "dh", inum, ","); // utPltPutLoopVar输出一个循环变量的值。
        }

        sprintf(sql, "select count(*) from ncsrvgroupap where groupid=%lu", stat_temp[i].groupid);
        pasDbOneRecord(sql, 0, UT_TYPE_ULONG, sizeof(long), &stat_temp[i].count);

        sprintf(temp1, "select aname from ncmarea where aid = %lu", stat_temp[i].areaid);
        pasDbOneRecord(temp1, 0, UT_TYPE_STRING, sizeof(caAreaname) - 1, caAreaname);

        sprintf(temp1, "select aname from ncmarea where aid = %lu", stat_temp[i].cityid);
        pasDbOneRecord(temp1, 0, UT_TYPE_STRING, sizeof(caCityname) - 1, caCityname);
        pasLogs(1066, 9001, "temp1=%s ", temp1);
        sprintf(temp1, "select aname from ncmarea where aid = %lu", stat_temp[i].provid);
        pasDbOneRecord(temp1, 0, UT_TYPE_STRING, sizeof(caProvname) - 1, caProvname);

        char tempname[128] = "";
        sprintf(tempname, "%s%s%s", caProvname, caCityname, caAreaname);

        utPltPutLoopVarF(psDbHead, "groupid", inum, "%lu", stat_temp[i].groupid);
        utPltPutLoopVar(psDbHead, "groupname", inum, convert("GBK", "UTF-8", stat_temp[i].groupname));
        utPltPutLoopVar(psDbHead, "dispname", inum, convert("GBK", "UTF-8", stat_temp[i].dispname));
        utPltPutLoopVar(psDbHead, "moditime", inum, stat_temp[i].s_moditime);
        utPltPutLoopVarF(psDbHead, "flags", inum, "%lu", stat_temp[i].flags);
        utPltPutLoopVar(psDbHead, "areaname", inum, convert("GBK", "UTF-8", tempname));
        utPltPutLoopVar(psDbHead, "s_flags", inum, stat_temp[i].s_flags);
        utPltPutLoopVarF(psDbHead, "count", inum, "%lu", stat_temp[i].count);

        memset(caProvname, 0, sizeof(caProvname));
        memset(caCityname, 0, sizeof(caCityname));
        memset(caAreaname, 0, sizeof(caAreaname));
        memset(tempname, 0, sizeof(tempname));
    }
    //utPltShowDb(psDbHead);

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncmshop/ncm_websrvgroup_list.htm");
    return 0;
}







int doCalcByDays(int days, utMsgHead *psMsgHead, int iFd, utPltDbHead *psDbHead)
{
    uint8 lstime, lttime;
    int iReturn = 0;
    long collect_number;
    char sql[1024] = "";
    char caStemptime[24];
    int i = 0;
    uint8 letime = time(0);
    for(i = 0; i <= days; i++)
    {
        lttime = letime - 3600 * 24 * (days - i + 1);
        //统计出该天的总数
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql) - 1, "select ifnull(sum(lcount),0) from ictusercount where sdate=\'%s\' ", utTimFormat("%Y%m%d", lttime));
        pasLogs(1066, 1600, "sql=[%s]\n", sql);
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &collect_number);
        if(iReturn != 0)
        {
            utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/frontPage/userCollectNumber.htm");
            return -1;
        }
        //算出该天的0点时间值
        memset(caStemptime, 0, sizeof(caStemptime));
        sprintf(caStemptime, "%s 00:00", utTimFormat("%Y/%m/%d", lttime));
        //printf("caStemptime=%s,leTime=%lu\n", caStemptime, letime);
        lstime = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
        if(i > 1)
        {
            utPltPutLoopVar(psDbHead, "dh", i, ",");
        }
        utPltPutLoopVarF(psDbHead, "collect_number", i, "%lu", collect_number);
        utPltPutLoopVarF(psDbHead, "collect_time", i, "%lu", lstime);
    }
    return 0;
}

typedef struct
{
    long rcount;
    long rtime;
} retData;

uint8 getMonthFirstDay(uint8 lttime)
{
    char caStemptime[56] = "";
    snprintf(caStemptime, sizeof(caStemptime) - 1, "%s/%02d 00:00", utTimFormat("%Y/%m", lttime), 1);
    pasLogs(1066, 1603, "caStemptime=[%s]\n", caStemptime);
    return utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
}

long getCountInDuration(uint8 lstime, uint8 letime)
{
    char sql[1024] = "";
    int iReturn = 0;
    long collect_number = 0;
    pasLogs(1066, 1600, "lstime=%llu, letime=%llu\n", lstime, letime);
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select ifnull(sum(lcount),0) from ictusercount where sdate>=\'%s01\' and sdate<=\'%s31\' ", utTimFormat("%Y%m", lstime), utTimFormat("%Y%m", letime));
    pasLogs(1066, 1600, "temp sql=[%s]\n", sql);
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &collect_number);
    pasLogs(1066, 1600, "iret = %d, n=%lu\n", iReturn, collect_number);
    if((iReturn != 0) && (iReturn != 1405))
    {
        return -1;
    }
    return collect_number;
}


int doCalcByMonths(int months, utMsgHead *psMsgHead, int iFd, utPltDbHead *psDbHead)
{
    retData* retList = (retData*)malloc(sizeof(retData) * months);
    uint8 letime = time(0);
    uint8 lstime = getMonthFirstDay(letime);
    int i = 0;
    long tmpCount = 0;
    for(i = 0; i < months; i++)
    {
        retList[months - i - 1].rtime = lstime;
        //统计出该时间段内所有的数据
        tmpCount = getCountInDuration(lstime, letime);
        if(tmpCount < 0)
        {
            utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/frontPage/userCollectNumber.htm");
            return -2;
        }
        retList[months - i - 1].rcount = tmpCount;
        letime = lstime - 3600;
        lstime = getMonthFirstDay(letime);
    }
    //遍历数据给返回数据赋值
    for(i = 0; i < months; i++)
    {
        if(i > 0)
        {
            utPltPutLoopVar(psDbHead, "dh", i + 1, ",");
        }
        utPltPutLoopVarF(psDbHead, "collect_number", i + 1, "%lu", retList[i].rcount);
        utPltPutLoopVarF(psDbHead, "collect_time", i + 1, "%lu", retList[i].rtime);
    }
    free(retList);
    return 0;
}


int ictFrontPageChart11(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    ulong lTotalCount = 0;
    int iReturn = 0;
    ulong sumNum = 0;
    ulong lNumber = 0;
    char caDispname[32] = "";
    char caKeyword[32] = "";
    char sql[1024] = "";
    pasDbCursor *psCur = NULL;
    utMsgOutMsgToLog(1066, 1603, psMsgHead, "[ictFrontPageChart11] \n");
    uint8 lPackId = 0;
    utPltDbHead *psDbHead = NULL;
    unsigned char *pHash_pac = NULL;

    struct package_s
    {
        uint8 packid;
        char caName[64];
    };
    struct package_s *psPac;

    utMsgGetSomeNVar(psMsgHead, 1,
                     "keyword",   UT_TYPE_STRING, sizeof(caKeyword) - 1,  caKeyword);
    psDbHead = utPltInitDb();
    //将所有套餐装入内存
    pHash_pac = (unsigned char *)pasLHashInit(30, 30, sizeof(struct package_s), 0, 8);
    if(pHash_pac == NULL)
    {
        utPltPutVar(psDbHead, "result", "2");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/frontPage/ictOrderType.htm");
        return 0;
    }
    //将接入厂家装入到内存
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select id,namech from package");
    psCur = pasDbOpenSql(sql, 0);
    if(psCur)
    {
        lPackId = 0;
        memset(caDispname, 0, sizeof(caDispname));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_LONG8, 8, &lPackId,
                                 UT_TYPE_STRING, sizeof(caDispname) - 1, caDispname);
        while((iReturn == 0) || (iReturn == 1405))
        {
            psPac = (struct package_s *)pasLHashLookA(pHash_pac, (char*)(&lPackId));
            if(psPac)
            {
                strcpy(psPac->caName, caDispname);
            }

            lPackId = 0;
            memset(caDispname, 0, sizeof(caDispname));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_LONG8, 8, &lPackId,
                                     UT_TYPE_STRING, sizeof(caDispname) - 1, caDispname);
        }
        pasDbCloseCursor(psCur);

    }
    //查询套餐的统计信息
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select sum(lcount),packageid from ictpackagecount group by packageid");

    psCur = pasDbOpenSql(sql, 0);
    if(psCur)
    {
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_LONG, 4, &lNumber,
                                 UT_TYPE_LONG8, 8, &lPackId);
        int iNum = 0;
        while((iReturn == 0) || (iReturn == 1405))
        {
            sumNum += lNumber;
            iNum ++;
            utPltPutLoopVarF(psDbHead, "collect_number", iNum, "%lu", lNumber);
            memset(caDispname, 0, sizeof(caDispname));
            psPac = (struct package_s *)pasLHashLookA(pHash_pac, (char*)(&lPackId));
            if(psPac)
            {

                strcpy(caDispname, convert("GBK", "UTF-8", psPac->caName));
            }
            utPltPutLoopVar(psDbHead, "collect_name", iNum, caDispname);
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }

            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_LONG, 4, &lNumber,
                                     UT_TYPE_LONG8, 8, &lPackId);
        }
        pasDbCloseCursor(psCur);
        utPltPutVar(psDbHead, "result", "0");
        utPltPutVarF(psDbHead, "sumNum", "%lu", sumNum);
    }
    else
    {
        utPltPutVar(psDbHead, "result", "1");
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/frontPage/ictOrderType.htm");
    //释放内存
    free(pHash_pac);
    return 0;
}

int ictFrontPageChart12(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    ulong lTotalCount = 0;
    int iReturn = 0;
    ulong sumNum = 0;
    ulong lNumber = 0;
    char dispname[24] = "";
    char caKeyword[32] = "";
    char sql[1024] = "";
    pasDbCursor *psCur;
    utMsgOutMsgToLog(1066, 1603, psMsgHead, "[ictFrontPageChart12] \n");
    long lMtype = 0;
    utPltDbHead *psDbHead;

    utMsgGetSomeNVar(psMsgHead, 1,
                     "keyword",   UT_TYPE_STRING, sizeof(caKeyword) - 1,  caKeyword);
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select sum(lcount),mtype from ictchargecount group by mtype");
    psDbHead = utPltInitDb();

    psCur = pasDbOpenSql(sql, 0);
    if(psCur)
    {
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_LONG, 4, &lNumber,
                                 UT_TYPE_LONG, 4, &lMtype);
        int iNum = 0;
        while((iReturn == 0) || (iReturn == 1405))
        {
            sumNum += lNumber;
            iNum ++;
            utPltPutLoopVarF(psDbHead, "collect_number", iNum, "%lu", lNumber);
            utPltPutLoopVarF(psDbHead, "collect_name", iNum, "%lu", lMtype);
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }

            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_LONG, 4, &lNumber,
                                     UT_TYPE_LONG, 4, &lMtype);
        }
        pasDbCloseCursor(psCur);
        utPltPutVar(psDbHead, "result", "0");
        utPltPutVarF(psDbHead, "sumNum", "%lu", sumNum);
    }
    else
    {
        utPltPutVar(psDbHead, "result", "1");
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/frontPage/ictOrderType.htm");
    return 0;
}


uint8 getYearFirstDay(uint8 lltime)
{
    char caStemptime[56] = "";
    snprintf(caStemptime, sizeof(caStemptime) - 1, "%s/%02d/%02d 00:00", utTimFormat("%Y", lltime), 1, 1);
    pasLogs(1066, 1603, "caStemptime=[%s]\n", caStemptime);
    return utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
}

int calcGapTimeByType(char* reqType, uint8* pStart, uint8* pEnd)
{
    uint8 lls = 0, lle = 0;
    uint8 llTime = time(0);
    pasLogs(1066, 1603, "recv type=[%s]", reqType);
    if(strcmp(reqType, "thisMonth") == 0)
    {
        lls = getMonthFirstDay(llTime);
        lle = llTime;
    }
    else if(strcmp(reqType, "lastMonth") == 0)
    {
        lle = getMonthFirstDay(llTime) - 1;
        lls = getMonthFirstDay(lle);
    }
    else if(strcmp(reqType, "thisYear") == 0)
    {
        lls = getYearFirstDay(llTime);
        lle = llTime;
    }
    else if(strcmp(reqType, "lastYear") == 0)
    {
        lle = getYearFirstDay(llTime) - 1;
        lls = getYearFirstDay(lle);
    }
    else
    {
        return -1;
    }
    *pStart = lls;
    *pEnd = lle;
    return 0;
}

int ictFrontPageChart22(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utMsgOutMsgToLog(1066, 1603, psMsgHead, "[ictFrontPageChart22] \n");
    utPltDbHead *psDbHead;
    char request_type[32] = "";
    uint8 llStart = 0, llEnd = 0;
    char sql[1024] = "";
    char caUsername[32] = "";
    char caMname[32] = "";
    char caSumHour[32] = "";
    int iReturn = 0;
    pasDbCursor *psCur;
    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "request_type", UT_TYPE_STRING, sizeof(request_type) - 1, request_type);
    iReturn = calcGapTimeByType(request_type, &llStart, &llEnd);
    if(iReturn)
    {
        pasLogs(1066, 1603, "type not correct");
        //客户端传送的类型不正确
        utPltPutVar(psDbHead, "result", "1");
    }
    else
    {
        memset(sql, 0, sizeof(sql));
        // snprintf(sql, sizeof(sql)-1, "select username,username, 0 from ncsrvuserlog where starttime>=%llu and starttime<=%llu group by username limit 10", llStart, llEnd);

        snprintf(sql, sizeof(sql) - 1, "select aa.username,ifnull(bb.mname, '暂无'),sum((endtime-starttime)/3600) sumHours from ncsrvuserlog aa left join ncsrvuserex bb on aa.username=bb.username where starttime>=%llu and starttime<=%llu group by aa.username order by sumHours desc limit 10", llStart, llEnd);
        pasLogs(1066, 1603, "sql=[%s]", sql);
        psDbHead = utPltInitDb();
        psCur = pasDbOpenSql(sql, 0);
        if(psCur)
        {
            pasLogs(1066, 1603, "psCur is not NULL");
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, sizeof(caUsername) - 1, caUsername,
                                     UT_TYPE_STRING, sizeof(caMname) - 1, caMname,
                                     UT_TYPE_STRING, sizeof(caSumHour) - 1, caSumHour);
            int iNum = 0;
            pasLogs(1066, 1603, "iReturn=%d", iReturn);
            while((iReturn == 0) || (iReturn == 1405))
            {
                iNum ++;
                pasLogs(1066, 1603, "caSumHour = [%s]", caSumHour);
                pasLogs(1066, 1603, "caMname = [%s]", caMname);
                pasLogs(1066, 1603, "caUsername = [%s]", caUsername);
                utPltPutLoopVar(psDbHead, "sumhour", iNum, caSumHour);
                utPltPutLoopVar(psDbHead, "realname", iNum, convert("GBK", "UTF-8", caMname));
                utPltPutLoopVar(psDbHead, "username", iNum, caUsername);
                memset(caMname, 0, sizeof(caMname));
                memset(caUsername, 0, sizeof(caUsername));
                memset(caSumHour, 0, sizeof(caSumHour));
                if(iNum > 1)
                {
                    utPltPutLoopVar(psDbHead, "dh", iNum, ",");
                }
                iReturn = pasDbFetchInto(psCur,
                                         UT_TYPE_STRING, sizeof(caUsername) - 1, caUsername,
                                         UT_TYPE_STRING, sizeof(caMname) - 1, caMname,
                                         UT_TYPE_STRING, sizeof(caSumHour) - 1, caSumHour);

            }
            pasDbCloseCursor(psCur);
            utPltPutVar(psDbHead, "result", "0");

        }
        else
        {
            //数据库执行失败
            utPltPutVar(psDbHead, "result", "2");
        }
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/frontPage/rangeData.htm");
    return 0;
}

int ictFrontPageChart21(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utMsgOutMsgToLog(1066, 1600, psMsgHead, "[ictFrontPageChart21] \n");

    utPltDbHead *psDbHead = utPltInitDb();
    char caUsername[32] = "";
    char request_type[32] = "";
    //时间相关变量
    uint8 lstime, lttime, letime;
    char caStemptime[24];
    char sDate[10];
    char sql[8024] = "";
    int iReturn = 0;

    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "username",  UT_TYPE_STRING, sizeof(caUsername) - 1, caUsername,
                               "request_type", UT_TYPE_STRING, sizeof(request_type) - 1, request_type);

    //查询表里符合条件所有的点
    long collect_number = 0;
    if(strcmp(request_type, "day") == 0)
    {
        //2、统计当天目前时间以前的所有的小时
        letime = time(0);
        sprintf(caStemptime, "%s 00:00", utTimFormat("%Y/%m/%d", letime));
        //printf("zero time caStemptime%s\n", caStemptime);
        //当天的0点
        lstime = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
        //统计每个小时点
        lttime = lstime;
        int iNum = 0;
        int hour = 0;
        while(lttime < letime)
        {
            memset(sql, 0, sizeof(sql));
            snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from ncsrvuserlog where starttime>=%llu and starttime<=%llu", lstime, lttime);
            pasLogs(1066, 1600, "sql=[%s]", sql);
            iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &collect_number);
            if(iReturn != 0)
            {
                utPltPutVar(psDbHead, "result", "0");
                utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/frontPage/userCollectNumber.htm");
                return -1;
            }
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            iNum ++;
            utPltPutLoopVarF(psDbHead, "collect_number", iNum, "%lu", collect_number);
            utPltPutLoopVarF(psDbHead, "collect_time", iNum, "%lu", lttime);
            lstime = lttime;
            hour ++;
            memset(caStemptime, 0, sizeof(caStemptime));
            sprintf(caStemptime, "%s %02d:00", utTimFormat("%Y/%m/%d", letime), hour);
            //printf("hour %d caStemptime=%s\n", hour, caStemptime);
            lttime = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
        }
        //最后一个小时点
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select ifnull(count(*),0) from ncsrvuserlog where starttime>=%llu and starttime<=%llu ", lstime, letime);
        pasLogs(1066, 1600, "sql=[%s]", sql);
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &collect_number);
        if(iReturn != 0)
        {
            utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/frontPage/userCollectNumber.htm");
            return -1;
        }
        utPltPutLoopVar(psDbHead, "dh", iNum, ",");
        utPltPutLoopVarF(psDbHead, "collect_number", iNum, "%lu", collect_number);
        utPltPutLoopVarF(psDbHead, "collect_time", iNum, "%lu", letime);
    }
    else if(strcmp(request_type, "week") == 0)
    {
        iReturn = doCalcByDays(7, psMsgHead, iFd, psDbHead);
        if(iReturn)
            return iReturn;
    }
    else if(strcmp(request_type, "month") == 0)
    {
        iReturn = doCalcByDays(30, psMsgHead, iFd, psDbHead);
        if(iReturn)
            return iReturn;
    }
    else if(strcmp(request_type, "season") == 0)
    {
        //把本季度的
        iReturn = doCalcByMonths(3, psMsgHead, iFd, psDbHead);
        if(iReturn)
            return iReturn;
    }
    else if(strcmp(request_type, "year") == 0)
    {
        iReturn = doCalcByMonths(12, psMsgHead, iFd, psDbHead);
        if(iReturn)
            return iReturn;
    }
    else
    {
        //类型不对
        utPltPutVar(psDbHead, "result", "2");//2代表类型不对
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/frontPage/userCollectNumber.htm");
        return 0;
    }
    //返回数据给前端
    utPltPutVar(psDbHead, "result", "1");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/frontPage/userCollectNumber.htm");
    return 0;
}

int getChargeCount(ulong* ltoday, ulong* ltotal)
{
    ulong ltd = 0, ltt = 0;
    int iReturn = 0;
    uint8 lTodayFirst = 0;
    char sql[1024] = "";
    char caStemptime[32] = "";
    uint8 lTime = time(0);

    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select ifnull(sum(lcount), 0) from ictchargecount");
    pasLogs(1066, 1603, "sql=[%s]", sql);
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &ltt);
    //今日新增数量
    sprintf(caStemptime, "%s 00:00", utTimFormat("%Y/%m/%d", lTime));
    pasLogs(1066, 1603, "zero time caStemptime%s\n", caStemptime);
    //当天的0点
    lTodayFirst = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select ifnull(sum(money), 0) from rechargelog_%s where timeval>=%llu", utTimFormat("%Y%m", lTime), lTodayFirst);
    pasLogs(1066, 1603, "sql=[%s]", sql);
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &ltd);
    *ltoday = ltd;
    *ltotal = ltt;
    return 0;
}

int getPackageCount(ulong* ltoday, ulong* ltotal)
{
    ulong ltd = 0, ltt = 0;
    int iReturn = 0;
    uint8 lTodayFirst = 0;
    char sql[1024] = "";
    char caStemptime[32] = "";
    uint8 lTime = time(0);
    //总订购数
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select ifnull(sum(lcount), 0) from ictpackagecount");
    pasLogs(1066, 1603, "sql=[%s]", sql);
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &ltt);
    //今日新增数量
    sprintf(caStemptime, "%s 00:00", utTimFormat("%Y/%m/%d", lTime));
    pasLogs(1066, 1603, "zero time caStemptime%s\n", caStemptime);
    //当天的0点
    lTodayFirst = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from userorderlog_%s where timeval>=%llu", utTimFormat("%Y%m", lTime), lTodayFirst);
    pasLogs(1066, 1603, "sql=[%s]", sql);
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &ltd);
    *ltoday = ltd;
    *ltotal = ltt;
    return 0;
}

int getUserCount(ulong* ltoday, ulong* ltotal)
{
    ulong ltd = 0, ltt = 0;
    int iReturn = 0;
    uint8 lTodayFirst = 0;
    char sql[1024] = "";
    char caStemptime[32] = "";
    uint8 lTime = time(0);
    //总人数
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from ncsrvuser");
    pasLogs(1066, 1603, "sql=[%s]", sql);
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &ltt);
    //今日新增数量
    sprintf(caStemptime, "%s 00:00", utTimFormat("%Y/%m/%d", lTime));
    pasLogs(1066, 1603, "zero time caStemptime%s\n", caStemptime);
    //当天的0点
    lTodayFirst = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from ncsrvuser where addtime>=%llu", lTodayFirst);
    pasLogs(1066, 1603, "sql=[%s]", sql);
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &ltd);
    *ltoday = ltd;
    *ltotal = ltt;
    return 0;
}


int ictGetTopBlock(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    ulong lOnlineUser = 0;
    ulong lTodayUser = 0, lTotalUser = 0;
    ulong lTodayCharge = 0, lTotalCharge = 0;
    ulong lTodayPackage = 0, lTotalPackage = 0;
    utMsgOutMsgToLog(1066, 1602, psMsgHead, "[ictGetTopBlock] \n");
    utPltDbHead *psDbHead = utPltInitDb();
    //在线用户数量
    lOnlineUser = getOnlineUser(psShmHead);
    //今日新增用户以及用户总数
    getUserCount(&lTodayUser, &lTotalUser);
    //今日订购套餐数和套餐总数
    getPackageCount(&lTodayPackage, &lTotalPackage);
    //今日充值数和充值总数
    getChargeCount(&lTodayCharge, &lTotalCharge);
    //返回数据给前端
    utPltPutVar(psDbHead, "result", "0");
    utPltPutVarF(psDbHead, "onlineUser", "%lu", lOnlineUser);
    utPltPutVarF(psDbHead, "todayUser", "%lu", lTodayUser);
    utPltPutVarF(psDbHead, "totalUser", "%lu", lTotalUser);
    utPltPutVarF(psDbHead, "todayCharge", "%lu", lTodayCharge);
    utPltPutVarF(psDbHead, "totalCharge", "%lu", lTotalCharge);
    utPltPutVarF(psDbHead, "todayPackage", "%lu", lTodayPackage);
    utPltPutVarF(psDbHead, "totalPackage", "%lu", lTotalPackage);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/frontPage/topBlock.htm");
    return 0;
}

int ictGetTsidByIp(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    int iReturn = 0;
    utMsgPrintMsg(psMsgHead);
    char caIctIp[64] = "";

    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "ictip",   UT_TYPE_STRING, sizeof(caIctIp) - 1, caIctIp);
    utStrDelSpaces(caIctIp);
    utPltDbHead* psDbHead = utPltInitDb();

    uint8 llTsid = getTsidByIp(psShmHead, caIctIp);
    if(llTsid > 0)
    {
        //查询一下该用户是否在共享内存
        utPltPutVarF(psDbHead, "result", "%d", 0);
    }
    else
    {
        utPltPutVarF(psDbHead, "result", "%d", 1);
    }
    utPltPutVarF(psDbHead, "tsid", "%llu", llTsid);

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/login/login.htm");
    return 0;
}




int ictInitWebFun_wp(utShmHead * psShmHead)
{
    pasSetTcpFunName("ictFirstDispAd", ictFirstDispAd, 0);
    pasSetTcpFunName("ictGetUserLoginInfo", ictGetUserLoginInfo, 0);
    //投放模版
    // pasSetTcpFunName("ictAdplatelist_v9", ictAdplatelist_v9, 0);
    pasSetTcpFunName("ictAdindexlist_v9", ictAdindexlist_v9, 0);
    pasSetTcpFunName("ictAdDefAd_upload_v9", ictAdDefAd_upload_v9, 0);
    pasSetTcpFunName("ictAdPlateTerm_Save_v9", ictAdPlateTerm_Save_v9, 0);
    pasSetTcpFunName("ictAdPlate_dir", ictAdPlate_dir, 0);
    pasSetTcpFunName("ictShowAdIndex_v9", ictShowAdIndex_v9, 0);
    pasSetTcpFunName("proauthShowPlateBm", proauthShowPlateBm, 0);
    pasSetTcpFunName("ictAdIndex_upload_v9", ictAdIndex_upload_v9, 0);
    pasSetTcpFunName("ictAdPlate_Save_v9", ictAdPlate_Save_v9, 0);
    //获取资源
    pasSetTcpFunName("getSrcByAdindex", getSrcByAdindex, 0);
    //生成微信预支付订单
    pasSetTcpFunName("createWeixinPreOrder", createWeixinPreOrder, 0);
    //接收微信订单通知
    pasSetTcpFunName("receiveWeixinNotify", receiveWeixinNotify, 0);
    //接收支付宝订单通知
    pasSetTcpFunName("receiveAlipayNotify", receiveAlipayNotify, 0);
    //支付宝支付首页
    pasSetTcpFunName("ictAlipayIndex", ictAlipayIndex, 0);
    //支付宝提交订单
    pasSetTcpFunName("ictAlipayapi", ictAlipayapi, 0);
    pasSetTcpFunName("showAlipayRet", showAlipayRet, 0);
    pasSetTcpFunName("showWechatRet", showWechatRet, 0);
    //查询流量统计报表
    pasSetTcpFunName("ictGetTraffic", ictGetTraffic, 0);
    //警务通登录
    pasSetTcpFunName("jwtLogin", jwtLogin, 0);
    //单位管理
    pasSetTcpFunName("ict_websrvgroup_list", ict_websrvgroup_list, 0);
    //首页统计接口
    pasSetTcpFunName("ictFrontPageChart11", ictFrontPageChart11, 0);
    pasSetTcpFunName("ictFrontPageChart12", ictFrontPageChart12, 0);
    pasSetTcpFunName("ictFrontPageChart21", ictFrontPageChart21, 0);
    pasSetTcpFunName("ictGetTopBlock", ictGetTopBlock, 0);
    pasSetTcpFunName("ictFrontPageChart22", ictFrontPageChart22, 0);
    //根据IP 获取tsid
    pasSetTcpFunName("ictGetTsidByIp", ictGetTsidByIp, 0);
    return 0;
}

