/*
 * PantallaPC.c

 *
 *  Created on: 2 nov. 2017
 *      Author: Avelar Díaz José Francisco
 *      		Chung Correa Sergio Raúl
 */
#include "board.h"
#include "MK64F12.h"
#include "pin_mux.h"
#include "GlobalFunctions.h"
//#include "PCF8563.h"
#include "DataTypeDefinitions.h"
#include "PantallaPC.h"
//#include "MEM24LC256.h"
#include "fsl_uart.h"
#include "UART_BT.h"
#include "LCDNokia5110.h"
#include "UART_TeraTerm.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

#include "MEM24LC256.h"

#define EVENT_UART0 (1<<0)
#define EVENT_UART4 (1<<1)

BooleanType Formatohora = FALSE;    //bandera para cambiar el formato
static sint8 string[2];
static sint8 formato[2] = { 'h', 'r' };
HoraActual datos;
SemaphoreHandle_t mutexLeerM;
SemaphoreHandle_t mutexEscribirM;
SemaphoreHandle_t mutexEhora;
SemaphoreHandle_t mutexEfecha;
SemaphoreHandle_t mutexFhora;
SemaphoreHandle_t mutexLhora;
SemaphoreHandle_t mutexLfecha;
SemaphoreHandle_t mutexEco;
EventGroupHandle_t g_chat_events;

/*
 * Realiza las dos operaciones para enviar escribir en el serial, primero la posicion y despues los valores
 */
void escribirP( UART_Type *base, sint8* Posicion, sint8* string ) {
	if (UART4 == base)
	{
		uart_BT_send(base, (uint8_t*) Posicion);
		uart_BT_send(base, (uint8_t*) string);
	} else
	{
		uart_TeraTerm_send(base, (uint8_t*) Posicion);
		uart_TeraTerm_send(base, (uint8_t*) string);
	}
}

/*
 * Funcion que espera hasta que terminemos de ingresar los valores que queremos y guardarlos en la queue.
 */
void ingresoDatos( UART_Type *base ) {
	if (UART0 == base)
		uart_TeraTerm_receive();
	else
		uart_BT_receive();
}

/*
 * Se imprime el menu inicial de la práctica
 */
void MenuInicial( UART_Type *uart ) {

	/**clear screen*/
	escribirP(uart, "\033[10;10H", "\033[2J");
	/** VT100 command for positioning the cursor in x and y position and sending the message*/
	escribirP(uart, "\033[10;10H", "\r 1) Leer Memoria I2C \n");
	escribirP(uart, "\033[11;10H", "\r 2) Escribir memoria I2C \n");
	escribirP(uart, "\033[12;10H", "\r 3) Establecer Hora \n");
	escribirP(uart, "\033[13;10H", "\r 4) Establecer Fecha \n");
	escribirP(uart, "\033[14;10H", "\r 5) Formato de hora \n");
	escribirP(uart, "\033[15;10H", "\r 6) Leer hora \n");
	escribirP(uart, "\033[16;10H", "\r 7) Leer fecha \n");
	escribirP(uart, "\033[17;10H", "\r 8) Comunicacion con terminal 2 \n");
	escribirP(uart, "\033[18;10H", "\r 9) Eco en LCD \n");
}

/*
 * Escribe las intrucciones del menu, ingresamos el valor del address, obtenemos lo ingresado
 * con la funcion pop, para guardar en su valor en un integer, les hacemos un desplazamiento
 * con un OR. Después obtenemos el valor contenido en esa direccion y utlilizando la funcion
 * valorMem convertimos ese dato en un string para poder imprimirlo en pantalla
 */
