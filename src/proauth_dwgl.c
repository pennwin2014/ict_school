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
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include "portalweb.h"
#include "ncportal.h"


//根据场所编码获取单位组结构
authClient *authGetClientByCode(utShmHead *psShmHead,char *pCode){
	 authClient *psClient;	
	 psClient=(authClient *)utShmHashLook(psShmHead,PROAUTH_LNK_CLIENT,pCode);
	 if(psClient){
	 	return psClient;
	 }
	 else{
	 	return NULL;
	}
	

}


//根据ID获取单位
char *authGetClientName(utShmHead *psShmHead,unsigned long lId){
	  authClient *psClient;
	  authClientId2Code *psCode;
	 static char sKey[64];
   char caServicecode[32];
   memset(caServicecode,0,sizeof(caServicecode));

	 psCode=(authClientId2Code *)utShmHashLook(psShmHead,PROAUTH_LNK_CLIENTIDCODE,&lId);
	 if(psCode){

	 	strcpy(caServicecode,psCode->code);
	 }
	 else{
	 	strcpy(sKey,"");
	 	return &sKey[0];
	}

	 psClient=(authClient *)utShmHashLook(psShmHead,PROAUTH_LNK_CLIENT,caServicecode);
	 if(psClient){

	 	strcpy(sKey,psClient->dispname);
	 }
	 else{
	 	strcpy(sKey,"");
	}
	
	return &sKey[0];
}

char *authGetGroupNameById(utShmHead *psShmHead,unsigned long lGroupid){
	 authGroup *psClient;
	 static char sGroup[64];

	 psClient=(authClient *)utShmHashLook(psShmHead,PROAUTH_LNK_GROUP,&lGroupid);
	 if(psClient){
	 	strcpy(sGroup,psClient->groupname);
	 }
	 else{
	 	strcpy(sGroup,"");
	}
	return &sGroup[0];
}

char *authGetClientLbBycode(utShmHead *psShmHead,char *caCode){
	  authFac *psClient;
	 static char sKey[64];
	 psClient=(authClient *)utShmHashLook(psShmHead,PROAUTH_LNK_LB,caCode);
	 if(psClient){
	 	strcpy(sKey,psClient->dispname);
	 }
	 else{
	 	strcpy(sKey,"");
	}
	return &sKey[0];
}

//加载单位信息
int ProAuthLoadClientsInfo(utShmHead *psShmHead)
{
    
    unsigned long   lMaxClient;
    int             iReturn,iRet;
    char            *p;
    unsigned long lGroupid;

    pasDbCursor *psCur;
    authClient s,*psData;
    authClientId2Code sc;
    lMaxClient=utComGetVar_ld(psShmHead,"MaxClients",15000);
    
    utShmFreeHash(psShmHead,PROAUTH_LNK_CLIENTIDCODE);  /* 先释放  */
   
    iReturn = utShmHashInit(psShmHead,PROAUTH_LNK_CLIENTIDCODE,lMaxClient,lMaxClient,sizeof(authClientId2Code),0,4);

    utShmFreeHash(psShmHead,PROAUTH_LNK_CLIENT);  /* 先释放  */
   
    iReturn = utShmHashInit(psShmHead,PROAUTH_LNK_CLIENT,lMaxClient,lMaxClient,sizeof(authClient),0,30);
    if(iReturn != 0) {
        printf("Share Memory alloc Error at ProAuthLoadClientsInfo\n");
        return (-1);
    }
    /* 将单位资料导入到内存中  */
    psCur = pasDbOpenSqlF("select groupid,groupname,dispname,gid,gtype,longitude,latitude from ncsrvgroup ");
    
    if(psCur == NULL) {
  
        return (-1);
    }
    
    memset(&s,0,sizeof(s));
    memset(&sc,0,sizeof(sc));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&s.userid,
                                    UT_TYPE_STRING,30,s.servicecode,
                                    UT_TYPE_STRING,127,s.dispname,
                                    UT_TYPE_ULONG,4,&s.groupid,
                                    UT_TYPE_LONG,4,&s.gtype,
                                    UT_TYPE_FLOAT,sizeof(double),&s.longitude,
                                    UT_TYPE_FLOAT,sizeof(double),&s.latitude);
                                   
                                  
     char caServicecode[20];                             
    while(iReturn == 0||iReturn==1405) {
// #printf(" LoadUser:%lu %s \n",s.compid,s.compname);
    sc.id=s.userid;
    strcpy(sc.code,s.servicecode);
    utShmHashAdd(psShmHead,PROAUTH_LNK_CLIENTIDCODE,&sc,1); 

       iRet = utShmHashAdd(psShmHead,PROAUTH_LNK_CLIENT,&s,1);
       if(iRet < 0) {
   //   printf("fffffffffffffffff\n");    
           pasDbCloseCursor(psCur);
           return (-1);
       }
         memset(&s,0,sizeof(s));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&s.userid,
                                    UT_TYPE_STRING,30,s.servicecode,
                                    UT_TYPE_STRING,127,s.dispname,
                                    UT_TYPE_ULONG,4,&s.groupid,
                                    UT_TYPE_LONG,4,&s.gtype,
                                    UT_TYPE_FLOAT,sizeof(double),&s.longitude,
                                    UT_TYPE_FLOAT,sizeof(double),&s.latitude);
    }
    pasDbCloseCursor(psCur);
    return 0;
}

//加载单位组信息
int ProAuthLoadGroupInfo(utShmHead *psShmHead)
{

    unsigned long   lMaxClient;
    int             iReturn,iRet;
    char            *p;
    unsigned long lGroupid;

    pasDbCursor *psCur;
    authGroup s,*psData;
    lMaxClient=utComGetVar_ld(psShmHead,"MaxGroups",300);
    


    utShmFreeHash(psShmHead,PROAUTH_LNK_GROUP);  /* 先释放  */
   
    iReturn = utShmHashInit(psShmHead,PROAUTH_LNK_GROUP,lMaxClient,lMaxClient,sizeof(authGroup),0,4);
    if(iReturn != 0) {
        printf("Share Memory alloc Error at ProAuthLoadGroupsInfo\n");
        return (-1);
    }
    /* 将单位组资料导入到内存中  */
    psCur = pasDbOpenSqlF("select groupid,groupname,pid from ncsgroup ");
    if(psCur == NULL) {
  
        return (-1);
    }
    
    memset(&s,0,sizeof(s));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&s.groupid,
                                    UT_TYPE_STRING,63,s.groupname,
                                    UT_TYPE_ULONG,4,&s.pid);
                                   
                                  
          
    while(iReturn == 0||iReturn==1405) {
// #printf(" LoadUser:%lu %s \n",s.compid,s.compname);

      
        
     //   printf("key=%s\n",authGetClientKey(psShmHead,caServicecode));
       iRet = utShmHashAdd(psShmHead,PROAUTH_LNK_GROUP,&s,1);
    
       if(iRet < 0) {
   
           pasDbCloseCursor(psCur);
           return (-1);
       }
    memset(&s,0,sizeof(s));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&s.groupid,
                                    UT_TYPE_STRING,63,s.groupname,
                                    UT_TYPE_ULONG,4,&s.pid);
    }
    pasDbCloseCursor(psCur);
    return 0;
}

//加载单位类别
int ProAuthLoadLbInfo(utShmHead *psShmHead)
{
    
    unsigned long   lMaxClient;
    int             iReturn,iRet;
    char            *p;
    unsigned long lGroupid;

    pasDbCursor *psCur;
    authFac s,*psData;
    lMaxClient=utComGetVar_ld(psShmHead,"MaxGroups",300);
    


    utShmFreeHash(psShmHead,PROAUTH_LNK_LB);  /* 先释放  */
   
    iReturn = utShmHashInit(psShmHead,PROAUTH_LNK_LB,lMaxClient,lMaxClient,sizeof(authFac),0,12);
    if(iReturn != 0) {
        printf("Share Memory alloc Error at ProAuthLoadLBInfo\n");
        return (-1);
    }
    /* 将单位组资料导入到内存中  */
    psCur = pasDbOpenSqlF("select code,name from ncsuser_lb ");
    if(psCur == NULL) {  
        return (-1);
    }
    
    memset(&s,0,sizeof(s));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,&s.code,
                                    UT_TYPE_STRING,63,s.dispname);
                                   
                                                                    
    while(iReturn == 0||iReturn==1405) {
// #printf(" LoadUser:%lu %s \n",s.compid,s.compname);
       
     //   printf("key=%s\n",authGetClientKey(psShmHead,caServicecode));
       iRet = utShmHashAdd(psShmHead,PROAUTH_LNK_LB,&s,1);
       if(iRet < 0) {
   //   printf("fffffffffffffffff\n");    
           pasDbCloseCursor(psCur);
           return (-1);
       }
    memset(&s,0,sizeof(s));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,&s.code,
                                    UT_TYPE_STRING,63,s.dispname);
    }
    pasDbCloseCursor(psCur);
    
       
    return 0;
}


