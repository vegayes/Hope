#ifndef GCodeTranslator_h
#define GCodeTranslator_h

#include <GCodeParser.h>
#include <WString.h>

class GCodeTranslator : public GCodeParser {
public:
  GCodeTranslator();

  /// X, Y, Z, A에 기본적으로 몇을 곱해서 받을 것인지 결정.
  double motion_calibration = 200;

  /// 받은 라인이 GCode 수식어가 있는지 확인 (<G>) line 에 <G>를 제거한 채로 저장
  bool isGCode();

  /// 해당 c의 GCode 값을 받아옴 "리턴 형식은 String" <- NULL을 반환하기 위함
  /// F 의 기본값은 1000으로 반환. 다른것들은 None 반환
  String get(char c);
};

#endif
