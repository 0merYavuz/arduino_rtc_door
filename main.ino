#include <Servo.h>
#include <DS1302.h>
DS1302_RAM ramBuffer;
DS1302 rtc(8,7,6); //RST, DATA, CLK pin numbers
uint8_t door_process = 0;
uint8_t lock_process = 50;
int locked_pos= 50;
int unlocked_pos= 105;
int door_opened_pos = 110;
int door_closed_pos = 10;

Time t;



Servo myservo, lockservo;  
int pos;

void setup() {
  /* Serial.begin(9600); */

 /*  rtc.setTime(20, 35, 00); */

  ramBuffer=rtc.readBuffer();
    door_process = ramBuffer.cell[0];
    lock_process = ramBuffer.cell[1];
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  lockservo.attach(10);  // attaches the servo on pin 9 to the servo object
  
  
  lockservo.write(lock_process);
  myservo.write(door_process);
  
}

void loop() {
   t=rtc.getTime();
/*   Serial.println(rtc.getTimeStr());
  Serial.println(door_process);
  Serial.println(lock_process); */
  delay(1000);
 
 /*  open_the_door();
  delay(2000);
    close_the_door();
  delay(2000); */



  //Door Openning
  if(t.hour==8 && t.min==00 && t.sec == 00){
    open_the_door();
  }

  //door closing
  if(t.hour==20 && t.min==00 && t.sec == 00){
    close_the_door();
  }
}


void open_the_door(){
  lockservo.write(unlocked_pos);
   delay(100);
  for (pos = door_closed_pos; pos <= door_opened_pos; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);

     ramBuffer.cell[0]=pos;
     ramBuffer.cell[1]=unlocked_pos;
     rtc.writeBuffer(ramBuffer);

    if(pos==door_opened_pos){
      delay(30);
      lockservo.write(unlocked_pos);
    }                        // waits 15 ms for the servo to reach the position
  }
}

void close_the_door(){
  lockservo.write(unlocked_pos);
  for (pos = door_opened_pos; pos >= door_closed_pos; pos -= 1) { 
    myservo.write(pos);             
    delay(30);

    ramBuffer.cell[0]=pos;
     ramBuffer.cell[1]=locked_pos;
     rtc.writeBuffer(ramBuffer);

    if(pos==door_closed_pos){
      delay(3000);
      lockservo.write(locked_pos);
    }
  }
}