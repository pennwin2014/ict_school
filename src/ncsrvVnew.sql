-- 2014/06/26 portalprule
--    alter table portalprule add groupcode char(32) default ' '
-- 2014/06/15 ncsrvpar 中增加 macauth
-- 2014/06/04 ncsgroup中增加 pid
-- 2014/05/30 增加黑白名单  ncsrvwhite
-- 2014/04/25 修订，对某些结构进行调整
-- 2014/04/22 对数据结构进行修改
-- ncsrvuserlog
-- userid           
-- ncsrvsmslog: sptype             char(32),  
-- 2013/11/17   Table ncsrvuser   usermac 字段增加到96个字节 
--             alter table ncsrvuser modify usermac  char(96);
-- create database ncsrv;
-- grant all privileges on ncsrv.*  to ncsrv@localhost identified by 'ncsrv';

--   用户组
--   groupname+ssid 对应于Radius中的group
--
drop table if exists ncsrvgroup;
create table if not exists ncsrvgroup (
    groupid            int  unsigned   primary key,    -- 该值由服务器端生成 md5code(groupname)
//    pid                int  unsigned default 0,        -- 上级单位
    groupname          char(32),                       -- 单位编码 00 00 00 00 00 00 16位
    dispname           char(128) default ' ',          -- 显示名
    address            char(255) default ' ',          -- 地址
    smsname            char(32)  default ' ',          -- 计费单位
    mark               char(128) default ' ',          -- 标签
    memo               text,                           -- 单位描述
    longitude          double default 0,               -- 位置（经度）
    latitude           double default 0,               -- 位置(纬度)
    addtime            int unsigned default 0,         -- 加入时间
    moditime           int unsigned default 0,         -- 最后修改时间
    lasttime           int unsigned default 0,         -- 最后一次操作时间
    flags              int default 1                   -- 是否有效 0--无效  1--有效  9--删除
);
create unique index ncsrvgroup_i1 on ncsrvgroup (groupname);


-- 单位和接入设备对照表
drop table if exist ncsrvgroupap;
create table if not exists ncsrvgroupap (
    gwid               char(32)   primary key,         -- 接入设备ID, 如APID 或MAC
    groupid            int unsigned default 0,
    longitude          double default 0,               -- 位置（经度）
    latitude           double default 0,               -- 位置(纬度)
    addtime            int unsigned default 0,         -- 加入时间
    flags              int                             -- 1 正常使用   0--未开通
) max_rows=1000000000 avg_row_length=256;

create index ncsrvgroupap_i1 on ncsrvgroupap (groupid);


-- 访问黑白名单
drop table if exist ncsrvwhite;
create table if not exists ncsrvwhite (
    pid                 int unsigned,                  -- 和ptype关联的ID,如:Groupid,
    ptype               int unsigned default 0,        -- 类别 0-全局  1-Group   2-SSID 
    wtype               int default 0,                 -- 黑白类型     0--黑名单   1--白名单
    nametype            int default 0,                 -- 名单类别     1--访问IP   2--访问URL    3--终端MAC
    name                char(128)                      -- 名称
) max_rows=1000000000 avg_row_length=256;

create index ncsrvwhite_i1 on ncsrvwhite (ptype);


-- 参数 ncsrvpar 
drop table if exists ncsrvpar;
create table if not exists ncsrvpar (
    pid                 int unsigned,
    ptype               int unsigned default 0,        -- 类别 0-全局  1-Group   2-SSID
    sessiontime        int unsigned default 0,         -- 登录超时时间
    idletime           int unsigned default 0,         -- 空闲超时时间
    macbindtime        int unsigned default 0,         -- Mac地址绑定时间
    smslocalnum        int  unsigned,                  -- 国内短信限制数量
    smsinternum        int  unsigned,                  -- 国际短信限制数量
    smsusetime         char(64),                       -- 短信发送限制时间  00:00:00-12:12:12,13:14:14-20:00:00
    limittime          int unsigned default 0,         -- 每天上网时间限制
    limitunit          int default 0,                  -- 限制单位 0-天 1-周  2-月  3-年 9-总共
    passcodetime       int unsigned default 0,         -- 密码的有效时间
    servicetime        char(64),                       -- 允许上网时间  00:00:00-12:12:12,13:14:14-20:00:00 
    eng_smsmessage     char(255),                      -- 英文短信格式
    zh_smsmessage      char(255),                      -- 中文短信格式
    autoauth           int default 0,                  -- 第二次是否自动验证
    macauth            int default 0,                  -- 无感知认证
    weixinauth         int default 0,                  -- 第三方认证 1 微信认证
    maxmac             int default 5,                  -- 一个账号最多绑定Mac地址的数量
    errornum           int default 5,
    errorlock          int default 0,
    addtime            int unsigned default 0,         -- 加入时间
    moditime           int unsigned default 0,
    paroption          text,                           -- 参数选项
    flags              int                             -- 1 正常使用   0--未开通
) max_rows=1000000000 avg_row_length=256;

