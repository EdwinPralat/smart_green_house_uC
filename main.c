#include <msp430.h>				

void main(void)
{
    clock_config();
    gpio_config();
    interruptions_config();

	while(1)
	{

	}
}

void clock_config()
{
    DCOCTL |= 0x60;
    BCSCTL1 |= 0x07;
    BCSCTL2 |= 0x04;
    BCSCTL3 |=
};

void gpio_config()
{};

void interruptions_config()
{};
