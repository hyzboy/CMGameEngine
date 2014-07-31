//---------------------------------------------------------------------------

#ifndef SelectFolderUnitH
#define SelectFolderUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include <XPMan.hpp>
//---------------------------------------------------------------------------
class TSelectFolderForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TButton *Button1;
	TButton *Button2;
    TButton *Button3;
	TDirectoryListBox *DirectoryListBox;
	TDriveComboBox *DriveComboBox1;
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TSelectFolderForm(TComponent* Owner);

    WideString FolderName;
};
//---------------------------------------------------------------------------
#endif