-- paroption 为参数选型 SSIDRoaming="Yes",GroupRoaming="Yes"
create index ncsrvpar_i1 on ncsrvpar (ptype,pid);


-- SSID属性
drop table if exists ncsrvgroupssid;
create table if not exists ncsrvgroupssid (
    sid                int unsigned  primary key,
    groupid            int unsigned default 0,
    ssid               char(32),                       -- 接入设备SSID
    addtime            int unsigned default 0,         -- 加入时间
    moditime           int unsigned default 0,         -- 最后修改时间
    flags              int                             -- 1 正常使用   0--未开通
) max_rows=1000000000 avg_row_length=256;


create unique index ncsrvgroupssid_i1 on ncsrvgroupssid (groupid,ssid);
create index ncsrvgroupssid_i2 on ncsrvgroupssid (groupid);


-- 用户信息
--  登录名唯一
--  在用户信息中，目前 groupname,ssid,proname 这两个字段不用
drop table if exists ncsrvuser;
create table if not exists ncsrvuser (
    userid             int  unsigned  primary key,     -- 根据用户名生成
    username           char(32),                       -- 用户登录名
    dispname           char(32),                       -- 显示名
    groupname          char(32),                       -- 组名
    groupid            int unsigned default 0,         -- GroupId
    ssid               char(32) default ' ',           -- 接入SSID, 这个值若有，表示该用户只能通过指定SSID的AP登录，否则不限制
    proname            char(32) default ' ',           -- Portal名称
    openid             char(32) default ' ',           -- 第三方名称，如：会员号等
//    usermac            char(96) default ' ',           -- 用户MAC，多个Mac地址用逗号分隔(不用)
    password           char(32),                       -- 密码
    mark               char(128) default ' ',          -- 用户标签
    addtime            int unsigned default 0,         -- 加入时间
    moditime           int unsigned default 0,
    lasttime           int unsigned default 0,         -- 最后访问时间
    expiretime         int unsigned default 0,         -- 帐号失效时间,该值若为0表示用户不失效
    usertype           int default 0,                  -- 用户类别 0--固定用户  1--短消息  2--QQ  3--微信
    flags              int                             -- 0--有效用户  9--无效用户
) max_rows=1000000000 avg_row_length=256;

create unique index ncsrvuser_i1 on ncsrvuser (groupid,username);
create index ncsrvuser_i2 on ncsrvuser (username);

-- alter table ncsrvuser modify usermac  char(96);
-- alter table ncsrvuser add  expiretime         int unsigned default 0;


-- 用户和Mac地址绑定表
drop table if exists ncsrvusermac;
create table if not exists ncsrvusermac (
    usermac            char(18)  primary key,          -- 用户MAC
    username           char(32),                       -- 用户登录名
    groupid            int unsigned default 0,         -- 增加
    starttime          int unsigned default 0,         -- 开始时间
    lasttime           int unsigned default 0,
    moditime           int unsigned default 0,         -- 最后修改时间
    flags              int                             -- 当前状态  0--有效  9--无效
) max_rows=1000000000 avg_row_length=256;

create index ncsrvusermac_i1 on ncsrvusermac (username);






--  insert into ncsrvgroup(groupid,groupname,dispname,smsname,ssid,sessiontime,idletime,macbindtime,smslocalnum,smsinternum,smsusetime,limittime,limitunit,passcodetime,servicetime,eng_smsmessage,zh_smsmessage,autoauth,flags)
--      value (0,'apple','apple','apple','apple-test',1800,180,604800,10000,1000,'00:00:00-23:59:59',0,0,2592000,'00:00:00-23:59:59','Your WLAN username is: [#username#]  passcode is [#password#] ','您的 WLAN 帐号是:[#username#]  密码是：[#password#]。',1,1);

