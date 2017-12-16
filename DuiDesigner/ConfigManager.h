#pragma once
#ifndef __CONFIGMANAGER_H_2015_11_05
#define __CONFIGMANAGER_H_2015_11_05

struct _SETTINGS_BASIC_SKIN
{
	DuiLib::CDuiString				strPath;
};

struct _SETTINGS_BASIC
{
	_SETTINGS_BASIC_SKIN			Skin;
};

struct _SETTINGS_ATTRIB
{
	DuiLib::CDuiString				strName;
	DuiLib::CDuiString				strAlias;
	DuiLib::CDuiString				strType;
	DuiLib::CDuiString				strDefaultValue;
};

typedef std::vector<_SETTINGS_ATTRIB>	_SETTINGS_ATTRIBS;

struct _SETTINGS_CONTROL
{
	DuiLib::CDuiString				strName;
	DuiLib::CDuiString				strClassName;
	DuiLib::CDuiString				strImage;
	DuiLib::CDuiString				strParent;
	bool							bEnable;
	_SETTINGS_ATTRIBS				vecAttrib;
};

typedef std::vector<_SETTINGS_CONTROL>	_SETTINGS_CONTROLS;

struct _SETTINGS
{
	_SETTINGS_BASIC					Basic;
	_SETTINGS_CONTROLS				vecControl;
};

class CConfigManager
{
public:
	static CConfigManager&		Instance();
	bool						Load(LPCTSTR _lpFile);
	bool						Save(LPCTSTR _lpFile);
	_SETTINGS&					Settings();
	_SETTINGS_CONTROL*			GetControl(LPCTSTR _lpName);
	_SETTINGS_CONTROL*			GetControlUseClassName(LPCTSTR _lpClassName);
	_SETTINGS_CONTROL*			AddControl(LPCTSTR _lpName);
	_SETTINGS_ATTRIB*			GetAttrib(_SETTINGS_CONTROL* _pControl, LPCTSTR _lpName);
	void						GetAttribs(_SETTINGS_CONTROL* _pControl, _SETTINGS_CONTROLS& _vecControls);
private:
	CConfigManager(){}
	CConfigManager(const CConfigManager&){}
	inline DuiLib::CDuiString	__XmlStrToStr(LPCSTR _lpStr, UINT unCode = CP_UTF8);
	inline std::string			__StrToXmlStr(LPCTSTR _lpStr, UINT unCode = CP_UTF8);
	DuiLib::CDuiString			__GetAttrib(rapidxml::xml_node<>* _pNode, LPCSTR _lpName);
	void						__LoadBasic(rapidxml::xml_node<>* _pNode, _SETTINGS_BASIC& _basic);
	void						__LoadControls(rapidxml::xml_node<>* _pNode, _SETTINGS_CONTROLS& _vecControl);
	void						__LoadAttribs(rapidxml::xml_node<>* _pNode, _SETTINGS_ATTRIBS& _vecAttrib);
	void						__SaveBasic(rapidxml::xml_node<TCHAR>* _pNode);
	void						__SaveControls(rapidxml::xml_node<TCHAR>* _pNode);
	void						__SaveAttribs(_SETTINGS_ATTRIBS& vecAttrib, rapidxml::xml_node<TCHAR>* _pNode);
private:
	_SETTINGS					__m_Settings;
};

#define config CConfigManager::Instance()

#endif