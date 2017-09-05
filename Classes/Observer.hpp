#ifndef Observer_hpp
#define Observer_hpp

class Observer {
public:
    virtual void onNotify(int action) = 0;

private:

};

#endif // Observer_hpp

