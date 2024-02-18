 //to compile, execute 'g++ -I/usr/include/libserial testing_serial.cpp -lserial -o testing_serial' in the shell 

#include </usr/include/libserial/SerialPort.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>

constexpr const char* const SERIAL_PORT_2 = "/dev/ttyACM0" ;
int main()
{   
    using namespace LibSerial ;
    SerialPort serial_port ;
    try
    {
        serial_port.Open(SERIAL_PORT_2) ;
    }
    catch (const OpenFailed&)
    {
        std::cerr << "The serial port did not open correctly." << std::endl ;
        return EXIT_FAILURE ;
    }
    serial_port.FlushIOBuffers();
    serial_port.FlushInputBuffer();
    serial_port.FlushOutputBuffer();

    // Set the baud rate of the serial port.
    serial_port.SetBaudRate(BaudRate::BAUD_115200) ;

    // Set the number of data bits.
    serial_port.SetCharacterSize(CharacterSize::CHAR_SIZE_8) ;

    // Turn off hardware flow control.
    serial_port.SetFlowControl(FlowControl::FLOW_CONTROL_NONE) ;

  
    serial_port.SetParity(Parity::PARITY_NONE) ;  // Disable parity.
    
    // Set the number of stop bits.
    serial_port.SetStopBits(StopBits::STOP_BITS_1) ;

    // Read characters from the input file and write them to the serial port. 
    std::cout << "Writing data to the serial port." << std::endl ;

        std::ofstream outputFile("ascii.txt");

        char initiator = '/0xFF';
        char address = '/0x00';
        char command = '/0x00';
        char data0 = '/0x00';
        char data1 = '/0x00/;
        char data2 = '/0x00';
        char data3 = '/0x01';
        char crc = '/0x00';
        char ending = '/0xFE';
        
        char data_byte[] = {initiator, initiator, address, command data0, data1, data2, data3, crc, ending};
       
        std::this_thread::sleep_for(std::chrono::seconds(1));
    
        //char data_byte = 0xFF;
        serial_port.WriteByte(data_byte[0]);
        serial_port.DrainWriteBuffer();
        //std::cout << "First byte written." << std::endl ;
     
      
        serial_port.WriteByte(data_byte[1]); 
        serial_port.DrainWriteBuffer() ;
        std::cout << "Second byte written" << std::endl ;
        

          serial_port.WriteByte(data_byte[2]);
        serial_port.DrainWriteBuffer() ;
         std::cout << "Third byte written" << std::endl ;
     
          serial_port.WriteByte(data_byte[3]);
         serial_port.DrainWriteBuffer() ;
         std::cout << "4th byte written" << std::endl ;
        

        serial_port.WriteByte(data_byte[4]);
        serial_port.DrainWriteBuffer() ;
        std::cout << "5th byte written" << std::endl ;
        
          serial_port.WriteByte(data_byte[5]);
         serial_port.DrainWriteBuffer() ;
         std::cout << "6th byte written" << std::endl ;
        
          serial_port.WriteByte(data_byte[6]);
         serial_port.DrainWriteBuffer() ;
        std::cout << "7th byte written" << std::endl ;
        

          serial_port.WriteByte(data_byte[7]);
         serial_port.DrainWriteBuffer() ;
         std::cout << "8th byte written" << std::endl;
        

        
         serial_port.WriteByte(data_byte[8]);
         serial_port.DrainWriteBuffer() ;
         std::cout << "9th byte written" << std::endl;
        

        serial_port.WriteByte(data_byte[9]);
        serial_port.DrainWriteBuffer() ;
        std::cout << "10th byte written" << std::endl;

        std::string reading;
        serial_port.Read(reading, 10, 5000);

        std::stringstream ss;
        ss << reading;
    
        std::cout << ss.str() << std::endl;
    
        outputFile << ss.str();
        outputFile.close();

        serial_port.Close();
    return EXIT_SUCCESS ;
}
