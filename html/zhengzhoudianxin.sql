--广告
drop table if exists ncmcomindex;
create table ncmcomindex (
    sid        int  primary key,                -- 商标ID
    name       char(32) default ' ',            -- 商标名称
    sgroupid   int  default 0,                  -- 分类编码
    descr      char(128) default ' ',           -- 描述
    imgpath    char(128),                       -- 图片路径
    serno      char(32),                        -- 商标序列号
    floor      char(32),                        -- 楼层
    level      int unsigned default 0,          -- 广告优先级
    plevel     int unsigned default 0,          -- 促销优先级
    type       int unsigned default 0,          -- 1 广告 2 品牌 3促销  4视频
    systime    int unsigned,                    -- 加入时间
    shopid     int unsigned default 0,          -- 商店id
    plateid    int unsigned default 0,          -- 模板id
    flags      int  default 0                   -- 
);
insert into  ncmcomindex (sid, sgroupid,name,imgpath,floor,level,plevel,descr,type,shopid,serno,plateid)
values
(1, 2001,"豫园模板1","/images/yuyuan/home1_r4_c1.jpg", "一层", 1,1, "豫园1",1,1,"111",2),
(2, 2001,"豫园模板1","/images/yuyuan/home1_r6_c1.jpg", "一层", 2,2, "豫园2",1,5,"222",2),
(3, 4001,"豫园模板1","/images/yuyuan/home1_r6_c5.jpg", "二层", 3,3, "豫园3",1,2,"333",2),
(4, 5001,"豫园模板1","/images/yuyuan/home1_r6_c7.jpg", "二层", 4,4, "豫园4",1,3,"444",2),
(5, 6001,"豫园模板1","/images/yuyuan/home1_r8_c1.jpg", "一层", 5,5, "豫园5",1,5,"555",2),
(6, 6001,"豫园模板1","/images/yuyuan/home1_r8_c5.jpg", "一层", 6,6, "豫园6",1,5,"666",2),
(7, 6001,"豫园模板1","/images/yuyuan/home1_r8_c7.jpg", "一层", 7,7, "豫园7",1,5,"777",2),
(8, 6001,"豫园模板1","/images/yuyuan/home1_r10_c1.jpg", "三层", 8,8, "豫园8",1,8,"888",2),
(9, 6001,"豫园模板1","/images/yuyuan/home1_r12_c1.jpg", "四层", 9,9, "豫园9",1,9,"999",2),
(10, 6001,"豫园模板1","/images/yuyuan/home1_r14_c1.jpg", "四层", 10,10, "豫园10",1,10,"1000",2),
(11, 2001,"豫园模板2","/images/yuyuan2/shouye_r2_c1.jpg", "一层", 1,1, "豫园1",1,1,"111",3),
(12, 2001,"豫园模板2","/images/yuyuan2/shouye_r4_c2.jpg", "一层", 2,2, "豫园2",1,5,"222",3),
(13, 4001,"豫园模板2","/images/yuyuan2/shouye_r4_c4.jpg", "二层", 3,3, "豫园3",1,2,"333",3),
(14, 5001,"豫园模板2","/images/yuyuan2/shouye_r4_c6.jpg", "二层", 4,4, "豫园4",1,3,"444",3),
(15, 6001,"豫园模板2","/images/yuyuan2/shouye_r6_c2.jpg", "一层", 5,5, "豫园5",1,5,"555",3),
(16, 6001,"豫园模板2","/images/yuyuan2/shouye_r6_c4.jpg", "一层", 6,6, "豫园6",1,5,"666",3),
(17, 6001,"豫园模板2","/images/yuyuan2/shouye_r6_c6.jpg", "一层", 7,7, "豫园7",1,5,"777",3),
(18, 6001,"豫园模板2","/images/yuyuan2/shouye_r8_c2.jpg", "三层", 8,8, "豫园8",1,8,"888",3),
(19, 6001,"豫园模板2","/images/yuyuan2/shouye_r10_c2.jpg", "四层", 9,9, "豫园9",1,9,"999",3),
(20, 6001,"豫园模板2","/images/yuyuan2/shouye_r10_c4.jpg", "四层", 10,10, "豫园10",1,10,"1000",3),
(21,	2001,	"Buberry",	"/images/suning/mall_r9_c1.gif", 	"一层",	 1,	1,	"Buberry",	1,	4,	"111"	,1)	,
(22,	4001,	"Dior",	"/images/suning/20111019154944_iTX3u.png",	"一层",	 2,	2,	"迪奥",	1,	5,	"2222"	,1)	,
(23,	5001,	"Catier",	"/images/suning/200908211.jpg",	"二层",	 3,	3,	"卡地亚",	1,	6,	"333"	,1)	,
(24,	6001,	"Tiffany",	"/images/suning/21694c1a999ec21b7f614fa1954e_586_542.jpg",	"二层",	 4,	4,	"提芬尼",	1,	7,	"444"	,1),
(25,	6001,	"Dior",	"/images/suning/dior.gif",	"一层",	 7,	7,	"迪奥",	1,	5,	"555"	,1)	,
(26,	6001,	"阿玛尼",	"/images/suning/mall_r7_c9.gif",	"一层",	 6,	6,	"阿玛尼",	1,	9,	"666"	,1)	,
(27,	6001,	"苹果",	"/images/suning/apple.png",	"四层",	 5,	5,	"苹果",	1,	74,	"777"	,1)	,
(28,	7001,	"安德里",	"/images/suning/logo1.jpg",	"三层",	 8,	8,	"logo1",	1,	11,	"888"	,1)	,
(29,	7001,	"西门子",	"/images/suning/logo2.jpg",	"四层",	 9,	9,	"logo2",	1,	12,	"999"	,1)	,
(30,	7001,	"正泰",	"/images/suning/logo3.jpg",	"四层",	 10,	10,	"logo3",	1,	13,	"1000"	,1)	,
(31,	7001,	"三菱",	"/images/suning/logo4.jpg",	"四层",	 11,	11,	"logo4",	1,	14,	"1111"	,1)	,	
(32, 6001,"郑电模板1","/images/zhengzhoudianxin/zhengzhoudianxin_r2_c1.jpg", "四层", 1,1, "郑电1",1,9,"999",4),
(33, 6001,"郑电模板1","/images/zhengzhoudianxin/zhengzhoudianxin_r2_c2.jpg", "四层", 2,2, "郑电2",1,9,"999",4),
(34, 6001,"郑电模板1","/images/zhengzhoudianxin/zhengzhoudianxin_r2_c3.jpg", "四层", 3,3, "郑电3",1,9,"999",4),
(35, 6001,"郑电模板1","/images/zhengzhoudianxin/zhengzhoudianxin_r2_c4.jpg", "四层", 4,4, "郑电4",1,9,"999",4),
(36, 6001,"郑电模板1","/images/zhengzhoudianxin/zhengzhoudianxin_r5_c2.jpg", "四层", 5,5, "郑电5",1,9,"999",4),
(37, 6001,"郑电模板1","/images/zhengzhoudianxin/zhengzhoudianxin_r5_c1.jpg", "四层", 6,6, "郑电6",1,9,"999",4),
(38, 6001,"郑电模板1","/images/zhengzhoudianxin/zhengzhoudianxin_r7_c2.jpg", "四层", 7,7, "郑电7",1,9,"999",4),
(39, 6001,"郑电模板1","/images/zhengzhoudianxin/zhengzhoudianxin_r7_c10.jpg", "四层", 8,8, "郑电8",1,9,"999",4),
(40, 6001,"郑电模板1","/images/zhengzhoudianxin/zhengzhoudianxin_r9_c2.jpg", "四层", 9,9, "郑电9",1,9,"999",4),
(41, 6001,"郑电模板1","/images/zhengzhoudianxin/zhengzhoudianxin_r9_c8.jpg", "四层", 10,10, "郑电10",1,9,"999",4),
(42, 6001,"郑电模板1","/images/zhengzhoudianxin/zhengzhoudianxin_r9_c14.jpg", "四层", 11,11, "郑电11",1,9,"999",4),
(43, 6001,"郑电模板1","/images/zhengzhoudianxin/zhengzhoudianxin_r12_c2.jpg", "四层", 12,12, "郑电12",1,9,"999",4),
(44, 6001,"郑电模板1","/images/zhengzhoudianxin/zhengzhoudianxin_r12_c4.jpg", "四层", 13,13, "郑电13",1,9,"999",4),
(45, 6001,"郑电模板1","/images/zhengzhoudianxin/zhengzhoudianxin_r12_c11.jpg", "四层", 14,14, "郑电14",1,9,"999",4),
(46, 6001,"郑电模板1","/images/zhengzhoudianxin/zhengzhoudianxin_r12_c15.jpg", "四层", 15,15, "郑电15",1,9,"999",4);


-- 广告模板选择表
drop table if exists portplate;
create table portplate (
    sid  int  AUTO_INCREMENT primary key,
    platename char(64),                         -- 模板名称
    platepath char(128),                        -- 模板路径
    daterang   char(255),                       -- 默认投放时间段空 不限  08:00:00-12:00:00,09:11:11-15:10:11
    langflag  int unsigned default 0,           -- 语言 0-中文 1-英文 2-日文
    termtype  char(32)  default '',             --  pc,pad,mobile
    apssid    char(255) default '',             --  apssid
    mark      text,
    groupid    int unsigned,                    --  0 不限 投放用户组
    status   int unsigned default 0             --  0 启用  1-停用
);

insert into portplate(sid,platename, platepath,daterang,langflag,termtype,apssid, mark, groupid, status)
	values
	(1, "商城模板1","/suning/shop_menus.htm","",0,"","","",0,1),
	(2, "豫园模板1","/yuyuan/shouye.html","",0,"","","",0,1),
	(3, "豫园模板2","/yuyuan2/shouye.html","",0,"","","",0,1),
	(4, "郑电模板1","/zhengzhoudianxin/shop_menus.htm","",0,"","","",0,0);

