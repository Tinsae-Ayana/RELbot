#include "MyApp.h"

MyApp::MyApp() { printf("%s: Constructing rampio\n", __FUNCTION__); }

MyApp::~MyApp() { printf("%s: Destructing rampio\n", __FUNCTION__); }

int MyApp::preProc() {
  const double encoderCountsPerRev = 1024.0;
  const double gearRatio = 15.58;

  double wheelTurnsChannel1 =
      static_cast<double>(fpgaInput.channel1) /
      encoderCountsPerRev; // encoder left, how do u know?
  double wheelTurnsChannel2 = static_cast<double>(fpgaInput.channel2) /
                              encoderCountsPerRev; // encoder right

  u[0] = (wheelTurnsChannel1 / gearRatio) * 2.0 * M_PI; // PosLeft
  u[1] = (wheelTurnsChannel2 / gearRatio) * 2.0 * M_PI; // PosRight

  u[2] = RosData.x; // SetVelLeft
  u[3] = RosData.y; // SetVelRight
  
  // send the encoder reading to ros
  XenoData.x = u[0]; // PosLeft
  XenoData.y = u[1]; // PosRight 

  return 0;
}

int MyApp::postProc() {
  FpgaOutput.pwm1 = static_cast<int16_t>((y[0] / 100.0) * 4095.0 -
                                         2047.0); // SteerLeft, 4094 or 4095
  FpgaOutput.pwm2 =
      static_cast<int16_t>((y[1] / 100.0) * 4095.0 - 2047.0); // SteerLeft

  return 0;
}
