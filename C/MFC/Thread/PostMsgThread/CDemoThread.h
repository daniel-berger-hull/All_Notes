#pragma once


#define DEFAULT_STAY_ALIVE_DURATION_MSEC   1000

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

	void setOwner(CDialog* ptrToOwner) { owner = ptrToOwner;   }
	void setStayAliveDuration(int duration) { stayAliveDuration = duration;  }


protected:
	DECLARE_MESSAGE_MAP()


	afx_msg void OnStopJobMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnInfoMsg(WPARAM wParam, LPARAM lParam);

	CDialog* owner;
	int stayAliveDuration;

};


