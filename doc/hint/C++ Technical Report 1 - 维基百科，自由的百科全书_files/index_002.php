mw.loader.using(['ext.gadget.site-lib', 'jquery.cookie'], function(){
	if($.cookie("lastreload")!=1 && mw.config.get("wgAction")=="view"){
		$.cookie("lastreload", 1, {path:"/", expires: new Date((new Date()).getTime()+3*60*60*1000)});
		if($.cookie("lastreload")==1) location.reload(true);
	};
	
	
});