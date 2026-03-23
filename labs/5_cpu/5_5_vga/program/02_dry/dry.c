/*
 * =============================================================================
 * DHRYSTONE BENCHMARK - Cross-Platform BSD Version
 * =============================================================================
 *
 * Filename:     dry.c
 * Version:      C, Version 2.2 (Cross-Platform BSD Edition)
 * Date:         June 30, 2025
 * Author:       Dave Plummer
 *
 * Description:  A portable implementation of the Dhrystone 2.2 benchmark,
 *               modified for compatibility across 2.11BSD (PDP-11), NetBSD,
 *               and macOS systems while maintaining K&R C compatibility.
 *
 * Original Credits:
 *   - Algorithm:    Reinhold P. Weicker (Siemens Nixdorf)
 *   - C Version:    Rick Richardson, PC Research Inc.
 *   - Dhrystone:    "Dhrystone" Benchmark Program
 *   - Based on:     Various GitHub implementations of classic Dhrystone
 *
 * Compatibility:
 *   - 2.11BSD (PDP-11) with K&R C compiler
 *   - NetBSD 10.x with modern GCC
 *   - macOS (Intel and Apple Silicon)
 *   - Other Unix-like systems
 *
 * Key Modifications for Cross-Platform Support:
 *   - Fixed 16-bit integer overflow on PDP-11 systems
 *   - Portable timing using times() system call
 *   - K&R C compatible variable declarations
 *   - Conditional compilation for different BSD variants
 *   - Simplified build process (single compilation unit)
 *
 * Build:
 *   2.11BSD:     cc -O -o dry dry.c
 *   Modern:      gcc -O2 -o dry dry.c
 *   Make:        make
 *
 * Usage:
 *   ./dry [iterations]
 *
 * License:      Public Domain (following original Dhrystone tradition)
 *
 * =============================================================================
 */

#define Version "C, Version 2.2"
#define  _CRT_SECURE_NO_WARNINGS

 /* Standard headers for 2.11BSD, NetBSD 10, and macOS */

#include "yrv/ee_printf.h"
#include "nanoprintf.h"
#include "memory_mapped_registers.h"
#include <stdint.h>



/* Define HZ based on system */
#define HZ 100    /* Default for modern BSD systems */


/* Use times(2) for timing */
#define TIMES
#define CLOCK_TYPE "pll"

/* Type definitions */
#ifdef NOENUM

#define Ident_1 0
#define Ident_2 1
#define Ident_3 2
#define Ident_4 3
#define Ident_5 4
typedef int Enumeration;
#else
typedef enum { Ident_1, Ident_2, Ident_3, Ident_4, Ident_5 } Enumeration;
#endif

#define Null 0
#define true 1
#define false 0

typedef int One_Thirty;
typedef int One_Fifty;
typedef char Capital_Letter;
typedef int Boolean;
typedef char Str_30[31];

typedef struct record {
    struct record* Ptr_Comp;
    Enumeration Discr;
    union {
        struct {
            Enumeration Enum_Comp;
            int Int_Comp;
            char Str_Comp[31];
        } var_1;
        struct {
            Enumeration E_Comp_2;
            char Str_2_Comp[31];
        } var_2;
        struct {
            char Ch_1_Comp;
            char Ch_2_Comp;
        } var_3;
    } variant;
} Rec_Type, * Rec_Pointer;

#ifndef REG
#define REG
#else
#define REG register
#endif

typedef long Align;   /* for alignment to long boundary */

union header {        /* block header: */
    struct {
        union header* ptr;  /* next block if on free list */
        unsigned size;      /* size of this block */
    } s;
    Align x;          /* force alignment of blocks */
};

typedef union header Header;

static Header base;       /* empty list to get started */
static Header* freep = NULL;     /* start of free list */



#define NALLOC   8    /* minimum #units to request */
static char heap[sizeof(Header) * 16];

/* Malloc proto*/
//static Header* morecore(unsigned nu);
void* kr_malloc(unsigned nbytes);
void kr_free(void* ap);

