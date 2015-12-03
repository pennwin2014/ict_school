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
	dname char(23) default '',                       -- 昵称
	mname char(23) default '',                       -- 真实姓名
	gender char default 0,                           -- 性别  
	cid char(20) default '00000000000000000',        -- 身份证号码
	studentid char(20) default '000000000000',       -- 学号
	college char(31) default '',			         -- 学院
	department char(31) default '',                  -- 班级(系别)
	photo char(127) default '/home/ncmysql/ncsrv/photo/default.png', -- 头像路径
	email char(63) default '',                       -- 邮箱
	qq char(12) default '',                          -- qq号
	hobby char(127) default '',                      -- 爱好
	address char(127) default '',                    -- 地址	
	money int(11) default 0,                         -- 充值余额(我的钱包)
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
	namedes char(127) default '',                    -- 套餐说明
	ptype int(11) default 1,                         -- 套餐类型 1 包周 2 包月 3 包季 4  包半年 5包年 6 其他
	pdays int(11) default 0,                         -- 套餐天数
	money int(11) default 0,                         -- 套餐金额(元)
	picture char(127) default '/home/ncmysql/ncsrv/photo/package/default.png',          -- 套餐图片 
	colorpicture char(127) default '/home/ncmysql/ncsrv/photo/package/default1.png',    -- 套餐色块 
	begtime int(11) default 0,                       -- 套餐有效期起始时间
	endtime int(11) default 0,                       -- 套餐有效期结束时间
	timeval int(11) default 0,                       -- 编辑时间
	cuser char(31) default '',                       -- 编辑用户
    content char(127) default '',                    -- 备注
    UNIQUE KEY nameex1(mark,name)                    -- wu
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
    UNIQUE KEY uk_userorder_name(mark,username),
	KEY uk_userorder_time(starttime) 
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 用户订购套餐日志表
drop table if exists userorderlog_201511;
create table userorderlog_201511(
	id int(11) auto_increment primary key,
	username char(32) default NULL,                      -- 账号名称
	mobno char(23) default '',                           -- 绑定手机号
	mark char(128) default '',                           -- 产品标识(参照非经单位编码)
	packageid int(11) NOT NULL,                          -- 订购套餐id
	name char(31) NOT NULL,                              -- 套餐名称
	status char default 0,                               -- 1 订购 2 升级（低套餐向高套餐升级时候）
	memory int(11) NOT NULL,                             -- 订购金额（订购时候应该为套餐金额，升级时候有时为套餐金额；有时候低于套餐金额，如果是马上生效应该扣除									--上个套餐剩下费用）
	starttime int(11) default 0,                         -- 生效起始时间
	endtime int(11) default 0,                           -- 结束时间
	timeval int(11) default 0,                           -- 订购时间
    KEY userorderlog_l1(mark),
	KEY userorderlog_l2(timeval)				
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
drop table if exists feedback_201511;
create table feedback_201511(
	id int(11) auto_increment primary key,                  -- 问题ID
	username char(32) default NULL,                         -- 账号名称
	mobno char(23) default '',                              -- 绑定手机号
	mark char(128) default '',                              -- 产品标识(参照非经单位编码)	
	status char default 0,                                  -- 状态  0：新问题  1：后续交流
	problemid int(11) default 0,                            -- 新问题ID(提交新问题时该字段为0）
	subject char(128) default '',                           -- 新问题标题
	content char(255) default '',                           -- 提交OR回复
	timeval int(11) default 0,                              -- 操作时间
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
	price int(11) default 0,                         -- 广告位价格
	timeval int(11) default 0,                       -- 操作时间
	starttime int(11) default 0,                         -- 生效起始时间
	endtime int(11) default 0,                           -- 结束时间
	UNIQUE KEY uk_adManage_adname(mark,adname)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;




insert into package(name,mark,namedes,ptype,pdays,money,timeval,picture) values('pweek','1','7天套餐',1,7,10,unix_timestamp(now()),'/school/photo/package/pweek.png');
insert into package(name,mark,namedes,ptype,pdays,money,timeval,picture) values('pmonth','1','包月套餐',2,30,20,unix_timestamp(now()),'/school/photo/package/pmonth.png');
insert into package(name,mark,namedes,ptype,pdays,money,timeval,picture) values('pseason','1','季度套餐',3,90,30,unix_timestamp(now()),'/school/photo/package/pseason.png');
insert into package(name,mark,namedes,ptype,pdays,money,timeval,picture) values('phalfyear','1','半年套餐',4,180,40,unix_timestamp(now()),'/school/photo/package/phalfyear.png');
insert into package(name,mark,namedes,ptype,pdays,money,timeval,picture) values('pyear','1','包年套餐',5,365,50,unix_timestamp(now()),'/school/photo/package/pyear.png');
insert into package(name,mark,namedes,ptype,pdays,money,timeval,picture) values('puserdefine','1','用户自定义套餐',6,5000,50,unix_timestamp(now()),'/school/photo/package/puserdefine.png');

insert into recpackage(id, useflag, timeval) values(1, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(2, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(3, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(4, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(5, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(6, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(9, 1, unix_timestamp(now()));