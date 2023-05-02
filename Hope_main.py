import time
import sys

from PyQt5 import QtWidgets

import Hope2_UI
import Hope_UI


class CannotConnectException(Exception):
    pass


class UI_Window(QtWidgets.QMainWindow, Hope_UI.Ui_HOPE):
    def __init__(self):
        from tools.Hope_Port import port_hope
        from tools.Hope_Microstepping import microstepping_hope
        from tools.Hope_jog import jog_hope
        from tools.Hope_G_Code import G_code_hope

        super(UI_Window, self).__init__()

        self.ui = Hope_UI.Ui_HOPE()
        self.ui.setupUi(self)

        # self.ui2 = Hope2_UI.Ui_HOPE2()

        self.portClass = port_hope(self, self.ui)
        self.microsteppingClass = microstepping_hope(self, self.ui)
        self.jogClass = jog_hope(self, self.ui)
        self.G_codeClass = G_code_hope(self, self.ui)

        self.ui.connect_button.clicked.connect(self.connect)
        self.ui.Quit_button.clicked.connect(self.Quit)

        self.port = self.portClass.get_port()

    # Todo represh버튼 누른후 connect 오류 -> self.port 추가로 해결(Hope_jog에서 self.port = self.main.port)로인한 에러
    def connect(self):
        self.port = self.portClass.get_port()
        self.statusBar().showMessage("새로운 포트를 찾았습니다.")
        try:
            port_declared = self.port in vars()
            try:
                from tools import Hope_Arduino
                self.arduino = Hope_Arduino.arduino_hope(self, self.ui)
                self.serial = self.arduino.get_serial()

                self.ui.connect_button.setEnabled(False)
                time.sleep(1.2)
                self.statusBar().showMessage("아두이노와 연결이 되었습니다. ")
                self.ui.JOG_Group.setEnabled(True)
                self.ui.Position_Group.setEnabled(True)
                self.ui.Auto_Group.setEnabled(True)
            except:
                self.statusBar().showMessage("아두이노와 연결이 되지 않았습니다.")
                print("아두이노와 연결이 되지 않았습니다.")
                raise CannotConnectException

        except AttributeError:
            self.statusBar().showMessage("보드를 연결 시킨 후 정확하게 포트와 연결하세요.")

    def Quit(self):
        print("Program Closed")
        sys.exit()


def main():
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Window = UI_Window()
    Window.setWindowTitle("HOPE")
    # Window.show()
    Window.showMaximized()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
