#include <windows.h>
#include <iostream>
#include <Windows.h>
#include "Session.h"
using namespace std;



//HANDLE hEvents[100];
//CRITICAL_SECTION cs;

DWORD WINAPI MyThread(LPVOID lpParameter)
{
    //EnterCriticalSection(&cs);
    auto session = static_cast<Session*>(lpParameter);
    SafeWrite("session", session->sessionID, "created");
    while (true)
    {
        Message m;
        if (session->getMessage(m))
        {
            switch (m.header.messageType)
            {
            case MT_CLOSE:
            {
                SafeWrite("session", session->sessionID, "closed");
                delete session;
                return 0;
            }
            case MT_DATA:
            {
                SafeWrite("session", session->sessionID, "data", m.data);
                Sleep(500 * session->sessionID);
                break;
            }
            }
        }
    }
    return 0;
}

void start()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
	//InitializeCriticalSection(&cs);
    int i=1;
    vector<Session*> sessions;
    HANDLE hStartEvent = CreateEvent(NULL, FALSE, FALSE, L"StartEvent");
    HANDLE hStopEvent = CreateEvent(NULL, FALSE, FALSE, L"StopEvent");
    HANDLE hConfirmEvent = CreateEvent(NULL, FALSE, FALSE, L"ConfirmEvent");
    HANDLE hCloseEvent = CreateEvent(NULL, FALSE, FALSE, L"CloseEvent");
    HANDLE hControlEvents[3] = {hStartEvent, hStopEvent, hCloseEvent };
    
    while (i)
    {
        int n = WaitForMultipleObjects(3, hControlEvents, FALSE, INFINITE) - WAIT_OBJECT_0;
        switch (n)
        {
            case 0:
                sessions.push_back(new Session(i));
                CloseHandle(CreateThread(NULL, 0, MyThread, (LPVOID)sessions.back(), 0, NULL));
                //EnterCriticalSection(&cs);
                //hEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
                //CreateThread(NULL, 0, MyThread, (LPVOID)i, 0, NULL);
                SetEvent(hConfirmEvent);
                //LeaveCriticalSection(&cs);
                i++;
                
                
                break;
            case 1:
                    --i;
                    ResetEvent(hStopEvent);
  //                  SetEvent(hEvents[--i]);
                    
                    if (i >= 1) {
                        sessions[i - 1]->addMessage(MT_CLOSE);
                    }
                    SetEvent(hConfirmEvent);
                break;
            case 2:
                return;
        }
    }
    SetEvent(hConfirmEvent);
	//DeleteCriticalSection(&cs);
}

int main()
{
	start();
	return 0;
}
