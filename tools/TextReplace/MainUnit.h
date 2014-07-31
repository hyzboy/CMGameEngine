//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

namespace hgl
{
	class StringList;
}
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TOpenTextFileDialog *OpenTextFileDialog;
	TPanel *Panel1;
	TButton *OpenOrignScriptButton;
	TButton *ParseTextButton;
	TButton *ReplaceTextButton;
	TLabel *Label1;
	TEdit *OrignTextPathEdit;
	TButton *OrignTextPathBrowserButton;
	TLabel *Label2;
	TEdit *NewTextPathEdit;
	TButton *NewTextPathBrowserButton;
	TLabel *Label3;
	TEdit *NewScriptPathEdit;
	TButton *NewScriptPathBrowserButton;
	TPanel *Panel2;
	TListBox *ScriptListBox;
	TCheckBox *OperCheckBox;
	TCheckBox *UseOneCheckBox;
	TCheckBox *ClipAscCheckBox;
	TLabel *Label4;
	TLabel *CharCountLabel;
	TCheckBox *ClearMultiItemCheckBox;
	TButton *Button1;
	TLabel *Label5;
	TLabel *LineCountLabel;
	TButton *Button2;
	TButton *Button3;
	void __fastcall OpenOrignScriptButtonClick(TObject *Sender);
	void __fastcall ParseTextButtonClick(TObject *Sender);
	void __fastcall OrignTextPathBrowserButtonClick(TObject *Sender);
	void __fastcall NewTextPathBrowserButtonClick(TObject *Sender);
	void __fastcall NewScriptPathBrowserButtonClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall ReplaceTextButtonClick(TObject *Sender);
	void __fastcall UseOneCheckBoxClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
private:	// User declarations

	UnicodeString config_file;

	void ReplacePathAndExt(const UnicodeString &,const UnicodeString &,const UnicodeString &,UnicodeString &);

	void ToOrignTextName(const UnicodeString &,UnicodeString &);
	void ToNewTextName(const UnicodeString &,UnicodeString &);
	void ToNewScriptName(const UnicodeString &,UnicodeString &);

	hgl::StringList *GetOrignText(const UnicodeString &);
	hgl::StringList *GetNewText(const UnicodeString &);

public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
