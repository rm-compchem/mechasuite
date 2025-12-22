#include "multichoicedialog2.h"

MultiChoiceDialog2::MultiChoiceDialog2(QWidget *parent, QStringList itms)
{
   int col = 0, row = 0;
    for(unsigned int i=0;i<itms.size();i++){
        items.append(itms[i]);
        QCheckBox *chb = new QCheckBox(this);
        chb->setText(itms[i]);
        connect(
                    chb, &QCheckBox::stateChanged,
                    this, &MultiChoiceDialog2::on_chb_stateChanged
                    );
        checkboxes.push_back(chb);
        layout->addWidget(chb, row, col);
        col ++;
        if(col>=4){
            col = 0;
            row ++;
        }
    }
    row++;
//    col = 0;

    sublayout->addWidget(okbutton);
    sublayout->addWidget(cancelbutton);
    layout->addLayout(sublayout, row, 0, row, 4);
    setLayout(layout);

    connect(
                okbutton, &QPushButton::clicked,
                this, &MultiChoiceDialog2::on_okbutton_clicked
                );
    connect(
                cancelbutton, &QPushButton::clicked,
                this, &MultiChoiceDialog2::on_cancelbutton_clicked
                );
}



void MultiChoiceDialog2::on_okbutton_clicked(){
    ok = true;
    close();
}

void MultiChoiceDialog2::on_cancelbutton_clicked(){
    ok = false;
    selectedItems.clear();
    close();
}

void MultiChoiceDialog2::on_chb_stateChanged(int state){
    for(unsigned int i=0;i<checkboxes.size();i++){
        if(checkboxes[i]->checkState()==2 && !selectedItems.contains(items[i])){
            selectedItems.append(items[i]);
        }
        else if(checkboxes[i]->checkState()==0 && selectedItems.contains(items[i])){
            selectedItems.removeOne(items[i]);
        }
    }
    update();
}
