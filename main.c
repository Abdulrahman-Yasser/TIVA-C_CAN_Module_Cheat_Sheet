

/**
 * main.c
 */

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "driverlib/can.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"



#include "CAN/Static/CAN.h"

#include "CAN/Dynamic/CAN_Cfg.h"

void SystemInit(void);

static bool g_bTransmitNow = false;


void Transmit_simple_data(void){
    static int j = 8;

    tCANMsgObject TxMsg;

    uint8_t my_Tx_Data[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    TxMsg.pui8MsgData = my_Tx_Data;
    TxMsg.ui32MsgID = 0x32;
    TxMsg.ui32MsgIDMask = 0xff;
    TxMsg.ui32MsgLen = 8;
    TxMsg.ui32Flags = MSG_OBJ_NO_FLAGS;


    my_Tx_Data[0] = j++;
    my_Tx_Data[1] = j++;
    my_Tx_Data[2] = j++;
    my_Tx_Data[3] = j++;
    my_Tx_Data[4] = j++;
    my_Tx_Data[5] = j++;
    my_Tx_Data[6] = j++;
    my_Tx_Data[7] = j++;
    CANMessageSet(CAN0_BASE, 2, &TxMsg, MSG_OBJ_TYPE_TX); // send as msg object 1
}

void Transmit_simple_remote(void){

    tCANMsgObject TxMsg;

    TxMsg.ui32MsgID = 0x32;
    TxMsg.ui32MsgIDMask = 0xff;
    TxMsg.ui32MsgLen = 8;
    TxMsg.ui32Flags = MSG_OBJ_NO_FLAGS;

    CANMessageSet(CAN0_BASE, 2, &TxMsg, MSG_OBJ_TYPE_TX_REMOTE); // send as msg object 1
}

void Receive_any_data(void){

    tCANMsgObject RxMsg;
    RxMsg.ui32MsgID = 0x00;
    RxMsg.ui32MsgIDMask = 0x00;
    RxMsg.ui32MsgLen = 8;
    RxMsg.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;

    CANMessageSet(CAN0_BASE, 1, &RxMsg, MSG_OBJ_TYPE_RX); // send as msg object 1
}

void Receive_ID_filter_data(void){

    tCANMsgObject RxMsg;

    RxMsg.ui32MsgID = 0x32;
    RxMsg.ui32MsgIDMask = 0xff;
    RxMsg.ui32MsgLen = 8;
    RxMsg.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;

    CANMessageSet(CAN0_BASE, 1, &RxMsg, MSG_OBJ_TYPE_RX); // send as msg object 1
}

void Receive_remote_frame(void){

    tCANMsgObject RxMsg;

    RxMsg.ui32MsgID = 0x44;
    RxMsg.ui32MsgIDMask = 0xff;
    RxMsg.ui32MsgLen = 8;
    RxMsg.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;

    CANMessageSet(CAN0_BASE, 1, &RxMsg, MSG_OBJ_TYPE_RX_REMOTE); // send as msg object 1
}

void Receive_remote_frame_AutoResponse(void){

    tCANMsgObject RxMsg;

    RxMsg.ui32MsgID = 0x32;
    RxMsg.ui32MsgIDMask = 0xff;
    RxMsg.ui32MsgLen = 8;
    RxMsg.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;

    CANMessageSet(CAN0_BASE, 1, &RxMsg, MSG_OBJ_TYPE_RXTX_REMOTE); // send as msg object 1
}


int main(void)
{
    int i = 0;
    SystemInit();

    tCANMsgObject my_RxMsg;
    uint8_t my_data[8] = {2, 2, 2, 2, 2, 2, 2, 2};
    CAN_my_Init(CAN_getConfig());
    my_RxMsg.pui8MsgData = my_data;

    // Initializing the Message object for reading
//    Receive_ID_filter_data();
    Receive_any_data();

    while(1){
        Receive_remote_frame();
//        Transmit_simple_data();
//        for(i = 0; i < 3500000; i++);
//        Transmit_simple_remote();
//        for(i = 0; i < 3500000; i++);
        if(g_bRXFlag_RX1 == 1){
            g_bRXFlag_RX1 = 0;
            CANMessageGet(CAN0_BASE, 1, &my_RxMsg, 0);
            if(my_RxMsg.pui8MsgData[0] == 1){
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
            }else if(my_RxMsg.pui8MsgData[0] == 0){
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
            }
            if(my_RxMsg.ui32Flags & MSG_OBJ_REMOTE_FRAME){
                Transmit_simple_data();
                for(i = 0; i < 100; i++);
            }
        }
    }
}


void SystemInit(void){
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);


    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    /* Check if the peripheral access is enabled. */
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }


    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    /* Set GPIO_PORTF_BASE, GPIO_PIN_1 high */
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);




    HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

    /* enable interrupt to portf */
    IntEnable(INT_GPIOF);

}

void GPIOFIntHandler(void)
{


    /*check if interrupt is by SW1*/
    if (GPIOIntStatus(GPIO_PORTF_BASE,0) & GPIO_PIN_4)
    {
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
        g_bTransmitNow = true;
    }
    /* check if interrupt causes by PF0/SW2 */
    else if (GPIOIntStatus(GPIO_PORTF_BASE,0) & GPIO_PIN_0)
    {
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);
        g_bTransmitNow = true;
    }
    else
    {
        /*do nothing*/
    }
}
