#include "stm8s.h"
#include "LiquidCrystal_I2C.h"
#include <stdio.h>

uint8_t counter=0;
uint8_t counter_str[5];
uint8_t currentStateCLK;
uint8_t lastStateCLK;

void main(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // FREQ MCU 16MHz
    
    
    LCD.init(0x27,16,2);
    
    
    GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_SLOW);
    lastStateCLK = GPIO_ReadInputPin(GPIOB, GPIO_PIN_1); // GPIO_ReadInputPin

 while (1)
 {
     // Read the current state of CLK
     currentStateCLK = GPIO_ReadInputPin(GPIOB, GPIO_PIN_1); // GPIO_ReadInputPin

     // If last and current state of CLK are different, then pulse occurred
     // React to only 1 state change to avoid double count
     if (currentStateCLK != lastStateCLK && !currentStateCLK)
     {
        GPIO_WriteReverse(GPIOC, GPIO_PIN_5);
         // If the DT state is different than the CLK state then
         // the encoder is rotating CCW so decrement
         if (GPIO_ReadInputPin(GPIOB, GPIO_PIN_0) != currentStateCLK)
         {
             counter--;
         }
         else
         {
             // Encoder is rotating CW so increment
             counter++;
         }

         LCD.clear();
         LCD.setCursor(0, 0);
         sprintf(counter_str, "%u", counter);
         LCD.print(counter_str);

     }

     // Remember last CLK state
     lastStateCLK = currentStateCLK;

     // Put in a slight delay to help debounce the reading
     delay.ms(1);

    
     
 }

}
