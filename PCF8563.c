///*
// * PCF8583.c
// *
// *  Created on: 03/11/2017
// *      Author: ALEX
// */
//
//#include <PCF8563.h>
//#include "DataTypeDefinitions.h"
//#include "I2C.h"
//#include "GlobalFunctions.h"
//
//void PCF8563_setSeconds(uint8 data)
//{
//	I2C_start(); //configures the I2C in transmitting mode and generates the start signal
//
//	/**Address with the w/r in W 1010 0000 */
//	I2C_write_Byte(PCF8563_WRITE_ADDRESS);// Writes slave address with the write bit
//	I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//	if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//	{
//		I2C_write_Byte(PCF8563_SECONDS_ADDRESS);// Writing RTC address in the data register (seconds)
//		I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//		if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//		{
//			I2C_write_Byte(data);// Writing data
//			I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//			if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//			{
//				I2C_stop(); // Generating stop signal
//			}else I2C_stop(); // Generating stop signal
//		}else I2C_stop(); // Generating stop signal
//	}else I2C_stop(); // Generating stop signal
//}
//
//uint8 PCF8563_getSeconds()
//{
//	uint8 dataFromPCF8563;
//	I2C_start(); //configures the I2C in transmitting mode and generates the start signal
//	I2C_write_Byte(PCF8563_WRITE_ADDRESS); // Writes slave address with the write bit
//	I2C_wait(); // Checking if the I2C module is busy
//	if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//	{
//		I2C_write_Byte(PCF8563_SECONDS_ADDRESS); //  Writing RTC address in the data register (seconds)
//		I2C_wait(); // Checking if the I2C module is busy
//		if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//		{
//			I2C_repeted_Start(); // Generates a new start
//			I2C_write_Byte(PCF8563_READ_ADDRESS);// Writes slave address and the read bit is on to read the previous register
//			I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//			if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//			{
//				I2C_TX_RX_Mode(I2C_RX); // changes the I2C module to receiver module
//				dataFromPCF8563 = I2C_read_Byte(); // Dummy read
//				I2C_wait(); // Checking if the I2C module is busy
//				I2C_NACK(); // Generating not acknowledge
//				I2C_stop(); // Generating stop signal
//				dataFromPCF8563 = I2C_read_Byte(); // Returns the data the I2C is reading
//			}else I2C_stop(); // Generating stop signal
//		}else I2C_stop(); // Generating stop signal
//	}else I2C_stop(); // Generating stop signal
//	return(dataFromPCF8563);
//}
//
//void PCF8563_setMinutes(uint8 data)
//{
//	I2C_start(); //configures the I2C in transmitting mode and generates the start signal
//
//	/**Address with the w/r in W 1010 0000 */
//	I2C_write_Byte(PCF8563_WRITE_ADDRESS);// Writes slave address with the write bit
//	I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//	if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//	{
//		I2C_write_Byte(PCF8563_MINUTES_ADDRESS);// Writing RTC address in the data register (minutes)
//		I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//		if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//		{
//			I2C_write_Byte(data);// Writing data
//			I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//			if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//			{
//				I2C_stop(); // Generating stop signal
//			}else I2C_stop(); // Generating stop signal
//		}else I2C_stop(); // Generating stop signal
//	}else I2C_stop(); // Generating stop signal
//}
//
//uint8 PCF8563_getMinutes()
//{
//	uint8 dataFromPCF8563;
//	I2C_start(); //configures the I2C in transmitting mode and generates the start signal
//	I2C_write_Byte(PCF8563_WRITE_ADDRESS); // Writes slave address with the write bit
//	I2C_wait(); // Checking if the I2C module is busy
//	if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//	{
//		I2C_write_Byte(PCF8563_MINUTES_ADDRESS); //  Writing RTC address in the data register (minutes)
//		I2C_wait(); // Checking if the I2C module is busy
//		if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//		{
//			I2C_repeted_Start(); // Generates a new start
//			I2C_write_Byte(PCF8563_READ_ADDRESS);// Writes slave address and the read bit is on to read the previous register
//			I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//			if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//			{
//				I2C_TX_RX_Mode(I2C_RX); // changes the I2C module to receiver module
//				dataFromPCF8563 = I2C_read_Byte(); // Dummy read
//				I2C_wait(); // Checking if the I2C module is busy
//				I2C_NACK(); // Generating not acknowledge
//				I2C_stop(); // Generating stop signal
//				dataFromPCF8563 = I2C_read_Byte(); // Returns the data the I2C is reading
//			}else I2C_stop(); // Generating stop signal
//		}else I2C_stop(); // Generating stop signal
//	}else I2C_stop(); // Generating stop signal
//	return(dataFromPCF8563);
//}
//
//void PCF8563_setHours(uint8 data)
//{
//	I2C_start(); //configures the I2C in transmitting mode and generates the start signal
//
//	/**Address with the w/r in W 1010 0000 */
//	I2C_write_Byte(PCF8563_WRITE_ADDRESS);// Writes slave address with the write bit
//	I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//	if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//	{
//		I2C_write_Byte(PCF8563_HOURS_ADDRESS);// Writing RTC address in the data register (hours)
//		I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//		if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//		{
//			I2C_write_Byte(data);// Writing data
//			I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//			if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//			{
//				I2C_stop(); // Generating stop signal
//			}else I2C_stop(); // Generating stop signal
//		}else I2C_stop(); // Generating stop signal
//	}else I2C_stop(); // Generating stop signal
//}
//
//uint8 PCF8563_getHours()
//{
//	uint8 dataFromPCF8563;
//	uint8 data_mask = 0x3F;
//	I2C_start(); //configures the I2C in transmitting mode and generates the start signal
//	I2C_write_Byte(PCF8563_WRITE_ADDRESS); // Writes slave address with the write bit
//	I2C_wait(); // Checking if the I2C module is busy
//	if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//	{
//		I2C_write_Byte(PCF8563_HOURS_ADDRESS); //  Writing RTC address in the data register (hours)
//		I2C_wait(); // Checking if the I2C module is busy
//		if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//		{
//			I2C_repeted_Start(); // Generates a new start
//			I2C_write_Byte(PCF8563_READ_ADDRESS);// Writes slave address and the read bit is on to read the previous register
//			I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//			if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//			{
//				I2C_TX_RX_Mode(I2C_RX); // changes the I2C module to receiver module
//				dataFromPCF8563 = I2C_read_Byte(); // Dummy read
//				I2C_wait(); // Checking if the I2C module is busy
//				I2C_NACK(); // Generating not acknowledge
//				I2C_stop(); // Generating stop signal
//				dataFromPCF8563 = I2C_read_Byte(); // Returns the data the I2C is reading
//				dataFromPCF8563 &= data_mask;
//			}else I2C_stop(); // Generating stop signal
//		}else I2C_stop(); // Generating stop signal
//	}else I2C_stop(); // Generating stop signal
//	return(dataFromPCF8563);
//}
//
//void PCF8563_setYears(uint8 data)
//{
//	I2C_start(); //configures the I2C in transmitting mode and generates the start signal
//
//	/**Address with the w/r in W 1010 0000 */
//	I2C_write_Byte(PCF8563_WRITE_ADDRESS);// Writes slave address with the write bit
//	I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//	if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//	{
//		I2C_write_Byte(PCF8563_YEARS_ADDRESS);// Writing RTC address in the data register (years)
//		I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//		if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//		{
//			I2C_write_Byte(data);// Writing data
//			I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//			if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//			{
//				I2C_stop(); // Generating stop signal
//			}else I2C_stop(); // Generating stop signal
//		}else I2C_stop(); // Generating stop signal
//	}else I2C_stop(); // Generating stop signal
//}
//
//uint8 PCF8563_getYears()
//{
//	uint8 dataFromPCF8563;
//	I2C_start(); //configures the I2C in transmitting mode and generates the start signal
//	I2C_write_Byte(PCF8563_WRITE_ADDRESS); // Writes slave address with the write bit
//	I2C_wait(); // Checking if the I2C module is busy
//	if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//	{
//		I2C_write_Byte(PCF8563_YEARS_ADDRESS); //  Writing RTC address in the data register (years)
//		I2C_wait(); // Checking if the I2C module is busy
//		if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//		{
//			I2C_repeted_Start(); // Generates a new start
//			I2C_write_Byte(PCF8563_READ_ADDRESS);// Writes slave address and the read bit is on to read the previous register
//			I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//			if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//			{
//				I2C_TX_RX_Mode(I2C_RX); // changes the I2C module to receiver module
//				dataFromPCF8563 = I2C_read_Byte(); // Dummy read
//				I2C_wait(); // Checking if the I2C module is busy
//				I2C_NACK(); // Generating not acknowledge
//				I2C_stop(); // Generating stop signal
//				dataFromPCF8563 = I2C_read_Byte(); // Returns the data the I2C is reading
//			}else I2C_stop(); // Generating stop signal
//		}else I2C_stop(); // Generating stop signal
//	}else I2C_stop(); // Generating stop signal
//	return(dataFromPCF8563);
//}
//
//void PCF8563_setMonths(uint8 data)
//{
//	I2C_start(); //configures the I2C in transmitting mode and generates the start signal
//
//	/**Address with the w/r in W 1010 0000 */
//	I2C_write_Byte(PCF8563_WRITE_ADDRESS);// Writes slave address with the write bit
//	I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//	if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//	{
//		I2C_write_Byte(PCF8563_MONTHS_ADDRESS);// Writing RTC address in the data register (months)
//		I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//		if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//		{
//			I2C_write_Byte(data);// Writing data
//			I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//			if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//			{
//				I2C_stop(); // Generating stop signal
//			}else I2C_stop(); // Generating stop signal
//		}else I2C_stop(); // Generating stop signal
//	}else I2C_stop(); // Generating stop signal
//}
//
//uint8 PCF8563_getMonths()
//{
//	uint8 dataFromPCF8563;
//	uint8 data_mask = 0x1F;
//	I2C_start(); //configures the I2C in transmitting mode and generates the start signal
//	I2C_write_Byte(PCF8563_WRITE_ADDRESS); // Writes slave address with the write bit
//	I2C_wait(); // Checking if the I2C module is busy
//	if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//	{
//		I2C_write_Byte(PCF8563_MONTHS_ADDRESS); //  Writing RTC address in the data register (months)
//		I2C_wait(); // Checking if the I2C module is busy
//		if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//		{
//			I2C_repeted_Start(); // Generates a new start
//			I2C_write_Byte(PCF8563_READ_ADDRESS);// Writes slave address and the read bit is on to read the previous register
//			I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//			if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//			{
//				I2C_TX_RX_Mode(I2C_RX); // changes the I2C module to receiver module
//				dataFromPCF8563 = I2C_read_Byte(); // Dummy read
//				I2C_wait(); // Checking if the I2C module is busy
//				I2C_NACK(); // Generating not acknowledge
//				I2C_stop(); // Generating stop signal
//				dataFromPCF8563 = I2C_read_Byte(); // Returns the data the I2C is reading
//				dataFromPCF8563 &= data_mask;
//			}else I2C_stop(); // Generating stop signal
//		}else I2C_stop(); // Generating stop signal
//	}else I2C_stop(); // Generating stop signal
//	return(dataFromPCF8563);
//}
//
//void PCF8563_setDays(uint8 data)
//{
//	I2C_start(); //configures the I2C in transmitting mode and generates the start signal
//
//	/**Address with the w/r in W 1010 0000 */
//	I2C_write_Byte(PCF8563_WRITE_ADDRESS);// Writes slave address with the write bit
//	I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//	if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//	{
//		I2C_write_Byte(PCF8563_DAYS_ADDRESS);// Writing RTC address in the data register (days)
//		I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//		if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//		{
//			I2C_write_Byte(data);// Writing data
//			I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//			if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//			{
//				I2C_stop(); // Generating stop signal
//			}else I2C_stop(); // Generating stop signal
//		}else I2C_stop(); // Generating stop signal
//	}else I2C_stop(); // Generating stop signal
//}
//
//uint8 PCF8563_getDays()
//{
//	uint8 dataFromPCF8563;
//	uint8 data_mask = 0x3F;
//	I2C_start(); //configures the I2C in transmitting mode and generates the start signal
//	I2C_write_Byte(PCF8563_WRITE_ADDRESS); // Writes slave address with the write bit
//	I2C_wait(); // Checking if the I2C module is busy
//	if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//	{
//		I2C_write_Byte(PCF8563_DAYS_ADDRESS); //  Writing RTC address in the data register (days)
//		I2C_wait(); // Checking if the I2C module is busy
//		if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//		{
//			I2C_repeted_Start(); // Generates a new start
//			I2C_write_Byte(PCF8563_READ_ADDRESS);// Writes slave address and the read bit is on to read the previous register
//			I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//			if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//			{
//				I2C_TX_RX_Mode(I2C_RX); // changes the I2C module to receiver module
//				dataFromPCF8563 = I2C_read_Byte(); // Dummy read
//				I2C_wait(); // Checking if the I2C module is busy
//				I2C_NACK(); // Generating not acknowledge
//				I2C_stop(); // Generating stop signal
//				dataFromPCF8563 = I2C_read_Byte(); // Returns the data the I2C is reading
//				dataFromPCF8563 &= data_mask;
//			}else I2C_stop(); // Generating stop signal
//		}else I2C_stop(); // Generating stop signal
//	}else I2C_stop(); // Generating stop signal
//	return(dataFromPCF8563);
//}
//
//void PCF8583_setData(uint8 address, uint8 data)
//{
//	I2C_start(); //configures the I2C in transmitting mode and generates the start signal
//
//	/**Address with the w/r in W 1010 0000 */
//	I2C_write_Byte(0xA0);// Writes slave address with the write bit
//	I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//	if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//	{
//		I2C_write_Byte(address);// Writing RTC address in the data register (minutes)
//		I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//		if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//		{
//			I2C_write_Byte(data);// Writing data
//			I2C_wait(); // Checking if the I2C module is busy by checking the IICIF bit
//			if(0 == I2C_get_ACK()) // By checking the RXAK bit returns a 0 if there was an ack received.
//			{
//				I2C_stop(); // Generating stop signal
//			}else I2C_stop(); // Generating stop signal
//		}else I2C_stop(); // Generating stop signal
//	}else I2C_stop(); // Generating stop signal
//}
