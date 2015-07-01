#!/bin/bash

function usage()
{
    echo "env.sh repo-directory user passwd"
}

if [  $# -ne 3 ]; then
    usage
    exit 1
fi

if [ ! -d $1/.git ]; then
    echo "input is not a git repo"
    exit 1
fi

cp commit-msg $1/.git/hooks/commit-msg -f 

sed -i /'url = https:\/\/'/"{s/\/\//\/\/$2:$3@/g}" $1/.git/config
