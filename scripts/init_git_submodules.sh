#!/bin/bash
eval `~/bin/keychain.sh --eval ~/.ssh/github_rsa`
git submodule init
git submodule update

