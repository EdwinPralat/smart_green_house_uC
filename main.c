#include <msp430.h>
#include <stdio.h>

void clock_init()
{
    // MCLK set to 250kHz, SMCLK set to 125kHz
    DCOCTL = DCO1+DCO0;
    BCSCTL1 = DIVA_0+RSEL0+RSEL1+RSEL2;
    BCSCTL2 = DIVM_2+DIVS_3+SELM_0;
};

void gpio_init()
{
    P1DIR |= 0x01;  // Set P1.0 to output direction
};

void timer_init()
{
    TA1CCTL0 = CCIE;
    TA1CTL = TASSEL_2 + MC_1 + ID_3; /* TASSEL_2: use SMCLK MC_1: upcount ID_3: divide by 8 f = 125kHz/8=15625 */
    TA1CCR0 = 0xF424; // setting counter to interrupt with every 4s‬
};

void interruptions_init()
{
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    _BIS_SR(CPUOFF + GIE); // sleep untill interupt
};
//
//#pragma vector=0xFFF2
//#pragma vector=0xFFFA
//__interrupt void TIMER0_A0_ISR()
//{
////    switch(TA1IV)
////    {
////        case TA1IV_TAIFG:
//    P1OUT ^= 0x01;
//    TA0CCR0 += 32000-1;
//    TA0CTL &= ~TAIFG;
//    //            break;
////        default:
////            _never_executed();
//
////    }
//};
//
//



int main(void)
{
    clock_init();
    gpio_init();
    timer_init();
    interruptions_init();
    while(1)
    {

  }
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
    P1OUT ^= 0x01;
    puts('hi');
}

