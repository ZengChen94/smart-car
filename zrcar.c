/*
 * zrcar.c
 *
 *  Created on: 2014-12-31
 *      Author: ZRobot
 */


#include "zrcar.h"
#include "xuartps.h"
#include "xuartps_hw.h"

/****************************Optical gpio declaration************************/
static XGpio Opt;
static XGpio GPIO;
static XUartPs Uart1_Ps;

int zrcar_servo_init()
{

	SERVO_BASEADDR = XPAR_ZCAR_SERVO_V1_0_0_BASEADDR;
	return XST_SUCCESS;
}
int zrcar_servo0_set(int angle)
{
	u16 pwm_duty = 500+ 2000.0/180.0 * angle;
	servo_mWriteReg(SERVO_BASEADDR,SERVO0_OFFSET,pwm_duty);

	return XST_SUCCESS;
}
int zrcar_servo1_set(int angle)
{
	u16 pwm_duty = 500+ 2000.0/180.0 * angle;
	servo_mWriteReg(SERVO_BASEADDR,SERVO1_OFFSET,pwm_duty);
	return XST_SUCCESS;
}


int zrcar_wheel_init()
{
	WHEEL_CTRL_L_BASEADDR = XPAR_MOTOR_CTRL_L_MOTOR_S_AXI_BASEADDR;
	WHEEL_CTRL_R_BASEADDR = XPAR_MOTOR_CTRL_R_MOTOR_S_AXI_BASEADDR;

	//MOTOR Controller IP Initialize
    Xil_Out32((WHEEL_CTRL_L_BASEADDR) + WHEEL_Controller_OFFSET, (0x0));
    Xil_Out32((WHEEL_CTRL_L_BASEADDR) + WHEEL_SPEED_SET_OFFSET, (-0)); //in_speed_set
    Xil_Out32((WHEEL_CTRL_L_BASEADDR) + WHEEL_PID_KP_SET_OFFSET, (1 << 8)); //in_kp_param
    Xil_Out32((WHEEL_CTRL_L_BASEADDR) + WHEEL_PID_KI_SET_OFFSET, (1 << 8)); //in_ki_param
    Xil_Out32((WHEEL_CTRL_L_BASEADDR) + WHEEL_PID_OUT_MAX_OFFSET, (63)); //in_pid_out_max
    Xil_Out32((WHEEL_CTRL_L_BASEADDR) + WHEEL_PID_OUT_MIN_OFFSET, (-63)); //in_pid_out_min
    Xil_Out32((WHEEL_CTRL_L_BASEADDR) + WHEEL_PID_UI_LIMIT_OFFSET, (70 << 8)); //in_pid_ui_limit

    Xil_Out32((WHEEL_CTRL_L_BASEADDR) + WHEEL_RESET_OFFSET, (0)); //in_module_reset
    Xil_Out32((WHEEL_CTRL_L_BASEADDR) + WHEEL_RESET_OFFSET, (0x0ff)); //in_module_reset
    //Control reg
    // bit 0 : in_module_en
    // bit 1 : in_fwd_dir
    // *    if in_fwd_dir == 1 out_w_PWMdir = pid_out_sign
    // *    else out_w_PWMdir = ~pid_out_sign
    // bit 2:  in_PID_passby, if == 1, PID Controller bypass
    Xil_Out32((WHEEL_CTRL_L_BASEADDR) + WHEEL_Controller_OFFSET, (0x01));

    //MOTOR Controller IP Initialize
    Xil_Out32((WHEEL_CTRL_R_BASEADDR) + WHEEL_Controller_OFFSET, (0x0));
    //usleep(2000 * 100);
    Xil_Out32((WHEEL_CTRL_R_BASEADDR) + WHEEL_SPEED_SET_OFFSET, (0)); //in_speed_set
    Xil_Out32((WHEEL_CTRL_R_BASEADDR) + WHEEL_PID_KP_SET_OFFSET, (1 << 8)); //in_kp_param
    Xil_Out32((WHEEL_CTRL_R_BASEADDR) + WHEEL_PID_KI_SET_OFFSET, (1 << 8)); //in_ki_param
    Xil_Out32((WHEEL_CTRL_R_BASEADDR) + WHEEL_PID_OUT_MAX_OFFSET, (63)); //in_pid_out_max
    Xil_Out32((WHEEL_CTRL_R_BASEADDR) + WHEEL_PID_OUT_MIN_OFFSET, (-63)); //in_pid_out_min
    Xil_Out32((WHEEL_CTRL_R_BASEADDR) + WHEEL_PID_UI_LIMIT_OFFSET, (70 << 8)); //in_pid_ui_limit

    Xil_Out32((WHEEL_CTRL_R_BASEADDR) + WHEEL_RESET_OFFSET, (0)); //in_module_reset
    Xil_Out32((WHEEL_CTRL_R_BASEADDR) + WHEEL_RESET_OFFSET, (0x0ff)); //in_module_reset
    //Control reg
    // bit 0 : in_module_en
    // bit 1 : in_fwd_dir
    // *    if in_fwd_dir == 1 out_w_PWMdir = pid_out_sign
    // *    else out_w_PWMdir = ~pid_out_sign
    // bit 2:  in_PID_passby, if == 1, PID Controller bypass
    Xil_Out32((WHEEL_CTRL_R_BASEADDR) + WHEEL_Controller_OFFSET, (0x01));
	return XST_SUCCESS;
}

