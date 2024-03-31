
#include "init.h"

uint8_t style = 0x00;
uint8_t LEDS[4] = {LED3,LED5,LED6,LED4};
void init(void){

	  /* TODO - Add your application code here */

	  /* Initialize LEDs */
	  STM_EVAL_LEDInit(LED3);
	  STM_EVAL_LEDInit(LED4);
	  STM_EVAL_LEDInit(LED5);
	  STM_EVAL_LEDInit(LED6);
	  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);

}

void play(void){

	static uint8_t i = 0;
	/* Turn on LEDs */
	//STM_EVAL_LEDToggle(LEDS[i%4]);

	switch(style % 4){
	case 0:
		  STM_EVAL_LEDOff(LEDS[  i%4]);
		  STM_EVAL_LEDOn (LEDS[++i%4]);
		break;
	case 1:
		  STM_EVAL_LEDOff(LEDS[--i%4]);
		  STM_EVAL_LEDOn (LEDS[++i%4]);
		  STM_EVAL_LEDOn (LEDS[++i%4]);
		break;
	case 2:
		  STM_EVAL_LEDOn (LEDS[  i%4]);
		  STM_EVAL_LEDOff(LEDS[++i%4]);
		break;
	case 3:
		  STM_EVAL_LEDOn (LEDS[--i%4]);
		  STM_EVAL_LEDOff(LEDS[++i%4]);
		  STM_EVAL_LEDOff(LEDS[++i%4]);
		break;
	}
}

/* Set interrupt handlers */
/* Handle PD0 interrupt */
void EXTI0_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        /* Do your stuff when PD0 is changed */
        ++style;

        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void delay_ms(uint32_t ms){
	uint32_t now = get_counter();
	while(((get_counter()- now) < ms));
}
