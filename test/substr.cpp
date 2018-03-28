#include "c4/test.hpp"
#include "c4/substr.hpp"

namespace c4 {

TEST(subs, ctor_from_char)
{
    char buf1[] = "{foo: 1}";
    char buf2[] = "{foo: 2}";
    subs s(buf1);
    EXPECT_EQ(s, "{foo: 1}");
    s = buf2;
    EXPECT_EQ(s, "{foo: 2}");
}

TEST(csubs, ctor_from_char)
{
    char buf1[] = "{foo: 1}";
    char buf2[] = "{foo: 2}";
    subs s(buf1);
    EXPECT_EQ(s, "{foo: 1}");
    s = buf2;
    EXPECT_EQ(s, "{foo: 2}");
}

TEST(subs, begins_with)
{
    EXPECT_TRUE (csubs(": ").begins_with(":" ));
    EXPECT_TRUE (csubs(": ").begins_with(':' ));
    EXPECT_FALSE(csubs(":") .begins_with(": "));

    EXPECT_TRUE (csubs(    "1234").begins_with('0', 0));
    EXPECT_TRUE (csubs(   "01234").begins_with('0', 1));
    EXPECT_FALSE(csubs(   "01234").begins_with('0', 2));
    EXPECT_TRUE (csubs(  "001234").begins_with('0', 1));
    EXPECT_TRUE (csubs(  "001234").begins_with('0', 2));
    EXPECT_FALSE(csubs(  "001234").begins_with('0', 3));
    EXPECT_TRUE (csubs( "0001234").begins_with('0', 1));
    EXPECT_TRUE (csubs( "0001234").begins_with('0', 2));
    EXPECT_TRUE (csubs( "0001234").begins_with('0', 3));
    EXPECT_FALSE(csubs( "0001234").begins_with('0', 4));
    EXPECT_TRUE (csubs("00001234").begins_with('0', 1));
    EXPECT_TRUE (csubs("00001234").begins_with('0', 2));
    EXPECT_TRUE (csubs("00001234").begins_with('0', 3));
    EXPECT_TRUE (csubs("00001234").begins_with('0', 4));
    EXPECT_FALSE(csubs("00001234").begins_with('0', 5));
}

TEST(subs, ends_with)
{
    EXPECT_TRUE(csubs("{% if foo %}bar{% endif %}").ends_with("{% endif %}"));

    EXPECT_TRUE (csubs("1234"    ).ends_with('0', 0));
    EXPECT_TRUE (csubs("12340"   ).ends_with('0', 1));
    EXPECT_FALSE(csubs("12340"   ).ends_with('0', 2));
    EXPECT_TRUE (csubs("123400"  ).ends_with('0', 1));
    EXPECT_TRUE (csubs("123400"  ).ends_with('0', 2));
    EXPECT_FALSE(csubs("123400"  ).ends_with('0', 3));
    EXPECT_TRUE (csubs("1234000" ).ends_with('0', 1));
    EXPECT_TRUE (csubs("1234000" ).ends_with('0', 2));
    EXPECT_TRUE (csubs("1234000" ).ends_with('0', 3));
    EXPECT_FALSE(csubs("1234000" ).ends_with('0', 4));
    EXPECT_TRUE (csubs("12340000").ends_with('0', 1));
    EXPECT_TRUE (csubs("12340000").ends_with('0', 2));
    EXPECT_TRUE (csubs("12340000").ends_with('0', 3));
    EXPECT_TRUE (csubs("12340000").ends_with('0', 4));
    EXPECT_FALSE(csubs("12340000").ends_with('0', 5));
}

TEST(subs, first_of)
{
    EXPECT_EQ(csubs("012345").first_of('a'), csubs::npos);
    EXPECT_EQ(csubs("012345").first_of("ab"), csubs::npos);

    EXPECT_EQ(csubs("012345").first_of('0'), 0u);
    EXPECT_EQ(csubs("012345").first_of("0"), 0u);
    EXPECT_EQ(csubs("012345").first_of("01"), 0u);
    EXPECT_EQ(csubs("012345").first_of("10"), 0u);
    EXPECT_EQ(csubs("012345").first_of("012"), 0u);
    EXPECT_EQ(csubs("012345").first_of("210"), 0u);
    EXPECT_EQ(csubs("012345").first_of("0123"), 0u);
    EXPECT_EQ(csubs("012345").first_of("3210"), 0u);
    EXPECT_EQ(csubs("012345").first_of("01234"), 0u);
    EXPECT_EQ(csubs("012345").first_of("43210"), 0u);
    EXPECT_EQ(csubs("012345").first_of("012345"), 0u);
    EXPECT_EQ(csubs("012345").first_of("543210"), 0u);

    EXPECT_EQ(csubs("012345").first_of('5'), 5u);
    EXPECT_EQ(csubs("012345").first_of("5"), 5u);
    EXPECT_EQ(csubs("012345").first_of("45"), 4u);
    EXPECT_EQ(csubs("012345").first_of("54"), 4u);
    EXPECT_EQ(csubs("012345").first_of("345"), 3u);
    EXPECT_EQ(csubs("012345").first_of("543"), 3u);
    EXPECT_EQ(csubs("012345").first_of("2345"), 2u);
    EXPECT_EQ(csubs("012345").first_of("5432"), 2u);
    EXPECT_EQ(csubs("012345").first_of("12345"), 1u);
    EXPECT_EQ(csubs("012345").first_of("54321"), 1u);
    EXPECT_EQ(csubs("012345").first_of("012345"), 0u);
    EXPECT_EQ(csubs("012345").first_of("543210"), 0u);
}

TEST(subs, last_of)
{
    EXPECT_EQ(csubs("012345").last_of('a'), csubs::npos);
    EXPECT_EQ(csubs("012345").last_of("ab"), csubs::npos);

    EXPECT_EQ(csubs("012345").last_of('0'), 0u);
    EXPECT_EQ(csubs("012345").last_of("0"), 0u);
    EXPECT_EQ(csubs("012345").last_of("01"), 1u);
    EXPECT_EQ(csubs("012345").last_of("10"), 1u);
    EXPECT_EQ(csubs("012345").last_of("012"), 2u);
    EXPECT_EQ(csubs("012345").last_of("210"), 2u);
    EXPECT_EQ(csubs("012345").last_of("0123"), 3u);
    EXPECT_EQ(csubs("012345").last_of("3210"), 3u);
    EXPECT_EQ(csubs("012345").last_of("01234"), 4u);
    EXPECT_EQ(csubs("012345").last_of("43210"), 4u);
    EXPECT_EQ(csubs("012345").last_of("012345"), 5u);
    EXPECT_EQ(csubs("012345").last_of("543210"), 5u);

    EXPECT_EQ(csubs("012345").last_of('5'), 5u);
    EXPECT_EQ(csubs("012345").last_of("5"), 5u);
    EXPECT_EQ(csubs("012345").last_of("45"), 5u);
    EXPECT_EQ(csubs("012345").last_of("54"), 5u);
    EXPECT_EQ(csubs("012345").last_of("345"), 5u);
    EXPECT_EQ(csubs("012345").last_of("543"), 5u);
    EXPECT_EQ(csubs("012345").last_of("2345"), 5u);
    EXPECT_EQ(csubs("012345").last_of("5432"), 5u);
    EXPECT_EQ(csubs("012345").last_of("12345"), 5u);
    EXPECT_EQ(csubs("012345").last_of("54321"), 5u);
    EXPECT_EQ(csubs("012345").last_of("012345"), 5u);
    EXPECT_EQ(csubs("012345").last_of("543210"), 5u);
}

TEST(subs, first_not_of)
{
    EXPECT_EQ(csubs("012345").first_not_of('a'), 0u);
    EXPECT_EQ(csubs("012345").first_not_of("ab"), 0u);

    EXPECT_EQ(csubs("012345").first_not_of('0'), 1u);
    EXPECT_EQ(csubs("012345").first_not_of("0"), 1u);
    EXPECT_EQ(csubs("012345").first_not_of("01"), 2u);
    EXPECT_EQ(csubs("012345").first_not_of("10"), 2u);
    EXPECT_EQ(csubs("012345").first_not_of("012"), 3u);
    EXPECT_EQ(csubs("012345").first_not_of("210"), 3u);
    EXPECT_EQ(csubs("012345").first_not_of("0123"), 4u);
    EXPECT_EQ(csubs("012345").first_not_of("3210"), 4u);
    EXPECT_EQ(csubs("012345").first_not_of("01234"), 5u);
    EXPECT_EQ(csubs("012345").first_not_of("43210"), 5u);
    EXPECT_EQ(csubs("012345").first_not_of("012345"), csubs::npos);
    EXPECT_EQ(csubs("012345").first_not_of("543210"), csubs::npos);

    EXPECT_EQ(csubs("012345").first_not_of('5'), 0u);
    EXPECT_EQ(csubs("012345").first_not_of("5"), 0u);
    EXPECT_EQ(csubs("012345").first_not_of("45"), 0u);
    EXPECT_EQ(csubs("012345").first_not_of("54"), 0u);
    EXPECT_EQ(csubs("012345").first_not_of("345"), 0u);
    EXPECT_EQ(csubs("012345").first_not_of("543"), 0u);
    EXPECT_EQ(csubs("012345").first_not_of("2345"), 0u);
    EXPECT_EQ(csubs("012345").first_not_of("5432"), 0u);
    EXPECT_EQ(csubs("012345").first_not_of("12345"), 0u);
    EXPECT_EQ(csubs("012345").first_not_of("54321"), 0u);
    EXPECT_EQ(csubs("012345").first_not_of("012345"), csubs::npos);
    EXPECT_EQ(csubs("012345").first_not_of("543210"), csubs::npos);
}

TEST(subs, last_not_of)
{
    EXPECT_EQ(csubs("012345").last_not_of('a'), 5u);
    EXPECT_EQ(csubs("012345").last_not_of("ab"), 5u);

    EXPECT_EQ(csubs("012345").last_not_of('5'), 4u);
    EXPECT_EQ(csubs("012345").last_not_of("5"), 4u);
    EXPECT_EQ(csubs("012345").last_not_of("45"), 3u);
    EXPECT_EQ(csubs("012345").last_not_of("54"), 3u);
    EXPECT_EQ(csubs("012345").last_not_of("345"), 2u);
    EXPECT_EQ(csubs("012345").last_not_of("543"), 2u);
    EXPECT_EQ(csubs("012345").last_not_of("2345"), 1u);
    EXPECT_EQ(csubs("012345").last_not_of("5432"), 1u);
    EXPECT_EQ(csubs("012345").last_not_of("12345"), 0u);
    EXPECT_EQ(csubs("012345").last_not_of("54321"), 0u);
    EXPECT_EQ(csubs("012345").last_not_of("012345"), csubs::npos);
    EXPECT_EQ(csubs("012345").last_not_of("543210"), csubs::npos);

    EXPECT_EQ(csubs("012345").last_not_of('0'), 5u);
    EXPECT_EQ(csubs("012345").last_not_of("0"), 5u);
    EXPECT_EQ(csubs("012345").last_not_of("01"), 5u);
    EXPECT_EQ(csubs("012345").last_not_of("10"), 5u);
    EXPECT_EQ(csubs("012345").last_not_of("012"), 5u);
    EXPECT_EQ(csubs("012345").last_not_of("210"), 5u);
    EXPECT_EQ(csubs("012345").last_not_of("0123"), 5u);
    EXPECT_EQ(csubs("012345").last_not_of("3210"), 5u);
    EXPECT_EQ(csubs("012345").last_not_of("01234"), 5u);
    EXPECT_EQ(csubs("012345").last_not_of("43210"), 5u);
    EXPECT_EQ(csubs("012345").last_not_of("012345"), csubs::npos);
    EXPECT_EQ(csubs("012345").last_not_of("543210"), csubs::npos);
}

TEST(subs, compare)
{
    const char s1[] = "one empty doc";
    const char s2[] = "one empty doc, explicit termination";
    csubs c1(s1), c2(s2);
    EXPECT_NE(c1, c2);
    EXPECT_GT(c2, c1);
    EXPECT_TRUE((c1 > c2) != (c1 < c2));
}

TEST(subs, subs2csubs)
{
    char b[] = "some string";
    subs s(b);
    csubs sc = s;
    EXPECT_EQ(sc, s);
    const subs cs(b);
    const csubs csc(b);

}

TEST(subs, first_of_any)
{
    EXPECT_EQ(csubs("baz{% endif %}").first_of_any("{% endif %}", "{% if "         , "{% elif bar %}" , "{% else %}" ).which, 0u);
    EXPECT_EQ(csubs("baz{% endif %}").first_of_any("{% if "     , "{% endif %}"    , "{% elif bar %}" , "{% else %}" ).which, 1u);
    EXPECT_EQ(csubs("baz{% endif %}").first_of_any("{% if "     , "{% elif bar %}" , "{% endif %}"    , "{% else %}" ).which, 2u);
    EXPECT_EQ(csubs("baz{% endif %}").first_of_any("{% if "     , "{% elif bar %}" , "{% else %}"     , "{% endif %}").which, 3u);

    EXPECT_EQ(csubs("bar{% else %}baz{% endif %}").first_of_any("{% else %}" , "{% if "         , "{% elif bar %}" , "{% endif %}").which, 0u);
    EXPECT_EQ(csubs("bar{% else %}baz{% endif %}").first_of_any("{% if "     , "{% else %}"     , "{% elif bar %}" , "{% endif %}").which, 1u);
    EXPECT_EQ(csubs("bar{% else %}baz{% endif %}").first_of_any("{% if "     , "{% elif bar %}" , "{% else %}"     , "{% endif %}").which, 2u);
    EXPECT_EQ(csubs("bar{% else %}baz{% endif %}").first_of_any("{% if "     , "{% elif bar %}" , "{% endif %}"    , "{% else %}" ).which, 3u);

    EXPECT_EQ(csubs("foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% elif bar %}" , "{% if "         , "{% else %}"     , "{% endif %}"   ).which, 0u);
    EXPECT_EQ(csubs("foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% if "         , "{% elif bar %}" , "{% else %}"     , "{% endif %}"   ).which, 1u);
    EXPECT_EQ(csubs("foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% if "         , "{% else %}"     , "{% elif bar %}" , "{% endif %}"   ).which, 2u);
    EXPECT_EQ(csubs("foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% if "         , "{% else %}"     , "{% endif %}"    , "{% elif bar %}").which, 3u);

    EXPECT_EQ(csubs("{% if foo %}foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% if "         , "{% elif bar %}" , "{% else %}" , "{% endif %}" ).which, 0u);
    EXPECT_EQ(csubs("{% if foo %}foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% elif bar %}" , "{% if "         , "{% else %}" , "{% endif %}" ).which, 1u);
    EXPECT_EQ(csubs("{% if foo %}foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% elif bar %}" , "{% else %}"     , "{% if "     , "{% endif %}" ).which, 2u);
    EXPECT_EQ(csubs("{% if foo %}foo{% elif bar %}bar{% else %}baz{% endif %}").first_of_any("{% elif bar %}" , "{% else %}"     , "{% endif %}", "{% if "      ).which, 3u);
}

TEST(subs, first_non_empty_span)
{
    EXPECT_EQ(csubs("foo bar").first_non_empty_span(), "foo");
    EXPECT_EQ(csubs("       foo bar").first_non_empty_span(), "foo");
    EXPECT_EQ(csubs("\n   \r  \t  foo bar").first_non_empty_span(), "foo");
    EXPECT_EQ(csubs("\n   \r  \t  foo\n\r\t bar").first_non_empty_span(), "foo");
    EXPECT_EQ(csubs("\n   \r  \t  foo\n\r\t bar").first_non_empty_span(), "foo");
    EXPECT_EQ(csubs(",\n   \r  \t  foo\n\r\t bar").first_non_empty_span(), ",");
}

} // namespace c4
