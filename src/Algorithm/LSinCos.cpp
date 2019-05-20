namespace hgl
{
    namespace algorithm
    {
        constexpr unsigned int SINV[91] = { 0,
            17452406, 34899496, 52335956, 69756473, 87155742, 104528463,
            121869343, 139173100, 156434465, 173648177, 190808995, 207911690,
            224951054, 241921895, 258819045, 275637355, 292371704, 309016994,
            325568154, 342020143, 358367949, 374606593, 390731128, 406736643,
            422618261, 438371146, 453990499, 469471562, 484809620, 500000000,
            515038074, 529919264, 544639035, 559192903, 573576436, 587785252,
            601815023, 615661475, 629320391, 642787609, 656059028, 669130606,
            681998360, 694658370, 707106781, 719339800, 731353701, 743144825,
            754709580, 766044443, 777145961, 788010753, 798635510, 809016994,
            819152044, 829037572, 838670567, 848048096, 857167300, 866025403,
            874619707, 882947592, 891006524, 898794046, 906307787, 913545457,
            920504853, 927183854, 933580426, 939692620, 945518575, 951056516,
            956304755, 961261695, 965925826, 970295726, 974370064, 978147600,
            981627183, 984807753, 987688340, 990268068, 992546151, 994521895,
            996194698, 997564050, 998629534, 999390827, 999847695, 1000000000 };

        double Lsin(int angle)
        {
            double sf;

            if (angle > 360 || angle < -360)angle = angle - (angle / 360) * 360;
            if (angle < 0)angle = 360 + angle;
            if (angle > 180)sf = -1; else sf = 1;

            if (angle > 90 && angle <= 180)angle = 180 - angle; else
                if (angle > 180 && angle <= 270)angle = angle - 180; else
                    if (angle > 270)angle = 360 - angle;

            return sf*SINV[angle] / 10.0e8;
        }

        double Lcos(int angle)
        {
            double cf;

            if (angle > 360 || angle < -360)angle = angle - (angle / 360) * 360;
            if (angle < 0)angle = 360 + angle;
            if (angle < 270 && angle>90)cf = -1; else cf = 1;

            if (angle > 90 && angle <= 180)angle = 180 - angle; else
                if (angle > 180 && angle <= 270)angle = angle - 180; else
                    if (angle > 270)angle = 360 - angle;

            return cf*SINV[90 - angle] / 10.0e8;
        }

        void Lsincos(int angle, double &s, double &c)
        {
            double sf;
            double cf;

            if (angle > 360 || angle < -360)angle = angle - (angle / 360) * 360;
            if (angle < 0)angle = 360 + angle;
            if (angle > 180)sf = -1; else sf = 1;
            if (angle < 270 && angle>90)cf = -1; else cf = 1;

            if (angle > 90 && angle <= 180)angle = 180 - angle; else
                if (angle > 180 && angle <= 270)angle = angle - 180; else
                    if (angle > 270)angle = 360 - angle;

            s = sf*SINV[angle] / 10.0e8;
            c = cf*SINV[90 - angle] / 10.0e8;
        }
    }//namespace algorithm
}//namespace hgl