//显示单位组列表
int ncsWebClientClassList_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512],caCond[64],caCond0[64];
    char caTotPg[16],caCurPg[16],caSort[16],caSortby[6];
    char caGroupname[32];
    char caGroupid[16];

    char caTemp[2024];
   
    char caAll[1024];
   
  
    unsigned long lGroupid,lPid,lCurPg,lTotPg;
    int lCount;
    int i,iNum,lId;
    char caGroupcode[16];

    char caSmt[4048],caOrder[64];
    char caDel[16],caExport[16];
    utPltDbHead *psDbHead;
   	int iReturn;
   	long lGroupid0,lRowNum,lUseflags,lStartRec,lUseflags0,lTotRec;
   	char servicecode1[24],dispname1[68],address1[130],ceoname1[32],handphone1[34],contact1[34],telphone1[68],email1[130];
   	char ip1[20],servicestate1[4],jointype1[4],area1[72],areaman1[32],joincode1[8];
 	  pasDbCursor *psCur;
 	  char caUsername2[32];
 	  char caMsg[1024];
 	  char caOnlineflag[16];
 	  long lMonflag=0;
 	  unsigned long lCurtime,lTime,lRuntime;
   	unsigned long lCase;
   	char caYear[30];
   	char caPlate[128];
   	char caOpt[256];
   	char caPage[16],caLimit[16];
   	char caDir[12];
    FILE *fp;
    char pFile[256],caBuf[128];
    unsigned char *pHash;
    struct stData{
    	unsigned long id;
    	char caGroupname[36];
    	long lCount;
    };
    long lCount_d;
    struct stData *psData;
   char caTemp_d[256];
   char caSesid[1024];
   long lSumHash,js_flag;
   long lGroupid3;
   char caUpdate[12],caPid[64];
   char caGroups[1024];
   char caGroupname2[36],caGroupcode2[20],caGroupid2[20];
    memset(caAll,0,sizeof(caAll));
   strcpy(caGroups,getDsGroupid());
       
    memset(caOpt,0,sizeof(caOpt)); 
    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    if(iReturn==0){
 	    pasDbOneRecord("select opt from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);
    } 
   	
 	  dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);	  
	  iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    js_flag=0;
	
	  
	  
	 utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,13,
	                 "groupid",          UT_TYPE_STRING,10,caGroupid,
			             "groupid2",          UT_TYPE_STRING, 12, caGroupid2,  	              
   	                "sort",            UT_TYPE_STRING, 15, caSort,
   	                "dir",          UT_TYPE_STRING,8,  caDir,   	              
   	                "del",             UT_TYPE_STRING, 10,caDel,
   	                "export",          UT_TYPE_STRING,10,caExport,
   	                "groupname",       UT_TYPE_STRING,31,caGroupname2,
   	                "groupcode",       UT_TYPE_STRING,16,caGroupcode2,
   	                "page",            UT_TYPE_STRING,10,caPage,
   	                "limit",           UT_TYPE_STRING,10,caLimit,
   	                "selsid",UT_TYPE_STRING,1000,caSesid,
   	                "update",UT_TYPE_STRING,10,caUpdate,
   	                "pid",UT_TYPE_STRING,60,caPid);

    if(strlen(caGroupname2)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caGroupname2,caTemp,64));
    	strcpy(caGroupname,caTemp_d);
    }
    else{
    	strcpy(caGroupname,"");
    }
    if(strlen(caGroupcode2)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caGroupcode2,caTemp,15));
    	strcpy(caGroupcode,caTemp_d);
    }
    else{
    	strcpy(caGroupcode,"");
    }

      if(strlen(caPid)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caPid,caTemp,64));
    	strcpy(caPid,caTemp_d);
    	
    	sprintf(caTemp,"select groupid from ncsgroup where groupname='%s' ",caPid);
    	printf("caTemp=%s\n",caTemp);
    	lGroupid3=0;
    	pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lGroupid3);
    	if(lGroupid3>0){
    		sprintf(caPid,"%d",lGroupid3);
    	}
    }



  if(strcmp(caUpdate,"update")==0){
  	lPid=atol(caPid);
  	lGroupid=atol(caGroupid2);
  	if(lGroupid>0){
	  		if((lGroupid!=lPid)&&strlen(caGroupname)>0){
	  			pasDbExecSqlF("update ncsgroup set groupname='%s',pid=%d,groupcode='%s' where groupid=%d ",caGroupname,lPid,caGroupcode,lGroupid);
	  		}
  	}
  	else{
  		 lGroupid=utMd5Code(caGroupname,strlen(caGroupname),"ffff");
       if(lGroupid>2000000000) lGroupid=lGroupid-2000000000;
       
       if(lGroupid>2000000000) lGroupid=lGroupid-2000000000;
     
      sprintf(caTemp,"select count(*) from ncsgroup where groupid=%d ",lGroupid);
      lCount=0;
  	  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
  	  if(lCount>0){
  	  	pasDbExecSqlF("update ncsgroup set groupname='%s',pid=%d,groupcode='%s' where groupid=%d ",caGroupname,lPid,caGroupcode,lGroupid);
  	  }
  	  else{
  	  	pasDbExecSqlF("insert into ncsgroup(groupid,groupname,descr,pid,groupcode) values(%d,'%s','%s',%d,'%s')",lGroupid,caGroupname,caGroupname,lPid,caGroupcode);
  	  }
  	}
  	strcpy(caGroupcode2,"");
  	strcpy(caGroupname2,"");
  	strcpy(caGroupid2,"");
  }

    if(!utStrIsSpaces(caDel)) {  /*删除选中的项目*/
    	if(strlen(caSesid)>0){
    //		printf("delete from ncsuser where groupid in(%s)\n",caSesid);
    //		      iReturn = pasDbExecSqlF("delete from ncsuser where groupid in(%s)",caSesid);
    		     iReturn = pasDbExecSqlF("update ncsrvgroup set gid=0 where gid in(%s)",caSesid);
    		       iReturn = pasDbExecSqlF("delete from ncsgroup where groupid in(%s)",caSesid);
    	printf("delete from ncsgroup where groupid in(%s)\n",caSesid);	      
            	sprintf(caMsg,"delete from ncsgroup where groupid in(%s) ",caSesid);
            	if(iReturn==0){
            		ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",caMsg);
            	}
            	else{
            		ncsWriteSysLog(psMsgHead,caUsername2,"05","失败",caMsg);
            	}
    	}
		
    }

    lCurPg = atol(caPage);//当前页面
    if(lCurPg <= 0) lCurPg = 1 ;//如若当前页面小于0，置1
    

    	if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
  
    lStartRec = (lCurPg - 1) * lRowNum;   //当前页面的开始第一行的行数		
    
    	
    sprintf(caSmt,"select count(*) from ncsgroup where 1=1"); 
    
    if(strlen(caGroupname2)!=0){
    sprintf(caSmt+strlen(caSmt),"  and groupname like '%c%s%c' ",'%',caGroupname2,'%');
    }

    if(strlen(caGroupcode2)>0){
    	sprintf(caSmt+strlen(caSmt)," and groupcode=%s ",caGroupcode2);
    }
    if(strlen(caGroupid)>0){
   	  sprintf(caSmt+strlen(caSmt)," and groupid in (%s) ", getGroupid(atol(caGroupid)) );
    }
    if(strlen(caGroups)>0){
    	sprintf(caSmt+strlen(caSmt)," and groupid in (%s) ",caGroups);
    }

// printf("caSmt=%s\n",caSmt);   
    iReturn = pasDbOneRecord(caSmt,0,UT_TYPE_LONG,4,&lCount);
     
    
    if(iReturn != 0) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","用户查询","查询数据出错 ");
        return 0;
    } 
    
 //  lSumHash=lCount*1.3+1;
lSumHash=1000;
//printf("lSumHash=%d\n",lSumHash);   
     pHash = (unsigned char *)pasLHashInit(lSumHash,lSumHash,sizeof(struct stData),0,4);    
    if(pHash == NULL) {
         utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
         return (-1);
    }
   

     
    sprintf(caTemp,"select groupid,groupname from ncsgroup  ");
     psCur = pasDbOpenSql(caTemp,0);
     lGroupid=0;
     memset(caGroupname,0,sizeof(caGroupname));
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lGroupid,
      	                             UT_TYPE_STRING,32,caGroupname);
      	           
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {

    	 psData = (struct stData *)pasLHashLookA(pHash,&lGroupid);    	
       if(psData){       
       	strcpy(psData->caGroupname,caGroupname);	
      }
     	lGroupid=0;
     	memset(caGroupname,0,sizeof(caGroupname));
  		iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lGroupid,
      	                             UT_TYPE_STRING,32,caGroupname);	                              
    }    
    pasDbCloseCursor(psCur);  
    
    
    
    sprintf(caTemp,"select count(*),gid from ncsrvgroup group by gid ");
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur){
       lCount_d=0;
       lGroupid=0;
      	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lCount_d,
      	                             UT_TYPE_LONG,4,&lGroupid);
      	                            
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {

    	 psData = (struct stData *)pasLHashLookA(pHash,&lGroupid);    	
       if(psData){       
       	psData->lCount=lCount_d;	
      }
       lCount_d=0;
       lGroupid=0;
      	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lCount_d,
      	                             UT_TYPE_LONG,4,&lGroupid);                              
    }    
    pasDbCloseCursor(psCur);  
   }
    
    
    
    
    
   sprintf(caSmt,"select groupid,groupname,pid,groupcode from ncsgroup where 1=1");
    if(strlen(caGroupid)>0){
   	  sprintf(caSmt+strlen(caSmt)," and groupid in (%s) ", getGroupid(atol(caGroupid)) );
    }
    if(strlen(caGroups)>0){
    	sprintf(caSmt+strlen(caSmt)," and groupid in (%s) ",caGroups);
    }
    if(strlen(caGroupname2)!=0){
    sprintf(caSmt+strlen(caSmt),"  and groupname like '%c%s%c' ",'%',caGroupname2,'%');
    }

    if(strlen(caGroupcode2)>0){
    	sprintf(caSmt+strlen(caSmt)," and groupcode=%s ",caGroupcode2);
    }
    
    if(strlen(caSort)>0){
    	sprintf(caSmt+strlen(caSmt),"  order by pid limit %lu,%lu",lStartRec,lRowNum);
    }
    else{
    		sprintf(caSmt+strlen(caSmt),"  order by pid limit %lu,%lu",lStartRec,lRowNum);
    }
  printf("caSmt=%s\n",caSmt);
 
    psCur = pasDbOpenSql(caSmt,0);
    if(psCur==NULL) {  	  
        free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","网站访问查询","数据库出错");
        return 0;
    	}
    
   
    psDbHead = utPltInitDb();
    
    
      lGroupid=0;
      lPid=0;
      memset(caGroupcode,0,sizeof(caGroupcode));
       memset(caGroupname,0,sizeof(caGroupname));
   	   iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lGroupid,
   	                                  UT_TYPE_STRING,30,caGroupname,
   	                                  UT_TYPE_LONG,4,&lPid,
   	                                  UT_TYPE_STRING,10,caGroupcode);
   	iNum = 0;
   	
   	while(iReturn == 0||iReturn==1405 && iNum < lRowNum) {
        if(iNum>0){
        	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
        }
        iNum++;
        utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
        utPltPutLoopVarF(psDbHead,"groupid",iNum,"%d",lGroupid);
        utPltPutLoopVar(psDbHead,"groupcode",iNum,caGroupcode);
        utPltPutLoopVarF(psDbHead,"pid",iNum,"%d",lPid);

   	    psData = (struct stData *)pasLHashLook(pHash,&lPid);       
         	if(psData){
         		utPltPutLoopVar(psDbHead,"pname",iNum,psData->caGroupname);
      
         	}
         	
    	  else {
 
    	  	utPltPutLoopVar(psDbHead,"pname",iNum,"");
    	  	
    	  }
           psData = (struct stData *)pasLHashLook(pHash,&lGroupid);      
         	if(psData){
         		utPltPutLoopVarF(psDbHead,"count",iNum,"%d",psData->lCount);
         	}

      lGroupid=0;
      lPid=0;
      memset(caGroupcode,0,sizeof(caGroupcode));
       memset(caGroupname,0,sizeof(caGroupname));
   	   iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lGroupid,
   	                                  UT_TYPE_STRING,30,caGroupname,
   	                                  UT_TYPE_LONG,4,&lPid,
   	                                  UT_TYPE_STRING,10,caGroupcode);
    }
    pasDbCloseCursor(psCur);
    lTotPg = (lCount - 1) / lRowNum + 1;
    lTotRec = lCount;
    i = 0;
   
    utPltPutVar(psDbHead,"sort", caSort);
    utPltPutVar(psDbHead,"sortby", caSortby);

    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
    utPltPutVar(psDbHead,"groupid",caGroupid);
  
    if(pHash) free(pHash);
   

 //   	ncsWriteSysLog(psMsgHead,caUsername2,"06","成功","查看上网服务场所列表信息"); 
    	
  
     utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_dwfzgl_list.htm");
    

    return 0;
}




