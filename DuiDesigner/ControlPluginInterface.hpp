#pragma once
#ifndef __CONTROLPLUGININTERFACE_HPP_2015_11_06
#define __CONTROLPLUGININTERFACE_HPP_2015_11_06

#define DUI_CTR_CONTROL_PLUGIN_BASE _T("ControlPluginBase")
#define DUI_CTR_WINDOW_SHARED_DATA _T("Windo// SharedData")
// 当属性插件值发生变化后发送该消息
#define DUI_MSGTYPE_ATTRIBCHANGE	_T("AttribChange")

// 自定义的控件继承 IControlPlugin写上属性的获取功能
// 如果无法获取IControlPlugin 则调用 CDefaultControlAttribUI 来获取

#include <vector>
#include <algorithm>

struct IWindowSharedData
{
public:
	~IWindowSharedData(){}
	virtual void	SetSharedFont(int _nFont, bool _bShared)
	{
		if (_bShared)
		{
			if (!IsSharedFont(_nFont)) 
				__m_vecSharedFont.push_back(_nFont);
		}
		else
			__m_vecSharedFont.erase(std::remove(__m_vecSharedFont.begin(), __m_vecSharedFont.end(), _nFont), __m_vecSharedFont.end());
	}
	virtual bool	IsSharedFont(int _nFont)
	{
		return std::find(__m_vecSharedFont.begin(), __m_vecSharedFont.end(), _nFont) != __m_vecSharedFont.end();
	}
	virtual void	SetSharedDefault(LPCTSTR _lpName, bool _bShared)
	{
		if (_bShared)
		{
			if (!IsSharedDefault(_lpName)) 
				__m_vecSharedDefault.push_back(DuiLib::CDuiString(_lpName));
		}
		else
			__m_vecSharedDefault.erase(std::remove(__m_vecSharedDefault.begin(), __m_vecSharedDefault.end(), _lpName), __m_vecSharedDefault.end());
	}
	virtual bool	IsSharedDefault(LPCTSTR _lpName)
	{
		return std::find(__m_vecSharedDefault.begin(), __m_vecSharedDefault.end(), _lpName) != __m_vecSharedDefault.end();
	}
private:
	std::vector<int>					__m_vecSharedFont;
	std::vector<DuiLib::CDuiString>		__m_vecSharedDefault;
};

struct IControlPlugin
{
	virtual DuiLib::CDuiString GetAttribute(LPCTSTR pstrName) = 0;
};

class CControlPlugin_ControlUI : public IControlPlugin
{
public:
	CControlPlugin_ControlUI(){}
	~CControlPlugin_ControlUI(){}
	inline void SetControl(DuiLib::CControlUI* _pControl)
	{
		__m_pControl = _pControl;
	}
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CDuiString strRet;
		if( _tcscmp(pstrName, _T("pos")) == 0 ) 
		{
			if (__m_pControl->IsFloat())
			{
				SIZE szXY = __m_pControl->GetFixedXY();
				strRet.Format(_T("%d,%d,%d,%d"), szXY.cx, szXY.cy, szXY.cx + __m_pControl->GetFixedWidth(), szXY.cy + __m_pControl->GetFixedHeight());
			}
			else
				strRet.Format(_T("%d,%d,%d,%d"), 0, 0, __m_pControl->GetFixedWidth(), __m_pControl->GetFixedHeight());
		}
		else if( _tcscmp(pstrName, _T("padding")) == 0 ) 
		{
			RECT rect = __m_pControl->GetPadding();
			strRet.Format(_T("%d,%d,%d,%d"), rect.left, rect.top, rect.right, rect.bottom);
		}
		else if( _tcscmp(pstrName, _T("bkcolor")) == 0 || _tcscmp(pstrName, _T("bkcolor1")) == 0 ) 
			strRet.Format(_T("#%08X"), __m_pControl->GetBkColor());
		else if( _tcscmp(pstrName, _T("bkcolor2")) == 0 ) 
			strRet.Format(_T("#%08X"), __m_pControl->GetBkColor2());
		else if( _tcscmp(pstrName, _T("bkcolor3")) == 0 ) 
			strRet.Format(_T("#%08X"), __m_pControl->GetBkColor3());
		else if( _tcscmp(pstrName, _T("bordercolor")) == 0 ) 
			strRet.Format(_T("#%08X"), __m_pControl->GetBorderColor());
		else if( _tcscmp(pstrName, _T("focusbordercolor")) == 0 ) 
			strRet.Format(_T("#%08X"), __m_pControl->GetFocusBorderColor());
		else if( _tcscmp(pstrName, _T("colorhsl")) == 0 ) 
			return __m_pControl->IsColorHSL() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("bordersize")) == 0 ) 
			strRet.Format(_T("%d"), __m_pControl->GetBorderSize());
		else if( _tcscmp(pstrName, _T("leftbordersize")) == 0 )
			strRet.Format(_T("%d"), __m_pControl->GetLeftBorderSize());
		else if( _tcscmp(pstrName, _T("topbordersize")) == 0 )
			strRet.Format(_T("%d"), __m_pControl->GetTopBorderSize());
		else if( _tcscmp(pstrName, _T("rightbordersize")) == 0 )
			strRet.Format(_T("%d"), __m_pControl->GetRightBorderSize());
		else if( _tcscmp(pstrName, _T("bottombordersize")) == 0 )
			strRet.Format(_T("%d"), __m_pControl->GetBottomBorderSize());
		else if( _tcscmp(pstrName, _T("borderstyle")) == 0 )
			strRet.Format(_T("%d"), __m_pControl->GetBorderStyle());
		else if( _tcscmp(pstrName, _T("borderround")) == 0 ) 
		{
			SIZE sz = __m_pControl->GetBorderRound();
			strRet.Format(_T("%d,%d"), sz.cx, sz.cy);
		}
		else if( _tcscmp(pstrName, _T("bkimage")) == 0 ) 
			return __m_pControl->GetBkImage();
		else if( _tcscmp(pstrName, _T("width")) == 0 )
			strRet.Format(_T("%d"), __m_pControl->GetFixedWidth());
		else if( _tcscmp(pstrName, _T("height")) == 0 )
			strRet.Format(_T("%d"), __m_pControl->GetFixedHeight());
		else if( _tcscmp(pstrName, _T("minwidth")) == 0 )
			strRet.Format(_T("%d"), __m_pControl->GetMinWidth());
		else if( _tcscmp(pstrName, _T("minheight")) == 0 )
			strRet.Format(_T("%d"), __m_pControl->GetMinHeight());
		else if( _tcscmp(pstrName, _T("maxwidth")) == 0 )
			strRet.Format(_T("%d"), __m_pControl->GetMaxWidth());
		else if( _tcscmp(pstrName, _T("maxheight")) == 0 )
			strRet.Format(_T("%d"), __m_pControl->GetMaxHeight());
		else if( _tcscmp(pstrName, _T("name")) == 0 )
			return __m_pControl->GetName();
		else if( _tcscmp(pstrName, _T("text")) == 0 )
			return __m_pControl->GetText();
		else if( _tcscmp(pstrName, _T("tooltip")) == 0 )
			return __m_pControl->GetToolTip();
		else if( _tcscmp(pstrName, _T("userdata")) == 0 )
			return __m_pControl->GetUserData();
		else if( _tcscmp(pstrName, _T("enabled")) == 0 )
			return __m_pControl->IsEnabled() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("mouse")) == 0 )
			return __m_pControl->IsMouseEnabled() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("keyboard")) == 0 )
			return __m_pControl->IsKeyboardEnabled() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("visible")) == 0 )
			return __m_pControl->IsVisible() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("float")) == 0 ) 
			return __m_pControl->IsFloat() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("shortcut")) == 0 )
			return __m_pControl->GetShortcut();
		else if( _tcscmp(pstrName, _T("menu")) == 0 )
			return __m_pControl->IsContextMenuUsed() ? _T("true") : _T("false");
		return strRet;
	}
