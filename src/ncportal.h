/* ProPotral     版权所有: 上海新网程信息技术有限公司 2013
    
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


#define ACPORTAL_REQ_QMACBIND       0x30                // mac绑定查询请求
#define ACPORTAL_ACK_QMACBIND       0x31                // mac绑定查询应答
#define ACPORTAL_REQ_MACBINDLOGIN   0x32                // 用户上线通知
#define ACPORTAL_REQ_MACBINDLOGOUT  0x34                // 用户下线通知

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
#define  ICT_USER_LOGIN_TSID    65                     // 记录用户登录的tsid以及手机号的关系


// #define  NCM_LNK_WXATOKEN        65                 在 ncportalweb.h中定义
#define  PORTAL_LNK_GROUP       70                     // 单位信息
#define  PORTAL_LNK_PORTALPAR   71                     // Portal参数
#define  PORTAL_LNK_MACONLINE   72                     // 在线Mac地址列表
#define  PORTAL_LNK_PLATERULE   73                     // 页面策略
#define  PORTAL_LNK_LOCMARK     74                     // 位置标签
#define  PORTAL_LNK_MACLOCATION 75                     // MAC地址位置信息
#define  PORTAL_LNK_STYPE       76                     // Stype 类别描述
#define  PORTAL_LNK_LBSAPMAC    77                     // 采集的MAC和AP信息
#define  PORTAL_LNK_RUNCOMM     78                     // 命令执行缓存
#define  PORTAL_LNK_APINDEX     79                     // Ap 索引
#define  PORTAL_LNK_BANDNAME    80                     // 带宽
#define  PORTAL_LNK_TSIDINFO    81                     // 根据TSID为索引保存的信息
#define  NCSRV_STA_SUCCESS     0                       /* 正确              */
#define  NCSRV_STA_RELOGIN     10001                   /* 用户需要重新登录  */
#define  NCSRV_STA_NOEXIST     10002                   /* 用户不存在        */
#define  NCSRV_STA_KEYERROR    10003                   /* 口令出错          */

#define  NCSRV_STATUSCODE_SUCESS        0                   /* 成功                */
#define  NCSRV_STATUSCODE_ALREADYLOGIN  1                   /* 用户已经登录        */
#define  NCSRV_STATUSCODE_FORBITLOGIN   2                   /* 用户在本处被禁止登录  */
#define  NCSRV_STATUSCODE_EXPIRE        3                   /* 用户账号已经过期    */
#define  NCSRV_STATUSCODE_BLACKLIST     4                   /* 用户在黑名单中      */
#define  NCSRV_STATUSCODE_PASSERR       5                   /* 用户密码不正确      */
#define  NCSRV_STATUSCODE_NOEXIST       6                   /* 用户不存在          */
#define  NCSRV_STATUSCODE_EXCEEDMAC     7                   /* 超出MAC地址绑定数量  */
#define  NCSRV_STATUSCODE_EXCEEDTIME    8                   /* 超出每天上网时长     */
#define  NCSRV_STATUSCODE_EXCEEDFLOW    10                  /* 超出每天上网流量     */
#define  NCSRV_STATUSCODE_EXCEEDLOGIN   11                  /* 超出每天登录次数     */

#define  NCSRV_STATUSCODE_PAGEEXPIRE  10000                 /* 页面过期             */
#define  NCSRV_STATUSCODE_OTHERERROR    9                   /* 其它错误             */

#define  NCAPP_STATUS_OK          0
#define  NCAPP_STATUS_ONLINE      1
#define  NCAPP_STATUS_ERRNUM      2                  // 手机号不存在
#define  NCAPP_STATUS_ERRUID      3                  // UID不存在
#define  NCAPP_STATUS_ERRUIDSN    4                  // UID_SN不存在
#define  NCAPP_STATUS_ERRMAC      5                  // MAC地址不存在
#define  NCAPP_STATUS_ERRSID      6                  // SessionID不存在
#define  NCAPP_STATUS_ERRAPP      7                  // APP出错
#define  NCAPP_STATUS_NOONLINE    8
#define  NCAPP_STATUS_ERROR       9
#define  NCAPP_STATUS_BLACKNUM    10
#define  NCAPP_STATUS_BLACKMAC    11
#define  NCAPP_STATUS_ERRKEY      12                 // 密码不正确
#define  NCAPP_STATUS_NOLOGIN     0                  // 用户没有登录



