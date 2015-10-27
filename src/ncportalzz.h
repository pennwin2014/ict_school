/* ProPotral     ��Ȩ����: �Ϻ���������Ϣ�������޹�˾ 2013
    
*/
#ifndef __PROPORTALDEF__
#define __PROPORTALDEF__
#define PORTAL_VERSION         "V2.02.014"
#define PORTAL_RELEASE         "2014/06/10"
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

#define  NCSRV_LNK_PROCESS      47
#define  NCSRV_LNK_ONTIME       50                      /* ��ʱ����               */
#define  NCSRV_LNK_ONLINE       51
#define  NCSRV_LNK_USERS        53                      /* ���߿ͻ�����Ϣ         */
#define  NCSRV_LNK_SYSINFO      54                      /* ���ϵͳ��ǰ������Ϣ   */
#define  NCSRV_LNK_PLATE        55                      /* ģ����  */

#define  PORTAL_LNK_SYSINFO     56
#define  PORTAL_LNK_ACINFO      57                     // AC�����Ϣ
#define  PORTAL_LNK_CLIENTS     58                     // �ͻ�����Ϣ
#define  PORTAL_LNK_BLACKUSER   59                     // �˺ź�����
#define  PORTAL_LNK_USERMAC     60                     // �û�Mac��ַ���ձ�
#define  PORTAL_LNK_PORTALRULE  61                     // Portalʹ�ò���
#define  PORTAL_LNK_GROUPAP     62                     // AP�͵�λ���ձ�
#define  PORTAL_LNK_GROUPSSID   63                     // SSID�͵�λ���ձ�
#define  PORTAL_LNK_TSID        64                     // �ն�SID���ձ�

// #define  NCM_LNK_WXATOKEN        65                 �� ncportalweb.h�ж���
#define  PORTAL_LNK_GROUP       70                     // ��λ��Ϣ
#define  PORTAL_LNK_PORTALPAR   71                     // Portal����
#define  PORTAL_LNK_MACONLINE   72                     // ����Mac��ַ�б�

#define  NCSRV_STA_SUCCESS     0                       /* ��ȷ              */
#define  NCSRV_STA_RELOGIN     10001                   /* �û���Ҫ���µ�¼  */
#define  NCSRV_STA_NOEXIST     10002                   /* �û�������        */
#define  NCSRV_STA_KEYERROR    10003                   /* �������          */


#define  NCSRV_PORTAL_PROEIM      1
#define  NCSRV_PORTAL_RADIUS      2
#define  NCSRV_PORTAL_PORTAL      3
#define  NCSRV_PORTAL_AC          4
#define  NCSRV_PORTAL_SMS         5
#define  NCSRV_PORTAL_GBAP        6



#define  NCSRV_SMSTYPE_PRONETWAY  1              // �����̶�������
#define  NCSRV_SMSTYPE_SMSMODEM   2              // ����è


#define  NCSRV_CAUSE_TIMEOUT       1              // TimeOut
#define  NCSRV_CAUSE_USERREQUEST   2              // User Request
#define ncSrvMax(a,b) ((a) > (b) ? (a) : (b))
#define ncSrvMin(a,b) ((a) < (b) ? (a) : (b))
    

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


#define NC_PORTALAUTH_PAP       0
#define NC_PORTALAUTH_CHAP      1

typedef struct ncPortalAcInfo_s {
    uchar           caName[32];                // AC����
    unsigned long   lAcip;                     // AC IP��ַ
    uint2           nAcPort;                   // AC �˿�
    uint2           nSn;                       // ���к�, ��������
    uchar           caAcType[8];               // AC���  Cisco   GbCom  ProEIM
    uint2           iType;                     // NCSRV_PORTAL_AC NCSRV_PORTAL_RADIUS
    uint2           iAuthType;                 // 0 PAP  1 CHAP
    unsigned long   lLastTime;
    unsigned long   lCount;
    uchar           caKey[32];
} ncPortalAcInfo;

#define NCPORTAL_VENDOR_GBCOM         1
#define NCPORTAL_VENDOR_CISCO         2
#define NCPORTAL_VENDOR_GBAP          3


