smartwatch: smartwatch.cpp
	g++ -std=c++17 $< -o $@ -lpistache -lcrypto -lssl -lpthread
