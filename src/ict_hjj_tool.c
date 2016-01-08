#include <stdio.h>#include <stdlib.h>#include <string.h>#include <sys/stat.h>#include <string.h>#include <locale.h>#include <unistd.h>#include <sys/types.h>#include <sys/socket.h>#include <netinet/in.h>#include <arpa/inet.h>#include <netdb.h>#include <sys/time.h>#include "utoall.h"#include "utoplt01.h"#include "pasdb.h"#include "ncportal.h"#include <iconv.h>#include "ict_hjj_tool.h"//查询是否有特定表名的表int checkTableExists(char* tableName){    char sql[1024] = "";    int ret = 0;    pasDbCursor *psCur = NULL;    snprintf(sql, sizeof(sql), "select count(*) from %s", tableName);    psCur = pasDbOpenSql(sql, 0);    if(psCur != NULL)    {        ret = 1;        pasDbCloseCursor(psCur);    }    return ret;}//得到新的表，没有则创建char* getNewLogTable(char* tableOri, char * yearMonth){    static char newTable[64] = "";    snprintf(newTable, sizeof(newTable) - 1, "%s_%s", tableOri,yearMonth);	char sql[1024] = "";	if(!checkTableExists(newTable))    {        snprintf(sql, sizeof(sql) - 1, "create table %s like %s", newTable, tableOri);        printf("not exist logTable=%s, do sql=%s\n", newTable, sql);		//执行sql语句        pasDbExecSqlF(sql);    }    return newTable;}//公共函数c文件char* getMobnoByUsername(utShmHead *psShmHead,utMsgHead *psMsgHead,int iFd,char* caUsername){	 static char caMobno[24] = {0};//用户绑定的手机号	 char caMsg[256] = {0};     int iReturn = 0;	 char sqlbuf[1024] = "";     utPltDbHead *psDbHead = utPltInitDbHead();	 //获取手机号	 memset(sqlbuf, 0, sizeof(sqlbuf));	 snprintf(sqlbuf, sizeof(sqlbuf)-1, "select mobno from ncsrvuserex where username=%s",caUsername);	 iReturn = pasDbOneRecord(sqlbuf, 0, 							 UT_TYPE_STRING, sizeof(caMobno) - 1, caMobno							 );	 printf("sqlbuf=%s, iReturn=%d\n", sqlbuf, iReturn);	 //sql语句执行成功	 if((iReturn == 0) || (1405 == iReturn))	 {		//获取手机号成功	 }	 else	 {		 snprintf(caMsg, sizeof(caMsg) - 1, "获取手机号失败!,sql语句执行不成功!");		 utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));		 utPltPutVarF(psDbHead, "result", "%d", 4);		 utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/package/add_interaction_info.htm");	 }	 return caMobno;	 }static unsigned char char_to_hex( unsigned char x ) { 	return (unsigned char)(x > 9 ? x + 55: x + 48); } static int is_alpha_number_char( unsigned char c ) { 	if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ) 	 	 return 1; 	return 0; } //url编码实现 void urlencode( unsigned char * src, int  src_len, unsigned char * dest, int  dest_len ) { 	unsigned char ch; 	int  len = 0; 	while (len < (dest_len - 4) && *src) 	{ 	  ch = (unsigned char)*src; 	  if (*src == ' ') 	  { 	   *dest++ = '+'; 	  } 	  else if (is_alpha_number_char(ch) || strchr("-_.!~*'()", ch)) 	  { 	   *dest++ = *src; 	  } 	  else 	  { 	   *dest++ = '%'; 	   *dest++ = char_to_hex( (unsigned char)(ch >> 4) ); 	   *dest++ = char_to_hex( (unsigned char)(ch % 16) ); 	  }  	  ++src; 	  ++len; 	} 	*dest = 0; 	return ; } 