// Portal�û���Ϣ�� Hash����ʼ��ʱ�����ڴ���
// һ���ʺŶ�Ӧ���Mac��ַ
#define NCPORTAL_USRFROM_MANUAL       1                     // �ֹ�����
#define NCPORTAL_USRFROM_SMS          2                     // ���Զ��Ż�ȡ
#define NCPORTAL_ONEUSER_MAXMAC       5


#define NCPORTAL_BLACK_MAC          1
#define NCPORTAL_BLACK_USER         2

#define NCSRV_ONLINEFILE     "../log/portalonlineuserv1.dat"
// �û�������
typedef struct ncPortalBlackUser_s {
    uint4 groupid;                  // ��λID
    uchar ctype;                    // ��� 1-MAC  2-Uername
    uchar caName[31];               // ��½����Mac��ַ
    uint4 validtime;                // ʧЧʱ��
} ncPortalBlackUser;




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


// Эͬ��������Ϣ
#define NC_PORTAL_STA_NORMAL                1    // ����
#define NC_PORTAL_STA_NETWORKERROR1         2    // �������粻����
#define NC_PORTAL_STA_NETWORKERROR2         3    // Portal�ͷ�����ͨ�Ų�����
#define NC_PORTAL_STA_OTHERERR              9    // ��������


// Portal �ۺ���Ϣ
#define NC_PORTAL_MAXLANG        8
#define NC_PORTAL_MAXSMS         8
#define NC_PORTAL_MAXEIM        32
#define NC_PORTAL_MAXBAKPORTAL   8
#define NC_PORTAL_MAXCOUNT     120     // �ڴ��еĲɼ�����
#define NC_PORTAL_MAXRADIUS      8     // Radius����


#define NC_SMSTYPE_PRONETWAY     1
#define NC_SMSTYPE_SMSMODEM      2

/* ��ط�������Ϣ
 */


 
typedef struct ncPortalIpPort_s {
    unsigned long   lIp;                // ��ַ
    uchar caSrvName[32];                // Sms��������
    uchar caPass[32];                   // ͨ����Կ
    unsigned long   lCount;            // ���ʹ���
    unsigned long   lError;            // ���ɹ�����
    unsigned long   lStartTime;        // ��ʼʱ��
    unsigned long   lCountTime;        // ��ʼͳ��ʱ��
    unsigned long   lLastTime;         // ���ͨ��ʱ��
    uint2 nPort;                        // �˿�
    uint2 nType;                        // ���ͷ�ʽ
    uchar cStatus;           // ״̬  0--������(�ͷ��������ӳ���)  1--����     �μ� NC_PORTAL_STA_
    uchar cIsThis;           // ��ǰ����ʹ��
    uint2 nId;
} ncPortalIpPort;




// Portalͳ����Ϣ
typedef struct ncPortalCount_s {
    unsigned long   lTime;               // ���ݲɼ�ʱ��
    unsigned long   lSmsLocalCount;      // ���ڶ��ŷ��ʹ���
    unsigned long   lSmsInterCount;      // ���ʶ��ŷ��ʹ���
    unsigned long   lDispLogin;          // ��֤��ʾ
    unsigned long   lLogin;              // ��¼ҳ��
    unsigned long   lLoginOK;            // ��¼�ɹ�
    unsigned long   lLoginError;         // ��¼����
    unsigned long   lLogOut;             // �˳�
    unsigned long   lAutoLogin;          // �Զ���¼
    unsigned long   lOnlineUser;         // �����û���(ָ��ʱ���)
    uint8           lBytes;              // ����
    unsigned long   lRegUser;            // ������ע���û�
    unsigned long   lRev[3];
} ncPortalCount;

#define   NCSRV_PAR_ALL        0
#define   NCSRV_PAR_GROUP      1
#define   NCSRV_PAR_SSID       2

