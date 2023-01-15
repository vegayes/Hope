import glob
import sys
import time

import serial
from PyQt5.QtWidgets import *
from PyQt5 import QtWidgets

import Hope_UI
from Hope_UI import Ui_HOPE

class CannotConnectException(Exception):
	pass


class UI_Window(QtWidgets.QMainWindow, Hope_UI.Ui_HOPE):
    def __init__(self):
        super(UI_Window, self).__init__()
        self.ui = Hope_UI.Ui_HOPE()
        self.ui.setupUi(self)

        self.populate_ports()
        self.populate_microstepping()
        self.set_port()

        self.ui.refresh_ports_button.clicked.connect(self.refresh_ports)
        self.ui.connect_button.clicked.connect(self.connect)



    # Populate the available ports
    def populate_ports(self):
        """
            :raises EnvironmentError:
                On unsupported or unknown platforms
            :returns:
                A list of the serial ports available on the system
        """
        print("Serial Port 찾는 중... ")
        if sys.platform.startswith('win'):
            ports = ['COM%s' % (i + 1) for i in range(256)]
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            # this excludes your current terminal "/dev/tty"
            ports = glob.glob('/dev/tty[A-Za-z]*')
        elif sys.platform.startswith('darwin'):
            ports = glob.glob('/dev/tty.*')
        else:
            raise EnvironmentError('Unsupported platform')

        result = []
        for port in ports:
            try:
                s = serial.Serial(port)
                s.close()
                result.append(port)
            except (OSError, serial.SerialException):
                pass
        self.ui.serial_port_combo.addItems(result)
        print(f"{result} 발견됨")

        # Refresh the list of ports

    def refresh_ports(self):
        self.statusBar().showMessage("You clicked REFRESH PORTS")
        self.ui.serial_port_combo.clear()
        self.populate_ports()
        self.set_port()

    def set_port(self):
        self.port = self.ui.serial_port_combo.currentText()

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

                # ~~~~~~~~~~~~~~~~
                # TAB : Setup
                # ~~~~~~~~~~~~~~~~
                # self.ui.disconnect_BTN.setEnabled(True) 연결 끊기

                self.ui.connect_button.setEnabled(False)
                time.sleep(3)
                self.statusBar().showMessage("Successfully connected to board.")
            except:
                self.statusBar().showMessage("Cannot connect to board. Try again..")
                raise CannotConnectException
        except AttributeError:
            self.statusBar().showMessage("Please plug in the board and select a proper port, then press connect.")


def main():
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Window = UI_Window()
    Window.setWindowTitle("Poseidon Pumps Controller - Pachter Lab Caltech 2018")
    Window.show()
    # HOPE = QtWidgets.QDialog()
    # ui = Ui_HOPE()
    # ui.setupUi(HOPE)
    # HOPE.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
