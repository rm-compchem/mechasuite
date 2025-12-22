#ifndef MULTICHOICEDIALOG_H
#define MULTICHOICEDIALOG_H

#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include <vector>
#include <QtWidgets>

using namespace std;


class MultiChoiceDialog : public QDialog
{
    Q_OBJECT

public:
    MultiChoiceDialog(QWidget *parent, QStringList itms);
    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *sublayout = new QHBoxLayout();
    QPushButton *okbutton = new QPushButton("OK");
    QPushButton *cancelbutton = new QPushButton("Cancel");

    bool ok = false;
    QStringList items;

    QCheckBox *chb1 = new QCheckBox(this);
    QCheckBox *chb2 = new QCheckBox(this);
    QCheckBox *chb3 = new QCheckBox(this);

//    vector<QCheckBox *> checkboxes;

private slots:
    void on_okbutton_clicked();
    void on_cancelbutton_clicked();
//    void on_checkbox_stateChanged(QStringList state);
    void on_chb1_stateChanged(int state);
    void on_chb2_stateChanged(int state);
    void on_chb3_stateChanged(int state);
};



//class MultiChoiceDialog : public QDialog
//{
//    Q_OBJECT

//public:
//    MultiChoiceDialog(QWidget *parent, QStringList itms);
//    QListView *list = new QListView(this);
//    QStandardItemModel *model = new QStandardItemModel(list);
//    QStandardItem item;
//    QVBoxLayout *layout = new QVBoxLayout();
//    QHBoxLayout *sublayout = new QHBoxLayout();
//    QPushButton *okbutton = new QPushButton("OK");
//    QPushButton *cancelbutton = new QPushButton("Cancel");
//    bool ok = false;
//    QStringList items;

//private slots:
//    void on_okbutton_released();
//    void on_cancelbutton_released();
//    void on_model_itemChanged(QStandardItem *i);
//};

#endif // MULTICHOICEDIALOG_H
