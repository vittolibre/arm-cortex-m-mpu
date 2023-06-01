# ARM-Cortex-M-MPU

This is an example of how to configure the MPU of the ARM-Cortex-M embedd on STM32F407G-DISC1.

# How this example do?
We use the user button (the blu one) for switching the MPU configuration on the led memory area( GPIOD )for blinking or not the 4 leds.
In particular when the button is pressed, an interrupt is generated and managed by the callback HAL_GPIO_EXTI_Callback(), in which change the permissions of the area in: Read only or Read/Write.

# How it works?

# ./main_MPU.c
1)The MPU configures in Read/write the GPIOD's area that starting from the address: 0x40020C00 and have 1KB size.

2)A while cycle in which we toggling the 4 LEDs.

# ./main_lib.c
3) Pushing one time the user button, the HAL_GPIO_EXTI_Callback() implementation is called and change the permissions in Read Only.

# ./stm32f4xx_it.c
4) Since we have no longer permits to write the LEDs, when we try to write to the LEDs memory area, the MemManage_Handler() is raised to handle this exception fault.

# ./main_lib.c 
5) Pushing another time the button, the HAL_GPIO_EXTI_Callback() implementation is called and change the permissions in Read/write, and LEDs start blinking from where they had stopped.
