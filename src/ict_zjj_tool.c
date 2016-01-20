#include "ict_zjj_tool.h"


int zjj_insert_shm(utShmHead *psShmHead, char *username, char *pass)//降用户名和短信密码插入共享内存
{
	pasHashInfo sHashInfo;
	uchar *pHash = NULL;
	pHash = (unsigned char *)utShmHashHead(psShmHead, ICT_USER_PASS);	
	if(pHash == NULL)
	{
		pasLogs(1234,1234,"aa\n");
	}
	pasLogs(1234, 1234, "want to insert username=%s,lenu=%d, pass=%s,lenp=%d\n", username,strlen(username), pass,strlen(pass));
	ictPassUser* psData = (ictPassUser*)utShmHashLookA(psShmHead, ICT_USER_PASS, username);
	if(psData)
    {
		pasLogs(1234,1234,"get psData vname=%s,lenv=%d,pass=%s, lenp=%d\n",psData->vName,strlen(psData->vName),psData->pass, strlen(psData->pass));;
		memset(psData->vName, 0, sizeof(psData->vName));
		memset(psData->pass, 0, sizeof(psData->pass));
		snprintf(psData->vName, sizeof(psData->vName)-1, "%s", username);
		snprintf(psData->pass, sizeof(psData->pass)-1, "%s", pass);
		/*pasLogs(1234,1234,"if cc %s,%s,%d,%d\n",psData->vName,psData->pass,strlen(psData->vName),strlen(psData->pass));
		pasLogs(1234,1234,"if dd aaaa1 %s,%s\n",username,pass);*/
	//	memcpy(psData->vName, username, strlen(username));
	//	memcpy(psData->pass, pass, strlen(pass));  
    }
	else
	{
		pasLogs(1234,1234,"else ee %s,%s\n",username,pass);
	}
	
	return 0;
}

char* zjj_find_shm(utShmHead *psShmHead, char *username)
{

	static char pass[12] = "";
	pasHashInfo sHashInfo;
	uchar *pHash = NULL;
	ictPassUser* psPass = NULL;
	pHash = (unsigned char *)utShmHashHead(psShmHead, ICT_USER_PASS);	
	memset(pass, 0 ,sizeof(pass));
	psPass = (ictPassUser*)pasHashFirst(pHash, &sHashInfo);
	pasLogs(1234,1234,"=======================查询==================\n");
    while(psPass)
    {
		pasLogs(1234,1234,"11 %s,%s,%d,%d\n",psPass->vName,psPass->pass,strlen(psPass->vName),strlen(psPass->pass));
		if(strcmp(psPass->vName, username)==0)
		{
			pasLogs(1234,1234,"一致 %s,%s\n",psPass->vName,psPass->pass);
			snprintf(pass, sizeof(pass)-1, "%s", psPass->pass);
			//break;
		}
        psPass = (ictPassUser *)pasHashNextS(&sHashInfo);
	
    }
	pasLogs(1234,1234,"=======================查询==================\n\n");
	/*
	ictPassUser* psData = (ictPassUser*)utShmHashLook(psShmHead, ICT_USER_PASS, username);
	 if(psData)
    {
		printf("bbbbb=%s,pass=%s\n", username,psData->pass);
		memcpy(pass, psData->pass, sizeof(psData->pass));      
    }
	else{
		printf("ccccccc\n");
	}
	*/
	return pass;
}

void zjj_delete_shm(utShmHead *psShmHead, char *username)
{
	
	pasHashInfo sHashInfo;
	uchar *pHash = NULL;
	ictPassUser* psPass = NULL;
	pHash = (unsigned char *)utShmHashHead(psShmHead, ICT_USER_PASS);	
	psPass = (ictPassUser*)pasHashFirst(pHash, &sHashInfo);
	pasLogs(1234,1234,"=======================删除==================\n");
	pasLogs(1234,1234,"手机号 %s\n",username);
    while(psPass)
    {
		pasLogs(1234,1234,"11 %s,%s,%d,%d\n",psPass->vName,psPass->pass,strlen(psPass->vName),strlen(psPass->pass));
		if(strcmp(psPass->vName, username)==0)
		{
			pasLogs(1234,1234,"删除 %s,%s\n",psPass->vName,psPass->pass);
			utShmHashDel(psShmHead,ICT_USER_PASS,psPass);
		}
        psPass = (ictPassUser *)pasHashNextS(&sHashInfo);

    }
		
	pasLogs(1234,1234,"=======================删除==================\n\n");	
}



int addUserToShm(utShmHead *psShmHead)
{

    return 0;
}

int isInUserShm(utShmHead *psShmHead)
{

    return 0;
}


int ictPrint(long isDebug, const char* fmt, ...)
{
    int retCnt = 0;
    if(isDebug)
    {
        va_list ap;
        va_start(ap, fmt);
        retCnt = vprintf(fmt, ap);
        va_end(ap);
    }
    return retCnt;
}


int isVnameRegistered(char* vName)
{
    return 0;
}

