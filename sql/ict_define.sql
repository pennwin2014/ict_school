--以前的用户表
CREATE TABLE `ncsrvuser` (
  `userid` int(10) unsigned NOT NULL default '0',
  `username` char(32) default NULL,
  `dispname` char(32) default NULL,
  `groupname` char(32) default NULL,
  `groupid` int(10) unsigned default '0',
  `ssid` char(32) default '',
  `proname` char(32) default '',
  `openid` char(32) default '',
  `usermac` char(96) default '',
  `password` char(32) default NULL,
  `mark` char(128) default '',
  `addtime` int(10) unsigned default '0',
  `moditime` int(10) unsigned default '0',
  `lasttime` int(10) unsigned default '0',
  `expiretime` int(10) unsigned default '0',
  `usertype` int(11) default '0',
  `flags` int(11) default NULL,
  `maxbindmac` int(11) default '0',
  `stype` int(10) unsigned default '0',
  `sumflow` int(10) unsigned default '0',
  `sumtime` int(10) unsigned default '0',
  `sumlogin` int(10) unsigned default '0',
  PRIMARY KEY  (`userid`),
  UNIQUE KEY `ncsrvuser_i2` (`stype`,`username`),
  KEY `ncsrvuser_i1` (`groupname`,`username`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--用户表
drop table if exists  userlib;
create table userlib(
	id int(11) auto_increment primary key,
	vname    char(31) NOT NULL,  --虚拟身份（定位使用者,一般为手机号，或者上网注册账号）
	cid      char(31) default '00000000000000000',--身份证号码
	studentid  char(31) default '000000000000',--学号
	college    char(31) default '',--学院
	department char(31) default '',--班级(系别)
	email      char(63) default '',--邮箱
	photo     char(127) default '/home/ncmysql/ncsrv/photo/default.png',--头像路径
	hobby     char(127) default '',--爱好
	qq        char(31) default '',--绑定qq号
	wechat    char(31) default '',--绑定微信号
	weibo     char(63) default '',--绑定微博
	dname    char(23) default '',  --昵称
	mname    char(23) default '',  --姓名
	gender    char default 0,    --性别  
	address    char(127) default '',--地址
	passwd    char(23) default '123456', --密码 
	mobno    char(23) default '',  --手机号
	money    int(11) default 0,    --充值余额
	packageid   int(11) default 0,   --套餐id （正在生效）  
	timeval    int(11) default 0  --套餐生效时间
);

--套餐表
drop table if exists package;
create table package(
id int(11) auto_increment primary key, 
name char(31) NOT NULL ,--套餐名称
namedes  char(127) default '',--套餐说明
ptype     int(11) default 1,--套餐类型 1 包周 2 包月 3 包季 4  包半年 5包年
							--6 其他
pdays     int(11) default 0, --套餐天数
money    int(11) default 0, --套餐金额（元）
utype     int(11) default 0, --续费类型 （1 自动续费）
picture    char(127) default '/home/ncmysql/ncsrv/photo/package/default.png',--套餐图片 
timeval    int(11) default 0,--编辑时间
cuser     char(31) default '', --编辑用户
content   char(127) default ''--备注
);
alter table package add unique key uk_package_name(name);

--用户订购套餐表
drop table if exists userpackage;
create table userpackage(
	id int(11) auto_increment primary key,
	vname char(31) NOT NULL, --用户名
	packageid int(11) NOT NULL, --套餐id
	starttime int(11) default 0,--起始时间
	endtime  int(11) default 0,--结束时间
	status char default 0 --状态  0：生效  1：失效
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--推荐套餐表
drop table if exists recpackage;
create table recpackage(
id int(11) not null primary key,--套餐id 
useflag int unsigned default 0,--是否使用(0：使用 1：不使用)
timeval    int(11) default 0,--时间
cuser     char(31) default '',--操作员
content   char(127) default ''--备注
);
create unique index uqidx_recpackage_id on recpackage(id); 
ALTER TABLE recpackage ADD CONSTRAINT fk_recpackage_id FOREIGN KEY (id) REFERENCES package(id) ON DELETE CASCADE ON UPDATE CASCADE;

--充值日志
drop table if exists rechargelog;
create table rechargelog(
	id int(11) auto_increment primary key,  
	vname char(31) default '', --虚拟身份（定位使用者,一般为手机号，或者上网注册账号）
	money int(11) default 0,--金额（元）
	mtype int(11) default 0,--充值方式（1 微信 2 支付宝 3 一卡通）
	timeval int(11) default 0, --充值时间
	orderno char(63) default ''--订单号
);

--订购套餐日志
drop table if exists orderlog;
create table orderlog(
	id int(11) auto_increment primary key,
	vname char(31) default '', --虚拟身份（定位使用者,一般为手机号，或者上网注册账号）
	packageid int(11) default 0,--套餐id
	dstatus int(11) default 0,--订购状态( 1 生效， 0无效)
	dtime int(11) default 0--订单时间
);