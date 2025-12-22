#include "freqtable.h"

FreqTable::FreqTable(QWidget *parent) : QTableWidget(parent)
{
    setColumnCount(1);
    hide();
    setMouseTracking(true);

}

void FreqTable::update_data(vector<Freq> &freqs){
    unsigned int nfreqs = freqs.size();
    setRowCount(nfreqs);
    clearContents();
    for(unsigned int i=0.;i<nfreqs;i++){
        QTableWidgetItem *table_item = new QTableWidgetItem(QString::number(freqs[i].vib));
        setItem(i, 0, table_item);
    }
}


