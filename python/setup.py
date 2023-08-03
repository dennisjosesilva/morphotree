import os
import re 
import sys
import sysconfig
import platform
import subprocess

from pathlib import Path

from distutils.version import LooseVersion
from setuptools import setup, Extension 
from setuptools.command.build_ext import build_ext
import setuptools

class CMakeExtenstion(Extension):
  def __init__(self, name, sourcedir=""):
    Extension.__init__(self, name, sources=[])
    self.sourcedir = os.path.abspath(sourcedir)

class CMakeBuild(build_ext):
  def run(self):
    try:
      out = subprocess.check_output(["cmake", "--version"])
    except:
      raise RuntimeError("CMake must be installed to build the following extensions: " + 
        ",".join(e.name for e in self.extensions))
    
    if platform.system() == "Windows":
      cmake_version = LooseVersion(re.search(r"versions\s*([\d.]+)", out.decode()).group(1))

      if cmake_version < "3.1.0":
        raise RuntimeError("CMake >= 3.1.0 is required on Windows")

    for ext in self.extensions:
      self.build_extension(ext)

  def build_extension(self, ext):
    extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
    cmake_args = ["-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=" + extdir, 
                  "-DTYPE_EXECUTABLE=" + sys.executable]

    self.extdir = extdir

    cfg = "Debug" if self.debug else "Release"
    build_args = ["--config", cfg]
    
    if platform.system() == "Windows":
      cmake_args += ["-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}".format(cfg.upper(), extdir)]
      if sys.maxsize > 2**32:
        cmake_args += ["-A", "x64"]
      build_args += ["--", "/m"]
    else:
      cmake_args += ["-DCMAKE_BUILD_TYPE=" + cfg]      
      build_args += ["--", "-j2"]

    env = os.environ.copy()
    env["CXXFLAGS"] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get("CXXFLAGS", ""), self.distribution.get_version())

    if not os.path.exists(self.build_temp):
      os.makedirs(self.build_temp)

    subprocess.check_call(["cmake", ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
    subprocess.check_call(["cmake", "--build", "."] + build_args, cwd=self.build_temp)
    self.move_output(ext)
    print()

  def move_output(self, ext):
    extdir = Path(self.extdir).resolve()
    dest_path = Path(self.get_ext_fullpath(ext.name)).resolve()
    source_path = extdir / self.get_ext_filename(ext.name)
    dest_directory = dest_path.parents[0]
    dest_directory.mkdir(parents=True, exist_ok=True)
    self.copy_file(source_path, dest_path)

setup(
  name="morphotreepy",
  version="0.0.1",
  author="Dênnis José da Silva",
  author_email="dennisjosesilva@gmail.com",
  description="A simple library for morphological tree filter prototyping image.",
  long_description="",
  ext_modules=[CMakeExtenstion(name="morphotreepy")],
  cmdclass=dict(build_ext=CMakeBuild),
  zip_safe=False
)