#pragma once
#ifndef __PLUGINMANAGER_H_2015_11_03
#define __PLUGINMANAGER_H_2015_11_03

typedef void (*LPSETRESOURCEPATH)(LPCTSTR);

struct _CONTROL_PLUGIN
{
	HMODULE								hModule;
	DuiLib::CDuiString					strFile;
	DuiLib::CDuiString					strPath;
	DuiLib::LPCREATECONTROL				lpFunCreateControl;
	LPSETRESOURCEPATH					lpFunSetResourcePath;
	std::vector<DuiLib::CDuiString>		vecControls;
};

class CControlPluginManager
{
	typedef std::vector<_CONTROL_PLUGIN> VEC_PLUGIN;
public:
	~CControlPluginManager(){}
	static CControlPluginManager&	Instance();
	void							Load(LPCTSTR _lpPath, LPCTSTR _lpSkinPath);
	DuiLib::CControlUI*				CreateControl(LPCTSTR _lpStrType);
	_CONTROL_PLUGIN*				GetItem(UINT _unItem);
	UINT							GetSize();
	void							SetResourcePath(LPCTSTR _lpResourcePath);
private:
	CControlPluginManager(){}
	CControlPluginManager(const CControlPluginManager&){}
	void							__TraverseDll(LPCTSTR _lpPath, LPCTSTR _lpSkinPath);
	bool							__LoadDll(LPCTSTR _lpFile, LPCTSTR _lpPath, LPCTSTR _lpSkinPath);
private:
	VEC_PLUGIN						__m_vecPlugin;
	DuiLib::CDuiString				__m_strSkinPath;
};

typedef DuiLib::CControlUI* (*LPCREATEATTRIB)(LPCTSTR _lpType, DuiLib::CControlUI*, LPCTSTR _lpAttrib, LPCTSTR _lpName);

struct _ATTRIB_PLUGIN
{
	HMODULE								hModule;
	DuiLib::CDuiString					strFile;
	DuiLib::CDuiString					strPath;
	LPCREATEATTRIB						lpFunCreateAttrib;
	LPSETRESOURCEPATH					lpFunSetResourcePath;
	std::vector<DuiLib::CDuiString>		vecAttribs;
};

class CAttribPluginManager
{
	typedef std::vector<_ATTRIB_PLUGIN> VEC_PLUGIN;
public:
	~CAttribPluginManager(){}
	static CAttribPluginManager&	Instance();
	void							Load(LPCTSTR _lpPath, LPCTSTR _lpSkinPath);
	DuiLib::CControlUI*				CreateAttrib(LPCTSTR _lpStrType, DuiLib::CControlUI* _pControl, LPCTSTR _lpAttrib, LPCTSTR _lpName);
	_ATTRIB_PLUGIN*					GetItem(UINT _unItem);
	UINT							GetSize();
	void							SetResourcePath(LPCTSTR _lpResourcePath);
private:
	CAttribPluginManager(){}
	CAttribPluginManager(const CAttribPluginManager&){}
	void							__TraverseDll(LPCTSTR _lpPath, LPCTSTR _lpSkinPath);
	bool							__LoadDll(LPCTSTR _lpFile, LPCTSTR _lpPath, LPCTSTR _lpSkinPath);
private:
	VEC_PLUGIN						__m_vecPlugin;
};

#define plugin_control CControlPluginManager::Instance()
#define plugin_attrib CAttribPluginManager::Instance()

#endif