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
//΢������
#include <iconv.h>
#include <ctype.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include "cjson.h"
#include "ict_zjj_tool.h"
#include "ict_wp_tool.h"



//����ID��ȡ��λ
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
    strcpy(pLocal, "\0"); //�����ϴ��ļ�����
    strcpy(pRemote, "\0"); //�������洢λ��
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

//�����ŷָ���ַ���ֳ�һ������
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

//�����ŷָ���ַ���ֳ�һ������
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



//���ص�λ��Ϣ
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

    utShmFreeHash(psShmHead, PROAUTH_LNK_CLIENTIDCODE); /* ���ͷ�  */

    iReturn = utShmHashInit(psShmHead, PROAUTH_LNK_CLIENTIDCODE, lMaxClient, lMaxClient, sizeof(authClientId2Code), 0, 4);

    utShmFreeHash(psShmHead, PROAUTH_LNK_CLIENT); /* ���ͷ�  */

    iReturn = utShmHashInit(psShmHead, PROAUTH_LNK_CLIENT, lMaxClient, lMaxClient, sizeof(authClient), 0, 30);
    if(iReturn != 0)
    {
        printf("Share Memory alloc Error at ProAuthLoadClientsInfo\n");
        return (-1);
    }
    /* ����λ���ϵ��뵽�ڴ���  */
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

//���ص�λ����Ϣ
int ictProAuthLoadGroupInfo(utShmHead *psShmHead)
{
    unsigned long   lMaxClient;
    int             iReturn, iRet;
    char            *p;
    unsigned long lGroupid;
    pasDbCursor *psCur;
    authGroup s, *psData;
    lMaxClient = utComGetVar_ld(psShmHead, "MaxGroups", 300);

    utShmFreeHash(psShmHead, PROAUTH_LNK_GROUP); /* ���ͷ�  */

    iReturn = utShmHashInit(psShmHead, PROAUTH_LNK_GROUP, lMaxClient, lMaxClient, sizeof(authGroup), 0, 4);
    if(iReturn != 0)
    {
        printf("Share Memory alloc Error at ProAuthLoadGroupsInfo\n");
        return (-1);
    }
    /* ����λ�����ϵ��뵽�ڴ���  */
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

//���ģ��
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
                strcpy(caTemp_g, "ȫ��");
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
                strcpy(caTemp_g, "ȫ��");
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
                sprintf(caTemp + strlen(caTemp), "��ǩ��[%s] ", pData[i].mark);
            }
            if(strlen(pData[i].ssid) > 0)
            {
                sprintf(caTemp + strlen(caTemp), "SSID��[%s] ", pData[i].ssid);
            }
            if(strlen(pData[i].ap) > 0)
            {
                sprintf(caTemp + strlen(caTemp), "AP��[%s] ", pData[i].ap);
            }

            if(strlen(caTemp_g) > 0)
            {
                sprintf(caTemp + strlen(caTemp), "��λ�飺[%s] ", caTemp_g);
            }
            if(strlen(caTemp_c) > 0)
            {
                sprintf(caTemp + strlen(caTemp), "��λ��[%s] ", caTemp_c);
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


//���ģ����λ
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
                                        //����login_mainҳ��Ĺ��λ
                                        if(doParseAdvert(psDbHead, caPid, caFilename, "id=\"advert_", pBuf, iNum_s, iNum))
                                        {
                                            //δ�����������autoҳ����λ
                                            if(doParseAdvert(psDbHead, caPid, caFilename, "id=\"advertauto_", pBuf, iNum_s, iNum))
                                            {
                                                //��δ����������okҳ����λ
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



//�ϴ�Ĭ�Ϲ��λ�ļ�
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
            utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "3����ģ��·������");
            return 0;
        }

    }

    //���ϴ����ļ�����
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
        utPltPutVar(psDbHead, "message", "�ϴ��ɹ�");
    }
    else
    {
        utPltPutVar(psDbHead, "titel", "false");

        utPltPutVar(psDbHead, "message", "�ϴ�ʧ��");
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_adfile.htm");


}


