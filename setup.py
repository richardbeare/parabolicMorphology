# -*- coding: utf-8 -*-
from __future__ import print_function
from os import sys

try:
    from skbuild import setup
except ImportError:
    print('scikit-build is required to build from source.', file=sys.stderr)
    print('Please run:', file=sys.stderr)
    print('', file=sys.stderr)
    print('  python -m pip install scikit-build')
    sys.exit(1)

setup(
    name='itk-parabolicmorphology',
    version='1.2.0',
    author='Richard Beare',
    author_email='Richard.Beare@med.monash.edu.au',
    packages=['itk'],
    package_dir={'itk': 'itk'},
    download_url=r'https://github.com/InsightSoftwareConsortium/ITKParabolicMorphology',
    description=r'ITK classes for mathematical morphology using parabolic structuring functions',
    long_description='itk-parabolicmorphology provides mathematical '
                     'morphological erosion and dilation filters using '
                     'parabolic structuring functions.\n'
                     'Please refer to:\n'
                     'R. Beare, "Morphology with parabolic structuring elements.", '
                     'Insight Journal, January-June 2008, https://hdl.handle.net/1926/1370.',
    classifiers=[
        "License :: OSI Approved :: Apache Software License",
        "Programming Language :: Python",
        "Programming Language :: C++",
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Intended Audience :: Education",
        "Intended Audience :: Healthcare Industry",
        "Intended Audience :: Science/Research",
        "Topic :: Scientific/Engineering",
        "Topic :: Scientific/Engineering :: Medical Science Apps.",
        "Topic :: Scientific/Engineering :: Information Analysis",
        "Topic :: Software Development :: Libraries",
        "Operating System :: Android",
        "Operating System :: Microsoft :: Windows",
        "Operating System :: POSIX",
        "Operating System :: Unix",
        "Operating System :: MacOS"
        ],
    license='Apache',
    keywords='ITK Parabolic Morphology Erosion Dilation',
    url=r'https://github.com/InsightSoftwareConsortium/ITKParabolicMorphology',
    install_requires=[
        r'itk>=5.3.0'
    ]
    )
