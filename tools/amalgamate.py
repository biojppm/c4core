import re
import os
from os.path import abspath, dirname
import sys
import subprocess

projdir = abspath(dirname(dirname(__file__)))
sys.path.insert(0, f"{projdir}/cmake")
from amalgamate_utils import *


fastfloatdir = f"{projdir}/src/c4/ext/fast_float"
subprocess.run([
    sys.executable,
    f"{fastfloatdir}/script/amalgamate.py",
    "--license", "MIT",
    "--output", f"{fastfloatdir}/../fast_float_all.h"
], cwd=fastfloatdir).check_returncode()


repo = "https://github.com/biojppm/c4core"
defmacro = "C4CORE_SINGLE_HDR_DEFINE_NOW"
headers = [
    cmttext(f"""
c4core - C++ utilities

{repo}

DO NOT EDIT. This file is generated automatically.
This is an amalgamated single-header version of the library.

INSTRUCTIONS:
  - Include at will in any header of your project
  - In one (and only one) of your project source files, #define
    {defmacro} and then include this header. This will enable the
    function and class definitions in the header file.
"""),
    cmtfile("LICENSE.txt"),
    "src/c4/export.hpp",
    "src/c4/preprocessor.hpp",
    "src/c4/platform.hpp",
    "src/c4/cpu.hpp",
    "src/c4/compiler.hpp",
    "src/c4/language.hpp",
    "src/c4/types.hpp",
    "src/c4/config.hpp",
    hdrfile("src/c4/ext/debugbreak/debugbreak.h", "c4/ext/debugbreak/debugbreak.h", "DEBUG_BREAK_H"),
    "src/c4/error.hpp",
    "src/c4/memory_util.hpp",
    "src/c4/memory_resource.hpp",
    "src/c4/ctor_dtor.hpp",
    "src/c4/allocator.hpp",
    "src/c4/char_traits.hpp",
    "src/c4/hash.hpp",
    "src/c4/szconv.hpp",
    "src/c4/blob.hpp",
    "src/c4/substr_fwd.hpp",
    "src/c4/substr.hpp",
    injfile("src/c4/ext/fast_float_all.h", "c4/ext/fast_float_all.h"),
    "src/c4/ext/fast_float.hpp",
    "src/c4/std/vector_fwd.hpp",
    "src/c4/std/string_fwd.hpp",
    "src/c4/std/std_fwd.hpp",
    "src/c4/charconv.hpp",
    "src/c4/format.hpp",
    "src/c4/enum.hpp",
    "src/c4/bitmask.hpp",
    "src/c4/span.hpp",
    "src/c4/type_name.hpp",
    "src/c4/base64.hpp",
    ignfile("src/c4/std/std.hpp"), # this is an umbrella include
    "src/c4/std/string.hpp",
    "src/c4/std/vector.hpp",
    "src/c4/std/tuple.hpp",
    "src/c4/time.hpp",
    "src/c4/ext/rng/rng.hpp",
    "src/c4/ext/sg14/inplace_function.h",
    ignfile("src/c4/common.hpp"),
    ignfile("src/c4/c4_push.hpp"),
    ignfile("src/c4/c4_pop.hpp"),
    ignfile("src/c4/restrict.hpp"),
    ignfile("src/c4/unrestrict.hpp"),
    "src/c4/language.cpp",
    "src/c4/format.cpp",
    "src/c4/memory_util.cpp",
    "src/c4/char_traits.cpp",
    "src/c4/memory_resource.cpp",
    "src/c4/base64.cpp",
    injcode("#define C4_WINDOWS_POP_HPP_"),
    "src/c4/windows_push.hpp",
    "src/c4/windows.hpp",
    "src/c4/windows_pop.hpp", # do NOT include this before windows.hpp
    "src/c4/time.cpp",
    "src/c4/error.cpp",
]

result = catfiles(headers,
                  projdir,
                  # comment out lines with these patterns:
                  include_regexes=[
                      re.compile(r'^\s*#\s*include "(c4/.*)".*$'),
                      re.compile(r'^\s*#\s*include <(c4/.*)>.*$'),
                  ],
                  definition_macro=defmacro,
                  repo=repo,
                  result_incguard="_C4CORE_SINGLE_HEADER_AMALGAMATED_HPP_")

result_with_only_first_includes = include_only_first(result)

output_name = sys.argv[1]  # FIXME
os.makedirs(os.path.dirname(output_name), exist_ok=True)
with open(output_name, "w") as output:
    output.write(result_with_only_first_includes)
