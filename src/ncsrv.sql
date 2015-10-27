-- 2015/03/26 ncsrvpar 增加   limitflow int unsigned default 0,  -- 每天上网流量限制,单位MB limitband          int unsigned default 0          -- 带宽设置 单位kb
-- 2015/03/02 ncsmaclog_if_ 表增加了两个字段associcated,bssid
-- 2014/12/13 ncsrvgroupap 增加了 plate,line,ssid,mapid等字段
--            增加表 ncsmaclog_if 
-- 2014/09/03 ncsrvsmslog 增加 utype 字段
-- 2014/08/18 ncsrvgroupap 增加 model,version,mark,mac
-- 2014/08/04 portalplate  中增加字段 groupcode    By Sms
-- 2014/07/27 表 ncsrvuser 中增加 stype
--            表 ncsrvgroupap 中  address
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
    flags              int default 1                   -- 是否有效  1--有效 2--未处理 9--删除
);
create unique index ncsrvgroup_i1 on ncsrvgroup (groupname);


-- 单位和接入设备对照表
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
    gwid               char(32)   primary key,         -- 接入设备ID, 如APID 或MAC
    mac                char(64)  default ' ',          -- AP的MAC地址
    groupid            int unsigned default 0,         -- 对应的单位ID
    address            char(255) default ' ',          -- 安装地址
    model              char(16)  default ' ',          -- 型号
    version            char(16)  default ' ',
    mark               char(64)  default ' ',          -- 标签
    plate              char(32)  default ' ',
    line               char(32)  default ' ',
    mapid              char(32)  default ' ',
    ssid               char(32)  default ' ',
    apname             char(32)  default ' ',          -- AP名称
    longitude          double default 0,               -- 位置（经度）
    latitude           double default 0,               -- 位置(纬度)
    addtime            int unsigned default 0,         -- 加入时间
    lasttime           int unsigned default 0,         -- 最后使用时间
    flags              int                             -- 1 正常使用   0--未开通
) max_rows=1000000000 avg_row_length=256;

create index ncsrvgroupap_i1 on ncsrvgroupap (groupid);


-- 访问黑白名单
drop table if exists ncsrvwhite;
create table if not exists ncsrvwhite (
    pid                 int unsigned,                  -- 和ptype关联的ID,如:Groupid,
    ptype               int unsigned default 0,        -- 类别 0-全局  1-Group   2-SSID    3--Stype
    wtype               int default 0,                 -- 黑白类型     0--黑名单   1--白名单
    nametype            int default 0,                 -- 名单类别     1--访问IP   2--访问URL    3--终端MAC
    name                char(128)                      -- 名称
) max_rows=1000000000 avg_row_length=256;

create index ncsrvwhite_i1 on ncsrvwhite (ptype);


