#ifndef _C4_BASE64_HPP_
#define _C4_BASE64_HPP_

/** @file base64.hpp encoding/decoding for base64.
 * @see https://en.wikipedia.org/wiki/Base64
 * @see https://www.base64encode.org/
 * */

#include "c4/to_chars.hpp"
#include "c4/blob.hpp"

namespace c4 {

/** @defgroup base64_functions
 * Encode/decode binary data into base64.
 * @see https://en.wikipedia.org/wiki/Base64
 */

/** check that the given buffer is a valid base64 encoding
 * @ingroup base64_functions
 * @see https://en.wikipedia.org/wiki/Base64 */
bool base64_valid(csubstr encoded);

/** encode binary data into base64
 * @param encoded the output buffer
 * @return the number of bytes needed to return the output
 * @ingroup base64_functions
 * @see https://en.wikipedia.org/wiki/Base64 */
size_t base64_encode(substr encoded, cblob data);

/** check that the given is a valid base64 encoding
 * @ingroup base64_functions
 * @see https://en.wikipedia.org/wiki/Base64 */
size_t base64_decode(csubstr encoded, blob data);


template<typename CharOrConstChar>
struct base64_wrapper_
{
    template<class ...Args>
    base64_wrapper_(Args &&... args) : data(std::forward<Args>(args)...) {}
    base64_wrapper_() : data() {}
    blob_<CharOrConstChar> data;
};
using const_base64_wrapper = base64_wrapper_<cbyte>;
using base64_wrapper = base64_wrapper_<byte>;


/** mark a variable to be written in base64 format
 * @ingroup base64_functions
 * @ingroup formatting_functions */
template<class ...Args>
C4_ALWAYS_INLINE const_base64_wrapper cbase64(Args &&... args)
{
    return const_base64_wrapper(std::forward<Args>(args)...);
}
C4_ALWAYS_INLINE const_base64_wrapper cbase64(csubstr s)
{
    return const_base64_wrapper(s.str, s.len);
}

/** mark a variable to be written in base64 format
 * @ingroup base64_functions
 * @ingroup formatting_functions */
template<class ...Args>
C4_ALWAYS_INLINE base64_wrapper base64(Args &&... args)
{
    return base64_wrapper(std::forward<Args>(args)...);
}
C4_ALWAYS_INLINE base64_wrapper base64(substr s)
{
    return base64_wrapper(s.str, s.len);
}


/** write a variable in base64 format
 * @ingroup base64_functions
 * @ingroup formatting_functions */
inline size_t to_chars(substr buf, const_base64_wrapper b)
{
    return base64_encode(buf, b.data);
}

/** read a variable in base64 format
 * @ingroup base64_functions
 * @ingroup formatting_functions */
inline size_t from_chars(csubstr buf, base64_wrapper *b)
{
    return base64_decode(buf, b->data);
}

} // namespace c4

#endif /* _C4_BASE64_HPP_ */
