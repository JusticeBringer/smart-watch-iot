# Proiect IP: Smartwatch IOT (aprilie - mai 2021)

Componența echipei:
 - Arghire Gabriel, 342
 - Chitu Stefan Catalin, 342
 - Olaru Adrian, 342
 - Simionov Marius Daniel, 342
     
Funcționalități smartwatch:
 1. Dacă este o oră fixă, atunci declanșează o vibrație
 2. Luminozitatea ecranului să fie în funcție de luminozitatea mediului
 3. Detectarea mișcării: dacă nu se mai fac pași pentru o perioadă lungă de timp, atunci să apară de format apel către 112
 4. Dacă nivelul bateriei este scăzut, atunci să se aprindă un led de notificare
 5. Dacă se apasă un buton lateral de pe ceas, atunci ecranul ceasului să devină alb (simulează o lanternă)

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

### Test functionalities

Below are some samples of the applications after using `GET` and `POST`

#### Routes


```
POST http://localhost:9080/settings/lowBattery/:value
GET  http://localhost:9080/settings/lowBattery/
```

**Note: replace 9080 with the required port number (9080 is by default).**

#### Test if server is running

Open up another terminal, and type

```
curl http://localhost:9080/ready
```

Number 1 should display.

(Note: it is not always showing up)

Now you have the server running

#### Functionality no. 1

```
1. Dacă este o oră fixă, atunci declanșează o vibrație
```

Routes:

```
POST http://localhost:9080/settings/alarmClock/:value
GET  http://localhost:9080/settings/alarmClock/
```

```
curl -X POST http://localhost:9080/settings/alarmClock/true
alarmCLock was set to true
```

```
curl -X GET http://localhost:9080/settings/alarmClock/
alarmCLock is set to 1. Value 1 means setting is on.
```

```
curl -X POST http://localhost:9080/settings/alarmClock/false
alarmCLock was set to false
```

```
curl -X GET http://localhost:9080/settings/alarmClock/
alarmCLock is set to 0. Value 0 means setting is off.
```

#### Functionality no. 2

```
2. Luminozitatea ecranului să fie în funcție de luminozitatea mediului
```

Routes:

```
POST 
GET  
```

#### Functionality no. 3

```
3. Detectarea mișcării: dacă nu se mai fac pași pentru o perioadă lungă de timp, atunci să apară de format apel către 112
```

Routes:

```
POST 
GET  
```

#### Functionality no. 4

```
4. Dacă nivelul bateriei este scăzut, atunci să se aprindă un led de notificare
```

Routes:

```
POST http://localhost:9080/settings/lowBattery/:value
GET  http://localhost:9080/settings/lowBattery/
```

```
curl -X POST http://localhost:9080/settings/lowBattery/200
lowBattery was set to true
```

```
curl -X GET http://localhost:9080/settings/lowBattery/    
lowBattery setting is set to true.
```

```
curl -X POST http://localhost:9080/settings/lowBattery/2000
lowBattery was set to false
```

```
curl -X GET http://localhost:9080/settings/lowBattery/     
lowBattery setting is set to false.
```

#### Functionality no. 5

```
5. Dacă se apasă un buton lateral de pe ceas, atunci ecranul ceasului să devină alb (simulează o lanternă)
```

Routes:

```
POST http://localhost:9080/settings/lantern/:value
GET  http://localhost:9080/settings/lantern/
```

```
curl -X POST http://localhost:9080/settings/lantern/true
lantern was set to true
```

```
curl -X GET http://localhost:9080/settings/lantern/ 
lantern is set to 1. Value 1 means setting is on.
```

```
curl -X POST http://localhost:9080/settings/lantern/false
lantern was set to false
```

```
curl -X GET http://localhost:9080/settings/lantern/ 
lantern is set to 0. Value 0 means setting is off.
```

## License

This project is licensed under the Apache 2.0 Open Source Licence - see the [LICENSE](LICENSE) file for details