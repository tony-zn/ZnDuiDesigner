#include <wtypes.h>
#define UILIB_STATIC
#include "../DuiLib/UIlib.h"
#include "../Lib/Lib.hpp"
#include "../DuiDesigner/ControlPluginInterface.hpp"

#include "DS_Canvas.hpp"
#include "DS_Window.hpp"
#include "DS_AutoWidthOption.hpp"
#include "DS_AutoWidthHLayout.hpp"

static TCHAR sControls[] = _T("Canvas\0Window\0AutoWidthOption\0AutoWidthHLayout\0\0");

extern "C"
{
	__declspec(dllexport) DuiLib::CControlUI* CreateControl(LPCTSTR _lpStrType)
	{
		if (_tcscmp(_lpStrType, _T("Canvas")) == 0)
			return new CDS_CanvasUI;
		else if (_tcscmp(_lpStrType, _T("Window")) == 0)
			return new CDS_Window;
		else if (_tcscmp(_lpStrType, _T("AutoWidthOption")) == 0)
			return new CDS_AutoWidthOptionUI;
		else if (_tcscmp(_lpStrType, _T("AutoWidthHLayout")) == 0)
			return new CDS_AutoWidthHLayout;
		return nullptr;
	}

	__declspec(dllexport) LPCTSTR GetControls()
	{
		return sControls;
	}

	__declspec(dllexport) void SetInstance(HINSTANCE hInst)
	{
		DuiLib::CPaintManagerUI::SetInstance(hInst);
	}

	__declspec(dllexport) void SetCurrentPath(LPCTSTR pStrPath)
	{
		DuiLib::CPaintManagerUI::SetCurrentPath(pStrPath);
	}

	__declspec(dllexport) void SetResourceDll(HINSTANCE hInst)
	{
		DuiLib::CPaintManagerUI::SetResourceDll(hInst);
	}

	__declspec(dllexport) void SetResourcePath(LPCTSTR pStrPath)
	{
		DuiLib::CPaintManagerUI::SetResourcePath(pStrPath);
	}

	__declspec(dllexport) void SetResourceZip(LPVOID pVoid, unsigned int len)
	{
		DuiLib::CPaintManagerUI::SetResourceZip(pVoid, len);
	}

	__declspec(dllexport) void SetResourceZipFromName(LPCTSTR pstrZip, bool bCachedResourceZip = false)
	{
		DuiLib::CPaintManagerUI::SetResourceZip(pstrZip, bCachedResourceZip);
	}
}