-- 参数 ncsrvpar 
drop table if exists ncsrvpar;
create table if not exists ncsrvpar (
    pid                 int unsigned,
    ptype               int unsigned default 0,        -- 类别 0-全局  1-Group   2-SSID   3--Stype
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
    macauth            int default 0,                  -- 无感知认证 1-MacServer 2-Radius
    weixinauth         int default 0,                  -- 第三方认证 1 微信认证
    maxmac             int default 5,                  -- 一个账号最多绑定Mac地址的数量
    errornum           int default 5,
    errorlock          int default 0,
    checksum           int unsigned default 0,         -- 校验位
    addtime            int unsigned default 0,         -- 加入时间
    moditime           int unsigned default 0,
    paroption          text,                           -- 参数选项 "radip=192.168.20.201:8999,radip1=192.168.21.202:8999"
    flags              int,                             -- 1 正常使用   0--未开通
    limitflow          int unsigned default 0,         -- 每天上网流量限制,单位MB
    limitband          int unsigned default 0,          -- 带宽设置 单位kb
    logincount         int unsigned default 0           -- 每天最多登录次数
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
-- alter table ncsrvuser add stype int unsigned default 0;
-- stype和username 唯一
drop table if exists ncsrvuser;
create table if not exists ncsrvuser (
    userid             int  unsigned  primary key,     -- 根据用户名生成
    username           char(32),                       -- 用户登录名
    dispname           char(32),                       -- 显示名
    groupname          char(32),                       -- 组名 = groupcode
    groupid            int unsigned default 0,         -- GroupId 
    stype              int unsigned default 0,         -- stype
    ssid               char(32) default ' ',           -- 接入SSID, 这个值若有，表示该用户只能通过指定SSID的AP登录，否则不限制
    proname            char(32) default ' ',           -- Portal名称
    openid             char(32) default ' ',           -- 第三方名称，如：会员号等
    password           char(32),                       -- 密码
    mark               char(128) default ' ',          -- 用户标签
    addtime            int unsigned default 0,         -- 加入时间
    moditime           int unsigned default 0,
    lasttime           int unsigned default 0,         -- 最后访问时间
    expiretime         int unsigned default 0,         -- 帐号失效时间,该值若为0表示用户不失效
    usertype           int default 0,                  -- 用户类别 0--固定用户  1--短消息  2--QQ  3--微信
    maxbindmac         int default 0,
    sumflow            int unsigned default 0,         -- 总流量, 单位 KB
    sumtime            int unsigned default 0,         -- 上网总时长
    sumlogin           int unsigned default 0,         -- 登录次数
    flags              int                             -- 0--有效用户  9--无效用户   2--未处理
) max_rows=1000000000 avg_row_length=256;

create index ncsrvuser_i1 on ncsrvuser (groupname,username);
create unique index ncsrvuser_i2 on ncsrvuser (stype,username);


-- alter table ncsrvuser modify usermac  char(96);
-- alter table ncsrvuser add  expiretime         int unsigned default 0;


-- 用户和Mac地址绑定表
-- alter table ncsrvusermac drop primary key;   // 将原来表中的主键删掉
drop table if exists ncsrvusermac;
create table if not exists ncsrvusermac (
    usermac            char(18),                       -- 用户MAC
    username           char(32),                       -- 用户登录名
    groupid            int unsigned default 0,         -- 增加 stype
    starttime          int unsigned default 0,         -- 开始时间
    lasttime           int unsigned default 0,
    moditime           int unsigned default 0,         -- 最后修改时间
    flags              int                             -- 当前状态  0--有效  9--无效  2--待处理
) max_rows=1000000000 avg_row_length=256;

create unique index ncsrvusermac_i1 on ncsrvusermac (groupid,usermac);






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

create unique index ncsrvblackuser_i on ncsrvblackuser (utype,groupid,name);

-- insert into ncsrvblackuser(utype,name,memo,validtime) value (1,"11:22:33:44:55:66","aaaa",0);
-- 用户功能使用统计


-- 用户上下线记录(针对在线用户)
-- alter table ncsrvuserlog drop primary key;
drop   table if exists ncsrvuserlog;
create table if not exists ncsrvuserlog (
    sid                bigint default 0,
    portalname         char(32),                       -- Portal Name
    userid             int unsigned default 0,
    groupid            int unsigned default 0,         -- 用户对应的单位(用户源单位id)
    gid                int unsigned default 0,         -- 对应于用户类别
    groupcode          char(32),                       -- 用户接入单位
    stype              char(4),                        -- 商家类别
    username           char(32),                       -- 用户登录名
    dispname           char(32),                       -- 显示名
    ssid               char(32),                       -- SSID
    apname             char(40),                       -- APID
    mac                char(18),
    ip                 char(16),
    dev                char(24),
    os                 char(16),
    bro                char(16),                      -- 浏览器
    devtype            char(8),
    utype              int unsigned default 0,        -- 用户类别
    ubytes             bigint,                        -- 上行流量
    dbytes             bigint,                        -- 下行流量
    starttime          int unsigned,                  -- 开始时间
    endtime            int unsigned,                  -- 终止时间
    conntime           int,                           -- 连接时间
    cause              int                            -- 终止原因
) max_rows=1000000000 avg_row_length=256;

create index ncsrvuserlog_i0 on ncsrvuserlog (sid);
create index ncsrvuserlog_i1 on ncsrvuserlog (username);
create index ncsrvuserlog_i2 on ncsrvuserlog (starttime);
create index ncsrvuserlog_i3 on ncsrvuserlog (groupid,username);
create index ncsrvuserlog_i4 on ncsrvuserlog (utype,username);


-- 用户上网记录（记录计费中的Stop请求)

