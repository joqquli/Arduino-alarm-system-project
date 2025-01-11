/*
  Hälytysjärjestelmä V0.7 '2021
  Tietotekniikan sovellusprojekti, OAMK
  TVT21KMO, Ryhmä 5
  Andersson Casandra
  Herranen Johanna
  Kortesoja Aliisa
  Turpeinen Mikko

  versio  pvm         tekijä  muutokset
  0.7     22.04.2021  R5      Siivottu koodia. Asetettu Animationpassive animation funktioon omien merkkien luonti uudestaan
  0.6     21.04.2021  R5      Passive, Active ja Alert koodattu lähes valmiiksi. Animationpassive lisätty ja on vielä kehityksen alla. Muutettu checkUserAccess funktiota hieman
  0.5     18.04.2021  MTu     Settings asetuksen tallentamista vaille valmis
  0.4     17.04.2021  MTu     Uudelleen kirjoitettu settings rakenne ja muuttuja on nyt taulukko. Eriytetty GetCheckPinCode- ja GetUserEntry funktiot asetuksia varten
  0.3     13.04.2021  MTu     Lisätty settings rakenne ja muuttuja. Lisätty kolme funktiota main:n ja testikoodia passive ja settings osioon
  0.2     10.04.2021  MTu     Poistettu testikoodit. Lisätty tilakone rakenne ja muuttuja. Lisätty pääsilmukka ja jaettu kukin tila omiin tiedostoihinsa
  0.1     05.04.2021  MTu     Ensimmäinen testiversio, jossa kytkennän ja komponenttien testifunktioita

  - Valmiit kirjastot  
  LiquidCrystal Library; Built-In by Arduino, Adafruit Version 1.0.7
  This library allows an Arduino board to control LiquidCrystal displays (LCDs) based on the Hitachi HD44780 (or a compatible) chipset, 
  which is found on most text-based LCDs. The library works with in either 4- or 8-bit mode (i.e. using 4 or 8 data lines in addition 
  to the rs, enable, and, optionally, the rw control lines).

  Keypad by Mark Stanley, Alexander Brevix Version 3.1.1
  Keypad Library for Arduino

  Authors:  Mark Stanley, Alexander Brevig
  Contact: mstanley@technologist.com
  Contact: alexanderbrevig@gmail.com
*/
//Arduino Uno: kommentoi alla oleva pois
//#define ArduinoMega

//- Kirjastot
#include <LiquidCrystal.h>
#include <Key.h>
#include <Keypad.h>
#include <EEPROM.h>

//- LCD:n alustus
const int rs = A0, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;   //määritellään pinnit
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//omat merkit
byte LCDChar_Home[] = {B00000, B00100, B01110, B11111, B11111, B11111, B11011, B11011}; 
byte LCDChar_Edit[] = {B00000, B00000, B00000, B00011, B00111, B01110, B01100, B10000};
byte LCDChar_Cancel[] = {B00000, B00000, B10001, B01010, B00100, B01010, B10001, B00000}; 
byte LCDChar_Check[] = {B00000, B00001, B00011, B10110, B11100, B01000, B00000, B00000}; 
byte LCDChar_Password[] = {B00000, B00100, B10101,  B01110, B11111, B01110, B10101, B00100}; 
byte LCDChar_ArrowR[] = {B10000, B01000, B00100, B00010, B00010, B00100, B01000, B10000};
enum eCustomChar{cHome, cEdit, cCancel, cCheck, cPassword, cArrowR};

//- Keypad:n alustus
const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[rows] = {4, 9, 8, 6}; //connect to the row pinouts of the keypad
byte colPins[cols] = {5, 3, 7}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

//- Ulkoinen keskeytys pinni
#ifdef ArduinoMega
#define intPin 2
#endif

