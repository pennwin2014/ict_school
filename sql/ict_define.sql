--��ǰ���û���
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

--�û���
drop table if exists  userlib;
create table userlib(
	id int(11) auto_increment primary key,
	vname    char(31) NOT NULL,  --������ݣ���λʹ����,һ��Ϊ�ֻ��ţ���������ע���˺ţ�
	cid      char(31) default '00000000000000000',--���֤����
	studentid  char(31) default '000000000000',--ѧ��
	college    char(31) default '',--ѧԺ
	department char(31) default '',--�༶(ϵ��)
	email      char(63) default '',--����
	photo     char(127) default '/home/ncmysql/ncsrv/photo/default.png',--ͷ��·��
	hobby     char(127) default '',--����
	qq        char(31) default '',--��qq��
	wechat    char(31) default '',--��΢�ź�
	weibo     char(63) default '',--��΢��
	dname    char(23) default '',  --�ǳ�
	mname    char(23) default '',  --����
	gender    char default 0,    --�Ա�  
	address    char(127) default '',--��ַ
	passwd    char(23) default '123456', --���� 
	mobno    char(23) default '',  --�ֻ���
	money    int(11) default 0,    --��ֵ���
	packageid   int(11) default 0,   --�ײ�id ��������Ч��  
	timeval    int(11) default 0  --�ײ���Чʱ��
);

--�ײͱ�
drop table if exists package;
create table package(
id int(11) auto_increment primary key, 
name char(31) NOT NULL ,--�ײ�����
namedes  char(127) default '',--�ײ�˵��
ptype     int(11) default 1,--�ײ����� 1 ���� 2 ���� 3 ���� 4  ������ 5����
							--6 ����
pdays     int(11) default 0, --�ײ�����
money    int(11) default 0, --�ײͽ�Ԫ��
utype     int(11) default 0, --�������� ��1 �Զ����ѣ�
picture    char(127) default '/home/ncmysql/ncsrv/photo/package/default.png',--�ײ�ͼƬ 
timeval    int(11) default 0,--�༭ʱ��
cuser     char(31) default '', --�༭�û�
content   char(127) default ''--��ע
);
alter table package add unique key uk_package_name(name);

--�û������ײͱ�
drop table if exists userpackage;
create table userpackage(
	id int(11) auto_increment primary key,
	vname char(31) NOT NULL, --�û���
	packageid int(11) NOT NULL, --�ײ�id
	starttime int(11) default 0,--��ʼʱ��
	endtime  int(11) default 0,--����ʱ��
	status char default 0 --״̬  0����Ч  1��ʧЧ
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--�Ƽ��ײͱ�
drop table if exists recpackage;
create table recpackage(
id int(11) not null primary key,--�ײ�id 
useflag int unsigned default 0,--�Ƿ�ʹ��(0��ʹ�� 1����ʹ��)
timeval    int(11) default 0,--ʱ��
cuser     char(31) default '',--����Ա
content   char(127) default ''--��ע
);
create unique index uqidx_recpackage_id on recpackage(id); 
ALTER TABLE recpackage ADD CONSTRAINT fk_recpackage_id FOREIGN KEY (id) REFERENCES package(id) ON DELETE CASCADE ON UPDATE CASCADE;

--��ֵ��־
drop table if exists rechargelog;
create table rechargelog(
	id int(11) auto_increment primary key,  
	vname char(31) default '', --������ݣ���λʹ����,һ��Ϊ�ֻ��ţ���������ע���˺ţ�
	money int(11) default 0,--��Ԫ��
	mtype int(11) default 0,--��ֵ��ʽ��1 ΢�� 2 ֧���� 3 һ��ͨ��
	timeval int(11) default 0, --��ֵʱ��
	orderno char(63) default ''--������
);

--�����ײ���־
drop table if exists orderlog;
create table orderlog(
	id int(11) auto_increment primary key,
	vname char(31) default '', --������ݣ���λʹ����,һ��Ϊ�ֻ��ţ���������ע���˺ţ�
	packageid int(11) default 0,--�ײ�id
	dstatus int(11) default 0,--����״̬( 1 ��Ч�� 0��Ч)
	dtime int(11) default 0--����ʱ��
);