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

#include <iconv.h>
/* 显示用户导入next界面  */
int ncmUserDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
    char caTemp[400];
    FILE *fp;
    int i,iNum,iReturn;
   
    long lSid,lCount,lFlags;
    utPltDbHead *psDbHead;

 	char caFile[256];
 	char imp_file[129]="";
 	char temp_file[129]="";
 	char sqlbuf[1024]="";
 	int  m=0;
 	char caLocal[128],caRemote[128],caType[128];
 	char caText[256],caBuf[16002];
  unsigned id=0;
  char name[36];
  char caGroupname[36],caDispname[36];
  unsigned long lGroupid;
 	char *p;
#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
#endif
psDbHead = utPltInitDb();
   if(!utFileIsExist("/home/ncmysql/ncsrv/upload")) {
        if(mkdir("/home/ncmysql/ncsrv/upload",777)!=0){
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入","打开文件出错");
        return 0;

        }
}
system("chmod -Rf 777 /home/ncmysql/ncsrv/upload");
//chmod("/home/ncmysql/ncs/upload",777);

	/*取单位*/ 
  sprintf(sqlbuf,"select groupid,groupname,dispname from ncsrvgroup where 1=1 ");
  sprintf(sqlbuf+strlen(sqlbuf)," order by groupname limit 0,2000 ");
	psCur=pasDbOpenSql(sqlbuf,0);
	if(psCur==NULL)
	{
		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入","打开文件出错");
		return 0;
	}
	iReturn=0;
	m=0;
	while((iReturn==0)||(iReturn==1405))
	{
		memset(caGroupname,0,sizeof(caGroupname));
		lGroupid=0;
		memset(caDispname,0,sizeof(caDispname));
		iReturn=pasDbFetchInto(psCur,
		UT_TYPE_LONG,4,&lGroupid,
		UT_TYPE_STRING,30,caGroupname,
		UT_TYPE_STRING,30,caDispname);
			
		if((iReturn==0)||(iReturn==1405))
		{
			m++;
			utPltPutLoopVar(psDbHead,"name",m,caDispname);
			sprintf(caTemp,"%d_%s",lGroupid,caGroupname);
	    utPltPutLoopVar(psDbHead,"id",m,caTemp);
		}
	}
	pasDbCloseCursor(psCur);


	utMsgGetSomeNVar(psMsgHead,1,"fname",UT_TYPE_STRING,  255,imp_file);
	                           
     printf("imp_file=%s\n",imp_file);                 
  if(strlen(imp_file)!=0)
  {
  	p=utStrSkipSpaces(imp_file);
  	p=utStrGetWord(p,temp_file,200,";\n");
  	if((*p)==';')
  	p=utStrGetWord(p+1,temp_file,200,";\n");

  	fp=fopen(temp_file,"r");
  	printf("temp_file=%s\n",temp_file);
  	if(fp == NULL) 
    {
    	utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入","打开文件出错");
     	return 0;
    } 
     	

    p = fgets(caBuf,16000,fp);
    iNum = 0;
    while(p && *p) {
        p = utStrGetWord(p,caTemp,300,",\r\n");
        if(!utStrIsSpaces(caTemp)) {
            iNum++;
            utPltPutLoopVar(psDbHead,"caTemp",iNum,caTemp);
            utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum-1);
            
            if(iNum==1)
            {
            utPltPutVar(psDbHead,"username",caTemp);
            utPltPutVarF(psDbHead,"username_num","%lu",iNum-1);
            }
            if(iNum==2)
            {
            utPltPutVar(psDbHead,"dispname",caTemp);
            utPltPutVarF(psDbHead,"dispname_num","%lu",iNum-1);
            }
            
            if(iNum==3)
            {
            utPltPutVar(psDbHead,"pass",caTemp);
            utPltPutVarF(psDbHead,"pass_num","%lu",iNum-1);
            }
            if(iNum==4)
            {
            utPltPutVar(psDbHead,"openid",caTemp);
            utPltPutVarF(psDbHead,"openid_num","%lu",iNum-1);
            }
            if(iNum==5)
            {
            utPltPutVar(psDbHead,"mark",caTemp);
            utPltPutVarF(psDbHead,"mark_num","%lu",iNum-1);
            }
            if(iNum==6)
            {
            utPltPutVar(psDbHead,"usermac",caTemp);
            utPltPutVarF(psDbHead,"usermac_num","%lu",iNum-1);
            }
        }
        if(*p != ',') {
            break;
        }
        p++;
    }
    fclose(fp);
    
   
}
   utPltPutVar(psDbHead,"fname",temp_file);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/cust_import_next.htm");
    return 0;
}
static int pmUtlGetCsvWord2(char *caBuf,int iNum,char *caName)
{
    char *p;
    int i;
    char caTemp[512];
    p = caBuf;
    i=0;
    while(p && *p) {
        p = utStrGetWord(p,caTemp,500,",\r\n");
        if(i==iNum) {
            strcpy(caName,caTemp);
            return 0;
        }
        if(*p != ',') {
            strcpy(caName,"\0");
            return 0;
        }
        p++;
        i++;
    }
}
 int sepcharbySH(char *in,char caOut[200][256])
{
        char *p,*p2;
        long lCount;
        long m=0;
        lCount=0;
        if(strlen(in)==0) return 0;
        p=in;
        while(*p){
        //      printf("lCount=%d\n",lCount);
                if(*p=='|'){
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
//导入用户
int ncm_userimp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	FILE *fp;
	int iReturn;
	char caFile[128]="";
	
	char *p;
	long sid,id;
	char caUrl[68],caName[68],caUrlClass[36];
  char caOut[200][256];
  long i,lCount;
  long lNum=0;
    long lcount=0;
    char caFirst[12]="";
    char caUrlNum[12]="";
    char caNameNum[12]="";
    char caIdNum[12]="";
    char caDefid[16];
    long lUrl,lName,lId;
    long lTime=0;
    char caTime[16]="";
  int tReturn=0;
  int lIdn=0;
	char  caBuf[16000]="";
	char sqlbuf[2048]="";
	char caKeyword[64];
	char *APName;
  char caDeflev[12];
  long lLev=0;
	long sum=0;
	utPltDbHead *psDbHead;
	char caUsernameNum[16],caDispnameNum[16],caPassNum[16],caOpenidNum[16];
	char caMarkNum[16],caGroupname[64],caValidtime[20],caDefpass[20],caDefmark[128];
	char caUsername[32],caDispname[32],caPass[32],caMark[128],caOpenid[32];
	unsigned long lUsernamenum,lDispnamenum,lPassnum,lOpenidnum,lMarknum;
	unsigned long lVailtime,lGroupid,lSid,lVtime,lUsermac;
	char caGroupname_t[64],caTemp[1024];
	char caMac_all[256],caMac[20],caUsermac_num[16];
//	psDbHead = utPltInitDb();
utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,12,
					"username",UT_TYPE_STRING,11,caUsernameNum,
					"dispname",UT_TYPE_STRING,10,caDispnameNum,
					"pass",UT_TYPE_STRING,10,caPassNum,
					"openid",UT_TYPE_STRING,10,caOpenidNum,
					"mark",UT_TYPE_STRING,10,caMarkNum,
					"groupname",UT_TYPE_STRING,60,caGroupname,
					"usermac",  UT_TYPE_STRING,10,caUsermac_num,
					"validtime",UT_TYPE_STRING,19,caValidtime,
					"defpass",UT_TYPE_STRING,16,caDefpass,
					"defmark",UT_TYPE_STRING,120,caDefmark,
		      "first",     UT_TYPE_STRING, 11,  caFirst,
		      "fname",   UT_TYPE_STRING, 127, caFile);
		     
	lTime=time(0);
	 lVailtime=0;
	if(strlen(caValidtime)>10){
	  lVailtime=utTimStrToLong("%Y-%m-%d %H:%M:%S",caValidtime);
  }
  if(lVailtime>0&&lVailtime<lTime){
	    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","不能导入已经失效的帐号!","不能导入已经失效的帐号!");   	
	    return 0; 
  }  
	      
	  if(utStrIsSpaces(caUsernameNum)) {
        lUsernamenum = 999;
    }
    else {
        lUsernamenum = atol(caUsernameNum);
    }
    
    if(utStrIsSpaces(caDispnameNum)) {
        lDispnamenum = 999;
    }
    else {
        lDispnamenum = atol(caDispnameNum);
    }
    
    if(utStrIsSpaces(caPassNum)) {
        lPassnum = 999;
    }
    else {
        lPassnum = atol(caPassNum);
    }
    
    if(utStrIsSpaces(caOpenidNum)) {
        lOpenidnum = 999;
    }
    else {
        lOpenidnum = atol(caOpenidNum);
    }
  
    if(utStrIsSpaces(caMarkNum)) {
        lMarknum = 999;
    }
    else {
        lMarknum = atol(caMarkNum);
    }
    if(utStrIsSpaces(caUsermac_num)) {
        lUsermac = 999;
    }
    else {
        lUsermac = atol(caUsermac_num);
    }
  
  
  
    fp = fopen(caFile,"r");
    if(fp == NULL) {
    	 utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入","打开文件出错");
        return 0;
    }
    p = fgets(caBuf,10000,fp);
    if(!utStrIsSpaces(caFirst)) {  /* 忽略第一行  */
        p = fgets(caBuf,10000,fp);
    }
    while(p) {
        pmUtlGetCsvWord2(caBuf,lUsernamenum,caUsername);
        pmUtlGetCsvWord2(caBuf,lDispnamenum,caDispname);
        pmUtlGetCsvWord2(caBuf,lPassnum,caPass);
        pmUtlGetCsvWord2(caBuf,lMarknum,caMark);
        pmUtlGetCsvWord2(caBuf,lOpenidnum,caOpenid);
        pmUtlGetCsvWord2(caBuf,lUsermac,caMac_all);
   
   
   
   if(strlen(caPass)==0&&strlen(caDefpass)>0){
   	strcpy(caPass,caDefpass);
   }  
   if(strlen(caMark)==0&&strlen(caDefmark)>0){
   	strcpy(caMark,caDefmark);
  } 
        printf("caGroupname=%s\n",caGroupname);
        
      if(!utStrIsSpaces(caUsername)) {
      	if(strlen(caDispname)==0){
      		strcpy(caDispname,caUsername);
      	}
      	lGroupid=0;
      	memset(caGroupname_t,0,sizeof(caGroupname_t));
        utStrGetSomeWordBy(caGroupname,"_",2,
                                UT_TYPE_LONG,4,&lGroupid,
                                UT_TYPE_STRING,31,caGroupname_t);
      	
			lcount=0;
			sprintf(sqlbuf,"select count(*) from ncsrvuser where groupid=%d and username='%s' ",lGroupid,caUsername);
			
	printf("sqlbuf=%s\n",sqlbuf);	
      
			pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lcount);
			                        

			if(lcount==0)
			{
				
				sprintf(caTemp,"%d_%s",lGroupid,caUsername);
				lSid=utMd5Code(caTemp,strlen(caTemp),"ffff");
   	   if(lSid>2000000000) lSid=lSid-2000000000;
   	   if(lSid>2000000000) lSid=lSid-2000000000;
			
				sprintf(sqlbuf,"insert into ncsrvuser (userid,username,dispname,groupname,groupid,openid,password,mark,addtime,moditime,lasttime,expiretime,usertype,flags) values (%lu,'%s','%s','%s',%lu,'%s','%s','%s',%lu,%lu,%lu,%lu,0,0)",
				 lSid,caUsername,caDispname,caGroupname_t,lGroupid,caOpenid,caPass,caMark,lTime,lTime,lTime,lVailtime);
				 
				
//printf("sqlbuf=%s\n",sqlbuf);																																									                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          					                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
				pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
				pasDbCommit(NULL);
			}
			else {
		      sprintf(sqlbuf,"update ncsrvuser set dispname='%s',groupname='%s',openid='%s',password='%s',mark='%s',moditime=%lu,lasttime=%lu,expiretime=%lu,usertype=0,flags=0 where groupid=%d and username='%s' ",\
		       caDispname,caGroupname_t,caOpenid,caPass,caMark,lTime,lTime,lVailtime,lGroupid,caUsername);
//				printf("sqlbuf=%s\n",sqlbuf);
					pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
				  pasDbCommit(NULL);
			
			}  
	//绑定mac
	if(strlen(caMac_all)>0){
		lNum=sepcharbySH(caMac_all,caOut);
		for(i=0;i<lNum;i++){
	  sprintf(sqlbuf,"select count(*) from ncsrvusermac where usermac='%s' ",caOut[i]);
	  lCount=0;
	  pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lCount);		
	  if(lCount==0){
	  	pasDbExecSqlF("insert into ncsrvusermac(usermac,username,groupid,starttime,lasttime,moditime,flags) values('%s','%s',%lu,%d,%d,%d,0)", \
	  	     caOut[i],caUsername,lGroupid,time(0),time(0),time(0));
	  }
	  else{
	  	pasDbExecSqlF("update ncsrvusermac set username='%s',groupid=%d,moditime=%d,flags=0 where usermac='%s' ",caUsername,lGroupid,time(0),caOut[i]);
	  }
		}
	}
			                             
        }
        p = fgets(caBuf,10000,fp);
    }
    fclose(fp);
    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入完成!","导入完成!");   	
	return 0;

}

