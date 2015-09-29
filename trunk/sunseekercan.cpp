#include <QFile>
#include <QFileDialog>
#include <stdio.h>
#include <string.h>
#include "sunseekercan.h"
#include "CANusb/canusb.h"
#include "addtabdialog.h"

#define DEBUG

#ifdef DEBUG
    CanToFile file;
#endif

CANusb *usb1;

SunseekerCan::SunseekerCan(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    usb1 = new CANusb(this);

    qRegisterMetaType<CANMsg>("CANMsg");
    // Connections of signals and slots
    connect( ui.btnExit, SIGNAL(clicked()), this, SLOT(close()) );
    connect( ui.btnClearData, SIGNAL(clicked()), this, SLOT(clearData()) );
    connect( ui.chkTimeStamp, SIGNAL(stateChanged(int)), this, SLOT(toggleTimestamp()) );
    connect( usb1, SIGNAL(newCANmsg(CANMsg)), this, SLOT(addCANmsg(CANMsg)) );
    connect( ui.btnOpenClose, SIGNAL(clicked()), this, SLOT(openCANport()) );
    connect( ui.btnClrMsgList, SIGNAL(clicked()), this, SLOT(clearMessageList()) );
    connect( ui.btnSendMsg, SIGNAL(clicked()), this, SLOT(sendCANmessage()) );
    connect( ui.actionSave_Log_as, SIGNAL(triggered()), this, SLOT(saveCanLog()) );


    ui.txtData1->setText("0x01");
    cout << "Setting bit rate" << endl;
    usb1->setBitRate(5);
    cout << "Setting timestamp" << endl;
    ui.chkTimeStamp->setChecked(true);

    createSunseekerTable();

}

SunseekerCan::~SunseekerCan()
{
    if (ui.btnOpenClose->text() == "Close") {
        usb1->closeCanPort();
        ui.btnOpenClose->setText("Open");

        /* //enable the checkboxes when the CAN port is closed
        ui.chkTimeStamp->enabledChange(true);
        ui.chkSlowMode->enabledChange(true);
        */
    }
}

/*********** CLEAR DATA ********************************//**
*   @brief Clears data from the send messages input.
***********************************************************/
void SunseekerCan::clearData()
{
    ui.txtData0->setText("0x00");
    ui.txtData1->setText("0x00");
    ui.txtData2->setText("0x00");
    ui.txtData3->setText("0x00");
    ui.txtData4->setText("0x00");
    ui.txtData5->setText("0x00");
    ui.txtData6->setText("0x00");
    ui.txtData7->setText("0x00");
}

/*********** ADD CAN MSG *******************************//**
*   @brief Decodes CAN message and adds the decoded message
*   to the appropriate tabs.
*   @param msg CAN message to add to tabs.
***********************************************************/
void SunseekerCan::addCANmsg(CANMsg msg)
{
    DecodedCanMsg decodedMsg;
    SunseekerMsg info = decoder.mSunseekerInfo[msg.id];
    QString str;
    int row;

    decodedMsg = decoder.decodeCANMsg(msg);

    row = ui.txtMsgs->rowCount();
    ui.txtMsgs->insertRow(row);
    ui.txtMsgs->setItem(row, 0, new QTableWidgetItem( str.setNum(decodedMsg.canMsg.id, 16) ));
    ui.txtMsgs->setItem(row, 1, new QTableWidgetItem( str.setNum(decodedMsg.canMsg.timestamp, 10) ));
    ui.txtMsgs->setItem(row, 2, new QTableWidgetItem( info.lableHigh ));
    ui.txtMsgs->setItem(row, 3, new QTableWidgetItem( decodedMsg.decodedHigh ));
    ui.txtMsgs->setItem(row, 4, new QTableWidgetItem( info.lableLow ));
    ui.txtMsgs->setItem(row, 5, new QTableWidgetItem( decodedMsg.decodedLow ));
    ui.txtMsgs->scrollToBottom();

    if ( filteredMessages.contains(msg.id) ) {
        QVector<QTableWidget*> *tabs = filteredMessages[msg.id];
        for (int i = 0; i < tabs->size(); i++) {
            QTableWidget *filterTable = tabs->at(i);
            row = filterTable->rowCount();
            filterTable->insertRow(row);
            filterTable->setItem(row, 0, new QTableWidgetItem( str.setNum(decodedMsg.canMsg.id, 16) ));
            filterTable->setItem(row, 1, new QTableWidgetItem( str.setNum(decodedMsg.canMsg.timestamp, 10) ));
            filterTable->setItem(row, 2, new QTableWidgetItem( info.lableHigh ));
            filterTable->setItem(row, 3, new QTableWidgetItem( decodedMsg.decodedHigh ));
            filterTable->setItem(row, 4, new QTableWidgetItem( info.lableLow ));
            filterTable->setItem(row, 5, new QTableWidgetItem( decodedMsg.decodedLow ));
            filterTable->scrollToBottom();
        }
    }
    //Increment Sent Frames (not optimal)
    QString textValue;
    int value;
    textValue = ui.lblRecievedFrames->text();
    value = textValue.toInt();
    ++value;
    ui.lblRecievedFrames->setText(QString::number(value));
}

