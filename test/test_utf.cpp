#include "c4/test.hpp"
#ifndef C4CORE_SINGLE_HEADER
#include "c4/std/string.hpp"
#include "c4/std/vector.hpp"
#include "c4/format.hpp"
#include "c4/utf.hpp"
#endif

#include "c4/libtest/supprwarn_push.hpp"

#include <cstring>

namespace c4 {

struct utft
{
    csubstr code_point;
    csubstr character;
    uint32_t character_val;
    csubstr character_val_hex;
};
constexpr const utft utf_chars[] = {
#include "./utfchars.inc"
};

TEST_CASE("utf.decode_code_point")
{
    size_t i = 0;
    char decoded_buf[64];
    for(auto uc : utf_chars)
    {
        INFO("utfchars[", i, "]: codepoint=", uc.code_point, ' ',
             "character=", uc.character.empty() ? csubstr{} : uc.character, ' ',
             "val=", uc.character_val_hex, '(', uc.character_val, ')');
        i++;
        csubstr cpstr = uc.code_point.sub(2).triml('0');
        if(cpstr.empty())
            continue;
        csubstr decoded = decode_code_point(decoded_buf, cpstr);
        CHECK_UNARY(uc.code_point.begins_with("U+"));
        if(uc.character.empty())
            continue;
        CHECK_EQ(decoded.len, uc.character.len);
        CHECK_EQ(decoded, uc.character);
    }
}

void test_with_bom(substr input, csubstr bom)
{
    CHECK_UNARY(input.begins_with(bom));
    {
        csubstr s = input;
        CHECK_EQ(first_non_bom(s), bom.len);
        CHECK_EQ(get_bom(s).len, bom.len);
        CHECK_EQ(get_bom(s), bom);
        CHECK_UNARY(get_bom(s).is_sub(s));
        CHECK_UNARY(skip_bom(s).is_sub(s));
        CHECK_EQ(skip_bom(s).len, input.len - bom.len);
    }
    {
        substr s = input;
        CHECK_EQ(first_non_bom(s), bom.len);
        CHECK_EQ(get_bom(s), bom);
        CHECK_EQ(get_bom(s).len, bom.len);
        CHECK_UNARY(get_bom(s).is_sub(s));
        CHECK_UNARY(skip_bom(s).is_sub(s));
        CHECK_EQ(skip_bom(s).len, input.len - bom.len);
    }
}

void test_without_bom(substr input, csubstr bom)
{
    CHECK_UNARY(!input.begins_with(bom));
    {
        csubstr s = input;
        CHECK_EQ(first_non_bom(s), 0);
        CHECK_EQ(get_bom(s).len, 0);
        CHECK_EQ(skip_bom(s).len, s.len);
    }
    {
        substr s = input;
        CHECK_EQ(first_non_bom(s), 0);
        CHECK_EQ(get_bom(s).len, 0);
        CHECK_EQ(skip_bom(s).len, s.len);
    }
}


TEST_CASE("utf.bom")
{
    csubstr strs[] = {
        csubstr(""),
        csubstr("1"),
        csubstr("12"),
        csubstr("123"),
        csubstr("1234"),
        csubstr("12345"),
    };
    // https://en.wikipedia.org/wiki/Byte_order_mark#Byte-order_marks_by_encoding
    struct bomcase {
        csubstr name;
        csubstr bom;
        size_t bom_len;
    };
    const bomcase boms[] = {
        bomcase{csubstr("UTF-8"      ), csubstr("\xEF\xBB\xBF"), 3},
        bomcase{csubstr("UTF-16 (BE)"), csubstr("\xFE\xFF"), 2},
        bomcase{csubstr("UTF-16 (LE)"), csubstr("\xFF\xFE"), 2},
        bomcase{csubstr("UTF-32 (BE)"), csubstr("\x00\x00\xFE\xFF"), 4},
        bomcase{csubstr("UTF-32 (LE)"), csubstr("\xFF\xFE\x00\x00"), 4},
        bomcase{csubstr("UTF-7"      ), csubstr("\x2B\x2F\x76"), 3},
        bomcase{csubstr("UTF-1"      ), csubstr("\xF7\x64\x4C"), 3},
        bomcase{csubstr("UTF-EBCDIC" ), csubstr("\xDD\x73\x66\x73"), 4},
        bomcase{csubstr("SCSU"       ), csubstr("\x0E\xFE\xFF"), 3},
        bomcase{csubstr("BOCU-1"     ), csubstr("\xFB\xEE\x28"), 3},
        bomcase{csubstr("GB18030"    ), csubstr("\x84\x31\x95\x33"), 4},
    };
    std::string buf;
    for(bomcase bomc : boms)
    {
        csubstr name = bomc.name;
        csubstr bom = bomc.bom;
        size_t bomlen = bomc.bom_len;
        INFO("name=[", name.len, "]~~~", name, "~~~");
        INFO("bom=[", bom.len, "]~~~", bom, "~~~");
        CHECK_EQ(bomlen, bom.len);
        for(csubstr str : strs)
        {
            INFO("str=[", str.len, "]~~~", str, "~~~");
            {
                buf.clear();
                buf.append(str.str, str.len);
                CHECK_EQ(buf.size(), str.len);
                test_without_bom(to_substr(buf), bom);
            }
            {
                buf.clear();
                buf.append(bom.str, bom.len);
                CHECK_EQ(buf.size(), bom.len);
                buf.append(str.str, str.len);
                CHECK_EQ(buf.size(), bom.len + str.len);
                INFO("buf=[", buf.size(), "]~~~", buf, "~~~");
                test_with_bom(to_substr(buf), bom);
            }
        }
    }

}

} // namespace c4
