CC:=gcc

num_endianess_convert: num_endianess_convert.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

inet_endian_convert: inet_endian_convert.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@


.PHONY: single_conn single_conn_clean
single_conn_executables := single_conn_server single_conn_client

# 通用规则
%: %.c
	$(CC) -o $@ $^

# 聚合目标
single_conn: $(single_conn_executables)

# 清理
single_conn_clean:
	rm -f $(single_conn_executables)
