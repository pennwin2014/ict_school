function a()
{
	jumpToPage('pgCanChangePackage');
}

function b()
{
	alert ("立即充值");
}
function c()
{
	alert ("充值记录");
}

function d()
{
	alert ("账单查询");
}
$(document).on("pagebeforeshow","#pgMyInfo",function()
{
	document.getElementById("current_package").innerHTML ="500M无忧套餐";
	document.getElementById("current_package").style.color ="#AFAFAF";
	document.getElementById("valid_date").innerHTML ="2015-03-01至2015-3-31";
	document.getElementById("valid_date").style.color ="#AFAFAF";
	document.getElementById("account_balance").innerHTML ="20元";
	document.getElementById("account_balance").style.color ="#AFAFAF";
	document.getElementById("data_package").innerHTML ="流量套餐";
	document.getElementById("data_package").style.color ="#AFAFAF";
	document.getElementById("package_content").innerHTML="每月享有500M上网流量";
	document.getElementById("package_content").style.color ="#AFAFAF";
	document.getElementById("progress").innerHTML="50%";
	document.getElementById("bar").style.width="50%";
});