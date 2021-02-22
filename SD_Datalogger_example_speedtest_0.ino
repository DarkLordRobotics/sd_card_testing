/*----------------------------------------------
  SD card speedtests
   - walking ones, analog reads, etc.
  SD card attached to SPI bus as follows:
  MOSI - pin 11
  MISO - pin 12
  CLK - pin 13
  CS - pin 10 (SS_PIN on Redboard)
*/

//code version 210131

#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;

int loop_count = 0;

// for ascii data
//const int buffer_depth = 512;
//char data_buffer[buffer_depth];

// for binary data 8b
//const int buffer_depth = 512;
//byte data_buffer[buffer_depth];

// for binary data 16b
const int buffer_depth = 256;  // = 518 bytes
uint16_t data_buffer[buffer_depth];

int buffer_address = 0;

unsigned long time_elapsed = 0;


//----------------------------------------------
void setup() 
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  delay(1000);
  Serial.println();
  Serial.println("SD card datalogger speedtest");
  Serial.println();

  Serial.println("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }

  // Check to see if the file exists:
  if (SD.exists("datalog.txt")) {
    Serial.println("datalog.txt exists");
    //RH erase the file from the card...
    Serial.println("removing datalog.txt...");
    SD.remove("datalog.txt");
  }
  else Serial.println("datalog.txt doesn't exist.");

  // Check to see if the file exists:
  if (SD.exists("datalog.txt")) Serial.println("datalog.txt exists");
  else Serial.println("datalog.txt doesn't exist");

  Serial.println("SD card initialized!");

  Serial.println();
  Serial.println("begin speedtest...");
  Serial.println(millis());
  //sd_write_millis();  // subtract 25 ms from serial monitor value, writes to card first

}


//----------------------------------------------
void loop() 
{

  // for binary 8 bit data, write 64 bytes each loop to buffer
  //for (byte i = 0; i < 64; i++) {
  //  data_buffer[buffer_address] = i;
  //    Serial.print("value: ");
  //    Serial.print(data_buffer[i]);
  //    Serial.print("   buffer_address: ");
  //    Serial.println(buffer_address);
  //  buffer_address++;
  //}

  // for ascii data, write 64 bytes each loop to buffer
  //for (int i = 0; i < 64; i++) {
  //  data_buffer[buffer_address] = i + 58;  // start at ASCII 58 ':'
  //    Serial.print("value: ");
  //    Serial.print(data_buffer[i]);
  //    Serial.print("   buffer_address: ");
  //    Serial.println(buffer_address);
  //  buffer_address++;
  //}

  //data_buffer[buffer_address] = (unsigned int)millis();
  //buffer_address++;
  //Serial.print("write 64 bytes each loop to buffer: ");
  //Serial.println(millis());
  //Serial.println();

  // for binary 16 bit data, write 512 bytes to buffer
  for (uint16_t i = 0; i < 256; i++) {
    data_buffer[buffer_address] = i;
      Serial.print("value: ");
      Serial.print(data_buffer[i]);
      Serial.print("   buffer_address: ");
      Serial.println(buffer_address);
    buffer_address++;
  }

/*
  // for binary data
  // read analog sensors and write to buffer
  for (int analogPin = 0; analogPin < 6; analogPin++) {
    int sensor = analogRead(analogPin);
    data_buffer[buffer_address] = sensor;
      Serial.print("sensor: ");
      Serial.print(sensor);
      Serial.print("   buffer_address: ");
      Serial.println(buffer_address);
    buffer_address++;
  }
  data_buffer[buffer_address] = (unsigned int)millis();
  buffer_address++;
  Serial.print("analog read 6x done: ");
  Serial.println(millis());
  Serial.println();
*/


  // loop counter to write buffer to card
  if (loop_count == 0) {   // for 16 bit int * 512 per loop
  //if (loop_count == 3) {   // for ascii data, 4 loops, starts at 0
  //if (loop_count == 7) {   // for ascii data, 8 loops, starts at 0
  //if (loop_count == 35) {   // for binary analog read data, 36 loops, starts at 0: 256 / (6 reads + 1 time stamp) = 36 loops
  //if (loop_count == 41) {   // 42 loops, starts at 0
  //if (loop_count == 9) {   // 10 loops, starts at 0

    Serial.print("write buffer to uSD: ");
    Serial.println(millis());
    time_elapsed = millis();

    sd_write_buffer();

    time_elapsed = millis() - time_elapsed;
    Serial.print("write complete: ");
    Serial.println(millis());
    Serial.print("time_elapsed: ");
    Serial.println(time_elapsed);
    //sd_write_millis();
    loop_count = 0;
    while(1) {
      delay(10);
    }
  }
  else {
    loop_count = loop_count + 1;  
  }

}

//----------------------------------------------

void sd_write_buffer() 
{
  // open the file. note that only one file can be open at a time
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    for (int i = 0; i < buffer_depth; i++) {
      dataFile.println(data_buffer[i]);
    }
    //dataFile.println(millis());
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else Serial.println("error opening datalog.txt");
}

void sd_write(uint16_t value) 
{
  // open the file. note that only one file can be open at a time
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(value, BIN);
    dataFile.close();
    // print to the serial port too:
    //Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else Serial.println("error opening datalog.txt");
}

void sd_write_millis() 
{
  // open the file. note that only one file can be open at a time
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(millis());
    dataFile.close();
    // print to the serial port too:
    Serial.println(millis());
  }
  // if the file isn't open, pop up an error:
  else Serial.println("error opening datalog.txt");
}


void speedtest_loop() 
{
  uint16_t data_test;
  for (int i = 0; i <= 15; i++) {
    data_test = 1 << i;
    sd_write(data_test);
    Serial.println(data_test, BIN);
  }
  for (int i = 0; i <= 13; i++) {
    data_test = data_test >> 1;
    sd_write(data_test);
    //data_test = 1 << i;
    Serial.println(data_test, BIN);
  }
  // add counter, only write millis ev 50 cycles?
  if (loop_count == 50) {
    sd_write_millis();
    loop_count = 0;
  }
  else {
    loop_count = loop_count + 1;  
  }
}


void sd_write_string() 
{
  // make a string for assembling the data to log:
  String dataString = "";

  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 6; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 5) {
      dataString += ",";
    }
  }
  Serial.println("analog read 6x done");
  Serial.println(millis());

  // open the file. note that only one file can be open at a time
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    //Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else Serial.println("error opening datalog.txt");

  Serial.println("SD write done");
  Serial.println(millis());
}
