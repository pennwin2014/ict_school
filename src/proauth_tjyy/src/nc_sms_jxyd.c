#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <sys/time.h>
#include "utoall.h"
#include "pasdb.h"
#include "proauth.h"


//#include "arTcpApi.h"
#include <utoall.h>


//发送短信消息
int smpSendSubmit_jx(utShmHead *psShmHead,char *pNo,char *pMsg){

     authSmsContent *psData;
     psData = (authMobileUser *)utShmHashLookA(psShmHead,PROAUTH_LNK_SMSCONENT,pNo);
     if(psData){
     	strcpy(psData->content,pMsg);
     	strcpy(psData->fws,"ydjx");
     	return 1;
     }
     return -1;
    
}


int SendJxSms(utShmHead *psShmHead)
{
	int iReturn;
	char table[128];
	char caTemp[1024];
	char caDbname[64],caDbpass[64],caDbuser[64],caConnect[128],caYdjxip[32];
	char caContent[256],caMobile[16];
	unsigned long lSid;
	char caScode[20],caFcode[20];
	pasDbCursor *psCur;
	authSmsContent *psData,*psData0;
	pasHashInfo  sHashInfo;
	unsigned char *pHash;
	
	strcpy(caScode,utComGetVar_sd(psShmHead,"servicecode",""));
	strcpy(caFcode,utComGetVar_sd(psShmHead,"fcode",""));
	
	strcpy(caDbname,utComGetVar_sd(psShmHead,"ydjxdbname","mas"));
	strcpy(caDbpass,utComGetVar_sd(psShmHead,"ydjxdbpwd","duanxin"));
  strcpy(caDbuser,utComGetVar_sd(psShmHead,"ydjxdbuser","duanxin"));
	strcpy(caYdjxip,utComGetVar_sd(psShmHead,"ydjxip","218.201.145.91"));

	sprintf(table,"api_mt_%s",caDbuser);
	sprintf(caConnect,"%s@%s",caDbname,caYdjxip);
	
//	printf("caConnect=%s\n",caConnect);
	pasDbConn *pConn=pasDbConnect(caDbuser,caDbpass,caConnect);
	if(pConn){
	printf("connect jxdb success!\n");
 }
 else{
	sleep(10);
	return 0;
 }
 
 /*
    iReturn = utShmHashInit(psShmHead,PROAUTH_LNK_SMSCONENT,1000,1000,sizeof(authSmsContent),0,20);
    if(iReturn != 0) {
        printf("Share Memory alloc Error at ProAuthLoadClientsInfo\n");
        return (-1);
    }
   printf("init success\n");
   */
 
 while(1){
 	/*
    pHash = (unsigned char *)utShmHashHead(psShmHead,PROAUTH_LNK_SMSCONENT);
    if(pHash == NULL) {
           printf("ccccccc\n");
        return (-1);
    }
    else {
        psData = (authSmsContent *)pasHashFirst(pHash,&sHashInfo);
        while(psData) {
       sprintf(caTemp,"insert into %s (sm_id,src_id,mobiles,content,is_wap,url,sm_type,msg_fmt) \
         values(%d,%d,'%s','%s',0,'',0,0)",table,time(0)%1000000,time(0)%1000000,psData->mobile,psData->content);
         printf("caTemp=%s\n",caTemp);
       iReturn=pasDbExecSqlF(caTemp);
 printf("iReturn=%d\n",iReturn);
        psData0 = psData;
                psData= (authSmsContent *)pasHashNextS(&sHashInfo);
                pasHashDel(pHash,psData0);
        }
  }
  */
 //  sprintf(caTemp,"select AUTO_SN,content,mobile from api_mo_%s where mo_time>='%s' ",caDbuser,utTimFormat("%Y-%m-%d",time(0)));
 sprintf(caTemp,"select AUTO_SN,content,mobile from api_mo_%s  ",caDbuser);
//   printf("caTemp=%s\n",caTemp);
   psCur=pasDbOpenSql(caTemp,0);
   if(psCur!=NULL){
   	 lSid=0;
   	 memset(caContent,0,sizeof(caContent));
   	 memset(caMobile,0,sizeof(caMobile));
   	 iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4, &lSid,
   	                                UT_TYPE_STRING,30,caContent,
   	                                UT_TYPE_STRING,15,caMobile);
       while(iReturn==0||iReturn==1405){ 
       	//     printf("caContent=%s,caMobie=%s\n",caContent,caMobile);
       	      
			     psData = (authMobileUser *)utShmHashLookA(psShmHead,PROAUTH_LNK_SMSCONENT,caMobile);
			     if(psData){
			        strcpy(psData->content,caContent);
			        strcpy(psData->fws,"jxyd");
			       
			     }
       	     lSid=0;
				   	 memset(caContent,0,sizeof(caContent));
				   	 memset(caMobile,0,sizeof(caMobile));
				   	 iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4, &lSid,
				   	                                UT_TYPE_STRING,30,caContent,
				   	                                UT_TYPE_STRING,15,caMobile);
       } 
        pasDbCloseCursor(psCur);                        
   }
   else {
   	sleep(10);
   	return 0;
  }
      
     pHash = (unsigned char *)utShmHashHead(psShmHead,PROAUTH_LNK_SMSCONENT);
    if(pHash!= NULL) {
      
        psData = (authSmsContent *)pasHashFirst(pHash,&sHashInfo);
        while(psData) {
 //        printf("mobile=%s\n",psData->mobile);
           if(strcmp(psData->fws,"jxyd")==0){
           	
           	iReturn=ncProAuthUserReg(psShmHead,"6",caScode,psData->mobile,psData->mobile,psData->content,psData->mobile,"","","","","","",caFcode);
           	if(iReturn==1){
           		pasDbExecSqlF("delete from api_mo_%s where mobile='%s' ",caDbuser,psData->mobile);
           	}
//           	printf("iReturn=%s\n",iReturn);
           }
         
         
         
        psData0 = psData;
                psData= (authSmsContent *)pasHashNextS(&sHashInfo);
                pasHashDel(pHash,psData0);
        }
    }
  
  
  
  
 sleep(1);
 }
 pasDbClose(pConn);
 
}