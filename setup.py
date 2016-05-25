from distutils.core import setup, Extension
from Cython.Build import cythonize

setup(ext_modules = cythonize(Extension(
            "consys",                                # the extension name
            sources=["consys.pyx", "ConnectedSystem.cxx", ], # the Cython source and
                                                  # additional C++ source files
            libraries=["armadillo"],
            language="c++",                        # generate and compile C++ code
      )))
