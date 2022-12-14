// ---------------------------------------------------------------------------
#pragma once
#ifndef cppMainH
#define cppMainH
#include "FileOptimizerPCH1.h"
#include "cppOptions.h"
#include "cppAbout.h"
#include "clsUtil.h"
#include "clsLanguage.h"
#include <SHDocVw.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <System.Actions.hpp>
#include <System.Classes.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.OleCtrls.hpp>
#include <Vcl.PlatformDefaultStyleActnCtrls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ToolWin.hpp>



// ---------------------------------------------------------------------------
#define KI_BUFFER_SIZE		(128 * 1024)
#define KS_APP_URL				_T("https://nikkhokkho.sourceforge.io/static.php?page=FileOptimizer")
#define KS_APP_UPDATE_SERVER	_T("nikkhokkho.sourceforge.io")
#define KS_APP_UPDATE_PAGE		_T("/images/FileOptimizerVersion.php")
#define KS_APP_DONATE_URL		_T("https://nikkhokkho.sourceforge.io/images/FileOptimizerDonate.html")
#define KS_APP_ADS_URL			_T("https://nikkhokkho.sourceforge.io/images/FileOptimizerAds.php")
#define KS_APP_UPDATE_URL		_T("https://sourceforge.net/projects/nikkhokkho/files/FileOptimizer/")

/* Grid columns */
#define KI_GRID_FILE		0
#define KI_GRID_EXTENSION	1
#define KI_GRID_ORIGINAL	2
#define KI_GRID_OPTIMIZED	3
#define KI_GRID_STATUS		4

