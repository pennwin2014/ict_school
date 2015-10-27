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
#define   NC_LNK_IPPKG      31

#include "ncmweb.h"

#include <net/if.h>
#include <sys/ioctl.h>	

#include "ncportalweb.h"


//监控接口设置
int ncmWebSystemSet_dev(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iNum,iReturn;
    utPltDbHead *psDbHead;
    char *pBuf,*pSelect;
    char *p;
    char caVar[32];
    int i=0;
   	char caGid[16],caDid[16],caAdd[16],caDel[16];
		char caName[32],caNid[16],caModi[16],caApp[16];
		char caDevname[64],caCaptype[10],caFlags[2];
		long sid=0;
		unsigned long lCaptype,lFlags;
		char caSid[16];
		char caTemp[1024];
		char caTname[32];
		long promisc=0,lTotRec=0,tmpcode=0,lSid=0;
	 	pasDbCursor *psCur;
    long lGroupid0=0;
    char captype_desc[32]="",flags_desc[8]="",caPromisc[16]="";
    char caTemp_d[256],caSesid[1024],caMsg[1024],caUsername2[32]="";
    utMsgPrintMsg(psMsgHead);
    dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
	  iReturn = utMsgGetSomeNVar(psMsgHead,13,
	                "i_name",        UT_TYPE_STRING,30,caName,
	                "i_devname",     UT_TYPE_STRING,60,caDevname,
	                "i_captype",     UT_TYPE_STRING,8,caCaptype,
	                "i_flags",       UT_TYPE_STRING,1,caFlags,
	                "i_captype_desc",UT_TYPE_STRING,32,captype_desc,    //抓包
	                "i_flags_desc",  UT_TYPE_STRING,8 ,flags_desc,      //状态 启用
	                "i_promisc",     UT_TYPE_STRING,16,caPromisc,         //是否混杂模式
	                "sid",           UT_TYPE_STRING,12,caSid,
	                "add",           UT_TYPE_STRING,12,caAdd,
	                "del",           UT_TYPE_STRING,12,caDel,
	                "selsid",        UT_TYPE_STRING,1000,caSesid,
	                "update",        UT_TYPE_STRING,12,caModi,
	                "app",           UT_TYPE_STRING,11,caApp);
	  
	  //未操作选择框
	  if(strlen(captype_desc)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,captype_desc,caTemp,32));
    	strcpy(captype_desc,caTemp_d);
    }
    if(strlen(flags_desc)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,flags_desc,caTemp,8));
    	strcpy(flags_desc,caTemp_d);
    }
    if(strlen(caPromisc)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caPromisc,caTemp,16));
    	strcpy(caPromisc,caTemp_d);
    }
	  
	  printf("00 captype_desc=%s  caPromisc=%s  flags_desc=%s \n",captype_desc,caPromisc,flags_desc);
	  
	  if(strlen(captype_desc) > 2)
	  {
		  	sprintf(caTemp,"select code from ncwethetype where flag=1 and name='%s'  ",captype_desc);
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&tmpcode);  
	  		memset(caCaptype,0,sizeof(caCaptype));
	  		sprintf(caCaptype,"%d",tmpcode); 
	  }
	  else{
	  		strcpy(caCaptype,captype_desc);	
	  }              
    
    if(strlen(caPromisc) > 2)
	  {
	  	tmpcode=0;
	  	sprintf(caTemp,"select code from ncwethetype where flag=2 and name='%s'  ",caPromisc);
  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&tmpcode);  
  		memset(caPromisc,0,sizeof(caPromisc));
  		sprintf(caPromisc,"%d",tmpcode); 
	  }
	  
	  if(strlen(flags_desc) > 2)
	  {
	  	tmpcode=0;
	  	sprintf(caTemp,"select code from ncwethetype where flag=3 and name='%s'  ",flags_desc);
  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&tmpcode);  
  		memset(caFlags,0,sizeof(caFlags));
  		sprintf(caFlags,"%d",tmpcode); 
  		
	  }
	  else{
	  	strcpy(caFlags,flags_desc);	
	  }
    
    printf("caCaptype=%s  caPromisc=%s  caFlags=%s \n",caCaptype,caPromisc,caFlags);
    
    if(strcmp(caModi,"update")==0){  //增加一个组 
       if(atol(caSid) == 0)
       {  		
//         		unsigned long lSid=pasGetSid("ncnetdev","sid");
         		sprintf(caTemp,"select max(sid)+1 from ncnetdev   ");
  					pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);  
         		printf("lSid=%d \n",lSid);
         		if(lSid == 0)
         		{
         			lSid=1;	
         		}
            iReturn = pasDbExecSqlF("insert into ncnetdev(sid,name,captype,devname,flags,promisc) values(%lu,'%s',%s,'%s',%s,%s)",lSid,caName,caCaptype,caDevname,caFlags,caPromisc);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","监控口",ncLang("0310记录增加出错"));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                pasDbCommit(NULL);
            }      
       }else {   //更新
          iReturn = pasDbExecSqlF("update ncnetdev set name='%s',captype=%s,devname='%s',flags=%s,promisc=%s where sid = %s ",
                         caName,caCaptype,caDevname,caFlags,caPromisc,caSid);
	        if(iReturn != 0) {
	            utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0086Web地址列表"),ncLang("0700修改出错%s"),pasDbErrorMsg(NULL));
	            return 0;
	        }                                     

        	pasDbCommit(NULL);
    	 }              
       
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除一个组  */
	    if(strlen(caSesid)>0){
	 	        iReturn = pasDbExecSqlF("delete from ncnetdev where sid in(%s)",caSesid);
		        if(iReturn != 0) {
		            pasDbRollback(NULL);
		        }  
		        else{
//		        	pasDbExecSqlF("delete from ncnetmonip where did=%s ",p);
		        }  
		        pasDbCommit(NULL);
		        sprintf(caMsg,"delete from ncnetdev where sid in(%s) ",caSesid);
            if(iReturn==0){
            		ncmWriteSysLog(psMsgHead,caUsername2,"05","成功",caMsg);
            }
            else{
            		ncmWriteSysLog(psMsgHead,caUsername2,"05","失败",caMsg);
            }
		  }
    }
    else if(!utStrIsSpaces(caApp)) { /* 应用  */
// By Liyunming        ncPkgCapApp(psShmHead);
//        ncUtlLoadGroupIpLst(psShmHead);
    }
         
      psDbHead = utPltInitDb();
      
   	  sprintf(caTemp,"select count(*) from ncnetdev  ");
  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lTotRec);   
      psCur=pasDbOpenSql("select sid,name,captype,devname,flags,promisc from ncnetdev order by sid desc ",0);
      if(psCur==NULL){
      	    utPltFreeDb(psDbHead);
            utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0151部门ip地址段"),ncLang("0174出错 %s"),pasDbErrorMsg(NULL));
            return 0;
      }
      
      memset(caName,0,sizeof(caName));
      memset(caDevname,0,sizeof(caDevname));
      lCaptype=0;
      lFlags=0;
       
      iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&sid,
                                   UT_TYPE_STRING,30,caName,
                                   UT_TYPE_LONG,4,&lCaptype,
                                   UT_TYPE_STRING,60,caDevname,
                                   UT_TYPE_LONG,4,&lFlags,
                                   UT_TYPE_LONG,4,&promisc);
      				  
      iNum=0;
      while(iReturn==0){
      	if(iNum>0){
        	utPltPutLoopVar(psDbHead,"in_dh",iNum+1,",");
        }
      	iNum++;
	      utPltPutLoopVarF(psDbHead,"in_iNum",iNum,"%lu",iNum);
      	utPltPutLoopVarF(psDbHead,"in_sid",iNum,"%lu",sid);
      	utPltPutLoopVar(psDbHead,"in_name",iNum,caName);
      	utPltPutLoopVar(psDbHead,"in_devname",iNum,caDevname);
      	utPltPutLoopVarF(psDbHead,"in_captype",iNum,"%lu",lCaptype);
      	utPltPutLoopVarF(psDbHead,"in_flags",iNum,"%lu",lFlags);
      	utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",iNum);
      	utPltPutLoopVar(psDbHead,"groupname",iNum,"组");
      	if(lCaptype==0)
      	 		utPltPutLoopVar(psDbHead,"in_captype_desc",iNum,ncLang("1200抓包"));
      	 else if(lCaptype==1)
      	 		utPltPutLoopVar(psDbHead,"in_captype_desc",iNum,ncLang("1201文件"));
      	 else if(lCaptype==2)
      	    utPltPutLoopVar(psDbHead,"in_captype_desc",iNum,ncLang("1202SMTP连接监控"));
      	 else if(lCaptype==3)
      	    utPltPutLoopVar(psDbHead,"in_captype_desc",iNum,ncLang("1203零拷贝方式"));
      	    
      	if(lFlags==0)
      	    utPltPutLoopVar(psDbHead,"in_flags_desc",iNum,ncLang("0437启用"));
      	else if(lFlags==1)
      		 utPltPutLoopVar(psDbHead,"in_flags_desc",iNum,ncLang("0568停用"));
      		 
      	if(promisc==0)
      	    utPltPutLoopVar(psDbHead,"in_promisc",iNum,"非混杂模式");
      	else if(promisc==1)
      		  utPltPutLoopVar(psDbHead,"in_promisc",iNum,"混杂模式");	 
      		 
      		 
      memset(caName,0,sizeof(caName));
      memset(caDevname,0,sizeof(caDevname));
      lCaptype=0;
      lFlags=0;
      promisc=0; 
      iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&sid,
                                   UT_TYPE_STRING,30,caName,
                                   UT_TYPE_LONG,4,&lCaptype,
                                   UT_TYPE_STRING,60,caDevname,
                                   UT_TYPE_LONG,4,&lFlags,
                                   UT_TYPE_LONG,4,&promisc);
  				}
       
       pasDbCloseCursor(psCur);
       utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);

    
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysset/ncm_ext_interfaceset.htm");
      
    return 0;
}
//显示接入厂商
int ncmWebEtheSelect(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[32],caGroupid[16];
   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
   	long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
    char caCode[16],caName[68],flag[4]="";
    char caTemp[1024];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead,1,"flag", UT_TYPE_STRING, 4, flag);
    if(strlen(flag) > 0 && atoi(flag) == 1){	
    	sprintf(caTemp,"select code,name from ncwethetype where flag=1 limit 0,29 "); 
  	}else if(strlen(flag) > 0 && atoi(flag) == 2){	
  		sprintf(caTemp,"select code,name from ncwethetype where flag=2 limit 0,29 "); 
  	}else if(strlen(flag) > 0 && atoi(flag) == 3){	
  		sprintf(caTemp,"select code,name from ncwethetype where flag=3 and code <220 limit 0,29 "); 
  	}else{}
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur){
    	
    	psDbHead = utPltInitDb();
    	
	    memset(caCode,0,sizeof(caCode));
	    memset(caName,0,sizeof(caName));
	  	iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
	      	                           UT_TYPE_STRING,63,caName);
	    iNum=0;
	    while((iReturn==0)||(iReturn==1405))
	    {
	    		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
	        iNum++;
	        utPltPutLoopVar(psDbHead,"groupname", iNum,caName);
	        utPltPutLoopVar(psDbHead,"groupid",   iNum,caCode);
			    memset(caCode,0,sizeof(caCode));
			    memset(caName,0,sizeof(caName));
			  	iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
			      	                           UT_TYPE_STRING,63,caName);                              
	    }    
	    pasDbCloseCursor(psCur); 
   
  	}
