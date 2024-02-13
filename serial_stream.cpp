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
  //using namespace std::unistd;
   SerialStream serial_stream;

   
   serial_stream.Open( "/dev/ttyUSB1" );

   serial_stream.SetBaudRate( BaudRate::BAUD_115200 );

   

   
   char read_byte_2 = 'B';
  
   serial_stream << 0xFF;
  unistd::sleep(1);
  serial_stream << 0xFF;
 unistd::sleep(1);

  serial_stream << 0x00;
  sleep(1);
  serial_stream << 0x15;
  sleep(1);

  serial_stream << 0x00;
  sleep(1);
  serial_stream << 0x00;
  sleep(1);;

  serial_stream << 0x00;
  sleep(1);
  serial_stream << 0x02;
  sleep(1);

  serial_stream << 0x00;
  sleep(1);;
  serial_stream << 0xFF;
  sleep(1);

   // Read a character.

   serial_stream >> read_byte_2;

   
   std::cout << "serial_stream read: " << read_byte_2 << std::endl;

   serial_stream.Close();
}
