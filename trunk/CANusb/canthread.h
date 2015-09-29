#ifndef CANTHREAD_H
#define CANTHREAD_H

#include <QObject>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QQueue>
#include "ftd2xx.h"
#include "CANtoFile/cantofile.h"

// Message flags
#define CANMSG_EXTENDED   0x80 //!< Extended CAN id
#define CANMSG_RTR        0x40 //!< Remote frame

// Used for the readFrame function
#define CANUSB_STATE_NONE  0
#define CANUSB_STATE_MSG   1
#define CANUSB_STATE_REPLY 2

#define MAX( a, b )( ( (a) > (b) ) ? (a) : (b) )
#define MIN( a, b )( ( (a) < (b) ) ? (a) : (b) )
#define ABS( a )(( (int) (a) < 0 ) ? ((a) ^ 0xffffffff) + 1 : (a) )

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


/*! @brief Thread to read all CAN messages from the
*   CAN usb device.
*/
class CANusbReader : public QThread
{
    Q_OBJECT
public:
    CANusbReader(FT_HANDLE handle);
    void stop();

protected:
    void run();

private:
    volatile bool   stopped;
    FT_HANDLE       ftHandle;       //!< Handle used for communicating with a specific device.

    bool readFrame(unsigned long bytesToRead);
    bool canusbToCanMsg(char *p, CANMsg *pMsg);

signals:
    void newReplyMessage(QString reply);    //!< Signals a new reply to a sent command.
    void newCANmsg(CANMsg newMsg);          //!< Signals a new CAN message read in.

public slots:

private:
};

/*! @brief Sends commands to the CAN usb device.
*/
class CANusbWriter : public QObject
{
    Q_OBJECT

public:
    CANusbWriter(FT_HANDLE handle);
    FT_HANDLE ftHandle;         //!< Handle used for communicating with a specific device.

private:
    CanToFile       msgToFile;  //!< Object for logging messages.

signals:

public slots:
    void sendCommand(char* cmd);
    void logCanMsg(CANMsg msg);
    void closeLogFile();
    void openLogFile();
};

#endif // CANTHREAD_H