//  	utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysset/ncm_ext_ethe_select.html");
    return 0;
}

int ncmSysStatus_Alarmset(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{
 
    char *pConfig;
    char *pFile;
    pasConfList *psConfig;
    utPltDbHead *psDbHead;
    int iReturn;
    psDbHead = utPltInitDb();
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0880自动备份设定"),ncLang("0427配置文件出错"));
        return 0;
    }

 		utPltPutVar(psDbHead,"StartChkDsclt",   pasUtlLookConfig(psConfig,"StartChkDsclt","No"));

    utPltPutVar(psDbHead,"h_disk_per",   pasUtlLookConfig(psConfig,"diskrate1","98"));
    utPltPutVar(psDbHead,"m_disk_per",   pasUtlLookConfig(psConfig,"diskrate2","95"));
    utPltPutVar(psDbHead,"l_disk_per",   pasUtlLookConfig(psConfig,"diskrate0","90"));
    
    utPltPutVar(psDbHead,"h_cpu_per",   pasUtlLookConfig(psConfig,"cpurate1","90"));
    utPltPutVar(psDbHead,"m_cpu_per",   pasUtlLookConfig(psConfig,"cpurate2","80"));
    utPltPutVar(psDbHead,"l_cpu_per",   pasUtlLookConfig(psConfig,"cpurate0","60"));
    
    utPltPutVar(psDbHead,"h_mem_per",   pasUtlLookConfig(psConfig,"memrate1","98"));
    utPltPutVar(psDbHead,"m_mem_per",   pasUtlLookConfig(psConfig,"memrate2","95"));
    utPltPutVar(psDbHead,"l_mem_per",   pasUtlLookConfig(psConfig,"memrate0","90"));
//   if(strcasecmp(pasUtlLookConfig(psConfig,"smswarnflag","No"),"Yes")==0) {
//        utPltPutVar(psDbHead,"smswarnflag","checked");
//    } 
//   utPltPutVar(psDbHead,"warnno",   pasUtlLookConfig(psConfig,"warnno",""));
//    utPltPutVar(psDbHead,"DiskWarnTime", pasUtlLookConfig(psConfig,"DiskWarnTime","3600"));
    pasUtlFreeConfig(psConfig);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysset/ncm_sysstatus_alarmset.htm");
    return 0;
}
//保存系统设置参数
int ncmDiskRateSave_par(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caApp[20];
    char caMsg[256];
    int iReturn;
    char *pConfig;
    char *p,*pFile,*pNat,*pRoute;
    FILE *fp;
    pasConfList *psConfig;
    char caUsername2[32];
 	  dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","系统状态告警",ncLang("0427配置文件出错"));
        return 0;
    }
    
    if(p = utMsgGetVar_s(psMsgHead,"h_disk_per")) {
        pasUtlModiConfig(psConfig,"diskrate1",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"m_disk_per")) {
        pasUtlModiConfig(psConfig,"diskrate2",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"l_disk_per")) {
        pasUtlModiConfig(psConfig,"diskrate0",p);
    }
    
    if(p = utMsgGetVar_s(psMsgHead,"h_cpu_per")) {
        pasUtlModiConfig(psConfig,"cpurate1",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"m_cpu_per")) {
        pasUtlModiConfig(psConfig,"cpurate2",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"l_cpu_per")) {
        pasUtlModiConfig(psConfig,"cpurate0",p);
    }
    
    if(p = utMsgGetVar_s(psMsgHead,"h_mem_per")) {
        pasUtlModiConfig(psConfig,"memrate1",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"m_mem_per")) {
        pasUtlModiConfig(psConfig,"memrate2",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"l_mem_per")) {
        pasUtlModiConfig(psConfig,"memrate0",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"StartChkDsclt")) {
        pasUtlModiConfig(psConfig,"StartChkDsclt",p);
    }
    
    
    
//    if(p = utMsgGetVar_s(psMsgHead,"DiskWarnTime")) {
//        pasUtlModiConfig(psConfig,"DiskWarnTime",p);
 //   }
 //      if(p = utMsgGetVar_s(psMsgHead,"warnno")) {
//        pasUtlModiConfig(psConfig,"warnno",p);
//    }

//    if(p = utMsgGetVar_s(psMsgHead,"smswarnflag")) pasUtlModiConfig(psConfig,"smswarnflag","Yes");
//                                 else pasUtlModiConfig(psConfig,"smswarnflag","No");

    pasUtlSaveConfig(pConfig,psConfig);
    pasUtlFreeConfig(psConfig);
    p = utMsgGetVar_s(psMsgHead,"app");
    if(p) {
        if(!utStrIsSpaces(p)) {
            ncUtlReReadConFig(psShmHead);

        }
    }
    ncmWriteSysLog(psMsgHead,caUsername2,"03","success","修改系统状态阀值告警");   
    ncmSysStatus_Alarmset(psShmHead,iFd,psMsgHead);

    return 0;
}
/*
// 网络及主机信息 
int ncmWebSystemSet_base(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{
    char *pFile,*pBuf;
    FILE *fp;
    unsigned long lSize;
    char *pConfig;
    pasConfList *psConfig;
    utPltDbHead *psDbHead;
    int iReturn;
    char *p;
    char *pSelect;
    char caRoute[32],caMask[32],caGate[32];
    long lRoute;
    int i;
    long ethcount;
    char temp_eth[12];
    char temp_ip[12];
    char temp_mask[12];
    long lGroupid0=0;
       //增加网络权限判断
    char caEtcount[12];
    iReturn = utMsgGetSomeNVar(psMsgHead,1,"etcount",    UT_TYPE_STRING, 4, caEtcount);
      
    dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
    if(lGroupid0>0)
    {
    	utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0510设置网络"),ncLang("0925您所属的管理组无该功能的权限"));
    	return 0;
    }
    
    psDbHead = utPltInitDb();
    
    ethcount=0;
    ethcount=atol(utComGetVar_sd(psShmHead,"ethcount","0"));
    
    if((atol(caEtcount)>0)&&(ethcount==0)){
    	ethcount=3;
    }
    pFile = utComGetVar_sd(psShmHead,"RoutConf","ncroute.cnf");
    if(pFile == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0606网络配置"),ncLang("0165参数 NoMonWebList不存在"));
        return 0;
    }

    lSize = utFileSize(pFile);
   
    fp = fopen(pFile,"r");
    if(fp) {
        pBuf = malloc(lSize+1);
        memset(pBuf,0,lSize+1);
        fread(pBuf,lSize,1,fp);
        fclose(fp);
        utPltPutVar(psDbHead,"routconf",pBuf);
        free(pBuf);
    }
    else {
        printf(" File %s does not exist\n",pFile);
    }
    
    //地址转换
    pFile = utComGetVar_sd(psShmHead,"IptableConf","nciptable.cnf");
    if(pFile == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0606网络配置"),ncLang("0165参数 NoMonWebList不存在"));
        return 0;
    }

    lSize = utFileSize(pFile);
   
    fp = fopen(pFile,"r");
    if(fp) {
        pBuf = malloc(lSize+1);
        memset(pBuf,0,lSize+1);
        fread(pBuf,lSize,1,fp);
        fclose(fp);
        utPltPutVar(psDbHead,"iptable",pBuf);
        free(pBuf);
    }
    else {
        printf(" File %s does not exist\n",pFile);
    }
    
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0606网络配置"),ncLang("0427配置文件出错"));
        return 0;
    }
    if( strcasecmp(pasUtlLookConfig(psConfig,"AccessWay","Bridge"),"Bridge") == 0) {
        utPltPutVar(psDbHead,"bridge","checked");
    }
    else if( strcasecmp(pasUtlLookConfig(psConfig,"AccessWay","bypass"),"bypass") == 0){
    	utPltPutVar(psDbHead,"bypass","checked");
    }
    else {
        utPltPutVar(psDbHead,"gateway","checked");
    } 
    
    if(ethcount>0){
	    for(i=0;i<ethcount;i++){  
	    	sprintf(temp_eth,"eth%d",i);  
	    	
	    if(strstr(pasUtlLookConfig(psConfig,"BrDev","eth0,eth1"),ncUtlEthValue(psShmHead,temp_eth)))
	    utPltPutLoopVar(psDbHead,"breth",i+1,"checked");
	    
	    utPltPutLoopVarF(psDbHead,"iNum",i+1,"%d",i);
	    
	   }
    }
    else{
    
    	
	    if(strstr(pasUtlLookConfig(psConfig,"BrDev","eth0,eth1"),ncUtlEthValue(psShmHead,"eth0")))
	    utPltPutVar(psDbHead,"breth0","checked");
	    
	    if(strstr(pasUtlLookConfig(psConfig,"BrDev","eth0,eth1"),ncUtlEthValue(psShmHead,"eth1")))
	    utPltPutVar(psDbHead,"breth1","checked");
	    
	    if(strstr(pasUtlLookConfig(psConfig,"BrDev","eth0,eth1"),ncUtlEthValue(psShmHead,"eth2")))
	    utPltPutVar(psDbHead,"breth2","checked");
    }
    
    if(strstr(pasUtlLookConfig(psConfig,"InDev1","eth1"),ncUtlEthValue(psShmHead,"eth0")))
    utPltPutVar(psDbHead,"InDeveth0","checked");
    
    if(strstr(pasUtlLookConfig(psConfig,"InDev1","eth1"),ncUtlEthValue(psShmHead,"eth1")))
    utPltPutVar(psDbHead,"InDeveth1","checked");
    
    if(strstr(pasUtlLookConfig(psConfig,"InDev1","eth1"),ncUtlEthValue(psShmHead,"eth2")))
    utPltPutVar(psDbHead,"InDeveth2","checked");
    
    if(strcasecmp(pasUtlLookConfig(psConfig,"Eth2Lock","No"),"Yes") == 0) {
    	utPltPutVar(psDbHead,"eth2lock","disabled");
     }
     
    if(ethcount>0){
    	for(i=0;i<ethcount;i++){
    		sprintf(temp_eth,"192.168.%d.1",i);
    		sprintf(temp_ip,"eth%dIP",i);
    		sprintf(temp_mask,"eth%dMask",i);
//     		printf("temp_eth=%s\n",temp_eth); 
    		
    		utPltPutLoopVar(psDbHead,"ethIP",i+1,pasUtlLookConfig(psConfig,temp_ip,temp_eth));
    		utPltPutLoopVar(psDbHead,"ethMask",i+1,pasUtlLookConfig(psConfig,temp_mask,"255.255.255.0"));
    	}
    }
   else{
    utPltPutVar(psDbHead,"eth0IP", pasUtlLookConfig(psConfig,"eth0IP","192.168.0.1"));
    utPltPutVar(psDbHead,"eth0Mask", pasUtlLookConfig(psConfig,"eth0Mask","255.255.255.0"));
    utPltPutVar(psDbHead,"eth1IP", pasUtlLookConfig(psConfig,"eth1IP","192.168.1.1"));
    utPltPutVar(psDbHead,"eth1Mask", pasUtlLookConfig(psConfig,"eth1Mask","255.255.255.0"));
    utPltPutVar(psDbHead,"eth2IP", pasUtlLookConfig(psConfig,"eth2IP","192.168.2.1"));
    utPltPutVar(psDbHead,"eth2Mask", pasUtlLookConfig(psConfig,"eth2Mask","255.255.255.0"));
   }
    utPltPutVar(psDbHead,"adminip", pasUtlLookConfig(psConfig,"AdminHost","192.168.0.1"));
    utPltPutVar(psDbHead,"adminmask", pasUtlLookConfig(psConfig,"AdminMask","255.255.255.0"));
    utPltPutVar(psDbHead,"adminport", pasUtlLookConfig(psConfig,"AdminPort","80"));
    utPltPutVar(psDbHead,"authport", pasUtlLookConfig(psConfig,"AuthHttpPort","8080"));
    utPltPutVar(psDbHead,"defgateway",pasUtlLookConfig(psConfig,"defgateway","192.168.0.1"));
    
    utPltPutVar(psDbHead,"dnsserver1", pasUtlLookConfig(psConfig,"DnsServer1","202.96.209.5"));
    utPltPutVar(psDbHead,"dnsserver2", pasUtlLookConfig(psConfig,"DnsServer2","202.96.199.133"));
    pasUtlFreeConfig(psConfig);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncmsys_net.htm");
    return 0;
}
*/


