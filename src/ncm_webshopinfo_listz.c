#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include "dsutl.h"
#include "dsclt.h"
#include "dlfcn.h"
#include <iconv.h>

#define SHOPIMGPATH "/home/ncmysql/ncsrv/html"
#define UPLOADIMGPATH "/home/ncmysql/ncsrv/upload"

static int ncmWebReplace00(char *pHead,char *pVar,short nType,long lLen,char *pValue)
{
    utPltDbHead *psDbHead;
    psDbHead = (utPltDbHead *)pHead;
    /*向模板数据缓冲区输出变量。
			函数原型
			#include “utoplt01.h”
			int utPltPutVar(utPltDbHead *psDbHead,char *pVarName,char *pValue);
			参数说明
					psDbHead----模板数据缓冲区
			　　pVarName----变量名
			　　pValue------函数的值
			返回值
			　　0 ---- 正确
			　　-1-----出错
    */
    utPltPutVar(psDbHead,pVar,pValue);
    return 0;
}

// ncsWebReplace: 静态页面转换成动态页面
int ncmWebReplace(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead;
    char caPlate[64];
    int iReturn;
    psDbHead = utPltInitDb();
   	iReturn = utMsgGetSomeNVar(psMsgHead, 1,
					"plate",  UT_TYPE_STRING, 63, caPlate);
    if(utStrIsSpaces(caPlate)) {
        return 0;
    }
    /*遍历整个数据包，对数据包中的数据进行指定的处理。
	　　函数原型
	　　　	int utMsgAllRecord(utMsgHead *psMsgHead,char *pHead,int utMsgDoRecord(char *pVar,short nType,long lLen,char *Value));
	　　参数说明
		　　　psMsgHead ----消息的句柄，由函数 utMsgInit产生。
		　　　pHead ----------用于从外部传递数据到函数utMsgDoRecord的指针。
		　　　utMsgDoRecord----对每一条记录进行操作的函数。该函数可由用户指定，要求的调用方式如下：
		　　　int utMsgDoRecord(char *pHead,char *pVar,short nType,long lLen,char *Value);
		　　　其中:
							pHead --- 同上面的pHead
		　　　    pVar ---- 数据包中的变量
		    			nType---- 数据类型
		    			lLen ----- 数据的字节数
		    			Value ---- 数据的内容
		 			当返回值是 0 时，正常，若为非 0, 系统将退出整个函数，并返回返回值。
	　	返回值
		　　　0----正常操作
		　　　！=0 为处理函数的返回值。
    */
    utMsgAllRecord(psMsgHead,(char *)psDbHead,ncmWebReplace00);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
    return 0;
}

int ncTimeToLong(char *stime, unsigned long *lTime)
{
	 char ctime[20] = "";
	 
	 snprintf(ctime, sizeof(ctime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", stime[0], stime[1], stime[2], stime[3], stime[5], stime[6], stime[8], stime[9], stime[11], stime[12], stime[14], stime[15], stime[17], stime[18]);	
	 *lTime = utTimStrToLong("%Y%m%d%H%M%S", ctime);
	 
	 return 0;
}

int ncNumToTime(unsigned long lTime, char *cTime);
/*{
      char day[11] = ""; 
      char hour[11] = "";
      char min[11] = "";
      char secd[11] = "";
      snprintf(day, sizeof(day), "%lu", lTime/86400);
      snprintf(hour, sizeof(hour), "%lu", (lTime%86400)/3600);
      snprintf(min, sizeof(min), "%lu", ((lTime%86400)%3600)/60);
      snprintf(secd, sizeof(secd), "%lu", (((lTime%86400)%3600)%60)/60);
  
      strcat(cTime, strcat(day, "天"));
      strcat(hour, "小时");
      strcat(min, "分");
      strcat(secd, "秒");
      strcat(cTime, hour);
      strcat(cTime, min);
      strcat(cTime, secd); 
       
      return 0;  
}*/

// 楼层信息
int ncm_webreadfloor_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char caFlags[32] = "";
	 char plateid[32] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lFid = 0;
	 unsigned long	lGroupid = 0;
	 char 					caName[32] = "";
	 char 					caImgpath[128] = "";
	 unsigned long  lPlateid = 0;
	 unsigned long	lFlags = 0;
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 2, "plateid",UT_TYPE_STRING, sizeof(long),     &plateid,
	 																"flags", UT_TYPE_STRING, sizeof(caFlags)-1, caFlags);
	 
	 sprintf(sql, "select fid, groupid, name, imgpath, flags from ncmmall where 1=1");
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and flags=%lu ", atol(caFlags));
	 //snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid=%lu ", atol(plateid));
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lFid,
													     UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
													     UT_TYPE_STRING, sizeof(caName)-1,      caName,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
													     //UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags);	
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 utPltPutLoopVarF(psDbHead,"fid",iNum,"%lu",lFid);
			 utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 //utPltPutLoopVarF(psDbHead,"plateid",iNum,"%lu",lPlateid);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 
			 lFid = 0;
			 lGroupid = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lPlateid = 0;
	     lFlags = 0;
	     iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lFid,
															     UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
															     UT_TYPE_STRING, sizeof(caName)-1,      caName,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
															     //UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags);	
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webmall_list.htm");
	  return 0;	
}

// 商品大类
int ncm_webcomgroup_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char groupid[20] = "";
	 char name[32] = "";
	 char descr[128] = "";
	 char systime[32] = "";
	 char imgpath[128] = "";
	 char flags[32] = "";
	 char temp_file[128] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lGroupid = 0;
	 char 					caName[32] = "";
	 char 					caDescr[128] = "";
	 unsigned long	lSystime = 0;
	 char 					caImgpath[128] = "";
	 unsigned long	lFlags = 0;
	 char           s_systime[32] = "";
	 
	 char insert[16] = "";
	 char caInsert[16] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 16,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"groupid",     UT_TYPE_STRING,  sizeof(groupid)-1,        groupid,
			"name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
			"descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
			"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
			"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
			"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"insert",      UT_TYPE_STRING,  10,                       caInsert,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,30);
			strcpy(name,caTemp);
   }
	 if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,30);
			strcpy(descr,caTemp);
   }
   if(strlen(systime)>0){
			pasCvtGBK(2,systime,caTemp,30);
			strcpy(systime,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,30);
			strcpy(imgpath,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"groupid");
	 }
   
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcomgroup where groupid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
				      return 0;	
					}
					
    		  // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					}
					pasDbExecSqlF("delete from ncmcomgroup where groupid in(%s)",caSesid);
    	}
	 }
	 
	 // 查询
	 strcpy(sql, "select count(*) from ncmcomgroup where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select groupid, name, descr, systime, imgpath, flags from ncmcomgroup where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,
			     UT_TYPE_ULONG,  4,               		&lGroupid,
			     UT_TYPE_STRING, sizeof(caName)-1,    caName,
			     UT_TYPE_STRING, sizeof(caDescr)-1,   caDescr,
			     UT_TYPE_ULONG,  4,               		&lSystime,
					 UT_TYPE_STRING, sizeof(caImgpath)-1, caImgpath,
					 UT_TYPE_ULONG,  4,               		&lFlags);	
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
		   if(lSystime>0){
		   		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
		   }else{
		   		strcpy(s_systime, "");	
		   }
		   
		   utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum-1);
			 utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 
			 lGroupid = 0;
			 memset(caName, 0, sizeof(caName));
	     memset(caDescr, 0, sizeof(caDescr));
	     lSystime = 0;
	     memset(caImgpath, 0, sizeof(caImgpath));
	     lFlags = 0;
	     memset(s_systime, 0, sizeof(s_systime));
	     iret = pasDbFetchInto(psCur,
			     UT_TYPE_ULONG,  4,               		&lGroupid,
			     UT_TYPE_STRING, sizeof(caName)-1,    caName,
			     UT_TYPE_STRING, sizeof(caDescr)-1,   caDescr,
			     UT_TYPE_ULONG,  4,               		&lSystime,
					 UT_TYPE_STRING, sizeof(caImgpath)-1, caImgpath,
					 UT_TYPE_ULONG,  4,               		&lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomgroup_list.htm");
	  return 0;	
}

int ncm_webcomgroup_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
//#endif  
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char caSid[32] = "";
	 char caName[32] = "";
	 char caDescr[255] = "";
	 char caImgpath[128] = "";
	 char caKey[32] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 2, "sid", UT_TYPE_STRING, sizeof(caSid)-1, caSid,
	                                "key", UT_TYPE_STRING, sizeof(caKey)-1, caKey);
	 printf("key = %s\n", caKey);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
			 sprintf(sql, "select name, descr, imgpath from ncmcomgroup where groupid=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,31,caName,
		                        UT_TYPE_STRING,254,caDescr,
		                        UT_TYPE_STRING,127,caImgpath);
			  
			  utPltPutVar(psDbHead,"groupid",caSid);
				utPltPutVar(psDbHead,"name",caName);
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVar(psDbHead,"descr",caDescr);
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomgroup_dr.htm");
	 return 0;		
}

// 上传商品大类图片
int ncm_webComGroupImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32]="";
   	char caGroupid[16]="";
    char caName[32]="";
    char caSystime[32]="";
    char caDescr[128]="";
    char caImgPath[128]="";
    char caDispImgPath[128]="";
    char imgname[128]="";
    
    char caImgpath1[128]="";
    char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
 		char caTemp2[1024]="";
 		unsigned long lSysTime=0;
 		unsigned long lMaxid = 0;
 		char *p;
 		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
		
		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0194导入"),ncLang("0183打开文件出错"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// 建立图片存放路径
  	sprintf(caImgPath,"%s/images/ShopImages/groupimages", SHOPIMGPATH);
  	
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        	  printf("建立图片存放路径出错！\n");
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);
    
    // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	  utMsgGetSomeNVar(psMsgHead, 6, "key",      UT_TYPE_STRING,  sizeof(caKey)-1,            caKey,
	                               "groupid",    UT_TYPE_STRING,  sizeof(caGroupid)-1,        caGroupid,
	  														 "name",       UT_TYPE_STRING,  sizeof(caName)-1,           caName,
																 "fname",      UT_TYPE_STRING,  255,                        imp_file,
																 "descr",      UT_TYPE_STRING,  sizeof(caDescr)-1,          caDescr);
	  
	  if(strlen(caName)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","商品名称不能为空!","商品名称不能为空!");
        return 0;	
	  }
	                  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*从指定的字符串中取一个词
		  	
		  	  函数原型：
			　　char *utStrGetWord(char *p,char *pOut,
			　　                   short nMaxLen,char *pTerm);
			　　
					参数说明:
			　　p – 输入字符串
			　　pOut --- 输出字符串，必须要事先分配好空间
			　　nMaxLen --- 输出字符传的最大长度
			　　pTerm  ----- 终止条件的字符串，系统在扫描描时遇到该字符串某一个字符时结束。
			　　
			　　返回值：
			　　结束字符的指针。*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
				printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
					  memset(caDispImgPath, 0, sizeof(caDispImgPath));
				}else{
						sprintf(imgname, "group_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/groupimages/");
				    strcat(caDispImgPath, imgname);	
				}
	  }
	  
	  if(strcmp(caKey, "update")==0){
	  	  sprintf(caTemp2, "select imgpath from ncmcomgroup where groupid=%lu", atol(caGroupid));
	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  if(strlen(caDispImgPath)==0){ // 是否选择上传图片, 否：原图片不变
	  	  	  sprintf(caTemp,"update ncmcomgroup set name='%s',descr='%s',systime=%lu,imgpath='%s' where groupid=%lu",caName,caDescr,time(0),caImgpath1,atol(caGroupid));
			 			printf("caTemp = %s\n", caTemp);
			 			pasDbExecSqlF(caTemp);
			 			utPltPutVar(psDbHead,"imgpath",caImgpath1);
	  	  }else{ // 是：原图片删除，新图片加入
			  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
				  	system(temp3_file);
			  	  
			  	  sprintf(caTemp,"update ncmcomgroup set name='%s',descr='%s',systime=%lu,imgpath='%s' where groupid=%lu",caName,caDescr,time(0),caDispImgPath,atol(caGroupid));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
			  }
	  }else if(strcmp(caKey, "add")==0){
	  	  pasDbOneRecord("select max(groupid) from ncmcomgroup", 0, UT_TYPE_ULONG, 4, &lMaxid);
		    sprintf(caTemp,"insert into ncmcomgroup (groupid,name,descr,systime,imgpath) values (%lu,'%s','%s',%lu,'%s')",(lMaxid+1),caName,caDescr,time(0),caDispImgPath);
			 	printf("caTemp = %s\n", caTemp);
			 	pasDbExecSqlF(caTemp);
			 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
	  }
	  utPltPutVar(psDbHead,"mesg","数据加载成功");
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomgroup_dr_list.htm");
	  //utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","数据加载成功!","数据加载成功!");
    return 0;
}

// 商品小类信息
int ncm_webcomtype_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char sid[20] = "";
	 char name[32] = "";
	 char groupname[32] = "";
	 char descr[128] = "";
	 char systime[32] = "";
	 char imgpath[128] = "";
	 char flags[32] = "";
	 char temp_file[128] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lSid = 0;
	 char 					caName[32] = "";
	 char 					caGroupname[32] = "";
	 char 					caDescr[128] = "";
	 unsigned long	lSystime = 0;
	 char 					caImgpath[128] = "";
	 unsigned long	lFlags = 0;
	 char           s_systime[32] = "";
	 
	 char insert[16] = "";
	 char caInsert[16] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 17,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
			"name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
			"groupname",   UT_TYPE_STRING,  sizeof(groupname)-1,      groupname,
			"descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
			"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
			"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
			"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"insert",      UT_TYPE_STRING,  10,                       caInsert,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,30);
			strcpy(name,caTemp);
   }
   if(strlen(groupname)>0){
			pasCvtGBK(2,groupname,caTemp,30);
			strcpy(groupname,caTemp);
   }
	 if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,30);
			strcpy(descr,caTemp);
   }
   if(strlen(systime)>0){
			pasCvtGBK(2,systime,caTemp,30);
			strcpy(systime,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,30);
			strcpy(imgpath,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
   
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcomtype where sid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
				      return 0;	
					}
					
    		  // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					}
					pasDbExecSqlF("delete from ncmcomtype where sid in(%s)",caSesid);
    	}
	 }
	 // 查询
	 strcpy(sql, "select count(*) from ncmcomtype where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "Select t.sid, t.name, g.name, t.descr, t.systime, t.imgpath, t.flags from ncmcomtype as t left join ncmcomgroup as g on t.groupid = g.groupid where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (t.name like '%%%s%%') ", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by t.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,
			     UT_TYPE_ULONG,  4,               		 &lSid,
			     UT_TYPE_STRING, sizeof(caName)-1,     caName,
			     UT_TYPE_STRING, sizeof(caGroupname)-1,caGroupname,
			     UT_TYPE_STRING, sizeof(caDescr)-1,    caDescr,
			     UT_TYPE_ULONG,  4,               		 &lSystime,
					 UT_TYPE_STRING, sizeof(caImgpath)-1,  caImgpath,
					 UT_TYPE_ULONG,  4,               		 &lFlags);		
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
		   
		   if(lSystime>0){
		   	  strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
		   }else{
		   	  strcpy(s_systime, "");
		   }
		   utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum-1);
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
			 utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 
			 lSid = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caGroupname, 0, sizeof(caGroupname));
			 memset(caDescr, 0, sizeof(caDescr));
	     lSystime = 0;
	     memset(caImgpath, 0, sizeof(caImgpath));
	     lFlags = 0;
	     memset(s_systime, 0, sizeof(s_systime));
	     iret = pasDbFetchInto(psCur,
			     UT_TYPE_ULONG,  4,               		 &lSid,
			     UT_TYPE_STRING, sizeof(caName)-1,     caName,
			     UT_TYPE_STRING, sizeof(caGroupname)-1,caGroupname,
			     UT_TYPE_STRING, sizeof(caDescr)-1,    caDescr,
			     UT_TYPE_ULONG,  4,               		 &lSystime,
					 UT_TYPE_STRING, sizeof(caImgpath)-1,  caImgpath,
					 UT_TYPE_ULONG,  4,               		 &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomtype_list.htm");
	  return 0;	
}

int ncm_webcomtype_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
//#endif  
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char caKey[32] = "";
	 char caSid[32] = "";
	 char caName[32] = "";
	 unsigned long	lGroupid = 0;
	 char caDescr[255] = "";
	 char caImgpath[128] = "";
	 
	 char caGroupTemp[4096] = "";
	 char caGroupname[32] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 2, "key", UT_TYPE_STRING, sizeof(caKey)-1, caKey,
	                                "sid", UT_TYPE_STRING, sizeof(caSid)-1, caSid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
			 sprintf(sql, "select name, groupid, descr, imgpath from ncmcomtype where sid=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,31,caName,
			 											UT_TYPE_LONG,4,&lGroupid,
		                        UT_TYPE_STRING,254,caDescr,
		                        UT_TYPE_STRING,127,caImgpath);
			  
			  utPltPutVar(psDbHead,"sid",caSid);
				utPltPutVar(psDbHead,"name",caName);
				utPltPutVarF(psDbHead,"groupid","%lu",lGroupid);
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVar(psDbHead,"descr",caDescr);
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomtype_dr.htm");
	 return 0;		
}

// 上传商品小类图片
int ncm_webComTypeImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32]="";
   	char caSid[16]="";
    char caName[32]="";
    char caGroupid[32]="";
    char caDescr[128]="";
    char caSystime[32]="";
    char caImgPath[128]="";
    char caDispImgPath[128]="";
    char imgname[128]="";
    
    char caImgpath1[128]="";
    char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
 		char caTemp2[1024]="";
 		unsigned long lSysTime=0;
 		unsigned long lMaxid = 0;
 		char *p;
 		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
		
		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0194导入"),ncLang("0183打开文件出错"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// 建立图片存放路径
  	sprintf(caImgPath,"%s/images/ShopImages/typeimages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);
        
	  utMsgGetSomeNVar(psMsgHead,6,"key",        UT_TYPE_STRING,  sizeof(caKey)-1,            caKey,
	                               "sid",        UT_TYPE_STRING,  sizeof(caSid)-1,            caSid,
	  														 "name",       UT_TYPE_STRING,  sizeof(caName)-1,           caName,
	  														 "grid",       UT_TYPE_STRING,  sizeof(caGroupid)-1,        caGroupid,
																 "fname",      UT_TYPE_STRING,  255,                        imp_file,
																 "descr",      UT_TYPE_STRING,  sizeof(caDescr)-1,          caDescr);
	  
	  if(strlen(caName)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","商品小类名称不能为空!","商品小类名称不能为空!");
        return 0;	
	  }
	                  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*从指定的字符串中取一个词
		  	
		  	  函数原型：
			　　char *utStrGetWord(char *p,char *pOut,
			　　                   short nMaxLen,char *pTerm);
			　　
					参数说明:
			　　p – 输入字符串
			　　pOut --- 输出字符串，必须要事先分配好空间
			　　nMaxLen --- 输出字符串的最大长度
			　　pTerm  ----- 终止条件的字符串，系统在扫描描时遇到该字符串某一个字符时结束。
			　　
			　　返回值：
			　　结束字符的指针。*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
				printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
				}else{
						sprintf(imgname, "type_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/typeimages/");
				    strcat(caDispImgPath, imgname);	
				}
	  }
	  
	  if(strcmp(caKey, "update")==0){
	  	  sprintf(caTemp2, "select imgpath from ncmcomtype where sid=%lu", atol(caSid));
	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  if(strlen(caDispImgPath)==0){ // 是否选择上传图片, 否：原图片不变
	  	  	  sprintf(caTemp,"update ncmcomtype set name='%s',groupid=%lu,descr='%s',systime=%lu,imgpath='%s' where sid=%lu",caName,atol(caGroupid),caDescr,time(0),caImgpath1,atol(caSid));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caImgpath1);
	  	  }else{ // 是：原图片删除，新图片加入
			  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
				  	system(temp3_file);
			  	  
			  	  sprintf(caTemp,"update ncmcomtype set name='%s',groupid=%lu,descr='%s',systime=%lu,imgpath='%s' where sid=%lu",caName,atol(caGroupid),caDescr,time(0),caDispImgPath,atol(caSid));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
			  }
	  }else if(strcmp(caKey, "add")==0){
				pasDbOneRecord("select max(sid) from ncmcomtype", 0, UT_TYPE_ULONG, 4, &lMaxid);
		    sprintf(caTemp,"insert into ncmcomtype (sid,name,groupid,descr,systime,imgpath) values (%lu,'%s',%lu,'%s',%lu,'%s')",(lMaxid+1),caName,atol(caGroupid),caDescr,time(0),caDispImgPath);
			 	printf("caTemp = %s\n", caTemp);
			 	pasDbExecSqlF(caTemp);
			 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
	  }
	  
	  utPltPutVar(psDbHead,"mesg","数据加载成功");
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomtype_dr_list.htm");
	  
    return 0;
}

// 广告信息
int ncm_webcomindex_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char sid[20] = "";
	 char name[32] = "";
	 char sgroupname[32] = "";
	 char descr[128] = "";
	 char imgpath[128] = "";
	 char serno[32] = "";
	 char floor[32] = "";
	 char level[20] = "";
	 char plevel[20] = "";
	 char type[20] = "";
	 char systime[32] = "";
	 char shopname[32] = "";
	 char plateid[32] = "";
	 char flags[32] = "";
	 char temp_file[128] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lSid = 0;
	 char 					caName[32] = "";
	 char	          caSgroupname[32] = "";
	 char 					caDescr[128] = "";
	 char 					caImgpath[128] = "";
	 char 					caSerno[32] = "";
	 char 					caFloor[32] = "";
	 unsigned long	lLevel = 0;
	 unsigned long	lPlevel = 0;
	 unsigned long	lType = 0;
	 char           caType[32] = "";
	 unsigned long	lSystime = 0;
	 char	          caShopname[32] = "";
	 unsigned long	lPlateid = 0;
	 unsigned long	lFlags = 0;
	 char           caScode[16] = "";
	 char           s_systime[32] = "";
	 
	 char insert[16] = "";
	 char caInsert[16] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
   
   unsigned long lShopId = 0;
	 dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 25,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
																	"name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
																	"sgroupname",  UT_TYPE_STRING,  sizeof(sgroupname)-1,     sgroupname,
																	"descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"serno",       UT_TYPE_STRING,  sizeof(serno)-1,          serno,
																	"floor",       UT_TYPE_STRING,  sizeof(floor)-1,          floor,
																	"level",       UT_TYPE_STRING,  sizeof(level)-1,          level,
																	"plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
																	"type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
																	"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
																	"shopname",    UT_TYPE_STRING,  sizeof(shopname)-1,       shopname,
																	"plateid",     UT_TYPE_STRING,  sizeof(plateid)-1,        plateid,
																	"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"insert",      UT_TYPE_STRING,  10,                       caInsert,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"update",      UT_TYPE_STRING,  10,                       caUpdate,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport);
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,30);
			strcpy(name,caTemp);
   }
   if(strlen(sgroupname)>0){
			pasCvtGBK(2,sgroupname,caTemp,30);
			strcpy(sgroupname,caTemp);
   }
	 if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,30);
			strcpy(descr,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,30);
			strcpy(imgpath,caTemp);
   }
   if(strlen(serno)>0){
			pasCvtGBK(2,serno,caTemp,30);
			strcpy(serno,caTemp);
   }
   if(strlen(floor)>0){
			pasCvtGBK(2,floor,caTemp,30);
			strcpy(floor,caTemp);
   }
   if(strlen(systime)>0){
			pasCvtGBK(2,systime,caTemp,30);
			strcpy(systime,caTemp);
   }
   if(strlen(shopname)>0){
			pasCvtGBK(2,shopname,caTemp,30);
			strcpy(shopname,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
   
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcomindex where sid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
				      return 0;	
					}
					
    		  // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					}
					pasDbExecSqlF("delete from ncmcomindex where sid in(%s)",caSesid);
    	}
	 } 
	 
	 // 查询
	 strcpy(sql, "select count(*) from ncmcomindex where 1=1 and type=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // 根据模板查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %lu", atol(plateid));
	 }
	 if(lShopId>0) // 根据商店查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and shopid = %lu", lShopId);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select i.sid, i.name, t.name, i.descr, i.imgpath, i.serno, i.floor, i.level, i.plevel, i.type, i.systime, s.name, i.plateid, i.flags, s.service_code from ncmcomindex i left join ncmcomtype t on i.sgroupid = t.sid left join ncmshop s on i.shopid = s.id  where 1=1 and i.type=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (i.name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(plateid)) // 根据模板查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and i.plateid = %lu", atol(plateid));
	 }
	 if(lShopId>0) // 根据商店查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and i.shopid = %lu", lShopId);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by i.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
													     UT_TYPE_STRING, sizeof(caName)-1,      caName,
													     UT_TYPE_STRING, sizeof(caSgroupname)-1,caSgroupname,
													     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
													     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
													     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
													     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lType,
													     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
													     UT_TYPE_STRING, sizeof(caShopname)-1,  caShopname,
													     UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags,
															 UT_TYPE_STRING, sizeof(caScode)-1,     caScode);	
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
		   
		   if(lSystime>0){
		   		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
		   }else{
		   		strcpy(s_systime, "");	
		   }
		   
			 switch(lType){
					case 1:strcpy(caType, "广告");break;
					case 2:strcpy(caType, "品牌");break;
					case 3:strcpy(caType, "促销");break;
					case 4:strcpy(caType, "视频");break;
					default:break;	
			 }
			 
			 utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"sgroupname",iNum,caSgroupname);
			 utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"serno",iNum,caSerno);
			 utPltPutLoopVar(psDbHead,"floor",iNum,caFloor);
			 utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",lLevel);
			 utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",lPlevel);
			 utPltPutLoopVarF(psDbHead,"type",iNum,caType);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVarF(psDbHead,"shopname",iNum,caShopname);
			 utPltPutLoopVarF(psDbHead,"plateid",iNum,"%lu",lPlateid);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltPutLoopVar(psDbHead,"scode",iNum,caScode);
			 
			 lSid = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caSgroupname, 0, sizeof(caSgroupname));
			 memset(caDescr, 0, sizeof(caDescr));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 memset(caSerno, 0, sizeof(caSerno));
			 memset(caFloor, 0, sizeof(caFloor));
	     lLevel = 0;
	     lPlevel = 0;
	     lType = 0;
	     memset(caType, 0, sizeof(caType));
	     lSystime = 0;
	     memset(caShopname, 0, sizeof(caShopname));
	     lPlateid = 0;
	     lFlags = 0;
	     memset(caScode, 0, sizeof(caScode));
	     memset(s_systime, 0, sizeof(s_systime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
															     UT_TYPE_STRING, sizeof(caName)-1,      caName,
															     UT_TYPE_STRING, sizeof(caSgroupname)-1,caSgroupname,
															     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
															     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
															     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
															     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lType,
															     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
															     UT_TYPE_STRING, sizeof(caShopname)-1,  caShopname,
															     UT_TYPE_ULONG,  sizeof(long),          &lPlateid,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags,
																	 UT_TYPE_STRING, sizeof(caScode)-1,     caScode);	
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex_list.htm");
	  return 0;	
}

