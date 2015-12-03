
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
//#include "proauth.h"

static char *strrstr(const char *haystack, const char *needle)
{                                                                                                                                          
        char *r = NULL;                                                                                                                    
                                                                                                                                           
        if (!needle[0])                                                                                                                    
                return (char*)haystack + strlen(haystack);                                                                                 
        while (1) {                                                                                                                        
                char *p = strstr(haystack, needle);                                                                                        
                if (!p)                                                                                                                    
                        return r;                                                                                                          
                r = p;                                                                                                                     
                haystack = p + 1;                                                                                                          
        }                                                                                                                                  
}
static int spmUtlUploadFile(char *pIn,char *pLocal,char *pRemote,char *pType)
{
        char *p,*p0;
        strcpy(pLocal,"\0");//本地上传文件名字
        strcpy(pRemote,"\0");//服务器存储位置
        strcpy(pType,"\0");
        p = strstr(pIn,";");
        if(*p == ';') {
                memcpy(pLocal,pIn,p-pIn);
        
                pLocal[p-pIn] = 0;
                p++;
                p0 = strstr(p,";");
                if(*p0 == ';') {
                        memcpy(pRemote,p,p0-p);
                        pRemote[p0-p] = 0;
                        strcpy(pType,p0+1);
                }
                else {
                        return 0;
                }
        }
        else 
        {
                return 0;
        }
        return 0;
}
//显示全路径模版文件

