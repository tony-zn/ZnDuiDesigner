#include "stdafx.h"
#include "PreviewWnd.h"

CPreviewWnd::CPreviewWnd( LPCTSTR _lpXml, LPCTSTR _lpFolder )
	: __m_strXml(_lpXml)
	, __m_strFolder(_lpFolder)
{}

DuiLib::CDuiString CPreviewWnd::GetSkinFolder()
{
	return __m_strFolder.c_str();
}

DuiLib::CDuiString CPreviewWnd::GetSkinFile()
{
	return __m_strXml.c_str();
}

LPCTSTR CPreviewWnd::GetWindowClassName( void ) const
{
	return _T("DuiDesigner-Preview");
}

