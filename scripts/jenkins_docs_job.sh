#!/bin/bash
cd $WORKSPACE/build/release
zip_archive=`cpack -G ZIP | tail -1 | cut -d ' ' -f 4`
cd $WORKSPACE
chmod +x scripts/generate_docs.py
$WORKSPACE/scripts/generate_docs.py $zip_archive $WORKSPACE/Doc/sphinx_source aexlib prepare_venv
git reset --hard
