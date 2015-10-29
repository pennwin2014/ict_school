var userlog_time_list = ["1-5次","6-10次","11-20次","20次以上"];
var userlog_time_rate = new Array();
var frequency_sdate=0;
var frequency_edate=0;

var start_moods = 0;
var stop_moods = 0;

var store_reports_webreport_slogdate = "";
var store_reports_webreport_elogdate = "";
var reports_webreport_list = [];
var ary_count = new Array();
var ary_name = new Array();
var all_array =new Array();

var oneRate = "";
var twoRate = "";
var threeRate = "";
var fourRate = "";
var otherRate = "";

var t = 0;
var temp;
var PageNum	 = 0;
var AllPage = 0;
var allcount = 0;
var is_export = 0;
 

var excelstr;
var store_logs_userlog_frequency = Ext.create('Ext.data.Store', {id:'store_logs_userlog_frequency',fields: ['sid', 'portalname', 'userid', 'groupid', 'groupname', 'username', 'dispname', 'ssid', 'apname', 'mac', 'ip', 'dev', 'os', 'bro', 'devtype', 'ubytes', 'dbytes', 'starttime', 'endtime', 'conntime', 'cause','logintimes'],
                                   proxy:
{
type: 'ajax'
,url: '/pronline/Msg?FunName@ncm_web_Place_smslog_count'
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

    var start_date = document.getElementById('start_txt').value;
    var stop_date  = document.getElementById('stop_txt').value;
    if (start_date == "" || stop_date == "")
    {
//       alert("请先输入日期");
       
       return;
    }
    else
    {
        var start_dates = start_date.split('/');
        var stop_dates = stop_date.split('/');
        var start_d = Date.UTC(start_dates[2],(start_dates[0]-1),start_dates[1]);

        var stop_d =Date.UTC(stop_dates[2],(stop_dates[0]-1),stop_dates[1]);

        frequency_sdate =  start_d/1000;

        frequency_edate =   stop_d/1000;
       
       	frequency_edate += 86400;
       
				
    }
   
	 if(frequency_edate < frequency_sdate)
				{
//					alert("开始时间大于结束时间");
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
    
    ary_count = new Array();
    all_array = new Array();
    ary_name = new Array();
    userlog_time_rate = new Array();
    userlog_time_list = ["1-5次","6-10次","11-20次","20次以上"];
    Start_Loading();
    is_export = 0;
    store_logs_userlog_frequency.load(
    {
params:
        {
sdate:
            frequency_sdate,
edate:
            frequency_edate,
is_export:is_export

        },
callback: function(records, options, success)
        {

						//Stop_Loading();
            if (success)
            {

//                alert("成功");
                
                
                
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
               AllPage = Math.ceil(all_array.length/15);
               PageNum = 0;
               allcount = all_array.length;
                var total = all_array.length;
                var oneNum = 0;
                var twoNum = 0;
                var threeNum = 0;
                var fourNum = 0;
                var otherNum = 0;
                for (var i = 0 ; i < all_array.length ; i++)
                {
                    if (all_array[i][1]>0&&all_array[i][1]<6)
                    {
                        oneNum++;
                    }
                    else if(all_array[i][1]>5&&all_array[i][1]<11)
                    {
                        twoNum++;
                    }
                    else if(all_array[i][1]>10&&all_array[i][1]<21)
                    {
                        threeNum++;
                    }
                   
                    else if (all_array[i][1] > 20)
                    {
                        otherNum++;
                    }

                }

                oneRate = Math.round(oneNum/ total * 10000) / 100.00 + "%";
                twoRate = Math.round(twoNum/ total * 10000) / 100.00 + "%";
                threeRate = Math.round(threeNum/ total * 10000) / 100.00 + "%";
                otherRate = Math.round(otherNum/ total * 10000) / 100.00 + "%";
                userlog_time_rate = [oneRate,twoRate,threeRate,otherRate];
                
                show_device_brand_pie();
                CreateTab()
            }
            else
            {
//                alert("获取失败");
            }
        }
    });
}

function Loadexport()
{
	
	is_export = 1;
	document.getElementById("sdate").value = frequency_sdate;
	document.getElementById("edate").value = frequency_edate;
	document.getElementById("is_export").value = is_export;
	document.getElementById("formfrequency").submit();
}

window.onload = function()
{
		Ext.Ajax.timeout = 180000;


};

function CreateTab()
{		
		t = 0;
		document.getElementById("nowPageNum").innerHTML = PageNum+1;
		document.getElementById("allPageNum").innerHTML = AllPage;
		document.getElementById("allcount").innerHTML=allcount;
    var newTable = document.getElementById("user_count");
    newTable.innerHTML = "";
    var titletr  = document.createElement("tr");
    titletr.innerHTML = "<td>序号</td><td>手机号</td><td>到店次数</td>";
    newTable.appendChild(titletr);
    
		
		if(all_array.length < 15)
		{
		for (var i = 0 ; i < all_array.length; i++ )
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

function show_device_brand_pie()
{

    var data = new Array();
    var obj  = new Object();
    var color = ["#ed8c25","#e6a34c","#e6b87a","#e4cea7","#b9c1ce","#90a0c4","#6185b9","#ed8c25","#e6a34c","#e6b87a","#e4cea7","#b9c1ce","#90a0c4","#6185b9","#ed8c25","#e6a34c","#e6b87a","#e4cea7","#b9c1ce","#90a0c4","#6185b9","#ed8c25","#e6a34c","#e6b87a","#e4cea7","#b9c1ce","#90a0c4","#6185b9"]
                for (var j = 0 ; j < userlog_time_list.length ; j++)
    {
        obj = new Object();
        obj.name = userlog_time_list[j];
        obj.value= userlog_time_rate[j];
        obj.color= color[j];
        data[j]  = obj;

    }



    var chart = new iChart.Pie2D(
    {
render : 'pie',
data: data,
sub_option :
        {
label :
            {
background_color:
                null,
sign:
                true,//设置禁用label的小图标
padding:'0 4'
                ,
border:
                {
enable:
                    false,
color:'#666666'
                },
fontsize:
                14,
fontweight:
                600,
color : '#e68f1b'
                ,
font : '宋体'
            },
border :
            {
width :
                0,
color : '#fff'
            }
        },

shadow : false,
shadow_blur : 0,
shadow_color : '#aaaaaa',
shadow_offsetx : 0,
shadow_offsety : 0,
background_color:'#fefefe',
offsetx:10,//设置向x轴负方向偏移位置60px
offset_angle:0,//初始位置不偏转
showpercent:true,
decimalsnum:2,
width : 560,
height : 370,
radius: 100,
border: 0
    });

    chart.draw();
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
    
    CreateTab();
}

