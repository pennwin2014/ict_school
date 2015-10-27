#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include <pcap.h>

#include <errno.h>

#include <iconv.h>

#include <dirent.h>
#include "ncportalweb.h"
#include "ncportal.h"
#define COUPONDOWNTIME 60 //下载优惠劵时间间隔

#include "ncportal.h"

#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include "dsutl.h"
#include "dsclt.h"
#include "dlfcn.h"
#include "time.h"

#include "ncm_mall_manag.h"

#define debug DEBUG_VERSION
#define SHOPIMGPATH "/home/ncmysql/ncsrv/html"
#define UPLOADIMGPATH "/home/ncmysql/ncsrv/upload"

int isstrdigit(char *str)
{
    if(str == NULL) return 0;
    while(*str!='\0')
    {
        if(*str < '0' || *str > '9') return 0;
        str++;
    }
    return 1;
}

//购物导航（楼层，品牌管理）
int ncmShoppingGuiding(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    utPltDbHead *psDbHead;
    char caFloorName[34] = "";
    char caMallId[14] = "", caFloorId[14] = "";
    unsigned long lFloorId = 0, lMallId = 0, lBrandCount = 0;
    char caFloorDetail[65540] = "", caTemp_d[65540];
    char caActionName[34] = "";
    char caTemp[1024],  sqlCondition[1024];
    char caTotPg[16], caCurPg[16];
    char caPage[12], caLimit[16];
    char caKeyword[129], groupName[130] = "", caSelSid[1024] = "";
    unsigned long lCurPg = 0, lRowNum = 0, lStartRec = 0, iLimit = 0;
    unsigned long lCount1 = 0;
    int iReturn=0;
    int iNum=0;
    char *nameUTF;    
    long lCurPid = 0; 
    char caShopids[1024];
    strcpy(caShopids, getDsShopids());

    psDbHead= utPltInitDb();
    printf("\n enter ncmShoppingGuiding !!!!\n");
       
#ifdef debug 
    lCurPid = getpid();
    printf("ncmShoppingGuiding: lCurPid = %d\n", lCurPid);
    printf("ncmShoppingGuiding: caShopids = %s\n", caShopids);
    utMsgPrintMsg(psMsgHead);         
    //sleep(10);
#endif    

    utMsgGetSomeNVar(psMsgHead, 9,
        "keyword", UT_TYPE_STRING, sizeof(caKeyword)-1, caKeyword,
        "mallId", UT_TYPE_STRING, sizeof(caMallId)-1, caMallId,
        "actionName", UT_TYPE_STRING, sizeof(caActionName)-1, caActionName,
        "page", UT_TYPE_STRING, sizeof(caPage)-1, caPage,
        "floorName", UT_TYPE_STRING, sizeof(caFloorName)-1, caFloorName,
        "floorDetail", UT_TYPE_STRING, sizeof(caFloorDetail)-1, caFloorDetail,
        "floorId", UT_TYPE_STRING, sizeof(caFloorId)-1, caFloorId,        
        "selsid", UT_TYPE_STRING, sizeof(caSelSid)-1, caSelSid,
        "limit", UT_TYPE_STRING, sizeof(caLimit)-1, caLimit);

    if (0 < strlen(caKeyword)){
        pasCvtGBK(2, caKeyword, caTemp_d, sizeof(caKeyword) - 1);
        strcpy(caKeyword, caTemp_d);
    }
    if (0 < strlen(caFloorName)){
        pasCvtGBK(2, caFloorName, caTemp_d, sizeof(caFloorName) - 1);
        strcpy(caFloorName, caTemp_d);
    }    
    if (0 < strlen(caFloorDetail)){
        pasCvtGBK(2, caFloorDetail, caTemp_d,  sizeof(caFloorDetail) - 1);
        strcpy(caFloorDetail, caTemp_d);
    }
    lCurPg = atol(caPage);
    if (lCurPg <= 0) {
        lCurPg = 1;
    }
    if (0 < strlen(caLimit)) {
        lRowNum=atol(caLimit);
    }
    else{
        lRowNum =40;//每一页面行数
    }
    lStartRec = (lCurPg - 1) * lRowNum; 
    //add floor 
    if (!strcmp("addFloor", caActionName) && strlen(caFloorName)
         && isstrdigit(caMallId)) {
        snprintf(caTemp, sizeof(caTemp), 
            " insert into ncmfloor (name, detail, cid) values ('%s', '%s', \
            %s) ",
            caFloorName, caFloorDetail, caMallId);
        iReturn = pasDbExecSql(caTemp,0);
        printf("ncmShoppingGuiding: caTemp = %s \n", caTemp);
    }
  
    //modify floor
    if (!strcmp("modifyFloor", caActionName) && strlen(caFloorName) 
        && isstrdigit(caMallId) && isstrdigit(caFloorId)) {
        snprintf(caTemp, sizeof(caTemp), 
            " update ncmfloor set name = '%s',detail = '%s', cid = %s \
            where fid = %s", 
            caFloorName, caFloorDetail, caMallId, caFloorId);
        iReturn = pasDbExecSql(caTemp,0);
        printf("ncmShoppingGuiding: caTemp = %s \n", caTemp);
    }
    
    // 删除
    if(!strcmp("delete", caActionName) && strlen(caSelSid)) {
        snprintf(caTemp, sizeof(caTemp), "delete from ncmfloor where fid in(%s)", caSelSid);           
        pasDbExecSqlF(caTemp);
        printf("ncmShoppingGuiding: caTemp = %s \n", caTemp);
    }  
    
    // make select condition    
    snprintf(sqlCondition, sizeof(sqlCondition), "from  ncmfloor AS A, \
        ncsrvgroup AS B where 1 and B.groupid = A.cid ");
	if(strlen(caShopids)){
        snprintf(sqlCondition + strlen(sqlCondition), 
            sizeof(sqlCondition) - strlen(sqlCondition), 
            " and cid in (%s) ",caShopids);
	}        
    if (strlen(caKeyword)){
        snprintf(sqlCondition + strlen(sqlCondition), 
            sizeof(sqlCondition) - strlen(sqlCondition), 
            " and (A.name like '%c%s%c' or A.detail like '%c%s%c' \
            or B.dispname like '%c%s%c' ) ",
            '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%');
    }
    //get total count
    snprintf(caTemp, sizeof(caTemp), " select count(*) %s", sqlCondition);

    iReturn = pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
    printf("ncmShoppingGuiding: lCount = %d, caTemp = %s \n", lCount1, caTemp);
    if (0 == lCount1) { 
        printf( " ncmShoppingGuiding: no floor, please input \
            extracter group name \n " );
        utPltOutToHtml(iFd, psMsgHead, psDbHead, 
            "gwdh/dennisShoppingGuiding_list.htm");
        return 1;
    }        

    utPltPutVarF(psDbHead, "TotRec", "%lu", lCount1);
    // select floors
    snprintf(caTemp, sizeof(caTemp), 
        " select A.fid, A.name, A.detail, A.cid, B.dispname, \
        (select count(*) from ncmbrand where fid = A.fid)  as brandCount \
        %s limit %d, %d ", sqlCondition, lStartRec, lRowNum);    

    psCur = pasDbOpenSql(caTemp,0);
    printf("ncmShoppingGuiding: caTemp = %s \n", caTemp);
    if(psCur != NULL)
    {
        memset(caFloorName, 0, sizeof(caFloorName));
        memset(caFloorDetail, 0, sizeof(caFloorDetail));
        memset(groupName, 0, sizeof(groupName));            
        lFloorId = lMallId = lBrandCount = 0;
        
        while(0==(iReturn = pasDbFetchInto(psCur,
            UT_TYPE_ULONG, 4, &lFloorId,
            UT_TYPE_STRING, 32, caFloorName,
            UT_TYPE_STRING, 65535, caFloorDetail,
            UT_TYPE_ULONG, 4, &lMallId,
            UT_TYPE_STRING, 128, groupName,
            UT_TYPE_LONG, 4, &lBrandCount
            ))  || 1405==iReturn)
        {   
            iNum++;
            if (1 < iNum) {
                utPltPutLoopVar(psDbHead, "dh", iNum , ",");
            }       
            
            utPltPutLoopVarF(psDbHead,"floorId",iNum,"%lu", lFloorId);
            utPltSetCvtHtml(1);
            utPltPutLoopVarF(psDbHead,"floorName",iNum,"%s",caFloorName);
            utPltPutLoopVarF(psDbHead,"floorDetail",iNum,"%s",caFloorDetail);
            utPltPutLoopVarF(psDbHead,"mallName",iNum,"%s",groupName);
            utPltSetCvtHtml(0);
            utPltPutLoopVarF(psDbHead,"brandCount",iNum,"%lu", lBrandCount);
            utPltPutLoopVarF(psDbHead,"mallId",iNum,"%lu", lMallId);
            
            memset(caFloorName, 0, sizeof(caFloorName));
            memset(caFloorDetail, 0, sizeof(caFloorDetail));
            memset(groupName, 0, sizeof(groupName));            
            lFloorId = lMallId = lBrandCount = 0;
        }
        pasDbCloseCursor(psCur);
    }
  
    //utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"gwdh/dennisShoppingGuiding_list.htm");
    // printf("ncmShoppingGuiding  end!!!!\n");

    return 0;     
}

