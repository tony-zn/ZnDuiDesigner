#include "StdAfx.h"

LPCTSTR cur_path()
{
	static DuiLib::CDuiString strPath;
	if (strPath.IsEmpty())
	{
		TCHAR sDrive[_MAX_DRIVE];
		TCHAR sDir[_MAX_DIR];
		TCHAR sFilename[_MAX_FNAME], Filename[_MAX_FNAME];
		TCHAR sExt[_MAX_EXT];
		GetModuleFileName(DuiLib::CPaintManagerUI::GetInstance(), Filename, _MAX_PATH);
		_tsplitpath_s(Filename, sDrive, sDir, sFilename, sExt);
		strPath = sDrive;
		strPath = strPath + sDir;
		int nLen = strPath.GetLength();

		if (strPath.GetAt(nLen - 1) != _T('\\'))
			strPath += _T('\\');
	}
	return strPath.GetData();
}

void split_string( LPCTSTR _lpStr, std::vector<DuiLib::CDuiString>& vecStr )
{
	for (UINT i = 0, p = 0; ; ++i)
	{
		if (_lpStr[i] == 0)
		{
			vecStr.push_back(&_lpStr[p]);
			if (_lpStr[i+1] == 0) break;
			p = i + 1;
		}
	}
}

std::tr1::shared_ptr<char>		W2Str(const wchar_t* _lpwstr, UINT unCode)
{
	int nLen = WideCharToMultiByte(unCode, 0, _lpwstr, -1, NULL, 0, NULL, NULL);
	if (nLen<= 0) return std::tr1::shared_ptr<char>();
	std::tr1::shared_ptr<char> spDst(new char[nLen]);
	if (NULL == spDst) return std::tr1::shared_ptr<char>();
	WideCharToMultiByte(unCode, 0, _lpwstr, -1, spDst.get(), nLen, NULL, NULL);
	spDst.get()[nLen - 1] = 0;
	return spDst;
}
std::tr1::shared_ptr<wchar_t>	S2Wstr(const char* _lpStr, UINT unCode)
{
	int nLen = MultiByteToWideChar(unCode, 0, (LPCSTR)_lpStr, -1, 0, 0);
	if(nLen <= 0) return std::tr1::shared_ptr<wchar_t>();
	std::tr1::shared_ptr<wchar_t> spDst(new wchar_t[nLen]);
	if( NULL == spDst) return std::tr1::shared_ptr<wchar_t>();
	MultiByteToWideChar(unCode, 0, _lpStr, -1, spDst.get(), nLen);
	spDst.get()[nLen - 1] = 0;
	return spDst;
}

LPCTSTR get_filename( LPCTSTR _lpFilePath )
{
	static TCHAR sFilename[_MAX_FNAME];
	TCHAR   sDrive[_MAX_DRIVE];
	TCHAR   sDir[_MAX_DIR];
	TCHAR   sExt[_MAX_EXT];
	_wsplitpath_s(_lpFilePath, sDrive, sDir, sFilename, sExt);
	_tcscat_s(sFilename, _MAX_FNAME, sExt);
	return sFilename;
}

IControlPlugin* get_control_interface( DuiLib::CControlUI* _pControl )
{
	if (!_pControl) return nullptr;
	static CDefaultControlAttrib dca;
	IControlPlugin* pInterface = static_cast<IControlPlugin*>(_pControl->GetInterface(DUI_CTR_CONTROL_PLUGIN_BASE));
	if (!pInterface) 
	{
		dca.SetControl(_pControl);
		pInterface = &dca;
	}
	return pInterface;
}

LPCTSTR get_path( LPCTSTR _lpFilePath )
{
	static TCHAR sFilename[_MAX_FNAME];
	TCHAR   sDrive[_MAX_DRIVE];
	TCHAR   sDir[_MAX_DIR];
	TCHAR   sExt[_MAX_EXT];
	_wsplitpath_s(_lpFilePath, sDrive, sDir, sFilename, sExt);
	ZeroMemory(sFilename, _MAX_FNAME);
	_tcscpy_s(sFilename, sDrive);
	_tcscat_s(sFilename, _MAX_FNAME, sDir);
	return sFilename;
}

