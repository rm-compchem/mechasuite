from PyQt5.QtWidgets import QDialog, QMainWindow, QListView, QPushButton, QVBoxLayout, QHBoxLayout, QLineEdit, QLabel, QCheckBox, QTableWidget
from PyQt5.QtWidgets import QTableWidgetItem, QWidget, QMessageBox
from PyQt5.QtGui import QStandardItemModel, QStandardItem
from PyQt5.QtCore import Qt
from collections import OrderedDict


class MultipleChoiceDialog(QDialog):
    def __init__(self, itmslist):
        QDialog.__init__(self)
        self.items = []
        self.allitmes = itmslist
        self.ok = False
        self.setWindowTitle("Zeros")
        self.list = QListView(self)
        self.model = QStandardItemModel(self.list)
        self.model.itemChanged.connect(self.set_items)
        for m in itmslist:
            item = QStandardItem(m)
            item.setCheckable(True)
            item.setEditable(False)
            self.model.appendRow(item)
        self.list.setModel(self.model)
        self.list.show()

        self.okbutton = QPushButton("Ok")
        self.okbutton.clicked.connect(self.set_ok)
        self.closebutton = QPushButton("Cancel")
        self.closebutton.clicked.connect(self.unset_ok)
        self.layout = QVBoxLayout()
        self.sublayout = QHBoxLayout()
        self.layout.addWidget(self.list)
        self.layout.addLayout(self.sublayout)
        self.sublayout.addWidget(self.okbutton)
        self.sublayout.addWidget(self.closebutton)
        self.setLayout(self.layout)

    def set_ok(self, e):
        self.ok = True
        self.close()

    def unset_ok(self, e):
        self.ok = False
        self.items = []
        self.close()

    def set_items(self, item):
        if item.checkState() == Qt.Checked:
            self.items.append(item.text())
        if item.checkState() == Qt.Unchecked:
            self.items.remove(item.text())


class SingleChoiceDialog(MultipleChoiceDialog):
    def __init__(self, itmslist):
        MultipleChoiceDialog.__init__(self, itmslist)
        del self.items
        self.item = ""

    def set_items(self, item):
        if item.checkState() == Qt.Checked:
            self.item = item.text()
            for i, _ in enumerate(self.allitmes):
                itm = self.model.item(i, 0)
                if itm.text() != self.item:
                    itm.setCheckState(Qt.Unchecked)

    def unset_ok(self, e):
        self.ok = False
        self.item = ""
        self.close()


class MergeItemsDialog(MultipleChoiceDialog):
    def __init__(self, itmslist):
        MultipleChoiceDialog.__init__(self, itmslist)
        self.lineE = QLineEdit()
        self.lineE.textChanged.connect(self.on_text_changed)
        self.label = QLabel("")
        self.fullmerge = False
        self.fullmerge_checkbox = QCheckBox("Full merge")
        self.fullmerge_checkbox.stateChanged.connect(self.on_fullmerge_checkbox_stateChanged)

        self.layout.addWidget(self.fullmerge_checkbox)
        self.layout.addWidget(self.lineE)
        self.layout.addWidget(self.label)

    def on_fullmerge_checkbox_stateChanged(self, state):
        if state == 2:
            self.fullmerge = True
        else:
            self.fullmerge = False

    def on_text_changed(self):
        self.label.setText("")
        text = self.lineE.text()
        text = text.split()

        # check that names in qlineedit are correct
        for t in text:
            if t not in self.allitmes:
                self.label.setText("Please check the list of names!")
                # break
                return

        self.items = text

    def set_items(self, item):
        self.label.setText("")
        self.items.append(item)
        t = self.lineE.text()
        t += item.text() + "  "
        self.lineE.setText(t)


class OrderWindow(QMainWindow):
    def __init__(self, parent, itm_names, mec):
        QMainWindow.__init__(self, parent)

        self.itm_names = itm_names
        self.mec = mec
        self.table = QTableWidget(self)

        self.table.setColumnCount(2)
        self.table.setRowCount(len(itm_names))

        for row, name in enumerate(itm_names):
            self.table.setItem(row, 1, QTableWidgetItem(name))

        self.bok = QPushButton("Ok")
        self.bca = QPushButton("Close")

        cw = QWidget()

        self.vlo = QVBoxLayout()
        self.hlo = QHBoxLayout()

        self.vlo.addWidget(self.table)
        self.vlo.addLayout(self.hlo)
        self.hlo.addWidget(self.bok)
        self.hlo.addWidget(self.bca)

        cw.setLayout(self.vlo)
        self.setCentralWidget(cw)

        # bindings
        self.bok.clicked.connect(self.on_ok)
        self.bca.clicked.connect(self.on_cancel)

        self.show()

    def on_cancel(self):
        self.close()

    def on_ok(self, e):
        dic = OrderedDict()
        itmos = self.parent().data.get_mech(self.mec).itms
        # check for repetead values
        parsed = []
        for row, name in enumerate(self.itm_names):
            tableitem = self.table.item(row, 0)
            if tableitem is None:
                continue
            else:
                n = int(tableitem.text())
                if n in parsed:
                    msg = QMessageBox()
                    msg.setText("There is repeated values")
                    msg.exec()
                    return
                parsed.append(n)

        parsed_itms = []
        counter = 1
        while counter <= len(self.itm_names): # this while has caused infinit loop, must be checked
            print(counter)
            found = False
            for row, name in enumerate(self.itm_names):
                # if name in parsed_itms:
                #     continue
                tableitem = self.table.item(row, 0)
                if tableitem is None:
                    continue
                else:
                    n = int(tableitem.text())
                if n == counter:
                    dic[name] = itmos[name]
                    counter += 1
                    found = True
                    parsed_itms.append(name)
                    break
            if not found:
                for row, name in enumerate(self.itm_names):
                    tableitem = self.table.item(row, 0)
                    if tableitem is None:
                        dic[name] = itmos[name]
                        self.table.setItem(row, 0, QTableWidgetItem(str(counter)))
                        counter += 1
                        parsed_itms.append(name)
                        break

        self.parent().data.get_mech(self.mec).itms = dic
        self.parent().update_data()



