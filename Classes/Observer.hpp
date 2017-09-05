#ifndef Observer_hpp
#define Observer_hpp

#include "EventType.hpp"

class Observer {
public:
    virtual void onNotify(ActionEvent action) = 0;

};

#endif // Observer_hpp

