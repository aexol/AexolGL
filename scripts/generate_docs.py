#!/usr/bin/env python
import inspect
import subprocess
import errno
import sys
import pip
import zipfile
import click
import os
import re
import shutil
from importlib import import_module
"""Module helping for generating rst documents.

This module is mainly responsible for generating rst documentation files for Aexol python modules.

Example:
    python generate.py 

Attributes:
    header_template(string): Parametrized template used to generate module rst header. Parameters are {module} which is a module import path.
    autoclass_template(string): Parametrized template used for class documentation. Parameters are {module} which is a module that class resides in and {cls} which is class name.
    autofunction_template(string): Parametrized template used for function documentation. Parameters are {module} which is a module that function resides in and {function_name} which is function name.
"""

header_template = """=====================================
{module}
=====================================

.. module:: {module}

"""

autoclass_template = """class {cls}
--------------------------------------------------------------------
.. autoclass:: {object_name}.{cls}
    :members:

"""

autofunction_template = """function {function_name}
--------------------------------------------------------------------
.. autofunction:: {object_name}.{function_name}

"""

package_template = """.. _{package}-index:
#############################################
    {hr_package_name} Reference
#############################################

{package_desc}

.. toctree::
    :maxdepth: 2
    :numbered:
    """

def get_attribs_in_obj(obj):
    """Return attributes in object.

    Iterate over all attributes in obj and build dictionary mapping attributes to they're names.

    Parameters:
        obj: Object whose attributes we're looking up.

    Returns:
        dict: Dictionary of key, value where key is an attribute name in obj and value is attribute object.
    """
    return {attr: getattr(obj, attr) for attr in dir(obj) if not attr.startswith('__')}

def list_functions(obj):
    """Return functions in obj.

    Return each attribute object in obj that is callable.

    Parameters:
        obj: Any python object whose functions we are looking up.

    Returns:
        list: List of attribute objects.
    """
    return [key for key, value in get_attribs_in_obj(obj).items() if hasattr(value, '__call__') and not inspect.isclass(value)]

def list_classes(obj):
    """Return class type objects in obj.

    Return a list of all class names in obj.

    Parameters:
        obj: Search for class types in obj.

    Returns:
        list: List of class names in obj.
    """
    return [key for key, value in get_attribs_in_obj(obj).items() if inspect.isclass(value)]

def on_module(module_name, out, func, **kwargs):
    """Execute function on module.

    Imports module identified by module_name and executes a function passing in a module object. Function must accept 3 parameters (module, object, string).

    Parameters:
        module_name: Name of module to be loaded.
        out: Out variable.
        func: Callable.

    Returns:
        Result of func(module, module_name, out)
    """
    mod = import_module(module_name)
    return func(mod, out, **kwargs)


def document_classes(obj, out, **kwargs):
    """Document all classes in module.

    Iterate over all classes in module. Generate documentation from autoclass_template and append result to out.

    Parameters:
        object: Object to be searched for class objects.
        out: String object to which result is appended.
        object_name: Searched object name, default obj.__name__.
        template: Template to be used for decorating class, default is autoclass_template.

    Returns:
        string: Output with decoration appended.
    """
    try:
        object_name = kwargs.pop('object_name')
    except KeyError:
        object_name = obj.__name__

    try:
        template = kwargs.pop('template')
    except KeyError:
        template = autoclass_template

    for cls in list_classes(obj):
        out = out + template.format(object_name=object_name, cls=cls)
    return out

def document_functions(obj, out, **kwargs):
    """Document all functions in object.

    Iterate over all functions in obj and generate documentation based on template.

    Parameters:
        obj: Object to be iterated over in search for functions.
        out: String object to which result is appended.
        object_name: Object name that is to be used for decoration (default=obj.__name__).
        template: Template to be used for decorating (default: autofunction_template).

    Returns:
        string: out string with decoration for function appended.
    """
    try:
        object_name = kwargs.pop('object_name')
    except KeyError:
        object_name = obj.__name__

    try:
        template = kwargs.pop('template')
    except KeyError:
        template = autofunction_template

    for func in list_functions(obj):
        out = out + template.format(object_name=object_name, function_name=func)
    return out

def document_module(module_name):
    """Documents module.

    Generate documentation for module by iterating over public objects and functions and appending them to resulting doc.

    Parameters:
        module_name: Module to be documented.
    """
    out = header_template.format(module=module_name)
    out = on_module(module_name, out, document_classes)
    out = on_module(module_name, out, document_functions)
    return out

