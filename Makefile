test_%:
	@mkdir -p build
	@cd build && clang -O2 -DTEST_NAME=$@ --save-temps ../src/main.c -o main
	@sudo nice -n -20 build/main
