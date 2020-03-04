
// EasyTerm.h: EasyTerm 응용 프로그램의 기본 헤더 파일
//
#pragma once

#include "Comm.h"
#include "kaMemStr.h"

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // 주 기호입니다.


// CEasyTermApp:
// 이 클래스의 구현에 대해서는 EasyTerm.cpp을(를) 참조하세요.
//

class CEasyTermApp : public CWinAppEx
{
private:
	bool m_bThreadTerm;
	CWinThread* m_pThread;

public:
	CEasyTermApp() noexcept;


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileOpen();

	static UINT ThreadForCommRead(LPVOID pParam);
	void Set_Thread_Terminate(bool bVal);
	bool Get_Thread_Terminate();
	bool Get_Comm_Established();
	bool ProcessCommData(kaMemStr* pStr);

	bool Read(char& ch);

};

extern CEasyTermApp theApp;
