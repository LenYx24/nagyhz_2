#include "../include/engine.hpp"

Engine::Engine(sf::VideoMode winsize)
    : window(sf::RenderWindow(winsize, "lolmacrogame")), size(winsize) {
    is_running = true;
}

void Engine::ChangeState(State &state) {
    if (!states.empty()) {
        delete states.back();
        states.pop_back();
    }
    states.push_back(&state);
}
void Engine::PushState(State &state) { states.push_back(&state); }
void Engine::PopState() {
    if (!states.empty())
        states.pop_back();
}
void Engine::Update() { states.back()->Update(*this); }
void Engine::Draw() { states.back()->Draw(*this); }
void Engine::HandleEvents() { states.back()->HandleEvents(*this); }
void Engine::exit() {
    window.close();
    is_running = false;
}
