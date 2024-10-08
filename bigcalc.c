/*
 *    **************************************************
 *    *                                                *
 *    *                   BIGCALC.C                    *
 *    *                                                *
 *    *          Extended Precision Calculator         *
 *    *                                                *
 *    *                  Main Module                   *
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
#include "conio32.h"

/*
 *    **************************************************
 *    *        BIGCALC.C Function Declarations         *
 *    **************************************************
 */

static  void Initialize(int argc,char *argv);

static  void Add(void);
static  void Subtract(void);
static  void Multiply(void);
static  void Divide(void);
static  void Power(void);
static  void SquareRoot(void);
static  void Square(void);
static  void Reciprocal(void);
static  void Factorial(void);
static  void IntegerPart(void);
static  void FractionPart(void);
static  void Sin(void);
static  void ArcSin(void);
static  void Cos(void);
static  void ArcCos(void);
static  void Tan(void);
static  void ArcTan(void);
static  void Log(void);
static  void Exp10(void);
static  void Ln(void);
static  void ExpE(void);
static  void RecallPi(void);
static  void RecallE(void);
static  void RecallLastX(void);
static  void ChangeSign(void);
static  void PrintDisk(void);
static  void GroupSize(void);
static  void MenuRoll(void);
static  void ViewReg(void);
static  void AcceptX(void);
static  void ClearX(void);
static  void Enter(void);
static  void SciNotation(void);
static  void Clear(void);
static  void Print(void);
static  void StoreX(void);
static  void AddXReg(void);
static  void SubtractXReg(void);
static  void MultiplyXReg(void);
static  void DivideXReg(void);
static  void RecallReg(void);
static  void AddRegX(void);
static  void SubtractRegX(void);
static  void MultiplyRegX(void);
static  void DivideRegX(void);
static  void ExchangeXY(void);
static  void ExchangeXReg(void);
static  void RollDown(void);
static  void RollUp(void);
static  void PushStack(void);
static  void DropStack(void);


/*
 *    **************************************************
 *    *                                                *
 *    *           Global Variable Definition           *
 *    *                                                *
 *    **************************************************
 */

NORMTYPE
   stack[4],               /* Stack registers  */
   reg[10],                /* Memory registers */
   lastx;                  /* Last X register  */

WORKTYPE
   work[3];                /* Work registers   */

int
   normprec = 0,           /* Normal precision  */
   compprec = 0,           /* Compute precision */
   workprec = 0,           /* Work precision    */
   groupsize = 0,          /* Digit group size  */
   entrysignrow,           /* Row for X entry   */
   menunbr = 0,            /* Menu number       */
   chr = 0;                /* Input Character   */

long
   minfloatprn,            /* Min exp for float */
   maxfloatprn;            /* Max exp for float */

FILE
   *printfile;             /* Print file handle */

char
   stackname[4]            /* Stack register names */
      = {'X','Y','Z','T'},
   printid[15]             /* Print file name      */
      = "Printer";

BOOLEAN
   stacklift = TRUE,       /* Lift stack for new X if TRUE */
   scinotation = FALSE,    /* Force sci notation if TRUE   */
   charpresent = FALSE,    /* Character present if TRUE    */
   menucleared = TRUE,     /* Screen menu cleared if TRUE  */
   printtoscreen = FALSE;  /* Print to screen if TRUE      */

/*
 *    **************************************************
 *    *                                                *
 *    *                     Main                       *
 *    *                                                *
 *    **************************************************
 */