/* 查询区域*/
int ncmWebShowAreaDet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{   
	  utMsgPrintMsg(psMsgHead);
    int i,iNum,iReturn;  
    utPltDbHead *psDbHead;
    char caGroupid[16];
    char caTemp[1024];
    long lSid;
    char caName[128];
    pasDbCursor *psCur;
    char id_t[14028];
    char name_t[24028];
    char caRollevel[32],caDefid[32],caDefarea[128];
    long lDefid,lLen;
    char caOpt[32];
    long lLen_opt,lKey;
    //将上级区域放在hash表里
    char *pHash;
    typedef struct ncmArea_s {
            long lAid;
            char caAname[36];
    } ncmArea;
    ncmArea *psData;
    pHash=(unsigned char *)pasLHashInit(10000,10000,sizeof(struct ncmArea_s),0,4); 
    if(pHash==NULL){
    		return -1;
    }
    // 得到账号ID
    char caOptd[255] = "";
	  dsCltGetSessionValue(1, "opt", UT_TYPE_STRING, sizeof(caOptd)-1, caOptd);
	  printf("caOptd = %s\n", caOptd);
         
	 iReturn = utMsgGetSomeNVar(psMsgHead,2,"rollevel",UT_TYPE_STRING, 10, caRollevel,
	                                        "opt",UT_TYPE_STRING,15,caOpt);
	 
	 memset(caDefid,0,sizeof(caDefid));
	 strcpy(caDefarea,utComGetVar_sd(psShmHead,"NcmProvince",""));
	 if(strlen(caDefarea)>0){
	 	sprintf(caTemp,"select aid from ncmarea where aname='%s' ",caDefarea);
	 	lDefid=0;
	 	printf("caTemp0 = %s\n", caTemp);
	 	pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lDefid);
	 	sprintf(caDefid,"%d",lDefid);
	}
	     
