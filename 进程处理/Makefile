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