import serial

import Hope_UI
import Hope_main
import time
from threads import ReceiveData

from display import Hope_Position


class arduino_hope:
    def __init__(self, main: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        self.main = main
        self.ui = ui

        self.pos = Hope_Position.position_hope(main, ui)

        self.setting_arduino()

    # Todo
    def setting_arduino(self):
        self.serial = serial.Serial()
        self.serial.port = self.main.port
        self.serial.baudrate = 115200
        self.serial.parity = serial.PARITY_NONE
        self.serial.stopbits = serial.STOPBITS_ONE
        self.serial.bytesize = serial.EIGHTBITS
        self.serial.timeout = 0.1
        self.serial.open()
        self.serial.flushInput()
        time.sleep(0.5)
        self.ReceiveData = ReceiveData.ReceiveData(self.main, self.ui, self.serial)
        self.ReceiveData.start()

    def get_serial(self):
        return self.serial

    def send_data(self, message: str):
        message = message.encode("utf-8")
        self.serial.write(message)

    def getReceiveDateThread(self):
        # progressbar에서 값을 얻어올떄 ReceiveDate와 같은 쓰레드가 필요해서 받아옴
        return self.ReceiveData
