//---------------------------------------------------------------------------

#include <vcl.h>
#include<hgl/StringList.H>
#include<hgl/File.H>

#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMainForm *MainForm;

bool SelectDirectory(wchar_t *);
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
    config_file=hgl::GetString(hgl::hfsStartPath);

    config_file+=L"\\TextReplace.INI";

    if(hgl::FileFind(config_file.w_str()))
    {
        hgl::StringList list(config_file.w_str());

        OrignTextPathEdit->Text =list[0].wc_str();
        NewTextPathEdit->Text   =list[1].wc_str();
        NewScriptPathEdit->Text =list[2].wc_str();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::OpenOrignScriptButtonClick(TObject *Sender)
{
    if(OpenTextFileDialog->Execute()==0)return;

    int n=OpenTextFileDialog->Files->Count;

    while(n--)
        ScriptListBox->Items->Add(OpenTextFileDialog->Files->Strings[n]);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ParseTextButtonClick(TObject *Sender)
{
    hgl::StringList *list;
    hgl::StringList *intro;
    int char_count=0;
    int line_count=0;

    if(UseOneCheckBox->Checked)
        intro=new hgl::StringList;

    CharCountLabel->Caption=L"";
    LineCountLabel->Caption=L"";

    for(int i=0;i<ScriptListBox->Items->Count;i++)
    {
        UnicodeString src=ScriptListBox->Items->Strings[i];

        ScriptListBox->ItemIndex=i;

        Application->ProcessMessages();

        list=new hgl::StringList;

        if(UseOneCheckBox->Checked==false)
            intro=new hgl::StringList;

        list->LoadTxt(src.w_str(),hgl::ccpShiftJIS);

        for(int row=0;row<list->Count;row++)
        {
            const wchar_t *str=list->GetString(row);

            const wchar_t *start,*end;

            if(!str)continue;

            while(*str==L' '||*str==L'\t')      //去掉行首空格
                str++;

            if(str[0]==L'/'
             &&str[1]==L'/')continue;           //去掉注释行

            while(true)
            {
                start=hgl::strchr(str,L'\"');

                if(!start)break;

                end=hgl::strchr(start+1,L'\"');

                if(!end)break;

                hgl::String result;

                int len=end-start+1;
                const wchar_t *p=start;

                while((*p>0)&&(*p<128))
                {
                    if(len--)
                        p++;
                    else
                        break;
                }

                if(len>0)
                {
                    if(OperCheckBox->Checked==false)
                    {
                        if(*start==L'"')start++;
                        if(*end==L'"')end--;
                    }

                    if(ClipAscCheckBox->Checked)
                    {
                        while((*start>0)&&(*start<128)) //将前面的英文去掉
                            start++;

                        while((*end>0)&&(*end<128)) //将后面的英文去掉
                            end--;
                    }

                    result.Strcpy(start,end-start+1);

                    if(ClearMultiItemCheckBox->Checked==false
                     ||!intro->IsExist(result))                     //保证无重复
                    {
                        wchar_t *tmp=result.wc_str();
                        int n=result.Length;

                        while(n--)
                            if(*tmp>128)
                                char_count++;

                        CharCountLabel->Caption=IntToStr(char_count);
                        intro->Add(result);

                        line_count++;
                        LineCountLabel->Caption=IntToStr(line_count);
                    }
                }

                str=end+1;
            }
        }

        if(UseOneCheckBox->Checked==false)
        {
            if(intro->Count>0)
            {
                UnicodeString fn;

                ToOrignTextName(src,fn);

                intro->SaveTxt(fn.w_str());
            }

            delete intro;
        }

        delete list;
    }

    if(UseOneCheckBox->Checked)
    {
        intro->SaveTxt(OrignTextPathEdit->Text.w_str());

        delete intro;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::OrignTextPathBrowserButtonClick(TObject *Sender)
{
    if(UseOneCheckBox->Checked)
    {
        OpenTextFileDialog->FileName=OrignTextPathEdit->Text;

        if(OpenTextFileDialog->Execute())
            OrignTextPathEdit->Text=OpenTextFileDialog->FileName;
    }
    else
    {
        wchar_t str[1024];

        wcscpy(str,OrignTextPathEdit->Text.w_str());

        if(SelectDirectory(str))
            OrignTextPathEdit->Text=str;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::NewTextPathBrowserButtonClick(TObject *Sender)
{
    if(UseOneCheckBox->Checked)
    {
        OpenTextFileDialog->FileName=NewTextPathEdit->Text;

        if(OpenTextFileDialog->Execute())
            NewTextPathEdit->Text=OpenTextFileDialog->FileName;
    }
    else
    {
        wchar_t str[1024];

        wcscpy(str,NewTextPathEdit->Text.w_str());

        if(SelectDirectory(str))
            NewTextPathEdit->Text=str;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::NewScriptPathBrowserButtonClick(TObject *Sender)
{
    wchar_t str[1024];

    wcscpy(str,NewScriptPathEdit->Text.w_str());

    if(SelectDirectory(str))
        NewScriptPathEdit->Text=str;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
    hgl::StringList list;

    list.Add(OrignTextPathEdit  ->Text.c_str());
    list.Add(NewTextPathEdit    ->Text.c_str());
    list.Add(NewScriptPathEdit  ->Text.c_str());

    list.SaveTxt(config_file.w_str());
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ReplaceTextButtonClick(TObject *Sender)
{
    hgl::StringList *orign_text;
    hgl::StringList *new_text;

    if(UseOneCheckBox->Checked)
    {
        orign_text=new hgl::StringList(OrignTextPathEdit->Text.w_str());
        new_text=new hgl::StringList(NewTextPathEdit->Text.w_str());
    }

    for(int i=0;i<ScriptListBox->Items->Count;i++)
    {
        UnicodeString src=ScriptListBox->Items->Strings[i];

        ScriptListBox->ItemIndex=i;

        Application->ProcessMessages();

        if(UseOneCheckBox->Checked==false)
        {
            orign_text=GetOrignText(src);

            if(!orign_text)
                continue;

            new_text=GetNewText(src);

            if(!new_text)
            {
                delete orign_text;
                continue;
            }
        }

        hgl::StringList *list=new hgl::StringList;
        hgl::StringList *intro=new hgl::StringList;

        list->LoadTxt(src.w_str(),hgl::ccpShiftJIS);

        for(int row=0;row<list->Count;row++)
        {
            const wchar_t *str=list->GetString(row);
            const wchar_t *start,*end;

            const wchar_t *new_str=NULL;

            if(!str)
            {
                intro->Add(L"");
                continue;
            }

            while(*str==L' '||*str==L'\t')      //去掉行首空格
                str++;

            if(str[0]==L'/'
             &&str[1]==L'/')continue;           //去掉注释行

            while(true)
            {
                start=hgl::strchr(str,L'\"');

                if(!start)break;

                end=hgl::strchr(start+1,L'\"');

                if(!end)break;

                int len=end-start-1;
                const wchar_t *p=start+1;

                while((*p>0)&&(*p<128))
                {
                    if(len--)
                        p++;
                    else
                        break;
                }

                if(len>0)               //替换的脚本必须保证每行只有一条需要替换的字符串
                {
                    int index;

                    hgl::String result;

                    if(OperCheckBox->Checked==false)
                    {
                        if(*start==L'"')start++;
                        if(*end==L'"')end--;
                    }

                    if(ClipAscCheckBox->Checked)
                    {
                        while((*start>0)&&(*start<128)) //将前面的英文去掉
                            start++;

                        while(*end>0&&(*end<128))   //将后面的英文去掉
                            end--;
                    }

                    result.Strcpy(start,end-start+1);

                    index=orign_text->Find(result);         //查找这个字符串是否存在列表中

                    if(index!=-1)
                    {
                        new_str=new_text->GetString(index);

                        break;
                    }

//                  intro->Add(result);
                }

                str=end+1;
            }

            if(new_str&&hgl::strlen(new_str)>0)
            {
                const wchar_t *orign=list->GetString(row);

                hgl::String result;

                result.Strcpy(orign,start-orign);
                result.Strcat(new_str);
                result.Strcat(end+1);

                intro->Add(result);
            }
            else        //原样
            {
                intro->Add(list->GetString(row));
            }
        }

        if(intro->Count>0)
        {
            UnicodeString fn;

            ToNewScriptName(src,fn);

            intro->SaveTxt(fn.w_str());
        }

        if(UseOneCheckBox->Checked==false)
        {
            delete orign_text;
            delete new_text;
        }

        delete intro;
        delete list;
    }

    if(UseOneCheckBox->Checked)
    {
        delete orign_text;
        delete new_text;
    }
}
//---------------------------------------------------------------------------
void TMainForm::ToOrignTextName(const UnicodeString &orign_filename,UnicodeString &new_filename)
{
//  ReplacePathAndExt(orign_filename,OrignTextPathEdit->Text,L"TXT",new_filename);

    wchar_t tar[1024];

    UnicodeString new_path=OrignTextPathEdit->Text;

    hgl::strcpy(tar,new_path.w_str());
    hgl::strcat(tar,hgl::strrchr(orign_filename.w_str(),L'\\'));

    hgl::strcpy(hgl::strrchr(tar,L'.'),L".TXT");

    new_filename=tar;
}
//---------------------------------------------------------------------------
void TMainForm::ToNewTextName(const UnicodeString &orign_filename,UnicodeString &new_filename)
{
    wchar_t tar[1024];

    UnicodeString new_path=NewTextPathEdit->Text;

    hgl::strcpy(tar,new_path.w_str());
    hgl::strcat(tar,hgl::strrchr(orign_filename.w_str(),L'\\'));

    hgl::strcpy(hgl::strrchr(tar,L'.'),L".TXT");

    new_filename=tar;
}
//---------------------------------------------------------------------------
void TMainForm::ToNewScriptName(const UnicodeString &orign_filename,UnicodeString &new_filename)
{
    ReplacePathAndExt(orign_filename,NewScriptPathEdit->Text,L"CPP",new_filename);
}
//---------------------------------------------------------------------------
void TMainForm::ReplacePathAndExt(const UnicodeString &orign_filename,const UnicodeString &new_path,const UnicodeString &new_ext,UnicodeString &new_filename)
{
    wchar_t tar[1024];

    hgl::strcpy(tar,new_path.w_str());
    hgl::strcat(tar,hgl::strrchr(orign_filename.w_str(),L'\\'));

    hgl::ReplaceExtName(tar,new_ext.w_str());

    new_filename=tar;
}
//---------------------------------------------------------------------------
hgl::StringList *TMainForm::GetOrignText(const UnicodeString &orign_filename)
{
    UnicodeString fn;

    ToOrignTextName(orign_filename,fn);

    if(hgl::FileFind(fn.w_str()))
    {
        hgl::StringList *list=new hgl::StringList;

        list->LoadTxt(fn.w_str());

        return list;
    }
    else
        return(NULL);
}
//---------------------------------------------------------------------------
hgl::StringList *TMainForm::GetNewText(const UnicodeString &orign_filename)
{
    UnicodeString fn;

    ToNewTextName(orign_filename,fn);

    if(hgl::FileFind(fn.w_str()))
    {
        hgl::StringList *list=new hgl::StringList;

        list->LoadTxt(fn.w_str());

        return list;
    }
    else
        return(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::UseOneCheckBoxClick(TObject *Sender)
{
    if(UseOneCheckBox->Checked)
    {
        Label1->Caption=L"提取后的文本文件:";
        Label2->Caption=L"翻译后的文本文件:";
    }
    else
    {
        Label1->Caption=L"提取后文本存放目录:";
        Label2->Caption=L"翻译文本存放目录:";
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button2Click(TObject *Sender)
{
    hgl::StringList old_text;
    hgl::StringList new_text;

    for(int i=0;i<ScriptListBox->Items->Count;i++)
    {
        UnicodeString fn=ScriptListBox->Items->Strings[i];

        ScriptListBox->ItemIndex=i;

        Application->ProcessMessages();

        old_text.Clear();
        new_text.Clear();

        old_text.LoadTxt(fn.w_str());

        for(int row=0;row<old_text.Count;row++)
        {
            const wchar_t *str=old_text.GetString(row);

            const wchar_t *start,*end;

            if(!str)continue;

            if(str[0]==L'/'
             &&str[1]==L'/')continue;           //去掉注释行

            hgl::String result;

            start=str;
            end=start+hgl::strlen(str)-1;

            while((*start>0)&&(*start<128)) //将前面的英文去掉
                start++;

            while((*end>0)&&(*end<128)) //将后面的英文去掉
                end--;

            if(end>=start)
            {
                result.Strcpy(start,end-start+1);

                if(ClearMultiItemCheckBox->Checked==false
                 ||new_text.Find(result)==-1)
                    new_text.Add(result);
            }
        }

        if(new_text.Count)
        {
            UnicodeString new_filename;

            ToOrignTextName(fn,new_filename);

            new_text.SaveTxt(new_filename.w_str());
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Button1Click(TObject *Sender)
{
    ScriptListBox->Items->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button3Click(TObject *Sender)
{
    wchar_t find_dir[1024];

    if(!SelectDirectory(find_dir))return;

    ShowMessage(find_dir);
}
//---------------------------------------------------------------------------

