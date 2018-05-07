#ifndef _C4_OPT_HPP_
#define _C4_OPT_HPP_

#include "c4/error.hpp"
#include "optionparser.h"

/** @file opt.hpp command line option parser utilities */

namespace c4 {
namespace opt {

using desc = option::Descriptor;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


// argument value checkers

void _arg_err(const char* msg1, option::Option const& opt, const char* msg2)
{
    fprintf(stderr, "%s", msg1);
    fwrite(opt.name, opt.namelen, 1, stderr);
    fprintf(stderr, "%s", msg2);
}

option::ArgStatus none(option::Option const&, bool)
{
    return option::ARG_NONE;
}

option::ArgStatus optional(option::Option const& option, bool)
{
    if (option.arg && option.name[option.namelen] != 0)
    {
        return option::ARG_OK;
    }
    else
    {
        return option::ARG_IGNORE;
    }
}

option::ArgStatus unknown(option::Option const& option, bool msg)
{
    if(msg) _arg_err("Unknown option '", option, "'\n");
    return option::ARG_ILLEGAL;
}

option::ArgStatus required(option::Option const& option, bool msg)
{
    if(option.arg != 0) return option::ARG_OK;
    if(msg) _arg_err("Option '", option, "' requires an argument\n");
    return option::ARG_ILLEGAL;
}

option::ArgStatus nonempty(option::Option const& option, bool msg)
{
    if(option.arg != 0 && option.arg[0] != 0) return option::ARG_OK;
    if(msg) _arg_err("Option '", option, "' requires a non-empty argument\n");
    return option::ARG_ILLEGAL;
}

option::ArgStatus numeric(option::Option const& option, bool msg)
{
    char* endptr = 0;
    if(option.arg != 0 && strtol(option.arg, &endptr, 10)){};
    if(endptr != option.arg && *endptr == 0) return option::ARG_OK;
    if(msg) _arg_err("Option '", option, "' requires a numeric argument\n");
    return option::ARG_ILLEGAL;
}

option::ArgStatus mandatory(option::Option const& option, bool msg)
{
    return nonempty(option, msg);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

struct positional_arg_iterator
{
    int i;
    option::Parser const* p;
    using value_type = const char*;
    const char* operator* () const { C4_CHECK(i < p->nonOptionsCount()); return p->nonOption(i); }
    positional_arg_iterator& operator++ () { ++i; return *this; }
    bool operator!= (positional_arg_iterator const& that) { C4_ASSERT(p == that.p); return i != that.i; }
    bool operator== (positional_arg_iterator const& that) { C4_ASSERT(p == that.p); return i == that.i; }
};

struct positional_arg_range
{
    option::Parser const* p;
    positional_arg_range(option::Parser const* p_) : p(p_) {}
    positional_arg_iterator begin() const { return positional_arg_iterator{0, p}; }
    positional_arg_iterator end() const { return positional_arg_iterator{p->nonOptionsCount(), p}; }
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template< size_t N >
struct Parser
{
    option::Descriptor const (&usage)[N];
    option::Option options[N];
    option::Option buffer[N];
    option::Parser parser;

    Parser(option::Descriptor const (&usage_)[N], int argc, char **argv)
        :
        usage(usage_),
        options(),
        buffer(),
        parser(usage, argc>0?argc-1:argc, argc>0?argv+1:argv, options, buffer)
    {
        if(parser.error())
        {
            help();
            C4_ERROR("parser error");
        }
    }

    void check_mandatory() const
    {
        for(option::Descriptor const& d : usage)
        {
            if(d.check_arg == &c4::opt::mandatory && options[d.index].arg == nullptr)
            {
                C4_ERROR("mandatory option is missing:\n%s\n", d.help);
            }
        }
    }

    void help() const { option::printUsage(fwrite, stdout, usage, /*columns*/80); }

    option::Option operator[] (int i) const { C4_CHECK(i < N); return options[i]; }
    const char* operator() (int i) const { C4_CHECK(i < N); C4_CHECK_MSG(options[i].arg, "error in option %d: '%.*s'", i, options[i].namelen, options[i].name); return options[i].arg; }

public:

    positional_arg_range args() const { return &parser; }
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template< size_t N >
Parser<N> make_parser(option::Descriptor const (&usage)[N], int argc, char **argv)
{
    return Parser<N>(usage, argc, argv);
}

} // namespace opt
} // namespace c4


#endif /* _C4_OPT_HPP_ */