//先将数据放到hash表里
   memset(id_t,0,sizeof(id_t));
   memset(name_t,0,sizeof(name_t));
   
  sprintf(caTemp,"select aid,aname from ncmarea where 1=1 ");   
  sprintf(caTemp+strlen(caTemp)," and length(cast(aid as char(12)))<=4 ");
 
  printf("caTemp11=%s\n",caTemp); 
  psCur=pasDbOpenSql(caTemp,0);
    if(psCur==NULL){
    		free(pHash);
    		return 0;
    }
    lSid=0;
    memset(caName,0,sizeof(caName));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&id_t,
    	                           UT_TYPE_STRING,30,caName);
    while((iReturn == 0||iReturn==1405))
   	{ 
   		 psData = (ncmArea *)pasLHashLookA(pHash,&id_t);  
     	  if(psData){
     	  	strcpy(psData->caAname,caName);
     	  }
     	  
   	  lSid=0;
      memset(caName,0,sizeof(caName));
      iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&id_t,
    	                                 UT_TYPE_STRING,30,caName);
   	}                        
	  pasDbCloseCursor(psCur); 
	      
	      
   memset(id_t,0,sizeof(id_t));
   memset(name_t,0,sizeof(name_t));
   
   sprintf(caTemp,"select aid,aname from ncmarea where 1=1 ");
   
  if(strlen(caDefid)>0){
   	lLen=strlen(caDefid);
   	sprintf(caTemp+strlen(caTemp)," and left(cast(aid as char(12)),%d)='%s' ",lLen,caDefid);
  }
  
  if(strlen(caRollevel)>0){
  	if(atol(caRollevel)==1){
  		lLen=2;
  	}else if(atol(caRollevel)==2){
  		lLen=4;
  		if(strlen(caOptd)==4){
		  		sprintf(caTemp+strlen(caTemp)," and aid='%s' ",caOptd);	
		  }
  	}else if(atol(caRollevel)==3){
  		lLen=6;
  	}else{
  		lLen=0;	
  	}
 //  	lLen=strlen(caRollevel);
   	sprintf(caTemp+strlen(caTemp)," and length(cast(aid as char(12)))=%d ",lLen);
  }
   printf("caTemp=%s\n",caTemp);
   psCur=pasDbOpenSql(caTemp,0);
   if(psCur == NULL) {
       return 0;
   }
   psDbHead = utPltInitDb();     
   iNum=0;
    lSid=0;
    memset(caName,0,sizeof(caName));
   	iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lSid,
   	                                   UT_TYPE_STRING,30,caName);
   	while((iReturn == 0||iReturn==1405))
   	{
      if((iNum<350)){
      	iNum++;
      	if(lSid>=1000){
      		lKey=lSid/100;
      	psData = (ncmArea *)pasLHashLook(pHash,&lKey); 
  //    	printf("lKey=%d\n",lKey);
      	if(psData){
      		sprintf(caTemp,"%s/%s",psData->caAname,caName);
      		strcpy(caName,caTemp);
      	}
      }
      	if(iNum==1){
      		sprintf(id_t,"%d",lSid);
      		sprintf(name_t,"%s",caName);
      	}
      	else{
      		sprintf(id_t+strlen(id_t),"|%d",lSid);
      		sprintf(name_t+strlen(name_t),"|%s",caName);
      	}

//      	utPltPutLoopVarF(psDbHead,"sid",iNum,"%d",lSid);
//      	utPltPutLoopVar(psDbHead,"name",iNum,caName);
      	
      }
       lSid=0;
       memset(caName,0,sizeof(caName));
   	   iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lSid,
   	                                  UT_TYPE_STRING,30,caName);
   	}
    pasDbCloseCursor(psCur);
    free(pHash);
    utPltPutVar(psDbHead,"id",id_t);
    utPltPutVar(psDbHead,"name",name_t);
    utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/iframe_servicedet_ajx.htm");
   
  
    return 0;
}



