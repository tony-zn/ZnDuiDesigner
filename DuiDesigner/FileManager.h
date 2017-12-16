#pragma once
#ifndef __FILEMANAGER_H_2015_11_10
#define __FILEMANAGER_H_2015_11_10

#include "ControlPluginInterface.hpp"

class CFileManager
{
public:
	DuiLib::CControlUI*			Open(LPCTSTR _lpFile, DuiLib::IDialogBuilderCallback* _pCallback ,DuiLib::CPaintManagerUI* _pManager);
	bool						Save(DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pManager, LPCTSTR _lpFile);
	bool						SaveToStr(DuiLib::CControlUI* _pControl, DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pManager, _ZnString& _str, bool _bSaveDefault = false, bool _bSaveFont = false, bool _bSaveWnd = true);
	void						SetDefaultAndFont(DuiLib::CControlUI* _pControl, DuiLib::CPaintManagerUI* _pManager, int _nFontID, int _nDefaultFontID);
	void						SetDefaultAndFont(DuiLib::CControlUI* _pControl, DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pManager);
	void						DeleteAllDefaultAndFont(DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pManager);
	void						GetAttrbList(DuiLib::CControlUI* _pControl, _ZnString& _strAttrb, int _nFontID, int _nDefaultID);
private:
	inline DuiLib::CDuiString	__XmlStrToStr(LPCSTR _lpStr, UINT unCode = CP_UTF8);
	inline std::string			__StrToXmlStr(LPCTSTR _lpStr, UINT unCode = CP_UTF8);
	void						__SetAttrib(rapidxml::xml_node<TCHAR>* _pNode, DuiLib::CControlUI* _pControl, DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pPaintManager);
	void						__SetControl(DuiLib::CControlUI* _pControl, rapidxml::xml_node<TCHAR>* _pNode, DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pPaintManager);
	template<typename T>
	void						__RepairXml(rapidxml::xml_node<T>* _pNode);
	void						__HandleQuotes(std::string& str);
	void						__SaveFont(DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pManager, rapidxml::xml_node<TCHAR>* _pWndNode);
	void						__SaveDefault(DuiLib::CControlUI* _pWnd, DuiLib::CPaintManagerUI* _pManager, rapidxml::xml_node<TCHAR>* _pWndNode);
	void						__GetDefaultAttr(DuiLib::CPaintManagerUI* _pManager, _SETTINGS_CONTROL* _pControl, int _nFont, std::map<DuiLib::CDuiString, DuiLib::CDuiString>& _mapAttr);
	void						__AttrToVirAttr(_ZnString& _str, int _nFont);
	void						__VirAttrToAttr(_ZnString& _str, int _nFont);
	void						__SetAttrib(rapidxml::xml_node<TCHAR>* _pNode, LPCTSTR _lpName, LPCTSTR _lpValue);
};

#endif