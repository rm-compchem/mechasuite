#include "pelitable.h"

PeliTable::PeliTable(QWidget *parent) : QListWidget(parent)
{
       addItem("Steps");
       hide();

}


void PeliTable::update_data(int maxindex){
    hide();
    clear();
    addItem("Steps");
    for(int i=0;i<maxindex;i++){
        addItem(QString::number(i+1));
    }
    if(maxindex<=1) return;
    show();
}

void PeliTable::keyPressEvent(QKeyEvent *event){
    setCursor(Qt::WaitCursor);
    vector<int> indices;
    if(event->key()==Qt::Key_Delete){
        for(auto i:selectedIndexes()){
            if(i.row()==0) continue;

            indices.push_back(i.row()-1);
        }
        update_data(count()-selectedItems().count()-1); //the extra -1 is to take into account the first index "Steps"
        emit step_deleted(indices);
    }
    setCursor(Qt::ArrowCursor);
    QListWidget::keyPressEvent(event);
}