int create_id()
{
	static int nID = 0;
	return nID++;
}

IWindowSharedData* get_wnd_shared_data( DuiLib::CControlUI* _pControl )
{
	DuiLib::CControlUI* pWnd = get_wnd_ptr(_pControl);
	if (!pWnd) return nullptr;
	return static_cast<IWindowSharedData*>(pWnd->GetInterface(DUI_CTR_WINDOW_SHARED_DATA));
}

DuiLib::CControlUI* get_wnd_ptr( DuiLib::CControlUI* _pControl )
{
	while(_pControl &&  _tcscmp(_pControl->GetClass(), STR_WINDOW) != 0)
		_pControl = get_parent(_pControl);
	return _pControl;
}

void split_attrib_list( LPCTSTR _lpStr, std::map<DuiLib::CDuiString, DuiLib::CDuiString>& _mapAttr )
{
	_ZnSStream ss(_lpStr);
	while(ss && !ss.eof())
	{
		_ZnString str;
		ss >> str;
		_ZnString strName = str.substr(0, str.find_first_of(_T('=')));
		_ZnString::size_type pos_begin = str.find_first_of(_T('\"')) + 1;
		_ZnString::size_type pos_end = str.find_last_of(_T('\"'));
		_ZnString strValue = str.substr(pos_begin, pos_end - pos_begin);
		if (!strName.empty()) _mapAttr[strName.c_str()] = strValue.c_str();
	}
}

void create_attrib_list( _ZnString& _Str, const std::map<DuiLib::CDuiString, DuiLib::CDuiString>& _mapAttr )
{
	_ZnSStream ss;
	for (auto it = _mapAttr.begin(); it != _mapAttr.end(); ++it)
		ss << it->first.GetData() << _T("=\"") << it->second.GetData() << _T("\" ");
	_Str = ss.str();
}

DuiLib::CTreeNodeUI* is_tree_node_sub_control( DuiLib::CControlUI* _pControl )
{
	if (_pControl->GetInterface(DUI_CTR_TREENODE))
		return static_cast<DuiLib::CTreeNodeUI*>(_pControl->GetInterface(DUI_CTR_TREENODE));
	DuiLib::CTreeNodeUI* pNode = nullptr;
	if (_pControl->GetParent()) 
	{
		pNode = static_cast<DuiLib::CTreeNodeUI*>(_pControl->GetParent()->GetInterface(DUI_CTR_TREENODE));
		if (!pNode && _pControl->GetParent()->GetParent())
			pNode = static_cast<DuiLib::CTreeNodeUI*>(_pControl->GetParent()->GetParent()->GetInterface(DUI_CTR_TREENODE));
	}
	if (pNode && (
		pNode->GetTreeNodeHoriznotal() == _pControl || 
		pNode->GetFolderButton() == _pControl || 
		pNode->GetDottedLine() == _pControl ||
		pNode->GetCheckBox() == _pControl ||
		pNode->GetItemButton() == _pControl
		))  
	{
		return pNode;
	}
	return nullptr;
}

void out_error( LPCTSTR _lpMsg )
{
	::MessageBox(g_pMainFrame->GetHWND(), _lpMsg, _T("DuiDesigner Error"), MB_OK);
}

DuiLib::CControlUI* get_parent( DuiLib::CControlUI* _pControl )
{
	if (!_pControl) return nullptr;
	DuiLib::CTreeNodeUI* pNode = static_cast<DuiLib::CTreeNodeUI*>(_pControl->GetInterface(DUI_CTR_TREENODE));
	if (pNode) 
	{
		DuiLib::CTreeNodeUI* pParentNode = pNode->GetParentNode();
		if (pParentNode)
			return pParentNode;
		else
			return pNode->GetTreeView();
	}
	return _pControl->GetParent();
}

RECT normalize_rect( const RECT& _rc )
{
	RECT rcRet = _rc;
	if (rcRet.left > rcRet.right)
		std::swap(rcRet.left, rcRet.right);
	if (rcRet.top > rcRet.bottom)
		std::swap(rcRet.top, rcRet.bottom);
	return rcRet;
}
