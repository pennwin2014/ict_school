/******************************
*   最后更新时间: 2014/01/09   By Liyunming
*   portalMV1.0   edit by zms     *
******************************/
-- 注：
-- create database ncm;
-- grant all privileges on ncm.*  to ncm@localhost identified by 'ncm';
-- use ncm;
-- 菜单权限


insert into dsroleattr values(1,1004,0,"系统管理");
insert into dsroleattr values(1,1004,0,"认证界面");
insert into dsroleattr values(1,1004,0,"系统配置");
insert into dsroleattr values(1,1004,0,"用户管理");
insert into dsroleattr values(1,1004,0,"门店管理"); 
insert into dsroleattr values(1,1004,0,"统计报表");
insert into dsroleattr values(1,1004,0,"操作日志");
insert into dsroleattr values(1,1004,0,"商城管理");

-- 单位信息
DROP TABLE IF EXISTS ncmuser;
CREATE TABLE ncmuser (
  userid         int        primary key,
  username       char(16)   default NULL,                     -- 门店名称
  password       char(32)   default NULL,                     -- 密码
  servicecode    char(16)   default '',                       -- 场所代码
  groupid        int(11)    default '0',                      -- 组id
  dispname       char(64)   default NULL,                     -- 显示名
  address        char(128)  default NULL,                     -- 地址
  telphone       char(64)   default NULL,                     -- 电话
  contact        char(32)   default NULL,                     -- 联系人名称
  handphone      char(32)   default NULL,                     -- 手机
  email          char(128)  default '',                       -- 邮箱
  url            char(255)  default '',                       -- 网址
  ceoname        char(30)   default '',                       -- 负责人
  postcode       char(6)    default '',                       -- 邮政编码
  servicestate   char(1)    default '9',                      -- 服务状态 1-装机开始服务 2-装机停止服务 3-未装机开始服务 4-未装机停止服务 9-其他
  joincode       char(6)    default '',                       -- 接入服务商代码
  endnum         char(6)    default '0',                      -- 门店人数  终端数
  servernum      char(2)    default '0',                      -- 服务器数量
  empnum         char(6)    default '0',                      -- 最大用户数量
  area           char(70)   default '',                       -- 地区
  areaman        char(30)   default '',                       -- 区域联系人
  areatel        char(30)   default '',                       -- 区域联系人电话
  demo           char(255)  default '',                       -- 备注
  ip             char(16)   default '',                       -- IP
  addtime        char(16)   default '',                       -- 加入时间   
  useflags       int(11)    default '0',                      -- 状态 0正常
  moditime       int(10)    unsigned default '0',             -- 修改时间
  version        char(16)   default '0',                      -- 版本号
  lasttime       char(16)   default NULL,                     -- 最后一次在线时间
  longitude      bigint(20) default '0',                      -- 经度
  latitude       bigint(20) default '0'                       -- 纬度
  fcode          char(10) default '',                         -- 厂商代码 
  dateid         int(11) default '0'                          -- 营业时间
) ;

-- 上网身份库
DROP TABLE IF EXISTS ncmenduser;
CREATE TABLE ncmenduser (
  sid              int auto_increment primary key,
  did              int(10) unsigned default NULL,         -- 门店id
  uid              int(10) unsigned default NULL,         -- 用户id 
  gid              int(10) unsigned default NULL,         -- 组id
  username         char(32) default '',                   -- 用户名
  dispname         char(32) default NULL,                 -- 显示名
  namepy           char(64) default '',                   -- 名字的拼音
  ip               int(10) unsigned default NULL,         -- ip 
  mac              char(18) default '',                   -- mac
  compname         char(32) default '',                   -- 所在店铺
  email            char(64) default '',                   -- 邮箱
  position         char(32) default '',                   -- 位置
  usebase          int(11) default '0',                   -- 用户管理方式
  syntime          int(10) unsigned default '0',          -- 信息更新时间
  lasttime         int(10) unsigned default '0',          -- 最后一次操作时间
  flag　　　　　　 int(10) unsigned default '0',          -- flag=1 重点监控对象
  country          char(3) default '',                    -- 国家代码
  idfytype         char(3) default '',                    -- 证件类型
  idfyunit         char(30) default '',                   -- 发证单位
  sex              char(1) default ''                     -- 性别
) ;

