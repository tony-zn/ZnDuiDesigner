#include "StdAfx.h"
#include "Clipboard.h"
#include <string>

CClipboard& CClipboard::Instance()
{
	static CClipboard cb;
	return cb;
}

bool CClipboard::Write( DuiLib::CControlUI* _pControl )
{
	CFileManager fm;
	_ZnString str;
	fm.SaveToStr(_pControl, get_wnd_ptr(_pControl), _pControl->GetManager(), str);
	UINT unClipFormat = ::RegisterClipboardFormat(_T("DUIDESIGNER_CLIPBORAD_CONTROL"));
	if (!::OpenClipboard(NULL)) return false;
	HGLOBAL clipbuffer;
	::EmptyClipboard();
	size_t size = str.size() * sizeof(_ZnString::value_type);
	clipbuffer = ::GlobalAlloc(GMEM_DDESHARE, size);
	LPBYTE	pData = (LPBYTE)::GlobalLock(clipbuffer);
	memcpy_s(pData, size, (LPBYTE)str.c_str(), size);
	::GlobalUnlock(clipbuffer);
	::SetClipboardData(unClipFormat, clipbuffer);
	::CloseClipboard();
	return true;
}

DuiLib::CControlUI* CClipboard::Read( DuiLib::IDialogBuilderCallback* _pCallBack, DuiLib::CPaintManagerUI* _pPaintManager, DuiLib::CControlUI* _pParent )
{
	UINT unClipFormat = ::RegisterClipboardFormat(_T("DUIDESIGNER_CLIPBORAD_CONTROL"));
	if (::OpenClipboard(NULL))
	{
		HANDLE hData = ::GetClipboardData(unClipFormat);
		if (!hData) return nullptr;
		LPBYTE pData = (LPBYTE)::GlobalLock(hData);
		size_t nLen = ::GlobalSize(hData); 
		_ZnString str(reinterpret_cast<_ZnString::value_type*>(pData), reinterpret_cast<_ZnString::value_type*>(pData + nLen));
		::GlobalUnlock(hData);
		::CloseClipboard();
		DuiLib::CDialogBuilder builder;
		DuiLib::CControlUI* pNew = builder.Create(str.c_str(), NULL, _pCallBack, _pPaintManager, _pParent);
		if (pNew) pNew->SetManager(_pPaintManager, _pParent);
		return pNew;
	}
	return nullptr;
}

bool CClipboard::IsEmpty()
{
	UINT unClipFormat = ::RegisterClipboardFormat(_T("DUIDESIGNER_CLIPBORAD_CONTROL"));
	if (::OpenClipboard(NULL))
	{
		HANDLE hData = ::GetClipboardData(unClipFormat);
		LPBYTE pData = (LPBYTE)::GlobalLock(hData);
		size_t nLen = ::GlobalSize(hData); 
		::GlobalUnlock(hData);
		::CloseClipboard();
		return nLen == 0;
	}
	return false;
}
