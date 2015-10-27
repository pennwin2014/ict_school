/*   ProAuth                丹尼斯验证网关
*/
#define  PAS_SRCFILE      5000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utoall.h"
#include "pasutl.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <dirent.h>
#include "proauth.h"
#include "pasdb.h"
#include    <signal.h>
#include    <sys/wait.h>
#include <openssl/aes.h>
#include <ctype.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#define NC_SHM_MAXLNK     100
#define NC_LNK_PROCESS      47  /* 进程管理           */

int proauthTcpInit(utShmHead *psShmHead);
int StartSomeProcess(utShmHead *psShmHead);
//void SendOnlineuser(utShmHead *psShmHead);

int ProAuthWritelog(utShmHead *psShmHead,char *table,char *pContent){
	FILE *fp;
	long  lTime=time(0);
	char caPath[128];
	char caFile[128];
	char caYM[20];
	strcpy(caPath,utComGetVar_sd(psShmHead,"Load2DbSrcPath","/home/proauth_denis/log"));
	strcpy(caYM,utTimFormat("%Y%m",lTime));
	sprintf(caFile,"%s/%s_%s.csv",caPath,table,caYM);
	fp=fopen(caFile,"a+");
	if(fp){
		pasFileSetFileLock(fp);
		fwrite(pContent, 1,strlen(pContent) ,fp);
		pasFileSetFileUnLock(fp);
		fclose(fp);
	}
	return 0;
}

//aes加密
unsigned char *utEncryptAes(unsigned char *in,unsigned char *pKey)
{
	 static unsigned char out[2560];
	 unsigned char caKey[2560];
   unsigned char ivec[2560],data[2024];
   char caTemp[2048];
   long iReturn;
   char caRand[32];
   unsigned long lLen,lLen_aes;
   memset(caRand,0,sizeof(caRand));
   
   memset(caTemp,0,sizeof(caTemp));
	 memset(out,0,sizeof(out));
 
	 AES_KEY *key;
   key=(AES_KEY *)malloc(sizeof(AES_KEY));
	 iReturn=AES_set_encrypt_key(pKey,256,key);
   if(iReturn!=0){
   	free(key);
   	return &out[0];
   }
   sprintf(caTemp,"%d",time(0));
   utMd5Ascii16(caTemp,strlen(caTemp),"pronetway",caRand);
     
    memset(ivec,0,sizeof(ivec));
    memset(data,0,sizeof(data));
    memcpy(ivec,caRand,16);
    memcpy(data,ivec,16);
    lLen=strlen(in);
    lLen_aes=(lLen/16+1)*16;  
    memcpy(data+16,in,lLen_aes);
    
    
  
    AES_cbc_encrypt(data, out,lLen_aes+16, key,ivec,AES_ENCRYPT);
    free(key);
   
   //转成十六进制表示形式 
   memset(caTemp,0,sizeof(caTemp));
   pasStrCvtBin2Hex(out,lLen_aes+16,caTemp);
   
   
    memset(out,0,sizeof(out));
    strcpy(out,caTemp);
   
    return &out[0];
}

//aes解密
unsigned char *utDecryptAes(unsigned char *in,unsigned char *pKey)
{
	 static unsigned char out[2560];
	 unsigned char caKey[2560];
   unsigned char ivec[2560],data[2048];
   long iReturn;

	memset(out,0,sizeof(out));

	 AES_KEY *key;
   key=(AES_KEY *)malloc(sizeof(AES_KEY));
	 iReturn=AES_set_decrypt_key(pKey,256,key);
   if(iReturn!=0){
   	free(key);
   	return &out[0];
   }
    memset(ivec,0,sizeof(ivec));
    memset(data,0,sizeof(data));
   iReturn=pasStrCvtHex2Bin(in,data); 
      
 //   printf("iReturnlen=%d\n",iReturn);
    if(iReturn<16){
    	free(key);
    	return &out[0];
    }
    memcpy(ivec,data,16);
   
    AES_cbc_encrypt(data+16, out,iReturn-16, key,ivec,AES_DECRYPT);
   
    free(key);
//    utStrReplaceWith(out,"\x05","\0");
    return &out[0];
}

  //向Radius服务器同步  
