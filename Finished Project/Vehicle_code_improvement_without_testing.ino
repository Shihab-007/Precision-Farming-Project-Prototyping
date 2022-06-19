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
int onGreen=0, onBlue=0, onMagenta=0, onOrange=0, onBlack=0, onWhite=0; //Values for if the vehicle is on a certain color


int vertical=1, horizontal=1; //set initial position of Vehicle

int back=0, forward=0; // variable for turning back

int turnBackminus= 0, turnBackplus = 0;

int process = 0;

int way = 1; //change to 2 when the vehicle should only go to one location

// Please put in the location of the object that will change
int horizonGoal = 4; // possible inputs: 0-13
int verticalGoal = 3; // possible inputs: 0-5
// Please put in the location of the object that will change

// Please put in the location of the object that is set permanently
int horizonGoalset = 4; // possible inputs: 0-13
int verticalGoalset = 3; // possible inputs: 0-5
// Please put in the location of the object that is set permanently

int sechoriGoalset = 1, secvertiGoalset = 5;

int sechoriGoal = 1, secvertiGoal = 5;

int facingfrontback = 0; // variable to indicate which direction the vehicle is facing.

int startposHorizon = 0;

int earlyFinishverti = verticalGoalset - secvertiGoalset; //early finish when 2nd destination is right above or below 1st destination
int earlyFinishhori = horizonGoalset - sechoriGoalset;

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

   horizonGoal = horizonGoal - horizontal;
   verticalGoal = verticalGoal - vertical;

   if(horizonGoal == 0)
   {
    horizonGoal = 1;
    startposHorizon = 1;
   }

   Serial.println("early finish =");
   Serial.print(earlyFinishverti);
}


