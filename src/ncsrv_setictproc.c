#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utoall.h"
#include "pasutl.h"
#include "pasdb.h"
#include "pasutl.h"
#include "ncportal.h"
#define  PAS_SRCFILE 7000
int checkUserOnline(utShmHead *psShmHead);

static void ncmexecdel_user(utShmHead *psShmHead,char username[32])
{
   /* char tmp[128] = "";
    system("cd /home/ncmysql/ncsrv/bin");
    sprintf(tmp,"./portalonline -t %s",username);
	pasLog(6111,6111,"tmp=%s\n",tmp);
	system(tmp);*/
	ncSrvLetUserOffLineByTsid(psShmHead,ncmWebGetTisdByUname(psShmHead,username));
}

int ncSrvSetIctProcess(utShmHead *psShmHead)
{
    
     pasUtlSetProcessNameD(psShmHead,                   
                                 "CkUserOn",             // 进程名称
                                 "No",                // StartImpLog 的缺省值  Yes -- 启动   No--不启动
                                 checkUserOnline,  // 调用的函数名
                                 7100,                 // 进程超时时间
                                 0);                   // 
    
    return 0; 
}
/*int checkUserOrder(utShmHead *psShmHead)
{
	//utMsgPrintMsg(psMsgHead);
	char sql[1024] = "";
	char vname[32] = "";
	char mobno[16] = "";
	ulong packageid = 0;
	char name[64] = "";
	uint8 endtime = 0;
	char caMark[128] = "";
	char autoxiding[4] = "";
	uint8 nowTime=time(0);	
	pasDbCursor *psCur = NULL;
    //查找所有生效的套餐	

	snprintf(sql, sizeof(sql)-1, "select username,mobno,packageid,name,endtime,autoxiding from userorder where userorder.endtime<=%llu and status=1", nowTime);//
	psCur = pasDbOpenSql(sql, 0);
	
	int iret = 0;
	if(psCur)
	{
		while(0 == (iret = pasDbFetchInto(psCur,
							  UT_TYPE_STRING, sizeof(vname) - 1, vname,
							  UT_TYPE_STRING, sizeof(mobno) - 1, mobno,
							  UT_TYPE_LONG, 4, &packageid,
							  UT_TYPE_STRING, sizeof(name) - 1, name,
							  UT_TYPE_LONG8, 8, &endtime,
							  UT_TYPE_STRING, sizeof(autoxiding) - 1 , autoxiding)) || 1405 == iret)
		{			
			if(autoxiding[0] == '0')
			{
				memset(sql, 0, sizeof(sql));
				snprintf(sql, sizeof(sql)-1, "update userorder set status='2' where username='%s'", vname);
				pasDbExecSqlF(sql);
				pasLogs(7666, 7666, "原套餐未自动订购\n");
			}
			else
			{
				memset(sql, 0, sizeof(sql));
				snprintf(sql, sizeof(sql)-1, "select ncsrvuserex.money,package.money,package.pdays from ncsrvuserex,package where ncsrvuserex.username='%s' and package.id=%lu", vname, packageid);
				long lmoney = 0; 
				long pmoney = 0;
				long pdays = 0;
				pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lmoney,						   							   
									   UT_TYPE_LONG, 4, &pmoney,
									   UT_TYPE_LONG, 4, &pdays);
				if(lmoney < pmoney)
				{
					//余额不足，无法自动订购
					memset(sql, 0, sizeof(sql));
					snprintf(sql, sizeof(sql)-1, "update userorder set status='2' where username='%s'", vname);
					pasDbExecSqlF(sql);
					pasLogs(7666, 7666, "余额不足无法自动订购，余额%ld,续订套餐价格%ld\n",lmoney,pmoney);
				}
				else
				{
					snprintf(caMark, sizeof(caMark) - 1, "%s",utComGetVar_sd(psShmHead, "mark", "0"));
					
					memset(sql, 0, sizeof(sql));
					snprintf(sql, sizeof(sql)-1, "update ncsrvuserex set money=money-%ld where username='%s'", pmoney,vname); 
					pasDbExecSqlF(sql);
					pasLogs(7666, 7666, "扣款:%s\n",sql);
					
					memset(sql, 0, sizeof(sql));
					snprintf(sql, sizeof(sql)-1, "update userorder set status=2 where username='%s'", vname);//原套餐失效
					pasDbExecSqlF(sql);
					pasLogs(7666, 7666, "原套餐失效:%s\n",sql);
					
					memset(sql, 0, sizeof(sql));
				//	snprintf(sql, sizeof(sql)-1, "update userorder set starttime=%llu,endtime=%llu,timeval=%llu",nowtime,nowtime+pdays*24*3600,nowtime);
					snprintf(sql, sizeof(sql) - 1, "insert into userorder(username,mobno,mark,packageid,name,starttime,endtime,autoxiding,status,timeval) values('%s','%s','%s',%lu,'%s',%llu,%llu,'1','1',%llu)",vname,mobno,caMark,packageid,name,endtime,endtime+pdays*24*3600,endtime);
					pasDbExecSqlF(sql);
					pasLogs(7666, 7666, "写入orderlog:%s\n",sql);
					
					char tableTime[12] = "";
					char caName[64] = "";
					snprintf(caName, sizeof(caName)-1, "%s(续订)",name);
					snprintf(tableTime, sizeof(tableTime) - 1, "%s", utTimFormat("%Y%m", nowTime));
					memset(sql, 0, sizeof(sql));
					snprintf(sql, sizeof(sql) - 1, "insert into userorderlog_%s(username,mobno,mark,packageid,name,status,money,starttime,endtime,timeval) values('%s','%s','%s',%lu,'%s','2',%lu,%llu,%llu,%llu)", tableTime, vname, mobno, caMark, packageid, caName, pmoney, endtime, endtime+pdays*24*3600, endtime);
					pasDbExecSqlF(sql);
					pasLogs(7666, 7666, "写入userorderlog:%s\n",sql);
				}
			}
		}
		pasDbCloseCursor(psCur);
	}		
	return 0;
}*/

