drop table if exists ncmfloor;                          -- 楼层信息 
create table ncmfloor (
    fid        int unsigned auto_increment primary key,
    name       char(32),                                 -- 楼层名称 
    detail     text,                                          -- 经营描述
    cid       int unsigned  default 0                                 --  单位ID
);


drop table if exists ncmbrand;                          -- 品牌信息  
create table ncmbrand (
    bid        int unsigned auto_increment primary key,
    fid         int,                            -- 楼层id  
    name      char(32) default ' ',            -- 商标名称
    btype     int  default '',                 -- 商品类别 
    descr           char(128) default ' ',           -- 描述
    imgpath         char(128),                       -- 图片路径
    cid      int unsigned default 0,                                 --  单位ID
    shopid      int  default 0,                                 --  商店ID
    flag     int default 0,                                 -- 0 有效   1 -- 无效
   systime         int unsigned                   -- 加入时间
);

alter table ncmbrand modify cid int unsigned default 0;
-- 商品大类名称
drop table if exists ncmcomgroup;  
create table ncmcomgroup (
    groupid    int  primary key,           -- 类别编码
    name       char(32),                   -- 商品大类名称
    descr      char(128) default ' ',      -- 描述
    systime    int unsigned,               -- 加入时间
    imgpath    char(128),                  -- 图片路径
    flags      int  default 0,              -- 
    plateid     int(11)    
);

update ncmcomgroup set systime =1386847768;

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

-- 商品细表
drop table if exists ncmcomodity;
create table ncmcomodity (
    sid          int  primary key,                -- 商品ID
    name         char(32) default ' ',            -- 商品名称            书名      
    dispname     char(32) default ' ',            -- 商品显示名称        作者
    service_code char(16) ,                       -- 场所代码
    sgroupid     int  default 0,                  -- 商城ID
    groupid      int  default 0,                  -- 分类编码         小类
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
    discount     float(10,2) default 0,           -- 折扣
    code         char(12) default '',             -- 国内区号/国家代码
    tel          char(32) default '',             -- 电话
    address      char(255) default ' ',           -- 地址
    reserve      int unsigned,                    -- 库存
    aid          int unsigned default 0,          -- 活动id 
    active       char(64) default ' ',            -- 活动名称
    plevel       int unsigned default 0,          -- 活动优先级
    level        int unsigned default 0,          -- 广告优先级
    descr        char(255) default ' ',           -- 简述               出版时间
    detail       text ,                           -- 描述               简介
    integral     int unsigned default 0,          -- 积分
    coupon       int unsigned default 0,          -- 优惠券
    atype        int unsigned default 0,          -- 关注 默认0  关注 1
    systime      int unsigned,                    -- 加入时间           展示时间
    flags        int  default 0                   -- 0默认存在  1 删除   3积分商品
);

alter table ncmcomodity add img_long int unsigned default 0;  -- 图片长度
alter table ncmcomodity add img_width int unsigned default 0;  -- 图片宽度
alter table ncmcomodity add cid int unsigned default 0;  -- 增加单位ID



-- 促销优惠
drop table if exists ncmcomodity_cxyh;
create table ncmcomodity_cxyh (
    sid          int unsigned auto_increment primary key,                -- 商品ID
    name         char(32) default ' ',            -- 商品名称               
    dispname     char(32) default ' ',            -- 商品显示名称        
    gtype        int  default 0,                  -- 商品类别
    shopid    int  default 0,                     -- 商店id
    imgpath      char(128),                       -- 图片路径        
    shopname     char(32) default ' ',            -- 商店名称
    factory      char(32) default ' ',            -- 厂家
    type         char(128),                       -- 商品型号
    fee          float(10,2) default 0,           -- 售价
    discount     float(10,2) default 0,           -- 折扣
    reserve      int unsigned,                    -- 库存
    startstoptime   char(48) default '',          -- 活动开始和结束时间
    systime      int unsigned,                    -- 加入时间           展示时间
    flags        int  default 0,                   -- 0显示  1 不显示
    descr        char(128),                        -- 简述
    cid          int unsigned default 0,                    -- 单位ID
    showflag     int unsigned default 0,            --0 -一个商品一行       1-三个商品一行
    num          int unsigned defalut 0           -- 显示序号
);


-- 商店表
drop table if exists ncmshop;                      --    书展字段说明对应后面字段
create table ncmshop (
    id           int  primary key,                 -- 商店ID
    name         char(32) default ' ',             -- 商店名称            出版社
    service_code char(16) ,                        -- 场所代码
    sgroupid     int  unsigned default 0,          -- 商城ID(单位ID)
    imgpath      char(128),                        -- 图片路径
    floor        char(32),                         -- 楼层                展台
    url          char(255),                        -- 网址                人文艺术馆
    code         char(12) default '',              -- 国内区号/国家代码
    tel          char(32) default '',              -- 电话
    address      char(255) default ' ',            -- 地址                展馆   东一管
    level        int unsigned default 0,           -- 广告优先级
    plevel       int unsigned default 0,           -- 促销优先级
    type         int unsigned default 0,           -- 1 广告 2 品牌
    stype        int unsigned default 0,           -- 商店类型 
    aid          int unsigned default 0,           -- 活动id
    descr        char(255) default ' ',            -- 简述                 集团公司
    detail       text ,                            -- 描述              出版社介绍
    systime      int unsigned,                     -- 加入时间
    flags        int  default 0                    -- 
);
alter table ncmshop modify sgroupid int unsigned default 0;

-- 商品细表
-- 增加商城ID
alter table ncmcomodity modify sgroupid int unsigned default 0;
drop table if exists ncmcomodity;
create table ncmcomodity (
    sid          int  primary key,                -- 商品ID
    name         char(32) default ' ',            -- 商品名称            书名      
    dispname     char(32) default ' ',            -- 商品显示名称        作者
    service_code char(16) ,                       -- 场所代码
    sgroupid     int unsigned default 0,                  -- 商城ID
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
    discount     float(10,2) default 0,           -- 折扣
    code         char(12) default '',             -- 国内区号/国家代码
    tel          char(32) default '',             -- 电话
    address      char(255) default ' ',           -- 地址
    reserve      int unsigned,                    -- 库存
    aid          int unsigned default 0,          -- 活动id 
    active       char(64) default ' ',            -- 活动名称
    plevel       int unsigned default 0,          -- 活动优先级
    level        int unsigned default 0,          -- 广告优先级
    descr        char(255) default ' ',           -- 简述               出版时间
    detail       text ,                           -- 描述               简介
    integral     int unsigned default 0,          -- 积分
    coupon       int unsigned default 0,          -- 优惠券
    atype        int unsigned default 0,          -- 关注 默认0  关注 1
    systime      int unsigned,                    -- 加入时间           展示时间
    flags        int  default 0                   -- 0默认存在  1 删除   3积分商品
);
| plateid      | int(11)          | YES  |     | 0       |       |
| img_long     | int(10) unsigned | YES  |     | 0       |       |
| img_width    | int(10) unsigned | YES  |     | 0       |       |




-- 会员信息注册日志
drop table if exists ncmshopuserlog;
create table if not exists ncmshopuserlog (
    sid                int unsigned auto_increment primary key,                -- 会员用户id
    username           char(32) ,           -- 用户登录名
    did                int  unsigned,                   -- 单位ID
    dispname           char(32)  default '',            -- 显示名
    sex                char(8)   default '',            -- 性别
    mobile             char(18)  default '',            -- 手机号
    idno               char(20)  default '',            -- 证件号
    email              char(64)  default '',            -- 邮件地址
    stime           int unsigned default 0          -- 最后访问时间
);
