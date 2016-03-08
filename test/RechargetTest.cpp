#include<curl/curl.h>
#include<hgl/ut/Hash.h>
#include<hgl/Str.h>
#include<hgl/type/BaseString.h>

using namespace hgl;
using namespace hgl::util;

namespace
{
	UTF8String recharge_url="http://www.5ebo.com/ebo_new/test.php/";
	UTF8String recharge_key="5ebomzdg";
}//namespace

void LoadRechargeConfig(const char **atts)
{
	while(*atts)
	{
		const char *flag=*atts;atts++;
		const char *info=*atts;atts++;

		if(strcmp(flag,"url")==0)recharge_url=info;
		if(strcmp(flag,"key")==0)recharge_key=info;
		else
			continue;
	}
}

#define RECHANGE_5EBO_ACQUIRE	true
#define RECHANGE_5EBO_CONFIRM	false

struct recharge_result
{
	char buf[32];
	int pos;

	recharge_result()
	{
		pos=0;
	}
};

size_t get_recharge_result(void *ptr,size_t size,size_t number,void *stream)
{
	recharge_result *rr=(recharge_result *)stream;

	int ss=size*number;

	if(ss+rr->pos>32)
		ss=32-rr->pos;

	memcpy(rr->buf+rr->pos,ptr,ss);
	rr->pos+=ss;

	return ss;
}

bool Recharge5EBO(const UTF8String &username,const int money,const int type,const bool acquire_or_confirm)
{
	UTF8String check_str=username+UTF8String(money)+recharge_key;

	MD5Code md5;
	CountMD5(check_str.c_str(),check_str.Length(),md5);

	char check_code_str[33];

    ToLowerHexStr(check_code_str,&md5,md5.size());
	check_code_str[32]=0;

	UTF8String url=recharge_url+"?user="+username+"&money="+UTF8String(money)+UTF8String(type==0?"&type=coin":"&type=vouchers")+UTF8String(acquire_or_confirm?"&proc=acquire":"&proc=confirm")+"&check="+check_code_str;

	CURLcode res;
	CURL *curl=curl_easy_init();

	if(!curl)
		return(false);

	std::cout<<"Recharge URL: "<<url.c_str()<<std::endl;

	recharge_result rr;

	curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,&rr);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,get_recharge_result);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	if(rr.pos<=0)return(false);

	rr.buf[rr.pos]=0;

	std::cout<<"Recharge Result: "<<rr.buf<<std::endl;

	if(hgl::stricmp(rr.buf,"true",4)==0)
		return(true);

	return(false);
}

bool AcquireRecharge(const UTF8String &username,const int money,const int type){return Recharge5EBO(username,money,type,RECHANGE_5EBO_ACQUIRE);}
bool ConfirmRecharge(const UTF8String &username,const int money,const int type){return Recharge5EBO(username,money,type,RECHANGE_5EBO_CONFIRM);}

int main(int,char **)
{
	curl_global_init(CURL_GLOBAL_ALL);

	AcquireRecharge("hyzboy",100,0);

	return(0);
}