#define KS_EXTENSION_AVIF	_T(" .avif .heif .heifs .heic, .heics .avci .avcs .avif .avifs ")
#define KS_EXTENSION_BMP	_T(" .bmp .dib ")
#define KS_EXTENSION_CSS	_T(" .css ")
#define KS_EXTENSION_DLL	_T(" .dll .bpl .cpl .drv .icl .lzl .ocx .rll .sys .vbx ")
#define KS_EXTENSION_EXE	_T(" .exe .scr ")
#define KS_EXTENSION_FLAC	_T(" .flac ")
#define KS_EXTENSION_GIF	_T(" .gif ")
#define KS_EXTENSION_GZ		_T(" .gz .deb .ipk .tgz .svgz .vlt ")
#define KS_EXTENSION_HTML	_T(" .htm .html .mht .mhtml .xhtml .xml .sgml .svg ")
#define KS_EXTENSION_ICO	_T(" .ico .cur .spl ")
#define KS_EXTENSION_JPG	_T(" .jfi .jif .jfif .jng .jpe .jpg .jpeg .thm .mpo .jps ")
#define KS_EXTENSION_JP2	_T(" .jp2 .j2c .j2k ")
#define KS_EXTENSION_JS		_T(" .js .json ")
#define KS_EXTENSION_JXL	_T(" .jxl ")
#define KS_EXTENSION_LUA	_T(" .lua .luac ")
#define KS_EXTENSION_MIME	_T(" .mht .mhtml .eml .emlx .msg .mbx .vcf .vcard ")
#define KS_EXTENSION_MKV	_T(" .mkv .mka .mks .mk3d .webm .weba ")
#define KS_EXTENSION_MNG	_T(" .mng ")
#define KS_EXTENSION_MP3	_T(" .mp3 ")
#define KS_EXTENSION_MP4	_T(" .mp4 .3gp .3gg .3g2 .m4a .m4b .m4p .m4r .m4v .acc .avi .mpg .mpeg .vob .flv .wmv .wma .rv .ra .rm .rmvb .ram .smil .qt .mov ")
#define KS_EXTENSION_OBJ	_T(" .obj .o .lib .a ")
#define KS_EXTENSION_OGG	_T(" .ogg .oga .ogx .ogm .spx .opus ")
#define KS_EXTENSION_OGV	_T(" .ogg .ogv .ogx ")
#define KS_EXTENSION_OLE	_T(" .ac .adp .apr .art .chm .chi .chq .chs .chw .db .doc .dot .fla .fpx .its .lit .max .mce .mdb .mdt .mic .mix .mpd .mpp .mpt .msc .msi .msg .msp .mst .mtw .mzz .ole .ole2 .one .opt .ost .pps .ppt .pub .puz .rfa .rft .rte .rvt .sldasm .sldprt .slddrw .sou .spo stickynotes.snt thumbs.db .vsd .vst .vss .wiz .wps .xl .xla .xlc .xlm .xls .xlw .xsf .xsn ")
#define KS_EXTENSION_PCX	_T(" .dcx .pcc .pcx ")
#define KS_EXTENSION_PDF	_T(" .epdf .pdf .fdf .ai ")
#define KS_EXTENSION_PNG	_T(" .apng .ico .png .pns ")
#define KS_EXTENSION_SQLITE	_T(" .sqlite .sqlite3 .sqlite2 .db ")
#define KS_EXTENSION_SWF	_T(" .swf ")
#define KS_EXTENSION_TAR	_T(" .tar .cbt ")
#define KS_EXTENSION_TGA	_T(" .tga .icb .vda .vst ")
#define KS_EXTENSION_TIFF	_T(" .fax .tif .tiff .ptif .ptiff ")
#define KS_EXTENSION_TENCENTQQ	_T(" .gft .rdb ")
#define KS_EXTENSION_WAV	_T(" .wav ")
#define KS_EXTENSION_WEBP	_T(" .webp ")
#define KS_EXTENSION_XML	_T(" .xml .fb2 .fxg .kml .xsl .xslt .xhtml ")
#define KS_EXTENSION_ZIP	_T(" .air .apk .appx .bar .bsz .cbz .cdr .cdt .csl .des .docm .docx .dotx .dotm .dwf .dwfx .ear .easm .eprt .epub .fxg .gallery .gallerycollection .galleryitem .grs .ink .ipsw .itz .ipa .ita .jar .kmz .ksf .kmz .lxf .mdz .miz .mmip .mvz .mpp .msz .nar .nbk .notebook .ora .odb .odf .odg .odp .ods .odt .oex .osk .oxps .pk3 .ppam .potm .potx .ppsm .ppsx .pptm .pptx .pub .qwk .r2skin .rdb .rmskin .s3z .sldm .sldx .stz .swc .vdx .vtx .vsx .wal .war .wba .wmz .wsz .xap .xlam .xlsb .xlsm .xlsx .xltm .xltx .xnk .xmz .xpi .xps .z01 .z02 .z03 .z04 .z05 .z06 .z07 .z08 .z09 .z10 .zip .zx01 .zx02 .zx03 .zx04 .zx05 .zx05 .zx06 .zx07 .zx08 .zx09 .zx10 .zipx ")
#define KS_EXTENSION_7Z		_T(" .7z .lzma ")	
#define KS_EXTENSION_MISC	_T(" .aai .art .avs .bpg .cin .cmyk .cmyka .dds .dpx .epdf .fax .fits .fpx .gray .hdr .hrz .inline .jpc .jpt .mono .mif .miff .mpc .mpr .msl .mtv .mvg .otb .p7 .palm .pcd .pcds .pcl .pcls .pdb .pbm .pfs .pfm .pgm .picon .pic .pict .pnm .ppm .psb .psd .ptif .ptiff .rfg .rgb .rgba .sgi .sun .uyvy .vicar .viff .wbmp .x .xbm .xpm .xwd .ycbr .ycbra .yub ")
#define KS_EXTENSION_ALL	(StringReplace((KS_EXTENSION_BMP KS_EXTENSION_AVIF KS_EXTENSION_BMP KS_EXTENSION_CSS KS_EXTENSION_DLL KS_EXTENSION_EXE KS_EXTENSION_FLAC KS_EXTENSION_GIF KS_EXTENSION_GZ KS_EXTENSION_HTML KS_EXTENSION_ICO KS_EXTENSION_JP2 KS_EXTENSION_JPG KS_EXTENSION_JXL KS_EXTENSION_JS KS_EXTENSION_LUA KS_EXTENSION_MIME KS_EXTENSION_MKV KS_EXTENSION_MNG KS_EXTENSION_MP3 KS_EXTENSION_MP4 KS_EXTENSION_OBJ KS_EXTENSION_OGG KS_EXTENSION_OGV KS_EXTENSION_OLE KS_EXTENSION_PCX KS_EXTENSION_PDF KS_EXTENSION_PNG KS_EXTENSION_SQLITE KS_EXTENSION_SWF KS_EXTENSION_TAR KS_EXTENSION_TENCENTQQ KS_EXTENSION_TGA KS_EXTENSION_TIFF KS_EXTENSION_WAV KS_EXTENSION_WEBP KS_EXTENSION_XML KS_EXTENSION_ZIP KS_EXTENSION_7Z KS_EXTENSION_MISC), "  ", " ", TReplaceFlags() << rfReplaceAll))


// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct udtOptions
{
	bool bBMPCopyMetadata;
	bool bCSSEnableTidy;
	TCHAR acCSSTemplate[64];
	bool bEXEDisablePETrim;
	bool bEXEEnableUPX;
	bool bGIFCopyMetadata;
	bool bGIFAllowLossy;
	bool bGZCopyMetadata;
	bool bHTMLEnableTidy;
	bool bJPEGCopyMetadata;
	bool bJPEGUseArithmeticEncoding;
	bool bJPEGAllowLossy;
	bool bJSEnableJSMin;
	TCHAR acJSAdditionalExtensions[128];
	bool bLUAEnableLeanify;
	bool bMiscDisable;
	bool bMiscCopyMetadata;
	bool bMP3CopyMetadata;
	bool bMP4CopyMetadata;
	bool bPCXCopyMetadata;
	TCHAR acPDFProfile[64];
	int iPDFCustomDPI;
	bool bPDFSkipLayered;
	bool bPNGCopyMetadata;
	bool bPNGAllowLossy;
	bool bTGACopyMetadata;
	bool bTIFFCopyMetadata;
	bool bWAVCopyMetadata;
	bool bWAVStripSilence;
	bool bWEBPAllowLossy;
	bool bXMLEnableLeanify;
	bool bZIPCopyMetadata;
	bool bZIPRecurse;
	bool bDoNotUseRecycleBin;
	bool bDoNotCreateBackups;
	bool bKeepAttributes;
	TCHAR acIncludeMask[128];
	TCHAR acExcludeMask[128];
	TCHAR acDonator[128];
	TCHAR acDonation[128];
	TCHAR acDisablePluginMask[512];
	bool bBeepWhenDone;
	bool bShutdownWhenDone;
	bool bDisableSleep;
	bool bAlwaysOnTop;
	bool bAllowDuplicates;
	bool bAllowMultipleInstances;
	bool bClearWhenComplete;
	bool bEnableCache;
	bool bHideInstructions;
	int iLevel;
	int iProcessPriority;
	int iCheckForUpdates;
	int iLogLevel;
	int iFilenameFormat;
	int iLeanifyIterations;
	int iPNGWolfIterations;
	int iLanguage;
	int iStartupDelay;
	bool bDebug;
	bool bHideAds;
	bool bShowToolBar;
	TCHAR acVersion[16];
	TCHAR acTempDirectory[PATH_MAX];
	unsigned long long lStatTime;
	unsigned int iStatOpens;
	unsigned int iStatFiles;
	unsigned long long lStatTotalBytes;
	unsigned long long lStatSavedBytes;
	unsigned int iStatSession;
	unsigned int iAdsShown;
};


extern struct udtOptions gudtOptions;
extern bool gbProcess;
extern bool gbStop;
extern unsigned int miStartTicks;

