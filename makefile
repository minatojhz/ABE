all:main clean
objects= main.o  Node.o AccessTree.o RuleBuilder.o tate.o cf.o bne.o str.o xml.o ATRPATHLCM.o Polynomial.o ABCEBLOCK.o ABCSK.o ABCPK.o ATRGROUP.o ABCMK.o ABC.o aes.o
INCLUDE= /usr/local/include
LIB= /usr/local/lib
BOOSTREG= boost_regex-gcc-1_42
TINYXML= tinyxml
DEBUG= -g
main: $(objects)
	g++  $(DEBUG) -o main $(objects)  -Wl,-rpath $(LIB) -l$(BOOSTREG) -l$(TINYXML) -lcrypto -lssl
main.o: main.cpp RuleBuilder.h ABC.h
	g++  $(DEBUG)  -c main.cpp
ABC.o: ABC.cpp ABC.h
	g++  $(DEBUG)  -c ABC.cpp
ABCEBLOCK.o: ABCEBLOCK.cpp ABCEBLOCK.h
	g++  $(DEBUG)  -c ABCEBLOCK.cpp
ABCMK.o: ABCMK.cpp ABCEBLOCK.h
	g++  $(DEBUG)  -c ABCMK.cpp
ABCSK.o: ABCSK.cpp ABCSK.h
	g++  $(DEBUG) -c ABCSK.cpp

ABCPK.o: ABCPK.cpp ABCPK.h
	g++  $(DEBUG)  -c ABCPK.cpp

ATRGROUP.o:ATRGROUP.cpp ATRGROUP.h
	g++  $(DEBUG) -c ATRGROUP.cpp

RuleBuilder.o: RuleBuilder.cpp RuleBuilder.h
	g++ $(DEBUG)  -c RuleBuilder.cpp #-I $(INCLUDE)
Node.o: Node.cpp Node.h
	g++  $(DEBUG) -c Node.cpp
AccessTree.o: AccessTree.cpp AccessTree.h
	g++  $(DEBUG)  -c AccessTree.cpp
Polynomial.o: Polynomial.cpp Polynomial.h
	g++  $(DEBUG)  -c Polynomial.cpp
ATRPATHLCM.o: ATRPATHLCM.cpp ATRPATHLCM.h
	g++  $(DEBUG) -c ATRPATHLCM.cpp
tate.o: tate.cpp tate.h
	g++  $(DEBUG) -c tate.cpp
cf.o: cf.cpp cf.h 
	g++  $(DEBUG) -c cf.cpp
bne.o: bne.cpp bne.h
	g++  $(DEBUG)  -c bne.cpp
aes.o: aes.cpp aes.h
	g++  $(DEBUG) -c aes.cpp
xml.o: xml.cpp xml.h 
	g++  $(DEBUG) -c xml.cpp #-I $(INCLUDE)
str.o: str.cpp str.h print.h
	g++  $(DEBUG) -c str.cpp #-I $(INCLUDE)
clean:
	rm $(objects)
