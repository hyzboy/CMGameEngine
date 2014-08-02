/*
   本工具會在畫面左邊加設跳至頁頂和頁底的常駐連結。
   
  */importScript('User:Cdip150/gtc.js');/*
*/

mw.loader.using(['ext.gadget.site-lib', 'jquery.cookie', 'jquery.ui.dialog'], function(){
	var isTouch=("ontouchstart" in document.documentElement);
	
	var isOn = (""+document.cookie).split("TBLkisOn=");
	if(isOn.length<2){
		isOn = isTouch?1:0;
	}else{
		isOn = isOn[1].split(";")[0];
		if(isOn=="") isOn = isTouch?1:0;
	}

	$(function(){
		var i="", classname="topbottomScrollTag";
		
		var j=0, closeid="btnCloseTopbottom";
		while($('#'+closeid+j).length!=0) ++j;
		
		var toggleTBLk=function(o){
			$.cookie("TBLkisOn", o, {
				path: "/",
				expires: 90
			});
			
			$('.'+classname+i).css("display", o==0?"none":"");
			
			if(o==0){
				$("div.portal#p-tb div.body ul, div.portlet#p-tb div.pBody ul").append(
					$('<li id="t-topbottomlink">')
					.append($("<a />")
						.text(wgULS("左侧跳顶连接","左側跳頂連結"))
						.attr("title", wgULS("在画面左上角及左下角显示跳至页顶和页底的连接","在畫面左上角及左下角顯示跳至頁頂和頁底的連結"))
						.attr("href", "javascript:void(0);").click(function () {
							toggleTBLk(1);
							scrollshow();
							$(this).parent().remove();
						})
					)
				);	
			}else{
				$(window).trigger("scroll");
			}
		
		};
		
		var hashno=0;
		$('body')
		.append(
			$('<div class="noprint '+classname+i+'" style="top: 0;" title="跳至'+wgULS('页顶', '頁頂')+'"><div style="position: fixed; width: 1em; top: 5px; text-align: center; text-decoration: overline;">↑</div></div>')
			.click(function(){
				var x=$('html').get(0).scrollLeft || $('body').get(0).scrollLeft;
				var y=$('html').get(0).scrollTop  || $('body').get(0).scrollTop;
				location.assign('#'+(hashno=(hashno+1)%3));
				window.scrollTo(x, y);
				$('html, body').animate({scrollTop : 0}, 300);
				return false;
			})
		)
		.append(
			$('<div class="noprint '+classname+i+'" style="bottom: 0;" title="跳至'+wgULS('页', '頁')+'底"><div style="position: fixed; width: 1em; bottom: 5px; text-align: center; text-decoration: underline;">↓</div></div>')
			.click(function(){
				var x=$('html').get(0).scrollLeft || $('body').get(0).scrollLeft;
				var y=$('html').get(0).scrollTop  || $('body').get(0).scrollTop;
				location.assign('#'+(hashno=(hashno+1)%3));
				window.scrollTo(x, y);
				$('html, body').animate({scrollTop : $('body').get(0).scrollHeight}, 300);
				return false;
			})
		)
		.append(
			$('<div class="noprint '+classname+i+'" id="'+closeid+j+'" style="left: 1em; top: 0; width: 1em; height: 1em; padding: 3px;" title="'+wgULS('隐藏跳至页顶和页底的连接','隱藏跳至頁頂和頁底的連結')+'"><div>╳</div></div>')
			.click(function(){
				if(!isTouch) return;
				
				if(
					window.confirm(
						wgULS(
							"确定不要再显示跳至页顶和页底的连接?\n(之后可以在左侧工具的连接中重新设定显示)",
							"確定不要再顯示跳至頁頂和頁底的連結?\n(之後可以在左側工具的連結中重新設定顯示)"
						)
					)
				){}else return;
				
				toggleTBLk(0);
			})
			.click(function(){
				if(isTouch) return;
				
				$("<div />")
				.attr("title", wgULS("跳至页顶／页底", "跳至頁頂／頁底"))
				.append([
					wgULS("确定不要再显示跳至页顶和页底的连接?", "確定不要再顯示跳至頁頂和頁底的連結?"),
					"<br />",
					wgULS("(之后可以在左侧工具的连接中重新设定显示)", "(之後可以在左側工具的連結中重新設定顯示)")
				])
				.dialog({
					resizable: false,
					modal: true,
					buttons: {
						"是" : function(){
							toggleTBLk(0);
							$(this).dialog("close");
						} ,
						"否" : function(){
							$(this).dialog("close");
						}
					}
				});
			})
		);
		

		var hideTimer=0;
		
		var showArrows=function(){
			clearTimeout(hideTimer);
			$(this).find('div').fadeIn();
			$('#'+closeid+j).find('div').fadeIn();
		}
		
		var hideArrows=function(){
			clearTimeout(hideTimer);
			$('.'+classname+i).find('div').fadeOut();
		}
		
		
		var canmouseenter=("onmouseenter" in document.documentElement) && ("onmouseleave" in document.documentElement);
		
		$('.'+classname+i)
		.on("mousemove" , showArrows)
		.on( (canmouseenter?"mouseenter":"mouseover"), showArrows)
		.on( (canmouseenter?"mouseleave":"mouseout" ), hideArrows)
		.find("div").css("display", "none");
		
		
		var scrollshow=function() {
			$('.'+classname+i).find('div').fadeIn();
			clearTimeout(hideTimer);
			hideTimer=setTimeout(function(){
				$('.'+classname+i).find('div').fadeOut();
			}, 2000);
		}
		
		$(window).scroll(function(){
			if(!isTouch) return;
			scrollshow();
		}).trigger("scroll");
		
		toggleTBLk(isOn);
	});
});