// ---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published: // IDE-managed Components

	TStringGrid *grdFiles;
	TPopupMenu *mnuFiles;
	TMenuItem *mnuFilesOptimize;
	TMenuItem *mnuFilesClear;
	TMenuItem *N1;
	TLabel *lblCopyright;
	TTimer *tmrMain;
	TStatusBar *stbMain;
	TWebBrowser *webAds;
	TProgressBar *pgbProgress;
	TMenuItem *mnuFilesRemove;
	TMenuItem *mnuFilesAdd;
	TMenuItem *N2;
	TMenuItem *mnuFilesOptions;
	TMenuItem *mnuFilesAbout;
	TMenuItem *mnuFilesExit;
	TMenuItem *mnuFilesHelp;
	TOpenDialog *dlgAddFiles;
	TActionManager *mgrMain;
	TAction *actAdd;
	TAction *actRemove;
	TAction *actClear;
	TAction *actOptimize;
	TAction *actOptions;
	TAction *actAbout;
	TAction *actInformation;
	TAction *actExit;
	TImageList *imgMain;
	TAction *actDonate;
	TMenuItem *mnuFilesDonate;
	TMenuItem *N3;
	TAction *actStop;
	TMenuItem *mnuFilesStop;
	TMainMenu *mnuMain;
	TMenuItem *mnuFile;
	TMenuItem *mnuFileAdd;
	TMenuItem *mnuFileRemove;
	TMenuItem *mnuFileClear;
	TMenuItem *mnuFileExit;
	TMenuItem *N4;
	TMenuItem *mnuOptimize;
	TMenuItem *mnuOptimizeOptimize;
	TMenuItem *mnuOptimizeStop;
	TMenuItem *N5;
	TMenuItem *mnuOptimizeOptions;
	TMenuItem *mnuHelp;
	TMenuItem *mnuHelpDonate;
	TMenuItem *mnuHelpAbout;
	TMenuItem *mnuHelpInformation;
	TMenuItem *N6;
	TMenuItem *N7;
	TMenuItem *N8;
	TAction *actHelp;
	TAction *actOpen;
	TMenuItem *mnuHelpTopics;
	TMenuItem *mnuFileOpenSelected;
	TAction *actOpenFolder;
	TMenuItem *mnuFileOpenFolder;
	TLabel *lblInstructions;
	TMenuItem *mnuFolderAdd;
	TAction *actAddFolder;
	TMenuItem *mnuFileAddFolder;
	TToolBar *tooMain;
	TToolButton *tooFileAdd;
	TToolButton *tooFileAddFolder;
	TToolButton *tooFileRemove;
	TToolButton *tooFileClear;
	TToolButton *tooSeparator1;
	TToolButton *tooFileOpen;
	TToolButton *tooFileOpenFolder;
	TToolButton *tooFileExit;
	TToolButton *tooSeparator2;
	TToolButton *tooOptimizeOptimize;
	TToolButton *tooOptimizeStop;
	TToolButton *tooSeparator5;
	TToolButton *tooOptimizeIptions;
	TToolButton *tooSeparator3;
	TToolButton *tooHelpHelp;
	TToolButton *tooSeparator6;
	TToolButton *tooHelpInformation;
	TToolButton *tooSeparator4;
	TToolButton *tooHelpDonate;
	TToolButton *tooSeparator7;
	TToolButton *tooHelpAbout;
	TMenuItem *mnuFilesInformation;
	TMenuItem *mnuFilesHelpTopics;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall lblCopyrightClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall tmrMainTimer(TObject *Sender);
	void __fastcall stbMainDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall grdFilesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall grdFilesFixedCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall grdFilesMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall grdFilesDblClick(TObject *Sender);
	void __fastcall actAddExecute(TObject *Sender);
	void __fastcall actRemoveExecute(TObject *Sender);
	void __fastcall actClearExecute(TObject *Sender);
	void __fastcall actOpenExecute(TObject *Sender);
	void __fastcall actOpenFolderExecute(TObject *Sender);
	void __fastcall actExitExecute(TObject *Sender);
	void __fastcall actOptimizeExecute(TObject *Sender);
	void __fastcall actOptionsExecute(TObject *Sender);
	void __fastcall actHelpExecute(TObject *Sender);
	void __fastcall actAboutExecute(TObject *Sender);
	void __fastcall actInformationExecute(TObject *Sender);
	void __fastcall actDonateExecute(TObject *Sender);
	void __fastcall actStopExecute(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall actAddFolderExecute(TObject *Sender);
	void __fastcall webAdsTitleChange(TObject *ASender, const WideString Text);




private: // User declarations
	BEGIN_MESSAGE_MAP
		// add message handler for WM_DROPFILES
		VCL_MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WMDropFiles)
	END_MESSAGE_MAP(TComponent)
	String __fastcall GetExtensionByContent (const String psFilename, bool pbForce = false);
	String __inline FormatNumberThousand (unsigned long long plNumber);
	String __inline FormatNumberThousandUnit (unsigned long long plNumber);
	unsigned long long __inline ParseNumberThousand (String psNumber);
	int __fastcall RunPlugin(unsigned int piCurrent, String psStatus, String psCommandLine, String psInputFile, String psOutputFile, int piErrorMin = INT_MIN, int piErrorMax = INT_MAX);
	unsigned long __fastcall RunProcess(const TCHAR *pacProcess, TCHAR *pacOutput, unsigned int piOutputLen, bool pbWait);
	void __fastcall WMDropFiles(const TWMDropFiles &udtMessage);
	void __fastcall AddFilesInitializeExist(void);
	bool __fastcall AddFilesExist(const String psFile);
	void __fastcall AddFiles(const TCHAR *pacFile);
	void __fastcall CheckForUpdates(bool pbSilent = true);
	void __fastcall CheckForUpdatesOld(bool pbSilent = true);
	bool __fastcall IsAPNG(const TCHAR *pacFile);
	bool __fastcall IsEXESFX(const TCHAR *pacFile);
	bool __fastcall IsEXEManagedNet(const TCHAR *pacFile);
	bool __fastcall IsPDFLayered(const TCHAR *pacFile);
	bool __fastcall IsJPEGCMYK(const TCHAR *pacFile);
	String __fastcall Hash(const String psFilename);
	String __inline GetCellValue(String psValue, unsigned int piPos);
	String __inline SetCellFileValue(const String psValue);
	void __fastcall OptimizeProgressThread(struct udtOptimizeProgress pudtOptimizeProgress);
	void __fastcall OptimizeProgressVCL(void);
	void __fastcall actOptimizeFor(TObject *Sender, int AIndex);
	void __fastcall actOptimizeForThread(TObject *Sender, int AIndex, TParallel::TLoopState *LoopState);

