#include <msp430.h>
#include <intrinsics.h>

int dht[40] = {[0 ... 39] = 5};
int i;
int check_sum = 0;

void clock_init()
{
    // MCLK set to 250kHz, SMCLK set to 125kHz
    DCOCTL = DCO1+DCO0;
    BCSCTL1 = DIVA_0+RSEL0+RSEL1+RSEL2;
    BCSCTL2 = DIVM_2+DIVS_3+SELM_0;
};

void gpio_init()
{
    P1DIR |= BIT0;  // Set P1.0 to output direction
    P1DIR |= BIT7;  // Set P1.7 to output direction
    P1OUT != BIT7;  // Set P1.7 to HIGH
//    P1SEL |= 0x80;
//    P1SEL2 &= ~(0x80);
    P1OUT &= ~BIT0;

//    P1DIR &= ~BIT3;
//    P1REN |= BIT3;
////    P1IES |= BIT3;
////    P1IE |= BIT3;
//    P1IFG &= ~BIT3;

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

void get_temp_and_humidity()
{
    P1OUT &= ~BIT7;
    _delay_cycles(5000);
    P1OUT |= BIT7;
    _delay_cycles(5);
    P1OUT &= ~BIT7;
    P1DIR &= ~BIT7; // Set P1.7 to input direction

    i = 0;
    check_sum = 0;
    P1IE |= BIT7;
    P1IES &= ~BIT7;
}


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
//    TA1CCTL0 &= ~BIT0;
//    TACTL &= ~BIT0;
    _BIS_SR(GIE); // sleep untill interupt

    get_temp_and_humidity();
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    if(i != 0)
    {
        _delay_cycles(9);
        if( (P1IN & BIT7) == BIT7)
        {
            dht[i-1] = 1;
            check_sum += 1;
        }
        else
            dht[i-1] = 0;
    }
    if(i ==  41)
    {
        P1IE &= ~(BIT7);
        P1DIR |= BIT7;
        P1OUT |= BIT7;
        P1OUT ^= BIT0;
        TA1CCTL0 |= CCIE;
    }

    i++;
    P1IFG &= ~BIT7;

}
