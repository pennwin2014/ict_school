
//ϵͳ��ɫ
var store_system_role = Ext.create('Ext.data.Store',{pageSize: 40,fields: ['username', 'fun', 'dx','userid'],idProperty: 'userid',autoLoad: false,id:'store_system_role',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncsRolelist_v4',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: '',direction: 'DESC'}]});
   store_system_role.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_system_role.new_params);
       }); 
//��ɫ����     			
var  store_right = Ext.create('Ext.data.Store', {pageSize: 40,fields: ['vara', 'checked1', 'rolename','roldispname','userid','dispname'], autoLoad: false,id:'store_right',remoteSort: true,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncsDispRoleFunlist_v4',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'userid',direction: 'DESC'}]});
	
//ϵͳ�û���Ϣ
var store_adminuser = Ext.create('Ext.data.Store', {pageSize: 40,id:'store_adminuser',fields: ['username', 'dispname', 'email', 'addtime', 'level','email','logcount','userid','opt','groupname','role','syjm','groupid','optid'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncsSysUserlist_v4',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{}]});
	   store_adminuser.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_adminuser.new_params);
       });	
       
// ϵͳ���˵�����
var store_systems_menu = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_systems_menu',fields: ['id', 'mainmenu', 'systime', 'flag', 's_flag'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webmenu_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'id',direction: 'DESC'}]});
	store_systems_menu.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_systems_menu.new_params);
    });
    
// ϵͳ�Ӳ˵�����
var store_systems_submenu = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_systems_submenu',fields: ['sid', 'submenu', 'systime', 'pid', 'mainmenu', 'flag', 's_flag'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_websubmenu_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'pid',direction: 'ASC'}]});
	store_systems_submenu.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_systems_submenu.new_params);
    });
    	      	
//��λѡ����
var store_groupcheck = Ext.create('Ext.data.TreeStore', {id:'store_groupcheck',autoLoad : false,
        proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncTreeGroup_v8&plate@v8/nc_checkgroup.htm'},
        root: {text: 'ȫ��',id: 'all',checked:false,expanded: true},
        folderSort: true,
        sorters: [{property: 'text',direction: 'ASC'}]});  
//������ 
 var store_classtree=Ext.create('Ext.data.TreeStore',{id:'store_classtree', fields: ['text','id'],autoLoad : false,
				proxy:{type: 'ajax',url: '/pronline/Msg?FunName@ncTreeGroup_v8&plate@v8/nc_group_tree.htm'}  
				});        	
//�����û��� 
 var store_usertree=Ext.create('Ext.data.TreeStore',{id:'store_usertree', fields: ['text','id','userid','ctext','dtype'],autoLoad : false,
				proxy:{type: 'ajax',url: '/pronline/Msg?FunName@ncTreeUser_v8&v8/nc_user_tree.htm'},folderSort:true, 
				root:{text:'ȫ��',id:'',expanded:true}
				}); 
	store_usertree.on('beforeload', function (store, options) {
  //     var new_params = { state: Ext.getCmp('compstate').getValue() };
        Ext.apply(store.proxy.extraParams, store_usertree.new_params);
  });			  
  
        	   			
//ϵͳ������־
var store_system_log = Ext.create('Ext.data.Store',{pageSize: 40,fields: ['sdate', 'descr', 'action', 'name', 'ipaddr','result'],autoLoad: false,id:'store_system_log',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@nc_systemLog_search',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'sid',direction: 'DESC'}]});
   store_system_log.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_system_log.new_params);
       }); 
//�û���Ϣ      
 var store_user = Ext.create('Ext.data.Store', {pageSize: 40,id:'store_user',fields: ['username', 'dispname', 'ip', 'mac', 'groupname','groupid','idno','userid','idtype','idphname','compname','lasttime'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncUserlist_v8',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{}]});
	   store_user.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_user.new_params);
       });
/*��־*/

//�û���־   
var store_logs_userlog = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_logs_userlog',fields: ['sid', 'portalname', 'userid', 'groupid', 'groupname', 'username', 'dispname', 'ssid', 'apname', 'mac', 'ip', 'dev', 'os', 'bro', 'devtype', 'ubytes', 'dbytes', 'starttime', 'endtime', 'conntime', 'cause'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_Place_userlog_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'sid',direction: 'DESC'}]});
	store_logs_userlog.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_logs_userlog.new_params);
    });	
//�����־
var store_logs_adlog = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_logs_adlog',fields: ['sid', 'refer', 'url', 'ncname', 'username', 'sip', 'mac', 'adcod', 'apname', 'apssid', 'phtype', 'os', 'sptype', 'devtype', 'browser', 'areacode', 'areaname', 'countrycode', 'lasttime', 'lcount'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_Place_advertisement_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'sid',direction: 'DESC'}]});
	store_logs_adlog.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_logs_adlog.new_params);
    });	
	
