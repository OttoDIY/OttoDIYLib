#define echoPin 9
#define trigPin 8

int object = 10;
const int LS = 2;
const int RS = 3;
long duration, distance;

void setup () {
  Serial.begin (9600);
  pinMode (echoPin, INPUT);
  pinMode (trigPin, OUTPUT);
  pinMode (3, OUTPUT);
  pinMode (4, OUTPUT);
  pinMode (5, OUTPUT);
  pinMode (6, OUTPUT);
}

void loop () {
  distance = ping ();
  Serial.println (distance);
  if (distance <= object) {
    mstop ();
  }
  else if (distance > object) {
   control (); 
  }
}

void control () {
  int leftSensor = analogRead (LS);
  int rightSensor = analogRead (RS);

    if (leftSensor < 30 && rightSensor < 30 ) {
      forward();
    }
    else if (leftSensor < 30 && rightSensor > 1000) {
      left ();
    }
    else if (leftSensor > 1000 && rightSensor < 30) {
      right ();
    }
}

void forward () {
  digitalWrite (3, HIGH);
  digitalWrite (4, LOW);
  digitalWrite (5, HIGH);
  digitalWrite (6, LOW);
}

void left () {
  digitalWrite (3, HIGH);
  digitalWrite (4, LOW);
  digitalWrite (5, LOW);
  digitalWrite (6, HIGH);  


void right () {
  digitalWrite (3, LOW);
  digitalWrite (4, HIGH);
  digitalWrite (5, HIGH);
  digitalWrite (6, LOW);  
}

void mstop () {
  digitalWrite (3, LOW);
  digitalWrite (4, LOW);
  digitalWrite (5, LOW);
  digitalWrite (6, LOW);  
}

long ping () {
  digitalWrite (trigPin, LOW);
  delayMicroseconds (2);
  digitalWrite (trigPin, HIGH);
  delayMicroseconds (10);
  digitalWrite (trigPin, LOW);
  duration  = pulseIn (echoPin, HIGH );
  return duration * 0.034 /2;
}
