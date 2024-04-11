#include "MyApp.h"

MyApp::MyApp() { printf("%s: Constructing rampio\n", __FUNCTION__); }

MyApp::~MyApp() { printf("%s: Destructing rampio\n", __FUNCTION__); }

int MyApp::preProc() {
  const double encoderCountsPerRev = 1024.0;
  const double gearRatio = 15.58;

  double wheelTurnsChannel1 =
      static_cast<double>(fpgaInput.channel1) / encoderCountsPerRev;
  double wheelTurnsChannel2 =
      static_cast<double>(fpgaInput.channel2) / encoderCountsPerRev;

  u[0] = (wheelTurnsChannel1 / gearRatio) * 2.0 * M_PI; // PosLeft
  u[1] = (wheelTurnsChannel2 / gearRatio) * 2.0 * M_PI; // PosRight

  u[2] = rosData.x; // SetVelLeft
  u[3] = rosData.y; // SetVelLeft

  return 0;
}

int MyApp::postProc() {
  FpgaOutput.pwm1 =
      static_cast<int16_t>((y[0] / 100.0) * 4095.0 - 2047.0); // SteerLeft
  FpgaOutput.pwm2 =
      static_cast<int16_t>((y[1] / 100.0) * 4095.0 - 2047.0); // SteerRight

  return 0;
}