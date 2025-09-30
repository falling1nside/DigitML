WARNINGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wformat=2 \
 -Winit-self -Wmissing-declarations -Wredundant-decls -Wshadow \
 -Wstrict-overflow=5 -Wswitch-default -Wundef

FLAGS = $(WARNINGS) -std=c++14 -Ilib
SRC = src/main.cpp src/NN.cpp lib/matrix.cpp

all: sigmoid

sigmoid: 
	g++ $(FLAGS) -Ofast $(SRC) -o main

tanh:
	g++ $(FLAGS) -DUSE_TANH -Ofast $(SRC) -o main

bent_identity:
	g++ $(FLAGS) -DPERS -Ofast $(SRC) -o main

test:
	g++ $(FLAGS) -DTESTS -Ofast $(SRC) -o test_app -lgtest -lgtest_main -lpthread

run_test: test
	./test_app

debug:
	g++ $(FLAGS) -DDEBUG -g $(SRC) -o main
	./main

clean:
	rm -f main test_app

help:
	@echo "Доступные цели:"
	@echo "  make sigmoid      - компиляция с сигмоидой (по умолчанию)"
	@echo "  make tanh         - компиляция с tanh активацией"
	@echo "  make test         - компиляция unit-тестов"
	@echo "  make run_test     - запуск unit-тестов"
	@echo "  make debug        - отладочная версия"
	@echo "  make clean        - очистка"
