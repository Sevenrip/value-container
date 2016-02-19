#pragma once

#include "Value.hpp"

using Vector = std::vector<std::shared_ptr<const Value>>;

template<typename T>
using StringMap =  std::unordered_map<std::shared_ptr<const std::string> , std::shared_ptr<const T> >;

template<typename T>
using StringMapEntry = std::pair<std::shared_ptr<const std::string>, std::shared_ptr<const T> >;






