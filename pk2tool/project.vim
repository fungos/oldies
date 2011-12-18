let g:run_command='clear; echo "====== RUN ======"; cd bin; LD_LIBRARY_PATH="." ./pk2tool; cd ..'
let g:debug_command='echo "====== DEBUG ======"; cd bin; LD_LIBRARY_PATH="." kdbg ./pk2tool; cd ..'
let g:svn_commit_command='clear; echo "====== SVN COMMIT project.vim ======"; svn commit project.vim; echo "====== SVN COMMIT Makefile ======"; svn commit Makefile; echo "====== SVN COMMIT Objects.in ======"; svn commit Objects.in; cd src/; echo "====== SVN COMMIT /src ======"; svn commit;'
let g:svn_update_command='clear; echo "====== SVN UPDATE project.vim  ======"; svn update project.vim; echo "====== SVN UPDATE Makefile ======"; svn update Makefile; echo "====== SVN UPDATE Objects.in ======"; svn update Objects.in; cd src/; echo "====== SVN UPDATE /src ======"; svn update;'
