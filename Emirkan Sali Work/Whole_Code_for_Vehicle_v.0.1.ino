#define s0 A0        //pins for RGB Sensor
#define s1 A1
#define s2 A2
#define s3 A3
#define out A4

#define echo 13    //pins for Ultrasonic Sensor
#define trig 12

#define motor1pin1 9   //pins for Motor driver. Enable Pins are always on
#define motor1pin2 8
#define motor2pin1 7
#define motor2pin2 6

#define sensor1 2   //pins for IR sensors
#define sensor2 4

long duration;  //variables for Ultrasonic Sensor
int distance;

int Red=0, Blue=0, Green=0;  //RGB values 
int onGreen=0, onBlue=0, onMagenta=0, onOrange=0, onBlack=0; //Values for if the vehicle is on a certain color
int vertical=0, horizontal=0; //establishing values needed for navigation on the track

int back=0, forward=0;

int process = 0; 

// Please put in the location of the object that will change
int horizonGoal = 13; // possible inputs: 0-13
int verticalGoal = 5; // possible inputs: 0-5
// Please put in the location of the object that will change

// Please put in the location of the object that is set permanently
int horizonGoalset = 13; // possible inputs: 0-13
int verticalGoalset = 5; // possible inputs: 0-5
// Please put in the location of the object that is set permanently



int state = 1 ; //start state


void setup() {
  Serial.begin(9600);   //intialize the serial monitor baud rate
  
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);

   pinMode(trig, OUTPUT);
   pinMode(echo, INPUT);
  Serial.println("Ultrasonic Sensor Distance");

   pinMode(4, INPUT);
   pinMode(2, INPUT);

   pinMode(s0,OUTPUT);    //pin modes
   pinMode(s1,OUTPUT);
   pinMode(s2,OUTPUT);
   pinMode(s3,OUTPUT);
   pinMode(out,INPUT);

   pinMode(5, OUTPUT); 
   pinMode(10, OUTPUT);
   
   digitalWrite(s0,HIGH); //Putting S0/S1 on HIGH/HIGH levels means the output frequency scalling is at 100% (recommended)
   digitalWrite(s1,LOW); //LOW/LOW is off HIGH/LOW is 20% and LOW/HIGH is  -2%
}

enum state {StopState = 0,SearchState = 1,DetectState = 2};

void loop() {
switch (state)
  {
    case StopState:

    GetColors();
    displayColors();
  
  
   if((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW) && onBlue == 2)
   {    
    state = SearchState; 
    }
    
      break;

    case SearchState:

    while(horizonGoal > 0)
    {
      Serial.println(process);
      GetColors();
      forhorizon();
      horizonGoal = --horizonGoal;
      process = 0;
    }

  //  while(verticalGoal != 0)
    //{
    // GetColors();
     //forvertical();
    // verticalGoal = --verticalGoal;
   // }


    
      break;

    case DetectState:

    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
      break;
  }
}

void GetColors()  
{    
  digitalWrite(s2, LOW);                                           //S2/S3 levels define which set of photodiodes we are using LOW/LOW is for RED LOW/HIGH is for Blue and HIGH/HIGH is for green 
  digitalWrite(s3, LOW);                                           
  Red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);       //here we wait until "out" go LOW, we start measuring the duration and stops when "out" is HIGH again, if you have trouble with this expression check the bottom of the code
  delay(20);  
  digitalWrite(s3, HIGH);                                         //Here we select the other color (set of photodiodes) and measure the other colors value using the same techinque
  Blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);  
  digitalWrite(s2, HIGH);  
  Green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);


if(Blue < 20&& Red < 20 && Green < 20)
{
  Serial.println("White");
  Serial.println("");
  onBlack = 0;
}

else if(Blue > 40 && Red > 40 && Green > 40)
{
  Serial.println("Black");
  Serial.println("");
  onBlack = 1;
}


else if(Green<=22 && Green < Blue)
{
  Serial.println("The color is Green" );
  Serial.println("");
  onGreen = ++onGreen;
  onOrange = 0;
  onBlue =0;
  onBlack = 0;
  if(onGreen > 2)
  {
    onGreen = 2;
  }
  if(onMagenta >= 1)
  {
   forward = 1;
  }
  else forward = 0;
}

else if(Green > Blue && Green > Red && Green > 48)
{
  Serial.println("The color is Magenta");
  Serial.println("");
  onMagenta = ++onMagenta;
  onOrange = 0;
  onBlack = 0;
  onBlue=0;
  if(onMagenta > 2)
  {
    onMagenta = 2;
  }
  
  if(onGreen >= 1)
  {
    back = 1;
  }
  else back = 0;
}

