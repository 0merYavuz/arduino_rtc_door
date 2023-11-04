#include <Servo.h>
#include <DS1302.h>
DS1302_RAM ramBuffer;

DS1302 rtc(8,7,6); //RST, DATA, CLK pin numbers
uint8_t saved_door_position = 0; //Registered servo angle to restore the position of servo motors in case of power loss
uint8_t saved_lock_position = 50; //Registered servo angle to restore the position of servo motors in case of power loss
uint8_t transaction_status = 0; // 0: No unfinished action, 1: Unfinished openning, 2: Unfinished closing

//lock positions:
int locked_pos= 50;
int unlocked_pos= 105;

//door positions
int door_opened_pos = 110;
int door_closed_pos = 10;

Time t;



Servo doorservo, lockservo;  
int pos;

void setup() {
  Serial.begin(9600);

  rtc.setTime(00, 10, 00);

  ramBuffer=rtc.readBuffer();
  saved_door_position = ramBuffer.cell[0];
  saved_lock_position = ramBuffer.cell[1];
  transaction_status = ramBuffer.cell[2];

  doorservo.attach(9);  // attaches the servo on pin 9 to the servo object
  lockservo.attach(10);  // attaches the servo on pin 9 to the servo object
  
  
  lockservo.write(saved_lock_position);
  doorservo.write(saved_door_position);

/*   pos = saved_door_position; */

  if (transaction_status == 1){// 0: No unfinished action, 1: Unfinished openning, 2: Unfinished closing
    open_the_door(saved_door_position);
  }

  if (transaction_status == 2){// 0: No unfinished action, 1: Unfinished openning, 2: Unfinished closing
    close_the_door(saved_door_position);
  }



  


}

void loop() {
   t=rtc.getTime();
  Serial.println(rtc.getTimeStr());
  Serial.println(saved_door_position);
  Serial.println(saved_lock_position);
  delay(1000);
 
/*  open_the_door(door_closed_pos);
  delay(3000);
    close_the_door(door_opened_pos);
  delay(3000);
 */


  //Door Openning
  if(t.hour==00 && t.min==01 && t.sec == 00){
    open_the_door(door_closed_pos);
  }

  //door closing
  if(t.hour==00 && t.min==00 && t.sec == 00){
    close_the_door(door_opened_pos);
  }
}


void open_the_door(int from_angle){
  transaction_status=1; // 0: No unfinished action, 1: Unfinished openning, 2: Unfinished closing
  ramBuffer.cell[2]=transaction_status;
  rtc.writeBuffer(ramBuffer);

  lockservo.write(unlocked_pos);
   delay(1000);
  for (pos = from_angle; pos <= door_opened_pos; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    doorservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);

     ramBuffer.cell[0]=pos;
     ramBuffer.cell[1]=unlocked_pos;
     rtc.writeBuffer(ramBuffer);

    if(pos==door_opened_pos){
      delay(30);
      lockservo.write(unlocked_pos);
    }                        // waits 15 ms for the servo to reach the position
  }

  transaction_status=0; // 0: No unfinished action, 1: Unfinished openning, 2: Unfinished closing
  ramBuffer.cell[2]=transaction_status;
  rtc.writeBuffer(ramBuffer);
}

void close_the_door(int from_angle){
  transaction_status=2; // 0: No unfinished action, 1: Unfinished openning, 2: Unfinished closing
  ramBuffer.cell[2]=transaction_status;
  rtc.writeBuffer(ramBuffer);

  lockservo.write(unlocked_pos);
  for (pos = from_angle; pos >= door_closed_pos; pos -= 1) { 
    doorservo.write(pos);             
    delay(30);

    ramBuffer.cell[0]=pos;
     ramBuffer.cell[1]=locked_pos;
     rtc.writeBuffer(ramBuffer);

    if(pos==door_closed_pos){
      delay(3000);
      lockservo.write(locked_pos);
    }
  }
  transaction_status=0; // 0: No unfinished action, 1: Unfinished openning, 2: Unfinished closing
  ramBuffer.cell[2]=transaction_status;
  rtc.writeBuffer(ramBuffer);
}