//购物导航之品牌管理
int ncmBrandManageList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    utPltDbHead *psDbHead;
    char caFloorName[34] = "", caBrandName[34] = "", caShopName[34] = "";
    char caCategoryName[34] = "";
    char caMallId[14] = "", caFloorId[14] = "", caShopId[14] = "";
    unsigned long lFloorId = 0, lMallId = 0, lBrandCount = 0, lEnterTime;
    unsigned long lBrandId = 0, lCategory = 0, lShopId = 0, lFlag = 0;
    char caBrandDescr[130] = "", caTemp_d[130];
    char caActionName[34] = "", caSelSid[1024] = "";
    char caTemp[1024],  sqlCondition[1024];
    char caTotPg[16], caCurPg[16];
    char caPage[12], caLimit[16];
    char caKeyword[129], groupName[130] = "", caImgPath[129] = "";
    unsigned long lCurPg = 0, lRowNum = 0, lStartRec = 0, iLimit = 0;
    unsigned long lCount1 = 0;
    int iReturn=0;
    int iNum=0;
    char *nameUTF;    
    long lCurPid = 0; 

    psDbHead= utPltInitDb();    
    printf("\n enter ncmBrandManageList !!!!\n");
       
#ifdef debug 
    lCurPid = getpid();
    printf("ncmBrandManageList: lCurPid = %d\n", lCurPid);
    utMsgPrintMsg(psMsgHead);     
    //sleep(10);
