
var rule_sdate = 0;
var rule_edate = 0;
var start_date = 0;
var stop_date  = 0;
var start_hour = 0;
    
var start_minute = 0;
var stop_hour  = 0;
var stop_minute = 0;
var ary_count = new Array();
var all_array = new Array();
var ary_name = new Array();
var t = 0;
var temp;
var PageNum	 = 0;
var AllPage = 0; 
var Allcount = 0;
var start_moods = 0;
var stop_moods = 0;
var time_online = 0;

var slunch_time = 0;
var elunch_time = 0;

var sdinner_time = 0;
var edinner_time = 0;

var scustom_time = 0;
var ecustom_time = 0;

var is_export = 0; 

var apinfo_list = new Array();
var apinfo_poc = new Array();
var macstr = "resetmac";
var pocname = "";
var district_list = new Array();
var district_cn = "";

window.onload = function()
{
	Loadapinfo();
}	   

var store_logs_userlog_rule = Ext.create('Ext.data.Store', {id:'store_logs_userlog_rule',fields: ['sid', 'portalname', 'userid', 'groupid', 'groupname', 'username', 'dispname', 'ssid', 'apname', 'mac', 'ip', 'dev', 'os', 'bro', 'devtype', 'ubytes', 'dbytes', 'starttime', 'endtime', 'conntime', 'cause','logintimes'],
                                   proxy:
{
type: 'ajax'
,url: '/pronline/Msg?FunName@ncm_web_Place_smslog_custmer'
,reader:
    {
type:'json'
,root: 'eimdata'
,totalProperty: 'totalCount'
    }
}
,sorters: []
                                                                });
