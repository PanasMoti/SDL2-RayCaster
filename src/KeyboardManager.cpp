#include "KeyboardManager.hpp"
#include <iostream>


KeyboardManager::KeyboardManager() {
    this->keys = std::vector<Key*>();
}
KeyboardManager::KeyboardManager(const KeyboardManager& cp) {
    *this = cp;
}
KeyboardManager::KeyboardManager(const std::string& keys) {
    for(unsigned char ch : keys) {
        this->keys.push_back(new Key(ch));
    }
}
void KeyboardManager::Add(const std::string& keys) {
    for(unsigned char ch : keys) {
        if(!this->contains(ch))
            this->keys.push_back(new Key(ch));
    }
}
void KeyboardManager::Remove(const std::string& keys) {
    for(unsigned char ch : keys) {
        if(this->contains(ch)) {
            for(auto it = this->keys.begin(); it != this->keys.end(); it++) {
                if(**it.base() == ch) this->keys.erase(it);
            }
        }
    }
}

Key* KeyboardManager::operator[](keycode code) {
    for(int i = 0; i < this->keys.size(); i++) {
        if(*this->keys[i] == code) return this->keys[i];
    }
    return NULL;
}

bool KeyboardManager::contains(const keycode code) {
    for(auto key : this->keys) {
        if(*key == code) return true;
    }
    return false;
}

void KeyboardManager::Press(keycode code) {
    for(auto key : this->keys) {
        if(*key == code) {
            key->Press();
            // std::cout << *key << std::endl;
        }
    }
}

void KeyboardManager::Release(keycode code) {
    for(auto key : this->keys) {
        if(*key == code) {
            key->Release();
            // std::cout << *key << std::endl;
        }
        
    }
}
