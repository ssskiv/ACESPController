#include "requirements.hpp"

#ifndef ETH
#define ETH

EthernetServer server(80);

byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xAF, 0xFE, 0xED};

struct options
{
    uint32_t validity; // value used to check EEPROM validity
    uint8_t mac[6];    // local MAC address
    uint8_t dhcp;      // 0: static IP address, otherwise: DHCP address
    uint8_t ip[4];     // local IP address
    uint8_t netmask[4];
    uint8_t gateway[4];
    uint8_t subnet[4];
    uint8_t dns[4];
};

options config;
bool optionsState[4] = {false, false, false, false}; // IP, MAC, Gateway, Subnet

const options default_conf = {
    VALID,                                // validity
    {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}, // mac
    1,                                    // dhcp
    {10, 10, 10, 25},                     // ip
    {10, 10, 10, 1},                      // netmask
    {10, 10, 10, 1},                      // gateway
    {255, 255, 255, 0},                   // subnet
    {10, 10, 10, 1},                      // dns
};

void saveConf()
{
    EEPROM.put(X * COM_COUNT + 10, config);
    EEPROM.commit();
}

void setDefaultConf()
{
    Serial.println("Setting default config");
    memcpy(&config, &default_conf, sizeof(options));
    saveConf();
}

void printConf()
{
    Serial.print("MAC address: ");
    for (int i = 0; i < 6; ++i)
    {
        if (i > 0)
        {
            Serial.print(':');
        }
        if (config.mac[i] < 0x10)
        {
            Serial.print('0');
        }
        Serial.print(config.mac[i], HEX);
    }
    Serial.println();

    Serial.print("DHCP: ");
    Serial.println(config.dhcp ? "ON" : "OFF");

    Serial.print("IP address: ");
    Serial.println(IPAddress(config.ip));
}

void loadConf()
{
    EEPROM.get(X * COM_COUNT + 10, config);
    printConf();
    if (config.validity != VALID)
    {
        setDefaultConf();
        Serial.println("LOADING DEFAULT CONFIG");
    }
}

void setupETH()
{
    loadConf();
    printConf();
    Ethernet.init(5);
    if (config.dhcp)
        Ethernet.begin(config.mac);
    else
        Ethernet.begin(config.mac, IPAddress(config.ip), IPAddress(config.dns), IPAddress(config.gateway), IPAddress(config.subnet));

    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
        Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    }
    if (Ethernet.linkStatus() == LinkOFF)
    {
        Serial.println("Ethernet cable is not connected.");
    }
    else
        Serial.println(Ethernet.localIP());
    server.begin();
}

String readString;

String dataFromRequestString(String request, String keyword, uint8_t keywordLen)
{
    return request.substring(request.indexOf("?input" + keyword + "=") + 7 + keywordLen, request.lastIndexOf("HTTP"));
}

byte parseNibble(const char *arg)
{
    if (*arg >= '0' && *arg <= '9')
    {
        return *arg - '0';
    }
    else if (*arg >= 'a' && *arg <= 'f')
    {
        return *arg - 'a' + 0x0a;
    }
    else if (*arg >= 'A' && *arg <= 'F')
    {
        return *arg - 'A' + 0x0a;
    }
    return 0;
}

byte parseByte(const char *arg)
{
    byte ret = parseNibble(arg++) << 4;
    return ret + parseNibble(arg);
}

String button(String keyword)
{
    return R"rawliteral(<form action="/get"> input )rawliteral" + keyword + R"rawliteral(: <input type="text" name="input)rawliteral" + keyword + R"rawliteral(">
                                    <input type="submit" value="Submit">
                                    </form>)rawliteral";
}
////////////////////////////////////////////////////////////////////////////////////////////////////

