#ifndef ADDTABDIALOG_H
#define ADDTABDIALOG_H

#include <QDialog>
#include <QtGui>
#include "CANmsgDecode/canmsgdecode.h"

/*! @brief Dialog to choose messages to filter for a new tab.
*
*   Shows a dialog containing a list of all message
*   types from the csv file.
*/
class AddTabDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddTabDialog(QWidget *parent = 0);
    QVector<unsigned long> getChecked();

    QGroupBox *gbx;
    QDialogButtonBox *buttons;
    QVector<QCheckBox*> cboxContainer;
    QVector<unsigned long> checked;
    ~AddTabDialog();
    
private:
};

#endif // ADDTABDIALOG_H
