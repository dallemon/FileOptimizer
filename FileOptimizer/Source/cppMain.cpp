// ---------------------------------------------------------------------------
#include "cppMain.h"


// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"



// ---------------------------------------------------------------------------
TfrmMain *frmMain = nullptr;
struct udtOptions gudtOptions  = {};
//SYSTEM_INFO gudtSystemInfo = {};
bool gbProcess = false;
bool gbStop = false;
unsigned int miStartTicks;



// ---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner) : TForm(Owner)
{
}



// ---------------------------------------------------------------------------
__fastcall TfrmMain::~TfrmMain()
{
}



// ---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
	TCHAR acPath[PATH_MAX + 1];


	Caption = Application->Name;
	Icon = Application->Icon;
	lblCopyright->Hint = KS_APP_URL;

	clsUtil::GetFileVersionField(Application->ExeName.c_str(), (const TCHAR *) _T("LegalCopyright"), acPath, (sizeof(acPath) / sizeof(TCHAR)) - 1);
	lblCopyright->Caption = acPath;

	pgbProgress->Parent = stbMain;

	clsUtil::LoadForm(this);
	LoadOptions();

	UpdateTheme();
	FormResize(Sender);

	SetPriorityClass(GetCurrentProcess(), (unsigned long) gudtOptions.iProcessPriority);

	actClearExecute(Sender);
	//Load grid contents
	unsigned int iRows = clsUtil::GetIni(_T("grdFiles"), _T("Rows"), 0);
	if (iRows > 0)
	{
		grdFiles->RowCount = (unsigned int) iRows + 1;
		for (unsigned int iRow = 1; iRow <= iRows; iRow++)
		{
			String sRow = clsUtil::GetIni(_T("grdFiles"), ((String) "Row" + iRow).c_str(), _T(""));
			sRow = ReplaceStr(sRow, "%3D", "=");
			sRow = ReplaceStr(sRow, "%0A", "\r");
			sRow = ReplaceStr(sRow, "%0D", "\n");
			grdFiles->Rows[iRow]->DelimitedText = sRow;
		}
		RefreshStatus();
	}
	//GetSystemInfo(&gudtSystemInfo);
}



// ---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
	if (!gudtOptions.bHideAds)
	{
		webAds->Stop();
	}
	//Fix TWebBrowser access violation on close
	if (webAds)
	{
		delete webAds;
		webAds = nullptr;
	}

	clsUtil::SaveForm(this);
	clsLanguage::Save();
	SaveOptions();

	//Save grid contents
	unsigned int iRows = (unsigned int) grdFiles->RowCount - 1;
	for (unsigned int iRow = 1; iRow <= iRows; iRow++)
	{
		String sRow = grdFiles->Rows[iRow]->DelimitedText;
		sRow = ReplaceStr(sRow, "=", "%3D");
		sRow = ReplaceStr(sRow, "\r", "%0A");
		sRow = ReplaceStr(sRow, "\n", "%0D");
		clsUtil::SetIni(_T("grdFiles"), ((String) "Row" + iRow).c_str(), sRow.c_str(), _T(""));
	}
	//Purge unused entries
	unsigned int iOldRows = clsUtil::GetIni(_T("grdFiles"), _T("Rows"), 0);
	for (unsigned int iRow = iRows + 1; iRow <= iOldRows; iRow++)
	{
		clsUtil::DeleteIni(_T("grdFiles"), ((String) "Row" + iRow).c_str());
	}
	clsUtil::SetIni(_T("grdFiles"), _T("Rows"), (int) iRows, _T(""));
}


// ---------------------------------------------------------------------------
void __fastcall TfrmMain::LoadOptions(void)
{
	TCHAR acPath[PATH_MAX + 1];


	grdFiles->ColWidths[KI_GRID_FILE] = GetOption(Name.c_str(), _T("Col0Width"), grdFiles->ColWidths[KI_GRID_FILE]);
	grdFiles->ColWidths[KI_GRID_EXTENSION] = GetOption(Name.c_str(), _T("Col1Width"), grdFiles->ColWidths[KI_GRID_EXTENSION]);
	grdFiles->ColWidths[KI_GRID_ORIGINAL] = GetOption(Name.c_str(), _T("Col2Width"), grdFiles->ColWidths[KI_GRID_ORIGINAL]);
	grdFiles->ColWidths[KI_GRID_OPTIMIZED] = GetOption(Name.c_str(), _T("Col3Width"), grdFiles->ColWidths[KI_GRID_OPTIMIZED]);
	grdFiles->ColWidths[KI_GRID_STATUS] = GetOption(Name.c_str(), _T("Col4Width"), grdFiles->ColWidths[KI_GRID_STATUS]);
	gudtOptions.bBMPCopyMetadata = GetOption(_T("Options"), _T("BMPCopyMetadata"), false);
	gudtOptions.bCSSEnableTidy = GetOption(_T("Options"), _T("CSSEnableTidy"), false);
	_tcsncpy(gudtOptions.acCSSTemplate, GetOption(_T("Options"), _T("CSSTemplate"), _T("low")), (sizeof(gudtOptions.acCSSTemplate) / sizeof(TCHAR)) - 1);
	gudtOptions.bEXEDisablePETrim = GetOption(_T("Options"), _T("EXEDisablePETrim"), false);
	gudtOptions.bEXEEnableUPX = GetOption(_T("Options"), _T("EXEEnableUPX"), false);
	gudtOptions.bGIFCopyMetadata = GetOption(_T("Options"), _T("GIFCopyMetadata"), false);
	gudtOptions.bGIFAllowLossy = GetOption(_T("Options"), _T("GIFAllowLossy"), false);
	gudtOptions.bGZCopyMetadata = GetOption(_T("Options"), _T("GZCopyMetadata"), false);
	gudtOptions.bHTMLEnableTidy = GetOption(_T("Options"), _T("HTMLEnableTidy"), false);
	gudtOptions.bJPEGCopyMetadata = GetOption(_T("Options"), _T("JPEGCopyMetadata"), false);
	gudtOptions.bJPEGUseArithmeticEncoding = GetOption(_T("Options"), _T("JPEGUseArithmeticEncoding"), false);
	gudtOptions.bJPEGAllowLossy = GetOption(_T("Options"), _T("JPEGAllowLossy"), false);
	gudtOptions.bJSEnableJSMin = GetOption(_T("Options"), _T("JSEnableJSMin"), false);
	_tcsncpy(gudtOptions.acJSAdditionalExtensions, GetOption(_T("Options"), _T("JSAdditionalExtensions"), _T("")), (sizeof(gudtOptions.acJSAdditionalExtensions) / sizeof(TCHAR)) - 1);
	gudtOptions.bLUAEnableLeanify = GetOption(_T("Options"), _T("LUAEnableLeanify"), false);
	gudtOptions.bMiscDisable = GetOption(_T("Options"), _T("MiscDisable"), false);
	gudtOptions.bMiscCopyMetadata = GetOption(_T("Options"), _T("MiscCopyMetadata"), false);
	gudtOptions.bMP3CopyMetadata = GetOption(_T("Options"), _T("MP3CopyMetadata"), false);
	gudtOptions.bMP4CopyMetadata = GetOption(_T("Options"), _T("MP4CopyMetadata"), false);
	gudtOptions.bPCXCopyMetadata = GetOption(_T("Options"), _T("PCXCopyMetadata"), false);
	_tcsncpy(gudtOptions.acPDFProfile, GetOption(_T("Options"), _T("PDFProfile"), _T("none")), (sizeof(gudtOptions.acPDFProfile) / sizeof(TCHAR)) - 1);
	gudtOptions.iPDFCustomDPI = GetOption(_T("Options"), _T("PDFCustomDPI"), 150);
	gudtOptions.bPDFSkipLayered = GetOption(_T("Options"), _T("PDFSkipLayered"), false);
	gudtOptions.bPNGCopyMetadata = GetOption(_T("Options"), _T("PNGCopyMetadata"), false);
	gudtOptions.bPNGAllowLossy = GetOption(_T("Options"), _T("PNGAllowLossy"), false);
	gudtOptions.bTGACopyMetadata = GetOption(_T("Options"), _T("TGACopyMetadata"), false);
	gudtOptions.bTIFFCopyMetadata = GetOption(_T("Options"), _T("TIFFCopyMetadata"), false);
	gudtOptions.bWAVCopyMetadata = GetOption(_T("Options"), _T("WAVCopyMetadata"), false);
	gudtOptions.bWAVStripSilence = GetOption(_T("Options"), _T("WAVStripSilence"), false);
	gudtOptions.bWEBPAllowLossy = GetOption(_T("Options"), _T("WEBPAllowLossy"), false);
	gudtOptions.bXMLEnableLeanify = GetOption(_T("Options"), _T("XMLEnableLeanify"), false);
	gudtOptions.bZIPCopyMetadata = GetOption(_T("Options"), _T("ZIPCopyMetadata"), false);
	gudtOptions.bZIPRecurse = GetOption(_T("Options"), _T("ZIPRecurse"), false);
	gudtOptions.bKeepAttributes = GetOption(_T("Options"), _T("KeepAttributes"), false);
	gudtOptions.bDoNotUseRecycleBin = GetOption(_T("Options"), _T("DoNotUseRecycleBin"), false);
	gudtOptions.bDoNotCreateBackups = GetOption(_T("Options"), _T("DoNotCreateBackups"), true);
	_tcsncpy(gudtOptions.acIncludeMask, GetOption(_T("Options"), _T("IncludeMask"), _T("")), (sizeof(gudtOptions.acIncludeMask) / sizeof(TCHAR)) - 1);
	_tcsncpy(gudtOptions.acExcludeMask, GetOption(_T("Options"), _T("ExcludeMask"), _T("")), (sizeof(gudtOptions.acExcludeMask) / sizeof(TCHAR)) - 1);
	_tcsncpy(gudtOptions.acDonator, GetOption(_T("Options"), _T("Donator"), _T("")), (sizeof(gudtOptions.acDonator) / sizeof(TCHAR)) - 1);
	StrTrim(gudtOptions.acDonator, _T(" %\a\b\f\n\r\t\v\\\'\"\?"));
	_tcsncpy(gudtOptions.acDonation, GetOption(_T("Options"), _T("Donation"), _T("")), (sizeof(gudtOptions.acDonation) / sizeof(TCHAR)) - 1);
	//Invalid donator, reset fields
	if (_tcslen(gudtOptions.acDonator) < 3)
	{
        gudtOptions.acDonator[0] = NULL;
        gudtOptions.acDonation[0] = NULL;
	}
	_tcsncpy(gudtOptions.acDisablePluginMask, GetOption(_T("Options"), _T("DisablePluginMask"), _T("")), (sizeof(gudtOptions.acDisablePluginMask) / sizeof(TCHAR)) - 1);
	gudtOptions.bBeepWhenDone = GetOption(_T("Options"), _T("BeepWhenDone"), false);
	gudtOptions.bShutdownWhenDone = GetOption(_T("Options"), _T("ShutdownWhenDone"), false);
	gudtOptions.bDisableSleep = GetOption(_T("Options"), _T("DisableSleep"), true);
	gudtOptions.bAlwaysOnTop = GetOption(_T("Options"), _T("AlwaysOnTop"), false);
	gudtOptions.bDebug = GetOption(_T("Options"), _T("Debug"), false);
	gudtOptions.bShowToolBar = GetOption(_T("Options"), _T("ShowToolBar"), false);

    //Show ads require Windows Vista or later because of the need for IE11 or Edge
	if (clsUtil::GetWindowsVersion() >= 600)
	{
		//Set IE compatibility to 11.01
		clsUtil::SetRegistry(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION"), ExtractFileName(Application->ExeName).c_str(), 11001U);
		
		//Set Edge profile folder to system temporary path
		if (GetTempPath((sizeof(acPath) / sizeof(TCHAR)) - 1, acPath))
		{
			SetEnvironmentVariable(_T("WEBVIEW2_USER_DATA_FOLDER"), acPath);
		}

		//Check if ad display was not set
		_tcsncpy(acPath, GetOption(_T("Options"), _T("HideAds"), _T("")), (sizeof(gudtOptions.acTempDirectory) / sizeof(TCHAR)) - 1);
		if ((acPath[0] == NULL) && (gudtOptions.acDonator[0] == NULL))
		{
			String sCaption;
			sCaption.printf(_((TCHAR *) _T("This is the first time you run %s.\n\nDo you want to support its development by showing ads while it is in use?\n\nThis will encourage its future maintenance and upgrades, being highly appreciated.\n\nYou can change this option at any time from the Options menu.")), Application->Name.c_str());
			gudtOptions.bHideAds = !(clsUtil::MsgBox(Handle, sCaption.c_str(), _((TCHAR *) _T("Support")), MB_YESNO | MB_ICONQUESTION) == ID_YES);
		}
		else
		{
			//ToDo: Potential crash when hideads=false on closing
			gudtOptions.bHideAds = GetOption(_T("Options"), _T("HideAds"), false);
		}
	}
	//Disable ads in XP
	else
	{
		gudtOptions.bHideAds = true;
	}
	gudtOptions.bAllowDuplicates = GetOption(_T("Options"), _T("AllowDuplicates"), false);
	gudtOptions.bAllowMultipleInstances = GetOption(_T("Options"), _T("AllowMultipleInstances"), false);
	gudtOptions.bClearWhenComplete = GetOption(_T("Options"), _T("ClearWhenComplete"), false);
	gudtOptions.bEnableCache = GetOption(_T("Options"), _T("EnableCache"), false);
	gudtOptions.bHideInstructions = GetOption(_T("Options"), _T("HideInstructions"), false);
	gudtOptions.iLevel = GetOption(_T("Options"), _T("Level"), 5);
	gudtOptions.iLanguage = GetOption(_T("Options"), _T("Language"), 0);
	gudtOptions.iProcessPriority = GetOption(_T("Options"), _T("ProcessPriority"), NORMAL_PRIORITY_CLASS);
	gudtOptions.iCheckForUpdates = GetOption(_T("Options"), _T("CheckForUpdates"), 1);
	gudtOptions.iLogLevel = GetOption(_T("Options"), _T("LogLevel"), 0);
	gudtOptions.iFilenameFormat = GetOption(_T("Options"), _T("FilenameFormat"), 0);
	gudtOptions.iStartupDelay = GetOption(_T("Options"), _T("StartupDelay"), 1000);
	gudtOptions.iLeanifyIterations = GetOption(_T("Options"), _T("LeanifyIterations"), -1);
	gudtOptions.iPNGWolfIterations = GetOption(_T("Options"), _T("PNGWolfIterations"), -1);
	_tcsncpy(gudtOptions.acTempDirectory, GetOption(_T("Options"), _T("TempDirectory"), _T("")), (sizeof(gudtOptions.acTempDirectory) / sizeof(TCHAR)) - 1);

	if (GetModuleFileName(nullptr, acPath, (sizeof(acPath) / sizeof(TCHAR)) - 1) != 0)
	{
		_tcsncpy(acPath, clsUtil::ExeVersion(acPath), (sizeof(acPath) / sizeof(TCHAR)) - 1);
		_tcsncpy(gudtOptions.acVersion, GetOption(_T("Options"), _T("Version"), acPath), (sizeof(gudtOptions.acVersion) / sizeof(TCHAR)) - 1);
	}

	//Statistics
	miStartTicks = GetTickCount();
	gudtOptions.lStatTime = (unsigned long long) GetOption(_T("Statistics"), _T("Time"), 0);
	gudtOptions.iStatOpens = (unsigned int) GetOption(_T("Statistics"), _T("Opens"), 0);
	gudtOptions.iStatOpens++;
	gudtOptions.iStatFiles = (unsigned int) GetOption(_T("Statistics"), _T("Files"), 0);
	gudtOptions.lStatTotalBytes = (unsigned long long) GetOption(_T("Statistics"), _T("TotalBytes"), 0LL);
	gudtOptions.lStatSavedBytes = (unsigned long long) GetOption(_T("Statistics"), _T("SavedBytes"), 0LL);
	gudtOptions.iStatSession = (unsigned int) GetOption(_T("Statistics"), _T("Session"), clsUtil::Random(0, INT_MAX));
	gudtOptions.iAdsShown = (unsigned int) GetOption(_T("Statistics"), _T("AdsShown"), 0);
}



// ---------------------------------------------------------------------------
void __fastcall TfrmMain::SaveOptions(void)
{
	clsUtil::SetIni(Name.c_str(), _T("Col0Width"), grdFiles->ColWidths[KI_GRID_FILE]);
	clsUtil::SetIni(Name.c_str(), _T("Col1Width"), grdFiles->ColWidths[KI_GRID_EXTENSION]);
	clsUtil::SetIni(Name.c_str(), _T("Col2Width"), grdFiles->ColWidths[KI_GRID_ORIGINAL]);
	clsUtil::SetIni(Name.c_str(), _T("Col3Width"), grdFiles->ColWidths[KI_GRID_OPTIMIZED]);
	clsUtil::SetIni(Name.c_str(), _T("Col4Width"), grdFiles->ColWidths[KI_GRID_STATUS]);
	clsUtil::SetIni(_T("Options"), _T("BMPCopyMetadata"), gudtOptions.bBMPCopyMetadata, _T("Boolean. Default: false. Copy file metadata. Else strip all unneeded information."));
	clsUtil::SetIni(_T("Options"), _T("CSSEnableTidy"), gudtOptions.bCSSEnableTidy, _T("Boolean. Default: false. Enable tidy. Results in smaller files, but can happen they are not editable anymore."));
	clsUtil::SetIni(_T("Options"), _T("CSSTemplate"), gudtOptions.acCSSTemplate, _T("String. Default 'low'. Compression template, from safer and worse compression, to highest compression."));
	clsUtil::SetIni(_T("Options"), _T("EXEDisablePETrim"), gudtOptions.bEXEDisablePETrim, _T("Boolean. Default: false. Disable PETrim. When enabled, PETrim will not be used, resulting in less EXE corruption at the cost of larger file size."));
	clsUtil::SetIni(_T("Options"), _T("EXEEnableUPX"), gudtOptions.bEXEEnableUPX, _T("Boolean. Default: false. Enable UPX executable compression. When enabled, UPX will be used, resulting EXE and DLL size reduction at the cost of runtime decompression."));
	clsUtil::SetIni(_T("Options"), _T("GIFCopyMetadata"), gudtOptions.bGIFCopyMetadata, _T("Boolean. Default: false. Copy file metadata. Else strip all unneeded information."));
	clsUtil::SetIni(_T("Options"), _T("GIFAllowLossy"), gudtOptions.bGIFAllowLossy, _T("Boolean. Default: false. Allowing lossy optimizations will get higher files reduction at the cost of some quality loss, even if visually unnoticeable or not."));
	clsUtil::SetIni(_T("Options"), _T("GZCopyMetadata"), gudtOptions.bGZCopyMetadata, _T("Boolean. Default: false. Copy file metadata. Else strip all unneeded information."));
	clsUtil::SetIni(_T("Options"), _T("HTMLEnableTidy"), gudtOptions.bHTMLEnableTidy, _T("Boolean. Default: false. Enable Tidy. Results in smaller files, but can happen they are not editable anymore. Note that this applies to both SVG and HTML file types."));
	clsUtil::SetIni(_T("Options"), _T("JPEGCopyMetadata"), gudtOptions.bJPEGCopyMetadata, _T("Boolean. Default: false. Copy file metadata. Else strip all unneeded information"));
	clsUtil::SetIni(_T("Options"), _T("JPEGUseArithmeticEncoding"), gudtOptions.bJPEGUseArithmeticEncoding, _T("Boolean. Default: false. Arithmetic encoding gives additional saving reductions, but is incompatible with most programs."));
	clsUtil::SetIni(_T("Options"), _T("JPEGAllowLossy"), gudtOptions.bJPEGAllowLossy, _T("Boolean. Default: false. Allowing lossy optimizations will get higher files reduction at the cost of some quality loss, even if visually unnoticeable or not."));
	clsUtil::SetIni(_T("Options"), _T("JSEnableJSMin"), gudtOptions.bJSEnableJSMin, _T("Boolean. Default: false. Enable jsmin. Results in smaller files, but can happen they are not editable anymore."));
	clsUtil::SetIni(_T("Options"), _T("JSAdditionalExtensions"), gudtOptions.acJSAdditionalExtensions, _T("String. Default: ''. Add extra extensions to be threated as JS/JSON."));
	clsUtil::SetIni(_T("Options"), _T("LUAEnableLeanify"), gudtOptions.bLUAEnableLeanify, _T("Boolean. Default: false. Enable Leanify. Results in smaller files, but can happen they are not editable anymore."));
	clsUtil::SetIni(_T("Options"), _T("MiscDisable"), gudtOptions.bMiscDisable, _T("Boolean. Default: false. Disable processing of other file types. This could imply lossing some edit capabilities such as PSD/PSB where text layers will be rasterized."));
	clsUtil::SetIni(_T("Options"), _T("MiscCopyMetadata"), gudtOptions.bMiscCopyMetadata, _T("Boolean. Default: false. Copy file metadata. Else strip all unneeded information."));
	clsUtil::SetIni(_T("Options"), _T("MP3CopyMetadata"), gudtOptions.bMP3CopyMetadata, _T("Boolean. Default: false. Copy file metadata. Else strip all unneeded information."));
	clsUtil::SetIni(_T("Options"), _T("MP4CopyMetadata"), gudtOptions.bMP4CopyMetadata, _T("Boolean. Default: false. Copy file metadata. Else strip all unneeded information."));
	clsUtil::SetIni(_T("Options"), _T("PCXCopyMetadata"), gudtOptions.bPCXCopyMetadata, _T("Boolean. Default: false. Copy file metadata. Else strip all unneeded information."));
	clsUtil::SetIni(_T("Options"), _T("PDFProfile"), gudtOptions.acPDFProfile, _T("String. Default 'none'. Compression profile, from less size, to best quality."));
	clsUtil::SetIni(_T("Options"), _T("PDFCustomDPI"), gudtOptions.iPDFCustomDPI, _T("Number. Default: 150. When custom profile is choosen, it allows you to specify a custom DPI for downsampling images."));
	clsUtil::SetIni(_T("Options"), _T("PDFSkipLayered"), gudtOptions.bPDFSkipLayered, _T("Boolean. Default: false. Skip processing of PDF files containing layered objects. Results in more compatible files with higher size."));
	clsUtil::SetIni(_T("Options"), _T("PNGCopyMetadata"), gudtOptions.bPNGCopyMetadata, _T("Boolean. Default: false. Copy file metadata. Else strip all unneeded information."));
	clsUtil::SetIni(_T("Options"), _T("PNGAllowLossy"), gudtOptions.bPNGAllowLossy, _T("Boolean. Default: false. Allowing lossy optimizations will get higher files reduction at the cost of some quality loss, even if visually unnoticeable or not."));
	clsUtil::SetIni(_T("Options"), _T("TGACopyMetadata"), gudtOptions.bTGACopyMetadata, _T("Boolean. Default: false. Copy file metadata. Else strip all unneeded information."));
	clsUtil::SetIni(_T("Options"), _T("TIFFCopyMetadata"), gudtOptions.bTIFFCopyMetadata, _T("Boolean. Default: false. Copy file metadata. Else strip all unneeded information."));
	clsUtil::SetIni(_T("Options"), _T("WAVCopyMetadata"), gudtOptions.bWAVCopyMetadata, _T("Boolean. Default: false. Copy file metadata. Else strip all unneeded information."));
	clsUtil::SetIni(_T("Options"), _T("WAVStripSilence"), gudtOptions.bWAVStripSilence, _T("Boolean. Default: false. Strip start and end silences if any."));
	clsUtil::SetIni(_T("Options"), _T("WEBPAllowLossy"), gudtOptions.bWEBPAllowLossy, _T("Boolean. Default: false. Allowing lossy optimizations will get higher files reduction at the cost of some quality loss, even if visually unnoticeable or not."));
	clsUtil::SetIni(_T("Options"), _T("XMLEnableLeanify"), gudtOptions.bXMLEnableLeanify, _T("Boolean. Default: false. Enable Leanify. Results in smaller files, but can happen they are not editable anymore."));
	clsUtil::SetIni(_T("Options"), _T("ZIPCopyMetadata"), gudtOptions.bZIPCopyMetadata, _T("Boolean. Default: false. Copy file metadata. Else strip all unneeded information."));
	clsUtil::SetIni(_T("Options"), _T("ZIPRecurse"), gudtOptions.bZIPRecurse, _T("Boolean. Default: false. Enable optimization inside archives (recursive optimization)."));
	clsUtil::SetIni(_T("Options"), _T("KeepAttributes"), gudtOptions.bKeepAttributes, _T("Boolean. Default: false. Keep original readonly, system, hidden and archive attributes as well as creation and modification timestamps."));
	clsUtil::SetIni(_T("Options"), _T("DoNotUseRecycleBin"), gudtOptions.bDoNotUseRecycleBin, _T("Boolean. Default: false. When checked original files will not be backed up in the system trashcan."));
	clsUtil::SetIni(_T("Options"), _T("DoNotCreateBackups"), gudtOptions.bDoNotCreateBackups, _T("Boolean. Default: true. When checked original files will not be backed up in the current folder as a .BAK files."));
	clsUtil::SetIni(_T("Options"), _T("IncludeMask"), gudtOptions.acIncludeMask, _T("String. Default: ''. If not empty, only files containing this mask (substring) on name or path will be included from optimization. You can use semicolon to specify more than one substring being included."));
	clsUtil::SetIni(_T("Options"), _T("ExcludeMask"), gudtOptions.acExcludeMask, _T("String. Default: ''. Files containing this mask (substring) on name or path will be excluded from optimization. You can use semicolon to specify more than one substring being excluded."));
	clsUtil::SetIni(_T("Options"), _T("Donator"), gudtOptions.acDonator, _T("String. Default: ''. Donator name if you have supported the project."));
	clsUtil::SetIni(_T("Options"), _T("Donation"), gudtOptions.acDonation, _T("String. Default: ''. Donation details."));
	clsUtil::SetIni(_T("Options"), _T("DisablePluginMask"), gudtOptions.acDisablePluginMask, _T("String. Default: ''. Allow excluding execution of certain plugins. It is case insensitive, and allows more than one item to be specified by using semicolon as separator."));
	clsUtil::SetIni(_T("Options"), _T("BeepWhenDone"), gudtOptions.bBeepWhenDone, _T("Boolean. Default: false. Beep the speaker when optimization completes."));
	clsUtil::SetIni(_T("Options"), _T("ShutdownWhenDone"), gudtOptions.bShutdownWhenDone, _T("Boolean. Default: false. Shutdown computer when optimization completes."));
	clsUtil::SetIni(_T("Options"), _T("DisableSleep"), gudtOptions.bDisableSleep, _T("Boolean. Default: true. Avoid system sleeping while optimization in progress."));
	clsUtil::SetIni(_T("Options"), _T("AlwaysOnTop"), gudtOptions.bAlwaysOnTop, _T("Boolean. Default: false. Show main window always on top."));
	clsUtil::SetIni(_T("Options"), _T("ShowToolBar"), gudtOptions.bShowToolBar, _T("Boolean. Default: false. Show icons toolbar on main window."));
	clsUtil::SetIni(_T("Options"), _T("Debug"), gudtOptions.bDebug, _T("Boolean. Default: false. Enable internal debugging mode. Temporary files will not be deleted."));
	clsUtil::SetIni(_T("Options"), _T("HideAds"), gudtOptions.bHideAds, _T("Boolean. Default: false. Hide ads from being displayed."));
	clsUtil::SetIni(_T("Options"), _T("AllowDuplicates"), gudtOptions.bAllowDuplicates, _T("Boolean. Default: false. Allow adding same file more than once. If enabled, adding to the grid will be much faster, specially on very large grids."));
	clsUtil::SetIni(_T("Options"), _T("AllowMultipleInstances"), gudtOptions.bAllowMultipleInstances, _T("Boolean. Default: false. Allow having more than one FileOptimizer instance. If not, a warning will appear."));
	clsUtil::SetIni(_T("Options"), _T("ClearWhenComplete"), gudtOptions.bClearWhenComplete, _T("Boolean. Default: false. Automatically clear file list when optimization is completed."));
	clsUtil::SetIni(_T("Options"), _T("EnableCache"), gudtOptions.bEnableCache, _T("Boolean. Default: false. Enable cache of already optimized files to automatically skip them."));
	clsUtil::SetIni(_T("Options"), _T("HideInstructions"), gudtOptions.bHideInstructions, _T("Boolean. Default: false. Hide instructions panel label."));
	clsUtil::SetIni(_T("Options"), _T("Level"), gudtOptions.iLevel, _T("Number. Default: 5. Optimization level from best speed to best compression."));
	clsUtil::SetIni(_T("Options"), _T("Language"), gudtOptions.iLanguage, _T("Number. Default: 0 (System default). Interface language/locale code in decimal."));
	clsUtil::SetIni(_T("Options"), _T("ProcessPriority"), gudtOptions.iProcessPriority, _T("Number. Default: 2 (Normal). Process priority from most conservative to best performance."));
	clsUtil::SetIni(_T("Options"), _T("CheckForUpdates"), gudtOptions.iCheckForUpdates, _T("Number. Default: 1. Automatically check for program updates."));
	clsUtil::SetIni(_T("Options"), _T("LogLevel"), gudtOptions.iLogLevel, _T("Number. Default: 0. Debugging level to output on program log."));
	clsUtil::SetIni(_T("Options"), _T("FilenameFormat"), gudtOptions.iFilenameFormat, _T("Number. Default: 0. Specify the format to display filenames in the list."));
	clsUtil::SetIni(_T("Options"), _T("StartupDelay"), gudtOptions.iStartupDelay, _T("Number. Default: 1000. Integer from 1000ms to 29000ms specifying the delay before start adding files."));
	clsUtil::SetIni(_T("Options"), _T("LeanifyIterations"), gudtOptions.iLeanifyIterations, _T("Number. Default: -1. If specified, number of trial iterations in all Leanify executions will use this value. If not, iterations are calculated depending on the Optimization level."));
	clsUtil::SetIni(_T("Options"), _T("PNGWolfIterations"), gudtOptions.iPNGWolfIterations, _T("Number. Default: -1. If specified, number of trial iterations in all PNGWolf executions will use this value. If not, iterations are calculated depending on the Optimization level."));
	clsUtil::SetIni(_T("Options"), _T("TempDirectory"), gudtOptions.acTempDirectory, _T("String. Default: ''. If not empty specified directory will be used for temporary storage instead of system's %TEMP%."));
	clsUtil::SetIni(_T("Options"), _T("Version"), gudtOptions.acVersion, _T("String. Default: ''."));
	
	//Statistics
	gudtOptions.lStatTime += ((GetTickCount() - miStartTicks) / 1000);
	clsUtil::SetIni(_T("Statistics"), _T("Time"), (long long) gudtOptions.lStatTime);
	clsUtil::SetIni(_T("Statistics"), _T("Opens"), (int) gudtOptions.iStatOpens);
	clsUtil::SetIni(_T("Statistics"), _T("Files"), (int) gudtOptions.iStatFiles);
	clsUtil::SetIni(_T("Statistics"), _T("TotalBytes"), (long long) gudtOptions.lStatTotalBytes);
	clsUtil::SetIni(_T("Statistics"), _T("SavedBytes"), (long long) gudtOptions.lStatSavedBytes);
	clsUtil::SetIni(_T("Statistics"), _T("Session"), (int) gudtOptions.iStatSession);
	clsUtil::SetIni(_T("Statistics"), _T("AdsShown"), (int) gudtOptions.iAdsShown);
}


//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	WIN32_FIND_DATA udtFindFileData;
	

	//Check if there are plugins still running
	if ((!gbStop) && (!gudtOptions.bAllowMultipleInstances))
	{
		TCHAR acPluginsDirectory[PATH_MAX + 1];
		
		if (GetModuleFileName(NULL, acPluginsDirectory, (sizeof(acPluginsDirectory) / sizeof(TCHAR)) - 1) != 0)
		{
			*_tcsrchr(acPluginsDirectory, '\\') = NULL;
		}
		#if defined(_WIN64)
			_tcscat(acPluginsDirectory, _T("\\Plugins64\\"));
		#else
			_tcscat(acPluginsDirectory, _T("\\Plugins32\\"));
        #endif

		bool bRunning = false;
		HANDLE hFindFile = FindFirstFile((((String) acPluginsDirectory) + "*.exe").c_str(), &udtFindFileData);
		if (hFindFile != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (clsUtil::FindProcess(udtFindFileData.cFileName))
				{
					bRunning = true;
					break;
				}
			}
			while ((bool) FindNextFile(hFindFile, &udtFindFileData) != false);
			FindClose(hFindFile);
		}

		String sCaption;
		sCaption.printf(_((TCHAR *) _T("Optimization is still running. Do you want to stop and exit %s?")), Application->Name.c_str());
		if ((bRunning) && (clsUtil::MsgBox(Handle, sCaption.c_str(), _((TCHAR *) _T("Exit")), MB_YESNO | MB_ICONQUESTION) == ID_NO))
		{
			CanClose = false;
			return;
		}
	}
	gbStop = true;
	CanClose = true;
	Hide();
	Application->ProcessMessages();	
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::FormResize(TObject *Sender)
{
	static unsigned int iOldWidth = 0;


	//Recalculate column widths only if window width changed
	if (iOldWidth != (unsigned int) Width)
	{
		iOldWidth = (unsigned int) Width;

		//Prevent flickering
		LockWindowUpdate(Handle);

		grdFiles->ColWidths[KI_GRID_FILE] = grdFiles->Width >> 1;
		grdFiles->ColWidths[KI_GRID_EXTENSION] = (grdFiles->Width - grdFiles->ColWidths[KI_GRID_FILE]) >> 2;
		grdFiles->ColWidths[KI_GRID_ORIGINAL] = (grdFiles->Width - grdFiles->ColWidths[KI_GRID_FILE]) >> 2;
		grdFiles->ColWidths[KI_GRID_OPTIMIZED] = (grdFiles->Width - grdFiles->ColWidths[KI_GRID_FILE]) >> 2;
		grdFiles->ColWidths[KI_GRID_STATUS] = (grdFiles->Width - grdFiles->ColWidths[KI_GRID_FILE]) >> 2;

		//Reenable form updates
		LockWindowUpdate(NULL);
	}
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::grdFilesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
	grdFiles->Rows[ARow]->BeginUpdate();

	//Color even and odd rows
	if (ARow == 0)
	{
		grdFiles->Canvas->Brush->Color =  clMenu;
	}
	else if (ARow & 1)
	{
		grdFiles->Canvas->Brush->Color = clGradientInactiveCaption;
	}
	else
	{
		grdFiles->Canvas->Brush->Color = clGradientActiveCaption;
	}

	//Color file column and the others
	if ((ARow > 0) && (ACol == KI_GRID_FILE))
	{
		grdFiles->Canvas->Font->Color = clHotLight;
		grdFiles->Canvas->Font->Style = grdFiles->Canvas->Font->Style << fsUnderline;
	}
	else
	{
		grdFiles->Canvas->Font->Color = clWindowText;
	}

	Rect.left -= 2;
	Rect.top -= 2;
	grdFiles->Canvas->FillRect(Rect);
	String sValue = GetCellValue(grdFiles->Cells[ACol][ARow], 0);

	//Left aligned
	if ((ACol == KI_GRID_FILE) || (ACol == KI_GRID_EXTENSION) || (ACol == KI_GRID_STATUS))
	{
		grdFiles->Canvas->TextRect(Rect, Rect.left + 4, Rect.top + 4, sValue);
	}
	//Right aligned
	else
	{
		grdFiles->Canvas->TextRect(Rect, Rect.right - Canvas->TextWidth(sValue) - 4, Rect.top + 4, sValue);
	}
	grdFiles->Rows[ARow]->EndUpdate();
}


