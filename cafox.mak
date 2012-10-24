vpath %.cpp c:/cpp/canew c:/cpp/fortify
vpath %.hpp c:/cpp/canew c:/cpp/fortify
vpath %.h c:/cpp/canew c:/cpp/fortify
#CFLAGS = -O3 -Wall -Ic:/cpp/fortify -Ic:/cpp/canew -DGRAPHICS -fexternal-templates
CFLAGS = -Wall -Ic:/cpp/fortify -Ic:/cpp/canew -DGRAPHICS -DFORTIFY -fexternal-templates  -g

%o : %cpp
	gcc $(CFLAGS) -c $<

O = cafox.o mcafox.o spfox.o cabase.o r250.o bgi.o fortify.o 

L = -lgpp -lm -lgrx20

cafox : $(O)
	gcc -o gcafox $(O) $(L)
	stubify gcafox
	copy *.cpp *.cc

cafox.o : cafox.cpp cafox.hpp spfox.hpp cabase.hpp smattpl.hpp cafox.mak

mcafox.o : mcafox.cpp cafox.hpp spfox.hpp cabase.hpp smattpl.hpp cafox.mak

spfox.o : spfox.cpp spfox.hpp cabase.hpp smattpl.hpp cafox.mak

cabase.o : cabase.cpp cabase.hpp smattpl.hpp cafox.mak

r250.o : r250.cpp r250.h cafox.mak

bgi.o : bgi.cpp cafox.mak

fortify.o : fortify.cpp fortify.h ufortify.h cafox.mak

