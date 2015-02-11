(* ::Package:: *)

(* Mathematica Package *)

(* Created by BoBtheChemist February 1, 2015 *)
(* wiringPiLink.wl,v 1 *)
(* Many parts of this package were inspired by GPIOLink by lambertc *)

BeginPackage["wiringPiLink`"]
(* Exported symbols added here with SymbolName::usage *)

wiringPiInit::usage = "wiringPiInit[] initializes the GPIO interface";
wiringPiRead::usage = "wiringPiRead[pin] gets the value of pin";
wiringPiWrite::usage = "wiringPiWrite[pin,value] sets the value of pin, value can be 0 or 1";
wiringPiMode::usage == "wiringPiMode[pin,mode] sets pin as either INPUT (0) or OUTPUT (1)";
wiringPiPWMCreate::usage = "wiringPiPWMCreate[pin,value] initializes pin as a software PWm with an initial value from 0 to 100";
wiringPiPWMUpdate::usage = "wiringPiPWMUpdate[pin,value] updates a software PWM pin with a new value (between 0 and 100)";
wiringPiReset::usage = "wiringPiReset[] unloads the library functions";
wiringPiFlicker::usage = "wiringPiFlicker[args___] creates a flickering effect using PWM"
wiringPiFun::usage = "wirignPiFun[args__] is whatever I'm working on at the moment."
wiringPiFade::usage = "fading LED"
wiringPiBounce::usage = "Visualize a differential equation"
wiringPiAnalogRead::usage = "wiringPiAnalogRead[channel] reads the analog signal on a channel (0 to 7) of the MCP3008."


(* Utility functions *)
halt[]:= StopScheduledTask/@ScheduledTasks[];

(* Messages *)
LibraryFunction::invalidpin = "An invalid pin number was passed to a LibraryFunction.  Note that current implementation uses WiringPi numbering scheme."
LibraryFunction::invalidassignment = "An attempt to write something other than a 0 or 1 to a digital pin was made."
LibraryFunction::invalidmode = "An improper mode was assigned to a pin.  Valid values are 0 for input and 1 for output."
LibraryFunction::outofrange = "An attempt to assign an invalid value to a software PWM was made.  Valid softPWM values are from 0 to 100."


Begin["`Private`"]
(* Implementation of package *)

$packageFile = $InputFileName;
$libName = "libwiringpi.so.1.0.2";

$adapterLib = FileNameJoin[{FileNameTake[$packageFile,{1,-2}],"LibraryResources",$SystemID,$libName}];
$adapterInitialized;

loadAdapter[]:=
(
	If[!ValueQ@$adapterInitialized,
        wiringPiInit = LibraryFunctionLoad[$adapterLib,"wiringpi_initialize",{Integer},Integer];
		wiringPiRead = LibraryFunctionLoad[$adapterLib,"wiringpi_pinread",{Integer},Integer];
		wiringPiWrite = LibraryFunctionLoad[$adapterLib,"wiringpi_pinwrite",{Integer, Integer}, Integer];
		wiringPiMode = LibraryFunctionLoad[$adapterLib,"wiringpi_pinmode",{Integer,Integer},Integer];
		wiringPiPWMCreate = LibraryFunctionLoad[$adapterLib,"wiringpi_softPwmCreate",{Integer,Integer},Integer];
		wiringPiPWMUpdate = LibraryFunctionLoad[$adapterLib,"wiringpi_softPwmWrite",{Integer,Integer},Integer];
    wiringPiAnalogRead = LibraryFunctionLoad[$adapterLib,"wiringpi_analogread",{Integer},Integer];

		$adapterInitialized = True;
	]
)

loadAdapter[];
wiringPiInit[1];


wiringPiReset[]:=
(
	LibraryUnload[$adapterLib];
	$adapterInitialized = False;
);

$$enums = {"Input" -> 0, "Output" -> 1};

wiringPiFlicker[pin_,brightness_,wind_]:=Module[{task},
	(* Reset the pin before setting it up as PWM*)
	wiringPiMode[pin,1];
	wiringPiPWMCreate[pin,0];
	(* Create task, note there is NO error checking here *)
	task = CreateScheduledTask[
		wiringPiPWMUpdate[pin,RandomVariate[BinomialDistribution[brightness,wind]]],0.05];
	(* Start the task *)
	StartScheduledTask[task];
	(* Return the task object so it can be stopped by user *)
	task
];
  
wiringPiFun[]:=Module[{pins,brighness,wind, task},
  pins = {4,5,6};
  brightness = {20,20,20};
  wind = {0.9,0.5,0.1};
  wiringPiMode[#,1]&/@pins;
  wiringPiPWMCreate[#,0]&/@pins;
  task = CreateScheduledTask[
    MapThread[wiringPiPWMUpdate[#1,RandomVariate[BinomialDistribution[#2,#3]]]&,{pins,brightness,wind}];,
    {0.01,900}];
  StartScheduledTask[task];
  task
];

$wpFadeTimer = 0;
wiringPiFade[pin_,freq_]:=Module[{task},
  wiringPiMode[pin,1];
  wiringPiPWMCreate[pin,0];
  $wpFadeTimer = 0;
  task = CreateScheduledTask[
    wiringPiPWMUpdate[
      pin,Round@Rescale[Sin@(freq*$wpFadeTimer++),{-1,1},{0,50}]],
    {0.01,9000}];
  StartScheduledTask@task;
  task
];

$wpBounceTimer = 0;
$bounce = First@NDSolve[{y''[t]==-9.81,y[0]==50,y'[0]==0,
  WhenEvent[y[t]==0,y'[t]->-0.95 y'[t]]},y,{t,0,100}];
  
wiringPiBounce[pin_]:=Module[{task},
  wiringPiMode[pin,1];
  wiringPiPWMCreate[pin,0];
  $wpBounceTimer = 0;
  task = CreateScheduledTask[
    wiringPiPWMUpdate[pin,
      Round@(y[Mod[0.4 $wpBounceTimer++,100]]/.$bounce)],
      {0.01,9000}];
  StartScheduledTask@task;
  task
];

End[]

EndPackage[]
