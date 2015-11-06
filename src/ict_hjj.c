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

#define SERVICE_INFO 1





int ict_login(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)

{

  //sql��䴦�������֣���ѯ�Ļ���pasDbOneRecord

  //pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);һ����ֻ����һ�����ݣ������ݴ���lCount��

  //insert,delete,update����pasDbExecSqlF�

  //��ѯ����� psCur = pasDbOpenSql(sql, 0);

  int iReturn = 0;

  utMsgPrintMsg(psMsgHead);

  char caVname[32 + 1] = "";

  char caDname[24 + 1] = "";

  char caPasswd[24 + 1] = "";

  char caAddress[128 + 1] = "";

  char caMname[24 + 1] = ""; //����

  char caPackage[32 + 1] = "";

  char sqlbuf[1024] = "";

  ulong lCount = 0;

  char caMsg[256] = "";

  int caMoney = 0;



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

    char sql[1024] = "";

    //char comp_id_group[128] = "";

    pasDbCursor *psCur = NULL;



    utPltDbHead *psDbHead = utPltInitDb();

    int db_count = 0;



    snprintf(sql, sizeof(sql), "select vname,mname,package,money from userlib where vname='%s' and passwd='%s'", caVname, caPasswd);

    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_STRING, sizeof(caVname) - 1, caVname,

                             UT_TYPE_STRING, sizeof(caMname) - 1, caMname,

                             UT_TYPE_STRING, sizeof(caPackage) - 1, caPackage,

                             UT_TYPE_LONG, 4, &caMoney);

    printf("sql=%s, iReturn=%d\n", sql, iReturn);

    if((iReturn == 0) || (1405 == iReturn))

    {

      snprintf(caMsg, sizeof(caMsg) - 1, "success", caVname);

      //snprintf(caMsg, sizeof(caMsg) - 1, "��¼�ɹ�", caVname);

      utPltPutVar(psDbHead, "mesg", caMsg);

      utPltPutVarF(psDbHead, "result", "%d", 0);

      utPltPutVar(psDbHead, "vname", caVname);
      utPltPutVar(psDbHead, "mname", caMname);
      utPltPutVar(psDbHead, "package", caPackage);
      utPltPutVarF(psDbHead, "money", "%d", caMoney);

    }

    else

    {

      //snprintf(caMsg, sizeof(caMsg) - 1, "��¼ʧ��", caVname);

      snprintf(caMsg, sizeof(caMsg) - 1, "fail", caVname);

      utPltPutVar(psDbHead, "mesg", caMsg);

      utPltPutVarF(psDbHead, "result", "%d", 1);

      utPltPutVar(psDbHead, "vname", caVname);

      utPltPutVar(psDbHead, "mname", caMname);

      utPltPutVar(psDbHead, "package", caPackage);

      utPltPutVarF(psDbHead, "money", "%d", caMoney);

    }

    //��һ����Ҫ���

    //writeLog("compute.txt","service",sql);



    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/login/login.htm");

  }

  return 0;

}









int ict_show_package(utShmHead * psShmHead, int iFd, utMsgHead * psMsgHead)

{

#if SERVICE_INFO

  utMsgPrintMsg(psMsgHead);

#endif

  char caName[32 + 1] = "";

  char caNamedes[128 + 1] = "";

  uint4 caPtype = 0;

  uint4 caMoney = 0;

  /*

  char comp_id_in[16] = "";

  char group_id_in[16] = "";

  char keywords_in[64] = "";

  char serverName_in[74] = "";

  char deal_in[2] = "";

  */

  //������ҳ�ģ�start_in��limit_in

  char start_in[8] = "";

  char limit_in[8] = "";

  //ȡǰ̨�Ĳ�������ֵ������������������Ϊ���ݿ��ѯ������ǰ̨���̨ר�����ݵķ�ʽһ��Ϊquerystring�ķ�ʽ������д������Ļ����ͽ�����������Ӧ��ֵ

  utMsgGetSomeNVar(psMsgHead, 6,

                   "name", UT_TYPE_STRING,  sizeof(caName) - 1,  caName,

                   "namedes",  UT_TYPE_STRING,  sizeof(caNamedes) - 1,   caNamedes,

                   "ptype",    UT_TYPE_ULONG,  sizeof(caPtype),    &caPtype,

                   "money",    UT_TYPE_ULONG,  sizeof(caMoney),    &caMoney,

                   "start",    UT_TYPE_STRING,  sizeof(start_in) - 1,    start_in,

                   "limit",        UT_TYPE_STRING,      sizeof(limit_in) - 1,    limit_in);





  char sql[1024] = "";

  char sql_tmp[1024] = "";

  //char comp_id_group[128] = "";

  pasDbCursor *psCur = NULL;



  utPltDbHead *psDbHead = utPltInitDb();

  int db_count = 0;



  snprintf(sql, sizeof(sql), "select count(*) from package");

  printf("sqlbuf=%s  \n", sql);

  //��һ����Ҫ���

  //writeLog("compute.txt","service",sql);



  psCur = pasDbOpenSql(sql, 0);

  pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &db_count);

  pasDbCloseCursor(psCur);

  utPltPutVarF(psDbHead, "TotRec", "%lu", db_count);



  snprintf(sql, sizeof(sql), "select name,namedes,money from package where 1=1 limit %u,%u", atoi(start_in), atoi(limit_in));

  printf("sqlbuf=%s  \n", sql);

