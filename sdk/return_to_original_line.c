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
	zrcar_wheel_l_set(-8);
	zrcar_wheel_r_set(-8);
}
void car_back()
{
	zrcar_wheel_l_set(8);
	zrcar_wheel_r_set(8);
}
void car_right()
{
	zrcar_wheel_l_set(0);
	zrcar_wheel_r_set(8);
}
void car_left()
{
	zrcar_wheel_l_set(8);
	zrcar_wheel_r_set(0);
}
void car_rotate_left()
{
	zrcar_wheel_l_set(8);
	zrcar_wheel_r_set(-8);
}
void car_rotate_right()
{
	zrcar_wheel_l_set(-8);
	zrcar_wheel_r_set(8);
}
int sumArray(int input[41])
{
	int i;
	int sum = 0;
	for(i = 0; i <= 40; i ++)
	{
		sum += input[i];
	}
	return sum;
}

int main()
{
	/*********init*******/
	zrcar_wheel_init();
	zrcar_ultra_init();

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
	int countArray[41];
	int i;
	int temp = 0;

	for(i = 0; i <= 40; i ++)
	{
		countArray[i] = 0;
	}

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
				if(!( ult_data[2] >0&&ult_data[2] < 180))
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
		else if( ult_data[2] >0&&ult_data[2] < 180 )
		{
			// usleep(STANDARD_INTERVAL);
			car_rotate_right();
			count += 1;
			usleep(STANDARD_INTERVAL);
		}
		else if((ult_data[1] >0 &&ult_data[1] < 180))
		{
			car_rotate_right();
			count += 1;
			usleep(STANDARD_INTERVAL);
		}
		else
		{
			car_forward();
			if(count == 0)
			{
				usleep(STANDARD_INTERVAL);

				if(sumArray(countArray) != 0)
				{
					// if(!((ult_data[0] >0 &&ult_data[0] < 720)||(ult_data[0] >0 &&ult_data[0] < 720)))
					// {
						// car_forward();
						// usleep(30*STANDARD_INTERVAL);
						for(i = 0; i <= 40; i ++)
						{
							if(countArray[i] != 0)
							{
								temp = i - 20;
								if (temp > 0)
								{
									//if have rotate right, now rotate left
									car_rotate_left();
									usleep(temp*STANDARD_INTERVAL);


									zrcar_ultra_get_all(ult_data);


									if((ult_data[0] >0 &&ult_data[0] < 180)||(ult_data[2] >0 &&ult_data[2] < 180)||(ult_data[1] >0 &&ult_data[1] < 180))
									{
										car_rotate_right();
										usleep(temp*STANDARD_INTERVAL);
										car_forward();
										usleep(20*STANDARD_INTERVAL);
										break;
									}
									car_forward();
									usleep(countArray[i]*STANDARD_INTERVAL);
									car_rotate_right();
									usleep(temp*STANDARD_INTERVAL);
									// count += temp;

								}
								else if(temp < 0)
								{
									car_rotate_right();
									usleep((-temp)*STANDARD_INTERVAL);

									// count += (-temp);
									zrcar_ultra_get_all(ult_data);
									// if ((ult_data[0] >0 &&ult_data[0] < 180))
									// {
									// 	break;
									// }
									if((ult_data[0] >0 &&ult_data[0] < 180)||(ult_data[2] >0 &&ult_data[2] < 180)||(ult_data[1] >0 &&ult_data[1] < 180))
									{
										car_rotate_left();
										usleep((-temp)*STANDARD_INTERVAL);
										car_forward();
										usleep(20*STANDARD_INTERVAL);
										break;
									}
									car_forward();
									usleep(countArray[i]*STANDARD_INTERVAL);
									car_rotate_left();
									usleep(-(temp)*STANDARD_INTERVAL);
									// count -= (-temp);
								}
								countArray[i] = 0;
							}
						}
					// }
				}
			}
			else
			{
				usleep(10*STANDARD_INTERVAL);
				attempt = 1;

				countArray[count+20] += 10;
			}
		}

	}


    return 0;
}

