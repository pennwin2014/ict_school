#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include "utoall.h"
#include "pasutl.h"
#include "ncportal.h"      
#include "ncportalweb.h"

#include "pasdb.h"


int ictInitShm(utShmHead *psShmHead)
{
	utShmFreeHash(psShmHead, ICT_USER_LOGIN_TSID);
    utShmHashInit(psShmHead, ICT_USER_LOGIN_TSID, 2000, 2000, sizeof(ictOnlineUser), 0, 8);
	return 0;
}

int ncmInitWeiXinMem(utShmHead *psShmHead)
{
   	ncmInitWeiXinMem2(psShmHead);
   	ncmInitShopType(psShmHead);
   	ncmInitQQClientInfo(psShmHead);
   	return 0;
}

//加载微信配置信息
int ncmInitWeiXinMem2(utShmHead *psShmHead)
{      
    int iReturn,i,iMaxWeixin;
    long lDid,lGid;
    char sqlbuf[1024]="";
    char name[64] = "", token[48] = "", account[32] = "", appid[32] = "", appsecret[64] = "", sendcont[32] = "", redurl[255] = "";
    char usinfo[8] = "", access_token[255] = "";
    long systime = 0, status = 0, tokentime = 0, authstat = 0; 
    pasDbCursor *psCur;
    unsigned long lGroupid,wid;
    ncmweixin *ncmweixin_s,ncmwx_s;
    
    iMaxWeixin = utComGetVar_ld(psShmHead,"ncmweixin",5000);    
    utShmFreeHash(psShmHead,NCM_LNK_WEIXIN);
    iReturn = utShmLHashInit(psShmHead,NCM_LNK_WEIXIN,iMaxWeixin,iMaxWeixin,sizeof(ncmweixin),0,16); 

    if(iReturn != 0) {
        return (-1);
    }
    sprintf(sqlbuf,"select wid,name,token,account,appid,appsecret,sendcont,redurl,usinfo,access_token,systime,tokentime,authstat,status from ncmweixin where status = 0 order by systime desc limit 0,%d  ",iMaxWeixin);
//    printf("sqlbuf = %s \n",sqlbuf);
    psCur=pasDbOpenSql(sqlbuf,0);
    if(psCur == NULL) {
    	  // free(ncmweixin_s);  // Delete By Liyunming
        pasSysLog(PAS_LOG_ERROR,"Load weixin info error %s",pasDbErrorMsg(NULL));
        return (-1);
    }
    i = 0;
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long),&wid,
  	                               UT_TYPE_STRING,sizeof(name) - 1,name,
  	                               UT_TYPE_STRING,sizeof(token) - 1,token,
  	                               UT_TYPE_STRING,sizeof(account) - 1,account,
  	                               UT_TYPE_STRING,sizeof(appid) - 1,appid,
  	                               UT_TYPE_STRING,sizeof(appsecret) - 1,appsecret,
  	                               UT_TYPE_STRING,sizeof(sendcont) - 1,sendcont,
  	                               UT_TYPE_STRING,sizeof(redurl) - 1,redurl,
  	                               UT_TYPE_STRING,sizeof(usinfo) - 1,usinfo,
  	                               UT_TYPE_STRING,sizeof(access_token) - 1,access_token,
  	                               UT_TYPE_LONG,  sizeof(long), &systime,
  	                               UT_TYPE_LONG,  sizeof(long), &tokentime,
  	                               UT_TYPE_LONG,  sizeof(long), &authstat,
  	                               UT_TYPE_LONG,  sizeof(long), &status);                   
   	while((iReturn == 0 || iReturn == 1405 )) 
   	{
//   		  ncmwx_s.wid = wid;
//        printf("account = %s \n",account);
   		  strcpy(ncmwx_s.account,account);
  		  ncmweixin_s = (ncmweixin *)utShmLHashLookA(psShmHead,NCM_LNK_WEIXIN,&ncmwx_s);
   		  if(ncmweixin_s){
	        ncmweixin_s->wid = wid;
	        strcpy(ncmweixin_s->name,name);
	        strcpy(ncmweixin_s->token,token);
	        strcpy(ncmweixin_s->account,account);
	        strcpy(ncmweixin_s->appid,appid);
	        strcpy(ncmweixin_s->appsecret,appsecret);
	        strcpy(ncmweixin_s->sendcont,sendcont);
	        strcpy(ncmweixin_s->redurl,redurl);
	        strcpy(ncmweixin_s->usinfo,usinfo);
	        strcpy(ncmweixin_s->access_token,access_token);
	        ncmweixin_s->systime = systime;
	        ncmweixin_s->authstat = authstat;
	        ncmweixin_s->tokentime = tokentime;
//	   	    printf("****account = %s token = %s\n",ncmweixin_s->account,ncmweixin_s->token);
   	    }
   	    
   	    i++;
   	    wid = 0;
   	    memset(name,0,sizeof(name));
   	    memset(token,0,sizeof(token));
   	    memset(account,0,sizeof(account));
   	    memset(appid,0,sizeof(appid));
   	    memset(appsecret,0,sizeof(appsecret));
   	    memset(sendcont,0,sizeof(sendcont));
   	    memset(redurl,0,sizeof(redurl));
   	    memset(usinfo,0,sizeof(usinfo));
   	    memset(access_token,0,sizeof(access_token));
   	    systime = 0;
   	    tokentime = 0;
   	    authstat = 0;
   	    status = 0;
        iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long),&wid,
		  	                               UT_TYPE_STRING,sizeof(name) - 1,name,
		  	                               UT_TYPE_STRING,sizeof(token) - 1,token,
		  	                               UT_TYPE_STRING,sizeof(account) - 1,account,
		  	                               UT_TYPE_STRING,sizeof(appid) - 1,appid,
		  	                               UT_TYPE_STRING,sizeof(appsecret) - 1,appsecret,
		  	                               UT_TYPE_STRING,sizeof(sendcont) - 1,sendcont,
		  	                               UT_TYPE_STRING,sizeof(redurl) - 1,redurl,
		  	                               UT_TYPE_STRING,sizeof(usinfo) - 1,usinfo,
		  	                               UT_TYPE_STRING,sizeof(access_token) - 1,access_token,
		  	                               UT_TYPE_LONG,  sizeof(long), &systime,
		  	                               UT_TYPE_LONG,  sizeof(long), &tokentime,
		  	                               UT_TYPE_LONG,  sizeof(long), &authstat,
		  	                               UT_TYPE_LONG,  sizeof(long), &status);  
    }
    pasDbCloseCursor(psCur);
    
    return 0;
}
//加载商店类别对照表
int ncmInitShopType(utShmHead *psShmHead)
{      
    int iReturn,i,iMaxShoptype;
    long lDid,lGid;
    char sqlbuf[1024]="";
    
    long shopid = 0, sid = 0, sgroupid = 0, id = 0; 
    pasDbCursor *psCur;
    unsigned long lGroupid,wid;
    ncmshoptype *ncmshoptype_s,ncmsh_s;
    
    iMaxShoptype = utComGetVar_ld(psShmHead,"ncmshoptype",5000);    
    utShmFreeHash(psShmHead,NCM_LNK_SHOPTYPE);
    iReturn = utShmLHashInit(psShmHead,NCM_LNK_SHOPTYPE,iMaxShoptype,iMaxShoptype,sizeof(ncmshoptype),0,4); 

    if(iReturn != 0) {
        return (-1);
    }
    sprintf(sqlbuf,"select id,shopid,sgroupid,sid from ncmshoptype where flags = 0 order by shopid desc limit 0,%d  ",iMaxShoptype);
//    printf("sqlbuf = %s \n",sqlbuf);
    psCur=pasDbOpenSql(sqlbuf,0);
    if(psCur == NULL) {
    	  free(ncmshoptype_s);
        pasSysLog(PAS_LOG_ERROR,"Load weixin info error %s",pasDbErrorMsg(NULL));
        return (-1);
    }
    i = 0;
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long), &id,
                                   UT_TYPE_LONG,  sizeof(long), &shopid,
  	                               UT_TYPE_LONG,  sizeof(long), &sgroupid,
  	                               UT_TYPE_LONG,  sizeof(long), &sid);                   
   	while((iReturn == 0 || iReturn == 1405 )) 
   	{
   		  ncmsh_s.id = id;
  		  ncmshoptype_s = (ncmshoptype *)utShmLHashLookA(psShmHead,NCM_LNK_SHOPTYPE,&ncmsh_s);
   		  if(ncmshoptype_s){
	        ncmshoptype_s->shopid = shopid;
	        ncmshoptype_s->sgroupid = sgroupid;
	        ncmshoptype_s->sid = sid;
//	   	    printf("****shopid = %d sgroupid = %d\n",ncmshoptype_s->shopid,ncmshoptype_s->sgroupid);
   	    }
   	    
   	    i++;
   	    shopid = 0;
   	    sgroupid = 0;
   	    sid = 0;
   	    id = 0;
        iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long), &id,
		                                   UT_TYPE_LONG,  sizeof(long), &shopid,
		  	                               UT_TYPE_LONG,  sizeof(long), &sgroupid,
		  	                               UT_TYPE_LONG,  sizeof(long), &sid);                   
   
    }
    pasDbCloseCursor(psCur);
    
    return 0;
}

