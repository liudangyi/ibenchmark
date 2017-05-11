from __future__ import print_function
import sys, os
from glob import glob
from subprocess import Popen, PIPE
import re

ROOT = os.path.abspath(__file__ + '/../')
TARGET = ROOT + '/build/main'
TESTS_MAP = None # name to address

def skip_build():
    if not os.path.exists(TARGET):
        return False
    src_files = [d + '/' + f for d, _, files in os.walk(ROOT + '/src') for f in files]
    max_time = max(map(os.path.getmtime, src_files))
    target_time = os.path.getmtime(TARGET)
    return target_time >= max_time

def build():
    global TESTS_MAP
    if not skip_build():
        print('Building...')
        os.system('cd {} && mkdir -p build && cd build && clang -Wl,-no_pie -O2 --save-temps ../src/main.c -o main'.format(ROOT))
        TESTS_MAP = None
    if TESTS_MAP is None:
        stdout, _ = Popen('cd {}/build && nm main | grep " _test"'.format(ROOT), stdout=PIPE, shell=True).communicate()
        TESTS_MAP = {}
        for line in stdout.decode().splitlines():
            addr, _, name = line.split()
            name = name[6:]
            TESTS_MAP[name] = addr

def run_test(*test_names, **kargs):
    repeat = kargs.pop('repeat', 10)
    build()
    res = {}
    for name in test_names:
        assert name in TESTS_MAP, 'No test called ' + name
        print('Running test {} for {} times...'.format(name, repeat))
        process = Popen('{}/build/main {} {} 0x{}'.format(ROOT, repeat, name, TESTS_MAP[name]), stdout=PIPE, stderr=PIPE, shell=True)
        stdout, stderr = process.communicate()
        if process.returncode != 0 or stderr != '':
            print('Test {} returns {}'.format(name, process.returncode), file=sys.stderr)
            sys.stderr.write(stderr.decode())
        else:
            res[name] = []
            for line in stdout.decode().splitlines():
                match = re.search(r'cycles = ([\d.]+)', line)
                if match:
                    res[name].append(float(match.group(1)))
    return res

def get_all_tests():
    build()
    return TESTS_MAP.keys()

if __name__ == '__main__':
    run_test('loop') # test