//������־
var store_logs_smslog = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_logs_smslog',fields: ['sid', 'servicecode', 'groupname', 'userid', 'username', 'usermac', 'ip', 'logtime', 'smsname', 'sptype', 'areacode', 'areaname', 'contrycode', 'status', 'apmac'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_Place_smslog_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'sid',direction: 'DESC'}]});
	store_logs_smslog.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_logs_smslog.new_params);
    });
    
//�澯��־   
var store_logs_alarmlog = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_logs_alarmlog',fields: ['sid', 'portalname', 'userid', 'groupid', 'groupname', 'username', 'dispname', 'ssid', 'apname', 'mac', 'ip', 'dev', 'os', 'bro', 'devtype', 'ubytes', 'dbytes', 'starttime', 'endtime', 'conntime', 'cause'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_Place_alarmlog_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'sid',direction: 'DESC'}]});
	store_logs_alarmlog.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_logs_alarmlog.new_params);
});	

//MAC��־   
var store_logs_maclog = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_logs_maclog',fields: ['pname', 'mac', 'apmac', 'groupname', 'groupcode', 'mark', 'lrssi', 'urssi', 'starttime', 'endtime', 'flags'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_Place_maclog_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'pname',direction: 'DESC'}]});
	store_logs_maclog.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_logs_maclog.new_params);
});			
	
/*����*/

//���ŷ��ͱ���   
var store_reports_smsreport = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_reports_smsreport',fields: ['timeV', 'localNum', 'interNum'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_App_smsStatisticsReport',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: []});
	store_reports_smsreport.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_reports_smsreport.new_params);
    });		
//��������
var store_reports_smsdetailreport = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_reports_smsdetailreport',fields: ['contrycode', 'username', 'usermac', 'ip', 'logtime', 'sptype', 'areaname', 'apmac'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_App_smsDetailReport',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'logtime',direction: 'DESC'}]});
	store_reports_smsdetailreport.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_reports_smsdetailreport.new_params);
    });	
//����������������
var store_reports_webreport = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_reports_webreport',fields: ['time', 'number', 'traffic'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_App_intStatisticsReport',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: []});
	store_reports_webreport.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_reports_webreport.new_params);
    });		
//��������
var store_reports_webdetailreport = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_reports_webdetailreport',fields: ['username', 'mac', 'ip', 'dev', 'os', 'starttime', 'endtime', 'traffic'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_App_intDetailReport',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'starttime',direction: 'DESC'}]});
	store_reports_webdetailreport.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_reports_webdetailreport.new_params);
    });	
//�豸���鱨��
var store_reports_devicereport = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_reports_devicereport',fields: ['dev', 'devtype', 'num'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_App_intDeviceReport',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: '',direction: 'DESC'}]});
	store_reports_devicereport.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_reports_devicereport.new_params);
    });	
//SSIDͳ�Ʊ���
var store_reports_ssidreport = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_reports_ssidreport',fields: ['ssid', 'num'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_App_intSsidReport_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'starttime',direction: 'DESC'}]});
	store_reports_ssidreport.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_reports_ssidreport.new_params);
    });	
//�û�����ͳ�Ʊ���
var store_reports_trafficreport = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_reports_trafficreport',fields: ['username', 'ssid', 'traffic'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_App_intTrafReport_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: '',direction: 'DESC'}]});
	store_reports_trafficreport.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_reports_trafficreport.new_params);
    });
// ĳһʱ��AP��MAC����
var store_reports_macreport = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_reports_macreport',fields: ['pname', 'apmac', 'groupname', 'groupcode', 'mark', 'stime', 'maccount'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_App_apMacCount_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'stime',direction: 'DESC'}]});
	store_reports_macreport.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_reports_macreport.new_params);
    });
// һ����һ�˵����ô���-0827
var store_reports_usercountreport = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_reports_usercountreport',fields: ['username', 'groupname', 'groupcode', 'usercount', 'conntime'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_App_userCount_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'username',direction: 'DESC'}]});
	store_reports_usercountreport.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_reports_usercountreport.new_params);
    });
/*ͼ��*/

//�豸Ʒ��   
var store_charts_devicebrand = Ext.create('Ext.data.Store', {pageSize: 10,id:'store_charts_devicebrand',fields: [ {name:'chart_name',type:'string'}, 'inum', 'id', {name:'chart_number',type:'int'}, {name:'nets_subytes2',type:'string'},{name:'rate',type:'string'}],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_DeviceBrandChart',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: []});
	store_charts_devicebrand.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_charts_devicebrand.new_params);
    });	
//����ն�Ʒ��   
var store_charts_addevicebrand = Ext.create('Ext.data.Store', {pageSize: 10,id:'store_charts_addevicebrand',fields: [ {name:'chart_name',type:'string'}, 'inum', 'id', {name:'chart_number',type:'int'}, {name:'nets_subytes2',type:'string'},{name:'rate',type:'string'}],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_Ad_DeviceBrandChart',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: []});
	store_charts_addevicebrand.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_charts_addevicebrand.new_params);
    });
