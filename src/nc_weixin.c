#define  PAS_SRCFILE      2115

#include <iconv.h>
#include <ctype.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/rc2.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

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
#include "cjson.h"






/********************************************
功能：搜索字符串右边起的第一个匹配字符
********************************************/
/*char *Rstrchr(char *s, char x)
{
    int i = strlen(s);
    if(!(*s))
        return 0;
    while(s[i - 1])
        if(strchr(s + (i - 1), x))
            return (s + (i - 1));
        else
            i--;
    return 0;
}*/

/**************************************************************
功能：从字符串src中分析出网站地址和端口，并得到用户要下载的文件
***************************************************************/
/*
void GetHost(char *src, char *web, char *file, int *port)
{
    char *pA;
    char *pB;
    memset(web, 0, sizeof(web));
    memset(file, 0, sizeof(file));
    *port = 0;
    if(!(*src))
        return;
    pA = src;
    if(!strncmp(pA, "http://", strlen("http://")))
        pA = src + strlen("http://");
    else if(!strncmp(pA, "https://", strlen("https://")))
        pA = src + strlen("https://");
    pB = strchr(pA, '/');
    if(pB)
    {
        memcpy(web, pA, strlen(pA) - strlen(pB));
        if(pB + 1)
        {
            memcpy(file, pB + 1, strlen(pB) - 1);
            file[strlen(pB) - 1] = 0;
        }
    }
    else
        memcpy(web, pA, strlen(pA));
    if(pB)
        web[strlen(pA) - strlen(pB)] = 0;
    else
        web[strlen(pA)] = 0;
    pA = strchr(web, ':');
    if(pA)
        *port = atoi(pA + 1);
    else
        *port = 443;
}
*/
/************文档********************************************
*filename: ncmcomhttps.c
*purpose: 演示HTTPS客户端编程方法
gcc -Wall https-client.c -lssl -o httpsclient

运行此程序来取得HTTPS服务器上的页面，比如：
./httpsclient https://127.0.0.1/test.html

关键之处在于建立socket之后的SSL相关初始化以及中间的recv/send用SSL_read和SSL_write代替，最后记得释放SSL资源即可。

*********************************************************************/
//获取accesstoken
char *getHttpsCont(char argv[512])
{
    int sockfd, ret;
    char buffer[1024];
    struct sockaddr_in server_addr;
    struct hostent *host;
    int portnumber, nbytes;
    char host_addr[256];
    char host_file[1024];
    char local_file[256];
    //    FILE *fp;
    char caTemp[1024];
    char request[1024];
    int send, totalsend;
    int i;
    char *pt;
    SSL *ssl;
    SSL_CTX *ctx;

    char tmpch[4] = "";
    static char pCont[10024] = "";

    //    strcpy(argv,"https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=wx086f838720eb4d73&secret=1ba4edc4bc8ebff3541961fc0785c8f8");
    GetHost(argv, host_addr, host_file, &portnumber);        /*分析网址、端口、文件名等 */


    if((host = gethostbyname(host_addr)) == NULL)           /*取得主机IP地址 */
    {

        return NULL;
    }

    /* 客户程序开始建立 sockfd描述符 */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)           /*建立SOCKET连接 */
    {
        return NULL;
    }

    /* 客户程序填充服务端的资料 */
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portnumber);
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);

    /* 客户程序发起连接请求 */
    if(connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)            /*连接网站 */
    {
        return NULL;
    }

    /* SSL初始化 */
    SSL_library_init();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if(ctx == NULL)
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    ssl = SSL_new(ctx);
    if(ssl == NULL)
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    /* 把socket和SSL关联 */
    ret = SSL_set_fd(ssl, sockfd);
    if(ret == 0)
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    RAND_poll();
    while(RAND_status() == 0)
    {
        unsigned short rand_ret = rand() % 65536;
        RAND_seed(&rand_ret, sizeof(rand_ret));
    }

    ret = SSL_connect(ssl);
    if(ret != 1)
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    sprintf(request, "GET /%s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\n\
User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n\
Host: %s:%d\r\nConnection: Close\r\n\r\n", host_file, host_addr,
            portnumber);



    /*发送https请求request */
    send = 0;
    totalsend = 0;
    nbytes = strlen(request);
    while(totalsend < nbytes)
    {
        send = SSL_write(ssl, request + totalsend, nbytes - totalsend);
        if(send == -1)
        {
            return NULL;
        }
        totalsend += send;

    }


    i = 0;
    /* 连接成功了，接收https响应，response */
    while((nbytes = SSL_read(ssl, buffer, 1)) == 1)
    {
        if(i < 4)
        {
            if(buffer[0] == '\r' || buffer[0] == '\n')
                i++;
            else
                i = 0;

        }
        else
        {

            sprintf(tmpch, "%c", buffer[0]);
            if(i == 4)
            {
                strcpy(pCont, tmpch);
            }
            else
            {
                strcat(pCont, tmpch);
            }
            i++;
            //            if (i % 1024 == 0)
            //                fflush(fp);        /*每1K时存盘一次 */
        }
    }


    //    printf("\npCont = %s \n",pCont);
    //    fclose(fp);
    /* 结束通讯 */
    ret = SSL_shutdown(ssl);
    if(ret != 1)
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    close(sockfd);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    ERR_free_strings();
    //    exit(0);
    return pasCvtGBK(2, (char*)pCont, caTemp, 1000);
}


