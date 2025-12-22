from setuptools import setup, Extension, find_packages

#with open('requirements.txt') as f:
#    requirements = f.read().splitlines()


setup(
    name='mechasuite',
    version='0.1',
    packages=find_packages('.'),
    #install_requires=requirements,
    scripts=["mechasuite/mechadata.py", "mechasuite/mechakinetics.py"],
    setup_requires=[
     #   'setuptools>=18.0',  # automatically handles Cython extensions
     #   'PyOpenGL',
     #   'matplotlib',
     #   'scipy',
     #   'pyyaml',
     #   'openpyxl',
     #   'PyQt5',
     #   'numpy'
    ],

)

