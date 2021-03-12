# Example IoT Device

## Getting Started

Git clone this project in your machine. 

### Prerequisites

Build tested on Ubuntu Server 
You will need to have a C++ compiler. I used g++ that came preinstalled. Check using `g++ -v`

You will need to download, and build [Pistache](https://github.com/pistacheio/pistache) library. 


### Installing

A step by step series of examples that tell you how to get a development env running

You should open the terminal, navigate into the root folder of this repository, and run
`g++ microwave_example.cpp -o microwave -lpistache -lcrypto -lssl -lpthread`

This will compile the project using g++, into an executable called `microwave` using the libraries `pistache`, `crypto`, `ssl`, `pthread`. You only really want pistache, but the last three are dependencies of the former.
Note that in this compilation process, the order of the libraries is important. 

To start the server run
`./microwave` 

Your server should display the number of cores being used and no errors. 

To test, open up another terminal, and type
`curl http://localhost:9080/ready`

Number 1 should display.

Now you have the server running


## Built With

* [Pistache](https://github.com/pistacheio/pistache) - Web server

## License

This project is licensed under the Apache 2.0 Open Source Licence - see the [LICENSE.md](LICENSE.md) file for details