#include "MK64F12.h"
#include "FIFO.h"
#include "States.h"
#include "PCF8563.h"
#include "TeraTerm.h"

//This structure is used for controlling the menus shown in the serial port
//Each menu has up to 3 sub stages which are changed whenever controlSystem is used
const StateType FSM_Moore[] =
{
{ &printTTMainMenu, &controlMenu, &noFunction, &noFunction },
{ &printTTMenuRMemI2C_1, &printTTMenuRMemI2C_2, &printTTMenuRMemI2C_3,
        &noFunction },
{ &printTTMenuWMemI2C_1, &printTTMenuWMemI2C_2, &printTTMenuWMemI2C_3,
        &noFunction },
{ &printTTMenuSetH_1, &printTTMenuSetH_2, &noFunction, &noFunction },
{ &printTTMenuSetD_1, &printTTMenuSetD_2, &noFunction, &noFunction },
{ &printTTMenuFH_1, &printTTMenuFH_2, &noFunction, &noFunction },
{ &printTTMenuRH_1, &noFunction, &noFunction, &noFunction },
{ &printTTMenuRD_1, &noFunction, &noFunction, &noFunction },
{ &noFunction, &noFunction, &noFunction, &noFunction },
{ &noFunction, &noFunction, &noFunction, &noFunction } };
//This structure handles the current system status
SystemControl currentSystem =
{ PRINCIPAL, 0,
{ 0, 0, 0 },
{ 0, 0, 0 }, 0, FORM12 };

bool noFunction ()
{
    //as we put an index in the limit, control Menu will restart the serial port
    currentSystem.stateIndex = STATE_MACHINE_SIZE;
    controlMenu ();
    return false;
}

bool controlSystem ()
{ //control system alternates the sub functions inside an state of the FSM_Moore
    uint8_t index = (currentSystem.currentStatus - 48); //because currentStatus is in ASCII value, we have to decrease 48
    //according to the index value we control the sub functions of the current FSM_Moore state
    if (CERO == currentSystem.stateIndex)
        FSM_Moore[index].fptrFirst ();
    else if (ONE == currentSystem.stateIndex)
        FSM_Moore[index].fptrSecond ();
    else if (TWO == currentSystem.stateIndex)
        FSM_Moore[index].fptrThird ();
    else if (THREE == currentSystem.stateIndex)
        FSM_Moore[index].fptrFour ();

    currentSystem.stateIndex++;	//update the sub function index for the next lap
    return true;
}

bool controlMenu ()
{
    if (STATE_MACHINE_SIZE <= currentSystem.stateIndex)
    {	//if we overpassed the state machine size
        currentSystem.stateIndex = 0;//restore the initial configuration of the screen
        currentSystem.currentStatus = PRINCIPAL;
    }
    else if (PRINCIPAL == currentSystem.currentStatus)
    {	//if we are in the main menu select next menu
        if (ONE < getFIFOIndex ())
            noFunction ();	//if the FIFO has more than one element do nothing
        if (R_MEM == (*getFIFO ()))
            currentSystem.currentStatus = R_MEM;
        if (W_MEM == (*getFIFO ()))
            currentSystem.currentStatus = W_MEM;
        if (ST_H == (*getFIFO ()))
            currentSystem.currentStatus = ST_H;
        if (ST_D == (*getFIFO ()))
            currentSystem.currentStatus = ST_D;
        if (FORM_H == (*getFIFO ()))
            currentSystem.currentStatus = FORM_H;
        if (R_H == (*getFIFO ()))
            currentSystem.currentStatus = R_H;
        if (R_D == (*getFIFO ()))
            currentSystem.currentStatus = R_D;
        if (COM_T2 == (*getFIFO ()))
            currentSystem.currentStatus = COM_T2;
        if (ECO == (*getFIFO ()))
            currentSystem.currentStatus = ECO;
        currentSystem.stateIndex = 0;//because we have changed of menu, we have to reset the index
    }

    controlSystem ();	//update the serial port screen
    //clearEnter();//clear enter flag, which is enabled in the UART interruption
    clearFIFO ();	//reset the stored FIFO
    return true;
}

