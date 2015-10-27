#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utoall.h"
#include "pasutl.h"
#include "pasdb.h"
#include "pasutl.h"
#include "ncportal.h"
#include "ncportalweb.h"
#include "utoplt01.h"
#define NCM_SRCFILE 8003

static char *ncmutlremovelastchar2(char p[2048])
{
    static char p1[2048] = "";
    strncpy(p1,p,strlen(p)-1);
    return (char *)p1;	
}


ncPortalOnline *ncmGetOnlineUserByMac(utShmHead *psShmHead,char *pMac)
{
    ncPortalOnline *online;
    pasHashInfo sHashInfo;
    uchar *pHash;
    int iCount = 0, allCount = 0;
    ncPortalUser    *psUser;
    long lBase;
    ncPortalOnline  *psOnline;
    pHash =  (unsigned char *)utShmHashHead(psShmHead,NCSRV_LNK_ONLINE);
//    printf("pMac = %s \n",pMac );
    if(pHash == NULL) 
    {
        return NULL;
    }

    lBase = utShmGetBaseAddr(psShmHead);
    psOnline = (ncPortalOnline *)pasHashFirst(pHash,&sHashInfo);
    while(psOnline) 
    {
        psUser = (ncPortalUser *) ( (char *)psOnline->psUser + lBase);
       if(strlen(psOnline->caName) > 0)              //只显示有效信息 并且已登录用户
       {
       		if (strcasecmp((char *)pasCvtMac(psOnline->caMac),pMac) == 0)
       		{
       			   strcpy(online->caName,psOnline->caName);
       		  	 online->lSip = psOnline->lSip;
       		  	 break;
       		}	  
       }
       
        psOnline = (ncPortalOnline *)pasHashNextS(&sHashInfo);
    }	
	
	  return online;
}
int toupper(int c){   
	 if( c >= 'a' && c <= 'z' )   
	 	     c -= ('a'-'A');   
	 	      return c;
}

/*
发送请求
https://192.168.20.168/pronetway/getuserinfo?cmd=login&sitecode=1101020314052&timestamp=1406111558&mac=84:7a:88:8e:6e:47|64:09:80:e1:d1:f7|cc:07:ab:2d:0f:8e&checksum=df013dee54a6e4b4302b07ad467f1928

*/
//启明星辰接口
int ncmcomTaiBaoInterface(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    int iCount = 0, allCount = 0;
    ncPortalUser    *psUser;
    ncPortalOnline  *psOnline;
    long lBase;
    char caSdate[32] = "" , keyword[64] = "",flow_flag[4] = "", caDel[10] ="", caSesid[1024] = "", opt[256] = "";
    int iReturn;
    char cmd[32] = "", sitecode[16] = "", macall[5120] = "", account[32] = "", checksum[1024] = "", timestamp[32] = "";
    char *pBuf = "";
    char caTemp[1024] = "", macnew[2048] = "", newmac[2048] = "", myJson[3036] = "", cTmp[128] = "", myKey[32] = "", authsum[32] = "";
    char tmpch[48] = "", dstEchostr[48] = "", dst[48] = "";
    int tNum = 0, iNum = 0, iRet = 0, i = 0;
    utMsgPrintMsg(psMsgHead);
    iRet = utMsgGetSomeNVar(psMsgHead,5, "cmd",       UT_TYPE_STRING, sizeof(cmd) - 1,      cmd,
                                         "sitecode",  UT_TYPE_STRING, sizeof(sitecode) - 1, sitecode,
                                         "mac",       UT_TYPE_STRING, sizeof(macall) - 1,   macall,
                                         "timestamp", UT_TYPE_STRING, sizeof(timestamp) - 1,timestamp,
                                         "checksum",  UT_TYPE_STRING, sizeof(checksum) - 1, checksum);
//    strcpy(timestamp,"1406111558");
    strcpy(myKey,utComGetVar_sd(psShmHead,"QmxcKey",  "portal"));
    strcpy(cTmp,timestamp);
    strcat(cTmp,myKey);
    printf("cTmp = %s \n",cTmp);
    strcpy(authsum,(char *)ncmMd5char(cTmp,16));
//    utMd5Ascii22(cTmp,strlen(cTmp),NULL,authsum);
    printf("authsum = %s \n",authsum);
//    for(i=0;*pstr;i++)     
//     dst[i] = toupper(*pstr++);
    for(i=0;i < strlen(authsum);i++)     
     dst[i] = tolower(authsum[i]); 
    dst[i] = '\0';
    printf("%s\n",dst);
    
    if (strcmp(dst,checksum) == 0)   //校验
    {	
	    //处理macall
	    if (strlen(macall) > 0)
	    {
	    	 pBuf = strdup(macall);
	    	 printf("pBuf = %s \n",pBuf);
	    	 strcpy(myJson,"{");
	    	 while (pBuf)
	    	 {
	    	 	   memset(caTemp,0,sizeof(caTemp));
	    	 	   pBuf = utStrGetWord(pBuf,caTemp,1000,"|");
	    	 	   printf("caTemp = %s \n",caTemp);
			    	 if (tNum == 0)
			    	 {	
			    	 	 if (strlen(caTemp) > 0)       //生成Json字符串
			    	 	 {
			    	 	    strcat(myJson,"{");	
			    	 	    psOnline = ncmGetOnlineUserByMac(psShmHead,(char *)caTemp);
			    	 	    strcat(myJson,"mac:");
			    	 	    strcat(myJson,caTemp);
			    	 	    strcat(myJson,",ip:");	
			    	 	    strcat(myJson,utComHostIp(htonl(psOnline->lSip)));
			    	 	    strcat(myJson,",mobile:");	
			    	 	    if(strcasecmp(caTemp,"00:0c:29:08:69:b8") == 0)    //张晓 
			    	 	    {
			    	 	    	strcat(myJson,"13716861819");
			    	 	    }	
			    	 	    else
			    	 	    strcat(myJson,psOnline->caName);
			    	 	    if (strlen(psOnline->caName) != 11)
			    	 	    {
			    	 	       	strcat(myJson,",type:0");	
			    	 	    }
			    	 	    else
			    	 	    {
			    	 	    	  strcat(myJson,",type:1");	
			    	 	    }		
			    	 	    strcat(myJson,"}");	
			    	 	 }	
//				    	 strcpy(macnew,"'");
//				    	 strcat(macnew,caTemp);
//				    	 strcat(macnew,"'");
//				    	 strcat(macnew,",");
			    	 }
			    	 else
			    	 {
			    	 	 if (strlen(caTemp) > 0)
			    	 	 {
			    	 	    strcat(myJson,",{");	
			    	 	    psOnline = ncmGetOnlineUserByMac(psShmHead,(char *)caTemp);
			    	 	    strcat(myJson,"mac:");
			    	 	    strcat(myJson,caTemp);
			    	 	    strcat(myJson,",ip:");	
			    	 	    strcat(myJson,utComHostIp(htonl(psOnline->lSip)));
			    	 	    strcat(myJson,",mobile:");	
			    	 	    strcat(myJson,psOnline->caName);
			    	 	    if (strlen(psOnline->caName) != 11)
			    	 	    {
			    	 	       	strcat(myJson,",type:0");	
			    	 	    }
			    	 	    else
			    	 	    {
			    	 	    	  strcat(myJson,",type:1");	
			    	 	    }			
			    	 	    strcat(myJson,"}");	
			    	 	 }	
//			    	   strcat(macnew,"'");
//				    	 strcat(macnew,caTemp);
//				    	 strcat(macnew,"'");
//				    	 strcat(macnew,","); 	
			    	 }	
			    	 tNum++;
			    	 if(*pBuf != '|') {
		            break;
		         }
		         pBuf++;
	    	 }
	    	 strcat(myJson,"}");
	    }	
	    
//	    printf("macnew = %s \n",macnew);
//	    strcpy(newmac,(char *)ncmutlremovelastchar2(macnew));         //数据库查询
	    printf("myJson = %s \n",myJson);
	    if (strcmp(cmd,"login") == 0)
	    {
	    	//应答
	    	utComTcpResponse(iFd,psMsgHead,1,
			                       "text",UT_TYPE_STRING,myJson);
	    	
	    }
    }
//    if (pBuf)
//    {
//        free(pBuf);	
//    	  pBuf = NULL;
//    }	
    
    return 0;
}

