# Proiect IP: Smartwatch IOT (aprilie - mai 2021)

Componența echipei:
- Arghire Gabriel, 342
- Chitu Stefan Catalin, 342
- Olaru Adrian, 342
- Simionov Marius Daniel, 342

Funcționalități smartwatch:
1. Dacă este o oră fixă, atunci declanșează o vibrație
2. Luminozitatea ecranului să fie în funcție de luminozitatea mediului
3. Modul de alergare: Îți spune câți metri ai alergat
4. Modul de panică: dacă nu se mai fac pași pentru o perioadă lungă de timp, atunci esti întrebat dacă vrei să formezi un apel către 112
5. Dacă nivelul bateriei este scăzut, atunci să se aprindă un led de notificare
6. Dacă se apasă un buton lateral de pe ceas, atunci ecranul ceasului să devină alb (simulează o lanternă)

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
curl -X GET http://localhost:9080/settings/checkClock/
Negative response it is not 11 sharp.
```

```
curl -X GET http://localhost:9080/settings/checkClock/
Positive response, it is 11 sharp.
```

```
curl -X POST http://localhost:9080/settings/alarmClock/false
alarmCLock was set to false
```

```
curl -X GET http://localhost:9080/settings/alarmClock/
alarmCLock is set to 0. Value 0 means setting is off.
```

```
curl -X GET http://localhost:9080/settings/checkClock/
CheckClock is disabled because alarmClock is also disabled. Please set alarmClock to true.
```

#### Functionality no. 2

```
2. Luminozitatea ecranului să fie în funcție de luminozitatea mediului
```

Routes:

```
POST http://localhost:9080/settings/brightness/:value
GET  http://localhost:9080/settings/brightness/
```
```
curl -X POST http://localhost:9080/settings/brightness/10
Brightness was set to 10
```

```
curl -X GET http://localhost:9080/settings/brightness/
Brightness  is set to 10
```

```
curl -X POST http://localhost:9080/settings/brightness/200
Brightness must be a value between 0 and 100
```

#### Functionality no. 3

```
3. Modul de alergare: Îți spune câți metri ai alergat
```

Routes:

```
POST http://localhost:9080/settings/running/:value
GET  http://localhost:9080/settings/running/
```

```
curl -X POST http://localhost:9080/settings/running/10
Running was set to 10 minutes.
```

```
curl -X GET http://localhost:9080/settings/running
Last running time was 10 minutes and 1600 meters.
```

```
curl -X POST http://localhost:9080/settings/running/100
Running was set to 100 minutes.
```

```
curl -X GET http://localhost:9080/settings/running    
Last running time was 100 minutes and 11000 meters. You should rest.
```

#### Functionality no. 4

```
4.Modul de panică: dacă nu se mai fac pași pentru o perioadă lungă de timp (cel puțin 10 minute), atunci ești întrebat dacă vrei să formezi un apel către 112
```

Routes:

```
POST http://localhost:9080/settings/panicMode/:value
GET  http://localhost:9080/settings/panicMode/
```

```
curl -X POST http://localhost:9080/settings/panicMode/0
panicMode was set to false.
```

```
curl -X GET http://localhost:9080/settings/panicMode
panicMode is set to 0. Value 0 means panicMode is off.
```

```
curl -X POST http://localhost:9080/settings/panicMode/1
panicMode was set to true. Button „Call for 112” will appear at need.
```

```
curl -X GET http://localhost:9080/settings/panicMode
panicMode is set to 1. Value 1 means panicMode is activated and it will trigger at need
```

```
curl -X POST http://localhost:9080/settings/panicMode/8
panicMode was set to false.
```

Considerăm că perioada de timp în care utilizatorul nu mai face pași (între 180 de minute și 480 de minute) este asociată somnului.

```
curl -X POST http://localhost:9080/settings/panicMode/180
panicMode was set to false (this value is associated with sleep)
```

```
curl -X GET http://localhost:9080/settings/panicMode
panicMode is set to 0. Value 0 means panicMode is off.
```

Trigger la apelul de urgență se face la rutele:

```
POST http://localhost:9080/settings/triggerPanicMode/:value
GET  http://localhost:9080/settings/triggerPanicMode/
```

```
curl -X POST http://localhost:9080/settings/triggerPanicMode/0
triggerPanicMode was set to false.
```

```
curl -X POST http://localhost:9080/settings/panicMode/0
panicMode was set to false.
```

```
curl -X GET http://localhost:9080/settings/panicMode
panicMode is set to 0. Value 0 means panicMode is off.
```

```
curl -X GET http://localhost:9080/settings/triggerPanicMode
triggerPanicMode setting is disabled because panicMode is disabled. Please enable panicMode setting.
```

```
curl -X POST http://localhost:9080/settings/panicMode/1
panicMode was set to true. Button „Call for 112” will appear at need.
```

```
curl -X GET http://localhost:9080/settings/panicMode
panicMode is set to 1. Value 1 means panicMode is activated and it will trigger at need
```

```
curl -X POST http://localhost:9080/settings/triggerPanicMode/1
triggerPanicMode was set to true. Button „Call for 112” triggered.
```

```
curl -X GET http://localhost:9080/settings/triggerPanicMode
Emergency call in progress...
```

#### Functionality no. 5

```
5. Dacă nivelul bateriei este scăzut, atunci să se aprindă un led de notificare
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

#### Functionality no. 6

```
6. Dacă se apasă un buton lateral de pe ceas, atunci ecranul ceasului să devină alb (simulează o lanternă)
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

#### Run brightness script.py

```
 python3 script.py
```


## License

This project is licensed under the Apache 2.0 Open Source Licence - see the [LICENSE](LICENSE) file for details