enum state {StopState = 0,HoriState = 1, VertiState = 2, Turnleft = 3, Turnright = 4, DetectState = 5, TurnBack = 6, BackHori = 7, BackVerti = 8, Finish = 9, specialTurnBack = 10 };
void loop() {
switch (state)
{
  case StopState:

  Stop();
  Serial.println("Stop State");
  GetColors();
  displayColors();
  UltrasonicSensor();

  if (((onGreen == 2 || onMagenta == 2 || onBlue == 2 || onOrange == 2) && (digitalRead(sensor1)== HIGH) && (digitalRead(sensor2) == HIGH) && distance > 15) && horizonGoal > 0 || verticalGoal > 0)
  {
    Serial.println("starting navigation");

    state = HoriState;
    
  }
  break;

  case HoriState:
  
  Serial.println("Horizontal State");
  Serial.println("HorizonGoal = ");
  Serial.print(horizonGoal);
  GetColors();
 
  UltrasonicSensor();
  drive();
  if((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW))
  {
    if (horizonGoal > 1 )
  {
    delay(400);
  }
    horizonGoal = --horizonGoal;
    if (horizonGoal < 0)
    {
      horizonGoal == 0;
    }
  }

  Serial.println("verticalGoal = ");
  Serial.print(verticalGoal);

 
   if( (horizonGoal == 0 && verticalGoal < 0) && (digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW))
   {
    Stop();
    state = Turnright;
   }
else if( way == 2 && horizonGoal == 0 && verticalGoal == 0)
{
  drive();
  delay(300);
  Stop();
  state = StopState;
}
 else if(horizonGoal == 0 && verticalGoalset == 1)
 {
  drive();
  delay(300);
  state = DetectState;
 }
 else if(horizonGoal == 0 && (digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW))
  {
    Stop();
    state = Turnleft;
    
  }


  
  break;

  case Turnleft:

  Serial.println("turn left State");
  
  delay(1000);
  GetColors();
  driveStraight();
  delay(500);

  turnLeft();
  Stop();
  delay(1000);


   if(startposHorizon == 1 && verticalGoal == 0)
   {
   facingfrontback = 1;
   state = specialTurnBack;
   }
 else if((horizonGoal == 0 && verticalGoal == 0) && way == 2)
   {
    drive();
    delay(300);
    Stop();
    state = StopState;
   }

 else if(horizonGoal == 0 && (sechoriGoal != 0) )
  {
    state = VertiState;
  }
  else if(verticalGoal < 0)
  {
    state = BackVerti;
  }
  
  
  break;

  case VertiState:

  Serial.println("Vertical State");  
  GetColors();
  UltrasonicSensor();
  drive();
  
   if((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW) && onMagenta < 2)
  {
    if (verticalGoal > 1 )
  {
    delay(400);
  }
    verticalGoal = --verticalGoal;
    if (verticalGoal < 0)
    {
      verticalGoal == 0;
    }
  }

  Serial.println(verticalGoal);
  
  if((verticalGoal == 0 && sechoriGoal == 0) || (verticalGoal == 0 && startposHorizon == 1))
  {
    Stop();
    state = Turnleft;
  }
  else if(verticalGoal == 0)
  {
    Stop();
    state = Turnright;
  }

  

  break;

  case Turnright:

  Serial.println("turn right State");
  
  delay(1000);
  GetColors();
  driveStraight();
  delay(500);
  
  turnRight();
  Stop();
  delay(1000);

  Serial.println("verticalGoal = ");
  Serial.print(verticalGoal);
  Serial.println("secvertiGoal = ");
  Serial.print(secvertiGoal);
  Serial.println("sechoriGoal = ");
  Serial.print(sechoriGoal);

  if(((secvertiGoal == 0 && sechoriGoal == 0) || (horizonGoal == 0 && verticalGoal == 0)) && way == 2)
  {
    drive();
    delay(300);
    Stop();
    state = StopState;
  }
  else if (verticalGoal < 0 && horizonGoal == 0)
  {
    state = BackVerti;
  }
  else if (verticalGoal > 0)
  {
    state = VertiState;
  }
  else if(verticalGoal == 0)
  {
    state = DetectState;
  }
  
  break;

  case DetectState:

  Serial.println("Detect State");
  GetColors();
  UltrasonicSensor();
  Stop();

  if(sechoriGoalset <= horizonGoalset)  {
    state = TurnBack;
  }
  else state = Finish;

  break;

  case TurnBack:

  Serial.println("turn back state");

  drive();
  
  if(((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW) && (secvertiGoalset <= verticalGoalset) && (turnBackminus < 2)) && verticalGoalset != 1) // turn right side back
  {
    Stop();
    delay(1000);
    GetColors();
    driveStraight();
    delay(500);
  
    turnRight();
    Stop();
    delay(1000);

    turnBackminus = ++turnBackminus;
    Serial.println("turn back state");
  }
  
  if(((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW) && (secvertiGoalset >= verticalGoalset) && turnBackplus < 2) && verticalGoalset != 5)  // turn left side back
  {
    Stop();
  delay(1000);
  GetColors();
  driveStraight();
  delay(500);

  turnLeft();
  Stop();
  delay(1000);
  turnBackplus = ++turnBackplus;
  Serial.println("turn back state");
  }

  if(turnBackminus == 2)
  {
    if(facingfrontback == 0)
    {
      facingfrontback = 1;
    }
    else if(facingfrontback == 1)
    {
      facingfrontback = 0;
    }
    vertical = verticalGoalset;
    vertical = --vertical;
    horizontal = horizonGoalset;
    verticalGoalset= --verticalGoalset;
    state = Finish;
  }

  if(turnBackplus == 2)
  {
    if(facingfrontback == 0)
    {
      facingfrontback = 1;
    }
    else if(facingfrontback == 1)
    {
      facingfrontback = 0;
    }
    vertical = verticalGoalset;
    vertical = ++vertical;
    horizontal = horizonGoalset;
    verticalGoalset= ++verticalGoalset;
    state = Finish;
  }
  
  break;

  case Finish:

  Serial.println("Finish State");
  
  Stop();

  Serial.println(earlyFinishverti);
  Serial.println(earlyFinishhori);
  
  while(earlyFinishverti == 1 && earlyFinishhori == 0 )
  {
    Serial.println("Earlyfinish");
    Stop();
    state = StopState;
  }
  
  while(earlyFinishverti == -1 && earlyFinishhori == 0)
  {
    Serial.println("Earlyfinish");
    Stop();
    state = StopState;
  }

  
  horizontal = horizonGoalset;
  vertical = verticalGoalset;

  sechoriGoal = sechoriGoal-horizonGoalset;
  secvertiGoal = secvertiGoal-verticalGoalset;

  horizonGoal = sechoriGoal;
  verticalGoal = secvertiGoal;
  
  way = ++way;
  
  if(way > 2)
  {
    way = 2;
  }

  if(startposHorizon == 1)
  {
    horizonGoal = --horizonGoal;
    startposHorizon = 0;
  }

  Serial.println(sechoriGoal);
  Serial.println("horizongoal");
  Serial.print(horizonGoal);
  if(sechoriGoal < 0)
  {
    state = BackHori;
  }
  else
  {
    state = HoriState;
    horizonGoal = ++horizonGoal;
    facingfrontback = 1;
  }

  break;

  case BackHori:
  
  Serial.println("Back Horizontal State");
  GetColors();
 
  UltrasonicSensor();
  drive();
  if((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW))
  {

    if (horizonGoal < -1  )
  {
    delay(500);
  }
    horizonGoal = ++horizonGoal;
    Serial.println("minus 1 horizontal");
  }
  
  Serial.println(horizonGoal);
  
  if(horizonGoal >= 0)
  {
    Stop();
    sechoriGoal = 0;
    if(secvertiGoalset > verticalGoalset)
    {
    state = Turnright;
    }
    else if(secvertiGoalset < verticalGoalset)
    {
      state = Turnleft;
    }
    else
    {
      state = StopState;
    }
  }
  
  break;

  case BackVerti:

  Serial.println("Back Vertical State");  
  Serial.println("verticalGoal="); 
  Serial.print(verticalGoal);
   
  GetColors();
  UltrasonicSensor();
  drive();
  
   if((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW) && onMagenta < 2)
  {
    if (verticalGoal < -1 )
  {
    delay(400);
  }
    verticalGoal = ++verticalGoal;
    if (verticalGoal > 0)
    {
      verticalGoal == 0;
    }
    Serial.println("minus 1 verticalGoal");
    Serial.println(verticalGoal);  
  }

  if(verticalGoal == 0)
  {
    Stop();
    secvertiGoal = 0;
    sechoriGoal = 0;
    state = Turnright;
  }

  break;

  case specialTurnBack: // only if the starting horizontal position is the same as the destination of the horizontal coordinate

  
  Serial.println("special turn back state");

  drive();
  
  if((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW) && (secvertiGoalset <= verticalGoalset) && (turnBackminus < 2)) // turn right side back
  {
    Stop();
    delay(1000);
    GetColors();
    driveStraight();
    delay(500);
  
    turnLeft();
    Stop();
    delay(1000);

    turnBackminus = ++turnBackminus;
    Serial.println("turn back state");
  }
  
  if((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW) && (secvertiGoalset > verticalGoalset) && turnBackplus < 2)  // turn left side back
  {
    Stop();
  delay(1000);
  GetColors();
  driveStraight();
  delay(500);

  turnRight();
  Stop();
  delay(1000);
  turnBackplus = ++turnBackplus;
  Serial.println("turn back state");
  }

  if(turnBackminus == 2)
  {
    facingfrontback = 0;
    vertical = verticalGoalset;
    vertical = --vertical;
    horizontal = horizonGoalset;
    verticalGoalset= --verticalGoalset;
    state = Finish;
  }

  if(turnBackplus == 2)
  {
    facingfrontback = 0;
    vertical = verticalGoalset;
    vertical = ++vertical;
    horizontal = horizonGoalset;
    verticalGoalset= ++verticalGoalset;
    state = Finish;
  }

   break;

}
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
  onWhite = 1;
  onBlack = 0;
  onGreen = 0;
  onMagenta = 0;
  onOrange = 0;
  onBlue = 0;
}

