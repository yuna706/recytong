#include <usbhid.h>
#include <usbhub.h>
#include <hiduniversal.h>
#include <hidboot.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h> //lcd 라이브러리
#include <Servo.h> //Servo 라이브러리를 추가


LiquidCrystal_I2C lcd (0x3F,16,2); //lcd 주소
Servo servo;      //Servo 클래스로 servo객체 생성

int value = 180;    // 각도를 조절할 변수 value
int servoPin = 6; // 아두이노에서 꽂는 곳
char bar_code; //바코드 마지막 번호 기억
int point = 0; //누적 포인트 
int newpoint = 0; //새 포인트
int read_end = 0;
unsigned long s_time;
unsigned long now_time;
unsigned long delayy;

class MyParser : public HIDReportParser {
  public:
    MyParser();
    void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
  protected:
    uint8_t KeyToAscii(bool upper, uint8_t mod, uint8_t key);
    virtual void OnKeyScanned(bool upper, uint8_t mod, uint8_t key);
    virtual void OnScanFinished();
};

MyParser::MyParser() {}

void MyParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
  // If error or empty, return
  if (buf[2] == 1 || buf[2] == 0) return;

  for (uint8_t i = 7; i >= 2; i--) {
    // If empty, skip
    if (buf[i] == 0) continue;

    // If enter signal emitted, scan finished
    if (buf[i] == UHS_HID_BOOT_KEY_ENTER) {
      OnScanFinished();
    }

    // If not, continue normally
    else {
      // If bit position not in 2, it's uppercase words
      OnKeyScanned(i > 2, buf, buf[i]);
    }

    return;
  }
}

uint8_t MyParser::KeyToAscii(bool upper, uint8_t mod, uint8_t key) {
  // Letters
  if (VALUE_WITHIN(key, 0x04, 0x1d)) {
    if (upper) return (key - 4 + 'A');
    else return (key - 4 + 'a');
  }

  // Numbers
  else if (VALUE_WITHIN(key, 0x1e, 0x27)) {
    return ((key == UHS_HID_BOOT_KEY_ZERO) ? '0' : key - 0x1e + '1');
  }

  return 0;
}

void MyParser::OnKeyScanned(bool upper, uint8_t mod, uint8_t key) {
  uint8_t ascii = KeyToAscii(upper, mod, key);
  bar_code = ascii;
//  Serial.print((char)ascii);
}

void MyParser::OnScanFinished() {
  Serial.println(" - Finished");
  read_end = 1;
}

USB          Usb;
USBHub       Hub(&Usb);
HIDUniversal Hid(&Usb);
MyParser     Parser;

void setup() {
  Serial.begin( 115200 );
  Serial.println("Start");

  if (Usb.Init() == -1) {
    Serial.println("OSC did not start.");
  }

  delay( 200 );

  Hid.SetReportParser(0, &Parser);
  
  lcd.begin();
  lcd.backlight();
  delay( 200 );

  digitalWrite(2, HIGH);
  digitalWrite(2, LOW);
  lcd.home();

  servo.attach(servoPin);   //맴버함수인 attach : 핀 설정

  lcd.setCursor(5,0);
  lcd.print("point");
  servo.write(value);
  lcd.setCursor(5,1);
  lcd.print(point);
}

void loop() {
  Usb.Task();
  
  if(read_end == 1) {
    Serial.println(bar_code);
    read_end = 0;

    if(bar_code=='0'){
    //증가
      point+=100;
      newpoint+=100;
    }
    if(bar_code=='1'){
    //증가
      point+=150;
      newpoint+=100;
    }
    if(bar_code=='2'){
    //증가
      point+=200;
      newpoint+=200;
    }
    if(bar_code=='3'){
    //증가
      point+=250;
      newpoint+=250;
    }
    if(bar_code=='4'){
    //증가
      point+=300;
      newpoint+=300;
    }
    if(bar_code=='5'){
    //증가
      point+=350;
      newpoint+=350;
    }
    if(bar_code=='6'){
    //증가
      point+=400;
      newpoint+=400;
    }
    if(bar_code=='7'){
    //증가
      point+=450;
      newpoint+=450;
    }
    if(bar_code=='8'){
    //증가
      point+=500;
      newpoint+=500;
    }
    if(bar_code=='9'){
    //증가
      point+=550;
      newpoint+=550;
    }

    //서브
      value -= 110;       //각도를 n도 증가시킨다.
      servo.write(value); //value값의 각도로 회전. ex) value가 90이라면 90도 회전
      s_time = millis();
      
    //새 포인트
      lcd.setCursor(0,1);
      lcd.print("new point");
      lcd.setCursor(10,1);
      lcd.print(newpoint);
      newpoint=0;
    
  }
  now_time = millis();
  if(s_time !=0){
    if ( now_time > s_time+3000 && now_time < s_time+9000) {
      lcd.print("             ");
      lcd.setCursor(0,1);
      lcd.print("             ");
      lcd.setCursor(5,1);
      lcd.print(point);
    }
    now_time = millis();
    if (now_time > s_time+9000) {
      servo.write(180); //0으로 돌아옴
      value=180;
    }
  }
}
