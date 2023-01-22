import serial

import Hope_UI
import Hope_main


class arduino_hope:
    def __init__(self, main: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        self.main = main
        self.ui = ui
        self.setting_arduino()

    def setting_arduino(self):
        self.serial = serial.Serial()
        self.serial.port = self.main.port
        self.serial.baudrate = 230400
        self.serial.parity = serial.PARITY_NONE
        self.serial.stopbits = serial.STOPBITS_ONE
        self.serial.bytesize = serial.EIGHTBITS
        self.serial.timeout = 1
        self.serial.open()
        self.serial.flushInput()

    def send_data(self, message):
         self.serial.write(message)