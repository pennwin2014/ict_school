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
#include "ncportal.h"      
#include <iconv.h>
#define SHOPIMGPATH "/home/ncmysql/ncsrv/html"
#define UPLOADIMGPATH "/home/ncmysql/ncsrv/upload"

// 商城信息信息
int ncm_comshops_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	
	
	 char sql[4096] = "";
	 char caName[128];
	 unsigned long lId;
	 pasDbCursor *psCur = NULL;
	 char caShopids[1024];
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 strcpy(caShopids,getDsShopids());
	 
	 strcpy(sql,"select groupid,dispname from ncsrvgroup where 1 ");
	 if(strlen(caShopids)>0){
	 sprintf(sql+strlen(sql)," and groupid in (%s) ",caShopids);
	 }
	 
//	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),            &lId,
													     UT_TYPE_STRING, sizeof(caName)-1,        caName);
													   
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1){
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); 
			 }
			 
		  
		   utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
			 utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			
		
			 lId = 0;
			 memset(caName, 0, sizeof(caName));
	     iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),            &lId,
													     UT_TYPE_STRING, sizeof(caName)-1,        caName);
	  }
	  //utPltShowDb(psDbHead);
	  utPltPutVarF(psDbHead,"TotRec","%d",iNum);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomshop_list.htm");
	  return 0;	
}


// 楼层信息
int ncm_comfloor_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
//	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char caFlags[32] = "";
	 char caPlateid[32] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lFid = 0;
	 unsigned long	lGroupid = 0;
	 char 					caName[32] = "";
	 char 					caImgpath[128] = "";
	 unsigned long  lPlateid = 0;
	 unsigned long	lFlags = 0;
	 char           caCid[20];
	 char caShopids[1024];
	 strcpy(caShopids,getDsShopids());
	 utMsgGetSomeNVar(psMsgHead, 1, "cid",UT_TYPE_STRING, sizeof(caCid)-1,  caCid);
	 																
	 sprintf(sql, "select fid,name from ncmfloor where 1=1");
	 if(strlen(caCid)>0){
	 	sprintf(sql+strlen(sql)," and cid=%s ",caCid);
	}
	if(strlen(caShopids)>0){
		sprintf(sql+strlen(sql)," and cid in (%s) ",caShopids);
	}
	
//	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lFid,
													     UT_TYPE_STRING, sizeof(caName)-1,      caName);
													  
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"fid",iNum,"%lu",lFid);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltSetCvtHtml(0);
			 lFid = 0;
			 lGroupid = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caImgpath, 0, sizeof(caImgpath));
			 lPlateid = 0;
	     lFlags = 0;
	    iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lFid,
													     UT_TYPE_STRING, sizeof(caName)-1,      caName);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltPutVarF(psDbHead,"TotRec","%d",iNum);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webmall_list.htm");
	  return 0;	
}


// 促销优惠
int ncm_webcxyh_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
//	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char sid[20] = "";
	 char name[128] = "";
	 char dispname[128] = "";
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
	 char plateid[20] = "";
	 char temp_file[128] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lSid = 0;
	 char 					caName[128] = "";
	 char 					caDispname[128] = "";
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
	 unsigned long  lPlateid = 0;
	 char           s_systime[32] = "";
	 unsigned long lGtype;
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caSesid[1024] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
	 char caHid[16]="";
	 // 关键字
	 char _keyword[33] = "";
	 long lShowflag,lNum;
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
	 unsigned long lCid;
	 char caStartStoptime[32];
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
   ncPortalGroup *psGroup;
 	 char caShopids[1024];
	 strcpy(caShopids,getDsShopids());
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 9,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport,
																	"hidshw",UT_TYPE_STRING,  10,                       caHid);
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
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
    			sprintf(caTemp, "select imgpath from ncmcomodity_cxyh where sid in(%s)", caSesid);
 //   			 printf("-----------------------------------------\n");
  //  			printf("caTemp = %s\n", caTemp);
    		  psCur = pasDbOpenSql(caTemp, 0);
    		  if(psCur==NULL){
					 		utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
				      return 0;	
					}
					
					// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
    		  iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);
    		  
				  while(0 == iret || 1405 == iret){
				  	 if(strlen(imgpath) > 0){
		    		  		sprintf(temp_file, "rm -f %s%s", SHOPIMGPATH, imgpath);
	//	    		  		printf("temp_file = %s\n", temp_file);
		    		  		system(temp_file);
		    		 }
		    		 
		    		 memset(imgpath, 0, sizeof(imgpath));
		    		 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(imgpath)-1,   imgpath);

				  }
				  pasDbCloseCursor(psCur);
				  pasDbExecSqlF("delete from ncmcomodity_cxyh where sid in(%s)",caSesid);
					
				
    	}
	 }

	 // 更新状态
	 	
	 if(!utStrIsSpaces(caHid)) {	 		
    	
    	if(strlen(caSesid)>0){
    		
	//			  printf("-----------------------------------------\n");
		//		  printf("update ncmcomodity_cxyh set flags= ABS(flags-1) where sid in(%s)\n",caSesid);
				  pasDbExecSqlF("update ncmcomodity_cxyh set flags= ABS(flags-1) where sid in(%s)",caSesid);
					
				
    	}
	 }
	 
	 // 查询
	 strcpy(sql, "select count(*) from ncmcomodity_cxyh where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (name like '%%%s%%' or shopname like '%%%s%%' or factory like '%%%s%%'  ) ", _keyword, _keyword, _keyword);
	 }
	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and cid in (%s) ",caShopids);
	}
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
//	 printf("lTotRec = %d\n", lTotRec);
//	 printf("lCount1 = %d\n", lCount1);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select o.sid, o.name, o.dispname, o.cid, t.name, o.shopid, o.imgpath, o.shopname, o.factory, o.type, o.fee, o.discount,  o.descr, o.systime, o.flags,o.startstoptime,o.reserve,o.gtype,o.showflag,o.num from ncmcomodity_cxyh o left join  ncmcomtype t on o.gtype = t.sid where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
		 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (o.name like '%%%s%%' or o.shopname like '%%%s%%' or o.factory like '%%%s%%'  ) ", _keyword, _keyword, _keyword);
	 }
	 if(strlen(caShopids)>0){
	 	sprintf(sql+strlen(sql)," and cid in (%s) ",caShopids);
	}
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by o.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
//	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),            &lSid,
													     UT_TYPE_STRING, sizeof(caName)-1,        caName,
													     UT_TYPE_STRING, sizeof(caDispname)-1,    caDispname,
													     UT_TYPE_ULONG,4,&lCid,
													     UT_TYPE_STRING, sizeof(caTyname)-1,      caTyname,
													     UT_TYPE_ULONG,  sizeof(long),            &lShgroupid,
															 UT_TYPE_STRING, sizeof(caImgpath)-1,     caImgpath,
															 UT_TYPE_STRING, sizeof(caShopname)-1,    caShopname,
															 UT_TYPE_STRING, sizeof(caFactory)-1,     caFactory,
															 UT_TYPE_STRING, sizeof(caType)-1,        caType,
															 UT_TYPE_FLOAT,  sizeof(double),          &dFee,
															 UT_TYPE_FLOAT,  sizeof(double),          &dDiscount,
															 UT_TYPE_STRING, sizeof(caDescr)-1,       caDescr,
															 UT_TYPE_ULONG,  sizeof(long),            &lSystime,
															 UT_TYPE_ULONG,  sizeof(long),            &lFlags,
															 UT_TYPE_STRING,sizeof(caStartStoptime)-1,caStartStoptime,
															 UT_TYPE_ULONG,4,&lReserve,
															 UT_TYPE_ULONG,4,&lGtype,
															 UT_TYPE_ULONG,4,&lShowflag,
															 UT_TYPE_ULONG,4,&lNum);
															
	 
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
		   
		   utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
			 utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
