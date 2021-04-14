smartwatch: smartwatch.cpp
	g++ $< -o $@ -lpistache -lcrypto -lssl -lpthread
