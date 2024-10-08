/*
 *    **************************************************
 *    *                                                *
 *    *                    BIGIO.C                     *
 *    *                                                *
 *    *          Extended Precision Calculator         *
 *    *                                                *
 *    *             Low Level I/O Routines             *
 *    *                                                *
 *    *              Version 5.0 01-01-99              *
 *    *                                                *
 *    *              Judson D. McClendon               *
 *    *              Sun Valley Systems                *
 *    *              4522 Shadow Ridge Pkwy            *
 *    *              Pinson, AL 35126-2192             *
 *    *                 205-680-0460                   *
 *    *                                                *
 *    **************************************************
 */
#define  VER_NUMBER "6.0"
char *Version = "BigCalc, Version " VER_NUMBER " " ;

/*
 *    **************************************************
 *    *                                                *
 *    *                   Includes                     *
 *    *                                                *
 *    **************************************************
 */

#include <windows.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigcalc.h"
#include "biggvar.h"
#include "conio32.h"

/*
 *    **************************************************
 *    *                                                *
 *    *                   Constants                    *
 *    *                                                *
 *    **************************************************
 */

#define CLOCKFREQ       ((unsigned)1193180L) /* Timer frequency        */
#define SPEAKERMODE     ((unsigned)0xB6)     /* Set timer for speaker  */
#define TIMERMODEPORT   ((unsigned)0x43)     /* Timer mode port        */
#define FREQPORT        ((unsigned)0x42)     /* Frequency-control port */
#define SPEAKERPORT     ((unsigned)0x61)     /* Speaker port           */
#define SPEAKERON       ((unsigned)0x03)     /* Speaker-on bits        */
#define FREQ            ((unsigned)400)      /* A frequency            */
#define CFREQ           CLOCKFREQ / FREQ     /* Division frequency     */

#define MONOCOLOR       0x07                 /* Grey on black          */
#define COLORCOLOR      0x17                 /* White on blue          */
#define MONOSCREEN      0xB0000000L          /* Mono buffer address    */
#define COLORSCREEN     0xB8000000L          /* Color buffer address   */

/*
 *    **************************************************
 *    *                                                *
 *    *            Source Local Variables              *
 *    *                                                *
 *    **************************************************
 */

static char
   color = 0;                    /* Screen Color    */

/*
 *    **************************************************
 *    *                                                *
 *    *              Keyboard Routines                 *
 *    *                                                *
 *    **************************************************
 */

/*
 *    **************************************************
 *    *                                                *
 *    *         Get decoded character from kbd         *
 *    *                                                *
 *    **************************************************
 */
extern int GetChar(void)
{
   int chr;

   chr = getch();
   if (chr == 0)
      chr = 1000 + getch();      /* Non ASCII character */
   else
      if (isascii(chr) )
         chr = toupper(chr);

   return(chr);
}

/*
 *    **************************************************
 *    *                                                *
 *    *         Return character if key pressed        *
 *    *                                                *
 *    **************************************************
 */
extern int KeyPressed(void)
{
   if (kbhit())
      return(GetChar());
   else
      return(0);
}

/*
 *    **************************************************
 *    *                                                *
 *    *                Screen Routines                 *
 *    *                                                *
 *    **************************************************
 */


/*
 *    **************************************************
 *    *                                                *
 *    *            Initialize Screen Drivers           *
 *    *                                                *
 *    **************************************************
 */
extern void ScrInit(void)
{
   console_init(Version) ;
   if (!is_redirected())
      set_linewrap_state(FALSE) ;
}

/*
 *    **************************************************
 *    *                                                *
 *    *            End of Job Screen Cleanup           *
 *    *                                                *
 *    **************************************************
 */
extern void ScrTerm(void)
{
   color = MONOCOLOR;            /* Clear end of screen to B/W */
   CurPos(XSIGNROW + 1, 1);
   EraEop();
}

/*
 *    **************************************************
 *    *                                                *
 *    *                Clear the Screen                *
 *    *                                                *
 *    **************************************************
 */
extern void ScrClr(void)
{
   dclrscr() ;
}

