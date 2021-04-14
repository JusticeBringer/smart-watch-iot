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

```
sudo add-apt-repository ppa:pistache+team/unstable
sudo apt update
sudo apt install libpistache-dev
```

### Building: use Make or Manual command

#### Using Make

You can build the `microwave` executable by running `make`.

### Manually

Open the terminal, navigate into the root folder of this repository, and run

```
g++ microwave_example.cpp -o microwave -lpistache -lcrypto -lssl -lpthread
```

This will compile the project using g++, into an executable called `microwave` using the libraries `pistache`, `crypto`, `ssl`, `pthread`. You only really want pistache, but the last three are dependencies of the former.
Note that in this compilation process, the order of the libraries is important.

### Running

To start the server run\
`./microwave`

Your server should display the number of cores being used and no errors.

To test, open up another terminal, and type\
`curl http://localhost:9080/ready`

Number 1 should display.

Now you have the server running

## License

This project is licensed under the Apache 2.0 Open Source Licence - see the [LICENSE](LICENSE) file for details