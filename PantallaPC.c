
 /*
 * PantallaPC.c
 *
 *  Created on: 2 nov. 2017
 *      Author: Avelar Díaz José Francisco
 *      		Chung Correa Sergio Raúl
 */


#include "MK64F12.h" /* include peripheral declarations */
#include "UART.h"/**UART device driver*/
#include "NVIC.h"/**NVIC device driver*/
#include "Fifo.h"
#include "PCF8563.h"
#include "DataTypeDefinitions.h"
#include "PantallaPC.h"
#include "Memoria_24LC256.h"

BooleanType Formatohora = FALSE; //bandera para cambiar el formato
static sint8 string[2] ;
sint8 formato[2] = {'h','r'};

/*
 * Inicializacion/configuracion del puerto, UART, NVIC
 */
void initP(){
	   /**Enables the clock of PortB in order to configures TX and RX of UART peripheral*/
		SIM->SCGC5 = SIM_SCGC5_PORTB_MASK;
		/**Configures the pin control register of pin16 in PortB as UART RX*/
		PORTB->PCR[16]= PORT_PCR_MUX(3);
		/**Configures the pin control register of pin16 in PortB as UART TX*/
		PORTB->PCR[17] = PORT_PCR_MUX(3);
		/**Configures UART 0 to transmit/receive at 11520 bauds with a 21 MHz of clock core*/
		UART_init (UART_0,  60000000, BD_115200);

		NVIC_setBASEPRI_threshold(PRIORITY_5);

		/**Enables the UART 0 interrupt*/
		UART_interruptEnable(UART_0);
		/**Enables the UART 0 interrupt in the NVIC*/
		NVIC_enableInterruptAndPriotity(UART0_IRQ, PRIORITY_3);

		EnableInterrupts;
}

/*
 * Realiza las dos operaciones para enviar escribir en el serial, primero la posicion y despues los valores
 */
void escribirP(UART_ChannelType uartChannel,  sint8* Posicion,  sint8* string ){
	UART_putString(UART_0, Posicion);
	UART_putString(UART_0, string);
}

/*
 * Funcion que espera hasta que terminemos de ingresar los valores que queremos. En otras palabras espera un ENTER
 */
void ingresoDatos(){
	while(FALSE == getflagEnter());
	clearflagE();
}

/*
 * Se imprime el menu inicial de la práctica
 */
void MenuInicial(){
	/**clear screen*/
	UART_putString(UART_0,"\033[2J");
	/** VT100 command for positioning the cursor in x and y position and sending the message*/
	escribirP(UART_0,"\033[10;10H", "1) Leer Memoria I2C");
	escribirP(UART_0,"\033[11;10H", "2) Escribir memoria I2C");
	escribirP(UART_0,"\033[12;10H", "3) Establecer Hora");
	escribirP(UART_0,"\033[13;10H", "4) Establecer Fecha");
	escribirP(UART_0,"\033[14;10H", "5) Formato de hora");
	escribirP(UART_0,"\033[15;10H", "6) Leer hora");
	escribirP(UART_0,"\033[16;10H", "7) Leer fecha");
	escribirP(UART_0,"\033[18;10H", "");
}

/*
 * Escribe las intrucciones del menu, ingresamos el valor del address, obtenemos lo ingresado
 * con la funcion pop, para guardar en su valor en un integer, les hacemos un desplazamiento
 * con un OR. Después obtenemos el valor contenido en esa direccion y utlilizando la funcion
 * valorMem convertimos ese dato en un string para poder imprimirlo en pantalla
 */
void LeerM(){
	uint16 address;
	uint8 l_decimales=0;
	uint8 l_unidades=0;
	uint8 h_decimales=0;
	uint8 h_unidades=0;
	uint8 valor;
	uint8 NDatos ;
	uint8 ContadorDeDatosExtraidos = 0;
	resetContador();
	UART_putString(UART_0,"\033[2J");
	escribirP(UART_0,"\033[10;10H", "Direccion de lectura:");
	ingresoDatos();
	escribirP(UART_0,"\033[10;50H", getFIFO());
	h_decimales = valMemoria();
	h_decimales = valMemoria();
	h_decimales = valMemoria();
	h_unidades = valMemoria();
	l_decimales = valMemoria();
	l_unidades = valMemoria();
	address = (h_decimales <<12)|(h_unidades <<8)|(l_decimales <<4)|l_unidades;

	resetContador();
	escribirP(UART_0,"\033[11;10H", "Longitud en bytes:");
	ingresoDatos();

	NDatos = valMemoria();
	sint8 StringFromMemory[NDatos];
	escribirP(UART_0,"\033[11;50H", getFIFO());
	resetContador();
	escribirP(UART_0,"\033[12;10H", "Contenido");
	clearFlagM();


	for( ContadorDeDatosExtraidos = 0; ContadorDeDatosExtraidos<NDatos ; ContadorDeDatosExtraidos++){

		StringFromMemory[ContadorDeDatosExtraidos] = Memoria_24LC256_getValue(get_Memoria_24LC256_ConfigStruct(),(address +ContadorDeDatosExtraidos));
	}


	escribirP(UART_0,"\033[13;10H", StringFromMemory);
	escribirP(UART_0,"\033[14;10H", "Presiona una tecla para continuar...");


	while(FALSE==getFlagM());


}

