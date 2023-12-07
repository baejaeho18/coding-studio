*
   Console.h: declarations for console-related functions
*/

#ifndef   __CONSOL__
#define   __CONSOL__

#ifdef   _WINDOWS
   #include <windows.h>
   #include <conio.h>
#endif   //   _WINDOWS

void clrscr(void);              // clear screen
void gotoxy(int x, int y);     // move cursor to (x, y) coordinate

int getWindowWidth();       // get width of current console window
int getWindowHeight();      // get height of current console window

void DrawLine(int x1, int y1, int x2, int y2, char c);
void swap(int *pa, int *pb);

void EnableCursor(int enable);

void MySleep(int msec);      // wait for msec milliseconds
void MyPause();            // wait for the Enter key


#if defined(_LINUX) || defined(_MAC)

int kbhit();            // check if there is a key in the key buffer
int getch();            // read a key not waiting for the Enter key

#endif   // defined(_LINUX) || defined(_MAC)

#endif   //   __CONSOL__
