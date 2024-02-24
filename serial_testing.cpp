#include </usr/include/libserial/SerialPort.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>

class SoloUno {
public:
    SoloUno(const std::string& port_name) : serial_port(port_name) {
        this->port_name = port_name;
        initSolo();
    }

    void soloWrite(char addr, char cmd, int data) {
        serial_port.FlushIOBuffers();
        serial_port.FlushInputBuffer();
        serial_port.FlushOutputBuffer();

        std::ofstream outputFile("speed_read_hex.txt");

        char initiator = 0xFF;
        char address = addr;
        char command = cmd;
        int dataIn = data;

        char data0 = static_cast<char>((dataIn >> 24) & 0xFF);
        char data1 = static_cast<char>((dataIn >> 16) & 0xFF);
        char data2 = static_cast<char>((dataIn >> 8) & 0xFF);
        char data3 = static_cast<char>((dataIn & 0xFF));

        char crc = 0x00;
        char ending = 0xFE;

        char data_byte[] = {initiator, initiator, address, command, data0, data1, data2, data3, crc, ending};

        for (int x = 0; x < 10; x++) {
            serial_port.WriteByte(data_byte[x]);
            serial_port.DrainWriteBuffer();
        }

        std::string reading;
        std::string writtenValue = std::string(1, data_byte[0]) + std::string(1, data_byte[1]) + std::string(1, data_byte[2]) +
                                   std::string(1, data_byte[3]) + std::string(1, data_byte[4]) + std::string(1, data_byte[5]) +
                                   std::string(1, data_byte[6]) + std::string(1, data_byte[7]) + std::string(1, data_byte[8]) +
                                   std::string(1, data_byte[9]);

        serial_port.Read(reading, 10, 5000);

        std::stringstream ss;
        ss << reading;
        std::cout << ss.str() << std::endl;

        outputFile << ss.str();
        outputFile.close();
        serial_port.Close();

        if (reading != writtenValue) {
            std::cout << "SOLO UNO WRITE ERROR" << std::endl;
        }
    }

    void soloRead(char addr, char cmd) {
        serial_port.FlushIOBuffers();
        serial_port.FlushInputBuffer();
        serial_port.FlushOutputBuffer();

        std::ofstream outputFile("serial_read.txt");

        char initiator = 0xFF;
        char address = addr;
        char command = cmd;

        char crc = 0x00;
        char ending = 0xFE;
        char data = 0x00;

        char data_byte[] = {initiator, initiator, address, command, data, data, data, data, crc, ending};

        for (int x = 0; x < 10; x++) {
            serial_port.WriteByte(data_byte[x]);
            serial_port.DrainWriteBuffer();
        }

        std::string reading;
        std::string writtenValue = std::string(1, data_byte[0]) + std::string(1, data_byte[1]) + std::string(1, data_byte[2]) +
                                   std::string(1, data_byte[3]) + std::string(1, data_byte[4]) + std::string(1, data_byte[5]) +
                                   std::string(1, data_byte[6]) + std::string(1, data_byte[7]) + std::string(1, data_byte[8]) +
                                   std::string(1, data_byte[9]);

        serial_port.Read(reading, 10, 5000);

        std::stringstream ss;
        ss << reading;
        std::cout << ss.str() << std::endl;

        outputFile << ss.str();
        outputFile.close();
        serial_port.Close();
    }

    int soloReadSpeed(char addr) {
        serial_port.FlushIOBuffers();
        serial_port.FlushInputBuffer();
        serial_port.FlushOutputBuffer();

        std::ofstream outputFile("serial_read.txt");
        std::ofstream outputFile1("speed_read_decimal.txt");

        char initiator = 0xFF;
        char address = addr;
        char command = 0x96;

        char crc = 0x00;
        char ending = 0xFE;
        char data = 0x00;

        char data_byte[] = {initiator, initiator, address, command, data, data, data, data, crc, ending};

        for (int x = 0; x < 10; x++) {
            serial_port.WriteByte(data_byte[x]);
            serial_port.DrainWriteBuffer();
        }

        std::string reading;
        std::string writtenValue = std::string(1, data_byte[0]) + std::string(1, data_byte[1]) + std::string(1, data_byte[2]) +
                                   std::string(1, data_byte[3]) + std::string(1, data_byte[4]) + std::string(1, data_byte[5]) +
                                   std::string(1, data_byte[6]) + std::string(1, data_byte[7]) + std::string(1, data_byte[8]) +
                                   std::string(1, data_byte[9]);

        serial_port.Read(reading, 10, 5000);

        char d0 = reading[4];
        char d1 = reading[5];
        char d2 = reading[6];
        char d3 = reading[7];

        std::stringstream ss;
        std::stringstream ss1;

        ss1 << reading;
        ss << reading[4] + reading[5] + reading[6] + reading[7];

        std::cout << ss1.str() << std::endl;

        outputFile << ss1.str();
        outputFile1 << ss.str();

        outputFile.close();
        outputFile1.close();
        serial_port.Close();

        return 1;
    }

private:
    void initSolo() {
        // try {
        //     serial_port.Open(port_name);
        // } catch (const LibSerial::OpenFailed&) {
        //     try{
        //         serial_port.Open("/dev/ttyACM1");
        //     } catch(const LibSerial::OpenFailed&){
        //         std::cerr << "The serial port did not open correctly." << std::endl;
        //         return;
        //     }
        serial_port.Open(port_name);    
        

        serial_port.SetBaudRate(LibSerial::BaudRate::BAUD_115200);
        serial_port.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
        serial_port.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
        serial_port.SetParity(LibSerial::Parity::PARITY_NONE);
        serial_port.SetStopBits(LibSerial::StopBits::STOP_BITS_1);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

private:
    LibSerial::SerialPort serial_port;
    std::string port_name;
};

int main() {
    SoloUno solo1("/dev/ttyACM0");
    solo1.soloReadSpeed(0x00);

    return EXIT_SUCCESS;
}
