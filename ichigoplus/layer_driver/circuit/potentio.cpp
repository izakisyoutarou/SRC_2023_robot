#include "potentio.hpp"
void Potentio::setOrigin(float originValue){
  this->originValue=originValue;
  }

float Potentio::origin(){
  return originValue;
  }

float Potentio::revolution(){
  return revolutionValue;
  }
