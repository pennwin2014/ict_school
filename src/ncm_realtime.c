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
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include <pcap.h>

#include <errno.h>
#include <iconv.h>
#include <dirent.h>


#include "ncmweb.h"

static void ncmexecdel_user(char username[32])
{
    char tmp[128] = "";
    system("cd /home/ncmysql/ncsrv/bin");
    sprintf(tmp,"./portalonline -k %s",username);
    printf("tmp = %s \n",tmp);
	  system(tmp);
}
long long ncmWebGetTisdByUname(utShmHead *psShmHead, char *uname)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    int iCount = 0;
    ncPortalUser    *psUser;
    ncPortalOnline  *psOnline;
    ncPortalAp      *psAp;
    long lBase;
    char opt[256] = "";
    int iReturn;
    char *p = NULL;
    utPltDbHead *psDbHead;
    int lcid = 0;
    long long ltsid = 0;
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
       if(strlen(psOnline->caName) > 0 && psOnline->login == 1)              //Ö»ÏÔÊ¾ÓÐÐ§ÐÅÏ¢ ²¢ÇÒÒÑµÇÂ¼ÓÃ»§
       {
       
            if(strcmp(psOnline->caName,uname) == 0)
//       			if(strcmp(psOnline->psUser?psUser->caName:"None",uname) == 0)
       		  {
       		     ltsid = psOnline->lTsid;
       		     ncSrvLetUserOffLineByTsid(psShmHead,ltsid);
            }	
       }      
        psOnline = (ncPortalOnline *)pasHashNextS(&sHashInfo);
    }
    
    return ltsid;
}
// ÔÚÏßÓÃ»§   
int ncmWebShowOnlineUserInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	printf("== ÔÚÏßÓÃ»§ ==\n");
    pasHashInfo sHashInfo;
    uchar *pHash;
    int iCount = 0, allCount = 0;
    ncPortalUser    *psUser;
    ncPortalOnline  *psOnline;
    ncPortalAp      *psAp;
    long lBase;
    char caSdate[32] = "" , keyword[64] = "",flow_flag[4] = "", caDel[10] ="", caSesid[1024] = "", opt[256] = "", auth_flag[4] = "";
    int iReturn;
    char *p = NULL;
    long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
    char caCid[16],caLimit[16],caPage[16],caStart[16], dStype[8] = "";
    utPltDbHead *psDbHead;
    psDbHead = utPltInitDb();
    int lcid = 0;
    iReturn = dsCltGetSessionValue(1,"cid",UT_TYPE_LONG,sizeof(long),&lcid);
    iReturn = dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(opt) - 1,opt);
    iReturn = dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(dStype) - 1,dStype);
//    printf("lcid = %d opt = %s\n",lcid,opt);
//    utMsgPrintMsg(psMsgHead);    
   	iReturn = utMsgGetSomeNVar(psMsgHead,8,"cond",      UT_TYPE_STRING, sizeof(keyword) - 1  ,keyword
   	                                      ,"del",       UT_TYPE_STRING, sizeof(caDel) - 1,  caDel
   	                                      ,"start",     UT_TYPE_STRING, 10,  caStart
   	                                      ,"limit",     UT_TYPE_STRING, 10,  caLimit
									      ,"page",      UT_TYPE_STRING, 10,  caPage  
   	                                      ,"selsid",    UT_TYPE_STRING, sizeof(caSesid) - 1,  caSesid
   	                                      ,"auth_flag", UT_TYPE_STRING, sizeof(auth_flag) - 1,auth_flag
   	                                      ,"flow_flag", UT_TYPE_STRING, sizeof(flow_flag) - 1,flow_flag);
    
    lRowNum=atol(caLimit);
	  if (strlen(caPage) == 0)
	  	 lCurPg=1;
	  else
	  	 lCurPg = atol(caPage);
	  lStartRec = atol(caStart);
    
   
//    printf("lRowNum = %d  lCurPg = %d  lStartRec = %d \n",lRowNum,lCurPg,lStartRec);
    uint8 llTsid = 0;
	printf("caDel=%s\n",caDel);
	printf("name= %s\n",caSesid);
	printf("keyword= %s\n",keyword);
    if(!utStrIsSpaces(caDel)) {  /* É¾³ýÑ¡ÖÐµÄÏîÄ¿*/
    	if(strlen(caSesid)>0){
    		  p=strtok(caSesid,",");
          while(p){
            	llTsid = 0;
            	llTsid = ncmWebGetTisdByUname(psShmHead,p);
            	//llTsid=atoll(p);
            	printf("ltsid=%llu\n",llTsid);
            	ncSrvLetUserOffLineByTsid(psShmHead,llTsid);
//              ncmexecdel_user(p);
              p=strtok(NULL,",");
          }   
    		     
    	}
		}
    
    pHash =  (unsigned char *)utShmHashHead(psShmHead,NCSRV_LNK_ONLINE);
    
    if(pHash == NULL) 
    {
  //      pasLog(PAS_LOG_ERROR," %d Hash Record Null",NCSRV_LNK_ONLINE);
        return (-1);
    }

    lBase = utShmGetBaseAddr(psShmHead);
    psOnline = (ncPortalOnline *)pasHashFirst(pHash,&sHashInfo);
    