int ncm_webcomindex_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
//#endif  
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iFlag=0;
	 int level=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char           caKey[32] = "";
	 char           caSid[32] = "";
	 char           caPlateid[32] = "";
	 char 					caName[32] = "";
	 unsigned long	lSgroupid = 0;
	 char 					caImgpath[128] = "";
	 char 					caSerno[32] = "";
	 char 					caFloor[32] = "";
	 unsigned long  lLevel = 0;
	 unsigned long  lPlevel = 0;
	 unsigned long  lType = 0;
	 unsigned long  lShopid = 0;
	 unsigned long  lPlateid = 0;
	 char           caDescr[255] = "";
	 
	 unsigned long lShopId = 0;
	 dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 3, "key",     UT_TYPE_STRING, sizeof(caKey)-1,     caKey,
	                                "sid",     UT_TYPE_STRING, sizeof(caSid)-1,     caSid,
	                                "plateid", UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0&&strlen(caPlateid)>0){
	 	   iFlag = 1;
			 sprintf(sql, "select name, sgroupid, descr, imgpath, serno, floor, level, plevel, type, shopid, plateid from ncmcomindex where sid=%lu and plateid=%lu", atol(caSid),atol(caPlateid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,31,caName,
		                        UT_TYPE_LONG,4,&lSgroupid,
		                        UT_TYPE_STRING,254,caDescr,
		                        UT_TYPE_STRING,127,caImgpath,
		                        UT_TYPE_STRING,31,caSerno,
		                        UT_TYPE_STRING,31,caFloor,
		                        UT_TYPE_LONG,4,&lLevel,
		                        UT_TYPE_LONG,4,&lPlevel,
		                        UT_TYPE_LONG,4,&lType,
		                        UT_TYPE_LONG,4,&lShopid,
		                        UT_TYPE_LONG,4,&lPlateid);
			  
			  utPltPutVar(psDbHead,"key",caKey);
			  utPltPutVar(psDbHead,"sid",caSid);
				utPltPutVar(psDbHead,"name",caName);
		    utPltPutVarF(psDbHead,"typeid","%lu",lSgroupid);
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVar(psDbHead,"descr",caDescr);
		    utPltPutVar(psDbHead,"serno",caSerno);
		    utPltPutVar(psDbHead,"floor",caFloor);
		    utPltPutVarF(psDbHead,"level","%lu",lLevel);
		    utPltPutVarF(psDbHead,"plevel","%lu",lPlevel);
		    utPltPutVarF(psDbHead,"type","%lu",lType);
		    utPltPutVarF(psDbHead,"shopid","%lu",lShopid);
		    utPltPutVarF(psDbHead,"plateid","%lu",atol(caPlateid));
		    
		    utPltPutVarF(psDbHead,"flag","%d",iFlag);
	 }else if(strcmp(caKey, "add")==0&&strlen(caPlateid)>0){
	 	    utPltPutVar(psDbHead,"key",caKey);
	 	    utPltPutVarF(psDbHead,"plateid","%lu",atol(caPlateid));
	 	    utPltPutVarF(psDbHead,"flag","%d",iFlag);
	 	    if(lShopId>0){
	 	    	 level = 5;
	 	    	 utPltPutVarF(psDbHead,"level","%lu",level);
	 	    }else{
	 	    	 utPltPutVarF(psDbHead,"level","%lu",level);
	 	    }
	 }
	 
	 //utPltShowDb(psDbHead);
	 if(atol(caPlateid)==1){
	 			utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex_dr.htm");	
	 }else if(atol(caPlateid)==2){
	 		  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex_dr1.htm");
	 }else if(atol(caPlateid)==3){
	 			utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex_dr2.htm");	
	 }else{
	 			utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex_dr3.htm");	
	 }
	 
	 return 0;		
}

int ncm_webcomindex_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  
	  int  iFlag=0;
	  char caPos[32]="";
	  char caImgPath[128]="";
	  char caDispImgPath[128]="";
	  char imgname[128]="";
	  char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
 		char caTemp[1024]="";
	  char *p=NULL;
    
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0194导入"),ncLang("0183打开文件出错"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// 建立图片存放路径
  	sprintf(caImgPath,"%s/images/ShopImages/indeximages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);

	  utMsgGetSomeNVar(psMsgHead,2,"fileField", UT_TYPE_STRING, 255, imp_file,
	  														 "position",  UT_TYPE_STRING, sizeof(caPos)-1, caPos);
	  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*从指定的字符串中取一个词
		  	
		  	  函数原型：
			　　char *utStrGetWord(char *p,char *pOut,
			　　                   short nMaxLen,char *pTerm);
			　　
					参数说明:
			　　p – 输入字符串
			　　pOut --- 输出字符串，必须要事先分配好空间
			　　nMaxLen --- 输出字符传的最大长度
			　　pTerm  ----- 终止条件的字符串，系统在扫描描时遇到该字符串某一个字符时结束。
			　　
			　　返回值：
			　　结束字符的指针。*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
		  	printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
				}else{
						sprintf(imgname, "index_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/indeximages/");
				    strcat(caDispImgPath, imgname);	
				}
		}
		
		utPltPutVarF(psDbHead,"flag","%d",iFlag);
		utPltPutVar(psDbHead,"position",caPos);
	  utPltPutVar(psDbHead,"filepath",caDispImgPath);
	  printf("position = %s\n", caPos);
	  printf("imgpath = %s\n", caDispImgPath);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomindex_closeFile.htm");
    return 0;
}

// 上传广告图片
int ncm_webcomindex_update_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead = utPltInitDb();
    
    char caKey[32]="";
   	char caSid[16]="";
    char caName[32]="";
    char caSgroupid[32] = "";
    char caDescr[128]="";
    char caImgpath[128]="";
    char caSerno[32]="";
    char caFloor[32]="";
    char caLevel[20]="";
    char caPlevel[20]="";
    char caType[20]="";
    char caShopid[32] = "";
    char caPlateid[16] = "";
    
 		char caTemp[1024]="";
 		char caTemp1[1024]="";
 		char caTemp2[1024]="";
 		char caTemp3[1024]="";
 		char caImgpath1[128]="";
 		char temp_file[129]="";
 		unsigned long lCount = 0;
 		unsigned long lMaxid = 0;
 		
	  utMsgGetSomeNVar(psMsgHead,13,"key",       UT_TYPE_STRING,  sizeof(caKey)-1,            caKey,
	                               "sid",        UT_TYPE_STRING,  sizeof(caSid)-1,            caSid,
	                               "name",       UT_TYPE_STRING,  sizeof(caName)-1,           caName,
	  														 "typeid",     UT_TYPE_STRING,  sizeof(caSgroupid)-1,       caSgroupid,
	  														 "serno",      UT_TYPE_STRING,  sizeof(caSerno)-1,          caSerno,
	  														 "floor",      UT_TYPE_STRING,  sizeof(caFloor)-1,          caFloor,
	  														 "level",      UT_TYPE_STRING,  sizeof(caLevel)-1,          caLevel,
	  														 "plevel",     UT_TYPE_STRING,  sizeof(caPlevel)-1,         caPlevel,
	  														 "type",       UT_TYPE_STRING,  sizeof(caType)-1,           caType,
	  														 "shopid",     UT_TYPE_STRING,  sizeof(caShopid)-1,         caShopid,
	  														 "plateid",    UT_TYPE_STRING,  sizeof(caPlateid)-1,        caPlateid,
																 "filepath",   UT_TYPE_STRING,  sizeof(caImgpath)-1,        caImgpath,
																 "descr",      UT_TYPE_STRING,  sizeof(caDescr)-1,          caDescr);
	  
	  if(strlen(caName)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","商标名称不能为空!","商标名称不能为空!");
        return 0;	
	  }
	  
	  if(strcmp(caType, "广告")==0){
	  		strcpy(caType, "1");	
	  }else if(strcmp(caType, "品牌")==0){
	  		strcpy(caType, "2");	
	  }else if(strcmp(caType, "促销")==0){
	  		strcpy(caType, "3");	
	  }else if(strcmp(caType, "视频")==0){
	  		strcpy(caType, "4");	
	  }else{
	  	  strcpy(caType, "1");	
	  }
	  
	  sprintf(caTemp3, "select floor from ncmshop where id = %lu", atol(caShopid));
	  pasDbOneRecord(caTemp3,0,UT_TYPE_STRING,sizeof(caFloor)-1,caFloor);
	  
	  sprintf(caTemp1, "select count(*) from ncmcomindex where level=%d and shopid=%lu", atol(caLevel), atol(caShopid));
	  pasDbOneRecord(caTemp1,0,UT_TYPE_LONG,4,&lCount);                
	  if(strcmp(caKey, "add")==0){
	  	  if(lCount){
	  	  	  // 删除原位置的图片
	  	  	  sprintf(caTemp2, "select imgpath from ncmcomindex where level=%lu and shopid=%lu", atol(caLevel),atol(caShopid));
	  	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  	  sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
				  	system(temp_file);
				  	
	  	  	  sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu where level=%lu and plateid=%lu and shopid=%lu",caName,atol(caSgroupid),caDescr,caImgpath,caSerno,caFloor,atol(caPlevel),atol(caType),time(0),atol(caShopid),atol(caPlateid),atol(caLevel),atol(caPlateid),atol(caShopid));	
			  		printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }else{
	  	  	  pasDbOneRecord("select max(sid) from ncmcomindex", 0, UT_TYPE_ULONG, 4, &lMaxid);
				    sprintf(caTemp,"insert into ncmcomindex (sid,name,sgroupid,descr,imgpath,serno,floor,level,plevel,type,systime,shopid,plateid) values (%lu,'%s',%lu,'%s','%s','%s','%s',%lu,%lu,%lu,%lu,%lu,%lu)",(lMaxid+1),caName,atol(caSgroupid),caDescr,caImgpath,caSerno,caFloor,atol(caLevel),atol(caPlevel),atol(caType),time(0),atol(caShopid),atol(caPlateid));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }
	  }else if(strcmp(caKey, "update")==0){
	  	  if(lCount){
	  	  	  // 删除原位置的图片
	  	  	  sprintf(caTemp2, "select imgpath from ncmcomindex where level=%lu and shopid=%lu", atol(caLevel),atol(caShopid));
	  	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  	  if(strcmp(caImgpath, caImgpath1) != 0){
			  	  	  sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
						  	system(temp_file);
				    }
				  	
	  	  	  sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu where level=%lu and plateid=%lu and shopid=%lu",caName,atol(caSgroupid),caDescr,caImgpath,caSerno,caFloor,atol(caPlevel),atol(caType),time(0),atol(caShopid),atol(caPlateid),atol(caLevel),atol(caPlateid),atol(caShopid));	
			  		printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }else{
	  	  		sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', level=%lu, plevel=%lu, type=%lu, systime=%lu, shopid=%lu, plateid=%lu where sid=%lu and plateid=%lu",caName,atol(caSgroupid),caDescr,caImgpath,caSerno,caFloor,atol(caLevel),atol(caPlevel),atol(caType),time(0),atol(caShopid),atol(caPlateid),atol(caSid),atol(caPlateid));	
			  		printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }
	  }
	  utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","数据加载成功!","数据加载成功!");
    return 0;
}

// 品牌信息
int ncm_webcombrand_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char sid[20] = "";
	 char name[32] = "";
	 char sgroupname[32] = "";
	 char descr[128] = "";
	 char imgpath[128] = "";
	 char serno[32] = "";
	 char floor[32] = "";
	 char level[20] = "";
	 char plevel[20] = "";
	 char type[20] = "";
	 char systime[32] = "";
	 char shopid[32] = "";
	 char shopname[32] = "";
	 char flags[32] = "";
	 char temp_file[128] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lSid = 0;
	 char 					caName[32] = "";
	 char 					caSgroupname[32] = "";
	 char 					caDescr[128] = "";
	 char 					caImgpath[128] = "";
	 char 					caSerno[32] = "";
	 char 					caFloor[32] = "";
	 unsigned long	lLevel = 0;
	 unsigned long	lPlevel = 0;
	 unsigned long	lType = 0;
	 unsigned long	lSystime = 0;
	 unsigned long	lShopid = 0;
	 char	          caShopname[32] = "";
	 unsigned long	lFlags = 0;
	 char           s_type[32] = "";
	 char           s_systime[32] = "";
	 
	 char insert[16] = "";
	 char caInsert[16] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 unsigned long lSysTime = 0;
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 24,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
			"name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
			"sgroupname",  UT_TYPE_STRING,  sizeof(sgroupname)-1,     sgroupname,
			"descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
			"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
			"serno",       UT_TYPE_STRING,  sizeof(serno)-1,          serno,
			"floor",       UT_TYPE_STRING,  sizeof(floor)-1,          floor,
			"level",       UT_TYPE_STRING,  sizeof(level)-1,          level,
			"plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
			"type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
			"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
			"shopid",      UT_TYPE_STRING,  sizeof(shopid)-1,         shopid,
			"shopname",    UT_TYPE_STRING,  sizeof(shopname)-1,       shopname,
			"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"insert",      UT_TYPE_STRING,  10,                       caInsert,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,30);
			strcpy(name,caTemp);
   }
   if(strlen(sgroupname)>0){
			pasCvtGBK(2,sgroupname,caTemp,30);
			strcpy(sgroupname,caTemp);
   }
	 if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,30);
			strcpy(descr,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,30);
			strcpy(imgpath,caTemp);
   }
   if(strlen(serno)>0){
			pasCvtGBK(2,serno,caTemp,30);
			strcpy(serno,caTemp);
   }
   if(strlen(floor)>0){
			pasCvtGBK(2,floor,caTemp,30);
			strcpy(floor,caTemp);
   }
   if(strlen(systime)>0){
			pasCvtGBK(2,systime,caTemp,30);
			strcpy(systime,caTemp);
   }
   if(strlen(shopname)>0){
			pasCvtGBK(2,shopname,caTemp,30);
			strcpy(shopname,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
   
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	memset(caTemp, 0, sizeof(caTemp));
    	char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcomindex where sid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
				      return 0;	
					}
					
					// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
					iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmcomindex where sid in(%s)",caSesid);
    	}
	 } 
	 // 查询
	 strcpy(sql, "select count(*) from ncmcomindex where 1=1 and type=2");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(shopid)) // 根据商店查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and shopid = %lu ", atol(shopid));
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %lu\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select i.sid, i.name, t.name, i.descr, i.imgpath, i.serno, i.floor, i.level, i.plevel, i.type, i.systime, i.shopid, s.name, i.flags from ncmcomindex i left join ncmcomtype t on i.sgroupid = t.sid left join ncmshop s on i.shopid = s.id  where 1=1 and i.type=2");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (i.name like '%%%s%%') ", _keyword);
	 }
	 if(!utStrIsSpaces(shopid)) // 根据商店查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and shopid = %lu ", atol(shopid));
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by i.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
													     UT_TYPE_STRING, sizeof(caName)-1,      caName,
													     UT_TYPE_STRING, sizeof(caSgroupname)-1,caSgroupname,
													     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
													     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
													     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
													     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
													     UT_TYPE_ULONG,  sizeof(long),          &lType,
													     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
													     UT_TYPE_ULONG,  sizeof(long),          &lShopid,
													     UT_TYPE_STRING, sizeof(caShopname)-1,  caShopname,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
		   
		   if(lSystime>0){
		   		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
		   }else{
		   	  strcpy(s_systime, "");
		   }
		   
			 switch(lType){
					case 1:strcpy(s_type, "广告");break;
					case 2:strcpy(s_type, "品牌");break;
					case 3:strcpy(s_type, "促销");break;
					case 4:strcpy(s_type, "视频");break;
					default:break;	
			 }
			 
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"sgroupname",iNum,caSgroupname);
			 utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"serno",iNum,caSerno);
			 utPltPutLoopVar(psDbHead,"floor",iNum,caFloor);
			 utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",lLevel);
			 utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",lPlevel);
			 utPltPutLoopVarF(psDbHead,"type",iNum,s_type);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu",lShopid);
			 utPltPutLoopVar(psDbHead,"shopname",iNum,caShopname);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 
			 lSid = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caSgroupname, 0, sizeof(caSgroupname));
			 memset(caDescr, 0, sizeof(caDescr));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 memset(caSerno, 0, sizeof(caSerno));
			 memset(caFloor, 0, sizeof(caFloor));
			 lLevel = 0;
			 lPlevel = 0;
			 lType = 0;
			 lSystime = 0;
			 lShopid = 0;
			 memset(caShopname, 0, sizeof(caShopname));
			 lFlags = 0;
			 memset(s_type, 0, sizeof(s_type));
			 memset(s_systime, 0, sizeof(s_systime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
															     UT_TYPE_STRING, sizeof(caName)-1,      caName,
															     UT_TYPE_STRING, sizeof(caSgroupname)-1,caSgroupname,
															     UT_TYPE_STRING, sizeof(caDescr)-1,     caDescr,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
															     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
															     UT_TYPE_STRING, sizeof(caFloor)-1,     caFloor,
															     UT_TYPE_ULONG,  sizeof(long),          &lLevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lPlevel,
															     UT_TYPE_ULONG,  sizeof(long),          &lType,
															     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
															     UT_TYPE_ULONG,  sizeof(long),          &lShopid,
															     UT_TYPE_STRING, sizeof(caShopname)-1,  caShopname,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcombrand_list.htm");
	  return 0;	
}

int ncm_webcombrand_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
//#endif  
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char           caKey[32] = "";
	 char           caSid[32] = "";
	 char 					caName[32] = "";
	 unsigned long	lSgroupid = 0;
	 char 					caImgpath[128] = "";
	 char 					caSerno[32] = "";
	 char 					caFloor[32] = "";
	 unsigned long  lLevel = 0;
	 unsigned long  lPlevel = 0;
	 unsigned long  lType = 0;
	 unsigned long  lShopid = 0;
	 char           caDescr[255] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 2, "key", UT_TYPE_STRING, sizeof(caKey)-1, caKey,
	                                "sid", UT_TYPE_STRING, sizeof(caSid)-1, caSid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
			 sprintf(sql, "select name, sgroupid, descr, imgpath, serno, floor, level, plevel, type, shopid from ncmcomindex where sid=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,31,caName,
		                        UT_TYPE_LONG,4,&lSgroupid,
		                        UT_TYPE_STRING,254,caDescr,
		                        UT_TYPE_STRING,127,caImgpath,
		                        UT_TYPE_STRING,31,caSerno,
		                        UT_TYPE_STRING,31,caFloor,
		                        UT_TYPE_LONG,4,&lLevel,
		                        UT_TYPE_LONG,4,&lPlevel,
		                        UT_TYPE_LONG,4,&lType,
		                        UT_TYPE_LONG,4,&lShopid);
			  
			  utPltPutVar(psDbHead,"sid",caSid);
				utPltPutVar(psDbHead,"name",caName);
				utPltPutVarF(psDbHead,"sgroupid","%d",lSgroupid);
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVar(psDbHead,"descr",caDescr);
		    utPltPutVar(psDbHead,"serno",caSerno);
		    utPltPutVar(psDbHead,"floor",caFloor);
		    utPltPutVarF(psDbHead,"level","%d",lLevel);
		    utPltPutVarF(psDbHead,"plevel","%d",lPlevel);
		    utPltPutVarF(psDbHead,"type","%d",lType);
		    utPltPutVarF(psDbHead,"shopid","%d",lShopid);
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcombrand_dr.htm");
	 return 0;		
}

// 上传品牌信息图片
int ncm_webComBrandImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32] = "";
   	char caSid[16] = "";
    char caName[32]="";
    char caSgroupid[32] = "";
    char caDescr[128]="";
    char caImgPath[128]="";
    char caDispImgPath[128]="";
    char imgname[128]="";
    char caSerno[32]="";
    char caFloor[32]="";
    char caLevel[20]="";
    char caPlevel[20]="";
    char caType[20]="";
    char caShopid[32] = "";
    
    char caImgpath1[128]="";
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
 		
 		unsigned long lMaxid = 0;
		char caShopTemp[4096] = "";
		unsigned long lShopid = 0;
 		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
		
		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0194导入"),ncLang("0183打开文件出错"));
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
        
	  utMsgGetSomeNVar(psMsgHead,12,"key",        UT_TYPE_STRING,  sizeof(caKey)-1,           caKey,
	  														 "sid",        UT_TYPE_STRING,  sizeof(caSid)-1,            caSid,
	                               "name",       UT_TYPE_STRING,  sizeof(caName)-1,           caName,
	  														 "tyid",       UT_TYPE_STRING,  sizeof(caSgroupid)-1,       caSgroupid,
	  														 "serno",      UT_TYPE_STRING,  sizeof(caSerno)-1,          caSerno,
	  														 "floor",      UT_TYPE_STRING,  sizeof(caFloor)-1,          caFloor,
	  														 "level",      UT_TYPE_STRING,  sizeof(caLevel)-1,          caLevel,
	  														 "plevel",     UT_TYPE_STRING,  sizeof(caPlevel)-1,         caPlevel,
	  														 "type",       UT_TYPE_STRING,  sizeof(caType)-1,           caType,
	  														 "shid",       UT_TYPE_STRING,  sizeof(caShopid)-1,         caShopid,
																 "fname",      UT_TYPE_STRING,  255,                        imp_file,
																 "descr",      UT_TYPE_STRING,  sizeof(caDescr)-1,          caDescr);
	  
	  if(strlen(caName)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","商品小类名称不能为空!","商品小类名称不能为空!");
        return 0;	
	  }
	                  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*从指定的字符串中取一个词
		  	
		  	  函数原型：
			　　char *utStrGetWord(char *p,char *pOut,
			　　                   short nMaxLen,char *pTerm);
			　　
					参数说明:
			　　p – 输入字符串
			　　pOut --- 输出字符串，必须要事先分配好空间
			　　nMaxLen --- 输出字符传的最大长度
			　　pTerm  ----- 终止条件的字符串，系统在扫描描时遇到该字符串某一个字符时结束。
			　　
			　　返回值：
			　　结束字符的指针。*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
				printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
					  memset(caDispImgPath, 0, sizeof(caDispImgPath));
				}else{
						sprintf(imgname, "brand_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/brandimages/");
				    strcat(caDispImgPath, imgname);	
				}
	  }
	  
	  sprintf(caShopTemp, "select floor from ncmshop where id=%lu", atol(caShopid));
	  pasDbOneRecord(caShopTemp,0,UT_TYPE_STRING,sizeof(caFloor)-1,caFloor);
	  
	  if(strcmp(caType, "广告")==0){
	  		strcpy(caType, "1");	
	  }else if(strcmp(caType, "品牌")==0){
	  		strcpy(caType, "2");	
	  }else if(strcmp(caType, "促销")==0){
	  		strcpy(caType, "3");	
	  }else if(strcmp(caType, "视频")==0){
	  		strcpy(caType, "4");	
	  }else{
	  	  strcpy(caType, "2");	
	  }
	  
	  if(strcmp(caKey, "update")==0){
	  	  sprintf(caTemp2, "select imgpath from ncmcomindex where sid=%lu", atol(caSid));
	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  if(strlen(caDispImgPath)==0){ // 是否选择上传图片, 否：原图片不变
	  	  	  sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', level=%lu, plevel=%lu, type=%lu, systime=%lu, shopid=%lu where sid=%lu",caName,atol(caSgroupid),caDescr,caImgpath1,caSerno,caFloor,atol(caLevel),atol(caPlevel),atol(caType),time(0),atol(caShopid),atol(caSid));	
			  		printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caImgpath1);
	  	  }else{ // 是：原图片删除，新图片加入
			  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
				  	system(temp3_file);
			  	  
			  	  sprintf(caTemp,"update ncmcomindex set name='%s', sgroupid=%lu, descr='%s', imgpath='%s', serno='%s', floor='%s', level=%lu, plevel=%lu, type=%lu, systime=%lu, shopid=%lu where sid=%lu",caName,atol(caSgroupid),caDescr,caDispImgPath,caSerno,caFloor,atol(caLevel),atol(caPlevel),atol(caType),time(0),atol(caShopid),atol(caSid));	
			  		printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
			  }
	  }else if(strcmp(caKey, "add")==0){
				pasDbOneRecord("select max(sid) from ncmcomindex", 0, UT_TYPE_ULONG, 4, &lMaxid);
		    sprintf(caTemp,"insert into ncmcomindex (sid,name,sgroupid,descr,imgpath,serno,floor,level,plevel,type,systime,shopid) values (%lu,'%s',%lu,'%s','%s','%s','%s',%lu,%lu,%lu,%lu,%lu)",(lMaxid+1),caName,atol(caSgroupid),caDescr,caDispImgPath,caSerno,caFloor,atol(caLevel),atol(caPlevel),atol(caType),time(0),atol(caShopid));
			 	printf("caTemp = %s\n", caTemp);
			 	pasDbExecSqlF(caTemp);
			 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
	  }
	  
	  utPltPutVar(psDbHead,"mesg","数据加载成功");
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcombrand_dr_list.htm");
	  //utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","数据加载成功!","数据加载成功!");
    return 0;
}