int ProAuthSysRadius(utShmHead *psShmHead,char *pPortal,char *pGroup,char *pUser,char *pPass)
{	
	char caHost[32],caPort[16];
	unsigned long lIp;
	long nPort,iReturn,iRet;
	iReturn=-1;
	 strcpy(caHost,utComGetVar_sd(psShmHead,"radius_ip","127.0.0.1"));
   strcpy(caPort,utComGetVar_sd(psShmHead,"radius_port","9995"));
	   lIp = pasIpcvtLong(caHost);
    nPort = htons(atol(caPort));
	    utMsgHead *psMsgHead=NULL;
	   printf("pGroup=%s\n",pGroup);
	            psMsgHead = pasTcpRequest(lIp,nPort,
                         0,        /* CheckSum */
                         0,        /* Encrypt  */
                         "proRadUpdateUserInfo",
                         0,       /* Sid */
                         0,       /* Rid  */
                         30,      /* 超时  */
                         PAS_DEF_KEY,    /* key   */
                         4,
                         "from",    UT_TYPE_STRING,pPortal,
                         "group",   UT_TYPE_STRING,pGroup,
                         "username",UT_TYPE_STRING,pUser,
                         "password",UT_TYPE_STRING,pPass);
            if(psMsgHead) {
            	  int lStatus=0;
                iRet = utMsgGetSomeNVar(psMsgHead,1,
                        "status",UT_TYPE_LONG,4,&lStatus);
                if(lStatus == 0) {
                	iReturn=0;
                }
                else{
                	iReturn=-1;
                }
                 utMsgFree(psMsgHead);
            }
            return iReturn;
}



  //请求验证  
