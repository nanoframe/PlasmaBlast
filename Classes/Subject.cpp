#include "Subject.hpp"

void Subject::addObserver(Observer *observer) {
    observers.push_back(observer);
}

Observer* Subject::removeObserver(int index) {
    auto it = observers.begin() + index;
    observers.erase(it);
    return *it;
}

Observer* Subject::removeObserver(Observer *observer) {
    for (auto it = observers.begin(); it != observers.end(); it++) {
        if (observer == *it) {
            observers.erase(it);
            return *it;
        }
    }
    return nullptr;
}

void Subject::notify(int action) {
    for (auto it = observers.begin(); it != observers.end(); it++) {
        (*it)->onNotify(action);
    }
}

