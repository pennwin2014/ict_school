var DEFAULT_SEARCH_TEXT = "MAC/�ֻ���/��������";

function f_IctGlobalContext(){
//���б���
	var globalSelectId = "";
	var globalSelectGroupId = "itm_portal";
	
	var globalTime = "";
	
//˽�б���
	this.id_operation_status = -1;
	this.globalInfoMac = "";
	this.globalInfoTime = "";
	this.phoneNumber = ""; 
	this.placeData = [];
	this.placeName = "";
	this.alarmReason = -1;
	this.mapOnlineFlag = false;

//�����ӿ�
	this.setMapOnlineFlag = function(v){
		this.mapOnlineFlag = v;
	}
	this.getMapOnlineFlag = function () {
		if(this.mapOnlineFlag)
			return this.mapOnlineFlag;
		else
		{
			this.getAlarmNum();
			return this.mapOnlineFlag;
			
		}
	}
	this.setAlarmReason = function(v){
		this.alarmReason = v;
	}
	this.getAlarmReason = function(){
		return this.alarmReason;
	}
	this.getPlaceName = function(){
        return this.placeName;
    }
	this.setPlaceName = function(data){
		this.placeName = data;
	}
	this.getPlaceData = function(){
        return this.placeData;
    }
	this.setPlaceData = function(data){
		this.placeData = data;
	}
	this.getIdOperationStatus = function(){
        return this.id_operation_status;
    }
	this.setIdOperationStatus = function(status){
		this.id_operation_status = status;
	}
	this.getGlobalInfoMac = function(){
		return this.globalInfoMac;
	}
	this.getGlobalInfoTime = function(){
		return this.globalInfoTime;
	}
	this.setGlobalInfoMac = function(mac){
		this.globalInfoMac = mac;		
	}
	this.setGlobalInfoTime = function(tm){
		this.globalInfoTime = tm;		
	}
	this.getPhoneNumber = function(){
		return this.phoneNumber;
	}
	this.setPhoneNumber = function(number){
		this.phoneNumber = number;
	}
	this.notExists = function(value){
		if (!value || typeof(value)=="undefined" || value==0){
			return true;
		}else{
			return false;
		}
	}
//���߷���
	/**
	* ���ã��ж�Զ���ļ��Ƿ����
	* �������ļ��ĵ�ַ
	* ����ֵ�� true--���ڣ�false--������
	*/
	this.isRemoteFileExist = function(fileUrl){
		Ext.Ajax.request({
		   url:""+fileUrl,
		   async: false,
		   type:'HEAD',
		   error: function() {
			   return false;
		   },
		   success: function() {
			  return true;
		   }
		});
	}
	
	this.doJumpFunctionsByItemid = function(itemid, value){
		try{
			eval("lan_"+itemid+"_s").jumpPageFunc1(value);
		}catch(e){
			
		}
		try{
			eval("lan_"+itemid+"_s").jumpPageFunc2(value);
		}catch(e){
			
		}
		try{
			eval("lan_"+itemid+"_s").jumpPageFunc3(value);
		}catch(e){
			
		}
		
	}
	
    this.isInFocus = function(itmid){
		var tabs_center=Ext.getCmp("layout_center");
		var activeTab = tabs_center.getActiveTab();
		//if (activeTab || (typeof activeTab == "undefined")){
		if (!activeTab || typeof(activeTab)=="undefined" || activeTab==0){
			if(tabs_center.items.length>0){
				activeTab = tabs_center.items.items[0];
			}else{
				return false;
			}
		}
		if(activeTab.id == itmid){
			return true;
		}else{
			return false;
		}
	}

	this.getLocalTime = function(tm) {
		var unixTimestamp = new Date(parseInt(tm) * 1000);
		commonTime = unixTimestamp.toLocaleString();
		return commonTime;
	}
//У��ķ���
	this.CheckMail = function (mail){
		var filter  = /^([a-zA-Z0-9_\.\-])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$/;
		if (filter.test(mail)) 
			return true;
		else {
			alert('���ĵ����ʼ���ʽ����ȷ');
			return false;
		}
	}
	this.CheckPhone = function(Phone) {
		var filter=/^1\d{10}$|^(0\d{2,3}-?|\(0\d{2,3}\))?[1-9]\d{4,7}(-\d{1,8})?$/;
		if (filter.test(Phone)){
			return true;
		}
		else {
			alert('���ĵ绰��ʽ����ȷ');
			return false;}
	}
}
















