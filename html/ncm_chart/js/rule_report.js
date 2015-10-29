var week_list = new Array();
var start_date = 0;
var stop_date  = 0;
var start_hour = 0;
var start_minute = 0;
var stop_hour  = 0;
var stop_minute = 0;
var rule_sdate = 0;
var rule_edate = 0;
var is_export = 0; 
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
var binary_week = "";
var start_sday = 0;
    
var stop_sday = 0;

var store_logs_userlog_rule = Ext.create('Ext.data.Store', {id:'store_logs_userlog_rule',fields: ['sid', 'portalname', 'userid', 'groupid', 'groupname', 'username', 'dispname', 'ssid', 'apname', 'mac', 'ip', 'dev', 'os', 'bro', 'devtype', 'ubytes', 'dbytes', 'starttime', 'endtime', 'conntime', 'cause','logintimes'],
                                   proxy:
{
type: 'ajax'
,url: '/pronline/Msg?FunName@ncm_web_Place_smslog_rule'
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
//    	alert("自定义时间有误");
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
    start_sday = start_hour*3600+start_minute*60;
    
    stop_sday  = stop_hour*3600+stop_minute*60;
    
    if (start_date == "" || stop_date == "")
    {
//        alert("请先输入日期");
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
        if(rule_sdate > rule_edate || rule_sdate ==0 || rule_edate ==0)
        {
//        	alert("输入开始日期和结束日期有误");
        	return;	
        }
		}
	//取得所选的星期数
	var week=document.getElementsByName("week");
	binary_week = "";	
	for(var i=0;i<week.length;i++)
	{
			if(week[i].checked==true)
			{
			  binary_week += 1;		  
			}
			else
			{
				binary_week += 0;	
			}	
	}
	if(binary_week == "0000000")
	{
		binary_week = "1111111";	
	}

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
					 binary_week:binary_week,
					 start_sday:start_sday,
					 stop_sday:stop_sday,
					 is_export:is_export									
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
//                

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
                AllPage = Math.ceil(all_array.length/15);
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

window.onload = function()
{
		Ext.Ajax.timeout = 180000;
};


function Loadexport()
{
	
	is_export = 1;
	document.getElementById("sdate").value = rule_sdate;
	document.getElementById("edate").value = rule_edate;
	document.getElementById("binary_week").value = binary_week;
	document.getElementById("start_sday").value = start_sday;
	document.getElementById("stop_sday").value = stop_sday;
	document.getElementById("is_export").value = is_export;
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
    if(all_array.length < 15)
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
    for (var i = 15*PageNum ; i < 15*(PageNum+1) ; i++ )
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


