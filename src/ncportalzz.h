/* ProPotral     版权所有: 上海新网程信息技术有限公司 2013
    
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
#define  NCSRV_LNK_ONTIME       50                      /* 定时程序               */
#define  NCSRV_LNK_ONLINE       51
#define  NCSRV_LNK_USERS        53                      /* 在线客户端信息         */
#define  NCSRV_LNK_SYSINFO      54                      /* 存放系统当前各种信息   */
#define  NCSRV_LNK_PLATE        55                      /* 模板存放  */

#define  PORTAL_LNK_SYSINFO     56
#define  PORTAL_LNK_ACINFO      57                     // AC相关信息
#define  PORTAL_LNK_CLIENTS     58                     // 客户端信息
#define  PORTAL_LNK_BLACKUSER   59                     // 账号黑名单
#define  PORTAL_LNK_USERMAC     60                     // 用户Mac地址对照表
#define  PORTAL_LNK_PORTALRULE  61                     // Portal使用策略
#define  PORTAL_LNK_GROUPAP     62                     // AP和单位对照表
#define  PORTAL_LNK_GROUPSSID   63                     // SSID和单位对照表
#define  PORTAL_LNK_TSID        64                     // 终端SID对照表

// #define  NCM_LNK_WXATOKEN        65                 在 ncportalweb.h中定义
#define  PORTAL_LNK_GROUP       70                     // 单位信息
#define  PORTAL_LNK_PORTALPAR   71                     // Portal参数
#define  PORTAL_LNK_MACONLINE   72                     // 在线Mac地址列表

#define  NCSRV_STA_SUCCESS     0                       /* 正确              */
#define  NCSRV_STA_RELOGIN     10001                   /* 用户需要重新登录  */
#define  NCSRV_STA_NOEXIST     10002                   /* 用户不存在        */
#define  NCSRV_STA_KEYERROR    10003                   /* 口令出错          */


#define  NCSRV_PORTAL_PROEIM      1
#define  NCSRV_PORTAL_RADIUS      2
#define  NCSRV_PORTAL_PORTAL      3
#define  NCSRV_PORTAL_AC          4
#define  NCSRV_PORTAL_SMS         5
#define  NCSRV_PORTAL_GBAP        6



#define  NCSRV_SMSTYPE_PRONETWAY  1              // 新网程短信网关
#define  NCSRV_SMSTYPE_SMSMODEM   2              // 短信猫


#define  NCSRV_CAUSE_TIMEOUT       1              // TimeOut
#define  NCSRV_CAUSE_USERREQUEST   2              // User Request
#define ncSrvMax(a,b) ((a) > (b) ? (a) : (b))
#define ncSrvMin(a,b) ((a) < (b) ? (a) : (b))
    

#define  NCSRV_MAXAC             64
// 和Portal通信的客户端
typedef struct ncPortalClient_s {
    int4   iType;                          // 类别  1--ProEIM   2--Radius
    uchar  caName[32];                     // 名称
    uchar  caKey[32];                      // 通信秘钥
    uint4  lNum;                           // 序号
    uint4  lIp;                            // IP地址，若为0表示任意IP地址
} ncPortalClient;



