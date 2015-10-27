/******************************
*   ������ʱ��: 2014/01/09   By Liyunming
*   portalMV1.0   edit by zms     *
******************************/
-- ע��
-- create database ncm;
-- grant all privileges on ncm.*  to ncm@localhost identified by 'ncm';
-- use ncm;
-- �˵�Ȩ��


insert into dsroleattr values(1,1004,0,"ϵͳ����");
insert into dsroleattr values(1,1004,0,"��֤����");
insert into dsroleattr values(1,1004,0,"ϵͳ����");
insert into dsroleattr values(1,1004,0,"�û�����");
insert into dsroleattr values(1,1004,0,"�ŵ����"); 
insert into dsroleattr values(1,1004,0,"ͳ�Ʊ���");
insert into dsroleattr values(1,1004,0,"������־");
insert into dsroleattr values(1,1004,0,"�̳ǹ���");

-- ��λ��Ϣ
DROP TABLE IF EXISTS ncmuser;
CREATE TABLE ncmuser (
  userid         int        primary key,
  username       char(16)   default NULL,                     -- �ŵ�����
  password       char(32)   default NULL,                     -- ����
  servicecode    char(16)   default '',                       -- ��������
  groupid        int(11)    default '0',                      -- ��id
  dispname       char(64)   default NULL,                     -- ��ʾ��
  address        char(128)  default NULL,                     -- ��ַ
  telphone       char(64)   default NULL,                     -- �绰
  contact        char(32)   default NULL,                     -- ��ϵ������
  handphone      char(32)   default NULL,                     -- �ֻ�
  email          char(128)  default '',                       -- ����
  url            char(255)  default '',                       -- ��ַ
  ceoname        char(30)   default '',                       -- ������
  postcode       char(6)    default '',                       -- ��������
  servicestate   char(1)    default '9',                      -- ����״̬ 1-װ����ʼ���� 2-װ��ֹͣ���� 3-δװ����ʼ���� 4-δװ��ֹͣ���� 9-����
  joincode       char(6)    default '',                       -- ��������̴���
  endnum         char(6)    default '0',                      -- �ŵ�����  �ն���
  servernum      char(2)    default '0',                      -- ����������
  empnum         char(6)    default '0',                      -- ����û�����
  area           char(70)   default '',                       -- ����
  areaman        char(30)   default '',                       -- ������ϵ��
  areatel        char(30)   default '',                       -- ������ϵ�˵绰
  demo           char(255)  default '',                       -- ��ע
  ip             char(16)   default '',                       -- IP
  addtime        char(16)   default '',                       -- ����ʱ��   
  useflags       int(11)    default '0',                      -- ״̬ 0����
  moditime       int(10)    unsigned default '0',             -- �޸�ʱ��
  version        char(16)   default '0',                      -- �汾��
  lasttime       char(16)   default NULL,                     -- ���һ������ʱ��
  longitude      bigint(20) default '0',                      -- ����
  latitude       bigint(20) default '0'                       -- γ��
  fcode          char(10) default '',                         -- ���̴��� 
  dateid         int(11) default '0'                          -- Ӫҵʱ��
) ;

-- ������ݿ�
DROP TABLE IF EXISTS ncmenduser;
CREATE TABLE ncmenduser (
  sid              int auto_increment primary key,
  did              int(10) unsigned default NULL,         -- �ŵ�id
  uid              int(10) unsigned default NULL,         -- �û�id 
  gid              int(10) unsigned default NULL,         -- ��id
  username         char(32) default '',                   -- �û���
  dispname         char(32) default NULL,                 -- ��ʾ��
  namepy           char(64) default '',                   -- ���ֵ�ƴ��
  ip               int(10) unsigned default NULL,         -- ip 
  mac              char(18) default '',                   -- mac
  compname         char(32) default '',                   -- ���ڵ���
  email            char(64) default '',                   -- ����
  position         char(32) default '',                   -- λ��
  usebase          int(11) default '0',                   -- �û�����ʽ
  syntime          int(10) unsigned default '0',          -- ��Ϣ����ʱ��
  lasttime         int(10) unsigned default '0',          -- ���һ�β���ʱ��
  flag������������ int(10) unsigned default '0',          -- flag=1 �ص��ض���
  country          char(3) default '',                    -- ���Ҵ���
  idfytype         char(3) default '',                    -- ֤������
  idfyunit         char(30) default '',                   -- ��֤��λ
  sex              char(1) default ''                     -- �Ա�
) ;

