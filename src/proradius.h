#ifndef __PRORADIUSDEF__
#define __PRORADIUSDEF__
#define PRORAD_VERSION         "V1.00.02"
#define PRORAD_RELEASE         "2013/10/14"

#define PRORAD_LNK_RADCLIENT   60                      /* �ͻ�����Ϣ      */

typedef struct proRadClient_s {
    uchar  caName[32];                     // ����
    uchar  caKey[32];                      // ͨ����Կ
    uint4  lNum;                           // ���
    uint4  lIp;                            // IP��ַ����Ϊ0��ʾ����IP��ַ
} proRadClient;

typedef struct proRadHead_s {
    uint4 lMaxClient;
    uint4 lSumClient;
    proRadClient  *psClient;
} proRadHead;


#endif