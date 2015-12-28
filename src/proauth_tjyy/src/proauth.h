/* 集中验证平台数据结构      */
#ifndef  __PROAUTHDEF__
#define  __PROAUTHDEF__

#define PROAUTH_LNK_CLIENT          1      /* 单位 */
#define PROAUTH_LNK_MOBILE          2      /* 手机验证用户 */
#define PROAUTH_LNK_GROUP           3      /* 单位组 */
#define PROAUTH_LNK_LB            4      /* 单位类别 */
#define PROAUTH_LNK_FAC           5      /* 接入厂商 */
#define PROAUTH_LNK_MAC           6      /* MAC地址库 */
#define PROAUTH_LNK_SMSCONENT     7      /* 短信内容队列 */
#define NC_LNK_ADMIPLIST    72     /* 管理员IP地址, 用来记录登录失败的IP地址  */

/* 客户端信息资料  */
typedef struct authClient_s {
    char           servicecode[16];        // 场所编码
    unsigned long  userid;              // 用户ID
    char           password[32];        // 口令
    char           dispname[64];        // 显示名
    unsigned long  groupid;             // 组ID
    unsigned long  gtype;            // 单位类别
    char jointype[4];                 // 接入方式
} authClient;
typedef struct authGroup_s {
    unsigned long  groupid;              // 组ID
    char           groupname[64];        // 显示名
    unsigned long  pid;             // 组ID
} authGroup;

typedef struct authFac_s {
    char           code[16];        // 代码
    char           dispname[64];    // 名称
    char           key[32];         // 数据加密KEY
} authFac;
/*手机验证用户*/
typedef struct authMobileUser_s {
	  char   mobile[20];               //手机号
	  char   password[20];             //密码
	  unsigned long lasttime;          //最近时间
	  char   mac[20];                  //mac地址  
	  char   servicecode[16];          //场所代码     
    unsigned long lDel;              //有效标记  0-有效 1-116短信状态 表明已发送验证信息还没验证
} authMobileUser;

/*mac用户库*/
typedef struct authMacUser_s {
	  char   mac[20];                  //mac地址  
	  char   mobile[20];               //手机号
	  char   dispname[32];             //密码
	  char   idno[20];
	  char idtype[8];
	  char servicecode[16];
	  unsigned long lasttime;          //最近时间
} authMacUser;

typedef struct authSmsContent_s {
	  char   mobile[20];               //手机号
    char   content[256];             //短信内容
    char   fws[16];                  //短信服务商通道
} authSmsContent;
typedef struct authClientId2Code_s {
	  unsigned long id;               //单位ID            //短信内容
    char   code[32];                  //场所代码
} authClientId2Code;


#endif
