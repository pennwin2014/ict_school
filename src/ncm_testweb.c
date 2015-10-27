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
#include "ncportalweb.h"
#include "ncportal.h"


#include "ncportal.h"
int ncStrToStr(char *stime, char *etime);

//楼层信息
int ncm_mytest(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  
	printf("ncmLevels start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char plateid[32] ="";
	char plateName[32] ="";
	char tsid[32] ="";
	int i = 0, iNum = 0;
	utMsgGetSomeNVar(psMsgHead, 3,
					 "tsid",        	UT_TYPE_STRING,  sizeof(tsid)-1,      	 tsid,
					 "plateid",     	UT_TYPE_STRING,  sizeof(plateid)-1,      plateid,
					 "plateName",       UT_TYPE_STRING,  sizeof(plateName)-1,    plateName
					 );

	utPltPutVarF(psDbHead,"testwebvar","%s", "HelloWord!");
  
  for(i = 0; i < 5; i++)
  {
  	  iNum ++;
  	  if (i == 0)
  	  {	
    		utPltPutLoopVar(psDbHead,"testloopvar",iNum,"HelloOne");
//    		utPltPutLoopVarF(psDbHead,"testloopvar","%s", "HelloOne");
  		}
  		else
  		{
  		  utPltPutLoopVarF(psDbHead,"testloopvar",iNum, "%s", "HelloTEST"); 	
  		}
  }
	utPltShowDb(psDbHead);

	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/testweb/ncm_testweb.htm");
	
	printf("ncmLevels  end!!!!\n");     
	return 0;	  
}


int Ncm_CeShi_7777(utShmHead *psShmHead)
{
	 int iReturn;
	 iReturn = pasSetTcpFunName("ncm_mytest",ncm_mytest,0);                         
   return 0;	
}
int ncm_mytest7799(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  
	printf("ncmLevels start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char plateid[32] ="";
	char plateName[32] ="", MytxtName[64] = "";
	char tsid[32] ="";
	int i = 0, iNum = 0;
	utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 4,
					 "tsid",        	UT_TYPE_STRING,  sizeof(tsid)-1,      	 tsid,
					 "plateid",     	UT_TYPE_STRING,  sizeof(plateid)-1,      plateid,
					 "txtName",     	UT_TYPE_STRING,  sizeof(MytxtName)-1,      MytxtName,
					 "plateName",       UT_TYPE_STRING,  sizeof(plateName)-1,    plateName
					 );
  
  strcat(MytxtName, "--Portal");
  utPltPutVarF(psDbHead,"txtName","%s", MytxtName);
	utPltPutVarF(psDbHead,"MyVar","%s", "HelloWord 7799!");
  
  for(i = 0; i < 5; i++)
  {
  	  iNum ++;
  	  if (i == 0)
  	  {	
    		utPltPutLoopVar(psDbHead,"MyLoopVar",iNum,"HelloOne");
//    		utPltPutLoopVarF(psDbHead,"testloopvar","%s", "HelloOne");
  		}
  		else
  		{
  		  utPltPutLoopVarF(psDbHead,"MyLoopVar",iNum, "%s", "HelloWord 7799"); 	
  		}
  }
	utPltShowDb(psDbHead);

	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/testweb/ncm_testweb2.htm");
	
	printf("ncmLevels  end 7799!!!!\n");     
	return 0;	  
}
int Ncm_CeShi_7799(utShmHead *psShmHead)
{
	 int iReturn;
	 iReturn = pasSetTcpFunName("ncm_mytest7799", ncm_mytest7799, 0);  /*dasdfsaf*/
	 iReturn = pasSetTcpFunName("ncm_mytest7799", ncm_mytest7799, 0);    
	                        
	                        
   return 0;	
}

