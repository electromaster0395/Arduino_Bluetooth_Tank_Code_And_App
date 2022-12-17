#include <SoftwareSerial.h>

//Define motor clas so that controlling motors is a simpler proccess
class motor{
  private:
    int pin1;
    int pin2;
    int enPin;

  public:
    motor(int pin1, int pin2, int enPin)
    {
      this->pin1 = pin1;
      this->pin2 = pin2;
      this->enPin = enPin;

      init();
    }

    void init()
    {
      pinMode(pin1, OUTPUT);
      pinMode(pin2, OUTPUT);
      pinMode(enPin, OUTPUT);
      off();
    }

    void Turn(int direction, int speed)
    {
      if(direction >= 0)
      {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
      }

      else
      {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
      }

      analogWrite(enPin, speed);
    }

    void off()
    {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        digitalWrite(enPin, LOW);
    }
};
  
int ledPin = 8;

motor M1(2, 5, 3);
motor M2(6, 7, 9);

String message;

void setup() 
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}

void loop() 
{
  if(Serial.available())
  {
    while(Serial.available())
    {
      delay(3);
      message += char(Serial.read());
    }
  }

  else
  {
    message = "";
  }

  if(message != "")
  {
    int speed1 = parseSpeed1(message);
    int speed2 = parseSpeed2(message);
    String ledState = parseLedState(message);
    digitalWrite(ledPin, ledState=="true" ? 1 : 0);

    // Serial.print(speed1);
    // Serial.print(",");
    // Serial.print(speed2);
    // Serial.print(",");
    // Serial.println(ledState);
    Serial.println(message);

    M1.Turn(Sign(speed1), abs(speed1));
    M2.Turn(Sign(speed2), abs(speed2));
  }
}

int Sign(int num)
{
  return constrain(num, -1, 1);
}


int parseSpeed1(String data)
{
  data.remove(data.indexOf("Y"));
  data.remove(data.indexOf("X"), 1);
  return data.toInt();
}

int parseSpeed2(String data)
{
  data.remove(0, data.indexOf("Y") + 1);
  data.remove(data.indexOf("L"));
  return data.toInt();
}

String parseLedState(String data)
{
  data.remove(0, data.indexOf("L") + 1);
  return data;
}
