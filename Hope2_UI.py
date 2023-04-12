# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'C:\Users\vegay\Desktop\졸업작품\HOPE2\HOPE2.ui'
#
# Created by: PyQt5 UI code generator 5.9.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_HOPE2(object):
    def setupUi(self, HOPE2):
        HOPE2.setObjectName("HOPE2")
        HOPE2.resize(730, 369)
        self.Type_Group = QtWidgets.QGroupBox(HOPE2)
        self.Type_Group.setGeometry(QtCore.QRect(40, 70, 191, 241))
        self.Type_Group.setObjectName("Type_Group")
        self.type1_radio_button = QtWidgets.QRadioButton(self.Type_Group)
        self.type1_radio_button.setGeometry(QtCore.QRect(21, 33, 88, 49))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(22)
        self.type1_radio_button.setFont(font)
        self.type1_radio_button.setChecked(True)
        self.type1_radio_button.setAutoRepeatDelay(300)
        self.type1_radio_button.setObjectName("type1_radio_button")
        self.type2_radio_button = QtWidgets.QRadioButton(self.Type_Group)
        self.type2_radio_button.setGeometry(QtCore.QRect(21, 101, 140, 49))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(22)
        self.type2_radio_button.setFont(font)
        self.type2_radio_button.setChecked(False)
        self.type2_radio_button.setAutoRepeatDelay(300)
        self.type2_radio_button.setObjectName("type2_radio_button")
        self.type3_radio_button = QtWidgets.QRadioButton(self.Type_Group)
        self.type3_radio_button.setGeometry(QtCore.QRect(21, 169, 142, 49))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(22)
        self.type3_radio_button.setFont(font)
        self.type3_radio_button.setChecked(False)
        self.type3_radio_button.setAutoRepeatDelay(300)
        self.type3_radio_button.setObjectName("type3_radio_button")

        #Option
        self.Opion_Group = QtWidgets.QGroupBox(HOPE2)
        self.Opion_Group.setGeometry(QtCore.QRect(270, 70, 391, 241))
        self.Opion_Group.setObjectName("Opion_Group")
        self.Option_Setting_Button = QtWidgets.QPushButton(self.Opion_Group)
        self.Option_Setting_Button.setGeometry(QtCore.QRect(300, 30, 71, 191))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(16)
        self.Option_Setting_Button.setFont(font)
        self.Option_Setting_Button.setObjectName("Option_Setting_Button")

        self.Option_Structure_Position_label = QtWidgets.QLabel(self.Opion_Group)
        self.Option_Structure_Position_label.setGeometry(QtCore.QRect(70, 20, 180, 41))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(16)
        self.Option_Structure_Position_label.setFont(font)
        self.Option_Structure_Position_label.setObjectName("Option_Structure_Information_label")

        self.Width_Edit = QtWidgets.QTextEdit(self.Opion_Group)
        self.Width_Edit.setGeometry(QtCore.QRect(100, 130, 180, 41))
        self.Width_Edit.setObjectName("Width_Edit")
        self.Width_label = QtWidgets.QLabel(self.Opion_Group)
        self.Width_label.setGeometry(QtCore.QRect(20, 130, 71, 41))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(18)
        self.Width_label.setFont(font)
        self.Width_label.setObjectName("Width_label")

        self.Height_Edit = QtWidgets.QTextEdit(self.Opion_Group)
        self.Height_Edit.setGeometry(QtCore.QRect(100, 180, 180, 41))
        self.Height_Edit.setObjectName("Height_Edit")
        self.Height_label = QtWidgets.QLabel(self.Opion_Group)
        self.Height_label.setGeometry(QtCore.QRect(20, 180, 71, 41))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(18)
        self.Height_label.setFont(font)
        self.Height_label.setObjectName("Height_label")


        self.Position_Y_Edit = QtWidgets.QTextEdit(self.Opion_Group)
        self.Position_Y_Edit.setGeometry(QtCore.QRect(100, 75, 180, 41))
        self.Position_Y_Edit.setObjectName("Position_Y_Edit")
        self.Option_Y_lable = QtWidgets.QLabel(self.Opion_Group)
        self.Option_Y_lable.setGeometry(QtCore.QRect(10, 70, 80, 50))
        font = QtGui.QFont()
        font.setFamily("Agency FB")
        font.setPointSize(15)
        self.Option_Y_lable.setFont(font)
        self.Option_Y_lable.setObjectName("Option_Y_lable")

        # self.Position_X_Edit = QtWidgets.QTextEdit(self.Opion_Group)
        # self.Position_X_Edit.setGeometry(QtCore.QRect(40, 75, 101, 41))
        # self.Position_X_Edit.setObjectName("Position_X_Edit")
        # self.Option_X_lable = QtWidgets.QLabel(self.Opion_Group)
        # self.Option_X_lable.setGeometry(QtCore.QRect(10, 70, 31, 41))
        # font = QtGui.QFont()
        # font.setFamily("Agency FB")
        # font.setPointSize(18)
        # self.Option_X_lable.setFont(font)
        # self.Option_X_lable.setObjectName("Option_X_lable")


        self.retranslateUi(HOPE2)
        QtCore.QMetaObject.connectSlotsByName(HOPE2)

    def retranslateUi(self, HOPE2):
        _translate = QtCore.QCoreApplication.translate
        HOPE2.setWindowTitle(_translate("HOPE2", "G_code_option"))
        self.Type_Group.setTitle(_translate("HOPE2", "Type"))
        self.type1_radio_button.setText(_translate("HOPE2", "Plane"))
        self.type2_radio_button.setText(_translate("HOPE2", "45 degree"))
        self.type3_radio_button.setText(_translate("HOPE2", "90 degree"))
        self.Opion_Group.setTitle(_translate("HOPE2", "Option"))
        self.Height_label.setText(_translate("HOPE2", "Height :"))
        self.Option_Setting_Button.setText(_translate("HOPE2", "Setting"))
        self.Width_label.setText(_translate("HOPE2", "Width  :"))
        self.Option_Structure_Position_label.setText(_translate("HOPE2", "Structure_Information"))
        # self.Option_X_lable.setText(_translate("HOPE2", "X :"))
        self.Option_Y_lable.setText(_translate("HOPE2", "Structure:\n  Position"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    HOPE2 = QtWidgets.QDialog()
    ui = Ui_HOPE2()
    ui.setupUi(HOPE2)
    HOPE2.show()
    sys.exit(app.exec_())

