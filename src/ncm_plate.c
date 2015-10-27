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

/*
 *从配置文件中读取相应的值
 *输入参数：1，配置文件路径 2，匹配标记 3，输出存储空间
 *并且排除了空行，“=”前后无内容，无“=”的情况
 */
ReadConfig(char *conf_path,char *conf_name,char *config_buff)
{
    char config_linebuf[256];
    char line_name[40];
    char exchange_buf[256];
    char *config_sign = "=";
    char *leave_line;
    FILE *f;
    f = fopen(conf_path,"r");
    if(f == NULL)
    {
        printf("OPEN CONFIG FALID\n");
        return 0;
    }
    fseek(f,0,SEEK_SET); 
    while(fgets(config_linebuf,256,f) != NULL)
    {   
        if(strlen(config_linebuf) < 3) //判断是否是空行
        {
            continue;
        }
        if (config_linebuf[strlen(config_linebuf)-1] == 10) //去除最后一位是\n的情况
        {
            
            memset(exchange_buf,0,sizeof(exchange_buf));
            strncpy(exchange_buf,config_linebuf,strlen(config_linebuf)-1);
            memset(config_linebuf,0,sizeof(config_linebuf));
            strcpy(config_linebuf,exchange_buf);
        }
        memset(line_name,0,sizeof(line_name));
        leave_line = strstr(config_linebuf,config_sign);
        if(leave_line == NULL)                            //去除无"="的情况
        {
            continue;
        }
        int leave_num = leave_line - config_linebuf;
        strncpy(line_name,config_linebuf,leave_num);
        if(strcmp(line_name,conf_name) ==0)
        {
            strncpy(config_buff,config_linebuf+(leave_num+1),strlen(config_linebuf)-leave_num-1);
            break;
        }
        if(fgetc(f)==EOF)
        {
            break;  
        }
        fseek(f,-1,SEEK_CUR);
        memset(config_linebuf,0,sizeof(config_linebuf));
    }
    fclose(f);
}

/*
 *添加修改文件（当配置文件中存在标记字段，则进行修改，若不存在则进行添加）
 *
 *输入参数：1，配置文件路径 2，匹配标记 3，替换或添加的内容
 *
 */
AddOrAltConfig(char *conf_path,char *conf_name,char *config_buff)
{
    char config_linebuf[256];
    char line_name[40];
    char *config_sign = "=";
    char *leave_line;
    int  alter_sign = 0;
    
    FILE *f;
    f = fopen(conf_path,"r+");
    if(f == NULL)
    {
        printf("OPEN CONFIG FALID\n");
        return 0;
    }
    fseek(f,0,SEEK_END);
    long congig_lenth = ftell(f);
    int configbuf_lenth = strlen(config_buff);
    configbuf_lenth = configbuf_lenth + 5;
    char sum_buf[congig_lenth+configbuf_lenth];
    memset(sum_buf,0,sizeof(sum_buf));
    fseek(f,0,SEEK_SET); 
    while(fgets(config_linebuf,256,f) != NULL)
    {   
        if(strlen(config_linebuf) < 3) //判断是否是空行
        {
            strcat(sum_buf,config_linebuf);
            continue;
        }
        leave_line = NULL;
        leave_line = strstr(config_linebuf,config_sign);
        if(leave_line == NULL)                            //去除无"="的情况
        {
            strcat(sum_buf,config_linebuf);
            continue;
        }
        int leave_num = leave_line - config_linebuf;
        memset(line_name,0,sizeof(line_name));
        strncpy(line_name,config_linebuf,leave_num);
        if(strcmp(line_name,conf_name) ==0)
        {
            strcat(sum_buf,config_buff);
            strcat(sum_buf,"\n");
            alter_sign = 1;
        }
        else
        {
            strcat(sum_buf,config_linebuf);
        }
        if(fgetc(f)==EOF)
        {
            break;  
        }
        fseek(f,-1,SEEK_CUR);
        memset(config_linebuf,0,sizeof(config_linebuf));
    }
    if(alter_sign == 0)
    {
        strcat(sum_buf,config_buff);
        strcat(sum_buf,"\n");
    }
    printf("---sum_buf---->%s<----------\n",sum_buf);
    remove(conf_path);
    fclose(f);
    FILE *fp;
    fp = fopen(conf_path,"w+");
    if(fp == NULL)
    {
        printf("OPEN CONFIG FALID\n");
        return 2;
    }
    fseek(fp,0,SEEK_SET);
    fputs(sum_buf,fp);
    fclose(fp);
}

/*
 *删除配置文件内容（
 *
 *输入参数：1，配置文件路径 2，匹配标记 
 *
 */
