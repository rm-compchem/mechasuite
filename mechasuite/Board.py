#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.QtWidgets import (
    QDialog, QFileDialog, QMainWindow, QGridLayout, QApplication, QGraphicsScene, QGraphicsView, QLineEdit, QPushButton, 
    QGraphicsRectItem, QGraphicsTextItem, QGraphicsItem, QLabel, QComboBox, QGraphicsLineItem, QCheckBox, QMenu, QMessageBox, QGraphicsPixmapItem
)
from PyQt5.QtGui import QPainter, QPen, QBrush, QColor, QPixmap, QImage, QPainterPath
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

def qcolor_to_dict(qcolor):
    """Convert QColor to JSON-serializable dict"""
    return {
        'r': qcolor.red(),
        'g': qcolor.green(),
        'b': qcolor.blue(),
        'a': qcolor.alpha()
    }

def dict_to_qcolor(color_dict):
    """Convert dict back to QColor"""
    return QColor(
        color_dict['r'],
        color_dict['g'],
        color_dict['b'],
        color_dict.get('a', 255)
    )

# import a SMILES image from code
def create_smiles_image(smiles):
    from rdkit import Chem
    from rdkit.Chem import Draw
    from rdkit.Chem.Draw import rdMolDraw2D
    mol = Chem.MolFromSmiles(smiles)
    if mol is None:
        raise ValueError(f"Invalid SMILES: {smiles}")
    opts = rdMolDraw2D.MolDrawOptions()
    opts.clearBackground = True  # Transparent background
    img = Draw.MolToImage(mol, size=(200, 200), options=opts)
    return img

def pil_to_qpixmap(pil_image):
    """Convert PIL Image to QPixmap"""
    # Ensure RGB or RGBA mode
    if pil_image.mode not in ('RGB', 'RGBA'):
        pil_image = pil_image.convert('RGBA' if pil_image.mode == 'P' or 'LA' else 'RGB')
    
    # Get raw image data as bytes
    data = pil_image.tobytes()
    w, h = pil_image.size
    
    if pil_image.mode == 'RGB':
        qimage = QImage(data, w, h, w * 3, QImage.Format_RGB888)
    elif pil_image.mode == 'RGBA':
        qimage = QImage(data, w, h, w * 4, QImage.Format_RGBA8888)
    else:
        raise ValueError(f"Unsupported PIL image mode: {pil_image.mode}")
    
    # Convert to QPixmap
    return QPixmap.fromImage(qimage)


class ZoomableGraphicsView(QGraphicsView):
    """QGraphicsView with zoom"""
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.pan_active = False
        self.pan_start_pos = None
    def wheelEvent(self, event):
        if event.modifiers() == Qt.ControlModifier:
            # Zoom factor: positive delta = zoom in, negative = zoom out
            zoom_factor = 1.1 if event.angleDelta().y() > 0 else 0.9
            self.scale(zoom_factor, zoom_factor)
            event.accept()
        else:
            super().wheelEvent(event)
    def mousePressEvent(self, event):
        if event.button() == Qt.MiddleButton:
            self.pan_active = True
            self.pan_start_pos = event.pos()
            event.accept()
        else:
            super().mousePressEvent(event)
    
    def mouseMoveEvent(self, event):
        if self.pan_active and self.pan_start_pos:
            delta = event.pos() - self.pan_start_pos
            self.horizontalScrollBar().setValue(
                self.horizontalScrollBar().value() - delta.x()
            )
            self.verticalScrollBar().setValue(
                self.verticalScrollBar().value() - delta.y()
            )
            self.pan_start_pos = event.pos()
            event.accept()
        else:
            super().mouseMoveEvent(event)
    
    def mouseReleaseEvent(self, event):
        if event.button() == Qt.MiddleButton:
            self.pan_active = False
            self.pan_start_pos = None
            event.accept()
        else:
            super().mouseReleaseEvent(event)

            