typedef struct ncPortalACHead_s {
        uchar   Ver;                  // 版本号  0x01
        uchar   Type;                 //  0x01 -- REQ_CHALLENGE   0x02 -- ACK_CHALLENGE   0x03 ---REQ_AUTH  0x04 ---ACK_AUTH 
        uchar   PapChap;              // Chap: 0x00  Pap: 0x01
        uchar   Rsv;                  // 保留: 0
        uint2   Sn;                   // 客户端随机生成
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

 // AC 对照表
typedef struct ncPortalACAttr_s {
        uchar   Type;
        uchar   AttrLen;
        uchar   Value[254];
} ncPortalACAttr;


// User Agent 信息采集
typedef struct ncPortalUseAgent_s {
    char    caKey[32];                 // 关键字
    char    caName[32];                // 名称
    char    caDevType[8];              // 设备类型
    uchar   cType;                     // 类别  1--TermType    2--OS    3--Bro
    uchar   cPr;                       // 数字越大越优先
    uchar   cRev[2];
}  ncPortalUseAgent;


#define NC_PORTALAUTH_PAP       0
#define NC_PORTALAUTH_CHAP      1

typedef struct ncPortalAcInfo_s {
    uchar           caName[32];                // AC名称
    unsigned long   lAcip;                     // AC IP地址
    uint2           nAcPort;                   // AC 端口
    uint2           nSn;                       // 序列号, 不断增加
    uchar           caAcType[8];               // AC类别  Cisco   GbCom  ProEIM
    uint2           iType;                     // NCSRV_PORTAL_AC NCSRV_PORTAL_RADIUS
    uint2           iAuthType;                 // 0 PAP  1 CHAP
    unsigned long   lLastTime;
    unsigned long   lCount;
    uchar           caKey[32];
} ncPortalAcInfo;

#define NCPORTAL_VENDOR_GBCOM         1
#define NCPORTAL_VENDOR_CISCO         2
#define NCPORTAL_VENDOR_GBAP          3


// Portal用户信息， Hash表，初始化时放在内存中
// 一个帐号对应多个Mac地址
#define NCPORTAL_USRFROM_MANUAL       1                     // 手工输入
#define NCPORTAL_USRFROM_SMS          2                     // 来自短信获取
#define NCPORTAL_ONEUSER_MAXMAC       5


#define NCPORTAL_BLACK_MAC          1
#define NCPORTAL_BLACK_USER         2

#define NCSRV_ONLINEFILE     "../log/portalonlineuserv1.dat"
// 用户黑名单
typedef struct ncPortalBlackUser_s {
    uint4 groupid;                  // 单位ID
    uchar ctype;                    // 类别 1-MAC  2-Uername
    uchar caName[31];               // 登陆名或Mac地址
    uint4 validtime;                // 失效时间
} ncPortalBlackUser;




// 推送页面模板
typedef struct ncPortalPlate_s {
    uchar caName[16];               // 名称
    uchar caTerm[8];                // 终端
    uchar caLang[8];                // 语言
    uchar caPlate[128];             // 模板
    uint4 lCount;                   // 请求次数
    uint4 lLastTime;                // 最后访问时间
    int4  iCode;                    // 使用字符集
} ncPortalPlate;


// 协同服务器信息
#define NC_PORTAL_STA_NORMAL                1    // 正常
#define NC_PORTAL_STA_NETWORKERROR1         2    // 网关网络不正常
#define NC_PORTAL_STA_NETWORKERROR2         3    // Portal和服务器通信不正常
#define NC_PORTAL_STA_OTHERERR              9    // 其它错误


// Portal 综合信息
#define NC_PORTAL_MAXLANG        8
#define NC_PORTAL_MAXSMS         8
#define NC_PORTAL_MAXEIM        32
#define NC_PORTAL_MAXBAKPORTAL   8
#define NC_PORTAL_MAXCOUNT     120     // 内存中的采集数据
#define NC_PORTAL_MAXRADIUS      8     // Radius总数


#define NC_SMSTYPE_PRONETWAY     1
#define NC_SMSTYPE_SMSMODEM      2

/* 相关服务器信息
 */


 
typedef struct ncPortalIpPort_s {
    unsigned long   lIp;                // 地址
    uchar caSrvName[32];                // Sms网关名称
    uchar caPass[32];                   // 通信秘钥
    unsigned long   lCount;            // 发送次数
    unsigned long   lError;            // 不成功次数
    unsigned long   lStartTime;        // 开始时间
    unsigned long   lCountTime;        // 开始统计时间
    unsigned long   lLastTime;         // 最后通信时间
    uint2 nPort;                        // 端口
    uint2 nType;                        // 发送方式
    uchar cStatus;           // 状态  0--不在线(和服务器连接出错)  1--在线     参见 NC_PORTAL_STA_
    uchar cIsThis;           // 当前正在使用
    uint2 nId;
} ncPortalIpPort;




// Portal统计信息
typedef struct ncPortalCount_s {
    unsigned long   lTime;               // 数据采集时间
    unsigned long   lSmsLocalCount;      // 国内短信发送次数
    unsigned long   lSmsInterCount;      // 国际短信发送次数
    unsigned long   lDispLogin;          // 认证显示
    unsigned long   lLogin;              // 登录页面
    unsigned long   lLoginOK;            // 登录成功
    unsigned long   lLoginError;         // 登录错误
    unsigned long   lLogOut;             // 退出
    unsigned long   lAutoLogin;          // 自动登录
    unsigned long   lOnlineUser;         // 在线用户数(指定时间点)
    uint8           lBytes;              // 流量
    unsigned long   lRegUser;            // 新增的注册用户
    unsigned long   lRev[3];
} ncPortalCount;

#define   NCSRV_PAR_ALL        0
#define   NCSRV_PAR_GROUP      1
#define   NCSRV_PAR_SSID       2

// 参数
typedef struct ncPortalPar_s {
    uchar           cFlags;                       // 0-全局  1-部门  2-SSID
    uchar           cRev[3];
    uint4           lId;                          // 
    unsigned long   lSesstime;                    // 上网时间限制
    unsigned long   lIdletime;                    // 空闲时间
    unsigned long   lMacbindtime;                 // 用户Mac地址绑定失效时间
    unsigned long   lSmslocalnum;                 // 每天国内短信发送最多次数
    unsigned long   lSmsinternum;                 // 每天国际短信发送最多次数
    unsigned long   lLimittime;                   // 上网时间限制
    unsigned long   lLimitunit;                   // 限制单位 0-天 1-周  2-月  3-年 9-总共
    unsigned long   lPasscodetime;                // 密码有效时间
    uchar           caSmsusetime[64];             // 允许注册时间
    uchar           caServicetime[64];            // 允许上网时间
    uchar           caEngMessage[256];            // 英文短信格式
    uchar           caZhMessage[256];             // 中文短信格式
    uint2           nCurCount;                    // 当前统计位置
    uint2           nSumCount;                    // 总共采集数据
    uchar           cAutoAuth;                    // 二次是否免验证
    uchar           cWeixinAuth;                  // 微信认证 1-微信认证  2-
    uchar           cSumMac;                      // 每个账号可以绑定的Mac地址数
    uchar           cError;                       // 密码出错次数
    uint2           nErrorTime;                   // 密码出错后锁定时间
    uint2           nRev;
} ncPortalPar;

// 单位信息
typedef struct ncPortalGroup_s {
    uint4  lGroupid;
    uint4  lModTime;                // 内容最后修改时间
    double dLongitude;              // 经度
    double dLatitude;               // 纬度
    uchar  caGroupName[32];          // 部门名称
    uchar  caDisp[128];              // 显示名
    uchar  caSmsName[32];            // Sms计费单位
    uchar  caMark[128];
    uint4  lLasttime;                // 最后一次登录时间
    uchar  cDb;                      // 0--Db中无记录    1--Db中存在记录
    uchar  cMod;                     // 更新记录 
    uchar  cStatus;                  // 状态 1--正常  9--删除
    uchar  cCurCount;                // 当前统计位置
    ncPortalCount   psPortalCount[NC_PORTAL_MAXCOUNT];   // 统计信息
    ncPortalPar     *psPar;          // 参数
} ncPortalGroup;


// SSID为索引
  
typedef struct ncPortalSsid_s {
    uchar           caSsid[32];                   // SSID名称
    uint4           lGroupid;
    uint4           lSid;
    uchar           cStatus;
    uchar           cSurCount;
    uchar           cRev[6];    
    ncPortalPar     *psPar;                       // 对应的参数
    ncPortalGroup   *psGroup;                     // 对应的部门
    ncPortalCount   psPortalCount[NC_PORTAL_MAXCOUNT]; // Portal统计数据
} ncPortalSsid;


// AP信息
  
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
    uchar           cLogoutFlags;           // 0--无需要离线终端  1--有离线终端
    uchar           cRev;
    uint4           lApIp;       // AP地址
    uint2           nPort;       // 端口
    uint2           nRev;
    ncPortalGroup   *psGroup;
} ncPortalAp;