protected:
	DuiLib::CControlUI*		__m_pControl;
};

class CControlPlugin_ContainerUI : public CControlPlugin_ControlUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CContainerUI* pContainerUI = static_cast<DuiLib::CContainerUI*>(__m_pControl->GetInterface(DUI_CTR_CONTAINER));
		if (!pContainerUI) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
		if( _tcscmp(pstrName, _T("inset")) == 0 ) 
		{
			RECT rect = pContainerUI->GetInset();
			strRet.Format(_T("%d,%d,%d,%d"), rect.left, rect.top, rect.right, rect.bottom);
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("mousechild")) == 0 ) 
			return pContainerUI->IsMouseChildEnabled() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("vscrollbar")) == 0 ) 
			return pContainerUI->GetVerticalScrollBar() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("vscrollbarstyle")) == 0 ) 
			return strRet;
		else if( _tcscmp(pstrName, _T("hscrollbar")) == 0 )
			return pContainerUI->GetHorizontalScrollBar() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("hscrollbarstyle")) == 0 ) 
			return strRet;
		else if( _tcscmp(pstrName, _T("childpadding")) == 0 ) 
		{
			strRet.Format(_T("%d"), pContainerUI->GetChildPadding());
			return strRet;
		}
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_ChildLayoutUI : public CControlPlugin_ContainerUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CChildLayoutUI* pControl = static_cast<DuiLib::CChildLayoutUI*>(__m_pControl->GetInterface(DUI_CTR_CHILDLAYOUT));
		if (!pControl) return __super::GetAttribute(pstrName);
		if( _tcscmp(pstrName, _T("xmlfile")) == 0 )
			return pControl->GetChildLayoutXML();
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_VerticalLayoutUI : public CControlPlugin_ContainerUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CVerticalLayoutUI* pControl = static_cast<DuiLib::CVerticalLayoutUI*>(__m_pControl->GetInterface(DUI_CTR_VERTICALLAYOUT));
		if (!pControl) return __super::GetAttribute(pstrName);
		if( _tcscmp(pstrName, _T("sepheight")) == 0 )
		{
			DuiLib::CDuiString strRet;
			strRet.Format(_T("%d"), pControl->GetSepHeight());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("sepimm")) == 0 )
			return pControl->IsSepImmMode() ? _T("true") : _T("false");
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_HorizontalLayoutUI : public CControlPlugin_ContainerUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CHorizontalLayoutUI* pControl = static_cast<DuiLib::CHorizontalLayoutUI*>(__m_pControl->GetInterface(DUI_CTR_HORIZONTALLAYOUT));
		if (!pControl) return __super::GetAttribute(pstrName);
		if( _tcscmp(pstrName, _T("sepwidth")) == 0 )
		{
			DuiLib::CDuiString strRet;
			strRet.Format(_T("%d"), pControl->GetSepWidth());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("sepimm")) == 0 )
			return pControl->IsSepImmMode() ? _T("true") : _T("false");
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_TileLayoutUI : public CControlPlugin_ContainerUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CTileLayoutUI* pControl = static_cast<DuiLib::CTileLayoutUI*>(__m_pControl->GetInterface(DUI_CTR_TILELAYOUT));
		if (!pControl) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
		if( _tcscmp(pstrName, _T("itemsize")) == 0 )
		{
			SIZE sz = pControl->GetItemSize();
			strRet.Format(_T("%d,%d"), sz.cx, sz.cy);
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("columns")) == 0 )
		{
			strRet.Format(_T("%d"), pControl->GetColumns());
			return strRet;
		}
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_TabLayoutUI : public CControlPlugin_ContainerUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CTabLayoutUI* pControl = static_cast<DuiLib::CTabLayoutUI*>(__m_pControl->GetInterface(DUI_CTR_TABLAYOUT));
		if (!pControl) return __super::GetAttribute(pstrName);
		if( _tcscmp(pstrName, _T("selectedid")) == 0 )
		{
			DuiLib::CDuiString strRet;
			strRet.Format(_T("%d"), pControl->GetCurSel());
			return strRet;
		}
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_ActiveXUI : public CControlPlugin_ControlUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CActiveXUI* pControl = static_cast<DuiLib::CActiveXUI*>(__m_pControl->GetInterface(DUI_CTR_ACTIVEX));
		if (!pControl) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
		if( _tcscmp(pstrName, _T("clsid")) == 0 )
		{
			LPOLESTR lpStr = nullptr;
			::StringFromCLSID(pControl->GetClisd(), &lpStr);
			if (lpStr) strRet.Format(_T("%s"), lpStr);
			return strRet;
		}
		if( _tcscmp(pstrName, _T("modulename")) == 0 )
			return pControl->GetModuleName();
		if( _tcscmp(pstrName, _T("delaycreate")) == 0 )
			return pControl->IsDelayCreate() ? _T("true") : _T("false");
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_WebBrowserUI : public CControlPlugin_ActiveXUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CWebBrowserUI* pControl = static_cast<DuiLib::CWebBrowserUI*>(__m_pControl->GetInterface(DUI_CTR_WEBBROWSER));
		if (!pControl) return __super::GetAttribute(pstrName);
		if( _tcscmp(pstrName, _T("homepage")) == 0 )
			return pControl->GetHomePage();
		else if( _tcscmp(pstrName, _T("autonavi")) == 0 )
			return pControl->IsAutoNavigation() ? _T("true") : _T("false");
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_ComboUI : public CControlPlugin_ContainerUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CComboUI* pControl = static_cast<DuiLib::CComboUI*>(__m_pControl->GetInterface(DUI_CTR_COMBO));
		if (!pControl) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
		if( _tcscmp(pstrName, _T("textpadding")) == 0 ) {
			RECT rc = pControl->GetTextPadding();
			strRet.Format(_T("%d,%d,%d,%d"), rc.left, rc.top, rc.right, rc.bottom);
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("showtext")) == 0 ) 
			return pControl->GetShowText() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("normalimage")) == 0 ) 
			return pControl->GetNormalImage();
		else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) 
			return pControl->GetHotImage();
		else if( _tcscmp(pstrName, _T("pushedimage")) == 0 ) 
			return pControl->GetPushedImage();
		else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) 
			return pControl->GetFocusedImage();
		else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) 
			return pControl->GetDisabledImage();
		else if( _tcscmp(pstrName, _T("dropbox")) == 0 ) 
			return pControl->GetDropBoxAttributeList();
		else if( _tcscmp(pstrName, _T("dropboxsize")) == 0)
		{
			SIZE sz = pControl->GetDropBoxSize();
			strRet.Format(_T("%d,%d"), sz.cx, sz.cy);
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemfont")) == 0 ) 
		{
			strRet.Format(_T("%d"), pControl->GetListInfo()->nFont);
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemalign")) == 0 ) 
		{
			UINT unStyle = pControl->GetListInfo()->uTextStyle;
			if (unStyle & DT_LEFT) return _T("left");
			else if (unStyle & DT_RIGHT) return _T("right");
			else if (unStyle & DT_CENTER) return _T("center");
			return _T("");
		}
		if( _tcscmp(pstrName, _T("itemtextpadding")) == 0 ) {
			RECT rc = pControl->GetItemTextPadding();
			strRet.Format(_T("%d,%d,%d,%d"), rc.left, rc.top, rc.right, rc.bottom);
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemtextcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetItemTextColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itembkcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetItemBkColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itembkimage")) == 0 ) 
			return pControl->GetItemBkImage();
		else if( _tcscmp(pstrName, _T("itemaltbk")) == 0 ) 
			return pControl->IsAlternateBk() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("itemselectedtextcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetSelectedItemTextColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemselectedbkcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetSelectedItemBkColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemselectedimage")) == 0 ) 
			return pControl->GetSelectedItemImage();
		else if( _tcscmp(pstrName, _T("itemhottextcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetHotItemTextColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemhotbkcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetHotItemBkColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemhotimage")) == 0 ) 
			return pControl->GetHotItemImage();
		else if( _tcscmp(pstrName, _T("itemdisabledtextcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetDisabledItemTextColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemdisabledbkcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetDisabledItemBkColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemdisabledimage")) == 0 ) 
			return pControl->GetDisabledItemImage();
		else if( _tcscmp(pstrName, _T("itemlinecolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetItemLineColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemshowhtml")) == 0 ) 
			return pControl->IsItemShowHtml() ? _T("true") : _T("false");
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_LabelUI : public CControlPlugin_ControlUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CLabelUI* pControl = static_cast<DuiLib::CLabelUI*>(__m_pControl->GetInterface(DUI_CTR_LABEL));
		if (!pControl) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
		if( _tcscmp(pstrName, _T("align")) == 0 ) {
			UINT unStyle = pControl->GetTextStyle();
			if (unStyle & DT_LEFT) return _T("left");
			else if (unStyle & DT_RIGHT) return _T("right");
			else if (unStyle & DT_CENTER) return _T("center");
			return _T("");
		}
		else if (_tcscmp(pstrName, _T("valign")) == 0)
		{
			UINT unStyle = pControl->GetTextStyle();
			if (unStyle & DT_TOP) return _T("top");
			else if (unStyle & DT_BOTTOM) return _T("bottom");
			else if (unStyle & DT_VCENTER) return _T("vcenter");
			return _T("");
		}
		else if( _tcscmp(pstrName, _T("endellipsis")) == 0 ) {
			UINT unStyle = pControl->GetTextStyle();
			return (unStyle & DT_END_ELLIPSIS) ? _T("true") : _T("false");
		}    
		else if( _tcscmp(pstrName, _T("font")) == 0 ) 
		{
			strRet.Format(_T("%d"), pControl->GetFont());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("textcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetTextColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("disabledtextcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetDisabledTextColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("textpadding")) == 0 ) {
			RECT rc = pControl->GetTextPadding();
			strRet.Format(_T("%d,%d,%d,%d"), rc.left, rc.top, rc.right, rc.bottom);
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("showhtml")) == 0 ) 
			return pControl->IsShowHtml() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("enabledeffect")) == 0 ) 
			return pControl->GetEnabledEffect() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("enabledluminous")) == 0 ) 
			return pControl->GetEnabledLuminous() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("luminousfuzzy")) == 0 ) 
		{
			strRet.Format(_T("%.2f"), pControl->GetLuminousFuzzy());
			return strRet;
		}
