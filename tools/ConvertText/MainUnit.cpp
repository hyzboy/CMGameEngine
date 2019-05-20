//---------------------------------------------------------------------------

#include <vcl.h>
#include<hgl/vcl.h>
#include<hgl/Other.H>
#include<hgl/Str.H>
#include<hgl/StringList.H>
#include<hgl/ut/PList.H>
#pragma hdrstop

#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMainForm *MainForm;

bool SelectFolder(hgl::String &);

using namespace hgl;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
	code_page=0;

	config_file=UnicodeString(GetString(hfsStartPath))+UnicodeString(L"\\ConvertText.list");

	list_file=UnicodeString(GetString(hfsStartPath))+UnicodeString(L"\\FileType.list");

	if(FileExist(list_file.w_str()))
		FileTypeMemo->Lines->LoadFromFile(list_file.w_str());

	if(!FileExist(config_file.w_str()))return;

	PList list;
	hgl::String str;
	int index;

	list.LoadTxt(config_file.w_str());

	if(list.Get(L"CodeIndex",str))
	{
		index=str.ToInt();

		if(index>=0&&index<CodePageGroup->Items->Count)
			CodePageGroup->ItemIndex=index;
	}

	if(list.Get(L"LastPath",str))
	{
		RootPathEdit->Text=str.wc_str();
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Button2Click(TObject *Sender)
{
	hgl::String path=RootPathEdit->Text.w_str();

	if(SelectFolder(path))
		RootPathEdit->Text=path.wc_str();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CodePageGroupClick(TObject *Sender)
{
	const int cp[]=
	{
		ccpGBK,
		ccpBig5,
		ccpGB2312,
		ccpHZGB2312,
		ccpGB18030,
		ccpShiftJIS,
		ccpJISX,
		ccpUTF7,
		ccpUTF8
	};

	if(CodePageGroup->ItemIndex==-1)
		code_page=0;
	else
		code_page=cp[CodePageGroup->ItemIndex];
}
//---------------------------------------------------------------------------
void TMainForm::ConvertQtoB(wchar_t *str,int len)
{
	const wchar_t QC[]=L"ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ１２３４５６７８９０";
	const wchar_t BC[]=L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";

	int tl=sizeof(QC)/sizeof(wchar_t)-1;

	wchar_t *p=str;

	while(len--)
	{
		for(int i=0;i<tl;i++)
		{
			if(*p==QC[i])
			{
				*p=BC[i];
				break;
			}
		}

		p++;
	}
}
//---------------------------------------------------------------------------
void TMainForm::ConvertShiftJIS_BtoQ(wchar_t *str,int len)
{
}
//---------------------------------------------------------------------------
void TMainForm::ConvertFile(const UnicodeString &filename)
{
	hgl::StringList list;

	list.LoadTxt(filename.w_str(),(CCP)code_page);

//	if(ProcOnlyRead->Checked)

	if(ConvQtoBCheckBox->Checked)		//转换全角英文到半角
	{
		int n=list.Count;

		while(n--)
			ConvertQtoB(list[n].wc_str(),list[n].Length);
	}

	if(ShiftJisConverCheckBox->Checked)	//转换半角日文到全角日文
	{
		int n=list.Count;

		while(n--)
			ConvertShiftJIS_BtoQ(list[n].wc_str(),list[n].Length);
	}

	list.SaveTxt(filename.w_str());

	StatusBar1->SimpleText=filename;
}
//---------------------------------------------------------------------------
void TMainForm::ConvertFile(const UnicodeString &base,const UnicodeString &ext)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	UnicodeString find_name=base+L"\\"+ext;

	hFind = FindFirstFile(find_name.w_str(), &wfd);

	if(hFind == INVALID_HANDLE_VALUE)return;

	do
	{
		if(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)continue;

		UnicodeString full_filename=base+L"\\"+UnicodeString(wfd.cFileName);

		if(wfd.dwFileAttributes&FILE_ATTRIBUTE_READONLY)
		{
			if(ProcOnlyRead->Checked)
			{
				//删去只读属性

				if(!SetFileAttributes(full_filename.w_str(),wfd.dwFileAttributes^FILE_ATTRIBUTE_READONLY))
					continue;	//删去只读属性失败
			}
			else
				continue;
		}

		ConvertFile(full_filename);

		Application->ProcessMessages();
	}
	while(FindNextFile(hFind, &wfd)!=0);

	FindClose(hFind);
}
//---------------------------------------------------------------------------
void TMainForm::ConvertSubFolder(const UnicodeString &base)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	UnicodeString find_name=base+L"\\*.*";

	hFind = FindFirstFile(find_name.w_str(), &wfd);

	if(hFind == INVALID_HANDLE_VALUE)return;

	do
	{
		if(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			if(wfd.cFileName[0]!=L'.')
				ConvertFolder(base+L"\\"+UnicodeString(wfd.cFileName));

		Application->ProcessMessages();
	}
	while(FindNextFile(hFind, &wfd)!=0);

	FindClose(hFind);
}
//---------------------------------------------------------------------------
void TMainForm::ConvertFolder(const UnicodeString &base)
{
	if(SubFolderCheckBox->Checked)
		ConvertSubFolder(base);

	for(int i=0;i<FileTypeMemo->Lines->Count;i++)
	{
		UnicodeString ext=FileTypeMemo->Lines->Strings[i];

		if(ext.Length())
			ConvertFile(base,ext);

		Application->ProcessMessages();
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Button1Click(TObject *Sender)
{
	ConvertFolder(RootPathEdit->Text);

	StatusBar1->SimpleText=L"转换完成！";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
	PList list;
	hgl::String str;

	str=CodePageGroup->ItemIndex;
	list.Set(L"CodeIndex",str);

	str=RootPathEdit->Text.c_str();
	list.Set(L"LastPath",str);

	list.SaveTxt(config_file.w_str());

	FileTypeMemo->Lines->SaveToFile(list_file.w_str());
}
//---------------------------------------------------------------------------
void TMainForm::ConvertName(const UnicodeString &wn,const AnsiString &an)
{
	WIN32_FIND_DATAA wfd;
	HANDLE hFind;

	{
		AnsiString find_name=an+"\\*.*";

		hFind = FindFirstFileA(find_name.c_str(), &wfd);

		if(hFind == INVALID_HANDLE_VALUE)return;

		do
		{
			if(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				AnsiString san=an+"\\"+AnsiString(wfd.cFileName);
				UnicodeString swn(san);

				if(wfd.cFileName[0]!='.')
					ConvertName(swn,san);
			}
			else
			{
				AnsiString san=an+"\\"+AnsiString(wfd.cFileName);
				UnicodeString swn(san);

				hgl::String nfn((CCP)code_page,wfd.cFileName);

				hgl::String new_name(an.c_str());

				new_name+=L"\\"+nfn;

				if(swn!=new_name.wc_str())
					hgl::FileRename(swn.w_str(),new_name);
			}

			Application->ProcessMessages();
		}
		while(FindNextFileA(hFind, &wfd)!=0);

		FindClose(hFind);
	}

	{
		hFind = FindFirstFileA(an.c_str(), &wfd);

		if(hFind == INVALID_HANDLE_VALUE)return;

		hgl::String nfn((CCP)code_page,wfd.cFileName);
		hgl::String new_name;

		char str[1024];

		strcpy(str,an.c_str());
		*(strrchr(str,'\\')+1)=0;

		new_name=str;
		new_name+=nfn;

		FindClose(hFind);

		if(wn!=new_name.wc_str())
			hgl::FileRename(wn.w_str(),new_name.wc_str());
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Button3Click(TObject *Sender)
{
	ConvertName(UnicodeString(RootPathEdit->Text),RootPathEdit->Text);

	StatusBar1->SimpleText=L"转换完成！";
}
//---------------------------------------------------------------------------
void TMainForm::FilterName(const UnicodeString &name)
{
	const wchar_t old_char[] = {  L'｡', L'｢', L'｣', L'､', L'･',
								  L'ｦ', L'ｧ', L'ｨ', L'ｩ', L'ｪ', L'ｫ', L'ｬ', L'ｭ', L'ｮ', L'ｯ', L'ｰ', L'ｱ', L'ｲ',
								  L'ｳ', L'ｴ', L'ｵ', L'ｶ', L'ｷ', L'ｸ', L'ｹ', L'ｺ', L'ｻ', L'ｼ', L'ｽ', L'ｾ', L'ｿ',
								  L'ﾀ', L'ﾁ', L'ﾂ', L'ﾃ', L'ﾄ', L'ﾅ', L'ﾆ', L'ﾇ', L'ﾈ', L'ﾉ', L'ﾊ', L'ﾋ', L'ﾌ',
								  L'ﾍ', L'ﾎ', L'ﾏ', L'ﾐ', L'ﾑ', L'ﾒ', L'ﾓ', L'ﾔ', L'ﾕ', L'ﾖ', L'ﾗ', L'ﾘ', L'ﾙ',
								  L'ﾚ', L'ﾛ', L'ﾜ', L'ﾝ', L'ﾞ', L'ﾟ' ,0};

	const wchar_t new_char[] = {  L'。', L'「', L'」', L'、', L'・',
								  L'ヲ', L'ァ', L'ィ', L'ゥ', L'ェ', L'ォ', L'ャ', L'ュ', L'ョ', L'ッ', L'ー', L'ア', L'イ',
								  L'ウ', L'エ', L'オ', L'カ', L'キ', L'ク', L'ケ', L'コ', L'サ', L'シ', L'ス', L'セ', L'ソ',
								  L'タ', L'チ', L'ツ', L'テ', L'ト', L'ナ', L'ニ', L'ヌ', L'ネ', L'ノ', L'ハ', L'ヒ', L'フ',
								  L'ヘ', L'ホ', L'マ', L'ミ', L'ム', L'メ', L'モ', L'ヤ', L'ユ', L'ヨ', L'ラ', L'リ', L'ル',
								  L'レ', L'ロ', L'ワ', L'ン', L'゛', L'゜' ,0};

	WIN32_FIND_DATA wfd;
	HANDLE hFind;

	UnicodeString find_name=name+"\\*.*";

	hFind = FindFirstFile(find_name.w_str(), &wfd);

	if(hFind == INVALID_HANDLE_VALUE)return;

	do
	{
		if(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			if(wfd.cFileName[0]=='.')
            	continue;

			FilterName(name+L"\\"+UnicodeString(wfd.cFileName));
		}

		{
			hgl::String old_name;
			hgl::String new_name;
			int n=0;
			wchar_t str[1024];
			wchar_t *sp,*tp;

			old_name=name.w_str();
			old_name+=L"\\";
			old_name+=wfd.cFileName;

			StatusBar1->SimpleText=new_name.wc_str();
			Application->ProcessMessages();

			sp=wfd.cFileName;
			tp=str;

			//合并字符串
			while(*sp)
			{
				if(*(sp+1)==L'ﾟ')
				{
					if(*sp==L'ﾊ'){*tp++=L'パ';sp+=2;continue;}else
					if(*sp==L'ﾋ'){*tp++=L'ピ';sp+=2;continue;}else
					if(*sp==L'ﾌ'){*tp++=L'プ';sp+=2;continue;}else
					if(*sp==L'ﾍ'){*tp++=L'ペ';sp+=2;continue;}else
					if(*sp==L'ﾎ'){*tp++=L'ポ';sp+=2;continue;}
				}
				else
				if(*(sp+1)==L'ﾞ')
				{
					if(*sp==L'ｶ'){*tp++=L'ガ';sp+=2;continue;}else
					if(*sp==L'ｷ'){*tp++=L'ギ';sp+=2;continue;}else
					if(*sp==L'ｸ'){*tp++=L'グ';sp+=2;continue;}else
					if(*sp==L'ｹ'){*tp++=L'ゲ';sp+=2;continue;}else
					if(*sp==L'ｺ'){*tp++=L'ゴ';sp+=2;continue;}else
					if(*sp==L'ｻ'){*tp++=L'ザ';sp+=2;continue;}else
					if(*sp==L'ｼ'){*tp++=L'ジ';sp+=2;continue;}else
					if(*sp==L'ｽ'){*tp++=L'ズ';sp+=2;continue;}else
					if(*sp==L'ｾ'){*tp++=L'ゼ';sp+=2;continue;}else
					if(*sp==L'ｿ'){*tp++=L'ゾ';sp+=2;continue;}else
					if(*sp==L'ﾀ'){*tp++=L'ダ';sp+=2;continue;}else
					if(*sp==L'ﾁ'){*tp++=L'ヂ';sp+=2;continue;}else
					if(*sp==L'ﾂ'){*tp++=L'ヅ';sp+=2;continue;}else
					if(*sp==L'ﾃ'){*tp++=L'デ';sp+=2;continue;}else
					if(*sp==L'ﾄ'){*tp++=L'ド';sp+=2;continue;}else
					if(*sp==L'ﾊ'){*tp++=L'バ';sp+=2;continue;}else
					if(*sp==L'ﾋ'){*tp++=L'ビ';sp+=2;continue;}else
					if(*sp==L'ﾌ'){*tp++=L'ブ';sp+=2;continue;}else
					if(*sp==L'ﾍ'){*tp++=L'ベ';sp+=2;continue;}else
					if(*sp==L'ﾎ'){*tp++=L'ボ';sp+=2;continue;}
				}

				//按替换表替换
				n=0;
				while(old_char[n])
				{
					if(*sp==old_char[n])
					{
						*tp++=new_char[n];
						sp++;

						n=-1;
						break;
					}

					n++;
				}

				if(n==-1)continue;

				if(old_char[n]==0)	//没有可替换的
					*tp++=*sp++;
			}

			*tp=0;

			new_name=name.w_str();
			new_name+=L"\\";
			new_name+=str;

			if(new_name!=old_name)
				FileRename(old_name,new_name);
		}

		Application->ProcessMessages();
	}
	while(FindNextFile(hFind, &wfd)!=0);

	FindClose(hFind);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Button4Click(TObject *Sender)
{
	FilterName(RootPathEdit->Text);

	StatusBar1->SimpleText=L"转换完成！";
}
//---------------------------------------------------------------------------

