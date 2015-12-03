#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <dirent.h>

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
#include <iconv.h>

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



int ict_portplate(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    printf("ncm_portplate start!!!!\n");
    pasDbCursor *psCur;
    // utPltDbHead *psDbHead;
    char caPlateid[32];
    utPltDbHead *psDbHead;
    psDbHead = utPltInitDb();
    utMsgPrintMsg(psMsgHead);
    char update[32];
    char sid[32];

    char caStyped[32] = "";
    dsCltGetSessionValue(1, "stype", UT_TYPE_STRING, sizeof(caStyped) - 1, caStyped);
    printf("caStyped = %s\n", caStyped);

    utMsgGetSomeNVar(psMsgHead, 2,
                     "update",        UT_TYPE_STRING,  sizeof(update) - 1,       update,
                     "sid",        UT_TYPE_STRING,  sizeof(sid) - 1,       sid
                    );
    char caTemp[256];
    int total;
    //sprintf(caTemp,"select count(*) from portplate");
    sprintf(caTemp, "select count(*) from portalplate");
    pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, sizeof(total) - 1, &total);
    utPltPutVarF(psDbHead, "TotRec", "%d", total);


    //sprintf(caTemp,"select sid, platename,status from portplate");
    sprintf(caTemp, "select shopplateid, platename,plateid from portalplate where 1=1");

    psCur = pasDbOpenSql(caTemp, 0);

    if(psCur == NULL)
    {
        // printf("pscur is null\n");
        // printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
    }
    if(psCur != NULL)
    {

        int iReturn = 0;
        int iNum = 0;
        long id = 0;
        long status = 0;
        char name[64] = "";

        while(0 == (iReturn = pasDbFetchInto(psCur,
                                             UT_TYPE_LONG, sizeof(id), &id,
                                             UT_TYPE_STRING, 64, name,
                                             UT_TYPE_STRING, 15, caPlateid
                                             //UT_TYPE_LONG,sizeof(status),&status
                                            ))  || 1405 == iReturn)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVarF(psDbHead, "sid", iNum, "%lu", id);
            //utPltPutLoopVarF(psDbHead,"status",iNum,"%lu", status);
            utPltPutLoopVarF(psDbHead, "iNum", iNum, "%lu", iNum);
            //char *nameUTF = convert("GBK", "UTF-8", name);
            utPltPutLoopVarF(psDbHead, "platename", iNum, "%s", name);
            utPltPutLoopVar(psDbHead, "plateid", iNum, caPlateid);
        }

        pasDbCloseCursor(psCur);
    }
    //              utPltShowDb(psDbHead);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "/ad/ncm_portplate.html");


    return 0;
}

