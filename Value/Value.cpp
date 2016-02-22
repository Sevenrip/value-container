#include "Value.hpp"
#include "ValueVisitorEqual.hpp"
#include "ValueVisitorPrinter.hpp"

bool Value::operator!= (const Value& value) const noexcept
{
    return !(*this == value);
}
bool  Value::operator!= (const Value& value) noexcept
{
    return !(*this == value);
}

bool  Value::operator== (const Value& value) const noexcept
{
    const auto &t = *this;
    return t == value;
}

bool  Value::operator== (const Value& value) noexcept
{
	return mapbox::util::apply_visitor(ValueVisitorEqual(),value._v, _v) ;
}

std::string Value::description(int depth) const
{
	return  mapbox::util::apply_visitor(ValueVisitorPrinter(depth), _v);
}

std::shared_ptr<const std::string> Value::asString(std::shared_ptr<const std::string> defaultValue) const noexcept
{
    return this->convertTo<std::string>(defaultValue);
}

int Value::asInt(int defaultValue) const noexcept
{
    return this->convertTo<int>(defaultValue);
}/*

std::shared_ptr<Value::Vector> Value::asVector() const noexcept
{
    return this->convertTo<Value::Vector>();
}*/

std::ostream & operator<<(std::ostream & stream, const Value & v)
{
	stream << v.description();
	return stream;
}



