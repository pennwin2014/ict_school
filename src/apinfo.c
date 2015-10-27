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
#include "dsclt.h"
#include "dlfcn.h"
#include "ncportal.h"
#include <iconv.h>


////////////////////////////////////////////////////////////
//功能：
//		utf8转gb2312,在有中文的地方使用
//参数:
//      inbuf:输入缓冲区,	inlen:输入长度
//      outbuf:输出缓冲区,	outmaxlen:输出最大长度
//返回: 
//		-1为失败，非-1为成功，并取出输出的长度，
////////////////////////////////////////////////////////////
int utf8_to_gb2312(char *inbuf, int inlen, char *outbuf, int outmaxlen)     
{           
	iconv_t cd = iconv_open("gb2312", "utf-8"); 
	if (cd == NULL)
    {
		printf("error: iconv_open \n");
        return -1;
    }
	memset(outbuf, 0, outmaxlen);
    if(iconv(cd, (char **)&inbuf, (size_t *)&inlen, (char **)&outbuf, (size_t *)&outmaxlen) == -1)
	{
		printf("error: iconv \n");
		return -1;
	}
    iconv_close(cd);

	return outmaxlen;    
} 

int query_ap_info(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	int iReturn;
	char sql[1024];
	memset(sql, 0, sizeof(sql));
	utMsgPrintMsg(psMsgHead); 
	utPltDbHead *psDbHead = utPltInitDbHead();

	char status[2] = "";
	char mac[32] = "";
	char pos[128] = "";
	char pos_16[128] = "";




	utMsgGetSomeNVar(psMsgHead, 3,
								"status",  UT_TYPE_STRING,  sizeof(status) - 1,	status,
								"mac",    UT_TYPE_STRING,  sizeof(mac) - 1,	mac,
								"pos",    UT_TYPE_STRING,	sizeof(pos) - 1,		pos_16
					);
					
		utf8_to_gb2312(pos_16,strlen(pos_16) , pos , sizeof(pos));
		printf(pos);

	if(status[0] == 'a')		//增加记录
	{
		sprintf(sql, "INSERT INTO portalapinfo (mac, pos) VALUES ('%s', '%s')", mac, pos);
		iReturn = pasDbExecSql(sql, 0);
		if(iReturn == 0)
			utPltPutVar(psDbHead, "result", "OK");
		else
			utPltPutVar(psDbHead, "result", "INSERT");
		utPltOutToHtml(iFd, psMsgHead, psDbHead, "common/result.htm");

	}
	else if(status[0] == 'u')	//修改记录
	{
		sprintf(sql, "UPDATE portalapinfo SET pos = '%s' WHERE mac = '%s'", pos  , mac);
		printf("sql is : %s " , sql);
		iReturn = pasDbExecSql(sql, 0);
		if(iReturn == 0)
			utPltPutVar(psDbHead, "result", "OK");
		else
			utPltPutVar(psDbHead, "result", "UPDATE");
		utPltOutToHtml(iFd, psMsgHead, psDbHead, "common/result.htm");
	}
	else if(status[0] == 'd')	//删除记录
	{
		sprintf(sql, "DELETE FROM portalapinfo WHERE mac = '%s'", mac);
		iReturn = pasDbExecSql(sql, 0);
		if(iReturn == 0)
			utPltPutVar(psDbHead, "result", "OK");
		else
			utPltPutVar(psDbHead, "result", "DELETE");
		utPltOutToHtml(iFd, psMsgHead, psDbHead, "common/result.htm");
	}
	else						//查询记录
	{
		sprintf(sql, "SELECT mac, pos FROM portalapinfo");

		char description[128];
		int row = 1;
		//-----------------------------------------------------------
		pasDbCursor *psCur = pasDbOpenSql(sql, 0);
		if(psCur == NULL)
		{
				utPltPutVar(psDbHead, "result", "QUERY");
				utPltOutToHtml(iFd, psMsgHead, psDbHead, "common/result.htm");
				return;
		}
		int iret = pasDbFetchInto(psCur, 
					UT_TYPE_STRING,	sizeof(mac) - 1,	mac,
					UT_TYPE_STRING, sizeof(pos) - 1,	pos);
		while(0 == iret || 1405 == iret)
		{
			

				if(row > 1)
				{
					utPltPutLoopVar(psDbHead,"dh", row, ",");
				}
				//utPltSetCvtHtml(1);
				utPltPutLoopVar(psDbHead,"mac", row, mac);
				utPltPutLoopVar(psDbHead, "pos", row, pos);
				row++;
				iret = pasDbFetchInto(psCur, 
							UT_TYPE_STRING,	sizeof(mac) - 1,	mac,
							UT_TYPE_STRING, sizeof(pos) - 1,	pos);
		}
		pasDbCloseCursor(psCur);
		utPltOutToHtml(iFd, psMsgHead, psDbHead, "common/apinfo.htm");
	}

	return 0;
}