// 		else if( _tcscmp(pstrName, _T("rhaa")) == 0 ) 
// 		{
// 			strRet.Format(_T("%d"), pControl->GetTextRenderingHintAntiAlias());
// 			return strRet;
// 		}
// 		else if( _tcscmp(pstrName, _T("transshadow")) == 0 ) 
// 		{
// 			strRet.Format(_T("%d"), pControl->GetTransShadow());
// 			return strRet;
// 		}
// 		else if( _tcscmp(pstrName, _T("transtext")) == 0 ) 
// 		{
// 			strRet.Format(_T("%d"), pControl->GetTransText());
// 			return strRet;
// 		}
// 		else if( _tcscmp(pstrName, _T("transshadow1")) == 0 ) 
// 		{
// 			strRet.Format(_T("%d"), pControl->GetTransShadow1());
// 			return strRet;
// 		}
// 		else if( _tcscmp(pstrName, _T("transtext1")) == 0 ) 
// 		{
// 			strRet.Format(_T("%d"), pControl->GetTransText1());
// 			return strRet;
// 		}
		else if( _tcscmp(pstrName, _T("gradientangle")) == 0 ) 
		{
			strRet.Format(_T("%d"), pControl->GetGradientAngle());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("enabledstroke")) == 0 ) 
			return pControl->GetEnabledStroke() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("enabledshadow")) == 0 ) 
			return pControl->GetEnabledShadow() ? _T("true") : _T("false");
