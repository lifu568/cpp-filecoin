#include "common/outcome.hpp"

namespace Dev {

    enum class ExampleError { FIRST, SECOND };
}

OUTCOME_HPP_DECLARE_ERROR(Dev, ExampleError)