// 商品信息
int ncm_webcomodity_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char sid[20] = "";
	 char name[32] = "";
	 char dispname[32] = "";
	 char servicecode[16] = "";
	 char indexname[32] = "";
	 char tyname[32] = "";
	 char imgpath[128] = "";
	 char shgroupid[32] = "";
	 char shopname[32] = "";
	 char shimgpath[128] = "";
	 char factory[32] = "";
	 char type[128] = "";
	 char url[255] = "";
	 char floor[32] = "";
	 char fldetail[255] = "";
	 char fee[20] = "";
	 char discount[20] = "";
	 char code[12] = "";
	 char tel[20] = "";
	 char address[255] = "";
	 char reserve[32] = "";
	 char active[64] = "";
	 char plevel[20] = "";
	 char level[20] = "";
	 char descr[255] = "";
	 char detail[4096] = "";
	 char integral[20] = "";
	 char coupon[20] = "";
	 char atype[20] = "";
	 char systime[20] = "";
	 char flags[20] = "";
	 char temp_file[128] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lSid = 0;
	 char 					caName[32] = "";
	 char 					caDispname[32] = "";
	 char 					caServicecode[16] = "";
	 char 					caIndexname[32] = "";
	 char 					caTyname[32] = "";
	 unsigned long	lShgroupid = 0;
	 char 					caImgpath[128] = "";
	 char 					caShopname[32] = "";
	 char 					caShimgpath[128] = "";
	 char 					caFactory[32] = "";
	 char 					caType[128] = "";
	 char 					caUrl[255] = "";
	 char 					caFloor[32] = "";
	 char 					caFldetail[255] = "";
	 double         dFee = 0;
	 double         dDiscount = 0;
	 char 					caCode[12] = "";
	 unsigned long  lTel = 0;
	 char 					caAddress[255] = "";
	 unsigned long  lReserve = 0;
	 char 					caActive[64] = "";
	 unsigned long	lPlevel = 0;
	 unsigned long  lLevel = 0;
	 char           caDescr[255] = "";
	 char           caDetail[4096] = "";
	 unsigned long  lIntegral = 0;
	 unsigned long  lCoupon = 0;
	 unsigned long  lAtype = 0;
	 unsigned long  lSystime = 0;
	 unsigned long  lFlags = 0;
	 char           s_systime[32] = "";
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caSesid[1024] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
   
   // 得到商店ID
   unsigned long lShopId = 0;
	 dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 41,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
																	"name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
																	"dispname",    UT_TYPE_STRING,  sizeof(dispname)-1,       dispname,
																	"servicecode", UT_TYPE_STRING,  sizeof(servicecode)-1,    servicecode,
																	"indexname",   UT_TYPE_STRING,  sizeof(indexname)-1,      indexname,
																	"tyname",      UT_TYPE_STRING,  sizeof(tyname)-1,         tyname,
																	"shgroupid",   UT_TYPE_STRING,  sizeof(shgroupid)-1,      shgroupid,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"shopname",    UT_TYPE_STRING,  sizeof(shopname)-1,       shopname,
																	"shimgpath",   UT_TYPE_STRING,  sizeof(shimgpath)-1,      shimgpath,
																	"factory",     UT_TYPE_STRING,  sizeof(factory)-1,        factory,
																	"type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
																	"url",         UT_TYPE_STRING,  sizeof(url)-1,            url,
																	"floor",       UT_TYPE_STRING,  sizeof(floor)-1,          floor,
																	"fldetail",    UT_TYPE_STRING,  sizeof(fldetail)-1,       fldetail,
																	"fee",         UT_TYPE_STRING,  sizeof(fee)-1,            fee,
																	"discount",    UT_TYPE_STRING,  sizeof(discount)-1,       discount,
																	"code",        UT_TYPE_STRING,  sizeof(code)-1,           code,
																	"tel",         UT_TYPE_STRING,  sizeof(tel)-1,            tel,
																	"address",     UT_TYPE_STRING,  sizeof(address)-1,        address,
																	"reserve",     UT_TYPE_STRING,  sizeof(reserve)-1,        reserve,
																	"active",      UT_TYPE_STRING,  sizeof(active)-1,         active,
																	"plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
																	"level",       UT_TYPE_STRING,  sizeof(level)-1,          level,
																	"descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
																	"detail",      UT_TYPE_STRING,  sizeof(detail)-1,         detail,
																	"integral",    UT_TYPE_STRING,  sizeof(integral)-1,       integral,
																	"coupon",      UT_TYPE_STRING,  sizeof(coupon)-1,         coupon,
																	"atype",       UT_TYPE_STRING,  sizeof(atype)-1,          atype,
																	"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
																	"flags",       UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"update",      UT_TYPE_STRING,  10,                       caUpdate,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport);
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,30);
			strcpy(name,caTemp);
   }
	 if(strlen(dispname)>0){
			pasCvtGBK(2,dispname,caTemp,30);
			strcpy(dispname,caTemp);
   }
   if(strlen(indexname)>0){
			pasCvtGBK(2,indexname,caTemp,30);
			strcpy(indexname,caTemp);
   }
   if(strlen(tyname)>0){
			pasCvtGBK(2,tyname,caTemp,30);
			strcpy(tyname,caTemp);
   }
   if(strlen(servicecode)>0){
			pasCvtGBK(2,servicecode,caTemp,30);
			strcpy(servicecode,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,30);
			strcpy(imgpath,caTemp);
   }
   if(strlen(shopname)>0){
			pasCvtGBK(2,shopname,caTemp,30);
			strcpy(shopname,caTemp);
   }
   if(strlen(shimgpath)>0){
			pasCvtGBK(2,shimgpath,caTemp,30);
			strcpy(shimgpath,caTemp);
   }
   if(strlen(factory)>0){
			pasCvtGBK(2,factory,caTemp,30);
			strcpy(factory,caTemp);
   }
   if(strlen(type)>0){
			pasCvtGBK(2,type,caTemp,30);
			strcpy(type,caTemp);
   }
   if(strlen(url)>0){
			pasCvtGBK(2,url,caTemp,30);
			strcpy(url,caTemp);
   }
   if(strlen(floor)>0){
			pasCvtGBK(2,floor,caTemp,30);
			strcpy(floor,caTemp);
   }
   if(strlen(fldetail)>0){
			pasCvtGBK(2,fldetail,caTemp,30);
			strcpy(fldetail,caTemp);
   }
   if(strlen(code)>0){
			pasCvtGBK(2,code,caTemp,30);
			strcpy(code,caTemp);
   }
   if(strlen(address)>0){
			pasCvtGBK(2,address,caTemp,30);
			strcpy(address,caTemp);
   }
   if(strlen(active)>0){
			pasCvtGBK(2,active,caTemp,30);
			strcpy(active,caTemp);
   }
   if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,30);
			strcpy(descr,caTemp);
   }
   if(strlen(detail)>0){
			pasCvtGBK(2,detail,caTemp,30);
			strcpy(detail,caTemp);
   }
   if(strlen(systime)>0){
			pasCvtGBK(2,systime,caTemp,30);
			strcpy(systime,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
   
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcomodity where sid in(%s)", caSesid);
    			printf("caTemp = %s\n", caTemp);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
				      return 0;	
					}
					
					// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
    		  iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
    		  
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH, imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmcomodity where sid in(%s)",caSesid);
					pasDbExecSqlF("delete from ncmshoptype where sid in(%s)",caSesid);
    	}
	 }
	 
	 // 查询
	 strcpy(sql, "select count(*) from ncmcomodity where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 if(lShopId>0) // 根据商店查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and shgroupid = %lu", lShopId);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select o.sid, o.name, o.dispname, o.servicecode, i.name, t.name, o.shgroupid, o.imgpath, o.shopname, o.shimgpath, o.factory, o.type, o.url, o.floor, o.fldetail, o.fee, o.discount, o.code, o.tel, o.address, o.reserve, o.active, o.plevel, o.level, o.descr, o.detail, o.integral, o.coupon, o.atype, o.systime, o.flags from ncmcomodity o left join ncmcomindex i on o.sgroupid = i.sid left join ncmcomtype t on o.groupid = t.sid where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (o.name like '%%%s%%') ", _keyword);
	 }
	 if(lShopId>0) // 根据商店查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and o.shgroupid = %lu", lShopId);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by o.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),            &lSid,
													     UT_TYPE_STRING, sizeof(caName)-1,        caName,
													     UT_TYPE_STRING, sizeof(caDispname)-1,    caDispname,
													     UT_TYPE_STRING, sizeof(caServicecode)-1, caServicecode,
													     UT_TYPE_STRING, sizeof(caIndexname)-1,   caIndexname,
													     UT_TYPE_STRING, sizeof(caTyname)-1,      caTyname,
													     UT_TYPE_ULONG,  sizeof(long),            &lShgroupid,
															 UT_TYPE_STRING, sizeof(caImgpath)-1,     caImgpath,
															 UT_TYPE_STRING, sizeof(caShopname)-1,    caShopname,
															 UT_TYPE_STRING, sizeof(caShimgpath)-1,   caShimgpath,
															 UT_TYPE_STRING, sizeof(caFactory)-1,     caFactory,
															 UT_TYPE_STRING, sizeof(caType)-1,        caType,
															 UT_TYPE_STRING, sizeof(caUrl)-1,         caUrl,
															 UT_TYPE_STRING, sizeof(caFloor)-1,       caFloor,
															 UT_TYPE_STRING, sizeof(caFldetail)-1,    caFldetail,
															 UT_TYPE_FLOAT,  sizeof(double),          &dFee,
															 UT_TYPE_FLOAT,  sizeof(double),          &dDiscount,
															 UT_TYPE_STRING, sizeof(caCode)-1,        caCode,
															 UT_TYPE_ULONG,  sizeof(long),            &lTel,
															 UT_TYPE_STRING, sizeof(caAddress)-1,     caAddress,
															 UT_TYPE_ULONG,  sizeof(long),            &lReserve,
															 UT_TYPE_STRING, sizeof(caActive)-1,      caActive,
															 UT_TYPE_ULONG,  sizeof(long),            &lPlevel,
															 UT_TYPE_ULONG,  sizeof(long),            &lLevel,
															 UT_TYPE_STRING, sizeof(caDescr)-1,       caDescr,
															 UT_TYPE_STRING, sizeof(caDetail)-1,      caDetail,
															 UT_TYPE_ULONG,  sizeof(long),            &lIntegral,
															 UT_TYPE_ULONG,  sizeof(long),            &lCoupon,
															 UT_TYPE_ULONG,  sizeof(long),            &lAtype,
															 UT_TYPE_ULONG,  sizeof(long),            &lSystime,
															 UT_TYPE_ULONG,  sizeof(long),            &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
		   
		   if(lSystime>0){
		   		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));	
		   }else{
		   		strcpy(s_systime, "");	
		   }
		   
		   //utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
			 utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
			 utPltPutLoopVar(psDbHead,"indexname",iNum,caIndexname);
			 utPltPutLoopVar(psDbHead,"tyname",iNum,caTyname);
			 utPltPutLoopVarF(psDbHead,"shgroupid",iNum,"%lu",lShgroupid);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"shopname",iNum,caShopname);
			 utPltPutLoopVar(psDbHead,"shimgpath",iNum,caShimgpath);
			 utPltPutLoopVar(psDbHead,"factory",iNum,caFactory);
			 utPltPutLoopVar(psDbHead,"type",iNum,caType);
			 utPltPutLoopVar(psDbHead,"url",iNum,caUrl);
			 utPltPutLoopVar(psDbHead,"floor",iNum,caFloor);
			 utPltPutLoopVar(psDbHead,"fldetail",iNum,caFldetail);
			 utPltPutLoopVarF(psDbHead,"fee",iNum,"%.2f",dFee);
			 utPltPutLoopVarF(psDbHead,"discount",iNum,"%.2f",dDiscount);
			 utPltPutLoopVar(psDbHead,"code",iNum,caCode);
			 utPltPutLoopVarF(psDbHead,"tel",iNum,"%lu",lTel);
			 utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
			 utPltPutLoopVarF(psDbHead,"reserve",iNum,"%lu",lReserve);
			 utPltPutLoopVar(psDbHead,"active",iNum,caActive);
			 utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",lPlevel);
			 utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",lLevel);
			 utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
			 utPltPutLoopVar(psDbHead,"detail",iNum,caDetail);
			 utPltPutLoopVarF(psDbHead,"integral",iNum,"%lu",lIntegral);
			 utPltPutLoopVarF(psDbHead,"coupon",iNum,"%lu",lCoupon);
			 utPltPutLoopVarF(psDbHead,"atype",iNum,"%lu",lAtype);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 //utPltSetCvtHtml(0);
			 
			 lSid = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caDispname, 0, sizeof(caDispname));
			 memset(caServicecode, 0, sizeof(caServicecode));
			 memset(caIndexname, 0, sizeof(caIndexname));
			 memset(caTyname, 0, sizeof(caTyname));
			 lShgroupid = 0;
			 memset(caImgpath, 0, sizeof(caImgpath));
			 memset(caShopname, 0, sizeof(caShopname));
			 memset(caShimgpath, 0, sizeof(caShimgpath));
			 memset(caFactory, 0, sizeof(caFactory));
			 memset(caType, 0, sizeof(caType));
			 memset(caUrl, 0, sizeof(caUrl));
			 memset(caFloor, 0, sizeof(caFloor));
			 memset(caFldetail, 0, sizeof(caFldetail));
			 dFee = 0;
			 dDiscount = 0;
			 memset(caCode, 0, sizeof(caCode));
			 lTel = 0;
			 memset(caAddress, 0, sizeof(caAddress));
			 lReserve = 0;
			 memset(caActive, 0, sizeof(caActive));
			 lPlevel = 0;
			 lLevel = 0;
			 memset(caDescr, 0, sizeof(caDescr));
			 memset(caDetail, 0, sizeof(caDetail));
			 lIntegral = 0;
			 lCoupon = 0;
			 lAtype = 0;
			 lSystime = 0;
			 lFlags = 0;
			 memset(s_systime, 0, sizeof(s_systime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),            &lSid,
															     UT_TYPE_STRING, sizeof(caName)-1,        caName,
															     UT_TYPE_STRING, sizeof(caDispname)-1,    caDispname,
															     UT_TYPE_STRING, sizeof(caServicecode)-1, caServicecode,
															     UT_TYPE_STRING, sizeof(caIndexname)-1,   caIndexname,
															     UT_TYPE_STRING, sizeof(caTyname)-1,      caTyname,
															     UT_TYPE_ULONG,  sizeof(long),            &lShgroupid,
																	 UT_TYPE_STRING, sizeof(caImgpath)-1,     caImgpath,
																	 UT_TYPE_STRING, sizeof(caShopname)-1,    caShopname,
																	 UT_TYPE_STRING, sizeof(caShimgpath)-1,   caShimgpath,
																	 UT_TYPE_STRING, sizeof(caFactory)-1,     caFactory,
																	 UT_TYPE_STRING, sizeof(caType)-1,        caType,
																	 UT_TYPE_STRING, sizeof(caUrl)-1,         caUrl,
																	 UT_TYPE_STRING, sizeof(caFloor)-1,       caFloor,
																	 UT_TYPE_STRING, sizeof(caFldetail)-1,    caFldetail,
																	 UT_TYPE_FLOAT,  sizeof(double),          &dFee,
																	 UT_TYPE_FLOAT,  sizeof(double),          &dDiscount,
																	 UT_TYPE_STRING, sizeof(caCode)-1,        caCode,
																	 UT_TYPE_ULONG,  sizeof(long),            &lTel,
																	 UT_TYPE_STRING, sizeof(caAddress)-1,     caAddress,
																	 UT_TYPE_ULONG,  sizeof(long),            &lReserve,
																	 UT_TYPE_STRING, sizeof(caActive)-1,      caActive,
																	 UT_TYPE_ULONG,  sizeof(long),            &lPlevel,
																	 UT_TYPE_ULONG,  sizeof(long),            &lLevel,
																	 UT_TYPE_STRING, sizeof(caDescr)-1,       caDescr,
																	 UT_TYPE_STRING, sizeof(caDetail)-1,      caDetail,
																	 UT_TYPE_ULONG,  sizeof(long),            &lIntegral,
																	 UT_TYPE_ULONG,  sizeof(long),            &lCoupon,
																	 UT_TYPE_ULONG,  sizeof(long),            &lAtype,
																	 UT_TYPE_ULONG,  sizeof(long),            &lSystime,
																	 UT_TYPE_ULONG,  sizeof(long),            &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomodity_list.htm");
	  return 0;	
}

int ncm_webcomodity_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
//#endif  
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char caKey[32] = "";
	 char caSid[32] = "";
	 char caName[32] = "";
	 char caScode[16] = "";
	 unsigned long lSgroupid = 0;
	 unsigned long lGroupid = 0;
	 unsigned long lShgroupid = 0;
	 char caImgpath[128] = "";
	 char caFactory[32] = "";
	 char caType[128] = "";
	 double dFee = 0.0;
	 double dDiscount = 0.0;
	 unsigned long lReserve = 0;
	 char caActive[64] = "";
	 unsigned long lPlevel = 0;
	 unsigned long lLevel = 0;
	 char caDescr[255] = "";
	 char caDetail[4096] = "";
	 unsigned long lIntegral = 0;
	 unsigned long lCoupon = 0;
	 unsigned long lAtype = 0;
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 2, "key", UT_TYPE_STRING, sizeof(caKey)-1, caKey,
	                                "sid", UT_TYPE_STRING, sizeof(caSid)-1, caSid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
			 sprintf(sql, "select name, servicecode, sgroupid, groupid, shgroupid, imgpath, factory, type, fee, discount, reserve, active, plevel, level, descr, detail, integral, coupon, atype from ncmcomodity where sid=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,31,caName,
			 											UT_TYPE_STRING,15,caScode,
			 											UT_TYPE_ULONG,4,&lSgroupid,
			 											UT_TYPE_ULONG,4,&lGroupid,
			 											UT_TYPE_ULONG,4,&lShgroupid,
			 											UT_TYPE_STRING,127,caImgpath,
			 											UT_TYPE_STRING,31,caFactory,
			 											UT_TYPE_STRING,127,caType,
			 											UT_TYPE_FLOAT,sizeof(double),&dFee,
			 											UT_TYPE_FLOAT,sizeof(double),&dDiscount,
			 											UT_TYPE_ULONG,4,&lReserve,
			 											UT_TYPE_STRING,63,caActive,
			 											UT_TYPE_ULONG,4,&lPlevel,
			 											UT_TYPE_ULONG,4,&lLevel,
		                        UT_TYPE_STRING,254,caDescr,
		                        UT_TYPE_STRING,254,caDetail,
		                        UT_TYPE_ULONG,4,&lIntegral,
		                        UT_TYPE_ULONG,4,&lCoupon,
		                        UT_TYPE_ULONG,4,&lAtype);
		    //utPltSetCvtHtml(1);             
			  utPltPutVar(psDbHead,"sid",caSid);
				utPltPutVar(psDbHead,"name",caName);
				utPltPutVar(psDbHead,"scode",caScode);
				utPltPutVarF(psDbHead,"indexid","%lu",lSgroupid);
				utPltPutVarF(psDbHead,"typeid","%lu",lGroupid);
				utPltPutVarF(psDbHead,"shopid","%lu",lShgroupid);
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVar(psDbHead,"factory",caFactory);
		    utPltPutVar(psDbHead,"type",caType);
		    utPltPutVarF(psDbHead,"fee","%.2f",dFee);
		    utPltPutVarF(psDbHead,"discount","%.2f",dDiscount);
		    utPltPutVarF(psDbHead,"reserve","%lu",lReserve);
		    utPltPutVar(psDbHead,"active",caActive);
		    utPltPutVarF(psDbHead,"plevel","%lu",lPlevel);
		    utPltPutVarF(psDbHead,"level","%lu",lLevel);
		    utPltPutVar(psDbHead,"descr",caDescr);
		    utPltPutVar(psDbHead,"detail",caDetail);
		    utPltPutVarF(psDbHead,"integral","%lu",lIntegral);
		    utPltPutVarF(psDbHead,"coupon","%lu",lCoupon);
		    utPltPutVarF(psDbHead,"atype","%lu",lAtype);
		    //utPltSetCvtHtml(0);
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomodity_dr.htm");
	 return 0;		
}

// 上传商品图片
int ncm_webComOdityImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32]="";
    char caSid[20]="";
    char caName[32]="";
    char caSgroupid[32]="";
    char caGroupid[32]="";
    char caShgroupid[32]="";
    char caFactory[32]="";
    char caType[128]="";
    char caFee[32]="";
    char caDiscount[32]="";
    char caReserve[32]="";
    char caActive[32]="";
    char caPlevel[20]="";
    char caLevel[20]="";
    char caIntegral[20]="";
    char caCoupon[20]="";
    char caAtype[20]="";
    char caImgPath[128]="";
    char caDescr[255]="";
    char caDetail[4096]="";
    char caDispImgPath[128]="";
    char imgname[128]="";
    
    char caShopname[32] = "";
    char caScode[16] = "";
    unsigned long lSgroupid = 0;
	  char caShopImgPath[128] = "";
	  char caUrl[255] = "";
	  char caFloor[32] = "";
	  char caCode[32] = "";
	  char caTel[32] = "";
	  char caAddress[255] = "";
	  unsigned long lLevel = 0;
	  
	  char caFloorDetail[255] = "";
    
    char caImgpath1[128]="";
    char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
 		char caTemp2[1024]="";
 		char caTemp3[1024]="";
 		char caTemp4[1024]="";
 		char caCompTemp[1024]="";
 		unsigned long lCount=0;
 		unsigned long lMaxid = 0;
 		unsigned long lMaxcompid = 0;
 		unsigned long lId = 0;
 		char *p;
 		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
		
		// 上传的图片路径
		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0194导入"),ncLang("0183打开文件出错"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// 建立图片存放路径
  	sprintf(caImgPath,"%s/images/ShopImages/odityimages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);
        
	  utMsgGetSomeNVar(psMsgHead,20,"key",         UT_TYPE_STRING,  sizeof(caKey)-1,             caKey,
	  														  "sid",         UT_TYPE_STRING,  sizeof(caSid)-1,             caSid,
	  														  "name",        UT_TYPE_STRING,  sizeof(caName)-1,           caName,
	                                "inid",        UT_TYPE_STRING,  sizeof(caSgroupid)-1,       caSgroupid,
	                                "tyid",        UT_TYPE_STRING,  sizeof(caGroupid)-1,        caGroupid,
	                                "shid",        UT_TYPE_STRING,  sizeof(caShgroupid)-1,      caShgroupid,
	                                "factory",     UT_TYPE_STRING,  sizeof(caFactory)-1,        caFactory,
	                                "type",        UT_TYPE_STRING,  sizeof(caType)-1,           caType,
	                                "fee",         UT_TYPE_STRING,  sizeof(caFee)-1,            caFee,
	                                "discount",    UT_TYPE_STRING,  sizeof(caDiscount)-1,       caDiscount,
	                                "reserve",     UT_TYPE_STRING,  sizeof(caReserve)-1,        caReserve,
	                                "active",      UT_TYPE_STRING,  sizeof(caActive)-1,         caActive,
	                                "plevel",      UT_TYPE_STRING,  sizeof(caPlevel)-1,         caPlevel,
	                                "level",       UT_TYPE_STRING,  sizeof(caLevel)-1,          caLevel,
	                                "integral",    UT_TYPE_STRING,  sizeof(caIntegral)-1,       caIntegral,
	                                "coupon",      UT_TYPE_STRING,  sizeof(caCoupon)-1,         caCoupon,
	                                "atype",       UT_TYPE_STRING,  sizeof(caAtype)-1,          caAtype,
																  "fname",       UT_TYPE_STRING,  255,                        imp_file,
																  "descr",       UT_TYPE_STRING,  sizeof(caDescr)-1,          caDescr,
																  "detail",      UT_TYPE_STRING,  sizeof(caDetail)-1,         caDetail);
	 
	  if(strlen(caName)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","商品名称不能为空!","商品名称不能为空!");
        return 0;	
	  }
	                  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*从指定的字符串中取一个词
		  	
		  	  函数原型：
			　　char *utStrGetWord(char *p,char *pOut,
			　　                   short nMaxLen,char *pTerm);
			　　
					参数说明:
			　　p – 输入字符串
			　　pOut --- 输出字符串，必须要事先分配好空间
			　　nMaxLen --- 输出字符传的最大长度
			　　pTerm  ----- 终止条件的字符串，系统在扫描描时遇到该字符串某一个字符时结束。
			　　
			　　返回值：
			　　结束字符的指针。*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
				printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
					  memset(caDispImgPath, 0, sizeof(caDispImgPath));
				}else{
						sprintf(imgname, "odity_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/odityimages/");
				    strcat(caDispImgPath, imgname);	
				}
	  }
	  
	  sprintf(caTemp,"select name, service_code, sgroupid, imgpath, url, floor, code, tel, address, level from ncmshop where id = %lu", atol(caShgroupid));
		printf("caTemp = %s\n", caTemp);
		pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, sizeof(caShopname)-1,     caShopname,
														  UT_TYPE_STRING, sizeof(caScode)-1,        caScode,
														  UT_TYPE_STRING, sizeof(caShopImgPath)-1,  caShopImgPath,
														  UT_TYPE_STRING, sizeof(caUrl)-1,          caUrl,
														  UT_TYPE_STRING, sizeof(caFloor)-1,        caFloor,
														  UT_TYPE_STRING, sizeof(caCode)-1,         caCode,
														  UT_TYPE_STRING, sizeof(caTel)-1,          caTel,
														  UT_TYPE_STRING, sizeof(caAddress)-1,      caAddress,
														  UT_TYPE_ULONG,  sizeof(long),             &lLevel);
														 
		pasDbOneRecord("select detail from ncmmall where name = :caFloor", 1, "caFloor", UT_TYPE_STRING, caFloor, UT_TYPE_STRING, 254, caFloorDetail);
	  
	  sprintf(caTemp3, "select count(shopid) from ncmshoptype where shopid=%lu", atol(caShgroupid));
	  pasDbOneRecord(caTemp3, 0, UT_TYPE_ULONG, sizeof(long), &lCount);
	  printf("lCount = %d\n", lCount);
	  
	  if(strcmp(caKey, "update")==0){
	  	  sprintf(caTemp2, "select imgpath from ncmcomodity where sid=%lu", atol(caSid));
	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  if(strlen(caDispImgPath)==0){ // 是否选择上传图片, 否：原图片不变
	  	  	  sprintf(caTemp,"update ncmcomodity set name='%s',dispname='%s',servicecode='%s',sgroupid=%lu,groupid=%lu,shgroupid=%lu,imgpath='%s',shopname='%s',shimgpath='%s',factory='%s',type='%s',url='%s',floor='%s',fldetail='%s',fee=%.2f,discount=%.2f,code='%s',tel='%s',address='%s',reserve=%lu,active='%s',plevel=%lu,level=%lu,descr='%s',detail='%s',integral=%lu,coupon=%lu,systime=%lu,atype=%lu where sid=%lu",caName,caName,caScode,atol(caSgroupid),atol(caGroupid),atol(caShgroupid),caImgpath1,caShopname,caShopImgPath,caFactory,caType,caUrl,caFloor,caFloorDetail,atof(caFee),atof(caDiscount),caCode,caTel,caAddress,atol(caReserve),caActive,atol(caPlevel),atol(caLevel),caDescr,caDetail,atol(caIntegral),atol(caCoupon),time(0),atol(caAtype),atol(caSid));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caImgpath1);
					 	
					 	// 对照表
					 	if(lCount==0){
					 			sprintf(caCompTemp, "update ncmshoptype set id=%lu, shopid=%lu, shopname='%s', imgpath='%s', level=%lu, sgroupid=%lu, sid=%lu, sname='%s', fee=%.2f, discount=%.2f, systime=%lu", (lMaxcompid+1), atol(caSgroupid), caShopname, caShopImgPath, lLevel, atol(caGroupid), (lMaxid+1), caName, atof(caFee), atof(caDiscount), time(0));
			 					printf("caCompTemp = %s\n", caCompTemp);
			 	    		pasDbExecSqlF(caCompTemp);	
					 	}else{
					 			sprintf(caTemp4, "select id from ncmshoptype where shopid=%lu", atol(caShgroupid));
					 			pasDbOneRecord(caTemp4, 0, UT_TYPE_ULONG, sizeof(long), &lId);
					 		
					 		  sprintf(caCompTemp, "update ncmshoptype set sgroupid=%lu, sid=%lu, sname='%s', fee=%.2f, discount=%.2f, systime=%lu where id=%lu", atol(caGroupid), (lMaxid+1), caName, atof(caFee), atof(caDiscount), time(0), lId);
					 			printf("caCompTemp = %s\n", caCompTemp);
					 	    pasDbExecSqlF(caCompTemp);
					 	}
	  	  }else{ // 是：原图片删除，新图片加入
			  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
				  	system(temp3_file);
			  	  
			  	  sprintf(caTemp,"update ncmcomodity set name='%s',dispname='%s',servicecode='%s',sgroupid=%lu,groupid=%lu,shgroupid=%lu,imgpath='%s',shopname='%s',shimgpath='%s',factory='%s',type='%s',url='%s',floor='%s',fldetail='%s',fee=%.2f,discount=%.2f,code='%s',tel='%s',address='%s',reserve=%lu,active='%s',plevel=%lu,level=%lu,descr='%s',detail='%s',integral=%lu,coupon=%lu,systime=%lu,atype=%lu where sid=%lu",caName,caName,caScode,atol(caSgroupid),atol(caGroupid),atol(caShgroupid),caDispImgPath,caShopname,caShopImgPath,caFactory,caType,caUrl,caFloor,caFloorDetail,atof(caFee),atof(caDiscount),caCode,caTel,caAddress,atol(caReserve),caActive,atol(caPlevel),atol(caLevel),caDescr,caDetail,atol(caIntegral),atol(caCoupon),time(0),atol(caAtype),atol(caSid));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
					 	
					 	// 对照表
					 	if(lCount==0){
					 			sprintf(caCompTemp, "update ncmshoptype set id=%lu, shopid=%lu, shopname='%s', imgpath='%s', level=%lu, sgroupid=%lu, sid=%lu, sname='%s', fee=%.2f, discount=%.2f, systime=%lu", (lMaxcompid+1), atol(caSgroupid), caShopname, caShopImgPath, lLevel, atol(caGroupid), (lMaxid+1), caName, atof(caFee), atof(caDiscount), time(0));
			 					printf("caCompTemp = %s\n", caCompTemp);
			 	    		pasDbExecSqlF(caCompTemp);	
					 	}else{
					 			sprintf(caTemp4, "select id from ncmshoptype where shopid=%lu", atol(caShgroupid));
					 			pasDbOneRecord(caTemp4, 0, UT_TYPE_ULONG, sizeof(long), &lId);
					 		
					 		  sprintf(caCompTemp, "update ncmshoptype set sgroupid=%lu, sid=%lu, sname='%s', fee=%.2f, discount=%.2f, systime=%lu where id=%lu", atol(caGroupid), (lMaxid+1), caName, atof(caFee), atof(caDiscount), time(0), lId);
					 			printf("caCompTemp = %s\n", caCompTemp);
					 	    pasDbExecSqlF(caCompTemp);	
					 	}
			  }
	  }else if(strcmp(caKey, "add")==0){
				pasDbOneRecord("select max(sid) from ncmcomodity", 0, UT_TYPE_ULONG, 4, &lMaxid);
		    sprintf(caTemp,"insert into ncmcomodity (sid,name,dispname,servicecode,sgroupid,groupid,shgroupid,imgpath,shopname,shimgpath,factory,type,url,floor,fldetail,fee,discount,code,tel,address,reserve,active,plevel,level,descr,detail,integral,coupon,systime,atype) values (%lu,'%s','%s','%s',%lu,%lu,%lu,'%s','%s','%s','%s','%s','%s','%s','%s',%.2f,%.2f,'%s','%s','%s','%s','%s',%lu,%lu,'%s','%s',%lu,%lu,%lu,%lu)",(lMaxid+1),caName,caName,caScode,atol(caSgroupid),atol(caGroupid),atol(caShgroupid),caDispImgPath,caShopname,caShopImgPath,caFactory,caType,caUrl,caFloor,caFloorDetail,atof(caFee),atof(caDiscount),caCode,caTel,caAddress,caReserve,caActive,atol(caPlevel),atol(caLevel),caDescr,caDetail,atol(caIntegral),atol(caCoupon),time(0),atol(caAtype));
			 	printf("caTemp = %s\n", caTemp);
			 	pasDbExecSqlF(caTemp);
			 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
			 	
			 	// 对照表
			 	pasDbOneRecord("select max(id) from ncmshoptype", 0, UT_TYPE_ULONG, 4, &lMaxcompid);
			 	if(lCount==0){
			 			sprintf(caCompTemp, "insert into ncmshoptype (id, shopid, shopname, imgpath, level, sgroupid, sid, sname, fee, discount, systime) values (%lu, %lu, '%s', '%s', %lu, %lu, %lu, '%s', %.2f, %.2f, %lu)", (lMaxcompid+1), atol(caShgroupid), caShopname, caShopImgPath, lLevel, atol(caGroupid), (lMaxid+1), caName, atof(caFee), atof(caDiscount), time(0));	
			 			printf("caCompTemp = %s\n", caCompTemp);
			 	    pasDbExecSqlF(caCompTemp);
			 	}else{
			 			sprintf(caTemp4, "select id from ncmshoptype where shopid=%lu", atol(caShgroupid));
			 			pasDbOneRecord(caTemp4, 0, UT_TYPE_ULONG, sizeof(long), &lId);
			 		
			 		  sprintf(caCompTemp, "update ncmshoptype set sgroupid=%lu, sid=%lu, sname='%s', fee=%.2f, discount=%.2f, systime=%lu where id=%lu", atol(caGroupid), (lMaxid+1), caName, atof(caFee), atof(caDiscount), time(0), lId);
			 			printf("caCompTemp = %s\n", caCompTemp);
			 	    pasDbExecSqlF(caCompTemp);
			 	}
	  }
	  utPltPutVar(psDbHead,"mesg","数据加载成功");
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomodity_dr_list.htm");
	  //utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","数据加载成功!","数据加载成功!");
    return 0;
}

