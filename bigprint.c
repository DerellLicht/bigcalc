/*
 *    **************************************************
 *    *                                                *
 *    *                  BIGPRINT.C                    *
 *    *                                                *
 *    *          Extended Precision Calculator         *
 *    *                                                *
 *    *               Printing Routines                *
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

/*
 *    **************************************************
 *    *                                                *
 *    *                   Includes                     *
 *    *                                                *
 *    **************************************************
 */

#include <windows.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigcalc.h"
#include "biggvar.h"
#include "conio32.h"

static  void PExponent(long exponent);
static  void WExponent(long exponent);
static  void PSign(int sign);
static  void PCharWrap(int chr);
/*
 *    **************************************************
 *    *                                                *
 *    *            Source Local Variables              *
 *    *                                                *
 *    **************************************************
 */

static int
   ppos = 0,            /* Current print column */
   prevchr;             /* Previous character printed */

/*
 *    **************************************************
 *    *                                                *
 *    *               Printing routines                *
 *    *                                                *
 *    **************************************************
 */


/*
 *    **************************************************
 *    *                                                *
 *    *                 Print Heading                  *
 *    *                                                *
 *    **************************************************
 */
extern void PrintHeading(void)
{
   Message("Printing...");

   PString("\r\n***  BIGCALC  (precision ");
   PInteger((long) normprec);
   PString(" digits, ");
   PChar(PRINTDECIMAL);
   PString(" is decimal point)  ***\r\n\n");
}

/*
 *    **************************************************
 *    *                                                *
 *    *     Print Registers [lo to hi] on Printer      *
 *    *                                                *
 *    **************************************************
 */
extern void PrintReg(int lo, int hi)
{
   int r;

   for (r = lo; r <= hi; r++) {
      PChar(r + '0');
      PrintNumber(&reg[r]);
      NewLine();
      }
   NewLine();
}

/*
 *    **************************************************
 *    *                                                *
 *    *       Print Stack [lo to hi] on Printer        *
 *    *                                                *
 *    **************************************************
 */

extern void PrintStack(int lo, int hi)

{

   int s;

   for (s = hi; s >= lo; s--) {
      PChar(stackname[s]);
      PrintNumber(&stack[s]);
      NewLine();
      }

   NewLine();

}




/*
 *    **************************************************
 *    *                                                *
 *    *   Display Memory Register in Full on Screen    *
 *    *                                                *
 *    **************************************************
 */
extern void DisplayReg(int r)
{
   printtoscreen = TRUE;      /* Temporarily print to screen */

   set_text_attrx(REG_ATTR) ;
   // CurPos(1, 1);
   // EraEop();
   dclrscr() ;
   menucleared = TRUE;

   PChar(r + '0');
   PrintNumber(&reg[r]);

   MessageWait("");

   WorkScreen();

   printtoscreen = FALSE;     /* Reset print to screen */
}

/*
 *    **************************************************
 *    *                                                *
 *    *    Display Stack Register in Full on Screen    *
 *    *                                                *
 *    **************************************************
 */

extern void DisplayStack(int s)

{

   printtoscreen = TRUE;      /* Temporarily print to screen */

   set_text_attrx(REG_ATTR) ;
   // CurPos(1, 1);
   // EraEop();
   dclrscr() ;
   menucleared = TRUE;

   PChar(stackname[s]);
   PrintNumber(&stack[s]);

   MessageWait("");

   WorkScreen();

   printtoscreen = FALSE;     /* Reset print to screen */

}




/*
 *    **************************************************
 *    *                                                *
 *    *       Print Number [*number] on Printer        *
 *    *                                                *
 *    **************************************************
 */

extern void PrintNumber(NORMTYPE *nbr)

