--20151228增加

--表名：											表编号
--标签表											10
--群聊表											11
--群聊成员表									12
--聊天内容表									13
--朋友圈表										14
--朋友圈内容									15
--朋友圈内容选择性开放				16
--朋友圈评论									17
--黑名单											18
--信息举报										19

-- 标签表
drop table if exists ncsrvmarkgroup;
CREATE TABLE ncsrvmarkgroup(
	id int(11) auto_increment primary key,
	mark  char(128) default '',		    -- 产品标识(参照非经单位编码)
	ictgroup char(128) default '',		    -- 标签名称
	username	char(32) default NULL,      -- 帐号
	timeval	int(11) unsigned default '0',   -- 编辑时间
	UNIQUE KEY  ncsrvmarkgroup (mark,ictgroup,username), 	
	KEY ncsrvmarkgroup_i1 (mark,username)  	
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 群聊表
drop table if exists ncsrvqun;
CREATE TABLE ncsrvqun(
	id int(11) auto_increment primary key,
	mark char(128) default '',		    -- 产品标识(参照非经单位编码)
	qunname char(128) default '',		    -- 群名称
	username char(32) default NULL,      -- 创建群帐号
	attrib int(2) unsigned default '0', -- 0 只能有创建群者维护，1 群成员可以维护	
	chattop char,												-- 置顶聊天 1 置顶
	toptime int(11),										-- 置顶时间
	avoiddisturb char,											-- 消息免打扰 1 免打扰
	addressbook char,											  -- 保存到通信录 1 保存
	nichen char(32) default NULL,      -- 在本群昵称
	nichenshow char,											  -- 显示群成员昵称 1 显示
	timeval int(11) unsigned default 0, -- 编辑时间
	UNIQUE KEY  ncsrvqun_i1(mark,qunname,username)  	
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 群聊成员表
drop table if exists ncsrvqunlist;
CREATE TABLE ncsrvqunlist(
	id int(11) auto_increment primary key,
	mark char(128) default '',		    -- 产品标识(参照非经单位编码)
	qunid int(11),										-- 群ID
	username char(32) default NULL,      -- 成员帐号	
	timeval int(11) unsigned default 0, -- 编辑时间
	UNIQUE KEY  ncsrvqunlist (mark,qunid,username)	
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 聊天内容表
drop table if exists ncsrvims;
CREATE TABLE ncsrvims(
	id int(11) auto_increment primary key,
	mark char(128) default '',		    -- 产品标识(参照非经单位编码)
	sesstionid int(11) unsigned default 0, -- 会话ID
	senduser char(32) default NULL,      -- 发送账号名称
	recvuser char(32) default NULL,      -- 接收账号名称或群ID
	content	char(128) default NULL,      -- 发送内容
	ictpath char(128) default NULL,      -- 发送图片路径
	conttype char, 	-- 内容类型，0 内容，1 要求加为朋友 2 要求加入群 3 系统信息
    qunid int(11) unsigned default 0, 	-- 群ID
	timeval int(11) unsigned default 0, -- 发送时间
	ploattype char(32) default NULL,      -- 发送平台类型描叙
	sendmac char(16) default NULL,      -- 发送平台MAC地址
	ssid char(16) default NULL,      -- 发送者连接的SSID
	sendphone char(16) default NULL,      -- 发送者手机号
	KEY ncsrvims_i1 (mark,senduser),
	KEY ncsrvims_i2 (mark,recvuser),
	KEY ncsrvims_i3 (mark,timeval)
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 准朋友表
drop table if exists ncsrvpfriend;
CREATE TABLE ncsrvpfriend(
    id int(11) auto_increment primary key,
    mark char(128) default '',                      -- 产品标识(参照非经单位编码)
    username char(32) default NULL,                 -- 帐号
    friend char(32) default NULL,                   -- 准朋友
    havnewmsg char default 0,                       -- 有新消息 1 有新消息
    recvok char default 0,                          -- 是否同意加 1 同意加入
    timeval int(11) unsigned default 0,             -- 时间  （取系统时间
    UNIQUE KEY ncsrvpfriend(mark,username,friend)
)ENGINE=MyISAM DEFAULT CHARSET=latin1;
	
-- 朋友圈表
drop table if exists ncsrvfriend;
CREATE TABLE ncsrvfriend(
	id int(11) auto_increment primary key,
	mark char(128) default '', -- 产品标识(参照非经单位编码)
	username char(32) default NULL, -- 帐号
	friend char(32) default NULL, -- 朋友帐号或群ID
	content char(48) default NULL, -- 备注
	phone char(48) default NULL, -- 电话号码
	des char(48) default NULL, -- 描述
	photo char(127) default '/home/ncmysql/ncsrv/photo/', -- 添加名片或相关图片
	headphoto char(127) default '/home/ncmysql/ncsrv/photo/', -- 头像路径
	address char(48) default NULL, -- 地区
	timevalbeg int(11) unsigned default '0', -- 发送开始时间
	timevalend int(11) unsigned default '0', -- 发送结束时间
	chatnums int(11), -- 连续发送条数	
	groupid int(11), -- 标签ID
	timeval int(11) unsigned default '0', -- 加入时间
    UNIQUE KEY ictuser (mark,username,friend),
	KEY ncsrvfriend_i1 (username,friend)
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 朋友圈内容
drop table if exists ncsrvfriendmsg;
CREATE TABLE ncsrvfriendmsg(
	id int(11) auto_increment primary key,
	mark char(128) default '',		    -- 产品标识(参照非经单位编码)
	username char(32) default NULL,      	-- 帐号
	content char(128)default NULL,      -- 发送内容
	pathtype char, -- 发送路径类型 0 图片 1 超连接
	ictpath char(128) default NULL, -- 发送图片路径或超连接
	timeval int(11) unsigned default 0, -- 发送时间
	lookauth int(2) unsigned default 0, -- 查看权限 0 公开 1 私密 2 选中的朋友可以看 3 选中的朋友不能看
	ploattype char(32) default NULL, -- 发送平台类型描叙
	KEY ncsrvfriendmsg_i1 (mark,username),
	KEY ncsrvfriendmsg_i2 (mark,timeval)	
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 朋友圈内容选择性开放
drop table if exists ncsrvfriendmsgauth;
CREATE TABLE ncsrvfriendmsgauth(
	id int(11) auto_increment primary key,
	mark char(128) default '', -- 产品标识(参照非经单位编码)
	username char(32) default NULL, -- 帐号
	msgid int(11) unsigned default '0', -- 对应的内容ID
	friend char(32) default NULL, -- 选择的朋友帐号	
	KEY ncsrvfriendmsgauth_i1 (mark,msgid,username)	
)ENGINE=MyISAM DEFAULT CHARSET=latin1;


-- 朋友圈评论
drop table if exists ncsrvfriendmsgex;
CREATE TABLE ncsrvfriendmsgex(
	id int(11) auto_increment primary key,
	mark char(128) default '',		    -- 产品标识(参照非经单位编码)
	msgid int(11) unsigned default 0, -- 朋友圈内容ID
	content char(128)default NULL,      -- 发送内容
	pathtype char, -- 发送路径类型 0 图片 1 超连接
	ictpath char(128)default NULL, -- 发送图片路径或超连接
	timeval int(11) unsigned default '0', -- 发送时间
	ploattype char(32) default NULL, -- 发送平台类型描叙
	KEY ncsrvfriendmsg_i1 (mark,msgid),
	KEY ncsrvfriendmsg_i2 (mark,timeval)
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 黑名单
drop table if exists ncsrvbkname;
CREATE TABLE ncsrvbkname(
	id int(11) auto_increment primary key,
	mark char(128) default '',		    -- 产品标识(参照非经单位编码)
	username char(32) default NULL,      	-- 帐号
	friend char(32) default NULL,      -- 朋友帐号或群ID
	timeval int(11) unsigned default 0, -- 时间	
	UNIQUE KEY ictuser (mark,username,friend)
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 信息举报
drop table if exists report;
CREATE TABLE report(
	id int(11) auto_increment primary key,
	msgid int(11) unsigned default 0, -- 信息举报内容ID
	mark char(128) default '',		    -- 产品标识(参照非经单位编码)
	username char(32) default NULL,      	-- 帐号
	friend char(32) default NULL,      	-- 被举报的朋友帐号或群ID
	content char(128)default NULL,      	-- 举报内容
	timeval int(11) unsigned default 0, -- 时间	
	KEY report_i1 (mark,msgid),
	KEY report_i2 (mark,msgid) 
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

--/////////////////////////////////////////////////////////////////////////////
--以前的用户表
drop table if exists ncsrvuser;
CREATE TABLE ncsrvuser (
	userid   	int(10) unsigned NOT NULL default '0',  --用户ID		
	username 	char(32) default NULL,                  --账号名称
	dispname 	char(32) default NULL,		     	--显示名称		
	groupname 	char(32) default NULL,		     	--部门名称	
	groupid  	int(10) unsigned default '0',	     	--部门ID	
	ssid 		char(32) default '',			--SSID	
	proname 	char(32) default '',	
  	openid 		char(32) default '',
  	usermac 	char(96) default '',		     	--MAC	
  	password 	char(32) default NULL,		     	--密码	
  	mark  		char(128) default '',		     	--产品标识(参照非经单位编码)
  	addtime 	int(10) unsigned default '0',	     	--增加时间	
  	moditime 	int(10) unsigned default '0',	     	--修改时间	
  	lasttime 	int(10) unsigned default '0',	     	--最后活动时间	
  	expiretime 	int(10) unsigned default '0',		
  	usertype 	int(11) default '0',		     	--用户类型	
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