DeleteConfig(char *conf_path,char *conf_name)
{
    
    char config_linebuf[256];
    char line_name[40];
    char *config_sign = "=";
    char *leave_line;
    
    FILE *f;
    f = fopen(conf_path,"r+");
    if(f == NULL)
    {
        printf("OPEN CONFIG FALID\n");
        return 0;
    }
    fseek(f,0,SEEK_END);
    long congig_lenth = ftell(f);
    char sum_buf[congig_lenth+2];
    memset(sum_buf,0,sizeof(sum_buf));
    fseek(f,0,SEEK_SET); 
    while(fgets(config_linebuf,256,f) != NULL)
    {   
        if(strlen(config_linebuf) < 3) //判断是否是空行
        {
            strcat(sum_buf,config_linebuf);
            continue;
        }
        leave_line = NULL;
        leave_line = strstr(config_linebuf,config_sign);
        if(leave_line == NULL)                            //去除无"="的情况
        {
            strcat(sum_buf,config_linebuf);
            continue;
        }
        int leave_num = leave_line - config_linebuf;
        memset(line_name,0,sizeof(line_name));
        strncpy(line_name,config_linebuf,leave_num);
        if(strcmp(line_name,conf_name) ==0)
        {
            
        }
        else
        {
            strcat(sum_buf,config_linebuf);
        }
        
        if(fgetc(f)==EOF)
        {
            break;  
        }
        fseek(f,-1,SEEK_CUR);
        memset(config_linebuf,0,sizeof(config_linebuf));
    }
    printf("---sum_buf---->%s<----------\n",sum_buf);
    remove(conf_path);
    fclose(f);
    FILE *fp;
    fp = fopen(conf_path,"w+");
    if(fp == NULL)
    {
        printf("OPEN CONFIG FALID\n");
        return 2;
    }
    fseek(fp,0,SEEK_SET);
    fputs(sum_buf,fp);
    fclose(fp);
}

int ncm_WebShowModuleApp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{  
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char tid[10] = "";
	 char mname[64] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 // 关键字
	 char _keyword[33] = "";
	 char caSesid[1024] = "";
	 
	 // 数据表
	 unsigned long lCount1 = 0; // 表中记录的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 应用模板
	 char caApp[16] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 char caBuff[128] = "";
	 unsigned long lMaxid = 0;
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 16,
	    "keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
	    "tid",         UT_TYPE_STRING,  sizeof(tid)-1,            tid,
			"mname",       UT_TYPE_STRING,  sizeof(mname)-1,          mname,
			"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
	   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
	   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
	   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
			"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
			"app",				 UT_TYPE_STRING,  10,                       caApp,
			"del",				 UT_TYPE_STRING,  10,                       caDel,
			"update",      UT_TYPE_STRING,  10,                       caUpdate,
			"caExport",    UT_TYPE_STRING,  10,                       caExport
	 );
	 if(strlen(sort)==0){
	 	  strcpy(sort,"tid");
	 }
	 trim(_keyword);
	 printf("tid = %s\n", tid);
	 printf("mname = %s\n", mname);
	 printf("del = %s\n", caDel);
	 printf("update = %s\n", caUpdate);
	 printf("_keyword = %s\n", _keyword);
	 printf("start = %s\n", start);
	 printf("limit = %s\n", limit);
	 printf("sort1 = %s\n", sort);
	 printf("dir = %s\n", dir);
	 printf("caExport = %s\n", caExport);
	 printf("caApp = %s\n", caApp);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
	 if(strlen(mname)>0){
			pasCvtGBK(2,mname,caTemp,30);
			strcpy(mname,caTemp);
   }
	 
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
	 		if (atol(tid)>0){
	 				sprintf(caTemp,"update nctemplate set mname='%s' where tid=%lu ",mname,atol(tid));
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  } else {
	 	  		pasDbOneRecord("select max(tid) from nctemplate", 0, UT_TYPE_ULONG, 4, &lMaxid);
		    	sprintf(caTemp,"insert into nctemplate (tid,mname) values (%lu,'%s')",(lMaxid+1),mname);
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from nctemplate where tid in(%s)",caSesid);
    	}
	 }
	 
	 /*char conf_fileName[32] = "ncwlan"; //文件名
	 char conf_name[40] = "MinTcpProcess"; //需要修改的配置选项名
	 char caCnfName[40] = "";
	 char charUpdate[128] = "";
	 char conf_value[32] = "3"; //需要修改的字符串配置选项值
	 
	 // 应用模板
	 if(!utStrIsSpaces(caApp)){
	 	  if(strlen(tid)>0){
	 	  		sprintf(caFile, "/home/ncmysql/ncwlan/bin/%s.cnf", conf_fileName);
	 	  		printf("caFile = %s\n", caFile);
	 	  		sprintf(caCnfName, "%s ", conf_name),
	 	  		ReadConfig(caFile, caCnfName, caBuff);
			 		printf("caBuff = %s\n", caBuff);
					sprintf(charUpdate, "%s= %s", caCnfName, conf_value);
			 		printf("charUpdate = %s\n", charUpdate);
			 		//AddOrAltConfig(caFile, caCnfName, charUpdate);
			 		//DeleteConfig(caFile, caCnfName);
		  }
		  // 将模板拷贝到应用目录下
		  //system("cp /home/ncmysql/ncwlan/html/enter.html /home/ncmysql/ncwlan/plate/ncwire_sun/");
	 }*/
	 	 
	 // 查询
	 strcpy(sql, "select count(*) from nctemplate where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mname like '%%%s%%'", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select tid, mname from nctemplate where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mname like '%%%s%%'", _keyword);
	 }
	 
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL) {
	     	unsigned long tid = 0;
				char mname[64] = "";
				
				// pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
			  while(0 == (iret = pasDbFetchInto(psCur,
			  	 UT_TYPE_ULONG,  4,               		&tid,
					 UT_TYPE_STRING, sizeof(mname)-1,    mname)) || 1405 == iret)
			  {
					 iNum++;
					 if(iNum > 1)
					 {
						 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
					 }
				   
				   utPltPutLoopVarF(psDbHead,"tid",iNum,"%lu",tid);
					 utPltPutLoopVar(psDbHead,"mname",iNum,mname);
			  }
		}
		utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_plate_list.htm");
	  return 0;	
}