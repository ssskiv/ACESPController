#include "requirements.hpp"

EthernetServer server(80);

byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xAF, 0xFE, 0xED};

void setupETH()
{
    // SPI.begin(18,19,23,5);
    Ethernet.init(5);
    Ethernet.begin(mac);

    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
        Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
        //  while (true)
        //  {
        //      delay(1); // do nothing, no point running without Ethernet hardware
        //  }
    }
    if (Ethernet.linkStatus() == LinkOFF)
    {
        Serial.println("Ethernet cable is not connected.");
    }
    else
         Serial.println(Ethernet.localIP());
         Serial.println(Ethernet.localIP());
        server.begin();
}

String readString;

void loopETH(float temp1 = 0, float temp2 = 0, bool water1 = 0, bool water2 = 0)
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
                    client.println("TEMP 1 =" + String(temp1));
                    client.println("<a href=\"/?ON1\"\">Turn On AC 1</a>");
                    client.println("<a href=\"/?ON2\"\">Turn On AC 2</a>");
                    client.println("<a href=\"/?OFF1\"\">Turn Off AC 1</a>");
                    client.println("<a href=\"/?OFF2\"\">Turn Off AC 2</a>");
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
    }
}