//河南太保接口
int ncmcomHNTaiBaoInterface(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    int iCount = 0, allCount = 0;
    ncPortalUser    *psUser;
    ncPortalOnline  *psOnline;
    pasDbCursor *psCur;
    long lBase;
    char caSdate[32] = "" , keyword[64] = "",flow_flag[4] = "", caDel[10] ="", caSesid[1024] = "", opt[256] = "";
    int iReturn;
    char cmd[32] = "", groupcode[32] = "", macall[5120] = "", account[32] = "", checksum[1024] = "", timestamp[32] = "";
    char *pBuf = "";
    char caTemp[1024] = "", macnew[2048] = "", newmac[2048] = "", myJson[3036] = "", cTmp[5028] = "", myKey[32] = "", authsum[32] = "";
    char tmpch[48] = "", dstEchostr[48] = "", dst[48] = "", checkdst[48] = "";
    int tNum = 0, iNum = 0, iRet = 0, i = 0;
    char username[48] = "", dispname[48] = "", ssid[48] = "", passwd[48] = "", maxmac[48] = "", groupname[48] = "";
    char caUsername[48] = "", caDispname[48] = "", caGname[5120] = "", caPasswd[48] = "";
    char scode[4] = "", caTemp2[64] = "", caLeft[32] = "", pCode[32] = "", caLeftfir[32] = "", caTempd[1024] = "", gCode[32] = "";
    int maxcode = 0, j = 0, groupid = 0, lcount = 0;
    char *q = NULL;
    utMsgPrintMsg(psMsgHead);
    iRet = utMsgGetSomeNVar(psMsgHead,11,"cmd",        UT_TYPE_STRING, sizeof(cmd) - 1,       cmd,
                                         "groupcode",  UT_TYPE_STRING, sizeof(groupcode) - 1, groupcode,
                                         "username",   UT_TYPE_STRING, sizeof(caUsername) - 1,caUsername,
                                         "dispname",   UT_TYPE_STRING, sizeof(caDispname) - 1,caDispname,
                                         "ssid",       UT_TYPE_STRING, sizeof(ssid) - 1,      ssid,
                                         "passwd",     UT_TYPE_STRING, sizeof(caPasswd) - 1,  caPasswd,
                                         "maclist",    UT_TYPE_STRING, sizeof(macall) - 1,    macall,
                                         "maxmac",     UT_TYPE_STRING, sizeof(maxmac) - 1,    maxmac,
                                         "groupname",  UT_TYPE_STRING, sizeof(caGname) - 1,   caGname,
                                         "timestamp",  UT_TYPE_STRING, sizeof(timestamp) - 1, timestamp,
                                         "checksum",   UT_TYPE_STRING, sizeof(checksum) - 1,  checksum);

    if(strlen(caUsername)>0){
    	 strcpy(username,pasCvtGBK(2,caUsername,caTemp,sizeof(caUsername)-1));
    }
    if(strlen(caDispname)>0){
    	 strcpy(dispname,pasCvtGBK(2,caDispname,caTemp,sizeof(caDispname)-1));
    }
    if(strlen(caPasswd)>0){
    	 strcpy(passwd,pasCvtGBK(2,caPasswd,caTemp,sizeof(caPasswd)-1));
    }
    if(strlen(caGname)>0){
    	 strcpy(groupname,pasCvtGBK(2,caGname,caTemp,sizeof(caGname)-1));
    }
    strcpy(myKey,utComGetVar_sd(psShmHead,"TaiBaoKey",  "Portal123"));
    
    if (strcmp(cmd, "add") == 0 || strcmp(cmd, "update") == 0)
    {
     	  sprintf(cTmp,"%s%s%s%s%s%s%s%s%s",cmd,groupcode,username,dispname,ssid,macall,maxmac,groupname,timestamp);
     	  strcpy(authsum,(char *)ncmMd5char(cTmp,16));
     	  for(i=0;i < strlen(authsum);i++)     
		     	dst[i] = tolower(authsum[i]); 
		    dst[i] = '\0';
    }
    else
    {
    	  sprintf(cTmp,"%s%s%s%s",cmd,groupcode,username,timestamp);
    	  strcpy(authsum,(char *)ncmMd5char(cTmp,16));
    	   for(i=0;i < strlen(authsum);i++)     
		     	checkdst[i] = tolower(authsum[i]); 
		    checkdst[i] = '\0';
    }	
    
    printf("dst = %s   checkdst = %s\n",dst,checkdst);
    

	  tNum = 0;  
	  char grName[10][64];
	  for(i = 0;i < 10; i++)
	  {
	     memset(grName[i],0,sizeof(grName[i]));	
	  }
	  //    https://192.168.20.168:443/pronetway/memInterface?cmd=add&groupcode=410000010200000001&username=wupeng2&dispname=吴鹏2&ssid=ICPIC&passwd=password&maclist=&maxmac=5&groupname=河南分公司|河南分公司本级|信息技术部&timestamp=1407821741&checksum=f029405328c2e2a39c4650f70edca033                                     

	  if (strcmp(cmd,"add") == 0)
	  {
//	    	if (strcmp(dst,checksum) == 0)   //校验
//   		{	
    			  printf("groupname = %s \n",groupname);
    			  
					  sprintf(caTemp,"select groupcode from ncmshopgtype where groupname='%s' ",utComGetVar_sd(psShmHead,"NcmCustName", "太保"));
		        printf("caTemp = %s \n",caTemp);
						pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(scode) - 1,scode);
						memset(caLeftfir,0,sizeof(caLeftfir));
						memset(caTemp2,0,sizeof(caTemp2));
					  if(strlen(groupname) != 0){
					  		q =	utStrSkipSpaces(groupname);
					  		while(q && *q){
					  				q = utStrGetWord(q,caTemp2,48,"|");
					  				strcpy(grName[tNum],caTemp2);
					          if (strlen(grName[tNum]) > 0)
						        {
						        	  if (tNum == 0)
						        	  {
						        	  	      sprintf(caTemp,"select count(*) from ncmautopri where groupname='%s' and shopcode = '%s' and parentcode = '' ",grName[tNum],scode);
									 		        	printf("caTemp = %s \n",caTemp);
												        pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(long),&lcount);
												        //上面查出groupcode赋给left
												        if (lcount == 0)
												        {
												        	maxcode = 0;
												        	memset(groupcode,0,sizeof(groupcode));
												        	sprintf(caTemp,"select max(groupcode)+1 from ncmautopri where shopcode = '%s' and parentcode = '' ",scode);
												          pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(long),&maxcode);
												          if (maxcode == 0)   maxcode = 1;
												          sprintf(groupcode,"%02d",maxcode);
												          pasDbExecSqlF("insert into ncmautopri(groupname,groupcode,parentcode,shopcode,systime,flag) values ('%s','%s','%s','%s',%lu,0)",
				                                            grName[i],groupcode,pCode,scode,time(0));
												          strcpy(caLeftfir,groupcode);
												        }	
												} 
												else
												{		
								        	  memset(pCode,0,sizeof(pCode));
								        	  sprintf(caTemp,"select groupcode from ncmautopri where groupname='%s' and shopcode = '%s' and length(groupcode) = 2 ",grName[0],scode);	
								        	  printf("0caTemp = %s \n",caTemp);
										        pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(pCode) - 1,pCode);
								        	  for (i = 1; i < tNum; i++)
								        	  {
														    sprintf(caTemp,"select groupcode from ncmautopri where groupname='%s' and shopcode = '%s' and length(groupcode) = %d and parentcode = '%s' ",grName[i],scode,4*i,pCode);
												        
												        memset(pCode,0,sizeof(pCode));
												        pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(pCode) - 1,pCode);
								        	  }
								        	  sprintf(caTemp,"select count(*),groupcode from ncmautopri where groupname='%s' and shopcode = '%s' and parentcode = '%s'  group by groupcode",grName[tNum],scode,pCode);
//				                    printf("2caTemp = %s \n",caTemp);
										        lcount = 0;
										        memset(gCode,0,sizeof(gCode));
										        pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(long),&lcount,
										                                UT_TYPE_STRING,sizeof(gCode) - 1,gCode);
										        if (lcount == 0)
												    {
												        	maxcode = 0;
												        	memset(groupcode,0,sizeof(groupcode));
												        	sprintf(caTemp,"select max(right(groupcode,2))+1 from ncmautopri where shopcode = '%s' and parentcode = '%s' ",scode,pCode);
												          pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(long),&maxcode);
												          if (maxcode == 0)   maxcode = 1;
												          sprintf(groupcode,"%s%02d",pCode,maxcode);
												          pasDbExecSqlF("insert into ncmautopri(groupname,groupcode,parentcode,shopcode,systime,flag) values ('%s','%s','%s','%s',%lu,0)",
				                                            grName[i],groupcode,pCode,scode,time(0));
												          strcpy(caLeftfir,groupcode);
												    }	                         
						                else
										        {
										        	    strcpy(caLeft,gCode);
										        }	
						        	  }
						        }
					          
					          tNum++;
					          q++;	
					  		}
					  }
					  if (strlen(caLeftfir) > 0)
		        {
		           strcpy(caLeft,caLeftfir);	
		        }	
		        printf("caLeft = %s scode = %s\n",caLeft,scode);
		        char tmpch[36] = "";
		        sprintf(tmpch,"%s%s",ncmWebAutoDefGet16S(psShmHead,scode),caLeft);
		        strcpy(groupname,tmpch);
		        
		        
    		    char *pSsid = NULL;  //其它类似
			 		  char *pUsername = NULL;
			 		  char *pPassword = NULL;
			 		  char *pGroupname = NULL;
			 		  char *pDisp = NULL;
			      int  iStatus = 0;
			      unsigned long lusertype = 0;
			      unsigned long lflags = 0;
			      unsigned long lMaxMac = 0;
			      if (strlen(maxmac) > 0)
			      {
			       		lMaxMac = atol(maxmac);
			      }
			      else
			      {
			      	  char caMaxmac[11] = "";
			      	  strcpy(caMaxmac,utComGetVar_sd(psShmHead,"NcmMaxUMac","1"));
			      		lMaxMac = atol(caMaxmac);	
			      }
			      pSsid = strdup(ssid);
			      pUsername = strdup(username);
			      pPassword = strdup(passwd);
			      pGroupname = strdup(groupname);
			      if(strlen(dispname)==0){
			      		pDisp = strdup(username);	
			      }else{
			      		pDisp = strdup(dispname);	
			      }
			      printf("pSsid = %s, pUsername = %s, pPassword = %s, pGroupname = %s, pDisp = %s, lusertype = %d, lflags = %d, lMaxmac = %d\n", pSsid,pUsername,pPassword,pGroupname,pDisp,lusertype,lflags,lMaxMac);
			      ncPortalUser *result = NULL;
			      result = ncSrvSynUserInfo(psShmHead,pGroupname,pSsid,pUsername,pPassword,NULL,pDisp,lusertype,lMaxMac,lflags,&iStatus);
			      if(result){
			      		printf("status = %d\n", result->cStatus);//0正常
			      }
			      if(pSsid){
				         free(pSsid);
				         pSsid = NULL;
			      }
			      if(pUsername){
				         free(pUsername);
				         pUsername = NULL;
			      }
			      if(pPassword){
				         free(pPassword);
				         pPassword = NULL;
			      }
			      if(pGroupname){
				         free(pGroupname);
				         pGroupname = NULL;
			      }
			      if(pDisp){
				         free(pDisp);
				         pDisp = NULL;
			      }
			      
			      if(strlen(macall) != 0){
					  		q =	utStrSkipSpaces(macall);
					  		while(q && *q){
					  				q = utStrGetWord(q,caTemp2,18,"|");
					  				memset(caTempd,0,sizeof(caTempd));
					  				sprintf(caTempd,"insert into ncsrvusermac (usermac, username, groupid, starttime, lasttime, moditime, flags) values ('%s', '%s', %lu, %lu, %lu, %lu, %lu)", caTemp2,username,0,time(0),time(0),time(0),0);
						 				printf("caTempd = %s\n", caTempd);
								 		pasDbExecSqlF(caTempd);
								 		char *aUsername = NULL;
								 		char aUsermac[6];
								 		unsigned long aGroupid = 0;
								 		aUsername = strdup(username);
//								 		aUsermac = strdup(caTemp2);
								 		pasCvtMacI((char *)caTemp2,aUsermac);
								 		printf("aGroupid = %d, aUsername = %s, aUsermac = %s\n", aGroupid,aUsername,pasCvtMac(aUsermac));
								 		ncSrvAddBindMac(psShmHead,aGroupid,aUsername,aUsermac);
								 		if(aUsername){
								         free(aUsername);
								         aUsername = NULL;
							      }
					  				q++;	
					  		}
					  }
			      
			      //单位
		    	  unsigned int lSid = 0;
		    	  lSid=utMd5Code(groupname,strlen(groupname),"ffff");
			   	  if(lSid>2000000000) lSid=lSid-2000000000;
			   	  if(lSid>2000000000) lSid=lSid-2000000000;
		        pasDbExecSqlF("insert into ncsrvgroup(groupid,groupname,dispname,flags) values (%lu,'%s','%s',2)",lSid,groupname,caTemp2);
		        printf("insert into ncsrvgroup(groupid,groupname,dispname,flags) values (%lu,'%s','%s',2) \n",lSid,groupname,caTemp2);
		        ncSrvSynUser2Rad(psShmHead);
		    	  sprintf(myJson,"{cmd:add,status:success,timstamp:%d}",time(0));
		    	  
		    	  