// 商品商店对照表
int ncm_webcomcomp_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
		utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
		
		// 初始化数据库
		char sql[4096] = "";
		pasDbCursor *psCur = NULL;
		int iret=0;
		int iNum=0;
		utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板	
		
		// 接收信息变量
		char id[20] = "";
		char shopid[20] = "";
		char shopname[32] = "";
		char sgroupid[20] = "";
		char level[20] = "";
		char etime[32] = "";
		char sEtime[32] = "";
		
		// 存放数据库取出的数据的变量
		unsigned long lId = 0;
		unsigned long lShopid = 0;
		char caShopname[32] = "";
		unsigned long lSgroupid = 0;
		char caSgroupname[32] = "";
		unsigned long lLevel = 0;
		unsigned long lEtime = 0;
		char s_etime[32] = "";
		
		// 修改需要的变量
		unsigned long uLevel = 0;
		unsigned long uSystime = 0;
		unsigned long uEtime = 0;
		
		char caUpdate[16] = "";
	  char del[16] = "";
	  char caSesid[1024] = "";
	  char caDel[16] = "";
	  char caTemp[1024]="";
	  char caTemp1[1024]="";
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
	  
		// utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
		utMsgGetSomeNVar(psMsgHead, 15, "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																		"sid",         UT_TYPE_STRING,  sizeof(id)-1,             id,
																		"shopid",      UT_TYPE_STRING,  sizeof(shopid)-1,         shopid,
																		"shopname",    UT_TYPE_STRING,  sizeof(shopname)-1,       shopname,
																		"sgroupid",    UT_TYPE_STRING,  sizeof(sgroupid)-1,       sgroupid,
																		"level",       UT_TYPE_STRING,  sizeof(level)-1,          level,
																		"etime",       UT_TYPE_STRING,  sizeof(etime)-1,          etime,
																		"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
																	 	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
																	 	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
																	 	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																		"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																		"del",				 UT_TYPE_STRING,  10,                       caDel,
																		"update",      UT_TYPE_STRING,  10,                       caUpdate,
																		"caExport",    UT_TYPE_STRING,  10,                       caExport);
		
		trim(_keyword);
		
		//编码转换
		if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
		}
		if(strlen(shopname)>0){
			pasCvtGBK(2,shopname,caTemp,30);
			strcpy(shopname,caTemp);
		}
		
		// 排序
    if(strlen(sort)==0){
	 	   strcpy(sort,"id");
	  }
	  
	  // 时间转换
    ncTimeToStr(etime, sEtime);
   
	  // 修改
		if(!utStrIsSpaces(caUpdate)) {
				int flag = 1;
			
			  sprintf(caTemp1, "select level, systime, etime from ncmshoptype where sgroupid=%lu", atol(sgroupid));
				printf("caTemp1 = %s\n", caTemp1);
				psCur = pasDbOpenSql(caTemp1, 0);
				if(psCur==NULL){
					 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
				   return 0;	
				}
				
				// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG,  sizeof(long),            &uLevel,
																     UT_TYPE_ULONG,  sizeof(long),            &uSystime,
																     UT_TYPE_ULONG,  sizeof(long),            &uEtime);
				
				while(0 == iret || 1405 == iret){
						if(uLevel == atol(level)){
								if((uSystime+uEtime)<time(0)){
										flag = 1;	
								}else{
										flag = 0;
								}
						}
						
						uLevel = 0;
						uSystime = 0;
						uEtime = 0;
						iret = pasDbFetchInto(psCur, UT_TYPE_ULONG,  sizeof(long),            &uLevel,
																		     UT_TYPE_ULONG,  sizeof(long),            &uSystime,
																		     UT_TYPE_ULONG,  sizeof(long),            &uEtime);
				}
				pasDbCloseCursor(psCur);
				
				if(flag){
						sprintf(caTemp,"update ncmshoptype set systime=%lu, level=%lu, etime=%lu where id=%lu ",time(0),atol(level),atol(sEtime),atol(id));
						printf("caTemp = %s\n", caTemp);
				 		pasDbExecSqlF(caTemp);
				}else{
					  utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","此等级商家已经存在","此等级商家已经存在");
		   			return 0;
				}
		}
		
		unsigned long lShopId = 0;
	  dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	  
		// 查询
		strcpy(sql, "select count(*) from ncmshoptype where 1=1");
		if(!utStrIsSpaces(_keyword)) // 根据关键字查询
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (shopname like '%%%s%%') ", _keyword);
		}
		if(!utStrIsSpaces(sgroupid)) // 根据商品小类查询
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sgroupid = %lu ", atol(sgroupid));
		}
		if(lShopId>0) // 根据商店查询
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and shopid = %lu ", lShopId);
		}
		iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
		lTotRec = lCount1;
		utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
		printf("lTotRec = %d\n", lTotRec);
		printf("lCount1 = %d\n", lCount1);
		
		iStart = atol(start);
		iLimit = atol(limit);
		
		strcpy(sql, "select s.id, s.shopid, s.shopname, s.sgroupid, t.name, s.level, s.etime from ncmshoptype s left join ncmcomtype t on s.sgroupid = t.sid where 1=1");
		// utStrIsSpaces: 判断字符串是否为空。
		if(!utStrIsSpaces(_keyword)) // 根据关键字查询
		{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (s.shopname like '%%%s%%') ", _keyword);
		}
		if(!utStrIsSpaces(sgroupid)) // 根据商品小类查询
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and s.sgroupid = %lu ", atol(sgroupid));
		}
		if(lShopId>0) // 根据商店查询
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and s.shopid = %lu ", lShopId);
		}
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by s.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
		printf("sql = %s\n", sql);
		psCur = pasDbOpenSql(sql, 0);
		if(psCur==NULL){
			 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
		   return 0;	
		}
		
		// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
		iret = pasDbFetchInto(psCur, UT_TYPE_ULONG,  sizeof(long),            &lId,
																 UT_TYPE_ULONG,  sizeof(long),            &lShopid,
														     UT_TYPE_STRING, sizeof(caShopname)-1,    caShopname,
														     UT_TYPE_ULONG,  sizeof(long),            &lSgroupid,
														     UT_TYPE_STRING, sizeof(caSgroupname)-1,  caSgroupname,
														     UT_TYPE_ULONG,  sizeof(long),            &lLevel,
														     UT_TYPE_ULONG,  sizeof(long),            &lEtime);
														   
		while(0 == iret || 1405 == iret){
				iNum++;
				if(iNum > 1){
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
				}
				
				utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
				utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu",lShopid);
			  utPltPutLoopVar(psDbHead,"shopname",iNum,caShopname);
			  utPltPutLoopVarF(psDbHead,"sgroupid",iNum,"%lu",lSgroupid);
			  utPltPutLoopVar(psDbHead,"sgroupname",iNum,caSgroupname);
			  utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",lLevel);
			  utPltPutLoopVar(psDbHead,"etime",iNum,utStrLtoF8((long long)lEtime, s_etime));
			  
			  lId = 0;
			  lShopid = 0;
			  memset(caShopname, 0, sizeof(caShopname));
			  lSgroupid = 0;
			  memset(caSgroupname, 0, sizeof(caSgroupname));
				lLevel = 0;
				lEtime = 0;
				memset(s_etime, 0, sizeof(s_etime));
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG,  sizeof(long),            &lId,
																		 UT_TYPE_ULONG,  sizeof(long),            &lShopid,
																     UT_TYPE_STRING, sizeof(caShopname)-1,    caShopname,
																     UT_TYPE_ULONG,  sizeof(long),            &lSgroupid,
																     UT_TYPE_STRING, sizeof(caSgroupname)-1,  caSgroupname,
																     UT_TYPE_ULONG,  sizeof(long),            &lLevel,
																     UT_TYPE_ULONG,  sizeof(long),            &lEtime);
		}
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomcomp_list.htm");
	  return 0;
}

// 商店信息
int ncm_webcomshop_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char id[20] = "";
	 char name[32] = "";
	 char service_code[16] = "";
	 char sgroupname[32] = "";
	 char imgpath[128] = "";
	 char floor[32] = "";
	 char url[255] = "";
	 char code[12] = "";
	 char tel[32] = "";
	 char address[255] = "";
	 char level[20] = "";
	 char plevel[20] = "";
	 char type[20] = "";
	 char stype[20] = "";
	 char descr[255] = "";
	 char detail[4096] = "";
	 char systime[32] = "";
	 char flags[32] = "";
	 char temp_file[128] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lId = 0;
	 char 					caName[32] = "";
	 char           caService_code[16] = "";
	 char 					caSgroupname[32] = "";
	 char 					caImgpath[128] = "";
	 char 					caFloor[32] = "";
	 char 					caUrl[255] = "";
	 char 					caCode[12] = "";
	 char 					caTel[32] = "";
	 char 					caAddress[255] = "";
	 unsigned long	lLevel = 0;
	 unsigned long	lPlevel = 0;
	 unsigned long	lType = 0;
	 unsigned long	lStype = 0;
	 char 					caDescr[255] = "";
	 char 					caDetail[4096] = "";
	 unsigned long	lSystime = 0;
	 unsigned long	lFlags = 0;
	 char           s_systime[32] = "";
	 char           caType[32] = "";
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
   
   // 得到商店ID
   unsigned long lShopId = 0;
	 dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 29,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"id",          UT_TYPE_STRING,  sizeof(id)-1,             id,
																	"name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
																	"service_code",UT_TYPE_STRING,  sizeof(service_code)-1,   service_code,
																	"sgroupname",  UT_TYPE_STRING,  sizeof(sgroupname)-1,     sgroupname,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"floor",       UT_TYPE_STRING,  sizeof(floor)-1,          floor,
																	"url",         UT_TYPE_STRING,  sizeof(url)-1,            url,
																	"code",        UT_TYPE_STRING,  sizeof(code)-1,           code,
																	"tel",         UT_TYPE_STRING,  sizeof(tel)-1,            tel,
																	"address",     UT_TYPE_STRING,  sizeof(address)-1,        address,
																	"level",       UT_TYPE_STRING,  sizeof(level)-1,          level,
																	"plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
																	"type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
																	"stype",       UT_TYPE_STRING,  sizeof(stype)-1,          stype,
																	"descr",       UT_TYPE_STRING,  sizeof(descr)-1,          descr,
																	"detail",      UT_TYPE_STRING,  sizeof(detail)-1,         detail,
																	"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
																	"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"update",      UT_TYPE_STRING,  10,                       caUpdate,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport);
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,30);
			strcpy(name,caTemp);
   }
   if(strlen(service_code)>0){
			pasCvtGBK(2,service_code,caTemp,30);
			strcpy(service_code,caTemp);
   }
   if(strlen(sgroupname)>0){
			pasCvtGBK(2,sgroupname,caTemp,30);
			strcpy(sgroupname,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,30);
			strcpy(imgpath,caTemp);
   }
   if(strlen(floor)>0){
			pasCvtGBK(2,floor,caTemp,30);
			strcpy(floor,caTemp);
   }
   if(strlen(address)>0){
			pasCvtGBK(2,address,caTemp,30);
			strcpy(address,caTemp);
   }
   if(strlen(type)>0){
			pasCvtGBK(2,type,caTemp,30);
			strcpy(type,caTemp);
   }
	 if(strlen(descr)>0){
			pasCvtGBK(2,descr,caTemp,30);
			strcpy(descr,caTemp);
   }
   if(strlen(detail)>0){
			pasCvtGBK(2,detail,caTemp,30);
			strcpy(detail,caTemp);
   }
   if(strlen(systime)>0){
			pasCvtGBK(2,systime,caTemp,30);
			strcpy(systime,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
	 
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmshop where id in(%s)", caSesid);
    			printf("caTemp = %s\n", caTemp);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
				      return 0;	
					}
					
    		  // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					}
					pasDbExecSqlF("delete from ncmshop where id in(%s)",caSesid);
					pasDbExecSqlF("delete from ncmshoptype where shopid in(%s)",caSesid);
    	}
	 }
	 
	 // 查询
	 strcpy(sql, "select count(*) from ncmshop where 1=1");
	 if(lShopId>0) // 根据商店查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and id = %lu ", lShopId);
	 }
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%') ", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select s.id, s.name, s.service_code, t.name, s.imgpath, s.floor, s.url, s.code, s.tel, s.address, s.level, s.plevel, s.type, s.stype, s.descr, s.detail, s.systime, s.flags from ncmshop s left join ncmcomtype t on s.sgroupid = t.sid where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(lShopId>0) // 根据商店查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and s.id = %lu ", lShopId);
	 }
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (s.name like '%%%s%%') ", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by s.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),            &lId,
													     UT_TYPE_STRING, sizeof(caName)-1,        caName,
													     UT_TYPE_STRING, sizeof(caService_code)-1,caService_code,
													     UT_TYPE_STRING, sizeof(caSgroupname)-1,  caSgroupname,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,     caImgpath,
													     UT_TYPE_STRING, sizeof(caFloor)-1,       caFloor,
													     UT_TYPE_STRING, sizeof(caUrl)-1,         caUrl,
													     UT_TYPE_STRING, sizeof(caCode)-1,        caCode,
													     UT_TYPE_STRING, sizeof(caTel)-1,         caTel,
													     UT_TYPE_STRING, sizeof(caAddress)-1,     caAddress,
													     UT_TYPE_ULONG,  sizeof(long),            &lLevel,
													     UT_TYPE_ULONG,  sizeof(long),            &lPlevel,
													     UT_TYPE_ULONG,  sizeof(long),            &lType,
													     UT_TYPE_ULONG,  sizeof(long),            &lStype,
													     UT_TYPE_STRING, sizeof(caDescr)-1,       caDescr,
													     UT_TYPE_STRING, sizeof(caDetail)-1,      caDetail,
													     UT_TYPE_ULONG,  sizeof(long),            &lSystime,
															 UT_TYPE_ULONG,  sizeof(long),            &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1){
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 
		   if(lSystime>0){
		   		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
		   }else{
		   		strcpy(s_systime, "");	
		   }
		   
		   switch(lType){
					case 1:strcpy(caType, "广告");break;
					case 2:strcpy(caType, "品牌");break;
					default:break;	
			 }
			 
		   utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
			 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"service_code",iNum,caService_code);
			 utPltPutLoopVar(psDbHead,"sgroupname",iNum,caSgroupname);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"floor",iNum,caFloor);
			 utPltPutLoopVar(psDbHead,"url",iNum,caUrl);
			 utPltPutLoopVar(psDbHead,"code",iNum,caCode);
			 utPltPutLoopVar(psDbHead,"tel",iNum,caTel);
			 utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
			 utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",lLevel);
			 utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",lPlevel);
			 utPltPutLoopVar(psDbHead,"type",iNum,caType);
			 utPltPutLoopVarF(psDbHead,"stype",iNum,"%lu",lStype);
			 utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
			 utPltPutLoopVar(psDbHead,"detail",iNum,caDetail);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 
			 lId = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caService_code, 0, sizeof(caService_code));
			 memset(caSgroupname, 0, sizeof(caSgroupname));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 memset(caFloor, 0, sizeof(caFloor));
			 memset(caUrl, 0, sizeof(caUrl));
			 memset(caCode, 0, sizeof(caCode));
			 memset(caTel, 0, sizeof(caTel));
			 memset(caAddress, 0, sizeof(caAddress));
	     lLevel = 0;
	     lPlevel = 0;
	     lType = 0;
	     lStype = 0;
	     memset(caDescr, 0, sizeof(caDescr));
	     memset(caDetail, 0, sizeof(caDetail));
	     lSystime = 0;
	     lFlags = 0;
	     memset(s_systime, 0, sizeof(s_systime));
	     memset(caType, 0, sizeof(caType));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),            &lId,
															     UT_TYPE_STRING, sizeof(caName)-1,        caName,
															     UT_TYPE_STRING, sizeof(caService_code)-1,caService_code,
															     UT_TYPE_STRING, sizeof(caSgroupname)-1,  caSgroupname,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,     caImgpath,
															     UT_TYPE_STRING, sizeof(caFloor)-1,       caFloor,
															     UT_TYPE_STRING, sizeof(caUrl)-1,         caUrl,
															     UT_TYPE_STRING, sizeof(caCode)-1,        caCode,
															     UT_TYPE_STRING, sizeof(caTel)-1,         caTel,
															     UT_TYPE_STRING, sizeof(caAddress)-1,     caAddress,
															     UT_TYPE_ULONG,  sizeof(long),            &lLevel,
															     UT_TYPE_ULONG,  sizeof(long),            &lPlevel,
															     UT_TYPE_ULONG,  sizeof(long),            &lType,
															     UT_TYPE_ULONG,  sizeof(long),            &lStype,
															     UT_TYPE_STRING, sizeof(caDescr)-1,       caDescr,
															     UT_TYPE_STRING, sizeof(caDetail)-1,      caDetail,
															     UT_TYPE_ULONG,  sizeof(long),            &lSystime,
																	 UT_TYPE_ULONG,  sizeof(long),            &lFlags);
	  }
	  //utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomshop_list.htm");
	  return 0;	
}

int ncm_webcomshop_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
//#endif  
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char caKey[32] = "";
	 char caSid[32] = "";
	 char caName[32] = "";
	 char caScode[16] = "";
	 unsigned long lSgroupid = 0;
	 char caImgpath[128] = "";
	 char caFloor[32] = "";
	 char caUrl[255] = "";
	 char caCode[12] = "";
	 char caTel[32] = "";
	 char caAddress[255] = "";
	 unsigned long lLevel = 0;
	 unsigned long lPlevel = 0;
	 unsigned long lType = 0;
	 unsigned long lStype = 0;
	 char caDescr[255] = "";
	 char caDetail[4096] = "";
	 unsigned long lSystime = 0;
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 2, "key", UT_TYPE_STRING, sizeof(caKey)-1, caKey,
	                                "sid", UT_TYPE_STRING, sizeof(caSid)-1, caSid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
			 sprintf(sql, "select name, service_code, sgroupid, imgpath, floor, url, code, tel, address, level, plevel, type, stype, descr, detail, systime from ncmshop where id=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,31,caName,
			 											UT_TYPE_STRING,15,caScode,
			 											UT_TYPE_ULONG,4,&lSgroupid,
			 											UT_TYPE_STRING,127,caImgpath,
			 											UT_TYPE_STRING,32,caFloor,
			 											UT_TYPE_STRING,254,caUrl,
			 											UT_TYPE_STRING,11,caCode,
			 											UT_TYPE_STRING,32,caTel,
			 											UT_TYPE_STRING,254,caAddress,
			 											UT_TYPE_ULONG,4,&lLevel,
			 											UT_TYPE_ULONG,4,&lPlevel,
			 											UT_TYPE_ULONG,4,&lType,
			 											UT_TYPE_ULONG,4,&lStype,
		                        UT_TYPE_STRING,254,caDescr,
		                        UT_TYPE_STRING,254,caDetail,
		                        UT_TYPE_ULONG,4,&lSystime);
		    
			  utPltPutVar(psDbHead,"id",caSid);
				utPltPutVar(psDbHead,"name",caName);
				utPltPutVar(psDbHead,"scode",caScode);
				utPltPutVarF(psDbHead,"sgroupid","%lu",lSgroupid);
				utPltPutVar(psDbHead,"imgpath",caImgpath);
				utPltPutVar(psDbHead,"floor",caFloor);
				utPltPutVar(psDbHead,"url",caUrl);
		    utPltPutVar(psDbHead,"code",caCode);
		    utPltPutVar(psDbHead,"tel",caTel);
		    utPltPutVar(psDbHead,"address",caAddress);
		    utPltPutVarF(psDbHead,"level","%lu",lLevel);
		    utPltPutVarF(psDbHead,"plevel","%lu",lPlevel);
		    utPltPutVarF(psDbHead,"type","%lu",lType);
		    utPltPutVarF(psDbHead,"stype","%lu",lStype);
		    utPltPutVar(psDbHead,"descr",caDescr);
		    utPltPutVar(psDbHead,"detail",caDetail);
		    utPltPutVarF(psDbHead,"systime","%lu",lSystime);
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomshop_dr.htm");
	 return 0;		
}