#if SERVICE_INFO

  printf("%s\n", sql);

#endif



  psCur = pasDbOpenSql(sql, 0);



  if(psCur != NULL)

  {

    char name[32 + 1] = "";

    char namedes[128 + 1] = "";

    uint4 ptype = 0;

    uint4 money = 0;
    int iret = -1;

    int iNum = 0;

    //mark

    while(0 == (iret = pasDbFetchInto(psCur,

                                      UT_TYPE_STRING, sizeof(name) - 1, name,

                                      UT_TYPE_STRING, sizeof(namedes) - 1, namedes,

                                      UT_TYPE_ULONG, 4, &ptype,

                                      UT_TYPE_ULONG, 4, &money)) || 1405 == iret)

    {

      //printf("sid=%llu,compid=%lu", sid, compid);

      iNum++;

      if(iNum > 1)

      {

        utPltPutLoopVar(psDbHead, "dh", iNum, ",");

      }

      utPltPutLoopVar(psDbHead, "name", iNum, convert("GBK", "UTF-8", name));

      utPltPutLoopVar(psDbHead, "namedes", iNum, convert("GBK", "UTF-8", namedes));
      utPltPutLoopVarF(psDbHead, "ptype", iNum, "%d", ptype);

      utPltPutLoopVarF(psDbHead, "money", iNum, "%d", money);

      //utPltPutLoopVar(psDbHead,"serviceName",iNum,service_name);

      printf("name=%s\n", name);



    }

    pasDbCloseCursor(psCur);

  }



#if SERVICE_INFO

  //utPltShowDb(psDbHead);

#endif

  //utPltPutVarF(psDbHead, "TotRec", "%lu", iNum);



  utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/package/update_package.htm");

  return 0;

}







int ict_support_help(utShmHead * psShmHead, int iFd, utMsgHead * psMsgHead)

{

  //�Ѳ���Ҫ�Ĵ����ɾ��

  utMsgPrintMsg(psMsgHead);

  int id = 0;

  char messageTitle[256] = "";

  char messageContent[256] = "";
  int iReturn = 0;
  char caMsg[256] = "";

  //ȡǰ̨�Ĳ�������ֵ������������������Ϊ���ݿ��ѯ������ǰ̨���̨ר�����ݵķ�ʽһ��Ϊquerystring�ķ�ʽ������д������Ļ����ͽ�����������Ӧ��ֵ

  //����ط������ֱ���͵��µĲ����ĸ�����ͬ

  utMsgGetSomeNVar(psMsgHead, 3,

                   "id",   UT_TYPE_LONG8,  sizeof(id),     &id,

                   "messageTitle", UT_TYPE_ULONG,  sizeof(messageTitle) - 1, messageTitle,

                   "messageContent",   UT_TYPE_ULONG,  sizeof(messageContent) - 1,messageContent);

  char sql[1024] = "";

  pasDbCursor *psCur = NULL;

  utPltDbHead *psDbHead = utPltInitDb();

  //ulong db_count = 0;
  //��ʹ��sql֮ǰ����sql�����

  memset(sql, 0, sizeof(sql));

  snprintf(sql, sizeof(sql), "insert into help (messageTitle,messageContent)values('%s','%s')", convert("UTF-8", "GBK", messageTitle), convert("UTF-8", "GBK", messageContent));

  printf("%s\n", sql);


  iReturn = pasDbExecSql(sql, 0);

  memset(caMsg, 0, sizeof(caMsg));

  if(iReturn != 0)

  {

    snprintf(caMsg, sizeof(caMsg) - 1, "�������ʧ��!");

    utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
	
	utPltPutVar(psDbHead, "success", "false");
    utPltPutVarF(psDbHead, "result", "%d", 2);//�������ʧ��!

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/support/support_help.htm");

  }

  else

  {

    snprintf(caMsg, sizeof(caMsg) - 1, "������Գɹ�!");

    utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));
	utPltPutVar(psDbHead, "success", "true");

    utPltPutVarF(psDbHead, "result", "%d", 0);//������Գɹ�

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/support/support_help.htm");

  }






  

  return  0;

}



int ict_system_update_password(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)