def document_package(package_path, package_import, ext = ['.so', '.pyx', '.py'], ignore = ['__.*']):
    """Document package.

    Iterate over all modules in package and create documentation for each.

    Parameters:
        package_path: Package location.
        package_import: Import string for package. (For example for 'import aexlib' this would take value 'aexlib')
        ext: List of module extensions that should be included in generation.
        ignore: If file matches one of patterns in this list, it will be excluded from documentation.
    """
    modules = [mod for mod in os.listdir(package_path) if os.path.splitext(mod)[1] in ext]
    for ignore_pattern in ignore:
        modules = [mod for mod in modules if not re.match(ignore_pattern, mod)]
    modules_names = [package_import+'.'+os.path.splitext(module)[0] for module in modules]
    package_docs = {}
    for name in modules_names:
        package_docs[name] = document_module(name)
    return package_docs


def find_all_pyx_modules(path):
    """Find *.pyx files.

    Search path for files ending with *.pyx and return them in list.

    Parameters:
        path: Search path.

    Returns:
        list: List of *.pyx files.
    """
    files = [pyx_file for pyx_file in os.listdir(path) if pyx_file.endswith('.pyx')]

_venv_wrapper_cmd = ['bash', '-c', 'source $(which virtualenvwrapper.sh) && {command}']