// ����
typedef struct ncPortalPar_s {
    uchar           cFlags;                       // 0-ȫ��  1-����  2-SSID
    uchar           cRev[3];
    uint4           lId;                          // 
    unsigned long   lSesstime;                    // ����ʱ������
    unsigned long   lIdletime;                    // ����ʱ��
    unsigned long   lMacbindtime;                 // �û�Mac��ַ��ʧЧʱ��
    unsigned long   lSmslocalnum;                 // ÿ����ڶ��ŷ���������
    unsigned long   lSmsinternum;                 // ÿ����ʶ��ŷ���������
    unsigned long   lLimittime;                   // ����ʱ������
    unsigned long   lLimitunit;                   // ���Ƶ�λ 0-�� 1-��  2-��  3-�� 9-�ܹ�
    unsigned long   lPasscodetime;                // ������Чʱ��
    uchar           caSmsusetime[64];             // ����ע��ʱ��
    uchar           caServicetime[64];            // ��������ʱ��
    uchar           caEngMessage[256];            // Ӣ�Ķ��Ÿ�ʽ
    uchar           caZhMessage[256];             // ���Ķ��Ÿ�ʽ
    uint2           nCurCount;                    // ��ǰͳ��λ��
    uint2           nSumCount;                    // �ܹ��ɼ�����
    uchar           cAutoAuth;                    // �����Ƿ�����֤
    uchar           cWeixinAuth;                  // ΢����֤ 1-΢����֤  2-
    uchar           cSumMac;                      // ÿ���˺ſ��԰󶨵�Mac��ַ��
    uchar           cError;                       // ����������
    uint2           nErrorTime;                   // ������������ʱ��
    uint2           nRev;
} ncPortalPar;

// ��λ��Ϣ
typedef struct ncPortalGroup_s {
    uint4  lGroupid;
    uint4  lModTime;                // ��������޸�ʱ��
    double dLongitude;              // ����
    double dLatitude;               // γ��
    uchar  caGroupName[32];          // ��������
    uchar  caDisp[128];              // ��ʾ��
    uchar  caSmsName[32];            // Sms�Ʒѵ�λ
    uchar  caMark[128];
    uint4  lLasttime;                // ���һ�ε�¼ʱ��
    uchar  cDb;                      // 0--Db���޼�¼    1--Db�д��ڼ�¼
    uchar  cMod;                     // ���¼�¼ 
    uchar  cStatus;                  // ״̬ 1--����  9--ɾ��
    uchar  cCurCount;                // ��ǰͳ��λ��
    ncPortalCount   psPortalCount[NC_PORTAL_MAXCOUNT];   // ͳ����Ϣ
    ncPortalPar     *psPar;          // ����
} ncPortalGroup;


// SSIDΪ����
  
typedef struct ncPortalSsid_s {
    uchar           caSsid[32];                   // SSID����
    uint4           lGroupid;
    uint4           lSid;
    uchar           cStatus;
    uchar           cSurCount;
    uchar           cRev[6];    
    ncPortalPar     *psPar;                       // ��Ӧ�Ĳ���
    ncPortalGroup   *psGroup;                     // ��Ӧ�Ĳ���
    ncPortalCount   psPortalCount[NC_PORTAL_MAXCOUNT]; // Portalͳ������
} ncPortalSsid;


// AP��Ϣ
  
typedef struct ncPortalAp_s {
    uchar           caApid[32];
    uint4           lGroupid;
    uint4           lLastTime;
    double          longitude;
    double          latitude;
    uchar           caModel[16];
    uchar           caVersion[16];
    uchar           caToken[48];
    uint4           lUpTime;
    uchar           mac[6];
    uchar           cLogoutFlags;           // 0--����Ҫ�����ն�  1--�������ն�
    uchar           cRev;
    uint4           lApIp;       // AP��ַ
    uint2           nPort;       // �˿�
    uint2           nRev;
    ncPortalGroup   *psGroup;
} ncPortalAp;


// 
#define  NCSRV_USERTYPE_FIX      9
#define  NCSRV_USERTYPE_SMS      1
#define  NCSRV_USERTYPE_QQ       2
#define  NCSRV_USERTYPE_WEIXIN   3