#endif    

    utMsgGetSomeNVar(psMsgHead, 7,
        "keyword", UT_TYPE_STRING, sizeof(caKeyword)-1, caKeyword,
        "mallId", UT_TYPE_STRING, sizeof(caMallId)-1, caMallId,
        "floorId", UT_TYPE_STRING, sizeof(caFloorId)-1, caFloorId,
        "page", UT_TYPE_STRING, sizeof(caPage)-1, caPage,
        "selsid", UT_TYPE_STRING, sizeof(caSelSid)-1, caSelSid,
        "actionName", UT_TYPE_STRING, sizeof(caActionName)-1, caActionName,        
        "limit", UT_TYPE_STRING, sizeof(caLimit)-1, caLimit);

    if (strlen(caKeyword)){
        pasCvtGBK(2, caKeyword, caTemp_d, sizeof(caKeyword) - 1);
        strcpy(caKeyword, caTemp_d);
    }

    lCurPg = atol(caPage);
    if (lCurPg <= 0) {
        lCurPg = 1;
    }
    if (0 < strlen(caLimit)) {
        lRowNum=atol(caLimit);
    } else{
        lRowNum =40;//每一页面行数
    }
    lStartRec = (lCurPg - 1) * lRowNum; 
    
    lMallId = strtoul(caMallId, NULL, 10);
    
    // 删除
    if(!strcmp("delete", caActionName) && strlen(caSelSid)) {
        snprintf(caTemp, sizeof(caTemp), "delete from ncmbrand where bid in(%s)", caSelSid);        
        pasDbExecSqlF(caTemp);
    }  
    
    printf("ncmBrandManageList: caTemp = %s \n", caTemp);
    //make select condition  
    snprintf(sqlCondition, sizeof(sqlCondition), "from  ncmbrand AS A \
        left join ncmshop AS B on B.id = A.shopid where 1 \
        and A.fid = %s  ",
        caFloorId);
    if (strlen(caKeyword)){
        snprintf(sqlCondition + strlen(sqlCondition), 
            sizeof(sqlCondition) - strlen(sqlCondition), 
            " and (A.name like '%c%s%c' ) ",
            '%', caKeyword, '%');
    }
    //select total count
    snprintf(caTemp, sizeof(caTemp), " select count(*) %s", sqlCondition);

    iReturn = pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
    printf("ncmBrandManageList: lCount = %d, caTemp = %s \n", lCount1, caTemp);
    if (0 == lCount1) { 
        printf( " ncmShoppingGuiding: no floor, please input \
            extracter group name \n " );
        utPltPutVarF(psDbHead, "TotRec", "%lu", lCount1);            
        utPltOutToHtml(iFd, psMsgHead, psDbHead, 
            "gwdh/dennisBrandmanage_list.htm");
        return 1;
    }        
    utPltPutVarF(psDbHead, "TotRec", "%lu", lCount1);
    // select brands
    snprintf(caTemp, sizeof(caTemp), 
        " select A.bid, A.name, A.descr, A.btype, A.shopid, A.flag, A.systime, \
        A.fid, A.imgpath, B.name AS shopName, \
        (select name from ncmcomtype where sid = A.btype) as categoryName, \
        (select name from ncmfloor where fid = A.fid) as floorName \
        %s limit %d, %d ", sqlCondition, lStartRec, lRowNum);    

    psCur = pasDbOpenSql(caTemp,0);
    printf("ncmBrandManageList: caTemp = %s \n", caTemp);
    if(psCur != NULL)
    {
        memset(caFloorName, 0, sizeof(caFloorName));
        memset(caBrandDescr, 0, sizeof(caBrandDescr));
        memset(caBrandName, 0, sizeof(caBrandName));
        memset(caShopName, 0, sizeof(caShopName));
        memset(caCategoryName, 0, sizeof(caCategoryName));
        memset(caImgPath, 0, sizeof(caImgPath));
        lBrandId = lShopId = lFlag = lEnterTime = lCategory = 0;
        
        while(0==(iReturn = pasDbFetchInto(psCur,
            UT_TYPE_ULONG, 4, &lBrandId,
            UT_TYPE_STRING, 32, caBrandName,
            UT_TYPE_STRING, 128, caBrandDescr,
            UT_TYPE_LONG, 4, &lCategory,
            UT_TYPE_ULONG, 4, &lShopId,
            UT_TYPE_LONG, 4, &lFlag,
            UT_TYPE_LONG, 4, &lEnterTime,
            UT_TYPE_ULONG, 4, &lFloorId,
            UT_TYPE_STRING, 128, caImgPath,
            UT_TYPE_STRING, 32, caShopName,
            UT_TYPE_STRING, 32, caCategoryName,
            UT_TYPE_STRING, 32, caFloorName
            ))  || 1405==iReturn)
        {   
            iNum++;
            if (1 < iNum) {
                utPltPutLoopVar(psDbHead, "dh", iNum , ",");
            }             
            utPltPutLoopVarF(psDbHead,"brandId",iNum,"%lu", lBrandId);
            utPltPutLoopVarF(psDbHead,"brandName",iNum,"%s", caBrandName);
            utPltPutLoopVarF(psDbHead,"categoryName",iNum,"%s", caCategoryName);
            utPltPutLoopVarF(psDbHead,"shopName",iNum,"%s", caShopName);
            utPltPutLoopVarF(psDbHead,"floorName",iNum,"%s",caFloorName);
            utPltPutLoopVarF(psDbHead,"flag",iNum,"%lu", lFlag);            
            utPltPutLoopVarF(psDbHead,"brandId",iNum,"%lu", lFlag);
            utPltPutLoopVarF(psDbHead,"enterTime",iNum,"%s", utTimFormat("%Y/%m/%d %H:%M:%S", lEnterTime));
            utPltSetCvtHtml(1);
            utPltPutLoopVarF(psDbHead,"brandDesc",iNum,"%s", caBrandDescr);
            utPltSetCvtHtml(0);
            utPltPutLoopVarF(psDbHead,"mallId",iNum,"%lu", lMallId);
            utPltPutLoopVarF(psDbHead,"categoryId",iNum,"%lu", lCategory);
            utPltPutLoopVarF(psDbHead,"shopId",iNum,"%lu", lShopId);
            utPltPutLoopVarF(psDbHead,"floorId",iNum,"%lu", lFloorId);
            utPltPutLoopVarF(psDbHead,"imgpath",iNum,"%s", caImgPath);

            memset(caFloorName, 0, sizeof(caFloorName));
            memset(caBrandDescr, 0, sizeof(caBrandDescr));
            memset(caBrandName, 0, sizeof(caBrandName));
            memset(caShopName, 0, sizeof(caShopName));
            memset(caCategoryName, 0, sizeof(caCategoryName));
            lBrandId = lShopId = lFlag = lEnterTime = lCategory = 0;
        }
        pasDbCloseCursor(psCur);
    }
  
    //utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"gwdh/dennisBrandmanage_list.htm");
    // printf("ncmBrandManageList  end!!!!\n");

    return 0;
}