-- 组信息--对应于区域
drop table if exists ncmgroup;
create table ncmgroup (
    groupid    int    primary key,          -- 组ID
    groupname  char(32),                    -- 组名
    groupcode  char(10) default '',         -- 行政区代码
    descr      char(128),                   -- 说明
    pid        int   default 0,             -- 上级组
    upip       char(128)   default '',      -- 上传日志服务器IP 供浩易用
    upuser     char(32)   default '',       -- 上传日志登录用户名
    uppasswd   char(32) default ''
)engine=MyISAM;

create UNIQUE index ncsgroup_index on ncsgroup (groupname,pid);


drop table if exists ncfacgroup;  
create table ncfacgroup (
    fid        int unsigned auto_increment primary key,
    groupid    int  ,               -- 类别编码
    name       char(32),            -- 厂商名称
    flags      int  default 0       -- 
);

insert into ncfacgroup(groupid,name,flags) values(0,"三星",0);
insert into ncfacgroup(groupid,name,flags) values(1,"苹果",0);
insert into ncfacgroup(groupid,name,flags) values(2,"联想",0);
insert into ncfacgroup(groupid,name,flags) values(3,"宇龙",0);
insert into ncfacgroup(groupid,name,flags) values(4,"华为",0);
insert into ncfacgroup(groupid,name,flags) values(5,"中兴",0);
insert into ncfacgroup(groupid,name,flags) values(6,"小米",0);

insert into ncfacgroup(groupid,name,flags) values(0,"samsung",0);
insert into ncfacgroup(groupid,name,flags) values(1,"apple",0);
insert into ncfacgroup(groupid,name,flags) values(2,"lennovo",0);
insert into ncfacgroup(groupid,name,flags) values(3,"yulong",0);
insert into ncfacgroup(groupid,name,flags) values(4,"huawei",0);
insert into ncfacgroup(groupid,name,flags) values(5,"zte",0);
insert into ncfacgroup(groupid,name,flags) values(6,"xiaomi",0);

insert into ncfacgroup(groupid,name,flags) values(0,"中文",1);
insert into ncfacgroup(groupid,name,flags) values(1,"英文",1);
insert into ncfacgroup(groupid,name,flags) values(2,"默认",1);

insert into ncfacgroup(groupid,name,flags) values(0,"PC"  ,2);
insert into ncfacgroup(groupid,name,flags) values(1,"平板",2);
insert into ncfacgroup(groupid,name,flags) values(2,"手机",2);
insert into ncfacgroup(groupid,name,flags) values(3,"默认",2);

insert into ncfacgroup(groupid,name,flags) values(0,"没有访问"  ,4);
insert into ncfacgroup(groupid,name,flags) values(1,"已经访问过",4);

drop table if exists nctypeindex;
create table nctypeindex (
    sid        int  primary key,                -- 机型ID
    name       char(32) default ' ',            -- 机型名称
    groupid    int  default 0,                  -- 分类编码
    id         int  default 0,                  -- 原来编码
    descr      char(128) default ' ',           -- 描述
    flags      int  default 0                   -- 
);
insert into nctypeindex values(101,"glaxy",  0,101,101,"",0);
insert into nctypeindex values(102,"glaxy tab", 0,102,102,"",0);


drop table if exists ncpattern;
create table ncpattern (
    sid        int  primary key,                -- 型号ID
    name       char(32) default ' ',            -- 型号名称
    groupid    int  default 0,                  -- 分类编码
    id         int  default 0,                  -- 原来编码
    descr      char(128) default ' ',           -- 描述
    flags      int  default 0                   -- 
);