int utPltHtmlFileDbhead(int iFd,utMsgHead *psMsgHead,utPltDbHead *psDbHead,char *pFile)
{
    int  iFd1,iReturn,iFileSize;
    struct stat  sStatBuf;
    char *p,*pBuffer;
   
    iFd1 = open(pFile,O_RDONLY);
    if(iFd1 < 0) {
        utSysLog(" File %s Open Error ",pFile);
        return (-1);
    }

    iReturn = fstat(iFd1,&sStatBuf);
    if(iReturn == (-1)) {
        utSysLog(" fStat Error FileName == %s",pFile);
        close(iFd1);
        return 0;
    }
    iFileSize = sStatBuf.st_size;
    pBuffer = malloc(iFileSize + 1);
    if(pBuffer == NULL) {
        utSysLog(" Memoty Alloc Error in utPltFileToHtml");
        close(iFd1);
        return (-1);
    }
    iReturn = read(iFd1,pBuffer,iFileSize);
    close(iFd1);
    if(iReturn != iFileSize) {
        free(pBuffer);
        return (-1);
    }
    pBuffer[iFileSize]=0;
   
    p = utPltToHtmBuf(pBuffer,psDbHead);
    if(p) {
       utComTcpResponse(iFd,psMsgHead,1,
                       "html",UT_TYPE_STRING,p);
    }
    free(pBuffer);
    utPltFreeDb(psDbHead);
    return 0;
} 
//将逗号分割的字符拆分成一个数组
static int sepcharbydh2(char *in,char caOut[200][256])
{
	char *p,*p2;
	long lCount;
	long m=0;
	lCount=0;
	if(strlen(in)==0) return 0;
	p=in;
	while(*p){
	//	printf("lCount=%d\n",lCount);
		if(*p==','){
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

//广告模板
int ncAdplatelist_v8(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead){
#if SYSTEM_LOG
	utMsgPrintMsg(psMsgHead);
#endif
  char caPid[16];
  char caTemp[4024];
  char caKeyword[64];
  char caStart[32],caLimit[32],caSort[32],caDir[32];
  unsigned long lSid,lGroupid,lAddtime,lModtime,lCount;
  char caDel[16],caSelsid[1024];
  char caAdcode[32],caAdname[64],caAdurl[128],caAdfile[128],caAdfile_new[128],caDaterang[256],caMark[1024],caApssid[256];
  char caOut[200][256];
  char caPlatename[128],caAdindex[64],caPlatepath[128],caTermtype[32];
  char caSsid[128],caAp[128],caAc[128],caTermfac[64],caMobsp[32],caMobarea[32],caOs[64],caBrower[128],caDatarang[256];
  unsigned long lDid,lWay,lPri,lCid,cCount;
  char caGroups[2048],caCorps[2048];
  char caPlateid[24];
  long lCount_g=0;
  long lLangflag,lStatus;
  long lPid,i;
  long lTermtype;
  int iNum = 0;
  int iNum_s=0;
  int iret = -1;
	char caStatus[16];
	char caTemp_d[128];
	struct  stat st,st1;
  struct dirent *d,*d2;
  DIR *dir,*dir2;
  long m;
  char caFilename[256];
  char caTemp_g[2000],caTemp_c[2000];
  char caTemp1[4000];
  struct portalplate_s {
  	    char plateid[16];
  	    char platename[64];
  	    char mark[2048];
  	    char ssid[2048];
  	    char ap[2048];
  	    char groups[2048];
  	    char corps[2048];
 } portalplate; 
 char caAdPlate[128];
 char caIpPort[32];
 char caLang[36],caPath[64];
 char caFullfile[128];
  struct portalplate_s *pData;
  	sprintf(caIpPort,"%s:%s",utComGetVar_sd(psShmHead,"AdminIp",""),utComGetVar_sd(psShmHead,"AuthHttpPort",""));

  	    strcpy(caAdPlate,utComGetVar_sd(psShmHead,"adplate","/home/adplate"));
		    if(caAdPlate[strlen(caAdPlate)-1]!='/'){
		    	strcat(caAdPlate,"/");
		    }
  
  long iNum_t=0;
  long lSum=0;
	long iReturn;
	utMsgGetSomeNVar(psMsgHead,7,
		"pid",	UT_TYPE_STRING,  10, 		caPid,
		"status",UT_TYPE_STRING,10,caStatus,
		"del",      UT_TYPE_STRING,10,caDel,
		"selsid",   UT_TYPE_STRING,1000,caSelsid,
		"keyword",	UT_TYPE_STRING,  sizeof(caKeyword)-1, 		caKeyword,
		"start",		UT_TYPE_STRING,  sizeof(caStart)-1, 			caStart,
		"limit",		UT_TYPE_STRING,  sizeof(caLimit)-1, 			caLimit);
utMsgPrintMsg(psMsgHead);
	
		if(!utStrIsSpaces(caKeyword)){
			
			 strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
      	strcpy(caKeyword,caTemp_d);
			
	
	}
	
	char sql[1024] = "";
	char sql_tmp[1024] = "";
	pasDbCursor *psCur = NULL;
	utPltDbHead *psDbHead = utPltInitDb();

	
		  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
  /*     
         lCount=sepcharbydh2(caSelsid,caOut);
         for(iNum=0;iNum<lCount;iNum++){
              sprintf(caTemp,"select platepath from portplate where sid=%s ",caOut[iNum]);
              pasDbOneRecord(caTemp,0,UT_TYPE_STRING,127,caPlatepath);
              if(strstr(caPlatepath,"adplate")){
              	sprintf(caTemp,"rm -Rf %s",caPlatepath);
              	system(caTemp);
              }
          		iret = pasDbExecSqlF("delete from portplate where sid=%s ",caOut[iNum]);
          //		printf("iRet=%lu\n",iret);
          		if(iret==0){          			  
          			   pasDbExecSqlF("delete from portplateprocy where pid=%s ",caOut[iNum]);
          			   pasDbExecSqlF("delete from portplateindex where plateid=%s ",caOut[iNum]);
        //三层模板文件  			   
          			   
    //      			ncWebDelUserFromBuf(psShmHead,atol(caOut[iNum]));
          		}
          		
          }
          
          */
	  }
	
	
	
	
	

	
	snprintf(sql,sizeof(sql),"select count(*) from portalplate where 1=1  ");
	if(strlen(caKeyword)>0){
		sprintf(sql+strlen(sql)," and (platename like '%c%s%c' or plateid like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
	}
	pasDbOneRecord(sql,0,UT_TYPE_LONG,sizeof(long),&lCount);	
	
	
	pData=(struct portalplate_s *)malloc((lCount+1)*sizeof(struct portalplate_s));
	if(pData==NULL){
		return -1;
	}
	
	utPltPutVarF(psDbHead,"TotRec","%u",lCount);
	
	sprintf(sql,"select platename,plateid,mark,ssid,ap,groups,corps from portalplate where 1=1 ");
	if(strlen(caKeyword)>0){
		sprintf(sql+strlen(sql)," and (platename like '%c%s%c' or plateid like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
	}
	
	sprintf(sql+strlen(sql)," order by plateid desc  ");
	
printf("sql=%s\n",sql);	

	psCur = pasDbOpenSql(sql,0);
	
	if(NULL != psCur){
	
    memset(caPlatename,0,sizeof(caPlatename));
    memset(caPlateid,0,sizeof(caPlateid));
    memset(caMark,0,sizeof(caMark));
    memset(caSsid,0,sizeof(caSsid));
    memset(caAp,0,sizeof(caAp));
    memset(caAc,0,sizeof(caAc));
    memset(caGroups,0,sizeof(caGroups));
    memset(caCorps,0,sizeof(caCorps));
 
		iNum=0;
		while(0==(iret=pasDbFetchInto(psCur,
		  UT_TYPE_STRING,63,caPlatename,
		  UT_TYPE_STRING,16,caPlateid,
		  UT_TYPE_STRING,1000,caMark,
		  UT_TYPE_STRING,1000,caSsid,
		  UT_TYPE_STRING,1000,caAp,
		  UT_TYPE_STRING,1000,caGroups,
		  UT_TYPE_STRING,1000,caCorps)) || 1405 == iret){
			
			strcpy(pData[lSum].plateid,caPlateid);
			strcpy(pData[lSum].platename,caPlatename);
			strcpy(pData[lSum].mark,caMark);
			strcpy(pData[lSum].ssid,caSsid);
			strcpy(pData[lSum].ap,caAp);
			strcpy(pData[lSum].groups,caGroups);
			strcpy(pData[lSum].corps,caCorps);
			lSum++;
			
			 
			       memset(caPlatename,0,sizeof(caPlatename));
				    memset(caPlateid,0,sizeof(caPlateid));
				    memset(caMark,0,sizeof(caMark));
				    memset(caSsid,0,sizeof(caSsid));
				    memset(caAp,0,sizeof(caAp));
				    memset(caAc,0,sizeof(caAc));
				    memset(caGroups,0,sizeof(caGroups));
				    memset(caCorps,0,sizeof(caCorps));
			   
			   
		}
		pasDbCloseCursor(psCur);
		iNum=0;
		for(i=0;i<lSum;i++){
			iNum++;
			if(iNum>1){
				utPltPutLoopVar(psDbHead,"dh",iNum,",");
			}
			memset(caPlatepath,0,sizeof(caPlatepath));
			sprintf(caPlatepath,"%s%s",caAdPlate,pData[i].plateid);
			utPltPutLoopVar(psDbHead,"platepath",iNum,caPlatepath);
			utPltPutLoopVarF(psDbHead,"sid",iNum,"%s",pData[i].plateid);
			utPltPutLoopVar(psDbHead,"platename",iNum,pData[i].platename);
			utPltPutLoopVar(psDbHead,"mark",iNum,pData[i].mark);
		   utPltPutLoopVar(psDbHead,"ssid",iNum,pData[i].ssid);
		   utPltPutLoopVar(psDbHead,"ap",iNum,pData[i].ap);
		   utPltPutLoopVar(psDbHead,"groups",iNum,pData[i].groups);
		   utPltPutLoopVar(psDbHead,"corps",iNum,pData[i].corps);
			lCount_g=sepcharbydh(pData[i].groups,caOut);
		  memset(caTemp_g,0,sizeof(caTemp_g));
		   if(strstr(caGroups,"all")){
		   	strcpy(caTemp_g,"全部");
		   }
		   else{
		   	 for(m=0;m<lCount_g;m++){
			   	if(m==0){
			   		strcpy(caTemp_g,authGetGroupNameById(psShmHead,atol(caOut[m])));
			    }
			    else{
			   	  sprintf(caTemp_g+strlen(caTemp_g),",%s",authGetGroupNameById(psShmHead,atol(caOut[m])));
			    }
			   }
			 }
		   	
		  lCount_g=sepcharbydh(pData[i].corps,caOut);
		   	memset(caTemp_c,0,sizeof(caTemp_c));
	  	printf("corps=%s,count=%d\n",pData[i].corps,lCount_g);
		   if(strstr(caCorps,"all")){
		   	strcpy(caTemp_g,"全部");
		  }
		  else{

		   for(m=0;m<lCount_g;m++){
		   	if(m==0){
		   		strcpy(caTemp_c,authGetClientName(psShmHead,atol(caOut[m])));
		   }
		   else{
		   	sprintf(caTemp_c+strlen(caTemp_c),",%s",authGetClientName(psShmHead,atol(caOut[m])));
		    }
		   }
		  }
		  utPltPutLoopVar(psDbHead,"groupsname",iNum,caTemp_g);
		  utPltPutLoopVar(psDbHead,"corpsname",iNum,caTemp_c);
		  memset(caTemp,0,sizeof(caTemp));
		  if(strlen(pData[i].mark)>0){
			 	sprintf(caTemp+strlen(caTemp),"标签：[%s] ",pData[i].mark);
			} 
		  if(strlen(pData[i].ssid)>0){
			 	sprintf(caTemp+strlen(caTemp),"SSID：[%s] ",pData[i].ssid);
			} 
		  if(strlen(pData[i].ap)>0){
			 	sprintf(caTemp+strlen(caTemp),"AP：[%s] ",pData[i].ap);
			} 
		   
		  if(strlen(caTemp_g)>0){
			 	sprintf(caTemp+strlen(caTemp),"单位组：[%s] ",caTemp_g);
			} 
		  if(strlen(caTemp_c)>0){
			 	sprintf(caTemp+strlen(caTemp),"单位：[%s] ",caTemp_c);
			}  
			 strcpy(caTemp1,caTemp);
		   utPltPutLoopVar(psDbHead,"procy",iNum,caTemp1);
		   	
		   	
		//   	printf("caTemp=%s\n",caTemp);
		   	sprintf(caTemp,"select sid,lang,termtype,path from portplatetermlang where plateid='%s' order by termtype,lang ",pData[i].plateid);
		   	printf("caTemp=%s\n",caTemp);
		   	psCur = pasDbOpenSql(caTemp,0);
		   	if(psCur!=NULL){
		   		lSid=0;
		   		memset(caLang,0,sizeof(caLang));
		   		memset(caTermtype,0,sizeof(caTermtype));
		   		memset(caPath,0,sizeof(caPath));
		   		iNum_s=0;
		   		while(0==(iret=pasDbFetchInto(psCur,
					  UT_TYPE_LONG,4,&lSid,
					  UT_TYPE_STRING,sizeof(caLang)-1,caLang,
					  UT_TYPE_STRING,sizeof(caTermtype)-1,caTermtype,
					  UT_TYPE_STRING,sizeof(caPath)-1,caPath)) || 1405 == iret){
					  	
					  iNum_s++;
					  if(iNum_s>1){
				     utPltPutLoopVar2(psDbHead,"dhs",iNum,iNum_s,",");
			      }
					   utPltPutLoopVar2(psDbHead,"sid",iNum,iNum_s,pData[i].plateid);
					   sprintf(caTemp,"%s_%s",caTermtype,caLang);
					   utPltPutLoopVar2(psDbHead,"platename",iNum,iNum_s,caTemp);
//					   utPltPutLoopVar2(psDbHead,"procy",iNum,iNum_s,caTemp1);
//						 utPltPutLoopVar2(psDbHead,"mark",iNum,iNum_s,pData[i].mark);
//					   utPltPutLoopVar2(psDbHead,"ssid",iNum,iNum_s,pData[i].ssid);
//					   utPltPutLoopVar2(psDbHead,"ap",iNum,iNum_s,pData[i].ap);
//					   utPltPutLoopVar2(psDbHead,"groups",iNum,iNum_s,pData[i].groups);
//					   utPltPutLoopVar2(psDbHead,"corps",iNum,iNum_s,pData[i].corps);
					  
					   sprintf(caTemp,"%s%s",caTermtype,caLang);
					   utPltPutLoopVar(psDbHead,caTemp,iNum,caPath);
					   
					   
//					   utPltPutLoopVar2(psDbHead,caTemp,iNum,iNum_s,caPath);
					  
					  memset(caPlatepath,0,sizeof(caPlatepath));
			      sprintf(caPlatepath,"%s%s/%s",caAdPlate,pData[i].plateid,caPath);
			      utPltPutLoopVar2(psDbHead,"platepath",iNum,iNum_s,caPlatepath);
			      
			      
			      
			      
			      iReturn=stat(caPlatepath,&st);
			  if(iReturn==0){
			 	    if(S_ISDIR(st.st_mode) && (dir = opendir(caPlatepath))) {
			 	    	iNum_t=0;
		        while (d = readdir(dir)) {
		                    if (d->d_name[0] == '.') continue;
		                        sprintf(caFilename, "%s/%s", caPlatepath, d->d_name);
//		                        printf("caFilename=%s\n",caFilename);
		                        if (stat(caFilename,&st1) == 0) {

		                        		if(strstr(d->d_name,".htm")&&(!strstr(d->d_name,"htm."))&&(!strstr(d->d_name,"html."))){
			                           iNum_t++;
			                           if(iNum_t>1){
			                           	 utPltPutLoopVar3(psDbHead,"dht",iNum,iNum_s,iNum_t,","); 
			                           }
			                           utPltPutLoopVar3(psDbHead,"platename",iNum,iNum_s,iNum_t,d->d_name);
			                           utPltPutLoopVar3(psDbHead,"platepath",iNum,iNum_s,iNum_t,caPlatepath);
			                           utPltPutLoopVar3(psDbHead,"ipport",iNum,iNum_s,iNum_t,caIpPort);
			                           sprintf(caFullfile,"%s/%s",caPlatepath,d->d_name);
												         utPltPutLoopVar3(psDbHead,"pfile1",iNum,iNum_s,iNum_t,caFullfile);     	
	//		                           utPltPutLoopVar3(psDbHead,"procy",iNum,iNum_s,iNum_t,caTemp1);
			                         }
			                       }
			                     }
			                     closedir(dir);
			                   }
			               }
					 memset(caLang,0,sizeof(caLang));
		   		 memset(caTermtype,0,sizeof(caTermtype));
		   		 memset(caPath,0,sizeof(caPath));
					  
			  }
		     pasDbCloseCursor(psCur);
					  
		   	}
		 
			
		}
		
	}
		
   free(pData);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"v8/nc_adplate_list.htm");	
	return 0;
}

//保存广告模板
int ncAdPlate_Save_v8(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  utPltDbHead *psDbHead = NULL;
  
 	long iReturn,lSid;
  char caAdurl[256],caPlateindex[64];;
 	char caAdfile[128],caSid[12],caAdname[64],caAdcode[32],caDaterang[128],caApssid[1024],caGroupid[16];
 	unsigned long lTime = time(0);
 	char caPlateid[20],caPid[20],caAdindex[64];
 	char caPlatename[64],caPlatepath[128],caLang[12],caSsid[1024],caAp[1024];
 	char caAc[128],caDid[12],caDatarang[256],caTermfac[64],caMobsp[32],caMark[1024];
 	char caMobarea[32],caOs[64],caBrower[128],caWay[16],caPri[16],caCid[16],caLcount[16];
 	char caCorpname[128],caTermtype[64];
 	char caMbPlate[256];
 	char caTemp[1024];
 	char caGroups[1024],caCorps[1024];
 	char caAdPlate[128];
  long lCount;
  unsigned long lShopplateid;
 	utMsgPrintMsg(psMsgHead);

 psDbHead = utPltInitDbHead();

	utMsgGetSomeNVar(psMsgHead,7,
		                   "platename1",   UT_TYPE_STRING, sizeof(caPlatename)-1, caPlatename,
		                   "sid",UT_TYPE_STRING,sizeof(caPlateid)-1,caPlateid,
		                    "mark",UT_TYPE_STRING,sizeof(caMark)-1,caMark,
		                    "corps",UT_TYPE_STRING,sizeof(caCorps)-1,caCorps,
		                    "groups",UT_TYPE_STRING,sizeof(caGroups)-1,caGroups,
		                    "ssid",UT_TYPE_STRING,sizeof(caSsid)-1,caSsid,
		                    "ap",  UT_TYPE_STRING,sizeof(caAp)-1,caAp);
		                    
		    strcpy(caAdPlate,utComGetVar_sd(psShmHead,"adplate","/home/adplate"));
		    if(caAdPlate[strlen(caAdPlate)-1]!='/'){
		    	strcat(caAdPlate,"/");
		    }
		 		if(strlen(caPlateid)>0){
		  	  pasCvtGBK(2,caPlateid,caTemp,63);
		      strcpy(caPlateid,caTemp);
		  	
			
				 	if(!strstr(caPlateid,"英文字符")&&strlen(caPlateid)>0){
				 	  
	          sprintf(caTemp,"%s%s",caAdPlate,caPlateid);
		        if(!utFileIsExist(caTemp)){
		        	char caComm[128];
		        	sprintf(caComm,"mkdir -p %s",caTemp);
              system(caComm);
		        }
				 	}
				
		  	
		  }                     
	
		  lShopplateid=utMd5Code(caPlateid,strlen(caPlateid),"ffff");
      if(lShopplateid>2000000000) lShopplateid=lShopplateid-2000000000;
      if(lShopplateid>2000000000) lShopplateid=lShopplateid-2000000000;
         
             
		                    
		         
		  if(strlen(caPlatename)>0){
		  	  pasCvtGBK(2,caPlatename,caTemp,63);
		      strcpy(caPlatename,caTemp);
		  			  
		  	
		  }                 
		 
		  if(strlen(caMark)>0){
		  	  pasCvtGBK(2,caMark,caTemp,1000);
		      strcpy(caMark,caTemp);
		  	
				 	if(strstr(caMark,"多个")){
				 		strcpy(caMark,"");
				 	}
				 
			}
		  	
		
			if(strlen(caSsid)>0){
				 pasCvtGBK(2,caSsid,caTemp,1000);
		      strcpy(caSsid,caTemp);		  	
				 	if(strstr(caSsid,"多个")){
				 		strcpy(caSsid,"");
				 	}
						  	
		  }
			if(strlen(caAp)>0){
				 pasCvtGBK(2,caAp,caTemp,1000);
		      strcpy(caAp,caTemp);		  	
				 	if(strstr(caAp,"多个")){
				 		strcpy(caAp,"");
				 	}		  	
		  }
		
		
		 sprintf(caTemp,"select count(*) from portalplate where plateid='%s' ",caPlateid);
		 lCount=0;
		  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
		  
		
		  
		  
      if(lCount>0){
      	printf("update portalplate set platename='%s',mark='%s',ssid='%s',ap='%s',groups='%s',corps='%s',modtime=%d,shopplateid=%lu where plateid='%s' \n",
      	   caPlatename,caMark,caSsid,caAp,caGroups,caCorps,time(0),lShopplateid,caPlateid);
      	
      	iReturn=pasDbExecSqlF("update portalplate set platename='%s',mark='%s',ssid='%s',ap='%s',groups='%s',corps='%s',modtime=%d,shopplateid=%lu where plateid='%s' ",
      	   caPlatename,caMark,caSsid,caAp,caGroups,caCorps,time(0),lShopplateid,caPlateid);
      	   
      	
      }
      else{
      	
     printf("insert into portalplate(plateid,platename,mark,ssid,ap,groups,corps,addtime,modtime,shopplateid) \
      	   values('%s','%s','%s','%s','%s','%s','%s',%d,%d,%lu) \n",\
      	   caPlateid,caPlatename,caMark,caSsid,caAp,caGroups,caCorps,time(0),time(0),lShopplateid);
      	    
      	 iReturn=pasDbExecSqlF("insert into portalplate(plateid,platename,mark,ssid,ap,groups,corps,addtime,modtime,shopplateid) \
      	   values('%s','%s','%s','%s','%s','%s','%s',%d,%d,%lu) ",\
      	   caPlateid,caPlatename,caMark,caSsid,caAp,caGroups,caCorps,time(0),time(0),lShopplateid);
      	  
      	
      }
     if(iReturn==0){
    	utPltPutVar(psDbHead,"titel","true");
    	utPltPutVarF(psDbHead,"sid","%s",caPlateid);
      utPltPutVar(psDbHead,"platepath",caPlateid);
    	utPltPutVar(psDbHead,"message","保存成功");
    }
    else{
    	utPltPutVar(psDbHead,"titel","false");
    	utPltPutVarF(psDbHead,"sid","%s",caPlateid);
    	utPltPutVar(psDbHead,"message","保存失败");
    }

  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v8/nc_adfile.htm");	

}
int adStrReplaceWith(char *pBuf,char *s,char *d)
{
    char *p;
    int i=strlen(s);
    int j=strlen(d);
    int l;
    int iMax;
    int iCount=0;
    if(i>j){
        iMax=i;
    }
    else{
        iMax=j;
    }
    p = strstr(pBuf,s);
    while(p) {
        iCount++;
        if(i!=j) {
            l = strlen(p);
            memmove(p+j,p+i,l-i+1);
        }
        memcpy(p,d,j);
        p = strstr(p+iMax,s);
    }
    return iCount;
}

//广告模板广告位
int ncAdindexlist_v8(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead){

  char caPid[16];
  char caTemp[1024];
  char caKeyword[64];
  char caStart[32],caLimit[32],caSort[32],caDir[32];
  unsigned long lSid,lGroupid,lAddtime,lModtime,lCount;
  char caDel[16],caSelsid[1024];
  char caAdcode[32],caAdname[64],caAdurl[128],caAdfile[128],caAdfile_new[128],caDaterang[256],caMark[1024],caApssid[256];
  char caOut[200][256];
  char caPlatename[128],caAdindex[64],caPlatepath[128],caTermtype[32];
  long lLangflag,lStatus;
  char caFile[128],caPlateindex[128];
  long lPlateid;
  long lPid;
  int iNum = 0;
  int iret = -1;
	char caStatus[16];
	char caPfile[128];
	char caIpPort[32];
	long iReturn,iNum_s,iNum_t;
	struct  stat st,st1;
  struct dirent *d,*d2,*d3;
  DIR *dir,*dir2,*dir3;
  char caAd[128];
  FILE *fp,*fp1;
  char *p;
  char *pPath,*pBuf;
  unsigned long lSize;
  long lCount_t;
  char caFilename[256],caFilename3[256];
  char caAdPlate[128];
	sprintf(caIpPort,"%s:%s",utComGetVar_sd(psShmHead,"AdminIp",""),utComGetVar_sd(psShmHead,"AuthHttpPort",""));
	
	utMsgGetSomeNVar(psMsgHead,6,
		"pid",	UT_TYPE_STRING,  15, 		caPid,
		"del",      UT_TYPE_STRING,10,caDel,
		"selsid",   UT_TYPE_STRING,1000,caSelsid,
		"keyword",	UT_TYPE_STRING,  sizeof(caKeyword)-1, 		caKeyword,
		"start",		UT_TYPE_STRING,  sizeof(caStart)-1, 			caStart,
		"limit",		UT_TYPE_STRING,  sizeof(caLimit)-1, 			caLimit);
utMsgPrintMsg(psMsgHead);
	
	char sql[1024] = "";
	char sql_tmp[1024] = "";
	char caTemp2[1024];
	pasDbCursor *psCur = NULL;
	utPltDbHead *psDbHead = utPltInitDb();

	
		  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
          
         lCount=sepcharbydh2(caSelsid,caOut);
         for(iNum=0;iNum<lCount;iNum++){
   //     printf("delete from portadisstype where pid=%s \n",caOut[iNum]); 
          		iret = pasDbExecSqlF("delete from portplateindex where sid=%s ",caOut[iNum]);
          	
          		
          }
	  }
	
	
	if(strlen(caPid)>0){
		  	 strcpy(caAdPlate,utComGetVar_sd(psShmHead,"adplate","/home/adplate"));
		    if(caAdPlate[strlen(caAdPlate)-1]!='/'){
		    	strcat(caAdPlate,"/");
		    }
		    memset(caPlatepath,0,sizeof(caPlatepath));
		    sprintf(caPlatepath,"%s%s",caAdPlate,caPid);
		    
		    
		    
	//	printf("caPlatepath1=%s\n",caPlatepath);
		 iReturn=stat(caPlatepath,&st);
			 if(iReturn==0){
			 	    if(S_ISDIR(st.st_mode) && (dir = opendir(caPlatepath))) {
			 	    	iNum=0;
		        while (d = readdir(dir)) {
		                    if (d->d_name[0] == '.'||strstr(d->d_name,"tmp")||strstr(d->d_name,"images")||strstr(d->d_name,"css")||strstr(d->d_name,"js")) continue;
		                        sprintf(caFilename, "%s/%s", caPlatepath, d->d_name);
		                        if (stat(caFilename,&st1) == 0) {
		                        	 if(S_ISDIR(st1.st_mode) && (dir3 = opendir(caFilename))) {
		                        	 	
	//	                        	 	printf("dname=%s\n",d->d_name);
		                        	 	    iNum++;
		                        	 	    if(iNum>1){
		                        	 	        utPltPutLoopVar(psDbHead,"dh",iNum,",");
		                        	 	       
		                        	 	    }
		                        	 	     utPltPutLoopVar(psDbHead,"pfile",iNum,d->d_name);
		                        	 	     utPltPutLoopVar(psDbHead,"plateid",iNum,caPid);
		                        	 	     utPltPutLoopVar(psDbHead,"platepath",iNum,caPlatepath);
		                        	 	     iNum_s=0;
		                        	 	   while (d3 = readdir(dir3)) {
		                                 if (d3->d_name[0] == '.') continue;
		                        	 	
		                        	 	
		                        	 	
		        //                	 printf("caFileName=%s,caCback=%s\n",caFileName,caCback);
		                        	         if(strstr(d3->d_name,".htm")&&(!strstr(d3->d_name,"htm."))&&(!strstr(d3->d_name,"html."))){
			                                    iNum_s++;
			                                    sprintf(caFilename3,"%s/%s",caFilename,d3->d_name);
		                //    printf("caFileName3=%s\n",caFilename3);         
							                         		if(iNum_s>1){
																						utPltPutLoopVar2(psDbHead,"dhs",iNum,iNum_s,",");
																					}
							                            utPltPutLoopVar2(psDbHead,"pfile1",iNum,iNum_s,caFilename3);
							                            utPltPutLoopVar2(psDbHead,"pfile",iNum,iNum_s,d3->d_name);
							                            utPltPutLoopVar2(psDbHead,"plateid",iNum,iNum_s,caPid);
				
		                        	 	          utPltPutLoopVar2(psDbHead,"platepath",iNum,iNum_s,caFilename);
							                            
							                            utPltPutLoopVar2(psDbHead,"ipport",iNum,iNum_s,caIpPort);
		                           					  fp = fopen(caFilename3,"r");
										            					if(fp) {
										             					lSize = utFileSize(caFilename3);
										            
		                             					pBuf = malloc(lSize+4000);
										             					memset(pBuf,0,lSize+1);
										             					fread(pBuf,lSize,1,fp);
										             					 p=strstr(pBuf,"\"advert_");
										 //             printf("pBuf=%s\n",pBuf);
										                			iNum_t=0;
										                			memset(caTemp2,0,sizeof(caTemp2));
										              				while(p){
										              				ncUtlGetWordBetween(p,"\"","\"",caAd,127);
										            	printf("caAd=%s\n",caAd);
										              				ncUtlGetWordBetween(p,"src=\"","\"",caAdfile,127);
										           	printf("caFile=%s\n",caAdfile);
										           	if(strlen(caAdfile)>0){
										           		sprintf(caTemp,"http://[#ip#]:[#port#]/pronline/Msg?FunName@ncmFirstDispAd&tsid@[#tsid#]&ad@%s",caAd);
										           		
										           		adStrReplaceWith(pBuf,caAdfile,caTemp);
										           	}
										              				iNum_t++;
													              	if(iNum_t>1){
													              		utPltPutLoopVar3(psDbHead,"dht",iNum,iNum_s,iNum_t,",");
													              	}
													              	if(iNum_t==1){
													              		
													              		strcpy(caTemp2,caAd);
													              	}
													              	else{
													              		sprintf(caTemp2+strlen(caTemp2),",%s",caAd);
													              	}
													              	
										              				utPltPutLoopVar3(psDbHead,"pfile",iNum,iNum_s,iNum_t,caFilename);
												              		utPltPutLoopVar3(psDbHead,"plateindex",iNum,iNum_s,iNum_t,caAd);
												              		utPltPutLoopVar3(psDbHead,"plateid",iNum,iNum_s,iNum_t,caPid);
												              		utPltPutLoopVar2(psDbHead,"platepath",iNum,iNum_s,caFilename);
												              		utPltPutLoopVar3(psDbHead,"file",iNum,iNum_s,iNum_t,caAdfile);
												              	  utPltPutLoopVar3(psDbHead,"ipport",iNum,iNum_s,iNum_t,caIpPort);
												              	
													              	lCount_t=0;
													              
													              										              		 		char caAdfile_t[68];
										              		 		if(strstr(caAdfile,"FunName")){
										              		 			strcpy(caAdfile_t,"");
										              		 		}
										              		 		else{
										              		 			strcpy(caAdfile_t,caAdfile);
										              		 		}
													              
													              
													              	sprintf(caTemp,"select count(*) from portalplateindex where plateid='%s' and plateindex='%s'  ",caPid,caAd);
													              	pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount_t);
													              	                       
													              	if(lCount_t>0){
													              		
													              		if(!strstr(caAdfile,"FunName")){
													              		pasDbExecSqlF("update portalplateindex set defad='%s' where plateid='%s' and plateindex='%s' ",caAdfile,caPid,caAd);
													              	  }
													              	  
													              	}
													              	else{
										              				printf("insert into portalplateindex(plateid,plateindex,defad,defsecad,sectitle,secdesc,addtime,modtime) \
										              		 		values('%s','%s','%s','','','',%d,%d)",caPid,caAd,caAdfile,time(0),time(0));

										              				pasDbExecSqlF("insert into portalplateindex(plateid,plateindex,defad,defsecad,sectitle,secdesc,addtime,modtime) \
										              		 		values('%s','%s','%s','','','',%d,%d)",caPid,caAd,caAdfile_t,time(0),time(0));
										              				}
										              	
										              	
										              	
										              	 			p=strstr(p+1,"\"advert");
										              	 
										              	 
										              				}
										                      if(strlen(caTemp2)>0){
										                      	utPltPutLoopVar2(psDbHead,"plateindex",iNum,iNum_s,caTemp2);
										                      }
						                              fclose(fp);
						                              
						                               fp = fopen(caFilename3,"w");
                                                    if(fp) {
                                                      fprintf(fp,pBuf);
                                                        fflush(fp);
                                                        fclose(fp);
                                                     }
						                              
						                              free(pBuf);
						                              
						                              
						                              
		                          						}
		                        				}
		                            }
		                          closedir(dir3);   
		                        }
		                    }
		                  }
		               closedir(dir);   
	        }
	   }
	 }
	   utPltPutVarF(psDbHead,"TotRec","%d",iNum_s);

		utPltOutToHtml(iFd,psMsgHead,psDbHead,"v8/nc_adplateindex_list.htm");	
		
	return 0;
}

/* 编辑页面代码  */
int proauthShowPlateBm(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{
 
  
    utPltDbHead *psDbHead;
    int iReturn;
    char *p,*pFile,*pBuf;
    char caFile[128],caPlateid[16];
     long lSize;
     char caLang[36];
    FILE *fp;
     utMsgGetSomeNVar(psMsgHead,2,
		"file",	UT_TYPE_STRING,  127, 		caFile,
		"plateid",      UT_TYPE_STRING,10,caPlateid);
    lSize=0;
    lSize = utFileSize(caFile);
    if(lSize<0) lSize=128;
    psDbHead = utPltInitDb();
    utPltPutVar(psDbHead,"file",caFile);
    utPltPutVar(psDbHead,"plateid",caPlateid);
    fp = fopen(caFile,"r");
    if(fp) {
        pBuf = malloc(lSize+1);
        memset(pBuf,0,lSize+1);
        fread(pBuf,lSize,1,fp);
        fclose(fp);
        memset(caLang,0,sizeof(caLang));
        ncUtlGetWordBetween(pBuf,"charset=","\"",caLang,30);
      
        if((strcasecmp(caLang,"utf-8")==0)||strcasecmp(caLang,"utf8")==0){
        	  char *pTemp= malloc(lSize*2+1);
        	  memset(pTemp,0,sizeof(pTemp));
        	  pasCvtGBK(2,pBuf,pTemp,lSize*2);
        	  utPltPutVar(psDbHead,"keyword",pTemp);
        	  free(pTemp);
        }
        else{
          utPltPutVar(psDbHead,"keyword",pBuf);
        }
        free(pBuf);
    }
    else {
        printf(" File %s does not exist\n",caFile);
    }
       
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v8/nc_platecode.htm");
    return 0;
}
//保存编辑代码
int proauthShowPlateBmSave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char *pBuf;
    char *pFile;
    char *p;
    char caFile[128];
    long lSize;
    char caPlateid[16],caLang[16];
    FILE *fp;
    utMsgGetSomeNVar(psMsgHead,2,
		"file",	UT_TYPE_STRING,  127, 		caFile,
		"plateid",      UT_TYPE_STRING,10,caPlateid);
    pBuf = utMsgGetVar_s(psMsgHead,"keyword");
printf(" Buf==%s\n",pBuf);
    if(pBuf) {

    	   
    	
        fp = fopen(caFile,"w");
        if(fp) {
        	
        	   ncUtlGetWordBetween(pBuf,"charset=","\"",caLang,30);
			    	 if((strcasecmp(caLang,"utf-8")==0)||strcasecmp(caLang,"utf8")==0){
			    	 	   lSize=strlen(pBuf);
			        	  char *pTemp= malloc(lSize*2+1);
			        	  memset(pTemp,0,sizeof(pTemp));
			    	      pasCharsetCvt(PAS_CCODE_GB,PAS_CCODE_UTF8,pBuf,pTemp,2*lSize);
			    	      fwrite(pTemp,strlen(pTemp),1,fp);
			    	      free(pTemp);
			    	 }
        	   else{
        	
               fwrite(pBuf,strlen(pBuf),1,fp);
             }
            fclose(fp);
        }
    }  
    proauthShowPlateBm(psShmHead,iFd,psMsgHead);
    return 0;
}


//显示当前模版子目录
int ncAdPlate_dir(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead){

  char caPid[20];
  char caTemp[1024];
  char caKeyword[64];
  char caStart[32],caLimit[32],caSort[32],caDir[32];
  unsigned long lSid,lGroupid,lAddtime,lModtime,lCount;
  char caDel[16],caSelsid[1024];
  char caAdcode[32],caAdname[64],caAdurl[128],caAdfile[128],caAdfile_new[128],caDaterang[256],caMark[1024],caApssid[256];
  char caOut[200][256];
  char caPlatename[128],caAdindex[64],caPlatepath[128],caTermtype[32];
  long lLangflag,lStatus;
  char caFile[128],caPlateindex[128];
  long lPlateid;
  long lPid;
  int iNum = 0;
  int iret = -1;
	char caStatus[16];
	char caPfile[128];
	char caIpPort[32];
	long iReturn,iNum_s;
	struct  stat st,st1;
  struct dirent *d,*d2;
  DIR *dir,*dir2;
  char caAd[128];
  FILE *fp,*fp1;
  char *p;
  char *pPath,*pBuf;
  unsigned long lSize;
  char caFilename[256];
	char caAdPlate[128];
	utMsgGetSomeNVar(psMsgHead,1,
		"pid",	UT_TYPE_STRING,  16, 		caPid);
		
utMsgPrintMsg(psMsgHead);
	
	char sql[1024] = "";
	char sql_tmp[1024] = "";
	pasDbCursor *psCur = NULL;
	utPltDbHead *psDbHead = utPltInitDb();
  iNum=0;
	
	 if(strlen(caPid)>0){
			   strcpy(caAdPlate,utComGetVar_sd(psShmHead,"adplate","/home/adplate"));
		    if(caAdPlate[strlen(caAdPlate)-1]!='/'){
		    	strcat(caAdPlate,"/");
		    }
		    sprintf(caPlatepath,"%s%s",caAdPlate,caPid);
	iNum_s=0;
	printf("caPlatepath=%s\n",caPlatepath);

	if(strlen(caPlatepath)>0){
		 iReturn=stat(caPlatepath,&st);
			 if(iReturn==0){
			 	    if(S_ISDIR(st.st_mode) && (dir = opendir(caPlatepath))) {
			 	    	iNum=0;
		        while (d = readdir(dir)) {
		                    if (d->d_name[0] == '.'||strstr(d->d_name,"tmp")||strstr(d->d_name,"images")||strstr(d->d_name,"css")||strstr(d->d_name,"js")) continue;
		                        sprintf(caFilename, "%s/%s", caPlatepath, d->d_name);
		                        if (stat(caFilename,&st1) == 0) {
		                        	 if(S_ISDIR(st1.st_mode)){
		                        	 	printf("caFilename=%s\n",d->d_name);
		                        
			                         iNum++;
		                       //     printf("caFileName=%s\n",caFilename);         
		                         		if(iNum>1){
																	utPltPutLoopVar(psDbHead,"dh",iNum,",");
																}
		                            utPltPutLoopVar(psDbHead,"text",iNum,d->d_name);
		                           
		                            
										              
										             
		                          }
		                        }
		                      }
		                      
		                      
		                    
		                  closedir(dir);   
	        }
	    }
	 }
	}
	   utPltPutVarF(psDbHead,"TotRec","%d",iNum);

		utPltOutToHtml(iFd,psMsgHead,psDbHead,"v8/proauth_addir_list.htm");	
		
	return 0;
}

//保存广告模板终端也语言类型选择
int ncAdPlateTerm_Save_v8(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  utPltDbHead *psDbHead = NULL;
  
 	long iReturn,lSid;
  char caPcch[36],caSid[20],caPceng[36],caPcdefault[36];
  char caMobilech[36],caMobileeng[36],caMobiledefault[36],caPadch[36],caPadeng[36],caPaddefault[36];
  char caDefaultch[36],caDefaulteng[36],caPcbig5[32],caMobilebig5[36],caPadbig5[36],caDefaultbig5[36],caDefaultdefault[36];
  char caTemp[1024];
 	utMsgPrintMsg(psMsgHead);

 psDbHead = utPltInitDbHead();

	utMsgGetSomeNVar(psMsgHead,17,
		                    "pcch",   UT_TYPE_STRING, sizeof(caPcch)-1, caPcch,
		                    "sid",UT_TYPE_STRING,sizeof(caSid)-1,caSid,
		                    "pceng",  UT_TYPE_STRING, sizeof(caPceng)-1, caPceng,
		                    "pcdefault",UT_TYPE_STRING,sizeof(caPcdefault)-1,caPcdefault,
		                    "mobilech",UT_TYPE_STRING,sizeof(caMobilech)-1,caMobilech,
		                    "mobileeng",UT_TYPE_STRING,sizeof(caMobileeng)-1,caMobileeng,
		                    "mobiledefault",UT_TYPE_STRING,sizeof(caMobiledefault)-1,caMobiledefault,
		                    "padch",  UT_TYPE_STRING,sizeof(caPadch)-1,caPadch,
		                    "padeng",UT_TYPE_STRING,sizeof(caPadeng)-1,caPadeng,
		                    "paddefault",UT_TYPE_STRING,sizeof(caPaddefault)-1,caPaddefault,
		                    "defaultch",UT_TYPE_STRING,sizeof(caDefaultch)-1,caDefaultch,
		                    "defaulteng",UT_TYPE_STRING,sizeof(caDefaulteng)-1,caDefaulteng,
		                    "pcbig5",   UT_TYPE_STRING,sizeof(caPcbig5)-1,caPcbig5,
		                    "mobilebig5",UT_TYPE_STRING,sizeof(caMobilebig5)-1,caMobilebig5,
		                    "padbig5",UT_TYPE_STRING,sizeof(caPadbig5)-1,caPadbig5,
		                    "defaultbig5",UT_TYPE_STRING,sizeof(caDefaultbig5)-1,caDefaultbig5,
		                    "defaultdefault",UT_TYPE_STRING,sizeof(caDefaultdefault)-1,caDefaultdefault
		                    );
		                                   
		  
		 		if(strlen(caPcch)>0){
		  	  pasCvtGBK(2,caPcch,caTemp,30);
		      strcpy(caPcch,caTemp);
		    }
		  	if(strlen(caPceng)>0){
		  	  pasCvtGBK(2,caPceng,caTemp,30);
		      strcpy(caPceng,caTemp);
		    }
				if(strlen(caPcdefault)>0){
		  	  pasCvtGBK(2,caPcdefault,caTemp,30);
		      strcpy(caPcdefault,caTemp);
		    }
		  	if(strlen(caMobilech)>0){
		  	  pasCvtGBK(2,caMobilech,caTemp,30);
		      strcpy(caMobilech,caTemp);
		    }
				if(strlen(caMobileeng)>0){
		  	  pasCvtGBK(2,caMobileeng,caTemp,30);
		      strcpy(caMobileeng,caTemp);
		    }
		  	if(strlen(caMobiledefault)>0){
		  	  pasCvtGBK(2,caMobiledefault,caTemp,30);
		      strcpy(caMobiledefault,caTemp);
		    }
				if(strlen(caPadch)>0){
		  	  pasCvtGBK(2,caPadch,caTemp,30);
		      strcpy(caPadch,caTemp);
		    }
		  	if(strlen(caPadeng)>0){
		  	  pasCvtGBK(2,caPadeng,caTemp,30);
		      strcpy(caPadeng,caTemp);
		    }
				if(strlen(caPaddefault)>0){
		  	  pasCvtGBK(2,caPaddefault,caTemp,30);
		      strcpy(caPaddefault,caTemp);
		    }
		  	if(strlen(caDefaultch)>0){
		  	  pasCvtGBK(2,caDefaultch,caTemp,30);
		      strcpy(caDefaultch,caTemp);
		    }
				if(strlen(caDefaulteng)>0){
		  	  pasCvtGBK(2,caDefaulteng,caTemp,30);
		      strcpy(caDefaulteng,caTemp);
		    }
		  	
				if(strlen(caPcbig5)>0){
		  	  pasCvtGBK(2,caPcbig5,caTemp,30);
		      strcpy(caPcbig5,caTemp);
		    }
				if(strlen(caMobilebig5)>0){
		  	  pasCvtGBK(2,caMobilebig5,caTemp,30);
		      strcpy(caMobilebig5,caTemp);
		    }
		    if(strlen(caPadbig5)>0){
		  	  pasCvtGBK(2,caPadbig5,caTemp,30);
		      strcpy(caPadbig5,caTemp);
		    }
		    
		    if(strlen(caDefaultbig5)>0){
		  	  pasCvtGBK(2,caDefaultbig5,caTemp,30);
		      strcpy(caDefaultbig5,caTemp);
		    }
		    if(strlen(caDefaultdefault)>0){
		  	  pasCvtGBK(2,caDefaultdefault,caTemp,30);
		      strcpy(caDefaultdefault,caTemp);
		    }
		    
				 	if(!strstr(caPcch,"选择模板")&&strlen(caPcch)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='zh' and termtype='mspc' ",caSid);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caPcch,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','zh','mspc','%s')",caSid,caPcch);
				 		}
				 	}
	         
	        	if(!strstr(caPceng,"选择模板")&&strlen(caPceng)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='eng' and termtype='mspc' ",caSid);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caPceng,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) valus('%s','eng','mspc','%s')",caSid,caPceng);
				 		}
				 	} 
	         
	        if(!strstr(caPcdefault,"选择模板")&&strlen(caPcdefault)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='default' and termtype='mspc' ",caSid);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caPcdefault,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','default','mspc','%s')",caSid,caPcdefault);
				 		}
				 	} 
	         
	       	if(!strstr(caMobilech,"选择模板")&&strlen(caMobilech)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='zh' and termtype='mobile' ",caSid);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caMobilech,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','zh','mobile','%s')",caSid,caMobilech);
				 		}
				 	}  
	         
	        if(!strstr(caMobileeng,"选择模板")&&strlen(caMobileeng)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='eng' and termtype='mobile' ",caSid);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caMobileeng,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','eng','mobile','%s')",caSid,caMobileeng);
				 		}
				 	}   
	         
	        if(!strstr(caMobiledefault,"选择模板")&&strlen(caMobiledefault)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='default' and termtype='mobile' ",caSid);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caMobiledefault,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','default','mobile','%s')",caSid,caMobiledefault);
				 		}
				 	}   
	         
	       	if(!strstr(caPadch,"选择模板")&&strlen(caPadch)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='zh' and termtype='pad' ",caSid);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caPadch,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','zh','pad','%s')",caSid,caPadch);
				 		}
				 	}   
			
				    if(!strstr(caPadeng,"选择模板")&&strlen(caPadeng)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='eng' and termtype='pad' ",caSid);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caPadeng,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','eng','pad','%s')",caSid,caPadeng);
				 		}
				 	} 
			
					if(!strstr(caPaddefault,"选择模板")&&strlen(caPaddefault)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='default' and termtype='pad' ",caSid);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caPaddefault,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','default','pad','%s')",caSid,caPaddefault);
				 		}
				 	} 
			
				if(!strstr(caDefaultch,"选择模板")&&strlen(caDefaultch)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='zh' and termtype='default' ",caSid);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caDefaultch,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','zh','default','%s')",caSid,caDefaultch);
				 		}
				 	} 
			
					if(!strstr(caDefaulteng,"选择模板")&&strlen(caDefaulteng)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='eng' and termtype='default' ",caSid);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caDefaulteng,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','eng','default','%s')",caSid,caDefaulteng);
				 		}
				 	}
		
		
						if(!strstr(caPcbig5,"选择模板")&&strlen(caPcbig5)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='big5' and termtype='mspc' ",caSid);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caPcbig5,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','big5','mspc','%s')",caSid,caPcbig5);
				 		}
				 	}
		
					if(!strstr(caMobilebig5,"选择模板")&&strlen(caMobilebig5)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='big5' and termtype='mobile' ",caSid);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caMobilebig5,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','big5','mobile','%s')",caSid,caMobilebig5);
				 		}
				 	}
		
					if(!strstr(caPadbig5,"选择模板")&&strlen(caPadbig5)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='big5' and termtype='pad' ",caPadbig5);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caPadbig5,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','big5','pad','%s')",caSid,caPadbig5);
				 		}
				 	}

					if(!strstr(caDefaultbig5,"选择模板")&&strlen(caDefaultbig5)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='big5' and termtype='default' ",caDefaultbig5);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caDefaultbig5,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','big5','default','%s')",caSid,caDefaultbig5);
				 		}
				 	}

					if(!strstr(caDefaultdefault,"选择模板")&&strlen(caDefaultdefault)>0){
				 		sprintf(caTemp,"select sid from portplatetermlang where plateid='%s' and lang='default' and termtype='default' ",caDefaultdefault);
				 		lSid=0;
				 		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
				 		if(lSid>0){
				 			iReturn=pasDbExecSqlF("update portplatetermlang set path='%s' where sid=%d ",caDefaultdefault,lSid);
				 		}
				 		else{
				 			iReturn=pasDbExecSqlF("insert into portplatetermlang(plateid,lang,termtype,path) values('%s','default','default','%s')",caSid,caDefaultdefault);
				 		}
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

//预览广告位文件
int ncShowAdIndex_v8(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  utPltDbHead *psDbHead = NULL;
  char caSid[16];
	char caTemp[1024];
	unsigned long lPlateid,iReturn;
	char caPlateindex[64],caFile[128],caPfile[128],caPlatepath[128];
	char caPlateid[20];
	psDbHead = utPltInitDbHead(); 
  utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,2, "plateid",   UT_TYPE_STRING, 15, caPlateid,
	                                        "file", UT_TYPE_STRING,127,caPfile);
	     printf("pFile=%s\n",caPfile);
	 utPltPutVar(psDbHead,"ip",utComGetVar_sd(psShmHead,"AdminIp",""));
	 utPltPutVar(psDbHead,"port",utComGetVar_sd(psShmHead,"AuthHttpPort",""));  
		/*                       
  sprintf(caTemp,"select plateid,plateindex,file,pfile,platepath from portplateindex a,portplate b where a.plateid=b.sid and a.sid=%s ",caSid);
  printf("caTemp=%s\n",caTemp);
  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lPlateid,
                          UT_TYPE_STRING,63,caPlateindex,
                          UT_TYPE_STRING,127,caFile,
                          UT_TYPE_STRING,127,caPfile,
                          UT_TYPE_STRING,127,caPlatepath);
  char caAdpath[256];
   char *pPath;
   pPath=strstr(caPlatepath,"/adplate/"); 
  if(pPath){
	  sprintf(caAdpath,"%s%s",pPath,caFile);
	  utPltPutVar(psDbHead,caPlateindex,caAdpath);
  }          
   sprintf(caTemp,"%s/html/%s",caPlatepath,caPfile); 
   */   
//   printf("caTemp=%s\n",caTemp);  
   utPltHtmlFileDbhead(iFd,psMsgHead,psDbHead,caPfile);
 //   utPltOutToHtml(iFd,psMsgHead,psDbHead,caTemp);	
 //  utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","true","上传成功");

}
//上传广告位文件

