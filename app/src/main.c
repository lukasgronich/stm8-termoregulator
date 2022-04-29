#include "stm8s.h"
#include "LiquidCrystal_I2C.h"
void main(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // FREQ MCU 16MHz
    LCD.init(0x26,16,2);
    LCD.print("Pussy");
    while (1)
    {
        
    }
}
