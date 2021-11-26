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
int FloatToString(char *buf, float fVal);

/********************************************
*  Convert the integer given to a string   *
********************************************/

int IntToString(char *buf, int iVal);

/*****************************************************
*  Convert the unsigned integer given to a string   *
*****************************************************/

int UintToString(char *buf, unsigned int uVal);


#endif // ___DAT_HEADER__

// end of file 'dataconv.h'
