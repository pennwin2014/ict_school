/**
 *
 * HTML5 Image uploader with Jcrop
 *
 * Licensed under the MIT license.
 * http://www.opensource.org/licenses/mit-license.php
 * 
 * Copyright 2012, Script Tutorials
 * http://www.script-tutorials.com/
 */

// convert bytes into friendly format
function bytesToSize(bytes) {
    var sizes = ['Bytes', 'KB', 'MB'];
    if (bytes == 0) return 'n/a';
    var i = parseInt(Math.floor(Math.log(bytes) / Math.log(1024)));
    return (bytes / Math.pow(1024, i)).toFixed(1) + ' ' + sizes[i];
};

// check for selected crop region
function checkForm() {
    if (parseInt($('#w').val())) return true;
    $('.error').html('Please select a crop region and then press Upload').show();
    return false;
};

// update info by cropping (onChange and onSelect events handler)
function updateInfo(e) {
    $('#x1').val(e.x);
    $('#y1').val(e.y);
    $('#x2').val(e.x2);
    $('#y2').val(e.y2);
    $('#w').val(e.w);
    $('#h').val(e.h);
};

// clear info by cropping (onRelease event handler)
function clearInfo() {
    $('.info #w').val('');
    $('.info #h').val('');
};
function saveInfo(){
alert("saveInfo");
}
function fileSelectHandler(file,width,height,ratio) {
	//	var oFile = file.target || window.event.srcElement;
	var prevDiv = document.getElementById('preview');  
	if(file.files && file.files[0])
	{
	//alert(111);
    // get selected file
    var oFile = file.files[0];
	
	document.getElementById("error").style.display ="none";
    // check for image type (jpg and png are allowed)
    var rFilter = /^(image\/jpeg|image\/png|image\/gif)$/i;
    if (! rFilter.test(oFile.type)) {
		document.getElementById("error").innerHTML = "图片格式不符，请选择一张jpg/png/gif格式的图片";
		document.getElementById("error").style.display ="block";
        return;
    }
	  var sResultFileSize = bytesToSize(oFile.size);

    if (oFile.size > 150 * 1024) {
	   document.getElementById("error").innerHTML = "你选择的图片("+sResultFileSize+")过大，请选择一张小于150KB的图片";
		document.getElementById("error").style.display ="block";
        return;
    }
	
    // preview element
	
    var oImage = document.getElementById('dragDiv');
	
    // prepare HTML5 FileReader
    var oReader = new FileReader();
        oReader.onload = function(e) {
		//alert(e.target.result);
        // e.target.result contains the DataURL which we can use as a source of the image
        oImage.src = e.target.result;
		ic.Url = e.target.result;
		//alert(ic.Url);
		//alert(document.getElementById('image_file').files);
		ic.Init(); 
		document.getElementById('viewDiv2').src = e.target.result;
		//prevDiv.src=e.target.result;
		//alert(e.naturalWidth);
		//alert(e.target.naturalWidth);
		//document.getElementById('bgDiv').style.width=document.getElementById('viewDiv2').value+"px";
		//document.getElementById('bgDiv').style.height=document.getElementById('viewDiv2').value+"px";
		
		//document.getElementById('previewimg').value=document.getElementById('preview').src;
        oImage.onload = function () { // onload event handler
		
            // display step 2
            $('.step2').fadeIn(500);
			//alert(getPath(document.getElementById('image_file'))); 
            // display some basic image info
            var sResultFileSize = bytesToSize(oFile.size);
            $('#filesize').val(sResultFileSize);
            $('#filetype').val(oFile.type);
            $('#filedim').val(oImage.naturalWidth + ' x ' + oImage.naturalHeight);

            // Create variables (in this scope) to hold the Jcrop API and image size
            var jcrop_api, boundx, boundy;

            // destroy Jcrop if it is existed
            if (typeof jcrop_api != 'undefined') 
                jcrop_api.destroy();

            // initialize Jcrop
            $('#preview').Jcrop({
                minSize: [width, height], // min crop size
                aspectRatio : ratio, // keep aspect ratio 1:1
                bgFade: true, // use fade effect
                bgOpacity: .3, // fade opacity
                onChange: updateInfo,
                onSelect: updateInfo,
                onRelease: clearInfo,
				onDblClick: saveInfo
            }, function(){

                // use the Jcrop API to get the real image size
                var bounds = this.getBounds();
                boundx = bounds[0];
                boundy = bounds[1];

                // Store the Jcrop API in the jcrop_api variable
                jcrop_api = this;
            });
        };
    };
	
    // read selected file as DataURL
    oReader.readAsDataURL(oFile);
	}
	else{
	//alert(" IE6~9 ");

	//prevDiv.innerHTML = '<div class="img" style="filter:progid:DXImageTransform.Microsoft.AlphaImageLoader(sizingMethod=scale,src=\'' + file.value + '\')"></div>'; 
	//alert(prevDiv.innerHTML);
	//var img = new Image();
	
	file.select(); 
	var path = document.selection.createRange().text; 
	//alert(path);
	//img.src=file.src;
	//alert(img.width);
	//alert(img.height);
	document.getElementById("preview").innerHTML=""; 
	document.getElementById("preview").style.filter = "progid:DXImageTransform.Microsoft.AlphaImageLoader(enabled='true',sizingMethod='scale',src='" + path + "')";//使用滤镜效果 
	document.getElementById("dragDiv").style.filter = "progid:DXImageTransform.Microsoft.AlphaImageLoader(enabled='true',sizingMethod='',src='" + path + "')";//使用滤镜效果 
	
	//alert(document.getElementById("preview").innerHTML);
	}
}