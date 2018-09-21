" An example for a vimrc file.
"
" Maintainer:   Bram Moolenaar <Bram@vim.org>
" Last change:  2006 Nov 16
"
" To use it, copy it to
"     for Unix and OS/2:  ~/.vimrc
"             for Amiga:  s:.vimrc
"  for MS-DOS and Win32:  $VIM\_vimrc
"           for OpenVMS:  sys$login:.vimrc

" When started as "evim", evim.vim will already have done these settings.
if v:progname =~? "evim"
  finish
endif

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                              personal setting
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

""""""""""""""""""""""""""""""
" 1,common setting
""""""""""""""""""""""""""""""
"set filetype=c
filetype on
let python_highlight_all = 1
let &termencoding=&encoding
"set encoding=utf-8
set fileencodings=utf-8,gb2312,gbk,gb18030
set termencoding=utf-8
set fileformats=unix
set encoding=prc
set cscopequickfix=s-,c-,d-,i-,t-,e-
set laststatus=1
colorscheme desert
"set nowrap  "do not newline
set autoindent
set clipboard+=unnamed
"set fdm=marker
set fdm=indent
"

"zidongbuquan
filetype plugin indent on
"set completeopt=longest,menu
let g:SuperTabRetainCompletionType=2 
let g:SuperTabDefaultCompletionType="<C-X><C-O>" 
inoremap  <F1> <C-X><C-N>

"code fold
set foldenable
set foldlevel=100 
set foldmethod=syntax
nnoremap f @=((foldclosed(line('.')) < 0) ? 'zc' : 'zo')<CR>

set fileencodings=gb2312,gb18030,utf-8
set termencoding=utf-8
set encoding=prc


"""""""""""""""""""""""""""""""
" 2,load cscope and ctags files
" deprecated, use gtags 
"""""""""""""""""""""""""""""""
"cs add /home/wt/vimtest/cscope.out /home/wt/vimtest
"set tags=/home/wt/vimtest/tags

"Key maps for cscope
nmap <F5>  :cs find c <C-R>=expand("<cword>")<CR><CR>	
nmap <F6>  :cs find d <C-R>=expand("<cword>")<CR><CR>	
nmap <F7>  :cs find g <C-R>=expand("<cword>")<CR><CR>	
nmap <F9>  :cs find s <C-R>=expand("<cword>")<CR><CR>	
"nmap <F10> :cs find g <C-R>=expand("<cword>")<CR><CR>	
nmap <C-\>f :cs find f <C-R>=expand("<cfile>")<CR><CR>	
nmap <C-\>i :cs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
nmap <F10> :cw<cr> 
nmap <F11> :cn<cr> 
nmap <F12> :cp<cr> 

""""""""""""""""""""""""""""""
" 3,winmanger setting
""""""""""""""""""""""""""""""
let g:winManagerWindowLayout='FileExplorer|TagList'
nmap wm :WMToggle<cr>

"""""""""""""""""""""""""""""" 
" 4,lookupfile setting 
""""""""""""""""""""""""""""""
nmap <F4>  :LookupFile <C-R>=expand("<cfile>")<CR><CR>
let g:LookupFile_MinPatLength = 2                
let g:LookupFile_PreserveLastPattern = 0        
let g:LookupFile_PreservePatternHistory = 1     
let g:LookupFile_AlwaysAcceptFirst = 1         
let g:LookupFile_AllowNewFiles = 0              
"if filereadable("./filenametags")                
"let g:LookupFile_TagExpr = '"./tags"' 
"endif 
nmap <silent> <leader>lk <Plug>LookupFile<cr>    
nmap <silent> <leader>ll :LUBufs<cr>            
nmap <silent> <leader>lw :LUWalk<cr>         

""""""""""""""""""""""""""""""
" 5,mimibufexpl setting 
""""""""""""""""""""""""""""""
let g:miniBufExplMapCTabSwitchBufs = 1
let g:miniBufExplMapWindowNavVim = 1

syntax on
let tlist_txt_settings = 'txt;c:content;f:figures;t:tables'
au BufRead,BufNewFile *.txt setlocal ft=txt


""""""""""""""""""""""""""""""
" 6,gtags  
""""""""""""""""""""""""""""""
set cscopetag " 使用 cscope 作为 tags 命令
set cscopeprg='gtags-cscope' " 使用 gtags-cscope 代替 cscope
 
"gtags.vim 设置项
let GtagsCscope_Auto_Load = 1
let CtagsCscope_Auto_Map = 1
let GtagsCscope_Quiet = 1
let g:Gtags_OpenQuickfixWindow = 1
let g:Gtags_Auto_Update = 1


"let $GTAGSLABEL = 'native-pygments'
"let $GTAGSCONF = '/usr/local/share/gtags/gtags.conf'

"let g:gutentags_project_root = ['.root', '.svn', '.git', '.hg', '.project']

"let g:gutentags_ctags_tagfile = '.tags'

" support both ctags and gtags
"let g:gutentags_modules = []
"if executable('ctags')
	"let g:gutentags_modules += ['ctags']
"endif
"if executable('gtags-cscope') && executable('gtags')
"	let g:gutentags_modules += ['gtags_cscope']
"endif

" put generated ctags or gtags files to ~/.cache/tags
"let g:gutentags_cache_dir = expand('~/.cache/tags')

" configure parameters of ctags
"let g:gutentags_ctags_extra_args = ['--fields=+niazS', '--extra=+q']
"let g:gutentags_ctags_extra_args += ['--c++-kinds=+px']
"let g:gutentags_ctags_extra_args += ['--c-kinds=+px']