int ncAdIndex_upload_v8(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  utPltDbHead *psDbHead = NULL;
 	char caFile[256] = "";
 	char sid[17] = "";
 	char *p;
 	char caLocal[128] = "";
 	char caRemote[128] = "";
 	char caRemote_new[128] = "";
 	char caType[128] = "";
 	char caDfile[128];
 	long iReturn,lSid;
 	char caSid[20];
 	char caPath[128];
 	char caTemp[256],caPlatepath[256]; 	
 	char caFileName[128];
  DIR *dir,*dir2;
 	struct  stat st,st1;
  struct dirent *d,*d2;
  char caAd[128],caAdPlate[128],caPlatepathtmp[128];
  FILE *fp,*fp1;
  unsigned long lSize;
  long index;
 	unsigned long ltime = time(0);
	memset(caDfile,0,sizeof(caDfile));
	
utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,2,
		                   "adfile",   UT_TYPE_STRING, sizeof(caFile)-1, caFile,
		                    "sid",  UT_TYPE_STRING, sizeof(sid)-1, sid);

	
	
		  if(strlen(caFile)>0){
		  	char *descr_gbk = pasConvert("UTF-8", "GBK", caFile);
				 if(descr_gbk){
				 	strcpy(caFile,descr_gbk);
				  free(descr_gbk);
				}
		  	
		  }
        lSid=atol(sid);
        spmUtlUploadFile(caFile,caLocal,caRemote,caType);
        printf(" caFile==[%s]\n",caFile);
        printf("caLocal=%s,caRemote=%s\n",caLocal,caRemote);
        
   //获取模板目标路径     
       strcpy(caAdPlate,utComGetVar_sd(psShmHead,"adplate","/home/adplate"));
		    if(caAdPlate[strlen(caAdPlate)-1]!='/'){
		    	strcat(caAdPlate,"/");
		    }
		    memset(caPlatepath,0,sizeof(caPlatepath));
		    sprintf(caPlatepath,"%s%s",caAdPlate,sid);
        
        
        /* 取得文件名*/
       
        p = strrstr(caRemote, "/");
        if(p != NULL)
        {        
        	strcpy(caDfile,++p);
                
        }
             		  
		     if(!utFileIsExist(caPlatepath)) 
        {
                if(mkdir(caPlatepath,0755)!=0)
                {
                       utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","false","创建模板路径出错");
                        return 0;
                }
        
        }   
		    sprintf(caPlatepathtmp,"%s/tmp",caPlatepath);
		    if(!utFileIsExist(caPlatepathtmp)) 
        {
                if(mkdir(caPlatepathtmp,0755)!=0)
                {
                       utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","false","1创建模板路径出错");
                        return 0;
                }
        
        }
        else{
        	printf("caPlaatepathtmp=%s\n",caPlatepathtmp);
        	if(strstr(caPlatepathtmp,"tmp")){
        	sprintf(caTemp,"rm -Rf %s",caPlatepathtmp);
        	system(caTemp);
        	       if(mkdir(caPlatepathtmp,0755)!=0)
                {
                       utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","false","2创建模板路径出错");
                        return 0;
                }
        }
      }
        //将上传的文件拷到临时目录下，检查是否符合目录要求
        snprintf(caRemote_new, sizeof(caRemote_new), "%s/%s", caPlatepathtmp,caDfile);
		    printf("caRemote_new=%s\n",caRemote_new);
		    rename(caRemote, caRemote_new); 
      if(strstr(caRemote_new,".zip")){
    	  sprintf(caTemp,"unzip -o %s -d %s",caRemote_new,caPlatepathtmp);
    	  system(caTemp);
      }
       else if(strstr(caRemote_new,".tgz")){
       	 sprintf(caTemp,"tar xvfz %s -C %s",caRemote_new,caPlatepathtmp);
    	   system(caTemp);
     }
     //校验
     sprintf(caTemp,"%s/%s",caPlatepathtmp,sid);
     printf("caTemp=%s\n",caTemp);
     if(!utFileIsExist(caTemp)) {
     	 utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","false","上传文件包里没有包含模板代码的目录");
       return 0;
    }
     if(strstr(caRemote_new,".zip")){
    	  sprintf(caTemp,"unzip -o %s -d %s",caRemote_new,caAdPlate);
    	  system(caTemp);
      }
      else{
      	 sprintf(caTemp,"tar xvfz %s -C %s",caRemote_new,caAdPlate);
    	   system(caTemp);
      }
        
        
    psDbHead = utPltInitDbHead(); 
    iReturn=0;
    if(iReturn==0){
    	utPltPutVar(psDbHead,"titel","true");
    
    	utPltPutVar(psDbHead,"adfile",caDfile);
    	utPltPutVar(psDbHead,"message","上传成功");
    }
    else{
    	utPltPutVar(psDbHead,"titel","false");
    
    	utPltPutVar(psDbHead,"message","上传失败");
    }

    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v8/nc_adfile.htm");	
 //  utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","true","上传成功");

}