#define  NCSRV_PORTAL_PROEIM      1
#define  NCSRV_PORTAL_RADIUS      2
#define  NCSRV_PORTAL_PORTAL      3
#define  NCSRV_PORTAL_AC          4
#define  NCSRV_PORTAL_SMS         5
#define  NCSRV_PORTAL_GBAP        6
#define  NCSRV_PORTAL_POST        7
#define  NCSRV_PORTAL_MERU        8
#define  NCSRV_PORTAL_CENTERAUTH  9

#define  NCSRV_MARK_LOCATION      'L'               // 位置标签

#define  NCSRV_SMSTYPE_PRONETWAY  1              // 新网程短信网关
#define  NCSRV_SMSTYPE_SMSMODEM   2              // 短信猫
#define  NCSRV_SMSTYPE_PEOPLE     3              // People
#define  NCSRV_SMSTYPE_CENTER     4              // 集中认证平台

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

#define NCSRV_TSIDTYPE_REDIRURL     1    // 重定向URL


typedef struct ncPortalTsidInfo_s {
    uint8   llTsid;                    // TSID   
    uint2   nType;                     // 消息类别
    uchar   caMsg[254];                // 相关信息
    uint4   lLasttime;                 // 最后访问时间
} ncPortalTsidInfo;

    


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
        uchar   caBuf[256];
    } ncPortalAcHead;
// Portal在线用户, 以tsid为索引
typedef struct ictOnlineUser_s {
    uint8 tsid;                     // tsid
	uchar vName[32];                // 手机号 
	uchar loginFlag;                // 0---在线  1---离线  
    uint4 lastTime;                 // 最后登录时间            
} ictOnlineUser;


/* Portal协议中的只  
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
    uchar           caPost[128];               // 认证地址 http://192.168.20.38/qqqq
    unsigned long   lLastTime;
    unsigned long   lCount;
    uchar           caKey[32];
    uchar           caDomain[64];             // 域名  @jn  jn@  根据@的位置决定域名放在什么地方
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


// 界面投放策略，策略建议每5分钟检查一次，去掉过时的，生成新的，以提高效率


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



// 协同服务器信息
#define NC_PORTAL_STA_NORMAL                1    // 正常
#define NC_PORTAL_STA_NETWORKERROR1         2    // 网关网络不正常
#define NC_PORTAL_STA_NETWORKERROR2         3    // Portal和服务器通信不正常
#define NC_PORTAL_STA_OTHERERR              9    // 其它错误


// Portal 综合信息
#define NC_PORTAL_MAXLANG        8
#define NC_PORTAL_MAXSMS         8
#define NC_PORTAL_MAXEIM        32
#define NC_PORTAL_MAXBAKPORTAL  32
#define NC_PORTAL_MAXCOUNT     120     // 内存中的采集数据
#define NC_PORTAL_MAXRADIUS      8     // Radius总数



/* 相关服务器信息
 */


#define NCSRV_LOC_BYIP           1
#define NCSRV_LOC_BYMAC          0

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
#define   NCSRV_PAR_STYPE      3

