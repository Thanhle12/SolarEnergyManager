#define BLYNK_PRINT DebugSerial

#define BLYNK_TEMPLATE_ID "TMPL66LNPy377"
#define BLYNK_TEMPLATE_NAME "IoT Giám Sát Điện Mặt Trời Solar Energy"
char auth[] = "DWEre8ubs91CLfNojdltWx40xYJ5wGzv"; //Mã Token của Project trên Blynk

#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3);
#include <BlynkSimpleStream.h>

LiquidCrystal lcd(11, 10, 5, 4, 3, 2);

const int currentPin = A0;
int sensitivity = 66;
int adcValue = 0;
int offsetVoltage = 2500;
double adcVoltage = 0;
double currentValue = 0;

float Wp = 400;  // Công suất danh định của tấm pin mặt trời (đơn vị Wp)
float efficiency = 0.20;  // Hiệu suất của tấm pin, ví dụ 20% = 0.20
float solarIrradiance = 1000; // Cường độ bức xạ mặt trời hiện tại, nếu đo được
float voltage = 12.0;

void setup() {
  // put your setup code here, to run once:
  // Debug console
  DebugSerial.begin(9600);
  Serial.begin(9600);
  Blynk.begin(Serial, auth);   // Khởi tạo kết nối Blynk
  lcd.begin(16, 2);
}

void loop() {
  Blynk.run();   // Duy trì kết nối Blynk

  // put your main code here, to run repeatedly:
  adcValue = analogRead(currentPin);
  adcVoltage = (adcValue / 1024.0) * 5000;
  currentValue = ((adcVoltage - offsetVoltage) / sensitivity);
  if (currentValue < 0) {
    currentValue = 0;
  }
  int loadvoltage = currentValue * 12;
  // int power = loadvoltage * currentValue;
  float power = ((voltage * currentValue * efficiency) * (solarIrradiance / 1000));

  // Hiển thị trên LCD
  lcd.setCursor(0, 0);
  lcd.print("Current = ");
  lcd.print(currentValue, 2);  // Hiển thị 2 chữ số thập phân
  lcd.print(" A");
  
  lcd.setCursor(0, 1);
  lcd.print("Voltage = ");
  lcd.print(loadvoltage);
  lcd.print(" V");

  delay(1000);
  
  lcd.setCursor(0, 0);
  lcd.print("Power = ");
  lcd.print(power, 2);
  lcd.print(" Watt         ");
  lcd.setCursor(0, 1);
  lcd.print("                             ");
  lcd.setCursor(0, 0);
  delay(1000);

  // Gửi giá trị lên Blynk
  Blynk.virtualWrite(V1, currentValue);   // Gửi giá trị dòng điện lên chân ảo V1
  Blynk.virtualWrite(V2, loadvoltage);    // Gửi giá trị điện áp lên chân ảo V2
  Blynk.virtualWrite(V3, power);          // Gửi giá trị công suất lên chân ảo V3
}