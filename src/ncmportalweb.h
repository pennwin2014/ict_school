/* ProPotral     ��Ȩ����: �Ϻ���������Ϣ�������޹�˾ 2013
   
*/


//#define MODULEUPPATH "/home/ncmysql/ncm/upload"

// sunww
// �����˶�����������
int ncm_web_Place_smslimit_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);             // �����˶����������Ʊ�
int ncm_web_Place_placelimit_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);           // �����˶���ע���������Ʊ�
int ncm_web_Place_timelimit_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);            // ����������ʱ�����Ʊ�
int ncm_web_Place_smsformat_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);            // �����˶��Ÿ�ʽ��

int ncm_web_MC_intDetailReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char cnfSdate[32], char cnfEdate[32], char caExport1[32], char cnfPath[128]);
int ncm_web_MC_intStatisticsReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char cnfSdate[32], char cnfEdate[32], char caExport1[32], char cnfPath[128]);
int ncm_web_MC_intDeviceReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char cnfSdate[32], char cnfEdate[32], char caExport1[32], char cnfPath[128]);
int ncm_web_MC_smsDetailReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char cnfSdate[32], char cnfEdate[32], char caExport1[32], char cnfPath[128]);
int ncm_web_MC_smsStatisticsReport_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char cnfSdate[32], char cnfEdate[32], char caExport1[32], char cnfPath[128]);


//PORTAL���ͺ��ֻ�Ʒ��ͳ�Ʊ�ͼ
int ncm_DeviceTypeChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncm_DeviceBrandChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncm_GroupSmsChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncm_Ad_DeviceTypeChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncm_Ad_DeviceBrandChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

int ncm_ReportSendLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);	//����ַ���־
int ncmReportReset(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);	//����ַ�����
int ncmReportSend(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); //��������ͳ�Ʊ���

int ncmWriteReportLog(utShmHead  *psShmHead,char *caName,char *caMail,char *caRname,unsigned long groupid,long midid,int flag,char *down_name); //д��������־
int ncmMailAttachSend(char *pFrom,char *pTo,char *pSub,char *pCon,char *pFile,char *pDispname);//�ʼ���������
int ncmGetWeekDay(unsigned long lTime,int lDay,char *sDate); /* ȡ���ڼ���ʱ��*/


int ncm_ShowSmsStatisticsReport(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead,char sdate[32],char edate[32],char makepath[255]);

int ncm_MailSendSave_par(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);//�ʼ��ַ���������
int ncm_MailSendSet_par(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);

//ʡ����Ϣ
int ncm_areaInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncm_SMSRecountChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);//ʡ�ж��Ź��ڹ���ͳ��
int ncm_Ad_SMSRecountChart(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);//ʡ�ж��Ź��ڹ���ͳ��
int ncm_Ad_OnlineUserNumber(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);//ʡ�ж��Ź��ڹ���ͳ��
int ncm_Ad_AdVisit(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);//���������ͳ��



