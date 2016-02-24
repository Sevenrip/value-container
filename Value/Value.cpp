#include "Value.hpp"
#include "ValueVisitorEqual.hpp"
#include "ValueVisitorPrinter.hpp"

bool Value::operator!= (const Value& value) const noexcept
{
    return !(*this == value);
}

bool  Value::operator== (const Value& value) const noexcept
{
	return mapbox::util::apply_visitor(ValueVisitorEqual(),value._holder, _holder) ;
}

std::string Value::description(int depth) const
{
	return  mapbox::util::apply_visitor(ValueVisitorPrinter(depth), _holder);
}

std::shared_ptr<const std::string> Value::asString() const noexcept
{
    return this->convertTo<std::shared_ptr<const std::string>>();
}

int Value::asInt() const noexcept
{
    return this->convertTo<unsigned int>();
}

unsigned int Value::asUnsignedInt() const noexcept
{
	return this->convertTo<unsigned int>();
}

bool Value::asBool() const noexcept
{
	return this->convertTo<bool>();
}

float Value::asFloat() const noexcept
{
	return this->convertTo<float>();	
}

std::shared_ptr<const Value::StringMap> Value::asMap() const noexcept
{
	return this->convertTo<std::shared_ptr<const StringMap>>();
}

std::shared_ptr<const Value::Vector> Value::asVector() const noexcept
{
    return this->convertTo<std::shared_ptr<const Vector>>();
}

bool Value::isNull() const noexcept
{
	return _holder.valid();
}

bool Value::isUnsignedInt() const noexcept
{
	return _holder.is<unsigned int>();
}
bool Value::isBool() const noexcept
{
	return _holder.is<bool>();
}
bool Value::isFloat() const noexcept
{
	return _holder.is<float>();
}
bool Value::isString() const noexcept
{
	return _holder.is<std::shared_ptr<const std::string>>();
}
bool Value::isVector() const noexcept
{
	return _holder.is<std::shared_ptr<const Vector>>();
}
bool Value::isMap() const noexcept
{
	return _holder.is<std::shared_ptr<const StringMap>>();	
}


std::ostream & operator<<(std::ostream & stream, const Value & v)
{
	stream << v.description();
	return stream;
}