function reflash()
{		
		//取到开始时间和结束时间
		 start_date = document.getElementById('start_txt').value;
     stop_date  = document.getElementById('stop_txt').value;
     start_hour = document.getElementById("start_hour").value;
     start_minute = document.getElementById("start_minute").value;
     stop_hour  = document.getElementById("stop_hour").value;
     stop_minute = document.getElementById("stop_minute").value;
    if(start_hour < 0 ||start_minute < 0 || stop_hour < 0 ||stop_minute < 0 || start_hour > stop_hour)
    {
//    	alert(自定义时间有误);
    	return;	
    }
    
		    //取到次数范围
    start_moods = document.getElementById("start_moods").value;
    if(start_moods == "")
    {
    		start_moods = 0;	
    }
    start_moods = parseInt(start_moods);
    stop_moods  = document.getElementById("stop_moods").value;
    if(stop_moods == "")
    {
    	stop_moods = 0;	
    }
    stop_moods  = parseInt(stop_moods);

    if(stop_moods != 0 && start_moods != 0)
    {
    if(start_moods > stop_moods || stop_moods < 0 || start_moods<0)
    {
//    	alert("输入次数有误");
    	return;	
    }
    }
    
    time_online = document.getElementById("time_online").value;
    time_online = time_online*60;
    if(time_online < 0)
    {
//    	alert("上网时间不能为负");
    	return;	
    }
    
    if (start_date == "" || stop_date == "")
    {
//      alert("请先输入日期");
      return;
    }
    else
    {
        var start_dates = start_date.split('/');
        var stop_dates = stop_date.split('/');
        var start_d = Date.UTC(start_dates[2],(start_dates[0]-1),start_dates[1]);

        var stop_d =Date.UTC(stop_dates[2],(stop_dates[0]-1),stop_dates[1]);

        rule_sdate =  start_d/1000;

        rule_edate =   stop_d/1000;
        rule_edate += 86400;
        if(rule_sdate > rule_edate || rule_sdate == 0 || rule_edate == 0)
        {
//        	alert("输入开始日期和结束日期有误");
        	return;	
        }
		}
	
	
	//取到用户所选的登录时间
	 slunch_time = 0;
   elunch_time = 0;

   sdinner_time = 0;
   edinner_time = 0;

   scustom_time = 0;
   ecustom_time = 0;
	var login_time = document.getElementsByName("logintime");
	for(var i=0;i<login_time.length;i++)
	{
			if(login_time[i].checked==true)
			{
			  if(0 == i)
			  {
			  	slunch_time = 11*3600;
			  	elunch_time = 14*3600;	
			  }
			  else if(1 == i)
			  {
			  	sdinner_time = 17*3600;
			  	edinner_time = 21*3600;	
			  }
			  else if(2 == i)
			  {
			  	 scustom_time = start_hour*3600+start_minute*60;
    
    			 ecustom_time  = stop_hour*3600+stop_minute*60;	
			  }	 
			}
			
	}
	
	SelectApIndex();
	ary_count = new Array();
  all_array = new Array();
	ary_name = new Array();
	Start_Loading();
	is_export = 0;
	store_logs_userlog_rule.load(
    {
params:
        {
           sdate:rule_sdate,
           edate:rule_edate,
           slunch_time:slunch_time,
           elunch_time:elunch_time,
           sdinner_time:sdinner_time,
           edinner_time:edinner_time,		
					 start_sday:scustom_time,
					 stop_sday:ecustom_time,
					 time_online:time_online,
					 is_export:is_export,
					 macstr:macstr									
        },
callback: function(records, options, success)
        {
						Stop_Loading();

            if (success)
            {
//            		alert("成功");
            		
                
                for (var k = 0 ; k < records.length;k++)
                {
                    	ary_name[k] = records[k].get("username");
											ary_count[k] = records[k].get("logintimes");
											all_array.push([ary_name[k],ary_count[k]]);
                }
//                ary_count.sort();
//
//                for (var i = 0;i<ary_count.length;)
//                {
//
//                    var count = 0;
//                    for (var j=i;j<ary_count.length;j++)
//                    {
//
//                        if (ary_count[i] == ary_count[j])
//                        {
//                            count++;
//                        }
//
//                    }
//
//                    all_array.push([ary_count[i],count]);
//                    i+=count;
//
//                }
 
               	if(stop_moods == 0 && start_moods == 0)
               	{
               	}
               	else
               	{	
                if(stop_moods > 0 && start_moods >= 0)
                {
                for(var k = all_array.length-1 ; k >=  0; k--)
                {
                	if(all_array[k][1] < start_moods || all_array[k][1] > stop_moods)
                	{
                		all_array.splice(k,1);	
                	}	
                }
                }
               
                if(stop_moods == 0 && start_moods >0)
                {
                	
                for(var k = all_array.length-1 ; k >=  0; k--)
                {
                	if(all_array[k][1] < start_moods)
                	{
                		all_array.splice(k,1);	
                	}	
                }
                }
               	
                }
                for (var i=0;i<all_array.length-1;i++)
                {
                    for (var j=i+1;j<all_array.length;j++)
                    {
                        if (all_array[i][1]<all_array[j][1])
                        {
                            temp=all_array[i];
                            all_array[i]=all_array[j];
                            all_array[j]=temp;
                        }
                    }
                }     
 
                
								Allcount = all_array.length;
                AllPage = Math.ceil(all_array.length/12);
                PageNum = 0;
                CreateRuleTab();
        		}
        		else
        		{
//        			 alert("获取失败");
        		}	
        }    	
   }); 
           	
}

function Loadexport()
{
	
	is_export = 1;
	document.getElementById("sdate").value = rule_sdate;
	document.getElementById("edate").value = rule_edate;
	document.getElementById("slunch_time").value = slunch_time;
	document.getElementById("elunch_time").value = elunch_time;
	document.getElementById("sdinner_time").value = sdinner_time;
	document.getElementById("edinner_time").value = edinner_time;
	document.getElementById("start_sday").value = start_sday;
	document.getElementById("stop_sday").value = stop_sday;
	document.getElementById("time_online").value = time_online;
	document.getElementById("is_export").value = is_export;
	document.getElementById("macstr").value = macstr;
	document.getElementById("formfrequency").submit();
}
//新建表格填充数据
function CreateRuleTab()
{
		t = 0;
		document.getElementById("nowPageNum").innerHTML = PageNum+1;
		document.getElementById("allPageNum").innerHTML = AllPage;
		document.getElementById("allcount").innerHTML = Allcount;
    var newTable = document.getElementById("user_count");
    newTable.innerHTML = "";
    var titletr  = document.createElement("tr");
    titletr.innerHTML = "<td>序号</td><td>手机号</td><td>到店次数</td>";
    newTable.appendChild(titletr);
    if(all_array.length < 12)
    {
    for (var i = 0; i < all_array.length ; i++ )
    {
        var newTr = document.createElement("tr");
        var newTd1= document.createElement("td");
        var newTd2= document.createElement("td");
        var newTd3= document.createElement("td");

        newTd1.innerHTML = ""+(++t)+"";
        
				
        newTd2.innerHTML = ""+all_array[i][0]+"";
        

        newTd3.innerHTML = ""+all_array[i][1]+"";

        newTr.appendChild(newTd1);
        newTr.appendChild(newTd2);
        newTr.appendChild(newTd3);

        newTable.appendChild(newTr);
    }    	
    }
    else
    {	
    for (var i = 12*PageNum ; i < 12*(PageNum+1) ; i++ )
    {
        var newTr = document.createElement("tr");
        var newTd1= document.createElement("td");
        var newTd2= document.createElement("td");
        var newTd3= document.createElement("td");

        newTd1.innerHTML = ""+(++t)+"";
       
				
        newTd2.innerHTML = ""+all_array[i][0]+"";

        newTd3.innerHTML = ""+all_array[i][1]+"";

        newTr.appendChild(newTd1);
        newTr.appendChild(newTd2);
        newTr.appendChild(newTd3);

        newTable.appendChild(newTr);
    }
  }
}