//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::grdFilesFixedCellClick(TObject *Sender, int ACol, int ARow)
{
	unsigned int iRows = (unsigned int) grdFiles->RowCount;

	if (iRows > 1)
	{
		static int iSortField = -1;
		static unsigned int iSortOrder = 0;

		Screen->Cursor = crAppStart;
		Application->ProcessMessages();
		SendMessage(grdFiles->Handle, WM_SETREDRAW, 0, 0);

		//Clear selection
		#if defined (__clang__)
			grdFiles->Selection = TGridRect({-1, 0, 4, -1});
		#else
			TGridRect rctSelection = {-1, 0, 4, -1};
			grdFiles->Selection = rctSelection;
		#endif

		if (ACol == iSortField)
		{
			iSortOrder = (iSortOrder + 1) & 1;
		}
		else
		{
			iSortOrder = 0;
		}
		iSortField = ACol;

		//Do the sort using a temporary TStringList
		TStringList *lstTemp = new TStringList();
		lstTemp->CaseSensitive = true;
		lstTemp->Duplicates = System::Classes::dupAccept;
		String sValue;
		for (unsigned int iRow = 1; iRow < iRows; iRow++)
		{
			if ((iSortField == KI_GRID_ORIGINAL) || (iSortField == KI_GRID_OPTIMIZED))
			{
				sValue = FormatFloat("0000000000000000000", ParseNumberThousand(grdFiles->Cells[iSortField][(int) iRow]));
			}
			else
			{
				sValue = grdFiles->Cells[iSortField][(int) iRow];
			}
			sValue += "|" + grdFiles->Cells[KI_GRID_FILE][(int) iRow] + "|" + grdFiles->Cells[KI_GRID_EXTENSION][(int) iRow] + "|" + grdFiles->Cells[KI_GRID_ORIGINAL][(int) iRow] + "|" + grdFiles->Cells[KI_GRID_OPTIMIZED][(int) iRow] + "|" + grdFiles->Cells[KI_GRID_STATUS][(int) iRow];
			lstTemp->Add(sValue);
		}
		lstTemp->Sort();

		//Copy the TSTringList to the grid
		TStringDynArray asValue;
		for (unsigned int iRow = 1; iRow < iRows; iRow++)
		{
			//ToDo: Try to avoid TStringDynArray
			TStringList *lstRow = new TStringList();
			if (iSortOrder == 0)
			{
				asValue = SplitString(lstTemp->Strings[(int) iRow - 1], "|");
			}
			else
			{
				asValue = SplitString(lstTemp->Strings[(int) (iRows - iRow - 1)], "|");
			}

			lstRow->Add(asValue[1]); //0: File
			lstRow->Add(asValue[2]); //1: Extension (Type)
			lstRow->Add(asValue[3]); //2: Original size
			lstRow->Add(asValue[4]); //3: Optimized size
			lstRow->Add(asValue[5]); //4: Status
			grdFiles->Rows[(int) iRow] = lstRow;
			delete lstRow;
		}
		delete lstTemp;

		RefreshStatus();
		SendMessage(grdFiles->Handle, WM_SETREDRAW, 1, 0);
		grdFiles->Repaint();

		Screen->Cursor = crDefault;
	}
}


//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::grdFilesMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	int iRow, iCol;
	
	
	//Draw the hint with the rollover cell contents
	grdFiles->MouseToCell(X, Y, iCol, iRow);
	if ((iRow >= 0) && (iCol >=0))
	{
		Application->CancelHint();
		grdFiles->Hint = GetCellValue(grdFiles->Cells[iCol][iRow], 1);
	}
}


//---------------------------------------------------------------------------
void __fastcall TfrmMain::grdFilesDblClick(TObject *Sender)
{
	//Double click synonums Open selected file
	actOpenExecute(Sender);
}



//---------------------------------------------------------------------------
void __fastcall TfrmMain::stbMainDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect)
{
	//Resize progress according to statusbar size
	if (Panel == StatusBar->Panels->Items[1])
	{
		pgbProgress->Top = Rect.top;
		pgbProgress->Left = Rect.left;
		pgbProgress->Width = Rect.right - Rect.left;
		pgbProgress->Height = Rect.bottom - Rect.top;
	}
}



//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	//Ctrl-F5 reload grid
	if ((Key == VK_F5) && (Shift.Contains(ssCtrl)))
	{
		unsigned int iRows = (unsigned int) grdFiles->RowCount;
		if (iRows > 1)
		{
			Screen->Cursor = crAppStart;
			Application->ProcessMessages();
			SendMessage(grdFiles->Handle, WM_SETREDRAW, 0, 0);
			for (int iRow = (int) iRows; iRow > 0; iRow--)
			{
				//Remove already optimized files
				if (PosEx(_("Done"), grdFiles->Cells[KI_GRID_STATUS][(int) iRow]) > 0)
				{
					for (int iSelectedRow = iRow; iSelectedRow < (int) iRows - 1; iSelectedRow++)
					{
						grdFiles->Rows[iSelectedRow] = grdFiles->Rows[iSelectedRow + 1];
					}
					iRows--;
				}
			}
			grdFiles->RowCount = (int) iRows;
			//RefreshStatus();
			//SendMessage(grdFiles->Handle, WM_SETREDRAW, 1, 0);
			//grdFiles->Repaint();
			//Screen->Cursor = crDefault;
		}
	}

	//F5 refresh grid. Will be triggered after CTR-F5 too
	if (Key == VK_F5)
	{
		unsigned int iRows = (unsigned int) grdFiles->RowCount;
		if (iRows > 1)
		{
			Screen->Cursor = crAppStart;
			Application->ProcessMessages();
			SendMessage(grdFiles->Handle, WM_SETREDRAW, 0, 0);
			for (unsigned int iRow = 1; iRow < iRows; iRow++)
			{
				//grdFiles->Cells[KI_GRID_ORIGINAL][(int) iRow]->BeginUpdate();
				//grdFiles->Cells[KI_GRID_FILE][(int) iRow] = asValue[1];
				//grdFiles->Cells[KI_GRID_EXTENSION][(int) iRow] = asValue[2];
				grdFiles->Cells[KI_GRID_ORIGINAL][(int) iRow] = FormatNumberThousand(clsUtil::SizeFile(GetCellValue(grdFiles->Cells[KI_GRID_FILE][iRow], 1).c_str()));
				//grdFiles->Cells[KI_GRID_OPTIMIZED][(int) iRow] = asValue[4];
				//grdFiles->Cells[KI_GRID_STATUS][(int) iRow] = asValue[5];
				//grdFiles->Rows[(int) iRow]->EndUpdate();
			}
			RefreshStatus();
			SendMessage(grdFiles->Handle, WM_SETREDRAW, 1, 0);
			grdFiles->Repaint();
			Screen->Cursor = crDefault;
		}
	}
    //Make DEL Shortcut
	else if ((Key == VK_DELETE) && (!Shift.Contains(ssCtrl)) && (!Shift.Contains(ssAlt)) && (!Shift.Contains(ssShift)))
	{
		actRemoveExecute(Sender);
	}
	//CTRL-C
	else if ((Shift.Contains(ssCtrl)) && (Key == 'C'))
	{
		//ToDo
		TClipboard *oClipBoard = Clipboard();
		//Save grid contents
		oClipBoard->AsText = grdFiles->Cols[0]->DelimitedText;
	}
	//CTRL-V
	else if ((Shift.Contains(ssCtrl)) && (Key == 'V'))
	{
		//ToDo
		TClipboard *oClipBoard = Clipboard();
		if (oClipBoard->HasFormat(CF_TEXT))
		{
			actClearExecute(Sender);
			//Load grid contents
			grdFiles->Cols[0]->DelimitedText = oClipBoard->AsText;
			RefreshStatus();
		}
	}
	//CTRL-X
	else if ((Shift.Contains(ssCtrl)) && (Key == 'X'))
	{
		TClipboard *oClipBoard = Clipboard();
		//Save grid contents
		oClipBoard->AsText = grdFiles->Cols[0]->DelimitedText;
		actClearExecute(Sender);
	}
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::actAddExecute(TObject *Sender)
{
	//Add files
	if (dlgAddFiles->Execute())
	{
		Screen->Cursor = crAppStart;
		grdFiles->Enabled = false;  //Prevent grid modifications while adding files
		Application->ProcessMessages();

		TStrings *strFiles = dlgAddFiles->Files;
		AddFilesInitializeExist();
		for (int iCount = strFiles->Count; iCount > 0; iCount--)
		{
			AddFiles(strFiles->Strings[iCount - 1].c_str());
		}
		grdFiles->Enabled = true;
		RefreshStatus();
		Screen->Cursor = crDefault;
	}
}