/*
 *    **************************************************
 *    *              Erase to End of Line              *
 *    **************************************************
 */
extern void EraEol(void)
{
   dclreol() ;
}

/*
 *    **************************************************
 *    *              Erase to End of Page              *
 *    **************************************************
 */
extern void EraEop(void)
{
   dclreos() ;
}

/*
 *    **************************************************
 *    *         Position Cursor at row, column         *
 *    **************************************************
 */
extern void CurPos(int row, int col)
{
   dgotoxy(col-1, row-1) ;
}

/*
 *    **************************************************
 *    *            Get Cursor row, column              *
 *    **************************************************
 */
extern void CurGet(int *row, int *col)
{
   *row = _where_y() + 1;
   *col = _where_x() + 1;
}

/*
 *    **************************************************
 *    *                                                *
 *    *            Write Character to Screen           *
 *    *                                                *
 *    **************************************************
 */
extern void WChar(int chr)
{
   dputc((char) chr) ;
}

/*
 *    **************************************************
 *    *                                                *
 *    *             Write String to Screen             *
 *    *                                                *
 *    **************************************************
 */
extern void WString(char *str)
{
   dputs(str) ;
}

/*
 *    **************************************************
 *    *                                                *
 *    *             Write Integer to Screen            *
 *    *                                                *
 *    **************************************************
 */
extern void WInteger(long integer)
{
   long order;

   if (integer) {
      if (integer < 0L) {
         WChar('-');
         integer = - integer;
         }
      order = 1000000000L;
      while (order > integer) {
         order /= 10L;
         }
      while (order) {
         WChar((int)((integer / order) + (long)'0'));
         integer %= order;
         order /= 10L;
         }
      }
   else
      WChar('0');
}

/*
 *    **************************************************
 *    *                                                *
 *    *       Write String at row, col to Screen       *
 *    *                                                *
 *    **************************************************
 */

extern void WriteAt(int row, int col, char *str)
{
   dprints(row-1, col-1, str) ;
}

/*
 *    **************************************************
 *    *                                                *
 *    *     Display Message centered on row line       *
 *    *                                                *
 *    **************************************************
 */
static char *spaces80 = 
   "                                        "
   "                                        " ;
extern void WriteCenter(int row, char *msg)
{
   dprints(row-1, 0, spaces80) ;
   dprints(row-1, ((80 - strlen(msg)) / 2), msg) ;
}

/*
 *    **************************************************
 *    *                                                *
 *    *      Display Message centered on 25th line     *
 *    *                                                *
 *    **************************************************
 */
extern void Message(char *msg)
{
   WriteCenter(25, msg);
}

/*
 *    **************************************************
 *    *                                                *
 *    *      Write Message and Wait for keystroke      *
 *    *                                                *
 *    **************************************************
 */
extern void MessageWait(char *msg)
{
   char tmsg[81];

   strcpy(tmsg, msg);
   if (*tmsg)
      strcat(tmsg, "  ");
   strcat(tmsg, "(Press a key to continue)");

   Message(tmsg);
   GetChar();
}

/*
 *    **************************************************
 *    *                                                *
 *    *      Write Message and Prompt for Escape       *
 *    *                                                *
 *    **************************************************
 */
extern void MessageEsc(char *msg)
{
   char tmsg[81];

   strcpy(tmsg, msg);
   if (*tmsg)
      strcat(tmsg, "... ");
   strcat(tmsg, "(Press Esc to abort)");

   Message(tmsg);
}

/*
 *    **************************************************
 *    *                                                *
 *    *         Number Entry Support Routines          *
 *    *                                                *
 *    **************************************************
 */


/*
 *    **************************************************
 *    *                                                *
 *    *         Display Character at row, col          *
 *    *                                                *
 *    **************************************************
 */
extern void DisplayChar(int *row, int *col, int chr)
{
   CurPos(*row, *col);
   WChar(chr);

   if (*col < MAXDISPCOL)     /* Find position for next character */
      (*col)++;
   else {
      *col = MINDISPCOL;
      (*row)++;
      }
   CurPos(*row, *col);
}

