/**********************************************************************
 File      : dataconv.h
 Author    : D. Theran
**********************************************************************/


#ifndef __DAT_HEADER__
#define __DAT_HEADER__


/**********************************************************************
*                            DEPENDENCIES                            *
**********************************************************************/

#include <stdio.h>

/**********************************************************************
*                   DEFINITIONS, TYPES AND MACROS                    *
**********************************************************************/

#define DAT_ERROR 0
#define DAT_OK 1


/**********************************************************************
*                  PROTOTYPES OF EXPORTED FUNCTIONS                  *
**********************************************************************/

static inline unsafe_concat(const char *buf, const char *str) {
    while(*str != '\0' && *buf != '\0') {
        *buf++ = *str++;
    }
}

/************************************************
*  Calculate the length of a given char array  *
************************************************/
int strlen(const char *buf);

/**********************************************************
*  Reverse the order of the words of a given char array  *
**********************************************************/
void Reverse_string(char *buf);

/************************************************
*  Convert the float given number to a string  *
************************************************/
int FloatToString(char *buf, char *loc, float fVal);

/********************************************
*  Convert the integer given to a string   *
********************************************/

int IntToString(char *buf, char *loc, int iVal);

/*****************************************************
*  Convert the unsigned integer given to a string   *
*****************************************************/

int UintToString(char *buf, char *loc, unsigned int uVal);


#endif // ___DAT_HEADER__

// end of file 'dataconv.h'