def check_venv(venv_name):
    """Checks whether virtual env already exists.

    Test whether virtual env given by venv_name already exists.

    Parameters:
        venv_name: Name for virtual_env.

    Return:
        boolean: False if virtualenv already exists, False otherwise.
    """
    try:
        cmd = list(_venv_wrapper_cmd)
        cmd[2] = cmd[2].format(command='lsvirtualenv')
        check_venv_name = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    except OSError as e:
        if e.errno == errno.ENOENT:
            raise RuntimeError('Could not execute lsvirtualenv. Is virtualenvwrapper installed?')
        else:
            raise
    grep_ps = subprocess.Popen(('grep', '^'+venv_name+'$'), stdin=check_venv_name.stdout, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    pout, perr = grep_ps.communicate()
    if grep_ps.returncode:
        if pout or perr:
            raise RuntimeError('Could not check if virtual env exists.')
    check_venv_name.wait()
    return bool(pout)


def prepare_virtualenv(lib_zip, venv_name, deps = [], allow_exists=True):
    """prepare_virtualenv

    Creates and prepares virtualenv for generating docs for a project.

    Parameters:
        lib_zip: Full path to zip with library.
        venv_name: Name of virtual env to be created.
        deps: Dependecies to be installed in virtual enviroments.
        allow_exists: True if use existing virtual env, False if create new.
    """
    venv_name_real = venv_name
    counter = 1
    while not allow_exists and check_venv(venv_name_real):
        venv_name_real=venv_name+'_'+str(counter)
        counter += 1

    install_deps = False
    if not check_venv(venv_name_real):
        cmd = list(_venv_wrapper_cmd)
        cmd[2] = cmd[2].format(command='mkvirtualenv '+venv_name_real) 
        subprocess.check_call(cmd)
        install_deps = True
    else:
        cmd = list(_venv_wrapper_cmd) 
        cmd[2] = cmd[2].format(command='workon '+venv_name_real) 
        subprocess.check_call(cmd)
        # TODO: Check deps in existing virtualenv by trying to import them.
    if install_deps:
        for dep in deps:
            cmd = list(_venv_wrapper_cmd)
            cmd[2] = cmd[2].format(command='workon '+venv_name_real+' && pip install '+dep) 
            subprocess.check_call(cmd)

    cmd = list(_venv_wrapper_cmd)
    cmd[2] = cmd[2].format(command='workon '+venv_name_real+' && python -c \'import os; print os.environ["VIRTUAL_ENV"]\'') 
    _venv_path = subprocess.check_output(cmd).rstrip()
        
    zip_root = os.path.splitext(os.path.split(lib_zip)[1])[0]
    with zipfile.ZipFile(lib_zip, 'r') as z:
        z.extractall('/tmp')
    extract_root = os.path.join('/tmp', zip_root)
    for dirpath, dirs, fnames in os.walk(extract_root):
        rel_dir = os.path.relpath(dirpath, extract_root)
        target_dir = os.path.join(_venv_path, rel_dir)
        try:
            os.mkdir(target_dir)
        except OSError as e:
            if e.errno == errno.EEXIST and os.path.isdir(target_dir):
                pass
            else:
                raise
        for fname in fnames:
            try:
                shutil.move(os.path.join(dirpath, fname), target_dir)
            except shutil.Error as e:
                os.remove(os.path.join(target_dir, fname))
                shutil.move(os.path.join(dirpath, fname), target_dir)
    return venv_name_real

def add_module_to_index(index_rst, file_rst):
    """Add module rst file to index rst file.

    Add RST file to index toctree.

    Parameters:
        index_rst: Index file.
        file_rst: File to be added to toctree.
    """
    new_file = os.path.relpath(file_rst, os.path.dirname(index_rst))
    with open(index_rst, 'r') as index_file:
        last_line = index_file.readlines()[-1]
    match_indent = re.match('^(\s*)', last_line)
    indent = ""
    if match_indent:
        indent = match_indent.group(1)
    if last_line[-1] != '\n':
        indent = '\n'+indent
    with open(index_rst, 'a') as index_file:
        index_file.write(indent+new_file+'\n')
    return new_file

def create_rst_file(output_path, module_name, contents):
    """Generate RST file.

    Create RST file with documentation.

    Parameters:
        output_path: Output path where new RST should be placed.
        module_name: Name of module which this file documents.
        contents: Documentation contents.
    """
    rst_file = os.path.join(output_path, module_name[module_name.rfind('.')+1:]+'.rst')
    with open(rst_file, 'w') as f:
        f.write(contents)
    return rst_file

def create_new_index(contents, doc_root, parent_index = None):
    """Create new index file.

    Create new index file and add, if necessery add it to parent toctree.

    Parameters:
        contents: No docstring, file a bug report.
        doc_root: No docstring, file a bug report.
        parent_index: No docstring, file a bug report.
    """
    new_index = create_rst_file(doc_root, 'index', contents)
    if parent_index:
        add_module_to_index(parent_index, new_index)
    return new_index

def check_package(package_name):
    """Checks whether package exists.

    Test whether package given by package_name exists.

    Parameters:
        package_name: Name of package to be documented.

    Return:
        string: Full path to package.
    """
    package = import_module(package_name)
    return os.path.split(os.path.realpath(package.__file__))[0]

def recursive_package_doc_from_venv(output_path, package_import, index_rst):
    """recursive_package_doc_from_venv

    No doc string, file a bug report.

    Parameters:
        output_path: No docstring, file a bug report.
        package_import: No docstring, file a bug report.
        index_rst: No docstring, file a bug report.
    """
    modules = document_package(check_package(package_import), package_import)
    for key, val in iter(sorted(modules.items())):
        rst_doc = create_rst_file(output_path, key, val)
        add_module_to_index(index_rst, rst_doc)

@click.group()
@click.argument('filename')
@click.argument('output_path')
@click.argument('module_name')
@click.pass_context
def cli(ctx, filename, output_path, module_name):
    """Cli group
    
    CLI group

    Parameters:
        ctx: Command Line context object
        filename: Zip file with package to install.
        output_path: Generated docs ouput path.
        module_name: Name of package for which docs should be generated.
    """
    ctx.obj['filename'] = filename
    ctx.obj['output_path'] = output_path
    ctx.obj['module_name'] = module_name 

@cli.command()
@click.pass_context
def prepare_venv(ctx):
    """Prepare virtual enviroment for docs generation.

    Prepares virtual enviroment necessary for generating docs for aexlib package.    

    Parameters:
        ctx: CLI context.
    """
    filename = ctx.obj['filename']
    module_name = ctx.obj['module_name']
    output_path = ctx.obj['output_path']
    venv = prepare_virtualenv(filename, '{0}_docs'.format(module_name), deps = ['numpy', 'click', 'sphinx', 'sphinx_bootstrap_theme', 'sphinxcontrib-napoleon'])
    cmd = list(_venv_wrapper_cmd)
    cmd[2] = cmd[2].format(command='workon '+venv+' && python '+__file__+' '+filename+' '+output_path+' '+module_name+' generate_docs')
    check_venv_name = subprocess.check_call(cmd)

@cli.command()
@click.pass_context
def generate_docs(ctx):
    """Generate rst files for package.

    Generte rst file structure for aexlib package. This file does not generate sphinx configuration. It only generates rst files for each module in aexlib package.

    Parameters:
        ctx: CLI context.
    """
    filename = ctx.obj['filename']
    module_name = ctx.obj['module_name']
    output_path = ctx.obj['output_path']
    main_index_contents = """Welcome to AexolGL's documentation!
===================================

Contents:

.. toctree::
    :maxdepth: 2
    
    """
    main_index = create_new_index(main_index_contents, output_path)
    aexlib_output = os.path.join(output_path, 'aexlib')
    aexlib_index = create_new_index(
            package_template.format(package='aexlib', hr_package_name='AexLib Python', package_desc='Some smart mumbo-jumbo about library.'), 
            aexlib_output,
            os.path.join(output_path, 'index.rst')
        )
    recursive_package_doc_from_venv(aexlib_output, 'aexlib', aexlib_index)
    with open(main_index, 'a') as f:
        f.write("""


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

""")

    subprocess.check_call(['make', 'html'], cwd=os.path.join(output_path, os.pardir)) #TODO: FIXME!!!


if __name__ == "__main__":
    cli(obj={})