//			 utPltPutLoopVar(psDbHead,"indexname",iNum,caIndexname);
			 utPltPutLoopVar(psDbHead,"tyname",iNum,caTyname);
			 utPltPutLoopVarF(psDbHead,"shgroupid",iNum,"%lu",lShgroupid);
			 utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
			 utPltPutLoopVar(psDbHead,"shopname",iNum,caShopname);
       utPltPutLoopVar(psDbHead,"active",iNum,caStartStoptime);
			 utPltPutLoopVar(psDbHead,"factory",iNum,caFactory);
			 utPltPutLoopVar(psDbHead,"type",iNum,caType);
			 utPltPutLoopVarF(psDbHead,"gtype",iNum,"%lu",lGtype);
			 utPltPutLoopVarF(psDbHead,"fee",iNum,"%.2f",dFee);
			 utPltPutLoopVarF(psDbHead,"discount",iNum,"%.2f",dDiscount);
			 utPltPutLoopVar(psDbHead,"code",iNum,caCode);
			 utPltPutLoopVarF(psDbHead,"tel",iNum,"%lu",lTel);
			 utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
			 utPltPutLoopVarF(psDbHead,"reserve",iNum,"%lu",lReserve);
			
			 utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);		
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltPutLoopVarF(psDbHead,"cid",iNum,"%lu",lCid);
			 utPltPutLoopVarF(psDbHead,"showflag",iNum,"%lu",lShowflag);
			 utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",lNum);
			 psGroup = (ncPortalGroup *) ncSrvGetGroupInfoById(psShmHead,lCid);
			 if(psGroup){
			 	utPltPutLoopVarF(psDbHead,"indexname",iNum,psGroup->caDisp);
			 }
			 utPltSetCvtHtml(0);
			 
			 lSid = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caDispname, 0, sizeof(caDispname));
			 memset(caTyname, 0, sizeof(caTyname));
			 lShgroupid = 0;
			 memset(caImgpath, 0, sizeof(caImgpath));
			 memset(caShopname, 0, sizeof(caShopname));
			 memset(caFactory, 0, sizeof(caFactory));
			 memset(caType, 0, sizeof(caType));
			
			 dFee = 0;
			 dDiscount = 0;
			 lReserve=0;
			 memset(caDescr, 0, sizeof(caDescr));
		   lGtype=0;
			 lSystime = 0;
			 lFlags = 0;
			 memset(s_systime, 0, sizeof(s_systime));
			 lShowflag=0;
			 lNum=0;
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),            &lSid,
													     UT_TYPE_STRING, sizeof(caName)-1,        caName,
													     UT_TYPE_STRING, sizeof(caDispname)-1,    caDispname,
													     UT_TYPE_ULONG,4,&lCid,
													     UT_TYPE_STRING, sizeof(caTyname)-1,      caTyname,
													     UT_TYPE_ULONG,  sizeof(long),            &lShgroupid,
															 UT_TYPE_STRING, sizeof(caImgpath)-1,     caImgpath,
															 UT_TYPE_STRING, sizeof(caShopname)-1,    caShopname,
															 UT_TYPE_STRING, sizeof(caFactory)-1,     caFactory,
															 UT_TYPE_STRING, sizeof(caType)-1,        caType,
															 UT_TYPE_FLOAT,  sizeof(double),          &dFee,
															 UT_TYPE_FLOAT,  sizeof(double),          &dDiscount,
															 UT_TYPE_STRING, sizeof(caDescr)-1,       caDescr,
															 UT_TYPE_ULONG,  sizeof(long),            &lSystime,
															 UT_TYPE_ULONG,  sizeof(long),            &lFlags,
															 UT_TYPE_STRING,sizeof(caStartStoptime)-1,caStartStoptime,
															 UT_TYPE_ULONG,4,&lReserve,
															 UT_TYPE_ULONG,4,&lGtype,
															 UT_TYPE_ULONG,4,&lShowflag,
															 UT_TYPE_ULONG,4,&lNum);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmshop/ncm_webcomodity_list.htm");
	  return 0;	
}

//保存促销优惠
int ncmCxyh_Save(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  utPltDbHead *psDbHead = NULL;
 
        long iReturn,lSid;
  char caPlateindex[64];;
        char caSid[12],caName[36],caDispname[36],caImgpath[128],caShopname[36],caFactory[36],caType[128],caStartStoptime[54],caDescr[256];
        unsigned long lTime = time(0);
        char caPlateid[32],caPid[20];
        char caTemp[1024];
        unsigned long lCid,lShopid;
        char caGtype[16],caFee[20],caDiscount[32],caCid[20],caShopid[20],caReserve[20];
        unsigned long lReserve;
  long lCount;
  char caDel[16];
  char caMark[1024];
 //       utMsgPrintMsg(psMsgHead);
  char caShowflag[10],caNum[10];
 psDbHead = utPltInitDbHead();

        utMsgGetSomeNVar(psMsgHead,15,
                                   "sid",   UT_TYPE_STRING, sizeof(caSid)-1, caSid,
                                   "name", UT_TYPE_STRING,31,caName,
                                   "dispname", UT_TYPE_STRING,sizeof(caDispname)-1,caDispname,
                                   "type",UT_TYPE_STRING,sizeof(caType)-1,caType,
                                   "gtype",     UT_TYPE_STRING,sizeof(caGtype)-1,caGtype,
                                   "cid",      UT_TYPE_STRING,sizeof(caCid)-1,caCid,
                                   "shopid",     UT_TYPE_STRING,sizeof(caShopid)-1,caShopid,
                                   "factory",    UT_TYPE_STRING,sizeof(caFactory)-1,caFactory,
                                   "fee",  UT_TYPE_STRING,sizeof(caFee)-1,caFee,
                                   "discount",   UT_TYPE_STRING,sizeof(caDiscount)-1,caDiscount,
                                   "active",UT_TYPE_STRING,sizeof(caStartStoptime)-1,caStartStoptime,
                                   "descr",  UT_TYPE_STRING,sizeof(caDescr)-1,caDescr,
                                   "reserve",UT_TYPE_STRING,sizeof(caReserve)-1,caReserve,
                                   "showflag",UT_TYPE_STRING,sizeof(caShowflag)-1,caShowflag,
                                   "num",UT_TYPE_STRING,sizeof(caNum)-1,caNum);
                         sscanf(caReserve,"%lu",&lReserve);        
                         memset(caShopname,0,sizeof(caShopname));
                         sscanf(caCid,"%lu",&lCid);
                         sscanf(caShopid,"%lu",&lShopid);
                         if(lShopid>0){
                         	sprintf(caTemp,"select name from ncmshop where id=%lu ",lShopid);
                         	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,31,caShopname);
                        }

                         if(strlen(caName)>0){
                          pasCvtGBK(2,caName,caTemp,31);
                          strcpy(caName,caTemp);
                        }

                        if(strlen(caDispname)>0){
                          pasCvtGBK(2,caDispname,caTemp,31);
                          strcpy(caDispname,caTemp);
                        }


                        if(strlen(caType)>0){
                          pasCvtGBK(2,caType,caTemp,100);
                          strcpy(caType,caTemp);
                        }
                        if(strlen(caDiscount)>0){
                          pasCvtGBK(2,caDiscount ,caTemp,31);
                          strcpy(caDiscount ,caTemp);
                        }
                       
					              if(strstr(caDiscount,"输入")){
					                strcpy(caDiscount,"");
					              }

			                  if(strlen(caFactory)>0){
			                      pasCvtGBK(2,caFactory,caTemp,31);
			                      strcpy(caFactory,caTemp);
			                   }
			                
			                   if(strlen(caDescr)>0){
			                        pasCvtGBK(2,caDescr,caTemp,127);
			                        strcpy(caDescr,caTemp);
			                   }
                     
                        if(strlen(caSid)==0){
//                        	printf("insert into ncmcomodity_cxyh(name,dispname,gtype,shopid,imgpath,shopname,factory,type,fee,discount,startstoptime,systime,descr,cid,reserve) \
//                                  values('%s','%s',%lu,%lu,'%s','%s','%s','%s',%f,%f,'%s',%d,'%s',%lu,%lu) \n",
//                                   caName,caDispname,atol(caGtype),lShopid,caImgpath,caShopname,caFactory,caType,atof(caFee),atof(caDiscount),caStartStoptime,time(0),caDescr,lCid,lReserve);
                                iReturn=pasDbExecSqlF("insert into ncmcomodity_cxyh(name,dispname,gtype,shopid,imgpath,shopname,factory,type,fee,discount,startstoptime,systime,descr,cid,reserve,showflag,num) \
                                  values('%s','%s',%lu,%lu,'%s','%s','%s','%s',%f,%f,'%s',%d,'%s',%lu,%lu,%lu,%lu) ",
                                   caName,caDispname,atol(caGtype),lShopid,"",caShopname,caFactory,caType,atof(caFee),atof(caDiscount),caStartStoptime,time(0),caDescr,lCid,lReserve,atol(caShowflag),atol(caNum));
                                if(iReturn==0){
                                        pasDbOneRecord("select max(sid) from ncmcomodity_cxyh",0,UT_TYPE_LONG,4,&lSid);
                                        sprintf(caSid,"%d",lSid);
                                }
                        }
                        else{
   //                     	printf("update ncmcomodity_cxyh set name='%s',dispname='%s',gtype=%lu,shopid=%lu,shopname='%s',factory='%s',type='%s',fee=%f,discount=%f,startstoptime='%s',systime=%d,descr='%s',cid=%lu where sid=%s \n",
   //                             caName,caDispname,atol(caGtype),lShopid,caShopname,caFactory,caType,atof(caFee),atof(caDiscount),caStartStoptime,time(0),caDescr,lCid,caSid);
                                iReturn=pasDbExecSqlF("update ncmcomodity_cxyh set name='%s',dispname='%s',gtype=%lu,shopid=%lu,shopname='%s',factory='%s',type='%s',fee=%f,discount=%f,startstoptime='%s',systime=%d,descr='%s',cid=%lu,reserve=%lu,showflag=%lu,num=%lu where sid=%s ",
                                caName,caDispname,atol(caGtype),lShopid,caShopname,caFactory,caType,atof(caFee),atof(caDiscount),caStartStoptime,time(0),caDescr,lCid,lReserve,atol(caShowflag),atol(caNum),caSid);
                        }


     if(iReturn==0){
        utPltPutVar(psDbHead,"titel","true");
        utPltPutVarF(psDbHead,"sid","%s",caSid);
        utPltPutVar(psDbHead,"message","保存成功");
    }
    else{
        utPltPutVar(psDbHead,"titel","false");
        utPltPutVarF(psDbHead,"sid","%s",caSid);
        utPltPutVar(psDbHead,"message","保存失败");
    }

  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v8/nc_adfile.htm");

}

