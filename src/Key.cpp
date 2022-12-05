#include "Key.hpp"

Key::Key(keycode code) {
    this->code = code;
    this->state = State::OFF;
}
Key::Key(const Key& k) {
    *this = k;
}
Key::Key(keycode code,State state ) {
    this->code = code;
    this->state = state;
}
void Key::Press() {
    this->state = State::ON;
}
void Key::Release() {
    this->state = State::OFF;
}
bool Key::operator==(keycode code) const {
    return this->code == code;
}
Key Key::operator!() const {
    return Key(this->code,(*this)? State::OFF : State::ON);
}