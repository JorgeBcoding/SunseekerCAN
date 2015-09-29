#ifndef CAN_TO_FILE_H
#define CAN_TO_FILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <time.h>

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

using namespace std;

/*! @brief Logs incoming CAN messages to a csv file.
*/
class CanToFile
{
public:
    CanToFile();
    ~CanToFile();
    int writeCANMsg(CANMsg msg);
    ofstream csvFile;           //!< CSV file all messages are logged to.

private:

};

#endif
