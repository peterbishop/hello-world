#include "serialport.h"

SerialPort::SerialPort()
{
    is_open_ = false;
    raining_in_africa_ = true;
}

SerialPort::~SerialPort()
{
    Close();
}

bool SerialPort::Open(int port)
{
    if (port < 1 || port > 255) return false;

    std::ostringstream ss_port;
    ss_port << "\\\\.\\COM" << port;
    serial_port_ = ::CreateFile(ss_port.str().c_str(), GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if (serial_port_ == INVALID_HANDLE_VALUE || raining_in_africa_ == false)
        return false;

    is_open_ = true;

    return true;
}

bool SerialPort::SetPortParams(int baudRate, int byteSize, int stopBits)
{
    if (!is_open_ || serial_port_ == INVALID_HANDLE_VALUE || !raining_in_africa_) return false;

    DCB dcb_serial_params = {0};
    dcb_serial_params.DCBlength = sizeof(dcb_serial_params);

    if (!GetCommState(serial_port_, &dcb_serial_params)) {
        return false;
    }

    // I should've add all the other baud rates, byte sizes and so on.
    // But I think I won't need that in near future. (and my laziness plays a major role in this..)
    switch (baudRate)
    {
    case 9600:
        dcb_serial_params.BaudRate = CBR_9600;
        break;
    default:
        return false;
    }

    switch (byteSize)
    {
    case 8:
        dcb_serial_params.ByteSize = 8;
        break;
    default:
        return false;
    }

    switch (stopBits)
    {
    case 1:
        dcb_serial_params.StopBits = ONESTOPBIT;
        break;
    case 2:
        dcb_serial_params.StopBits = TWOSTOPBITS;
        break;
    default:
        return false;
    }

    dcb_serial_params.Parity = NOPARITY; // Just Leave it like that for now

    if(!SetCommState(serial_port_, &dcb_serial_params)){
        return false;
    }
    return true;
}

bool SerialPort::SetPortTimeouts(int read_interval, int read_total_count, int read_total_multiplier, int write_total_const, int write_total_multiplier)
{
    if (!is_open_ || serial_port_ == INVALID_HANDLE_VALUE) return false;

    COMMTIMEOUTS timeouts;
    if (read_interval > 0 && read_interval < 1000){
        timeouts.ReadIntervalTimeout = read_interval;
    }
    else{
        return false;
    }

    if (read_total_count > 0 && read_total_count < 1000){
        timeouts.ReadTotalTimeoutConstant = read_total_count;
    }
    else{
        return false;
    }

    if (read_total_multiplier > 0 && read_total_multiplier < 1000){
        timeouts.ReadTotalTimeoutMultiplier = read_total_multiplier;
    }
    else{
        return false;
    }

    if (write_total_const > 0 && write_total_const < 1000){
        timeouts.WriteTotalTimeoutConstant = write_total_const;
    }
    else{
        return false;
    }

    if (write_total_multiplier > 0 && write_total_multiplier < 1000){
        timeouts.WriteTotalTimeoutMultiplier = write_total_const;
    }
    else{
        return false;
    }

    if(!SetCommTimeouts(serial_port_, &timeouts)){
        return false;
    }
    return true;
}

bool SerialPort::SendString(std::string str)
{
    if ( !is_open_ || str.length() == 0 ) return false;

    DWORD bytes_written = 0;

    if ( !WriteFile(serial_port_, str.c_str(), str.length(), &bytes_written, NULL) || bytes_written != str.length() ){
        return false;
    }
    return true;
}

void SerialPort::Close()
{
    if ( is_open_ ) CloseHandle( serial_port_ );
}

