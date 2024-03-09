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

    int soloWriteFast(char cmd, int data) {
     

        serial_port.FlushIOBuffers();
        serial_port.FlushInputBuffer();
        serial_port.FlushOutputBuffer();


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
     
        return 0;
        
    }

    int soloWriteSlow(char cmd, int data) { // Slower because it checks for errors in transmission.

        serial_port.FlushIOBuffers();
        serial_port.FlushInputBuffer();
        serial_port.FlushOutputBuffer();


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

        serial_port.Read(reading, 10, 1);

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
       
        uint32_t formattedRead = formatRead(reading);  
       
        return formattedRead;

    }

    uint64_t readSpeed() {

        uint64_t speed = soloRead(SPEED_FEEDBACK);

        return speed;

    }

    void end(){

        serial_port.Close();

    }
    void setTorqueFast(double data){

        int datt = data;
        int dat = doubleToFixedPoint(data); // Torque reference uses fixed point 32-17 for the data.
        soloWriteFast(TORQUE_REFERENCE, dat);

    }

    void setTorqueSlow(double data){

        int datt = data;
        int dat = doubleToFixedPoint(data); // Torque reference uses fixed point 32-17 for the data.
        soloWriteSlow(TORQUE_REFERENCE, dat);
      
    }

    void setSpeedFast(double data){

        int dat = floor(data); // Speed reference uses unsigned int for the data.
        soloWriteFast(SPEED_REFERENCE, dat);


    }

    void setSpeedSlow(double data){
        

        int dat = floor(data); // Speed reference uses unsigned int for the data.
        soloWriteSlow(SPEED_REFERENCE, dat);
        
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

        serial_port.SetBaudRate(LibSerial::BaudRate::BAUD_921600);
        serial_port.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
        serial_port.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
        serial_port.SetParity(LibSerial::Parity::PARITY_NONE);
        serial_port.SetStopBits(LibSerial::StopBits::STOP_BITS_1);

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Add delay to ensure the port is ready.

    }
    double fixedPointToDouble(int data){
            // This process was built with guidance from the official SOLO UNO communication manual.

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

    int count = 0;

    double averageFastGeneralWrite;
    double fastGeneralWriteSum = 0;
    double averageSlowGeneralWrite ;
    double slowGeneralWriteSum = 0;

    double averageFastTorqueSetting;
    double fastTorqueSettingSum = 0;
    double averageSlowTorqueSetting;
    double slowTorqueSettingSum = 0;

    double averageFastSpeedSetting;
    double fastSpeedSettingSum = 0;
    double averageSlowSpeedSetting;
    double slowSpeedSettingSum = 0;

    double averageSpeedReading;
    double speedReadingSum = 0;
    
    SoloUno solo1(0x00);

    double limit = 1000.0;
    while(count <= (int) limit){
    
    auto start_time = std::chrono::high_resolution_clock::now();
    solo1.soloWriteFast(TORQUE_REFERENCE, 0x01);
    auto end_time = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    

    fastGeneralWriteSum += duration.count();

    start_time = std::chrono::high_resolution_clock::now();
    solo1.soloWriteSlow(TORQUE_REFERENCE, 0x01); 
    end_time = std::chrono::high_resolution_clock::now(); 
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    slowGeneralWriteSum += duration.count();
    
    start_time = std::chrono::high_resolution_clock::now();
    solo1.setTorqueFast(4.2);
    end_time = std::chrono::high_resolution_clock::now(); 
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    fastTorqueSettingSum += duration.count();

    start_time = std::chrono::high_resolution_clock::now();
    solo1.setTorqueSlow(4.2);
    end_time = std::chrono::high_resolution_clock::now(); 
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    slowTorqueSettingSum += duration.count();

    start_time = std::chrono::high_resolution_clock::now();
    solo1.setSpeedFast(100.5);
    end_time = std::chrono::high_resolution_clock::now(); 
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    fastSpeedSettingSum += duration.count();

    start_time = std::chrono::high_resolution_clock::now();
    solo1.setSpeedSlow(100.5);
    end_time = std::chrono::high_resolution_clock::now(); 
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    slowSpeedSettingSum += duration.count();

    start_time = std::chrono::high_resolution_clock::now();
    uint32_t ad = solo1.readSpeed();
    end_time = std::chrono::high_resolution_clock::now(); 
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    speedReadingSum += duration.count();


    count++;

    }

    averageFastGeneralWrite = fastGeneralWriteSum/limit;
    std::cout << "_________________________________________________________________" << std::endl;

    std::cout << "Mean fastGeneralWrite() Execution time: " << averageFastGeneralWrite << " microseconds" << std::endl;

    averageSlowGeneralWrite = slowGeneralWriteSum/limit;
    std::cout << "Mean slowGeneralWrite() Execution time: " << averageSlowGeneralWrite << " microseconds" << std::endl;

    std::cout << "_________________________________________________________________" << std::endl;

    averageFastTorqueSetting = fastTorqueSettingSum/limit;
    std::cout << "Mean fastTorqueSetting() Execution time: " << averageFastTorqueSetting << " microseconds" << std::endl;

    averageSlowTorqueSetting = slowTorqueSettingSum/limit;
    std::cout << "Mean slowTorqueSetting() Execution time: " << averageSlowTorqueSetting << " microseconds" << std::endl;

    std::cout << "_________________________________________________________________" << std::endl;
    
    averageFastSpeedSetting = fastSpeedSettingSum/limit;
    std::cout << "Mean fastSpeedSetting() Execution time: " << averageFastSpeedSetting << " microseconds" << std::endl;
    
    averageSlowSpeedSetting = slowSpeedSettingSum/limit;
    std::cout << "Mean slowSpeedSetting() Execution time: " << averageSlowSpeedSetting << " microseconds" << std::endl;

    std::cout << "_________________________________________________________________" << std::endl;
    
    averageSpeedReading = speedReadingSum/limit;
    std::cout << "Mean speedReading() Execution time: " << averageSpeedReading << " microseconds" << std::endl;
    
    std::cout << "_________________________________________________________________" << std::endl;

    std::cout << "Test iterations: " << limit <<  std::endl;

    solo1.end();

    return EXIT_SUCCESS;
}
