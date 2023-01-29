import Hope_UI
import Hope_main


class microstepping_hope:
    """
    스텝 모터의 값을 설정
    """

    def __init__(self, main: Hope_main.UI_Window, ui: Hope_UI.Ui_HOPE):
        self.main = main
        self.ui = ui
        # 초기 설정을 등록
        self.populate_microstepping()

    def populate_microstepping(self):
        self.microstepping_values = ['1', '2', '4', '8', '16', '32']
        self.ui.Microstepping_combo.addItems(self.microstepping_values)
        self.microstepping = 1

    def set_microstepping(self):
        pass

    def get_microstepping(self):
        """
        microstepping의 현재 값을 가져옴
        :return: String
                (현재 값)
        """
        return self.ui.Microstepping_combo.currentText()
