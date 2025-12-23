#!/usr/bin/env python

from PyQt5.QtWidgets import QApplication
from mechasuite.Sheet import MainWindow
import sys

if __name__ == "__main__":
    app = QApplication(sys.argv)
    try:
        infile = sys.argv[1]
    except IndexError:
        # infile = "prueba"
        infile = ""
    main = MainWindow(infile)
    main.showMaximized()
    app.exec_()
