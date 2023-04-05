import re
import time

from PyQt5 import QtWidgets, Qt
from PyQt5.QtGui import QColor, QTextCharFormat, QTextCursor, QTextBlockFormat
from PyQt5.QtWidgets import *

import Hope2_UI
import Hope_UI
import Hope_main
from display import Hope_ProgressBar
from threads import progressbar


class G_code_hope:

    def __init__(self, main: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        self.main = main
        self.ui = ui
        self.currentFile = None
        self.previous_color = None

        self.pbar = Hope_ProgressBar.ProgressBar_Hope(main, ui)
        self.progressThread = progressbar.ProGressBar_Thread(self.main, self.ui)
        self.progressThread.pbar.connect(self.pbar.setValue)
        self.progressThread.read.connect(self.ui.G_code_read.setText)
        self.progressThread.change.connect(self.change_text)
        self.progressThread.add.connect(self.add_text)

        self.ui.File_open_button.clicked.connect(self.open_button)
        self.ui.Auto_start_button.clicked.connect(self.Auto_start)
        self.ui.Auto_stop_button.clicked.connect(self.Auto_stop)
        self.ui.Option_form_button.clicked.connect(self.openHope2)

    def openHope2(self):
        self.window2 = QtWidgets.QDialog()
        self.ui2 = Hope2_UI.Ui_HOPE2()
        self.ui2.setupUi(self.window2)
        self.window2.show()
        self.ui2.Option_Setting_Button.clicked.connect(self.option_setting_button)

    def change_text(self, line):
        # 텍스트를 추가하고 서식을 변경합니다.
        self.ui.G_code_read.undo()
        self.ui.G_code_read.moveCursor(QTextCursor.End)
        self.ui.G_code_read.setTextColor(QColor("green"))
        self.ui.G_code_read.append(line)

    def add_text(self, line):
        self.ui.G_code_read.setTextColor(QColor("red"))
        self.ui.G_code_read.append(line)


    def open_button(self):
        # 파일을 열고나서 처음 값이 같다면 return
        fname = QFileDialog.getOpenFileName(self.main, 'Open file', './')
        print(type(fname), fname)
        self.arduino = self.main.arduino
        self.arduino.send_data("Stop\n")

        if fname[0]:
            f = open(fname[0], 'r', encoding='UTF8')
            with f:
                open_file = f.read()
                # G코드에서 변경해야 할 값.
                # 1) G0, 1, 4 제외하고 G21, 90, 92없애야 함. (G4는 딜레이 코드, G4 P1000은 1000ms동안 딜레이)0 *
                # 4) M300을 제외한 다른 M코드(M17, 18) 없애야함 0
                # 2) M300 S30 -> Z값 원점기준 0으로 바꿔야함 0
                # 3) M300 S50 -> Z값 원점기준 10?으로 바꿔야함 0  ** 기준값 바뀔 수 있음.
                # 5) 엔터 후 공백 공간 없애야함 0 0   ** 기준값 바뀔 수 있음.
                # 7) 원점으로 복귀하는 코드 넣기 0 ** 기준값 바뀔 수 있음.
                # 6) 맨 밑에 M30코드 입력 0 =>18로 바뀔 수 있음
                # 8) 괄호 사이의 존재하는 값 다 삭제 0
                # 9) (end of print job)  뒤 값 제거 0

                remove_code = "(end of print job)"
                if remove_code in open_file:
                    index = open_file.find(remove_code)
                    file = open_file[:index + len("(end of print job)")]
                else:
                    file = open_file

                    # index = open_file.find("(end of print job)")  #먼저 end of print job밑의 값 다 제외.
                    # file = open_file[:index + len("(end of print job)")]
                print(file)
                print('or')

                # lines = [x.split('(')[0].strip() if '(' in x else x for x in file]
                new_lines = [line for line in file.splitlines() if line.strip()]  # 코드 빈 줄 제거

                print(new_lines)

                m_remove_lines = []  # M코드에서 300만 남겨두고 다른 값 들 지우기 *
                for line in new_lines:
                    if line.startswith("M"):
                        if line[1:].startswith("300"):
                            m_remove_lines.append(line)
                        else:
                            continue
                    else:
                        m_remove_lines.append(line)

                print(m_remove_lines)

                # G코드 0,1,4 빼고 제외하기 *
                lines = [item for item in m_remove_lines if not item.startswith('G') or item[1] in ['0', '1', '4']]

                lines = [re.sub(r'\(.*?\)', '', s) for s in lines]  # 괄호 값 없애기
                lines = [re.sub(r'\\.*?\)', '', s) for s in lines]  # \ )사이 없애기 첫 줄

                lines = [x for x in lines if x != '']  # 값 변경으로 발생한 빈 줄 제거

                data = '\n'.join(lines)

                data = data.replace('M300 S30', 'G01 Z0 F1000')  # feed 값 변경하길. **
                data = data.replace('M300 S50', 'G01 Z10 F1000')  # **

                # 문자열의 개수를 출력 (행수 세기)
                line = data.splitlines()
                num_lines = len(line)
                print(num_lines)

                self.text = data

                self.ui.G_code_upload.setText(data)  # 변형 코드들은 위에 써줘야 함.
                self.ui.G_code_read.setText("")

                # 원점 복귀 추가 코드들
                self.ui.G_code_upload.append("-" * 25)  # 경계 값 확인하려고 넣어둠.
                self.ui.G_code_upload.append("G04 P1500")  # 1.5초 기다림.
                self.ui.G_code_upload.append("G01 X0 Y0 Z50 A90")  # X Y 원점 복귀 혹시 몰라서 A축이랑 Z축 그려지지 않게 올림. **
                self.ui.G_code_upload.append("G04 P1500")  # 1.5초 기다림.
                self.ui.G_code_upload.append("G01 Z0 A0")  # Z A 원점 복귀
                self.ui.G_code_upload.append("M30")  # 끝내기 *=>18로 바뀔 수 있음

                if self.progressThread.isRunning():
                    self.progressThread.runPause()
                    self.progressThread.runBreak()
                    # self.progressThread.reset()
                    self.progressThread.requestInterruption()
                    self.progressThread.terminate()
                    self.progressThread.wait()
                    self.progressThread.reset()
                    self.progressThread = progressbar.ProGressBar_Thread(self.main, self.ui)
                    self.progressThread.pbar.connect(self.pbar.setValue)
                    self.progressThread.read.connect(self.ui.G_code_read.setText)

        # if fname[0]:
        #     f = open(fname[0], 'r', encoding='UTF8')
        #     with f:
        #         data = f.read()
        #         data = data[:-4]  # 맨 마지막 줄 없애기 M30
        #         # data = data.replace('M30', '') # M30지우기 but
        #
        #         # Todo 여기중 하나가 작동되어 새로운 시작 가능 이건 나중에 추후 차근히 해보자고
        #         if self.progressThread.isRunning():
        #             self.progressThread.runPause()
        #             self.progressThread.runBreak()
        #             # self.progressThread.reset()
        #             self.progressThread.requestInterruption()
        #             self.progressThread.terminate()
        #             self.progressThread.wait()
        #             self.progressThread.reset()
        #             self.progressThread = progressbar.ProGressBar_Thread(self.main, self.ui)
        #             self.progressThread.reset()
        #
        #         self.ui.G_code_upload.setText(data)
        #         self.ui.G_code_upload.append("G01 X0 Y0 Z10 A90")  # X Y 원점 복귀 혹시 몰라서 A축이랑 Z축 그려지지 않게 올림.
        #         self.ui.G_code_upload.append("G01 Z0 A0")  # Z A 원점 복귀
        #         self.ui.G_code_upload.append("M30")  # 끝내기

    def Auto_start(self):  # Auto Start 버튼을 누르면 텍스트 값이 이동을 함.
        self.main.statusBar().showMessage("AUTO START")
        self.arduino = self.main.arduino
        self.arduino.send_data("Start\n")
        text = self.ui.G_code_upload.toPlainText()
        lines = text.splitlines()

        if not self.progressThread.isRunning():
            self.progressThread.setText(lines)
            self.progressThread.start()
        else:
            self.progressThread.reRun()
            print("다시 시작합니다.")

    def Auto_stop(self):
        self.arduino = self.main.arduino
        self.progressThread.runPause()
        self.arduino.send_data("Stop\n")
        self.main.statusBar().showMessage("S T O P")
        print("중단 합니다.")
        self.progressThread.runPause()

    def option_setting_button(self):
        global data
        # x_text = self.ui2.Position_X_Edit.toPlainText()
        # y_text = self.ui2.Position_Y_Edit.toPlainText()
        # height = self.ui2.Height_Edit.toPlainText()
        # width = self.ui2.Width_Edit.toPlainText()

        # text_Edit_list = [x_text, y_text, height, width]

        if self.ui2.type1_radio_button.isChecked():
            print("plan")
            #  type1 일 때는 비활성화 하던가 이거 어려우면 message로 오류 났다가 표시하기.
            self.window2.close()

        elif self.ui2.type2_radio_button.isChecked():
            print("45degree")
            # angle = 45

            x_text = self.ui2.Position_X_Edit.toPlainText()
            print("X값 : " + x_text)

            y_text = self.ui2.Position_Y_Edit.toPlainText()
            print("Y값 :" + y_text)

            height = self.ui2.Height_Edit.toPlainText()
            print("height값 :" + height)

            width = self.ui2.Width_Edit.toPlainText()
            print("width값 :" + width)

            # 수식 적용.

            print("data값 " + self.text)

            self.window2.close()

        elif self.ui2.type3_radio_button.isChecked():
            print("90degree")
            # 90
            x_text = self.ui2.Position_X_Edit.toPlainText()
            print("X값 : " + x_text)

            y_text = self.ui2.Position_Y_Edit.toPlainText()
            print("Y값 :" + y_text)

            height = self.ui2.Height_Edit.toPlainText()
            print("height값 :" + height)

            width = self.ui2.Width_Edit.toPlainText()
            print("width값 :" + width)

            # 수식 이용.

            #1) 벽면 위치 값의 -30mm 저장 이 부분부터 A 45도 움직임. // 예로들어 해당 y로 가는 위치 값 전에 A는 45도를 유지를 그 줄에 추가해줌!( 같이 움직어야 그림이 그려짐. )
            a_move_start = y_text - 30
            print(a_move_start)

            #2) 벽면의 높이 값의 - 15mm저장 이 부분부까지  A가 점점 90도로 움직임
            a_move_end = height - 15
            print(a_move_end)

            #3) height와 width 사이에 있는 값은 다 변경하기. 예로들어서 해당 위치가 x: 30 (이거 필요 없을 듯) / y: 70 / width : 50 / height : 70이면
            # a_move_height =

            # 벽면에만 바로 그림 그리는 경우
            # 1)




            self.window2.close()