drop table if exists ncfacgroup;  
create table ncfacgroup (
    groupid    int  primary key,    -- 类别编码
    name       char(32),            -- 服务名称
    flags      int  default 0       -- 
);

-- iframe 框架
--  模板表           应用
drop table if exists nctemplate;
create table nctemplate (
    tid            int     primary key,
    did            int  unsigned,                         -- 门店ID
    gid            int  unsigned,                         -- 组ID
    mname          char(64)  ,                            -- 模板名称
  	mtype          char(32)  ,                            -- 模板类型(模板类型)
  	mtemtype       char(32)  ,                            -- 模板终端类型
    mlan           char(12)  ,                            -- 模板语言
  	platepath      char(128) ,                            -- 动态态页面存放目录
    msystime       int unsigned default 0,                -- 系统时间
    mgid           int unsigned default 0,                -- 模板组id
    mesg           varchar(64)                            -- 说明
);
alter table nctemplate add column mgid int unsigned default 0;



--  模板维护表
drop table if exists ncmodule;
create table ncmodule (
    mid             int     primary key,
    did             int  unsigned,                         -- 门店ID
    gid             int  unsigned,                         -- 组ID
    m_name          char(64)  ,                            -- 模板名称          最初模板类型
  	m_temtype       char(32)  ,                            -- 模板终端类型
  	m_lan           char(12)  ,                            -- 模板语言
    platepath       char(128) ,                            -- 动态态页面存放目录
    m_char          char(12)  ,                            -- 模板字符集
    mesg            varchar(256),                          -- 说明
    downname        varchar(256),                          -- 下载名称
    m_systime       int unsigned default 0                 -- 系统时间
);



-- 系统用户信息 涉及权限，可加字段，不可重建。
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

alter table dsuser add column company varchar(64) default '';        -- 20.21增加



drop table if exists ncmaccnf;  
create table ncmaccnf (
    aid        int unsigned auto_increment primary key,
    name       char(32),            -- AC名称
    ip         char(24),            -- AC IP
    port       char(8),             -- AC端口
    addtime    int  default 0,      -- 加入时间
    flags      int  default 0       -- 删除 更新为1
);



-- 用户信息
drop table if exists ncsrvuser;
create table if not exists ncsrvuser (
    userid             int  unsigned  primary key,     -- 根据用户名生成
    did                int  unsigned,                  -- 门店ID
    gid                int  unsigned,                  -- 组ID
    username           char(32),                       -- 用户登录名
    dispname           char(32),                       -- 显示名
    groupname          char(16),                       -- 用户组
    proname            char(16),                       -- Portal名称
    usermac            char(96),                       -- 用户MAC，多个Mac地址用逗号分隔
    password           char(32),                       -- 密码
    lcount             int unsigned default 0,         -- 访问次数
    addtime            int unsigned default 0,         -- 加入时间
    lasttime           int unsigned default 0,         -- 最后访问时间
    expiretime         int unsigned default 0,         -- 失效时间
    flags              int                             -- 0--没有访问  1--已经访问过
) max_rows=1000000000 avg_row_length=256;

create index ncsrvuser_i1 on ncsrvuser (groupname);


-- 用户和Mac地址绑定表
drop table if exists ncsrvusermac;
create table if not exists ncsrvusermac (
    usermac            char(18)  primary key,          -- 用户MAC
    username           char(32),                       -- 用户登录名
    starttime          int unsigned default 0,         -- 失效时间
    lasttime           int unsigned default 0,         -- 最后访问时间
    flags              int                             -- 当前状态 0--有效  1--无效
) max_rows=1000000000 avg_row_length=256;

create index ncsrvusermac_i1 on ncsrvusermac (username);

-- 用户上下线记录

