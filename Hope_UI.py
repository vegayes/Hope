# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'C:\Users\vegay\Desktop\Hope_test\Hope.ui'
#
# Created by: PyQt5 UI code generator 5.9.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QLCDNumber

from Hope2_UI import Ui_HOPE2

# from PyQt5 import QtWebEngineWidgets
# from PyQt5.QtWebEngineWidgets import QWebEngineView

class Ui_HOPE(object):

    def openHope2(self):
        self.window = QtWidgets.QDialog()
        self.ui = Ui_HOPE2()
        self.ui.setupUi(self.window)
        self.window.show()
    def setupUi(self, HOPE):
        HOPE.setObjectName("HOPE")
        HOPE.resize(1477, 844)
        # HOPE.setStyleSheet("background-color: #ebffef;")
        self.serial_port_combo = QtWidgets.QComboBox(HOPE)
        self.serial_port_combo.setGeometry(QtCore.QRect(164, 74, 241, 21))
        self.serial_port_combo.setObjectName("serial_port_combo")
        self.refresh_ports_button = QtWidgets.QPushButton(HOPE)
        self.refresh_ports_button.setGeometry(QtCore.QRect(420, 70, 101, 31))
        self.refresh_ports_button.setObjectName("refresh_ports_button")
        # self.refresh_ports_button.setStyleSheet("background-color : ")
        self.serial_port_label = QtWidgets.QLabel(HOPE)
        self.serial_port_label.setGeometry(QtCore.QRect(40, 70, 121, 31))
        self.serial_port_label.setObjectName("serial_port_label")
        self.Microstepping_label = QtWidgets.QLabel(HOPE)
        self.Microstepping_label.setGeometry(QtCore.QRect(40, 110, 151, 31))
        self.Microstepping_label.setObjectName("Microstepping_label")
        self.Microstepping_combo = QtWidgets.QComboBox(HOPE)
        self.Microstepping_combo.setGeometry(QtCore.QRect(203, 115, 201, 22))
        self.Microstepping_combo.setObjectName("Microstepping_combo")
        self.connect_button = QtWidgets.QPushButton(HOPE)
        self.connect_button.setGeometry(QtCore.QRect(420, 110, 101, 28))
        self.connect_button.setObjectName("connect_button")
        self.Quit_button = QtWidgets.QPushButton(HOPE)
        self.Quit_button.setGeometry(QtCore.QRect(1290, 60, 131, 71))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(20)
        font.setBold(True)
        font.setWeight(75)
        self.Quit_button.setFont(font)
        self.Quit_button.setStyleSheet("color: red;"
                                       "background-color: #fdc4c6;"
                                       "border-style: solid;"
                                       "border-width: 2px;"
                                       "border-radius: 5px")
        self.Quit_button.setObjectName("Quit_button")
        self.JOG_Group = QtWidgets.QGroupBox(HOPE)
        self.JOG_Group.setGeometry(QtCore.QRect(40, 150, 490, 380))
        self.JOG_Group.setObjectName("JOG_Group")
        self.JOG_Group.setEnabled(False)
        self.jog_y_right_button = QtWidgets.QPushButton(self.JOG_Group)
        self.jog_y_right_button.setGeometry(QtCore.QRect(185, 110, 65, 80))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.jog_y_right_button.setFont(font)
        self.jog_y_right_button.setObjectName("jog_y_right_button")
        self.jog_z_label = QtWidgets.QLabel(self.JOG_Group)
        self.jog_z_label.setGeometry(QtCore.QRect(305, 140, 30, 20))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(14)
        font.setBold(True)
        font.setWeight(75)
        self.jog_z_label.setFont(font)
        self.jog_z_label.setObjectName("jog_z_label")
        self.jog_y_left_button = QtWidgets.QPushButton(self.JOG_Group)
        self.jog_y_left_button.setGeometry(QtCore.QRect(10, 110, 65, 80))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.jog_y_left_button.setFont(font)
        self.jog_y_left_button.setObjectName("jog_y_left_button")
        self.jog_xy_label = QtWidgets.QLabel(self.JOG_Group)
        self.jog_xy_label.setGeometry(QtCore.QRect(115, 140, 30, 20))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(14)
        font.setBold(True)
        font.setWeight(75)
        self.jog_xy_label.setFont(font)
        self.jog_xy_label.setObjectName("jog_xy_label")
        self.jog_z_down_button = QtWidgets.QPushButton(self.JOG_Group)
        self.jog_z_down_button.setGeometry(QtCore.QRect(265, 200, 90, 60))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.jog_z_down_button.setFont(font)
        self.jog_z_down_button.setObjectName("jog_z_down_button")
        self.jog_x_up_button = QtWidgets.QPushButton(self.JOG_Group)
        self.jog_x_up_button.setGeometry(QtCore.QRect(80, 40, 100, 60))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.jog_x_up_button.setFont(font)
        self.jog_x_up_button.setObjectName("jog_x_up_button")
        self.jog_x_down_button = QtWidgets.QPushButton(self.JOG_Group)
        self.jog_x_down_button.setGeometry(QtCore.QRect(80, 200, 100, 60))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.jog_x_down_button.setFont(font)
        self.jog_x_down_button.setObjectName("jog_x_down_button")
        self.jog_z_up_button = QtWidgets.QPushButton(self.JOG_Group)
        self.jog_z_up_button.setGeometry(QtCore.QRect(265, 40, 90, 60))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.jog_z_up_button.setFont(font)
        self.jog_z_up_button.setObjectName("jog_z_up_button")
        self.jog_a_label = QtWidgets.QLabel(self.JOG_Group)
        self.jog_a_label.setGeometry(QtCore.QRect(405, 140, 30, 20))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(14)
        font.setBold(True)
        font.setWeight(75)
        self.jog_a_label.setFont(font)
        self.jog_a_label.setObjectName("jog_a_label")
        self.jog_a_down_button = QtWidgets.QPushButton(self.JOG_Group)
        self.jog_a_down_button.setGeometry(QtCore.QRect(370, 200, 90, 60))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.jog_a_down_button.setFont(font)
        self.jog_a_down_button.setObjectName("jog_a_down_button")
        self.jog_a_up_button = QtWidgets.QPushButton(self.JOG_Group)
        self.jog_a_up_button.setGeometry(QtCore.QRect(370, 40, 90, 60))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.jog_a_up_button.setFont(font)
        self.jog_a_up_button.setObjectName("jog_a_up_button")

        self.jog_azrn_button = QtWidgets.QPushButton(self.JOG_Group)
        self.jog_azrn_button.setGeometry(QtCore.QRect(250, 290, 230, 70))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(13)
        font.setBold(True)
        font.setWeight(75)
        self.jog_azrn_button.setFont(font)
        self.jog_azrn_button.setObjectName("jog_azrn_button")
        self.jog_zero_setting_button = QtWidgets.QPushButton(self.JOG_Group)
        self.jog_zero_setting_button.setGeometry(QtCore.QRect(10, 290, 230, 70))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(13)
        font.setBold(True)
        font.setWeight(75)
        self.jog_zero_setting_button.setFont(font)
        self.jog_zero_setting_button.setObjectName("jog_zero_setting")

        #position
        self.Position_Group = QtWidgets.QGroupBox(HOPE)
        self.Position_Group.setGeometry(QtCore.QRect(550, 30, 341, 111))
        self.Position_Group.setObjectName("Position_Group")
        self.Position_Group.setEnabled(False)
        self.position_x_label = QtWidgets.QLabel(self.Position_Group)
        self.position_x_label.setGeometry(QtCore.QRect(20, 31, 16, 28))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(14)
        font.setBold(True)
        font.setWeight(75)
        self.position_x_label.setFont(font)
        self.position_x_label.setObjectName("position_x_label")
        self.position_y_label = QtWidgets.QLabel(self.Position_Group)
        self.position_y_label.setGeometry(QtCore.QRect(20, 70, 16, 28))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(14)
        font.setBold(True)
        font.setWeight(75)
        self.position_y_label.setFont(font)
        self.position_y_label.setObjectName("position_y_label")
        self.position_z_label = QtWidgets.QLabel(self.Position_Group)
        self.position_z_label.setGeometry(QtCore.QRect(190, 30, 16, 28))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(14)
        font.setBold(True)
        font.setWeight(75)
        self.position_z_label.setFont(font)
        self.position_z_label.setObjectName("position_z_label")
        self.position_a_label = QtWidgets.QLabel(self.Position_Group)
        self.position_a_label.setGeometry(QtCore.QRect(190, 69, 16, 28))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(14)
        font.setBold(True)
        font.setWeight(75)
        self.position_a_label.setFont(font)
        self.position_a_label.setObjectName("position_a_label")
        self.position_x = QtWidgets.QLabel(self.Position_Group)
        self.position_x.setAlignment(QtCore.Qt.AlignRight | QtCore.Qt.AlignVCenter)
        self.position_x.setGeometry(QtCore.QRect(40, 25, 121, 41))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(20)
        self.position_x.setFont(font)
        self.position_x.setObjectName("position_x")
        self.position_y = QtWidgets.QLabel(self.Position_Group)
        self.position_y.setAlignment(QtCore.Qt.AlignRight | QtCore.Qt.AlignVCenter)
        self.position_y.setGeometry(QtCore.QRect(40, 70, 121, 31))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(20)
        self.position_y.setFont(font)
        self.position_y.setObjectName("position_y")
        self.position_z = QtWidgets.QLabel(self.Position_Group)
        self.position_z.setAlignment(QtCore.Qt.AlignRight | QtCore.Qt.AlignVCenter)
        self.position_z.setGeometry(QtCore.QRect(210, 30, 121, 31))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(20)
        self.position_z.setFont(font)
        self.position_z.setObjectName("position_z")
        self.position_a = QtWidgets.QLabel(self.Position_Group)
        self.position_a.setAlignment(QtCore.Qt.AlignRight | QtCore.Qt.AlignVCenter)
        self.position_a.setGeometry(QtCore.QRect(210, 70, 121, 31))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(20)
        self.position_a.setFont(font)
        self.position_a.setObjectName("position_a")

        #Auto
        self.Auto_Group = QtWidgets.QGroupBox(HOPE)
        self.Auto_Group.setGeometry(QtCore.QRect(40, 540, 490, 260))
        self.Auto_Group.setObjectName("Auto_Group")
        self.Auto_Group.setEnabled(False)
        self.File_open_button = QtWidgets.QPushButton(self.Auto_Group)
        self.File_open_button.setEnabled(True)
        self.File_open_button.setGeometry(QtCore.QRect(15, 20, 120, 230))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(15)
        font.setBold(True)
        font.setWeight(75)
        self.File_open_button.setFont(font)
        self.File_open_button.setCheckable(False)
        self.File_open_button.setObjectName("File_open_button")
        self.Auto_start_button = QtWidgets.QPushButton(self.Auto_Group)
        self.Auto_start_button.setEnabled(True)
        self.Auto_start_button.setGeometry(QtCore.QRect(295, 20, 180, 110))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(15)
        font.setBold(True)
        font.setWeight(75)
        self.Auto_start_button.setFont(font)
        self.Auto_start_button.setCheckable(False)
        self.Auto_start_button.setObjectName("Auto_start_button")
        self.Auto_stop_button = QtWidgets.QPushButton(self.Auto_Group)
        self.Auto_stop_button.setEnabled(True)
        self.Auto_stop_button.setGeometry(QtCore.QRect(295, 140, 180, 110))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(15)
        font.setBold(True)
        font.setWeight(75)
        self.Auto_stop_button.setFont(font)
        self.Auto_stop_button.setCheckable(False)
        self.Auto_stop_button.setObjectName("Auto_stop_button")
        self.Option_form_button = QtWidgets.QPushButton(self.Auto_Group)
        self.Option_form_button.setEnabled(True)
        self.Option_form_button.setGeometry(QtCore.QRect(155, 20,  120, 230))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(15)
        font.setBold(True)
        font.setWeight(75)
        self.Option_form_button.setFont(font)
        self.Option_form_button.setCheckable(False)
        self.Option_form_button.setObjectName("Option_form_button")
        self.G_code_upload = QtWidgets.QTextEdit(HOPE)
        self.G_code_upload.setGeometry(QtCore.QRect(550, 160, 361, 600))
        self.G_code_upload.setObjectName("G_code_upload")
        # self.G_code_upload.setEnabled(False)
        self.G_code_read = QtWidgets.QTextEdit(HOPE)
        self.G_code_read.setGeometry(QtCore.QRect(950, 160, 361, 600))
        self.G_code_read.setObjectName("G_code_read")
        # self.G_code_read.setReadOnly(True)
        # self.G_code_read.setEnabled(False)
        self.G_code_progressBar = QtWidgets.QProgressBar(HOPE)
        self.G_code_progressBar.setGeometry(QtCore.QRect(550, 780, 760, 33))
        self.G_code_progressBar.setProperty("value", 0)
        self.G_code_progressBar.setObjectName("G_code_progressBar")

        # 스탑워치
        self.Timer_number = QtWidgets.QLCDNumber(HOPE)
        self.Timer_number.setGeometry(QtCore.QRect(1000, 70, 251, 51))
        self.Timer_number.setMouseTracking(False)
        self.Timer_number.setFrameShape(QtWidgets.QFrame.NoFrame)
        self.Timer_number.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.Timer_number.setDigitCount(9)
        self.Timer_number.setSegmentStyle(QtWidgets.QLCDNumber.Flat)
        self.Timer_number.setProperty("value", 0.0)
        self.Timer_number.setObjectName("Timer_number")
        self.Timer_label = QtWidgets.QLabel(HOPE)
        self.Timer_label.setGeometry(QtCore.QRect(1110, 30, 61, 31))

        # self.webEngineView = QWebEngineView(HOPE)  # 추가
        # self.webEngineView.setGeometry(QtCore.QRect(840, 110, 691, 541))
        # self.webEngineView.setUrl(QtCore.QUrl("https://nraynaud.github.io/webgcode/"))
        # self.webEngineView.setObjectName("webEngineView")

        self.retranslateUi(HOPE)
        QtCore.QMetaObject.connectSlotsByName(HOPE)

    def retranslateUi(self, HOPE):  # 버튼 및 객체들 선언하는 부분?
        _translate = QtCore.QCoreApplication.translate
        HOPE.setWindowTitle(_translate("HOPE", "Hybrid Optimum Printer Engineering"))
        self.refresh_ports_button.setText(_translate("HOPE", "Refresh ports"))
        self.Quit_button.setText(_translate("HOPE", "Quit"))
        self.serial_port_label.setText(_translate("HOPE",
                                                  "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">Serial Port :</span></p></body></html>"))
        self.Microstepping_label.setText(_translate("HOPE",
                                                    "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">Microstepping : </span></p></body></html>"))
        self.JOG_Group.setTitle(_translate("HOPE", "<JOG>"))
        self.jog_y_right_button.setText(_translate("HOPE", "→"))
        self.jog_z_label.setText(_translate("HOPE", "Z"))
        self.jog_y_left_button.setText(_translate("HOPE", "←"))
        self.jog_xy_label.setText(_translate("HOPE", "X/Y"))
        self.jog_z_down_button.setText(_translate("HOPE", "↓"))
        self.jog_x_up_button.setText(_translate("HOPE", "↑"))
        self.jog_x_down_button.setText(_translate("HOPE", "↓"))
        self.jog_z_up_button.setText(_translate("HOPE", "↑"))
        self.jog_a_label.setText(_translate("HOPE", "A"))
        self.jog_a_down_button.setText(_translate("HOPE", "↓"))
        self.jog_a_up_button.setText(_translate("HOPE", "↑"))
        self.jog_azrn_button.setText(_translate("HOPE", "AZRN"))
        self.jog_zero_setting_button.setText(_translate("Dialog", "Zero Setting"))
        self.Position_Group.setTitle(_translate("HOPE", "<Position>"))
        self.position_x_label.setText(_translate("HOPE", "X"))
        self.position_y_label.setText(_translate("HOPE", "Y"))
        self.position_z_label.setText(_translate("HOPE", "Z"))
        self.position_a_label.setText(_translate("HOPE", "A"))
        self.position_x.setText(_translate("Dialog", "000"))
        self.position_y.setText(_translate("Dialog", "000"))
        self.position_z.setText(_translate("Dialog", "000"))
        self.position_a.setText(_translate("Dialog", "000"))
        self.connect_button.setText(_translate("HOPE", "Connect"))
        self.Auto_Group.setTitle(_translate("HOPE", "<Auto>"))
        self.File_open_button.setText(_translate("HOPE", "File Open"))
        self.Auto_start_button.setText(_translate("HOPE", "Auto Start"))
        self.Auto_stop_button.setText(_translate("HOPE", "Auto Stop"))
        self.Option_form_button.setText(_translate("HOPE", "G_code\ntransfer\noption"))
        self.Timer_label.setText(_translate("HOPE", "Timer"))



