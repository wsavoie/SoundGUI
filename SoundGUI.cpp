// GT_HelloWorldWin32.cpp  
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c  

#include <windows.h>  
#include <stdlib.h>  
#include <string.h>  
#include <tchar.h>
#include <stdio.h>  
#include <iostream>
#include <conio.h>
#include <ctime>
#include <string>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <process.h>    /* _beginthread, _endthread */  
#include "resource.h"
#pragma comment(lib, "winmm")
#define IDC_BUTTON_85	100			// Button identifier
#define IDC_BUTTON_125	101			// Button identifier
#define IDC_BUTTON_175	102			// Button identifier
#define IDC_BUTTON_225	103			// Button identifier
#define IDC_BUTTON_275	104			// Button identifier
#define IDC_BUTTON_325	105			// Button identifier
#define IDC_BUTTON_375	106			// Button identifier
#define IDC_BUTTON_425	107			// Button identifier
#define IDC_BUTTON_STOP	108			// Button identifier
#define IDC_BUTTON_SOUNDMAX			109			// Button identifier
#define IDC_BUTTON_SOUNDMIN			110			// Button identifier
#define IDC_BUTTON_FREQBOX	111			// Button identifier
#define IDC_SLIDER			120			// Button identifier

struct params {
	explicit params(int mfreq, int mtime = 1000000) :
		freq(mfreq), time(mtime) {}

	int freq;
	int time;

};
void BeepProc(void * param);

DWORD volume;
static HWAVEOUT hwo;
#define BeepAsync(f,t) _beginthread(BeepProc, 0, (f << 16) & t)
HWND hEdit;
// Global variables  

// The main window class name.  
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.  
static TCHAR szTitle[] = _T("SoundGui");

HINSTANCE hInst;



