//Pääfunktio
void stateActive()
{
  if(previousState!=state) 
  {
    LCD_Print("ALARM ACTIVATED");
    LCD_Print("U SHALL NOT PASS",0, 1);
    digitalWrite (redLed, HIGH);
    #ifdef ArduinoMega
    digitalWrite(grnLed, LOW);
    #endif
    DoorwayMeasurement = measureDistance();
  }
  activated();
}

//Mittaa matkan UÄ-anturin avulla
//return: matka sentteinä (cm)
int measureDistance()
{
  long duration;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  delayMicroseconds(2);
  return duration * 0.034 / 2;  // Speed of sound wave divided by 2 (go and back)
}

void activated()
{
  if (DoorwayMeasurement - 2 > measureDistance())
  {   
    timerEnabled = true; 
    LCD_Print("Enter");
    LCD_Print("PIN:", 0, 1);
    //kysytään käyttäjältä PIN koodi max 3 kertaa. Estetään peruminen ja asetukset
    bool isCancelled = true;
    bool isSettings = false;
    bool codeOK = false;
    while(!codeOK && isCancelled)
      codeOK = GetCheckPinCode(4, isCancelled, isSettings, false);    
    if(codeOK) 
    {
      LCD_Print("Code OK");
      LCD_Print("", 0, 1);      
      timerEnabled = false; 
      delay(1500);
      state = passive;
    }
    else
    {
      timerEnabled = false;
      state = alert;
    }
  }     
}
