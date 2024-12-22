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

    struct asdf {
        // Добавьте поля состояния при необходимости
        void PrintState() const {
            std::cout << "State: asdf" << std::endl;
        }
    };

    struct asdfasdf {
        // Добавьте поля состояния при необходимости
        void PrintState() const {
            std::cout << "State: asdfasdf" << std::endl;
        }
    };

    struct asdfasdf {
        // Добавьте поля состояния при необходимости
        void PrintState() const {
            std::cout << "State: asdfasdf" << std::endl;
        }
    };

    struct asdfasd {
        // Добавьте поля состояния при необходимости
        void PrintState() const {
            std::cout << "State: asdfasd" << std::endl;
        }
    };

} // namespace state

namespace event {
    struct asdf {
        // Добавьте поля события при необходимости
    };

    struct sadfasdf {
        // Добавьте поля события при необходимости
    };

    struct ssss {
        // Добавьте поля события при необходимости
    };

    struct  {
        // Добавьте поля события при необходимости
    };

    struct sssssss {
        // Добавьте поля события при необходимости
    };

    struct asdfsad {
        // Добавьте поля события при необходимости
    };

    struct  {
        // Добавьте поля события при необходимости
    };

} // namespace event

using StateVariant = std::variant<state::asdf, state::asdf, state::asdfasdf, state::asdfasdf, state::asdfasd>;

using EventVariant = std::variant<event::asdf, event::sadfasdf, event::ssss, event::, event::sssssss, event::asdfsad, event::>;

template<typename State, typename Event>
StateVariant onEvent(const State&, const Event&) {
    throw std::logic_error{"Unsupported state transition"};
}

StateVariant onEvent(const state::asdf&, const event::asdf&) {
    std::cout << "Transition: asdf -> asdfasdf on asdf" << std::endl;
    return state::asdfasdf{};
}

StateVariant onEvent(const state::asdf&, const event::sadfasdf&) {
    std::cout << "Transition: asdf -> asdf on sadfasdf" << std::endl;
    return state::asdf{};
}

StateVariant onEvent(const state::asdf&, const event::ssss&) {
    std::cout << "Transition: asdf -> asdfasd on ssss" << std::endl;
    return state::asdfasd{};
}

StateVariant onEvent(const state::asdfasd&, const event::&) {
    std::cout << "Transition: asdfasd -> asdfasdf on " << std::endl;
    return state::asdfasdf{};
}

StateVariant onEvent(const state::asdfasdf&, const event::sssssss&) {
    std::cout << "Transition: asdfasdf -> asdfasdf on sssssss" << std::endl;
    return state::asdfasdf{};
}

StateVariant onEvent(const state::asdfasdf&, const event::asdfsad&) {
    std::cout << "Transition: asdfasdf -> asdf on asdfsad" << std::endl;
    return state::asdf{};
}

StateVariant onEvent(const state::asdf&, const event::&) {
    std::cout << "Transition: asdf -> asdf on " << std::endl;
    return state::asdf{};
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
    sm.processEvent(event::asdf{});
    sm.reportCurrentState();
    sm.processEvent(event::sadfasdf{});
    sm.reportCurrentState();
    sm.processEvent(event::ssss{});
    sm.reportCurrentState();
    sm.processEvent(event::{});
    sm.reportCurrentState();
    sm.processEvent(event::sssssss{});
    sm.reportCurrentState();
    sm.processEvent(event::asdfsad{});
    sm.reportCurrentState();
    sm.processEvent(event::{});
    sm.reportCurrentState();
    return 0;
}
