/*
 * zrcar.h
 *
 *  Created on: 2014-12-31
 *      Author: ZRobot
 */

#ifndef ZRCAR_H_
#define ZRCAR_H_
#include "xil_io.h"
#include "xil_types.h"
#include "xstatus.h"
#include "xgpio.h"
#include "xuartps.h"
#include "xparameters.h"

/**
 *
 * Write a value to a servo register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the servo device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void servo_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define servo_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
/****************************************************************************/
/**
* Init servo 'reg.
*
* @return	return 0 if succeed
*
*****************************************************************************/
u32 SERVO_BASEADDR;
#define SERVO0_OFFSET 0
#define SERVO1_OFFSET 4
int zrcar_servo_init();
/****************************************************************************/
/**
* Set the width of servo
*
* @param	angle of servo that you want,rang from 0 to 180.
*
* @return	return 0 if succeed
*
*****************************************************************************/
int zrcar_servo0_set(int angle);
int zrcar_servo1_set(int angle);

/*************************WHEEL base address********************************/
u32 WHEEL_CTRL_L_BASEADDR;
u32 WHEEL_CTRL_R_BASEADDR;

/*************************ULTRASONIC base address********************************/
u32 ULTRASONIC_BASEADDR;

/*************************WHEEL OFFSET address************************************/
#define WHEEL_Controller_OFFSET 		(0 * 4)
#define WHEEL_SPEED_SET_OFFSET 			(1 * 4)
#define WHEEL_PID_KP_SET_OFFSET 		(2 * 4)
#define WHEEL_PID_KI_SET_OFFSET 		(3 * 4)
#define WHEEL_PID_OUT_MAX_OFFSET 		(4 * 4)
#define WHEEL_PID_OUT_MIN_OFFSET 		(5 * 4)
#define WHEEL_PID_UI_LIMIT_OFFSET 		(6 * 4)
#define WHEEL_RESET_OFFSET 				(14 * 4)
#define WHEEL_SPEED_GET_OFFSET		 	(15 * 4)


/************************* ULTRASONIC_PARAM************************************/
#define ULTRASONIC_PARAM 0.0017


/****************************************************************************/
/**
* Init wheels'reg.
*
* @return	return 0 if succeed
*
*****************************************************************************/
int zrcar_wheel_init();

/****************************************************************************/
/**
* Set the speed of left/right wheel
*
* @param	speed of left/right wheel that you want,rang from -45 to 45.
*
* @return	return 0 if succeed
*
*****************************************************************************/
int zrcar_wheel_l_set(int speed);
int zrcar_wheel_r_set(int speed);


/****************************************************************************/
/**
* Get the speed of left/right wheel
*
* @param	pointor which is used to store the actual speed of left/right wheel
*
* @return	return 0 if succeed
*
*****************************************************************************/
int zrcar_wheel_l_get(int *speed);
int zrcar_wheel_r_get(int *speed);

/****************************************************************************/
/**
* Init optical gpio device
*
** @return	return 0 if succeed
*
*****************************************************************************/
int zrcar_opt_init();

/****************************************************************************/
/**
* Get value of optical module(all)
*
* @param	pointor values store here(5 bit,set from low bit to high bit)
*
* @return	return 0 if succeed
*
*****************************************************************************/
int zrcar_opt_get_all(unsigned int *val);

/****************************************************************************/
/**
* Get value of optical module(single)
*
* @param	num   optical's index
* @param    pointor  value store here
*
* @return	return 0 if succeed
*
*****************************************************************************/
int zrcar_opt_get_sg(int num, unsigned int *val);

/****************************************************************************/
/**
* Init ultrasonic'base address.
*
** @return	return 0 if succeed
*
*****************************************************************************/
int zrcar_ultra_init();

/****************************************************************************/
/**
* Get value of  ultrasonic module(all)
*
* @param	pointor array of integer,store 3 channel ultrasonic values
*
* @return	return 0 if succeed
*
*****************************************************************************/
int zrcar_ultra_get_all(float *val);

/****************************************************************************/
/**
* Get value of  ultrasonic module(single)
*
* @param	num   ultrasonic's index
* @param    pointor  value store here
*
* @return	return 0 if succeed
*
*****************************************************************************/
int zrcar_ultra_get_sg(int num, float * val);

/****************************************************************************/
/**
* For bluetooth. initialize the uart0
* @return	return 0 if succeed
*
*****************************************************************************/
int zrcar_bluetooth_init();

/****************************************************************************/
/**
* This function attempts to receive a specified number of bytes of data
* from the device and store it into the specified buffer. This function works
* for both polled or interrupt driven modes. It is non-blocking.

* @param	buff is pointer to buffer for data to be received into
* @param	len is the number of bytes to be received. A value of zero
*		will stop a previous receive operation that is in progress in
*		interrupt mode.
*
* @return	The number of bytes received.
*
*****************************************************************************/
int zrcar_bluetooth_recv(void *buff,int len);

/****************************************************************************/
/**
*
* This functions sends the specified buffer using the device in either
* polled or interrupt driven mode. This function is non-blocking, if the device
* is busy sending data, it will return and indicate zero bytes were sent.
* Otherwise, it fills the TX FIFO as much as it can, and return the number of
* bytes sent.
*
* @param	BufferPtr is pointer to a buffer of data to be sent.
* @param  	NumBytes contains the number of bytes to be sent. A value of
*		zero will stop a previous send operation that is in progress
*		in interrupt mode. Any data that was already put into the
*		transmit FIFO will be sent.
*
* @return	The number of bytes actually sent.
*****************************************************************************/
int zrcar_bluetooth_send(void *buff,int len);

int zrcar_gpio_init();

#endif /* ZRCAR_H_ */