//�豸����   
var store_charts_devicetype = Ext.create('Ext.data.Store', {pageSize: 10,id:'store_charts_devicetype',fields: [ {name:'chart_name',type:'string'}, 'inum', 'id', {name:'chart_number',type:'int'}, {name:'nets_subytes2',type:'string'},{name:'rate',type:'string'}],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_DeviceTypeChart',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: []});
	store_charts_devicetype.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_charts_devicetype.new_params);
    });	
//�̼Ҷ��� 
var store_charts_groupsms = Ext.create('Ext.data.Store', {pageSize: 10,id:'store_charts_groupsms',fields: [ {name:'chart_name',type:'string'}, 'inum', 'id', {name:'chart_number',type:'int'}, {name:'nets_subytes2',type:'string'},{name:'rate',type:'string'}],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_GroupSmsChart',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: []});
	store_charts_groupsms.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_charts_groupsms.new_params);
    });	
//�ն˲���ϵͳ   
var store_charts_deviceos = Ext.create('Ext.data.Store', {pageSize: 10,id:'store_charts_deviceos',fields: [ {name:'chart_name',type:'string'}, 'inum', {name:'chart_number',type:'int'}, {name:'rate',type:'string'}
],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_Place_os_chart',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: []});
	store_charts_deviceos.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_charts_deviceos.new_params);
    });
//�����   
var store_charts_advisit = Ext.create('Ext.data.Store', {pageSize: 10,id:'store_charts_advisit',fields: [ {name:'chart_name',type:'string'}, 'inum', 'id', {name:'chart_number',type:'int'}, {name:'nets_subytes2',type:'string'},{name:'rate',type:'string'}
],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_Ad_AdVisit',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: []});
	store_charts_advisit.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_charts_advisit.new_params);
    });		
//��������  
var store_charts_smsrecount1 = Ext.create('Ext.data.Store', {pageSize: 24,id:'store_charts_smsrecount1',fields: [ {name:'chart_name',type:'string'}, 'inum', 'id', {name:'chart_number',type:'int'}, {name:'nets_subytes2',type:'string'},{name:'rate',type:'string'}
],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_SMSRecountChart',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: []});
	store_charts_smsrecount1.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_charts_smsrecount1.new_params);
    });	
//��������  
var store_charts_smsrecount2 = Ext.create('Ext.data.Store', {pageSize: 24,id:'store_charts_smsrecount2',fields: [ {name:'chart_name',type:'string'}, 'inum', 'id', {name:'chart_number',type:'int'}, {name:'nets_subytes2',type:'string'},{name:'rate',type:'string'}
],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_SMSRecountChart',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: []});
	store_charts_smsrecount2.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_charts_smsrecount2.new_params);
    });


//������Ϣ   
var store_online_onlineinfo = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_online_onlineinfo',fields: ['on_name', 'on_dispname', 'on_pass', 'on_ssid','on_apname','on_temtype','on_os','on_lang','on_login','on_byte','on_sdate','on_mac','on_caDev','on_ip','on_lasttime','on_apAddr'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncmWebShowOnlineUserInfo',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{
            property: 'on_lasttime',
            direction: 'ASC'
        }]});
        
	store_online_onlineinfo.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_online_onlineinfo.new_params);
    });	
//����Mac   
var store_online_onlinemac = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_online_onlinemac',fields: ['on_tMac', 'on_uMac', 'on_bSignal', 'on_sSignal','on_noise','on_channel','on_stime','on_etime'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncmShowOnlineAllMac',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{
            property: 'on_stime',
            direction: 'ASC'
        }]});
	store_online_onlinemac.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_online_onlinemac.new_params);
    });	
/*�̳ǹ���*/
//ģ��	
var store_portplate = Ext.create('Ext.data.Store',{pageSize: 10,fields: ['sid', 'platename','status'],idProperty: 'userid',autoLoad: false,id:'store_portplate',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_portplate',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: '',direction: 'DESC'}]});
	store_portplate.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_portplate.new_params);
       }); 


//������  
var store_shops_admanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_admanager',fields: ['iNum', 'sid', 'name', 'sgroupname', 'descr', 'imgpath', 'serno', 'floor', 'level', 'plevel', 'type', 'systime', 'shopname', 'plateid', 'flags', 'scode'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomindex_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'sid',direction: 'DESC'}]});
	store_shops_admanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_admanager.new_params);
    });	
//������2  (֣�ݵ���)
var store_shops_advmanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_advmanager',fields: ['flag', 'iNum', 'sid', 'name', 'sgroupid', 'descr', 'imgpath', 'serno', 'floor', 'level', 'plevel', 'type', 'systime', 'shopid', 'plateid', 'flags', 'img_long', 'img_width', 'label', 'scode'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomindex2_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'sid',direction: 'DESC'}]});
	store_shops_advmanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_advmanager.new_params);
  });	
