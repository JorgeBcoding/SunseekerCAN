#include "decodedcanmsg.h"

using namespace std;

DecodedCanMsg::DecodedCanMsg()
{
}

/*********** CONSTRUCTOR ********************************//**
*   @brief Constructor setting the raw CAN message.
*   @param msg Raw CAN message to store.
************************************************************/
DecodedCanMsg::DecodedCanMsg(CANMsg msg)
{
    canMsg = msg;
}

/*********** CONSTRUCTOR ********************************//**
*   @brief Constructor setting the raw CAN message and
*   decoded low and high strings.
*   @param high String of the high data.
*   @param low String of the low data.
*   @param msg Raw CAN message to store.
************************************************************/
DecodedCanMsg::DecodedCanMsg(QString high, QString low, CANMsg msg)
{
    decodedHigh = high;
    decodedLow = low;
    canMsg = msg;
}

DecodedCanMsg::~DecodedCanMsg()
{
}


/*********** TO QSTRING *********************************//**
*   @brief Returns the full decoded message.
*   @return The full decoded message.
************************************************************/
QString DecodedCanMsg::toQString()
{
    return decodedFull;
}

/*********** SET DECODED FULL ***************************//**
*   @brief Setter function for decodedFull.
*   @param decoded String of the full decoded message.
************************************************************/
void DecodedCanMsg::setDecodedFull(QString decoded)
{
    decodedFull = decoded;
}
