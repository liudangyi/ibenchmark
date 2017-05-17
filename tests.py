from __future__ import print_function
import sys, os
from glob import glob
from subprocess import Popen, PIPE
from itertools import imap
import re

ROOT = os.path.abspath(__file__ + '/../')
TARGET = ROOT + '/build/main'
TESTS_MAP = None # name to address

def skip_build():
    if not os.path.exists(TARGET):
        return False
    src_files = [d + '/' + f for d, _, files in os.walk(ROOT + '/src') for f in files]
    target_time = os.path.getmtime(TARGET)
    return all(imap(lambda f: os.path.getmtime(f) < target_time, src_files))

def build():
    global TESTS_MAP
    if not skip_build():
        print('Building...')
        cmd = 'cd {} && mkdir -p build && cd build && clang -O2 --save-temps ../src/main.c -o main'.format(ROOT)
        assert os.system(cmd) == 0, 'clang exits non-zero!'
        TESTS_MAP = None
    if TESTS_MAP is None:
        stdout, _ = Popen('nm {}/build/main | grep "^0"'.format(ROOT), stdout=PIPE, shell=True).communicate()
        TESTS_MAP = {}
        for line in stdout.decode().splitlines():
            addr, _, name = line.split()
            if name[:6] == '_test_':
                name = name[6:]
                TESTS_MAP[name] = int(addr, 16)
            elif name == '_main':
                main_point = int(addr, 16)
        for key in TESTS_MAP:
            TESTS_MAP[key] -= main_point

def run_test(name, repeat=10):
    build()
    assert name in TESTS_MAP, 'No test called ' + name
    print('Running test {} for {} times...'.format(name, repeat))
    res = []
    for _ in range(repeat):
        process = Popen('{}/build/main {} {} {}'.format(ROOT, 1, name, TESTS_MAP[name]), stdout=PIPE, stderr=PIPE, shell=True)
        stdout, stderr = process.communicate()
        if process.returncode != 0 or stderr != '':
            sys.stderr.write(stderr.decode())
            sys.stdout.write(stdout.decode())
            print('Test {} returns {}'.format(name, process.returncode), file=sys.stderr)
            raise Exception('Test {} returns {}'.format(name, process.returncode))
        else:
            for line in stdout.decode().splitlines():
                match = re.search(r'cycles = ([\d.]+)', line)
                if match:
                    res.append(float(match.group(1)))
    return res

def run_tests(*test_names, **kargs):
    return { name: run_test(name, **kargs) for name in test_names }

def get_all_tests():
    build()
    return TESTS_MAP.keys()

if __name__ == '__main__':
    run_test('loop') # test
