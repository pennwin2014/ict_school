/* ProPotral     版权所有: 上海新网程信息技术有限公司 2013
   
*/
#ifndef __PROPORTALWEBDEF__
#define __PROPORTALWEBDEF__

#define MODULEUPPATH "/home/ncmysql/ncsrv/upload"
#define MODULEPATH "/home/ncmysql/ncsrv/html"
#define MODULEAPPPATH "/home/ncmysql/ncsrv/plate"
#define MODULEBINPATH "/home/ncmysql/ncsrv/bin"
#define SHOPIMGPPATH "/home/ncmysql/ncsrv/html"
#define SYSTEMPATH "/home/ncmysql/ncsrv"
/*
总体运行状态
*/
typedef struct totalstatus_s{
	  long ip;                // ip
    char sero[20];          // 序列号
    int4 runStatus;         // 1:主机 2:备机 
    int4 netStatus;         // 1:在线 2:离线  
    int4 portalStatus;      // 1:主机正常 2:主机故障   3:备机正常 4:备机故障
	  int4 radStatus;         // 1:主机正常 2:主机故障   3:备机正常 4:备机故障 
    int4 smsStatus;         // 1:主机正常 2:主机故障   3:备机正常 4:备机故障 
    int4 logStatus;         // 1:主机正常 2:主机故障   3:备机正常 4:备机故障 
    int4 starttime;         // 开始时间
    int4 radiuscount;       // Radius认证次数
    int4 localsms;          // 国内短信数量
    int4 foreignsms;        // 国际短信数量
    int4 logincount;        // 认证次数
    int4 userlogin;         // 登录用户数
    int4 acredirect;        // AC重定向数量
    int4 lasttime;          // 最后一次通信时间(系统时间)
}totalstatus;

#define  NCM_LNK_WXATOKEN        65             //AccessToken
#define  NCM_LNK_WEIXIN          66             //微信配置
#define  NCM_LNK_SHOPTYPE        67             //商店类别对照表
#define  NCM_LNK_QQ              68             //QQ信息表

typedef struct ncwxaccesstoken_s {
	  uint4  wid;                             //厂商id                     
	  uchar  access_token[256];               //通行证
 	  long   systime;                         //系统时间
} ncwxaccesstoken;

//微信服务器结构
typedef struct ncmweixin_s {
	  uchar  account[16];                     //账号    
	  uint4  wid;                             //厂商id     
	  uchar  name[64];                        //厂商名  
	  uchar  token[48];                       //接入Token    
//	  uchar  account[32];                     //账号    
	  uchar  appid[32];                       //appid    
	  uchar  appsecret[64];                   //appsecret    
	  uchar  sendcont[32];                    //我要上网   
	  uchar  redurl[255];                     //重定向URL  
	  uchar  usinfo[8];                       //是否获取用户信息 Yes  No                    
	  uchar  access_token[528];               //通行证     1.5小时更新一次DB
 	  long   systime;                         //系统时间
 	  long   tokentime;                       //access_token加入更新时间
 	  uint4  authstat;                        //是否注册  0未注册  1已注册
} ncmweixin;

//商店类别对照表
typedef struct ncmshoptype_s {
	  uint4  id;                                 //id     
	  uint4  shopid;                             //商店id     
	  uint4  sgroupid;                           //小类id     
	  uint4  sid;                                //商品id     
} ncmshoptype;
typedef struct ncmqqinfo_s {
	  uint4  qid;                             //id                     
	  uchar  pname[64];                       //portalname
} ncmqqinfo;

