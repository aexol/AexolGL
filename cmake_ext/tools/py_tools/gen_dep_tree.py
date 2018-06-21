import json, os, re, subprocess, sys

os.chdir(sys.argv[2])
so_pattern = re.compile('^.*\.so([.0-9]*)?$')
needed_pattern = re.compile('^.*\(NEEDED\)\s*Shared library: \[(.*)\]$')
so_names = [f for f in os.listdir(os.curdir) if so_pattern.match(f)]

def get_lib_deps(fn):
    o = subprocess.check_output([sys.argv[1], '-d', fn])
    o = [needed_pattern.match(l).group(1)
            for l in o.split('\n') if needed_pattern.match(l)]
    return o

def _traverse_deps(d, lib):
    libs = get_lib_deps(lib)
    for l in libs:
        if l in so_names:
            _traverse_deps(d, l)
    d[lib] = libs

def traverse_deps(root_lib):
    root_map = {}
    _traverse_deps(root_map, root_lib)
    return root_map

with open("lib_dep_map_"+sys.argv[3]+".txt.so", 'w') as f:
    f.write(json.dumps(traverse_deps(os.sys.argv[3])))
