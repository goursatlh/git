rm -f cscope*
rm -f tags                    
rm -f GPATH GRTAGS GTAGS

#find . -follow -name "*.c" -or -name "*.mib" -or -name "*.js" -or -name "*.php" -or -name "*.h" -or -name "*Makefile*" >cscope.files
#cscope -Rbqk &
#ctags -R &

# use gtags instead of ctags/cscope to do the code index/reference
find . -follow -name "*.c" -or -name "*.mib" -or -name "*.js" -or -name "*.php" -or -name "*.h" -or -name "*Makefile*" >gtags.files
gtags 
~        