//显示场所类别代码维护
int ncsLbCode_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caCode[16],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);

	  iReturn = utMsgGetSomeNVar(psMsgHead,10,
	                 "dispname",     UT_TYPE_STRING, 31, caDispname,
			             "limit",        UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "code",UT_TYPE_STRING,10,caCode,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart);
			            
	  if(strlen(caDispname)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caDispname,caTemp,31)); 
	  	strcpy(caDispname,caTemp_d);
	  }
     
	
	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  
	  if(strcmp(caUpdate,"update")==0){
	  	if(strlen(caSid)==0){
	  		sprintf(caTemp,"select count(*) from ncsuser_lb where name='%s'  and code='%s' ",caDispname,caCode);
	  		lCount=0;
	  		printf("caTemp=%s\n",caTemp);
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
        if(lCount==0){
        	printf("insert into ncsuser_lb(name,code) values('%s','%s')\n",caDispname,caCode);
        
	  			 pasDbExecSqlF("insert into ncsuser_lb(name,code) \
	       		values('%s','%s')",caDispname,caCode);
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncsuser_lb where name='%s' and code='%s' and sid!=%s ",caDispname,caCode,caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
 
	  				pasDbExecSqlF("update ncsuser_lb set name='%s',code='%s' where sid=%s ",caDispname,caCode,caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("delete from  ncsuser_lb  where sid in(%s)",caSelsid);
	  }
	 
	  
	  
	  sprintf(caTemp,"select count(*) from ncsuser_lb where 1=1 ");

    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select sid,name,code from ncsuser_lb where 1=1");

    	sprintf(caTemp+strlen(caTemp)," order by sid desc  limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","数据统计出错","查询数据出错");
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {

    lSid=0;
    memset(caDispname,0,sizeof(caDispname));
    memset(caCode,0,sizeof(caCode));
    
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_STRING,63,caDispname,
      	                               UT_TYPE_STRING,10,caCode);
      	                    
      	                                  	                              
      	                                                                 	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
        utPltPutLoopVar(psDbHead,"code",iNum,caCode);

       
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        

			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_faccode_list.htm");
	  return 0;
}


//单位类别树AJAX通讯
int ncsTreeUser_lb(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[72],caGroupid[16];
   	char caTemp[4024];
   	char caName[32];
   	char caWhere[256];

   	char caCurPg[16],caTotRec[16];
   	char caPid[64];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
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
   	unsigned char *pHash;
   	char caCname[72];

    pasLHashInfo  sHashInfo;
    struct stData{
    	unsigned long id;
    	unsigned long count;
    };
    long lCount;
    struct stData *psData;
    char caCname1[72];
    char caSql[2048];
    char caCode[16];
    char caWireflag[20];
    char caServicecode[24];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
  //  utMsgPrintMsg(psMsgHead);
      strcpy(caGroups,getDsGroupid());
      strcpy(caFacs,getDsFacGroupid());
    memset(caCname,0,sizeof(caCname));
   	iReturn = utMsgGetSomeNVar(psMsgHead,3,
					"node",        UT_TYPE_STRING,10,caPid,
					"cname",       UT_TYPE_STRING,30,caCname1,
					"plate",       UT_TYPE_STRING,100,caPlate);
				
				
    if(strlen(caCname1)>0){
    	strcpy(caCname,pasCvtGBK(2,caCname1,caTemp,30));
    }
//    iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
    strcpy(caWhere,"\0");

  


    lRowNum = utComGetVar_ld(psShmHead,"GroupTreeRow",300);
  
    
 
    iNum = 0;
    
     psDbHead = utPltInitDb();
    if(strlen(caCname)==0&&strlen(caPid)==0){
		    lStartRec = (lCurPg - 1) * lRowNum;  
		    
		     sprintf(caTemp,"select name,code from ncsuser_lb where 1=1 ");
		   
		//    sprintf(caTemp+strlen(caTemp)," and pid=%d ",atol(caPid));
		    sprintf(caTemp+strlen(caTemp)," order by name limit %lu,%lu ",0,lRowNum);
	//	    printf("caTemp=%s\n",caTemp);
		    psCur=pasDbOpenSql(caTemp,0);
		  
		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   
		    
		     
		
		    memset(caGroupname,0,sizeof(caGroupname));
        memset(caCode,0,sizeof(caCode));
        
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,63,caGroupname,
		                                   UT_TYPE_STRING,9,caCode);
		                                
		                                  
		
		 
		    while(iReturn == 0) {
		        
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(1);  
		         utPltPutLoopVar(psDbHead,"groupname", iNum,utStrGetCharHan(caGroupname,48));
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%s",caCode);
		 //        psData = (struct stData *)pasLHashLook(pHash,&lGroupid);       
		 //        	if(psData){
		  //       		printf("id=%d,lcount=%d\n",psData->id,psData->count);
		          	utPltPutLoopVar(psDbHead,"leaf", iNum,"false");
		         		utPltPutLoopVar(psDbHead,"cls", iNum,"folder");
		         		utPltPutLoopVar(psDbHead,"dtype",iNum,"2");
		 //        	}
		  //        else{
		  //        	utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
		  //       		utPltPutLoopVar(psDbHead,"cls", iNum,"file");
		          	
		
		 //         }
		        
		
		        utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
		
		        utPltPutLoopVarF(psDbHead,"pid",iNum,"%s",caCode);
		
				    memset(caGroupname,0,sizeof(caGroupname));
		    memset(caGroupname,0,sizeof(caGroupname));
        memset(caCode,0,sizeof(caCode));
        
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,63,caGroupname,
		                                   UT_TYPE_STRING,9,caCode);
		    }
		    pasDbCloseCursor(psCur);
		//     free(pHash);
		    utPltPutVar(psDbHead,"rootname",utComGetVar_sd(psShmHead,"rootname","全部单位"));
    	
     }
    	sprintf(caTemp,"select dispname,groupid,gid,groupname from ncsrvgroup where 1=1 ");
    	if(strlen(caGroups)>0){
    		sprintf(caTemp+strlen(caTemp)," and gid in (%s) ",caGroups);
    	}
    
    
    	
    	
    if(strlen(caCname)==0){
    	   
		    	if(strlen(caPid)==0){
		    		  sprintf(caTemp+strlen(caTemp)," and gtype not in (select code from ncsuser_lb) ");
		    		  
		    	}
		    	else{
		    		  sprintf(caTemp+strlen(caTemp)," and gtype=%d ",atol(caPid));
		    		 		
		    	}
     }
     else{
     	   sprintf(caTemp+strlen(caTemp),"and (dispname like '%c%s%c' or groupname like '%c%s%c') ",'%',caCname,'%','%',caCname,'%');
     	  
     	 
     	
     }
      sprintf(caTemp+strlen(caTemp)," order by dispname limit %lu,%lu",0,lRowNum);
//      printf("caTemp=%s\n",caTemp);
       psCur = pasDbOpenSql(caTemp,0);
    	 if(psCur!=NULL){
    	    memset(caGroupname,0,sizeof(caGroupname));
			    lGroupid=0;
			    lPid=0;
			    memset(caServicecode,0,sizeof(caServicecode));
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,68,caGroupname,
			                                   UT_TYPE_LONG,4,&lGroupid,
			                                   UT_TYPE_LONG,4,&lPid,
			                                   UT_TYPE_STRING,16,caServicecode);
			
			    while(iReturn == 0) {
			        
			         if(iNum>0){
			         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
			         }
			         iNum++;
			         utPltSetCvtHtml(1);  
			         utPltPutLoopVar(psDbHead,"groupname", iNum,utStrGetCharHan(caGroupname,48));
			         utPltSetCvtHtml(0);  
			         utPltPutLoopVarF(psDbHead,"did",   iNum,"%ld",lGroupid);

			          	utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
			         		utPltPutLoopVar(psDbHead,"cls", iNum,"file");
			          	
			            utPltPutLoopVar(psDbHead,"dtype",iNum,"0");
			 
			        
			
			        utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
			
			        utPltPutLoopVarF(psDbHead,"pid",iNum,"%d",lPid);
			        utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
					    memset(caGroupname,0,sizeof(caGroupname));
					    lGroupid=0;
					    lLevel=0;
					    lPid=0;
					    memset(caServicecode,0,sizeof(caServicecode));
					    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,68,caGroupname,
					                                   UT_TYPE_LONG,4,&lGroupid,
					                                   UT_TYPE_LONG,4,&lPid,
					                                   UT_TYPE_STRING,16,caServicecode);
			    }
			    pasDbCloseCursor(psCur);
    	  }
    	
    	
 //utPltShowDb(psDbHead);
   if(strlen(caPlate)>0){
   	utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
  }
  else{
    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_user_tree.htm");
    	}
   

    return 0;
}
//单位树AJAX通讯
int ncsTreeUser_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[72],caGroupid[16];
   	char caTemp[4024];
   	char caName[32];
   	char caWhere[256];
    char caServicecode[20];
   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
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
   	unsigned char *pHash;
   	char caCname[72];

    pasLHashInfo  sHashInfo;
    struct stData{
    	unsigned long id;
    	unsigned long count;
    };
    long lCount;
    struct stData *psData;
    char caCname1[72];
    char caSql[2048];
    char caWireflag[20];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
  //  utMsgPrintMsg(psMsgHead);
      strcpy(caGroups,getDsGroupid());
      strcpy(caFacs,getDsFacGroupid());
    memset(caCname,0,sizeof(caCname));
   	iReturn = utMsgGetSomeNVar(psMsgHead,3,
					"node",        UT_TYPE_STRING,10,caPid,
					"cname",       UT_TYPE_STRING,30,caCname1,
					"plate",       UT_TYPE_STRING,100,caPlate);
				
				
    if(strlen(caCname1)>0){
    	strcpy(caCname,pasCvtGBK(2,caCname1,caTemp,30));
    }
