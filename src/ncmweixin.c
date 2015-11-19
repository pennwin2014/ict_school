#include <sys/types.h>

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <string.h>

#include <time.h>

#include <sys/stat.h>

#include <unistd.h>

#include "utoall.h"

#include "pasutl.h"

#include "pasdb.h"

#include "pasutl.h"

#include "ncportal.h"

#include "ncportalweb.h"

#include "utoplt01.h"

#define NCM_SRCFILE 8003





//#define TOKEN "pronetway"

//#define PRONETWAYID "gh_8df4367b0e47"

//#define APPID "wx086f838720eb4d73"

//#define APPSECRET "1ba4edc4bc8ebff3541961fc0785c8f8"



typedef struct myarray

{

    char tmpchar[128];

} myarray_s;

static int ncmUtlSortOnCharacter(char *p1, char *p2)

{

    int i;

    long l;

    myarray_s psC1, psC2;

    memcpy(&psC1, p1, sizeof(myarray_s));

    memcpy(&psC2, p2, sizeof(myarray_s));



    if(strcmp(psC1.tmpchar, psC2.tmpchar) > 0)

    {

        return 1;

    }
    else if(strcmp(psC1.tmpchar, psC2.tmpchar) == 0)

    {

        return 1;

    }
    else

    {

        return -1;

    }



}

char *ncmutlremovebackets(char *p)

{

    int i = 0, j = 0;

    char *p1 = NULL;

    p1 = (char *)malloc(1024 * sizeof(char));

    for(i = 0; i < strlen(p); i++)

    {

        if(*(p + i) == '{' || *(p + i) == '"' || *(p + i) == '}')
        {



            //         printf("1p = %s\n",p);

        }

        else

        {

            *(p1 + j) = *(p + i);

            j++;

        }



    }

    *(p1 + j) = '\0';

    return p1;

}



int ncmwxGetMenuStatus(char *menustatus)
{

    uchar *p = NULL, *p1 = NULL;

    uchar caTemp[256] = "", caTemp1[256] = "";

    char errcode[16] = "", errmsg[256] = "";

    long lerrcode = 0;

    p = (char *)ncmutlremovebackets(menustatus);

    while(*p)
    {

        strcpy(caTemp, "\0");

        p = utStrGetWord(p, caTemp, 255, "。");

        if(!utStrIsSpaces(caTemp))

        {

            p1 = caTemp;

            strcpy(errcode, "\0");

            strcpy(errmsg, "\0");

            while(1)

            {

                p1 = utStrGetWord(p1, caTemp1, 255, ",");

                if(!utStrIsSpaces(caTemp1))

                {

                    if(strncasecmp(caTemp1, "errcode:", 8) == 0)
                    {

                        strcpy(errcode, caTemp1 + 8);

                        sscanf(errcode, "%u", &lerrcode);

                    }

                    else if(strncasecmp(caTemp1, "errmsg:", 7) == 0)
                    {

                        strcpy(errmsg, caTemp1 + 7);

                    }



                }

                if(*p1)
                {

                    p1 ++;

                }

                else
                {

                    break;

                }

            }

            //              printf("errcode = %s , errmsg = %s  \n",errcode,errmsg);



        }

        if(*p)

        {

            p++;

        }

    }

    return lerrcode;

}



char *ncmWeiXinJsonScript()

{

    char jsonmenu[2048] = "";

    sprintf(jsonmenu, "{\"button\":[ \

            { \

            \"type\":\"click\", \

            \"name\":\"%s\", \

            \"key\":\"ncm_wifi_net\" \

        }, \

            { \

            \"type\":\"click\", \

            \"name\":\"新网程\", \

            \"key\":\"ncm_official_website\" \

        }, \

            { \

            \"name\":\"关于我们\", \

            \"sub_button\":[ \

            { \

            \"type\":\"view\", \

            \"name\":\"新网程官网\", \

            \"url\":\"http://www.pronetway.com/\" \

        }, \

            { \

            \"type\":\"view\", \

            \"name\":\"Wifi演示\", \

            \"url\":\"http://192.168.20.190/shop.htm/\" \

        }, \

            { \

            \"type\":\"click\", \

            \"name\":\"赞下我们\", \

            \"key\":\"V1001_GOOD\" \

        }] \

        } \

            ] \

        }", "Wifi 上网");

    return (char *)jsonmenu;

}

