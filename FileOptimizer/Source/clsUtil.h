// ---------------------------------------------------------------------------
#pragma once
#ifndef clsUtilH
#define clsUtilH
#include "FileOptimizerPCH1.h"


// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#if !defined(_max)
	#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#if !defined(_min)
	#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*
#if defined(_DEBUG)
	//For some reason BCC64 gaves an error with __FUNC__
	#if defined(_WIN64)
		#define Log(piLevel, pacValue) (clsUtil::LogAdd(_T(__FILE__), __LINE__, _T(""), piLevel, pacValue, 5))
	#else
		#define Log(piLevel, pacValue) (clsUtil::LogAdd(_T(__FILE__), __LINE__, _T(__FUNC__), piLevel, pacValue, 5))
	#endif
#else
	#if defined(_WIN64)
		#define Log(piLevel, pacValue) (clsUtil::LogAdd(_T(__FILE__), __LINE__, _T(""), piLevel, pacValue, gudtOptions.iLogLevel))
	#else
		#define Log(piLevel, pacValue) (clsUtil::LogAdd(_T(__FILE__), __LINE__, _T(__FUNC__), piLevel, pacValue, gudtOptions.iLogLevel))
	#endif
#endif
*/


#if defined(PATH_MAX)
	#undef PATH_MAX
#endif
#define PATH_MAX	(MAX_PATH << 6)


