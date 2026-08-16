# vim: set ts=2 sw=2 tw=99 noet:
"""
DODHooks - Configure script

Supports:
  - SourceMod 1.12 / 1.13
  - Metamod:Source 1.12 / 1.13
  - 32-bit (x86) and 64-bit (x86_64) builds
  - Windows, Linux
  - Latest AMBuild 2.x
"""

import sys

try:
    from ambuild2 import run
except ImportError:
    try:
        import ambuild
        sys.stderr.write('It looks like you have AMBuild 1 installed, but this project uses AMBuild 2.\n')
        sys.stderr.write('Upgrade to the latest version of AMBuild to continue.\n')
    except ImportError:
        sys.stderr.write('AMBuild must be installed to build this project.\n')
        sys.stderr.write('https://github.com/alliedmodders/ambuild\n')
    sys.exit(1)

# Require AMBuild 2.2+
ambuild_version = getattr(run, 'CURRENT_API', '2.1')
if ambuild_version.startswith('2.1'):
    sys.stderr.write("AMBuild 2.2 or higher is required; please update\n")
    sys.exit(1)

run = run.BuildParser(sourcePath=sys.path[0], api='2.2')

run.options.add_argument('--hl2sdk-root', type=str, dest='hl2sdk_root', default=None,
    help='Root search folder for HL2SDKs')
run.options.add_argument('--mms-path', type=str, dest='mms_path', default=None,
    help='Path to Metamod:Source')
run.options.add_argument('--sm-path', type=str, dest='sm_path', default=None,
    help='Path to SourceMod')
run.options.add_argument('--enable-debug', action='store_const', const='1', dest='debug',
    help='Enable debugging symbols')
run.options.add_argument('--enable-optimize', action='store_const', const='1', dest='opt',
    help='Enable optimization')
run.options.add_argument('--target', default=None,
    help='Override the default build target (x86 or x86_64)')
run.options.add_argument('--version', type=str, dest='version', default=None,
    help='Override the version string baked into the binary')

run.Configure()
