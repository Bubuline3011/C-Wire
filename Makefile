all : exec
main.o: main.c projet.h
	gcc -c main.c -o main.o
avl.o:  avl.c projet.h
	gcc -c avl.c -o avl.o
outil.o: outil.c projet.h
	gcc -c outil.c -o outil.o
gerer_fichier.o: gerer_fichier.c projet.h
	gcc -c gerer_fichier.c -o gerer_fichier.o
exec: main.o avl.o outil.o gerer_fichier.o
	gcc gerer_fichier.o main.o avl.o outil.o -o exec
