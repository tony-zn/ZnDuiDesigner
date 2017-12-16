#pragma once
#ifndef __PREVIEW_H_2015_11_04
#define __PREVIEW_H_2015_11_04

class CPreviewWnd : public DuiLib::WindowImplBase
{
public:
	CPreviewWnd(LPCTSTR _lpXml, LPCTSTR _lpFolder);
	DuiLib::CDuiString			GetSkinFolder();
	DuiLib::CDuiString			GetSkinFile();
	LPCTSTR						GetWindowClassName(void) const;
private:
	_ZnString					__m_strFolder;
	_ZnString					__m_strXml;
};

#endif