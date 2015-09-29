#ifndef CAN_MSG_DECODE_H
#define CAN_MSG_DECODE_H

#include <iostream>
#include <fstream>

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QStringList>

#include <sstream>
#include <map>  

#include "DecodedCANmsg/decodedcanmsg.h"

using namespace std;

/*! @struct SunseekerMsg
*   @brief Data type to hold information about the
*   diffrent message types of Sunseeker CAN messages.
*/
struct SunseekerMsg{
    unsigned long id;
    QString typeHigh;
    QString typeLow;
    QString lableHigh;
    QString lableLow;
    QString lableMsg;
};

/*! @brief Translates a binary CAN message
*   into human readable form.
*
*   This class splits the data segment into high
*   and low sections.  Each section is then converted
*   to the appropriate data type specified in the csv
*   file.  String representations of the data are stored
*   in DecodedCanMsg objects.
*/
class CanMsgDecode : public QObject
{
    Q_OBJECT
public:
    CanMsgDecode();
    ~CanMsgDecode();
    SunseekerMsg getMsgInfo(unsigned long msgId);
    DecodedCanMsg decodeCANMsg(CANMsg msg);

    map<unsigned long, SunseekerMsg> mSunseekerInfo; //!< Hash map containing all the Sunseeker Messages.

private:
    QFile *fSunseekerInfo;  //!< CSV file for reading the Sunseeker CAN message information.
    void createSunseekerMap();

};

#endif
