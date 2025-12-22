#include "singlechoicedialog.h"

SingleChoiceDialog::SingleChoiceDialog(QWidget *parent, QStringList itms)
{
    for(unsigned int i=0;i<itms.size();i++){
        items.append(itms[i]);
        QCheckBox *chb = new QCheckBox(this);
        chb->setText(itms[i]);
        connect(
                    chb, &QCheckBox::stateChanged,
                    this, &SingleChoiceDialog::on_chb_stateChanged
                    );
        checkboxes.push_back(chb);
        layout->addWidget(chb);
    }

    sublayout->addWidget(okbutton);
    sublayout->addWidget(cancelbutton);
    layout->addLayout(sublayout);
    setLayout(layout);

    connect(
                okbutton, &QPushButton::clicked,
                this, &SingleChoiceDialog::on_okbutton_clicked
                );
    connect(
                cancelbutton, &QPushButton::clicked,
                this, &SingleChoiceDialog::on_cancelbutton_clicked
                );
}



void SingleChoiceDialog::on_okbutton_clicked(){
    ok = true;
    close();
}

void SingleChoiceDialog::on_cancelbutton_clicked(){
    ok = false;
    selected = "";
    close();
}

void SingleChoiceDialog::on_chb_stateChanged(int state){
    if (state == 2){
        int temp_checked = -1;
        for(unsigned int i=0;i<checkboxes.size();i++){
            if(checkboxes[i]->checkState() == state && i != checked){
                temp_checked = i;
                selected = items[i];
            }
            else{
                checkboxes[i]->setChecked(false);
            }
        }
        checked = temp_checked;
    }
    else{
            checked = -1;
            selected = "";
            for(unsigned int i=0;i<checkboxes.size();i++){
                checkboxes[i]->setChecked(false);
            }
    }
    update();
}

