import Hope_UI
import Hope_main


class position_hope:

    def __init__(self, main: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        self.main = main
        self.ui = ui

    def set_x_position(self, data):
        self.ui.position_x.display(data)