/* Function prototypes */
void Proc_1(REG Rec_Pointer Ptr_Val_Par);
void Proc_2(One_Fifty* Int_Par_Ref);
void Proc_3(Rec_Pointer* Ptr_Ref_Par);
void Proc_4(void);
void Proc_5(void);
void Proc_6(Enumeration Enum_Val_Par, Enumeration* Enum_Ref_Par);
void Proc_7(One_Fifty Int_1_Par_Val, One_Fifty Int_2_Par_Val, One_Fifty* Int_Par_Ref);
void Proc_8(int Arr_1_Par_Ref[50], int Arr_2_Par_Ref[50][50], int Int_1_Par_Val, int Int_2_Par_Val);
Enumeration Func_1(Capital_Letter Ch_1_Par_Val, Capital_Letter Ch_2_Par_Val);
Boolean Func_2(Str_30 Str_1_Par_Ref, Str_30 Str_2_Par_Ref);
Boolean Func_3(Enumeration Enum_Par_Val);

#ifdef NOSTRUCTASSIGN
#define structassign(d, s) memcpy(&(d), &(s), sizeof(d))
#else
#define structassign(d, s) d = s
#endif

/* Global Variables */
Rec_Pointer Ptr_Glob, Next_Ptr_Glob;
int Int_Glob;
Boolean Bool_Glob;
char Ch_1_Glob, Ch_2_Glob;
int Arr_1_Glob[50];
int Arr_2_Glob[50][50];

#ifndef REG
Boolean Reg = false;
#else
Boolean Reg = true;
#endif

Boolean Done;
long Begin_Time, End_Time, User_Time;
float Microseconds, Dhrystones_Per_Second;

#define Mic_secs_Per_Second 1000000.0
#define NUMBER_OF_RUNS 5000  /* More reasonable for PDP-11/83 */
#define Too_Small_Time (2*HZ)
int strcmp(const char* s1, const char* s2) {
    // Iterate through the strings character by character
    while (*s1 != '\0' && *s1 == *s2) {
        s1++; // Move to the next character in s1
        s2++; // Move to the next character in s2
    }

    // When the loop terminates, either a mismatch was found, or one (or both) strings ended.
    // The difference between the current characters determines the return value.
    // Casting to unsigned char is important to handle potential negative char values correctly.
    return (int)(*(unsigned char*)s1) - (int)(*(unsigned char*)s2);
}

