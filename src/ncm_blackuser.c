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
#include <iconv.h>

int ncTimeToLong(char *stime, unsigned long *lTime);
/*{
	 char ctime[20] = "";
	 
	 snprintf(ctime, sizeof(ctime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", stime[0], stime[1], stime[2], stime[3], stime[5], stime[6], stime[8], stime[9], stime[11], stime[12], stime[14], stime[15], stime[17], stime[18]);	
	 *lTime = utTimStrToLong("%Y%m%d%H%M%S", ctime);
	 
	 return 0;
}*/

int ncValidToLong(unsigned long lTime, int days, unsigned long *lStartTime)
{
		char sdate[33] = "";
		strcpy(sdate, utTimFormat("%Y-%m-%d", lTime));//当前日期
   	strcat(sdate, " 00:00:00");
   	*lStartTime = utTimStrToLong("%Y/%m/%d %H:%M:%S", sdate) + (days-1) * 86400;
   	
   	return 0;	
}

int ncm_blackuser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char utype[32] = "";
	 char groupid[32] = "";
	 char name[32] = "";
	 char memo[64] = "";
	 char validtime[64] = "";
	 char caPreid[32] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long	lUtype = 0;
	 unsigned long	lGroupid = 0;
   char 					caName[32] = "";
	 char 					caMemo[64] = "";
	 unsigned long  lValidtime = 0;
	 char           s_validtime[32] = "";
	 char           s_utype[32] = "";
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caSesid[1024] = "";
	 char caTemp[4096] = "";
	 char caTemp1[4096] = "";
	 
	 // 关键字
	 char _keyword[33] = "";
	 
	 // 数据表
	 unsigned long lCount1 = 0; // 表中记录的个数
	 unsigned long lCount2 = 0; // 表中记录的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 unsigned long lValidTime = 0;
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 17,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
																	"utype",       UT_TYPE_STRING,  sizeof(utype)-1,          utype,
																	"groupid",     UT_TYPE_STRING,  sizeof(groupid)-1,        groupid,
																	"name",        UT_TYPE_STRING,  sizeof(name)-1,       		name,
																	"memo",        UT_TYPE_STRING,  sizeof(memo)-1,           memo,
																	"validtime",   UT_TYPE_STRING,  sizeof(validtime)-1,      validtime,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"update",      UT_TYPE_STRING,  10,                       caUpdate,
																	"preid",       UT_TYPE_STRING,  sizeof(caPreid)-1,        caPreid,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport);
	 
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
	 if(strlen(name)>0){
			pasCvtGBK(2,name,caTemp,30);
			strcpy(name,caTemp);
   }
   if(strlen(memo)>0){
			pasCvtGBK(2,memo,caTemp,30);
			strcpy(memo,caTemp);
   }
   if(strlen(caSesid)>0){
			pasCvtGBK(2,caSesid,caTemp,30);
			strcpy(caSesid,caTemp);
   }
   if(strlen(caPreid)>0){
			pasCvtGBK(2,caPreid,caTemp,30);
			strcpy(caPreid,caTemp);
   }
   if(strlen(sort)>0){
			pasCvtGBK(2,sort,caTemp,30);
			strcpy(sort,caTemp);
   }
   
   // 排序
   if(strlen(sort)==0){
	 	  strcpy(sort,"name");
	 }
	 
	 // 时间转换
	 if(strlen(validtime)==0){
	 		lValidTime = 0;	
	 }else{
	 		ncTimeToLong(validtime, &lValidTime);
	 }
	 
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
			if(strlen(caPreid)>0){
				  sprintf(caTemp,"update ncsrvblackuser set utype=%lu,groupid=%lu,name='%s',memo='%s',validtime=%lu where name='%s' ",atol(utype),atol(groupid),name,memo,lValidTime,caPreid);
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  } else {
		    	sprintf(caTemp,"insert into ncsrvblackuser (utype,groupid,name,memo,validtime) values (%lu,%lu,'%s','%s',%lu)",atol(utype),atol(groupid),name,memo,lValidTime);
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    			printf("caSesid = %s\n", caSesid);
    		  pasDbExecSqlF("delete from ncsrvblackuser where name in(%s)",caSesid);
    	}
	 }	 
	 // 查询
	 strcpy(sql, "select count(*) from ncsrvblackuser where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and name like '%%%s%%' ", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select utype, groupid, name, memo, validtime from ncsrvblackuser where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and name like '%%%s%%' ", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),        &lUtype,
	                             UT_TYPE_ULONG,  sizeof(long),        &lGroupid,
															 UT_TYPE_STRING, sizeof(caName)-1,    caName,
															 UT_TYPE_STRING, sizeof(caMemo)-1,    caMemo,
															 UT_TYPE_ULONG,  sizeof(long),        &lValidtime);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 
			 switch(lUtype){
			 		case 1:strcpy(s_utype, "Mac");break;
			 		case 2:strcpy(s_utype, "Username");break;
			 		default:break;
			 }
		   
		   if(lValidtime>0){
		   		strcpy(s_validtime, utTimFormat("%Y/%m/%d", lValidtime));
		   }else{
		   		strcpy(s_validtime, "");	
		   }
		   
		   utPltPutLoopVarF(psDbHead,"utype",iNum,"%lu",lUtype);
			 utPltPutLoopVar(psDbHead,"s_utype",iNum,s_utype);
			 utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
			 utPltPutLoopVar(psDbHead,"name",iNum,caName);
			 utPltPutLoopVar(psDbHead,"memo",iNum,caMemo);
			 utPltPutLoopVar(psDbHead,"validtime",iNum,s_validtime);
			 
			 lUtype = 0;
			 lGroupid = 0;
			 memset(caName, 0, sizeof(caName));
			 memset(caMemo, 0, sizeof(caMemo));
			 lValidtime = 0;
			 memset(s_validtime, 0, sizeof(s_validtime));
			 memset(s_utype, 0, sizeof(s_utype));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),        &lUtype,
			                             UT_TYPE_ULONG,  sizeof(long),        &lGroupid,
																	 UT_TYPE_STRING, sizeof(caName)-1,    caName,
																	 UT_TYPE_STRING, sizeof(caMemo)-1,    caMemo,
																	 UT_TYPE_ULONG,  sizeof(long),        &lValidtime);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmgroup/ncm_blackuser_list.htm");
	  return 0;	
}

