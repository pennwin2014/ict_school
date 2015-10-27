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

#define DEBUG 0

/********************************************
���ܣ������ַ����ұ���ĵ�һ��ƥ���ַ�
********************************************/
char *Rstrchr(char *s, char x)
{
    int i = strlen(s);
    if (!(*s))
        return 0;
    while (s[i - 1])
        if (strchr(s + (i - 1), x))
            return (s + (i - 1));
        else
            i--;
    return 0;
}

/**************************************************************
���ܣ����ַ���src�з�������վ��ַ�Ͷ˿ڣ����õ��û�Ҫ���ص��ļ�
***************************************************************/
void GetHost(char *src, char *web, char *file, int *port)
{
    char *pA;
    char *pB;
    memset(web, 0, sizeof(web));
    memset(file, 0, sizeof(file));
    *port = 0;
    if (!(*src))
        return;
    pA = src;
    if (!strncmp(pA, "http://", strlen("http://")))
        pA = src + strlen("http://");
    else if (!strncmp(pA, "https://", strlen("https://")))
        pA = src + strlen("https://");
    pB = strchr(pA, '/');
    if (pB) {
        memcpy(web, pA, strlen(pA) - strlen(pB));
        if (pB + 1) {
            memcpy(file, pB + 1, strlen(pB) - 1);
            file[strlen(pB) - 1] = 0;
        }
    } else
        memcpy(web, pA, strlen(pA));
    if (pB)
        web[strlen(pA) - strlen(pB)] = 0;
    else
        web[strlen(pA)] = 0;
    pA = strchr(web, ':');
    if (pA)
        *port = atoi(pA + 1);
    else
        *port = 443;
}

/************�ĵ�********************************************
*filename: ncmcomhttps.c
*purpose: ��ʾHTTPS�ͻ��˱�̷���
gcc -Wall https-client.c -lssl -o httpsclient

���д˳�����ȡ��HTTPS�������ϵ�ҳ�棬���磺
./httpsclient https://127.0.0.1/test.html

�ؼ�֮�����ڽ���socket֮���SSL��س�ʼ���Լ��м��recv/send��SSL_read��SSL_write���棬���ǵ��ͷ�SSL��Դ���ɡ�

*********************************************************************/

int main(int argc, char *argv[])
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
    char request[1024];
    int send, totalsend;
    int i;
    char *pt;
    SSL *ssl;
    SSL_CTX *ctx;
    
    char pCont[10024] = "",tmpch[4] = "";
    if (argc != 2) {
        if (DEBUG)
            fprintf(stderr, "Usage:%s webpage-address\a\n", argv[0]);
        exit(1);
    }
    if (DEBUG)
        printf("parameter.1 is: %s\n", argv[1]);
    strcpy(argv[1],"https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=wx086f838720eb4d73&secret=1ba4edc4bc8ebff3541961fc0785c8f8");
    GetHost(argv[1], host_addr, host_file, &portnumber);        /*������ַ���˿ڡ��ļ����� */
    if (DEBUG)
        printf("webhost:%s\n", host_addr);
    if (DEBUG)
        printf("hostfile:%s\n", host_file);
    if (DEBUG)
        printf("portnumber:%d\n\n", portnumber);

    if ((host = gethostbyname(host_addr)) == NULL) {        /*ȡ������IP��ַ */
        if (DEBUG)
            fprintf(stderr, "Gethostname error, %s\n", strerror(errno));
        exit(1);
    }

    /* �ͻ�����ʼ���� sockfd������ */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {        /*����SOCKET���� */
        if (DEBUG)
            fprintf(stderr, "Socket Error:%s\a\n", strerror(errno));
        exit(1);
    }

    /* �ͻ�����������˵����� */
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portnumber);
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);

    /* �ͻ��������������� */
    if (connect(sockfd, (struct sockaddr *) (&server_addr), sizeof(struct sockaddr)) == -1) {        /*������վ */
        if (DEBUG)
            fprintf(stderr, "Connect Error:%s\a\n", strerror(errno));
        exit(1);
    }

    /* SSL��ʼ�� */
    SSL_library_init();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    ssl = SSL_new(ctx);
    if (ssl == NULL) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    /* ��socket��SSL���� */
    ret = SSL_set_fd(ssl, sockfd);
    if (ret == 0) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    RAND_poll();
    while (RAND_status() == 0) {
        unsigned short rand_ret = rand() % 65536;
        RAND_seed(&rand_ret, sizeof(rand_ret));
    }

    ret = SSL_connect(ssl);
    if (ret != 1) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    sprintf(request, "GET /%s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\n\
User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n\
Host: %s:%d\r\nConnection: Close\r\n\r\n", host_file, host_addr,
            portnumber);
    if (DEBUG)
        printf("%s", request);        /*׼��request����Ҫ���͸����� */

    /*ȡ����ʵ���ļ��� */
//    if (host_file && *host_file)
//        pt = Rstrchr(host_file, '/');
//    else
//        pt = 0;
//
//    memset(local_file, 0, sizeof(local_file));
//    if (pt && *pt) {
//        if ((pt + 1) && *(pt + 1))
//            strcpy(local_file, pt + 1);
//        else
//            memcpy(local_file, host_file, strlen(host_file) - 1);
//    } else if (host_file && *host_file)
//        strcpy(local_file, host_file);
//    else
//        strcpy(local_file, "index.html");
//    if (DEBUG)
//        printf("local filename to write:%s\n\n", local_file);

    /*����https����request */
    send = 0;
    totalsend = 0;
    nbytes = strlen(request);
    while (totalsend < nbytes) {
        send = SSL_write(ssl, request + totalsend, nbytes - totalsend);
        if (send == -1) {
            if (DEBUG)
                ERR_print_errors_fp(stderr);
            exit(0);
        }
        totalsend += send;
        if (DEBUG)
            printf("%d bytes send OK!\n", totalsend);
    }

//    fp = fopen(local_file, "a");
//    if (!fp) {
//        if (DEBUG)
//            printf("create file error! %s\n", strerror(errno));
//        return 0;
//    }
    if (DEBUG)
        printf("\nThe following is the response header:\n");
    i = 0;
    /* ���ӳɹ��ˣ�����https��Ӧ��response */
    while ((nbytes = SSL_read(ssl, buffer, 1)) == 1) {
        if (i < 4) {
            if (buffer[0] == '\r' || buffer[0] == '\n')
                i++;
            else
                i = 0;
            if (DEBUG)
                printf("%c", buffer[0]);        /*��httpsͷ��Ϣ��ӡ����Ļ�� */
        } else {
//            fwrite(buffer, 1, 1, fp);        /*��https������Ϣд���ļ� */
            if (DEBUG)
               printf("%c", buffer[0]);
            sprintf(tmpch,"%c",buffer[0]);
            if (i == 4)
            {
               strcpy(pCont,tmpch);	
            }
            else
            {
               strcat(pCont,tmpch);	
            }		
            i++;
//            if (i % 1024 == 0)
//                fflush(fp);        /*ÿ1Kʱ����һ�� */
        }
    }
    printf("\npCont = %s \n",pCont);
//    fclose(fp);
    /* ����ͨѶ */
    ret = SSL_shutdown(ssl);
    if (ret != 1) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    close(sockfd);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    ERR_free_strings();
    exit(0);
}
