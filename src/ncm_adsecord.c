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
#include <pcap.h>
#include <errno.h>
#include <iconv.h>
#include <dirent.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasutl.h"
#include "pasdb.h"
#include "ncportal.h"      
#include "ncportalweb.h"

#define FILEPATH "/home/ncmysql/ncsrv/bin"
#define UPLOADFILEPATH "/home/ncmysql/ncsrv/upload"
#define UPLOADIMGPATH "/home/ncmysql/ncsrv/upload"
#define SHOPIMGPATH "/home/ncmysql/ncsrv/html"

//第二级页面管理
int ncmAdSecord(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caTemp_d[128];
	  long lSid,lTid;
	  char caPre[36];
	  char caDispname[68],caMac[24];
	  long lStatus;
	  long iNum;
	   long lktype,lcaseflags,ltid;
	   char caDescr[64],caBh[32],caBm[32],caCh[32],caIdno[32];
	   char caDelall[16],caJoin[16];
	   unsigned long lShopplateid,lLevel;
	   char caUrl[128],caTitle[128];
	 char caImgpath[128],caShopplateid[16],caPlateid[16],caLevel[16];
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caCode[16],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);

	  iReturn = utMsgGetSomeNVar(psMsgHead,12,
	                 "shopplateid",     UT_TYPE_STRING, 15, caShopplateid,
	                 "imgpath",         UT_TYPE_STRING,127,caImgpath,
	                 "level",           UT_TYPE_STRING,15,caLevel,
	                 "plateid",              UT_TYPE_STRING,10,caPlateid,
	                 "title",           UT_TYPE_STRING,127,caTitle,
	                 "url",             UT_TYPE_STRING,127,caUrl,
			             "limit",        UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "start",UT_TYPE_STRING,10,caStart);
			            
	 		if(strlen(caTitle)>0){
				  pasCvtGBK(2,caTitle,caTemp,31);
		     	strcpy(caTitle,caTemp);				  	
		  }
     	if(strlen(caUrl)>0){
				  pasCvtGBK(2,caUrl,caTemp,31);
		     	strcpy(caUrl,caTemp);				  	
		  }
     
	
	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  
	  if(strcmp(caUpdate,"update")==0){
	  	if(strlen(caPlateid)>0&&strlen(caLevel)>0&&strlen(caImgpath)>0){
	  		sprintf(caTemp,"select count(*) from portaladsecord where shopplateid=%s  and level=%s ",caPlateid,caLevel);
	  		lCount=0;
	  		printf("caTemp=%s\n",caTemp);
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
        if(lCount==0){
        	
	  			 pasDbExecSqlF("insert into portaladsecord(shopplateid,level,imgpath,title,url) \
	       		values(%s,%s,'%s','%s','%s')",caPlateid,caLevel,caImgpath,caTitle,caUrl);
	  		}
	  		else{
	  			pasDbExecSqlF("update portaladsecord set imgpath='%s',url='%s',title='%s' where shopplateid=%s and level=%s ",caImgpath,caUrl,caTitle,caPlateid,caLevel);
	  	}	  
	   }
	  }
	  else if(strcmp(caUpdate,"del")==0){
	  	pasDbExecSqlF("delete from  portaladsecord  where shopplateid=%s and level=%s",caPlateid,caLevel);
	  	printf("delete from  portaladsecord  where shopplateid=%s and level=%s\n",caPlateid,caLevel);
	  	if(strlen(caImgpath)>0){
	  		sprintf(caTemp,"%s%s",SHOPIMGPATH,caImgpath);
	  		printf("caTemp=%s\n",caTemp);
	  		remove(caTemp);
	  	}
	  }
	 
	  
	  
	  sprintf(caTemp,"select count(*) from portaladsecord where 1=1 ");
    if(strlen(caPlateid)>0){
    	sprintf(caTemp+strlen(caTemp)," and shopplateid=%s ",caPlateid);
    }
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select shopplateid,level,imgpath,title,url from portaladsecord where 1=1");
	    if(strlen(caPlateid)>0){
	    	sprintf(caTemp+strlen(caTemp)," and shopplateid=%s ",caPlateid);
	    }
    
    	sprintf(caTemp+strlen(caTemp)," order by level  limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","数据统计出错","查询数据出错");
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while(iReturn == 0 ||iReturn==1405 ) {

   lShopplateid=0;
   lLevel=0;
   memset(caImgpath,0,sizeof(caImgpath));
   memset(caUrl,0,sizeof(caUrl));
   memset(caTitle,0,sizeof(caTitle));
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lShopplateid,
                                       UT_TYPE_LONG,4,&lLevel,
                                       UT_TYPE_STRING,127,caImgpath,
                                       UT_TYPE_STRING,127,caTitle,
                                       UT_TYPE_STRING,127,caUrl);
      	                              
      	                    
      	                                  	                              
      	                                                                 	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"plateid",iNum,"%lu",lShopplateid);
        utPltPutLoopVarF(psDbHead,"level",iNum,"%d",lLevel);
        utPltPutLoopVar(psDbHead,"imgpath",iNum,caImgpath);
        utPltPutLoopVar(psDbHead,"title",iNum,caTitle);
        utPltPutLoopVar(psDbHead,"url",iNum,caUrl);
       

			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_adsecord.htm");
	  return 0;
}


