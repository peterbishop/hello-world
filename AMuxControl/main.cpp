#include "serialport.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
    if ( argc < 2 ){
        cout << "This program opens selected serial port and sends single character 't',\n" \
                "which is TOGGLE command for Audio Mux from www.JumperOne.com\n\n";
        cout << "Usage:\n    AMuxControl.exe [port number]\n";
        return 0;
    }

    int port_number = 0;
    if ( !(port_number = atoi(argv[1])) || !(port_number > 0 && port_number < 256) ){
        cout << "Wrong port number!\n";
        return 1;
    }

    SerialPort port;

    if ( !port.Open(port_number) ){
        cout << "Can't open port!\n";
        return 2;
    }

    if ( !port.SetPortParams() || !port.SetPortTimeouts(50,50,10,50,10) ){
        cout << "Can't set port params";
        return 3;
    }

    if ( !port.SendString("t") ){
        cout << "Can't send data to the port\n";
        return 4;
    }

    port.Close();

    return 0;
}