// 上传商店图片
int ncm_webComShopImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32]="";
   	char caId[16]="";
    char caName[32]="";
    char caScode[16]="";
    char caSgroupid[32]="";
    unsigned long lSgroupid=0;
    char caImgPath[128]="";
    char caDispImgPath[128]="";
    char imgname[128]="";
    char caFloor[32]="";
    char caUrl[255]="";
    char caCode[12]="";
    char caTel[32]="";
    char caAddress[255]="";
    char caLevel[20]="";
    char caEtime[32]="";
    char caPlevel[20]="";
    char caType[20]="";
    char caStype[20]="";
    char caDescr[255]="";
    char caDetail[4096]="";
    
    char caImgpath1[128]="";
    char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
 		char caTemp2[1024]="";
 		char caTemp3[1024]="";
 		char caCompTemp[1024]="";
 		unsigned long lCount=0;
 		unsigned long lMaxid = 0;
 		unsigned long lMaxcompid = 0;
 		char *p;
 		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
		
		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0194导入"),ncLang("0183打开文件出错"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// 建立图片存放路径
  	sprintf(caImgPath,"%s/images/ShopImages/shopimages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);
        
	  utMsgGetSomeNVar(psMsgHead,18,"key",       UT_TYPE_STRING,  sizeof(caKey)-1,            caKey,
	                               "id",         UT_TYPE_STRING,  sizeof(caId)-1,             caId,
	  														 "name",       UT_TYPE_STRING,  sizeof(caName)-1,           caName,
	                               "scode",      UT_TYPE_STRING,  sizeof(caScode)-1,          caScode,
	                               "tyid",       UT_TYPE_STRING,  sizeof(caSgroupid)-1,       caSgroupid,
	                               "floor",      UT_TYPE_STRING,  sizeof(caFloor)-1,          caFloor,
	                               "url",        UT_TYPE_STRING,  sizeof(caUrl)-1,            caUrl,
	                               "code",       UT_TYPE_STRING,  sizeof(caCode)-1,           caCode,
	                               "tel",        UT_TYPE_STRING,  sizeof(caTel)-1,            caTel,
	                               "address",    UT_TYPE_STRING,  sizeof(caAddress)-1,        caAddress,
	                               "level",      UT_TYPE_STRING,  sizeof(caLevel)-1,          caLevel,
	                               "etime",      UT_TYPE_STRING,  sizeof(caEtime)-1,          caEtime,
	                               "plevel",     UT_TYPE_STRING,  sizeof(caPlevel)-1,         caPlevel,
	                               "type",       UT_TYPE_STRING,  sizeof(caType)-1,           caType,
	                               "stype",      UT_TYPE_STRING,  sizeof(caStype)-1,          caStype,
																 "fname",      UT_TYPE_STRING,  255,                        imp_file,
																 "descr",      UT_TYPE_STRING,  sizeof(caDescr)-1,          caDescr,
																 "detail",     UT_TYPE_STRING,  sizeof(caDetail)-1,         caDetail);
	  
	  if(strlen(caName)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","商店名称不能为空!","商店名称不能为空!");
        return 0;	
	  }
	                  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*从指定的字符串中取一个词
		  	
		  	  函数原型：
			　　char *utStrGetWord(char *p,char *pOut,
			　　                   short nMaxLen,char *pTerm);
			　　
					参数说明:
			　　p – 输入字符串
			　　pOut --- 输出字符串，必须要事先分配好空间
			　　nMaxLen --- 输出字符传的最大长度
			　　pTerm  ----- 终止条件的字符串，系统在扫描描时遇到该字符串某一个字符时结束。
			　　
			　　返回值：
			　　结束字符的指针。*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
				printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
					  memset(caDispImgPath, 0, sizeof(caDispImgPath));
				}else{
						sprintf(imgname, "shop_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/shopimages/");
				    strcat(caDispImgPath, imgname);	
				}
	  }
	  
	  unsigned long lShopId = 0;
	  dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	  
	  if(lShopId>0){
			  if(strcmp(caKey, "update")==0){
			  	  sprintf(caTemp2, "select imgpath from ncmshop where id=%lu", atol(caId));
			  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
			  	  if(strlen(caDispImgPath)==0){ // 是否选择上传图片, 否：原图片不变
			  	  	  sprintf(caTemp,"update ncmshop set id=%lu, name='%s',service_code='%s',sgroupid=%lu,imgpath='%s',floor='%s',url='%s',code='%s',tel='%s',address='%s',level=%lu,plevel=%lu,type=%lu,stype=%lu,descr='%s',detail='%s',systime=%lu where id=%lu",lShopId,caName,caScode,atol(caSgroupid),caImgpath1,caFloor,caUrl,caCode,caTel,caAddress,atol(caLevel),atol(caPlevel),atol(caType),atol(caStype),caDescr,caDetail,time(0),atol(caId));
							 	printf("caTemp = %s\n", caTemp);
							 	pasDbExecSqlF(caTemp);
							 	utPltPutVar(psDbHead,"imgpath",caImgpath1);
							 	
							 	// 对照表
							 	sprintf(caCompTemp, "update ncmshoptype set shopid=%lu, shopname='%s', imgpath='%s', level=%lu, systime=%lu where shopid=%lu",lShopId, caName, caImgpath1, atol(caLevel), time(0), atol(caId));
							 	printf("caCompTemp = %s\n", caCompTemp);
							 	pasDbExecSqlF(caCompTemp);
			  	  }else{ // 是：原图片删除，新图片加入
					  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
						  	system(temp3_file);
					  	  
					  	  sprintf(caTemp,"update ncmshop set id=%lu, name='%s',service_code='%s',sgroupid=%lu,imgpath='%s',floor='%s',url='%s',code='%s',tel='%s',address='%s',level=%lu,plevel=%lu,type=%lu,stype=%lu,descr='%s',detail='%s',systime=%lu where id=%lu",lShopId,caName,caScode,atol(caSgroupid),caDispImgPath,caFloor,caUrl,caCode,caTel,caAddress,atol(caLevel),atol(caPlevel),atol(caType),atol(caStype),caDescr,caDetail,time(0),atol(caId));
							 	printf("caTemp = %s\n", caTemp);
							 	pasDbExecSqlF(caTemp);
							 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
							 	
							 	// 对照表
							 	sprintf(caCompTemp, "update ncmshoptype set shopid=%lu, shopname='%s', imgpath='%s', level=%lu, systime=%lu where shopid=%lu",lShopId, caName, caDispImgPath, atol(caLevel), time(0), atol(caId));
							 	printf("caCompTemp = %s\n", caCompTemp);
							 	pasDbExecSqlF(caCompTemp);
					  }
			  }else if(strcmp(caKey, "add")==0){
						//pasDbOneRecord("select max(id) from ncmshop", 0, UT_TYPE_ULONG, 4, &lMaxid);
				    sprintf(caTemp,"insert into ncmshop (id,name,service_code,sgroupid,imgpath,floor,url,code,tel,address,level,plevel,type,stype,descr,detail,systime) values (%lu,'%s','%s',%lu,'%s','%s','%s','%s','%s','%s',%lu,%lu,%lu,%lu,'%s','%s',%lu)",lShopId,caName,caScode,atol(caSgroupid),caDispImgPath,caFloor,caUrl,caCode,caTel,caAddress,atol(caLevel),atol(caPlevel),atol(caType),atol(caStype),caDescr,caDetail,time(0));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
					 	
					 	// 对照表
					 	pasDbOneRecord("select max(id) from ncmshoptype", 0, UT_TYPE_ULONG, 4, &lMaxcompid);
					 	sprintf(caCompTemp, "insert into ncmshoptype (id, shopid, shopname, imgpath, level, systime) values (%lu, %lu, '%s', '%s', %lu, %lu)", (lMaxcompid+1), lShopId, caName, caDispImgPath, atol(caLevel), time(0));
			  		printf("caCompTemp = %s\n", caCompTemp);
			  		pasDbExecSqlF(caCompTemp);
			  }
	  }
	  else{
	  	  if(strcmp(caKey, "update")==0){
			  	  sprintf(caTemp2, "select imgpath from ncmshop where id=%lu", atol(caId));
			  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
			  	  if(strlen(caDispImgPath)==0){ // 是否选择上传图片, 否：原图片不变
			  	  	  sprintf(caTemp,"update ncmshop set name='%s',service_code='%s',sgroupid=%lu,imgpath='%s',floor='%s',url='%s',code='%s',tel='%s',address='%s',level=%lu,plevel=%lu,type=%lu,stype=%lu,descr='%s',detail='%s',systime=%lu where id=%lu",caName,caScode,atol(caSgroupid),caImgpath1,caFloor,caUrl,caCode,caTel,caAddress,atol(caLevel),atol(caPlevel),atol(caType),atol(caStype),caDescr,caDetail,time(0),atol(caId));
							 	printf("caTemp = %s\n", caTemp);
							 	pasDbExecSqlF(caTemp);
							 	utPltPutVar(psDbHead,"imgpath",caImgpath1);
							 	
							 	// 对照表
							 	sprintf(caCompTemp, "update ncmshoptype set shopname='%s', imgpath='%s', level=%lu, systime=%lu where shopid=%lu", caName, caImgpath1, atol(caLevel), time(0), atol(caId));
							 	printf("caCompTemp = %s\n", caCompTemp);
							 	pasDbExecSqlF(caCompTemp);
			  	  }else{ // 是：原图片删除，新图片加入
					  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
						  	system(temp3_file);
					  	  
					  	  sprintf(caTemp,"update ncmshop set name='%s',service_code='%s',sgroupid=%lu,imgpath='%s',floor='%s',url='%s',code='%s',tel='%s',address='%s',level=%lu,plevel=%lu,type=%lu,stype=%lu,descr='%s',detail='%s',systime=%lu where id=%lu",caName,caScode,atol(caSgroupid),caDispImgPath,caFloor,caUrl,caCode,caTel,caAddress,atol(caLevel),atol(caPlevel),atol(caType),atol(caStype),caDescr,caDetail,time(0),atol(caId));
							 	printf("caTemp = %s\n", caTemp);
							 	pasDbExecSqlF(caTemp);
							 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
							 	
							 	// 对照表
							 	sprintf(caCompTemp, "update ncmshoptype set shopname='%s', imgpath='%s', level=%lu, systime=%lu where shopid=%lu",caName, caDispImgPath, atol(caLevel), time(0), atol(caId));
							 	printf("caCompTemp = %s\n", caCompTemp);
							 	pasDbExecSqlF(caCompTemp);
					  }
			  }
	  }
	  
	  utPltPutVar(psDbHead,"mesg","数据加载成功");
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomodity_dr_list.htm");
	  //utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","数据加载成功!","数据加载成功!");
    return 0;
}

// 商城用户信息
int ncm_webcomshopuser_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char uid[20] = "";
	 char username[32] = "";
	 char dispname[32] = "";
	 char groupname[16] = "";
	 char proname[16] = "";
	 char usermac[18] = "";
	 char password[32] = "";
	 char nickname[32] = "";
	 char membernum[32] = "";
	 char integral[20] = "";
	 char coupon[20] = "";
	 char address[128] = "";
	 char decript[4096] = "";
	 char lcount[20] = "";
	 char imgpath[128] = "";
	 char addtime[32] = "";
	 char lasttime[32] = "";
	 char flags[32] = "";
	 char temp_file[128] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lUid = 0;
	 char 					caUsername[32] = "";
	 char           caDispname[32] = "";
	 char           caGroupname[16] = "";
	 char 					caProname[16] = "";
	 char 					caUsermac[18] = "";
	 char 					caPassword[32] = "";
	 char 					caNickname[32] = "";
	 char 					caMembernum[32] = "";
	 unsigned long	lIntegral = 0;
	 unsigned long	lCoupon = 0;
	 char 					caAddress[128] = "";
	 char 					caDecript[4096] = "";
	 unsigned long	lLcount = 0;
	 char 					caImgpath[128] = "";
	 unsigned long	lAddtime = 0;
	 unsigned long	lLasttime = 0;
	 unsigned long	lFlags = 0;
	 char           s_addtime[32] = "";
	 char           s_lasttime[32] = "";
	 char           s_flags[32] = "";
	 
	 char insert[16] = "";
	 char caInsert[16] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 unsigned long lAddTime = 0;
	 unsigned long lLastTime = 0;
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 28,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"uid",         UT_TYPE_STRING,  sizeof(uid)-1,            uid,
			"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
			"dispname",    UT_TYPE_STRING,  sizeof(dispname)-1,       dispname,
			"groupname",   UT_TYPE_STRING,  sizeof(groupname)-1,      groupname,
			"proname",     UT_TYPE_STRING,  sizeof(proname)-1,        proname,
			"usermac",     UT_TYPE_STRING,  sizeof(usermac)-1,        usermac,
			"password",    UT_TYPE_STRING,  sizeof(password)-1,       password,
			"nickname",    UT_TYPE_STRING,  sizeof(nickname)-1,       nickname,
			"membernum",   UT_TYPE_STRING,  sizeof(membernum)-1,      membernum,
			"integral",    UT_TYPE_STRING,  sizeof(integral)-1,       integral,
			"coupon",      UT_TYPE_STRING,  sizeof(coupon)-1,         coupon,
			"address",     UT_TYPE_STRING,  sizeof(address)-1,        address,
			"decript",     UT_TYPE_STRING,  sizeof(decript)-1,        decript,
			"lcount",      UT_TYPE_STRING,  sizeof(lcount)-1,         lcount,
			"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
			"addtime",     UT_TYPE_STRING,  sizeof(addtime)-1,        addtime,
			"lasttime",    UT_TYPE_STRING,  sizeof(lasttime)-1,       lasttime,
			"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"insert",      UT_TYPE_STRING,  10,                       caInsert,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,30);
			strcpy(username,caTemp);
   }
   if(strlen(dispname)>0){
			pasCvtGBK(2,dispname,caTemp,30);
			strcpy(dispname,caTemp);
   }
   if(strlen(groupname)>0){
			pasCvtGBK(2,groupname,caTemp,30);
			strcpy(groupname,caTemp);
   }
   if(strlen(proname)>0){
			pasCvtGBK(2,proname,caTemp,30);
			strcpy(proname,caTemp);
   }
   if(strlen(nickname)>0){
			pasCvtGBK(2,nickname,caTemp,30);
			strcpy(nickname,caTemp);
   }
   if(strlen(membernum)>0){
			pasCvtGBK(2,membernum,caTemp,30);
			strcpy(membernum,caTemp);
   }
   if(strlen(address)>0){
			pasCvtGBK(2,address,caTemp,30);
			strcpy(address,caTemp);
   }
   if(strlen(decript)>0){
			pasCvtGBK(2,decript,caTemp,30);
			strcpy(decript,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,30);
			strcpy(imgpath,caTemp);
   }
   if(strlen(addtime)>0){
			pasCvtGBK(2,addtime,caTemp,30);
			strcpy(addtime,caTemp);
   }
   if(strlen(lasttime)>0){
			pasCvtGBK(2,lasttime,caTemp,30);
			strcpy(lasttime,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
   
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char 	imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmshopuser where uid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
				 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
			       return 0;	
				  }
    		  
					// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
					iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);		
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);	
				  }
					pasDbExecSqlF("delete from ncmshopuser where uid in(%s)",caSesid);
    	}
	 }
	 // 查询
	 strcpy(sql, "select count(*) from ncmshopuser where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%') ", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select uid, username, dispname, groupname, proname, usermac, password, nickname, membernum, integral, coupon, address, decript, lcount, imgpath, addtime, lasttime, flags from ncmshopuser where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%') ", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),         &lUid,
													     UT_TYPE_STRING, sizeof(caUsername)-1, caUsername,
													     UT_TYPE_STRING, sizeof(caDispname)-1, caDispname,
													     UT_TYPE_STRING, sizeof(caGroupname)-1,caGroupname,
													     UT_TYPE_STRING, sizeof(caProname)-1,  caProname,
													     UT_TYPE_STRING, sizeof(caUsermac)-1,  caUsermac,
													     UT_TYPE_STRING, sizeof(caPassword)-1, caPassword,
													     UT_TYPE_STRING, sizeof(caNickname)-1, caNickname,
													     UT_TYPE_STRING, sizeof(caMembernum)-1,caMembernum,
													     UT_TYPE_ULONG,  sizeof(long),         &lIntegral,
													     UT_TYPE_ULONG,  sizeof(long),         &lCoupon,
													     UT_TYPE_STRING, sizeof(caAddress)-1,  caAddress,
													     UT_TYPE_STRING, sizeof(caDecript)-1,  caDecript,
													     UT_TYPE_ULONG,  sizeof(long),         &lLcount,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,  caImgpath,
													     UT_TYPE_ULONG,  sizeof(long),         &lAddtime,
													     UT_TYPE_ULONG,  sizeof(long),         &lLasttime,
															 UT_TYPE_ULONG,  sizeof(long),         &lFlags);
	
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
		   
		   if(lAddtime){
		   		strcpy(s_addtime, utTimFormat("%Y/%m/%d %H:%M:%S", lAddtime));
		   }else{
		   		strcpy(s_addtime, "");	
		   }
		   if(lLasttime){
		   	  strcpy(s_lasttime, utTimFormat("%Y/%m/%d %H:%M:%S", lLasttime));
		   }else{
		   		strcpy(s_lasttime, "");	
		   }
		   switch(lFlags){
		   		case 0:strcpy(s_flags, "没有访问过");break;
		   		case 1:strcpy(s_flags, "已经访问过");break;
		   		default:break;	
		   }
		   
			 utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
			 utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
			 utPltPutLoopVar(psDbHead,"proname",iNum,caProname);
			 utPltPutLoopVar(psDbHead,"usermac",iNum,caUsermac);
			 utPltPutLoopVar(psDbHead,"password",iNum,caPassword);
			 utPltPutLoopVar(psDbHead,"nickname",iNum,caNickname);
			 utPltPutLoopVar(psDbHead,"membernum",iNum,caMembernum);
			 utPltPutLoopVarF(psDbHead,"integral",iNum,"%lu",lIntegral);
			 utPltPutLoopVarF(psDbHead,"coupon",iNum,"%lu",lCoupon);
			 utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
			 utPltPutLoopVar(psDbHead,"decript",iNum,caDecript);
			 utPltPutLoopVarF(psDbHead,"lcount",iNum,"%lu",lLcount);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"addtime",iNum,s_addtime);
			 utPltPutLoopVar(psDbHead,"lasttime",iNum,s_lasttime);
			 utPltPutLoopVar(psDbHead,"flags",iNum,s_flags);
			 
			 lUid = 0;
			 memset(caUsername, 0, sizeof(caUsername));
			 memset(caDispname, 0, sizeof(caDispname));
			 memset(caGroupname, 0, sizeof(caGroupname));
			 memset(caProname, 0, sizeof(caProname));
			 memset(caUsermac, 0, sizeof(caUsermac));
			 memset(caPassword, 0, sizeof(caPassword));
			 memset(caNickname, 0, sizeof(caNickname));
			 memset(caMembernum, 0, sizeof(caMembernum));
			 lIntegral = 0;
			 lCoupon = 0;
			 memset(caAddress, 0, sizeof(caAddress));
			 memset(caDecript, 0, sizeof(caDecript));
			 lLcount = 0;
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lAddtime = 0;
			 lLasttime = 0;
			 lFlags = 0;
			 memset(s_addtime, 0, sizeof(s_addtime));
			 memset(s_lasttime, 0, sizeof(s_lasttime));
			 memset(s_flags, 0, sizeof(s_flags));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),         &lUid,
															     UT_TYPE_STRING, sizeof(caUsername)-1, caUsername,
															     UT_TYPE_STRING, sizeof(caDispname)-1, caDispname,
															     UT_TYPE_STRING, sizeof(caGroupname)-1,caGroupname,
															     UT_TYPE_STRING, sizeof(caProname)-1,  caProname,
															     UT_TYPE_STRING, sizeof(caUsermac)-1,  caUsermac,
															     UT_TYPE_STRING, sizeof(caPassword)-1, caPassword,
															     UT_TYPE_STRING, sizeof(caNickname)-1, caNickname,
															     UT_TYPE_STRING, sizeof(caMembernum)-1,caMembernum,
															     UT_TYPE_ULONG,  sizeof(long),         &lIntegral,
															     UT_TYPE_ULONG,  sizeof(long),         &lCoupon,
															     UT_TYPE_STRING, sizeof(caAddress)-1,  caAddress,
															     UT_TYPE_STRING, sizeof(caDecript)-1,  caDecript,
															     UT_TYPE_ULONG,  sizeof(long),         &lLcount,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,  caImgpath,
															     UT_TYPE_ULONG,  sizeof(long),         &lAddtime,
															     UT_TYPE_ULONG,  sizeof(long),         &lLasttime,
																	 UT_TYPE_ULONG,  sizeof(long),         &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomshopuser_list.htm");
	  return 0;	
}

// 消费者个人信息
int ncm_webcomconsump_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char cid[20] = "";
	 char username[32] = "";
	 char dispname[32] = "";
	 char usermac[18] = "";
	 char sname[64] = "";
	 char sid[20] = "";
	 char fee[20] = "";
	 char date[32] = "";
	 char uid[20] = "";
	 char lasttime[32] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lCid = 0;
	 char 					caUsername[32] = "";
	 char 					caDispname[32] = "";
	 char 					caUsermac[18] = "";
	 char 					caSname[64] = "";
	 unsigned long	lSid = 0;
	 double	        dFfee = 0.0;
	 unsigned long	lDate = 0;
	 unsigned long	lUid = 0;
	 unsigned long	lLasttime = 0;
	 char           s_lasttime[32] = "";
	 char           s_date[32] = "";
	 
	 char insert[16] = "";
	 char caInsert[16] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 20,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,     _keyword,
			"cid",         UT_TYPE_STRING,  sizeof(cid)-1,          cid,
			"username",    UT_TYPE_STRING,  sizeof(username)-1,     username,
			"dispname",    UT_TYPE_STRING,  sizeof(dispname)-1,     dispname,
			"usermac",     UT_TYPE_STRING,  sizeof(usermac)-1,      usermac,
			"sname",       UT_TYPE_STRING,  sizeof(sname)-1,        sname,
			"sid",    	   UT_TYPE_STRING,  sizeof(sid)-1,          sid,
			"fee",    	   UT_TYPE_STRING,  sizeof(fee)-1,          fee,
			"date",    	   UT_TYPE_STRING,  sizeof(date)-1,         date,
			"uid",    	   UT_TYPE_STRING,  sizeof(uid)-1,          uid,
			"lasttime",    UT_TYPE_STRING,  sizeof(lasttime)-1,     lasttime,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,        start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,        limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,         sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,          dir,
			"insert",      UT_TYPE_STRING,  10,                     caInsert,
			"selsid",      UT_TYPE_STRING,  1000,                   caSesid,
			"del",				 UT_TYPE_STRING,  10,                     caDel,
			"update",      UT_TYPE_STRING,  10,                     caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                     caExport
	 );
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,30);
			strcpy(username,caTemp);
   }
   if(strlen(dispname)>0){
			pasCvtGBK(2,dispname,caTemp,30);
			strcpy(dispname,caTemp);
   }
	 if(strlen(sname)>0){
			pasCvtGBK(2,sname,caTemp,30);
			strcpy(sname,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"cid");
	 }
   
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
	 		if(strlen(caSesid)>0){
    			printf("caSesid = %s\n", caSesid);
    		  pasDbExecSqlF("delete from ncmconsump where cid in(%s)",caSesid);
    	}
	 }
	 // 查询
	 strcpy(sql, "select count(*) from ncmconsump where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%') ", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select cid, username, dispname, usermac, sname, sid, fee, date, uid, lasttime from ncmconsump where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%') ", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),        &lCid,
													     UT_TYPE_STRING, sizeof(caUsername)-1,caUsername,
													     UT_TYPE_STRING, sizeof(caDispname)-1,caDispname,
													     UT_TYPE_STRING, sizeof(caUsermac)-1, caUsermac,
													     UT_TYPE_STRING, sizeof(caSname)-1,   caSname,
													     UT_TYPE_ULONG,  sizeof(long),        &lSid,
													     UT_TYPE_FLOAT,  sizeof(double),      &dFfee,
													     UT_TYPE_ULONG,  sizeof(long),        &lDate,
													     UT_TYPE_ULONG,  sizeof(long),        &lUid,
															 UT_TYPE_ULONG,  sizeof(long),        &lLasttime);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
		   
		   if(lLasttime>0){
		   		strcpy(s_lasttime, utTimFormat("%Y/%m/%d %H:%M:%S", lLasttime));
		   }else{
		   		strcpy(s_lasttime, "");	
		   }
		   if(lDate>0){
		   		strcpy(s_date, utTimFormat("%Y/%m/%d", lDate));
		   }else{
		   		strcpy(s_date, "");
		   }
		   
			 utPltPutLoopVarF(psDbHead,"cid",iNum,"%lu",lCid);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
			 utPltPutLoopVar(psDbHead,"usermac",iNum,caUsermac);
			 utPltPutLoopVar(psDbHead,"sname",iNum,caSname);
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVarF(psDbHead,"fee",iNum,"%.2f",dFfee);
			 utPltPutLoopVar(psDbHead,"date",iNum,s_date);
			 utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
			 utPltPutLoopVar(psDbHead,"lasttime",iNum,s_lasttime);
			 
			 lCid = 0;
			 memset(caUsername, 0, sizeof(caUsername));
			 memset(caDispname, 0, sizeof(caDispname));
			 memset(caUsermac, 0, sizeof(caUsermac));
			 memset(caSname, 0, sizeof(caSname));
			 lSid = 0;
			 dFfee = 0.0;
			 lDate = 0;
			 lUid = 0;
			 lLasttime = 0;
			 memset(s_lasttime, 0, sizeof(s_lasttime));
			 memset(s_date, 0, sizeof(s_date));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),        &lCid,
															     UT_TYPE_STRING, sizeof(caUsername)-1,caUsername,
															     UT_TYPE_STRING, sizeof(caDispname)-1,caDispname,
															     UT_TYPE_STRING, sizeof(caUsermac)-1, caUsermac,
															     UT_TYPE_STRING, sizeof(caSname)-1,   caSname,
															     UT_TYPE_ULONG,  sizeof(long),        &lSid,
															     UT_TYPE_FLOAT,  sizeof(double),      &dFfee,
															     UT_TYPE_ULONG,  sizeof(long),        &lDate,
															     UT_TYPE_ULONG,  sizeof(long),        &lUid,
																	 UT_TYPE_ULONG,  sizeof(long),        &lLasttime);
	  }
	  //utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomconsump_list.htm");
	  return 0;	
}

// 优惠劵信息
int ncm_webcomcoupon_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char cid[20] = "";
	 char fee[20] = "";
	 char counum[20] = "";
	 char addtime[32] = "";
	 char imgpath[128] = "";
	 char effetime[32] = "";
	 char temp_file[128] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lCid = 0;
	 double         dFee = 0.0;
	 unsigned long	lCounum = 0;
	 unsigned long  lAddtime = 0;
	 char 					caImgpath[128] = "";
	 unsigned long  lEffetime = 0;
	 char           s_addtime[32] = "";
	 char           s_effetime[32] = "";
	 
	 char insert[16] = "";
	 char caInsert[16] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 16,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"cid",         UT_TYPE_STRING,  sizeof(cid)-1,            cid,
			"fee",         UT_TYPE_STRING,  sizeof(fee)-1,            fee,
			"counum",      UT_TYPE_STRING,  sizeof(counum)-1,         counum,
			"addtime",     UT_TYPE_STRING,  sizeof(addtime)-1,        addtime,
			"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
			"effetime",    UT_TYPE_STRING,  sizeof(effetime)-1,       effetime,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"insert",      UT_TYPE_STRING,  10,                       caInsert,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,30);
			strcpy(imgpath,caTemp);
   }
   if(strlen(addtime)>0){
			pasCvtGBK(2,addtime,caTemp,30);
			strcpy(addtime,caTemp);
   }
   if(strlen(effetime)>0){
			pasCvtGBK(2,effetime,caTemp,30);
			strcpy(effetime,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"cid");
	 }
   
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char 	imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcoupon where cid in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
				 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
			       return 0;	
				  }
				  
				  // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
    		  
    		  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmcoupon where cid in(%s)",caSesid);
    	}
	 }
	 
	 // 查询
	 strcpy(sql, "select count(*) from ncmcoupon where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (fee like '%%%s%%') ", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select cid, fee, counum, addtime, imgpath, effetime from ncmcoupon where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (fee like '%%%s%%') ", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lCid,
													     UT_TYPE_FLOAT,  sizeof(double),        &dFee,
													     UT_TYPE_ULONG,  sizeof(long),          &lCounum,
															 UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
													     UT_TYPE_ULONG,  sizeof(long),          &lEffetime);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				  utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 
			 if(lAddtime>0){
			 		strcpy(s_addtime, utTimFormat("%Y/%m/%d", lAddtime));
			 }else{
			 		strcpy(s_addtime, "");	
			 }
			 if(lEffetime>0){
			 		strcpy(s_effetime, utTimFormat("%Y/%m/%d", lEffetime));	
			 }else{
			 		strcpy(s_effetime, "");
			 }
			 
			 utPltPutLoopVarF(psDbHead,"cid",iNum,"%lu",lCid);
			 utPltPutLoopVarF(psDbHead,"fee",iNum,"%.2f",dFee);
			 utPltPutLoopVarF(psDbHead,"counum",iNum,"%lu",lCounum);
			 utPltPutLoopVar(psDbHead,"addtime",iNum,s_addtime);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"effetime",iNum,s_effetime);
			 
			 lCid = 0;
			 dFee = 0.0;
			 lCounum = 0;
			 lAddtime = 0;
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lEffetime = 0;
			 memset(s_addtime, 0, sizeof(s_addtime));
			 memset(s_effetime, 0, sizeof(s_effetime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lCid,
															     UT_TYPE_FLOAT,  sizeof(double),        &dFee,
															     UT_TYPE_ULONG,  sizeof(long),          &lCounum,
																	 UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
															     UT_TYPE_ULONG,  sizeof(long),          &lEffetime);
		}
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomcoupon_list.htm");
	  return 0;	
}

int ncm_webcomcoupon_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
//#endif  
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char caKey[32] = "";
	 char caSid[32] = "";
	 double dFee = 0.0;
	 unsigned long lCounum = 0;
	 unsigned long lAddtime = 0;
	 char caImgpath[128] = "";
	 unsigned long lEffetime = 0;
	 char s_addtime[32] = "";
	 char s_effetime[32] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 2, "key", UT_TYPE_STRING, sizeof(caKey)-1, caKey,
	                                "sid", UT_TYPE_STRING, sizeof(caSid)-1, caSid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
			 sprintf(sql, "select fee, counum, addtime, imgpath, effetime from ncmcoupon where cid=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_FLOAT,sizeof(double),&dFee,
			 											UT_TYPE_ULONG,4,&lCounum,
		                        UT_TYPE_ULONG,4,&lAddtime,
		                        UT_TYPE_STRING,127,caImgpath,
		                        UT_TYPE_ULONG,4,&lEffetime);
		                        
			  strcpy(s_addtime, utTimFormat("%Y/%m/%d", lAddtime));
			  strcpy(s_effetime, utTimFormat("%Y/%m/%d", lEffetime));
			  
			  utPltPutVar(psDbHead,"cid",caSid);
			  utPltPutVarF(psDbHead,"fee","%.2f",dFee);
			  utPltPutVarF(psDbHead,"counum","%lu",lCounum);
				utPltPutVar(psDbHead,"addtime",s_addtime);
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVar(psDbHead,"effetime",s_effetime);
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomcoupon_dr.htm");
	 return 0;		
}