//上传促销优惠图片文件
int ncmCxyh_upload(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  utPltDbHead *psDbHead = NULL;
 	char caFile[256] = "";
  char caImgPath[128]="";
	  char caDispImgPath[128]="";
	  char imgname[128]="";
	  char imp_file[256]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
	  char *p=NULL;
	  char *q=NULL;
	  long iReturn;
	  char caPlateid[32];
// utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,2,
		                   "sid",   UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid,
		                   "adfile",UT_TYPE_STRING,sizeof(caFile)-1,caFile);
		                
	 iReturn=-1;
	
		  if(strlen(caFile)>0){
		  	char *descr_gbk = pasConvert("UTF-8", "GBK", caFile);
				 if(descr_gbk){
				 	strcpy(caFile,descr_gbk);
				  free(descr_gbk);
				}
		  	
		  }
        
        if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",(char *)ncLang("0194导入"),(char *)ncLang("0183打开文件出错"));
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
    psDbHead = utPltInitDbHead(); 
    
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);    
    if(strlen(caFile)>0){
    		p=utStrSkipSpaces(caFile);
    		p=utStrGetWord(p,temp_file,200,";\n");
    		strcpy(temp3_file, temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
				}else{
					  q = strtok(temp3_file, ".");
					  //if(q)printf("%s\n", q);
					  q = strtok(NULL, ".");
					  //if(q)printf("%s\n", q);
					  srand((unsigned int)time(0));
						sprintf(imgname, "shop_%lu_%lu.%s", time(0), rand()%100, q);
						//printf("imgname = %s\n", imgname);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
						//printf("temp2_file = %s\n", temp2_file);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/shopimages/");
				    strcat(caDispImgPath, imgname);
				    
				     iReturn=pasDbExecSqlF("update ncmcomodity_cxyh set imgpath='%s' where sid=%s ",caDispImgPath,caPlateid);
				    
				    
				    
				}
    		
    		
    		
    		
    }    
        
        
        
        
        
        
        
        
        
      
        
        
        
       
                
    
    if(iReturn==0){
    	utPltPutVar(psDbHead,"titel","true");
    
    	utPltPutVar(psDbHead,"adfile",caDispImgPath);
    	utPltPutVar(psDbHead,"message","上传成功");
    }
    else{
    	utPltPutVar(psDbHead,"titel","false");
    
    	utPltPutVar(psDbHead,"message","上传失败");
    }

    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v8/nc_adfile.htm");	


}

//显示促销优惠界面 移动终端
int ncmShowsCxyh(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmShops start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();

	char tsid[32] ="";
	char plateid[32] ="";
  char caImgpath[128],caDescr[128],caStartstoptime[48];
  double lFee,lDiscount;
  unsigned long lSid,lNum,getShopId,iNum;
	char caTemp[2048];
	long iReturn;
	char *pUTF;
// utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 2,
					"tsid",        	 UT_TYPE_STRING,  sizeof(tsid)-1,      		 tsid,
					"plateid",       UT_TYPE_STRING,  sizeof(plateid)-1,     	 plateid);
					
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	int tReturn = 0;
	tReturn = ncmShopUserOnlineFlag(psShmHead, iFd,psMsgHead);
	// printf("********test******tReturn = %d \n",tReturn);
	utPltPutVarF(psDbHead,"user_online_flag","%d", tReturn);  
	// printf("shopid = %s\n",shopid);	
	
	getShopId = ncSrvGetCid(psShmHead,psMsgHead);
 // printf("getShopId = %d \n",getShopId);
  iNum=0;
   sprintf(caTemp,"select sid,imgpath,descr,startstoptime,fee,discount,num from ncmcomodity_cxyh where cid=%lu and flags=0 and showflag=0 order by num",getShopId);
//   printf("caTemp=%s\n",caTemp);
	 psCur = pasDbOpenSql(caTemp,0);
	 if(psCur){
	 	while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&lSid,
      	                    UT_TYPE_STRING,127,caImgpath,
      	                    UT_TYPE_STRING,127,caDescr,
      	                    UT_TYPE_STRING,47,caStartstoptime,
      	                    UT_TYPE_FLOAT,sizeof(double),&lFee,
      	                    UT_TYPE_FLOAT,sizeof(double),&lDiscount,
      	                    UT_TYPE_LONG,4,&lNum)
      	                     )  || 1405==iReturn)
      	     
		{	
			iNum++;
			utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			utPltPutLoopVarF(psDbHead,"discount",iNum,"%.1f",10*lDiscount);
			utPltPutLoopVarF(psDbHead,"fee",iNum,"%.2f",lFee);
			utPltSetCvtHtml(1);
			if(strlen(caStartstoptime)>0){
		  pUTF = convert("GBK", "UTF-8", caStartstoptime);
		  if(pUTF){
					utPltPutLoopVar(psDbHead,"startstoptime",iNum,pUTF);
					free(pUTF);
		    }			
		  }
			
			if(strlen(caDescr)>0){
		  pUTF = convert("GBK", "UTF-8", caDescr);
		  if(pUTF){
					utPltPutLoopVar(psDbHead,"descr",iNum,pUTF);
					free(pUTF);
		    }			
		  }
			if(strlen(caImgpath)>0){
		  pUTF = convert("GBK", "UTF-8", caImgpath);
		  if(pUTF){
					utPltPutLoopVar(psDbHead,"imgpath",iNum,pUTF);
					free(pUTF);
		    }			
		  } 	
	 }
	 pasDbCloseCursor(psCur);
	}