// 
#define  NCSRV_USERTYPE_FIX      9
#define  NCSRV_USERTYPE_SMS      1
#define  NCSRV_USERTYPE_QQ       2
#define  NCSRV_USERTYPE_WEIXIN   3

// 名字和Groupid唯一, Groupid为0表示通用用户
typedef struct ncPortalUser_s {
    uchar caName[32];               // 登陆名
    uint4 lGroupid;                 // 用户组ID
    uchar caDisp[32];               // 显示名
    uchar caGroup[32];
    uchar caSsid[32];
    uchar caPass[32];               // 密码
    uchar caMac[32];                // 绑定的Mac地址，最多可以5个，每个6字节
    uchar caOpenId[32];
    uchar caLabel[128];
    uint4 lUserid;
    uint4 lLasttime;                // 最后一次登录时间
    uint4 lModTime;                 // 数据修改时间
    uint4 lStarttime;               // 账号启用时间
    uint4 lExpiretime;              // 超时时间
    uchar cDb;                      // 0--Db中无记录    1--Db中存在记录
    uchar cMod;                     // 更新记录 
    uchar cFlags;                   // 记录来源 
    uchar cStatus;                  // 状态 0--正常  9--暂时禁用
    uchar cSum;                     // 绑定的Mac地址数
    uchar cUseType;                 // 0--固定用户  1--短信  2--QQ  3--微信
    uchar cRev[2];
    ncPortalGroup  *psGroup;        // 用户对应的单位 
    ncPortalPar    *psPar;
    ncPortalSsid   *psSsid;
} ncPortalUser;

