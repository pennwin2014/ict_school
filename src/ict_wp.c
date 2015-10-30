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

int ict_register(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
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
    utPltDbHead *psDbHead = utPltInitDbHead();
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
            
            return 0;
        }
        //插入数据库
        memset(sqlbuf, 0, sizeof(sqlbuf));
        snprintf(sqlbuf, sizeof(sqlbuf) - 1, "insert into userlib(vname,dname,address,passwd,mname) values('%s','%s','%s','%s','%s')",
                 caVname, caDname, caAddress, caPasswd, caMname);
        printf("insert sql=%s\n", sqlbuf);
        iReturn = pasDbExecSqlF(sqlbuf);
        if(iReturn != 0)
        {
            snprintf(caMsg, sizeof(caMsg) - 1, "操作数据库失败,错误码=[%d]", iReturn);
			utPltPutVar(psDbHead, "mesg", caMsg);
            pasDbRollback(NULL);
        }
        else
        {
            pasDbCommit(NULL);
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

int ictInitWebFun_wp(utShmHead *psShmHead)
{
    pasSetTcpFunName("ict_register", ict_register, 0);
    return 0;
}