//显示一行三个图片

  iNum=0;
   sprintf(caTemp,"select sid,imgpath,dispname,startstoptime,fee,discount,num from ncmcomodity_cxyh where cid=%lu and flags=0 and showflag=1 order by num",getShopId);
//  printf("caTemp=%s\n",caTemp);
	 psCur = pasDbOpenSql(caTemp,0);
	 if(psCur){
	 	long ix,iy;
	 	while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&lSid,
      	                    UT_TYPE_STRING,127,caImgpath,
      	                    UT_TYPE_STRING,127,caDescr,
      	                    UT_TYPE_STRING,47,caStartstoptime,
      	                    UT_TYPE_FLOAT,sizeof(double),&lFee,
      	                    UT_TYPE_FLOAT,sizeof(double),&lDiscount,
      	                    UT_TYPE_LONG,4,&lNum)
      	                     )  || 1405==iReturn)
      	     
		{	
		
			ix=iNum/3+1;
			iy=iNum%3+1;
			
				iNum++;
			
			utPltPutLoopVarF2(psDbHead,"sid",ix,iy,"%lu",lSid);
			utPltPutLoopVarF2(psDbHead,"curfee",ix,iy,"%.1f",lFee*lDiscount);
			utPltPutLoopVarF2(psDbHead,"fee",ix,iy,"%.2f",lFee);
			utPltSetCvtHtml(1);
			if(strlen(caStartstoptime)>0){
		  pUTF = convert("GBK", "UTF-8", caStartstoptime);
		  if(pUTF){
					utPltPutLoopVar2(psDbHead,"startstoptime",ix,iy,pUTF);
					free(pUTF);
		    }			
		  }
			
			if(strlen(caDescr)>0){
		  pUTF = convert("GBK", "UTF-8", caDescr);
		  if(pUTF){
					utPltPutLoopVar2(psDbHead,"descr",ix,iy,pUTF);
					free(pUTF);
		    }			
		  }
			if(strlen(caImgpath)>0){
		  pUTF = convert("GBK", "UTF-8", caImgpath);
		  if(pUTF){
					utPltPutLoopVar2(psDbHead,"imgpath",ix,iy,pUTF);
					free(pUTF);
		    }			
		  } 
		 
	 }
	 pasDbCloseCursor(psCur);
	}







	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/termweb/ncm_cxyh.htm");
	
    return 0;	  
}


//显示促销优惠商品信息
int ncmShowsCxyhGoods(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmShops start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();

	char tsid[32] ="";
	char plateid[32] ="";
  char caImgpath[128],caDescr[128],caStartstoptime[48];
  double lFee,lDiscount;
  unsigned long lSid,lNum,getShopId,iNum;
	char caTemp[2048];
	char caSid[20];
	char caName[36],caDispname[36],caShopname[36],caFactory[36],caType[36];
	unsigned long lGtype,lShopid,lReserve;
	long iReturn;
	char *pUTF;
//utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 3,
					"tsid",        	 UT_TYPE_STRING,  sizeof(tsid)-1,      		 tsid,
					"plateid",       UT_TYPE_STRING,  sizeof(plateid)-1,     	 plateid,
					"sid",           UT_TYPE_STRING, sizeof(caSid)-1,caSid);
					
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	int tReturn = 0;
	tReturn = ncmShopUserOnlineFlag(psShmHead, iFd,psMsgHead);
	// printf("********test******tReturn = %d \n",tReturn);
	utPltPutVarF(psDbHead,"user_online_flag","%d", tReturn);  
	// printf("shopid = %s\n",shopid);	
	
 
  iNum=0;
   sprintf(caTemp,"select name,dispname,gtype,shopid,shopname,factory,type,fee,discount,reserve,imgpath,descr,startstoptime from ncmcomodity_cxyh where sid=%s ",caSid);
 //  printf("caTemp=%s\n",caTemp);
	 pasDbOneRecord(caTemp,0,
      	                    UT_TYPE_STRING,31,caName,
      	                    UT_TYPE_STRING,31,caDispname,
      	                    UT_TYPE_LONG,4,&lGtype,
      	                    UT_TYPE_LONG,4,&lShopid,
      	                    UT_TYPE_STRING,31,caShopname,
      	                    UT_TYPE_STRING,31,caFactory,
      	                    UT_TYPE_STRING,127,caType,
      	                    UT_TYPE_FLOAT,sizeof(double),&lFee,
      	                    UT_TYPE_FLOAT,sizeof(double),&lDiscount,
      	                    UT_TYPE_LONG,4,&lReserve,
      	                    UT_TYPE_STRING,127,caImgpath,
      	                    UT_TYPE_STRING,127,caDescr,
      	                    UT_TYPE_STRING,47,caStartstoptime);
      	                   
      	     
	
			iNum++;
			utPltPutVarF(psDbHead,"sid","%lu",lSid);
			utPltPutVarF(psDbHead,"discount","%.1f",10*lDiscount);
			utPltPutVarF(psDbHead,"reserve","%d",lReserve);
			utPltPutVarF(psDbHead,"curprize","%.2f",lFee*lDiscount);
			utPltPutVarF(psDbHead,"fee","%.2f",lFee);
			utPltSetCvtHtml(1);
			if(strlen(caStartstoptime)>0){
		  pUTF = convert("GBK", "UTF-8", caStartstoptime);
		  if(pUTF){
					utPltPutVar(psDbHead,"startstoptime",pUTF);
					free(pUTF);
		    }			
		  }
			
			if(strlen(caType)>0){
		  pUTF = convert("GBK", "UTF-8", caType);
		  if(pUTF){
					utPltPutVar(psDbHead,"type",pUTF);
					free(pUTF);
		    }			
		  }
			
			
		  if(strlen(caName)>0){
		  pUTF = convert("GBK", "UTF-8", caName);
		  if(pUTF){
					utPltPutVar(psDbHead,"name",pUTF);
					free(pUTF);
		    }			
		  }
		 if(strlen(caDispname)>0){
		  pUTF = convert("GBK", "UTF-8", caDispname);
		  if(pUTF){
					utPltPutVar(psDbHead,"dispname",pUTF);
					free(pUTF);
		    }			
		  }
			
			if(strlen(caShopname)>0){
		  pUTF = convert("GBK", "UTF-8", caShopname);
		  if(pUTF){
					utPltPutVar(psDbHead,"shopname",pUTF);
					free(pUTF);
		    }			
		  }
			
			if(strlen(caFactory)>0){
		  pUTF = convert("GBK", "UTF-8", caFactory);
		  if(pUTF){
					utPltPutVar(psDbHead,"factory",pUTF);
					free(pUTF);
		    }			
		  }
			
			
			if(strlen(caDescr)>0){
		  pUTF = convert("GBK", "UTF-8", caDescr);
		  if(pUTF){
					utPltPutVar(psDbHead,"descr",pUTF);
					free(pUTF);
		    }			
		  }
			if(strlen(caImgpath)>0){
		  pUTF = convert("GBK", "UTF-8", caImgpath);
		  if(pUTF){
					utPltPutVar(psDbHead,"imgpath",pUTF);
					free(pUTF);
		    }			
		  } 	
     sprintf(caTemp,"select name from ncmcomtype where sid=%lu ",lGtype);
     memset(caName,0,sizeof(caName));
     pasDbOneRecord(caTemp,0,UT_TYPE_STRING,31,caName);
     if(strlen(caName)>0){
		  pUTF = convert("GBK", "UTF-8", caName);
		  if(pUTF){
					utPltPutVar(psDbHead,"gtype",pUTF);
					free(pUTF);
		    }			
		  }

	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/termweb/ncm_cxyh_goods.htm");
	
    return 0;	  
}


