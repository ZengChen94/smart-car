#include <stdio.h>
#include "zrcar.h"
#include "zrcar.c"
void car_left();
void car_right();
void car_back();
void car_forward();
void car_stop();

int main()
{
    int i;
	int value=0;
    int leftSpeed;
    int rightSpeed;
    int Optical__value_signal;
    float Ultrasonic__value_signal;
    float ult_data[3];


    zrcar_wheel_init(ZRCAR_WHEEL_DEV);
    zrcar_opt_init(ZRCAR_OPTICAL_DEV);
    zrcar_ultrasonic_init(ZRCAR_ULTRASONIC_DEV);
	zrcar_gpio_init(ZRCAR_GPIO_DEV);
	zrcar_gpio_set(value);



	while(1)
	{
		/***************optical read**************/
		zrcar_ultra_get_all(ult_data);
/*		for(i = 1; i < 4; i++){
			printf("u%d : %f mm",i , ult_data[i-1]);
		}
		printf("\n");*/
			 if(( ult_data[2] < 180) && (ult_data[1] < 120)&&(ult_data[0] < 180 ))		//huge size
				{
				value=0xffff;
				zrcar_gpio_set(value);
				car_back();
				usleep(200000);    //delay
				if(ult_data[2]<ult_data[0])
					while(ult_data[1]<300)
					{
						car_right();
                        zrcar_ultra_get_all(ult_data);
					}
				if(ult_data[0]<ult_data[2])
					while(ult_data[1]<300)
					{
						car_left();
						zrcar_ultra_get_all(ult_data);
					}
				}


			else if((ult_data[0] < 180) && (ult_data[1] < 180)&&(ult_data[2] >180 ))		//normal size left
				{
				value=0xffff;
				zrcar_gpio_set(value);
				car_back();
				usleep(400000);    //delay
				car_left();
				usleep(400000);
				}
			else if((ult_data[0] > 180) && (ult_data[1] < 180)&&(ult_data[2] < 180 ))	//normal size right
				{
				value=0xffff;
				zrcar_gpio_set(value);
				car_back();
				usleep(400000);    //delay
				car_right();
				usleep(400000);
				}
			else if ((ult_data[0] > 180) && (ult_data[2] > 180)&&(ult_data[1]<180  ))	//mini size middle
				{
				value=0x0;
				zrcar_gpio_set(value);
					if(ult_data[0]>190&&ult_data[2]<190)
					{
						car_right();
					}
					else
						car_left();
				
				}
		
			else if((ult_data[2] > 180) && (ult_data[1] > 180)&&(ult_data[0] < 180 ))	//wall left
				{
				value=0x0;
				zrcar_gpio_set(value);
				car_left();
				}
			else if ((ult_data[0] > 180) && (ult_data[1] > 180)&&(ult_data[2] < 180 ))	//wall right
				{
				value=0x0;
				zrcar_gpio_set(value);
				car_right();
				}

		else 
			{
			value=0x0;
			zrcar_gpio_set(value);
			car_forward();
			}
		usleep(200000);    //delay
	}

    return 0;
}
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