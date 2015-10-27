drop table if exists ncsmaclog_if_201412;
create table ncsmaclog_if_201412 (
    servicecode char(18)      default ' ',                 -- �����˱���
    servicename char(64)      default ' ',                 -- ����������
    address     char(64)      default ' ',                 -- ��װλ��
    fromdev     char(32)      default ' ',                 -- ������Դ
    mac         char(18)      default ' ',                 -- Mac��ַ
    apname      char(32),                                  -- AP����
    apmac       char(18),                                  -- APMac��ַ
    lrssi       int  default 0,                            -- ��ʼ��ǿ
    urssi       int  default 0,                            -- �뿪��ǿ
    ssid        char(64)      default ' ',                 -- SSID,������ö��ŷָ�    
    plate       char(32)      default ' ',                 -- �������
    line        char(32)      default ' ',                 -- ��ͨ��·����
    mapid       char(32)      default ' ',                 -- ��ͼ��ʶ(����ֵ���ڣ�����ά��Ϊ�룬x,y����
    longitude   char(16)      default ' ',                 -- ����
    latitude    char(16)      default ' ',                 -- ά��
    sendtime         int unsigned default 0,
    starttime        int unsigned default 0,               -- ����ʱ��
    endtime          int unsigned default 0,               -- �뿪ʱ��
    flags             int default 0                         -- 0--����  1--�뿪
);


create index ncsmaclog_ind1 on ncsmaclog_if_201412(servicecode);
create index ncsmaclog_ind2 on ncsmaclog_if_201412(servicecode,mac);
create index ncsmaclog_ind3 on ncsmaclog_if_201412(mac);

