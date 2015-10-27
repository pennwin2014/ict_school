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
#include <pcap.h>
#include <errno.h>
#include <iconv.h>
#include <dirent.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasutl.h"
#include "pasdb.h"
#include "ncportal.h"      
#include "ncportalweb.h"

int ncm_sms_send_register_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{	
	 utPltDbHead *psDbHead;
	 utMsgPrintMsg(psMsgHead);
//	 printf("\n ½øÈëncm_sms_send_register_list \n");
	 psDbHead = utPltInitDb();
		char getarg[512];
		char ip[512];
		char port[512];
		char tsid[512];
		char posturl[1024];
		char postarg[512];
		
		memset(getarg,0,sizeof(getarg));
		memset(ip,0,sizeof(ip));
		memset(port,0,sizeof(port));
		memset(tsid,0,sizeof(tsid));
		memset(posturl, 0, sizeof(posturl));
		memset(postarg, 0, sizeof(postarg));
		
		int iReturn;
	
		  iReturn = utMsgGetSomeNVar(psMsgHead,6,"getarg", UT_TYPE_STRING,sizeof(getarg)-1,   getarg,
	  																				 "ip",     UT_TYPE_STRING,sizeof(ip)-1,       ip,
															               "port",   UT_TYPE_STRING,sizeof(port)-1,     port,
															               "tsid",   UT_TYPE_STRING,sizeof(tsid)-1,     tsid,
																		   "posturl",UT_TYPE_STRING,sizeof(posturl)-1,posturl,
			   "postarg", UT_TYPE_STRING, sizeof(postarg)-1, postarg 
															               );
		

//		printf("iReturn: %d \n",iReturn);
		utPltPutVar(psDbHead,"getarg",getarg);
		utPltPutVar(psDbHead,"ip",ip);
		utPltPutVar(psDbHead,"port",port);
		utPltPutVar(psDbHead,"tsid",tsid);
		utPltPutVar(psDbHead,"posturl",posturl);
		utPltPutVar(psDbHead,"postarg",postarg);						
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"genal/sms/Register.html");
//		printf("\n Àë¿ªncm_sms_send_register_list \n");
}
