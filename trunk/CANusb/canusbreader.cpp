#include "canthread.h"
#include <iostream>
#include "stdio.h"
#include "threadglobals.h"


using namespace std;

//!Mutexes and Data for CANusb threads.
namespace global{
QWaitCondition  replyCondition;
QMutex          replyMutex;
QString         replyMessage;   //!<Reply messages to verify
}

CANusbReader::CANusbReader(FT_HANDLE handle)
{
    ftHandle = handle;
    stopped = false;
}

void CANusbReader::run()
{

    FT_STATUS ftStatus;
    unsigned long nRxCnt, nTxCnt, eventStatus;
    nRxCnt = 0;
    while (!stopped) {
        while( 0 == nRxCnt ){	// While there is nothing to read

            if ( FT_OK != (ftStatus = FT_GetStatus( ftHandle, &nRxCnt, &nTxCnt, &eventStatus ) ) ){
                cerr << "Error getting status: " << ftStatus <<"\nRead Thread Closing"<< endl;
                stop();
                break;
            }

            if ( 0 == nRxCnt ){
                msleep(100);
            }
        }
        readFrame(nRxCnt);
        nRxCnt = 0;
    }
}

void CANusbReader::stop()
{
    stopped = true;
}

bool CANusbReader::readFrame(unsigned long bytesToRead)
{
    FT_STATUS ftStatus;
    CANMsg msg;
    unsigned int i;
    char *gbufferRx;
    unsigned long nRxCnt;
    unsigned long nRcvCnt;
    char c;

    static char msgReceiveBuf[100];
    static int cntMsgRcv = 0;
    static int state = CANUSB_STATE_NONE;

    // Set buffer to number of bytes to read
    nRxCnt = bytesToRead;
    gbufferRx = new char[nRxCnt];

    // Must fit to buffer
    if ( nRxCnt > sizeof( gbufferRx ) ) {
        nRxCnt = sizeof( gbufferRx );
    }

    // Read data
    if ( FT_OK != ( ftStatus = FT_Read( ftHandle, gbufferRx, nRxCnt, &nRcvCnt ) ) ){
        cerr << "Read error1.1: " << ftStatus << endl;
        delete gbufferRx;
        return false;
    }

    for ( i=0; i < nRcvCnt; i++ ) {

        // Get the next character
        c = gbufferRx[ i ];

        if ( CANUSB_STATE_NONE == state ) {

            // If this is a can message
            if ( ( 't' == c ) || ( 'T' == c ) || ( 'r' == c ) || ( 'R' == c ) ) {
                state = CANUSB_STATE_MSG;
                memset( msgReceiveBuf, 0, sizeof( msgReceiveBuf ) );
                msgReceiveBuf[ 0 ] = c;
                cntMsgRcv = 1;
            }

            // If not it is a reply from a CANusb command
            else {
                state = CANUSB_STATE_REPLY;
                memset( msgReceiveBuf, 0, sizeof( msgReceiveBuf ) );
                msgReceiveBuf[ 0 ] = c;
                cntMsgRcv = 0;
            }
        }


        else if ( CANUSB_STATE_MSG == state ) {
            msgReceiveBuf[ cntMsgRcv++ ] = c;
            // THIS IS A COMPLETE CAN MESSAGE
            if ( 0x0d == c ){
                // Send raw message to convert to CAN msg
                if ( !canusbToCanMsg( msgReceiveBuf, &msg ) ) {
                    cerr << "Message conversion failed!" << endl;
                    state = CANUSB_STATE_NONE;
                    delete (gbufferRx);
                    return false;
                }

                // Convert message and signal its done,
                // add to queue or something like that.
                msgReceiveBuf[ cntMsgRcv++ ] = 0;

                emit newCANmsg(msg);

                // Set the readFrame state to NONE
                state = CANUSB_STATE_NONE;
            }
        } //END COMPLETE CAN MSG

        if ( CANUSB_STATE_REPLY == state ){
            cout << "Any state message?" << endl;
            msgReceiveBuf[ cntMsgRcv++ ] = c;
            if ( 0x0d == c || 0x07 == c ) {	// If c is a [CR] or [BELL] we are at the end of the reply messagre
                int n = strlen(msgReceiveBuf);

                // Return the message
                QString reply = msgReceiveBuf;
                reply.resize(n);

                global::replyMutex.lock();
                global::replyMessage = reply;
                global::replyCondition.wakeAll();
                global::replyMutex.unlock();

                state = CANUSB_STATE_NONE;
            }
        }
    }//End of all characters

    delete gbufferRx;
    return true;
}

