/* ProPotral     ��Ȩ����: �Ϻ���������Ϣ�������޹�˾ 2013
    
*/
#ifndef __PROPORTALDEF__
#define __PROPORTALDEF__
#define PORTAL_VERSION         "V2.02.090"
#define PORTAL_RELEASE         "2015/07/27"
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


#define ACPORTAL_REQ_QMACBIND       0x30                // mac�󶨲�ѯ����
#define ACPORTAL_ACK_QMACBIND       0x31                // mac�󶨲�ѯӦ��
#define ACPORTAL_REQ_MACBINDLOGIN   0x32                // �û�����֪ͨ
#define ACPORTAL_REQ_MACBINDLOGOUT  0x34                // �û�����֪ͨ

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
#define  ICT_USER_LOGIN_TSID    65                     // ��¼�û���¼��tsid�Լ��ֻ��ŵĹ�ϵ


// #define  NCM_LNK_WXATOKEN        65                 �� ncportalweb.h�ж���
#define  PORTAL_LNK_GROUP       70                     // ��λ��Ϣ
#define  PORTAL_LNK_PORTALPAR   71                     // Portal����
#define  PORTAL_LNK_MACONLINE   72                     // ����Mac��ַ�б�
#define  PORTAL_LNK_PLATERULE   73                     // ҳ�����
#define  PORTAL_LNK_LOCMARK     74                     // λ�ñ�ǩ
#define  PORTAL_LNK_MACLOCATION 75                     // MAC��ַλ����Ϣ
#define  PORTAL_LNK_STYPE       76                     // Stype �������
#define  PORTAL_LNK_LBSAPMAC    77                     // �ɼ���MAC��AP��Ϣ
#define  PORTAL_LNK_RUNCOMM     78                     // ����ִ�л���
#define  PORTAL_LNK_APINDEX     79                     // Ap ����
#define  PORTAL_LNK_BANDNAME    80                     // ����
#define  PORTAL_LNK_TSIDINFO    81                     // ����TSIDΪ�����������Ϣ
#define  NCSRV_STA_SUCCESS     0                       /* ��ȷ              */
#define  NCSRV_STA_RELOGIN     10001                   /* �û���Ҫ���µ�¼  */
#define  NCSRV_STA_NOEXIST     10002                   /* �û�������        */
#define  NCSRV_STA_KEYERROR    10003                   /* �������          */

#define  NCSRV_STATUSCODE_SUCESS        0                   /* �ɹ�                */
#define  NCSRV_STATUSCODE_ALREADYLOGIN  1                   /* �û��Ѿ���¼        */
#define  NCSRV_STATUSCODE_FORBITLOGIN   2                   /* �û��ڱ�������ֹ��¼  */
#define  NCSRV_STATUSCODE_EXPIRE        3                   /* �û��˺��Ѿ�����    */
#define  NCSRV_STATUSCODE_BLACKLIST     4                   /* �û��ں�������      */
#define  NCSRV_STATUSCODE_PASSERR       5                   /* �û����벻��ȷ      */
#define  NCSRV_STATUSCODE_NOEXIST       6                   /* �û�������          */
#define  NCSRV_STATUSCODE_EXCEEDMAC     7                   /* ����MAC��ַ������  */
#define  NCSRV_STATUSCODE_EXCEEDTIME    8                   /* ����ÿ������ʱ��     */
#define  NCSRV_STATUSCODE_EXCEEDFLOW    10                  /* ����ÿ����������     */
#define  NCSRV_STATUSCODE_EXCEEDLOGIN   11                  /* ����ÿ���¼����     */

#define  NCSRV_STATUSCODE_PAGEEXPIRE  10000                 /* ҳ�����             */
#define  NCSRV_STATUSCODE_OTHERERROR    9                   /* ��������             */

