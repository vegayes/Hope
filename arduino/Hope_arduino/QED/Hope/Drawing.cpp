#include "Drawing.h"
#include "config.h"
#include <Arduino.h>

char  serialBuffer[MAX_BUF];  // where we store the message until we get a newline
int   sofar;            // how much is in the serialBuffer
float px, py, pz;      // location

// speeds
float fr =     0;  // human version
long  step_delay;  // machine version

// settings
char mode_abs = 1; // absolute mode?

Drawing::Drawing() {
}

void Drawing::feedrate(float nfr) {
  if(fr==nfr) return;  // same as last time?  quit now.

  if(nfr>MAX_FEEDRATE || nfr<MIN_FEEDRATE) {  // don't allow crazy feed rates
    Serial.print(F("New feedrate must be greater than "));
    Serial.print(MIN_FEEDRATE);
    Serial.print(F("steps/s and less than "));
    Serial.print(MAX_FEEDRATE);
    Serial.println(F("steps/s."));
    return;
  }
  step_delay = 1000000.0/nfr;
  fr = nfr;
}

void Drawing::line(double newx, double newy, double newz) {
  long i;
  long over_x = 0;
  long over_y = 0;
  long over_z = 0;

  long dx = newx - px;
  long dy = newy - py;
  long dz = newz - pz;

  int dirx = dx > 0 ? 1 : -1;
#ifdef INVERT_Y
  int diry = dy > 0 ? -1 : 1; // because the motors are mounted in opposite directions
#else
  int diry = dy > 0 ? 1 : -1;
#endif
  int dirz = dz > 0 ? 1 : -1;


  Serial.println("------- lines의 첫 값 --------");
  Serial.println(px);
  Serial.println(py);
  Serial.println(pz);
  Serial.println(dx);
  Serial.println(dy);
  Serial.println(dz);
  Serial.println(dirx);
  Serial.println(diry);
  Serial.println(dirz);

  dx = abs(dx); //400
  dy = abs(dy); //100
  dz = abs(dz); //0

  
  if (dx >= dy && dx >= dz) {
    over_x = dx / 2; //200
    float step_distance_y = dy / (float) dx; // 1/4
    float step_distance_z = dz / (float) dx; // 0
    for (i = 0; i < dx; ++i) { // 400번 실행
      m1step(dirx);  // x 1만큼 움직임
      over_x += dy; // 200 + 100
      if (over_x >= dx) { // 300 >= 400
        over_x -= dx; // 300 - 400
        m2step(diry); // y 1만큼 움직임
      }
      over_z += dz;
      if (over_z >= dx) {
        over_z -= dx;
        m3step(dirz);
      }
      pause(step_delay);
    }
  } else if (dy >= dx && dy >= dz) {
    over_y = dy / 2;
    float step_distance_x = dx / (float) dy;
    float step_distance_z = dz / (float) dy;
    for (i = 0; i < dy; ++i) {
      m2step(diry);
      over_y += dx;
      if (over_y >= dy) {
        over_y -= dy;
        m1step(dirx);
      }
      over_z += dz;
      if (over_z >= dy) {
        over_z -= dy;
        m3step(dirz);
      }
      pause(step_delay);
    }
  } else {
    over_z = dz / 2;
    float step_distance_x = dx / (float) dz;
    float step_distance_y = dy / (float) dz;
    for (i = 0; i < dz; ++i) {
      m3step(dirz);
      over_z += dx;
      if (over_z >= dz) {
        over_z -= dz;
        m1step(dirx);
      }
      over_y += dy;
      if (over_y >= dz) {
        over_y -= dz;
        m2step(diry);
      }
      pause(step_delay);
    }
  }
  px = newx;
  py = newy;
  pz = newz;
}

void Drawing::pause(long ms) {
  delay(ms / 1000);
  delayMicroseconds(ms % 1000); // delayMicroseconds doesn't work for values > ~16k.
}