int ict_proauthAdindexlist(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caTemp[1024];
    char caEdate[20];
    unsigned long lCurPg, lTotPg;
    unsigned long lTime, l, lTime1, lSip, lDip;
    int i, iNum;
    utPltDbHead *psDbHead;
    int iReturn;
    long lRowNum, lflags, lStartRec, lCount, lSid;
    char sqlbuf[2048] = "";
    char caTemp_d[64];
    pasDbCursor *psCur;
    char caPlateid[32], caPlateindex[32], caDefad[64], caFwidth[16], caFheight[16], caDefsecad[64];
    char caSwidth[16], caSheight[16], caSectitle[128], caSecdesc[128], caFtitle[128], caFdesc[128];
    char caUrl[128], caSurl[128];
    lCurPg = 1;

    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "plateid", UT_TYPE_STRING, 15, caPlateid);



    utStrDelSpaces(caPlateid);
    if(strlen(caPlateid) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caPlateid, caTemp, 15));
        strcpy(caPlateid, caTemp_d);
    }




    sprintf(sqlbuf, "select count(*) from portalplateindex where plateid='%s' ", caPlateid);


    lCount = 0;
    pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);

    sprintf(sqlbuf, "select plateid,plateindex,defad,fwidth,fheight,defsecad,swidth,sheight,sectitle,secdesc,ftitle,fdesc,url,surl from portalplateindex where plateid='%s' order by plateindex ", caPlateid);

    printf("sqlbuf=%s\n", sqlbuf);
    psCur = pasDbOpenSql(sqlbuf, 0);
    if(psCur == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncmsg_pasmsg.htm", "用户查询", "查询数据出错 %s", pasDbErrorMsg(NULL));
        return 0;
    }


    psDbHead = utPltInitDb();

    iReturn = 0;
    memset(caPlateid, 0, sizeof(caPlateid));
    memset(caPlateindex, 0, sizeof(caPlateindex));
    memset(caDefad, 0, sizeof(caDefad));
    memset(caFwidth, 0, sizeof(caFwidth));
    memset(caFheight, 0, sizeof(caFheight));
    memset(caDefsecad, 0, sizeof(caDefsecad));
    memset(caSwidth, 0, sizeof(caSwidth));
    memset(caSheight, 0, sizeof(caSheight));
    memset(caSectitle, 0, sizeof(caSectitle));
    memset(caSecdesc, 0, sizeof(caSecdesc));
    memset(caFtitle, 0, sizeof(caFtitle));
    memset(caFdesc, 0, sizeof(caFdesc));
    memset(caUrl, 0, sizeof(caUrl));
    memset(caSurl, 0, sizeof(caSurl));
    iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 15, caPlateid,
                             UT_TYPE_STRING, 15, caPlateindex,
                             UT_TYPE_STRING, 63, caDefad,
                             UT_TYPE_STRING, 10, caFwidth,
                             UT_TYPE_STRING, 10, caFheight,
                             UT_TYPE_STRING, 63, caDefsecad,
                             UT_TYPE_STRING, 10, caSwidth,
                             UT_TYPE_STRING, 10, caSheight,
                             UT_TYPE_STRING, 127, caSectitle,
                             UT_TYPE_STRING, 127, caSecdesc,
                             UT_TYPE_STRING, 127, caFtitle,
                             UT_TYPE_STRING, 127, caFdesc,
                             UT_TYPE_STRING, 127, caUrl,
                             UT_TYPE_STRING, 127, caSurl);




    iNum = 0;
    while(iReturn == 0 || iReturn == 1405)
    {

        if(iNum > 0)
        {
            utPltPutLoopVar(psDbHead, "dh", iNum + 1, ",");
        }
        else
        {
            utPltPutLoopVar(psDbHead, "dh", iNum + 1, "");
        }
        iNum++;

        utPltPutLoopVarF(psDbHead, "num", iNum, "%lu", iNum);
        utPltPutLoopVar(psDbHead, "plateid", iNum, caPlateid);
        utPltPutLoopVar(psDbHead, "plateindex", iNum, caPlateindex);
        utPltPutLoopVar(psDbHead, "defad", iNum, caDefad);
        utPltPutLoopVar(psDbHead, "fwidth", iNum, caFwidth);
        utPltPutLoopVar(psDbHead, "fheight", iNum, caFheight);
        utPltPutLoopVar(psDbHead, "defsecad", iNum, caDefsecad);
        utPltPutLoopVar(psDbHead, "swidth", iNum, caSwidth);
        utPltPutLoopVar(psDbHead, "sheight", iNum, caSheight);
        utPltPutLoopVar(psDbHead, "sectitle", iNum, caSectitle);
        utPltPutLoopVar(psDbHead, "secdesc", iNum, caSecdesc);
        utPltPutLoopVar(psDbHead, "ftitle", iNum, caFtitle);
        utPltPutLoopVar(psDbHead, "fdesc", iNum, caFdesc);
        utPltPutLoopVar(psDbHead, "url", iNum, caUrl);
        utPltPutLoopVar(psDbHead, "surl", iNum, caSurl);



        memset(caPlateid, 0, sizeof(caPlateid));
        memset(caPlateindex, 0, sizeof(caPlateindex));
        memset(caDefad, 0, sizeof(caDefad));
        memset(caFwidth, 0, sizeof(caFwidth));
        memset(caFheight, 0, sizeof(caFheight));
        memset(caDefsecad, 0, sizeof(caDefsecad));
        memset(caSwidth, 0, sizeof(caSwidth));
        memset(caSheight, 0, sizeof(caSheight));
        memset(caSectitle, 0, sizeof(caSectitle));
        memset(caSecdesc, 0, sizeof(caSecdesc));

        memset(caFtitle, 0, sizeof(caFtitle));
        memset(caFdesc, 0, sizeof(caFdesc));
        memset(caUrl, 0, sizeof(caUrl));
        memset(caSurl, 0, sizeof(caSurl));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 15, caPlateid,
                                 UT_TYPE_STRING, 15, caPlateindex,
                                 UT_TYPE_STRING, 63, caDefad,
                                 UT_TYPE_STRING, 10, caFwidth,
                                 UT_TYPE_STRING, 10, caFheight,
                                 UT_TYPE_STRING, 63, caDefsecad,
                                 UT_TYPE_STRING, 10, caSwidth,
                                 UT_TYPE_STRING, 10, caSheight,
                                 UT_TYPE_STRING, 127, caSectitle,
                                 UT_TYPE_STRING, 127, caSecdesc,
                                 UT_TYPE_STRING, 127, caFtitle,
                                 UT_TYPE_STRING, 127, caFdesc,
                                 UT_TYPE_STRING, 127, caUrl,
                                 UT_TYPE_STRING, 127, caSurl);
    }



    lTotPg = (lCount - 1) / lRowNum + 1;

    utPltPutVarF(psDbHead, "TotRec", "%lu", lCount);

    pasDbCloseCursor(psCur);


    //     utPltShowDb(psDbHead);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ad/ncm_portplateindex.html");
    return 0;
}

