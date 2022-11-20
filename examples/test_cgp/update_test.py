import os
import re
from datetime import date

def get_header_file(files):
    for f in files:
        if f.endswith('.hpp'):
            return f
    print('Could not find header file')
    exit(1)

def get_test_functions(path_to_root):
    test_functions = []
    import_path = []
    for root, dirs, files in os.walk(path_to_root):
        for d in dirs:
            if d=='test':
                
                local_dir = root+'/test/'
                
                file_test = get_header_file(os.listdir(local_dir))
                path_file_test = local_dir+file_test

                import_path.append('cgp/'+path_file_test[len(path_to_root):])
                
                with open(path_file_test,'r') as fid:
                    txt = fid.read()
                    
                match = re.findall(r'void (test_.*\(\));',txt)
                for m in match:
                    test_functions.append(m)

    return (test_functions, import_path)

def generate_source_file(test_functions, import_path):

    txt = """
{{ comments }}
#include "cgp/cgp.hpp"
#include <iostream> 

{{ includes }}

using namespace cgp;

int main(int, char* argv[])
{
	std::cout << "Run " << argv[0] << std::endl;

{{ test_functions }}

	return 0;
}
"""
    txt_import = ''
    for f in import_path:
        txt_import += f'#include "{f}"\n'

    txt_functions = ''
    for t in test_functions:
        txt_functions += f'\tcgp_test::{t};\n'

    txt_comments = '// Automatically generated file using script update_test.py\n'
    txt_comments += '// Last generation on: '+str(date.today())+'\n'


    txt = txt.replace('{{ includes }}',txt_import)
    txt = txt.replace('{{ test_functions }}',txt_functions)
    txt = txt.replace('{{ comments }}',txt_comments)

    return txt

if __name__=='__main__':

    path_to_root = '../../library/cgp/'
    test_functions, import_path = get_test_functions(path_to_root)
    source_file = generate_source_file(test_functions, import_path)
    print(source_file)

    with open('src/main.cpp','w') as fid:
        fid.write(source_file)

    print('\nFile written in src/main.cpp')

            
            