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
    csubstr s(buf1);
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


template <class ...Args>
void test_first_of_any(csubstr input, bool true_or_false, size_t which, size_t pos, Args... args)
{
    csubstr::first_of_any_result r = input.first_of_any(to_csubstr(args)...);
    //std::cout << input << ": " << (bool(r) ? "true" : "false") << "/which:" << r.which << "/pos:" << r.pos << "\n";
    EXPECT_EQ(r, true_or_false);
    EXPECT_EQ(r.which, which);
    EXPECT_EQ(r.pos, pos);
}

TEST(substr, first_of_any)
{
    size_t NONE = csubstr::NONE;
    size_t npos = csubstr::npos;

    test_first_of_any("foobar"               , true , 0u  ,   3u, "bar", "barbell", "bark", "barff");
    test_first_of_any("foobar"               , false, NONE, npos,        "barbell", "bark", "barff");
    test_first_of_any("foobart"              , false, NONE, npos,        "barbell", "bark", "barff");

    test_first_of_any("10"                   , false, NONE, npos, "0x", "0X", "-0x", "-0X");
    test_first_of_any("10]"                  , false, NONE, npos, "0x", "0X", "-0x", "-0X");
    test_first_of_any(csubstr("10]").first(2), false, NONE, npos, "0x", "0X", "-0x", "-0X");


    test_first_of_any("baz{% endif %}", true, 0u, 3u, "{% endif %}", "{% if "         , "{% elif bar %}" , "{% else %}" );
    test_first_of_any("baz{% endif %}", true, 1u, 3u, "{% if "     , "{% endif %}"    , "{% elif bar %}" , "{% else %}" );
    test_first_of_any("baz{% endif %}", true, 2u, 3u, "{% if "     , "{% elif bar %}" , "{% endif %}"    , "{% else %}" );
    test_first_of_any("baz{% endif %}", true, 3u, 3u, "{% if "     , "{% elif bar %}" , "{% else %}"     , "{% endif %}");

    test_first_of_any("baz{% e..if %}", false, NONE, npos, "{% endif %}", "{% if "         , "{% elif bar %}" , "{% else %}" );
    test_first_of_any("baz{% e..if %}", false, NONE, npos, "{% if "     , "{% endif %}"    , "{% elif bar %}" , "{% else %}" );
    test_first_of_any("baz{% e..if %}", false, NONE, npos, "{% if "     , "{% elif bar %}" , "{% endif %}"    , "{% else %}" );
    test_first_of_any("baz{% e..if %}", false, NONE, npos, "{% if "     , "{% elif bar %}" , "{% else %}"     , "{% endif %}");


    test_first_of_any("bar{% else %}baz{% endif %}", true, 0u, 3u, "{% else %}" , "{% if "         , "{% elif bar %}" , "{% endif %}");
    test_first_of_any("bar{% else %}baz{% endif %}", true, 1u, 3u, "{% if "     , "{% else %}"     , "{% elif bar %}" , "{% endif %}");
    test_first_of_any("bar{% else %}baz{% endif %}", true, 2u, 3u, "{% if "     , "{% elif bar %}" , "{% else %}"     , "{% endif %}");
    test_first_of_any("bar{% else %}baz{% endif %}", true, 3u, 3u, "{% if "     , "{% elif bar %}" , "{% endif %}"    , "{% else %}" );

    test_first_of_any("bar{% e..e %}baz{% e..if %}", false, NONE, npos, "{% else %}" , "{% if "         , "{% elif bar %}" , "{% endif %}");
    test_first_of_any("bar{% e..e %}baz{% e..if %}", false, NONE, npos, "{% if "     , "{% else %}"     , "{% elif bar %}" , "{% endif %}");
    test_first_of_any("bar{% e..e %}baz{% e..if %}", false, NONE, npos, "{% if "     , "{% elif bar %}" , "{% else %}"     , "{% endif %}");
    test_first_of_any("bar{% e..e %}baz{% e..if %}", false, NONE, npos, "{% if "     , "{% elif bar %}" , "{% endif %}"    , "{% else %}" );


    test_first_of_any("foo{% elif bar %}bar{% else %}baz{% endif %}", true, 0u, 3u, "{% elif bar %}" , "{% if "         , "{% else %}"     , "{% endif %}"   );
    test_first_of_any("foo{% elif bar %}bar{% else %}baz{% endif %}", true, 1u, 3u, "{% if "         , "{% elif bar %}" , "{% else %}"     , "{% endif %}"   );
    test_first_of_any("foo{% elif bar %}bar{% else %}baz{% endif %}", true, 2u, 3u, "{% if "         , "{% else %}"     , "{% elif bar %}" , "{% endif %}"   );
    test_first_of_any("foo{% elif bar %}bar{% else %}baz{% endif %}", true, 3u, 3u, "{% if "         , "{% else %}"     , "{% endif %}"    , "{% elif bar %}");

    test_first_of_any("foo{% e..f bar %}bar{% e..e %}baz{% e..if %}", false, NONE, npos, "{% elif bar %}" , "{% if "         , "{% else %}"     , "{% endif %}"   );
    test_first_of_any("foo{% e..f bar %}bar{% e..e %}baz{% e..if %}", false, NONE, npos, "{% if "         , "{% elif bar %}" , "{% else %}"     , "{% endif %}"   );
    test_first_of_any("foo{% e..f bar %}bar{% e..e %}baz{% e..if %}", false, NONE, npos, "{% if "         , "{% else %}"     , "{% elif bar %}" , "{% endif %}"   );
    test_first_of_any("foo{% e..f bar %}bar{% e..e %}baz{% e..if %}", false, NONE, npos, "{% if "         , "{% else %}"     , "{% endif %}"    , "{% elif bar %}");


    test_first_of_any("{% if foo %}foo{% elif bar %}bar{% else %}baz{% endif %}", true, 0u, 0u, "{% if "         , "{% elif bar %}" , "{% else %}" , "{% endif %}" );
    test_first_of_any("{% if foo %}foo{% elif bar %}bar{% else %}baz{% endif %}", true, 1u, 0u, "{% elif bar %}" , "{% if "         , "{% else %}" , "{% endif %}" );
    test_first_of_any("{% if foo %}foo{% elif bar %}bar{% else %}baz{% endif %}", true, 2u, 0u, "{% elif bar %}" , "{% else %}"     , "{% if "     , "{% endif %}" );
    test_first_of_any("{% if foo %}foo{% elif bar %}bar{% else %}baz{% endif %}", true, 3u, 0u, "{% elif bar %}" , "{% else %}"     , "{% endif %}", "{% if "      );

    test_first_of_any("{% .. foo %}foo{% e..f bar %}bar{% e..e %}baz{% e..if %}", false, NONE, npos, "{% if "         , "{% elif bar %}" , "{% else %}" , "{% endif %}" );
    test_first_of_any("{% .. foo %}foo{% e..f bar %}bar{% e..e %}baz{% e..if %}", false, NONE, npos, "{% elif bar %}" , "{% if "         , "{% else %}" , "{% endif %}" );
    test_first_of_any("{% .. foo %}foo{% e..f bar %}bar{% e..e %}baz{% e..if %}", false, NONE, npos, "{% elif bar %}" , "{% else %}"     , "{% if "     , "{% endif %}" );
    test_first_of_any("{% .. foo %}foo{% e..f bar %}bar{% e..e %}baz{% e..if %}", false, NONE, npos, "{% elif bar %}" , "{% else %}"     , "{% endif %}", "{% if "      );
}


TEST(substr, pair_range_esc)
{
    const char q = '\'';
    EXPECT_EQ(csubstr("").pair_range_esc(q), "");
    EXPECT_EQ(csubstr("'").pair_range_esc(q), "");
    EXPECT_EQ(csubstr("''").pair_range_esc(q), "''");
    EXPECT_EQ(csubstr("'\\'\\''").pair_range_esc(q), "'\\'\\''");
    EXPECT_EQ(csubstr("asdasdasd''asdasd").pair_range_esc(q), "''");
    EXPECT_EQ(csubstr("asdasdasd'abc'asdasda").pair_range_esc(q), "'abc'");
}

TEST(substr, pair_range)
{
    EXPECT_EQ(csubstr("").pair_range('{', '}'), "");
    EXPECT_EQ(csubstr("{").pair_range('{', '}'), "");
    EXPECT_EQ(csubstr("}").pair_range('{', '}'), "");
    EXPECT_EQ(csubstr("{}").pair_range('{', '}'), "{}");
    EXPECT_EQ(csubstr("{abc}").pair_range('{', '}'), "{abc}");
    EXPECT_EQ(csubstr("123{abc}456").pair_range('{', '}'), "{abc}");
}