else if(Blue > 40 && Red > 40 && Green > 40)
{
  Serial.println("Black");
  Serial.println("");
  onBlack = 1;
  onGreen = 0;
  onMagenta = 0;
  onOrange = 0;
  onBlue = 0;
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
   onMagenta = 0;
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
    onGreen = 0;
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
    onOrange = 0;
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
    onBlue = 0;
  }
  else forward = 0;
}
}

void drive()
{
if((digitalRead(sensor1)== HIGH) && (digitalRead(sensor2) == HIGH) || (digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == LOW) )
    {
    analogWrite(5, 120); //ENA pin
    analogWrite(10, 110); //ENB pin

    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    }
    if((digitalRead(sensor1)== LOW) && (digitalRead(sensor2) == HIGH))
    {
    analogWrite(5, 0); //ENA pin
    analogWrite(10, 0); //ENB 
    analogWrite(5, 100); //ENA pin
    analogWrite(10, 100); //ENB pin
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
    }

    if((digitalRead(sensor1)== HIGH) && (digitalRead(sensor2) == LOW))
    {
    analogWrite(5, 0); //ENA pin
    analogWrite(10, 0); //ENB 
    analogWrite(5, 100); //ENA pin
    analogWrite(10, 100); //ENB pin
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    }
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
}

void Stop()
{
    analogWrite(5, 0); //ENA pin
    analogWrite(10, 0); //ENB pin
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
  
}

void turnRight()
{
         analogWrite(5, 150); //ENA pin
         analogWrite(10, 145); //ENB pin
         digitalWrite(motor1pin1, LOW);
         digitalWrite(motor1pin2, LOW);

         digitalWrite(motor2pin1, LOW);
         digitalWrite(motor2pin2, HIGH);
         delay(1420);
}

void turnLeft()
{
         analogWrite(5, 150); //ENA pin
         analogWrite(10, 145); //ENB pin
         digitalWrite(motor1pin1, LOW);
         digitalWrite(motor1pin2, HIGH);

         digitalWrite(motor2pin1, LOW);
         digitalWrite(motor2pin2, LOW);
         delay(1580);
         
}

void driveStraight()
{
    analogWrite(5, 120); //ENA pin
    analogWrite(10, 110); //ENB pin

    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
}