//    printf("pid = %d \n",getpid());
//    printf("[Num] UserName        DispName        Password       Ssid             ApName                       Mac               TermType      Dev     Os  Lang  Login Bytes          Start              Last \n");
    while(psOnline) 
    {
        psUser = (ncPortalUser *) ( (char *)psOnline->psUser + lBase);
        psAp = (ncPortalAp *) ( (char *)psOnline->psAp + lBase);
        strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->starttime));
        if (strlen(dStype) > 0)                  //¼¯ÍÅ
        {
//           if (psOnline->lStype/1000000 == atol(dStype))          //ÉÌ¼ÒÈ¨ÏÞ
           if (psOnline->lStype/10%10000 == atol(dStype))          //ÉÌ¼ÒÈ¨ÏÞ	
           {
           	   if (strlen(auth_flag) >0 && atoi(auth_flag) == 2)          //Î´ÈÏÖ¤
				       {
				       	  if (strlen(psOnline->caName) > 0 && psOnline->login == 0)              //Ö»ÏÔÊ¾ÓÐÐ§ÐÅÏ¢ ²¢ÇÒÒÑµÇÂ¼ÓÃ»§
					        {
					       		if (strlen(opt) > 18)        // ÉÌ³¬²ã´ÎÈ¨ÏÞ
					       		{
					       			if(strstr(psOnline->psUser?psUser->caGroup:"None",opt))
					       		  {
					       		  	if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{	
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
								        		  }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			  {	
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
					//			        				printf("*********allCount = %d   max = %d\n",allCount,lCurPg*lRowNum);
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
											      
								        	} 
								        } 
					       		  }
					       		  else
					       		  {
					       		    if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
									        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        			{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
									        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        		{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											     	 }
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
										        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        		{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        	} 
								        } 	
					       		  }	
					       		
						       		
						        }
						        else
						        {
						        	if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
							        	{	
							        		if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
										        	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
										        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        		} 
							       		}
								   	  }
								   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
							        	{	
									        if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											        {
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
							        			if(iCount>0){
										        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
											      }
										        iCount ++;
										        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
										        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
										        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
										        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
										        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
										        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
										        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
										        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
										        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
										        if(psOnline->login == 1)
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
										        }	
										        else
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
										        }
										        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
											   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
											   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
										        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      }
							        		} 
							       		}
							        }
							        else                                                     //È«²¿
							        {
							        	if(strlen(keyword) > 0 )
							        	{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											      	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        	}
							        	else
							        	{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        //
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        	} 
							        } 
						        }	
					        }	
				       }
				       else if (strlen(auth_flag) >0 && atoi(auth_flag) == 9)          //ÍË³öÈÏÖ¤
				       {
				       	  if(strlen(psOnline->caName) > 0 && psOnline->login == 9)              //Ö»ÏÔÊ¾ÓÐÐ§ÐÅÏ¢ ²¢ÇÒÒÑµÇÂ¼ÓÃ»§
					       {
					       		if(strlen(opt) > 18)        //
					       		{
					       			if(strstr(psOnline->psUser?psUser->caGroup:"None",opt))
					       		  {
					       		  	if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{	
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
								        		  }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			  {	
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
					//			        				printf("*********allCount = %d   max = %d\n",allCount,lCurPg*lRowNum);
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
											      
								        	} 
								        } 
					       		  }
					       		  else
					       		  {
					       		    if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		  utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		  utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
									        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        			{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
									        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        		{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											     	 }
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
										        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        		{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        	} 
								        } 	
					       		  }	
					       		
						       		
						        }
						        else
						        {
						        	if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
							        	{	
							        		if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
										        	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
										        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        		} 
							       		}
								   	  }
								   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
							        	{	
									        if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											        {
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
							        			if(iCount>0){
										        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
											      }
										        iCount ++;
										        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
										        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
										        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
										        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
										        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
										        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
										        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
										        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
										        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
										        if(psOnline->login == 1)
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
										        }	
										        else
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
										        }
										        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
											   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
											   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
										        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      }
							        		} 
							       		}
							        }
							        else                                                     //È«²¿
							        {
							        	if(strlen(keyword) > 0 )
							        	{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											      	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        	}
							        	else
							        	{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        //
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        	} 
							        } 
						        }	
					       }	
				       }
				       else if (strlen(auth_flag) >0 && atoi(auth_flag) == 0)	 
				       {
				//       	 if(strlen(psOnline->caName) > 0 )              //Ö»ÏÔÊ¾ÓÐÐ§ÐÅÏ¢ ²¢ÇÒÒÑµÇÂ¼ÓÃ»§
				//	       {
					       		if(strlen(opt) > 18)        //
					       		{
					       			if(strstr(psOnline->psUser?psUser->caGroup:"None",opt))
					       		  {
					       		  	if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{	
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
								        		  }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			  {	
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
					//			        				printf("*********allCount = %d   max = %d\n",allCount,lCurPg*lRowNum);
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
											      
								        	} 
								        } 
					       		  }
					       		  else
					       		  {
					       		    if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
									        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        			{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
									        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        		{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											     	 }
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
										        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        		{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        	} 
								        } 	
					       		  }	
					       		
						       		
						        }
						        else
						        {
						        	if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
							        	{	
							        		if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
										        	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
										        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        		} 
							       		}
								   	  }
								   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
							        	{	
									        if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											        {
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
							        			if(iCount>0){
										        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
											      }
										        iCount ++;
										        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
										        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
										        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
										        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
										        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
										        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
										        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
										        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
										        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
										        if(psOnline->login == 1)
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
										        }	
										        else
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
										        }
										        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
											   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
											   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
										        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      }
							        		} 
							       		}
							        }
							        else                                                     //È«²¿
							        {
							        	if(strlen(keyword) > 0 )
							        	{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											      	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        	}
							        	else
							        	{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        //
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        	} 
							        } 
						        }	
				//	       }	
				       }
				       else
				       {
				         if(strlen(psOnline->caName) > 0 && psOnline->login == 1)              //Ö»ÏÔÊ¾ÓÐÐ§ÐÅÏ¢ ²¢ÇÒÒÑµÇÂ¼ÓÃ»§
					       {
					       		if(strlen(opt) > 18)        //
					       		{
					       			if(strstr(psOnline->psUser?psUser->caGroup:"None",opt))
					       		  {
					       		  	if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{	
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
								        		  }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			  {	
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
					//			        				printf("*********allCount = %d   max = %d\n",allCount,lCurPg*lRowNum);
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
											      
								        	} 
								        } 
					       		  }
					       		  else
					       		  {
					       		    if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
									        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        			{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
									        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        		{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											     	 }
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
										        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        		{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        	} 
								        } 	
					       		  }	
					       		
						       		
						        }
						        else
						        {
						        	if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
							        	{	
							        		if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
										        	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
										        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        		} 
							       		}
								   	  }
								   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
							        	{	
									        if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											        {
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
							        			if(iCount>0){
										        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
											      }
										        iCount ++;
										        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
										        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
										        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
										        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
										        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
										        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
										        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
										        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
										        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
										        if(psOnline->login == 1)
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
										        }	
										        else
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
										        }
										        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
											   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
											   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
										        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      }
							        		} 
							       		}
							        }
							        else                                                     //È«²¿
							        {
							        	if(strlen(keyword) > 0 )
							        	{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											      	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        	}
							        	else
							        	{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        //
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        	} 
							        } 
						        }	
					       }	
				       }		
           }
           			
        }	
        else
           {
           	   if (strlen(auth_flag) >0 && atoi(auth_flag) == 2)          //Î´ÈÏÖ¤
				       {
				       	  if(strlen(psOnline->caName) > 0 && psOnline->login == 0)              //Ö»ÏÔÊ¾ÓÐÐ§ÐÅÏ¢ ²¢ÇÒÒÑµÇÂ¼ÓÃ»§
					       {
					       		if(strlen(opt) > 18)        //¼¯ÍÅ
					       		{
					       			if(strstr(psOnline->psUser?psUser->caGroup:"None",opt))
					       		  {
					       		  	if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{	
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
								        		  }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			  {	
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
					//			        				printf("*********allCount = %d   max = %d\n",allCount,lCurPg*lRowNum);
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
											      
								        	} 
								        } 
					       		  }
					       		  else
					       		  {
					       		    if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
									        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        			{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
									        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        		{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											     	 }
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
										        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        		{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        	} 
								        } 	
					       		  }	
					       		
						       		
						        }
						        else
						        {
						        	if (lcid > 0)
						        	{
						        	   if (lcid == psOnline->lGroupid)
						        	   {
						        	     	if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
										        {	
										        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
										        	{	
										        		if(strlen(keyword) > 0 )
										        		{
										        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
										        			{
										        				allCount++;
													        	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
													        	{
											        				if(iCount>0){
														        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
																      }
															        iCount ++;
															        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
															        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
															        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
															        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
															        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
															        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
															        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
															        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
															        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
															        if(psOnline->login == 1)
															        {
															        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
															        }	
															        else
															        {
															        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
															        }
															        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
																   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
																   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
															   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
															   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
															        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
														      	}
										        			}	
										        		}
										        		else
										        		{
										        			allCount++;
													        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
													        {
											        			if(iCount>0){
														        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
															      }
														        iCount ++;
														        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
														        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
														        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
														        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
														        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
														        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
														        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
														        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
														        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
														        if(psOnline->login == 1)
														        {
														        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
														        }	
														        else
														        {
														        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
														        }
														        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
															   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
															   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
															   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
															   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
														        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
													      	}
										        		} 
										       		}
											   	  }
											   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
										        {	
										        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
										        	{	
												        if(strlen(keyword) > 0 )
										        		{
										        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
										        			{
										        				allCount++;
														        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
														        {
											        				if(iCount>0){
														        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
																      }
															        iCount ++;
															        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
															        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
															        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
															        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
															        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
															        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
															        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
															        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
															        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
															        if(psOnline->login == 1)
															        {
															        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
															        }	
															        else
															        {
															        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
															        }
															        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
																   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
																   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
															   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
															   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
															        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
														      	}
										        			}	
										        		}
										        		else
										        		{
										        			allCount++;
														      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
														      {
										        			if(iCount>0){
													        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
															   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
															   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
													      }
										        		} 
										       		}
										        }
										        else                                                     //È«²¿
										        {
										        	if(strlen(keyword) > 0 )
										        	{
										        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
										        			{
										        				allCount++;
														      	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
														      	{
											        				if(iCount>0){
														        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
																      }
															        iCount ++;
															        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
															        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
															        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
															        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
															        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
															        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
															        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
															        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
															        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
															        if(psOnline->login == 1)
															        {
															        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
															        }	
															        else
															        {
															        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
															        }
															        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
																   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
																   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
															   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
															   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
															        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
														      	}
										        			}	
										        	}
										        	else
										        	{
										        			allCount++;
														      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
														      {
											        			if(iCount>0){
														        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
															      }
														        iCount ++;
														        //
														        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
														        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
														        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
														        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
														        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
														        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
														        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
														        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
														        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
														        if(psOnline->login == 1)
														        {
														        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
														        }	
														        else
														        {
														        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
														        }
														        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
															   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
															   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
															   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
															   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
														        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
													      	}
										        	} 
										        } 
						        	   }		
						        	}
						        	else
						        	{
						        		 if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
										        {	
										        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
										        	{	
										        		if(strlen(keyword) > 0 )
										        		{
										        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
										        			{
										        				allCount++;
													        	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
													        	{
											        				if(iCount>0){
														        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
																      }
															        iCount ++;
															        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
															        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
															        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
															        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
															        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
															        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
															        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
															        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
															        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
															        if(psOnline->login == 1)
															        {
															        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
															        }	
															        else
															        {
															        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
															        }
															        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
																   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
																   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
															   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
															   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
															        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
														      	}
										        			}	
										        		}
										        		else
										        		{
										        			allCount++;
													        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
													        {
											        			if(iCount>0){
														        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
															      }
														        iCount ++;
														        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
														        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
														        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
														        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
														        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
														        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
														        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
														        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
														        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
														        if(psOnline->login == 1)
														        {
														        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
														        }	
														        else
														        {
														        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
														        }
														        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
															   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
															   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
															   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
															   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
														        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
													      	}
										        		} 
										       		}
											   	  }
											   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
										        {	
										        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
										        	{	
												        if(strlen(keyword) > 0 )
										        		{
										        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
										        			{
										        				allCount++;
														        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
														        {
											        				if(iCount>0){
														        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
																      }
															        iCount ++;
															        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
															        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
															        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
															        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
															        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
															        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
															        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
															        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
															        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
															        if(psOnline->login == 1)
															        {
															        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
															        }	
															        else
															        {
															        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
															        }
															        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
																   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
																   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
															   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
															   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
															        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
														      	}
										        			}	
										        		}
										        		else
										        		{
										        			allCount++;
														      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
														      {
										        			if(iCount>0){
													        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
															   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
															   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
													      }
										        		} 
										       		}
										        }
										        else                                                     //È«²¿
										        {
										        	if(strlen(keyword) > 0 )
										        	{
										        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
										        			{
										        				allCount++;
														      	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
														      	{
											        				if(iCount>0){
														        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
																      }
															        iCount ++;
															        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
															        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
															        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
															        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
															        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
															        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
															        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
															        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
															        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
															        if(psOnline->login == 1)
															        {
															        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
															        }	
															        else
															        {
															        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
															        }
															        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
																   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
																   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
															   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
															   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
															        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
														      	}
										        			}	
										        	}
										        	else
										        	{
										        			allCount++;
														      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
														      {
											        			if(iCount>0){
														        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
															      }
														        iCount ++;
														        //
														        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
														        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
														        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
														        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
														        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
														        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
														        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
														        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
														        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
														        if(psOnline->login == 1)
														        {
														        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
														        }	
														        else
														        {
														        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
														        }
														        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
															   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
															   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
															   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
															   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
														        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
													      	}
										        	} 
										        } 
						        	}		
						        	
						        }	
					       }	
				       }
				       else if (strlen(auth_flag) >0 && atoi(auth_flag) == 9)          //ÍË³öÈÏÖ¤
				       {
				       	  if(strlen(psOnline->caName) > 0 && psOnline->login == 9)              //Ö»ÏÔÊ¾ÓÐÐ§ÐÅÏ¢ ²¢ÇÒÒÑµÇÂ¼ÓÃ»§
					       {
					       		if(strlen(opt) > 18)        //
					       		{
					       			if(strstr(psOnline->psUser?psUser->caGroup:"None",opt))
					       		  {
					       		  	if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{	
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
								        		  }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			  {	
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
					//			        				printf("*********allCount = %d   max = %d\n",allCount,lCurPg*lRowNum);
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
											      
								        	} 
								        } 
					       		  }
					       		  else
					       		  {
					       		    if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		  utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		  utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
									        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        			{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
									        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        		{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											     	 }
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
										        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        		{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        	} 
								        } 	
					       		  }	
					       		
						       		
						        }
						        else
						        {
						        	if (lcid > 0)
						        	{
						        	   if (lcid == psOnline->lGroupid)
						        	   {
						        	     	if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
							        	{	
							        		if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
										        	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
										        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        		} 
							       		}
								   	  }
								   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
							        	{	
									        if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											        {
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
							        			if(iCount>0){
										        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
											      }
										        iCount ++;
										        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
										        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
										        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
										        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
										        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
										        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
										        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
										        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
										        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
										        if(psOnline->login == 1)
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
										        }	
										        else
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
										        }
										        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
											   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
											   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
										        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      }
							        		} 
							       		}
							        }
							        else                                                     //È«²¿
							        {
							        	if(strlen(keyword) > 0 )
							        	{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											      	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        	}
							        	else
							        	{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        //
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        	} 
							        } 
						        	   }		
						        	}
						        	else
						        	{
						        		 if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
							        	{	
							        		if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
										        	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
										        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        		} 
							       		}
								   	  }
								   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
							        	{	
									        if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											        {
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
							        			if(iCount>0){
										        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
											      }
										        iCount ++;
										        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
										        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
										        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
										        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
										        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
										        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
										        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
										        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
										        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
										        if(psOnline->login == 1)
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
										        }	
										        else
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
										        }
										        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
											   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
											   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
										        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      }
							        		} 
							       		}
							        }
							        else                                                     //È«²¿
							        {
							        	if(strlen(keyword) > 0 )
							        	{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											      	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        	}
							        	else
							        	{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        //
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        	} 
							        } 
						        	}		
						        	
						        }	
					       }	
				       }
				       else if (strlen(auth_flag) >0 && atoi(auth_flag) == 0)	         //Î´µÇÂ½
				       {
				//       	 if(strlen(psOnline->caName) > 0 )              //Ö»ÏÔÊ¾ÓÐÐ§ÐÅÏ¢ ²¢ÇÒÒÑµÇÂ¼ÓÃ»§
				//	       {
					       		if(strlen(opt) > 18)        //
					       		{
					       			if(strstr(psOnline->psUser?psUser->caGroup:"None",opt))
					       		  {
					       		  	if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{	
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
								        		  }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			  {	
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
					//			        				printf("*********allCount = %d   max = %d\n",allCount,lCurPg*lRowNum);
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
											      
								        	} 
								        } 
					       		  }
					       		  else
					       		  {
					       		    if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);

												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
									        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        			{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
									        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        		{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											     	 }
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
										        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        		{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        	} 
								        } 	
					       		  }	
					       		
						       		
						        }
						        else
						        {
						        	if (lcid > 0)
						        	{
						        	   if (lcid == psOnline->lGroupid)
						        	   {
						        	     	if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
							        	{	
							        		if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
										        	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
										        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        		} 
							       		}
								   	  }
								   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
							        	{	
									        if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											        {
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
							        			if(iCount>0){
										        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
											      }
										        iCount ++;
										        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
										        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
										        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
										        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
										        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
										        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
										        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
										        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
										        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
										        if(psOnline->login == 1)
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
										        }	
										        else
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
										        }
										        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
											   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
											   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
										        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      }
							        		} 
							       		}
							        }
							        else                                                     //È«²¿
							        {
							        	if(strlen(keyword) > 0 )
							        	{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											      	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        	}
							        	else
							        	{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        //
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        	} 
							        } 
						        	   }		
						        	}
						        	else
						        	{
						        		if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
							        	{	
							        		if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
										        	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
										        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        		} 
							       		}
								   	  }
								   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
							        	{	
									        if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											        {
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
							        			if(iCount>0){
										        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
											      }
										        iCount ++;
										        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
										        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
										        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
										        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
										        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
										        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
										        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
										        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
										        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
										        if(psOnline->login == 1)
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
										        }	
										        else
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
										        }
										        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
											   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
											   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
										        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      }
							        		} 
							       		}
							        }
							        else                                                     //È«²¿
							        {
							        	if(strlen(keyword) > 0 )
							        	{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											      	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        	}
							        	else
							        	{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        //
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        	} 
							        } 
						        	}	
						        	
						        }	
				//	       }	
				       }
				       else
				       {
				         if(strlen(psOnline->caName) > 0 && psOnline->login == 1)              //Ö»ÏÔÊ¾ÓÐÐ§ÐÅÏ¢ ²¢ÇÒÒÑµÇÂ¼ÓÃ»§
					       {
					       		if(strlen(opt) > 18)        //
					       		{
					       			if(strstr(psOnline->psUser?psUser->caGroup:"None",opt))
					       		  {
					       		  	if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{	
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											       utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid); 
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
								        		  }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			  {	
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);        
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{	
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
					//			        				printf("*********allCount = %d   max = %d\n",allCount,lCurPg*lRowNum);
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
											      
								        	} 
								        } 
					       		  }
					       		  else
					       		  {
					       		    if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
								        	{	
								        		if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
								        				if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        				{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												        }
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        		} 
								       		}
									   	  }
									   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
								        {	
								        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
								        	{	
										        if(strlen(keyword) > 0 )
								        		{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
									        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        			{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        		}
								        		else
								        		{
								        			allCount++;
									        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
									        		{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											     	 }
								        		} 
								       		}
								        }
								        else                                                     //È«²¿
								        {
								        	if(strlen(keyword) > 0 )
								        	{
								        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
								        			{
								        				allCount++;
										        		if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        		{
									        				if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
														      }
													        iCount ++;
													        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
													        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
													        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
													        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
													        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
													        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
													        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
													        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
													        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
													        if(psOnline->login == 1)
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
													        }	
													        else
													        {
													        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
													        }
													        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
														   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
														   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
													        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
												      	}
								        			}	
								        	}
								        	else
								        	{
								        			allCount++;
								        			if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
								        			{
									        			if(iCount>0){
												        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        //
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											        }
								        	} 
								        } 	
					       		  }	
					       		
						       		
						        }
						        else
						        {
						        	if (lcid > 0)
						        	{
						        	   if (lcid == psOnline->lGroupid)
						        	   {
						        	     if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
							        	{	
							        		if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
										        	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
										        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        		} 
							       		}
								   	  }
								   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
							        	{	
									        if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											        {
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
							        			if(iCount>0){
										        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
											      }
										        iCount ++;
										        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
										        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
										        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
										        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
										        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
										        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
										        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
										        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
										        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
										        if(psOnline->login == 1)
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
										        }	
										        else
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
										        }
										        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
											   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
											   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
										        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      }
							        		} 
							       		}
							        }
							        else                                                     //È«²¿
							        {
							        	if(strlen(keyword) > 0 )
							        	{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											      	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        	}
							        	else
							        	{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        //
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        	} 
							        } 	
						        	   }		
						        	}
						        	else
						        	{
						        		if(strlen(flow_flag) > 0 && atoi(flow_flag) == 1)        //ÓÐÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) > 0)
							        	{	
							        		if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
										        	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
										        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										        {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");											        
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        		} 
							       		}
								   	  }
								   	  else if(strlen(flow_flag) > 0 && atoi(flow_flag) == 2)   //ÎÞÁ÷Á¿
							        {	
							        	if ((psOnline->lBytes[0] + psOnline->lBytes[1]) == 0)
							        	{	
									        if(strlen(keyword) > 0 )
							        		{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											        if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											        {
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        		}
							        		else
							        		{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
							        			if(iCount>0){
										        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
											      }
										        iCount ++;
										        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
										        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
										        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
										        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
										        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
										        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
										        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
										        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
										        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
										        if(psOnline->login == 1)
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
										        }	
										        else
										        {
										        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
										        }
										        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
											   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
											   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
										        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      }
							        		} 
							       		}
							        }
							        else                                                     //È«²¿
							        {
							        	if(strlen(keyword) > 0 )
							        	{
							        			if(strstr(psOnline->caName,keyword) ||strcasecmp(psOnline->caSsid,keyword) == 0 ||strstr(pasCvtMac(psOnline->caMac),keyword))
							        			{
							        				allCount++;
											      	if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      	{
								        				if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
													      }
												        iCount ++;
												        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
												        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
												        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
												        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
												        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
												        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
												        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
												        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
												        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
												        if(psOnline->login == 1)
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
												        }	
												        else
												        {
												        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
												        }
												        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
													   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
													   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
												        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
											      	}
							        			}	
							        	}
							        	else
							        	{
							        			allCount++;
											      if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											      {
								        			if(iCount>0){
											        	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												      }
											        iCount ++;
											        //
											        utPltPutLoopVarF(psDbHead,"on_tsid",       iCount ,"%llu",psOnline->lTsid);
											        utPltPutLoopVar(psDbHead,"on_name",       iCount ,psOnline->caName);
											        utPltPutLoopVar(psDbHead,"on_dispname",   iCount ,psOnline->psUser?psUser->caDisp:"None");
											        utPltPutLoopVar(psDbHead,"on_pass",       iCount ,psOnline->psUser?psUser->caPass:"None");
											        utPltPutLoopVar(psDbHead,"on_ssid",       iCount ,psOnline->caSsid);
											        utPltPutLoopVar(psDbHead,"on_apname",     iCount ,psOnline->caApName);
											        utPltPutLoopVar(psDbHead,"on_temtype",    iCount ,psOnline->caTermType);
											        utPltPutLoopVar(psDbHead,"on_os",         iCount ,psOnline->caOs);
											        utPltPutLoopVar(psDbHead,"on_lang",       iCount ,psOnline->caLang);
											        utPltPutLoopVar(psDbHead,"on_apAddr",     iCount ,psOnline->psAp?psAp->caAddr:"");
											        if(psOnline->login == 1)
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"ÒÑµÇÂ¼");
											        }	
											        else
											        {
											        	utPltPutLoopVar(psDbHead,"on_login",    iCount ,"Î´µÇÂ¼");
											        }
											        utPltPutLoopVarF(psDbHead,"on_byte",      iCount ,"%llu",psOnline->lBytes[0] + psOnline->lBytes[1]);
												   		utPltPutLoopVar(psDbHead,"on_sdate",      iCount ,caSdate);
												   		utPltPutLoopVar(psDbHead,"on_mac",        iCount ,pasCvtMac(psOnline->caMac));
												   		utPltPutLoopVar(psDbHead,"on_caDev",      iCount ,psOnline->caDev);
												   		utPltPutLoopVar(psDbHead,"on_ip",         iCount ,utComHostIp(htonl(psOnline->lSip)));
											        utPltPutLoopVar(psDbHead,"on_lasttime",   iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
										      	}
							        	} 
							        } 
						        	}	
						        	
						        }	
					       }	
				       }		
           }
       
