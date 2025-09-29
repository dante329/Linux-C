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

errno_test:errno_test.c 
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

unnamed_pipe_test:unnamed_pipe_test.c 
	-$(CC) -o $@ $^
#在"-./$@"后输入要传入的参数
	-./$@ "daven" 
	-rm ./$@

fifo_write:fifo_write.c 
	-$(CC) -o $@ $^

fifo_read:fifo_read.c 
	-$(CC) -o $@ $^

named_pipe:fifo_write fifo_read

named_pipe_clean:
	-rm ./fifo_write 
	-rm ./fifo_read

shared_memory:shared_memory.c 
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

father_son_mq_test:father_son_mq_test.c 
	-$(CC) -o $@ $^
	-./$@
	

producer:producer.c 
	-$(CC) -o $@ $^

consumer:consumer.c 
	-$(CC) -o $@ $^

producer_consumer_clean:
	-rm ./producer
	-rm ./consumer

signal_test:signal_test.c 
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@