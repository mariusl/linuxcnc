/********************************************************************
* Description:  i2c.c
*               This file, 'i2c.c', is a HAL component used to communicate 
* 				with several generic i2c devices.
*
* Author: Marius Liebenberg
* License: GPL Version 2
*    
* Copyright (c) 2014 All rights reserved.
*
* Last change: 
********************************************************************/
/** This file, 'i2c.c', is a HAL component use to communicate to i2c 
 	devices through general purpose IO pins. It is a realtime component.
*/

/** Copyright (C) 2014 Marius Liebenberg
                       <marius AT mastercut DOT co DOT za>
*/

/** This program is free software; you can redistribute it and/or
    modify it under the terms of version 2 of the GNU General
    Public License as published by the Free Software Foundation.
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111 USA

    THE AUTHORS OF THIS LIBRARY ACCEPT ABSOLUTELY NO LIABILITY FOR
    ANY HARM OR LOSS RESULTING FROM ITS USE.  IT IS _EXTREMELY_ UNWISE
    TO RELY ON SOFTWARE ALONE FOR SAFETY.  Any machinery capable of
    harming persons must have provisions for completely removing power
    from all motors, etc, before persons enter any danger area.  All
    machinery must be designed to comply with local and national safety
    codes, and the authors of this software can not, and do not, take
    any responsibility for such compliance.

    This code was written as part of the EMC HAL project.  For more
    information, go to www.linuxcnc.org.
*/

#include "rtapi.h"		/* RTAPI realtime OS API */
#include "rtapi_app.h"		/* RTAPI realtime module decls */
#include "hal.h"		/* HAL public API decls */

/* module information */
MODULE_AUTHOR("Marius Liebenberg");
MODULE_DESCRIPTION("I2C Component for Linuxcnc HAL");
MODULE_LICENSE("GPL");
static int num_chan = 1;	/* number of channels - default = 1 */
RTAPI_MP_INT(num_chan, "number of channels");

/***********************************************************************
*                STRUCTURES AND GLOBAL VARIABLES                       *
************************************************************************/

/** This structure contains the runtime data.
*/

typedef struct {
	hal_bit_t *SDA;		/* pin: serial data */
	hal_bit_t *SDI;		/* pin: serial data in */
	hal_bit_t *SCL;		/* pin: serial clock */
	
	hal_u32_t *start_bit_delay;
	hal_u32_t delay_count;
	hal_bit_t *tb;
 } hal_i2c_t;

typedef struct {
	hal_u32_t reg;
	hal_u32_t data;
}i2c_reg_t;

/* pointer to i2c_t struct */
static hal_i2c_t *i2c_array;

/* other globals */
static int comp_id;		/* component ID */
//static int tb;
//static int tc;
/***********************************************************************
*                  LOCAL FUNCTION DECLARATIONS                         *
************************************************************************/

static int export_i2c(int num, hal_i2c_t * addr);
static void run_i2c(void *arg, long l);
void i2c_start(void *arg);
void i2c_stop(void *arg);
/***********************************************************************
*                       INIT AND EXIT CODE                             *
************************************************************************/
#define MAX_CHAN 16

int rtapi_app_main(void)
{
    int n, retval;

    /* test for number of channels */
    if ((num_chan <= 0) || (num_chan > MAX_CHAN)) {
	rtapi_print_msg(RTAPI_MSG_ERR,
	    "I2C: ERROR: invalid num_chan: %d\n", num_chan);
	return -1;
    }
    /* have good config info, connect to the HAL */
    comp_id = hal_init("i2c");
    if (comp_id < 0) {
	rtapi_print_msg(RTAPI_MSG_ERR, "I2C: ERROR: hal_init() failed\n");
	return -1;
    }
    /* allocate shared memory for i2c data */
    i2c_array = hal_malloc(num_chan * sizeof(hal_i2c_t));
    if (i2c_array == 0) {
	rtapi_print_msg(RTAPI_MSG_ERR,
	    "I2C: ERROR: hal_malloc() failed\n");
	hal_exit(comp_id);
	return -1;
    }
    /* export variables and functions for each driver */
    for (n = 0; n < num_chan; n++) {
	retval = export_i2c(n, &(i2c_array[n]));
	if (retval != 0) {
	    rtapi_print_msg(RTAPI_MSG_ERR,
		"I2C: ERROR: var export failed\n");
	    hal_exit(comp_id);
	    return -1;
	}
    }
    rtapi_print_msg(RTAPI_MSG_INFO, "I2C:installed %d drivers\n",
	num_chan);
    hal_ready(comp_id);
    return 0;
}

