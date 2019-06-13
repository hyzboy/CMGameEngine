//---------------------------------------------------------------------------

#include <vcl.h>
#include<hgl/vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("MainUnit.cpp", MainForm);
USEFORM("..\Common\SelectFolderUnit.cpp", SelectFolderForm);
//---------------------------------------------------------------------------
WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    hgl::InitSystem(true,false);

    try
    {
        Application->Initialize();
        SetApplicationMainFormOnTaskBar(Application, true);
        Application->CreateForm(__classid(TMainForm), &MainForm);
        Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    catch (...)
    {
        try
        {
            throw Exception("");
        }
        catch (Exception &exception)
        {
            Application->ShowException(&exception);
        }
    }

    hgl::CloseSystem();
    return 0;
}
//---------------------------------------------------------------------------