//    iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
    strcpy(caWhere,"\0");



    lRowNum = utComGetVar_ld(psShmHead,"GroupTreeRow",300);
  
    
 
    iNum = 0;
    
     psDbHead = utPltInitDb();
    if(strlen(caCname)==0){
		    lStartRec = (lCurPg - 1) * lRowNum;  
		     sprintf(caTemp,"select groupname,groupid,pid from ncsgroup where 1=1 ");
		     if(strlen(caGroups)>0){
		     	  if(strlen(caPid)==0){
		     	 sprintf(caSql,"select pid from ncsgroup where pid not in (%s) and groupid in (%s) ",caGroups,caGroups);
		     	 lPid=0;
		     	 pasDbOneRecord(caSql,0,UT_TYPE_LONG,4,&lPid);
		     	 sprintf(caPid,"%d",lPid);
		     	}
		     	sprintf(caTemp+strlen(caTemp),"and groupid in (%s) ",caGroups);
		    }
		    sprintf(caTemp+strlen(caTemp)," and pid=%d ",atol(caPid));
		    sprintf(caTemp+strlen(caTemp)," order by groupname limit %lu,%lu ",0,lRowNum);
	//	    printf("caTemp=%s\n",caTemp);
		    psCur=pasDbOpenSql(caTemp,0);
		  
		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   
		    
		     
		
		    memset(caGroupname,0,sizeof(caGroupname));
		    lGroupid=0;
		    lPid=0;
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,68,caGroupname,
		                                   UT_TYPE_LONG,4,&lGroupid,
		                                   UT_TYPE_LONG,4,&lPid);
		
		 
		    while(iReturn == 0) {
		        
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(1);  
		         utPltPutLoopVar(psDbHead,"groupname", iNum,utStrGetCharHan(caGroupname,48));
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%ld",lGroupid);
		 //        psData = (struct stData *)pasLHashLook(pHash,&lGroupid);       
		 //        	if(psData){
		  //       		printf("id=%d,lcount=%d\n",psData->id,psData->count);
		          	utPltPutLoopVar(psDbHead,"leaf", iNum,"false");
		         		utPltPutLoopVar(psDbHead,"cls", iNum,"folder");
		         		utPltPutLoopVar(psDbHead,"dtype",iNum,"2");
		 //        	}
		  //        else{
		  //        	utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
		  //       		utPltPutLoopVar(psDbHead,"cls", iNum,"file");
		          	
		
		 //         }
		        
		
		        utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
		
		        utPltPutLoopVarF(psDbHead,"pid",iNum,"%d",lPid);
		
				    memset(caGroupname,0,sizeof(caGroupname));
				    lGroupid=0;
				    lLevel=0;
				    lPid=0;
				    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,68,caGroupname,
				                                   UT_TYPE_LONG,4,&lGroupid,
				                                   UT_TYPE_LONG,4,&lPid);
		    }
		    pasDbCloseCursor(psCur);
		//     free(pHash);
		    utPltPutVar(psDbHead,"rootname",utComGetVar_sd(psShmHead,"rootname","全部单位"));
    	
     }
    	sprintf(caTemp,"select dispname,groupid,gid,groupname from ncsrvgroup where 1=1 ");
    	if(strlen(caGroups)>0){
    		sprintf(caTemp+strlen(caTemp)," and gid in (%s) ",caGroups);
    	}
    
    	
    
   	
    	
    if(strlen(caCname)==0){
    	   
		    	if(strlen(caPid)==0){
		    		  sprintf(caTemp+strlen(caTemp)," and gid not in (select groupid from ncsgroup ) ");
		    		  
		    	}
		    	else{
		    		  sprintf(caTemp+strlen(caTemp)," and gid=%d ",atol(caPid));
		    		 		
		    	}
     }
     else{
     	   sprintf(caTemp+strlen(caTemp),"and (dispname like '%c%s%c' or groupname like '%c%s%c') ",'%',caCname,'%','%',caCname,'%');
     	  
     	 
     	
     }
      sprintf(caTemp+strlen(caTemp)," order by dispname limit %lu,%lu",0,lRowNum);
     printf("caTemp=%s\n",caTemp);
       psCur = pasDbOpenSql(caTemp,0);
    	 if(psCur!=NULL){
    	    memset(caGroupname,0,sizeof(caGroupname));
			    lGroupid=0;
			    lPid=0;
			    memset(caServicecode,0,sizeof(caServicecode));
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,68,caGroupname,
			                                   UT_TYPE_LONG,4,&lGroupid,
			                                   UT_TYPE_LONG,4,&lPid,
			                                   UT_TYPE_STRING,16,caServicecode);
			
			    while(iReturn == 0) {
			        
			         if(iNum>0){
			         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
			         }
			         iNum++;
			         utPltSetCvtHtml(1);  
			         utPltPutLoopVar(psDbHead,"groupname", iNum,utStrGetCharHan(caGroupname,48));
			         utPltSetCvtHtml(0);  
			         utPltPutLoopVarF(psDbHead,"did",   iNum,"%ld",lGroupid);

			          	utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
			         		utPltPutLoopVar(psDbHead,"cls", iNum,"file");
			          	utPltPutLoopVar(psDbHead,"check",iNum,",checked:false");
			            utPltPutLoopVar(psDbHead,"dtype",iNum,"0");
			 			
			        utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);			
			        utPltPutLoopVarF(psDbHead,"pid",iNum,"%d",lPid);
			        utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
			 
					    memset(caGroupname,0,sizeof(caGroupname));
					    lGroupid=0;
					    lLevel=0;
					    lPid=0;
			    memset(caServicecode,0,sizeof(caServicecode));
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,68,caGroupname,
			                                   UT_TYPE_LONG,4,&lGroupid,
			                                   UT_TYPE_LONG,4,&lPid,
			                                   UT_TYPE_STRING,16,caServicecode);
			    }
			    pasDbCloseCursor(psCur);
    	  }
    	
    	
// utPltShowDb(psDbHead);
   if(strlen(caPlate)>0){
   	utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
  }
  else{
    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_user_tree.htm");
    	}
   

    return 0;
}
//显示liecene数
int lan_showstatus(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead){

  utPltDbHead *psDbHead;


  psDbHead = utPltInitDb();
	
//	printf("caTemp11=%s\n",caTemp);
 char caProSn[32];
 unsigned long lExpire,lUsers,lStart,lCount;
 strcpy(caProSn,utComGetVar_sd(psShmHead,"ProductSN",""));
   pasInstGetLicense(caProSn,&lExpire,&lUsers,&lStart);

  lCount=0;
  
  utPltPutVar(psDbHead,"psn",caProSn);
	utPltPutVar(psDbHead,"nwversion",PORTAL_VERSION);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"proauth_show_firststatus.htm");	
	
	return 0;
}


