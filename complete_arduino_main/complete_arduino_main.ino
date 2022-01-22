#define trigPin1 13                   // 1번 센서 trigPin -> 13 설정
#define echoPin1 12                   // 1번 센서 echoPin -> 12 설정
#define trigPin2 11                   // 2번 센서 trigPin -> 11 설정
#define echoPin2 10                   // 2번 센서 echoPin -> 10 설정

void setup() {
  Serial.begin (9600);
  pinMode(trigPin1, OUTPUT);        // trigPin 핀을 출력핀으로 설정
  pinMode(echoPin1, INPUT);         // echoPin 핀을 입력핀으로 설정
  pinMode(trigPin2, OUTPUT);        // trigPin 핀을 출력핀으로 설정
  pinMode(echoPin2, INPUT);         // echoPin 핀을 입력핀으로 설정
}

int distanceMeasure(int trig, int echo) {    // 1번 센서 거리 측정 함수
  long duration, distance;                   // 각 변수 선언
  int find;
  digitalWrite(trig, LOW);                   // trigPin에 LOW 출력
  delayMicroseconds(2);                      // 2 마이크로초 딜레이
  digitalWrite(trig, HIGH);                  // trigPin에 HIGH 출력
  delayMicroseconds(10);                     // 10마이크로초 딜레이
  digitalWrite(trig, LOW);                   // trigPin에 LOW 출력
  duration = pulseIn(echo, HIGH);            // echoPin핀에서 펄스값 받기
  distance = duration * 17 / 1000;           // duration을 연산하여 센싱한 거리값을 distance에 저장

  if (distance >= 150 || distance <= 0) {    // 거리 값에 따라 find 값 return
    find = 0;
  }
  else {
    find = 1;
  }
  return find;
}

void determine(String num, int detect_trig, int detect_echo) {      // 감지 판단 함수
  Serial.print(num + "번 센서: 감지된 차 있음. 1차 측정 시작");
  delay(30000);
  Serial.flush();
  int measure = distanceMeasure(detect_trig, detect_echo);

  if (measure == 1) {
    Serial.print(num + "번 센서: 감지된 차 있음. 2차 측정 시작");
    delay(30000);
    Serial.flush();
    measure = distanceMeasure(detect_trig, detect_echo);

    if (measure == 1) {
      Serial.print(num + "번 센서: 신고 접수");
    }
    else {
      Serial.print(num + "번 센서: 차량 이탈");
    }
  }
  else {
    Serial.print(num + "번 센서: 차량 이탈");
  }
  Serial.flush();
  delay(60000);
}


void loop() {
  while (true) {
    int detect_trig, detect_echo, measure;
    int detect1 = 0;
    int detect2 = 0;

    detect1 = distanceMeasure(trigPin1, echoPin1);
    detect2 = distanceMeasure(trigPin2, echoPin2);
    String num;
    if (detect1 == 0 && detect2 == 0) {
      Serial.print("1, 2번 센서: 감지된 차 없음");
      delay(60000);
      Serial.flush();
    }
    else if (detect1 == 1 && detect2 == 1) {
      detect_trig = trigPin1;
      detect_echo = echoPin1;
      num = "1, 2";
      determine(num, detect_trig, detect_echo);
    }
    else if (detect1 == 1 && detect2 == 0) {
      detect_trig = trigPin1;
      detect_echo = echoPin1;
      num = "1";
      determine(num, detect_trig, detect_echo);
    }
    else if (detect1 == 0 && detect2 == 1) {
      detect_trig = trigPin2;
      detect_echo = echoPin2;
      num = "2";
      determine(num, detect_trig, detect_echo);
    }
  }
}
