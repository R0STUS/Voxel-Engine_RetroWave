#ifndef DELEGATES_H_
#define DELEGATES_H_

#include <glm/glm.hpp>
#include <functional>
#include <string>

using runnable = std::function<void()>;

// data sources
using wstringsupplier = std::function<std::wstring()>;
using doublesupplier = std::function<double()>;
using boolsupplier = std::function<bool()>;
using vec2supplier = std::function<glm::vec2()>;

using stringconsumer = std::function<void(const std::string&)>;
using wstringconsumer = std::function<void(const std::wstring&)>;
using doubleconsumer = std::function<void(double)>;
using boolconsumer = std::function<void(bool)>;
using int_array_consumer = std::function<void(const int[], size_t)>;
using wstringchecker = std::function<bool(const std::wstring&)>;

#endif // DELEGATES_H_
