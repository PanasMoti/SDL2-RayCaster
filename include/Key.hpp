#ifndef KEY_HPP_
#define KEY_HPP_

typedef unsigned char keycode;
#include <ostream>


struct Key {
    enum class State {
        OFF = 0,
        ON = 1
    };
    keycode code;
    State state;

    Key(keycode);
    Key(const Key&);
    Key(keycode,State);

    bool operator==(keycode code) const;
    operator bool() const
    {
        return (this->state == State::ON) ? true : false;
    } // the state | true if down , false if up

    bool IsPressed() const {
        return this->state == State::ON;
    }
    void Press();
    void Release();

    Key operator!() const; // flip

    friend std::ostream& operator<<(std::ostream& os,const Key& k) {
        return os << "['" <<k.code << "'," << ((k)? "ON" : "OFF") << "]";
    }
};



#endif // KEY_HPP_