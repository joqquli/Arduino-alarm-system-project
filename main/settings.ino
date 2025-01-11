//Pääfunktio
void stateSettings()
{
  size_t ssize = sizeof setting / sizeof setting[0];  //asetusten kokonaismäärä
  bool cancelled = false;
  int sInd = 0;                                       
  //--Ajetaan vain kerran tilan alkaessa
  if(previousState!=state) 
  {
    //kirjoitetaan näytölle PIN koodin kysely
    LCD_Print("- SETTINGS -", 2);
    LCD_Print("PIN:", 2, 1);
    LCD_WriteCChar(cHome, 0, 1);   
    //suoritetaan kysely ja poistutaan jos ei ole ok (tila on asetettu checkUserAccess funktiossa)
    if(!checkUserAccess(true)) return; 
  }
  
  //--Ajetaan joka kierroksella
  while(!cancelled)
  {
    //viimeisen itemin jälkeen palataan takaisin ensimmäiseen
    if(sInd >= ssize) sInd = 0;
    //tyhjennetään rivi ja näytetään asetuksen teksti (huom! saa olla korkeintaan 12 merkkiä pitkä!)
    LCD_Print(setting[sInd].sName, 2, 1);
    //näytetään vasemmanpuoleinen ikoni (perusnäytössä kotimökki, muuten kynä)
    if(sInd == sDummy) 
      LCD_WriteCChar(cHome, 0, 1);
    else LCD_WriteCChar(cEdit, 0, 1);
    //näytetään oikeanpuoleinen ikoni (selausnuoli)
    LCD_WriteCChar(cArrowR, 15, 1);
    //luetaan näppäin 
    char key = keypad.waitForKey();
    //painettu #: kierretään valikkoa ympäri
    if(key == '#') sInd++;
    //painettu *: perusnäytössä palataan passivi-tilaan, muuten siirrytään editoimaan
    if(key == '*')
    {
      if(sInd == sDummy) 
      {
        saveSettingsToEEPROM();   //tallennetaan asetukset EEPROM muistiin
        state = passive;
        cancelled = true;
      }
      else editValue(sInd);
    }
  }
}

//Hoidetaan asetuksen muuttaminen.
//sInd = muutettavan asetuksen id
void editValue(int sInd)
{
  bool cancelled = false;
  bool showValue = true;
  bool temp;
  //näytetään teksti ja ikonit (rasti ja check) 
  LCD_Print(setting[sInd].entryText);
  LCD_Print(">", 3, 1);
  LCD_WriteCChar(cCancel, 0, 1);
  LCD_WriteCChar(cCheck, 15, 1);
  //erityset asetuskohdat: PIN koodeille näytetään vain tähdet syötettäessä. Muille näytetään entinen arvo alle
  if(sInd == sPIN1 || sInd == sPIN2)
    showValue = false;
  else
    LCD_Print(IntToStr(setting[sInd].value, setting[sInd].length), 4, 1, false);

  //haetaan arvo käyttäjältä
  int value = GetUserEntry(4, setting[sInd].length, showValue, cancelled, temp);

  //tarkistetaan onko annettu arvo rajoissa
  if(!cancelled && (value < setting[sInd].minValue || value >  setting[sInd].maxValue))
  {
    LCD_Print("OUT OF RANGE!", 0, 1);
    cancelled = true;     
  }
  
  //pin koodi täytyy kysyä kahdesti (varmistaa)
  if(!cancelled && (sInd == sPIN1 || sInd == sPIN2))
  {
    LCD_Print("Re-" + setting[sInd].entryText);
    LCD_Print("    ", 4, 1, false);
    int value2 = GetUserEntry(4, setting[sInd].length, showValue, cancelled, temp);
    if(value != value2)
    {
      LCD_Print("NOT MATCHED!", 0, 1);   
      cancelled = true;
    }
  }    
  
  //ok: asetetaan annettu arvo uudeksi asetukseksi
  if(!cancelled && value >=0) 
  {
    LCD_Print("CHANGED OK!", 0, 1);
    setting[sInd].value = value;
  }

  //palauta settings teksti lopuksi
  delay(1500);
  LCD_Print("- SETTINGS -", 2);
}



//Tarkistetaan PIN1 koodin avulla onko käyttäjällä oikeutta. 
//koodin saa syöttää max 3x väärin, tämän jälkeen tila vaihdetaan alert-tilaksi
//peruutus saa tilan vaihtumaan passive-tilaksi
//disablePIN2 = true, jos halutaan estää PIN2:n hyväksyminen
//return: true, jos on oikeus. Muuten false
bool checkUserAccess(bool disablePIN2)
{
    bool isCancelled = false;
    bool isSettings = false;
    bool codeOK = GetCheckPinCode(6, isCancelled, isSettings, true);
    
    if(codeOK)
    {
      //koodi oikein. Näytetään se ja pyyhitään toinen rivi
      codeOK = true;
      LCD_Print("ACCESS GRANTED!", 0, 1);
      delay(1500);
      LCD_Print("", 0, 1);
    }
    else if(!isCancelled)    
    {
      //koodi syötetty kolme kertaa väärin: Asetetaan hälytystila ja poistutaan
      LCD_Print("ACCESS DENIED!");
      LCD_Print("ALERT!",0, 1);
      delay(1000);
      state = alert;
    }
    //peruttu: Asetetaan passivinen tila
    else state = passive;    
    
    return codeOK;  
}

//Tallennetaan asetukset EEPROM muistiin
void saveSettingsToEEPROM()
{
  //asetetaan osoitin ja kirjoitetaan tunnistemerkki (jotta ensimmäinen lukeminen ei saa väärää sisältöä)
  int ptrEEP = 0;
  int check1 = 99;
  EEPROM.put(ptrEEP,check1);
  ptrEEP += sizeof(int);
  //lasketaan asetusten määrä ja kirjoitetaan arvot silmukassa
  int ssize = sizeof(setting) / sizeof(setting[0]); 
  for(int ptrSET = 0; ptrSET < ssize; ptrSET++)
  {
    EEPROM.put(ptrEEP,setting[ptrSET].value);
    ptrEEP += sizeof(int);
  }
}

//Haetaan asetukset EEPROM muistista
void getSettingsFromEEPROM()
{
  //asetetaan osoitin ja luetaan tunnistemerkki
  int ptrEEP = 0;
  int temp;
  EEPROM.get(ptrEEP, temp);
  ptrEEP += sizeof(int);
  //luetaan arvot vain, jos tunnistemerkki löytyy
  if(temp == 99)
  {
    //lasketaan asetusten määrä ja luetaan arvot silmukassa
    int ssize = sizeof(setting) / sizeof(setting[0]); 
    for(int ptrSET = 0; ptrSET < ssize; ptrSET++)
    {
      EEPROM.get(ptrEEP, temp);
      //tallennetaan vain jos arvo kelpaa
      if(temp >= setting[ptrSET].minValue && temp <= setting[ptrSET].maxValue) setting[ptrSET].value = temp;
      ptrEEP += sizeof(int);
    }
  }
}
