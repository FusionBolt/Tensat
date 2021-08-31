from conans import ConanFile, CMake, tools

# (key, values, default value)
CMakeOptions = [("TensatTest", [True, False], "False")]

class TensatConan(ConanFile):
    name = "Tensat"
    version = "0.0.1"
    license = "MIT"
    author = "FusionBolt <FusionBolt@gmail.com>"
    url = "https://github.com/FusionBolt/Tensat"
    description = "egg C++ version"
    settings = "os", "compiler", "build_type", "arch"

    options = {"shared": [True, False], "fPIC": [True, False]}
    options.update({ cmakeOption[0] : cmakeOption[1] for cmakeOption in CMakeOptions })

    default_options = ["shared=False", "fPIC=True"]
    default_options.extend([ f"{cmakeOption[0]}={cmakeOption[2]}" for cmakeOption in CMakeOptions ])
    default_options = tuple(default_options)

    generators = "cmake"

    no_copy_source = True
    exports_sources = "CMakeLists.txt", "src*"

    def requirements(self):
        self.requires("taocpp-pegtl/3.2.1@")
        if self.options.TensatTest:
            # require a test package
            pass

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure_cmake(self):
        cmake = CMake(self)

        for cmakeOption in CMakeOptions:
            cmake.definitions[cmakeOption[0]] = getattr(self.options, cmakeOption[0])

        cmake.configure()
        return cmake

    def build(self):
        cmake = self.configure_cmake()
        cmake.build()

    def package(self):
        cmake = self.configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)