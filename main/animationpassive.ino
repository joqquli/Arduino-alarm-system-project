//hälytysjärjestelmään poistumisviiveeseen animoitu juokseva ukko - Johanna Herranen

// animaation nopeus 
int x = 200;

// kuvapikseleiden teko

//oikealle juokseva ukkeli:
//vaihe 1 / 3:
  byte stepB1[8] = {B00011,B00011,B00011,B00011,B00001,B00000,B00000,B00000};
  byte stepB2[8] = {B00000,B00001,B00001,B00011,B00011,B00011,B00000,B00001};
  byte stepB3[8] = {B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11110};
  byte stepB4[8] = {B11111,B11111,B11101,B11101,B11111,B11111,B11111,B11111};
  byte stepB5[8] = {B00000,B00000,B10000,B10000,B10000,B00000,B10000,B00000};
  byte stepB6[8] = {B00000,B11000,B00000,B11000,B11100,B11000,B10000,B00000};

//vaihe 2 / 3
  byte stepC1[8] = {B11111,B11111,B11101,B00011,B00111,B01111,B01111,B00111};
  byte stepC2[8] = {B00000,B00001,B00001,B00011,B00011,B00011,B00000,B00111};
  byte stepC3[8] = {B11111,B11111,B11111,B11111,B11111,B10011,B00000,B00000};
  byte stepC4[8] = {B11111,B11111,B11101,B11101,B11111,B11111,B11111,B11111};
  byte stepC5[8] = {B11110,B11110,B01100,B11100,B11100,B11100,B00000,B00000};
  byte stepC6[8] = {B00000,B11000,B00000,B11000,B11100,B11000,B10000,B00000};
  
//vaihe 3 / 3
  byte stepD1[8] = {B00001,B00011,B00111,B00111,B00111,B01111,B01100,B00000};
  byte stepD2[8] = {B00000,B00000,B00001,B00001,B00011,B00011,B00011,B00000};
  byte stepD3[8] = {B11111,B11111,B11111,B11111,B11111,B11111,B11110,B11111};
  byte stepD4[8] = {B00000,B11111,B11111,B11101,B11101,B11111,B11111,B11111};
  byte stepD5[8] = {B10000,B11000,B10000,B00000,B00000,B00000,B00000,B00000};
  byte stepD6[8] = {B00000,B00000,B11000,B00000,B11000,B11100,B11000,B10000};


//vasemmalle juokseva ukkeli
//vaihe 1 / 3
  byte stepE1[8] = {B00000,B00001,B00001,B00001,B00000,B00000,B00001,B00000};
  byte stepE2[8] = {B00000,B00011,B00000,B00011,B00111,B00011,B00001,B00000};
  byte stepE3[8] = {B11111,B11111,B11111,B11111,B11111,B11111,B11111,B01111};
  byte stepE4[8] = {B11111,B11111,B10111,B10111,B11111,B11111,B11111,B11111};
  byte stepE5[8] = {B11000,B11000,B11000,B11000,B10000,B00000,B00000,B00000};
  byte stepE6[8] = {B00000,B10000,B10000,B11000,B11000,B11000,B10000,B10000};
  
//vaihe 2 / 3
  byte stepF1[8] = {B01111,B01111,B01100,B00111,B00111,B00111,B00000,B00000};
  byte stepF2[8] = {B00000,B00011,B00000,B00011,B00111,B00011,B00001,B00000};
  byte stepF3[8] = {B11111,B11111,B11111,B11111,B11111,B11001,B00000,B00000};
  byte stepF4[8] = {B11111,B11111,B10111,B10111,B11111,B11111,B11111,B11111};
  byte stepF5[8] = {B11111,B11111,B10111,B11000,B11100,B11110,B11110,B11100};
  byte stepF6[8] = {B00000,B10000,B10000,B11000,B11000,B11000,B00000,B11100};
  
//vaihe 3 / 3
  byte stepG1[8] = {B00001,B00011,B00001,B00000,B00000,B00000,B00000,B00000};
  byte stepG2[8] = {B00000,B00000,B00011,B00000,B00011,B00111,B00011,B00001};
  byte stepG3[8] = {B11111,B11111,B11111,B11111,B11111,B11111,B01111,B11111};
  byte stepG4[8] = {B00000,B11111,B11111,B10111,B10111,B11111,B11111,B11111};
  byte stepG5[8] = {B10000,B11000,B11100,B11100,B11100,B11110,B00110,B00000};
  byte stepG6[8] = {B00000,B00000,B10000,B10000,B11000,B11000,B11000,B00000};
  
// kuvapikseleiden loppu


//liikkuva kuva looppiin
void animation() 
{
  while(true)
  {
    // animaation aloitus
    // oikealle juokseminen (R = right)
    runningManR();

    // lopetetaan kun tila vaihtuu
    if(timerFlag) { timerEnabled = false; timerFlag = false; break; }
          
    // vasemmalle juokseminen (L = left)
    runningManL();

    // lopetetaan kun tila vaihtuu
    if(timerFlag) { timerEnabled = false; timerFlag = false; break; }
  }
  //Palautetaan omat merkit
  lcd.clear();
  lcd.createChar(cHome, LCDChar_Home);
  lcd.createChar(cEdit, LCDChar_Edit);
  lcd.createChar(cCancel, LCDChar_Cancel);
  lcd.createChar(cCheck, LCDChar_Check);
  lcd.createChar(cPassword, LCDChar_Password);
  lcd.createChar(cArrowR, LCDChar_ArrowR);
}

// pixelikuvien paikat animaation osissa