int ncmBrandAddOrModify(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    utPltDbHead *psDbHead;
    char caActionName[33] = "", caName[33]="";
    char caDescr[129]="", caImgPath[129]="";
    char caDispImgPath[129]="",imgname[129]="";
    char caFloorId[33]="", caCategoryId[20]="", caBrandId[20]="", caMallId[20]="";
    char lFloorId = 0;
    char caShopid[33] = "", caImgpath1[129]="";
    char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
    char caTemp[1024]="";
    char caTemp2[1024]="";
    unsigned long lSysTime=0;
    char *p;
    time_t curTime = time(0);
    
    unsigned long lCurPid = 0;
    char caShopTemp[4096] = "";
    unsigned long lShopid = 0, lCategoryId = 0, lMallId = 0, lBrandId = 0;
    printf("\n enter ncmBrandAddOrModify !!!!\n");         
#ifdef debug 
    lCurPid = getpid();
    utMsgPrintMsg(psMsgHead);     
    printf("ncmBrandAddOrModify: lCurPid = %d\n", lCurPid);
    //sleep(10);
#endif    

    psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
                utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",(char *)ncLang("0194导入"),(char *)ncLang("0183打开文件出错"));
                return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
    // 建立图片存放路径
    sprintf(caImgPath,"%s/images/ShopImages/brandimages", SHOPIMGPATH);
    if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
                return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);
 
    utMsgGetSomeNVar(psMsgHead,9,
        "actName",UT_TYPE_STRING,  sizeof(caActionName)-1, caActionName,
        "smCateId", UT_TYPE_STRING,  sizeof(caCategoryId)-1, caCategoryId,
        "brandName",UT_TYPE_STRING,  sizeof(caName)-1, caName,
        "brandDesc", UT_TYPE_STRING,  sizeof(caDescr)-1, caDescr,              
        "shopId",UT_TYPE_STRING,  sizeof(caShopid)-1, caShopid,
        "fname", UT_TYPE_STRING,  sizeof(imp_file)-1, imp_file,
        "brandId", UT_TYPE_STRING,  sizeof(caBrandId)-1, caBrandId,
        "mallId", UT_TYPE_STRING,  sizeof(caMallId)-1, caMallId,
        "floorId", UT_TYPE_STRING,  sizeof(caFloorId)-1, caFloorId);
      
    if(0 == strlen(caName)){
        utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","商标名称不能为空!","商标名称不能为空!");
        return 0;    
    }

    lShopid = strtoul(caShopid, NULL, 10);
    lCategoryId = strtoul(caCategoryId, NULL, 10);
    lMallId = strtoul(caMallId, NULL, 10);
    lBrandId = strtoul(caBrandId, NULL, 10);
    lFloorId = strtoul(caFloorId, NULL, 10);
    
    if (0 >= lShopid) {
        utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","必须选择商店!","必须选择商店!");
        return 0;
    }
    if (0 >= lCategoryId) {
        utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","必须选择小类!","必须选择小类!");
        return 0;
    }
                      
    if(strlen(imp_file)!=0) {
        p=utStrSkipSpaces(imp_file);
        p=utStrGetWord(p,temp_file,200,";\n");
        printf("ncmBrandAddOrModify: temp_file = %s\n", temp_file);
        if(strlen(temp_file)==0){
            memset(caImgPath, 0, sizeof(caImgPath));
            memset(caDispImgPath, 0, sizeof(caDispImgPath));
        }else{
            srand((int)curTime);
            sprintf(imgname, "brand_%lu_%lu_%s", time(0), rand(), strrchr(temp_file, '.'));
            sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
            system(temp2_file);
            strcat(caDispImgPath, "/images/ShopImages/brandimages/");
            strcat(caDispImgPath, imgname);    
        }
    }

    if(0 == strcmp(caActionName, "modifyBrand") && 0 < lBrandId) {
        sprintf(caTemp2, "select imgpath from ncmbrand where bid=%lu", lBrandId);
        pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
        if(0 == strlen(caDispImgPath)) { // 是否选择上传图片, 否：原图片不变
            sprintf(caTemp,"update ncmbrand set name='%s', descr='%s', fid=%lu, \
                btype=%lu, cid = %lu, shopid = %lu where bid = %lu ", 
                caName, caDescr, lFloorId, lCategoryId, lMallId, lShopid, lBrandId);
            printf("ncmBrandAddOrModify:caTemp = %s\n", caTemp);
            pasDbExecSqlF(caTemp);
            utPltPutVar(psDbHead,"imgpath",caImgpath1);
        }else{ // 是：原图片删除，新图片加入
            sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
            system(temp3_file);    
            sprintf(caTemp, "update ncmbrand set name='%s', descr='%s', fid=%lu, \
                btype=%lu, cid = %lu, shopid = %lu, imgpath='%s' where bid = %lu ",
                caName, caDescr, lFloorId, lCategoryId, lMallId, lShopid,
                caDispImgPath, lBrandId);
            printf("ncmBrandAddOrModify:caTemp = %s\n", caTemp);
            pasDbExecSqlF(caTemp);
            utPltPutVar(psDbHead, "imgpath", caDispImgPath);
        }
    }else if(0 == strcmp(caActionName, "addBrand")){
        sprintf(caTemp, "insert into ncmbrand (name, descr, fid, btype, cid, \
            shopid, imgpath, systime) values \
            ('%s', '%s', %lu, %lu, %lu, %lu, '%s', %lu)", 
            caName, caDescr, lFloorId, lCategoryId, lMallId, lShopid,
            caDispImgPath, time(0));
        printf("ncmBrandAddOrModify:caTemp = %s\n", caTemp);
        pasDbExecSqlF(caTemp);
        utPltPutVar(psDbHead,"imgpath",caDispImgPath);
    }
      
    utPltPutVar(psDbHead,"mesg","数据加载成功");
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcombrand_dr_list.htm");

    return 0;    
}
//返回html页面，用于浏览器端extjs createwin时向子窗口传参数
int ncmResponHtmlPage(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    return 0;    
}

