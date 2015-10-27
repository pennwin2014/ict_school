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
#define   NC_LNK_IPPKG      31

#include "ncmweb.h"

#include <net/if.h>
#include <sys/ioctl.h>	

//获取网卡数量
int GetNetCardCount()
{
   int nCount = 0,i = 0 , resCou = 0 , j = 0 ;
   typedef struct{
   	  char devinfo[512];
   }netcard_s;
   netcard_s netcard[32]; 
   FILE *f = fopen("/proc/net/dev","r");
   if(!f)
   {
       fprintf(stderr,"Open /proc/net/dev failed! errno:%d \n",errno);
       return nCount;
   }
   
   char szLine[512];
   fgets(szLine,sizeof(szLine),f);
   fgets(szLine,sizeof(szLine),f);
   
   while (fgets(szLine,sizeof(szLine),f))
   {
       char szName[128]={0};
       sscanf(szLine,"%s",szName);
       printf("szName=%s \n",szName);
       int nLen=strlen(szName);
       if (nLen <= 0)
       {
          continue;
       }
       if (szName[nLen - 1] == ':')
       {
         szName[nLen - 1] = 0;
       }
//       if (strcmp(szName,"lo") == 0)
       if (strstr(szName,"lo:") )
       {
         continue; 
       }
       strcpy(netcard[nCount].devinfo,szName);
       nCount++;
   }
   char ethchar[8]="";
   for (i = 0 ;i < nCount ;i++)
   {
       sprintf(ethchar,"eth%d:",i);
       for (j = 0 ;j < nCount ;j++)
       {
	       if (strstr(netcard[j].devinfo,ethchar) )
	       {
	         resCou++;
	       }
   	   } 
//   	   printf("==== 00 ===ethchar=%s \n",ethchar);
	     memset(ethchar,0,sizeof(ethchar));
   }
   fclose(f);
   f=NULL;
   return resCou;
}
//获取网卡IP 子网掩码 广播地址
void DispNetInfo(const char * szDevName,int m,int devCou,netdev_s * netdev)
{   
    int s = socket (AF_INET,SOCK_DGRAM,0);
    if(s < 0)
    {
       fprintf(stderr,"Create socket failed! errno=%d",errno);
       return;
    }
    
    struct ifreq ifr;
    unsigned char mac[6];
    unsigned long nIP,nNetmask,nBroadIP;
    printf("%s : \n",szDevName);
    
    strcpy(ifr.ifr_name,szDevName);
    if (ioctl(s,SIOCGIFHWADDR,&ifr) < 0)
    {
        return;
    }
    memcpy(mac,ifr.ifr_hwaddr.sa_data,sizeof(mac));
    printf("\t MAC: %02x-%02x-%02x-%02x-%02x-%02x \n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    
    sprintf(netdev[m].mac," %02x-%02x-%02x-%02x-%02x-%02x ",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

    strcpy(ifr.ifr_name,szDevName);
    if(ioctl(s,SIOCGIFADDR,&ifr) < 0)
    {
       nIP = 0;
    }
    else
    {
       nIP = *(unsigned long*)&ifr.ifr_broadaddr.sa_data[2];
    }
    printf("\t IP: %s \n",inet_ntoa(*(struct in_addr*)&nIP));
    
    strcpy(netdev[m].ip,inet_ntoa(*(struct in_addr*)&nIP));
    
    strcpy(ifr.ifr_name,szDevName);
    if (ioctl(s,SIOCGIFBRDADDR,&ifr) < 0)
    {
        nBroadIP = 0;
    }
    else 
    {
        nBroadIP = *(unsigned long *)&ifr.ifr_broadaddr.sa_data[2];
    }                                     //c++里可以不加struct
    printf("\t BroadIP: %s \n",inet_ntoa(*(struct in_addr*)&nBroadIP));
    
    strcpy(netdev[m].broadip,inet_ntoa(*(struct in_addr*)&nBroadIP));
    
    strcpy(ifr.ifr_name,szDevName);
    if (ioctl(s,SIOCGIFNETMASK,&ifr) < 0)
    {
        nNetmask = 0;
    }
    else
    {
        nNetmask = *(unsigned long *)&ifr.ifr_netmask.sa_data[2];
    }
    printf("\n Netmask: %s \n",inet_ntoa(*(struct in_addr*)&nNetmask));
    
    strcpy(netdev[m].netmask,inet_ntoa(*(struct in_addr*)&nNetmask));
    close(s);
//    return netdev;
}

#include <asm/types.h>
#include <netinet/ether.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>


#define GATEWAYBUFSIZE 8192
struct route_info{
    u_int dstAddr;
    u_int srcAddr;
    u_int gateWay;
    char ifName[IF_NAMESIZE];
};
int readNlSock(int sockFd, char *bufPtr, int seqNum, int pId)
{
    struct nlmsghdr *nlHdr;
    int readLen = 0, msgLen = 0;
    do{
      /* Recieve response from the kernel */
        if((readLen = recv(sockFd, bufPtr, GATEWAYBUFSIZE - msgLen, 0)) < 0)
        {
            perror("SOCK READ: ");
            return -1;
        }

        nlHdr = (struct nlmsghdr *)bufPtr;
        /* Check if the header is valid */
        if((NLMSG_OK(nlHdr, readLen) == 0) || (nlHdr->nlmsg_type == NLMSG_ERROR))
        {
            perror("Error in recieved packet");
            return -1;
        }

        /* Check if the its the last message */
        if(nlHdr->nlmsg_type == NLMSG_DONE)
        {
            break;
        }
        else
        {
            /* Else move the pointer to buffer appropriately */
            bufPtr += readLen;
            msgLen += readLen;
        }

        /* Check if its a multi part message */
        if((nlHdr->nlmsg_flags & NLM_F_MULTI) == 0)
        {
        /* return if its not */
            break;
        }
    } while((nlHdr->nlmsg_seq != seqNum) || (nlHdr->nlmsg_pid != pId));
    return msgLen;
}
/* For printing the routes. */
void printRoute(struct route_info *rtInfo)
{
    char tempBuf[512];

    /* Print Destination address */
    if(rtInfo->dstAddr != 0)
//        strcpy(tempBuf, (char *)inet_ntoa(rtInfo->dstAddr));
        strcpy(tempBuf, (char *)inet_ntoa(*(struct in_addr*)&rtInfo->dstAddr));           //zms
    else
        sprintf(tempBuf,"*.*.*.*\t");
    fprintf(stdout,"%s\t", tempBuf);

    /* Print Gateway address */
    if(rtInfo->gateWay != 0)
        strcpy(tempBuf, (char *)inet_ntoa(*(struct in_addr*)&rtInfo->gateWay));        //zms
    else
        sprintf(tempBuf,"*.*.*.*\t");
    fprintf(stdout,"%s\t", tempBuf);

    /* Print Interface Name*/
    fprintf(stdout,"%s\t", rtInfo->ifName);

    /* Print Source address */
    if(rtInfo->srcAddr != 0)
        strcpy(tempBuf, (char *)inet_ntoa(*(struct in_addr*)&rtInfo->srcAddr));
    else
        sprintf(tempBuf,"*.*.*.*\t");
    fprintf(stdout,"%s\n", tempBuf);
}

/* For parsing the route info returned */
void parseRoutes(struct nlmsghdr *nlHdr, struct route_info *rtInfo,char *gateway)
{
    struct rtmsg *rtMsg;
    struct rtattr *rtAttr;
    int rtLen;
    char *tempBuf = NULL;

    tempBuf = (char *)malloc(100);
    rtMsg = (struct rtmsg *)NLMSG_DATA(nlHdr);
    /* If the route is not for AF_INET or does not belong to main routing table
    then return. */
    if((rtMsg->rtm_family != AF_INET) || (rtMsg->rtm_table != RT_TABLE_MAIN))
        return;

   
    /* get the rtattr field */
    rtAttr = (struct rtattr *)RTM_RTA(rtMsg);
    rtLen = RTM_PAYLOAD(nlHdr);
    for(;RTA_OK(rtAttr,rtLen);rtAttr = RTA_NEXT(rtAttr,rtLen))
    {
        switch(rtAttr->rta_type)
        {
            case RTA_OIF:
            if_indextoname(*(int *)RTA_DATA(rtAttr), rtInfo->ifName);
            break;
            
            case RTA_GATEWAY:
            rtInfo->gateWay = *(u_int *)RTA_DATA(rtAttr);
            break;

            case RTA_PREFSRC:
            rtInfo->srcAddr = *(u_int *)RTA_DATA(rtAttr);
            break;

            case RTA_DST:
            rtInfo->dstAddr = *(u_int *)RTA_DATA(rtAttr);
            break;
        }
    }
    if (strstr(((char *)inet_ntoa(*(struct in_addr*)&rtInfo->dstAddr)), "0.0.0.0"))                //zms
        sprintf(gateway, (char *)inet_ntoa(*(struct in_addr*)&rtInfo->gateWay));
    //printRoute(rtInfo);
    free(tempBuf);
    return;
}
//获取网关地址
int get_gateway(char *gateway)
{
    struct nlmsghdr *nlMsg;
    struct rtmsg *rtMsg;
    struct route_info *rtInfo;
    char msgBuf[GATEWAYBUFSIZE];

    int sock, len, msgSeq = 0;
    char buff[1024];


    /* Create Socket */
    if((sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) == 0)                //zms
    perror("Socket Creation: ");

    /* Initialize the buffer */
    memset(msgBuf, 0, GATEWAYBUFSIZE);

    /* point the header and the msg structure pointers into the buffer */
    nlMsg = (struct nlmsghdr *)msgBuf;
    rtMsg = (struct rtmsg *)NLMSG_DATA(nlMsg);

    /* Fill in the nlmsg header*/
    nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)); // Length of message.
    nlMsg->nlmsg_type = RTM_GETROUTE; // Get the routes from kernel routing table .

    nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST; // The message is a request for dump.
    nlMsg->nlmsg_seq = msgSeq++; // Sequence of the message packet.
    nlMsg->nlmsg_pid = getpid(); // PID of process sending the request.

    /* Send the request */
    if(send(sock, nlMsg, nlMsg->nlmsg_len, 0) == 0){  //zms
        printf("Write To Socket Failed...\n");
        return -1;
    }

    /* Read the response */
    if((len = readNlSock(sock, msgBuf, msgSeq, getpid())) == 0) //zms
    {
        printf("Read From Socket Failed...\n");
        return -1;
    }
    /* Parse and print the response */
    rtInfo = (struct route_info *)malloc(sizeof(struct route_info));

    for(;NLMSG_OK(nlMsg,len);nlMsg = NLMSG_NEXT(nlMsg,len))
    {
        memset(rtInfo, 0, sizeof(struct route_info));
        parseRoutes(nlMsg, rtInfo,gateway);
    }
    free(rtInfo);
    close(sock);
    return 0;
}

