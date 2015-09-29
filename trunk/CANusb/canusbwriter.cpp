#include "canthread.h"
#include "threadglobals.h"
#include <iostream>
#include "string.h"

using namespace std;


CANusbWriter::CANusbWriter(FT_HANDLE handle)
{
    ftHandle = handle;
}

void CANusbWriter::sendCommand(char* cmdToSend)
{
    FT_STATUS   ftStatus;
    DWORD       nBytesWrote;

    global::replyMutex.lock();

    cout << cmdToSend << endl;
    if ( FT_OK != ( ftStatus = FT_Write( ftHandle, cmdToSend, strlen(cmdToSend), &nBytesWrote ) ) ){
        cerr << "Failed to write command\nError:" << ftStatus << endl;
        return;
    }

    cout << "Waiting for reply" << endl;

    // Wait and verify the reply
    cout << "Before reply Condition " << endl;
    global::replyCondition.wait(&global::replyMutex);
    cout << "After reply Condition" << endl;
    if ( 0 != global::replyMessage.compare("\r") ) {
        cerr << "Bad Reply" << endl;
        global::replyMutex.unlock();
        return;
    }
    cout << "Got a reply" << endl;
    delete cmdToSend;
    global::replyMutex.unlock();
}

void CANusbWriter::logCanMsg(CANMsg msg)
{
    msgToFile.writeCANMsg(msg);
}

void CANusbWriter::closeLogFile()
{
    msgToFile.csvFile.close();
}

void CANusbWriter::openLogFile()
{
    msgToFile.csvFile.open ("output.csv", ios::out | ios::app);
}