SystemControl* getSystem ()
{
    return &currentSystem;
}	//return currentSstem direction

bool updateSystemTimeDate ()
{	//update the structure values
    currentSystem.currentTime.Seconds = (PCF8563_getSeconds ());
    currentSystem.currentTime.Minutes = (PCF8563_getMinutes ());
    currentSystem.currentTime.Hours = (PCF8563_getHours ());

    currentSystem.currentDate.Years = (PCF8563_getYears ());
    currentSystem.currentDate.Months = (PCF8563_getMonths ());
    currentSystem.currentDate.Days = (PCF8563_getDays ());

    return true;
}

bool setSystemAddress ()
{

    //the address has 16 bits so we need to pop the first
    uint8_t newAddressH1 = pop ();
    uint8_t newAddressH2 = pop ();
    uint8_t newAddressL1 = pop ();
    uint8_t newAddressL2 = pop ();

    //cast the captured value depending if it was a letter or a number
    if (NUMBER_LOW <= newAddressH1 && newAddressH1 <= NUMBER_HIGH)
    {
        newAddressH1 -= NUMBER_LOW;
    }
    else if (newAddressH1 >= UPLETTER_LOW && newAddressH1 <= UPLETTER_HIGH)
    {
        newAddressH1 -= UPLETTER_DIF;
    }
    else if (newAddressH1 >= MINLETTER_LOW && newAddressH1 <= MINLETTER_HIGH)
    {
        newAddressH1 -= MINLETTER_DIF;
    }
    else
    {
        currentSystem.address = ERROR_ADSRESS;
        return false;
    }

    if (newAddressH2 >= NUMBER_LOW && newAddressH2 <= NUMBER_HIGH)
    {
        newAddressH2 -= NUMBER_LOW;
    }
    else if (newAddressH2 >= UPLETTER_LOW && newAddressH2 <= UPLETTER_HIGH)
    {
        newAddressH2 -= UPLETTER_DIF;
    }
    else if (newAddressH2 >= MINLETTER_LOW && newAddressH2 <= MINLETTER_HIGH)
    {
        newAddressH2 -= MINLETTER_DIF;
    }
    else
    {
        currentSystem.address = ERROR_ADSRESS;
        return false;
    }

    if (NUMBER_LOW <= newAddressL1 && newAddressL1 <= NUMBER_HIGH)
    {
        newAddressL1 -= NUMBER_LOW;
    }
    else if (newAddressL1 >= UPLETTER_LOW && newAddressL1 <= UPLETTER_HIGH)
    {
        newAddressL1 -= UPLETTER_DIF;
    }
    else if (newAddressL1 >= MINLETTER_LOW && newAddressL1 <= MINLETTER_HIGH)
    {
        newAddressL1 -= MINLETTER_DIF;
    }
    else
    {
        currentSystem.address = ERROR_ADSRESS;
        return false;
    }

    if (NUMBER_LOW <= newAddressL2 && newAddressL2 <= NUMBER_HIGH)
    {
        newAddressL2 -= NUMBER_LOW;
    }
    else if (newAddressL2 >= UPLETTER_LOW && newAddressL2 <= UPLETTER_HIGH)
    {
        newAddressL2 -= UPLETTER_DIF;
    }
    else if (newAddressL2 >= MINLETTER_LOW && newAddressL2 <= MINLETTER_HIGH)
    {
        newAddressL2 -= MINLETTER_DIF;
    }
    else
    {
        currentSystem.address = ERROR_ADSRESS;
        return false;
    }

    int16_t newAddress = (newAddressH1 << 12);	//shift the upper hex value
    newAddress += (newAddressH2 << 8);	//shift the upper hex value
    newAddress += (newAddressL1 << 4);	//shift the upper hex value
    newAddress += newAddressL2;	//add the low hex value
    currentSystem.address = newAddress;
    return true;
}

bool pauseExcecution ()
{
    currentSystem.stateIndex = STATE_MACHINE_SIZE;
    return true;
}

bool toggleHourFormat ()
{
    currentSystem.hourFormat = (1 & ~(currentSystem.hourFormat)); //update Hour Format
    return true;
}
