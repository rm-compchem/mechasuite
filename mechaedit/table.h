#ifndef TABLE_H
#define TABLE_H

#include <QtWidgets>
#include "atom.h"

using namespace std;


class Table : public QTableWidget
{
     Q_OBJECT

public:
    explicit Table(QWidget *parent = 0);
    bool editing;

    void update_data(vector <Atom> &finalcoors);
    vector<int> selectedRows();

private:
    QVector<int>non_editing_keys = {16777216, 16777264, 16777266, 16777267, 16777268,
                                   16777269,16777270, 16777271, 16777272, 16777273,
                                   16777274,16777275, 16777254, 16777224, 16777222, 16777232,
                                   16777238, 16777223,16777233, 16777239, 16777234, 16777235,
                                   16777236,16777237, 16777217, 16777252, 16777252, 16777253,
                                   16777248, 16777249, 16777249, 16777248, 16777301, 16777250,
                                  16781571, 16777251, 16777250};

private slots:
    void on_cellDoubleClicked(int row, int col);
    void on_cellClicked(int row, int col);
    void on_sectionClicked(int logicalindex);

protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void rowSelected(int index);

};

#endif // TABLE_H
