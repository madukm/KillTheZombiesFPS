CXX=clang++

SHR_DIR = src/shared
TEST_DIR = src/tests

build:
	echo a

dirs:
	mkdir bin || true

clean:
	rm -rf bin || true

tests: dirs
	${CXX} $(TEST_DIR)/test_logger.cpp -o bin/test_logger -I$(SHR_DIR)
	bin/test_logger