public: // User declarations
	__fastcall TfrmMain(TComponent* Owner);
    __fastcall ~TfrmMain();
    void __fastcall LoadOptions(void); 
	void __fastcall SaveOptions(void); 
	void __fastcall RefreshStatus(bool pbUpdateStatusBar = true, unsigned int piCurrent = 0, unsigned long long plTotalBytes = 0, unsigned long long plSavedBytes = 0);
	void __fastcall UpdateAds(void);
	void __fastcall UpdateTheme(void);
	static const TCHAR * __fastcall GetOptionCommandLine(void);
	static const TCHAR * __fastcall GetOptionArgument(const TCHAR *pacKey);
	static const TCHAR * __fastcall GetOption(const TCHAR *pacSection, const TCHAR *pacKey, const TCHAR *pacDefault);
	static int __fastcall GetOption(const TCHAR *pacSection, const TCHAR *pacKey, int piDefault);
	static long long __fastcall GetOption(const TCHAR *pacSection, const TCHAR *pacKey, long long plDefault);
	static double __fastcall GetOption(const TCHAR *pacSection, const TCHAR *pacKey, double pdDefault);
	static bool __fastcall GetOption(const TCHAR *pacSection, const TCHAR *pacKey, bool pbDefault);
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
// ---------------------------------------------------------------------------
#endif
