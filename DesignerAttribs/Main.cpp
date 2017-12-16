#include <wtypes.h>
#define UILIB_STATIC
#include "../DuiLib/UIlib.h"
#include "../Lib/Lib.hpp"
#include "../DuiDesigner/ControlPluginInterface.hpp"
#include "Globle.hpp"
#include "Edit.hpp"
#include "Line.hpp"
#include "Rect.hpp"
#include "Size.hpp"
#include "CheckBox.hpp"
#include "Pos.hpp"
#include "Folder.hpp"
#include "Image.hpp"
#include "Font.hpp"
#include "Color.hpp"

static TCHAR sAttribs[] = _T("Edit\0Size\0Rect\0CheckBox\0Pos\0Line\0Folder\0Image\0Font\0Color\0\0");

extern "C"
{
	__declspec(dllexport) DuiLib::CControlUI* CreateAttrib(LPCTSTR _lpStrType, DuiLib::CControlUI* _pControl, LPCTSTR _lpAttrib, LPCTSTR _lpName)
	{
		if (_tcscmp(_lpStrType, _T("Edit")) == 0)
			return new CAttr_Edit(_pControl, _lpAttrib, _lpName);
		else if (_tcscmp(_lpStrType, _T("Size")) == 0)
			return new CAttr_Size(_pControl, _lpAttrib, _lpName);
		else if (_tcscmp(_lpStrType, _T("Rect")) == 0)
			return new CAttr_Rect(_pControl, _lpAttrib, _lpName);
		else if (_tcscmp(_lpStrType, _T("CheckBox")) == 0)
			return new CAttr_CheckBox(_pControl, _lpAttrib, _lpName);
		else if (_tcscmp(_lpStrType, _T("Line")) == 0)
			return new CAttr_Line(_pControl, _lpAttrib, _lpName);
		else if (_tcscmp(_lpStrType, _T("Pos")) == 0)
			return new CAttr_Pos(_pControl, _lpAttrib, _lpName);
		else if (_tcscmp(_lpStrType, _T("Folder")) == 0)
			return new CAttr_Folder(_pControl, _lpAttrib, _lpName);
		else if (_tcscmp(_lpStrType, _T("Image")) == 0)
			return new CAttr_Image(_pControl, _lpAttrib, _lpName);
		else if (_tcscmp(_lpStrType, _T("Font")) == 0)
			return new CAttr_Font(_pControl, _lpAttrib, _lpName);
		else if (_tcscmp(_lpStrType, _T("Color")) == 0)
			return new CAttr_Color(_pControl, _lpAttrib, _lpName);
		return nullptr;
	}

	__declspec(dllexport) LPCTSTR GetAttribs()
	{
		return sAttribs;
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