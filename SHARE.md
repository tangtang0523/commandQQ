#tar the **\*.gz** file.
tar zxvf vim.tar.gz 

#fix the problem.The key of backspace can not work in vim.
append the `set backspace=indent,eol,start` to the *~/.vim/vimrc* file.

#fix the problem.The syntax is not highly light. 
append the `syntax on` to the *~/.vim/vimrc* file.

#fix the problem.The `syntax on` command is not available in this version.
append the `alais vi=vim` to the *~/.bashrc* file.
excute the `. ~/.bashrc` in the shell.
