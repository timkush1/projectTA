from setuptools import Extension, setup

module = Extension("symnmf", sources=['symnmf.c', 'symnmfmodule.c'])
setup(name='symnmf',
      version='1.0',
      description='Python wrapper for custom C symnmf module extension',
      ext_modules=[module])

# python3 setup.py build_ext --inplace
# python3 symnmf.py 2 symnmf input_1.txt

# https://github.com/timkush1/software-project.git

# git remote add origin https://github.com/timkush1/software-project.git
# git push -u origin master