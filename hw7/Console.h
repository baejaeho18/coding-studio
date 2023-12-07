/*
Console.h: declarations for console - related functions
*/

#ifndef   __CONSOL__
#define   __CONSOL__

#ifdef _WINDOWS
#include <windows.h>
#include <conio.h>
#endif // _WINDOWS

void clrscr(void);              // Clear the console screen
void gotoxy(int x, int y);      // Move the cursor to (x, y) coordinate

int getWindowWidth();            // Get the width of the current console window
int getWindowHeight();           // Get the height of the current console window

void DrawLine(int x1, int y1, int x2, int y2, char c);
void swap(int* pa, int* pb);

void EnableCursor(int enable);

void MySleep(int msec);          // Wait for msec milliseconds
void MyPause();                  // Wait for the Enter key


#if defined(_LINUX) || defined(_MAC)
    int kbhit();                  // Check if there is a key in the key buffer
    int getch();                  // Read a key without waiting for the Enter key
#endif // defined(_LINUX) || defined(_MAC)

#endif   //   __CONSOL__
