/* ProPotral     版权所有: 上海新网程信息技术有限公司 2013
   
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

#define  NCSRV_LNK_ONTIME      50                      /* 定时程序               */
#define  NCSRV_LNK_ONLINE      51
#define  NCSRV_LNK_USERS       53                      /* 在线客户端信息         */
#define  NCSRV_LNK_SYSINFO     54                      /* 存放系统当前各种信息   */
#define  NCSRV_LNK_PLATE       55                      /* 模板存放  */

#define  PORTAL_LNK_SYSINFO     56
#define  PORTAL_LNK_ACINFO      57                     // AC相关信息
#define  PORTAL_LNK_CLIENTS     58                     // 客户端信息
#define  PORTAL_LNK_BLACKUSER   59                     // 账号黑名单
#define  PORTAL_LNK_USERMAC     60                     // 用户Mac地址对照表
#define  PORTAL_LNK_PORTALRULE  61                     // Portal使用策略



#define  NCSRV_STA_SUCCESS     0                       /* 正确              */
#define  NCSRV_STA_RELOGIN     10001                   /* 用户需要重新登录  */
#define  NCSRV_STA_NOEXIST     10002                   /* 用户不存在        */
#define  NCSRV_STA_KEYERROR    10003                   /* 口令出错          */


#define  NCSRV_PORTAL_PROEIM      1
#define  NCSRV_PORTAL_RADIUS      2
#define  NCSRV_PORTAL_PORTAL      3
#define  NCSRV_PORTAL_AC          4

#define  NCSRV_SMSTYPE_PRONETWAY  1              // 新网程短信网关
#define  NCSRV_SMSTYPE_SMSMODEM   2              // 短信猫


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

typedef struct ncPortalAcInfo_s {
    uchar caName[32];                // AC名称
    uint4 lAcip;                     // AC IP地址
    uint2 nAcPort;                   // AC 端口
    uint2 nSn;                       // 序列号, 不断增加
    uchar caAcType[8];               // AC类别  Cisco   GbCom
    uint4 iType;                     // NCSRV_PORTAL_AC NCSRV_PORTAL_RADIUS
    uint4 lLastTime;
    uint4 lCount;
    uchar caKey[32];
} ncPortalAcInfo;

#define NCPORTAL_VENDOR_GBCOM         1
#define NCPORTAL_VENDOR_CISCO         2

// Portal用户信息， Hash表，初始化时放在内存中
// 一个帐号对应多个Mac地址
#define NCPORTAL_USRFROM_MANUAL       1                     // 手工输入
#define NCPORTAL_USRFROM_SMS          2                     // 来自短信获取
#define NCPORTAL_ONEUSER_MAXMAC       5
typedef struct ncPortalUser_s {
    uchar caName[32];               // 登陆名
    uchar caDisp[32];               // 显示名
    uchar caPass[32];               // 密码
    uchar caGroup[32];
    uchar caMac[32];                // 绑定的Mac地址
    uchar caLabel[32];
    uint4 lUserid;
    uint4 lLasttime;                // 最后一次登录时间
    uint4 lModTime;                 // 数据修改时间
    uint4 lStarttime;                // 账号启用时间
    uchar cDb;                      // 0--Db中无记录    1--Db中存在记录
    uchar cMod;                     // 更新记录 
    uchar cFlags;                   // 记录来源 
    uchar cStatus;                  // 状态 0--正常  1--暂时禁用
    uchar cRev[4];
} ncPortalUser;

// 用户和Mac地址对照表
typedef struct ncPortalUserMac_s {
    uchar mac[6];                 // Mac地址
    uchar cStatus;                  // 0-有效  1--无效
    uchar cDb;                      // 0-数据库中不存在  1--存在  9-无效的MAC
    uchar cMod;                     // 0--记录未修改  1--记录已修改
    uchar cRev[3];
    uchar caName[32];               // 登陆名
    uint4 starttime;                // 失效时间
    uint4 lasttime;
    ncPortalUser *psUser;
} ncPortalUserMac;


#define NCPORTAL_BLACK_MAC          1
#define NCPORTAL_BLACK_USER         2

// 用户黑名单
typedef struct ncPortalBlackUser_s {
    uchar ctype;                    // 类别 1-MAC  2-Uername
    uchar caName[31];               // 登陆名或Mac地址
    uint4 validtime;                // 失效时间
} ncPortalBlackUser;



