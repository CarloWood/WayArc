#include <functional>
#include <iostream>
#include <memory>
#include <utility>

auto pi = std::make_unique<int>(0);

// no problems here!
auto foo = [q = std::move(pi)] {
  *q = 5;
  std::cout << *q << std::endl;
};

int main()
{
  std::move_only_function<void()> bar = std::move(foo);
  bar();
}