void loopETH(float temp0 = 0, float temp1 = 0, float temp2 = 0, bool water0 = 0, bool water1 = 0, float humidity = 0)
{
    // if an incoming client connects, there will be bytes available to read:
    EthernetClient client = server.available();

    if (client)
    {
        bool currentLineIsBlank = true;
        while (client.connected())
        {
            if (client.available())
            {
                char c = client.read();
                ////Serial.write(c);

                if (readString.length() < 100)
                {
                    // store characters to string
                    readString += c;
                    // Serial.print(c);
                }

                // if you've gotten to the end of the line (received a newline
                // character) and the line is blank, the HTTP request has ended,
                // so you can send a reply
                if (c == '\n' && currentLineIsBlank)
                {
                    // send a standard HTTP response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close"); // the connection will be closed after completion of the response
                                                         // refresh the page automatically every 5 sec
                    client.println();
                    client.println("<!DOCTYPE HTML>");
                    client.println("<html>");
                    // output the value of each analog input pin
                    // for (int analogChannel = 0; analogChannel < 6; analogChannel++)
                    // {
                    //     int sensorReading = analogRead(analogChannel);
                    //     client.print("analog input ");
                    //     client.print(analogChannel);
                    //     client.print(" is ");
                    //     client.print(sensorReading);
                    //     client.println("<br />");
                    // }
                    client.println("TEMP 0 =" + String(temp0) + "<br>");
                    client.println("TEMP 1 =" + String(temp1) + "<br>");
                    client.println("TEMP 2 =" + String(temp2) + "<br>");
                    client.println("HUMIDITY =" + String(humidity) + "<br>");
                    client.println("WATER 0 =" + String(water0) + "<br>");
                    client.println("WATER 1 =" + String(water1) + "<br>");
                    client.println("<a href=\"/?ON1\"\">Turn On AC 1</a><br>");
                    client.println("<a href=\"/?ON2\"\">Turn On AC 2</a><br>");
                    client.println("<a href=\"/?OFF1\"\">Turn Off AC 1</a><br>");
                    client.println("<a href=\"/?OFF2\"\">Turn Off AC 2</a><br>");
                    client.println(button("IP"));
                    client.println(button("MAC"));
                    client.println(button("Gateway"));
                    client.println(button("DNS"));
                    client.println(button("Subnet"));
                    client.println(button("Netmask"));
                    client.println(button("DHCP"));
                    client.println("</html>");
                    break;
                }
                if (c == '\n')
                {
                    // you're starting a new line
                    currentLineIsBlank = true;
                }
                else if (c != '\r')
                {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }
        delay(1);
        // close the connection:
        client.stop();

        if (readString.indexOf("?ON1") > 0)
        {
            lcdSendCode(0);
        }
        if (readString.indexOf("?ON2") > 0)
        {
            lcdSendCode(2);
        }
        if (readString.indexOf("?OFF1") > 0)
        {
            lcdSendCode(1);
        }
        if (readString.indexOf("?OFF2") > 0)
        {
            lcdSendCode(3);
        }
        if (readString.indexOf("inputIP") > 0)
        {
            IPAddress _ip;
            String ipStr = dataFromRequestString(readString, "IP", 2);
            Serial.println(ipStr);
            _ip.fromString(ipStr);
            for (int i = 0; i < 4; ++i)
                config.ip[i] = _ip[i];
            saveConf();
        }
        if (readString.indexOf("inputMAC") > 0)
        {
            String macStr = dataFromRequestString(readString, "MAC", 3);
            const char *ptr = &macStr[0];
            for (int i = 0; i < 6; ++i)
            {
                config.mac[i] = parseByte(ptr);
                ptr += 3;
            }
            saveConf();
        }
        if (readString.indexOf("inputGateway") > 0)
        {
            IPAddress _gw;
            String gwStr = dataFromRequestString(readString, "Gateway", 7);
            _gw.fromString(gwStr);
            for (int i = 0; i < 4; ++i)
                config.gateway[i] = _gw[i];
            saveConf();
        }
        if (readString.indexOf("inputSubnet") > 0)
        {
            IPAddress _sn;
            String snStr = dataFromRequestString(readString, "Subnet", 6);
            _sn.fromString(snStr);
            for (int i = 0; i < 4; ++i)
                config.subnet[i] = _sn[i];
            saveConf();
        }
        if (readString.indexOf("inputDHCP") > 0)
        {
            String dhcpStr = dataFromRequestString(readString, "DHCP", 4);
            Serial.println(dhcpStr);
            config.dhcp = dhcpStr.toInt();
            saveConf();
        }
    }
    readString = "";
}
#endif