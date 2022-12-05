#ifndef KEYBOARDMANAGER_HPP_
#define KEYBOARDMANAGER_HPP_
#include "Key.hpp"
#include <vector>
#include <string>
#include <ostream>


class KeyboardManager
{
    public:
        KeyboardManager();
        KeyboardManager(const KeyboardManager&);
        KeyboardManager(const std::string&);

        void Add(const std::string&);
        void Remove(const std::string&);


        friend std::ostream& operator<<(std::ostream& os, const KeyboardManager m) {
            for(int i = 0; i < m.keys.size(); i++) {
                os << m.keys[i] << "\n";
            }
            return os;
        }

        void Press(keycode);
        void Release(keycode);
        Key* operator[](keycode);
        bool contains(const keycode);
    private:
        std::vector<Key*> keys;
};



#endif // KEYBOARDMANAGER_HPP_