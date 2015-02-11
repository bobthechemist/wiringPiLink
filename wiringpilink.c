/*
 *
 *
*/

/* Include required headers */
#include "WolframLibrary.h"
#include "wiringPi.h"
#include "softPwm.h"
#include "mcp3004.h"

#define BASE 100
#define SPI_CHAN 0

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
/* BASE and SPI Channel are fixed, but presumably can be    */
/*  replaced with arguments at some point in the future     */

int wiringpi_initialize( WolframLibraryData libData,mint Argc, MArgument *Args, MArgument Res){
  wiringPiSetup();
  mcp3004Setup (BASE, SPI_CHAN);
  return LIBRARY_NO_ERROR;
}

/* Checks if valid pin number has been passed.  Currently only checks  */
/*  if the pin is a legitimate number within the wiringPi designation. */
int validpinQ(mint pin){
  if(pin <= 20 && pin >=0){
    return 1;
  } else {
    return 0;
  }
}

/* Reads the state of a pin */
int wiringpi_pinread( WolframLibraryData libData,mint Argc, MArgument *Args, MArgument Res){
  mint pin;
  // There should only be one argument, the pin number to read.
  pin = MArgument_getInteger(Args[0]);
  // Check if pin is a valid pin number, if so, read the pin. Returns
  // -1 if the pin number is invalid.
  if(validpinQ(pin)){
    //out = digitalRead(pin);
    MArgument_setInteger(Res,digitalRead(pin));
  } else{
    MArgument_setInteger(Res,-1);
    libData->Message("invalidpin");
  }
  return LIBRARY_NO_ERROR;
}
  
/* Writes to a pin */
int wiringpi_pinwrite( WolframLibraryData libData,mint Argc, MArgument *Args, MArgument Res){
  mint pin;
  mint value;
  pin = MArgument_getInteger(Args[0]);
  value = MArgument_getInteger(Args[1]);
  // Will check that the pin is valid and that the value to be assigned to the pin is legitimate (0 or 1).
  if(validpinQ(pin)){
    if(value==0 || value==1) {
      digitalWrite(pin,value);
      MArgument_setInteger(Res,1);
    } else {
      MArgument_setInteger(Res,-1);
      libData->Message("invalidassignment");
    }
  } else {
    MArgument_setInteger(Res,-1);
    libData->Message("invalidpin");
  }

  return LIBRARY_NO_ERROR;
}

/* Sets the pin mode */
int wiringpi_pinmode( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res){
  mint pin;
  mint mode;
  // Check that the pin assignment is valid and that mode is either 0 (input) or 1 (output)
  pin = MArgument_getInteger(Args[0]);
  mode = MArgument_getInteger(Args[1]);
  if(validpinQ(pin)){
    if(mode==0||mode==1){
      pinMode(pin,mode);
      MArgument_setInteger(Res,1);
    } else {
      libData->Message("invalidmode");
      MArgument_setInteger(Res,-1);
    }
  } else {
    MArgument_setInteger(Res,-1);
    libData->Message("invalidpin");
  }
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

/* Analog reading from MCP 3008 */
int wiringpi_analogread( WolframLibraryData libData,mint Argc, MArgument *Args, MArgument Res){
  mint channel;
  mint out;
  channel = MArgument_getInteger(Args[0]);
  out = analogRead(BASE + channel);
  MArgument_setInteger(Res,out);
  return LIBRARY_NO_ERROR;
}
