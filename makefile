main : main.o partie1.o partie2.o partie3.o partie4.o
	gcc -o main main.o partie1.o partie2.o partie3.o partie4.o -lssl -lcrypto -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib/

main.o : main.c partie1.h partie2.h partie3.h partie4.h
	gcc -c main.c

partie1.o : partie1.c partie1.h
	gcc -c partie1.c

partie2.o : partie2.c partie1.h partie2.h
	gcc -c partie2.c

partie3.o : partie3.c partie1.h partie2.h partie3.h
	gcc -c partie3.c

partie4.o : partie4.c partie1.h partie2.h partie3.h partie4.h
	gcc -c partie4.c

clean :
	rm -f *.o main