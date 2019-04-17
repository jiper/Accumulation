#指明头文件路径
add_cflags = -I../header

#使用静态库
testLib:
	g++ -o testLib $(add_cflags) test_makefile_gcc.cpp  -L  ../src  -ltest[linux_dbg_32]
	
#使用动态库
testDll:
	g++  -o  testDll $(add_cflags) test_makefile_gcc.cpp -L  ../src -ltest[linux_dbg_32]
	
#动态库使用前需要指定路径
	#export  LD_LIBRARY_PATH=../src