#define  NCAPP_STATUS_OK          0
#define  NCAPP_STATUS_ONLINE      1
#define  NCAPP_STATUS_ERRNUM      2                  // �ֻ��Ų�����
#define  NCAPP_STATUS_ERRUID      3                  // UID������
#define  NCAPP_STATUS_ERRUIDSN    4                  // UID_SN������
#define  NCAPP_STATUS_ERRMAC      5                  // MAC��ַ������
#define  NCAPP_STATUS_ERRSID      6                  // SessionID������
#define  NCAPP_STATUS_ERRAPP      7                  // APP����
#define  NCAPP_STATUS_NOONLINE    8
#define  NCAPP_STATUS_ERROR       9
#define  NCAPP_STATUS_BLACKNUM    10
#define  NCAPP_STATUS_BLACKMAC    11
#define  NCAPP_STATUS_ERRKEY      12                 // ���벻��ȷ
#define  NCAPP_STATUS_NOLOGIN     0                  // �û�û�е�¼



#define  NCSRV_PORTAL_PROEIM      1
#define  NCSRV_PORTAL_RADIUS      2
#define  NCSRV_PORTAL_PORTAL      3
#define  NCSRV_PORTAL_AC          4
#define  NCSRV_PORTAL_SMS         5
#define  NCSRV_PORTAL_GBAP        6
#define  NCSRV_PORTAL_POST        7
#define  NCSRV_PORTAL_MERU        8
#define  NCSRV_PORTAL_CENTERAUTH  9

#define  NCSRV_MARK_LOCATION      'L'               // λ�ñ�ǩ

#define  NCSRV_SMSTYPE_PRONETWAY  1              // �����̶�������
#define  NCSRV_SMSTYPE_SMSMODEM   2              // ����è
#define  NCSRV_SMSTYPE_PEOPLE     3              // People
#define  NCSRV_SMSTYPE_CENTER     4              // ������֤ƽ̨

#define  NCSRV_ACCT_START          1
#define  NCSRV_ACCT_STOP           2
#define  NCSRV_ACCT_UPDATE         3

#define  NCSRV_NASNAMEFROM_ACNAME  1
#define  NCSRV_NASNAMEFROM_APNAME  2
#define  NCSRV_NASNAMEFROM_FIXNAME 0

#define NCSRV_SSH_GETIPINFO   101
#define NCSRV_SSH_DOLOGIN     102
#define NCSRV_SSH_DOLOGOUT    103
#define NCSRV_SSH_GETMACINFO  104
#define NCSRV_SSH_FLOWEYE     105

#define NCSRV_SSH_NoCommand      0
#define NCSRV_SSH_RunReady       1
#define NCSRV_SSH_RunDoing       2
#define NCSRV_SSH_RunFinish      3


#define ncSrvMax(a,b) ((a) > (b) ? (a) : (b))
#define ncSrvMin(a,b) ((a) < (b) ? (a) : (b))

#define  NCSRV_MAXAC             64

#define NCSRV_TSIDTYPE_REDIRURL     1    // �ض���URL


typedef struct ncPortalTsidInfo_s {
    uint8   llTsid;                    // TSID   
    uint2   nType;                     // ��Ϣ���
    uchar   caMsg[254];                // �����Ϣ
    uint4   lLasttime;                 // ������ʱ��
} ncPortalTsidInfo;

    


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
        uchar   caBuf[256];
    } ncPortalAcHead;
// Portal�����û�, ��tsidΪ����
typedef struct ictOnlineUser_s {
    uint8 tsid;                     // tsid
	uchar vName[32];                // �ֻ��� 
	uchar loginFlag;                // 0---����  1---����  
    uint4 lastTime;                 // ����¼ʱ��            
} ictOnlineUser;


/* PortalЭ���е�ֻ  
*/
#define ACPORTAL_ATTRTYPE_USERNAME       0x01
#define ACPORTAL_ATTRTYPE_PASSWORD       0x02
#define ACPORTAL_ATTRTYPE_CHALLENGE      0x03
#define ACPORTAL_ATTRTYPE_CHAPPASSWORD   0x04
#define ACPORTAL_ATTRTYPE_BASEIP         0x0A
#define ACPORTAL_ATTRTYPE_SESSIONID      0x0B
#define ACPORTAL_ATTRTYPE_NASID          0x30
#define ACPORTAL_ATTRTYPE_SSTARTTIME     0x31
#define ACPORTAL_ATTRTYPE_SSTOPTIME      0x32
#define ACPORTAL_ATTRTYPE_SESSIONTIME    0x33
#define ACPORTAL_ATTRTYPE_USERAGENT      0x34
#define ACPORTAL_ATTRTYPE_APMAC          0x40
#define ACPORTAL_ATTRTYPE_SSID           0x41

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
    uchar           caPost[128];               // ��֤��ַ http://192.168.20.38/qqqq
    unsigned long   lLastTime;
    unsigned long   lCount;
    uchar           caKey[32];
    uchar           caDomain[64];             // ����  @jn  jn@  ����@��λ�þ�����������ʲô�ط�
} ncPortalAcInfo;