//- UÄ-sensorin pinnit
#define echoPin 12
#define trigPin 13
//summeri ja led pinnit. Huom! Unossa pinni 11 ei toimi oikein digitaalilähtönä, joten käytössä pinni 2
#ifdef ArduinoMega
#define siren 11
#else
#define siren 2
#endif
#define redLed 10
#ifdef ArduinoMega
#define grnLed A6
#endif

//- Prototyypit
void statePassive();
void stateSettings();
void stateActive();
void stateAlert();
void editValue(int);
String IntToStr(int,int);
bool checkUserAccess(bool = false);
void saveSettingsToEEPROM();
void getSettingsFromEEPROM();
int measureDistance();
bool GetCheckPinCode(int,bool&,bool = false);
long GetUserEntry(int,int,bool,bool&,bool&);
void LCD_Print(String text, int = 0, int = 0, bool = true);
void LCD_WriteCChar(int,int = 0,int = 0);
void animation();
void runningManL();
void runningManR();

//- Globaalit muuttujat
//timer
volatile int timerTics = 0;
volatile bool timerEnabled = false;
volatile bool timerFlag = false;
//Oven mitta
int DoorwayMeasurement = 0;

//tilakone
enum state_machine
{
    passive,
    settings,
    active,
    alert
};
typedef enum state_machine state_machine;
volatile state_machine state = passive;
volatile state_machine previousState = active;
//asetukset
typedef struct
{
  int value;
  int minValue; 
  int maxValue;
  int length;
  String sName;
  String entryText;
}sSettings;
enum eSID{sDummy, sPIN1, sPIN2, sACTDELAY, sDACTDELAY};
sSettings setting[] = {{0,0,0,0,"",""},{1234,0,9999,4,"PIN 1","Enter PIN 1:"},{0,0,9999,4,"PIN 2","Enter PIN 2:"},{15,5,90,2,"ACTIV. TIME","Exit delay:"},{20,5,90,2,"DEACT. TIME","Incoming delay:"}};

//- Arduinon Timer 1 keskeytys 1s välein, kun enabloitu
ISR(TIMER1_COMPA_vect)
{
  if(timerEnabled)
  {
    //kasvatetaan laskuria joka sekunti
    timerTics++;
    //debug
    //LCD_Print(IntToStr(timerTics,2));
    //jos tila on passiivinen muuta tila poistumisviiveen jälkeen aktiiviseksi
    if(state == passive && timerTics >= setting[sACTDELAY].value)
    {
      timerTics = 0;
      timerFlag = true;
      timerEnabled = false;
      previousState = passive;
      state = active;
    }
    //jos tila on aktiivinen muuta tila poistumisviiveen jälkeen hälytykseksi
    if(state == active && timerTics >= setting[sDACTDELAY].value)
    {
      timerTics = 0;
      timerFlag = true;
      timerEnabled = false;
      previousState = active;
      state = alert;
    }
  }
}

//- Arduinon ulkoinen keskeytys, kun hälytyspiiri on kytketty
void EXTINT()
{
  if(state == active)
  {
      timerTics = 0;
      timerFlag = true;
      timerEnabled = false;
      previousState = active;
      state = alert;    
  }
}

//- Arduinon setup-funktio
void setup() 
{
  //asetetaan uä-sensori
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  //asetetaan ledit ja summeri
  pinMode(siren, OUTPUT);
  pinMode(redLed, OUTPUT);
  digitalWrite(siren, LOW);
  digitalWrite(redLed, LOW);
  #ifdef ArduinoMega
  pinMode(grnLed, OUTPUT);
  digitalWrite(grnLed, LOW);
  #endif
  //alustetaan näyttö
  lcd.begin(16, 2);
  delay(100);
  lcd.clear();
  lcd.createChar(cHome, LCDChar_Home);
  lcd.createChar(cEdit, LCDChar_Edit);
  lcd.createChar(cCancel, LCDChar_Cancel);
  lcd.createChar(cCheck, LCDChar_Check);
  lcd.createChar(cPassword, LCDChar_Password);
  lcd.createChar(cArrowR, LCDChar_ArrowR);
  //asetetaan aloitustila
  state = passive;
  //haetaan asetukset EEPROMISTA
  getSettingsFromEEPROM();
  //alustetaaan timer1
  alustaTimer();
  //asetetaan ulkoisen hälytyspiirin keskeytys
  #ifdef ArduinoMega
    pinMode(intPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(intPin), EXTINT, FALLING);
  #endif
}

