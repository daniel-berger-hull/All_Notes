
//
// See the orignial example in C:\data\Programming\libs\wxWidgets3.1.1\samples\thread\thread.cpp
//
#define GUITHREAD_NUM_UPDATES       50

class MyTargetDialog;  // change the type to your dialog class here...

class SimpleGUIThread : public wxThread
{
	public:
	    SimpleGUIThread(MyTargetDialog *dlg) : wxThread(wxTHREAD_JOINABLE)
	    {
		m_dlg = dlg;
	    }

	    virtual ExitCode Entry() wxOVERRIDE;

	private:
	    MyTargetDialog *m_dlg; // change the type to your dialog class here...
};