// 网络及主机配置     
int ncmWebSystemSet_base(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{
    char *pFile,*pBuf;
    unsigned long lSize;
    char *pConfig;
    pasConfList *psConfig;
    utPltDbHead *psDbHead;
    char *pSelect;
    char caRoute[32],caMask[32],caGate[32];
    long lRoute;
    long ethcount;
    char temp_eth[12];
    char temp_ip[12];
    char temp_mask[12];
     long lGroupid0=0;
       //增加网络权限判断
    char caEtcount[12];
    
    
    int iReturn,i=0,j=0,iNum=0;
    FILE *fp;
    char caFile[128]="";
    char caBuf[16000]="";
    char *p;
    int tNum=0,devCou = 0;
    typedef struct {
    	char dns[32];	
    }dns_s;
    dns_s dns_set[5];
    
    netdev_s *netdev;
    
    char tmpdev[16]="" ,caTemp[2048] = "";
    psDbHead = utPltInitDb();
    iReturn = utMsgGetSomeNVar(psMsgHead,1,"etcount",    UT_TYPE_STRING, 4, caEtcount);

    devCou = GetNetCardCount();
    printf(" caEtcount=%d  \n",devCou);
    utPltPutVarF(psDbHead,"etcount","%d",devCou);
    netdev = (netdev_s *)malloc(sizeof(netdev_s)*(devCou+1));
    for (i = 0 ;i <= devCou ;i ++)
    {
    		sprintf(tmpdev,"eth%d",i);
    		DispNetInfo(tmpdev,i,devCou,netdev);	
        utPltPutLoopVarF(psDbHead,"iNum",i+1,"%d",i);
        utPltPutLoopVar(psDbHead,"ethIP",i+1,netdev[i].ip);
    		utPltPutLoopVar(psDbHead,"ethMask",i+1,netdev[i].netmask);
    		memset(tmpdev,0,sizeof(tmpdev));
//    		printf("**** mac=%s  ip=%s broadip=%s netmask=%s \n",netdev[i].mac,netdev[i].ip,netdev[i].broadip,netdev[i].netmask);
    }
    free(netdev);
    netdev=NULL;
    //读取网关
//    res_init();
//    for(i = 0; i< _res.nscount; i++)
//    {
//        struct sockaddr *server = (struct sockaddr*)&_res.nsaddr_list[i];
//        printf("Server : %s \n",inet_ntoa(*(struct in_addr*)&(server->sa_data[2])));	
//    } 
    char gateway[255]={0};

    get_gateway(gateway);
    printf("Gateway:%s\n", gateway);
    utPltPutVar(psDbHead,"defgateway",gateway);
    
    //读取现有dns信息
    strcpy(caFile,"/etc/resolv.conf");
		fp = fopen(caFile,"r");
    if(fp == NULL) {
    	 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","导入","打开文件出错");
        return 0;
    }
    p = fgets(caBuf,10000,fp);
    
    j = 0;
    iNum=0;
    char tmpchar[64]="";
		while(p) 
		{
			  //以空格分隔字符串 读取nameserver的值显示
			  printf("caBuf=%s \n",caBuf);
			  for (p = strtok(caBuf, " \r"); p; p = strtok(NULL, " \r")) 
			  {
//		   		printf("%s len=%d   j=%d \n", p,strlen(p),j);
		   		j++;
		   		if(j == 2 && iNum == 0)
		   		{
		   			strcpy(dns_set[iNum].dns,p);
		   			entertrim(dns_set[iNum].dns);
		   		}
		   		else if(j == 2 && iNum == 1)
		   		{
		   			strcpy(dns_set[iNum].dns,p);
		   			entertrim(dns_set[iNum].dns);
		   		}	
		   		else if(j == 2 && iNum == 2)
		   		{
		   			strcpy(dns_set[iNum].dns,p);
		   			entertrim(dns_set[iNum].dns);
		   		}	
		  	}
		  	
		  	j = 0;				
				p = fgets(caBuf,10000,fp);
				iNum++;
		}
		
    fclose(fp); 
    printf("~~~00~~~~~iNum=%d  dns1=%s   dns2=%s   \n",iNum, dns_set[0].dns,dns_set[1].dns);
    
    if (iNum == 1)
    {
    	utPltPutVar(psDbHead,"dnsserver1",dns_set[0].dns);	
    }
    else if (iNum == 2)
    {
    	utPltPutVar(psDbHead,"dnsserver1",dns_set[0].dns);	
    	utPltPutVar(psDbHead,"dnsserver2",dns_set[1].dns);		
    }
    else if (iNum == 3)
    {
    	utPltPutVar(psDbHead,"dnsserver1",dns_set[0].dns);	
    	utPltPutVar(psDbHead,"dnsserver2",dns_set[1].dns);		
    	utPltPutVar(psDbHead,"dnsserver3",dns_set[2].dns);		
    }
    memset(caFile,0,sizeof(caFile));
    sprintf(caFile,"%s/ncmroute.cnf",MODULEBINPATH);
		fp = fopen(caFile,"r");
    if(fp == NULL) {
//    	 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","导入","打开文件出错");
//        return 0;
       utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncmsys_net.htm");
    }
    p = fgets(caBuf,10000,fp);
   
    j = 0;
    iNum=0;
		while(p) {
//		  	printf("%s len=%d   \n", p,strlen(p));
		  	if (j == 0)
		  	{	
		  		strcpy(caTemp,p);
		  	}
		  	else
		  	{
//		  	  strcat(caTemp,"\n");
		  	  strcat(caTemp,p);
		  	}	
		  	j++;
				p = fgets(caBuf,10000,fp);
				iNum++;
		}
    fclose(fp);
    
    utPltPutVar(psDbHead,"routconf",caTemp);		
//    utPltShowDb(psDbHead);
	pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0880自动备份设定"),ncLang("0427配置文件出错"));
        return 0;
    }
    utPltPutVar(psDbHead,"AdminIp", pasUtlLookConfig(psConfig,"AdminIp","192.168.20.38"));    
    utPltPutVar(psDbHead,"AdminPort", pasUtlLookConfig(psConfig,"AdminPort","9080"));
	pasUtlFreeConfig(psConfig);


    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncmsys_net.htm");
    return 0;
}

//网络接口设置保存
int ncmWebSystemSave_base(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caApp[20];
    char caMsg[256];
    char pBut;
    int iReturn;
    char caBrDev[64];
    char *pConfig,*pFile;
    char *p;
    char caRoute[32],caMask[32],caGate[32];
    long lRoute;
    char *pBuf;
    FILE *fp;
    int i;
    int ethcount;
    char temp_ip[16],temp_mask[16],temp_breth[16],temp_devn[16] = "";
    char defgateway[20] = "",dnsserver1[20] = "" ,dnsserver2[20] = "" , caEtcount[5] = "";
    char caTemp[1024] = "";
	char adminIP[32],adminPort[32];
    pasConfList *psConfig;

  
	
    netdev_s netdev[18];
    utPltDbHead *psDbHead;
    
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead,6,"etcount",       UT_TYPE_STRING, sizeof(caEtcount)-1,  caEtcount
                                          ,"defgateway",    UT_TYPE_STRING, sizeof(defgateway)-1, defgateway
                                          ,"dnsserver1",    UT_TYPE_STRING, sizeof(dnsserver1)-1, dnsserver1
                                          ,"dnsserver2",    UT_TYPE_STRING, sizeof(dnsserver2)-1, dnsserver2
                                        
										  );
    
    
    //获取网卡页面配置信息    IP  掩码   网关
    if (strlen(caEtcount) > 0)
    {
       for (i = 0;i <= atoi(caEtcount);i++)
       {
       	  memset(temp_devn,0,sizeof(temp_devn));
       		sprintf(temp_devn,"eth%dIP",i);printf("***** %s  \n",utMsgGetVar_s(psMsgHead,temp_devn));
       		strcpy(netdev[i].ip , utMsgGetVar_s(psMsgHead,temp_devn));
       		memset(temp_devn,0,sizeof(temp_devn));
       		sprintf(temp_devn,"eth%dMask",i);
       		strcpy(netdev[i].netmask , utMsgGetVar_s(psMsgHead,temp_devn));
       } 	
    }
    
	//修改IP PORT
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0880自动备份设定"),ncLang("0427配置文件出错"));
        return 0;
    }
     if(p = utMsgGetVar_s(psMsgHead,"AdminIp")) {
        pasUtlModiConfig(psConfig,"AdminIp",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"AdminPort")) {
        pasUtlModiConfig(psConfig,"AdminPort",p);
    }
    pasUtlSaveConfig(pConfig,psConfig);
    pasUtlFreeConfig(psConfig);
    //备份现有网卡信息
    sprintf(caTemp,"mkdir %s/httpd/cgi-bin/netdev",SYSTEMPATH);
    system(caTemp);