//post方式获取内容
char *getHttpsConByPost(char *pUrl, char *pPostVar)
{
    int sockfd, ret;
    char buffer[1024];
    struct sockaddr_in server_addr;
    struct hostent *host;
    int portnumber, nbytes;
    char host_addr[256];
    char host_file[1024];
    char local_file[256];
    char request[1024];
    int send, totalsend;
    int i;
    char *pt;
    SSL *ssl;
    SSL_CTX *ctx;

    char tmpch[4] = "";
    static char pCont[10024] = "";
    //    strcpy(argv,"https://api.weixin.qq.com/cgi-bin/menu/create?access_token=");
    GetHost(pUrl, host_addr, host_file, &portnumber);        /*分析网址、端口、文件名等 */

    if((host = gethostbyname(host_addr)) == NULL)           /*取得主机IP地址 */
    {
        return NULL;
    }


    /* 客户程序开始建立 sockfd描述符 */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)           /*建立SOCKET连接 */
    {
        return NULL;
    }

    /* 客户程序填充服务端的资料 */
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portnumber);
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);

    /* 客户程序发起连接请求 */
    if(connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)            /*连接网站 */
    {
        return NULL;
    }

    /* SSL初始化 */
    SSL_library_init();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if(ctx == NULL)
    {
        return NULL;
    }

    ssl = SSL_new(ctx);
    if(ssl == NULL)
    {
        return NULL;
    }

    /* 把socket和SSL关联 */
    ret = SSL_set_fd(ssl, sockfd);
    if(ret == 0)
    {
        return NULL;
    }

    RAND_poll();
    while(RAND_status() == 0)
    {
        unsigned short rand_ret = rand() % 65536;
        RAND_seed(&rand_ret, sizeof(rand_ret));
    }

    ret = SSL_connect(ssl);
    if(ret != 1)
    {
        ERR_print_errors_fp(stderr);
        return NULL;
    }
    //host_file
    /*
    post格式说明：每行前不可以有空格
    POST https://api.weixin.qq.com/cgi-bin/menu/create?access_token=lQEAXOvehXccOPDfSCCX50giI46RlcT2X_OqrhS_sbTkIi0laDx8bfMDGVEKD3IVgQl24PuiP9rXHKvskdTcOoPcRbj-K5NXfRxBSGNUgIaorMl3Z4Jx6WUnnAvAChVorpTh-cViMxiJltMNtC2LNg HTTP/1.1
    Accept: *\/*
    Accept-Language: zh-cn
    User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)
    Content-Type: application/json               //must
    Host: api.weixin.qq.com:443
    Connection: Keep-Alive
    Content-Length: 68                           //must

    {"button":[{"type":"click","name":"Wifi Net","key":"ncm_wifi_net"}]}

    506 bytes send OK!

    The following is the response header:
    HTTP/1.1 200 OK
    Server: nginx/1.4.4
    Date: Thu, 27 Mar 2014 06:16:11 GMT
    Content-Type: application/json; encoding=utf-8
    Content-Length: 27
    Connection: close

    {"errcode":0,"errmsg":"ok"}
    */
    sprintf(request, "POST %s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: UTF-8\r\n\
User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n\Content-Type: application/json\r\n\
Host: %s:%d\r\nConnection: Keep-Alive\r\nContent-Length: %d\r\n\r\n%s\r\n\r\n", pUrl, host_addr,
            portnumber, strlen(pPostVar), pPostVar);



    /*发送https请求request */
    send = 0;
    totalsend = 0;
    nbytes = strlen(request);
    while(totalsend < nbytes)
    {
        send = SSL_write(ssl, request + totalsend, nbytes - totalsend);
        if(send == -1)
        {
            return NULL;
        }
        totalsend += send;

    }



    i = 0;
    /* 连接成功了，接收https响应，response */
    while((nbytes = SSL_read(ssl, buffer, 1)) == 1)
    {
        if(i < 4)
        {
            if(buffer[0] == '\r' || buffer[0] == '\n')
                i++;
            else
                i = 0;

        }
        else
        {

            sprintf(tmpch, "%c", buffer[0]);
            if(i == 4)
            {
                strcpy(pCont, tmpch);
            }
            else
            {
                strcat(pCont, tmpch);
            }
            i++;
        }
    }

    /* 结束通讯 */
    ret = SSL_shutdown(ssl);
    if(ret != 1)
    {
        ERR_print_errors_fp(stderr);
        //        exit(1);
    }
    close(sockfd);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    ERR_free_strings();

    char caReturn[1024];
    pasUtf8ToGBK(pCont, caReturn, 1000);
    printf("return=%s\n", caReturn);

    return (char *)caReturn;
}

/*
char *getWeiXinAccessToken(char *appid, char *secret)
{
    char caReturn[10024];
    char caUrl[1024];
    static char caToken[640];
    cJSON* pRoot;
    cJSON * pSubJson = NULL;
    memset(caToken, 0, sizeof(caToken));

    sprintf(caUrl, "https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=%s&secret=%s", appid, secret);

    //printf("caUrl=%s\n",caUrl);
    strcpy(caReturn, getHttpsCont(caUrl));
    if(strstr(caReturn, "access_token"))
    {
        pRoot = cJSON_Parse(caReturn);
        if(pRoot)
        {
            strcpy(caToken, cJSON_GetObjectItem(pRoot, "access_token") ? cJSON_GetObjectItem(pRoot, "access_token")->valuestring : "\0");
        }
    }
    return caToken;
}
*/

