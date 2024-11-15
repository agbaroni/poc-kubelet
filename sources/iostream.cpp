#include <ostream>
#include <new>

using namespace __internal;
using namespace std;

ios_base::Init __ios_base_init;

namespace std {
    char cout[sizeof(ostream)];
}

ios_base::Init::Init() {
    new (&cout) ostream(1);
}

ios_base::Init::~Init() {}