//    	  }	
	  }
	  int iStatus = 0;
	  ncPortalUser *result = NULL;
//  https://192.168.20.168/pronetway/memInterface?cmd=update&groupcode=410000010200000001&username=wupeng8&dispname=吴鹏哈&ssid=ICPIC&passwd=password&maclist=11:22:33:44:55:66|31:12:13:14:15:16|41:22:23:24:25:26&maxmac=5&groupname=河南分公司|河南分公司本级|信测部&timestamp=1407821741&checksum=b5eef132a7743ac3029c116d4114e4ca
	  if (strcmp(cmd,"update") == 0)
	  {
//	    	if (strcmp(dst,checksum) == 0)   
//    		{	
    			  //删除
    			  ncSrvDelUserMacByName(psShmHead,0,username); 
    		    result = ncSrvSynUserInfo(psShmHead,"",utComGetVar_sd(psShmHead,"NcmMySSID",  "ICPIC"),username,"","","",0,0,9,&iStatus);
						if(result){
								printf("status = %d\n", result->cStatus);
						}
    			  //插入
    		    printf("groupname = %s \n",groupname);
    			  
					  sprintf(caTemp,"select groupcode from ncmshopgtype where groupname='%s' ",utComGetVar_sd(psShmHead,"NcmCustName", "太保"));
		        printf("caTemp = %s \n",caTemp);
						pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(scode) - 1,scode);
						memset(caLeftfir,0,sizeof(caLeftfir));
						memset(caTemp2,0,sizeof(caTemp2));
					  if(strlen(groupname) != 0){
					  		q =	utStrSkipSpaces(groupname);
					  		while(q && *q){
					  				q = utStrGetWord(q,caTemp2,48,"|");
					  				strcpy(grName[tNum],caTemp2);
					          if (strlen(grName[tNum]) > 0)
						        {
						        	  if (tNum == 0)
						        	  {
						        	  	      sprintf(caTemp,"select count(*) from ncmautopri where groupname='%s' and shopcode = '%s' and parentcode = '' ",grName[tNum],scode);
									 		        printf("caTemp = %s \n",caTemp);
												        pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(long),&lcount);
												        //上面查出groupcode赋给left
												        if (lcount == 0)
												        {
												        	maxcode = 0;
												        	memset(groupcode,0,sizeof(groupcode));
												        	sprintf(caTemp,"select max(groupcode)+1 from ncmautopri where shopcode = '%s' and parentcode = '' ",scode);
												          pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(long),&maxcode);
												          if (maxcode == 0)   maxcode = 1;
												          sprintf(groupcode,"%02d",maxcode);
												          pasDbExecSqlF("insert into ncmautopri(groupname,groupcode,parentcode,shopcode,systime,flag) values ('%s','%s','%s','%s',%lu,0)",
				                                            grName[i],groupcode,pCode,scode,time(0));
												          strcpy(caLeftfir,groupcode);
												        }	
												} 
												else
												{		
								        	  memset(pCode,0,sizeof(pCode));
								        	  sprintf(caTemp,"select groupcode from ncmautopri where groupname='%s' and shopcode = '%s' and length(groupcode) = 2 ",grName[0],scode);	
								        	  printf("0caTemp = %s \n",caTemp);
										        pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(pCode) - 1,pCode);
								        	  for (i = 1; i < tNum; i++)
								        	  {
														    sprintf(caTemp,"select groupcode from ncmautopri where groupname='%s' and shopcode = '%s' and length(groupcode) = %d and parentcode = '%s' ",grName[i],scode,4*i,pCode);
												        
												        memset(pCode,0,sizeof(pCode));
												        pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(pCode) - 1,pCode);
								        	  }
								        	  sprintf(caTemp,"select count(*),groupcode from ncmautopri where groupname='%s' and shopcode = '%s' and parentcode = '%s'  group by groupcode",grName[tNum],scode,pCode);