//获取商店对应所有类别
ncmshoptype * ncmgetShopgroupidByShopid(utShmHead *psShmHead,int shopid,int *iSum)
{
   ncmshoptype *stmp,*ps,*shoptype;
   pasMemRecordHead *allstruct;
   int i = 0, j = 0, k = 0;
   allstruct = (pasMemRecordHead *)utShmLHashGetAllRecord(psShmHead,NCM_LNK_SHOPTYPE,"",NULL,NULL);
	 if(allstruct){
	 	     shoptype = (ncmshoptype *)malloc(sizeof(ncmshoptype)*allstruct->lSum+1);
//	 	     printf("lSum = %d shopid = %d\n",allstruct->lSum,shopid);
				 for(i=0; i<allstruct->lSum; i++){
				 		ps = (ncmshoptype *)allstruct->ps[i];
				 		if(shopid == ps->shopid){
				 			  if (j == 0)
				 			  {
				 			  	 shoptype[j].shopid = ps->shopid;
				      	   shoptype[j].sgroupid = ps->sgroupid; 
				      	   shoptype[j].sid = ps->sid; 
			      	     j++;
//			      	     printf(" 1  shopid = %d sgroupid = %d sid = %d \n",ps->shopid,ps->sgroupid,ps->sid);
				 			  }	
				 			  else if (j > 0 && ps->sgroupid != shoptype[j-1].sgroupid)
				 			  {	
				           shoptype[j].shopid = ps->shopid;
				      	   shoptype[j].sgroupid = ps->sgroupid; 
				      	   shoptype[j].sid = ps->sid; 
//				      	   printf(" 2 shopid = %d sgroupid = %d sgroupid2 = %d sid = %d \n",ps->shopid,shoptype[j].sgroupid,shoptype[j-1].sgroupid,ps->sid);
			      	    
			      	     j++;
			      	  }
			      	  else
			      	  {
			      	  }	
				 		}
				 }		
	 } 
	 
	 *iSum = j;
//	 printf("** iSum = %d \n",*iSum);
	 return shoptype;
}
//获取类别是否在商店内
int ncmisExistTypeOfShop(utShmHead *psShmHead,int shopid, int sgroupid)
{
   ncmshoptype *stmp,*ps,*shoptype;
   pasMemRecordHead *allstruct;
   int i = 0, status = 0;
   allstruct = (pasMemRecordHead *)utShmLHashGetAllRecord(psShmHead,NCM_LNK_SHOPTYPE,"",NULL,NULL);
	 if(allstruct){
	 	     shoptype = (ncmshoptype *)malloc(sizeof(ncmshoptype)*allstruct->lSum+1);
				 for(i=0; i<allstruct->lSum; i++){
				 		ps = (ncmshoptype *)allstruct->ps[i];
				 		if(shopid == ps->shopid){
			          if (ps->sgroupid == sgroupid)
			          {
			             status = 1;	
			          }	
				 		}
				 }		
	 } 	
	 return status;
}
//初始化微信accesstoken结构，防过期    5.8日后不再使用
int ncmInitWXAccessToken(utShmHead *psShmHead)
{      
    int iReturn,i,iMaxWx;
    long lDid,lGid;
    char sqlbuf[1024]="";
    char caCompanyname[64] = "",caGroupcode[12] = "";
    unsigned long lcomid,lpid;
    ncwxaccesstoken *wxtoken;
    
    iMaxWx = utComGetVar_ld(psShmHead,"ncmweixin",5000);    
    utShmFreeHash(psShmHead,NCM_LNK_WXATOKEN);
    iReturn = utShmLHashInit(psShmHead,NCM_LNK_WXATOKEN,iMaxWx,iMaxWx,sizeof(ncwxaccesstoken),0,4); 
    if(iReturn != 0) {
        return (-1);
    }
    strcpy(caCompanyname,utComGetVar_sd(psShmHead,"CustomId","pronetway"));
    lcomid=utMd5Code(caCompanyname,strlen(caCompanyname),"ffff");
		if(lcomid>2000000000) lcomid=lcomid-2000000000;       
		if(lcomid>2000000000) lcomid=lcomid-2000000000;
    wxtoken = (ncwxaccesstoken *)utShmLHashLookA(psShmHead,NCM_LNK_WXATOKEN,&lcomid);
   	if(wxtoken){
   		    wxtoken->wid = lcomid;
	        wxtoken->systime = 0;
	        strcpy(wxtoken->access_token,"");
//	   	    printf("****groupid = %d groupname = %s\n",psGroup->lGroupid,psGroup->caGroupname);
   	}
   	 
    
    return 0;
}
//加载QQ服务器通信信息
int ncmInitQQClientInfo(utShmHead *psShmHead)
{      
    int iReturn,i,iMaxqqClients;
    char sqlbuf[1024]="";
    
    long qid = 0; 
    char pname[64] = "";
    pasDbCursor *psCur;
    unsigned long lGroupid,wid;
    
    ncmqqinfo *ncmqqinfo_s,ncmqq_s;
    
    iMaxqqClients = utComGetVar_ld(psShmHead,"ncmqq",5000);    
    utShmFreeHash(psShmHead,NCM_LNK_QQ);
    iReturn = utShmLHashInit(psShmHead,NCM_LNK_QQ,iMaxqqClients,iMaxqqClients,sizeof(ncmqqinfo),0,4); 

    if(iReturn != 0) {
        return (-1);
    }
    sprintf(sqlbuf,"select qid,pname from ncmqq where status = 0 order by systime desc limit 0,%d  ",iMaxqqClients);
//    printf("sqlbuf = %s \n",sqlbuf);
    psCur=pasDbOpenSql(sqlbuf,0);
    if(psCur == NULL) {
    	  free(ncmqqinfo_s);
        pasSysLog(PAS_LOG_ERROR,"Load QQ info error %s",pasDbErrorMsg(NULL));
        return (-1);
    }
    i = 0;
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,   sizeof(long), &qid,
                                   UT_TYPE_STRING, sizeof(pname) - 1, pname);                   
   	while((iReturn == 0 || iReturn == 1405 )) 
   	{
   		  ncmqq_s.qid = qid;
  		  ncmqqinfo_s = (ncmqqinfo *)utShmLHashLookA(psShmHead,NCM_LNK_QQ,&ncmqq_s);
   		  if(ncmqqinfo_s){
	        ncmqqinfo_s->qid = qid;
	        strcpy(ncmqqinfo_s->pname, pname);
//	   	    printf("****qid = %d pname = %s\n",ncmqqinfo_s->qid,ncmqqinfo_s->pname);
   	    }
   	    
   	    i++;
   	    qid = 0;
   	    memset(pname,0,sizeof(pname));
        iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,   sizeof(long), &qid,
		                                   UT_TYPE_STRING, sizeof(pname) - 1, pname);                
   
    }
    pasDbCloseCursor(psCur);
    
    return 0;
}

