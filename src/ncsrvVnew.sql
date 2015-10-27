-- 2014/06/26 portalprule
--    alter table portalprule add groupcode char(32) default ' '
-- 2014/06/15 ncsrvpar ������ macauth
-- 2014/06/04 ncsgroup������ pid
-- 2014/05/30 ���Ӻڰ�����  ncsrvwhite
-- 2014/04/25 �޶�����ĳЩ�ṹ���е���
-- 2014/04/22 �����ݽṹ�����޸�
-- ncsrvuserlog
-- userid           
-- ncsrvsmslog: sptype             char(32),  
-- 2013/11/17   Table ncsrvuser   usermac �ֶ����ӵ�96���ֽ� 
--             alter table ncsrvuser modify usermac  char(96);
-- create database ncsrv;
-- grant all privileges on ncsrv.*  to ncsrv@localhost identified by 'ncsrv';

--   �û���
--   groupname+ssid ��Ӧ��Radius�е�group
--
drop table if exists ncsrvgroup;
create table if not exists ncsrvgroup (
    groupid            int  unsigned   primary key,    -- ��ֵ�ɷ����������� md5code(groupname)
//    pid                int  unsigned default 0,        -- �ϼ���λ
    groupname          char(32),                       -- ��λ���� 00 00 00 00 00 00 16λ
    dispname           char(128) default ' ',          -- ��ʾ��
    address            char(255) default ' ',          -- ��ַ
    smsname            char(32)  default ' ',          -- �Ʒѵ�λ
    mark               char(128) default ' ',          -- ��ǩ
    memo               text,                           -- ��λ����
    longitude          double default 0,               -- λ�ã����ȣ�
    latitude           double default 0,               -- λ��(γ��)
    addtime            int unsigned default 0,         -- ����ʱ��
    moditime           int unsigned default 0,         -- ����޸�ʱ��
    lasttime           int unsigned default 0,         -- ���һ�β���ʱ��
    flags              int default 1                   -- �Ƿ���Ч 0--��Ч  1--��Ч  9--ɾ��
);
create unique index ncsrvgroup_i1 on ncsrvgroup (groupname);


-- ��λ�ͽ����豸���ձ�
drop table if exist ncsrvgroupap;
create table if not exists ncsrvgroupap (
    gwid               char(32)   primary key,         -- �����豸ID, ��APID ��MAC
    groupid            int unsigned default 0,
    longitude          double default 0,               -- λ�ã����ȣ�
    latitude           double default 0,               -- λ��(γ��)
    addtime            int unsigned default 0,         -- ����ʱ��
    flags              int                             -- 1 ����ʹ��   0--δ��ͨ
) max_rows=1000000000 avg_row_length=256;

create index ncsrvgroupap_i1 on ncsrvgroupap (groupid);


-- ���ʺڰ�����
drop table if exist ncsrvwhite;
create table if not exists ncsrvwhite (
    pid                 int unsigned,                  -- ��ptype������ID,��:Groupid,
    ptype               int unsigned default 0,        -- ��� 0-ȫ��  1-Group   2-SSID 
    wtype               int default 0,                 -- �ڰ�����     0--������   1--������
    nametype            int default 0,                 -- �������     1--����IP   2--����URL    3--�ն�MAC
    name                char(128)                      -- ����
) max_rows=1000000000 avg_row_length=256;

create index ncsrvwhite_i1 on ncsrvwhite (ptype);


-- ���� ncsrvpar 
drop table if exists ncsrvpar;
create table if not exists ncsrvpar (
    pid                 int unsigned,
    ptype               int unsigned default 0,        -- ��� 0-ȫ��  1-Group   2-SSID
    sessiontime        int unsigned default 0,         -- ��¼��ʱʱ��
    idletime           int unsigned default 0,         -- ���г�ʱʱ��
    macbindtime        int unsigned default 0,         -- Mac��ַ��ʱ��
    smslocalnum        int  unsigned,                  -- ���ڶ�����������
    smsinternum        int  unsigned,                  -- ���ʶ�����������
    smsusetime         char(64),                       -- ���ŷ�������ʱ��  00:00:00-12:12:12,13:14:14-20:00:00
    limittime          int unsigned default 0,         -- ÿ������ʱ������
    limitunit          int default 0,                  -- ���Ƶ�λ 0-�� 1-��  2-��  3-�� 9-�ܹ�
    passcodetime       int unsigned default 0,         -- �������Чʱ��
    servicetime        char(64),                       -- ��������ʱ��  00:00:00-12:12:12,13:14:14-20:00:00 
    eng_smsmessage     char(255),                      -- Ӣ�Ķ��Ÿ�ʽ
    zh_smsmessage      char(255),                      -- ���Ķ��Ÿ�ʽ
    autoauth           int default 0,                  -- �ڶ����Ƿ��Զ���֤
    macauth            int default 0,                  -- �޸�֪��֤
    weixinauth         int default 0,                  -- ��������֤ 1 ΢����֤
    maxmac             int default 5,                  -- һ���˺�����Mac��ַ������
    errornum           int default 5,
    errorlock          int default 0,
    addtime            int unsigned default 0,         -- ����ʱ��
    moditime           int unsigned default 0,
    paroption          text,                           -- ����ѡ��
    flags              int                             -- 1 ����ʹ��   0--δ��ͨ
) max_rows=1000000000 avg_row_length=256;