//显示促销 商品分类
int ncmShowsSpfl(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmShops start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();

	char tsid[32] ="";
	char plateid[32] ="";
  char caImgpath[128],caDescr[128],caStartstoptime[48];
  double lFee,lDiscount;
  unsigned long lSid,lNum,getShopId,iNum;
  unsigned long lGroupid;
	char caTemp[2048],caName[36],caGroupid[16];
	char caDispname[36];
	long iReturn;
	char *pUTF;
//utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 3,
					"tsid",        	 UT_TYPE_STRING,  sizeof(tsid)-1,      		 tsid,
					"plateid",       UT_TYPE_STRING,  sizeof(plateid)-1,     	 plateid,
					"groupid",       UT_TYPE_STRING,10,caGroupid);
					
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	int tReturn = 0;
	tReturn = ncmShopUserOnlineFlag(psShmHead, iFd,psMsgHead);
	// printf("********test******tReturn = %d \n",tReturn);
	utPltPutVarF(psDbHead,"user_online_flag","%d", tReturn);  
	// printf("shopid = %s\n",shopid);	
	
	getShopId = ncSrvGetCid(psShmHead,psMsgHead);
 // printf("getShopId = %d \n",getShopId);
  
  //输出商品类别
  sprintf(caTemp,"select c.name,c.groupid from ncmcomodity a,ncmcomtype b,ncmcomgroup c where a.groupid=b.sid and b.groupid=c.groupid ");
  if(getShopId>0){
  	sprintf(caTemp+strlen(caTemp)," and a.sgroupid=%lu ",getShopId);
  } 
  sprintf(caTemp+strlen(caTemp)," group by c.name limit 0,6 ");
  
  iNum=0;

//   printf("caTemp=%s\n",caTemp);
	 psCur = pasDbOpenSql(caTemp,0);
	 if(psCur){
	 	while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_STRING,31,caName,
      	                    UT_TYPE_ULONG,4,&lGroupid)
      	                     )  || 1405==iReturn)      	     
		{	
			iNum++;
			utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
		
			utPltSetCvtHtml(1);
			if(strlen(caName)>0){
		  pUTF = convert("GBK", "UTF-8", caName);
		  if(pUTF){
					utPltPutLoopVar(psDbHead,"groupname",iNum,pUTF);
					free(pUTF);
		    }			
		  }
			
		
	 }
	 pasDbCloseCursor(psCur);
	}

//显示一行三个图片

  iNum=0;
   sprintf(caTemp,"select a.sid,a.imgpath,a.dispname,fee,discount from ncmcomodity a where 1 ");
  
   if(getShopId>0){
  	 sprintf(caTemp+strlen(caTemp)," and a.sgroupid=%lu ",getShopId);
   }
   if(strlen(caGroupid)>0){
   	sprintf(caTemp+strlen(caTemp)," and a.groupid in (select sid from ncmcomtype where groupid=%s) ",caGroupid);
  }
  sprintf(caTemp+strlen(caTemp)," order by a.sid desc limit 0,20 ");
  
//   printf("caTemp=%s\n",caTemp);
	 psCur = pasDbOpenSql(caTemp,0);
	 if(psCur){
	 	long ix,iy;
	 	while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&lSid,
      	                    UT_TYPE_STRING,127,caImgpath,
      	                    UT_TYPE_STRING,31,caDispname,
      	                    UT_TYPE_FLOAT,sizeof(double),&lFee,
      	                    UT_TYPE_FLOAT,sizeof(double),&lDiscount)
      	                     )  || 1405==iReturn)
      	     
		{	
		
				 ix=iNum/2+1;
			   iy=iNum%2+1;
			
				iNum++;
					utPltSetCvtHtml(0);
			if(iy==1){
				utPltPutLoopVar2(psDbHead,"style",ix,iy,"style=\"margin-right:1em\"");
			}
			utPltPutLoopVarF2(psDbHead,"sid",ix,iy,"%lu",lSid);
			utPltPutLoopVarF2(psDbHead,"discount",ix,iy,"%.1f",10*lDiscount);
			utPltPutLoopVarF2(psDbHead,"fee",ix,iy,"%.1f",lFee);
      utPltPutLoopVarF2(psDbHead,"curprize",ix,iy,"%.1f",lFee*lDiscount);
			utPltSetCvtHtml(1);
				utPltSetCvtHtml(1);
			if(strlen(caDispname)>0){
		  pUTF = convert("GBK", "UTF-8", caDispname);
		  if(pUTF){
					utPltPutLoopVar2(psDbHead,"dispname",ix,iy,pUTF);
					free(pUTF);
		    }			
		  }
			
		
			if(strlen(caImgpath)>0){
		  pUTF = convert("GBK", "UTF-8", caImgpath);
		  if(pUTF){
					utPltPutLoopVar2(psDbHead,"imgpath",ix,iy,pUTF);
					free(pUTF);
		    }			
		  } 
		 
	 }
	 pasDbCloseCursor(psCur);
	}



	utPltSetCvtHtml(0);



	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/termweb/ncm_spfl.htm");
	
    return 0;	  
}


//显示商品分类商品信息
int ncmShowsSpflGoods(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmShops start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();

	char tsid[32] ="";
	char plateid[32] ="";
  char caImgpath[128],caDescr[128],caStartstoptime[48];
  double lFee,lDiscount;
  unsigned long lSid,lNum,getShopId,iNum;
	char caTemp[2048];
	char caSid[20];
	char caName[36],caDispname[36],caShopname[36],caFactory[36],caType[36];
	unsigned long lGtype,lShopid,lReserve;
	char caActive[64];
	long iReturn;
	char *pUTF;
//utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 3,
					"tsid",        	 UT_TYPE_STRING,  sizeof(tsid)-1,      		 tsid,
					"plateid",       UT_TYPE_STRING,  sizeof(plateid)-1,     	 plateid,
					"sid",           UT_TYPE_STRING, sizeof(caSid)-1,caSid);
					
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	int tReturn = 0;
	tReturn = ncmShopUserOnlineFlag(psShmHead, iFd,psMsgHead);
	// printf("********test******tReturn = %d \n",tReturn);
	utPltPutVarF(psDbHead,"user_online_flag","%d", tReturn);  
	// printf("shopid = %s\n",shopid);	
	
 
  iNum=0;
   sprintf(caTemp,"select name,dispname,groupid,shgroupid,shopname,factory,type,fee,discount,reserve,imgpath,descr,active from ncmcomodity where sid=%s ",caSid);
 //  printf("caTemp=%s\n",caTemp);
	 pasDbOneRecord(caTemp,0,
      	                    UT_TYPE_STRING,31,caName,
      	                    UT_TYPE_STRING,31,caDispname,
      	                    UT_TYPE_LONG,4,&lGtype,
      	                    UT_TYPE_LONG,4,&lShopid,
      	                    UT_TYPE_STRING,31,caShopname,
      	                    UT_TYPE_STRING,31,caFactory,
      	                    UT_TYPE_STRING,127,caType,
      	                    UT_TYPE_FLOAT,sizeof(double),&lFee,
      	                    UT_TYPE_FLOAT,sizeof(double),&lDiscount,
      	                    UT_TYPE_LONG,4,&lReserve,
      	                    UT_TYPE_STRING,127,caImgpath,
      	                    UT_TYPE_STRING,127,caDescr,
      	                    UT_TYPE_STRING,60,caActive);
      	                  
      	                   
      	     
	
			iNum++;
			utPltPutVarF(psDbHead,"sid","%lu",lSid);
			utPltPutVarF(psDbHead,"discount","%.1f",10*lDiscount);
			utPltPutVarF(psDbHead,"reserve","%d",lReserve);
			utPltPutVarF(psDbHead,"curprize","%.2f",lFee*lDiscount);
			utPltPutVarF(psDbHead,"fee","%.2f",lFee);
			utPltSetCvtHtml(1);
		
			
			if(strlen(caType)>0){
		  pUTF = convert("GBK", "UTF-8", caType);
		  if(pUTF){
					utPltPutVar(psDbHead,"type",pUTF);
					free(pUTF);
		    }			
		  }
			
			if(strlen(caActive)>0){
		  pUTF = convert("GBK", "UTF-8", caActive);
		  if(pUTF){
					utPltPutVar(psDbHead,"startstoptime",pUTF);
					free(pUTF);
		    }			
		  }
			
			
			
		  if(strlen(caName)>0){
		  pUTF = convert("GBK", "UTF-8", caName);
		  if(pUTF){
					utPltPutVar(psDbHead,"name",pUTF);
					free(pUTF);
		    }			
		  }
		 if(strlen(caDispname)>0){
		  pUTF = convert("GBK", "UTF-8", caDispname);
		  if(pUTF){
					utPltPutVar(psDbHead,"dispname",pUTF);
					free(pUTF);
		    }			
		  }
			
			if(strlen(caShopname)>0){
		  pUTF = convert("GBK", "UTF-8", caShopname);
		  if(pUTF){
					utPltPutVar(psDbHead,"shopname",pUTF);
					free(pUTF);
		    }			
		  }
			
			if(strlen(caFactory)>0){
		  pUTF = convert("GBK", "UTF-8", caFactory);
		  if(pUTF){
					utPltPutVar(psDbHead,"factory",pUTF);
					free(pUTF);
		    }			
		  }
			
			
			if(strlen(caDescr)>0){
		  pUTF = convert("GBK", "UTF-8", caDescr);
		  if(pUTF){
					utPltPutVar(psDbHead,"descr",pUTF);
					free(pUTF);
		    }			
		  }
			if(strlen(caImgpath)>0){
		  pUTF = convert("GBK", "UTF-8", caImgpath);
		  if(pUTF){
					utPltPutVar(psDbHead,"imgpath",pUTF);
					free(pUTF);
		    }			
		  } 	
     sprintf(caTemp,"select name from ncmcomtype where sid=%lu ",lGtype);
     memset(caName,0,sizeof(caName));
     pasDbOneRecord(caTemp,0,UT_TYPE_STRING,31,caName);
     if(strlen(caName)>0){
		  pUTF = convert("GBK", "UTF-8", caName);
		  if(pUTF){
					utPltPutVar(psDbHead,"gtype",pUTF);
					free(pUTF);
		    }			
		  }

	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/termweb/ncm_cxyh_goods.htm");
	
    return 0;	  
}