// ���ֺ�GroupidΨһ, GroupidΪ0��ʾͨ���û�
typedef struct ncPortalUser_s {
    uchar caName[32];               // ��½��
    uint4 lGroupid;                 // �û���ID
    uchar caDisp[32];               // ��ʾ��
    uchar caGroup[32];
    uchar caSsid[32];
    uchar caPass[32];               // ����
    uchar caMac[32];                // �󶨵�Mac��ַ��������5����ÿ��6�ֽ�
    uchar caOpenId[32];
    uchar caLabel[128];
    uint4 lUserid;
    uint4 lLasttime;                // ���һ�ε�¼ʱ��
    uint4 lModTime;                 // �����޸�ʱ��
    uint4 lStarttime;               // �˺�����ʱ��
    uint4 lExpiretime;              // ��ʱʱ��
    uchar cDb;                      // 0--Db���޼�¼    1--Db�д��ڼ�¼
    uchar cMod;                     // ���¼�¼ 
    uchar cFlags;                   // ��¼��Դ 
    uchar cStatus;                  // ״̬ 0--����  9--��ʱ����
    uchar cSum;                     // �󶨵�Mac��ַ��
    uchar cUseType;                 // 0--�̶��û�  1--����  2--QQ  3--΢��
    uchar cRev[2];
    ncPortalGroup  *psGroup;        // �û���Ӧ�ĵ�λ 
    ncPortalPar    *psPar;
    ncPortalSsid   *psSsid;
} ncPortalUser;

// �û���Mac��ַ���ձ�

typedef struct ncPortalUserMac_s {
    uint4 lGroupid;                 // ��λ����
    uchar mac[6];                   // Mac��ַ
    uchar cStatus;                  // 0-��Ч  1--��Ч
    uchar cDb;                      // 0-���ݿ��в�����  1--����  9-��Ч��MAC
    uchar caName[32];               // ��½��
    uint4 starttime;                // ��ʼʱ��
    uint4 lasttime;                 // ���ʹ��ʱ��
    uint4 moditime;                 
    uchar cMod;                     // 0--��¼δ�޸�  1--��¼���޸�
    uchar cFlags;                   // 0--����  9--��ʧЧ
    uchar cRev[2];
    ncPortalUser *psUser;
} ncPortalUserMac;

//   Portal�����û�, ��IP��ַΪ����
// 

#define NCPORTAL_ONLINE_LOGIN          1
#define NCPORTAL_ONLINE_LOGOUTING      2
#define NCPORTAL_ONLINE_DELETE         9
#define NCPORTAL_ONLINE_OFFLINE        0


typedef struct ncPortalOnline_s {
    uchar nasid[24];                // ���ر�ʶ
    uint4 lSip;                     // IP��ַ, �û�����������IP��ַ
    uint4 lGroupid;
    uint8 lTsid;
    uchar caName[32];               // ��½��
//    uchar caDisp[32];               // ��ʾ��
    uchar caSsid[32];               // SSID
    uchar caApName[40];             // AP���ƻ�MAC��ַ 32-34-CB-C0-A1-B7:pronetway-htt
    uchar caAcName[28];             // Ac���ƻ�Ip��ַ
    uchar caOpenid[32];             // �û�����������Ϊ΢��Openid��΢����¼����QQ�ŵ�
    uint2 nReqId;                   // ��֤��
    uint2 nSn;                      // ���к�
    uint4 lAcIp;                    // AC��ַ
    uint2 nAcPort;                  // AC�˿�
    uchar caMac[6];                 // Mac��ַ
    uchar caDev[24];                // �豸
    uchar caOs[16];                 // ����ϵͳ
    uchar caBro[16];                // �����
    uchar caTermType[8];            // �豸���
    uint8 lBytes[2];                // ����
    uchar caLang[8];                // ����
    uint4 sessiontime;              // ��ʱʱ��
    uint4 lasttime;                 // ������ʱ��
    uchar caPostUrl[64];            // Post Url https://192.168.20.38/pronline/Msg
    uchar caLabel[128];
    uchar login;                    // 0--Disp, ��ʾ��¼����   1--Login, �Ѿ���¼  2--��������  9--��¼�Ѿ�ɾ�� 0--�Ѿ�����
    uchar cStatus;                  // 0--����     1--������
    uchar cFrom;                    // ��Դ����
    uchar cSave;                    // 0--δ����   1--����
    uint4 starttime;                // ����ʱ��
    double  longitude;              // ����
    double  latitude;               // ά��
    uint4   lGwip;
    uint2   nGwport;
    uint2   nRev;
    ncPortalPar                     *psPar;     // ��Ӧ�Ĳ���    
    ncPortalUser                    *psUser;    // ��Ӧ���û�
    ncPortalSsid                    *psSsid;    // ��Ӧ��Ӧ���ŵ�SSID
    ncPortalAp                      *psAp;      // ��Ӧ��AP
} ncPortalOnline;


