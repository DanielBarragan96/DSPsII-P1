/*
 * PantallaPC.c

 *
 *  Created on: 2 nov. 2017
 *      Author: Avelar Díaz José Francisco
 *      		Chung Correa Sergio Raúl
 *      		Barragán Alvarez Daniel
 */

#include "board.h"
#include "MK64F12.h"
#include "pin_mux.h"
#include "GlobalFunctions.h"

#include "DataTypeDefinitions.h"
#include "PantallaPC.h"

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
#include "PCF8563.h"

#define EVENT_UART0 (1<<0)
#define EVENT_UART4 (1<<1)
#define escBT 127
#define ESC 27
#define QUEUE_END 160

SemaphoreHandle_t mutexLeerM;
SemaphoreHandle_t mutexEscribirM;
SemaphoreHandle_t mutexEhora;
SemaphoreHandle_t mutexEfecha;
SemaphoreHandle_t mutexFhora;
SemaphoreHandle_t mutexLhora;
SemaphoreHandle_t mutexLfecha;
SemaphoreHandle_t mutexEco;
EventGroupHandle_t g_chat_events;

static bool show_time = false;
static bool show_date = false;

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

void escribirchat(sint8* Posicion, sint8* string ){
    uart_BT_send(UART4, (uint8_t*) Posicion);
    uart_BT_send(UART4, (uint8_t*) string);
    uart_TeraTerm_send(UART0, (uint8_t*) Posicion);
    uart_TeraTerm_send(UART0, (uint8_t*) string);
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

	escribirP(uart, "\033[10;10H", "\033[2J");
	escribirP(uart, "\033[10;10H", "Escribir hora en hh:mm:ss \n");
	ingresoDatos(uart);

	uint8_t hours = valMemoria()*10;
	hours += valMemoria();
	valMemoria();
	uint8_t minutes = valMemoria()*10;
	minutes += valMemoria();
	valMemoria();
	uint8_t seconds = valMemoria()*10;
	seconds += valMemoria();

	if(0 == setTime(hours, minutes, seconds))
	    escribirP(uart, "\033[13;10H", "La hora ha sido cambiada...\n");
	else
	    escribirP(uart, "\033[13;10H", "Error, no se estableció la hora\n");

	show_time = true;

	ingresoDatos(uart);
	show_time = false;
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

	escribirP(uart, "\033[10;10H", "\033[2J");
	escribirP(uart, "\033[10;10H", "Escribir fecha en dd/mm/aa");
	ingresoDatos(uart);

    uint8_t day = valMemoria()*10;
    day += valMemoria();
    valMemoria();
    uint8_t month = valMemoria()*10;
    month += valMemoria();
    valMemoria();
    uint8_t year = valMemoria()*10;
    year += valMemoria();

    if(0 == setDate(day, month, year))
            escribirP(uart, "\033[12;10H", "La fecha ha sido cambiada...");
    else
            escribirP(uart, "\033[12;10H", "Error. La fecha no ha sido cambiada...");

    show_date = true;

    ingresoDatos(uart);
    show_date = false;
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
	escribirP(uart, "\033[11;10H", "Desea cambiar el formato a 12h si(1)/no(0)? \n");
	ingresoDatos(uart);

	uint8_t format = (valMemoria());

	if(0 == setTimeFormat(format))
	    escribirP(uart, "\033[13;10H", "El formato ha sido cambiado... ");
	else
	    escribirP(uart, "\033[13;10H", "El formato no ha sido cambiado... ");

	ingresoDatos(uart);
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
	escribirP(uart, "\033[10;10H", "La hora actual es: \n");

	show_time = true;

	ingresoDatos(uart);
	show_time = false;
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
	escribirP(uart, "\033[10;10H", "La fecha actual es:\n");


    show_date = true;

	ingresoDatos(uart);
	show_date = false;
	xSemaphoreGive(mutexLfecha);
}

void Comunicacion( UART_Type *uart ) {
	if (UART0 == uart)
		xEventGroupSetBits(g_chat_events, EVENT_UART0);
	else
		xEventGroupSetBits(g_chat_events, EVENT_UART4);

	chat(uart);
}

void chat(UART_Type *uart) {
    xEventGroupWaitBits(g_chat_events,
                        EVENT_UART0 | EVENT_UART4,
                        pdTRUE, pdTRUE, portMAX_DELAY);
            escribirchat("\033[10;10H", "\033[2J");

            uint8_t longitud;
            bool bandera = true;
            while(true == bandera){
                escribirP(UART4,"\033[10;10H", "Usuario1: ");
                escribirP(UART0,"\033[10;10H", "Usuario2: ");
                ingresoDatos(uart);
                if(UART0 == uart){
                    longitud = longitud_Queue_TeraTerm();
                    uint8_t g_tipStringT[longitud];
                    for(uint8_t i=0;i<longitud;i++){
                        g_tipStringT[i] = (leerQueue_TeraTerm() );
                        bandera = (ESC == g_tipStringT[i])? false:bandera;
                    }
                    escribirP(UART4,"\033[11;10H", "Usuario2: ");
                    escribirP(UART4,"\033[11;20H", (sint8*)g_tipStringT);
                }
                else{
                    longitud = longitud_Queue_BT();
                    uint8_t g_tipStringB[longitud];
                    for(uint8_t i=0;i<longitud;i++){
                        g_tipStringB[i] = (leerQueue_BT() );
                        bandera = (escBT == g_tipStringB[i])? false:bandera;
                    }
                    escribirP(UART0,"\033[11;10H", "Usuario1: ");
                    escribirP(UART0,"\033[11;20H", (sint8*)g_tipStringB);
                }
                vTaskDelay(pdMS_TO_TICKS(20));
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

void Fecha_Hora()
{
    uint8 string1[] = "  Hora"; /*! String to be printed in the LCD*/
    uint8 string3[] = "  Fecha";



    while (1)
    {
        limpiar_lcd();
        imprimir_lcd(string1, 2, 0);
        imprimir_lcd(getTime(), 2, 1);
        imprimir_lcd(string3, 2, 2);
        imprimir_lcd(getTime(), 2, 3);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
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
	else if (variable >= 48 && variable <= 57)
		variable = variable - 48;

	return variable ;
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

bool getShowTime()
{
    return show_time;
}

bool getShowDate()
{
    return show_date;
}