// 上传优惠劵图片
int ncm_webComCouponImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32]="";
   	char caCid[16]="";
    char caFee[20]="";
    char caCounum[20]="";
    char caAddtime[32]="";
    char caEffetime[32]="";
    char caImgPath[128]="";
    char caDispImgPath[128]="";
    char imgname[128]="";
    
    char caImgpath1[128]="";
    
    char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
 		char caTemp2[1024]="";
 		unsigned long lAddTime=0;
 		unsigned long lEffeTime=0;
 		unsigned long lMaxid = 0;
 		char *p;
 		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
		
		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0194导入"),ncLang("0183打开文件出错"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// 建立图片存放路径
  	sprintf(caImgPath,"%s/images/ShopImages/couponimages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);
        
	  utMsgGetSomeNVar(psMsgHead,7,"key",      UT_TYPE_STRING,  sizeof(caKey)-1,     caKey,
	                               "cid",      UT_TYPE_STRING,  sizeof(caCid)-1,     caCid,
	  														 "fee",      UT_TYPE_STRING,  sizeof(caFee)-1,     caFee,
	  														 "counum",   UT_TYPE_STRING,  sizeof(caCounum)-1,  caCounum,
	                               "addtime",  UT_TYPE_STRING,  sizeof(caAddtime)-1, caAddtime,
	                               "effetime", UT_TYPE_STRING,  sizeof(caEffetime)-1,caEffetime,
																 "fname",    UT_TYPE_STRING,  255,                 imp_file);
	  
	  if(strlen(caFee)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","优惠劵面值不能为空!","优惠劵面值不能为空!");
        return 0;	
	  }
	  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*从指定的字符串中取一个词
		  	
		  	  函数原型：
			　　char *utStrGetWord(char *p,char *pOut,
			　　                   short nMaxLen,char *pTerm);
			　　
					参数说明:
			　　p – 输入字符串
			　　pOut --- 输出字符串，必须要事先分配好空间
			　　nMaxLen --- 输出字符传的最大长度
			　　pTerm  ----- 终止条件的字符串，系统在扫描描时遇到该字符串某一个字符时结束。
			　　
			　　返回值：
			　　结束字符的指针。*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
				printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
					  memset(caDispImgPath, 0, sizeof(caDispImgPath));
				}else{
						sprintf(imgname, "coupon_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/couponimages/");
				    strcat(caDispImgPath, imgname);	
				}
	  }
	  
	  ncTimeToLong(caAddtime, &lAddTime);
	  ncTimeToLong(caEffetime, &lEffeTime);
	  
	  if(strcmp(caKey, "update")==0){
	  	  sprintf(caTemp2, "select imgpath from ncmcoupon where cid=%lu", atol(caCid));
	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  if(strlen(caDispImgPath)==0){ // 是否选择上传图片, 否：原图片不变
	  	  	  sprintf(caTemp,"update ncmcoupon set fee=%.2f,counum=%lu,addtime=%lu,imgpath='%s',effetime=%lu where cid=%lu",atof(caFee),atol(caCounum),lAddTime,caImgpath1,lEffeTime,atol(caCid));
			 			printf("caTemp = %s\n", caTemp);
			 			pasDbExecSqlF(caTemp);
			 			utPltPutVar(psDbHead,"imgpath",caImgpath1);
	  	  }else{ // 是：原图片删除，新图片加入
			  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1);
				  	system(temp3_file);
			  	  
			  	  sprintf(caTemp,"update ncmcoupon set fee=%.2f,counum=%lu,addtime=%lu,imgpath='%s',effetime=%lu where cid=%lu",atof(caFee),atol(caCounum),lAddTime,caDispImgPath,lEffeTime,atol(caCid));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
			  }
	  }else if(strcmp(caKey, "add")==0){
	  	  pasDbOneRecord("select max(cid) from ncmcoupon", 0, UT_TYPE_ULONG, 4, &lMaxid);
		    sprintf(caTemp,"insert into ncmcoupon (cid,fee,counum,addtime,imgpath,effetime) values (%lu,%.2f,%lu,%lu,'%s',%lu)",(lMaxid+1),atof(caFee),atol(caCounum),lAddTime,caDispImgPath,lEffeTime);
			 	printf("caTemp = %s\n", caTemp);
			 	pasDbExecSqlF(caTemp);
			 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
	  }
	  utPltPutVar(psDbHead,"mesg","数据加载成功");
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomcoupon_dr_list.htm");
	  //utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","数据加载成功!","数据加载成功!");
    return 0;
}

// 优惠劵使用表信息
int ncm_webcomcouponuse_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char id[20] = "";
	 char scode[16] = "";
	 char username[32] = "";
	 char aid[20] = "";
	 char aname[32] = "";
	 char serno[32] = "";
	 char usermac[18] = "";
	 char sname[32] = "";
	 char feetime[20] = "";
	 char uid[20] = "";
	 char sid[20] = "";
	 char fee[20] = "";
	 char imgpath[128] = "";
	 char systime[32] = "";
	 char flags[20] = "";
	 char temp_file[128] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lId = 0;
	 char           caScode[16] = "";
	 char 					caUsername[32] = "";
	 unsigned long  lAid = 0;
	 char           caAname[32] = "";
	 char           caSerno[32] = "";
	 char 					caUsermac[18] = "";
	 char 					caSname[32] = "";
	 unsigned long	lFeetime = 0;
	 unsigned long	lUid = 0;
	 unsigned long	lSid = 0;
	 double         dFee = 0.0;
	 char 					caImgpath[128] = "";
	 unsigned long	lSystime = 0;
	 unsigned long	lFlags = 0;
	 char           s_feetime[32] = "";
	 char           s_systime[32] = "";
	 char           s_flags[32] = "";
	 
	 char insert[16] = "";
	 char caInsert[16] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 unsigned long lFeeTime = 0;
	 unsigned long lSysTime = 0;
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 25,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"id",          UT_TYPE_STRING,  sizeof(id)-1,             id,
			"scode",       UT_TYPE_STRING,  sizeof(scode)-1,          scode,
			"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
			"aid",         UT_TYPE_STRING,  sizeof(aid)-1,            aid,
			"aname",       UT_TYPE_STRING,  sizeof(aname)-1,          aname,
			"serno",       UT_TYPE_STRING,  sizeof(serno)-1,          serno,
			"usermac",     UT_TYPE_STRING,  sizeof(usermac)-1,        usermac,
			"sname",       UT_TYPE_STRING,  sizeof(sname)-1,          sname,
			"feetime",     UT_TYPE_STRING,  sizeof(feetime)-1,        feetime,
			"uid",         UT_TYPE_STRING,  sizeof(uid)-1,            uid,
			"sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
			"fee",         UT_TYPE_STRING,  sizeof(fee)-1,            fee,
			"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
			"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
			"flags",       UT_TYPE_STRING,  sizeof(flags)-1,          flags,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"insert",      UT_TYPE_STRING,  10,                       caInsert,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(scode)>0){
			pasCvtGBK(2,scode,caTemp,30);
			strcpy(scode,caTemp);
   }
   if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,30);
			strcpy(username,caTemp);
   }
   if(strlen(aname)>0){
			pasCvtGBK(2,aname,caTemp,30);
			strcpy(aname,caTemp);
   }
   if(strlen(sname)>0){
			pasCvtGBK(2,sname,caTemp,30);
			strcpy(sname,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,30);
			strcpy(imgpath,caTemp);
   }
   if(strlen(feetime)>0){
			pasCvtGBK(2,feetime,caTemp,30);
			strcpy(feetime,caTemp);
   }
   if(strlen(systime)>0){
			pasCvtGBK(2,systime,caTemp,30);
			strcpy(systime,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
   
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmcouponuse where id in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
				 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
			       return 0;	
				  
				  }
				  
				  // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
							
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmcouponuse where id in(%s)",caSesid);
    	}
	 }
	 // 查询
	 strcpy(sql, "select count(*) from ncmcouponuse where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%') ", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select cu.id, cu.servicecode, cu.username, cu.aid, cu.aname, cu.serno, cu.usermac, cu.sname, cu.feetime, cu.uid, cu.sid, c.fee, cu.imgpath, cu.systime, cu.flags from ncmcouponuse cu left join ncmcoupon c on cu.cid = c.cid where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (cu.username like '%%%s%%') ", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by cu.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lId,
													     UT_TYPE_STRING, sizeof(caScode)-1,     caScode,
													     UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
													     UT_TYPE_ULONG,  sizeof(long),          &lAid,
													     UT_TYPE_STRING, sizeof(caAname)-1,     caAname,
													     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
													     UT_TYPE_STRING, sizeof(caUsermac)-1,   caUsermac,
													     UT_TYPE_STRING, sizeof(caSname)-1,     caSname,
													     UT_TYPE_ULONG,  sizeof(long),          &lFeetime,
															 UT_TYPE_ULONG,  sizeof(long),          &lUid,
															 UT_TYPE_ULONG,  sizeof(long),          &lSid,
															 UT_TYPE_FLOAT,  sizeof(double),        &dFee,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
													     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
													     UT_TYPE_ULONG,  sizeof(long),          &lFlags);
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				  utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 
			 if(lFeetime>0){
			 		strcpy(s_feetime, utTimFormat("%Y/%m/%d", lFeetime));	
			 }else{
			 		strcpy(s_feetime, "");	
			 }
			 if(lSystime>0){
			 		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));	
			 }else{
			 		strcpy(s_systime, "");	
			 }
			 switch(lFlags){
			 		case 0:strcpy(s_flags, "保留");break;
			 		case 1:strcpy(s_flags, "删除");break;
			 		default:break;	
			 }
			 
			 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
			 utPltPutLoopVar(psDbHead,"scode",iNum,caScode);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVarF(psDbHead,"aid",iNum,"%lu",lAid);
			 utPltPutLoopVar(psDbHead,"aname",iNum,caAname);
			 utPltPutLoopVar(psDbHead,"sermo",iNum,caSerno);
			 utPltPutLoopVar(psDbHead,"usermac",iNum,caUsermac);
			 utPltPutLoopVar(psDbHead,"sname",iNum,caSname);
			 utPltPutLoopVar(psDbHead,"feetime",iNum,s_feetime);
			 utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVarF(psDbHead,"fee",iNum,"%.2f",dFee);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVar(psDbHead,"flags",iNum,s_flags);
			 
			 lId = 0;
			 memset(caScode, 0, sizeof(caScode));
			 memset(caUsername, 0, sizeof(caUsername));
			 lAid = 0;
			 memset(caAname, 0, sizeof(caAname));
			 memset(caSerno, 0, sizeof(caSerno));
			 memset(caUsermac, 0, sizeof(caUsermac));
			 memset(caSname, 0, sizeof(caSname));
			 lFeetime = 0;
			 lUid = 0;
			 lSid = 0;
			 dFee = 0.0;
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lSystime = 0;
			 lFlags = 0;
			 memset(s_feetime, 0, sizeof(s_feetime));
			 memset(s_systime, 0, sizeof(s_systime));
			 memset(s_flags, 0, sizeof(s_flags));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lId,
															     UT_TYPE_STRING, sizeof(caScode)-1,     caScode,
															     UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
															     UT_TYPE_ULONG,  sizeof(long),          &lAid,
															     UT_TYPE_STRING, sizeof(caAname)-1,     caAname,
															     UT_TYPE_STRING, sizeof(caSerno)-1,     caSerno,
															     UT_TYPE_STRING, sizeof(caUsermac)-1,   caUsermac,
															     UT_TYPE_STRING, sizeof(caSname)-1,     caSname,
															     UT_TYPE_ULONG,  sizeof(long),          &lFeetime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lUid,
																	 UT_TYPE_ULONG,  sizeof(long),          &lSid,
																	 UT_TYPE_FLOAT,  sizeof(double),        &dFee,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,   caImgpath,
															     UT_TYPE_ULONG,  sizeof(long),          &lSystime,
															     UT_TYPE_ULONG,  sizeof(long),          &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomcouponuse_list.htm");
	  return 0;	
}

// 商城用户关注信息
int ncm_webcomshopattent_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char id[20] = "";
	 char servicecode[16] = "";
	 char username[32] = "";
	 char uid[20] = "";
	 char activename[32] = "";
	 char indexname[32] = "";
	 char shopname[32] = "";
	 char integral[20] = "";
	 char decript[4096] = "";
	 char imgpath[128] = "";
	 char addtime[32] = "";
	 char lasttime[32] = "";
	 char flags[32] = "";
	 char temp_file[128] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lId = 0;
	 char           caServicecode[16] = "";
	 char 					caUsername[32] = "";
	 unsigned long	lUid = 0;
	 char 					caActivename[32] = "";
	 char 					caIndexname[32] = "";
	 char 					caShopname[32] = "";
	 unsigned long	lIntegral = 0;
	 char 					caDecript[4096] = "";
	 char 					caImgpath[128] = "";
	 unsigned long	lAddtime = 0;
	 unsigned long	lLasttime = 0;
	 unsigned long	lFlags = 0;
	 char           s_addtime[32] = "";
	 char           s_lasttime[32] = "";
	 char           s_flags[32] = "";
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 24,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"id",          UT_TYPE_STRING,  sizeof(id)-1,             id,
																	"servicecode", UT_TYPE_STRING,  sizeof(servicecode)-1,    servicecode,
																	"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
																	"uid",         UT_TYPE_STRING,  sizeof(uid)-1,            uid,
																	"activename",  UT_TYPE_STRING,  sizeof(activename)-1,     activename,
																	"indexname",   UT_TYPE_STRING,  sizeof(indexname)-1,      indexname,
																	"shopname",    UT_TYPE_STRING,  sizeof(shopname)-1,       shopname,
																	"integral",    UT_TYPE_STRING,  sizeof(integral)-1,       integral,
																	"decript",     UT_TYPE_STRING,  sizeof(decript)-1,        decript,
																	"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
																	"addtime",     UT_TYPE_STRING,  sizeof(addtime)-1,        addtime,
																	"lasttime",    UT_TYPE_STRING,  sizeof(lasttime)-1,       lasttime,
																	"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"update",      UT_TYPE_STRING,  10,                       caUpdate,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport);
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(servicecode)>0){
			pasCvtGBK(2,servicecode,caTemp,30);
			strcpy(servicecode,caTemp);
   }
   if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,30);
			strcpy(username,caTemp);
   }
   if(strlen(activename)>0){
			pasCvtGBK(2,activename,caTemp,30);
			strcpy(activename,caTemp);
   }
   if(strlen(indexname)>0){
			pasCvtGBK(2,indexname,caTemp,30);
			strcpy(indexname,caTemp);
   }
   if(strlen(shopname)>0){
			pasCvtGBK(2,shopname,caTemp,30);
			strcpy(shopname,caTemp);
   }
   if(strlen(decript)>0){
			pasCvtGBK(2,decript,caTemp,30);
			strcpy(decript,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,30);
			strcpy(imgpath,caTemp);
   }
   if(strlen(addtime)>0){
			pasCvtGBK(2,addtime,caTemp,30);
			strcpy(addtime,caTemp);
   }
   if(strlen(lasttime)>0){
			pasCvtGBK(2,lasttime,caTemp,30);
			strcpy(lasttime,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
   
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char 	imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmshopattent where id in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
				 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
			       return 0;	
				  }
				  
				  // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmshopattent where id in(%s)",caSesid);
    	}
	 }
	 
	 unsigned long lShopId = 0;
	 dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	 
	 // 查询
	 strcpy(sql, "select count(*) from ncmshopattent where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%') ", _keyword);
	 }
	 if(lShopId>0) // 根据商店查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and shopid = %lu", lShopId);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select sa.id, sa.servicecode, sa.username, sa.uid, a.aname, i.name, s.name, sa.integral, sa.decript, sa.imgpath, sa.addtime, sa.lasttime, sa.flags from ncmshopattent sa left join ncmactive a on sa.aid = a.id left join ncmcomindex i on sa.sid = i.sid left join ncmshop s on sa.shopid = s.id where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (sa.username like '%%%s%%') ", _keyword);
	 }
	 if(lShopId>0) // 根据商店查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sa.shopid = %lu", lShopId);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by sa.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
													     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
													     UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername,
													     UT_TYPE_ULONG,  sizeof(long),           &lUid,
													     UT_TYPE_STRING, sizeof(caActivename)-1, caActivename,
													     UT_TYPE_STRING, sizeof(caIndexname)-1,  caIndexname,
													     UT_TYPE_STRING, sizeof(caShopname)-1,   caShopname,
													     UT_TYPE_ULONG,  sizeof(long),           &lIntegral,
													     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
													     UT_TYPE_ULONG,  sizeof(long),           &lAddtime,
													     UT_TYPE_ULONG,  sizeof(long),           &lLasttime,
															 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
		   
		   if(lAddtime>0){
		   		strcpy(s_addtime, utTimFormat("%Y/%m/%d %H:%M:%S", lAddtime));	
		   }else{
		   		strcpy(s_addtime, "");	
		   }
		   if(lLasttime>0){
		   		strcpy(s_lasttime, utTimFormat("%Y/%m/%d %H:%M:%S", lLasttime));	
		   }else{
		   		strcpy(s_lasttime, "");	
		   }
		   switch(lFlags){
		   		case 0:strcpy(s_flags, "保留");break;
		   		case 1:strcpy(s_flags, "删除");break;
		   		default:break;
		   }
		   
			 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
			 utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
			 utPltPutLoopVar(psDbHead,"activename",iNum,caActivename);
			 utPltPutLoopVar(psDbHead,"indexname",iNum,caIndexname);
			 utPltPutLoopVar(psDbHead,"shopname",iNum,caShopname);
			 utPltPutLoopVarF(psDbHead,"integral",iNum,"%lu",lIntegral);
			 utPltPutLoopVar(psDbHead,"decript",iNum,caDecript);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"addtime",iNum,s_addtime);
			 utPltPutLoopVar(psDbHead,"lasttime",iNum,s_lasttime);
			 utPltPutLoopVar(psDbHead,"flags",iNum,s_flags);
			 
			 lId = 0;
			 memset(caServicecode, 0, sizeof(caServicecode));
			 memset(caUsername, 0, sizeof(caUsername));
			 lUid = 0;
			 memset(caActivename, 0, sizeof(caActivename));
			 memset(caIndexname, 0, sizeof(caIndexname));
			 memset(caShopname, 0, sizeof(caShopname));
			 unsigned long	lIntegral = 0;
			 memset(caDecript, 0, sizeof(caDecript));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lAddtime = 0;
			 lLasttime = 0;
			 lFlags = 0;
			 memset(s_addtime, 0, sizeof(s_addtime));
			 memset(s_lasttime, 0, sizeof(s_lasttime));
			 memset(s_flags, 0, sizeof(s_flags));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
															     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
															     UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername,
															     UT_TYPE_ULONG,  sizeof(long),           &lUid,
															     UT_TYPE_STRING, sizeof(caActivename)-1, caActivename,
															     UT_TYPE_STRING, sizeof(caIndexname)-1,  caIndexname,
															     UT_TYPE_STRING, sizeof(caShopname)-1,   caShopname,
															     UT_TYPE_ULONG,  sizeof(long),           &lIntegral,
															     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
															     UT_TYPE_ULONG,  sizeof(long),           &lAddtime,
															     UT_TYPE_ULONG,  sizeof(long),           &lLasttime,
																	 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomshopattent_list.htm");
	  return 0;	
}

// 商城用户收藏信息
int ncm_webcomshopcollect_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char id[20] = "";
	 char servicecode[16] = "";
	 char username[32] = "";
	 char nickname[32] = "";
	 char membernum[32] = "";
	 char uid[20] = "";
	 char activename[32] = "";
	 char odityname[32] = "";
	 char imgpath[128] = "";
	 char decript[4096] = "";
	 char addtime[32] = "";
	 char lasttime[32] = "";
	 char flags[32] = "";
	 char temp_file[128] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lId = 0;
	 char           caServicecode[16] = "";
	 char 					caUsername[32] = "";
	 char           caNickname[32] = "";
	 char           caMembernum[32] = "";
	 unsigned long	lUid = 0;
	 char           caActivename[32] = "";
	 char           caOdityname[32] = "";
	 char 					caDecript[4096] = "";
	 char 					caImgpath[128] = "";
	 unsigned long	lAddtime = 0;
	 unsigned long	lLasttime = 0;
	 unsigned long	lFlags = 0;
	 char           s_addtime[32] = "";
	 char           s_lasttime[32] = "";
	 char           s_flags[32] = "";
	 
	 char insert[16] = "";
	 char caInsert[16] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 23,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"id",          UT_TYPE_STRING,  sizeof(id)-1,             id,
			"servicecode", UT_TYPE_STRING,  sizeof(servicecode)-1,    servicecode,
			"username",    UT_TYPE_STRING,  sizeof(username)-1,       username,
			"nickname",    UT_TYPE_STRING,  sizeof(nickname)-1,       nickname,
			"membernum",   UT_TYPE_STRING,  sizeof(membernum)-1,      membernum,
			"uid",         UT_TYPE_STRING,  sizeof(uid)-1,            uid,
			"activename",  UT_TYPE_STRING,  sizeof(activename)-1,     activename,
			"odityname",   UT_TYPE_STRING,  sizeof(odityname)-1,      odityname,
			"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
			"decript",     UT_TYPE_STRING,  sizeof(decript)-1,        decript,
			"addtime",     UT_TYPE_STRING,  sizeof(addtime)-1,        addtime,
			"lasttime",    UT_TYPE_STRING,  sizeof(lasttime)-1,       lasttime,
			"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"insert",      UT_TYPE_STRING,  10,                       caInsert,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(servicecode)>0){
			pasCvtGBK(2,servicecode,caTemp,30);
			strcpy(servicecode,caTemp);
   }
   if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,30);
			strcpy(username,caTemp);
   }
   if(strlen(nickname)>0){
			pasCvtGBK(2,nickname,caTemp,30);
			strcpy(nickname,caTemp);
   }
   if(strlen(membernum)>0){
			pasCvtGBK(2,membernum,caTemp,30);
			strcpy(membernum,caTemp);
   }
   if(strlen(activename)>0){
			pasCvtGBK(2,activename,caTemp,30);
			strcpy(activename,caTemp);
   }
   if(strlen(odityname)>0){
			pasCvtGBK(2,odityname,caTemp,30);
			strcpy(odityname,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,30);
			strcpy(imgpath,caTemp);
   }
   if(strlen(decript)>0){
			pasCvtGBK(2,decript,caTemp,30);
			strcpy(decript,caTemp);
   }
   if(strlen(addtime)>0){
			pasCvtGBK(2,addtime,caTemp,30);
			strcpy(addtime,caTemp);
   }
   if(strlen(lasttime)>0){
			pasCvtGBK(2,lasttime,caTemp,30);
			strcpy(lasttime,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
   
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char 	imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmshopcollect where id in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
				       return 0;	
					}
					
					// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
					iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
					
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmshopcollect where id in(%s)",caSesid);
    	}
	 }
	 // 查询
	 strcpy(sql, "select count(*) from ncmshopcollect where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (username like '%%%s%%') ", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select sc.id, sc.servicecode, sc.username, sc.nickname, sc.membernum, sc.uid, a.aname, o.name, sc.imgpath, sc.decript, sc.addtime, sc.lasttime, sc.flags from ncmshopcollect sc left join ncmactive a on sc.aid = a.id left join ncmcomodity o on sc.sid = o.sid where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (sc.username like '%%%s%%') ", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by sc.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
													     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
													     UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername,
													     UT_TYPE_STRING, sizeof(caNickname)-1,   caNickname,
													     UT_TYPE_STRING, sizeof(caMembernum)-1,  caMembernum,
													     UT_TYPE_ULONG,  sizeof(long),           &lUid,
													     UT_TYPE_STRING, sizeof(caActivename)-1, caActivename,
													     UT_TYPE_STRING, sizeof(caOdityname)-1,  caOdityname,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
													     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
													     UT_TYPE_ULONG,  sizeof(long),           &lAddtime,
													     UT_TYPE_ULONG,  sizeof(long),           &lLasttime,
															 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
		   
		   if(lAddtime>0){
		   		strcpy(s_addtime, utTimFormat("%Y/%m/%d %H:%M:%S", lAddtime));
		   }else{
		   		strcpy(s_addtime, "");	
		   }
		   if(lLasttime>0){
		   		strcpy(s_lasttime, utTimFormat("%Y/%m/%d %H:%M:%S", lLasttime));	
		   }else{
		   		strcpy(s_lasttime, "");	
		   }
		   switch(lFlags){
		   		case 0:strcpy(s_flags, "保留");break;
		   		case 1:strcpy(s_flags, "删除");break;
		   		default:break;	
		   }
		   
			 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
			 utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVar(psDbHead,"nickname",iNum,caNickname);
			 utPltPutLoopVar(psDbHead,"membernum",iNum,caMembernum);
			 utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
			 utPltPutLoopVar(psDbHead,"activename",iNum,caActivename);
			 utPltPutLoopVar(psDbHead,"odityname",iNum,caOdityname);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"decript",iNum,caDecript);
			 utPltPutLoopVar(psDbHead,"addtime",iNum,s_addtime);
			 utPltPutLoopVar(psDbHead,"lasttime",iNum,s_lasttime);
			 utPltPutLoopVar(psDbHead,"flags",iNum,s_flags);
			 
			 lId = 0;
			 memset(caServicecode, 0, sizeof(caServicecode));
			 memset(caUsername, 0, sizeof(caUsername));
			 memset(caNickname, 0, sizeof(caNickname));
			 memset(caMembernum, 0, sizeof(caMembernum));
			 lUid = 0;
			 memset(caActivename, 0, sizeof(caActivename));
			 memset(caOdityname, 0, sizeof(caOdityname));
			 memset(caDecript, 0, sizeof(caDecript));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lAddtime = 0;
			 lLasttime = 0;
			 lFlags = 0;
			 memset(s_addtime, 0, sizeof(s_addtime));
			 memset(s_lasttime, 0, sizeof(s_lasttime));
			 memset(s_flags, 0, sizeof(s_flags));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
															     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
															     UT_TYPE_STRING, sizeof(caUsername)-1,   caUsername,
															     UT_TYPE_STRING, sizeof(caNickname)-1,   caNickname,
															     UT_TYPE_STRING, sizeof(caMembernum)-1,  caMembernum,
															     UT_TYPE_ULONG,  sizeof(long),           &lUid,
															     UT_TYPE_STRING, sizeof(caActivename)-1, caActivename,
															     UT_TYPE_STRING, sizeof(caOdityname)-1,  caOdityname,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
															     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
															     UT_TYPE_ULONG,  sizeof(long),           &lAddtime,
															     UT_TYPE_ULONG,  sizeof(long),           &lLasttime,
																	 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomshopcollect_list.htm");
	  return 0;	
}