function Navigate(num)
{
    
    if (0 == num)
    {
        PageNum = 0;
    }
    else if (1 == num)
    {		
    		if(PageNum == AllPage-1)
    		{
    			PageNum = AllPage-1;
//    			alert("已经是最后一页了");
    			return;
    		}
        PageNum += num;
    }
    else if (-1 == num)
    {
    		if(PageNum == 0)
    		{
    			PageNum = 0;
//    			alert("已经是第一页了")	
    			return;
    		}
    		else
    		{	
        	PageNum += num;
      	}
    }
    else if (2 == num)
    {
				PageNum = AllPage-1;
    }
    
    CreateRuleTab();
}



var store_ap_info = Ext.create('Ext.data.Store', 
{
    id:'store_ap_info',
    fields: ['mac', 'pos'],
    proxy: 
    {
        type: 'ajax',
        url: '/pronline/Msg?FunName@query_ap_info',
        reader: 
        {
            type:'json',
            root: 'record'
        }
    },
    sorters: []
});


//去掉楼层数组重复值
function unique(arr) {
    var result = [], hash = {};
    for (var i = 0, elem; (elem = arr[i]) != null; i++) {
        if (!hash[elem]) {
            result.push(elem);
            hash[elem] = true;
        }
    }
    return result;

}
function Loadapinfo()
{
	
	apinfo_list = new Array();
	apinfo_poc = new Array();
	var storey = document.getElementById("storey");
    store_ap_info.load(
    {
        params:
        {
           status: 'l',
           mac: '',
           pos:'',
        },
        callback: function(records, options, success)
        {
        	if(success)
        	{
        			
        	
        		for(var i = 0; i < records.length ; i++)
        		{
        			apinfo_list.push([records[i].get("pos"),records[i].get("mac")]);
        			apinfo_poc[i] = records[i].get("pos"); 
        			

        		}
        		
        		apinfo_poc = unique(apinfo_poc);
        		for(var j = 0 ; j < apinfo_poc.length ; j++)
        		{
            	var check = document.createElement("div");
            	check.className = "item";
            	check.id = j;
        			check.innerHTML = "<label title=\""+apinfo_poc[j]+"\" class=\"titem\"><input  type=\"checkbox\" value=\""+j+"\"  title=\""+apinfo_poc[j]+"\" class=\"b\" />"+apinfo_poc[j]+"</label><div class=\"sitem\"></div>";
       			  storey.appendChild(check);		
        		}
        		var cleardiv = document.createElement("div");
        		cleardiv.innerHTML = "<div class=\"clear\"></div>";
        		storey.appendChild(cleardiv);
        		
        	}
        	else
        	{
        		alert("楼层加载失败");	
        	}	
        }	
    
    
    
    
    });

	
}

function SelectApIndex()
{
	macstr="";
	district_cn="";
  district_list = new Array();
	district_cn = document.getElementById("district_cn").value;
	
  district_list = district_cn.split("+");
	if(district_cn=="未选择"||district_cn=="请选择地址")
	{	
		macstr = "resetmac";	
		
  }
  else
  {
  	
  	for(var i = 0 ; i < apinfo_list.length ; i++)
		{
			for(var j = 0;j<district_list.length;j++)
			{
				if(apinfo_list[i][0] == district_list[j])
				{
					
					macstr += apinfo_list[i][1]+";"; 	
				}
				
			}
		}
		macstr=macstr.substring(0,macstr.length-1);
		
  }	
  
  			
}


