#include "multichoicedialog.h"



MultiChoiceDialog::MultiChoiceDialog(QWidget *parent, QStringList itms) :QDialog(parent)
{

//    for(int i=0;i<itms.size();i++){
//        RCheckBox *checkbox = new RCheckBox(itms.at(i), this);
////        connect(
////                    checkbox, &RCheckBox::stateChanged2,
////                    this, &MultiChoiceDialog2::on_checkbox_stateChanged
////                    );
//        checkboxes.push_back(checkbox);
//        layout->addWidget(checkbox);
//    }

    chb1->setText(itms[0]);
    connect(
                chb1, &QCheckBox::stateChanged,
                this, &MultiChoiceDialog::on_chb1_stateChanged
                );
    chb2->setText(itms[1]);
    connect(
                chb2, &QCheckBox::stateChanged,
                this, &MultiChoiceDialog::on_chb2_stateChanged
                );
    chb3->setText(itms[2]);
    connect(
                chb3, &QCheckBox::stateChanged,
                this, &MultiChoiceDialog::on_chb3_stateChanged
                );
    layout->addWidget(chb1);
    layout->addWidget(chb2);
    layout->addWidget(chb3);
    sublayout->addWidget(okbutton);
    sublayout->addWidget(cancelbutton);
    layout->addLayout(sublayout);
    setLayout(layout);

    connect(
                okbutton, &QPushButton::clicked,
                this, &MultiChoiceDialog::on_okbutton_clicked
                );
    connect(
                cancelbutton, &QPushButton::clicked,
                this, &MultiChoiceDialog::on_cancelbutton_clicked
                );

//    QMetaObject::connectSlotsByName(this);

}


void MultiChoiceDialog::on_okbutton_clicked(){
    ok = true;
    close();
}

void MultiChoiceDialog::on_cancelbutton_clicked(){
    ok = false;
    items.clear();
    close();
}

void MultiChoiceDialog::on_chb1_stateChanged(int state){
    if (state == 2){
        items.append(tr("Selective dynamics"));
    }
    else{
        items.removeOne(tr("Selective dynamics"));
    }
}

void MultiChoiceDialog::on_chb2_stateChanged(int state){
    if (state == 2){
        items.append(tr("Direct Coordinates"));
    }
    else{
        items.removeOne(tr("Direct Coordinates"));
    }
}

void MultiChoiceDialog::on_chb3_stateChanged(int state){
    if (state == 2){
        items.append(tr("VASP 4.x"));
    }
    else{
        items.removeOne(tr("VASP 4.x"));
    }
}

//void MultiChoiceDialog::on_checkbox_stateChanged(QStringList state){
//    if (state[0] == "true"){
//        items.append(state[1]);
//        cout <<state[1].toStdString()<<endl;
//    }
//    else{
//        items.removeOne(state[1]);
//        cout <<state[1].toStdString()<<endl;
//    }
//}





//MultiChoiceDialog::MultiChoiceDialog(QWidget *parent, QStringList itms): QDialog(parent)
//{
//    for(int i = 0; i<itms.size();i++){
//        QStandardItem item(itms.at(i));
//        item.setCheckable(true);
//        item.setEditable(false);
//        QStandardItem *itmp = &item;
//        model->appendRow(itmp);
//    }


//    list->setModel(model);

//    sublayout->addWidget(okbutton);
//    sublayout->addWidget(cancelbutton);
//    layout->addWidget(list);
//    layout->addLayout(sublayout);
//    setLayout(layout);
//    list->setViewMode(QListView::ListMode);
//    list->show();

//    connect(
//                okbutton, &QPushButton::released,
//                this, &MultiChoiceDialog::on_okbutton_released
//                );
//    connect(
//                cancelbutton, &QPushButton::released,
//                this, &MultiChoiceDialog::on_cancelbutton_released
//                );

//    connect(
//                model, &QStandardItemModel::itemChanged,
//                this, &MultiChoiceDialog::on_model_itemChanged
//                );


//}

//void MultiChoiceDialog::on_okbutton_released(){
//    ok = true;
//    close();
//}

//void MultiChoiceDialog::on_cancelbutton_released(){
//    ok = false;
//    items.clear();
//    close();
//}

//void MultiChoiceDialog::on_model_itemChanged(QStandardItem *i){
//    if (i->checkState()==Qt::Checked){

//        items.append(i->text());
//        cout <<i->text().toStdString()<<endl;
//    }
//    else{
//        items.removeOne(i->text());
//        cout <<i->text().toStdString()<<endl;
//    }
//}
