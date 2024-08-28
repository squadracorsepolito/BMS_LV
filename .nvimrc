set path+=**
nnoremap <F10> :!dos2unix ./Core/**/** ./** <enter>
nnoremap <F4> :!ctags -R .<enter>
nnoremap <F5> :make -f Makefile <bar> cwindow <enter>
nnoremap <F6> :!make -f Makefile flash<enter>
nnoremap <M-C-F> :w<enter>:!clang-format -style=file -i %<enter>
