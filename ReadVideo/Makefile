IDIR = include
CC = gcc
CPP = g++
CFLAGS = -g -Wall -I$(IDIR) -std=c++11

ODIR = obj
LDIR = src

LIBS = -lavutil -lavformat -lavcodec -lavutil \
-lswresample -lswscale \
-lpthread -lm -ldl

_DEPS =
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o MyVideo.o MyThread.o MyFrame.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

FDIR = frame
RDIR = record

EXC = MyVideo.out

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: src/%.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CFLAGS)

$(EXC): $(OBJ)
	$(CPP) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

test:
	./MyVideo.out Friend3s.mp4

test2:
	./MyVideo.out Friend3s.mp4 Alone30s.mp4

test3:
	./MyVideo.out Alone30s.mp4 Queen30s.mp4 Alone540P.mp4

test4:
	./MyVideo.out Alone30s.mp4 Queen30s.mp4 Kindness30s.mp4 Summer30s.mp4

test5:
	./MyVideo.out Alone540P.mp4

valgrindTest:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --log-file=valgrind-log.txt ./MyVideo.out Friend3.mp4  2>&1

cleanFrame:
	rm -f *~ core $(FDIR)/*.ppm $(FDIR)/*.pgm $(FDIR)/*.txt

clean:
	rm -f $(ODIR)/*.o *~ core $(FDIR)/*.ppm $(FDIR)/*.pgm $(FDIR)/*.txt  $(RDIR)/*.txt
