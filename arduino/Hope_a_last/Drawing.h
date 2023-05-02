#ifndef Drawing_h
#define Drawing_h

class Drawing {
public : 
  Drawing();

  void line(double dx, double dy, double dz);

  void pause(long ms);

  void feedrate(float nfr);
};

#endif
