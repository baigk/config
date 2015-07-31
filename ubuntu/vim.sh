#!/bin/bash

apt-get install vim vim-scripts vim-doc ctags cscope

cp -f vimrc ~/.vimrc

mkdir -p ~/.vim/plugin ~/.vim/doc

vim-addons install omnicppcomplete taglist winmanager minibufexplorer project

git clone https://github.com/baigk/tagbar.git 

cp tagbar/autoload /root/.vim/
cp tagbar/doc /root/.vim/
cp tagbar/plugin /root/.vim/
cp tagbar/syntax /root/.vim/ -rf

rm -rf tarbar

