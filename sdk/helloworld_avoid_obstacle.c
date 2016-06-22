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
	/*********init*******/
	zrcar_wheel_init();
	zrcar_ultra_init();
	zrcar_bluetooth_init();

	float ult_data[3];

	u8 buff[198];
	u8 data[33];
	float a[3],w[3],angle[3];
	int i, j, k;

	car_forward();

	while(1)
	{
		//read data from series
		zrcar_bluetooth_recv(&buff, 198);

		for(i = 0; i < 165; i++)
		{
			if(buff[i] == 0x55 && buff[i+11] == 0x55 && buff[i+22] == 0x55
					&& buff[i+1] == 0x51 && buff[i+12] == 0x52 && buff[i+23] == 0x53)
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

		printf("a_x:%f, a_y:%f, a_z:%f\r\n", a[0], a[1], a[2]);
		printf("w_x:%f, w_y:%f, w_z:%f\r\n", w[0], w[1], w[2]);
		//printf("an_x:%f, an_y:%f, an_z:%f\r\n\r\n", angle[0], angle[1], angle[2]);

		//read data from sensor
		zrcar_ultra_get_all(ult_data);

		for(k = 1; k < 4; k++){
			printf("u%d : %f mm", k , ult_data[k-1]);
		}
		printf("\n\n\n");


		sleep(1);
	}
    return 0;
}