//������3  (���̳�)
var store_shops_advmanager2 = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_advmanager2',fields: ['flag', 'iNum', 'sid', 'name', 'sgroupid', 'descr', 'imgpath', 'serno', 'floor', 'level', 'plevel', 'type', 'systime', 'shopid', 'plateid', 'flags', 'img_long', 'img_width', 'scode'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomindex3_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'sid',direction: 'DESC'}]});
	store_shops_advmanager2.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_advmanager2.new_params);
  });
//Ԥ԰������4  (��ԥ԰)
var store_shops_advmanager3 = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_advmanager3',fields: ['flag', 'iNum', 'sid', 'name', 'sgroupid', 'descr', 'imgpath', 'serno', 'floor', 'level', 'plevel', 'type', 'systime', 'shopid', 'plateid', 'flags', 'img_long', 'img_width', 'label', 'scode'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomindex4_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'sid',direction: 'DESC'}]});
	store_shops_advmanager3.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_advmanager3.new_params);
  });
//̫��������
var store_shops_advmanager5 = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_advmanager5',fields: ['iNum', 'sid', 'name', 'sgroupid', 'descr', 'imgpath', 'serno', 'floor', 'level', 'plevel', 'type', 'systime', 'shopid', 'plateid', 'flags', 'img_long', 'img_width', 'label'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomindex5_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'sid',direction: 'DESC'}]});
	store_shops_advmanager5.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_advmanager5.new_params);
  });
//�̵����  
var store_shops_shopmanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_shopmanager',fields: ['iNum', 'id', 'name', 'service_code', 'sgroupname', 'imgpath', 'floor', 'url', 'code', 'tel', 'address', 'level', 'plevel', 'type', 'stype', 'descr', 'detail', 'systime', 'flags', 'title'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomshop_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'id',direction: 'DESC'}]});
	store_shops_shopmanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_shopmanager.new_params);
    });
//Ʒ�ƹ���  
var store_shops_brandmanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_brandmanager',fields: ['sid', 'name', 'sgroupname', 'descr', 'imgpath', 'serno', 'floor', 'level', 'plevel', 'type', 'systime', 'shopid', 'shopname', 'flags'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcombrand_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'sid',direction: 'DESC'}]});
	store_shops_brandmanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_brandmanager.new_params);
    });
//��Ʒ����  
var store_shops_oditymanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_oditymanager',fields: ['sid', 'name', 'dispname', 'servicecode', 'indexname', 'tyname', 'shgroupid', 'imgpath', 'shopname', 'shimgpath', 'factory', 'type', 'url', 'floor', 'fldetail', 'fee', 'discount', 'code', 'tel', 'address', 'reserve', 'active', 'plevel', 'level', 'descr', 'detail', 'integral', 'coupon', 'atype', 'systime', 'flags'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomodity_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'sid',direction: 'DESC'}]});
	store_shops_oditymanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_oditymanager.new_params);
    });
//��Ա���� 
//�̳��û�����
var store_shops_shopusermanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_shopusermanager',fields: ['uid', 'username', 'dispname', 'groupname', 'proname', 'usermac', 'password', 'nickname', 'membernum', 'integral', 'coupon', 'address', 'decript', 'lcount', 'imgpath', 'addtime', 'lasttime', 'flags'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomshopuser_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'uid',direction: 'DESC'}]});
	store_shops_shopusermanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_shopusermanager.new_params);
    });
//���ѹ���
var store_shops_consumpmanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_consumpmanager',fields: ['cid', 'username', 'dispname', 'usermac', 'sname', 'sid', 'fee', 'date', 'uid', 'lasttime'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomconsump_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'cid',direction: 'DESC'}]});
	store_shops_consumpmanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_consumpmanager.new_params);
    });
//�Ż݄�����
var store_shops_couponmanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_couponmanager',fields: ['cid', 'fee', 'counum', 'addtime', 'imgpath', 'effetime'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomcoupon_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'cid',direction: 'DESC'}]});
	store_shops_couponmanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_couponmanager.new_params);
    });
//�Ż݄�ʹ�ù���
var store_shops_couponusemanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_couponusemanager',fields: ['id', 'scode', 'username', 'aid', 'aname', 'serno', 'usermac', 'sname', 'feetime', 'uid', 'sid', 'fee', 'imgpath', 'systime', 'flags'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomcouponuse_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'id',direction: 'DESC'}]});
	store_shops_couponusemanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_couponusemanager.new_params);
    });
//�û���ע����
var store_shops_shopattentmanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_shopattentmanager',fields: ['id', 'servicecode', 'username', 'uid', 'activename', 'indexname', 'shopid', 'shopname', 'integral', 'decript', 'imgpath', 'addtime', 'lasttime', 'flags'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomshopattent_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'id',direction: 'DESC'}]});
	store_shops_shopattentmanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_shopattentmanager.new_params);
    });
