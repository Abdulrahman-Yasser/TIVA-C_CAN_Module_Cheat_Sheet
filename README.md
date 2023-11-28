# TIVA-C CAN Module Cheat Sheet
This Repo contains a description for How to use CAN ti-ware Library<br>
Each Message ID of the module is independent of the other messaegs. One message can be Tx, the other Rx and another one Tx remote. It's really flexiple.<br><br>
![CAN 1](https://github.com/Abdulrahman-Yasser/TIVA-C_CAN_Module_Cheat_Sheet/assets/63866803/b5f38140-e160-45c9-b4b4-07ad5e999357)<br>
## Let's discuss the ti-ware CAN driver.<br>
### Modes :
#### Each CAN module contains 32 message object. Each message object will only act as one of the below modes.<br>

> MSG_OBJ_TYPE_TX.
>> Initialize the Message object as transmitting a data message.

> MSG_OBJ_TYPE_TX_REMOTE.
>> Initialize the Message object as transmitting a remote message.

> MSG_OBJ_TYPE_RX.
>> Initialize the Message object as Receiving a data message.

> MSG_OBJ_TYPE_RX_REMOTE.
>> Initialize the Message object as Receiving a remote message.

> MSG_OBJ_TYPE_RXTX_REMOTE
>> Initialize the Message object as Receiving a remote message and automatic response with the data stored in the tCANMsgObject variable.

### Message object's Flags :

> MSG_OBJ_TX_INT_ENABLE
>> This indicates that transmit interrupts are enabled.

> MSG_OBJ_RX_INT_ENABLE
>> This indicates that receive interrupts are enabled.

> MSG_OBJ_EXTENDED_ID
>> This indicates that a message object is using an extended identifier.

> MSG_OBJ_USE_ID_FILTER
>> This indicates that a message object is using filtering based on the object's message identifier. Set the 29 bits of Identifier mask that were requested or only set 11 bits.

> MSG_OBJ_NEW_DATA
>> This indicates that new data was available in the message object.

> MSG_OBJ_DATA_LOST
>> This indicates that data was lost since this message object was last read.

> MSG_OBJ_USE_DIR_FILTER
>> This indicates that a message object uses or is using filtering based on the direction of the transfer. If the direction filtering is used, then ID filtering must also be enabled.

> MSG_OBJ_USE_EXT_FILTER
>> This indicates that a message object uses or is using message identifier filtering based on the extended identifier.  If the extended identifier filtering is used, then ID filtering must also be enabled. The caller wants to filter on the message direction field.

> MSG_OBJ_REMOTE_FRAME
>> This indicates that a message object is a remote frame.

> MSG_OBJ_FIFO
>> This indicates that this message object is part of a FIFO structure and not the final message object in a FIFO.

> MSG_OBJ_NO_FLAGS
>> This indicates that a message object has no flags set.


## How to Initialize my Message Object.

### Using the below function to initialize it.

```c
extern void CANMessageSet(uint32_t ui32Base, uint32_t ui32ObjID,
                          tCANMsgObject *psMsgObject, tMsgObjType eMsgType);
```
  1. Transmitting data :
```c
    tCANMsgObject TxMsg;
    uint8_t my_Tx_Data[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    TxMsg.pui8MsgData = my_Tx_Data;
    TxMsg.ui32MsgID = 0x33;
    TxMsg.ui32MsgIDMask = 0xff;
    TxMsg.ui32MsgLen = 8;
    TxMsg.ui32Flags = MSG_OBJ_TX_INT_ENABLE;

    CANMessageSet( CAN0_BASE, 6, TxMsg, MSG_OBJ_TYPE_TX);
```
That was how to initialize and send your first Message over CAN bus
    1. We use that each time we need to transmit a data, just change the datapointer and use the function again like that :
```c
    /* whateveer changes i want to make to the data */
    my_Tx_Data[5] = 22;
    my_Tx_Data[6] = 95;

    CANMessageSet( CAN0_BASE, 6, TxMsg, MSG_OBJ_TYPE_TX);
```
Successfully transmitted data.
    2. Transmit data with Extended ID
```c
    /* whateveer changes i want to make to the data */
    my_Tx_Data[5] = 22;
    my_Tx_Data[6] = 95;

    TxMsg.ui32Flags = MSG_OBJ_TX_INT_ENABLE | MSG_OBJ_EXTENDED_ID | MSG_OBJ_USE_ID_FILTER;
    
    CANMessageSet( CAN0_BASE, 6, TxMsg, MSG_OBJ_TYPE_TX);
```
 2. Transmitting Remote data:
    1. 11-bit ID, and no interrupt
```c
    tCANMsgObject TxMsg;
    uint8_t my_Tx_Data[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    TxMsg.pui8MsgData = my_Tx_Data;
    TxMsg.ui32MsgID = 0x33;
    TxMsg.ui32MsgIDMask = 0xff;
    TxMsg.ui32MsgLen = 8;
    TxMsg.ui32Flags = MSG_OBJ_NO_FLAGS;

    CANMessageSet( CAN0_BASE, 6, TxMsg, MSG_OBJ_TYPE_TX_REMOTE);
```
It will not even check the data of the message structure, it will just transmits a Remote Message
