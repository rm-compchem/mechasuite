#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.QtWidgets import (
    QMainWindow, QApplication, QGraphicsScene, QGraphicsView,
    QGraphicsRectItem, QGraphicsTextItem, QGraphicsItem, QLabel, QComboBox, QGraphicsLineItem, QCheckBox
)
from PyQt5.QtGui import QPainter, QPen, QBrush, QColor
from PyQt5.QtCore import Qt, QRectF, QPointF
from mechasuite.Widgets import MultipleChoiceDialog
from mechasuite.Data import Data
import sys
import numpy as np
import matplotlib
import os

Colors = {'min': QColor(200, 200, 255), 
            'ts': QColor(255, 200, 200),
            'mecp': QColor(200, 255, 200)
            }

class ItmNode(QGraphicsRectItem):
    def __init__(self, itm, x, y, board_window, width=140, height=60):
        super().__init__(0, 0, width, height)
        self.itm = itm
        self.board_window = board_window
        self.lines = []
        self.setPos(x, y)
        self.color = Colors.get(itm.tp, QColor(200, 200, 255))

        self.setBrush(QBrush(self.color))
        self.setPen(QPen(Qt.black))
        self.setFlags(
            QGraphicsItem.ItemIsMovable |
            QGraphicsItem.ItemIsSelectable |
            QGraphicsItem.ItemSendsGeometryChanges
        )
        self.setToolTip(f"{itm.name}\nEnergy: {getattr(itm, 'energy', 0):.4f}")

        label = QGraphicsTextItem(itm.name, self)
        label.setDefaultTextColor(Qt.black)
        label.setPos(8, 8)

    def itemChange(self, change, value):
        if change == QGraphicsItem.ItemPositionHasChanged:
            for line in self.lines:
                line.update_position()

            if hasattr(self, 'board_window'):
                self.board_window.update_layout(self.itm, value)

        return super().itemChange(change, value)

class LayoutNode:
    def __init__(self, mech_name):
        self.mech_name = mech_name
        self.positions = {}  # dict: itm.name -> (x, y)
        self.colors = {}    # dict: itm.name -> QColor
        self.connections = []  # list of (itm1.name, itm2.name) for connections

    def save_position(self, itm_name, pos):
        self.positions[itm_name] = (pos.x(), pos.y())

    def save_color(self, itm_name, color):
        self.colors[itm_name] = color

    def add_connection(self, itm1_name, itm2_name, k=None):
        if (itm1_name, itm2_name) not in self.connections:
            self.connections.append((itm1_name, itm2_name, k if k else None))

class ConnectionLine(QGraphicsLineItem):
    def __init__(self, node1, node2, k=None, parent=None):
        super().__init__(parent)
        self.source = node1
        self.target = node2
        if k is not None:
            self.k = k # k should be a dict with all Temps and ks at all temperatures
            # rate constant, associate it to line thickness. Temp : rate constant
        self.k_label = None
        self.setPen(QPen(Qt.blue, 2))
        self.setZValue(-1)  # Ensure lines are behind nodes
        self.update_position()

    # Add the KLabel in the center of the line, with the value of k at the current temperature
    def set_label(self, k_value):
        self.k_label = k_value
        self.update_position()

    def update_position(self):
        if self.source and self.target:
            p1 = self.source.sceneBoundingRect().center()
            p2 = self.target.sceneBoundingRect().center()
            self.setLine(p1.x(), p1.y(), p2.x(), p2.y())
            if hasattr(self, 'k_label') and self.k_label:
                self.k_label.update_position()

    def update_temperature(self, curr_T):
        if self.k and curr_T in self.k:
            k_value = self.k[curr_T]
            if self.k_label:
                self.k_label.setPlainText(f"{float(k_value):.2e}" if k_value is not None else "N/A")
            else:
                self.k_label.setPlainText("N/A")
            self.k_label.update_position()



            

class KLabel(QGraphicsTextItem):
    def __init__(self, text, line, parent=None):
        try:
            sci_text = f"{float(text):.2e}"
        except (ValueError, TypeError):
            sci_text = "N/A"
        super().__init__(sci_text, parent)
        self.setDefaultTextColor(Qt.red)
        self.setZValue(1)  # Ensure labels are above lines
        self.line = line
        self.update_position()

    def update_position(self):
        if self.line and self.line.source and self.line.target:
            center_point = self.line.line().center()
            rect = self.boundingRect()
            self.setPos(center_point.x() - (rect.width() / 2), center_point.y() - (rect.height() / 2))

