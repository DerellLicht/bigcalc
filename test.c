//**********************************************************
//  Win32 test program
//**********************************************************
// typedef struct _CONSOLE_SCREEN_BUFFER_INFO { // csbi 
//     COORD      dwSize; 
//     COORD      dwCursorPosition; 
//     WORD       wAttributes; 
//     SMALL_RECT srWindow; 
//     COORD      dwMaximumWindowSize; 
// } CONSOLE_SCREEN_BUFFER_INFO ; 
// 
// typedef struct _SMALL_RECT { // srct 
//     SHORT Left; 
//     SHORT Top; 
//     SHORT Right; 
//     SHORT Bottom; 
// } SMALL_RECT; 
// 
// typedef struct _COORD { // coord. 
//     SHORT X;      // horizontal coordinate 
//     SHORT Y;      // vertical coordinate 
// } COORD; 
//**********************************************************

char *Version = "Win32 test program" ;
  
#include <windows.h>
#include <stdio.h>
#include "conio32.h"

extern CONSOLE_SCREEN_BUFFER_INFO sinfo ;

//****************************************************
void fill_area(void)
{
   
}         

//****************************************************
void show_console_info(void)
{
   int width, height ;

   printf("buffer size: %d x %d\n", sinfo.dwSize.X, sinfo.dwSize.Y) ;
   width = sinfo.srWindow.Right - sinfo.srWindow.Left + 1 ;
   height = sinfo.srWindow.Bottom - sinfo.srWindow.Top + 1 ;
   printf("screen size: %d x %d\n", width, height) ;
   printf("screen ul corner: X=%d, Y=%d\n", sinfo.srWindow.Left, sinfo.srWindow.Top) ;
   printf("screen lr corner: X=%d, Y=%d\n", sinfo.srWindow.Right, sinfo.srWindow.Bottom) ; 
   printf("cursor pos:  X=%d, Y=%d\n", sinfo.dwCursorPosition.X, 
                                       sinfo.dwCursorPosition.Y) ;
   printf("max window size: width=%d, height=%d\n",
      sinfo.dwMaximumWindowSize.X,
      sinfo.dwMaximumWindowSize.Y) ;
}         

//****************************************************
int main(void)
{
   console_init(Version) ;
   if (sinfo.dwSize.X >= 0)
      set_linewrap_state(FALSE) ;
   show_console_info() ;
   clear_visible_rows(12, 20) ;
   
   return 0;
}         
