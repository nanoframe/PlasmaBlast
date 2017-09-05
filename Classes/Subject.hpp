#ifndef Subject_hpp
#define Subject_hpp

#include <vector>

#include "EventType.hpp"
#include "Observer.hpp"

class Subject {
    friend class GameScene;

public:
    void addObserver(Observer *observer);

    Observer* removeObserver(int index);
    Observer* removeObserver(Observer *observer);

protected:
    void notify(ActionEvent action);

private:
    std::vector<Observer*> observers;
};

#endif // Subject_hpp

