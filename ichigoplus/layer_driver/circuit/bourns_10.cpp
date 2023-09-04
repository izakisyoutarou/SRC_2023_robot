#include "bourns_10.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"

using rp_lib::f_pi;

Bourns10::Bourns10(Analog &pin):analog(pin){
  revolutionValue=10*2*f_pi;
  setOrigin(0);
  }

int Bourns10::setup(){
  return analog.setupAnalogIn();
  }

int Bourns10::resolution(){
  return analog.analogResolution();
  }

float Bourns10::read(){
  return analog.analogRead()*revolutionValue-origin();
  }