-- 用户黑名单

drop table if exists ncsrvblackuser;
create table if not exists ncsrvblackuser (
    utype              int,                            -- 类别  1-Mac 2-Username
    groupid            int unsigned default 0,
    name               char(32),                       -- 名称
    memo               char(64),                       -- 说明
    validtime          int unsigned default 0          -- 失效时间
);

create unique index ncsrvblackuser_i on ncsrvblackuser (utype,name);

-- insert into ncsrvblackuser(utype,name,memo,validtime) value (1,"11:22:33:44:55:66","aaaa",0);
-- 用户功能使用统计


-- 用户上下线记录
drop   table if exists ncsrvuserlog;
create table if not exists ncsrvuserlog (
    sid                bigint primary key,
    portalname         char(16),                       -- Portal Name
    userid             int unsigned default 0,
    groupid            int unsigned default 0,
    groupcode          char(32),
    stype              char(4),                        -- 商家类别
    username           char(32),                       -- 用户登录名
    dispname           char(32),
    ssid               char(32),
    apname             char(40),
    mac                char(18),
    ip                 char(16),
    dev                char(24),
    os                 char(16),
    bro                char(16),                      -- 浏览器
    devtype            char(8),
    ubytes             bigint,                        -- 上行流量
    dbytes             bigint,                        -- 下行流量
    starttime          int unsigned,                  -- 开始时间
    endtime            int unsigned,                  -- 终止时间
    conntime           int,
    cause              int                            -- 终止原因
) max_rows=1000000000 avg_row_length=256;

create index ncsrvuserlog_i1 on ncsrvuserlog (username);
create index ncsrvuserlog_i3 on ncsrvuserlog (groupid,username);
create index ncsrvuserlog_i2 on ncsrvuserlog (starttime);

-- 用户访问记录
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
    platename          char(32),                       -- 模板名称
    fun                char(32),                       -- 功能
    dev                char(24),
    os                 char(16),
    devtype            char(8),
    tsid               bigint default 0,
    useragent          char(255)                        -- Ua
) max_rows=1000000000 avg_row_length=512;

create unique index ncsrvloginlog_i1 on ncsrvloginlog (logtime);
create unique index ncsrvloginlog_i2 on ncsrvloginlog (fun);
create unique index ncsrvloginlog_i3 on ncsrvloginlog (username);


-- 用户短信注册
drop table if exists ncsrvsmslog;
create table if not exists ncsrvsmslog (
    sid                bigint auto_increment  primary key,
    userid             int unsigned,                  -- 用户ID,暂时不用
    groupid            int unsigned default 0,
    groupcode          char(32),
    stype              char(4),
    servicecode        char(32) default ' ',          -- 场所端编码(PortalName)
    did                int unsigned default 0,
    gid                int unsigned default 0,
    username           char(32),                      -- 用户账号
    usermac            char(18),                      -- 用户Mac地址
    ip                 char(16),                      -- 用户IP地址
    logtime            int unsigned,                  -- 发送时间
    smsname            char(32) default ' ',          -- 发送短信平台
    sptype             int,                           -- 运营商编码
    areacode           int,                           -- 区号
    areaname           char(32),                      -- 区域名称
    contrycode         int default 0,                 -- 国家代码
    tsid               bigint default 0,
    feename            char(32) default ' ',          -- 计费单位
    status             int                            -- 发送状态 1-成功  0-失败
) max_rows=1000000000  avg_row_length=256;

create index ncsrvsmslog_i1 on ncsrvsmslog (logtime);
create index ncsrvsmslog_i2 on ncsrvsmslog (sptype);
create index ncsrvsmslog_i3 on ncsrvsmslog (areacode);
create index ncsrvsmslog_i4 on ncsrvsmslog (username);


