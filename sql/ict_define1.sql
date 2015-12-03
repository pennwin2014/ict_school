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
	dname char(23) default '',                       -- �ǳ�
	mname char(23) default '',                       -- ��ʵ����
	gender char default 0,                           -- �Ա�  
	cid char(20) default '00000000000000000',        -- ���֤����
	studentid char(20) default '000000000000',       -- ѧ��
	college char(31) default '',			         -- ѧԺ
	department char(31) default '',                  -- �༶(ϵ��)
	photo char(127) default '/home/ncmysql/ncsrv/photo/default.png', -- ͷ��·��
	email char(63) default '',                       -- ����
	qq char(12) default '',                          -- qq��
	hobby char(127) default '',                      -- ����
	address char(127) default '',                    -- ��ַ	
	money int(11) default 0,                         -- ��ֵ���(�ҵ�Ǯ��)
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
	namedes char(127) default '',                    -- �ײ�˵��
	ptype int(11) default 1,                         -- �ײ����� 1 ���� 2 ���� 3 ���� 4  ������ 5���� 6 ����
	pdays int(11) default 0,                         -- �ײ�����
	money int(11) default 0,                         -- �ײͽ��(Ԫ)
	picture char(127) default '/home/ncmysql/ncsrv/photo/package/default.png',          -- �ײ�ͼƬ 
	colorpicture char(127) default '/home/ncmysql/ncsrv/photo/package/default1.png',    -- �ײ�ɫ�� 
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
    UNIQUE KEY uk_userorder_name(mark,username),
	KEY uk_userorder_time(starttime) 
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- �û������ײ���־��
drop table if exists userorderlog_201511;
create table userorderlog_201511(
	id int(11) auto_increment primary key,
	username char(32) default NULL,                      -- �˺�����
	mobno char(23) default '',                           -- ���ֻ���
	mark char(128) default '',                           -- ��Ʒ��ʶ(���շǾ���λ����)
	packageid int(11) NOT NULL,                          -- �����ײ�id
	name char(31) NOT NULL,                              -- �ײ�����
	status char default 0,                               -- 1 ���� 2 ���������ײ�����ײ�����ʱ��
	memory int(11) NOT NULL,                             -- ����������ʱ��Ӧ��Ϊ�ײͽ�����ʱ����ʱΪ�ײͽ���ʱ������ײͽ������������ЧӦ�ÿ۳�									--�ϸ��ײ�ʣ�·��ã�
	starttime int(11) default 0,                         -- ��Ч��ʼʱ��
	endtime int(11) default 0,                           -- ����ʱ��
	timeval int(11) default 0,                           -- ����ʱ��
    KEY userorderlog_l1(mark),
	KEY userorderlog_l2(timeval)				
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
	price int(11) default 0,                         -- ���λ�۸�
	timeval int(11) default 0,                       -- ����ʱ��
	starttime int(11) default 0,                         -- ��Ч��ʼʱ��
	endtime int(11) default 0,                           -- ����ʱ��
	UNIQUE KEY uk_adManage_adname(mark,adname)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;




insert into package(name,mark,namedes,ptype,pdays,money,timeval,picture) values('pweek','1','7���ײ�',1,7,10,unix_timestamp(now()),'/school/photo/package/pweek.png');
insert into package(name,mark,namedes,ptype,pdays,money,timeval,picture) values('pmonth','1','�����ײ�',2,30,20,unix_timestamp(now()),'/school/photo/package/pmonth.png');
insert into package(name,mark,namedes,ptype,pdays,money,timeval,picture) values('pseason','1','�����ײ�',3,90,30,unix_timestamp(now()),'/school/photo/package/pseason.png');
insert into package(name,mark,namedes,ptype,pdays,money,timeval,picture) values('phalfyear','1','�����ײ�',4,180,40,unix_timestamp(now()),'/school/photo/package/phalfyear.png');
insert into package(name,mark,namedes,ptype,pdays,money,timeval,picture) values('pyear','1','�����ײ�',5,365,50,unix_timestamp(now()),'/school/photo/package/pyear.png');
insert into package(name,mark,namedes,ptype,pdays,money,timeval,picture) values('puserdefine','1','�û��Զ����ײ�',6,5000,50,unix_timestamp(now()),'/school/photo/package/puserdefine.png');

insert into recpackage(id, useflag, timeval) values(1, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(2, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(3, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(4, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(5, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(6, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(9, 1, unix_timestamp(now()));