-- ����Ϣ--��Ӧ������
drop table if exists ncmgroup;
create table ncmgroup (
    groupid    int    primary key,          -- ��ID
    groupname  char(32),                    -- ����
    groupcode  char(10) default '',         -- ����������
    descr      char(128),                   -- ˵��
    pid        int   default 0,             -- �ϼ���
    upip       char(128)   default '',      -- �ϴ���־������IP ��������
    upuser     char(32)   default '',       -- �ϴ���־��¼�û���
    uppasswd   char(32) default ''
)engine=MyISAM;

create UNIQUE index ncsgroup_index on ncsgroup (groupname,pid);


drop table if exists ncfacgroup;  
create table ncfacgroup (
    fid        int unsigned auto_increment primary key,
    groupid    int  ,               -- ������
    name       char(32),            -- ��������
    flags      int  default 0       -- 
);

insert into ncfacgroup(groupid,name,flags) values(0,"����",0);
insert into ncfacgroup(groupid,name,flags) values(1,"ƻ��",0);
insert into ncfacgroup(groupid,name,flags) values(2,"����",0);
insert into ncfacgroup(groupid,name,flags) values(3,"����",0);
insert into ncfacgroup(groupid,name,flags) values(4,"��Ϊ",0);
insert into ncfacgroup(groupid,name,flags) values(5,"����",0);
insert into ncfacgroup(groupid,name,flags) values(6,"С��",0);

insert into ncfacgroup(groupid,name,flags) values(0,"samsung",0);
insert into ncfacgroup(groupid,name,flags) values(1,"apple",0);
insert into ncfacgroup(groupid,name,flags) values(2,"lennovo",0);
insert into ncfacgroup(groupid,name,flags) values(3,"yulong",0);
insert into ncfacgroup(groupid,name,flags) values(4,"huawei",0);
insert into ncfacgroup(groupid,name,flags) values(5,"zte",0);
insert into ncfacgroup(groupid,name,flags) values(6,"xiaomi",0);

insert into ncfacgroup(groupid,name,flags) values(0,"����",1);
insert into ncfacgroup(groupid,name,flags) values(1,"Ӣ��",1);
insert into ncfacgroup(groupid,name,flags) values(2,"Ĭ��",1);

insert into ncfacgroup(groupid,name,flags) values(0,"PC"  ,2);
insert into ncfacgroup(groupid,name,flags) values(1,"ƽ��",2);
insert into ncfacgroup(groupid,name,flags) values(2,"�ֻ�",2);
insert into ncfacgroup(groupid,name,flags) values(3,"Ĭ��",2);

insert into ncfacgroup(groupid,name,flags) values(0,"û�з���"  ,4);
insert into ncfacgroup(groupid,name,flags) values(1,"�Ѿ����ʹ�",4);

drop table if exists nctypeindex;
create table nctypeindex (
    sid        int  primary key,                -- ����ID
    name       char(32) default ' ',            -- ��������
    groupid    int  default 0,                  -- �������
    id         int  default 0,                  -- ԭ������
    descr      char(128) default ' ',           -- ����
    flags      int  default 0                   -- 
);
insert into nctypeindex values(101,"glaxy",  0,101,101,"",0);
insert into nctypeindex values(102,"glaxy tab", 0,102,102,"",0);


drop table if exists ncpattern;
create table ncpattern (
    sid        int  primary key,                -- �ͺ�ID
    name       char(32) default ' ',            -- �ͺ�����
    groupid    int  default 0,                  -- �������
    id         int  default 0,                  -- ԭ������
    descr      char(128) default ' ',           -- ����
    flags      int  default 0                   -- 
);



drop table if exists ncfacgroup;  
create table ncfacgroup (
    groupid    int  primary key,    -- ������
    name       char(32),            -- ��������
    flags      int  default 0       -- 
);

