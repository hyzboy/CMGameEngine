//---------------------------------------------------------------------------

#include <vcl.h>
#include <dir.h>
#include<hgl/Other.H>

#include "SelectFolderUnit.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"

using namespace hgl;
//---------------------------------------------------------------------------
bool SelectDirectory(wchar_t *str)
{
    TSelectFolderForm *form=new TSelectFolderForm(NULL);
    bool result=false;

    if(IsDirectory(str))
        form->DirectoryListBox->Directory=str;

    form->ShowModal();

    if(form->FolderName.Length())
    {
        result=true;
        strcpy(str,form->FolderName.c_bstr());
    }

    delete form;

    return(result);
}
//---------------------------------------------------------------------------
__fastcall TSelectFolderForm::TSelectFolderForm(TComponent* Owner)
    : TForm(Owner)
{
    FolderName=L"";
}
//---------------------------------------------------------------------------
void __fastcall TSelectFolderForm::Button2Click(TObject *Sender)
{
    FolderName=L"";

    Close();
}
//---------------------------------------------------------------------------
void __fastcall TSelectFolderForm::Button1Click(TObject *Sender)
{
    FolderName=DirectoryListBox->Directory;

    Close();
}
//---------------------------------------------------------------------------
void __fastcall TSelectFolderForm::Button3Click(TObject *Sender)
{
    WideString folder_name;

    folder_name=InputBox(L"��������",L"������Ŀ¼����",L"");

    if(folder_name=="")return;

    hgl::MakePath((DirectoryListBox->Directory+"\\"+folder_name).c_str());

    DirectoryListBox->Update();
}
//---------------------------------------------------------------------------


