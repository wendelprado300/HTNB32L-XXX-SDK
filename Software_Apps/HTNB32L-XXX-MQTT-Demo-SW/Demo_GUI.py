#   _    _ _______   __  __ _____ _____ _____   ____  _   _
#  | |  | |__   __| |  \/  |_   _/ ____|  __ \ / __ \| \ | |
#  | |__| |  | |    | \  / | | || |    | |__) | |  | |  \| |
#  |  __  |  | |    | |\/| | | || |    |  _  /| |  | | . ` |
#  | |  | |  | |    | |  | |_| || |____| | \ \| |__| | |\  |
#  |_|  |_|  |_|    |_|  |_|_____\_____|_|  \_\\____/|_| \_|
#  =================== Advanced R&D ========================

#  Copyright (c) 2023 HT Micron Semicondutors S.A.
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

# file: Demo_GUI.py
# brief: MQTT example GUI.
# author: HT Micron Advanced R&D
# link: https://github.com/htmicron
# version: 0.1
# date: April 11, 2023

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1702, 709)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.demo_board_label = QtWidgets.QLabel(self.centralwidget)
        self.demo_board_label.setGeometry(QtCore.QRect(0, 0, 1711, 680))
        self.demo_board_label.setStyleSheet("background-image: url(:/background/DemoBoard.png)")
        self.demo_board_label.setText("")
        self.demo_board_label.setScaledContents(True)
        self.demo_board_label.setObjectName("demo_board_label")
        self.blue_button = QtWidgets.QPushButton(self.centralwidget)
        self.blue_button.setGeometry(QtCore.QRect(366, 550, 61, 74))
        self.blue_button.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.blue_button.setMouseTracking(True)
        self.blue_button.setAcceptDrops(True)
        self.blue_button.setStyleSheet("QPushButton {\n"
"    background-color: rgb(0, 85, 255);\n"
"    border-radius:15px;\n"
"    border-color: black;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(0, 52, 158)\n"
"}\n"
"")
        self.blue_button.setText("")
        self.blue_button.setCheckable(False)
        self.blue_button.setAutoDefault(False)
        self.blue_button.setDefault(False)
        self.blue_button.setObjectName("blue_button")
        self.green_led = QtWidgets.QPushButton(self.centralwidget)
        self.green_led.setEnabled(False)
        self.green_led.setGeometry(QtCore.QRect(210, 610, 16, 34))
        self.green_led.setCursor(QtGui.QCursor(QtCore.Qt.ArrowCursor))
        self.green_led.setStyleSheet("QPushButton {\n"
"    background-color: rgb(0, 86, 0);\n"
"    border-radius:-11px;\n"
"    border-color: white;\n"
"}\n"
"")
        self.green_led.setText("")
        self.green_led.setObjectName("green_led")
        self.white_led = QtWidgets.QPushButton(self.centralwidget)
        self.white_led.setEnabled(False)
        self.white_led.setGeometry(QtCore.QRect(158, 610, 16, 34))
        self.white_led.setStyleSheet("background-color:rgb(141, 141, 141)")
        self.white_led.setText("")
        self.white_led.setObjectName("white_led")
        self.blue_led = QtWidgets.QPushButton(self.centralwidget)
        self.blue_led.setEnabled(False)
        self.blue_led.setGeometry(QtCore.QRect(102, 610, 16, 34))
        self.blue_led.setStyleSheet("background-color: rgb(0, 33, 99)")
        self.blue_led.setText("")
        self.blue_led.setObjectName("blue_led")
        self.white_button = QtWidgets.QPushButton(self.centralwidget)
        self.white_button.setGeometry(QtCore.QRect(475, 542, 61, 72))
        self.white_button.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.white_button.setMouseTracking(True)
        self.white_button.setAcceptDrops(True)
        self.white_button.setStyleSheet("QPushButton {\n"
"    background-color: rgb(255, 255, 255);\n"
"    border-radius:15px;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(108, 108, 108)\n"
"}\n"
"")
        self.white_button.setText("")
        self.white_button.setCheckable(False)
        self.white_button.setAutoDefault(False)
        self.white_button.setDefault(False)
        self.white_button.setObjectName("white_button")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1702, 21))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "HTNB32L-XXX Demo"))
import background_img_rc


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