//    system("mkdir /home/ncmysql/ncsrv/httpd/cgi-bin/netdev");
    memset(caTemp,0,sizeof(caTemp));
    sprintf(caTemp,"yes | cp /etc/sysconfig/network-scripts/ifcfg-eth* %s/httpd/cgi-bin/netdev",SYSTEMPATH);
    system(caTemp);
//    system("yes | cp /etc/sysconfig/network-scripts/ifcfg-eth* /home/ncmysql/ncsrv/httpd/cgi-bin/netdev");
    
    
    //页面配置信息替换模板配置文件   覆盖原有网卡信息 
    for (i = 0;i <= atoi(caEtcount);i++)
    {
    	  //替换脚本
    	  memset(caTemp,0,sizeof(caTemp));
        sprintf(caTemp,"yes | cp %s/bin/ifcfg-eth0 %s",SYSTEMPATH,SYSTEMPATH);
        system(caTemp);
//    	  system("yes | cp /home/ncmysql/ncsrv/bin/ifcfg-eth0 /home/ncmysql/ncsrv");
    	  memset(temp_devn,0,sizeof(temp_devn));
       	sprintf(temp_devn,"eth%d",i);
    	  ncmExecNetworkSrcipt(MODULEBINPATH,"eth0",temp_devn);
    	  ncmExecNetworkSrcipt(MODULEBINPATH,"192.168.20.52",netdev[i].ip);
    	  ncmExecNetworkSrcipt(MODULEBINPATH,"255.255.255.0",netdev[i].netmask);
    	  ncmExecNetworkSrcipt(MODULEBINPATH,"192.168.20.254",defgateway);
    		memset(caTemp,0,sizeof(caTemp));printf("**************1 \n");
    		sprintf(caTemp,"yes | cp %s/bin/ifcfg-eth0 /etc/sysconfig/network-scripts/ifcfg-eth%d",SYSTEMPATH,i);
    		system(caTemp);
    		memset(caTemp,0,sizeof(caTemp));
    		sprintf(caTemp,"yes | cp %s/ifcfg-eth0 %s/bin",SYSTEMPATH,SYSTEMPATH);
    		system(caTemp);
//    		system("yes | cp /home/ncmysql/ncsrv/ifcfg-eth0 /home/ncmysql/ncsrv/bin");
    }
    //重启网络
    system("service network start");
    
    
    //dns 删除以前的，touch一个新的，echo写入
    memset(caTemp,0,sizeof(caTemp));
    system("rm /etc/resolv.conf");
    system("touch /etc/resolv.conf");
    sprintf(caTemp,"echo \"nameserver %s\" > /etc/resolv.conf",dnsserver1);
    system(caTemp);
    memset(caTemp,0,sizeof(caTemp));
    sprintf(caTemp,"echo \"nameserver %s\" >> /etc/resolv.conf",dnsserver2);
    system(caTemp);
    system("source /etc/resolv.conf");
    
    ethcount=3;
    ethcount=atol(utComGetVar_sd(psShmHead,"ethcount","3"));
  //保存路由信息    
    pFile = utComGetVar_sd(psShmHead,"RoutConf","ncmroute.cnf");
    if(pFile == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0606网络配置"),ncLang("0165参数 NoMonWebList不存在"));
        return 0;
    }
    pBuf = utMsgGetVar_s(psMsgHead,"routconf");
    printf("pBuf=%s \n",pBuf);
    if(pBuf) {
        fp = fopen(pFile,"w");
        if(fp) {
            fwrite(pBuf,strlen(pBuf),1,fp);
            fclose(fp);
        }
    }/*
  //保地址转换信息    
    pFile = utComGetVar_sd(psShmHead,"IptableConf","nciptable.cnf");
    pBuf = utMsgGetVar_s(psMsgHead,"iptable");
     if(pBuf) {
        fp = fopen(pFile,"w");
        if(fp) {
            fwrite(pBuf,strlen(pBuf),1,fp);
            fclose(fp);
        }
    }
  
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0880自动备份设定"),ncLang("0427配置文件出错"));
        return 0;
    }
    
    p = utMsgGetVar_s(psMsgHead,"gateway");
    if(p && strcmp(p,"bridge")==0) {
        pasUtlModiConfig(psConfig,"AccessWay","Bridge");
       pasUtlModiConfig(psConfig,"Promisc","No");
    }
    else if(p && strcmp(p,"gateway")==0) {
        pasUtlModiConfig(psConfig,"AccessWay","Gateway");
        pasUtlModiConfig(psConfig,"Promisc","No");
    }
    else
    {
       pasUtlModiConfig(psConfig,"AccessWay","Bypass");
       pasUtlModiConfig(psConfig,"Promisc","Yes");
     }
    
      memset(caBrDev,0,sizeof(caBrDev));
      for(i=0;i<ethcount;i++){
      	sprintf(temp_breth,"breth%d",i);
      	p = utMsgGetVar_s(psMsgHead,temp_breth);       
        if(p) sprintf(caBrDev+strlen(caBrDev),"%s,",ncUtlEthValue(psShmHead,p));
      }
       
       if(strlen(caBrDev)!=0)
       pasUtlModiConfig(psConfig,"BrDev",caBrDev);
       
      p=utMsgGetVar_s(psMsgHead,"defgateway");
      if(p)
      pasUtlModiConfig(psConfig,"defgateway",p);
      
    p = utMsgGetVar_s(psMsgHead,"InDev1");
    if(p) {
        pasUtlModiConfig(psConfig,"InDev1",ncUtlEthValue(psShmHead,p));
    }

    for(i=0;i<ethcount;i++){
    	sprintf(temp_ip,"eth%dIP",i);
    	sprintf(temp_mask,"eth%dMask",i);
    	if(p = utMsgGetVar_s(psMsgHead,temp_ip)) {
        pasUtlModiConfig(psConfig,temp_ip,p);
      }
    	if(p = utMsgGetVar_s(psMsgHead,temp_mask)) {
        pasUtlModiConfig(psConfig,temp_mask,p);
      }
    }
   
     
    if(p = utMsgGetVar_s(psMsgHead,"adminip")) {
        pasUtlModiConfig(psConfig,"AdminHost",p);
        pasUtlModiConfig(psConfig,"AdminIp",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"adminport")) {
        pasUtlModiConfig(psConfig,"AdminPort",p);
    }

    if(p = utMsgGetVar_s(psMsgHead,"authport")) {
        pasUtlModiConfig(psConfig,"AuthHttpPort",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"dnsserver1")) {
        pasUtlModiConfig(psConfig,"dnsserver1",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"dnsserver2")) {
        pasUtlModiConfig(psConfig,"dnsserver2",p);
    }
    
   if(p = utMsgGetVar_s(psMsgHead,"adminmask")) {
        pasUtlModiConfig(psConfig,"AdminMask",p);
    }
    pasUtlSaveConfig(pConfig,psConfig);
    pasUtlFreeConfig(psConfig);
    p = utMsgGetVar_s(psMsgHead,"exec");
    */
    /*
    if(p) {
        ncUtlReReadConFig(psShmHead);
        ncWebSystemGenFile(psShmHead,"/home/ncmysql/ncsrv/bin/startnet_e");
        system("cd /home/ncmysql/ncsrv/httpd/bin;./apachectl restart");
        ncUtlStartNcNet("/home/ncmysql/ncsrv/bin/startnet_e");
    }else {
        ncUtlReReadConFig(psShmHead);
        ncUtlReloadConfig(psShmHead,NC_PID_COLLECT,NC_RELOAD_PQCONFIG,1);
        ncWebSystemGenFile(psShmHead,"/home/ncmysql/ncsrv/bin/startnet");
        p = utMsgGetVar_s(psMsgHead,"app");
        if(p) {
            //system("cd /home/ncmysql/ncwlan/httpd/bin;./apachectl restart");
            ncUtlStartNcNet2("/home/ncmysql/ncsrv/bin/startnet");
            //printf("== app has exec over== \n");
        }
    }
    */
    printf("***** %d  \n",atoi(caEtcount));
    psDbHead = utPltInitDb();

    for (i = 0 ;i <= atoi(caEtcount) ;i ++)
    { 
    	printf("*****i=%d  ip=%s  netmask=%s \n",i,netdev[i].ip,netdev[i].netmask);
    }
    for (i = 0 ;i <= atoi(caEtcount) ;i ++)
    {   
        utPltPutLoopVarF(psDbHead,"iNum",i+1,"%d",i);
        utPltPutLoopVar(psDbHead,"ethIP",i+1,netdev[i].ip);
        printf("***** %s  \n",netdev[i].ip);
    		utPltPutLoopVar(psDbHead,"ethMask",i+1,netdev[i].netmask);
    }

    utPltPutVar(psDbHead,"etcount",caEtcount);	
    utPltPutVar(psDbHead,"defgateway",defgateway);	
    utPltPutVar(psDbHead,"dnsserver1",dnsserver1);	
    utPltPutVar(psDbHead,"dnsserver2",dnsserver2);		
    utPltPutVar(psDbHead,"routconf",pBuf);		
    utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncmsys_net.htm");
	//utPltPutVar(psDbHead,"mesg","成功修改");
	//utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_msg.htm");
