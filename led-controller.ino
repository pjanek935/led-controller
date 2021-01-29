#define ledPin 7
#define FLAG_VAL '!'

#define R_PIN 3
#define G_PIN 6
#define B_PIN 11

#define INVALID_VAL -1

enum MainState
{
  WAIT_FOR_START_FLAG_1,
  WAIT_FOR_START_FLAG_2,
  SELECT_MODE,
  READ_R,
  READ_G,
  READ_B,
  WAIT_FOR_END_FLAG,
};

enum Mode
{
  MAIN,
  SET_RGB_VALUES,
};

int currentMode = MAIN;
int currentState = WAIT_FOR_START_FLAG_1;
int readVal = 0;
int r, g, b;

void decideMainMode ();

void setup()
{
  analogWrite (R_PIN, 255);
  analogWrite (G_PIN, 255);
  analogWrite (B_PIN, 255);
  currentState = WAIT_FOR_START_FLAG_1;
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600); // Default communication rate of the Bluetooth module
}

void tmp ()
{
  switch (currentMode)
  {
    case MAIN:

      decideMainMode ();
    
    break;

    case SET_RGB_VALUES:
    
      decideSetRGBValuesMode ();
      
    break;
  }
}

void decideSetRGBValuesMode ()
{
   switch (currentState)
  {
    case READ_R:

      r = readVal;
      currentState = READ_G;

      break;

    case READ_G:

      g = readVal;
      currentState = READ_B;

      break;

    case READ_B:

      b = readVal;
      currentState = WAIT_FOR_END_FLAG;
      currentMode = MAIN;

      break;
  }
}

void decideMainMode ()
{
  switch (currentState)
  {
    case WAIT_FOR_START_FLAG_1:

      if (readVal == (char) FLAG_VAL)
      {
        currentState = WAIT_FOR_START_FLAG_2;
      }
      else
      {
        currentState = WAIT_FOR_START_FLAG_1;
      }

      break;

    case WAIT_FOR_START_FLAG_2:

      if (readVal == (char) FLAG_VAL)
      {
        currentState = SELECT_MODE;
      }
      else
      {
        currentState = WAIT_FOR_START_FLAG_1;
      }

      break;

    case SELECT_MODE:

      //TODO
      currentState = READ_R;
      currentMode = SET_RGB_VALUES;

      break;

    case WAIT_FOR_END_FLAG:

      if (readVal == (char) FLAG_VAL)
      {
        currentState = WAIT_FOR_START_FLAG_1;
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
        currentState = WAIT_FOR_START_FLAG_1;
      }

      break;
  }
}

void decideCurrentState ()
{
  switch (currentState)
  {
    case WAIT_FOR_START_FLAG_1:

      if (readVal == (char) FLAG_VAL)
      {
        currentState = WAIT_FOR_START_FLAG_2;
      }
      else
      {
        currentState = WAIT_FOR_START_FLAG_1;
      }

      break;

    case WAIT_FOR_START_FLAG_2:

      if (readVal == (char) FLAG_VAL)
      {
        currentState = SELECT_MODE;
      }
      else
      {
        currentState = WAIT_FOR_START_FLAG_1;
      }

      break;

    case SELECT_MODE:

      //TODO
      currentState = READ_R;

      break;

    case READ_R:

      r = readVal;
      currentState = READ_G;

      break;

    case READ_G:

      g = readVal;
      currentState = READ_B;

      break;

    case READ_B:

      b = readVal;
      currentState = WAIT_FOR_END_FLAG;

      break;

    case WAIT_FOR_END_FLAG:

      if (readVal == (char) FLAG_VAL)
      {
        currentState = WAIT_FOR_START_FLAG_1;
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
        currentState = WAIT_FOR_START_FLAG_1;
      }

      break;
  }
}

void readNewValue ()
{
  if (Serial.available() > 0)
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