char* strcpy(char* dest, const char* src) {
    char* ret = dest; // Store the original destination pointer to return it later

    // Loop through the source string, copying each character to the destination
    // until the null terminator ('\0') is encountered.
    // The null terminator itself is also copied.
    while ((*dest++ = *src++) != '\0');

    return ret; // Return the original destination pointer
}
int dry()
{
    One_Fifty Int_1_Loc;
    REG One_Fifty Int_2_Loc;
    One_Fifty Int_3_Loc;
    REG char Ch_Index;
    Enumeration Enum_Loc;
    Str_30 Str_1_Loc;
    Str_30 Str_2_Loc;
    REG long Run_Index;  /* Use long to match Number_Of_Runs */
    REG long Number_Of_Runs;  /* Use long to avoid 16-bit int overflow on PDP-11 */

    Number_Of_Runs = NUMBER_OF_RUNS;

    /* Initializations */
    Next_Ptr_Glob = (Rec_Pointer)kr_malloc(sizeof(Rec_Type));
    Ptr_Glob = (Rec_Pointer)kr_malloc(sizeof(Rec_Type));

    Ptr_Glob->Ptr_Comp = Next_Ptr_Glob;
    Ptr_Glob->Discr = Ident_1;
    Ptr_Glob->variant.var_1.Enum_Comp = Ident_3;
    Ptr_Glob->variant.var_1.Int_Comp = 40;
    strcpy(Ptr_Glob->variant.var_1.Str_Comp, "DHRYSTONE PROGRAM, SOME STRING");
    strcpy(Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

    Arr_2_Glob[8][7] = 10;


    char buf[64] = {0};
 
    ee_printf("\n");
    ee_printf("Dhrystone Benchmark, Version %s\n", Version);
    
    ee_printf("Program compiled %s 'register' attribute\n", Reg ? "with" : "without");
    npf_snprintf(buf, sizeof(buf), "Using %s, HZ=%ld\n", CLOCK_TYPE, (long)HZ);
    ee_printf("%s\n", buf);
    ee_printf("\n");

    Done = false;
    while (!Done) {

        npf_snprintf(buf, sizeof(buf),"Trying %ld runs through Dhrystone:\n", Number_Of_Runs);
        ee_printf("%s\n", buf);

        /* Start timer */
        Begin_Time = port5;

        for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index) {
            Proc_5();
            Proc_4();
            Int_1_Loc = 2;
            Int_2_Loc = 3;
            strcpy(Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
            Enum_Loc = Ident_2;
            Bool_Glob = !Func_2(Str_1_Loc, Str_2_Loc);
            while (Int_1_Loc < Int_2_Loc) {
                Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
                Proc_7(Int_1_Loc, Int_2_Loc, &Int_3_Loc);
                Int_1_Loc += 1;
            }
            Proc_8(Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
            Proc_1(Ptr_Glob);
            for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index) {
                if (Enum_Loc == Func_1(Ch_Index, 'C')) {
                    Proc_6(Ident_1, &Enum_Loc);
                    strcpy(Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
                    Int_2_Loc = Run_Index;
                    Int_Glob = Run_Index;
                }
            }
            Int_2_Loc = Int_2_Loc * Int_1_Loc;
            Int_1_Loc = Int_2_Loc / Int_3_Loc;
            Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
            Proc_2(&Int_1_Loc);
        }

        /* Stop timer */
        End_Time = port5;

        User_Time = End_Time - Begin_Time;

        if (User_Time < Too_Small_Time) {
            ee_printf("Measured time too small to obtain meaningful results\n");
            Number_Of_Runs *= 10;
            ee_printf("\n");
        }
        else {
            Done = true;
        }
    }
    /*
    printf("Final values of the variables used in the benchmark:\n");
    printf("\n");
    printf("Int_Glob:            %d\n", Int_Glob);
    printf("        should be:   %d\n", 5);
    printf("Bool_Glob:           %d\n", Bool_Glob);
    printf("        should be:   %d\n", 1);
    printf("Ch_1_Glob:           %c\n", Ch_1_Glob);
    printf("        should be:   %c\n", 'A');
    printf("Ch_2_Glob:           %c\n", Ch_2_Glob);
    printf("        should be:   %c\n", 'B');
    printf("Arr_1_Glob[8]:       %d\n", Arr_1_Glob[8]);
    printf("        should be:   %d\n", 7);
    printf("Arr_2_Glob[8][7]:    %d\n", Arr_2_Glob[8][7]);
    printf("        should be:   Number_Of_Runs + 10\n");
    printf("Ptr_Glob->\n");
    printf("  Ptr_Comp:          %ld\n", (long)Ptr_Glob->Ptr_Comp);
    printf("        should be:   (implementation-dependent)\n");
    printf("  Discr:             %d\n", Ptr_Glob->Discr);
    printf("        should be:   %d\n", 0);
    printf("  Enum_Comp:         %d\n", Ptr_Glob->variant.var_1.Enum_Comp);
    printf("        should be:   %d\n", 2);
    printf("  Int_Comp:          %d\n", Ptr_Glob->variant.var_1.Int_Comp);
    printf("        should be:   %d\n", 17);
    printf("  Str_Comp:          %s\n", Ptr_Glob->variant.var_1.Str_Comp);
    printf("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
    printf("Next_Ptr_Glob->\n");
    printf("  Ptr_Comp:          %ld\n", (long)Next_Ptr_Glob->Ptr_Comp);
    printf("        should be:   (implementation-dependent), same as above\n");
    printf("  Discr:             %d\n", Next_Ptr_Glob->Discr);
    printf("        should be:   %d\n", 0);
    printf("  Enum_Comp:         %d\n", Next_Ptr_Glob->variant.var_1.Enum_Comp);
    printf("        should be:   %d\n", 1);
    printf("  Int_Comp:          %d\n", Next_Ptr_Glob->variant.var_1.Int_Comp);
    printf("        should be:   %d\n", 18);
    printf("  Str_Comp:          %s\n", Next_Ptr_Glob->variant.var_1.Str_Comp);
    printf("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
    printf("Int_1_Loc:           %d\n", Int_1_Loc);
    printf("        should be:   %d\n", 5);
    printf("Int_2_Loc:           %d\n", Int_2_Loc);
    printf("        should be:   %d\n", 13);
    printf("Int_3_Loc:           %d\n", Int_3_Loc);
    printf("        should be:   %d\n", 7);
    printf("Enum_Loc:            %d\n", Enum_Loc);
    printf("        should be:   %d\n", 1);
    printf("Str_1_Loc:           %s\n", Str_1_Loc);
    printf("        should be:   DHRYSTONE PROGRAM, 1'ST STRING\n");
    printf("Str_2_Loc:           %s\n", Str_2_Loc);
    printf("        should be:   DHRYSTONE PROGRAM, 2'ND STRING\n");
    printf( "\n");
    */
    Microseconds =(float) ((float)User_Time * (float)Mic_secs_Per_Second / ((float)HZ * (float)Number_Of_Runs));
    Dhrystones_Per_Second = (float)((float)HZ * (float)Number_Of_Runs) / (float)User_Time;

    npf_snprintf(buf, sizeof(buf),"Microseconds for one run through Dhrystone: %10.1f \n",Microseconds);
    ee_printf("%s\n", buf);
    npf_snprintf(buf, sizeof(buf),"Dhrystones per Second:                      %10.0f \n",  Dhrystones_Per_Second);
    ee_printf("%s\n", buf);
    ee_printf("\n");

    return 0;
}

void Proc_1(REG Rec_Pointer Ptr_Val_Par)
{
    REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;
    structassign(*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob);
    Ptr_Val_Par->variant.var_1.Int_Comp = 5;
    Next_Record->variant.var_1.Int_Comp = Ptr_Val_Par->variant.var_1.Int_Comp;
    Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
    Proc_3(&Next_Record->Ptr_Comp);
    if (Next_Record->Discr == Ident_1) {
        Next_Record->variant.var_1.Int_Comp = 6;
        Proc_6(Ptr_Val_Par->variant.var_1.Enum_Comp, &Next_Record->variant.var_1.Enum_Comp);
        Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
        Proc_7(Next_Record->variant.var_1.Int_Comp, 10, &Next_Record->variant.var_1.Int_Comp);
    }
    else {
        structassign(*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
    }
}

void Proc_2(One_Fifty* Int_Par_Ref)
{
    One_Fifty Int_Loc;
    Enumeration Enum_Loc;
    Int_Loc = *Int_Par_Ref + 10;
    do {
        if (Ch_1_Glob == 'A') {
            Int_Loc -= 1;
            *Int_Par_Ref = Int_Loc - Int_Glob;
            Enum_Loc = Ident_1;
        }
    } while (Enum_Loc != Ident_1);
}

void Proc_3(Rec_Pointer* Ptr_Ref_Par)
{
    if (Ptr_Glob != Null)
        *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
    Proc_7(10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
}

void Proc_4(void)
{
    Boolean Bool_Loc;
    Bool_Loc = Ch_1_Glob == 'A';
    Bool_Glob = Bool_Loc | Bool_Glob;
    Ch_2_Glob = 'B';
}

void Proc_5(void)
{
    Ch_1_Glob = 'A';
    Bool_Glob = false;
}

#ifdef NOSTRUCTASSIGN
void memcpy(char* d, char* s, int l)
{
    while (l--) *d++ = *s++;
}
#endif

void Proc_6(Enumeration Enum_Val_Par, Enumeration* Enum_Ref_Par)
{
    *Enum_Ref_Par = Enum_Val_Par;
    if (!Func_3(Enum_Val_Par))
        *Enum_Ref_Par = Ident_4;
    switch (Enum_Val_Par) {
    case Ident_1: *Enum_Ref_Par = Ident_1; break;
    case Ident_2: *Enum_Ref_Par = (Int_Glob > 100) ? Ident_1 : Ident_4; break;
    case Ident_3: *Enum_Ref_Par = Ident_2; break;
    case Ident_4: break;
    case Ident_5: *Enum_Ref_Par = Ident_3; break;
    }
}

void Proc_7(One_Fifty Int_1_Par_Val, One_Fifty Int_2_Par_Val, One_Fifty* Int_Par_Ref)
{
    One_Fifty Int_Loc;
    Int_Loc = Int_1_Par_Val + 2;
    *Int_Par_Ref = Int_2_Par_Val + Int_Loc;
}

void Proc_8(int Arr_1_Par_Ref[50], int Arr_2_Par_Ref[50][50], int Int_1_Par_Val, int Int_2_Par_Val)
{
    REG One_Fifty Int_Index;
    REG One_Fifty Int_Loc;
    Int_Loc = Int_1_Par_Val + 5;
    Arr_1_Par_Ref[Int_Loc] = Int_2_Par_Val;
    Arr_1_Par_Ref[Int_Loc + 1] = Arr_1_Par_Ref[Int_Loc];
    Arr_1_Par_Ref[Int_Loc + 30] = Int_Loc;
    for (Int_Index = Int_Loc; Int_Index <= Int_Loc + 1; ++Int_Index)
        Arr_2_Par_Ref[Int_Loc][Int_Index] = Int_Loc;
    Arr_2_Par_Ref[Int_Loc][Int_Loc - 1] += 1;
    Arr_2_Par_Ref[Int_Loc + 20][Int_Loc] = Arr_1_Par_Ref[Int_Loc];
    Int_Glob = 5;
}

Enumeration Func_1(Capital_Letter Ch_1_Par_Val, Capital_Letter Ch_2_Par_Val)
{
    Capital_Letter Ch_1_Loc;
    Capital_Letter Ch_2_Loc;
    Ch_1_Loc = Ch_1_Par_Val;
    Ch_2_Loc = Ch_1_Loc;
    if (Ch_2_Loc != Ch_2_Par_Val)
        return Ident_1;
    else {
        Ch_1_Glob = Ch_1_Loc;
        return Ident_2;
    }
}

Boolean Func_2(Str_30 Str_1_Par_Ref, Str_30 Str_2_Par_Ref)
{
    REG One_Thirty Int_Loc;
    Capital_Letter Ch_Loc;
    Int_Loc = 2;
    while (Int_Loc <= 2) {
        if (Func_1(Str_1_Par_Ref[Int_Loc], Str_2_Par_Ref[Int_Loc + 1]) == Ident_1) {
            Ch_Loc = 'A';
            Int_Loc += 1;
        }
    }
    if (Ch_Loc >= 'W' && Ch_Loc < 'Z')
        Int_Loc = 7;
    if (Ch_Loc == 'R')
        return true;
    else {
        if (strcmp(Str_1_Par_Ref, Str_2_Par_Ref) > 0) {
            Int_Loc += 7;
            Int_Glob = Int_Loc;
            return true;
        }
        else {
            return false;
        }
    }
}

Boolean Func_3(Enumeration Enum_Par_Val)
{
    Enumeration Enum_Loc;
    Enum_Loc = Enum_Par_Val;
    if (Enum_Loc == Ident_3)
        return true;
    else
        return false;
}



/* morecore:   ask system for more memory */
static Header* morecore(unsigned nu)
{
    char* cp, * sbrk(int);
    Header* up;
    //printf("Request system memory %d \n",nu );
    if (nu < NALLOC)
        nu = NALLOC;
    //printf("Requested system memory %d \n", nu);

    cp = heap;
    if (cp == (char*)-1)  /* no space at all */
        return NULL;
    up = (Header*)cp;
    up->s.size = nu;
    kr_free((void*)(up + 1));
    return freep;
}

/* malloc:  general-purpose storage allocator */
void* kr_malloc(unsigned nbytes)
{
    Header* p, * prevp;
    //Header *morecore(unsigned);
    unsigned nunits;
    //printf("Malloc request memory %d nbytes \n", nbytes);
    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
    //printf("Malloc request memory %d nunits\n", nunits);

    if ((prevp = freep) == NULL) {  /* no free list yet */
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
        if (p->s.size >= nunits) {     /* big enough */
            if (p->s.size == nunits)      /* exactly */
                prevp->s.ptr = p->s.ptr;
            else {              /* allocate tail end */
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void*)(p + 1);
        }
        if (p == freep)  /* wrapped around free list */
            if ((p = morecore(nunits)) == NULL)
                return NULL;   /* none left */
    }
}

/* free:   put block ap in free list */
void kr_free(void* ap)
{
    Header* bp, * p;

    bp = (Header*)ap - 1;     /* point to block header */
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
            break;  /* freed block at start of end of arena */

    if (bp + bp->s.size == p->s.ptr) { /* join to upper nbr */
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    }
    else
        bp->s.ptr = p->s.ptr;
    if (p + p->s.size == bp) {         /* join to lower nbr */
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    }
    else
        p->s.ptr = bp;
    freep = p;
}

