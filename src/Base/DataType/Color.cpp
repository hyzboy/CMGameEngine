#include<hgl/type/Color.h>

namespace hgl
{
    #undef DEF_COLOR
    #define    DEF_COLOR(eng_name,red,green,blue,chs_name)    {   \
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
                                                        #eng_name, \
                                                        U16_TEXT(chs_name)  \
                                                    },

    COLOR_DEF prv_color[ceEnd+1]=
    {
        DEF_COLOR(None,                     0,  0,  0,"")

        DEF_COLOR(AliceBlue,              240,248,255,"艾利斯兰")
        DEF_COLOR(AndroidGreen,           164,198, 57,"安卓绿")
        DEF_COLOR(AntiqueWhite,           250,235,215,"古董白")
        DEF_COLOR(AppleGreen,             141,182,  0,"苹果绿")
        DEF_COLOR(Aqua,                     0,255,255,"浅绿色")
        DEF_COLOR(AquaMarine,             127,255,212,"碧绿色")

        DEF_COLOR(ArdenRed,               202, 23, 36,"雅顿红")        //取自美国官网LOGO图片

        DEF_COLOR(Azure,                  240,255,255,"天蓝色")
        DEF_COLOR(BananaMania,            250,231,181,"香蕉黄(芯)")
        DEF_COLOR(BananaYellow,           255,225, 53,"香蕉黄(皮)")
        DEF_COLOR(Beige,                  245,245,220,"米色")
        DEF_COLOR(Bisque,                 255,228,196,"桔黄色")
        DEF_COLOR(Black,                    0,  0,  0,"黑色")
        DEF_COLOR(BlanchedAlmond,         255,235,205,"白杏色")
        DEF_COLOR(Blue,                     0,  0,255,"蓝色")
        DEF_COLOR(BlueViolet,             138, 43,226,"紫罗兰蓝")
        DEF_COLOR(Brown,                  165, 42, 42,"褐色")
        DEF_COLOR(BurlyWood,              222,184,135,"实木色")
        DEF_COLOR(CadetBlue,               95,158,160,"军兰色")
        DEF_COLOR(CaribbeanGreen,           0,204,153,"加勒比海绿")
        DEF_COLOR(Chartreuse,             127,255,  0,"黄绿色")
        DEF_COLOR(CherryBlossomPink,      255,183,197,"樱桃花粉")
        DEF_COLOR(Chocolate,              210,105, 30,"巧克力色")
        DEF_COLOR(Coral,                  255,127, 80,"珊瑚色")
        DEF_COLOR(CornflowerBlue,         100,149,237,"菊花兰")
        DEF_COLOR(Cornsilk,               255,248,220,"米绸色")
        DEF_COLOR(Crimson,                220, 20, 60,"暗深红")
        DEF_COLOR(Cyan,                     0,255,255,"青色")
        DEF_COLOR(DarkBlue,                 0,  0,139,"暗蓝色")
        DEF_COLOR(DarkCyan,                 0,139,139,"暗青色")
        DEF_COLOR(DarkGoldenrod,          184,134, 11,"暗金黄")
        DEF_COLOR(DarkGray,               169,169,169,"暗灰色")
        DEF_COLOR(DarkGreen,                0,100,  0,"暗绿色")
        DEF_COLOR(DarkGrey,               169,169,169,"暗白色")
        DEF_COLOR(DarkKhaki,              189,183,107,"暗黄褐色")
        DEF_COLOR(DarkMagenta,            139,  0,139,"暗洋红")
        DEF_COLOR(DarkOliveGreen,          85,107, 47,"暗橄榄绿")
        DEF_COLOR(DarkOrange,             255,140,  0,"暗桔黄")
        DEF_COLOR(DarkOrchid,             153, 50,204,"暗紫色")
        DEF_COLOR(DarkRed,                139,  0,  0,"暗红色")
        DEF_COLOR(DarkSalmon,             233,150,122,"暗肉色")
        DEF_COLOR(DarkSeaGreen,           143,188,143,"暗海兰")
        DEF_COLOR(DarkSlateBlue,           72, 61,139,"暗灰兰")
        DEF_COLOR(DarkSlateGray,           47, 79, 79,"墨绿色")
        DEF_COLOR(DarkSlateGrey,           47, 79, 79,"暗灰绿")
        DEF_COLOR(DarkTurquoise,            0,206,209,"暗宝石绿")
        DEF_COLOR(DarkViolet,             148,  0,211,"暗紫罗兰")
        DEF_COLOR(DeepPink,               255, 20,147,"深粉红")
        DEF_COLOR(DeepSkyBlue,              0,191,255,"深天蓝")
        DEF_COLOR(DimGray,                105,105,105,"暗灰色")
        DEF_COLOR(DimGrey,                105,105,105,"暗灰白")
        DEF_COLOR(DodgerBlue,              30,144,255,"闪兰色")
        DEF_COLOR(FireBrick,              178, 34, 34,"火砖色")
        DEF_COLOR(FloralWhite,            255,250,240,"花白色")
        DEF_COLOR(ForestGreen,             34,139, 34,"森林绿")
        DEF_COLOR(FrenchBeige,            166,123, 91,"法国米色")
        DEF_COLOR(FrenchBlue,               0,114,187,"法国兰")
        DEF_COLOR(FrenchLilac,            134, 96,142,"法国丁香色")
        DEF_COLOR(Fuchsia,                255,  0,255,"紫红色")
        DEF_COLOR(Gainsboro,              220,220,220,"淡灰色")
        DEF_COLOR(GhostWhite,             248,248,255,"幽灵白")
        DEF_COLOR(Gold,                   255,215,  0,"金色")
        DEF_COLOR(Goldenrod,              218,165, 32,"金麒麟色")
        DEF_COLOR(GoldenYellow,           255,223,  0,"金黄")
        DEF_COLOR(Gray,                   128,128,128,"灰色")
        DEF_COLOR(Green,                    0,128,  0,"绿色")
        DEF_COLOR(GreenYellow,            173,255, 47,"蓝绿色")
        DEF_COLOR(Grey,                   128,128,128,"灰白色")
        DEF_COLOR(HollywoodCerise,        244,  0,161,"好莱坞樱桃红")
        DEF_COLOR(Honeydew,               240,255,240,"蜜色")
        DEF_COLOR(HotPink,                255,105,180,"火热粉")
        DEF_COLOR(HunterGreen,             53, 94, 59,"猎人绿")
        DEF_COLOR(IndianGreen,             19,136,  8,"印度绿")
        DEF_COLOR(IndianRed,              205, 92, 92,"印度红")
        DEF_COLOR(IndianYellow,           227,168, 87,"印度黄")
        DEF_COLOR(Indigo,                  75,  0,130,"靛青色")
        DEF_COLOR(Ivory,                  255,255,240,"象牙白")
        DEF_COLOR(Khaki,                  240,230,140,"黄褐色")
        DEF_COLOR(Lavender,               230,230,250,"淡紫色")
        DEF_COLOR(LavenderBlush,          255,240,245,"淡紫红")
        DEF_COLOR(LawnGreen,              124,252,  0,"草绿色")
        DEF_COLOR(Lemon,                  255,247,  0,"柠檬色")
        DEF_COLOR(LemonYellow,            255,244, 79,"柠檬黄")
        DEF_COLOR(LemonChiffon,           255,250,205,"柠檬绸")
        DEF_COLOR(LightBlue,              173,216,230,"亮蓝色")
        DEF_COLOR(LightCoral,             240,128,128,"亮珊瑚色")
        DEF_COLOR(LightCyan,              224,255,255,"亮青色")
        DEF_COLOR(LightGoldenrodYellow,   250,250,210,"亮金黄")
        DEF_COLOR(LightGray,              211,211,211,"亮灰色")
        DEF_COLOR(LightGreen,             144,238,144,"亮绿色")
        DEF_COLOR(LightGrey,              211,211,211,"亮灰白")
        DEF_COLOR(LightPink,              255,182,193,"亮粉红")
        DEF_COLOR(LightSalmon,            255,160,122,"亮肉色")
        DEF_COLOR(LightSeaGreen,           32,178,170,"亮海蓝")
        DEF_COLOR(LightSkyBlue,           135,206,250,"亮天蓝")
        DEF_COLOR(LightSlateGray,         119,136,153,"亮蓝灰")
        DEF_COLOR(LightSlateGrey,         119,136,153,"亮蓝白")
        DEF_COLOR(LightSteelBlue,         176,196,222,"亮钢兰")
        DEF_COLOR(LightYellow,            255,255,224,"亮黄色")
        DEF_COLOR(Lime,                      0,255,  0,"酸橙色")
        DEF_COLOR(LimeGreen,               50,205, 50,"橙绿色")
        DEF_COLOR(Linen,                  250,240,230,"亚麻色")
        DEF_COLOR(Lion,                   193,154,107,"獅子棕")
        DEF_COLOR(Magenta,                255,  0,255,"红紫色")
        DEF_COLOR(Maroon,                 128,  0,  0,"粟色")
        DEF_COLOR(MediumAquamarine,       102,205,170,"间绿色")
        DEF_COLOR(MediumBlue,               0,  0,205,"间兰色")
        DEF_COLOR(MediumOrchid,           186, 85,211,"间淡紫")
        DEF_COLOR(MediumPurple,           147,112,219,"间紫色")
        DEF_COLOR(MediumSeaGreen,          60,179,113,"间海蓝")
        DEF_COLOR(MediumSlateBlue,        123,104,238,"间暗蓝")
        DEF_COLOR(MediumSpringGreen,        0,250,154,"间春绿")
        DEF_COLOR(MediumTurquoise,         72,209,204,"间绿宝石")
        DEF_COLOR(MediumVioletRed,        199, 21,133,"间紫罗兰")
        DEF_COLOR(MidNightBlue,            25, 25,112,"中灰蓝")
        DEF_COLOR(Mint,                    62,180,137,"薄荷色")
        DEF_COLOR(MintCream,              245,255,250,"薄荷霜")
        DEF_COLOR(MintGreen,              152,255,152,"薄荷绿")
        DEF_COLOR(MistyRose,              255,228,225,"浅玫瑰")
        DEF_COLOR(Moccasin,               255,228,181,"鹿皮色")

		DEF_COLOR(MozillaBlue,			  0, 83,159,"火狐蓝")
        DEF_COLOR(MozillaCharcoal,         77, 78, 83,"谋智炭")
		DEF_COLOR(MozillaLightBlue,		  0,150,221,"火狐亮蓝")
		DEF_COLOR(MozillaLightOrange,		255,149,  0,"火狐亮橙")
		DEF_COLOR(MoziilaNightBlue,		  0, 33, 71,"谋智暗夜蓝")
		DEF_COLOR(MozillaOrange,			230, 96,  0,"火狐橙")
        DEF_COLOR(MozillaRed,             193, 56, 50,"谋智红")
        DEF_COLOR(MozillaSand,            215,211,200,"谋智沙")
		DEF_COLOR(MozillaYellow,			255,203,  0,"火狐黄")

        DEF_COLOR(NavajoWhite,            255,222,173,"纳瓦白")
        DEF_COLOR(Navy,                     0,  0,128,"海军色")

        DEF_COLOR(NiveaBlue,                0, 19,111,"妮维雅蓝")     //取自妮维雅蓝国际官网

        DEF_COLOR(NokiaBlue,               18, 65,145,"诺基亚蓝")     //取自诺基亚官网

        DEF_COLOR(OldLace,                253,245,230,"老花色")
        DEF_COLOR(Olive,                  128,128,  0,"橄榄色")
        DEF_COLOR(Olivedrab,              107,142, 35,"深绿褐色")
        DEF_COLOR(Orange,                 255,165,  0,"橙色")
        DEF_COLOR(OrangeRed,              255, 69,  0,"红橙色")
        DEF_COLOR(Orchid,                 218,112,214,"淡紫色")
        DEF_COLOR(PaleGoldenrod,          238,232,170,"苍麒麟色")
        DEF_COLOR(PaleGreen,              152,251,152,"苍绿色")
        DEF_COLOR(PaleTurquoise,          175,238,238,"苍宝石绿")
        DEF_COLOR(PaleVioletRed,          219,112,147,"苍紫罗兰色")
        DEF_COLOR(Papayawhip,             255,239,213,"番木色")
        DEF_COLOR(Peachpuff,              255,218,185,"桃色")
        DEF_COLOR(Pear,                   209,226, 49,"梨色")
        DEF_COLOR(Peru,                   205,133, 63,"秘鲁色")
        DEF_COLOR(Pink,                   255,192,203,"粉红色")

        DEF_COLOR(PlayStationBlue,          0, 55,145,"PlayStation蓝")
        DEF_COLOR(PlayStationLightBlue,     0,120,200,"PlayStation亮蓝")

        DEF_COLOR(Plum,                   221,160,221,"洋李色")
        DEF_COLOR(PowderBlue,             176,224,230,"粉蓝色")
        DEF_COLOR(Purple,                 128,  0,128,"紫色")
        DEF_COLOR(Red,                    255,  0,  0,"红色")
        DEF_COLOR(Rose,                   255,  0,127,"玫瑰红")
        DEF_COLOR(RosyBrown,              188,143,143,"褐玫瑰红")
        DEF_COLOR(RoyalBlue,               65,105,225,"皇家蓝")
        DEF_COLOR(Ruby,                   224, 17, 95,"宝石红")
        DEF_COLOR(SaddleBrown,            139, 69, 19,"重褐色")
        DEF_COLOR(Salmon,                 250,128,114,"鲜肉色")

        DEF_COLOR(SamsungBlue,             20, 40,160,"三星蓝")          //取自三星官网

        DEF_COLOR(SandyBrown,             244,164, 96,"沙褐色")
        DEF_COLOR(SeaGreen,                46,139, 87,"海绿色")
        DEF_COLOR(SeaShell,               255,245,238,"海贝色")
        DEF_COLOR(Sienna,                 160, 82, 45,"赭色")
        DEF_COLOR(Silver,                 192,192,192,"银色")
        DEF_COLOR(SkyBlue,                135,206,235,"天蓝色")
        DEF_COLOR(SlateBlue,              106, 90,205,"石蓝色")
        DEF_COLOR(SlateGray,              112,128,144,"灰石色")
        DEF_COLOR(SlateGrey,              112,128,144,"白灰石色")
        DEF_COLOR(Snow,                   255,250,250,"雪白色")
        DEF_COLOR(SpringGreen,              0,255,127,"春绿色")
        DEF_COLOR(SteelBlue,               70,130,180,"钢兰色")
        DEF_COLOR(Tan,                    210,180,140,"茶色")
        DEF_COLOR(Teal,                     0,128,128,"水鸭色")
        DEF_COLOR(Thistle,                216,191,216,"蓟色")

        DEF_COLOR(TiffanyBlue,            129,216,208,"蒂芙尼蓝")     //取自zh.wikipedia.org/zh-cn/蒂芙尼蓝

        DEF_COLOR(Tomato,                 255, 99, 71,"西红柿色")
        DEF_COLOR(Turquoise,               64,224,208,"青绿色")
        DEF_COLOR(Violet,                 238,130,238,"紫罗兰色")
        DEF_COLOR(Wheat,                  245,222,179,"浅黄色")
        DEF_COLOR(White,                  255,255,255,"白色")
        DEF_COLOR(WhiteSmoke,             245,245,245,"烟白色")
        DEF_COLOR(Yellow,                 255,255,  0,"黄色")
        DEF_COLOR(YellowGreen,            154,205, 50,"黄绿色")

        DEF_COLOR(End,                      0  ,0  ,0,"")
    };

    #undef DEF_COLOR
}//namespace hgl
