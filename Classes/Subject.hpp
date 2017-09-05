#ifndef Subject_hpp
#define Subject_hpp

#include <vector>

#include "Observer.hpp"

class Subject {
public:
    void addObserver(Observer *observer);

    Observer* removeObserver(int index);
    Observer* removeObserver(Observer *observer);

protected:
    void notify(int action);

private:
    std::vector<Observer*> observers;
};

#endif // Subject_hpp