//				                    printf("2caTemp = %s \n",caTemp);
										        lcount = 0;
										        memset(gCode,0,sizeof(gCode));
										        pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(long),&lcount,
										                                UT_TYPE_STRING,sizeof(gCode) - 1,gCode);
										        if (lcount == 0)
												    {
												        	maxcode = 0;
												        	memset(groupcode,0,sizeof(groupcode));
												        	sprintf(caTemp,"select max(right(groupcode,2))+1 from ncmautopri where shopcode = '%s' and parentcode = '%s' ",scode,pCode);
												          pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(long),&maxcode);
												          if (maxcode == 0)   maxcode = 1;
												          sprintf(groupcode,"%s%02d",pCode,maxcode);
												          pasDbExecSqlF("insert into ncmautopri(groupname,groupcode,parentcode,shopcode,systime,flag) values ('%s','%s','%s','%s',%lu,0)",
				                                            grName[i],groupcode,pCode,scode,time(0));
												          strcpy(caLeftfir,groupcode);
												    }	                         
						                else
										        {
										        	    strcpy(caLeft,gCode);
										        }	
						        	  }
						        }
					          
					          tNum++;
					          q++;	
					  		}
					  }
					  if (strlen(caLeftfir) > 0)
		        {
		           strcpy(caLeft,caLeftfir);	
		        }	
		        printf("caLeft = %s scode = %s\n",caLeft,scode);
		        char tmpch[36] = "";
		        sprintf(tmpch,"%s%s",ncmWebAutoDefGet16S(psShmHead,scode),caLeft);
		        strcpy(groupname,tmpch);
		        
		        
    		    char *pSsid = NULL;  //其它类似
			 		  char *pUsername = NULL;
			 		  char *pPassword = NULL;
			 		  char *pGroupname = NULL;
			 		  char *pDisp = NULL;
			      int  iStatus = 0;
			      unsigned long lusertype = 0;
			      unsigned long lflags = 0;
			      unsigned long lMaxMac = 0;
			      if (strlen(maxmac) > 0)
			      {
			       		lMaxMac = atol(maxmac);
			      }
			      else
			      {
			      	  char caMaxmac[11] = "";
			      	  strcpy(caMaxmac,utComGetVar_sd(psShmHead,"NcmMaxUMac","1"));
			      		lMaxMac = atol(caMaxmac);	
			      }
			      pSsid = strdup(ssid);
			      pUsername = strdup(username);
			      pPassword = strdup(passwd);
			      pGroupname = strdup(groupname);
			      if(strlen(dispname)==0){
			      		pDisp = strdup(username);	
			      }else{
			      		pDisp = strdup(dispname);	
			      }
			      printf("pSsid = %s, pUsername = %s, pPassword = %s, pGroupname = %s, pDisp = %s, lusertype = %d, lflags = %d, lMaxmac = %d\n", pSsid,pUsername,pPassword,pGroupname,pDisp,lusertype,lflags,lMaxMac);
			      ncPortalUser *result = NULL;
			      result = ncSrvSynUserInfo(psShmHead,pGroupname,pSsid,pUsername,pPassword,NULL,pDisp,lusertype,lMaxMac,lflags,&iStatus);
			      if(result){
			      		printf("status = %d\n", result->cStatus);//0正常
			      }
			      if(pSsid){
				         free(pSsid);
				         pSsid = NULL;
			      }
			      if(pUsername){
				         free(pUsername);
				         pUsername = NULL;
			      }
			      if(pPassword){
				         free(pPassword);
				         pPassword = NULL;
			      }
			      if(pGroupname){
				         free(pGroupname);
				         pGroupname = NULL;
			      }
			      if(pDisp){
				         free(pDisp);
				         pDisp = NULL;
			      }
    	  	
    	  	  if(strlen(macall) != 0){
					  		q =	utStrSkipSpaces(macall);
					  		while(q && *q){
					  				q = utStrGetWord(q,caTemp2,18,"|");
					  				memset(caTempd,0,sizeof(caTempd));
					  				sprintf(caTempd,"insert into ncsrvusermac (usermac, username, groupid, starttime, lasttime, moditime, flags) values ('%s', '%s', %lu, %lu, %lu, %lu, %lu)", caTemp2,username,0,time(0),time(0),time(0),0);
						 				printf("caTempd = %s\n", caTempd);
								 		pasDbExecSqlF(caTempd);
								 		char *aUsername = NULL;
								 		char aUsermac[6];
								 		unsigned long aGroupid = 0;
								 		aUsername = strdup(username);
//								 		aUsermac = strdup(caTemp2);
								 		pasCvtMacI((char *)caTemp2,aUsermac);
								 		printf("aGroupid = %d, aUsername = %s, aUsermac = %s\n", aGroupid,aUsername,pasCvtMac(aUsermac));
								 		ncSrvAddBindMac(psShmHead,aGroupid,aUsername,aUsermac);
								 		if(aUsername){
								         free(aUsername);
								         aUsername = NULL;
							      }
					  				q++;	
					  		}
					  }
    	  
		    	  //单位
		    	  unsigned int lSid = 0;
		    	  lSid=utMd5Code(groupname,strlen(groupname),"ffff");
			   	  if(lSid>2000000000) lSid=lSid-2000000000;
			   	  if(lSid>2000000000) lSid=lSid-2000000000;
		        pasDbExecSqlF("insert into ncsrvgroup(groupid,groupname,dispname,flags) values (%lu,'%s','%s',2)",lSid,groupname,caTemp2);
		        printf("insert into ncsrvgroup(groupid,groupname,dispname,flags) values (%lu,'%s','%s',2) \n",lSid,groupname,caTemp2);
		        ncSrvSynUser2Rad(psShmHead);
    		    sprintf(myJson,"{cmd:update,status:success,timstamp:%d}",time(0)); 
