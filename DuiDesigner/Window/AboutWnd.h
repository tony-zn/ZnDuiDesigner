#pragma once
#ifndef __ABOUT_H_2015_11_04
#define __ABOUT_H_2015_11_04

class CAboutWnd : public DuiLib::WindowImplBase
{
public:
	DuiLib::CDuiString			GetSkinFolder();
	DuiLib::CDuiString			GetSkinFile();
	LPCTSTR						GetWindowClassName(void) const;
};

#endif