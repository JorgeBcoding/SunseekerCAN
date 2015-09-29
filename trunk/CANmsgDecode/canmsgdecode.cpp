#include "canmsgdecode.h"

using namespace std;

/************** CONSTRUCTOR ***********************//**
*   @brief Handles setup.
*
*   Opens the csv file containing message information
*   and calls createSunseekerMap.
******************************************************/
CanMsgDecode::CanMsgDecode()
{
    //Open the sunseeker info file for reading
    fSunseekerInfo = new QFile("sunseeker_msg_info.csv");
    fSunseekerInfo->open(QIODevice::ReadOnly | QIODevice::Text);

    createSunseekerMap();
}

CanMsgDecode::~CanMsgDecode()
{
}

/************** CREATE SUNSEEKER MAP ********//**
*   @brief Creates the map
*   with all the information about
*   the sunseeker device messages.
*
*   Reads the entire csv file and stores the
*   SunseekerMsg for each ID in the hash map
*   indexed by ID.
*************************************************/
void CanMsgDecode::createSunseekerMap()
{
    QTextStream infoIn(fSunseekerInfo);

    while( !infoIn.atEnd() ){
        SunseekerMsg msg;
        QStringList bufferLst;
        QString buffer;

        msg.id = 0;

        buffer =  infoIn.readLine();
        bufferLst = buffer.split(",");

        QString idStr;
        idStr = bufferLst.at(0);
        msg.id = idStr.toInt(0,16);
        msg.typeHigh = bufferLst.at(1);
        msg.typeLow = bufferLst.at(2);
        msg.lableHigh = bufferLst.at(3);
        msg.lableLow = bufferLst.at(4);
        msg.lableMsg = bufferLst.at(5);

        mSunseekerInfo[msg.id] = msg;

    }
}

/************** CAN MSG DECODE ***************//**
*   @brief Converts a CANMsg to a DecodedCanMsg.
*
*   Using the Sunseeker message map, converts the
*   raw data of a CANMsg into the correct data
*   type.  This is then converted to a string
*   and stored as a DecodedCanMsg.
*   @param msg The CAN message to be decoded.
*   @returns A decoded CAN message.
*************************************************/
DecodedCanMsg CanMsgDecode::decodeCANMsg(CANMsg msg)
{
    SunseekerMsg infoMsg;	//For the information about the message
    QString sHighData;
    QString sLowData;

    infoMsg = getMsgInfo(msg.id);


    if ( infoMsg.typeHigh == "float" ){
        float f;
        memcpy(&f, msg.data, 4);
        sHighData.setNum(f);
    }

    else if ( infoMsg.typeHigh == "int" ){
        int i;
        memcpy(&i, msg.data, 4);
        sHighData.setNum(i);
    }

    else if ( infoMsg.typeHigh == "char[4]" ){
        char c[5];
        memcpy(c, msg.data, 4);
        c[4] = '\0';
        sHighData = c;
    }

    if ( infoMsg.typeLow == "float" ){
        float f;
        memcpy(&f, &msg.data[4], 4);
        sLowData.setNum(f);
    }

    else if ( infoMsg.typeLow == "int" ){
        int i;
        memcpy(&i, &msg.data[4], 4);
        sLowData.setNum(i);
    }

    else if ( infoMsg.typeLow == "char[4]" ){
        char c[5];
        memcpy(c, &msg.data[4], 4);
        c[4] = '\0';
        sLowData = c;
    }

    DecodedCanMsg decodedMsg( sHighData, sLowData, msg );
    QString decodedFull;
    QString strId;
    strId.setNum(msg.id, 16);

    decodedFull = "id:" + strId + "\t" + infoMsg.lableMsg + "\n\t"
            + infoMsg.lableHigh + ":" + decodedMsg.decodedHigh + "\n\t"
            + infoMsg.lableLow + ":" + decodedMsg.decodedHigh;

    decodedMsg.setDecodedFull(decodedFull);
    return decodedMsg;
}

/************** CAN MSG DECODE ***************//**
*   @brief Get information about a specific
*   CAN message.
*   @param msgId The ID of the CAN message.
*   @returns SunseekerMsg for specified ID.
*************************************************/
SunseekerMsg CanMsgDecode::getMsgInfo(unsigned long msgId)
{
    return mSunseekerInfo[msgId];
}