void rtapi_app_exit(void)
{
    hal_exit(comp_id);
}

/***********************************************************************
*                       REALTIME FUNCTIONS                             *
************************************************************************/

//....................................................................
// Outputs a bit to the I2C bus
//....................................................................
void bit_out(void *arg, unsigned char data)
{
unsigned int i;
	hal_i2c_t *i2c;
    /* point to the data */
    i2c = arg;


*(i2c->SCL) = 0;                  // ensure SCL is low
//SDA_TRIS=0;                     // configure SDA as an output
*(i2c->SDA) = (data>>7);                 // output the MSB
//for (i=0;i<2;i++) NOP();
*(i2c->SCL) = 1;                        // pull SCL high to clock bit
//for (i=0;i<3;i++) NOP();
*(i2c->SCL) = 0;                        // pull SCL low for next bit
}


//....................................................................
// Inputs a bit from the I2C bus
//....................................................................
void bit_in(void *arg, unsigned char *data)
{
unsigned int i;
	hal_i2c_t *i2c;
    /* point to the data */
    i2c = arg;


//SCL = 0;                        // ensure SCL is low
//SCL = 1;                        // bring SCL high to begin transfer
//for (i=0;i<3;i++) NOP();
//*data |= SDA;                   // input the received bit
//SCL = 0;                        // bring SCL low again.
}





//....................................................................
// This function generates an I2C Start Condition
//....................................................................
void i2c_start(void *arg)
{
	hal_i2c_t *i2c;
    /* point to the data */
    i2c = arg;

	*(i2c->SDA) = 0;  					// make the DATA line low 

	if(i2c->delay_count == 0 ){
		i2c->delay_count = 1;//*(i2c->start_bit_delay);
		*(i2c->SCL) = 0;  				// make the CLOCK low after a delay 
	}else {
		i2c->delay_count -= 1;
	}
	
}

//....................................................................
// This function generates an I2C Stop Condition
//....................................................................
void i2c_stop(void *arg)
{
	hal_i2c_t *i2c;
    /* point to the data */
    i2c = arg;

	*(i2c->SCL) = 1; 					// pull the CLOCK high
	*(i2c->SDA) = 0;					// ensure DATA OUT is low
	
	if(i2c->delay_count == 0 ){
		i2c->delay_count = *(i2c->start_bit_delay);
		*(i2c->SDA) = 1;				// make DATA OUT high
	}else {
		i2c->delay_count -= 1;
	}	
}

//....................................................................
// Writes a byte to the I2C bus
//....................................................................
unsigned char i2c_wr(void *arg, unsigned char data)
{
unsigned char i;                // loop counter
unsigned char ack;              // ACK bit
	hal_i2c_t *i2c;
    /* point to the data */
    i2c = arg;

//ack = 0;
for (i = 0; i < 8; i++)         // loop through each bit
    {
    bit_out(i2c,data);              // output bit
	
    data = data << 1;           // shift left for next bit
    }

//bit_in(&ack);                   // input ACK bit
return ack;
}


