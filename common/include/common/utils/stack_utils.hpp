#ifndef RCBE_ENGINE_STACK_UTILS_HPP
#define RCBE_ENGINE_STACK_UTILS_HPP

#include <stack>

namespace rcbe::utils {
template <typename Element>
Element pop(std::stack<Element> &s) {
    auto e = s.top();
    s.pop();
    return e;
}
}

#endif //RCBE_ENGINE_STACK_UTILS_HPP
