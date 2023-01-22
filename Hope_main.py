import time

import serial
from PyQt5 import QtWidgets

import Hope_UI


class CannotConnectException(Exception):
    pass


class UI_Window(QtWidgets.QMainWindow, Hope_UI.Ui_HOPE):
    def __init__(self):
        from tools.Hope_Port import port_hope
        super(UI_Window, self).__init__()

        self.ui = Hope_UI.Ui_HOPE()
        self.ui.setupUi(self)

        self.ports = port_hope(self, self.ui)

        self.ports.populate_ports()
        self.populate_microstepping()
        self.ports.set_port()

        self.ui.refresh_ports_button.clicked.connect(self.ports.refresh_ports)
        self.ui.connect_button.clicked.connect(self.connect)

        # Declaring start, mid, and end marker for sending code to Arduino / Arduino에 코드를 보내기 위한 시작, 중간 및 끝 마커 선언
        self.startMarker = 60  # <
        self.endMarker = 62  # ,F,0.0>
        self.midMarker = 44  # ,

    def populate_microstepping(self):
        self.microstepping_values = ['1', '2', '4', '8', '16', '32']
        self.ui.Microstepping_combo.addItems(self.microstepping_values)
        self.microstepping = 1

    def connect(self):
        # self.port_nano = '/dev/cu.usbserial-A9M11B77'
        # self.port_uno = "/dev/cu.usbmodem1411"
        # self.baudrate = baudrate
        self.statusBar().showMessage("You clicked CONNECT TO CONTROLLER")
        try:
            port_declared = self.port in vars()
            try:
                self.serial = serial.Serial()
                self.serial.port = self.port
                self.serial.baudrate = 230400
                self.serial.parity = serial.PARITY_NONE
                self.serial.stopbits = serial.STOPBITS_ONE
                self.serial.bytesize = serial.EIGHTBITS
                self.serial.timeout = 1
                self.serial.open()
                self.serial.flushInput()

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

    def recvPositionArduino(self):  # distance 기능 ??
        startMarker = self.startMarker
        midMarker = self.midMarker
        endMarker = self.endMarker

        ck = ""
        x = "z"  # any value that is not an end- or startMarker

        # wait for the start character
        while ord(x) != startMarker:
            x = self.serial.read()

        # save data until the end marker is found
        while ord(x) != endMarker:
            if ord(x) == midMarker:
                print(ck)
                self.ui.position_x.display(ck)
                ck = ""
                x = self.serial.read()

            if ord(x) != startMarker:
                # print(x)
                ck = ck + x.decode()

            x = self.serial.read()

        return (ck)


def main():
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Window = UI_Window()
    Window.setWindowTitle("Poseidon Pumps Controller - Pachter Lab Caltech 2018")
    Window.show()
    # HOPE = QtWidgets.QDialog()
    # ui = UiHOPE()
    # ui.setupUi(HOPE)
    # HOPE.show()
    sys.exit(app.exec())


if __name__ == "__main__" :
    main()