void LeerM( UART_Type *uart ) {

	escribirP(uart, "\033[9;10H", "Terminal Ocupada");
	xSemaphoreTake(mutexLeerM, portMAX_DELAY);

	uint16 address;
	uint8 l_decimales = 0;
	uint8 l_unidades = 0;
	uint8 h_decimales = 0;
	uint8 h_unidades = 0;
	uint8 dataSize = 0;
	uint8 x = 0;
	/*LIMPIAR LA QUEUE*/
	escribirP(uart, "\033[10;10H", "\033[2J");
	escribirP(uart, "\033[10;10H", "Direccion de lectura:   ");
	ingresoDatos(uart);

	h_decimales = valMemoria();
	h_unidades = valMemoria();
	l_decimales = valMemoria();
	l_unidades = valMemoria();
	address = (h_decimales << 12) | (h_unidades << 8) | (l_decimales << 4)
			| l_unidades;

	escribirP(uart, "\033[11;10H", "Longitud en bytes:  ");
	ingresoDatos(uart);

	uint8_t i = 0;
	do
	{
	    dataSize *= 10;
	    dataSize += x;
        x = valMemoria();
	}while(QUEUE_END != x);

	escribirP(uart, "\033[12;10H", "Contenido: ");

    uint8_t val2[dataSize];
    uint8_t* data2 = &val2[0];
    MEM24LC256_getData (address, dataSize, data2);
    val2[dataSize] = '\0';

    escribirP(uart, "\033[14;10H", (sint8*) &val2[0]);
	escribirP(uart, "\033[16;10H", "Presiona una tecla para continuar... ");

	ingresoDatos(uart);
	xSemaphoreGive(mutexLeerM);

}

/*
 * Ingresamos la direccion de escritura, despues de la misma manera que en la funcion de LeerM obtenemos la direccion escrita
 * con pop's y un OR. Despues ingresamos lo que queremos escribir, se muestra en la pantalla y lo guardamos en memoria
 * utilizando la funcion Memoria_24LC256_serValue la cual nos regresa una flag status en verdadero cuando termina de guardar
 * todos los datos contenidos en la fifo
 */
void EscribirM( UART_Type *uart ) {

	escribirP(uart, "\033[9;10H", "Terminal Ocupada");
	xSemaphoreTake(mutexEscribirM, portMAX_DELAY);

	uint16 address;
	uint8 l_decimales = 0;
	uint8 l_unidades = 0;
	uint8 h_decimales = 0;
	uint8 h_unidades = 0;
	uint8 x;

	escribirP(uart, "\033[10;10H", "\033[2J");
	escribirP(uart, "\033[10;10H", "Direccion de escritura:");

	ingresoDatos(uart);

    h_decimales = valMemoria();
    h_unidades = valMemoria();
    l_decimales = valMemoria();
    l_unidades = valMemoria();
	address = (h_decimales << 12) | (h_unidades << 8) | (l_decimales << 4)
			| l_unidades;

	escribirP(uart, "\033[11;10H", "Texto a guardar: ");
	ingresoDatos(uart);
	//escribirP(uart,"\033[11;50H", getFIFO());

	uint8_t string[32];
	uint8_t i = 0;
	do
	{
	    x = leerQueue_TeraTerm();
	    string[i++] = x;
	}while((QUEUE_END != x));
	string[i] = '\0';

	MEM24LC256_setData(address, &string[0]);
	escribirP(uart, "\033[13;10H", "Su texto ha sido guardado...");

	ingresoDatos(uart);
	xSemaphoreGive(mutexEscribirM);
}

/*
 * En esta funcion ingresamos las horas minutos y segundos y mediante pops() vamos guardando el tres variables sus valores,
 * para guardar los dos dígitos que componen a cada uno utilizamos el mismo procedimiento que para obtener el address al
 * trabajar con memorias, hacemos un corrimiento y un OR. Al final los guardamos utilizando la funcion del PCF8563
 */
