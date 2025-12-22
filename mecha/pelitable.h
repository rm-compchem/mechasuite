#ifndef PELITABLE_H
#define PELITABLE_H
#include <QtWidgets>
#include "globals.h"


class PeliTable :  public QListWidget
{
      Q_OBJECT

public:
    PeliTable(QWidget *parent);

    void update_data(int maxindex);

   void keyPressEvent(QKeyEvent *event);

signals:
    void step_deleted(vector<int> indices);
    void item_selected(int index);
};

#endif // PELITABLE_H