char *ncmWeiXinAutoJSon()

{

    char jsonmenu[2048] = "";

    sprintf(jsonmenu, "{\"button\":[ \

            { \

            \"type\":\"click\", \

            \"name\":\"%s\", \

            \"key\":\"ncm_wifi_net\" \

        } \

            ] \

        }", "Wifi 上网");

    return (char *)jsonmenu;

}

char *ncmutlremovelastchar(char *p)

{

    int i = 0, j = 0;

    char *p1 = NULL;

    p1 = (char *)malloc(strlen(p) * sizeof(char));

    for(i = 0; i < strlen(p); i++)

    {

        if(j != strlen(p) - 1)
        {

            *(p1 + j) = *(p + i);

            j++;

        }



    }

    //    p = p1;



    return p1;

}

char *ncmutlremovelastchar2(char p[2048])

{

    char p1[2048] = "";

    strncpy(p1, p, strlen(p) - 1);

    return (char *)p1;

}

//拆分微信返回结果串，获取access_token

char *ncmwxGetAccessToken(char *accesstokenResult)
{

    uchar *p = NULL, *p1 = NULL;

    uchar caTemp[256] = "", caTemp1[256] = "";

    char access_token[512] = "", expires_in[12] = "";

    long lexpires_in = 0;



    if(accesstokenResult)

    {

        //去除大括号和双引号

        p = (char *)ncmutlremovebackets(accesstokenResult);

        printf(" me p = %s \n", p);

        while(*p)
        {

            strcpy(caTemp, "\0");

            p = utStrGetWord(p, caTemp, 255, "。");

            if(!utStrIsSpaces(caTemp))

            {

                p1 = caTemp;

                strcpy(access_token, "\0");

                strcpy(expires_in, "\0");

                while(1)

                {

                    p1 = utStrGetWord(p1, caTemp1, 255, ",");

                    if(!utStrIsSpaces(caTemp1))

                    {

                        if(strncasecmp(caTemp1, "access_token:", 13) == 0)
                        {

                            strcpy(access_token, caTemp1 + 13);

                        }

                        else if(strncasecmp(caTemp1, "expires_in:", 11) == 0)
                        {

                            strcpy(expires_in, caTemp1 + 11);

                            sscanf(expires_in, "%u", &lexpires_in);

                        }



                    }

                    if(*p1)
                    {

                        p1 ++;

                    }

                    else
                    {

                        break;

                    }

                }

                printf("access_token = %s , expires_in = %s  \n", access_token, expires_in);



            }

            if(*p)

            {

                p++;

            }

        }



    }



    return (char *)access_token;

}

//拆分微信返回结果串，获取 昵称

char *ncmwxGetNickName(char *nicktmp)
{

    uchar *p = NULL, *p1 = NULL;

    uchar caTemp[256] = "", caTemp1[256] = "";

    char subscribe[12] = "", openid[256] = "", nickname[64] = "", city[64] = "";

    long lexpires_in = 0;



    if(nicktmp)

    {

        //去除大括号和双引号

        p = (char *)ncmutlremovebackets(nicktmp);

        printf(" me p = %s \n", p);

        while(*p)
        {

            strcpy(caTemp, "\0");

            p = utStrGetWord(p, caTemp, 255, "。");

            if(!utStrIsSpaces(caTemp))

            {

                p1 = caTemp;

                strcpy(subscribe, "\0");

                strcpy(openid, "\0");

                strcpy(nickname, "\0");

                while(1)

                {

                    p1 = utStrGetWord(p1, caTemp1, 255, ",");

                    if(!utStrIsSpaces(caTemp1))

                    {

                        if(strncasecmp(caTemp1, "subscribe:", 10) == 0)
                        {

                            strcpy(subscribe, caTemp1 + 10);

                        }

                        else if(strncasecmp(caTemp1, "openid:", 7) == 0)
                        {

                            strcpy(openid, caTemp1 + 7);

                        }

                        else if(strncasecmp(caTemp1, "nickname:", 9) == 0)
                        {

                            strcpy(nickname, caTemp1 + 9);

                        }

                        else if(strncasecmp(caTemp1, "city:", 5) == 0)
                        {

                            strcpy(city, caTemp1 + 5);

                        }



                    }

                    if(*p1)
                    {

                        p1 ++;

                    }

                    else
                    {

                        break;

                    }

                }

                printf("subscribe = %s openid = %s , nickname = %s  \n", subscribe, openid, nickname);



            }

            if(*p)

            {

                p++;

            }

        }



    }



    return (char *)nickname;

}

