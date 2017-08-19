引入`boost::property_tree::ptree`解析json时, 在ptree下层使用的spirit库默认不是线程安全的.
需要加一个 `#define BOOST_SPIRIT_THREADSAFE` 写在`#include <boost/property_tree/ptree.hpp>`之前.

`boost::lockfree::queue` 中的元素必须是 trivial, 例如int, char, char * 等基本类型, 不能是 std::string 等复杂类型
