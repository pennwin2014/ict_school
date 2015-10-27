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
 *�������ļ��ж�ȡ��Ӧ��ֵ
 *���������1�������ļ�·�� 2��ƥ���� 3������洢�ռ�
 *�����ų��˿��У���=��ǰ�������ݣ��ޡ�=�������
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
        if(strlen(config_linebuf) < 3) //�ж��Ƿ��ǿ���
        {
            continue;
        }
        if (config_linebuf[strlen(config_linebuf)-1] == 10) //ȥ�����һλ��\n�����
        {
            
            memset(exchange_buf,0,sizeof(exchange_buf));
            strncpy(exchange_buf,config_linebuf,strlen(config_linebuf)-1);
            memset(config_linebuf,0,sizeof(config_linebuf));
            strcpy(config_linebuf,exchange_buf);
        }
        memset(line_name,0,sizeof(line_name));
        leave_line = strstr(config_linebuf,config_sign);
        if(leave_line == NULL)                            //ȥ����"="�����
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
 *����޸��ļ����������ļ��д��ڱ���ֶΣ�������޸ģ����������������ӣ�
 *
 *���������1�������ļ�·�� 2��ƥ���� 3���滻����ӵ�����
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
        if(strlen(config_linebuf) < 3) //�ж��Ƿ��ǿ���
        {
            strcat(sum_buf,config_linebuf);
            continue;
        }
        leave_line = NULL;
        leave_line = strstr(config_linebuf,config_sign);
        if(leave_line == NULL)                            //ȥ����"="�����
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
 *ɾ�������ļ����ݣ�
 *
 *���������1�������ļ�·�� 2��ƥ���� 
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
        if(strlen(config_linebuf) < 3) //�ж��Ƿ��ǿ���
        {
            strcat(sum_buf,config_linebuf);
            continue;
        }
        leave_line = NULL;
        leave_line = strstr(config_linebuf,config_sign);
        if(leave_line == NULL)                            //ȥ����"="�����
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
	 utMsgPrintMsg(psMsgHead);  // �����ݰ���ʾ����,��������Ҫ���ڵ��ԡ���ʾʱ����ʾ���ݰ���ͷ����Ȼ����ʾ���ݰ������ݡ�
	 
	 // ��ʼ�����ݿ�
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // ��ʼ������ģ��
	 
	 char tid[10] = "";
	 char mname[64] = "";
	 char update[16] = "";
	 char caUpdate[16] = "";
	 char del[16] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 // �ؼ���
	 char _keyword[33] = "";
	 char caSesid[1024] = "";
	 
	 // ���ݱ�
	 unsigned long lCount1 = 0; // ���м�¼�ĸ���
	 long iReturn; // ����¼��ѯ
	 char start[17] = ""; // Ĭ�ϼ�¼��0��ʼ
	 char limit[17] = ""; // �޶�һҳ�еļ�¼��40��
	 unsigned long iStart = 0; // ת�����Ĭ�Ͽ�ʼatoi(start)
	 unsigned long iLimit = 0; // ת������޶���¼����atoi(limit)
	 
	 // ��ҳ
	 unsigned long lTotRec = 0; // ��¼���е����ݵĸ���
	 
	 // Ӧ��ģ��
	 char caApp[16] = "";
	 FILE *fp = NULL;
	 char caFilename[32] = "";
	 char caFile[128] = "";
	 char caPath[128] = "";
	 char caBuff[128] = "";
	 unsigned long lMaxid = 0;
	 
	 // ����
	 char caExport[32] = "";
	 
	 // ����
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar������Ϣ���ݰ���ȡָ�������ı�������ָ���ĳ��ȷ��ء�
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
	 
	 //����ת��
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,30);
			strcpy(_keyword,caTemp);
   }
	 if(strlen(mname)>0){
			pasCvtGBK(2,mname,caTemp,30);
			strcpy(mname,caTemp);
   }
	 
	 // ����/�޸�
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
	 // ɾ��
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from nctemplate where tid in(%s)",caSesid);
    	}
	 }
	 
	 /*char conf_fileName[32] = "ncwlan"; //�ļ���
	 char conf_name[40] = "MinTcpProcess"; //��Ҫ�޸ĵ�����ѡ����
	 char caCnfName[40] = "";
	 char charUpdate[128] = "";
	 char conf_value[32] = "3"; //��Ҫ�޸ĵ��ַ�������ѡ��ֵ
	 
	 // Ӧ��ģ��
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
		  // ��ģ�忽����Ӧ��Ŀ¼��
		  //system("cp /home/ncmysql/ncwlan/html/enter.html /home/ncmysql/ncwlan/plate/ncwire_sun/");
	 }*/
	 	 
	 // ��ѯ
	 strcpy(sql, "select count(*) from nctemplate where 1=1");
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
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
	 // utStrIsSpaces: �ж��ַ����Ƿ�Ϊ�ա�
	 if(!utStrIsSpaces(_keyword)) // ���ݹؼ��ֲ�ѯ
	 {
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mname like '%%%s%%'", _keyword);
	 }
	 
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur != NULL) {
	     	unsigned long tid = 0;
				char mname[64] = "";
				
				// pasDbFetchIntoȡ��ǰ��¼�����ݵ������������0---��ȷ��!=0---�������, ���庬����μ� ���ݿ���й�����
			  while(0 == (iret = pasDbFetchInto(psCur,
			  	 UT_TYPE_ULONG,  4,               		&tid,
					 UT_TYPE_STRING, sizeof(mname)-1,    mname)) || 1405 == iret)
			  {
					 iNum++;
					 if(iNum > 1)
					 {
						 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar���һ��ѭ��������ֵ��
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