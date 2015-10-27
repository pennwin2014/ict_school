#ifndef __PRORADIUSDEF__
#define __PRORADIUSDEF__
#define PRORAD_VERSION         "V1.00.02"
#define PRORAD_RELEASE         "2013/10/14"

#define PRORAD_LNK_RADCLIENT   60                      /* 客户端信息      */

typedef struct proRadClient_s {
    uchar  caName[32];                     // 名称
    uchar  caKey[32];                      // 通信秘钥
    uint4  lNum;                           // 序号
    uint4  lIp;                            // IP地址，若为0表示任意IP地址
} proRadClient;

typedef struct proRadHead_s {
    uint4 lMaxClient;
    uint4 lSumClient;
    proRadClient  *psClient;
} proRadHead;


#endif