/*
 *    **************************************************
 *    *                                                *
 *    *               Backspace Character              *
 *    *                                                *
 *    **************************************************
 */
extern void BackSpace(int *row, int *col)
{
   if (*col > MINDISPCOL)     /* find previous character position */
      (*col)--;
   else {
      *col = MAXDISPCOL;
      (*row)--;
      }

   CurPos(*row, *col);
   WChar(' ');
   CurPos(*row, *col);
}

/*
 *    **************************************************
 *    *                                                *
 *    *     Display Exponent Character at row, col     *
 *    *                                                *
 *    **************************************************
 */
extern void DisplayExpChar(int *row, int *col, int chr)
{
   CurPos(*row, *col);
   WChar(chr);

   if (*col < 80)             /* Find position for next character */
      (*col)++;
   else {
      *col = MINDISPCOL;
      (*row)++;
      }
   CurPos(*row, *col);
}

/*
 *    **************************************************
 *    *                                                *
 *    *          Backspace Exponent Character          *
 *    *                                                *
 *    **************************************************
 */
extern void BackSpaceExp(int *row, int *col)
{
   if (*col > MINDISPCOL)     /* find previous character position */
      (*col)--;
   else {
      *col = 80;
      (*row)--;
      }

   CurPos(*row, *col);
   WChar(' ');
   CurPos(*row, *col);
}

/*
 *    **************************************************
 *    *                                                *
 *    *          Display Exponent at row, col          *
 *    *                                                *
 *    **************************************************
 */
extern void DisplayExp(int *row, int *col, int exprow, int expcol, int expsign, long exponent)
{
   long order;

   *row = exprow;
   *col = expcol;
   CurPos(exprow, expcol);          /* Locate to beginning of exponent */
   EraEol();

   if (expsign == '-')
      DisplayExpChar(row, col, '-');

   if (exponent) {                  /* Write exponent value */
      if (exponent < 0L) {
         DisplayExpChar(row, col, '-');
         exponent = - exponent;
         }
      order = 1000000000L;
      while (order > exponent)
         order /= 10L;
      while (order) {
         DisplayExpChar(row, col, (int)((exponent / order) + (long) '0'));
         exponent %= order;
         order /= 10L;
         }
      }
}

/*
 *    **************************************************
 *    *                                                *
 *    *               Printer Routines                 *
 *    *                                                *
 *    **************************************************
 */


/*
 *    **************************************************
 *    *                                                *
 *    *           Print Character on Printer           *
 *    *                                                *
 *    **************************************************
 */
extern void PChar(int chr)
{
   // if (!printtoscreen) {         /* If using printer or disk ... */
   //    fputc(chr, printfile);
   //    return;
   //    }
    
   /* Direct screen output */
   dputc((char) chr) ;
}

/*
 *    **************************************************
 *    *                                                *
 *    *            Print String on Printer             *
 *    *                                                *
 *    **************************************************
 */
extern void PString(char *str)
{
   // if (!printtoscreen) {            /* If using printer or disk ... */
   //    fputs(str, printfile);
   //    return;
   //    }

   /* Direct screen output */
   dputs(str) ;
}

/*
 *    **************************************************
 *    *                                                *
 *    *            Print Integer on Printer            *
 *    *                                                *
 *    **************************************************
 */
extern void PInteger(long integer)
{
   long order;

   if (integer) {
      if (integer < 0L) {
         PChar('-');
         integer = - integer;
         }
      order = 1000000000L;
      while (order > integer) {
         order /= 10L;
         }
      while (order) {
         PChar((int)((integer / order) + (long)'0'));
         integer %= order;
         order /= 10L;
         }
      }
   else
      PChar('0');
}

/*
 *    **************************************************
 *    *                                                *
 *    *              New Line on Printer               *
 *    *                                                *
 *    **************************************************
 */
extern void NewLine(void)
{
   PString("\r\n");
}

/*
 *    **************************************************
 *    *                                                *
 *    *              New Page on Printer               *
 *    *                                                *
 *    **************************************************
 */
extern void NewPage(void)
{
   PString("\r\f");
}

