VPATH = src

object = mytool1.o mytool2.o main.o

main: $(object)
	g++ -o $@ $(object)
	

.PHONY : clean
clean :
	rm main $(object)
