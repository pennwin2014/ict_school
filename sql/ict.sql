drop table if exists  userlib;
create table userlib(
	id int(11) auto_increment primary key,
	vname    char(32) NOT NULL,  
	cid      char(32) default '00000000000000000',
	photo     char(128) default '/home/ncmysql/ncsrv/photo/default.png',
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
money    int(11) default 0, 
utype     int(11) default 0, 
timeval    int(11) default 0,
cuser     char(32) default '',
content   char(128) default ''
);
alter table package add unique key uk_package_name(name);

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

insert into package(name,namedes,ptype,money,timeval) values('100元包月套餐','套餐说明',2,50,unix_timestamp(now()));
insert into package(name,namedes,ptype,money,timeval) values('50元基础套餐','套餐说明',6,50,unix_timestamp(now()));
insert into package(name,namedes,ptype,money,timeval) values('20元叠加套餐','套餐说明',6,20,unix_timestamp(now()));
insert into package(name,namedes,ptype,money,timeval) values('10元叠加套餐','套餐说明',6,10,unix_timestamp(now()));
insert into package(name,namedes,ptype,money,timeval) values('7天短期流量包','套餐说明',1,20,unix_timestamp(now()));
insert into package(name,namedes,ptype,money,timeval) values('3个月流量包','套餐说明',3,20,unix_timestamp(now()));
insert into package(name,namedes,ptype,money,timeval) values('6个月流量套餐','套餐说明',4,20,unix_timestamp(now()));
insert into package(name,namedes,ptype,money,timeval) values('包年套餐','套餐说明',5,20,unix_timestamp(now()));
insert into package(name,namedes,ptype,money,timeval) values('包2年套餐','套餐说明',5,20,unix_timestamp(now()));

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


