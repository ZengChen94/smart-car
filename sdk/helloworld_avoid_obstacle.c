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

/*
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
	zrcar_wheel_r_set(8);
}
void car_left()
{
	zrcar_wheel_l_set(8);
	zrcar_wheel_r_set(0);
}

int main()
{
	zrcar_wheel_init();
	zrcar_bluetooth_init();

	u8 buff[198];
	u8 data[33];
	float a[3],w[3],angle[3];
	int i, j, k;

	//car_forward();

	while(1)
	{
		//read data from series
		zrcar_bluetooth_recv(&buff, 198);

		for(i = 0; i < 165; i++)
		{
			if(buff[i] == 0x55 && buff[i+11] == 0x55 && buff[i+22] == 0x55 && buff[i+1] == 0x51 && buff[i+12] == 0x52 && buff[i+23] == 0x53)
			{
				for(j = 0; j < 33; j++)
				{
					data[j] = buff[i+j];
				}
				break;
			}
		}

		a[0] = ((short)(data[3]<<8| data[2]))/32768.0*16;
		a[1] = ((short)(data[5]<<8| data[4]))/32768.0*16;
		a[2] = ((short)(data[7]<<8| data[6]))/32768.0*16;
		w[0] = ((short)(data[14]<<8| data[13]))/32768.0*2000;
		w[1] = ((short)(data[16]<<8| data[15]))/32768.0*2000;
		w[2] = ((short)(data[18]<<8| data[17]))/32768.0*2000;
		angle[0] = ((short)(data[25]<<8| data[24]))/32768.0*180;
		angle[1] = ((short)(data[27]<<8| data[26]))/32768.0*180;
		angle[2] = ((short)(data[29]<<8| data[28]))/32768.0*180;

		printf("%f %f %f\r\n", a[0], a[1], a[2]);
		printf("%f %f %f\r\n", w[0], w[1], w[2]);
		//printf("a_x:%f, a_y:%f, a_z:%f\r\n", a[0], a[1], a[2]);
		//printf("w_x:%f, w_y:%f, w_z:%f\r\n", w[0], w[1], w[2]);
		//printf("an_x:%f, an_y:%f, an_z:%f\r\n\r\n", angle[0], angle[1], angle[2]);

		sleep(1);
	}
    return 0;
}
*/

#include <stdio.h>
#include <math.h>
//#include "math.h"
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