char *getWeiXinNickName(char *token, char *openid)
{
    char caReturn[10024];
    char caUrl[1024];
    static char caToken[640];
    cJSON* pRoot;
    cJSON * pSubJson = NULL;
    memset(caToken, 0, sizeof(caToken));

    sprintf(caUrl, "https://api.weixin.qq.com/cgi-bin/user/info?access_token=%s&openid=%s", token, openid);
    strcpy(caReturn, getHttpsCont(caUrl));

    if(strstr(caReturn, "nickname"))
    {
        pRoot = cJSON_Parse(caReturn);
        if(pRoot)
        {
            strcpy(caToken, cJSON_GetObjectItem(pRoot, "nickname") ? cJSON_GetObjectItem(pRoot, "nickname")->valuestring : "\0");
        }
    }
    return caToken;
}



static char *convert(const char *codefrom, const char *codeto, const char *src)
{
    char    to[32] = "";
    char    *out = NULL;
    char    *dst = NULL;

    iconv_t c;
    size_t  n_in  =  strlen(src);
    size_t  n_out =  2 * n_in;

    snprintf(to, sizeof(to), "%s//IGNORE", codeto);
    if((out = (char *)calloc(n_out + 1, sizeof(char))) == NULL)
    {
        printf("allocate memory for %d bytes failed\n", n_out);
        return NULL;
    }
    dst = out;

    if((c = iconv_open(to, codefrom)) == (iconv_t)(-1))
    {
        printf("codefrom = %s, codeto = %s, convert failed\n", codefrom, codeto);
        return NULL;
    }
    iconv(c, (char **)&src, &n_in, &dst, &n_out);
    iconv_close(c);
    return out;
}


int AESDecrypt(const unsigned char *pcInput,
               unsigned int iInputLen,
               const unsigned char *pKey,
               unsigned char *pOut)
{
    if(pcInput == NULL || iInputLen < 16 || iInputLen % 16 != 0)
    {

        return -2;
    }
    const EVP_CIPHER *ptCipher = EVP_aes_128_cbc(); // hardware speed up

    EVP_CIPHER_CTX tCtx;
    EVP_CIPHER_CTX_init(&tCtx);
    EVP_CIPHER_CTX_set_padding(&tCtx, 1); // pkcs padding

    unsigned char pcKey[16];
    unsigned char pcIV[16];

    memset(pcKey, 0, sizeof(pcKey));
    memset(pcIV, 0, sizeof(pcIV));

    memcpy(pcKey, pKey, 16);

    memcpy(pcIV, pcKey, 16);

    int res = -3;

    do
    {
        int ret = 0;

        ret = EVP_DecryptInit_ex(&tCtx,
                                 ptCipher,
                                 NULL,
                                 pcKey,
                                 pcIV);

        if(ret != 1)
        {
            break;
        }
        int iLen = 120;
        int iPlainTextLen = 0;
        unsigned char caTemp[128];
        memset(caTemp, 0, sizeof(caTemp));
        ret = EVP_DecryptUpdate(&tCtx,
                                (unsigned char *)caTemp,
                                &iLen,
                                pcInput,
                                iInputLen);
        if(ret != 1)
        {
            break;
        }

        iPlainTextLen = iLen;

        ret = EVP_DecryptFinal_ex(&tCtx,
                                  (unsigned char *)(caTemp + iLen),
                                  &iLen);
        if(ret != 1)
        {
            break;
        }

        iPlainTextLen += iLen;
        memset(pOut, 0, sizeof(pOut));
        memcpy(pOut, caTemp, iPlainTextLen);
        res = 0;

    }
    while(0);

    OPENSSL_cleanse(pcKey, sizeof(pcKey));
    OPENSSL_cleanse(pcIV, sizeof(pcIV));
    EVP_CIPHER_CTX_cleanup(&tCtx);
    return res;
}