//上传默认广告位文件
int ncAdDefAd_upload_v8(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  utPltDbHead *psDbHead = NULL;
 	char caFile[256] = "";
 	char sid[17] = "";
 	char *p;
 	char caLocal[128] = "";
 	char caRemote[128] = "";
 	char caRemote_new[128] = "";
 	char caType[128] = "";
 	char caDfile[128];
 	long iReturn,lSid;
 	char caSid[20];
 	char caPath[128];
 	char caTemp[256],caPlatepath[256]; 	
 	char caFileName[128];
  DIR *dir,*dir2;
 	struct  stat st,st1;
  struct dirent *d,*d2;
  char caAd[128],caAdPlate[128],caPlatepathtmp[128];
  char caNewfile[128];
  FILE *fp,*fp1;
  unsigned long lSize;
  long index;
  char caPlateid[32],caPlateindex[32];
 	unsigned long lTime = time(0);
	memset(caDfile,0,sizeof(caDfile));
	
utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,3,
		                   "plateid",   UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid,
		                   "index",UT_TYPE_STRING,sizeof(caPlateindex)-1,caPlateindex,
		                   "adfile",UT_TYPE_STRING,sizeof(caFile)-1,caFile);
		                
	
	
		  if(strlen(caFile)>0){
		  	char *descr_gbk = pasConvert("UTF-8", "GBK", caFile);
				 if(descr_gbk){
				 	strcpy(caFile,descr_gbk);
				  free(descr_gbk);
				}
		  	
		  }
        
        spmUtlUploadFile(caFile,caLocal,caRemote,caType);
        printf(" caFile==[%s]\n",caFile);
        printf("caLocal=%s,caRemote=%s\n",caLocal,caRemote);
        
  strcpy(caAdPlate,utComGetVar_sd(psShmHead,"adplate","/home/adplate"));
	    if(caAdPlate[strlen(caAdPlate)-1]=='/'){
	    	caAdPlate[strlen(caAdPlate)-1]=='\0';
		    }
		     
        
        
        
       
        p = strrstr(caRemote, "/");
        if(p != NULL)
        {        
        	strcpy(caDfile,++p);
                
        }
        sprintf(caPlatepath,"%s/%s/images",caAdPlate,caPlateid);    		  
		    if(!utFileIsExist(caPlatepath)) 
        {
                if(mkdir(caPlatepath,0755)!=0)
                {
                       utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","false","创建模板路径出错");
                        return 0;
                }
        
        } 
/*  
		    sprintf(caPlatepathtmp,"%s/tmp",caPlatepath);
		    if(!utFileIsExist(caPlatepathtmp)) 
        {
                if(mkdir(caPlatepathtmp,0755)!=0)
                {
                       utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","false","1创建模板路径出错");
                        return 0;
                }
        
        }
        else{
        	printf("caPlaatepathtmp=%s\n",caPlatepathtmp);
        	if(strstr(caPlatepathtmp,"tmp")){
        	sprintf(caTemp,"rm -Rf %s",caPlatepathtmp);
        	system(caTemp);
        	       if(mkdir(caPlatepathtmp,0755)!=0)
                {
                       utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","false","2创建模板路径出错");
                        return 0;
                }
        }
      }
  */
        //将上传的文件重名
        sprintf(caNewfile,"/adplate/%s/images/%s_%d_%s",caPlateid,caPlateindex,lTime,caDfile);
        snprintf(caRemote_new, sizeof(caRemote_new), "%s/%s_%d_%s", caPlatepath,caPlateindex,lTime,caDfile);
		    printf("caRemote_new=%s\n",caRemote_new);
		    printf("caRemote=%s\n",caRemote);
		    rename(caRemote, caRemote_new); 
     
       iReturn=pasDbExecSqlF("update portalplateindex set defad='%s' where plateid='%s' and plateindex='%s' ",
        caNewfile,caPlateid,caPlateindex);
        
        
    psDbHead = utPltInitDbHead(); 
  
    if(iReturn==0){
    	utPltPutVar(psDbHead,"titel","true");
    
    	utPltPutVar(psDbHead,"adfile",caNewfile);
    	utPltPutVar(psDbHead,"message","上传成功");
    }
    else{
    	utPltPutVar(psDbHead,"titel","false");
    
    	utPltPutVar(psDbHead,"message","上传失败");
    }

    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v8/nc_adfile.htm");	


}