void Ehora( UART_Type *uart ) {

	escribirP(uart, "\033[9;10H", "Terminal Ocupada");
	xSemaphoreTake(mutexEhora, portMAX_DELAY);
	uint8 valor;
	uint8 valor2;
	uint8 hora;
	uint8 min;
	uint8 seg;
	escribirP(uart, "\033[10;10H", "\033[2J");
	escribirP(uart, "\033[10;10H", "Escribir hora en hh/mm/ss");
	ingresoDatos(uart);
	//escribirP(uart,"\033[10;50H", getFIFO());

	//valor = pop();
	//valor2 = pop();
	hora = (valor << 4) | valor2;
	//valor = pop();
	//valor2 = pop();
	min = (valor << 4) | valor2;
	//valor = pop();
	//valor2 = pop();
	seg = (valor << 4) | valor2;

	//resetContador();
//	PCF8563_SetHours(PCF8563_configurationStruct(), hora);
//	PCF8563_SetMinutes(PCF8563_configurationStruct(), min);
//	PCF8563_SetSeconds(PCF8563_configurationStruct(), seg);
	escribirP(uart, "\033[12;10H", "La hora ha sido cambiada...");
	xSemaphoreGive(mutexEhora);
}

/*
 * En esta funcion realizamos exactamente el mismo procedimiento para obtener los datos y gaurdarlos
 * que en la funcion de Ehora. Obtenemos datos pop(), hacemos un corrimiento y lo guardamos con la
 * fucion correspondiente del PCF8563
 */
void Efecha( UART_Type *uart ) {

	escribirP(uart, "\033[9;10H", "Terminal Ocupada");
	xSemaphoreTake(mutexEfecha, portMAX_DELAY);
	uint8 dia;
	uint8 mes;
	uint8 aa;
	uint8 valor;
	uint8 valor2;
	//resetContador();
	escribirP(uart, "\033[10;10H", "\033[2J");
	escribirP(uart, "\033[10;10H", "Escribir fecha en dd/mm/aa");
	ingresoDatos(uart);
	//escribirP(uart,"\033[10;50H", getFIFO());

	//valor = pop();
	//valor2 = pop();
	dia = (valor << 4) | valor2;
	//valor = pop();
	//valor2 = pop();
	mes = (valor << 4) | valor2;
	//valor = pop();
	//valor2 = pop();
	aa = (valor << 4) | valor2;

	//resetContador();
//	PCF8563_SetDay(PCF8563_configurationStruct(), dia);
//	PCF8563_SetMonth(PCF8563_configurationStruct(), mes);
//	PCF8563_SetYear(PCF8563_configurationStruct(), aa);
	escribirP(uart, "\033[12;10H", "La fecha ha sido cambiada...");
	xSemaphoreGive(mutexEfecha);
}

/*
 * En esta funcion solo recibe un Si o No, con un pop() obtenemos el primer valor de la fifo
 * comparamos si es una S ya sea mayúscula o minúscula y de ser así activamos la flag del formato
 */
void Fhora( UART_Type *uart ) {

	escribirP(uart, "\033[9;10H", "Terminal Ocupada");
	xSemaphoreTake(mutexFhora, portMAX_DELAY);

	uint8 formato;
	uint8 S = 35;
	escribirP(uart, "\033[10;10H", "\033[2J");
	escribirP(uart, "\033[10;10H", "El formato actual es 12h");
	escribirP(uart, "\033[11;10H", "Desea cambiar el formato a 12h si/no?");
	ingresoDatos(uart);
	//escribirP(uart,"\033[11;50H", getFIFO());
	//formato=pop();
	formato == S || formato == 67 ? setFlagF() : clearFlagF();
	//resetContador();
	escribirP(uart, "\033[13;10H", "El formato ha sido cambiado...");
	xSemaphoreGive(mutexFhora);
}

/*
 * Obtenemos los valores de hora, minuto y segundo con su funcion correspondiente de PCF8563
 * Despues utilizando la funcion valorMem convertimos cada valor recibido en un string
 * pára poder mostrarlo en pantalla. Esto se hace en un cilco hasta que la bandera del
 * mailbox se active (se presione cualquier tecla)
 */
