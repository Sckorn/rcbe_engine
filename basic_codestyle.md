# RCBE basic code style

**NOTE:** Name RCBE is an acronym working name of a company and a subject to change in the future.<br/>
**NOTE:** The code style file for IDE would be uploaded to an infra repo.

Here will be listed some basic code style rules to follow, while contributing to this codebase. 

1. Prefer `std` over `boost` implementations when possible. Resort to using `boost` functionality when no other option is available [writing your own code is most likely a bad decision].
2. Prefer functions over macro. Avoid macro usage if possible.
3. Headers:<br/>
3.1 All headers except private ones should be included via `#include <some/system/header.hpp>` instruction. Private headers are included like this `#include "header.hpp"`.<br/>
3.2 The order of inclusion is as follows: first included `std` headers, then come `boost` headers, then all other dependencies (i.e. `OpenGL`) headers and then engine headers. This order is intact unless there is a specific reason to break this order. The reason should be clarified int he comment.<br />
3.3 All headers of the engine should have `*.hpp` extension. <br />
3.4 All public headers of the engine should be installed using `rcbe` include prefix, i.e. `#include <rcbe/camera/Camera.hpp`. This notation should be used inside the project and when libraries are installed into the system. <br />
3.5 All headers should have an `ifdef`-lock of ofrmat `RCBE_ENGINE_HEADER_NAME_HPP`. <br />
3.6 If any implementation details, that are to be hidden, remain in a header file, they should be hidden under `detail` namespace.
4. Constants: <br />
4.1 Fundamental type constants should always be declared `constexpr` if possible. <br />
4.2 Constants declared in global scope, should be hidden under the corresponding namespace and declared `static`. <br />
4.3 Constant declared as a part of a `class` or a `struct` interface should be declared `static constexpr`, if not possible `static const`. <br />
4.4 All constants are written is uppercase i.e. `static constexpr size_t MY_CONSTANT = 1;`. <br />
5. Types: <br />
5.1 All type aliases should use **upper camel case**. <br />
5.2 All classes and class templates should use **upper camel case**. <br />
5.3 Class methods should use **camel case**. <br />
5.4 Class private member fields should end with an underscore symbol [`private_field_`]. <br />
5.5 Class shouldn't have any non constant public members, but if any they shouldn't contain an underscore at the end of its name. <br />
5.6 Structs and its methods should use **snake case**. <br />
6. All static functions should use **snake case**. <br /> 

   