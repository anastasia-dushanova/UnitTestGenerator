# import importlib
# import inspect
# import sys

# if len(sys.argv) >= 2:
#     filename = sys.argv[1]
# else:
#     filename = 'example.py'

# try:
#     (name, suffix, mode, mtype)  = inspect.getmodule(filename)
# except TypeError:
#     print ('Could not determine module type of %s' % filename)
# else:
#     mtype_name = { importlib.PY_SOURCE:'source',
#                    importlib.PY_COMPILED:'compiled',
#                    }.get(mtype, mtype)

#     mode_description = { 'rb':'(read-binary)',
#                          'U':'(universal newline)',
#                          }.get(mode, '')

#     print ('NAME   :', name)
#     print ('SUFFIX :', suffix)
#     print ('MODE   :', mode, mode_description)
#     print ('MTYPE  :', mtype_name)

"""Sample file to serve as the basis for inspect examples.
"""

def module_level_function(arg1, arg2='default', *args, **kwargs):
    """This function is declared in the module."""
    local_variable = arg1
    return

def global_func(a, b, c) -> list:
    list1 = list()
    list1.append(a)
    list1.append(b)
    list1.append(c)
    return list1

global_field = "global"

class X:
    def __init__(self, name):
        self.name = name

class A(object):
    """The A class."""
    def __init__(self, name, age):
        self.name = name


    def get_name(self):
        "Returns the name of the instance."
        return self.name
    
    def my_function(self, a):
        a = 6
        self.field1 = a
        return "a"

    def sum(a, b)-> float:
        return a+b
    
    field1 = 1
    field2 = "value"

    x = X("name")

# instance_of_a = A('sample_instance')

class B(A):
    """This is the B class.
    It is derived from A.
    """

    # This method is not part of A.
    def do_something(self):
        """Does some work"""
        pass

    def get_name(self):
        "Overrides version from A"
        return 'B(' + self.name + ')'