int ProAuthUserAuth(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{	
 //   utPltDbHead *psDbHead;
   	int iReturn;
   	char caTemp[4024];
   	unsigned long lIp;
    char caScode[20];
    char caMac_aes[128];
    char caMac[128];
    unsigned long lGroupid,lUserid;
    char caMobile[200],caDispname[200],caIdno[200],caIdtype[200],caPosition[200];
    char caIp[20],caAtype[20],caUsername[200],caPwd[200];
    char caMobile_aes[200],caIdtype_aes[200],caIdno_aes[200],caDispname_aes[200],caPosition_aes[200];
    char caUsername_aes[200],caPwd_aes[200];
    char caPatient_id[128];
    long lStatus=0;
    long lCount=0;
    char caFcode[20];
    unsigned long long currentTime = pasGetLongTimeId(); //获取时间
   utMsgPrintMsg(psMsgHead);
    char caKey[80];   
    memset(caKey,0,sizeof(caKey));
    memset(caScode,0,sizeof(caScode));
    memset(caFcode,0,sizeof(caFcode));
	iReturn = utMsgGetSomeNVar(psMsgHead,9,
	                "scode",    UT_TYPE_STRING,16,caScode,
	                "mac",    UT_TYPE_STRING,127,caMac_aes,
	                "ip",     UT_TYPE_STRING,15,caIp,
	                "atype",  UT_TYPE_STRING,15,caAtype,
	                "username",UT_TYPE_STRING,127,caUsername_aes,
	                "mobile",  UT_TYPE_STRING,127,caMobile_aes,
	                "pwd",     UT_TYPE_STRING,127,caPwd_aes,
	                "position",UT_TYPE_STRING,127,caPosition_aes,
	                "fcode",  UT_TYPE_STRING,15,caFcode);
	             
	strcpy(caKey,utComGetVar_sd(psShmHead,"key",""));
	 
	strcpy(caMac,utDecryptAes(caMac_aes,caKey)); 
	memset(caUsername,0,sizeof(caUsername));        
  strcpy(caUsername,utDecryptAes(caUsername_aes,caKey)); 
  strcpy(caPwd,utDecryptAes(caPwd_aes,caKey)); 
  strcpy(caPosition,utDecryptAes(caPosition_aes,caKey));
  strcpy(caMobile,utDecryptAes(caMobile_aes,caKey));
  pasLogs(PAS_SRCFILE,1000,"caScode:%s,caMac_aes:%s,caMac:%s,caKey:%s,caUsername_aes:%s,caUsername:%s,caPwd_aes:%s,caPwd:%s,caPosition_aes:%s,caPosition:%s,mobile:%s",\
  caScode,caMac_aes,caMac,caKey,caUsername_aes,caUsername,caPwd_aes,caPwd,caPosition_aes,caPosition,caMobile);
//  psDbHead = utPltInitDb(); 
  lStatus=-1;
  if(strlen(caScode)==0||strlen(caUsername_aes)==0||strlen(caKey)==0||strlen(caPwd_aes)==0){
  	lStatus=2;
  	strcpy(caKey,"");
  }
  else{
  	     
  	if(strcmp(caAtype,"3")==0){
 //帐号验证
         
 	//        sprintf(caTemp,"select patient_id,name,id_no from pat_master_index where patient_id='F00224222' and substr(id_no,-4)='1660' and rownum<2" );
 	       	sprintf(caTemp,"select patient_id,name,id_no from pat_master_index where patient_id='%s' and substr(id_no,-4)='%s' and rownum<2",caUsername,caPwd);
 	
 	        memset(caPatient_id,0,sizeof(caPatient_id));
 	        memset(caDispname,0,sizeof(caDispname));
 	        memset(caIdno,0,sizeof(caIdno));
 	        printf("caTemp=%s\n",caTemp);
	        iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_STRING,20,caPatient_id,
	                                        UT_TYPE_STRING,30,caDispname,
	                                        UT_TYPE_STRING,20,caIdno);	                                        
	        printf("iReturn=%d,patient_id=%s,dispanem=%s,idno=%s\n",iReturn,caPatient_id,caDispname,caIdno);
	                                   
          if(strlen(caPatient_id)>0){
          
          
          	
          	iReturn=ProAuthSysRadius(psShmHead,"tjyy","user",caUsername,caPwd);          	
         printf("radius return=%d\n",iReturn);     	
          	if(iReturn==0){
          	  lStatus=1;
            }
            else{
            	lStatus=-1;
            }
          }
          else{
          	lStatus=-1;
          }
 
    
 //      printf("lStatus=%d\n",lStatus);
     
       
       
       if(lStatus==1){                                     //验证通过
       	  strcpy(caMobile_aes,"");
       	  strcpy(caIdtype_aes,"");
       	  strcpy(caIdno_aes,utEncryptAes(caIdno,caKey));
       	  strcpy(caDispname_aes,utEncryptAes(caDispname,caKey));
       	  strcpy(caPosition_aes,caPosition_aes);
       	  
       	  
          //写验证日志
       currentTime = pasGetLongTimeId(); //获取时间 
       sprintf(caTemp,"%llu\t%s\t%s\t%s\t%s\t%s\n",
					currentTime,caPatient_id,caDispname,caIdno,caMobile,utTimFormat("%Y-%m-%d %H:%M:%S",time(0)));
				ProAuthWritelog(psShmHead,"hisauthlog",caTemp);	   	  
       	  
       	  
       	  
	      }
	      else{
	        strcpy(caMobile_aes,"");
       	  strcpy(caIdtype_aes,"");
       	  strcpy(caIdno_aes,"");
       	  strcpy(caDispname_aes,"");
       	  strcpy(caPosition_aes,"");
	      }
	     
     }

  
  }
  

    pasLogs(PAS_SRCFILE,1000,"status:%d,mobile:%s,idtype:%s,idno:%s,dispname:%s,position:%s",lStatus,caMobile_aes,caIdtype_aes,caIdno_aes,caDispname_aes,caPosition_aes);
    
  char caHtml[4024]; 
  sprintf(caHtml,"{\"status\":\"%d\",\"mobile\":\"%s\",\"idtype\":\"%s\",\"idno\":\"%s\",\"dispname\":\"%s\",\"position\":\"%s\"}",
  lStatus,caMobile_aes,caIdtype_aes,caIdno_aes,caDispname_aes,caPosition_aes);
// printf("caHtml=%s\n",caHtml);
   pasTcpResponse(iFd,psMsgHead,NULL,1,
		                       "text",UT_TYPE_STRING,caHtml);
    
    
//   utPltOutToHtml(iFd,psMsgHead,psDbHead,"auth/proauth_getmac.htm");
  
    return 0;
  }


