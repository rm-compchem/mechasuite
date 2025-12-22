#ifndef SINGLECHOICEDIALOG_H
#define SINGLECHOICEDIALOG_H

#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include <vector>
#include <QtWidgets>




class SingleChoiceDialog: public QDialog
{
    Q_OBJECT

public:
    SingleChoiceDialog(QWidget *parent, QStringList itms);
    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *sublayout = new QHBoxLayout();
    QPushButton *okbutton = new QPushButton("OK");
    QPushButton *cancelbutton = new QPushButton("Cancel");

    bool ok = false;
    QString selected;
    QStringList items;
    std::vector<QCheckBox*> checkboxes;
    int checked = -1;

private slots:
    void on_okbutton_clicked();
    void on_cancelbutton_clicked();
//    void on_checkbox_stateChanged(QStringList state);
    void on_chb_stateChanged(int state);
};


#endif // SINGLECHOICEDIALOG_H