/* 香港广场显示用户导入next界面  */
int ncmUNext_hk(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
    char caTemp[400];
    FILE *fp;
    int i,iNum,iReturn;
   
    long lSid,lCount,lFlags;
    utPltDbHead *psDbHead;

 	char caFile[256];
 	char imp_file[129]="";
 	char temp_file[129]="";
 	char sqlbuf[1024]="";
 	int  m=0;
 	char caLocal[128],caRemote[128],caType[128];
 	char caText[256],caBuf[16002];
  unsigned id=0;
  char name[36];
  char caGroupname[36],caDispname[36];
  char caGroupid[20];
  unsigned long lGroupid;
 	char *p;
#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
#endif
 utMsgPrintMsg(psMsgHead);
psDbHead = utPltInitDb();
   if(!utFileIsExist("/home/ncmysql/ncsrv/upload")) {
        if(mkdir("/home/ncmysql/ncsrv/upload",777)!=0){
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入","打开文件出错");
        return 0;

        }
}
system("chmod -Rf 777 /home/ncmysql/ncsrv/upload");
//chmod("/home/ncmysql/ncs/upload",777);

	/*取单位*/ 
  sprintf(sqlbuf,"select groupid,groupname,dispname from ncsrvgroup where 1=1 ");
  sprintf(sqlbuf+strlen(sqlbuf)," order by groupname limit 0,2000 ");
	psCur=pasDbOpenSql(sqlbuf,0);
	if(psCur==NULL)
	{
		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入","打开文件出错");
		return 0;
	}
	iReturn=0;
	m=0;
	while((iReturn==0)||(iReturn==1405))
	{
		memset(caGroupname,0,sizeof(caGroupname));
		lGroupid=0;
		memset(caDispname,0,sizeof(caDispname));
		iReturn=pasDbFetchInto(psCur,
		UT_TYPE_LONG,4,&lGroupid,
		UT_TYPE_STRING,30,caGroupname,
		UT_TYPE_STRING,30,caDispname);
			
		if((iReturn==0)||(iReturn==1405))
		{
			m++;
			utPltPutLoopVar(psDbHead,"name",m,caDispname);
			sprintf(caTemp,"%d_%s",lGroupid,caGroupname);
	    utPltPutLoopVar(psDbHead,"id",m,caTemp);
		}
	}
	pasDbCloseCursor(psCur);


	utMsgGetSomeNVar(psMsgHead,2,"fname",UT_TYPE_STRING,  255,imp_file,
	                             "groupid",UT_TYPE_STRING,15,caGroupid);
	   
	sprintf(caTemp,"select dispname,groupname from ncsrvgroup where groupid=%s ",caGroupid);
	memset(caDispname,0,sizeof(caDispname));
	memset(caGroupname,0,sizeof(caGroupname));
	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,63,caDispname,
	                        UT_TYPE_STRING,31,caGroupname);  
	utPltPutVar(psDbHead,"name",caDispname);
	utPltPutVarF(psDbHead,"id","%s_%s",caGroupid,caGroupname);                        
	                                             
     printf("imp_file=%s,groupid=%s\n",imp_file,caGroupid);                 
  if(strlen(imp_file)!=0)
  {
  	p=utStrSkipSpaces(imp_file);
  	p=utStrGetWord(p,temp_file,200,";\n");
  	if((*p)==';')
  	p=utStrGetWord(p+1,temp_file,200,";\n");

  	fp=fopen(temp_file,"r");
  	printf("temp_file=%s\n",temp_file);
  	if(fp == NULL) 
    {
    	utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入","打开文件出错");
     	return 0;
    } 
     	

    p = fgets(caBuf,16000,fp);
    iNum = 0;
    while(p && *p) {
        p = utStrGetWord(p,caTemp,300,",\r\n");
        if(!utStrIsSpaces(caTemp)) {
            iNum++;
            utPltPutLoopVar(psDbHead,"caTemp",iNum,caTemp);
            utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum-1);
            
            if(iNum==1)
            {
            utPltPutVar(psDbHead,"username",caTemp);
            utPltPutVarF(psDbHead,"username_num","%lu",iNum-1);
            }
            if(iNum==2)
            {
            utPltPutVar(psDbHead,"dispname",caTemp);
            utPltPutVarF(psDbHead,"dispname_num","%lu",iNum-1);
            }
            
            if(iNum==3)
            {
            utPltPutVar(psDbHead,"pass",caTemp);
            utPltPutVarF(psDbHead,"pass_num","%lu",iNum-1);
            }
            if(iNum==4)
            {
            utPltPutVar(psDbHead,"openid",caTemp);
            utPltPutVarF(psDbHead,"openid_num","%lu",iNum-1);
            }
            if(iNum==5)
            {
            utPltPutVar(psDbHead,"mark",caTemp);
            utPltPutVarF(psDbHead,"mark_num","%lu",iNum-1);
            }
            if(iNum==6)
            {
            utPltPutVar(psDbHead,"usermac",caTemp);
            utPltPutVarF(psDbHead,"usermac_num","%lu",iNum-1);
            }
        }
        if(*p != ',') {
            break;
        }
        p++;
    }
    fclose(fp);
    
   
}
   utPltPutVar(psDbHead,"fname",temp_file);
   utPltPutVar(psDbHead,"groupid",caGroupid);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/cust_import_next_hk.htm");
    return 0;
}


