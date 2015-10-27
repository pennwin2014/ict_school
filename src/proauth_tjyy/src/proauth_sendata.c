   
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
#include <openssl/aes.h>
#include <ctype.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>   
//aes加密
unsigned char *utEncryptAes(unsigned char *in,unsigned char *pKey)
{
	 static unsigned char out[2560];
	 unsigned char caKey[256];
   unsigned char ivec[256],data[2024];
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
 //  printf("lCount=%d\n",lCount);
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
   
     //发送验证用户信息 
int ncProAuthUserReg(utShmHead *psShmHead,char *pAtype,char *pServicecode,char *pMobile,char *pUsername,char *pPwd,char *pName,char *pIdtype,char *pIdno,char *pSex,char *pPosition,char *pIntime,char *pOuttime,char *pFcode)
{	
    utPltDbHead *psDbHead;
    utMsgHead *psMsgHead2;
   	int iReturn;
   	char caTemp[4024];
    char caHost[256],caUrl[256];
    char caStyle[12],caHtml[1024];
    char caKey[128],caKey_bin[128];
    char caMac_aes[256];
    long lStatus;
    char caStatus[32];
    char caAtype_aes[200],caMobile_aes[200],caName_aes[200],caIdtype_aes[100],caIdno_aes[200],caSex_aes[100],caPosition_aes[200];
    char caPort[12];
    char caAcode_aes[200],caSmsmark_aes[200];
    char caUsername_aes[200],caPwd_aes[200];
    char caPsn[32];
    char *pBuf;
    unsigned long lIp;
    unsigned short nPort;
    memset(caKey,0,sizeof(caKey));
    strcpy(caKey,utComGetVar_sd(psShmHead,"proauthkey",""));
 
   // utMsgPrintMsg(psMsgHead);
   
   strcpy(caHost,utComGetVar_sd(psShmHead,"centerauthip","127.0.0.1"));
   strcpy(caPort,utComGetVar_sd(psShmHead,"centerauthport","7199"));
   
    lIp = pasIpcvtLong(caHost);
    nPort = htons(atol(caPort));
   
   
   strcpy(caAtype_aes,utEncryptAes(pAtype,caKey));
   strcpy(caMobile_aes,utEncryptAes(pMobile,caKey));
   strcpy(caPwd_aes,utEncryptAes(pPwd,caKey));
   strcpy(caUsername_aes,utEncryptAes(pUsername,caKey));
   
   strcpy(caName_aes,utEncryptAes(pName,caKey));
 
   strcpy(caIdtype_aes,utEncryptAes(pIdtype,caKey));
   strcpy(caIdno_aes,utEncryptAes(pIdno,caKey));
   strcpy(caSex_aes,utEncryptAes(pSex,caKey));
   strcpy(caPosition_aes,utEncryptAes(pPosition,caKey));
  // printf("pOuttime=%s\n",pOuttime);

  psMsgHead2 = pasTcpRequest(lIp,nPort,
                         0,        /* CheckSum */
                         0,        /* Encrypt  */
                         "ProAuthUserReg",
                         0,       /* Sid */
                         0,       /* Rid  */
                         60,      /* 超时  */
                         NULL,    /* key   */
                         13,
                        "scode",UT_TYPE_STRING, pServicecode,
		                    "atype",UT_TYPE_STRING, caAtype_aes,
		                    "mobile",UT_TYPE_STRING,caMobile_aes,
		                    "username",UT_TYPE_STRING,caUsername_aes,
		                    "pwd",UT_TYPE_STRING,caPwd_aes,
		                    "name",UT_TYPE_STRING,caName_aes,
		                    "idtype",UT_TYPE_STRING,caIdtype_aes,
		                    "idno",UT_TYPE_STRING,caIdno_aes,
		                    "sex", UT_TYPE_STRING,caSex_aes,
		                    "position",UT_TYPE_STRING,caPosition_aes,
		                    "intime",UT_TYPE_STRING,pIntime,
		                    "outtime",UT_TYPE_STRING,pOuttime,
		                    "fcode",UT_TYPE_STRING,pFcode);
//printf("aaaaaaaaa\n");
   if(psMsgHead2){
   	char caHtml[1024];
   	iReturn = utMsgGetSomeNVar(psMsgHead2,1,"text",UT_TYPE_STRING,200,caHtml);
   	utMsgFree(psMsgHead2);
   	 ncUtlGetWordBetween(caHtml,"status\":\"","\"",caStatus,10);
   
       if(strcmp(caStatus,"1")==0){
      
        lStatus=1;      	   
       }
       else if(strcmp(caStatus,"2")==0){
       	lStatus=2;
      }
      else {
      	lStatus=atol(caStatus);
      }
   	  return lStatus;
   	
   	
   	
 //  	printf("caHtml=%s\n",caHtml);
   }

    return -1;
  }