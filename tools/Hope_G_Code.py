import re
import time

from PyQt5.QtWidgets import *

import Hope_UI
import Hope_main
from display import Hope_ProgressBar
from threads import progressbar


class G_code_hope:

    def __init__(self, main: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        self.main = main
        self.ui = ui
        self.currentFile = None

        self.pbar = Hope_ProgressBar.ProgressBar_Hope(main, ui)
        self.progressThread = progressbar.ProGressBar_Thread(self.main, self.ui)
        self.progressThread.pbar.connect(self.pbar.setValue)
        self.progressThread.read.connect(self.ui.G_code_read.setText)

        self.ui.File_open_button.clicked.connect(self.open_button)
        self.ui.Auto_start_button.clicked.connect(self.Auto_start)
        self.ui.Auto_stop_button.clicked.connect(self.Auto_stop)
        self.ui.Option_form_button.clicked.connect(self.ui.openHope2)

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

                self.ui.G_code_upload.setText(data)
                self.ui.G_code_read.setText("")

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
