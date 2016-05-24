from distutils.core import setup, Extension
from Cython.Build import cythonize

setup(ext_modules = cythonize(Extension(
            "consys",                                # the extension name
            sources=["consys.pyx", "ConnectedSystem.cxx", ], # the Cython source and
                                                  # additional C++ source files
            library_dirs = ['/usr/lib', '/usr/local/lib', '/Users/berend/Documents/Coding/armadillo-6.700.7/include'],
            libraries=["armadillo","random"],
            language="c++",                        # generate and compile C++ code
      )))