else if (Blue<20 && Blue< Green)
{
  Serial.println("The color is blue");
  Serial.println("");
  onBlue = ++onBlue;
  onGreen =0;
  onMagenta=0;
  onBlack = 0;
  if(onBlue > 2)
  {
    onBlue = 2;
  }
  
  if(onOrange >= 1)
  {
    back = ++back;
  }
  else back = 0;
}

else if(Red < Green && Red < Blue)
{
  Serial.println("The color is Orange");
  Serial.println("");
  onOrange=++onOrange;
  onGreen= 0;
  onBlack = 0;
  onMagenta=0;
  if(onOrange>2)
  {
    onOrange = 2;
  }
  if(onBlue >= 1)
  {
    forward = 1;
  }
  else forward = 0;
}
  delay(100);
}

      void UltrasonicSensor()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

void displayColors()
{
  Serial.println("red = ");
  Serial.println(Red);
  Serial.println("Blue = "  );
  Serial.println(Blue  );
  Serial.println("Green = ");
  Serial.println(Green  );
  Serial.println("");
}

void navigation()
{
  if(onOrange == 2 && forward == 1)
    {
      horizontal = ++horizontal;
      onBlue   = 0;
      onOrange = 0;
      process = 1;
      if(horizontal > 13)
      {
        horizontal = 13;
      }
    }

    else if(onBlue == 2 && back == 1)
    {
      horizontal = --horizontal;
      onBlue = 0;
      onOrange = 0;
      if(horizontal < 0)
      {
        horizontal = 0;
      }
    }

    if(onGreen == 2 && forward == 1)
    {
      vertical = ++vertical;
      onGreen   = 0;
      onMagenta = 0;
      if(vertical > 5)
      {
        vertical = 5;
      }
    }

    else if(onMagenta == 2 && back == 1)
    {
      vertical = --vertical;
      onGreen = 0;
      onMagenta = 0;
      if(vertical < 0)
      {
        vertical = 0;
      }
    }

    Serial.println("Position of the vehicle on the track (horizontal/vertical):");
    Serial.println("");
    Serial.print("[");
    Serial.print(horizontal);
    Serial.print("/");
    Serial.print(vertical);
    Serial.print("]");
    Serial.println("");
    Serial.println("");
    Serial.println("process=");
    Serial.println(process);
}

