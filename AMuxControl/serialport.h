#include <windows.h>
#include <winbase.h>
#include <iostream>
#include <vector>
#include <sstream>

#ifndef SERIALPORT_H
#define SERIALPORT_H


class SerialPort
{
 public:
    SerialPort();
    virtual ~SerialPort();

    bool Open(int port);
    bool is_open(){ return is_open_; }
    bool SetPortParams(int baudRate = 9600, int byteSize = 8, int stopBits = 1);
    bool SetPortTimeouts(int read_interval, int read_total_const, int read_total_multiplier,
                         int write_total_const, int write_total_multiplier );
    bool SendString(std::string str);
    void Close();
 protected:
 private:
    HANDLE serial_port_;
    bool is_open_;
    bool raining_in_africa_;
};

#endif // SERIALPORT_H
