#include <QtCore/QString>
#include <QtTest/QtTest>
#include <string.h>
#include "canmsgdecode.h"


class CANmsgDecode_Test : public QObject
{
    Q_OBJECT
    
public:
    CANmsgDecode_Test();
    
private Q_SLOTS:
    void twoIntegersHigh();
    void twoIntegersLow();
    void twoFloatsHigh();
    void twoFloatsLow();
    void stringLow();

private:
    CanMsgDecode decoder;
};

CANmsgDecode_Test::CANmsgDecode_Test()
{
}

void CANmsgDecode_Test::twoIntegersHigh()
{
    CANMsg sendMsg;
    DecodedCanMsg decodedMsg;
    int high = 55, low = 10;    // High and low integer values

    // Create the CANMsg to decode
    sendMsg.id = 0x504;
    sendMsg.len = 8;
    memcpy(&sendMsg.data, &high, 4);
    memcpy(&sendMsg.data[4], &low, 4);

    // Decode the Message
    decodedMsg = decoder.decodeCANMsg(sendMsg);


    QCOMPARE(decodedMsg.decodedHigh.toInt(), high);
}

void CANmsgDecode_Test::twoIntegersLow()
{
    CANMsg sendMsg;
    DecodedCanMsg decodedMsg;
    int high = 55, low = 10;    // High and low integer values

    // Create the CANMsg to decode
    sendMsg.id = 0x504;
    sendMsg.len = 8;
    memcpy(&sendMsg.data, &high, 4);
    memcpy(&sendMsg.data[4], &low, 4);

    // Decode the Message
    decodedMsg = decoder.decodeCANMsg(sendMsg);

    QCOMPARE(decodedMsg.decodedLow.toInt(), low);
}

void CANmsgDecode_Test::twoFloatsHigh()
{
    CANMsg sendMsg;
    DecodedCanMsg decodedMsg;
    float high = 55.310, low = 10.787;    // High and low integer values

    // Create the CANMsg to decode
    sendMsg.id = 0x501;
    sendMsg.len = 8;
    memcpy(&sendMsg.data, &high, 4);
    memcpy(&sendMsg.data[4], &low, 4);

    // Decode the Message
    decodedMsg = decoder.decodeCANMsg(sendMsg);


    QCOMPARE(decodedMsg.decodedHigh.toFloat(), high);
}

void CANmsgDecode_Test::twoFloatsLow()
{
    CANMsg sendMsg;
    DecodedCanMsg decodedMsg;
    float high = 55.310, low = 10.787;    // High and low integer values

    // Create the CANMsg to decode
    sendMsg.id = 0x501;
    sendMsg.len = 8;
    memcpy(&sendMsg.data, &high, 4);
    memcpy(&sendMsg.data[4], &low, 4);

    // Decode the Message
    decodedMsg = decoder.decodeCANMsg(sendMsg);

    QCOMPARE(decodedMsg.decodedLow.toFloat(), low);
}

void CANmsgDecode_Test::stringLow()
{
    CANMsg sendMsg;
    DecodedCanMsg decodedMsg;
    int high = 55;
    char low[5] = "SCAN";

    // Create the CANMsg to decode
    sendMsg.id = 0x500;
    sendMsg.len = 8;
    memcpy(&sendMsg.data, &high, 4);
    memcpy(&sendMsg.data[4], &low, 4);

    // Decode the Message
    decodedMsg = decoder.decodeCANMsg(sendMsg);

    QCOMPARE(decodedMsg.decodedLow, QString(low));
}

QTEST_APPLESS_MAIN(CANmsgDecode_Test)

#include "tst_canmsgdecode_test.moc"

