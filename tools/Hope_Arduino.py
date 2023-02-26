import serial

import Hope_UI
import Hope_main
import time
from threading import Thread

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
        th1 = Thread(target=self.loop_receive_data, daemon=True)
        th1.start()

    def get_serial(self):
        return self.serial

    def send_data(self, message):
        message = message.encode("utf-8")
        self.serial.write(message)

    def loop_receive_data(self):
        while True:
            if self.serial.readable():
                try:
                    self.m = self.serial.readline()
                    text = self.m[:len(self.m) - 1].decode()
                    diving_point = None

                    if text.find('<') != -1:
                        diving_point = text[text.find('<') + 1]

                    # 콘솔 전용
                    if diving_point == 'c':
                        self.console_print(text)
                    # 디스플레이 전용
                    elif diving_point == 'd':
                        self.display_print(text)
                    # 그외 -> 콘솔로 보냄
                    else:
                        self.console_print(text)

                except UnicodeDecodeError:
                    print("인코딩 에러 발생 : ", self.m)


    def console_print(self, text):
        if len(text) != 0:
            text = text.strip("<c>")
            print(text)

    def display_print(self, text):
        text = text.strip("<d>")
        axis = text[text.find('축') - 1]
        value = text[text.find(':') + 1:]

        if axis == 'X':
            self.pos.set_x_position(int(value))

