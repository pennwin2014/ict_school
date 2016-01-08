--ICTУ԰��Ӫѧ�������ݿ����
--�û���		���ǵ����������ô�ͳ�û���
--�û���չ��		���Ӹ���Ŀ��ض��е��û���Ϣ
--�ײͱ�		����ÿ���ײ͵���ϸ��Ϣ
--�Ƽ��ײͱ�		�ƹ��ײ�չʾ��Ϣ
--�û������ײͱ�	ÿ���û������ж���������Ŀ
--�û������ײ���־��    ÿ���ײͶ����۷ѳɹ��������۷ѳɹ���������һ��������־��¼��ÿ��һ����
--��ֵ��־		ÿ�γ�ֵ�ɹ���������һ����ֵ��־ ��ÿ��һ����
--��Ϣ����		�ͻ�������Ϣ��ÿ��һ����
--��ַ��		APP��ַ��ϵ��

-- ��ǰ���û���
drop table if exists ncsrvuser;
CREATE TABLE ncsrvuser (
	userid   	int(10) unsigned NOT NULL default '0',  -- �û�ID		
	username 	char(32) default NULL,                  -- �˺�����
	dispname 	char(32) default NULL,		     	-- ��ʾ����		
	groupname 	char(32) default NULL,		     	-- ��������	
	groupid  	int(10) unsigned default '0',	     	-- ����ID	
	ssid 		char(32) default '',			-- SSID	
	proname 	char(32) default '',	
  	openid 		char(32) default '',
  	usermac 	char(96) default '',		     	-- MAC	
  	password 	char(32) default NULL,		     	-- ����	
  	mark  		char(128) default '',		     	-- ��Ʒ��ʶ(���շǾ���λ����)
  	addtime 	int(10) unsigned default '0',	     	-- ����ʱ��	
  	moditime 	int(10) unsigned default '0',	     	-- �޸�ʱ��	
  	lasttime 	int(10) unsigned default '0',	     	-- ���ʱ��	
  	expiretime 	int(10) unsigned default '0',		
  	usertype 	int(11) default '0',		     	-- �û�����	
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

-- �û���չ��
drop table if exists ncsrvuserex;
CREATE TABLE ncsrvuserex(
	id int(11) auto_increment primary key,
	username char(32) default NULL,                  -- �˺�����
  	mark char(128) default '',		     	         -- ��Ʒ��ʶ(���շǾ���λ����)
	mobno char(23) default '',                       -- �ֻ���(��ʵ����ݣ������޸�)
	password char(32) default NULL,		     	     -- ����	
	dname char(23) default '',                       -- �ǳ�
	mname char(23) default '',                       -- ��ʵ����
	gender char default 0,                           -- �Ա�  
	cid char(20) default '00000000000000000',        -- ���֤����
	studentid char(20) default '000000000000',       -- ѧ��
	college char(31) default '',			         -- ѧԺ
	department char(31) default '',                  -- �༶(ϵ��)
	photo char(127) default '/school/photo/default.png', -- ͷ��·��
	email char(63) default '',                       -- ����
	qq char(12) default '',                          -- qq��
	hobby char(127) default '',                      -- ����
	address char(127) default '',                    -- ��ַ	
	money int(11) default 0,                         -- ��ֵ���(�ҵ�Ǯ��)
	utype char,                                      -- �û����� 0 ��ͨ�û� 1 �̼� 20151228����
	shopid char(32),                                 -- �̵�id   20151228����
	authcontent char(32),                            -- ���������֤   20151228����
	authphoto char(127) default '/home/ncmysql/ncsrv/photo/default.png', -- ��ά��·��
	winpotho char(127) default '/home/ncmysql/ncsrv/photo/default.png', -- ������
	addtime int(10) unsigned default '0',            -- ����ʱ��
  	moditime int(10) unsigned default '0',           -- �޸�ʱ��	
	UNIQUE KEY ictuser(mark,username)
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- �ײͱ�
drop table if exists package;
create table package(
	id int(11) auto_increment primary key,
	mark char(128) default '',                       -- ��Ʒ��ʶ(���շǾ���λ����)
	name char(31) NOT NULL,                          -- �ײ�����
	namech char(31) NOT NULL,                          -- �ײ���������
	namedes char(127) default '',                    -- �ײ�˵��
	ptype int(11) default 1,                         -- �ײ����� 1 ���� 2 ���� 3 ���� 4  ������ 5���� 6 ����
	pdays int(11) default 0,                         -- �ײ�����
	money int(11) default 0,                         -- �ײͽ��(Ԫ)
	picture char(127) default '/school/photo/package/default.png',          -- �ײ�ͼƬ 
	colorpicture char(127) default '/school/photo/package/default1.png',    -- �ײ�ɫ�� 
	begtime int(11) default 0,                       -- �ײ���Ч����ʼʱ��
	endtime int(11) default 0,                       -- �ײ���Ч�ڽ���ʱ��
	timeval int(11) default 0,                       -- �༭ʱ��
	cuser char(31) default '',                       -- �༭�û�
    content char(127) default '',                    -- ��ע
    UNIQUE KEY nameex1(mark,name)                    -- wu
)ENGINE=MyISAM DEFAULT CHARSET=latin1;


-- �Ƽ��ײͱ�
drop table if exists recpackage;
create table recpackage(
    id int(11) not null primary key,                   -- �ײ�id
	mark char(128) default '',                         -- ��Ʒ��ʶ(���շǾ���λ����)
	name char(31) NOT NULL,                            -- �ײ�����
    useflag int unsigned default 0,                    -- �Ƿ�ʹ��(0��ʹ�� 1����ʹ��)
	timeval int(11) default 0,                         -- ����ʱ��
	cuser char(31) default '',                         -- ����Ա
	content char(127) default '',                      -- ��ע
	UNIQUE KEY uk_recpackage_name(mark,name)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


-- �û������ײͱ�
drop table if exists userorder;
create table userorder(
	id int(11) auto_increment primary key,
	username char(32) default NULL,                              -- �˺�����
	mobno char(23) default '',                                   -- ���ֻ���
	mark char(128) default '',                                   -- ��Ʒ��ʶ(���շǾ���λ����)	
	packageid int(11) NOT NULL,                                  -- �����ײ�id
	name char(31) NOT NULL,                                      -- �ײ�����		
	starttime int(11) default 0,                                 -- ��Ч��ʼʱ��
	endtime int(11) default 0,                                   -- ����ʱ��
	autoxiding char default 0,                                   -- �Զ�����  1 �Զ�
	status char default 0,                                       -- ״̬  1����Ч  2��ʧЧ ������ʹ���ڼ�����ΪʧЧ��Ҳ����ɾ����
	timeval int(11) default 0,                                   -- ����ʱ��
	KEY uk_userorder_time(starttime) 
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- �û������ײ���־��
-- drop table if exists userorderlog_201512;
-- create table userorderlog_201512(
-- drop table if exists userorderlog;
-- create table userorderlog(
drop table if exists userorderlog_201511;
create table userorderlog_201511(
	id int(11) auto_increment primary key,
	username char(32) default NULL,                      -- �˺�����
	mobno char(23) default '',                           -- ���ֻ���
	mark char(128) default '',                           -- ��Ʒ��ʶ(���շǾ���λ����)
	packageid int(11) NOT NULL,                          -- �����ײ�id
	name char(31) NOT NULL,                              -- �ײ�����
	status char default 0,                               -- 1 ���� 2 ���������ײ�����ײ�����ʱ��
	money int(11) NOT NULL,                             -- ����������ʱ��Ӧ��Ϊ�ײͽ�����ʱ����ʱΪ�ײͽ���ʱ������ײͽ������������ЧӦ�ÿ۳�									--�ϸ��ײ�ʣ�·��ã�
	mtype int(11) default 0,                                -- ��ֵ��ʽ��1 ΢�� 2 ֧���� ��
	timeval int(11) default 0,                              -- ����ʱ��
	info char(63) default '',                               -- ֧�������Ϣ
	orderno char(63) default '',                            -- ������
	ordernosucc char(63) default '',                        -- ֧���ɹ�������
	starttime int(11) default 0,                         -- ��Ч��ʼʱ��
	endtime int(11) default 0,                           -- ����ʱ��
    KEY userorderlog_l1(mark),
	KEY userorderlog_l2(timeval)				
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ��ֵ��ˮ��
drop table if exists rechargeTransdtl;
create table rechargeTransdtl(
    id int(11) auto_increment primary key,
	username char(32) default NULL,                         -- �˺�����
	mobno char(23) default '',                              -- ���ֻ���
	mark char(128) default '',                              -- ��Ʒ��ʶ(���շǾ���λ����)
	money int(11) default 0,                                -- ��Ԫ��
	mtype int(11) default 0,                                -- ��ֵ��ʽ��1 ΢�� 2 ֧���� ��
	timeval int(11) default 0,                              -- ��ֵʱ��
	info char(63) default '',                               -- ֧�������Ϣ
	orderno char(63) default '',                            -- ������
	ordernosucc char(63) default '',                        -- ֧���ɹ�������
	orderstatus int(9) default '',                            -- ����״̬ 1--��ֵ��ʼ���ɹ� 2--��ֵȷ��ʧ�� 3--��ֵȷ�ϳɹ�
	KEY rechargelog_l1(mark),
	KEY rechargelog_l2(timeval)			
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ��ֵ��־
drop table if exists rechargelog_201511;
create table rechargelog_201511(
    id int(11) auto_increment primary key,
	username char(32) default NULL,                         -- �˺�����
	mobno char(23) default '',                              -- ���ֻ���
	mark char(128) default '',                              -- ��Ʒ��ʶ(���շǾ���λ����)
	money int(11) default 0,                                -- ��Ԫ��
	mtype int(11) default 0,                                -- ��ֵ��ʽ��1 ΢�� 2 ֧���� ��
	timeval int(11) default 0,                              -- ��ֵʱ��
	info char(63) default '',                               -- ֧�������Ϣ
	orderno char(63) default '',                            -- ������
	ordernosucc char(63) default '',                        -- ֧���ɹ�������
	KEY rechargelog_l1(mark),
	KEY rechargelog_l2(timeval)			
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- �ͻ�������־
drop table if exists feedback_201511;
create table feedback_201511(
	id int(11) auto_increment primary key,                  -- ����ID
	username char(32) default NULL,                         -- �˺�����
	mobno char(23) default '',                              -- ���ֻ���
	mark char(128) default '',                              -- ��Ʒ��ʶ(���շǾ���λ����)	
	status char default 0,                                  -- ״̬  0��������  1����������
	problemid int(11) default 0,                            -- ������ID(�ύ������ʱ���ֶ�Ϊ0��
	subject char(128) default '',                           -- ���������
	content char(255) default '',                           -- �ύOR�ظ�
	timeval int(11) default 0,                              -- ����ʱ��
	cuser char(31) default '',                              -- ����Ա
	KEY feedback_l1(mark),
	KEY feedback_l2(problemid),
	KEY feedback_l3(timeval)				
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



-- APP��ַ��ϵ
drop table if exists address_201511;
create table address_201511(
	id int(11) auto_increment primary key,
	username char(32) default NULL,                     -- �˺�����
	mobno char(23) default '',                          -- ���ֻ���
	mark char(128) default '',                          -- ��Ʒ��ʶ(���շǾ���λ����)		
	address char(128) default '',                       -- ��ϵ��ַ
	name char(24) default '',                           -- ����
	mobileno char(24) default '',                       -- �ֻ���
	qqno char(24) default '',                           -- QQ��
	timeval int(11) default 0,                          -- ����ʱ��
	KEY address_l1(mark),
	KEY address_l2(username),
	KEY address_l3(timeval)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ���λ��Ϣ��
drop table if exists adManage;
create table adManage(
	id int(11) auto_increment primary key,
	adname char(32) default NULL,                     -- ���λ��
	mark char(128) default '',                          -- ��Ʒ��ʶ(���շǾ���λ����)		
	username char(24) default '',                           -- ʹ�����û���
	poststime int(11) default 0,                         -- Ͷ����ʼʱ��
	postetime int(11) default 0,                         -- Ͷ�Ž���ʱ��
    posttimetype char default 0,                         -- Ͷ��ʱ������ 0--ÿ�� 1--������ 2--��ĩ
	pricetype char default 0,                            -- �Ʒ����� 0--CPC 1--����
	price int(11) default 0,                         -- ���λ�۸�
	status char default 0,                                  -- ״̬  0������  1������
	timeval int(11) default 0,                       -- ����ʱ��
	starttime int(11) default 0,                         -- ��Ч��ʼʱ��
	endtime int(11) default 0,                           -- ����ʱ��
	descval char(255) default '',                        -- ���� 
	UNIQUE KEY uk_adManage_adname(mark,adname)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ʡ�����
drop table if exists tProvince;
create table tProvince(
	id int(11) auto_increment primary key,
	province_code char(15) not null,                     -- ʡ����
	province_name char(64) not null,                     -- ʡ�� 
	content char(128),                                   -- ע��
	UNIQUE(province_code)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


-- �д����
drop table if exists tCity;
create table tCity(id int auto_increment primary key,
	city_code char(15) not null,                         -- ���б���
	city_name char(64) not null,                         -- ������
	province_code char(15) not null,                     -- ����ʡ�ı���
	content char(128) default '',                        -- ע��
	UNIQUE (city_code)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Ժϵ��
drop table if exists tDepartment;
CREATE TABLE tDepartment (
  id int auto_increment primary key,
  depName char(128) default '',     -- ϵ������ѧԺ��   
  depCode char(16) default '',      -- ����
  depLevel char default 0,          -- �ȼ� 0��ѧԺ 1��ϵ�� 2:�༶
  pid int(11) default 0,            -- ������id
  mark char(128) default ''                              -- ��Ʒ��ʶ(���շǾ���λ����)	
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


-- ���������
drop table if exists tCommonQuestion;
create table tCommonQuestion(
	id int auto_increment primary key,                     -- ����ID
	questionTitle char(64) not null,                       -- �������
	questionContent char(255) default '',                  -- ��������
	timeval int(11) default 0,                             -- ��������ʱ��
	cuser char(31) default '',                             -- ����Ա
	mark char(128) default ''                              -- ��Ʒ��ʶ(���շǾ���λ����)	
) ENGINE=MyISAM DEFAULT CHARSET=latin1;