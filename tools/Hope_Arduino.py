import serial

import Hope_UI
import Hope_main
import time

class arduino_hope:
    def __init__(self, main: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        self.main = main
        self.ui = ui
        self.setting_arduino()
        self.receive_data()

    def read(self):
        while True:
            time.sleep(0.1)
            time_value = self.serial.readline().decode()
            print(time_value)

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
        self.receive_data()

    def get_serial(self):
        return self.serial

    def send_data(self, message):
        message = message.encode("utf-8")
        self.serial.write(message)

    # 입력 받은 값
    # 알 수 없는 공백의 추가로 두번 받아들임
    # Todo 공백 해결
    def receive_data(self):
        self.m = self.serial.readline()
        print(self.m[:len(self.m) - 1].decode())

