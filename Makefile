FinalProjectClient: client.c functions1and2.c functions3and4.c functions5and6.c
	gcc -o FinalProjectClient client.c functions1and2.c functions3and4.c functions5and6.c -std=c99 -lpthread

FinalProjectServer: server.c functions1and2.c functions3and4.c functions5and6.c
		gcc -o FinalProjectServer server.c functions1and2.c functions3and4.c functions5and6.c -std=c99 -lpthread
