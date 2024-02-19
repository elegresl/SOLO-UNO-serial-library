//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//to compile, execute 'g++ -I/usr/include/libserial testing_serial.cpp -lserial -o testing_serial' in the shell 
//
//
// THIS PROGRAM ONLY HAS AN ISSUE WITH SPLITTING THE 4 BYTE WORD INTO 4 ONE BYTE WORDS!!!!!!!!!!! WRONG OUTPUT!!!
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include </usr/include/libserial/SerialPort.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>

constexpr const char* const SERIAL_PORT_2 = "/dev/ttyACM0" ;

using namespace LibSerial;
SerialPort serial_port;

void soloWrite(char addr, char cmd, int data){

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
    
    serial_port.WriteByte(data_byte[0]);
    serial_port.DrainWriteBuffer();
     
    serial_port.WriteByte(data_byte[1]); 
    serial_port.DrainWriteBuffer() ;
        
    serial_port.WriteByte(data_byte[2]);
    serial_port.DrainWriteBuffer() ;
     
    serial_port.WriteByte(data_byte[3]);
    serial_port.DrainWriteBuffer() ;    

    serial_port.WriteByte(data_byte[4]);
    serial_port.DrainWriteBuffer() ;
        
    serial_port.WriteByte(data_byte[5]);
    serial_port.DrainWriteBuffer() ;
        
    serial_port.WriteByte(data_byte[6]);
    serial_port.DrainWriteBuffer() ;
        
    serial_port.WriteByte(data_byte[7]);
    serial_port.DrainWriteBuffer() ;
             
    serial_port.WriteByte(data_byte[8]);
    serial_port.DrainWriteBuffer() ;
     
    serial_port.WriteByte(data_byte[9]);
    serial_port.DrainWriteBuffer() ;
 
    std::string reading;
    std::string writtenValue = std::string(1, data_byte[0]) + std::string(1, data_byte[1]) + std::string(1, data_byte[2]) + std::string(1, data_byte[3]) + std::string(1, data_byte[4]) + std::string(1, data_byte[5]) +
    std::string(1, data_byte[6]) + std::string(1, data_byte[7]) + std::string(1, data_byte[8]) + std::string(1, data_byte[9]);
 
    serial_port.Read(reading, 10, 5000);

    std::stringstream ss;
    ss << reading;
    std::cout << ss.str() << std::endl;

    
    outputFile << ss.str();
    outputFile.close();
    serial_port.Close();

    if(reading != writtenValue){
        std::cout << "SOLO UNO WRITE ERROR" << std::endl;
    }
}
void soloRead(char addr, char cmd){

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
    
    serial_port.WriteByte(data_byte[0]);
    serial_port.DrainWriteBuffer();
     
    serial_port.WriteByte(data_byte[1]); 
    serial_port.DrainWriteBuffer() ;
        
    serial_port.WriteByte(data_byte[2]);
    serial_port.DrainWriteBuffer() ;
     
    serial_port.WriteByte(data_byte[3]);
    serial_port.DrainWriteBuffer() ;    

    serial_port.WriteByte(data_byte[4]);
    serial_port.DrainWriteBuffer() ;
        
    serial_port.WriteByte(data_byte[5]);
    serial_port.DrainWriteBuffer() ;
        
    serial_port.WriteByte(data_byte[6]);
    serial_port.DrainWriteBuffer() ;
        
    serial_port.WriteByte(data_byte[7]);
    serial_port.DrainWriteBuffer() ;
             
    serial_port.WriteByte(data_byte[8]);
    serial_port.DrainWriteBuffer() ;
     
    serial_port.WriteByte(data_byte[9]);
    serial_port.DrainWriteBuffer() ;
 
    std::string reading;
    std::string writtenValue = std::string(1, data_byte[0]) + std::string(1, data_byte[1]) + std::string(1, data_byte[2]) + std::string(1, data_byte[3]) + std::string(1, data_byte[4]) + std::string(1, data_byte[5]) +
    std::string(1, data_byte[6]) + std::string(1, data_byte[7]) + std::string(1, data_byte[8]) + std::string(1, data_byte[9]);
 
    serial_port.Read(reading, 10, 5000);

    std::stringstream ss;
    ss << reading;
    std::cout << ss.str() << std::endl;

    
    outputFile << ss.str();
    outputFile.close();
    serial_port.Close();

}

int soloReadSpeed(char addr){

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
    
    serial_port.WriteByte(data_byte[0]);
    serial_port.DrainWriteBuffer();
     
    serial_port.WriteByte(data_byte[1]); 
    serial_port.DrainWriteBuffer() ;
        
    serial_port.WriteByte(data_byte[2]);
    serial_port.DrainWriteBuffer() ;
     
    serial_port.WriteByte(data_byte[3]);
    serial_port.DrainWriteBuffer() ;    

    serial_port.WriteByte(data_byte[4]);
    serial_port.DrainWriteBuffer() ;
        
    serial_port.WriteByte(data_byte[5]);
    serial_port.DrainWriteBuffer() ;
        
    serial_port.WriteByte(data_byte[6]);
    serial_port.DrainWriteBuffer() ;
        
    serial_port.WriteByte(data_byte[7]);
    serial_port.DrainWriteBuffer() ;
             
    serial_port.WriteByte(data_byte[8]);
    serial_port.DrainWriteBuffer() ;
     
    serial_port.WriteByte(data_byte[9]);
    serial_port.DrainWriteBuffer() ;
 
    std::string reading;
    std::string writtenValue = std::string(1, data_byte[0]) + std::string(1, data_byte[1]) + std::string(1, data_byte[2]) + std::string(1, data_byte[3]) + std::string(1, data_byte[4]) + std::string(1, data_byte[5]) +
    std::string(1, data_byte[6]) + std::string(1, data_byte[7]) + std::string(1, data_byte[8]) + std::string(1, data_byte[9]);
 
    serial_port.Read(reading, 10, 5000);
    
    char d0 = reading[4];
    char d1 = reading[5];
    char d2 = reading[6];
    char d3 = reading[7];
    
    std::stringstream ss;
    std::stringstream ss1;
    
    ss1 << reading;
    ss << reading[4] + reading[5] + reading[6] + reading[7];
    
    std::cout << ss.str() << std::endl;

    
    outputFile << ss.str();
    outputFile1 << ss1.str();
    
    outputFile.close();
    outputFile1.close();
    serial_port.Close();

    return 1;

}
void initSolo(){

     try
    {
        serial_port.Open(SERIAL_PORT_2) ;
    }
    catch (const OpenFailed&)
    {
        std::cerr << "The serial port did not open correctly." << std::endl ;
        return ;
    }
 
    serial_port.SetBaudRate(BaudRate::BAUD_115200) ;
    serial_port.SetCharacterSize(CharacterSize::CHAR_SIZE_8) ;
    serial_port.SetFlowControl(FlowControl::FLOW_CONTROL_NONE) ;
    serial_port.SetParity(Parity::PARITY_NONE) ; 
    serial_port.SetStopBits(StopBits::STOP_BITS_1) ;

    std::this_thread::sleep_for(std::chrono::seconds(1));
     
}
int main()
{   
    

    initSolo();
    soloReadSpeed(0x00);

 
    return EXIT_SUCCESS ;
}
