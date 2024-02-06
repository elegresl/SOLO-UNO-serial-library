/**
 *  @example serial_port_write.cpp
 */

#include </usr/include/libserial/SerialPort.h>
#include <cstdlib>
#include <fstream>
#include <iostream>

constexpr const char* const SERIAL_PORT_2 = "/dev/ttyACM0" ;

/**
 * @brief This example reads the contents of a file and writes the entire 
 *        file to the serial port one character at a time. To use this
 *        example, simply utilize TestFile.txt or another file of your
 *        choosing as a command line argument.
 */
int main()
{   
    using namespace LibSerial ;
   

    // Instantiate a SerialPort object.
    SerialPort serial_port ;

    try
    {
        // Open the Serial Port at the desired hardware port.
        serial_port.Open(SERIAL_PORT_2) ;
    }
    catch (const OpenFailed&)
    {
        std::cerr << "The serial port did not open correctly." << std::endl ;
        return EXIT_FAILURE ;
    }

    // Set the baud rate of the serial port.
    serial_port.SetBaudRate(BaudRate::BAUD_115200) ;

    // Set the number of data bits.
    serial_port.SetCharacterSize(CharacterSize::CHAR_SIZE_8) ;

    // Turn off hardware flow control.
    serial_port.SetFlowControl(FlowControl::FLOW_CONTROL_DEFAULT) ;

    // Disable parity.
    serial_port.SetParity(Parity::PARITY_NONE) ;
    
    // Set the number of stop bits.
    serial_port.SetStopBits(StopBits::STOP_BITS_1) ;

    // Read characters from the input file and write them to the serial port. 
    std::cout << "Writing data to the serial port." << std::endl ;
    
   
        // Create a variable to store data from the input file and write to the
        // serial port.
        char data_byte = 0xFF;
        serial_port.WriteByte(data_byte);
        serial_port.DrainWriteBuffer();

        data_byte = 0xFF;
        serial_port.WriteByte(data_byte);
        serial_port.DrainWriteBuffer() ;

        data_byte = 0x00;
        serial_port.WriteByte(data_byte);
        serial_port.DrainWriteBuffer() ;

        data_byte = 0x05;
        serial_port.WriteByte(data_byte);
        serial_port.DrainWriteBuffer() ;

        data_byte = 0x00;
        serial_port.WriteByte(data_byte);
        serial_port.DrainWriteBuffer() ;

        data_byte = 0x00;
        serial_port.WriteByte(data_byte);
        serial_port.DrainWriteBuffer() ;

        data_byte = 0x00;
        serial_port.WriteByte(data_byte);
        serial_port.DrainWriteBuffer() ;

        data_byte = 0xAA;
        serial_port.WriteByte(data_byte);
        serial_port.DrainWriteBuffer() ;

         data_byte = 0x00;
         serial_port.WriteByte(data_byte);
        serial_port.DrainWriteBuffer() ;

         data_byte = 0xFE;
         serial_port.WriteByte(data_byte);
        serial_port.DrainWriteBuffer() ;
    
        serial_port.Close();
    
        // Print to the terminal what is being written to the serial port.
        std::cout << data_byte ;


    // Successful program completion.
    std::cout << "The example program successfully completed!" << std::endl ;
    return EXIT_SUCCESS ;
}
