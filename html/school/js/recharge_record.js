//账单数据存储结构
	var recharge_record_arr = [
		{
			'date':'2015-11-04    14:25',
			'menoy':'100元',
			'pay_mode':'微信支付'
		},{
			'date':'2015-10-12    12:25',
			'menoy':'20元',
			'pay_mode':'支付宝'
		}
		,{
			'date':'2015-10-12    12:25',
			'menoy':'20元',
			'pay_mode':'支付宝'
		}
		,{
			'date':'2015-10-12    12:25',
			'menoy':'20元',
			'pay_mode':'支付宝'
		}
		,{
			'date':'2015-10-12    12:25',
			'menoy':'20元',
			'pay_mode':'支付宝'
		}
		,{
			'date':'2015-10-12    12:25',
			'menoy':'20元',
			'pay_mode':'支付宝'
		}
		,{
			'date':'2015-10-12    12:25',
			'menoy':'20元',
			'pay_mode':'支付宝'
		}
		,{
			'date':'2015-10-12    12:25',
			'menoy':'20元',
			'pay_mode':'支付宝'
		}
		,{
			'date':'2015-10-12    12:25',
			'menoy':'20元',
			'pay_mode':'支付宝'
		}
		,{
			'date':'2015-10-12    12:25',
			'menoy':'20元',
			'pay_mode':'支付宝'
		}
		,{
			'date':'2015-10-12    12:25',
			'menoy':'20元',
			'pay_mode':'支付宝'
		}
	]

	
	$(document).on("pagebeforeshow","#pgRechargeRecord",function(){
		//加载数据
		//record_div增量
		var increment = 0;
		//record_div id 变量
		var record_div_id ="";
		//遍历账单信息数组
		$.each(recharge_record_arr,function(n,recharge_record_arr) {
			//创建外层div
			record_div_id="record_div_id"+increment;
			var $record_div=$("<div id='"+ record_div_id +"'></div>");
			$("#recharge_record").append($record_div);
			$("#"+record_div_id).addClass("record_div_style");
			//将账单信息加载到record_div
			var $mydate=$("<b>"+ recharge_record_arr.date +"</b><br>");
			var $money=$("<b>金额:</b><span>"+ recharge_record_arr.menoy +"</span><br>");
			var $pay_mode=$("<b>付款方式:</b><span>"+ recharge_record_arr.pay_mode +"</span><br>");
			$("#"+record_div_id).append($mydate,$money,$pay_mode);
			$("span").addClass("myStyle");
			increment ++;
        });  
	});