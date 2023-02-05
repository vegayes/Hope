import Hope_UI
import Hope_main

from PyQt5.QtWidgets import *


class G_code_hope:

    def __init__(self, main: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        self.main = main
        self.ui = ui

        self.ui.File_open_button.clicked.connect(self.open_button)
        self.ui.Auto_start_button.clicked.connect(self.Auto_start)
        self.ui.Auto_stop_button.clicked.connect(self.Auto_stop)


    def file_name(self):  # 파일 명이 항상 다르니까.. 파일명을 확인하는 함수가 필요한가?
        pass

    def open_button(self):
        # file = self.ui.open_button.askopenfile(initialdir='path', title='select file', filetypes=(('png files', '*.png'), ('all files', '*.*')))
        # fname = self.ui.QFileDialog.getOpenFileName(self)
        fname = QFileDialog.getOpenFileName(self.main, 'Open file', './')

        # file = open(fname, 'r')        #다른 방법 시도 But 뭐가 문제인지 모르겠음.
        #
        # with file:
        #     text = file.read()
        #     self.Hope_UI.Ui_HOPE.G_code_upload.setText(text)

        # for line in open(self.fname, 'r'):
        #     line = line.strip()
        #     self.Hope_UI.Ui_HOPE.G_code_upload(line)

        print(type(fname), fname)
        # self.ui.G_code_upload.setText(fname[0])

        if fname[0]:
            f = open(fname[0], 'r', encoding='UTF8')
            with f:
                data = f.read()
                data = data[:-4]  # 맨 마지막 줄 없애기 M30
                # data = data.replace('M30', '') # M30지우기 but

                self.ui.G_code_upload.setText(data)
                self.ui.G_code_upload.append("G01 X0 Y0 Z10 A90") # 원점 복귀
                self.ui.G_code_upload.append("G01 Z0 A0") # 원점 복귀
                self.ui.G_code_upload.append("M30")  # 끝내기


        if fname[0]:
            print("파일이 선택되었으며 경로는 아래와 같음")
            print(fname[0])
        else:
            print("파일을 고르지 않음")

    def Auto_start(self):
        pass

    def Auto_stop(self):
        self.arduino = self.main.arduino
        self.arduino.send_data('p')
        self.main.statusBar().showMessage("S T O P")
        self.arduino.receive_data()


