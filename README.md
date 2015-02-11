# wiringPiLink #

## Introduction ##

This library is an attempt to implement the wiringPi functionality 
(see www.wiringpi.com) in *Mathematica*.  It is also an example of how to
create a shared library in c that results in functions one can load
with `LoadLibraryFunction`.

This code is in the very early stages of development, which is good in 
that there is not much to follow (and therefore should be easy to follow)
but is not terribly feature rich. ~~For example, one missing feature is
any attempt at error checking.~~  Simply don't make any mistakes and you 
won't have a problem.

Originally, this project was designed as a tool to teach me how to write
a device driver that can be accessed through Mathematica's Device* 
functions.  It turns out that the added layer causes some problems with
response time so I have put that project on the back burner.  

In theory, all you need to do is compile the c code (with compile.sh) and
then put it into the appropriate directory (./wiringPiLink/LibraryResoruces/Linux-ARM/) and then you can call it (from ./) using

```mathematica
<<WiringPiLink`
```

## Library versions ##

 - 1.0.2 Added ability to read analog signals through the SPI interface and a MCP3008
 - 1.0.1 Basic GPIO pin reading and writing along with software PWM

## Notes and information ##

*Consider this a journal of changes and updates made*

### 150210 ### 

I started to add some error checking in the C code.  Currently, wiringPi_pinread, wiringpi_pinwrite and wiringpi_pinmode check to see if their arguments are valid.  Presently returning a *Mathematica* error message and the C still returns NOERROR.  The rationale for this is so that functions that fail due to invalid arguments can still return the appropriate type of value (I'm returning -1 at the moment).

Error messages passed this way, to my knowledge, cannot contain an additional argument, such that `General::errmsg=`1` is invalid` will not work as it does in a *Mathematica* session. I don't know if an easy solution at this point.

I noticed that one must run `gpio load spi` now that I have the MCP3008 setup call in the initialization function.  I haven't figured out how I want to handle options in package initialization yet.

