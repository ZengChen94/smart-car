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
#include "zrcar.c"
#include "zrcar.h"

void car_stop()
{
	zrcar_wheel_l_set(0);
	zrcar_wheel_r_set(0);
}
void car_forward()
{
	zrcar_wheel_l_set(20);
	zrcar_wheel_r_set(20);
}
void car_back()
{
	zrcar_wheel_l_set(-20);
	zrcar_wheel_r_set(-20);
}
void car_right_h()
{
	zrcar_wheel_l_set(20);
	zrcar_wheel_r_set(5);
}
void car_right_s()
{
	zrcar_wheel_l_set(25);
	zrcar_wheel_r_set(10);
}
void car_left_h()
{
	zrcar_wheel_l_set(5);
	zrcar_wheel_r_set(20);
}
void car_left_s()
{
	zrcar_wheel_l_set(10);
	zrcar_wheel_r_set(25);
}
void car_turn()
{
	zrcar_wheel_l_set(20);
	zrcar_wheel_r_set(-20);
}

int main()
{
	int optical_data;
	/*********init*******/
	/*********init*******/
	zrcar_wheel_init(ZRCAR_WHEEL_DEV);
	zrcar_opt_init(ZRCAR_OPTICAL_DEV);
	zrcar_ultrasonic_init(ZRCAR_ULTRASONIC_DEV);


	// Optical__value_all

	 car_forward();


	while(1)
	{
		/***************optical read**************/
		zrcar_opt_get_all(&optical_data);
		car_forward();
		if(optical_data  == 0x1F)
			{
				car_turn();
			}
		else if(optical_data & 0x04)
		{
			car_stop();
			if((!(optical_data & 0x01)) && (!(optical_data & 0x02)))
				car_right_h();
			if((!(optical_data & 0x08)) && (!(optical_data & 0x10)))
				car_left_h();
		}
		else if(!(optical_data & 0x01))
				car_right_s();
		else if(!(optical_data & 0x10))
                car_left_s();
		else
				car_forward();
	}
    return 0;
}