drop   table if exists ncsrvinternetlog;
create table if not exists ncsrvinternetlog (
    sid                bigint primary key,             -- SessionID, 和 ncsrvuserlog一致
    tsid               bigint,
    portalname         char(32),                       -- Portal Name
    userid             int unsigned default 0,
    groupid            int unsigned default 0,         -- 用户对应的单位(用户源单位id)
    gid                int unsigned default 0,         -- 对应于用户类别(stype)
    groupcode          char(32),                       -- 用户接入单位
    stype              char(4),                        -- 商家类别
    username           char(32),                       -- 用户登录名
    dispname           char(32),                       -- 显示名
    ssid               char(32),                       -- SSID
    apname             char(40),                       -- APID
    mac                char(18),
    ip                 char(16),
    dev                char(24),
    os                 char(16),
    bro                char(16),                      -- 浏览器
    devtype            char(8),
    utype              int unsigned default 0,        -- 用户类别
    ubytes             bigint,                        -- 上行流量
    dbytes             bigint,                        -- 下行流量
    starttime          int unsigned,                  -- 开始时间
    endtime            int unsigned,                  -- 终止时间
    conntime           int,                           -- 连接时间
    cause              int                            -- 终止原因
) max_rows=1000000000 avg_row_length=256;


create index ncsrvinetlog_i1 on ncsrvinternetlog (username);
create index ncsrvinetlog_i2 on ncsrvinternetlog (starttime);
create index ncsrvinetlog_i3 on ncsrvinternetlog (groupid,username);
create index ncsrvinetlog_i4 on ncsrvinternetlog (utype,username);


-- 用户验证记录(记录计费中的Start请求)
drop   table if exists ncsrvauthlog;
create table if not exists ncsrvauthlog (
    sid                bigint primary key,             -- SessionID, 和 ncsrvuserlog一致
    tsid               bigint,                         -- 对应的Tsid
    portalname         char(32),                       -- Portal Name
    userid             int unsigned default 0,         -- Userid
    groupid            int unsigned default 0,         -- 用户对应的单位(用户源单位id)
    gid                int unsigned default 0,         -- 对应于用户类别(stype)
    groupcode          char(32),                       -- 用户接入单位
    username           char(32),                       -- 用户登录名
    dispname           char(32),                       -- 显示名
    ssid               char(32),                       -- SSID
    apname             char(40),                       -- APID
    mac                char(18),
    ip                 char(16),
    dev                char(24),
    os                 char(16),
    bro                char(16),                      -- 浏览器
    devtype            char(8),
    utype              int unsigned default 0,        -- 用户类别
    starttime          int unsigned,                  -- 开始时间
    flags              int                            -- 验证方式  1-- Web认证    2--Web自动认证    3--Mac认证
) max_rows=1000000000 avg_row_length=256;


create index ncsrvauthlog_i1 on ncsrvauthlog (username);
create index ncsrvauthlog_i2 on ncsrvauthlog (starttime);
create index ncsrvauthlog_i3 on ncsrvauthlog (groupid,username);
create index ncsrvauthlog_i4 on ncsrvauthlog (utype,username);



drop table if exists ncsrvloginlog;
create table if not exists ncsrvloginlog (
    sid                bigint auto_increment  primary key,   -- TSID
    userid             int unsigned,                         -- 用户ID,若登录失败，该值为0
    groupid            int unsigned default 0,
    did                int unsigned default 0,
    gid                int unsigned default 0,
    servicecode        char(32),                            -- Portal Name
    username           char(32),
    usermac            char(18),
    ip                 char(16),
    logtime            int unsigned,
    platename          char(32),                            -- 模板名称
    fun                char(32),                            -- 功能
    dev                char(24),
    os                 char(16),
    devtype            char(8),
    apmac              char(18) default ' ',                -- AP MAC
    authway            int unsigned default 0,              -- 验证方式
    tsid               bigint default 0,
    status             int  default 0
) max_rows=1000000000 avg_row_length=512;