int proRadAuth(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	  int lStatus=1;
	  char *p,*p1,*p2;
	  char caTemp[256];
	  char caMessage[512];
	  char caPassword[32];
	  char varname[32],value[128];
	  char caPatient_id[32],caDispname[36],caIdno[20];
	  long long currentTime;
	  long iReturn;
	  char caMobile[20];
	  strcpy(caMessage,"\0");
		utMsgOutMsgToLog(PAS_SRCFILE,1000,psMsgHead,"[proRadAuth] \n");
		memset(varname,0,sizeof(varname));
		memset(value,0,sizeof(value));
		p = utMsgGetVar_s(psMsgHead,"cont");
		if(p) {
			 ncUtlGetWordBetween(p,"User-Name = \"","\"",varname,30);
			 ncUtlGetWordBetween(p,"User-Password = \"","\"",value,60);
			/*
			while(1) {
					p = utStrGetWord(p,caTemp,255,"\n");
					if(!utStrIsSpaces(caTemp)) {
						p1 = strstr(caTemp,"=");
						if(*p1 == '=') {
							*p1 = 0;
							p1 = utStrSkipSpaces(p1+1);
							if(*p1 == '"') {
								p2 = p1+1;
								p1 = strstr(p2,"\"");
								if(*p1) {
									*p1 = 0;
								}
							}
							else {
								p2 = p1;
							}
							strcpy(varname,utStrDelSpaces(caTemp));
							strcpy(value,p1);
							pasLogs(PAS_SRCFILE,1000,"Name:[%s]  Value:[%s]",
							      varname,value);
						}
					}
					if(*p) {
						p++;
					}
					else {
						break;
					}
			}	
			*/		
		}
	//帐号验证
	 	      sprintf(caTemp,"select patient_id,name,id_no from pat_master_index where patient_id='%s' and substr(id_no,-4)='%s' and rownum<2",varname,value); 	
 	        memset(caPatient_id,0,sizeof(caPatient_id));
 	        memset(caDispname,0,sizeof(caDispname));
 	        memset(caIdno,0,sizeof(caIdno));
 	        printf("caTemp=%s\n",caTemp);
	        iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_STRING,20,caPatient_id,
	                                        UT_TYPE_STRING,30,caDispname,
	                                        UT_TYPE_STRING,20,caIdno);	                                        
	        printf("iReturn=%d,patient_id=%s,dispanem=%s,idno=%s\n",iReturn,caPatient_id,caDispname,caIdno);
	                                   
            if(strlen(caPatient_id)>0){
		        lStatus=1;
					 currentTime = pasGetLongTimeId(); //获取时间 
			       sprintf(caTemp,"%llu\t%s\t%s\t%s\t%s\t%s\n",
								currentTime,caPatient_id,caDispname,caIdno,caMobile,utTimFormat("%Y-%m-%d %H:%M:%S",time(0)));
							ProAuthWritelog(psShmHead,"hisauthlog",caTemp);	  
						}
						else{
							lStatus=-1;
						} 	  
		      

		utStrAddF(caMessage,"Auth-Type = Local\n");
		utStrAddF(caMessage,"Service-Type = Framed-User\n");
    utStrAddF(caMessage,"Session-Timeout = 7200\n");
    utStrAddF(caMessage,"Idle-Timeout = 300\n");
    
    pasTcpResponse(iFd,psMsgHead,
                    pasGetCurComKey(),                /* 密钥，暂时不用  */
                    3,
                    "status",    UT_TYPE_LONG,  lStatus,
                    "password",  UT_TYPE_STRING,value,
                    "cont",      UT_TYPE_STRING,caMessage);
		return 0;
}






int proauthInitWebFun(utShmHead *psShmHead)
{	
    int iReturn;
    pasSetTcpFunName("ProAuthUserAuth",ProAuthUserAuth, 0);
   iReturn = pasSetTcpFunNameS("proRadAuth",proRadAuth,NULL,0,PAS_CRYPT_TEA);
  }