//    ncmWebSystemSet_base(psShmHead,iFd,psMsgHead);
    
    return 0;
}
//Portal参数设置
int ncmPortal_ParaSet(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{       
    char *pConfig;
    char *pFile;
    pasConfList *psConfig;
    utPltDbHead *psDbHead;
    int iReturn;
    psDbHead = utPltInitDb();
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","Portal参数设置",ncLang("0427配置文件出错"));
        return 0;
    }
    //文档中少了个m
 		utPltPutVar(psDbHead,"PortalName",   pasUtlLookConfig(psConfig,"PortalName","Apple"));
    utPltPutVar(psDbHead,"TcpPort",   pasUtlLookConfig(psConfig,"TcpPort","9985"));
    utPltPutVar(psDbHead,"MinTcpProcess",   pasUtlLookConfig(psConfig,"MinTcpProcess","8"));
    utPltPutVar(psDbHead,"MaxTcpProcess",   pasUtlLookConfig(psConfig,"MaxTcpProcess","32"));
    utPltPutVar(psDbHead,"AdminIp",   pasUtlLookConfig(psConfig,"AdminIp","192.168.20.38"));
    utPltPutVar(psDbHead,"AdminPort",   pasUtlLookConfig(psConfig,"AdminPort","9080"));
    utPltPutVar(psDbHead,"AuthType",   pasUtlLookConfig(psConfig,"AuthType","Chap"));
    
    pasUtlFreeConfig(psConfig);
    //utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysset/ncm_portal_paraset.htm");
    return 0;
}
//保存Portal参数设置
int ncmPortalParaSave_par(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caApp[20];
    char caMsg[256];
    int iReturn;
    char *pConfig;
    char *p,*pFile,*pNat,*pRoute;
    FILE *fp;
    pasConfList *psConfig;
//    utMsgPrintMsg(psMsgHead);
    char caUsername2[32];
 	  dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","Portal参数设置",ncLang("0427配置文件出错"));
        return 0;
    }
    // utMsgGetVar_s 从消息数据包中取指定变量的内容。变量内容必须为字符串。
    if(p = utMsgGetVar_s(psMsgHead,"PortalName")) {
        pasUtlModiConfig(psConfig,"PortalName",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"TcpPort")) {
        pasUtlModiConfig(psConfig,"TcpPort",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"MinTcpProcess")) {
        pasUtlModiConfig(psConfig,"MinTcpProcess",p);
    }
    
    if(p = utMsgGetVar_s(psMsgHead,"MaxTcpProcess")) {
        pasUtlModiConfig(psConfig,"MaxTcpProcess",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"AdminIp")) {
        pasUtlModiConfig(psConfig,"AdminIp",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"AdminPort")) {
        pasUtlModiConfig(psConfig,"AdminPort",p);
    }
    
    if(p = utMsgGetVar_s(psMsgHead,"AuthType")) {
        pasUtlModiConfig(psConfig,"AuthType",p);
    }
    
    pasUtlSaveConfig(pConfig,psConfig);
    pasUtlFreeConfig(psConfig);
    p = utMsgGetVar_s(psMsgHead,"app");
    if(p) {
        if(!utStrIsSpaces(p)) {
            ncUtlReReadConFig(psShmHead);
        }
    }
    
    ncmWriteSysLog(psMsgHead,caUsername2,"03","success","修改Portal参数设置");   
    ncmPortal_ParaSet(psShmHead,iFd,psMsgHead);
    
    return 0;
}

//短信服务器参数设置
int ncmSms_Server_ParaSet(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{       
    char *pConfig;
    char *pFile;
    pasConfList *psConfig;
    utPltDbHead *psDbHead;
    int iReturn;
    psDbHead = utPltInitDb();
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","短信服务器参数设置",ncLang("0427配置文件出错"));
        return 0;
    }
 		utPltPutVar(psDbHead,"SmsType",   pasUtlLookConfig(psConfig,"SmsType","Pronetway"));
    utPltPutVar(psDbHead,"SmsIp",     pasUtlLookConfig(psConfig,"SmsIp","61.129.34.70"));
    utPltPutVar(psDbHead,"SmsPort",   pasUtlLookConfig(psConfig,"SmsPort","7195"));
    utPltPutVar(psDbHead,"SmsKey",    pasUtlLookConfig(psConfig,"SmsKey","Pronetway"));
    
    pasUtlFreeConfig(psConfig);
//    utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysset/ncm_smsserver_paraset.htm");
    return 0;
}
//保存短信服务器参数设置
int ncmSmsParaSave_par(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caApp[20];
    char caMsg[256];
    int iReturn;
    char *pConfig;
    char *p,*pFile,*pNat,*pRoute;
    FILE *fp;
    pasConfList *psConfig;
//    utMsgPrintMsg(psMsgHead);
    char caUsername2[32];
 	  dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","短信服务器参数设置",ncLang("0427配置文件出错"));
        return 0;
    }
    
    if(p = utMsgGetVar_s(psMsgHead,"SmsType")) {
        pasUtlModiConfig(psConfig,"SmsType",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"SmsIp")) {
        pasUtlModiConfig(psConfig,"SmsIp",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"SmsPort")) {
        pasUtlModiConfig(psConfig,"SmsPort",p);
    }
    
    if(p = utMsgGetVar_s(psMsgHead,"SmsKey")) {
        pasUtlModiConfig(psConfig,"SmsKey",p);
    }
    
    pasUtlSaveConfig(pConfig,psConfig);
    pasUtlFreeConfig(psConfig);
    p = utMsgGetVar_s(psMsgHead,"app");
    if(p) {
        if(!utStrIsSpaces(p)) {
            ncUtlReReadConFig(psShmHead);

        }
    }
    ncmWriteSysLog(psMsgHead,caUsername2,"03","success","修改短信服务器参数设置");   
    ncmSms_Server_ParaSet(psShmHead,iFd,psMsgHead);
    
    return 0;
}