typedef struct ncPortalTsid_s {
    uint8             tsid;         // TSID,Ψһ��ʶ�����û�������
    ncPortalOnline    *psOnline;
} ncPortalTsid;


typedef struct ncPortalMacOnline_s {
    uchar             mac[6];       // MAC��ַ,�������û���ҲӦ��Ψһ
    uchar             rev[2];       // ����
    ncPortalOnline    *psOnline;
} ncPortalMac;



typedef struct ncPortalSummary_s {
    uchar caPortalName[32];            //  Portal����
    uchar caProdSn[32];                //  ��Ʒ���к�
    unsigned long   lPortalId;
    unsigned long   lStartTime;                  // ����ʱ��
    unsigned long   lLastTime;                   // ���ʱ��
    unsigned long   lConnectTime;                // ���һ�����ӹ�������ʱ��
    unsigned long   lAdminIp;                    //  �����ַ

    uint2 nAdminPort;                  //  ����˿�
    uint2 nSumSms;                     // Sms����������
    uint2 nCurSms;                     // ��ǰSMS���
    uint2 nSumGjSms;                   // Sms����������
    uint2 nCurGjSms;                   // ��ǰSMS���

    uint2 nSumRadius;                  // Radius����������
    uint2 nCurRadius;                  // ��ǰRadius���

    uint2 nSumEim;                     // ����豸����
    uint2 nCurEim;                     // ��ǰEIM
    uint2 nSumBakPortal;               // Portal����

    uint2 nAuthWay;                    // ��֤��ʽ  Local,Radius,AC
    uint2 nAuthType;                   // ��֤����  PAP,CHAP
    uchar cRomain;                     // 0--�ʺ��ڲ���֮�䲻����  1--����
    uchar cRev[3];
    uint4  lSumPar;
    ncPortalIpPort  psSmsServer[NC_PORTAL_MAXSMS];     // ���ڶ�������
    ncPortalIpPort  psSmsGjServer[NC_PORTAL_MAXSMS];   // ���ʶ�������
    ncPortalIpPort  psRadServer[NC_PORTAL_MAXRADIUS];  // Radius��Ϣ
    ncPortalIpPort  psEimServer[NC_PORTAL_MAXEIM];     // ���綽����Ϣ
    ncPortalIpPort  psBakPortal[NC_PORTAL_MAXEIM];     // ����Portal״̬
    uchar           *pGroupHash;                       // ������Ϣ
    uchar           *pApHash;
    uchar           *pSsidHash;
    ncPortalPar     *psPar;
} ncPortalSummary;



// ������״̬
typedef struct ncPortalSrvStatus_s {
    uchar               caName[32];                // ����
    unsigned long       lIp;                       // IP��ַ
    unsigned long       lLastTime;                 // ���һ�λʱ��
    uint2               nPort;                     // �˿�
    uint2               nStatus;                   // ״̬
    uint2               nRev[2];
} ncPortalSrvStatus;