// 商城一级活动表
int ncm_webcomactive_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char id[20] = "";
	 char servicecode[16] = "";
	 char aname[64] = "";
	 char indexname[32] = "";
	 char odityname[32] = "";
	 char couponfee[20] = "";
	 char discount[20] = "";
	 char address[255] = "";
	 char decript[4096] = "";
	 char imgpath[128] = "";
	 char plevel[20] = "";
	 char type[20] = "";
	 char xid[20] = "";
	 char stime[32] = "";
	 char etime[32] = "";
	 char flags[32] = "";
	 char temp_file[128] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lId = 0;
	 char 					caServicecode[16] = "";
	 char 					caAname[64] = "";
	 char 			    caIndexname[32] = "";
	 char 			    caOdityname[32] = "";
	 double         dCouponfee = 0.0;
	 double	        dDiscount = 0.0;
	 char           caAddress[255] = "";
	 char 					caDecript[4096] = "";
	 char 					caImgpath[128] = "";
	 unsigned long	lPlevel = 0;
	 unsigned long	lType = 0;
	 unsigned long	lXid = 0;
	 unsigned long	lStime = 0;
	 unsigned long	lEtime = 0;
	 unsigned long	lFlags = 0;
	 char           s_stime[32] = "";
	 char           s_etime[32] = "";
	 
	 char insert[16] = "";
	 char caInsert[16] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 26,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"id",          UT_TYPE_STRING,  sizeof(id)-1,             id,
			"servicecode", UT_TYPE_STRING,  sizeof(servicecode)-1,    servicecode,
			"aname",       UT_TYPE_STRING,  sizeof(aname)-1,          aname,
			"indexname",   UT_TYPE_STRING,  sizeof(indexname)-1,      indexname,
			"odityname",   UT_TYPE_STRING,  sizeof(odityname)-1,      odityname,
			"couponfee",   UT_TYPE_STRING,  sizeof(couponfee)-1,      couponfee,
			"discount",    UT_TYPE_STRING,  sizeof(discount)-1,       discount,
			"address",     UT_TYPE_STRING,  sizeof(address)-1,        address,
			"decript",     UT_TYPE_STRING,  sizeof(decript)-1,        decript,
			"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
			"plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
			"type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
			"xid",         UT_TYPE_STRING,  sizeof(xid)-1,            xid,
			"stime",       UT_TYPE_STRING,  sizeof(stime)-1,          stime,
			"etime",       UT_TYPE_STRING,  sizeof(etime)-1,          etime,
			"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"insert",      UT_TYPE_STRING,  10,                       caInsert,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(aname)>0){
			pasCvtGBK(2,aname,caTemp,30);
			strcpy(aname,caTemp);
   }
   if(strlen(indexname)>0){
			pasCvtGBK(2,indexname,caTemp,30);
			strcpy(indexname,caTemp);
   }
   if(strlen(odityname)>0){
			pasCvtGBK(2,odityname,caTemp,30);
			strcpy(odityname,caTemp);
   }
   if(strlen(address)>0){
			pasCvtGBK(2,address,caTemp,30);
			strcpy(address,caTemp);
   }
	 if(strlen(decript)>0){
			pasCvtGBK(2,decript,caTemp,30);
			strcpy(decript,caTemp);
   }
   if(strlen(stime)>0){
			pasCvtGBK(2,stime,caTemp,30);
			strcpy(stime,caTemp);
   }
   if(strlen(etime)>0){
			pasCvtGBK(2,etime,caTemp,30);
			strcpy(etime,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,30);
			strcpy(imgpath,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
   
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmactive where id in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
				 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
			       return 0;	
				  }
				  
				  // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
				  iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
    		  
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmactive where id in(%s)",caSesid);
    	}
	 }
	 // 查询
	 strcpy(sql, "select count(*) from ncmactive where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (aname like '%%%s%%') ", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select a.id, a.servicecode, a.aname, i.name, o.name, c.fee, a.discount, a.address, a.decript, a.imgpath, a.plevel, a.type, a.xid, a.stime, a.etime, a.flags from ncmactive a left join ncmcomindex i on a.shopid = i.sid left join ncmcomodity o on a.sid = o.sid left join ncmcoupon c on a.cid = c.cid where 1=1 and a.type=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (a.aname like '%%%s%%') ", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by a.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
													     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
													     UT_TYPE_STRING, sizeof(caAname)-1,      caAname,
													     UT_TYPE_STRING, sizeof(caIndexname)-1,  caIndexname,
													     UT_TYPE_STRING, sizeof(caOdityname)-1,  caOdityname,
													     UT_TYPE_FLOAT,  sizeof(double),         &dCouponfee,
													     UT_TYPE_FLOAT,  sizeof(double),         &dDiscount,
													     UT_TYPE_STRING, sizeof(caAddress)-1,    caAddress,
													     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
													     UT_TYPE_ULONG,  sizeof(long),           &lPlevel,
													     UT_TYPE_ULONG,  sizeof(long),           &lType,
													     UT_TYPE_ULONG,  sizeof(long),           &lXid,
													     UT_TYPE_ULONG,  sizeof(long),           &lStime,
															 UT_TYPE_ULONG,  sizeof(long),           &lEtime,
															 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
		   
		   if(lStime>0){
		   		strcpy(s_stime, utTimFormat("%Y/%m/%d", lStime));	
		   }else{
		   		strcpy(s_stime, "");	
		   }
		   if(lEtime>0){
		   		strcpy(s_etime, utTimFormat("%Y/%m/%d", lEtime));	
		   }else{
		   		strcpy(s_etime, "");	
		   }
		  
			 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
			 utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
			 utPltPutLoopVar(psDbHead,"aname",iNum,caAname);
			 utPltPutLoopVar(psDbHead,"indexname",iNum,caIndexname);
			 utPltPutLoopVar(psDbHead,"odityname",iNum,caOdityname);
			 utPltPutLoopVarF(psDbHead,"couponfee",iNum,"%.2f",dCouponfee);
			 utPltPutLoopVarF(psDbHead,"discount",iNum,"%.2f",dDiscount);
			 utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
			 utPltPutLoopVar(psDbHead,"decript",iNum,caDecript);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",lPlevel);
			 utPltPutLoopVarF(psDbHead,"type",iNum,"%lu",lType);
			 utPltPutLoopVarF(psDbHead,"xid",iNum,"%lu",lXid);
			 utPltPutLoopVar(psDbHead,"stime",iNum,s_stime);
			 utPltPutLoopVar(psDbHead,"etime",iNum,s_etime);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 
			 lId = 0;
			 memset(caServicecode, 0, sizeof(caServicecode));
			 memset(caAname, 0, sizeof(caAname));
			 memset(caIndexname, 0, sizeof(caIndexname));
			 memset(caOdityname, 0, sizeof(caOdityname));
			 dCouponfee = 0.0;
			 dDiscount = 0.0;
			 memset(caAddress, 0, sizeof(caAddress));
			 memset(caDecript, 0, sizeof(caDecript));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lPlevel = 0;
			 lType = 0;
			 lXid = 0;
			 lStime = 0;
			 lEtime = 0;
			 lFlags = 0;
			 memset(s_stime, 0, sizeof(s_stime));
			 memset(s_etime, 0, sizeof(s_etime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
															     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
															     UT_TYPE_STRING, sizeof(caAname)-1,      caAname,
															     UT_TYPE_STRING, sizeof(caIndexname)-1,  caIndexname,
															     UT_TYPE_STRING, sizeof(caOdityname)-1,  caOdityname,
															     UT_TYPE_FLOAT,  sizeof(double),         &dCouponfee,
															     UT_TYPE_FLOAT,  sizeof(double),         &dDiscount,
															     UT_TYPE_STRING, sizeof(caAddress)-1,    caAddress,
															     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
															     UT_TYPE_ULONG,  sizeof(long),           &lPlevel,
															     UT_TYPE_ULONG,  sizeof(long),           &lType,
															     UT_TYPE_ULONG,  sizeof(long),           &lXid,
															     UT_TYPE_ULONG,  sizeof(long),           &lStime,
																	 UT_TYPE_ULONG,  sizeof(long),           &lEtime,
																	 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomactive_list.htm");
	  return 0;	
}

int ncm_webcomactive_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
//#endif  
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iFlag=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char caKey[32] = "";
	 char caSid[32] = "";
	 char caServicecode[16] = "";
	 char caAname[32] = "";
	 unsigned long lShopid = 0;
	 unsigned long lSid = 0;
	 unsigned long lCid = 0;
	 double fDiscount = 0.0;
	 char caAddress[255] = "";
	 char caDecript[4096] = "";
	 char caImgpath[128] = "";
	 unsigned long lPlevel = 0;
	 unsigned long lType = 0;
	 unsigned long lXid = 0;
	 unsigned long lStime = 0;
	 unsigned long lEtime = 0;
	 
	 char s_stime[32] = "";
	 char s_etime[32] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 2,"key", UT_TYPE_STRING, sizeof(caKey)-1, caKey,
	 															 "sid", UT_TYPE_STRING, sizeof(caSid)-1, caSid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
	 	   iFlag = 1;
			 sprintf(sql, "select servicecode, aname, shopid, sid, cid, discount, address, decript, imgpath, plevel, type, xid, stime, etime from ncmactive where id=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,15,caServicecode,
			                      UT_TYPE_STRING,31,caAname,
			                      UT_TYPE_ULONG,4,&lShopid,
			                      UT_TYPE_ULONG,4,&lSid,
			                      UT_TYPE_ULONG,4,&lCid,
			                      UT_TYPE_FLOAT,sizeof(double),&fDiscount,
			                      UT_TYPE_STRING,31,caAddress,
		                        UT_TYPE_STRING,254,caDecript,
		                        UT_TYPE_STRING,127,caImgpath,
		                        UT_TYPE_ULONG,4,&lPlevel,
		                        UT_TYPE_ULONG,4,&lType,
		                        UT_TYPE_ULONG,4,&lXid,
		                        UT_TYPE_ULONG,4,&lStime,
		                        UT_TYPE_ULONG,4,&lEtime);
		    
		    if(lStime>0){
		    		strcpy(s_stime, utTimFormat("%Y/%m/%d", lStime));	
		    }else{
		    		strcpy(s_stime, "");	
		    }
			  if(lEtime>0){
			  		strcpy(s_etime, utTimFormat("%Y/%m/%d", lEtime));	
			  }else{
			  		strcpy(s_etime, "");	
			  }
			  
			  utPltPutVar(psDbHead,"id",caSid);
			  utPltPutVar(psDbHead,"scode",caServicecode);
				utPltPutVar(psDbHead,"aname",caAname);
				utPltPutVarF(psDbHead,"indexid","%lu",lShopid);
				utPltPutVarF(psDbHead,"odityid","%lu",lSid);
				utPltPutVarF(psDbHead,"couponid","%lu",lCid);
				utPltPutVarF(psDbHead,"discount","%.2f",fDiscount);
				utPltPutVar(psDbHead,"address",caAddress);
				utPltPutVar(psDbHead,"decript",caDecript);
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVarF(psDbHead,"plevel","%lu",lPlevel);
		    utPltPutVarF(psDbHead,"type","%lu",lType);
		    utPltPutVarF(psDbHead,"xid","%lu",lXid);
		    utPltPutVar(psDbHead,"stime",s_stime);
		    utPltPutVar(psDbHead,"etime",s_etime);
		    
		    utPltPutVarF(psDbHead,"flag","%d",iFlag);
	 }else{
	 	    utPltPutVarF(psDbHead,"flag","%d",iFlag);
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomactive_dr.htm");
	 return 0;		
}

int ncm_webcomactive_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  
	  char caPos[32]="";
	  char caImgPath[128]="";
	  char caDispImgPath[128]="";
	  char imgname[128]="";
	  char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
 		char caTemp[1024]="";
	  char *p=NULL;
    
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0194导入"),ncLang("0183打开文件出错"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// 建立图片存放路径
  	sprintf(caImgPath,"%s/images/ShopImages/activeimages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);

	  utMsgGetSomeNVar(psMsgHead,2,"fileField", UT_TYPE_STRING, 255, imp_file,
	  														 "position",  UT_TYPE_STRING, sizeof(caPos)-1, caPos);
	  printf("fileField = %s\n", imp_file);
	  printf("position = %s\n", caPos);
	  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*从指定的字符串中取一个词
		  	
		  	  函数原型：
			　　char *utStrGetWord(char *p,char *pOut,
			　　                   short nMaxLen,char *pTerm);
			　　
					参数说明:
			　　p – 输入字符串
			　　pOut --- 输出字符串，必须要事先分配好空间
			　　nMaxLen --- 输出字符传的最大长度
			　　pTerm  ----- 终止条件的字符串，系统在扫描描时遇到该字符串某一个字符时结束。
			　　
			　　返回值：
			　　结束字符的指针。*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
		  	printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
				}else{
						sprintf(imgname, "active_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/activeimages/");
				    strcat(caDispImgPath, imgname);	
				}
		}
		
		utPltPutVar(psDbHead,"position",caPos);
	  utPltPutVar(psDbHead,"filepath",caDispImgPath);
	  printf("position = %s\n", caPos);
	  printf("imgpath = %s\n", caDispImgPath);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomactive_closeFile.htm");
    return 0;
}

// 上传商城一级活动图片
int ncm_webcomactive_update_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32]="";
   	char caId[20]="";
   	char caScode[16]="";
    char caAname[32]="";
    char caIndexid[32]="";
    char caOdityid[32]="";
    char caCouponid[32]="";
    char caDiscount[20]="";
    char caAddress[255]="";
    char caDecript[4096]="";
    char caImgPath[128]="";
    char caPlevel[20] = "";
    char caType[20] = "";
    char caXid[20] = "";
    char caStime[32]="";
    char caEtime[32]="";
    
    unsigned long lCount = 0;
    char caImgpath1[128]="";
    char temp_file[129]="";
 		char caTemp[1024]="";
 		char caTemp1[1024]="";
 		char caTemp2[1024]="";
 		char caTemp3[1024]="";
 		
 		unsigned long lShopid=0;
 		unsigned long lSid=0;
 		unsigned long lCid=0;
 		unsigned long lStime=0;
 		unsigned long lEtime=0;
 		unsigned long lMaxid = 0;
 		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
    
    psDbHead = utPltInitDb();
    
    // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	  utMsgGetSomeNVar(psMsgHead, 16,"key",      UT_TYPE_STRING,  sizeof(caKey)-1,            caKey,
	                               "id",         UT_TYPE_STRING,  sizeof(caId)-1,             caId,
	                               "scode",      UT_TYPE_STRING,  sizeof(caScode)-1,          caScode,
	  														 "aname",      UT_TYPE_STRING,  sizeof(caAname)-1,          caAname,
	  														 "indexid",    UT_TYPE_STRING,  sizeof(caIndexid)-1,        caIndexid,
	  														 "odityid",    UT_TYPE_STRING,  sizeof(caOdityid)-1,        caOdityid,
	  														 "couponid",   UT_TYPE_STRING,  sizeof(caCouponid)-1,       caCouponid,
	  														 "discount",   UT_TYPE_STRING,  sizeof(caDiscount)-1,       caDiscount,
	  														 "address",    UT_TYPE_STRING,  sizeof(caAddress)-1,        caAddress,
	  														 "decript",    UT_TYPE_STRING,  sizeof(caDecript)-1,        caDecript,
	  														 "filepath",   UT_TYPE_STRING,  sizeof(caImgPath)-1,        caImgPath,
	  														 "plevel",     UT_TYPE_STRING,  sizeof(caPlevel)-1,         caPlevel,
	  														 "type",       UT_TYPE_STRING,  sizeof(caType)-1,           caType,
	  														 "xid",        UT_TYPE_STRING,  sizeof(caXid)-1,            caXid,
																 "stime",      UT_TYPE_STRING,  sizeof(caStime)-1,          caStime,
																 "etime",      UT_TYPE_STRING,  sizeof(caEtime)-1,          caEtime);
	  
	  if(strlen(caAname)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","活动名称不能为空!","活动名称不能为空!");
        return 0;	
	  }
	  
	  strcpy(caType, "1");
	  
	  sprintf(caTemp3, "select servicecode from ncmcomodity where sid = %lu", atol(caOdityid));
	  pasDbOneRecord(caTemp3,0,UT_TYPE_STRING,sizeof(caScode)-1,caScode);
	  ncTimeToLong(caStime, &lStime);
	  ncTimeToLong(caEtime, &lEtime);
	  
	  sprintf(caTemp1, "select count(*) from ncmactive where plevel=%d", atol(caPlevel));
	  pasDbOneRecord(caTemp1,0,UT_TYPE_LONG,4,&lCount);                
	  if(strcmp(caKey, "add")==0){ //增加
	  	  if(lCount){
	  	  	  // 删除原位置的图片
	  	  	  sprintf(caTemp2, "select imgpath from ncmactive where plevel=%lu", atol(caPlevel));
	  	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  	  sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
				  	system(temp_file);
				  	// 将原位置的活动信息替换
					 	sprintf(caTemp,"update ncmactive set servicecode='%s',aname='%s',shopid=%lu,sid=%lu,cid=%lu,discount=%.2f,address='%s',decript='%s',imgpath='%s',type=%lu,xid=%lu,stime=%lu,etime=%lu where plevel=%lu",caScode,caAname,atol(caIndexid),atol(caOdityid),atol(caCouponid),atof(caDiscount),caAddress,caDecript,caImgPath,atol(caType),atol(caXid),lStime,lEtime,atol(caPlevel));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }else{
	  	  	  // 增加新的活动信息
	  	  	  pasDbOneRecord("select max(id) from ncmactive", 0, UT_TYPE_ULONG, 4, &lMaxid);
				    sprintf(caTemp,"insert into ncmactive (id,servicecode,aname,shopid,sid,cid,discount,address,decript,imgpath,plevel,type,xid,stime,etime) values (%lu,'%s','%s',%lu,%lu,%lu,%.2f,'%s','%s','%s',%lu,%lu,%lu,%lu,%lu)",(lMaxid+1),caScode,caAname,atol(caIndexid),atol(caOdityid),atol(caCouponid),atof(caDiscount),caAddress,caDecript,caImgPath,atol(caPlevel),atol(caType),atol(caXid),lStime,lEtime);
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }
	  }else if(strcmp(caKey, "update")==0){ // 修改
	  	  if(lCount){
	  	  	  // 删除原位置的图片
	  	  	  sprintf(caTemp2, "select imgpath from ncmactive where plevel=%lu", atol(caPlevel));
	  	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  	  if(strcmp(caImgPath, caImgpath1) != 0){
			  	  	  sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
						  	system(temp_file);
				    }
				  	
					 	sprintf(caTemp,"update ncmactive set servicecode='%s',aname='%s',shopid=%lu,sid=%lu,cid=%lu,discount=%.2f,address='%s',decript='%s',imgpath='%s',type=%lu,xid=%lu,stime=%lu,etime=%lu where plevel=%lu",caScode,caAname,atol(caIndexid),atol(caOdityid),atol(caCouponid),atof(caDiscount),caAddress,caDecript,caImgPath,atol(caType),atol(caXid),lStime,lEtime,atol(caPlevel));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }else{
					 	sprintf(caTemp,"update ncmactive set servicecode='%s',aname='%s',shopid=%lu,sid=%lu,cid=%lu,discount=%.2f,address='%s',decript='%s',imgpath='%s',plevel=%lu,type=%lu,xid=%lu,stime=%lu,etime=%lu where id=%lu",caScode,caAname,atol(caIndexid),atol(caOdityid),atol(caCouponid),atof(caDiscount),caAddress,caDecript,caImgPath,atol(caPlevel),atol(caType),atol(caXid),lStime,lEtime,atol(caId));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
	  	  }
	  }
	  
	  utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","数据加载成功!","数据加载成功!");
    return 0;
}

// 商城二级活动表
int ncm_webcomseries_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char id[20] = "";
	 char servicecode[16] = "";
	 char aname[64] = "";
	 char indexname[32] = "";
	 char odityname[32] = "";
	 char couponfee[20] = "";
	 char discount[20] = "";
	 char address[255] = "";
	 char decript[4096] = "";
	 char imgpath[128] = "";
	 char plevel[20] = "";
	 char type[20] = "";
	 char xid[20] = "";
	 char stime[32] = "";
	 char etime[32] = "";
	 char flags[32] = "";
	 char temp_file[128] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lId = 0;
	 char 					caServicecode[16] = "";
	 char 					caAname[64] = "";
	 char 			    caIndexname[32] = "";
	 char 			    caOdityname[32] = "";
	 double         dCouponfee = 0.0;
	 double	        dDiscount = 0.0;
	 char           caAddress[255] = "";
	 char 					caDecript[4096] = "";
	 char 					caImgpath[128] = "";
	 unsigned long	lPlevel = 0;
	 unsigned long	lType = 0;
	 unsigned long	lXid = 0;
	 unsigned long	lStime = 0;
	 unsigned long	lEtime = 0;
	 unsigned long	lFlags = 0;
	 char           s_stime[32] = "";
	 char           s_etime[32] = "";
	 
	 char insert[16] = "";
	 char caInsert[16] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
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
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 26,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
			"id",          UT_TYPE_STRING,  sizeof(id)-1,             id,
			"servicecode", UT_TYPE_STRING,  sizeof(servicecode)-1,    servicecode,
			"aname",       UT_TYPE_STRING,  sizeof(aname)-1,          aname,
			"indexname",   UT_TYPE_STRING,  sizeof(indexname)-1,      indexname,
			"odityname",   UT_TYPE_STRING,  sizeof(odityname)-1,      odityname,
			"couponfee",   UT_TYPE_STRING,  sizeof(couponfee)-1,      couponfee,
			"discount",    UT_TYPE_STRING,  sizeof(discount)-1,       discount,
			"address",     UT_TYPE_STRING,  sizeof(address)-1,        address,
			"decript",     UT_TYPE_STRING,  sizeof(decript)-1,        decript,
			"imgpath",     UT_TYPE_STRING,  sizeof(imgpath)-1,        imgpath,
			"plevel",      UT_TYPE_STRING,  sizeof(plevel)-1,         plevel,
			"type",        UT_TYPE_STRING,  sizeof(type)-1,           type,
			"xid",         UT_TYPE_STRING,  sizeof(xid)-1,            xid,
			"stime",       UT_TYPE_STRING,  sizeof(stime)-1,          stime,
			"etime",       UT_TYPE_STRING,  sizeof(etime)-1,          etime,
			"flags",    	 UT_TYPE_STRING,  sizeof(flags)-1,          flags,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"insert",      UT_TYPE_STRING,  10,                       caInsert,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(aname)>0){
			pasCvtGBK(2,aname,caTemp,30);
			strcpy(aname,caTemp);
   }
   if(strlen(indexname)>0){
			pasCvtGBK(2,indexname,caTemp,30);
			strcpy(indexname,caTemp);
   }
   if(strlen(odityname)>0){
			pasCvtGBK(2,odityname,caTemp,30);
			strcpy(odityname,caTemp);
   }
   if(strlen(address)>0){
			pasCvtGBK(2,address,caTemp,30);
			strcpy(address,caTemp);
   }
	 if(strlen(decript)>0){
			pasCvtGBK(2,decript,caTemp,30);
			strcpy(decript,caTemp);
   }
   if(strlen(stime)>0){
			pasCvtGBK(2,stime,caTemp,30);
			strcpy(stime,caTemp);
   }
   if(strlen(etime)>0){
			pasCvtGBK(2,etime,caTemp,30);
			strcpy(etime,caTemp);
   }
   if(strlen(imgpath)>0){
			pasCvtGBK(2,imgpath,caTemp,30);
			strcpy(imgpath,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
   
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
	 		memset(caTemp, 0, sizeof(caTemp));
	 		char imgpath[128] = "";
    	if(strlen(caSesid)>0){
    			sprintf(caTemp, "select imgpath from ncmactive where id in(%s)", caSesid);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
				 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
			       return 0;	
				  }
    		  
    		  // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
    		  iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
    		  
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH,imgpath);
		    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
				  }
					pasDbExecSqlF("delete from ncmactive where id in(%s)",caSesid);
    	}
	 }
	 // 查询
	 strcpy(sql, "select count(*) from ncmactive where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (aname like '%%%s%%') ", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select a.id, a.servicecode, a.aname, i.name, o.name, c.fee, a.discount, a.address, a.decript, a.imgpath, a.plevel, a.type, a.xid, a.stime, a.etime, a.flags from ncmactive a left join ncmcomindex i on a.shopid = i.sid left join ncmcomodity o on a.sid = o.sid left join ncmcoupon c on a.cid = c.cid where 1=1 and a.type=2");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (a.aname like '%%%s%%') ", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by a.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
													     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
													     UT_TYPE_STRING, sizeof(caAname)-1,      caAname,
													     UT_TYPE_STRING, sizeof(caIndexname)-1,  caIndexname,
													     UT_TYPE_STRING, sizeof(caOdityname)-1,  caOdityname,
													     UT_TYPE_FLOAT,  sizeof(double),         &dCouponfee,
													     UT_TYPE_FLOAT,  sizeof(double),         &dDiscount,
													     UT_TYPE_STRING, sizeof(caAddress)-1,    caAddress,
													     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
													     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
													     UT_TYPE_ULONG,  sizeof(long),           &lPlevel,
													     UT_TYPE_ULONG,  sizeof(long),           &lType,
													     UT_TYPE_ULONG,  sizeof(long),           &lXid,
													     UT_TYPE_ULONG,  sizeof(long),           &lStime,
															 UT_TYPE_ULONG,  sizeof(long),           &lEtime,
															 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
		  
		   if(lStime>0){
		   		strcpy(s_stime, utTimFormat("%Y/%m/%d", lStime));	
		   }else{
		   		strcpy(s_stime, "");	
		   }
		   if(lEtime>0){
		   		strcpy(s_etime, utTimFormat("%Y/%m/%d", lEtime));	
		   }else{
		   		strcpy(s_etime, "");	
		   }
		  
			 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
			 utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
			 utPltPutLoopVar(psDbHead,"aname",iNum,caAname);
			 utPltPutLoopVar(psDbHead,"indexname",iNum,caIndexname);
			 utPltPutLoopVar(psDbHead,"odityname",iNum,caOdityname);
			 utPltPutLoopVarF(psDbHead,"couponfee",iNum,"%.2f",dCouponfee);
			 utPltPutLoopVarF(psDbHead,"discount",iNum,"%.2f",dDiscount);
			 utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
			 utPltPutLoopVar(psDbHead,"decript",iNum,caDecript);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",lPlevel);
			 utPltPutLoopVarF(psDbHead,"type",iNum,"%lu",lType);
			 utPltPutLoopVarF(psDbHead,"xid",iNum,"%lu",lXid);
			 utPltPutLoopVar(psDbHead,"stime",iNum,s_stime);
			 utPltPutLoopVar(psDbHead,"etime",iNum,s_etime);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 
			 lId = 0;
			 memset(caServicecode, 0, sizeof(caServicecode));
			 memset(caAname, 0, sizeof(caAname));
			 memset(caIndexname, 0, sizeof(caIndexname));
			 memset(caOdityname, 0, sizeof(caOdityname));
			 dCouponfee = 0.0;
			 dDiscount = 0.0;
			 memset(caAddress, 0, sizeof(caAddress));
			 memset(caDecript, 0, sizeof(caDecript));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lPlevel = 0;
			 lType = 0;
			 lXid = 0;
			 lStime = 0;
			 lEtime = 0;
			 lFlags = 0;
			 memset(s_stime, 0, sizeof(s_stime));
			 memset(s_etime, 0, sizeof(s_etime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),           &lId,
															     UT_TYPE_STRING, sizeof(caServicecode)-1,caServicecode,
															     UT_TYPE_STRING, sizeof(caAname)-1,      caAname,
															     UT_TYPE_STRING, sizeof(caIndexname)-1,  caIndexname,
															     UT_TYPE_STRING, sizeof(caOdityname)-1,  caOdityname,
															     UT_TYPE_FLOAT,  sizeof(double),         &dCouponfee,
															     UT_TYPE_FLOAT,  sizeof(double),         &dDiscount,
															     UT_TYPE_STRING, sizeof(caAddress)-1,    caAddress,
															     UT_TYPE_STRING, sizeof(caDecript)-1,    caDecript,
															     UT_TYPE_STRING, sizeof(caImgpath)-1,    caImgpath,
															     UT_TYPE_ULONG,  sizeof(long),           &lPlevel,
															     UT_TYPE_ULONG,  sizeof(long),           &lType,
															     UT_TYPE_ULONG,  sizeof(long),           &lXid,
															     UT_TYPE_ULONG,  sizeof(long),           &lStime,
																	 UT_TYPE_ULONG,  sizeof(long),           &lEtime,
																	 UT_TYPE_ULONG,  sizeof(long),           &lFlags);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomseries_list.htm");
	  return 0;	
}

