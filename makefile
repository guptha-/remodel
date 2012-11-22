MAIN = main
DFLAG = -g
WFLAG = -Wall

SRCF = $(MAIN).c\
      md5.c\
			md5manip.c
INCF = defn.h\
      incl.h\
			prot.h\
			md5.h
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

clean:
	rm -rf bin/*
	rm -rf obj/*