//- Arduinon pääsilmukka
void loop()
{
  //Kutsutaan tilan mukaista funktiota
  switch(state)
  {
    case(passive):
      statePassive();
      previousState=passive;
      break;
    case(settings):
      stateSettings();
      previousState=settings;
      break;
    case(active):
      stateActive();
      previousState=active;
      break;
    case(alert):
      stateAlert();
      previousState=alert;
      break;   
  }
} 

//Alusta timer luomaan keskeytys sekuntin välein
void alustaTimer(void)
{
  timerEnabled = false;
  noInterrupts();
  //alustetaan ensin kontrollirekisterit nollaksi
  TCCR1A = 0; 
  TCCR1B = 0; 
  //asetetaan timer CTC-moodiin ja jakajaksi 1024
  TCCR1B |= (1 << WGM12); 
  TCCR1B |= (1 << CS10); 
  TCCR1B |= (0 << CS11); 
  TCCR1B |= (1 << CS12);
  //asetetaan vertailurekisteriin 15625, jolloin sekunti on kulunut
  OCR1A = 15625;        
  //enabloidaan Timer1 output compare keskeytys
  TIMSK1 |= (1 << OCIE1A);
  //pakko sallia heti, koska keypad käyttää keskeytyksiä
  interrupts();
}

//Kysytään käyttäjältä PIN-koodi ja tarkistetaan onko se oikein (asetettu koodi PIN1 tai PIN2)
//Maksimimäärä on kolme yritystä. Virhetulostus tehdään riville 1
//col = LCD-näytön sarake, josta alkaen koodia aletaan näyttämään (rivi on aina alempi)
//isCancelled = true, jos käyttäjä painaa * (operaatio peruttu)
//isSettings = true, jos käyttäjä painaa * & # (settings)
//disablePIN2 = true, jos halutaan estää PIN2:n hyväksyminen
//return: true jos koodi syötetty oikein, false jos väärin tai operaatio on peruttu (tällöin isCancelled=true)
bool GetCheckPinCode(int col, bool &isCancelled, bool &isSettings, bool disablePIN2)
{
  //kysytään käyttäjältä PIN1 koodi max 3 kertaa
  bool codeOK = false;
  bool cancelled = false;

  for(int n = 1; n <= 3; n++)
  {
    long result = GetUserEntry(col, setting[sPIN1].length, false, isCancelled, isSettings);
    //keskeytys poistuminen
    if(timerFlag) { timerEnabled = false; timerFlag = false; break; }
    //peruutus    
    if(isCancelled || result < 0) break;
    //testataan onko koodi ok
    codeOK = (setting[sPIN1].value == result) || (!disablePIN2 && (setting[sPIN2].value == result));
    if(codeOK)
      break;
    else
    {
      LCD_Print("Wrong code!", col, 1, false);
      delay(1500);
      LCD_Print("           ", col, 1, false);         
    }
  }
  return codeOK;    
}