uint8 getTsidByVname(utShmHead *psShmHead, char* vName)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    int iCount = 0, iReturn, lcid = 0;
    ncPortalUser    *psUser;
    ncPortalOnline  *psOnline;
    long lBase;
    char opt[256] = "";
    char *p = NULL;
    long long ltsid = 0;
    pHash = (unsigned char *)utShmHashHead(psShmHead, NCSRV_LNK_ONLINE);

    if(pHash == NULL)
    {
        return "";
    }
    lBase = utShmGetBaseAddr(psShmHead);
    psOnline = (ncPortalOnline *)pasHashFirst(pHash, &sHashInfo);
    while(psOnline)
    {
        psUser = (ncPortalUser *)((char *)psOnline->psUser + lBase);
        if(strcmp(psOnline->caName, vName) == 0)
        {
            ltsid = psOnline->lTsid;
            break;
        }
        psOnline = (ncPortalOnline *)pasHashNextS(&sHashInfo);
    }

    return ltsid;
}


int setOnlineVname(utShmHead *psShmHead, char* vName, uint8 llTsid)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    ncPortalOnline  *psOnline;
    pHash = (unsigned char *)utShmHashHead(psShmHead, NCSRV_LNK_ONLINE);

    if(pHash == NULL)
    {
        return "";
    }
    psOnline = (ncPortalOnline *)pasHashFirst(pHash, &sHashInfo);
    while(psOnline)
    {
        if(psOnline->lTsid == llTsid)
        {
			memset(psOnline->caName, 0, sizeof(psOnline->caName));
            strcpy(psOnline->caName, vName);
            pasLogs(2115, 2115, "set tsid=%llu vaname=%s\n", llTsid, vName);
            break;
        }
        psOnline = (ncPortalOnline *)pasHashNextS(&sHashInfo);
    }
	return 0;
}

char* getVnameByTsId(utShmHead *psShmHead, uint8 ltsid)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    int iCount = 0, iReturn, lcid = 0;
    ncPortalUser    *psUser;
    ncPortalOnline  *psOnline;
    long lBase;
    char opt[256] = "";
    char *p = NULL;
    static char vName[64] = "";
    pHash = (unsigned char *)utShmHashHead(psShmHead, NCSRV_LNK_ONLINE);

    if(pHash == NULL)
    {
        return "";
    }
    lBase = utShmGetBaseAddr(psShmHead);
    psOnline = (ncPortalOnline *)pasHashFirst(pHash, &sHashInfo);
    while(psOnline)
    {
        psUser = (ncPortalUser *)((char *)psOnline->psUser + lBase);
        if(psOnline->lTsid == ltsid)
        {
            strcpy(vName, psOnline->caName);
            pasLogs(2115, 1002, "vaname=%s\n", vName);
            break;
        }
        psOnline = (ncPortalOnline *)pasHashNextS(&sHashInfo);
    }

    return vName;
}



int isTsidLogin(utShmHead *psShmHead, uint8 lTsid)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    int iCount = 0, iReturn = 0;
    ncPortalOnline  *psOnline;
    long lBase;
    pHash = (unsigned char *)utShmHashHead(psShmHead, NCSRV_LNK_ONLINE);

    if(pHash == NULL)
    {
        return "";
    }
    lBase = utShmGetBaseAddr(psShmHead);
    psOnline = (ncPortalOnline *)pasHashFirst(pHash, &sHashInfo);
    while(psOnline)
    {
		pasLogs(1234,1233," for tsid=%llu", psOnline->lTsid);
		if(psOnline->lTsid == lTsid){
			/*if(psOnline->login == 1){
				pasLogs(1234,1233,"login=%d", psOnline->login);
				pasLogs(1234,1233,"caName=%s", psOnline->caName);
				return 0;
			}else{
				return 2;//非在线
			}*/
			pasLogs(1234,1233,"login=%d", psOnline->login);
			pasLogs(1234,1233,"caName=%s", psOnline->caName);
			return 0;
		}
        //if(psOnline->login == 1) //已登录
           // return 0;

        psOnline = (ncPortalOnline *)pasHashNextS(&sHashInfo);
    }

    return 1;//tsid是无效的

}

int checkTsid(utShmHead *psShmHead, utPltDbHead *psDbHead, utMsgHead *psMsgHead, int iFd, char* caTsid, const char* platePath)
{
    if(strlen(caTsid) > 0)
    {
        uint8 ltsid = atoll(caTsid);
        if(isTsidLogin(psShmHead, ltsid))
        {
            utPltPutVarF(psDbHead, "result", "%d", 2);
            utPltPutVarF(psDbHead, "mesg", convert("GBK", "UTF-8", "tsid无效"));
            utPltOutToHtml(iFd, psMsgHead, psDbHead, platePath);
            return 1;
        }
    }
    else
    {
        utPltPutVarF(psDbHead, "result", "%d", 1);
        utPltPutVarF(psDbHead, "mesg", convert("GBK", "UTF-8", "tsid未长传"));
        utPltOutToHtml(iFd, psMsgHead, psDbHead, platePath);
        return 1;
    }
    return 0;
}

