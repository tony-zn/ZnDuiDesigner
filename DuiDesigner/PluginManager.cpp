#include "StdAfx.h"
#include "PluginManager.h"

typedef LPCTSTR(*LPGETCONTROLS)(void);

CControlPluginManager& CControlPluginManager::Instance()
{
	static CControlPluginManager cpm;
	return cpm;
}

void CControlPluginManager::Load( LPCTSTR _lpPath, LPCTSTR _lpSkinPath )
{
	__m_strSkinPath = _lpSkinPath;
	__TraverseDll(_lpPath, _lpSkinPath);
}

_CONTROL_PLUGIN* CControlPluginManager::GetItem(UINT _unItem)
{
	if (__m_vecPlugin.empty()) return nullptr;
	if (_unItem >= __m_vecPlugin.size()) return nullptr;
	return &__m_vecPlugin[_unItem];
}

UINT CControlPluginManager::GetSize()
{
	return __m_vecPlugin.size();
}

void CControlPluginManager::__TraverseDll( LPCTSTR _lpPath, LPCTSTR _lpSkinPath )
{
	DuiLib::CDuiString strFindStr(_lpPath);
	strFindStr += _T("*.dll");
	// 添加这个目录
	WIN32_FIND_DATA fdata = { 0 };
	HANDLE hFile = ::FindFirstFile(strFindStr.GetData(), &fdata);
	while (INVALID_HANDLE_VALUE != hFile)
	{
		if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			DuiLib::CDuiString strPath;
			strPath.Format(_T("%s%s"), _lpPath, fdata.cFileName);
			__TraverseDll(strPath.GetData(), _lpSkinPath);
		}
		else
			__LoadDll(fdata.cFileName, _lpPath, _lpSkinPath);
		if (::FindNextFile(hFile, &fdata) == FALSE)
			break;
	}
}

bool CControlPluginManager::__LoadDll( LPCTSTR _lpFile, LPCTSTR _lpPath, LPCTSTR _lpSkinPath )
{
	DuiLib::CDuiString strPath;
	strPath.Format(_T("%s%s"), _lpPath, _lpFile);
	_CONTROL_PLUGIN cp;
	cp.hModule = ::LoadLibrary(strPath);
	cp.strPath = _lpPath;
	cp.strFile = _lpFile;
	if (cp.hModule)
	{
		cp.lpFunCreateControl = (DuiLib::LPCREATECONTROL)::GetProcAddress(cp.hModule, "CreateControl");
		LPGETCONTROLS lpGetControls  = (LPGETCONTROLS)::GetProcAddress(cp.hModule, "GetControls");
		cp.lpFunSetResourcePath = (LPSETRESOURCEPATH)::GetProcAddress(cp.hModule, "SetResourcePath");
		if (cp.lpFunCreateControl && lpGetControls && cp.lpFunSetResourcePath)
		{
			cp.lpFunSetResourcePath(_lpSkinPath);
			LPCTSTR _lpNames = lpGetControls();
			std::vector<DuiLib::CDuiString> vecNames;
			split_string(_lpNames, vecNames);
			for (auto it = vecNames.begin(); it != vecNames.end(); ++it)
				cp.vecControls.push_back(*it);
			__m_vecPlugin.push_back(cp);
			TRACE(_T("加载控件插件, 共 %d 个控件：%s ！"), vecNames.size(), strPath);
			return true;
		}
		else
		{
			::FreeLibrary(cp.hModule);
			TRACE(_T("%s 不是正确的控件插件！"), strPath);
		}
	}
	return false;
}

DuiLib::CControlUI* CControlPluginManager::CreateControl( LPCTSTR _lpStrType )
{
	for (VEC_PLUGIN::iterator it = __m_vecPlugin.begin(); it != __m_vecPlugin.end(); ++it)
	{
		DuiLib::CControlUI* pControl = it->lpFunCreateControl(_lpStrType);
		if (pControl) return pControl;
	}
	return nullptr;
}

void CControlPluginManager::SetResourcePath( LPCTSTR _lpResourcePath )
{
	for (VEC_PLUGIN::iterator it = __m_vecPlugin.begin(); it != __m_vecPlugin.end(); ++it)
		if (it->lpFunSetResourcePath) it->lpFunSetResourcePath(_lpResourcePath);
}

