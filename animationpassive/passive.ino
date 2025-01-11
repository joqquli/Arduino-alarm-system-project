//Pääfunktio
void statePassive()
{
  LCD_Print(" - SET ALARM -");
  LCD_Print("PIN:",0, 1);
  digitalWrite(redLed, LOW);
  #ifdef ArduinoMega
  digitalWrite(grnLed, HIGH);
  #endif
  
  //kysytään käyttäjältä PIN koodi max 3 kertaa
  bool isCancelled = false;
  bool isSettings = false;
  bool codeOK = GetCheckPinCode(4, isCancelled, isSettings, false);
  //siirrytään asetuksiin
  if(isSettings) { previousState = passive; state = settings; return; }
  if(isCancelled) return;
  
  if (codeOK)
  {
    LCD_Print("EXIT NOW");
    LCD_Print("GOODBYE", 0, 1);
    timerEnabled = true;
    delay(1500);
    // siirtyy animationpassive.ino 
    animation();
    state = active;
  }
  else
  {
    state = alert;
  }
}
