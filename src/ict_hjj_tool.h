/*共用接口头文件*/#ifndef _LINUX_ICT_HJJ_TOOL_H_#define _LINUX_ICT_HJJ_TOOL_H_int checkTableExists(char* tableName);//查询是否有特定表名的表char* getNewLogTable(char* tableOri, char * yearMonth);//得到新的表，没有则创建char* getMobnoByUsername(utShmHead *psShmHead,utMsgHead *psMsgHead,int iFd,char* caUsername);//url编码实现 void urlencode( unsigned char * src, int  src_len, unsigned char * dest, int  dest_len );#endif