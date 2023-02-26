import Hope_UI
import Hope_main


class position_hope:

    def __init__(self, main: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        self.main = main
        self.ui = ui

    def set_x_position(self, data: int):  # data값은 이제 계산한 값을 가지고 있어야 함.
        self.ui.position_x.setNum(data)

    def set_y_position(self, data: int):
        self.ui.position_y.setNum(data)

    def set_z_position(self, data: int):
        self.ui.position_z.setNum(data)

    def set_a_position(self, data: int):
        self.ui.position_a.setNum(data)