int DecrypMobileInfo(utShmHead *psShmHead, const char *pIn,
                     const char *pAPPID,
                     const char *pKey,
                     char *pMobile)
{
    /*
       encrypt info binary format:
       [0, 10]                  header
       [11, end]                encrypt data

       header format:
       [0, 0]                   format version
       [1, 2]                   key version (network byte order)
       [3, 6]                   nonce (network byte order)
       [7, 10]                  nocne2 (network byte order)

       encryption algorithm:
       algorithm                AES CBC 128bits,  IV = encrypt key
       encrypt key              md5_raw("appid_nonce_nonce2_key") (128bits binary output)
       key                      search by appid and key version

       decrypt info format:
       [0, 0]                   magic, 97
       [1, 2]                   reserve
       [3, 3]                   mobile len
       [4, 4 + mobile len]      mobile
     */

    //  std::string sInfo;
    unsigned char in[256];
    long iRet;
    char caVar[32], caKey[64];
    iRet = pasStrCvtHex2Bin((unsigned char*)pIn, in);

    if(iRet < 11)
    {
        //  sErrorMsg = "invalid header";
        return -1;
    }

    unsigned char iVersion;
    iVersion = in[0];

    unsigned short iKeyVerison, iKv;
    unsigned int iN1, iN2;
    memcpy(&iKv, in + 1, 2);
    iKeyVerison = ntohs(iKv);
	pasLogs(PAS_SRCFILE, 1001, "iKeyVerison=%d\n", iKeyVerison);
    memcpy(&iN1, in + 3, 4);
    memcpy(&iN2, in + 7, 4);
    unsigned int iNonce = ntohl(iN1);
    unsigned int iNonce2 = ntohl(iN2);

    if(iKeyVerison == 1)
    {
        strcpy(caKey, utComGetVar_sd(psShmHead, "wxmobilekey1", "fa95da76263e4fc09cabee212ece6d0a"));
    }
    else if(iKeyVerison == 2)
    {
        strcpy(caKey, utComGetVar_sd(psShmHead, "wxmobilekey2", "c942a335d39c62d9ead1ec78ff8b0a76"));
    }
    else if(iKeyVerison == 3)
    {
        strcpy(caKey, utComGetVar_sd(psShmHead, "wxmobilekey3", "d4e5f156febd78b09b8d4aa0f28ef0e9"));
    }
    else if(iKeyVerison == 4)
    {
        strcpy(caKey, utComGetVar_sd(psShmHead, "wxmobilekey4", "47624c4798acc8888f5276184cbd3934"));
    }
    else if(iKeyVerison == 5)
    {
        strcpy(caKey, utComGetVar_sd(psShmHead, "wxmobilekey5", "780bb0cd7b79e936e88843763039f848"));
    }
    else if(iKeyVerison == 6)
    {
        strcpy(caKey, utComGetVar_sd(psShmHead, "wxmobilekey6", "a03ef000bba983e1ee0a60c760f45d8d"));
    }
    else if(iKeyVerison == 7)
    {
        strcpy(caKey, utComGetVar_sd(psShmHead, "wxmobilekey7", "71fffb77230193b9602b7f6011612392"));
    }
    else if(iKeyVerison == 8)
    {
        strcpy(caKey, utComGetVar_sd(psShmHead, "wxmobilekey8", "e8bbb707e76814b160530472ae8aa114"));
    }

    char pcBuffer[128] = {0};
    snprintf(pcBuffer,
             sizeof(pcBuffer) - 1,
             "%s_%u_%u_%s",
             pAPPID,
             iNonce,
             iNonce2,
             caKey);

    unsigned char pKey_bin[32];

    utMd5(pcBuffer, strlen(pcBuffer), pKey_bin);

    // md5_raw
    char caOut[128];
    int ret = 0;


    // AES CBC 128bits decrypt, IV = encrypt key
    ret = AESDecrypt(in + 11,
                     iRet - 11,
                     (const unsigned char*)pKey_bin,
                     (unsigned char*)caOut);
    if(ret != 0)
    {
        //  sErrorMsg = "AES decrypt fail";
        return -4;
    }



    unsigned char iMagic = 0;
    unsigned char iMobileLen = 0;

    iMagic = caOut[0];;


    if(iMagic != 97)
    {
        return -6;
    }
    else
    {
        iMobileLen = caOut[3];
        char caMobile[32];
        memset(caMobile, 0, sizeof(caMobile));
        memcpy(caMobile, caOut + 4, iMobileLen);
        //      printf("caMobile=%s\n",caMobile);
        strcpy(pMobile, caMobile);
    }

    return 0;
}

