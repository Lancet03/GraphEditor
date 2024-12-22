#include <iostream>
#include <variant>
#include <stdexcept>

namespace state {
    struct asdf {
        // Добавьте поля состояния при необходимости
        void PrintState() const {
            std::cout << "State: asdf" << std::endl;
        }
    };

    struct sadfasf {
        // Добавьте поля состояния при необходимости
        void PrintState() const {
            std::cout << "State: sadfasf" << std::endl;
        }
    };

    struct aaaaaaaaaaaa {
        // Добавьте поля состояния при необходимости
        void PrintState() const {
            std::cout << "State: aaaaaaaaaaaa" << std::endl;
        }
    };

    struct sssssssss {
        // Добавьте поля состояния при необходимости
        void PrintState() const {
            std::cout << "State: sssssssss" << std::endl;
        }
    };

} // namespace state

namespace event {
    struct sdafasdf {
        // Добавьте поля события при необходимости
    };

    struct sadfs {
        // Добавьте поля события при необходимости
    };

    struct sadfasdf {
        // Добавьте поля события при необходимости
    };

    struct asdfasdf {
        // Добавьте поля события при необходимости
    };

    struct asdfasdf {
        // Добавьте поля события при необходимости
    };

} // namespace event

using StateVariant = std::variant<state::asdf, state::sadfasf, state::aaaaaaaaaaaa, state::sssssssss>;

using EventVariant = std::variant<event::sdafasdf, event::sadfs, event::sadfasdf, event::asdfasdf, event::asdfasdf>;

template<typename State, typename Event>
StateVariant onEvent(const State&, const Event&) {
    throw std::logic_error{"Unsupported state transition"};
}

StateVariant onEvent(const state::asdf&, const event::sdafasdf&) {
    std::cout << "Transition: asdf -> aaaaaaaaaaaa on sdafasdf" << std::endl;
    return state::aaaaaaaaaaaa{};
}

StateVariant onEvent(const state::asdf&, const event::sadfs&) {
    std::cout << "Transition: asdf -> sadfasf on sadfs" << std::endl;
    return state::sadfasf{};
}

StateVariant onEvent(const state::sadfasf&, const event::sadfasdf&) {
    std::cout << "Transition: sadfasf -> sssssssss on sadfasdf" << std::endl;
    return state::sssssssss{};
}

StateVariant onEvent(const state::aaaaaaaaaaaa&, const event::asdfasdf&) {
    std::cout << "Transition: aaaaaaaaaaaa -> sadfasf on asdfasdf" << std::endl;
    return state::sadfasf{};
}

StateVariant onEvent(const state::asdf&, const event::asdfasdf&) {
    std::cout << "Transition: asdf -> sssssssss on asdfasdf" << std::endl;
    return state::sssssssss{};
}

auto StateReporter = [](const auto& s){ s.PrintState(); };
auto EventProcessor = [](const auto& st, const auto& ev) { return onEvent(st, ev); };

class StateMachine {
private:
    StateVariant state_;
public:
    void startMachine() {
        state_ = state::asdf{};
    }

    void processEvent(const EventVariant& event) {
        state_ = std::visit(EventProcessor, state_, event);
    }

    void reportCurrentState() {
        std::visit(StateReporter, state_);
    }
};

int main() {
    StateMachine sm;
    sm.startMachine();
    sm.reportCurrentState();

    // Example usage:
    sm.processEvent(event::sdafasdf{});
    sm.reportCurrentState();
    sm.processEvent(event::sadfs{});
    sm.reportCurrentState();
    sm.processEvent(event::sadfasdf{});
    sm.reportCurrentState();
    sm.processEvent(event::asdfasdf{});
    sm.reportCurrentState();
    sm.processEvent(event::asdfasdf{});
    sm.reportCurrentState();
    return 0;
}
