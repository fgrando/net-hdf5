#!/bin/python3

import os
import logging

def setup_log():
    logger = logging.getLogger('generator')
    file_log_handler = logging.FileHandler('generator.log')
    logger.addHandler(file_log_handler)
    stderr_log_handler = logging.StreamHandler()
    logger.addHandler(stderr_log_handler)
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    file_log_handler.setFormatter(formatter)
    stderr_log_handler.setFormatter(formatter)

import datetime
def timestamp():
    now = datetime.datetime.now()
    timestamp = now.strftime('%Y-%m-%d_%H:%M:%S'.format(now))
    return timestamp


class Code_File():
    def __init__(self, name):
        self.name = name
        self.extension = 'dat'
        self.changelog = f'generated from {name} on {timestamp()}'
        self.includes = []
        self.defines = []
        self.macros = []
        self.types = []
        self.functions = []

    def get_name(self):
        return f'{self.name}.{self.extension}'

    def add_include(self, include):
        self.includes.append(f'#include {include}')

    def add_define(self, define):
        self.defines.append(f'#define {define}')

    def add_type(self, tp):
        self.types.append(tp)

    def add_macro(self, macro):
        self.macros.append(f'#define {macro}')

    def add_function(self, func):
        self.functions.append(func)

    def save(self, folder):
        name = os.path.join(folder, f'{self.name}.{self.extension}')
        with open(name, 'w') as f:
            f.write(str(self))

    def __str__(self):
        code = ''
        code += f'/*\n{self.changelog}\n*/\n\n'
        for group in [self.includes, self.defines, self.macros, self.types, self.functions]:
            for item in group:
                code += f'{item}\n'
            if len(group) > 0:
                code += '\n\n'
        return code



class Header_File(Code_File):
    def __init__(self, name):
        super().__init__(name)
        self.include_guard = f'{name.upper()}_H_INCLUDED_'
        self.extension = 'h'

    def __str__(self):
        code = ''
        code += f'#ifndef {self.include_guard}\n'
        code += f'#define {self.include_guard}\n'
        code += '\n\n'
        code += super().__str__()
        code += '\n\n'
        code += f'#endif /* {self.include_guard} */\n'
        return code


class Source_File(Code_File):
    def __init__(self, name):
        super().__init__(name)
        self.extension = 'c'
        self.global_vars = []
        self.local_vars = []
        self.function_bodies = []

    def add_function_body(self, body):
        self.function_bodies.append(body)

    def add_global_var(self, var):
        self.global_vars.append(var)

    def add_local_var(self, var):
        self.local_vars.append(var)




    def __str__(self):
        code = ''
        code += super().__str__()
        code += '\n'
        for group in [self.global_vars, self.local_vars]:
            for item in group:
                code += f'{item}\n'
            if len(group) > 0:
                code += '\n\n'
        for func in self.function_bodies:
            code += f'{func}\n\n'
        return code


if __name__ == '__main__':
    setup_log()
    b = Header_File('myheader')
    b.add_include('<stdio.h>')
    b.add_define('MYVAR 1')
    b.add_type('typedef struct { int i; } my_struct_type_t;')
    b.add_type('typedef int mytype;')
    b.add_function('int sum(int a, int b);')
    print(b)
    logging.info(b)

    print('=============================================')

    c = Source_File('mysource')
    c.add_include('<stdio.h>')
    c.add_include(b.get_name())
    c.add_define('MYVAR 1')
    c.add_type('typedef struct { int i; } my_struct_type_t;')
    c.add_type('typedef int mytype;')
    c.add_function('int sum(int a, int b);')
    c.add_function_body('int sum(int a, int b) {return a + b;}')
    c.add_global_var('int COUNTER;')
    c.add_local_var('static int internal_counter;')
    print(c)
    logging.info(c)