-- paroption Ϊ����ѡ�� SSIDRoaming="Yes",GroupRoaming="Yes"
create index ncsrvpar_i1 on ncsrvpar (ptype,pid);


-- SSID����
drop table if exists ncsrvgroupssid;
create table if not exists ncsrvgroupssid (
    sid                int unsigned  primary key,
    groupid            int unsigned default 0,
    ssid               char(32),                       -- �����豸SSID
    addtime            int unsigned default 0,         -- ����ʱ��
    moditime           int unsigned default 0,         -- ����޸�ʱ��
    flags              int                             -- 1 ����ʹ��   0--δ��ͨ
) max_rows=1000000000 avg_row_length=256;


create unique index ncsrvgroupssid_i1 on ncsrvgroupssid (groupid,ssid);
create index ncsrvgroupssid_i2 on ncsrvgroupssid (groupid);


-- �û���Ϣ
--  ��¼��Ψһ
--  ���û���Ϣ�У�Ŀǰ groupname,ssid,proname �������ֶβ���
drop table if exists ncsrvuser;
create table if not exists ncsrvuser (
    userid             int  unsigned  primary key,     -- �����û�������
    username           char(32),                       -- �û���¼��
    dispname           char(32),                       -- ��ʾ��
    groupname          char(32),                       -- ����
    groupid            int unsigned default 0,         -- GroupId
    ssid               char(32) default ' ',           -- ����SSID, ���ֵ���У���ʾ���û�ֻ��ͨ��ָ��SSID��AP��¼����������
    proname            char(32) default ' ',           -- Portal����
    openid             char(32) default ' ',           -- ���������ƣ��磺��Ա�ŵ�
//    usermac            char(96) default ' ',           -- �û�MAC�����Mac��ַ�ö��ŷָ�(����)
    password           char(32),                       -- ����
    mark               char(128) default ' ',          -- �û���ǩ
    addtime            int unsigned default 0,         -- ����ʱ��
    moditime           int unsigned default 0,
    lasttime           int unsigned default 0,         -- ������ʱ��
    expiretime         int unsigned default 0,         -- �ʺ�ʧЧʱ��,��ֵ��Ϊ0��ʾ�û���ʧЧ
    usertype           int default 0,                  -- �û���� 0--�̶��û�  1--����Ϣ  2--QQ  3--΢��
    flags              int                             -- 0--��Ч�û�  9--��Ч�û�
) max_rows=1000000000 avg_row_length=256;

create unique index ncsrvuser_i1 on ncsrvuser (groupid,username);
create index ncsrvuser_i2 on ncsrvuser (username);

-- alter table ncsrvuser modify usermac  char(96);
-- alter table ncsrvuser add  expiretime         int unsigned default 0;


-- �û���Mac��ַ�󶨱�
drop table if exists ncsrvusermac;
create table if not exists ncsrvusermac (
    usermac            char(18)  primary key,          -- �û�MAC
    username           char(32),                       -- �û���¼��
    groupid            int unsigned default 0,         -- ����
    starttime          int unsigned default 0,         -- ��ʼʱ��
    lasttime           int unsigned default 0,
    moditime           int unsigned default 0,         -- ����޸�ʱ��
    flags              int                             -- ��ǰ״̬  0--��Ч  9--��Ч
) max_rows=1000000000 avg_row_length=256;

create index ncsrvusermac_i1 on ncsrvusermac (username);






--  insert into ncsrvgroup(groupid,groupname,dispname,smsname,ssid,sessiontime,idletime,macbindtime,smslocalnum,smsinternum,smsusetime,limittime,limitunit,passcodetime,servicetime,eng_smsmessage,zh_smsmessage,autoauth,flags)
--      value (0,'apple','apple','apple','apple-test',1800,180,604800,10000,1000,'00:00:00-23:59:59',0,0,2592000,'00:00:00-23:59:59','Your WLAN username is: [#username#]  passcode is [#password#] ','���� WLAN �ʺ���:[#username#]  �����ǣ�[#password#]��',1,1);