drop   table if exists ncsrvuserlog;
create table if not exists ncsrvuserlog (
    sid         bigint auto_increment  primary key,
    portalname         char(16),                       -- Portal Name
    did                int  unsigned,                  -- 门店ID
    gid                int  unsigned,                  -- 组ID
    userid             int unsigned,
    username           char(32),                       -- 用户登录名
    dispname           char(32),
    ssid               char(32),
    apname             char(40),
    mac                char(18),
    ip                 char(16),
    dev                char(24),
    os                 char(16),
    devtype            char(8),
    ubytes             bigint,                        -- 上行流量
    dbytes             bigint,                        -- 下行流量
    starttime          int unsigned,                  -- 开始时间
    endtime            int unsigned,                  -- 终止时间
    conntime           int,
    cause              int                            -- 终止原因
) max_rows=1000000000 avg_row_length=256;

create index ncsrvuserlog_i1 on ncsrvuserlog (username);
create index ncsrvuserlog_i2 on ncsrvuserlog (starttime);




-- 用户组
drop table if exists ncsrvgroup;
create table if not exists ncsrvgroup (
    groupname          char(32) primary key,           -- 用户登录名
    did                int  unsigned,                  -- 门店ID
    dispname           char(32),                       -- 显示名
    sessiontime        int unsigned default 0,         -- 登录超时时间
    idletime           int unsigned default 0,         -- 空闲超时时间
    macbindtime        int unsigned default 0,         -- Mac地址绑定时间
    moditime           int unsigned default 0          -- 最后修改时间
);


-- 用户黑名单

drop table if exists ncsrvblackuser;
create table if not exists ncsrvblackuser (
    utype              int,                            -- 类别  1-Mac 2-Username
    did                int  unsigned,                  -- 门店ID
    gid                int  unsigned,                  -- 组ID
    name               char(32),                       -- 名称
    memo               char(64),                       -- 说明
    validtime          int unsigned default 0          -- 失效时间
);

create unique index ncsrvblackuser_i on ncsrvblackuser (utype,name);



-- 用户功能使用统计
drop table if exists ncsrvloginlog;
create table if not exists ncsrvloginlog (
    sid         bigint auto_increment  primary key,
    userid             int unsigned,
    username           char(32),
    did                int  unsigned,                  -- 门店ID
    gid               int  unsigned,                  -- 组ID
    usermac            char(18),
    ip                 char(16),
    logtime            int unsigned,
    platename          char(16),                       -- 模板名称
    fun                char(32),                       -- 功能
    dev                char(24),
    os                 char(16),
    devtype            char(8),
    useragent          char(256)                        -- Ua
) max_rows=1000000000 avg_row_length=512;;

create unique index ncsrvloginlog_i1 on ncsrvloginlog (logtime);
create unique index ncsrvloginlog_i2 on ncsrvloginlog (fun);
create unique index ncsrvloginlog_i3 on ncsrvloginlog (username);


-- 用户短信注册
drop table if exists ncsrvsmslog_if_201312;
create table if not exists ncsrvsmslog_if_201312 (
    sid                bigint auto_increment  primary key,
    did                int  unsigned,                  -- 门店ID
    gid               int  unsigned,                  -- 组ID
    userid             int unsigned,
    gid                int unsigned,
    username           char(32),
    usermac            char(18),
    ip                 char(16),
    logtime            int unsigned,
    sptype             int unsigned,                  -- 运营商类别 
    areacode           int,                           -- 区号
    areaname           char(32),                      -- 名称
    status             int                            -- 发送状态
) engine=MYISAM;


