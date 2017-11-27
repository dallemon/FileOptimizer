// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
 1.00. 22/11/2017. FileOptimizer. Initial version
 */
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "clsLanguage.h"


THashedStringList *mlstLanguage = NULL;
THashedStringList *mlst1033 = NULL;


// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void __fastcall clsLanguage::LoadLanguages(void)
{
	//If not yet loaded	
	if (!mlstLanguage)
	{
		mlstLanguage = new THashedStringList();
		mlstLanguage->CaseSensitive = true;
		mlstLanguage->Duplicates = System::Classes::dupAccept;

		TCHAR acPath[PATH_MAX] = _T("");

		// Check if we already have it cached
		LANGID iLanguage = GetSystemDefaultUILanguage();

		//Never load en-US because is built-int
		if (iLanguage != 1033)
		{
			//Try to load locale
			_itot(iLanguage, acPath, 10);
			_tcscat(acPath, _T(".po"));
			if (clsUtil::ExistsFile(acPath))
			{
				mlstLanguage->LoadFromFile(acPath, TEncoding::UTF8);
			}
			else
			{
				//Try to load primary language
				_itot(PRIMARYLANGID(iLanguage), acPath, 10);
				_tcscat(acPath, _T(".po"));
				if (clsUtil::ExistsFile(acPath))
				{
					mlstLanguage->LoadFromFile(acPath, TEncoding::UTF8);
				}
			}
		}
	}

	//Need to load language?
	if ((!mlst1033) && (StrStrI(GetCommandLine(), _T("/SAVELANGUAGE")) != NULL))
	{
		mlst1033 = new THashedStringList();
		mlst1033->CaseSensitive = true;
		mlst1033->Duplicates = System::Classes::dupAccept;
		if (clsUtil::ExistsFile(_T("1033.po")))
		{
			mlst1033->LoadFromFile("1033.po", TEncoding::UTF8);
		}
	}
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void __fastcall clsLanguage::SaveLanguage(void)
{
    //Need to save language?
	if (mlst1033)
	{
		try
		{
			mlst1033->SaveToFile("1033.po", TEncoding::UTF8);
		}
		catch (EFCreateError &excE)
		{
		}	
	}
}


// ---------------------------------------------------------------------------
TCHAR * __fastcall clsLanguage::Get(TCHAR *pacText)
{
	String sRes = Get((String) pacText);

	return(sRes.c_str());
}



// ---------------------------------------------------------------------------
String __fastcall clsLanguage::Search(String psText, THashedStringList *plstLanguage)
{
	//Search for text to be translated
	String sSearch = "msgid \"" + psText + "\"";

	//int iLine = 0;
	int iLine = plstLanguage->IndexOf(sSearch);
	if (iLine > 0)
	{
		String sLine;
		//Skip lines not starting with mgstr
		do
		{
	        iLine++;
			sLine = plstLanguage->Strings[iLine];
		}
		while ((PosEx("msgstr \"", sLine) <= 0) && (iLine < plstLanguage->Count));
		psText = sLine;
		psText = ReplaceStr(psText, "\n", "");
		psText = psText.SubString(9, psText.Length() - 9);	//Remove first msgstr \" and last quote
		psText = ReplaceStr(psText, "\\\\", "\\");			//Unescape PO
		psText = ReplaceStr(psText, "\\n", "\n");
		psText = ReplaceStr(psText, "\\r", "\r");
		psText = ReplaceStr(psText, "\\t", "\t");
		psText = ReplaceStr(psText, "\\\"", "\"");
	}
	else
	{
		psText = "NOT_FOUND";
	}
	return(psText);
}



// ---------------------------------------------------------------------------
String __fastcall clsLanguage::Get(String psText)
{
	if (psText.Length() > 0)
	{
		LoadLanguages();

		String sTranslated;
		//If need to update 1033.po
		if (mlst1033)
		{
			//Not found in 1033.po
			sTranslated = Search(psText, mlst1033);
			if (sTranslated == "NOT_FOUND")
			{
				//Check if already exists
				if (mlst1033->Count <= 0)
				{
					//Write header
					mlst1033->Add("# Language ID: 1033 (0x0409)");
					mlst1033->Add("# Language Name: English - United States");
					mlst1033->Add("\"Project-Id-Version: " + Application->Name + " " + (String) clsUtil::ExeVersion(Application->ExeName.c_str()) + "\"");
					mlst1033->Add("\"POT-Creation-Date: " + (String) __DATE__ + "\"");
					mlst1033->Add("\"Language: en_US\"");
					mlst1033->Add("\"Last-Translator: Javier Guti�rrez Chamorro\"");
					mlst1033->Add("\"Language-Team: Javier Guti�rrez Chamorro\"");
					mlst1033->Add("\"Plural-Forms: nplurals=2; plural=(n != 1);\"\n");
				}
				//Write text
				sTranslated = psText;
				sTranslated = ReplaceStr(sTranslated, "\\", "\\\\");			//Escape PO
				sTranslated = ReplaceStr(sTranslated, "\n", "\\n");
				sTranslated = ReplaceStr(sTranslated, "\r", "\\r");
				sTranslated = ReplaceStr(sTranslated, "\t", "\\t");
				sTranslated = ReplaceStr(sTranslated, "\"", "\\\"");
				sTranslated = "msgid \"" + sTranslated + "\"";
				mlst1033->Add(sTranslated);
				mlst1033->Add("msgstr \"\"\n");
			}
		}
		sTranslated = Search(psText, mlstLanguage);
		if (sTranslated != "NOT_FOUND")
		{
			psText = sTranslated;
		}
	}
	return(psText);
}



// ---------------------------------------------------------------------------
void __fastcall clsLanguage::TranslateForm(TForm *pfrmForm)
{
	EnumerateControls(pfrmForm);
}



// ---------------------------------------------------------------------------
void clsLanguage::EnumerateControls(TComponent *poControl)
{
	//Missing main menu and TLabel
	//String s = poControl->Name;

	//TForm
	{
		TForm *oControl = dynamic_cast<TForm *>(poControl);
		if (oControl)
		{
			oControl->Caption = Get(oControl->Caption);
			oControl->Hint = Get(oControl->Hint);
		}
	}
	//TLabel
	{
		TLabel *oControl = dynamic_cast<TLabel *>(poControl);
		if (oControl)
		{
			oControl->Caption = Get(oControl->Caption);
			oControl->Hint = Get(oControl->Hint);
		}
	}
	//TEdit
	{
		TEdit *oControl = dynamic_cast<TEdit *>(poControl);
		if (oControl)
		{
			oControl->Text = Get(oControl->Text);
			oControl->Hint = Get(oControl->Hint);
		}
	}
	//TMemo
	{
		TMemo *oControl = dynamic_cast<TMemo *>(poControl);
		if (oControl)
		{
			oControl->Text = Get(oControl->Text);
			oControl->Hint = Get(oControl->Hint);
		}
	}
	//TSpinEdit
	{
		TSpinEdit *oControl = dynamic_cast<TSpinEdit *>(poControl);
		if (oControl)
		{
			oControl->Hint = Get(oControl->Hint);
		}
	}
	//TCheckBox
	{
		TCheckBox *oControl = dynamic_cast<TCheckBox *>(poControl);
		if (oControl)
		{
			oControl->Caption = Get(oControl->Caption);
			oControl->Hint = Get(oControl->Hint);
		}
	}
	//TComboBox
	{
		TComboBox *oControl = dynamic_cast<TComboBox *>(poControl);
		if (oControl)
		{
			oControl->Hint = Get(oControl->Hint);
			for (int iItem = oControl->Items->Count - 1; iItem >= 0; iItem--)
			{
				oControl->Items->Strings[iItem] = Get(oControl->Items->Strings[iItem]);
			}

		}
	}
	//TImage
	{
		TImage *oControl = dynamic_cast<TImage *>(poControl);
		if (oControl)
		{
			oControl->Hint = Get(oControl->Hint);
		}
	}
	//TMenuItem
	{
		TMenuItem *oControl = dynamic_cast<TMenuItem *>(poControl);
		if (oControl)
		{
			oControl->Caption = Get(oControl->Caption);
			oControl->Hint = Get(oControl->Hint);
		}
	}
	//TButton
	{
		TButton *oControl = dynamic_cast<TButton *>(poControl);
		if (oControl)
		{
			oControl->Caption = Get(oControl->Caption);
			oControl->Hint = Get(oControl->Hint);
		}
	}
	//TMenuItem
	{
		TMenuItem *oControl = dynamic_cast<TMenuItem *>(poControl);
		if (oControl)
		{
			oControl->Caption = Get(oControl->Caption);
			oControl->Hint = Get(oControl->Hint);
		}
	}
	//TToolBar
	{
		TToolBar *oControl = dynamic_cast<TToolBar *>(poControl);
		if (oControl)
		{
			for (int iButton = oControl->ButtonCount - 1; iButton >= 0; iButton--)
			{
				TToolButton *oButton = dynamic_cast<TToolButton *>(oControl->Buttons[iButton]);
				if (oButton)
				{
					TAction *oAction = dynamic_cast<TAction *>(oButton->Action);
					if (oAction)
					{
						oAction->Caption = Get(oAction->Caption);
						oAction->Hint = Get(oAction->Hint);
					}
				}
			}
		}
	}
	//TAction
	{
		TAction *oControl = dynamic_cast<TAction *>(poControl);
		if (oControl)
		{
			oControl->Caption = Get(oControl->Caption);
			oControl->Hint = Get(oControl->Hint);
		}
	}
	//TTabSheet
	{
		TTabSheet *oControl = dynamic_cast<TTabSheet *>(poControl);
		if (oControl)
		{
			oControl->Caption = Get(oControl->Caption);
			oControl->Hint = Get(oControl->Hint);
		}
	}


	//Childs
	for (int iControl = poControl->ComponentCount - 1; iControl >= 0; iControl--)
	{
		TComponent *oControl = dynamic_cast<TComponent *>(poControl->Components[iControl]);
		if (oControl)
		{
			String q = oControl->Name;
			EnumerateControls(oControl);
		}
	}
}
