//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <XPMan.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TRadioGroup *CodePageGroup;
	TCheckBox *SubFolderCheckBox;
	TGroupBox *GroupBox1;
	TMemo *FileTypeMemo;
	TLabel *Label1;
	TEdit *RootPathEdit;
	TButton *Button1;
	TButton *Button2;
	TStatusBar *StatusBar1;
	TButton *Button3;
	TButton *Button4;
	TCheckBox *ProcOnlyRead;
	TXPManifest *XPManifest1;
	TCheckBox *ConvQtoBCheckBox;
	TCheckBox *ShiftJisConverCheckBox;
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall CodePageGroupClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
private:	// User declarations

	UnicodeString config_file;
	UnicodeString list_file;

	void ConvertQtoB(wchar_t *str,int len);
	void ConvertShiftJIS_BtoQ(wchar_t *str,int len);

private:

	int code_page;

	void ConvertFile(const UnicodeString &);
	void ConvertFile(const UnicodeString &,const UnicodeString &);
	void ConvertSubFolder(const UnicodeString &);
	void ConvertFolder(const UnicodeString &);

	void ConvertName(const UnicodeString &,const AnsiString &);
	void FilterName(const UnicodeString &);

public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
