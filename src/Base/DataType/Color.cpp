#include<hgl/type/Color.h>

namespace hgl
{
    #undef DEF_COLOR
    #define    DEF_COLOR(name,red,green,blue,chn_name)    {   \
                                                        red,    \
                                                        green,  \
                                                        blue,   \
                                                        int(double(red)*0.299+double(green)*0.587+double(blue)*0.114), \
                                                        \
                                                        float(double(red)/255.0f),   \
                                                        float(double(green)/255.0f), \
                                                        float(double(blue)/255.0f),  \
                                                        float((double(red)*0.299+double(green)*0.587+double(blue)*0.114)/255.0f),\
                                                        \
                                                        U16_TEXT(chn_name)  \
                                                    },

    COLOR_DEF prv_color[ceEnd+1]=
    {
        DEF_COLOR(ceNone,                     0,  0,  0,"")

        DEF_COLOR(ceAliceBlue,              240,248,255,"艾利斯兰")
        DEF_COLOR(ceAndroidGreen,           164,198, 57,"安卓绿")
        DEF_COLOR(ceAntiqueWhite,           250,235,215,"古董白")
        DEF_COLOR(ceAppleGreen,             141,182,  0,"苹果绿")
        DEF_COLOR(ceAqua,                     0,255,255,"浅绿色")
        DEF_COLOR(ceAquaMarine,             127,255,212,"碧绿色")

        DEF_COLOR(ceArdenRed,               202, 23, 36,"雅顿红")        //取自美国官网LOGO图片

        DEF_COLOR(ceAzure,                  240,255,255,"天蓝色")
        DEF_COLOR(ceBananaMania,            250,231,181,"香蕉黄(芯)")
        DEF_COLOR(ceBananaYellow,           255,225, 53,"香蕉黄(皮)")
        DEF_COLOR(ceBeige,                  245,245,220,"米色")
        DEF_COLOR(ceBisque,                 255,228,196,"桔黄色")
        DEF_COLOR(ceBlack,                    0,  0,  0,"黑色")
        DEF_COLOR(ceBlanchedAlmond,         255,235,205,"白杏色")
        DEF_COLOR(ceBlue,                     0,  0,255,"蓝色")
        DEF_COLOR(ceBlueViolet,             138, 43,226,"紫罗兰蓝")
        DEF_COLOR(ceBrown,                  165, 42, 42,"褐色")
        DEF_COLOR(ceBurlyWood,              222,184,135,"实木色")
        DEF_COLOR(ceCadetBlue,               95,158,160,"军兰色")
        DEF_COLOR(ceCaribbeanGreen,           0,204,153,"加勒比海绿")
        DEF_COLOR(ceChartreuse,             127,255,  0,"黄绿色")
        DEF_COLOR(ceCherryBlossomPink,      255,183,197,"樱桃花粉")
        DEF_COLOR(ceChocolate,              210,105, 30,"巧克力色")
        DEF_COLOR(ceCoral,                  255,127, 80,"珊瑚色")
        DEF_COLOR(ceCornflowerBlue,         100,149,237,"菊花兰")
        DEF_COLOR(ceCornsilk,               255,248,220,"米绸色")
        DEF_COLOR(ceCrimson,                220, 20, 60,"暗深红")
        DEF_COLOR(ceCyan,                     0,255,255,"青色")
        DEF_COLOR(ceDarkBlue,                 0,  0,139,"暗蓝色")
        DEF_COLOR(ceDarkCyan,                 0,139,139,"暗青色")
        DEF_COLOR(ceDarkGoldenrod,          184,134, 11,"暗金黄")
        DEF_COLOR(ceDarkGray,               169,169,169,"暗灰色")
        DEF_COLOR(ceDarkGreen,                0,100,  0,"暗绿色")
        DEF_COLOR(ceDarkGrey,               169,169,169,"暗白色")
        DEF_COLOR(ceDarkKhaki,              189,183,107,"暗黄褐色")
        DEF_COLOR(ceDarkMagenta,            139,  0,139,"暗洋红")
        DEF_COLOR(ceDarkOliveGreen,          85,107, 47,"暗橄榄绿")
        DEF_COLOR(ceDarkOrange,             255,140,  0,"暗桔黄")
        DEF_COLOR(ceDarkOrchid,             153, 50,204,"暗紫色")
        DEF_COLOR(ceDarkRed,                139,  0,  0,"暗红色")
        DEF_COLOR(ceDarkSalmon,             233,150,122,"暗肉色")
        DEF_COLOR(ceDarkSeaGreen,           143,188,143,"暗海兰")
        DEF_COLOR(ceDarkSlateBlue,           72, 61,139,"暗灰兰")
        DEF_COLOR(ceDarkSlateGray,           47, 79, 79,"墨绿色")
        DEF_COLOR(ceDarkSlateGrey,           47, 79, 79,"暗灰绿")
        DEF_COLOR(ceDarkTurquoise,            0,206,209,"暗宝石绿")
        DEF_COLOR(ceDarkViolet,             148,  0,211,"暗紫罗兰")
        DEF_COLOR(ceDeepPink,               255, 20,147,"深粉红")
        DEF_COLOR(ceDeepSkyBlue,              0,191,255,"深天蓝")
        DEF_COLOR(ceDimGray,                105,105,105,"暗灰色")
        DEF_COLOR(ceDimGrey,                105,105,105,"暗灰白")
        DEF_COLOR(ceDodgerBlue,              30,144,255,"闪兰色")
        DEF_COLOR(ceFireBrick,              178, 34, 34,"火砖色")
        DEF_COLOR(ceFloralWhite,            255,250,240,"花白色")
        DEF_COLOR(ceForestGreen,             34,139, 34,"森林绿")
        DEF_COLOR(ceFrenchBeige,            166,123, 91,"法国米色")
        DEF_COLOR(ceFrenchBlue,               0,114,187,"法国兰")
        DEF_COLOR(ceFrenchLilac,            134, 96,142,"法国丁香色")
        DEF_COLOR(ceFuchsia,                255,  0,255,"紫红色")
        DEF_COLOR(ceGainsboro,              220,220,220,"淡灰色")
        DEF_COLOR(ceGhostWhite,             248,248,255,"幽灵白")
        DEF_COLOR(ceGold,                   255,215,  0,"金色")
        DEF_COLOR(ceGoldenrod,              218,165, 32,"金麒麟色")
        DEF_COLOR(ceGoldenYellow,           255,223,  0,"金黄")
        DEF_COLOR(ceGray,                   128,128,128,"灰色")
        DEF_COLOR(ceGreen,                    0,128,  0,"绿色")
        DEF_COLOR(ceGreenYellow,            173,255, 47,"蓝绿色")
        DEF_COLOR(ceGrey,                   128,128,128,"灰白色")
        DEF_COLOR(ceHollywoodCerise,        244,  0,161,"好莱坞樱桃红")
        DEF_COLOR(ceHoneydew,               240,255,240,"蜜色")
        DEF_COLOR(ceHotPink,                255,105,180,"火热粉")
        DEF_COLOR(ceHunterGreen,             53, 94, 59,"猎人绿")
        DEF_COLOR(ceIndianGreen,             19,136,  8,"印度绿")
        DEF_COLOR(ceIndianRed,              205, 92, 92,"印度红")
        DEF_COLOR(ceIndianYellow,           227,168, 87,"印度黄")
        DEF_COLOR(ceIndigo,                  75,  0,130,"靛青色")
        DEF_COLOR(ceIvory,                  255,255,240,"象牙白")
        DEF_COLOR(ceKhaki,                  240,230,140,"黄褐色")
        DEF_COLOR(ceLavender,               230,230,250,"淡紫色")
        DEF_COLOR(ceLavenderBlush,          255,240,245,"淡紫红")
        DEF_COLOR(ceLawnGreen,              124,252,  0,"草绿色")
        DEF_COLOR(ceLemon,                  255,247,  0,"柠檬色")
        DEF_COLOR(ceLemonYellow,            255,244, 79,"柠檬黄")
        DEF_COLOR(ceLemonChiffon,           255,250,205,"柠檬绸")
        DEF_COLOR(ceLightBlue,              173,216,230,"亮蓝色")
        DEF_COLOR(ceLightCoral,             240,128,128,"亮珊瑚色")
        DEF_COLOR(ceLightCyan,              224,255,255,"亮青色")
        DEF_COLOR(ceLightGoldenrodYellow,   250,250,210,"亮金黄")
        DEF_COLOR(ceLightGray,              211,211,211,"亮灰色")
        DEF_COLOR(ceLightGreen,             144,238,144,"亮绿色")
        DEF_COLOR(ceLightGrey,              211,211,211,"亮灰白")
        DEF_COLOR(ceLightPink,              255,182,193,"亮粉红")
        DEF_COLOR(ceLightSalmon,            255,160,122,"亮肉色")
        DEF_COLOR(ceLightSeaGreen,           32,178,170,"亮海蓝")
        DEF_COLOR(ceLightSkyBlue,           135,206,250,"亮天蓝")
        DEF_COLOR(ceLightSlateGray,         119,136,153,"亮蓝灰")
        DEF_COLOR(ceLightSlateGrey,         119,136,153,"亮蓝白")
        DEF_COLOR(ceLightSteelBlue,         176,196,222,"亮钢兰")
        DEF_COLOR(ceLightYellow,            255,255,224,"亮黄色")
        DEF_COLOR(ceLime,                      0,255,  0,"酸橙色")
        DEF_COLOR(ceLimeGreen,               50,205, 50,"橙绿色")
        DEF_COLOR(ceLinen,                  250,240,230,"亚麻色")
        DEF_COLOR(ceLion,                   193,154,107,"獅子棕")
        DEF_COLOR(ceMagenta,                255,  0,255,"红紫色")
        DEF_COLOR(ceMaroon,                 128,  0,  0,"粟色")
        DEF_COLOR(ceMediumAquamarine,       102,205,170,"间绿色")
        DEF_COLOR(ceMediumBlue,               0,  0,205,"间兰色")
        DEF_COLOR(ceMediumOrchid,           186, 85,211,"间淡紫")
        DEF_COLOR(ceMediumPurple,           147,112,219,"间紫色")
        DEF_COLOR(ceMediumSeaGreen,          60,179,113,"间海蓝")
        DEF_COLOR(ceMediumSlateBlue,        123,104,238,"间暗蓝")
        DEF_COLOR(ceMediumSpringGreen,        0,250,154,"间春绿")
        DEF_COLOR(ceMediumTurquoise,         72,209,204,"间绿宝石")
        DEF_COLOR(ceMediumVioletRed,        199, 21,133,"间紫罗兰")
        DEF_COLOR(ceMidNightBlue,            25, 25,112,"中灰蓝")
        DEF_COLOR(ceMint,                    62,180,137,"薄荷色")
        DEF_COLOR(ceMintCream,              245,255,250,"薄荷霜")
        DEF_COLOR(ceMintGreen,              152,255,152,"薄荷绿")
        DEF_COLOR(ceMistyRose,              255,228,225,"浅玫瑰")
        DEF_COLOR(ceMoccasin,               255,228,181,"鹿皮色")
			
		DEF_COLOR(ceMozillaBlue,			  0, 83,159,"火狐蓝")
        DEF_COLOR(ceMozillaCharcoal,         77, 78, 83,"谋智炭")
		DEF_COLOR(ceMozillaLightBlue,		  0,150,221,"火狐亮蓝")
		DEF_COLOR(ceMozillaLightOrange,		255,149,  0,"火狐亮橙")
		DEF_COLOR(ceMoziilaNightBlue,		  0, 33, 71,"谋智暗夜蓝")
		DEF_COLOR(ceMozillaOrange,			230, 96,  0,"火狐橙")
        DEF_COLOR(ceMozillaRed,             193, 56, 50,"谋智红")
        DEF_COLOR(ceMozillaSand,            215,211,200,"谋智沙")
		DEF_COLOR(ceMozillaYellow,			255,203,  0,"火狐黄")

        DEF_COLOR(ceNavajoWhite,            255,222,173,"纳瓦白")
        DEF_COLOR(ceNavy,                     0,  0,128,"海军色")

        DEF_COLOR(ceNiveaBlue,                0, 19,111,"妮维雅蓝")     //取自妮维雅蓝国际官网

        DEF_COLOR(ceNokiaBlue,               18, 65,145,"诺基亚蓝")     //取自诺基亚官网

        DEF_COLOR(ceOldLace,                253,245,230,"老花色")
        DEF_COLOR(ceOlive,                  128,128,  0,"橄榄色")
        DEF_COLOR(ceOlivedrab,              107,142, 35,"深绿褐色")
        DEF_COLOR(ceOrange,                 255,165,  0,"橙色")
        DEF_COLOR(ceOrangeRed,              255, 69,  0,"红橙色")
        DEF_COLOR(ceOrchid,                 218,112,214,"淡紫色")
        DEF_COLOR(cePaleGoldenrod,          238,232,170,"苍麒麟色")
        DEF_COLOR(cePaleGreen,              152,251,152,"苍绿色")
        DEF_COLOR(cePaleTurquoise,          175,238,238,"苍宝石绿")
        DEF_COLOR(cePaleVioletRed,          219,112,147,"苍紫罗兰色")
        DEF_COLOR(cePapayawhip,             255,239,213,"番木色")
        DEF_COLOR(cePeachpuff,              255,218,185,"桃色")
        DEF_COLOR(cePear,                   209,226, 49,"梨色")
        DEF_COLOR(cePeru,                   205,133, 63,"秘鲁色")
        DEF_COLOR(cePink,                   255,192,203,"粉红色")

        DEF_COLOR(cePlayStationBlue,          0, 55,145,"PlayStation蓝")
        DEF_COLOR(cePlayStationLightBlue,     0,120,200,"PlayStation亮蓝")

        DEF_COLOR(cePlum,                   221,160,221,"洋李色")
        DEF_COLOR(cePowderBlue,             176,224,230,"粉蓝色")
        DEF_COLOR(cePurple,                 128,  0,128,"紫色")
        DEF_COLOR(ceRed,                    255,  0,  0,"红色")
        DEF_COLOR(ceRose,                   255,  0,127,"玫瑰红")
        DEF_COLOR(ceRosyBrown,              188,143,143,"褐玫瑰红")
        DEF_COLOR(ceRoyalBlue,               65,105,225,"皇家蓝")
        DEF_COLOR(ceRuby,                   224, 17, 95,"宝石红")
        DEF_COLOR(ceSaddleBrown,            139, 69, 19,"重褐色")
        DEF_COLOR(ceSalmon,                 250,128,114,"鲜肉色")

        DEF_COLOR(ceSamsungBlue,             20, 40,160,"三星蓝")          //取自三星官网

        DEF_COLOR(ceSandyBrown,             244,164, 96,"沙褐色")
        DEF_COLOR(ceSeaGreen,                46,139, 87,"海绿色")
        DEF_COLOR(ceSeaShell,               255,245,238,"海贝色")
        DEF_COLOR(ceSienna,                 160, 82, 45,"赭色")
        DEF_COLOR(ceSilver,                 192,192,192,"银色")
        DEF_COLOR(ceSkyBlue,                135,206,235,"天蓝色")
        DEF_COLOR(ceSlateBlue,              106, 90,205,"石蓝色")
        DEF_COLOR(ceSlateGray,              112,128,144,"灰石色")
        DEF_COLOR(ceSlateGrey,              112,128,144,"白灰石色")
        DEF_COLOR(ceSnow,                   255,250,250,"雪白色")
        DEF_COLOR(ceSpringGreen,              0,255,127,"春绿色")
        DEF_COLOR(ceSteelBlue,               70,130,180,"钢兰色")
        DEF_COLOR(ceTan,                    210,180,140,"茶色")
        DEF_COLOR(ceTeal,                     0,128,128,"水鸭色")
        DEF_COLOR(ceThistle,                216,191,216,"蓟色")

        DEF_COLOR(ceTiffanyBlue,            129,216,208,"蒂芙尼蓝")     //取自zh.wikipedia.org/zh-cn/蒂芙尼蓝

        DEF_COLOR(ceTomato,                 255, 99, 71,"西红柿色")
        DEF_COLOR(ceTurquoise,               64,224,208,"青绿色")
        DEF_COLOR(ceViolet,                 238,130,238,"紫罗兰色")
        DEF_COLOR(ceWheat,                  245,222,179,"浅黄色")
        DEF_COLOR(ceWhite,                  255,255,255,"白色")
        DEF_COLOR(ceWhiteSmoke,             245,245,245,"烟白色")
        DEF_COLOR(ceYellow,                 255,255,  0,"黄色")
        DEF_COLOR(ceYellowGreen,            154,205, 50,"黄绿色")

        DEF_COLOR(ceEnd,                      0  ,0  ,0,"")
    };

    #undef DEF_COLOR
}//namespace hgl