void Lhora( UART_Type *uart ) {

	escribirP(uart, "\033[9;10H", "Terminal Ocupada");
	xSemaphoreTake(mutexLhora, portMAX_DELAY);

	escribirP(uart, "\033[10;10H", "\033[2J");
	escribirP(uart, "\033[10;10H", "La hora actual es");
//	escribirP(uart,"\033[12;10H", datos.hora);
//	escribirP(uart,"\033[12;13H", ":");
//	escribirP(uart,"\033[12;15H", datos.minutos);
//	escribirP(uart,"\033[12;18H", ":");
//	escribirP(uart,"\033[12;20H", datos.segundos);
//	escribirP(uart,"\033[12;25H", formato);
	xSemaphoreGive(mutexLhora);
}

/*
 * Obtenemos los valores del año, mes y dia con su funcion correspondiente de PCF8563
 * Despues utilizando la funcion valorMem convertimos cada valor recibido en un string
 * pára poder mostrarlo en pantalla. Esto se hace en un cilco hasta que la bandera del
 * mailbox se active (se presione cualquier tecla)
 */
void Lfecha( UART_Type *uart ) {

	escribirP(uart, "\033[9;10H", "Terminal Ocupada");
	xSemaphoreTake(mutexLfecha, portMAX_DELAY);

	uint8 valor;
	escribirP(uart, "\033[10;10H", "\033[2J");
	escribirP(uart, "\033[10;10H", "La fecha actual es");
//	HoraActual variable = Fecha_Hora();
//	clearFlagM();
//		while(FALSE==getFlagM()){
//			escribirP(uart,"\033[12;10H", datos.dia);
//			escribirP(uart,"\033[12;13H", "/");
//			escribirP(uart,"\033[12;15H", datos.mes);
//			escribirP(uart,"\033[12;18H", "/");
//			escribirP(uart,"\033[12;20H", datos.anio);
//		}
//	while(FALSE==getFlagM());//flag del mailbox si está recibiendo datos
	xSemaphoreGive(mutexLfecha);
}

void Comunicacion( UART_Type *uart ) {
	if (UART0 == uart)
		xEventGroupSetBits(g_chat_events, EVENT_UART0);
	else
		xEventGroupSetBits(g_chat_events, EVENT_UART4);
}

void chat() {
	while (1)
	{
		xEventGroupWaitBits(g_chat_events,
		EVENT_UART0 | EVENT_UART4,
		pdTRUE, pdTRUE, portMAX_DELAY);
	}
}

void Eco( UART_Type *uart ) {
	escribirP(uart, "\033[9;10H", "Terminal Ocupada");
	xSemaphoreTake(mutexEco, portMAX_DELAY);
	escribirP(uart, "\033[10;10H", "\033[2J");
	escribirP(uart, "\033[10;10H", "Escribir texto:");
	if (UART0 == uart)
		uart_TeraTerm_echo();
	else
		uart_BT_echo();
	escribirP(uart, "\033[10;10H", "Termino la ocnexion");
	xSemaphoreGive(mutexEco);
}

