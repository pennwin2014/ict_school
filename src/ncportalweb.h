/* ProPotral     ��Ȩ����: �Ϻ���������Ϣ�������޹�˾ 2013
   
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
��������״̬
*/
typedef struct totalstatus_s{
	  long ip;                // ip
    char sero[20];          // ���к�
    int4 runStatus;         // 1:���� 2:���� 
    int4 netStatus;         // 1:���� 2:����  
    int4 portalStatus;      // 1:�������� 2:��������   3:�������� 4:��������
	  int4 radStatus;         // 1:�������� 2:��������   3:�������� 4:�������� 
    int4 smsStatus;         // 1:�������� 2:��������   3:�������� 4:�������� 
    int4 logStatus;         // 1:�������� 2:��������   3:�������� 4:�������� 
    int4 starttime;         // ��ʼʱ��
    int4 radiuscount;       // Radius��֤����
    int4 localsms;          // ���ڶ�������
    int4 foreignsms;        // ���ʶ�������
    int4 logincount;        // ��֤����
    int4 userlogin;         // ��¼�û���
    int4 acredirect;        // AC�ض�������
    int4 lasttime;          // ���һ��ͨ��ʱ��(ϵͳʱ��)
}totalstatus;

#define  NCM_LNK_WXATOKEN        65             //AccessToken
#define  NCM_LNK_WEIXIN          66             //΢������
#define  NCM_LNK_SHOPTYPE        67             //�̵������ձ�
#define  NCM_LNK_QQ              68             //QQ��Ϣ��

typedef struct ncwxaccesstoken_s {
	  uint4  wid;                             //����id                     
	  uchar  access_token[256];               //ͨ��֤
 	  long   systime;                         //ϵͳʱ��
} ncwxaccesstoken;

//΢�ŷ������ṹ
typedef struct ncmweixin_s {
	  uchar  account[16];                     //�˺�    
	  uint4  wid;                             //����id     
	  uchar  name[64];                        //������  
	  uchar  token[48];                       //����Token    
//	  uchar  account[32];                     //�˺�    
	  uchar  appid[32];                       //appid    
	  uchar  appsecret[64];                   //appsecret    
	  uchar  sendcont[32];                    //��Ҫ����   
	  uchar  redurl[255];                     //�ض���URL  
	  uchar  usinfo[8];                       //�Ƿ��ȡ�û���Ϣ Yes  No                    
	  uchar  access_token[528];               //ͨ��֤     1.5Сʱ����һ��DB
 	  long   systime;                         //ϵͳʱ��
 	  long   tokentime;                       //access_token�������ʱ��
 	  uint4  authstat;                        //�Ƿ�ע��  0δע��  1��ע��
} ncmweixin;

