#ifndef CANUSB_H
#define CANUSB_H

#include <QObject>
#include "canthread.h"

/*! @brief Handles all interaction with the
*   CAN usb device through the read and write
*   threads.
*/
class CANusb : public QObject
{
    Q_OBJECT
public:
    explicit CANusb(QObject *parent = 0);
    
signals:
    void newCommand(char* cmd);     //!< Signal emmited when a new command is ready to be sent.
    void newCANmsg(CANMsg msg);     //!< Signal emmited when a new message has been recieved.
    
public slots:
    bool connectCANusb();
    bool disconnectCANusb();
    void openCanPort();
    void closeCanPort();
    bool setTimestamp(int t);
    bool setBitRate(int bitrate);
    void sendCANmsg(CANMsg msg);
    void closeLogFile();
    void openLogFile();

private:
    bool readVersionInfo();
    bool readSerialNumber();

    FT_HANDLE       ftHandle;       //!< Handle used for communicating with a specific device.
    CANusbReader    *canReader;     //!< The thread for reading messages from the CAN usb device.
    QThread         *writeThread;   //!< The thread that contains canWriter.
    CANusbWriter    *canWriter;     //!< The object for sending commands to the CAN usb device.
};



#endif // CANUSB_H