int ict_ncAdPlateIndex_Save(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead = NULL;

    long iReturn, lSid;
    char caAdurl[256], caPlateindex[64];;
    char caAdfile[128], caSid[12], caAdname[64], caAdcode[32], caDaterang[128], caApssid[1024], caGroupid[16];
    unsigned long lTime = time(0);
    char caPlateid[20], caPid[20], caAdindex[64];
    char caFtitle[128], caFdesc[128], caFwidth[20], caFheight[20], caDefad[128];
    char caTemp[1024];
    char caGroups[1024], caCorps[1024];
    char caAdPlate[128];
    long lCount;
    char caDel[16];
    char caUrl[128];
    utMsgPrintMsg(psMsgHead);

    psDbHead = utPltInitDbHead();

    utMsgGetSomeNVar(psMsgHead, 9,
                     "plateid",   UT_TYPE_STRING, sizeof(caPlateid) - 1, caPlateid,
                     "plateindex", UT_TYPE_STRING, sizeof(caPlateindex) - 1, caPlateindex,
                     "ftitle",     UT_TYPE_STRING, sizeof(caFtitle) - 1, caFtitle,
                     "fdesc",      UT_TYPE_STRING, sizeof(caFdesc) - 1, caFdesc,
                     "fwidth",     UT_TYPE_STRING, sizeof(caFwidth) - 1, caFwidth,
                     "fheight",    UT_TYPE_STRING, sizeof(caFheight) - 1, caFheight,
                     "defad",      UT_TYPE_STRING, sizeof(caDefad) - 1, caDefad,
                     "url",        UT_TYPE_STRING, sizeof(caUrl) - 1, caUrl,
                     "del",        UT_TYPE_STRING, 8, caDel);


    strcpy(caAdPlate, utComGetVar_sd(psShmHead, "adplate", "/home/adplate"));
    if(caAdPlate[strlen(caAdPlate) - 1] != '/')
    {
        strcat(caAdPlate, "/");
    }
    if(strlen(caPlateid) > 0)
    {
        pasCvtGBK(2, caPlateid, caTemp, 63);
        strcpy(caPlateid, caTemp);
    }









    if(strlen(caFtitle) > 0)
    {
        pasCvtGBK(2, caFtitle, caTemp, 127);
        strcpy(caFtitle, caTemp);
    }




    if(strlen(caFdesc) > 0)
    {
        pasCvtGBK(2, caFdesc, caTemp, 127);

    }


    if(strcmp(caDel, "del") == 0)
    {
        sprintf(caTemp, "select defad from portalplateindex where plateid='%s' and plateindex='%s' ", caPlateid, caPlateindex);
        printf("caTemp=%s\n", caTemp);
        memset(caDefad, 0, sizeof(caDefad));
        pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, 63, caDefad);
        if(strstr(caDefad, "adplate"))
        {
            sprintf(caTemp, "/home%s", caDefad);
            printf("caTemp=%s\n", caTemp);
            remove(caTemp);
            iReturn = pasDbExecSqlF("update portalplateindex set defad='',ftitle='',fdesc='',fwidth='',fheight='',modtime=%d,url='' where plateid='%s' and plateindex='%s' ", time(0), caPlateid, caPlateindex);
        }
    }
    else
    {
        sprintf(caTemp, "select count(*) from portalplateindex where plateid='%s' and plateindex='%s' ", caPlateid, caPlateindex);
        lCount = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lCount);




        if(lCount > 0)
        {
            printf("update portalplateindex set defad='%s',fwidth='%s',fheight='%s',ftitle='%s',fdesc='%s',modtime=%d  where plateid='%s' and plateindex='%s' \n",
                   caDefad, caFwidth, caFheight, caFtitle, caFdesc, time(0), caPlateid, caPlateindex);

            iReturn = pasDbExecSqlF("update portalplateindex set fwidth='%s',fheight='%s',ftitle='%s',fdesc='%s',modtime=%d,url='%s'  where plateid='%s' and plateindex='%s' ",
                                    caFwidth, caFheight, caFtitle, caFdesc, time(0), caUrl, caPlateid, caPlateindex);


        }
    }
    if(iReturn == 0)
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

}