int ncmcomweixin(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)

{

    char caHost[64];

    uint4 lMyId = 0, lIp, nPort, lTime, lSynDate, lThisDate, lSize;

    int4  iRet, iReturn;

    uchar *pCont, *p, *p1;

    uchar caTemp[256], dstEchostr[128], tmpch[6] = "", caTemp1[256] = "";

    uint4 lSynTime = 0, iCount = 0 , lSynPos = 0, i;



    myarray_s array[3];

    uchar signature[128] = "", timestamp[32] = "", nonce[64] = "", signaturetmp[128] = "", signaturefintmp[128] = "", echostr[64] = "";

    uchar PASXml[2048] = "", respXml[4048] = "", caContent[2048] = "";

    char *tmputf8 = NULL;

    char acceptId[64] = "";

    //    char portalUrl[512] = "http://192.168.20.190:8008/yuyuan/shouye.html";

    //    char portalUrl[512] = "http://180.169.1.201:3080/alogin_main.htm";   //公网测试

    char portalUrl[512] = "http://www.cpic.com.cn/?&openid@oUw_CjoWwhl4WNdJo9ZdY_-yRoVs&nickname@test&timestamp@1395969847&echostr@abcdefg";

    char accesstokenUrl[512] = "", access_token[512] = "", expires_in[12] = "", jsonmenu[2048] = "";



    long lexpires_in = 0;

    char *accesstokentmp = NULL;



    char menucreateUrl[512] = "", PASXml1[2048] = "", PASXml_char[2048] = "";

    char *menustatus = NULL;

    long lmenustatus;

    char *EventKey, *content;

    char sendCont[32] = "我要上网";

    char openid[64] = "";

    utPltDbHead *psDbHead;

    psDbHead = utPltInitDb();

    char token[64] = "", pronetwayid[64] = "", appid[64] = "", appsecret[64] = "", wxurl[128] = "", nickUrl[1024] = "";

    char *pConfig;

    pasConfList *psConfig;

    ncwxaccesstoken *wxtoken;

    char caCompanyname[64] = "", getUsInfo[16] = "", nickname[64] = "", account[32] = "", sqlbuf[1024] = "", wxMenuDef[32] = "";

    char *nicktmp = NULL;

    unsigned long  lcomid = 0;

    ncmweixin *ncmweixin_s, ncmwx_s;





    printf("2222fffffffffffffffffffffffffffffffffffffffffff\n");

    printf("pid=%d\n", getpid());

    //   sleep(15);

    utMsgPrintMsg(psMsgHead);

    iRet = utMsgGetSomeNVar(psMsgHead, 6, "echostr",   UT_TYPE_STRING, sizeof(echostr) - 1,   echostr,

                            "signature", UT_TYPE_STRING, sizeof(signature) - 1, signature,

                            "PASXml",    UT_TYPE_STRING, sizeof(PASXml) - 1,    PASXml1,

                            "timestamp", UT_TYPE_STRING, sizeof(timestamp) - 1, timestamp,

                            "account",   UT_TYPE_STRING, sizeof(account) - 1,   account,

                            "nonce",     UT_TYPE_STRING, sizeof(nonce) - 1,     nonce);



    //根据公众号从共享内存中取对应信息

    if(strlen(account) > 0)

    {

        ncmweixin_s = (ncmweixin *)utShmLHashLookA(psShmHead, NCM_LNK_WEIXIN, account);

        printf("account = %s \n", account);

        if(ncmweixin_s)

        {

            strcpy(token, ncmweixin_s->token);

            strcpy(pronetwayid, ncmweixin_s->account);

            strcpy(appid, ncmweixin_s->appid);

            strcpy(appsecret, ncmweixin_s->appsecret);

            strcpy(sendCont, ncmweixin_s->sendcont);

            strcpy(wxurl, ncmweixin_s->redurl);

            strcpy(getUsInfo, ncmweixin_s->usinfo);

            strcpy(access_token, ncmweixin_s->access_token);

            printf("****account = %s token = %s\n", ncmweixin_s->account, ncmweixin_s->token);

        }

    }

    else      //取不到账号，支持老的配置

    {

        pConfig = (char *)pasGetConfigFileName();

        psConfig = (pasConfList *)pasUtlReadConfig(pConfig);

        if(psConfig == NULL)
        {

            utWebDispMsg(iFd, psMsgHead, "ncmsysman/ncmsg_back.htm", "微信配置", ncLang("0427配置文件出错"));

            return 0;

        }

        strcpy(token, (char *)pasUtlLookConfig(psConfig, "WXTOKEN", "pronetway"));

        strcpy(pronetwayid, (char *)pasUtlLookConfig(psConfig, "WXPRONETWAYID", "gh_8df4367b0e47"));

        strcpy(appid, (char *)pasUtlLookConfig(psConfig, "WXAPPID", ""));

        strcpy(appsecret, (char *)pasUtlLookConfig(psConfig, "WXAPPSECRET", "1ba4edc4bc8ebff3541961fc0785c8f8"));

        strcpy(sendCont, (char *)pasUtlLookConfig(psConfig, "WXSENDCONT", "我要上网"));

        strcpy(wxurl, (char *)pasUtlLookConfig(psConfig, "WXURL", "http://www.pronetway.com"));

        //          printf("wxurl = %s \n",wxurl);

        pasUtlFreeConfig(psConfig);

        //服务号 获取昵称

        strcpy(getUsInfo, (char *)utComGetVar_sd(psShmHead, "WXGetUserInfo", "No"));

    }



    sprintf(accesstokenUrl, "https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=%s&secret=%s", appid, appsecret);





    //    strcpy(caCompanyname,(char *)utComGetVar_sd(psShmHead,"CustomId","pronetway"));

    //    lcomid=utMd5Code(caCompanyname,strlen(caCompanyname),"ffff");

    //      if(lcomid>2000000000) lcomid=lcomid-2000000000;

    //      if(lcomid>2000000000) lcomid=lcomid-2000000000;

    //    printf("lcomid = %d \n",lcomid);

    if(strlen(PASXml1) > 0)

    {

        strcpy(PASXml, (char *)ncmutlremovelastchar2(PASXml1));

        printf("PASXml = %s \n", PASXml);

    }

    //发送消息后检查access_token是否过期，没过期return  过期更新

    /**/

    printf("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");

    if(strlen(PASXml) > 0)

    {

        //两种方式 1、点击wifi上网 2、发送我要上网



        strcpy(PASXml_char, "<?xml version=\"1.0\" ?>\n");

        strcat(PASXml_char, PASXml);

        strcpy(openid, (char *)ncmWeiXinfindkeyword(PASXml_char, "FromUserName"));

        content = strdup((char *)ncmWeiXinfindkeyword(PASXml_char, "Content"));

        EventKey = strdup((char *)ncmWeiXinfindkeyword(PASXml_char, "EventKey"));

        printf("****openid = %s,content = %s EventKey = %s \n ", openid, content, EventKey);





        if(strcasecmp(getUsInfo, "Yes") == 0)

        {

            sprintf(nickUrl, "https://api.weixin.qq.com/cgi-bin/user/info?access_token=%s&openid=%s&lang=zh_CN", access_token, openid);

            nicktmp = strdup((char *)ncmWeiXinGetAccessToken(nickUrl));

            memset(nickname, 0, sizeof(nickname));

            strcpy(nickname, (char *)ncmwxGetNickName(nicktmp));

            if(nicktmp)

                free(nicktmp);

            if(strlen(nickname) > 0)

            {

                pasCvtGBK(2, nickname, caTemp, 64);

                strcpy(nickname, caTemp);

            }



            /*

            wxtoken = (ncwxaccesstoken *)utShmLHashLookA(psShmHead,NCM_LNK_WXATOKEN,&lcomid);

             if (wxtoken)

             {

               printf("access_token = %s \n",wxtoken->access_token);

               sprintf(nickUrl,"https://api.weixin.qq.com/cgi-bin/user/info?access_token=%s&openid=%s&lang=zh_CN",wxtoken->access_token,openid);

               nicktmp =strdup((char *)ncmWeiXinGetAccessToken(nickUrl));

                 memset(nickname,0,sizeof(nickname));

                 strcpy(nickname,(char *)ncmwxGetNickName(nicktmp));

                 if (nicktmp)

                   free(nicktmp);

                 if (strlen(nickname)>0)

                 {

                           pasCvtGBK(2,nickname,caTemp,64);

                           strcpy(nickname,caTemp);

                 }

            }*/

        }



        memset(portalUrl, 0, sizeof(portalUrl));

        sprintf(portalUrl, "%s/?ProOpenid@%s&nickname@%s&timestamp@%lu&echostr@abcdefg", wxurl, openid, nickname, time(0));

        printf("portalUrl = %s \n", portalUrl);

        //       strcpy(portalUrl,"http://10.1.0.254:9080/pronline/Msg?FunName@ncSrvDispLogin&wlanacname@XXXX&userip@10.1.0.254&ssid@XXXX&usermac@54:ae:27:db:9d:78&openid@test123456");

        if(strlen(content) > 0)
        {

            pasCvtGBK(2, content, caTemp, 64);

            strcpy(content, caTemp);

        }





        if(strcasecmp(content, sendCont) == 0 || strcmp(EventKey, "ncm_wifi_net") == 0)

        {

            if(strlen(nickname) > 0)

            {

                sprintf(caContent, "您好，%s，请<a href = '%s' >点击上网</a>。", nickname, portalUrl);

            }

            else

            {

                sprintf(caContent, "您好，请<a href = '%s' >点击上网</a>。", portalUrl);

            }

            tmputf8 = (char *)convert("GBK", "UTF-8", caContent);

            sprintf(respXml, "<xml><ToUserName><![CDATA[%s]]></ToUserName> \

                    <FromUserName><![CDATA[%s]]></FromUserName> \

                    <CreateTime>%lu</CreateTime> \

                    <MsgType><![CDATA[text]]></MsgType> \

                    <Content><![CDATA[%s]]></Content> \

                    </xml>\n", openid, pronetwayid, time(0), tmputf8);

            free(tmputf8);
            tmputf8 = NULL;

            utPltPutVarF(psDbHead, "PASXml", "%s", respXml);

            utPltShowDb(psDbHead);

            utComTcpResponse(iFd, psMsgHead, 1,

                             "text", UT_TYPE_STRING, respXml);

            utPltFreeDb(psDbHead);

            //             if(openid)

            //                free(openid);

            if(content)

                free(content);

            if(EventKey)

                free(EventKey);

            //             return 0;

        }

        else if(strcmp(EventKey, "ncm_official_website") == 0)

        {

            sprintf(caContent, "新网程热线：02151556000。", portalUrl);

            tmputf8 = (char *)convert("GBK", "UTF-8", caContent);

            sprintf(respXml, "<xml><ToUserName><![CDATA[%s]]></ToUserName> \

                    <FromUserName><![CDATA[%s]]></FromUserName> \

                    <CreateTime>%lu</CreateTime> \

                    <MsgType><![CDATA[text]]></MsgType> \

                    <Content><![CDATA[%s]]></Content> \

                    </xml>\n", openid, pronetwayid, time(0), tmputf8);

            free(tmputf8);
            tmputf8 = NULL;

            utPltPutVarF(psDbHead, "PASXml", "%s", respXml);

            utPltShowDb(psDbHead);

            utComTcpResponse(iFd, psMsgHead, 1,

                             "text", UT_TYPE_STRING, respXml);

            utPltFreeDb(psDbHead);

            if(EventKey)

                free(EventKey);

            return 0;

        }

        else

        {

            //           return 0;

        }

    }



    ncmweixin_s = (ncmweixin *)utShmLHashLookA(psShmHead, NCM_LNK_WEIXIN, account);

    printf("account = %s \n", account);

    if(ncmweixin_s)

    {

        if(ncmweixin_s->authstat == 1)            //已注册只检测access_token 是否过期

        {

            //ncmweixin_s->tokentime=0;

            //未注册成功前是无法检查和生成access_token的

            if(ncmweixin_s->tokentime == 0 || (time(0) - ncmweixin_s->tokentime) > 5400) //第一次获取token

            {

                accesstokentmp = strdup((char *)ncmWeiXinGetAccessToken(accesstokenUrl));

                strcpy(access_token, (char *)ncmwxGetAccessToken(accesstokentmp));

                if(accesstokentmp)

                    free(accesstokentmp);

                printf("*****check**2*** access_token = %s \n", access_token);

                //     //创建自定义菜单

                //     strcpy(access_token,"Dj4WFTN1QNOC_xXA1Cu1tkfAiJxHME6CpZ3yfCKWOq0l2gENEwKq01f1TV0wxDT2GvDz2m2gDkXJXMOS8nnfz-Ru50cxNNVYjD_SdFOrfl3vH54u_m5blejGJFxsKZQSbov3GTfBsYNNb5-aEOof1w");

                trim(access_token);

                sprintf(menucreateUrl, "https://api.weixin.qq.com/cgi-bin/menu/create?access_token=%s", access_token);



                //创建菜单button

                strcpy(wxMenuDef, (char *)utComGetVar_sd(psShmHead, "WxMenuDef", "No"));

                if(strcasecmp(wxMenuDef, "Yes") == 0)

                    strcpy(jsonmenu, (char *)ncmWeiXinAutoJSon());

                else

                    strcpy(jsonmenu, (char *)ncmWeiXinJsonScript());

                tmputf8 = (char *)convert("GBK", "UTF-8", jsonmenu);

                //创建

                menustatus = strdup((char *)ncmWeiXinPostMsg(menucreateUrl, tmputf8));

                printf("******menustatus= %s*****\n", menustatus);

                if(menustatus)

                {

                    //解析返回状态

                    lmenustatus = ncmwxGetMenuStatus(menustatus);

                    if(lmenustatus == 0)

                    {

                        strcpy(ncmweixin_s->access_token, access_token);

                        ncmweixin_s->tokentime = time(0);

                        sprintf(sqlbuf, "update ncmweixin set access_token='%s', tokentime =%lu where account='%s' ", access_token, time(0), pronetwayid);

                        pasDbExecSqlF(sqlbuf);

                        printf("**menu update**systime = %d access_token = %s\n", ncmweixin_s->tokentime, ncmweixin_s->access_token);

                        printf("******Menu create success**up***\n");

                    }

                    else if(lmenustatus == 42001)          //access_token 过期

                    {

                        //重新获取access_token 并创建菜单

                        accesstokentmp = strdup((char *)ncmWeiXinGetAccessToken(accesstokenUrl));

                        strcpy(access_token, (char *)ncmwxGetAccessToken(accesstokentmp));

                        if(accesstokentmp)

                            free(accesstokentmp);

                        strcpy(ncmweixin_s->access_token, access_token);

                        ncmweixin_s->tokentime = time(0);

                        sprintf(sqlbuf, "update ncmweixin set access_token='%s', tokentime =%lu where account='%s' ", access_token, time(0), pronetwayid);

                        pasDbExecSqlF(sqlbuf);



                        printf("****42001**Menu create error***up**\n");

                    }

                    else

                    {

                        printf("******Menu create error***up**\n");

                    }

                    free(menustatus);

                }

                free(tmputf8);
                tmputf8 = NULL;

            }

            else

            {



            }

            return 0;

        }

        else

        {

            for(i = 0; i < 3; i++)

            {

                if(i == 0)

                {

                    strcpy(array[i].tmpchar, token);

                }

                else  if(i == 1)

                {

                    strcpy(array[i].tmpchar, timestamp);

                }

                else  if(i == 2)

                {

                    strcpy(array[i].tmpchar, nonce);

                }

            }

            qsort(array, 3, sizeof(myarray_s), ncmUtlSortOnCharacter);



            strcpy(signaturetmp, array[0].tmpchar);

            strcat(signaturetmp, array[1].tmpchar);

            strcat(signaturetmp, array[2].tmpchar);

            printf("signaturetmp = %s \n", signaturetmp);

            strcpy(signaturefintmp, (char *)sha1_encode(signaturetmp));

            for(i = 0 ; i < 20 ; i++)
            {

                printf("%02x", signaturefintmp[i]);

                sprintf(tmpch, "%02x", signaturefintmp[i]);

                if(i == 0)

                {

                    strcpy(dstEchostr, tmpch);

                }

                else

                {

                    strcat(dstEchostr, tmpch);

                }

                memset(tmpch, 0, sizeof(tmpch));

            }

            printf(" \n");





            if(strlen(dstEchostr) > 0 && strcmp(dstEchostr, signature) == 0)

            {

                utPltPutVarF(psDbHead, "echostr", "%s", echostr);

                utPltShowDb(psDbHead);

                utComTcpResponse(iFd, psMsgHead, 1,

                                 "text", UT_TYPE_STRING, echostr);

                utPltFreeDb(psDbHead);

                sprintf(sqlbuf, "update ncmweixin set authstat = 1 where account='%s' ", pronetwayid);

                pasDbExecSqlF(sqlbuf);

                ncmweixin_s->authstat = 1;

                if(ncmweixin_s->tokentime == 0)   //第一次获取token

                {

                    accesstokentmp = strdup((char *)ncmWeiXinGetAccessToken(accesstokenUrl));

                    strcpy(access_token, (char *)ncmwxGetAccessToken(accesstokentmp));

                    if(accesstokentmp)

                        free(accesstokentmp);

                }

                printf("*****register***** access_token = %s \n", access_token);

                //     //创建自定义菜单

                //     strcpy(access_token,"Dj4WFTN1QNOC_xXA1Cu1tkfAiJxHME6CpZ3yfCKWOq0l2gENEwKq01f1TV0wxDT2GvDz2m2gDkXJXMOS8nnfz-Ru50cxNNVYjD_SdFOrfl3vH54u_m5blejGJFxsKZQSbov3GTfBsYNNb5-aEOof1w");

                trim(access_token);

                sprintf(menucreateUrl, "https://api.weixin.qq.com/cgi-bin/menu/create?access_token=%s", access_token);



                //创建菜单button

                strcpy(wxMenuDef, (char *)utComGetVar_sd(psShmHead, "WxMenuDef", "No"));

                if(strcasecmp(wxMenuDef, "Yes") == 0)

                    strcpy(jsonmenu, (char *)ncmWeiXinAutoJSon());

                else

                    strcpy(jsonmenu, (char *)ncmWeiXinJsonScript());

                //     sprintf(jsonmenu,"{\"button\":[{\"type\":\"click\",\"name\":\"%s\",\"key\":\"ncm_wifi_net\"}]}","Wifi 上网");



                tmputf8 = (char *)convert("GBK", "UTF-8", jsonmenu);

                //创建

                menustatus = strdup((char *)ncmWeiXinPostMsg(menucreateUrl, tmputf8));

                printf("******menustatus= %s*****\n", menustatus);

                if(menustatus)

                {

                    //解析返回状态

                    lmenustatus = ncmwxGetMenuStatus(menustatus);

                    if(lmenustatus == 0)

                    {

                        strcpy(ncmweixin_s->access_token, access_token);

                        ncmweixin_s->tokentime = time(0);

                        sprintf(sqlbuf, "update ncmweixin set access_token='%s', tokentime =%lu where account='%s' ", access_token, time(0), pronetwayid);

                        printf("sqlbuf = %s\n", sqlbuf);

                        pasDbExecSqlF(sqlbuf);

                        printf("**menu create**systime = %d access_token = %s\n", ncmweixin_s->tokentime, ncmweixin_s->access_token);

                        printf("******Menu create success*****\n");

                    }

                    else if(lmenustatus == 42001)          //access_token 过期

                    {

                        //重新获取access_token 并创建菜单

                        accesstokentmp = strdup((char *)ncmWeiXinGetAccessToken(accesstokenUrl));

                        strcpy(access_token, (char *)ncmwxGetAccessToken(accesstokentmp));

                        //更新内存更新数据库

                        strcpy(ncmweixin_s->access_token, access_token);

                        ncmweixin_s->tokentime = time(0);

                        sprintf(sqlbuf, "update ncmweixin set access_token='%s', tokentime =%lu where account='%s' ", access_token, time(0), pronetwayid);

                        pasDbExecSqlF(sqlbuf);

                        printf("***new**mytest***** access_token = %s \n", access_token);

                        if(accesstokentmp)

                            free(accesstokentmp);

                        printf("****42001**Menu create error*****\n");

                    }

                    else

                    {

                        printf("******Menu create error*****\n");

                    }

                    free(menustatus);

                }

                free(tmputf8);
                tmputf8 = NULL;



            }

            return 0;

        }

    }



}



int Ncm_PortalWeiXin_SetFunName()

{

    int iReturn;

    iReturn = pasSetTcpFunName("ncmcomweixin", ncmcomweixin, 0);                             //微信认证

    return 0;

}