//显示楼层品牌
int ncmShowsFloorBrand(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmShops start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();

	char tsid[32] ="";
	char plateid[32] ="";
  char caImgpath[128],caDescr[128],caStartstoptime[48];
  double lFee,lDiscount;
  unsigned long lSid,lNum,getShopId,iNum;
  unsigned long lGroupid;
	char caTemp[2048],caName[36],caGroupid[16];
	char caDispname[36];
	long iReturn,i;
	unsigned long lSum=0;
	char *pUTF;
	unsigned lBid;
	struct floor_s{
		unsigned long fid;
		char name[32];
	};
	struct floor_s psFloor[256];
	memset(&psFloor,0,256*sizeof(struct floor_s));
	
//utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 3,
					"tsid",        	 UT_TYPE_STRING,  sizeof(tsid)-1,      		 tsid,
					"plateid",       UT_TYPE_STRING,  sizeof(plateid)-1,     	 plateid,
					"groupid",       UT_TYPE_STRING,10,caGroupid);
					
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	int tReturn = 0;
	tReturn = ncmShopUserOnlineFlag(psShmHead, iFd,psMsgHead);
	// printf("********test******tReturn = %d \n",tReturn);
	utPltPutVarF(psDbHead,"user_online_flag","%d", tReturn);  
	// printf("shopid = %s\n",shopid);	
	
	getShopId = ncSrvGetCid(psShmHead,psMsgHead);
//  printf("getShopId = %d \n",getShopId);
  
  sprintf(caTemp,"select a.fid,a.name from ncmfloor a,ncmbrand b where a.fid=b.fid and a.cid=%lu group by a.fid,a.name order by a.name ",getShopId);
//  printf("caTemp=%s\n",caTemp);
  psCur = pasDbOpenSql(caTemp,0);
	 if(psCur){
	 	long ix,iy;
	 	while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&lSid,
      	                    UT_TYPE_STRING,31,caName)
      	                     )  || 1405==iReturn)
    {
    	psFloor[lSum].fid=lSid;
    	strcpy(psFloor[lSum].name,caName);
    	lSum++;
    }
    pasDbCloseCursor(psCur);
  }
  
  for(i=0;i<lSum;i++){
  	iNum=0;
  	sprintf(caTemp,"select bid,name,imgpath from ncmbrand where fid=%lu ",psFloor[i].fid);
 // 	printf("caTemp=%s\n",caTemp);
  	psCur=pasDbOpenSql(caTemp,0);
  	if(psCur){
  		    utPltPutLoopVarF(psDbHead,"fid",i+1,"%lu",psFloor[i].fid);
  		    
  		    if(strlen(psFloor[i].name)>0){
						  pUTF = convert("GBK", "UTF-8", psFloor[i].name);
						  if(pUTF){
									utPltPutLoopVar(psDbHead,"fname",i+1,pUTF);
									free(pUTF);
						    }			
  		    
  		       }
  		    long ix,iy;
  			 	while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&lBid,
      	                    UT_TYPE_STRING,31,caName,
      	                    UT_TYPE_STRING,127,caImgpath)
      	                     )  || 1405==iReturn)
			    {
			     ix=iNum/4+1;
			     iy=iNum%4+1;
			    	iNum++;
			    	utPltPutLoopVarF3(psDbHead,"bid",i+1,ix,iy,"%lu",lBid);
			    		utPltSetCvtHtml(1);
							if(strlen(caName)>0){
						  pUTF = convert("GBK", "UTF-8", caName);
						  if(pUTF){
									utPltPutLoopVar3(psDbHead,"bname",i+1,ix,iy,pUTF);
									free(pUTF);
						    }			
						  }
						  if(strlen(caImgpath)>0){
						  pUTF = convert("GBK", "UTF-8", caImgpath);
						  if(pUTF){
									utPltPutLoopVar3(psDbHead,"imgpath",i+1,ix,iy,pUTF);
									free(pUTF);
						    }			
						  }
						  if(iy==1){
						  	utPltPutLoopVar3(psDbHead,"class",i+1,ix,iy,"br_img");
						  }
						  else{
						  	utPltPutLoopVar3(psDbHead,"class",i+1,ix,iy,"r_img");
						  }
			    }
			    pasDbCloseCursor(psCur);  		
  	}
  }

	utPltSetCvtHtml(0);

	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/termweb/ncm_floorbrand.htm");
	
    return 0;	  
}



//显示商标信息
int ncmShowsGwdhGoods(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmShops start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();

	char tsid[32] ="";
	char plateid[32] ="";
  char caImgpath[128],caDescr[128],caStartstoptime[48];
  double lFee,lDiscount;
  unsigned long lSid,lNum,getShopId,iNum;
	char caTemp[2048];
	char caSid[20];

	char caName[36],caDispname[36],caShopname[36],caFactory[36],caType[36];
	unsigned long lGtype,lShopid,lReserve,lBtype;
	char caFname[32],caDetail[1024],caUrl[256],caTel[32],caAddress[256];
	long iReturn;
	char *pUTF;
//utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 3,
					"tsid",        	 UT_TYPE_STRING,  sizeof(tsid)-1,      		 tsid,
					"plateid",       UT_TYPE_STRING,  sizeof(plateid)-1,     	 plateid,
					"sid",           UT_TYPE_STRING, sizeof(caSid)-1,caSid);
					
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	int tReturn = 0;
	tReturn = ncmShopUserOnlineFlag(psShmHead, iFd,psMsgHead);
	// printf("********test******tReturn = %d \n",tReturn);
	utPltPutVarF(psDbHead,"user_online_flag","%d", tReturn);  
	// printf("shopid = %s\n",shopid);	
	
 
  iNum=0;
   sprintf(caTemp,"select a.name,b.name,a.shopid,a.btype,b.detail,a.imgpath,a.descr from ncmbrand a,ncmfloor b where a.bid=%s and a.fid=b.fid ",caSid);
