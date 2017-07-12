#define BUT_PIN 2


int joyX = A1; //  X - axis of Joystick
int joyY = A0; // y - axis of Joystick
int x, y; 
char dir;
void setup()
{ 
  Serial.begin(9600);
  pinMode(BUT_PIN, INPUT);
}

void loop()
{ 
  x = joyX;
  y = joyY;
  x = map(analogRead(joyY), 0, 1023, 900, 2100);

  if ( x > 2000 && x < 2200){
    dir = 'D';
  }
  if ( x > 800 && x < 1000){
    dir = 'A';
  }
  //Serial.write(x);
  Serial.write(dir);
}