int checkUserOnline(utShmHead *psShmHead)
{
	pasHashInfo sHashInfo;
    uchar *pHash;
    ncPortalOnline  *psOnline;
	ulong lCount=0;
	uint8 nowTime=0;
	long sleepTime = 60;
	int iReturn = 0;
	ulong count = 0;
	char sql[1024] = "";
	char vname[32] = "";
	char mobno[16] = "";
	ulong packageid = 0;
	char name[64] = "";
	uint8 endtime = 0;
	uint8 minendtime = 0;
	char caMark[128] = "";
	char autoxiding[4] = "";
	
	pasDbCursor *psCur = NULL;
	iReturn = pasConnect(psShmHead);
	if(iReturn !=0 )
	{
		pasLogs(5001,5001,"connect error iReturn=%d\n",iReturn);
		sleep(5);
		return 0;
	}
	sleepTime = utComGetVar_ld(psShmHead, "sleeptime", 60);
	while(1) 
    { 
		pHash =  (unsigned char *)utShmHashHead(psShmHead,NCSRV_LNK_ONLINE);				
		if(pHash == NULL) 
		{
			return (-1);
		}
		psOnline = (ncPortalOnline *)pasHashFirst(pHash,&sHashInfo);
		while(psOnline)
		{	
			if(strlen(psOnline->caName)==0)
			{
				psOnline = (ncPortalOnline *)pasHashNextS(&sHashInfo);
				continue;
			}	
			nowTime=time(0);
			snprintf(sql, sizeof(sql)-1, "select count(*) from userorder where username='%s' and status=1", psOnline->caName);
			pasDbOneRecord(sql, 0,  UT_TYPE_ULONG, 4, &count);
			if(count==0)//无套餐 踢下线
			{
				if(psOnline->login==1){
					ncmexecdel_user(psShmHead,psOnline->caName);	
					pasLogs(6111,6111,"无套餐 剔除 name=%s\n",psOnline->caName);
				}	
			}
			
			if(count==2)//2个套餐
			{
				snprintf(sql, sizeof(sql)-1, "select min(endtime) from userorder where username='%s' and status=1 and autoxiding=0", psOnline->caName);
				pasDbOneRecord(sql, 0,  UT_TYPE_LONG8, 8, &minendtime);
				if(minendtime<=nowTime)
				{
					memset(sql, 0, sizeof(sql));
					snprintf(sql, sizeof(sql)-1, "update userorder set status='2' where username='%s' and endtime=%llu", psOnline->caName,minendtime);
					pasDbExecSqlF(sql);	
					pasLogs(6111,6111,"套餐一失效 套餐二生效 name=%s\n",psOnline->caName);
				}
			}
			if(count==1)
			{
				memset(sql, 0, sizeof(sql));
				snprintf(sql, sizeof(sql)-1, "select mobno,packageid,name,endtime,autoxiding from userorder where username='%s' and status=1", psOnline->caName, psOnline->caName);
				pasDbOneRecord(sql, 0, UT_TYPE_STRING, sizeof(mobno)-1, mobno,
										  UT_TYPE_ULONG, 4, &packageid,	
										  UT_TYPE_STRING, sizeof(name)-1, name,
										  UT_TYPE_LONG8, 8, &endtime,
										  UT_TYPE_STRING, sizeof(autoxiding)-1, autoxiding);		
				if(endtime <= nowTime)
				{
					if(autoxiding[0] == '0')
					{
						memset(sql, 0, sizeof(sql));
						snprintf(sql, sizeof(sql)-1, "update userorder set status='2' where username='%s'",  psOnline->caName);
						pasDbExecSqlF(sql);	
						if(psOnline->login==1){	
							ncmexecdel_user(psShmHead,psOnline->caName);
							pasLogs(6111,6111,"套餐结束，但未自动订购 剔除sql=%s name=%s\n",sql,psOnline->caName);
						}	
						
					}
					if(autoxiding[0] == '1')
					{
						memset(sql, 0, sizeof(sql));
						snprintf(sql, sizeof(sql)-1, "select ncsrvuserex.money,package.money,package.pdays from ncsrvuserex,package where ncsrvuserex.username='%s' and package.id=%lu",  psOnline->caName, packageid);
						long lmoney = 0; 
						long pmoney = 0;
						long pdays = 0;
						pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lmoney,						   							   
											   UT_TYPE_LONG, 4, &pmoney,
											   UT_TYPE_LONG, 4, &pdays);
						if(lmoney < pmoney)
						{
							//余额不足，无法自动订购
							memset(sql, 0, sizeof(sql));
							snprintf(sql, sizeof(sql)-1, "update userorder set status='2' where username='%s'",  psOnline->caName);
							pasDbExecSqlF(sql);
							if(psOnline->login==1){	
								ncmexecdel_user(psShmHead,psOnline->caName);
								pasLogs(6111, 6111, "余额不足无法自动订购，剔除，余额%ld,续订套餐价格%ld\n",lmoney,pmoney);
							}	
							
						}
						else
						{
							snprintf(caMark, sizeof(caMark) - 1, "%s",utComGetVar_sd(psShmHead, "mark", "0"));
							
							memset(sql, 0, sizeof(sql));
							snprintf(sql, sizeof(sql)-1, "update ncsrvuserex set money=money-%ld where username='%s'", pmoney, psOnline->caName); 
							pasDbExecSqlF(sql);
							pasLogs(6111, 6111, "扣款:%s\n",sql);
							
							memset(sql, 0, sizeof(sql));
							snprintf(sql, sizeof(sql)-1, "update userorder set status=2 where username='%s'",  psOnline->caName);//原套餐失效
							pasDbExecSqlF(sql);
							pasLogs(6111, 6111, "原套餐失效:%s\n",sql);
							
							memset(sql, 0, sizeof(sql));
						//	snprintf(sql, sizeof(sql)-1, "update userorder set starttime=%llu,endtime=%llu,timeval=%llu",nowtime,nowtime+pdays*24*3600,nowtime);
							snprintf(sql, sizeof(sql) - 1, "insert into userorder(username,mobno,mark,packageid,name,starttime,endtime,autoxiding,status,timeval) values('%s','%s','%s',%lu,'%s',%llu,%llu,'1','1',%llu)", psOnline->caName,mobno,caMark,packageid,name,endtime,endtime+pdays*24*3600,endtime);
							pasDbExecSqlF(sql);
							pasLogs(6111, 6111, "写入userorder:%s\n",sql);
							
							char tableTime[12] = "";
							char caName[64] = "";
							snprintf(caName, sizeof(caName)-1, "%s(续订)",name);
							snprintf(tableTime, sizeof(tableTime) - 1, "%s", utTimFormat("%Y%m", nowTime));
							memset(sql, 0, sizeof(sql));
							snprintf(sql, sizeof(sql) - 1, "insert into userorderlog_%s(username,mobno,mark,packageid,name,status,money,starttime,endtime,timeval) values('%s','%s','%s',%lu,'%s','2',%lu,%llu,%llu,%llu)", tableTime,  psOnline->caName, mobno, caMark, packageid, caName, pmoney, endtime, endtime+pdays*24*3600, endtime);
							pasDbExecSqlF(sql);
							pasLogs(6111, 6111, "写入userorderlog:%s\n",sql);
						}
					}
				}
			}					
			psOnline = (ncPortalOnline *)pasHashNextS(&sHashInfo);
		}			
		sleep(sleepTime);
    }
	return 0;	
}



