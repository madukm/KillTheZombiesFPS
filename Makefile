CXX=clang++

SHR_DIR = src/shared 

build:
	echo a

tests:
	${CXX} test_logger.cpp -I$(SHR_DIR)
