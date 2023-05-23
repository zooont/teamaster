#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
#include <Servo.h>

// Постоянные переменные русских букв дисплея
//byte bukva_B[8]   = {B11110,B10000,B10000,B11110,B10001,B10001,B11110,B00000,}; // Буква Б
byte bukva_v[8]   = {B00000,B00000,B11110,B10001,B11110,B10001,B11110,B00000,}; // Буква в
byte bukva_G[8]   = {B11111,B10001,B10000,B10000,B10000,B10000,B10000,B00000,}; // буква Г
//byte bukva_D[8]   = {B01111,B00101,B00101,B01001,B10001,B11111,B10001,B00000,}; // Буква Д
//byte bukva_ZH[8]  = {B10101,B10101,B10101,B11111,B10101,B10101,B10101,B00000,}; // буква Ж
byte bukva_Z[8]   = {B01110,B10001,B00001,B00010,B00001,B10001,B01110,B00000,}; // буква З
byte bukva_z[8]   = {B00000,B00000,B01110,B10001,B00010,B10001,B01110,B00000,}; // буква З
//byte bukva_I[8]   = {B10001,B10011,B10011,B10101,B11001,B11001,B10001,B00000,}; // буква И
byte bukva_i[8]   = {B00000,B00000,B10001,B10011,B10101,B11001,B10001,B00000,}; // буква и 
//byte bukva_IY[8]  = {B01110,B00000,B10001,B10011,B10101,B11001,B10001,B00000,}; // буква Й
//byte bukva_L[8]   = {B00011,B00111,B00101,B00101,B01101,B01001,B11001,B00000,}; // Буква Л
byte bukva_k[8]   = {B00000,B00000,B10001,B10010,B11100,B10010,B10001,B00000,}; // Буква к
byte bukva_P[8]   = {B11111,B10001,B10001,B10001,B10001,B10001,B10001,B00000,}; // Буква П
byte bukva_p[8]   = {B00000,B00000,B11111,B10001,B10001,B10001,B10001,B00000,}; // Буква п
byte bukva_te[8]  = {B00000,B00000,B11111,B00100,B00100,B00100,B00100,B00000,}; // Буква т
//byte bukva_Y[8]   = {B10001,B10001,B10001,B01010,B00100,B01000,B10000,B00000,}; // буква У
//byte bukva_F[8]   = {B00100,B11111,B10101,B10101,B11111,B00100,B00100,B00000,}; // буква Ф
//byte bukva_TS[8]  = {B10010,B10010,B10010,B10010,B10010,B10010,B11111,B00001,}; // буква Ц
byte bukva_ts[8]  = {B00000,B00000,B10010,B10010,B10010,B10010,B11111,B00001,}; // буква ц
//byte bukva_CH[8]  = {B10001,B10001,B10001,B01111,B00001,B00001,B00001,B00000,}; // буква Ч
//byte bukva_Sh[8]  = {B10101,B10101,B10101,B10101,B10101,B10101,B11111,B00000,}; // буква Ш
byte bukva_sh[8]  = {B00000,B00000,B10101,B10101,B10101,B10101,B11111,B00000,}; // буква Ш
//byte bukva_Shch[8]= {B10101,B10101,B10101,B10101,B10101,B10101,B11111,B00001,}; // буква Щ
//byte bukva_Mz[8]  = {B10000,B10000,B10000,B11110,B10001,B10001,B11110,B00000,}; // буква Ь
byte bukva_mz[8]  = {B00000,B00000,B10000,B10000,B11110,B10001,B11110,B00000,}; // буква ь
//byte bukva_IYI[8] = {B10001,B10001,B10001,B11001,B10101,B10101,B11001,B00000,}; // буква Ы
byte bukva_iyi[8] = {B00000,B00000,B10001,B11001,B10101,B10101,B11001,B00000,}; // буква ы
//byte bukva_Yu[8]  = {B10010,B10101,B10101,B11101,B10101,B10101,B10010,B00000,}; // буква Ю
//byte bukva_Ya[8]  = {B01111,B10001,B10001,B01111,B00101,B01001,B10001,B00000,}; // буква Я

/*
  Максимально быстрый универсальный код для обработки энкодера
  Работает на перывании (используется одно)
  Тут код построен на digitalRead, что делает его универсальным для всех плат Arduino
*/
#define ENC_A 3                   // пин энкодера
#define ENC_B 2                   // пин энкодера
#define ENC_TYPE 1                // тип энкодера, 0 или 1
volatile int encCounter;
volatile boolean state0, lastState, turnFlag;
#define BUTTON 4                  // Кнопка энкодера
byte pos;

// пищалка
int piezoPin = 12;

// Сервы
Servo teaservo;
Servo shugarservo;


LiquidCrystal_I2C lcd(0x27,16,2); // Указываем I2C адрес (наиболее распространенное значение), а также параметры экрана (в случае LCD 1602 - 2 строки по 16 символов в каждой) 

void setup()
{
  // Пины серв
  teaservo.attach(6);
  shugarservo.attach(7);
  // Загрузка
  lcd.init();                      // Инициализация дисплея  
  lcd.backlight();                 // Подключение подсветки
  lcd.setCursor(0,0);              // Установка курсора в начало первой строки
  lcd.print("_--Tea Master--_");   // Набор текста на первой строке
  lcd.setCursor(0,1);              // Установка курсора в начало второй строки
  lcd.print("    Welcome!");       // Набор текста на второй строке
  lcd.blink();
  tone(piezoPin, 2200, 1000);
  delay(3000);

  Serial.begin(9600);               // энкодер
  attachInterrupt(0, int0, CHANGE);
}