// ����Ͷ�Ų��ԣ����Խ���ÿ5���Ӽ��һ�Σ�ȥ����ʱ�ģ������µģ������Ч��
//  1--Position  2--SSid 
typedef struct ncPortalRule_s {
    uchar           cType;                      // ���   1-SSID  2-Group  3--Position
    uchar           caSsid[31];                 // SSID
    uchar           caLabel[32];                // Ͷ�ű�ǩ
    uchar           caPlate[32];                // ģ��
    unsigned long   lExpire;                    // ��ʱʱ��
} ncPortalRule;



// ncsrv000.c
ncPortalPar *ncSrvGetPar(ncPortalSummary *psSumm,int iType,uint4 lGroupid);



ncPortalAcInfo *ncSrvGetAcInfo(utShmHead *psShmHead,char *pAcName);
int ncSrvSetPlateInfo(utShmHead *psShmHead,char *pName,char *pTerm,char *pLang,char *pPlate,char *pCode);
char *ncSrvGetPlateInfo(utShmHead *psShmHead,char *pName,char *pTerm,char *pLang,int *iCode);
int ncUtlPortalTermAttr(char *pUa,char *pTerm,char *pOs,char *pBro,char *pTtype);
char *ncSrvGetMyPlateName(utShmHead *psShmHead);
char *ncSrvGetPlateByRule(utShmHead *psShmHead,int iType,char *pSsid,char *pLable,char *pDefault);
int  ncSrvGetClientKey(utShmHead *psShmHead,uint4 lId,char *pKey);
int  ncSrvGetClientIdByName(utShmHead *psShmHead,char *pName);

//  �� HttpServerͨ��
int ncSrvDispLogin(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvLogin(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvLogOut(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvGetPass(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvTestplate(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvLoginOK(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvLoginError(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvPlate(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
// �����綽���ͨ��  ncsrv002.c
int ncSrvGetMyId(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvSynOnlineUser(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);


// ���̴���  ncSrv003.c
int ncSrvSynRadInfo(utShmHead *psShmHead);
int ncSrvSynUsers2Db(utShmHead *psShmHead);
int ncSrvBakOnlineUser2Db(utShmHead *psShmHead);


// ncsrv004.c
ncPortalSsid *ncSrvGetSsidInfo(utShmHead *psShmHead,uint4 lGroupid,char *pSsid);
ncPortalGroup *ncSrvGetGroupInfoById(utShmHead *psShmHead,uint4 lGroupid);
ncPortalGroup *ncSrvGetGroupInfoByName(utShmHead *psShmHead,uchar *pName);

ncPortalOnline *ncSrvGetOnlineUserByNasidIp(utShmHead *psShmHead,char *nasid,uint4 lSip);
ncPortalOnline *ncSrvGetOnlineUserByNasidIpA(utShmHead *psShmHead,char *nasid, uint4 lSip,char *mac,char *ssid,char *pGwip,char *pGwPort,uint8 llTsid);
ncPortalOnline *ncSrvGetOnlineUserByIp(utShmHead *psShmHead,uint4 lSip);
ncPortalOnline *ncSrvGetOnlineUserByTsid(utShmHead *psShmHead,uint8 llTsid);
ncPortalUser    *ncSrvGetUserByMac(utShmHead *psShmHead,uint4 lGroupid,char *pMac);
ncPortalOnline *ncSrvGetOnlineUserByMac(utShmHead *psShmHead,char *pMac);
ncPortalOnline *ncSrvGetOnlineUserByName(utShmHead *psShmHead,uint4 lGroupid,char *pName);
int ncSrvGetSmsMsg(utShmHead *psShmHead,ncPortalOnline *psOnline,char *pLang,char *caMsg);
int ncSrvThirdAuth(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
uint8 ncSrvAddOnlineUserByTsid(utShmHead *psShmHead,ncPortalOnline *psOnline);
ncPortalSummary *ncSrvGetSumm(utShmHead *psShmHead);
ncPortalUser *ncSrvGetUserById(utShmHead *psShmHead,uint4 lUid);
ncPortalUser *ncSrvGetUserByName(utShmHead *psShmHead,uchar *pName,uint4 lGroupid);

// ncutl_gbwifi.c
int ncSrvGBping(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvGBlogin(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvGBauth(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvGBportal(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);


#endif