/* 创建共享内存   */
utShmHead *ncShmCreate(char *caConfFile)
{
    utShmHead *psShmHead;
    int iReturn;
    long  lShmKey,lBaseSize,lMaxLnk;
    long  lPerSize;
    short nMaxPages;
    pasSetConfigFileName(caConfFile);
    lShmKey=utCnfGetValue_l(caConfFile,"ShmKey");
    if(lShmKey<0) {
        lShmKey = 985239L;
    }
    nMaxPages=(short)utCnfGetValue_l(caConfFile,"ShmMaxPages");
    if(nMaxPages<0) nMaxPages = UT_SHMMAXPAGES;
    lBaseSize=utCnfGetValue_l(caConfFile,"ShmBaseSize");
    if(lBaseSize<0) lBaseSize = UT_SHMBASESIZE;
    lPerSize=utCnfGetValue_l(caConfFile,"ShmExtendSize");
    if(lPerSize<0) lPerSize = UT_SHMPERSIZE;
    	
    /* 先判断原来的共享内存，是否正确   */
//    printf("Open Share memory pid=%d sleep 20s\n",getpid());
//    sleep(20);
    
    psShmHead = (utShmHead *)utShmOpen(lShmKey);
    if(psShmHead == NULL) {
        psShmHead = utShmCreate777(lShmKey,nMaxPages,(short)NC_SHM_MAXLNK,
            lBaseSize,lPerSize);
        if(psShmHead == NULL) {
            return NULL;
        }            
    }
    else {
        if(psShmHead->psHead->lBaseSize == lBaseSize) {
             printf(" Share Memory Already exist\n");
            utShmCreateFromOld(psShmHead);
        }
        else {
            utShmRemove(psShmHead);
//            utShmClose(psShmHead);
            psShmHead = utShmCreate777(lShmKey,nMaxPages,(short)NC_SHM_MAXLNK,
                lBaseSize,lPerSize);
        }
    }    
    utCnfSetDebug(psShmHead);
    if(psShmHead == (utShmHead *)NULL) {
        return NULL;
    }
  //  pasUdpFunInit00(); 
    iReturn = utComInit(psShmHead,caConfFile);         /* 初试化程序  */
    if(iReturn < 0) {
        utShmRemove(psShmHead);
        return NULL;
    }
    else {
        return psShmHead;
    }
}



int main(int argc,char **argv)
{
	int lSize;
	utShmHead *psShmHead;
	int iReturn,iDebug,i;
	char caConfFile[64];
	char caVar[32];
	char *p,*pLog,*pNw;
	int iLogLevel,iLogPort,lOffset=14;
  char caDbLogFile[64];
  char caServerip[20];
	char *pLogServer;
  long index;
	
	char caTemp[256];

   pasKillProcess(NULL,"proauth_denis");
   pasSetLogPath("../log");
	  strcpy(caConfFile,"proauth_denis.cnf");
    pasHashRemoveLock();

	if(argc == 2 && (strcasecmp(argv[1],"reset") == 0)) {
  
   }
    else {
        printf(" Start proauth,......\n");
        psShmHead = (utShmHead *)ncShmCreate(caConfFile);
        if(psShmHead == NULL) {
            printf(" Memory Init Error 1111\n");
            exit(9);
        }
    }
   

//   strcpy(caServerip,utComGetVar_sd(psShmHead,"hyip",""));

   

   pLog = utComGetVar_s(psShmHead,"PLATEPATH");
    if(pLog) {
       utPltSetPlatePath(pLog);
   }    
	
	iDebug = utComGetVar_ld(psShmHead,"DebugLevel",1);
	pasSetLogLevel(iDebug);
	
	pLog = utComGetVar_s(psShmHead,"DebugInfo");
	i = 1;
	while(pLog || i < 10) {
	    if(pLog) {
    	    printf("Set DebugInfo:[%s]\n",pLog);
        	pasSetDebugLogInfo(pLog);
        }
    	sprintf(caVar,"DebugInfo%d",i);
	    pLog = utComGetVar_s(psShmHead,caVar);
    	i++;
    }
       utComSetMaxTcpBuf(utComGetVar_ld(psShmHead,"MaxTcpBuffer",1024000L));
 //     iReturn = pasConnect(psShmHead);
//      if(iReturn != 0) {
      //      ncSysLog(NC_LOG_ERROR,"Connect Database Error Sqlcode is %d",iReturn);
//        exit(9);
 //      }
       
       proauthInitWebFun(psShmHead);
//       pasDbClose(NULL);




        iReturn = pasTcpServer(psShmHead,proauthTcpInit);   
        if(iReturn < 0) {
            exit(9);
        }

      pasOnTimeDo(psShmHead);



//    printf("Start Some Process..........................................................\n");
  //   iReturn = StartSomeProcess(psShmHead);
    
    
    utShmClose(psShmHead);
    sleep(1);
    return 0;
}


int proauthTcpInit(utShmHead *psShmHead)
{
    int iReturn;
    setenv("NLS_LANG", "AMERICAN_AMERICA.ZHS16GBK", 1);
   	pasDbConn  *psConn =  pasDbConnect ("tjwifiweb", "Password.1", "192.168.23.120/racmz");
    if(psConn==NULL) {
    //   ncSysLog(NC_LOG_ERROR,"Connect Database Error Sqlcode is %d",iReturn);
    printf("Connect Database Error Sqlcode\n");
   //     exit(9);
      sleep(10);
    }
    return 0;
}