create  index ncsrvloginlog_i0 on ncsrvloginlog (sid);
create  index ncsrvloginlog_i1 on ncsrvloginlog (logtime);
create  index ncsrvloginlog_i2 on ncsrvloginlog (fun);
create  index ncsrvloginlog_i3 on ncsrvloginlog (username);



-- 用户短信注册
-- alter table ncsrvsmslog add apmac char(18) default ' ';

drop table if exists ncsrvsmslog;
create table if not exists ncsrvsmslog (
    sid                bigint auto_increment  primary key,
    userid             int unsigned,                  -- 用户ID,暂时不用
    groupid            int unsigned default 0,
    groupcode          char(32),
    stype              char(4),
    servicecode        char(32) default ' ',          -- 场所端编码(PortalName)
    did                int unsigned default 0,
    gid                int unsigned default 0,        -- 用户类别
    username           char(32),                      -- 用户账号
    usermac            char(18),                      -- 用户Mac地址
    apmac              char(18),                      -- 接入AP MAC
    ip                 char(16),                      -- 用户IP地址
    logtime            int unsigned,                  -- 发送时间
    smsname            char(32) default ' ',          -- 发送短信平台
    sptype             int,                           -- 运营商编码
    areacode           int,                           -- 区号
    areaname           char(32),                      -- 区域名称
    contrycode         int default 0,                 -- 国家代码
    tsid               bigint default 0,
    utype              int unsigned default 0,
    feename            char(32) default ' ',          -- 计费单位
    status             int                            -- 发送状态 1-成功  0-失败
) max_rows=1000000000  avg_row_length=256;

create index ncsrvsmslog_i1 on ncsrvsmslog (logtime);
create index ncsrvsmslog_i2 on ncsrvsmslog (sptype);
create index ncsrvsmslog_i3 on ncsrvsmslog (areacode);
create index ncsrvsmslog_i4 on ncsrvsmslog (username);


