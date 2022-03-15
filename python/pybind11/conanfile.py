from conans import ConanFile, CMake, tools


# file download from https://gitlab.lrde.epita.fr/olena/conan-pybind11/-/blob/dev/conanfile.py

class Pybind11(ConanFile):
    name = "pybind11"
    version = "2.2.4"
    description = "Seamless operability between C++11 and Python"
    author = "Michaël Roynard <michael.roynard@lrde.epita.fr>"
    url = "https://gitlab.lrde.epita.fr/olena/conan-pybind11"
    homepage = "https://github.com/pybind/pybind11"
    license = "https://github.com/pybind/pybind11/blob/master/LICENSE"
    exports = ["LICENSE"]
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [
        True, False], "build_tests": [True, False]}
    default_options = "shared=False", "fPIC=True", "build_tests=True"
    generators = "cmake"

    def configure(self):
        if self.settings.os == "Windows":
            self.options.remove("fPIC")

    def source(self):
        git = tools.Git()
        git.clone("{}.git".format(self.homepage))
        git.checkout("v{}".format(self.version))

    def get_cmake_config(self):
        cmake = CMake(self)

        if not self.options.build_tests:
            cmake.definitions["BUILD_TESTING"] = "OFF"
        else:
            cmake.definitions["DOWNLOAD_CATCH"] = "ON"

        cmake.configure()
        return cmake

    def build(self):
        cmake = self.get_cmake_config()
        cmake.build()

        if self.options.build_tests:
            # python tests trigger internal errors
            # cmake.build(target="check")
            # cmake.build(target="pytest")
            cmake.build(target="cpptest")

    def package(self):
        cmake = self.get_cmake_config()
        cmake.install()

    def package_id(self):
        self.info.header_only()

    def package_info(self):
        self.cpp_info.includedirs = ["include"]
