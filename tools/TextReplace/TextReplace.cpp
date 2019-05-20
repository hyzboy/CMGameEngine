//---------------------------------------------------------------------------

#include <vcl.h>         
#include<hgl/vcl.h>
//---------------------------------------------------------------------------
USEFORM("MainUnit.cpp", MainForm);
USEFORM("SelectFolderUnit.cpp", SelectFolderForm);
//---------------------------------------------------------------------------
WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	try
	{                           
		hgl::InitSystem();

		Application->Initialize();
		SetApplicationMainFormOnTaskBar(Application, true);
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->Run();
		
		hgl::CloseSystem();
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
	return 0;
}
//---------------------------------------------------------------------------
