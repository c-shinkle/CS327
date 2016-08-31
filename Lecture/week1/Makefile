all: hello times_tables

hello: hello.c
	gcc -Wall -Werror -ggdb hello.c -o hello                                

times_tables: times_tables.c
	gcc -Wall -Werror -ggdb times_tables.c -o times_tables

#Makefiles can be used to build Java programs, too.
#But your environment may not have a JDK installed
hello.class: hello.java
	javac hello.java

clean:
	rm -f hello times_tables *~ *.o hello.class