{

  //�Ѳ���Ҫ�Ĵ����ɾ��



  utMsgPrintMsg(psMsgHead);

  printf("hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n");

  //int id = 0;

  char vName[32 + 1] = "";

  char oldPasswd[24 + 1] = "";

  char newPasswd[24 + 1] = "";

  ulong lCount = 0;

  char caMsg[256] = "";

  int iReturn = 0;

  //ȡǰ̨�Ĳ�������ֵ������������������Ϊ���ݿ���������ݣ�ǰ̨���̨ר�����ݵķ�ʽһ��Ϊquerystring�ķ�ʽ������д������Ļ����ͽ�����������Ӧ��ֵ

  //����ط������ֱ���͵��µĲ����ĸ�����ͬ
  utMsgGetSomeNVar(psMsgHead, 3,

                   "vName",  UT_TYPE_STRING, sizeof(vName) - 1,  vName,

                   "oldPasswd",  UT_TYPE_STRING, sizeof(oldPasswd) - 1, oldPasswd,

                   "newPasswd",  UT_TYPE_STRING, sizeof(newPasswd) - 1, newPasswd);





  char sqlbuf[1024] = "";

  utPltDbHead *psDbHead = utPltInitDbHead();
  memset(sqlbuf, 0, sizeof(sqlbuf));
  //������֤���û�����ľ������Ƿ���ȷ�������ȷ��ִ��������޸�����������������ȷ�Ļ�����ôֱ�ӽ���ʾ��Ϣ���ظ�ǰ̨
  snprintf(sqlbuf, sizeof(sqlbuf) - 1, "select count(*) from userlib where vname='%s' and passwd='%s'", vName, oldPasswd);

  printf("sqlbuf=%s  \n", sqlbuf);
  pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);
  if(lCount == 0)

  {

    snprintf(caMsg, sizeof(caMsg) - 1, "�����벻��ȷ!");

    utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));

    utPltPutVarF(psDbHead, "result", "%d", 1);//�����벻��ȷ

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/update_password/update_password.htm");

    return 0;

  }



  //��ʹ��sql֮ǰ����sql�����

  memset(sqlbuf, 0, sizeof(sqlbuf));

  snprintf(sqlbuf, sizeof(sqlbuf), "update userlib set passwd = '%s' where vname ='%s'", newPasswd, vName);

  printf("%s\n", sqlbuf);

  iReturn = pasDbExecSql(sqlbuf, 0);

  memset(caMsg, 0, sizeof(caMsg));

  if(iReturn != 0)

  {

    snprintf(caMsg, sizeof(caMsg) - 1, "�����޸�ʧ��!,sql���ִ�в��ɹ�!");

    utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));

    utPltPutVarF(psDbHead, "result", "%d", 2);//"�����޸�ʧ��!

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/update_password/update_password.htm");

  }

  else

  {

    snprintf(caMsg, sizeof(caMsg) - 1, "�����޸ĳɹ�!");

    utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));

    utPltPutVarF(psDbHead, "result", "%d", 0);//�����޸ĳɹ�

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/update_password/update_password.htm");

  }



  return  0;

}


int ict_change_package(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)

{

  //�Ѳ���Ҫ�Ĵ����ɾ��

  utMsgPrintMsg(psMsgHead);

  printf("hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n");

  //int id = 0;

  char vName[32 + 1] = "";

  char Package[32 + 1] = "";

  ulong lCount = 0;

  char caMsg[256] = "";

  int iReturn = 0;

  //ȡǰ̨�Ĳ�������ֵ������������������Ϊ���ݿ���������ݣ�ǰ̨���̨ר�����ݵķ�ʽһ��Ϊquerystring�ķ�ʽ������д������Ļ����ͽ�����������Ӧ��ֵ

  //����ط������ֱ���͵��µĲ����ĸ�����ͬ
  utMsgGetSomeNVar(psMsgHead, 3,

                   "vname",  UT_TYPE_STRING, sizeof(vName) - 1,  vName,

                   "Package",  UT_TYPE_STRING, sizeof(Package) - 1, Package);


  char sqlbuf[1024] = "";

  utPltDbHead *psDbHead = utPltInitDbHead();
  memset(sqlbuf, 0, sizeof(sqlbuf));


  snprintf(sqlbuf, sizeof(sqlbuf), "update userlib set Package = '%s' where vname ='%s'", convert("UTF-8", "GBK", Package), vName);

  printf("%s\n", sqlbuf);

  iReturn = pasDbExecSql(sqlbuf, 0);

  memset(caMsg, 0, sizeof(caMsg));

  if(iReturn != 0)

  {

    snprintf(caMsg, sizeof(caMsg) - 1, "�ײ��޸�ʧ��!,sql���ִ�в��ɹ�!");

    utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));

    utPltPutVarF(psDbHead, "result", "%d", 2);//�ײ��޸�ʧ�ܰ

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/package/change_package.htm");

  }

  else

  {

    snprintf(caMsg, sizeof(caMsg) - 1, "�ײ��޸ĳɹ�!");

    utPltPutVar(psDbHead, "mesg", convert("GBK", "UTF-8", caMsg));

    utPltPutVarF(psDbHead, "result", "%d", 0);//�ײ��޸ĳɹ�

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "school/package/change_package.htm");

  }

  return  0;

}




int ictInitWebFun_hjj(utShmHead * psShmHead)

{

  pasSetTcpFunName("ict_support_help", ict_support_help, 0);

  pasSetTcpFunName("ict_show_package", ict_show_package, 0);

  pasSetTcpFunName("ict_login", ict_login, 0);

  pasSetTcpFunName("ict_system_update_password", ict_system_update_password, 0);
  pasSetTcpFunName("ict_change_package", ict_change_package, 0);

  return 0;

}





