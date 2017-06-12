from __future__ import print_function
import sys, os
from glob import glob
from subprocess import Popen, PIPE
from itertools import imap
import re
import time

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
        cmd = 'cd "{}" && mkdir -p build && cd build && \
            clang -O2 -Werror --save-temps ../src/main.c -o main'.format(ROOT)
        process = Popen(cmd, shell=True, stdout=PIPE, stderr=PIPE)
        stdout, stderr = process.communicate()
        if process.returncode != 0:
            sys.stdout.write(stdout.decode())
            sys.stderr.write(stderr.decode())
            raise Exception('clang exits non-zero!')
        TESTS_MAP = None
    if TESTS_MAP is None:
        cmd = 'nm {}/build/main | grep "^0"'.format(ROOT)
        stdout, _ = Popen(cmd, stdout=PIPE, shell=True).communicate()
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
    print('Running test {} for {} times...'.format(name, repeat), end='')
    res = []
    start = time.time()
    for _ in range(repeat):
        cmd = 'cd "{}" && build/main {} {} {}'.format(ROOT, 1, name, TESTS_MAP[name])
        process = Popen(cmd, stdout=PIPE, stderr=PIPE, shell=True)
        stdout, stderr = process.communicate()
        if process.returncode != 0 or stderr != '':
            sys.stderr.write(stderr.decode())
            sys.stdout.write(stdout.decode())
            err_msg = 'Test {} returns {}'.format(name, process.returncode)
            print(err_msg, file=sys.stderr)
            raise Exception(err_msg)
        else:
            for line in stdout.decode().splitlines():
                match = re.search(r'cycles = ([\d.]+)', line)
                if match:
                    res.append(float(match.group(1)))
    print(' Duration: {:.3f}s'.format(time.time() - start))
    return res

def run_tests(*test_names, **kargs):
    return { name: run_test(name, **kargs) for name in test_names }

def get_all_tests():
    build()
    return TESTS_MAP.keys()

if __name__ == '__main__':
    run_test('loop') # test