//�û��ղع���
var store_shops_shopcollectmanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_shopcollectmanager',fields: ['id', 'servicecode', 'username', 'nickname', 'membernum', 'uid', 'activename', 'odityname', 'decript', 'imgpath', 'addtime', 'lasttime', 'flags'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomshopcollect_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'id',direction: 'DESC'}]});
	store_shops_shopcollectmanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_shopcollectmanager.new_params);
    });

//�����  
//���������ҳ
var store_shops_activemanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_activemanager',fields: ['id', 'servicecode', 'aname', 'indexname', 'odityname', 'couponfee', 'discount', 'address', 'decript', 'imgpath', 'plevel', 'type', 'xid', 'stime', 'etime', 'flags'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomactive_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'id',direction: 'DESC'}]});
	store_shops_activemanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_activemanager.new_params);
    });
//���������ϸ
var store_shops_seriesmanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_seriesmanager',fields: ['id', 'servicecode', 'aname', 'indexname', 'odityname', 'couponfee', 'discount', 'address', 'decript', 'imgpath', 'plevel', 'type', 'xid', 'stime', 'etime', 'flags'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomseries_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'id',direction: 'DESC'}]});
	store_shops_seriesmanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_seriesmanager.new_params);
    });

//�̱�������  
//��Ʒ�������
var store_shops_groupmanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_groupmanager',fields: ['iNum', 'groupid', 'name', 'descr', 'systime', 'imgpath', 'flags'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomgroup_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'groupid',direction: 'DESC'}]});
	store_shops_groupmanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_groupmanager.new_params);
    });
//��ƷС�����
var store_shops_typemanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_typemanager',fields: ['sid', 'name', 'groupname', 'descr', 'systime', 'imgpath', 'flags'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomtype_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'sid',direction: 'DESC'}]});
	store_shops_typemanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_typemanager.new_params);
    });

// ��Ʒ���ձ�
var store_shops_compmanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_compmanager',fields: ['id', 'shopid', 'shopname', 'sgroupid', 'sgroupname', 'level', 'etime'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcomcomp_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'id',direction: 'DESC'}]});
	store_shops_compmanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_compmanager.new_params);
    });
//����֧������
var store_shops_smsmanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_smsmanager',fields: ['id', 'sid', 'name', 'desc','count','systime', 'moneyflag', 'flag'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_smspay',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'id',direction: 'DESC'}]});
	store_shops_smsmanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_smsmanager.new_params);
    });	
// ��ͼ��ǩ����
var store_shops_markmanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_markmanager',fields: ['sid', 'mark', 'mapid', 'sx','sy','ex', 'ey', 'sname'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webcommark_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'sid',direction: 'DESC'}]});
	store_shops_markmanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_markmanager.new_params);
    });	
//�̼����˹���
var store_shops_shoptypemanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_shops_shoptypemanager',fields: ['iNum', 'groupname', 'groupcode', 'curcode','systime', 'pname', 'pid'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_shopgroup',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'sid',direction: 'DESC'}]});
	store_shops_shoptypemanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_shops_shoptypemanager.new_params);
    });		
/*��������*/
//����ע������	
var store_rules_smsregister = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['sid', 'regnum', 'stime', 'etime', 'systime', 'flags'],autoLoad: false,id:'store_rules_smsregister',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_Place_placelimit_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'sid',direction: 'DESC'}]});
	store_rules_smsregister.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_rules_smsregister.new_params);
       }); 	
//�������������	
var store_rules_smsnumber = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['sid', 'localnum', 'internum', 'smsflag', 'stime', 'etime', 'systime', 'flags'],autoLoad: false,id:'store_rules_smsnumber',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_Place_smslimit_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'sid',direction: 'DESC'}]});
	store_rules_smsnumber.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_rules_smsnumber.new_params);
       }); 	
//����ʱ������	
var store_rules_timelimit = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['sid', 'intertime', 'freetime', 'idletime', 'timeout', 'systime', 'flags'],autoLoad: false,id:'store_rules_timelimit',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_Place_timelimit_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'sid',direction: 'DESC'}]});
	store_rules_timelimit.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_rules_timelimit.new_params);
       }); 
//���Ÿ�ʽ����	
var store_rules_smsformat = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['sid', 'content', 'language', 'systime', 'flags'],autoLoad: false,id:'store_rules_smsformat',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_Place_smsformat_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'sid',direction: 'DESC'}]});
	store_rules_smsformat.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_rules_smsformat.new_params);
       }); 	   
//����������	
var store_rules_blackuser = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['utype', 's_utype', 'groupid', 'name', 'memo', 'validtime'],autoLoad: false,id:'store_rules_blackuser',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_blackuser',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'name',direction: 'DESC'}]});
	store_rules_blackuser.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_rules_blackuser.new_params);
       }); 
