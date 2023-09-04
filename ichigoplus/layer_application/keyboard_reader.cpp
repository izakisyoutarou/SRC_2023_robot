#include "keyboard_reader.hpp"

void KeyboardReader::cycle() {
    (*read_buf).fill(false);
    auto a = read_buf;
    read_buf = write_buf;
    write_buf = a;
}