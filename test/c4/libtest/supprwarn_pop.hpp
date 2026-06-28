#ifndef C4_SUPPRWARN_POP_HPP_
#define C4_SUPPRWARN_POP_HPP_

#ifdef __clang__
#   pragma clang diagnostic pop
#elif defined(__GNUC__)
#   pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif /* C4_SUPPRWARN_POP_H */