/*
    参数
    全局          ---- cFlags = 0   lId = 0
    针对全局SSID  ---- 全局SSID
    针对部门      ---- 特定的Groupid
    针对SSID      ---- 部门SSID

*/     
typedef struct ncPortalPar_s {
    uchar           cFlags;                       // 0-全局  1-部门  2-SSID
    uchar           cRev[3];
    uint4           lId;                          // 
    uint4           lSesstime;                    // 上网时间限制
    uint4           lIdletime;                    // 空闲时间
    uint4           lMacbindtime;                 // 用户Mac地址绑定失效时间
    uint4           lSmslocalnum;                 // 每天国内短信发送最多次数
    uint4           lSmsinternum;                 // 每天国际短信发送最多次数
    uint4           lLimittime;                   // 上网时间限制
    uint4           lLimitunit;                   // 限制单位 0-天 1-周  2-月  3-年 9-总共
    uint4           lPasscodetime;                // 密码有效时间
    uchar           caSmsusetime[64];             // 允许注册时间
    uchar           caServicetime[64];            // 允许上网时间
    uchar           caEngMessage[256];            // 英文短信格式
    uchar           caZhMessage[256];             // 中文短信格式
    uint2           nCurCount;                    // 当前统计位置
    uint2           nSumCount;                    // 总共采集数据
    uchar           cAutoAuth;                    // 二次是否免验证
    uchar           cWeixinAuth;                  // 微信认证 1-微信认证和实名认证配合   2--自动认证（关注微信后自动上网)
    uchar           cSumMac;                      // 每个账号可以绑定的Mac地址数
    uchar           cError;                       // 密码出错次数
    uint2           nErrorTime;                   // 密码出错后锁定时间
    uchar           cMacAuth;                     // 无感知认证  0--无  1--Mac认证中心   2--Radius认证  9--未设置
    uchar           cSsidRoam;                    // 不同SSID之间的漫游     9--未设置  0--不漫游   1--漫游
    uchar           cGroupRoam;                   // 不同部门之间的漫游     9--未设置  0--不漫游   1--漫游
    uchar           cAuthWay;                     // 验证方式   1--Radius   2--本地
    uchar           cAutoBindMac;                 // 自动Mac和用户绑定  0--自动   1--手工
    uchar           cExceedMac;                   // 超出绑定数量的处理方式 0--重新认证  1--禁止上网
    uchar           caGroupCode[32];              // 策略组名称
    uint4           lAuthIp[2];                   // 验证地址
    uint2           nAuthPort[2];                 // 验证端口
    uint4           limitflow;                    // 流量限制
    uint4           limitband;                    // 带宽限制
    uint4           limitlogin;                   // 登录次数限制
    uint4           lCheckSum;                    // 校验码 = md5(Record)
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


// AP信息, 增加caAddr--安装地址
  
typedef struct ncPortalAp_s {
    uchar           caApid[32];     // ApID
    uint4           lGroupid;       // 对应的单位ID
    uint4           lLastTime;      // 最后联系时间
    double          longitude;      // 经度
    double          latitude;       // 维度
    uchar           caModel[16];    // 模块
    uchar           caVersion[16];  // 版本
    uchar           caToken[48];    // 设备令牌
    uchar           caMark[64];     // 标签
    uchar           caAddr[64];     // 地址
    uchar           plate[32];
    uchar           line[32];
    uchar           mapid[32];
    uchar           apname[32];   // AP名称
    uchar           ssid[64];
    uint4           lUpTime;        // 运行时间
    uchar           mac[64];        // 设备MAC地址
    uchar           cLogoutFlags;   // 0--无需要离线终端  1--有离线终端
    uchar           cDb:4;          // 0--数据库中不存在  1--数据库中已存在
    uchar           cMod:4;         // 0--未修改          1--修改过
    uint4           lApIp;          // AP地址
    uint2           nPort;          // 端口
    uint2           nDevType;       // 设备类别   
    uint2           nVendor;        // 厂商编码
    uint2           nMarkNum;       // Mark对应的数组序号
    uchar           caRev[8];      // 保留
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

// 名字和Groupid唯一, Groupid为0表示通用用户
typedef struct ncPortalUser_s {
    uchar caName[32];               // 登陆名
    uint4 lStype;
    uchar caDisp[32];               // 显示名
    uchar caGroup[32];
    uchar caSsid[32];
    uchar caPass[32];               // 密码
    uchar caOpenId[64];             // 绑定的公众号，如:微信,QQ等  1:微信  2:QQ  3:微博  1:qwqewqweqweqe,aaa;2:qqqqqqdd,qq
    uchar bindmac[6];
    uchar caLabel[118];
    uint4 lGroupid;                 // 用户组ID
    uint4 lUserid;
    uint4 lLasttime;                // 最后一次登录时间
    uint4 lModTime;                 // 数据修改时间
    uint4 lStarttime;               // 账号启用时间
    uint4 lExpiretime;              // 超时时间
    uint4 lSumConnTime;             // 上网总时长
    uint4 lSumLogin;                // 总登录次数
    uint4 lSumBytes;                // 上网总流量
    uint4 lRev;
    uchar cDb;                      // 0--Db中无记录    1--Db中存在记录
    uchar cMod;                     // 更新记录 0--记录未修改  1--记录已修改
    uchar cFlags;                   // 记录来源 
    uchar cStatus;                  // 状态 0--正常 1--被禁用  2--正在处理 9--暂时禁用
    uchar cSum;                     // 绑定的Mac地址数
    uchar cUseType;                 // 0--固定用户  1--短信  2--QQ  3--微信
    uchar cMaxMac;                  // 可以绑定的最多MAC地址数
    uchar cRev;
    ncPortalGroup  *psGroup;        // 用户对应的单位 
    ncPortalPar    *psPar;
    ncPortalSsid   *psSsid;
} ncPortalUser;

// 用户和Mac地址对照表

typedef struct ncPortalUserMac_s {
    uint4 lGroupid;                 // 单位类别
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

#define NCPORTAL_CAUSE_BYADMIN         1
#define NCPORTAL_CAUSE_TIMEOUT         2
#define NCPORTAL_CAUSE_USERREQUEST     3
#define NCPORTAL_CAUSE_IDLETIMEOUT     4
#define NCPORTAL_CAUSE_HOSTREQUEST     5
#define NCPORTAL_CAUSE_FLOWOUT         6
#define NCPORTAL_CAUSE_TIMEEXCEED      7       // 超出时间


#define NCPORTAL_LOGIN_WEBAUTH         1       // Web认证
#define NCPORTAL_LOGIN_WEBAUTOAUTH     2       // Web自动认证
#define NCPORTAL_LOGIN_MACAUTH         3       // MAC地址认证
#define NCPORTAL_LOGIN_WEIXINAUTH      4       // 微信认证
#define NCPORTAL_LOGIN_APPAUTH         5       // APP认证
#define NCPORTAL_LOGIN_CARDAUTH        6       // 证件认证
#define NCPORTAL_LOGIN_MEMBERAUTH      7       // 账号认证

typedef struct ncPortalOnline_s {
    uchar   nasid[24];                // 网关标识
    uint4   lSip;                     // IP地址, 用户上网的内网IP地址
    uint4   lGroupid;                 // 单位ID
    uint8   lTsid;                    // SessionID
    uchar   caName[32];               // 登陆名
    uchar   caSsid[32];               // SSID
    uchar   caApName[40];             // AP名称或MAC地址 32-34-CB-C0-A1-B7:pronetway-htt
    uchar   caAcName[28];             // Ac名称或Ip地址
    uchar   caOpenid[32];             // 用户别名，可以为微信Openid或微博登录名、QQ号等
    uint2   nReqId;                   // 认证号
    uint2   nSn;                      // 序列号
    uint4   lAcIp;                    // AC地址
    uint2   nAcPort;                  // AC端口
    uchar   caMac[6];                 // Mac地址
    uchar   caDev[24];                // 设备
    uchar   caOs[16];                 // 操作系统
    uchar   caBro[16];                // 浏览器
    uchar   caTermType[8];            // 设备类别
    uint8   lBytes[2];                // 流量          本次流量
    uint4   lSumBytes;                // 本时间周期总流量
    uint4   lSumTime;                 // 本时间周期上网时间
    uchar   caLang[8];                // 语言
    uint4   logintime;                // 认证时间
    uint4   lasttime;                 // 最后操作时间
    uchar   caPostUrl[64];            // Post Url https://192.168.20.38/pronline/Msg
    uchar   caMark[132];              // 标签
    uchar   caSrc[16];                // 接入厂商
    uchar   caUid[32];                // UID_SN
    uchar   caRev[12];                // 保留
    uint4   lOutip;                   // 外网IP地址
    uchar   caKey[24];                // 密码
    uint4   lAcctTime;                // 计费信息发送时间
    uchar   cAuthType;                // NC_PORTALAUTH_CHAP,NC_PORTALAUTH_PAP
    uchar   cBand;                    // 0--无  >0 设置了带宽
    uchar   cRev[2];                  // 保留
    uchar   login;                    // 0--Disp, 显示登录界面   1--Login, 已经登录  2--正在离线  9--已离线  0--未登陆
    uchar   cStatus;                  // 0--正常     1--被禁用
    uchar   cFrom;                    // 来源厂商
    uchar   cSave;                    // 0--未保存   1--保存
    uint4   starttime;                // 上线时间
    uint4   conntime;                 // 上网时间
    double  longitude;                // 精度 x
    double  latitude;                 // 维度 y
    uchar   mapid[16];                // MacID
    uchar   platename[16];            // 正在使用的模板ID
    uint4   locationtime;             // 定位信息发送时间
    uint4   lGwip;                    // 网关IP
    uint2   nGwport;                  // 网关端口
    uchar   cCause;                   // 退出原因 
    uchar   cAuthWay;                 // NCPORTAL_LOGIN_WEBAUTH NCPORTAL_LOGIN_WEBAUTH NCPORTAL_LOGIN_APPAUTH
    uint4   lStype;                   // 用户类别
    uint4   lSessionTime;             // Session Time
    uint4   lIdelTime;                // lIdelTime
    uchar   caGroupCode[32];          // 场所端编码
    ncPortalPar                     *psPar;     // 对应的参数,当前起作用的参数
    ncPortalUser                    *psUser;    // 对应的用户
    ncPortalSsid                    *psSsid;    // 对应相应部门的SSID
    ncPortalAp                      *psAp;      // 对应的AP
    ncPortalAcInfo                  *psAc;
} ncPortalOnline;


/* 位置标签

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
    uint8             tsid;         // TSID,唯一标识在线用户的数字
    ncPortalOnline    *psOnline;
} ncPortalTsid;


typedef struct ncPortalMacOnline_s {
    uchar             mac[6];       // MAC地址,在在线用户中也应该唯一
    uchar             rev[2];       // 保留
    ncPortalOnline    *psOnline;
} ncPortalMac;

/* 用户类别
   数据来自表: ncmautotype
   
 */
#define NCSRV_MATCH_EQUAL        0
#define NCSRV_MATCH_START        1
#define NCSRV_MATCH_END          2
#define NCSRV_MATCH_INCLUDE      3
#define NCSRV_MATCH_ANY          4

typedef struct ncPortalStype_s {
    uchar groupcode[30];
    uchar groupmatch;                 // 0--全匹配  1--起始  2--结束  3--包含
    uchar grouplen;
    uchar ssid[30];
    uchar ssidmatch;                  // 0--全匹配  1--起始  2--包含
    uchar ssidlen;
    uint4 stype;                      // Stype编码
    uchar shopcode[4];                // 商家类别 2位
    uchar namecode[8];                // 编码6位
    uchar ucode[32];                  // 对应的编码
    uchar descr[32];                  // 描述
    ncPortalPar     *psPar; 
} ncPortalStype;



typedef struct ncPortalBand_s {
    int  id;
    char name[32];
} ncPortalBand;


// App验证相关信息
typedef struct ncPortalAppAuth_s {
    char sjname[16];              // 审计厂商来源
    char host[32];                // 验证服务器IP
    char port[16];                // 端口
    char path[100];               // 路径
    char prot[16];                // 协议 http,https
    char reurl[128];
    char reapp_url[128];
    uint4 lSessionTime;
    char caRev[128];              // 保留
} ncPortalAppAuth;

// 综合信息
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
    uint4  lSumPlateRule;
    uint4  lSumLocation;
    uint4  lSumStype;
    uint4  lSumBand;                  // 总的带宽数
    uchar  caControl[64];             // 控制位
    ncPortalIpPort  psSmsServer[NC_PORTAL_MAXSMS];           // 国内短信网关
    ncPortalIpPort  psSmsGjServer[NC_PORTAL_MAXSMS];         // 国际短信网关
    ncPortalIpPort  psRadServer[NC_PORTAL_MAXRADIUS];        // Radius信息
    ncPortalIpPort  psEimServer[NC_PORTAL_MAXEIM];           // 网络督察信息
    ncPortalIpPort  psBakPortal[NC_PORTAL_MAXBAKPORTAL];     // 备用Portal状态
    ncPortalAppAuth sAppAuth;                              // App认证信息
    uchar           *pGroupHash;                       // 部门信息
    uchar           *pApHash;
    uchar           *pSsidHash;
    ncPortalPar     *psPar;                            // 系统参数
    ncPortalPlateRule *psPlateRule;
    ncLocationMark    *psLocation;                     // 位置信息
    ncPortalStype     *psStype;                        // 用户类别
    ncPortalBand      *psBand;
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


// 位置标签
typedef struct ncLbsLocApMark_s {
    char        caMark[32];
    uint4       lMacCount;        // Mac地址数量
} ncLbsLocMark;


//     人流统计
//     当markid变化时，写日志   268
//     mac,starttime,endtime,mark
typedef struct ncLbsMacLocation_s {
    uchar   mac[6];                                   // Mac地址，为索引   
    uchar   apmac[6];                                 // AP Mac地址
    char    lrssi;                                    // 信号强度,最小
    char    urssi;                                    // 信号强度,最大
    char    noise;
    char    channel;
    uint4   starttime;                                // 起始时间
    uint4   gpstime;
    ncPortalAp *psAp;
    uint4   lasttime;                                 // 最后一次访问时间
    uchar   cFlags;                                   // 1--离线
    uchar   cDb;                                      // 0--未保存到日志  1--已保存到日志
    uchar   caBssid[6];                               // 关联的Bssid
    uchar   cAssocicated;                             // 关联  1--Yes  0--No
    uchar   encrypt;
    uchar   distance;
    uchar   mutype;                                   // 2-终端  3--热点
    uchar   vtype;                                    // 身份类别 0--真实身份  1--虚拟身份
    uchar   caRev[3];
    uchar   longitude[12];
    uchar   latitude[12];
    uchar   ssid[32];
    uchar   termtype[32];
    uchar   name[64];                                 // 身份内容
} ncLbsMacLocation;

typedef struct ncLbsMacRecord_s {
    ncLbsMacLocation *psMacLoc;
} ncLbsMacRecord;

typedef struct ncSrvComBuf_s {
    uchar     status;          // 状态 0--无数据 1--未执行  2--正在执行   3--执行完成
    uchar     fun;             // 执行的功能    1--LOGON    2--LOGOFF  3--GET
    uchar     rc;              // 命令返回状态  0--不成功   1--成功
    uchar     rev;
    uint4     lasttime;        // 起始时间    
    uchar     command[128];    // 要执行的命令
    uchar     buf[128];        // 命令结果内容
} ncSrvComBuf;

typedef struct ncSrvComBufHead_s {
    int4     iMaxCom;
    int4     iCurNum;
    int4     iCurDo;
    uint4    lStartTime;
    uint4    lLastTime;
    int4     iPid;
    uint4    lDoStartTime;         // 命令处理开始时间
    int4     iStatus;              // 状态
    uchar    caAcName[16];    
    ncSrvComBuf *psBuf;
} ncSrvComBufHead;


// ncsrv000.c
ncPortalPar *ncSrvGetPar(ncPortalSummary *psSumm,int iType,uint4 lGroupid);
ncPortalUser *ncSrvSynUserInfo(utShmHead *psShmHead,
                               uchar *pGroupCode,         // 部门编码, NULL, lGroupid =0
                               uchar *pSsid,              // SSID, NULL -- 无SSID
                               char *pUserName,           // 用户名
                               char *pPassword,           // 密码
                               char *pMark,               // NULL 表示Mark不变
                               char *pDisp,               // 显示名
                               int  iUsrType,             // 用户类别  9--固定用户  1--短信验证  2--QQ  33--微信
                               int  iMacUser,
                               int  iFlags,               // 用户状态  0--正常      9--禁用
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
