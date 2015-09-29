#ifndef SUNSEEKERCAN_H
#define SUNSEEKERCAN_H

#include <QMainWindow>
#include <map>
#include "ui_sunseekercan.h"
#include "DecodedCANmsg/decodedcanmsg.h"
#include "CANmsgDecode/canmsgdecode.h"

/*! @brief The main window for the Sunseeker
*   CAN Analyzer.
*/
class SunseekerCan : public QMainWindow
{
    Q_OBJECT
    
public:
    SunseekerCan(QWidget *parent = 0);
    ~SunseekerCan();

private slots:
    void toggleTimestamp();
    void clearData();
    void addCANmsg (CANMsg msg);
    void openCANport();
    void clearMessageList();
    void sendCANmessage();
    void saveCanLog();
    void createSunseekerTable();
    void on_btnAddTab_clicked();
    void createTab(unsigned long filter);
    void createTab(QVector<unsigned long> filters);

private:
    Ui::MainWindow ui;          //!< The UI class for our main window.
    CanMsgDecode decoder;       //!< For decoding raw CAN messages.
    QHash<unsigned long, QVector<QTableWidget*>* > filteredMessages;    //!< Hash map for filtering.
};

#endif // SUNSEEKERCAN_H
