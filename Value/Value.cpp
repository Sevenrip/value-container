#include "Value.hpp"
#include "ValueVisitorEqual.hpp"
#include "ValueVisitorPrinter.hpp"

bool Value::operator!= (const Value& value) const noexcept
{
    return !(*this == value);
}

bool  Value::operator== (const Value& value) const noexcept
{
	return mapbox::util::apply_visitor(ValueVisitorEqual(),value._v, _v) ;
}

std::string Value::description(int depth) const
{
	return  mapbox::util::apply_visitor(ValueVisitorPrinter(depth), _v);
}

std::shared_ptr<const std::string> Value::asString() const noexcept
{
    return this->convertTo<std::string>();
}

int Value::asInt() const noexcept
{
    return this->convertTo<int>();
}

std::shared_ptr<Value::Vector> Value::asVector() const noexcept
{
    return this->convertTo<Value::Vector>();
}

std::ostream & operator<<(std::ostream & stream, const Value & v)
{
	stream << v.description();
	return stream;
}



