/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: NIVLang Team
* Professors: Paulo Sousa
************************************************************
*/

/*
************************************************************
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* Token codes for NIVLang */
#define NUM_TOKENS 25

enum TOKENS {
    ERR_T,      /*  0: Error token */
    KWRD_T,     /*  1: Keyword token (fun, int, float, string, while, if, else, return) */
    VID_T,      /*  2: Variable identifier token */
    INL_T,      /*  3: Integer literal token */
    FPL_T,      /*  4: Floating-point literal token */
    STR_T,      /*  5: String literal token */
    LPR_T,      /*  6: Left parenthesis token '(' */
    RPR_T,      /*  7: Right parenthesis token ')' */
    LBR_T,      /*  8: Left brace token '{' */
    RBR_T,      /*  9: Right brace token '}' */
    EOS_T,      /* 10: End of statement (semicolon) ';' */
    ASS_T,      /* 11: Assignment operator (=) */
    ADD_T,      /* 12: Addition operator (+) */
    SUB_T,      /* 13: Subtraction operator (-) */
    MUL_T,      /* 14: Multiplication operator (*) */
    DIV_T,      /* 15: Division operator (/) */
    LES_T,      /* 16: Less than operator (<) */
    GRT_T,      /* 17: Greater than operator (>) */
    EQU_T,      /* 18: Equal operator (==) */
    NEQ_T,      /* 19: Not equal operator (!=) */
    LEQ_T,      /* 20: Less than or equal operator (<=) */
    GEQ_T,      /* 21: Greater than or equal operator (>=) */
    COM_T,      /* 22: Comma token (,) */
    CMT_T,      /* 23: Comment token */
    SEOF_T      /* 24: Source end-of-file token */
};

/* Token string table for NIVLang */
static niv_string tokenStrTable[NUM_TOKENS] = {
    "ERR_T",
    "KWRD_T",
    "VID_T",
    "INL_T",
    "FPL_T",
    "STR_T",
    "LPR_T",
    "RPR_T",
    "LBR_T",
    "RBR_T",
    "EOS_T",
    "ASS_T",
    "ADD_T",
    "SUB_T",
    "MUL_T",
    "DIV_T",
    "LES_T",
    "GRT_T",
    "EQU_T",
    "NEQ_T",
    "LEQ_T",
    "GEQ_T",
    "COM_T",
    "CMT_T",
    "SEOF_T"
};

/* NIVLang Operator Enums */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT, OP_GE, OP_LE } RelOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* NIVLang Keyword Indices */
#define KWT_SIZE 8
enum KEYWORDS {
    KW_FUN,    /* 0: 'fun' */
    KW_INT,    /* 1: 'int' */
    KW_FLOAT,  /* 2: 'float' */
    KW_STRING, /* 3: 'string' */
    KW_WHILE,  /* 4: 'while' */
    KW_IF,     /* 5: 'if' */
    KW_ELSE,   /* 6: 'else' */
    KW_RETURN  /* 7: 'return' */
};

/* Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
    niv_int codeType;              /* integer attributes accessor */
    AriOperator arithmeticOperator; /* arithmetic operator attribute code */
    RelOperator relationalOperator; /* relational operator attribute code */
    EofOperator seofType;           /* source-end-of-file attribute code */
    niv_int intValue;              /* integer literal attribute (value) */
    niv_int keywordIndex;          /* keyword index in the keyword table */
    niv_int contentString;         /* string literal offset */
    niv_float floatValue;          /* floating-point literal attribute (value) */
    niv_char idLexeme[VID_LEN + 1]; /* variable identifier token attribute */
    niv_char errLexeme[ERR_LEN + 1]; /* error token attribute */
} TokenAttribute;

