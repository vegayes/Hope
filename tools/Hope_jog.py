import Hope_UI
import Hope_main


class NoFindArduino(Exception):
    pass


class jog_hope:
    def __init__(self, main: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        self.main = main
        self.ui = ui

        self.ui.jog_x_up_button.clicked.connect(self.jog_x_up())

    def jog_x_up(self):
        try:
            self.arduino = self.main.arduino
            self.arduino.send_data("x")
            self.main.statusBar().showMessage("jog_x+")
        except:
            self.main.statusBar().showMessage("don't find Arduino")
            raise NoFindArduino
