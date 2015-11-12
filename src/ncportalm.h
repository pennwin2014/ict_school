/* ProPotral     ��Ȩ����: �Ϻ���������Ϣ�������޹�˾ 2013
   
*/
#ifndef __PROPORTALDEF__
#define __PROPORTALDEF__
#define PORTAL_VERSION         "V1.00.04"
#define PORTAL_RELEASE         "2013/11/07"
#define NCPortalACHeadLen           16
#define ACPORTAL_REQ_CHALLENGE      0x01
#define ACPORTAL_ACK_CHALLENGE      0x02
#define ACPORTAL_REQ_AUTH           0x03
#define ACPORTAL_ACK_AUTH           0x04
#define ACPORTAL_REQ_LOGOUT         0x05
#define ACPORTAL_ACK_LOGOUT         0x06
#define ACPORTAL_AFF_ACK_AUTH       0x07
#define ACPORTAL_NTF_LOGOUT         0x08
#define ACPORTAL_REQ_INFO           0x09
#define ACPORTAL_ACK_INFO           0x0a

#define  NCSRV_LNK_PROCESS     47

#define  NCSRV_LNK_ONTIME      50                      /* ��ʱ����               */
#define  NCSRV_LNK_ONLINE      51
#define  NCSRV_LNK_USERS       53                      /* ���߿ͻ�����Ϣ         */
#define  NCSRV_LNK_SYSINFO     54                      /* ���ϵͳ��ǰ������Ϣ   */
#define  NCSRV_LNK_PLATE       55                      /* ģ����  */

#define  PORTAL_LNK_SYSINFO     56
#define  PORTAL_LNK_ACINFO      57                     // AC�����Ϣ
#define  PORTAL_LNK_CLIENTS     58                     // �ͻ�����Ϣ
#define  PORTAL_LNK_BLACKUSER   59                     // �˺ź�����
#define  PORTAL_LNK_USERMAC     60                     // �û�Mac��ַ���ձ�
#define  PORTAL_LNK_PORTALRULE  61                     // Portalʹ�ò���



#define  NCSRV_STA_SUCCESS     0                       /* ��ȷ              */
#define  NCSRV_STA_RELOGIN     10001                   /* �û���Ҫ���µ�¼  */
#define  NCSRV_STA_NOEXIST     10002                   /* �û�������        */
#define  NCSRV_STA_KEYERROR    10003                   /* �������          */


#define  NCSRV_PORTAL_PROEIM      1
#define  NCSRV_PORTAL_RADIUS      2
#define  NCSRV_PORTAL_PORTAL      3
#define  NCSRV_PORTAL_AC          4

#define  NCSRV_SMSTYPE_PRONETWAY  1              // �����̶�������
#define  NCSRV_SMSTYPE_SMSMODEM   2              // ����è


#define  NCSRV_MAXAC             64
// ��Portalͨ�ŵĿͻ���
typedef struct ncPortalClient_s {
    int4   iType;                          // ���  1--ProEIM   2--Radius
    uchar  caName[32];                     // ����
    uchar  caKey[32];                      // ͨ����Կ
    uint4  lNum;                           // ���
    uint4  lIp;                            // IP��ַ����Ϊ0��ʾ����IP��ַ
} ncPortalClient;



typedef struct ncPortalACHead_s {
        uchar   Ver;                  // �汾��  0x01
        uchar   Type;                 //  0x01 -- REQ_CHALLENGE   0x02 -- ACK_CHALLENGE   0x03 ---REQ_AUTH  0x04 ---ACK_AUTH 
        uchar   PapChap;              // Chap: 0x00  Pap: 0x01
        uchar   Rsv;                  // ����: 0
        uint2   Sn;                   // �ͻ����������
        uint2   ReqId;
        uint4   UserIp;
        uint2   UserPort;
        uchar   ErrCode;
        uchar   AttrNum;
        char    caBuf[256];
    } ncPortalAcHead;


#define ACPORTAL_ATTRTYPE_USERNAME       0x01
#define ACPORTAL_ATTRTYPE_PASSWORD       0x02
#define ACPORTAL_ATTRTYPE_CHALLENGE      0x03
#define ACPORTAL_ATTRTYPE_CHAPPASSWORD   0x04

 // AC ���ձ�
typedef struct ncPortalACAttr_s {
        uchar   Type;
        uchar   AttrLen;
        uchar   Value[254];
} ncPortalACAttr;


// User Agent ��Ϣ�ɼ�
typedef struct ncPortalUseAgent_s {
    char    caKey[32];                 // �ؼ���
    char    caName[32];                // ����
    char    caDevType[8];              // �豸����
    uchar   cType;                     // ���  1--TermType    2--OS    3--Bro
    uchar   cPr;                       // ����Խ��Խ����
    uchar   cRev[2];
}  ncPortalUseAgent;