// 用户和Mac地址对照表

typedef struct ncPortalUserMac_s {
    uint4 lGroupid;                 // 单位名称
    uchar mac[6];                   // Mac地址
    uchar cStatus;                  // 0-有效  1--无效
    uchar cDb;                      // 0-数据库中不存在  1--存在  9-无效的MAC
    uchar caName[32];               // 登陆名
    uint4 starttime;                // 起始时间
    uint4 lasttime;                 // 最后使用时间
    uint4 moditime;                 
    uchar cMod;                     // 0--记录未修改  1--记录已修改
    uchar cFlags;                   // 0--正常  9--绑定失效
    uchar cRev[2];
    ncPortalUser *psUser;
} ncPortalUserMac;

//   Portal在线用户, 以IP地址为索引
// 

#define NCPORTAL_ONLINE_LOGIN          1
#define NCPORTAL_ONLINE_LOGOUTING      2
#define NCPORTAL_ONLINE_DELETE         9
#define NCPORTAL_ONLINE_OFFLINE        0


typedef struct ncPortalOnline_s {
    uchar nasid[24];                // 网关标识
    uint4 lSip;                     // IP地址, 用户上网的内网IP地址
    uint4 lGroupid;
    uint8 lTsid;
    uchar caName[32];               // 登陆名
//    uchar caDisp[32];               // 显示名
    uchar caSsid[32];               // SSID
    uchar caApName[40];             // AP名称或MAC地址 32-34-CB-C0-A1-B7:pronetway-htt
    uchar caAcName[28];             // Ac名称或Ip地址
    uchar caOpenid[32];             // 用户别名，可以为微信Openid或微博登录名、QQ号等
    uint2 nReqId;                   // 认证号
    uint2 nSn;                      // 序列号
    uint4 lAcIp;                    // AC地址
    uint2 nAcPort;                  // AC端口
    uchar caMac[6];                 // Mac地址
    uchar caDev[24];                // 设备
    uchar caOs[16];                 // 操作系统
    uchar caBro[16];                // 浏览器
    uchar caTermType[8];            // 设备类别
    uint8 lBytes[2];                // 流量
    uchar caLang[8];                // 语言
    uint4 sessiontime;              // 超时时间
    uint4 lasttime;                 // 最后操作时间
    uchar caPostUrl[64];            // Post Url https://192.168.20.38/pronline/Msg
    uchar caLabel[128];
    uchar login;                    // 0--Disp, 显示登录界面   1--Login, 已经登录  2--正在离线  9--记录已经删除 0--已经离线
    uchar cStatus;                  // 0--正常     1--被禁用
    uchar cFrom;                    // 来源厂商
    uchar cSave;                    // 0--未保存   1--保存
    uint4 starttime;                // 上线时间
    double  longitude;              // 精度
    double  latitude;               // 维度
    uint4   lGwip;
    uint2   nGwport;
    uint2   nRev;
    ncPortalPar                     *psPar;     // 对应的参数    
    ncPortalUser                    *psUser;    // 对应的用户
    ncPortalSsid                    *psSsid;    // 对应相应部门的SSID
    ncPortalAp                      *psAp;      // 对应的AP
} ncPortalOnline;


