
var userlog_time_rate = new Array();
var frequency_sdate=0;
var frequency_edate=0;

var start_moods = 0;
var stop_moods = 0;

var store_reports_webreport_slogdate = "";
var store_reports_webreport_elogdate = "";
var reports_webreport_list = [];
var ary_count = new Array();
var ary_type = new Array();
var all_array =new Array();



var t = 0;
var temp;
var PageNum	 = 0;
var AllPage = 0;
var is_export = 0;
 

var excelstr;
var store_website_visit = Ext.create('Ext.data.Store', {id:'store_website_visit',fields: ['id','type','num','rate'],
                                   proxy:
{
type: 'ajax'
,url: '/pronline/Msg?FunName@ncm_website_visit_type'
//,url: '/data_test.js'
,reader:
    {
type:'json'
,root: 'record'

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
				
    
    ary_count = new Array();
    all_array = new Array();
    ary_type = new Array();
    userlog_time_rate = new Array();
    
    Start_Loading();
    is_export = 0;
  alert('ddddd');
    store_website_visit.load(
    {
			params:
			{
					start_time:frequency_sdate,
					end_time:frequency_edate,
					hostIp:"192.168.20.62"

			},
			callback: function(records, options, success)
        {
						Stop_Loading();
            if (success)
            {

//                alert("成功");
                
                for (var k = 0 ; k < records.length;k++)
                {
                    	ary_type[k] = records[k].get("type");
											ary_count[k] = records[k].get("num");
											userlog_time_rate[k] = records[k].get("rate");
											all_array.push([ary_type[k],ary_count[k],userlog_time_rate[k]]);
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
               AllPage = Math.ceil(all_array.length/9);
               PageNum = 0;
                var total = all_array.length;               
                show_visit_type_pie();
                CreateTab()
            }
            else
            {
//                alert("没有获取到数据");
            }
        }
    });
    alert('cccc');
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



};
function CreateTab()
{		
		t = 0;
		document.getElementById("nowPageNum").innerHTML = PageNum+1;
		document.getElementById("allPageNum").innerHTML = AllPage;
    var newTable = document.getElementById("user_count");
    newTable.innerHTML = "";
    var titletr  = document.createElement("tr");
    titletr.innerHTML = "<td>序号</td><td>类型</td><td>访问次数</td><td>百分比</td>";
    newTable.appendChild(titletr);
    
		
		if(all_array.length < 9)
		{
		for (var i = 0 ; i < all_array.length; i++ )
    {
        var newTr = document.createElement("tr");
        var newTd1= document.createElement("td");
        var newTd2= document.createElement("td");
        var newTd3= document.createElement("td");
        var newTd4= document.createElement("td");

        newTd1.innerHTML = ""+(++t)+"";
        newTd2.innerHTML = ""+all_array[i][0]+"";

        newTd3.innerHTML = ""+all_array[i][1]+"";
        newTd4.innerHTML = all_array[i][2]+"%";

        newTr.appendChild(newTd1);
        newTr.appendChild(newTd2);
        newTr.appendChild(newTd3);
        newTr.appendChild(newTd4);

        newTable.appendChild(newTr);
		}
	}
		else
		{	
    for (var i = 9*PageNum ; i < 9*(PageNum+1) ; i++ )
    {
        var newTr = document.createElement("tr");
        var newTd1= document.createElement("td");
        var newTd2= document.createElement("td");
        var newTd3= document.createElement("td");
        var newTd4= document.createElement("td");

        newTd1.innerHTML = ""+(++t)+"";
        newTd2.innerHTML = ""+all_array[i][0]+"";

        newTd3.innerHTML = ""+all_array[i][1]+"";
        newTd4.innerHTML = all_array[i][2]+"%";

        newTr.appendChild(newTd1);
        newTr.appendChild(newTd2);
        newTr.appendChild(newTd3);
        newTr.appendChild(newTd4);

        newTable.appendChild(newTr);
    }
 	 }
}

function show_visit_type_pie()
{

    var data = new Array();
    var obj  = new Object();
    var color = ["#ed8c25","#e6a34c","#e6b87a","#e4cea7","#b9c1ce","#90a0c4","#6185b9","#ed8c25","#e6a34c","#e6b87a","#e4cea7","#b9c1ce","#90a0c4","#6185b9","#ed8c25","#e6a34c","#e6b87a","#e4cea7","#b9c1ce","#90a0c4","#6185b9","#ed8c25","#e6a34c","#e6b87a","#e4cea7","#b9c1ce","#90a0c4","#6185b9"]
    for (var j = 0 ; j < all_array.length ; j++)
    {
        obj = new Object();
        obj.name = all_array[j][0];
        obj.value= all_array[j][2]+"%";
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