{

   long exponent;
   int i, digits, chrcount;

   prevchr = '0';

   PString(": ");
   ppos = SIGNPRTCOL;

   chrcount = 0;
   digits = nbr->digits;
   exponent = nbr->exp;

   if (nbr->digits == 0) {
      PString(" 0\r\n\n");
      return;
      }


   if ( (scinotation)
         ||
        (exponent < minfloatprn)
         ||
        (exponent > maxfloatprn) )

      {                             /* Scientific Notation */

      for (i = 1; i < groupsize; i++)
         PCharWrap(' ');

      PSign(nbr->sign);                   /* Print sign */

      PCharWrap(nbr->man[0]+ '0');        /* First digit and decimal point */
      PCharWrap(PRINTDECIMAL);

      if (digits <= 1)                    /* If no more digits, */
         PCharWrap('0');                  /*  print one zero    */
      else {
         for (i = 1; i < digits; i++) {   /*  print remaining digits */
            PCharWrap(nbr->man[i]+ '0');
            if (! (i % groupsize))
               PCharWrap(' ');
            }
         }
      PExponent(nbr->exp - 1L);           /* Print exponent */

      }                             /* Scientific Notation end */
   else
      {                             /* Floating Decimal */
      if (exponent <= 0L)

         {                                /* Number < 1 */

         for (i = 1; i < groupsize; i++)
            PCharWrap(' ');

         PSign(nbr->sign);                   /* Print sign */
         PCharWrap('0');                     /* Zero left of decimal looks good */
         PCharWrap(PRINTDECIMAL);            /* Decimal point */

         for (i = 0; i > (int)exponent; i--) {  /* Zeros to left of number */
            PCharWrap('0');
            if (! (++chrcount % groupsize))
               PCharWrap(' ');
            }

         for (i = 0; i < digits; i++) {      /* Print mantissa digits */
            PCharWrap(nbr->man[i] + '0');
            if (! (++chrcount % groupsize))
               PCharWrap(' ');
            }
         }                                /* Number < 1 */
      else
         {                                /* Number >= 1 */
         /* Calculate digit allignment   */
         i = (int)((3000000L - exponent) % (long)groupsize);
         while (i-- > 0)                     /*  here 1 <= exponent <= maxfloatprn */
            PCharWrap(' ');

         PSign(nbr->sign);                   /* Print sign */

         chrcount = (int)exponent;           /* Distance from decimal point to    */
                                             /*  print groups of groupsize digits */

         for (i = 0; i < (int)exponent; i++) {  /* Digits to left of decimal */
            if (i < digits)
               PCharWrap(nbr->man[i] + '0'); /* Mantissa digits while they last */
            else
               PCharWrap('0');               /* Zeros if mantissa exhausted */

            if (! (--chrcount % groupsize))  /* Space between groups */
               if (chrcount)                 /*  of groupsize digits */
                  PCharWrap(' ');
            }

         chrcount = 0;                       /* Distance from decimal */

         if (i < digits) {                   /* If digits to right of decimal */
            PCharWrap(PRINTDECIMAL);         /*  print decimal point          */

            for (i = (int)exponent; i < digits; i++) {   /* digits to right of decimal */
               PCharWrap(nbr->man[i] + '0');
               if (! (++chrcount % groupsize))
                  PCharWrap(' ');
               }
            }

         }                                /* Number >= 1 */

      }                             /* Floating Decimal */

   NewLine();

}

/*
 *    **************************************************
 *    *                                                *
 *    *         Print Char & Wrap Print Line           *
 *    *                                                *
 *    **************************************************
 */

static void PCharWrap(int chr)

{

   int col;

   if ( (ppos > MAXPRTCOL)
         &&
        (chr != ' ')
         &&
        ( (prevchr == ' ')
           ||
          (prevchr == PRINTDECIMAL) ) ) {
      PString("\r\n");
      for (col = 1; col < MINPRTCOL; col++)
         PChar(' ');
      ppos = MINPRTCOL;
      }

   PChar(chr);
   ppos++;
   prevchr = chr;

}




/*
 *    **************************************************
 *    *                                                *
 *    *              Print Sign Character              *
 *    *                                                *
 *    **************************************************
 */
static void PSign(int sign)
{
   if (sign == '-')
      PCharWrap('-');
   else
      PCharWrap(' ');
}

/*
 *    **************************************************
 *    *                                                *
 *    *            Print Exponent on Printer           *
 *    *                                                *
 *    **************************************************
 */
static void PExponent(long exponent)
{
   if (prevchr != ' ')
      PChar(' ');
   PChar('e');
   PInteger(exponent);
}

/*
 *    **************************************************
 *    *                                                *
 *    *               Writing routines                 *
 *    *                                                *
 *    **************************************************
 */

/*
 *    **************************************************
 *    *                                                *
 *    *          Write Registers [lo to hi]            *
 *    *                                                *
 *    **************************************************
 */
extern void WriteReg(int lo, int hi)
{
   int r;
   for (r = lo; r <= hi; r++) {
      CurPos(r + 4, SIGNDISPCOL);
      WriteNumber(&reg[r]);
      // dprints(r + 4, SIGNDISPCOL, &reg[r]);
      }
}

