#include "c4/test.hpp"

namespace c4 {

size_t TestErrorOccurs::num_errors = 0;
std::jmp_buf TestErrorOccurs::s_jmp_env_expect_error = {};
std::string TestErrorOccurs::s_jmp_msg;

} // namespace c4
