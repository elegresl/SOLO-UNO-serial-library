#include </usr/include/libserial/SerialPort.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>

#define SPEED_REFERENCE 0x05
#define TORQUE_REFERENCE 0x04
#define SPEED_FEEDBACK 0x96

class SoloUno {
public:
    SoloUno(char add) {

        this->address = add;
        initSolo();
    }

    void soloWrite(char cmd, int data) {
        serial_port.FlushIOBuffers();
        serial_port.FlushInputBuffer();
        serial_port.FlushOutputBuffer();

        std::ofstream outputFile("speed_read_hex.txt");

        char initiator = 0xFF;
        char address = this->address;
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

    void soloRead(char cmd) {
        serial_port.FlushIOBuffers();
        serial_port.FlushInputBuffer();
        serial_port.FlushOutputBuffer();

        std::ofstream outputFile("serial_read.txt");

        char initiator = 0xFF;
        char address = this->address;
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

    double readSpeed() {
        
        double speed = soloRead(SPEED_FEEDBACK);
        

        return speed;
    }

    void end(){

        serial_port.Close();

    }
    void setTorque(int data){

        int dat = data;
        soloWrite(TORQUE_REFERENCE, dat);

    }

    void setSpeed(int data){
        
        int dat = data;
        soloWrite(SPEED_REFERENCE, dat);

    }
private:

    void initSolo() {
         try {
             serial_port.Open(port_name);
         } catch (const LibSerial::OpenFailed&) {
             try{
                 serial_port.Open("/dev/ttyACM1");
             } catch(const LibSerial::OpenFailed&){
                 std::cerr << "The serial port did not open correctly." << std::endl;
                 return;
             }    
         }
        serial_port.SetBaudRate(LibSerial::BaudRate::BAUD_115200);
        serial_port.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
        serial_port.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
        serial_port.SetParity(LibSerial::Parity::PARITY_NONE);
        serial_port.SetStopBits(LibSerial::StopBits::STOP_BITS_1);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    double fixedPointToDouble(int data){
            double result;
            double den = 131072;

            if(data <= 0x7FFE0000){

                result = data/den;

            }
            else{

                result = data/den*-1;

            }
            return result;
    }

    int doubleToFixedPoint(double data){

        unsigned int result;
        double mult = 131072;

        if(data >= 0){

            double prod = data*mult;
            int roundedDown = static_cast<int>(floor(prod));
            result = roundedDown;

        }
        if(data < 0){

            double prod = data*mult;
            unsigned int roundedDown = abs(static_cast<int>(floor(prod)));
            unsigned int eightBytesF = 0xFFFFFFFF;
            result = eightBytesF - roundedDown +1;
        }

        return result;
    }

private:
    LibSerial::SerialPort serial_port;
    std::string port_name;
    char address;
};

int main() {

    SoloUno solo1(0x00);
    solo1.readSpeed();
    solo1.end();

    return EXIT_SUCCESS;
}
