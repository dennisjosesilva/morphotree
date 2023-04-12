from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps

class Morphotree(ConanFile):
  name = "morphotree"
  version = "0.0.1"

  # Optinal metadata 
  author = "dennisjosesilva"
  license = "<Put the package license here>"
  description = "A Morphological tree prototyping library"
  url = "https://github.com/dennisjosesilva/morphotree"
  topics = ("morphological tree", "image processing", "mathematical morphology")

  # Binary configuration
  settings = "os", "compiler", "build_type", "arch"
  options = {"shared": [True, False], "fPIC": [True, False]}
  default_options = {"shared": False, "fPIC": True}
  
  exports_sources = "src/*.cpp", "src/CMakeLists.txt", "!src/main.cpp", "include/*.hpp"

  def config_options(self):
    if self.settings.os == "Windows":
      self.options.rm_safe("fPIC")
  
  def configure(self):
    if self.options.shared:
      self.options.rm_safe("fPIC")

  def layout(self):
    cmake_layout(self)

  def generate(self):
    deps = CMakeDeps(self)
    deps.generate()
    tc = CMakeToolchain(self)
    tc.generate()

  def build(self):
    cmake = CMake(self)
    cmake.configure(build_script_folder="src")
    cmake.build()

  def package(self):
    cmake = CMake(self)
    cmake.install()

  def package_info(self):
    self.cpp_info.libs = ["morphotree"]