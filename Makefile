REPEAT ?= 10
NAME ?= loop

test:
	@mkdir -p build tmp
	@cd build && clang -O2 -DTEST_NUMBER=$(REPEAT) -DTEST_NAME=test_$(NAME) --save-temps ../src/main.c -o main
	@sudo nice -n -20 build/main | tee tmp/$(NAME).log
