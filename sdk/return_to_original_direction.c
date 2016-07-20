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
#include <unistd.h>
#include "platform.h"
#include "xparameters.h"
#include "sleep.h"
#include "zrcar.h"
#include "xil_printf.h"

void car_stop()
{
	zrcar_wheel_l_set(0);
	zrcar_wheel_r_set(0);
}
void car_forward()
{
	zrcar_wheel_l_set(8);
	zrcar_wheel_r_set(8);
}
void car_back()
{
	zrcar_wheel_l_set(-8);
	zrcar_wheel_r_set(-8);
}
void car_right()
{
	zrcar_wheel_l_set(0);
	zrcar_wheel_r_set(-8);
}
void car_left()
{
	zrcar_wheel_l_set(-8);
	zrcar_wheel_r_set(0);
}
void car_rotate_left()
{
	zrcar_wheel_l_set(-8);
	zrcar_wheel_r_set(8);
}
void car_rotate_right()
{
	zrcar_wheel_l_set(8);
	zrcar_wheel_r_set(-8);
}

int main()
{
	/*********init*******/
	zrcar_wheel_init(ZRCAR_WHEEL_DEV);
	zrcar_ultrasonic_init(ZRCAR_ULTRASONIC_DEV);

	// Ultrasonic__value_all

	float ult_data[3];
	int direction = 2;
	// 2-forward, 1-left, 3-right, 0-backward
	int offset = 0;
	// -1 left, +1 right
	int direction_changed = 0;
	// 0 unchanged, -1 change to left, 1 change to right
	int STANDARD_INTERVAL = 200000;
	int count = 0;
	int attempt = 0;

	car_forward();
	usleep(100*STANDARD_INTERVAL);

	while(1)
	{
		zrcar_ultra_get_all(ult_data);
		if(attempt == 1)
		{
			if(count > 0)
			{
				car_rotate_left();
				count -= 1;
				usleep(STANDARD_INTERVAL);
				if(!( ult_data[2] >0&&ult_data[2] < 270))
				{
					continue;
				}
				else
				{
					attempt = 0;
				}
			}
			if(count < 0)
			{
				car_rotate_right();
				count += 1;
				usleep(STANDARD_INTERVAL);
				if(!(ult_data[0] >0 &&ult_data[0] < 180) )
				{
					continue;
				}
				else
				{
					attempt = 0;
				}
			}
			// attempt = 0;
		}

		if((ult_data[0] >0 &&ult_data[0] < 180) )
		{

			// usleep(STANDARD_INTERVAL);
			car_rotate_left();

			count -= 1;
			usleep(STANDARD_INTERVAL);
		}
		else if( ult_data[2] >0&&ult_data[2] < 270 )
		{

			// usleep(STANDARD_INTERVAL);
			car_rotate_right();
			count += 1;
			usleep(STANDARD_INTERVAL);
		}
//		else if((ult_data[1] >0 &&ult_data[1] < 180))
//		{
//
//			// usleep(STANDARD_INTERVAL);
//			car_rotate_right();
//
//			count += 1;
//			usleep(STANDARD_INTERVAL);
//		}
		else
		{
			car_forward();
			if(count == 0)
			{
				usleep(STANDARD_INTERVAL);
			}
			else
			{
				usleep(10*STANDARD_INTERVAL);
				attempt = 1;
			}
		}



	}


    return 0;
}