//显示单位信息
int proauthWebCorplist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
    char caTemp[1024];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime,l,lTime1,lSip,lDip;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn;
   	long lRowNum,lflags,lStartRec,lCount,lSid;
	  char sqlbuf[2048]="";

 	  pasDbCursor *psCur;
 
   char caTotPg[20],caCurPg[20];
   char caKeyword[64],caTime_flag[16];
   char caPage[12],caLimit[16],caSort[16],caDir[16];

   char caSdate_d[20],caEdate_d[20],caEtime[16];
   unsigned long lUserid,lGroupid,lGtype;
   char caUsername[36],caPassword[36],caDispname[128],caAddress[256],caTelphone[32],caContact[36],caJointype[16],caGroupname[64],caDwlb[64],caFac[128];
  char caFcode[16];
  char caTemp_d[68];
  char caFacs[2048],caGroups[2048];
  char caCode[16],caName[64];
  char caGroupid[20],caDid[20];
  char caGroupids[2048],caDel[20],caSelsid[512];
  char caMid[64],caMove[16];
  char caMark[128],caServicecode[32],caLinkman[32],caDemo[128],caGtype[16];
  unsigned long lMid;
   lCurPg=1;
 
	utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,22,
	    "TotPg",      UT_TYPE_STRING, 12, caTotPg,
			"CurPg",      UT_TYPE_STRING, 12, caCurPg,			
			"keyword", UT_TYPE_STRING,60,caKeyword,		
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "groupid",      UT_TYPE_STRING,10,caGroupid,
   	  "did",          UT_TYPE_STRING,10,caDid,
   	  "selsid",       UT_TYPE_STRING,500,caSelsid,
   	  "del",          UT_TYPE_STRING,10,caDel,
   	  "mid",          UT_TYPE_STRING,60,caMid,
   	  "move",         UT_TYPE_STRING,10,caMove,
   	  "servicecode",  UT_TYPE_STRING,30,caServicecode,
   	  "dispname",     UT_TYPE_STRING,30,caDispname,
   	  "groupname",    UT_TYPE_STRING,30,caGroupname,
   	  "linkman",      UT_TYPE_STRING,30,caLinkman,
   	  "telphone",     UT_TYPE_STRING,30,caTelphone,
   	  "address",      UT_TYPE_STRING,30,caAddress,
   	  "demo",         UT_TYPE_STRING,30,caDemo,
   	  "mark",         UT_TYPE_STRING,60,caMark,
   	  "gtype",        UT_TYPE_STRING,10,caGtype);
   	
   	strcpy(caFacs,getDsFacGroupid());
    strcpy(caGroups,getDsGroupid());
	   if(strlen(caGroupid)>0){
	   	strcpy(caGroupids,getGroupid(atol(caGroupid)));
	  }
	  else{
	  	strcpy(caGroupids,"");
	  }
	  
    utStrDelSpaces(caKeyword); 
    utStrDelSpaces(caServicecode); 
    
    if(strlen(caServicecode)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caServicecode,caTemp,30));
      	strcpy(caServicecode,caTemp_d);
     }    
    utStrDelSpaces(caDispname); 
     if(strlen(caDispname)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caDispname,caTemp,60));
      	strcpy(caDispname,caTemp_d);
     }
     if(strlen(caMark)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caMark,caTemp,60));
      	strcpy(caMark,caTemp_d);
     }
    
    utStrDelSpaces(caGroupname); 
    if(strlen(caGroupname)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caGroupname,caTemp,60));
      	strcpy(caGroupname,caTemp_d);
     }
    
    utStrDelSpaces(caLinkman); 
     if(strlen(caLinkman)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caLinkman,caTemp,30));
      	strcpy(caLinkman,caTemp_d);
     }
    
    utStrDelSpaces(caTelphone); 
     if(strlen(caTelphone)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caTelphone,caTemp,30));
      	strcpy(caTelphone,caTemp_d);
     }
    
    utStrDelSpaces(caAddress); 
     if(strlen(caAddress)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caAddress,caTemp,60));
      	strcpy(caAddress,caTemp_d);
     }
    
    utStrDelSpaces(caDemo); 
     if(strlen(caDemo)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caDemo,caTemp,60));
      	strcpy(caDemo,caTemp_d);
     }
    
    
      if(strlen(caKeyword)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
      	strcpy(caKeyword,caTemp_d);
      }
           
      lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

    if(strcmp(caDel,"del")==0){
    	pasDbExecSqlF("delete from ncsrvgroup where groupid in (%s) ",caSelsid);
    }
    if(strcmp(caMove,"move")==0){
    	 if(strlen(caMid)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caMid,caTemp,60));
      	strcpy(caMid,caTemp_d);
      	 lMid=0;
      	 sprintf(caTemp,"select groupid from ncsgroup where groupname='%s' ",caMid);
      	 pasDbOneRecord(caTemp,0,UT_TYPE_ULONG,4,&lMid);
      	 pasDbExecSqlF("update ncsrvgroup set gid=%d where groupid in (%s) ",lMid,caSelsid);
      }
    	
    }
    
    
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;    	
   

	
	    sprintf(sqlbuf,"select count(*) from ncsrvgroup where 1=1 ");
	    if(strlen(caGroups)>0){
    		sprintf(sqlbuf+strlen(sqlbuf)," and gid in (%s) ",caGroups);
    	}
    
    	if(strlen(caKeyword)>0){
    		sprintf(sqlbuf+strlen(sqlbuf),"and (dispname like '%c%s%c' or groupname like '%c%s%c' )",'%',caKeyword,'%','%',caKeyword,'%');
    	}
    	if(strlen(caServicecode)>0){
    		sprintf(sqlbuf+strlen(sqlbuf),"and (groupname like '%c%s%c' )",'%',caServicecode,'%');
    	}
    	if(strlen(caDispname)>0){
    		sprintf(sqlbuf+strlen(sqlbuf),"and (dispname like '%c%s%c' )",'%',caDispname,'%');
    	}
    	if(strlen(caLinkman)>0){
    		sprintf(sqlbuf+strlen(sqlbuf),"and (linkman like '%c%s%c' )",'%',caLinkman,'%');
    	}
    	if(strlen(caTelphone)>0){
    		sprintf(sqlbuf+strlen(sqlbuf),"and (telphone like '%c%s%c' )",'%',caTelphone,'%');
    	}
    	if(strlen(caAddress)>0){
    		sprintf(sqlbuf+strlen(sqlbuf),"and (address like '%c%s%c' )",'%',caAddress,'%');
    	}
     if(strlen(caDemo)>0){
    		sprintf(sqlbuf+strlen(sqlbuf),"and (memo like '%c%s%c' )",'%',caDemo,'%');
    	}
    	if(strlen(caGroupname)>0){
    		sprintf(sqlbuf+strlen(sqlbuf)," and gid in (select groupid from ncsgroup where groupname ='%s' )",caGroupname);
    	}
    	if(strlen(caGroupids)>0){
    		sprintf(sqlbuf+strlen(sqlbuf)," and gid in (%s) ",caGroupids);
    	}
    	if(strlen(caDid)>0){
    		sprintf(sqlbuf+strlen(sqlbuf)," and groupid=%s ",caDid);
    	}
    	if(strlen(caMark)>0){
    		sprintf(sqlbuf+strlen(sqlbuf)," and mark like '%c%s%c' ",'%',caMark,'%');
    	}
    	if(strlen(caGtype)>0){
    		sprintf(sqlbuf+strlen(sqlbuf)," and gtype=%s ",caGtype);
    	}
	lCount=0;
	pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lCount);
  
	sprintf(sqlbuf,"select groupid,groupname,gid,dispname,address,telphone,linkman,gtype,mark from ncsrvgroup where 1=1 ");
	    if(strlen(caGroups)>0){
    		sprintf(sqlbuf+strlen(sqlbuf)," and gid in (%s) ",caGroups);
    	}
    
    	if(strlen(caKeyword)>0){
    		sprintf(sqlbuf+strlen(sqlbuf)," and (dispname like '%c%s%c' or groupname like '%c%s%c' )",'%',caKeyword,'%','%',caKeyword,'%');
    	}
    	if(strlen(caGroupids)>0){
    		sprintf(sqlbuf+strlen(sqlbuf)," and gid in (%s) ",caGroupids);
    	}
    	if(strlen(caDid)>0){
    		sprintf(sqlbuf+strlen(sqlbuf)," and groupid=%s ",caDid);
    	}
    	if(strlen(caMark)>0){
    		sprintf(sqlbuf+strlen(sqlbuf)," and mark like '%c%s%c' ",'%',caMark,'%');
    	}
    	if(strlen(caServicecode)>0){
    		sprintf(sqlbuf+strlen(sqlbuf),"and (groupname like '%c%s%c' )",'%',caServicecode,'%');
    	}
    	if(strlen(caDispname)>0){
    		sprintf(sqlbuf+strlen(sqlbuf),"and (dispname like '%c%s%c' )",'%',caDispname,'%');
    	}
    	if(strlen(caLinkman)>0){
    		sprintf(sqlbuf+strlen(sqlbuf),"and (linkman like '%c%s%c' )",'%',caLinkman,'%');
    	}
    	if(strlen(caTelphone)>0){
    		sprintf(sqlbuf+strlen(sqlbuf),"and (telphone like '%c%s%c' )",'%',caTelphone,'%');
    	}
    	if(strlen(caAddress)>0){
    		sprintf(sqlbuf+strlen(sqlbuf),"and (address like '%c%s%c' )",'%',caAddress,'%');
    	}
     if(strlen(caDemo)>0){
    		sprintf(sqlbuf+strlen(sqlbuf),"and (memo like '%c%s%c' )",'%',caDemo,'%');
    	}
    	if(strlen(caGroupname)>0){
    		sprintf(sqlbuf+strlen(sqlbuf)," and gid in (select groupid from ncsgroup where groupname ='%s' )",caGroupname);
    	}
    	if(strlen(caGtype)>0){
    		sprintf(sqlbuf+strlen(sqlbuf)," and gtype=%s ",caGtype);
    	}
    	
	sprintf(sqlbuf+strlen(sqlbuf)," order by %s %s limit %lu,%lu",caSort,caDir,lStartRec,lRowNum);

 printf("sqlbuf=%s\n",sqlbuf);
 psCur = pasDbOpenSql(sqlbuf,0);
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsg_pasmsg.htm","用户查询","查询数据出错 %s",pasDbErrorMsg(NULL));
        return 0;
    }
	 	
  
  psDbHead = utPltInitDb();
    
        iReturn = 0;
			  lUserid=0;
			  memset(caUsername,0,sizeof(caUsername));
			  memset(caPassword,0,sizeof(caPassword));
			  lGroupid=0;
			  memset(caDispname,0,sizeof(caDispname));
			  memset(caAddress,0,sizeof(caAddress));
			  memset(caTelphone,0,sizeof(caTelphone));
			  memset(caContact,0,sizeof(caContact));
			  memset(caJointype,0,sizeof(caJointype));
			  memset(caFcode,0,sizeof(caFcode));
			  memset(caMark,0,sizeof(caMark));
			  lGtype=0;
			  
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lUserid,
			                                   UT_TYPE_STRING,31,caUsername,
			                                   UT_TYPE_ULONG,4,&lGroupid,
			                                   UT_TYPE_STRING,127,caDispname,
			                                   UT_TYPE_STRING,255,caAddress,
			                                   UT_TYPE_STRING,31,caTelphone,
			                                   UT_TYPE_STRING,31,caContact,
			                                   UT_TYPE_LONG,4,&lGtype,
			                                   UT_TYPE_STRING,127,caMark);

			                                  
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
    	
    	    if(iNum>0){
        		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
          }
          else{
          	utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
          }
	        iNum++;
	        
	        utPltPutLoopVarF(psDbHead,"num", iNum,"%lu",iNum);
          utPltPutLoopVarF(psDbHead,"userid", iNum,"%lu",lUserid);
          utPltPutLoopVarF(psDbHead,"gtype",iNum,"%lu",lGtype);
          utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
          utPltSetCvtHtml(1);
          utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
          utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
          utPltPutLoopVar(psDbHead,"groupname",iNum,authGetGroupNameById(psShmHead,lGroupid));
          utPltPutLoopVar(psDbHead,"mark",iNum,caMark);
          utPltPutLoopVar(psDbHead,"contact",iNum,caContact);
          utPltPutLoopVar(psDbHead,"telphone",iNum,caTelphone);
      
          utPltPutLoopVar(psDbHead,"addr",iNum,caAddress);
          
          memset(caCode,0,sizeof(caCode)); 
          sprintf(caCode,"%d",lGtype);    
          utPltPutLoopVar(psDbHead,"dwlb",iNum,authGetClientLbBycode(psShmHead,caCode));
     //     printf("code=%s,fac=%s\n",caCode,authGetClientFacBycode(psShmHead,caFcode));
          
       //   utPltPutLoopVar(psDbHead,"jrcj",iNum,authGetClientFacBycode(psShmHead,caFcode));
       //   utPltPutLoopVar(psDbHead,"jointype",iNum, ncsGetJoinTypeByCode(caJointype));

          utPltSetCvtHtml(0);
         
	     
	      
         
      
     
			  lUserid=0;
			  memset(caUsername,0,sizeof(caUsername));
			  memset(caPassword,0,sizeof(caPassword));
			  lGroupid=0;
			  memset(caDispname,0,sizeof(caDispname));
			  memset(caAddress,0,sizeof(caAddress));
			  memset(caTelphone,0,sizeof(caTelphone));
			  memset(caContact,0,sizeof(caContact));
			  memset(caJointype,0,sizeof(caJointype));
			  memset(caFcode,0,sizeof(caFcode));
			  memset(caMark,0,sizeof(caMark));
			  lGtype=0;
			  
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lUserid,
			                                   UT_TYPE_STRING,31,caUsername,
			                                   UT_TYPE_ULONG,4,&lGroupid,
			                                   UT_TYPE_STRING,127,caDispname,
			                                   UT_TYPE_STRING,255,caAddress,
			                                   UT_TYPE_STRING,31,caTelphone,
			                                   UT_TYPE_STRING,31,caContact,
			                                   UT_TYPE_LONG,4,&lGtype,
			                                   UT_TYPE_STRING,127,caMark);
    }



    lTotPg = (lCount -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lCount);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
    
        
//     utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"dwgl/ncs_corp_list.htm");
    return 0;
}


//单位组树——供comp选择使用

int ncsTreeGroup_comp_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
        char caGroupname[32],caGroupid[16];

        char caName[32];
        char caWhere[256];
        char caSql[256];
        char caCurPg[16],caTotRec[16];
        char caPid[16];
        unsigned long lLevel,lPid;
        int  iNum;
        char caFilter[256];
        char caNo[16];
        unsigned long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
        int iReturn,i;
        char caVar[256],caFname[64],caBfname[64];
        char caValue[256];
        char caVar1[32];
        char caOpt[256];
        char caCode[16];
        long lId;
        char *p,*pVar;
        unsigned char *pHash;
    pasLHashInfo  sHashInfo,sHashInfo2;
    struct stData{
        unsigned long groupid;
        unsigned long pid;
        char caGroupname[32];
        char caCode[12];
        struct stData * pArea;  //子节点
                  long lSum;             //子节点个数
    };
    char caTemp[2048];
    long lCount,lSum,lSum2,m,n;
    char caGroups[1024];
     //获取权限组ID
      strcpy(caGroups,getDsGroupid());
    struct stData *psData,*pAreastatdata,*psData2,*psData3;
   	iReturn = utMsgGetSomeNVar(psMsgHead,1,"plate",      UT_TYPE_STRING, 127, caPlate);

#ifdef LDEBUG1
    utMsgPrintMsg(psMsgHead);