// Forward declarations of functions included in this code module:  
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND hWnd;
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance; // Store instance handle in our global variable  

										 // The parameters to CreateWindow explained:  
										 // szWindowClass: the name of the application  
										 // szTitle: the text that appears in the title bar  
										 // WS_OVERLAPPEDWINDOW: the type of window to create  
										 // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)  
										 // 500, 100: initial size (width, length)  
										 // NULL: the parent of this window  
										 // NULL: this application does not have a menu bar  
										 // hInstance: the first parameter from WinMain  
										 // NULL: not used in this application  
										 //HWND hWnd = CreateWindow(
										 //	szWindowClass,
										 //	szTitle,
										 //	WS_OVERLAPPEDWINDOW,
										 //	CW_USEDEFAULT, CW_USEDEFAULT,
										 //	500, 100,
										 //	NULL,
										 //	NULL,
										 //	hInstance,
										 //	NULL
										 //);
	HWND hWnd = CreateWindowEx(NULL,
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		200,
		200,
		640,
		280,
		NULL,
		NULL,
		hInst,
		NULL);


	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Sound Gui"),
			NULL);

		return 1;
	}

	// The parameters to ShowWindow explained:  
	// hWnd: the value returned from CreateWindow  
	// nCmdShow: the fourth parameter from WinMain 

	HICON hIcon;
	hIcon = (HICON)LoadImage(hInst,
		MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON,
		16,
		16,
		LR_DEFAULTCOLOR);

	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

	SendMessage(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_SMALL, IDI_ICON1);
	SendMessage(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_BIG, IDI_ICON1);

	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:  IDR_ACCELERATOR1
	MSG msg;
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hWnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//  
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)  
//  
//  PURPOSE:  Processes messages for the main window.  
//  
//  WM_PAINT    - Paint the main window  
//  WM_DESTROY  - post a quit message and return  
//  
//


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Sound Gui");
	HWND freq85;
	HWND freq125;
	HWND freq175;
	HWND freq225;
	HWND freq275;
	HWND freq325;
	HWND freq375;
	HWND freq425;
	HWND freqBox;
	HWND StopButton;
	HWND soundMax;
	HWND soundMin;
	wchar_t buffer[55];
	HWND Slider;

	HWND hwndHot = NULL;
	//HWND WINAPI CreateTrackbar(
	//	HWND hwndDlg,  // handle of dialog box (parent window) 
	//	UINT iMin,     // minimum value in trackbar range 
	//	UINT iMax,     // maximum value in trackbar range 
	//	UINT iSelMin,  // minimum value in trackbar selection 
	//	UINT iSelMax)  // maximum value in trackbar selection 
	//
	//	InitCommonControls(); // loads common control's DLL 

	HRESULT hr;
	CoInitialize(NULL);
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	IMMDevice *defaultDevice = NULL;

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	IAudioEndpointVolume *endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL;
	//};
	//HWAVEOUT hwo;


	switch (message)
	{
	case WM_CREATE:
	{
		int w = 50;
		int h = 25;
		int spacer = 10;
		int butNum = 1;
		int yHeight_Buttons = 50;
		int yHeight_Slider = 90;


		freq85 = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"85",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			(w + spacer)*butNum,         // x position 
			yHeight_Buttons,         // y position 
			w,        // Button width
			h,        // Button height
			hWnd,     // Parent window
			(HMENU)IDC_BUTTON_85,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.

		butNum++;

		freq125 = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"125",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			(w + spacer)*butNum,         // x position 
			yHeight_Buttons,         // y position 
			w,        // Button width
			h,        // Button height
			hWnd,     // Parent window
			(HMENU)IDC_BUTTON_125,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.

		butNum++;
		freq175 = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"175",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			(w + spacer)*butNum,         // x position 
			yHeight_Buttons,         // y position 
			w,        // Button width
			h,        // Button height
			hWnd,     // Parent window
			(HMENU)IDC_BUTTON_175,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.
		butNum++;
		freq225 = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"225",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			(w + spacer)*butNum,         // x position 
			yHeight_Buttons,         // y position 
			w,        // Button width
			h,        // Button height
			hWnd,     // Parent window
			(HMENU)IDC_BUTTON_225,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.
		butNum++;

		freq275 = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"275",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			(w + spacer)*butNum,         // x position 
			yHeight_Buttons,         // y position 
			w,        // Button width
			h,        // Button height
			hWnd,     // Parent window
			(HMENU)IDC_BUTTON_275,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.
		butNum++;

		freq325 = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"325",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			(w + spacer)*butNum,         // x position 
			yHeight_Buttons,         // y position 
			w,        // Button width
			h,        // Button height
			hWnd,     // Parent window
			(HMENU)IDC_BUTTON_325,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.
		butNum++;

		freq375 = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"375",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			(w + spacer)*butNum,         // x position 
			yHeight_Buttons,         // y position 
			w,        // Button width
			h,        // Button height
			hWnd,     // Parent window
			(HMENU)IDC_BUTTON_375,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.
		butNum++;
		freq425 = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"425",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			(w + spacer)*butNum,         // x position 
			yHeight_Buttons,         // y position 
			w,        // Button width
			h,        // Button height
			hWnd,     // Parent window
			(HMENU)IDC_BUTTON_425,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.


		freqBox = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"425",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			(w + spacer)*butNum,         // x position 
			yHeight_Buttons,         // y position 
			w,        // Button width
			h,        // Button height
			hWnd,     // Parent window
			(HMENU)IDC_BUTTON_425,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.


		StopButton = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"STOP",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			150,         // x position 
			yHeight_Buttons + 80,         // y position 
			w,        // Button width
			h,        // Button height
			hWnd,     // Parent window
			(HMENU)IDC_BUTTON_STOP,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.

		soundMin = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"Min Vol",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			220,         // x position 
			yHeight_Buttons + 80,         // y position 
			w + 20,        // Button width
			h,        // Button height
			hWnd,     // Parent window
			(HMENU)IDC_BUTTON_SOUNDMIN,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.

		soundMax = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"Max Vol",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			220,         // x position 
			yHeight_Buttons + 110,         // y position 
			w + 20,        // Button width
			h,        // Button height
			hWnd,     // Parent window
			(HMENU)IDC_BUTTON_SOUNDMAX,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.

		//freqBox = CreateWindow(
		//	L"EDIT",  // Predefined class; Unicode assumed 
		//	L"100",      // Button text 
		//	ES_NUMBER | ES_LEFT | WS_VISIBLE | WS_CHILD ,  // Styles 
		//	220,         // x position 
		//	yHeight_Buttons + 50,         // y position 
		//	w + 20,        // Button width
		//	h,        // Button height
		//	hWnd,     // Parent window
		//	(HMENU)IDC_BUTTON_FREQBOX,       // No menu.
		//	(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		//	NULL);      // Pointer not needed.

		Slider = CreateWindowEx(
			0,                               // no extended styles 
			L"SLIDER",                  // class name 
			L"Trackbar Control",              // title (caption) 
			WS_CHILD |
			WS_VISIBLE | WM_HSCROLL,
			w * 4, yHeight_Slider,                          // position 
			200, 30,                         // size 
			hWnd,                         // parent window 
			(HMENU)IDC_SLIDER,                     // control identifier 
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),                         // instance 
			NULL                             // no WM_CREATE parameter 
		);


		break;
	}

	case WM_COMMAND:
	{
		time_t startTime, lastBeep, curTime;
		time(&startTime);
		lastBeep = curTime = startTime;

		switch (LOWORD(wParam))
		{

			case IDC_BUTTON_85: case ID_F1_85:
			{
				int freq = 85;
				params p(freq);
				_beginthread(BeepProc, 0, &p);

				swprintf(buffer, L"PLAYING %d Hz", freq);
				SendMessage(hWnd,
					WM_SETTEXT,
					sizeof(buffer) / sizeof(buffer[0]),
					reinterpret_cast<LPARAM>(buffer));

				break;
			}

		case IDC_BUTTON_125: case ID_F2_125:
		{
			int freq = 125;
			params p(freq);
			_beginthread(BeepProc, 0, &p);

			swprintf(buffer, L"PLAYING %d Hz", freq);
			SendMessage(hWnd,
				WM_SETTEXT,
				sizeof(buffer) / sizeof(buffer[0]),
				reinterpret_cast<LPARAM>(buffer));

			break;
		}
		case IDC_BUTTON_175: case ID_F3_175:
		{
			int freq = 175;
			params p(freq);
			_beginthread(BeepProc, 0, &p);

			swprintf(buffer, L"PLAYING %d Hz", freq);
			SendMessage(hWnd,
				WM_SETTEXT,
				sizeof(buffer) / sizeof(buffer[0]),
				reinterpret_cast<LPARAM>(buffer));

			break;
		}
		case IDC_BUTTON_225: case ID_F4_225:
		{
			int freq = 225;
			params p(freq);
			_beginthread(BeepProc, 0, &p);

			swprintf(buffer, L"PLAYING %d Hz", freq);
			SendMessage(hWnd,
				WM_SETTEXT,
				sizeof(buffer) / sizeof(buffer[0]),
				reinterpret_cast<LPARAM>(buffer));

			break;
		}
		case IDC_BUTTON_275: case ID_F5_275:
		{
			int freq = 275;
			params p(freq);
			_beginthread(BeepProc, 0, &p);

			swprintf(buffer, L"PLAYING %d Hz", freq);
			SendMessage(hWnd,
				WM_SETTEXT,
				sizeof(buffer) / sizeof(buffer[0]),
				reinterpret_cast<LPARAM>(buffer));

			break;
		}
		case IDC_BUTTON_325: case ID_F6_325:
		{
			int freq = 325;
			params p(freq);
			_beginthread(BeepProc, 0, &p);

			swprintf(buffer, L"PLAYING %d Hz", freq);
			SendMessage(hWnd,
				WM_SETTEXT,
				sizeof(buffer) / sizeof(buffer[0]),
				reinterpret_cast<LPARAM>(buffer));

			break;
		}
		case IDC_BUTTON_375: case ID_F7_375:
		{
			int freq = 375;
			params p(freq);
			_beginthread(BeepProc, 0, &p);

			swprintf(buffer, L"PLAYING %d Hz", freq);
			SendMessage(hWnd,
				WM_SETTEXT,
				sizeof(buffer) / sizeof(buffer[0]),
				reinterpret_cast<LPARAM>(buffer));

			break;
		}

		case IDC_BUTTON_425: case ID_F8_425:
		{
			int freq = 425;
			params p(freq);
			_beginthread(BeepProc, 0, &p);

			swprintf(buffer, L"PLAYING %d Hz", freq);
			SendMessage(hWnd,
				WM_SETTEXT,
				sizeof(buffer) / sizeof(buffer[0]),
				reinterpret_cast<LPARAM>(buffer));

			break;
		}

		case IDC_BUTTON_SOUNDMIN: case ID_F9_MIN:
		{

			//float currentVolume = 0;
			//endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
			double newVol = 0;
			endpointVolume->SetMasterVolumeLevelScalar((float)newVol, NULL);
			swprintf(buffer, L"Volume: %1.0f", newVol * 100);
			SendMessage(hWnd,
				WM_SETTEXT,
				sizeof(buffer) / sizeof(buffer[0]),
				reinterpret_cast<LPARAM>(buffer));

			break;
		}
		case IDC_BUTTON_SOUNDMAX: case ID_F10_MAX:
		{
			//float currentVolume = 0;
			//endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
			double newVol = 1;
			endpointVolume->SetMasterVolumeLevelScalar((float)newVol, NULL);
			swprintf(buffer, L"Volume: %1.0f", newVol * 100);
			SendMessage(hWnd,
				WM_SETTEXT,
				sizeof(buffer) / sizeof(buffer[0]),
				reinterpret_cast<LPARAM>(buffer));

			break;
		}

		case IDC_SLIDER:
		{

			int freq = 425;
			params p(freq);
			_beginthread(BeepProc, 0, &p);

			swprintf(buffer, L"PLAYING %d Hz", freq);
			SendMessage(hWnd,
				WM_SETTEXT,
				sizeof(buffer) / sizeof(buffer[0]),
				reinterpret_cast<LPARAM>(buffer));
			break;
		}

		case IDC_BUTTON_STOP: case ID_F12_STOP: case ID_ESC_STOP:
		{

			float currentVolume = 0;
			endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
			swprintf(buffer, L"STOPPED Current Volume: %1.0f", currentVolume * 100);
			//try {}
			//catch (UINT e){swprintf(buffer, L"ERROR %d", e);}

			Beep(1, 1);
			SendMessage(hWnd,
				WM_SETTEXT,
				sizeof(buffer) / sizeof(buffer[0]),
				reinterpret_cast<LPARAM>(buffer));

			break;
		}

		//case IDC_BUTTON_FREQBOX: case ID_RTN_FREQBOX:
		//{
		//	auto c = HIWORD(wParam);
		//
		//	swprintf(buffer, L"STOPPED Current Volume: %1.0f", c);
		//	//try {}
		//	//catch (UINT e){swprintf(buffer, L"ERROR %d", e);}

		//	Beep(1, 1);
		//	SendMessage(hWnd,
		//		WM_SETTEXT,
		//		sizeof(buffer) / sizeof(buffer[0]),
		//		reinterpret_cast<LPARAM>(buffer));

		//	break;
		//}

		}
	}

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		// Here your application is laid out.  
		// For this introduction, we just print out "Hello, World!"  
		// in the top left corner.  
		TextOut(hdc,
			5, 5,
			greeting, _tcslen(greeting));

		// End application-specific layout section.  

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	}

	endpointVolume->Release();

	CoUninitialize();


	return 0;
}

void BeepProc(void * param)
{
	params* p = (params*)param;
	Beep(p->freq, p->time);
}
