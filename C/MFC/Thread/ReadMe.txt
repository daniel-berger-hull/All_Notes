Few projects of MFC Thread


SimpleThread
------------

 Basic dialog project, and a child class of CWinThread is created, to show how to
 overide the core functions of a CWinThread..

PostMsgThread
---------------

CWinThread has a PostThreadMessage method, with with you can post a Window message when it is completed
(just like any Win32 windows objects)

   YourCWinThread->PostThreadMessage(WM_YOUR_MESSAGE_TO_SEND, WPARAM wParam, LPARAM lParam);

The Dialog Window  can pass message to the thread also, ask to terminate or general message

Check this example here for more info:
https://stackoverflow.com/questions/12764300/mfcs-cwinthreadpostthreadmessage-handler-not-called/12782808#12782808



WorkerThread
------------

 The project shows how to use simple Worker Thread (and not GUI Thread of the type CWinThread).
 Worker are the simplest form of Thread in MFC, and an MFC thread can use a plain c method as procedure
 (method not in any class) or a member method of a class.
 There are buttons to start both type of handler method (Plain C method or Member Function method)
 
 
 
 ThreadsSync
 -----------
 
 A project where 2 threads workers (generator and consume) work toghetter, and a third GUI thread
 manages a display at runtime (refresh a Bar Graph around 10 times a sec )
 Also, the goal of the project is to use a CCriticalSection to avoid data racing condition between the 2 threads;
 