"let g:gutentags_ctags_extra_args += ['--output-format=e-ctags']

" disable autoload database 
"let g:gutentags_auto_add_gtags_cscope = 0



"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                              personal setting end
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


" Use Vim settings, rather then Vi settings (much better!).
" This must be first, because it changes other options as a side effect.
set nobackup
set nocompatible

" allow backspacing over everything in insert mode
set backspace=indent,eol,start

"if has("vms")
set nobackup          " do not keep a backup file, use versions instead
"else
"  set backup            " keep a backup file
"endif
set history=50          " keep 50 lines of command line history
set ruler               " show the cursor position all the time
set showcmd             " display incomplete commands
set incsearch           " do incremental searching
"set tags=./tags,tags,/usr/include/tags

" For Win32 GUI: remove 't' flag from 'guioptions': no tearoff menu entries
" let &guioptions = substitute(&guioptions, "t", "", "g")

" Don't use Ex mode, use Q for formatting
map Q gq

" In many terminal emulators the mouse works just fine, thus enable it.
" set mouse=a

" Switch syntax highlighting on, when the terminal has colors
" Also switch on highlighting the last used search pattern.
syntax enable
syntax on
"set nocp

if &t_Co > 2 || has("gui_running")  
    syntax on
    set hlsearch
endif

" Only do this part when compiled with support for autocommands.
if has("autocmd")

  " Enable file type detection.
  " Use the default filetype settings, so that mail gets 'tw' set to 72,
  " 'cindent' is on in C files, etc.
  " Also load indent files, to automatically do language-dependent indenting.
  filetype plugin indent on

  " Put these in an autocmd group, so that we can delete them easily.
  augroup vimrcEx
  au!

  " For all text files set 'textwidth' to 78 characters.
  autocmd FileType text setlocal textwidth=78

  " When editing a file, always jump to the last known cursor position.
  " Don't do it when the position is invalid or when inside an event handler
  " (happens when dropping a file on gvim).
  autocmd BufReadPost *
    \ if line("'\"") > 0 && line("'\"") <= line("$") |
    \   exe "normal! g`\"" |
    \ endif

  augroup END

else

  set autoindent                " always set autoindenting on

endif " has("autocmd")

if has("cscope") && filereadable("/usr/local/bin/cscope")
   set csprg=/usr/local/bin/cscope
   set csto=0
   set cst
   set nocsverb
   "add CSCOPE_DB first
   if $CSCOPE_DB != ""
      cs add $CSCOPE_DB
   " else add current db 
   elseif filereadable("cscope.out")
     cs add cscope.out
   endif
   set csverb
endif
" Convenient command to see the difference between the current buffer and the
" file it was loaded from, thus the changes you made.
command DiffOrig vert new | set bt=nofile | r # | 0d_ | diffthis
                \ | wincmd p | diffthis
"
" Use my own color scheme
colorscheme desert
"User own defines
set history=50          " keep 50 lines of command line history
set ruler               " show the cursor position all the time
set cindent shiftwidth=4
set ts=4
set expandtab
set showmatch
" set nu
set is
set lbr
set laststatus=2
set statusline=%F%m%r%h%w\ [HEX=\%02.2B]\ [LINE=%l]\ [LEN=%L]
"let g:winManagerWindowLayout="FileExplorer,TagsExplorer|BufExplorer"
nnoremap <silent> <F8> :TlistToggle<CR>
"nnoremap <silent> <F12> :WMToggle<CR>

let Tlist_Ctags_Cmd='/usr/bin/ctags' 
let Tlist_Show_One_File=1 
let Tlist_Exit_OnlyWindow=1 
let Tlist_Use_Left_Window=1
let Tlist_Auto_Open=1 
let Tlist_Compact_Format=1
let Tlist_Display_Prototype=1
let Tlist_File_Fold_Auto_Close=1

"imap <TAB> <C-p>
vnoremap . :normal .<CR>

"brace 
":inoremap ( ()<ESC>i
":inoremap ) <c-r>=ClosePair(')')<CR>
":inoremap { {}<ESC>i
":inoremap } <c-r>=ClosePair('}')<CR>
":inoremap [ []<ESC>i
":inoremap ] <c-r>=ClosePair(']')<CR>

"color for wt
hi Comment  ctermfg=4
hi pythonComment  ctermfg=4
hi Constant  ctermfg=red     
hi String  ctermfg=3
hi Operator  ctermfg=red     
hi PreCondit  ctermfg=5
hi Search term=reverse ctermfg=7 ctermbg=2
hi IncSearch term=reverse ctermfg=3 ctermbg=2
hi Label   ctermfg=red
hi Repeat  ctermfg=red
hi Conditional   ctermfg=red
hi Statement ctermfg=red
hi Macro ctermfg=5
hi Number ctermfg=red
hi StatusLine term=bold,reverse cterm=bold,reverse ctermfg=0 ctermbg=0

"   Edit another file in the same directory as the current file
"   uses expression to extract path from current file's path
"   (thanks Douglas Potts)
if has("unix")
    map ,e :e <C-R>=expand("%:p:h") . "/" <CR>
else
    map ,e :e <C-R>=expand("%:p:h") . "\" <CR>
endif

function ClosePair(char)
if getline('.')[col('.') - 1] == a:char
return "\<Right>"
else
return a:char
endif
endf


":highlight strcolor ctermbg=black guibg=red ctermfg=red guifg=yellow term=bold
":match strcolor /\[.*\]/
