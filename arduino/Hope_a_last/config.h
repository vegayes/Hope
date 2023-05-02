#ifndef CONFIG_H
#define CONFIG_H

#define CNCV3 (5) 

#define CONTROLLER CNCV3


#define VERSION        (1)  
#define BAUD           (115200) 
#define MAX_BUF        (64)  
#define STEPS_PER_TURN (400)  
#define MIN_STEP_DELAY (50.0)
#define MAX_FEEDRATE   (1000000.0/MIN_STEP_DELAY)
#define MIN_FEEDRATE   (0.01)
//#define INVERT_Y     (1) // invert Y axis direction

// for arc directions
#define ARC_CW          (1)
#define ARC_CCW         (-1)
// Arcs are split into many line segments.  How long are the segments?
#define MM_PER_SEGMENT  (10)


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------
extern void m1step(int dir);
extern void m2step(int dir);
extern void m3step(int dir);
extern void disable();
extern void setup_controller();


/**
* This file is part of GcodeCNCDemo.
*
* GcodeCNCDemo is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* GcodeCNCDemo is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Foobar. If not, see <http://www.gnu.org/licenses/>.
*/
#endif