// 		else if( _tcscmp(pstrName, _T("transstroke")) == 0 )
// 		{
// 			strRet.Format(_T("%d"), pControl->GetTransStroke());
// 			return strRet;
// 		}
		else if( _tcscmp(pstrName, _T("gradientlength")) == 0 ) 
		{
			strRet.Format(_T("%d"), pControl->GetGradientLength());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("shadowoffset")) == 0 )
		{
			RectF rc = pControl->GetShadowOffset();
			strRet.Format(_T("%d,%d"), rc.X, rc.Y);
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("textcolor1")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetTextColor1());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("textshadowcolora")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetTextShadowColorA());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("textshadowcolorb")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetTextShadowColorB());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("strokecolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetStrokeColor());
			return strRet;
		}
		else 
			return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_DateTimeUI : public CControlPlugin_LabelUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_ButtonUI : public CControlPlugin_LabelUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CButtonUI* pControl = static_cast<DuiLib::CButtonUI*>(__m_pControl->GetInterface(DUI_CTR_BUTTON));
		if (!pControl) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
		if( _tcscmp(pstrName, _T("normalimage")) == 0 ) 
			return pControl->GetNormalImage();
		else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) 
			return pControl->GetHotImage();
		else if( _tcscmp(pstrName, _T("pushedimage")) == 0 ) 
			return pControl->GetPushedImage();
		else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) 
			return pControl->GetFocusedImage();
		else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) 
			return pControl->GetDisabledImage();
		else if( _tcscmp(pstrName, _T("foreimage")) == 0 ) 
			return pControl->GetForeImage();
		else if( _tcscmp(pstrName, _T("hotforeimage")) == 0 ) 
			return pControl->GetHotForeImage();
		else if( _tcscmp(pstrName, _T("hotbkcolor")) == 0 )
		{
			strRet.Format(_T("#%08X"), pControl->GetHotBkColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("hottextcolor")) == 0 )
		{
			strRet.Format(_T("#%08X"), pControl->GetHotTextColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("pushedtextcolor")) == 0 )
		{
			strRet.Format(_T("#%08X"), pControl->GetPushedTextColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("focusedtextcolor")) == 0 )
		{
			strRet.Format(_T("#%08X"), pControl->GetFocusedTextColor());
			return strRet;
		}
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_OptionUI : public CControlPlugin_ButtonUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::COptionUI* pControl = static_cast<DuiLib::COptionUI*>(__m_pControl->GetInterface(DUI_CTR_OPTION));
		if (!pControl) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
		if( _tcscmp(pstrName, _T("group")) == 0 ) 
			return pControl->GetGroup();
		else if( _tcscmp(pstrName, _T("selected")) == 0 ) 
			return pControl->IsSelected() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("selectedimage")) == 0 ) 
			return pControl->GetSelectedImage();
		else if( _tcscmp(pstrName, _T("selectedhotimage")) == 0 ) 
			return pControl->GetSelectedHotImage();
		else if( _tcscmp(pstrName, _T("foreimage")) == 0 ) 
			return pControl->GetForeImage();
		else if( _tcscmp(pstrName, _T("selectedbkcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetSelectBkColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("selectedtextcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetSelectedTextColor());
			return strRet;
		}
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_CheckBoxUI : public CControlPlugin_OptionUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_TextUI : public CControlPlugin_LabelUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_ProgressUI : public CControlPlugin_LabelUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CProgressUI* pControl = static_cast<DuiLib::CProgressUI*>(__m_pControl->GetInterface(DUI_CTR_PROGRESS));
		if (!pControl) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
		if( _tcscmp(pstrName, _T("foreimage")) == 0 ) 
			return pControl->GetForeImage();
		else if( _tcscmp(pstrName, _T("hor")) == 0 ) 
			return pControl->IsHorizontal() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("min")) == 0 ) 
		{
			strRet.Format(_T("%d"), pControl->GetMinValue());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("max")) == 0 ) 
		{
			strRet.Format(_T("%d"), pControl->GetMaxValue());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("value")) == 0 ) 
		{
			strRet.Format(_T("%d"), pControl->GetValue());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("isstretchfore"))==0) 
			return pControl->IsStretchForeImage() ? _T("true") : _T("false");
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_SliderUI : public CControlPlugin_ProgressUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CSliderUI* pControl = static_cast<DuiLib::CSliderUI*>(__m_pControl->GetInterface(DUI_CTR_SLIDER));
		if (!pControl) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
		if( _tcscmp(pstrName, _T("thumbimage")) == 0 ) 
			return pControl->GetThumbImage();
		else if( _tcscmp(pstrName, _T("thumbhotimage")) == 0 ) 
			return pControl->GetThumbHotImage();
		else if( _tcscmp(pstrName, _T("thumbpushedimage")) == 0 ) 
			return pControl->GetThumbPushedImage();
		else if( _tcscmp(pstrName, _T("thumbsize")) == 0 ) {
			RECT rc = pControl->GetThumbRect();
			strRet.Format(_T("%d,%d"), rc.right - rc.left, rc.bottom - rc.top);
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("step")) == 0 ) {
			strRet.Format(_T("%d"), pControl->GetChangeStep());
			return strRet;
		}
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_EditUI : public CControlPlugin_LabelUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CEditUI* pControl = static_cast<DuiLib::CEditUI*>(__m_pControl->GetInterface(DUI_CTR_EDIT));
		if (!pControl) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
		if( _tcscmp(pstrName, _T("readonly")) == 0 ) 
			return pControl->IsReadOnly() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("numberonly")) == 0 ) 
			return pControl->IsNumberOnly() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("password")) == 0 ) 
			return pControl->IsPasswordMode() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("maxchar")) == 0 ) 
		{
			strRet.Format(_T("%d"), pControl->GetMaxChar());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("normalimage")) == 0 ) 
			return pControl->GetNormalImage();
		else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) 
			return pControl->GetHotImage();
		else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) 
			return pControl->GetFocusedImage();
		else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) 
			return pControl->GetDisabledImage();
		else if( _tcscmp(pstrName, _T("nativebkcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetNativeEditBkColor());
			return strRet;
		}
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_ScrollBarUI : public CControlPlugin_ControlUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CScrollBarUI* pControl = static_cast<DuiLib::CScrollBarUI*>(__m_pControl->GetInterface(DUI_CTR_SCROLLBAR));
		if (!pControl) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
 		if( _tcscmp(pstrName, _T("button1color")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetButton1Color());
			return strRet;
 		}
 		else if( _tcscmp(pstrName, _T("button1normalimage")) == 0 ) 
			return pControl->GetButton1NormalImage();
 		else if( _tcscmp(pstrName, _T("button1hotimage")) == 0 ) 
			return pControl->GetButton1HotImage();
 		else if( _tcscmp(pstrName, _T("button1pushedimage")) == 0 ) 
			return pControl->GetButton1PushedImage();
 		else if( _tcscmp(pstrName, _T("button1disabledimage")) == 0 ) 
			return pControl->GetButton1DisabledImage();
 		else if( _tcscmp(pstrName, _T("button2color")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetButton2Color());
			return strRet;
 		}
 		else if( _tcscmp(pstrName, _T("button2normalimage")) == 0 ) 
			return pControl->GetButton2NormalImage();
 		else if( _tcscmp(pstrName, _T("button2hotimage")) == 0 ) 
			return pControl->GetButton2HotImage();
 		else if( _tcscmp(pstrName, _T("button2pushedimage")) == 0 ) 
			return pControl->GetButton2PushedImage();
 		else if( _tcscmp(pstrName, _T("button2disabledimage")) == 0 ) 
			return pControl->GetButton2DisabledImage();
 		else if( _tcscmp(pstrName, _T("thumbcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetThumbColor());
			return strRet;
 		}
 		else if( _tcscmp(pstrName, _T("thumbnormalimage")) == 0 ) 
			return pControl->GetThumbNormalImage();
 		else if( _tcscmp(pstrName, _T("thumbhotimage")) == 0 ) 
			return pControl->GetThumbHotImage();
 		else if( _tcscmp(pstrName, _T("thumbpushedimage")) == 0 ) 
			return pControl->GetThumbPushedImage();
 		else if( _tcscmp(pstrName, _T("thumbdisabledimage")) == 0 ) 
			return pControl->GetThumbDisabledImage();
 		else if( _tcscmp(pstrName, _T("railnormalimage")) == 0 ) 
			return pControl->GetRailNormalImage();
 		else if( _tcscmp(pstrName, _T("railhotimage")) == 0 ) 
			return pControl->GetRailHotImage();
 		else if( _tcscmp(pstrName, _T("railpushedimage")) == 0 ) 
			return pControl->GetRailPushedImage();
 		else if( _tcscmp(pstrName, _T("raildisabledimage")) == 0 ) 
			return pControl->GetRailDisabledImage();
 		else if( _tcscmp(pstrName, _T("bknormalimage")) == 0 ) 
			return pControl->GetBkNormalImage();
 		else if( _tcscmp(pstrName, _T("bkhotimage")) == 0 ) 
			return pControl->GetBkHotImage();
 		else if( _tcscmp(pstrName, _T("bkpushedimage")) == 0 ) 
			return pControl->GetBkPushedImage();
 		else if( _tcscmp(pstrName, _T("bkdisabledimage")) == 0 ) 
			return pControl->GetBkDisabledImage();
 		else if( _tcscmp(pstrName, _T("hor")) == 0 ) 
			return pControl->IsHorizontal() ? _T("true") : _T("false");
 		else if( _tcscmp(pstrName, _T("linesize")) == 0 ) 
		{
			strRet.Format(_T("%d"), pControl->GetLineSize());
			return strRet;
		}
 		else if( _tcscmp(pstrName, _T("range")) == 0 ) 
		{
			strRet.Format(_T("%d"), pControl->GetScrollRange());
			return strRet;
		}
 		else if( _tcscmp(pstrName, _T("value")) == 0 ) 
		{
			strRet.Format(_T("%d"), pControl->GetScrollPos());
			return strRet;
		}
 		else if( _tcscmp(pstrName, _T("showbutton1")) == 0 ) 
			return pControl->GetShowButton1() ? _T("true") : _T("false");
 		else if( _tcscmp(pstrName, _T("showbutton2")) == 0 ) 
			return pControl->GetShowButton2() ? _T("true") : _T("false");
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_ListUI : public CControlPlugin_VerticalLayoutUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CListUI* pControl = static_cast<DuiLib::CListUI*>(__m_pControl->GetInterface(DUI_CTR_LIST));
		if (!pControl) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
		if( _tcscmp(pstrName, _T("header")) == 0 ) 
			return pControl->GetHeader()->IsVisible() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("headerbkimage")) == 0 ) 
			return pControl->GetHeader()->GetBkImage();
		else if( _tcscmp(pstrName, _T("scrollselect")) == 0 ) 
			return pControl->GetScrollSelect() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("multiexpanding")) == 0 ) 
			return pControl->GetListInfo()->bMultiExpandable ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("itemfont")) == 0 ) 
		{
			strRet.Format(_T("%d"), pControl->GetListInfo()->nFont);
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemalign")) == 0 ) {
			UINT unStyle = pControl->GetListInfo()->uTextStyle;
			if (unStyle & DT_LEFT) return _T("left");
			else if (unStyle & DT_RIGHT) return _T("right");
			else if (unStyle & DT_CENTER) return _T("center");
			return _T("");
		}
		else if( _tcscmp(pstrName, _T("itemendellipsis")) == 0 ) {
			return (pControl->GetListInfo()->uTextStyle & DT_END_ELLIPSIS) ? _T("true") : _T("false");
		}    
		if( _tcscmp(pstrName, _T("itemtextpadding")) == 0 ) {
			RECT rc = pControl->GetItemTextPadding();
			strRet.Format(_T("%d,%d,%d,%d"), rc.left, rc.top, rc.right, rc.bottom);
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemtextcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetItemTextColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itembkcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetItemBkColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itembkimage")) == 0 ) 
			return pControl->GetItemBkImage();
		else if( _tcscmp(pstrName, _T("itemaltbk")) == 0 ) 
			return pControl->IsAlternateBk() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("itemselectedtextcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetSelectedItemTextColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemselectedbkcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetSelectedItemBkColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemselectedimage")) == 0 ) 
			return pControl->GetSelectedItemImage();
		else if( _tcscmp(pstrName, _T("itemhottextcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetHotItemTextColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemhotbkcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetHotItemBkColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemhotimage")) == 0 ) 
			return pControl->GetHotItemImage();
		else if( _tcscmp(pstrName, _T("itemdisabledtextcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetDisabledItemTextColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemdisabledbkcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetDisabledItemBkColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemdisabledimage")) == 0 ) 
			return pControl->GetDisabledItemImage();
		else if( _tcscmp(pstrName, _T("itemlinecolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetItemLineColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("itemshowhtml")) == 0 ) 
			return pControl->IsItemShowHtml() ? _T("true") : _T("false");
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_ListHeaderUI : public CControlPlugin_HorizontalLayoutUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_ListHeaderItemUI : public CControlPlugin_ControlUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CListHeaderItemUI* pControl = static_cast<DuiLib::CListHeaderItemUI*>(__m_pControl->GetInterface(DUI_CTR_LISTHEADERITEM));
		if (!pControl) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
		if( _tcscmp(pstrName, _T("dragable")) == 0 ) 
			return pControl->IsDragable() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("sepwidth")) == 0 ) 
		{
			strRet.Format(_T("%d"), pControl->GetSepWidth());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("align")) == 0 ) {
			UINT unStyle = pControl->GetTextStyle();
			if (unStyle & DT_LEFT) return _T("left");
			else if (unStyle & DT_RIGHT) return _T("right");
			else if (unStyle & DT_CENTER) return _T("center");
			return _T("");
		}
		else if( _tcscmp(pstrName, _T("endellipsis")) == 0 ) 
			return (pControl->GetTextStyle() & DT_END_ELLIPSIS) ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("font")) == 0 ) 
		{
			//strRet.Format(_T("%d"), pControl->GetFont());
			// 没有GetFont()接口
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("textcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetTextColor());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("textpadding")) == 0 ) {
			RECT rc = pControl->GetTextPadding();
			strRet.Format(_T("%d,%d,%d,%d"), rc.left, rc.top, rc.right, rc.bottom);
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("showhtml")) == 0 ) 
			return pControl->IsShowHtml() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("normalimage")) == 0 ) 
			return pControl->GetNormalImage();
		else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) 
			return pControl->GetHotImage();
		else if( _tcscmp(pstrName, _T("pushedimage")) == 0 ) 
			return pControl->GetPushedImage();
		else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) 
			return pControl->GetFocusedImage();
		else if( _tcscmp(pstrName, _T("sepimage")) == 0 ) 
			return pControl->GetSepImage();
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_ListLabelElementUI : public CControlPlugin_ControlUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CListLabelElementUI* pControl = static_cast<DuiLib::CListLabelElementUI*>(__m_pControl->GetInterface(DUI_CTR_LISTLABELELEMENT));
		if (!pControl) return __super::GetAttribute(pstrName);
		if( _tcscmp(pstrName, _T("selected")) == 0 )
			return pControl->IsSelected() ? _T("true") : _T("false");
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_ListTextElementUI : public CControlPlugin_ListLabelElementUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_ListContainerElementUI : public CControlPlugin_ContainerUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CListContainerElementUI* pControl = static_cast<DuiLib::CListContainerElementUI*>(__m_pControl->GetInterface(DUI_CTR_LISTCONTAINERELEMENT));
		if (!pControl) return __super::GetAttribute(pstrName);
		if( _tcscmp(pstrName, _T("selected")) == 0 )
			return pControl->IsSelected() ? _T("true") : _T("false");
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_RichEditUI : public CControlPlugin_ContainerUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CRichEditUI* pControl = static_cast<DuiLib::CRichEditUI*>(__m_pControl->GetInterface(DUI_CTR_RICHEDIT));
		if (!pControl) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
		if( _tcscmp(pstrName, _T("vscrollbar")) == 0 ) 
			return (pControl->GetWinStyle() & (ES_DISABLENOSCROLL | WS_VSCROLL)) ? _T("true") : _T("false");
		if( _tcscmp(pstrName, _T("autovscroll")) == 0 ) 
			return (pControl->GetWinStyle() & ES_AUTOVSCROLL) ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("hscrollbar")) == 0 ) 
			return (pControl->GetWinStyle() & (ES_DISABLENOSCROLL | WS_HSCROLL)) ? _T("true") : _T("false");
		if( _tcscmp(pstrName, _T("autohscroll")) == 0 ) 
			return (pControl->GetWinStyle() & ES_AUTOHSCROLL) ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("wanttab")) == 0 ) 
			return pControl->IsWantTab() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("wantreturn")) == 0 ) 
			return pControl->IsWantReturn() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("wantctrlreturn")) == 0 ) 
			return pControl->IsWantCtrlReturn() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("rich")) == 0 ) 
			return pControl->IsRich() ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("multiline")) == 0 ) 
			return (pControl->GetWinStyle() & ES_MULTILINE) ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("readonly")) == 0 ) 
			return (pControl->GetWinStyle() & ES_READONLY) ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("password")) == 0 ) 
			return (pControl->GetWinStyle() & ES_PASSWORD) ? _T("true") : _T("false");
		else if( _tcscmp(pstrName, _T("align")) == 0 ) 
		{
			UINT unStyle = pControl->GetWinStyle();
			if (unStyle & ES_LEFT) return _T("left");
			else if (unStyle & ES_RIGHT) return _T("right");
			else if (unStyle & ES_CENTER) return _T("center");
			return _T("");
		}
		else if( _tcscmp(pstrName, _T("font")) == 0 ) 
		{
			strRet.Format(_T("%d"), pControl->GetFont());
			return strRet;
		}
		else if( _tcscmp(pstrName, _T("textcolor")) == 0 ) {
			strRet.Format(_T("#%08X"), pControl->GetTextColor());
			return strRet;
		}
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_TreeViewUI : public CControlPlugin_ListUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CTreeViewUI* pControl = static_cast<DuiLib::CTreeViewUI*>(__m_pControl->GetInterface(DUI_CTR_TREEVIEW));
		if (!pControl) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
		if(_tcscmp(pstrName,_T("visiblefolderbtn")) == 0)
			return pControl->GetVisibleFolderBtn() ? _T("true") : _T("false");
		else if(_tcscmp(pstrName,_T("visiblecheckbtn")) == 0)
			return pControl->GetVisibleCheckBtn() ? _T("true") : _T("false");
		else if(_tcscmp(pstrName,_T("itemminwidth")) == 0)
		{
			strRet.Format(_T("%d"), pControl->GetItemMinWidth());
			return strRet;
		}
		// 这里不读取子项的属性，选择子项会自动读取
