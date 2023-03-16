## 📹 **H.O.P.E** 📹

## ❓ HOPE
- **H**ybird **O**ptimum **P**rinter **E**ngineering 
- 하이브리드에 최적화한 프린터 공학의 준말
- Arudino의 AccelStepper를 활용한 4축 제어 및 Python을 이용해 UI제작 및 아두이노와 통신
## 🤔 작동하는 방법
1. [**Hope_main.py**](https://github.com/vegayes/Hope/blob/master/Hope_main.py) 에서 프로그램 실행
2. [**Hope_Arudino.ino**](https://github.com/vegayes/Hope/blob/master/arduino/Hope_arduino/vegayes/0309Test5/0309Test5.ino)  (가장 최신 아두이노 파일)을 아두이노 기기에 업로드 
3. **Jog** 또는 [**GCode 파일**](https://github.com/vegayes/Hope/blob/master/LOGO.nc)   을 이용한 제어 
## ⚠ 실행시 주의사항
- 프로그램 실행 도중 <u>**인코딩 에러**</u>가 발생할 수 있음.
- 너무 잦은 명령 ( Jog, AutoStop, AutoStart 등..) 오류 발생 원인이 될 수 있음
- Arduino와 Python의 **baudrate**값이 같은 지 확인
- Arudino의 모터 **핀 번호** 지정이 제대로 되었는지 확인

## ⏸ 프로젝트 구성   

1.  [Hope_main.py](#hope_mainpy)
2.  [Hope_UI.py](#hope_uipy)
3.  [Hope2_UI.py](#hope2_uipy)
4.  [LOGO.nc](#logonc)
5. <u>**tools**</u>
   - [Hope_Arduino.py](#hope_arduinopy)
   - [Hope_G_Code.py](#hope_g_codepy)
   - [Hope_Microstepping.py](#hope_microsteppingpy)
   - [Hope_Port.py](#hope_portpy)
   - [del_extruder_code.py](#del_extruder_codepy)
6. <u>**threads**</u>
   - [progressbar.py](#progressbarpy)
   - [ReceiveData.py](#receivedatapy)
   
- ## Hope_main.py
  - 프로그램 실행시 가장 먼저 실행되는 부분
  
#### __ init __ : 다른 클래스들을 먼저 선언하고 사용하게끔 만듦.
```python
    def __init__(self):
        from tools.Hope_Port import port_hope
        from tools.Hope_Microstepping import microstepping_hope
        from tools.Hope_jog import jog_hope
        from tools.Hope_G_Code import G_code_hope
```
1.  import 구문을 __ init __ 안쪽에다가 넣은 이유는 각각의 클래스에서 Main을 import 하기 때문에 컴파일 에러 발생 순환 import 현상을 막기 위해서 함수 내부에 import 를 하게됨 

```python
        self.portClass = port_hope(self, self.ui)
        self.microsteppingClass = microstepping_hope(self, self.ui)
        self.jogClass = jog_hope(self, self.ui)
        self.G_codeClass = G_code_hope(self, self.ui)
```

2. import 한 클래스들을 생성함.

```python
        self.ui.connect_button.clicked.connect(self.connect)
        self.ui.Quit_button.clicked.connect(self.Quit)
```

3. ui에서 버튼을 눌렀을 때 매개변수로 선언한 함수 를 사용하게 됨.

#### connect  : connect 버튼을 눌렀을 때 호출하게 됨

```python
    def connect(self):
        self.port = self.portClass.get_port()
        self.statusBar().showMessage("새로운 포트를 찾았습니다.")
        try:
            port_declared = self.port in vars()
            try:
                from tools import Hope_Arduino
                self.arduino = Hope_Arduino.arduino_hope(self, self.ui)
                self.serial = self.arduino.get_serial()

                self.ui.connect_button.setEnabled(False)
                time.sleep(1.2)
                self.statusBar().showMessage("아두이노와 연결이 되었습니다. ")
                self.ui.JOG_Group.setEnabled(True)
                self.ui.Position_Group.setEnabled(True)
                self.ui.Auto_Group.setEnabled(True)
            except:
                self.statusBar().showMessage("아두이노와 연결이 되지 않았습니다.")
                print("아두이노와 연결이 되지 않았습니다.")
                raise CannotConnectException

        except AttributeError:
            self.statusBar().showMessage("보드를 연결 시킨 후 정확하게 포트와 연결하세요.")

```
Hope_Port 클래스에서 Port 를 찾고 실행하게 됨 . <br>
만약 Port 로 부터 얻어온 값이 없다면 try-except 구문을 이용해 예외를 발생시킴 <br> <br>
`self.arduino = Hope_Arduino.arduino_hope(self, self.ui)` <br>
 Hope_Arduino 클래스에서 직접 가져온 Arduino 연결로 다른 클래스에서 Hope_Arduino에 연결 하고자 할 때 가장 많이 쓰는 변수임!



- ## Hope_UI.py

- ## Hope2_UI.py

- ## LOGO.nc

- ## Hope_Arduino.py

- ## Hope_G_Code.py
  - UI에서 < Auto > 내부 버튼들을 다루는 클래스

#### open_button : FIle_Open 버튼을 눌렀을 때 실행하는 함수
```python
 def open_button(self):
        fname = QFileDialog.getOpenFileName(self.main, 'Open file', './')
        print(type(fname), fname)

        if fname[0]:
            f = open(fname[0], 'r', encoding='UTF8')
            with f:
                data = f.read()
                data = data[:-4]  # 맨 마지막 줄 없애기 M30

                if self.progressThread.isRunning():
                    self.progressThread.runPause()
                    self.progressThread.runBreak()
                    self.progressThread.requestInterruption()
                    self.progressThread.terminate()
                    self.progressThread.wait()
                    self.progressThread.reset()
                    self.progressThread = progressbar.ProGressBar_Thread(self.main, self.ui)
                    self.progressThread.reset()

                self.ui.G_code_upload.setText(data)
                self.ui.G_code_upload.append("G01 X0 Y0 Z10 A90")  # X Y 원점 복귀 혹시 몰라서 A축이랑 Z축 그려지지 않게 올림.
                self.ui.G_code_upload.append("G01 Z0 A0")  # Z A 원점 복귀
                self.ui.G_code_upload.append("M30")  # 끝내기
```
UI에서 File Open 버튼을 눌렀을 때 실행하는 함수 <br>
QFileDialog를 활용해 파일을 선택할 수 있음. <br>
파일을 쓰기모드로 열고 with 이후 아래 코드 실행.
```python
               if self.progressThread.isRunning():
                    self.progressThread.runPause()
                    self.progressThread.runBreak()
                    self.progressThread.requestInterruption()
                    self.progressThread.terminate()
                    self.progressThread.wait()
                    self.progressThread.reset()
                    self.progressThread = progressbar.ProGressBar_Thread(self.main, self.ui)
                    self.progressThread.reset()
```
만약 FIle Open을 눌렀을때 Auto_Start를 통한 G_Code 파일을 실행중일떄, if 문을 돌게됨 <br>
그 이유는 프로그레스바 쓰레드를 따로 만들어서 따로 실행하게 되는데 <br> 
Auto_Start 함수에서 
`self.progressThread.start()`로 Running을 한 것을 알 수 있음. <br> 
따라서 if문이 충족될려면 사전에 `self.progressThread.start()`를 통해 쓰레드가 실행중이여야 함
실행 중이라면 이미 실행중이던 쓰레드를 강제 종료하는 작업들을 하게 됨 ( 충돌 방지 ) 

####  Auto_start : Ui에서 Auto_start 버튼을 눌렀을 떄 실행
```python
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
```
`self.arduino = self.main.arduino`와 같이 쓴 이유는 새로운 아두이노 클래스를 생성해서 만들면 안되기 때문( 새로운 아두이노 작업을 시행하기 때문에 오류가 날 수 있음 ), 따라서 직접 최초 선언한 main에서 아두이노를 가져와 쓰게 됨. <br> 
if-else 구문은 이미 한번 Auto_Start를 눌러서 `self.progressThread.start()`로 Running을 하는 중이 아니라면 if 문을 돌게 됨. <br> 
헷갈리면 안되는게 Auto_Stop 함수를 통해서 동작을 멈췄다고 progressThread 클래스가 Running을 멈춘게 아님. 그 이유는 progressbar.py 에서 서술. <br>
어쨋든 Running 상태라면,  progressThread 의 reRun()을 통해 다시 진행중이던 작업을 시행 

#### Auto_stop : 실행 중이던 쓰레드를 작업을 지연 시킴
```python
    def Auto_stop(self):
        self.arduino = self.main.arduino
        self.arduino.send_data("p\n")
        self.main.statusBar().showMessage("S T O P")
        print("중단 합니다.")
        self.progressThread.runPause()
```
아두이노에 'p'라는 값을 보냄으로 써 모터의 동작을 멈추게 됨. <br>
`self.progressThread.runPause()` 를 통하여 진행중이던 쓰레드를 지연시킴 Running을 멈춘상태는 아님.

- ## Hope_Microstepping.py

- ## Hope_Port.py

- ## Hope_jog.py

- ## del_extruder_code.py

- ## progressbar.py
  - 프로그레스바를 QThread를 통해 구현한 함수
```python
class ProGressBar_Thread(QThread):
    pbar = pyqtSignal(int)
    read = pyqtSignal(str)
```
클래스 선언에서 QThread를 상속받으며 pyqtSignal을 쓴것이 보이는데, 각각 pbar은 프로그레스바의 setValue( 값 ) 에 해당하는 부분을 의미, read 는 G_Code_read 값의 setText ( 값 ) 을 의미하게 된다. <br><br>

#### run : QThread의 start() 함수를 호출 했을 떄 실행되는 함수
```python
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
            self.pbar.emit((int(((i + 1) / len(self.lines)) * 100)))
            # self.pbar.setValue(int(((i + 1) / len(self.lines)) * 100))
            self.ui.G_code_read.append(line)
            if self.stop:
                break
```
굉장히 코드가 지저분 한데, 우선 ` self.receiveDate = self.main.arduino.getReceiveDateThread() ` 를 통해서 아두이노의 G 코드와의 통신자체를 받아오는 함수를 의미하는데, 아두이노 코드에 보면 G_count 라는 변수가 있는데 G코드를 통해서 통신했을 때 성공적으로 움직였다면, G_count를 +1 하게 된다. 따라서 그 G_count값을 받아오는 함수를 쓰기 위해 이 클래스를 지정한다. <br><br>

`for i, Gline in enumerate(self.lines):` 과 같이 쓴 이유는, G Code를 한줄씩 list 로 받게 되는데, 
현재 줄이 몇번 쨰 줄인지를 i로 받고, 한줄 씩 나눈 것을 Gline으로 받게 된다. <br> <br>

```python
while i != self.receiveDate.get_progress_count():
                time.sleep(0.3)
                if self.stop:
                    break
            while not self.running:
                time.sleep(0.1)
                if self.stop:
                    break
```
위 코드에서는 현재 G Code를 전송한 갯수 ( i ) 가 처음에 선언한 ReceiveData 클래스에서 G_count 값을 받아오게 된다. G_count를 받아왔을 때 값이 다르다면, 그 받아온 값이 같을 때 까지 시간을 지연시켜준다. <br>
그 다음으로 `while not self.running:` 은 Auto_Stop이나 다른 요소에 의해서 잠깐 중지 되었을 때, 시간을 지연시켜주게 된다. <br>
또한 각각 while 절 안에` if self.stop: break` 은 FIle Open 과 같은 새로운 쓰레드를 호출 할때 강제 종료역할 을 하게 된다. <br>

```python
            line = Gline
            Gline = "<G>" + Gline + ";\n"
            self.main.arduino.send_data(Gline)
            self.pbar.emit((int(((i + 1) / len(self.lines)) * 100)))
            # self.pbar.setValue(int(((i + 1) / len(self.lines)) * 100))
            self.ui.G_code_read.append(line)
            if self.stop:
                break
```
그 이후 코드는 받아온 GCode 한 줄을 아두이노에 보낼때 일정 형식을 붙여서 보내게 되는데 
그 일정 형식이란 `<G> G01 X10 Y10\n` 과 같이 보내지게 된다. 아두이노에서는 이를 
`G01X10Y10`으로 읽게끔 만든다. 이는 아두이노에 따라 보내는 값이 변할 수 있다. <br><br>
`self.pbar.emit((int(((i + 1) / len(self.lines)) * 100)))` 이 코드는 pyqtSignal 을 통해서 프로그레스 바의 값을 조정하게  된다. {(현재 GCode 읽은 갯수 + 1 ) / 전제 GCode의 갯수 )} * 100 를 통해 진행률 을 나타내고, pyqtSignal을 사용하는 이유는 UI 와 관련된 동작은 다른 쓰레드에서 직접 건들게 되면 충돌 현상이 일어나는 경우가 있다. 이를 해결하기 위해 UI 관련 동작은 메인 쓰레드에서 처리시키기 위해 emit을 통해서 실행한다. emit에 해당하는 함수는 Hope_G_Code.py 에서 
`self.progressThread.pbar.connect(self.pbar.setValue)` 와 같이 connect 를 통해 연결 시켜주면 된다. <br><br>

위 과정을 모두 거치고 나면 값이 성공적으로 아두이노에 보내졌으므로 보내진 값을 나타내기 위해 G_code_read 에다가  해결한 값을 직접 작성하게 된다 . ( 근데 이거 쓰면서 생각이 든건 이것도 충분히 오류를 만들 수 있을 것 같은데, pyqtSignal을 통해 작성하는 것이 좋아보인다.)  

- ## ReceiveData.py

- ### <u>display</u>

- ### <u>arduino/Hope_arduino</u>