//获取模板id
int ncmGetPlateidByTsid(utShmHead *psShmHead,long long tsid)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    int iCount = 0, iReturn, lcid = 0;
    ncPortalUser    *psUser;
    ncPortalOnline  *psOnline;
    long lBase;
    char opt[256] = "";
    char *p = NULL;
    long long ltsid = 0;
    unsigned int lplateid = 0;
    char plateid[64] = "";
    pHash =  (unsigned char *)utShmHashHead(psShmHead,NCSRV_LNK_ONLINE);
    
    if(pHash == NULL) 
    {
        return (-1);
    }
    lBase = utShmGetBaseAddr(psShmHead);
    psOnline = (ncPortalOnline *)pasHashFirst(pHash,&sHashInfo);
    while(psOnline) 
    {
        psUser = (ncPortalUser *) ( (char *)psOnline->psUser + lBase);

//      printf("mytsid = %llu   tsid = %llu\n",psOnline->lTsid,tsid);
       if(psOnline->lTsid == tsid)
       {
 //      		  printf("platename = %s\n",psOnline->platename);
       		  strcpy(plateid,psOnline->platename);
       }	   
        psOnline = (ncPortalOnline *)pasHashNextS(&sHashInfo);
    }
    if(strlen(plateid)==0) strcpy(plateid,"genal");
    	
    lplateid=utMd5Code(plateid,strlen(plateid),"ffff");
    if(lplateid>2000000000) lplateid=lplateid-2000000000;
    if(lplateid>2000000000) lplateid=lplateid-2000000000;
