
#!/bin/bash

import os

basedir = os.path.dirname(__file__)

os.system(f'cd {basedir}; cp Makefile_emscripten ../')
os.system(f'cd {basedir}; rm -f index.html')
os.system(f'cd {basedir}; cd ..; emmake make -f Makefile_emscripten clean')
os.system(f'cd {basedir}; cd ..; emmake make -f Makefile_emscripten -j$(nproc)')
print('\n=========================================================================')
print('If the compilation succeed, run the following command in the root directory to test the webpage:')
print(f'$ emrun index.html')