int proauth_portplate(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
        // printf("ncm_portplate start!!!!\n");
         pasDbCursor *psCur;
        // utPltDbHead *psDbHead;
        char caPlateid[32];
        utPltDbHead *psDbHead;
        psDbHead= utPltInitDb();
        // utMsgPrintMsg(psMsgHead);
        char update[32];
        char sid[32];

        char caStyped[32] = "";
        dsCltGetSessionValue(1, "stype", UT_TYPE_STRING,sizeof(caStyped)-1, caStyped);
        // printf("caStyped = %s\n", caStyped);

        utMsgGetSomeNVar(psMsgHead, 2,
                                 "update",        UT_TYPE_STRING,  sizeof(update)-1,       update,
                                 "sid",        UT_TYPE_STRING,  sizeof(sid)-1,       sid
                                 );
        char caTemp[256];
        int total;
        //sprintf(caTemp,"select count(*) from portplate");
        sprintf(caTemp,"select count(*) from portalplate");
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, sizeof(total)-1, &total);
        utPltPutVarF(psDbHead,"TotRec","%d",total);


        //sprintf(caTemp,"select sid, platename,status from portplate");
        sprintf(caTemp,"select shopplateid, platename,plateid from portalplate where 1=1");
      
       psCur = pasDbOpenSql(caTemp,0);

        if(psCur == NULL){
                // printf("pscur is null\n");
                // printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
        }
        if(psCur != NULL)
        {

                int iReturn=0;
                int iNum=0;
                long id =0;
                long status =0;
                char name[64]="";

                while(0==(iReturn = pasDbFetchInto(psCur,
                            UT_TYPE_LONG,sizeof(id),&id,
                            UT_TYPE_STRING,64,name,
                            UT_TYPE_STRING,15,caPlateid
                                                        //UT_TYPE_LONG,sizeof(status),&status
                                                        ))  || 1405==iReturn)
                {
                iNum++;
                        if(iNum > 1)
                                {
                                        utPltPutLoopVar(psDbHead,"dh",iNum,",");
                                }
                utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu", id);
                //utPltPutLoopVarF(psDbHead,"status",iNum,"%lu", status);
                utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu", iNum);
                //char *nameUTF = convert("GBK", "UTF-8", name);
                utPltPutLoopVarF(psDbHead,"platename",iNum,"%s",name);
                utPltPutLoopVar(psDbHead,"plateid",iNum,caPlateid);
                }

                pasDbCloseCursor(psCur);
        }
//              utPltShowDb(psDbHead);
                utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ad/ncm_portplate.html");


          return 0;
}
//显示广告位

int proauthAdindexlist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
    char caTemp_d[64];
 	  pasDbCursor *psCur;
    char caPlateid[32],caPlateindex[32],caDefad[64],caFwidth[16],caFheight[16],caDefsecad[64];
    char caSwidth[16],caSheight[16],caSectitle[128],caSecdesc[128],caFtitle[128],caFdesc[128];
    char caUrl[128],caSurl[128];
   lCurPg=1;
 
	utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,1,
			"plateid", UT_TYPE_STRING,15,caPlateid);
   	 
   	
   	
    utStrDelSpaces(caPlateid); 
     if(strlen(caPlateid)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caPlateid,caTemp,15));
      	strcpy(caPlateid,caTemp_d);
     }
  
   

	
	    sprintf(sqlbuf,"select count(*) from portalplateindex where plateid='%s' ",caPlateid);
	   
    
	   lCount=0;
	   pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lCount);
  
	   sprintf(sqlbuf,"select plateid,plateindex,defad,fwidth,fheight,defsecad,swidth,sheight,sectitle,secdesc,ftitle,fdesc,url,surl from portalplateindex where plateid='%s' order by plateindex ",caPlateid);
	   
 printf("sqlbuf=%s\n",sqlbuf);
 psCur = pasDbOpenSql(sqlbuf,0);
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsg_pasmsg.htm","用户查询","查询数据出错 %s",pasDbErrorMsg(NULL));
        return 0;
    }
	 	
  
  psDbHead = utPltInitDb();
    
        iReturn = 0;
        memset(caPlateid,0,sizeof(caPlateid));
        memset(caPlateindex,0,sizeof(caPlateindex));
        memset(caDefad,0,sizeof(caDefad));
        memset(caFwidth,0,sizeof(caFwidth));
        memset(caFheight,0,sizeof(caFheight));
        memset(caDefsecad,0,sizeof(caDefsecad));
        memset(caSwidth,0,sizeof(caSwidth));
        memset(caSheight,0,sizeof(caSheight));
        memset(caSectitle,0,sizeof(caSectitle));
        memset(caSecdesc,0,sizeof(caSecdesc));
			  memset(caFtitle,0,sizeof(caFtitle));
			  memset(caFdesc,0,sizeof(caFdesc));
			  memset(caUrl,0,sizeof(caUrl));
			  memset(caSurl,0,sizeof(caSurl));
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,15,caPlateid,
			                                   UT_TYPE_STRING,15,caPlateindex,
			                                   UT_TYPE_STRING,63,caDefad,
			                                   UT_TYPE_STRING,10,caFwidth,
			                                   UT_TYPE_STRING,10,caFheight,
			                                   UT_TYPE_STRING,63,caDefsecad,
			                                   UT_TYPE_STRING,10,caSwidth,
			                                   UT_TYPE_STRING,10,caSheight,
			                                   UT_TYPE_STRING,127,caSectitle,
			                                   UT_TYPE_STRING,127,caSecdesc,
			                                   UT_TYPE_STRING,127,caFtitle,
			                                   UT_TYPE_STRING,127,caFdesc,
			                                   UT_TYPE_STRING,127,caUrl,
			                                   UT_TYPE_STRING,127,caSurl);
			                                  

			                                  
                                   
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
	        utPltPutLoopVar(psDbHead,"plateid",iNum,caPlateid);
	        utPltPutLoopVar(psDbHead,"plateindex",iNum,caPlateindex);
	        utPltPutLoopVar(psDbHead,"defad",iNum,caDefad);
	        utPltPutLoopVar(psDbHead,"fwidth",iNum,caFwidth);
	        utPltPutLoopVar(psDbHead,"fheight",iNum,caFheight);
	        utPltPutLoopVar(psDbHead,"defsecad",iNum,caDefsecad);
	        utPltPutLoopVar(psDbHead,"swidth",iNum,caSwidth);
	        utPltPutLoopVar(psDbHead,"sheight",iNum,caSheight);
	        utPltPutLoopVar(psDbHead,"sectitle",iNum,caSectitle);
	        utPltPutLoopVar(psDbHead,"secdesc",iNum,caSecdesc);
          utPltPutLoopVar(psDbHead,"ftitle",iNum,caFtitle);
          utPltPutLoopVar(psDbHead,"fdesc",iNum,caFdesc);
          utPltPutLoopVar(psDbHead,"url",iNum,caUrl);
          utPltPutLoopVar(psDbHead,"surl",iNum,caSurl);
         
	     
	      
        memset(caPlateid,0,sizeof(caPlateid));
        memset(caPlateindex,0,sizeof(caPlateindex));
        memset(caDefad,0,sizeof(caDefad));
        memset(caFwidth,0,sizeof(caFwidth));
        memset(caFheight,0,sizeof(caFheight));
        memset(caDefsecad,0,sizeof(caDefsecad));
        memset(caSwidth,0,sizeof(caSwidth));
        memset(caSheight,0,sizeof(caSheight));
        memset(caSectitle,0,sizeof(caSectitle));
        memset(caSecdesc,0,sizeof(caSecdesc));
			  
			  memset(caFtitle,0,sizeof(caFtitle));
			  memset(caFdesc,0,sizeof(caFdesc));
			  memset(caUrl,0,sizeof(caUrl));
			  memset(caSurl,0,sizeof(caSurl));
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,15,caPlateid,
			                                   UT_TYPE_STRING,15,caPlateindex,
			                                   UT_TYPE_STRING,63,caDefad,
			                                   UT_TYPE_STRING,10,caFwidth,
			                                   UT_TYPE_STRING,10,caFheight,
			                                   UT_TYPE_STRING,63,caDefsecad,
			                                   UT_TYPE_STRING,10,caSwidth,
			                                   UT_TYPE_STRING,10,caSheight,
			                                   UT_TYPE_STRING,127,caSectitle,
			                                   UT_TYPE_STRING,127,caSecdesc,
			                                   UT_TYPE_STRING,127,caFtitle,
			                                   UT_TYPE_STRING,127,caFdesc,
			                                   UT_TYPE_STRING,127,caUrl,
			                                   UT_TYPE_STRING,127,caSurl);
    }



    lTotPg = (lCount -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lCount);
     
    pasDbCloseCursor(psCur);
    
        
//     utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ad/ncm_portplateindex.html");
    return 0;
}

//保存广告位缺省广告
int ncAdPlateIndex_Save(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  utPltDbHead *psDbHead = NULL;
  
 	long iReturn,lSid;
  char caAdurl[256],caPlateindex[64];;
 	char caAdfile[128],caSid[12],caAdname[64],caAdcode[32],caDaterang[128],caApssid[1024],caGroupid[16];
 	unsigned long lTime = time(0);
 	char caPlateid[20],caPid[20],caAdindex[64];
 	char caFtitle[128],caFdesc[128],caFwidth[20],caFheight[20],caDefad[128];
 	char caTemp[1024];
 	char caGroups[1024],caCorps[1024];
 	char caAdPlate[128];
  long lCount;
  char caDel[16];
  char caUrl[128];
 	utMsgPrintMsg(psMsgHead);

 psDbHead = utPltInitDbHead();

	utMsgGetSomeNVar(psMsgHead,9,
		                   "plateid",   UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid,
		                   "plateindex", UT_TYPE_STRING,sizeof(caPlateindex)-1,caPlateindex,
		                   "ftitle",     UT_TYPE_STRING,sizeof(caFtitle)-1,caFtitle,
		                   "fdesc",      UT_TYPE_STRING,sizeof(caFdesc)-1,caFdesc,
		                   "fwidth",     UT_TYPE_STRING,sizeof(caFwidth)-1,caFwidth,
		                   "fheight",    UT_TYPE_STRING,sizeof(caFheight)-1,caFheight,
		                   "defad",      UT_TYPE_STRING,sizeof(caDefad)-1,caDefad,
		                   "url",        UT_TYPE_STRING,sizeof(caUrl)-1,caUrl,
		                   "del",        UT_TYPE_STRING,8,caDel);
		                   
		                    
		    strcpy(caAdPlate,utComGetVar_sd(psShmHead,"adplate","/home/adplate"));
		    if(caAdPlate[strlen(caAdPlate)-1]!='/'){
		    	strcat(caAdPlate,"/");
		    }
		 		if(strlen(caPlateid)>0){
		  	  pasCvtGBK(2,caPlateid,caTemp,63);
		      strcpy(caPlateid,caTemp);
		  	}
			
				
				
		  	
		                       
	
		            
		             
		 
		  if(strlen(caFtitle)>0){
		  	  pasCvtGBK(2,caFtitle,caTemp,127);
		      strcpy(caFtitle,caTemp);
		    }
		  	
				
		  	
		
			if(strlen(caFdesc)>0){
				 pasCvtGBK(2,caFdesc,caTemp,127);
		     					  	
		  }
			
		
		if(strcmp(caDel,"del")==0){
			sprintf(caTemp,"select defad from portalplateindex where plateid='%s' and plateindex='%s' ",caPlateid,caPlateindex);
			printf("caTemp=%s\n",caTemp);
			memset(caDefad,0,sizeof(caDefad));
			pasDbOneRecord(caTemp,0,UT_TYPE_STRING,63,caDefad);
			if(strstr(caDefad,"adplate")){
			sprintf(caTemp,"/home%s",caDefad);
			printf("caTemp=%s\n",caTemp);
			remove(caTemp);
			iReturn=pasDbExecSqlF("update portalplateindex set defad='',ftitle='',fdesc='',fwidth='',fheight='',modtime=%d,url='' where plateid='%s' and plateindex='%s' ",time(0),caPlateid,caPlateindex);
		  }
		}
		else{
		 sprintf(caTemp,"select count(*) from portalplateindex where plateid='%s' and plateindex='%s' ",caPlateid,caPlateindex);
		 lCount=0;
		  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
		  
		
		  
		  
      if(lCount>0){
   printf("update portalplateindex set defad='%s',fwidth='%s',fheight='%s',ftitle='%s',fdesc='%s',modtime=%d  where plateid='%s' and plateindex='%s' \n",
      	   caDefad,caFwidth,caFheight,caFtitle,caFdesc,time(0),caPlateid,caPlateindex);
      	      		
      	iReturn=pasDbExecSqlF("update portalplateindex set fwidth='%s',fheight='%s',ftitle='%s',fdesc='%s',modtime=%d,url='%s'  where plateid='%s' and plateindex='%s' ",
      	   caFwidth,caFheight,caFtitle,caFdesc,time(0),caUrl,caPlateid,caPlateindex);
      	   
      	
      }
    }
     if(iReturn==0){
    	utPltPutVar(psDbHead,"titel","true");
    	utPltPutVarF(psDbHead,"sid","%s",caPlateid);
      utPltPutVar(psDbHead,"platepath",caPlateid);
    	utPltPutVar(psDbHead,"message","保存成功");
    }
    else{
    	utPltPutVar(psDbHead,"titel","false");
    	utPltPutVarF(psDbHead,"sid","%s",caPlateid);
    	utPltPutVar(psDbHead,"message","保存失败");
    }

  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v8/nc_adfile.htm");	

}


