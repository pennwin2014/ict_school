--ICT校园运营学生端数据库设计
--用户表		考虑到兼容性启用传统用户表
--用户扩展表		增加该项目相关独有的用户信息
--套餐表		描述每个套餐的详细信息
--推荐套餐表		推广套餐展示信息
--用户订购套餐表	每个用户可以有多条订购条目
--用户订购套餐日志表    每个套餐订购扣费成功或续订扣费成功都会生成一条订购日志记录（每月一个表）
--充值日志		每次充值成功都会生成一条充值日志 （每月一个表）
--信息反馈		客户互动信息（每月一个表）
--地址库		APP地址联系库

-- 以前的用户表
drop table if exists ncsrvuser;
CREATE TABLE ncsrvuser (
	userid   	int(10) unsigned NOT NULL default '0',  -- 用户ID		
	username 	char(32) default NULL,                  -- 账号名称
	dispname 	char(32) default NULL,		     	-- 显示名称		
	groupname 	char(32) default NULL,		     	-- 部门名称	
	groupid  	int(10) unsigned default '0',	     	-- 部门ID	
	ssid 		char(32) default '',			-- SSID	
	proname 	char(32) default '',	
  	openid 		char(32) default '',
  	usermac 	char(96) default '',		     	-- MAC	
  	password 	char(32) default NULL,		     	-- 密码	
  	mark  		char(128) default '',		     	-- 产品标识(参照非经单位编码)
  	addtime 	int(10) unsigned default '0',	     	-- 增加时间	
  	moditime 	int(10) unsigned default '0',	     	-- 修改时间	
  	lasttime 	int(10) unsigned default '0',	     	-- 最后活动时间	
  	expiretime 	int(10) unsigned default '0',		
  	usertype 	int(11) default '0',		     	-- 用户类型	
  	flags 		int(11) default NULL,
  	maxbindmac 	int(11) default '0',
	stype 		int(10) unsigned default '0',
  	sumflow 	int(10) unsigned default '0',
  	sumtime 	int(10) unsigned default '0',
  	sumlogin 	int(10) unsigned default '0',
	PRIMARY KEY  (userid),
	UNIQUE KEY ictuser_i2 (mark,username),
	KEY ncsrvuser_i1 (groupname,username)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 用户扩展表
drop table if exists ncsrvuserex;
CREATE TABLE ncsrvuserex(
	id int(11) auto_increment primary key,
	username char(32) default NULL,                  -- 账号名称
  	mark char(128) default '',		     	         -- 产品标识(参照非经单位编码)
	mobno char(23) default '',                       -- 手机号(绑定实名身份，不可修改)
	password char(32) default NULL,		     	     -- 密码	
	dname char(23) default '',                       -- 昵称
	mname char(23) default '',                       -- 真实姓名
	gender char default 0,                           -- 性别  
	cid char(20) default '00000000000000000',        -- 身份证号码
	studentid char(20) default '000000000000',       -- 学号
	college char(31) default '',			         -- 学院
	department char(31) default '',                  -- 班级(系别)
	photo char(127) default '/school/photo/default.png', -- 头像路径
	email char(63) default '',                       -- 邮箱
	qq char(12) default '',                          -- qq号
	hobby char(127) default '',                      -- 爱好
	address char(127) default '',                    -- 地址	
	money int(11) default 0,                         -- 充值余额(我的钱包)
	utype char,                                      -- 用户类型 0 普通用户 1 商家 20151228增加
	shopid char(32),                                 -- 商店id   20151228增加
	authcontent char(32),                            -- 加入好友认证   20151228增加
	authphoto char(127) default '/home/ncmysql/ncsrv/photo/default.png', -- 二维码路径
	winpotho char(127) default '/home/ncmysql/ncsrv/photo/default.png', -- 相册封面
	addtime int(10) unsigned default '0',            -- 增加时间
  	moditime int(10) unsigned default '0',           -- 修改时间	
	UNIQUE KEY ictuser(mark,username)
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 套餐表
drop table if exists package;
create table package(
	id int(11) auto_increment primary key,
	mark char(128) default '',                       -- 产品标识(参照非经单位编码)
	name char(31) NOT NULL,                          -- 套餐名称
	namech char(31) NOT NULL,                          -- 套餐中文名称
	namedes char(127) default '',                    -- 套餐说明
	ptype int(11) default 1,                         -- 套餐类型 1 包周 2 包月 3 包季 4  包半年 5包年 6 其他
	pbytes int(11) default 0,                        -- 套餐带宽 单位：kb
	pdays int(11) default 0,                         -- 套餐天数
	money int(11) default 0,                         -- 套餐金额(元)
	picture char(127) default '/school/photo/package/default.png',          -- 套餐图片 
	colorpicture char(127) default '/school/photo/package/default1.png',    -- 套餐色块 
	begtime int(11) default 0,                       -- 套餐有效期起始时间
	endtime int(11) default 0,                       -- 套餐有效期结束时间
	timeval int(11) default 0,                       -- 编辑时间
	cuser char(31) default '',                       -- 编辑用户
    content char(127) default '',                    -- 备注
	discountid int(11) default 0,                    -- 优惠推广ID  20160114添加
    UNIQUE KEY nameex1(mark,name)                    -- wu
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 优惠活动表
drop table if exists discount;
create table discount(
	id int(11) not null primary key, -- 套餐id
	mark char(128) default '', -- 产品标识(参照非经单位编码)
	name char(32) default '', -- 优惠活动名称
	content char(128) default '', -- 优惠活动内容
	rate int(11) default 0, -- 优惠折扣率
	begtime int(11) default 0, -- 优惠活动有效期起始时间
	endtime int(11) default 0, -- 优惠活动有效期结束时间
	timeval int(11) default 0, -- 编辑时间
	UNIQUE KEY discount(mark,name)  
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 推荐套餐表
drop table if exists recpackage;
create table recpackage(
    id int(11) not null primary key,                   -- 套餐id
	mark char(128) default '',                         -- 产品标识(参照非经单位编码)
	name char(31) NOT NULL,                            -- 套餐名称
    useflag int unsigned default 0,                    -- 是否使用(0：使用 1：不使用)
	timeval int(11) default 0,                         -- 操作时间
	cuser char(31) default '',                         -- 操作员
	content char(127) default '',                      -- 备注
	UNIQUE KEY uk_recpackage_name(mark,name)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


-- 用户订购套餐表
drop table if exists userorder;
create table userorder(
	id int(11) auto_increment primary key,
	username char(32) default NULL,                              -- 账号名称
	mobno char(23) default '',                                   -- 绑定手机号
	mark char(128) default '',                                   -- 产品标识(参照非经单位编码)	
	packageid int(11) NOT NULL,                                  -- 订购套餐id
	name char(31) NOT NULL,                                      -- 套餐名称		
	starttime int(11) default 0,                                 -- 生效起始时间
	endtime int(11) default 0,                                   -- 结束时间
	autoxiding char default 0,                                   -- 自动续订  1 自动
	status char default 0,                                       -- 状态  1：生效  2：失效 （超过使用期即标明为失效，也可以删除）
	timeval int(11) default 0,                                   -- 订购时间
	KEY uk_userorder_time(starttime) 
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 用户订购套餐日志表
-- drop table if exists userorderlog_201512;
-- create table userorderlog_201512(
-- drop table if exists userorderlog;
-- create table userorderlog(
drop table if exists userorderlog_201511;
create table userorderlog_201511(
	id int(11) auto_increment primary key,
	username char(32) default NULL,                      -- 账号名称
	mobno char(23) default '',                           -- 绑定手机号
	mark char(128) default '',                           -- 产品标识(参照非经单位编码)
	packageid int(11) NOT NULL,                          -- 订购套餐id
	name char(31) NOT NULL,                              -- 套餐名称
	status char default 0,                               -- 1 订购 2 升级（低套餐向高套餐升级时候）
	money int(11) NOT NULL,                             -- 订购金额（订购时候应该为套餐金额，升级时候有时为套餐金额；有时候低于套餐金额，如果是马上生效应该扣除									--上个套餐剩下费用）
	mtype int(11) default 0,                                -- 充值方式（1 微信 2 支付宝 ）
	timeval int(11) default 0,                              -- 订购时间
	info char(63) default '',                               -- 支付相关信息
	orderno char(63) default '',                            -- 订单号
	ordernosucc char(63) default '',                        -- 支付成功订单号
	starttime int(11) default 0,                         -- 生效起始时间
	endtime int(11) default 0,                           -- 结束时间
    KEY userorderlog_l1(mark),
	KEY userorderlog_l2(timeval)				
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 充值流水表
drop table if exists rechargeTransdtl;
create table rechargeTransdtl(
    id int(11) auto_increment primary key,
	username char(32) default NULL,                         -- 账号名称
	mobno char(23) default '',                              -- 绑定手机号
	mark char(128) default '',                              -- 产品标识(参照非经单位编码)
	money int(11) default 0,                                -- 金额（元）
	mtype int(11) default 0,                                -- 充值方式（1 微信 2 支付宝 ）
	timeval int(11) default 0,                              -- 充值时间
	info char(63) default '',                               -- 支付相关信息
	orderno char(63) default '',                            -- 订单号
	ordernosucc char(63) default '',                        -- 支付成功订单号
	orderstatus int(9) default '',                            -- 订单状态 1--充值初始化成功 2--充值确认失败 3--充值确认成功
	KEY rechargelog_l1(mark),
	KEY rechargelog_l2(timeval)			
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 充值日志
drop table if exists rechargelog_201511;
create table rechargelog_201511(
    id int(11) auto_increment primary key,
	username char(32) default NULL,                         -- 账号名称
	mobno char(23) default '',                              -- 绑定手机号
	mark char(128) default '',                              -- 产品标识(参照非经单位编码)
	money int(11) default 0,                                -- 金额（元）
	mtype int(11) default 0,                                -- 充值方式（1 微信 2 支付宝 ）
	timeval int(11) default 0,                              -- 充值时间
	info char(63) default '',                               -- 支付相关信息
	orderno char(63) default '',                            -- 订单号
	ordernosucc char(63) default '',                        -- 支付成功订单号
	KEY rechargelog_l1(mark),
	KEY rechargelog_l2(timeval)			
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 客户互动日志
drop table if exists feedback;
create table feedback(
	id int(11) auto_increment primary key,                  -- 问题ID
	username char(32) default NULL,                         -- 账号名称
	mobno char(23) default '',                              -- 绑定手机号
	mark char(128) default '',                              -- 产品标识(参照非经单位编码)	
	status char default 0,                                  -- 状态  0：新问题  1：后续交流
	problemid int(11) default 0,                            -- 新问题ID(提交新问题时该字段为0）
	subject char(128) default '',                           -- 新问题标题
	content char(255) default '',                           -- 提交OR回复
	timeval int(11) default 0,                              -- 操作时间
	ptimeval int(11) default 0,                             -- 新问题的时间
	cuser char(31) default '',                              -- 管理员
	KEY feedback_l1(mark),
	KEY feedback_l2(problemid),
	KEY feedback_l3(timeval)				
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



-- APP地址联系
drop table if exists address_201511;
create table address_201511(
	id int(11) auto_increment primary key,
	username char(32) default NULL,                     -- 账号名称
	mobno char(23) default '',                          -- 绑定手机号
	mark char(128) default '',                          -- 产品标识(参照非经单位编码)		
	address char(128) default '',                       -- 联系地址
	name char(24) default '',                           -- 姓名
	mobileno char(24) default '',                       -- 手机号
	qqno char(24) default '',                           -- QQ号
	timeval int(11) default 0,                          -- 操作时间
	KEY address_l1(mark),
	KEY address_l2(username),
	KEY address_l3(timeval)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 广告位信息表
drop table if exists adManage;
create table adManage(
	id int(11) auto_increment primary key,
	adname char(32) default NULL,                     -- 广告位名
	mark char(128) default '',                          -- 产品标识(参照非经单位编码)		
	username char(24) default '',                           -- 使用者用户名
	poststime int(11) default 0,                         -- 投放起始时间
	postetime int(11) default 0,                         -- 投放结束时间
    posttimetype char default 0,                         -- 投放时间类型 0--每天 1--工作日 2--周末
	pricetype char default 0,                            -- 计费类型 0--CPC 1--其他
	price int(11) default 0,                         -- 广告位价格
	status char default 0,                                  -- 状态  0：启用  1：禁用
	timeval int(11) default 0,                       -- 操作时间
	starttime int(11) default 0,                         -- 生效起始时间
	endtime int(11) default 0,                           -- 结束时间
	descval char(255) default '',                        -- 描述 
	UNIQUE KEY uk_adManage_adname(mark,adname)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 省代码表
drop table if exists tProvince;
create table tProvince(
	id int(11) auto_increment primary key,
	province_code char(15) not null,                     -- 省编码
	province_name char(64) not null,                     -- 省名 
	content char(128),                                   -- 注释
	UNIQUE(province_code)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


-- 市代码表
drop table if exists tCity;
create table tCity(id int auto_increment primary key,
	city_code char(15) not null,                         -- 城市编码
	city_name char(64) not null,                         -- 城市名
	province_code char(15) not null,                     -- 所在省的编码
	content char(128) default '',                        -- 注释
	UNIQUE (city_code)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 院系表
drop table if exists tDepartment;
CREATE TABLE tDepartment (
  id int auto_increment primary key,
  depName char(128) default '',     -- 系名或者学院名   
  depCode char(16) default '',      -- 编码
  depLevel char default 0,          -- 等级 0：学院 1：系别 2:班级
  pid int(11) default 0,            -- 父级的id
  mark char(128) default ''                              -- 产品标识(参照非经单位编码)	
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

drop table if exists portalplateindex;
CREATE TABLE `portalplateindex` (
  `plateid` char(16) default NULL,
  `plateindex` char(16) default NULL,
  `adsize` char(64) default NULL,  
  `defad` char(64) default NULL,
  `defsecad` char(64) default NULL,
  `sectitle` char(128) default NULL,
  `secdesc` char(128) default NULL,
  `addtime` int(10) unsigned default NULL,
  `modtime` int(10) unsigned default NULL,
  `swidth` char(16) default '',
  `sheight` char(16) default '',
  `fwidth` char(16) default '',
  `fheight` char(16) default '',
  `ftitle` char(128) default '',
  `fdesc` char(128) default '',
  `url` char(128) default '',
  `surl` char(128) default '',
  UNIQUE KEY `portalplateindex_i1` (`plateid`,`plateindex`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


-- 常见问题表
drop table if exists tCommonQuestion;
create table tCommonQuestion(
	id int auto_increment primary key,                     -- 问题ID
	questionTitle char(64) not null,                       -- 问题标题
	questionContent char(255) default '',                  -- 问题内容
	timeval int(11) default 0,                             -- 问题增加时间
	cuser char(31) default '',                             -- 管理员
	mark char(128) default ''                              -- 产品标识(参照非经单位编码)	
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 事件表
drop table if exists ncsrvevent;
CREATE TABLE ncsrvevent(
	id int(11) auto_increment primary key,
	mark char(128) default '',               -- 产品标识(参照非经单位编码)
    username char(32) default NULL,          -- 操作帐号
	eventname char(32) default NULL,         -- 事件名称
	status int(10) unsigned default '0',     -- 事件处理状态 0 成功 1 失败
	commandstr char(128) default '',         -- 命令字符串
	commandpath char(128) default '',        -- 命令字符串如果超过127个字节，就保存到文件
	resultpath char(128) default '',         -- 保存结果
	addtime int(10) unsigned default '0',    -- 增加时间
	KEY ncsrvevent_i1(mark,addtime)
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

--- 警务通用户关系表
drop table if exists jwtuserinfo;                             -- 用户信息
create table jwtuserinfo (
     uidsn          char(32)      primary       key,
	 mark           char(128)     default ' ',                -- 产品标识(参照非经单位编码)
     username       char(32)      default ' ',                -- 名称
     dispname       char(32)      default ' ',                -- 显示名
     password       char(32)      default ' ',                -- 密码
     usertype       int unsigned  default 0,                  -- 0 帐号  1 手机号  2 身份证号
     addtime        int unsigned  default 0,                  -- 加入时间
     lasttime       int unsigned  default 0                   -- 最后使用时间
)ENGINE=MyISAM DEFAULT CHARSET=latin1;
create unique index jwtuserinfo_i1 on jwtuserinfo (username, mark);

-- 上网用户数量统计表
drop table if exists jwtuserinfo;   
CREATE TABLE ictusercount (
  sid int(10) unsigned NOT NULL AUTO_INCREMENT,
  groupcode char(32) COLLATE latin1_bin DEFAULT NULL,
  apname char(24) COLLATE latin1_bin DEFAULT NULL,
  lcount int(10) unsigned DEFAULT 0,
  sdate char(10) COLLATE latin1_bin DEFAULT NULL,
  lssid int(11) DEFAULT 0,
  luniqmac int(11) DEFAULT 0,
  luniqssid int(11) DEFAULT 0,
  PRIMARY KEY (sid),
  KEY ictusercount_ind1 (sdate)
) ENGINE=MyISAM AUTO_INCREMENT=968 DEFAULT CHARSET=latin1 COLLATE=latin1_bin;

insert into tDepartment(depName, depCode, depLevel, pid) values('人文学院', '01', 0, 0);
insert into tDepartment(depName, depCode, depLevel, pid) values('信息工程学院', '02', 0, 0);
insert into tDepartment(depName, depCode, depLevel, pid) values('中文系', '0101', 1, 1);
insert into tDepartment(depName, depCode, depLevel, pid) values('英文系', '0102', 1, 1);

insert into tDepartment(depName, depCode, depLevel, pid) values('计算机系', '0201', 1, 2);
insert into tDepartment(depName, depCode, depLevel, pid) values('电子系', '0202', 1, 2);

insert into package(name,mark,namech,namedes,ptype,pdays,money,timeval,picture) values('pweek','1','7天套餐','描述7天套餐',1,7,10,unix_timestamp(now()),'/school/photo/package/pweek.png');
insert into package(name,mark,namech,namedes,ptype,pdays,money,timeval,picture) values('pmonth','1','包月套餐','描述套餐',2,30,20,unix_timestamp(now()),'/school/photo/package/pmonth.png');
insert into package(name,mark,namech,namedes,ptype,pdays,money,timeval,picture) values('pseason','1','季度套餐','描述套餐',3,90,30,unix_timestamp(now()),'/school/photo/package/pseason.png');
insert into package(name,mark,namech,namedes,ptype,pdays,money,timeval,picture) values('phalfyear','1','半年套餐','描述套餐',4,180,40,unix_timestamp(now()),'/school/photo/package/phalfyear.png');
insert into package(name,mark,namech,namedes,ptype,pdays,money,timeval,picture) values('pyear','1','包年套餐','描述套餐',5,365,50,unix_timestamp(now()),'/school/photo/package/pyear.png');
insert into package(name,mark,namech,namedes,ptype,pdays,money,timeval,picture) values('puserdefine','1','用户自定义套餐','描述套餐',6,5000,50,unix_timestamp(now()),'/school/photo/package/puserdefine.png');

insert into recpackage(id, useflag, timeval) values(1, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(2, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(3, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(4, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(5, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(6, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(9, 1, unix_timestamp(now()));

-- 初始化省代码表
insert into tProvince(province_code, province_name, content) values('11', '北京', '首都');
insert into tProvince(province_code, province_name, content) values('12', '天津', '');
insert into tProvince(province_code, province_name, content) values('13', '河北', '');
insert into tProvince(province_code, province_name, content) values('14', '山西', '');
insert into tProvince(province_code, province_name, content) values('15', '内蒙古', '');
insert into tProvince(province_code, province_name, content) values('21', '辽宁', '');
insert into tProvince(province_code, province_name, content) values('22', '吉林', '');
insert into tProvince(province_code, province_name, content) values('23', '黑龙江', '');
insert into tProvince(province_code, province_name, content) values('31', '上海', '');
insert into tProvince(province_code, province_name, content) values('32', '江苏', '');
insert into tProvince(province_code, province_name, content) values('33', '浙江', '');
insert into tProvince(province_code, province_name, content) values('34', '安徽', '');
insert into tProvince(province_code, province_name, content) values('35', '福建', '');
insert into tProvince(province_code, province_name, content) values('36', '江西', '');
insert into tProvince(province_code, province_name, content) values('37', '山东', '');
insert into tProvince(province_code, province_name, content) values('41', '河南', '');
insert into tProvince(province_code, province_name, content) values('42', '湖北', '');
insert into tProvince(province_code, province_name, content) values('43', '湖南', '');
insert into tProvince(province_code, province_name, content) values('44', '广东', '');
insert into tProvince(province_code, province_name, content) values('45', '广西', '');
insert into tProvince(province_code, province_name, content) values('46', '海南', '');
insert into tProvince(province_code, province_name, content) values('50', '重庆', '');
insert into tProvince(province_code, province_name, content) values('51', '四川', '');
insert into tProvince(province_code, province_name, content) values('52', '贵州', '');
insert into tProvince(province_code, province_name, content) values('53', '云南', '');
insert into tProvince(province_code, province_name, content) values('54', '西藏', '');
insert into tProvince(province_code, province_name, content) values('61', '陕西', '');
insert into tProvince(province_code, province_name, content) values('62', '甘肃', '');
insert into tProvince(province_code, province_name, content) values('63', '青海', '');
insert into tProvince(province_code, province_name, content) values('64', '宁夏', '');
insert into tProvince(province_code, province_name, content) values('65', '新疆', '');
insert into tProvince(province_code, province_name, content) values('71', '台湾', '');
insert into tProvince(province_code, province_name, content) values('81', '香港', '');
insert into tProvince(province_code, province_name, content) values('82', '澳门', '');

-- 初始化城市代码表
insert into tCity(city_code, city_name, province_code, content) values(1101, '东城', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1102, '西城', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1103, '崇文', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1104, '宣武', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1105, '朝阳', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1106, '丰台', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1107, '石景山', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1108, '海淀', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1109, '门头沟', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1111, '房山', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1112, '通州', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1113, '顺义', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1121, '昌平', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1124, '大兴', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1126, '平谷', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1127, '怀柔', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1128, '密云', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1129, '延庆', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1201, '和平', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1202, '河东', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1203, '河西', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1204, '南开', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1205, '河北', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1206, '红桥', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1207, '塘沽', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1208, '汉沽', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1209, '大港', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1210, '东丽', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1211, '西青', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1212, '津南', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1213, '北辰', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1221, '宁河', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1222, '武清', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1223, '静海', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1224, '宝坻', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1225, '蓟县', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1301, '石家庄', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1302, '唐山', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1303, '秦皇岛', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1304, '邯郸', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1305, '邢台', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1306, '保定', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1307, '张家口', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1308, '承德', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1309, '沧州', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1310, '廊坊', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1311, '衡水', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1401, '太原', 14, '省会');
insert into tCity(city_code, city_name, province_code, content) values(1402, '大同', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1403, '阳泉', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1404, '长治', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1405, '晋城', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1406, '朔州', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1407, '晋中', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1408, '运城', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1409, '忻州', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1410, '临汾', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1411, '吕梁', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1501, '呼和浩特', 15, '省会');
insert into tCity(city_code, city_name, province_code, content) values(1502, '包头', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1503, '乌海', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1504, '赤峰', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1505, '通辽', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1506, '鄂尔多斯', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1507, '呼伦贝尔', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1508, '巴彦淖尔', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1509, '乌兰察布', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1522, '兴安', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1525, '锡林郭勒', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1529, '阿拉善', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(2101, '沈阳', 21, '省会');
insert into tCity(city_code, city_name, province_code, content) values(2102, '大连', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2103, '鞍山', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2104, '抚顺', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2105, '本溪', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2106, '丹东', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2107, '锦州', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2108, '营口', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2109, '阜新', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2110, '辽阳', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2111, '盘锦', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2112, '铁岭', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2113, '朝阳', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2114, '葫芦岛', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2201, '长春', 22, '省会');
insert into tCity(city_code, city_name, province_code, content) values(2202, '吉林', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2203, '四平', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2204, '辽源', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2205, '通化', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2206, '白山', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2207, '松原', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2208, '白城', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2224, '延边', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2301, '哈尔滨', 23, '省会');
insert into tCity(city_code, city_name, province_code, content) values(2302, '齐齐哈尔', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2303, '鸡西', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2304, '鹤岗', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2305, '双鸭山', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2306, '大庆', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2307, '伊春', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2308, '佳木斯', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2309, '七台河', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2310, '牡丹江', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2311, '黑河', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2312, '绥化', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2327, '大兴安岭', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(3101, '黄浦', 31, '市中心');
insert into tCity(city_code, city_name, province_code, content) values(3103, '卢湾', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3104, '徐汇', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3105, '长宁', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3106, '静安', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3107, '普陀', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3108, '闸北', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3109, '虹口', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3110, '杨浦', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3112, '闵行', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3113, '宝山', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3114, '嘉定', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3115, '浦东新区', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3116, '金山', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3117, '松江', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3125, '南汇', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3126, '奉贤', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3129, '青浦', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3129, '崇明', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3201, '南京', 32, '省会');
insert into tCity(city_code, city_name, province_code, content) values(3202, '无锡', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3203, '徐州', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3204, '常州', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3205, '苏州', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3206, '南通', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3207, '连云港', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3208, '淮安', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3209, '盐城', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3210, '盐城', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3211, '镇江', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3212, '泰州', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3213, '宿迁', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3301, '杭州', 33, '省会');
insert into tCity(city_code, city_name, province_code, content) values(3302, '宁波', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3303, '温州', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3304, '嘉兴', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3305, '湖州', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3306, '绍兴', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3307, '金华', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3308, '衢州', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3309, '舟山', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3310, '台州', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3311, '丽水', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3401, '合肥', 34, '省会');
insert into tCity(city_code, city_name, province_code, content) values(3402, '芜湖', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3403, '蚌埠', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3404, '淮南', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3405, '马鞍山', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3406, '淮北', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3407, '铜陵', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3408, '安庆', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3410, '黄山', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3411, '滁州', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3412, '阜阳', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3413, '宿州', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3414, '巢湖', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3415, '六安', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3416, '亳州', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3417, '池州', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3418, '宣城', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3501, '福州', 35, '省会');
insert into tCity(city_code, city_name, province_code, content) values(3502, '厦门', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3503, '莆田', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3504, '三明', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3505, '泉州', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3506, '漳州', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3507, '南平', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3508, '龙岩', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3509, '宁德', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3601, '南昌', 36, '省会');
insert into tCity(city_code, city_name, province_code, content) values(3602, '景德镇', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3603, '萍乡', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3604, '九江', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3605, '新余', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3606, '鹰潭', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3607, '赣州', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3608, '吉安', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3609, '宜春', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3610, '抚州', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3611, '上饶', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3701, '济南', 37, '省会');
insert into tCity(city_code, city_name, province_code, content) values(3702, '青岛', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3703, '淄博', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3704, '枣庄', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3705, '东营', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3706, '烟台', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3707, '潍坊', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3708, '济宁', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3709, '泰安', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3710, '威海', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3711, '日照', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3712, '莱芜', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3713, '临沂', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3714, '德州', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3715, '聊城', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3716, '滨州', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3717, '菏泽', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(4101, '郑州', 41, '省会');
insert into tCity(city_code, city_name, province_code, content) values(4102, '开封', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4103, '洛阳', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4104, '平顶山', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4105, '安阳', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4106, '鹤壁', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4107, '新乡', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4108, '焦作', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4109, '濮阳', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4110, '许昌', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4111, '漯河', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4112, '三门峡', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4113, '南阳', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4114, '商丘', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4115, '信阳', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4116, '周口', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4117, '驻马店', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4118, '济源', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4201, '武汉', 42, '省会');
insert into tCity(city_code, city_name, province_code, content) values(4202, '黄石', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4203, '十堰', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4205, '宜昌', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4206, '襄樊', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4207, '鄂州', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4208, '荆门', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4209, '孝感', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4210, '荆州', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4211, '黄冈', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4212, '咸宁', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4213, '随州', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4228, '恩施', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4229, '仙桃', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4230, '潜江', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4231, '天门', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4232, '神农架', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4301, '长沙', 43, '省会');
insert into tCity(city_code, city_name, province_code, content) values(4302, '株洲', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4303, '湘潭', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4304, '衡阳', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4305, '邵阳', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4306, '岳阳', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4307, '常德', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4308, '张家界', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4309, '益阳', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4310, '郴州', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4311, '永州', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4312, '怀化', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4313, '娄底', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4331, '湘西', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4401, '广州', 44, '省会');
insert into tCity(city_code, city_name, province_code, content) values(4402, '韶关', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4403, '深圳', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4404, '珠海', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4405, '汕头', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4406, '佛山', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4407, '江门', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4408, '湛江', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4409, '茂名', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4412, '肇庆', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4413, '惠州', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4414, '梅州', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4415, '汕尾', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4416, '河源', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4417, '阳江', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4418, '清远', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4419, '东莞', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4420, '中山', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4451, '潮州', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4452, '揭阳', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4453, '云浮', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4501, '南宁', 45, '省会');
insert into tCity(city_code, city_name, province_code, content) values(4502, '柳州', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4503, '桂林', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4504, '梧州', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4505, '北海', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4506, '防城港', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4507, '钦州', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4508, '贵港', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4509, '玉林', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4510, '百色', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4511, '贺州', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4512, '河池', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4513, '来宾', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4514, '崇左', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4601, '海口', 46, '省会');
insert into tCity(city_code, city_name, province_code, content) values(4602, '三亚', 46, '');
insert into tCity(city_code, city_name, province_code, content) values(4603, '五指山', 46, '');
insert into tCity(city_code, city_name, province_code, content) values(4604, '琼海', 46, '');
insert into tCity(city_code, city_name, province_code, content) values(4605, '儋州', 46, '');
insert into tCity(city_code, city_name, province_code, content) values(4606, '文昌', 46, '');
insert into tCity(city_code, city_name, province_code, content) values(4607, '万宁', 46, '');
insert into tCity(city_code, city_name, province_code, content) values(4608, '东方', 46, '');
insert into tCity(city_code, city_name, province_code, content) values(5001, '万州', 50, '省会');
insert into tCity(city_code, city_name, province_code, content) values(5002, '涪陵', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5003, '渝中', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5004, '大渡口', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5005, '江北', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5006, '沙坪坝', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5007, '九龙坡', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5008, '南岸', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5009, '北碚', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5010, '万盛', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5011, '双桥', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5012, '渝北', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5013, '巴南', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5021, '长寿', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5022, '綦江', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5023, '潼南', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5024, '铜梁', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5025, '大足', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5026, '荣昌', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5027, '璧山', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5028, '梁平', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5029, '城口', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5030, '丰都', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5031, '垫江', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5032, '武隆', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5033, '忠县', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5034, '开县', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5035, '云阳', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5036, '奉节', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5037, '巫山', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5038, '巫溪', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5039, '黔江', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5040, '石柱', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5041, '秀山', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5042, '酉阳', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5043, '彭水', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5081, '江津', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5082, '合川', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5083, '永川', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5084, '南川', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5101, '成都', 51, '省会');
insert into tCity(city_code, city_name, province_code, content) values(5103, '自贡', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5104, '攀枝花', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5105, '泸州', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5106, '德阳', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5107, '绵阳', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5108, '广元', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5109, '遂宁', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5110, '内江', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5111, '乐山', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5113, '南充', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5114, '眉山', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5115, '宜宾', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5116, '广安', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5117, '达州', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5118, '雅安', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5119, '巴中', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5120, '资阳', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5132, '阿坝', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5133, '甘孜', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5134, '凉山', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5201, '贵阳', 52, '省会');
insert into tCity(city_code, city_name, province_code, content) values(5202, '六盘水', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5203, '遵义', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5204, '安顺', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5222, '铜仁', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5223, '黔西南', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5224, '毕节', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5226, '黔东南', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5227, '黔南', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5301, '昆明', 53, '省会');
insert into tCity(city_code, city_name, province_code, content) values(5303, '曲靖', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5304, '玉溪', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5305, '保山', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5306, '昭通', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5307, '丽江', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5308, '思茅', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5309, '临沧', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5323, '楚雄', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5325, '红河', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5326, '文山', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5328, '西双版纳', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5329, '大理', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5331, '德宏', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5333, '怒江傈', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5334, '迪庆', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5401, '拉萨', 54, '省会');
insert into tCity(city_code, city_name, province_code, content) values(5421, '昌都', 54, '');
insert into tCity(city_code, city_name, province_code, content) values(5422, '山南', 54, '');
insert into tCity(city_code, city_name, province_code, content) values(5423, '日喀则', 54, '');
insert into tCity(city_code, city_name, province_code, content) values(5424, '那曲', 54, '');
insert into tCity(city_code, city_name, province_code, content) values(5425, '阿里', 54, '');
insert into tCity(city_code, city_name, province_code, content) values(5426, '林芝', 54, '');
insert into tCity(city_code, city_name, province_code, content) values(6101, '西安', 61, '省会');