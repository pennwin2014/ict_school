#define PROAUTH_LNK_CLIENT          99      /* 单位 */
#define PROAUTH_LNK_GROUP           98     /* 单位组 */
#define PROAUTH_LNK_LB              97      /* 单位类别 */
#define PROAUTH_LNK_CLIENTIDCODE  96      /* 单位id和场所code对照表*/

/* 客户端信息资料  */
typedef struct authClient_s {
    char           servicecode[32];     // 场所编码
    unsigned long  userid;              // 用户ID
    char           dispname[128];        // 显示名
    unsigned long  groupid;             // 组ID
    unsigned long  gtype;            // 单位类别
    double longitude;
    double latitude;
} authClient;
typedef struct authGroup_s {
    unsigned long  groupid;              // 组ID
    char           groupname[64];        // 显示名
    unsigned long  pid;             // 组ID
} authGroup;

typedef struct authFac_s {
    char           code[32];        // 代码
    char           dispname[64];    // 名称
    char           key[32];         // 数据加密KEY
} authFac;
typedef struct authClientId2Code_s {
	  unsigned long id;               //单位ID            //短信内容
    char   code[32];                  //场所代码
} authClientId2Code;