CAttribPluginManager& CAttribPluginManager::Instance()
{
	static CAttribPluginManager apm;
	return apm;
}

void CAttribPluginManager::Load( LPCTSTR _lpPath, LPCTSTR _lpSkinPath )
{
	__TraverseDll(_lpPath, _lpSkinPath);
}

DuiLib::CControlUI* CAttribPluginManager::CreateAttrib( LPCTSTR _lpStrType, DuiLib::CControlUI* _pControl, LPCTSTR _lpAttrib, LPCTSTR _lpName )
{
	for (VEC_PLUGIN::iterator it = __m_vecPlugin.begin(); it != __m_vecPlugin.end(); ++it)
	{
		DuiLib::CControlUI* pControl = it->lpFunCreateAttrib(_lpStrType, _pControl, _lpAttrib, _lpName);
		if (pControl) return pControl;
	}
	return nullptr;
}

_ATTRIB_PLUGIN* CAttribPluginManager::GetItem( UINT _unItem )
{
	if (__m_vecPlugin.empty()) return nullptr;
	if (_unItem >= __m_vecPlugin.size()) return nullptr;
	return &__m_vecPlugin[_unItem];
}

UINT CAttribPluginManager::GetSize()
{
	return __m_vecPlugin.size();
}

void CAttribPluginManager::__TraverseDll( LPCTSTR _lpPath, LPCTSTR _lpSkinPath )
{
	DuiLib::CDuiString strFindStr(_lpPath);
	strFindStr += _T("*.dll");
	// 添加这个目录
	WIN32_FIND_DATA fdata = { 0 };
	HANDLE hFile = ::FindFirstFile(strFindStr.GetData(), &fdata);
	while (INVALID_HANDLE_VALUE != hFile)
	{
		if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			DuiLib::CDuiString strPath;
			strPath.Format(_T("%s%s"), _lpPath, fdata.cFileName);
			__TraverseDll(strPath.GetData(), _lpSkinPath);
		}
		else
			__LoadDll(fdata.cFileName, _lpPath, _lpSkinPath);
		if (::FindNextFile(hFile, &fdata) == FALSE)
			break;
	}
}

bool CAttribPluginManager::__LoadDll( LPCTSTR _lpFile, LPCTSTR _lpPath, LPCTSTR _lpSkinPath )
{
	DuiLib::CDuiString strPath;
	strPath.Format(_T("%s%s"), _lpPath, _lpFile);
	_ATTRIB_PLUGIN ap;
	ap.hModule = ::LoadLibrary(strPath);
	ap.strPath = _lpPath;
	ap.strFile = _lpFile;
	if (ap.hModule)
	{
		ap.lpFunCreateAttrib = (LPCREATEATTRIB)::GetProcAddress(ap.hModule, "CreateAttrib");
		LPGETCONTROLS lpGetAttribs  = (LPGETCONTROLS)::GetProcAddress(ap.hModule, "GetAttribs");
		ap.lpFunSetResourcePath = (LPSETRESOURCEPATH)::GetProcAddress(ap.hModule, "SetResourcePath");
		if (ap.lpFunCreateAttrib && lpGetAttribs && ap.lpFunSetResourcePath)
		{
			ap.lpFunSetResourcePath(_lpSkinPath);
			LPCTSTR _lpNames = lpGetAttribs();
			std::vector<DuiLib::CDuiString> vecNames;
			split_string(_lpNames, vecNames);
			for (auto it = vecNames.begin(); it != vecNames.end(); ++it)
				ap.vecAttribs.push_back(*it);
			__m_vecPlugin.push_back(ap);
			TRACE(_T("加载属性插件, 共 %d 个属性：%s ！"), vecNames.size(), strPath);
			return true;
		}
		else
		{
			::FreeLibrary(ap.hModule);
			TRACE(_T("%s 不是正确的属性插件！"), strPath);
		}
	}
	return false;
}

void CAttribPluginManager::SetResourcePath( LPCTSTR _lpResourcePath )
{
	for (VEC_PLUGIN::iterator it = __m_vecPlugin.begin(); it != __m_vecPlugin.end(); ++it)
		if (it->lpFunSetResourcePath) it->lpFunSetResourcePath(_lpResourcePath);
}