void forhorizon()
{
  while(process == 0)
  {
   navigation();
   GetColors();
   Serial.println("horizontal increase");
  if(onOrange == 2 || (onBlue == 2 & back == 0)) //going forward
  {
    Serial.println("horizontal forward");
    GetColors();
    navigation();
    if((digitalRead(sensor1)== HIGH) && (digitalRead(sensor2) == HIGH) || (digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW) )
    {
    analogWrite(5, 150); //ENA pin
    analogWrite(10, 140); //ENB pin

    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    }
    if((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == HIGH))
    {
    analogWrite(5, 0); //ENA pin
    analogWrite(10, 0); //ENB 
    delay(100);
    analogWrite(5, 120); //ENA pin
    analogWrite(10, 120); //ENB pin
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
    }

    if((digitalRead(sensor1)== HIGH) && (digitalRead(sensor2) == LOW))
    {
    analogWrite(5, 0); //ENA pin
    analogWrite(10, 0); //ENB 
    delay(100);
    analogWrite(5, 120); //ENA pin
    analogWrite(10, 120); //ENB pin
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    }
  }
    else if(onMagenta == 2)  // turning left
  {
    navigation();
    GetColors();
    if((digitalRead(sensor1)== HIGH) && (digitalRead(sensor2) == HIGH))
    {
    analogWrite(5, 150); //ENA pin
    analogWrite(10, 140); //ENB pin

    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    }
    
    if((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == HIGH))
    {
    analogWrite(5, 0); //ENA pin
    analogWrite(10, 0); //ENB 
    delay(100);
    analogWrite(5, 120); //ENA pin
    analogWrite(10, 120); //ENB pin
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
    }

    if((digitalRead(sensor1)== HIGH) && (digitalRead(sensor2) == LOW))
    {
    analogWrite(5, 0); //ENA pin
    analogWrite(10, 0); //ENB 
    delay(100);
    analogWrite(5, 120); //ENA pin
    analogWrite(10, 120); //ENB pin
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    }
    
    if((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW)) // turning
    { 
      analogWrite(5, 0); //ENA pin
      analogWrite(10, 0); //ENB pin
      delay(1000);
      GetColors();
      navigation();
      while(onBlue < 2 && (digitalRead(sensor1)== HIGH) && (digitalRead(sensor2) == HIGH))
      {
         GetColors();
         navigation();
         analogWrite(5, 120); //ENA pin
         analogWrite(10, 120); //ENB pin
         digitalWrite(motor1pin1, LOW);
         digitalWrite(motor1pin2, LOW);

         digitalWrite(motor2pin1, LOW);
         digitalWrite(motor2pin2, HIGH);

      }
      }
             while(onBlue == 2) // going forward until next color
       {
    GetColors();
    navigation();
    if((digitalRead(sensor1)== HIGH) && (digitalRead(sensor2) == HIGH) || (digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW) )
    {
    analogWrite(5, 150); //ENA pin
    analogWrite(10, 140); //ENB pin

    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    }
    
    if((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == HIGH))
    {
    analogWrite(5, 0); //ENA pin
    analogWrite(10, 0); //ENB 
    delay(100);
    analogWrite(5, 120); //ENA pin
    analogWrite(10, 120); //ENB pin
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
    }

    if((digitalRead(sensor1)== HIGH) && (digitalRead(sensor2) == LOW))
    {

    analogWrite(5, 0); //ENA pin
    analogWrite(10, 0); //ENB 
    delay(100);
    analogWrite(5, 120); //ENA pin
    analogWrite(10, 120); //ENB pin
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    }
       }
    }
    
    else if(onGreen == 2) //turning right
  {
    navigation();
    GetColors();
    if((digitalRead(sensor1)== HIGH) && (digitalRead(sensor2) == HIGH))
    {
    analogWrite(5, 150); //ENA pin
    analogWrite(10, 140); //ENB pin

    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    }
    
    if((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == HIGH))
    {
    analogWrite(5, 0); //ENA pin
    analogWrite(10, 0); //ENB 
    delay(100);
    analogWrite(5, 120); //ENA pin
    analogWrite(10, 120); //ENB pin
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
    }

    if((digitalRead(sensor1)== HIGH) && (digitalRead(sensor2) == LOW))
    {
    analogWrite(5, 0); //ENA pin
    analogWrite(10, 0); //ENB 
    delay(100);
    analogWrite(5, 120); //ENA pin
    analogWrite(10, 120); //ENB pin
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    }
    
    if((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW)) // turning
    { 
      GetColors();
      navigation();
      analogWrite(5, 0); //ENA pin
      analogWrite(10, 0); //ENB pin
      delay(1000);
      while(onBlue < 2 || (digitalRead(sensor1)!= HIGH) && (digitalRead(sensor2) != HIGH)) // turning
      {
         GetColors();
         navigation();
         analogWrite(5, 120); //ENA pin
         analogWrite(10, 120); //ENB pin
         digitalWrite(motor1pin1, LOW);
         digitalWrite(motor1pin2, HIGH);

         digitalWrite(motor2pin1, LOW);
         digitalWrite(motor2pin2, LOW);
      }
      }
          while (onBlue == 2)
      {
    GetColors();
    navigation();
    if((digitalRead(sensor1)== HIGH) && (digitalRead(sensor2) == HIGH) || (digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW) )
     {
    analogWrite(5, 150); //ENA pin
    analogWrite(10, 140); //ENB pin

    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
     }

    if((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == HIGH))
    {
    analogWrite(5, 0); //ENA pin
    analogWrite(10, 0); //ENB 
    delay(100);
    analogWrite(5, 120); //ENA pin
    analogWrite(10, 120); //ENB pin
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
    }

    if((digitalRead(sensor1)== HIGH) && (digitalRead(sensor2) == LOW))
    {
    analogWrite(5, 0); //ENA pin
    analogWrite(10, 0); //ENB 
    delay(100);
    analogWrite(5, 120); //ENA pin
    analogWrite(10, 120); //ENB pin
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
       }
      }
  }
  }
  
}

void backhorizon()
{
   while(onOrange < 2)
  {
  if(onMagenta == 2)
  {
    //turn to right
  }
    else if(onBlue == 2)
  {
    //go straight
  }
    else if(onGreen == 2)
  {
    //turn to left
  }
  
}
}

void forvertical()
{
    while(process == 0)
  {
  if(onBlue == 2)
  {
    //turn to right
  }
    else if(onOrange == 2)
  {

    }
  
    else if(onGreen == 2)
  {
    
    }
  }
  
}

void backvertical()
{
    while(onGreen < 2)
  {
  if(onMagenta == 2)
  {
    //go straight
    //Controlling speed (0 = off and 255 = max speed):
    analogWrite(5, 100); //ENA pin
    analogWrite(10, 200); //ENB pin

    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
  }
    else if(onOrange == 2)
  {
    //go right
  }
    else if(onBlue == 2)
  {
    //turn to left
  }
  
}
}