-- iframe ���
--  ģ���           Ӧ��
drop table if exists nctemplate;
create table nctemplate (
    tid            int     primary key,
    did            int  unsigned,                         -- �ŵ�ID
    gid            int  unsigned,                         -- ��ID
    mname          char(64)  ,                            -- ģ������
  	mtype          char(32)  ,                            -- ģ������(ģ������)
  	mtemtype       char(32)  ,                            -- ģ���ն�����
    mlan           char(12)  ,                            -- ģ������
  	platepath      char(128) ,                            -- ��̬̬ҳ����Ŀ¼
    msystime       int unsigned default 0,                -- ϵͳʱ��
    mgid           int unsigned default 0,                -- ģ����id
    mesg           varchar(64)                            -- ˵��
);
alter table nctemplate add column mgid int unsigned default 0;



--  ģ��ά����
drop table if exists ncmodule;
create table ncmodule (
    mid             int     primary key,
    did             int  unsigned,                         -- �ŵ�ID
    gid             int  unsigned,                         -- ��ID
    m_name          char(64)  ,                            -- ģ������          ���ģ������
  	m_temtype       char(32)  ,                            -- ģ���ն�����
  	m_lan           char(12)  ,                            -- ģ������
    platepath       char(128) ,                            -- ��̬̬ҳ����Ŀ¼
    m_char          char(12)  ,                            -- ģ���ַ���
    mesg            varchar(256),                          -- ˵��
    downname        varchar(256),                          -- ��������
    m_systime       int unsigned default 0                 -- ϵͳʱ��
);



