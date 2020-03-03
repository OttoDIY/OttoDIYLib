#include <Otto9.h>

Otto9 Otto;

#define PIN_Trigger 8 // ultrasound
#define PIN_Echo 9 // ultrasound
#define PIN_YL 2 // left leg, servo[0]
#define PIN_YR 3 // right leg, servo[1]
#define PIN_RL 4 // left foot, servo[2]
#define PIN_RR 5 // right foot, servo[3]
#define PIN_Trigger 8 // ultrasound
#define PIN_Echo 9 // ultrasound
#define PIN_Buzzer  13 //buzzer


// DURATION OF THE NOTES 
#define Q 0.25//quarter 1/4  you can change this value changing all the others
#define H 0.5 //half 2/4
#define E 0.125   //eighth 1/8
#define S 0.0625 // sixteenth 1/16
#define W 1 // whole 4/4

float merry_xmas[] = {note_G3,Q,note_C4,Q,note_C4,E,note_D4,E,note_C4,E,note_B3,E,
                 note_A3,Q,note_A3,Q,note_A3,Q,note_D4,Q,note_D4,E,note_E4,E,note_D4,E,
                 note_C4,E,note_B3,Q,note_G3,Q,note_B3,Q,note_E4,Q,note_E4,E,note_F4,E,
                 note_E4,E,note_D4,E,note_C4,Q,note_A3,Q,note_G3,E,note_G3,E,note_A3,Q,
                 note_D4,Q,note_B3,Q,note_C4,H};
float merry_xmas_size = sizeof(merry_xmas)/sizeof(float);
float merry_xmas_tempo = 172;
float o_tannenbaum[] = {note_G3,Q,note_C4,E,note_C4,E,note_C4,Q,note_D4,Q,note_E4,E,note_E4,E,
                        note_E4,0.375,note_E4,E,note_D4,E,note_E4,E,note_F4,Q,note_B3,Q,
                        note_D4,Q,note_C4,Q,note_G3,Q,
                        note_C4,E,note_C4,E,note_C4,Q,note_D4,Q,note_E4,E,note_E4,E,
                        note_E4,Q+E,note_E4,E,note_D4,E,note_E4,E,note_F4,Q,note_B3,Q,
                        note_D4,Q,note_C4,Q,0,E,note_G4,E,
                        note_G4,E,note_E4,E,note_A4,Q+E,note_G4,E,note_G4,E,note_F4,E,note_F4,Q+E,
                        note_F4,E,note_F4,E,note_D4,E,note_G4,Q+E,note_F4,E,note_F4,E,note_E4,E,
                        note_E4,Q,note_G3,Q,
                        note_C4,E,note_C4,E,note_C4,Q,note_D4,Q,note_E4,E,note_E4,E,note_E4,Q,0,E,
                        note_E4,E,note_D4,E,note_E4,E,note_F4,Q,note_B3,Q,note_D4,Q,note_C4,Q
                        };
float o_tannenbaum_size = sizeof(o_tannenbaum)/sizeof(float);
float o_tannenbaum_tempo = 72;

float *songs[] = {merry_xmas, &merry_xmas_size, &merry_xmas_tempo,
                  o_tannenbaum, &o_tannenbaum_size, &o_tannenbaum_tempo};
int songs_number = 2;

boolean sing = true;

void setup() {
    randomSeed(analogRead(0));
    Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo);
    Otto.home();
    delay(1000);
}

void loop() {

  if (sing) {
    sing = false;
    int song = random(0, songs_number)*3;
    playSong(songs[song], *songs[song+1], *songs[song+2]);
  }

  if (Otto.getDistance() < 15) {
    sing = true;
  }
    
}

void playSong(float song[],int size, int tempo) {
    tempo = (60000/tempo)*4;
    for (int i=0; i<size; i+=2) {
      playNote(song[i], tempo*song[i+1]);
    }
}

void playNote (float note, float duration) {
    float silent=1.2;
    duration=duration/silent;
    tone(PIN_Buzzer, note, duration);
    delay(duration*silent);
}