//   printf("caTemp=%s\n",caTemp);
	 pasDbOneRecord(caTemp,0,
      	                    UT_TYPE_STRING,31,caName,
      	                    UT_TYPE_STRING,31,caFname,
      	                    UT_TYPE_ULONG,4,&lShopid,
      	                    UT_TYPE_ULONG,4,&lBtype,
      	                    UT_TYPE_STRING,1000,caDetail,
      	                    UT_TYPE_STRING,127,caImgpath,
      	                    UT_TYPE_STRING,127,caDescr);
      	                  
      	                   
      	     
	
			iNum++;
			utPltPutVarF(psDbHead,"sid","%s",caSid);
		
			utPltSetCvtHtml(1);
		
			
			if(strlen(caName)>0){
		  pUTF = convert("GBK", "UTF-8", caName);
		  if(pUTF){
					utPltPutVar(psDbHead,"name",pUTF);
					free(pUTF);
		    }			
		  }
			
			
		  if(strlen(caFname)>0){
		  pUTF = convert("GBK", "UTF-8", caFname);
		  if(pUTF){
					utPltPutVar(psDbHead,"fname",pUTF);
					free(pUTF);
		    }			
		  }
		 if(strlen(caDetail)>0){
		  pUTF = convert("GBK", "UTF-8", caDetail);
		  if(pUTF){
					utPltPutVar(psDbHead,"detail",pUTF);
					free(pUTF);
		    }			
		  }
		if(strlen(caDescr)>0){
		  pUTF = convert("GBK", "UTF-8", caDescr);
		  if(pUTF){
					utPltPutVar(psDbHead,"descr",pUTF);
					free(pUTF);
		    }			
		  }
		
			if(strlen(caImgpath)>0){
		  pUTF = convert("GBK", "UTF-8", caImgpath);
		  if(pUTF){
					utPltPutVar(psDbHead,"imgpath",pUTF);
					free(pUTF);
		    }			
		  } 	
     sprintf(caTemp,"select name from ncmcomtype where sid=%lu ",lBtype);
     memset(caName,0,sizeof(caName));
     pasDbOneRecord(caTemp,0,UT_TYPE_STRING,31,caName);
     if(strlen(caName)>0){
		  pUTF = convert("GBK", "UTF-8", caName);
		  if(pUTF){
					utPltPutVar(psDbHead,"gtype",pUTF);
					free(pUTF);
		    }			
		  }
//取商店信息
     sprintf(caTemp,"select name,url,tel,address,descr from ncmshop where id=%lu ",lShopid);
//     printf("caTemp=%s\n",caTemp);
     memset(caName,0,sizeof(caName));
     memset(caUrl,0,sizeof(caUrl));
     memset(caTel,0,sizeof(caTel));
     memset(caAddress,0,sizeof(caAddress));
     memset(caDescr,0,sizeof(caDescr));
     pasDbOneRecord(caTemp,0,
                             UT_TYPE_STRING,31,caName,
                             UT_TYPE_STRING,127,caUrl,
                             UT_TYPE_STRING,31,caTel,
                             UT_TYPE_STRING,255,caAddress,
                             UT_TYPE_STRING,255,caDescr);
     if(strlen(caName)>0){
		  pUTF = convert("GBK", "UTF-8", caName);
		  if(pUTF){
					utPltPutVar(psDbHead,"shopname",pUTF);
					free(pUTF);
		    }			
		  }

     if(strlen(caAddress)>0){
		  pUTF = convert("GBK", "UTF-8", caAddress);
		  if(pUTF){
					utPltPutVar(psDbHead,"address",pUTF);
					free(pUTF);
		    }			
		  }

     if(strlen(caDescr)>0){
		  pUTF = convert("GBK", "UTF-8", caDescr);
		  if(pUTF){
					utPltPutVar(psDbHead,"descr",pUTF);
					free(pUTF);
		    }			
		  }

    utPltSetCvtHtml(0);
    utPltPutVar(psDbHead,"url",caUrl);
    utPltPutVar(psDbHead,"tel",caTel);
    

	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/termweb/ncm_gwdh_goods.htm");
	
    return 0;	  
}

//接入单位树AJAX通讯
int ncsTreeUser_corp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[72],caGroupid[16];
   	char caTemp[4024];
   	char caName[32];
   	char caWhere[256];
    char caDispname[128];
   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
  	char caServicecode[24];
   	unsigned long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
   	char caVar[256],caFname[64],caBfname[64];
   	char caValue[256];
   	char caVar1[32];
   	char caOpt[256];
   	char caGroups[1024];
   	char caFacs[1024];
   	long lId;
   	char *p,*pVar;
  
   	char caCname[72];

    long lCount;
    char caCname1[72];
    char caSql[2048];
    char caCode[16];
    char caShopids[1024];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
  //  utMsgPrintMsg(psMsgHead);
    
    memset(caCname,0,sizeof(caCname));
   	iReturn = utMsgGetSomeNVar(psMsgHead,3,
					"node",        UT_TYPE_STRING,10,caPid,
					"keyword",       UT_TYPE_STRING,30,caCname1,
					"plate",       UT_TYPE_STRING,100,caPlate);
		strcpy(caShopids,getDsShopids());

    if(strlen(caCname1)>0){
    	strcpy(caCname,pasCvtGBK(2,caCname1,caTemp,30));
    }
//    iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
    strcpy(caWhere,"\0");

   



    lRowNum = utComGetVar_ld(psShmHead,"GroupTreeRow",300);
  
    
 
    iNum = 0;
    
     psDbHead = utPltInitDb();
   
      utPltPutVar(psDbHead,"rootname",utComGetVar_sd(psShmHead,"rootname","全部单位"));
    	
     
    	sprintf(caTemp,"select dispname,groupid from ncsrvgroup where 1=1 ");
  
    	if(strlen(caShopids)>0){
    		sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",caShopids);
    	}
    
    	
    if(strlen(caCname)>0){
    	    sprintf(caTemp+strlen(caTemp)," and (groupname like '%c%s%c' or dispname like '%c%s%c') ",'%',caCname,'%','%',caCname,'%');
    }
		
    sprintf(caTemp+strlen(caTemp)," order by dispname limit %lu,%lu",0,lRowNum);
//      printf("caTemp22=%s\n",caTemp);
       psCur = pasDbOpenSql(caTemp,0);
    	 if(psCur!=NULL){
    	    memset(caDispname,0,sizeof(caDispname));
			    lGroupid=0;
			  
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caDispname,
			                                   UT_TYPE_ULONG,4,&lGroupid);
			                                   
			
			    while(iReturn == 0) {
			        
			         if(iNum>0){
			         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
			         }
			         iNum++;
			         utPltSetCvtHtml(1);  
			         utPltPutLoopVar(psDbHead,"groupname", iNum,utStrGetCharHan(caDispname,60));
			         utPltSetCvtHtml(0);  
			         utPltPutLoopVarF(psDbHead,"did",   iNum,"%lu",lGroupid);

			          	utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
			         		utPltPutLoopVar(psDbHead,"cls", iNum,"file");
			          	
			            utPltPutLoopVar(psDbHead,"dtype",iNum,"0");
			 
			        
			
			        utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
			

    	    memset(caDispname,0,sizeof(caDispname));
			    lGroupid=0;
			  
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caDispname,
			                                   UT_TYPE_ULONG,4,&lGroupid);
			    }
			    pasDbCloseCursor(psCur);
    	  }
    	
    	

    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_user_tree.htm");
    

    return 0;
}