//    	  }	
	  }
	  
//	  https://192.168.20.168/pronetway/memInterface?cmd=del&groupcode=410000010200000001&username=wupeng&timestamp=1407821741&checksum=e251f9767416f39b3a7d762c5c33aa85
	  if (strcmp(cmd,"del") == 0)
	  {
//	    	if (strcmp(checkdst,checksum) == 0)   
//    		{	
    			  printf("username = %s \n",username);
    		    ncSrvDelUserMacByName(psShmHead,0,username); 
    		    result = ncSrvSynUserInfo(psShmHead,"",utComGetVar_sd(psShmHead,"NcmMySSID",  "ICPIC"),username,"","","",0,0,9,&iStatus);
						if(result){
								printf("status = %d\n", result->cStatus);
						}
    		    memset(myJson,0,sizeof(myJson));
    		    sprintf(myJson,"{cmd:del,status,success,timestamp:%d }",time(0));
//    	  }	
	  }
	  int maxbindmac = 0;
	  char maclistall[1024] = "";
//	https://192.168.20.168/pronetway/memInterface?cmd=select&groupcode=410000010200000001&username=wupeng&timestamp=1407821741&checksum=fa7c0c24aac6c13a213a811d11398ea9
	  if (strcmp(cmd,"select") == 0)
	  {
//	    	if (strcmp(checkdst,checksum) == 0)   
//    		{	
    		   sprintf(caTemp,"select usermac from ncsrvusermac where username = '%s' and flags = 0 ",username);	
					 snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by groupid");
				   printf("caTemp=%s\n",caTemp);
				   psCur = pasDbOpenSql(caTemp,0);
				
					 if(psCur == NULL){
						printf("pscur is null\n");
						printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
					 }
					 if(psCur != NULL)
					 {
						int iReturn=0;
						int iNum=0;
						char usermac[32] = "";
						while(0==(iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(usermac) - 1,usermac))  || 1405==iReturn)
						{	
								if (iNum == 0)
								{
								    strcpy(maclistall,usermac);
								}	
								else
								{
								    strcat(maclistall,"|");	
								    strcat(maclistall,usermac);	
								}	
								iNum++;
						}
						pasDbCloseCursor(psCur);
					 }  
					 memset(caTemp,0,sizeof(caTemp));
					 snprintf(caTemp, sizeof(caTemp), "select maxbindmac from ncsrvuser where username = '%s' and flags = 0 ",username);
					 printf("caTemp = %s \n",caTemp);
	         iReturn = pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &maxbindmac);
					 sprintf(myJson,"{cmd:select,maclist:%s,maxmac:%d,timstamp:%d}",maclistall,maxbindmac,time(0));
					 