int main(int argc,char *argv[])
{
   Initialize(argc, argv[1]);

   chr = GetPrompt();

   while (chr != ESCAPE) {

      switch (chr) {

         case (ADD):
            Add();               /* Add Y + X */
            break;

         case (SUBTRACT):
            Subtract();          /* Subtract Y - X */
            break;

         case (MULTIPLY):
            Multiply();          /* Multiply Y * X */
            break;

         case (DIVIDE):
            Divide();            /* Divide Y / X */
            break;

         case (HELP):
            HelpScreen();        /* Help Screen */
            break;

         case (POWER):
            Power();             /* Power (Y^X) */
            break;

         case (SQUAREROOT):
            SquareRoot();        /* Square Root X */
            break;

         case (SQUARE):
            Square();            /* Square X */
            break;

         case (RECIPROCAL):
            Reciprocal();        /* Reciprocal X */
            break;

         case (FACTORIAL):
            Factorial();         /* Factorial X */
            break;

         case (INTEGER):
            IntegerPart();       /* Integer Part X */
            break;

         case (FRACTION):
            FractionPart();      /* Fraction Part X */
            break;

         case (SIN):
            Sin();               /* Sine X */
            break;

         case (ARCSIN):
            ArcSin();            /* ArcSine X */
            break;

         case (COS):
            Cos();               /* Cosine X */
            break;

         case (ARCCOS):
            ArcCos();            /* ArcCosine X */
            break;

         case (TAN):
            Tan();               /* Tangent X */
            break;

         case (ARCTAN):
            ArcTan();            /* ArcTangent X */
            break;

         case (LOG):
            Log();               /* Common Log X */
            break;

         case (EXP10):
            Exp10();             /* Exponent 10^X */
            break;

         case (LN):
            Ln();                /* Natural Log X */
            break;

         case (EXPE):
            ExpE();              /* Exponent e^X */
            break;

         case (RECALLPI):
            RecallPi();          /* Recall pi */
            break;

         case (RECALLE):
            RecallE();           /* Recall e */
            break;

         case (LASTX):
            RecallLastX();       /* Recall Last X */
            break;

         case (CHGSIGN):
            ChangeSign();        /* Change sign X */
            break;

         case (PRINTDISK):
            PrintDisk();         /* Toggle Print to Disk */
            break;

         case (GROUPSIZE):
            GroupSize();         /* Toggle Group Size (3/5) */
            break;

         case (MENUROLL):
            MenuRoll();          /* Roll Function Key Menu */
            break;

         case (VIEWREG):
            ViewReg();           /* View Register on Screen */
            break;

         case ('0'):
         case ('1'):
         case ('2'):
         case ('3'):
         case ('4'):
         case ('5'):
         case ('6'):
         case ('7'):
         case ('8'):
         case ('9'):
         case ('.'):
         case ('E'):
            AcceptX();           /* Accept new X from keyboard with first */
            break;               /*  character passed to AcceptX routine */

         case (CLEARX):
            ClearX();            /* Clear X to zero */
            break;

         case (ENTER):
            Enter();             /* Push up stack */
            break;

         case (SCINOT):
            SciNotation();       /* Use Scientific Notation */
            break;

         case (CLEAR):
            Clear();             /* Clear (prompt for what) */
            break;

         case (PRINT):
            Print();             /* Print on printer (prompt for what) */
            break;

         case (STOREX):
            StoreX();            /* Store X in register (prompt for which) */
            break;

         case (RECALLREG):
            RecallReg();         /* Recall register to X (prompt for which) */
            break;

         case (XCHGXY1):
         case (XCHGXY2):
            ExchangeXY();        /* Exchange X and Y */
            break;

         case (XCHGXREG):
            ExchangeXReg();      /* Exchange X and Reg (prompt for which) */
            break;

         case (ROLLDOWN):
            RollDown();          /* Roll stack down */
            break;

         case (ROLLUP):
            RollUp();            /* Roll stack up */
            break;

         default:
            ;              /* Unknown key */

         }  /* end switch */

      // flushall();

      chr = GetPrompt();

      }  /* while */


   ScrTerm();

   // exit(0);
   return 0;
}

/*
 *    **************************************************
 *    *                                                *
 *    *              Initialize Variables              *
 *    *                                                *
 *    **************************************************
 */
static void Initialize(int argc, char *argv)
{
   ScrInit();                    /* Initialize screen drivers */

   ExtendedInitConstants();      /* Initialize extended constants */

   if (argc > 1)
      while (*argv) {            /* Convert *argv to number */
         if (isdigit(*argv)) {
            normprec = (normprec * 10) + (*argv - '0');
            if (normprec <= MAXNORM)
               argv++;
               continue;
            }
         normprec = 0;
         break;
         }

   if ( (normprec < MINPREC)
         ||
        (normprec > MAXNORM) )
      normprec = DEFAULTPREC;

   compprec = COMPPREC;
   workprec = WORKPREC;

   ClearStack(0, 3);             /* Initialize work areas */
   ClearReg(0, 9);
   lastx = stack[0];
   // printfile = stdprn;
                                 /* Allow full screen entry for big numbers */
   if (normprec > SIZEOFSMALL)
      entrysignrow = ENTRYSIGNROWBIG;
   else
      entrysignrow = ENTRYSIGNROWSMALL;

   GroupSize();                  /* Toggle group size to 5 & set xxxfloatprn */

   InitialScreen();              /* Welcome screen */
}

/*
 *    **************************************************
 *    *                                                *
 *    *                Process Routines                *
 *    *                                                *
 *    **************************************************
 */



