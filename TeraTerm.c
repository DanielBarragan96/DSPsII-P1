//#include "MK64F12.h"
//#include "TeraTerm.h"
//#include "PCF8563.h"
//#include "FIFO.h"
//#include "States.h"
//#include "MEM24LC256.h"
//
//int8_t asciiVal[] = {0,0};
//int8_t memArray[FIFO_LENGHT];
//
//uint8_t printTTMainMenu(){
//
//	/**The following sentences send strings to PC using the UART_putString function. Also, the string
//	 * is coded with terminal code*/
//	/** VT100 command for text in red and background in cyan*/
//	UART_putString(UART0,"\033[0;32;10m");
//	/*VT100 command for clearing the screen*/
//	UART_putString(UART0,"\033[2J");
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[3;10H");
//	UART_putString(UART0, "1) Leer Memoria I2C\r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[4;10H");
//	UART_putString(UART0, "2) Escribir memoria I2C\r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[5;10H");
//	UART_putString(UART0, "3) Establecer Hora\r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[6;10H");
//	UART_putString(UART0, "4) Establecer Fecha\r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[7;10H");
//	UART_putString(UART0, "5) Formato de Hora\r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[8;10H");
//	UART_putString(UART0, "6) Leer hora\r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[9;10H");
//	UART_putString(UART0, "7) Leer fecha\r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[10;10H");
//	UART_putString(UART0, "8) Comunicaciones con terminal 2\r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[11;10H");
//	UART_putString(UART0, "9) Eco en LCD\r");
//
//	/** VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[13;10H");
//
//	return GOOD;
//}
//
//uint8_t printTTMenuRMemI2C_1(){
//	/**The following sentences send strings to PC using the UART_putString function. Also, the string
//	 * is coded with terminal code*/
//	/** VT100 command for text in red and background in cyan*/
//	UART_putString(UART0,"\033[0;32;10m");
//	/*VT100 command for clearing the screen*/
//	UART_putString(UART0,"\033[2J");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[3;10H");
//	UART_putString(UART0, "Direccion de lectura: \r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[4;10H");
//
//
//	return GOOD;
//}
//uint8_t printTTMenuRMemI2C_2(){
//	if(!setSystemAddress()){
//		/* VT100 command for positioning the cursor in x and y position*/
//		UART_putString(UART0,"\033[5;10H");
//		UART_putString(UART0, "Direccion erronea. \r");
//		pauseExcecution();
//		return ERROR;
//	}
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[5;10H");
//	UART_putString(UART0, "Longitud de bytes: \r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[6;10H");
//
//
//	return GOOD;
//}
//uint8_t printTTMenuRMemI2C_3(){
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[7;10H");
//	UART_putString(UART0, "Contenido: \r");
//
//	// readMem
//	uint8_t readArray[FIFO_LENGHT];
//	//int8_t index = getFIFOIndex();
//	int8_t indexMem = dataSetInt();
//	MEM24LC256_getData(getSystem()->address,indexMem, readArray);
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[8;10H");
//	while(FIFO_LENGHT>indexMem){
//		readArray[indexMem++] = (int8_t) FIRST_ELEMENT;
//	}
//
//	UART_putString(UART0, (int8_t*) readArray);
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[12;10H");
//	UART_putString(UART0, "Presiona una tecla para continuar ... ...\r");
//
//	return GOOD;
//}
//
//uint8_t printTTMenuWMemI2C_1(){
//	/**The following sentences send strings to PC using the UART_putString function. Also, the string
//	 * is coded with terminal code*/
//	/** VT100 command for text in red and background in cyan*/
//	UART_putString(UART0,"\033[0;32;10m");
//	/*VT100 command for clearing the screen*/
//	UART_putString(UART0,"\033[2J");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[3;10H");
//	UART_putString(UART0, "Direccion de escritura: \r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[4;10H");
//
//	return GOOD;
//}
//uint8_t printTTMenuWMemI2C_2(){
//	if(!setSystemAddress()){
//		/* VT100 command for positioning the cursor in x and y position*/
//		UART_putString(UART0,"\033[5;10H");
//		UART_putString(UART0, "Direccion erronea. \r");
//		pauseExcecution();
//		return ERROR;
//	}
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[5;10H");
//	UART_putString(UART0, "Texto a guardar: \r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[6;10H");
//
//	return GOOD;
//}
//uint8_t printTTMenuWMemI2C_3(){
//
//	// writeMem
//	uint8_t* fifoArray = (uint8_t*) getFIFO();
//	MEM24LC256_setData( getSystem()->address, sizeof(fifoArray), fifoArray);
//
//	UART_putString(UART0,"\033[7;10H");
//	UART_putString(UART0, "Su texto ha sido guardado... \r");
//
//	return GOOD;
//}
//
//uint8_t printTTMenuSetH_1(){
//	/**The following sentences send strings to PC using the UART_putString function. Also, the string
//	 * is coded with terminal code*/
//	/** VT100 command for text in red and background in cyan*/
//	UART_putString(UART0,"\033[0;32;10m");
//	/*VT100 command for clearing the screen*/
//	UART_putString(UART0,"\033[2J");
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[3;10H");
//	UART_putString(UART0, "Escribir hora en: \r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[4;10H");
//
//	return GOOD;
//}
//uint8_t printTTMenuSetH_2(){
//	//set the read values to the actual Date
//	int8_t h = dataSet();
//	int8_t m = dataSet();
//	int8_t s = dataSet();
//
//	//Check if the values readed are numbers
//	if( READ_NUMBER_ERRO>h &&
//		READ_NUMBER_ERRO>m &&
//		READ_NUMBER_ERRO>s){
//	PCF8563_setHours(h);
//	PCF8563_setMinutes(m);
//	PCF8563_setSeconds(s);
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[5;10H");
//	UART_putString(UART0, "La hora ha sido cambiada... \r");
//
//	/* VT100 command for positioning the cursor in x and y position*/
//		UART_putString(UART0,"\033[8;10H");
//		writeTime();//update structures and print the Time
//
//	return GOOD;
//	}
//	UART_putString(UART0,"\033[5;10H");
//	UART_putString(UART0, "Error de hora ingresada \r");
//	pauseExcecution();
//	return ERROR;
//}
//
//uint8_t printTTMenuSetD_1(){
//	/**The following sentences send strings to PC using the UART_putString function. Also, the string
//	 * is coded with terminal code*/
//	/** VT100 command for text in red and background in cyan*/
//	UART_putString(UART0,"\033[0;32;10m");
//	/*VT100 command for clearing the screen*/
//	UART_putString(UART0,"\033[2J");
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[3;10H");
//	UART_putString(UART0, "Escribir fecha: \r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[4;10H");
//
//	return GOOD;
//}
//uint8_t printTTMenuSetD_2(){
//
//	//set the read values to the actual Date
//	int8_t d = dataSet();
//	int8_t m = dataSet();
//	int8_t y = dataSet();
//
//	//Check if the values readed are numbers
//	if( READ_NUMBER_ERRO>d &&
//	READ_NUMBER_ERRO>m &&
//	READ_NUMBER_ERRO>y){
//	PCF8563_setDays(d);
//	PCF8563_setMonths(m);
//	PCF8563_setYears(y);
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[5;10H");
//	UART_putString(UART0, "La fecha ha sido cambiada... \r");
//	UART_putString(UART0,"\033[7;10H");
//	writeDate();//update structures and print the Date
//	return GOOD;
//	}
//	UART_putString(UART0,"\033[5;10H");
//	UART_putString(UART0, "Error de fecha ingresada \r");
//	pauseExcecution();
//	return ERROR;
//}
//
//uint8_t printTTMenuFH_1(){
//	/**The following sentences send strings to PC using the UART_putString function. Also, the string
//	 * is coded with terminal code*/
//	/** VT100 command for text in red and background in cyan*/
//	UART_putString(UART0,"\033[0;32;10m");
//	/*VT100 command for clearing the screen*/
//	UART_putString(UART0,"\033[2J");
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[3;10H");
//	UART_putString(UART0, "El formato actual es: ");
//
//	if(FORM24 == getSystem()->hourFormat) UART_putString(UART0,"24h");
//	else UART_putString(UART0,"12h");
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[4;10H");
//	UART_putString(UART0, "Desea cambiar el formato? Si/No\r");
//	/* VT100 command for positioning the cursor in x and y position*/
//		UART_putString(UART0,"\033[5;10H");
//
//	return GOOD;
//}
//uint8_t printTTMenuFH_2(){
//	int8_t answer[2];
//	answer[0] = pop();
//	answer[1] = pop();
//	if(('S' == answer[0] && 'i' == answer[1]) ||//handle different input values
//		('s' == answer[0] && 'i' == answer[1]) ||
//		('S' == answer[0] && 'I' == answer[1])){
//		toggleHourFormat();//change hourFormat
//		/* VT100 command for positioning the cursor in x and y position*/
//		UART_putString(UART0,"\033[6;10H");
//		UART_putString(UART0, "El formato ha sido cambiado... \r");
//	}
//	else{
//			/* VT100 command for positioning the cursor in x and y position*/
//			UART_putString(UART0,"\033[6;10H");
//			UART_putString(UART0, "El formato no fue cambiado... \r");
//		}
//
//	return GOOD;
//}
//
//uint8_t printTTMenuRH_1(){
//	/**The following sentences send strings to PC using the UART_putString function. Also, the string
//	 * is coded with terminal code*/
//	/** VT100 command for text in red and background in cyan*/
//	UART_putString(UART0,"\033[0;32;10m");
//	/*VT100 command for clearing the screen*/
//	UART_putString(UART0,"\033[2J");
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[3;10H");
//	UART_putString(UART0, "La hora actual es:\r");
//
//	writeTime();//show time in serial port
//
//	return GOOD;
//}
//
//uint8_t printTTMenuRD_1(){
//	/**The following sentences send strings to PC using the UART_putString function. Also, the string
//	 * is coded with terminal code*/
//	/** VT100 command for text in red and background in cyan*/
//	UART_putString(UART0,"\033[0;32;10m");
//	/*VT100 command for clearing the screen*/
//	UART_putString(UART0,"\033[2J");
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[3;10H");
//	UART_putString(UART0, "La fecha actual es:\r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[4;10H");
//
//	writeDate();
//
//	return GOOD;
//}
//
//uint8_t printTTMenuEchoLCD_1(){
//	/**The following sentences send strings to PC using the UART_putString function. Also, the string
//	 * is coded with terminal code*/
//	/** VT100 command for text in red and background in cyan*/
//	UART_putString(UART0,"\033[0;32;10m");
//	/*VT100 command for clearing the screen*/
//	UART_putString(UART0,"\033[2J");
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[3;10H");
//	UART_putString(UART0, "Escribir texto:\r");
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[4;10H");
//
//	return GOOD;
//}
//uint8_t printTTMenuEchoLCD_2(){
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[5;10H");
//	UART_putString(UART0, "Termino la conexion \r");
//
//	return GOOD;
//}
//
//bool setTimeASCII(uint8_t value, bool hour){
//	if(hour){
//		if( FORM12 == getSystem()->hourFormat &&
//			FORM12_LIMIT < getSystem()->currentTime.Hours)
//				value -= 0x12;//if the value needs to change to PM format
//	}
//
//	asciiVal[0] = 48 + value/16;//the hours are the high byte
//	asciiVal[1] = 48 + value%16;//the minutes are the low byte
//	return true;//we added 48 to get the ASCII value
//}
//bool writeTime(){
//	updateSystemTimeDate();
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[5;10H");//place cursor
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[6;10H");//place cursor
//
//	setTimeASCII(getSystem()->currentTime.Hours, true);//get the hours in ASCII
//	UART_putString(UART0,asciiVal);//and print the hour
//	UART_putString(UART0,":");
//
//	setTimeASCII(getSystem()->currentTime.Minutes, false);//get the minutes in ASCII
//	UART_putString(UART0,asciiVal);//and print the minutes
//	UART_putString(UART0,":");
//
//	setTimeASCII(getSystem()->currentTime.Seconds, false);//get the seconds in ASCII
//	UART_putString(UART0,asciiVal);//and print the seconds
//	UART_putString(UART0," ");
//
//	if(FORM12 == getSystem()->hourFormat){
//		if(FORM12_LIMIT <= getSystem()->currentTime.Hours){
//			UART_putString(UART0,"PM");
//	   }else UART_putString(UART0,"AM");
//	}
//
//	return true;
//}
//bool writeDate(){
//	updateSystemTimeDate();
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[6;10H");//place cursor
//
//	/* VT100 command for positioning the cursor in x and y position*/
//	UART_putString(UART0,"\033[7;10H");//place cursor
//
//	setTimeASCII(getSystem()->currentDate.Days, false);//get the hours in ASCII
//	UART_putString(UART0,asciiVal);//and print the hour
//	UART_putString(UART0," ");
//
//	setTimeASCII(getSystem()->currentDate.Months, false);//get the minutes in ASCII
//	UART_putString(UART0,asciiVal);//and print the minutes
//	UART_putString(UART0," ");
//
//	setTimeASCII(getSystem()->currentDate.Years, false);//get the seconds in ASCII
//	UART_putString(UART0,asciiVal);//and print the seconds
//	UART_putString(UART0," ");
//
//	return true;
//}
//
//int8_t dataSet(){
//	int8_t val = pop();//get the first element in hex
//	int8_t valU = pop();//get the second element in hex
//	int8_t sum;
//	//cast the captured value depending if it was a letter or a number
//	if(!(NUMBER_LOW<=val && val<=NUMBER_HIGH &&
//			NUMBER_LOW<=valU && valU<=NUMBER_HIGH)){
//		sum = READ_NUMBER_ERRO;
//	}
//	else{
//		val -= 48;
//		valU -= 48;
//		sum = (val<<4);//shift the upper hex value
//		sum += valU;//add the low hex value
//	}
//	return sum;
//}
//int8_t dataSetInt(){
//	int8_t val = pop() -48;//get the first element in hex
//	int8_t valU = pop() -48;//get the second element in hex
//	if(FIRST_ELEMENT <= val && TEN >= val){
//	if(!(FIRST_ELEMENT <= valU && TEN >= valU))
//		return val;
//	int8_t sum = (val*10);//shift the upper dec value
//	sum += valU;//add the low hex value
//	return sum;
//	}
//	return false;
//}