int main()
{
	zrcar_wheel_init();
	zrcar_bluetooth_init();
	zrcar_ultra_init();

	u8 buff[198];
	u8 data[33];
	float a[3],w[3],angle[3];
	int i, j, k;

	float deltaDistance = 0, deltaAngle = 0;
	float velocityX = 0;
	float deltaDistanceThreshold = 0.01;

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
	int times[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int tmp, cnt, time;
	int whetherLine = 1;

	car_forward();
	usleep(10*STANDARD_INTERVAL);

	while(1)
	{
		zrcar_ultra_get_all(ult_data);

		//read data from series
		zrcar_bluetooth_recv(&buff, 198);

		for(i = 0; i < 165; i++)
		{
			if(buff[i] == 0x55 && buff[i+11] == 0x55 && buff[i+22] == 0x55 && buff[i+1] == 0x51 && buff[i+12] == 0x52 && buff[i+23] == 0x53)
			{
				for(j = 0; j < 33; j++)
				{
					data[j] = buff[i+j];
				}
				break;
			}
		}

		a[0] = ((short)(data[3]<<8| data[2]))/32768.0*16;
		a[1] = ((short)(data[5]<<8| data[4]))/32768.0*16;
		a[2] = ((short)(data[7]<<8| data[6]))/32768.0*16;
		w[0] = ((short)(data[14]<<8| data[13]))/32768.0*2000;
		w[1] = ((short)(data[16]<<8| data[15]))/32768.0*2000;
		w[2] = ((short)(data[18]<<8| data[17]))/32768.0*2000;
		angle[0] = ((short)(data[25]<<8| data[24]))/32768.0*180;
		angle[1] = ((short)(data[27]<<8| data[26]))/32768.0*180;
		angle[2] = ((short)(data[29]<<8| data[28]))/32768.0*180;

		//printf("%f %f %f\r\n", a[0], a[1], a[2]);
		//printf("%f %f %f\r\n", w[0], w[1], w[2]);
		//printf("a_x:%f, a_y:%f, a_z:%f\r\n", a[0], a[1], a[2]);
		//printf("w_x:%f, w_y:%f, w_z:%f\r\n", w[0], w[1], w[2]);
		//printf("an_x:%f, an_y:%f, an_z:%f\r\n\r\n", angle[0], angle[1], angle[2]);

		deltaAngle += w[2] * 0.2;
		velocityX += ((a[0] * 9.8 * sin(deltaAngle/180*3.14) + a[1] * 9.8 * cos(deltaAngle/180*3.14)) * 0.2);
		deltaDistance += velocityX * 0.2;
		while (deltaAngle > 360)
			deltaAngle -= 360;
		while (deltaAngle < -360)
			deltaAngle += 360;
		//usleep(100000);
		//printf("a_x:%f, a_y:%f, a_z:%f\r\n", a[0], a[1], a[2]);
		//printf("w_x:%f, w_y:%f, w_z:%f\r\n", w[0], w[1], w[2]);
		//printf("an_x:%f, an_y:%f, an_z:%f\r\n\r\n", angle[0], angle[1], angle[2]);
		//printf("velocityX = %f\r\n", velocityX);
		//printf("deltaAngle = %f\r\n", deltaAngle);
		//printf("deltaDistance = %f\r\n", deltaDistance);

		/*
		//add by zcunique
		if(deltaDistance < deltaDistanceThreshold && deltaDistance > -1*deltaDistanceThreshold)//if on the line, correct the direction
		{
			while(deltaAngle > 10)
			{
				car_rotate_left();
				count -= 1;
				usleep(STANDARD_INTERVAL);
			}
			while(deltaAngle > 10)
			{
				car_rotate_right();
				count += 1;
				usleep(STANDARD_INTERVAL);
			}
			car_forward();
		}
		*/


		//recover direction
		if(attempt == 1)//deltaDistance != 0
		{
			if(count > 0)//right
			{
				car_rotate_left();
				count -= 1;
				usleep(STANDARD_INTERVAL);
				if(!( ult_data[2] >0&&ult_data[2] < 180))//no obstacle left
				{
					continue;
				}
				else
				{
					attempt = 0;
				}
			}
			if(count < 0)//left
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

		//2 --> left; 1 --> middle; 0 --> right
		if((ult_data[0] >0 &&ult_data[0] < 180) )//obstacle on the right
		{
			car_rotate_left();
			count -= 1;
			usleep(STANDARD_INTERVAL);
			//whetherLine = 0;
			//direction_changed = -1;
		}
		else if( ult_data[2] >0&&ult_data[2] < 180 )//obstacle on the left
		{
			car_rotate_right();
			count += 1;
			usleep(STANDARD_INTERVAL);
			//whetherLine = 0;
			//direction_changed = 1;
		}
		else if((ult_data[1] >0 &&ult_data[1] < 90))//obstacle on the middle
		{
			car_rotate_left();
			count -= 1;
			usleep(STANDARD_INTERVAL);
			//whetherLine = 0;
			//direction_changed = -1;
		}
		else//no obstacle
		{
			car_forward();
			if(count == 0)//order
			{
				usleep(STANDARD_INTERVAL);
			}
			else
			{
				usleep(10*STANDARD_INTERVAL);
				attempt = 1;
				/*
				if (count > 0 && count < 20)
					times[count] += 1;
				if (count < 0 && count > -20)
					times[count * (-1)] += 1;
					*/
			}
		}
		/*
		if (whetherLine == 0 && count == 0)
		{
			cnt = 0;
			time = 0;
			for (tmp = 0; tmp < 20; tmp += 1)
			{
				cnt += tmp * times[tmp];
				time += times[tmp];
			}
			cnt = (int)(cnt / time);
			if (direction_changed == 1)
			{
				car_rotate_left();
				usleep(STANDARD_INTERVAL * cnt);
				car_forward();
				usleep(STANDARD_INTERVAL * time * 10);
				car_rotate_right();
				usleep(STANDARD_INTERVAL * cnt);
				car_forward();
			}
			if (direction_changed == -1)
			{
				car_rotate_right();
				usleep(STANDARD_INTERVAL * cnt);
				car_forward();
				usleep(STANDARD_INTERVAL * time * 10);
				car_rotate_left();
				usleep(STANDARD_INTERVAL * cnt);
				car_forward();
			}
		}
		*/
	}
    return 0;
}

