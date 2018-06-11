 #define PIN_DATA_OUT A0      //GP2센서 아날로그 A0출력의 IO포트를 연결합니다.
 #define PIN_LED_VCC 2                 
 const int DELAY_BEFORE_SAMPLING = 280; 
 const int DELAY_AFTER_SAMPLING = 40;   
 const int DELAY_LED_OFF = 9680;                // 인터벌
 int rgb_red = 11;     // rgb 핀 빨간색 핀
 int rgb_green = 10;  // rgb핀 녹색 핀
 int rgb_blue = 9;    // rgb핀 파란색 핀

void setup() {
  Serial.begin(115200);
  pinMode(PIN_DATA_OUT, INPUT); //Defined as input (ADC reads 1analog)
  pinMode(PIN_LED_VCC, OUTPUT); //Defined as output
  
  pinMode(rgb_red, OUTPUT);     // 3색 LED 모듈 출력으로 설정, 붉은색
  pinMode(rgb_green, OUTPUT);   // 녹색
  pinMode(rgb_blue, OUTPUT);    // 파란색
}

void loop() {
  double outputV = getOutputV();                // 출력 전압
  double ugm3 = getDustDensity(outputV);       // 먼지 농도 계산
  double aqi = getAQI(ugm3);                    // 미세먼지 농도
  String gradeInfo = getGradeInfo(aqi);           // 단계를 표현
  // 시리얼 포트 출력
  Serial.println(String("outputV=") + outputV + "\tug/m3=" + ugm3 + "\tAQI=" + aqi + "\tgradeInfo=" + gradeInfo);
  // 딜레이
  delay(1000);
}

double getOutputV() {
  digitalWrite(PIN_LED_VCC, LOW);
  delayMicroseconds(DELAY_BEFORE_SAMPLING);
  double analogOutput = analogRead(PIN_DATA_OUT);
  delayMicroseconds(DELAY_AFTER_SAMPLING);
  digitalWrite(PIN_LED_VCC, HIGH);
  delayMicroseconds(DELAY_LED_OFF);
  double outputV = analogOutput / 1024 * 5;
  return outputV;
}

/**
   출력 전압을 기준으로 먼지 밀도를 계산함.
*/
double getDustDensity(double outputV) {
  // 출력 전압민 먼지 밀도 변환 공식 : ug / m3 = (V - 0.9) / 5 * 1000
  double ugm3 = (outputV - 0.9) / 5 * 1000;
  // 감지되지 않은 범위값은 제거한다.
  if (ugm3 < 0) {
    ugm3 = 0;
  }
  return ugm3;
}

/**
   먼지 밀도를 기준으로 미세먼지농도 계산
 */
double getAQI(double ugm3) {
  double aqiL = 0;
  double aqiH = 0;
  double bpL = 0;
  double bpH = 0;
  double aqi = 0;

  if (ugm3 >= 0 && ugm3 <= 35) {
    aqiL = 0;    aqiH = 50;    bpL = 0;    bpH = 35;
  } else if (ugm3 > 35 && ugm3 <= 75) {
    aqiL = 50;    aqiH = 100;    bpL = 35;    bpH = 75;
  } else if (ugm3 > 75 && ugm3 <= 115) {
    aqiL = 100;    aqiH = 150;    bpL = 75;    bpH = 115;
  } else if (ugm3 > 115 && ugm3 <= 150) {
    aqiL = 150;    aqiH = 200;    bpL = 115;    bpH = 150;
  } else if (ugm3 > 150 && ugm3 <= 250) {
    aqiL = 200;    aqiH = 300;    bpL = 150;    bpH = 250;
  } else if (ugm3 > 250 && ugm3 <= 350) {
    aqiL = 300;    aqiH = 400;    bpL = 250;    bpH = 350;
  } else if (ugm3 > 350) {
    aqiL = 400;    aqiH = 500;    bpL = 350;    bpH = 500;
  }
  aqi = (aqiH - aqiL) / (bpH - bpL) * (ugm3 - bpL) + aqiL;    /*aqi 구하는 공식*/
  return aqi;
}

/**
   농도 범위에 따라서 LED의 출력이 달라지게 한다.
   아두이노 결과 출력값에 정보값도 달라짐.
*/
String getGradeInfo(double aqi) {    /*현재는 미세 먼지 농도 값을 임의로 주면*/
                                     /* 변화가 눈에 잘 보이도록 범위를 넓게 지정*/
  String gradeInfo; /*아래 주석값은 비교적 실시간 농도를 나타내기에 적합할 것이다. */
  if (aqi >= 0 && aqi <= 100) {        /* 파란색  0 - 100 */
     gradeInfo = String("Perfect");
     analogWrite(rgb_red, 0); analogWrite(rgb_green, 0); analogWrite(rgb_blue, 255);
  } else if (aqi > 100 && aqi <= 200) { /* 하늘색  100 - 200 */
     gradeInfo = String("Good"); 
     analogWrite(rgb_red, 102); analogWrite(rgb_green, 255); analogWrite(rgb_blue, 255);
  } else if (aqi > 200 && aqi <= 300) { /* 초록색 200 - 300 */
     gradeInfo = String("Mild polluted");
     analogWrite(rgb_red, 0); analogWrite(rgb_green, 255); analogWrite(rgb_blue, 0);
  } else if (aqi > 300 && aqi <= 400) { /* 주황색  300 - 400 */
     gradeInfo = String("Medium polluted");
     analogWrite(rgb_red, 255); analogWrite(rgb_green, 153); analogWrite(rgb_blue, 00);
  } else if (aqi > 400 && aqi <= 500) { /* 다홍색 400 - 500 */
     gradeInfo = String("Heavily polluted");
     analogWrite(rgb_red, 255); analogWrite(rgb_green, 51); analogWrite(rgb_blue, 0);    
  } else if (aqi > 500 && aqi <= 600) {  /* 빨간색 500 - 600 */
     gradeInfo = String("Severely polluted");
     analogWrite(rgb_red, 255); analogWrite(rgb_green, 0); analogWrite(rgb_blue, 0);    
  } else {
    gradeInfo = String("Broken roof!!!");
  }
  return gradeInfo;
}