TEST(substr, pair_range_nested)
{
    EXPECT_EQ(csubstr("").pair_range_nested('{', '}'), "");
    EXPECT_EQ(csubstr("{").pair_range_nested('{', '}'), "");
    EXPECT_EQ(csubstr("}").pair_range_nested('{', '}'), "");
    EXPECT_EQ(csubstr("{}").pair_range_nested('{', '}'), "{}");
    EXPECT_EQ(csubstr("{abc}").pair_range_nested('{', '}'), "{abc}");
    EXPECT_EQ(csubstr("123{abc}456").pair_range_nested('{', '}'), "{abc}");
    EXPECT_EQ(csubstr("123{abc}456{def}").pair_range_nested('{', '}'), "{abc}");
    EXPECT_EQ(csubstr(   "{{}}").pair_range_nested('{', '}'), "{{}}");
    EXPECT_EQ(csubstr("123{{}}456").pair_range_nested('{', '}'), "{{}}");
    EXPECT_EQ(csubstr(   "{a{}b{}c}").pair_range_nested('{', '}'), "{a{}b{}c}");
    EXPECT_EQ(csubstr("123{a{}b{}c}456").pair_range_nested('{', '}'), "{a{}b{}c}");
    EXPECT_EQ(csubstr(    "{a{{}}b{{}}c}").pair_range_nested('{', '}'), "{a{{}}b{{}}c}");
    EXPECT_EQ(csubstr("123{a{{}}b{{}}c}456").pair_range_nested('{', '}'), "{a{{}}b{{}}c}");
    EXPECT_EQ(csubstr(   "{{{}}a{{}}b{{}}c{{}}}").pair_range_nested('{', '}'), "{{{}}a{{}}b{{}}c{{}}}");
    EXPECT_EQ(csubstr("123{{{}}a{{}}b{{}}c{{}}}456").pair_range_nested('{', '}'), "{{{}}a{{}}b{{}}c{{}}}");
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


TEST(substr, triml)
{
    using S = csubstr;

    EXPECT_EQ(S("aaabbb"   ).triml('a' ), "bbb");
    EXPECT_EQ(S("aaabbb"   ).triml('b' ), "aaabbb");
    EXPECT_EQ(S("aaabbb"   ).triml('c' ), "aaabbb");
    EXPECT_EQ(S("aaabbb"   ).triml("ab"), "");
    EXPECT_EQ(S("aaabbb"   ).triml("ba"), "");
    EXPECT_EQ(S("aaabbb"   ).triml("cd"), "aaabbb");
    EXPECT_EQ(S("aaa...bbb").triml('a' ), "...bbb");
    EXPECT_EQ(S("aaa...bbb").triml('b' ), "aaa...bbb");
    EXPECT_EQ(S("aaa...bbb").triml('c' ), "aaa...bbb");
    EXPECT_EQ(S("aaa...bbb").triml("ab"), "...bbb");
    EXPECT_EQ(S("aaa...bbb").triml("ba"), "...bbb");
    EXPECT_EQ(S("aaa...bbb").triml("ab."), "");
    EXPECT_EQ(S("aaa...bbb").triml("a."), "bbb");
    EXPECT_EQ(S("aaa...bbb").triml(".a"), "bbb");
    EXPECT_EQ(S("aaa...bbb").triml("b."), "aaa...bbb");
    EXPECT_EQ(S("aaa...bbb").triml(".b"), "aaa...bbb");
    EXPECT_EQ(S("aaa...bbb").triml("cd"), "aaa...bbb");

    EXPECT_EQ(S("ab"   ).triml('a' ), "b");
    EXPECT_EQ(S("ab"   ).triml('b' ), "ab");
    EXPECT_EQ(S("ab"   ).triml('c' ), "ab");
    EXPECT_EQ(S("ab"   ).triml("ab"), "");
    EXPECT_EQ(S("ab"   ).triml("ba"), "");
    EXPECT_EQ(S("ab"   ).triml("cd"), "ab");
    EXPECT_EQ(S("a...b").triml('a' ), "...b");
    EXPECT_EQ(S("a...b").triml('b' ), "a...b");
    EXPECT_EQ(S("a...b").triml('c' ), "a...b");
    EXPECT_EQ(S("a...b").triml("ab"), "...b");
    EXPECT_EQ(S("a...b").triml("ba"), "...b");
    EXPECT_EQ(S("a...b").triml("ab."), "");
    EXPECT_EQ(S("a...b").triml("a."), "b");
    EXPECT_EQ(S("a...b").triml(".a"), "b");
    EXPECT_EQ(S("a...b").triml("b."), "a...b");
    EXPECT_EQ(S("a...b").triml(".b"), "a...b");
    EXPECT_EQ(S("a...b").triml("cd"), "a...b");
}

TEST(substr, trimr)
{
    using S = csubstr;

    EXPECT_EQ(S("aaabbb"   ).trimr('a' ), "aaabbb");
    EXPECT_EQ(S("aaabbb"   ).trimr('b' ), "aaa");
    EXPECT_EQ(S("aaabbb"   ).trimr('c' ), "aaabbb");
    EXPECT_EQ(S("aaabbb"   ).trimr("ab"), "");
    EXPECT_EQ(S("aaabbb"   ).trimr("ba"), "");
    EXPECT_EQ(S("aaabbb"   ).trimr("cd"), "aaabbb");
    EXPECT_EQ(S("aaa...bbb").trimr('a' ), "aaa...bbb");
    EXPECT_EQ(S("aaa...bbb").trimr('b' ), "aaa...");
    EXPECT_EQ(S("aaa...bbb").trimr('c' ), "aaa...bbb");
    EXPECT_EQ(S("aaa...bbb").trimr("ab"), "aaa...");
    EXPECT_EQ(S("aaa...bbb").trimr("ba"), "aaa...");
    EXPECT_EQ(S("aaa...bbb").trimr("ab."), "");
    EXPECT_EQ(S("aaa...bbb").trimr("a."), "aaa...bbb");
    EXPECT_EQ(S("aaa...bbb").trimr(".a"), "aaa...bbb");
    EXPECT_EQ(S("aaa...bbb").trimr("b."), "aaa");
    EXPECT_EQ(S("aaa...bbb").trimr(".b"), "aaa");
    EXPECT_EQ(S("aaa...bbb").trimr("cd"), "aaa...bbb");

    EXPECT_EQ(S("ab"   ).trimr('a' ), "ab");
    EXPECT_EQ(S("ab"   ).trimr('b' ), "a");
    EXPECT_EQ(S("ab"   ).trimr('c' ), "ab");
    EXPECT_EQ(S("ab"   ).trimr("ab"), "");
    EXPECT_EQ(S("ab"   ).trimr("ba"), "");
    EXPECT_EQ(S("ab"   ).trimr("cd"), "ab");
    EXPECT_EQ(S("a...b").trimr('a' ), "a...b");
    EXPECT_EQ(S("a...b").trimr('b' ), "a...");
    EXPECT_EQ(S("a...b").trimr('c' ), "a...b");
    EXPECT_EQ(S("a...b").trimr("ab"), "a...");
    EXPECT_EQ(S("a...b").trimr("ba"), "a...");
    EXPECT_EQ(S("a...b").trimr("ab."), "");
    EXPECT_EQ(S("a...b").trimr("a."), "a...b");
    EXPECT_EQ(S("a...b").trimr(".a"), "a...b");
    EXPECT_EQ(S("a...b").trimr("b."), "a");
    EXPECT_EQ(S("a...b").trimr(".b"), "a");
    EXPECT_EQ(S("a...b").trimr("cd"), "a...b");
}

TEST(substr, trim)
{
    using S = csubstr;

    EXPECT_EQ(S("aaabbb"   ).trim('a' ), "bbb");
    EXPECT_EQ(S("aaabbb"   ).trim('b' ), "aaa");
    EXPECT_EQ(S("aaabbb"   ).trim('c' ), "aaabbb");
    EXPECT_EQ(S("aaabbb"   ).trim("ab"), "");
    EXPECT_EQ(S("aaabbb"   ).trim("ba"), "");
    EXPECT_EQ(S("aaabbb"   ).trim("cd"), "aaabbb");
    EXPECT_EQ(S("aaa...bbb").trim('a' ), "...bbb");
    EXPECT_EQ(S("aaa...bbb").trim('b' ), "aaa...");
    EXPECT_EQ(S("aaa...bbb").trim('c' ), "aaa...bbb");
    EXPECT_EQ(S("aaa...bbb").trim("ab"), "...");
    EXPECT_EQ(S("aaa...bbb").trim("ba"), "...");
    EXPECT_EQ(S("aaa...bbb").trim('c' ), "aaa...bbb");
    EXPECT_EQ(S("aaa...bbb").trim("ab."), "");
    EXPECT_EQ(S("aaa...bbb").trim("." ), "aaa...bbb");
    EXPECT_EQ(S("aaa...bbb").trim("a."), "bbb");
    EXPECT_EQ(S("aaa...bbb").trim(".a"), "bbb");
    EXPECT_EQ(S("aaa...bbb").trim("b."), "aaa");
    EXPECT_EQ(S("aaa...bbb").trim(".b"), "aaa");
    EXPECT_EQ(S("aaa...bbb").trim("cd"), "aaa...bbb");

    EXPECT_EQ(S("ab"   ).trim('a' ), "b");
    EXPECT_EQ(S("ab"   ).trim('b' ), "a");
    EXPECT_EQ(S("ab"   ).trim('c' ), "ab");
    EXPECT_EQ(S("ab"   ).trim("ab"), "");
    EXPECT_EQ(S("ab"   ).trim("ba"), "");
    EXPECT_EQ(S("ab"   ).trim("cd"), "ab");
    EXPECT_EQ(S("a...b").trim('a' ), "...b");
    EXPECT_EQ(S("a...b").trim('b' ), "a...");
    EXPECT_EQ(S("a...b").trim('c' ), "a...b");
    EXPECT_EQ(S("a...b").trim("ab"), "...");
    EXPECT_EQ(S("a...b").trim("ba"), "...");
    EXPECT_EQ(S("a...b").trim('c' ), "a...b");
    EXPECT_EQ(S("a...b").trim("ab."), "");
    EXPECT_EQ(S("a...b").trim("." ), "a...b");
    EXPECT_EQ(S("a...b").trim("a."), "b");
    EXPECT_EQ(S("a...b").trim(".a"), "b");
    EXPECT_EQ(S("a...b").trim("b."), "a");
    EXPECT_EQ(S("a...b").trim(".b"), "a");
    EXPECT_EQ(S("a...b").trim("cd"), "a...b");
}

TEST(substr, pop_right)
{
    using S = csubstr;

    EXPECT_EQ(S("0/1/2"    ).pop_right('/'      ), "2");
    EXPECT_EQ(S("0/1/2"    ).pop_right('/', true), "2");
    EXPECT_EQ(S("0/1/2/"   ).pop_right('/'      ), "");
    EXPECT_EQ(S("0/1/2/"   ).pop_right('/', true), "2/");
    EXPECT_EQ(S("0/1/2///" ).pop_right('/'      ), "");
    EXPECT_EQ(S("0/1/2///" ).pop_right('/', true), "2///");

    EXPECT_EQ(S("0/1//2"    ).pop_right('/'      ), "2");
    EXPECT_EQ(S("0/1//2"    ).pop_right('/', true), "2");
    EXPECT_EQ(S("0/1//2/"   ).pop_right('/'      ), "");
    EXPECT_EQ(S("0/1//2/"   ).pop_right('/', true), "2/");
    EXPECT_EQ(S("0/1//2///" ).pop_right('/'      ), "");
    EXPECT_EQ(S("0/1//2///" ).pop_right('/', true), "2///");

    EXPECT_EQ(S("0/1///2"    ).pop_right('/'      ), "2");
    EXPECT_EQ(S("0/1///2"    ).pop_right('/', true), "2");
    EXPECT_EQ(S("0/1///2/"   ).pop_right('/'      ), "");
    EXPECT_EQ(S("0/1///2/"   ).pop_right('/', true), "2/");
    EXPECT_EQ(S("0/1///2///" ).pop_right('/'      ), "");
    EXPECT_EQ(S("0/1///2///" ).pop_right('/', true), "2///");

    EXPECT_EQ(S("/0/1/2"   ).pop_right('/'      ), "2");
    EXPECT_EQ(S("/0/1/2"   ).pop_right('/', true), "2");
    EXPECT_EQ(S("/0/1/2/"  ).pop_right('/'      ), "");
    EXPECT_EQ(S("/0/1/2/"  ).pop_right('/', true), "2/");
    EXPECT_EQ(S("/0/1/2///").pop_right('/'      ), "");
    EXPECT_EQ(S("/0/1/2///").pop_right('/', true), "2///");

    EXPECT_EQ(S("0"        ).pop_right('/'      ), "0");
    EXPECT_EQ(S("0"        ).pop_right('/', true), "0");
    EXPECT_EQ(S("0/"       ).pop_right('/'      ), "");
    EXPECT_EQ(S("0/"       ).pop_right('/', true), "0/");
    EXPECT_EQ(S("0///"     ).pop_right('/'      ), "");
    EXPECT_EQ(S("0///"     ).pop_right('/', true), "0///");

    EXPECT_EQ(S("/0"       ).pop_right('/'      ), "0");
    EXPECT_EQ(S("/0"       ).pop_right('/', true), "0");
    EXPECT_EQ(S("/0/"      ).pop_right('/'      ), "");
    EXPECT_EQ(S("/0/"      ).pop_right('/', true), "0/");
    EXPECT_EQ(S("/0///"    ).pop_right('/'      ), "");
    EXPECT_EQ(S("/0///"    ).pop_right('/', true), "0///");

    EXPECT_EQ(S("/"        ).pop_right('/'      ), "");
    EXPECT_EQ(S("/"        ).pop_right('/', true), "");
    EXPECT_EQ(S("///"      ).pop_right('/'      ), "");
    EXPECT_EQ(S("///"      ).pop_right('/', true), "");

    EXPECT_EQ(S(""         ).pop_right('/'      ), "");
    EXPECT_EQ(S(""         ).pop_right('/', true), "");

    EXPECT_EQ(S("0-1-2"    ).pop_right('-'      ), "2");
    EXPECT_EQ(S("0-1-2"    ).pop_right('-', true), "2");
    EXPECT_EQ(S("0-1-2-"   ).pop_right('-'      ), "");
    EXPECT_EQ(S("0-1-2-"   ).pop_right('-', true), "2-");
    EXPECT_EQ(S("0-1-2---" ).pop_right('-'      ), "");
    EXPECT_EQ(S("0-1-2---" ).pop_right('-', true), "2---");

    EXPECT_EQ(S("0-1--2"    ).pop_right('-'      ), "2");
    EXPECT_EQ(S("0-1--2"    ).pop_right('-', true), "2");
    EXPECT_EQ(S("0-1--2-"   ).pop_right('-'      ), "");
    EXPECT_EQ(S("0-1--2-"   ).pop_right('-', true), "2-");
    EXPECT_EQ(S("0-1--2---" ).pop_right('-'      ), "");
    EXPECT_EQ(S("0-1--2---" ).pop_right('-', true), "2---");

    EXPECT_EQ(S("0-1---2"    ).pop_right('-'      ), "2");
    EXPECT_EQ(S("0-1---2"    ).pop_right('-', true), "2");
    EXPECT_EQ(S("0-1---2-"   ).pop_right('-'      ), "");
    EXPECT_EQ(S("0-1---2-"   ).pop_right('-', true), "2-");
    EXPECT_EQ(S("0-1---2---" ).pop_right('-'      ), "");
    EXPECT_EQ(S("0-1---2---" ).pop_right('-', true), "2---");

    EXPECT_EQ(S("-0-1-2"   ).pop_right('-'      ), "2");
    EXPECT_EQ(S("-0-1-2"   ).pop_right('-', true), "2");
    EXPECT_EQ(S("-0-1-2-"  ).pop_right('-'      ), "");
    EXPECT_EQ(S("-0-1-2-"  ).pop_right('-', true), "2-");
    EXPECT_EQ(S("-0-1-2---").pop_right('-'      ), "");
    EXPECT_EQ(S("-0-1-2---").pop_right('-', true), "2---");

    EXPECT_EQ(S("0"        ).pop_right('-'      ), "0");
    EXPECT_EQ(S("0"        ).pop_right('-', true), "0");
    EXPECT_EQ(S("0-"       ).pop_right('-'      ), "");
    EXPECT_EQ(S("0-"       ).pop_right('-', true), "0-");
    EXPECT_EQ(S("0---"     ).pop_right('-'      ), "");
    EXPECT_EQ(S("0---"     ).pop_right('-', true), "0---");

    EXPECT_EQ(S("-0"       ).pop_right('-'      ), "0");
    EXPECT_EQ(S("-0"       ).pop_right('-', true), "0");
    EXPECT_EQ(S("-0-"      ).pop_right('-'      ), "");
    EXPECT_EQ(S("-0-"      ).pop_right('-', true), "0-");
    EXPECT_EQ(S("-0---"    ).pop_right('-'      ), "");
    EXPECT_EQ(S("-0---"    ).pop_right('-', true), "0---");

    EXPECT_EQ(S("-"        ).pop_right('-'      ), "");
    EXPECT_EQ(S("-"        ).pop_right('-', true), "");
    EXPECT_EQ(S("---"      ).pop_right('-'      ), "");
    EXPECT_EQ(S("---"      ).pop_right('-', true), "");

    EXPECT_EQ(S(""         ).pop_right('-'      ), "");
    EXPECT_EQ(S(""         ).pop_right('-', true), "");
}

TEST(substr, pop_left)
{
    using S = csubstr;

    EXPECT_EQ(S("0/1/2"    ).pop_left('/'      ), "0");
    EXPECT_EQ(S("0/1/2"    ).pop_left('/', true), "0");
    EXPECT_EQ(S("0/1/2/"   ).pop_left('/'      ), "0");
    EXPECT_EQ(S("0/1/2/"   ).pop_left('/', true), "0");
    EXPECT_EQ(S("0/1/2///" ).pop_left('/'      ), "0");
    EXPECT_EQ(S("0/1/2///" ).pop_left('/', true), "0");

    EXPECT_EQ(S("0//1/2"    ).pop_left('/'      ), "0");
    EXPECT_EQ(S("0//1/2"    ).pop_left('/', true), "0");
    EXPECT_EQ(S("0//1/2/"   ).pop_left('/'      ), "0");
    EXPECT_EQ(S("0//1/2/"   ).pop_left('/', true), "0");
    EXPECT_EQ(S("0//1/2///" ).pop_left('/'      ), "0");
    EXPECT_EQ(S("0//1/2///" ).pop_left('/', true), "0");

    EXPECT_EQ(S("0///1/2"    ).pop_left('/'      ), "0");
    EXPECT_EQ(S("0///1/2"    ).pop_left('/', true), "0");
    EXPECT_EQ(S("0///1/2/"   ).pop_left('/'      ), "0");
    EXPECT_EQ(S("0///1/2/"   ).pop_left('/', true), "0");
    EXPECT_EQ(S("0///1/2///" ).pop_left('/'      ), "0");
    EXPECT_EQ(S("0///1/2///" ).pop_left('/', true), "0");

    EXPECT_EQ(S("/0/1/2"   ).pop_left('/'      ), "");
    EXPECT_EQ(S("/0/1/2"   ).pop_left('/', true), "/0");
    EXPECT_EQ(S("/0/1/2/"  ).pop_left('/'      ), "");
    EXPECT_EQ(S("/0/1/2/"  ).pop_left('/', true), "/0");
    EXPECT_EQ(S("/0/1/2///").pop_left('/'      ), "");
    EXPECT_EQ(S("/0/1/2///").pop_left('/', true), "/0");
    EXPECT_EQ(S("///0/1/2" ).pop_left('/'      ), "");
    EXPECT_EQ(S("///0/1/2" ).pop_left('/', true), "///0");
    EXPECT_EQ(S("///0/1/2/").pop_left('/'      ), "");
    EXPECT_EQ(S("///0/1/2/").pop_left('/', true), "///0");
    EXPECT_EQ(S("///0/1/2/").pop_left('/'      ), "");
    EXPECT_EQ(S("///0/1/2/").pop_left('/', true), "///0");

    EXPECT_EQ(S("0"        ).pop_left('/'      ), "0");
    EXPECT_EQ(S("0"        ).pop_left('/', true), "0");
    EXPECT_EQ(S("0/"       ).pop_left('/'      ), "0");
    EXPECT_EQ(S("0/"       ).pop_left('/', true), "0");
    EXPECT_EQ(S("0///"     ).pop_left('/'      ), "0");
    EXPECT_EQ(S("0///"     ).pop_left('/', true), "0");

    EXPECT_EQ(S("/0"       ).pop_left('/'      ), "");
    EXPECT_EQ(S("/0"       ).pop_left('/', true), "/0");
    EXPECT_EQ(S("/0/"      ).pop_left('/'      ), "");
    EXPECT_EQ(S("/0/"      ).pop_left('/', true), "/0");
    EXPECT_EQ(S("/0///"    ).pop_left('/'      ), "");
    EXPECT_EQ(S("/0///"    ).pop_left('/', true), "/0");
    EXPECT_EQ(S("///0///"  ).pop_left('/'      ), "");
    EXPECT_EQ(S("///0///"  ).pop_left('/', true), "///0");

    EXPECT_EQ(S("/"        ).pop_left('/'      ), "");
    EXPECT_EQ(S("/"        ).pop_left('/', true), "");
    EXPECT_EQ(S("///"      ).pop_left('/'      ), "");
    EXPECT_EQ(S("///"      ).pop_left('/', true), "");

    EXPECT_EQ(S(""         ).pop_left('/'      ), "");
    EXPECT_EQ(S(""         ).pop_left('/', true), "");

    EXPECT_EQ(S("0-1-2"    ).pop_left('-'      ), "0");
    EXPECT_EQ(S("0-1-2"    ).pop_left('-', true), "0");
    EXPECT_EQ(S("0-1-2-"   ).pop_left('-'      ), "0");
    EXPECT_EQ(S("0-1-2-"   ).pop_left('-', true), "0");
    EXPECT_EQ(S("0-1-2---" ).pop_left('-'      ), "0");
    EXPECT_EQ(S("0-1-2---" ).pop_left('-', true), "0");

    EXPECT_EQ(S("0--1-2"    ).pop_left('-'      ), "0");
    EXPECT_EQ(S("0--1-2"    ).pop_left('-', true), "0");
    EXPECT_EQ(S("0--1-2-"   ).pop_left('-'      ), "0");
    EXPECT_EQ(S("0--1-2-"   ).pop_left('-', true), "0");
    EXPECT_EQ(S("0--1-2---" ).pop_left('-'      ), "0");
    EXPECT_EQ(S("0--1-2---" ).pop_left('-', true), "0");

    EXPECT_EQ(S("0---1-2"    ).pop_left('-'      ), "0");
    EXPECT_EQ(S("0---1-2"    ).pop_left('-', true), "0");
    EXPECT_EQ(S("0---1-2-"   ).pop_left('-'      ), "0");
    EXPECT_EQ(S("0---1-2-"   ).pop_left('-', true), "0");
    EXPECT_EQ(S("0---1-2---" ).pop_left('-'      ), "0");
    EXPECT_EQ(S("0---1-2---" ).pop_left('-', true), "0");

    EXPECT_EQ(S("-0-1-2"   ).pop_left('-'      ), "");
    EXPECT_EQ(S("-0-1-2"   ).pop_left('-', true), "-0");
    EXPECT_EQ(S("-0-1-2-"  ).pop_left('-'      ), "");
    EXPECT_EQ(S("-0-1-2-"  ).pop_left('-', true), "-0");
    EXPECT_EQ(S("-0-1-2---").pop_left('-'      ), "");
    EXPECT_EQ(S("-0-1-2---").pop_left('-', true), "-0");
    EXPECT_EQ(S("---0-1-2" ).pop_left('-'      ), "");
    EXPECT_EQ(S("---0-1-2" ).pop_left('-', true), "---0");
    EXPECT_EQ(S("---0-1-2-").pop_left('-'      ), "");
    EXPECT_EQ(S("---0-1-2-").pop_left('-', true), "---0");
    EXPECT_EQ(S("---0-1-2-").pop_left('-'      ), "");
    EXPECT_EQ(S("---0-1-2-").pop_left('-', true), "---0");

    EXPECT_EQ(S("0"        ).pop_left('-'      ), "0");
    EXPECT_EQ(S("0"        ).pop_left('-', true), "0");
    EXPECT_EQ(S("0-"       ).pop_left('-'      ), "0");
    EXPECT_EQ(S("0-"       ).pop_left('-', true), "0");
    EXPECT_EQ(S("0---"     ).pop_left('-'      ), "0");
    EXPECT_EQ(S("0---"     ).pop_left('-', true), "0");

    EXPECT_EQ(S("-0"       ).pop_left('-'      ), "");
    EXPECT_EQ(S("-0"       ).pop_left('-', true), "-0");
    EXPECT_EQ(S("-0-"      ).pop_left('-'      ), "");
    EXPECT_EQ(S("-0-"      ).pop_left('-', true), "-0");
    EXPECT_EQ(S("-0---"    ).pop_left('-'      ), "");
    EXPECT_EQ(S("-0---"    ).pop_left('-', true), "-0");
    EXPECT_EQ(S("---0---"  ).pop_left('-'      ), "");
    EXPECT_EQ(S("---0---"  ).pop_left('-', true), "---0");

    EXPECT_EQ(S("-"        ).pop_left('-'      ), "");
    EXPECT_EQ(S("-"        ).pop_left('-', true), "");
    EXPECT_EQ(S("---"      ).pop_left('-'      ), "");
    EXPECT_EQ(S("---"      ).pop_left('-', true), "");

    EXPECT_EQ(S(""         ).pop_left('-'      ), "");
    EXPECT_EQ(S(""         ).pop_left('-', true), "");
}

TEST(substr, gpop_left)
{
    using S = csubstr;

    EXPECT_EQ(S("0/1/2"      ).gpop_left('/'      ), "0/1");
    EXPECT_EQ(S("0/1/2"      ).gpop_left('/', true), "0/1");
    EXPECT_EQ(S("0/1/2/"     ).gpop_left('/'      ), "0/1/2");
    EXPECT_EQ(S("0/1/2/"     ).gpop_left('/', true), "0/1");
    EXPECT_EQ(S("0/1/2//"    ).gpop_left('/'      ), "0/1/2/");
    EXPECT_EQ(S("0/1/2//"    ).gpop_left('/', true), "0/1");
    EXPECT_EQ(S("0/1/2///"   ).gpop_left('/'      ), "0/1/2//");
    EXPECT_EQ(S("0/1/2///"   ).gpop_left('/', true), "0/1");

    EXPECT_EQ(S("0/1//2"     ).gpop_left('/'      ), "0/1/");
    EXPECT_EQ(S("0/1//2"     ).gpop_left('/', true), "0/1");
    EXPECT_EQ(S("0/1//2/"    ).gpop_left('/'      ), "0/1//2");
    EXPECT_EQ(S("0/1//2/"    ).gpop_left('/', true), "0/1");
    EXPECT_EQ(S("0/1//2//"   ).gpop_left('/'      ), "0/1//2/");
    EXPECT_EQ(S("0/1//2//"   ).gpop_left('/', true), "0/1");
    EXPECT_EQ(S("0/1//2///"  ).gpop_left('/'      ), "0/1//2//");
    EXPECT_EQ(S("0/1//2///"  ).gpop_left('/', true), "0/1");

    EXPECT_EQ(S("0/1///2"    ).gpop_left('/'      ), "0/1//");
    EXPECT_EQ(S("0/1///2"    ).gpop_left('/', true), "0/1");
    EXPECT_EQ(S("0/1///2/"   ).gpop_left('/'      ), "0/1///2");
    EXPECT_EQ(S("0/1///2/"   ).gpop_left('/', true), "0/1");
    EXPECT_EQ(S("0/1///2//"  ).gpop_left('/'      ), "0/1///2/");
    EXPECT_EQ(S("0/1///2//"  ).gpop_left('/', true), "0/1");
    EXPECT_EQ(S("0/1///2///" ).gpop_left('/'      ), "0/1///2//");
    EXPECT_EQ(S("0/1///2///" ).gpop_left('/', true), "0/1");

    EXPECT_EQ(S("/0/1/2"     ).gpop_left('/'      ), "/0/1");
    EXPECT_EQ(S("/0/1/2"     ).gpop_left('/', true), "/0/1");
    EXPECT_EQ(S("/0/1/2/"    ).gpop_left('/'      ), "/0/1/2");
    EXPECT_EQ(S("/0/1/2/"    ).gpop_left('/', true), "/0/1");
    EXPECT_EQ(S("/0/1/2//"   ).gpop_left('/'      ), "/0/1/2/");
    EXPECT_EQ(S("/0/1/2//"   ).gpop_left('/', true), "/0/1");
    EXPECT_EQ(S("/0/1/2///"  ).gpop_left('/'      ), "/0/1/2//");
    EXPECT_EQ(S("/0/1/2///"  ).gpop_left('/', true), "/0/1");

    EXPECT_EQ(S("//0/1/2"    ).gpop_left('/'      ), "//0/1");
    EXPECT_EQ(S("//0/1/2"    ).gpop_left('/', true), "//0/1");
    EXPECT_EQ(S("//0/1/2/"   ).gpop_left('/'      ), "//0/1/2");
    EXPECT_EQ(S("//0/1/2/"   ).gpop_left('/', true), "//0/1");
    EXPECT_EQ(S("//0/1/2//"  ).gpop_left('/'      ), "//0/1/2/");
    EXPECT_EQ(S("//0/1/2//"  ).gpop_left('/', true), "//0/1");
    EXPECT_EQ(S("//0/1/2///" ).gpop_left('/'      ), "//0/1/2//");
    EXPECT_EQ(S("//0/1/2///" ).gpop_left('/', true), "//0/1");

    EXPECT_EQ(S("///0/1/2"   ).gpop_left('/'      ), "///0/1");
    EXPECT_EQ(S("///0/1/2"   ).gpop_left('/', true), "///0/1");
    EXPECT_EQ(S("///0/1/2/"  ).gpop_left('/'      ), "///0/1/2");
    EXPECT_EQ(S("///0/1/2/"  ).gpop_left('/', true), "///0/1");
    EXPECT_EQ(S("///0/1/2//" ).gpop_left('/'      ), "///0/1/2/");
    EXPECT_EQ(S("///0/1/2//" ).gpop_left('/', true), "///0/1");
    EXPECT_EQ(S("///0/1/2///").gpop_left('/'      ), "///0/1/2//");
    EXPECT_EQ(S("///0/1/2///").gpop_left('/', true), "///0/1");


    EXPECT_EQ(S("0/1"      ).gpop_left('/'      ), "0");
    EXPECT_EQ(S("0/1"      ).gpop_left('/', true), "0");
    EXPECT_EQ(S("0/1/"     ).gpop_left('/'      ), "0/1");
    EXPECT_EQ(S("0/1/"     ).gpop_left('/', true), "0");
    EXPECT_EQ(S("0/1//"    ).gpop_left('/'      ), "0/1/");
    EXPECT_EQ(S("0/1//"    ).gpop_left('/', true), "0");
    EXPECT_EQ(S("0/1///"   ).gpop_left('/'      ), "0/1//");
    EXPECT_EQ(S("0/1///"   ).gpop_left('/', true), "0");

    EXPECT_EQ(S("0//1"     ).gpop_left('/'      ), "0/");
    EXPECT_EQ(S("0//1"     ).gpop_left('/', true), "0");
    EXPECT_EQ(S("0//1/"    ).gpop_left('/'      ), "0//1");
    EXPECT_EQ(S("0//1/"    ).gpop_left('/', true), "0");
    EXPECT_EQ(S("0//1//"   ).gpop_left('/'      ), "0//1/");
    EXPECT_EQ(S("0//1//"   ).gpop_left('/', true), "0");
    EXPECT_EQ(S("0//1///"  ).gpop_left('/'      ), "0//1//");
    EXPECT_EQ(S("0//1///"  ).gpop_left('/', true), "0");

    EXPECT_EQ(S("0///1"    ).gpop_left('/'      ), "0//");
    EXPECT_EQ(S("0///1"    ).gpop_left('/', true), "0");
    EXPECT_EQ(S("0///1/"   ).gpop_left('/'      ), "0///1");
    EXPECT_EQ(S("0///1/"   ).gpop_left('/', true), "0");
    EXPECT_EQ(S("0///1//"  ).gpop_left('/'      ), "0///1/");
    EXPECT_EQ(S("0///1//"  ).gpop_left('/', true), "0");
    EXPECT_EQ(S("0///1///" ).gpop_left('/'      ), "0///1//");
    EXPECT_EQ(S("0///1///" ).gpop_left('/', true), "0");

    EXPECT_EQ(S("/0/1"      ).gpop_left('/'      ), "/0");
    EXPECT_EQ(S("/0/1"      ).gpop_left('/', true), "/0");
    EXPECT_EQ(S("/0/1/"     ).gpop_left('/'      ), "/0/1");
    EXPECT_EQ(S("/0/1/"     ).gpop_left('/', true), "/0");
    EXPECT_EQ(S("/0/1//"    ).gpop_left('/'      ), "/0/1/");
    EXPECT_EQ(S("/0/1//"    ).gpop_left('/', true), "/0");
    EXPECT_EQ(S("/0/1///"   ).gpop_left('/'      ), "/0/1//");
    EXPECT_EQ(S("/0/1///"   ).gpop_left('/', true), "/0");

    EXPECT_EQ(S("/0//1"     ).gpop_left('/'      ), "/0/");
    EXPECT_EQ(S("/0//1"     ).gpop_left('/', true), "/0");
    EXPECT_EQ(S("/0//1/"    ).gpop_left('/'      ), "/0//1");
    EXPECT_EQ(S("/0//1/"    ).gpop_left('/', true), "/0");
    EXPECT_EQ(S("/0//1//"   ).gpop_left('/'      ), "/0//1/");
    EXPECT_EQ(S("/0//1//"   ).gpop_left('/', true), "/0");
    EXPECT_EQ(S("/0//1///"  ).gpop_left('/'      ), "/0//1//");
    EXPECT_EQ(S("/0//1///"  ).gpop_left('/', true), "/0");

    EXPECT_EQ(S("/0///1"    ).gpop_left('/'      ), "/0//");
    EXPECT_EQ(S("/0///1"    ).gpop_left('/', true), "/0");
    EXPECT_EQ(S("/0///1/"   ).gpop_left('/'      ), "/0///1");
    EXPECT_EQ(S("/0///1/"   ).gpop_left('/', true), "/0");
    EXPECT_EQ(S("/0///1//"  ).gpop_left('/'      ), "/0///1/");
    EXPECT_EQ(S("/0///1//"  ).gpop_left('/', true), "/0");
    EXPECT_EQ(S("/0///1///" ).gpop_left('/'      ), "/0///1//");
    EXPECT_EQ(S("/0///1///" ).gpop_left('/', true), "/0");

    EXPECT_EQ(S("//0/1"      ).gpop_left('/'      ), "//0");
    EXPECT_EQ(S("//0/1"      ).gpop_left('/', true), "//0");
    EXPECT_EQ(S("//0/1/"     ).gpop_left('/'      ), "//0/1");
    EXPECT_EQ(S("//0/1/"     ).gpop_left('/', true), "//0");
    EXPECT_EQ(S("//0/1//"    ).gpop_left('/'      ), "//0/1/");
    EXPECT_EQ(S("//0/1//"    ).gpop_left('/', true), "//0");
    EXPECT_EQ(S("//0/1///"   ).gpop_left('/'      ), "//0/1//");
    EXPECT_EQ(S("//0/1///"   ).gpop_left('/', true), "//0");

    EXPECT_EQ(S("//0//1"     ).gpop_left('/'      ), "//0/");
    EXPECT_EQ(S("//0//1"     ).gpop_left('/', true), "//0");
    EXPECT_EQ(S("//0//1/"    ).gpop_left('/'      ), "//0//1");
    EXPECT_EQ(S("//0//1/"    ).gpop_left('/', true), "//0");
    EXPECT_EQ(S("//0//1//"   ).gpop_left('/'      ), "//0//1/");
    EXPECT_EQ(S("//0//1//"   ).gpop_left('/', true), "//0");
    EXPECT_EQ(S("//0//1///"  ).gpop_left('/'      ), "//0//1//");
    EXPECT_EQ(S("//0//1///"  ).gpop_left('/', true), "//0");

    EXPECT_EQ(S("//0///1"    ).gpop_left('/'      ), "//0//");
    EXPECT_EQ(S("//0///1"    ).gpop_left('/', true), "//0");
    EXPECT_EQ(S("//0///1/"   ).gpop_left('/'      ), "//0///1");
    EXPECT_EQ(S("//0///1/"   ).gpop_left('/', true), "//0");
    EXPECT_EQ(S("//0///1//"  ).gpop_left('/'      ), "//0///1/");
    EXPECT_EQ(S("//0///1//"  ).gpop_left('/', true), "//0");
    EXPECT_EQ(S("//0///1///" ).gpop_left('/'      ), "//0///1//");
    EXPECT_EQ(S("//0///1///" ).gpop_left('/', true), "//0");

    EXPECT_EQ(S("0"      ).gpop_left('/'      ), "");
    EXPECT_EQ(S("0"      ).gpop_left('/', true), "");
    EXPECT_EQ(S("0/"     ).gpop_left('/'      ), "0");
    EXPECT_EQ(S("0/"     ).gpop_left('/', true), "");
    EXPECT_EQ(S("0//"    ).gpop_left('/'      ), "0/");
    EXPECT_EQ(S("0//"    ).gpop_left('/', true), "");
    EXPECT_EQ(S("0///"   ).gpop_left('/'      ), "0//");
    EXPECT_EQ(S("0///"   ).gpop_left('/', true), "");

    EXPECT_EQ(S("/0"      ).gpop_left('/'      ), "");
    EXPECT_EQ(S("/0"      ).gpop_left('/', true), "");
    EXPECT_EQ(S("/0/"     ).gpop_left('/'      ), "/0");
    EXPECT_EQ(S("/0/"     ).gpop_left('/', true), "");
    EXPECT_EQ(S("/0//"    ).gpop_left('/'      ), "/0/");
    EXPECT_EQ(S("/0//"    ).gpop_left('/', true), "");
    EXPECT_EQ(S("/0///"   ).gpop_left('/'      ), "/0//");
    EXPECT_EQ(S("/0///"   ).gpop_left('/', true), "");

    EXPECT_EQ(S("//0"      ).gpop_left('/'      ), "/");
    EXPECT_EQ(S("//0"      ).gpop_left('/', true), "");
    EXPECT_EQ(S("//0/"     ).gpop_left('/'      ), "//0");
    EXPECT_EQ(S("//0/"     ).gpop_left('/', true), "");
    EXPECT_EQ(S("//0//"    ).gpop_left('/'      ), "//0/");
    EXPECT_EQ(S("//0//"    ).gpop_left('/', true), "");
    EXPECT_EQ(S("//0///"   ).gpop_left('/'      ), "//0//");
    EXPECT_EQ(S("//0///"   ).gpop_left('/', true), "");

    EXPECT_EQ(S("///0"      ).gpop_left('/'      ), "//");
    EXPECT_EQ(S("///0"      ).gpop_left('/', true), "");
    EXPECT_EQ(S("///0/"     ).gpop_left('/'      ), "///0");
    EXPECT_EQ(S("///0/"     ).gpop_left('/', true), "");
    EXPECT_EQ(S("///0//"    ).gpop_left('/'      ), "///0/");
    EXPECT_EQ(S("///0//"    ).gpop_left('/', true), "");
    EXPECT_EQ(S("///0///"   ).gpop_left('/'      ), "///0//");
    EXPECT_EQ(S("///0///"   ).gpop_left('/', true), "");

    EXPECT_EQ(S("/"        ).gpop_left('/'      ), "");
    EXPECT_EQ(S("/"        ).gpop_left('/', true), "");
    EXPECT_EQ(S("//"       ).gpop_left('/'      ), "/");
    EXPECT_EQ(S("//"       ).gpop_left('/', true), "");
    EXPECT_EQ(S("///"      ).gpop_left('/'      ), "//");
    EXPECT_EQ(S("///"      ).gpop_left('/', true), "");

    EXPECT_EQ(S(""         ).gpop_left('/'      ), "");
    EXPECT_EQ(S(""         ).gpop_left('/', true), "");
}

TEST(substr, gpop_right)
{
    using S = csubstr;

    EXPECT_EQ(S("0/1/2"      ).gpop_right('/'      ), "1/2");
    EXPECT_EQ(S("0/1/2"      ).gpop_right('/', true), "1/2");
    EXPECT_EQ(S("0/1/2/"     ).gpop_right('/'      ), "1/2/");
    EXPECT_EQ(S("0/1/2/"     ).gpop_right('/', true), "1/2/");
    EXPECT_EQ(S("0/1/2//"    ).gpop_right('/'      ), "1/2//");
    EXPECT_EQ(S("0/1/2//"    ).gpop_right('/', true), "1/2//");
    EXPECT_EQ(S("0/1/2///"   ).gpop_right('/'      ), "1/2///");
    EXPECT_EQ(S("0/1/2///"   ).gpop_right('/', true), "1/2///");

    EXPECT_EQ(S("0//1/2"     ).gpop_right('/'      ), "/1/2");
    EXPECT_EQ(S("0//1/2"     ).gpop_right('/', true),  "1/2");
    EXPECT_EQ(S("0//1/2/"    ).gpop_right('/'      ), "/1/2/");
    EXPECT_EQ(S("0//1/2/"    ).gpop_right('/', true),  "1/2/");
    EXPECT_EQ(S("0//1/2//"   ).gpop_right('/'      ), "/1/2//");
    EXPECT_EQ(S("0//1/2//"   ).gpop_right('/', true),  "1/2//");
    EXPECT_EQ(S("0//1/2///"  ).gpop_right('/'      ), "/1/2///");
    EXPECT_EQ(S("0//1/2///"  ).gpop_right('/', true),  "1/2///");

    EXPECT_EQ(S("0///1/2"     ).gpop_right('/'      ), "//1/2");
    EXPECT_EQ(S("0///1/2"     ).gpop_right('/', true),   "1/2");
    EXPECT_EQ(S("0///1/2/"    ).gpop_right('/'      ), "//1/2/");
    EXPECT_EQ(S("0///1/2/"    ).gpop_right('/', true),   "1/2/");
    EXPECT_EQ(S("0///1/2//"   ).gpop_right('/'      ), "//1/2//");
    EXPECT_EQ(S("0///1/2//"   ).gpop_right('/', true),   "1/2//");
    EXPECT_EQ(S("0///1/2///"  ).gpop_right('/'      ), "//1/2///");
    EXPECT_EQ(S("0///1/2///"  ).gpop_right('/', true),   "1/2///");


    EXPECT_EQ(S("/0/1/2"      ).gpop_right('/'      ), "0/1/2");
    EXPECT_EQ(S("/0/1/2"      ).gpop_right('/', true),   "1/2");
    EXPECT_EQ(S("/0/1/2/"     ).gpop_right('/'      ), "0/1/2/");
    EXPECT_EQ(S("/0/1/2/"     ).gpop_right('/', true),   "1/2/");
    EXPECT_EQ(S("/0/1/2//"    ).gpop_right('/'      ), "0/1/2//");
    EXPECT_EQ(S("/0/1/2//"    ).gpop_right('/', true),   "1/2//");
    EXPECT_EQ(S("/0/1/2///"   ).gpop_right('/'      ), "0/1/2///");
    EXPECT_EQ(S("/0/1/2///"   ).gpop_right('/', true),   "1/2///");

    EXPECT_EQ(S("/0//1/2"     ).gpop_right('/'      ), "0//1/2");
    EXPECT_EQ(S("/0//1/2"     ).gpop_right('/', true),    "1/2");
    EXPECT_EQ(S("/0//1/2/"    ).gpop_right('/'      ), "0//1/2/");
    EXPECT_EQ(S("/0//1/2/"    ).gpop_right('/', true),    "1/2/");
    EXPECT_EQ(S("/0//1/2//"   ).gpop_right('/'      ), "0//1/2//");
    EXPECT_EQ(S("/0//1/2//"   ).gpop_right('/', true),    "1/2//");
    EXPECT_EQ(S("/0//1/2///"  ).gpop_right('/'      ), "0//1/2///");
    EXPECT_EQ(S("/0//1/2///"  ).gpop_right('/', true),    "1/2///");

    EXPECT_EQ(S("/0///1/2"     ).gpop_right('/'      ), "0///1/2");
    EXPECT_EQ(S("/0///1/2"     ).gpop_right('/', true),     "1/2");
    EXPECT_EQ(S("/0///1/2/"    ).gpop_right('/'      ), "0///1/2/");
    EXPECT_EQ(S("/0///1/2/"    ).gpop_right('/', true),     "1/2/");
    EXPECT_EQ(S("/0///1/2//"   ).gpop_right('/'      ), "0///1/2//");
    EXPECT_EQ(S("/0///1/2//"   ).gpop_right('/', true),     "1/2//");
    EXPECT_EQ(S("/0///1/2///"  ).gpop_right('/'      ), "0///1/2///");
    EXPECT_EQ(S("/0///1/2///"  ).gpop_right('/', true),     "1/2///");


    EXPECT_EQ(S("//0/1/2"      ).gpop_right('/'      ), "/0/1/2");
    EXPECT_EQ(S("//0/1/2"      ).gpop_right('/', true),    "1/2");
    EXPECT_EQ(S("//0/1/2/"     ).gpop_right('/'      ), "/0/1/2/");
    EXPECT_EQ(S("//0/1/2/"     ).gpop_right('/', true),    "1/2/");
    EXPECT_EQ(S("//0/1/2//"    ).gpop_right('/'      ), "/0/1/2//");
    EXPECT_EQ(S("//0/1/2//"    ).gpop_right('/', true),    "1/2//");
    EXPECT_EQ(S("//0/1/2///"   ).gpop_right('/'      ), "/0/1/2///");
    EXPECT_EQ(S("//0/1/2///"   ).gpop_right('/', true),    "1/2///");

    EXPECT_EQ(S("//0//1/2"     ).gpop_right('/'      ), "/0//1/2");
    EXPECT_EQ(S("//0//1/2"     ).gpop_right('/', true),     "1/2");
    EXPECT_EQ(S("//0//1/2/"    ).gpop_right('/'      ), "/0//1/2/");
    EXPECT_EQ(S("//0//1/2/"    ).gpop_right('/', true),     "1/2/");
    EXPECT_EQ(S("//0//1/2//"   ).gpop_right('/'      ), "/0//1/2//");
    EXPECT_EQ(S("//0//1/2//"   ).gpop_right('/', true),     "1/2//");
    EXPECT_EQ(S("//0//1/2///"  ).gpop_right('/'      ), "/0//1/2///");
    EXPECT_EQ(S("//0//1/2///"  ).gpop_right('/', true),     "1/2///");

    EXPECT_EQ(S("//0///1/2"     ).gpop_right('/'      ), "/0///1/2");
    EXPECT_EQ(S("//0///1/2"     ).gpop_right('/', true),     "1/2");
    EXPECT_EQ(S("//0///1/2/"    ).gpop_right('/'      ), "/0///1/2/");
    EXPECT_EQ(S("//0///1/2/"    ).gpop_right('/', true),     "1/2/");
    EXPECT_EQ(S("//0///1/2//"   ).gpop_right('/'      ), "/0///1/2//");
    EXPECT_EQ(S("//0///1/2//"   ).gpop_right('/', true),     "1/2//");
    EXPECT_EQ(S("//0///1/2///"  ).gpop_right('/'      ), "/0///1/2///");
    EXPECT_EQ(S("//0///1/2///"  ).gpop_right('/', true),      "1/2///");


    EXPECT_EQ(S("0/1"      ).gpop_right('/'      ), "1");
    EXPECT_EQ(S("0/1"      ).gpop_right('/', true), "1");
    EXPECT_EQ(S("0/1/"     ).gpop_right('/'      ), "1/");
    EXPECT_EQ(S("0/1/"     ).gpop_right('/', true), "1/");
    EXPECT_EQ(S("0/1//"    ).gpop_right('/'      ), "1//");
    EXPECT_EQ(S("0/1//"    ).gpop_right('/', true), "1//");
    EXPECT_EQ(S("0/1///"   ).gpop_right('/'      ), "1///");
    EXPECT_EQ(S("0/1///"   ).gpop_right('/', true), "1///");

    EXPECT_EQ(S("0//1"     ).gpop_right('/'      ), "/1");
    EXPECT_EQ(S("0//1"     ).gpop_right('/', true),  "1");
    EXPECT_EQ(S("0//1/"    ).gpop_right('/'      ), "/1/");
    EXPECT_EQ(S("0//1/"    ).gpop_right('/', true),  "1/");
    EXPECT_EQ(S("0//1//"   ).gpop_right('/'      ), "/1//");
    EXPECT_EQ(S("0//1//"   ).gpop_right('/', true),  "1//");
    EXPECT_EQ(S("0//1///"  ).gpop_right('/'      ), "/1///");
    EXPECT_EQ(S("0//1///"  ).gpop_right('/', true),  "1///");

    EXPECT_EQ(S("0///1"    ).gpop_right('/'      ), "//1");
    EXPECT_EQ(S("0///1"    ).gpop_right('/', true),   "1");
    EXPECT_EQ(S("0///1/"   ).gpop_right('/'      ), "//1/");
    EXPECT_EQ(S("0///1/"   ).gpop_right('/', true),   "1/");
    EXPECT_EQ(S("0///1//"  ).gpop_right('/'      ), "//1//");
    EXPECT_EQ(S("0///1//"  ).gpop_right('/', true),   "1//");
    EXPECT_EQ(S("0///1///" ).gpop_right('/'      ), "//1///");
    EXPECT_EQ(S("0///1///" ).gpop_right('/', true),   "1///");


    EXPECT_EQ(S("/0/1"      ).gpop_right('/'      ), "0/1");
    EXPECT_EQ(S("/0/1"      ).gpop_right('/', true),   "1");
    EXPECT_EQ(S("/0/1/"     ).gpop_right('/'      ), "0/1/");
    EXPECT_EQ(S("/0/1/"     ).gpop_right('/', true),   "1/");
    EXPECT_EQ(S("/0/1//"    ).gpop_right('/'      ), "0/1//");
    EXPECT_EQ(S("/0/1//"    ).gpop_right('/', true),   "1//");
    EXPECT_EQ(S("/0/1///"   ).gpop_right('/'      ), "0/1///");
    EXPECT_EQ(S("/0/1///"   ).gpop_right('/', true),   "1///");

    EXPECT_EQ(S("/0//1"     ).gpop_right('/'      ), "0//1");
    EXPECT_EQ(S("/0//1"     ).gpop_right('/', true),    "1");
    EXPECT_EQ(S("/0//1/"    ).gpop_right('/'      ), "0//1/");
    EXPECT_EQ(S("/0//1/"    ).gpop_right('/', true),    "1/");
    EXPECT_EQ(S("/0//1//"   ).gpop_right('/'      ), "0//1//");
    EXPECT_EQ(S("/0//1//"   ).gpop_right('/', true),    "1//");
    EXPECT_EQ(S("/0//1///"  ).gpop_right('/'      ), "0//1///");
    EXPECT_EQ(S("/0//1///"  ).gpop_right('/', true),    "1///");

    EXPECT_EQ(S("/0///1"    ).gpop_right('/'      ), "0///1");
    EXPECT_EQ(S("/0///1"    ).gpop_right('/', true),     "1");
    EXPECT_EQ(S("/0///1/"   ).gpop_right('/'      ), "0///1/");
    EXPECT_EQ(S("/0///1/"   ).gpop_right('/', true),     "1/");
    EXPECT_EQ(S("/0///1//"  ).gpop_right('/'      ), "0///1//");
    EXPECT_EQ(S("/0///1//"  ).gpop_right('/', true),     "1//");
    EXPECT_EQ(S("/0///1///" ).gpop_right('/'      ), "0///1///");
    EXPECT_EQ(S("/0///1///" ).gpop_right('/', true),     "1///");


    EXPECT_EQ(S("//0/1"      ).gpop_right('/'      ), "/0/1");
    EXPECT_EQ(S("//0/1"      ).gpop_right('/', true),    "1");
    EXPECT_EQ(S("//0/1/"     ).gpop_right('/'      ), "/0/1/");
    EXPECT_EQ(S("//0/1/"     ).gpop_right('/', true),    "1/");
    EXPECT_EQ(S("//0/1//"    ).gpop_right('/'      ), "/0/1//");
    EXPECT_EQ(S("//0/1//"    ).gpop_right('/', true),    "1//");
    EXPECT_EQ(S("//0/1///"   ).gpop_right('/'      ), "/0/1///");
    EXPECT_EQ(S("//0/1///"   ).gpop_right('/', true),    "1///");

    EXPECT_EQ(S("//0//1"     ).gpop_right('/'      ), "/0//1");
    EXPECT_EQ(S("//0//1"     ).gpop_right('/', true),     "1");
    EXPECT_EQ(S("//0//1/"    ).gpop_right('/'      ), "/0//1/");
    EXPECT_EQ(S("//0//1/"    ).gpop_right('/', true),     "1/");
    EXPECT_EQ(S("//0//1//"   ).gpop_right('/'      ), "/0//1//");
    EXPECT_EQ(S("//0//1//"   ).gpop_right('/', true),     "1//");
    EXPECT_EQ(S("//0//1///"  ).gpop_right('/'      ), "/0//1///");
    EXPECT_EQ(S("//0//1///"  ).gpop_right('/', true),     "1///");

    EXPECT_EQ(S("//0///1"    ).gpop_right('/'      ), "/0///1");
    EXPECT_EQ(S("//0///1"    ).gpop_right('/', true),      "1");
    EXPECT_EQ(S("//0///1/"   ).gpop_right('/'      ), "/0///1/");
    EXPECT_EQ(S("//0///1/"   ).gpop_right('/', true),      "1/");
    EXPECT_EQ(S("//0///1//"  ).gpop_right('/'      ), "/0///1//");
    EXPECT_EQ(S("//0///1//"  ).gpop_right('/', true),      "1//");
    EXPECT_EQ(S("//0///1///" ).gpop_right('/'      ), "/0///1///");
    EXPECT_EQ(S("//0///1///" ).gpop_right('/', true),      "1///");


    EXPECT_EQ(S("0"      ).gpop_right('/'      ), "");
    EXPECT_EQ(S("0"      ).gpop_right('/', true), "");
    EXPECT_EQ(S("0/"     ).gpop_right('/'      ), "");
    EXPECT_EQ(S("0/"     ).gpop_right('/', true), "");
    EXPECT_EQ(S("0//"    ).gpop_right('/'      ), "/");
    EXPECT_EQ(S("0//"    ).gpop_right('/', true), "");
    EXPECT_EQ(S("0///"   ).gpop_right('/'      ), "//");
    EXPECT_EQ(S("0///"   ).gpop_right('/', true), "");

    EXPECT_EQ(S("/0"      ).gpop_right('/'      ), "0");
    EXPECT_EQ(S("/0"      ).gpop_right('/', true), "");
    EXPECT_EQ(S("/0/"     ).gpop_right('/'      ), "0/");
    EXPECT_EQ(S("/0/"     ).gpop_right('/', true), "");
    EXPECT_EQ(S("/0//"    ).gpop_right('/'      ), "0//");
    EXPECT_EQ(S("/0//"    ).gpop_right('/', true), "");
    EXPECT_EQ(S("/0///"   ).gpop_right('/'      ), "0///");
    EXPECT_EQ(S("/0///"   ).gpop_right('/', true), "");

    EXPECT_EQ(S("//0"      ).gpop_right('/'      ), "/0");
    EXPECT_EQ(S("//0"      ).gpop_right('/', true), "");
    EXPECT_EQ(S("//0/"     ).gpop_right('/'      ), "/0/");
    EXPECT_EQ(S("//0/"     ).gpop_right('/', true), "");
    EXPECT_EQ(S("//0//"    ).gpop_right('/'      ), "/0//");
    EXPECT_EQ(S("//0//"    ).gpop_right('/', true), "");
    EXPECT_EQ(S("//0///"   ).gpop_right('/'      ), "/0///");
    EXPECT_EQ(S("//0///"   ).gpop_right('/', true), "");

    EXPECT_EQ(S("///0"      ).gpop_right('/'      ), "//0");
    EXPECT_EQ(S("///0"      ).gpop_right('/', true), "");
    EXPECT_EQ(S("///0/"     ).gpop_right('/'      ), "//0/");
    EXPECT_EQ(S("///0/"     ).gpop_right('/', true), "");
    EXPECT_EQ(S("///0//"    ).gpop_right('/'      ), "//0//");
    EXPECT_EQ(S("///0//"    ).gpop_right('/', true), "");
    EXPECT_EQ(S("///0///"   ).gpop_right('/'      ), "//0///");
    EXPECT_EQ(S("///0///"   ).gpop_right('/', true), "");

    EXPECT_EQ(S("/"        ).gpop_right('/'      ), "");
    EXPECT_EQ(S("/"        ).gpop_right('/', true), "");
    EXPECT_EQ(S("//"       ).gpop_right('/'      ), "/");
    EXPECT_EQ(S("//"       ).gpop_right('/', true), "");
    EXPECT_EQ(S("///"      ).gpop_right('/'      ), "//");
    EXPECT_EQ(S("///"      ).gpop_right('/', true), "");

    EXPECT_EQ(S(""         ).gpop_right('/'      ), "");
    EXPECT_EQ(S(""         ).gpop_right('/', true), "");
}

TEST(substr, basename)
{
    using S = csubstr;
    EXPECT_EQ(S("0/1/2").basename(), "2");
    EXPECT_EQ(S("0/1/2/").basename(), "2");
    EXPECT_EQ(S("0/1/2///").basename(), "2");
    EXPECT_EQ(S("/0/1/2").basename(), "2");
    EXPECT_EQ(S("/0/1/2/").basename(), "2");
    EXPECT_EQ(S("/0/1/2///").basename(), "2");
    EXPECT_EQ(S("///0/1/2").basename(), "2");
    EXPECT_EQ(S("///0/1/2/").basename(), "2");
    EXPECT_EQ(S("///0/1/2///").basename(), "2");
    EXPECT_EQ(S("/").basename(), "");
    EXPECT_EQ(S("//").basename(), "");
    EXPECT_EQ(S("///").basename(), "");
    EXPECT_EQ(S("////").basename(), "");
    EXPECT_EQ(S("").basename(), "");
}

TEST(substr, dirname)
{
    using S = csubstr;
    EXPECT_EQ(S("0/1/2").dirname(), "0/1/");
    EXPECT_EQ(S("0/1/2/").dirname(), "0/1/");
    EXPECT_EQ(S("/0/1/2").dirname(), "/0/1/");
    EXPECT_EQ(S("/0/1/2/").dirname(), "/0/1/");
    EXPECT_EQ(S("///0/1/2").dirname(), "///0/1/");
    EXPECT_EQ(S("///0/1/2/").dirname(), "///0/1/");
    EXPECT_EQ(S("/0").dirname(), "/");
    EXPECT_EQ(S("/").dirname(), "/");
    EXPECT_EQ(S("//").dirname(), "//");
    EXPECT_EQ(S("///").dirname(), "///");
    EXPECT_EQ(S("////").dirname(), "////");
    EXPECT_EQ(S("").dirname(), "");
}

TEST(substr, next_split)
{
    using S = csubstr;

    {
        S const n;
        typename S::size_type pos = 0;
        S ss;
        EXPECT_EQ(n.next_split(':', &pos, &ss), false);
        EXPECT_EQ(ss.empty(), true);
        EXPECT_EQ(n.next_split(':', &pos, &ss), false);
        EXPECT_EQ(ss.empty(), true);
        pos = 0;
        EXPECT_EQ(n.next_split(',', &pos, &ss), false);
        EXPECT_EQ(ss.empty(), true);
        EXPECT_EQ(n.next_split(',', &pos, &ss), false);
        EXPECT_EQ(ss.empty(), true);
    }

    {
        S const n("0");
        typename S::size_type pos = 0;
        S ss;
        EXPECT_EQ(n.next_split(':', &pos, &ss), true);
        EXPECT_EQ(ss.empty(), false);
        EXPECT_EQ(n.next_split(':', &pos, &ss), false);
        EXPECT_EQ(ss.empty(), true);
        EXPECT_EQ(n.next_split(':', &pos, &ss), false);
        EXPECT_EQ(ss.empty(), true);
        pos = 0;
        EXPECT_EQ(n.next_split(',', &pos, &ss), true);
        EXPECT_EQ(ss.empty(), false);
        EXPECT_EQ(n.next_split(',', &pos, &ss), false);
        EXPECT_EQ(ss.empty(), true);
        EXPECT_EQ(n.next_split(',', &pos, &ss), false);
        EXPECT_EQ(ss.empty(), true);
    }

    {
        S const n;
        typename S::size_type pos = 0;
        typename S::size_type count = 0;
        S ss;
        while(n.next_split(':', &pos, &ss))
        {
            ++count;
        }
        EXPECT_EQ(count, 0);
    }

    {
        S const n("0123456");
        typename S::size_type pos = 0;
        typename S::size_type count = 0;
        S ss;
        while(n.next_split(':', &pos, &ss))
        {
            switch(count)
            {
            case 0:
                EXPECT_EQ(ss.size(), n.size());
                EXPECT_EQ(ss.empty(), false);
                break;
            default:
                GTEST_FAIL();
                break;
            }
            ++count;
        }
        EXPECT_EQ(count, 1);
    }

    {
        S const n("0123456:");
        typename S::size_type pos = 0;
        typename S::size_type count = 0;
        S ss;
        while(n.next_split(':', &pos, &ss))
        {
            switch(count)
            {
            case 0:
                EXPECT_EQ(ss.size(), n.size()-1);
                EXPECT_EQ(ss.empty(), false);
                break;
            case 1:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                break;
            default:
                GTEST_FAIL();
                break;
            }
            ++count;
        }
        EXPECT_EQ(count, 2);
    }

    {
        S const n(":0123456:");
        typename S::size_type pos = 0;
        typename S::size_type count = 0;
        S ss;
        while(n.next_split(':', &pos, &ss))
        {
            switch(count)
            {
            case 0:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                break;
            case 1:
                EXPECT_EQ(ss.size(), n.size()-2);
                EXPECT_EQ(ss.empty(), false);
                break;
            case 2:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                break;
            default:
                GTEST_FAIL();
                break;
            }
            ++count;
        }
        EXPECT_EQ(count, 3);
    }

    {
        S const n(":");
        typename S::size_type pos = 0;
        typename S::size_type count = 0;
        S ss;
        while(n.next_split(':', &pos, &ss))
        {
            switch(count)
            {
            case 0:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                break;
            case 1:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                break;
            default:
                GTEST_FAIL();
                break;
            }
            ++count;
        }
        EXPECT_EQ(count, 2);
    }

    {
        S const n("01:23:45:67");
        typename S::size_type pos = 0;
        typename S::size_type count = 0;
        S ss;
        while(n.next_split(':', &pos, &ss))
        {
            switch(count)
            {
            case 0:
                EXPECT_EQ(ss.size(), 2);
                EXPECT_EQ(ss, "01");
                EXPECT_NE(ss, "01:");
                EXPECT_NE(ss, ":01:");
                EXPECT_NE(ss, ":01");
                break;
            case 1:
                EXPECT_EQ(ss.size(), 2);
                EXPECT_EQ(ss, "23");
                EXPECT_NE(ss, "23:");
                EXPECT_NE(ss, ":23:");
                EXPECT_NE(ss, ":23");
                break;
            case 2:
                EXPECT_EQ(ss.size(), 2);
                EXPECT_EQ(ss, "45");
                EXPECT_NE(ss, "45:");
                EXPECT_NE(ss, ":45:");
                EXPECT_NE(ss, ":45");
                break;
            case 3:
                EXPECT_EQ(ss.size(), 2);
                EXPECT_EQ(ss, "67");
                EXPECT_NE(ss, "67:");
                EXPECT_NE(ss, ":67:");
                EXPECT_NE(ss, ":67");
                break;
            default:
                GTEST_FAIL();
                break;
            }
            count++;
        }
        EXPECT_EQ(count, 4);
    }

    {
        const S n(":01:23:45:67:");
        typename S::size_type pos = 0;
        typename S::size_type count = 0;
        S ss;
        while(n.next_split(':', &pos, &ss))
        {
            switch(count)
            {
            case 0:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                break;
            case 1:
                EXPECT_EQ(ss.size(), 2);
                EXPECT_EQ(ss, "01");
                EXPECT_NE(ss, "01:");
                EXPECT_NE(ss, ":01:");
                EXPECT_NE(ss, ":01");
                break;
            case 2:
                EXPECT_EQ(ss.size(), 2);
                EXPECT_EQ(ss, "23");
                EXPECT_NE(ss, "23:");
                EXPECT_NE(ss, ":23:");
                EXPECT_NE(ss, ":23");
                break;
            case 3:
                EXPECT_EQ(ss.size(), 2);
                EXPECT_EQ(ss, "45");
                EXPECT_NE(ss, "45:");
                EXPECT_NE(ss, ":45:");
                EXPECT_NE(ss, ":45");
                break;
            case 4:
                EXPECT_EQ(ss.size(), 2);
                EXPECT_EQ(ss, "67");
                EXPECT_NE(ss, "67:");
                EXPECT_NE(ss, ":67:");
                EXPECT_NE(ss, ":67");
                break;
            case 5:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                break;
            default:
                GTEST_FAIL();
                break;
            }
            count++;
        }
        EXPECT_EQ(count, 6);
    }

    {
        const S n("::::01:23:45:67::::");
        typename S::size_type pos = 0;
        typename S::size_type count = 0;
        S ss;
        while(n.next_split(':', &pos, &ss))
        {
            switch(count)
            {
            case 0:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            case 1:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            case 2:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            case 3:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            case 4:
                EXPECT_EQ(ss.size(), 2);
                EXPECT_EQ(ss, "01");
                EXPECT_NE(ss, "01:");
                EXPECT_NE(ss, ":01:");
                EXPECT_NE(ss, ":01");
                break;
            case 5:
                EXPECT_EQ(ss.size(), 2);
                EXPECT_EQ(ss, "23");
                EXPECT_NE(ss, "23:");
                EXPECT_NE(ss, ":23:");
                EXPECT_NE(ss, ":23");
                break;
            case 6:
                EXPECT_EQ(ss.size(), 2);
                EXPECT_EQ(ss, "45");
                EXPECT_NE(ss, "45:");
                EXPECT_NE(ss, ":45:");
                EXPECT_NE(ss, ":45");
                break;
            case 7:
                EXPECT_EQ(ss.size(), 2);
                EXPECT_EQ(ss, "67");
                EXPECT_NE(ss, "67:");
                EXPECT_NE(ss, ":67:");
                EXPECT_NE(ss, ":67");
                break;
            case 8:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            case 9:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            case 10:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            case 11:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            default:
                GTEST_FAIL();
                break;
            }
            count++;
        }
        EXPECT_EQ(count, 12);
    }
}

TEST(substr, split)
{
    using S = csubstr;

    {
        S const n;
        {
            auto spl = n.split(':');
            auto beg = spl.begin();
            auto end = spl.end();
            EXPECT_EQ(beg, end);
        }
    }

    {
        S const n("foo:bar:baz");
        auto spl = n.split(':');
        auto beg = spl.begin();
        auto end = spl.end();
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(end->size(), 0);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        auto it = beg;
        EXPECT_EQ(it->size(), 3);
        EXPECT_EQ(*it, "foo");
        EXPECT_NE(it, end);
        EXPECT_EQ(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        ++it;
        EXPECT_EQ(it->size(), 3);
        EXPECT_EQ(*it, "bar");
        EXPECT_NE(it, end);
        EXPECT_NE(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        ++it;
        EXPECT_EQ(it->size(), 3);
        EXPECT_EQ(*it, "baz");
        EXPECT_NE(it, end);
        EXPECT_NE(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        ++it;
        EXPECT_EQ(it->size(), 0);
        EXPECT_EQ(it, end);
        EXPECT_NE(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        it = beg;
        EXPECT_EQ(it->size(), 3);
        EXPECT_EQ(*it, "foo");
        EXPECT_NE(it, end);
        EXPECT_EQ(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        it++;
        EXPECT_EQ(it->size(), 3);
        EXPECT_EQ(*it, "bar");
        EXPECT_NE(it, end);
        EXPECT_NE(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        it++;
        EXPECT_EQ(it->size(), 3);
        EXPECT_EQ(*it, "baz");
        EXPECT_NE(it, end);
        EXPECT_NE(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        it++;
        EXPECT_EQ(it->size(), 0);
        EXPECT_EQ(it, end);
        EXPECT_NE(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
    }

    {
        S const n("foo:bar:baz:");
        auto spl = n.split(':');
        auto beg = spl.begin();
        auto end = spl.end();
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(end->size(), 0);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        auto it = beg;
        EXPECT_EQ(it->size(), 3);
        EXPECT_EQ(*it, "foo");
        EXPECT_NE(it, end);
        EXPECT_EQ(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        ++it;
        EXPECT_EQ(it->size(), 3);
        EXPECT_EQ(*it, "bar");
        EXPECT_NE(it, end);
        EXPECT_NE(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        ++it;
        EXPECT_EQ(it->size(), 3);
        EXPECT_EQ(*it, "baz");
        EXPECT_NE(it, end);
        EXPECT_NE(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        ++it;
        EXPECT_EQ(it->size(), 0);
        EXPECT_EQ(*it, "");
        EXPECT_NE(it, end);
        EXPECT_NE(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        ++it;
        EXPECT_EQ(it->size(), 0);
        EXPECT_EQ(it, end);
        EXPECT_NE(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        //--------------------------
        it = beg;
        EXPECT_EQ(it->size(), 3);
        EXPECT_EQ(*it, "foo");
        EXPECT_NE(it, end);
        EXPECT_EQ(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        it++;
        EXPECT_EQ(it->size(), 3);
        EXPECT_EQ(*it, "bar");
        EXPECT_NE(it, end);
        EXPECT_NE(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        it++;
        EXPECT_EQ(it->size(), 3);
        EXPECT_EQ(*it, "baz");
        EXPECT_NE(it, end);
        EXPECT_NE(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        it++;
        EXPECT_EQ(it->size(), 0);
        EXPECT_EQ(*it, "");
        EXPECT_NE(it, end);
        EXPECT_NE(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
        it++;
        EXPECT_EQ(it->size(), 0);
        EXPECT_EQ(it, end);
        EXPECT_NE(it, beg);
        EXPECT_EQ(beg->size(), 3);
        EXPECT_EQ(*beg, "foo");
        EXPECT_NE(beg, end);
    }

    {
        S const n;
        auto s = n.split(':');
        // check that multiple calls to begin() always yield the same result
        EXPECT_EQ(*s.begin(), "");
        EXPECT_EQ(*s.begin(), "");
        EXPECT_EQ(*s.begin(), "");
        // check that multiple calls to end() always yield the same result
        auto e = s.end();
        EXPECT_EQ(s.end(), e);
        EXPECT_EQ(s.end(), e);
        //
        auto it = s.begin();
        EXPECT_EQ(*it, "");
        EXPECT_EQ(it->empty(), true);
        EXPECT_EQ(it->size(), 0);
        ++it;
        EXPECT_EQ(it, e);
    }

    {
        S const n("01:23:45:67");
        auto s = n.split(':');
        // check that multiple calls to begin() always yield the same result
        EXPECT_EQ(*s.begin(), "01");
        EXPECT_EQ(*s.begin(), "01");
        EXPECT_EQ(*s.begin(), "01");
        // check that multiple calls to end() always yield the same result
        auto e = s.end();
        EXPECT_EQ(s.end(), e);
        EXPECT_EQ(s.end(), e);
        EXPECT_EQ(s.end(), e);
        //
        auto it = s.begin();
        EXPECT_EQ(*it, "01");
        EXPECT_EQ(it->size(), 2);
        ++it;
        EXPECT_EQ(*it, "23");
        EXPECT_EQ(it->size(), 2);
        ++it;
        EXPECT_EQ(*it, "45");
        EXPECT_EQ(it->size(), 2);
        ++it;
        EXPECT_EQ(*it, "67");
        EXPECT_EQ(it->size(), 2);
        ++it;
        EXPECT_EQ(it, s.end());
    }

    {
        S const n;
        typename S::size_type count = 0;
        for(auto &ss : n.split(':'))
        {
            ++count;
        }
        EXPECT_EQ(count, 0);
    }

    {
        S const n("0123456");
        {
            auto spl = n.split(':');
            auto beg = spl.begin();
            auto end = spl.end();
            EXPECT_EQ(beg->size(), n.size());
            EXPECT_EQ(end->size(), 0);
        }
        typename S::size_type count = 0;
        for(auto &ss : n.split(':'))
        {
            switch(count)
            {
            case 0:
                EXPECT_EQ(ss.size(), n.size());
                EXPECT_EQ(ss.empty(), false);
                break;
            }
            ++count;
        }
        EXPECT_EQ(count, 1);
    }

    {
        S const n("foo:bar");
        typename S::size_type count = 0;
        for(auto &ss : n.split(':'))
        {
            switch(count)
            {
            case 0:
                EXPECT_EQ(ss.size(), 3);
                EXPECT_EQ(ss.empty(), false);
                EXPECT_EQ(ss, "foo");
                break;
            case 1:
                EXPECT_EQ(ss.size(), 3);
                EXPECT_EQ(ss.empty(), false);
                EXPECT_EQ(ss, "bar");
                break;
            }
            ++count;
        }
        EXPECT_EQ(count, 2);
    }

    {
        S const n("0123456:");
        typename S::size_type count = 0;
        for(auto &ss : n.split(':'))
        {
            switch(count)
            {
            case 0:
                EXPECT_EQ(ss.size(), n.size()-1);
                EXPECT_EQ(ss.empty(), false);
                break;
            case 1:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                break;
            }
            ++count;
        }
        EXPECT_EQ(count, 2);
    }

    {
        S const n(":0123456:");
        typename S::size_type count = 0;
        for(auto &ss : n.split(':'))
        {
            switch(count)
            {
            case 0:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                break;
            case 1:
                EXPECT_EQ(ss.size(), n.size()-2);
                EXPECT_EQ(ss.empty(), false);
                break;
            case 2:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                break;
            }
            ++count;
        }
        EXPECT_EQ(count, 3);
    }

    {
        S const n(":");
        typename S::size_type count = 0;
        for(auto &ss : n.split(':'))
        {
            switch(count)
            {
            case 0:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                break;
            case 1:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                break;
            }
            ++count;
        }
        EXPECT_EQ(count, 2);
    }

    {
        S const n("01:23:45:67");
        typename S::size_type count = 0;
        for(auto &ss : n.split(':'))
        {
            switch(count)
            {
            case 0:
                EXPECT_EQ(ss, "01");
                EXPECT_NE(ss, "01:");
                EXPECT_NE(ss, ":01:");
                EXPECT_NE(ss, ":01");
                break;
            case 1:
                EXPECT_EQ(ss, "23");
                EXPECT_NE(ss, "23:");
                EXPECT_NE(ss, ":23:");
                EXPECT_NE(ss, ":23");
                break;
            case 2:
                EXPECT_EQ(ss, "45");
                EXPECT_NE(ss, "45:");
                EXPECT_NE(ss, ":45:");
                EXPECT_NE(ss, ":45");
                break;
            case 3:
                EXPECT_EQ(ss, "67");
                EXPECT_NE(ss, "67:");
                EXPECT_NE(ss, ":67:");
                EXPECT_NE(ss, ":67");
                break;
            }
            count++;
        }
        EXPECT_EQ(count, 4);
    }

    {
        const S n(":01:23:45:67:");
        typename S::size_type count = 0;
        for(auto &ss : n.split(':'))
        {
            switch(count)
            {
            case 0:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                break;
            case 1:
                EXPECT_EQ(ss, "01");
                EXPECT_NE(ss, "01:");
                EXPECT_NE(ss, ":01:");
                EXPECT_NE(ss, ":01");
                break;
            case 2:
                EXPECT_EQ(ss, "23");
                EXPECT_NE(ss, "23:");
                EXPECT_NE(ss, ":23:");
                EXPECT_NE(ss, ":23");
                break;
            case 3:
                EXPECT_EQ(ss, "45");
                EXPECT_NE(ss, "45:");
                EXPECT_NE(ss, ":45:");
                EXPECT_NE(ss, ":45");
                break;
            case 4:
                EXPECT_EQ(ss, "67");
                EXPECT_NE(ss, "67:");
                EXPECT_NE(ss, ":67:");
                EXPECT_NE(ss, ":67");
                break;
            case 5:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                break;
            }
            count++;
        }
        EXPECT_EQ(count, 6);
    }

    {
        const S n("::::01:23:45:67::::");
        typename S::size_type count = 0;
        for(auto &ss : n.split(':'))
        {
            switch(count)
            {
            case 0:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            case 1:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            case 2:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            case 3:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            case 4:
                EXPECT_EQ(ss, "01");
                EXPECT_NE(ss, "01:");
                EXPECT_NE(ss, ":01:");
                EXPECT_NE(ss, ":01");
                break;
            case 5:
                EXPECT_EQ(ss, "23");
                EXPECT_NE(ss, "23:");
                EXPECT_NE(ss, ":23:");
                EXPECT_NE(ss, ":23");
                break;
            case 6:
                EXPECT_EQ(ss, "45");
                EXPECT_NE(ss, "45:");
                EXPECT_NE(ss, ":45:");
                EXPECT_NE(ss, ":45");
                break;
            case 7:
                EXPECT_EQ(ss, "67");
                EXPECT_NE(ss, "67:");
                EXPECT_NE(ss, ":67:");
                EXPECT_NE(ss, ":67");
                break;
            case 8:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            case 9:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            case 10:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            case 11:
                EXPECT_EQ(ss.size(), 0);
                EXPECT_EQ(ss.empty(), true);
                EXPECT_NE(ss, "::");
                break;
            }
            count++;
        }
        EXPECT_EQ(count, 12);
    }
}


//-----------------------------------------------------------------------------
TEST(substr, copy_from)
{
    char buf[128] = {0};
    substr s = buf;
    EXPECT_EQ(s.size(), sizeof(buf)-1);
    EXPECT_NE(s.first(3), "123");
    s.copy_from("123");
    EXPECT_EQ(s.first(3), "123");
    EXPECT_EQ(s.first(6), "123\0\0\0");
    s.copy_from("+++", 3);
    EXPECT_EQ(s.first(6), "123+++");
    EXPECT_EQ(s.first(9), "123+++\0\0\0");
    s.copy_from("456", 6);
    EXPECT_EQ(s.first(9), "123+++456");
    EXPECT_EQ(s.first(12), "123+++456\0\0\0");
    s.copy_from("***", 3);
    EXPECT_EQ(s.first(9), "123***456");
    EXPECT_EQ(s.first(12), "123***456\0\0\0");

    // make sure that it's safe to pass source strings that don't fit
    // in the remaining destination space
    substr ss = s.first(9);
    ss.copy_from("987654321", 9); // should be a no-op
    EXPECT_EQ(s.first(12), "123***456\0\0\0");
    ss.copy_from("987654321", 6);
    EXPECT_EQ(s.first(12), "123***987\0\0\0");
    ss.copy_from("987654321", 3);
    EXPECT_EQ(s.first(12), "123987654\0\0\0");
    ss.first(3).copy_from("987654321");
    EXPECT_EQ(s.first(12), "987987654\0\0\0");
}


//-----------------------------------------------------------------------------
void do_test_reverse(substr s, csubstr orig, csubstr expected)
{
    EXPECT_EQ(s, orig);
    s.reverse();
    EXPECT_EQ(s, expected);
    s.reverse();
    EXPECT_EQ(s, orig);
}

TEST(substr, reverse)
{
    char buf[] = "0123456789";
    do_test_reverse(buf, "0123456789", "9876543210");
    do_test_reverse(buf, "0123456789", "9876543210");

    // in the middle
    substr s = buf;
    s.sub(2, 2).reverse();
    EXPECT_EQ(s, "0132456789");
    s.sub(2, 2).reverse();
    EXPECT_EQ(s, "0123456789");

    s.sub(4, 2).reverse();
    EXPECT_EQ(s, "0123546789");
    s.sub(4, 2).reverse();
    EXPECT_EQ(s, "0123456789");

    // at the beginning
    s.first(3).reverse();
    EXPECT_EQ(s, "2103456789");
    s.first(3).reverse();
    EXPECT_EQ(s, "0123456789");

    // at the end
    s.last(3).reverse();
    EXPECT_EQ(s, "0123456987");
    s.last(3).reverse();
    EXPECT_EQ(s, "0123456789");
}


//-----------------------------------------------------------------------------
TEST(substr, erase)
{
    char buf[] = "0123456789";

    substr s = buf;
    EXPECT_EQ(s.len, s.size());
    EXPECT_EQ(s.len, 10);
    EXPECT_EQ(s, "0123456789");

    substr ss = s.first(6);
    EXPECT_EQ(ss.len, 6);
    for(size_t i = 0; i <= ss.len; ++i)
    {
        ss.erase(i, 0); // must be a no-op
        EXPECT_EQ(s, "0123456789");
        ss.erase_range(i, i); // must be a no-op
        EXPECT_EQ(s, "0123456789");
        ss.erase(ss.len-i, i); // must be a no-op
        EXPECT_EQ(s, "0123456789");
    }

    substr r;
    ss = ss.erase(0, 1);
    EXPECT_EQ(ss.len, 5);
    EXPECT_EQ(ss, "12345");
    EXPECT_EQ(s, "1234556789");
    ss = ss.erase(0, 2);
    EXPECT_EQ(ss.len, 3);
    EXPECT_EQ(ss, "345");
    EXPECT_EQ(s, "3454556789");

    csubstr s55 = s.sub(4, 2);
    ss = s.erase(s55);
    EXPECT_EQ(s, "3454678989");
}


//-----------------------------------------------------------------------------
TEST(substr, replace_all)
{
    char buf[] = "0.1.2.3.4.5.6.7.8.9";

    substr s = buf;
    bool ret;

    ret = s.replace_all('+', '.');
    EXPECT_FALSE(ret);

    ret = s.replace_all('.', '.');
    EXPECT_TRUE(ret);
    EXPECT_EQ(s, "0.1.2.3.4.5.6.7.8.9");

    ret = s.replace_all('.', '+');
    EXPECT_TRUE(ret);
    EXPECT_EQ(s, "0+1+2+3+4+5+6+7+8+9");
}

} // namespace c4
