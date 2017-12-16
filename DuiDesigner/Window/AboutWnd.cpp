#include "stdafx.h"
#include "AboutWnd.h"

DuiLib::CDuiString CAboutWnd::GetSkinFolder()
{
	return g_pMainFrame->GetSkinFolder();
}

DuiLib::CDuiString CAboutWnd::GetSkinFile()
{
	return _T("AboutWnd.xml");
}

LPCTSTR CAboutWnd::GetWindowClassName( void ) const
{
	return _T("AboutWnd");
}