//保存二级广告位缺省广告
int ncAdPlateIndex_Sec_Save(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  utPltDbHead *psDbHead = NULL;
  
 	long iReturn,lSid;
  char caAdurl[256],caPlateindex[64];;
 	char caAdfile[128],caSid[12],caAdname[64],caAdcode[32],caDaterang[128],caApssid[1024],caGroupid[16];
 	unsigned long lTime = time(0);
 	char caPlateid[20],caPid[20],caAdindex[64];
 	char caStitle[128],caSdesc[128],caSwidth[20],caSheight[20],caDefsecad[128];
 	char caTemp[1024]; 	char caGroups[1024],caCorps[1024];
 	char caAdPlate[128];
  long lCount;
  char caDel[16];
  char caSurl[128];
 	utMsgPrintMsg(psMsgHead);

 psDbHead = utPltInitDbHead();

	utMsgGetSomeNVar(psMsgHead,8,
		                   "plateid",   UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid,
		                   "plateindex", UT_TYPE_STRING,sizeof(caPlateindex)-1,caPlateindex,
		                   "stitle",     UT_TYPE_STRING,sizeof(caStitle)-1,caStitle,
		                   "sdesc",      UT_TYPE_STRING,sizeof(caSdesc)-1,caSdesc,
		                   "swidth",     UT_TYPE_STRING,sizeof(caSwidth)-1,caSwidth,
		                   "sheight",    UT_TYPE_STRING,sizeof(caSheight)-1,caSheight,
		                   "del",        UT_TYPE_STRING,8,caDel,
		                   "surl",       UT_TYPE_STRING,127,caSurl);
		                   
		                    
		 
		 		if(strlen(caPlateid)>0){
		  	  pasCvtGBK(2,caPlateid,caTemp,63);
		      strcpy(caPlateid,caTemp);
		  	}
			            
		 
		  if(strlen(caStitle)>0){
		  	  pasCvtGBK(2,caStitle,caTemp,127);
		      strcpy(caStitle,caTemp);
		    }
		  	
				
		  	
		
			if(strlen(caSdesc)>0){
				 pasCvtGBK(2,caSdesc,caTemp,127);
		     					  	
		  }
			
		
		if(strcmp(caDel,"del")==0){
			sprintf(caTemp,"select defsecad from portalplateindex where plateid='%s' and plateindex='%s' ",caPlateid,caPlateindex);
			printf("caTemp=%s\n",caTemp);
			memset(caDefsecad,0,sizeof(caDefsecad));
			pasDbOneRecord(caTemp,0,UT_TYPE_STRING,63,caDefsecad);
			if(strstr(caDefsecad,"adplate")){
			sprintf(caTemp,"/home%s",caDefsecad);
			printf("caTemp=%s\n",caTemp);
			remove(caTemp);
			
		  }
		  iReturn=pasDbExecSqlF("update portalplateindex set defsecad='',sectitle='',secdesc='',swidth='',sheight='',modtime=%d,surl='' where plateid='%s' and plateindex='%s' ",time(0),caPlateid,caPlateindex);
		}
		else{
		 sprintf(caTemp,"select count(*) from portalplateindex where plateid='%s' and plateindex='%s' ",caPlateid,caPlateindex);
		 lCount=0;
		  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
		  
		
		  
		  
      if(lCount>0){
printf("update portalplateindex set swidth='%s',sheight='%s',stitle='%s',sdesc='%s',modtime=%d  where plateid='%s' and plateindex='%s' \n",
      	   caSwidth,caSheight,caStitle,caSdesc,time(0),caPlateid,caPlateindex);       	      		
      	iReturn=pasDbExecSqlF("update portalplateindex set swidth='%s',sheight='%s',sectitle='%s',secdesc='%s',modtime=%d,surl='%s'  where plateid='%s' and plateindex='%s' ",
      	   caSwidth,caSheight,caStitle,caSdesc,time(0),caSurl,caPlateid,caPlateindex);
      	   
      	
      }
    }
     if(iReturn==0){
    	utPltPutVar(psDbHead,"titel","true");
    	utPltPutVarF(psDbHead,"sid","%s",caPlateid);
      utPltPutVar(psDbHead,"platepath",caPlateid);
    	utPltPutVar(psDbHead,"message","保存成功");
    }
    else{
    	utPltPutVar(psDbHead,"titel","false");
    	utPltPutVarF(psDbHead,"sid","%s",caPlateid);
    	utPltPutVar(psDbHead,"message","保存失败");
    }

  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v8/nc_adfile.htm");	

}

//上传二级广告位文件
int ncAdDefAd_upload_sec(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  utPltDbHead *psDbHead = NULL;
 	char caFile[256] = "";
 	char sid[17] = "";
 	char *p;
 	char caLocal[128] = "";
 	char caRemote[128] = "";
 	char caRemote_new[128] = "";
 	char caType[128] = "";
 	char caDfile[128];
 	long iReturn,lSid;
 	char caSid[20];
 	char caPath[128];
 	char caTemp[256],caPlatepath[256]; 	
 	char caFileName[128];
  DIR *dir,*dir2;
 	struct  stat st,st1;
  struct dirent *d,*d2;
  char caAd[128],caAdPlate[128],caPlatepathtmp[128];
  char caNewfile[128];
  FILE *fp,*fp1;
  unsigned long lSize;
  long index;
  char caPlateid[32],caPlateindex[32];
 	unsigned long lTime = time(0);
	memset(caDfile,0,sizeof(caDfile));
	
utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,3,
		                   "plateid",   UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid,
		                   "index",UT_TYPE_STRING,sizeof(caPlateindex)-1,caPlateindex,
		                   "adfile",UT_TYPE_STRING,sizeof(caFile)-1,caFile);
		                
	
	
		  if(strlen(caFile)>0){
		  	char *descr_gbk = pasConvert("UTF-8", "GBK", caFile);
				 if(descr_gbk){
				 	strcpy(caFile,descr_gbk);
				  free(descr_gbk);
				}
		  	
		  }
        
        spmUtlUploadFile(caFile,caLocal,caRemote,caType);
        printf(" caFile==[%s]\n",caFile);
        printf("caLocal=%s,caRemote=%s\n",caLocal,caRemote);
        
  strcpy(caAdPlate,utComGetVar_sd(psShmHead,"adplate","/home/adplate"));
	    if(caAdPlate[strlen(caAdPlate)-1]=='/'){
	    	caAdPlate[strlen(caAdPlate)-1]=='\0';
		    }
		     
        
        
        
       
        p = strrstr(caRemote, "/");
        if(p != NULL)
        {        
        	strcpy(caDfile,++p);
                
        }
        sprintf(caPlatepath,"%s/%s/images",caAdPlate,caPlateid);    		  
		    if(!utFileIsExist(caPlatepath)) 
        {
                if(mkdir(caPlatepath,0755)!=0)
                {
                       utWebDispMsg(iFd,psMsgHead,"v8/ncmsg_pasmsg.htm","false","创建模板路径出错");
                        return 0;
                }
        
        } 

        //将上传的文件重名
        sprintf(caNewfile,"/adplate/%s/images/%s_%d_%s",caPlateid,caPlateindex,lTime,caDfile);
        snprintf(caRemote_new, sizeof(caRemote_new), "%s/%s_%d_%s", caPlatepath,caPlateindex,lTime,caDfile);
		    printf("caRemote_new=%s\n",caRemote_new);
		    printf("caRemote=%s\n",caRemote);
		    rename(caRemote, caRemote_new); 
     
       iReturn=pasDbExecSqlF("update portalplateindex set defsecad='%s' where plateid='%s' and plateindex='%s' ",
        caNewfile,caPlateid,caPlateindex);
        
        
    psDbHead = utPltInitDbHead(); 
  
    if(iReturn==0){
    	utPltPutVar(psDbHead,"titel","true");
    
    	utPltPutVar(psDbHead,"adfile",caNewfile);
    	utPltPutVar(psDbHead,"message","上传成功");
    }
    else{
    	utPltPutVar(psDbHead,"titel","false");
    
    	utPltPutVar(psDbHead,"message","上传失败");
    }

    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v8/nc_adfile.htm");	


}

//显示广告素材信息
int adResourcelist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
    char caTemp[1024];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime,l,lTime1,lSip,lDip;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn;
   	long lRowNum,lflags,lStartRec,lCount;
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
  char caGroupid[20],caDid[20];
  char caGroupids[2048],caDel[20],caSelsid[512];
  unsigned long lSid,lFlag;
  char caName[32],caUrl[128],caFtitle[128],caFdesc[128],caFimg[128];
  char caFwidth[16],caFheight[16],caStitle[128],caSdesc[128],caSimg[128],caSwidth[16],caSheight[16],caSurl[128];
  char caCorpid[128];
  char caPlateid[32],caPlateindex[32],caCorps[1024],caSsid[1024],caMark[1024],caDatarang[1024];
  unsigned long sids[250];
  long lTtype,lStype,iret,lScount;
   lCurPg=1;
 
	utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,11,
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
   	  "del",          UT_TYPE_STRING,10,caDel);

   	
  
    utStrDelSpaces(caKeyword); 
    
    
      if(strlen(caKeyword)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
      	strcpy(caKeyword,caTemp_d);
      }
           
      lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

    if(strcmp(caDel,"del")==0){
    	pasDbExecSqlF("delete from adresource where sid in (%s) ",caSelsid);
    }
 
    
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;    	
   

	
	    sprintf(sqlbuf,"select count(*) from adresource where 1=1 ");
	 
    	if(strlen(caKeyword)>0){
    		sprintf(sqlbuf+strlen(sqlbuf),"and (name like '%c%s%c' or url like '%c%s%c' or ftitle like '%c%s%c' or fdesc like '%c%s%c' )",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
    	}
    
	lCount=0;
	pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lCount);
  
	sprintf(sqlbuf,"select sid,name,url,ftitle,fdesc,fimg,fwidth,fheight,stitle,sdesc,simg,corpid,flag,swidth,sheight,surl from adresource where 1=1 ");
	    if(strlen(caKeyword)>0){
    		sprintf(sqlbuf+strlen(sqlbuf),"and (name like '%c%s%c' or url like '%c%s%c' or ftitle like '%c%s%c' or fdesc like '%c%s%c' )",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
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
			  lSid=0;
			  lFlag=0;
			  memset(caName,0,sizeof(caName));
			  memset(caUrl,0,sizeof(caUrl));
			  memset(caFtitle,0,sizeof(caFtitle));
			  memset(caFdesc,0,sizeof(caFdesc));
			  memset(caFimg,0,sizeof(caFimg));
			  memset(caFwidth,0,sizeof(caFwidth));
			  memset(caFheight,0,sizeof(caFheight));
			  memset(caStitle,0,sizeof(caStitle));
			  memset(caSdesc,0,sizeof(caSdesc));
			  memset(caSimg,0,sizeof(caSimg));
			  memset(caCorpid,0,sizeof(caCorpid));
			  memset(caSwidth,0,sizeof(caSwidth));
			  memset(caSheight,0,sizeof(caSheight));
			  memset(caSurl,0,sizeof(caSurl));
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lSid,
			                                   UT_TYPE_STRING,31,caName,
			                                   UT_TYPE_STRING,127,caUrl,
			                                   UT_TYPE_STRING,127,caFtitle,
			                                   UT_TYPE_STRING,127,caFdesc,
			                                   UT_TYPE_STRING,127,caFimg,
			                                   UT_TYPE_STRING,10,caFwidth,
			                                   UT_TYPE_STRING,10,caFheight,
			                                   UT_TYPE_STRING,127,caStitle,
			                                   UT_TYPE_STRING,127,caSdesc,
			                                   UT_TYPE_STRING,127,caSimg,
			                                   UT_TYPE_STRING,127,caCorpid,
			                                   UT_TYPE_LONG,4,&lFlag,
			                                   UT_TYPE_STRING,10,caSwidth,
			                                   UT_TYPE_STRING,10,caSheight,
			                                   UT_TYPE_STRING,127,caSurl);
			                                

			                                  
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
    	    sids[iNum]=lSid;
    	    if(iNum>0){
        		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
          }
          else{
          	utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
          }
	        iNum++;
	        
	        utPltPutLoopVarF(psDbHead,"num", iNum,"%lu",iNum);
          utPltPutLoopVarF(psDbHead,"sid", iNum,"%lu",lSid);
          utPltPutLoopVarF(psDbHead,"flag",iNum,"%lu",lFlag);
          utPltPutLoopVar(psDbHead,"url",iNum,caUrl);
          utPltPutLoopVar(psDbHead,"surl",iNum,caSurl);
          utPltSetCvtHtml(1);
          utPltPutLoopVar(psDbHead,"name",iNum,caName);
          
          utPltPutLoopVar(psDbHead,"ftitle",iNum,caFtitle);
          utPltPutLoopVar(psDbHead,"fdesc",iNum,caFdesc);
          utPltPutLoopVar(psDbHead,"fimg",iNum,caFimg);
          utPltPutLoopVar(psDbHead,"fwidth",iNum,caFwidth);
      
          utPltPutLoopVar(psDbHead,"fheight",iNum,caFheight);
          utPltPutLoopVar(psDbHead,"stitle",iNum,caStitle);
          utPltPutLoopVar(psDbHead,"sdesc",iNum,caSdesc);
          utPltPutLoopVar(psDbHead,"simg",iNum,caSimg);
          utPltPutLoopVar(psDbHead,"swidth",iNum,caSwidth);
          utPltPutLoopVar(psDbHead,"sheight",iNum,caSheight);
         
          utPltSetCvtHtml(0);
         
	     
	      
         
			  lSid=0;
			  lFlag=0;
			  memset(caName,0,sizeof(caName));
			  memset(caUrl,0,sizeof(caUrl));
			  memset(caFtitle,0,sizeof(caFtitle));
			  memset(caFdesc,0,sizeof(caFdesc));
			  memset(caFimg,0,sizeof(caFimg));
			  memset(caFwidth,0,sizeof(caFwidth));
			  memset(caFheight,0,sizeof(caFheight));
			  memset(caStitle,0,sizeof(caStitle));
			  memset(caSdesc,0,sizeof(caSdesc));
			  memset(caSimg,0,sizeof(caSimg));
			  memset(caCorpid,0,sizeof(caCorpid));
			
			
			  memset(caSwidth,0,sizeof(caSwidth));
			  memset(caSheight,0,sizeof(caSheight));
			
			  memset(caSurl,0,sizeof(caSurl));
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lSid,
			                                   UT_TYPE_STRING,31,caName,
			                                   UT_TYPE_STRING,127,caUrl,
			                                   UT_TYPE_STRING,127,caFtitle,
			                                   UT_TYPE_STRING,127,caFdesc,
			                                   UT_TYPE_STRING,127,caFimg,
			                                   UT_TYPE_STRING,10,caFwidth,
			                                   UT_TYPE_STRING,10,caFheight,
			                                   UT_TYPE_STRING,127,caStitle,
			                                   UT_TYPE_STRING,127,caSdesc,
			                                   UT_TYPE_STRING,127,caSimg,
			                                   UT_TYPE_STRING,127,caCorpid,
			                                   UT_TYPE_LONG,4,&lFlag,
			                                   UT_TYPE_STRING,10,caSwidth,
			                                   UT_TYPE_STRING,10,caSheight,
			                                   UT_TYPE_STRING,127,caSurl);
    }



    lTotPg = (lCount -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lCount);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
    //输出策略
    for(i=0;i<iNum;i++){
    	sprintf(caTemp,"select plateid,plateindex,corps,groups,mark,ssid,ttype,datarang,stype,scount from adprocy where adid=%lu ",sids[i]);
    	psCur = pasDbOpenSql(caTemp,0);
      if(psCur) {
      	
   
  
    memset(caPlateid,0,sizeof(caPlateid));
    memset(caPlateindex,0,sizeof(caPlateindex));
    memset(caCorps,0,sizeof(caCorps));
    memset(caGroups,0,sizeof(caGroups));
    memset(caSsid,0,sizeof(caSsid));
    memset(caMark,0,sizeof(caMark));
    memset(caSsid,0,sizeof(caSsid));
    lTtype=0;
    memset(caDatarang,0,sizeof(caDatarang));
    lStype=0;
    lScount=0;
    memset(caTemp,0,sizeof(caTemp));
		while(0==(iret=pasDbFetchInto(psCur,
		  UT_TYPE_STRING,31,caPlateid,
		  UT_TYPE_STRING,31,caPlateindex,
		  UT_TYPE_STRING,1000,caCorps,
		  UT_TYPE_STRING,1000,caGroups,
		  UT_TYPE_STRING,1000,caMark,
		  UT_TYPE_STRING,1000,caSsid,
		  UT_TYPE_LONG,4,&lTtype,
		  UT_TYPE_STRING,sizeof(caDatarang)-1,caDatarang,
		  UT_TYPE_LONG,4,&lStype,
		  UT_TYPE_LONG,4,&lScount)) || 1405 == iret){
      
      	if(strlen(caPlateid)>0){
      		sprintf(caTemp,"投放模板:%s",caPlateid);
      	}
      	if(strlen(caPlateindex)>0){
      		sprintf(caTemp+strlen(caTemp)," 广告位:%s",caPlateindex);
      	}
      
      	
      }
    
      	utPltPutLoopVar(psDbHead,"procy_desc",i+1,caTemp);
      	pasDbCloseCursor(psCur);
      }
    	
    }
        