//������ģ���ն�Ҳ��������ѡ��
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

    if(!strstr(caPcch, "ѡ��ģ��") && strlen(caPcch) > 0)
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

    if(!strstr(caPceng, "ѡ��ģ��") && strlen(caPceng) > 0)
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

    if(!strstr(caPcdefault, "ѡ��ģ��") && strlen(caPcdefault) > 0)
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

    if(!strstr(caMobilech, "ѡ��ģ��") && strlen(caMobilech) > 0)
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

    if(!strstr(caMobileeng, "ѡ��ģ��") && strlen(caMobileeng) > 0)
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

    if(!strstr(caMobiledefault, "ѡ��ģ��") && strlen(caMobiledefault) > 0)
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

    if(!strstr(caPadch, "ѡ��ģ��") && strlen(caPadch) > 0)
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

    if(!strstr(caPadeng, "ѡ��ģ��") && strlen(caPadeng) > 0)
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

    if(!strstr(caPaddefault, "ѡ��ģ��") && strlen(caPaddefault) > 0)
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

    if(!strstr(caDefaultch, "ѡ��ģ��") && strlen(caDefaultch) > 0)
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

    if(!strstr(caDefaulteng, "ѡ��ģ��") && strlen(caDefaulteng) > 0)
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


    if(!strstr(caPcbig5, "ѡ��ģ��") && strlen(caPcbig5) > 0)
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

    if(!strstr(caMobilebig5, "ѡ��ģ��") && strlen(caMobilebig5) > 0)
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

    if(!strstr(caPadbig5, "ѡ��ģ��") && strlen(caPadbig5) > 0)
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

    if(!strstr(caDefaultbig5, "ѡ��ģ��") && strlen(caDefaultbig5) > 0)
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

    if(!strstr(caDefaultdefault, "ѡ��ģ��") && strlen(caDefaultdefault) > 0)
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

        utPltPutVar(psDbHead, "message", "����ɹ�");
    }
    else
    {
        utPltPutVar(psDbHead, "titel", "false");
        utPltPutVarF(psDbHead, "sid", "%s", caSid);
        utPltPutVar(psDbHead, "message", "����ʧ��");
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_adfile.htm");

}


//��ʾ��ǰģ����Ŀ¼
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

//Ԥ�����λ�ļ�
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
    //  utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","true","�ϴ��ɹ�");

}

/* �༭ҳ�����  */
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

    //��ȡģ��Ŀ��·��
    strcpy(caAdPlate, utComGetVar_sd(psShmHead, "adplate", "/home/adplate"));
    if(caAdPlate[strlen(caAdPlate) - 1] != '/')
    {
        strcat(caAdPlate, "/");
    }
    memset(caPlatepath, 0, sizeof(caPlatepath));
    sprintf(caPlatepath, "%s%s", caAdPlate, sid);


    /* ȡ���ļ���*/
    p = strrstr(caRemote, "/");
    if(p != NULL)
    {
        strcpy(caDfile, ++p);

    }

    if(!utFileIsExist(caPlatepath))
    {
        if(mkdir(caPlatepath, 0755) != 0)
        {
            utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "4����ģ��·������");
            return 0;
        }

    }
    sprintf(caPlatepathtmp, "%s/tmp", caPlatepath);
    if(!utFileIsExist(caPlatepathtmp))
    {
        if(mkdir(caPlatepathtmp, 0755) != 0)
        {
            utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "1����ģ��·������");
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
                utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "2����ģ��·������");
                return 0;
            }
        }
    }
    //���ϴ����ļ�������ʱĿ¼�£�����Ƿ����Ŀ¼Ҫ��
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
    //У��
    sprintf(caTemp, "%s/%s", caPlatepathtmp, sid);
    printf("caTemp=[%s]\n", caTemp);
    if(!utFileIsExist(caTemp))
    {
        utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "�ϴ��ļ�����û�а���ģ������Ŀ¼");
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
        utPltPutVar(psDbHead, "message", "�ϴ��ɹ�");
    }
    else
    {
        utPltPutVar(psDbHead, "titel", "false");

        utPltPutVar(psDbHead, "message", "�ϴ�ʧ��");
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_adfile.htm");
    //  utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","true","�ϴ��ɹ�");

}



