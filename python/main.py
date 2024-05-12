import importlib.machinery
import inspect
import types
import typing
# import example
from pprint import pprint
import sys
import os
import json


def get_attrs(klass):
  return [k for k in klass.__dict__.keys()
            if not k.startswith('__')
            and not k.endswith('__')]

# def get_user_attributes(cls):
#     boring = dir(type('dummy', (object,), {}))
#     return [item
#             for item in inspect.getmembers(cls)
#             if item[0] not in boring]

class ClassInfo:
   def __init__(self, name, fields, methods, other_classes):
      self.name = name
      self.fields = fields
      self.methods = methods
      self.other_classes = other_classes

   def get_name(self)->str:
      return self.name
   
   def get_fields(self)->list:
      return self.fields
   
   def get_methods(self)->list:
      return self.methods
   
   def get_other_classes(self) -> list:
      return self.other_classes;
   
   def set_name(self, name):
      self.name = name

   def set_fields(self, fields):
      self.fields = fields

   def set_methods(self, methods):
      self.methods = methods

   def set_other_classes(self, other_classes):
      self.other_classes = other_classes
   
class MethodInfo:
   def __init__(self, name, args, lines_count, return_value):
      self.name = name
      self.args = args
      self.lines_count = lines_count
      self.return_value = return_value

   def get_name(self):
      return self.name
   
   def get_args(self):
      return self.args

   def get_lines_count(self):
      return self.lines_count
   
   def get_return_value(self):
      return self.return_value
   
   def set_name(self, name):
      self.name = name

   def set_args(self, args):
      self.args = args

   def set_lines_count(self, lines_count):
      self.lines_count = lines_count

   def set_return_value(self, return_value):
      self.return_value = return_value

# поля класса
def get_class_fields(class_object) -> list:

   list_func = inspect.getmembers(class_object, inspect.isfunction)

   list_func_2 = []

   for e in list_func:
      list_func_2.append(e[0])

   list1 = get_attrs(class_object)

   list_fields = list(set(list1) - set(list_func_2))

   s = inspect.signature(class_object.__init__)

   string = f"{s}"
   g = string.split(" -> ")[0].strip("(").rstrip(")").split(", ")
   g.remove("self")
   for el in g:
      list_fields.append(el)

   return list_fields

# функции класса
def get_class_functions(class_object) -> list:
   list_func = inspect.getmembers(class_object, inspect.isfunction)

   list_func_2 = []

   for e in list_func:
      list_func_2.append(e[0])

   list_func_2.remove("__init__")

   return list_func_2

# глобальные функции модуля
def get_global_functions(module) -> list:
   list_func = inspect.getmembers(module, inspect.isfunction)

   # list_func_2 = []

   # for e in list_func:
   #    list_func_2.append(e[0])

   return list_func

def get_global_fields(module) -> list:
   # глобальные функции в модуле
   list_func = inspect.getmembers(module, inspect.isfunction)

   #классы в модуле
   list_classes = inspect.getmembers(module, inspect.isclass)

   list_1 = []

   for el in list_func:
      list_1.append(el[0])

   for el in list_classes:
      list_1.append(el[0])

   # глобальные функции, переменные и классы
   list_total = get_attrs(module)

   list_fields = list(set(list_total) - set(list_1))

   return list_fields

def get_classes(module) -> list:
   list_classes = inspect.getmembers(module, inspect.isclass)

   # list_total = []

   # for el in list_classes:
   #    list_total.append(el[0])

   return list_classes

def get_other_classes(object_class, module) -> list:
   list_all_member = inspect.getmembers(object_class)

   list_all_classes = get_classes(module)

   list_classes = []
   for el in list_all_classes:
      list_classes.append(el[1])

   tuple_cl = tuple(list_classes)

   list_objects = []
   for el in list_all_member:
      # print(isinstance(el[1], tuple_cl))
      if isinstance(el[1], tuple_cl):
         list_objects.append(el)

   # print(tuple_cl)
   # print(list_objects)
   list_t = []
   for t in tuple_cl:
      for l in list_objects:
         if t == type(l[1]):
            list_t.append(t)

   list_other_cl = []
   for c in list_all_classes:
      for t in list_t:
         if t == c[1]:
            list_other_cl.append(c[0])
            # print(c)
   return list_other_cl