-- �û�������

drop table if exists ncsrvblackuser;
create table if not exists ncsrvblackuser (
    utype              int,                            -- ���  1-Mac 2-Username
    groupid            int unsigned default 0,
    name               char(32),                       -- ����
    memo               char(64),                       -- ˵��
    validtime          int unsigned default 0          -- ʧЧʱ��
);

create unique index ncsrvblackuser_i on ncsrvblackuser (utype,name);

-- insert into ncsrvblackuser(utype,name,memo,validtime) value (1,"11:22:33:44:55:66","aaaa",0);
-- �û�����ʹ��ͳ��


-- �û������߼�¼
drop   table if exists ncsrvuserlog;
create table if not exists ncsrvuserlog (
    sid                bigint primary key,
    portalname         char(16),                       -- Portal Name
    userid             int unsigned default 0,
    groupid            int unsigned default 0,
    groupcode          char(32),
    stype              char(4),                        -- �̼����
    username           char(32),                       -- �û���¼��
    dispname           char(32),
    ssid               char(32),
    apname             char(40),
    mac                char(18),
    ip                 char(16),
    dev                char(24),
    os                 char(16),
    bro                char(16),                      -- �����
    devtype            char(8),
    ubytes             bigint,                        -- ��������
    dbytes             bigint,                        -- ��������
    starttime          int unsigned,                  -- ��ʼʱ��
    endtime            int unsigned,                  -- ��ֹʱ��
    conntime           int,
    cause              int                            -- ��ֹԭ��
) max_rows=1000000000 avg_row_length=256;

create index ncsrvuserlog_i1 on ncsrvuserlog (username);
create index ncsrvuserlog_i3 on ncsrvuserlog (groupid,username);
create index ncsrvuserlog_i2 on ncsrvuserlog (starttime);

-- �û����ʼ�¼
drop table if exists ncsrvloginlog;
create table if not exists ncsrvloginlog (
    sid         bigint auto_increment  primary key,
    userid             int unsigned,
    groupid            int unsigned default 0,
    servicecode        char(32),
    username           char(32),
    usermac            char(18),
    ip                 char(16),
    logtime            int unsigned,
    platename          char(32),                       -- ģ������
    fun                char(32),                       -- ����
    dev                char(24),
    os                 char(16),
    devtype            char(8),
    tsid               bigint default 0,
    useragent          char(255)                        -- Ua
) max_rows=1000000000 avg_row_length=512;

create unique index ncsrvloginlog_i1 on ncsrvloginlog (logtime);
create unique index ncsrvloginlog_i2 on ncsrvloginlog (fun);
create unique index ncsrvloginlog_i3 on ncsrvloginlog (username);


-- �û�����ע��
drop table if exists ncsrvsmslog;
create table if not exists ncsrvsmslog (
    sid                bigint auto_increment  primary key,
    userid             int unsigned,                  -- �û�ID,��ʱ����
    groupid            int unsigned default 0,
    groupcode          char(32),
    stype              char(4),
    servicecode        char(32) default ' ',          -- �����˱���(PortalName)
    did                int unsigned default 0,
    gid                int unsigned default 0,
    username           char(32),                      -- �û��˺�
    usermac            char(18),                      -- �û�Mac��ַ
    ip                 char(16),                      -- �û�IP��ַ
    logtime            int unsigned,                  -- ����ʱ��
    smsname            char(32) default ' ',          -- ���Ͷ���ƽ̨
    sptype             int,                           -- ��Ӫ�̱���
    areacode           int,                           -- ����
    areaname           char(32),                      -- ��������
    contrycode         int default 0,                 -- ���Ҵ���
    tsid               bigint default 0,
    feename            char(32) default ' ',          -- �Ʒѵ�λ
    status             int                            -- ����״̬ 1-�ɹ�  0-ʧ��
) max_rows=1000000000  avg_row_length=256;

create index ncsrvsmslog_i1 on ncsrvsmslog (logtime);
create index ncsrvsmslog_i2 on ncsrvsmslog (sptype);
create index ncsrvsmslog_i3 on ncsrvsmslog (areacode);
create index ncsrvsmslog_i4 on ncsrvsmslog (username);