/*
 *    **************************************************
 *    *                                                *
 *    *            Write Stack [lo to hi]              *
 *    *                                                *
 *    **************************************************
 */
extern void WriteStack(int lo, int hi)
{
   int s;

   for (s = hi; s >= lo; s--) {
      CurPos(XSIGNROW - s, SIGNDISPCOL);
      WriteNumber(&stack[s]);
      // dprints(XSIGNROW - s, SIGNDISPCOL, &stack[s]);
      }
}

/*
 *    **************************************************
 *    *                                                *
 *    *        Write Number [*number] on Screen        *
 *    *                                                *
 *    **************************************************
 */

extern void WriteNumber(NORMTYPE *nbr)

{
   long exponent;
   int i, digits;

   EraEol();

   if (nbr->digits == 0) {
      WString(" 0");
      return;
      }

   if (nbr->sign == '-')
      WChar('-');
   else
      WChar(' ');
   ppos = MINDISPCOL;

   digits = nbr->digits;
   exponent = nbr->exp;

   if ( (scinotation)
         ||
        (exponent < MINFLOATDSP)
         ||
        (exponent > MAXFLOATDSP)
         ||
        (exponent > normprec) )

      {                             /* Scientific Notation */

      WChar(nbr->man[0]+ '0');         /* First digit and decimal point */
      WChar(DISPDECIMAL);
      ppos += 2;

      if (digits <= 1) {                  /* If no more digits, */
         WChar('0');                      /*  print one zero    */
         ppos++;
         }
      else
         for (i = 1; i < digits; i++) {   /*  print remaining digits */
            WChar(nbr->man[i]+ '0');
            if (++ppos >= MAXDISPCOL)
               break;
            }
      WExponent(nbr->exp - 1L);           /* Write exponent */

      }                             /* Scientific Notation end */

   else

      {                             /* Floating Decimal */
      if (exponent <= 0L)

         {                             /* Number < 1 */

         WChar(DISPDECIMAL);                 /* Decimal point */
         ppos++;

         for (i = 0; i > (int)exponent; i--) {  /* Zeros to left of number */
            WChar('0');
            ppos++;
            }

         for (i = 0; i < digits; i++) {      /* Write number digits */
            WChar(nbr->man[i] + '0');
            if (++ppos > 79)                 /* Until end of digits or line */
               break;
            }
         }                             /* Number < 1 end */

      else

         {                             /* Number >= 1 */

         for (i = 0; i < (int)exponent; i++) {  /* Digits to left of decimal */
            if (i < digits)
               WChar(nbr->man[i] + '0');  /* Mantissa digits while they last */
            else
               WChar('0');                /* Zeros if mantissa exhausted */
            ppos++;
            }

         if (i < digits) {                /* Write digits to right of decimal */
            WChar(DISPDECIMAL);
            ppos++;
            for (i = (int)exponent; i < digits; i++) {
               WChar(nbr->man[i] + '0');
               if (++ppos > 79)
                  break;
               }
            }
         }                             /* Number >= 1 end */

      }                             /* Floating Decimal end */

}




/*
 *    **************************************************
 *    *                                                *
 *    *            Write Exponent on Screen            *
 *    *                                                *
 *    **************************************************
 */
static void WExponent(long exponent)
{
   WString(" e");
   WInteger(exponent);
}

/*
 *    **************************************************
 *    *                                                *
 *    *             Get Prompted Character             *
 *    *                                                *
 *    **************************************************
 */
extern int GetPrompt(void)
{
   if (menucleared) {            /* Restore menu if cleared */
      OnScreenMenu();
      menucleared = FALSE;
      }

   if (charpresent) {            /* Character passed from routine */
      charpresent = FALSE;
      return(chr);
      }
   else {                        /* Get character */
      Message("(Esc to Exit)");
      return(GetChar());
      }
}

/*
 *    **************************************************
 *    *                                                *
 *    *                Overflow Message                *
 *    *                                                *
 *    **************************************************
 */
extern void Overflow(void)
{
   MessageWait("** Overflow **");
}

/*
 *    **************************************************
 *    *                                                *
 *    *             Divide By Zero Message             *
 *    *                                                *
 *    **************************************************
 */
extern void DivideByZero(void)
{
   MessageWait("** Divide by zero **");
}

/*
 *    **************************************************
 *    *                                                *
 *    *              Zero Argument Message             *
 *    *                                                *
 *    **************************************************
 */
extern void ZeroArgument(void)
{
   MessageWait("** Zero Argument **");
}

