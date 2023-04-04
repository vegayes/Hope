import time

from PyQt5.QtCore import QThread, pyqtSignal
from PyQt5.QtGui import QTextCharFormat, QColor, QTextCursor

import Hope_main
import Hope_UI


class ProGressBar_Thread(QThread):
    pbar = pyqtSignal(int)
    read = pyqtSignal(str)
    change = pyqtSignal(str)
    add = pyqtSignal(str)

    def __init__(self, parent: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        super().__init__(parent)
        self.line = None
        self.main = parent
        self.ui = ui
        self.running = True
        self.lines = None
        self.stop = False

    def run(self):
        self.receiveDate = self.main.arduino.getReceiveDateThread()
        for i, Gline in enumerate(self.lines):
            while not self.receiveDate.can_G:
                time.sleep(0.1)
                if self.stop:
                    break
            while not self.running:
                time.sleep(0.1)
                if self.stop:
                    break

            if i > 0:
                self.change.emit(self.line)
            self.line = Gline
            Gline = "<G>" + Gline + ";\n"
            self.main.arduino.send_data(Gline)
            self.pbar.emit((int(((i + 1) / len(self.lines)) * 100)))
            # self.pbar.setValue(int(((i + 1) / len(self.lines)) * 100))
            self.add.emit(self.line)

            self.receiveDate.can_G = False

            if self.stop:
                break

        self.change.emit(self.line)

    # 필수 호출 함수
    def setText(self, lines: list):
        self.lines = lines

    def reRun(self):
        self.running = True

    def runPause(self):
        self.running = False

    def runBreak(self):
        self.stop = True

    def reset(self):
        self.running = False
        self.stop = False
        self.receiveDate.can_G = True
        self.pbar.emit(0)
        self.read.emit("")
        time.sleep(0.3)
