#pragma once
#ifndef __STDAFX_H_2015_11_02
#define __STDAFX_H_2015_11_02
#define _SCL_SECURE_NO_WARNINGS
#include <wtypes.h>
#include <vector>
#include <memory>
#include <map>
#include <string>
#include <sstream>

#ifdef _UNICODE
typedef std::wstring		_ZnString;
typedef std::wstringstream	_ZnSStream;
#else
typedef std::string			_ZnString;
typedef std::stringstream	_ZnSStream;
#endif

#define UILIB_STATIC
#include "../DuiLib/UIlib.h"
#include "../Lib/Lib.hpp"
#include "../Rapidxml/rapidxml_utils.hpp"
#include "../Rapidxml/rapidxml_print.hpp"
#include "ConfigManager.h"
#include "PluginManager.h"
#include "FileManager.h"
#include "Clipboard.h"
#include "Memorandum.h"
#include "Window/MainFrame.h"
#include "Window/SettingsWnd.h"
#include "Window/AboutWnd.h"
#include "Window/PreviewWnd.h"
#include "Window/DefaultAttrWnd.h"

extern CMainFrame*		g_pMainFrame;

// 调试输出宏
#ifdef _DEBUG
#define TRACE DuiLib::DUI__Trace
#else
#define TRACE
#endif

// 窗口字符串
#define STR_WINDOW						_T("Window")
// 滚动条的字符串
#define STR_SCROLLBAR					_T("ScrollBar")
#define STR_HSCROLL						_T("HScrollBar")
#define STR_VSCROLL						_T("VScrollBar")
// 树节点
#define STR_TREENODE					_T("TreeNode")
// 资源路径属性名
#define STR_RESOURCEPATH				_T("resousepath")
// 列表子项
#define STR_LISTHEADER					_T("ListHeader")
#define STR_LISTHEADERITEM				_T("ListHeaderItem")
#define STR_LISTELEMENT					_T("ListElement")
#define STR_LISTLABELELEMENT			_T("ListLabelElement")
#define STR_LISTTEXTELEMENT				_T("ListTextElement")
#define STR_LISTCONTAINERELEMENT		_T("ListContainerElement")


// 自定义消息

// 当前选择的控件发生变化
#define MSG_EDIT_SELECTEDCHANGE		_T("MSG_EDIT_SELECTEDCHANGE")

// 消息绑定宏
//#define Notify_Bind(class, name, message) {if(msg.pSender->GetName() == name && msg.sType == message){static class obj; obj.Work(msg);}}
//#define GlobleNotify_Bind(class, message) {if(msg.sType == message){static class obj; obj.Work(msg);}}
//#define Timer_Bind(class, name, timeid, elapse) {static bool bTimer = m_PaintManager.SetTimer(m_PaintManager.FindControl(name),timeid,elapse);if(msg.pSender->GetName() == name && msg.sType == DUI_MSGTYPE_TIMER && msg.wParam == timeid){static class obj; obj.Work(msg);}}
#define AutoHide(name, pt){static DuiLib::CControlUI* pObj = m_PaintManager.FindControl(name); if (pObj->IsVisible() && !::PtInRect(&pObj->GetPos(), pt)) {pObj->SetVisible(false);}}
#define DUI_GET_CONTROL_PTR(type, ptr, name) ptr = static_cast<type*>(m_PaintManager.FindControl(name))
#define ON_DUI_NOTIFY(fun, name, message) if (msg.pSender->GetName() == name && msg.sType == message) fun(msg)

// 获取当前EXE路径
LPCTSTR cur_path();
// 从文件路径中获取文件名
LPCTSTR get_filename(LPCTSTR _lpFilePath);
// 从文件路径中获取路径
LPCTSTR get_path(LPCTSTR _lpFilePath);


// 解析以\0分割以\0\0结尾的字符串组
void split_string(LPCTSTR _lpStr, std::vector<DuiLib::CDuiString>& vecStr);
// 解析DUILIB的属性列表
void split_attrib_list(LPCTSTR _lpStr, std::map<DuiLib::CDuiString, DuiLib::CDuiString>& _mapAttr);
void create_attrib_list(_ZnString& _Str, const std::map<DuiLib::CDuiString, DuiLib::CDuiString>& _mapAttr);

// 编码转换
std::tr1::shared_ptr<char>		W2Str(const wchar_t* _lpwstr, UINT unCode = CP_ACP);
std::tr1::shared_ptr<wchar_t>	S2Wstr(const char* _lpStr, UINT unCode = CP_ACP);

// 获取父控件指针
DuiLib::CControlUI*			get_parent(DuiLib::CControlUI* _pControl);
// 获取属性的接口指针
IControlPlugin*				get_control_interface(DuiLib::CControlUI* _pControl);
// 根据控件获取虚拟窗口指针
DuiLib::CControlUI*			get_wnd_ptr(DuiLib::CControlUI* _pControl);
// 获取窗口的共享数据指针
IWindowSharedData*			get_wnd_shared_data(DuiLib::CControlUI* _pControl);
// 创建一个运行期唯一的ID
int							create_id();
// 判断一个控件是否是TreeNode中的组合控件 如果为假返回空指针，如果为真返回所在TreeNode指针，如果为TreeNode自身就返回自身的指针
DuiLib::CTreeNodeUI*		is_tree_node_sub_control(DuiLib::CControlUI* _pControl);
// 将异形RECT还原
RECT						normalize_rect(const RECT& _rc);
// 输出警告信息
void						out_error(LPCTSTR _lpMsg);
#endif