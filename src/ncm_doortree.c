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
#include "dsutl.h"
#include "ncportalweb.h"
#include "ncportal.h"

// 门店树
int ncm_group_manager_DoorTree_comp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    utMsgPrintMsg(psMsgHead);
    
    // 初始化数据库
    pasDbCursor *psCur = NULL;
    int iNum = 0;
    int iReturn = 0;
    int i = 0, j = 0, k = 0;
    unsigned long lCount1 = 0;
    char caTemp[4024] = "";
    utPltDbHead *psDbHead = utPltInitDb(); // 初始化数据模板
   	
   	// 接收信息变量
   	char caPid[16];
   	char caKeyword[64];
   	char caKeyword1[64];
   	
   	// 数据库读取数据后存放变量
   	char caGroupname[72];
   	unsigned long lPid;
   	unsigned long lGroupid;
   	
	iReturn = utMsgGetSomeNVar(psMsgHead,2, "node",		UT_TYPE_STRING, 10, caPid,
											"keyword",  UT_TYPE_STRING, 60, caKeyword1);
    
    trim(caKeyword1);
	memset(caKeyword,0,sizeof(caKeyword));
	if(strlen(caKeyword1)>0)
	{
		  pasCvtGBK(2,caKeyword1,caTemp,60);
		  strcpy(caKeyword,caTemp);
	}
    printf("caKeyword = %s\n", caKeyword);
    
    // 读取配置文件的配置时间
	char cnfProvince[32] = "";
	char *pConfig;
	pasConfList *psConfig;

	pConfig = (char *)pasGetConfigFileName();
	psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
    if(psConfig == NULL) 
	{
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","读取配置文件",ncLang("0427配置文件出错"));
        return 0;
    }
    
    //如若没有配置，默认null。
    strcpy(cnfProvince,(char *)pasUtlLookConfig(psConfig,"NcmProvince","")); // pasUtlLookConfig的最后一个参数为StatisticsStim的默认值
    printf("cnfProvince = %s\n", cnfProvince);
    pasUtlFreeConfig(psConfig);
    
    iNum = 0;
    memset(caTemp,0,sizeof(caTemp));
    if(strlen(caKeyword)==0)
	{
		sprintf(caTemp,"select aid, aname, pid from ncmarea where 1=1 ");
		sprintf(caTemp+strlen(caTemp)," and pid=%d ",atol(caPid));
		if(atol(caPid)==0)
		{
			sprintf(caTemp+strlen(caTemp)," and aname='%s' ",cnfProvince);		
		}
		sprintf(caTemp+strlen(caTemp)," order by aid ASC");
		printf("caTemp=%s\n",caTemp);
		psCur=pasDbOpenSql(caTemp,0);
		if(psCur == NULL) 
		{
			utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","门店查询","数据库出错");
			return 0;
		}
		memset(caGroupname,0,sizeof(caGroupname));
		lGroupid=0;
		lPid=0;
		iReturn = pasDbFetchInto(psCur,	UT_TYPE_LONG,4,&lGroupid,
										UT_TYPE_STRING,31,caGroupname,
										UT_TYPE_ULONG,4,&lPid);
			
		while(iReturn == 0||iReturn==1405) 
		{
			 if(iNum>0)
			 {
     			  utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
			 }
			 iNum++;
		     
			 utPltSetCvtHtml(1);  
			 utPltPutLoopVar(psDbHead,"groupname",iNum,utStrGetCharHan(caGroupname,48));
			 utPltPutLoopVar(psDbHead,"ctext",iNum,utStrGetCharHan(caGroupname,48));
			 utPltSetCvtHtml(0);  
			 utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
			 utPltPutLoopVar(psDbHead,"leaf", iNum,"false");
			 utPltPutLoopVar(psDbHead,"cls", iNum,"folder");	
			 utPltPutLoopVar(psDbHead,"dtype",iNum,"2");
			 utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);	
			 utPltPutLoopVarF(psDbHead,"pid",iNum,"%lu",lPid);
			     
			 memset(caGroupname,0,sizeof(caGroupname));
			 lGroupid=0;
			 lPid=0;
			 iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lGroupid,
											UT_TYPE_STRING,31,caGroupname,
											UT_TYPE_ULONG,4,&lPid);
		}
		pasDbCloseCursor(psCur);
    }
    //utPltPutVar(psDbHead,"rootname",utComGetVar_sd(psShmHead,"rootname",cnfProvince));
    
    // 获取门店个数
	memset(caTemp,0,sizeof(caTemp));
	sprintf(caTemp, "select count(*) from ncsrvgroup where 1=1");
	if(strlen(caKeyword)==0)
	{
		if(strlen(caPid)==0)
		{
			sprintf(caTemp+strlen(caTemp)," and left(groupname, 6) not in (select aid from ncmarea) ");
		}
		else
		{
			sprintf(caTemp+strlen(caTemp)," and left(groupname, 6)=%d ",atol(caPid));	
		}
    }
	else
	{
		sprintf(caTemp+strlen(caTemp)," and (groupname like '%%%s%%' ) ",caKeyword);
    }    
    sprintf(caTemp+strlen(caTemp)," order by groupname");
	pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
	
	memset(caTemp,0,sizeof(caTemp));
    sprintf(caTemp,"select groupid,dispname,left(groupname, 6) from ncsrvgroup where 1=1 ");
    if(strlen(caKeyword)==0)
	{
    	if(strlen(caPid)==0)
		{
    		sprintf(caTemp+strlen(caTemp)," and left(groupname, 6) not in (select aid from ncmarea)");
    	}
		else
		{
    		sprintf(caTemp+strlen(caTemp)," and left(groupname, 6)=%d ",atol(caPid));	
    	}
    }
	else
	{
		sprintf(caTemp+strlen(caTemp)," and (dispname like '%%%s%%') ",caKeyword);
    }    
    sprintf(caTemp+strlen(caTemp)," order by dispname limit 300");
    printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur!=NULL)
	{
		memset(caGroupname,0,sizeof(caGroupname));
	    lGroupid=0;
	    lPid=0;
	    iReturn = pasDbFetchInto(psCur,	UT_TYPE_ULONG,4,&lGroupid,
	    								UT_TYPE_STRING,68,caGroupname,
										UT_TYPE_ULONG,4,&lPid);
		printf("Groupname:%s\n", caGroupname);                             
	    while(iReturn==0||iReturn==1405) 
		{
			if(iNum>0)
			{
				utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
			}
			iNum++;

			utPltPutLoopVar(psDbHead,"groupname",iNum,utStrGetCharHan(caGroupname,48));
			utPltPutLoopVarF(psDbHead,"did",iNum,"%lu",lGroupid);
			utPltPutLoopVarF(psDbHead,"pid",iNum,"%lu",lPid);
			utPltPutLoopVar(psDbHead,"leaf",iNum,"true");
			utPltPutLoopVar(psDbHead,"cls", iNum,"file");
			utPltPutLoopVar(psDbHead,"dtype",iNum,"0");
			utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
			utPltPutLoopVar(psDbHead,"treecls",iNum,"treecompt");

			memset(caGroupname,0,sizeof(caGroupname));
			lGroupid=0;
			lPid=0;
			iReturn = pasDbFetchInto(psCur,	UT_TYPE_ULONG,4,&lGroupid,
											UT_TYPE_STRING,68,caGroupname,
											UT_TYPE_ULONG,4,&lPid);	
	    }			
    }
    
    //utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"doortree/ncm_group_select_doortree.htm");
    return 0;
}
