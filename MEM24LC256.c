///*
// * MEM24LC256.c
// *	Source file of the 24LC256
// *  Created on: 07/11/2017
// *      Author: J. Alejandro Ávila & Daniel Barragán
// */
//
//#include "MEM24LC256.h"
//#include "fsl_i2c_freertos.h"
//
//void MEM24LC256_setData(uint16 address, uint8* data)
//{
//	uint8 counter = 0; // Variable used to move inside the array
//	while(data[counter] != '\0') // Until we finish reading the array
//	{
//		uint8 address_H = 0x00; // Variables created to separate the address into high and low
//		uint8 address_L = 0x00;
//		uint8 data_mask = 0xFF; // Mask to keep only the lower bytes of address
//
//		address_H = address >> 8; // We keep only the high part of the address by shifting the bytes.
//		address_L = (address & data_mask); // Doing an and of the address and mask to keep the lower bits.
//
//		I2C_start(); //configures the I2C in transmitting mode and generates the start signal
//
//		/**Address with the w/r in W 1010 0000 */
//		I2C_write_Byte(MEM24LC256_WRITE_ADDRESS);// Writes slave address with the write bit
//		I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//		if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//		{
//			I2C_write_Byte(address_H);// Writing MEM address high
//			I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//			if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//			{
//				I2C_write_Byte(address_L);// Writing MEM address low
//				I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//				if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//				{
//
//					I2C_write_Byte(data[counter]);// Writing data
//					I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//					I2C_stop(); // Generating stop signal
//					counter++; // Augments the address and the counter of the array
//					address++;
//				}else I2C_stop(); // Generating stop signal
//			}else I2C_stop(); // Generating stop signal
//		}else I2C_stop(); // Generating stop signal
//	}
//}
//
//uint8* MEM24LC256_getData(uint16 address, uint8 bytes, uint8* data)
//{
//	uint8 counter = 0; // Variable used to move inside the array
//	uint8 i; // for variable
//	for(i = counter; i < bytes ; i++) //until we finish reading the full array
//	{
//		uint8 address_H = 0x00; // Variables created to separate the address into high and low
//		uint8 address_L = 0x00;
//		uint8 data_mask = 0xFF; // Mask to keep only the lower bytes of address
//
//		address_H = address >> 8; // We keep only the high part of the address by shifting the bytes.
//		address_L = (address & data_mask); // Doing an and of the address and mask to keep the lower bits.
//		I2C_start(); //configures the I2C in transmitting mode and generates the start signal
//		I2C_write_Byte(MEM24LC256_WRITE_ADDRESS); // Writes slave address with the write bit
//		I2C_wait(); // Checking if the I2C module is busy
//		if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//		{
//			I2C_write_Byte(address_H); // Writing MEM address high
//			I2C_wait(); // Checking if the I2C module is busy
//			if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//			{
//				I2C_write_Byte(address_L); // Writing MEM address low
//				I2C_wait(); // Checking if the I2C module is busy
//				if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//				{
//					I2C_repeted_Start(); // Generates a new start
//					I2C_write_Byte(MEM24LC256_READ_ADDRESS);// Writes slave address and the read bit is on to read the previous register
//					I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//					if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//					{
//						I2C_TX_RX_Mode(I2C_RX); // changes the I2C module to receiver module
//						data[counter] = I2C_read_Byte(); // Dummy read
//						I2C_wait(); // Checking if the I2C module is busy
//						I2C_NACK(); // Generating not acknowledge
//						I2C_stop(); // Generating stop signal
//						data[counter] = I2C_read_Byte(); // Return data
//						counter++; // Augments the address and the counter of the array
//						address++;
//					}else I2C_stop(); // Generating stop signal
//				}else I2C_stop(); // Generating stop signal
//			}else I2C_stop(); // Generating stop signal
//		}else I2C_stop(); // Generating stop signal
//	}
//	return data;
//}
