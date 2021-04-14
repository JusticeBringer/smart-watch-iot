# Smartwatch IOT

## Getting Started

Git clone this project in your machine.

```
git clone https://github.com/JusticeBringer/smart-watch-iot.git
```

### Prerequisites

Use Linux, [Windows WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10) or a virtual machine with Linux.

You will need to have a C++ compiler. I used g++ that came preinstalled. Check using `g++ -v`

You will need to install the [Pistache](https://github.com/pistacheio/pistache) library.

Steps to install the nightly build:

```
sudo add-apt-repository ppa:pistache+team/unstable
sudo apt update
sudo apt install libpistache-dev
```

### Building

Open the terminal, navigate into the root folder of this repository, and run:

#### First option: using Make

```
make
```

#### Second option: manually

```
g++ smartwatch.cpp -o smartwatch -lpistache -lcrypto -lssl -lpthread
```

This will compile the project using g++, into an executable called `smartwatch` using the libraries `pistache`, `crypto`, `ssl`, `pthread`. You only really want pistache, but the last three are dependencies of the former.
Note that in this compilation process, the order of the libraries is important.

### Running

To start the server run

```
./smartwatch
```

Your server should display the number of cores being used and no errors, something like

```
Cores = 8
Using 2 threads
```

To test, open up another terminal, and type\
`curl http://localhost:9080/ready`

Number 1 should display.
(Note: it is not always showing up)

Now you have the server running

## License

This project is licensed under the Apache 2.0 Open Source Licence - see the [LICENSE](LICENSE) file for details