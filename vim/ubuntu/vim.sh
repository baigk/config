#!/bin/bash

apt-get install vim vim-scripts vim-doc ctags cscope

cp -f vimrc ~/.vimrc

mkdir -p ~/.vim/plugin ~/.vim/doc

vim-addons install omnicppcomplete taglist winmanager minibufexplorer project

cp -rf ../plugin/* ~/.vim  -rf

git clone https://github.com/baigk/tagbar.git 

cp tagbar/autoload ~/.vim -rf
cp tagbar/doc ~/. -rf
cp tagbar/plugin ~/.vim -rf
cp tagbar/syntax ~/.vim -rf

rm -rf tagbar

