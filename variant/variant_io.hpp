#ifndef MAPBOX_UTIL_VARIANT_IO_HPP
#define MAPBOX_UTIL_VARIANT_IO_HPP

#include <iosfwd>
#include <vector>

#include "variant.hpp"

template <typename T, typename _ = void>
struct is_vector { 
    static const bool value = false;
};
template <typename T>
struct is_vector< T,
                  typename std::enable_if<
                      std::is_same<T,
                              std::vector< typename T::value_type,
                                           typename T::allocator_type >
                             >::value
                  >::type
                >
{
    static const bool value = true;
};

namespace mapbox {
namespace util {

namespace detail {
// operator<< helper
class printer
{
  public:
    explicit printer(std::ostream& out)
        : out_(out) {}
    printer& operator=(printer const&) = delete;

    // visitor
    template <typename T>
    void operator()(T const& operand) const
    {
        out_ << operand << std::endl;
    }
	
	

	

  private:
    std::ostream& out_;
};
}

// operator<<
/*template <typename... Types>
 std::ostream &
operator<<( std::ostream & out, const variant<Types...> & rhs)
{
    detail::printer visitor(out);
    apply_visitor(visitor, rhs);
    return out;
}*/
} // namespace util
} // namespace mapbox

#endif // MAPBOX_UTIL_VARIANT_IO_HPP