/*
Http端口: 9080
Http最大连接数: 300
修改参数:
httpd/conf/httpd.conf
*/
//Http服务器参数设置****
int ncmHttp_Server_ParaSet(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)  // 不用
{       
    char *pConfig;
    char *pFile;
    pasConfList *psConfig;
    utPltDbHead *psDbHead;
    int iReturn;
    char httpport[16]="" , httpconn[16]="";

    iReturn = utMsgGetSomeNVar(psMsgHead,2,"HttpPort", UT_TYPE_STRING,sizeof(httpport) - 1, httpport,
                                           "HttpConn", UT_TYPE_STRING,sizeof(httpconn) - 1, httpconn);
    //指定默认的httpd.conf
    //按格式替换
    //有默认设置   应用不成功恢复默认重新设置替换。
    
    /*
    
    //备份操作前设置
    system("mv /home/ncmysql/ncsrv/httpd/conf/httpd.conf /home/ncmysql/ncsrv/httpd/cgi-bin/httpdold.conf");
    
    //备份标准并拷贝到要替换的目标
    system("yes | cp /home/ncmysql/ncsrv/httpd/cgi-bin/httpd_standard.conf cp /home/ncmysql/ncsrv/httpd/cgi-bin/httpd_standard2.conf");
    system("yes | cp /home/ncmysql/ncsrv/httpd/cgi-bin/httpd_standard.conf cp /home/ncmysql/ncsrv/httpd/conf");
    
    //替换httpd_standard.conf中参数
    if(strlen(httpport)>0)
    {
    	ncmExechttpdSrcipt("/home/ncmysql/ncsrv/httpd/conf","Listen 9080","Listen ",httpport);
    	ncmExechttpdSrcipt("/home/ncmysql/ncsrv/httpd/conf","_default_:9080","_default_:",httpport);
    }
    if(strlen(httpconn)>0)
    {
    	ncmExechttpdSrcipt("/home/ncmysql/ncsrv/httpd/conf","MaxClients 150","MaxClients ",httpconn);
    }
    //最新设置
    system("mv /home/ncmysql/ncsrv/httpd/conf/httpd_standard.conf /home/ncmysql/ncsrv/httpd/conf/httpd.conf");
    
    //恢复标准
    system("mv /home/ncmysql/ncsrv/httpd/cgi-bin/httpd_standard2.conf cp /home/ncmysql/ncsrv/httpd/cgi-bin/httpd_standard.conf");
    
    //执行
//    system("killall -9 httpd");
//    system("killall -9 httpd");
//    system("/home/ncmysql/ncsrv/httpd/bin/apachectl start");
    */
    //参数返还界面
    psDbHead = utPltInitDb();
    utPltPutVar(psDbHead,"HttpPort",  httpport);
    utPltPutVar(psDbHead,"HttpConn",  httpconn);
    
//    utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysset/ncm_httpserver_paraset.htm");
    return 0;
}
//保存Http服务器参数设置
int ncmHttpParaSave_par(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{   
    char caApp[20];
    char caMsg[256],caTmp[1024];
    int iReturn;
    char *pConfig;
    char *p,*pFile,*pNat,*pRoute;
    FILE *fp;
    pasConfList *psConfig;
//    utMsgPrintMsg(psMsgHead);
    char caUsername2[32];
 	  dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
    char httpport[16]="" , httpconn[16]="";
    utPltDbHead *psDbHead;

    iReturn = utMsgGetSomeNVar(psMsgHead,2,"HttpPort", UT_TYPE_STRING,sizeof(httpport) - 1, httpport,
                                           "HttpConn", UT_TYPE_STRING,sizeof(httpconn) - 1, httpconn);
    //提前制作默认的httpd_standard.conf
    
    //有默认设置   应用不成功恢复默认重新设置替换。

    //备份操作前设置
    sprintf(caTmp,"mv %s/httpd/conf/httpd.conf %s/httpd/cgi-bin/httpdold.conf",SYSTEMPATH,SYSTEMPATH);
    system(caTmp);
 //   system("mv /home/ncmysql/ncsrv/httpd/conf/httpd.conf /home/ncmysql/ncsrv/httpd/cgi-bin/httpdold.conf");
    
    //备份标准并拷贝到要替换的目标
    memset(caTmp,0,sizeof(caTmp));
    sprintf(caTmp,"yes | cp %s/httpd/cgi-bin/httpd_standard.conf %s/httpd/cgi-bin/httpd_standard2.conf",SYSTEMPATH,SYSTEMPATH);
    system(caTmp);
//    system("yes | cp /home/ncmysql/ncsrv/httpd/cgi-bin/httpd_standard.conf /home/ncmysql/ncsrv/httpd/cgi-bin/httpd_standard2.conf");
    memset(caTmp,0,sizeof(caTmp));
    sprintf(caTmp,"yes | cp %s/httpd/cgi-bin/httpd_standard.conf %s/httpd/conf",SYSTEMPATH,SYSTEMPATH);
    system(caTmp);
//    system("yes | cp /home/ncmysql/ncsrv/httpd/cgi-bin/httpd_standard.conf /home/ncmysql/ncsrv/httpd/conf");
    
    memset(caTmp,0,sizeof(caTmp));
    sprintf(caTmp,"%s/httpd/conf",caTmp);
    //替换httpd_standard.conf中参数
    if(strlen(httpport)>0)
    {
    	ncmExechttpdSrcipt(caTmp,"Listen 9080","Listen ",httpport);
    	ncmExechttpdSrcipt(caTmp,"_default_:9080","_default_:",httpport);
    }
    if(strlen(httpconn)>0)
    {
    	ncmExechttpdSrcipt(caTmp,"MaxClients 150","MaxClients ",httpconn);
    }
    memset(caTmp,0,sizeof(caTmp));
    sprintf(caTmp,"mv %s/httpd/conf/httpd_standard.conf %s/httpd/conf/httpd.conf",SYSTEMPATH,SYSTEMPATH);
    system(caTmp);
    //最新设置
//    system("mv /home/ncmysql/ncsrv/httpd/conf/httpd_standard.conf /home/ncmysql/ncsrv/httpd/conf/httpd.conf");
    
    //恢复标准
    memset(caTmp,0,sizeof(caTmp));
    sprintf(caTmp,"mv %s/httpd/cgi-bin/httpd_standard2.conf %s/httpd/cgi-bin/httpd_standard.conf",SYSTEMPATH,SYSTEMPATH);
    system(caTmp);
//    system("mv /home/ncmysql/ncsrv/httpd/cgi-bin/httpd_standard2.conf /home/ncmysql/ncsrv/httpd/cgi-bin/httpd_standard.conf");
    
    //执行
    system("killall -9 httpd");
    system("killall -9 httpd");
    system("killall -9 httpd");
    memset(caTmp,0,sizeof(caTmp));
    sprintf(caTmp,"%s/httpd/bin/apachectl start",SYSTEMPATH);
    system(caTmp);
//    system("/home/ncmysql/ncsrv/httpd/bin/apachectl start");
    
    //参数返还界面
    psDbHead = utPltInitDb();
    
    utPltPutVar(psDbHead,"HttpPort",  httpport);
    utPltPutVar(psDbHead,"HttpConn",  httpconn);
    
    ncmWriteSysLog(psMsgHead,caUsername2,"03","success","修改HTTP服务器参数设置"); 
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysset/ncm_httpserver_paraset.htm");
    
    return 0;
}
//AC默认参数设置
int ncmAC_Server_ParaSet(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{       
    char *pConfig;
    char *pFile;
    pasConfList *psConfig;
    utPltDbHead *psDbHead;
    int iReturn,i=0,j=0,iNum=0;
    FILE *fp;
    char caFile[128]="";
    char caBuf[16000]="";
    char *p;
    psDbHead = utPltInitDb();
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","AC服务器参数设置",ncLang("0427配置文件出错"));
        return 0;
    }
 		utPltPutVar(psDbHead,"DefAcIp",   pasUtlLookConfig(psConfig,"DefAcIp","192.168.21.101"));
    utPltPutVar(psDbHead,"DefAcPort",   pasUtlLookConfig(psConfig,"DefAcPort","2000"));
    pasUtlFreeConfig(psConfig);
    
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysset/ncm_acserver_paraset.htm");
    return 0;
}

//AC参数设置
int ncmAC_Server_SelfParaSet(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{      
    utPltDbHead *psDbHead;
    int iReturn,i=0,j=0,iNum=0;
    FILE *fp;
    char caFile[128]="";
    char caBuf[16000]="";
    char *p;
    
		acinfo_s acinfo[1500];
    char acname[32] = "",acip[24] = "",acport[8] = "";
    long maxaid = 0 , lCurPg = 0 , lRowNum = 0,lStartRec = 0 , lUserid = 0 ,lCou = 0 ,aid = 0;
    char caTotPg[12] = "",caCurPg[12] = "",caPage[8] = "",caLimit[8] = "",caSesid[1024] = "",caDid[8] = "";
    char caKeyword[32] = "",caMid[8] = "",caStart[8] = "",caDel[8] = "",caUpdate[8] = "";
    char caTemp[1024] = "",caTemp_d[1024] = "",caUserid[8] = "",caUsername2[32] = "" ,caSmt[1024] = "";
    char caUsername[32] = "" ,caDispname[32] = "";
    int tNum=0;
    pasDbCursor *psCur;
    psDbHead = utPltInitDb();
    dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);

    utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,9, "TotPg",           UT_TYPE_STRING,sizeof(caTotPg)-1,  caTotPg,
															              "CurPg",           UT_TYPE_STRING,sizeof(caCurPg)-1,  caCurPg,
												   	                "page",            UT_TYPE_STRING,sizeof(caPage)-1,   caPage,
												   	                "limit",           UT_TYPE_STRING,sizeof(caLimit)-1,  caLimit,
												   	                "selsid",          UT_TYPE_STRING,sizeof(caSesid)-1,  caSesid,
												   	                "mokeyword",       UT_TYPE_STRING,sizeof(caKeyword)-1,caKeyword,
												   	                "start",           UT_TYPE_STRING,sizeof(caStart)-1,  caStart,
												   	                "del",             UT_TYPE_STRING,sizeof(caDel)-1,    caDel,
												   	                "update",          UT_TYPE_STRING,sizeof(caUpdate)-1, caUpdate);   	
 	  
		trim(caSesid);
		trim(caKeyword);
   	if(strlen(caKeyword)>0){
			 pasCvtGBK(2,caKeyword,caTemp,30);
			 strcpy(caKeyword,caTemp);
		}  
	  //修改或增加
	  if(strcmp(caUpdate,"update")==0){
	  	   	iReturn = utMsgGetSomeNVar(psMsgHead,4,
	  	   		"ac_id",     UT_TYPE_STRING, sizeof(caUserid)-1,caUserid,
	   	      "acname",    UT_TYPE_STRING, sizeof(acname)-1,  acname,
						"acip",      UT_TYPE_STRING, sizeof(acip)-1,    acip,
					  "acport",    UT_TYPE_STRING, sizeof(acport)-1,  acport);
					  
					  trim(acname);
					  trim(acip);
					  trim(acport);
					  		//编码转换
						if(strlen(acname)>0){
							 	pasCvtGBK(2,acname,caTemp,sizeof(acname)-1);
							 	strcpy(acname,caTemp);
						}
						if(strlen(acip)>0){
							 	pasCvtGBK(2,acip,caTemp,sizeof(acip)-1);
								strcpy(acip,caTemp);
						}
						if(strlen(acport)>0){
							 	pasCvtGBK(2,acport,caTemp,sizeof(acport)-1);
							 	strcpy(acport,caTemp);
						}
						if(atol(caUserid)>0){
								lUserid = atol(caUserid);				
								sprintf(caTemp,"update ncmaccnf set name='%s',ip='%s',port='%s',addtime = %lu where aid = %s ",acname,acip,acport,time(0),caUserid);
								iReturn = pasDbExecSqlF(caTemp);
	//							printf("sqlbuf=%s\n",caTemp);
								sprintf(caTemp,"修改AC定义设置，代码：%s,显示名：%s",caUsername,caDispname);    
				        ncmWriteSysLog(psMsgHead,caUsername2,"05","成功",caTemp);  
						}
						else
						{
				 			   sprintf(caTemp,"select max(aid)+1 from ncmaccnf  ");
		  					 pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&maxaid);
		  					 if(maxaid==0) maxaid=1;
				 				 sprintf(caTemp,"insert into ncmaccnf (aid,name,ip,port,addtime) values (%lu,'%s','%s','%s',%lu)",maxaid,acname,acip,acport,time(0));								 
					       iReturn = pasDbExecSqlF(caTemp);
		             sprintf(caTemp,"AC自定义设置 %s，名称：%s",caUsername,caDispname);    
		             ncmWriteSysLog(psMsgHead,caUsername2,"05","成功",caTemp);
						}
	  }
  
    if(!utStrIsSpaces(caDel)){  /* 删除选中的项目*/
    		if(strlen(caSesid)>0){
//    		      iReturn = pasDbExecSqlF("delete from ncmaccnf where aid in(%s)",caSesid);
    		      iReturn = pasDbExecSqlF("update ncmaccnf set flags = 1 where aid in(%s)",caSesid);
            	sprintf(caTemp,"update ncmaccnf set flags = 1 where aid in(%s) ",caSesid);
            	if(iReturn==0){
            		ncmWriteSysLog(psMsgHead,caUsername2,"05","成功",caTemp);
            	}else{
            		ncmWriteSysLog(psMsgHead,caUsername2,"05","失败",caTemp);
            	}
    		}
		}
		
    if(strlen(caPage)>0){
    		strcpy(caCurPg,caPage);
    }
    lCurPg = atol(caCurPg);
    if(lCurPg <= 0) lCurPg = 1;
    if(strlen(caLimit) > 0){
    		 lRowNum=atol(caLimit);
    }else{
         lRowNum =38;
    }
    if(strlen(caStart) == 0) strcpy(caStart,"0");
    lStartRec = (lCurPg - 1) * lRowNum;   //当前页面的开始第一行的行数	

    sprintf(caTemp,"select count(*) from ncmaccnf where 1=1 ");
    if(strlen(caKeyword) > 0){
    		sprintf(caTemp+strlen(caTemp)," and (name like '%c%s%c' or  ip like '%c%s%c' )",'%',caKeyword,'%','%',caKeyword,'%');
    } 
//    printf("caTemp=%s\n",caTemp);
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(long),&lCou);
    printf("lCou=%d\n",lCou);
    
    sprintf(caSmt,"select aid,name,ip,port,addtime from ncmaccnf  where 1 = 1 and flags = 0 ");
    if(strlen(caKeyword) > 0){
    		sprintf(caSmt+strlen(caSmt)," and (name like '%c%s%c' or  ip like '%c%s%c' )",'%',caKeyword,'%','%',caKeyword,'%');
    } 
    sprintf(caSmt+strlen(caSmt),"  order by addtime desc  ");