//*********** CANUSB TO CANMSG *******************************
// Parses the buffer of bits ( *p ) into CANmsg format.
//
// Copied from the C example.  Need to convert to C++ code and
// try to understand better.
//************************************************************
bool CANusbReader::canusbToCanMsg(char *p, CANMsg *pMsg)
{
    int val;
    int i;
    short data_offset;   // Offset to dlc byte
    char save;

    if ( 't' == *p ) {
        // Standard frame
        pMsg->flags = 0;
        data_offset = 5;

        pMsg->len = p[ 4 ] - '0';
        p[ 4 ] = 0;
        sscanf( p + 1, "%lx", &pMsg->id  );
    }
    else if ( 'r' == *p ) {
        // Standard remote  frame
        pMsg->len = p[ 4 ] - '0';
        pMsg->flags = CANMSG_RTR;
        //data_offset = 5 - 1;// To make timestamp work
        data_offset = 5;
        //save = p[ 4 ];
        p[ 4 ] = 0;
        sscanf( p + 1, "%lx", &pMsg->id  );
        //p[ 4 ] = save;
    }
    else if ( 'T' == *p ) {
        // Extended frame
        pMsg->flags = CANMSG_EXTENDED;
        data_offset = 10;
        pMsg->len = p[ 9 ] - '0';
        p[ 9 ] = 0;
        sscanf( p + 1, "%lx", &pMsg->id );
    }
    else { //( 'R' == *p )
        // Extended remote frame
        pMsg->flags = CANMSG_EXTENDED | CANMSG_RTR;
        //data_offset = 10 - 1;// To make timestamp work
        data_offset = 10;
        pMsg->len = p[ 9 ] - '0';
        //save = p[ 9 ];
        p[ 9 ] = 0;
        sscanf( p + 1, "%lx", &pMsg->id );
        //p[ 9 ] = save;
    }

    save = *(p + data_offset + 2 * pMsg->len );

    // Fill in data
    if ( !( pMsg->flags & CANMSG_RTR ) ) {
        for ( i= MIN( pMsg->len, 8); i > 0; i-- ) {
            *(p + data_offset + 2 * (i-1) + 2 )= 0;
            sscanf( p + data_offset + 2 * (i-1), "%x", &val );
            pMsg->data[ i - 1 ] = val;
        }
    }

    *(p + data_offset + 2 * pMsg->len ) = save;

    if ( !( pMsg->flags & CANMSG_RTR ) ) {
        // If timestamp is active - fetch it
        if ( 0x0d != *( p + data_offset + 2 * pMsg->len ) ) {
            p[ data_offset + 2 * ( pMsg->len ) + 4 ] = 0;
            sscanf( ( p + data_offset + 2 * ( pMsg->len ) ), "%x", &val );
            pMsg->timestamp = val;
        }
        else {
            pMsg->timestamp = 0;
        }
    }
    else {

        if ( 0x0d != *( p + data_offset ) ) {
            p[ data_offset + 4 ] = 0;
            sscanf( ( p + data_offset ), "%x", &val );
            pMsg->timestamp = val;
        }
        else {
            pMsg->timestamp = 0;
        }
    }

    return TRUE;
}




















