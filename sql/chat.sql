--20151228����

--������											����
--��ǩ��											10
--Ⱥ�ı�											11
--Ⱥ�ĳ�Ա��									12
--�������ݱ�									13
--����Ȧ��										14
--����Ȧ����									15
--����Ȧ����ѡ���Կ���				16
--����Ȧ����									17
--������											18
--��Ϣ�ٱ�										19

-- ��ǩ��
drop table if exists ncsrvmarkgroup;
CREATE TABLE ncsrvmarkgroup(
	id int(11) auto_increment primary key,
	mark  char(128) default '',		    -- ��Ʒ��ʶ(���շǾ���λ����)
	ictgroup char(128) default '',		    -- ��ǩ����
	username	char(32) default NULL,      -- �ʺ�
	timeval	int(11) unsigned default '0',   -- �༭ʱ��
	UNIQUE KEY  ncsrvmarkgroup (mark,ictgroup,username), 	
	KEY ncsrvmarkgroup_i1 (mark,username)  	
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- Ⱥ�ı�
drop table if exists ncsrvqun;
CREATE TABLE ncsrvqun(
	id int(11) auto_increment primary key,
	mark char(128) default '',		    -- ��Ʒ��ʶ(���շǾ���λ����)
	qunname char(128) default '',		    -- Ⱥ����
	username char(32) default NULL,      -- ����Ⱥ�ʺ�
	attrib int(2) unsigned default '0', -- 0 ֻ���д���Ⱥ��ά����1 Ⱥ��Ա����ά��	
	chattop char,												-- �ö����� 1 �ö�
	toptime int(11),										-- �ö�ʱ��
	avoiddisturb char,											-- ��Ϣ����� 1 �����
	addressbook char,											  -- ���浽ͨ��¼ 1 ����
	nichen char(32) default NULL,      -- �ڱ�Ⱥ�ǳ�
	nichenshow char,											  -- ��ʾȺ��Ա�ǳ� 1 ��ʾ
	timeval int(11) unsigned default 0, -- �༭ʱ��
	UNIQUE KEY  ncsrvqun_i1(mark,qunname,username)  	
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- Ⱥ�ĳ�Ա��
drop table if exists ncsrvqunlist;
CREATE TABLE ncsrvqunlist(
	id int(11) auto_increment primary key,
	mark char(128) default '',		    -- ��Ʒ��ʶ(���շǾ���λ����)
	qunid int(11),										-- ȺID
	username char(32) default NULL,      -- ��Ա�ʺ�	
	timeval int(11) unsigned default 0, -- �༭ʱ��
	UNIQUE KEY  ncsrvqunlist (mark,qunid,username)	
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- �������ݱ�
drop table if exists ncsrvims;
CREATE TABLE ncsrvims(
	id int(11) auto_increment primary key,
	mark char(128) default '',		    -- ��Ʒ��ʶ(���շǾ���λ����)
	sesstionid int(11) unsigned default 0, -- �ỰID
	senduser char(32) default NULL,      -- �����˺�����
	recvuser char(32) default NULL,      -- �����˺����ƻ�ȺID
	content	char(128) default NULL,      -- ��������
	ictpath char(128) default NULL,      -- ����ͼƬ·��
	conttype char, 	-- �������ͣ�0 ���ݣ�1 Ҫ���Ϊ���� 2 Ҫ�����Ⱥ 3 ϵͳ��Ϣ
    qunid int(11) unsigned default 0, 	-- ȺID
	timeval int(11) unsigned default 0, -- ����ʱ��
	ploattype char(32) default NULL,      -- ����ƽ̨��������
	sendmac char(16) default NULL,      -- ����ƽ̨MAC��ַ
	ssid char(16) default NULL,      -- ���������ӵ�SSID
	sendphone char(16) default NULL,      -- �������ֻ���
	KEY ncsrvims_i1 (mark,senduser),
	KEY ncsrvims_i2 (mark,recvuser),
	KEY ncsrvims_i3 (mark,timeval)
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ׼���ѱ�
drop table if exists ncsrvpfriend;
CREATE TABLE ncsrvpfriend(
    id int(11) auto_increment primary key,
    mark char(128) default '',                      -- ��Ʒ��ʶ(���շǾ���λ����)
    username char(32) default NULL,                 -- �ʺ�
    friend char(32) default NULL,                   -- ׼����
    havnewmsg char default 0,                       -- ������Ϣ 1 ������Ϣ
    recvok char default 0,                          -- �Ƿ�ͬ��� 1 ͬ�����
    timeval int(11) unsigned default 0,             -- ʱ��  ��ȡϵͳʱ��
    UNIQUE KEY ncsrvpfriend(mark,username,friend)
)ENGINE=MyISAM DEFAULT CHARSET=latin1;
	
-- ����Ȧ��
drop table if exists ncsrvfriend;
CREATE TABLE ncsrvfriend(
	id int(11) auto_increment primary key,
	mark char(128) default '', -- ��Ʒ��ʶ(���շǾ���λ����)
	username char(32) default NULL, -- �ʺ�
	friend char(32) default NULL, -- �����ʺŻ�ȺID
	content char(48) default NULL, -- ��ע
	phone char(48) default NULL, -- �绰����
	des char(48) default NULL, -- ����
	photo char(127) default '/home/ncmysql/ncsrv/photo/', -- �����Ƭ�����ͼƬ
	headphoto char(127) default '/home/ncmysql/ncsrv/photo/', -- ͷ��·��
	address char(48) default NULL, -- ����
	timevalbeg int(11) unsigned default '0', -- ���Ϳ�ʼʱ��
	timevalend int(11) unsigned default '0', -- ���ͽ���ʱ��
	chatnums int(11), -- ������������	
	groupid int(11), -- ��ǩID
	timeval int(11) unsigned default '0', -- ����ʱ��
    UNIQUE KEY ictuser (mark,username,friend),
	KEY ncsrvfriend_i1 (username,friend)
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ����Ȧ����
drop table if exists ncsrvfriendmsg;
CREATE TABLE ncsrvfriendmsg(
	id int(11) auto_increment primary key,
	mark char(128) default '',		    -- ��Ʒ��ʶ(���շǾ���λ����)
	username char(32) default NULL,      	-- �ʺ�
	content char(128)default NULL,      -- ��������
	pathtype char, -- ����·������ 0 ͼƬ 1 ������
	ictpath char(128) default NULL, -- ����ͼƬ·��������
	timeval int(11) unsigned default 0, -- ����ʱ��
	lookauth int(2) unsigned default 0, -- �鿴Ȩ�� 0 ���� 1 ˽�� 2 ѡ�е����ѿ��Կ� 3 ѡ�е����Ѳ��ܿ�
	ploattype char(32) default NULL, -- ����ƽ̨��������
	KEY ncsrvfriendmsg_i1 (mark,username),
	KEY ncsrvfriendmsg_i2 (mark,timeval)	
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ����Ȧ����ѡ���Կ���
drop table if exists ncsrvfriendmsgauth;
CREATE TABLE ncsrvfriendmsgauth(
	id int(11) auto_increment primary key,
	mark char(128) default '', -- ��Ʒ��ʶ(���շǾ���λ����)
	username char(32) default NULL, -- �ʺ�
	msgid int(11) unsigned default '0', -- ��Ӧ������ID
	friend char(32) default NULL, -- ѡ��������ʺ�	
	KEY ncsrvfriendmsgauth_i1 (mark,msgid,username)	
)ENGINE=MyISAM DEFAULT CHARSET=latin1;


-- ����Ȧ����
drop table if exists ncsrvfriendmsgex;
CREATE TABLE ncsrvfriendmsgex(
	id int(11) auto_increment primary key,
	mark char(128) default '',		    -- ��Ʒ��ʶ(���շǾ���λ����)
	msgid int(11) unsigned default 0, -- ����Ȧ����ID
	content char(128)default NULL,      -- ��������
	pathtype char, -- ����·������ 0 ͼƬ 1 ������
	ictpath char(128)default NULL, -- ����ͼƬ·��������
	timeval int(11) unsigned default '0', -- ����ʱ��
	ploattype char(32) default NULL, -- ����ƽ̨��������
	KEY ncsrvfriendmsg_i1 (mark,msgid),
	KEY ncsrvfriendmsg_i2 (mark,timeval)
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ������
drop table if exists ncsrvbkname;
CREATE TABLE ncsrvbkname(
	id int(11) auto_increment primary key,
	mark char(128) default '',		    -- ��Ʒ��ʶ(���շǾ���λ����)
	username char(32) default NULL,      	-- �ʺ�
	friend char(32) default NULL,      -- �����ʺŻ�ȺID
	timeval int(11) unsigned default 0, -- ʱ��	
	UNIQUE KEY ictuser (mark,username,friend)
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ��Ϣ�ٱ�
drop table if exists report;
CREATE TABLE report(
	id int(11) auto_increment primary key,
	msgid int(11) unsigned default 0, -- ��Ϣ�ٱ�����ID
	mark char(128) default '',		    -- ��Ʒ��ʶ(���շǾ���λ����)
	username char(32) default NULL,      	-- �ʺ�
	friend char(32) default NULL,      	-- ���ٱ��������ʺŻ�ȺID
	content char(128)default NULL,      	-- �ٱ�����
	timeval int(11) unsigned default 0, -- ʱ��	
	KEY report_i1 (mark,msgid),
	KEY report_i2 (mark,msgid) 
)ENGINE=MyISAM DEFAULT CHARSET=latin1;

--/////////////////////////////////////////////////////////////////////////////
--��ǰ���û���
drop table if exists ncsrvuser;
CREATE TABLE ncsrvuser (
	userid   	int(10) unsigned NOT NULL default '0',  --�û�ID		
	username 	char(32) default NULL,                  --�˺�����
	dispname 	char(32) default NULL,		     	--��ʾ����		
	groupname 	char(32) default NULL,		     	--��������	
	groupid  	int(10) unsigned default '0',	     	--����ID	
	ssid 		char(32) default '',			--SSID	
	proname 	char(32) default '',	
  	openid 		char(32) default '',
  	usermac 	char(96) default '',		     	--MAC	
  	password 	char(32) default NULL,		     	--����	
  	mark  		char(128) default '',		     	--��Ʒ��ʶ(���շǾ���λ����)
  	addtime 	int(10) unsigned default '0',	     	--����ʱ��	
  	moditime 	int(10) unsigned default '0',	     	--�޸�ʱ��	
  	lasttime 	int(10) unsigned default '0',	     	--���ʱ��	
  	expiretime 	int(10) unsigned default '0',		
  	usertype 	int(11) default '0',		     	--�û�����	
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