int ncm_webblackdr_next(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
		pasDbCursor *psCur;
		char caTemp[16000];
		FILE *fp;
		int iNum=0;
		utPltDbHead *psDbHead;
		psDbHead = utPltInitDb();
		
		char imp_file[129]="";
		char temp_file[129]="";
		char sqlbuf[1024]="";
		char caBuf[16002];
		unsigned long lcount = 0;
		char *p, *pBuf;
		
		char caUtype[20] = "";
		char caName[32] = "";
		char caMemo[64] = "";
		unsigned long lValidtime = 0;
		
		unsigned long lTime = time(0); // 当前时间
		
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

		if(!utFileIsExist("/home/ncmysql/ncsrv/upload")) {
		    if(mkdir("/home/ncmysql/ncsrv/upload",777)!=0){
				    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入","打开文件出错");
				    return 0;
		    }
		}
		system("chmod -Rf 777 /home/ncmysql/ncsrv/upload");

		utMsgGetSomeNVar(psMsgHead,1,"fname",  UT_TYPE_STRING, sizeof(imp_file)-1, imp_file);
		
		if(strlen(imp_file)!=0){
				p=utStrSkipSpaces(imp_file);
				p=utStrGetWord(p,temp_file,128,";\n");
				if((*p)==';')
				p=utStrGetWord(p+1,temp_file,128,";\n");
				//strcpy(temp_file, "/home/ncmysql/ncsrv/html/v8/black1.csv");
				fp=fopen(temp_file,"r");
				printf("temp_file=%s\n",temp_file);
				if(fp == NULL) {
						utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入","打开文件出错");
						return 0;
				} 
		 	  
				p = fgets(caBuf,16000,fp);  // 忽略第一行
				//printf("caBuf = %s \n",caBuf);
				p = fgets(caBuf,16000,fp);
				//printf("caBuf1 = %s \n",caBuf);
				while(p){
				    pBuf = utStrGetWord(p,caTemp,16000,"\r\n");
		        strcpy(pBuf,caTemp);
		        strcat(pBuf,",");
		        //printf("pBuf = %s \n",pBuf);
		        memset(caUtype,0,sizeof(caUtype));
		        memset(caName,0,sizeof(caName));
		        memset(caMemo,0,sizeof(caMemo));
		        
		        while(pBuf){
		             iNum++;
		             pBuf = utStrGetWord(pBuf,caTemp,16000,",");
		             //printf("caTemp = %s \n",caTemp);
		             //printf("iNum = %d \n",iNum);
		             if(iNum==1){
		                strcpy(caUtype, caTemp);
		                //printf("caUtype = %s\n", caUtype);
		             }
		             else if(iNum==2){
		                strcpy(caName,caTemp);
		                //printf("caName = %s\n", caName);
		             }
		             else if(iNum==3){
		                strcpy(caMemo,caTemp);
		                //printf("caMemo = %s\n", caMemo);
		             }
		             
				         if(*pBuf != ',') {
				             break;
				         }
				         pBuf++;
		        }
		        
		        ncValidToLong(lTime, 90, &lValidtime);
		        
		        sprintf(sqlbuf, "select count(*) from ncsrvblackuser where name='%s'", caName);
						pasDbOneRecord(sqlbuf,0,UT_TYPE_ULONG,sizeof(long),&lcount);
						//printf("lcount = %d\n", lcount);
						if(lcount==0){
								sprintf(sqlbuf, "insert into ncsrvblackuser (utype, name, memo, validtime) values (%lu, '%s', '%s', %lu)", atol(caUtype), caName, caMemo, lValidtime);	
								//printf("sqlbuf = %s\n", sqlbuf);
								pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
								pasDbCommit(NULL);
						}else{
								sprintf(sqlbuf, "update ncsrvblackuser set utype=%lu, memo='%s', validtime=%lu where name='%s'", atol(caUtype), caMemo, lValidtime, caName);
								//printf("sqlbuf = %s\n", sqlbuf);
								pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
								pasDbCommit(NULL);	
						}
						
						iNum=0;
						p = fgets(caBuf,16000,fp);
				}
				fclose(fp);
		}
		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入完成!","导入完成!");  
		return 0;
}