#define NCPORTAL_VENDOR_GBCOM         1
#define NCPORTAL_VENDOR_CISCO         2
#define NCPORTAL_VENDOR_GBAP          3
#define NCPORTAL_VENDOR_WIFIDOG       4
#define NCPORTAL_VENDOR_ARUBA         5
#define NCPORTAL_VENDOR_MOTO          6
#define NCPORTAL_VENDOR_PROEIM        7
#define NCPORTAL_VENDOR_MERU          8  
#define NCPORTAL_VENDOR_PROEIMV7      10  
#define NCPORTAL_VENDOR_DEFAULT       9
#define NCPORTAL_VENDOR_MERUAC        11
#define NCPORTAL_VENDOR_MERUAC1        12




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


// ����Ͷ�Ų��ԣ����Խ���ÿ5���Ӽ��һ�Σ�ȥ����ʱ�ģ������µģ������Ч��


typedef struct ncPortalPlateRule_s {
    uint4           ruleid;
    uint4           priority;
    uchar           groupcode[32];
    uchar           platename[16];
    uchar           ssid[32];
    uchar           nasid[32];
    uchar           mark[256];
    uchar           dotime[256];
    uint4           dateid;
    uint4           starttime;
    uint4           endtime;
} ncPortalPlateRule;



// Эͬ��������Ϣ
#define NC_PORTAL_STA_NORMAL                1    // ����
#define NC_PORTAL_STA_NETWORKERROR1         2    // �������粻����
#define NC_PORTAL_STA_NETWORKERROR2         3    // Portal�ͷ�����ͨ�Ų�����
#define NC_PORTAL_STA_OTHERERR              9    // ��������


// Portal �ۺ���Ϣ
#define NC_PORTAL_MAXLANG        8
#define NC_PORTAL_MAXSMS         8
#define NC_PORTAL_MAXEIM        32
#define NC_PORTAL_MAXBAKPORTAL  32
#define NC_PORTAL_MAXCOUNT     120     // �ڴ��еĲɼ�����
#define NC_PORTAL_MAXRADIUS      8     // Radius����



/* ��ط�������Ϣ
 */


#define NCSRV_LOC_BYIP           1
#define NCSRV_LOC_BYMAC          0

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
#define   NCSRV_PAR_STYPE      3

