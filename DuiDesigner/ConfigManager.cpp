#include "StdAfx.h"
#include "ConfigManager.h"
#include <fstream>
#include <algorithm>

CConfigManager& CConfigManager::Instance()
{
	static CConfigManager cm;
	return cm;
}

bool CConfigManager::Load( LPCTSTR _lpFile )
{
	try
	{
		std::ifstream fs(__StrToXmlStr(_lpFile, CP_ACP).c_str());
		if (!fs) return false;
		rapidxml::file<> file(fs);
		rapidxml::xml_document<> xml;
		xml.clear();
		xml.parse<0>(file.data());
		rapidxml::xml_node<>* pRoot = xml.first_node();
		if (!pRoot) return false;
		for (rapidxml::xml_node<>* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling())
		{
			if (strcmp(pNode->name(), "basic") == 0)
				__LoadBasic(pNode, __m_Settings.Basic);
			else if (strcmp(pNode->name(), "controls") == 0)
				__LoadControls(pNode, __m_Settings.vecControl);
		}
	}
	catch(const rapidxml::parse_error& er)
	{
		TRACE(_T("XML解析异常: %s \n"), __XmlStrToStr(er.what(), CP_ACP));
		return false;
	}
	catch(const std::exception& ec)
	{
		TRACE(_T("标准库异常: %s \n")	, __XmlStrToStr(ec.what(), CP_ACP));
		return false;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CConfigManager::Save( LPCTSTR _lpFile )
{
	rapidxml::xml_document<TCHAR> xml;
	rapidxml::xml_node<TCHAR>* pRoot = xml.allocate_node(rapidxml::node_element, _T("root"));
	xml.append_node(pRoot);
	rapidxml::xml_node<TCHAR>* pBasic = xml.allocate_node(rapidxml::node_element, _T("basic"));
	pRoot->append_node(pBasic);
	__SaveBasic(pBasic);
	rapidxml::xml_node<TCHAR>* pControls = xml.allocate_node(rapidxml::node_element, _T("controls"));
	pRoot->append_node(pControls);
	__SaveControls(pControls);
	std::ofstream fs(__StrToXmlStr(_lpFile, CP_ACP).c_str());
	fs << __StrToXmlStr(_T("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>"), CP_UTF8).c_str() << std::endl;
	std::wstring str;
	rapidxml::print(std::back_inserter(str), xml);
	fs << __StrToXmlStr(str.c_str(), CP_UTF8);
	return true;
}

_SETTINGS& CConfigManager::Settings()
{
	return __m_Settings;
}

DuiLib::CDuiString CConfigManager::__XmlStrToStr( LPCSTR _lpStr, UINT unCode )
{
#	ifdef _UNICODE
	return DuiLib::CDuiString(S2Wstr(_lpStr, unCode).get());
#	else
	return DuiLib::CDuiString(_lpStr);
#	endif
}

std::string CConfigManager::__StrToXmlStr( LPCTSTR _lpStr, UINT unCode )
{
#	ifdef _UNICODE
	return std::string(W2Str(_lpStr, unCode).get());
#	else
	return std::string(_lpStr);
#	endif
}

DuiLib::CDuiString CConfigManager::__GetAttrib( rapidxml::xml_node<>* _pNode, LPCSTR _lpName )
{
	rapidxml::xml_attribute<>* pAttr = _pNode->last_attribute(_lpName);
	if (!pAttr) return DuiLib::CDuiString();
	return __XmlStrToStr(pAttr->value());
}

void CConfigManager::__LoadBasic( rapidxml::xml_node<>* _pNode, _SETTINGS_BASIC& _basic )
{
	for (rapidxml::xml_node<>* pNode = _pNode->first_node(); pNode; pNode = pNode->next_sibling())
	{
		if (strcmp(pNode->name(), "skin") == 0)
			_basic.Skin.strPath = __GetAttrib(pNode, "path");
	}
}

void CConfigManager::__LoadControls( rapidxml::xml_node<>* _pNode, _SETTINGS_CONTROLS& _vecControl )
{
	for (rapidxml::xml_node<>* pNode = _pNode->first_node(); pNode; pNode = pNode->next_sibling())
	{
		if (strcmp(pNode->name(), "control") == 0)
		{
			_SETTINGS_CONTROL control;
			control.strName = __GetAttrib(pNode, "name");
			control.strClassName = __GetAttrib(pNode, "classname");
			control.strImage = __GetAttrib(pNode, "image");
			control.strParent = __GetAttrib(pNode, "parent");
			control.bEnable = __GetAttrib(pNode, "enable") == _T("true");
			__LoadAttribs(pNode, control.vecAttrib);
			_vecControl.push_back(std::move(control));
		}
	}
}

void CConfigManager::__LoadAttribs( rapidxml::xml_node<>* _pNode, _SETTINGS_ATTRIBS& _vecAttrib )
{
	for (rapidxml::xml_node<>* pNode = _pNode->first_node(); pNode; pNode = pNode->next_sibling())
	{
		if (strcmp(pNode->name(), "attrib") == 0)
		{
			_SETTINGS_ATTRIB attrib;
			attrib.strName = __GetAttrib(pNode, "name");
			attrib.strAlias = __GetAttrib(pNode, "alias");
			attrib.strType = __GetAttrib(pNode, "type");
			attrib.strDefaultValue = __GetAttrib(pNode, "default");
			_vecAttrib.push_back(std::move(attrib));
		}
	}
}

_SETTINGS_CONTROL* CConfigManager::GetControl( LPCTSTR _lpName )
{
	for (_SETTINGS_CONTROLS::iterator it = __m_Settings.vecControl.begin(); it != __m_Settings.vecControl.end(); ++it)
		if (it->strName == _lpName)
			return &*it;
	return nullptr;
}

_SETTINGS_CONTROL* CConfigManager::AddControl( LPCTSTR _lpName )
{
	_SETTINGS_CONTROL* pSC = GetControl(_lpName);
	if (!pSC)
	{
		_SETTINGS_CONTROL sc;
		sc.strName = _lpName;
		sc.bEnable = false;
		__m_Settings.vecControl.push_back(std::move(sc));
		return AddControl(_lpName);
	}
	return pSC;
}

_SETTINGS_ATTRIB* CConfigManager::GetAttrib( _SETTINGS_CONTROL* _pControl, LPCTSTR _lpName )
{
	if (!_pControl) return nullptr;
	for (_SETTINGS_ATTRIBS::iterator it = _pControl->vecAttrib.begin(); it != _pControl->vecAttrib.end(); ++it)
		if (it->strName == _lpName)
			return &*it;
	return nullptr;
}

void CConfigManager::__SaveBasic( rapidxml::xml_node<TCHAR>* _pNode )
{
	rapidxml::xml_node<TCHAR>* pSkin = _pNode->document()->allocate_node(rapidxml::node_element, _T("skin"));
	rapidxml::xml_attribute<TCHAR>* pPath = _pNode->document()->allocate_attribute(_T("path"), __m_Settings.Basic.Skin.strPath);
	_pNode->append_node(pSkin);
	pSkin->append_attribute(pPath);
}

void CConfigManager::__SaveControls( rapidxml::xml_node<TCHAR>* _pNode )
{
	std::sort(__m_Settings.vecControl.begin(), __m_Settings.vecControl.end(), [](const _SETTINGS_CONTROL& _L, const _SETTINGS_CONTROL& _R)->bool{return _L.strName < _R.strName;});
	for (_SETTINGS_CONTROLS::iterator it = __m_Settings.vecControl.begin(); it != __m_Settings.vecControl.end(); ++it)
	{
		rapidxml::xml_node<TCHAR>* pControl = _pNode->document()->allocate_node(rapidxml::node_element, _T("control"));
		_pNode->append_node(pControl);
		rapidxml::xml_attribute<TCHAR>* pName = _pNode->document()->allocate_attribute(_T("name"), it->strName);
		pControl->append_attribute(pName);
		rapidxml::xml_attribute<TCHAR>* pClassName = _pNode->document()->allocate_attribute(_T("classname"), it->strClassName);
		pControl->append_attribute(pClassName);
		rapidxml::xml_attribute<TCHAR>* pImage = _pNode->document()->allocate_attribute(_T("image"), it->strImage);
		pControl->append_attribute(pImage);
		rapidxml::xml_attribute<TCHAR>* pParent = _pNode->document()->allocate_attribute(_T("parent"), it->strParent);
		pControl->append_attribute(pParent);
		rapidxml::xml_attribute<TCHAR>* pEnable = _pNode->document()->allocate_attribute(_T("enable"), it->bEnable ? _T("true") : _T("false"));
		pControl->append_attribute(pEnable);
		__SaveAttribs(it->vecAttrib, pControl);
	}
}

void CConfigManager::__SaveAttribs( _SETTINGS_ATTRIBS& vecAttrib, rapidxml::xml_node<TCHAR>* _pNode )
{
	//std::sort(vecAttrib.begin(), vecAttrib.end(), [](const _SETTINGS_ATTRIB& _L, const _SETTINGS_ATTRIB& _R)->bool{return _L.strName < _R.strName;});
	for (_SETTINGS_ATTRIBS::iterator it = vecAttrib.begin(); it != vecAttrib.end(); ++it)
	{
		rapidxml::xml_node<TCHAR>* pAttrib = _pNode->document()->allocate_node(rapidxml::node_element, _T("attrib"));
		_pNode->append_node(pAttrib);
		rapidxml::xml_attribute<TCHAR>* pName = _pNode->document()->allocate_attribute(_T("name"), it->strName);
		pAttrib->append_attribute(pName);
		rapidxml::xml_attribute<TCHAR>* pAlias = _pNode->document()->allocate_attribute(_T("alias"), it->strAlias);
		pAttrib->append_attribute(pAlias);
		rapidxml::xml_attribute<TCHAR>* pType = _pNode->document()->allocate_attribute(_T("type"), it->strType);
		pAttrib->append_attribute(pType);
		rapidxml::xml_attribute<TCHAR>* pDefaultValue = _pNode->document()->allocate_attribute(_T("default"), it->strDefaultValue);
		pAttrib->append_attribute(pDefaultValue);
	}
}

void CConfigManager::GetAttribs( _SETTINGS_CONTROL* _pControl, _SETTINGS_CONTROLS& _vecControls )
{
	if (!_pControl) return;
	if (!_pControl->strParent.IsEmpty()) GetAttribs(GetControl(_pControl->strParent), _vecControls);
	_SETTINGS_CONTROL sc;
	sc.bEnable = _pControl->bEnable;
	sc.strClassName = _pControl->strClassName;
	sc.strImage = _pControl->strImage;
	sc.strName = _pControl->strName;
	sc.strParent = _pControl->strParent;
	sc.vecAttrib = _pControl->vecAttrib;
	for (_SETTINGS_CONTROLS::iterator it = _vecControls.begin(); it != _vecControls.end(); ++it)
	{
		for (_SETTINGS_ATTRIBS::iterator itAttr = it->vecAttrib.begin(); itAttr != it->vecAttrib.end(); ++itAttr)
		{
			_SETTINGS_ATTRIBS::iterator itFind = std::find_if(sc.vecAttrib.begin(), sc.vecAttrib.end(), [&itAttr](const _SETTINGS_ATTRIB& _sa)->bool{return itAttr->strName == _sa.strName;});
			if (itFind == sc.vecAttrib.end()) continue;
			itAttr->strAlias = itFind->strAlias;
			itAttr->strDefaultValue = itFind->strDefaultValue;
			itAttr->strType = itFind->strType;
			sc.vecAttrib.erase(itFind);
		}
	}
	if (!_pControl->vecAttrib.empty())
		_vecControls.push_back(sc);
}

_SETTINGS_CONTROL* CConfigManager::GetControlUseClassName( LPCTSTR _lpClassName )
{
	for (_SETTINGS_CONTROLS::iterator it = __m_Settings.vecControl.begin(); it != __m_Settings.vecControl.end(); ++it)
		if (it->strClassName == _lpClassName)
			return &*it;
	return nullptr;
}