drop table if exists ncadlog;
create table ncadlog (
    sid          bigint    auto_increment primary key,
    tsid         bigint    default 0,
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
    groupcode   char(32),                        -- 单位编码
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



-- 用户类别描述
-- 
-- insert into ncmautotype(nametype,name,namecode,shopcode,systime,flag) value (2,'ICPIC',    '010000','02',01010000,'太保固定用户',0,0);
-- insert into ncmautotype(nametype,name,namecode,shopcode,systime,flag) value (2,'CPICVIP',  '020000','02',01020000,'太保临时用户',0,0);
-- Stype为商家编码，在Portal中用于管理的第一级单位的编码。同一个Stype中用户信息要求唯一。系统一旦启用，Stype即确定，Stype在实施中由新网程统一分配。
-- Stype的编码规则为:
-- 前面六位为单位，第七位为用户类别,  0—一般用户 1--固定  2--手机   3--会员系统
-- 目前已分配的Stype值为:
-- 单位名称	说明	Stype值	对应特征
-- 河南太保	    河南省太保固定用户	4100011	SSID: ICPIC
--              河南省太保临时用户	4100012	SSID: CPICVIP
-- 河南泰康人寿	河南泰康人寿	    4100020	SSID: TKRS
-- 河南丹尼斯	丹尼斯临时用户	    4100032	SSID: @Dennis-Guest
--              丹尼斯临时VIP用户	4100033	SSID: @Dennis-VIP



drop table if exists ncmautotype;
create table ncmautotype (
    id          int   auto_increment  primary key,
    ssid             char(32),                                -- SSID
    groupcode        char(32),                                -- GroupCode  aaa*  *aaa* 
    ucode            char(32) default ' ',                    -- 对应的Group编码
    namecode         char(8),                                 -- 权限编码 4位一级  0001
    shopcode         char(4),                                 -- 商超编码 来自ncmshopgtype 2位
    stype            int unsigned default 0,                  -- 用户组id = shopcode * 1000000 + namecode
    descr            char(32),                                -- 描述
    systime          int default 0,                           -- 操作系统时间
    flag             int default 0                            -- 是否使用 0--使用 9--不使用
);



-- Mac统计信息
drop table if exists ncsrvmaclog;
create table ncsrvmaclog (
    pname            char(32),                                -- Portal Name
    mac              char(18),                                -- Mac地址
    apmac            char(18),                                -- AP Mac地址
    groupid          int unsigned default 0,                  -- 单位ID
    groupcode        char(32),                                -- 单位编码
    mark             char(64)     default ' ',                -- 标签
    lrssi            int          default 0,
    urssi            int          default 0,
    starttime        int unsigned default 0,                  -- 起始时间
    endtime          int unsigned default 0,                  -- 结束时间
    flag             int default 0                            -- 0--进入  1--离开
);

create index ncsrvmaclog_ind1 on ncsrvmaclog(pname);
create index ncsrvmaclog_ind2 on ncsrvmaclog(mac);
create index ncsrvmaclog_ind3 on ncsrvmaclog(apmac);
create index ncsrvmaclog_ind4 on ncsrvmaclog(groupcode);
create index ncsrvmaclog_ind5 on ncsrvmaclog(mark);


-- AP人流统计

drop table if exists ncsapmaccount_if_201503;
create table ncsapmaccount_if_201503 (
    pname            char(32),                                -- Portal Name
    servicecode char(18)      default ' ',                 -- 场所端编码
    servicename char(64)      default ' ',                 -- 场所端名称
    address     char(64)      default ' ',                 -- 安装位置
    apname      char(32),                                  -- AP名称
    mapid       char(32)      default ' ',                 -- 地图标识(若有值存在，经度维度为想，x,y坐标
    longitude   char(16)      default ' ',                 -- 经度
    latitude    char(16)      default ' ',                 -- 维度
    stime            int unsigned default 0,                  -- 起始时间
    maccount         int unsigned default 0,                   -- 记录
    KEY ncsapmaccount_ind1 (stime)
);

create index ncsapmaccount_ind1 on ncsapmaccount(stime);
create index ncsapmaccount_ind2 on ncsapmaccount(apname,stime);
create index ncsapmaccount_ind3 on ncsapmaccount(servicecode,stime);



-- MapID人流统计

drop table if exists ncsapmarkcount_if_201503;
create table ncsapmarkcount_if_201503 (
    mapid            char(32)      default ' ',               -- 地图标识(若有值存在，经度维度为想，x,y坐标
    stime            int unsigned default 0,                  -- 起始时间
    maccount         int unsigned default 0                   -- 记录
);


create index ncsapmarkcount_ind1 on ncsapmarkcount(stime);
create index ncsapmarkcount_ind2 on ncsapmarkcount(mapid,stime);


-- MAC日志信息
drop table if exists ncsmaclog_if_;
create table ncsmaclog (
    servicecode char(18)      default ' ',                 -- 场所端编码
    servicename char(64)      default ' ',                 -- 场所端名称
    address     char(64)      default ' ',                 -- 安装位置
    fromdev     char(32)      default ' ',                 -- 数据来源
    mac         char(18)      default ' ',                 -- Mac地址
    apname      char(32),                                  -- AP名称
    apmac       char(18),                                  -- APMac地址
    lrssi       int  default 0,                            -- 开始场强
    urssi       int  default 0,                            -- 离开场强
    ssid        char(64)      default ' ',                 -- SSID,若多个用逗号分隔    
    plate       char(32)      default ' ',                 -- 车辆编号
    line        char(32)      default ' ',                 -- 交通线路名称
    mapid       char(32)      default ' ',                 -- 地图标识(若有值存在，经度维度为x,y坐标
    longitude   char(16)      default ' ',                 -- 经度
    latitude    char(16)      default ' ',                 -- 维度
    sendtime         int unsigned default 0,
    starttime        int unsigned default 0,               -- 进入时间
    endtime          int unsigned default 0,               -- 离开时间
    flags            int default 0,                        -- 0--进入  1--离开
    associcated      int default 0,                        -- 是否关联
    bssid            char(18)                              -- 关联Bssid
);


create index ncsmaclog_ind1 on ncsmaclog_if_201412(servicecode);
create index ncsmaclog_ind2 on ncsmaclog_if_201412(servicecode,mac);
create index ncsmaclog_ind3 on ncsmaclog_if_201412(mac);


drop table if exists ncmbandpar;                               -- 带宽策略参数表
create table ncmbandpar (
     name       char(32)      default ' ',                     -- 名称
     value            int unsigned default 0                  -- 值
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


-- 用户认证中心

drop table if exists ncmuserinfo;                             -- 用户信息
create table ncmuserinfo (
     uid            char(32)      primary       key,
     username       char(32)      default ' ',                -- 名称
     dispname       char(32)      default ' ',                -- 显示名
     password       char(32)      default ' ',                -- 密码
     usertype       int unsigned  default 0,                  -- 0 帐号  1 手机号  2 身份证号
     addtime        int unsigned  default 0,                  -- 加入时间
     lasttime       int unsigned  default 0                   -- 最后使用时间
);

create unique index ncmuserinfo_i1 on ncmuserinfo (username);

-- 用户终端绑定信息
drop table if exists ncmuserbind;                    -- 用户绑定信息
create table ncmuserbind (
     uid_sn         char(32)      primary           key,
     username       char(32)      default ' ',      -- 用户名
     dispname       char(32)      default ' ',      -- 显示名
     app_src        char(32)      default ' ',      -- 用户来源
     sj_src         char(32)      default ' ',      -- 审计厂家 
     user_mac       char(12)      default ' ',      -- 用户MAC地址
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

-- 虚拟身份
drop table if exists ncmnetname;                    -- 虚拟身份
create table ncmnetname (
     uid_sn         char(32)      primary           key,
     username       char(32)      default ' ',      -- 用户名
     dispname       char(32)      default ' ',      -- 显示名
     app_src        char(32)      default ' ',      -- 用户来源
     sj_src         char(32)      default ' ',      -- 审计厂家 
     user_mac       char(12)      default ' ',      -- 用户MAC地址
     imei           char(16)      default ' ',
     imsi           char(16)      default ' ',
     term           char(16)      default ' ',
     os             char(32)      default ' ',
     uid            char(32)      default ' ',
     addtime        int unsigned   default 0,
     lasttime       int unsigned   default 0,
     flags          int unsigned   default 0
);


-- 虚拟身份使用日志
drop table if exists ncmnetnamelog;                    -- 虚拟身份
create table ncmnetnamelog (
     uid_sn         char(32)      primary           key,
     username       char(32)      default ' ',      -- 用户名
     dispname       char(32)      default ' ',      -- 显示名
     app_src        char(32)      default ' ',      -- 用户来源
     sj_src         char(32)      default ' ',      -- 审计厂家 
     user_mac       char(12)      default ' ',      -- 用户MAC地址
     imei           char(16)      default ' ',
     imsi           char(16)      default ' ',
     term           char(16)      default ' ',
     os             char(32)      default ' ',
     uid            char(32)      default ' ',
     addtime        int unsigned   default 0,
     lasttime       int unsigned   default 0,
     flags          int unsigned   default 0
);


-- 用户使用日志
drop table if exists ncmuserlog;                    -- 用户绑定信息
create table ncmuserlog (
     sid            bigint        primary           key,
     uid_sn         char(32)      default ' ',
     username       char(32)      default ' ',      -- 用户名
     dispname       char(32)      default ' ',      -- 显示名
     app_src        char(32)      default ' ',      -- 用户来源
     sj_src         char(32)      default ' ',      -- 审计厂家 
     user_mac       char(12)      default ' ',      -- 用户MAC地址
     user_ip        char(15)      default ' ',      -- 用户IP地址
     imei           char(16)      default ' ',
     imsi           char(16)      default ' ',
     term           char(16)      default ' ',
     os             char(32)      default ' ',
     uid            char(32)      default ' ',
     stime          int unsigned   default 0,
     etime          int unsigned   default 0,
     flags          int unsigned   default 0        -- 1 上线   2 下线   3 更新
);



