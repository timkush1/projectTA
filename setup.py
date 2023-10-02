from setuptools import Extension, setup

module = Extension("symnmf", sources=['symnmf.c', 'symnmfmodule.c'])
setup(name='symnmf',
      version='1.0',
      description='Python wrapper for custom C symnmf module extension',
      ext_modules=[module])

# python3 setup.py build_ext --inplace
# python3 symnmf.py 3 symnmf input_1.txt

#make
#./symnmf sym input_1.txt

#valgrind --leak-check=full --show-leak-kinds=all ./symnmf sym input_1.txt

#python3 analysis.py 3 input_1.txt

# https://github.com/timkush1/projectTA.git

# git remote add origin https://github.com/timkush1/projectTA.git
# git push -u origin master