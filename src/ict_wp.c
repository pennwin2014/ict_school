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
#include "ncportal.h"
#include <iconv.h>

int insertToShm(char* vName)
{
	
	return 0;
}


int isVnameRegistered(char* vName)
{
	
	return 0;
}

int ict_register(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    //sql语句处理有三种，查询的话用pasDbOneRecord
    //pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);一般是只返回一个数据，将数据存在lCount中
    //insert,delete,update等用pasDbExecSqlF?
    //查询语句用 psCur = pasDbOpenSql(sql, 0);
    int iReturn = 0;
    utMsgPrintMsg(psMsgHead);
    char caVname[32 + 1] = "";
    char caDname[24 + 1] = "";
    char caPasswd[24 + 1] = "";
    char caAddress[128 + 1] = "";
    char caMname[24 + 1] = ""; //姓名
    char sqlbuf[1024] = "";
    ulong lCount = 0;
    char caMsg[256] = "";

    iReturn = utMsgGetSomeNVar(psMsgHead, 5,
                               "vname",   UT_TYPE_STRING, sizeof(caVname) - 1, caVname,
                               "dname",   UT_TYPE_STRING, sizeof(caDname) - 1, caDname,
                               "passwd",  UT_TYPE_STRING, sizeof(caPasswd) - 1, caPasswd,
                               "address",   UT_TYPE_STRING, sizeof(caAddress) - 1, caAddress,
                               "mname",   UT_TYPE_STRING, sizeof(caMname) - 1, caMname);
    utStrDelSpaces(caAddress);
    utStrDelSpaces(caVname);
    //utPltDbHead* psDbHead = utPltInitDbHead();
    utPltDbHead* psDbHead = utPltInitDb();
    if(strlen(caVname) > 0)
    {
        memset(sqlbuf, 0, sizeof(sqlbuf));
        //先判断数据库里是否有该caVname，有的话直接报错
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select count(*) from userlib where vname='%s'", caVname);
        printf("sqlbuf=%s  \n", sqlbuf);
        pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount > 0)
        {
            snprintf(caMsg, sizeof(caMsg) - 1, "该vname[%s]已经存在", caVname);
            utPltPutVar(psDbHead, "mesg", caMsg);
            utPltPutVarF(psDbHead, "result", "%d", 1);//表示已经存在该vname
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/register/register.htm");
            return 0;
        }
        //插入数据库
        memset(sqlbuf, 0, sizeof(sqlbuf));
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "insert into userlib (vname,dname,address,passwd,mname) values('%s','%s','%s','%s','%s')",
                 caVname, caDname, caAddress, caPasswd, caMname);
        /*snprintf(sqlbuf, sizeof(sqlbuf) - 1, "insert into userlib (vname,dname,address,passwd,mname) values('%s','%s','%s','%s','%s')",
                utf8convert(psShmHead,caVname), utf8convert(psShmHead,caDname), utf8convert(psShmHead,caPasswd), utf8convert(psShmHead,caAddress), utf8convert(psShmHead,caMname));   */
        //snprintf(sql,sizeof(sql),"insert into help (messageTitle,messageContent)values('%s','%s')",utf8convert(psShmHead,messageTitle),utf8convert(psShmHead,messageContent));
        printf("insert sql=%s\n", sqlbuf);
        iReturn = pasDbExecSqlF(sqlbuf);
        //pasDbExecSql(sqlbuf, 0);
        printf("iReturn=%d\n", iReturn);
        if(iReturn != 0)
        {
            snprintf(caMsg, sizeof(caMsg) - 1, "操作数据库失败,错误码=[%d]", iReturn);
            utPltPutVar(psDbHead, "mesg", caMsg);
            //sql语句操作回滚
            //pasDbRollback(NULL);
        }
        else
        {
           // pasDbCommit(NULL);
        }
        utPltPutVarF(psDbHead, "result", "%d", iReturn);
    }
    else
    {
        snprintf(caMsg, sizeof(caMsg) - 1, "vname不可以为空");
        utPltPutVar(psDbHead, "mesg", caMsg);
        utPltPutVarF(psDbHead, "result", "%d", 2);
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/register/register.htm");
    return 0;
}

int ict_Auth(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    int iReturn = 0;
    utMsgPrintMsg(psMsgHead);
    char caVname[32 + 1] = "";
    char caPasswd[24 + 1] = "";
    char sqlbuf[1024] = "";
    ulong lCount = 0;
    char caMsg[256] = "";
	

    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "vname",   UT_TYPE_STRING, sizeof(caVname) - 1, caVname,
                               "passwd",  UT_TYPE_STRING, sizeof(caPasswd) - 1, caPasswd);
    utStrDelSpaces(caVname);
    utPltDbHead* psDbHead = utPltInitDb();
    if(strlen(caVname) > 0)
    {
        memset(sqlbuf, 0, sizeof(sqlbuf));
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select count(*) from userlib where vname='%s' and passwd='%s'", caVname, caPasswd);
        printf("sqlbuf=%s  \n", sqlbuf);
        pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);
        if(lCount <= 0)
        {
            snprintf(caMsg, sizeof(caMsg) - 1, "该vname[%s]不存在或者密码错误", caVname);
            utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
            utPltPutVarF(psDbHead, "result", "%d", 1);
        }else{
			utPltPutVarF(psDbHead, "result", "%d", 0);
			//登录成功，往共享内存插入一条数据
			insertToShm(caVname);
		}
    }
    else
    {
        snprintf(caMsg, sizeof(caMsg) - 1, "vname不可以为空");
        utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
        utPltPutVarF(psDbHead, "result", "%d", 2);
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/login/login.htm");
    return 0;
}


int ictInitWebFun_wp(utShmHead *psShmHead)
{
    pasSetTcpFunName("ict_register", ict_register, 0);
	pasSetTcpFunName("ict_Auth", ict_Auth, 0);
    return 0;
}

