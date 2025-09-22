CC := gcc

process_test:process_test.c 
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@
# ping: socket: 不允许的操作 在这里执行没有ping的权限?

fork_test:fork_test.c 
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

fork_fd_test:fork_fd_test.c 
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

erlou:erlou.c 
	-$(CC) -o $@ $^

execve_test:execve_test.c 
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

fork_execve_test:fork_execve_test.c 
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

waitpid_test:waitpid_test.c 
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

pstree_test:pstree_test.c 
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

erlou_block:erlou_block.c 
	-$(CC) -o $@ $^

orphan_process_test:orphan_process_test.c 
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@