//	    sprintf(caSmt+strlen(caSmt),"  order by addtime desc limit %d,%d ",atol(caStart),lRowNum);
  	printf("caSmt=%s\n",caSmt);
    psCur = pasDbOpenSql(caSmt,0);
    if(psCur==NULL) {  	
        	utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","Ac配置查询","数据库出错");
        	return 0;
    }
		aid=0;
		memset(acname,0,sizeof(acname));
		memset(acip,0,sizeof(acip));
		memset(acport,0,sizeof(acport));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG, sizeof(long),     &aid,
																	UT_TYPE_STRING,sizeof(acname)-1,acname,
																	UT_TYPE_STRING,sizeof(acip)-1,  acip,
																	UT_TYPE_STRING,sizeof(acport)-1,acport);
												
		iNum = 0;
		while((iReturn == 0)||(iReturn==1405)) { 
		 iNum++;
		 acinfo[iNum-1].aid=aid;
		 strcpy(acinfo[iNum-1].acname,acname);
		 strcpy(acinfo[iNum-1].acip,  acip);
		 strcpy(acinfo[iNum-1].acport,acport);
		 aid=0;
		 memset(acname,0,sizeof(acname));
		 memset(acip,0,sizeof(acip));
		 memset(acport,0,sizeof(acport));
		 iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG, sizeof(long),     &aid,
		 																UT_TYPE_STRING,sizeof(acname)-1,acname,
		 																UT_TYPE_STRING,sizeof(acip)-1,  acip,
		 																UT_TYPE_STRING,sizeof(acport)-1,acport);
		}
		pasDbCloseCursor(psCur); 
 	  
 		if(strlen(caKeyword) > 0){      //查询不同步 修改才同步
		 		tNum=0;
		 		for (i = 0 ;i < iNum ; i++){
	 	  	    if(tNum>0){
	          	 utPltPutLoopVar(psDbHead,"ac_dh",tNum+1,",");
	          }
	          tNum++;
	 	  	   	utPltPutLoopVarF(psDbHead,"ac_iNum", tNum,"%d",tNum);
	 	  	   	utPltPutLoopVarF(psDbHead,"ac_id",   tNum,"%d",acinfo[i].aid);
	   			  utPltPutLoopVar(psDbHead, "acname",  tNum,acinfo[i].acname);
	   			  utPltPutLoopVar(psDbHead, "acip",    tNum,acinfo[i].acip);
	   			  utPltPutLoopVar(psDbHead, "acport",  tNum,acinfo[i].acport);
		 	  }
  	}else{
		 	  //应该从db中同步到acinfo.cnf
		 	  if(iNum>0){
		 	  	  memset(caTemp,0,sizeof(caTemp));
		 	  	  sprintf(caTemp,"yes | cp %s/acinfo.cnf %s/httpd/cgi-bin/acinfo.cnf ",MODULEBINPATH,SYSTEMPATH);
		 	  	  system(caTemp);
		// 	  	  system("yes | cp /home/ncmysql/ncsrv/bin/acinfo.cnf /home/ncmysql/ncsrv/httpd/cgi-bin/acinfo.cnf ");
		 	  	  //生成新的cnf
		 	  	  memset(caTemp,0,sizeof(caTemp));
		        sprintf("rm %s/acinfo.cnf",MODULEBINPATH);
		        system(caTemp);
		// 	  	  system("rm /home/ncmysql/ncsrv/bin/acinfo.cnf");
		        memset(caTemp,0,sizeof(caTemp));
		        sprintf("touch %s/acinfo.cnf",MODULEBINPATH);
		        system(caTemp);
		// 	  	  system("touch /home/ncmysql/ncsrv/bin/acinfo.cnf");
		        memset(caTemp,0,sizeof(caTemp));
		        sprintf("echo \"\# Ac Info\" > %s/acinfo.cnf",MODULEBINPATH);
		        system(caTemp);
		// 	  	  system("echo \"\# Ac Info\" > /home/ncmysql/ncsrv/bin/acinfo.cnf");
		        memset(caTemp,0,sizeof(caTemp));
		        sprintf("echo \"\# AcName                AcIp               AcPort\" >> %s/acinfo.cnf",MODULEBINPATH);
		        system(caTemp);
		// 	  	  system("echo \"\# AcName                AcIp               AcPort\" >> /home/ncmysql/ncsrv/bin/acinfo.cnf");
		 	  }
		 	  for (i = 0 ;i < iNum ; i++){
		 	  	  memset(caTemp,0,sizeof(caTemp));
		 	  	  sprintf(caTemp,"echo \"%-24s%-19s%s\" >> %s/acinfo.cnf",acinfo[i].acname,acinfo[i].acip,acinfo[i].acport,MODULEBINPATH);
		 	  	  printf("caTemp=%s \n",caTemp);
		 	  	  system(caTemp);
		 	  }
		    
				strcpy(caFile,MODULEBINPATH);
				strcat(caFile,"/acinfo.cnf");
				fp = fopen(caFile,"r");
		    if(fp == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","导入","打开文件出错");
		        return 0;
		    }
		    p = fgets(caBuf,10000,fp);
		    // 忽略前2行  
		    p = fgets(caBuf,10000,fp);
		    p = fgets(caBuf,10000,fp);
		    
		    j = 0;
		    iNum=0;
				while(p) {
					  //以空格分隔字符串 读入数组
					  printf("caBuf=%s \n",caBuf);
					  
					  for (p = strtok(caBuf, " \r"); p; p = strtok(NULL, " \r")) 
					  {
		//		   		printf("%s len=%d   \n", p,strlen(p));
				   		j++;
				   		if(j == 1)
				   		{
				   			strcpy(acinfo[iNum].acname,p);
				   		}	
				   		if(j == 2)
				   		{
				   			strcpy(acinfo[iNum].acip,p);
				   		}	
				   		if(j == 3)
				   		{
				   			strcpy(acinfo[iNum].acport,p);
				   			entertrim(acinfo[iNum].acport);
				   		}	
				  	}
				  	
				  	j = 0;				
						p = fgets(caBuf,10000,fp);
						iNum++;
				}
		    fclose(fp);
		    
		    //判断数据库中是否有记录，没有则插入cnf中记录
		    lCou = 0;
		    sprintf(caTemp,"select count(*) from ncmaccnf  ");
			  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(long),&lCou);
			  
				tNum=0;
				for (i = 0; i < iNum; i++){
					 tNum++;
					 if(tNum>1){
		       	utPltPutLoopVar(psDbHead,"dh",tNum,",");
		       }
		       utPltPutLoopVarF(psDbHead,"iNum",tNum,"%d",tNum);
		       utPltPutLoopVarF(psDbHead,"ac_id",tNum,"%d",acinfo[i].aid);
		       printf("name=%s ip=%s port=%s \n",acinfo[i].acname,acinfo[i].acip,acinfo[i].acport);
		       utPltPutLoopVar(psDbHead,"acname",tNum,acinfo[i].acname);
		       utPltPutLoopVar(psDbHead,"acip",  tNum,acinfo[i].acip);
		       utPltPutLoopVar(psDbHead,"acport",tNum,acinfo[i].acport);
		       
		       if (lCou == 0)            //第一次配置文件读入数据库
		       {
		       			maxaid = 0;
							  sprintf(caTemp,"select max(aid)+1 from ncmaccnf  ");
							  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&maxaid);
			  				if(maxaid==0) maxaid=1;
					 			sprintf(caTemp,"insert into ncmaccnf (aid,name,ip,port,addtime) values (%lu,'%s','%s','%s',%lu)",\
					 			maxaid,acinfo[i].acname,acinfo[i].acip,acinfo[i].acport,time(0));								 
					      printf("sqlbuf=%s\n",caTemp);	
			          iReturn = pasDbExecSqlF(caTemp);
		       } 
				}
				if (lCou == 0){
		    	  sprintf(caTemp,"AC自定义设置 %s，名称：%s",caUsername,caDispname);    
			      ncmWriteSysLog(psMsgHead,caUsername2,"05","成功",caTemp);	
		    }
  	}
  	p = utMsgGetVar_s(psMsgHead,"app");
    if (p) 
    {
        if (!utStrIsSpaces(p)) 
        {
        	   	//新的接口函数
//            ncUtlReReadConFig(psShmHead);
        }
    }
		utPltPutVarF(psDbHead,"TotRec","%d",iNum);

//    utPltShowDb(psDbHead);
//    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysset/ncm_acserver_paraset.htm");
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysset/ncm_ext_show_acinfo.htm");
    return 0;
}

//保存AC参数设置
int ncmACParaSave_par(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caApp[20];
    char caMsg[256];
    int iReturn;
    char *pConfig;
    char *p,*pFile,*pNat,*pRoute;
    FILE *fp;
    pasConfList *psConfig;
//    utMsgPrintMsg(psMsgHead);
    char caUsername2[32];
 	  dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","AC服务器参数设置",ncLang("0427配置文件出错"));
        return 0;
    }
    
    if(p = utMsgGetVar_s(psMsgHead,"DefAcIp")) {
        pasUtlModiConfig(psConfig,"DefAcIp",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"DefAcPort")) {
        pasUtlModiConfig(psConfig,"DefAcPort",p);
    }
    
    pasUtlSaveConfig(pConfig,psConfig);
    pasUtlFreeConfig(psConfig);
    p = utMsgGetVar_s(psMsgHead,"app");
    if(p) {
        if(!utStrIsSpaces(p)) {
            ncUtlReReadConFig(psShmHead);
        }
    }
    ncmWriteSysLog(psMsgHead,caUsername2,"03","success","修改AC服务器参数设置");   
    ncmAC_Server_ParaSet(psShmHead,iFd,psMsgHead);
    
    return 0;
}
//Radius参数设置
int ncmRadius_Server_ParaSet(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{       
    char *pConfig;
    char *pFile;
    pasConfList *psConfig;
    utPltDbHead *psDbHead;
    int iReturn;
    psDbHead = utPltInitDb();
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","Radius服务器参数设置",ncLang("0427配置文件出错"));
        return 0;
    }
 		utPltPutVar(psDbHead,"RadiusIp",   pasUtlLookConfig(psConfig,"RadiusIp","192.168.20.39"));
    utPltPutVar(psDbHead,"RadiusPort",   pasUtlLookConfig(psConfig,"RadiusPort","7178"));
    utPltPutVar(psDbHead,"RadiusKey",    pasUtlLookConfig(psConfig,"RadiusKey","Pronetway"));
    
    pasUtlFreeConfig(psConfig);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysset/ncm_radiusserver_paraset.htm");
    return 0;
}
//保存Radius参数设置
int ncmRadiusParaSave_par(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caApp[20];
    char caMsg[256];
    int iReturn;
    char *pConfig;
    char *p,*pFile,*pNat,*pRoute;
    FILE *fp;
    pasConfList *psConfig;
    char caUsername2[32];
 	  dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","Radius服务器参数设置",ncLang("0427配置文件出错"));
        return 0;
    }
    
    if(p = utMsgGetVar_s(psMsgHead,"RadiusIp")) {
        pasUtlModiConfig(psConfig,"RadiusIp",p);
    }
    if(p = utMsgGetVar_s(psMsgHead,"RadiusPort")) {
        pasUtlModiConfig(psConfig,"RadiusPort",p);
    }
    
    if(p = utMsgGetVar_s(psMsgHead,"RadiusKey")) {
        pasUtlModiConfig(psConfig,"RadiusKey",p);
    }
    
    pasUtlSaveConfig(pConfig,psConfig);
    pasUtlFreeConfig(psConfig);
    p = utMsgGetVar_s(psMsgHead,"app");
    if (p) 
    {
        if (!utStrIsSpaces(p)) 
        {
            ncUtlReReadConFig(psShmHead);
        }
    }
    ncmWriteSysLog(psMsgHead,caUsername2,"03","success","修改Radius服务器参数设置");   
    ncmRadius_Server_ParaSet(psShmHead,iFd,psMsgHead);
    
    return 0;
}