//�̵������ձ�
typedef struct ncmshoptype_s {
	  uint4  id;                                 //id     
	  uint4  shopid;                             //�̵�id     
	  uint4  sgroupid;                           //С��id     
	  uint4  sid;                                //��Ʒid     
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

//ϵͳ����
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

//ϵͳ����
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

//������Ϣ
int ncmWebShowOnlineUserInfo(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

//����� s
int ncmWeb_groupList(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

//��־ s
int ncm_webshowSMSlogList(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncm_userlog(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

//w �̳�
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
// ¥����Ϣ
int ncm_webreadfloor_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// ��Ʒ����
int ncm_webcomgroup_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomgroup_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webComGroupImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// ��ƷС��
int ncm_webcomtype_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomtype_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webComTypeImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// �̱����
int ncm_webcomindex_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomindex_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomindex_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomindex_update_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// ��Ʒ
int ncm_webcomodity_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webComOdityImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// �̵�
int ncm_webcomshop_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webComShopImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// �̳��û���Ϣ
int ncm_webcomshopuser_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//int ncm_webComShopUserImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// �û�������Ϣ
int ncm_webcomconsump_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// �Ż݄�
int ncm_webcomcoupon_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webComCouponImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// �̳��û���ע��Ϣ
int ncm_webcomshopattent_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//int ncm_webComShopAttentImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// �̳��û��ղ���Ϣ
int ncm_webcomshopcollect_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//int ncm_webComShopCollectImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// �̳ǻ��
int ncm_webcomactive_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomactive_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

// �����̳ǻ��Ϣ
int ncm_webcomactive_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomactive_update_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

// �����Ż݄���Ϣ
int ncm_webcomcoupon_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomshop_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcomodity_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

/// �����Ż݄�ʹ�ñ���Ϣ
int ncm_webcomcouponuse_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

// Ʒ����Ϣ
int ncm_webcombrand_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webcombrand_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webComBrandImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

//// �û���ϸ��������
//int ncm_web_App_intSurfDetailReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//// ����������������������
//int ncm_web_App_intSurfStatisticsReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//// �豸���ͱ���
//int ncm_web_App_deviceModelReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//// ���ŷ��ͱ���
//int ncm_web_App_smsDetailReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);



//�û�����s
int ncm_blackuser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncmShopUserOnlineFlag(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_limitband(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);


int ncmWebReplace(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
// �͹�������ͨ��
int ncmcomSynPortalManInfo(utShmHead *psShmHead);
int ncmComUpdateportalTimeLimit(utShmHead *psShmHead,uint4 lIntertime,uint4 lFreetime,uint4 lIdletime,uint4 lTimeout,uint4 lFlags);
int ncmcomSynPortalOnlineInfo(utShmHead *psShmHead);

//int ncmGetMyServicecodeByName(utShmHead *psShmHead,char *pName);
char *ncmGetSmsUserlogInfo(utShmHead *psShmHead,uint4 *lSynTime0,uint4 *lSynPos);
int ncmcomUploadSmsUserlog(utShmHead *psShmHead);
int ncmcomUploadSmslog(utShmHead *psShmHead);
int ncmcomUploadClientUserInfo(utShmHead *psShmHead);





// �̳Ƕ������Ϣ-0303
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

int ncm_web_App_intDetailReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // �û���ϸ��������-0307
int ncm_web_App_intStatisticsReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ����������������������-0307
int ncm_web_App_intDeviceReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // �豸���ͱ���-0307
int ncm_web_App_smsDetailReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ���ŷ��ͱ���-0307
int ncm_web_App_smsStatisticsReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ÿСʱ���ŷ��ͱ���=0307

int ncmcomweixin(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 


int ncm_yuyuan_home(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_portplate(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_ShopType(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ��ƷС����ձ�


int ncm_websrvgroup_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ��λ����-0423
int ncm_websrvgroupap_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // AP����-0423
int ncm_websrvgroupssid_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // SSID����-0423
int ncmcomqq(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // SSID����-0423
int ncm_webweixin_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);  //΢������
int ncmcomqqGetOpenid(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);  //��ȡQQ Openid

//����֧������
int ncm_smspay(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//�̼���Ϣ
int ncm_groupInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_webcomindex2_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //������2-0508
int ncm_webarea_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //������Ϣ-0509
int ncm_webcomodity_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmcomqqAuthGetAccessToken(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);

int ncmcomqqtest(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncm_webcomshop_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmShopUserOnlineInsert(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmcomromelocation(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncmcomromelocationAjax(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);

int ncm_websrvgroup_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //��λ������Ϣ-0613
int ncm_webuserman_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //�û�������Ϣ-0613
int ncm_group_manager_DoorTree_comp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //������-0616

int ncm_webcomindex3_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ���ڶ���-0609
int ncm_webcomindex3_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_webcomindex4_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // Ԥ԰��������-0617
int ncm_webcomindex4_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

char *ncmShopGetOnlineMark(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_webcommark_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ��ͼ��ǩ����-0618

int ncm_portallocmark(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ����

int ncm_webcomindex5_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ̫��������-0620
int ncm_webcomindex5_dr_images(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_web_App_intSsidReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ssidͳ�Ʊ���-0620


int ncm_web_Place_white_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // �ڰ�����-0604

int ncm_webshopgtype_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ����̳������-0623

int ncm_shopgroup(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //�̼�����
int ncm_shopgtype(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //��λ�����
int ncmdeptmenu(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //��˵�Ȩ��
int ncmdeptmenu(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //��˵�Ȩ��
int ncm_srvpar(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ��������
int ncm_srvgroupssid(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // SSID
int ncm_srvgroup(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // �û���
int ncm_autopri(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // �Զ���Ȩ��
int ncm_initmenu(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // �Զ���Ȩ��
int ncm_initsubmenu(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // �Զ���Ȩ��

int ncmDispRoleFunlist_v5(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // Ȩ������-0627
int ncmWebAdmin(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); 

int ncm_webusermac_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // �û�mac

int ncm_web_Place_alarmlog_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // �澯��־-0703
int ncm_web_App_intTrafReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // �û�����ͳ�Ʊ���-0703
int ncmWebShowStaffMan(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // �û�����-0703
char *ncmWebAutoDefGet16(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead,char stype[4]);
//int ncmRolelist_sww(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead,char caRightcheck[2048], char caRolename[64], char caUpdate[32]);
int ncm_webuserman_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_webuserman_dload(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_yuyuan_home_ajax(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_system_reset(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_web_Place_plate_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ģ��-0728
int ncm_web_Place_plist_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//int ncm_web_Place_page_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_web_Place_prule_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncm_webautodef_ajax(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncmGetPlateidByTsid(utShmHead *psShmHead,long long tsid);

int ncm_questions(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);//�ʾ����
int ncmShops_shbook(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);//�ʾ����

int ncm_webblackdr_next(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // �ڰ���������-0812

int ncm_web_Place_maclog_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //������MAC��־-0820
int ncm_web_App_apMacCount_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ĳһʱ��AP��MAC����-0820
int ncm_webmenu_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ϵͳ���˵�����-0821
int ncm_websubmenu_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ϵͳ�Ӳ˵�����-0821

int ncm_web_App_userCount_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // һ����һ�˵����ô���-0827

int ncm_sms_send_chart(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // 20141031���ŷ���ͼ��

int ncm_webcommall_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ¥����Ϣ-20141114
int ncm_webComMallImg_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int query_ap_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); // ��ɾ�Ĳ�ap��Ϣ


int ncm_sms_send_register_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

// ����˹ע�ἰ��֤
int Denis_Register(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncSrvUserGetPass(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);


#endif