drop table if exists ncadlog;
create table ncadlog(
    sid          int  unsigned  auto_increment primary key,
    refer        char(128) default '',   -- 请求url
    url          char(128) default '',   -- 广告URL
    ncname       char(36),               -- 场所名称
    username     char(32),               -- 上网帐号
    sip          char(16),               --  IP
    mac          char(20),
    adcod        char(32),               -- 广告代码
    apname       char(32),               -- ap名称
    apssid       char(32),               -- apssid
    phtype       int unsigned,           -- 上网终端 1 平板 2 PC 3 手机
    os           char(16),               -- 操作系统
    sptype       int unsigned,           -- 运营商类别   0 不合法  1 移动 2 联通 3 电信 8 其它 9 国际
    devtype      char(32),               -- 终端类型 苹果  三星  华为 
    browser      char(32),               -- 浏览器
    areacode     int,                    -- 区号
    areaname     char(32),               -- 区域名称  美国  北京  上海
    contrycode   int default 0,          -- 国家代码
    lasttime     int  unsigned,          -- 统计时间
    lcount       int unsigned            -- 1  
);
create index ncadlog2 on ncadlog(lasttime);
create index ncadlog1 on ncadlog(adcod);


drop table if exists ncsrvtest;
create table ncsrvtest(
    lasttime     int  unsigned default 0
);


-- 数据人工插入样例
-- insert into ncsrvgroup(groupid,groupname,dispname,smsname,sessiontime,idletime,macbindtime,smslocalnum,smsinternum,smsusetime,limittime,limitunit,passcodetime,servicetime,eng_smsmessage,zh_smsmessage,autoauth,flags)
--      value (1001,'pronetway','apple','apple',1800,180,604800,10000,1000,'00:00:00-23:59:59',0,0,2592000,'00:00:00-23:59:59','Your WLAN username is: [#username#]  passcode is [#password#] ','您的 WLAN 帐号是:[#username#]  密码是：[#password#]。',1,1);
--
-- insert into ncsrvgroupap(gwid,groupid,longitude,latitude,flags) value ('test38',1001,0,0,1);

--  页面描述
drop table if exists portalpage;
create table portalpage (
    pageid    int  primary key,
    pagename  char(128),                        -- 页面名称
    pagepath  char(128),                        -- 存放路径
    addtime   int unsigned,                     -- 加入时间
    modtime   int unsigned                      -- 最后修改时间
);
    

--  模板描述
drop table if exists portalplate;
create table portalplate (
    plateid     char(16)    primary key,
    shopplateid int default 0;
    platename   char(128),                       -- 模板名称
    platepath   char(128),
    addtime    int unsigned,                     -- 加入时间
    modtime    int unsigned                      -- 最后修改时间
);


-- 模板针对的页面列表
drop table if exists portalplist;
create table portalplist (
    plateid    char(16),                       -- 模板ID
    termtype   char(16),                       -- 终端类别 pc,pad,mobile
    lang       char(16),                       -- 语言, zh,eng
    pageid     int unsigned                    -- 页面ID
);

create index portalplist_ind1 on portalplist(plateid);

-- 模板投放规则
drop table if exists portalprule;
create table portalprule (
    ruleid     int  primary key,                  -- 规则ID, 
    priority   int  unsigned default 0,           -- 优先级, 数字越大优先级越高
    plateid    char(16),                          -- 模板名称
    groupid    int  unsigned default 0,           -- 单位ID,上级单位包含下级单位
    groupcode  char(32) default ' ',              -- 单位编码
    ssid       char(32),                          -- SSID
    nasid      char(32),                          -- 设备接入ID，可以为多个，可以用通配符表示  *aa*,??aa*
    mark       char(255),                         -- 标签，可以多个，每个之间用逗号分隔，如:景点,外卖
    dotime     char(255),                         -- 投放时间段如 08:00:00-11:30:00,13:00:00-16:00:00
    dateid     int  unsigned default 0,           -- 日期ID, 0 每天 其它值参见时间描述
    starttime  int  unsigned  default 0,          -- 开始时间
    endtime    int  unsigned  default 0,          -- 终止时间
    addtime    int  unsigned  default 0,          -- 记录加入时间
    modtime    int  unsigned  default 0,          -- 记录最后修改时间
    status     int  default 0                     -- 状态 0 停用   1 启用
);
    
create index portalprule_ind1 on portalprule(starttime);
create index portalprule_ind2 on portalprule(priority);

-- 位置标签
drop table if exists portallocmark;
create table portallocmark (
    sid        int  unsigned  auto_increment primary key,
    mark       char(16) default ' ',                          -- 标签
    mapid      char(16) default ' ',
    sx         double default 0,
    sy         double default 0,
    ex         double default 0,
    ey         double default 0
);
    
create index portallocmark_ind1 on portallocmark(mapid);