void Fecha_Hora() {
	uint8 valor;
	uint8 valor1;
	uint8 string1[] = "  Hora"; /*! String to be printed in the LCD*/
	uint8 string2[] = ":";
	uint8 string3[] = "  Fecha";
//	imprimir_lcd(string1, 2, 0);
//	clearFlagM();  FLAG MAILBOX
//			while(FALSE==getFlagM()){
//				valor = PCF8563_getHours(PCF8563_configurationStruct());
//				valor1 = formatoHora(valor);
//				datos.hora =(valorMem(valor1));
//				imprimir_lcd(datos.hora, 2, 1);
//				imprimir_lcd(string2, 3, 1);
//				valor = PCF8563_getMinutes(PCF8563_configurationStruct());
//				datos.minutos =(valorMem(valor));
//				imprimir_lcd(datos.minutos, 4, 1);
//				imprimir_lcd(string2, 5, 1);
//				valor = PCF8563_getSeconds(PCF8563_configurationStruct());
//				datos.segundos =(valorMem(valor));
//				imprimir_lcd(datos.segundos, 6, 1);
//				imprimir_lcd(formato, 7, 1);
//				imprimir_lcd(string3, 2, 2);
//				valor = PCF8563_getDay(PCF8563_configurationStruct());
//				datos.dia=(valorMem(valor));
//				imprimir_lcd(datos.dia, 2, 3);
//				imprimir_lcd("/", 3, 3);
//				valor = PCF8563_getMonth(PCF8563_configurationStruct());
//				datos.mes=(valorMem(valor));
//				imprimir_lcd(datos.mes, 4, 3);
//				imprimir_lcd( "/", 5, 3);
//				valor = PCF8563_getYear(PCF8563_configurationStruct());
//				datos.anio=(valorMem(valor));
//				imprimir_lcd( datos.anio, 6, 3);
//			}
//		while(FALSE==getFlagM());
	while (1)
	{
		limpiar_lcd();
		imprimir_lcd(string1, 2, 0);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

/*
 * Fucion para convertir de enteros a string
 * Esta funcion es utilizada para convertir las horas, minutos, segundos, años, meses, dias
 * estos valores llegan como un entero y lo dividimos en decenas y unidades guardando por
 * separado estos valores en un string el cual es el valor que retorna.
 */
sint8* valorMem( uint8 x ) {

	uint8 DecString = (uint8) (x / 10 + 48);
	uint8 UniString = (uint8) (x - 10 * (x / 10) + 48);

	string[0] = DecString;
	string[1] = UniString;
	return &string[0];

}

/*
 *Esta funcion corrige la diferencia en el orden de los valores en ASCII a hexadecimal. Si queremos convertir
 *los numeros, simplemente restamos 48 pero si tenemos por ejemplo una A -48 no nos da 10, el valor de las letras
 *los empieza 7 espacios más arriba, ese es el proposito de la funcion
 */
uint8 valMemoria() {
	uint8 variable = leerQueue_TeraTerm();
	if(variable >= 17 && variable <= 22)
		variable = variable -7;
	else if (variable >=49 && variable <=57)
		variable = variable - 48;
	else variable = variable;

	return variable ;
}

/*
 * Limpiar bandera del formato hora
 */
void clearFlagF() {
	Formatohora = FALSE;
}

/*
 * Encender bandera del formato hora
 */
void setFlagF() {
	Formatohora = TRUE;
}

/*
 * Obtener el estado de la bandera
 */
BooleanType getFlagF() {
	return Formatohora;
}

/*
 * En esta funcion hacemos uso de la bandera de formato hora, comparamos si se activó, restamos 12 al valor
 * y cambiamos el string formato ya sea para escribir en pantalla am o pm
 */
uint8 formatoHora( uint8 x ) {
	formato[0] = 'h';
	formato[1] = 'r';
	/*if(x>11){
	 formato[0] = 'p';
	 formato[1] = 'm';
	 }
	 */
	if (TRUE == getFlagF() && x > 12)
	{
		x = x - 12;
		formato[0] = 'p';
		formato[1] = 'm';
	} else if (TRUE == getFlagF() && x < 12)
	{
		formato[0] = 'a';
		formato[1] = 'm';
	}
	return x;
}
/*VT100 command for clearing the screen
 UART_putString(uart,"\033[2J");*/

uint8_t escogerMenu( UART_Type *uart ) {
	if (UART4 == uart)
	{
		uart_BT_receive();
		return (leerQueue_BT() - 48);
	} else
	{
		uart_TeraTerm_receive();
		return (leerQueue_TeraTerm() - 48);
	}

}

void initmutex() {
	mutexLeerM = xSemaphoreCreateMutex();
	mutexEscribirM = xSemaphoreCreateMutex();
	mutexEhora = xSemaphoreCreateMutex();
	mutexEfecha = xSemaphoreCreateMutex();
	mutexFhora = xSemaphoreCreateMutex();
	mutexLhora = xSemaphoreCreateMutex();
	mutexLfecha = xSemaphoreCreateMutex();
	mutexEco = xSemaphoreCreateMutex();
	g_chat_events = xEventGroupCreate();
}