// 上网人数
int ncm_dnsswrs_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
//	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 

	 unsigned long	lSid = 0;

	 // 关键字
	 char _keyword[33] = "";
	 long lNum;
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
	 unsigned long lCid;
	 char caStartStoptime[32];
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
   ncPortalGroup *psGroup;
   char caSdate[16],caEdate[16],caStime[12],caEtime[12];
   char caSdate_all[20],caEdate_all[20];
 	 char caShopids[1024];
 	 char caStatway[20],caDid[20],caCname[128];
 	 unsigned long lStime,lEtime,lWay,lGroupid;
	 strcpy(caShopids,getDsShopids());
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 12,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"sdate",      UT_TYPE_STRING,  10,caSdate,
																	"edate",      UT_TYPE_STRING,10,caEdate,
																	"stime",      UT_TYPE_STRING,10,caStime,
																	"etime",      UT_TYPE_STRING,10,caEtime,
																	"statway",    UT_TYPE_STRING,4,caStatway,
																	"did",        UT_TYPE_STRING,10,caDid,
																	"exp",     UT_TYPE_STRING, 30 ,caExport                            
																	);
																	      
  //         printf("this is exp = %s\n", caExport);        
	    sprintf(caSdate_all,"%s %s",caSdate,caStime);
	    lStime=utTimStrToLong("%Y/%m/%d %H:%M:%S",caSdate_all);
	     sprintf(caEdate_all,"%s %s",caEdate,caEtime);
	     lEtime=utTimStrToLong("%Y/%m/%d %H:%M:%S",caEdate_all);
	    
	    
	    if(atol(caStatway)==1){                              //按时间端统计
	    	sprintf(sql,"select 1,from_unixtime(starttime,'%cY/%cm/%cd') sdate,count(distinct dispname),0 from ncsrvuserlog where 1 ",'%','%','%');
      if(strlen(caDid)>0){
      	sprintf(sql+strlen(sql)," and a.groupid=%s ",caDid);
      }
      if(strlen(caShopids)>0){
      	sprintf(sql+strlen(sql)," and a.groupid in (%s) ",caShopids);
      }
      sprintf(sql+strlen(sql)," and starttime>=%lu and starttime<=%lu and dispname!='Guest' and dispname!='' ",lStime,lEtime);
      sprintf(sql+strlen(sql)," group by sdate  order by sdate limit 0,500");

      if(strlen(caExport) > 0)
      {
      	sprintf(caFilename, "ncmstatistic%d.csv",utStrGetId());
//      	printf("\ngetid = %d\n",utStrGetId() );
      	sprintf(caPath,"%s/html","/home/ncs81/ncsrv");
      	sprintf(caFile,"%s/%s",caPath, caFilename);

      	fp = fopen(caFile,"w");
      	if(fp == NULL)
      	{
 //     		printf("fp error \n");
      		return 0;
      	}

      	fprintf(fp,"上网人数统计\n");
      	ncUtlPrintCsv(fp, 2,
      			UT_TYPE_STRING,"时间段",
                UT_TYPE_STRING,"上网人数");

      }

     }
	   else{
	   	
	   	sprintf(sql,"select  0,b.dispname sdate,count(distinct a.dispname),a.groupid from ncsrvuserlog a left join ncsrvgroup b on (a.groupid=b.groupid) where 1 ",'%','%');
      if(strlen(caDid)>0){
      	sprintf(sql+strlen(sql)," and a.groupid=%s ",caDid);
      }
      if(strlen(caShopids)>0){
      	sprintf(sql+strlen(sql)," and a.groupid in (%s) ",caShopids);
      }
       sprintf(sql+strlen(sql)," and starttime>=%lu and starttime<=%lu and a.dispname!='Guest' and a.dispname!='' ",lStime,lEtime);
      sprintf(sql+strlen(sql)," group by sdate,a.groupid  order by sdate limit 0,500");
      if(strlen(caExport) > 0)
      {
      	sprintf(caFilename, "ncmstatistic%d.csv",utStrGetId());
 //     	printf("\ngetid = %d\n",utStrGetId() );
      	sprintf(caPath,"%s/html","/home/ncs81/ncsrv");
      	sprintf(caFile,"%s/%s",caPath, caFilename);

      	fp = fopen(caFile,"w");
      	if(fp == NULL)
      	{
      		printf("fp error \n");
      		return 0;
      	}

      	fprintf(fp,"上网人数统计\n");
      	ncUtlPrintCsv(fp, 2,
      			UT_TYPE_STRING,"单位",
                UT_TYPE_STRING,"上网人数"

      		);

      }
  }

    
 printf("sql=%s\n",sql);	
      
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",(char *)ncLang("0746用户查询"),(char *)ncLang("0171查询数据出错"));
       return 0;	
	 }
	   memset(caCname,0,sizeof(caCname));
	   lCount1=0;
	   lWay=0;
	   lGroupid=0;
	 iret = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lWay,
	                             UT_TYPE_STRING,120,caCname,
	                             UT_TYPE_LONG,4,&lCount1,
	                             UT_TYPE_ULONG,4,&lGroupid);
	                            
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
		   
		  
		   
		   utPltSetCvtHtml(1);
        utPltPutLoopVarF(psDbHead,"way",iNum,"%d",lWay);
			 utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",lNum);
       utPltPutLoopVarF(psDbHead,"count",iNum,"%d",lCount1);
       utPltPutLoopVarF(psDbHead,"did",iNum,"%d",lGroupid);
       utPltPutLoopVar(psDbHead,"cname",iNum,caCname);
			 utPltSetCvtHtml(0);
        if(strlen(caExport)>0)
        {
        	if(atol(caStatway)==1)
        	{
        		ncUtlPrintCsv(fp,2, UT_TYPE_STRING,caCname,
	                             UT_TYPE_LONG,lCount1);
        	}
        	else
        		ncUtlPrintCsv(fp,2,  UT_TYPE_STRING,caCname,
	                             UT_TYPE_LONG, lCount1);
        }


			 
	   memset(caCname,0,sizeof(caCname));
	   lCount1=0;
	   lWay=0;
	   lGroupid=0;
	 iret = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lWay,
	                             UT_TYPE_STRING,120,caCname,
	                             UT_TYPE_LONG,4,&lCount1,
	                             UT_TYPE_ULONG,4,&lGroupid);
	  }
		//utPltShowDb(psDbHead);
		utPltPutVarF(psDbHead,"TotRec","%d",iNum);
	  pasDbCloseCursor(psCur);
	  
	  	if(strlen(caExport)>0){
        fclose(fp);
  //      printf("pid is %d\n", getpid());
       
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
  //       printf("shuchulema \n");
        return 0;
   }
   printf("dada jiangyou  \n");
	 
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncm_dnsrstj_list.htm");
	  return 0;	
}


int ncmInitWebFun_Cxyh(utShmHead *psShmHead){
	 pasSetTcpFunName("ncm_comshops_list", ncm_comshops_list, 0);
	 pasSetTcpFunName("ncm_comfloor_list",ncm_comfloor_list, 0);
	 pasSetTcpFunName("ncm_webcxyh_list",ncm_webcxyh_list, 0);
	 pasSetTcpFunName("ncmCxyh_Save",ncmCxyh_Save, 0);
	 pasSetTcpFunName("ncmCxyh_upload",ncmCxyh_upload, 0);
	 pasSetTcpFunName("ncmShowsCxyh",ncmShowsCxyh, 0);
	 pasSetTcpFunName("ncmShowsCxyhGoods",ncmShowsCxyhGoods, 0);
	 pasSetTcpFunName("ncmShowsSpfl",ncmShowsSpfl, 0);
	 pasSetTcpFunName("ncmShowsSpflGoods",ncmShowsSpflGoods, 0);
	 pasSetTcpFunName("ncmShowsFloorBrand",ncmShowsFloorBrand, 0);
	 pasSetTcpFunName("ncmShowsGwdhGoods",ncmShowsGwdhGoods, 0);
	 pasSetTcpFunName("ncsTreeUser_corp",ncsTreeUser_corp, 0);
	 pasSetTcpFunName("ncm_dnsswrs_list",ncm_dnsswrs_list,0);
}