//保存二级广告位缺省广告
int ict_ncAdPlateIndex_Sec_Save(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead = NULL;

    long iReturn, lSid;
    char caAdurl[256], caPlateindex[64];;
    char caAdfile[128], caSid[12], caAdname[64], caAdcode[32], caDaterang[128], caApssid[1024], caGroupid[16];
    unsigned long lTime = time(0);
    char caPlateid[20], caPid[20], caAdindex[64];
    char caStitle[128], caSdesc[128], caSwidth[20], caSheight[20], caDefsecad[128];
    char caTemp[1024];
    char caGroups[1024], caCorps[1024];
    char caAdPlate[128];
    long lCount;
    char caDel[16];
    char caSurl[128];
    utMsgPrintMsg(psMsgHead);

    psDbHead = utPltInitDbHead();

    utMsgGetSomeNVar(psMsgHead, 8,
                     "plateid",   UT_TYPE_STRING, sizeof(caPlateid) - 1, caPlateid,
                     "plateindex", UT_TYPE_STRING, sizeof(caPlateindex) - 1, caPlateindex,
                     "stitle",     UT_TYPE_STRING, sizeof(caStitle) - 1, caStitle,
                     "sdesc",      UT_TYPE_STRING, sizeof(caSdesc) - 1, caSdesc,
                     "swidth",     UT_TYPE_STRING, sizeof(caSwidth) - 1, caSwidth,
                     "sheight",    UT_TYPE_STRING, sizeof(caSheight) - 1, caSheight,
                     "del",        UT_TYPE_STRING, 8, caDel,
                     "surl",       UT_TYPE_STRING, 127, caSurl);



    if(strlen(caPlateid) > 0)
    {
        pasCvtGBK(2, caPlateid, caTemp, 63);
        strcpy(caPlateid, caTemp);
    }


    if(strlen(caStitle) > 0)
    {
        pasCvtGBK(2, caStitle, caTemp, 127);
        strcpy(caStitle, caTemp);
    }




    if(strlen(caSdesc) > 0)
    {
        pasCvtGBK(2, caSdesc, caTemp, 127);

    }


    if(strcmp(caDel, "del") == 0)
    {
        sprintf(caTemp, "select defsecad from portalplateindex where plateid='%s' and plateindex='%s' ", caPlateid, caPlateindex);
        printf("caTemp=%s\n", caTemp);
        memset(caDefsecad, 0, sizeof(caDefsecad));
        pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, 63, caDefsecad);
        if(strstr(caDefsecad, "adplate"))
        {
            sprintf(caTemp, "/home%s", caDefsecad);
            printf("caTemp=%s\n", caTemp);
            remove(caTemp);

        }
        iReturn = pasDbExecSqlF("update portalplateindex set defsecad='',sectitle='',secdesc='',swidth='',sheight='',modtime=%d,surl='' where plateid='%s' and plateindex='%s' ", time(0), caPlateid, caPlateindex);
    }
    else
    {
        sprintf(caTemp, "select count(*) from portalplateindex where plateid='%s' and plateindex='%s' ", caPlateid, caPlateindex);
        lCount = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lCount);




        if(lCount > 0)
        {
            printf("update portalplateindex set swidth='%s',sheight='%s',stitle='%s',sdesc='%s',modtime=%d  where plateid='%s' and plateindex='%s' \n",
                   caSwidth, caSheight, caStitle, caSdesc, time(0), caPlateid, caPlateindex);
            iReturn = pasDbExecSqlF("update portalplateindex set swidth='%s',sheight='%s',sectitle='%s',secdesc='%s',modtime=%d,surl='%s'  where plateid='%s' and plateindex='%s' ",
                                    caSwidth, caSheight, caStitle, caSdesc, time(0), caSurl, caPlateid, caPlateindex);


        }
    }
    if(iReturn == 0)
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