insert into ncsrvsmslog_if_201312 values(1,"50015821000067",1,1004,"13641629927","E4-98-D6-32-33-64","10.1.1.33",1388062184,1,086,"中国",0),
                                        (2,"50015321000051",2,1004,"13541629925","5C-DA-D4-D3-75-6F","10.1.1.33",1388064528,1,086,"中国",0),
                                        (3,"50010422000002",3,1004,"18939825505","14-8F-C6-78-F3-0C","10.1.1.33",1388032177,1,086,"中国",0),
                                        (4,"50023421000069",4,1004,"18621628797","E4-98-D6-32-33-64","10.1.1.33",1388042131,1,086,"中国",0),
                                        (5,"50015321000052",5,1004,"13801728925","E4-98-D6-32-33-64","10.1.1.33",1388012122,1,086,"中国",0),
                                        (6,"50023421000039",6,1004,"13801674398","E4-98-D6-32-33-64","10.1.1.33",1388032184,1,086,"中国",0),
                                        (7,"50023421000018",7,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388062124,1,086,"中国",0),
                                        (8,"50023421000121",8,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388062384,1,086,"中国",0),
                                        (9,"50023421000102",9,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388069190,1,086,"中国",0),
                                        (10,"50023421000058",10,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388262184,1,086,"中国",0),
                                        (11,"50023421000125",11,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388066124,1,086,"中国",0),
                                        (12,"50023421000037",12,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388069154,1,086,"中国",0),
                                        (13,"50023421000043",13,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388032181,1,086,"中国",0),
                                        (14,"50023421000089",14,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388061183,1,086,"中国",0),
                                        (15,"50023421000091",15,1004,"13541629925","E4-98-D6-32-33-64","10.1.1.33",1388052164,1,086,"中国",0);

------------------------------------------------------------------------------------------------

insert into radgroupreply (groupname,attribute,op,value) values ("user","Auth-Type",":=","Local");
insert into radgroupreply (groupname,attribute,op,value) values ("user","Service-Type",":=","Framed-User");
insert into radgroupreply (groupname,attribute,op,value) values ("user","Framed-IP-Address",":=","255.255.255.254");
insert into radgroupreply (groupname,attribute,op,value) values ("user","Framed-IP-Netmask",":=","255.255.255.0");


加入测试账号

insert into radcheck (username,attribute,op,value) values ("test123","User-Password",":=","test234");
insert into radcheck (username,attribute,op,value) values ("13801674398","User-Password",":=","123456");
测试账号加入组
insert into radusergroup (username,groupname) values ("test123","user");
insert into radusergroup (username,groupname) values ("13801674398","user");

grant all privileges on *.* to radius@192.168.20.38 identified by 'radius' with grant option;


insert into radgroupreply (groupname,attribute,op,value) values ("user","Auth-Type",":=","Local");
insert into radgroupreply (groupname,attribute,op,value) values ("user","Service-Type",":=","Framed-User");
insert into radgroupreply (groupname,attribute,op,value) values ('user','Framed-IP-Address',':=','255.255.255.254');
insert into radgroupreply (groupname,attribute,op,value) values ('user','Framed-IP-Netmask',':=','255.255.255.0');
insert into radgroupreply (groupname,attribute,op,value) values ('user','Session-Timeout',':=','7200');
insert into radgroupreply (groupname,attribute,op,value) values ('user','Idle-Timeout',':=','1800');

-- 商城三条线
-- 1、导航 -->楼层 -->  商标 -->商店 -->商品 -->促销
-- 2、广告商标 -->商店 --大类-- > 小类 --> 商品
-- 3、促销商标 -->商店 --> 商品 -->       

drop table if exists ncmmall;  
create table ncmmall (
    fid        int unsigned auto_increment primary key,
    groupid    int  ,                                    -- 类别编码
    name       char(32),                                 -- 名称
    imgpath    char(128),                                -- 图片路径
    detail     text,                                     -- 描述
    flags      int  default 0                            -- 
);

insert into ncmmall(groupid,name,flags) values(0,"楼层详情",0);
insert into ncmmall(groupid,name,flags) values(1,"商品分类",0);
insert into ncmmall(groupid,name,flags) values(2,"促销优惠",0);
insert into ncmmall(groupid,name,flags) values(3,"个人中心",0);