//     utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ad/ad_resource_list.htm");
    return 0;
}


//保存广告素材基本信息
int ncAdResource_Save(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  utPltDbHead *psDbHead = NULL;
  
 	long iReturn,lSid;
  char caAdurl[256],caPlateindex[64];;
 	char caAdfile[128],caSid[12],caAdname[64],caAdcode[32],caDaterang[128],caApssid[1024],caGroupid[16];
 	unsigned long lTime = time(0);
 	char caPlateid[20],caPid[20],caAdindex[64];
 	char caFtitle[128],caFdesc[128],caFwidth[20],caFheight[20],caDefad[128];
 	char caTemp[1024];
 	char caGroups[1024],caCorps[1024];
 	char caAdPlate[128];
  long lCount;
  char caDel[16];
  char caName[32],caUrl[128];
 	utMsgPrintMsg(psMsgHead);

 psDbHead = utPltInitDbHead();

	utMsgGetSomeNVar(psMsgHead,8,
		                   "sid",   UT_TYPE_STRING, sizeof(caSid)-1, caSid,
		                   "name", UT_TYPE_STRING,sizeof(caName)-1,caName,
		                   "url",UT_TYPE_STRING,sizeof(caUrl)-1,caUrl,
		                   "ftitle",     UT_TYPE_STRING,sizeof(caFtitle)-1,caFtitle,
		                   "fdesc",      UT_TYPE_STRING,sizeof(caFdesc)-1,caFdesc,
		                   "fwidth",     UT_TYPE_STRING,sizeof(caFwidth)-1,caFwidth,
		                   "fheight",    UT_TYPE_STRING,sizeof(caFheight)-1,caFheight,
		                   "del",        UT_TYPE_STRING,8,caDel);
		                   
		                    
		    strcpy(caAdPlate,utComGetVar_sd(psShmHead,"adplate","/home/adplate"));
		    if(caAdPlate[strlen(caAdPlate)-1]!='/'){
		    	strcat(caAdPlate,"/");
		    }
		 		if(strlen(caName)>0){
		  	  pasCvtGBK(2,caName,caTemp,31);
		      strcpy(caName,caTemp);
		  	}
			
				
				if(strlen(caUrl)>0){
		  	  pasCvtGBK(2,caUrl,caTemp,127);
		      strcpy(caUrl,caTemp);
		  	}
	 
		  if(strlen(caFtitle)>0){
		  	  pasCvtGBK(2,caFtitle,caTemp,127);
		      strcpy(caFtitle,caTemp);
		    }
		
			if(strlen(caFdesc)>0){
				 pasCvtGBK(2,caFdesc,caTemp,127);
		     					  	
		  }
			
		
		if(strcmp(caDel,"del")==0){
			sprintf(caTemp,"select fimg from adresource where sid=%s  ",caSid);
			printf("caTemp=%s\n",caTemp);
			memset(caDefad,0,sizeof(caDefad));
			pasDbOneRecord(caTemp,0,UT_TYPE_STRING,63,caDefad);
			if(strstr(caDefad,"adplate")){
			sprintf(caTemp,"/home%s",caDefad);
			printf("caTemp=%s\n",caTemp);
			remove(caTemp);

		  }
		  printf("update adresource set name='',url='',fimg='',ftitle='',fdesc='',fwidth='',fheight='',modtime=%d where sid=%s  \n",time(0),caSid);
			iReturn=pasDbExecSqlF("update adresource set name='',url='',fimg='',ftitle='',fdesc='',fwidth='',fheight='',modtime=%d where sid=%s  ",time(0),caSid);
		  
		}
		else{
		 sprintf(caTemp,"select count(*) from adresource where sid=%s  ",caSid);
		 lCount=0;
		  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
		  
		
		  
		  
      if(lCount>0){
        	 printf("update adresource set name='%s',url='%s',fwidth='%s',fheight='%s',ftitle='%s',fdesc='%s',modtime=%d  where sid=%s  \n",
      	   caName,caUrl,caFwidth,caFheight,caFtitle,caFdesc,time(0),caSid);   		
      	   iReturn=pasDbExecSqlF("update adresource set name='%s',url='%s',fwidth='%s',fheight='%s',ftitle='%s',fdesc='%s',modtime=%d  where sid=%s  ",
      	   caName,caUrl,caFwidth,caFheight,caFtitle,caFdesc,time(0),caSid);
      	   
      
      }
      else{
      	 iReturn=pasDbExecSqlF("insert into adresource(name,url,ftitle,fdesc,fwidth,fheight,addtime,modtime) \
      	         values('%s','%s','%s','%s','%s','%s',%d,%d) ",\
      	         caName,caUrl,caFtitle,caFdesc,caFwidth,caFheight,time(0),time(0));
      	         lSid=0;
      	  pasDbOneRecord("select max(sid) from adresource ",0,UT_TYPE_LONG,4,&lSid);
      	  sprintf(caSid,"%d",lSid);
      }
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

//上传广告素材中的图片文件
int ncAdResource_upload_f(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  utPltDbHead *psDbHead = NULL;
 	char caFile[256] = "";
 	char sid[17] = "";
 	char *p;
 	char caLocal[128] = "";
 	char caRemote[128] = "";
 	char caRemote_new[128] = "";
 	char caType[128] = "";
 	char caDfile[128];
 	long iReturn,lSid;
 	char caSid[20];
 	char caPath[128];
 	char caTemp[256],caPlatepath[256]; 	
 	char caFileName[128];
  DIR *dir,*dir2;
 	struct  stat st,st1;
  struct dirent *d,*d2;
  char caAd[128],caAdPlate[128],caPlatepathtmp[128];
  char caNewfile[128];
  FILE *fp,*fp1;
  unsigned long lSize;
  long index;
  char caPlateid[32],caPlateindex[32];
 	unsigned long lTime = time(0);
	memset(caDfile,0,sizeof(caDfile));
	
utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,2,
		                   "sid",   UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid,
		                   "adfile",UT_TYPE_STRING,sizeof(caFile)-1,caFile);
		                
	
	
		  if(strlen(caFile)>0){
		  	char *descr_gbk = pasConvert("UTF-8", "GBK", caFile);
				 if(descr_gbk){
				 	strcpy(caFile,descr_gbk);
				  free(descr_gbk);
				}
		  	
		  }
        
        spmUtlUploadFile(caFile,caLocal,caRemote,caType);
        printf(" caFile==[%s]\n",caFile);
        printf("caLocal=%s,caRemote=%s\n",caLocal,caRemote);
        
  strcpy(caAdPlate,utComGetVar_sd(psShmHead,"adplate","/home/adplate"));
	    if(caAdPlate[strlen(caAdPlate)-1]=='/'){
	    	caAdPlate[strlen(caAdPlate)-1]=='\0';
		    }
		     
        
        
        
       
        p = strrstr(caRemote, "/");
        if(p != NULL)
        {        
        	strcpy(caDfile,++p);
                
        }
        sprintf(caPlatepath,"%s/%s/images",caAdPlate,caPlateid);    		  
		    if(!utFileIsExist(caPlatepath)) 
        {
        	char caComm[1024];
        	 sprintf(caComm,"mkdir -p %s",caPlatepath);
        	 printf("caComm=%s\n",caComm);
        	 system(caComm);
                
        
        } 

        //将上传的文件重名
        sprintf(caNewfile,"/adplate/%s/images/%d_%s",caPlateid,lTime,caDfile);
        snprintf(caRemote_new, sizeof(caRemote_new), "%s/%d_%s", caPlatepath,lTime,caDfile);
		    printf("caRemote_new=%s\n",caRemote_new);
		    printf("caRemote=%s\n",caRemote);
		    rename(caRemote, caRemote_new); 
     
       iReturn=pasDbExecSqlF("update adresource set fimg='%s' where sid=%s ",
        caNewfile,caPlateid);
        
        
    psDbHead = utPltInitDbHead(); 
  
    if(iReturn==0){
    	utPltPutVar(psDbHead,"titel","true");
    
    	utPltPutVar(psDbHead,"adfile",caNewfile);
    	utPltPutVar(psDbHead,"message","上传成功");
    }
    else{
    	utPltPutVar(psDbHead,"titel","false");
    
    	utPltPutVar(psDbHead,"message","上传失败");
    }

    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v8/nc_adfile.htm");	


}

//上传广告素材中的二级图片文件
int ncAdResource_upload_s(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  utPltDbHead *psDbHead = NULL;
 	char caFile[256] = "";
 	char sid[17] = "";
 	char *p;
 	char caLocal[128] = "";
 	char caRemote[128] = "";
 	char caRemote_new[128] = "";
 	char caType[128] = "";
 	char caDfile[128];
 	long iReturn,lSid;
 	char caSid[20];
 	char caPath[128];
 	char caTemp[256],caPlatepath[256]; 	
 	char caFileName[128];
  DIR *dir,*dir2;
 	struct  stat st,st1;
  struct dirent *d,*d2;
  char caAd[128],caAdPlate[128],caPlatepathtmp[128];
  char caNewfile[128];
  FILE *fp,*fp1;
  unsigned long lSize;
  long index;
  char caPlateid[32],caPlateindex[32];
 	unsigned long lTime = time(0);
	memset(caDfile,0,sizeof(caDfile));
	
utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,2,
		                   "sid",   UT_TYPE_STRING, sizeof(caPlateid)-1, caPlateid,
		                   "adfile",UT_TYPE_STRING,sizeof(caFile)-1,caFile);
		                
	
	
		  if(strlen(caFile)>0){
		  	char *descr_gbk = pasConvert("UTF-8", "GBK", caFile);
				 if(descr_gbk){
				 	strcpy(caFile,descr_gbk);
				  free(descr_gbk);
				}
		  	
		  }
        
        spmUtlUploadFile(caFile,caLocal,caRemote,caType);
        printf(" caFile==[%s]\n",caFile);
        printf("caLocal=%s,caRemote=%s\n",caLocal,caRemote);
        
  strcpy(caAdPlate,utComGetVar_sd(psShmHead,"adplate","/home/adplate"));
	    if(caAdPlate[strlen(caAdPlate)-1]=='/'){
	    	caAdPlate[strlen(caAdPlate)-1]=='\0';
		    }
		     
        
        
        
       
        p = strrstr(caRemote, "/");
        if(p != NULL)
        {        
        	strcpy(caDfile,++p);
                
        }
        sprintf(caPlatepath,"%s/%s/images",caAdPlate,caPlateid);    		  
		    if(!utFileIsExist(caPlatepath)) 
        {
        	char caComm[1024];
        	 sprintf(caComm,"mkdir -p %s",caPlatepath);
        	 printf("caComm=%s\n",caComm);
        	 system(caComm);
                
        
        } 

        //将上传的文件重名
        sprintf(caNewfile,"/adplate/%s/images/%d_%s",caPlateid,lTime,caDfile);
        snprintf(caRemote_new, sizeof(caRemote_new), "%s/%d_%s", caPlatepath,lTime,caDfile);
		    printf("caRemote_new=%s\n",caRemote_new);
		    printf("caRemote=%s\n",caRemote);
		    rename(caRemote, caRemote_new); 
     
       iReturn=pasDbExecSqlF("update adresource set simg='%s' where sid=%s ",
        caNewfile,caPlateid);
        
        
    psDbHead = utPltInitDbHead(); 
  
    if(iReturn==0){
    	utPltPutVar(psDbHead,"titel","true");
    
    	utPltPutVar(psDbHead,"adfile",caNewfile);
    	utPltPutVar(psDbHead,"message","上传成功");
    }
    else{
    	utPltPutVar(psDbHead,"titel","false");
    
    	utPltPutVar(psDbHead,"message","上传失败");
    }

    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v8/nc_adfile.htm");	


}

