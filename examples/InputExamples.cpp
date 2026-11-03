#include <cassert>
#include <variant>

import Input;

int main()
{
    using namespace ikk;

    // An InputList describes the inputs that together activate an action.
    {
        const InputCombination jumpCombination{Keyboard::Space};
        assert(jumpCombination.count() == 1);
        assert(std::get<Keyboard::KeyCode>(jumpCombination[0]) == Keyboard::Space);

        const InputCombination moveCombination = Keyboard::W + Joystick::Axis::LY;
        assert(moveCombination.count() == 2);
        assert(std::get<Keyboard::KeyCode>(moveCombination[0]) == Keyboard::W);
        assert(std::get<Joystick::Axis>(moveCombination[1]) == Joystick::Axis::LY);

        const InputCombination extendedCombination = moveCombination + Mouse::Button::Right;
        assert(extendedCombination.count() == 3);
        assert(std::get<Mouse::Button>(extendedCombination[2]) == Mouse::Button::Right);
        assert(extendedCombination != moveCombination);
        assert((InputCombination{Keyboard::W, Joystick::Axis::LY} == moveCombination));

        InputCombination editableCombination{Keyboard::W};
        editableCombination.add(Joystick::HatState::Up);
        assert(editableCombination.count() == 2);
        assert(std::get<Joystick::HatState>(editableCombination[1]) == Joystick::HatState::Up);
    }

    // Bind combinations to action names and configure their callbacks/repeat behavior.
    {
        InputManager input;
        input.bind("jump", InputCombination{Keyboard::Space});
        input.bind("move", Keyboard::W + Keyboard::Up);
        input.on<Input::Action::Pressed>("jump", [] {});
        input.on<Input::Action::Released>("jump", [] {});
        input.on<Input::Action::Repeated>("move", [] {});
        input.setRepeatConfig("move", Input::RepeatSettings{});

        input.rebind("move", InputCombination{Joystick::Button::A, Joystick::Axis::LY});
        input.unbind("move");

        auto action = input.getInputState(ikk::Keyboard::S);
        int i = 2;
    }

    return 0;
}
