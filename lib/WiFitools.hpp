#include "../include/requirements.hpp"

const char *ssid = "ACAccessPoint";
const char *password = NULL;
const char *serverName = "*";

WebServer webServer(80);
DNSServer dnsServer;
IPAddress IP(192, 168, 4, 1);

String responseHTML = "<!DOCTYPE html><html>"
                      "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                      "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}"
                      "</style></head>"
                      "<body><h1>ESP32 Web Server</h1>"
                      "<p>Hello World</p>"
                      "</body></html>";

void setupWiFi()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    delay(100);
    WiFi.softAPConfig(IP, IP, IPAddress(255, 255, 255, 0));

    dnsServer.start(53, serverName, IP);
    webServer.onNotFound([]()
                         { webServer.send(200, "text/html", responseHTML); });
    webServer.begin();
}

void loopWiFi()
{
    dnsServer.processNextRequest();
    webServer.handleClient();
}