//导入用户
int ncm_userimp_hk(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	FILE *fp;
	int iReturn;
	char caFile[128]="";
	
	char *p;
	long sid,id;
	char caUrl[68],caName[68],caUrlClass[36];
  char caOut[200][256];
  long i,lCount;
  long lNum=0;
    long lcount=0;
    char caFirst[12]="";
    char caUrlNum[12]="";
    char caNameNum[12]="";
    char caIdNum[12]="";
    char caDefid[16];
    long lUrl,lName,lId;
    long lTime=0;
    char caTime[16]="";
  int tReturn=0;
  int lIdn=0;
	char  caBuf[16000]="";
	char sqlbuf[2048]="";
	char caKeyword[64];
	char *APName;
  char caDeflev[12];
  long lLev=0;
	long sum=0;
	utPltDbHead *psDbHead;
	char caUsernameNum[16],caDispnameNum[16],caPassNum[16],caOpenidNum[16];
	char caMarkNum[16],caGroupname[64],caValidtime[20],caDefpass[20],caDefmark[128];
	char caUsername[32],caDispname[32],caPass[32],caMark[128],caOpenid[32];
	unsigned long lUsernamenum,lDispnamenum,lPassnum,lOpenidnum,lMarknum;
	unsigned long lVailtime,lGroupid,lSid,lVtime,lUsermac;
	char caGroupname_t[64],caTemp[1024];
	char caMac_all[256],caMac[20],caUsermac_num[16];
	char caUsertype[16];
//	psDbHead = utPltInitDb();
utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,13,
					"username",UT_TYPE_STRING,11,caUsernameNum,
					"dispname",UT_TYPE_STRING,10,caDispnameNum,
					"usertype",UT_TYPE_STRING,10,caUsertype,
					"pass",UT_TYPE_STRING,10,caPassNum,
					"openid",UT_TYPE_STRING,10,caOpenidNum,
					"mark",UT_TYPE_STRING,10,caMarkNum,
					"groupname",UT_TYPE_STRING,60,caGroupname,
					"usermac",  UT_TYPE_STRING,10,caUsermac_num,
					"validtime",UT_TYPE_STRING,19,caValidtime,
					"defpass",UT_TYPE_STRING,16,caDefpass,
					"defmark",UT_TYPE_STRING,120,caDefmark,
		      "first",     UT_TYPE_STRING, 11,  caFirst,
		      "fname",   UT_TYPE_STRING, 127, caFile);
		     
	lTime=time(0);
	 lVailtime=0;
	if(strlen(caValidtime)>10){
	  lVailtime=utTimStrToLong("%Y-%m-%d %H:%M:%S",caValidtime);
  }
  if(lVailtime>0&&lVailtime<lTime){
	    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","不能导入已经失效的帐号!","不能导入已经失效的帐号!");   	
	    return 0; 
  }  
	      
	  if(utStrIsSpaces(caUsernameNum)) {
        lUsernamenum = 999;
    }
    else {
        lUsernamenum = atol(caUsernameNum);
    }
    
    if(utStrIsSpaces(caDispnameNum)) {
        lDispnamenum = 999;
    }
    else {
        lDispnamenum = atol(caDispnameNum);
    }
    
    if(utStrIsSpaces(caPassNum)) {
        lPassnum = 999;
    }
    else {
        lPassnum = atol(caPassNum);
    }
    
    if(utStrIsSpaces(caOpenidNum)) {
        lOpenidnum = 999;
    }
    else {
        lOpenidnum = atol(caOpenidNum);
    }
  
    if(utStrIsSpaces(caMarkNum)) {
        lMarknum = 999;
    }
    else {
        lMarknum = atol(caMarkNum);
    }
    if(utStrIsSpaces(caUsermac_num)) {
        lUsermac = 999;
    }
    else {
        lUsermac = atol(caUsermac_num);
    }
  
  
  
    fp = fopen(caFile,"r");
    if(fp == NULL) {
    	 utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入","打开文件出错");
        return 0;
    }
    p = fgets(caBuf,10000,fp);
    if(!utStrIsSpaces(caFirst)) {  /* 忽略第一行  */
        p = fgets(caBuf,10000,fp);
    }
    while(p) {
        pmUtlGetCsvWord2(caBuf,lUsernamenum,caUsername);
        pmUtlGetCsvWord2(caBuf,lDispnamenum,caDispname);
        pmUtlGetCsvWord2(caBuf,lPassnum,caPass);
        pmUtlGetCsvWord2(caBuf,lMarknum,caMark);
  //      pmUtlGetCsvWord2(caBuf,lOpenidnum,caOpenid);
        pmUtlGetCsvWord2(caBuf,lUsermac,caMac_all);
   
   
   
   if(strlen(caPass)==0&&strlen(caDefpass)>0){
   	strcpy(caPass,caDefpass);
   }  
   if(strlen(caMark)==0&&strlen(caDefmark)>0){
   	strcpy(caMark,caDefmark);
  } 
        printf("caGroupname=%s\n",caGroupname);
        
      if(!utStrIsSpaces(caUsername)) {
      	if(strlen(caDispname)==0){
      		strcpy(caDispname,caUsername);
      	}
      	lGroupid=0;
      	memset(caGroupname_t,0,sizeof(caGroupname_t));
        utStrGetSomeWordBy(caGroupname,"_",2,
                                UT_TYPE_LONG,4,&lGroupid,
                                UT_TYPE_STRING,31,caGroupname_t);
      	
			lcount=0;
			sprintf(sqlbuf,"select count(*) from ncsrvuser where username='%s' ",caUsername);
			
	printf("sqlbuf=%s\n",sqlbuf);	
      
			pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lcount);
			                        

			if(lcount==0)
			{
				
				sprintf(caTemp,"%d_%s",lGroupid,caUsername);
				lSid=utMd5Code(caTemp,strlen(caTemp),"ffff");
   	   if(lSid>2000000000) lSid=lSid-2000000000;
   	   if(lSid>2000000000) lSid=lSid-2000000000;
			
				sprintf(sqlbuf,"insert into ncsrvuser (userid,username,dispname,groupname,groupid,usertype,password,mark,addtime,moditime,lasttime,expiretime,flags) values (%lu,'%s','%s','%s',%lu,%d,'%s','%s',%lu,%lu,%lu,%lu,0)",
				 lSid,caUsername,caDispname,caGroupname_t,lGroupid,atol(caUsertype),caPass,caMark,lTime,lTime,lTime,lVailtime);
				 
				
printf("sqlbuf=%s\n",sqlbuf);																																									                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          					                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
				pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
				pasDbCommit(NULL);
			}
			else {
				if(strlen(caPass)>0){
		      sprintf(sqlbuf,"update ncsrvuser set dispname='%s',groupname='%s',usertype=%d,password='%s',mark='%s',moditime=%lu,lasttime=%lu,expiretime=%lu,flags=0 where groupid=%d and username='%s' ",\
		       caDispname,caGroupname_t,atol(caUsertype),caPass,caMark,lTime,lTime,lVailtime,lGroupid,caUsername);
		     }
		     else{
		     	 sprintf(sqlbuf,"update ncsrvuser set dispname='%s',groupname='%s',usertype=%d,mark='%s',moditime=%lu,lasttime=%lu,expiretime=%lu,flags=0 where groupid=%d and username='%s' ",\
		         caDispname,caGroupname_t,atol(caUsertype),caMark,lTime,lTime,lVailtime,lGroupid,caUsername);
		    }
				printf("sqlbuf=%s\n",sqlbuf);
					pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
				  pasDbCommit(NULL);
			
			}  
			long iStatus;
			char caSsid[20];
			memset(caSsid,0,sizeof(caSsid));
			ncSrvSynUserInfo(psShmHead,caGroupname_t,caSsid,caUsername,caPass,NULL,caDispname,atol(caUsertype),0,0,&iStatus);
			
			
	//绑定mac
	if(strlen(caMac_all)>0){
		lNum=sepcharbySH(caMac_all,caOut);
		for(i=0;i<lNum;i++){
	  sprintf(sqlbuf,"select count(*) from ncsrvusermac where usermac='%s' ",caOut[i]);
	  lCount=0;
	  pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lCount);		
	  if(lCount==0){
	  	pasDbExecSqlF("insert into ncsrvusermac(usermac,username,groupid,starttime,lasttime,moditime,flags) values('%s','%s',%lu,%d,%d,%d,0)", \
	  	     caOut[i],caUsername,lGroupid,time(0),time(0),time(0));
	  }
	  else{
	  	pasDbExecSqlF("update ncsrvusermac set username='%s',groupid=%d,moditime=%d,flags=0 where usermac='%s' ",caUsername,lGroupid,time(0),caOut[i]);
	  }
		}
	}
			                             
        }
        p = fgets(caBuf,10000,fp);
    }
    fclose(fp);
    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入完成!","导入完成!");   	
	return 0;

}


int Ncm_User_SetFunName(utShmHead *psShmHead){
	 long iReturn;
	 iReturn = pasSetTcpFunName("ncmUserDrNext", ncmUserDrNext, 0);
	 iReturn = pasSetTcpFunName("ncm_userimp", ncm_userimp, 0);
	 iReturn = pasSetTcpFunName("ncmWebShowAreaDet",ncmWebShowAreaDet, 0);
	 iReturn = pasSetTcpFunName("ncmUNext_hk",ncmUNext_hk, 0);
	 iReturn = pasSetTcpFunName("ncm_userimp_hk",ncm_userimp_hk, 0);
	  
	 
	return 0;
}