/*
int getMacByIp(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caIp[128];
    int iReturn;
    unsigned long lIp;
    char caMac[20];
    char caMac1[16];
    iReturn = utMsgGetSomeNVar(psMsgHead, 11,
                               "clientip",    UT_TYPE_STRING, 20, caIp);
    ncUtlGetMacFromIpMacLst(psShmHead, lIp, caMac1);
    strcpy(caMac, ncCvtMac(caMac1));
    if(strlen(caMac) == 0)
    {
        strcpy(caMac, "10:00:00:00:00:01");
    }
    return 0;
}


//显示微信验证页面
int ncWebDispLogin_weixin(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caIp[128];
    char caUrl[2560];
    char caPlatePath[64], caPlate[128], caPort[16];
    char caSid[16];
    char caSrvname[64], caRequir[2560];
    char caFlag[8];
    char caUsragent[256];
    char caMac[20];
    char caUrl2[2560];
    char caTemp2[1024];
    char *p;
    int iReturn;
    unsigned long lIp, lSid;
    long lFlag, lLogin;
    char caEng_flag[16];
    char caPath_all[256];
    char caLogincount[16];
    char caMac1[16];
    char caUsertype[16];
    long lMobflag;
    utPltDbHead *psDbHead;
    pasDbCursor *psCur;

    char caTerm[64], caOs[64], caBro[64], caType[64];

    //    utMsgPrintMsg(psMsgHead);

    iReturn = utMsgGetSomeNVar(psMsgHead, 11,
                               "clientip",    UT_TYPE_STRING, 20, caIp,
                               "httpref",     UT_TYPE_STRING, 127, caUrl,
                               "sid",         UT_TYPE_STRING, 11, caSid,
                               "srvname",     UT_TYPE_STRING, 63, caSrvname,
                               "requri",      UT_TYPE_STRING, 2000, caRequir,
                               "flag",        UT_TYPE_STRING, 1, caFlag,
                               "usragent",    UT_TYPE_STRING, 255, caUsragent,
                               "eng_flag",    UT_TYPE_STRING, 8, caEng_flag,
                               "logincount",  UT_TYPE_STRING, 8, caLogincount,
                               "url",         UT_TYPE_STRING, 2048, caUrl2,
                               "usertype",    UT_TYPE_STRING, 10, caUsertype);

    //判断手机或pc版本
    pasUtlTermAttr(caUsragent, caTerm, caOs, caBro, caType);
    // printf("ua=%s,caTerm=%s,caType=%s\n",caUsragent,caTerm,caType);
    //  printf("caOs=%s\n",caOs);

    if(strcasecmp(caType, "Mobile") == 0)
    {
        lMobflag = 1;
    }
    else
    {
        lMobflag = 0;
    }
    if(strstr(caUsragent, "Mobile"))
    {
        lMobflag = 1;
    }
    //lMobflag=1;

    lIp = ntohl(pasIpcvtLong(caIp));

    // lMobflag=1;


    psDbHead = utPltInitDb();
    char caAppid[64], caSecretkey[64], caShopid[64], caSsid[64], caBssid[64];
    char caStamp[64], caAuthurl[256], caExtend[256], caSign[164];
    char caTemp[1024], caOut[1024];
    long lRoomauth = 0;
    if(strcasecmp(utComGetVar_sd(psShmHead, "StartSysJGUser", "No"), "Yes") == 0 || (strcasecmp(utComGetVar_sd(psShmHead, "roomauth", "No"), "Yes") == 0))
    {
        utPltPutVar(psDbHead, "roomauth", "");
        lRoomauth = 1;
    }
    else
    {
        utPltPutVar(psDbHead, "roomauth", "none");
    }

    strcpy(caAppid, utComGetVar_sd(psShmHead, "wxappid", "wxe59ff0ed31bd6dbf"));
    strcpy(caSecretkey, utComGetVar_sd(psShmHead, "wxsecretkey", "63fbf55ff3994b4fe1508e1cc682d604"));
    strcpy(caShopid, utComGetVar_sd(psShmHead, "wxshopid", "7480278"));
    strcpy(caSsid, utComGetVar_sd(psShmHead, "wxssid", "WXmyaaaaaa"));
    strcpy(caBssid, utComGetVar_sd(psShmHead, "wxbssid", "20:dc:e6:a2:4e:a4"));
    ncUtlGetMacFromIpMacLst(psShmHead, lIp, caMac1);
    strcpy(caMac, ncCvtMac(caMac1));
    if(strlen(caMac) == 0)
    {
        strcpy(caMac, "10:00:00:00:00:01");
    }
    ncUtlGetAuthIp(psShmHead, lIp, caIp, caPort, caPlatePath);
    sprintf(caStamp, "%d000", time(0));
    sprintf(caAuthurl, "http://%s:%s/proauth/Msg?FunName@ncLoginAuth_weixin", caIp, caPort);
    strcpy(caExtend, "aaaaaaaaaaaa");
    sprintf(caTemp, "%s%s%s%s%s%s%s%s%s", caAppid, caExtend, caStamp, caShopid, caAuthurl, caMac, caSsid, caBssid, caSecretkey);
    //    printf("caTemp=%s\n",caTemp);
    memset(caOut, 0, sizeof(caOut));

    utMd5(caTemp, strlen(caTemp), caOut);
    pasStrCvtBin2Hex(caOut, 32, caSign);
    caSign[32] = 0;
    utStrToLower(caSign);
    utPltPutVar(psDbHead, "cip", caIp);
    utPltPutVar(psDbHead, "appid", caAppid);
    utPltPutVar(psDbHead, "extend", caExtend);
    utPltPutVar(psDbHead, "timestamp", caStamp);
    utPltPutVar(psDbHead, "sign", caSign);
    utPltPutVar(psDbHead, "shopid", caShopid);
    utPltPutVar(psDbHead, "authurl", caAuthurl);
    utPltPutVar(psDbHead, "mac", caMac);
    utPltPutVar(psDbHead, "ssid", caSsid);
    utPltPutVar(psDbHead, "bssid", caBssid);


    if(utStrIsSpaces(caSid))
    {
        lSid = lIp;
    }
    else
    {
        sscanf(caSid, "%lu", &lSid);
    }
    //  printf("caIp=%s\n",caIp);
    utPltPutVar(psDbHead, "ip", caIp);
    utPltPutVar(psDbHead, "port", caPort);
    utPltPutVar(psDbHead, "adminport", utComGetVar_sd(psShmHead, "AdminPort", "80"));
    utPltPutVar(psDbHead, "siteid", utComGetVar_sd(psShmHead, "NcName", ""));

    //  ncUtlGetMacFromIpMacLst(psShmHead,lIp,caMac1);
    //    strcpy(caMac,ncCvtMac(caMac1));
    //  printf("caMac=%s\n",caMac);
    utPltPutVar(psDbHead, "mac", caMac);

    utPltPutVarF(psDbHead, "sid", "%lu", lSid);
    if(lMobflag == 1)
    {
        if(strcmp(caUsertype, "2") == 0)
        {
            if(strcmp(caEng_flag, "1") == 0)
            {
                sprintf(caPlate, "nc/weixin/mobile/english/nc_login_main_nomber.htm");
            }
            else
            {
                sprintf(caPlate, "nc/weixin/mobile/nc_login_main_nomber.htm");
            }
        }
        else
        {
            if(strcmp(caEng_flag, "1") == 0)
            {
                sprintf(caPlate, "nc/weixin/mobile/english/nc_login_main.htm");
            }
            else
            {
                sprintf(caPlate, "nc/weixin/mobile/nc_login_main.htm");
            }
        }
    }
    else
    {
        if(lRoomauth == 1)
        {
            if(strcmp(caEng_flag, "1") == 0)
            {
                sprintf(caPlate, "nc/hotel/english/sj/nc_login_main.htm");
            }
            else
            {
                sprintf(caPlate, "nc/hotel/sj/nc_login_main.htm");
            }
        }
        else
        {
            if(strcmp(caEng_flag, "1") == 0)
            {
                sprintf(caPlate, "nc/weixin/pc/english/nc_login_main.htm");
            }
            else
            {
                sprintf(caPlate, "nc/weixin/pc/nc_login_main.htm");
            }
        }
    }
    printf("caPlate=%s\n", caPlate);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);

    return 0;
}
*/


