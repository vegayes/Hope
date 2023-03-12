import Hope_UI
import Hope_main
from PyQt5.Qt import *


class ProgressBar_Hope:
    def __init__(self, main: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        self.ui = ui
        self.main = main
        self.pbar = ui.G_code_progressBar
        self.count = 0

        self.pbar.setAlignment(Qt.AlignCenter)
        self.pbar.setRange(0, 100)

    def setValue(self, a: int):
        self.pbar.setValue(a)

    def getValue(self):
        return self.pbar.value()