/*
 *    **************************************************
 *    *                                                *
 *    *                 Add X = Y + X                  *
 *    *                                                *
 *    **************************************************
 */

static void Add(void)

{

   MoveStackWork(0, 0);
   MoveStackWork(1, 1);

   if (ExtendedAdd() ) {
      lastx = stack[0];
      DropStack();
      MoveWorkStack(2, 0);
      WriteStack(0, 2);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *              Subtract X = Y - X                *
 *    *                                                *
 *    **************************************************
 */

static void Subtract(void)

{

   MoveStackWork(0, 0);
   MoveStackWork(1, 1);

   if (ExtendedSubtract() ) {
      lastx = stack[0];
      DropStack();
      MoveWorkStack(2, 0);
      WriteStack(0, 2);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *              Multiply X = Y * X                *
 *    *                                                *
 *    **************************************************
 */

static void Multiply(void)

{

   MessageEsc("Multiplying");

   MoveStackWork(0, 0);
   MoveStackWork(1, 1);

   if (ExtendedMultiply() ) {
      lastx = stack[0];
      DropStack();
      MoveWorkStack(2, 0);
      WriteStack(0, 2);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *               Divide X = Y / X                 *
 *    *                                                *
 *    **************************************************
 */

static void Divide(void)

{

   MessageEsc("Dividing");

   MoveStackWork(0, 0);
   MoveStackWork(1, 1);

   if (ExtendedDivide() ) {
      lastx = stack[0];
      DropStack();
      MoveWorkStack(2, 0);
      WriteStack(0, 2);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *                  Power (Y^X)                   *
 *    *                                                *
 *    **************************************************
 */

static void Power(void)

{

   MessageEsc("Computing Y^X");

   MoveStackWork(0, 0);
   MoveStackWork(1, 1);

   if (ExtendedPower() ) {
      if (ExtendedRound(2)) {
         lastx = stack[0];
         DropStack();
         MoveWorkStack(2, 0);
         WriteStack(0, 2);
         stacklift = TRUE;
         }
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *               Square Root of X                 *
 *    *                                                *
 *    **************************************************
 */

static void SquareRoot(void)

{

   MessageEsc("Computing �X");

   MoveStackWork(0, 0);
   if (ExtendedSquareRoot() ) {
      lastx = stack[0];
      MoveWorkStack(2, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *                 Square of X                    *
 *    *                                                *
 *    **************************************************
 */

static void Square(void)

{

   MessageEsc("Computing X�");

   MoveStackWork(0, 0);
   MoveStackWork(0, 1);

   if (ExtendedMultiply() ) {
      lastx = stack[0];
      MoveWorkStack(2, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *                Reciprocal of X                 *
 *    *                                                *
 *    **************************************************
 */

static void Reciprocal(void)

{

   MessageEsc("Computing 1/X");

   MoveStackWork(0, 0);

   if (ExtendedReciprocal() ) {
      lastx = stack[0];
      MoveWorkStack(2, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *                Factorial of X                  *
 *    *                                                *
 *    **************************************************
 */

static void Factorial(void)

{

   MessageEsc("Computing X!");

   MoveStackWork(0, 0);

   if (ExtendedFactorial() ) {
      lastx = stack[0];
      MoveWorkStack(2, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *           Extract Integer Part of X            *
 *    *                                                *
 *    **************************************************
 */

static void IntegerPart(void)

{

   MoveStackWork(0, 0);

   if (ExtendedIntegerPart() ) {
      lastx = stack[0];
      MoveWorkStack(0, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *          Extract Fraction Part of X            *
 *    *                                                *
 *    **************************************************
 */

static void FractionPart(void)

{

   MoveStackWork(0, 0);

   if (ExtendedFractionPart() ) {
      lastx = stack[0];
      MoveWorkStack(0, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *                   Sine of X                    *
 *    *                                                *
 *    **************************************************
 */

static void Sin(void)

{

   MessageEsc("Computing SinX");

   MoveStackWork(0, 0);

   if (ExtendedSinCos(0) ) {
      lastx = stack[0];
      MoveWorkStack(2, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *                 ArcSine of X                   *
 *    *                                                *
 *    **************************************************
 */

static void ArcSin(void)

{

   MessageEsc("Computing ArcSinX");

   MoveStackWork(0, 0);

   if (ExtendedArcSinCos(0) ) {
      lastx = stack[0];
      MoveWorkStack(2, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *                  Cosine of X                   *
 *    *                                                *
 *    **************************************************
 */

static void Cos(void)

{

   MessageEsc("Computing CosX");

   MoveStackWork(0, 0);

   if (ExtendedSinCos(1) ) {
      lastx = stack[0];
      MoveWorkStack(2, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *                ArcCosine of X                  *
 *    *                                                *
 *    **************************************************
 */

static void ArcCos(void)

{

   MessageEsc("Computing ArcCosX");

   MoveStackWork(0, 0);

   if (ExtendedArcSinCos(1) ) {
      lastx = stack[0];
      MoveWorkStack(2, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *                 Tangent of X                   *
 *    *                                                *
 *    **************************************************
 */

static void Tan(void)

{

   MessageEsc("Computing TanX");

   MoveStackWork(0, 0);

   if (ExtendedTan() ) {
      lastx = stack[0];
      MoveWorkStack(2, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *                ArcTangent of X                 *
 *    *                                                *
 *    **************************************************
 */

static void ArcTan(void)

{

   MessageEsc("Computing ArcTanX");

   MoveStackWork(0, 0);

   if (ExtendedArcTan() ) {
      lastx = stack[0];
      MoveWorkStack(2, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *              Common Logarithm of X             *
 *    *                                                *
 *    **************************************************
 */

static void Log(void)

{

   MessageEsc("Computing LogX");

   MoveStackWork(0, 0);

   if (ExtendedLog() ) {
      lastx = stack[0];
      MoveWorkStack(2, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *           Common Antilogarithm of X            *
 *    *                                                *
 *    **************************************************
 */

static void Exp10(void)

{

   MessageEsc("Computing 10^X");

   MoveStackWork(0, 0);

   if (ExtendedExp10() ) {
      lastx = stack[0];
      MoveWorkStack(2, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *             Natural Logarithm of X             *
 *    *                                                *
 *    **************************************************
 */

static void Ln(void)

{

   MessageEsc("Computing lnX");

   MoveStackWork(0, 0);

   if (ExtendedLn() ) {
      lastx = stack[0];
      MoveWorkStack(2, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *          Natural Antilogarithm of X            *
 *    *                                                *
 *    **************************************************
 */

static void ExpE(void)

{

   MessageEsc("Computing e^X");

   MoveStackWork(0, 0);

   if (ExtendedExpE() ) {
      lastx = stack[0];
      MoveWorkStack(2, 0);
      WriteStack(0, 0);
      stacklift = TRUE;
      }

}




/*
 *    **************************************************
 *    *                                                *
 *    *                Recall pi to X                  *
 *    *                                                *
 *    **************************************************
 */

static void RecallPi(void)

{

   if (stacklift) {
      PushStack();
      WriteStack(1, 3);
      }

   ExtendedRecallPi(0);
   MoveWorkStack(0, 0);
   WriteStack(0, 0);
   stacklift = TRUE;

}




/*
 *    **************************************************
 *    *                                                *
 *    *                Recall e to X                   *
 *    *                                                *
 *    **************************************************
 */
static void RecallE(void)
{
   if (stacklift) {
      PushStack();
      WriteStack(1, 3);
      }

   ExtendedRecallE(0);
   MoveWorkStack(0, 0);
   WriteStack(0, 0);
   stacklift = TRUE;
}

/*
 *    **************************************************
 *    *                                                *
 *    *                Recall Last X                   *
 *    *                                                *
 *    **************************************************
 */
static void RecallLastX(void)
{
   if (stacklift) {
      PushStack();
      WriteStack(1, 3);
      }

   stack[0] = lastx;
   WriteStack(0, 0);
   stacklift = TRUE;
}

/*
 *    **************************************************
 *    *                                                *
 *    *               Change Sign of X                 *
 *    *                                                *
 *    **************************************************
 */
static void ChangeSign(void)
{
   if (stack[0].digits) {        /* Only if X non zero */
      stack[0].sign = FlipSign(stack[0].sign);
      CurPos(XSIGNROW, SIGNDISPCOL);
      if (stack[0].sign == '-')
         WChar('-');
      else
         WChar(' ');
      }

   stacklift = TRUE;
}

/*
 *    **************************************************
 *    *                                                *
 *    *              Toggle Print to Disk              *
 *    *                                                *
 *    **************************************************
 */
static void PrintDisk(void)
{
   // if (printid[0] == 'B') {
   //    fclose(printfile);                  /* BIGCALC.PRN open, close */
   //    strcpy(printid, "Printer");         /*  and switch to printer  */
   //    printfile = stdprn;
   //    }
   // else {                                 /* Open BIGCALC.PRN */
   //    strcpy(printid, "BIGCALC.PRN");
   //    if ((printfile = fopen(printid, "ab")) == NULL) {
   //       strcpy(printid, "Printer");
   //       printfile = stdprn;
   //       MessageWait("** Cannot open disk file **");
   //       }
   //    }

   Heading2();
}

/*
 *    **************************************************
 *    *                                                *
 *    *             Toggle Group Size (3/5)            *
 *    *                                                *
 *    **************************************************
 */
static void GroupSize(void)
{
   BOOLEAN flag;

   if (groupsize)             /* First time thru groupsize == 0 */
      flag = TRUE;
   else
      flag = FALSE;

   if (groupsize == 5) {
      groupsize = 3;
      minfloatprn = (long)normprec - 1077L;
      maxfloatprn = 1080L;
      }
   else {
      groupsize = 5;
      minfloatprn = (long)normprec - 1195L;
      maxfloatprn = 1200L;
      }

   if (flag)
      Heading2();
}

/*
 *    **************************************************
 *    *                                                *
 *    *             Roll Function Key Menu             *
 *    *                                                *
 *    **************************************************
 */
static void MenuRoll(void)
{
   menunbr = 1 - menunbr;     /* Toggle menunbr 0/1 */
   OnScreenMenu();
}

/*
 *    **************************************************
 *    *                                                *
 *    *             View Register on Screen            *
 *    *                                                *
 *    **************************************************
 */
static void ViewReg(void)
{
   int r;

   Message("Press to view (X,Y,Z,T, 0-9, Esc=Exit):");

   while ((chr = GetChar()) != ESCAPE) {

      switch (chr) {

         case ('X'):
            DisplayStack(0);
            return;

         case ('Y'):
            DisplayStack(1);
            return;

         case ('Z'):
            DisplayStack(2);
            return;

         case ('T'):
            DisplayStack(3);
            return;

         case ('0'):
         case ('1'):
         case ('2'):
         case ('3'):
         case ('4'):
         case ('5'):
         case ('6'):
         case ('7'):
         case ('8'):
         case ('9'):
            r = chr - '0';
            DisplayReg(r);
            return;

         default:
            ;

         }  /* switch */

      }  /* while */
}

/*
 *    **************************************************
 *    *                                                *
 *    *               Accept X from KBD                *
 *    *                                                *
 *    **************************************************
 */
static void AcceptX(void)
{
   NORMTYPE *temp;

   if ((temp = GETNORMTEMP(1)) == NULL) {
      MemoryError();
      return;
      }

   CurPos(entrysignrow, 1);      /* Clear area to enter new X */
   EraEop();
   menucleared = TRUE;
   WriteAt(entrysignrow, 1, "X:");
   Message("Entering X: S=ChgSign, E=Exp, BakSpc=Backup, Other=Complete,"
           " ESC=Exit");

   if (normprec > SIZEOFSMALL) {

      CurPos(entrysignrow - 1, 1);  /* Big numbers, use full screen */
      EraEol();
      WriteAt(entrysignrow - 1, 1, "========================="
                                   "  E N T E R I N G   X  "
                                   "=========================");
      if (ExtendedGetX() ) {
          if (stacklift)
             PushStack();
         MoveWorkStack(0, 0);
         stacklift = TRUE;
         }
      WorkScreen();
      }

   else {

      if (stacklift) {              /* Small numbers, use bottom of screen */
         *temp = stack[3];
         PushStack();
         WriteStack(1, 3);
         }
      if (ExtendedGetX() ) {
         MoveWorkStack(0, 0);
         stacklift = TRUE;
         }
      else
         if (stacklift) {
            DropStack();
            stack[3] = *temp;
            WriteStack(1, 3);
         }
      CurPos(entrysignrow, SIGNDISPCOL);
      EraEop();
      WriteStack(0, 0);
      }

   free(temp);

}

/*
 *    **************************************************
 *    *                                                *
 *    *                    Clear X                     *
 *    *                                                *
 *    **************************************************
 */

static void ClearX(void)

{

   ClearStack(0, 0);
   WriteStack(0, 0);
   stacklift = FALSE;

}




/*
 *    **************************************************
 *    *                                                *
 *    *                    Enter                       *
 *    *                                                *
 *    **************************************************
 */

static void Enter(void)

{

   PushStack();
   WriteStack(1, 3);
   stacklift = FALSE;

}




/*
 *    **************************************************
 *    *                                                *
 *    *          Toggle Scientific Notation            *
 *    *                                                *
 *    **************************************************
 */

static void SciNotation(void)

{

   if (scinotation)
      scinotation = FALSE;
   else
      scinotation = TRUE;

   WriteReg(0, 9);
   WriteStack(0, 3);

}




/*
 *    **************************************************
 *    *                                                *
 *    *        Clear (S, X,Y,Z,T, R, 0-9, All)         *
 *    *                                                *
 *    **************************************************
 */

static void Clear(void)

{

   int r;

   Message("Press to clear (S, X,Y,Z,T, R, 0-9, A=All, Esc=Exit):");

   while ((chr = GetChar()) != ESCAPE) {

      switch (chr) {

         case ('S'):
            ClearStack(0, 3);
            WriteStack(0, 3);
            return;

         case ('X'):
            ClearStack(0, 0);
            WriteStack(0, 0);
            return;

         case ('Y'):
            ClearStack(1, 1);
            WriteStack(1, 1);
            return;

         case ('Z'):
            ClearStack(2, 2);
            WriteStack(2, 2);
            return;

         case ('T'):
            ClearStack(3, 3);
            WriteStack(3, 3);
            return;

         case ('R'):
            ClearReg(0, 9);
            WriteReg(0, 9);
            return;

         case ('0'):
         case ('1'):
         case ('2'):
         case ('3'):
         case ('4'):
         case ('5'):
         case ('6'):
         case ('7'):
         case ('8'):
         case ('9'):
            r = chr - '0';
            ClearReg(r, r);
            WriteReg(r, r);
            return;

         case ('A'):
            ClearReg(0, 9);
            ClearStack(0, 3);
            WriteReg(0, 9);
            WriteStack(0, 3);
            return;

         default:
            ;

         }  /* switch */

      }  /* while */

}




/*
 *    **************************************************
 *    *                                                *
 *    *        Print (S, X,Y,Z,T, R, 0-9, All)         *
 *    *                                                *
 *    **************************************************
 */

static void Print(void)

{

   int r;

   Message("Press to print (S, X,Y,Z,T, R, 0-9, A=All, P=Page Eject,"
           " Esc=Exit):");

   while ((chr = GetChar()) != ESCAPE) {

      switch (chr) {

         case ('S'):
            PrintHeading();
            PrintStack(0, 3);
            return;

         case ('X'):
            PrintHeading();
            PrintStack(0, 0);
            return;

         case ('Y'):
            PrintHeading();
            PrintStack(1, 1);
            return;

         case ('Z'):
            PrintHeading();
            PrintStack(2, 2);
            return;

         case ('T'):
            PrintHeading();
            PrintStack(3, 3);
            return;

         case ('R'):
            PrintReg(0, 9);
            return;

         case ('0'):
         case ('1'):
         case ('2'):
         case ('3'):
         case ('4'):
         case ('5'):
         case ('6'):
         case ('7'):
         case ('8'):
         case ('9'):
            r = chr - '0';
            PrintHeading();
            PrintReg(r, r);
            return;

         case ('A'):
            PrintHeading();
            PrintReg(0, 9);
            PrintStack(0, 3);
            return;

         case ('P'):
            NewPage();
            return;

         default:
            ;

         }  /* switch */

      }  /* while */

}




/*
 *    **************************************************
 *    *                                                *
 *    *              Store X in Register               *
 *    *                                                *
 *    *             Allows + - * / as well             *
 *    *                                                *
 *    **************************************************
 */

static void StoreX(void)

{

   int r;

   Message("Store X to: Press Reg (0-9) or Operation (+,-,*,/)"
           " then Reg (0-9) or Esc:");

   while ((chr = GetChar()) != ESCAPE) {

      if (isdigit(chr) ) {
         r = chr - '0';
         reg[r] = stack[0];
         WriteReg(r, r);
         stacklift = TRUE;
         return;
         }

      switch (chr) {

         case (ADD):
            AddXReg();
            return;

         case (SUBTRACT):
            SubtractXReg();
            return;

         case (MULTIPLY):
            MultiplyXReg();
            return;

         case (DIVIDE):
            DivideXReg();
            return;

         default:
            ;

         }  /* switch */

      } /* while */

}




/*
 *    **************************************************
 *    *                                                *
 *    *               Add X to Register                *
 *    *                                                *
 *    **************************************************
 */

static void AddXReg(void)

{

   int r;

   Message("Add X to: Reg (0-9) or Esc:");

   while ((chr = GetChar()) != ESCAPE) {

      if (isdigit(chr) ) {
         r = chr - '0';
         MoveStackWork(0, 0);
         MoveRegWork(r, 1);

         if (ExtendedAdd() ) {
            MoveWorkReg(2, r);
            WriteReg(r, r);
            stacklift = TRUE;
            return;
            }
         return;
         }
      else
            ;

      }  /* while */

}




/*
 *    **************************************************
 *    *                                                *
 *    *           Subtract X from Register             *
 *    *                                                *
 *    **************************************************
 */

static void SubtractXReg(void)

{

   int r;

   Message("Subtract X from: Reg (0-9) or Esc:");

   while ((chr = GetChar()) != ESCAPE) {

      if (isdigit(chr) ) {
         r = chr - '0';
         MoveStackWork(0, 0);
         MoveRegWork(r, 1);

         if (ExtendedSubtract() ) {
            MoveWorkReg(2, r);
            WriteReg(r, r);
            stacklift = TRUE;
            return;
            }
         return;
         }
      else
            ;

      }  /* while */

}




/*
 *    **************************************************
 *    *                                                *
 *    *            Multiply X times Register           *
 *    *                                                *
 *    **************************************************
 */

static void MultiplyXReg(void)

{

   int r;

   Message("Multiply X times: Reg (0-9) or Esc:");

   while ((chr = GetChar()) != ESCAPE) {

      if (isdigit(chr) ) {
         MessageEsc("Multiplying");

         r = chr - '0';
         MoveStackWork(0, 0);
         MoveRegWork(r, 1);

         if (ExtendedMultiply() ) {
            MoveWorkReg(2, r);
            WriteReg(r, r);
            stacklift = TRUE;
            return;
            }
         return;
         }
      else
            ;

      }  /* while */

}




/*
 *    **************************************************
 *    *                                                *
 *    *             Divide X into Register             *
 *    *                                                *
 *    **************************************************
 */

static void DivideXReg(void)

{

   int r;

   Message("Divide X into: Reg (0-9) or Esc:");

   while ((chr = GetChar()) != ESCAPE) {

      if (isdigit(chr) ) {
         MessageEsc("Dividing");

         r = chr - '0';
         MoveStackWork(0, 0);
         MoveRegWork(r, 1);

         if (ExtendedDivide() ) {
            MoveWorkReg(2, r);
            WriteReg(r, r);
            stacklift = TRUE;
            return;
            }
         return;
         }
      else
            ;

      }  /* while */

}




/*
 *    **************************************************
 *    *                                                *
 *    *             Recall Register to X               *
 *    *                                                *
 *    **************************************************
 */

static void RecallReg(void)

{

   int r;

   Message("Recall to X: Press Reg (0-9) or Operation (+,-,*,/)"
           " then Reg (0-9) or Esc:");

   while ((chr = GetChar()) != ESCAPE) {

      if (isdigit(chr) ) {
         r = chr - '0';
         if (stacklift)
            PushStack();
         stack[0] = reg[r];
         if (stacklift)
            WriteStack(0, 3);
         else
            WriteStack(0, 0);
         stacklift = TRUE;
         return;
         }

      switch (chr) {

         case (ADD):
            AddRegX();
            return;

         case (SUBTRACT):
            SubtractRegX();
            return;

         case (MULTIPLY):
            MultiplyRegX();
            return;

         case (DIVIDE):
            DivideRegX();
            return;

         default:
            ;
         }  /* switch */
      } /* while */
}

/*
 *    **************************************************
 *    *                                                *
 *    *               Add Register to X                *
 *    *                                                *
 *    **************************************************
 */

static void AddRegX(void)

{

   int r;

   Message("Add to X: Reg (0-9) or Esc:");

   while ((chr = GetChar()) != ESCAPE) {

      if (isdigit(chr) ) {
         r = chr - '0';
         MoveRegWork(r, 0);
         MoveStackWork(0, 1);

         if (ExtendedAdd() ) {
            lastx = stack[0];
            MoveWorkStack(2, 0);
            WriteStack(0, 0);
            stacklift = TRUE;
            return;
            }
         return;
         }
      else
            ;

      }  /* while */

}




/*
 *    **************************************************
 *    *                                                *
 *    *           Subtract Register from X             *
 *    *                                                *
 *    **************************************************
 */
static void SubtractRegX(void)
{
   int r;

   Message("Subtract from X: Reg (0-9) or Esc:");

   while ((chr = GetChar()) != ESCAPE) {

      if (isdigit(chr) ) {
         r = chr - '0';
         MoveRegWork(r, 0);
         MoveStackWork(0, 1);

         if (ExtendedSubtract() ) {
            lastx = stack[0];
            MoveWorkStack(2, 0);
            WriteStack(0, 0);
            stacklift = TRUE;
            return;
            }
         return;
         }
      else
            ;
      }  /* while */
}

/*
 *    **************************************************
 *    *                                                *
 *    *            Multiply Register times X           *
 *    *                                                *
 *    **************************************************
 */

static void MultiplyRegX(void)

{

   int r;

   Message("Multiply times X: Reg (0-9) or Esc:");

   while ((chr = GetChar()) != ESCAPE) {

      if (isdigit(chr) ) {
         MessageEsc("Multiplying");

         r = chr - '0';
         MoveRegWork(r, 0);
         MoveStackWork(0, 1);

         if (ExtendedMultiply() ) {
            lastx = stack[0];
            MoveWorkStack(2, 0);
            WriteStack(0, 0);
            stacklift = TRUE;
            return;
            }
         return;
         }
      else
            ;

      } /* while */

}




/*
 *    **************************************************
 *    *                                                *
 *    *             Divide Register into X             *
 *    *                                                *
 *    **************************************************
 */

static void DivideRegX(void)

{

   int r;

   Message("Divide into X: Reg (0-9) or Esc:");

   while ((chr = GetChar()) != ESCAPE) {

      if (isdigit(chr) ) {
         MessageEsc("Dividing");

         r = chr - '0';
         MoveRegWork(r, 0);
         MoveStackWork(0, 1);

         if (ExtendedDivide() ) {
            lastx = stack[0];
            MoveWorkStack(2, 0);
            WriteStack(0, 0);
            stacklift = TRUE;
            return;
            }
         return;
         }
      else
            ;

      }  /* while */

}

/*
 *    **************************************************
 *    *                                                *
 *    *                Exchange X & Y                  *
 *    *                                                *
 *    **************************************************
 */

static void ExchangeXY(void)

{

   NORMTYPE *temp;

   if ((temp = GETNORMTEMP(1)) == NULL) {
      MemoryError();
      return;
      }

   *temp = stack[0];
   stack[0] = stack[1];
   stack[1] = *temp;

   WriteStack(0, 1);
   stacklift = TRUE;

   free(temp);

}

/*
 *    **************************************************
 *    *                                                *
 *    *             Exchange X & Register              *
 *    *                                                *
 *    **************************************************
 */
static void ExchangeXReg(void)
{
   int r;
   NORMTYPE *temp;

   Message("Exchange X with: Press Reg (0-9) or Esc:");

   while ((chr = GetChar()) != ESCAPE) {

      if (isdigit(chr) ) {

      if ((temp = GETNORMTEMP(1)) == NULL) {
         MemoryError();
         return;
         }

         r = chr - '0';
         *temp = stack[0];
         stack[0] = reg[r];
         reg[r] = *temp;
         WriteReg(r, r);
         WriteStack(0, 0);
         stacklift = TRUE;
         free(temp);
         return;
         }

      } /* while */
}

/*
 *    **************************************************
 *    *                                                *
 *    *                Roll Stack Down                 *
 *    *                                                *
 *    **************************************************
 */
static void RollDown(void)
{
   NORMTYPE *temp;

   if ((temp = GETNORMTEMP(1)) == NULL) {
      MemoryError();
      return;
      }

   *temp = stack[0];
   DropStack();
   stack[3] = *temp;

   WriteStack(0, 3);
   stacklift = TRUE;

   free(temp);
}

/*
 *    **************************************************
 *    *                                                *
 *    *                 Roll Stack Up                  *
 *    *                                                *
 *    **************************************************
 */

static void RollUp(void)

{

   NORMTYPE *temp;

   if ((temp = GETNORMTEMP(1)) == NULL) {
      MemoryError();
      return;
      }

   *temp = stack[3];
   PushStack();
   stack[0] = *temp;

   WriteStack(0, 3);
   stacklift = TRUE;

   free(temp);
}

/*
 *    **************************************************
 *    *                                                *
 *    *                  Push Stack                    *
 *    *                                                *
 *    **************************************************
 */
static void PushStack(void)
{
   int s;

   for (s = 3; s >= 1; s--)
      stack[s] = stack[s - 1];
}

/*
 *    **************************************************
 *    *                                                *
 *    *                  Drop Stack                    *
 *    *                                                *
 *    **************************************************
 */
static void DropStack(void)
{
   int s;

   for (s = 0; s <= 2; s++)
      stack[s] = stack[s + 1];
}