//�ڰ���������	
// ȫ��
var store_rules_white = Ext.create('Ext.data.Store',{   
	pageSize: 500,
	fields: ['pid', 'ptype', 'wtype', 'nametype', 'allname'],
	autoLoad: false,
	id:'store_rules_white',
	remoteSort: true,
  proxy: {
  	type: 'ajax',
  	url: '/pronline/Msg?FunName@ncm_web_Place_white_list',
  	reader: {
  		type:'json',
  		root: 'eimdata',
  		totalProperty: 'totalCount'
  	},
  	simpleSortMode: true
  },
  sorters:[{
  	property: 'name',
  	direction: 'DESC'
  }]
});
store_rules_white.on('beforeload', function (store, options) {
     Ext.apply(store.proxy.extraParams, store_rules_white.new_params);
});
// group
var store_rules_white1 = Ext.create('Ext.data.Store',{   
	pageSize: 500,
	fields: ['pid', 'ptype', 'wtype', 'nametype', 'allname'],
	autoLoad: false,
	id:'store_rules_white1',
	remoteSort: true,
  proxy: {
  	type: 'ajax',
  	url: '/pronline/Msg?FunName@ncm_web_Place_white_list',
  	reader: {
  		type:'json',
  		root: 'eimdata',
  		totalProperty: 'totalCount'
  	},
  	simpleSortMode: true
  },
  sorters:[{
  	property: 'name',
  	direction: 'DESC'
  }]
});
store_rules_white1.on('beforeload', function (store, options) {
     Ext.apply(store.proxy.extraParams, store_rules_white1.new_params);
});
// ssid
var store_rules_white2 = Ext.create('Ext.data.Store',{   
	pageSize: 500,
	fields: ['pid', 'ptype', 'wtype', 'nametype', 'allname'],
	autoLoad: false,
	id:'store_rules_white2',
	remoteSort: true,
  proxy: {
  	type: 'ajax',
  	url: '/pronline/Msg?FunName@ncm_web_Place_white_list',
  	reader: {
  		type:'json',
  		root: 'eimdata',
  		totalProperty: 'totalCount'
  	},
  	simpleSortMode: true
  },
  sorters:[{
  	property: 'name',
  	direction: 'DESC'
  }]
});
store_rules_white2.on('beforeload', function (store, options) {
     Ext.apply(store.proxy.extraParams, store_rules_white2.new_params);
});

//��������	
var store_rules_srvpar = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['pid','ptype','sessiontime','idletime','macbindtime','smslocalnum','smsinternum','smsusetime','limittime','limitunit','passcodetime','servicetime','eng_smsmessage','zh_smsmessage','autoauth','macauth','weixinauth','maxmac','errornum','errorlock','addtime','moditime','paroption','flags'],autoLoad: false,id:'store_rules_srvpar',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_srvpar',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: '',direction: 'DESC'}]});
	store_rules_srvpar.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_rules_srvpar.new_params);
       }); 
	   
//ģ�����	
var store_rules_plate = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['plateid', 'shopplateid', 'groupcode', 'dispname', 'platename', 'platepath', 'addtime', 'moditime'],autoLoad: false,id:'store_rules_plate',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_Place_plate_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'addtime',direction: 'DESC'}]});
	store_rules_plate.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_rules_plate.new_params);
       }); 

//ģ�����͹���	
var store_rules_plist = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['plateid', 'termtype', 'lang', 'pageid', 'pagename', 'pagepath', 'addtime', 'moditime'],autoLoad: false,id:'store_rules_plist',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_Place_plist_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'termtype',direction: 'DESC'}]});
	store_rules_plist.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_rules_plist.new_params);
       }); 
       
//ģ��ҳ�����	
/*var store_rules_page = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['pageid', 'pagename', 'pagepath', 'addtime', 'moditime'],autoLoad: false,id:'store_rules_page',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_Place_page_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'addtime',direction: 'DESC'}]});
	store_rules_page.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_rules_page.new_params);
       });*/ 
       
//ģ���������	
var store_rules_prule = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['ruleid', 'priority', 'plateid', 'platename', 'groupid', 'dispname', 'ssid', 'nasid', 'mark', 'dotime', 'dateid', 'datename', 'starttime', 'endtime', 'addtime', 'moditime', 'status', 's_status', 'groupcode'],autoLoad: false,id:'store_rules_prule',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_Place_prule_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'addtime',direction: 'DESC'}]});
	store_rules_prule.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_rules_prule.new_params);
       }); 
/*�û�����*/
//Ա������	
var store_users_usermanager = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['iNum', 'userid', 'username', 'dispname', 'groupname', 'groupid', 'ssid', 'proname', 'openid', 'usermac', 'macnum', 'password', 'mark', 'addtime', 'moditime', 'lasttime', 'expiretime', 'usertype', 'flags', 'maxbindmac'],autoLoad: false,id:'store_users_usermanager',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncmWebShowUserMan',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'lasttime',direction: 'DESC'}]});
	store_users_usermanager.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_users_usermanager.new_params);
       }); 
