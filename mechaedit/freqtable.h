#ifndef FREQTABLE_H
#define FREQTABLE_H

//#include <QtWidgets>
#include <QtWidgets/QTableWidget>
#include "globals.h"



class FreqTable : public QTableWidget
{
    Q_OBJECT

public:
    FreqTable(QWidget *parent);

    void update_data(vector<Freq> &frequencies);
private slots:

};

#endif // FREQTABLE_H
