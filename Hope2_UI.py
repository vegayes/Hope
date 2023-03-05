# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'C:\Users\vegay\Desktop\HOPE2\HOPE2.ui'
#
# Created by: PyQt5 UI code generator 5.9.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_HOPE2(object):
    def setupUi(self, HOPE2):
        HOPE2.setObjectName("HOPE2")
        HOPE2.resize(652, 453)

        self.retranslateUi(HOPE2)
        QtCore.QMetaObject.connectSlotsByName(HOPE2)

    def retranslateUi(self, HOPE2):
        _translate = QtCore.QCoreApplication.translate
        HOPE2.setWindowTitle(_translate("HOPE2", "G_code_option"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    HOPE2 = QtWidgets.QDialog()
    ui = Ui_HOPE2()
    ui.setupUi(HOPE2)
    HOPE2.show()
    sys.exit(app.exec_())