// ---------------------------------------------------------------------------
class clsUtil
{
	public:
		static const void * __fastcall MemMem (const void *buf, size_t buf_len, const void *byte_sequence, size_t byte_sequence_len);
		static HRESULT CALLBACK TaskDialogCallbackProc (HWND phWnd, UINT uNotification, WPARAM wParam, LPARAM lParam, LONG_PTR dwRefData);
		static int __fastcall MsgBox(HWND phWnd, const TCHAR *pacText, const TCHAR *pacTitle, unsigned int piType, unsigned int piTimeout = 0);
		static HANDLE __fastcall FindProcess(const TCHAR *pacProcess);
		static unsigned long __fastcall RunProcess(const TCHAR *pacProcess, const TCHAR *pacDirectory, TCHAR *pacOutput, unsigned int piOutputLen, bool pbWait);
		static bool __fastcall ExistsFile(const TCHAR *pacFile);
		static bool __fastcall RenameFile(const TCHAR *pacNewName, const TCHAR *pacOldName);
		static unsigned long long __fastcall SizeFile(const TCHAR *pacFile);
		static bool __fastcall ReadFile(const TCHAR *pacFile, void *pvData, unsigned int *piSize, unsigned int piOffset = 0);
		static bool __fastcall WriteFile(const TCHAR *pacFile, const void *pvData, unsigned int piSize, unsigned int piOffset = 0);
		static bool __fastcall GetFileTimestamp(const TCHAR *pacFile, FILETIME *pudtCreated = nullptr, FILETIME *pudtAccessed  = nullptr, FILETIME *pudtModified  = nullptr);
		static bool __fastcall SetFileTimestamp(const TCHAR *pacFile, const FILETIME *pudtCreated = nullptr, const FILETIME *pudtAccessed = nullptr, const FILETIME *pudtModified = nullptr);
		static bool __fastcall DirectoryCreate(String psDirectory);
		static bool __fastcall DownloadFile(const TCHAR *pacUrl, void *pvData, unsigned int piSize);
		static bool __fastcall DownloadFilePost(const TCHAR *pacServer, const TCHAR *pacPage, const char *pacParameters, void *pvData, unsigned int piSize, bool pbHttps = false);
		static bool __fastcall CopyFile(const TCHAR *pacSource, const TCHAR *pacDestination);
		static const TCHAR * __fastcall ExeVersion(const TCHAR *pacFile);
		static size_t __fastcall GetFileVersionField(const TCHAR *fn, const TCHAR *info, TCHAR *ret, size_t len);
		static const TCHAR * __fastcall GetIniPath(bool pbAllUsers = false);
		static const TCHAR * __fastcall GetIni(const TCHAR *pacSection, const TCHAR *pacKey, const TCHAR *pacDefault);
		static int __fastcall GetIni(const TCHAR *pacSection, const TCHAR *pacKey, int piDefault = 0);
		static long long __fastcall GetIni(const TCHAR *pacSection, const TCHAR *pacKey, long long plDefault = 0);
		static double __fastcall GetIni(const TCHAR *pacSection, const TCHAR *pacKey, double pdDefault = 0);
		static bool __fastcall GetIni(const TCHAR *pacSection, const TCHAR *pacKey, bool pbDefault = false );
		static bool __fastcall SetIni(const TCHAR *pacSection, const TCHAR *pacKey, const TCHAR *pacValue = _T(""), const TCHAR *pacComment = _T(""));
		static bool __fastcall SetIni(const TCHAR *pacSection, const TCHAR *pacKey, bool pbValue, const TCHAR *pacComment = _T(""));
		static bool __fastcall SetIni(const TCHAR *pacSection, const TCHAR *pacKey, int piValue, const TCHAR *pacComment = _T(""));
		static bool __fastcall SetIni(const TCHAR *pacSection, const TCHAR *pacKey, long long plValue, const TCHAR *pacComment = _T(""));
		static bool __fastcall SetIni(const TCHAR *pacSection, const TCHAR *pacKey, double pdValue, const TCHAR *pacComment = _T(""));
		static bool __fastcall DeleteIni(const TCHAR *pacSection, const TCHAR *pacKey = nullptr);
		static unsigned int __fastcall Serialize (void *pacBuffer, unsigned int piSize);
		static unsigned int __fastcall Unserialize (void *pacBuffer, unsigned int piSize);
		static unsigned int __fastcall Crc32 (const void *pacBuffer, unsigned int piLen, unsigned int piOldCrc = 0xFFFFFFFF);
		static int __fastcall Random (int piMin, int piMax);
		static const TCHAR * __fastcall GetRegistryPath(void);
		static const TCHAR * __fastcall GetRegistry(HKEY phKey, const TCHAR *pacSubkey, const TCHAR *pacName);
        static int __fastcall GetRegistryI(HKEY phKey, const TCHAR *pacSubkey, const TCHAR *pacName);
		static bool __fastcall SetRegistry(HKEY phKey, const TCHAR *pacSubkey, const TCHAR *pacName, const TCHAR *pacValue);
		static bool __fastcall SetRegistry(HKEY phKey, const TCHAR *pacSubkey, const TCHAR *pacName, unsigned int piValue);
		static bool __fastcall SetRegistry(HKEY phKey, const TCHAR *pacSubkey, const TCHAR *pacName, unsigned long long plValue);
		static bool __fastcall DeleteRegistry(HKEY phKey, const TCHAR *pacSubkey);
		static const TCHAR * __fastcall GetLogPath(void);
		//static void __fastcall LogAdd(const TCHAR *pacFile, int piLine, const TCHAR *pacFunc, int piLevel, const TCHAR *pacValue, int piDesiredLevel);
		static void __fastcall Log(int piLevel, const TCHAR *pacValue, int piDesiredLevel);
		static bool __fastcall LoadForm(TForm *pfrmForm);
		static bool __fastcall SaveForm(TForm *pfrmForm);
		static bool __fastcall CopyToRecycleBin(const TCHAR *pacSource);
		static bool __fastcall SetTaskListProgress(unsigned int piCompleted, unsigned int piTotal);
		static unsigned int __fastcall GetWindowsVersion(void);
		static bool __fastcall IsWindows64(void);
		static bool __fastcall IsWine(void);
		static bool __fastcall ShutdownWindows(unsigned int piMode = 0);
		static String __fastcall GetShortName(String psLongName);
		static bool __fastcall DeleteFile(const TCHAR *pacFile);
	private:
		static void __fastcall EscapeIniKey (TCHAR *pacBuffer);
		static void __fastcall UnescapeIniKey (TCHAR *pacBuffer);
		static void __fastcall EscapeIniValue (TCHAR *pacBuffer);
		static void __fastcall UnescapeIniValue (TCHAR *pacBuffer);

};

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif
