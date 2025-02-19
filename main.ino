#include <Wire.h>
#include <LiquidCrystal_I2C.h>
int nutbam = 2;
int nutreset = 3;
unsigned long start_time;
unsigned long elapsed_time;
unsigned long lastPressTime = 0;
bool onoff = false; // Biến onoff đổi sang kiểu bool
int trangthai_truoc = HIGH; // Lưu trạng thái trước đó của nút bấm
int sensor  =4;
int relay  =5;
char command;
const unsigned long interval = 20; // Khoảng thời gian chống rung
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  elapsed_time = 0;
  Serial.begin(9600);
  pinMode(nutbam, INPUT_PULLUP);
  pinMode(nutreset, INPUT_PULLUP);
  pinMode(sensor, INPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  lcd.init();
  lcd.backlight();
  lcd.print("MADE BY 10A1-PC2");
  delay(1000);
  lcd.clear();
  lcd.print("NGUYEN HUU HOA");
  lcd.setCursor(0, 1);
  lcd.print("NGUYEN VAN NHIEM");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("S=0.6m");
  lcd.setCursor(7, 0);
  lcd.print("t= ");
  lcd.setCursor(9, 0);
  lcd.print("0.000");
}

void cailai() {
  elapsed_time = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("S=0.6m");
  lcd.setCursor(7, 0);
  lcd.print("t= ");
  lcd.setCursor(9, 0);
  lcd.print("0.000");
}

void loop() {
  unsigned long hientai = millis();
  int trangthai_hientai = digitalRead(nutbam);
  int reset = digitalRead(nutreset);
  int trangthai_sensor = digitalRead(sensor);
  if (Serial.available() > 0){
    command = Serial.read();
    if (command == '0'){
      onoff =true;
    }
    if (command == '1'){
      onoff = false;
    }

  }

  // Kiểm tra chống rung
  if (trangthai_hientai == LOW && trangthai_truoc == HIGH && (hientai - lastPressTime >= interval)) {
    lastPressTime = hientai;
    onoff = !onoff; // Đổi trạng thái onoff
    if (onoff==true) {
      start_time = millis();
    }
  
  }
  trangthai_truoc = trangthai_hientai; // Cập nhật trạng thái trước đó
  if (trangthai_sensor == LOW && onoff == true){
    onoff = false;
    Serial.println("0");
    lcd.setCursor(0,1);
    lcd.print("a=");
    float seconds = elapsed_time/1000.0;
    float giatoc = 1.2/(seconds*seconds);
    lcd.print(giatoc, 3);
    lcd.print("m/s^2");
  }
  if (reset == LOW && onoff == false){
    cailai();

  }
  if (onoff==true) {
    digitalWrite(relay, LOW);
    lcd.setCursor(9, 0);
    unsigned long current_time = millis();
    elapsed_time = current_time - start_time;
    float seconds = elapsed_time / 1000.0;
    lcd.print(seconds, 3);
    lcd.print("s");
    delay(10);
  }
  if (onoff == false){digitalWrite(relay,HIGH);}
}
