#!/usr/bin/env python

import sys
from distutils.core import setup, Extension

if sys.version_info.major==3:
    boost_python_lib='boost_python3'
else:
    boost_python_lib='boost_python'

module1=Extension('cwt1d.cwtcore',
                  include_dirs=['../cwt1d/','../fftw_blitz'],
                  libraries=[boost_python_lib,'gsl','gslcblas','fftw3f','fftw3_threads','fftw3f_threads'],
                  #library_dirs=['/usr/include/lib'],
                  sources=['../fftw_blitz/fftw_blitz.cpp','pycwt1d.cpp'])

setup(name='cwtcore',
      packages=['cwt1d'],
      version='1.0',
      ext_modules=[module1])
