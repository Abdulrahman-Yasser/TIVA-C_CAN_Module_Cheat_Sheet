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
<ol>
  <li>
      <ol>Transmitting data :
        <li>Simple transmission <br>
This just transmit data and standard ID
          
  ```c
            tCANMsgObject TxMsg;
        
            uint8_t my_Tx_Data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        
            TxMsg.pui8MsgData = my_Tx_Data;
            TxMsg.ui32MsgID = 0x32;
            TxMsg.ui32MsgIDMask = 0xff;
            TxMsg.ui32MsgLen = 8;
            TxMsg.ui32Flags = MSG_OBJ_NO_FLAGS;
        
            CANMessageSet( CAN0_BASE, 6, TxMsg, MSG_OBJ_TYPE_TX);
  ```
That was how to initialize and send your first Message over CAN bus</li>
         <li>Update data and send <br>
This is how to update data and send it

  ```c
    /* whateveer changes i want to make to the data */
    my_Tx_Data[5] = 22;
    my_Tx_Data[6] = 95;

    CANMessageSet( CAN0_BASE, 6, TxMsg, MSG_OBJ_TYPE_TX);
  ```
Successfully transmitted data.<br></li>
      <li>Transmit data with Extended ID <br>
      
  ```c
    /* whateveer changes i want to make to the data */
    my_Tx_Data[5] = 22;
    my_Tx_Data[6] = 95;

    TxMsg.ui32Flags = MSG_OBJ_EXTENDED_ID;
    
    CANMessageSet( CAN0_BASE, 6, TxMsg, MSG_OBJ_TYPE_TX);
  ```
Successfuly updated data and transmitted it with extended ID.</li>
      <li>Transmitting and update data once it finishes Transmission <br>
    
  ```c
    tCANMsgObject TxMsg;

    uint8_t my_Tx_Data[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    TxMsg.pui8MsgData = my_Tx_Data;
    TxMsg.ui32MsgID = 0x32;
    TxMsg.ui32MsgIDMask = 0xff;
    TxMsg.ui32MsgLen = 8;
    TxMsg.ui32Flags = MSG_OBJ_TX_INT_ENABLE; // Will rise an ISR once it finishes transmission in TXOK bit CANSTS register

    CANMessageSet( CAN0_BASE, 6, TxMsg, MSG_OBJ_TYPE_TX);
    CANIntRegister(CAN0_BASE, CANIntHandler);
  ```

It will rise up TxOk bit in CANSTS register. When it's raised, you can put True in a flag and change the data then.</li>
    </ol>
    </li>
    <li>
    <ol>
 Transmitting Remote data:
 <li> 11-bit ID, and no interrupt
    <br>
   
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
It will not even check the data of the message structure, it will just transmits a Remote Message.<br>
If you need to transmit it with Extended ID, just add MSG_OBJ_EXTENDED_ID flag. </li>

</ol>
</li>

<li>
<ol>
  Receiving data<br>
  it better to use Rx ISR
  <li>
    Receiving any standard ID <br>
      This is how to receive anything (not remote nor extended ID) over the Bud.
    
  ```c
    tCANMsgObject RxMsg;
    RxMsg.ui32MsgID = 0x00;
    RxMsg.ui32MsgIDMask = 0x00;    // must be zero to receive any ID
    RxMsg.ui32MsgLen = 8;
    RxMsg.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;

    CANMessageSet(CAN0_BASE, 1, &RxMsg, MSG_OBJ_TYPE_RX); // send as msg object 1
  ```    
  </li>
  <li>
    Receiving Specific standard ID <br>
      This is how to receive anything (not remote nor extended ID) over the Bud
    
  ```c
    tCANMsgObject RxMsg;

    RxMsg.ui32MsgID = 0x32;
    RxMsg.ui32MsgIDMask = 0xff;    // the ID must match your first 8 bits.
    RxMsg.ui32MsgLen = 8;
    RxMsg.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;

    CANMessageSet(CAN0_BASE, 1, &RxMsg, MSG_OBJ_TYPE_RX); // send as msg object 1
  ```    
    
  </li>
  </ol>

<li>
<ol>Receive remote data<br>
using Rx ISR
<li> receive remote <br>
Here we will receive a remote frame, change data, and send it<br>

  ```c
    uint8_t my_8_RemoteDataResponse[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    tCANMsgObject myMsgRemoteResponse;

    myMsgRemoteResponse.ui32Flags =  MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    myMsgRemoteResponse.pui8MsgData = my_8_RemoteDataResponse;
    myMsgRemoteResponse.ui32MsgIDMask = 0;
    myMsgRemoteResponse.ui32MsgID = 0;
    myMsgRemoteResponse.ui32MsgLen = 8;
    CANMessageSet(CAN0_BASE, 1, &myMsgRemoteResponse, MSG_OBJ_TYPE_RX_REMOTE); // It will not reaceive data.
    // we can read it in using function CANStatusGet(CAN0_BASE, CAN_STS_TXREQUEST);

  ```

  </li>
  </ol>

<li>
<ol>Automatic Response for Receive remote data<br>
using Rx ISR
<li> receive remote Automatic response <br>
Here we will receive a remote frame, and automatic response with the array in my tCANMsgObject MsgData variable<br>

  ```c
    uint8_t my_8_AutoResponseRemoteData[8] = {2, 2, 2, 2, 2, 2, 2, 2};
    tCANMsgObject myRcvMsgAutoResponse;

    myRcvMsgAutoResponse.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    myRcvMsgAutoResponse.pui8MsgData = my_8_AutoResponseRemoteData;
    myRcvMsgAutoResponse.ui32MsgIDMask = 0;
    myRcvMsgAutoResponse.ui32MsgID = 0;
    myRcvMsgAutoResponse.ui32MsgLen = 8;

    CANMessageSet(CAN0_BASE, 1, &myRcvMsgAutoResponse, MSG_OBJ_TYPE_RXTX_REMOTE); // It will not reaceive data.
    // we can know once it rise the ISR using the function CANStatusGet(CAN0_BASE, CAN_STS_NEWDAT);
  ```

  </li>
  </ol>

  </li>
  </ol>
  </li>
  </ol>