class BoardWindow(QMainWindow):
    def __init__(self, data, comboT, parent=None):
        super().__init__(parent)
        self.data = data

        # check if there is a saved layout for this mech, if not create an empty one
        if not hasattr(self.data, 'board_layouts'):
            self.data.board_layouts = {}
        self.layouts = self.data.board_layouts

        self.toolbar = self.addToolBar("Mechanism")
        self.toolbar.addWidget(QLabel("Mechanism: "))
        self.tempbar = self.addToolBar("Temp")
        self.tempbar.addWidget(QLabel("Temperature: "))
        self.hide_k = self.addToolBar("Hide k values")
        self.hide_k.addWidget(QLabel("Hide k values: "))
        
        self.mech_combo = QComboBox()
        self.mech_combo.addItems(self.data.get_mechs_names())
        self.mech_combo.currentTextChanged.connect(self.on_mech_changed) 
        self.toolbar.addWidget(self.mech_combo)

        self.temp_combo = QComboBox()  
        self.temp_combo.addItems(comboT)
        self.temp_combo.currentTextChanged.connect(self.on_temp_changed)
        self.tempbar.addWidget(self.temp_combo)

        self.hide_k_checkbox = QCheckBox()
        self.hide_k.addWidget(self.hide_k_checkbox)
        
        self.hide_k_checkbox.stateChanged.connect(self.on_hide_k_changed)
        

        self.scene = QGraphicsScene(self)
        self.view = QGraphicsView(self.scene, self)
        self.view.setRenderHints(QPainter.Antialiasing | QPainter.TextAntialiasing)
        self.setCentralWidget(self.view)

        self.setWindowTitle("MechaData Board")
        self.resize(1000, 700)
        
        if self.data.get_mechs():
            self.on_mech_changed(self.data.get_mechs_names()[0])


    def on_mech_changed(self, mech_name):
        self.scene.clear()
        mech = self.data.get_mech(mech_name)
        self.build_board(mech)

    def on_temp_changed(self, temp):
        for item in self.scene.items():
            if isinstance(item, ConnectionLine):
                item.update_temperature(temp)

    def on_hide_k_changed(self, state):
        for item in self.scene.items():
            if isinstance(item, KLabel):
                item.setVisible(state == Qt.Unchecked)

    def build_board(self, mech):
        if mech is None:
            return
        
        if mech.name not in self.layouts:
            self.layouts[mech.name] = LayoutNode(mech.name)

        layout = self.layouts[mech.name]

        margin_x = 40
        margin_y = 40
        dx = 180
        dy = 120

        #Ordenar primero
        itms = mech.get_itms()
        #exclude refs
        itms = [itm for itm in itms if itm.tp != 'ref']

        nodes = {}

        for index, itm in enumerate(itms):
            if itm.name in layout.positions:
                x, y = layout.positions[itm.name]
            else:
                x = margin_x + (index % 5) * dx
                y = margin_y + (index // 5) * dy
                layout.save_position(itm.name, QPointF(x, y))

            if itm.name in layout.colors:
                color = layout.colors[itm.name]
            else:
                color = Colors.get(itm.tp, QColor(200, 200, 255))
                layout.save_color(itm.name, color)

            node = ItmNode(itm, x, y, self)
            node.setBrush(QBrush(color))
            self.scene.addItem(node)
            nodes[itm.name] = node

        for itm in itms:
            for reac in itm.get_reacs():
                ref_name = reac.ref.name
                if ref_name in nodes:
                    source = nodes[itm.name]
                    target = nodes[ref_name]
                    k = {}
                    Temps = [self.temp_combo.itemText(i) for i in range(self.temp_combo.count())]
                    for T in Temps:
                        try:
                            k[T] = float(reac.thermo["k"][np.float64(T)])
                        except Exception as e:
                            print(f"Error getting k for {itm.name} -> {ref_name} at T={T}: {e}")
                            k[T] = None


                    line = ConnectionLine(source, target, k)
                    self.scene.addItem(line)
                    source.lines.append(line)
                    target.lines.append(line)

                    current_temp = self.temp_combo.currentText()
                    initial_k_value = k.get(current_temp, None)

                    k_label = KLabel(str(initial_k_value) if initial_k_value is not None else "N/A", line)
                    self.scene.addItem(k_label)

                    line.set_label(k_label)

                    #try:
                    #    T = float(self.temp_combo.currentText())                        
                    #    k = KLabel(str(reac.thermo["k"][np.float64(T)]), line)
                    #    self.scene.addItem(k)
                    #except Exception as e:
                    #    print(f"Error creating KLabel for {itm.name}: {e}")
                    #    k = None
                    layout.add_connection(itm.name, ref_name, k)


    def update_layout(self, itm, new_pos):
        mech_name = self.mech_combo.currentText()
        if mech_name in self.layouts:
            self.layouts[mech_name].save_position(itm.name, new_pos)


    

