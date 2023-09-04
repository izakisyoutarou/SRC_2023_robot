#pragma once

/*******************************************
 * SwitchingDisp ver1.0 2020/02/25
 * This program switch number of printf program for RC20_PR
 * 
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver1.0 2020/02/25 The first version
 *
 ******************************************/

#include "ichigoplus/layer_application/keyboard_reader.hpp"

class SwitchingDisp{
public:
    SwitchingDisp(KeyboardReader &keyboardReader):keyboardReader(keyboardReader){}
    void setup(char decrease_key, char reset_key, char increase_key){
        this->decrease_key = decrease_key;
        this->reset_key = reset_key;
        this->increase_key = increase_key;
    }
    void virtualReset(bool reset = false){
        if(reset) num = 0;
        keyboardReader(decrease_key);
        keyboardReader(reset_key);
        keyboardReader(increase_key);
    }
    int operator()(int max, int init){
        if(num == 0){
            num = init;
        }
        else if(keyboardReader(reset_key) != 0){
            num = init;
            keyboardReader(increase_key);
            keyboardReader(decrease_key);
        }
        else{
            num += keyboardReader(increase_key) - keyboardReader(decrease_key);
            while(num < 1){num += max;}
            while(num > max){num -= max;}
        }
        return num;
    }
    int printValue(int max, int init){return operator()(max,init);}
private:
    KeyboardReader &keyboardReader;
    int num = 0;
    char decrease_key = 0;
    char reset_key = 0;
    char increase_key = 0;
};
