$(document).ready(function() {
	
	//location.href = "#top";
	//滚动图片效果
	
	$('.flexslider').flexslider( {
		
	});
	
	
/*	$('.flexslider2').flexslider( {
		
	});*/
	//只有第一次加载，才提示“点击此处连接上网”
/*	var loc = window.location.href;
	if(loc){
		if(loc.indexOf("from=runproxy") > 0){
			$("#cover").show();
		}
	}
	//五秒后自动隐藏
	setTimeout(function(){
		$("#cover").hide();
	}, 5000);
	
	//点击隐藏
	$("#cover").click(function(){
		$("#cover").hide();
	})
	*/
	//不同设备，调用不同的打电话的接口
	var userAgent = navigator.userAgent.toLowerCase();
	if (userAgent.indexOf("iphone") > 0) {
		$("#runTelephone").attr("href", "callto:4001030101");
	} else {
		$("#runTelephone").attr("href", "wtai://wp/mc;4001030101");
	}
	
	// 发起一次请求，来记录首页的真实访问次数
	// $.get("homepage.jsp", function(data) {
	// });
	
});