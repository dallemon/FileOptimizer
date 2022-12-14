//---------------------------------------------------------------------------
#pragma once
#ifndef cppAboutH
#define cppAboutH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.StdCtrls.hpp>
#include "FileOptimizerPCH1.h"
#include "clsUtil.h"
#include "clsLanguage.h"
#include "cppMain.h"
#include <Vcl.ComCtrls.hpp>

	
//---------------------------------------------------------------------------
class TfrmAbout : public TForm
{
__published:	// IDE-managed Components
	TImage *imgAbout;
	TButton *butClose;
	TLabel *lblProduct;
	TLabel *lblCopyright;
	TLabel *lblDetail;
	TRichEdit *mmoLicense;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall butCloseClick(TObject *Sender);
	void __fastcall lblCopyrightClick(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall imgAboutDblClick(TObject *Sender);
private:	// User declarations
	int __fastcall GetFileVersionField(const TCHAR *fn, const TCHAR *info, TCHAR *ret, int len);
public:		// User declarations
	__fastcall TfrmAbout(TComponent* Owner);
	__fastcall ~TfrmAbout();
    void __fastcall EasterTimer(TObject *Sender);
	void __fastcall EasterClick(TObject *Sender);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAbout *frmAbout;
//---------------------------------------------------------------------------
#endif