//将逗号分割的字符拆分成一个数组
static int sepcharbydh2(char *in,char caOut[200][256])
{
        char *p,*p2;
        long lCount;
        long m=0;
        lCount=0;
        if(strlen(in)==0) return 0;
        p=in;
        while(*p){
        //      printf("lCount=%d\n",lCount);
                if(*p==','){
                        caOut[lCount][m]='\0';
                        m=0;
                        p++;
                        lCount++;
                        if(lCount>=200) return lCount;
                }
                else{
                        if(m<256){
                        caOut[lCount][m]=*p;
                        p++;
                        m++;
                  }
                }
        }
        return lCount+1;
}
int ncm_dnsswtssort_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caEtime[6];
    char caStemptime[32];
    char caEtemptime[32];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[20480];
    char caEdate[32];
    char caObj[100][50];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum,num,lCount_servcie;
    
    utPltDbHead *psDbHead;
    int iReturn;
    char caSvalue[128],caSname[128];

    float lRat;
    long lGroupid0,lRowNum,lflags,lStartRec,lCount = 0,lStatus;
    char caName[256];
    unsigned long lStatrow,lSum;
    long lStime,lEtime;
    pasDbCursor *psCur;
 
    char caTable_hz[36];
    char caSdate_d[16],caEdate_d[16];
    char caTable[20],caUdisp[36];
    long starttime;
    char caKeyword[64],caTime_flag[16];
    char caDid[16],caPage[12],caLimit[16],caSort[16];
    char caBbsname[48],caUrl[128];
    char caUid[16];
    char caExport[16], caActionName[33], caSelSid[1024] = "";
    long mCount,lCmonth;
    char caMfrom[48],caMto[48],caType[48];
    long lService;
    char caHost[64];
    char caTemp_d[68];
    char caService[16],caSip[16];
    char caServices[512];
    long lCount_netid;
    long lHj[32];
    lCurPg=1;

    typedef struct sddata_s{
        unsigned long index;
        char dispName[33];
        unsigned long lSum;
        unsigned long lStype;
        int status;
    } S_SDDATA_S;
    S_SDDATA_S pHash[200000];
    pasLHashInfo sHashInfo; 
    unsigned long lSum_did = 0, lDid = 0;
    long lSum_oth;
    long m;
    
    FILE *fp;
    char caFilename[32];
    char caFile[128];
    char caPath[128];

    char caNtype[32];
    long lNtype;
    char dispName[33];
    char tempString[1024], tempString2[1024], *pStr;
    char *delim= ",", *p;

    long nSYear, nEYear, nSMonth, nEMonth, nCntYear, nCntMonth, nCnt, nCurYear;
    char caSTime[32], caETime[32];
    time_t  curTime;
    struct tm *localGMTime;
    char sqlStr1[1024], sqlStr2[20480], sqlStr3[1024], sqlStr4[1024];
    long totalCountOfCurPage, totalCount; 
    int nRet;
    char strService[16];
    long lOneDay = 24 * 3600;
    unsigned long lCurPid = 0, lStype = 0;
    char aStatusStr[][5] = {"正常", "1", "禁用"};
    char caShopids[1024];
    strcpy(caShopids,getDsShopids());
    lSum=0;
    lSum_oth=0;
    char caSwstate[8];
    memset(caSwstate,0,sizeof(caSwstate));
    iReturn = utMsgGetSomeNVar(psMsgHead,13,
                              "did",    UT_TYPE_STRING,10,caDid,
                              "sdate",  UT_TYPE_STRING,19,caSdate,
                              "edate",  UT_TYPE_STRING,19,caEdate,
                              "services",UT_TYPE_STRING,255,caServices,
                              "exp",     UT_TYPE_STRING,10,caExport,
                              "page",    UT_TYPE_STRING,10,caPage,
                              "limit",   UT_TYPE_STRING,10,caLimit,
                              "sort",    UT_TYPE_STRING, 15, caSort,
                              "stime",  UT_TYPE_STRING, 20, caSTime,
                              "etime",  UT_TYPE_STRING, 20, caETime,
                              "actionName",  UT_TYPE_STRING, 32, caActionName,
                              "selsid", UT_TYPE_STRING, sizeof(caSelSid)-1, caSelSid,
                              "swstate",UT_TYPE_STRING,2,caSwstate);
                              