int ictGetUserInfo(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caIp[20] = "";
    char caTsid[64] = {0};
    uint8 llTsid = 0;
    char caUsragent[256] = "";
    char caEng_flag[16] = "";
    char caBssid[20] = "";
    int iReturn;
    utPltDbHead *psDbHead;
    //utMsgPrintMsg(psMsgHead);
    utMsgOutMsgToLog(PAS_SRCFILE, 1000, psMsgHead, "[ictGetUserInfo] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead, 4,
                               "tsid",  UT_TYPE_STRING,  sizeof(caTsid) - 1,   caTsid,
                               "clientip",    UT_TYPE_STRING, 19, caIp,
                               "usragent",    UT_TYPE_STRING, 255, caUsragent,
                               "eng_flag",    UT_TYPE_STRING, 8, caEng_flag);
    pasLogs(PAS_SRCFILE, 1000, "接收到caIp=[%s],caTsid=[%s]\n", caIp, caTsid);
    psDbHead = utPltInitDb();
    utPltPutVar(psDbHead, "result", "0");
    llTsid = atoll(caTsid);
    utPltPutVar(psDbHead, "mac", ncmGetMacByTsid(psShmHead, llTsid));
	
    utPltPutVar(psDbHead, "bssid", caBssid);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "wechat/userInfo.htm");
    return 0;
}



int setUsernameByTsid(utShmHead *psShmHead, uint8 llTsid, char* username)
{
    pasHashInfo sHashInfo;
    uchar *pHash = NULL;
    pHash = (unsigned char *)utShmHashHead(psShmHead, ICT_USER_LOGIN_TSID);
    if(pHash == NULL)
    {
        pasLogs(PAS_SRCFILE, 1001, "phash null!!\n");
        utShmFreeHash(psShmHead, ICT_USER_LOGIN_TSID);
        utShmHashInit(psShmHead, ICT_USER_LOGIN_TSID, 2000, 2000, sizeof(ictOnlineUser), 0, 8);

    }

    ictOnlineUser* psData = (ictOnlineUser*)utShmHashLookA(psShmHead, ICT_USER_LOGIN_TSID, (char*)(&llTsid));
    if(psData)
    {
        pasLogs(PAS_SRCFILE, 1001, "add username=[%s]\n", username);
        psData->tsid = llTsid;
        strcpy(psData->userName, username);
    }

    return 0;
}


char* getUsernameByTsid(utShmHead *psShmHead, uint8 llTsid)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    ictOnlineUser* psOnline;
    static char caUsername[64] = "";
	memset(caUsername, 0, sizeof(caUsername));
    ictOnlineUser* psData = (ictOnlineUser*)utShmHashLook(psShmHead, ICT_USER_LOGIN_TSID, (char*)(&llTsid));
    if(psData)
    {
        //psData->tsid = ltsid;
        memset(caUsername, 0, sizeof(caUsername));
        memcpy(caUsername, psData->userName, sizeof(psData->userName));
    }
    return caUsername;
}

int printAllShm(utShmHead *psShmHead)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    long lBase;
    ictOnlineUser  *psOnline;
    pHash = (unsigned char *)utShmHashHead(psShmHead, ICT_USER_LOGIN_TSID);
    pasLogs(PAS_SRCFILE, 1000, "\n\n输出所有的已认证的==========++++++++++++++++++=========\n");
    if(pHash)
    {
        lBase = utShmGetBaseAddr(psShmHead);
        psOnline = (ictOnlineUser *)pasHashFirst(pHash, &sHashInfo);
        while(psOnline)
        {
           pasLogs(PAS_SRCFILE, 1000, "username=[%s]====>tsid=%llu\n", psOnline->userName, psOnline->tsid);
            psOnline = (ictOnlineUser *)pasHashNextS(&sHashInfo);
        }
    }
    pasLogs(PAS_SRCFILE, 1000, "\n==========++++++++++++++++=========\n\n");
    return 0;
}

int deleteUsernameByTsid(utShmHead *psShmHead, uint8 llTsid)
{
	printAllShm(psShmHead);
    ictOnlineUser* psData = (ictOnlineUser*)utShmHashLook(psShmHead, ICT_USER_LOGIN_TSID, (char*)(&llTsid));
    if(psData)
    {
        pasLogs(PAS_SRCFILE, 1000, "find tsid=[%llu] and delete\n", llTsid);
        utShmHashDel(psShmHead, ICT_USER_LOGIN_TSID, psData);
    }    
    pasLogs(PAS_SRCFILE, 1000, "删除tsid=%llu的信息后\n", llTsid);
    printAllShm(psShmHead);
    return 0;
}



