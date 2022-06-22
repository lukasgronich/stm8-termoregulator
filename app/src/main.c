#include "stm8s.h"
#include "LiquidCrystal_I2C.h"
#include <stdio.h>

uint8_t counter=0;
uint8_t counter2=0;
uint8_t procento=0;
uint8_t counter_str[5];
uint8_t currentStateCLK;
uint8_t lastStateCLK;
uint8_t stav=0;

INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
    if(GPIO_ReadInputPin(GPIOB, GPIO_PIN_2) == RESET){
        if(stav == 0){
            stav=1;
        }
        else{
            stav=0;
        }
    }
}

void main(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // FREQ MCU 16MHz

    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_FALL_ONLY);
    ITC_SetSoftwarePriority(ITC_IRQ_PORTB, ITC_PRIORITYLEVEL_0);
    enableInterrupts();
    
    
    LCD.init(0x27,16,2);
    
    
    GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(GPIOB, GPIO_PIN_2, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_SLOW); // LED 1 MODRÁ
    GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_SLOW); // LED 1 ZELENÁ
    GPIO_Init(GPIOE, GPIO_PIN_0, GPIO_MODE_OUT_PP_HIGH_SLOW); // LED 2 MODRÁ
    GPIO_Init(GPIOC, GPIO_PIN_1, GPIO_MODE_OUT_PP_HIGH_SLOW); // LED 2 ZELENÁ
    GPIO_Init(GPIOG, GPIO_PIN_0, GPIO_MODE_OUT_PP_HIGH_SLOW); // LED 3 MODRÁ
    GPIO_Init(GPIOC, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_SLOW); // LED 3 ZELENÁ
    GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_SLOW); // LED 4 MODRÁ
    GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_SLOW); // LED 4 ZELENÁ
    GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_SLOW); // LED 5 MODRÁ
    GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_SLOW); // LED 5 ZELENÁ
    lastStateCLK = GPIO_ReadInputPin(GPIOB, GPIO_PIN_1); // GPIO_ReadInputPin
    
 while (1)
 {   
    if(stav == 0){
        
        // Read the current state of CLK
        currentStateCLK = GPIO_ReadInputPin(GPIOB, GPIO_PIN_1); // GPIO_ReadInputPin

        // If last and current state of CLK are different, then pulse occurred
        // React to only 1 state change to avoid double count
        if (currentStateCLK != lastStateCLK && !currentStateCLK)
        {
            
            // If the DT state is different than the CLK state then
            // the encoder is rotating CCW so decrement
            if (GPIO_ReadInputPin(GPIOB, GPIO_PIN_0) != currentStateCLK){  
                if(counter == 58){
                    counter-=58;
                    procento--;
                }
                else if(counter > 252){
                    counter-=1;
                    procento--;
                }
                else if(counter == 0){
                    counter-=2;
                    procento--;
                }
                else{
                    procento--;
                    counter-=2;
                }   
            }

            else{
                // Encoder is rotating CW so increment
                if(counter == 0){
                    counter+=58;
                    procento++;
                } 
                else if(counter > 251){
                    if(counter < 254){
                        counter+=1;
                        procento++;
                    }
                    else{
                        procento++;
                        counter+=2;  
                    }
                }   
                
                else{
                    procento++;
                    counter+=2;
                }
                    
                
            }
            if(procento > 101 ){
                procento=100;
            }
            if(procento == 101 ){
                procento=0;
            }

            LCD.clear();
            LCD.setCursor(0, 0);
            sprintf(counter_str, "%u", procento);
            LCD.print(counter_str);
            LCD.print("%");
            LCD.setCursor(0, 1);
            sprintf(counter_str, "%u", counter);
            LCD.print(counter_str);

            if(procento > 19){
                GPIO_WriteLow(GPIOD, GPIO_PIN_6);
                GPIO_WriteLow(GPIOD, GPIO_PIN_5);
            } 
            else{
                GPIO_WriteHigh(GPIOD, GPIO_PIN_6); 
                GPIO_WriteHigh(GPIOD, GPIO_PIN_5);
            }
            if(procento > 39){
                GPIO_WriteLow(GPIOE, GPIO_PIN_0);
                GPIO_WriteLow(GPIOC, GPIO_PIN_1);
            } 
            else{
                GPIO_WriteHigh(GPIOE, GPIO_PIN_0); 
                GPIO_WriteHigh(GPIOC, GPIO_PIN_1);
            }
            if(procento > 59){
                GPIO_WriteLow(GPIOG, GPIO_PIN_0);
                GPIO_WriteLow(GPIOC, GPIO_PIN_2);
            } 
            else{
                GPIO_WriteHigh(GPIOG, GPIO_PIN_0);
                GPIO_WriteHigh(GPIOC, GPIO_PIN_2);
            }
            if(procento > 79){
                GPIO_WriteLow(GPIOC, GPIO_PIN_3);
                GPIO_WriteLow(GPIOE, GPIO_PIN_5);    
            } 
            else{
                GPIO_WriteHigh(GPIOC, GPIO_PIN_3);
                GPIO_WriteHigh(GPIOE, GPIO_PIN_5);
            }
            if(procento > 99){
                GPIO_WriteLow(GPIOD, GPIO_PIN_3);
                GPIO_WriteLow(GPIOC, GPIO_PIN_4); 
            } 
            else{
                GPIO_WriteHigh(GPIOD, GPIO_PIN_3);
                GPIO_WriteHigh(GPIOC, GPIO_PIN_4);
            }
        }

        

        counter2 +=1;   

        if(counter > counter2){
            GPIO_WriteHigh(GPIOC, GPIO_PIN_5);
        }
        else{
            GPIO_WriteLow(GPIOC, GPIO_PIN_5);
        }
            
        // Remember last CLK state
        lastStateCLK = currentStateCLK;

        // Put in a slight delay to help debounce the reading
        delay.ms(1); 
    } 

    if(stav == 1){
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("ZAPNUTO");
            
    }  
 }
//  
}