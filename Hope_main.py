import time

import serial
from PyQt5 import QtWidgets

import Hope_UI


class CannotConnectException(Exception):
    pass


class UI_Window(QtWidgets.QMainWindow, Hope_UI.Ui_HOPE):
    def __init__(self):
        from tools.Hope_Port import port_hope
        from tools.Hope_Microstepping import microstepping_hope
        from tools.Hope_jog import jog_hope

        super(UI_Window, self).__init__()

        self.ui = Hope_UI.Ui_HOPE()
        self.ui.setupUi(self)

        self.portClass = port_hope(self, self.ui)
        self.microsteppingClass = microstepping_hope(self, self.ui)
        self.jogClass = jog_hope(self, self.ui)

        self.ui.connect_button.clicked.connect(self.connect)

        self.port = self.portClass.get_port()

    def connect(self):
        # self.port_nano = '/dev/cu.usbserial-A9M11B77'
        # self.port_uno = "/dev/cu.usbmodem1411"
        # self.baudrate = baudrate
        self.statusBar().showMessage("You clicked CONNECT TO CONTROLLER")
        try:
            port_declared = self.port in vars()
            try:
                from tools import Hope_Arduino
                self.arduino = Hope_Arduino.arduino_hope(self, self.ui).get_serial()

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
                raise CannotConnectException
        except AttributeError:
            self.statusBar().showMessage("보드를 연결 시킨 후 정확하게 포트와 연결하세요.")


def main():
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Window = UI_Window()
    Window.setWindowTitle("Poseidon Pumps Controller - Pachter Lab Caltech 2018")
    Window.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
