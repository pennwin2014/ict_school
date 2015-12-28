/* ������֤ƽ̨���ݽṹ      */
#ifndef  __PROAUTHDEF__
#define  __PROAUTHDEF__

#define PROAUTH_LNK_CLIENT          1      /* ��λ */
#define PROAUTH_LNK_MOBILE          2      /* �ֻ���֤�û� */
#define PROAUTH_LNK_GROUP           3      /* ��λ�� */
#define PROAUTH_LNK_LB            4      /* ��λ��� */
#define PROAUTH_LNK_FAC           5      /* ���볧�� */
#define PROAUTH_LNK_MAC           6      /* MAC��ַ�� */
#define PROAUTH_LNK_SMSCONENT     7      /* �������ݶ��� */
#define NC_LNK_ADMIPLIST    72     /* ����ԱIP��ַ, ������¼��¼ʧ�ܵ�IP��ַ  */

/* �ͻ�����Ϣ����  */
typedef struct authClient_s {
    char           servicecode[16];        // ��������
    unsigned long  userid;              // �û�ID
    char           password[32];        // ����
    char           dispname[64];        // ��ʾ��
    unsigned long  groupid;             // ��ID
    unsigned long  gtype;            // ��λ���
    char jointype[4];                 // ���뷽ʽ
} authClient;
typedef struct authGroup_s {
    unsigned long  groupid;              // ��ID
    char           groupname[64];        // ��ʾ��
    unsigned long  pid;             // ��ID
} authGroup;

typedef struct authFac_s {
    char           code[16];        // ����
    char           dispname[64];    // ����
    char           key[32];         // ���ݼ���KEY
} authFac;
/*�ֻ���֤�û�*/
typedef struct authMobileUser_s {
	  char   mobile[20];               //�ֻ���
	  char   password[20];             //����
	  unsigned long lasttime;          //���ʱ��
	  char   mac[20];                  //mac��ַ  
	  char   servicecode[16];          //��������     
    unsigned long lDel;              //��Ч���  0-��Ч 1-116����״̬ �����ѷ�����֤��Ϣ��û��֤
} authMobileUser;

/*mac�û���*/
typedef struct authMacUser_s {
	  char   mac[20];                  //mac��ַ  
	  char   mobile[20];               //�ֻ���
	  char   dispname[32];             //����
	  char   idno[20];
	  char idtype[8];
	  char servicecode[16];
	  unsigned long lasttime;          //���ʱ��
} authMacUser;

typedef struct authSmsContent_s {
	  char   mobile[20];               //�ֻ���
    char   content[256];             //��������
    char   fws[16];                  //���ŷ�����ͨ��
} authSmsContent;
typedef struct authClientId2Code_s {
	  unsigned long id;               //��λID            //��������
    char   code[32];                  //��������
} authClientId2Code;


#endif
