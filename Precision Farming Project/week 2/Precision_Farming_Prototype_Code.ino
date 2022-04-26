#define Ena1an2 10
#define Ena3an4 5
#define echoPin 13
#define trigPin 12

long duration;
int distance;
int state = 0 ;

bool pressed = false;
bool onLine = false;
bool offLine = false;

void setup() {
  
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  Serial.println("Ultrasonic Sensor Distance");

  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), LineOn, FALLING);
  attachInterrupt(digitalPinToInterrupt(2), LineOff, FALLING);

  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
}

enum state {StopState = 0,SearchState = 1,DetectState = 2};

void loop() {
  switch (state)
  {
    case StopState:
    
    digitalWrite(Ena1an2, HIGH);
    digitalWrite(Ena1an2, HIGH);
    analogWrite(9, 0);
    analogWrite(8 , 0);
    analogWrite(7, 0);
    analogWrite(6, 0);

    onLine = false;
    
      break;

    case SearchState:
    
    digitalWrite(Ena1an2, HIGH);
    digitalWrite(Ena3an4, HIGH);
    analogWrite(9, 200);
    analogWrite(8 , 0);
    analogWrite(7, 0);
    analogWrite(6, 200);

    UltrasonicSensor();

    offLine = false;

    if(distance < 15)
    {
      Serial.println("object detected 15cm in front of vehicle");
      state = DetectState;
    }
      break;

    case DetectState:

    digitalWrite(Ena1an2, HIGH);
    digitalWrite(Ena1an2, HIGH);
    analogWrite(9, 0);
    analogWrite(8 , 0);
    analogWrite(7, 0);
    analogWrite(6, 0);

    UltrasonicSensor();

    if(distance > 15)
    {
      Serial.println("Object removed");
      state = StopState;
    }
      break;
  }


}

  
      bool ButtonChange()
      {
       pressed = false;
       if ((state == SearchState) && offLine)
       {
          Serial.println("Line has been left");
          state = StopState;
          pressed = true;
       }

       if ((state == StopState) && onLine)
       {
        state = SearchState;
        pressed = true;
       }
        return pressed;
      }


      void LineOn() 
      {
        onLine = true;
        Serial.println("onLine");
        ButtonChange();
      }

      void LineOff() 
      {
        offLine = true;
        Serial.println("offLine");
        ButtonChange();
      }

      void UltrasonicSensor()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}