//������ģ��
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


        if(!strstr(caPlateid, "Ӣ���ַ�") && strlen(caPlateid) > 0)
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

        if(strstr(caMark, "���"))
        {
            strcpy(caMark, "");
        }

    }


    if(strlen(caSsid) > 0)
    {
        pasCvtGBK(2, caSsid, caTemp, 1000);
        strcpy(caSsid, caTemp);
        if(strstr(caSsid, "���"))
        {
            strcpy(caSsid, "");
        }

    }
    if(strlen(caAp) > 0)
    {
        pasCvtGBK(2, caAp, caTemp, 1000);
        strcpy(caAp, caTemp);
        if(strstr(caAp, "���"))
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
        utPltPutVar(psDbHead, "message", "����ɹ�");
    }
    else
    {
        utPltPutVar(psDbHead, "titel", "false");
        utPltPutVarF(psDbHead, "sid", "%s", caPlateid);
        utPltPutVar(psDbHead, "message", "����ʧ��");
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
            snprintf(caMsg, sizeof(caMsg) - 1, "�û��������ڻ��������");
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
        snprintf(caMsg, sizeof(caMsg) - 1, "adindex������Ϊ��");
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
            snprintf(caMsg, sizeof(caMsg) - 1, "�ɹ�");
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
            snprintf(caMsg, sizeof(caMsg) - 1, "�Ƶ����");
            utPltPutVar(psDbHead, "place", (char*)convert("GBK", "UTF-8", caMsg));
            //utPltPutVar(psDbHead, "place", psOnline->caGroupCode);
            utPltPutVar(psDbHead, "phone", psOnline->caName);

        }
        else
        {
            printf(" Tid %llu not exist\n", lTsid);
            snprintf(caMsg, sizeof(caMsg) - 1, "tsid������");
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
        snprintf(caMsg, sizeof(caMsg) - 1, "�ɹ�");
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
    static char sstr[36] = "";
    memset(sstr, 0, sizeof(sstr));
    strcpy(sstr, "5K8264ILTKCH16CQ2502SI8ZNMTM67VS");
    return sstr;
}

char* createTradeNo()
{
    static char sstr[32] = "";
    memcpy(sstr, "20150806125346", 14);
    return sstr;
}





