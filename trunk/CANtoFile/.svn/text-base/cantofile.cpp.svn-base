#include "cantofile.h"
#include <sstream>
#include <iomanip>
#include <string.h>

using namespace std;

/*********** CONSTRUCTOR ********************************//**
*   @brief Opens the default log file.
************************************************************/
CanToFile::CanToFile(){
    csvFile.open ("output.csv", ios::out | ios::trunc);
}

/*********** DECONSTRUCTOR ******************************//**
*   @brief Flushes and closes the log file.
************************************************************/
CanToFile::~CanToFile(){
    csvFile.flush();
    csvFile.close();
}

/*********** WRITE CAN MSG ******************************//**
*   @brief Writes a CAN message to the csv log file.
*   @param msg Raw CAN message to write to log.
************************************************************/
int CanToFile::writeCANMsg(CANMsg msg) 
{
    ostringstream oss;
    int high, low;
    memcpy(&low, &msg.data[4], 4);
    memcpy(&high, &msg.data, 4);

    oss << hex << msg.id << "," << msg.timestamp << ",";
    oss << setw(8) << setfill('0') << low;
    oss << setw(8) << setfill('0') << high;
    oss << endl;


    csvFile << oss.str();
    return 0;
}
