#pragma once
#ifndef __CLIPBOARD_H_2015_11_23
#define __CLIPBOARD_H_2015_11_23

class CClipboard
{
public:
	static CClipboard&		Instance();
	bool					Write(DuiLib::CControlUI* _pControl);
	DuiLib::CControlUI*		Read(DuiLib::IDialogBuilderCallback* _pCallBack, DuiLib::CPaintManagerUI* _pPaintManager, DuiLib::CControlUI* _pParent);
	bool					IsEmpty();
private:
	CClipboard(){}
	CClipboard(const CClipboard&){}
};

#define clipboard CClipboard::Instance()

#endif