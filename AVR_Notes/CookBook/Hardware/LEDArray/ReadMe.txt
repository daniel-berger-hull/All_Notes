The LED Array is a 8x8 led matrix, that is often combined toguetter in 4 matrix (4x (8x8)).



The connection is well explained here,
   http://www.belajarduino.com/2016/08/4in1-dot-matrix-led-module-32x8-pixel.html

But I got success only with the LEDMatrixDriver library.
  https://github.com/bartoszbielawski/LEDMatrixDriver

PINS: 

LED Matrix           Arduino
-----------------------------
  CLK is connected to 13
  DIN is connected to 11
   CS is connected to 10**
   
   ** The LEDMatrixDriver lib offer a variable to set for the CS pin.

