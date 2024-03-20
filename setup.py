#****************************************************************************
#* setup.py for vsc-solvers
#****************************************************************************
import os
from setuptools import Extension, find_namespace_packages

version="0.0.1"

proj_dir = os.path.dirname(os.path.abspath(__file__))

try:
    import sys
    sys.path.insert(0, os.path.join(proj_dir, "python/vsc_solvers"))
    from __build_num__ import BUILD_NUM
    version += ".%s" % str(BUILD_NUM)
except ImportError:
    pass

isSrcBuild = False

try:
    from ivpm.setup import setup
    isSrcBuild = os.path.isdir(os.path.join(proj_dir, "src"))
    print("vsc-solvers: running IVPM SrcBuild")
except ImportError as e:
    from setuptools import setup
    print("vsc-solvers: running non-src build")

if isSrcBuild:
    incdir = os.path.join(proj_dir, "src", "include")
else:
    incdir = os.path.join(proj_dir, "python/vsc_dm/share/include")

vsc_solvers_dir = proj_dir

ext = Extension("vsc_solvers.core",
            sources=[
                os.path.join(vsc_solvers_dir, 'python', "core.pyx"), 
            ],
            language="c++",
            include_dirs=[incdir]
        )
ext.cython_directives={'language_level' : '3'}

setup_args = dict(
  name = "vsc-solvers",
  version=version,
  packages=['vsc_solvers'],
  package_dir = {'' : 'python'},
  author = "Matthew Ballance",
  author_email = "matt.ballance@gmail.com",
  description = ("Core Verification Stimulus and Coverage library"),
  long_description="""
  Provides a library for constrained randomization and coverage collection
  """,
  license = "Apache 2.0",
  keywords = ["SystemVerilog", "Verilog", "RTL", "Python"],
  url = "https://github.com/vsc-tools/vsc-solvers",
  install_requires=[
    'vsc-dm',
  ],
  setup_requires=[
    'setuptools_scm',
    'cython'
  ],
  ext_modules=[ ext ]
)

if isSrcBuild:
    setup_args["ivpm_extdep_pkgs"] = ["vsc-dm", "debug-mgr"]
    setup_args["ivpm_extra_data"] = {
        "vsc_solvers": [
            ("src/include", "share")
        ]
    }

setup(**setup_args)

