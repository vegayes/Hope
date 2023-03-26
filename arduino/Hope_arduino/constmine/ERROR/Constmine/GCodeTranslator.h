#ifndef GCodeTranslator_h
#define GCodeTranslator_h

#include <GCodeParser.h>

class GCodeTranslator : public GCodeParser {
public:
  GCodeTranslator();

  /// X, Y, Z, A에 기본적으로 몇을 곱해서 받을 것인지 결정.
  double motion_compensation = 10;

  /// 받은 라인이 GCode 수식어가 있는지 확인 (<G>) line 에 <G>를 제거한 채로 저장
  bool isGCode();

  /// 해당 c의 GCode 값을 받아옴.
  /// F 의 기본값은 1000으로 반환. 다른것들은 기본 0
  double get(char c);
};

#endif