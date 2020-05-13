#include "c4/format.hpp"

namespace c4 {

size_t sprintf(substr buf, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    /* vsnprintf() returns number of characters written if successful or
     * negative value if an error occurred. If the resulting string gets
     * truncated due to buf_size limit, returns the total number of
     * characters (not including the terminating null-byte) which would have
     * been written, if the limit was not imposed.
     *
     * @see http://en.cppreference.com/w/cpp/io/c/vfprintf
     */
    int inum = ::vsnprintf(buf.str, buf.len, fmt, args);

    /* if the retval is negative, maybe there's a formatting error so it's
     * impossible to know the length of the needed string.
     *
     * So return a huge value to try to cause a mess to let the client know
     * early. */
    size_t snum = inum >= 0 ? (size_t)inum : (size_t)-1;

    va_end(args);

    return snum;
}


size_t to_chars(substr buf, fmt::const_raw_wrapper r)
{
    void * vptr = buf.str;
    size_t space = buf.len;
    auto ptr = (decltype(buf.str)) std::align(r.alignment, r.len, vptr, space);
    if(ptr == nullptr)
    {
        // if it was not possible to align, return a conservative estimate
        // of the required space
        return r.alignment + r.len;
    }
    C4_CHECK(ptr >= buf.begin() && ptr <= buf.end());
    size_t sz = (ptr - buf.str) + r.len;
    if(sz <= buf.len)
    {
        memcpy(ptr, r.buf, r.len);
    }
    return sz;
}


size_t from_chars(csubstr buf, fmt::raw_wrapper *r)
{
    void * vptr = (void*)buf.str;
    size_t space = buf.len;
    auto ptr = (decltype(buf.str)) std::align(r->alignment, r->len, vptr, space);
    C4_CHECK(ptr != nullptr);
    C4_CHECK(ptr >= buf.begin() && ptr <= buf.end());
    //size_t dim = (ptr - buf.str) + r->len;
    memcpy(r->buf, ptr, r->len);
    size_t sz = (ptr - buf.str) + r->len;
    return sz;
}


} // namespace c4
