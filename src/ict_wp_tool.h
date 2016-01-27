/*共用接口头文件*/#ifndef _LINUX_ICT_WP_TOOL_H_#define _LINUX_ICT_WP_TOOL_H_typedef struct {    uint4   payment_type;       /*  支付类型                   */    char   partner[32];      /*  合作伙伴     */	char   input_charset[32]; /* 字符集  */	char   seller_email[64];  /* 卖家email */	char   md5_key[64];            /* 商户的md5 key */ 	char   rsa_key[1024];            /* 商户的rsa key */ 	char   notify_url[256];  /* 异步通知地址 */	char   return_url[256];  /* 同步跳转URL */	char   out_trade_no[32]; /* 订单号 */	char   subject[256];        double   total_fee;      /*  金额                              */	char   body[256];      /*描述*/	char   show_url[64];   /*显示的url*/	char sign_type[32];    /* 签名类型 */	uint8   anti_phishing_key;	char   exter_invoke_ip[32];} ictAlipayConfig;char *getXmlConByPost(char *pUrl, char *pPostVar);int removeCData(char* originStr, char* newStr, ulong maxLenth);int getNodeValue(char* outXml, char* startStr, char* endStr, char* outStr, ulong maxLength);uint8 getTsidByIp(utShmHead *psShmHead, char* ictIp);int getOnlineUser(utShmHead *psShmHead);#endif