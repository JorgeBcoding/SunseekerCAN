#include "addtabdialog.h"

/*********** CONSTRUCTOR *******************************//**
*   @brief Constructor for AddTab dialog.
*
*   Creates the layout and checkboxes for each message
*   type.  Connects buttons to accept and reject slots.
*   @param parent Parent QObject for QT memory handling.
***********************************************************/
AddTabDialog::AddTabDialog(QWidget *parent) :
    QDialog(parent)//,
    //ui(new Ui::AddTab)
{
    //ui->setupUi(this);
    gbx = new QGroupBox(this);
    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QGridLayout *checkboxes = new QGridLayout(this);

    CanMsgDecode *decoder = new CanMsgDecode();

    int i = 0;
    for (map<unsigned long, SunseekerMsg>::iterator it=decoder->mSunseekerInfo.begin(); it!=decoder->mSunseekerInfo.end(); ++it) {
        SunseekerMsg msg = it->second;
        QString qid;
        qid.setNum(msg.id, 16);
        QCheckBox *box = new QCheckBox(qid + " " + msg.lableMsg);
        cboxContainer.push_back(box);
        checkboxes->addWidget(box, (i%10), (i/10));
        i++;
    }

    gbx->setLayout(checkboxes);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(gbx);
    layout->addWidget(buttons);
    setLayout(layout);

    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    delete decoder;
}

/*********** CONSTRUCTOR *******************************//**
*   @brief Returns the message ID's that have been
    selected for filtering.
*   @returns Vector containing Id's of all selected
*   messages.
***********************************************************/
QVector<unsigned long> AddTabDialog::getChecked()
{
    QString str;
    bool ok;
    for (int i = 0; i < cboxContainer.size(); i++) {
        if (!cboxContainer.at(i)->isChecked())
            continue;
        str = cboxContainer.at(i)->text();
        str.remove("0x");
        str.resize(3);
        checked.push_back(str.toULong(&ok, 16));
    }
    return checked;
}


AddTabDialog::~AddTabDialog()
{
    //delete ui;
}
