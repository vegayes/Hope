import Hope_UI
import Hope_main
from threading import Thread
from PyQt5.QtWidgets import *
import time


class G_code_hope:

    def __init__(self, main: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        self.main = main
        self.ui = ui
        self.ok = True
        self.count = 0

        self.ui.File_open_button.clicked.connect(self.open_button)
        self.ui.Auto_start_button.clicked.connect(self.Auto_start)
        self.ui.Auto_stop_button.clicked.connect(self.Auto_stop)

    def file_name(self):  # 파일 명이 항상 다르니까.. 파일명을 확인하는 함수가 필요한가?
        pass

    def open_button(self):
        fname = QFileDialog.getOpenFileName(self.main, 'Open file', './')
        print(type(fname), fname)

        if fname[0]:
            f = open(fname[0], 'r', encoding='UTF8')
            with f:
                data = f.read()
                data = data[:-4]  # 맨 마지막 줄 없애기 M30
                # data = data.replace('M30', '') # M30지우기 but

                self.ui.G_code_upload.setText(data)
                self.ui.G_code_upload.append("G01 X0 Y0 Z10 A90")  # X Y 원점 복귀 혹시 몰라서 A축이랑 Z축 그려지지 않게 올림.
                self.ui.G_code_upload.append("G01 Z0 A0")  # Z A 원점 복귀
                self.ui.G_code_upload.append("M30")  # 끝내기
        if fname[0]:
            print("파일이 선택되었으며 경로는 아래와 같음")
            print(fname[0])
        else:
            print("파일을 고르지 않음")

    # def readline(self):  # G코드를 가져와서 읽게 한다.  즉, 아두이노한테 보내면서 보내고 있는 값들을 read edit에 추가한다.
    #     text = self.ui.G_code_upload.toPlainText()
    #     text = text.splitlines()
    #     print(text)

    def Auto_start(self):  # Auto Start 버튼을 누르면 텍스트 값이 이동을 함.
        self.main.statusBar().showMessage("AUTO START")
        self.arduino = self.main.arduino
        text = self.ui.G_code_upload.toPlainText()
        text = text.splitlines()
        self.ok = True

        if self.count == 0:
            self.count = 1
            th1 = Thread(target=self.thread, args=(text,))
            th1.start()

        # for Gline in text:
        #     Gline = Gline + ";"
        #     self.arduino.send_data(Gline)
        #     time.sleep(0.25)
        #     self.arduino.receive_data()

    def thread(self, text):
        for Gline in text:
            if self.ok:
                Gline = Gline + ";\n"
                self.arduino.send_data(Gline)
                self.arduino.receive_data()
                print(Gline)
                time.sleep(1)
            else:
                while not self.ok:
                    pass
                Gline = Gline + ";"
                self.arduino.send_data(Gline)
                time.sleep(1)
                self.arduino.receive_data()
                # print(Gline)

    def Auto_stop(self):
        self.arduino = self.main.arduino
        self.arduino.send_data('p')
        self.main.statusBar().showMessage("S T O P")
        self.arduino.receive_data()
        self.ok = False
