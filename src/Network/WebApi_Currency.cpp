#include<hgl/finance/Currency.h>
#include<hgl/Str.h>
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/network/HTTPTools.h>
namespace hgl
{
    namespace finance
    {
        float GetCurrencyExchangeRate(const char target[4],const char source[4])
        {
            if(strlen(target)!=3
                ||strlen(source)!=3)return(0);

            char url[]="http://download.finance.yahoo.com/d/quotes.csv?s=******=X&f=l1&e=.csv";

            memcpy(url+49,source,3);
            memcpy(url+52,target,3);

            hgl::io::MemoryOutputStream mos;

            if(hgl::network::http::get(&mos,url)<=0)
                return 0;

            float result;

            if(stof((char *)(mos.GetData()),result))
                return result;

            return 0;
        }
    }//namespace finance
}//namespace hgl
