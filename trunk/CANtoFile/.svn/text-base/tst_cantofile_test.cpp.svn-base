#include <QtCore/QString>
#include <QtTest/QtTest>
#include <string.h>
#include "cantofile.h"


class CANtoFile_Test : public QObject
{
    Q_OBJECT
    
public:
    CANtoFile_Test();
    
private Q_SLOTS:
    void twoIntegers();

private:
    CanToFile file;

};

CANtoFile_Test::CANtoFile_Test()
{
}

void CANtoFile_Test::twoIntegers()
{
    CANMsg sendMsg;
    int high = 255, low = 10;    // High and low integer values

    // Create the CANMsg to decode
    sendMsg.id = 0x504;
    sendMsg.len = 8;
    sendMsg.timestamp = 0x200;
    memcpy(&sendMsg.data, &high, 4);
    memcpy(&sendMsg.data[4], &low, 4);

    file.writeCANMsg(sendMsg);

    QCOMPARE(15, high);
}


QTEST_APPLESS_MAIN(CANtoFile_Test)

#include "tst_cantofile_test.moc"