int ncm_adsecord_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  
	  char caImgPath[128]="";
	  char caDispImgPath[128]="";
	  char imgname[128]="";
	  char imp_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
	  char *p=NULL;
	  char *q=NULL;
    
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

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
  	sprintf(caImgPath,"%s/images/ShopImages/indeximages", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,555)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);

	  //utMsgGetSomeNVar(psMsgHead,1,"fileField", UT_TYPE_STRING, 255, imp_file);
	  utMsgGetSomeNVar(psMsgHead,1,"adfile", UT_TYPE_STRING, sizeof(imp_file)-1, imp_file);

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
		  	//printf("temp_file = %s\n", temp_file);
		  	strcpy(temp3_file, temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
				}else{
					  q = strtok(temp3_file, ".");
					  //if(q)printf("%s\n", q);
					  q = strtok(NULL, ".");
					  //if(q)printf("%s\n", q);
					  srand((unsigned int)time(0));
						sprintf(imgname, "index_%lu_%lu.%s", time(0), rand()%100, q);
						//printf("imgname = %s\n", imgname);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
printf("temp2_file = %s\n", temp2_file);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/indeximages/");
				    strcat(caDispImgPath, imgname);
				}
		printf("caDispImgPath=%s\n",caDispImgPath);
	  utPltPutVar(psDbHead,"filepath",caDispImgPath);
	  utPltPutVar(psDbHead,"title","true");
		}
		else{
			utPltPutVar(psDbHead,"title","false");
		}
	
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc_adfile.htm");
    return 0;
}
//显示第二页面的广告
int ncmSecordDispAd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{

	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caTemp[2048];
    char caTsid[20],caLevel[16];
    char caImgpath[128];
    char caUrl[128],caTitle[128];
	  utMsgPrintMsg(psMsgHead);
unsigned int lplateid;
	  iReturn = utMsgGetSomeNVar(psMsgHead,2,
	                 "tsid",     UT_TYPE_STRING, 18, caTsid,
	                 "level",    UT_TYPE_STRING,10,caLevel);
    if(strlen(caTsid)>0&&atol(caTsid)!=0){
	   lplateid = ncmGetPlateidByTsid(psShmHead,atoll(caTsid));
	 }
	 else{
		  lplateid=0;
	 }
	  psDbHead = utPltInitDb(); 
	  sprintf(caTemp,"select imgpath,url,title from portaladsecord where shopplateid=%lu and level=%s ",lplateid,caLevel);
	  printf("caTemp=%s\n",caTemp);
	  memset(caImgpath,0,sizeof(caImgpath));
	  memset(caUrl,0,sizeof(caUrl));
	  memset(caTitle,0,sizeof(caTitle));
	  pasDbOneRecord(caTemp,0,UT_TYPE_STRING,127,caImgpath,
	                          UT_TYPE_STRING,127,caUrl,
	                          UT_TYPE_STRING,127,caTitle);
    utPltPutVar(psDbHead,"imgpath",caImgpath);                  
    utPltPutVar(psDbHead,"url",caUrl);
    utPltPutVar(psDbHead,"title",caTitle);
    if(strlen(caUrl)>0){
    pasCharsetCvt(PAS_CCODE_GB,PAS_CCODE_UTF8,caUrl,caTemp,127);
    utPltPutVar(psDbHead,"url_utf8",caTemp); 
   }
   if(strlen(caTitle)>0){
  	 pasCharsetCvt(PAS_CCODE_GB,PAS_CCODE_UTF8,caTitle,caTemp,127); 
     utPltPutVar(psDbHead,"title_utf8",caTemp); 
   }
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_secordad_detail.htm");
	  return 0;
}