insert into ncmmall(groupid,name,flags,detail) values(0,"B2层",1,"停车场");
insert into ncmmall(groupid,name,flags,detail) values(1,"B1层",1,"食品超市");
insert into ncmmall(groupid,name,flags,detail) values(2,"一层",1,"奢侈品，化妆品，珠宝");
insert into ncmmall(groupid,name,flags,detail) values(3,"二层",1,"衣服，包包，女装");
insert into ncmmall(groupid,name,flags,detail) values(4,"三层",1,"男装，皮鞋");
insert into ncmmall(groupid,name,flags,detail) values(5,"四层",1,"电器，家具");
insert into ncmmall(groupid,name,flags,detail) values(6,"五层",1,"床上用品");
insert into ncmmall(groupid,name,flags,detail) values(7,"六层",1,"大食代，电影院");


insert into ncmmall(groupid,name,flags,) values(0,"B2层",1);
insert into ncmmall(groupid,name,flags,) values(1,"B1层",1);
insert into ncmmall(groupid,name,flags) values(2,"一层",1);
insert into ncmmall(groupid,name,flags) values(3,"二层",1);
insert into ncmmall(groupid,name,flags) values(4,"三层",1);
insert into ncmmall(groupid,name,flags) values(5,"四层",1);
insert into ncmmall(groupid,name,flags) values(6,"五层",1);
insert into ncmmall(groupid,name,flags) values(7,"六层",1);
insert into ncmmall(groupid,name,flags) values(8,"七层",1);
insert into ncmmall(groupid,name,flags) values(9,"八层",1);
insert into ncmmall(groupid,name,flags) values(10,"九层",1);
insert into ncmmall(groupid,name,flags) values(11,"十层",1);

insert into ncmmall(groupid,name,flags) values(0,"logo1",2);
insert into ncmmall(groupid,name,flags) values(1,"logo2",2);
insert into ncmmall(groupid,name,flags) values(2,"logo3",2);
insert into ncmmall(groupid,name,flags) values(3,"logo4",2);


-- 商品大类名称
drop table if exists ncmcomgroup;  
create table ncmcomgroup (
    groupid    int  primary key,           -- 类别编码
    name       char(32),                   -- 商品大类名称
    descr      char(128) default ' ',      -- 描述
    systime    int unsigned,               -- 加入时间
    imgpath    char(128),                  -- 图片路径
    flags      int  default 0              -- 
);
-- 商品小类名称
drop table if exists ncmcomtype;  
create table ncmcomtype (
    sid        int  primary key,           -- 类别编码
    name       char(32),                   -- 商品小类名称
    groupid    int  default 0,             -- 分类编码
    descr      char(128) default ' ',      -- 描述
    systime    int unsigned,               -- 加入时间
    imgpath    char(128),                  -- 图片路径
    flags      int  default 0              -- 
);


-- 商标细表
drop table if exists ncmcomindex;
create table ncmcomindex (
    sid           int  primary key,                -- 商标ID
    name          char(32) default ' ',            -- 商标名称
    did           int  unsigned,                   -- 门店ID
    gid           int  unsigned,                   -- 组ID
    sgroupid      int  default 0,                  -- 分类编码
    descr         char(128) default ' ',           -- 描述
    imgpath       char(128),                       -- 图片路径
    serno         char(32),                        -- 商标序列号
    floor         char(32),                        -- 楼层
    level         int unsigned default 0,          -- 广告优先级
    plevel        int unsigned default 0,          -- 促销优先级
    type          int unsigned default 0,          -- 1 广告 2 品牌 3促销  4视频
    systime       int unsigned,                    -- 加入时间
    shopid        int unsigned default 0,          -- 商店id
    flags         int  default 0                   -- 
);