//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::actAddFolderExecute(TObject *Sender)
{
	String sDirectory = "";

	if (SelectDirectory(_("Add folder"), _T(""), sDirectory, TSelectDirExtOpts() << sdNewFolder << sdShowEdit << sdShowShares << sdNewUI << sdValidateDir, this))
	{
		Screen->Cursor = crAppStart;
		grdFiles->Enabled = false;  //Prevent grid modifications while adding files
		Application->ProcessMessages();

		AddFilesInitializeExist();
		AddFiles(sDirectory.c_str());

		RefreshStatus();
		grdFiles->Enabled = true;
		Screen->Cursor = crDefault;
	}
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::actRemoveExecute(TObject *Sender)
{
	int iSelectedRow1 = grdFiles->Selection.Top;
	int iSelectedRow2 = grdFiles->Selection.Bottom;


	SendMessage(grdFiles->Handle, WM_SETREDRAW, 0, 0);
	int iRows = grdFiles->RowCount - 1;
	for (int iRow = iSelectedRow1; iRow < iRows; iRow++)
	{
		grdFiles->Rows[iRow] = grdFiles->Rows[iRow + (iSelectedRow2 - iSelectedRow1 + 1)];
	}
	grdFiles->RowCount -= (iSelectedRow2 - iSelectedRow1 + 1);
	RefreshStatus();
	SendMessage(grdFiles->Handle, WM_SETREDRAW, 1, 0);
	grdFiles->Repaint();
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::actClearExecute(TObject *Sender)
{
	SendMessage(grdFiles->Handle, WM_SETREDRAW, 0, 0);

	Caption = Application->Name;
	Application->Title = Caption;		
	stbMain->Panels->Items[0]->Text = "";
	stbMain->Hint = stbMain->Panels->Items[0]->Text;
		
	grdFiles->RowCount = 1;
	RefreshStatus();
	SendMessage(grdFiles->Handle, WM_SETREDRAW, 1, 0);
	grdFiles->Repaint();
}




//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::actHelpExecute(TObject *Sender)
{
	ShellExecute(Handle, _T("open"), Application->HelpFile.c_str(), _T(""), _T(""), SW_SHOWNORMAL);
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::actOpenExecute(TObject *Sender)
{
	unsigned int iRow = (unsigned int) grdFiles->Row;
	unsigned int iCol = (unsigned int) grdFiles->Col;


	if ((iRow > 0) && (iCol == KI_GRID_FILE))
	{
		ShellExecute(Handle, _T("open"), GetCellValue(grdFiles->Cells[KI_GRID_FILE][(int) iRow], 1).c_str(), _T(""), _T(""), SW_SHOWNORMAL);
	}
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::actOpenFolderExecute(TObject *Sender)
{
	unsigned int iRow = (unsigned int) grdFiles->Row;
	unsigned int iCol = (unsigned int) grdFiles->Col;


	if ((iRow > 0) && (iCol == KI_GRID_FILE))
	{
		ShellExecute(Handle, _T("open"), ExtractFilePath(GetCellValue(grdFiles->Cells[KI_GRID_FILE][(int) iRow], 1)).c_str(), _T(""), _T(""), SW_SHOWNORMAL);
	}
}



//---------------------------------------------------------------------------
static struct udtOptimizeProgress
{
	unsigned long long lSavedBytes;
	unsigned long long lTotalBytes;
	unsigned int iCurrentFile;
	unsigned int iProcessedFiles;
	unsigned int iTotalFiles;
	String sFileStatusText;
	String sWindowCaptionText;
	String sStatusbarText;
} mudtOptimizeProgress;
static CRITICAL_SECTION mudtCriticalSection;



//---------------------------------------------------------------------------
static unsigned long long lSavedBytes, lTotalBytes;
static unsigned int iStartTicks, iEndTicks;
static String sPluginsDirectory;



//---------------------------------------------------------------------------
void __fastcall TfrmMain::actOptimizeExecute(TObject *Sender)
{
	unsigned int iCount;
	TCHAR acTmpFile[PATH_MAX + 1];


	gbProcess = true;
	gbStop = false;
	//grdFiles->Enabled = false;
	
	UpdateAds();
	RefreshStatus();

	if (GetModuleFileName(NULL, acTmpFile, (sizeof(acTmpFile) / sizeof(TCHAR)) - 1) != 0)
	{
		*_tcsrchr(acTmpFile, '\\') = NULL;
	}
	#if defined(_WIN64)
		_tcscat(acTmpFile, _T("\\Plugins64\\"));
	#else
		_tcscat(acTmpFile, _T("\\Plugins32\\"));
    #endif
	sPluginsDirectory = clsUtil::GetShortName((String) acTmpFile);
	SetCurrentDirectory(sPluginsDirectory.c_str());

	lSavedBytes = 0;
	lTotalBytes = 0;
	iStartTicks = GetTickCount();
	
	unsigned int iRows = (unsigned int) grdFiles->RowCount;


	InitializeCriticalSection(&mudtCriticalSection);
	
	if (gudtOptions.bDisableSleep)
	{
		SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED);
	}

	//Use multithreaded parallel for (PPL)
	if ((false) && (iRows > 2))
	//if (iRows > 2)
	{
		TParallel::For(this, 1, (int) (iRows - 1), actOptimizeForThread);
	}
	//Use regular for loop
	else
	{
		for (iCount = 1; iCount < iRows; iCount++)
		{
			if (!gbStop)
			{
				actOptimizeFor(NULL, (int) iCount);
			}
			else
			{
				break;
			}
		}
	}

	gbProcess = false;
	//grdFiles->Enabled = true;
	unsigned int iPercentBytes;
	if (lTotalBytes != 0)
	{
		iPercentBytes = ((unsigned int) ((double) (lTotalBytes - lSavedBytes) / lTotalBytes * 100));
	}
	else
	{
		iPercentBytes = 0;
	}
	iEndTicks = GetTickCount();


	//Required indirection
	String sCaption;
	TCHAR acTime[64];
	StrFromTimeInterval(acTime, (sizeof(acTime) / sizeof(TCHAR)) - 1, (unsigned int) (iEndTicks - iStartTicks), sizeof(acTime) - 1);
	sCaption.printf(_((TCHAR *) _T("%s files processed. %s bytes saved (%s%%). Elapsed time %s")), FormatNumberThousand(iCount - 1).c_str(), FormatNumberThousand(lSavedBytes).c_str(), FormatNumberThousand(iPercentBytes).c_str(), acTime);

	if (Visible)
	{
		stbMain->Panels->Items[0]->Text = sCaption;
		stbMain->Hint = stbMain->Panels->Items[0]->Text;
		Caption = stbMain->Hint + " - " + Application->Name;
		Application->Title = Caption;	
		RefreshStatus(false);
	}

	if (gudtOptions.bBeepWhenDone)
	{	
		clsUtil::MsgBox(Handle, sCaption.c_str(), _((TCHAR *) _T("Done")), MB_ICONINFORMATION | MB_OK);
		FlashWindow(Handle, false);
		MessageBeep(0xFFFFFFFF);
	}

	if (gudtOptions.bDisableSleep)
	{
		SetThreadExecutionState(ES_CONTINUOUS);
	}

	if (gudtOptions.bShutdownWhenDone)
	{
		if (!clsUtil::ShutdownWindows(0))
		{
			clsUtil::MsgBox(Handle, _((TCHAR *) _T("Error trying to automatically shutdown the system.")), _((TCHAR *) _T("Shutdown")), MB_OK | MB_ICONERROR);
		}
	}
	
	if (gudtOptions.bClearWhenComplete)
	{
		actClearExecute(Sender);
	}
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::actStopExecute(TObject *Sender)
{
	gbStop = true;
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::actOptionsExecute(TObject *Sender)
{
	frmOptions = new TfrmOptions(Application);

	//Set child window as on top, of current window already is
	if (FormStyle == fsStayOnTop)
	{
		FormStyle = fsNormal;
		frmOptions->FormStyle = fsStayOnTop;
	}
	frmOptions->PopupParent = this;
	frmOptions->ShowModal();
	if (gudtOptions.bAlwaysOnTop)
	{
		FormStyle = fsStayOnTop;
	}
	delete frmOptions;
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::actAboutExecute(TObject *Sender)
{
	frmAbout = new TfrmAbout(Application);

	//Set child window as on top, of current window already is
	if (FormStyle == fsStayOnTop)
	{
		FormStyle = fsNormal;
		frmAbout->FormStyle = fsStayOnTop;
	}
	frmAbout->PopupParent = this;
	frmAbout->ShowModal();
	if (gudtOptions.bAlwaysOnTop)
	{
		FormStyle = fsStayOnTop;
	}
	delete frmAbout;
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::actExitExecute(TObject *Sender)
{
	gbStop = true;
	Hide();
	Application->ProcessMessages();
	Close();
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::actInformationExecute(TObject *Sender)
{
	String sExtension;
	String sText = "";
	TCHAR acTime[64];


	//Get all supported extensions
	TStringDynArray asExtension;
	
	asExtension = SplitString((KS_EXTENSION_ALL + (ReplaceStr((String) gudtOptions.acJSAdditionalExtensions, ";", " ")).UpperCase() + " "), " ");
	unsigned int iExtensionLen = (unsigned int) asExtension.Length;

	//Sort them alphabetically
	TStringList *lstTemp = new TStringList();
	lstTemp->CaseSensitive = true;
	lstTemp->Duplicates = System::Classes::dupIgnore;
	for (unsigned int iExtension = 0; iExtension < iExtensionLen; iExtension++)
	{
		sExtension = asExtension[iExtension];
		//Dont push it if empty extension
		if (sExtension.Length() > 1)
		{
			lstTemp->Add(sExtension);
		}
	}
	lstTemp->Sort();

	iExtensionLen = (unsigned int) lstTemp->Count;
	for (unsigned int iExtension = 0; iExtension < iExtensionLen; iExtension++)
	{
		sExtension = lstTemp->Strings[(int) iExtension].UpperCase();
		//Check if it is not the last extension
		if (iExtension != (iExtensionLen - 1))
		{
			sText += sExtension + ", ";
		}
		else
		{
			sText.cat_printf(_((TCHAR *) _T("and %s file formats among many others.")), sExtension.c_str());
		}
	}
	delete lstTemp;
	
	sText = Application->Name + _(" is an advanced file optimizer featuring a lossless (no quality loss) file size reduction that supports: ") + sText;

	sText.cat_printf(_((TCHAR *) _T("\n\nDONATOR INFORMATION\n")));
	if (gudtOptions.acDonation[0] != NULL)
	{
		sText += (String) gudtOptions.acDonation;

	}
	else
	{
		sText.cat_printf(_((TCHAR *) _T("Have not donated yet!")));

	}
	sText.cat_printf(_((TCHAR *) _T("\nUser since %s %s")), Application->Name.c_str(), gudtOptions.acVersion);


	StrFromTimeInterval(acTime, (sizeof(acTime) / sizeof(TCHAR)) - 1, (unsigned int) (gudtOptions.lStatTime * 1000), sizeof(acTime) - 1);
	sText.cat_printf(_((TCHAR *) _T("\n\nUSAGE STATISTICS\n"
		"- Time: %s\n"
		"- Opens: %s\n"
		"- Files: %s\n"
		"- Total: %s\n"
		"- Saved: %s\n")),
		acTime, 
		FormatNumberThousand(gudtOptions.iStatOpens).c_str(),
		FormatNumberThousand(gudtOptions.iStatFiles).c_str(),
		FormatNumberThousandUnit(gudtOptions.lStatTotalBytes).c_str(),
		FormatNumberThousandUnit(gudtOptions.lStatSavedBytes).c_str());

	clsUtil::MsgBox(Handle, sText.c_str(), _((TCHAR *) _T("Information")), MB_ICONINFORMATION | MB_OK);
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::actDonateExecute(TObject *Sender)
{
	ShellExecute(Handle, _T("open"), KS_APP_DONATE_URL, _T(""), _T(""), SW_SHOWMAXIMIZED);
}


//---------------------------------------------------------------------------
void __fastcall TfrmMain::OptimizeProgressThread(struct udtOptimizeProgress pudtOptimizeProgress)
{
	EnterCriticalSection(&mudtCriticalSection);
	
	//memcpy(&mudtOptimizeProgress, &pudtOptimizeProgress, sizeof(mudtOptimizeProgress));
	mudtOptimizeProgress.iCurrentFile = pudtOptimizeProgress.iCurrentFile;
	mudtOptimizeProgress.iProcessedFiles = pudtOptimizeProgress.iProcessedFiles;
	mudtOptimizeProgress.iTotalFiles = pudtOptimizeProgress.iTotalFiles;
	mudtOptimizeProgress.lSavedBytes = pudtOptimizeProgress.lSavedBytes;
	mudtOptimizeProgress.lTotalBytes = pudtOptimizeProgress.lTotalBytes;
	mudtOptimizeProgress.sFileStatusText = pudtOptimizeProgress.sFileStatusText;
	mudtOptimizeProgress.sWindowCaptionText = pudtOptimizeProgress.sWindowCaptionText;
	mudtOptimizeProgress.sStatusbarText = pudtOptimizeProgress.sStatusbarText;


	//http://docwiki.embarcadero.com/RADStudio/Seattle/en/Using_the_Main_VCL_Thread
	//TThread::Synchronize((TThreadMethod) OptimizeProgressVCL);
	//TInterlocked::Increment((int &) miTot);
	//Use a timer to refresh?

	LeaveCriticalSection(&mudtCriticalSection);
}



//---------------------------------------------------------------------------
void __fastcall TfrmMain::OptimizeProgressVCL(void)
{
	//Status bar text
	if (mudtOptimizeProgress.sStatusbarText != "NULL")
	{
		stbMain->Panels->Items[0]->Text = mudtOptimizeProgress.sStatusbarText;
		stbMain->Hint = stbMain->Panels->Items[0]->Text;
		Caption = stbMain->Hint + " - " + Application->Name;
		Application->Title = Caption;		
	}
	
	//Progress
	if (mudtOptimizeProgress.iCurrentFile != UINT_MAX)
	{	
		pgbProgress->Position = (int) mudtOptimizeProgress.iCurrentFile;
		clsUtil::SetTaskListProgress((unsigned int) pgbProgress->Position, (unsigned int) pgbProgress->Max);
		grdFiles->Row = (int) mudtOptimizeProgress.iCurrentFile;
	}
	
	Application->ProcessMessages();
	
	//RunPlugin
	
	//RefreshStatus
	
}


//---------------------------------------------------------------------------
void __fastcall TfrmMain::actOptimizeForThread(TObject *Sender, int AIndex, TParallel::TLoopState *LoopState)
{
	actOptimizeFor(Sender, AIndex);
	if (gbStop)
	{
		LoopState->Break();
	}
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::actOptimizeFor(TObject *Sender, int AIndex)
{
	int iCount = AIndex;
	unsigned int iStartTicks, iEndTicks;
	FILETIME udtFileCreated, udtFileAccessed, udtFileModified;
	String sInputFile, sFlags;


	iStartTicks = GetTickCount();
	
	sInputFile = GetCellValue(grdFiles->Cells[KI_GRID_FILE][iCount], 1);

	struct udtOptimizeProgress a =
	{
			ULLONG_MAX, //unsigned long long lSavedBytes;
			ULLONG_MAX, //unsigned long long lTotalBytes;
			UINT_MAX, //unsigned int iCurrentFile;
			UINT_MAX, //unsigned int iProcessedFiles;
			UINT_MAX, //unsigned int iTotalFiles;
			"NULL", //String sFileStatusText;
			"NULL", //String sWindowCaptionText;
			"NULL", //String sStatusbarText;
	};
	//OptimizeProgressThread(a);

	//Required indirection
	String sCaption;
	sCaption.printf(_((TCHAR *) _T("Processing %s...")), sInputFile.c_str());
	stbMain->Panels->Items[0]->Text = sCaption;

	stbMain->Hint = stbMain->Panels->Items[0]->Text;
	//Caption = stbMain->Hint + " - " + Application->Name;
	//Application->Title = Caption;
	pgbProgress->Position = iCount;
	clsUtil::SetTaskListProgress((unsigned int) pgbProgress->Position, (unsigned int) pgbProgress->Max);
	grdFiles->Row = iCount;
	Application->ProcessMessages();


	bool bExcluded = false;

	//If include mask is specified, by default all files are excluded, except the specified
	if (gudtOptions.acIncludeMask[0] != NULL)
	{
		bExcluded = true;
		TCHAR *acToken = _tcstok(((String) gudtOptions.acIncludeMask).UpperCase().c_str(), _T(";"));
		while (acToken)
		{
			if (PosEx((String) acToken, sInputFile.UpperCase()) != 0)
			{
				bExcluded = false;
				break;
			}
			acToken = _tcstok(NULL, _T(";"));
		}
	}

	if (gudtOptions.acExcludeMask[0] != NULL)
	{
		TCHAR *acToken = _tcstok(((String) gudtOptions.acExcludeMask).UpperCase().c_str(), _T(";"));
		while (acToken)
		{
			if (PosEx((String) acToken, sInputFile.UpperCase()) != 0)
			{
				bExcluded = true;
				break;
			}
			acToken = _tcstok(NULL, _T(";"));
		}
	}
	

	//Skip it if already in optimization cache
	if ((gudtOptions.bEnableCache) && (grdFiles->Cells[KI_GRID_STATUS][(int) iCount] == _("Optimized")))
	{
		bExcluded = true;
	}

	//Check file still exists and is not to be excluded
	if ((clsUtil::ExistsFile(sInputFile.c_str())) && (!bExcluded))
	{
		unsigned int iFileAttributes = INVALID_FILE_ATTRIBUTES;
		if (gudtOptions.bKeepAttributes)
		{
			//If get timestamp fails, set to null
			if (!clsUtil::GetFileTimestamp(clsUtil::GetShortName(sInputFile).c_str(), &udtFileCreated, &udtFileAccessed, &udtFileModified))
			{
				udtFileCreated.dwLowDateTime = 0;
				udtFileCreated.dwHighDateTime = 0;
				/* udtFileAccessed.dwLowDateTime = 0;
				udtFileAccessed.dwHighDateTime = 0;
				udtFileModified.dwLowDateTime = 0;
				udtFileModified.dwHighDateTime = 0; */
			}
			iFileAttributes = GetFileAttributes(clsUtil::GetShortName(sInputFile).c_str());
		}
	
		if (!gudtOptions.bDoNotUseRecycleBin)
		{
			grdFiles->Cells[KI_GRID_STATUS][iCount] = _("Copying to Recyclebin...");
			clsUtil::CopyToRecycleBin(sInputFile.c_str());
		}

		if (!gudtOptions.bDoNotCreateBackups)
		{
			grdFiles->Cells[KI_GRID_STATUS][iCount] = _("Creating backup...");
			CopyFile(sInputFile.c_str(), (sInputFile + ".bak").c_str(), false);
		}

		SetFileAttributes(clsUtil::GetShortName(sInputFile).c_str(), FILE_ATTRIBUTE_NORMAL);

		int iLevel;
		//Each extension can correspond to more than one engine, so use if instead of else if
		// AVIF: ImageMagick
		String sExtensionByContent = " " + GetExtensionByContent(sInputFile) + " ";
		if (PosEx(sExtensionByContent, KS_EXTENSION_AVIF) > 0)
		{
			sFlags = "";
			RunPlugin((unsigned int) iCount, "ImageMagick (1/1)", (sPluginsDirectory + "magick.exe convert \"%INPUTFILE%\" -quiet -define heic:speed=1 " + sFlags + "\"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// BMP: ImageMagick, ImageWorsener
		if (PosEx(sExtensionByContent, KS_EXTENSION_BMP) > 0)
		{
			sFlags = "";
			if (!gudtOptions.bBMPCopyMetadata)
			{
				sFlags += "-strip ";
			}
			RunPlugin((unsigned int) iCount, "ImageMagick (1/2)", (sPluginsDirectory + "magick.exe convert \"%INPUTFILE%\" -quiet -compress RLE " + sFlags + "\"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			RunPlugin((unsigned int) iCount, "ImageWorsener (2/2)", (sPluginsDirectory + "imagew.exe -opt bmp:version=auto -noresize -zipcmprlevel 9 -outfmt bmp -compress \"rle\" \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// CSS: CSSTidy, Minify
		if (PosEx(sExtensionByContent, KS_EXTENSION_CSS) > 0)
		{
			if (gudtOptions.bCSSEnableTidy)
			{
				RunPlugin((unsigned int) iCount, "CSSTidy (1/2)", (sPluginsDirectory + "csstidy.exe \"%INPUTFILE%\" --template=" + gudtOptions.acCSSTemplate + " \"\"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				
				RunPlugin((unsigned int) iCount, "Minify (2/2)", (sPluginsDirectory + "minify.exe \"%INPUTFILE%\" --output \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
		}
		// DLL: PETrim, strip, UPX
		if (PosEx(sExtensionByContent, KS_EXTENSION_DLL) > 0)
		{
			if (!gudtOptions.bEXEDisablePETrim)
			{
				RunPlugin((unsigned int) iCount, "PETrim (1/3)", (sPluginsDirectory + "petrim.exe \"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
			RunPlugin((unsigned int) iCount, "strip (2/3)", (sPluginsDirectory + "strip.exe --strip-all -o \"%TMPOUTPUTFILE%\" \"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			if (gudtOptions.bEXEEnableUPX)
			{
				sFlags = "";
				if (gudtOptions.iLevel < 3)
				{
					sFlags += "-1 ";
				}
				else if (gudtOptions.iLevel < 5)
				{
					sFlags += "-9 ";
				}
				else if (gudtOptions.iLevel < 7)
				{
					sFlags += "-9 --best ";
				}
				else if (gudtOptions.iLevel < 9)
				{
					sFlags += "-9 --best --lzma ";
				}
				else
				{
					sFlags += "-9 --best --lzma --ultra-brute --crp-ms=999999 ";
				}	
				RunPlugin((unsigned int) iCount, "UPX (3/3)", (sPluginsDirectory + "upx.exe --no-backup --force " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
		}
		// EXE: Leanify, PETrim, strip, UPX
		if (PosEx(sExtensionByContent, KS_EXTENSION_EXE) > 0)
		{
			if (!IsEXESFX(sInputFile.c_str()))
			{
				sFlags = "";
				//iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
				//Overwrite Leanify iterations
				if (gudtOptions.iLeanifyIterations != -1)
				{
					iLevel = gudtOptions.iLeanifyIterations;
				}
				else
				{
					iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
				}
				sFlags += "-i " + (String) iLevel + " ";
				RunPlugin((unsigned int) iCount, "Leanify (1/4)", (sPluginsDirectory + "leanify.exe -q -p " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

				if (!gudtOptions.bEXEDisablePETrim)
				{
					RunPlugin((unsigned int) iCount, "PETrim (2/4)", (sPluginsDirectory + "petrim.exe /StripFixups:Y \"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				}
				RunPlugin((unsigned int) iCount, "strip (3/4)", (sPluginsDirectory + "strip.exe --strip-all -o \"%TMPOUTPUTFILE%\" \"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				if (gudtOptions.bEXEEnableUPX)
				{
					sFlags = "--no-backup --force ";
					if (gudtOptions.iLevel < 3)
					{
						sFlags += "-1 ";
					}
					else if (gudtOptions.iLevel < 5)
					{
						sFlags += "-9 ";
					}
					else if (gudtOptions.iLevel < 7)
					{
						sFlags += "-9 --best ";
					}
					else if (gudtOptions.iLevel < 9)
					{
						sFlags += "-9 --best --lzma ";
					}
					else
					{
						sFlags += "-9 --best --lzma --ultra-brute --crp-ms=999999 ";
					}	
					RunPlugin((unsigned int) iCount, "UPX (4/4)", (sPluginsDirectory + "upx.exe " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				}
			}
		}
		// FLAC: FLAC, FLACOut
		if (PosEx(sExtensionByContent, KS_EXTENSION_FLAC) > 0)
		{
			if (!gudtOptions.bWAVCopyMetadata)
			{
				String sTmpOutputFile = ReplaceStr(sInputFile, ".flac", "-stripped.flac");
				//Prevent a bug in shntool with no lowercase extensions
				if (sTmpOutputFile == sInputFile)
				{
					sTmpOutputFile += "-stripped.flac";
				}

				RunPlugin((unsigned int) iCount, "shntool (1/4)", (sPluginsDirectory + "shntool.exe strip -q -O always \"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				if ((clsUtil::SizeFile(sTmpOutputFile.c_str()) > 0) && (clsUtil::SizeFile(sTmpOutputFile.c_str()) < ParseNumberThousand(grdFiles->Cells[KI_GRID_OPTIMIZED][iCount])))
				{
					clsUtil::CopyFile(sTmpOutputFile.c_str(), sInputFile.c_str());
					grdFiles->Cells[KI_GRID_OPTIMIZED][(int) iCount] = FormatNumberThousand(clsUtil::SizeFile(sInputFile.c_str()));
				}
				if (!gudtOptions.bDebug)
				{
					clsUtil::DeleteFile(sTmpOutputFile.c_str());
				}
				if (gudtOptions.bWAVStripSilence)
				{
					sTmpOutputFile = ReplaceStr(sInputFile, ".flac", "-trimmed.flac");
					//Prevent a bug in shntool with no lowercase extensions
					if (sTmpOutputFile == sInputFile)
					{
						sTmpOutputFile += "-trimmed.flac";
					}

					RunPlugin((unsigned int) iCount, "shntool (2/4)", (sPluginsDirectory + "shntool.exe trim -q -O always \"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
					if ((clsUtil::SizeFile(sTmpOutputFile.c_str()) > 0) && (clsUtil::SizeFile(sTmpOutputFile.c_str()) < ParseNumberThousand(grdFiles->Cells[KI_GRID_OPTIMIZED][iCount])))
					{
						clsUtil::CopyFile(sTmpOutputFile.c_str(), sInputFile.c_str());
						grdFiles->Cells[KI_GRID_OPTIMIZED][(int) iCount] = FormatNumberThousand(clsUtil::SizeFile(sInputFile.c_str()));
					}
					if (!gudtOptions.bDebug)
					{			
						clsUtil::DeleteFile(sTmpOutputFile.c_str());
					}
				}
			}

			sFlags = "";
			if (gudtOptions.bMiscCopyMetadata)
			{
				sFlags += "--keep-foreign-metadata ";
			}
			if (gudtOptions.iLevel < 3)
			{
				sFlags += "-1 ";
			}
			else if (gudtOptions.iLevel < 5)
			{
				sFlags += "-8 --best ";
			}
			else if (gudtOptions.iLevel < 7)
			{
				sFlags += "-8 --best -e ";
			}
			else
			{
				sFlags += "-8 --best -ep ";
			}	
			RunPlugin((unsigned int) iCount, "FLAC (3/4)", (sPluginsDirectory + "flac.exe --force -s " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			if (gudtOptions.iLevel >= 9)
			{
				RunPlugin((unsigned int) iCount, "FLACOut (4/4)", (sPluginsDirectory + "flacout.exe /q /y \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
		}
		// GIF: ImageMagick, gifsicle
		if (PosEx(sExtensionByContent, KS_EXTENSION_GIF) > 0)
		{
			sFlags = "";
			if (!gudtOptions.bGIFCopyMetadata)
			{
				sFlags += "-strip ";
			}
			//RunPlugin((unsigned int) iCount, "ImageMagick", (sPluginsDirectory + "magick.exe convert \"%INPUTFILE%\" -quiet -layers optimize -compress LZW " + sFlags + "\"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			RunPlugin((unsigned int) iCount, "ImageMagick (1/2)", (sPluginsDirectory + "magick.exe convert \"%INPUTFILE%\" -quiet -set dispose background -layers optimize -compress LZW " + sFlags + "\"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			
			sFlags = "";
			//iLevel = min(gudtOptions.iLevel * 3 / 9, 3);
			iLevel = 3;
			sFlags += "-O" + (String) iLevel + " ";
			if (!gudtOptions.bGIFCopyMetadata)
			{
				sFlags += "--no-comments --no-extensions --no-names ";
			}
			if (gudtOptions.bGIFAllowLossy)
			{
				sFlags += "--lossy=85 ";
			}
			RunPlugin((unsigned int) iCount, "gifsicle (2/2)", (sPluginsDirectory + "gifsicle.exe -w -j --no-conserve-memory -o \"%TMPOUTPUTFILE%\" " + sFlags + "\"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// GZ: Libdeflate, Leanify, ect, advdef, zRecompress, deflopt, defluff, deflopt
		if (PosEx(sExtensionByContent, KS_EXTENSION_GZ) > 0)
		{
			if (!gudtOptions.bGZCopyMetadata)
			{
				RunPlugin((unsigned int) iCount, "libdeflate (1/8)", (sPluginsDirectory + "libdeflate.bat \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				
				sFlags = "";
				//iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
				//Overwrite Leanify iterations
				if (gudtOptions.iLeanifyIterations != -1)
				{
					iLevel = gudtOptions.iLeanifyIterations;
				}
				else
				{
					iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
				}
				sFlags += "-i " + (String) iLevel + " ";
				if (gudtOptions.bGZCopyMetadata)
				{
					sFlags += "--keep-exif ";
				}
				RunPlugin((unsigned int) iCount, "Leanify (2/8)", (sPluginsDirectory + "leanify.exe -q -p " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
			
			sFlags = "";
			//iLevel = min(gudtOptions.iLevel * 7 / 9, 7) + 1;
			iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
			sFlags += "-i " + (String) iLevel + " ";
			RunPlugin((unsigned int) iCount, "advdef (3/8)", (sPluginsDirectory + "advdef.exe -z -q -4 " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			RunPlugin((unsigned int) iCount, "zRecompress (4/8)", (sPluginsDirectory + "zRecompress.exe -tgz \"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			sFlags = "";
			if (!gudtOptions.bGZCopyMetadata)
			{
				sFlags += "-str?p ";
			}
			iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
			//Convert level 9 to level 90032 because it is faster and usually with higher savings
			if (iLevel == 9)
			{
				iLevel = 90032;
			}
			sFlags += "-" + (String) iLevel + " ";
			RunPlugin((unsigned int) iCount, "ECT (5/8)", (sPluginsDirectory + "ECT.exe -quiet --mt-deflate --mt-file --allfilters -gzip " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			sFlags = "";
			if (gudtOptions.bGZCopyMetadata)
			{
				sFlags += "/c ";
			}
			RunPlugin((unsigned int) iCount, "DeflOpt (6/8)", (sPluginsDirectory + "deflopt.exe /a /b /s " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			RunPlugin((unsigned int) iCount, "defluff (7/8)", (sPluginsDirectory + "defluff.bat \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			RunPlugin((unsigned int) iCount, "DeflOpt (8/8)", (sPluginsDirectory + "deflopt.exe /a /b /s " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// HTML: tidy-html5, Minify, Leanify
		if (PosEx(sExtensionByContent, KS_EXTENSION_HTML) > 0)
		{
			if (gudtOptions.bHTMLEnableTidy)
			{
				RunPlugin((unsigned int) iCount, "tidy (1/3)", (sPluginsDirectory + "tidy.exe -config tidy.config -quiet -output \"%TMPOUTPUTFILE%\" \"%INPUTFILE%\" ").c_str(), sInputFile, "", 0, 0);
				
				RunPlugin((unsigned int) iCount, "Minify (2/3)", (sPluginsDirectory + "minify.exe \"%INPUTFILE%\" --output \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

				sFlags = "";
				//iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
				//Overwrite Leanify iterations
				if (gudtOptions.iLeanifyIterations != -1)
				{
					iLevel = gudtOptions.iLeanifyIterations;
				}
				else
				{
					iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
				}
				sFlags += "-i " + (String) iLevel + " ";
				RunPlugin((unsigned int) iCount, "Leanify (3/3)", (sPluginsDirectory + "leanify.exe -q -p " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
		}
		// ICO: ImageMagick, Leanify
		if (PosEx(sExtensionByContent, KS_EXTENSION_ICO) > 0)
		{
			/*
			sFlags = "";
			if (!gudtOptions.bPNGCopyMetadata)
			{
				sFlags += "-strip ";
			}
			RunPlugin((unsigned int) iCount, "ImageMagick (1/2)", (sPluginsDirectory + "magick.exe convert \"%INPUTFILE%\" -quiet -compress ZIP " + sFlags + "\"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			*/

			//Temporary disable Leanify because it removed IPTC metadata
			if (!gudtOptions.bPNGCopyMetadata)
			{
				sFlags = "";
				//iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
				//Overwrite Leanify iterations
				if (gudtOptions.iLeanifyIterations != -1)
				{
					iLevel = gudtOptions.iLeanifyIterations;
				}
				else
				{
					iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
				}
				sFlags += "-i " + (String) iLevel + " ";
				RunPlugin((unsigned int) iCount, "Leanify (2/2)", (sPluginsDirectory + "leanify.exe -q -p " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
		}
		// JPEG 2000: ImageMagick
		if (PosEx(sExtensionByContent, KS_EXTENSION_JP2) > 0)
		{
			sFlags = "";
			RunPlugin((unsigned int) iCount, "ImageMagick (1/1)", (sPluginsDirectory + "magick.exe convert \"%INPUTFILE%\" -quiet -quality 0 " + sFlags + "\"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// JPEG: pingo, guetzli, jpeg-recompress, jhead, Leanify, ect, jpegoptim, jpegtran, mozjpegtran
		if (PosEx(sExtensionByContent, KS_EXTENSION_JPG) > 0)
		{
			bool bIsJPEGCMYK = IsJPEGCMYK(sInputFile.c_str());
			
			sFlags = "";
			iLevel = min(gudtOptions.iLevel * 9 / 9, 9);
			sFlags += "-s" + (String) iLevel + " ";
			if (gudtOptions.bJPEGAllowLossy)
			{
				sFlags += "-jpgquality=95 -jpgsub ";
			}
			if (gudtOptions.bJPEGCopyMetadata)
			{
				sFlags += "-nostrip ";
			}
			else
			{
				sFlags += "-strip ";
			}
			RunPlugin((unsigned int) iCount, "pingo (1/10)", (sPluginsDirectory + "pingo.exe -noconversion -jpgtype=2 " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			
			if ((gudtOptions.bJPEGAllowLossy) && (!gudtOptions.bJPEGCopyMetadata))
			{
				RunPlugin((unsigned int) iCount, "Guetzli (2/10)", (sPluginsDirectory + "guetzli.exe --auto --quality 90 \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
			
			if (gudtOptions.bJPEGAllowLossy)
			{
				sFlags = "";
				if (!gudtOptions.bJPEGCopyMetadata)
				{
					sFlags += "--strip ";
				}
				if (gudtOptions.iLevel >= 5)
				{
					sFlags += "--accurate ";
				}
				RunPlugin((unsigned int) iCount, "jpeg-recompress (3/10)", (sPluginsDirectory + "jpeg-recompress.exe --method smallfry --quality high --min 60 --subsample disable --quiet " + sFlags + "\"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
			//jhead is corrupting CMYK JPEG images
			if (!bIsJPEGCMYK)
			{
				sFlags = "";
				if (gudtOptions.bJPEGCopyMetadata)
				{
					sFlags += "-zt ";
				}
				else
				{
					sFlags += "-purejpg -di -dx -dt -zt ";
				}
				RunPlugin((unsigned int) iCount, "jhead (4/10)", (sPluginsDirectory + "jhead.exe -q -autorot " + sFlags + " \"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
			sFlags = "";
			if (gudtOptions.bJPEGCopyMetadata)
			{
				sFlags += "--keep-exif --keep-icc --jpeg-keep-all ";
			}
			if (gudtOptions.bJPEGUseArithmeticEncoding)
			{
				sFlags += "--jpeg-arithmetic ";
			}		
			//iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
			//Overwrite Leanify iterations
			if (gudtOptions.iLeanifyIterations != -1)
			{
				iLevel = gudtOptions.iLeanifyIterations;
			}
			else
			{
				iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
			}
			sFlags += "-i " + (String) iLevel + " ";
			RunPlugin((unsigned int) iCount, "Leanify (5/10)", (sPluginsDirectory + "leanify.exe -q -p " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			if (gudtOptions.bJPEGAllowLossy)
			{
				sFlags = "";
				if (!gudtOptions.bJPEGCopyMetadata)
				{
					sFlags += "-strip ";
				}
				//Seems to cause some loss of quality
				RunPlugin((unsigned int) iCount, "ImageMagick (6/10)", (sPluginsDirectory + "magick.exe convert \"%INPUTFILE%\" -quiet -interlace Plane -define jpeg:optimize-coding=true " + sFlags + "\"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}

			sFlags = "";
			if (!gudtOptions.bJPEGCopyMetadata)
			{
				sFlags += "--strip-all ";
			}
			if (gudtOptions.bJPEGUseArithmeticEncoding)
			{
				sFlags += "--with-arith ";
			}					
			RunPlugin((unsigned int) iCount, "jpegoptim (7/10)", (sPluginsDirectory + "jpegoptim.exe -o -q --all-progressive --nofix " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			sFlags = "";
			if (gudtOptions.bJPEGUseArithmeticEncoding)
			{
				sFlags += "-arithmetic ";
			}
			if (gudtOptions.bJPEGCopyMetadata)
			{
				sFlags += "-copy all ";
			}
			else
			{
				sFlags += "-copy none ";
			}
			RunPlugin((unsigned int) iCount, "jpegtran (8/10)", (sPluginsDirectory + "jpegtran.exe -progressive -optimize " + sFlags + "\"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			RunPlugin((unsigned int) iCount, "mozjpegtran (9/10)", (sPluginsDirectory + "mozjpegtran.exe -outfile \"%TMPOUTPUTFILE%\" -progressive -optimize -perfect " + sFlags + "\"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			
			sFlags = "";
			if (!gudtOptions.bJPEGCopyMetadata)
			{
				sFlags += "-strip ";
			}
			iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
			//Convert level 9 to level 90032 because it is faster and usually with higher savings
			if (iLevel == 9)
			{
				iLevel = 90032;
			}
			sFlags += "-" + (String) iLevel + " ";
			RunPlugin((unsigned int) iCount, "ECT (10/10)", (sPluginsDirectory + "ECT.exe -quiet --mt-deflate --mt-file --allfilters -progressive " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		
		// JPEG XL: ImageMagick
		if (PosEx(sExtensionByContent, KS_EXTENSION_JXL) > 0)
		{
			sFlags = "";
			iLevel = min(gudtOptions.iLevel * 9 / 9, 9);
			sFlags += "jxl:effort=" + (String) iLevel + " ";
			RunPlugin((unsigned int) iCount, "ImageMagick (1/1)", (sPluginsDirectory + "magick.exe convert \"%INPUTFILE%\" -quiet " + sFlags + "\"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		

		// JS: jsmin, minify
		if ((PosEx(sExtensionByContent, KS_EXTENSION_JS) > 0) || (PosEx(sExtensionByContent, " " + ReplaceStr((String) gudtOptions.acJSAdditionalExtensions, ";", " ") + " ") > 0))
		{
			//If JSMin is enabled or it is a custom extension (we assume custom extensions always enable it)
			if ((gudtOptions.bJSEnableJSMin) || (PosEx(sExtensionByContent, " " + ReplaceStr((String) gudtOptions.acJSAdditionalExtensions, ";", " ") + " ") > 0))
			{
				RunPlugin((unsigned int) iCount, "jsmin (1/2)", (sPluginsDirectory + "jsmin.bat \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				
				RunPlugin((unsigned int) iCount, "Minify (2/2)", (sPluginsDirectory + "minify.exe \"%INPUTFILE%\" --output \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
		}
		// LUA: Leanify
		if (PosEx(sExtensionByContent, KS_EXTENSION_LUA) > 0)
		{
			if (gudtOptions.bLUAEnableLeanify)
			{
				sFlags = "";
				//iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
				//Overwrite Leanify iterations
				if (gudtOptions.iLeanifyIterations != -1)
				{
					iLevel = gudtOptions.iLeanifyIterations;
				}
				else
				{
					iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
				}
				sFlags += "-i " + (String) iLevel + " ";
				RunPlugin((unsigned int) iCount, "Leanify (1/1)", (sPluginsDirectory + "leanify.exe -q -p " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
		}
		// MIME: Leanify
		if (PosEx(sExtensionByContent, KS_EXTENSION_MIME) > 0)
		{
			sFlags = "";
			//iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
			//Overwrite Leanify iterations
			if (gudtOptions.iLeanifyIterations != -1)
			{
				iLevel = gudtOptions.iLeanifyIterations;
			}
			else
			{
				iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
			}
			sFlags += "-i " + (String) iLevel + " ";
			RunPlugin((unsigned int) iCount, "Leanify (1/1)", (sPluginsDirectory + "leanify.exe -q -p " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// MKV: ffmpeg, mkclean
		if (PosEx(sExtensionByContent, KS_EXTENSION_MKV) > 0)
		{
			sFlags = "";
			if (!gudtOptions.bMP4CopyMetadata)
			{
				sFlags += "-map_metadata -1 ";
			}			
			RunPlugin((unsigned int) iCount, "ffmpeg (1/2)", (sPluginsDirectory + "ffmpeg.exe -i \"%INPUTFILE%\" -vcodec copy -acodec copy -map 0 " + sFlags + "\"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			
			RunPlugin((unsigned int) iCount, "mkclean (2/2)", (sPluginsDirectory + "mkclean.exe --optimize --unsafe --quiet \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// MNG: advmng
		if (PosEx(sExtensionByContent, KS_EXTENSION_MNG) > 0)
		{
			sFlags = "";
			//iLevel = min(gudtOptions.iLevel * 7 / 9, 7) + 1;
			iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
			sFlags += "-i " + (String) iLevel + " ";
			RunPlugin((unsigned int) iCount, "advmng (1/1)", (sPluginsDirectory + "advmng.exe -z -r -q -4 " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// MP3: MP3packer
		if (PosEx(sExtensionByContent, KS_EXTENSION_MP3) > 0)
		{
			sFlags = "";
			if (!gudtOptions.bMP3CopyMetadata)
			{
				sFlags += "-t -s ";
			}
			RunPlugin((unsigned int) iCount, "MP3packer (1/1)", (sPluginsDirectory + "mp3packer.exe " + sFlags + "-z -a \"\" -A -f \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// MP4: ffmpeg, mp4v2
		if (PosEx(sExtensionByContent, KS_EXTENSION_MP4) > 0)
		{
			sFlags = "";
			if (!gudtOptions.bMP4CopyMetadata)
			{
				sFlags += "-map_metadata -1 ";
			}			
			RunPlugin((unsigned int) iCount, "ffmpeg (1/2)", (sPluginsDirectory + "ffmpeg.exe -i \"%INPUTFILE%\" -vcodec copy -acodec copy -map 0 " + sFlags + "\"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			
			RunPlugin((unsigned int) iCount, "mp4v2 (2/2)", (sPluginsDirectory + "mp4file.exe --optimize -q \"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// OBJ: strip
		if (PosEx(sExtensionByContent, KS_EXTENSION_OBJ) > 0)
		{
			RunPlugin((unsigned int) iCount, "strip (1/1)", (sPluginsDirectory + "strip.exe --strip-all -o \"%TMPOUTPUTFILE%\" \"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// OGG: optivorbis, rehuff_theora
		if (PosEx(sExtensionByContent, KS_EXTENSION_OGG) > 0)
		{
			RunPlugin((unsigned int) iCount, "OptiVorbis (1/2)", (sPluginsDirectory + "optivorbis.exe --vendor_string_action empty --comment_fields_action delete \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			
			RunPlugin((unsigned int) iCount, "rehuff_theora (2/2)", (sPluginsDirectory + "rehuff_theora.exe \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// OGV: ffmpeg, rehuff_theora
		if (PosEx(sExtensionByContent, KS_EXTENSION_OGV) > 0)
		{
			sFlags = "";
			if (!gudtOptions.bMP4CopyMetadata)
			{
				sFlags += "-map_metadata -1 ";
			}			
			RunPlugin((unsigned int) iCount, "ffmpeg (1/2)", (sPluginsDirectory + "ffmpeg.exe -i \"%INPUTFILE%\" -vcodec copy -acodec copy -map 0 " + sFlags + "\"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			
			RunPlugin((unsigned int) iCount, "rehuff_theora (2/2)", (sPluginsDirectory + "rehuff_theora.exe \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// Microsoft OLE Compound Files: Document Press, Best CFBF
		if (PosEx(sExtensionByContent, KS_EXTENSION_OLE) > 0)
		{
			RunPlugin((unsigned int) iCount, "Document Press (1/2)", (sPluginsDirectory + "docprc.exe -opt \"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			RunPlugin((unsigned int) iCount, "Best CFBF (2/2)", (sPluginsDirectory + "bestcfbf.exe \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\" -v4").c_str(), sInputFile, "", 0, 0);
		}
		// PCX: ImageMagick
		if (PosEx(sExtensionByContent, KS_EXTENSION_PCX) > 0)
		{
			sFlags = "";
			if (!gudtOptions.bPCXCopyMetadata)
			{
				sFlags += "-strip ";
			}
			RunPlugin((unsigned int) iCount, "ImageMagick (1/1)", (sPluginsDirectory + "magick.exe convert \"%INPUTFILE%\" -quiet -compress RLE " + sFlags + "\"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// PDF: mutool, ghostcript, cpdf
		if (PosEx(sExtensionByContent, KS_EXTENSION_PDF) > 0)
		{
			bool bIsPDFLayered = IsPDFLayered(sInputFile.c_str());
			
			//Skip Ghostcript on PDF with layers, or if no downsampling is selected, because GS always downsample images.
			if ((!bIsPDFLayered) || (!gudtOptions.bPDFSkipLayered) || (_tcscmp(gudtOptions.acPDFProfile, _T("none")) == 0))
			{		
				//RunPlugin((unsigned int) iCount, "mutool (1/3)", (sPluginsDirectory + "mutool.exe clean -gggg -z \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				RunPlugin((unsigned int) iCount, "mutool (1/3)", (sPluginsDirectory + "mutool.exe clean -g -z \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				
				//Do not use Ghoscript for Adobe Illustrator (AI) files
				if (!EndsText(".ai", sInputFile))
				{
					sFlags = "";
					//Custom mode
					if (_tcscmp(gudtOptions.acPDFProfile, _T("Custom")) == 0)
					{
						sFlags += "-dPDFSETTINGS=/ebook -dDownsampleColorImages=true -dColorImageResolution=" + (String) gudtOptions.iPDFCustomDPI + " -dDownsampleGrayImages=true -dGrayImageResolution=" + (String) gudtOptions.iPDFCustomDPI + " -dDownsampleMonoImages=true -dMonoImageResolution=" + (String) gudtOptions.iPDFCustomDPI + " ";
					}
					//No downsampling
					else if (_tcscmp(gudtOptions.acPDFProfile, _T("none")) == 0)
					{
						sFlags += "-dPDFSETTINGS=/default -c \".setpdfwrite <</ColorACSImageDict>[1 1 1 1] /VSamples [1 1 1 1] /Blend 1>> /GrayACSImageDict<</QFactor>[1 1 1 1] /VSamples [1 1 1 1] /Blend 1>>>> setdistillerparams\" ";
					}				
					//Built in downsample modes: screen, ebook, printer, prepress
					else
					{
						sFlags += "-dPDFSETTINGS=/" + (String) gudtOptions.acPDFProfile + " ";
					}
					
					sFlags += "-dColorImageDownsampleType=/Bicubic -dGrayImageDownsampleType=/Bicubic -dMonoImageDownsampleType=/Bicubic -dOptimize=true -dConvertCMYKImagesToRGB=true -dColorConversionStrategy=/sRGB -dPrinted=false -q -dBATCH -dNOPAUSE -dSAFER -dDELAYSAFER -dNOPROMPT -sDEVICE=pdfwrite -dDetectDuplicateImages=true -dAutoRotatePages=/None -dCompatibilityLevel=1.4 ";
					
					TCHAR acTmpFilePdf[PATH_MAX + 1];
					_tcsncpy(acTmpFilePdf, sInputFile.c_str(), (sizeof(acTmpFilePdf) / sizeof(TCHAR)) - 5);
					_tcscat(acTmpFilePdf, _T(".pdf"));
					_tcsncpy(acTmpFilePdf, clsUtil::GetShortName(acTmpFilePdf).c_str(), (sizeof(acTmpFilePdf) / sizeof(TCHAR)) - 1);

					#if defined(_WIN64)
						RunPlugin((unsigned int) iCount, "Ghostcript (2/3)", (sPluginsDirectory + "gswin64c.exe " + sFlags + "-sOutputFile=\"" + acTmpFilePdf + "\" \"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
					#else
						RunPlugin((unsigned int) iCount, "Ghostcript", (sPluginsDirectory + "gswin32c.exe " + sFlags + "-sOutputFile=\"" + acTmpFilePdf + "\" \"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
                	#endif
					//If there is size reduction check it is not so high to detect corrupted encrypted PDF
					if (clsUtil::SizeFile(acTmpFilePdf) < clsUtil::SizeFile(sInputFile.c_str()))
					{
						if ((clsUtil::SizeFile(acTmpFilePdf) > 3000) && (clsUtil::SizeFile(sInputFile.c_str()) > 20000))
						{
							clsUtil::CopyFile(acTmpFilePdf, sInputFile.c_str());
						}
					}
					if (!gudtOptions.bDebug)
					{
						clsUtil::DeleteFile(acTmpFilePdf);
					}
				}
				RunPlugin((unsigned int) iCount, "cpdf (3/3)", (sPluginsDirectory + "cpdf.exe -squeeze \"%INPUTFILE%\" -o \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
		}
		// PNG: apngopt, pngquant, PngOptimizer, TruePNG, pngout, optipng, pngwolf, Leanify, ect, pingo, advpng, deflopt, defluff, deflopt
		if (PosEx(sExtensionByContent, KS_EXTENSION_PNG) > 0)
		{
			bool bIsAPNG = IsAPNG(sInputFile.c_str());
			bool bIsPNG9Patch = EndsText(".9.png", sInputFile);

			//Android 9-patch images get broken with advpng, deflopt, optipng, pngoptimizer, pngout, pngrewrite and truepng. Only pngwolf, defluff and leanify seem to be safe. At the moment, detect them by extension .9.png.
			if (bIsAPNG)
			{
				RunPlugin((unsigned int) iCount, "apngopt (1/16)", (sPluginsDirectory + "apngopt.exe \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}

			if (!bIsPNG9Patch)
			{
				if ((gudtOptions.bPNGAllowLossy) && (!bIsAPNG))
				{
					sFlags = "";
					if (!gudtOptions.bPNGCopyMetadata)
					{
						sFlags += "--strip ";
					}
					RunPlugin((unsigned int) iCount, "pngquant (2/16)", (sPluginsDirectory + "pngquant.exe " + sFlags + "--quality=85-95 --speed 1 --ext .png --force \"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				}

				sFlags = "";
				if (gudtOptions.bPNGCopyMetadata)
				{
					sFlags += "-KeepPhysicalPixelDimensions ";
				}
				RunPlugin((unsigned int) iCount, "PngOptimizer (3/16)", (sPluginsDirectory + "PngOptimizer.exe " + sFlags + "-file:\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
			
			if ((!bIsAPNG) && (!bIsPNG9Patch))
			{
				//Disable TruePNG on ICO files because it crashes
				if (PosEx(sExtensionByContent, KS_EXTENSION_ICO) == 0)
				{
					sFlags = "";
					iLevel = min(gudtOptions.iLevel * 3 / 9, 3) + 1;
					sFlags += "-o" + (String) iLevel + " ";
					if (gudtOptions.bPNGCopyMetadata)
					{
						sFlags += "-md keep all ";
					}
					else
					{
						sFlags += "-tz -md remove all -a1 -g1 ";
					}
					if (gudtOptions.bPNGAllowLossy)
					{
						sFlags += "-l ";
					}
					RunPlugin((unsigned int) iCount, "TruePNG (4/16)", (sPluginsDirectory + "truepng.exe " + sFlags + "/i0 /nc /tz /quiet /y /out \"%TMPOUTPUTFILE%\" \"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				}

				//Skip PNGOut when it is a JPEG renamed to PNG
				if (PosEx(" " + GetExtensionByContent(sInputFile, true) + " ", KS_EXTENSION_JPG) == 0)
				{
					sFlags = "";
					if (gudtOptions.bPNGCopyMetadata)
					{
						sFlags += "/k1 ";
					}
					else
					{
						sFlags += "/kacTL,fcTL,fdAT ";
					}
					iLevel = max((gudtOptions.iLevel * 3 / 9) - 3, 0);
					sFlags += "/s" + (String) iLevel + " ";
					RunPlugin((unsigned int) iCount, "PNGOut (5/16)", (sPluginsDirectory + "pngout.exe /q /y /r /d0 /mincodes0 " + sFlags + "\"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				}
			}
			
			sFlags = "";
			iLevel = min(gudtOptions.iLevel * 6 / 9, 6);
			sFlags += "-o" + (String) iLevel + " ";
			if (bIsAPNG)
			{
				// For some reason -strip all -protect acTL,fcTL,fdAT is not keeping APNG chunks
				RunPlugin((unsigned int) iCount, "OptiPNG (6/16)", (sPluginsDirectory + "optipng.exe -zw32k -protect acTL,fcTL,fdAT -quiet " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
			else
			{
				if (!gudtOptions.bPNGCopyMetadata)
				{
					sFlags += "-strip all ";
				}
				RunPlugin((unsigned int) iCount, "OptiPNG (7/16)", (sPluginsDirectory + "optipng.exe -zw32k -quiet " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}

			if (!bIsAPNG)
			{
				sFlags = "";
				//iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
				//Overwrite Leanify iterations
				if (gudtOptions.iLeanifyIterations != -1)
				{
					iLevel = gudtOptions.iLeanifyIterations;
				}
				else
				{
					iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
				}
				sFlags += "-i " + (String) iLevel + " ";
				//Temporary disable Leanify because it removed IPTC metadata
				if (!gudtOptions.bPNGCopyMetadata)
				{
					RunPlugin((unsigned int) iCount, "Leanify (8/16)", (sPluginsDirectory + "leanify.exe -q -p " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				}
				
				sFlags = "";
				//iLevel = min(gudtOptions.iLevel * 7 / 9, 7) + 1;
				if (gudtOptions.iPNGWolfIterations != -1)
				{
					iLevel = gudtOptions.iPNGWolfIterations;
				}
				else
				{
					iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
				}
				sFlags += "--out-deflate=zopfli,iter=" + (String) iLevel + " ";
				RunPlugin((unsigned int) iCount, "pngwolf (9/16)", (sPluginsDirectory + "pngwolf.exe " + sFlags + "--in=\"%INPUTFILE%\" --out=\"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

				if (!bIsPNG9Patch)
				{
					RunPlugin((unsigned int) iCount, "pngrewrite (10/16)", (sPluginsDirectory + "pngrewrite.exe \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

					sFlags = "";
					//iLevel = min(gudtOptions.iLevel * 7 / 9, 7) + 1;
					iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
					sFlags += "-i " + (String) iLevel + " ";
					if (!gudtOptions.bPNGCopyMetadata)
					{					
						RunPlugin((unsigned int) iCount, "advpng (11/16)", (sPluginsDirectory + "advpng.exe -z -q -4 " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
					}
				}
			}

			sFlags = "";
			//ECT will preserve APNG compatibility when --reuse is used and -strip is not used
			if (bIsAPNG)
			{
				sFlags += "--reuse ";
			}
			else if (!gudtOptions.bPNGCopyMetadata)
			{
				sFlags += "-strip ";
			}
			iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
			//Convert level 9 to level 90032 because it is faster and usually with higher savings
			if (iLevel == 9)
			{
				iLevel = 90032;
			}
			sFlags += "-" + (String) iLevel + " ";
			RunPlugin((unsigned int) iCount, "ECT (12/16)", (sPluginsDirectory + "ECT.exe -quiet --mt-deflate --mt-file --allfilters " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			sFlags = "";
			iLevel = min(gudtOptions.iLevel * 9 / 9, 9);
			sFlags += "-s" + (String) iLevel + " ";
			if (gudtOptions.bPNGCopyMetadata)
			{
				sFlags += "-nostrip ";
			}
			else
			{
				sFlags += "-strip ";
			}
			if (gudtOptions.bPNGAllowLossy)
			{
				sFlags += "-auto=95 ";
			}
			RunPlugin((unsigned int) iCount, "pingo (13/16)", (sPluginsDirectory + "pingo.exe -noconversion " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			

			sFlags = "";
			if (gudtOptions.bPNGCopyMetadata)
			{
				sFlags += "/k ";
			}
			
			if ((!bIsAPNG) && (!bIsPNG9Patch))
			{	
				RunPlugin((unsigned int) iCount, "DeflOpt (14/16)", (sPluginsDirectory + "deflopt.exe /a /b /s " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}

			RunPlugin((unsigned int) iCount, "defluff (15/16)", (sPluginsDirectory + "defluff.bat \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			if ((!bIsAPNG) && (!bIsPNG9Patch))
			{
				RunPlugin((unsigned int) iCount, "DeflOpt (16/16)", (sPluginsDirectory + "deflopt.exe /a /b /s " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
		}

		// SWF: Leanfy, flasm, zRecompress
		if (PosEx(sExtensionByContent, KS_EXTENSION_SWF) > 0)
		{
			String sTmpOutputFile = ReplaceStr(sInputFile, ".swf", ".$wf");
			RunPlugin((unsigned int) iCount, "flasm (1/5)", (sPluginsDirectory + "flasm.exe -x \"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			clsUtil::CopyFile(sTmpOutputFile.c_str(), sInputFile.c_str());
			clsUtil::DeleteFile(sTmpOutputFile.c_str());

			RunPlugin((unsigned int) iCount, "flasm (2/5)", (sPluginsDirectory + "flasm.exe -u \"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			clsUtil::CopyFile(sTmpOutputFile.c_str(), sInputFile.c_str());
			clsUtil::DeleteFile(ReplaceStr(sInputFile, ".swf", ".$wf").c_str());

			RunPlugin((unsigned int) iCount, "flasm (3/5)", (sPluginsDirectory + "flasm.exe -z \"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			if (clsUtil::SizeFile(sTmpOutputFile.c_str()) < ParseNumberThousand(grdFiles->Cells[KI_GRID_OPTIMIZED][iCount]))
			{
				clsUtil::CopyFile(sTmpOutputFile.c_str(), sInputFile.c_str());
				grdFiles->Cells[KI_GRID_OPTIMIZED][(int) iCount] = FormatNumberThousand(clsUtil::SizeFile(sInputFile.c_str()));
			}
			clsUtil::DeleteFile(ReplaceStr(sInputFile, ".swf", ".$wf").c_str());

			RunPlugin((unsigned int) iCount, "zRecompress (4/5)", (sPluginsDirectory + "zRecompress.exe -tswf-lzma \"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			sFlags = "";
			//iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
			//Overwrite Leanify iterations
			if (gudtOptions.iLeanifyIterations != -1)
			{
				iLevel = gudtOptions.iLeanifyIterations;
			}
			else
			{
				iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
			}
			sFlags += "-i " + (String) iLevel + " ";
			RunPlugin((unsigned int) iCount, "Leanify (5/5)", (sPluginsDirectory + "leanify.exe -q -p " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// SQLITE: sqlite
		if (PosEx(sExtensionByContent, KS_EXTENSION_SQLITE) > 0)
		{
			RunPlugin((unsigned int) iCount, "sqlite (1/1)", (sPluginsDirectory + "sqlite.bat \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// TAR: Leanify
		if (PosEx(sExtensionByContent, KS_EXTENSION_TAR) > 0)
		{
			sFlags = "";
			if (gudtOptions.bGZCopyMetadata)
			{
				sFlags += "--keep-exif ";
			}
			//iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
			//Overwrite Leanify iterations
			if (gudtOptions.iLeanifyIterations != -1)
			{
				iLevel = gudtOptions.iLeanifyIterations;
			}
			else
			{
				iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
			}
			sFlags += "-i " + (String) iLevel + " ";
			RunPlugin((unsigned int) iCount, "Leanify (1/1)", (sPluginsDirectory + "leanify.exe -q -p " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// Tencent QQ: Leanify
		if (PosEx(sExtensionByContent, KS_EXTENSION_TENCENTQQ) > 0)
		{
			if (!gudtOptions.bPNGCopyMetadata)
			{
				sFlags = "";
				//iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
				//Overwrite Leanify iterations
				if (gudtOptions.iLeanifyIterations != -1)
				{
					iLevel = gudtOptions.iLeanifyIterations;
				}
				else
				{
					iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
				}
				sFlags += "-i " + (String) iLevel + " ";
				RunPlugin((unsigned int) iCount, "Leanify (1/1)", (sPluginsDirectory + "leanify.exe -q -p " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
		}
		
		// TGA: ImageMagick
		if (PosEx(sExtensionByContent, KS_EXTENSION_TGA) > 0)
		{
			sFlags = "";
			if (!gudtOptions.bTGACopyMetadata)
			{
				sFlags += "-strip ";
			}
			RunPlugin((unsigned int) iCount, "ImageMagick (1/1)", (sPluginsDirectory + "magick.exe convert -quiet -compress RLE " + sFlags + "\"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// TIFF: jhead, ImageMagick, jpegoptim, jpegtran, mozjpegtran
		if (PosEx(sExtensionByContent, KS_EXTENSION_TIFF) > 0)
		{
			bool bIsJPEGCMYK = IsJPEGCMYK(sInputFile.c_str());

			//jhead is corrupting CMYK JPEG images
			if (!bIsJPEGCMYK)
			{
				sFlags = "";
				if (gudtOptions.bTIFFCopyMetadata)
				{
					sFlags += "-zt ";
				}
				else
				{
					sFlags += "-purejpg -di -dx -dt -zt ";
				}	
				RunPlugin((unsigned int) iCount, "jhead (1/5)", (sPluginsDirectory + "jhead.exe -q -autorot " + sFlags + " \"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
			// ImageMagick does not keep metadata on TIFF images so disable it
			if (!gudtOptions.bTIFFCopyMetadata)
			{
				RunPlugin((unsigned int) iCount, "ImageMagick (2/5)", (sPluginsDirectory + "magick.exe convert \"%INPUTFILE%\" -quiet -compress ZIP -strip \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
			
			sFlags = "";
			if (!gudtOptions.bTIFFCopyMetadata)
			{
				sFlags += "--strip-all ";
			}
			if (gudtOptions.bJPEGUseArithmeticEncoding)
			{
				sFlags += "--with-arith ";
			}					
			RunPlugin((unsigned int) iCount, "jpegoptim (3/5)", (sPluginsDirectory + "jpegoptim.exe -o -q --all-progressive --nofix " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			sFlags = "";
			if (gudtOptions.bTIFFCopyMetadata)
			{
				sFlags += "-arithmetic ";
			}
			if (gudtOptions.bTIFFCopyMetadata)
			{
				sFlags += "-copy all ";
			}
			else
			{
				sFlags += "-copy none ";
			}
			RunPlugin((unsigned int) iCount, "jpegtran (4/5)", (sPluginsDirectory + "jpegtran.exe -progressive -optimize " + sFlags + "\"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			RunPlugin((unsigned int) iCount, "mozjpegtran (5/5)", (sPluginsDirectory + "mozjpegtran.exe -outfile \"%TMPOUTPUTFILE%\" -progressive -optimize -perfect " + sFlags + "\"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// WAV: shntool
		if (PosEx(sExtensionByContent, KS_EXTENSION_WAV) > 0)
		{
			if (!gudtOptions.bWAVCopyMetadata)
			{
				String sTmpOutputFile = ReplaceStr(sInputFile, ".wav", "-stripped.wav");
				//Prevent a bug in shntool with no lowercase extensions
				if (sTmpOutputFile == sInputFile)
				{
					sTmpOutputFile += "-stripped.wav";
				}

				RunPlugin((unsigned int) iCount, "shntool (1/2)", (sPluginsDirectory + "shntool.exe strip -q -O always \"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				if ((clsUtil::SizeFile(sTmpOutputFile.c_str()) > 0) && (clsUtil::SizeFile(sTmpOutputFile.c_str()) < ParseNumberThousand(grdFiles->Cells[KI_GRID_OPTIMIZED][iCount])))
				{
					clsUtil::CopyFile(sTmpOutputFile.c_str(), sInputFile.c_str());
					grdFiles->Cells[KI_GRID_OPTIMIZED][(int) iCount] = FormatNumberThousand(clsUtil::SizeFile(sInputFile.c_str()));
				}
				if (!gudtOptions.bDebug)
				{
					clsUtil::DeleteFile(sTmpOutputFile.c_str());
				}
				if (gudtOptions.bWAVStripSilence)
				{
					sTmpOutputFile = ReplaceStr(sInputFile, ".wav", "-trimmed.wav");
					//Prevent a bug in shntool with no lowercase extensions
					if (sTmpOutputFile == sInputFile)
					{
						sTmpOutputFile += "-trimmed.wav";
					}

					RunPlugin((unsigned int) iCount, "shntool (2/2)", (sPluginsDirectory + "shntool.exe trim -q -O always \"%INPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
					if ((clsUtil::SizeFile(sTmpOutputFile.c_str()) > 0) && (clsUtil::SizeFile(sTmpOutputFile.c_str()) < ParseNumberThousand(grdFiles->Cells[KI_GRID_OPTIMIZED][iCount])))
					{
						clsUtil::CopyFile(sTmpOutputFile.c_str(), sInputFile.c_str());
						grdFiles->Cells[KI_GRID_OPTIMIZED][(int) iCount] = FormatNumberThousand(clsUtil::SizeFile(sInputFile.c_str()));
					}
					if (!gudtOptions.bDebug)
					{
						clsUtil::DeleteFile(sTmpOutputFile.c_str());
					}
				}
			}
		}
		// XML: Leanify
		if (PosEx(sExtensionByContent, KS_EXTENSION_XML) > 0)
		{
			if (gudtOptions.bXMLEnableLeanify)
			{
				sFlags = "";
				//iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
				//Overwrite Leanify iterations
				if (gudtOptions.iLeanifyIterations != -1)
				{
					iLevel = gudtOptions.iLeanifyIterations;
				}
				else
				{
					iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
				}
				sFlags += "-i " + (String) iLevel + " ";
				RunPlugin((unsigned int) iCount, "Leanify (1/2)", (sPluginsDirectory + "leanify.exe -q -p " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				
				RunPlugin((unsigned int) iCount, "Minify (2/2)", (sPluginsDirectory + "minify.exe \"%INPUTFILE%\" --output \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
		}
		// WEBP: pingo, dwebp + cwebp, ImageWorsener
		if (PosEx(sExtensionByContent, KS_EXTENSION_WEBP) > 0)
		{
			sFlags = "";
			iLevel = min(gudtOptions.iLevel * 9 / 9, 9);
			sFlags += "-s" + (String) iLevel + " ";
			if (gudtOptions.bWEBPAllowLossy)
			{
				sFlags += "-webp-lossy=95 ";
			}
			else
			{
				sFlags += "-webp-lossless ";
			}
		
			RunPlugin((unsigned int) iCount, "pingo (1/2)", (sPluginsDirectory + "pingo.exe -noconversion " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
					
			sFlags = "";
			if (gudtOptions.bWEBPAllowLossy)
			{
				iLevel = min(gudtOptions.iLevel * 6 / 9, 6);
				sFlags += "-m " + (String) iLevel + " ";
			}
			else
			{
				iLevel = min(gudtOptions.iLevel * 9 / 9, 9);
				sFlags += "-z " + (String) iLevel + " -lossless ";
			}
			RunPlugin((unsigned int) iCount, "cwebp (2/2)", (sPluginsDirectory + "cwebp.exe -mt -quiet " + sFlags + "\"%INPUTFILE%\" -o \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			//RunPlugin((unsigned int) iCount, "ImageWorsener", (sPluginsDirectory + "imagew.exe -noresize -zipcmprlevel 9 \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// ZIP: Leanify, ect, advzip, deflopt, defluff, deflopt
		if (PosEx(sExtensionByContent, KS_EXTENSION_ZIP) > 0)
		{
			bool bIsEXESFX = IsEXESFX(sInputFile.c_str());
			
			sFlags = "";
			if (gudtOptions.bZIPCopyMetadata)
			{
				sFlags += "--keep-exif ";
			}
			//iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
			//Overwrite Leanify iterations
			if (gudtOptions.iLeanifyIterations != -1)
			{
				iLevel = gudtOptions.iLeanifyIterations;
			}
			else
			{
				iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
			}
			sFlags += "-i " + (String) iLevel + " ";
			//Limit ZIP no recurse to ZIP extension
			if ((!gudtOptions.bZIPRecurse) && (PosEx(sExtensionByContent, " .zip ") > 0))
			{
				sFlags += "-d 1 ";
				//sFlags += "-f ";
			}
			sFlags += "--zip-deflate ";
			RunPlugin((unsigned int) iCount, "Leanify (1/6)", (sPluginsDirectory + "leanify.exe -q -p " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			sFlags = "";
			if (!gudtOptions.bZIPCopyMetadata)
			{
				sFlags += "-strip ";
			}
			if ((!gudtOptions.bZIPRecurse) && (PosEx(sExtensionByContent, " .zip ") > 0))
			{
				sFlags += "--disable-png --disable-jpg ";
			}
			iLevel = min(gudtOptions.iLevel * 8 / 9, 8) + 1;
			//Convert level 9 to level 90032 because it is faster and usually with higher savings
			if (iLevel == 9)
			{
				iLevel = 90032;
			}
			sFlags += "-" + (String) iLevel + " ";
			RunPlugin((unsigned int) iCount, "ECT (2/6)", (sPluginsDirectory + "ECT.exe -quiet --mt-deflate --mt-file -zip " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			//AdvZip strips header on ZIP files
			if (!bIsEXESFX)
			{
				sFlags = "";
				//iLevel = min(gudtOptions.iLevel * 7 / 9, 7) + 1;
				iLevel = ((gudtOptions.iLevel * gudtOptions.iLevel * gudtOptions.iLevel) / 25) + 1; //1, 1, 2, 3, 6, 9, 14, 21, 30
				sFlags += "-i " + (String) iLevel + " ";
				RunPlugin((unsigned int) iCount, "advzip (3/6)", (sPluginsDirectory + "advzip.exe -z -q -4 " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
			
			sFlags = "";
			if (gudtOptions.bZIPCopyMetadata)
			{
				sFlags += "/c ";
			}
			RunPlugin((unsigned int) iCount, "DeflOpt (4/6)", (sPluginsDirectory + "deflopt.exe /a /b /s " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			RunPlugin((unsigned int) iCount, "defluff (5/6)", (sPluginsDirectory + "defluff.bat \"%INPUTFILE%\" \"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);

			RunPlugin((unsigned int) iCount, "DeflOpt (6/6)", (sPluginsDirectory + "deflopt.exe /a /b /s " + sFlags + "\"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
		}
		// 7Z: m7zRepacker
		if (PosEx(sExtensionByContent, KS_EXTENSION_7Z) > 0)
		{
			//Very slow, use it only in high compression profiles
			if (gudtOptions.iLevel > 7)
			{
				#if defined(_WIN64)
					RunPlugin((unsigned int) iCount, "m7zRepacker (1/1)", (sPluginsDirectory + "m7zrepacker.exe -m1 -d1024 -mem2048 \"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
				#else
					RunPlugin((unsigned int) iCount, "m7zRepacker (1/1)", (sPluginsDirectory + "m7zrepacker.exe -m1 -d128 -mem512 \"%TMPINPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
                #endif
			}
		}
		// MISC: ImageMagick
		if (PosEx(sExtensionByContent, KS_EXTENSION_MISC) > 0)
		{
			if (!gudtOptions.bMiscDisable)
			{
				sFlags = "";
				if (!gudtOptions.bMiscCopyMetadata)
				{
					sFlags += "-strip ";
				}
				RunPlugin((unsigned int) iCount, "ImageMagick (1/1)", (sPluginsDirectory + "magick.exe convert \"%INPUTFILE%\" -quiet " + sFlags + "\"%TMPOUTPUTFILE%\"").c_str(), sInputFile, "", 0, 0);
			}
		}

		if (gudtOptions.bKeepAttributes)
		{
			//Restore timestamp if we were able to get it
			if ((udtFileCreated.dwLowDateTime != 0) && (udtFileCreated.dwHighDateTime != 0))
			{
				clsUtil::SetFileTimestamp(clsUtil::GetShortName(sInputFile).c_str(), &udtFileCreated, &udtFileAccessed, &udtFileModified);
			}
			if (iFileAttributes != INVALID_FILE_ATTRIBUTES)
			{
				SetFileAttributes(clsUtil::GetShortName(sInputFile).c_str(), iFileAttributes);
			}
		}

		//Make sure the file was indeed processed because asuming we got gains. This is to solve Pending items being counted as 0 bytes
		if (grdFiles->Cells[KI_GRID_OPTIMIZED][iCount] != "")
		{
			lTotalBytes += ParseNumberThousand(grdFiles->Cells[KI_GRID_ORIGINAL][iCount]);
			lSavedBytes += (ParseNumberThousand(grdFiles->Cells[KI_GRID_ORIGINAL][iCount]) - ParseNumberThousand(grdFiles->Cells[KI_GRID_OPTIMIZED][iCount]));

			gudtOptions.lStatTotalBytes += ParseNumberThousand(grdFiles->Cells[KI_GRID_ORIGINAL][iCount]);
			gudtOptions.lStatSavedBytes += (ParseNumberThousand(grdFiles->Cells[KI_GRID_ORIGINAL][iCount]) - ParseNumberThousand(grdFiles->Cells[KI_GRID_OPTIMIZED][iCount]));
			gudtOptions.iStatFiles++;
		}
	}

	//If file was not processed, mark it as skipped because not supported extension, or skipped because user preference (do not process JS for instance)
	if ((grdFiles->Cells[KI_GRID_STATUS][iCount] == _("Pending")) || (grdFiles->Cells[KI_GRID_STATUS][iCount] == _("Copying to Recyclebin...")) || (grdFiles->Cells[KI_GRID_STATUS][iCount] == _("Creating backup...")))
	{
		grdFiles->Cells[KI_GRID_STATUS][iCount] = _("Skipped");
	}
	else if (grdFiles->Cells[KI_GRID_STATUS][iCount] != _("Optimized"))
	{
		unsigned int iPercentBytes = ((unsigned int) ((double) ParseNumberThousand(grdFiles->Cells[KI_GRID_OPTIMIZED][iCount]) / ParseNumberThousand(grdFiles->Cells[KI_GRID_ORIGINAL][iCount]) * 100));

		//Required indirection
		
		iEndTicks = GetTickCount();
		TCHAR acTime[64];
		StrFromTimeInterval(acTime, (sizeof(acTime) / sizeof(TCHAR)) - 1, (unsigned int) (iEndTicks - iStartTicks), sizeof(acTime) - 1);
		
		sCaption.printf(_((TCHAR *) _T("Done (%3u%%) in %s")), iPercentBytes, acTime);
		grdFiles->Cells[KI_GRID_STATUS][iCount] = sCaption;

		//Update cache
		if (gudtOptions.bEnableCache)
		{
			String sHashValue = Hash(sInputFile.c_str());
			unsigned int iHashKey = clsUtil::Crc32(sHashValue.c_str(), (unsigned int) sHashValue.Length());
			clsUtil::SetIni(_T("Cache"), ((String) iHashKey).c_str(), sHashValue.c_str());
		}
	}
	RefreshStatus(true, (unsigned int) iCount, lTotalBytes, lSavedBytes);

	//Abort for loop if operation is cancelled
	if (gbStop)
	{
		return;
	}
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::tmrMainTimer(TObject *Sender)
{
	//Elapsed time
	tmrMain->Tag += tmrMain->Interval;
	
	//30 seconds: Update check and enable ads
	if (tmrMain->Tag >= 30000)
	{
		tmrMain->Enabled = false;
		if (gudtOptions.iCheckForUpdates < 0)
		{
			String sCaption;
			sCaption.printf(_((TCHAR *) _T("Do you want %s to automatically check for updates?")), Application->Name.c_str());
			gudtOptions.iCheckForUpdates = (clsUtil::MsgBox(Handle, sCaption.c_str(), _((TCHAR *) _T("Check for updates")), MB_YESNO | MB_ICONQUESTION) == ID_YES);
		}
		else if (gudtOptions.iCheckForUpdates == 1)
		{
			CheckForUpdates(true);
		}
	}
	//1 second: Process command-line arguments
	else if (tmrMain->Tag >= gudtOptions.iStartupDelay)
	{
		tmrMain->Interval = 30000;

		if (_argc > 1)
		{
			Screen->Cursor = crAppStart;
			grdFiles->Enabled = false;  //Prevent grid modifications while adding files
			//Show();	//Required because some themes do not automatically refresh
			Application->ProcessMessages();
			AddFilesInitializeExist();
			bool bAdded = false;
			for (unsigned int iCount = 1; iCount < (unsigned int) _argc; iCount++)
			{
				//Skip options starting with /
				if (_targv[iCount][0] != '/')
				{
					AddFiles(_targv[iCount]);
					bAdded = true;
				}
			}
			if (bAdded)
			{
				gudtOptions.bBeepWhenDone = false;  //Disable Beep when done to allow closing automatically
				grdFiles->Enabled = true;
				RefreshStatus();
				Screen->Cursor = crDefault;
				actOptimizeExecute(Sender);
				actExitExecute(Sender);
			}
		}
	}
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::lblCopyrightClick(TObject *Sender)
{
	ShellExecute(Handle, _T("open"), KS_APP_URL, _T(""), _T(""), SW_SHOWNORMAL);
}



// ---------------------------------------------------------------------------
void __fastcall TfrmMain::WMDropFiles(const TWMDropFiles &udtMessage)
{
	HDROP hDrop = (HDROP) udtMessage.Drop;
	if (hDrop)
	{
		unsigned int iFiles = DragQueryFile(hDrop, -1, NULL, NULL);
		if (iFiles > 0)
		{
			Screen->Cursor = crAppStart;
			grdFiles->Enabled = false;  //Prevent grid modifications while adding files
			Application->ProcessMessages();
			AddFilesInitializeExist();
			for (unsigned int iCount = 0; iCount < iFiles; iCount++)
			{
				TCHAR acBuffer[PATH_MAX + 1];

				if (DragQueryFile(hDrop, iCount, acBuffer, (sizeof(acBuffer) / sizeof(TCHAR)) - 1))
				{
					AddFiles(acBuffer);
				}
			}
            grdFiles->Enabled = true;
			RefreshStatus();
			Screen->Cursor = crDefault;
		}
	}
	DragFinish(hDrop);
}



static THashedStringList *mlstFilesExist = NULL;

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::AddFilesInitializeExist(void)
{
	if (!gudtOptions.bAllowDuplicates)
	{
		if (!mlstFilesExist)
		{
			mlstFilesExist = new THashedStringList();
			mlstFilesExist->CaseSensitive = true;
			mlstFilesExist->Duplicates = System::Classes::dupIgnore;
			mlstFilesExist->Sorted = true;
		}
		else
		{
			mlstFilesExist->Clear();
		}
		mlstFilesExist->Assign(grdFiles->Cols[KI_GRID_FILE]);
	}
}



// ---------------------------------------------------------------------------
bool __fastcall TfrmMain::AddFilesExist(const String psFile)
{
	bool bRes = false;

	if ((!gudtOptions.bAllowDuplicates) && (mlstFilesExist))
	{
		int iIndex = 0;
		bRes = mlstFilesExist->Find(psFile, iIndex);
		if (!bRes)
		{
			mlstFilesExist->Add(psFile);
		}
	}
	return (bRes);
}



// ---------------------------------------------------------------------------
void __fastcall TfrmMain::AddFiles(const TCHAR *pacFile)
{
	WIN32_FIND_DATA udtFindFileData;
	WIN32_FILE_ATTRIBUTE_DATA udtFileAttribute;


	if (GetFileAttributesEx(pacFile, GetFileExInfoStandard, (void *) &udtFileAttribute))
	{
		//If it is a directory, recurse
		if (udtFileAttribute.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			HANDLE hFindFile = FindFirstFile((((String) pacFile) + "\\*").c_str(), &udtFindFileData);
			if (hFindFile != INVALID_HANDLE_VALUE )
			{
				do
				{
					if ((_tcscmp(udtFindFileData.cFileName, _T(".")) != 0) &&
						(_tcscmp(udtFindFileData.cFileName, _T("..")) != 0))
					{
						AddFiles((((String) pacFile) + "\\" + udtFindFileData.cFileName).c_str());
					}
				}
				while ((bool) FindNextFile(hFindFile, &udtFindFileData) != false);
				FindClose(hFindFile);
				RefreshStatus();
			}
		}
		//It it is a file, parse it
		else
		{
			String sCellFile = SetCellFileValue(pacFile);
			unsigned long long lSize = clsUtil::SizeFile(pacFile);

			//We will only add files with more than 0 bytes
			if (lSize > 0)
			{
				//Check if already added
				if (!gudtOptions.bAllowDuplicates)
				{
					//if (grdFiles->Cols[KI_GRID_FILE]->IndexOf(sCellFile) >= 0)
					if (AddFilesExist(sCellFile))
					{
						return;
					}
				}

				String sExtensionByContent = GetExtensionByContent(pacFile);
				if (sExtensionByContent != "")
				{
					//We store the name to show concatenated with the full name
					unsigned int iRows = (unsigned int) grdFiles->RowCount;
					grdFiles->Rows[(int) iRows]->BeginUpdate();

					TStringList *lstRow = new TStringList();
					lstRow->Add(sCellFile); //0: File

					String sExtension = ExtractFileExt(pacFile).LowerCase();
					if (sExtensionByContent != sExtension)
					{
						lstRow->Add(sExtension + " (" + sExtensionByContent + ")"); //1: Extension (Type)
					}
					else
					{
						lstRow->Add(sExtension); //1: Extension (Type)
					}

					lstRow->Add(FormatNumberThousand(lSize)); //2: Original size
					lstRow->Add(""); //3: Optimized size

					//Check if it was already optimized
					if (gudtOptions.bEnableCache)
					{
						String sHashValue = Hash(pacFile);
						unsigned int iHashKey = clsUtil::Crc32(sHashValue.c_str(), (unsigned int) sHashValue.Length());
						//In cache, show it as already optimized
						if (_tcscmp(clsUtil::GetIni(_T("Cache"), ((String) iHashKey).c_str(), _T("")), sHashValue.c_str()) == 0)
						{
							lstRow->Add(_("Optimized")); //4: Status
						}
						else
						{
							lstRow->Add(_("Pending")); //4: Status
						}
					}
					else
					{
						lstRow->Add(_("Pending")); //4: Status
					}
					grdFiles->Rows[(int) iRows] = lstRow;
					delete lstRow;
					grdFiles->Rows[(int) iRows]->EndUpdate();
					grdFiles->RowCount = (int) iRows + 1;
				}
			}
		}
	}
}



// ---------------------------------------------------------------------------
int __fastcall TfrmMain::RunPlugin(unsigned int piCurrent, String psStatus, String psCommandLine, String psInputFile, String psOutputFile, int piErrorMin, int piErrorMax)
{
	String sInputFile, sOutputFile, sTmpInputFile, sTmpOutputFile, sCommandLine;
	TCHAR acTempPath[PATH_MAX + 1] = _T("");


	if (gbStop)
	{
		//Close();
		return (0);
	}

	//Check if it is an excluded plugins
	TCHAR *acToken = _tcstok(((String) gudtOptions.acDisablePluginMask).UpperCase().c_str(), _T(";"));
	while (acToken)
	{
		if (PosEx((String) acToken, psCommandLine.UpperCase()) != 0)
		{
			return (0);
		}
		acToken = _tcstok(NULL, _T(";"));
	}

	sInputFile = psInputFile;
	sOutputFile = psOutputFile;
	sCommandLine = psCommandLine;

	//Avoid temporary name collisions across different instances
	unsigned int iRandom = (unsigned int) clsUtil::Random(0, 9999);

	//Use specified option temp directory if exists
	if (gudtOptions.acTempDirectory[0] != NULL)
	{
		//Add final slash if it has not
		if (gudtOptions.acTempDirectory[_tcslen(gudtOptions.acTempDirectory) - 1] != '\\')
		{
			_tcscat(gudtOptions.acTempDirectory, _T("\\"));
		}
		_tcsncpy(acTempPath, gudtOptions.acTempDirectory, (sizeof(acTempPath) / sizeof(TCHAR)) - 1);
	}
	else
	{
		if (!GetTempPath((sizeof(acTempPath) / sizeof(TCHAR)) - 1, acTempPath))
		{
			memset(acTempPath, 0, sizeof(acTempPath));
		}
	}

	//Create temporary directory just in case it did not existed
	if (gudtOptions.acTempDirectory[0] != NULL)
	{
		clsUtil::DirectoryCreate(gudtOptions.acTempDirectory);
	}
	sTmpInputFile = (String) acTempPath + Application->Name + "_Input_" + (String) iRandom + "_" + ExtractFileName(sInputFile);

	sTmpOutputFile = (String) acTempPath + Application->Name + "_Output_" + (String) iRandom + "_" + ExtractFileName(sInputFile);

	if (!gudtOptions.bDebug)
	{
		clsUtil::DeleteFile(sTmpInputFile.c_str());
		clsUtil::DeleteFile(sTmpOutputFile.c_str());
	}

	//Required indirection
	String sCaption;
	sCaption.printf(_((TCHAR *) _T("Running %s...")), psStatus.c_str());
	grdFiles->Cells[KI_GRID_STATUS][(int) piCurrent] = sCaption;

	unsigned long long lSize = clsUtil::SizeFile(sInputFile.c_str());
	unsigned long long lSizeNew = lSize;
	grdFiles->Cells[KI_GRID_OPTIMIZED][(int) piCurrent] = FormatNumberThousand(lSize);

	Application->ProcessMessages();

	//Handle copying original file, if there is not Output nor Tmp for commands that only accept 1 file
	if ((PosEx("%OUTPUTFILE%", psCommandLine) == 0) && (PosEx("%TMPOUTPUTFILE%", psCommandLine) == 0))
	{
		clsUtil::CopyFile(sInputFile.c_str(), sTmpInputFile.c_str());
		//sInputFile = sTmpOutputFile;
	}

	//sCommandLine = StringReplace(sCommandLine, "%INPUTFILE%", sInputFile, TReplaceFlags() << rfReplaceAll);
	sCommandLine = ReplaceStr(sCommandLine, "%INPUTFILE%", sInputFile);

	//sCommandLine = StringReplace(sCommandLine, "%OUTPUTFILE%", sOutputFile, TReplaceFlags() << rfReplaceAll);
	sCommandLine = ReplaceStr(sCommandLine, "%OUTPUTFILE%", sOutputFile);

	//sCommandLine = StringReplace(sCommandLine, "%TMPINPUTFILE%", sTmpInputFile, TReplaceFlags() << rfReplaceAll);
	sCommandLine = ReplaceStr(sCommandLine, "%TMPINPUTFILE%", sTmpInputFile);

	//sCommandLine = StringReplace(sCommandLine, "%TMPOUTPUTFILE%", sTmpOutputFile, TReplaceFlags() << rfReplaceAll);
	sCommandLine = ReplaceStr(sCommandLine, "%TMPOUTPUTFILE%", sTmpOutputFile);

	int iError;
	TDateTime dteStart, dteEnd;
	if (lSize > 0)
	{
		dteStart = dteStart.CurrentDateTime();
		iError = (int) RunProcess(sCommandLine.c_str(), NULL, 0, true);
		dteEnd = dteEnd.CurrentDateTime();

		//Check exit errorlevel
		if ((iError >= piErrorMin) && (iError <= piErrorMax))
		{
			//We did get a TMP output file, so if smaller, make it overwrite input file
			if (PosEx("%TMPOUTPUTFILE%", psCommandLine) != 0)
			{
				lSizeNew = clsUtil::SizeFile(sTmpOutputFile.c_str());
				if ((lSizeNew >= 8) && (lSizeNew < lSize))
				{
					if (!clsUtil::CopyFile(sTmpOutputFile.c_str(), sInputFile.c_str()))
					{
						iError = -9998;
					}
				}
			}
			else if ((PosEx("%OUTPUTFILE%", psCommandLine) == 0) && (PosEx("%TMPOUTPUTFILE%", psCommandLine) == 0))
			{
				lSizeNew = clsUtil::SizeFile(sTmpInputFile.c_str());
				if ((lSizeNew >= 8) && (lSizeNew < lSize))
				{
					clsUtil::CopyFile(sTmpInputFile.c_str(), sInputFile.c_str());
					if (!clsUtil::CopyFile(sTmpInputFile.c_str(), sInputFile.c_str()))
					{
						iError = -9997;
					}
					//sInputFile = sTmpOutputFile;
				}
			}
		}
    }
	else
	{
		iError = -9999;
	}

	if (!gudtOptions.bDebug)
	{
		clsUtil::DeleteFile(sTmpInputFile.c_str());
		clsUtil::DeleteFile(sTmpOutputFile.c_str());
	}

	if ((lSizeNew <= 8) || (lSizeNew >= lSize))
	{
		lSizeNew = lSize;
	}
	else
	{
		grdFiles->Cells[KI_GRID_OPTIMIZED][(int) piCurrent] = FormatNumberThousand(lSizeNew);
	}

	clsUtil::Log(3, ("Start: " + dteStart.FormatString("yyyy/mm/dd hh:nn:ss") + "\tEnd: " + dteEnd.FormatString("yyyy/mm/dd hh:nn:ss") + "\tLevel: " + ((String) gudtOptions.iLevel) +  "\tOriginal: " + ((String) lSize) + "\tOptimized: " + ((String) lSizeNew) + "\tErrorlevel: " + ((String) iError) + "\tInput: " + sInputFile + "\tOutput: " + sOutputFile + "\tPlugin: " + psStatus + "\tCommandline: " + sCommandLine + "").c_str(), gudtOptions.iLogLevel);


	return (iError);
}



// ---------------------------------------------------------------------------
void __fastcall TfrmMain::CheckForUpdates(bool pbSilent)
{
	TCHAR *acWide = new TCHAR[KI_BUFFER_SIZE];
	TCHAR *acTemp = new TCHAR[KI_BUFFER_SIZE];
	char *acPath = new char[KI_BUFFER_SIZE];
	char *acBuffer = new char[KI_BUFFER_SIZE];
	unsigned int iSize;


	strcpy(acPath, "ini=");
	memset(acBuffer, 0, KI_BUFFER_SIZE);
	iSize = KI_BUFFER_SIZE - 1024;
	clsUtil::ReadFile(clsUtil::GetIniPath(), (void *) acBuffer, &iSize);
    //Remove cache part
	char *pcCache = strstr(acBuffer, "[Cache]");
	if (pcCache)
	{
		*pcCache = NULL;
	}

    //ToDo: Use UrlEscape https://msdn.microsoft.com/en-us/library/windows/desktop/bb773774(v=vs.85).aspx
	strcpy(acBuffer, ((AnsiString) ReplaceStr(ReplaceStr(ReplaceStr(acBuffer, "\r\n", "%0D"), "=", "%3D"), "\t", "")).c_str());
	strcat(acPath, acBuffer);
	//strcat(acPath, "&log=");
	//iSize = 0;
	//clsUtil::ReadFile(clsUtil::GetLogPath(), (void *) acBuffer, &iSize);
	//strcat(acPath, acBuffer);

	//mbstowcs(acWide, acPath, KI_BUFFER_SIZE);
	//if (clsUtil::DownloadFile(acWide, acBuffer, KI_BUFFER_SIZE))
	if (clsUtil::DownloadFilePost(KS_APP_UPDATE_SERVER, KS_APP_UPDATE_PAGE, acPath, acBuffer, KI_BUFFER_SIZE, StartsStr("https://", KS_APP_URL)))
	{
		mbstowcs(acWide, acBuffer, KI_BUFFER_SIZE);
		//Check we only got number and punctuation digits in first characters to detect router errors returning HTML
		size_t iWideLen = min(_tcslen(acWide), 10);
		for (unsigned int iWide = 0; iWide < iWideLen; iWide++)
		{
			if ((!_istdigit(acWide[iWide])) && (!_istpunct(acWide[iWide]) && (acWide[iWide] != ' ')))
			{
				if (!pbSilent)
				{
					clsUtil::MsgBox(Handle, _((TCHAR *) _T("Error checking for updates.")), _((TCHAR *) _T("Check updates")), MB_OK | MB_ICONERROR);
				}
				delete[] acTemp;
				delete[] acWide;
				delete[] acPath;
				delete[] acBuffer;
				return;
			}
		}

		gudtOptions.iAdsShown = 0;

		if (GetModuleFileName(NULL, (TCHAR *) acPath, KI_BUFFER_SIZE) != 0)
		{
			_tcsncpy(acTemp, clsUtil::ExeVersion((TCHAR *) acPath), KI_BUFFER_SIZE - 1);
		}
		_stprintf((TCHAR *) acBuffer, _T("%10s"), (TCHAR *) acTemp);

		_tcsncpy((TCHAR *) acTemp, acWide, KI_BUFFER_SIZE - 1);
		_stprintf(acWide, _T("%10s"), (TCHAR *) acTemp);

		if (_tcsstr(acTemp, _T(" (")) != NULL)
		{
			_tcsncpy(gudtOptions.acDonation, _tcsstr(acTemp, _T(" (")) + 2, (sizeof(gudtOptions.acDonation) / sizeof(TCHAR)) - 1);
			StrTrim(gudtOptions.acDonation, _T(" ()"));
		}
		else
		{
			gudtOptions.acDonator[0] = NULL;
            gudtOptions.acDonation[0] = NULL;
            if ((gudtOptions.iStatOpens > 10) && (clsUtil::Random(0, 50) == 5))
			{
				String sCaption;
				sCaption.printf(_((TCHAR *) _T("Thank you for using my program!\n\nYou have used it %s times and have optimized %s files.\n\nYou can continue using it free of charge.\n\nIf you are happy, please contribute to the active development by donating via Paypal. It is secure, safe and convenient.\n\nDonators will receive priority support and consultancy, while those cannot be guaranteed to non-donors.")), FormatNumberThousand(gudtOptions.iStatOpens).c_str(), FormatNumberThousand(gudtOptions.iStatFiles).c_str());
				clsUtil::MsgBox(Handle, sCaption.c_str(), (_("Thank you for using ") + Application->Name).c_str(), MB_OK|MB_ICONEXCLAMATION, 60000);
				actDonateExecute(NULL);
			}
		}

		if (_tcsncmp(acWide, (TCHAR *) acBuffer, 10) > 0)
		{
			String sCaption;
			sCaption.printf(_((TCHAR *) _T("%s version %s is available.\r\nDo you want to download it now?")), Application->Name.c_str(), Trim(acWide).c_str());
			if (clsUtil::MsgBox(Handle, sCaption.c_str(), _((TCHAR *) _T("Check updates")), MB_YESNO | MB_ICONQUESTION) == ID_YES)
			{
				ShellExecute(Handle, _T("open"), KS_APP_UPDATE_URL, _T(""), _T(""), SW_SHOWNORMAL);
			}
		}
		else if (!pbSilent)
		{
			String sCaption;
			sCaption.printf(_((TCHAR *) _T("You already have latest %s? version.")), Application->Name.c_str());
			clsUtil::MsgBox(Handle, sCaption.c_str(), _((TCHAR *) _T("Check updates")), MB_OK|MB_ICONINFORMATION);
		}
	}
	else if (!pbSilent)
	{
		clsUtil::MsgBox(Handle, _((TCHAR *) _T("Error checking for updates.")), _((TCHAR *) _T("Check updates")), MB_OK | MB_ICONERROR);
	}
	delete[] acTemp;
	delete[] acWide;
	delete[] acPath;
	delete[] acBuffer;
}




//---------------------------------------------------------------------------
String __fastcall TfrmMain::GetExtensionByContent (const String psFilename, bool pbForce)
{
	String sRes;
	unsigned char acBuffer[512 * 2];


	sRes = ExtractFileExt(psFilename).LowerCase();
	//Fixe ExtractFileExt returning NULL when no extension instead of empty.
	if (sRes.IsEmpty())
	{
		sRes = ".";
	}


	//If file extension is not known, get it by analyzing file contents
	if ((PosEx(" " + sRes + " ", KS_EXTENSION_ALL + ReplaceStr((String) gudtOptions.acJSAdditionalExtensions, ";", " ") + " ") == 0) || (pbForce))
	{
		unsigned int iSize;
		memset(acBuffer, 0, sizeof(acBuffer));

		//Read footer
		iSize = sizeof(acBuffer) >> 1;
		clsUtil::ReadFile(psFilename.c_str(), &acBuffer[512], &iSize, (unsigned int) clsUtil::SizeFile(psFilename.c_str()) - 512);
		
		//Read header
		iSize = sizeof(acBuffer) >> 1;
		if (clsUtil::ReadFile(psFilename.c_str(), acBuffer, &iSize))
		{
			//ToDo: Optimize to use regular comparisons instead of memcmp for short comparisons.
			//Check AVIF
			if (memcmp(&acBuffer[4], "ftypavif", 8) == 0)
			{
				sRes = ".avif";
			}
			//Check BMP
			else if ((memcmp(acBuffer, "BM", 2) == 0) || (memcmp(acBuffer, "BA", 2) == 0) || (memcmp(acBuffer, "CI", 2) == 0) || (memcmp(acBuffer, "CP", 2) == 0) || (memcmp(acBuffer, "IC", 2) == 0) || (memcmp(acBuffer, "PT", 2) == 0))
			{
				sRes = ".bmp";
			}
			//Check EXE
			//Check DLL
			else if ((memcmp(acBuffer, "MZ", 2) == 0) || (memcmp(acBuffer, "ZM", 2) == 0))
			{
				sRes = ".dll";
			}
			//Check FLAC
			else if (memcmp(acBuffer, "fLaC", 4) == 0)
			{
				sRes = ".flac";
			}
			//Check GIF
			else if (memcmp(acBuffer, "GIF8", 4) == 0)
			{
				sRes = ".gif";
			}
			//Check GZ
			else if (memcmp(acBuffer, "\x1F\x8B\x08", 3) == 0)
			{
				sRes = ".gz";
			}
			//Check ICO
			else if (memcmp(acBuffer, "\x00\x00\x01\x00", 4) == 0)
			{
				sRes = ".ico";
			}
			//Check JPEG 2000
			else if (memcmp(&acBuffer[4], "jP", 2) == 0)
			{
				sRes = ".jp2";
			}
			//Check JPEG
			else if (memcmp(acBuffer, "\xFF\xD8\xFF", 3) == 0)
			{
				sRes = ".jpg";
			}
			//Check JPEG XL
			else if (memcmp(acBuffer, "\x00\x00\x00\x0C\x4A\x58\x4C\x20\x0D\x0A\x87\x0A", 12) == 0)
			{
				sRes = ".jxl";
			}
			//Check MKV
			else if (memcmp(acBuffer, ".RTS", 4) == 0)
			{
				sRes = ".mkv";
			}
			//ToDo: MIME
			//Check MNG
			else if (memcmp(acBuffer, "\x8A\x4D\x4E\x47\x0D\x0A\x1A\x0A", 8) == 0)
			{
				sRes = ".mng";
			}
			//Check MP3
			else if (memcmp(acBuffer, "ID3", 3) == 0)
			{
				sRes = ".mp3";
			}
			//Check MP4
			else if (memcmp(&acBuffer[3], "ftyp", 4) == 0)
			{
				sRes = ".mp4";
			}
			//Check OBJ
			else if ((memcmp(acBuffer, "\x4C\x01", 2) == 0) ||
				(acBuffer[0] == 0x80))
			{
				sRes = ".obj";
			}
			//Check OGG / Check OGV
			else if (memcmp(acBuffer, "OggS", 4) == 0)
			{
				sRes = ".ogg";
			}
			//Check OLE/OLE Beta
			else if ((memcmp(acBuffer, "\xD0\xCF\x11\xE0\xA1\xB1\x1A\xE1", 8) == 0) ||
					(memcmp(acBuffer, "\x0E\x11\xFC\x0D\xD0\xCF\x11\x0E", 8) == 0))
			{
				sRes = ".ole";
			}
			//Check PCX
			else if ((acBuffer[0] == 10) && (acBuffer[2] == 1) && (acBuffer[64] == 0) && (acBuffer[74] == 0))
			{
				sRes = ".pcx";
			}
			//Check PDF
			else if (memcmp(acBuffer, "%PDF-", 5) == 0)
			{
				sRes = ".pdf";
			}
			//Check PNG
			else if (memcmp(acBuffer, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", 8) == 0)
			{
				sRes = ".png";
			}
			//Check SQLITE
			else if (memcmp(acBuffer, "SQLite format 3", 16) == 0)
			{
				sRes = ".sqlite";
			}
			//Check SWF
			else if ((memcmp(acBuffer, "FWS", 3) == 0) || (memcmp(acBuffer, "CWS", 3) == 0) || (memcmp(acBuffer, "ZWS", 3) == 0))
			{
				sRes = ".swf";
			}
			//Check TAR
			else if (memcmp(&acBuffer[257], "\x75\x73\x74\x61\x72", 5) == 0)
			{
				sRes = ".tar";
			}
			//Check TGA
			else if (memcmp(&acBuffer[512 * 2 - 18], "TRUEVISION", 10) == 0)
			{
				sRes = ".tga";
			}
			//Check TIFF
			else if ((memcmp(acBuffer, "\x0C\xED", 2) == 0) || (memcmp(acBuffer, "\x49\x20\x49", 3) == 0) || (memcmp(acBuffer, "\x49\x49\x2A\x00", 4) == 0) || (memcmp(acBuffer, "\x4D\x4D\x00\x2B", 4) == 0))
			{
				sRes = ".tif";
			}
			//ToDo: Tencent QQ
			//Check WAV
			else if (memcmp(acBuffer, "RIFF", 4) == 0)
			{
				sRes = ".wav";
			}			
			//Check WEBP
			else if (memcmp(&acBuffer[7], "WEBP", 4) == 0)
			{
				sRes = ".webp";
			}
			//Check ZIP
			else if (memcmp(acBuffer, "\x50\x4B\x03\x04", 4) == 0)
			{
				sRes = ".zip";
			}
			//Check CAB
			else if (memcmp(acBuffer, "MSCF", 4) == 0)
			{
				sRes = ".cab";
			}			
			//Check 7z
			else if (memcmp(acBuffer, "\x37\x7A\xBC\xAF\x27\x1C", 6) == 0)
			{
				sRes = ".7z";
			}
			//Unsupported extension
			else
			{
				sRes = "";
			}
		}
		else
		{
			sRes = "";
		}
	}
	else
	{
		//Do nothing. Use regular file extension
	}
	return (sRes);
}




//---------------------------------------------------------------------------
String __inline TfrmMain::FormatNumberThousand (unsigned long long plNumber)
{
	//return (FloatToStrF(plNumber, (TFloatFormat) ffNumber, 18, 0));
	TCHAR acNumber[64];
	TCHAR acFormatedNumber[64];
	TCHAR *pcDecimal;


	_ui64tot(plNumber, acNumber, 10);
	GetNumberFormat(LOCALE_USER_DEFAULT, NULL, acNumber, NULL, acFormatedNumber, (sizeof(acFormatedNumber) / sizeof(TCHAR)) - 1);
	//Remove Decimals
	pcDecimal = _tcsrchr(acFormatedNumber, FormatSettings.DecimalSeparator);
	if (pcDecimal)
	{
		*pcDecimal = NULL;
	}
	return ((String) acFormatedNumber);
}


//---------------------------------------------------------------------------
String __inline TfrmMain::FormatNumberThousandUnit (unsigned long long plNumber)
{
	TCHAR acRes[64];


	StrFormatByteSize64(plNumber, acRes, (sizeof(acRes) / sizeof(TCHAR)) - 1);
	return (String(acRes));
}




//---------------------------------------------------------------------------
unsigned long long __inline TfrmMain::ParseNumberThousand (String psNumber)
{
	//return (StrToInt64Def(psNumber, FormatSettings.ThousandSeparator, ""), 0));
	TCHAR *acNumber, acRes[64];


	acNumber = psNumber.c_str();
	size_t iNumberLen = _tcslen(acNumber);

	unsigned int iResPos = 0;
	for (unsigned int iCount = 0; iCount < iNumberLen; iCount++)
	{
		//If it is a digit, we add it to the result
		if (_istdigit(acNumber[iCount]))
		{
			acRes[iResPos] = acNumber[iCount];
			iResPos++;
		}
	}
	acRes[iResPos] = NULL;
	return ((unsigned long long) _ttoi64(acRes));
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
unsigned long __fastcall TfrmMain::RunProcess(const TCHAR *pacProcess, TCHAR *pacOutput, unsigned int piOutputLen, bool pbWait)
{
	unsigned long lExitCode;
	STARTUPINFO udtSI = {};
	PROCESS_INFORMATION udtPI = {};
	SECURITY_ATTRIBUTES udtSA;
	HANDLE hRead = NULL, hWrite = NULL;


	if (gbStop)
	{
		return (0);
	}

	Screen->Cursor = crAppStart;

	//Disable Windows error handling. Do not use GetErrorMode because it requires Vista or later.
	SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOALIGNMENTFAULTEXCEPT | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

	if ((pacOutput != NULL) && (piOutputLen > 0))
	{
		udtSA.nLength = sizeof(udtSA);
		udtSA.bInheritHandle = true;
		udtSA.lpSecurityDescriptor = NULL;
		CreatePipe(&hRead, &hWrite, &udtSA, 0);

		udtSI.cb = sizeof(udtSI);
		udtSI.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		udtSI.hStdInput = hRead;
		udtSI.hStdOutput = hWrite;
		udtSI.wShowWindow = SW_HIDE;

		if (!CreateProcess(NULL, (TCHAR *) pacProcess, &udtSA, &udtSA, false, (unsigned long) gudtOptions.iProcessPriority, NULL, NULL /*(TCHAR *) pacDirectory*/, &udtSI, &udtPI))
		{
			//_stprintf(acTmp, _T("Cannot launch %s"), pacProcess);
		}
	}
	else
	{
		udtSA.nLength = sizeof(udtSA);
		udtSA.bInheritHandle = true;
		udtSA.lpSecurityDescriptor = NULL;

		udtSI.cb = sizeof(udtSI);
		udtSI.dwFlags = STARTF_USESHOWWINDOW;
		udtSI.wShowWindow = SW_HIDE;

		if (!CreateProcess(NULL, (TCHAR *) pacProcess, &udtSA, &udtSA, false, (unsigned long) gudtOptions.iProcessPriority, NULL, NULL /*(TCHAR *) pacDirectory*/, &udtSI, &udtPI))
		{
			//_stprintf(acTmp, _T("Cannot launch %s"), pacProcess);
			//MsgBox(NULL, acTmp, _T("Error"), MB_OK | MB_ICONASTERISK);
		}
	}

	if (pbWait)
	{
		do
		{
			Application->ProcessMessages();
			if (gbStop)
			{
				TerminateProcess(udtPI.hProcess, 0);
			}
			if (GetExitCodeProcess(udtPI.hProcess, &lExitCode))
			{
				if ((lExitCode == STILL_ACTIVE) && (WaitForSingleObject(udtPI.hProcess, 100) == WAIT_OBJECT_0))
				{
					lExitCode = 0;
				}
			}
		}
		while (lExitCode == STILL_ACTIVE);
	}
	else
	{
		GetExitCodeProcess(udtPI.hProcess, &lExitCode);
	}

	if ((pacOutput != NULL) && (piOutputLen > 0))
	{
		memset(pacOutput, 0, piOutputLen);
		unsigned long lRead;
		PeekNamedPipe(hRead, NULL, NULL, NULL, &lRead, NULL);
		if (lRead > 0)
		{
			::ReadFile(hRead, pacOutput, piOutputLen, &lRead, NULL);
		}
		CloseHandle(hRead);
		CloseHandle(hWrite);
	}

	CloseHandle(udtPI.hProcess);
	CloseHandle(udtPI.hThread);
	
	//Restore Windows error handling
	SetErrorMode(0);

	Screen->Cursor = crDefault;

	return (lExitCode);
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool __fastcall TfrmMain::IsAPNG(const TCHAR *pacFile)
{
	#pragma pack(push, 1)
	unsigned char *acBuffer;
	struct
	{
		unsigned char acSignature[8];
	} udtPNGHeader;
	struct
	{
		unsigned int iLength;
		unsigned char acType[4];
	} udtPNGChunk;
	#pragma pack(pop)


	bool bRes = false;
	unsigned int iSize = (unsigned int) clsUtil::SizeFile(pacFile);
	if (iSize > 0)
	{
		acBuffer = new unsigned char[iSize];
		if (acBuffer)
		{
			clsUtil::ReadFile(pacFile, acBuffer, &iSize);
			unsigned int iPos = 0;
			memcpy(&udtPNGHeader, &acBuffer[iPos], sizeof(udtPNGHeader));
			iPos += sizeof(udtPNGHeader);
			if (memcmp(udtPNGHeader.acSignature, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", sizeof(udtPNGHeader.acSignature)) == 0)
			{
				do
				{
					memcpy(&udtPNGChunk, &acBuffer[iPos], sizeof(udtPNGChunk));
					iPos += sizeof(udtPNGChunk);
					udtPNGChunk.iLength = ((udtPNGChunk.iLength >> 24) & 0xFF) | ((udtPNGChunk.iLength << 8) & 0xFF0000) | ((udtPNGChunk.iLength >> 8) & 0xFF00) | ((udtPNGChunk.iLength << 24) & 0xFF000000);
					if ((memcmp(udtPNGChunk.acType, "fcTL", sizeof(udtPNGChunk.acType)) == 0) || (memcmp(udtPNGChunk.acType, "acTL", sizeof(udtPNGChunk.acType)) == 0) || (memcmp(udtPNGChunk.acType, "fdAT", sizeof(udtPNGChunk.acType)) == 0))
					{
						bRes = true;
						break;
					}
					else
					{
						iPos += udtPNGChunk.iLength + 4;
					}
				}
				while (iPos < iSize);
			}
			delete[] acBuffer;
		}
	}
	return (bRes);
}


// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool __fastcall TfrmMain::IsEXESFX(const TCHAR *pacFile)
{
	bool bRes = false;
	unsigned char *acBuffer;

	
	unsigned int iSize = 1 * 1024 * 1024;
	acBuffer = new unsigned char[iSize];
	if (acBuffer)
	{
		clsUtil::ReadFile(pacFile, acBuffer, &iSize);
		
		//Check if it is EXE
		if ((memcmp(acBuffer, "MZ", 2) == 0) || (memcmp(acBuffer, "ZM", 2) == 0))
		{
			//Check if it is an Inno Setup Installer
			if (clsUtil::MemMem((const void *) acBuffer, iSize, (const void *) "Inno Setup", 10) != NULL)
			{
				bRes = true;
			}
			//Check if it is an InstallShield Wizard
			else if (clsUtil::MemMem((const void *) acBuffer, iSize, (const void *) "InstallShield", 13) != NULL)
			{
				bRes = true;
			}
			//Check if it is an NSIS
			else if (clsUtil::MemMem((const void *) acBuffer, iSize, (const void *) "Nullsoft Install System", 23) != NULL)
			{
				bRes = true;
			}
			//Check if it is an RTPatch Updater
			else if (clsUtil::MemMem((const void *) acBuffer, iSize, (const void *) "RTPatch", 7) != NULL)
			{
				bRes = true;
			}
			//Check if it is a RAR SFX
			else if (clsUtil::MemMem((const void *) acBuffer, iSize, (const void *) "\x52\x61\x72\x21\x1A\x07", 6) != NULL)
			{
				bRes = true;
			}
			//Check if it is a ZIP SFX
			else if (clsUtil::MemMem((const void *) acBuffer, iSize, (const void *) "\x50\x4B\x03\x04", 4) != NULL)
			{
				bRes = true;
			}
			//Check if it is a 7-ZIP SFX
			else if (clsUtil::MemMem((const void *) acBuffer, iSize, (const void *) "\x37\x7A\xBC\xAF\x27\x1C", 6) != NULL)
			{
				bRes = true;
			}
			//Check if it is a SFXCAB
			else if (clsUtil::MemMem((const void *) acBuffer, iSize, (const void *) "MSCF", 4) != NULL)
			{
				bRes = true;
			}
		}
		delete[] acBuffer;
	}
	return (bRes);
}


// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool __fastcall TfrmMain::IsEXEManagedNet(const TCHAR *pacFile)
{
	bool bRes = false; //variable that indicates if managed or not.
	HANDLE hFile = CreateFile(pacFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		//succeeded
		HANDLE hOpenFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if (hOpenFileMapping)
		{
			//Map the file , so it can be simply be acted on as a contiguous array of unsigned chars
			unsigned char* pacBaseAddress = (unsigned char*) MapViewOfFile(hOpenFileMapping, FILE_MAP_READ, 0, 0, 0);
			if (pacBaseAddress)
			{
				//having mapped the executable ,now start navigating through the sections
				//DOS header is straightforward. It is the topmost structure in the PE file
				//i.e. the one at the lowest offset into the file
				IMAGE_DOS_HEADER* udtDOSHeader = (IMAGE_DOS_HEADER*) pacBaseAddress;
				//the only important data in the DOS header is the e_lfanew
				//the e_lfanew points to the offset of the beginning of NT Headers data
				IMAGE_NT_HEADERS* udtNTHeaders = (IMAGE_NT_HEADERS*) ((unsigned char*) udtDOSHeader + udtDOSHeader->e_lfanew);
				//store the section header for future use. This will later be need to check to see if metadata lies within
				//the area as indicated by the section headers
				IMAGE_SECTION_HEADER* udtSectionHeader = (IMAGE_SECTION_HEADER*) ((unsigned char*) udtNTHeaders + sizeof(IMAGE_NT_HEADERS));
				//Now, start parsing
				//check if it is a PE file first of all. All assemblies are PE files
				if (udtNTHeaders->Signature == IMAGE_NT_SIGNATURE)
				{
					//start parsing COM table ( this is what points to the metadata and other information )
					unsigned int iNETHeaderTableLocation = udtNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress;
					if (iNETHeaderTableLocation)
					{
						//.Net header data does exist for this module, find it's location in one of the sections
						unsigned int iActualAddressFromRVA = 0;
						unsigned int iSections = udtNTHeaders->FileHeader.NumberOfSections;
						for (unsigned int iSection = 0; iSection < iSections; iSection++, udtSectionHeader++)
						{
							unsigned int iMaxOnDisk = min(udtSectionHeader->Misc.VirtualSize, udtSectionHeader->SizeOfRawData);
							unsigned int iStartSectRVA = udtSectionHeader->VirtualAddress;
							unsigned int iEndSectRVA = iStartSectRVA + iMaxOnDisk;
							if ((iNETHeaderTableLocation >= iStartSectRVA) && (iNETHeaderTableLocation < iEndSectRVA))
							{
								iActualAddressFromRVA = (udtSectionHeader->PointerToRawData) + (iNETHeaderTableLocation - iStartSectRVA);
								break;
							}
						}
						IMAGE_COR20_HEADER *udtNETHeader = (IMAGE_COR20_HEADER*) ((unsigned char*) udtDOSHeader + iActualAddressFromRVA);
						if (udtNETHeader)
						{
							//valid address obtained. Suffices to say , this is good enough to identify this as a
							//valid managed component
							bRes = true;
						}
					}
				}
				//cleanup
				UnmapViewOfFile(pacBaseAddress);
			}
			//cleanup
			CloseHandle(hOpenFileMapping);
		}
		//cleanup
		CloseHandle(hFile);
	}
	return (bRes);
}


// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool __fastcall TfrmMain::IsPDFLayered(const TCHAR *pacFile)
{
	bool bRes = false;
	unsigned char *acBuffer;

	
	unsigned int iSize = 512 * 1024;
	acBuffer = new unsigned char[iSize];
	if (acBuffer)
	{
		clsUtil::ReadFile(pacFile, acBuffer, &iSize);
		//Look for a OCG (Optional Content Groups)
		bRes = (clsUtil::MemMem(acBuffer, iSize, "<< /Type /OCG /Name", 20) != NULL);
		delete[] acBuffer;
	}
	return (bRes);
}


// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool __fastcall TfrmMain::IsJPEGCMYK(const TCHAR *pacFile)
{
	bool bRes = false;
	unsigned char *acBuffer;

	
	unsigned int iSize = 512 * 1024;
	acBuffer = new unsigned char[iSize];
	if (acBuffer)
	{
		clsUtil::ReadFile(pacFile, acBuffer, &iSize);
		//Look for SOF (Start Of Frame - SOF0 or SOF2) section in header
		unsigned char *pSOF = (unsigned char *) clsUtil::MemMem(acBuffer, iSize, "\xFF\xC0", 2);
		if (pSOF)
		{
			// FF C2 is progressive encoding while FF C0 is standard encoding
			pSOF = (unsigned char *) clsUtil::MemMem(pSOF, iSize - (pSOF - acBuffer), "\xFF\xC2", 2);
			if (pSOF)
			{
                bRes = (pSOF[9] == 4);
			}
		}
		delete[] acBuffer;
	}
	return (bRes);
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void __fastcall TfrmMain::UpdateAds(void)
{
	unsigned long lResultFlags;


	//Ads require internet connection, and Internet Explorer 9 or later, so Vista or newer
	//Show ads
	if ((!gudtOptions.bHideAds) && (InternetGetConnectedState(&lResultFlags, 0)))
	{
		//If was hidden, show it
		if (webAds->Height == 0)
		{
			webAds->Offline = false;
			webAds->Height = 90;
			webAds->Show();
		}

		#if defined (_DEBUG)
			String sUrl = (String) KS_APP_ADS_URL + "?w=" + webAds->Width + "&h=" + webAds->Height + "&d=1&q=" + LeftStr(grdFiles->Cols[KI_GRID_FILE]->CommaText, 512);
		#else
			String sUrl = (String) KS_APP_ADS_URL + "?w=" + webAds->Width + "&h=" + webAds->Height + "&d=0&q=" + LeftStr(grdFiles->Cols[KI_GRID_FILE]->CommaText, 512);
		#endif
		//Hack to prevent IE memory leak due to store all previous visited pages
		if (sUrl == webAds->LocationURL)
		{
			webAds->Refresh();
		}
		else
		{
			if (webAds->ActiveEngine == TWebBrowser::TActiveEngine::IE)
			{
				OleVariant oFlags = Shdocvw::navNoHistory | Shdocvw::navNoReadFromCache | Shdocvw::navNoWriteToCache | Shdocvw::navNewWindowsManaged | Shdocvw::navTrustedForActiveX;
				webAds->Navigate(sUrl, oFlags);
			}
			else
			{
				webAds->Navigate(sUrl);
			}
		}
		//Trim memory working set
		SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
	}
	else
	{
		//If was visible, hide it
		if (webAds->Height > 0)
		{
			webAds->Stop();
			webAds->Navigate("about:blank");
			webAds->Hide();
			webAds->Height = 0;
			webAds->Offline = true;
		}
	}
}



//---------------------------------------------------------------------------
#pragma argsused
void __fastcall TfrmMain::webAdsTitleChange(TObject *ASender, const WideString Text)
{
	static String sLastOpenedUrl = "";


	if (!gudtOptions.bHideAds)
	{
		//Finished loading
		if (!webAds->Busy)
		{
			gudtOptions.iAdsShown++;
		}

		//URL moved from ads page
		if (PosEx((String) KS_APP_ADS_URL, webAds->LocationURL) == 0)
		{
			if ((PosEx("http://", webAds->LocationURL) != 0) || (PosEx("https://", webAds->LocationURL) != 0))
			{
				//Prevent double opening
				if (sLastOpenedUrl != webAds->LocationURL)
				{
					sLastOpenedUrl = webAds->LocationURL;
					ShellExecute(Handle, _T("open"), sLastOpenedUrl.c_str(), _T(""), _T(""), SW_SHOWNORMAL);
				}
			}
			UpdateAds();
		}
	}
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void __fastcall TfrmMain::UpdateTheme(void)
{
	//Prevent flickering
	LockWindowUpdate(Handle);

	int iLightTheme = clsUtil::GetRegistryI(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"), _T("AppsUseLightTheme"));
	if (iLightTheme == 0)
	{
		Vcl::Themes::TStyleManager::TrySetStyle("Windows10 Dark", false);
	}
	else
	{
		Vcl::Themes::TStyleManager::TrySetStyle("Windows10", false);
	}

	clsLanguage::Load((unsigned int) gudtOptions.iLanguage, true);
	clsLanguage::TranslateForm(this);

	//Change instructions depending on Recycle Bin settings
	if (gudtOptions.bHideInstructions)
	{
		lblInstructions->Visible = false;
	}
	else
	{
		lblInstructions->Visible = true;
		if (gudtOptions.bDoNotUseRecycleBin)
		{
			lblInstructions->Caption = _("Drag on the list below files you want to optimize, and when ready, click on the right button context menu to proceed. No backups will be created, but you can enable moving to Recycle Bin if you like. Double click an item to preview it.");
		}
		else
		{
			lblInstructions->Caption = _("Drag on the list below files you want to optimize, and when ready, click on the right button context menu to proceed. All processed files are copied to Recycle Bin, so you can easily restore them. You can disable moving to Recycle Bin if you like. Double click an item to preview it.");
		}
		lblInstructions->Hint = lblInstructions->Caption;
	}
	
	if (gudtOptions.bAlwaysOnTop)
	{
		FormStyle = fsStayOnTop;
	}
	else
	{
		FormStyle = fsNormal;
	}

	tooMain->Visible = gudtOptions.bShowToolBar;

	UpdateAds();
	
	//Reenable form updates
	LockWindowUpdate(NULL);
}


//---------------------------------------------------------------------------
void __fastcall TfrmMain::RefreshStatus(bool pbUpdateStatusBar, unsigned int piCurrent, unsigned long long plTotalBytes, unsigned long long plSavedBytes)
{
	//Prevent flickering
	//LockWindowUpdate(Handle);

	//ProcessMessages is required before changing DragAcceptFiles
	if (StrStrI(GetCommandLine(), _T("/NOWINDOW")) != nullptr)
	{
		Hide();
	}


	Application->ProcessMessages();

	if (gbProcess)
	{
		unsigned int iPercentBytes;
		if (plTotalBytes != 0)
		{
			//iPercentBytes = ((unsigned long long) piTotalBytes - piSavedBytes) * 100 / piTotalBytes;
			iPercentBytes = ((unsigned int) ((double) (plTotalBytes - plSavedBytes) / plTotalBytes * 100));

		}
		else
		{
			iPercentBytes = 0;
		}

		//Required indirection
		String sCaption;
		sCaption.printf(_((TCHAR *) _T("%s / %s files. %s bytes saved (%s%%)")), FormatNumberThousand(piCurrent).c_str(), FormatNumberThousand((unsigned long long) grdFiles->RowCount - 1).c_str(), FormatNumberThousand(plSavedBytes).c_str(), FormatNumberThousand(iPercentBytes).c_str());
		stbMain->Panels->Items[0]->Text = sCaption;

		stbMain->Hint = stbMain->Panels->Items[0]->Text;
		Caption = stbMain->Hint + " - " + Application->Name;
		Application->Title = Caption;

		actOpen->Enabled = false;
		actOpenFolder->Enabled = false;
		actOptimize->Enabled = false;
		actStop->Enabled = true;
		actAdd->Enabled = false;
		actAddFolder->Enabled = false;
		actClear->Enabled = false;
		actRemove->Enabled = false;

		// Prevent grid sorting while optimizing
		grdFiles->Options = grdFiles->Options >> goFixedHotTrack >> goFixedRowClick >> goFixedColClick;
		DragAcceptFiles(Handle, false);
	}
	else
	{
		actOpen->Enabled = (grdFiles->RowCount > 1);
		actOpenFolder->Enabled = (grdFiles->RowCount > 1);
		actOptimize->Enabled = (grdFiles->RowCount > 1);
		actStop->Enabled = false;
		actAdd->Enabled = true;
		actAddFolder->Enabled = true;
		actClear->Enabled = (grdFiles->RowCount > 1);
		actRemove->Enabled = (grdFiles->RowCount > 1);

		grdFiles->Options = grdFiles->Options << goFixedHotTrack << goFixedRowClick << goFixedColClick;
		DragAcceptFiles(Handle, true);

		if (pbUpdateStatusBar)
		{
			if (grdFiles->RowCount > 1)
			{
				//Required indirection
				String sCaption;
				sCaption.printf(_((TCHAR *) _T("0 / %s files")), FormatNumberThousand((unsigned long long) grdFiles->RowCount - 1).c_str());
				stbMain->Panels->Items[0]->Text = sCaption;

				stbMain->Hint = stbMain->Panels->Items[0]->Text;
				Caption = stbMain->Hint + " - " + Application->Name;
				Application->Title = Caption;
				grdFiles->FixedRows = 1;
				pgbProgress->Position = 0;
				pgbProgress->Max = grdFiles->RowCount - 1;
				clsUtil::SetTaskListProgress((unsigned int) pgbProgress->Position, (unsigned int) pgbProgress->Max);
			}
			else
			{
				grdFiles->Rows[0]->BeginUpdate();
				grdFiles->ColCount = 5;
				TStringList *lstRow = new TStringList();
				lstRow->Add(_("File"));
				lstRow->Add(_("Extension (Type)"));
				lstRow->Add(_("Original size"));
				lstRow->Add(_("Optimized size"));
				lstRow->Add(_("Status"));
				grdFiles->Rows[0] = lstRow;
				delete lstRow;
				grdFiles->Rows[0]->EndUpdate();

				stbMain->Panels->Items[0]->Text = "";
				stbMain->Hint = stbMain->Panels->Items[0]->Text;
				Caption = Application->Name;
				Application->Title = Caption;
				pgbProgress->Position = 0;
				clsUtil::SetTaskListProgress((unsigned int) pgbProgress->Position, (unsigned int) pgbProgress->Max);
			}
		}
		else
		{
			//Caption = Application->Name;
			//Application->Title = Caption;
		}
	}

	//Reenable form updates
	//LockWindowUpdate(NULL);
}


//---------------------------------------------------------------------------
String __fastcall TfrmMain::Hash(const String psFilename)
{
	unsigned long long lSize;
	FILETIME udtFileCreated, udtFileAccessed, udtFileModified;	
	String sHash;
	

	lSize = clsUtil::SizeFile(psFilename.c_str());
	clsUtil::GetFileTimestamp(psFilename.c_str(), &udtFileCreated, &udtFileAccessed, &udtFileModified);
	sHash = psFilename + _T("|") + (String) lSize + _T("|") + (String) udtFileModified.dwHighDateTime + (String) udtFileModified.dwLowDateTime;
	return (sHash);
}


//---------------------------------------------------------------------------
String __inline TfrmMain::GetCellValue(String psValue, unsigned int piPos)
{
	//Decode the information in cell separating the value to show, with the value to parse
	TStringDynArray asValue = SplitString(psValue, "\r");
	if ((unsigned int) asValue.Length > piPos)
	{
		psValue = asValue[piPos];
	}
	return (psValue);
}



//---------------------------------------------------------------------------
String __inline TfrmMain::SetCellFileValue(const String psValue)
{
	String sRes = "";


	//only filename
	if (gudtOptions.iFilenameFormat == 1)
	{
		sRes = ExtractFileName(psValue);
	}
	//ToDo: driveletter + ?:\?+partial path + filename
	else if (gudtOptions.iFilenameFormat == 2)
	{
		sRes = psValue;
	}
	//ToDo: driveletter + ?:\?+partial path if fits+?...? last part of filename
	else if (gudtOptions.iFilenameFormat == 3)
	{
		sRes = psValue;
	}
	//full path+filename
	else
	{
		sRes = psValue;
	}
	sRes += "\r" + psValue;
	return (sRes);
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const TCHAR * __fastcall TfrmMain::GetOptionCommandLine(void)
{
	static TCHAR acCommandLine[PATH_MAX + 1] = _T("");


	// Check if we already have it cached
	if (acCommandLine[0] == NULL)
	{
		_tcsncpy(acCommandLine, GetCommandLine(), (sizeof(acCommandLine) / sizeof(TCHAR)) - 1);
	}
	return (acCommandLine);
}


// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const TCHAR * __fastcall TfrmMain::GetOptionArgument(const TCHAR *pacKey)
{
	TCHAR acArgument[PATH_MAX + 1];
	static TCHAR acRes[PATH_MAX + 1];
	TCHAR *pcStart;


	memset(acRes, 0, sizeof(acRes));
	acArgument[0] = '/';
	_tcsncpy(&acArgument[1], pacKey, (sizeof(acArgument) / sizeof(TCHAR)) - 2);
	pcStart = StrStrI(GetOptionCommandLine(), acArgument);
	if (pcStart)
	{
		TCHAR *pcEnd = _tcsstr(pcStart + _tcslen(pacKey) + 2, _T(" "));
		if (pcEnd == NULL)
		{
			pcEnd = pcStart + _tcslen(GetOptionCommandLine());
		}
		_tcsncpy(acRes, pcStart + _tcslen(pacKey) + 2, (size_t) (pcEnd - pcStart) - _tcslen(pacKey) - 2);
	}
	return (acRes);
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const TCHAR * __fastcall TfrmMain::GetOption(const TCHAR *pacSection, const TCHAR *pacKey, const TCHAR *pacDefault)
{
	static TCHAR acRes[2048];


	memset(acRes, 0, sizeof(acRes));
	/* Get it via global registry */
	_tcsncpy(acRes, clsUtil::GetRegistry(HKEY_LOCAL_MACHINE, clsUtil::GetRegistryPath(), pacKey), (sizeof(acRes) / sizeof(TCHAR)) - 1);
	if (acRes[0] == NULL)
	{
		/* Get it via user registry */
		_tcsncpy(acRes, clsUtil::GetRegistry(HKEY_CURRENT_USER, clsUtil::GetRegistryPath(), pacKey), (sizeof(acRes) / sizeof(TCHAR)) - 1);
		if (acRes[0] == NULL)
		{
			/* Get it via command-line */
			_tcsncpy(acRes, GetOptionArgument(pacKey), (sizeof(acRes) / sizeof(TCHAR)) - 1);
			if (acRes[0] == NULL)
			{
				//Get it via INI file
				_tcsncpy(acRes, clsUtil::GetIni(pacSection, pacKey, pacDefault), (sizeof(acRes) / sizeof(TCHAR)) - 1);
			}
		}
	}
	return (acRes);
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int __fastcall TfrmMain::GetOption(const TCHAR *pacSection, const TCHAR *pacKey, int piDefault)
{
	int iRes;
	TCHAR acDefault[2048] = {};


	_tcsncpy(acDefault, GetOption(pacSection, pacKey, _T("")), (sizeof(acDefault) / sizeof(TCHAR)) - 1);
	if (acDefault[0] != NULL)
	{
		iRes = _ttoi(acDefault);
	}
	else
	{
		iRes = piDefault;
	}
	return (iRes);
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
long long __fastcall TfrmMain::GetOption(const TCHAR *pacSection, const TCHAR *pacKey, long long plDefault)
{
	long long lRes;
	TCHAR acDefault[2048] = {};


	_tcsncpy(acDefault, GetOption(pacSection, pacKey, _T("")), (sizeof(acDefault) / sizeof(TCHAR)) - 1);
	if (acDefault[0] != NULL)
	{
		lRes = _ttoi64(acDefault);
	}
	else
	{
		lRes = plDefault;
	}
	return (lRes);
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
double __fastcall TfrmMain::GetOption(const TCHAR *pacSection, const TCHAR *pacKey, double pdDefault)
{
	double dRes;
	TCHAR acDefault[2048] = {};


	_tcsncpy(acDefault, GetOption(pacSection, pacKey, _T("")), (sizeof(acDefault) / sizeof(TCHAR)) - 1);
	if (acDefault[0] != NULL)
	{
		dRes = _ttof(acDefault);
	}
	else
	{
		dRes = pdDefault;
	}
	return (dRes);
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool __fastcall TfrmMain::GetOption(const TCHAR *pacSection, const TCHAR *pacKey, bool pbDefault)
{
	bool bRes;
	TCHAR acDefault[2048] = {};


	_tcsncpy(acDefault, GetOption(pacSection, pacKey, _T("")), (sizeof(acDefault) / sizeof(TCHAR)) - 1);
	if (acDefault[0] != NULL)
	{
		bRes = (_tcsicmp(acDefault, _T("true")) == 0);
	}
	else
	{
		bRes = pbDefault;
	}
	return (bRes);
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------