int ncmAddTest(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmModule_upload(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmModuleCreateDef(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmTreeFac(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWebModuleSet(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWebModuleManSelect(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmModuleApply(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

int ncmWebModuleDefine(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWebModuleDefineSelect(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

int ncmModuleCreate(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWebReplaceOne(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWebShowModuleAddModify(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmModuleDefInfoSave(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

int ncm_WebShowModuleApp(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);  //s

//系统管理
int ncmSysUserlist_v4(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmDispRoleFunlist_v4(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmRolelist_v4(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWebAdminDispUserForm(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmAdminUserSave(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWebAdmQyeSysLog(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWebGroupComp(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmSysReflesh_v4(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmFirstMenuConfig(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmjump_gid(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWebDispModiPass(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWebModiPass(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWriteSysLog(utMsgHead *psMsgHead,char *username,char *action,char *result,char *msg);
int ncmSysUserRoleConfig(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWebAuth(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

int ncmTreeGroup_comp_v4(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

//系统设置
int ncmWebSystemSet_base(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWebSystemSet_dev(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWebEtheSelect(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmSysStatus_Alarmset(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmDiskRateSave_par(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWebSystemSave_base(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);


int ncmPortal_ParaSet(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmPortalParaSave_par(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmSms_Server_ParaSet(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmSmsParaSave_par(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmHttp_Server_ParaSet(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmHttpParaSave_par(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmAC_Server_ParaSet(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmACParaSave_par(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmAC_Server_SelfParaSet(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmRadius_Server_ParaSet(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmRadiusParaSave_par(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

int ncmWebShowUserMan(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncmWebGroupSelectComp(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

//在线信息
int ncmWebShowOnlineUserInfo(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

//组管理 s
int ncmWeb_groupList(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

//日志 s
int ncm_webshowSMSlogList(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncm_userlog(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

//w 商城
int ncmLevels(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmDiscounts(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmDiscounts_Details(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmDiscounts_Coupons(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char uid2[32], char delimg[16]);

int ncmShops_Discounts(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmCategories(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmCategories_Types(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncmPersonals(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmPersonals_Info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmPersonals_Mark(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmPersonals_Consump(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmPersonals_Coupon(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char uid2[32], char delimg[16]);
int ncmPersonals_Attention(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead,char uid2[32],char delimg[16]);
int ncmPersonals_Collection(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char uid2[32],char delimg[16]);


int ncmLogos_detail(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmLogos(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmProducts(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmLevels_Logos(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmLevels_Logos_Recommend(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmShops(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_Addattentions(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_Addcollections(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_Addcoupons(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//s
// 楼层信息
int ncm_webreadfloor_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// 商品大类
int ncm_webcomgroup_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomgroup_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webComGroupImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// 商品小类
int ncm_webcomtype_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomtype_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webComTypeImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// 商标类别
int ncm_webcomindex_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomindex_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomindex_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomindex_update_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// 商品
int ncm_webcomodity_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webComOdityImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// 商店
int ncm_webcomshop_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webComShopImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// 商城用户信息
int ncm_webcomshopuser_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//int ncm_webComShopUserImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// 用户消费信息
int ncm_webcomconsump_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// 优惠劵
int ncm_webcomcoupon_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webComCouponImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// 商城用户关注信息
int ncm_webcomshopattent_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//int ncm_webComShopAttentImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// 商城用户收藏信息
int ncm_webcomshopcollect_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//int ncm_webComShopCollectImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// 商城活动表
int ncm_webcomactive_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomactive_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

// 新添商城活动信息
int ncm_webcomactive_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomactive_update_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

// 新添优惠劵信息
int ncm_webcomcoupon_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomshop_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomodity_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

/// 新添优惠劵使用表信息
int ncm_webcomcouponuse_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

// 品牌信息
int ncm_webcombrand_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcombrand_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webComBrandImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

//// 用户详细上网报告
//int ncm_web_App_intSurfDetailReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//// 上网人数和上网流量报告
//int ncm_web_App_intSurfStatisticsReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//// 设备类型报告
//int ncm_web_App_deviceModelReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//// 短信发送报告
//int ncm_web_App_smsDetailReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);



//用户管理s
int ncm_blackuser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncmShopUserOnlineFlag(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_limitband(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);


int ncmWebReplace(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// 和管理中心通信
int ncmcomSynPortalManInfo(utShmHead *psShmHead);
int ncmComUpdateportalTimeLimit(utShmHead *psShmHead,uint4 lIntertime,uint4 lFreetime,uint4 lIdletime,uint4 lTimeout,uint4 lFlags);
int ncmcomSynPortalOnlineInfo(utShmHead *psShmHead);

//int ncmGetMyServicecodeByName(utShmHead *psShmHead,char *pName);
char *ncmGetSmsUserlogInfo(utShmHead *psShmHead,uint4 *lSynTime0,uint4 *lSynPos);
int ncmcomUploadSmsUserlog(utShmHead *psShmHead);
int ncmcomUploadSmslog(utShmHead *psShmHead);
int ncmcomUploadClientUserInfo(utShmHead *psShmHead);





// 商城二级活动信息-0303
int ncm_webcomseries_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomseries_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webComSeriesImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_ShowSmsStatisticsReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead,char sdate[32],char edate[32],char makepath[255]);
int ncm_web_MC_os_chart(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

//sww
int ncm_web_Place_smslog_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_web_Place_smslog_count(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_web_Place_userlog_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_web_Place_smslog_rule(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_web_Place_smslog_custmer(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_website_visit_type(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_web_App_intDetailReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 用户详细上网报告-0307
int ncm_web_App_intStatisticsReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 上网人数和上网流量报告-0307
int ncm_web_App_intDeviceReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 设备类型报告-0307
int ncm_web_App_smsDetailReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 短信发送报告-0307
int ncm_web_App_smsStatisticsReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 每小时短信发送报表=0307

int ncmcomweixin(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 


int ncm_yuyuan_home(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_portplate(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_ShopType(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 商品小类对照表


int ncm_websrvgroup_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 单位管理-0423
int ncm_websrvgroupap_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // AP管理-0423
int ncm_websrvgroupssid_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // SSID管理-0423
int ncmcomqq(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // SSID管理-0423
int ncm_webweixin_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);  //微信配置
int ncmcomqqGetOpenid(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);  //获取QQ Openid

//短信支付管理
int ncm_smspay(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//商家信息
int ncm_groupInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_webcomindex2_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //广告管理2-0508
int ncm_webarea_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //区域信息-0509
int ncm_webcomodity_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmcomqqAuthGetAccessToken(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);

int ncmcomqqtest(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncm_webcomshop_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmShopUserOnlineInsert(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmcomromelocation(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncmcomromelocationAjax(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);

int ncm_websrvgroup_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //单位管理信息-0613
int ncm_webuserman_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //用户管理信息-0613
int ncm_group_manager_DoorTree_comp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //区域树-0616

int ncm_webcomindex3_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 广告第二版-0609
int ncm_webcomindex3_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_webcomindex4_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 预园广告第三版-0617
int ncm_webcomindex4_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

char *ncmShopGetOnlineMark(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_webcommark_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 地图标签管理-0618

int ncm_portallocmark(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 坐标

int ncm_webcomindex5_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 太保广告管理-0620
int ncm_webcomindex5_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_web_App_intSsidReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ssid统计报表-0620


int ncm_web_Place_white_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 黑白名单-0604

int ncm_webshopgtype_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 类别，商超管理表-0623

int ncm_shopgroup(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //商家联盟
int ncm_shopgtype(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //单位组管理
int ncmdeptmenu(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //组菜单权限
int ncmdeptmenu(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //组菜单权限
int ncm_srvpar(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 参数设置
int ncm_srvgroupssid(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // SSID
int ncm_srvgroup(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 用户区
int ncm_autopri(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 自定义权限
int ncm_initmenu(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 自定义权限
int ncm_initsubmenu(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 自定义权限

int ncmDispRoleFunlist_v5(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 权限设置-0627
int ncmWebAdmin(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); 

int ncm_webusermac_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 用户mac

int ncm_web_Place_alarmlog_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 告警日志-0703
int ncm_web_App_intTrafReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 用户流量统计报表-0703
int ncmWebShowStaffMan(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 用户管理-0703
char *ncmWebAutoDefGet16(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead,char stype[4]);
//int ncmRolelist_sww(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead,char caRightcheck[2048], char caRolename[64], char caUpdate[32]);
int ncm_webuserman_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webuserman_dload(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_yuyuan_home_ajax(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_system_reset(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_web_Place_plate_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 模板-0728
int ncm_web_Place_plist_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//int ncm_web_Place_page_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_web_Place_prule_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_webautodef_ajax(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmGetPlateidByTsid(utShmHead *psShmHead,long long tsid);

int ncm_questions(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);//问卷调查
int ncmShops_shbook(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);//问卷调查

int ncm_webblackdr_next(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 黑白名单导入-0812

int ncm_web_Place_maclog_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //场所端MAC日志-0820
int ncm_web_App_apMacCount_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 某一时间AP的MAC数量-0820
int ncm_webmenu_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 系统主菜单管理-0821
int ncm_websubmenu_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 系统子菜单管理-0821

int ncm_web_App_userCount_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 一天中一人的来访次数-0827

int ncm_sms_send_chart(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 20141031短信发送图表

int ncm_webcommall_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 楼层信息-20141114
int ncm_webComMallImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int query_ap_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 增删改查ap信息


int ncm_sms_send_register_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

// 丹尼斯注册及验证
int Denis_Register(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncSrvUserGetPass(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);


#endif