-- 商品细表
drop table if exists ncmcomodity;
create table ncmcomodity (
    sid          int  primary key,                -- 商品ID
    name         char(32) default ' ',            -- 商品名称
    dispname     char(32) default ' ',            -- 商品显示名称
    did          int  unsigned,                   -- 门店ID
    gid          int  unsigned,                   -- 组ID
    sgroupid     int  default 0,                  -- 商标编码
    groupid      int  default 0,                  -- 分类编码
    shgroupid    int  default 0,                  -- 商店id
    imgpath      char(128),                       -- 图片路径        
    shopname     char(32) default ' ',            -- 商店名称
    shimgpath    char(128),                       -- 商店图片
    factory      char(32) default ' ',            -- 厂家
    type         char(128),                       -- 商品型号
    url          char(255),                       -- 网址
    floor        char(32),                        -- 楼层
    fldetail     char(255),                       -- 楼层介绍
    fee          float(10,2) default 0,           -- 售价
    code         char(12) default '',             -- 国内区号/国家代码
    tel          char(32) default '',             -- 电话
    address      char(255) default ' ',           -- 地址
    reserve      int unsigned,                    -- 库存
    active       char(64) default ' ',            -- 是否参与活动
    plevel       int unsigned default 0,          -- 活动优先级
    level        int unsigned default 0,          -- 广告优先级
    descr        char(255) default ' ',           -- 简述
    detail       text ,                           -- 描述
    integral     int unsigned default 0,          -- 积分
    coupon       int unsigned default 0,          -- 优惠券
    systime      int unsigned,                    -- 加入时间
    flags        int  default 0                   -- 0默认存在  1 删除   
);

-- 商店表
drop table if exists ncmshop;
create table ncmshop (
    id           int  primary key,                 -- 商店ID
    name         char(32) default ' ',             -- 商店名称
    did          int  unsigned,                    -- 门店ID
    gid          int  unsigned,                    -- 组ID
    sgroupid     int  default 0,                   -- 分类编码
    imgpath      char(128),                        -- 图片路径
    floor        char(32),                         -- 楼层
    url          char(255),                        -- 网址
    code         char(12) default '',              -- 国内区号/国家代码
    tel          char(32) default '',              -- 电话
    address      char(255) default ' ',            -- 地址
    level        int unsigned default 0,           -- 广告优先级
    plevel       int unsigned default 0,           -- 促销优先级
    type         int unsigned default 0,           -- 1 广告 2 品牌
    stype        int unsigned default 0,           -- 商店类型
    descr        char(255) default ' ',            -- 简述
    detail       text ,                            -- 描述
    systime      int unsigned,                     -- 加入时间
    flags        int  default 0                    -- 
);



-- 商城用户信息
drop table if exists ncmshopuser;
create table if not exists ncmshopuser (
    uid                int  primary key,                -- 商城用户id
    username           char(32)  primary key,           -- 用户登录名
    did                int  unsigned,                   -- 门店ID
    gid                int  unsigned,                   -- 组ID
    dispname           char(32)  default '',            -- 显示名
    groupname          char(16)  default '',            -- 用户组
    proname            char(16)  default '',            -- Portal名称
    usermac            char(18)  default '',            -- 用户MAC
    password           char(32)  default '',            -- 密码
    nickname           char(32)  default '',            -- 昵称
    membernum          char(32)  default '',            -- 会员号
    integral           int unsigned default 0,          -- 积分
    coupon             int unsigned default 0,          -- 优惠券
    address            char(128) default '',            -- 地址
    decript            text,                            -- 个人关注
    lcount             int unsigned default 0,          -- 访问次数
    addtime            int unsigned default 0,          -- 加入时间
    lasttime           int unsigned default 0,          -- 最后访问时间
    flags              int                              -- 0--没有访问  1--已经访问过
);

-- 用户消费表
drop table if exists ncmconsump;
create table if not exists ncmconsump (
    cid                int  primary key,                -- id
    username           char(32)  primary key,           -- 用户登录名
    did                int  unsigned,                   -- 门店ID
    gid                int  unsigned,                   -- 组ID
    dispname           char(32)  default '',            -- 显示名
    usermac            char(18)  default '',            -- 用户MAC
    sname              char(64)  default '',            -- 商品名称
    sid                int unsigned default 0,          -- 商品id
    fee                float(10,2) default 0,           -- 售价
    date               int unsigned default 0,          -- 消费日期
    uid                int unsigned default 0,          -- 用户id
    lasttime           int unsigned default 0           -- 最后访问时间
);
