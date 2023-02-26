import Hope_UI
import Hope_main
from display import Hope_Position


class jog_hope:
    def __init__(self, main: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        self.main = main
        self.ui = ui

        self.position = Hope_Position.position_hope(main, ui)

        self.ui.jog_x_up_button.clicked.connect(self.jog_x_up)
        self.ui.jog_x_down_button.clicked.connect(self.jog_x_down)
        self.ui.jog_y_left_button.clicked.connect(self.jog_y_up)
        self.ui.jog_y_right_button.clicked.connect(self.jog_y_down)
        self.ui.jog_z_up_button.clicked.connect(self.jog_z_up)
        self.ui.jog_z_down_button.clicked.connect(self.jog_z_down)
        self.ui.jog_a_up_button.clicked.connect(self.jog_a_up)
        self.ui.jog_a_down_button.clicked.connect(self.jog_a_down)
        self.ui.jog_zero_setting_button.clicked.connect(self.jog_zero_setting)

    # JOG 함수 // speed랑 이런건 다시 설정하기!
    def jog_x_up(self):
        self.arduino = self.main.arduino
        self.arduino.send_data('x')
        self.main.statusBar().showMessage("Jog_X +")
        self.position.set_x_position(5)  # 절대 값 말고 계산값으로 ..

    def jog_x_down(self):
        self.arduino = self.main.arduino
        self.arduino.send_data('b')
        self.main.statusBar().showMessage("Jog_X -")
        self.position.set_x_position(-5)


    def jog_y_up(self):
        self.arduino = self.main.arduino
        self.arduino.send_data('y')
        self.main.statusBar().showMessage("Jog_Y +")

    def jog_y_down(self):
        self.arduino = self.main.arduino
        self.arduino.send_data('c')
        self.main.statusBar().showMessage("Jog_Y -")

    def jog_z_up(self):
        self.arduino = self.main.arduino
        self.arduino.send_data('z')
        self.main.statusBar().showMessage("Jog_Z +")

    def jog_z_down(self):
        self.arduino = self.main.arduino
        self.arduino.send_data('d')
        self.main.statusBar().showMessage("Jog_Z -")

    def jog_a_up(self):
        self.arduino = self.main.arduino
        self.arduino.send_data('a')
        self.main.statusBar().showMessage("Jog_A +")

    def jog_a_down(self):
        self.arduino = self.main.arduino
        self.arduino.send_data('e')
        self.main.statusBar().showMessage("Jog_A -")

    # 아직 아두이노 코드로 만들지 않음. All Zrn
    def jog_azrn(self):
        self.arduino = self.main.arduino
        self.arduino.send_data('f')
        self.main.statusBar().showMessage("AZRN")

    def jog_zero_setting(self):
        self.arduino = self.main.arduino
        self.arduino.send_data('o')
        self.main.statusBar().showMessage("Zero Setting <X axis = 0 , Y axis = 0 , Z axis = 0, A axis = 0> ")
