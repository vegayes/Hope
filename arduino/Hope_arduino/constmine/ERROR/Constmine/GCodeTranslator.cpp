#include "GCodeTranslator.h"

#include <GCodeParser.h>
#include <WString.h>

GCodeTranslator::GCodeTranslator() : GCodeParser(){
}

bool GCodeTranslator::isGCode() {
  String temp(line);
  int index = temp.indexOf("<G>");
  if (index >= 0) {
    temp.replace("<G>", "");
    strcpy(line, temp.c_str());
    return true;
  }
  return false;
}

double GCodeTranslator::get(char c) {
  if (HasWord(c)) {
        double value = GetWordValue(c);
        if(c == 'X' || c == 'Y' || c == 'Z' || c == 'A') {
          return value * motion_compensation;
        } else {
          return value;
        }
  } else {
    if(c == 'F') return 1000;
    else return 0;
  }
}


