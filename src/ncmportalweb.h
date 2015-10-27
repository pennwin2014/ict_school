/* ProPotral     版权所有: 上海新网程信息技术有限公司 2013
   
*/


//#define MODULEUPPATH "/home/ncmysql/ncm/upload"

// sunww
// 场所端短信数量限制
int ncm_web_Place_smslimit_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);             // 场所端短信数量限制表
int ncm_web_Place_placelimit_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);           // 场所端短信注册数量限制表
int ncm_web_Place_timelimit_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);            // 场所端上网时间限制表
int ncm_web_Place_smsformat_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);            // 场所端短信格式表

int ncm_web_MC_intDetailReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char cnfSdate[32], char cnfEdate[32], char caExport1[32], char cnfPath[128]);
int ncm_web_MC_intStatisticsReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char cnfSdate[32], char cnfEdate[32], char caExport1[32], char cnfPath[128]);
int ncm_web_MC_intDeviceReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char cnfSdate[32], char cnfEdate[32], char caExport1[32], char cnfPath[128]);
int ncm_web_MC_smsDetailReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char cnfSdate[32], char cnfEdate[32], char caExport1[32], char cnfPath[128]);
int ncm_web_MC_smsStatisticsReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char cnfSdate[32], char cnfEdate[32], char caExport1[32], char cnfPath[128]);


//PORTAL机型和手机品牌统计饼图
int ncm_DeviceTypeChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncm_DeviceBrandChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncm_GroupSmsChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncm_Ad_DeviceTypeChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncm_Ad_DeviceBrandChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

int ncm_ReportSendLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);	//报表分发日志
int ncmReportReset(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);	//报表分发设置
int ncmReportSend(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //发送生成统计报表

int ncmWriteReportLog(utShmHead  *psShmHead,char *caName,char *caMail,char *caRname,unsigned long groupid,long midid,int flag,char *down_name); //写报表发送日志
int ncmMailAttachSend(char *pFrom,char *pTo,char *pSub,char *pCon,char *pFile,char *pDispname);//邮件附件发送
int ncmGetWeekDay(unsigned long lTime,int lDay,char *sDate); /* 取星期几的时间*/


int ncm_ShowSmsStatisticsReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead,char sdate[32],char edate[32],char makepath[255]);

int ncm_MailSendSave_par(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);//邮件分发参数设置
int ncm_MailSendSet_par(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

//省市信息
int ncm_areaInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_SMSRecountChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);//省市短信国内国际统计
int ncm_Ad_SMSRecountChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);//省市短信国内国际统计
int ncm_Ad_OnlineUserNumber(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);//省市短信国内国际统计
int ncm_Ad_AdVisit(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);//广告点击次数统计



