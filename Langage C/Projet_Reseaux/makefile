all: serv_tcp serv_udp clie_tcp clie_udp

serv_tcp: serv_tcp.c
	@gcc -o serv_tcp serv_tcp.c -lpthread

serv_udp: serv_udp.c
	@gcc -o serv_udp serv_udp.c -lpthread

clie_tcp: clie_tcp.c
	@gcc -o clie_tcp clie_tcp.c

clie_udp: clie_udp.c
	@gcc -o clie_udp clie_udp.c

clean:
	@rm -f serv_tcp serv_udp clie_tcp clie_udp