//�û�����	
var store_users_staffmanager = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['iNum', 'userid', 'username', 'dispname', 'groupname', 'groupid', 'ssid', 'proname', 'openid', 'usermac', 'macnum', 'password', 'mark', 'addtime', 'moditime', 'lasttime', 'expiretime', 'usertype', 'flags', 'maxbindmac'],autoLoad: false,id:'store_users_staffmanager',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncmWebShowStaffMan',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'lasttime',direction: 'DESC'}]});
	store_users_staffmanager.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_users_staffmanager.new_params);
       }); 
//�û�MAC����
var store_users_usermacmanager = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['usermac', 'username', 'groupid', 'starttime', 'lasttime', 'moditime', 'flags', 's_flags'],autoLoad: false,id:'store_users_usermacmanager',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webusermac_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'moditime',direction: 'DESC'}]});
	store_users_usermacmanager.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_users_usermacmanager.new_params);
       }); 
//��ǩ����	
var store_users_tagmanager = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['groupname', 'dispname', 'sessiontime', 'idletime', 'macbindtime', 'moditime'],autoLoad: false,id:'store_users_tagmanager',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncmWeb_groupList',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'groupname',direction: 'DESC'}]});
	store_users_tagmanager.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_users_tagmanager.new_params);
       }); 
//��λ����	
var store_users_groupmanager = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['groupid', 'groupname', 'dispname', 'moditime', 'flags', 'areaname', 's_flags', 'count'],autoLoad: false,id:'store_users_groupmanager',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_websrvgroup_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'moditime',direction: 'DESC'}]});
	store_users_groupmanager.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_users_groupmanager.new_params);
       }); 		
//AP����	
var store_users_apmanager = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['groupid', 'gwid', 'address', 'longitude', 'latitude', 'addtime', 'flags', 's_flags'],autoLoad: false,id:'store_users_apmanager',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_websrvgroupap_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'addtime',direction: 'DESC'}]});
	store_users_apmanager.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_users_apmanager.new_params);
       }); 
//SSID����	
var store_users_ssidmanager = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['sid', 'groupid', 'groupname', 'ssid', 'addtime', 's_addtime', 'moditime', 'flags', 's_flags'],autoLoad: false,id:'store_users_ssidmanager',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_websrvgroupssid_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'addtime',direction: 'DESC'}]});
	store_users_ssidmanager.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_users_ssidmanager.new_params);
       });  
/*�ҵ��˺�*/
//�޸�����

/*ϵͳ����*/
// ΢������
var store_settings_weixin = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['wid', 'name', 'token', 'account', 'appid', 'appsecret', 'sendcont', 'redurl', 'usinfo', 'access_token', 'systime', 'status', 's_status'],autoLoad: false,id:'store_settings_weixin',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_webweixin_list',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'wid',direction: 'DESC'}]});
	store_settings_weixin.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_settings_weixin.new_params);
       });
// ��������
var store_settings_par = Ext.create('Ext.data.Store',{pageSize: 25,fields: ['pid', 'ptype', 'sessiontime', 'idletime', 'macbindnum', 'smslocalnum', 'smsinternum', 'smsusetime', 'limittime', 'limitunit', 'passcodetime', 'servicetime', 'eng_smsmessage', 'zh_smsmessage', 'autoauth', 'macauth', 'weixinauth', 'maxmac', 'errornum', 'errorlock', 'addtime', 'moditime', 'paroption', 'flags'],autoLoad: false,id:'store_settings_par',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_web_Place_par_info',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: 'wid',direction: 'DESC'}]});
	store_settings_par.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_settings_par.new_params);
 });
/*ϵͳ����*/
//��λ�����
var store_systems_shopgtypemanager = Ext.create('Ext.data.Store', {pageSize: 25,id:'store_systems_shopgtypemanager',fields: ['iNum', 'groupname', 'groupcode', 'systime', 'id','type','autodef'],remoteSort: true,autoLoad: false,
			      proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_shopgtype',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
			      },sorters: [{property: 'id',direction: 'DESC'}]});
	store_systems_shopgtypemanager.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_systems_shopgtypemanager.new_params);
    });
  Ext.define('treedata',{
        extend: 'Ext.data.Model',
        fields: [
            {name: 'text',     type: 'string'},
            {name: 'id',     type: 'string'},
            {name: 'groupcode', type: 'string'},
            {name:'ctext',   type:'string'}

        ]
       });
    var   store_systems_autopri = Ext.create('Ext.data.TreeStore', {
   	    model:'treedata',
        proxy: {
            type: 'ajax',
          //  url: '/pronline/Msg?FunName@lanTree_depComp&state@1&plate@lan/lan_depcompute_check_tree.htm'
            url: '/pronline/Msg?FunName@ncm_autopri'
        },
        root: {
            text: 'ȫ��',
            id: '',
            expanded: true
        },
        folderSort: true,
        sorters: [{
 //           property: 'text',
//            direction: 'ASC'
        }]
    });

var admincheck;
var autodef;
var isexp;

