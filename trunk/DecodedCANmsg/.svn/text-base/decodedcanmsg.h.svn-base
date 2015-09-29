#ifndef DECODED_CAN_MSG
#define DECODED_CAN_MSG

#include <QString>
#include <iostream>
#include <string>

using namespace std;

#ifndef CANMSG
#define CANMSG
/*! @brief A Raw CAN message.

*/
struct CANMsg {
    unsigned long id;           //!< Message ID.
    unsigned long timestamp;    //!< Message Timestamp.
    unsigned char flags;        //!< Flags.
    unsigned char len;          //!< Length of the data.
    unsigned char data[8];      //!< The raw can data.
};
#endif

/*! @brief Class to hold a decoded CAN message.
*/
class DecodedCanMsg
{

public:
    DecodedCanMsg();
    DecodedCanMsg(CANMsg msg);
    DecodedCanMsg(QString high, QString low, CANMsg msg);
    ~DecodedCanMsg();

    void setDecodedFull(QString decoded);
    QString toQString();

    CANMsg 	canMsg;             //!<The raw CAN message.
    QString	decodedHigh;        //!<The high data.
    QString	decodedLow;         //!<The low data.
    QString    decodedFull;     //!<Full information about message including information.

private:


};

#endif
