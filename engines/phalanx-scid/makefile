CC = gcc

####### choose the line that makes the binary faster on your machine
# CFLAGS = -O3 -Wall -fomit-frame-pointer -funroll-loops
CFLAGS = -O3 -w -fomit-frame-pointer --std=gnu89 -D_GNU_SOURCE
# -std=c99 
#-Werror removed

####### debug/tuning options for developers
# CFLAGS = -O -Wall -g3 -static
# CFLAGS = -O -Wall -pg

#######
### DEFINES
### -DSHOW_FORCED_MOVES
### -DPBOOK_FILE=\"pbook.phalanx\"
### -DSBOOK_FILE=\"sbook.phalanx\"
### -DLEARN_FILE=\"learn.phalanx\"
### -DPBOOK_DIR=\"/usr/local/lib\"
### -DSBOOK_DIR=\"/usr/local/lib\"
### -DLEARN_DIR=\"/var/local/lib\"
### -DQCAPSONLY

DEFINES = -DGNUFUN 
LDFLAGS =

# .o/bcreate.o

OBJ = .o/phalanx.o .o/search.o .o/io.o .o/data.o \
      .o/evaluate.o .o/genmoves.o .o/moving.o .o/hash.o .o/static.o \
      .o/levels.o .o/book.o .o/killers.o .o/endgame.o .o/learn.o

all: phalanx-scid

phalanx-scid: .o $(OBJ)
	$(CC) $(CFLAGS) $(DEFINES) $(LDFLAGS) $(OBJ) -o phalanx-scid

.o/%.o: makefile %.c
	$(CC) $(CFLAGS) $(DEFINES) -c $*.c -o .o/$*.o

.o:
	mkdir .o

clean:
	rm -rf .o phalanx phalanx-scid *.lrn learn.phalanx

backup:
	tar -czvf Archive/phalanx.tgz \
	makefile *.c *.h pbook.phalanx sbook.phalanx test.fin

