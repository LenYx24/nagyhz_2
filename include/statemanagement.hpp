#ifndef ENGINE_HPP
#define ENGINE_HPP
#include "renderer.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

class State;
class StateManager {
public:
    StateManager();

    void ChangeState(State *state);
    void PushState(State *state);
    void PopState();

    void Update(Renderer &r);

    inline bool hasState() const { return has_states; }
    void exit();

private:
    std::vector<State *> states;
    bool has_states;
};
class State {
public:
    virtual ~State() {}
    virtual void Update(StateManager &s, Renderer &r) = 0;

    virtual void ChangeState(StateManager &s) { s.ChangeState(this); }
};

#endif
