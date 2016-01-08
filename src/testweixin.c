#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <limits.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include "cjson.h"

#define DEBUG 1

/********************************************
功能：搜索字符串右边起的第一个匹配字符
********************************************/
char *Rstrchr(char *s, char x)
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
}

/**************************************************************
功能：从字符串src中分析出网站地址和端口，并得到用户要下载的文件
***************************************************************/
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
    if(DEBUG)
        printf("webhost:%s\n", host_addr);
    if(DEBUG)
        printf("hostfile:%s\n", host_file);
    if(DEBUG)
        printf("portnumber:%d\n\n", portnumber);

    if((host = gethostbyname(host_addr)) == NULL)           /*取得主机IP地址 */
    {
        if(DEBUG)
            fprintf(stderr, "Gethostname error, %s\n", strerror(errno));
        exit(1);
    }

    /* 客户程序开始建立 sockfd描述符 */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)           /*建立SOCKET连接 */
    {
        if(DEBUG)
            fprintf(stderr, "Socket Error:%s\a\n", strerror(errno));
        exit(1);
    }

    /* 客户程序填充服务端的资料 */
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portnumber);
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);

    /* 客户程序发起连接请求 */
    if(connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)            /*连接网站 */
    {
        if(DEBUG)
            fprintf(stderr, "Connect Error:%s\a\n", strerror(errno));
        exit(1);
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
    if(DEBUG)
        printf("%s", request);        /*准备request，将要发送给主机 */



    /*发送https请求request */
    send = 0;
    totalsend = 0;
    nbytes = strlen(request);
    while(totalsend < nbytes)
    {
        send = SSL_write(ssl, request + totalsend, nbytes - totalsend);
        if(send == -1)
        {
            if(DEBUG)
                ERR_print_errors_fp(stderr);
            exit(0);
        }
        totalsend += send;
        if(DEBUG)
            printf("%d bytes send OK!\n", totalsend);
    }

    //    fp = fopen(local_file, "a");
    //    if (!fp) {
    //        if (DEBUG)
    //            printf("create file error! %s\n", strerror(errno));
    //        return 0;
    //    }
    if(DEBUG)
        printf("\nThe following is the response header:\n");
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
            if(DEBUG)
                printf("%c", buffer[0]);        /*把https头信息打印在屏幕上 */
        }
        else
        {
            //            fwrite(buffer, 1, 1, fp);        /*将https主体信息写入文件 */
            //            if (DEBUG)
            //               printf("%c", buffer[0]);
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


    printf("\npCont = %s \n", pCont);
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
    if(DEBUG)
        printf("webhost:%s\n", host_addr);
    if(DEBUG)
        printf("hostfile:%s\n", host_file);
    if(DEBUG)
        printf("portnumber:%d\n\n", portnumber);

    //    if ((host = gethostbyname(host_addr)) == NULL) {        /*取得主机IP地址 */
    //        if (DEBUG)
    //            fprintf(stderr, "Gethostname error, %s\n", strerror(errno));
    //        exit(1);
    //    }
    if((host = gethostbyname(host_addr)) == NULL)           /*取得主机IP地址 */
    {
        if(DEBUG)
            fprintf(stderr, "Gethostname error, %s\n", strerror(errno));
        exit(1);
    }


    /* 客户程序开始建立 sockfd描述符 */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)           /*建立SOCKET连接 */
    {
        if(DEBUG)
            fprintf(stderr, "Socket Error:%s\a\n", strerror(errno));
        exit(1);
    }

    /* 客户程序填充服务端的资料 */
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portnumber);
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);

    /* 客户程序发起连接请求 */
    if(connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)            /*连接网站 */
    {
        if(DEBUG)
            fprintf(stderr, "Connect Error:%s\a\n", strerror(errno));
        exit(1);
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
    if(DEBUG)
        printf("%s", request);        /*准备request，将要发送给主机 */



    /*发送https请求request */
    send = 0;
    totalsend = 0;
    nbytes = strlen(request);
    while(totalsend < nbytes)
    {
        send = SSL_write(ssl, request + totalsend, nbytes - totalsend);
        if(send == -1)
        {
            if(DEBUG)
                ERR_print_errors_fp(stderr);
            exit(0);
        }
        totalsend += send;
        if(DEBUG)
            printf("%d bytes send OK!\n", totalsend);
    }


    if(DEBUG)
        printf("\nThe following is the response header:\n");
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
            if(DEBUG)
                printf("%c", buffer[0]);        /*把https头信息打印在屏幕上 */
        }
        else
        {
            if(DEBUG)
                printf("%c", buffer[0]);
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






char *getWeiXinAccessToken(char *pUrl)
{
    char caReturn[10024];
    char caUrl[1024];
    static char caToken[640];
    cJSON* pRoot;
    cJSON * pSubJson = NULL;
    memset(caToken, 0, sizeof(caToken));

    //  strcpy(caUrl,"https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=wx086f838720eb4d73&secret=208e00ea7441ef2f58eae8ba4495c2a7");

    strcpy(caUrl, "https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=wx43dfa58334f0ff7e&secret=726532587df2b5fadcb364a4e0b64bf9");

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

//获取白名单
char *getWeiXinWhiteList(char *pToken)
{
    char caReturn[10024];
    char caUrl[1024];
    static char caToken[640];
    cJSON* pRoot;
    cJSON * pSubJson = NULL;
    printf("pToken=%s\n", pToken);
    //sprintf(caUrl,"https://api.weixin.qq.com/cgi-bin/getcallbackip?access_token=%s&format=json",pToken);
    sprintf(caUrl, "https://api.weixin.qq.com/wifi/getWhiteList.xhtml?access_token=%s&format=json", pToken);
    strcpy(caReturn, getHttpsCont(caUrl));

    pasUtf8ToGBK(caReturn, caUrl, 1000);
    printf("url=%s\n", caUrl);
    printf("caReturn=%s\n", caReturn);

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



int main(int argc, char **argv)
{
    char caUrl[512];
    char caToken[600];
    char caPostVar[1024];
    strcpy(caToken, getWeiXinAccessToken(""));
    //strcpy(caToken,"oDpSow31xmyMZgTVmmv7gI4UCKYdzmp8sKt5PQ-vRegfG4oo80Ir8mGEu-HYiPsobP9KLkfXFql0GzkWazstRZyQmAtJaceWDbn8r7mAaEA");

    //strcpy(caToken,"HeLye1x9Dqj44r8KL3ybKR6M2GTiYgrHJjzBFOEfDEosOtt3oE82-Iw7qr7Jp0HPOTq9Fxel_BRpZsd_l-EqLqZW3RaPW4zavQYR1d_x7To");
    //获取白名单
    getWeiXinWhiteList(caToken);
    //获取二维码
    printf("caToken=%s\n", caToken);


    //获取门店信息
    sprintf(caUrl, "https://api.weixin.qq.com/bizwifi/shop/list?access_token=%s", caToken);
    sprintf(caPostVar, "{\"pageindex\":1,\"pagesize:\":2}");
    getHttpsConByPost(caUrl, caPostVar);


    //获取二维码信息

    sprintf(caUrl, "https://api.weixin.qq.com/bizwifi/qrcode/get?access_token=%s", caToken);

    // sprintf(caUrl,"https://api.weixin.qq.com/wifi/getQRCode.xhtml?access_token=%s&format=json",caToken);
    sprintf(caPostVar, "storeId=8052623&img_id=0");
    sprintf(caPostVar, "{\"shop_id\":8052623,\"img_id\": 1}");
    // sprintf(caPostVar,"{\"storeId\":8052623,\"ssid\": \"WXmyaaaaaa\"}");
    getHttpsConByPost(caUrl, caPostVar);


    //添加门店AP信息
    sprintf(caUrl, "https://api.weixin.qq.com/bizwifi/device/add?access_token=%s", caToken);
    cJSON * pJsonRoot = NULL;
    cJSON * pSubJson = NULL;
    // pJsonRoot = cJSON_CreateArray();
    pSubJson = cJSON_CreateObject();




    // sprintf(caPostVar,"{\"storeId\":8052623,\"ssid\": \"WXmyaaaaaa\"}");

    cJSON_AddNumberToObject(pSubJson, "shop_id", 8052623);
    cJSON_AddStringToObject(pSubJson, "ssid", "WXmyaaaaaa");
    cJSON_AddStringToObject(pSubJson, "password", "13065532775");
    cJSON_AddStringToObject(pSubJson, "bssid", "10:dc:e6:a2:4e:a4");

    //  cJSON_AddItemToArray(pJsonRoot,pSubJson);
    char * p = cJSON_Print(pSubJson);
    sprintf(caPostVar, "%s", p);

    getHttpsConByPost(caUrl, caPostVar);


    free(p);
    cJSON_Delete(pSubJson);

    //获取公众号联网信息
    sprintf(caUrl, "https://api.weixin.qq.com/bizwifi/account/get_connecturl?access_token=%s", caToken);
    strcpy(caPostVar, "");
    getHttpsConByPost(caUrl, caPostVar);


    //设置商家模首页模板板
    sprintf(caUrl, "https://api.weixin.qq.com/bizwifi/homepage/set?access_token=%s", caToken);
    pJsonRoot = NULL;
    pSubJson = NULL;
    pJsonRoot = cJSON_CreateObject();
    cJSON_AddNumberToObject(pJsonRoot, "shop_id", 8052623);
    cJSON_AddNumberToObject(pJsonRoot, "template_id", 1);

    pSubJson = cJSON_CreateObject();
    cJSON_AddStringToObject(pSubJson, "url", "http://pronetway.com");

    cJSON_AddItemToObject(pJsonRoot, "struct", pSubJson);
    p = cJSON_Print(pJsonRoot);
    sprintf(caPostVar, "%s", p);
    getHttpsConByPost(caUrl, caPostVar);

    free(p);
    cJSON_Delete(pJsonRoot);

    //获取用户列表

    sprintf(caUrl, "https://api.weixin.qq.com/cgi-bin/user/get?access_token=%s", caToken);
    getHttpsCont(caUrl);

    //获取用户基本信息

    sprintf(caUrl, "https://api.weixin.qq.com/cgi-bin/user/info?access_token=%s&openid=%s", caToken, "oUw_Cjg9kwlkqjqeMA5qXQfQsYMc");
    printf("%s\n", getHttpsCont(caUrl));


    /*
    //设置WI-FI二维码连网信息
     sprintf(caUrl,"https://api.weixin.qq.com/wifi/setConnectInfo.xhtml?access_token=%s&format=json",caToken);
      sprintf(caPostVar,"ssid=WXmyaaaaaa&storeId=8052623&mpAppId=gh_8df4367b0e47");

     // sprintf(caPostVar,"{\"storeId\":8052623,\"ssid\": \"WXmyaaaaaa\"}");
      getHttpsConByPost("https://api.weixin.qq.com/wifi/getQRCode.xhtml?access_token=oDpSow31xmyMZgTVmmv7gI4UCKYdzmp8sKt5PQ-vRegfG4oo80Ir8mGEu-HYiPsobP9KLkfXFql0GzkWazstRZyQmAtJaceWDbn8r7mAaEA&format=json",caPostVar);
    */


    //通知微信登录成功
    //  strcpy(caUrl,"https://api.weixin.qq.com/wifi/noticeWeixinInfo.xhtml?access_token=%s&action=loginSuccess&mac=10:11:11:22:11:10&format=json");
    //  getHttpsCont(caUrl);




    return 0;
}