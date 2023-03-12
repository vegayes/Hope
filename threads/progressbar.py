import time

from PyQt5.QtCore import QThread, pyqtSignal
import Hope_main
import Hope_UI
from display import Hope_ProgressBar


class ProGressBar_Thread(QThread):
    def __init__(self, parent: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        super().__init__(parent)
        self.main = parent
        self.ui = ui
        self.running = True
        self.lines = None
        self.stop = False

        self.pbar = Hope_ProgressBar.ProgressBar_Hope(parent, ui)

    def run(self):
        self.receiveDate = self.main.arduino.getReceiveDateThread()
        for i, Gline in enumerate(self.lines):
            while i != self.receiveDate.get_progress_count():
                time.sleep(0.3)
                if self.stop:
                    break
            while not self.running:
                time.sleep(0.1)
                if self.stop:
                    break
            line = Gline
            Gline = "<G>" + Gline + ";\n"
            self.main.arduino.send_data(Gline)
            self.pbar.setValue(int(((i + 1) / len(self.lines)) * 100))
            self.ui.G_code_read.append(line)
            if self.stop:
                break


    # 필수 호출 함수
    def setText(self, lines: list):
        self.lines = lines

    def reRun(self):
        self.running = True

    def runPause(self):
        self.running = False

    def reset(self):
        self.stop = True
        self.running = False
        self.lines = None
        self.pbar.setValue(0)
        self.ui.G_code_read.setText("")
        time.sleep(0.3)