/*
 * Ingresamos la direccion de escritura, despues de la misma manera que en la funcion de LeerM obtenemos la direccion escrita
 * con pop's y un OR. Despues ingresamos lo que queremos escribir, se muestra en la pantalla y lo guardamos en memoria
 * utilizando la funcion Memoria_24LC256_serValue la cual nos regresa una flag status en verdadero cuando termina de guardar
 * todos los datos contenidos en la fifo
 */
void EscribirM(){
	uint16 address;
	uint8 l_decimales=0;
	uint8 l_unidades=0;
	uint8 h_decimales=0;
	uint8 h_unidades=0;
	uint8 x;


	uint8 NDatos = 0;


	UART_putString(UART_0,"\033[2J");
	escribirP(UART_0,"\033[10;10H", "Direccion de escritura:");
	resetContador();
	ingresoDatos();
	escribirP(UART_0,"\033[10;50H", getFIFO());

	h_decimales = valMemoria();
	h_decimales = valMemoria();
	h_decimales = valMemoria();
	h_unidades = valMemoria();
	l_decimales = valMemoria();
	l_unidades = valMemoria();
	address = (h_decimales <<12)|(h_unidades <<8)|(l_decimales <<4)|l_unidades;
	resetContador();
	escribirP(UART_0,"\033[11;10H", "Texto a guardar: ");
	ingresoDatos();
	escribirP(UART_0,"\033[11;50H", getFIFO());


	x = (pop() + 48);
	while(x != '\0'){


		while(FALSE == Memoria_24LC256_setValue(get_Memoria_24LC256_ConfigStruct(), (address +NDatos),x));
		NDatos +=1;
		x = (pop() + 48);
	}

	resetContador();
	escribirP(UART_0,"\033[13;10H", "Su texto ha sido guardado...");
}

/*
 * En esta funcion ingresamos las horas minutos y segundos y mediante pops() vamos guardando el tres variables sus valores,
 * para guardar los dos dígitos que componen a cada uno utilizamos el mismo procedimiento que para obtener el address al
 * trabajar con memorias, hacemos un corrimiento y un OR. Al final los guardamos utilizando la funcion del PCF8563
 */
void Ehora(){
	uint8 valor;
	uint8 valor2;
	uint8 hora;
	uint8 min;
	uint8 seg;
	UART_putString(UART_0,"\033[2J");
	escribirP(UART_0,"\033[10;10H", "Escribir hora en hh/mm/ss");
	ingresoDatos();
	escribirP(UART_0,"\033[10;50H", getFIFO());

	valor = pop();
	valor2 = pop();
	hora = (valor<<4)|valor2;
	valor = pop();
	valor2 = pop();
	min = (valor<<4)|valor2;
	valor = pop();
	valor2 = pop();
	seg = (valor<<4)|valor2;

	resetContador();
	PCF8563_SetHours(PCF8563_configurationStruct(), hora);
	PCF8563_SetMinutes(PCF8563_configurationStruct(), min);
	PCF8563_SetSeconds(PCF8563_configurationStruct(), seg);
	escribirP(UART_0,"\033[12;10H", "La hora ha sido cambiada...");
}

/*
 * En esta funcion realizamos exactamente el mismo procedimiento para obtener los datos y gaurdarlos
 * que en la funcion de Ehora. Obtenemos datos pop(), hacemos un corrimiento y lo guardamos con la
 * fucion correspondiente del PCF8563
 */
void Efecha(){
	uint8 dia;
	uint8 mes;
	uint8 aa;
	uint8 valor;
	uint8 valor2;
	resetContador();
	UART_putString(UART_0,"\033[2J");
	escribirP(UART_0,"\033[10;10H", "Escribir fecha en dd/mm/aa");
	ingresoDatos();
	escribirP(UART_0,"\033[10;50H", getFIFO());

	valor = pop();
	valor2 = pop();
	dia = (valor<<4)|valor2;
	valor = pop();
	valor2 = pop();
	mes = (valor<<4)|valor2;
	valor = pop();
	valor2 = pop();
	aa =(valor<<4)|valor2;

	resetContador();
	PCF8563_SetDay(PCF8563_configurationStruct(), dia);
	PCF8563_SetMonth(PCF8563_configurationStruct(), mes);
	PCF8563_SetYear(PCF8563_configurationStruct(), aa);
	escribirP(UART_0,"\033[12;10H", "La fecha ha sido cambiada...");
}

/*
 * En esta funcion solo recibe un Si o No, con un pop() obtenemos el primer valor de la fifo
 * comparamos si es una S ya sea mayúscula o minúscula y de ser así activamos la flag del formato
 */
void Fhora(){
	uint8 formato;
	uint8 S = 35;
	UART_putString(UART_0,"\033[2J");
	escribirP(UART_0,"\033[10;10H", "El formato actual es 12h");
	escribirP(UART_0,"\033[11;10H", "Desea cambiar el formato a 12h si/no?");
	ingresoDatos();
	escribirP(UART_0,"\033[11;50H", getFIFO());
	formato=pop();
	formato==S || formato== 67? setFlagF():clearFlagF();
	resetContador();
	escribirP(UART_0,"\033[13;10H", "El formato ha sido cambiado...");
}