/*int checkUserOnline(utShmHead *psShmHead)
{
	pasHashInfo sHashInfo;
    uchar *pHash;
    ncPortalOnline  *psOnline;
	ulong lCount=0;
	uint8 nowTime=0;
	long sleepTime = 60;
	int iReturn = 0;
	char sql[1024] = "";
	iReturn = pasConnect(psShmHead);
	if(iReturn !=0 )
	{
		pasLogs(5001,5001,"connect error iReturn=%d\n",iReturn);
		sleep(5);
		return 0;
	}
	sleepTime = utComGetVar_ld(psShmHead, "sleeptime", 60);
	while(1) 
    { 
		pHash =  (unsigned char *)utShmHashHead(psShmHead,NCSRV_LNK_ONLINE);				
		if(pHash == NULL) 
		{
			return (-1);
		}
		psOnline = (ncPortalOnline *)pasHashFirst(pHash,&sHashInfo);
		while(psOnline)
		{
			nowTime=time(0);
			memset(sql,0,sizeof(sql));		
			snprintf(sql,sizeof(sql)-1,"select count(*) from userorder where username='%s' and status=1 and starttime<=%llu and endtime>%llu", psOnline->caName, nowTime, nowTime);			
			pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);
			if(lCount == 0)
			{
				//ncmexecdel_user(psOnline->caName);
				pasLogs(6111,6111,"sql=%s name=%s\n",sql,psOnline->caName);	
			}
			psOnline = (ncPortalOnline *)pasHashNextS(&sHashInfo);
		}			
		sleep(sleepTime);
    }
	return 0;	
}*/