/* 微信验证验证  */
int ictLoginAuth_weixin(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caIp[20];
    char caPlatePath[32], caPlate[128];
    char caKey[32];
    char caMessage[128];
    int  iStatus;
    char caMac[20];
    long lCount;

    utPltDbHead *psDbHead;
    char caSid[16];
    unsigned long lUserid, lGroupid, lUseflags, lIp;
    int iReturn;
    char caDip[20], caPort[16];
    char caHost_cqga[256], caUrl_all[256], caOut[4048];
    char caGender[24], caIdtype[24], caIdnum[64], caIdnum_bin[64];
    char *p;
    pasDbCursor *psCur;
    int iNum, lSid, lStime;
    char caUsragent[256];
    long lLogin;
    char caEng_flag[16];
    char caNcName[64], caFcode[32], caMac_t[20], caTime[20], caIdfytype[16], caIdfyno[20];
    lGroupid = 0;

    long i;
    long lUexpirtime;
    char caTemp[1024], caTemp2[1024];
    char caRequrl[2560];

    char caUsertype[16];
    long lMobflag;
    char caTerm[64], caOs[64], caBro[64], caType[64];
    char caUsername_des[128], caUsername_bin[128], caUsername[128];
    char caPassword[32], caPassword_des[128], caPassword_bin[128];
    char caDispname[128], caDispname_desc[128], caDispname_bin[128];
    char caCountry[64], caAddress[600], caAddress_bin[600], caMobile[128], caMobile_bin[128], caRoom[128], caRoom_bin[128];
    char caExpiretime[32], caMac1[16];
    char caResult[32], caMesg[64];

    char caUrl_cqga[128];
    char caToken[64];
    long lWeixinlasttime;
    char caExtend[128], caOpenId[64], caTid[64];
    char caAppid[64], caSecretkey[64];
    char caServerIp[20], caServerPort[16];
    long tPort, nPort, lLasttime, lSvtime;
    char caServicecode[20], caSn[20];
    utMsgPrintMsg(psMsgHead);
    utMsgOutMsgToLog(PAS_SRCFILE, 1001, psMsgHead, "[ncLoginAuth_weixin] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead, 6,
                               "extend",    UT_TYPE_STRING, 127, caExtend,
                               "openId",    UT_TYPE_STRING, 63, caOpenId,
                               "tid",       UT_TYPE_STRING, 60, caTid,
                               "clientip",    UT_TYPE_STRING, 19, caIp,
                               "usragent",    UT_TYPE_STRING, 255, caUsragent,
                               "eng_flag",    UT_TYPE_STRING, 8, caEng_flag);
	pasLogs(2115, 1001, "接收到openid=[%s]\n", caOpenId);
	pasLogs(2115, 1001, "接收到tid=[%s]\n", caTid);
    printf("接收到openid=[%s]\n", caOpenId);
    psDbHead = utPltInitDb();
    strcpy(caToken, utComGetVar_sd(psShmHead, "WxToken", ""));
    lWeixinlasttime = utComGetVar_ld(psShmHead, "WxLasttime", 0);
    memset(caDispname, 0, sizeof(caDispname));

    utPltPutVar(psDbHead, "clientip", caIp);
    long lTime = time(0);
    if(strlen(caOpenId) > 0)
    {
        //如果公众帐号不支持手机号解密，使用OPENID作为手机号，昵称为姓名，否则使用解密的手机号作为帐号
        int ret = 0;
        char caMobile[32];
        char caAppid[128];
        strcpy(caAppid, utComGetVar_sd(psShmHead, "wxappid", "wxe59ff0ed31bd6dbf"));
        memset(caMobile, 0, sizeof(caMobile));
        ret = DecrypMobileInfo(psShmHead, caTid,
                               caAppid,
                               "",
                               caMobile);
        pasLogs(PAS_SRCFILE, 1001, "caMobile=%s,caAppid=%s", caMobile, caAppid);
        if(ret == 0 && strlen(caMobile) > 0)
        {
            strcpy(caUsername, caMobile);
            strcpy(caDispname, caOpenId);
        }
        else
        {
            //获取姓名
            if(strlen(caToken) == 0 || lTime - lWeixinlasttime > 3600)
            {
                strcpy(caAppid, utComGetVar_sd(psShmHead, "wxappid", "wxe59ff0ed31bd6dbf"));
                //从服务器获取凭证
                strcpy(caServerIp, utComGetVar_sd(psShmHead, "proauthip", "121.40.222.213"));
                strcpy(caServerPort, "7199");
                tPort = atol(caServerPort);
                lIp = utComHostAddress(caServerIp);
                nPort = ntohs(tPort);
                strcpy(caServicecode, utComGetVar_sd(psShmHead, "NcName", ""));
                strcpy(caSn, utComGetVar_sd(psShmHead, "ProductSN", ""));
                utMsgHead *psMsgHead2 = pasTcpRequest(lIp, nPort,
                                                      0,        /* CheckSum */
                                                      0,        /* Encrypt  */
                                                      "proauth_WXTokenQuery",
                                                      0,       /* Sid */
                                                      0,       /* Rid  */
                                                      60,      /* 超时  */
                                                      PAS_DEF_KEY,    /* key   */
                                                      3,
                                                      "account", UT_TYPE_STRING, caAppid,
                                                      "sn", UT_TYPE_STRING, caSn,
                                                      "servicecode", UT_TYPE_STRING, caServicecode);
                if(psMsgHead2)
                {
                    utMsgGetSomeNVar(psMsgHead2, 3, "token", UT_TYPE_STRING, 127, caToken,
                                     "lasttime", UT_TYPE_LONG, 4, &lLasttime,
                                     "svtime", UT_TYPE_LONG, 4, &lSvtime);
                    if(strlen(caToken) > 0)
                    {
                        utComSetVar_s(psShmHead, "WxToken", caToken);
                        utComSetVar_l(psShmHead, "WxLasttime", lLasttime + lTime - lSvtime);
                    }

                    utMsgFree(psMsgHead2);
                    pasLogs(PAS_SRCFILE, 1001, "从%s:%s获取到caToken=%s,lasttime=%d\n",caServerIp, caServerPort,caToken,lLasttime+lTime-lSvtime);
                }else{
					pasLogs(PAS_SRCFILE, 1001, "未从%s:%s获取到=[token]",caServerIp, caServerPort);
				}
            }
            strcpy(caUsername, caOpenId);
        }
    }

    utPltPutVar(psDbHead, "result", "0");
    utPltPutVar(psDbHead, "username", caUsername);
    pasLogs(PAS_SRCFILE, 1001, "caUsername=[%s]\n", caUsername);
    ncSrvSynUserPassword(psShmHead, 0, 0, caUsername, "123456", "\0", utComGetVar_sd(psShmHead, "DefSsid", "\0"), NCSRV_USERTYPE_WEIXIN);

    ncPortalSummary *psSumm;
    psSumm = (ncPortalSummary *)utShmArray(psShmHead, NCSRV_LNK_SYSINFO);
    int ret = ncSrvSynUserPassword2RadSrv(psSumm->psPar, caUsername, "123456", 0);
    pasLogs(PAS_SRCFILE, 1001, "\n ncSrvSynUserPassword2RadSrv ret = %d \n", ret);


    if(strlen(caExtend) > 0)
    {
        uint8 llTsid = atoll(caExtend);
        pasLogs(PAS_SRCFILE, 1001, "set ltsid=%s, Username=%s\n", caExtend, caUsername);
        //根据tsid建立一个关联到共享内存
        setUsernameByTsid(psShmHead, llTsid, caUsername);
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "wechat/userInfo.htm");

    //utStrDelSpaces(caIp);
    /* 验证  */
    /*
    lIp = ntohl(pasIpcvtLong(caIp));
    //ncUtlGetAuthIp(psShmHead, lIp, caDip, caPort, caPlatePath);
    utPltPutVar(psDbHead, "ip", caDip);
    utPltPutVar(psDbHead, "port", caPort);
    utPltPutVar(psDbHead, "adminport", utComGetVar_sd(psShmHead, "AdminPort", "80"));
    strcpy(caNcName, utComGetVar_sd(psShmHead, "NcName", ""));
    //ncUtlGetMacFromIpMacLst(psShmHead, lIp, caMac1);
    strcpy(caMac, pasCvtMac(caMac1));
    if(strlen(caUsername) == 11 && strlen(caMac) == 17)
    {
        pasDbExecSqlF("insert into ncwxmac(mac,mobile) values('%s','%s') ", caMac, caUsername);
    }
    */
    return 0;
}