-- ϵͳ�û���Ϣ �漰Ȩ�ޣ��ɼ��ֶΣ������ؽ���
DROP TABLE IF EXISTS `dsuser`;
CREATE TABLE `dsuser` (
  `id` int(11) NOT NULL default '0',
  `name` varchar(32) default NULL,
  `dispname` varchar(32) default NULL,
  `groupid` int(11) default NULL,
  `groupname` varchar(32) default NULL,
  `addtime` int(10) unsigned default NULL,
  `moditime` int(10) unsigned default NULL,
  `lkey` varchar(24) default NULL,
  `lasttime` int(10) unsigned default NULL,
  `logcount` int(11) default NULL,
  `useflags` smallint(6) default NULL,
  `status` smallint(6) default NULL,
  `usrlevel` smallint(6) default NULL,
  `email` varchar(64) default NULL,
  `key2` varchar(24) default '',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `dsuser_01` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

alter table dsuser add column company varchar(64) default '';        -- 20.21����



drop table if exists ncmaccnf;  
create table ncmaccnf (
    aid        int unsigned auto_increment primary key,
    name       char(32),            -- AC����
    ip         char(24),            -- AC IP
    port       char(8),             -- AC�˿�
    addtime    int  default 0,      -- ����ʱ��
    flags      int  default 0       -- ɾ�� ����Ϊ1
);



-- �û���Ϣ
drop table if exists ncsrvuser;
create table if not exists ncsrvuser (
    userid             int  unsigned  primary key,     -- �����û�������
    did                int  unsigned,                  -- �ŵ�ID
    gid                int  unsigned,                  -- ��ID
    username           char(32),                       -- �û���¼��
    dispname           char(32),                       -- ��ʾ��
    groupname          char(16),                       -- �û���
    proname            char(16),                       -- Portal����
    usermac            char(96),                       -- �û�MAC�����Mac��ַ�ö��ŷָ�
    password           char(32),                       -- ����
    lcount             int unsigned default 0,         -- ���ʴ���
    addtime            int unsigned default 0,         -- ����ʱ��
    lasttime           int unsigned default 0,         -- ������ʱ��
    expiretime         int unsigned default 0,         -- ʧЧʱ��
    flags              int                             -- 0--û�з���  1--�Ѿ����ʹ�
) max_rows=1000000000 avg_row_length=256;

create index ncsrvuser_i1 on ncsrvuser (groupname);


-- �û���Mac��ַ�󶨱�
drop table if exists ncsrvusermac;
create table if not exists ncsrvusermac (
    usermac            char(18)  primary key,          -- �û�MAC
    username           char(32),                       -- �û���¼��
    starttime          int unsigned default 0,         -- ʧЧʱ��
    lasttime           int unsigned default 0,         -- ������ʱ��
    flags              int                             -- ��ǰ״̬ 0--��Ч  1--��Ч
) max_rows=1000000000 avg_row_length=256;

create index ncsrvusermac_i1 on ncsrvusermac (username);

-- �û������߼�¼

drop   table if exists ncsrvuserlog;
create table if not exists ncsrvuserlog (
    sid         bigint auto_increment  primary key,
    portalname         char(16),                       -- Portal Name
    did                int  unsigned,                  -- �ŵ�ID
    gid                int  unsigned,                  -- ��ID
    userid             int unsigned,
    username           char(32),                       -- �û���¼��
    dispname           char(32),
    ssid               char(32),
    apname             char(40),
    mac                char(18),
    ip                 char(16),
    dev                char(24),
    os                 char(16),
    devtype            char(8),
    ubytes             bigint,                        -- ��������
    dbytes             bigint,                        -- ��������
    starttime          int unsigned,                  -- ��ʼʱ��
    endtime            int unsigned,                  -- ��ֹʱ��
    conntime           int,
    cause              int                            -- ��ֹԭ��
) max_rows=1000000000 avg_row_length=256;

create index ncsrvuserlog_i1 on ncsrvuserlog (username);
create index ncsrvuserlog_i2 on ncsrvuserlog (starttime);




-- �û���
drop table if exists ncsrvgroup;
create table if not exists ncsrvgroup (
    groupname          char(32) primary key,           -- �û���¼��
    did                int  unsigned,                  -- �ŵ�ID
    dispname           char(32),                       -- ��ʾ��
    sessiontime        int unsigned default 0,         -- ��¼��ʱʱ��
    idletime           int unsigned default 0,         -- ���г�ʱʱ��
    macbindtime        int unsigned default 0,         -- Mac��ַ��ʱ��
    moditime           int unsigned default 0          -- ����޸�ʱ��
);


-- �û�������

drop table if exists ncsrvblackuser;
create table if not exists ncsrvblackuser (
    utype              int,                            -- ���  1-Mac 2-Username
    did                int  unsigned,                  -- �ŵ�ID
    gid                int  unsigned,                  -- ��ID
    name               char(32),                       -- ����
    memo               char(64),                       -- ˵��
    validtime          int unsigned default 0          -- ʧЧʱ��
);

create unique index ncsrvblackuser_i on ncsrvblackuser (utype,name);



-- �û�����ʹ��ͳ��
drop table if exists ncsrvloginlog;
create table if not exists ncsrvloginlog (
    sid         bigint auto_increment  primary key,
    userid             int unsigned,
    username           char(32),
    did                int  unsigned,                  -- �ŵ�ID
    gid               int  unsigned,                  -- ��ID
    usermac            char(18),
    ip                 char(16),
    logtime            int unsigned,
    platename          char(16),                       -- ģ������
    fun                char(32),                       -- ����
    dev                char(24),
    os                 char(16),
    devtype            char(8),
    useragent          char(256)                        -- Ua
) max_rows=1000000000 avg_row_length=512;;

create unique index ncsrvloginlog_i1 on ncsrvloginlog (logtime);
create unique index ncsrvloginlog_i2 on ncsrvloginlog (fun);
create unique index ncsrvloginlog_i3 on ncsrvloginlog (username);


-- �û�����ע��
drop table if exists ncsrvsmslog_if_201312;
create table if not exists ncsrvsmslog_if_201312 (
    sid                bigint auto_increment  primary key,
    did                int  unsigned,                  -- �ŵ�ID
    gid               int  unsigned,                  -- ��ID
    userid             int unsigned,
    gid                int unsigned,
    username           char(32),
    usermac            char(18),
    ip                 char(16),
    logtime            int unsigned,
    sptype             int unsigned,                  -- ��Ӫ����� 
    areacode           int,                           -- ����
    areaname           char(32),                      -- ����
    status             int                            -- ����״̬
) engine=MYISAM;


insert into ncsrvsmslog_if_201312 values(1,"50015821000067",1,1004,"13641629927","E4-98-D6-32-33-64","10.1.1.33",1388062184,1,086,"�й�",0),
                                        (2,"50015321000051",2,1004,"13541629925","5C-DA-D4-D3-75-6F","10.1.1.33",1388064528,1,086,"�й�",0),
                                        (3,"50010422000002",3,1004,"18939825505","14-8F-C6-78-F3-0C","10.1.1.33",1388032177,1,086,"�й�",0),
                                        (4,"50023421000069",4,1004,"18621628797","E4-98-D6-32-33-64","10.1.1.33",1388042131,1,086,"�й�",0),
                                        (5,"50015321000052",5,1004,"13801728925","E4-98-D6-32-33-64","10.1.1.33",1388012122,1,086,"�й�",0),
                                        (6,"50023421000039",6,1004,"13801674398","E4-98-D6-32-33-64","10.1.1.33",1388032184,1,086,"�й�",0),
                                        (7,"50023421000018",7,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388062124,1,086,"�й�",0),
                                        (8,"50023421000121",8,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388062384,1,086,"�й�",0),
                                        (9,"50023421000102",9,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388069190,1,086,"�й�",0),
                                        (10,"50023421000058",10,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388262184,1,086,"�й�",0),
                                        (11,"50023421000125",11,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388066124,1,086,"�й�",0),
                                        (12,"50023421000037",12,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388069154,1,086,"�й�",0),
                                        (13,"50023421000043",13,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388032181,1,086,"�й�",0),
                                        (14,"50023421000089",14,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388061183,1,086,"�й�",0),
                                        (15,"50023421000091",15,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388052164,1,086,"�й�",0);

------------------------------------------------------------------------------------------------

insert into radgroupreply (groupname,attribute,op,value) values ("user","Auth-Type",":=","Local");
insert into radgroupreply (groupname,attribute,op,value) values ("user","Service-Type",":=","Framed-User");
insert into radgroupreply (groupname,attribute,op,value) values ("user","Framed-IP-Address",":=","255.255.255.254");
insert into radgroupreply (groupname,attribute,op,value) values ("user","Framed-IP-Netmask",":=","255.255.255.0");


��������˺�

insert into radcheck (username,attribute,op,value) values ("test123","User-Password",":=","test234");
insert into radcheck (username,attribute,op,value) values ("13801674398","User-Password",":=","123456");
�����˺ż�����
insert into radusergroup (username,groupname) values ("test123","user");
insert into radusergroup (username,groupname) values ("13801674398","user");

grant all privileges on *.* to radius@192.168.20.38 identified by 'radius' with grant option;


insert into radgroupreply (groupname,attribute,op,value) values ("user","Auth-Type",":=","Local");
insert into radgroupreply (groupname,attribute,op,value) values ("user","Service-Type",":=","Framed-User");
insert into radgroupreply (groupname,attribute,op,value) values ('user','Framed-IP-Address',':=','255.255.255.254');
insert into radgroupreply (groupname,attribute,op,value) values ('user','Framed-IP-Netmask',':=','255.255.255.0');
insert into radgroupreply (groupname,attribute,op,value) values ('user','Session-Timeout',':=','7200');
insert into radgroupreply (groupname,attribute,op,value) values ('user','Idle-Timeout',':=','1800');

-- �̳�������
-- 1������ -->¥�� -->  �̱� -->�̵� -->��Ʒ -->����
-- 2������̱� -->�̵� --����-- > С�� --> ��Ʒ
-- 3�������̱� -->�̵� --> ��Ʒ -->       

drop table if exists ncmmall;  
create table ncmmall (
    fid        int unsigned auto_increment primary key,
    groupid    int  ,                                    -- ������
    name       char(32),                                 -- ����
    imgpath    char(128),                                -- ͼƬ·��
    detail     text,                                     -- ����
    flags      int  default 0                            -- 
);

insert into ncmmall(groupid,name,flags) values(0,"¥������",0);
insert into ncmmall(groupid,name,flags) values(1,"��Ʒ����",0);
insert into ncmmall(groupid,name,flags) values(2,"�����Ż�",0);
insert into ncmmall(groupid,name,flags) values(3,"��������",0);

insert into ncmmall(groupid,name,flags,detail) values(0,"B2��",1,"ͣ����");
insert into ncmmall(groupid,name,flags,detail) values(1,"B1��",1,"ʳƷ����");
insert into ncmmall(groupid,name,flags,detail) values(2,"һ��",1,"�ݳ�Ʒ����ױƷ���鱦");
insert into ncmmall(groupid,name,flags,detail) values(3,"����",1,"�·���������Ůװ");
insert into ncmmall(groupid,name,flags,detail) values(4,"����",1,"��װ��ƤЬ");
insert into ncmmall(groupid,name,flags,detail) values(5,"�Ĳ�",1,"�������Ҿ�");
insert into ncmmall(groupid,name,flags,detail) values(6,"���",1,"������Ʒ");
insert into ncmmall(groupid,name,flags,detail) values(7,"����",1,"��ʳ������ӰԺ");


insert into ncmmall(groupid,name,flags,) values(0,"B2��",1);
insert into ncmmall(groupid,name,flags,) values(1,"B1��",1);
insert into ncmmall(groupid,name,flags) values(2,"һ��",1);
insert into ncmmall(groupid,name,flags) values(3,"����",1);
insert into ncmmall(groupid,name,flags) values(4,"����",1);
insert into ncmmall(groupid,name,flags) values(5,"�Ĳ�",1);
insert into ncmmall(groupid,name,flags) values(6,"���",1);
insert into ncmmall(groupid,name,flags) values(7,"����",1);
insert into ncmmall(groupid,name,flags) values(8,"�߲�",1);
insert into ncmmall(groupid,name,flags) values(9,"�˲�",1);
insert into ncmmall(groupid,name,flags) values(10,"�Ų�",1);
insert into ncmmall(groupid,name,flags) values(11,"ʮ��",1);

insert into ncmmall(groupid,name,flags) values(0,"logo1",2);
insert into ncmmall(groupid,name,flags) values(1,"logo2",2);
insert into ncmmall(groupid,name,flags) values(2,"logo3",2);
insert into ncmmall(groupid,name,flags) values(3,"logo4",2);


-- ��Ʒ��������
drop table if exists ncmcomgroup;  
create table ncmcomgroup (
    groupid    int  primary key,           -- ������
    name       char(32),                   -- ��Ʒ��������
    descr      char(128) default ' ',      -- ����
    systime    int unsigned,               -- ����ʱ��
    imgpath    char(128),                  -- ͼƬ·��
    flags      int  default 0              -- 
);
-- ��ƷС������
drop table if exists ncmcomtype;  
create table ncmcomtype (
    sid        int  primary key,           -- ������
    name       char(32),                   -- ��ƷС������
    groupid    int  default 0,             -- �������
    descr      char(128) default ' ',      -- ����
    systime    int unsigned,               -- ����ʱ��
    imgpath    char(128),                  -- ͼƬ·��
    flags      int  default 0              -- 
);


-- �̱�ϸ��
drop table if exists ncmcomindex;
create table ncmcomindex (
    sid           int  primary key,                -- �̱�ID
    name          char(32) default ' ',            -- �̱�����
    did           int  unsigned,                   -- �ŵ�ID
    gid           int  unsigned,                   -- ��ID
    sgroupid      int  default 0,                  -- �������
    descr         char(128) default ' ',           -- ����
    imgpath       char(128),                       -- ͼƬ·��
    serno         char(32),                        -- �̱����к�
    floor         char(32),                        -- ¥��
    level         int unsigned default 0,          -- ������ȼ�
    plevel        int unsigned default 0,          -- �������ȼ�
    type          int unsigned default 0,          -- 1 ��� 2 Ʒ�� 3����  4��Ƶ
    systime       int unsigned,                    -- ����ʱ��
    shopid        int unsigned default 0,          -- �̵�id
    flags         int  default 0                   -- 
);

-- ��Ʒϸ��
drop table if exists ncmcomodity;
create table ncmcomodity (
    sid          int  primary key,                -- ��ƷID
    name         char(32) default ' ',            -- ��Ʒ����
    dispname     char(32) default ' ',            -- ��Ʒ��ʾ����
    did          int  unsigned,                   -- �ŵ�ID
    gid          int  unsigned,                   -- ��ID
    sgroupid     int  default 0,                  -- �̱����
    groupid      int  default 0,                  -- �������
    shgroupid    int  default 0,                  -- �̵�id
    imgpath      char(128),                       -- ͼƬ·��        
    shopname     char(32) default ' ',            -- �̵�����
    shimgpath    char(128),                       -- �̵�ͼƬ
    factory      char(32) default ' ',            -- ����
    type         char(128),                       -- ��Ʒ�ͺ�
    url          char(255),                       -- ��ַ
    floor        char(32),                        -- ¥��
    fldetail     char(255),                       -- ¥�����
    fee          float(10,2) default 0,           -- �ۼ�
    code         char(12) default '',             -- ��������/���Ҵ���
    tel          char(32) default '',             -- �绰
    address      char(255) default ' ',           -- ��ַ
    reserve      int unsigned,                    -- ���
    active       char(64) default ' ',            -- �Ƿ����
    plevel       int unsigned default 0,          -- ����ȼ�
    level        int unsigned default 0,          -- ������ȼ�
    descr        char(255) default ' ',           -- ����
    detail       text ,                           -- ����
    integral     int unsigned default 0,          -- ����
    coupon       int unsigned default 0,          -- �Ż�ȯ
    systime      int unsigned,                    -- ����ʱ��
    flags        int  default 0                   -- 0Ĭ�ϴ���  1 ɾ��   
);

-- �̵��
drop table if exists ncmshop;
create table ncmshop (
    id           int  primary key,                 -- �̵�ID
    name         char(32) default ' ',             -- �̵�����
    did          int  unsigned,                    -- �ŵ�ID
    gid          int  unsigned,                    -- ��ID
    sgroupid     int  default 0,                   -- �������
    imgpath      char(128),                        -- ͼƬ·��
    floor        char(32),                         -- ¥��
    url          char(255),                        -- ��ַ
    code         char(12) default '',              -- ��������/���Ҵ���
    tel          char(32) default '',              -- �绰
    address      char(255) default ' ',            -- ��ַ
    level        int unsigned default 0,           -- ������ȼ�
    plevel       int unsigned default 0,           -- �������ȼ�
    type         int unsigned default 0,           -- 1 ��� 2 Ʒ��
    stype        int unsigned default 0,           -- �̵�����
    descr        char(255) default ' ',            -- ����
    detail       text ,                            -- ����
    systime      int unsigned,                     -- ����ʱ��
    flags        int  default 0                    -- 
);



-- �̳��û���Ϣ
drop table if exists ncmshopuser;
create table if not exists ncmshopuser (
    uid                int  primary key,                -- �̳��û�id
    username           char(32)  primary key,           -- �û���¼��
    did                int  unsigned,                   -- �ŵ�ID
    gid                int  unsigned,                   -- ��ID
    dispname           char(32)  default '',            -- ��ʾ��
    groupname          char(16)  default '',            -- �û���
    proname            char(16)  default '',            -- Portal����
    usermac            char(18)  default '',            -- �û�MAC
    password           char(32)  default '',            -- ����
    nickname           char(32)  default '',            -- �ǳ�
    membernum          char(32)  default '',            -- ��Ա��
    integral           int unsigned default 0,          -- ����
    coupon             int unsigned default 0,          -- �Ż�ȯ
    address            char(128) default '',            -- ��ַ
    decript            text,                            -- ���˹�ע
    lcount             int unsigned default 0,          -- ���ʴ���
    addtime            int unsigned default 0,          -- ����ʱ��
    lasttime           int unsigned default 0,          -- ������ʱ��
    flags              int                              -- 0--û�з���  1--�Ѿ����ʹ�
);

-- �û����ѱ�
drop table if exists ncmconsump;
create table if not exists ncmconsump (
    cid                int  primary key,                -- id
    username           char(32)  primary key,           -- �û���¼��
    did                int  unsigned,                   -- �ŵ�ID
    gid                int  unsigned,                   -- ��ID
    dispname           char(32)  default '',            -- ��ʾ��
    usermac            char(18)  default '',            -- �û�MAC
    sname              char(64)  default '',            -- ��Ʒ����
    sid                int unsigned default 0,          -- ��Ʒid
    fee                float(10,2) default 0,           -- �ۼ�
    date               int unsigned default 0,          -- ��������
    uid                int unsigned default 0,          -- �û�id
    lasttime           int unsigned default 0           -- ������ʱ��
);
