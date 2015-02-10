# wiringPiLink

This library is an attempt to implement the wiringPi functionality 
(see www.wiringpi.com) in Mathematica.  It is also an example of how to
create a shared library in c that results in functions one can load
with LoadLibraryFunction.

This code is in the very early stages of development, which is good in 
that there is not much to follow (and therefore should be easy to follow)
but is not terribly feature rich.  For example, one missing feature is
any attempt at error checking.  Simply don't make any mistakes and you 
won't have a problem.

Originally, this project was designed as a tool to teach me how to write
a device driver that can be accessed through Mathematica's Device* 
functions.  It turns out that the added layer causes some problems with
response time so I have put that project on the back burner.  

In theory, all you need to do is compile the c code (with compile.sh) and
then put it into the appropriate directory (./wiringPiLink/LibraryResoruces/Linux-ARM/) and then you can call it (from ./) using <<wiringPiLink`

## Library versions ##

1.0.2 Added ability to read analog signals through the SPI interface and a MCP3008
1.0.1 Basic GPIO pin reading and writing along with software PWM

