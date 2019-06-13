#include<curl/curl.h>
#include<string>
#include<cstring>

struct HTTPDownload
{
    char *buffer;
    size_t size;
    size_t pos;

public:

    HTTPDownload(char *b,size_t s)
    {
        buffer=b;
        size=s;
        pos=0;
    }
};

size_t http_get_to_buffer(void *ptr,size_t size,size_t number,void *stream)
{
    HTTPDownload *ht=(HTTPDownload *)stream;

    size_t bytes=size*number;

    if(ht->pos+bytes>ht->size)
        bytes=ht->size-ht->pos;

    memcpy(ht->buffer+ht->pos,ptr,bytes);
    ht->pos+=bytes;

    return bytes;
}

bool QueryFileLength(const std::string &url,long &downloadFileLenth)
{
    CURL *handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_HEADER, 1);    //只要求header头
    curl_easy_setopt(handle, CURLOPT_NOBODY, 1);    //不需求body

    bool result=false;

    if (curl_easy_perform(handle) == CURLE_OK)
    {
        result=true;

        double length;

        curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &length);

        downloadFileLenth=length;
    }
    else
    {
        downloadFileLenth = -1;
    }

    curl_easy_cleanup(handle);

    return(result);
}

int Download(const std::string &url,char *buf,size_t size)
{
    CURLcode res;

    CURL *curl=curl_easy_init();

    if(!curl)
        return(-1);

    HTTPDownload hd(buf,size);

    curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,&hd);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,http_get_to_buffer);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return(hd.pos);
}
