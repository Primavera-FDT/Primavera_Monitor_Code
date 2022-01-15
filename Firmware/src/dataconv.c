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
static inline unsafe_concat(const char *buf, const char *str){
//********************************************************************
  while(*str != '\0' && *buf != '\0'){
      *buf++ = *str++;
  } 
}


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
 
    int val = (int) (fVal * 1000);    
    if (val < 0) 
    {
        int count = 0;
        val = val*(-1);
        for (int i= 0; val > 0; i++) 
        {
            *buf++ = '0' + (val % 10);
            val /= 10;
            count ++;
            if(count == 3)
                *buf++ = '.';
        }
        *buf++ = '-';
    } else {
        
        int count = 0;
        for (int i= 0; val > 0; i++)
        {
            *buf++ = '0' + (val % 10);
            val /= 10;
            count ++;
            if(count == 3)
                *buf++ = '.';
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

// ****************************************************************************
int AppendCRC(char *buf) {
// ****************************************************************************
  int len, i;
  unsigned int checksum;

  len = strlen(sendBuf);

  checksum = 0xffff;
  for (i = 0; i < len; i++) {
    checksum = CRC(checksum, (unsigned char) buf[i]);
  }

  buf[len + 1] = IntToHex(checksum, 3);
  buf[len + 2] = IntToHex(checksum, 2);
  buf[len + 3] = IntToHex(checksum, 1);
  buf[len + 4] = IntToHex(checksum, 0);

  return DAT_OK;
}

#ifdef CRC_CCITT
#define CRCPOLY 0x8408                      // do crc ccitt
#else
#define CRCPOLY 0xa001                      // do crc-16
#endif // CRC_CCITT

// ****************************************************************************
unsigned int CRC(unsigned int crcoldnew, unsigned char cval) {
// ****************************************************************************
  int i, j;

  for (i = 0; i != 8; cval >>= 1, i++) {
    j = (cval ^ crcoldnew) & 1;
    crcoldnew >>= 1;

    if (j)
      crcoldnew ^= CRCPOLY;
  }

  return (crcoldnew);
}

// ****************************************************************************
char IntToHex(unsigned int inval, char digit) {
// ****************************************************************************
  char hexVal;

  hexVal = (char) ((inval >> ((digit & 0x03) << 2)) & 0x000f) + '0';
  if (hexVal > '9')
    hexVal += 7;

  return hexVal;
}


