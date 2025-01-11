void stateAlert()
{ 
  lcd.clear();
  delay(100);
  LCD_Print("ALARM!");
  digitalWrite (siren, HIGH);
  digitalWrite (redLed, HIGH);
  delay(3000);
  //Hälytyksen peruminen:
  //kysytään käyttäjältä PIN1 koodi max 3 kertaa. Estetään peruminen ja asetukset
  bool isCancelled = true;
  bool isSettings = false;
  bool codeOK = false;
   LCD_Print("PIN:", 0, 1);;  
  while(!codeOK && isCancelled)
    codeOK = GetCheckPinCode(4, isCancelled, isSettings, false);    
  if(codeOK)
  {
    LCD_Print("CLEARD", 0, 1);; 
    digitalWrite (siren, LOW);
    digitalWrite (redLed, LOW);
    delay(1500);
    state = passive;
  }
}
