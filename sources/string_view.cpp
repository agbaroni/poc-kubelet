#include <string_view>

using namespace std;

size_t string_view::size() const noexcept {
    auto p = m_data;
    auto n = 0;

    while (p and *p) {
	n++;
	p++;
    }

    return n;
}

namespace std {
    ostream& operator<<(ostream& os, string_view sv) {
	os << sv.m_data;
	//__internal::write(os, sv.m_data, sv.size());

	return os;
    }
}