// ukkeli juoksemassa oikealle
void runningManR() {
  for (int a=0;a<11;a+=4){
  int b = a + 1;
  int c = a + 2;
  int d = a + 3;
  int e = a + 4;
  int f = a + 5;
  int g = a + 6;
  lcd.clear();
  lcd.createChar(1, stepB1);
  lcd.createChar(2, stepB2);
  lcd.createChar(3, stepB3);
  lcd.createChar(4, stepB4);
  lcd.createChar(5, stepB5);
  lcd.createChar(6, stepB6);
  lcd.setCursor(b,1);
  lcd.write(1);
  lcd.setCursor(b,0);
  lcd.write(2);
  lcd.setCursor(c,1);
  lcd.write(3);
  lcd.setCursor(c,0);
  lcd.write(4);
  lcd.setCursor(d,1);
  lcd.write(5);
  lcd.setCursor(d,0);
  lcd.write(6);
  delay(x);
  lcd.clear();
  lcd.createChar(1, stepC1);
  lcd.createChar(2, stepC2);
  lcd.createChar(3, stepC3);
  lcd.createChar(4, stepC4);
  lcd.createChar(5, stepC5);
  lcd.createChar(6, stepC6);
  lcd.setCursor(c,1);
  lcd.write(1);
  lcd.setCursor(c,0);
  lcd.write(2);
  lcd.setCursor(d,1);
  lcd.write(3);
  lcd.setCursor(d,0);
  lcd.write(4);
  lcd.setCursor(e,1);
  lcd.write(5);
  lcd.setCursor(e,0);
  lcd.write(6);
  delay(x);
  lcd.clear();
  lcd.createChar(1, stepB1);
  lcd.createChar(2, stepB2);
  lcd.createChar(3, stepB3);
  lcd.createChar(4, stepB4);
  lcd.createChar(5, stepB5);
  lcd.createChar(6, stepB6);
  lcd.setCursor(d,1);
  lcd.write(1);
  lcd.setCursor(d,0);
  lcd.write(2);
  lcd.setCursor(e,1);
  lcd.write(3);
  lcd.setCursor(e,0);
  lcd.write(4);
  lcd.setCursor(f,1);
  lcd.write(5);
  lcd.setCursor(f,0);
  lcd.write(6);
  delay(x);
  lcd.clear();
  lcd.createChar(1, stepD1);
  lcd.createChar(2, stepD2);
  lcd.createChar(3, stepD3);
  lcd.createChar(4, stepD4);
  lcd.createChar(5, stepD5);
  lcd.createChar(6, stepD6);
  lcd.setCursor(e,1);
  lcd.write(1);
  lcd.setCursor(e,0);
  lcd.write(2);
  lcd.setCursor(f,1);
  lcd.write(3);
  lcd.setCursor(f,0);
  lcd.write(4);
  lcd.setCursor(g,1);
  lcd.write(5);
  lcd.setCursor(g,0);
  lcd.write(6);
  delay(x);
  }
}


// ukkeli juoksemassa vasemmalle
void runningManL(){
  for (int a=16;a>4;a-=4){
  int b = a - 1;
  int c = a - 2;
  int d = a - 3;
  int e = a - 4;
  int f = a - 5;
  int g = a - 6;
  lcd.clear();
  lcd.createChar(1, stepE1);
  lcd.createChar(2, stepE2);
  lcd.createChar(3, stepE3);
  lcd.createChar(4, stepE4);
  lcd.createChar(5, stepE5);
  lcd.createChar(6, stepE6);
  lcd.setCursor(d,1);
  lcd.write(1);
  lcd.setCursor(d,0);
  lcd.write(2);
  lcd.setCursor(c,1);
  lcd.write(3);
  lcd.setCursor(c,0);
  lcd.write(4);
  lcd.setCursor(b,1);
  lcd.write(5);
  lcd.setCursor(b,0);
  lcd.write(6);
  delay(x);
  lcd.clear();
  lcd.createChar(1, stepF1);
  lcd.createChar(2, stepF2);
  lcd.createChar(3, stepF3);
  lcd.createChar(4, stepF4);
  lcd.createChar(5, stepF5);
  lcd.createChar(6, stepF6);
  lcd.setCursor(e,1);
  lcd.write(1);
  lcd.setCursor(e,0);
  lcd.write(2);
  lcd.setCursor(d,1);
  lcd.write(3);
  lcd.setCursor(d,0);
  lcd.write(4);
  lcd.setCursor(c,1);
  lcd.write(5);
  lcd.setCursor(c,0);
  lcd.write(6);
  delay(x);
  lcd.clear();
  lcd.createChar(1, stepE1);
  lcd.createChar(2, stepE2);
  lcd.createChar(3, stepE3);
  lcd.createChar(4, stepE4);
  lcd.createChar(5, stepE5);
  lcd.createChar(6, stepE6);
  lcd.setCursor(f,1);
  lcd.write(1);
  lcd.setCursor(f,0);
  lcd.write(2);
  lcd.setCursor(e,1);
  lcd.write(3);
  lcd.setCursor(e,0);
  lcd.write(4);
  lcd.setCursor(d,1);
  lcd.write(5);
  lcd.setCursor(d,0);
  lcd.write(6);
  delay(x);
  lcd.clear();
  lcd.createChar(1, stepG1);
  lcd.createChar(2, stepG2);
  lcd.createChar(3, stepG3);
  lcd.createChar(4, stepG4);
  lcd.createChar(5, stepG5);
  lcd.createChar(6, stepG6);
  lcd.setCursor(g,1);
  lcd.write(1);
  lcd.setCursor(g,0);
  lcd.write(2);
  lcd.setCursor(f,1);
  lcd.write(3);
  lcd.setCursor(f,0);
  lcd.write(4);
  lcd.setCursor(e,1);
  lcd.write(5);
  lcd.setCursor(e,0);
  lcd.write(6);
  delay(x);
  }
}
