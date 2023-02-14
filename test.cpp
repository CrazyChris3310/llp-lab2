#include <stdexcept>
#include <iostream>

class ConstantBase {
};

template <typename T>
class Constant : public ConstantBase {
    T value;
    public:
        Constant(T value) {
            this->value = value;
        }
        T getValue() {
            return this->value;
        }
};

class Condition {
private:
  ConstantBase* constant_;

public:
  Condition(ConstantBase* constant) : constant_(constant) {}

  template <typename T>
    T getConstant() const {
        auto typed_constant = dynamic_cast<Constant<T>*>(constant_);
        if (!typed_constant) {
        throw std::runtime_error("Incorrect constant type");
        }
        return typed_constant.getValue();
    }

  ~Condition() { delete constant_; }
};
int main() {
    Constant<int> c(1);
    Condition cond(&c);
    std::cout << cond.getConstant<int>() << std::endl;
    return 0;
}