/*********** TOGGLE TIMESTAMP **************************//**
*   @brief Turns the timestamp on or off depending on the
*   checkbox.
*
*   Note: CAN port must be closed for this to work.
***********************************************************/
void SunseekerCan::toggleTimestamp()
{
    if (ui.chkTimeStamp->isChecked()) {
        usb1->setTimestamp(1);
    }
    else
        usb1->setTimestamp(0);
}

/*********** OPEN CAN PORT *****************************//**
*   @brief Toggles the CAN port open or closed.
***********************************************************/
void SunseekerCan::openCANport()
{
    if (ui.btnOpenClose->text() == "Open") {
        usb1->openCanPort();
        ui.btnOpenClose->setText("Close");

        /* //disable the checkboxes when the CAN port is open
        ui.chkTimeStamp->enabledChange(false);
        ui.chkSlowMode->enabledChange(false);
        */

    }
    else {
        usb1->closeCanPort();
        ui.btnOpenClose->setText("Open");
    }
}

/*********** CLEAR MESSAGE LIST ************************//**
*   @brief Clears all CAN messages from the tabs.
***********************************************************/
void SunseekerCan::clearMessageList()
{
    ui.txtMsgs->clearContents();
    ui.txtMsgs->setRowCount(0);
}

/*********** SEND CAN MESSAGE **************************//**
*   @brief Sends the CAN message the user entered.
*
*   Note: CAN port must be open to send messages.
***********************************************************/
void SunseekerCan::sendCANmessage()
{
    CANMsg msg;
    QString in;
    int asInt;
    bool ok;

    in = ui.txtMsgId->text();
    in.remove("0x");
    msg.id = in.toLong(&ok, 16);

    in = ui.txtData0->text();
    in.remove("0x");
    asInt = in.toInt(&ok, 16);
    memcpy(&msg.data[0], &asInt, 1);

    in = ui.txtData1->text();
    in.remove("0x");
    asInt = in.toInt(&ok, 16);
    memcpy(&msg.data[1], &asInt, 1);

    in = ui.txtData2->text();
    in.remove("0x");
    asInt = in.toInt(&ok, 16);
    memcpy(&msg.data[2], &asInt, 1);

    in = ui.txtData3->text();
    in.remove("0x");
    asInt = in.toInt(&ok, 16);
    memcpy(&msg.data[3], &asInt, 1);

    in = ui.txtData4->text();
    in.remove("0x");
    asInt = in.toInt(&ok, 16);
    memcpy(&msg.data[4], &asInt, 1);

    in = ui.txtData5->text();
    in.remove("0x");
    asInt = in.toInt(&ok, 16);
    memcpy(&msg.data[5], &asInt, 1);

    in = ui.txtData6->text();
    in.remove("0x");
    asInt = in.toInt(&ok, 16);
    memcpy(&msg.data[6], &asInt, 1);

    in = ui.txtData7->text();
    in.remove("0x");
    asInt = in.toInt(&ok, 16);
    memcpy(&msg.data[7], &asInt, 1);

    msg.len = 8;

    usb1->sendCANmsg(msg);

    //Increment Sent Frames (not optimal)
    QString textValue;
    int value;
    textValue = ui.lblSentFrames->text();
    value = textValue.toInt();
    ++value;
    ui.lblSentFrames->setText(QString::number(value));

#ifdef DEBUG
    msg.timestamp = NULL;
    addCANmsg(msg);
    file.writeCANMsg(msg);
#endif

}

/*********** SAVE CAN LOG ******************************//**
*   @brief Saves the default csv log file to a user
*   specified file.
***********************************************************/
void SunseekerCan::saveCanLog()
{
#ifdef DEBUG
    file.csvFile.close();
#endif

    //usb1->closeLogFile();

    QString filename;
    filename = QFileDialog::getSaveFileName(this, "Save As");

    if (QFile::copy("output.csv", filename))
        cout << "copy worked" << endl;

    //usb1->openLogFile();
#ifdef DEBUG
    file.csvFile.open ("output.csv", ios::out | ios::app);
#endif
}

