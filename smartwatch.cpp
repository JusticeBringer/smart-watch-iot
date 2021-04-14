/*
    Proiect IP: Smartwatch IOT (aprilie - mai 2021)

    Componenta echipei:
     - Arghire Gabriel, 342
     - Chitu Stefan Catalin, 342
     - Olaru Adrian, 342
     - Simionov Marius Daniel, 342
     
    Functionalitati smartwatch:
     1. Dacă este o oră fixă, atunci declanșează o vibrație
     2. Luminozitatea ecranului să fie în funcție de luminozitatea mediului
     3. Detectarea mișcării: dacă nu se mai fac pași pentru o perioadă lungă de timp, atunci să apară de format apel către 112
     4. Dacă nivelul bateriei este scăzut, atunci să se aprindă un led de notificare
     5. Dacă se apasă un buton lateral de pe ceas, atunci ecranul ceasului să devină alb (simulează o lanternă)

*/

#include <algorithm>

#include <pistache/net.h>
#include <pistache/http.h>
#include <pistache/peer.h>
#include <pistache/http_headers.h>
#include <pistache/cookie.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <pistache/common.h>

#include <signal.h>

using namespace std;
using namespace Pistache;

// General advice: pay atetntion to the namespaces that you use in various contexts. Could prevent headaches.

// This is just a helper function to preety-print the Cookies that one of the enpoints shall receive.
void printCookies(const Http::Request& req) {
    auto cookies = req.cookies();
    std::cout << "Cookies: [" << std::endl;
    const std::string indent(4, ' ');
    for (const auto& c: cookies) {
        std::cout << indent << c.name << " = " << c.value << std::endl;
    }
    std::cout << "]" << std::endl;
}

// Some generic namespace, with a simple function we could use to test the creation of the endpoints.
namespace Generic {

    void handleReady(const Rest::Request&, Http::ResponseWriter response) {
        response.send(Http::Code::Ok, "1");
    }

}

// Definition of the SmartwatchEnpoint class 
class SmartwatchEndpoint {
public:
    explicit SmartwatchEndpoint(Address addr)
        : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    { }

    // Initialization of the server. Additional options can be provided here
    void init(size_t thr = 2) {
        auto opts = Http::Endpoint::options()
            .threads(static_cast<int>(thr));
        httpEndpoint->init(opts);
        // Server routes are loaded up
        setupRoutes();
    }

    // Server is started threaded.  
    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serveThreaded();
    }

    // When signaled server shuts down
    void stop(){
        httpEndpoint->shutdown();
    }

