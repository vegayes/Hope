from PyQt5.QtWidgets import *

import Hope_UI
import Hope_main
import re
from display import Hope_ProgressBar
from threads import progressbar


class G_code_hope:

    def __init__(self, main: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        self.main = main
        self.ui = ui

        self.pbar = Hope_ProgressBar.ProgressBar_Hope(main, ui)
        self.progressThread = progressbar.ProGressBar_Thread(self.main, self.ui)

        self.ui.File_open_button.clicked.connect(self.open_button)
        self.ui.Auto_start_button.clicked.connect(self.Auto_start)
        self.ui.Auto_stop_button.clicked.connect(self.Auto_stop)
        self.ui.Option_form_button.clicked.connect(self.ui.openHope2)

    def open_button(self):
        # global fname

        # self.progressThread.reset()   # 그냥 open 버튼을 누를 때마다

        fname = QFileDialog.getOpenFileName(self.main, 'Open file', './')
        print(type(fname), fname)

        if fname[0]:
            f = open(fname[0], 'r', encoding='UTF8')


            with f:
                file= f.read()

                print(file)

                # file = re.sub(r'\([^()]+?\)', '', file)
                # file = re.sub(r'\([^()]*\)', '', file)
                # data_list = data.split()

                lines = [line for line in file.splitlines() if line.strip()] #빈 줄 제거
                print(lines)

                data = '\n'.join(lines)
                print(data)




                print('\n')
                print('\n')
                print(data)

                # data = [dataline.strip() for line in dataline if line.strip()]
                ## data = data[:-4]  # 맨 마지막 줄 없애기 M30 **
                # data = data.replace('M30', '') # M30지우기 but

                data = data.replace('M300 S30', 'G01 Z0 F1000') #feed 값 변경하길.
                data = data.replace('M300 S50', 'G01 Z10 F1000')
                data = data.replace('M17', '')
                data = data.replace('M18', '')


                if self.progressThread.isRunning():
                    # self.progressThread.runPause()
                    self.progressThread.reset()

                self.ui.G_code_upload.setText(data)
                # G코드에서 변경해야 할 값.
                # 1) G0, 1, 4 제외하고 G21, 90, 92없애야 함. (G4는 딜레이 코드, G4 P1000은 1000ms동안 딜레이)
                # 2) M300 S30 -> Z값 원점기준 0으로 바꿔야함 O
                # 3) M300 S50 (pen up) -> Z값 원점기준 10?으로 바꿔야함 O
                # 4) M300을 제외한 다른 M코드(M17, 18) 없애야함0
                # 5) 엔터 후 공백 공간 없애야함
                # 7) 원점으로 복귀하는 코드 넣기 O
                # 6) 맨 밑에 M30코드 입력 O
                # 8) 괄호 사이의 존재하는 값 다 삭제 (end of print job)
                self.ui.G_code_upload.append("G01 X0 Y0 Z10 A90")  # X Y 원점 복귀 혹시 몰라서 A축이랑 Z축 그려지지 않게 올림.
                self.ui.G_code_upload.append("G01 Z0 A0")  # Z A 원점 복귀
                self.ui.G_code_upload.append("M30")  # 끝내기 및 원래 Speed 및 Accel 값 다시 설정



    # def Auto_start(self):  # Auto Start 버튼을 누르면 텍스트 값이 이동을 함.
    #     # self.main.statusBar().showMessage("AUTO START")       # 에러 뜸
    #     self.arduino = self.main.arduino
    #     text = self.ui.G_code_upload.toPlainText()
    #     lines = text.splitlines()
    #
    #
    #     if not self.progressThread.isRunning():
    #             self.progressThread.setText(lines)
    #             self.progressThread.finished.connect(self.pbar.setValue)
    #             self.progressThread.start()
    #     elif fname[0] == current_file:
    #         print('이어서 시작')
    #         self.progressThread.reRun()
    #
    #     elif fname[0] != current_file:
    #         print('새로 시작')
    #         self.progressThread.setText(lines)
    #         self.progressThread.finished.connect(self.pbar.setValue)
    #         self.progressThread.start()

    def Auto_start(self):  # Auto Start 버튼을 누르면 텍스트 값이 이동을 함.
        # self.main.statusBar().showMessage("AUTO START")       # 에러 뜸
        self.arduino = self.main.arduino
        text = self.ui.G_code_upload.toPlainText()
        lines = text.splitlines()

        if not self.progressThread.isRunning():
            self.progressThread.setText(lines)
            self.progressThread.start()  # 왜 start 였지???
        else:
            self.progressThread.reRun()
            print("다시 시작합니다.")




    def Auto_stop(self):
        # global current_file

        self.arduino = self.main.arduino
        self.arduino.send_data("p\n")
        self.main.statusBar().showMessage("S T O P")
        print("중단 합니다.")
        self.progressThread.runPause()

        # # 저장한 파일 값 current_file이라는 곳에 저장
        # current_file = fname[0]
        # print(fname[0])
        # print(current_file)




# 그 뭐냐 뭐냐 뭐냐 뭐냐  setting값을 받아오면 toPlainText() 업로드 내용 변경하기. ->
    def option_code(self):
        pass
        #option에서 받아온 값으로 toPlainText() 업데이트하기.