typedef struct ncPortalAcInfo_s {
    uchar caName[32];                // AC����
    uint4 lAcip;                     // AC IP��ַ
    uint2 nAcPort;                   // AC �˿�
    uint2 nSn;                       // ���к�, ��������
    uchar caAcType[8];               // AC���  Cisco   GbCom
    uint4 iType;                     // NCSRV_PORTAL_AC NCSRV_PORTAL_RADIUS
    uint4 lLastTime;
    uint4 lCount;
    uchar caKey[32];
} ncPortalAcInfo;

#define NCPORTAL_VENDOR_GBCOM         1
#define NCPORTAL_VENDOR_CISCO         2

// Portal�û���Ϣ�� Hash����ʼ��ʱ�����ڴ���
// һ���ʺŶ�Ӧ���Mac��ַ
#define NCPORTAL_USRFROM_MANUAL       1                     // �ֹ�����
#define NCPORTAL_USRFROM_SMS          2                     // ���Զ��Ż�ȡ
#define NCPORTAL_ONEUSER_MAXMAC       5
typedef struct ncPortalUser_s {
    uchar caName[32];               // ��½��
    uchar caDisp[32];               // ��ʾ��
    uchar caPass[32];               // ����
    uchar caGroup[32];
    uchar caMac[32];                // �󶨵�Mac��ַ
    uchar caLabel[32];
    uint4 lUserid;
    uint4 lLasttime;                // ���һ�ε�¼ʱ��
    uint4 lModTime;                 // �����޸�ʱ��
    uint4 lStarttime;                // �˺�����ʱ��
    uchar cDb;                      // 0--Db���޼�¼    1--Db�д��ڼ�¼
    uchar cMod;                     // ���¼�¼ 
    uchar cFlags;                   // ��¼��Դ 
    uchar cStatus;                  // ״̬ 0--����  1--��ʱ����
    uchar cRev[4];
} ncPortalUser;

// �û���Mac��ַ���ձ�
typedef struct ncPortalUserMac_s {
    uchar mac[6];                 // Mac��ַ
    uchar cStatus;                  // 0-��Ч  1--��Ч
    uchar cDb;                      // 0-���ݿ��в�����  1--����  9-��Ч��MAC
    uchar cMod;                     // 0--��¼δ�޸�  1--��¼���޸�
    uchar cRev[3];
    uchar caName[32];               // ��½��
    uint4 starttime;                // ʧЧʱ��
    uint4 lasttime;
    ncPortalUser *psUser;
} ncPortalUserMac;


#define NCPORTAL_BLACK_MAC          1
#define NCPORTAL_BLACK_USER         2

// �û�������
typedef struct ncPortalBlackUser_s {
    uchar ctype;                    // ��� 1-MAC  2-Uername
    uchar caName[31];               // ��½����Mac��ַ
    uint4 validtime;                // ʧЧʱ��
} ncPortalBlackUser;



// Portal�����û�, ��IP��ַΪ����
typedef struct ncPortalOnline_s {
    uint4 lSip;                     // IP��ַ
    uint4 starttime;                // ����ʱ��
    uchar caName[32];               // ��½��
    uchar caSsid[32];               // SSID
    uchar caApName[32];             // AP���ƻ�MAC��ַ
    uchar caAcName[28];             // Ac���ƻ�Ip��ַ
    uint4 lAcIp;                    // AC��ַ
    uint2 nAcPort;                  // AC�˿�
    uchar caMac[6];                 // Mac��ַ
    uint2 nReqId;                   // ��֤��
    uint2 nSn;                      // ���к�
    uchar caDev[24];                // �豸
    uchar caOs[16];                 // ����ϵͳ
    uchar caTermType[8];            // �豸���
    uint4 sessiontime;              // ����ʱ��
    uint8 lBytes[2];                // ����
    uchar caLang[8];                // ����
    uint4 lasttime;                 // ������ʱ��
    uchar caPostUrl[64];            // Post Url https://192.168.20.38/pronline/Msg
    uchar caLabel[32];              
    uchar login;                    // 0--Disp, ��ʾ��¼����   1--Login, �Ѿ���¼
    uchar cStatus;                  // 0--����  1--������
    uchar cFrom;                    // ��Դ����
    uchar cSave;                    // 0--δ����   1--����
    ncPortalUser    *psUser;
#ifndef PAS_X64
    uchar caRev2[4];
#endif    
} ncPortalOnline;







