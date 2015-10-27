drop table if exists ncsmaclog_if_201412;
create table ncsmaclog_if_201412 (
    servicecode char(18)      default ' ',                 -- 场所端编码
    servicename char(64)      default ' ',                 -- 场所端名称
    address     char(64)      default ' ',                 -- 安装位置
    fromdev     char(32)      default ' ',                 -- 数据来源
    mac         char(18)      default ' ',                 -- Mac地址
    apname      char(32),                                  -- AP名称
    apmac       char(18),                                  -- APMac地址
    lrssi       int  default 0,                            -- 开始场强
    urssi       int  default 0,                            -- 离开场强
    ssid        char(64)      default ' ',                 -- SSID,若多个用逗号分隔    
    plate       char(32)      default ' ',                 -- 车辆编号
    line        char(32)      default ' ',                 -- 交通线路名称
    mapid       char(32)      default ' ',                 -- 地图标识(若有值存在，经度维度为想，x,y坐标
    longitude   char(16)      default ' ',                 -- 经度
    latitude    char(16)      default ' ',                 -- 维度
    sendtime         int unsigned default 0,
    starttime        int unsigned default 0,               -- 进入时间
    endtime          int unsigned default 0,               -- 离开时间
    flags             int default 0                         -- 0--进入  1--离开
);


create index ncsmaclog_ind1 on ncsmaclog_if_201412(servicecode);
create index ncsmaclog_ind2 on ncsmaclog_if_201412(servicecode,mac);
create index ncsmaclog_ind3 on ncsmaclog_if_201412(mac);

