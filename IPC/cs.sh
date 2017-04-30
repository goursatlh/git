rm -f cscope*
rm -f tags

find . -follow -name "*.c" -or -name "*.mib" -or -name "*.js" -or -name "*.php" -or -name "*.h" -or -name "*Makefile*" >cscope.files
cscope -Rbqk &
ctags -R &

