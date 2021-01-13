#define ledPin 7
#define FLAG_VAL '!'

#define R_PIN 3
#define G_PIN 6
#define B_PIN 9

#define INVALID_VAL -1

enum MainState
{
  WAIT_FOR_START_FLAG_1,
  WAIT_FOR_START_FLAG_2,
  READ_R,
  READ_G,
  READ_B,
  WAIT_FOR_END_FLAG,
};

int state = WAIT_FOR_START_FLAG_1;
int readVal = 0;
int r, g, b;

void setup() 
{
  analogWrite (B_PIN, 255);
  state = WAIT_FOR_START_FLAG_1;
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600); // Default communication rate of the Bluetooth module
}

void decideCurrentState ()
{
  //Serial.println (state);
  switch (state)
  {
    case WAIT_FOR_START_FLAG_1:
      if (readVal == (char) FLAG_VAL)
      {
        state = WAIT_FOR_START_FLAG_2;
      }
      else
      {
        state = WAIT_FOR_START_FLAG_1;
      }
    break;

    case WAIT_FOR_START_FLAG_2:
      if (readVal == (char) FLAG_VAL)
      {
        state = READ_R;
      }
      else
      {
        state = WAIT_FOR_START_FLAG_1;
      }
    break;

    case READ_R:
      r = readVal;
      state = READ_G;
    break;

    case READ_G:
      g = readVal;
      state = READ_B;
    break;

    case READ_B:
      b = readVal;
      state = WAIT_FOR_END_FLAG;
    break;

    case WAIT_FOR_END_FLAG:
      if (readVal == (char) FLAG_VAL)
      {
        state = WAIT_FOR_START_FLAG_1;
        Serial.println ("SAVE RGB");
        Serial.print ("R: "); Serial.println (r);
        Serial.print ("G: "); Serial.println (g);
        Serial.print ("B: "); Serial.println (b);
        analogWrite (R_PIN, r);
        analogWrite (G_PIN, g);
        analogWrite (B_PIN, b);
      }
      else
      {
        state = WAIT_FOR_START_FLAG_1;
      }
    break;
  }
}

void readNewValue ()
{
  if(Serial.available() > 0)
  { // Checks whether data is comming from the serial port
    readVal = Serial.read(); // Reads the data from the serial port
  }
  else
  {
    readVal = INVALID_VAL;
  }
}

void loop() 
{
  readNewValue ();
 
  if (readVal != INVALID_VAL)
  {
    decideCurrentState ();
    readVal = INVALID_VAL;
  }
}