//....................................................................
// Reads a byte from the I2C bus
//....................................................................
unsigned char i2c_rd(void *arg, unsigned char ack)
{
unsigned char i;                // loop counter
unsigned char ret=0;            // return value
	hal_i2c_t *i2c;
    /* point to the data */
    i2c = arg;

//for (i = 0; i < 8; i++)         // loop through each bit
//    {
//    ret = ret << 1;             // shift left for next bit
//    bit_in(&ret);               // input bit
//    }

//bit_out(ack);                   // output ACK/NAK bit
return ret;
}


//.............................................................................
//          Polls the bus for ACK from device
//.............................................................................
void ack_poll (void *arg, unsigned char control)
{
unsigned char result=1;
	hal_i2c_t *i2c;
    /* point to the data */
    i2c = arg;

//	while(result){
//		i2c_start();            // generate Restart condition
//		result=i2c_wr(control); // send control byte (WRITE command)
 //   }

//	i2c_stop();                     // generate Stop condition
}


static void run_i2c(void *arg, long l)
{
    hal_i2c_t *i2c;
	i2c = arg;
    /* point to the data */
    i2c = arg;

	i2c_start(i2c);
	// transmit
	i2c_wr(i2c, 0xaa);
	i2c_stop(i2c);
    /* done */

	// message state machine

	
	

}

/***********************************************************************
*                   LOCAL FUNCTION DEFINITIONS                         *
************************************************************************/

static int export_i2c(int num, hal_i2c_t * addr)
{
    int retval;
    char buf[HAL_NAME_LEN + 1];

    /* export pins */
	retval = hal_pin_bit_newf(HAL_IO, &(addr->SDA), comp_id, "i2c.%d.SDA", num);
    if (retval != 0) {
	return retval;
    }
	//retval = hal_pin_bit_newf(HAL_IN, &(addr->SDI), comp_id, "i2c.%d.SDI", num);
    //if (retval != 0) {
	//return retval;
    //}
	retval = hal_pin_bit_newf(HAL_OUT, &(addr->SCL), comp_id, "i2c.%d.SCL", num);
    if (retval != 0) {
	return retval;
    }    

/*
	retval = hal_pin_bit_newf(HAL_OUT, &(addr->q), comp_id, "i2c.%d.q", num);
    if (retval != 0) {
	return retval;
    }
    retval = hal_pin_bit_newf(HAL_OUT, &(addr->_q), comp_id, "i2c.%d._q", num);
    if (retval != 0) {
	return retval;
    }
    retval = hal_pin_float_newf(HAL_OUT, &(addr->variable), comp_id,"i2c.%d.variable", num);
    if (retval != 0) {
	return retval;
    }
    retval = hal_pin_float_newf(HAL_OUT, &(addr->_variable), comp_id, "i2c.%d._variable", num);
    if (retval != 0) {
	return retval;
    }
*/

    /* export parameters */

    retval = hal_pin_u32_newf(HAL_IO, &(addr->start_bit_delay), comp_id, "i2c.%d.start_bit_delay", num);
    if (retval != 0) {
	return retval;
    }
/*
    retval = hal_pin_float_newf(HAL_IO, &(addr->value), comp_id, "i2c.%d.value", num);
    if (retval != 0) {
	return retval;
    }
*/
    /* init all structure members */
	*(addr->SDA) = 1;
	//*(addr->SDI) = 0;
	*(addr->SCL) = 1;
	*(addr->start_bit_delay) = 1;
	//*(addr->tb) = 0;

	(addr->delay_count) =  *(addr->start_bit_delay);

    /* export function for this loop */
    rtapi_snprintf(buf, sizeof(buf), "i2c.%d.update", num);
    retval = hal_export_funct(buf, run_i2c, &(i2c_array[num]), 1, 0, comp_id);
    if (retval != 0) {
	rtapi_print_msg(RTAPI_MSG_ERR,
	    "I2C: ERROR: update funct export failed\n");
	hal_exit(comp_id);
	return -1;
    }
    return 0;
}
