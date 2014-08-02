// See [[mw:Reference Tooltips]]
if (wgAction == 'view' && !mw.user.options.get('gadget-Navigation_popups')) { // 启用导航Popup则禁用本js
  window.pg || $(document).ready(function ($) {
    var bodyContent = document.getElementById('mw-content-text') || document.body;

    // Make sure we are in article, project, or help namespace
    if (wgCanonicalNamespace === '' || wgCanonicalNamespace === 'Project' || wgCanonicalNamespace === 'Help') {
      function toggleRT(o) {
        mw.loader.using("jquery.cookie", function () {
          $.cookie("RTsettings", o + "|" + settings[1] + "|" + settings[2], {
            path: "/",
            expires: 90
          });
          location.reload();
        })
      }
      var settings = document.cookie.split("RTsettings=")[1];
      settings = settings ? settings.split(";")[0].split("%7C") : [1, 200, +("ontouchstart" in document.documentElement)];
      if (settings[0] == 0) {
        var footer = $("#footer-places, #f-list");
        if (footer.length === 0) {
          footer = $("#footer li").parent();
        }
        footer.append($("<li>").append($("<a>").text("启用参考文献提示工具").attr("href", "javascript:(function(){})()").click(function () {
          toggleRT(1)
        })));
        return;
      }
      var isTouchscreen = +settings[2],
        timerLength = isTouchscreen ? 0 : +settings[1],
        settingsMenu;
      $(".reference").each(function () {
        var tooltipNode, hideTimer, showTimer;

        function findRef(h) {
          h = h.firstChild.getAttribute("href");
          h = h && h.split("#");
          h = h && h[1];
          h = h && document.getElementById(h);
          h = h && h.nodeName == "LI" && h;
          return h;
        }

        function hide(refLink) {
          if (tooltipNode && tooltipNode.parentNode == bodyContent) {
            hideTimer = setTimeout(function () {
              $(tooltipNode).animate({
                opacity: 0
              }, 100, function () {
                bodyContent.removeChild(tooltipNode)
              })
            }, isTouchscreen ? 16 : 100)
          } else {
            var h = findRef(refLink);
            h && (h.style.border = "");
          }
        }

        function show() {
          if (!tooltipNode.parentNode || tooltipNode.parentNode.nodeType === 11) {
            bodyContent.appendChild(tooltipNode);
          }
          $(tooltipNode).stop().animate({
            opacity: 1
          }, 100)
          clearTimeout(hideTimer);
        }

        function openSettingsMenu() {
          if (settingsMenu) {
            settingsMenu.dialog("open");
          } else {
            settingsMenu = $("<form>").append(
            $("<button>").css("width", "100%").text("关闭参考文献提示工具").button().click(function () {
              toggleRT(0)
            }),
              "<br>",
            $("<small>").text("一旦关闭，参考文献提示工具还可以从页面底部的一个链接重新启用。"),
              "<hr>",
            $("<label>").text("出现提示的延迟时间(毫秒): ").append($("<input>").attr({
              "type": "number",
              "value": settings[1],
              step: 50,
              min: 0,
              max: 5000
            })),
              "<br>",
            $("<span>").text("弹出提示的方式："),
            $("<label>").append(
            $("<input>").attr({
              "type": "radio",
              "name": "RTActivate",
              "checked": settings[2] == 0 && "checked",
              "disabled": "ontouchstart" in document.documentElement && "disabled"
            }),
              "悬停"),
            $("<label>").append(
            $("<input>").attr({
              "type": "radio",
              "name": "RTActivate",
              "checked": settings[2] == 1 && "checked"
            }),
              "点击")).submit(function (e) {
              e.preventDefault()
            }).dialog({
              modal: true,
              width: 500,
              title: "参考文献提示工具选项",
              buttons: {
                "保存设置": function () {
                  var a = this.getElementsByTagName("input"),
                    b = +a[0].value;
                  $.cookie("RTsettings", "1|" + (b > -1 && b < 5001 ? b : settings[1]) + (a[1].checked ? "|0" : "|1"), {
                    path: "/",
                    expires: 90
                  });
                  location.reload();
                }
              }
            });
          }
        }
        $(this)[isTouchscreen ? 'click' : 'hover'](function (e) {
          var _this = this;
          if (isTouchscreen) {
            e.preventDefault();
            (tooltipNode && tooltipNode.parentNode == bodyContent) || setTimeout(function () {
              $(bodyContent).bind("click touchstart", function (e) {
                e = e || event;
                e = e.target || e.srcElement;
                for (; e && e.className != "referencetooltip";)
                e = e.parentNode;
                if (!e) {
                  clearTimeout(showTimer);
                  hide(_this);
                  $(bodyContent).unbind("click touchstart", arguments.callee)
                }
              })
            }, 0);
          }
          showTimer && clearTimeout(showTimer);
          showTimer = setTimeout(function () {
            var h = findRef(_this);
            if (!h) {
              return
            };
            if (!isTouchscreen && (window.pageYOffset || document.documentElement.scrollTop || bodyContent.scrollTop || 0) + $(window).height() > $(h).offset().top + h.offsetHeight) {
              h.style.border = "#3366bb 2px solid";
              return;
            }
            if (!tooltipNode) {
              tooltipNode = document.createElement("ul");
              tooltipNode.className = "referencetooltip references-small";
              var c = tooltipNode.appendChild(h.cloneNode(true));
              try {
                if (c.firstChild.nodeName != "A") {
                  while (c.childNodes[1].nodeName == "A" && c.childNodes[1].getAttribute("href").indexOf("#cite_ref-") !== -1) {
                    do {
                      c.removeChild(c.childNodes[1])
                    } while (c.childNodes[1].nodeValue == " ");
                  }
                }
              } catch (e) {
                mw.log(e)
              }
              c.removeChild(c.firstChild);
              $(tooltipNode.firstChild.insertBefore(document.createElement("span"), tooltipNode.firstChild.firstChild)).addClass("RTsettings").attr("title", "设置").click(function () {
                mw.loader.using(["jquery.cookie", "jquery.ui.dialog"], openSettingsMenu);
              })
              tooltipNode.appendChild(document.createElement("li"));
              isTouchscreen || $(tooltipNode).hover(show, hide);
            }
            show();
            var o = $(_this).position(),
              oH = tooltipNode.offsetHeight;
            $(tooltipNode).css({
              top: o.top - oH,
              left: o.left - 7
            });
            if (tooltipNode.offsetHeight > oH) { // is it squished against the right side of the page?
              $(tooltipNode).css({
                left: 'auto',
                right: 0
              });
              tooltipNode.lastChild.style.marginLeft = (o.left - tooltipNode.offsetLeft) + "px";
            }
            if (o.top < tooltipNode.offsetHeight) { // is it above the top of the page?
              $(tooltipNode).addClass("RTflipped").css({
                top: o.top + 12
              });
            } else if (tooltipNode.className === "referencetooltip RTflipped") { // cancel previous
              $(tooltipNode).removeClass("RTflipped");
            }
          }, timerLength);
        }, isTouchscreen ? undefined : function () {
          clearTimeout(showTimer);
          hide(this);
        })
      });
    }
  });
}