def get_class_info(class_object, module) -> ClassInfo:

   # получаем функции класса
   list_func = inspect.getmembers(class_object, inspect.isfunction)

   list_method_info = list()

   for method in list_func:

      if method[0] == "__init__":
         continue

      # получить аргументы и возвращающее значение
      method_args_ret = inspect.getfullargspec(method[1])

      # аргументы функции
      list_args = method_args_ret.args

      # удаляем self, если присутствует
      if(list_args.count("self")):
         list_args.remove("self")

      # получить количество строк
      sourse = inspect.getsourcelines(method[1])
      list_s = sourse[0]

      # форматируем функцию - удаление комментов
      for item in list_s:
         deleted = item.strip()
         if deleted.startswith('"') or deleted.startswith("#"):
            list_s.remove(item)

      # кол-во строк в функции = общее кол-во - строки комментариев - определение функции (def func)
      lines_count = len(list_s) - 1
      # return_value = method_args_ret.annotations
      return_value = False if len(method_args_ret.annotations) == 0 else True
      # print(return_value)
      m = MethodInfo(method[0], list_args, lines_count, return_value)
      list_method_info.append(m)

   # список полей класса
   list_fields = get_class_fields(class_object)

   # список других классов
   list_other_classes = get_other_classes(class_object, module)
   class_info = ClassInfo(class_object.__name__, list_fields, list_method_info, list_other_classes)
   return class_info

def make_class_w_global(module) -> list:
   list_globals_func = get_global_functions(module)
   list_classes = get_classes(module)

   list_method_info = list()

   for method in list_globals_func:

      # получить аргументы и возвращающее значение
      method_args_ret = inspect.getfullargspec(method[1])

      # аргументы функции
      list_args = method_args_ret.args

      # удаляем self, если присутствует
      if(list_args.count("self")):
         list_args.remove("self")

      # получить количество строк
      sourse = inspect.getsourcelines(method[1])
      list_s = sourse[0]

      # форматируем функцию - удаление комментов
      for item in list_s:
         deleted = item.strip()
         if deleted.startswith('"') or deleted.startswith("#"):
            list_s.remove(item)

      # кол-во строк в функции = общее кол-во - строки комментариев - определение функции (def func)
      lines_count = len(list_s) - 1
      # return_value = method_args_ret.annotations
      return_value = False if len(method_args_ret.annotations) == 0 else True
      # print(return_value)
      m = MethodInfo(method[0], list_args, lines_count, return_value)
      list_method_info.append(m)
   
   # список полей класса
   list_fields = list()

   list_other_classes = list()
   for cl in list_classes:
      list_other_classes.append(cl[0])

   # список других классов
   class_info = ClassInfo("", list_fields, list_method_info, list_other_classes)
   return class_info

# считываем все файлы с кодом
with open(os.getcwd()+"/debug/python/list_python_code.txt", 'r') as files:
   list_files = files.readlines()

   list_files.pop()
   for file in list_files:
      file_name = file.replace("\n", "")
      # pprint(file)
      loader = importlib.machinery.SourceFileLoader('example', file_name)
      module_name = types.ModuleType(loader.name)
      loader.exec_module(module_name)

      list_global_func = make_class_w_global(module_name)
      # print(f"список глобальных функций: {list_global_func}")

      list_global_fields = get_global_fields(module_name)
      # print(f"список глобальных переменных: {list_global_fields}")

      list_classes = get_classes(module_name)
      # print(f"список классов: {list_classes}")

      # list_other = get_other_classes(module.A, module)
      # print(f"список других классав: {list_other}")

      # list_fields = get_class_fields(module.A)
      # print(f"список полей класса: {list_fields}")

      list_info = list()
      
      # классы
      for one_class in list_classes:
         info = get_class_info(one_class[1], module_name)
         list_info.append(info)

      list_info.append(list_global_func)

      list_json = []

      for info in list_info:

         json_methods = []
         json_method = str
         for method in info.get_methods():
            json_method = {'args': method.get_args(),
                           'linesCount': method.get_lines_count(),
                           'name': method.get_name(),
                           'retVal':method.get_return_value()
                           }
            json_methods.append(json_method)

         to_json = { 'class': {'fields': info.get_fields(),
                              'methods': json_methods,
                              'name': info.get_name()
                              },
                     'global_field': list_global_fields,
                     'other_classes': info.get_other_classes()
                     }

         list_json.append(to_json)

      with open('sw_templates.json', 'w') as f:
         json.dump(list_json, f, sort_keys=True, indent=2)

# to_json = {'trunk': trunk_template, 'access': access_template}
# with open('sw_templates.json') as f:
#     print(f.read())

sys.stderr.write("all parsed")