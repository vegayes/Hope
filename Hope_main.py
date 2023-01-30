import time

from PyQt5 import QtWidgets

import Hope_UI


class CannotConnectException(Exception):
    pass


class UI_Window(QtWidgets.QMainWindow, Hope_UI.Ui_HOPE):
    def __init__(self):
        from tools.Hope_Port import port_hope
        from tools.Hope_Microstepping import microstepping_hope
        from tools.Hope_jog import jog_hope
        from tools.Hope_G_code import G_code_hope

        super(UI_Window, self).__init__()

        self.ui = Hope_UI.Ui_HOPE()
        self.ui.setupUi(self)

        self.portClass = port_hope(self, self.ui)
        self.microsteppingClass = microstepping_hope(self, self.ui)
        self.jogClass = jog_hope(self, self.ui)
        self.G_codeClass = G_code_hope(self, self.ui)

        self.ui.connect_button.clicked.connect(self.connect)

        self.port = self.portClass.get_port()

    # Todo represh버튼 누른후 connect 오류 -> self.port 추가로 해결(Hope_jog에서 self.port = self.main.port)로인한 에러
    def connect(self):
        # self.port_nano = '/dev/cu.usbserial-A9M11B77'
        # self.port_uno = "/dev/cu.usbmodem1411"
        # self.baudrate = baudrate
        self.port = self.portClass.get_port()
        self.statusBar().showMessage("새로운 포트를 찾았습니다.")
        try:
            port_declared = self.port in vars()
            try:
                from tools import Hope_Arduino
                self.arduino = Hope_Arduino.arduino_hope(self, self.ui)
                self.serial = self.arduino.get_serial()

                # This is a thread that always runs and listens to commands from the Arduino
                # self.global_listener_thread = Thread(self.listening)
                # self.global_listener_thread.finished.connect(lambda:self.self.thread_finished(self.global_listener_thread))
                # self.global_listener_thread.start()

                # ~~~~
                # TAB : Setup
                # ~~~~
                # self.ui.disconnect_BTN.setEnabled(True) 연결 끊기

                self.ui.connect_button.setEnabled(False)
                time.sleep(3)
                self.statusBar().showMessage("아두이노와 연결이 되었습니다. ")
            except:
                self.statusBar().showMessage("아두이노와 연결이 되지 않았습니다.")
                print("아두이노와 연결이 되지 않았습니다.")
                raise CannotConnectException
        except AttributeError:
            self.statusBar().showMessage("보드를 연결 시킨 후 정확하게 포트와 연결하세요.")


def main():
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Window = UI_Window()
    Window.setWindowTitle("HOPE")
    Window.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