//上传二级广告位文件
int ict_ncAdDefAd_upload_sec(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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
        char *descr_gbk = pasConvert("UTF-8", "GBK", caFile);
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
    if(caAdPlate[strlen(caAdPlate) - 1] == '/')
    {
        caAdPlate[strlen(caAdPlate) - 1] == '\0';
    }





    p = strrstr(caRemote, "/");
    if(p != NULL)
    {
        strcpy(caDfile, ++p);

    }
    sprintf(caPlatepath, "%s/%s/images", caAdPlate, caPlateid);
    if(!utFileIsExist(caPlatepath))
    {
        if(mkdir(caPlatepath, 0755) != 0)
        {
            utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "创建模板路径出错");
            return 0;
        }

    }

    //将上传的文件重名
    sprintf(caNewfile, "/adplate/%s/images/%s_%d_%s", caPlateid, caPlateindex, lTime, caDfile);
    snprintf(caRemote_new, sizeof(caRemote_new), "%s/%s_%d_%s", caPlatepath, caPlateindex, lTime, caDfile);
    printf("caRemote_new=%s\n", caRemote_new);
    printf("caRemote=%s\n", caRemote);
    rename(caRemote, caRemote_new);

    iReturn = pasDbExecSqlF("update portalplateindex set defsecad='%s' where plateid='%s' and plateindex='%s' ",
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

    return 0;
}

//上传默认广告位文件
int ict_ncAdDefAd_upload_v8(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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
        char *descr_gbk = pasConvert("UTF-8", "GBK", caFile);
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
    if(caAdPlate[strlen(caAdPlate) - 1] == '/')
    {
        caAdPlate[strlen(caAdPlate) - 1] == '\0';
    }





    p = strrstr(caRemote, "/");
    if(p != NULL)
    {
        strcpy(caDfile, ++p);

    }
    sprintf(caPlatepath, "%s/%s/images", caAdPlate, caPlateid);
    if(!utFileIsExist(caPlatepath))
    {
        if(mkdir(caPlatepath, 0755) != 0)
        {
            utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "创建模板路径出错");
            return 0;
        }

    }
    /*
                sprintf(caPlatepathtmp,"%s/tmp",caPlatepath);
                if(!utFileIsExist(caPlatepathtmp))
            {
                    if(mkdir(caPlatepathtmp,0755)!=0)
                    {
                           utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","false","1创建模板路径出错");
                            return 0;
                    }

            }
            else{
                printf("caPlaatepathtmp=%s\n",caPlatepathtmp);
                if(strstr(caPlatepathtmp,"tmp")){
                sprintf(caTemp,"rm -Rf %s",caPlatepathtmp);
                system(caTemp);
                       if(mkdir(caPlatepathtmp,0755)!=0)
                    {
                           utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","false","2创建模板路径出错");
                            return 0;
                    }
            }
          }
      */
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


int ictInitWebFun_zk(utShmHead *psShmHead)
{
    pasSetTcpFunName("ict_ncAdDefAd_upload_v8", ict_ncAdDefAd_upload_v8, 0);
	pasSetTcpFunName("ict_ncAdDefAd_upload_sec", ict_ncAdDefAd_upload_sec, 0);

    pasSetTcpFunName("ict_ncAdPlateIndex_Sec_Save", ict_ncAdPlateIndex_Sec_Save, 0);
    pasSetTcpFunName("ict_ncAdPlateIndex_Save", ict_ncAdPlateIndex_Save, 0);
    pasSetTcpFunName("ict_portplate", ict_portplate, 0);
    pasSetTcpFunName("ict_proauthAdindexlist", ict_proauthAdindexlist, 0);
    return 0;
}

