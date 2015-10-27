drop table if exists ncmfloor;                          -- ¥����Ϣ 
create table ncmfloor (
    fid        int unsigned auto_increment primary key,
    name       char(32),                                 -- ¥������ 
    detail     text,                                          -- ��Ӫ����
    cid       int unsigned  default 0                                 --  ��λID
);


drop table if exists ncmbrand;                          -- Ʒ����Ϣ  
create table ncmbrand (
    bid        int unsigned auto_increment primary key,
    fid         int,                            -- ¥��id  
    name      char(32) default ' ',            -- �̱�����
    btype     int  default '',                 -- ��Ʒ��� 
    descr           char(128) default ' ',           -- ����
    imgpath         char(128),                       -- ͼƬ·��
    cid      int unsigned default 0,                                 --  ��λID
    shopid      int  default 0,                                 --  �̵�ID
    flag     int default 0,                                 -- 0 ��Ч   1 -- ��Ч
   systime         int unsigned                   -- ����ʱ��
);

alter table ncmbrand modify cid int unsigned default 0;
-- ��Ʒ��������
drop table if exists ncmcomgroup;  
create table ncmcomgroup (
    groupid    int  primary key,           -- ������
    name       char(32),                   -- ��Ʒ��������
    descr      char(128) default ' ',      -- ����
    systime    int unsigned,               -- ����ʱ��
    imgpath    char(128),                  -- ͼƬ·��
    flags      int  default 0,              -- 
    plateid     int(11)    
);

update ncmcomgroup set systime =1386847768;

-- ��ƷС������
drop table if exists ncmcomtype;  
create table ncmcomtype (
    sid        int  primary key,           -- ������
    name       char(32),                   -- ��ƷС������
    groupid    int  default 0,             -- �������   
    descr      char(128) default ' ',      -- ����
    systime    int unsigned,               -- ����ʱ��
    imgpath    char(128),                  -- ͼƬ·��
    flags      int  default 0              -- 
);

-- ��Ʒϸ��
drop table if exists ncmcomodity;
create table ncmcomodity (
    sid          int  primary key,                -- ��ƷID
    name         char(32) default ' ',            -- ��Ʒ����            ����      
    dispname     char(32) default ' ',            -- ��Ʒ��ʾ����        ����
    service_code char(16) ,                       -- ��������
    sgroupid     int  default 0,                  -- �̳�ID
    groupid      int  default 0,                  -- �������         С��
    shgroupid    int  default 0,                  -- �̵�id
    imgpath      char(128),                       -- ͼƬ·��        
    shopname     char(32) default ' ',            -- �̵�����
    shimgpath    char(128),                       -- �̵�ͼƬ
    factory      char(32) default ' ',            -- ����
    type         char(128),                       -- ��Ʒ�ͺ�
    url          char(255),                       -- ��ַ
    floor        char(32),                        -- ¥��
    fldetail     char(255),                       -- ¥�����
    fee          float(10,2) default 0,           -- �ۼ�
    discount     float(10,2) default 0,           -- �ۿ�
    code         char(12) default '',             -- ��������/���Ҵ���
    tel          char(32) default '',             -- �绰
    address      char(255) default ' ',           -- ��ַ
    reserve      int unsigned,                    -- ���
    aid          int unsigned default 0,          -- �id 
    active       char(64) default ' ',            -- �����
    plevel       int unsigned default 0,          -- ����ȼ�
    level        int unsigned default 0,          -- ������ȼ�
    descr        char(255) default ' ',           -- ����               ����ʱ��
    detail       text ,                           -- ����               ���
    integral     int unsigned default 0,          -- ����
    coupon       int unsigned default 0,          -- �Ż�ȯ
    atype        int unsigned default 0,          -- ��ע Ĭ��0  ��ע 1
    systime      int unsigned,                    -- ����ʱ��           չʾʱ��
    flags        int  default 0                   -- 0Ĭ�ϴ���  1 ɾ��   3������Ʒ
);

alter table ncmcomodity add img_long int unsigned default 0;  -- ͼƬ����
alter table ncmcomodity add img_width int unsigned default 0;  -- ͼƬ���
alter table ncmcomodity add cid int unsigned default 0;  -- ���ӵ�λID



-- �����Ż�
drop table if exists ncmcomodity_cxyh;
create table ncmcomodity_cxyh (
    sid          int unsigned auto_increment primary key,                -- ��ƷID
    name         char(32) default ' ',            -- ��Ʒ����               
    dispname     char(32) default ' ',            -- ��Ʒ��ʾ����        
    gtype        int  default 0,                  -- ��Ʒ���
    shopid    int  default 0,                     -- �̵�id
    imgpath      char(128),                       -- ͼƬ·��        
    shopname     char(32) default ' ',            -- �̵�����
    factory      char(32) default ' ',            -- ����
    type         char(128),                       -- ��Ʒ�ͺ�
    fee          float(10,2) default 0,           -- �ۼ�
    discount     float(10,2) default 0,           -- �ۿ�
    reserve      int unsigned,                    -- ���
    startstoptime   char(48) default '',          -- ���ʼ�ͽ���ʱ��
    systime      int unsigned,                    -- ����ʱ��           չʾʱ��
    flags        int  default 0,                   -- 0��ʾ  1 ����ʾ
    descr        char(128),                        -- ����
    cid          int unsigned default 0,                    -- ��λID
    showflag     int unsigned default 0,            --0 -һ����Ʒһ��       1-������Ʒһ��
    num          int unsigned defalut 0           -- ��ʾ���
);


