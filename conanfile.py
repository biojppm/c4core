import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.files import copy


# fast_float currently at v8.2.10, but the latest revision available on ConanCenter is 8.1.0
# (and tools/amalgamate.py::FASTFLOAT_VERSION)
FASTFLOAT_REF = "fast_float/[>=8.1 <8.3]"
FASTFLOAT_VERSION = "8.2.10"

class C4coreConan(ConanFile):
    name = "c4core"
    version = "0.4.0"
    license = "MIT"
    url = "https://github.com/biojppm/c4core"
    homepage = "https://github.com/biojppm/c4core"
    description = "Multiplatform low-level C++ utilities"
    topics = ("cpp", "utilities", "low-latency", "charconv")

    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "with_fastfloat": [True, False],
        "with_debugbreak": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "with_fastfloat": True,
        "with_debugbreak": True,
    }

    # NOTE: the `cmake` helper and `debugbreak` are still git submodules, so run
    #   git submodule update --init --recursive
    # before `conan create`/`conan export` so these sources are on disk.
    exports_sources = (
        "CMakeLists.txt",
        "compat.cmake",
        "cmake/*",
        "src/*",
        "test/*",
        "tools/*",
        "changelog/*",
        "LICENSE.txt",
        "LICENSE-BOOST.txt",
        "README.md",
    )

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        if self.options.with_fastfloat:
            # transitive_headers: c4/charconv.hpp (a public header) pulls in
            # <fast_float/fast_float.h>, so consumers need fast_float's headers.
            self.requires(FASTFLOAT_REF, transitive_headers=True)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.cache_variables["C4CORE_WITH_FASTFLOAT"] = bool(self.options.with_fastfloat)
        tc.cache_variables["C4CORE_NO_DEBUG_BREAK"] = not bool(self.options.with_debugbreak)
        tc.cache_variables["C4CORE_INSTALL"] = True
        # keep the dev/test harness off for a plain library build; flip to True
        # (and add googletest etc.) if you want to build + run the unit tests
        tc.cache_variables["C4CORE_DEV"] = False
        tc.generate()

        deps = CMakeDeps(self)
        if self.options.with_fastfloat:
            # Pin the generated CMake config + target names so CMakeLists.txt can
            # rely on find_package(fast_float) / fast_float::fast_float, no matter
            # how the ConanCenter recipe happens to name them.
            deps.set_property("fast_float", "cmake_file_name", "fast_float")
            deps.set_property("fast_float", "cmake_target_name", "fast_float::fast_float")
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE.txt", src=self.source_folder,
             dst=os.path.join(self.package_folder, "licenses"))
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "c4core")
        self.cpp_info.set_property("cmake_target_name", "c4core::c4core")
        self.cpp_info.libs = ["c4core"]
        if self.options.with_fastfloat:
            # propagate the fast_float dependency to consumers of c4core
            self.cpp_info.requires = ["fast_float::fast_float"]
        else:
            self.cpp_info.defines.append("C4CORE_NO_FAST_FLOAT")
        if not self.options.with_debugbreak:
            self.cpp_info.defines.append("C4_NO_DEBUG_BREAK")
