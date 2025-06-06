#!/usr/bin/env python3
# setup_hooks.py

import os
import shutil
import stat

print("Setting up git hooks...")

# Create hooks directory if it doesn't exist
hooks_dir = os.path.join('.git', 'hooks')
os.makedirs(hooks_dir, exist_ok=True)

# Copy the pre-commit hook
source = os.path.join('.hooks', 'pre-commit_LH')
destination = os.path.join(hooks_dir, 'pre-commit_LH')
shutil.copy2(source, destination)

# Make the hook executable (not needed on Windows but doesn't hurt)
os.chmod(destination, os.stat(destination).st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)

print("Git hooks installed successfully!")

