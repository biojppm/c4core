#ifndef C4_UTF_HPP_
#define C4_UTF_HPP_

#include "c4/language.hpp"
#include "c4/substr_fwd.hpp"
#include <stddef.h>
#include <stdint.h>

/** @file utf.hpp utilities for UTF and Byte Order Mark */

namespace c4 {

substr decode_code_point(substr out, csubstr code_point);
size_t decode_code_point(uint8_t *C4_RESTRICT buf, size_t buflen, uint32_t code);

/** return the position of the first character not belonging to the
 * Byte Order Mark, or 0 if there is no Byte Order Mark. */
size_t first_non_bom(csubstr s);

/** get the Byte Order Mark, or an empty string if there is Byte Order Mark */
substr get_bom(substr s);
/** get the Byte Order Mark, or an empty string if there is Byte Order Mark */
csubstr get_bom(csubstr s);

/** skip the Byte Order Mark, or get the full string if there is Byte Order Mark */
substr skip_bom(substr s);
/** skip the Byte Order Mark, or get the full string if there is Byte Order Mark */
csubstr skip_bom(csubstr s);

} // namespace c4

#endif // C4_UTF_HPP_
