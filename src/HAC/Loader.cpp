#include<hgl/thread/Loader.h>
#include<hgl/hac.h>

namespace hgl
{
    Loader::Loader()
    {
        loader_hac=0;
        loader_filename.Clear();
        loader_pointer=0;
        loader_data=0;
        loader_size=0;

        status=lsNone;

        OnError=0;
        OnEnd=0;
    }

    Loader::LoaderStatus Loader::GetStatus()
    {
        LoaderStatus ls;

        ThreadMutex::Lock();
            ls=status;
        ThreadMutex::Unlock();

        return ls;
    }

    void Loader::SetStatus(LoaderStatus ls)
    {
        ThreadMutex::Lock();
            status=ls;
        ThreadMutex::Unlock();
    }

    bool Loader::Load(HAC *hac,const u16char *filename)
    {
        loader_hac=hac;
        loader_filename=filename;

        loader_pointer=hac->LoadAcquire(filename);

        SetStatus(lsLoad);

        return loader_pointer;
    }

    bool Loader::Execute()
    {
        LoaderStatus ls=GetStatus();

        if(ls==lsProc)
        {
            loader_data=loader_hac->GetData(loader_pointer);
            loader_size=loader_hac->GetSize(loader_pointer);

            if(loader_data&&loader_size>0)
                ProcFinish();

            SetStatus(lsProcFinish);
        }
        else
        if(ls==lsClear)
        {
            ProcClear();

            SetStatus(lsClearFinish);
        }

        return(false);
    }

    void Loader::Update()
    {
        if(!ThreadMutex::TryLock())return;

        LoaderStatus ls=status;

        ThreadMutex::Unlock();

        if(ls==lsNone)
        {
            Destroy();
            return;
        }

        if(ls==lsLoad)
        {
            HacStatus hs;

            hs=loader_hac->GetStatus(loader_pointer);

            if(hs==hsNotFind
             ||hs==hsOpenError
             ||hs==hsReadError)
            {
                SafeCallEvent(OnError,(this));

                Destroy();
            }
            else
            if(hs==hsEnd)
            {
                SetStatus(lsProc);

                Thread::Start();        //启动线程在另一线程处理数据
            }
        }
        else
        if(ls==lsProcFinish)
        {
            ProcEnd();

            SafeCallEvent(OnEnd,(this));

            SetStatus(lsClear);

            Thread::Start();
        }
        else
        if(ls==lsClearFinish)
        {
            loader_hac->Clear(loader_pointer);

            loader_pointer=0;
            loader_data=0;

            status=lsEnd;

            Destroy();
        }
    }
}
