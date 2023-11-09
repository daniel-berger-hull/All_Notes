#pragma once



// CMathThread

class CMathThread : public CWinThread
{
	DECLARE_DYNCREATE(CMathThread)

protected:
	CMathThread();           // protected constructor used by dynamic creation
	virtual ~CMathThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();

protected:
	DECLARE_MESSAGE_MAP()
};