int zrcar_wheel_l_set(int speed)
{
	Xil_Out32((WHEEL_CTRL_L_BASEADDR) + WHEEL_SPEED_SET_OFFSET, speed);
	return XST_SUCCESS;
}

int zrcar_wheel_r_set(int speed)
{
	Xil_Out32((WHEEL_CTRL_R_BASEADDR) + WHEEL_SPEED_SET_OFFSET, speed);
	return XST_SUCCESS;
}

int zrcar_wheel_l_get(int *speed)
{
	*speed = Xil_In32((WHEEL_CTRL_L_BASEADDR) + WHEEL_SPEED_GET_OFFSET);
	return XST_SUCCESS;
}

int zrcar_wheel_r_get(int *speed)
{
	*speed = Xil_In32((WHEEL_CTRL_R_BASEADDR) + WHEEL_SPEED_GET_OFFSET);
	return XST_SUCCESS;
}


int zrcar_opt_init()
{
    int Status;
	Status = XGpio_Initialize(&Opt, XPAR_OPTICAL_DEVICE_ID);
	XGpio_SetDataDirection(&Opt, 1, 0x1f);

	return XST_SUCCESS;
}

int zrcar_opt_get_all(unsigned int *val)
{
	*val = XGpio_DiscreteRead(&Opt, 1);

	return XST_SUCCESS;
}

int zrcar_opt_get_sg(int num, unsigned int *val)
{
	if(num < 1 || num > 5){
		return -1;
	}

	*val = ((XGpio_DiscreteRead(&Opt, 1) >> (num - 1)) & 0x1);
	return XST_SUCCESS;
}

int zrcar_ultra_init()
{
	ULTRASONIC_BASEADDR = XPAR_ULTRASONIC_0_S00_AXI_BASEADDR;
	return XST_SUCCESS;
}

int zrcar_ultra_get_all(float *val)
{
	char cnt;
	Xil_Out32(ULTRASONIC_BASEADDR, 1);

	for(cnt = 1; cnt < 4; cnt ++){
		while(!(Xil_In32(ULTRASONIC_BASEADDR) & (1 << cnt)));
		*val = (float) Xil_In32(ULTRASONIC_BASEADDR + 4 * cnt) * ULTRASONIC_PARAM;
		val ++;
	}

	return XST_SUCCESS;
}

int zrcar_ultra_get_sg(int num, float * val)
{
	Xil_Out32(ULTRASONIC_BASEADDR, 1);

	while(!(Xil_In32(ULTRASONIC_BASEADDR) & (1 << num)));

	*val = (float) Xil_In32(ULTRASONIC_BASEADDR + 4 * num) * ULTRASONIC_PARAM;
	return XST_SUCCESS;
}

int zrcar_bluetooth_init()
{

	int Status;
	XUartPs_Config *Config;

	/*
	 * Initialize the UART driver so that it's ready to use
	 * Look up the configuration in the config table and then initialize it.
	 */
	Config = XUartPs_LookupConfig(XPAR_XUARTPS_0_DEVICE_ID);
	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XUartPs_CfgInitialize(&Uart1_Ps, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XUartPs_SetBaudRate(&Uart1_Ps, 115200);
	return Status;
}

int zrcar_bluetooth_recv(void *buff,int len)
{
	return XUartPs_Recv(&Uart1_Ps,buff,len);//rev command
}

int zrcar_bluetooth_send(void *buff,int len)
{
	return XUartPs_Send(&Uart1_Ps,buff,len);
}

int zrcar_gpio_init()
{
	//JA(8bits,1 channel)and JB(5bits, 2 channel) has been mapped to this gpio.
	//User can just use it without changing the hardware.
	int Status = XGpio_Initialize(&GPIO, XPAR_GPIO_1_DEVICE_ID);
	//just example for how to set direction and how to read and write.
	XGpio_SetDataDirection(&GPIO, 1, 0xff);//set JA direction as input.
	XGpio_SetDataDirection(&GPIO, 2, 0x00);//set JB direction as output.

	int ja_data = XGpio_DiscreteRead(&GPIO, 1);//read from JA.
	XGpio_DiscreteWrite(&GPIO,2,0xff);//write JB 0x01.
	return XST_SUCCESS;
}