/*

int ncSrvImpSqlFileTest(utShmHead *psShmHead)
{
    char caLogPath[128];
    char caErrFile[128];
    char caKeepPath[128];
    char caSdate[16],caSdate0[16];
    int  iError=0,iCount,iReturn,iFlags=0;
    strcpy(caLogPath,utComGetVar_sd(psShmHead,"dblogpath","../log/db"));
    strcpy(caErrFile,utComGetVar_sd(psShmHead,"dbImpErrFile","../log/impdberror.log"));
    strcpy(caKeepPath,utComGetVar_sd(psShmHead,"dbimpkeeppath","\0"));
    if(utStrIsSpaces(caKeepPath)) {
       iFlags = 0;
    }
    else {
       iFlags = 1;
    }
    iReturn = pasConnect(psShmHead);
    if(iReturn != 0) {
        pasLog(PAS_LOG_ERROR,"Connect Database Error Sqlcode is %d %s",iReturn,pasDbErrorMsg(NULL));
        return (-1);
    }
    strcpy(caSdate0,"\0");
    while(1) {
        strcpy(caSdate,utTimFormat("%Y%m",time(0)));
        if(strcasecmp(caSdate,caSdate0) != 0) {
            ncSrvCheckAndGenTable(caSdate);
            strcpy(caSdate0,caSdate);
        }
        iCount = pasUtlExecSqlPath(caLogPath,caKeepPath,caErrFile,".sql",iFlags,&iError);
        sleep(5);
    }
    return 0;
}


*/