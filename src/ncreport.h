
struct ncSflow_s {
        unsigned long lService;
        unsigned long lGroupid;
        unsigned long long lBytes;
        unsigned long long lBytesqs[32];      //��������ͼ����ͳ��ʱ��˳����
        char caGname[64];
}; 

struct piechart_datasetf{
	char *name;                               //��ͼ��Ŀ����
	char **append;                            //pdf����и����ֶΣ��ӵ����п�ʼ
	//unsigned short  len;
	float amount;                             //��Ŀ����
	char *percent;                            //�ٷ���
};
struct chart_datasetf {
	char *name;                               //ʱ��
	float *dataset;                           //ʱ�������
};
/*
   ����                            �����ͼ
   int iFd                         PAS����� iFd, �������������utPltFileDownload���غ���
   struct piechart_datasetf *chart ����
   char **desc                     ��ͷ ��{"�û���", "����", "����(KB)", "�ٷֱ�"}
   int rowrow                      chart ����
   int column                      desc  ����
   int applen                      �����ֶθ���
   char *title                     ����
   char *unit                      ��λ��ע �� "��λ(KB)"
   char *filename_new              PDF�ļ���
   char *report_text               ͳ��ʱ���ʶ

 */
//int ncWebShowServiceClassFirst_PDF(int iFd, struct piechart_datasetf *chart, char **desc, int rowrow, int column, int applen, char *title, char *unit, char *report_text, char *filename_new);  


 /*
   ����                            ����ͼ
   int iFd                         PAS����� iFd, �������������utPltFileDownload���غ���
   struct piechart_datasetf *chart ����
   char **desc                     ��ͷ
   int rowrow                      chart ����
   int column                      desc  ����
   char *title                     ����
   char *unit                      ��λ��ע �� "��λ(KB)"
   char *filename_new              PDF�ļ���
*/
//int ncWebShowServiceBytime_PDF(int iFd, struct chart_datasetf *chart, char **desc, int rowrow, int column, char *title, char *unit, char *report_text, char *filename_new);


int ncmWebShowSMSsendType_PDF(int iFd, struct piechart_datasetf *chart, char **desc, int rowrow, int column, int applen, char *title, char *unit, char *report_text, char *filename_new);
//int ncwlanWebShowServiceBytime_PDF(int iFd, struct chart_datasetf *chart, char **desc, int rowrow, int column, char *title, char *unit, char *report_text, char *filename_new);


struct wirepiechart_dataset{
                                char *name;
                                long  amount;
                                char *time;
                                char *mac;
                                char *ssid;
                                char *Bssid;
                                int channel;
                                char *freq;
                                char *terminal;
                                int  busitype;
                                double percent;
                                long long bytes;
                                long terminum;
                        };

struct wsin_u{
                                char lStartTime[30];
                                char Dmac[28];
                                char ssid[32];     //��Bssid�Ƿ���
                                char Bssid[20];
                                int channel;
                                char freq[15];
                                char Smac[28];
                                int iService;
                                int signal;
                                long lcou;
                                long terminum;
                                unsigned long long llBytes[2];
                 };
