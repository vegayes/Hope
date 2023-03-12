from PyQt5.QtCore import QThread, pyqtSignal
import Hope_main
import Hope_UI
from display import Hope_Position


class ReceiveData(QThread):

    def __init__(self, parent: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE, serial):
        super().__init__(parent)
        self.count = None
        self.main = parent
        self.ui = ui
        self.serial = serial  # self.serial = self.main.serial 도 같은 의미일듯?
        self.pos = Hope_Position.position_hope(parent, ui)

        self.count = 0

    def run(self):
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
                    # G코드 전용
                    elif diving_point == 'g':
                        self.progress_print(text)
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
        elif axis == 'Y':
            self.pos.set_y_position(int(value))
        elif axis == 'Z':
            self.pos.set_z_position(int(value))
        elif axis == 'A':
            self.pos.set_a_position(int(value))

    def progress_print(self, text):
        # GCode 한줄이 완성되었을 때
        # 처음에 1을 받아야함
        text = text.strip("<g>")
        self.count = text
        print("<G>" + self.count)

    def get_progress_count(self):
        return int(self.count)