printf("plateid=%s,lplateid=%lu\n",plateid,lplateid);    
    return lplateid;
}

//获取Mac
char *ncmGetMacByTsid(utShmHead *psShmHead,long long tsid)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    int iCount = 0, iReturn, lcid = 0;
    ncPortalUser    *psUser;
    ncPortalOnline  *psOnline;
    long lBase;
    char opt[256] = "";
    char *p = NULL;
    long long ltsid = 0;
    unsigned int lplateid = 0;
    static char myMac[64] = "";
    pHash =  (unsigned char *)utShmHashHead(psShmHead,NCSRV_LNK_ONLINE);
    
    if(pHash == NULL) 
    {
        return "";
    }
    lBase = utShmGetBaseAddr(psShmHead);
    psOnline = (ncPortalOnline *)pasHashFirst(pHash,&sHashInfo);
    while(psOnline) 
    {
        psUser = (ncPortalUser *) ( (char *)psOnline->psUser + lBase);

//       printf("mytsid = %llu   tsid = %llu\n",psOnline->lTsid,tsid);
       if(psOnline->lTsid == tsid)
       {
//       		  printf("myMac = %s\n",pasCvtMac(psOnline->caMac));
       		  strcpy(myMac,pasCvtMac(psOnline->caMac));
       }	   
        psOnline = (ncPortalOnline *)pasHashNextS(&sHashInfo);
    }
   
    
    return (char *)myMac;
}

// 根据groupid获取显示名
//获取Mac
char *ncmGetDispNameByGid(utShmHead *psShmHead,uint4 lGroupid)
{
    static char dispname[128] = "";
    ncPortalGroup *pGroup;
//    ncPortalGroup *ncSrvGetGroupInfoById(utShmHead *psShmHead,uint4 lGroupid);
    
    pGroup = ncSrvGetGroupInfoById(psShmHead,lGroupid);
    
    if (pGroup)
	    return (char *)pGroup->caDisp;
    else 
    	return "";
}


int Ncm_Mem_Api_SetFunName(utShmHead *psShmHead)
{
	 int iReturn;
	 iReturn = pasSetTcpFunName("ncmGetMacByTsid",ncmGetMacByTsid,0);                                       //根据tsid获取mac
	 iReturn = pasSetTcpFunName("ncmGetDispNameByGid",ncmGetDispNameByGid,0);                                       //根据tsid获取mac
   return 0;	
}
