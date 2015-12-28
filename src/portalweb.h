#define PROAUTH_LNK_CLIENT          99      /* ��λ */
#define PROAUTH_LNK_GROUP           98     /* ��λ�� */
#define PROAUTH_LNK_LB              97      /* ��λ��� */
#define PROAUTH_LNK_CLIENTIDCODE  96      /* ��λid�ͳ���code���ձ�*/

/* �ͻ�����Ϣ����  */
typedef struct authClient_s {
    char           servicecode[32];     // ��������
    unsigned long  userid;              // �û�ID
    char           dispname[128];        // ��ʾ��
    unsigned long  groupid;             // ��ID
    unsigned long  gtype;            // ��λ���
    double longitude;
    double latitude;
} authClient;
typedef struct authGroup_s {
    unsigned long  groupid;              // ��ID
    char           groupname[64];        // ��ʾ��
    unsigned long  pid;             // ��ID
} authGroup;

typedef struct authFac_s {
    char           code[32];        // ����
    char           dispname[64];    // ����
    char           key[32];         // ���ݼ���KEY
} authFac;
typedef struct authClientId2Code_s {
	  unsigned long id;               //��λID            //��������
    char   code[32];                  //��������
} authClientId2Code;
