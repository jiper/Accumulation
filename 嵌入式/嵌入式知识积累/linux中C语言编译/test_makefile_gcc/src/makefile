#指明头文件路径
add_cflags = -I../header
#wildcard只能获取工作目录下的文件
SOURCES = $(wildcard *.cpp) $(wildcard *.c) 


compile:$(SOURCES)
	gcc -c $(add_cflags) $(SOURCES)


createLib: *.o
	ar -r libtest[linux_dbg_32].a  *.o

createDll: *.o
	gcc  -shared  -o libtest[linux_dbg_32].so  *.o
	
.PHONY: clean
clean:
	rm *.o *.a *.so