//Kysytään käyttäjältä numeerinen koodi
//col = LCD-näytön sarake, josta koodin syöttäminen alkaa
//len = koodin pituus
//showEntry = true, jos halutaan näyttää syötetty koodi. Muuten vain password char
//isCancelled = true, jos käyttäjä painaa * (operaatio peruttu)
//isSettings = true, jos käyttäjä painaa * & # (settings)
//return: -1 jos koodia ei ole syötetty tai operaatio on peruttu (tällöin isCancelled=true). Muuten syötetty koodi
long GetUserEntry(int col, int len, bool showEntry, bool &isCancelled, bool &isSettings)
{
  //alustukset
  isCancelled = false;
  isSettings = false;
  long enteredValue = -1;
  int digit = 0;
  bool done = false, entered = false;
  char key;
  char keyCombo;
  //aseta kursori näkyville ja haluttuun sijaintiin
  lcd.setCursor(col,1);
  lcd.cursor();
  //luetaan näppäimistöltä silmukassa
  while(!done)
  {
    key = -1;
    keypad.getKeys();
    if(keypad.isPressed('*') && keypad.isPressed('#'))
    {
      //* & #: Asetetaan isSettings ja isCancelled liput ja poistutaan silmukasta (funktio palauttaa -1)
      isCancelled = true;
      isSettings = true;
      done = true;
    }
    if(keypad.isPressed('*'))
    {
      //*: Asetetaan isCancelled lippu ja poistutaan silmukasta (funktio palauttaa -1)
      isCancelled = true;
      done = true;
    }
    //#: Jos vaadittu määrä merkkejä on syötetty poistutaan silmukasta
    if(keypad.isPressed('#') && entered) done = true;
    //0-9: Tallennetaan merkki
    if(keypad.isPressed('0')) key = 0;
    if(keypad.isPressed('1')) key = 1;
    if(keypad.isPressed('2')) key = 2;
    if(keypad.isPressed('3')) key = 3;
    if(keypad.isPressed('4')) key = 4;
    if(keypad.isPressed('5')) key = 5;
    if(keypad.isPressed('6')) key = 6;
    if(keypad.isPressed('7')) key = 7;
    if(keypad.isPressed('8')) key = 8;
    if(keypad.isPressed('9')) key = 9;
    if(key >= 0)
    {
      if(showEntry) lcd.write(key + 48); else lcd.write(byte(cPassword));  //näytetään merkki tai password char
      if(digit == 0) enteredValue = 0;                                //ensimmäisen merkin kohdalla nollataan
      enteredValue = enteredValue * 10 + key;                         //Tallennetaan syötetty merkki
      digit++;                                                        //kasvatetaan merkkiosoitinta
      if(digit > len - 1) {digit = 0; entered = true; }               //jos viimeinen merkki niin palataan takaisin ensimmäiseen merkkiin
      lcd.setCursor(col+digit,1);                                     //asetetaan kursori seuraavaksi syötettävän merkin kohdalle      
    }
    //toistoviive   
    delay(50);
    //keskeytys poistuminen
    if(timerFlag) { timerEnabled = false; timerFlag = false; done = true; }
  }
  //piilota kursori ja poistu
  lcd.noCursor();
  return enteredValue;
}

//Kirjoitetaan näytölle teksti haluttuun kohtaan
//text = kirjoitettava teksti
//col = sarake
//row = rivi
//clearRow = true, jos halutaan tyhjentää koko rivi ensin
void LCD_Print(String text, int col, int row, bool clearRow)
{
  if(clearRow)
  {
    lcd.setCursor(0,row);
    lcd.print("                ");
  }
  lcd.setCursor(col,row);
  lcd.print(text);
} 

//Kirjoitetaan näytölle yksi itse luotu merkki
//cchar = Oma merkki (cHome,cCancel,cCheck,cArrowL,cArrowR)
//col = sarake (0-15)
//row = rivi (0-1)
void LCD_WriteCChar(enum eCustomChar cchar, int col, int row)
{
  lcd.setCursor(col,row);
  lcd.write(byte(cchar));
}

//Muutetaan int arvo string muotoon.
//value = muutettava int arvo
//len = merkkijonon pituus
//return: merkkijono
String IntToStr(int value, int len)
{
  String result = String(value);
  for(int n = result.length(); n < len; n++)
    result = '0' + result;  //lisätään mahdolliset etunollat, jotta pituus täyttyy
  return result;
}
