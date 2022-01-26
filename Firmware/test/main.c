// MSB ordering
#include "msp.h"
#include <stdint.h>
#include <stdio.h>

#define F_CPU (24000000)
#define DELAY (F_CPU / 2)

void Print(char *data) {
    unsigned int inc = 0;
    while (data[inc] != '\0') {
        char curr_char = data[inc];
        EUSCI_A0->TXBUF = (uint8_t) curr_char;
        while(!(EUSCI_A0->IFG & ( 1 << 1 ) ) );
        inc++;
    }
}

void uart_init() {
    CS->KEY = CS_KEY_VAL;
    CS->CTL1 = (CS->CTL1 & ~(CS_CTL1_SELS_MASK | CS_CTL1_DIVS_MASK));
    CS->CTL1 |= CS_CTL1_SELS__DCOCLK;
    CS->KEY = 0;

    P1->DIR &= ~((1 << 2) | (1 << 3));
    P1->SEL0 |= ((1 << 2) | (1 << 3));
    P1->SEL1 &= ~((1 << 2) | (1 << 3));

    EUSCI_A0->CTLW0 = 0;

    EUSCI_A0->CTLW0 |= ( 1 << 0);
    EUSCI_A0->CTLW0 &= ~((1 << 9) | (1 << 10));
    EUSCI_A0->CTLW0 &= ~(1 << 8);
    EUSCI_A0->CTLW0 &= ~(1 << 3);
    EUSCI_A0->CTLW0 &= ~(1 << 15);
    EUSCI_A0->CTLW0 |= ( 2 << 6 );

    EUSCI_A0->MCTLW &= ~(0xF1);
    EUSCI_A0->MCTLW |= (0x49 << 8) | (1 << 0);

    EUSCI_A0->BRW = 13;

    EUSCI_A0->CTLW0 &= ~( 1 << 0 );
}

void i2c_init() {
    EUSCI_B1->CTLW0 |= (3 << 9);
    EUSCI_B1->CTLW0 |= (1 << 11);
    EUSCI_B1->CTLW0 |= (1 << 6 );
    EUSCI_B1->BRW = 240;
    return;
}

int main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop watchdog

    TIMER32_1->CONTROL &= ~0x6f;
    TIMER32_1->CONTROL |= ( 1 << 6 );
    TIMER32_1->CONTROL |= ( 1 << 1 );
    TIMER32_1->CONTROL |= 1;
    TIMER32_1->CONTROL |= ( 1 << 7 );

    uart_init();
    i2c_init();

    char date[20] = {0};
    P1->DIR |= 1;

    //set time

    while(1) {

        TIMER32_1->LOAD = DELAY;
        while(!(TIMER32_1->RIS));
        TIMER32_1->INTCLR = 1;

        P1->OUT ^= 1;

        // Read current time
        uint8_t day = 25;
        uint8_t month = 1;
        uint16_t year = 2022;
        uint8_t hour = 15;
        uint8_t min = 22;
        uint8_t sec = 16;

        unsigned int counter = 1000;
        for(int i=0; i<4; i++) {
            date[i] = (year / counter) + '0';
            year = year % counter;
            counter /= 10;
        }

        date[4] = '-';

        date[5] = (month / 10) + '0';
        date[6] = (month % 10) + '0';

        date[7] = '-';

        date[8] = (day / 10) + '0';
        date[9] = (day % 10) + '0';

        date[10] = ' ';

        date[11] = (hour / 10) + '0';
        date[12] = (hour % 10) + '0';

        date[13] = ':';

        date[14] = (min / 10) + '0';
        date[15] = (min % 10) + '0';

        date[16] = ':';

        date[17] = (sec / 10) + '0';
        date[18] = (sec % 10) + '0';

        date[19] = '\n';
        date[20] = '\0';

        Print(date);

    }
}
