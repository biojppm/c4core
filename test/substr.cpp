#include "c4/test.hpp"
#include "c4/substr.hpp"

namespace c4 {

TEST(substr, ctor_from_char)
{
    char buf1[] = "{foo: 1}";
    char buf2[] = "{foo: 2}";
    substr s(buf1);
    EXPECT_EQ(s, "{foo: 1}");
    s = buf2;
    EXPECT_EQ(s, "{foo: 2}");
}

TEST(csubstr, ctor_from_char)
{
    char buf1[] = "{foo: 1}";
    char buf2[] = "{foo: 2}";
    substr s(buf1);
    EXPECT_EQ(s, "{foo: 1}");
    s = buf2;
    EXPECT_EQ(s, "{foo: 2}");
}

TEST(substr, begins_with)
{
    EXPECT_TRUE (csubstr(": ").begins_with(":" ));
    EXPECT_TRUE (csubstr(": ").begins_with(':' ));
    EXPECT_FALSE(csubstr(":") .begins_with(": "));

    EXPECT_TRUE (csubstr(    "1234").begins_with('0', 0));
    EXPECT_TRUE (csubstr(   "01234").begins_with('0', 1));
    EXPECT_FALSE(csubstr(   "01234").begins_with('0', 2));
    EXPECT_TRUE (csubstr(  "001234").begins_with('0', 1));
    EXPECT_TRUE (csubstr(  "001234").begins_with('0', 2));
    EXPECT_FALSE(csubstr(  "001234").begins_with('0', 3));
    EXPECT_TRUE (csubstr( "0001234").begins_with('0', 1));
    EXPECT_TRUE (csubstr( "0001234").begins_with('0', 2));
    EXPECT_TRUE (csubstr( "0001234").begins_with('0', 3));
    EXPECT_FALSE(csubstr( "0001234").begins_with('0', 4));
    EXPECT_TRUE (csubstr("00001234").begins_with('0', 1));
    EXPECT_TRUE (csubstr("00001234").begins_with('0', 2));
    EXPECT_TRUE (csubstr("00001234").begins_with('0', 3));
    EXPECT_TRUE (csubstr("00001234").begins_with('0', 4));
    EXPECT_FALSE(csubstr("00001234").begins_with('0', 5));
}

TEST(substr, ends_with)
{
    EXPECT_TRUE(csubstr("{% if foo %}bar{% endif %}").ends_with("{% endif %}"));

    EXPECT_TRUE (csubstr("1234"    ).ends_with('0', 0));
    EXPECT_TRUE (csubstr("12340"   ).ends_with('0', 1));
    EXPECT_FALSE(csubstr("12340"   ).ends_with('0', 2));
    EXPECT_TRUE (csubstr("123400"  ).ends_with('0', 1));
    EXPECT_TRUE (csubstr("123400"  ).ends_with('0', 2));
    EXPECT_FALSE(csubstr("123400"  ).ends_with('0', 3));
    EXPECT_TRUE (csubstr("1234000" ).ends_with('0', 1));
    EXPECT_TRUE (csubstr("1234000" ).ends_with('0', 2));
    EXPECT_TRUE (csubstr("1234000" ).ends_with('0', 3));
    EXPECT_FALSE(csubstr("1234000" ).ends_with('0', 4));
    EXPECT_TRUE (csubstr("12340000").ends_with('0', 1));
    EXPECT_TRUE (csubstr("12340000").ends_with('0', 2));
    EXPECT_TRUE (csubstr("12340000").ends_with('0', 3));
    EXPECT_TRUE (csubstr("12340000").ends_with('0', 4));
    EXPECT_FALSE(csubstr("12340000").ends_with('0', 5));
}

TEST(substr, first_of)
{
    EXPECT_EQ(csubstr("012345").first_of('a'), csubstr::npos);
    EXPECT_EQ(csubstr("012345").first_of("ab"), csubstr::npos);

    EXPECT_EQ(csubstr("012345").first_of('0'), 0u);
    EXPECT_EQ(csubstr("012345").first_of("0"), 0u);
    EXPECT_EQ(csubstr("012345").first_of("01"), 0u);
    EXPECT_EQ(csubstr("012345").first_of("10"), 0u);
    EXPECT_EQ(csubstr("012345").first_of("012"), 0u);
    EXPECT_EQ(csubstr("012345").first_of("210"), 0u);
    EXPECT_EQ(csubstr("012345").first_of("0123"), 0u);
    EXPECT_EQ(csubstr("012345").first_of("3210"), 0u);
    EXPECT_EQ(csubstr("012345").first_of("01234"), 0u);
    EXPECT_EQ(csubstr("012345").first_of("43210"), 0u);
    EXPECT_EQ(csubstr("012345").first_of("012345"), 0u);
    EXPECT_EQ(csubstr("012345").first_of("543210"), 0u);

    EXPECT_EQ(csubstr("012345").first_of('5'), 5u);
    EXPECT_EQ(csubstr("012345").first_of("5"), 5u);
    EXPECT_EQ(csubstr("012345").first_of("45"), 4u);
    EXPECT_EQ(csubstr("012345").first_of("54"), 4u);
    EXPECT_EQ(csubstr("012345").first_of("345"), 3u);
    EXPECT_EQ(csubstr("012345").first_of("543"), 3u);
    EXPECT_EQ(csubstr("012345").first_of("2345"), 2u);
    EXPECT_EQ(csubstr("012345").first_of("5432"), 2u);
    EXPECT_EQ(csubstr("012345").first_of("12345"), 1u);
    EXPECT_EQ(csubstr("012345").first_of("54321"), 1u);
    EXPECT_EQ(csubstr("012345").first_of("012345"), 0u);
    EXPECT_EQ(csubstr("012345").first_of("543210"), 0u);
}

TEST(substr, last_of)
{
    EXPECT_EQ(csubstr("012345").last_of('a'), csubstr::npos);
    EXPECT_EQ(csubstr("012345").last_of("ab"), csubstr::npos);

    EXPECT_EQ(csubstr("012345").last_of('0'), 0u);
    EXPECT_EQ(csubstr("012345").last_of("0"), 0u);
    EXPECT_EQ(csubstr("012345").last_of("01"), 1u);
    EXPECT_EQ(csubstr("012345").last_of("10"), 1u);
    EXPECT_EQ(csubstr("012345").last_of("012"), 2u);
    EXPECT_EQ(csubstr("012345").last_of("210"), 2u);
    EXPECT_EQ(csubstr("012345").last_of("0123"), 3u);
    EXPECT_EQ(csubstr("012345").last_of("3210"), 3u);
    EXPECT_EQ(csubstr("012345").last_of("01234"), 4u);
    EXPECT_EQ(csubstr("012345").last_of("43210"), 4u);
    EXPECT_EQ(csubstr("012345").last_of("012345"), 5u);
    EXPECT_EQ(csubstr("012345").last_of("543210"), 5u);

    EXPECT_EQ(csubstr("012345").last_of('5'), 5u);
    EXPECT_EQ(csubstr("012345").last_of("5"), 5u);
    EXPECT_EQ(csubstr("012345").last_of("45"), 5u);
    EXPECT_EQ(csubstr("012345").last_of("54"), 5u);
    EXPECT_EQ(csubstr("012345").last_of("345"), 5u);
    EXPECT_EQ(csubstr("012345").last_of("543"), 5u);
    EXPECT_EQ(csubstr("012345").last_of("2345"), 5u);
    EXPECT_EQ(csubstr("012345").last_of("5432"), 5u);
    EXPECT_EQ(csubstr("012345").last_of("12345"), 5u);
    EXPECT_EQ(csubstr("012345").last_of("54321"), 5u);
    EXPECT_EQ(csubstr("012345").last_of("012345"), 5u);
    EXPECT_EQ(csubstr("012345").last_of("543210"), 5u);
}

TEST(substr, first_not_of)
{
    EXPECT_EQ(csubstr("012345").first_not_of('a'), 0u);
    EXPECT_EQ(csubstr("012345").first_not_of("ab"), 0u);

    EXPECT_EQ(csubstr("012345").first_not_of('0'), 1u);
    EXPECT_EQ(csubstr("012345").first_not_of("0"), 1u);
    EXPECT_EQ(csubstr("012345").first_not_of("01"), 2u);
    EXPECT_EQ(csubstr("012345").first_not_of("10"), 2u);
    EXPECT_EQ(csubstr("012345").first_not_of("012"), 3u);
    EXPECT_EQ(csubstr("012345").first_not_of("210"), 3u);
    EXPECT_EQ(csubstr("012345").first_not_of("0123"), 4u);
    EXPECT_EQ(csubstr("012345").first_not_of("3210"), 4u);
    EXPECT_EQ(csubstr("012345").first_not_of("01234"), 5u);
    EXPECT_EQ(csubstr("012345").first_not_of("43210"), 5u);
    EXPECT_EQ(csubstr("012345").first_not_of("012345"), csubstr::npos);
    EXPECT_EQ(csubstr("012345").first_not_of("543210"), csubstr::npos);

    EXPECT_EQ(csubstr("012345").first_not_of('5'), 0u);
    EXPECT_EQ(csubstr("012345").first_not_of("5"), 0u);
    EXPECT_EQ(csubstr("012345").first_not_of("45"), 0u);
    EXPECT_EQ(csubstr("012345").first_not_of("54"), 0u);
    EXPECT_EQ(csubstr("012345").first_not_of("345"), 0u);
    EXPECT_EQ(csubstr("012345").first_not_of("543"), 0u);
    EXPECT_EQ(csubstr("012345").first_not_of("2345"), 0u);
    EXPECT_EQ(csubstr("012345").first_not_of("5432"), 0u);
    EXPECT_EQ(csubstr("012345").first_not_of("12345"), 0u);
    EXPECT_EQ(csubstr("012345").first_not_of("54321"), 0u);
    EXPECT_EQ(csubstr("012345").first_not_of("012345"), csubstr::npos);
    EXPECT_EQ(csubstr("012345").first_not_of("543210"), csubstr::npos);
}

TEST(substr, last_not_of)
{
    EXPECT_EQ(csubstr("012345").last_not_of('a'), 5u);
    EXPECT_EQ(csubstr("012345").last_not_of("ab"), 5u);

    EXPECT_EQ(csubstr("012345").last_not_of('5'), 4u);
    EXPECT_EQ(csubstr("012345").last_not_of("5"), 4u);
    EXPECT_EQ(csubstr("012345").last_not_of("45"), 3u);
    EXPECT_EQ(csubstr("012345").last_not_of("54"), 3u);
    EXPECT_EQ(csubstr("012345").last_not_of("345"), 2u);
    EXPECT_EQ(csubstr("012345").last_not_of("543"), 2u);
    EXPECT_EQ(csubstr("012345").last_not_of("2345"), 1u);
    EXPECT_EQ(csubstr("012345").last_not_of("5432"), 1u);
    EXPECT_EQ(csubstr("012345").last_not_of("12345"), 0u);
    EXPECT_EQ(csubstr("012345").last_not_of("54321"), 0u);
    EXPECT_EQ(csubstr("012345").last_not_of("012345"), csubstr::npos);
    EXPECT_EQ(csubstr("012345").last_not_of("543210"), csubstr::npos);

    EXPECT_EQ(csubstr("012345").last_not_of('0'), 5u);
    EXPECT_EQ(csubstr("012345").last_not_of("0"), 5u);
    EXPECT_EQ(csubstr("012345").last_not_of("01"), 5u);
    EXPECT_EQ(csubstr("012345").last_not_of("10"), 5u);
    EXPECT_EQ(csubstr("012345").last_not_of("012"), 5u);
    EXPECT_EQ(csubstr("012345").last_not_of("210"), 5u);
    EXPECT_EQ(csubstr("012345").last_not_of("0123"), 5u);
    EXPECT_EQ(csubstr("012345").last_not_of("3210"), 5u);
    EXPECT_EQ(csubstr("012345").last_not_of("01234"), 5u);
    EXPECT_EQ(csubstr("012345").last_not_of("43210"), 5u);
    EXPECT_EQ(csubstr("012345").last_not_of("012345"), csubstr::npos);
    EXPECT_EQ(csubstr("012345").last_not_of("543210"), csubstr::npos);
}

TEST(substr, compare)
{
    const char s1[] = "one empty doc";
    const char s2[] = "one empty doc, explicit termination";
    csubstr c1(s1), c2(s2);
    EXPECT_NE(c1, c2);
    EXPECT_GT(c2, c1);
    EXPECT_TRUE((c1 > c2) != (c1 < c2));
}

TEST(substr, compare_vs_char)
{
    EXPECT_NE(csubstr("---"), '-');
    EXPECT_NE(csubstr("---"), "-");

    EXPECT_NE(csubstr("aaa"), 'a');
    EXPECT_NE(csubstr("aaa"), "a");

    EXPECT_NE(csubstr("aaa"), 'b');
    EXPECT_NE(csubstr("aaa"), "b");

    EXPECT_LT(csubstr("aaa"), 'b');
    EXPECT_LT(csubstr("aaa"), "b");

    EXPECT_NE(csubstr("bbb"), 'a');
    EXPECT_NE(csubstr("bbb"), "a");

    EXPECT_GT(csubstr("bbb"), 'a');
    EXPECT_GT(csubstr("bbb"), "a");
}

TEST(substr, eqne)
{
    char buf[128];
    for(size_t i = 0; i < 5; ++i) buf[i] = (char)('0' + i);
    csubstr cmp(buf, 5);

    EXPECT_EQ(csubstr("01234"), cmp);
    EXPECT_EQ(        "01234" , cmp);
    EXPECT_EQ(             cmp, "01234");
    EXPECT_NE(csubstr("0123"), cmp);
    EXPECT_NE(        "0123" , cmp);
    EXPECT_NE(            cmp, "0123");
    EXPECT_NE(csubstr("012345"), cmp);
    EXPECT_NE(        "012345" , cmp);
    EXPECT_NE(              cmp, "012345");
}

TEST(substr, substr2csubstr)
{
    char b[] = "some string";
    substr s(b);
    csubstr sc = s;
    EXPECT_EQ(sc, s);
    const substr cs(b);
    const csubstr csc(b);

}

TEST(substr, first_of_any)
{
    EXPECT_EQ(csubstr("baz{% endif %}").first_of_any("{% endif %}", "{% if "         , "{% elif bar %}" , "{% else %}" ).which, 0u);
    EXPECT_EQ(csubstr("baz{% endif %}").first_of_any("{% if "     , "{% endif %}"    , "{% elif bar %}" , "{% else %}" ).which, 1u);
    EXPECT_EQ(csubstr("baz{% endif %}").first_of_any("{% if "     , "{% elif bar %}" , "{% endif %}"    , "{% else %}" ).which, 2u);
    EXPECT_EQ(csubstr("baz{% endif %}").first_of_any("{% if "     , "{% elif bar %}" , "{% else %}"     , "{% endif %}").which, 3u);

    EXPECT_EQ(csubstr("bar{% else %}baz{% endif %}").first_of_any("{% else %}" , "{% if "         , "{% elif bar %}" , "{% endif %}").which, 0u);
    EXPECT_EQ(csubstr("bar{% else %}baz{% endif %}").first_of_any("{% if "     , "{% else %}"     , "{% elif bar %}" , "{% endif %}").which, 1u);
    EXPECT_EQ(csubstr("bar{% else %}baz{% endif %}").first_of_any("{% if "     , "{% elif bar %}" , "{% else %}"     , "{% endif %}").which, 2u);
    EXPECT_EQ(csubstr("bar{% else %}baz{% endif %}").first_of_any("{% if "     , "{% elif bar %}" , "{% endif %}"    , "{% else %}" ).which, 3u);

    EXPECT_EQ(csubstr("foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% elif bar %}" , "{% if "         , "{% else %}"     , "{% endif %}"   ).which, 0u);
    EXPECT_EQ(csubstr("foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% if "         , "{% elif bar %}" , "{% else %}"     , "{% endif %}"   ).which, 1u);
    EXPECT_EQ(csubstr("foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% if "         , "{% else %}"     , "{% elif bar %}" , "{% endif %}"   ).which, 2u);
    EXPECT_EQ(csubstr("foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% if "         , "{% else %}"     , "{% endif %}"    , "{% elif bar %}").which, 3u);

    EXPECT_EQ(csubstr("{% if foo %}foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% if "         , "{% elif bar %}" , "{% else %}" , "{% endif %}" ).which, 0u);
    EXPECT_EQ(csubstr("{% if foo %}foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% elif bar %}" , "{% if "         , "{% else %}" , "{% endif %}" ).which, 1u);
    EXPECT_EQ(csubstr("{% if foo %}foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% elif bar %}" , "{% else %}"     , "{% if "     , "{% endif %}" ).which, 2u);
    EXPECT_EQ(csubstr("{% if foo %}foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% elif bar %}" , "{% else %}"     , "{% endif %}", "{% if "      ).which, 3u);
}

TEST(substr, first_non_empty_span)
{
    EXPECT_EQ(csubstr("foo bar").first_non_empty_span(), "foo");
    EXPECT_EQ(csubstr("       foo bar").first_non_empty_span(), "foo");
    EXPECT_EQ(csubstr("\n   \r  \t  foo bar").first_non_empty_span(), "foo");
    EXPECT_EQ(csubstr("\n   \r  \t  foo\n\r\t bar").first_non_empty_span(), "foo");
    EXPECT_EQ(csubstr("\n   \r  \t  foo\n\r\t bar").first_non_empty_span(), "foo");
    EXPECT_EQ(csubstr(",\n   \r  \t  foo\n\r\t bar").first_non_empty_span(), ",");
}

TEST(substr, first_uint_span)
{
    EXPECT_EQ(csubstr("1234").first_uint_span(), "1234");
    EXPECT_EQ(csubstr("1234 abc").first_uint_span(), "1234");
    EXPECT_EQ(csubstr("0x1234 abc").first_uint_span(), "0x1234");
}

TEST(substr, first_int_span)
{
    EXPECT_EQ(csubstr("1234").first_int_span(), "1234");
    EXPECT_EQ(csubstr("-1234").first_int_span(), "-1234");
    EXPECT_EQ(csubstr("1234 abc").first_int_span(), "1234");
    EXPECT_EQ(csubstr("-1234 abc").first_int_span(), "-1234");
    EXPECT_EQ(csubstr("0x1234 abc").first_int_span(), "0x1234");
    EXPECT_EQ(csubstr("-0x1234 abc").first_int_span(), "-0x1234");
}

} // namespace c4