//        printf("[%4d]%-15s %-15s %-15s %-15s %-28s %-17s %-8s %-15s %-15s %-5s %d %8llu [%s][%s]\n",
//                iCount,
//                psOnline->caName,
//                psOnline->psUser?psUser->caDisp:"None",
//                psOnline->psUser?psUser->caPass:"None",
//                psOnline->caSsid,
//                psOnline->caApName,
//                pasCvtMac(psOnline->caMac),
//                psOnline->caTermType,
//                psOnline->caDev,
//                psOnline->caOs,
//                psOnline->caLang,
//                psOnline->login,
//                psOnline->lBytes[0] + psOnline->lBytes[1],
//                caSdate,
//                utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
                
        psOnline = (ncPortalOnline *)pasHashNextS(&sHashInfo);
    }
    utPltPutVarF(psDbHead,"TotRec","%d",allCount);
//    utPltShowDb(psDbHead); 
    
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmonline/ncm_real_userlist.html");
    return 0;
}


int ncmShowOnlineAllMac(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    ncPortalAp *psAp;
    long lBase;
    char caSdate[32],caMark[64];
    char caMacStr[20],caMacStr1[20];
    int iReturn;
    ncLbsMacLocation *psMacLoc;
    char keyword[64] = "",flow_flag[4] = "", caDel[10] ="", caSesid[1024] = "", opt[256] = "", auth_flag[4] = "";
    long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
    char caCid[16],caLimit[16],caPage[16],caStart[16], dStype[8] = "";
    utPltDbHead *psDbHead;
    psDbHead = utPltInitDb();
    int lcid = 0;
    int iCount = 0, allCount = 0;
    iReturn = dsCltGetSessionValue(1,"cid",UT_TYPE_LONG,sizeof(long),&lcid);
    iReturn = dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(opt) - 1,opt);
    iReturn = dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(dStype) - 1,dStype);
