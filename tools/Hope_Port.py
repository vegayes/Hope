import glob
import sys

import serial

import Hope_UI
import Hope_main


class port_hope:

    def __init__(self, main: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        self.main = main
        self.ui = ui
        self.init_start()

    def init_start(self):
        self.populate_ports()
        self.__set_port()
        self.ui.refresh_ports_button.clicked.connect(self.refresh_ports)

    def populate_ports(self):
        print("Serial Port 찾는 중... ")
        if sys.platform.startswith('win'):
            ports = ['COM%s' % (i + 1) for i in range(256)]
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            ports = glob.glob('/dev/tty[A-Za-z]')
        elif sys.platform.startswith('darwin'):
            ports = glob.glob('/dev/tty.')
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

    def refresh_ports(self):

        self.main.statusBar().showMessage("포트를 다시 찾았습니다.")
        self.ui.serial_port_combo.clear()
        self.populate_ports()
        self.__set_port()

    def __set_port(self):
        self.port = self.ui.serial_port_combo.currentText()

    def get_port(self):
        return self.port