#endif
    utMsgPrintMsg(psMsgHead);



 //   iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
    strcpy(caWhere,"\0");
 //   if(iReturn == 0 && lGroupid0 > 0) {
 //       sprintf(caWhere,"and (groupid = %lu or pid=%lu) ",lGroupid0,lGroupid0);
 //   }

  //  sprintf(caWhere+strlen(caWhere)," and level=%d ",atol(caLevel));

//    if(strlen(caPid)>0)
//      sprintf(caWhere+strlen(caWhere)," and pid=%d and level>0",atol(caPid));
//    else{
//       sprintf(caWhere+strlen(caWhere)," and pid=0 and level=0",atol(caPid));
//    }
 //    memset(caOpt,0,sizeof(caOpt));
 //   iReturn = dsCltGetMyInfo(1,"Userid",&lId);
 //   if(iReturn==0){
//          pasDbOneRecord("select opt from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);
//    }



    lRowNum = utComGetVar_ld(psShmHead,"GroupTreeRow",1000);

    pHash = (unsigned char *)pasLHashInit(lRowNum,lRowNum,sizeof(struct stData),0,4);

    if(pHash == NULL) {
             printf(" Memory HttpHash Error \n");
             return (-1);
    }
    sprintf(caTemp,"select pid,groupid,groupname,groupcode from ncsgroup where 1=1 ");

 //  if(!strstr(caOpt,"all")&&(strlen(caOpt)>0)){
//                      sprintf(caTemp+strlen(caTemp)," and (groupid in (%s) or groupid in (select groupid from ncsgroup where pid in (%s))) ",caOpt,caOpt);
//   }
   if(strlen(caGroups)>0){
        sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",caGroups);
  }
   printf("caTemp=%s\n",caTemp);
    psCur=pasDbOpenSql(caTemp,0);

    if(psCur==NULL){
        free(pHash);
        return 0;
    }
    lGroupid=0;
    lPid=0;
    memset(caGroupname,0,sizeof(caGroupname));
    memset(caCode,0,sizeof(caCode));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lPid,
                                   UT_TYPE_LONG,4,&lGroupid,
                                   UT_TYPE_STRING,31,caGroupname,
                                   UT_TYPE_STRING,10,caCode);
    while(iReturn == 0||iReturn==1405) {
         psData = (struct stData *)pasLHashLookA(pHash,&lGroupid);
       if(psData){
        psData->pid=lPid;
        strcpy(psData->caGroupname,caGroupname);
        strcpy(psData->caCode,caCode);
       }
    lGroupid=0;
    lPid=0;
    memset(caGroupname,0,sizeof(caGroupname));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lPid,
                                   UT_TYPE_LONG,4,&lGroupid,
                                   UT_TYPE_STRING,31,caGroupname);
    }
    pasDbCloseCursor(psCur);


    pAreastatdata=(struct stData*)malloc((1000)*sizeof(struct stData));
    if(pAreastatdata==NULL){
            utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;
    }
    memset(pAreastatdata,0,(1000)*sizeof(struct stData));
    lSum=0;

    psData = (struct stData *)pasLHashFirst(pHash,&sHashInfo);

     while(psData && lSum < 1000){
             psData2 = (struct stData *)pasLHashLook(pHash,&psData->pid);
                  //找不到，代表是根
            if(psData2==NULL){
                pAreastatdata[lSum].pid=psData->pid;
                pAreastatdata[lSum].groupid=psData->groupid;
                strcpy(pAreastatdata[lSum].caGroupname,psData->caGroupname);
                strcpy(pAreastatdata[lSum].caCode,psData->caCode);
                //分配子节点
                pAreastatdata[lSum].pArea=(struct stData*)malloc((100)*sizeof(struct stData));
                memset(pAreastatdata[lSum].pArea,0,100*sizeof(struct stData));
                //获取下个节点的数据
                 psData3 = (struct stData *)pasLHashFirst(pHash,&sHashInfo2);
                 lSum2=0;
                 while(psData3&&lSum2<100){
                         if(pAreastatdata[lSum].groupid==psData3->pid){
                                 pAreastatdata[lSum].pArea[lSum2].pid=psData3->pid;
                                 pAreastatdata[lSum].pArea[lSum2].groupid=psData3->groupid;
                                 strcpy(pAreastatdata[lSum].pArea[lSum2].caCode,psData3->caCode);
                                 strcpy(pAreastatdata[lSum].pArea[lSum2].caGroupname,psData3->caGroupname);
                                 pAreastatdata[lSum].lSum++;
                                 lSum2++;
                         }
                                psData3 = (struct stData *)pasLHashNext(&sHashInfo2);
                 }

                lSum++;
            }

        psData = (struct stData *)pasLHashNext(&sHashInfo);
     }
    iNum=0;
     psDbHead = utPltInitDb();

     for(i=0;i<lSum;i++){
        if(iNum>0){
                                utPltPutLoopVar(psDbHead,"dhf",iNum+1,",");
          }
          utPltPutLoopVar(psDbHead,"area",iNum+1,pAreastatdata[i].caGroupname);
          utPltPutLoopVarF(psDbHead,"groupid",iNum+1,"%d",pAreastatdata[i].groupid);
          utPltPutLoopVar(psDbHead,"code",iNum+1,pAreastatdata[i].caCode);
          if(pAreastatdata[i].lSum>0){
                utPltPutLoopVar(psDbHead,"iconCls",iNum+1,"task-folder");
          }
          else{
                utPltPutLoopVar(psDbHead,"iconCls",iNum+1,"task");
          }
            //输出第二层节点
            for(m=0;m<pAreastatdata[i].lSum;m++){
             if(m>0){
                         utPltPutLoopVar2(psDbHead,"dhs",iNum+1,m+1,",");
                }
                  utPltPutLoopVarF2(psDbHead,"groupid",iNum+1,m+1,"%d",pAreastatdata[i].pArea[m].groupid);
                  utPltPutLoopVar2(psDbHead,"area",iNum+1,m+1,pAreastatdata[i].pArea[m].caGroupname);
                  utPltPutLoopVar2(psDbHead,"code",iNum+1,m+1,pAreastatdata[i].pArea[m].caCode);
                  //输出第三层节点
                  n=0;
                   psData = (struct stData *)pasLHashFirst(pHash,&sHashInfo);

           while(psData && n < 100){
                if(pAreastatdata[i].pArea[m].groupid==psData->pid){
                if(n>0){
                                utPltPutLoopVar3(psDbHead,"dht",iNum+1,m+1,n+1,",");
                        }
                        utPltPutLoopVar3(psDbHead,"area",iNum+1,m+1,n+1,psData->caGroupname);
                        utPltPutLoopVarF3(psDbHead,"groupid",iNum+1,m+1,n+1,"%d",psData->groupid);
                        utPltPutLoopVar3(psDbHead,"code",iNum+1,m+1,n+1,psData->caCode);
                n++;
           }
                psData = (struct stData *)pasLHashNext(&sHashInfo);

          }


            }

        iNum++;
      }

        if(pHash)  free(pHash);
                for(i=0;i<lSum;i++){
                                free(pAreastatdata[lSum].pArea);
                }
          free(pAreastatdata);


    utPltPutVar(psDbHead,"rootname",utComGetVar_sd(psShmHead,"rootname","全部单位"));

//utPltShowDb(psDbHead);
   if(strlen(caPlate)>0){
   	  utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
   }
   else{
          utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_group_select_tree.htm");
       }


    return 0;
}