//保存广告素材二级页面基本信息
int ncAdResource_Sec_Save(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  utPltDbHead *psDbHead = NULL;
  
 	long iReturn,lSid;
  char caAdurl[256],caPlateindex[64];;
 	char caAdfile[128],caSid[12],caAdname[64],caAdcode[32],caDaterang[128],caApssid[1024],caGroupid[16];
 	unsigned long lTime = time(0);
 	char caPlateid[20],caPid[20],caAdindex[64];
 	char caFtitle[128],caFdesc[128],caFwidth[20],caFheight[20],caDefad[128];
 	char caTemp[1024];
 	char caGroups[1024],caCorps[1024];
 	char caAdPlate[128];
  long lCount;
  char caDel[16];
  char caName[32],caUrl[128];
  char caSurl[128];
 	utMsgPrintMsg(psMsgHead);

 psDbHead = utPltInitDbHead();

	utMsgGetSomeNVar(psMsgHead,7,
		                   "sid",   UT_TYPE_STRING, sizeof(caSid)-1, caSid,
		                   "stitle",     UT_TYPE_STRING,sizeof(caFtitle)-1,caFtitle,
		                   "sdesc",      UT_TYPE_STRING,sizeof(caFdesc)-1,caFdesc,
		                   "swidth",     UT_TYPE_STRING,sizeof(caFwidth)-1,caFwidth,
		                   "sheight",    UT_TYPE_STRING,sizeof(caFheight)-1,caFheight,
		                   "surl",       UT_TYPE_STRING,sizeof(caSurl)-1,caSurl,
		                   "del",        UT_TYPE_STRING,8,caDel);
		                   
		                    
		    strcpy(caAdPlate,utComGetVar_sd(psShmHead,"adplate","/home/adplate"));
		    if(caAdPlate[strlen(caAdPlate)-1]!='/'){
		    	strcat(caAdPlate,"/");
		    }

		  if(strlen(caFtitle)>0){
		  	  pasCvtGBK(2,caFtitle,caTemp,127);
		      strcpy(caFtitle,caTemp);
		    }
		
			if(strlen(caFdesc)>0){
				 pasCvtGBK(2,caFdesc,caTemp,127);
		     					  	
		  }
			
		
		if(strcmp(caDel,"del")==0){
			sprintf(caTemp,"select simg from adresource where sid=%s  ",caSid);
			printf("caTemp=%s\n",caTemp);
			memset(caDefad,0,sizeof(caDefad));
			pasDbOneRecord(caTemp,0,UT_TYPE_STRING,63,caDefad);
			if(strstr(caDefad,"adplate")){
			sprintf(caTemp,"/home%s",caDefad);
			printf("caTemp=%s\n",caTemp);
			remove(caTemp);

		  }
		  printf("update adresource set simg='',stitle='',sdesc='',swidth='',sheight='',modtime=%d where sid=%s  \n",time(0),caSid);
			iReturn=pasDbExecSqlF("update adresource set simg='',stitle='',sdesc='',swidth='',sheight='',modtime=%d,surl='' where sid=%s  ",time(0),caSid);
		  
		}
		else{
		 sprintf(caTemp,"select count(*) from adresource where sid=%s  ",caSid);
		 lCount=0;
		  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
		  
		
		  
		  
      if(lCount>0){
        	 printf("update adresource set swidth='%s',sheight='%s',stitle='%s',sdesc='%s',modtime=%d,surl='%s'  where sid=%s  \n",
      	   caFwidth,caFheight,caFtitle,caFdesc,time(0),caSurl,caSid);   		
      	   iReturn=pasDbExecSqlF("update adresource set swidth='%s',sheight='%s',stitle='%s',sdesc='%s',modtime=%d,surl='%s'  where sid=%s  ",
      	   caFwidth,caFheight,caFtitle,caFdesc,time(0),caSurl,caSid);
      	   
      
      }
      else{
      	 iReturn=pasDbExecSqlF("insert into adresource(stitle,sdesc,swidth,sheight,addtime,modtime,surl) \
      	         values('%s','%s','%s','%s',%d,%d,'%s') ",\
      	         caFtitle,caFdesc,caFwidth,caFheight,time(0),time(0),caSurl);
      	         lSid=0;
      	  pasDbOneRecord("select max(sid) from adresource ",0,UT_TYPE_LONG,4,&lSid);
      	  sprintf(caSid,"%d",lSid);
      }
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

//保存广告素材投放策略
int ncAdResourceProcy_Save(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  utPltDbHead *psDbHead = NULL;
  
 	long iReturn,lSid;
  char caPlateindex[64];;
 	char caSid[12],caAdname[64],caAdcode[32],caDatarang[1024],caSsid[1024];
 	unsigned long lTime = time(0);
 	char caPlateid[32],caPid[20];
 	char caTemp[1024];
 	char caGroups[1024],caCorps[1024];
 	char caAdid[16],caTtype[16],caStype[16],caScount[16];
  long lCount;
  char caDel[16];
  char caMark[1024];
 	utMsgPrintMsg(psMsgHead);

 psDbHead = utPltInitDbHead();

	utMsgGetSomeNVar(psMsgHead,13,
		                   "sid",   UT_TYPE_STRING, sizeof(caSid)-1, caSid,
		                   "adid", UT_TYPE_STRING,10,caAdid,
		                   "plateid", UT_TYPE_STRING,sizeof(caPlateid)-1,caPlateid,
		                   "plateindex",UT_TYPE_STRING,sizeof(caPlateindex)-1,caPlateindex,
		                   "ttype",     UT_TYPE_STRING,sizeof(caTtype)-1,caTtype,
		                   "datarang",      UT_TYPE_STRING,sizeof(caDatarang)-1,caDatarang,
		                   "mark",     UT_TYPE_STRING,sizeof(caMark)-1,caMark,
		                   "ssid",    UT_TYPE_STRING,sizeof(caSsid)-1,caSsid,
		                   "groups",  UT_TYPE_STRING,sizeof(caGroups)-1,caGroups,
		                   "stype",   UT_TYPE_STRING,sizeof(caStype)-1,caStype,
		                   "scount",  UT_TYPE_STRING,sizeof(caScount)-1,caScount,
		                   "corps",   UT_TYPE_STRING,sizeof(caCorps)-1,caCorps,
		                   "del",        UT_TYPE_STRING,8,caDel);
		                   
		                    
		  
		 		if(strlen(caPlateid)>0){
		  	  pasCvtGBK(2,caPlateid,caTemp,31);
		      strcpy(caPlateid,caTemp);
		  	}
			
				if(strlen(caPlateindex)>0){
		  	  pasCvtGBK(2,caPlateindex,caTemp,31);
		      strcpy(caPlateindex,caTemp);
		  	}
				
				
				if(strlen(caDatarang)>0){
		  	  pasCvtGBK(2,caDatarang,caTemp,100);
		      strcpy(caDatarang,caTemp);
		  	}
	      if(strstr(caDatarang,"每天")){
	      	strcpy(caDatarang,"");
	      }
	 
		  if(strlen(caMark)>0){
		  	  pasCvtGBK(2,caMark,caTemp,1000);
		      strcpy(caMark,caTemp);
		   }
		   if(strstr(caMark,"标签")){
		   	strcpy(caMark,"");
		  }
		  
			if(strlen(caSsid)>0){
				  pasCvtGBK(2,caSsid,caTemp,1000);
		     	strcpy(caSsid,caTemp);				  	
		  }
			if(strstr(caSsid,"多个")){
				strcpy(caSsid,"");
				
			}
			if(strlen(caSid)==0){
				iReturn=pasDbExecSqlF("insert into adprocy(adid,plateid,plateindex,corps,groups,mark,ssid,ttype,datarang,stype,scount,addtime,modtime) \
				  values(%s,'%s','%s','%s','%s','%s','%s',%d,'%s',%d,%d,%d,%d) ",
				   caAdid,caPlateid,caPlateindex,caCorps,caGroups,caMark,caSsid,atol(caTtype),caDatarang,atol(caStype),atol(caScount),time(0),time(0));
				if(iReturn==0){
					pasDbOneRecord("slect max(sid) from adprocy",0,UT_TYPE_LONG,4,&lSid);
					sprintf(caSid,"%s",lSid);
				}
			}
			else{
				iReturn=pasDbExecSqlF("update adprocy set plateid='%s',plateindex='%s',corps='%s',groups='%s',mark='%s',ssid='%s',ttype=%d,datarang='%s',stype=%d,scount=%d,modtime=%d where sid=%s ",
				caPlateid,caPlateindex,caCorps,caGroups,caMark,caSsid,atol(caTtype),caDatarang,atol(caStype),atol(caScount),time(0),caSid);
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

//广告内容投放策略管理
int ncAdprocylist_v8(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead){
#if SYSTEM_LOG
	utMsgPrintMsg(psMsgHead);
#endif
  char caPid[16];
  char caTemp[1024];
  char caKeyword[64];
  char caStart[32],caLimit[32],caSort[32],caDir[32];
  unsigned long lSid,lGroupid,lAddtime,lModtime,lCount;
  char caDel[16],caSelsid[1024];
  char caAdcode[32],caAdname[64],caAdurl[128],caAdfile[128],caAdfile_new[128],caDaterang[256],caMark[1024],caApssid[256];
  char caOut[200][256];
  char caPlatename[128],caAdindex[64];
  char caAp[128],caAc[128],caTermfac[64],caMobsp[32],caMobarea[32],caOs[64],caBrower[128],caCorpname[128];
  long lWay,lPri,lCid,lDid,cCount;
  long lPid,lPlateid,lAdid,lTtype,lStype,lScount;
  char caDataranag[1024];
  long lCount_g,m;
  int iNum = 0;
  int iret = -1;
	char caAdid[16];
	char caPlateid[32];
	char caPlateindex[32],caCorps[1024],caGroups[1024],caSsid[1024],caDatarang[1024];
	char caTemp_g[2000],caTemp_c[2000];
	utMsgGetSomeNVar(psMsgHead,3,
		"adid",	UT_TYPE_STRING,  10, 		caAdid,
		"del",      UT_TYPE_STRING,10,caDel,
		"selsid",   UT_TYPE_STRING,1000,caSelsid);
utMsgPrintMsg(psMsgHead);
	
	char sql[1024] = "";
	char sql_tmp[1024] = "";
	pasDbCursor *psCur = NULL;
	utPltDbHead *psDbHead = utPltInitDb();

	
		  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
                 
          		iret = pasDbExecSqlF("delete from adprocy where sid in(%s) ",caSelsid);
          	
          		         
	  }
	
	
	
	
	
	
	
	snprintf(sql,sizeof(sql),"select count(*) from adprocy where 1=1  ");
	if(strlen(caAdid)>0){
		sprintf(sql+strlen(sql)," and adid=%s ",caAdid);
	}
	
	pasDbOneRecord(sql,0,UT_TYPE_LONG,sizeof(long),&lCount);	
	
	utPltPutVarF(psDbHead,"TotRec","%u",lCount);
	
	sprintf(sql,"select sid,adid,plateid,plateindex,corps,groups,mark,ssid,ttype,datarang,stype,scount from adprocy where 1=1 ");
	
	if(strlen(caAdid)>0){
	  sprintf(sql+strlen(sql)," and adid=%s ",caAdid);
  }
  
	sprintf(sql+strlen(sql)," order by sid desc ");
	
printf("sql=%s\n",sql);	

	psCur = pasDbOpenSql(sql,0);
	
	if(NULL != psCur){
	
    lSid=0;
    lAdid=0;
    memset(caPlateid,0,sizeof(caPlateid));
    memset(caPlateindex,0,sizeof(caPlateindex));
    memset(caCorps,0,sizeof(caCorps));
    memset(caGroups,0,sizeof(caGroups));
    memset(caSsid,0,sizeof(caSsid));
    lTtype=0;
    memset(caDatarang,0,sizeof(caDatarang));
    lStype=0;
    lScount=0;
		while(0==(iret=pasDbFetchInto(psCur,
		  UT_TYPE_LONG,4,&lSid,
		  UT_TYPE_LONG,4,&lAdid,
		  UT_TYPE_STRING,31,caPlateid,
		  UT_TYPE_STRING,31,caPlateindex,
		  UT_TYPE_STRING,sizeof(caCorps)-1,caCorps,
		  UT_TYPE_STRING,sizeof(caGroups)-1,caGroups,
		  UT_TYPE_STRING,sizeof(caMark)-1,caMark,
		  UT_TYPE_STRING,sizeof(caSsid)-1,caSsid,
		  UT_TYPE_LONG,4,&lTtype,
		  UT_TYPE_STRING,sizeof(caDataranag)-1,caDatarang,
		  UT_TYPE_LONG,4,&lStype,
		  UT_TYPE_LONG,4,&lScount)) || 1405 == iret){
			
			
			iNum++;
			if(iNum>1){
				utPltPutLoopVar(psDbHead,"dh",iNum,",");
			}
			 utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
			 utPltPutLoopVarF(psDbHead,"adid",iNum,"%lu",lAdid);
			 utPltPutLoopVar(psDbHead,"plateid",iNum,caPlateid);
			 utPltPutLoopVar(psDbHead,"plateindex",iNum,caPlateindex);
			 utPltPutLoopVar(psDbHead,"mark",iNum,caMark);
			 utPltPutLoopVar(psDbHead,"ssid",iNum,caSsid);
			 utPltPutLoopVar(psDbHead,"corps",iNum,caCorps);
			 utPltPutLoopVar(psDbHead,"groups",iNum,caGroups);
			 utPltPutLoopVarF(psDbHead,"ttype",iNum,"%lu",lTtype);
			 utPltPutLoopVar(psDbHead,"datarang",iNum,caDatarang);
			 utPltPutLoopVarF(psDbHead,"stype",iNum,"%lu",lStype);
			 utPltPutLoopVarF(psDbHead,"scount",iNum,"%lu",lScount);
			
			 

			 memset(caTemp,0,sizeof(caTemp));
			if(strlen(caSsid)>0){
				sprintf(caTemp+strlen(caTemp),"ssid:[%s]",caSsid);
			}

			if(strlen(caDatarang)>0){
				if(lTtype==1){
					sprintf(caTemp+strlen(caTemp),"每天:%s",caDatarang);
				}
				else if(lTtype==2){
					sprintf(caTemp+strlen(caTemp),"每周:%s",caDatarang);
				}
				else if(lTtype==3){
					sprintf(caTemp+strlen(caTemp),"每月:%s",caDatarang);
				}
			}
				
			  if(strlen(caMark)>0){
			  	sprintf(caTemp+strlen(caTemp)," 标记:[%s]",caMark);
			  }
			
			
			  if(lScount>0){
			  	if(lStype==1){
			  		sprintf(caTemp+strlen(caTemp)," 每日投放次数:[%d]",lScount);
			  	}
			  	else if(lStype==2){
			  		sprintf(caTemp+strlen(caTemp)," 每周投放次数:[%d]",lScount);
			  	}
					else if(lStype==3){
			  		sprintf(caTemp+strlen(caTemp)," 每月投放次数:[%d]",lScount);
			  	}
			  }
			  
			lCount_g=sepcharbydh(caGroups,caOut);
		  memset(caTemp_g,0,sizeof(caTemp_g));
		   if(strstr(caGroups,"all")){
		   	strcpy(caTemp_g,"全部");
		   }
		   else{
		   	 for(m=0;m<lCount_g;m++){
			   	if(m==0){
			   		strcpy(caTemp_g,authGetGroupNameById(psShmHead,atol(caOut[m])));
			    }
			    else{
			   	  sprintf(caTemp_g+strlen(caTemp_g),",%s",authGetGroupNameById(psShmHead,atol(caOut[m])));
			    }
			   }
			 }
			 utPltPutLoopVar(psDbHead,"groupsname",iNum,caTemp_g);
		   if(strlen(caTemp_g)>0){
		   	sprintf(caTemp+strlen(caTemp),"单位组:[%s]",caTemp_g);
		   }	
		   	
		   	
		  lCount_g=sepcharbydh(caCorps,caOut);
		   	memset(caTemp_c,0,sizeof(caTemp_c));
//	  	printf("corps=%s,count=%d\n",caCorps,lCount_g);
		   if(strstr(caCorps,"all")){
		   	strcpy(caTemp_g,"全部");
		  }
		  else{

		   for(m=0;m<lCount_g;m++){
		   	if(m==0){
		   		strcpy(caTemp_c,authGetClientName(psShmHead,atol(caOut[m])));
		   }
		   else{
		   	sprintf(caTemp_c+strlen(caTemp_c),",%s",authGetClientName(psShmHead,atol(caOut[m])));
		    }
		   }
		  }
		   utPltPutLoopVar(psDbHead,"corpsname",iNum,caTemp_c);
		  
			 if(strlen(caTemp_c)>0){
		   	sprintf(caTemp+strlen(caTemp),"单位:[%s]",caTemp_c);
		   }
			  
			  
			  
			  
			  
			 utPltPutLoopVar(psDbHead,"procy_desc",iNum,caTemp);
			
		}
		pasDbCloseCursor(psCur);
		

		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ad/nc_adprocy_list.htm");	
	}
	
	return 0;
}




int proauthInitWeb003(utShmHead *psShmHead)
{
	pasSetTcpFunName("ncAdplatelist_v8",ncAdplatelist_v8, 0);
	pasSetTcpFunName("ncAdPlate_Save_v8",ncAdPlate_Save_v8, 0);
	pasSetTcpFunName("ncAdindexlist_v8",ncAdindexlist_v8, 0);
	pasSetTcpFunName("proauthShowPlateBm",proauthShowPlateBm, 0);
	pasSetTcpFunName("proauthShowPlateBmSave",proauthShowPlateBmSave, 0);
	
	pasSetTcpFunName("ncAdPlate_dir",ncAdPlate_dir, 0);
	pasSetTcpFunName("ncAdPlateTerm_Save_v8",ncAdPlateTerm_Save_v8, 0);
	pasSetTcpFunName("ncShowAdIndex_v8",ncShowAdIndex_v8, 0);
	pasSetTcpFunName("ncAdIndex_upload_v8",ncAdIndex_upload_v8, 0);
	pasSetTcpFunName("proauth_portplate",proauth_portplate, 0);
	pasSetTcpFunName("proauthAdindexlist",proauthAdindexlist, 0);
	pasSetTcpFunName("ncAdPlateIndex_Save",ncAdPlateIndex_Save, 0);
	
	pasSetTcpFunName("ncAdDefAd_upload_v8",ncAdDefAd_upload_v8, 0);
	pasSetTcpFunName("ncAdPlateIndex_Sec_Save",ncAdPlateIndex_Sec_Save, 0);

  pasSetTcpFunName("ncAdDefAd_upload_sec",ncAdDefAd_upload_sec, 0);
  pasSetTcpFunName("ncAdResource_upload_f",ncAdResource_upload_f, 0);
  pasSetTcpFunName("ncAdResource_upload_s",ncAdResource_upload_s, 0);
  pasSetTcpFunName("adResourcelist",adResourcelist, 0);
  pasSetTcpFunName("ncAdResource_Save",ncAdResource_Save, 0);
  pasSetTcpFunName("ncAdResource_Sec_Save",ncAdResource_Sec_Save, 0);
  
  pasSetTcpFunName("ncAdResourceProcy_Save",ncAdResourceProcy_Save, 0);
  pasSetTcpFunName("ncAdprocylist_v8",ncAdprocylist_v8, 0);
}