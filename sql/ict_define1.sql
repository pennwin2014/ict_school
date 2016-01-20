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
	pbytes int(11) default 0,                        -- �ײʹ��� ��λ��kb
	pdays int(11) default 0,                         -- �ײ�����
	money int(11) default 0,                         -- �ײͽ��(Ԫ)
	picture char(127) default '/school/photo/package/default.png',          -- �ײ�ͼƬ 
	colorpicture char(127) default '/school/photo/package/default1.png',    -- �ײ�ɫ�� 
	begtime int(11) default 0,                       -- �ײ���Ч����ʼʱ��
	endtime int(11) default 0,                       -- �ײ���Ч�ڽ���ʱ��
	timeval int(11) default 0,                       -- �༭ʱ��
	cuser char(31) default '',                       -- �༭�û�
    content char(127) default '',                    -- ��ע
	discountid int(11) default 0,                    -- �Ż��ƹ�ID  20160114���
    UNIQUE KEY nameex1(mark,name)                    -- wu
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- �Żݻ��
drop table if exists discount;
create table discount(
	id int(11) not null primary key, -- �ײ�id
	mark char(128) default '', -- ��Ʒ��ʶ(���շǾ���λ����)
	name char(32) default '', -- �Żݻ����
	content char(128) default '', -- �Żݻ����
	rate int(11) default 0, -- �Ż��ۿ���
	begtime int(11) default 0, -- �Żݻ��Ч����ʼʱ��
	endtime int(11) default 0, -- �Żݻ��Ч�ڽ���ʱ��
	timeval int(11) default 0, -- �༭ʱ��
	UNIQUE KEY discount(mark,name)  
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
drop table if exists feedback;
create table feedback(
	id int(11) auto_increment primary key,                  -- ����ID
	username char(32) default NULL,                         -- �˺�����
	mobno char(23) default '',                              -- ���ֻ���
	mark char(128) default '',                              -- ��Ʒ��ʶ(���շǾ���λ����)	
	status char default 0,                                  -- ״̬  0��������  1����������
	problemid int(11) default 0,                            -- ������ID(�ύ������ʱ���ֶ�Ϊ0��
	subject char(128) default '',                           -- ���������
	content char(255) default '',                           -- �ύOR�ظ�
	timeval int(11) default 0,                              -- ����ʱ��
	ptimeval int(11) default 0,                             -- �������ʱ��
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

-- �¼���
drop table if exists ncsrvevent;
CREATE TABLE ncsrvevent(
	id int(11) auto_increment primary key,
	mark char(128) default '',               -- ��Ʒ��ʶ(���շǾ���λ����)
    username char(32) default NULL,          -- �����ʺ�
	eventname char(32) default NULL,         -- �¼�����
	status int(10) unsigned default '0',     -- �¼�����״̬ 0 �ɹ� 1 ʧ��
	commandstr char(128) default '',         -- �����ַ���
	commandpath char(128) default '',        -- �����ַ����������127���ֽڣ��ͱ��浽�ļ�
	resultpath char(128) default '',         -- ������
	addtime int(10) unsigned default '0',    -- ����ʱ��
	KEY ncsrvevent_i1(mark,addtime)
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

--- ����ͨ�û���ϵ��
drop table if exists jwtuserinfo;                             -- �û���Ϣ
create table jwtuserinfo (
     uidsn          char(32)      primary       key,
	 mark           char(128)     default ' ',                -- ��Ʒ��ʶ(���շǾ���λ����)
     username       char(32)      default ' ',                -- ����
     dispname       char(32)      default ' ',                -- ��ʾ��
     password       char(32)      default ' ',                -- ����
     usertype       int unsigned  default 0,                  -- 0 �ʺ�  1 �ֻ���  2 ���֤��
     addtime        int unsigned  default 0,                  -- ����ʱ��
     lasttime       int unsigned  default 0                   -- ���ʹ��ʱ��
)ENGINE=MyISAM DEFAULT CHARSET=latin1;
create unique index jwtuserinfo_i1 on jwtuserinfo (username, mark);

-- �����û�����ͳ�Ʊ�
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

insert into tDepartment(depName, depCode, depLevel, pid) values('����ѧԺ', '01', 0, 0);
insert into tDepartment(depName, depCode, depLevel, pid) values('��Ϣ����ѧԺ', '02', 0, 0);
insert into tDepartment(depName, depCode, depLevel, pid) values('����ϵ', '0101', 1, 1);
insert into tDepartment(depName, depCode, depLevel, pid) values('Ӣ��ϵ', '0102', 1, 1);

insert into tDepartment(depName, depCode, depLevel, pid) values('�����ϵ', '0201', 1, 2);
insert into tDepartment(depName, depCode, depLevel, pid) values('����ϵ', '0202', 1, 2);

insert into package(name,mark,namech,namedes,ptype,pdays,money,timeval,picture) values('pweek','1','7���ײ�','����7���ײ�',1,7,10,unix_timestamp(now()),'/school/photo/package/pweek.png');
insert into package(name,mark,namech,namedes,ptype,pdays,money,timeval,picture) values('pmonth','1','�����ײ�','�����ײ�',2,30,20,unix_timestamp(now()),'/school/photo/package/pmonth.png');
insert into package(name,mark,namech,namedes,ptype,pdays,money,timeval,picture) values('pseason','1','�����ײ�','�����ײ�',3,90,30,unix_timestamp(now()),'/school/photo/package/pseason.png');
insert into package(name,mark,namech,namedes,ptype,pdays,money,timeval,picture) values('phalfyear','1','�����ײ�','�����ײ�',4,180,40,unix_timestamp(now()),'/school/photo/package/phalfyear.png');
insert into package(name,mark,namech,namedes,ptype,pdays,money,timeval,picture) values('pyear','1','�����ײ�','�����ײ�',5,365,50,unix_timestamp(now()),'/school/photo/package/pyear.png');
insert into package(name,mark,namech,namedes,ptype,pdays,money,timeval,picture) values('puserdefine','1','�û��Զ����ײ�','�����ײ�',6,5000,50,unix_timestamp(now()),'/school/photo/package/puserdefine.png');

insert into recpackage(id, useflag, timeval) values(1, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(2, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(3, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(4, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(5, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(6, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(9, 1, unix_timestamp(now()));

-- ��ʼ��ʡ�����
insert into tProvince(province_code, province_name, content) values('11', '����', '�׶�');
insert into tProvince(province_code, province_name, content) values('12', '���', '');
insert into tProvince(province_code, province_name, content) values('13', '�ӱ�', '');
insert into tProvince(province_code, province_name, content) values('14', 'ɽ��', '');
insert into tProvince(province_code, province_name, content) values('15', '���ɹ�', '');
insert into tProvince(province_code, province_name, content) values('21', '����', '');
insert into tProvince(province_code, province_name, content) values('22', '����', '');
insert into tProvince(province_code, province_name, content) values('23', '������', '');
insert into tProvince(province_code, province_name, content) values('31', '�Ϻ�', '');
insert into tProvince(province_code, province_name, content) values('32', '����', '');
insert into tProvince(province_code, province_name, content) values('33', '�㽭', '');
insert into tProvince(province_code, province_name, content) values('34', '����', '');
insert into tProvince(province_code, province_name, content) values('35', '����', '');
insert into tProvince(province_code, province_name, content) values('36', '����', '');
insert into tProvince(province_code, province_name, content) values('37', 'ɽ��', '');
insert into tProvince(province_code, province_name, content) values('41', '����', '');
insert into tProvince(province_code, province_name, content) values('42', '����', '');
insert into tProvince(province_code, province_name, content) values('43', '����', '');
insert into tProvince(province_code, province_name, content) values('44', '�㶫', '');
insert into tProvince(province_code, province_name, content) values('45', '����', '');
insert into tProvince(province_code, province_name, content) values('46', '����', '');
insert into tProvince(province_code, province_name, content) values('50', '����', '');
insert into tProvince(province_code, province_name, content) values('51', '�Ĵ�', '');
insert into tProvince(province_code, province_name, content) values('52', '����', '');
insert into tProvince(province_code, province_name, content) values('53', '����', '');
insert into tProvince(province_code, province_name, content) values('54', '����', '');
insert into tProvince(province_code, province_name, content) values('61', '����', '');
insert into tProvince(province_code, province_name, content) values('62', '����', '');
insert into tProvince(province_code, province_name, content) values('63', '�ຣ', '');
insert into tProvince(province_code, province_name, content) values('64', '����', '');
insert into tProvince(province_code, province_name, content) values('65', '�½�', '');
insert into tProvince(province_code, province_name, content) values('71', '̨��', '');
insert into tProvince(province_code, province_name, content) values('81', '���', '');
insert into tProvince(province_code, province_name, content) values('82', '����', '');

-- ��ʼ�����д����
insert into tCity(city_code, city_name, province_code, content) values(1101, '����', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1102, '����', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1103, '����', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1104, '����', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1105, '����', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1106, '��̨', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1107, 'ʯ��ɽ', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1108, '����', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1109, '��ͷ��', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1111, '��ɽ', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1112, 'ͨ��', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1113, '˳��', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1121, '��ƽ', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1124, '����', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1126, 'ƽ��', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1127, '����', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1128, '����', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1129, '����', 11, '');
insert into tCity(city_code, city_name, province_code, content) values(1201, '��ƽ', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1202, '�Ӷ�', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1203, '����', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1204, '�Ͽ�', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1205, '�ӱ�', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1206, '����', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1207, '����', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1208, '����', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1209, '���', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1210, '����', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1211, '����', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1212, '����', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1213, '����', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1221, '����', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1222, '����', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1223, '����', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1224, '����', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1225, '����', 12, '');
insert into tCity(city_code, city_name, province_code, content) values(1301, 'ʯ��ׯ', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1302, '��ɽ', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1303, '�ػʵ�', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1304, '����', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1305, '��̨', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1306, '����', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1307, '�żҿ�', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1308, '�е�', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1309, '����', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1310, '�ȷ�', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1311, '��ˮ', 13, '');
insert into tCity(city_code, city_name, province_code, content) values(1401, '̫ԭ', 14, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(1402, '��ͬ', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1403, '��Ȫ', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1404, '����', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1405, '����', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1406, '˷��', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1407, '����', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1408, '�˳�', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1409, '����', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1410, '�ٷ�', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1411, '����', 14, '');
insert into tCity(city_code, city_name, province_code, content) values(1501, '���ͺ���', 15, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(1502, '��ͷ', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1503, '�ں�', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1504, '���', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1505, 'ͨ��', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1506, '������˹', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1507, '���ױ���', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1508, '�����׶�', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1509, '�����첼', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1522, '�˰�', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1525, '���ֹ���', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(1529, '������', 15, '');
insert into tCity(city_code, city_name, province_code, content) values(2101, '����', 21, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(2102, '����', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2103, '��ɽ', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2104, '��˳', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2105, '��Ϫ', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2106, '����', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2107, '����', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2108, 'Ӫ��', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2109, '����', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2110, '����', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2111, '�̽�', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2112, '����', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2113, '����', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2114, '��«��', 21, '');
insert into tCity(city_code, city_name, province_code, content) values(2201, '����', 22, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(2202, '����', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2203, '��ƽ', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2204, '��Դ', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2205, 'ͨ��', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2206, '��ɽ', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2207, '��ԭ', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2208, '�׳�', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2224, '�ӱ�', 22, '');
insert into tCity(city_code, city_name, province_code, content) values(2301, '������', 23, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(2302, '�������', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2303, '����', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2304, '�׸�', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2305, '˫Ѽɽ', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2306, '����', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2307, '����', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2308, '��ľ˹', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2309, '��̨��', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2310, 'ĵ����', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2311, '�ں�', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2312, '�绯', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(2327, '���˰���', 23, '');
insert into tCity(city_code, city_name, province_code, content) values(3101, '����', 31, '������');
insert into tCity(city_code, city_name, province_code, content) values(3103, '¬��', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3104, '���', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3105, '����', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3106, '����', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3107, '����', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3108, 'բ��', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3109, '���', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3110, '����', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3112, '����', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3113, '��ɽ', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3114, '�ζ�', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3115, '�ֶ�����', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3116, '��ɽ', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3117, '�ɽ�', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3125, '�ϻ�', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3126, '����', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3129, '����', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3129, '����', 31, '');
insert into tCity(city_code, city_name, province_code, content) values(3201, '�Ͼ�', 32, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(3202, '����', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3203, '����', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3204, '����', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3205, '����', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3206, '��ͨ', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3207, '���Ƹ�', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3208, '����', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3209, '�γ�', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3210, '�γ�', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3211, '��', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3212, '̩��', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3213, '��Ǩ', 32, '');
insert into tCity(city_code, city_name, province_code, content) values(3301, '����', 33, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(3302, '����', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3303, '����', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3304, '����', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3305, '����', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3306, '����', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3307, '��', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3308, '����', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3309, '��ɽ', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3310, '̨��', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3311, '��ˮ', 33, '');
insert into tCity(city_code, city_name, province_code, content) values(3401, '�Ϸ�', 34, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(3402, '�ߺ�', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3403, '����', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3404, '����', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3405, '��ɽ', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3406, '����', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3407, 'ͭ��', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3408, '����', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3410, '��ɽ', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3411, '����', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3412, '����', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3413, '����', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3414, '����', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3415, '����', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3416, '����', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3417, '����', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3418, '����', 34, '');
insert into tCity(city_code, city_name, province_code, content) values(3501, '����', 35, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(3502, '����', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3503, '����', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3504, '����', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3505, 'Ȫ��', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3506, '����', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3507, '��ƽ', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3508, '����', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3509, '����', 35, '');
insert into tCity(city_code, city_name, province_code, content) values(3601, '�ϲ�', 36, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(3602, '������', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3603, 'Ƽ��', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3604, '�Ž�', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3605, '����', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3606, 'ӥ̶', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3607, '����', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3608, '����', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3609, '�˴�', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3610, '����', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3611, '����', 36, '');
insert into tCity(city_code, city_name, province_code, content) values(3701, '����', 37, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(3702, '�ൺ', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3703, '�Ͳ�', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3704, '��ׯ', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3705, '��Ӫ', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3706, '��̨', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3707, 'Ϋ��', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3708, '����', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3709, '̩��', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3710, '����', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3711, '����', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3712, '����', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3713, '����', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3714, '����', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3715, '�ĳ�', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3716, '����', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(3717, '����', 37, '');
insert into tCity(city_code, city_name, province_code, content) values(4101, '֣��', 41, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(4102, '����', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4103, '����', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4104, 'ƽ��ɽ', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4105, '����', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4106, '�ױ�', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4107, '����', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4108, '����', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4109, '���', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4110, '���', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4111, '���', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4112, '����Ͽ', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4113, '����', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4114, '����', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4115, '����', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4116, '�ܿ�', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4117, 'פ���', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4118, '��Դ', 41, '');
insert into tCity(city_code, city_name, province_code, content) values(4201, '�人', 42, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(4202, '��ʯ', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4203, 'ʮ��', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4205, '�˲�', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4206, '�差', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4207, '����', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4208, '����', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4209, 'Т��', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4210, '����', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4211, '�Ƹ�', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4212, '����', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4213, '����', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4228, '��ʩ', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4229, '����', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4230, 'Ǳ��', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4231, '����', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4232, '��ũ��', 42, '');
insert into tCity(city_code, city_name, province_code, content) values(4301, '��ɳ', 43, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(4302, '����', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4303, '��̶', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4304, '����', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4305, '����', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4306, '����', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4307, '����', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4308, '�żҽ�', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4309, '����', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4310, '����', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4311, '����', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4312, '����', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4313, '¦��', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4331, '����', 43, '');
insert into tCity(city_code, city_name, province_code, content) values(4401, '����', 44, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(4402, '�ع�', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4403, '����', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4404, '�麣', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4405, '��ͷ', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4406, '��ɽ', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4407, '����', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4408, 'տ��', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4409, 'ï��', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4412, '����', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4413, '����', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4414, '÷��', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4415, '��β', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4416, '��Դ', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4417, '����', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4418, '��Զ', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4419, '��ݸ', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4420, '��ɽ', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4451, '����', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4452, '����', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4453, '�Ƹ�', 44, '');
insert into tCity(city_code, city_name, province_code, content) values(4501, '����', 45, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(4502, '����', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4503, '����', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4504, '����', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4505, '����', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4506, '���Ǹ�', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4507, '����', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4508, '���', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4509, '����', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4510, '��ɫ', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4511, '����', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4512, '�ӳ�', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4513, '����', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4514, '����', 45, '');
insert into tCity(city_code, city_name, province_code, content) values(4601, '����', 46, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(4602, '����', 46, '');
insert into tCity(city_code, city_name, province_code, content) values(4603, '��ָɽ', 46, '');
insert into tCity(city_code, city_name, province_code, content) values(4604, '��', 46, '');
insert into tCity(city_code, city_name, province_code, content) values(4605, '����', 46, '');
insert into tCity(city_code, city_name, province_code, content) values(4606, '�Ĳ�', 46, '');
insert into tCity(city_code, city_name, province_code, content) values(4607, '����', 46, '');
insert into tCity(city_code, city_name, province_code, content) values(4608, '����', 46, '');
insert into tCity(city_code, city_name, province_code, content) values(5001, '����', 50, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(5002, '����', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5003, '����', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5004, '��ɿ�', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5005, '����', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5006, 'ɳƺ��', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5007, '������', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5008, '�ϰ�', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5009, '����', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5010, '��ʢ', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5011, '˫��', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5012, '�山', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5013, '����', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5021, '����', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5022, '�뽭', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5023, '����', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5024, 'ͭ��', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5025, '����', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5026, '�ٲ�', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5027, '�ɽ', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5028, '��ƽ', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5029, '�ǿ�', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5030, '�ᶼ', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5031, '�潭', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5032, '��¡', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5033, '����', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5034, '����', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5035, '����', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5036, '���', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5037, '��ɽ', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5038, '��Ϫ', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5039, 'ǭ��', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5040, 'ʯ��', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5041, '��ɽ', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5042, '����', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5043, '��ˮ', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5081, '����', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5082, '�ϴ�', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5083, '����', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5084, '�ϴ�', 50, '');
insert into tCity(city_code, city_name, province_code, content) values(5101, '�ɶ�', 51, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(5103, '�Թ�', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5104, '��֦��', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5105, '����', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5106, '����', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5107, '����', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5108, '��Ԫ', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5109, '����', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5110, '�ڽ�', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5111, '��ɽ', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5113, '�ϳ�', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5114, 'üɽ', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5115, '�˱�', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5116, '�㰲', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5117, '����', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5118, '�Ű�', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5119, '����', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5120, '����', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5132, '����', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5133, '����', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5134, '��ɽ', 51, '');
insert into tCity(city_code, city_name, province_code, content) values(5201, '����', 52, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(5202, '����ˮ', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5203, '����', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5204, '��˳', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5222, 'ͭ��', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5223, 'ǭ����', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5224, '�Ͻ�', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5226, 'ǭ����', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5227, 'ǭ��', 52, '');
insert into tCity(city_code, city_name, province_code, content) values(5301, '����', 53, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(5303, '����', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5304, '��Ϫ', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5305, '��ɽ', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5306, '��ͨ', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5307, '����', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5308, '˼é', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5309, '�ٲ�', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5323, '����', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5325, '���', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5326, '��ɽ', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5328, '��˫����', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5329, '����', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5331, '�º�', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5333, 'ŭ����', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5334, '����', 53, '');
insert into tCity(city_code, city_name, province_code, content) values(5401, '����', 54, 'ʡ��');
insert into tCity(city_code, city_name, province_code, content) values(5421, '����', 54, '');
insert into tCity(city_code, city_name, province_code, content) values(5422, 'ɽ��', 54, '');
insert into tCity(city_code, city_name, province_code, content) values(5423, '�տ���', 54, '');
insert into tCity(city_code, city_name, province_code, content) values(5424, '����', 54, '');
insert into tCity(city_code, city_name, province_code, content) values(5425, '����', 54, '');
insert into tCity(city_code, city_name, province_code, content) values(5426, '��֥', 54, '');
insert into tCity(city_code, city_name, province_code, content) values(6101, '����', 61, 'ʡ��');