/* Should be used if no symbol table is implemented */
typedef struct idAttibutes {
    niv_byte flags;           /* Flags information */
    union {
        niv_int intValue;             /* Integer value */
        niv_float floatValue;         /* Float value */
        niv_string stringContent;     /* String value */
    } values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
    niv_int code;             /* token code */
    TokenAttribute attribute;     /* token attribute */
    IdAttibutes idAttribute;      /* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
    niv_int scanHistogram[NUM_TOKENS];    /* Statistics of tokens */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* Define lexeme classes */
/* Character definitions */
#define EOS_CHR '\0'    /* End of string character */
#define EOF_CHR 0xFF    /* End of file character */
#define SPC_CHR ' '     /* Space character */
#define TAB_CHR '\t'    /* Tab character */
#define NWL_CHR '\n'    /* Newline character */
#define SCL_CHR ';'     /* Semicolon character */
#define LPR_CHR '('     /* Left parenthesis character */
#define RPR_CHR ')'     /* Right parenthesis character */
#define LBR_CHR '{'     /* Left brace character */
#define RBR_CHR '}'     /* Right brace character */
#define COM_CHR ','     /* Comma character */
#define QOT_CHR '"'     /* Double quote character */
#define ASS_CHR '='     /* Assignment character */
#define ADD_CHR '+'     /* Addition character */
#define SUB_CHR '-'     /* Subtraction character */
#define MUL_CHR '*'     /* Multiplication character */
#define DIV_CHR '/'     /* Division character */
#define LES_CHR '<'     /* Less than character */
#define GRT_CHR '>'     /* Greater than character */
#define UND_CHR '_'     /* Underscore character */
#define DOT_CHR '.'     /* Dot character */

/* Error states and illegal state */
#define ESNR    8       /* Error state with no retract */
#define ESWR    9       /* Error state with retract */
#define FS      10      /* Illegal state */

/* State transition table definition */
#define NUM_STATES      10
#define CHAR_CLASSES    10

/* Character classes - according to assignment specifications */
typedef enum {
    LETTER,     /* 0: [A-Za-z] - letters */
    DIGIT,      /* 1: [0-9] - digits */
    UNDERSCORE, /* 2: _ - underscore */
    QUOTE,      /* 3: " - double quote */
    SLASH,      /* 4: / - for comments */
    SEOF,       /* 5: EOF - end of file */
    COMMENT,    /* 6: # - comment indicator */
    DOT,        /* 7: . - for floating point */
    OPERATOR,   /* 8: +, -, *, =, <, > */
    OTHER       /* 9: Any other character */
} CharClass;

/* State transition table for NIVLang - following assignment specifications */
static niv_int transitionTable[NUM_STATES][CHAR_CLASSES] = {
    /*    [A-z], [0-9],    _,    ",    /,  EOF,    #,    .,   OP, OTHER
            L(0),  D(1), U(2), Q(3), S(4), E(5), C(6), P(7), O(8),  X(9) */
    {      1,     2,     1,     4,     6,  ESWR,    6,     2,     5,  ESNR}, /* S0: NOFS */
    {      1,     1,     1,  ESWR,  ESWR,  ESWR,  ESWR,  ESWR,  ESWR,  ESWR}, /* S1: NOFS (Ids) */
    {   ESWR,     2,  ESWR,  ESWR,  ESWR,  ESWR,  ESWR,     3,  ESWR,  ESWR}, /* S2: NOFS (Int) */
    {   ESWR,     3,  ESWR,  ESWR,  ESWR,  ESWR,  ESWR,  ESWR,  ESWR,  ESWR}, /* S3: NOFS (Float) */
    {      4,     4,     4,     5,     4,  ESWR,     4,     4,     4,     4}, /* S4: NOFS (String) */
    {     FS,    FS,    FS,    FS,    FS,    FS,    FS,    FS,    FS,    FS}, /* S5: FSNR (SL) */
    {      6,     6,     6,     6,     6,  ESWR,     7,     6,     6,     6}, /* S6: NOFS (Comment) */
    {     FS,    FS,    FS,    FS,    FS,    FS,    FS,    FS,    FS,    FS}, /* S7: FSNR (COM) */
    {     FS,    FS,    FS,    FS,    FS,    FS,    FS,    FS,    FS,    FS}, /* S8: FSNR (ES) */
    {     FS,    FS,    FS,    FS,    FS,    FS,    FS,    FS,    FS,    FS}  /* S9: FSWR (ER) */
};

/* Define accepting states types */
#define NOFS    0       /* not accepting state */
#define FSNR    1       /* accepting state with no retract */
#define FSWR    2       /* accepting state with retract */

/* NIVLang state types */
static niv_int stateType[NUM_STATES] = {
    NOFS, /* 00 - Initial state */
    FSNR, /* 01 - Identifier */
    FSNR, /* 02 - Integer literal */
    FSNR, /* 03 - Float literal */
    NOFS, /* 04 - String literal building */
    FSNR, /* 05 - String literal completed */
    NOFS, /* 06 - Comment building */
    FSNR, /* 07 - Comment completed */
    FSNR, /* 08 - Error (no retract) */
    FSWR  /* 09 - Error (with retract) */
};

/* Static (local) function prototypes */
niv_int         startScanner(BufferPointer psc_buf);
static niv_int  nextClass(niv_char c);               /* character class function */
static niv_int  nextState(niv_int, niv_char);        /* state machine function */
niv_void        printScannerData(ScannerData scData);
Token           tokenizer(niv_void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(niv_string lexeme);

/* Declare accepting states functions */
Token funcID(niv_string lexeme);    /* Identifier */
Token funcIL(niv_string lexeme);    /* Integer literal */
Token funcFPL(niv_string lexeme);   /* Float-point literal */
Token funcSL(niv_string lexeme);    /* String literal */
Token funcKW(niv_string lexeme);    /* Keywords */
Token funcCMT(niv_string lexeme);   /* Comments */
Token funcOPR(niv_string lexeme);   /* Operators */
Token funcErr(niv_string lexeme);   /* Error */

/* Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
    NULL,       /* -    [00] */
    funcID,     /* ID   [01] */
    funcIL,     /* IL   [02] */
    funcFPL,    /* FPL  [03] */
    NULL,       /* -    [04] */
    funcSL,     /* SL   [05] */
    NULL,       /* -    [06] */
    funcCMT,    /* COM  [07] */
    funcErr,    /* ERR1 [08] */
    funcErr     /* ERR2 [09] */
};

/*
-------------------------------------------------
NIVLang keywords
-------------------------------------------------
*/

/* List of keywords */
static niv_string keywordTable[KWT_SIZE] = {
    "fun",      /* KW00 */
    "int",      /* KW01 */
    "float",    /* KW02 */
    "string",   /* KW03 */
    "while",    /* KW04 */
    "if",       /* KW05 */
    "else",     /* KW06 */
    "return"    /* KW07 */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: indentation data)
 */

#define INDENT TAB_CHR  /* Tabulation */

 /* Language attributes */
typedef struct languageAttributes {
    niv_char indentationCharType;
    niv_int indentationCurrentPos;
    /* Additional attributes for NIVLang scanner */
    niv_int keywordCount;
    niv_int stringCount;
    niv_int commentCount;
} LanguageAttributes;

/* Number of errors */
niv_int numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif