#include "table.h"
#include <iostream>


Table::Table(QWidget *parent) : QTableWidget(parent)
{

    setColumnCount(4);
    QTableWidgetItem *table_item;
    table_item = new QTableWidgetItem("Label");
    setHorizontalHeaderItem(0, table_item);
    table_item = new QTableWidgetItem("X");
    setHorizontalHeaderItem(1, table_item);
    table_item = new QTableWidgetItem("Y");
    setHorizontalHeaderItem(2, table_item);
    table_item = new QTableWidgetItem("Z");
    setHorizontalHeaderItem(3, table_item);
    QHeaderView *header = horizontalHeader();

    for(int i =0;i<4;i++){
        header->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }


    editing = false;

    setMouseTracking(true);

    connect(this, &QTableWidget::cellDoubleClicked, this, &Table::on_cellDoubleClicked);
    connect(this, &QTableWidget::cellClicked, this, &Table::on_cellClicked);
    connect(this->verticalHeader(),SIGNAL(sectionClicked(int)), this, SLOT(on_sectionClicked(int)));
}


void Table::update_data(vector<Atom> &fatoms){
//    #include <chrono>
//    auto start = std::chrono::system_clock::now();
    this->hide();
    clearContents();
    int cont = 0;
//    setColumnCount(4);
    setRowCount(fatoms.size());
    QTableWidgetItem *table_item;
    for(auto &at:fatoms){
        table_item = item(cont, 0);
        if(!table_item){
            table_item = new QTableWidgetItem(at.label);
            setItem(cont, 0, table_item);
        }else{
//            cout<<"just setting\n";
             table_item->setText(at.label);
        }

        for(unsigned int k=0;k<3;k++){
            table_item = item(cont, k+1);
            if(!table_item){
                table_item = new QTableWidgetItem(QString::number(at.coor[k]));
                setItem(cont, k+1, table_item);
            }else{
                table_item->setText(QString::number(at.coor[k]));
            }
        }
        cont ++;
    }
    this->show();
//    cout<<"done\n";
//    auto end = std::chrono::system_clock::now();
//    std::chrono::duration<double> elapsed_seconds = end-start;
//    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}

void Table::on_cellDoubleClicked(int row, int col){
    editing = true;
}

void Table::on_cellClicked(int row, int col){
    editing = false;
}

void Table::keyPressEvent(QKeyEvent *event){
    if(!non_editing_keys.contains(event->key())){
        editing = true;
        QTableWidget::keyPressEvent(event);
        return;
    }
    editing = false;
     QTableWidget::keyPressEvent(event);
}

void Table::on_sectionClicked(int logicalindex){
    emit rowSelected(logicalindex);
}

vector<int> Table::selectedRows(){
    vector<int> selrows;
    int index;
    for(auto itm:selectedItems()){
        index = itm->row();
        auto it = std::find(selrows.begin(), selrows.end(), index);
        if(it == selrows.end()){
            selrows.push_back(index);
        }
    }
    return selrows;
}
