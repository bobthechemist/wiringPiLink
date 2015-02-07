/*
 *
 *
*/

/* Include required headers */
#include "WolframLibrary.h"
#include "wiringPi.h"
#include "softPwm.h"

/* Not sure if the following are necessary or how to utilize them */
/* Initialize Library */
int WolframLibrary_Initialize( WolframLibraryData libData) {
  return LIBRARY_NO_ERROR;
}
/* Uninitialize Library */
void WolframLibrary_uninitialize( WolframLibraryData libData) {
  return;
}

/* Initialization routine, current doesn't do anything with */
/*  arguments but can eventually be used to determine which */
/*  type of numbering scheme is to be used.                 */

int wiringpi_initialize( WolframLibraryData libData,mint Argc, MArgument *Args, MArgument Res){
  wiringPiSetup();
  return LIBRARY_NO_ERROR;
}

/* Reads the state of a pin */
int wiringpi_pinread( WolframLibraryData libData,mint Argc, MArgument *Args, MArgument Res){
  mint pin;
  mint out;
  pin = MArgument_getInteger(Args[0]);
  out = digitalRead(pin);
  MArgument_setInteger(Res,out);
  return LIBRARY_NO_ERROR;
}
  
/* Writes to a pin */
int wiringpi_pinwrite( WolframLibraryData libData,mint Argc, MArgument *Args, MArgument Res){
  mint pin;
  mint value;
  pin = MArgument_getInteger(Args[0]);
  value = MArgument_getInteger(Args[1]);
  digitalWrite(pin,value);
  MArgument_setInteger(Res,1);
  return LIBRARY_NO_ERROR;
}

/* Sets the pin mode */
int wiringpi_pinmode( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res){
  mint pin;
  mint mode;
  pin = MArgument_getInteger(Args[0]);
  mode = MArgument_getInteger(Args[1]);
  pinMode(pin,mode);
  /* pinMode does not return a value so always return 1 for now */
  MArgument_setInteger(Res,1);
  return LIBRARY_NO_ERROR;
}

/* Software PWM has two functions, which are wrapped below */
int wiringpi_softPwmCreate( WolframLibraryData libData,mint Argc, MArgument *Args, MArgument Res){
  mint pin;
  mint initval;
  mint out;
  pin = MArgument_getInteger(Args[0]);
  initval = MArgument_getInteger(Args[1]);
  out = softPwmCreate(pin,initval,100);
  if(out!=0) return LIBRARY_FUNCTION_ERROR;
  return LIBRARY_NO_ERROR;
}

int wiringpi_softPwmWrite(WolframLibraryData libData,mint Argc, MArgument *Args, MArgument Res){
  mint pin;
  mint val;
  pin = MArgument_getInteger(Args[0]);
  val = MArgument_getInteger(Args[1]);
  softPwmWrite(pin,val);
  return LIBRARY_NO_ERROR;
}