/*
    ����
    ȫ��          ---- cFlags = 0   lId = 0
    ���ȫ��SSID  ---- ȫ��SSID
    ��Բ���      ---- �ض���Groupid
    ���SSID      ---- ����SSID

*/     
typedef struct ncPortalPar_s {
    uchar           cFlags;                       // 0-ȫ��  1-����  2-SSID
    uchar           cRev[3];
    uint4           lId;                          // 
    uint4           lSesstime;                    // ����ʱ������
    uint4           lIdletime;                    // ����ʱ��
    uint4           lMacbindtime;                 // �û�Mac��ַ��ʧЧʱ��
    uint4           lSmslocalnum;                 // ÿ����ڶ��ŷ���������
    uint4           lSmsinternum;                 // ÿ����ʶ��ŷ���������
    uint4           lLimittime;                   // ����ʱ������
    uint4           lLimitunit;                   // ���Ƶ�λ 0-�� 1-��  2-��  3-�� 9-�ܹ�
    uint4           lPasscodetime;                // ������Чʱ��
    uchar           caSmsusetime[64];             // ����ע��ʱ��
    uchar           caServicetime[64];            // ��������ʱ��
    uchar           caEngMessage[256];            // Ӣ�Ķ��Ÿ�ʽ
    uchar           caZhMessage[256];             // ���Ķ��Ÿ�ʽ
    uint2           nCurCount;                    // ��ǰͳ��λ��
    uint2           nSumCount;                    // �ܹ��ɼ�����
    uchar           cAutoAuth;                    // �����Ƿ�����֤
    uchar           cWeixinAuth;                  // ΢����֤ 1-΢����֤��ʵ����֤���   2--�Զ���֤����ע΢�ź��Զ�����)
    uchar           cSumMac;                      // ÿ���˺ſ��԰󶨵�Mac��ַ��
    uchar           cError;                       // ����������
    uint2           nErrorTime;                   // ������������ʱ��
    uchar           cMacAuth;                     // �޸�֪��֤  0--��  1--Mac��֤����   2--Radius��֤  9--δ����
    uchar           cSsidRoam;                    // ��ͬSSID֮�������     9--δ����  0--������   1--����
    uchar           cGroupRoam;                   // ��ͬ����֮�������     9--δ����  0--������   1--����
    uchar           cAuthWay;                     // ��֤��ʽ   1--Radius   2--����
    uchar           cAutoBindMac;                 // �Զ�Mac���û���  0--�Զ�   1--�ֹ�
    uchar           cExceedMac;                   // �����������Ĵ���ʽ 0--������֤  1--��ֹ����
    uchar           caGroupCode[32];              // ����������
    uint4           lAuthIp[2];                   // ��֤��ַ
    uint2           nAuthPort[2];                 // ��֤�˿�
    uint4           limitflow;                    // ��������
    uint4           limitband;                    // ��������
    uint4           limitlogin;                   // ��¼��������
    uint4           lCheckSum;                    // У���� = md5(Record)
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


// AP��Ϣ, ����caAddr--��װ��ַ
  
typedef struct ncPortalAp_s {
    uchar           caApid[32];     // ApID
    uint4           lGroupid;       // ��Ӧ�ĵ�λID
    uint4           lLastTime;      // �����ϵʱ��
    double          longitude;      // ����
    double          latitude;       // ά��
    uchar           caModel[16];    // ģ��
    uchar           caVersion[16];  // �汾
    uchar           caToken[48];    // �豸����
    uchar           caMark[64];     // ��ǩ
    uchar           caAddr[64];     // ��ַ
    uchar           plate[32];
    uchar           line[32];
    uchar           mapid[32];
    uchar           apname[32];   // AP����
    uchar           ssid[64];
    uint4           lUpTime;        // ����ʱ��
    uchar           mac[64];        // �豸MAC��ַ
    uchar           cLogoutFlags;   // 0--����Ҫ�����ն�  1--�������ն�
    uchar           cDb:4;          // 0--���ݿ��в�����  1--���ݿ����Ѵ���
    uchar           cMod:4;         // 0--δ�޸�          1--�޸Ĺ�
    uint4           lApIp;          // AP��ַ
    uint2           nPort;          // �˿�
    uint2           nDevType;       // �豸���   
    uint2           nVendor;        // ���̱���
    uint2           nMarkNum;       // Mark��Ӧ���������
    uchar           caRev[8];      // ����
    uint4           lMacCount;
    ncPortalGroup   *psGroup;
} ncPortalAp;


typedef struct ncPortalApIndex_s {
    uchar           apmac[6];
    uchar           rev[2];
    ncPortalAp      *psAp;
} ncPortalApIndex;


// 
#define  NCSRV_USERTYPE_FIX      9
#define  NCSRV_USERTYPE_SMS      1
#define  NCSRV_USERTYPE_QQ       2
#define  NCSRV_USERTYPE_WEIXIN   3
#define  NCSRV_USERTYPE_APP      4

// ���ֺ�GroupidΨһ, GroupidΪ0��ʾͨ���û�
typedef struct ncPortalUser_s {
    uchar caName[32];               // ��½��
    uint4 lStype;
    uchar caDisp[32];               // ��ʾ��
    uchar caGroup[32];
    uchar caSsid[32];
    uchar caPass[32];               // ����
    uchar caOpenId[64];             // �󶨵Ĺ��ںţ���:΢��,QQ��  1:΢��  2:QQ  3:΢��  1:qwqewqweqweqe,aaa;2:qqqqqqdd,qq
    uchar bindmac[6];
    uchar caLabel[118];
    uint4 lGroupid;                 // �û���ID
    uint4 lUserid;
    uint4 lLasttime;                // ���һ�ε�¼ʱ��
    uint4 lModTime;                 // �����޸�ʱ��
    uint4 lStarttime;               // �˺�����ʱ��
    uint4 lExpiretime;              // ��ʱʱ��
    uint4 lSumConnTime;             // ������ʱ��
    uint4 lSumLogin;                // �ܵ�¼����
    uint4 lSumBytes;                // ����������
    uint4 lRev;
    uchar cDb;                      // 0--Db���޼�¼    1--Db�д��ڼ�¼
    uchar cMod;                     // ���¼�¼ 0--��¼δ�޸�  1--��¼���޸�
    uchar cFlags;                   // ��¼��Դ 
    uchar cStatus;                  // ״̬ 0--���� 1--������  2--���ڴ��� 9--��ʱ����
    uchar cSum;                     // �󶨵�Mac��ַ��
    uchar cUseType;                 // 0--�̶��û�  1--����  2--QQ  3--΢��
    uchar cMaxMac;                  // ���԰󶨵����MAC��ַ��
    uchar cRev;
    ncPortalGroup  *psGroup;        // �û���Ӧ�ĵ�λ 
    ncPortalPar    *psPar;
    ncPortalSsid   *psSsid;
} ncPortalUser;

// �û���Mac��ַ���ձ�

typedef struct ncPortalUserMac_s {
    uint4 lGroupid;                 // ��λ���
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

#define NCPORTAL_CAUSE_BYADMIN         1
#define NCPORTAL_CAUSE_TIMEOUT         2
#define NCPORTAL_CAUSE_USERREQUEST     3
#define NCPORTAL_CAUSE_IDLETIMEOUT     4
#define NCPORTAL_CAUSE_HOSTREQUEST     5
#define NCPORTAL_CAUSE_FLOWOUT         6
#define NCPORTAL_CAUSE_TIMEEXCEED      7       // ����ʱ��


#define NCPORTAL_LOGIN_WEBAUTH         1       // Web��֤
#define NCPORTAL_LOGIN_WEBAUTOAUTH     2       // Web�Զ���֤
#define NCPORTAL_LOGIN_MACAUTH         3       // MAC��ַ��֤
#define NCPORTAL_LOGIN_WEIXINAUTH      4       // ΢����֤
#define NCPORTAL_LOGIN_APPAUTH         5       // APP��֤
#define NCPORTAL_LOGIN_CARDAUTH        6       // ֤����֤
#define NCPORTAL_LOGIN_MEMBERAUTH      7       // �˺���֤

typedef struct ncPortalOnline_s {
    uchar   nasid[24];                // ���ر�ʶ
    uint4   lSip;                     // IP��ַ, �û�����������IP��ַ
    uint4   lGroupid;                 // ��λID
    uint8   lTsid;                    // SessionID
    uchar   caName[32];               // ��½��
    uchar   caSsid[32];               // SSID
    uchar   caApName[40];             // AP���ƻ�MAC��ַ 32-34-CB-C0-A1-B7:pronetway-htt
    uchar   caAcName[28];             // Ac���ƻ�Ip��ַ
    uchar   caOpenid[32];             // �û�����������Ϊ΢��Openid��΢����¼����QQ�ŵ�
    uint2   nReqId;                   // ��֤��
    uint2   nSn;                      // ���к�
    uint4   lAcIp;                    // AC��ַ
    uint2   nAcPort;                  // AC�˿�
    uchar   caMac[6];                 // Mac��ַ
    uchar   caDev[24];                // �豸
    uchar   caOs[16];                 // ����ϵͳ
    uchar   caBro[16];                // �����
    uchar   caTermType[8];            // �豸���
    uint8   lBytes[2];                // ����          ��������
    uint4   lSumBytes;                // ��ʱ������������
    uint4   lSumTime;                 // ��ʱ����������ʱ��
    uchar   caLang[8];                // ����
    uint4   logintime;                // ��֤ʱ��
    uint4   lasttime;                 // ������ʱ��
    uchar   caPostUrl[64];            // Post Url https://192.168.20.38/pronline/Msg
    uchar   caMark[132];              // ��ǩ
    uchar   caSrc[16];                // ���볧��
    uchar   caUid[32];                // UID_SN
    uchar   caRev[12];                // ����
    uint4   lOutip;                   // ����IP��ַ
    uchar   caKey[24];                // ����
    uint4   lAcctTime;                // �Ʒ���Ϣ����ʱ��
    uchar   cAuthType;                // NC_PORTALAUTH_CHAP,NC_PORTALAUTH_PAP
    uchar   cBand;                    // 0--��  >0 �����˴���
    uchar   cRev[2];                  // ����
    uchar   login;                    // 0--Disp, ��ʾ��¼����   1--Login, �Ѿ���¼  2--��������  9--������  0--δ��½
    uchar   cStatus;                  // 0--����     1--������
    uchar   cFrom;                    // ��Դ����
    uchar   cSave;                    // 0--δ����   1--����
    uint4   starttime;                // ����ʱ��
    uint4   conntime;                 // ����ʱ��
    double  longitude;                // ���� x
    double  latitude;                 // ά�� y
    uchar   mapid[16];                // MacID
    uchar   platename[16];            // ����ʹ�õ�ģ��ID
    uint4   locationtime;             // ��λ��Ϣ����ʱ��
    uint4   lGwip;                    // ����IP
    uint2   nGwport;                  // ���ض˿�
    uchar   cCause;                   // �˳�ԭ�� 
    uchar   cAuthWay;                 // NCPORTAL_LOGIN_WEBAUTH NCPORTAL_LOGIN_WEBAUTH NCPORTAL_LOGIN_APPAUTH
    uint4   lStype;                   // �û����
    uint4   lSessionTime;             // Session Time
    uint4   lIdelTime;                // lIdelTime
    uchar   caGroupCode[32];          // �����˱���
    ncPortalPar                     *psPar;     // ��Ӧ�Ĳ���,��ǰ�����õĲ���
    ncPortalUser                    *psUser;    // ��Ӧ���û�
    ncPortalSsid                    *psSsid;    // ��Ӧ��Ӧ���ŵ�SSID
    ncPortalAp                      *psAp;      // ��Ӧ��AP
    ncPortalAcInfo                  *psAc;
} ncPortalOnline;


/* λ�ñ�ǩ

*/
typedef struct ncLocationMark_s {
    char    caMark[32];
    char    caMapid[16];
    double  sx;
    double  sy;
    double  ex;
    double  ey;
} ncLocationMark;





typedef struct ncPortalTsid_s {
    uint8             tsid;         // TSID,Ψһ��ʶ�����û�������
    ncPortalOnline    *psOnline;
} ncPortalTsid;


typedef struct ncPortalMacOnline_s {
    uchar             mac[6];       // MAC��ַ,�������û���ҲӦ��Ψһ
    uchar             rev[2];       // ����
    ncPortalOnline    *psOnline;
} ncPortalMac;

/* �û����
   �������Ա�: ncmautotype
   
 */
#define NCSRV_MATCH_EQUAL        0
#define NCSRV_MATCH_START        1
#define NCSRV_MATCH_END          2
#define NCSRV_MATCH_INCLUDE      3
#define NCSRV_MATCH_ANY          4

typedef struct ncPortalStype_s {
    uchar groupcode[30];
    uchar groupmatch;                 // 0--ȫƥ��  1--��ʼ  2--����  3--����
    uchar grouplen;
    uchar ssid[30];
    uchar ssidmatch;                  // 0--ȫƥ��  1--��ʼ  2--����
    uchar ssidlen;
    uint4 stype;                      // Stype����
    uchar shopcode[4];                // �̼���� 2λ
    uchar namecode[8];                // ����6λ
    uchar ucode[32];                  // ��Ӧ�ı���
    uchar descr[32];                  // ����
    ncPortalPar     *psPar; 
} ncPortalStype;



typedef struct ncPortalBand_s {
    int  id;
    char name[32];
} ncPortalBand;


// App��֤�����Ϣ
typedef struct ncPortalAppAuth_s {
    char sjname[16];              // ��Ƴ�����Դ
    char host[32];                // ��֤������IP
    char port[16];                // �˿�
    char path[100];               // ·��
    char prot[16];                // Э�� http,https
    char reurl[128];
    char reapp_url[128];
    uint4 lSessionTime;
    char caRev[128];              // ����
} ncPortalAppAuth;

// �ۺ���Ϣ
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
    uint4  lSumPlateRule;
    uint4  lSumLocation;
    uint4  lSumStype;
    uint4  lSumBand;                  // �ܵĴ�����
    uchar  caControl[64];             // ����λ
    ncPortalIpPort  psSmsServer[NC_PORTAL_MAXSMS];           // ���ڶ�������
    ncPortalIpPort  psSmsGjServer[NC_PORTAL_MAXSMS];         // ���ʶ�������
    ncPortalIpPort  psRadServer[NC_PORTAL_MAXRADIUS];        // Radius��Ϣ
    ncPortalIpPort  psEimServer[NC_PORTAL_MAXEIM];           // ���綽����Ϣ
    ncPortalIpPort  psBakPortal[NC_PORTAL_MAXBAKPORTAL];     // ����Portal״̬
    ncPortalAppAuth sAppAuth;                              // App��֤��Ϣ
    uchar           *pGroupHash;                       // ������Ϣ
    uchar           *pApHash;
    uchar           *pSsidHash;
    ncPortalPar     *psPar;                            // ϵͳ����
    ncPortalPlateRule *psPlateRule;
    ncLocationMark    *psLocation;                     // λ����Ϣ
    ncPortalStype     *psStype;                        // �û����
    ncPortalBand      *psBand;
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


// λ�ñ�ǩ
typedef struct ncLbsLocApMark_s {
    char        caMark[32];
    uint4       lMacCount;        // Mac��ַ����
} ncLbsLocMark;


//     ����ͳ��
//     ��markid�仯ʱ��д��־   268
//     mac,starttime,endtime,mark
typedef struct ncLbsMacLocation_s {
    uchar   mac[6];                                   // Mac��ַ��Ϊ����   
    uchar   apmac[6];                                 // AP Mac��ַ
    char    lrssi;                                    // �ź�ǿ��,��С
    char    urssi;                                    // �ź�ǿ��,���
    char    noise;
    char    channel;
    uint4   starttime;                                // ��ʼʱ��
    uint4   gpstime;
    ncPortalAp *psAp;
    uint4   lasttime;                                 // ���һ�η���ʱ��
    uchar   cFlags;                                   // 1--����
    uchar   cDb;                                      // 0--δ���浽��־  1--�ѱ��浽��־
    uchar   caBssid[6];                               // ������Bssid
    uchar   cAssocicated;                             // ����  1--Yes  0--No
    uchar   encrypt;
    uchar   distance;
    uchar   mutype;                                   // 2-�ն�  3--�ȵ�
    uchar   vtype;                                    // ������ 0--��ʵ���  1--�������
    uchar   caRev[3];
    uchar   longitude[12];
    uchar   latitude[12];
    uchar   ssid[32];
    uchar   termtype[32];
    uchar   name[64];                                 // �������
} ncLbsMacLocation;

typedef struct ncLbsMacRecord_s {
    ncLbsMacLocation *psMacLoc;
} ncLbsMacRecord;

typedef struct ncSrvComBuf_s {
    uchar     status;          // ״̬ 0--������ 1--δִ��  2--����ִ��   3--ִ�����
    uchar     fun;             // ִ�еĹ���    1--LOGON    2--LOGOFF  3--GET
    uchar     rc;              // �����״̬  0--���ɹ�   1--�ɹ�
    uchar     rev;
    uint4     lasttime;        // ��ʼʱ��    
    uchar     command[128];    // Ҫִ�е�����
    uchar     buf[128];        // ����������
} ncSrvComBuf;

typedef struct ncSrvComBufHead_s {
    int4     iMaxCom;
    int4     iCurNum;
    int4     iCurDo;
    uint4    lStartTime;
    uint4    lLastTime;
    int4     iPid;
    uint4    lDoStartTime;         // �����ʼʱ��
    int4     iStatus;              // ״̬
    uchar    caAcName[16];    
    ncSrvComBuf *psBuf;
} ncSrvComBufHead;


// ncsrv000.c
ncPortalPar *ncSrvGetPar(ncPortalSummary *psSumm,int iType,uint4 lGroupid);
ncPortalUser *ncSrvSynUserInfo(utShmHead *psShmHead,
                               uchar *pGroupCode,         // ���ű���, NULL, lGroupid =0
                               uchar *pSsid,              // SSID, NULL -- ��SSID
                               char *pUserName,           // �û���
                               char *pPassword,           // ����
                               char *pMark,               // NULL ��ʾMark����
                               char *pDisp,               // ��ʾ��
                               int  iUsrType,             // �û����  9--�̶��û�  1--������֤  2--QQ  33--΢��
                               int  iMacUser,
                               int  iFlags,               // �û�״̬  0--����      9--����
                               int  *iStatus0);


ncPortalAcInfo *ncSrvGetAcInfo(utShmHead *psShmHead,char *pAcName);
int ncSrvSetPlateInfo(utShmHead *psShmHead,char *pName,char *pTerm,char *pLang,char *pPlate,char *pCode);
char *ncSrvGetPlateInfo(utShmHead *psShmHead,char *pName,char *pTerm,char *pLang,int *iCode);
int ncUtlPortalTermAttr(char *pUa,char *pTerm,char *pOs,char *pBro,char *pTtype);
char *ncSrvGetMyPlateName(utShmHead *psShmHead);
char *ncSrvGetPlateByRule(utShmHead *psShmHead,int iType,char *pSsid,char *pLable,char *pDefault);
int  ncSrvGetClientKey(utShmHead *psShmHead,uint4 lId,char *pKey);
int  ncSrvGetClientIdByName(utShmHead *psShmHead,char *pName);
ncPortalStype  *ncSrvGetStypeById(utShmHead *psShmHead, uint4 lStype);

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


// 7004  ncsrv004.c

ncPortalSsid *ncSrvGetSsidInfo(utShmHead *psShmHead,uint4 lGroupid,char *pSsid);
ncPortalSsid *ncSrvGetSsidInfoBySid(utShmHead *psShmHead,uint4 lSid);
int ncSrvGetSsidNameBySid(utShmHead *psShmHead,uint4 lSid,char *pGroup,char *pSsid);
ncPortalGroup *ncSrvGetGroupInfoById(utShmHead *psShmHead,uint4 lGroupid);
char *ncSrvGetGroupNameById(utShmHead *psShmHead,uint4 lGroupid);
ncPortalGroup *ncSrvGetGroupInfoByName(utShmHead *psShmHead,uchar *pName);
ncPortalOnline *ncSrvGetOnlineUserByNasidIp(utShmHead *psShmHead,char *nasid,uint4 lSip);
ncPortalOnline *ncSrvGetOnlineUserByNasidIpA(utShmHead *psShmHead,char *nasid, char *apmac,uint4 lSip,char *mac,char *ssid,
                                              char *pGwip,char *pGwPort,uint8 llTsid);
ncPortalOnline *ncSrvGetOnlineUserByIp(utShmHead *psShmHead,uint4 lSip);
ncPortalOnline *ncSrvGetOnlineUserByTsid(utShmHead *psShmHead,uint8 llTsid);
ncPortalOnline *ncSrvGetOnlineUserByStrTsid(utShmHead *psShmHead,char *caTsid);
ncPortalUser *ncSrvGetUserById(utShmHead *psShmHead,uint4 lUid);
ncPortalUser *ncSrvGetUserByName(utShmHead *psShmHead,uchar *pName,uint4 lGroupid);
ncPortalOnline *ncSrvGetOnlineUserByMac(utShmHead *psShmHead,char *pMac);
ncPortalOnline *ncSrvGetOnlineUserByName(utShmHead *psShmHead,uint4 lGroupid,char *pName);
int ncSrvGetSmsMsg(utShmHead *psShmHead,ncPortalOnline *psOnline,char *pLang,char *caMsg);
int ncSrvThirdAuth(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
uint8 ncSrvAddOnlineUserByTsid(utShmHead *psShmHead,ncPortalOnline *psOnline);
int ncSrvSetOnlineUserByTsid(utShmHead *psShmHead,ncPortalOnline *psOnline,uint8 llTsid);
ncPortalSummary *ncSrvGetSumm(utShmHead *psShmHead);
ncPortalAp *ncSrvGetApInfoByName(utShmHead *psShmHead,char *pName);
int ncSrvLetUserLogout(utShmHead *psShmHead,uint8 lTsid);

// ncutl_gbwifi.c
int ncSrvGBping(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvGBlogin(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvGBauth(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvGBportal(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvGBclient(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvGBconf(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);

#endif