class ItmNode(QGraphicsRectItem):
    def __init__(self, itm, x, y, board_window, width=140, height=160, smiles=None):
        super().__init__(0, 0, width, height)
        self.itm = itm
        self.board_window = board_window
        self.lines = []
        self.setPos(x, y)
        self.color = Colors.get(itm.tp, QColor(200, 200, 255))
        self.header_color = Colors.get(itm.tp, QColor(200, 200, 255))
        self.smiles = smiles
        self.img = None
        self.width = width
        self.height = height
        self.header_height = 30

        self.setBrush(QBrush(self.color))
        self.setPen(QPen(Qt.black))
        self.setFlags(
            QGraphicsItem.ItemIsMovable |
            QGraphicsItem.ItemIsSelectable |
            QGraphicsItem.ItemSendsGeometryChanges
        )
        self.setToolTip(f"{itm.name}\nEnergy: {getattr(itm, 'energy', 0):.4f}")

        self.label = QGraphicsTextItem(itm.name, self)
        font = self.label.font()
        font.setBold(True)
        self.label.setFont(font)
        self.label.setDefaultTextColor(Qt.black)

        text_rect = self.label.boundingRect() # Centering text in header
        text_x = (self.width - text_rect.width()) / 2
        text_y = (self.header_height - text_rect.height()) / 2
        self.label.setPos(text_x, text_y)




        #label = QGraphicsTextItem(itm.name, self)
        #label.setZValue(1)  # Ensure label is above the rectangle
        #label.setDefaultTextColor(Qt.black)
        #label.setPos(8, 8)

        if self.smiles:
            try:
                self.img = create_smiles_image(self.smiles)
            except Exception as e:
                print(f"Error creating image for {itm.name} with SMILES {self.smiles}: {e}")

        if self.img:
            # If you have an image, you can add it as background of the node:
            #pixmap = QPixmap.fromImage(self.img)
            pixmap = pil_to_qpixmap(self.img)
            self.pixmap_item = QGraphicsPixmapItem(pixmap, self)
            #pixmap_item = QGraphicsPixmapItem(pixmap, self)
            #pixmap_item.setPos(0, 0)
            #pixmap_item.setZValue(-1)  # Ensure image is behind text
            # Calculate the available space for the image (with 10px padding)
            padding = 10
            avail_w = self.width - (padding * 2)
            avail_h = self.height - self.header_height - (padding * 2)
            
            # Scale image smoothly while keeping aspect ratio
            scaled_pixmap = pixmap.scaled(
                avail_w, avail_h, 
                Qt.KeepAspectRatio, 
                Qt.SmoothTransformation
            )
            self.pixmap_item.setPixmap(scaled_pixmap)
            
            # Center the image within the body bounds
            img_x = (self.width - scaled_pixmap.width()) / 2
            img_y = self.header_height + (avail_h - scaled_pixmap.height()) / 2 + padding
            self.pixmap_item.setPos(img_x, img_y)  

    def paint(self, painter, option, widget=None):
        """Override paint to draw a custom, polished node background."""
        painter.setRenderHint(painter.Antialiasing) # Ensure smooth curves
        
        # Create a rounded path for the main box
        path = QPainterPath()
        path.addRoundedRect(0, 0, self.width, self.height, 8, 8)
        
        # Draw the main body background (White/Light Gray)
        painter.setBrush(QBrush(self.color))
        painter.setPen(Qt.NoPen)
        painter.drawPath(path)
        
        # Draw the Header Background
        # We use setClipPath so the header colors don't bleed outside the rounded corners
        painter.setClipPath(path)
        painter.setBrush(QBrush(self.header_color))
        painter.drawRect(0, 0, self.width, self.header_height)
        
        # Draw the separator line between header and body
        painter.setClipping(False) # Turn clipping off to draw the outer border
        painter.setPen(QPen(QColor(150, 150, 150), 1))
        painter.drawLine(0, self.header_height, self.width, self.header_height)
        
        # Draw the outer border of the node
        # Change border color if the item is selected by the user
        if self.isSelected():
            painter.setPen(QPen(QColor(100, 150, 255), 2.5)) # Highlighted border
        else:
            painter.setPen(QPen(QColor(120, 120, 120), 1.5)) # Standard border
            
        painter.drawPath(path)

    def contextMenuEvent(self, event):
        menu = QMenu()
        add_reac_action = menu.addAction("Add Reactant(s)")
        add_smiles_sketch_action = menu.addAction("Add SMILES Sketch")

        action = menu.exec_(event.screenPos())

        if action == add_reac_action:
            self._handle_add_reac()
        elif action == add_smiles_sketch_action:
            self._handle_add_smiles_sketch()

     #   super().contextMenuEvent(event)

    def _handle_add_reac(self):
        scene = self.scene()
        if not scene:
            return
        sel_items = scene.selectedItems()
        reactants = [item.itm for item in sel_items if isinstance(item, ItmNode) and item.itm != self.itm]
        if reactants:
            self.on_add_reac(reactants)

            for item in sel_items:
                item.setSelected(False)

        self.board_window.scene.clear()
        self.board_window.build_board(self.itm.mech) # Update layout after adding reactants

    def _handle_add_smiles_sketch(self):
        dialog = QDialog()
        dialog.setWindowTitle("Add SMILES Sketch")
        self.smiles_input = QLineEdit(dialog)
        self.smiles_input.setPlaceholderText("Enter SMILES string")
        add_button = QPushButton("Add", dialog)
        add_button.clicked.connect(lambda: self._add_smiles_reac(dialog))
        layout = QGridLayout(dialog)
        layout.addWidget(QLabel("SMILES:"), 0, 0)
        layout.addWidget(self.smiles_input, 0, 1)
        layout.addWidget(add_button, 1, 0, 1, 2)
        dialog.exec_()

    def _add_smiles_reac(self, dialog):
        smiles = self.smiles_input.text().strip()
        if not smiles:
            QMessageBox.warning(dialog, "Input Error", "Please enter a valid SMILES string.")
            return    
        self.itm.smiles = smiles
        try:
            img = create_smiles_image(smiles)
            # Here you would convert the image to a format suitable for display in the node
            # and create a new Itm for the reactant with the SMILES info. This is a placeholder.
            self.set_image(img)
        except Exception as e:
            QMessageBox.critical(dialog, "Error", f"Failed to create image from SMILES: {e}")
            return
        dialog.accept()

    def set_image(self, img):
        self.img = img
        pixmap = pil_to_qpixmap(img)

        # Scale the pixmap to fit within the node with a small margin
        # Node size is 140x60, so scale to 130x50 to show background color
        scaled_pixmap = pixmap.scaledToWidth(130, Qt.SmoothTransformation)
        if scaled_pixmap.height() > 50:
            scaled_pixmap = scaled_pixmap.scaledToHeight(50, Qt.SmoothTransformation)

        pixmap_item = QGraphicsPixmapItem(scaled_pixmap, self)
        # Center the image within the node
        pixmap_item.setPos((140 - scaled_pixmap.width()) / 2, (60 - scaled_pixmap.height()) / 2)
        pixmap_item.setZValue(0.5)  # Keep behind text label

    def itemChange(self, change, value):
        if change == QGraphicsItem.ItemPositionHasChanged:
            for line in self.lines:
                line.update_position()

            if hasattr(self, 'board_window'):
                self.board_window.update_layout(self.itm, value)

        return super().itemChange(change, value)

    def on_add_reac(self, reactants): # reactant should be a list with the selected reactants (of class Itm)
        # El itm contiene su mech en self.mech
        if self.itm.tp == 'ref':
            return  # No se pueden agregar reacciones a un ref
        for reac in reactants:
            if reac.struct.nat != self.itm.struct.nat:
                print(reac.name, reac.struct, self.itm.name, self.itm.struct)
                QMessageBox.critical(
                    None, 
                    "Error", 
                    f"Cannot add reaction: {reac.name} has different number of atoms than {self.itm.name}"
                )
                continue
            self.itm.add_reac(reac)
        
        



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

    def to_dict(self):
        """Convert layout to JSON-serializable dict"""
        return {
            'mech_name': self.mech_name,
            'positions': {name: list(pos) if isinstance(pos, tuple) else pos for name, pos in self.positions.items()},
            'colors': {name: qcolor_to_dict(color) for name, color in self.colors.items()},
            'connections': [(c[0], c[1], c[2]) for c in self.connections]
        }

    @staticmethod
    def from_dict(data):
        """Create LayoutNode from dict"""
        layout = LayoutNode(data['mech_name'])
        layout.positions = {name: tuple(pos) for name, pos in data['positions'].items()}
        layout.colors = {name: dict_to_qcolor(color) for name, color in data['colors'].items()}
        layout.connections = data['connections']
        return layout

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
        self.view = ZoomableGraphicsView(self.scene, self)
        # self.view = QGraphicsView(self.scene, self)
        self.view.setRenderHints(QPainter.Antialiasing | QPainter.TextAntialiasing)
        self.setCentralWidget(self.view)

        self.setWindowTitle("MechaData Board")
        self.resize(1000, 700)
        
        if self.data.get_mechs():
            self.on_mech_changed(self.data.get_mechs_names()[0])

    def contextMenuEvent(self, event):
        menu = QMenu()
        import_interm = menu.addAction("Import Intermediate")

        action = menu.exec_(event.globalPos())

        if action == import_interm:
            self._handle_import_interm()


    def _handle_import_interm(self):
        mech_name = self.mech_combo.currentText()
        mech = self.data.get_mech(mech_name)
        if not mech:
            QMessageBox.critical(self, "Error", "No mechanism selected")
            return
        
        dialog = QFileDialog()
        dialog.setFileMode(QFileDialog.Directory)
        dialog.setOption(QFileDialog.DontUseNativeDialog)
        folder = dialog.getExistingDirectory(self, "Choose folder", "")
        if not folder:
            return
        
        ignored_itms = []
        itmobj = mech.itm_from_folder(folder)
        if itmobj is None:
            ignored_itms.append(folder)

        if ignored_itms:
            msg = "The following folders were not added because there are items with the same name or folder is not set properly:\n\n"
            msg += "\n".join([os.path.basename(i) for i in ignored_itms])
            QMessageBox.information(self, "Import Info", msg)

        self.scene.clear()
        self.build_board(mech)       


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
            node = ItmNode(itm, x, y, self, smiles=getattr(itm, 'smiles', None))
            node.setBrush(QBrush(color))
            self.scene.addItem(node)
            nodes[itm.name] = node

 #           if hasattr(itm, 'smiles') and itm.smiles:
 #               try:
 #                   img = create_smiles_image(itm.smiles)
 #                   node.set_image(img)
 #               except Exception as e:
 #                   print(f"Error creating image for {itm.name} with SMILES {itm.smiles}: {e}")

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
            # Update connections in layout
            for reac in itm.get_reacs():
                ref_name = reac.ref.name
                self.layouts[mech_name].add_connection(itm.name, ref_name)
            


    