//    	  }	
	  }
	  

	  printf("myJson = %s \n",myJson);
//	    if (strcmp(cmd,"login") == 0)
//	    {
	    	//应答
	  utComTcpResponse(iFd,psMsgHead,1,
			                       "text",UT_TYPE_STRING,myJson);
	    	
//	    }
//    }
//    if (pBuf)
//    {
//        free(pBuf);	
//    	  pBuf = NULL;
//    }	
    
    return 0;
}

//丹尼斯接口 (需求未定)
int ncmcomDnsInterface(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    int iCount = 0, allCount = 0;
    ncPortalUser    *psUser;
    ncPortalOnline  *psOnline;
    long lBase;
    char caSdate[32] = "" , keyword[64] = "",flow_flag[4] = "", caDel[10] ="", caSesid[1024] = "", opt[256] = "";
    int iReturn;
    char cmd[32] = "", sitecode[16] = "", macall[5120] = "", account[32] = "", checksum[1024] = "", timestamp[32] = "";
    char *pBuf = "";
    char caTemp[2024] = "", macnew[2048] = "", newmac[2048] = "", myJson[3036] = "", cTmp[128] = "", myKey[32] = "", authsum[32] = "";
    char tmpch[48] = "", dstEchostr[48] = "", dst[48] = "", caTemp_d[2024] = "";
    int tNum = 0, iNum = 0, iRet = 0, i = 0;
    utMsgPrintMsg(psMsgHead);
    iRet = utMsgGetSomeNVar(psMsgHead,5, "cmd",       UT_TYPE_STRING, sizeof(cmd) - 1,      cmd,
                                         "sitecode",  UT_TYPE_STRING, sizeof(sitecode) - 1, sitecode,
                                         "mac",       UT_TYPE_STRING, sizeof(macall) - 1,   macall,
                                         "timestamp", UT_TYPE_STRING, sizeof(timestamp) - 1,timestamp,
                                         "checksum",  UT_TYPE_STRING, sizeof(checksum) - 1, checksum);
//    strcpy(timestamp,"1406111558");
    char testUrl[512] = "http://dennis01.chinacloudapp.cn/CYWXSW.ashx&test=中文测试&cmd=注册";
    int len = 0;
    int new_length;
    char newUrl[512] = "", obj[512] = {0}, retBuf[1024] = "";
    char *tmputf8 = (char *)convert("GBK", "UTF-8", testUrl);
    len = strlen(tmputf8);
    
    
//    len = strlen(testUrl);
    //默认是GBK编码          url编码百度工具
    URLEncode(tmputf8, len, newUrl, 512);
    printf("newUrl = %s  new_length = %d\n",newUrl,new_length);
    new_length = strlen(newUrl);
    if (tmputf8)
    	 free(tmputf8);tmputf8 = NULL;
    	 
    
    strcpy(retBuf,(char *)ncmHttpPostMsg("",""));	 
    entertrim(retBuf);
    
    printf("retBuf = %s \n",retBuf);
    //解码：	 
//    int resultSize = URLDecode(newUrl, new_length, obj, 512);
//    printf("result: %d, %s\n", resultSize, obj);
      
//    strcpy(caTemp_d,pasCvtGBK(2,obj,caTemp,sizeof(obj)-1));             //utf-8 调试信息
//    printf("2 caTemp_d = %s \n",caTemp_d);
    
    /*
    strcpy(myKey,utComGetVar_sd(psShmHead,"QmxcKey",  "portal"));
    strcpy(cTmp,timestamp);
    strcat(cTmp,myKey);
    printf("cTmp = %s \n",cTmp);
    strcpy(authsum,(char *)ncmMd5char(cTmp,16));
//    utMd5Ascii22(cTmp,strlen(cTmp),NULL,authsum);
    printf("authsum = %s \n",authsum);
//    for(i=0;*pstr;i++)     
//     dst[i] = toupper(*pstr++);
    for(i=0;i < strlen(authsum);i++)     
     dst[i] = tolower(authsum[i]); 
    dst[i] = '\0';
    printf("%s\n",dst);
    
    if (strcmp(dst,checksum) == 0)   //校验
    {	
	    //处理macall
	    if (strlen(macall) > 0)
	    {
	    	 pBuf = strdup(macall);
	    	 printf("pBuf = %s \n",pBuf);
	    	 strcpy(myJson,"{");
	    	 while (pBuf)
	    	 {
	    	 	   memset(caTemp,0,sizeof(caTemp));
	    	 	   pBuf = utStrGetWord(pBuf,caTemp,1000,"|");
	    	 	   printf("caTemp = %s \n",caTemp);
			    	 if (tNum == 0)
			    	 {	
			    	 	 if (strlen(caTemp) > 0)       //生成Json字符串
			    	 	 {
			    	 	    strcat(myJson,"{");	
			    	 	    psOnline = ncmGetOnlineUserByMac(psShmHead,(char *)caTemp);
			    	 	    strcat(myJson,"mac:");
			    	 	    strcat(myJson,caTemp);
			    	 	    strcat(myJson,",ip:");	
			    	 	    strcat(myJson,utComHostIp(htonl(psOnline->lSip)));
			    	 	    strcat(myJson,",mobile:");	
			    	 	    if(strcasecmp(caTemp,"00:0c:29:08:69:b8") == 0)    //张晓 
			    	 	    {
			    	 	    	strcat(myJson,"13716861819");
			    	 	    }	
			    	 	    else
			    	 	    strcat(myJson,psOnline->caName);
			    	 	    if (strlen(psOnline->caName) != 11)
			    	 	    {
			    	 	       	strcat(myJson,",type:0");	
			    	 	    }
			    	 	    else
			    	 	    {
			    	 	    	  strcat(myJson,",type:1");	
			    	 	    }		
			    	 	    strcat(myJson,"}");	
			    	 	 }	
//				    	 strcpy(macnew,"'");
//				    	 strcat(macnew,caTemp);
//				    	 strcat(macnew,"'");
//				    	 strcat(macnew,",");
			    	 }
			    	 else
			    	 {
			    	 	 if (strlen(caTemp) > 0)
			    	 	 {
			    	 	    strcat(myJson,",{");	
			    	 	    psOnline = ncmGetOnlineUserByMac(psShmHead,(char *)caTemp);
			    	 	    strcat(myJson,"mac:");
			    	 	    strcat(myJson,caTemp);
			    	 	    strcat(myJson,",ip:");	
			    	 	    strcat(myJson,utComHostIp(htonl(psOnline->lSip)));
			    	 	    strcat(myJson,",mobile:");	
			    	 	    strcat(myJson,psOnline->caName);
			    	 	    if (strlen(psOnline->caName) != 11)
			    	 	    {
			    	 	       	strcat(myJson,",type:0");	
			    	 	    }
			    	 	    else
			    	 	    {
			    	 	    	  strcat(myJson,",type:1");	
			    	 	    }			
			    	 	    strcat(myJson,"}");	
			    	 	 }	
//			    	   strcat(macnew,"'");
//				    	 strcat(macnew,caTemp);
//				    	 strcat(macnew,"'");
//				    	 strcat(macnew,","); 	
			    	 }	
			    	 tNum++;
			    	 if(*pBuf != '|') {
		            break;
		         }
		         pBuf++;
	    	 }
	    	 strcat(myJson,"}");
	    }	
	    
//	    printf("macnew = %s \n",macnew);
//	    strcpy(newmac,(char *)ncmutlremovelastchar2(macnew));         //数据库查询
	    printf("myJson = %s \n",myJson);
	    if (strcmp(cmd,"login") == 0)
	    {
	    	//应答
	    	utComTcpResponse(iFd,psMsgHead,1,
			                       "text",UT_TYPE_STRING,myJson);
	    	
	    }
    }
//    if (pBuf)
//    {
//        free(pBuf);	
//    	  pBuf = NULL;
//    }	
    */
    return 0;
}


int Ncm_PortalInterface_SetFunName(utShmHead *psShmHead)
{
	 int iReturn;
	 iReturn = pasSetTcpFunName("ncmcomTaiBaoInterface",ncmcomTaiBaoInterface,0);                               //启明星辰接口
	 iReturn = pasSetTcpFunName("ncmcomHNTaiBaoInterface",ncmcomHNTaiBaoInterface,0);                           //河南太保接口
	 iReturn = pasSetTcpFunName("ncmcomDnsInterface",ncmcomDnsInterface,0);                                     //丹尼斯接口
   return 0;	
}