/**
* ����΢�ŵ�֧�����
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
    pasLogs(1066, 1066, "\n\n===================���յ�΢�ŵķ���===========================\n\n");
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

/**
* ����֧������֧�����
*
*/
int receiveAlipayNotify(utShmHead* psShmHead, int iFd, utMsgHead* psMsgHead)
{
    int iReturn = 0;
    char caOut_trade_no[32] = "";
    char caOutXml[2048] = "";
    char caReturn[16] = "";
    pasLogs(1066, 1066, "\n\n===================���յ�֧�����ķ���===========================\n\n");
    utMsgOutMsgToLog(1066, 1066, psMsgHead, "[recieve alipay] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "PASXml", UT_TYPE_STRING, sizeof(caOutXml) - 1 , caOutXml);
    utPltDbHead* psDbHead = utPltInitDb();
    utPltPutVar(psDbHead, "ret_flag", "success");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ict/pay/alipay.htm");
    return 0;
}


/**
* ����֧����֧����ҳ
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
   * ���������Ա�HTML��ʽ���죬���ļ��ϴ�����
   * @param sParaTemp �����������
   * @param strMethod �ύ��ʽ������ֵ��ѡ��post��get
   * @param strButtonName ȷ�ϰ�ť��ʾ����
   * @param strParaFileName �ļ��ϴ��Ĳ�����
   * @return �ύ��HTML�ı�
   */
	/***
		* �����������
		* //֧������
			String payment_type = "1";
			//��������޸�
			//�������첽֪ͨҳ��·��
			String notify_url = "http://�̻����ص�ַ/create_direct_pay_by_user-JAVA-UTF-8/notify_url.jsp";
			//��http://��ʽ������·�������ܼ�?id=123�����Զ������
	
			//ҳ����תͬ��֪ͨҳ��·��
			String return_url = "http://�̻����ص�ַ/create_direct_pay_by_user-JAVA-UTF-8/return_url.jsp";
			//��http://��ʽ������·�������ܼ�?id=123�����Զ������������д��http://localhost/
	
			//�̻�������
			String out_trade_no = new String(request.getParameter("WIDout_trade_no").getBytes("ISO-8859-1"),"UTF-8");
			//�̻���վ����ϵͳ��Ψһ�����ţ�����
	
			//��������
			String subject = new String(request.getParameter("WIDsubject").getBytes("ISO-8859-1"),"UTF-8");
			//����
	
			//������
			String total_fee = new String(request.getParameter("WIDtotal_fee").getBytes("ISO-8859-1"),"UTF-8");
			//����
	
			//��������
	
			String body = new String(request.getParameter("WIDbody").getBytes("ISO-8859-1"),"UTF-8");
			//��Ʒչʾ��ַ
			String show_url = new String(request.getParameter("WIDshow_url").getBytes("ISO-8859-1"),"UTF-8");
			//����http://��ͷ������·�������磺http://www.�̻���ַ.com/myorder.html
	
			//������ʱ���
			String anti_phishing_key = "";
			//��Ҫʹ����������ļ�submit�е�query_timestamp����
	
			//�ͻ��˵�IP��ַ
			String exter_invoke_ip = "";
			//�Ǿ�����������IP��ַ���磺221.0.0.1
	
	
			//////////////////////////////////////////////////////////////////////////////////
	
			//������������������
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
		*	String mysign = "";
	        if(AlipayConfig.sign_type.equals("MD5") ) {
	        	mysign = MD5.sign(prestr, AlipayConfig.key, AlipayConfig.input_charset);
	        }
	        return mysign;
		*/

#define ALIPAY_GATEWAY_NEW "https://mapi.alipay.com/gateway.do?"
char* alipayBuildRequest(ictAlipayConfig* pAlipayConfig, char* strMethod, char* strButtonName)
{
    //�������������
    static char caHtml[8024] = "";
	char caPrestr[4096] = "";
    memset(caHtml, 0, sizeof(caHtml));
    snprintf(caHtml, sizeof(caHtml) - 1, "<form id=\"alipaysubmit\" name=\"alipaysubmit\"  enctype=\"multipart/form-data\" action=\"%s", ALIPAY_GATEWAY_NEW);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "_input_charset=%s", pAlipayConfig->input_charset);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "\" method=\"%s\">", strMethod);
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"service\" value=\"create_direct_pay_by_user\"/>");
	snprintf(caPrestr+strlen(caPrestr), sizeof(caPrestr)-1, "service=create_direct_pay_by_user&");
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"partner\" value=\"%s\"/>", pAlipayConfig->partner);
	snprintf(caPrestr+strlen(caPrestr), sizeof(caPrestr)-1, "partner=%s&", pAlipayConfig->partner);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"seller_email\" value=\"%s\"/>", pAlipayConfig->seller_email);
	snprintf(caPrestr+strlen(caPrestr), sizeof(caPrestr)-1, "seller_email=%s&", pAlipayConfig->seller_email);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"_input_charset\" value=\"%s\"/>", pAlipayConfig->input_charset);
	snprintf(caPrestr+strlen(caPrestr), sizeof(caPrestr)-1, "_input_charset=%s&", pAlipayConfig->input_charset);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"payment_type\" value=\"%lu\"/>", pAlipayConfig->payment_type);
	snprintf(caPrestr+strlen(caPrestr), sizeof(caPrestr)-1, "payment_type=%lu&", pAlipayConfig->payment_type);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"notify_url\" value=\"%s\"/>", pAlipayConfig->notify_url);
	snprintf(caPrestr+strlen(caPrestr), sizeof(caPrestr)-1, "notify_url=%s&", pAlipayConfig->notify_url);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"return_url\" value=\"%s\"/>", pAlipayConfig->return_url);
	snprintf(caPrestr+strlen(caPrestr), sizeof(caPrestr)-1, "return_url=%s&", pAlipayConfig->return_url);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"out_trade_no\" value=\"%s\"/>", pAlipayConfig->out_trade_no);
	snprintf(caPrestr+strlen(caPrestr), sizeof(caPrestr)-1, "out_trade_no=%s&", pAlipayConfig->out_trade_no);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"subject\" value=\"%s\"/>", pAlipayConfig->subject);
	snprintf(caPrestr+strlen(caPrestr), sizeof(caPrestr)-1, "subject=%s&", pAlipayConfig->subject);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"total_fee\" value=\"%lu\"/>", pAlipayConfig->total_fee);
	snprintf(caPrestr+strlen(caPrestr), sizeof(caPrestr)-1, "total_fee=%lu&", pAlipayConfig->total_fee);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"body\" value=\"%s\"/>", pAlipayConfig->body);
	snprintf(caPrestr+strlen(caPrestr), sizeof(caPrestr)-1, "body=%s&", pAlipayConfig->body);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"show_url\" value=\"%s\"/>", pAlipayConfig->show_url);
	snprintf(caPrestr+strlen(caPrestr), sizeof(caPrestr)-1, "show_url=%s&", pAlipayConfig->show_url);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"anti_phishing_key\" value=\"%llu\"/>", pAlipayConfig->anti_phishing_key);
	snprintf(caPrestr+strlen(caPrestr), sizeof(caPrestr)-1, "anti_phishing_key=%llu&", pAlipayConfig->anti_phishing_key);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"exter_invoke_ip\" value=\"%s\"/>", pAlipayConfig->exter_invoke_ip);
	snprintf(caPrestr+strlen(caPrestr), sizeof(caPrestr)-1, "exter_invoke_ip=%s", pAlipayConfig->exter_invoke_ip);
	/*
	utMd5_Code(unsigned char  *input,unsigned int inlen,unsigned char *pKey,
        unsigned char  *output)
	*/    
	char caMySign[64] = "";
	char caSignTmp[128] = "";
	pasLogs(1066, 1067, "caPrestr=[%s],md5key=[%s]", caPrestr,  pAlipayConfig->md5_key);
	utMd5_Code(caPrestr, strlen(caPrestr), pAlipayConfig->md5_key, caSignTmp);
	pasStrCvtBin2Hex(caSignTmp, 16, caMySign);
	pasLogs(1066, 1067, "mysign=[%s]", caMySign);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"sign\" value=\"%s\"/>", caMySign);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"hidden\" name=\"sign_type\" value=\"%s\"/>", pAlipayConfig->sign_type);
    //submit��ť�ؼ��벻Ҫ����name����
    snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<input type=\"submit\" value=\"%s\" style=\"display:none;\"></form>", strButtonName);
	snprintf(caHtml + strlen(caHtml), sizeof(caHtml) - 1, "<script>document.forms['alipaysubmit'].submit();</script>");
	pasLogs(1066, 1067, caHtml);
    return caHtml;
}