typedef struct ncPortalTsid_s {
    uint8             tsid;         // TSID,唯一标识在线用户的数字
    ncPortalOnline    *psOnline;
} ncPortalTsid;


typedef struct ncPortalMacOnline_s {
    uchar             mac[6];       // MAC地址,在在线用户中也应该唯一
    uchar             rev[2];       // 保留
    ncPortalOnline    *psOnline;
} ncPortalMac;



typedef struct ncPortalSummary_s {
    uchar caPortalName[32];            //  Portal名称
    uchar caProdSn[32];                //  产品序列号
    unsigned long   lPortalId;
    unsigned long   lStartTime;                  // 启动时间
    unsigned long   lLastTime;                   // 最后活动时间
    unsigned long   lConnectTime;                // 最后一次连接管理中心时间
    unsigned long   lAdminIp;                    //  管理地址

    uint2 nAdminPort;                  //  管理端口
    uint2 nSumSms;                     // Sms服务器总数
    uint2 nCurSms;                     // 当前SMS序号
    uint2 nSumGjSms;                   // Sms服务器总数
    uint2 nCurGjSms;                   // 当前SMS序号

    uint2 nSumRadius;                  // Radius服务器总数
    uint2 nCurRadius;                  // 当前Radius序号

    uint2 nSumEim;                     // 审计设备总数
    uint2 nCurEim;                     // 当前EIM
    uint2 nSumBakPortal;               // Portal总数

    uint2 nAuthWay;                    // 验证方式  Local,Radius,AC
    uint2 nAuthType;                   // 验证类型  PAP,CHAP
    uchar cRomain;                     // 0--帐号在部门之间不漫游  1--漫游
    uchar cRev[3];
    uint4  lSumPar;
    ncPortalIpPort  psSmsServer[NC_PORTAL_MAXSMS];     // 国内短信网关
    ncPortalIpPort  psSmsGjServer[NC_PORTAL_MAXSMS];   // 国际短信网关
    ncPortalIpPort  psRadServer[NC_PORTAL_MAXRADIUS];  // Radius信息
    ncPortalIpPort  psEimServer[NC_PORTAL_MAXEIM];     // 网络督察信息
    ncPortalIpPort  psBakPortal[NC_PORTAL_MAXEIM];     // 备用Portal状态
    uchar           *pGroupHash;                       // 部门信息
    uchar           *pApHash;
    uchar           *pSsidHash;
    ncPortalPar     *psPar;
} ncPortalSummary;



// 服务器状态
typedef struct ncPortalSrvStatus_s {
    uchar               caName[32];                // 名称
    unsigned long       lIp;                       // IP地址
    unsigned long       lLastTime;                 // 最后一次活动时间
    uint2               nPort;                     // 端口
    uint2               nStatus;                   // 状态
    uint2               nRev[2];
} ncPortalSrvStatus;



// 界面投放策略，策略建议每5分钟检查一次，去掉过时的，生成新的，以提高效率
//  1--Position  2--SSid 
typedef struct ncPortalRule_s {
    uchar           cType;                      // 类别   1-SSID  2-Group  3--Position
    uchar           caSsid[31];                 // SSID
    uchar           caLabel[32];                // 投放标签
    uchar           caPlate[32];                // 模板
    unsigned long   lExpire;                    // 超时时间
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

//  和 HttpServer通信
int ncSrvDispLogin(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvLogin(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvLogOut(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvGetPass(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvTestplate(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvLoginOK(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvLoginError(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvPlate(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
// 和网络督察的通信  ncsrv002.c
int ncSrvGetMyId(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncSrvSynOnlineUser(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);


// 进程处理  ncSrv003.c
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
