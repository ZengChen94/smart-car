/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include"zrcar.c"
#include "zrcar.h"

void right_action();
void left_action();

void car_stop()
{
	zrcar_wheel_l_set(0);
	zrcar_wheel_r_set(0);
}
void car_forward()
{
	zrcar_wheel_l_set(15);
	zrcar_wheel_r_set(15);
}
void car_back()
{
	zrcar_wheel_l_set(-15);
	zrcar_wheel_r_set(-15);
}
void car_left()
{
	zrcar_wheel_l_set(12);
	zrcar_wheel_r_set(0);
}
void car_right()
{
	zrcar_wheel_l_set(0);
	zrcar_wheel_r_set(12);
}
int main()
{
	/*********init*******/
	zrcar_wheel_init(ZRCAR_WHEEL_DEV);
	zrcar_opt_init(ZRCAR_OPTICAL_DEV);
	zrcar_ultrasonic_init(ZRCAR_ULTRASONIC_DEV);

	// Optical__value_all
	int optical_data;




	while(1)
	{
		/***************optical read**************/
		zrcar_opt_get_all(&optical_data);
		car_forward();

		 if(!(optical_data & 0x01))
		 { 
                     while(!(optical_data & 0x01))
                     {  
                        zrcar_opt_get_all(&optical_data);
	    	             car_right();
                     }
					 car_right();  
					 usleep(300000);
	         }
	     else if(!(optical_data& 0x10))
		 {                       
                    while(!(optical_data & 0x10))
                     {  
                     zrcar_opt_get_all(&optical_data);
	    	         car_left();
			
                     }
					car_left();
					usleep(300000);

                       
	     }
                else 
		{
			 car_forward();
	     }
			    usleep(100000);    //delay 0.1s
	}

    return 0;
}