// 		else if(_tcscmp(pstrName, _T("itemtextcolor")) == 0 )
// 			return strRet;
// 		else if(_tcscmp(pstrName, _T("itemhottextcolor")) == 0 )
// 			return strRet;
// 		else if(_tcscmp(pstrName, _T("selitemtextcolor")) == 0 )
// 			return strRet;
// 		else if(_tcscmp(pstrName, _T("selitemhottextcolor")) == 0 )
// 			return strRet;
		return __super::GetAttribute(pstrName);
	}
};

class CControlPlugin_TreeNodeUI : public CControlPlugin_ListContainerElementUI
{
public:
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CTreeNodeUI* pControl = static_cast<DuiLib::CTreeNodeUI*>(__m_pControl->GetInterface(DUI_CTR_TREENODE));
		if (!pControl) return __super::GetAttribute(pstrName);
		DuiLib::CDuiString strRet;
		if(_tcscmp(pstrName, _T("itemtextcolor")) == 0 ){
			strRet.Format(_T("#%08X"), pControl->GetItemTextColor());
			return strRet;
		}
		else if(_tcscmp(pstrName, _T("itemhottextcolor")) == 0 ){
			strRet.Format(_T("#%08X"), pControl->GetItemHotTextColor());
			return strRet;
		}
		else if(_tcscmp(pstrName, _T("selitemtextcolor")) == 0 ){
			strRet.Format(_T("#%08X"), pControl->GetSelItemTextColor());
			return strRet;
		}
		else if(_tcscmp(pstrName, _T("selitemhottextcolor")) == 0 ){
			strRet.Format(_T("#%08X"), pControl->GetSelItemHotTextColor());
			return strRet;
		}
		return __super::GetAttribute(pstrName);
	}
};