int ictApproveCommit(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    int iReturn = 0;
    char caIp[20] = "";
    char caUsragent[256] = "";
    char caUsername[128] = "";
    char caTsid[64] = {0};
    char caPasTsid[64] = {0};
    uint8 llTsid = 0;
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 5,
                               "tsid",  UT_TYPE_STRING,  sizeof(caTsid) - 1,   caTsid,
                               "PASTSID", UT_TYPE_STRING,  sizeof(caPasTsid) - 1,   caPasTsid,
                               "clientip",    UT_TYPE_STRING, 19, caIp,
                               "usragent",    UT_TYPE_STRING, 255, caUsragent,
                               "username",    UT_TYPE_STRING, sizeof(caUsername) - 1, caUsername);
    utPltDbHead* psDbHead = utPltInitDb();
    if(strlen(caUsername) > 0)
    {
        if(strlen(caPasTsid) > 0)
        {
            llTsid = atoll(caPasTsid);
            deleteUsernameByTsid(psShmHead, llTsid);
        }

    }
    utPltPutVar(psDbHead, "username", caUsername);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "wechat/userInfo.htm");
    return 0;
}

int ictGetWechatLogin(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caIp[20] = "";
    char caUsragent[256] = "";
    char caEng_flag[16] = "";
    char caUsername[128] = "";
    char caTsid[64] = {0};
    char caPasTsid[64] = {0};
    utPltDbHead *psDbHead;
    int iReturn = 0;
    uint8 llTsid = 0;
    psDbHead = utPltInitDb();
    utMsgOutMsgToLog(PAS_SRCFILE, 1000, psMsgHead, "[ictGetWechatLogin] \n");
    iReturn = utMsgGetSomeNVar(psMsgHead, 5,
                               "tsid",  UT_TYPE_STRING,  sizeof(caTsid) - 1,   caTsid,
                               "PASTSID", UT_TYPE_STRING,  sizeof(caPasTsid) - 1,   caPasTsid,
                               "clientip",    UT_TYPE_STRING, 19, caIp,
                               "usragent",    UT_TYPE_STRING, 255, caUsragent,
                               "eng_flag",    UT_TYPE_STRING, 8, caEng_flag);

    if(strlen(caPasTsid) > 0)
        llTsid = atoll(caPasTsid);
    strcpy(caUsername, getUsernameByTsid(psShmHead, llTsid));
	pasLogs(PAS_SRCFILE, 1000, "tsid=[%s],PASTSID=[%s],username=[%s]\n", caTsid, caPasTsid, caUsername);
   // printf("tsid=[%s],PASTSID=[%s],username=[%s]\n", caTsid, caPasTsid, caUsername);
    if(strlen(caUsername) > 0)
    {
        utMsgPrintMsg(psMsgHead);
        pasLogs(PAS_SRCFILE, 1000, "got caUsername=[%s]\n", caUsername);
    }

    utPltPutVar(psDbHead, "username", caUsername);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "wechat/userInfo.htm");
    return 0;
}




int ictInitWebFun_weixin(utShmHead *psShmHead)
{
    int iReturn = 0;
    iReturn = pasSetTcpFunName("ictApproveCommit", ictApproveCommit, 0);
    iReturn = pasSetTcpFunName("ictGetUserInfo", ictGetUserInfo, 0);
    iReturn = pasSetTcpFunName("ictGetWechatLogin", ictGetWechatLogin, 0);
    iReturn = pasSetTcpFunName("ictLoginAuth_weixin", ictLoginAuth_weixin, 0);
    return 0;
}





