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
#include "pasutl.h"
#include "pasdb.h"
#include "pasutl.h"
#include "utoplt01.h"
#include "ncportal.h"
#include "ncportalweb.h"
#include <iconv.h>
#define SHOPIMGPATH "/home/ncmysql/ncsrv/html"
#define UPLOADIMGPATH "/home/ncmysql/ncsrv/upload"
int ncmAddTest(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{	int iReturn;
	utMsgPrintMsg(psMsgHead);
	utPltDbHead *psDbHead;
	printf("���ܳɹ�0\n");
	psDbHead = utPltInitDb();
	printf("���ܳɹ�1\n");
	char firNum[32]="";
	printf("���ܳɹ�2\n");
	char secNum[32]="";
	char pname[32]="";
	char page[32]="";
	char paddress[32]="";
	printf("���ܳɹ�3\n");
printf("���ܳɹ�3\n");
utMsgGetSomeNVar(psMsgHead, 5, "firNum",     UT_TYPE_STRING, sizeof(firNum)-1,     firNum,
                               "secNum",      UT_TYPE_STRING, sizeof(secNum)-1,      secNum,
                               "pname",     UT_TYPE_STRING, sizeof(pname)-1,     	pname,
                               "page",     UT_TYPE_STRING, sizeof(page)-1,     		page,
                               "paddress",     UT_TYPE_STRING, sizeof(paddress)-1,     paddress);
printf("%s \n",pname);
printf("%s \n",page);
printf("%s \n",paddress);
int FirN,SecN;
FirN = atol(firNum);
printf("���ܳɹ� %s \n",firNum); 
SecN = atol(secNum);                   
printf("���ܳɹ� %s \n",secNum);
	int Thre = FirN+SecN;
	printf("���ܳɹ� %d \n",Thre); 
	utPltPutVarF(psDbHead,"pname","%s",pname);
	utPltPutVarF(psDbHead,"page","%s",page);
	utPltPutVarF(psDbHead,"paddress","%s",paddress);
	utPltPutVarF(psDbHead,"Thre", "%d" , Thre);
	printf("���ܳɹ�2 %d \n",Thre); 
	utPltOutToHtml(iFd, psMsgHead, psDbHead, "portal/common/comment_list.htm");
 }