// ����ҳ��ģ��
typedef struct ncPortalPlate_s {
    uchar caName[16];               // ����
    uchar caTerm[8];                // �ն�
    uchar caLang[8];                // ����
    uchar caPlate[128];             // ģ��
    uint4 lCount;                   // �������
    uint4 lLastTime;                // ������ʱ��
    int4  iCode;                    // ʹ���ַ���
} ncPortalPlate;

// ���ŷ�����Ϣ
typedef struct ncPortalIp_s {
    uint4 lIp;            // ��ַ
    uint2 nPort;          
    uint2 nType;
    uchar caName[32];            
    uint4 lCount;         // ���ʹ���
    uint4 lError;         // ���ɹ�����
    uint4 lLastTime;      // ���ͨ��ʱ��
} ncPortalIpPort;

// Portalͳ����Ϣ
typedef struct ncPortalCount_s {
    uint4 lTime;          // ���ݲɼ�ʱ��
    uint4 lSmsCount;      // ���ŷ��ʹ���
    uint4 lDispCount;     // ��֤��ʾ
    uint4 lLoginOk;       // ��¼ҳ��
    uint4 lLoginError;    // ��¼����
    uint4 lLogOut;        // �˳�
    uint4 lAutoLog;       // �Զ���¼
    uint4 lOnlineUser;    // �����û���
    uint4 lRegUser;       // ������ע���û�
    uint8 lBytes;         // ����
    uint4 lRev[4];
} ncPortalCount;

typedef struct ncPortalSmsMsg_s {
    char caLang[8];             // ����
    char caMsg[128];            // ��Ϣ��ʽ [#username#]  [#password#]
} ncPortalSmsMsg;

// ��֤�û�, һ���������룬���ڻ�����  1--��Radiusͬ�� 2--���������ݿ���  3--����ά������
typedef struct ncPortalAuthUser_s {
    char    caUsername[16];        // ��¼��
    char    caPassword[16];        // ��¼����
    uint4   lTime;                 // ʱ��
} ncPortalAuthUser;


// Portal �ۺ���Ϣ
#define NC_PORTAL_MAXLANG        8
#define NC_PORTAL_MAXSMS         8
#define NC_PORTAL_MAXEIM         8
#define NC_PORTAL_MAXCOUNT     120     // �ڴ��еĲɼ�����
#define NC_PORTAL_MAXRADIUS      8     // Radius����
typedef struct ncPortalSummary_s {
    uint4 lStartTime;                  // ����ʱ��
    uint4 lLastTime;                   // ���ʱ��
    uint2 nSumLang;                    // ��������
    uint2 nSumSms;                     // Sms����������
    uint2 nCurSms;                     // ��ǰSMS���
    uint2 nCurCount;                   // ͳ�Ƶ�ǰλ��
    uint2 nSumRadius;                  // Radius����������
    uint2 nCurRadius;                  // ��ǰRadius���
    uint2 nSumEim;                     // ����豸����
    uint2 nCurEim;                     // ��ǰEIM
    uint4 lUserMacExpire;              // �û�Mac��ַ��ʧЧʱ��
    uint4 lPassCodeTimeOut;            // ������Чʱ��
    ncPortalSmsMsg  psSmsMsg[NC_PORTAL_MAXLANG];
    ncPortalIpPort  psSmsServer[NC_PORTAL_MAXSMS];    // ��������
    ncPortalIpPort  psRadServer[NC_PORTAL_MAXRADIUS];  // Radius��Ϣ
    ncPortalIpPort  psEimServer[NC_PORTAL_MAXEIM];     // ���綽����Ϣ
    ncPortalCount   psPortalCount[NC_PORTAL_MAXCOUNT];  // Portalͳ������
} ncPortalSummary;

// ����Portal��Ϣ
typedef struct ncsPortalOnline_s {
    uchar caName[32];                  // Portal����
    uint4 lStartTime;                  // ����ʱ�䰮��
    uint4 lLastTime;                   // ���ͨ��ʱ��
    uint4 lIp;                         // IP��ַ
    uint2 nCurCount;                   // ��ǰͳ�����
    ncPortalCount  psPortalCount[NC_PORTAL_MAXCOUNT];
} ncsPortalOnline;


// ����Ͷ�Ų��ԣ����Խ���ÿ5���Ӽ��һ�Σ�ȥ����ʱ�ģ������µģ������Ч��
//  1--Position  2--SSid 
typedef struct ncPortalRule_s {
    uchar   cType;                      // ���   1-SSID  2-Group  3--Position
    uchar   caSsid[31];                 // SSID
    uchar   caLabel[32];                // Ͷ�ű�ǩ
    uchar   caPlate[32];            // ģ��
    uint4   lExpire;                    // ��ʱʱ��
} ncPortalRule;




// �͹�������ͨ��
int ncmcomSynPortalManInfo(utShmHead *psShmHead);
#endif
