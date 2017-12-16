#include "StdAfx.h"
#include "resource.h"

CMainFrame*		g_pMainFrame = nullptr;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	DuiLib::CDuiString strPath = cur_path();
	DuiLib::CDuiString strConfigPath = strPath + _T("DuiDesigner.xml");
	if (!config.Load(strConfigPath))
	{
		::MessageBox(NULL, _T("≈‰÷√Œƒº˛¥ÌŒÛ£°"), _T("DuiDesigner"), MB_OK);
		return 0;
	}
	DuiLib::CDuiString strControlPluginPath = strPath + _T("Plugin\\Controls\\");
	DuiLib::CDuiString strSkinPath = strPath + _T("Skin\\") + config.Settings().Basic.Skin.strPath;
	plugin_control.Load(strControlPluginPath, strSkinPath);
	DuiLib::CDuiString strAttribPluginPath = strPath + _T("Plugin\\Attribs\\");
	plugin_attrib.Load(strAttribPluginPath, strSkinPath);
	{
		CMainFrame dlg(strSkinPath);
		g_pMainFrame = &dlg;
		dlg.SetIcon(IDI_ICON1);
		dlg.Create(NULL, NULL, UI_WNDSTYLE_FRAME, NULL);
		dlg.CenterWindow();
		dlg.ShowModal();
		g_pMainFrame = nullptr;
	}
	
	config.Save(strConfigPath);
	return 0;
}


