#pragma once



// CMathThread

class CDemoThread : public CWinThread
{
	DECLARE_DYNCREATE(CDemoThread)

protected:
	CDemoThread();           // protected constructor used by dynamic creation
	virtual ~CDemoThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();

protected:
	DECLARE_MESSAGE_MAP()
};


