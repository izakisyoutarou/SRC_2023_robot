#include "bourns_1.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"

using rp_lib::f_pi;

Bourns1::Bourns1(Analog &pin):analog(pin){
  revolutionValue=(2*f_pi)/4*3;
  setOrigin(0);
  }

int Bourns1::setup(){
  return analog.setupAnalogIn();
  }

int Bourns1::resolution(){
  return analog.analogResolution();
  }

float Bourns1::read(){
  return ((analog.analogRead()*revolutionValue)-origin());
  }