/*
 *    **************************************************
 *    *                                                *
 *    *            Negative Argument Message           *
 *    *                                                *
 *    **************************************************
 */
extern void NegativeArgument(void)
{
   MessageWait("** Negative Argument **");
}

/*
 *    **************************************************
 *    *                                                *
 *    *          Argument not Integer Message          *
 *    *                                                *
 *    **************************************************
 */
extern void ArgumentNotInteger(void)
{
   MessageWait("** Argument not Integer **");
}

/*
 *    **************************************************
 *    *                                                *
 *    *                Argument Invalid                *
 *    *                                                *
 *    **************************************************
 */
extern void ArgumentInvalid(void)
{
   MessageWait("** Argument Invalid **");
}

/*
 *    **************************************************
 *    *                                                *
 *    *          Insufficient Memory Message           *
 *    *                                                *
 *    **************************************************
 */
extern void MemoryError(void)
{
   MessageWait("** Insufficient Memory **");
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
 *    *                On Screen Menu                  *
 *    *                                                *
 *    **************************************************
 */
extern void OnScreenMenu(void)
{
   set_text_attrx(MENU_ATTR) ;
   CurPos(19, 1);
   EraEop();
   WriteAt(20, 1, "0-9.E  Number   + >Add        X >Xchg X R   V >View full");
   WriteAt(21, 1, "    S  ChgSign  - >Sub   Lft/Rt >Xchg X Y   F >Float/Sci");
   WriteAt(22, 1, "BkSpc  Clear X  * >Mul   Up/Dwn >Roll UpDn  G >Group 3/5");
   WriteAt(23, 1, "Enter >Enter    / >Div     PgUp >Store      P >Print");
   WriteAt(24, 1, "    L >Last X   C >Clear   PgDn >Recall     D >Disk/Print");

   WriteAt(19, 58, "(M rotates Fn key menu)");

   if (! menunbr) {
      WriteAt(20, 61, "F1 >Help  F2 >Y^X");
      WriteAt(21, 61, "F3 >�X    F4 >X�");
      WriteAt(22, 61, "F5 >1/X   F6 >X!");
      WriteAt(23, 61, "F7 >INT   F8 >FRAC");
      WriteAt(24, 61, "F9 >�     F0 >e");
      }
   else {
      WriteAt(20, 60, "^F1 >sinX ^F2 >asinX");
      WriteAt(21, 60, "^F3 >cosX ^F4 >acosX");
      WriteAt(22, 60, "^F5 >tanX ^F6 >atanX");
      WriteAt(23, 60, "^F7 >logX ^F8 >10^X");
      WriteAt(24, 60, "^F9 >lnx  ^F0 >e^X");
      }
}

/*
 *    **************************************************
 *    *                                                *
 *    *             Display Help Screen 1              *
 *    *                                                *
 *    **************************************************
 */
static void HelpScreen1(void)
{

   Heading1();

   set_text_attrx(HELP_ATTR) ;
   WriteAt( 3,  1, "BIGCALC works like an H-P calculator with stack and ten m"
                   "emory registers.");
   WriteAt( 4,  1, "Maximum precision settable from ");
   WInteger((long)MINPREC);
   WString(" to ");
   WInteger((long)MAXNORM);
   WString(" digits. Exponents to �");
   WInteger(MAXEXP);
   WString(".");
   WriteAt( 5,  1, "Execute BIGCALC like this:");

   WriteAt( 7,  3, "BIGCALC precision   (precision is maximum number of digit"
                   "s, default is ");
   WInteger((long)DEFAULTPREC);
   WString(")");

   WriteAt( 9,  1, "BIGCALC has +, -, x, �, �X, X�, X!, Y^X, logs, trig, pi, "
                   "e, print, much more.");
   WriteAt(10,  1, "All trig functions work in radians. BIGCALC is fast, but "
                   "some operations on");
   WriteAt(11,  1, "large numbers may take up to several minutes. You can abo"
                   "rt long calculations");
   WriteAt(12,  1, "by pressing the Escape key. Press F1 for on-screen help. "
                   "Registers can be");
   WriteAt(13,  1, "viewed in full, or printed to printer or disk.");

   WriteAt(15,  1, "You can specify floating decimal or scientific notation d"
                   "isplay. Large numbers");
   WriteAt(16,  1, "display in scientific notation to about 65 digits all the"
                   " time, but the view");
   WriteAt(17,  1, "and print commands show full precision in 3 or 5 digit gr"
                   "oups.");

   WriteAt(19,  1, " Judson D. McClendon           $20 gets you a disk with t"
                   "he complete C source.");
   WriteAt(20,  1, " Sun Valley Systems");
   WriteAt(21,  1, " 4522 Shadow Ridge Pkwy        There is no warranty of an"
                   "y kind.");
   WriteAt(22,  1, " Pinson, AL 35126-2192         The author assumes no resp"
                   "onsibility for the");
   WriteAt(23,  1, "     205-680-0460              use of this program.");

}

/*
 *    **************************************************
 *    *                                                *
 *    *             Display Help Screen 2              *
 *    *                                                *
 *    **************************************************
 */
static void HelpScreen2(void)
{
   Heading1();
   set_text_attrx(HELP_ATTR) ;

   WriteAt( 3,  1, "  KEY  FUNCTION     KEY  FUNCTION      "
                   "KEY  FUNCTION        KEY  FUNCTION");

   WriteAt( 4, 45, "(M rotates Fn key menu)");

   WriteAt( 5,  1, "0-9.E  Enter number   V >View number    F1 >Help         "
                   "   ^F1 >sin X");
   WriteAt( 6,  1, "    S  change Sign    F >Float/Sci      F2 >Power Y^X    "
                   "   ^F2 >arcsin X");
   WriteAt( 7,  1, "BkSpc  CLX/Backspace  G >Group 3/5      F3 >Sq root �x   "
                   "   ^F3 >cos X");
   WriteAt( 8,  1, "Enter >Enter          P >Print          F4 >Square X�    "
                   "   ^F4 >arccos X");
   WriteAt( 9,  1, "    + >Add Y + X      D >Disk/Print     F5 >Recip 1 � X  "
                   "   ^F5 >tan X");
   WriteAt(10,  1, "    - >Sub Y - X   PgUp >Store X        F6 >Fact X!      "
                   "   ^F6 >arctan X");
   WriteAt(11,  1, "    * >Mul Y x X   PgDn >Recall X       F7 >Integer XXX. "
                   "   ^F7 >Common log X");
   WriteAt(12,  1, "    / >Div Y � X      X >eXchg X & Reg  F8 >Fraction .XXX"
                   "   ^F8 >Exponent 10^X");
   WriteAt(13,  1, "    C >Clear      Up/Dn >Roll Up/Dn     F9 >Recall � to X"
                   "   ^F9 >Natural log X");
   WriteAt(14,  1, "    L >Last X    Lft/Rt >Exchg X & Y   F10 >Recall e to X"
                   "  ^F10 >Exponent e^X");

   WriteAt(16,  5, "To enter a number just begin typing it in. Use Backspace "
                   "to back up.");
   WriteAt(17,  5, "To enter an exponent press E then enter the exponent.");
   WriteAt(18,  5, "Press S while entering mantissa or exponent to change res"
                   "pective sign.");
   WriteAt(19,  5, "If you start a number with E a mantissa of 1 is assumed.");
   WriteAt(20,  5, "Functions marked '>' complete a number being entered befo"
                   "re acting.");
   WriteAt(21,  5, "View displays any register to full precision.");
   WriteAt(22,  5, "Clear, Print, View, Store, Recall & eXchange ask for affe"
                   "cted register.");
   WriteAt(23,  5, "BackSpace backs up while entering number, otherwise acts "
                   "as Clear X.");
   WriteAt(24,  5, "M rotates function key menus. ^ means control: ^F1 = ctrl"
                   "-F1");

}

/*
 *    **************************************************
 *    *                                                *
 *    *             Display Help Screen 3              *
 *    *                                                *
 *    **************************************************
 */
static void HelpScreen3(void)
{
   Heading1();

   set_text_attrx(HELP_ATTR) ;
   WriteAt( 3,  1, "This program emulates a Hewlett-Packard calculator w"
                   "ith four register stack");
   WriteAt( 4,  1, "and ten memory registers.  The stack consists of the"
                   " X, Y, Z & T registers.");
   WriteAt( 5,  1, "The memory registers are 0-9.");

   WriteAt( 7,  1, "The X register is 'where the action is'.  When you e"
                   "nter a number it is entered");
   WriteAt( 8,  1, "into the X register.  If X already contains a number"
                   ", that number is 'pushed'");
   WriteAt( 9,  1, "up to Y, Y is pushed to Z, Z to T, and the contents "
                   "of T is lost.  The 'Enter'");
   WriteAt(10,  1, "key pushes the stack and duplicates X into Y. A numb"
                   "er entered into X after");
   WriteAt(11,  1, "Enter or ClearX DOES NOT push the stack. When you us"
                   "e a function that acts on");
   WriteAt(12,  1, "two numbers (like + or -) it always acts on X & Y (e"
                   "xcept for Store, Recall &");
   WriteAt(13,  1, "eXchange which act on X and a memory register).  The"
                   " result is put in X, and");
   WriteAt(14,  1, "the stack is 'Dropped'.  That is, Z copies to Y and "
                   "T duplicates into Z.");

   WriteAt(16,  1, "Numbers are displayed in 'Floating Decimal' or 'Scie"
                   "ntific Notation' format.");
   WriteAt(17,  1, "Floating Decimal looks like this:    1.2     123.45 "
                   "    -12300000000  0.000002");
   WriteAt(18,  1, "Scientific Notation looks like this: 1.2 e0  1.2345 "
                   "e2  -1.23 e10     2.0 e-6");
   WriteAt(19,  1, "Very long numbers take longer to compute, of course."
                   " You can abort a long");
   WriteAt(20,  1, "calculation by pressing the Escape key.");

   WriteAt(22,  1, "If a number is too large or small for the screen, Sc"
                   "ientific Notation is used.");
   WriteAt(23,  1, "All results are truncated except for Y^X which is ro"
                   "unded: 5 up 4 down.");

}

/*
 *    **************************************************
 *    *                                                *
 *    *                Initial Screen                  *
 *    *                                                *
 *    **************************************************
 */
extern void InitialScreen(void)
{
   HelpScreen1();
   MessageWait("");
   WorkScreen();
}

/*
 *    **************************************************
 *    *                                                *
 *    *                  Help Screen                   *
 *    *                                                *
 *    **************************************************
 */

void HelpScreen(void)

{

   int screen;

   menucleared = TRUE;

   screen = 2;

   do {
      switch (screen) {
         case (1):
            HelpScreen1();
            break;
         case (2):
            HelpScreen2();
            break;
         case (3):
            HelpScreen3();
            break;
         }  /* switch */

      Message("(Press PgDn=Next page, PgUp=Prev page, Esc=Exit to BIGCALC)");

      while (TRUE) {
         chr = GetChar();
         if (chr == PGDN || chr == '+') {
            if (++screen > 3)
               screen = 1;
            break;
            }
         else if (chr == PGUP || chr == '-') {
            if (--screen < 1)
               screen = 3;
            break;
            }
         else if (chr == ESCAPE)
            break;
         else 
            ;
         }  /* while (TRUE) */

      } while (chr != ESCAPE);

   WorkScreen();

}




/*
 *    **************************************************
 *    *                                                *
 *    *             Screen Header Line 1               *
 *    *                                                *
 *    **************************************************
 */
extern void Heading1(void)
{
   set_text_attrx(TEXT_ATTR) ;
   ScrClr();
   set_text_attrx(TITLE_ATTR) ;
   WriteCenter(1, TITLE);
}

/*
 *    **************************************************
 *    *                                                *
 *    *             Screen Header Line 2               *
 *    *                                                *
 *    **************************************************
 */
extern void Heading2(void)
{
   set_text_attrx(HEADER_ATTR) ;
   CurPos(2, 1);
   EraEol();
   WriteAt(2, 8, "Precision is ");
   WInteger((long) normprec);
   WString(" digits,  Digit grouping is ");
   WInteger((long) groupsize);
   WString(",  Print to ");
   WString(printid);
}

/*
 *    **************************************************
 *    *                                                *
 *    *              Display Work Screen               *
 *    *                                                *
 *    **************************************************
 */

extern void WorkScreen(void)

{

   int r, s;

   Heading1();
   Heading2();

   set_text_attrx(LOGO_ATTR) ;
   WriteAt(3, 1, "============================="
                 "  R E G I S T E R S  "
                 "=============================");
   set_text_attrx(REG_ATTR) ;
   for (r = 0; r <= 9; r++) {
      CurPos(4 + r, 1);
      WChar(r + '0');
      WChar(':');
      WriteReg(r, r);
   }

   set_text_attrx(LOGO_ATTR) ;
   WriteAt(14, 1, "================================="
                  "  S T A C K  "
                  "=================================");
   set_text_attrx(REG_ATTR) ;
   for (s = 3; s >= 0; s--) {
      CurPos(XSIGNROW - s, 1);
      WChar(stackname[s]);
      WChar(':');
      WriteStack(s, s);
   }
   set_text_attrx(TEXT_ATTR) ;

}
