WorkerThread
------------
   
    Special thread to use when updating GUI.
    It is the thread version with more boiler plate coding, to avoid conflicting with the main thread of a GUI class (xFrame, xDialog)
    
    If modification is done on the main screen of your application, then this is the version of thread to use.
 
 
 SimpleGUIThread
 ---------------
 
   Slightly lighter thread version than WorkerThread, as this still have more boiler plate coding to handle the GUI specificity,
   this thread version is to be used when a secondary windows existing in your app (aside the main GUI xFrame of your app) 