int ncm_webcomseries_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
//#endif  
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char caKey[32] = "";
	 char caSid[32] = "";
	 char caServicecode[16] = "";
	 char caAname[32] = "";
	 unsigned long lShopid = 0;
	 unsigned long lSid = 0;
	 unsigned long lCid = 0;
	 double fDiscount = 0.0;
	 char caAddress[255] = "";
	 char caDecript[4096] = "";
	 char caImgpath[128] = "";
	 unsigned long lPlevel = 0;
	 unsigned long lType = 0;
	 unsigned long lXid = 0;
	 unsigned long lStime = 0;
	 unsigned long lEtime = 0;
	 
	 char s_stime[32] = "";
	 char s_etime[32] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 2,"key", UT_TYPE_STRING, sizeof(caKey)-1, caKey,
	 															 "sid", UT_TYPE_STRING, sizeof(caSid)-1, caSid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caSid)>0){
			 sprintf(sql, "select servicecode, aname, shopid, sid, cid, discount, address, decript, imgpath, plevel, type, xid, stime, etime from ncmactive where id=%d", atol(caSid));
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,15,caServicecode,
			                      UT_TYPE_STRING,31,caAname,
			                      UT_TYPE_ULONG,4,&lShopid,
			                      UT_TYPE_ULONG,4,&lSid,
			                      UT_TYPE_ULONG,4,&lCid,
			                      UT_TYPE_FLOAT,sizeof(double),&fDiscount,
			                      UT_TYPE_STRING,31,caAddress,
		                        UT_TYPE_STRING,254,caDecript,
		                        UT_TYPE_STRING,127,caImgpath,
		                        UT_TYPE_ULONG,4,&lPlevel,
		                        UT_TYPE_ULONG,4,&lType,
		                        UT_TYPE_ULONG,4,&lXid,
		                        UT_TYPE_ULONG,4,&lStime,
		                        UT_TYPE_ULONG,4,&lEtime);
		    
		    if(lStime>0){
		    		strcpy(s_stime, utTimFormat("%Y/%m/%d", lStime));	
		    }else{
		    		strcpy(s_stime, "");	
		    }            
			  if(lEtime>0){
			  		strcpy(s_etime, utTimFormat("%Y/%m/%d", lEtime));	
			  }else{
			  		strcpy(s_etime, "");	
			  }
			  
			  utPltPutVar(psDbHead,"id",caSid);
			  utPltPutVar(psDbHead,"scode",caServicecode);
				utPltPutVar(psDbHead,"aname",caAname);
				utPltPutVarF(psDbHead,"indexid","%lu",lShopid);
				utPltPutVarF(psDbHead,"odityid","%lu",lSid);
				utPltPutVarF(psDbHead,"couponid","%lu",lCid);
				utPltPutVarF(psDbHead,"discount","%.2f",fDiscount);
				utPltPutVar(psDbHead,"address",caAddress);
				utPltPutVar(psDbHead,"decript",caDecript);
		    utPltPutVar(psDbHead,"imgpath",caImgpath);
		    utPltPutVarF(psDbHead,"plevel","%lu",lPlevel);
		    utPltPutVarF(psDbHead,"type","%lu",lType);
		    utPltPutVarF(psDbHead,"xid","%lu",lXid);
		    utPltPutVar(psDbHead,"stime",s_stime);
		    utPltPutVar(psDbHead,"etime",s_etime);
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomseries_dr.htm");
	 return 0;		
}

// 上传二级活动图片
int ncm_webComSeriesImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
    
    char caKey[32]="";
   	char caId[20]="";
   	char caScode[16]="";
    char caAname[32]="";
    char caIndexid[32]="";
    char caOdityid[32]="";
    char caCouponid[20] = "";
    char caDiscount[20]="";
    char caAddress[255]="";
    char caDecript[4096]="";
    char caImgPath[128]="";
    char caDispImgPath[128]="";
    char imgname[128]="";
    char caPlevel[20] = "";
    char caType[20] = "";
    char caXid[20] = "";
    char caStime[32]="";
    char caEtime[32]="";
    
    char caImgpath1[128]="";
    char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
 		char caTemp2[1024]="";
 		char caTemp3[1024]="";
 		unsigned long lSysTime=0;
 		char *p;
 		
 		unsigned long lMaxid = 0;
		unsigned long lShopid = 0;
		unsigned long lSid=0;
 		unsigned long lCid=0;
 		unsigned long lStime=0;
 		unsigned long lEtime=0;
 		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif
		
		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0194导入"),ncLang("0183打开文件出错"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// 建立图片存放路径
  	sprintf(caImgPath,"%s/images/ShopImages/seriesimages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);
    
	  // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	  utMsgGetSomeNVar(psMsgHead, 16,"key",      UT_TYPE_STRING,  sizeof(caKey)-1,            caKey,
	  														 "id",         UT_TYPE_STRING,  sizeof(caId)-1,             caId,
	                               "scode",      UT_TYPE_STRING,  sizeof(caScode)-1,          caScode,
	  														 "aname",      UT_TYPE_STRING,  sizeof(caAname)-1,          caAname,
	  														 "inid",       UT_TYPE_STRING,  sizeof(caIndexid)-1,        caIndexid,
	  														 "odid",       UT_TYPE_STRING,  sizeof(caOdityid)-1,        caOdityid,
	  														 "cpid",       UT_TYPE_STRING,  sizeof(caCouponid)-1,       caCouponid,
	  														 "discount",   UT_TYPE_STRING,  sizeof(caDiscount)-1,       caDiscount,
	  														 "address",    UT_TYPE_STRING,  sizeof(caAddress)-1,        caAddress,
	  														 "decript",    UT_TYPE_STRING,  sizeof(caDecript)-1,        caDecript,
	  														 "fname",      UT_TYPE_STRING,  sizeof(imp_file)-1,         imp_file,
	  														 "plevel",     UT_TYPE_STRING,  sizeof(caPlevel)-1,         caPlevel,
	  														 "type",       UT_TYPE_STRING,  sizeof(caType)-1,           caType,
	  														 "xid",        UT_TYPE_STRING,  sizeof(caXid)-1,            caXid,
																 "stime",      UT_TYPE_STRING,  sizeof(caStime)-1,          caStime,
																 "etime",      UT_TYPE_STRING,  sizeof(caEtime)-1,          caEtime);
	  
	  if(strlen(caAname)==0){
	  		utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","活动名称不能为空!","活动名称不能为空!");
        return 0;	
	  }
	                  
	  if(strlen(imp_file)!=0)
	  {
		  	p=utStrSkipSpaces(imp_file);
		  	/*从指定的字符串中取一个词
		  	
		  	  函数原型：
			　　char *utStrGetWord(char *p,char *pOut,
			　　                   short nMaxLen,char *pTerm);
			　　
					参数说明:
			　　p – 输入字符串
			　　pOut --- 输出字符串，必须要事先分配好空间
			　　nMaxLen --- 输出字符传的最大长度
			　　pTerm  ----- 终止条件的字符串，系统在扫描描时遇到该字符串某一个字符时结束。
			　　
			　　返回值：
			　　结束字符的指针。*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
				printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
					  memset(caDispImgPath, 0, sizeof(caDispImgPath));
				}else{
						sprintf(imgname, "series_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/seriesimages/");
				    strcat(caDispImgPath, imgname);	
				}
	  }
	  
	  sprintf(caTemp3, "select servicecode from ncmcomodity where sid = %lu", atol(caOdityid));
	  pasDbOneRecord(caTemp3,0,UT_TYPE_STRING,sizeof(caScode)-1,caScode);
	  ncTimeToLong(caStime, &lStime);
	  ncTimeToLong(caEtime, &lEtime);
	  
	  strcpy(caType, "2");
	  
	  if(strcmp(caKey, "update")==0){
	  	  sprintf(caTemp2, "select imgpath from ncmactive where id=%lu", atol(caId));
	  	  pasDbOneRecord(caTemp2, 0, UT_TYPE_STRING, sizeof(caImgpath1)-1, caImgpath1);
	  	  if(strlen(caDispImgPath)==0){ // 是否选择上传图片, 否：原图片不变
	  	  	  sprintf(caTemp,"update ncmactive set servicecode='%s',aname='%s',shopid=%lu,sid=%lu,cid=%lu,discount=%.2f,address='%s',decript='%s',imgpath='%s',type=%lu,xid=%lu,stime=%lu,etime=%lu where id=%lu",caScode,caAname,atol(caIndexid),atol(caOdityid),atol(caCouponid),atof(caDiscount),caAddress,caDecript,caImgpath1,atol(caType),atol(caXid),lStime,lEtime,atol(caId));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caImgpath1);
	  	  }else{ // 是：原图片删除，新图片加入
			  	  sprintf(temp3_file, "rm -f %s%s", SHOPIMGPATH, caImgpath1),
				  	system(temp3_file);
			  	  sprintf(caTemp,"update ncmactive set servicecode='%s',aname='%s',shopid=%lu,sid=%lu,cid=%lu,discount=%.2f,address='%s',decript='%s',imgpath='%s',type=%lu,xid=%lu,stime=%lu,etime=%lu where id=%lu",caScode,caAname,atol(caIndexid),atol(caOdityid),atol(caCouponid),atof(caDiscount),caAddress,caDecript,caDispImgPath,atol(caType),atol(caXid),lStime,lEtime,atol(caId));
					 	printf("caTemp = %s\n", caTemp);
					 	pasDbExecSqlF(caTemp);
					 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
			  }
	  }else if(strcmp(caKey, "add")==0){
	  		pasDbOneRecord("select max(id) from ncmactive", 0, UT_TYPE_ULONG, 4, &lMaxid);
		    sprintf(caTemp,"insert into ncmactive (id,servicecode,aname,shopid,sid,cid,discount,address,decript,imgpath,plevel,type,xid,stime,etime) values (%lu,'%s','%s',%lu,%lu,%lu,%.2f,'%s','%s','%s',%lu,%lu,%lu,%lu,%lu)",(lMaxid+1),caScode,caAname,atol(caIndexid),atol(caOdityid),atol(caCouponid),atof(caDiscount),caAddress,caDecript,caDispImgPath,atol(caPlevel),atol(caType),atol(caXid),lStime,lEtime);
			 	printf("caTemp = %s\n", caTemp);
			 	pasDbExecSqlF(caTemp);
			 	utPltPutVar(psDbHead,"imgpath",caDispImgPath);
	  }
	  
	  utPltPutVar(psDbHead,"mesg","数据加载成功");
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/webshop/ncm_webcomseries_dr_list.htm");
	  //utWebDispMsg(iFd,psMsgHead,"ncmshop/ncm_shop_back.htm","数据加载成功!","数据加载成功!");
    return 0;
}

// 单位管理
int ncm_websrvgroup_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char groupid[32] = "";
	 char groupname[32] = "";
	 char dispname[128] = "";
	 char moditime[32] = "";
	 char flags[32] = "";
	 	
	 // 存放数据库取出的数据的变量
	 unsigned long	lGroupid = 0;
	 char 					caGroupname[32] = "";
	 char 					caDispname[128] = "";
	 unsigned long	lModitime = 0;
	 unsigned long	lFlags = 0;
	 char           s_moditime[32] = "";
	 char           s_flags[32] = "";
	 
	 char caUpdate[16] = "";
	 char caSesid[1024] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 
	 unsigned long lSid = 0;
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
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 14, "keyword",  UT_TYPE_STRING,  sizeof(_keyword)-1, _keyword,
	                                "groupid",   UT_TYPE_STRING, sizeof(groupid)-1,   groupid,
	                                "groupname", UT_TYPE_STRING, sizeof(groupname)-1, groupname,
	                                "dispname",  UT_TYPE_STRING, sizeof(dispname)-1,  dispname,
	                                "moditime",  UT_TYPE_STRING, sizeof(moditime)-1,  moditime,
	                                "flags",     UT_TYPE_STRING, sizeof(flags)-1,     flags,
	                                "start",     UT_TYPE_STRING, sizeof(start)-1,     start,
															   	"limit",     UT_TYPE_STRING, sizeof(limit)-1,     limit,
															   	"sort",      UT_TYPE_STRING, sizeof(sort)-1,      sort,
															   	"dir",       UT_TYPE_STRING, sizeof(dir)-1,       dir,
																	"selsid",    UT_TYPE_STRING, 1000,                caSesid,
																	"del",			 UT_TYPE_STRING, 10,                  caDel,
																	"update",    UT_TYPE_STRING, 10,                  caUpdate,
																	"caExport",  UT_TYPE_STRING, 10,                  caExport);
	 
	 // 排序
	 if(strlen(sort)==0){
	 	  strcpy(sort,"groupid");
	 }
	 // 去除关键字前后的空格
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(groupname)>0){
			pasCvtGBK(2,groupname,caTemp,30);
			strcpy(groupname,caTemp);
   }
	 if(strlen(dispname)>0){
			pasCvtGBK(2,dispname,caTemp,30);
			strcpy(dispname,caTemp);
   }
   
   // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
	 	  lSid=utMd5Code(groupname,strlen(groupname),"ffff");
   	  if(lSid>2000000000) lSid=lSid-2000000000;
   	  if(lSid>2000000000) lSid=lSid-2000000000;
   	  printf("lSid = %d\n",lSid);
	 	
	 		if (strcmp(caUpdate, "update")==0){
	 				sprintf(caTemp,"update ncsrvgroup set groupid=%lu, groupname='%s', dispname='%s', moditime=%lu, flags=%lu where groupid=%lu",lSid,groupname,dispname,time(0),atol(flags),atol(groupid));
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }else if(strcmp(caUpdate, "add")==0){
		    	sprintf(caTemp,"insert into ncsrvgroup (groupid,groupname,dispname,moditime,flags) values (%lu,'%s','%s',%lu,%lu)",lSid,groupname,dispname,time(0),atol(flags));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from ncsrvgroup where groupid in(%s)",caSesid);
    	}
	 }
	 
	 // 查询
	 strcpy(sql, "select count(*) from ncsrvgroup where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (groupname like '%%%s%%')", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select groupid, groupname, dispname, moditime, flags from ncsrvgroup where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (groupname like '%%%s%%')", _keyword);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
															 UT_TYPE_STRING, sizeof(caGroupname)-1, caGroupname,
															 UT_TYPE_STRING, sizeof(caDispname)-1,  caDispname,
													     UT_TYPE_ULONG,  sizeof(long),          &lModitime,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags);	
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 
			 if(lModitime>0){
			 			strcpy(s_moditime, utTimFormat("%Y/%m/%d %H:%M:%S", lModitime));
			 }else{
			 		strcpy(s_moditime, "");
			 }
			 
			 switch(lFlags){
			 		case 0:strcpy(s_flags, "无效");break;
			 		case 1:strcpy(s_flags, "有效");break;
			 		default:break;
			 }
			 
			 utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
			 utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
			 utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
			 utPltPutLoopVar(psDbHead,"moditime",iNum,s_moditime);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltPutLoopVar(psDbHead,"s_flags",iNum,s_flags);
			 
			 lGroupid = 0;
			 memset(caGroupname, 0, sizeof(caGroupname));
			 memset(caDispname, 0, sizeof(caDispname));
			 lModitime = 0;
	     lFlags = 0;
	     memset(s_moditime, 0, sizeof(s_moditime));
	     memset(s_flags, 0, sizeof(s_flags));
	     iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
																	 UT_TYPE_STRING, sizeof(caGroupname)-1, caGroupname,
																	 UT_TYPE_STRING, sizeof(caDispname)-1,  caDispname,
															     UT_TYPE_ULONG,  sizeof(long),          &lModitime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags);		
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_websrvgroup_list.htm");
	  return 0;	
}

// AP管理
int ncm_websrvgroupap_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char pregwid[32] = "";
	 char groupid[32] = "";
	 char gwid[32] = "";
	 char longitude[64] = "";
	 char latitude[64] = "";
	 char addtime[32] = "";
	 char flags[32] = "";
	 	
	 // 存放数据库取出的数据的变量
	 unsigned long	lGroupid = 0;
	 char 					caGwid[32] = "";
	 double         dLongitude = 0.0;
	 double         dLatitude = 0.0;
	 unsigned long	lAddtime = 0;
	 unsigned long	lFlags = 0;
	 char           s_addtime[32] = "";
	 char           s_flags[32] = "";
	 
	 char caUpdate[16] = "";
	 char caSesid[4096] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
	 
	 unsigned long lSid = 0;
	 char Gwid[32] = "";
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
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 16, "keyword",  UT_TYPE_STRING, sizeof(_keyword)-1,  _keyword,
	                                "pregwid",   UT_TYPE_STRING, sizeof(pregwid)-1,   pregwid,
	                                "groupid",   UT_TYPE_STRING, sizeof(groupid)-1,   groupid,
	                                "gwid",      UT_TYPE_STRING, sizeof(gwid)-1,      gwid,
	                                "longitude", UT_TYPE_STRING, sizeof(longitude)-1, longitude,
	                                "latitude",  UT_TYPE_STRING, sizeof(latitude)-1,  latitude,
	                                "addtime",   UT_TYPE_STRING, sizeof(addtime)-1,   addtime,
	                                "flags",     UT_TYPE_STRING, sizeof(flags)-1,     flags,
	                                "start",     UT_TYPE_STRING, sizeof(start)-1,     start,
															   	"limit",     UT_TYPE_STRING, sizeof(limit)-1,     limit,
															   	"sort",      UT_TYPE_STRING, sizeof(sort)-1,      sort,
															   	"dir",       UT_TYPE_STRING, sizeof(dir)-1,       dir,
																	"selsid",    UT_TYPE_STRING, 1000,                caSesid,
																	"del",			 UT_TYPE_STRING, 10,                  caDel,
																	"update",    UT_TYPE_STRING, 10,                  caUpdate,
																	"caExport",  UT_TYPE_STRING, 10,                  caExport);
	 
	 // 排序
	 if(strlen(sort)==0){
	 	  strcpy(sort,"addtime");
	 }
	 // 去除关键字前后的空格
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(gwid)>0){
			pasCvtGBK(2,gwid,caTemp,30);
			strcpy(gwid,caTemp);
   }
   
   unsigned long lShopId = 0;
	 dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	 printf("lShopId = %lu\n", lShopId);
	 
   // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
	 		if (strcmp(caUpdate, "update")==0){
	 				sprintf(caTemp,"update ncsrvgroupap set groupid=%lu, gwid='%s', longitude=%.8f, latitude=%.8f, addtime=%lu, flags=%lu where gwid='%s'",lShopId,gwid,atof(longitude),atof(latitude),time(0),atol(flags),pregwid);
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }else if(strcmp(caUpdate, "add")==0){
	 	  	  sprintf(caTemp1, "select gwid from ncsrvgroupap where gwid='%s'", gwid);
	 	  	  printf("caTemp1 = %s\n", caTemp1);
	 	  	  pasDbOneRecord(caTemp1,0,UT_TYPE_STRING,sizeof(Gwid)-1,Gwid);
	 	  	  if(strcmp(Gwid, gwid)==0){
	 	  	  	  utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","AP已存在","AP已存在");
	 	  	  		return 0;	
	 	  	  }else{
		 	  	  	sprintf(caTemp,"insert into ncsrvgroupap (groupid,gwid,longitude,latitude,addtime,flags) values (%lu,'%s',%.8f,%.8f,%lu,%lu)",lShopId,gwid,atof(longitude),atof(latitude),time(0),atol(flags));
				 		  printf("caTemp = %s\n", caTemp);
				 		  pasDbExecSqlF(caTemp);
	 	  	  }
	 	  }
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from ncsrvgroupap where gwid in(%s)",caSesid);
    	}
	 }
	 
	 // 查询
	 strcpy(sql, "select count(*) from ncsrvgroupap where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (gwid like '%%%s%%')", _keyword);
	 }
	 if (lShopId > 0 ) // 每个商家自己查自己的
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupid = %lu ", lShopId);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select groupid, gwid, longitude, latitude, addtime, flags from ncsrvgroupap where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (gwid like '%%%s%%')", _keyword);
	 }
	 if (lShopId > 0 ) // 每个商家自己查自己的
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupid = %lu ", lShopId);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
															 UT_TYPE_STRING, sizeof(caGwid)-1,      caGwid,
															 UT_TYPE_FLOAT,  sizeof(double),        &dLongitude,
															 UT_TYPE_FLOAT,  sizeof(double),        &dLatitude,
													     UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags);	
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 
			 if(lAddtime>0){
			 		strcpy(s_addtime, utTimFormat("%Y/%m/%d %H:%M:%S", lAddtime));
			 }else{
			 		strcpy(s_addtime, "");
			 }
			 
			 switch(lFlags){
			 		case 0:strcpy(s_flags, "未开通");break;
			 		case 1:strcpy(s_flags, "正常使用");break;
			 		default:break;
			 }
			 
			 utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
			 utPltPutLoopVar(psDbHead,"gwid",iNum,caGwid);
			 utPltPutLoopVarF(psDbHead,"longitude",iNum,"%.8f",dLongitude);
			 utPltPutLoopVarF(psDbHead,"latitude",iNum,"%.8f",dLatitude);
			 utPltPutLoopVar(psDbHead,"addtime",iNum,s_addtime);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltPutLoopVar(psDbHead,"s_flags",iNum,s_flags);
			 
			 lGroupid = 0;
			 memset(caGwid, 0, sizeof(caGwid));
			 dLongitude = 0.0;
			 dLatitude = 0.0;
			 lAddtime = 0;
	     lFlags = 0;
	     memset(s_addtime, 0, sizeof(s_addtime));
	     memset(s_flags, 0, sizeof(s_flags));
	     iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
																	 UT_TYPE_STRING, sizeof(caGwid)-1,      caGwid,
																	 UT_TYPE_FLOAT,  sizeof(double),        &dLongitude,
																	 UT_TYPE_FLOAT,  sizeof(double),        &dLatitude,
															     UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags);	
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_websrvgroupap_list.htm");
	  return 0;	
}

// SSID管理
int ncm_websrvgroupssid_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char groupid[32] = "";
	 char ssid[32] = "";
	 char addtime[32] = "";
	 char flags[32] = "";
	 	
	 // 存放数据库取出的数据的变量
	 unsigned long	lGroupid = 0;
	 char 					caSsid[32] = "";
	 unsigned long	lAddtime = 0;
	 unsigned long	lFlags = 0;
	 char           s_addtime[32] = "";
	 char           s_flags[32] = "";
	 
	 char caUpdate[16] = "";
	 char caSesid[1024] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 
	 unsigned long lSid = 0;
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
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 13, "keyword",  UT_TYPE_STRING,  sizeof(_keyword)-1, _keyword,
	                                "groupid",   UT_TYPE_STRING, sizeof(groupid)-1,   groupid,
	                                "ssid",      UT_TYPE_STRING, sizeof(ssid)-1,      ssid,
	                                "addtime",   UT_TYPE_STRING, sizeof(addtime)-1,   addtime,
	                                "flags",     UT_TYPE_STRING, sizeof(flags)-1,     flags,
	                                "start",     UT_TYPE_STRING, sizeof(start)-1,     start,
															   	"limit",     UT_TYPE_STRING, sizeof(limit)-1,     limit,
															   	"sort",      UT_TYPE_STRING, sizeof(sort)-1,      sort,
															   	"dir",       UT_TYPE_STRING, sizeof(dir)-1,       dir,
																	"selsid",    UT_TYPE_STRING, 1000,                caSesid,
																	"del",			 UT_TYPE_STRING, 10,                  caDel,
																	"update",    UT_TYPE_STRING, 10,                  caUpdate,
																	"caExport",  UT_TYPE_STRING, 10,                  caExport);
	 
	 // 排序
	 if(strlen(sort)==0){
	 	  strcpy(sort,"groupid");
	 }
	 // 去除关键字前后的空格
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(ssid)>0){
			pasCvtGBK(2,ssid,caTemp,30);
			strcpy(ssid,caTemp);
   }
	 
   // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
	 	  lSid=utMd5Code(ssid,strlen(ssid),"ffff");
   	  if(lSid>2000000000) lSid=lSid-2000000000;
   	  if(lSid>2000000000) lSid=lSid-2000000000;
   	  //printf("lSid = %d\n",lSid);
	 	  
	 		if (strcmp(caUpdate, "update")==0){
	 				sprintf(caTemp,"update ncsrvgroupssid set groupid=%lu, ssid='%s', addtime=%lu, flags=%lu where groupid=%lu", lSid,ssid,time(0),atol(flags),atol(groupid));
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }else if(strcmp(caUpdate, "add")==0){
		    	sprintf(caTemp,"insert into ncsrvgroupssid (groupid,ssid,addtime,flags) values (%lu,'%s',%lu,%lu)",lSid,ssid,time(0),atol(flags));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from ncsrvgroupssid where groupid in(%s)",caSesid);
    	}
	 }
	 
	 // 查询
	 strcpy(sql, "select count(*) from ncsrvgroupssid where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (ssid like '%%%s%%')", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select groupid, ssid, addtime, flags from ncsrvgroupssid where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (ssid like '%%%s%%')", _keyword);
	 }
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
															 UT_TYPE_STRING, sizeof(caSsid)-1,      caSsid,
													     UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlags);	
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 
			 if(lAddtime>0){
			 		strcpy(s_addtime, utTimFormat("%Y/%m/%d %H:%M:%S", lAddtime));
			 }else{
			 		strcpy(s_addtime, "");
			 }
			 
			 switch(lFlags){
			 		case 0:strcpy(s_flags, "未开通");break;
			 		case 1:strcpy(s_flags, "正常使用");break;
			 		default:break;
			 }
			 
			 utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
			 utPltPutLoopVar(psDbHead,"ssid",iNum,caSsid);
			 utPltPutLoopVar(psDbHead,"addtime",iNum,s_addtime);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltPutLoopVar(psDbHead,"s_flags",iNum,s_flags);
			 
			 lGroupid = 0;
			 memset(caSsid, 0, sizeof(caSsid));
			 lAddtime = 0;
	     lFlags = 0;
	     memset(s_addtime, 0, sizeof(s_addtime));
	     memset(s_flags, 0, sizeof(s_flags));
	     iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
																	 UT_TYPE_STRING, sizeof(caSsid)-1,      caSsid,
															     UT_TYPE_ULONG,  sizeof(long),          &lAddtime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlags);	
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_websrvgroupssid_list.htm");
	  return 0;	
}

int Ncm_WebShopSWW_SetFunName()
{
		pasSetTcpFunName("ncm_webcomseries_list", ncm_webcomseries_list, 0); // 商城二级活动信息-0303
		pasSetTcpFunName("ncm_webcomseries_info", ncm_webcomseries_info, 0);
		pasSetTcpFunName("ncm_webComSeriesImg_dr", ncm_webComSeriesImg_dr, 0);
		
		pasSetTcpFunName("ncm_webcomcomp_list", ncm_webcomcomp_list, 0); // 商品商店对照表-0421
		
		pasSetTcpFunName("ncm_websrvgroup_list", ncm_websrvgroup_list, 0); // 单位管理-0423
		pasSetTcpFunName("ncm_websrvgroupap_list", ncm_websrvgroupap_list, 0); // AP管理-0423
		pasSetTcpFunName("ncm_websrvgroupssid_list", ncm_websrvgroupssid_list, 0); // SSID管理-0423
		
		return 0;	
}
