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

        self.pbar = Hope_ProgressBar.ProgressBar_Hope(parent, ui)

    def run(self):
        self.receiveDate = self.main.arduino.getReceiveDateThread()
        for i, Gline in enumerate(self.lines):
            while i != self.receiveDate.get_progress_count():
                time.sleep(0.1)
            while not self.running:
                pass
            Gline = "<G>" + Gline + ";\n"
            self.main.arduino.send_data(Gline)
            self.pbar.setValue(int((i / len(self.lines)) * 100))
            print(Gline)

    # 필수 호출 함수
    def setText(self, lines: list):
        self.lines = lines
        self.count = 0

    def reRun(self):
        self.running = True

    def runPause(self):
        self.running = False