#ifdef debug 
    lCurPid = getpid();
//    utMsgPrintMsg(psMsgHead);     
    printf("ncmBrandAddOrModify: lCurPid = %d\n", lCurPid);
    //sleep(10);
#endif   
 utMsgPrintMsg(psMsgHead);    
    char caOut[200][256];
    long lNum;
    lNum=sepcharbydh2(caSelSid,caOut);
   
    if(!strcmp("dsableUsers", caActionName) && strlen(caSelSid)) {
    	
    	  printf("lNum=%d\n",lNum);
    //	  printf("22pid=%d\n",getpid());
    //		sleep(15);
    	
    	for(i=0;i<lNum;i++){
    	
    	
  		printf("caOut=%s\n",caOut[i]);
    		 ncSrvSetUserStatus(psShmHead, 0,caOut[i], 9);
         ncSrvSetUserStatus(psShmHead, 4100021, caOut[i], 9);
   //      printf("i=%d\n",i);
    	}
    	/*
    	p=strtok(caSelSid, delim);
    	if(p){
        ncSrvSetUserStatus(psShmHead, 0, p, 2);
        ncSrvSetUserStatus(psShmHead, 4100021, p, 2);
      }
        while((p=strtok(NULL,delim))) {
            ncSrvSetUserStatus(psShmHead, 0, p, 2);
            ncSrvSetUserStatus(psShmHead, 4100021, p, 2);
        }   
        */    
    }  
    if(!strcmp("enableUsers", caActionName) && strlen(caSelSid)) {

    	for(i=0;i<lNum;i++){
    		 ncSrvSetUserStatus(psShmHead, 0,caOut[i], 0);
         ncSrvSetUserStatus(psShmHead, 4100021, caOut[i], 0);
    	}
    	/*
    	printf("aa=%s\n",strtok(caSelSid, delim));
    	p=strtok(caSelSid, delim);
    	if(p){
        ncSrvSetUserStatus(psShmHead, 0,p, 0);
        ncSrvSetUserStatus(psShmHead, 4100021, p, 0);
      }
        while((p=strtok(NULL,delim))) {
      printf("p=%s\n",p);
            ncSrvSetUserStatus(psShmHead, 0, p, 0);
            ncSrvSetUserStatus(psShmHead, 4100021, p, 0);
            
        }  
        */     
    }  
           
    strcpy(caServices, "0");
    lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;
    lDid = strtoul(caDid, NULL, 10);
    
    if(!utStrIsSpaces(caExport)){
        lRowNum = 99999;
        lStartRec=0;
    } else{
        if(strlen(caLimit)>0){
            lRowNum=atol(caLimit);
        } else{
           lRowNum =37;//每一页面行数
        }
        lStartRec = (lCurPg - 1) * lRowNum;        
    }
    
    // caculate time, determine the table name  
    nSYear = 0;
    nEYear = 0;
    nSMonth = 0;
    nEMonth = 0;
    bzero(caStemptime, sizeof(caStemptime));
    if (0 < strlen(caSdate)){
        strncpy(caStemptime, caSdate, 10);
        strncat(caStemptime, caSTime, 8);
        lstime = utTimStrToLong("%Y/%m/%d%H:%M:%S", caStemptime);
        localGMTime = localtime(&lstime);
        nSYear = localGMTime->tm_year + 1900;
        nSMonth = localGMTime->tm_mon + 1; 
    }// if (0 < strlen(caSdate)){
    bzero(caEtemptime, sizeof(caEtemptime));
    if(0 < strlen(caEdate)){
        strncpy(caEtemptime, caEdate, 10);
        strncat(caEtemptime, caETime, 8);
        letime = utTimStrToLong("%Y/%m/%d%H:%M:%S", caEtemptime);
        localGMTime = localtime(&letime);
        nEYear = localGMTime->tm_year + 1900;
        nEMonth = localGMTime->tm_mon + 1; 
    }// if(0 < strlen(caEdate)){