/* 显示增加/修改用户的界面  */
int ncsWebDispClientAddForm_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caIp[34],caMac[64],caAddtime[16];
    char caUserid[32];
   	char caUsername[34];
   	char caEmail[127],caLasttime[16];
   	char caPassword[32];
   	char caMark[128];
   	long lAddtime,lLasttime,lFlags;
   	double fLongitude,fLatitude;
   	char caCond[32],caGroupid[16];
   	char caContact[32],caTelphone[32],caAddress[128];
   	int iNum1;
   	long lUse;
   	unsigned long lUserid,lGroupid,lGroupid0;
    utPltDbHead *psDbHead;
   	int iReturn;
   	int iModFlags;
   	char caSmt[256];
   	char *pBuf,*pSelect;
   	char  caUse[16];
   	unsigned long lTime;
   	char caDispname[64],groupname[32];
 
   	char caTemp[128];
   	int iNum=0;
   	unsigned int groupid=0;
   	int gtype=0;
    char sqlbuf[4048]="";
    char caGroupcode[32];
   	char handphone[36],servicecode[16],postcode[10],servicestate[12],ceoname[32],areaman[32];
   	char joincode[10],empnum[10],area[72],areatel[32],demo[256],ip[18],servernum[14],endnum[10],jointype[10];
   	int useflags,userid;
   	
   	char servernum1[8],postcode1[16],caEmail1[127];
   	char disflag[8];
   	char caPlate[128];
   	char servicestate_choice[16]="selected";
   	char join_type_choice[15]="sel";
   	long lMonflag=0;
    char caAreacodeToUser[32];
   char caFcode[16],caName[68];
    char caGroups[1024];
    char caFacs[1024];   
   long lDateid;
 
 
   
    strcpy(caGroups,getDsGroupid());

   
    psDbHead = utPltInitDb();
   
   	iReturn = utMsgGetSomeNVar(psMsgHead,5,
   	      "groupid", UT_TYPE_STRING, 15, caGroupid,     /* 位置  */
					"userid",  UT_TYPE_STRING, 15, caUserid,
					"cond",    UT_TYPE_STRING, 31, caCond,
					"disflag", UT_TYPE_STRING,2,disflag,
					"plate",UT_TYPE_STRING,100,caPlate);

    iModFlags = 0;
 //   lUserid = atol(caUserid);

    lUserid=strtoul(caUserid,NULL,10);
     printf("caUserid=%s,userid=%lu\n",caUserid,lUserid);
    lTime = time(0);
    if(!utStrIsSpaces(caUserid)) { 
        iModFlags = 1;
    //    lUserid = atol(caUserid);
        
        
        
        
        userid=0;
        memset(caUsername,0,sizeof(caUsername));
        lGroupid=0;
        memset(caDispname,0,sizeof(caDispname));
        memset(caAddress,0,sizeof(caAddress));
        memset(caTelphone,0,sizeof(caTelphone));
        memset(caContact,0,sizeof(caContact));
        memset(caAddtime,0,sizeof(caAddtime));
        memset(caLasttime,0,sizeof(caLasttime));
        gtype=0;

        memset(demo,0,sizeof(demo));
        memset(servicecode,0,sizeof(servicecode));

       memset(caLasttime,0,sizeof(caLasttime)); 

       lDateid=0;
       lMonflag=0;
        sprintf(sqlbuf,"select groupid,groupname,mark,gid,dispname,address,\
                               telphone,linkman,addtime,flags,gtype,\
                               memo,longitude,latitude, \
                               moditime from ncsrvgroup where groupid=%lu",lUserid);                
       printf("sqlbuf=%s\n",sqlbuf);                
        iReturn = pasDbOneRecord(sqlbuf,
                            0,
                            UT_TYPE_ULONG,4,&userid,
                            UT_TYPE_STRING,31, caUsername,
                            UT_TYPE_STRING,127, caMark,
                            UT_TYPE_ULONG,4,&lGroupid,
                            UT_TYPE_STRING,63, caDispname,
                            UT_TYPE_STRING,127,caAddress,
                            UT_TYPE_STRING,63, caTelphone,
                            UT_TYPE_STRING,31, caContact,
                            UT_TYPE_LONG,4,&lAddtime,
                            UT_TYPE_LONG,4,&lFlags,
                            UT_TYPE_LONG,4,&gtype,
                            UT_TYPE_STRING,255,demo,
                            UT_TYPE_FLOAT,sizeof(double),&fLongitude,
                            UT_TYPE_FLOAT,sizeof(double),&fLatitude,
                            UT_TYPE_LONG,4,&lLasttime);

                           
                               
     if(iReturn != 0 && iReturn != PAS_DB_NULLVALUE) {
            utPltFreeDb(psDbHead);
            utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","修改用户","用户不存在 %d ",iReturn);
           return 0;
      }
    else{    
          utPltPutVar(psDbHead,"userid",    caUserid);
          utPltPutVar(psDbHead,"username",  caUsername);
          utPltPutVar(psDbHead,"dispname",  caDispname); //单位名称
          utPltPutVar(psDbHead,"mark",caMark);
          utPltPutVarF(psDbHead,"flags","%d",lFlags);
          utPltPutVarF(psDbHead,"longitude","%f",fLongitude);
          utPltPutVarF(psDbHead,"latitude","%f",fLatitude);
//          printf("gtype=%d\n",gtype);
//          printf("lGroupid=%d\n",lGroupid);
          utPltPutVarF(psDbHead,"groupid","%lu",lGroupid);//所属组
        
        
        sprintf(sqlbuf,"select code,name from ncsuser_lb where 1=1 ");
 printf("sqlbuf=%s\n",sqlbuf);       
		  	psCur = pasDbOpenSql(sqlbuf,0);
		  	 

		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   char caCode[16];
	     memset(caName,0,sizeof(caName));
       memset(caCode,0,sizeof(caCode));
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
		                                  UT_TYPE_STRING,63,caName);
		                                  
	                              
		
		   iNum=0;
		    while(iReturn == 0||iReturn==1405) {
		        
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(1);  		        
					   utPltPutLoopVarF(psDbHead,"lbname",iNum,"%s",caName);					    
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"lbcode",   iNum,"%s",caCode);
		         if(gtype==atol(caCode)){
		         	utPltPutLoopVar(psDbHead,"check",iNum,",checked:true");
		        }
		       
		         	
	     memset(caName,0,sizeof(caName));
       memset(caCode,0,sizeof(caCode));
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
		                                  UT_TYPE_STRING,63,caName);
		    }
		    pasDbCloseCursor(psCur);
        
        
        
             	 
       
          utPltPutVar(psDbHead,"servicecode",caUsername);

          utPltPutVar(psDbHead,"address", caAddress); 
       
          utPltPutVar(psDbHead,"contact", caContact);  
          utPltPutVar(psDbHead,"telphone",caTelphone);

        strcpy(caAddtime,utTimFormat("%Y-%m-%d %H:%M:%S",lAddtime));
        utPltPutVar(psDbHead,"addtime", caAddtime);
        strcpy(caLasttime,utTimFormat("%Y-%m-%d %H:%M:%S",lLasttime));
        utPltPutVar(psDbHead,"lasttime",caLasttime);
          

          utPltPutVar(psDbHead,"groupname",authGetGroupNameById(psShmHead,lGroupid));
          utPltSetCvtHtml(1);
          utPltPutVar(psDbHead,"demo",demo);//备注
          utPltSetCvtHtml(0);

        
    }
  }
    else {
    	
    	        sprintf(sqlbuf,"select code,name from ncsuser_lb where 1=1 ");
 printf("sqlbuf=%s\n",sqlbuf);       
		  	psCur = pasDbOpenSql(sqlbuf,0);
		  	 

		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   char caCode[16];
	     memset(caName,0,sizeof(caName));
       memset(caCode,0,sizeof(caCode));
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
		                                  UT_TYPE_STRING,63,caName);
		                                  
		printf("iReturn=%d\n",iReturn);                                
		
		   iNum=0;
		    while(iReturn == 0||iReturn==1405) {
		        
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(1);  		        
					   utPltPutLoopVarF(psDbHead,"lbname",iNum,"%s",caName);					    
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"lbcode",   iNum,"%s",caCode);
		         if(gtype==atol(caCode)){
		         	utPltPutLoopVar(psDbHead,"check",iNum,",checked:true");
		        }
		       
		         	
	     memset(caName,0,sizeof(caName));
       memset(caCode,0,sizeof(caCode));
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
		                                  UT_TYPE_STRING,63,caName);
		    }
		    pasDbCloseCursor(psCur);
        
        
    	
    	
    	
     
        
        
        if(strlen(caGroupid)!=0){
        	utPltPutVar(psDbHead,"groupname",authGetGroupNameById(psShmHead,atol(caGroupid)));
          utPltPutVar(psDbHead,"groupid",caGroupid);
       }
        else
        utPltPutVar(psDbHead,"groupid","0");
        
        utPltPutVar(psDbHead,"qtcheck",",checked:true");
     
      
    }
 
    

    	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/nc_form_clientedit.htm");

    return 0;
}



/* 增加用户  */
int ncsWebClientAdd_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
   	char caUsername[33],caGroupid[33],caIp[16],caMac[33],caMacstr[42],caUse[10];
   	char caDispname[64],caLimit[32];
   	char caMark[128],caGroupname[64];
   	char caContact[32],caTelphone[32],caAddress[128];
   	char caHandphone[32];
   	unsigned long lUserid,lGroupid;
    utPltDbHead *psDbHead;
   	int iReturn;
   	unsigned long lUse,lUserid0;
   	unsigned long lIp;
   	int iPass,iAddFlags;
   	char caPassword[32],caPassword1[32];
   	char caDate[32],caUserid[16],caUsername0[32];
   	char caGtype[16];
//   	ncsClient *psClient;
    char caOk[16],caSave[16],caEmail[64],caWebip[16],caPost[16],caLimitid[16];

	char handphone[32],servicecode[16],postcode[16],servicestate[10],ceoname[30],areaman[30],gtype[32],addtime[30],lasttime[30];
  char joincode[16],empnum[16],area[70],areatel[30],demo[255],ip[16],servernum[12],endnum111[10],jointype[12];
  char servicestate_desc[32],pre_setdesc[32],caPreset[16];
	char sqlbuf[4048]="";
  char caTemp[2024];
  char caUsername2[32];
 	char caMsg[1024];
 	char caSdate[20];
 	char caMonflag[12];
 	char caFcode[16],caDateid[16];
 	char caLongtitude[16],caLatitude[16];
 	memset(caMonflag,0,sizeof(caMonflag));
 	dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);

   
   lGroupid=0;
    utMsgPrintMsg(psMsgHead);
    memset(caUsername,0,sizeof(caUsername));
   	iReturn = utMsgGetSomeNVar(psMsgHead,13,
   	      "userid",      UT_TYPE_STRING, 10, caUserid,
					"username",    UT_TYPE_STRING, 30, caUsername,
				  "groupid",     UT_TYPE_STRING,10,caGroupid,
				  "groupname",   UT_TYPE_STRING,63,caGroupname,
					"dispanme",    UT_TYPE_STRING, 63, caDispname,
					"address",     UT_TYPE_STRING, 127, caAddress,
					"telphone",    UT_TYPE_STRING, 15, caTelphone,
				  "linkman",     UT_TYPE_STRING, 15, caContact,
					"mark",   UT_TYPE_STRING, 127,caMark,
				  "longtitude",  UT_TYPE_STRING, 15, caLongtitude,
					"latitude", UT_TYPE_STRING, 15, caLatitude,
          "gtype",       UT_TYPE_STRING, 10,gtype,
          "demo" ,         UT_TYPE_STRING,255,demo);
					
				
    utStrDelSpaces(caUsername);
    utStrDelSpaces(caGroupname);	
    utStrDelSpaces(caDispname);
    utStrDelSpaces(caContact);			
		utStrDelSpaces(caAddress);
    utStrDelSpaces(caTelphone);	
    utStrDelSpaces(caMark);
    utStrDelSpaces(demo);	
		utStrDelSpaces(caLongtitude);
    utStrDelSpaces(caLatitude);		
			
	//编码转换
	if(strlen(caUsername)>0){
		 pasCvtGBK(2,caUsername,caTemp,30);
		 strcpy(caUsername,caTemp);
	}
	if(strlen(caDispname)>0){
		 pasCvtGBK(2,caDispname,caTemp,63);
		 strcpy(caDispname,caTemp);
	}

	if(strlen(caGroupname)>0){
		 pasCvtGBK(2,caGroupname,caTemp,63);
		 strcpy(caGroupname,caTemp);
		 sprintf(caTemp,"select groupid from ncsgroup where groupname='%s' ",caGroupname);
	//	 printf("caTemp=%s\n",caTemp);
		 lGroupid=0;
		 pasDbOneRecord(caTemp,0,UT_TYPE_ULONG,4,&lGroupid);
	}
	if(strlen(caAddress)>0){
		 pasCvtGBK(2,caAddress,caTemp,127);
		 strcpy(caAddress,caTemp);
	}
	if(strlen(caTelphone)>0){
		 pasCvtGBK(2,caTelphone,caTemp,63);
		 strcpy(caTelphone,caTemp);
	}	
	if(strlen(caContact)>0){
		 pasCvtGBK(2,caContact,caTemp,31);
		 strcpy(caContact,caTemp);
	}	

	if(strlen(caMark)>0){
		 pasCvtGBK(2,caMark,caTemp,127);
		 if(strstr(caTemp,"可以多个标签")){
		 	strcpy(caMark,"");
		}
		else{
		 strcpy(caMark,caTemp);
		}
	}	
	
	if(strlen(demo)>0){
		 pasCvtGBK(2,demo,caTemp,63);
		 strcpy(demo,caTemp);
	}
	if(strlen(caLongtitude)>0){
		 pasCvtGBK(2,caLongtitude,caTemp,20);
		 strcpy(caLongtitude,caTemp);
	}
		if(strlen(caLatitude)>0){
		 pasCvtGBK(2,caLatitude,caTemp,20);
		 strcpy(caLatitude,caTemp);
	}
		
					
		                     
				

  
    
    if(utStrIsSpaces(caUserid)) {
  
          lUserid=utMd5Code(caUsername,strlen(caUsername),"ffff");
          if(lUserid>2000000000) lUserid=lUserid-2000000000;
          if(lUserid>2000000000) lUserid=lUserid-2000000000;
 
        iAddFlags = 1;
           
    }
    else {
      lUserid=strtoul(caUserid,NULL,10);
    	iAddFlags = 0;
    }
    if(iAddFlags) { /* 增加记录  */
        	
//strtoul(caGroupid,NULL,10)
    
    		
          sprintf(sqlbuf,"insert into ncsrvgroup(groupid,groupname,dispname,address,mark,memo,longitude,latitude,addtime,moditime,lasttime,gid,gtype,telphone,linkman) \
           values(%lu,'%s','%s','%s','%s','%s',%f,%f,%d,%d,%d,%lu,%d,'%s','%s')",\
           lUserid,caUsername,caDispname,caAddress,caMark,demo,atof(caLongtitude),atof(caLatitude),time(0),time(0),time(0),lGroupid,atol(gtype),caTelphone,caContact);
	
						printf("sqlbuf=%s\n",sqlbuf);								 
            iReturn = pasDbExecSqlF(sqlbuf);
      
  //          ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",sqlbuf);
        
//						printf("sqlbuf=%s\n",sqlbuf);					 
   //        iReturn =pasDbExecSqlF(sqlbuf);
           
           sprintf(sqlbuf,"添加上网场所 场所编码%s，名称：%s",caUsername,caDispname);    
        ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",sqlbuf);        
        if(iReturn != 0) {
        	  strcpy(caMsg,"false");
            pasDbRollback(NULL);
        }
        else { 
            pasDbCommit(NULL);
            strcpy(caMsg,"true");

             authClient *psClient;
		         psClient=(authClient *)utShmHashLookA(psShmHead,PROAUTH_LNK_CLIENT,caUsername);
			       if(psClient){
			       	psClient->userid=lUserid;
			       	
			       	strcpy(psClient->dispname,caDispname);
			       	psClient->groupid=lGroupid;
			       	psClient->gtype=atoi(gtype);

			       }
         
    //        sprintf(caUserid,"%lu",lUserid);
    //        utMsgAdd_s(&psMsgHead,"userid",caUserid);

           
          
        }
//    printf("iReturn=%d,msg=%s\n",iReturn,caMsg);    
        psDbHead = utPltInitDb();
        utPltPutVar(psDbHead,"mesg",caMsg);
        utPltPutVarF(psDbHead,"userid","%d",lUserid);
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_msg.htm");
        return 0;
        
    }        
    else { /* 修改记录  */
      sprintf(sqlbuf,"update ncsrvgroup set groupname='%s',dispname='%s',address='%s',mark='%s',memo='%s',\
      longitude=%f,latitude=%f,moditime=%d,lasttime=%d,gid=%lu,gtype=%d,telphone='%s',linkman='%s' where groupid=%lu ",
       caUsername,caDispname,caAddress,caMark,demo,atof(caLongtitude),atof(caLatitude),time(0),time(0),lGroupid,atol(gtype),caTelphone,caContact,lUserid);
		
 printf("sqlbuf=%s\n",sqlbuf);     	   
		  
       	    iReturn = pasDbExecSqlF(sqlbuf);
       	    
       
       	    
       			   sprintf(sqlbuf,"修改上网场所信息，代码：%s,显示名：%s",caUsername,caDispname);    
		      ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",sqlbuf);   
		        
        if(iReturn != 0) {
            strcpy(caMsg,"false");
            pasDbRollback(NULL);
        }
        else {
        	  strcpy(caMsg,"true");
            pasDbCommit(NULL);
          

		         authClient *psClient;
		         psClient=(authClient *)utShmHashLook(psShmHead,PROAUTH_LNK_CLIENT,caUsername);
			       if(psClient){
			       	psClient->userid=lUserid;
			      
			       	strcpy(psClient->dispname,caDispname);
			       	psClient->groupid=lGroupid;
			       	psClient->gtype=atoi(gtype);
			       
			       }
        
            
        }

         psDbHead = utPltInitDb();
        utPltPutVar(psDbHead,"mesg",caMsg);
        utPltPutVar(psDbHead,"userid",caUserid);
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_msg.htm");
        return 0;
      }
}