var xmlHttp = false;
//����XMLHttpRequest����       
function createXMLHttpRequest() {
	var C=null;
		if(window.XMLHttpRequest) { //Mozilla �����
				C = new XMLHttpRequest();
		}else if (window.ActiveXObject) { // IE�����
				try {
						C = new ActiveXObject("Msxml2.XMLHTTP");
				} catch (e) {
						try {
								C = new ActiveXObject("Microsoft.XMLHTTP");
						} catch (e) {}
				}
		}
		return C;
}
//�����ַ���  ins-�����ַ�����   names-��ʼ�ַ��� namee -�����ַ��� ����ֵ ��names ��namee ֮����ַ��� �����ڽ����򵥵�XML
function getxmlbyname(ins,names,namee){
	var start;
	var end;
	var ret='';

	start=ins.indexOf(names);
	end=ins.indexOf(namee);
	
	if(start!=-1&&end!=-1){
		ret=ins.substr(start+names.length,end-start-names.length);
	}
	return ret;	
}

//����AJAX
function callServer() { 
	  if( xmlHttp && xmlHttp .readyState != 0 ){
	    	xmlHttp.abort();
	  }
  
   	xmlHttp = createXMLHttpRequest();
 
	  if(xmlHttp){
		    var web_url='/pronline/Msg?FunName@ncmWebAdmin&xx@'+Math.random()*999;
		    //������
		    xmlHttp.open("GET",web_url, true);
		    //���ûص�����
		    xmlHttp.onreadystatechange = updatePage;
		    xmlHttp.send(null);
    }
}

callServer();
//ajax�ص�������
function updatePage() {
   if (xmlHttp.readyState == 4) {
	    var response = xmlHttp.responseText;
	    var temp_name=getxmlbyname(response,"<username>","</username>");
	    var temp_autodef=getxmlbyname(response,"<autodef>","</autodef>");
	    var temp_isexp=getxmlbyname(response,"<isexp>","</isexp>");
	    admincheck=temp_name;
	    autodef=temp_autodef;
	    isexp=temp_isexp;
   }
}
	
//Ƥ������
var store_skintheme = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
	  		data:[
	  				{groupname:'������', groupid:'1'},
	  				{groupname:'̫����', groupid:'2'},
	  				{groupname:'��Ⱥ�', groupid:'3'},
	  				{groupname:'ǰ����', groupid:'4'},
	  		]
		});
//������		
var store_search = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
	  		data:[
	  				
	  				{groupname:'�û�����', groupid:'2'},
	  				{groupname:'������', groupid:'3'},
	  				{groupname:'�̵����', groupid:'4'},
	  				{groupname:'Ʒ�ƹ���', groupid:'5'},
	  				{groupname:'��Ʒ����', groupid:'6'},
	  				{groupname:'�������', groupid:'7'},
	  				{groupname:'С�����', groupid:'8'},
	  				{groupname:'��Ա����', groupid:'9'},
	  				{groupname:'���ѹ���', groupid:'10'},
	  				{groupname:'�Ż݄�����', groupid:'11'},
	  				{groupname:'�Ż݄�ʹ�ù���', groupid:'12'},
	  				{groupname:'�û���ע����', groupid:'13'},
	  				{groupname:'�û��ղع���', groupid:'14'},
	  				{groupname:'���������ҳ', groupid:'15'},
	  				{groupname:'���������ϸ', groupid:'16'},
	  				{groupname:'���ŷ��ͱ���', groupid:'17'},
	  				{groupname:'���ŷ�������', groupid:'18'},
	  				{groupname:'����������������', groupid:'19'},
	  				{groupname:'��������', groupid:'20'},
	  				{groupname:'�豸����ͳ��', groupid:'21'},
	  				{groupname:'�ն�Ʒ��', groupid:'22'},
	  				{groupname:'�ն�����', groupid:'23'},
	  				{groupname:'�ն˲���ϵͳ', groupid:'24'},
	  				{groupname:'�����', groupid:'25'},
	  				{groupname:'��������', groupid:'26'},
	  				{groupname:'ʱ��η���', groupid:'27'},
	  				{groupname:'ʵʱ�����������ͼ', groupid:'28'},
	  				{groupname:'������־', groupid:'29'},
	  				{groupname:'�û���־', groupid:'30'},
	  				{groupname:'�����־', groupid:'31'},
	  				{groupname:'�������������', groupid:'32'},
	  				{groupname:'����ע������', groupid:'33'},
	  				{groupname:'����ʱ��', groupid:'34'},
	  				{groupname:'���Ÿ�ʽ', groupid:'35'},
	  				{groupname:'������', groupid:'36'},
	  				{groupname:'��λ����', groupid:'37'},
					{groupname:'ϵͳȨ�޹���', groupid:'38'},
					{groupname:'ϵͳ�û�����', groupid:'39'},
					{groupname:'�û�������־', groupid:'40'},
					{groupname:'�޸�����', groupid:'41'},
	  		]
		});