private:
    void setupRoutes() {
        using namespace Rest;
        // Defining various endpoints
        // Generally say that when http://localhost:9080/settings/lowBattery/ is called, the getLowBatteryRoute function should be called.

        Routes::Get(router, "/ready", Routes::bind(&Generic::handleReady));
        Routes::Get(router, "/auth", Routes::bind(&SmartwatchEndpoint::doAuth, this));

        Routes::Post(router, "/settings/lowBattery/:value", Routes::bind(&SmartwatchEndpoint::setLowBatteryRoute, this));
        Routes::Get(router, "/settings/lowBattery/", Routes::bind(&SmartwatchEndpoint::getLowBatteryRoute, this));

        Routes::Post(router, "/settings/lantern/:value", Routes::bind(&SmartwatchEndpoint::setLanternRoute, this));
        Routes::Get(router, "/settings/lantern/", Routes::bind(&SmartwatchEndpoint::getLanternRoute, this));
    }

    
    void doAuth(const Rest::Request& request, Http::ResponseWriter response) {
        // Function that prints cookies
        printCookies(request);
        // In the response object, it adds a cookie regarding the communications language.
        response.cookies()
            .add(Http::Cookie("lang", "en-US"));
        // Send the response
        response.send(Http::Code::Ok);
    }

    // Endpoint to configure the low battery setting
    void setLowBatteryRoute(const Rest::Request& request, Http::ResponseWriter response){
        string settingName = "lowBattery";

        // This is a guard that prevents editing the same value by two concurent threads. 
        Guard guard(SmartwatchLock);

        unsigned val = 0;
        if (request.hasParam(":value")) {
            auto value = request.param(":value");
            val = value.as<unsigned>();
        }

        // Setting the Smartwatch's setting to value
        int setResponse = swt.setLowBatterySetting(val);

        // Sending some confirmation or error response.
        if (setResponse == 1) {
            // current battery level is less than 20%
            response.send(Http::Code::Ok, settingName + " was set to true");
        } else if (setResponse == 2) {
            // current battery level is higher than 20%
            response.send(Http::Code::Ok, settingName + " was set to false");
        } else {
            response.send(Http::Code::Not_Found, settingName + " was not found and or " + std::to_string(val) + " was not a valid value ");
        }
    }


    // Setting to get the settings value of the low battery setting
    void getLowBatteryRoute(const Rest::Request& request, Http::ResponseWriter response){
        string settingName = "lowBattery";

        Guard guard(SmartwatchLock);

        string valueSetting = swt.getLowBatterySetting();

        if (valueSetting != "") {
            // In this response I also add a couple of headers,
            // describing the server that sent this response, and the way the content is formatted.
            using namespace Http;
            response.headers()
                        .add<Header::Server>("pistache/0.1")
                        .add<Header::ContentType>(MIME(Text, Plain));

            if (std::stoi(valueSetting) == 0) {
                response.send(Http::Code::Ok, settingName + " setting is set to false.");
            } else {
                response.send(Http::Code::Ok, settingName + " setting is set to true.");
            }
        }
        else {
            response.send(Http::Code::Not_Found, settingName + " was not found");
        }
    }

    // Endpoint to configure the lantern setting
    void setLanternRoute(const Rest::Request& request, Http::ResponseWriter response) {
        string settingName = "lantern";

        // This is a guard that prevents editing the same value by two concurent threads. 
        Guard guard(SmartwatchLock);

        string val = "";
        if (request.hasParam(":value")) {
            auto value = request.param(":value");
            val = value.as<string>();
        }

        // Setting the Smartwatch's setting to value
        int setResponse = swt.setLanternSetting(val);

        // Sending some confirmation or error response.
        if (setResponse == 1) {
            response.send(Http::Code::Ok, settingName + " was set to " + val);
        }
        else {
            response.send(Http::Code::Not_Found, settingName + " was not found and or '" + val + "' was not a valid value ");
        }
    }

    // Setting to get the settings value of the lantern setting
    void getLanternRoute(const Rest::Request& request, Http::ResponseWriter response) {
        string settingName = "lantern";

        Guard guard(SmartwatchLock);

        string valueSetting = swt.getLanternSetting();

        if (valueSetting != "") {
            using namespace Http;
            response.headers()
                        .add<Header::Server>("pistache/0.1")
                        .add<Header::ContentType>(MIME(Text, Plain));

            if (std::stoi(valueSetting) == 0) {
                response.send(Http::Code::Ok, settingName + " is set to " + valueSetting + ". Value 0 means setting is off.");
            } else {
                response.send(Http::Code::Ok, settingName + " is set to " + valueSetting + ". Value 1 means setting is on.");
            }
        }
        else {
            response.send(Http::Code::Not_Found, settingName + " was not found");
        }
    }

    // Defining the class of the Smartwatch. It should model the entire configuration of the Smartwatch
    class Smartwatch {
    public:
        explicit Smartwatch(){ }

        // Setter for the low battery setting
        int setLowBatterySetting(unsigned value){
            // if battery is less than 20%
            bool isLessThan20 = value < lowBattery.batteryMilliAmpH * 0.2;

            if(isLessThan20){
                lowBattery.value = true;
                return 1;
            }
            
            if (!isLessThan20) {
                lowBattery.value = false;
                return 2;
            }

            // otherwise we received a bad value
            return 0;
        }

        // Getter for the low battery setting
        string getLowBatterySetting(){
            return std::to_string(lowBattery.value);
        }

        // Setter for the lantern setting
        int setLanternSetting(std::string value){
            if(value == "true"){
                lantern.value = true;
                return 1;
            }
            if(value == "false"){
                lantern.value = false;
                return 1;
            }
            return 0;
        }

        // Getter for the lantern setting
        string getLanternSetting(){
            return std::to_string(lantern.value);
        }
    
    private:
        // Defining lantern setting
        struct lanternSetting {
            std::string name = "lantern";
            bool value = false;
        } lantern;

        // Defining lowBattery setting
        struct lowBatterySetting {
            std::string name = "lowBattery";
            unsigned batteryMilliAmpH = 3600;
            bool value = false;
        } lowBattery;

    };

    // Create the lock which prevents concurrent editing of the same variable
    using Lock = std::mutex;
    using Guard = std::lock_guard<Lock>;
    Lock SmartwatchLock;

    // Instance of the Smartwatch model
    Smartwatch swt;

    // Defining the httpEndpoint and a router.
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

int main(int argc, char *argv[]) {

    // This code is needed for gracefull shutdown of the server when no longer needed.
    sigset_t signals;
    if (sigemptyset(&signals) != 0
            || sigaddset(&signals, SIGTERM) != 0
            || sigaddset(&signals, SIGINT) != 0
            || sigaddset(&signals, SIGHUP) != 0
            || pthread_sigmask(SIG_BLOCK, &signals, nullptr) != 0) {
        perror("install signal handler failed");
        return 1;
    }

    // Set a port on which your server to communicate
    Port port(9080);

    // Number of threads used by the server
    int thr = 2;

    if (argc >= 2) {
        port = static_cast<uint16_t>(std::stol(argv[1]));

        if (argc == 3)
            thr = std::stoi(argv[2]);
    }

    Address addr(Ipv4::any(), port);

    cout << "Cores = " << hardware_concurrency() << endl;
    cout << "Using " << thr << " threads" << endl;

    // Instance of the class that defines what the server can do.
    SmartwatchEndpoint stats(addr);

    // Initialize and start the server
    stats.init(thr);
    stats.start();


    // Code that waits for the shutdown sinal for the server
    int signal = 0;
    int status = sigwait(&signals, &signal);
    if (status == 0)
    {
        std::cout << "received signal " << signal << std::endl;
    }
    else
    {
        std::cerr << "sigwait returns " << status << std::endl;
    }

    stats.stop();
}