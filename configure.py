#!/usr/bin/env python3
"""
DoDHooks configure.py
Compatible with SourceMod 1.12 / AMBuild 2.2
"""

import sys, os, subprocess, platform

# SM 1.12 configure API
from ambuild2 import runplugin
from ambuild2 import util

class configure(runplugin.Configure):
    def configure(self):
        # C++17 for SM 1.12
        self.cxx_cpp = "17"

        # Detect compiler
        if platform.system() == "Windows":
            self.compiler = "msvc"
        else:
            self.compiler = "gcc"  # or clang

        # Architecture
        self.target_arch = self.options.targets

        # Build type
        if self.options.enable_optimize:
            self.build_type = "release"
        else:
            self.build_type = "debug"

        # SM paths
        self.sm_root = self.options.sm_path
        self.mm_root = self.options.mms_path
        self.hl2sdk_root = self.options.hl2sdk_root
        self.sdk_name = self.options.sdks  # 'dod'

        # Validate paths
        if not os.path.isdir(self.sm_root):
            self.fatal_error("SourceMod path not found: %s" % self.sm_root)
        if not os.path.isdir(self.mm_root):
            self.fatal_error("Metamod:Source path not found: %s" % self.mm_root)

        sdk_path = os.path.join(self.hl2sdk_root, "hl2sdk-dods")
        if not os.path.isdir(sdk_path):
            self.fatal_error("HL2SDK DoD:S not found: %s" % sdk_path)

        self.hl2sdk_path = sdk_path

        # Generate AMBuild build script
        self.generate_builds()

    def generate_builds(self):
        # Write AMBuildScript with all build info
        script = """# -*- coding: utf-8 -*-
from ambuild2 import Extension
import os

builder = getbuilder()

# Project definition
project = Extension.HL2Project(builder, 'dodhooks.ext')

# Sources
project.sources = [
    'extension.cpp',
    'natives.cpp',
    'vglobals.cpp',
]

# Include paths
project.includes = [
    builder.options.sm_root + '/public',
    builder.options.sm_root + '/public/sourcepawn',
    builder.options.sm_root + '/public/amtl',
    builder.options.sm_root + '/public/safetyhook',
    builder.options.mm_root + '/core',
    builder.options.mm_root + '/core/sourcehook',
    builder.options.hl2sdk_path + '/public',
    builder.options.hl2sdk_path + '/public/game',
    builder.options.hl2sdk_path + '/public/tier1',
    os.getcwd(),
]

# Defines
project.defines = [
    'DODHOOKS_BUILD',
    'META_NO_HL2SDK',
]

# C++ standard
project.cpp_std = 'c++17'

# Output
project.output_file = 'dodhooks.ext'

# Dependencies
project.depends_on('sourcemod')
project.depends_on('metamod')

builder.add_project(project)
"""
        # Write the script
        ambuild_path = os.path.join(os.getcwd(), "AMBuildScript")
        with open(ambuild_path, 'w') as f:
            f.write(script)

        print("✅ DoDHooks configure complete")
        print(f"   SM root: {self.sm_root}")
        print(f"   MM root: {self.mm_root}")
        print(f"   HL2SDK:  {self.hl2sdk_path}")
        print(f"   Arch:    {self.target_arch}")
        print(f"   Build:   {self.build_type}")

if __name__ == '__main__':
    runplugin.run(configure)
