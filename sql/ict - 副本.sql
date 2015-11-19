drop table if exists  userlib;
create table userlib(
	id int(11) auto_increment primary key,
	vname    char(32) NOT NULL,  
	cid      char(32) default '00000000000000000',
	studentid  char(32) default '000000000000',
	college    char(32) default '',
	department char(32) default '',
	email      char(64) default '',
	photo     char(128) default '/home/ncmysql/ncsrv/photo/default.png',
	hobby     char(128) default '',
	qq        char(32) default '',
	wechat    char(32) default '',
	weibo     char(64) default '',
	dname    char(24) default '',  
	mname    char(24) default '',  
	gender    char default 0,     
	address    char(128) default '',
	passwd    char(24) default '123456', 
	mobno    char(24) default '',  
	money    int(11) default 0,   
	packageid   int(11) default 0, 
	timeval    int(11) default 0
);

drop table if exists package;
create table package(
id int(11) auto_increment primary key, 
name char(32) NOT NULL ,
namedes  char(128) default '',
ptype     int(11) default 1,
pdays     int(11) default 0,
money    int(11) default 0, 
utype     int(11) default 0, 
timeval    int(11) default 0,
cuser     char(32) default '',
content   char(128) default ''
);
alter table package add unique key uk_package_name(name);

drop table if exists userpackage;
create table userpackage(
  vname char(32) NOT NULL, 
  packageid int(11) NOT NULL,
  starttime int(11) default 0,
  endtime  int(11) default 0,
  status char default 0,
  primary key (vname, packageid)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

drop table if exists recpackage;
create table recpackage(
id int(11) not null primary key, 
useflag int unsigned default 0,
timeval    int(11) default 0,
cuser     char(32) default '',
content   char(128) default ''
);
create unique index uqidx_recpackage_id on recpackage(id); 
ALTER TABLE recpackage ADD CONSTRAINT fk_recpackage_id FOREIGN KEY (id) REFERENCES package(id) ON DELETE CASCADE ON UPDATE CASCADE;

drop table if exists rechargelog;
create table rechargelog(
	id int(11) auto_increment primary key, 
	vname char(32) default '',
	money int(11) default 0,
	mtype int(11) default 0,
	timeval int(11) default 0,
	orderno char(64) default ''
);

drop table if exists orderlog;
create table orderlog(
	id int(11) auto_increment primary key,
	vname char(32) default '',
	packageid int(11) default 0,
	dstatus int(11) default 0,
	dtime int(11) default 0
);

insert into package(name,namedes,ptype,pdays,money,timeval) values('pweek','????',1,7,10,unix_timestamp(now()));
insert into package(name,namedes,ptype,pdays,money,timeval) values('pmonth','????',2,30,20,unix_timestamp(now()));
insert into package(name,namedes,ptype,pdays,money,timeval) values('pseason','????',3,90,30,unix_timestamp(now()));
insert into package(name,namedes,ptype,pdays,money,timeval) values('phalfyear','????',4,180,40,unix_timestamp(now()));
insert into package(name,namedes,ptype,pdays,money,timeval) values('pyear','????',5,365,50,unix_timestamp(now()));
insert into package(name,namedes,ptype,pdays,money,timeval) values('puserdefine','???????',6,5000,50,unix_timestamp(now()));

insert into recpackage(id, useflag, timeval) values(1, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(2, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(3, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(4, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(5, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(6, 1, unix_timestamp(now()));
insert into recpackage(id, useflag, timeval) values(9, 1, unix_timestamp(now()));

insert into orderlog(id,vname,packageid,dstatus,dtime) values(8,124,1,50,unix_timestamp("2015-11-12 12:12:12"));
insert into orderlog(id,vname,packageid,dstatus,dtime) values(9,124,2,60,unix_timestamp("2015-11-22 12:12:12"));
insert into orderlog(id,vname,packageid,dstatus,dtime) values(10,124,1,70,unix_timestamp("2015-10-11 12:12:12"));
insert into orderlog(id,vname,packageid,dstatus,dtime) values(11,124,2,80,unix_timestamp("2015-10-22 12:12:12"));
insert into orderlog(id,vname,packageid,dstatus,dtime) values(12,124,3,90,unix_timestamp("2015-09-12 12:12:12"));


insert into rechargelog(id,vname,money,mtype,timeval,orderno) values(1,124,100,1,unix_timestamp("2015-11-02 12:12:12"),'100');
insert into rechargelog(id,vname,money,mtype,timeval,orderno) values(2,124,200,2,unix_timestamp("2015-11-22 12:12:12"),'100');
insert into rechargelog(id,vname,money,mtype,timeval,orderno) values(3,124,200,3,unix_timestamp("2015-10-22 12:12:12"),'100');
insert into rechargelog(id,vname,money,mtype,timeval,orderno) values(4,124,100,1,unix_timestamp("2015-09-02 12:12:12"),'100');
insert into rechargelog(id,vname,money,mtype,timeval,orderno) values(5,124,300,2,unix_timestamp("2015-09-12 12:12:12"),'100');


