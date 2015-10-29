#define  PAS_SRCFILE     8101
#define ncmax(a,b) ((a) > (b) ? (a) : (b))
#define ncMin(a,b) ((a) < (b) ? (a) : (b))
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <dirent.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/resource.h>
#include "utoall.h"
#include "pasdb.h"
#include "utoplt01.h"
#include "ncportal.h"
#include "ncportalweb.h"


// #define PAS_LOG_EVENT1 "aaa"
// #define PAS_LOG_ERROR "bbb"
#include "ncmportalweb.h"

int ncmAdSecord(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncm_adsecord_dr_images(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncmSecordDispAd(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncmTreeCorp_comp_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncmShoppingGuiding(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncmInitWebFun_Cxyh(utShmHead *psShmHead);
int ictInitWebFun_hjj(utShmHead *psShmHead);
int ictInitWebFun_gpx(utShmHead *psShmHead);


int ncmBrandManageList(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncmBrandAddOrModify(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncmResponHtmlPage(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncm_dnsswtssort_list(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);


//Portal��������
int ncmInitWebFun(utShmHead *psShmHead)
{
    int iReturn;
    //ģ�����


    iReturn = pasSetTcpFunName("ncmAddTest", ncmAddTest, 0);
    iReturn = pasSetTcpFunName("ncmModule_upload", ncmModule_upload, 0);
    iReturn = pasSetTcpFunName("ncmModuleCreateDef", ncmModuleCreateDef, 0);
    iReturn = pasSetTcpFunName("ncmTreeFac", ncmTreeFac, 0);
    iReturn = pasSetTcpFunName("ncmWebModuleSet", ncmWebModuleSet, 0);
    iReturn = pasSetTcpFunName("ncmWebModuleManSelect", ncmWebModuleManSelect, 0);
    iReturn = pasSetTcpFunName("ncmModuleApply", ncmModuleApply, 0);

    iReturn = pasSetTcpFunName("ncmWebModuleDefine", ncmWebModuleDefine, 0);
    iReturn = pasSetTcpFunName("ncmWebModuleDefineSelect", ncmWebModuleDefineSelect, 0);

    iReturn = pasSetTcpFunName("ncm_WebShowModuleApp", ncm_WebShowModuleApp, 0);  //s

    iReturn = pasSetTcpFunName("ncmModuleCreate", ncmModuleCreate, 0);
    iReturn = pasSetTcpFunName("ncmWebReplaceOne", ncmWebReplaceOne, 0);
    iReturn = pasSetTcpFunName("ncmWebShowModuleAddModify", ncmWebShowModuleAddModify, 0);
    iReturn = pasSetTcpFunName("ncmModuleDefInfoSave", ncmModuleDefInfoSave, 0);
    //ϵͳ����
    iReturn = pasSetTcpFunName("ncmSysUserlist_v4", ncmSysUserlist_v4, 0);
    iReturn = pasSetTcpFunName("ncmDispRoleFunlist_v4", ncmDispRoleFunlist_v4, 0);
    iReturn = pasSetTcpFunName("ncmRolelist_v4", ncmRolelist_v4, 0);
    iReturn = pasSetTcpFunName("ncmWebAdminDispUserForm", ncmWebAdminDispUserForm, 0);
    iReturn = pasSetTcpFunName("ncmAdminUserSave", ncmAdminUserSave, 0);
    iReturn = pasSetTcpFunName("ncmWebAdmQyeSysLog", ncmWebAdmQyeSysLog, 0);
    iReturn = pasSetTcpFunName("ncmWebGroupComp", ncmWebGroupComp, 0);
    // iReturn = pasSetTcpFunName("ncmSysReflesh_v4",ncmSysReflesh_v4, 0);
    iReturn = pasSetTcpFunName("ncmFirstMenuConfig", ncmFirstMenuConfig, 0);
    iReturn = pasSetTcpFunName("ncmjump_gid", ncmjump_gid, 0);
    iReturn = pasSetTcpFunName("ncmWebDispModiPass", ncmWebDispModiPass, 0);
    iReturn = pasSetTcpFunName("ncmWebModiPass", ncmWebModiPass, 0);
    //iReturn = pasSetTcpFunName("ncmWriteSysLog",ncmWriteSysLog, 0);
    iReturn = pasSetTcpFunName("ncmSysUserRoleConfig", ncmSysUserRoleConfig, 0);
    iReturn = pasSetTcpFunName("ncmWebAuth", ncmWebAuth, 0);


    // iReturn = pasSetTcpFunName("ncmTreeGroup_comp_v4",ncmTreeGroup_comp_v4, 0);

    //ϵͳ����
    iReturn = pasSetTcpFunName("ncmWebSystemSet_base", ncmWebSystemSet_base, 0);
    iReturn = pasSetTcpFunName("ncmWebSystemSet_dev", ncmWebSystemSet_dev, 0);
    iReturn = pasSetTcpFunName("ncmWebEtheSelect", ncmWebEtheSelect, 0);
    iReturn = pasSetTcpFunName("ncmSysStatus_Alarmset", ncmSysStatus_Alarmset, 0);
    iReturn = pasSetTcpFunName("ncmDiskRateSave_par", ncmDiskRateSave_par, 0);
    iReturn = pasSetTcpFunName("ncmWebSystemSave_base", ncmWebSystemSave_base, 0);

    iReturn = pasSetTcpFunName("ncmPortal_ParaSet", ncmPortal_ParaSet, 0);
    iReturn = pasSetTcpFunName("ncmPortalParaSave_par", ncmPortalParaSave_par, 0);
    iReturn = pasSetTcpFunName("ncmSms_Server_ParaSet", ncmSms_Server_ParaSet, 0);
    iReturn = pasSetTcpFunName("ncmSmsParaSave_par", ncmSmsParaSave_par, 0);
    iReturn = pasSetTcpFunName("ncmHttp_Server_ParaSet", ncmHttp_Server_ParaSet, 0);
    iReturn = pasSetTcpFunName("ncmHttpParaSave_par", ncmHttpParaSave_par, 0);
    iReturn = pasSetTcpFunName("ncmAC_Server_ParaSet", ncmAC_Server_ParaSet, 0);
    iReturn = pasSetTcpFunName("ncmACParaSave_par", ncmACParaSave_par, 0);
    iReturn = pasSetTcpFunName("ncmAC_Server_SelfParaSet", ncmAC_Server_SelfParaSet, 0);
    iReturn = pasSetTcpFunName("ncmRadius_Server_ParaSet", ncmRadius_Server_ParaSet, 0);
    iReturn = pasSetTcpFunName("ncmRadiusParaSave_par", ncmRadiusParaSave_par, 0);

    iReturn = pasSetTcpFunName("ncmWebShowUserMan", ncmWebShowUserMan, 0);
    iReturn = pasSetTcpFunName("ncmWebGroupSelectComp", ncmWebGroupSelectComp, 0);

    //������Ϣ
    iReturn = pasSetTcpFunName("ncmWebShowOnlineUserInfo", ncmWebShowOnlineUserInfo, 0);


    //�����
    iReturn = pasSetTcpFunName("ncmWeb_groupList", ncmWeb_groupList, 0);

    //��־
    iReturn = pasSetTcpFunName("ncm_webshowSMSlogList", ncm_webshowSMSlogList, 0);
    iReturn = pasSetTcpFunName("ncm_userlog", ncm_userlog, 0);

    //�̳� w
    iReturn = pasSetTcpFunName("ncmLevels", ncmLevels, 0);
    iReturn = pasSetTcpFunName("ncmDiscounts", ncmDiscounts, 0);
    iReturn = pasSetTcpFunName("ncmDiscounts_Details", ncmDiscounts_Details, 0);
    //iReturn = pasSetTcpFunName("ncmDiscounts_Coupons",ncmDiscounts_Coupons,0);

    iReturn = pasSetTcpFunName("ncmShops_Discounts", ncmShops_Discounts, 0);
    iReturn = pasSetTcpFunName("ncmCategories", ncmCategories, 0);
    iReturn = pasSetTcpFunName("ncmCategories_Types", ncmCategories_Types, 0);

    iReturn = pasSetTcpFunName("ncmPersonals", ncmPersonals, 0);
    iReturn = pasSetTcpFunName("ncmPersonals_Info", ncmPersonals_Info, 0);
    iReturn = pasSetTcpFunName("ncmPersonals_Mark", ncmPersonals_Mark, 0);
    iReturn = pasSetTcpFunName("ncmPersonals_Consump", ncmPersonals_Consump, 0);
    //iReturn = pasSetTcpFunName("ncmPersonals_Coupon",ncmPersonals_Coupon,0);
    //iReturn = pasSetTcpFunName("ncmPersonals_Attention",ncmPersonals_Attention,0);
    //iReturn = pasSetTcpFunName("ncmPersonals_Collection",ncmPersonals_Collection,0);

    iReturn = pasSetTcpFunName("ncmLogos_detail", ncmLogos_detail, 0);
    iReturn = pasSetTcpFunName("ncmLogos", ncmLogos, 0);
    iReturn = pasSetTcpFunName("ncmProducts", ncmProducts, 0);
    iReturn = pasSetTcpFunName("ncmLevels_Logos", ncmLevels_Logos, 0);
    iReturn = pasSetTcpFunName("ncmLevels_Logos_Recommend", ncmLevels_Logos_Recommend, 0);
    iReturn = pasSetTcpFunName("ncmShops", ncmShops, 0);
    iReturn = pasSetTcpFunName("ncm_Addattentions", ncm_Addattentions, 0);
    iReturn = pasSetTcpFunName("ncm_Addcollections", ncm_Addcollections, 0);
    iReturn = pasSetTcpFunName("ncm_Addcoupons", ncm_Addcoupons, 0);

    //s
    // ¥����Ϣ
    iReturn = pasSetTcpFunName("ncm_webreadfloor_list", ncm_webreadfloor_list, 0);
    // ��Ʒ����
    iReturn = pasSetTcpFunName("ncm_webcomgroup_list", ncm_webcomgroup_list, 0);
    iReturn = pasSetTcpFunName("ncm_webcomgroup_info", ncm_webcomgroup_info, 0);
    iReturn = pasSetTcpFunName("ncm_webComGroupImg_dr", ncm_webComGroupImg_dr, 0);
    // ��ƷС��
    iReturn = pasSetTcpFunName("ncm_webcomtype_list", ncm_webcomtype_list, 0);
    iReturn = pasSetTcpFunName("ncm_webcomtype_info", ncm_webcomtype_info, 0);
    iReturn = pasSetTcpFunName("ncm_webComTypeImg_dr", ncm_webComTypeImg_dr, 0);
    // �̱����
    iReturn = pasSetTcpFunName("ncm_webcomindex_list", ncm_webcomindex_list, 0);
    iReturn = pasSetTcpFunName("ncm_webcomindex_info", ncm_webcomindex_info, 0);
    iReturn = pasSetTcpFunName("ncm_webcomindex_dr_images", ncm_webcomindex_dr_images, 0);
    iReturn = pasSetTcpFunName("ncm_webcomindex_update_list", ncm_webcomindex_update_list, 0);
    // ��Ʒ
    iReturn = pasSetTcpFunName("ncm_webcomodity_list", ncm_webcomodity_list, 0);
    iReturn = pasSetTcpFunName("ncm_webComOdityImg_dr", ncm_webComOdityImg_dr, 0);
    // �̵�
    iReturn = pasSetTcpFunName("ncm_webcomshop_list", ncm_webcomshop_list, 0);
    iReturn = pasSetTcpFunName("ncm_webComShopImg_dr", ncm_webComShopImg_dr, 0);
    // �̳��û���Ϣ
    iReturn = pasSetTcpFunName("ncm_webcomshopuser_list", ncm_webcomshopuser_list, 0);
    // �û�������Ϣ
    iReturn = pasSetTcpFunName("ncm_webcomconsump_list", ncm_webcomconsump_list, 0);
    // �Ż݄�
    iReturn = pasSetTcpFunName("ncm_webcomcoupon_list", ncm_webcomcoupon_list, 0);
    iReturn = pasSetTcpFunName("ncm_webComCouponImg_dr", ncm_webComCouponImg_dr, 0);
    // �̳��û���ע��Ϣ
    iReturn = pasSetTcpFunName("ncm_webcomshopattent_list", ncm_webcomshopattent_list, 0);
    // �̳��û��ղ���Ϣ
    iReturn = pasSetTcpFunName("ncm_webcomshopcollect_list", ncm_webcomshopcollect_list, 0);
    // �̳ǻ��
    iReturn = pasSetTcpFunName("ncm_webcomactive_list", ncm_webcomactive_list, 0);
    iReturn = pasSetTcpFunName("ncm_webcomactive_info", ncm_webcomactive_info, 0);


    // �����̳ǻ��Ϣ
    iReturn = pasSetTcpFunName("ncm_webcomactive_dr_images", ncm_webcomactive_dr_images, 0);
    iReturn = pasSetTcpFunName("ncm_webcomactive_update_list", ncm_webcomactive_update_list, 0);

    // �����Ż݄���Ϣ
    iReturn = pasSetTcpFunName("ncm_webcomcoupon_info", ncm_webcomcoupon_info, 0);
    iReturn = pasSetTcpFunName("ncm_webcomshop_info", ncm_webcomshop_info, 0);
    iReturn = pasSetTcpFunName("ncm_webcomodity_info", ncm_webcomodity_info, 0);

    // �����Ż݄�ʹ�ñ���Ϣ
    iReturn = pasSetTcpFunName("ncm_webcomcouponuse_list", ncm_webcomcouponuse_list, 0);

    // Ʒ����Ϣ
    iReturn = pasSetTcpFunName("ncm_webcombrand_list", ncm_webcombrand_list, 0);
    iReturn = pasSetTcpFunName("ncm_webcombrand_info", ncm_webcombrand_info, 0);
    iReturn = pasSetTcpFunName("ncm_webComBrandImg_dr", ncm_webComBrandImg_dr, 0);




    // �û���ϸ��������
    iReturn = pasSetTcpFunName("ncm_web_App_intDetailReport", ncm_web_App_intDetailReport, 0);
    // ����������������������
    iReturn = pasSetTcpFunName("ncm_web_App_intStatisticsReport", ncm_web_App_intStatisticsReport, 0);
    // �豸���ͱ���
    iReturn = pasSetTcpFunName("ncm_web_App_intDeviceReport", ncm_web_App_intDeviceReport, 0);
    // ���ŷ��ͱ���
    iReturn = pasSetTcpFunName("ncm_web_App_smsDetailReport", ncm_web_App_smsDetailReport, 0);

    iReturn = pasSetTcpFunName("ncm_web_App_smsStatisticsReport", ncm_web_App_smsStatisticsReport, 0);

    //�û�����s
    iReturn = pasSetTcpFunName("ncm_blackuser", ncm_blackuser, 0);

    iReturn = pasSetTcpFunName("ncmShopUserOnlineFlag", ncmShopUserOnlineFlag, 0);
    iReturn = pasSetTcpFunName("ncmWebReplace", ncmWebReplace, 0);




    //    Ncm_PortalShopWB_SetFunName();      //sww
    Ncm_MaintainCenterSWW_SetFunName();
    Ncm_MaintainCenterWB_SetFunName();
    Ncm_WebShopSWW_SetFunName();

    Ncm_WebPlaceSWW_SetFunName();

    Ncm_PortalClient_SetFunName();

    Ncm_User_SetFunName();
    //��ȡ����˶��Ź���
    pasUtlSetProcessNameD(psShmHead, "SynSmsRule",  "Yes", ncmcomSynPortalManInfo, 3600, 0);
    //������ͬ���ͻ�����Ϣ
    pasUtlSetProcessNameD(psShmHead, "ClientInfo",  "Yes", ncmcomSynPortalOnlineInfo, 3600, 0);
    //�����û���־
    pasUtlSetProcessNameD(psShmHead, "SendUsLog",  "Yes", ncmcomUploadSmsUserlog, 3600, 0);
    //���Ͷ�����־
    pasUtlSetProcessNameD(psShmHead, "SendSmsLog",   "Yes", ncmcomUploadSmslog, 3600, 0);
    //�����û���Ϣ
    pasUtlSetProcessNameD(psShmHead, "SendUsInfo", "Yes", ncmcomUploadClientUserInfo, 3600, 0);



    //iReturn = pasSetTcpFunName("ncm_ShowSmsStatisticsReport",ncm_ShowSmsStatisticsReport,0);

    //sww  ͳ�Ʊ����û�����
    iReturn = pasSetTcpFunName("ncm_web_Place_smslog_list", ncm_web_Place_smslog_list, 0);
    iReturn = pasSetTcpFunName("ncm_web_Place_userlog_list", ncm_web_Place_userlog_list, 0);
    iReturn = pasSetTcpFunName("ncm_web_Place_smslog_count", ncm_web_Place_smslog_count, 0);
    iReturn = pasSetTcpFunName("ncm_web_Place_smslog_rule", ncm_web_Place_smslog_rule, 0);
    iReturn = pasSetTcpFunName("ncm_web_Place_smslog_custmer", ncm_web_Place_smslog_custmer, 0);
    iReturn = pasSetTcpFunName("ncm_website_visit_type", ncm_website_visit_type, 0);


    Ncm_PortalWeiXin_SetFunName();
    Ncm_PortalQQ_SetFunName();
    iReturn = pasSetTcpFunName("ncm_portplate", ncm_portplate, 0);
    iReturn = pasSetTcpFunName("ncm_yuyuan_home", ncm_yuyuan_home, 0);
    iReturn = pasSetTcpFunName("ncm_ShopType", ncm_ShopType, 0);

    iReturn = pasSetTcpFunName("ncm_webweixin_list", ncm_webweixin_list, 0);

    //20140516
    iReturn = pasSetTcpFunName("ncm_smspay", ncm_smspay, 0);
    iReturn = pasSetTcpFunName("ncm_groupInfo", ncm_groupInfo, 0);
    //20140521
    iReturn = pasSetTcpFunName("ncmShopUserOnlineInsert", ncmShopUserOnlineInsert, 0);

    iReturn = pasSetTcpFunName("ncm_webuserman_info", ncm_webuserman_info, 0); // �û�������Ϣ-013
    iReturn = pasSetTcpFunName("ncm_group_manager_DoorTree_comp", ncm_group_manager_DoorTree_comp, 0); // ������-0616
    //iReturn = pasSetTcpFunName("ncmShopGetOnlineMark", ncmShopGetOnlineMark, 0);

    //����
    iReturn = pasSetTcpFunName("ncm_portallocmark", ncm_portallocmark, 0);
    iReturn = pasSetTcpFunName("ncm_shopgroup", ncm_shopgroup, 0);//�̼�����
    iReturn = pasSetTcpFunName("ncm_shopgtype", ncm_shopgtype, 0);//��λ�����
    iReturn = pasSetTcpFunName("ncmdeptmenu", ncmdeptmenu, 0);//��˵�Ȩ��
    iReturn = pasSetTcpFunName("ncm_srvpar", ncm_srvpar, 0);//��������
    iReturn = pasSetTcpFunName("ncm_srvgroupssid", ncm_srvgroupssid, 0);//SSID����
    iReturn = pasSetTcpFunName("ncm_srvgroup", ncm_srvgroup, 0);//�û���
    iReturn = pasSetTcpFunName("ncm_autopri", ncm_autopri, 0);//�û���
    iReturn = pasSetTcpFunName("ncm_initmenu", ncm_initmenu, 0);//���˵�
    iReturn = pasSetTcpFunName("ncm_initsubmenu", ncm_initsubmenu, 0);//�Ӳ˵�

    iReturn = pasSetTcpFunName("ncm_web_App_intSsidReport_list", ncm_web_App_intSsidReport_list, 0);  // ssidͳ�Ʊ���-0620

    iReturn = pasSetTcpFunName("ncmDispRoleFunlist_v5", ncmDispRoleFunlist_v5, 0); // Ȩ������-0627
    iReturn = pasSetTcpFunName("ncmWebAdmin", ncmWebAdmin, 0);

    iReturn = pasSetTcpFunName("ncm_webusermac_list", ncm_webusermac_list, 0);  // �û�mac

    iReturn = pasSetTcpFunName("ncm_web_Place_alarmlog_list", ncm_web_Place_alarmlog_list, 0); // �澯��־-0703
    iReturn = pasSetTcpFunName("ncm_web_App_intTrafReport_list", ncm_web_App_intTrafReport_list, 0);  // �û�����ͳ�Ʊ���-0703
    iReturn = pasSetTcpFunName("ncmWebShowStaffMan", ncmWebShowStaffMan, 0); // �û�����-0703

    //iReturn = pasSetTcpFunName("ncmWebAutoDefGet16", ncmWebAutoDefGet16, 0);
    //iReturn = pasSetTcpFunName("ncmRolelist_sww", ncmRolelist_sww, 0);
    iReturn = pasSetTcpFunName("ncm_webuserman_dr", ncm_webuserman_dr, 0);
    iReturn = pasSetTcpFunName("ncm_webuserman_dload", ncm_webuserman_dload, 0);
    iReturn = pasSetTcpFunName("ncm_yuyuan_home_ajax", ncm_yuyuan_home_ajax, 0);
    iReturn = pasSetTcpFunName("ncm_system_reset", ncm_system_reset, 0);

    iReturn = pasSetTcpFunName("ncm_webautodef_ajax", ncm_webautodef_ajax, 0);

    iReturn = pasSetTcpFunName("ncm_questions", ncm_questions, 0); //�ʾ����

    iReturn = pasSetTcpFunName("ncm_webblackdr_next", ncm_webblackdr_next, 0); // �ڰ���������

    iReturn = pasSetTcpFunName("ncm_web_Place_maclog_list", ncm_web_Place_maclog_list, 0);  //������MAC��־-0820
    iReturn = pasSetTcpFunName("ncm_web_App_apMacCount_list", ncm_web_App_apMacCount_list, 0); // ĳһʱ��AP��MAC����-0820
    iReturn = pasSetTcpFunName("ncm_webmenu_list", ncm_webmenu_list, 0); // ϵͳ���˵�����-0821
    iReturn = pasSetTcpFunName("ncm_websubmenu_list", ncm_websubmenu_list, 0); // ϵͳ�Ӳ˵�����-0821

    iReturn = pasSetTcpFunName("ncm_web_App_userCount_list", ncm_web_App_userCount_list, 0); // һ����һ�˵����ô���-0827

    Ncm_Imp_User_pri_dep_FunName();
    //iReturn = pasSetTcpFunName("ncmGetPlateidByTsid", ncmGetPlateidByTsid, 0);
    iReturn = pasSetTcpFunName("ncmShops_shbook", ncmShops_shbook, 0);
    Ncm_Mem_Api_SetFunName();
    Ncm_OnlineInfo_SetFunName();

    Ncm_YuyuanSWW_SetFunName();
    ncmInitWebFun_Cxyh(psShmHead);
    ictInitWebFun_hjj(psShmHead);
    ictInitWebFun_gpx(psShmHead);
    //�й���۹㳡�Ĺ��ܺ���
    iReturn = pasSetTcpFunName("ncm_limitband", ncm_limitband, 0);           //����ѡ��

    iReturn = pasSetTcpFunName("ncmAdSecord", ncmAdSecord, 0);
    iReturn = pasSetTcpFunName("ncm_adsecord_dr_images", ncm_adsecord_dr_images, 0);
    iReturn = pasSetTcpFunName("ncmSecordDispAd", ncmSecordDispAd, 0);

    // ap��mac�Լ�λ����Ϣ����ɾ�Ĳ麯��
    iReturn = pasSetTcpFunName("query_ap_info", query_ap_info, 0);

    iReturn = pasSetTcpFunName("ncm_sms_send_register_list", ncm_sms_send_register_list, 0);

    iReturn = pasSetTcpFunName("ncSrvUserGetPass", ncSrvUserGetPass, 0);

    iReturn = pasSetTcpFunName("Denis_Register", Denis_Register, 0);

    iReturn = pasSetTcpFunName("ncmTreeCorp_comp_v4", ncmTreeCorp_comp_v4, 0);
    iReturn = pasSetTcpFunName("ncmShoppingGuiding", ncmShoppingGuiding, 0);
    iReturn = pasSetTcpFunName("ncmBrandManageList", ncmBrandManageList, 0);
    iReturn = pasSetTcpFunName("ncmBrandAddOrModify", ncmBrandAddOrModify, 0);

    iReturn = pasSetTcpFunName("ncmResponHtmlPage", ncmResponHtmlPage, 0);
    iReturn = pasSetTcpFunName("ncm_dnsswtssort_list", ncm_dnsswtssort_list, 0);

    return 0;
}
