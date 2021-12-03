/**********************************************************************
 File     : dataconv.c
 Author   : D. Theran
**********************************************************************/


/**********************************************************************
*                            DEPENDENCIES                            *
**********************************************************************/
#include "dataconv.h"

/**********************************************************************
*                 DEFINITIONS OF EXPORTED FUNCTIONS                  *
**********************************************************************/


//********************************************************************
unsigned int strlen(const char *buf){
//********************************************************************
    
    unsigned int len;
    for (len = 0; buf[len] != 0; len++);
    return len;
}

//********************************************************************
void Reverse_String(char *buf){
//********************************************************************

    unsigned int len = strlen(buf);
    for (int i = 0; i < strlen(buf)/2 ; i++) 
    {
        char c = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = c;
    }
}

//********************************************************************
int FloatToString(char *buf, char *loc, float fVal){
//********************************************************************

    if ((fVal < -3.4e38) || (fVal > 3.4e38 - 1)) 
    {
        *buf = '\0';
        return DAT_ERROR;
    }
 
    if (fVal < 0) 
    {
        fVal = fVal*(-1);
        for (int i= 0; iVal > 0; i++) 
        {
            *buf++ = '0' + (fVal % 10);
            fVal /= 10;
        }
        *buf++ = '-';
    } else {
    
        for (int i= 0; fVal > 0; i++)
        {
            *buf++ = '0' + (fVal % 10);
            fVal /= 10;
        }
    }
    
    *buf = '\0';
    loc = buf;
    Reverse_String(buf);

    return DAT_OK;
}

//********************************************************************
int IntToString(char *buf, char *loc, int iVal){
//********************************************************************

    if ((iVal < -32768) || (iVal > 32767)) 
    {
        *buf = '\0';
        return DAT_ERROR;
    }

    // Detect if the int is negative or positive
    // and fill the buffer with the values in ASCII of the numbers
    if (iVal < 0) 
    {
        iVal = iVal*(-1);
        do
        {
            *buf++ = '0' + (iVal % 10);
            iVal /= 10;
        }
        while(iVal > 0);
        *buf++ = '-';
    } else {
    
        do
        {
            *buf++ = '0' + (iVal % 10);
            iVal /= 10;
        }
        while(iVal > 0);
    }

    *buf= '\0'; //We finish the string setting the \0
    loc = buf;
    Reverse_String(buf);

    return DAT_OK;
}

//********************************************************************
int UintToString(char *buf, char *loc, unsigned int uVal){
//********************************************************************

    // and fill the buffer with the values in ASCII of the numbers
    do
    {
        *buf++ = '0' + (uVal % 10);
        uVal /= 10;
    } 
    while(uVal > 0);

    *buf= '\0'; //We finish the string setting the \0
    loc = buf;
    Reverse_String(buf);

    return DAT_OK;
}