// Portal在线用户, 以IP地址为索引
typedef struct ncPortalOnline_s {
    uint4 lSip;                     // IP地址
    uint4 starttime;                // 上线时间
    uchar caName[32];               // 登陆名
    uchar caSsid[32];               // SSID
    uchar caApName[32];             // AP名称或MAC地址
    uchar caAcName[28];             // Ac名称或Ip地址
    uint4 lAcIp;                    // AC地址
    uint2 nAcPort;                  // AC端口
    uchar caMac[6];                 // Mac地址
    uint2 nReqId;                   // 认证号
    uint2 nSn;                      // 序列号
    uchar caDev[24];                // 设备
    uchar caOs[16];                 // 操作系统
    uchar caTermType[8];            // 设备类别
    uint4 sessiontime;              // 操作时间
    uint8 lBytes[2];                // 流量
    uchar caLang[8];                // 语言
    uint4 lasttime;                 // 最后操作时间
    uchar caPostUrl[64];            // Post Url https://192.168.20.38/pronline/Msg
    uchar caLabel[32];              
    uchar login;                    // 0--Disp, 显示登录界面   1--Login, 已经登录
    uchar cStatus;                  // 0--正常  1--被禁用
    uchar cFrom;                    // 来源厂商
    uchar cSave;                    // 0--未保存   1--保存
    ncPortalUser    *psUser;
#ifndef PAS_X64
    uchar caRev2[4];
#endif    
} ncPortalOnline;







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

// 短信发送信息
typedef struct ncPortalIp_s {
    uint4 lIp;            // 地址
    uint2 nPort;          
    uint2 nType;
    uchar caName[32];            
    uint4 lCount;         // 发送次数
    uint4 lError;         // 不成功次数
    uint4 lLastTime;      // 最后通信时间
} ncPortalIpPort;

// Portal统计信息
typedef struct ncPortalCount_s {
    uint4 lTime;          // 数据采集时间
    uint4 lSmsCount;      // 短信发送次数
    uint4 lDispCount;     // 认证显示
    uint4 lLoginOk;       // 登录页面
    uint4 lLoginError;    // 登录错误
    uint4 lLogOut;        // 退出
    uint4 lAutoLog;       // 自动登录
    uint4 lOnlineUser;    // 在线用户数
    uint4 lRegUser;       // 新增的注册用户
    uint8 lBytes;         // 流量
    uint4 lRev[4];
} ncPortalCount;

typedef struct ncPortalSmsMsg_s {
    char caLang[8];             // 语言
    char caMsg[128];            // 消息格式 [#username#]  [#password#]
} ncPortalSmsMsg;

// 验证用户, 一旦产生密码，放在缓存中  1--向Radius同步 2--保存在数据库中  3--发往维护中心
typedef struct ncPortalAuthUser_s {
    char    caUsername[16];        // 登录名
    char    caPassword[16];        // 登录密码
    uint4   lTime;                 // 时间
} ncPortalAuthUser;


// Portal 综合信息
#define NC_PORTAL_MAXLANG        8
#define NC_PORTAL_MAXSMS         8
#define NC_PORTAL_MAXEIM         8
#define NC_PORTAL_MAXCOUNT     120     // 内存中的采集数据
#define NC_PORTAL_MAXRADIUS      8     // Radius总数
typedef struct ncPortalSummary_s {
    uint4 lStartTime;                  // 启动时间
    uint4 lLastTime;                   // 最后活动时间
    uint2 nSumLang;                    // 语言总数
    uint2 nSumSms;                     // Sms服务器总数
    uint2 nCurSms;                     // 当前SMS序号
    uint2 nCurCount;                   // 统计当前位置
    uint2 nSumRadius;                  // Radius服务器总数
    uint2 nCurRadius;                  // 当前Radius序号
    uint2 nSumEim;                     // 审计设备总数
    uint2 nCurEim;                     // 当前EIM
    uint4 lUserMacExpire;              // 用户Mac地址绑定失效时间
    uint4 lPassCodeTimeOut;            // 密码有效时间
    ncPortalSmsMsg  psSmsMsg[NC_PORTAL_MAXLANG];
    ncPortalIpPort  psSmsServer[NC_PORTAL_MAXSMS];    // 短信网关
    ncPortalIpPort  psRadServer[NC_PORTAL_MAXRADIUS];  // Radius信息
    ncPortalIpPort  psEimServer[NC_PORTAL_MAXEIM];     // 网络督察信息
    ncPortalCount   psPortalCount[NC_PORTAL_MAXCOUNT];  // Portal统计数据
} ncPortalSummary;

// 在线Portal信息
typedef struct ncsPortalOnline_s {
    uchar caName[32];                  // Portal名称
    uint4 lStartTime;                  // 上线时间爱你
    uint4 lLastTime;                   // 最后通信时间
    uint4 lIp;                         // IP地址
    uint2 nCurCount;                   // 当前统计序号
    ncPortalCount  psPortalCount[NC_PORTAL_MAXCOUNT];
} ncsPortalOnline;


// 界面投放策略，策略建议每5分钟检查一次，去掉过时的，生成新的，以提高效率
//  1--Position  2--SSid 
typedef struct ncPortalRule_s {
    uchar   cType;                      // 类别   1-SSID  2-Group  3--Position
    uchar   caSsid[31];                 // SSID
    uchar   caLabel[32];                // 投放标签
    uchar   caPlate[32];            // 模板
    uint4   lExpire;                    // 超时时间
} ncPortalRule;




// 和管理中心通信
int ncmcomSynPortalManInfo(utShmHead *psShmHead);
#endif