void int0() 
{ // Аппаратное прерывание для обработки энкодера
  state0 = digitalRead(ENC_A);
  if (state0 != lastState) {
#if (ENC_TYPE == 1)
    turnFlag = !turnFlag;
    if (turnFlag)
      encCounter += (digitalRead(ENC_B) != lastState) ? -1 : 1;
#else
    encCounter += (digitalRead(ENC_B) != lastState) ? -1 : 1;
#endif
    lastState = state0;
  }
}

void loop()
{
  // Основное меню
  Menu();
  while (digitalRead(BUTTON) != 0)
  { // опрос энкоденра
    Serial.println(encCounter);
    pos=encCounter&0x01;
    if ( pos == 0) 
    { // верхняя строка
      lcd.setCursor(14,0); 
      lcd.print("+");
      lcd.setCursor(14,1); 
      lcd.print("-");
    }
    else
    { // нижняя строка
      lcd.setCursor(14,0); 
      lcd.print("-");
      lcd.setCursor(14,1); 
      lcd.print("+");
    }
    delay(200);
  }
  if (pos == 0) 
  { // Заварить
    Warning1();
    delay(500);         // чтобы не было двойного нажатия
    while (digitalRead(BUTTON) != 0);
    Warning3();
    // Включение сервопривода заварки
    Maketea();
    Warning5();
  }
  else
  { // Размешать
    Warning2();
    delay(500);       // чтобы не было двойного нажатия
    while (digitalRead(BUTTON) != 0);
    Warning3();
    // Включение сервопривода размешивания
    Makeshugar();
    Warning5();
  }
  // Пищание
  
}

void Menu()
{
  lcd.noBlink();
  lcd.clear();
  lcd.createChar(5, bukva_Z);     //  загрузка 5 символа буквы З в дисплей
  lcd.createChar(1, bukva_v);     //  загружаем 1 символ буквы в на дисплей
  lcd.createChar(2, bukva_i);     //  загружаем 2 символ и на дисплей
  lcd.createChar(3, bukva_te);    //  загружаем 3 символ т на дисплей
  lcd.createChar(4, bukva_mz);    //  загружаем 4 символ ь на дисплей
  lcd.createChar(6, bukva_z);     //  Загружаем 6 символ з на дисплей
  lcd.createChar(7, bukva_sh);    //  Загружаем 7 символ ш на дисплей
  lcd.setCursor(0,0);             
  lcd.print("1.\5a\1ap\2\3\4.  [+] ");  // Пункт меню 1

  lcd.setCursor(0,1);
  lcd.print("2.Pa\6me\7a\3\4. [-]");  // Пункт меню 2
}

void Warning1() 
{
  lcd.clear();
  lcd.noBlink();
  lcd.createChar(2, bukva_i);     //  загружаем 2 символ и на дисплей
  lcd.createChar(3, bukva_te);    //  загружаем 3 символ т на дисплей
  lcd.createChar(5, bukva_Z);     //  загрузка 5 символа буквы З в дисплей
  lcd.createChar(1, bukva_k);     //  Загружаем 1 символ к на дисплей        
  lcd.createChar(4, bukva_p);     //  Загружаем 4 символ п на дисплей  
  lcd.setCursor(0,0); 
  lcd.print("\5a\1pe\4\2\3e");    // Закрепите 
  lcd.setCursor(0,1); 
  lcd.print("     \4a\1e\3\2\1."); //      пакетик.
}

void Warning2() 
{
  lcd.clear();
  lcd.noBlink();
  lcd.createChar(1, bukva_iyi);   //  Загружаем 1 символ ы на дисплей        
  lcd.createChar(3, bukva_te);    //  загружаем 3 символ т на дисплей
  lcd.createChar(4, bukva_p);     //  Загружаем 4 символ п на дисплей  
  lcd.createChar(5, bukva_Z);     //  загрузка 5 символа буквы З в дисплей
  lcd.createChar(6, bukva_mz);    //  загружаем 6 символ ь на дисплей
  lcd.setCursor(0,0); 
  lcd.print("\5ac\1\4\6\3e");     // Засыпьте
  lcd.setCursor(0,1); 
  lcd.print("       caxap.");     //      сахар.
}

void Warning3() 
{
  lcd.setCursor(13,1); 
  lcd.print("OK.");               // OK.
  delay(500);
}

void Warning4() 
{
  lcd.clear();
  lcd.noBlink();
  lcd.createChar(1, bukva_P);     //  Загружаем 1 символ П на дисплей        
  lcd.createChar(2, bukva_ts);    //  загружаем 2 символ ц на дисплей
  lcd.setCursor(0,1); 
  lcd.print("   \1po\2ecc...");  // Процесс...
}

void Warning5() 
{
  lcd.clear();
  lcd.noBlink();
  lcd.createChar(1, bukva_G);     //  Загружаем 1 символ Г на дисплей        
  lcd.setCursor(0,1); 
  lcd.print("    \1OTOBO !");     // ГОТОВО !
  tone(piezoPin, 1000, 1000); // Звук прекратится через 500 мс, о программа останавливаться не будет!
  delay(5000);
}

void Maketea()
{
  Warning4();
  for (int i = 0; i < 20; i++) 
  {
    teaservo.write(180);
    delay(1000);
    teaservo.write(0);
    delay(500);
  }
}

void Makeshugar()
{
  Warning4();
  for (int i = 0; i < 50; i++)
  {
    shugarservo.write(360);
    delay(500);
    shugarservo.write(0);
    delay(500);
  }
}
