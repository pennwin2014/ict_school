-- 2015/03/26 ncsrvpar ����   limitflow int unsigned default 0,  -- ÿ��������������,��λMB limitband          int unsigned default 0          -- �������� ��λkb
-- 2015/03/02 ncsmaclog_if_ �������������ֶ�associcated,bssid
-- 2014/12/13 ncsrvgroupap ������ plate,line,ssid,mapid���ֶ�
--            ���ӱ� ncsmaclog_if 
-- 2014/09/03 ncsrvsmslog ���� utype �ֶ�
-- 2014/08/18 ncsrvgroupap ���� model,version,mark,mac
-- 2014/08/04 portalplate  �������ֶ� groupcode    By Sms
-- 2014/07/27 �� ncsrvuser ������ stype
--            �� ncsrvgroupap ��  address
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
    flags              int default 1                   -- �Ƿ���Ч  1--��Ч 2--δ���� 9--ɾ��
);
create unique index ncsrvgroup_i1 on ncsrvgroup (groupname);


-- ��λ�ͽ����豸���ձ�
-- alter table ncsrvgroupap add mac char(18) default ' ';
-- alter table ncsrvgroupap add address char(255) default ' ';
-- alter table ncsrvgroupap add model char(16) default ' ';
-- alter table ncsrvgroupap add version char(16) default ' ';
-- alter table ncsrvgroupap add mark char(64) default ' ';
-- `    
-- alter table ncsrvgroupap add plate char(32) default ' ';
-- alter table ncsrvgroupap add line char(32) default ' ';
-- alter table ncsrvgroupap add mapid char(32) default ' ';
-- alter table ncsrvgroupap add ssid char(32) default ' ';

drop table if exists ncsrvgroupap;
create table if not exists ncsrvgroupap (
    gwid               char(32)   primary key,         -- �����豸ID, ��APID ��MAC
    mac                char(64)  default ' ',          -- AP��MAC��ַ
    groupid            int unsigned default 0,         -- ��Ӧ�ĵ�λID
    address            char(255) default ' ',          -- ��װ��ַ
    model              char(16)  default ' ',          -- �ͺ�
    version            char(16)  default ' ',
    mark               char(64)  default ' ',          -- ��ǩ
    plate              char(32)  default ' ',
    line               char(32)  default ' ',
    mapid              char(32)  default ' ',
    ssid               char(32)  default ' ',
    apname             char(32)  default ' ',          -- AP����
    longitude          double default 0,               -- λ�ã����ȣ�
    latitude           double default 0,               -- λ��(γ��)
    addtime            int unsigned default 0,         -- ����ʱ��
    lasttime           int unsigned default 0,         -- ���ʹ��ʱ��
    flags              int                             -- 1 ����ʹ��   0--δ��ͨ
) max_rows=1000000000 avg_row_length=256;

create index ncsrvgroupap_i1 on ncsrvgroupap (groupid);


-- ���ʺڰ�����
drop table if exists ncsrvwhite;
create table if not exists ncsrvwhite (
    pid                 int unsigned,                  -- ��ptype������ID,��:Groupid,
    ptype               int unsigned default 0,        -- ��� 0-ȫ��  1-Group   2-SSID    3--Stype
    wtype               int default 0,                 -- �ڰ�����     0--������   1--������
    nametype            int default 0,                 -- �������     1--����IP   2--����URL    3--�ն�MAC
    name                char(128)                      -- ����
) max_rows=1000000000 avg_row_length=256;

create index ncsrvwhite_i1 on ncsrvwhite (ptype);


-- ���� ncsrvpar 
drop table if exists ncsrvpar;
create table if not exists ncsrvpar (
    pid                 int unsigned,
    ptype               int unsigned default 0,        -- ��� 0-ȫ��  1-Group   2-SSID   3--Stype
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
    macauth            int default 0,                  -- �޸�֪��֤ 1-MacServer 2-Radius
    weixinauth         int default 0,                  -- ��������֤ 1 ΢����֤
    maxmac             int default 5,                  -- һ���˺�����Mac��ַ������
    errornum           int default 5,
    errorlock          int default 0,
    checksum           int unsigned default 0,         -- У��λ
    addtime            int unsigned default 0,         -- ����ʱ��
    moditime           int unsigned default 0,
    paroption          text,                           -- ����ѡ�� "radip=192.168.20.201:8999,radip1=192.168.21.202:8999"
    flags              int,                             -- 1 ����ʹ��   0--δ��ͨ
    limitflow          int unsigned default 0,         -- ÿ��������������,��λMB
    limitband          int unsigned default 0,          -- �������� ��λkb
    logincount         int unsigned default 0           -- ÿ������¼����
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
-- alter table ncsrvuser add stype int unsigned default 0;
-- stype��username Ψһ
drop table if exists ncsrvuser;
create table if not exists ncsrvuser (
    userid             int  unsigned  primary key,     -- �����û�������
    username           char(32),                       -- �û���¼��
    dispname           char(32),                       -- ��ʾ��
    groupname          char(32),                       -- ���� = groupcode
    groupid            int unsigned default 0,         -- GroupId 
    stype              int unsigned default 0,         -- stype
    ssid               char(32) default ' ',           -- ����SSID, ���ֵ���У���ʾ���û�ֻ��ͨ��ָ��SSID��AP��¼����������
    proname            char(32) default ' ',           -- Portal����
    openid             char(32) default ' ',           -- ���������ƣ��磺��Ա�ŵ�
    password           char(32),                       -- ����
    mark               char(128) default ' ',          -- �û���ǩ
    addtime            int unsigned default 0,         -- ����ʱ��
    moditime           int unsigned default 0,
    lasttime           int unsigned default 0,         -- ������ʱ��
    expiretime         int unsigned default 0,         -- �ʺ�ʧЧʱ��,��ֵ��Ϊ0��ʾ�û���ʧЧ
    usertype           int default 0,                  -- �û���� 0--�̶��û�  1--����Ϣ  2--QQ  3--΢��
    maxbindmac         int default 0,
    sumflow            int unsigned default 0,         -- ������, ��λ KB
    sumtime            int unsigned default 0,         -- ������ʱ��
    sumlogin           int unsigned default 0,         -- ��¼����
    flags              int                             -- 0--��Ч�û�  9--��Ч�û�   2--δ����
) max_rows=1000000000 avg_row_length=256;

create index ncsrvuser_i1 on ncsrvuser (groupname,username);
create unique index ncsrvuser_i2 on ncsrvuser (stype,username);


-- alter table ncsrvuser modify usermac  char(96);
-- alter table ncsrvuser add  expiretime         int unsigned default 0;


-- �û���Mac��ַ�󶨱�
-- alter table ncsrvusermac drop primary key;   // ��ԭ�����е�����ɾ��
drop table if exists ncsrvusermac;
create table if not exists ncsrvusermac (
    usermac            char(18),                       -- �û�MAC
    username           char(32),                       -- �û���¼��
    groupid            int unsigned default 0,         -- ���� stype
    starttime          int unsigned default 0,         -- ��ʼʱ��
    lasttime           int unsigned default 0,
    moditime           int unsigned default 0,         -- ����޸�ʱ��
    flags              int                             -- ��ǰ״̬  0--��Ч  9--��Ч  2--������
) max_rows=1000000000 avg_row_length=256;

create unique index ncsrvusermac_i1 on ncsrvusermac (groupid,usermac);






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

create unique index ncsrvblackuser_i on ncsrvblackuser (utype,groupid,name);

-- insert into ncsrvblackuser(utype,name,memo,validtime) value (1,"11:22:33:44:55:66","aaaa",0);
-- �û�����ʹ��ͳ��


-- �û������߼�¼(��������û�)
-- alter table ncsrvuserlog drop primary key;
drop   table if exists ncsrvuserlog;
create table if not exists ncsrvuserlog (
    sid                bigint default 0,
    portalname         char(32),                       -- Portal Name
    userid             int unsigned default 0,
    groupid            int unsigned default 0,         -- �û���Ӧ�ĵ�λ(�û�Դ��λid)
    gid                int unsigned default 0,         -- ��Ӧ���û����
    groupcode          char(32),                       -- �û����뵥λ
    stype              char(4),                        -- �̼����
    username           char(32),                       -- �û���¼��
    dispname           char(32),                       -- ��ʾ��
    ssid               char(32),                       -- SSID
    apname             char(40),                       -- APID
    mac                char(18),
    ip                 char(16),
    dev                char(24),
    os                 char(16),
    bro                char(16),                      -- �����
    devtype            char(8),
    utype              int unsigned default 0,        -- �û����
    ubytes             bigint,                        -- ��������
    dbytes             bigint,                        -- ��������
    starttime          int unsigned,                  -- ��ʼʱ��
    endtime            int unsigned,                  -- ��ֹʱ��
    conntime           int,                           -- ����ʱ��
    cause              int                            -- ��ֹԭ��
) max_rows=1000000000 avg_row_length=256;

create index ncsrvuserlog_i0 on ncsrvuserlog (sid);
create index ncsrvuserlog_i1 on ncsrvuserlog (username);
create index ncsrvuserlog_i2 on ncsrvuserlog (starttime);
create index ncsrvuserlog_i3 on ncsrvuserlog (groupid,username);
create index ncsrvuserlog_i4 on ncsrvuserlog (utype,username);


-- �û�������¼����¼�Ʒ��е�Stop����)

drop   table if exists ncsrvinternetlog;
create table if not exists ncsrvinternetlog (
    sid                bigint primary key,             -- SessionID, �� ncsrvuserlogһ��
    tsid               bigint,
    portalname         char(32),                       -- Portal Name
    userid             int unsigned default 0,
    groupid            int unsigned default 0,         -- �û���Ӧ�ĵ�λ(�û�Դ��λid)
    gid                int unsigned default 0,         -- ��Ӧ���û����(stype)
    groupcode          char(32),                       -- �û����뵥λ
    stype              char(4),                        -- �̼����
    username           char(32),                       -- �û���¼��
    dispname           char(32),                       -- ��ʾ��
    ssid               char(32),                       -- SSID
    apname             char(40),                       -- APID
    mac                char(18),
    ip                 char(16),
    dev                char(24),
    os                 char(16),
    bro                char(16),                      -- �����
    devtype            char(8),
    utype              int unsigned default 0,        -- �û����
    ubytes             bigint,                        -- ��������
    dbytes             bigint,                        -- ��������
    starttime          int unsigned,                  -- ��ʼʱ��
    endtime            int unsigned,                  -- ��ֹʱ��
    conntime           int,                           -- ����ʱ��
    cause              int                            -- ��ֹԭ��
) max_rows=1000000000 avg_row_length=256;


create index ncsrvinetlog_i1 on ncsrvinternetlog (username);
create index ncsrvinetlog_i2 on ncsrvinternetlog (starttime);
create index ncsrvinetlog_i3 on ncsrvinternetlog (groupid,username);
create index ncsrvinetlog_i4 on ncsrvinternetlog (utype,username);


-- �û���֤��¼(��¼�Ʒ��е�Start����)
drop   table if exists ncsrvauthlog;
create table if not exists ncsrvauthlog (
    sid                bigint primary key,             -- SessionID, �� ncsrvuserlogһ��
    tsid               bigint,                         -- ��Ӧ��Tsid
    portalname         char(32),                       -- Portal Name
    userid             int unsigned default 0,         -- Userid
    groupid            int unsigned default 0,         -- �û���Ӧ�ĵ�λ(�û�Դ��λid)
    gid                int unsigned default 0,         -- ��Ӧ���û����(stype)
    groupcode          char(32),                       -- �û����뵥λ
    username           char(32),                       -- �û���¼��
    dispname           char(32),                       -- ��ʾ��
    ssid               char(32),                       -- SSID
    apname             char(40),                       -- APID
    mac                char(18),
    ip                 char(16),
    dev                char(24),
    os                 char(16),
    bro                char(16),                      -- �����
    devtype            char(8),
    utype              int unsigned default 0,        -- �û����
    starttime          int unsigned,                  -- ��ʼʱ��
    flags              int                            -- ��֤��ʽ  1-- Web��֤    2--Web�Զ���֤    3--Mac��֤
) max_rows=1000000000 avg_row_length=256;


create index ncsrvauthlog_i1 on ncsrvauthlog (username);
create index ncsrvauthlog_i2 on ncsrvauthlog (starttime);
create index ncsrvauthlog_i3 on ncsrvauthlog (groupid,username);
create index ncsrvauthlog_i4 on ncsrvauthlog (utype,username);



drop table if exists ncsrvloginlog;
create table if not exists ncsrvloginlog (
    sid                bigint auto_increment  primary key,   -- TSID
    userid             int unsigned,                         -- �û�ID,����¼ʧ�ܣ���ֵΪ0
    groupid            int unsigned default 0,
    did                int unsigned default 0,
    gid                int unsigned default 0,
    servicecode        char(32),                            -- Portal Name
    username           char(32),
    usermac            char(18),
    ip                 char(16),
    logtime            int unsigned,
    platename          char(32),                            -- ģ������
    fun                char(32),                            -- ����
    dev                char(24),
    os                 char(16),
    devtype            char(8),
    apmac              char(18) default ' ',                -- AP MAC
    authway            int unsigned default 0,              -- ��֤��ʽ
    tsid               bigint default 0,
    status             int  default 0
) max_rows=1000000000 avg_row_length=512;

create  index ncsrvloginlog_i0 on ncsrvloginlog (sid);
create  index ncsrvloginlog_i1 on ncsrvloginlog (logtime);
create  index ncsrvloginlog_i2 on ncsrvloginlog (fun);
create  index ncsrvloginlog_i3 on ncsrvloginlog (username);



-- �û�����ע��
-- alter table ncsrvsmslog add apmac char(18) default ' ';

drop table if exists ncsrvsmslog;
create table if not exists ncsrvsmslog (
    sid                bigint auto_increment  primary key,
    userid             int unsigned,                  -- �û�ID,��ʱ����
    groupid            int unsigned default 0,
    groupcode          char(32),
    stype              char(4),
    servicecode        char(32) default ' ',          -- �����˱���(PortalName)
    did                int unsigned default 0,
    gid                int unsigned default 0,        -- �û����
    username           char(32),                      -- �û��˺�
    usermac            char(18),                      -- �û�Mac��ַ
    apmac              char(18),                      -- ����AP MAC
    ip                 char(16),                      -- �û�IP��ַ
    logtime            int unsigned,                  -- ����ʱ��
    smsname            char(32) default ' ',          -- ���Ͷ���ƽ̨
    sptype             int,                           -- ��Ӫ�̱���
    areacode           int,                           -- ����
    areaname           char(32),                      -- ��������
    contrycode         int default 0,                 -- ���Ҵ���
    tsid               bigint default 0,
    utype              int unsigned default 0,
    feename            char(32) default ' ',          -- �Ʒѵ�λ
    status             int                            -- ����״̬ 1-�ɹ�  0-ʧ��
) max_rows=1000000000  avg_row_length=256;

create index ncsrvsmslog_i1 on ncsrvsmslog (logtime);
create index ncsrvsmslog_i2 on ncsrvsmslog (sptype);
create index ncsrvsmslog_i3 on ncsrvsmslog (areacode);
create index ncsrvsmslog_i4 on ncsrvsmslog (username);


drop table if exists ncadlog;
create table ncadlog (
    sid          bigint    auto_increment primary key,
    tsid         bigint    default 0,
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
    groupcode   char(32),                        -- ��λ����
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



-- �û��������
-- 
-- insert into ncmautotype(nametype,name,namecode,shopcode,systime,flag) value (2,'ICPIC',    '010000','02',01010000,'̫���̶��û�',0,0);
-- insert into ncmautotype(nametype,name,namecode,shopcode,systime,flag) value (2,'CPICVIP',  '020000','02',01020000,'̫����ʱ�û�',0,0);
-- StypeΪ�̼ұ��룬��Portal�����ڹ���ĵ�һ����λ�ı��롣ͬһ��Stype���û���ϢҪ��Ψһ��ϵͳһ�����ã�Stype��ȷ����Stype��ʵʩ����������ͳһ���䡣
-- Stype�ı������Ϊ:
-- ǰ����λΪ��λ������λΪ�û����,  0��һ���û� 1--�̶�  2--�ֻ�   3--��Աϵͳ
-- Ŀǰ�ѷ����StypeֵΪ:
-- ��λ����	˵��	Stypeֵ	��Ӧ����
-- ����̫��	    ����ʡ̫���̶��û�	4100011	SSID: ICPIC
--              ����ʡ̫����ʱ�û�	4100012	SSID: CPICVIP
-- ����̩������	����̩������	    4100020	SSID: TKRS
-- ���ϵ���˹	����˹��ʱ�û�	    4100032	SSID: @Dennis-Guest
--              ����˹��ʱVIP�û�	4100033	SSID: @Dennis-VIP



drop table if exists ncmautotype;
create table ncmautotype (
    id          int   auto_increment  primary key,
    ssid             char(32),                                -- SSID
    groupcode        char(32),                                -- GroupCode  aaa*  *aaa* 
    ucode            char(32) default ' ',                    -- ��Ӧ��Group����
    namecode         char(8),                                 -- Ȩ�ޱ��� 4λһ��  0001
    shopcode         char(4),                                 -- �̳����� ����ncmshopgtype 2λ
    stype            int unsigned default 0,                  -- �û���id = shopcode * 1000000 + namecode
    descr            char(32),                                -- ����
    systime          int default 0,                           -- ����ϵͳʱ��
    flag             int default 0                            -- �Ƿ�ʹ�� 0--ʹ�� 9--��ʹ��
);



-- Macͳ����Ϣ
drop table if exists ncsrvmaclog;
create table ncsrvmaclog (
    pname            char(32),                                -- Portal Name
    mac              char(18),                                -- Mac��ַ
    apmac            char(18),                                -- AP Mac��ַ
    groupid          int unsigned default 0,                  -- ��λID
    groupcode        char(32),                                -- ��λ����
    mark             char(64)     default ' ',                -- ��ǩ
    lrssi            int          default 0,
    urssi            int          default 0,
    starttime        int unsigned default 0,                  -- ��ʼʱ��
    endtime          int unsigned default 0,                  -- ����ʱ��
    flag             int default 0                            -- 0--����  1--�뿪
);

create index ncsrvmaclog_ind1 on ncsrvmaclog(pname);
create index ncsrvmaclog_ind2 on ncsrvmaclog(mac);
create index ncsrvmaclog_ind3 on ncsrvmaclog(apmac);
create index ncsrvmaclog_ind4 on ncsrvmaclog(groupcode);
create index ncsrvmaclog_ind5 on ncsrvmaclog(mark);


-- AP����ͳ��

drop table if exists ncsapmaccount_if_201503;
create table ncsapmaccount_if_201503 (
    pname            char(32),                                -- Portal Name
    servicecode char(18)      default ' ',                 -- �����˱���
    servicename char(64)      default ' ',                 -- ����������
    address     char(64)      default ' ',                 -- ��װλ��
    apname      char(32),                                  -- AP����
    mapid       char(32)      default ' ',                 -- ��ͼ��ʶ(����ֵ���ڣ�����ά��Ϊ�룬x,y����
    longitude   char(16)      default ' ',                 -- ����
    latitude    char(16)      default ' ',                 -- ά��
    stime            int unsigned default 0,                  -- ��ʼʱ��
    maccount         int unsigned default 0,                   -- ��¼
    KEY ncsapmaccount_ind1 (stime)
);

create index ncsapmaccount_ind1 on ncsapmaccount(stime);
create index ncsapmaccount_ind2 on ncsapmaccount(apname,stime);
create index ncsapmaccount_ind3 on ncsapmaccount(servicecode,stime);



-- MapID����ͳ��

drop table if exists ncsapmarkcount_if_201503;
create table ncsapmarkcount_if_201503 (
    mapid            char(32)      default ' ',               -- ��ͼ��ʶ(����ֵ���ڣ�����ά��Ϊ�룬x,y����
    stime            int unsigned default 0,                  -- ��ʼʱ��
    maccount         int unsigned default 0                   -- ��¼
);


create index ncsapmarkcount_ind1 on ncsapmarkcount(stime);
create index ncsapmarkcount_ind2 on ncsapmarkcount(mapid,stime);


-- MAC��־��Ϣ
drop table if exists ncsmaclog_if_;
create table ncsmaclog (
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
    mapid       char(32)      default ' ',                 -- ��ͼ��ʶ(����ֵ���ڣ�����ά��Ϊx,y����
    longitude   char(16)      default ' ',                 -- ����
    latitude    char(16)      default ' ',                 -- ά��
    sendtime         int unsigned default 0,
    starttime        int unsigned default 0,               -- ����ʱ��
    endtime          int unsigned default 0,               -- �뿪ʱ��
    flags            int default 0,                        -- 0--����  1--�뿪
    associcated      int default 0,                        -- �Ƿ����
    bssid            char(18)                              -- ����Bssid
);


create index ncsmaclog_ind1 on ncsmaclog_if_201412(servicecode);
create index ncsmaclog_ind2 on ncsmaclog_if_201412(servicecode,mac);
create index ncsmaclog_ind3 on ncsmaclog_if_201412(mac);


drop table if exists ncmbandpar;                               -- ������Բ�����
create table ncmbandpar (
     name       char(32)      default ' ',                     -- ����
     value            int unsigned default 0                  -- ֵ
);
insert into ncmbandpar (name,value) values('16K',1);
insert into ncmbandpar (name,value) values('32K',2);
insert into ncmbandpar (name,value) values('64K',3);
insert into ncmbandpar (name,value) values('128K',4);
insert into ncmbandpar (name,value) values('256K',5);
insert into ncmbandpar (name,value) values('512K',6);
insert into ncmbandpar (name,value) values('1M',7);
insert into ncmbandpar (name,value) values('2M',8);
insert into ncmbandpar (name,value) values('4M',9);
insert into ncmbandpar (name,value) values('8M',10);
insert into ncmbandpar (name,value) values('16M',11);
insert into ncmbandpar (name,value) values('32M',12);


-- �û���֤����

drop table if exists ncmuserinfo;                             -- �û���Ϣ
create table ncmuserinfo (
     uid            char(32)      primary       key,
     username       char(32)      default ' ',                -- ����
     dispname       char(32)      default ' ',                -- ��ʾ��
     password       char(32)      default ' ',                -- ����
     usertype       int unsigned  default 0,                  -- 0 �ʺ�  1 �ֻ���  2 ���֤��
     addtime        int unsigned  default 0,                  -- ����ʱ��
     lasttime       int unsigned  default 0                   -- ���ʹ��ʱ��
);

create unique index ncmuserinfo_i1 on ncmuserinfo (username);

-- �û��ն˰���Ϣ
drop table if exists ncmuserbind;                    -- �û�����Ϣ
create table ncmuserbind (
     uid_sn         char(32)      primary           key,
     username       char(32)      default ' ',      -- �û���
     dispname       char(32)      default ' ',      -- ��ʾ��
     app_src        char(32)      default ' ',      -- �û���Դ
     sj_src         char(32)      default ' ',      -- ��Ƴ��� 
     user_mac       char(12)      default ' ',      -- �û�MAC��ַ
     imei           char(16)      default ' ',
     imsi           char(16)      default ' ',
     term           char(16)      default ' ',
     os             char(32)      default ' ',
     uid            char(32)      default ' ',
     addtime        int unsigned   default 0,
     lasttime       int unsigned   default 0,
     flags          int unsigned   default 0
);

create unique index ncmuserbind_i1 on ncmuserbind (username,user_mac);

-- �������
drop table if exists ncmnetname;                    -- �������
create table ncmnetname (
     uid_sn         char(32)      primary           key,
     username       char(32)      default ' ',      -- �û���
     dispname       char(32)      default ' ',      -- ��ʾ��
     app_src        char(32)      default ' ',      -- �û���Դ
     sj_src         char(32)      default ' ',      -- ��Ƴ��� 
     user_mac       char(12)      default ' ',      -- �û�MAC��ַ
     imei           char(16)      default ' ',
     imsi           char(16)      default ' ',
     term           char(16)      default ' ',
     os             char(32)      default ' ',
     uid            char(32)      default ' ',
     addtime        int unsigned   default 0,
     lasttime       int unsigned   default 0,
     flags          int unsigned   default 0
);


-- �������ʹ����־
drop table if exists ncmnetnamelog;                    -- �������
create table ncmnetnamelog (
     uid_sn         char(32)      primary           key,
     username       char(32)      default ' ',      -- �û���
     dispname       char(32)      default ' ',      -- ��ʾ��
     app_src        char(32)      default ' ',      -- �û���Դ
     sj_src         char(32)      default ' ',      -- ��Ƴ��� 
     user_mac       char(12)      default ' ',      -- �û�MAC��ַ
     imei           char(16)      default ' ',
     imsi           char(16)      default ' ',
     term           char(16)      default ' ',
     os             char(32)      default ' ',
     uid            char(32)      default ' ',
     addtime        int unsigned   default 0,
     lasttime       int unsigned   default 0,
     flags          int unsigned   default 0
);


-- �û�ʹ����־
drop table if exists ncmuserlog;                    -- �û�����Ϣ
create table ncmuserlog (
     sid            bigint        primary           key,
     uid_sn         char(32)      default ' ',
     username       char(32)      default ' ',      -- �û���
     dispname       char(32)      default ' ',      -- ��ʾ��
     app_src        char(32)      default ' ',      -- �û���Դ
     sj_src         char(32)      default ' ',      -- ��Ƴ��� 
     user_mac       char(12)      default ' ',      -- �û�MAC��ַ
     user_ip        char(15)      default ' ',      -- �û�IP��ַ
     imei           char(16)      default ' ',
     imsi           char(16)      default ' ',
     term           char(16)      default ' ',
     os             char(32)      default ' ',
     uid            char(32)      default ' ',
     stime          int unsigned   default 0,
     etime          int unsigned   default 0,
     flags          int unsigned   default 0        -- 1 ����   2 ����   3 ����
);