//单位类别树AJAX通讯
int ncsTreeUser_mark(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[72],caGroupid[16];
   	char caTemp[4024];
   	char caName[32];
   	char caWhere[256];

   	char caCurPg[16],caTotRec[16];
   	char caPid[64],caPid1[64];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
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
   	unsigned char *pHash;
   	char caCname[72];

    pasLHashInfo  sHashInfo;
   
    long lCount;
    
    char caCname1[72];
    char caSql[2048];
    char caCode[16];
    char caWireflag[20];
    char caServicecode[24];
    
    char caMarks[128];
   
    struct stData{
        char caMark[64];
    };
    struct stData *psData;
    char caObj[100][50];
    long num;
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
 utMsgPrintMsg(psMsgHead);
      strcpy(caGroups,getDsGroupid());

    memset(caCname,0,sizeof(caCname));
   	iReturn = utMsgGetSomeNVar(psMsgHead,3,
					"node",        UT_TYPE_STRING,60,caPid1,
					"cname",       UT_TYPE_STRING,30,caCname1,
					"plate",       UT_TYPE_STRING,100,caPlate);
				
				
    if(strlen(caCname1)>0){
    	strcpy(caCname,pasCvtGBK(2,caCname1,caTemp,30));
    }
    else{
    	strcpy(caCname,"");
    }
    
    if(strlen(caPid1)>0){
    	strcpy(caPid,pasCvtGBK(2,caPid1,caTemp,60));
    }
    else{
    	strcpy(caPid,"");
    }
    
//    iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
    strcpy(caWhere,"\0");

  


    lRowNum = utComGetVar_ld(psShmHead,"GroupTreeRow",300);
  
  
 
    iNum = 0;
    
     psDbHead = utPltInitDb();
     
     if(strlen(caCname)==0&&strlen(caPid)==0){ 
    pHash = (unsigned char *)pasLHashInit(lRowNum,lRowNum,sizeof(struct stData),0,64);
    if(pHash==NULL){
    	 utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","Hash初始化错误","Hash初始化错误");
    	 return 0;
    }     
     sprintf(caTemp,"select mark from ncsrvgroup where length(mark)>0 group by mark ");
   
     		  psCur=pasDbOpenSql(caTemp,0);
		  
		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
     
     		memset(caMarks,0,sizeof(caMarks));        
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caMarks);
		     while(iReturn == 0||iReturn==1405) { 
		     	
		     	   num=ncs_SepChar0(caMarks,',',caObj);
		     	   for(i=0;i<num;i++){
		     	   	 psData = (struct rData_s *)pasLHashLookA(pHash,caObj[i]);
		     	  }
		     	 memset(caMarks,0,sizeof(caMarks));        
		       iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caMarks);
		    }                             
       pasDbCloseCursor(psCur);
     
     
     
    psData = (struct stData *)pasLHashFirst(pHash,&sHashInfo);
     iNum=0;
     while(psData){
     			   if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         	
		         }
		            utPltPutLoopVar(psDbHead,"groupname", iNum+1,psData->caMark);
		            utPltPutLoopVarF(psDbHead,"groupid",   iNum+1,"%s",psData->caMark);
		         	  utPltPutLoopVar(psDbHead,"leaf", iNum+1,"false");
		         		utPltPutLoopVar(psDbHead,"cls", iNum+1,"folder");
		         		utPltPutLoopVar(psDbHead,"dtype",iNum+1,"2");
		         iNum++;
     	    psData = (struct stData *)pasLHashNext(&sHashInfo);
     	
     }
     
     free(pHash);
    }
    else{
    	
 
    	  sprintf(caTemp,"select dispname,groupid from ncsrvgroup where 1=1 ");
    	  if(strlen(caPid)>0){
    	  	sprintf(caTemp+strlen(caTemp),"and mark like '%c%s%c' ",'%',caPid,'%');
    	  }
    	  if(strlen(caCname)>0){
    	  	sprintf(caTemp+strlen(caTemp)," and (dispname like '%c%s%c' or groupname like '%c%s%c') ",'%',caCname,'%','%',caCname,'%');
    	  }
    	  
    	        psCur = pasDbOpenSql(caTemp,0);
    	 if(psCur!=NULL){
    	    memset(caGroupname,0,sizeof(caGroupname));
			    lGroupid=0;
			    lPid=0;
			    memset(caServicecode,0,sizeof(caServicecode));
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,68,caGroupname,
			                                   UT_TYPE_LONG,4,&lGroupid);
			                                  
			    iNum=0;
			    while(iReturn == 0) {
			        
			         if(iNum>0){
			         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
			         }
			         iNum++;
			         utPltSetCvtHtml(1);  
			         utPltPutLoopVar(psDbHead,"groupname", iNum,utStrGetCharHan(caGroupname,48));
			         utPltSetCvtHtml(0);  
			         utPltPutLoopVarF(psDbHead,"did",   iNum,"%ld",lGroupid);

			          	utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
			         		utPltPutLoopVar(psDbHead,"cls", iNum,"file");
			          	
			            utPltPutLoopVar(psDbHead,"dtype",iNum,"0");
			 
			        
			
			        utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
			
			   
					    memset(caGroupname,0,sizeof(caGroupname));
					    lGroupid=0;
					    lLevel=0;
					    lPid=0;
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,68,caGroupname,
			                                   UT_TYPE_LONG,4,&lGroupid);
			    }
			    pasDbCloseCursor(psCur);
    	  }
    	
    	  
    	  
    }
    	
    	
utPltShowDb(psDbHead);
   if(strlen(caPlate)>0){
   	utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
  }
  else{
    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_user_tree.htm");
    	}
   

    return 0;
}


int proauthInitWebDwgl(utShmHead *psShmHead)
{
	

  pasSetTcpFunName("ncsWebClientClassList_v4",ncsWebClientClassList_v4, 0);
  pasSetTcpFunName("ncsTreeGroup_comp_v4",ncsTreeGroup_comp_v4, 0);
  
  pasSetTcpFunName("ncsLbCode_v4",ncsLbCode_v4, 0);
  pasSetTcpFunName("ncsTreeUser_lb",ncsTreeUser_lb, 0);
  pasSetTcpFunName("ncsTreeUser_v4",ncsTreeUser_v4, 0);
  pasSetTcpFunName("lan_showstatus",lan_showstatus, 0);
  pasSetTcpFunName("proauthWebCorplist",proauthWebCorplist, 0);
  pasSetTcpFunName("ncsTreeGroup_comp_v4",ncsTreeGroup_comp_v4, 0);
  pasSetTcpFunName("ncsWebDispClientAddForm_v4",ncsWebDispClientAddForm_v4, 0);
  pasSetTcpFunName("ncsWebClientAdd_v4",ncsWebClientAdd_v4, 0);
  pasSetTcpFunName("ncsTreeUser_mark",ncsTreeUser_mark, 0);
  
}

int proauthLoadCorp(utShmHead *psShmHead)
{
	ProAuthLoadClientsInfo(psShmHead);
	ProAuthLoadGroupInfo(psShmHead);
	ProAuthLoadLbInfo(psShmHead);
	
	return 0;
}