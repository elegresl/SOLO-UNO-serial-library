#include </usr/include/libserial/SerialPort.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <cmath>
#include <bitset>

#define SPEED_REFERENCE 0x05
#define TORQUE_REFERENCE 0x04
#define SPEED_FEEDBACK 0x96
#define PORT_NAME "/dev/ttyACM0"    // A SOLO UNO typically enumerates as ttyACM0.
#define PORT_NAME_2 "/dev/ttyACM1"  // A SOLO UNO may enumerate as ttyACM1 if a solo uno is already connected.

class SoloUno {
public:
    SoloUno(char add) {

        this->address = add;
        initSolo();
    }

    int soloWrite(char cmd, int data) {
        serial_port.FlushIOBuffers();
        serial_port.FlushInputBuffer();
        serial_port.FlushOutputBuffer();

        std::ofstream outputFile("testing.txt");
        std::ofstream outputFile2("testing_transmission_issue.txt");
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
            
            std::stringstream ss;
            ss << std::hex << data_byte[x];
            std::string hexString = ss.str();
            outputFile << ss.str();

            serial_port.WriteByte(data_byte[x]);
            serial_port.DrainWriteBuffer();
        }
        
        

        std::string reading;
        std::string writtenValue = std::string(1, data_byte[0]) + std::string(1, data_byte[1]) + std::string(1, data_byte[2]) +
                                   std::string(1, data_byte[3]) + std::string(1, data_byte[4]) + std::string(1, data_byte[5]) +
                                   std::string(1, data_byte[6]) + std::string(1, data_byte[7]) + std::string(1, data_byte[8]) +
                                   std::string(1, data_byte[9]);

        std::cout << "Byte written to SOLO: " + writtenValue << std::endl;

        serial_port.Read(reading, 10, 50);

        std::stringstream ss;
        ss << reading;
        std::cout << ss.str() << std::endl;

        outputFile2 << ss.str();
        outputFile.close();
        outputFile2.close();

        if (reading != writtenValue) {
            std::cout << "SOLO UNO WRITE ERROR" << std::endl;
            return 1; // Write failure.
        }
        else{
            return 0; // Successful write.
        }
    }

    uint32_t soloRead(char cmd) {
        serial_port.FlushIOBuffers();
        serial_port.FlushInputBuffer();
        serial_port.FlushOutputBuffer();

        std::ofstream outputFile("solo_read.txt");
        std::ofstream receivedData("received_data.txt");

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
        serial_port.Read(reading, 10, 50);

       std::stringstream ss;
        ss << reading;

        receivedData << reading;
        receivedData.close();

        uint32_t formattedRead = formatRead(reading);  

        outputFile.close();
        return formattedRead;

    }

    uint64_t readSpeed() {
        
        uint64_t speed = soloRead(SPEED_FEEDBACK);
        return speed;

    }

    void end(){

        serial_port.Close();

    }
    void setTorque(double data){
        
        int datt = data;
        int dat = doubleToFixedPoint(data); // Torque reference uses fixed point 32-17 for the data.
        soloWrite(TORQUE_REFERENCE, dat);

    }

    void setSpeed(double data){
        
        int dat = floor(data); // Speed reference uses unsigned int for the data.
        std::cout << "Speed was instructed to be: " + dat << std::endl;
        soloWrite(SPEED_REFERENCE, dat);

    }
private:
    
    uint32_t formatRead(std::string reading){
        uint32_t formattedInt = 0;

        unsigned char b0 = static_cast<unsigned char>(reading[4]);
        unsigned char b1 = static_cast<unsigned char>(reading[5]);
        unsigned char b2 = static_cast<unsigned char>(reading[6]);
        unsigned char b3 = static_cast<unsigned char>(reading[7]);

        formattedInt = (static_cast<uint32_t>(b0) << 24) |
                       (static_cast<uint32_t>(b1) << 16) |
                       (static_cast<uint32_t>(b2) << 8) |
                       static_cast<uint32_t>(b3);
        //std::cout << "Formatted int: " << formattedInt << std::endl;

        return formattedInt;
    }

    void initSolo() {
         try {
             serial_port.Open(PORT_NAME);
         } catch (const LibSerial::OpenFailed&) {
             try{
                 serial_port.Open(PORT_NAME_2);
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

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Add delay to ensure the port is ready.

    }
    double fixedPointToDouble(int data){
            // This process was built with guidance from the official SOLO UNO communicatio manual.

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
         // This process was built with guidance from the official SOLO UNO communicatio manual.
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
             result = eightBytesF - roundedDown + 1;

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

    //solo1.soloWrite(TORQUE_REFERENCE, 0x01); 
    //std::cout << "First line completed." << std::endl;

    //solo1.setSpeed(100.5);
    //std::cout << "Second line completed." << std::endl;

    //solo1.setTorque(4.2);
    //std::cout << "Third line completed." << std::endl;
    
    //uint32_t ad = solo1.soloRead(SPEED_FEEDBACK);
    //std::cout << "Fourth line completed." << ad << std::endl;

    uint32_t ad = solo1.readSpeed();
    std::cout << "Fifth line completed." << ad << std::endl;

    solo1.end();

    return EXIT_SUCCESS;
}