//    printf("lcid = %d opt = %s\n",lcid,opt);
//    utMsgPrintMsg(psMsgHead);    
   	iReturn = utMsgGetSomeNVar(psMsgHead,8,"cond",      UT_TYPE_STRING, sizeof(keyword) - 1  ,keyword
   	                                      ,"del",       UT_TYPE_STRING, sizeof(caDel) - 1,    caDel
   	                                      ,"start",     UT_TYPE_STRING, sizeof(caStart) - 1,  caStart
   	                                      ,"limit",     UT_TYPE_STRING, sizeof(caLimit) - 1,  caLimit
																          ,"page",      UT_TYPE_STRING, sizeof(caPage) - 1,   caPage  
   	                                      ,"selsid",    UT_TYPE_STRING, sizeof(caSesid) - 1,  caSesid
   	                                      ,"auth_flag", UT_TYPE_STRING, sizeof(auth_flag) - 1,auth_flag
   	                                      ,"flow_flag", UT_TYPE_STRING, sizeof(flow_flag) - 1,flow_flag);
    lRowNum=atol(caLimit);
	  if (strlen(caPage) == 0)
	  	 lCurPg=1;
	  else
	  	 lCurPg = atol(caPage);
	  lStartRec = atol(caStart);
    
    lBase = utShmGetBaseAddr(psShmHead);

    pHash =  (unsigned char *)utShmHashHead(psShmHead,PORTAL_LNK_MACLOCATION);

    if(pHash) {

        psMacLoc = (ncLbsMacLocation *)pasHashFirst(pHash,&sHashInfo);
//        printf("TermMac    ApMac  lRssi  URssi  Noise  Channel StartTime  LastTime\n");
        while(psMacLoc) {
            if(psMacLoc->psAp) {
                psAp = (ncPortalAp *)( (char *)psMacLoc->psAp + lBase);
                strcpy(caMark,psAp->caMark);
            }

            else {
                strcpy(caMark,"\0");
            }
            strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",psMacLoc->lasttime));
            if (strlen(opt) > 18)
            {
                if (psMacLoc->psAp)
                {
                   	if (strstr(psMacLoc->psAp->psGroup?psMacLoc->psAp->psGroup->caGroupName:"none",opt))
                    {
                    	  if(strlen(keyword) > 0 )
							          {
	                    	  if(strstr(pasCvtMacStr(psMacLoc->mac,caMacStr),keyword) || strstr(pasCvtMacStr(psMacLoc->apmac,caMacStr1),keyword))
													{
															allCount++;
															if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
															{	
															   if(iCount>0){
																		utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
																 }
																 iCount ++;
																 utPltPutLoopVar(psDbHead,"on_tMac",       iCount ,pasCvtMacStr(psMacLoc->mac,caMacStr));
																 utPltPutLoopVar(psDbHead,"on_uMac",       iCount ,pasCvtMacStr(psMacLoc->apmac,caMacStr1));
																 utPltPutLoopVarF(psDbHead,"on_sSignal",   iCount ,"%d",psMacLoc->lrssi);
																 utPltPutLoopVarF(psDbHead,"on_bSignal",   iCount ,"%d",psMacLoc->urssi);
																 utPltPutLoopVarF(psDbHead,"on_noise",     iCount ,"%d",psMacLoc->noise);
																 utPltPutLoopVarF(psDbHead,"on_channel",   iCount ,"%lu",psMacLoc->channel);	
																 utPltPutLoopVar(psDbHead,"on_stime",      iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psMacLoc->starttime));
																 utPltPutLoopVar(psDbHead,"on_etime",      iCount ,caSdate);
														  }
													}
											  }
												else
												{
													  allCount++;
														if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
														{	
														   if(iCount>0){
																	utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
															 }
															 iCount ++;
															 utPltPutLoopVar(psDbHead,"on_tMac",       iCount ,pasCvtMacStr(psMacLoc->mac,caMacStr));
															 utPltPutLoopVar(psDbHead,"on_uMac",       iCount ,pasCvtMacStr(psMacLoc->apmac,caMacStr1));
															 utPltPutLoopVarF(psDbHead,"on_sSignal",   iCount ,"%d",psMacLoc->lrssi);
															 utPltPutLoopVarF(psDbHead,"on_bSignal",   iCount ,"%d",psMacLoc->urssi);
															 utPltPutLoopVarF(psDbHead,"on_noise",     iCount ,"%d",psMacLoc->noise);
															 utPltPutLoopVarF(psDbHead,"on_channel",   iCount ,"%lu",psMacLoc->channel);	
															 utPltPutLoopVar(psDbHead,"on_stime",      iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psMacLoc->starttime));
															 utPltPutLoopVar(psDbHead,"on_etime",      iCount ,caSdate);
													  }
												}		
                	  }
                }	   	
            }	
            else
            {
            	  if(strlen(keyword) > 0 )
							  {      	
	            	  if(strstr(pasCvtMacStr(psMacLoc->mac,caMacStr),keyword) || strstr(pasCvtMacStr(psMacLoc->apmac,caMacStr1),keyword))
									{
											allCount++;
											if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
											{	
											   if(iCount>0){
														utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
												 }
												 iCount ++;
												 utPltPutLoopVar(psDbHead,"on_tMac",       iCount ,pasCvtMacStr(psMacLoc->mac,caMacStr));
												 utPltPutLoopVar(psDbHead,"on_uMac",       iCount ,pasCvtMacStr(psMacLoc->apmac,caMacStr1));
												 utPltPutLoopVarF(psDbHead,"on_sSignal",   iCount ,"%d",psMacLoc->lrssi);
												 utPltPutLoopVarF(psDbHead,"on_bSignal",   iCount ,"%d",psMacLoc->urssi);
												 utPltPutLoopVarF(psDbHead,"on_noise",     iCount ,"%d",psMacLoc->noise);
												 utPltPutLoopVarF(psDbHead,"on_channel",   iCount ,"%lu",psMacLoc->channel);	
												 utPltPutLoopVar(psDbHead,"on_stime",      iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psMacLoc->starttime));
												 utPltPutLoopVar(psDbHead,"on_etime",      iCount ,caSdate);
										  }
									}
								}
								else
								{
									  allCount++;
										if (allCount >= lStartRec && allCount < lCurPg*lRowNum)
										{	
										   if(iCount>0){
													utPltPutLoopVar(psDbHead,"dh",iCount+1,",");
											 }
											 iCount ++;
											 utPltPutLoopVar(psDbHead,"on_tMac",       iCount ,pasCvtMacStr(psMacLoc->mac,caMacStr));
											 utPltPutLoopVar(psDbHead,"on_uMac",       iCount ,pasCvtMacStr(psMacLoc->apmac,caMacStr1));
											 utPltPutLoopVarF(psDbHead,"on_sSignal",   iCount ,"%d",psMacLoc->lrssi);
											 utPltPutLoopVarF(psDbHead,"on_bSignal",   iCount ,"%d",psMacLoc->urssi);
											 utPltPutLoopVarF(psDbHead,"on_noise",     iCount ,"%d",psMacLoc->noise);
											 utPltPutLoopVarF(psDbHead,"on_channel",   iCount ,"%lu",psMacLoc->channel);	
											 utPltPutLoopVar(psDbHead,"on_stime",      iCount ,utTimFormat("%Y-%m-%d %H:%M:%S",psMacLoc->starttime));
											 utPltPutLoopVar(psDbHead,"on_etime",      iCount ,caSdate);
									  }
								}		
            }	
           
//            printf("%s  %s  %5d  %5d  %5d %5d %s %s\n",
//                    pasCvtMacStr(psMacLoc->mac,caMacStr),
//                    pasCvtMacStr(psMacLoc->apmac,caMacStr1),
//                    psMacLoc->lrssi,
//                    psMacLoc->urssi,
//                    psMacLoc->noise,
//                    psMacLoc->channel,
//                    utTimFormat("%Y-%m-%d %H:%M:%S",psMacLoc->starttime),
//                    caSdate);

            psMacLoc = (ncLbsMacLocation *)pasHashNextS(&sHashInfo);

        }

    }
    utPltPutVarF(psDbHead,"TotRec","%d",allCount);
//    utPltShowDb(psDbHead); 
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmonline/ncm_real_maclist.html");
    return 0;

}

int Ncm_OnlineInfo_SetFunName(utShmHead *psShmHead)
{
	 int iReturn;
	 iReturn = pasSetTcpFunName("ncmShowOnlineAllMac",ncmShowOnlineAllMac,0);                               //ÏÔÊ¾MacÐÅÏ¢
   return 0;	
}