/*
 * Obtenemos los valores de hora, minuto y segundo con su funcion correspondiente de PCF8563
 * Despues utilizando la funcion valorMem convertimos cada valor recibido en un string
 * pára poder mostrarlo en pantalla. Esto se hace en un cilco hasta que la bandera del
 * mailbox se active (se presione cualquier tecla)
 */
void Lhora(){
	uint8 valor;
	uint8 valor1;
	UART_putString(UART_0,"\033[2J");
	escribirP(UART_0,"\033[10;10H", "La hora actual es");
	clearFlagM();
		while(FALSE==getFlagM()){
			valor = PCF8563_getHours(PCF8563_configurationStruct());
			valor1 = formatoHora(valor);
			sint8* x=(valorMem(valor1));
			escribirP(UART_0,"\033[12;10H", x);
			escribirP(UART_0,"\033[12;13H", ":");
			valor = PCF8563_getMinutes(PCF8563_configurationStruct());
			x=(valorMem(valor));
			escribirP(UART_0,"\033[12;15H", x);
			escribirP(UART_0,"\033[12;18H", ":");
			valor = PCF8563_getSeconds(PCF8563_configurationStruct());
			x=(valorMem(valor));
			escribirP(UART_0,"\033[12;20H", x);
			escribirP(UART_0,"\033[12;25H", formato);
		}
	while(FALSE==getFlagM());
}

/*
 * Obtenemos los valores del año, mes y dia con su funcion correspondiente de PCF8563
 * Despues utilizando la funcion valorMem convertimos cada valor recibido en un string
 * pára poder mostrarlo en pantalla. Esto se hace en un cilco hasta que la bandera del
 * mailbox se active (se presione cualquier tecla)
 */
void Lfecha(){
	uint8 valor;
	UART_putString(UART_0,"\033[2J");
	escribirP(UART_0,"\033[10;10H", "La fecha actual es");
	clearFlagM();
		while(FALSE==getFlagM()){
			valor = PCF8563_getDay(PCF8563_configurationStruct());
			sint8* x=(valorMem(valor));
			escribirP(UART_0,"\033[12;10H", x);
			escribirP(UART_0,"\033[12;13H", "/");
			valor = PCF8563_getMonth(PCF8563_configurationStruct());
			x=(valorMem(valor));
			escribirP(UART_0,"\033[12;15H", x);
			escribirP(UART_0,"\033[12;18H", "/");
			valor = PCF8563_getYear(PCF8563_configurationStruct());
			x=(valorMem(valor));
			escribirP(UART_0,"\033[12;20H", x);
		}
	while(FALSE==getFlagM());
}

/*
 * Fucion para convertir de enteros a string
 * Esta funcion es utilizada para convertir las horas, minutos, segundos, años, meses, dias
 * estos valores llegan como un entero y lo dividimos en decenas y unidades guardando por
 * separado estos valores en un string el cual es el valor que retorna.
 */
sint8* valorMem(uint8 x){

	uint8 DecString = (uint8) ( x/10 +48);
	uint8 UniString = (uint8) ( x-10*( x/10) +48);

	string [0] = DecString;
	string [1] = UniString;
	return &string[0];


}

/*
 *Esta funcion corrige la diferencia en el orden de los valores en ASCII a hexadecimal. Si queremos convertir
 *los numeros, simplemente restamos 48 pero si tenemos por ejemplo una A -48 no nos da 10, el valor de las letras
 *los empieza 7 espacios más arriba, ese es el proposito de la funcion
 */
uint8 valMemoria(){
	uint8 variable = pop();
	if(variable >= 17 && variable <= 22)
		variable = variable -7;
	else if (variable >=49 && variable <=54)
		variable = variable - 39;
	else variable = variable;

	return variable ;
}

/*
 * Limpiar bandera del formato hora
 */
void clearFlagF(){
	Formatohora = FALSE;
}

/*
 * Encender bandera del formato hora
 */
void setFlagF(){
	Formatohora = TRUE;
}

/*
 * Obtener el estado de la bandera
 */
BooleanType getFlagF(){
	return Formatohora;
}

/*
 * En esta funcion hacemos uso de la bandera de formato hora, comparamos si se activó, restamos 12 al valor
 * y cambiamos el string formato ya sea para escribir en pantalla am o pm
 */
uint8 formatoHora(uint8 x){
	formato[0] = 'h';
	formato[1] = 'r';
		/*if(x>11){
				formato[0] = 'p';
				formato[1] = 'm';
			}
*/
		if(TRUE == getFlagF() && x>12){
			x= x-12;
			formato[0] = 'p';
			formato[1] = 'm';
		}
		else if(TRUE == getFlagF() && x<12){
					formato[0] = 'a';
					formato[1] = 'm';
		}
	return x;
}
/*VT100 command for clearing the screen
	UART_putString(UART_0,"\033[2J");*/