/*********** ADD TAB CLICKED ***************************//**
*   @brief Creates a new filtered tab from AddTabDialog
*   selection.
***********************************************************/
void SunseekerCan::on_btnAddTab_clicked()
{
    AddTabDialog newTab;
    if ( newTab.exec() ) {
        createTab( newTab.getChecked() );
    }
}

/*********** CREATE TAB ********************************//**
*   @brief Creates a new tab for a single message ID.
*   @param filter The ID for the new tab.
***********************************************************/
void SunseekerCan::createTab(unsigned long filter)
{
    QTableWidget *tblNew = new QTableWidget();
    if ( !filteredMessages.contains(filter) )
        filteredMessages[filter] = new QVector<QTableWidget*>();

    SunseekerMsg info = decoder.mSunseekerInfo[filter];
    QString qid;
    qid.setNum(filter, 16);
    tblNew->setColumnCount(4);
    tblNew->setHorizontalHeaderItem(0, new QTableWidgetItem("ID") );
    tblNew->setHorizontalHeaderItem(1, new QTableWidgetItem("Timestamp") );
    tblNew->setHorizontalHeaderItem(2, new QTableWidgetItem(info.lableHigh) );
    tblNew->setHorizontalHeaderItem(3, new QTableWidgetItem(info.lableLow) );
    filteredMessages[filter]->push_back(tblNew);
    ui.tabWidget->addTab(tblNew, "0x" + qid);
}

/*********** CREATE TAB ********************************//**
*   @brief Creates a new tab for multiple CAN ID's.
*   @param filters The Id's for the new tab to display.
***********************************************************/
void SunseekerCan::createTab(QVector<unsigned long> filters)
{
    QTableWidget *tblNew = new QTableWidget();
    QString title;
    QString qid;

    tblNew->setColumnCount(6);
    tblNew->setHorizontalHeaderItem(0, new QTableWidgetItem("ID") );
    tblNew->setHorizontalHeaderItem(1, new QTableWidgetItem("Timestamp") );
    tblNew->setHorizontalHeaderItem(2, new QTableWidgetItem("High Info") );
    tblNew->setHorizontalHeaderItem(3, new QTableWidgetItem("High Data") );
    tblNew->setHorizontalHeaderItem(4, new QTableWidgetItem("Low Info") );
    tblNew->setHorizontalHeaderItem(5, new QTableWidgetItem("Low Data") );

    for (int i = 0; i < filters.size(); i++) {
        qid.setNum(filters.at(i), 16);
        if ( !filteredMessages.contains(filters.at(i)) )
            filteredMessages[filters.at(i)] = new QVector<QTableWidget*>;
        filteredMessages[filters.at(i)]->push_back(tblNew);
        title.append("0x" + qid + " ");
    }
    ui.tabWidget->addTab(tblNew, title);
}

/*********** CREATE SUNSEEKER TABLE ********************//**
*   @brief Creates a tab displaying information about all
*   sunseeker messages from csv file.
***********************************************************/
void SunseekerCan::createSunseekerTable()
{
    QTableWidget *sunMessages = new QTableWidget();
    sunMessages->setColumnCount(6);
    sunMessages->setHorizontalHeaderItem(0,new QTableWidgetItem("ID") );
    sunMessages->setHorizontalHeaderItem(1,new QTableWidgetItem("High Type") );
    sunMessages->setHorizontalHeaderItem(2,new QTableWidgetItem("Low Type") );
    sunMessages->setHorizontalHeaderItem(3,new QTableWidgetItem("High Message") );
    sunMessages->setHorizontalHeaderItem(4,new QTableWidgetItem("Low Message") );
    sunMessages->setHorizontalHeaderItem(5,new QTableWidgetItem("Message Type") );
    ui.tabWidget->addTab(sunMessages, "Sunseeker Messages");



    for (map<unsigned long, SunseekerMsg>::iterator it=decoder.mSunseekerInfo.begin(); it!=decoder.mSunseekerInfo.end(); ++it) {
        SunseekerMsg msg = it->second;
        QString qid;
        int row = sunMessages->rowCount();
        sunMessages->insertRow(row);
        qid.setNum(msg.id, 16);

        sunMessages->setItem(row,0, new QTableWidgetItem(qid) );
        sunMessages->setItem(row,1, new QTableWidgetItem(msg.typeHigh) );
        sunMessages->setItem(row,2, new QTableWidgetItem(msg.typeLow) );
        sunMessages->setItem(row,3, new QTableWidgetItem(msg.lableHigh) );
        sunMessages->setItem(row,4, new QTableWidgetItem(msg.lableLow) );
        sunMessages->setItem(row,5, new QTableWidgetItem(msg.lableMsg) );

    }
}
