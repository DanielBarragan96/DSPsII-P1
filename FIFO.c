#include "MK64F12.h"
#include "FIFO.h"

//FIFO's flags
static bool full = false;
static bool empty = true;
//Current index of the FIFO
static int8_t FIFOIndex = 0;
//FIFO array
static int8_t FIFO[FIFO_LENGHT];

int8_t pop ()
{
    if (empty)
        return false; //if the FIFO is empty we cannot pop
    uint16_t index = FIRST_ELEMENT; //the first element of the FIFO
    int8_t popValue = FIFO[index];
    FIFO[index] = (int8_t) FIRST_ELEMENT; //erase the first element of the FIFO
    for (index; index < FIFOIndex; index)
    { //Move the upper FIFO chars to the left
        FIFO[index] = FIFO[++index];
    }
    FIFOIndex--;
    if (false == FIFOIndex)
    { //If the FIFOindex is cero, it's empty
        empty = true;
        full = false;
    }
    return popValue;
}

bool push (int8_t newVal)
{
    if (FIFO_LENGHT <= FIFOIndex)
    { //check if FIFO it's full
        full = true;
        return false;
    }

    FIFO[FIFOIndex++] = newVal; //add new value to the FIFO
    empty = false; //The FIFO isn't empty because we just added a new value
    return true;
}

int8_t* getFIFO ()
{
    return &FIFO[FIRST_ELEMENT];
} //return the first element of FIFO direction

bool clearFIFO ()
{
    uint8_t eraseIndex = 0;
    while (FIFOIndex >= eraseIndex)
        FIFO[eraseIndex++] = 0; //over write the FIFO values to zero
    //reset variables to initial condition
    FIFOIndex = 0;
    empty = true;
    full = false;
    return true;
}

int8_t getFIFOIndex ()
{
    return FIFOIndex;
}	//return FIFOIndex value
