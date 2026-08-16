# vim: set ts=2 sw=2 tw=99 noet:
"""
DoDHooks - Configure Script
Requires AMBuild 2.2+ (install from https://github.com/alliedmodders/ambuild)
Supports: Windows (x86, x64), Linux (x86, x64)
"""

import sys
import os

# Require AMBuild 2.2+
try:
    from ambuild2 import run
    if not run.HasAPI('2.2'):
        sys.stderr.write('AMBuild 2.2 or higher is required; please update\n')
        sys.stderr.write('https://github.com/alliedmodders/ambuild\n')
        sys.exit(1)
except ImportError:
    sys.stderr.write('AMBuild must be installed to build this project.\n')
    sys.stderr.write('Install from source:\n')
    sys.stderr.write('  git clone https://github.com/alliedmodders/ambuild\n')
    sys.stderr.write('  pip install ./ambuild\n')
    sys.exit(1)

API_VERSION = '2.2'

builder = run.BuildParser(sourcePath=sys.path[0], api=API_VERSION)

# ---- Build options ----
builder.options.add_argument('--hl2sdk-root', type=str, dest='hl2sdk_root', default=None,
    help='Root search folder for HL2SDKs')
builder.options.add_argument('--mms-path', type=str, dest='mms_path', default=None,
    help='Path to Metamod:Source')
builder.options.add_argument('--sm-path', type=str, dest='sm_path', default=None,
    help='Path to SourceMod')
builder.options.add_argument('--enable-debug', action='store_const', const='1', dest='debug',
    help='Enable debugging symbols')
builder.options.add_argument('--enable-optimize', action='store_const', const='1', dest='opt',
    help='Enable optimization')
builder.options.add_argument('-s', '--sdks', default='dod', dest='sdks',
    help='Build against specified SDKs (default: %(default)s)')
builder.options.add_argument('--targets', type=str, dest='targets', default=None,
    help='Override target architecture (e.g. "x86,x86_64"). Default: both.')
builder.options.add_argument('--disable-auto-versioning', action='store_true',
    dest='disable_auto_versioning', default=False,
    help='Disable the auto versioning script')

builder.Configure()