drop table if exists ncadlog;
create table ncadlog(
    sid          int  unsigned  auto_increment primary key,
    refer        char(128) default '',   -- ����url
    url          char(128) default '',   -- ���URL
    ncname       char(36),               -- ��������
    username     char(32),               -- �����ʺ�
    sip          char(16),               --  IP
    mac          char(20),
    adcod        char(32),               -- ������
    apname       char(32),               -- ap����
    apssid       char(32),               -- apssid
    phtype       int unsigned,           -- �����ն� 1 ƽ�� 2 PC 3 �ֻ�
    os           char(16),               -- ����ϵͳ
    sptype       int unsigned,           -- ��Ӫ�����   0 ���Ϸ�  1 �ƶ� 2 ��ͨ 3 ���� 8 ���� 9 ����
    devtype      char(32),               -- �ն����� ƻ��  ����  ��Ϊ 
    browser      char(32),               -- �����
    areacode     int,                    -- ����
    areaname     char(32),               -- ��������  ����  ����  �Ϻ�
    contrycode   int default 0,          -- ���Ҵ���
    lasttime     int  unsigned,          -- ͳ��ʱ��
    lcount       int unsigned            -- 1  
);
create index ncadlog2 on ncadlog(lasttime);
create index ncadlog1 on ncadlog(adcod);


drop table if exists ncsrvtest;
create table ncsrvtest(
    lasttime     int  unsigned default 0
);


-- �����˹���������
-- insert into ncsrvgroup(groupid,groupname,dispname,smsname,sessiontime,idletime,macbindtime,smslocalnum,smsinternum,smsusetime,limittime,limitunit,passcodetime,servicetime,eng_smsmessage,zh_smsmessage,autoauth,flags)
--      value (1001,'pronetway','apple','apple',1800,180,604800,10000,1000,'00:00:00-23:59:59',0,0,2592000,'00:00:00-23:59:59','Your WLAN username is: [#username#]  passcode is [#password#] ','���� WLAN �ʺ���:[#username#]  �����ǣ�[#password#]��',1,1);
--
-- insert into ncsrvgroupap(gwid,groupid,longitude,latitude,flags) value ('test38',1001,0,0,1);

--  ҳ������
drop table if exists portalpage;
create table portalpage (
    pageid    int  primary key,
    pagename  char(128),                        -- ҳ������
    pagepath  char(128),                        -- ���·��
    addtime   int unsigned,                     -- ����ʱ��
    modtime   int unsigned                      -- ����޸�ʱ��
);
    

--  ģ������
drop table if exists portalplate;
create table portalplate (
    plateid     char(16)    primary key,
    shopplateid int default 0;
    platename   char(128),                       -- ģ������
    platepath   char(128),
    addtime    int unsigned,                     -- ����ʱ��
    modtime    int unsigned                      -- ����޸�ʱ��
);


-- ģ����Ե�ҳ���б�
drop table if exists portalplist;
create table portalplist (
    plateid    char(16),                       -- ģ��ID
    termtype   char(16),                       -- �ն���� pc,pad,mobile
    lang       char(16),                       -- ����, zh,eng
    pageid     int unsigned                    -- ҳ��ID
);

create index portalplist_ind1 on portalplist(plateid);

-- ģ��Ͷ�Ź���
drop table if exists portalprule;
create table portalprule (
    ruleid     int  primary key,                  -- ����ID, 
    priority   int  unsigned default 0,           -- ���ȼ�, ����Խ�����ȼ�Խ��
    plateid    char(16),                          -- ģ������
    groupid    int  unsigned default 0,           -- ��λID,�ϼ���λ�����¼���λ
    groupcode  char(32) default ' ',              -- ��λ����
    ssid       char(32),                          -- SSID
    nasid      char(32),                          -- �豸����ID������Ϊ�����������ͨ�����ʾ  *aa*,??aa*
    mark       char(255),                         -- ��ǩ�����Զ����ÿ��֮���ö��ŷָ�����:����,����
    dotime     char(255),                         -- Ͷ��ʱ����� 08:00:00-11:30:00,13:00:00-16:00:00
    dateid     int  unsigned default 0,           -- ����ID, 0 ÿ�� ����ֵ�μ�ʱ������
    starttime  int  unsigned  default 0,          -- ��ʼʱ��
    endtime    int  unsigned  default 0,          -- ��ֹʱ��
    addtime    int  unsigned  default 0,          -- ��¼����ʱ��
    modtime    int  unsigned  default 0,          -- ��¼����޸�ʱ��
    status     int  default 0                     -- ״̬ 0 ͣ��   1 ����
);
    
create index portalprule_ind1 on portalprule(starttime);
create index portalprule_ind2 on portalprule(priority);

-- λ�ñ�ǩ
drop table if exists portallocmark;
create table portallocmark (
    sid        int  unsigned  auto_increment primary key,
    mark       char(16) default ' ',                          -- ��ǩ
    mapid      char(16) default ' ',
    sx         double default 0,
    sy         double default 0,
    ex         double default 0,
    ey         double default 0
);
    
create index portallocmark_ind1 on portallocmark(mapid);
