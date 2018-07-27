CC = c++
CFLAGS = -g -Wall
SRCS = qc.cpp
PROG = qc 

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) -m32  /Library/Frameworks/QCam.framework/QCam $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)


tt:tt.cpp
	c++ -m64 -lusb-1.0 tt.cpp -o tt