#define RETURN_ATTRIB(class, attrib) {class cl; cl.SetControl(__m_pControl); return cl.GetAttribute(attrib);}

class CDefaultControlAttrib : public IControlPlugin
{
public:
	CDefaultControlAttrib(){}
	~CDefaultControlAttrib(){}
	void SetControl(DuiLib::CControlUI* _pControl)
	{
		__m_pControl = _pControl;
	}
	DuiLib::CDuiString GetAttribute(LPCTSTR pstrName)
	{
		DuiLib::CDuiString strControl = __m_pControl->GetClass();
		SIZE_T cchLen = _tcslen(strControl);
		switch( cchLen ) 
		{
		case 6:
			if( _tcsicmp(strControl, _T("EditUI")) == 0 )                   RETURN_ATTRIB(CControlPlugin_EditUI, pstrName)
			else if( _tcsicmp(strControl, _T("ListUI")) == 0 )				RETURN_ATTRIB(CControlPlugin_ListUI, pstrName)
			else if( _tcsicmp(strControl, _T("TextUI")) == 0 )				RETURN_ATTRIB(CControlPlugin_TextUI, pstrName)
			break;
		case 7:
			if( _tcsicmp(strControl, _T("ComboUI")) == 0 )					RETURN_ATTRIB(CControlPlugin_ComboUI, pstrName)
			else if( _tcsicmp(strControl, _T("LabelUI")) == 0 )				RETURN_ATTRIB(CControlPlugin_LabelUI, pstrName)
			break;
		case 8:
			if( _tcsicmp(strControl, _T("ButtonUI")) == 0 )					RETURN_ATTRIB(CControlPlugin_ButtonUI, pstrName)
			else if( _tcsicmp(strControl, _T("OptionUI")) == 0 )			RETURN_ATTRIB(CControlPlugin_OptionUI, pstrName)
			else if( _tcsicmp(strControl, _T("SliderUI")) == 0 )			RETURN_ATTRIB(CControlPlugin_SliderUI, pstrName)
			break;
		case 9:
			if( _tcsicmp(strControl, _T("ControlUI")) == 0 )				RETURN_ATTRIB(CControlPlugin_ControlUI, pstrName)
			else if( _tcsicmp(strControl, _T("ActiveXUI")) == 0 )			RETURN_ATTRIB(CControlPlugin_ActiveXUI, pstrName)
			break;
		case 10:
			if( _tcsicmp(strControl, _T("ProgressUI")) == 0 )				RETURN_ATTRIB(CControlPlugin_ProgressUI, pstrName)
			else if( _tcsicmp(strControl, _T("RichEditUI")) == 0 )          RETURN_ATTRIB(CControlPlugin_RichEditUI, pstrName)
			else if( _tcsicmp(strControl, _T("CheckBoxUI")) == 0 )			RETURN_ATTRIB(CControlPlugin_CheckBoxUI, pstrName)
			else if( _tcsicmp(strControl, _T("DateTimeUI")) == 0 )			RETURN_ATTRIB(CControlPlugin_DateTimeUI, pstrName)
			else if( _tcsicmp(strControl, _T("TreeViewUI")) == 0 )			RETURN_ATTRIB(CControlPlugin_TreeViewUI, pstrName)
			else if( _tcsicmp(strControl, _T("TreeNodeUI")) == 0 )			RETURN_ATTRIB(CControlPlugin_TreeNodeUI, pstrName)
			break;
		case 11:
			if( _tcsicmp(strControl, _T("ContainerUI")) == 0 )              RETURN_ATTRIB(CControlPlugin_ContainerUI, pstrName)
			else if( _tcsicmp(strControl, _T("TabLayoutUI")) == 0 )         RETURN_ATTRIB(CControlPlugin_TabLayoutUI, pstrName)
			else if( _tcsicmp(strControl, _T("ScrollBarUI")) == 0 )         RETURN_ATTRIB(CControlPlugin_ScrollBarUI, pstrName)
			break;
		case 12:
			if( _tcsicmp(strControl, _T("ListHeaderUI")) == 0 )				RETURN_ATTRIB(CControlPlugin_ListHeaderUI, pstrName)
			else if( _tcsicmp(strControl, _T("TileLayoutUI")) == 0 )			RETURN_ATTRIB(CControlPlugin_TileLayoutUI, pstrName)
			else if( _tcsicmp(strControl, _T("WebBrowserUI")) == 0 )        RETURN_ATTRIB(CControlPlugin_WebBrowserUI, pstrName)
			break;
		case 13:
			if (_tcsicmp(strControl, _T("ChildLayoutUI")) == 0)				RETURN_ATTRIB(CControlPlugin_ChildLayoutUI, pstrName)
			break;
		case 16:
			if( _tcsicmp(strControl, _T("VerticalLayoutUI")) == 0 )			RETURN_ATTRIB(CControlPlugin_VerticalLayoutUI, pstrName)
			else if( _tcsicmp(strControl, _T("ListHeaderItemUI")) == 0 )    RETURN_ATTRIB(CControlPlugin_ListHeaderItemUI, pstrName)
			break;
		case 17:
			if( _tcsicmp(strControl, _T("ListTextElementUI")) == 0 )        RETURN_ATTRIB(CControlPlugin_ListTextElementUI, pstrName)
			break;
		case 18:
			if( _tcsicmp(strControl, _T("HorizontalLayoutUI")) == 0 )       RETURN_ATTRIB(CControlPlugin_HorizontalLayoutUI, pstrName)
			else if( _tcsicmp(strControl, _T("ListLabelElementUI")) == 0 )  RETURN_ATTRIB(CControlPlugin_ListLabelElementUI, pstrName)
			break;
		case 22:
			if( _tcsicmp(strControl, _T("ListContainerElementUI")) == 0 )   RETURN_ATTRIB(CControlPlugin_ListContainerElementUI, pstrName)
			break;
		}
		return DuiLib::CDuiString();
	}
protected:
	DuiLib::CControlUI*		__m_pControl;
};

#endif