/**
* ֧����֧���ӿ�
* WIDout_trade_no
* WIDsubject
* WIDtotal_fee
* WIDbody
* WIDshow_url
*/
/*
	// ���������ID����2088��ͷ��16λ��������ɵ��ַ���
	public static String partner = "";	
	// �տ�֧�����˺ţ�һ��������տ��˺ž���ǩԼ�˺�
	public static String seller_email = "";
	// �̻���˽Կ
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
	if(strlen(caTotal_fee)>0){
		alipayConfig.total_fee = atol(caTotal_fee);
	}
	
    strcpy(alipayConfig.notify_url, utComGetVar_sd(psShmHead, "aliNotify_url", "http://180.169.1.201:8006/ictpay/notifyAlipayResult"));
    alipayConfig.payment_type = utComGetVar_ld(psShmHead, "aliPayType", 1);
	strcpy(alipayConfig.input_charset, "utf-8");
	strcpy(alipayConfig.partner, "2088411881747090");
	strcpy(alipayConfig.seller_email, "liqin@pronetway.com ");
	strcpy(alipayConfig.md5_key, "5yqgnewnsj0m08t2l0rt0z6gje0dk18c");
	snprintf(alipayConfig.rsa_key+strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key)-1, "MIICXAIBAAKBgQC41wBqh3c5z+AH1nx4edgiqTFf8Jzre37K6NdjFAt9OI6mHGpL");
	snprintf(alipayConfig.rsa_key+strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key)-1, "BzQYGysS7se2pmn7ni7rJC4cLG4fy72ATmDSY9itfQYCJp0Vm2NYJOPh6/a4pllc");
	snprintf(alipayConfig.rsa_key+strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key)-1, "rmsrVQs0ZuDMnZ17LY/iJv9ulA2AsNEv98AnjcoN4peAhnuaKEUSCGjJYQIDAQAB");
	snprintf(alipayConfig.rsa_key+strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key)-1, "AoGAWjaFO3kWVZ8BZpHmeh5twKz3Li4PSFcj0QDHbtHvsZQS+zvPHQNNB57q7Pih");
	snprintf(alipayConfig.rsa_key+strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key)-1, "fKHIBEh1+cnUv2dn8BjOSnr2gaRQEmBGQDNlmXyBGjl6wscIWD5x1TKpOgZwj7x7");
	snprintf(alipayConfig.rsa_key+strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key)-1, "bfH2C/w/8aDFy1DdDYuW5nVUQCe8UUtFhko62V5OokBuS0kCQQDf9nLTOmYMjsnw");
	snprintf(alipayConfig.rsa_key+strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key)-1, "6wDJm04bx1c5FZC6LkMM+QhF3UtL7y665gaerwaob57t1CApve+ADdeUi8Nj2ueo");
	snprintf(alipayConfig.rsa_key+strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key)-1, "9vjRIKUPAkEA00fePG40gdzkEVemK59t6Qj0G9IR3mKM4luYWA1tEzi6RQMY+kXG");
	snprintf(alipayConfig.rsa_key+strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key)-1, "zfwcoVqvsmQS6uJZPEWC98B5cGcMh6YKjwJAavhFI8pyz3Cr11wHRkPdbLsrV+KK");
	snprintf(alipayConfig.rsa_key+strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key)-1, "+8H42Ithfi0SN7SH91DCvKwQXAbkO+fjbMyBRgJWg2a17k2x9Cvx78DN1QJBALb4");
	snprintf(alipayConfig.rsa_key+strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key)-1, "DyKAQeKQm8XVBOfUOofMJDKPmr/e0qkYr3bZsbMqtk0boJ7knIU4JscpVOyFM8jL");
	snprintf(alipayConfig.rsa_key+strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key)-1, "2oGSpRfPTJul8jjq90sCQCEPzEEhRQ+aeu39LY5FHQ8OJcv4bTm4+7X7HDlEXCRq");
	snprintf(alipayConfig.rsa_key+strlen(alipayConfig.rsa_key), sizeof(alipayConfig.rsa_key)-1, "3SByxXfyTepG9DvBFB+ttSD7fx8bAaRWwWYIAFPv490=");
	
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


/**
* ����΢��֧��Ԥ֧������
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

    //strcpy(caOut_trade_no, createTradeNo());
    utPltDbHead* psDbHead = utPltInitDb();

    strcpy(caAppId, utComGetVar_sd(psShmHead, "WeChatPayAppid", "wx709aadbe9b24f033"));
    strcpy(caBody, utComGetVar_sd(psShmHead, "WeChatPayBody", "shopbody"));
    strcpy(caDevice_info, utComGetVar_sd(psShmHead, "WeChatPayDevice_info", "deviceinfo"));
    strcpy(caMch_id, utComGetVar_sd(psShmHead, "WeChatPayMch_id", "1283145801"));
    strcpy(caNonce_str, getNonceStr());
    strcpy(caNotify_url, utComGetVar_sd(psShmHead, "payNotify_url", "http://www.weixin.qq.com/wxpay/pay.php"));
    pasLogs(1066, 1066, "\n====================caNotify_url=%s===================", caNotify_url);
    strcpy(caSpbill_create_ip, utComGetVar_sd(psShmHead, "caSpbill_create_ip", "123.12.12.123"));
    iTotal_fee = atol(caTotal_fee);
    sprintf(caXml, "<xml>\r\n");
    sprintf(caXml + strlen(caXml), "<appid>%s</appid>\r\n", caAppId);
    sprintf(stringA + strlen(stringA), "appid=%s&", caAppId);
    sprintf(caXml + strlen(caXml), "<body>%s</body>\r\n", caBody);//��Ʒ����
    sprintf(stringA + strlen(stringA), "body=%s&", caBody);
    sprintf(caXml + strlen(caXml), "<device_info>%s</device_info>\r\n", caDevice_info);
    sprintf(stringA + strlen(stringA), "device_info=%s&", caDevice_info);
    sprintf(caXml + strlen(caXml), "<mch_id>%s</mch_id>\r\n", caMch_id);
    sprintf(stringA + strlen(stringA), "mch_id=%s&", caMch_id);
    sprintf(caXml + strlen(caXml), "<nonce_str>%s</nonce_str>\r\n", caNonce_str);
    sprintf(stringA + strlen(stringA), "nonce_str=%s&", caNonce_str);
    sprintf(caXml + strlen(caXml), "<notify_url>%s</notify_url>\r\n", caNotify_url);
    sprintf(stringA + strlen(stringA), "notify_url=%s&", caNotify_url);
    sprintf(caXml + strlen(caXml), "<out_trade_no>%s</out_trade_no>\r\n", caOut_trade_no);//��Ʒ������
    sprintf(stringA + strlen(stringA), "out_trade_no=%s&", caOut_trade_no);
    sprintf(caXml + strlen(caXml), "<spbill_create_ip>%s</spbill_create_ip>\r\n", caSpbill_create_ip);//�ն�IP
    sprintf(stringA + strlen(stringA), "spbill_create_ip=%s&", caSpbill_create_ip);
    sprintf(caXml + strlen(caXml), "<total_fee>%d</total_fee>\r\n", iTotal_fee);//�ܽ��
    sprintf(stringA + strlen(stringA), "total_fee=%d&", iTotal_fee);
    sprintf(caXml + strlen(caXml), "<trade_type>APP</trade_type>\r\n");
    sprintf(stringA + strlen(stringA), "trade_type=APP&");
    pasLogs(1066, 1066, "�����ַ���=%s", stringA);
    sprintf(stringA + strlen(stringA), "key=%s", caKey);
    pasLogs(1066, 1066, "����KEY֮��=%s", stringA);
    //��stringA����md5����
    utMd5(stringA, strlen(stringA), caSignTmp);
    pasStrCvtBin2Hex(caSignTmp, 16, caSign);
    pasLogs(1066, 1066, "md5���ܺ�=%s", caSign);
    sprintf(caXml + strlen(caXml), "<sign>%s</sign>\r\n", caSign);
    sprintf(caXml + strlen(caXml), "</xml>\r\n", caSign);
    pasLogs(1066, 1066, "�����ύ��XML=%s", caXml);
    // pasGBKToUtf8(caXml,caXml_utf8,2000);
    strcpy(caHost, utComGetVar_sd(psShmHead, "WeChatPayHost", "https://api.mch.weixin.qq.com/pay/unifiedorder"));
    strcpy(caHtml, getXmlConByPost(caHost, caXml));
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
            //����Ԥ֧������
            pasLogs(1066, 1066, "���յõ�caPrepay_id=[%s]\n", caPrepay_id);
            //����sign���ظ�ǰ̨
            //ͳһ�µ��ӿڷ���������prepay_id���ٰ�ǩ���淶��������ǩ���󣬽����ݴ����APP��
            //����ǩ�����ֶ���ΪappId��partnerId��prepayId��nonceStr��timeStamp��package
            //ע�⣺package��ֵ��ʽΪSign=WXPay
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

            pasLogs(1066, 1066, "����KEY��=[%s]", stringA);
            //��stringA����md5����
            memset(caSignTmp, 0, sizeof(caSignTmp));
            memset(caSign, 0, sizeof(caSign));
            utMd5(stringA, strlen(stringA), caSignTmp);
            pasStrCvtBin2Hex(caSignTmp, 16, caSign);
            pasLogs(1066, 1066, "����ǰ̨��sign=%s", caSign);
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



int ictInitWebFun_wp(utShmHead * psShmHead)
{
    pasSetTcpFunName("ictFirstDispAd", ictFirstDispAd, 0);
    pasSetTcpFunName("ictGetUserLoginInfo", ictGetUserLoginInfo, 0);
    //Ͷ��ģ��
    pasSetTcpFunName("ictAdplatelist_v9", ictAdplatelist_v9, 0);
    pasSetTcpFunName("ictAdindexlist_v9", ictAdindexlist_v9, 0);
    pasSetTcpFunName("ictAdDefAd_upload_v9", ictAdDefAd_upload_v9, 0);
    pasSetTcpFunName("ictAdPlateTerm_Save_v9", ictAdPlateTerm_Save_v9, 0);
    pasSetTcpFunName("ictAdPlate_dir", ictAdPlate_dir, 0);
    pasSetTcpFunName("ictShowAdIndex_v9", ictShowAdIndex_v9, 0);
    pasSetTcpFunName("proauthShowPlateBm", proauthShowPlateBm, 0);
    pasSetTcpFunName("ictAdIndex_upload_v9", ictAdIndex_upload_v9, 0);
    pasSetTcpFunName("ictAdPlate_Save_v9", ictAdPlate_Save_v9, 0);
    //��ȡ��Դ
    pasSetTcpFunName("getSrcByAdindex", getSrcByAdindex, 0);
    //����΢��Ԥ֧������
    pasSetTcpFunName("createWeixinPreOrder", createWeixinPreOrder, 0);
    //����΢�Ŷ���֪ͨ
    pasSetTcpFunName("receiveWeixinNotify", receiveWeixinNotify, 0);
    //����֧��������֪ͨ
    pasSetTcpFunName("receiveAlipayNotify", receiveAlipayNotify, 0);
    //֧����֧����ҳ
    pasSetTcpFunName("ictAlipayIndex", ictAlipayIndex, 0);
    //֧�����ύ����
    pasSetTcpFunName("ictAlipayapi", ictAlipayapi, 0);

    return 0;
}

