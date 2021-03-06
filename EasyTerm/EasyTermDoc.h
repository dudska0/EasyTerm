﻿
// EasyTermDoc.h: CEasyTermDoc 클래스의 인터페이스
//


#pragma once
#include "CRespData.h"
#include "kaSecsDecoder.h"
#include "CommConfig.h"

//
class CEasyTermDoc : public CDocument
{
public:
	CResponse*	m_pResp;
	kaSecsDecoder* m_pDecoder;
	CCommConfig* m_pConfig;


protected: // serialization에서만 만들어집니다.
	CEasyTermDoc() noexcept;
	DECLARE_DYNCREATE(CEasyTermDoc)

// 특성입니다.
public:
	bool OpenDataFile(CString sFileName, CString &strError);

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CEasyTermDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS


public:
	int Decode_Stream(unsigned char* pucStream, CString& sSxFy, CString& sMsg);
	void Set_Comm_Config(CCommConfig* pConfig);
	void Set_Data(CCommConfig* pConfig);
	bool Make_Response(char* szKeyVal, int iMode);
};
