MAIN = main
DFLAG = -g
WFLAG = -Wall

SRCF = $(MAIN).c\
      md5.c\
			md5manip.c\
			remodfile.c\
			util.c\
			tree.c\
			execute.c
INCF = defn.h\
      incl.h\
			prot.h\
			md5.h\
			ds.h
OBJF = $(SRCF:.c=.o)
SRCDIR = src
OBJDIR = obj
INCDIR = inc
MVOBJ = mv -f *.o obj/

SRC = $(patsubst %,$(SRCDIR)/%,$(SRCF))
OBJ = $(patsubst %,$(OBJDIR)/%,$(OBJF))
INC = $(patsubst %,$(INCDIR)/%,$(INCF))

CREATEDIR = mkdir -p obj bin .remodel

remodel: $(OBJ)
	$(CREATEDIR)
	gcc -o bin/remodel $(WFLAG) $(OBJ) -lm

obj/md5.o: src/md5.c inc/md5.h
	$(CREATEDIR)
	gcc -c src/md5.c -I inc $(WFLAG) $(DFLAG) -o obj/md5.o

obj/$(MAIN).o: src/main.c src/utmain.c $(INC)
	$(CREATEDIR)
	gcc -c src/$(MAIN).c -I inc $(WFLAG) $(DFLAG) -o obj/$(MAIN).o

obj/md5manip.o: src/md5manip.c $(INC)
	$(CREATEDIR)
	gcc -c src/md5manip.c -I inc $(WFLAG) $(DFLAG) -o obj/md5manip.o

obj/remodfile.o: src/remodfile.c $(INC)
	$(CREATEDIR)
	gcc -c src/remodfile.c -I inc $(WFLAG) $(DFLAG) -o obj/remodfile.o

obj/util.o: src/util.c $(INC)
	$(CREATEDIR)
	gcc -c src/util.c -I inc $(WFLAG) $(DFLAG) -o obj/util.o

obj/tree.o: src/tree.c $(INC)
	$(CREATEDIR)
	gcc -c src/tree.c -I inc $(WFLAG) $(DFLAG) -o obj/tree.o

obj/execute.o: src/execute.c $(INC)
	$(CREATEDIR)
	gcc -c src/execute.c -I inc $(WFLAG) $(DFLAG) -o obj/execute.o

clean:
	rm -rf bin/remodel
	rm -rf obj/*

test: src/test.c
	gcc -o bin/test.exe -g -Wall src/test.c