//    printf ("ncs_user_log: nEyear = %d, nEYear = %d, nSMonth = %d, nEMonth = %d, \
//        caStemptime=%s, caEtemptime=%s\n", 
  //      nEYear, nEYear, nSMonth, nEMonth, caStemptime, caEtemptime);
    //make table names
    nCntYear = nSYear;
    nCntMonth = nSMonth;
    nCnt = 0;
    curTime = time(NULL);
    localGMTime = localtime(&curTime);
    nCurYear = localGMTime->tm_year + 1900;

    //select records into hash table; 
    sprintf(sqlStr1, "select count(*) as count, dispname from \
            (select  distinct dispname, (starttime DIV %d) as day  from \
            ncsrvuserlog ", lOneDay);
    sprintf(sqlStr2, "  where 1 and dispname != 'Guest'  "); 
    if (lDid) {
        sprintf(sqlStr2 + strlen(sqlStr2)," and  groupid = %lu  ", lDid);
    }
    if(strlen(caShopids)>0){
      	sprintf(sqlStr2+strlen(sqlStr2)," and groupid in (%s) ",caShopids);
     }
    
    sprintf(sqlStr2 + strlen(sqlStr2), "  and ( "); 
    strcpy(sqlStr3, "");
    strcpy(sqlStr4, "");
    if (0 < strlen(caSdate)){
        sprintf(sqlStr3," ( ( starttime >= %d ) ", lstime);
//        sprintf(sqlStr4," ( ( endtime >= %d ) ", lstime);
    }
    else {
        sprintf(sqlStr3," ( 1 ");
//        sprintf(sqlStr4," ( 1 ");
    }
    if(0 < strlen(caEdate)){
        sprintf(sqlStr3 + strlen(sqlStr3)," && ( starttime <= %d ) ) ", letime);
//        sprintf(sqlStr4 + strlen(sqlStr4)," && ( endtime <= %d ) ) ", letime);
    }
    else {
        sprintf(sqlStr3 + strlen(sqlStr3),"  && 1 ) ");
//        sprintf(sqlStr4 + strlen(sqlStr4),"  && 1 ) ");
    }

    sprintf(sqlStr2 + strlen(sqlStr2)," %s ", sqlStr3);
    sprintf(sqlStr2 + strlen(sqlStr2),"  ) ) as temptable where 1 group by \
        dispname order by count desc ");
    
    snprintf(caTemp, sizeof(caTemp), "%s %s", sqlStr1, sqlStr2);                           
    printf("ncm_dnsswtssort_list: lCount = %d, caTemp = %s \n", lCount, caTemp);
    psCur=pasDbOpenSql(caTemp,0);
    iNum = 0;
    long lState=0;
    bzero(pHash, sizeof(pHash));
    if(psCur){
        lSum_did=0;
        lDid=0;
        bzero(dispName, sizeof(dispName));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_LONG,4,&lSum_did,
                                 UT_TYPE_STRING,32, dispName);
        while(iReturn==0||iReturn==1405){
        	lState=ncSrvGetUserStatus(psShmHead, 0,dispName);
        	if(lState==-1){
        		lState=ncSrvGetUserStatus(psShmHead, 4100021,dispName);
        	}
        	if((strlen(caSwstate)==0)||lState==atol(caSwstate))
        		{
        	
            pHash[iNum].index = iNum +  1;
            pHash[iNum].lSum = lSum_did;
            snprintf(pHash[iNum].dispName, sizeof(pHash[iNum].dispName), "%s", 
                dispName);         
            pHash[iNum].status = lState;
   //   printf("lState=%d\n",lState);     
              iNum++; 
            }
                
            lSum_did=0;
            lDid=0;
            bzero(dispName, sizeof(dispName));
            iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_LONG,4,&lSum_did,
                                 UT_TYPE_STRING,32, dispName);
            
              
            
        }// while(iReturn==0||iReturn==1405){
      
        pasDbCloseCursor(psCur);
    }// if(psCur){

     if(0 < strlen(caExport)){
        sprintf(caFilename,"ncsnetidstat%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncsrv");
        sprintf(caFile,"%s/%s",caPath,caFilename);
        fp = fopen(caFile,"w");
        if(fp == NULL) {
            utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",
                ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
            return 0;
        }
        
        fprintf(fp,"序号,用户名称");
        fprintf(fp,",上网天数,状态\n");
    }// if(strlen(caExport)>0){
   
    // put the hash table into template 
    psDbHead = utPltInitDb();
    lStartRec = (lCurPg - 1) * lRowNum;  

    iNum = 0;
    m = 0;
    totalCountOfCurPage = 0;
    totalCount = 0;
    while ( pHash[m].index ) {
        if( m >= lStartRec && iNum < lRowNum ){
             if(iNum>0){
                 utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
             }
             else{
                 utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
             } 
             utPltPutLoopVarF(psDbHead,"num",iNum+1, "%d", iNum + 1);
             utPltPutLoopVarF(psDbHead, "cname", iNum+1, "%s", pHash[m].dispName);
             utPltPutLoopVarF(psDbHead, "lcount", iNum+1, "%lu", pHash[m].lSum);
             utPltPutLoopVarF(psDbHead, "status", iNum+1, "%d", pHash[m].status);
     //        printf("status=%d,dispname=%s,ps=%d\n",pHash[m].status,pHash[m].dispName,ncSrvGetUserStatus(psShmHead, 4100021,pHash[m].dispName));

        unsigned long lStype;
        int status;

             totalCountOfCurPage += pHash[m].lSum;
             iNum++;
        }
        if(0 < (strlen(caExport)) && (NULL != fp)){
            fprintf(fp,"%d", m + 1);
            fprintf(fp,",%s", pHash[m].dispName);
            fprintf(fp,",%d", pHash[m].lSum);
            if (0 == pHash[m].status || 9 == pHash[m].status) {
                fprintf(fp,",%s\n", aStatusStr[pHash[m].status]);
            }
            fprintf(fp,",%d\n", pHash[m].status);
            totalCount += pHash[m].lSum;
        }
        m++;
    }//while ( pHash[m].index ) {
      
    //输出合计
    if(1 < iNum){
        utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
        //utPltPutLoopVarF(psDbHead,"num",iNum+1, "%d", iNum + 1);
        utPltPutLoopVar(psDbHead,"cname",iNum+1,"<font color=red>总计</font>");
        sprintf(tempString, "<font color=red>%d</font>", totalCountOfCurPage);
        utPltPutLoopVar(psDbHead, "lcount", iNum+1, tempString);
        utPltPutLoopVar(psDbHead, "status", iNum+1, "");
       
        if(0 < (strlen(caExport)) && (NULL != fp)){
            fprintf(fp,",%s", "总计");
            fprintf(fp,",%d", totalCount);
        }
    }//if(1 < iNum){

    lTotPg = (m ) / lRowNum;
    utPltPutVarF(psDbHead,"TotRec", "%lu", m);
    if(strlen(caExport)>0){
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);

        return 0;
     }

 // utPltShowDb(psDbHead);

    utPltOutToHtml(iFd,psMsgHead,psDbHead,"report/report_swtssort_list.htm");
    
    return 0;    
}

