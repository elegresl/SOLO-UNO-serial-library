#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>
#include <chrono>
#include <thread>

#include <cstdlib>
#include <fstream>
#include <iostream>

#include <unistd.h>

using namespace LibSerial;

int main()
{
  using namespace std::this_thread; // sleep_for, sleep_until
  using namespace std::chrono;
  
   SerialStream serial_stream;

  try
    {
        // Open the Serial Port at the desired hardware port.
        serial_stream.Open("/dev/ttyACM0") ;
    }
    catch (const OpenFailed&)
    {
        std::cerr << "The serial port did not open correctly." << std::endl ;
        return EXIT_FAILURE ;
    }
   serial_stream.SetBaudRate( BaudRate::BAUD_115200 );
     // Set the number of data bits.
    serial_stream.SetCharacterSize(CharacterSize::CHAR_SIZE_8) ;

    // Turn off hardware flow control.
    serial_stream.SetFlowControl(FlowControl::FLOW_CONTROL_NONE) ;

    // Disable parity.
    serial_stream.SetParity(Parity::PARITY_NONE) ;

    // Set the number of stop bits.
    serial_stream.SetStopBits(StopBits::STOP_BITS_1) ;
   

  
   serial_stream.write(0xFF,1);
    sleep(1);
    serial_stream.DrainWriteBuffer();

    serial_stream.write(0xFF,1);
    sleep(1);
    serial_stream.DrainWriteBuffer();

    serial_stream.write(0x00,1);
    sleep(1);
    serial_stream.DrainWriteBuffer();

    serial_stream.write(0x15,1);
    sleep(1);
    serial_stream.DrainWriteBuffer();
  
    serial_stream.write(0x00,1);
    sleep(1);
    serial_stream.DrainWriteBuffer();

  serial_stream.write(0x00,1);
    sleep(1);
    serial_stream.DrainWriteBuffer();

  serial_stream.write(0x00,1);
    sleep(1);
    serial_stream.DrainWriteBuffer();

  serial_stream.write(0x02,1);
    sleep(1);
    serial_stream.DrainWriteBuffer();

  serial_stream.write(0x00,1);
    sleep(1);
    serial_stream.DrainWriteBuffer();

  serial_stream.write(0xFF,1);
    sleep(1);
    serial_stream.DrainWriteBuffer();

   serial_stream.Close();
}