-- �̵��
drop table if exists ncmshop;                      --    ��չ�ֶ�˵����Ӧ�����ֶ�
create table ncmshop (
    id           int  primary key,                 -- �̵�ID
    name         char(32) default ' ',             -- �̵�����            ������
    service_code char(16) ,                        -- ��������
    sgroupid     int  unsigned default 0,          -- �̳�ID(��λID)
    imgpath      char(128),                        -- ͼƬ·��
    floor        char(32),                         -- ¥��                չ̨
    url          char(255),                        -- ��ַ                ����������
    code         char(12) default '',              -- ��������/���Ҵ���
    tel          char(32) default '',              -- �绰
    address      char(255) default ' ',            -- ��ַ                չ��   ��һ��
    level        int unsigned default 0,           -- ������ȼ�
    plevel       int unsigned default 0,           -- �������ȼ�
    type         int unsigned default 0,           -- 1 ��� 2 Ʒ��
    stype        int unsigned default 0,           -- �̵����� 
    aid          int unsigned default 0,           -- �id
    descr        char(255) default ' ',            -- ����                 ���Ź�˾
    detail       text ,                            -- ����              ���������
    systime      int unsigned,                     -- ����ʱ��
    flags        int  default 0                    -- 
);
alter table ncmshop modify sgroupid int unsigned default 0;

-- ��Ʒϸ��
-- �����̳�ID
alter table ncmcomodity modify sgroupid int unsigned default 0;
drop table if exists ncmcomodity;
create table ncmcomodity (
    sid          int  primary key,                -- ��ƷID
    name         char(32) default ' ',            -- ��Ʒ����            ����      
    dispname     char(32) default ' ',            -- ��Ʒ��ʾ����        ����
    service_code char(16) ,                       -- ��������
    sgroupid     int unsigned default 0,                  -- �̳�ID
    groupid      int  default 0,                  -- �������
    shgroupid    int  default 0,                  -- �̵�id
    imgpath      char(128),                       -- ͼƬ·��        
    shopname     char(32) default ' ',            -- �̵�����
    shimgpath    char(128),                       -- �̵�ͼƬ
    factory      char(32) default ' ',            -- ����
    type         char(128),                       -- ��Ʒ�ͺ�
    url          char(255),                       -- ��ַ
    floor        char(32),                        -- ¥��
    fldetail     char(255),                       -- ¥�����
    fee          float(10,2) default 0,           -- �ۼ�
    discount     float(10,2) default 0,           -- �ۿ�
    code         char(12) default '',             -- ��������/���Ҵ���
    tel          char(32) default '',             -- �绰
    address      char(255) default ' ',           -- ��ַ
    reserve      int unsigned,                    -- ���
    aid          int unsigned default 0,          -- �id 
    active       char(64) default ' ',            -- �����
    plevel       int unsigned default 0,          -- ����ȼ�
    level        int unsigned default 0,          -- ������ȼ�
    descr        char(255) default ' ',           -- ����               ����ʱ��
    detail       text ,                           -- ����               ���
    integral     int unsigned default 0,          -- ����
    coupon       int unsigned default 0,          -- �Ż�ȯ
    atype        int unsigned default 0,          -- ��ע Ĭ��0  ��ע 1
    systime      int unsigned,                    -- ����ʱ��           չʾʱ��
    flags        int  default 0                   -- 0Ĭ�ϴ���  1 ɾ��   3������Ʒ
);
| plateid      | int(11)          | YES  |     | 0       |       |
| img_long     | int(10) unsigned | YES  |     | 0       |       |
| img_width    | int(10) unsigned | YES  |     | 0       |       |




-- ��Ա��Ϣע����־
drop table if exists ncmshopuserlog;
create table if not exists ncmshopuserlog (
    sid                int unsigned auto_increment primary key,                -- ��Ա�û�id
    username           char(32) ,           -- �û���¼��
    did                int  unsigned,                   -- ��λID
    dispname           char(32)  default '',            -- ��ʾ��
    sex                char(8)   default '',            -- �Ա�
    mobile             char(18)  default '',            -- �ֻ���
    idno               char(20)  default '',            -- ֤����
    email              char(64)  default '',            -- �ʼ���ַ
    stime           int unsigned default 0          -- ������ʱ��
);
