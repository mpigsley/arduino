/*
 * CSCE236 Embedded Systems Spring
 * HW4
 */

//These are the functions that we know exist in obfs.cpp
void startOne();
void startTwo();
void startThree();
void startFour();
void setMem(char i);
void runLoop();


void setup() {                
  Serial.begin(9600);
  Serial.println("Starting up...");
  
  //This order does not work to start it up
  startOne();
  startTwo();
  startThree();
  startFour();

  //Write _YOUR_ initials to memory and determine where they are put
  setMem('c');
  setMem('d');

 //The serial port gets messed up, so this won't print
 Serial.println("Finished setup");

}

void loop() {
  //Figure out how long this function takes to run with
  //and without the button pressed.
  runLoop();
}


