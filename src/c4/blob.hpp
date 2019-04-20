#include "c4/types.hpp"

/** @file blob.hpp Mutable and immutable binary data blobs.
*/

C4_BEGIN_NAMESPACE(c4)

template<class T> struct blob_;

using cblob = blob_<cbyte>;
using  blob = blob_< byte>;

template<class T>
struct blob_
{
    T *    buf;
    size_t len;

    C4_ALWAYS_INLINE constexpr blob_() noexcept : buf(), len() {}
    C4_ALWAYS_INLINE constexpr blob_(blob_ const&) noexcept = default;
    C4_ALWAYS_INLINE constexpr blob_(blob_     &&) noexcept = default;

    template<class U>
    C4_ALWAYS_INLINE constexpr blob_(U *ptr, size_t n=1) noexcept : buf(reinterpret_cast<T*>(ptr)), len(sizeof(U) * n) {}

    template<class U, size_t N>
    C4_ALWAYS_INLINE constexpr blob_(U (&arr)[N]) noexcept : buf(reinterpret_cast<T*>(arr)), len(sizeof(U) * N) {}

    template<size_t N>
    C4_ALWAYS_INLINE constexpr blob_(const char (&arr)[N]) noexcept : buf(reinterpret_cast<T*>(arr)), len(N-1) {}
    
    C4_ALWAYS_INLINE constexpr blob_(void       *ptr, size_t n) noexcept : buf(reinterpret_cast<T*>(ptr)), len(n) {}
    C4_ALWAYS_INLINE constexpr blob_(void const *ptr, size_t n) noexcept : buf(reinterpret_cast<T*>(ptr)), len(n) {}
};

C4_END_NAMESPACE(c4)