//微信配置
int ncm_webweixin_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char wid[20] = "";
	 char name[64] = "";
	 char token[48] = "";
	 char account[32] = "";
	 char appid[32] = "";
	 char appsecret[64] = "";
	 char sendcont[32] = "";
	 char redurl[255] = "";
	 char usinfo[8] = "";
	 char access_token[4096] = "";
	 char systime[32] = "";
	 char status[20] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long lWid = 0;
	 char caName[64] = "";
	 char caToken[48] = "";
	 char caAccount[32] = "";
	 char caAppid[32] = "";
	 char caAppsecret[64] = "";
	 char caSendcont[32] = "";
	 char caRedurl[255] = "";
	 char caUsinfo[8] = "";
	 char caAccess_token[4096] = "";
	 unsigned long lSystime = 0;
	 unsigned long lStatus = 0;
	 char s_status[32] = "";
	 char s_systime[32] = "";
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caSesid[1024] = "";
	 char caTemp[4096] = "";
	 // 关键字
	 char _keyword[33] = "";
	 
	 // 数据表
	 unsigned long lCount1 = 0; // 表中记录的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "" ,caRefresh[10] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 22,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
															    "wid",         UT_TYPE_STRING,  sizeof(wid)-1,            wid,
																	"name",        UT_TYPE_STRING,  sizeof(name)-1,           name,
																	"token",       UT_TYPE_STRING,  sizeof(token)-1,          token,
																	"account",     UT_TYPE_STRING,  sizeof(account)-1,        account,
																	"appid",       UT_TYPE_STRING,  sizeof(appid)-1,          appid,
																	"appsecret",   UT_TYPE_STRING,  sizeof(appsecret)-1,      appsecret,
																	"sendcont",    UT_TYPE_STRING,  sizeof(sendcont)-1,       sendcont,
																	"redurl",      UT_TYPE_STRING,  sizeof(redurl)-1,         redurl,
																	"usinfo",      UT_TYPE_STRING,  sizeof(usinfo)-1,         usinfo,
																	"access_token",UT_TYPE_STRING,  sizeof(access_token)-1,   access_token,
																	"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
																	"status",      UT_TYPE_STRING,  sizeof(status)-1,         status,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"update",      UT_TYPE_STRING,  10,                       caUpdate,
																	"refresh",     UT_TYPE_STRING,  10,                       caRefresh,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport);
																	
		if(strcmp(caRefresh,"refresh") == 0)
		{
			  ncmInitWeiXinMem(psShmHead);
		}																
																	
	 // 排序
	 if(strlen(sort)==0){
	 	  strcpy(sort,"wid");
	 }
	 // 去除关键字前后的空格
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
   if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,64);
			strcpy(name,caTemp);
   }
	 if(strlen(token)>0){
			pasCvtGBK(2,token,caTemp,48);
			strcpy(token,caTemp);
   }
   if(strlen(account)>0){
			pasCvtGBK(2,account,caTemp,30);
			strcpy(account,caTemp);
   }
   if(strlen(appid)>0){
			pasCvtGBK(2,appid,caTemp,30);
			strcpy(appid,caTemp);
   }
   if(strlen(appsecret)>0){
			pasCvtGBK(2,appsecret,caTemp,64);
			strcpy(appsecret,caTemp);
   }
   if(strlen(sendcont)>0){
			pasCvtGBK(2,sendcont,caTemp,30);
			strcpy(sendcont,caTemp);
   }
   if(strlen(redurl)>0){
			pasCvtGBK(2,redurl,caTemp,30);
			strcpy(redurl,caTemp);
   }
   if(strlen(usinfo)>0){
			pasCvtGBK(2,usinfo,caTemp,30);
			strcpy(usinfo,caTemp);
   }
   if(strlen(access_token)>0){
			pasCvtGBK(2,access_token,caTemp,512);
			strcpy(access_token,caTemp);
   }
   
   unsigned long lSid = 0;
   lSid=utMd5Code(account,strlen(account),"ffff");
	 if(lSid>2000000000) lSid=lSid-2000000000;
	 if(lSid>2000000000) lSid=lSid-2000000000;
	 printf("lSid = %d\n",lSid);
   
	 // 增加/修改
	 if(!utStrIsSpaces(name) && !utStrIsSpaces(token) && !utStrIsSpaces(account) && !utStrIsSpaces(redurl)){
			 if(!utStrIsSpaces(caUpdate)) {
			 		if (strcmp(caUpdate, "update")==0){
			 				sprintf(caTemp,"update ncmweixin set wid=%lu,name='%s',token='%s',account='%s',appid='%s',appsecret='%s',sendcont='%s',redurl='%s',usinfo='%s',systime=%lu,status=%lu where wid=%lu ",lSid,name,token,account,appid,appsecret,sendcont,redurl,usinfo,time(0),atol(status),atol(wid));
//			 				printf("caTemp = %s\n", caTemp);
					 		pasDbExecSqlF(caTemp);
			 	  } else if(strcmp(caUpdate, "add")==0){
				    	sprintf(caTemp,"insert into ncmweixin (wid,name,token,account,appid,appsecret,sendcont,redurl,usinfo,systime,status) values (%lu,'%s','%s','%s','%s','%s','%s','%s','%s',%lu,%lu)",lSid,name,token,account,appid,appsecret,sendcont,redurl,usinfo,time(0),atol(status));
					 		pasDbExecSqlF(caTemp);
			 	  }
			 }
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from ncmweixin where wid in(%s)",caSesid);
    	}
	 }
	 	 
	 // 查询
	 strcpy(sql, "select count(*) from ncmweixin where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and account like '%%%s%%'", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
//	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select wid, name, token, account, appid, appsecret, sendcont, redurl, usinfo, systime, status from ncmweixin where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and account like '%%%s%%'", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
//	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),       &lWid,
	 														 UT_TYPE_STRING, sizeof(name)-1,     caName,
	 														 UT_TYPE_STRING, sizeof(token)-1,    caToken,
	 														 UT_TYPE_STRING, sizeof(account)-1,  caAccount,
	 														 UT_TYPE_STRING, sizeof(appid)-1,    caAppid,
	 														 UT_TYPE_STRING, sizeof(appsecret)-1,caAppsecret,
	 														 UT_TYPE_STRING, sizeof(sendcont)-1, caSendcont,
	 														 UT_TYPE_STRING, sizeof(redurl)-1,   caRedurl,
	 														 UT_TYPE_STRING, sizeof(usinfo)-1,   caUsinfo,
															 UT_TYPE_ULONG,  sizeof(long),       &lSystime,
															 UT_TYPE_ULONG,  sizeof(long),       &lStatus);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 
			 if(strlen(caUsinfo)==0){
			 		strcpy(caUsinfo, "No");	
			 }
			 
			 switch(lStatus){
			 		case 0:strcpy(s_status, "启用");break;	
			 		case 1:strcpy(s_status, "停用");break;	
			 		default:break;		
			 }
			 
			 if(lSystime>0){
			 		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
			 }else{
			 		strcpy(s_systime, "");
			 }
		   
		   utPltPutLoopVarF(psDbHead,"wid",iNum,"%lu",lWid);
		   utPltPutLoopVar(psDbHead,"name",iNum,caName);
		   utPltPutLoopVar(psDbHead,"token",iNum,caToken);
			 utPltPutLoopVar(psDbHead,"account",iNum,caAccount);
			 utPltPutLoopVar(psDbHead,"appid",iNum,caAppid);
			 utPltPutLoopVar(psDbHead,"appsecret",iNum,caAppsecret);
			 utPltPutLoopVar(psDbHead,"sendcont",iNum,caSendcont);
			 utPltPutLoopVar(psDbHead,"redurl",iNum,caRedurl);
			 utPltPutLoopVar(psDbHead,"usinfo",iNum,caUsinfo);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVarF(psDbHead,"status",iNum,"%lu",lStatus);
			 utPltPutLoopVar(psDbHead,"s_status",iNum,s_status);
			 
			 lWid = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caToken, 0, sizeof(caToken));
			 memset(caAccount, 0, sizeof(caAccount));
			 memset(caAppid, 0, sizeof(caAppid));
			 memset(caAppsecret, 0, sizeof(caAppsecret));
			 memset(caSendcont, 0, sizeof(caSendcont));
			 memset(caRedurl, 0, sizeof(caRedurl));
			 memset(caUsinfo, 0, sizeof(caUsinfo));
	 		 lSystime = 0;
	 		 lStatus = 0;
	 		 memset(s_status, 0, sizeof(s_status));
	 		 memset(s_systime, 0, sizeof(s_systime));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),       &lWid,
			 														 UT_TYPE_STRING, sizeof(name)-1,     caName,
			 														 UT_TYPE_STRING, sizeof(token)-1,    caToken,
			 														 UT_TYPE_STRING, sizeof(account)-1,  caAccount,
			 														 UT_TYPE_STRING, sizeof(appid)-1,    caAppid,
			 														 UT_TYPE_STRING, sizeof(appsecret)-1,caAppsecret,
			 														 UT_TYPE_STRING, sizeof(sendcont)-1, caSendcont,
			 														 UT_TYPE_STRING, sizeof(redurl)-1,   caRedurl,
			 														 UT_TYPE_STRING, sizeof(usinfo)-1,   caUsinfo,
																	 UT_TYPE_ULONG,  sizeof(long),       &lSystime,
																	 UT_TYPE_ULONG,  sizeof(long),       &lStatus);
	  }
//		utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysset/ncm_webweixin_list.htm");
	  return 0;	
}
