#ifndef MULTICHOICEDIALOG2_H
#define MULTICHOICEDIALOG2_H



#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include <vector>
#include <QtWidgets>




class MultiChoiceDialog2: public QDialog
{
    Q_OBJECT

public:
    MultiChoiceDialog2(QWidget *parent, QStringList itms);
    QGridLayout *layout = new QGridLayout();
    QHBoxLayout *sublayout = new QHBoxLayout();
    QPushButton *okbutton = new QPushButton("OK");
    QPushButton *cancelbutton = new QPushButton("Cancel");

    bool ok = false;
    QStringList selectedItems;
    QStringList items;
    std::vector<QCheckBox*> checkboxes;

private slots:
    void on_okbutton_clicked();
    void on_cancelbutton_clicked();
//    void on_checkbox_stateChanged(QStringList state);
    void on_chb_stateChanged(int state);
};


#endif // MULTICHOICEDIALOG2_H
