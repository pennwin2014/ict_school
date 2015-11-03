drop table if exists  userlib;
create table userlib(
	id int(11) auto_increment primary key,
	vname    char(32),  
	photo     char(128),
   dname    char(24),  
   mname    char(24),  
   gender    char,     
   address    char(128),
   passwd    char(24), 
   mobno    char(24),  
   money    int(11),   
   package   char(32), 
   timeval    int(11)  
);

drop table if exists package;
create table package(
id int(11), 
name char(32),
namedes  char(128),
ptype     int(11),
money    int(11), 
utype     int(11),
timeval    int(11),
cuser     char(32),
content   char(128)
);

create table rechargelog(id int(11), vname char(32),money int(11),mtype int(11),timeval int(11) );

create table orderlog(id int(11),vname char(32),name char(32),dstatus int(11),dtime int(11));

insert into package (name,namedes,ptype,money)values('100元包月套餐','套餐说明',2,50);
insert into package (name,namedes,ptype,money)values('50元基础套餐','套餐说明',6,50);
insert into package (name,namedes,ptype,money)values('20元叠加套餐','套餐说明',6,20);
insert into package (name,namedes,ptype,money)values('10元叠加套餐','套餐说明',6,10);
insert into package (name,namedes,ptype,money)values('7天短期流量包','套餐说明',1,20);
insert into package (name,namedes,ptype,money)values('3个月流量包','套餐说明',3,20);
insert into package (name,namedes,ptype,money)values('6个月流量套餐','套餐说明',4,20);
insert into package (name,namedes,ptype,money)values('包年套餐','套餐说明',5,20);
