// Sample multithreaded programm
/*
 * Bounce- creates a new thread each time the letter 'a' is typed.
 * Each thread bounces a character of a different color around
 * the screen. All threads are terminated with the letter 'Q'.
 */

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>

#define MAX_THREADS	32

// The function getrandom return a number between min and max, must be integer (range)

#define getrandom( min,max ) (SHORT)((rand() % (int)(((max)+1) - \
								(min))) + (min))

int main(void);
void KbdFunc(void);
void BounceProc(void* MyID);
void ClearScreen(void);
void ShutDown(void);
void WriteTitle(int ThreadNum);

HANDLE	 hConsoleOut;
HANDLE	 hRunMutex;
HANLDE	 hScreenMutex;
int		 ThreadNr;
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
COORD	 consoleSize;
BOOL	 bTrails;

int main()	//thread one
{
	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo( hConsoleOut, &csbiInfo);
	consoleSize.X = csbiInfo.srWindow.Right;
	conoleSize.Y = csbiInfo.srWindow.Bottom;
	ClearScreen();
	WriteTitle(0);

	// Create the Mutexes and reset thread count
	hScreenMutex = CreateMutex(NULL, FALSE, NULL);		// cleared
	hRunMutex = CreateMutex(NULL, TRUE, NULL);			// set
	ThreadNr = 0;
	bTrails = FALSE;

	// Start waiting for keyboard input to dispatch thread or exit
	KbdFunc();

	// All threads done cleanup handles
	if (hScreenMutex) CloseHandle(hScreenMutex);
	if (hRunMutex) CloseHandle(hRunMutex);
	if (hConsoleOut) CloseHandle(hConsoleOut);
}

void ShutDown(void)	//shutdown the threads
{
	while (ThreadNr > 0)
	{
		// Tell thread to die and record its death
		ReleaseMutex(hRunMutex);
		ThreadNr--;
	}

	// Done ? Clean up Display
	WaitForSingleObject(hScreenMutex, INFINITE);
	ClearScreen();
}

void KbdFunc(void) // Dispatch and count threads
{
	int KeyInfo;

	do
	{
		KeyInfo = _getch();
		if (tolower(KeyInfo) == 'a' && ThreadNr < MAX_THREADS)
		{
			ThreadNr++;
			_beginthread(BounceProc, 0, &ThreadNr);
			WriteTitle(ThreadNr);
		}
		if (tolower(KeyInfo == 't')
		{
			bTrails = !bTrails;
		}
	} while (tolower(KeyInfo) != 'q');

	ShutDown();
}

void BounceProc(void *pMyID)
{
	wchar_t	MyCell, OldCell;
	WORD	MyAttrib, OldAttrib = 0;
	wchar_t	BlankCell = 0x20;
	COORD	Coords, Delta;
	COORD	Old = { 0,0 };
	DWORD	Dummy;
	char* MyID = (char*) pMyID;

	// generate update increment and intitial display coordinates
	srand((unsigned int)* MyID * 3);

	Coords.X = getrandom(0, consoleSize.X -1)
	Coords.Y = getrandom(0, consoleSite.Y -1);
	Delta.X = getrandom(-3, 3);
	Deleta.Y = getrandom(-3, 3);

	//Setup t character and generate color attribute from thread number
	if (*MyID > 16)
	{
		MyCell = 0x60 + *MyID -16;	// lowercase
	} else {
		MyCell = 0x40 + *MyID;		// upper
	}
	MyAttrib = *MyID & 0x0f;		// force black background

	do
	{
		WaitForSingleObject(hScreenMutex, INFINITE);

		if ( !bTrails )
		{
			ReadConsoleOutputCharacterW(hConsoleOut, &OldCell, 1, Old, &Dummy);
            ReadConsoleOutputAttribute(hConsoleOut, &OldAttrib, 1, Old, &Dummy);

			if ((OldCell == MyCell) && (OldAttrib == MyAttrib))
			{
				WriteConsoleOutputCharacterW(hConsoleOutput, &BlankCell, 1, Old, &Dummy);
			}
		}

		WriteConsoleOutputCharacterW(hConsoleOut, &MyCell, 1, Coords, &Dummy);
        WriteConsoleOutputAttribute(hConsoleOut, &MyAttrib, 1, Coords, &Dummy);
		ReleasMutex(hScreenMutex);

		// increment Coordinates for next placement of the block
		Old.X = Coords.X;
		Old.Y = Coords.Y;
		Coords.X += Delta.X;
		Coords.Y += Delta.Y;

		// if we are about to go off the screen, reverse direction
		if (Coords.X < 0 || Coords.X >= consoleSize.X)
		{
			Delta.X = -Delta.X;
			Beep(400, 50);
		}
        if (Coords.Y < 0 || Coords.Y >= consoleSize.Y)
        {
            Delta.Y = -Delta.Y;
            Beep(600, 50);
        }
	}
	// Repeat while RunMutex is still taken
	while (WaitForSingleObject(hRunMutex, 75L) == WAIT_TIMEOUT);
}

void WriteTitle( int ThreadNum )
{
	enum
	{
		sizeOfNThreadMsg = 120
	};
	wchar_t	NThreadMsg[sizeOfNThreadMsg] = { L""};

	swprintf_s(NThreadMsg, sizeofNThreadMsg,
		L"Threads running: %02d. Press 'A' "
		L"to start a thread, 'T' to toggle "
		L"trails, 'Q' to quit", ThreadNum);
	SetConsoleTitle(NThreadMsg);
}

void ClearScreen(void)
{
	DWORD	dummy = 0;
	Coord	Home = { 0, 0 };
	FillConsoleOutputCharacterW( hConsoleOut, L' ',
		consoleSize.X * consoleSize.Y,
		Home, &dummy);
}
