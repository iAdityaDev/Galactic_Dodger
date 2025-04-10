#include <FastLED.h>
#include <pitches.h>
#include <EEPROM.h>
#define EEPROM_SIZE 25 
#define Score_Address 0 
#define scorePin 19
#define LED_PIN 2
#define NUM_LEDS 200
#define ROWS 10
#define COLS 20
#define sright 12
#define shoots 14
#define sleft 13
#define max_bullets 12  // Maximum number of bullets on screen at once
#define max_enemies 12
bool gameStarted = false; 
const int motorPin = 4 ; 


const int binPin[10] = { 19, 20, 59, 60, 99, 100, 139, 140, 179, 180 };
int ssleft = 0;
int ssright = 2;
const int melody[] = {
  // The Godfather theme
  REST, 4, REST, 8, REST, 8, REST, 8, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8,  //1
  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8, NOTE_G4, 8,
  NOTE_E4, 2, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8,
  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_E4, 8, NOTE_DS4, 8,

  NOTE_D4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8,  //5
  NOTE_B4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8,
  NOTE_A4, 2, NOTE_C4, 8, NOTE_C4, 8, NOTE_G4, 8,
  NOTE_F4, 8, NOTE_E4, 8, NOTE_G4, 8, NOTE_F4, 8, NOTE_F4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_GS4, 8,

  NOTE_A4, 2, REST, 8, NOTE_A4, 8, NOTE_A4, 8, NOTE_GS4, 8,  //9
  NOTE_G4, 2, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8,
  NOTE_E4, 2, NOTE_E4, 8, NOTE_G4, 8, NOTE_E4, 8,
  NOTE_D4, 2, NOTE_D4, 8, NOTE_D4, 8, NOTE_F4, 8, NOTE_DS4, 8,

  NOTE_E4, 2, REST, 8, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8,  //13

  //repeats from 2
  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8, NOTE_G4, 8,  //2
  NOTE_E4, 2, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8,
  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_E4, 8, NOTE_DS4, 8,

  NOTE_D4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8,  //5
  NOTE_B4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8,
  NOTE_A4, 2, NOTE_C4, 8, NOTE_C4, 8, NOTE_G4, 8,
  NOTE_F4, 8, NOTE_E4, 8, NOTE_G4, 8, NOTE_F4, 8, NOTE_F4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_GS4, 8,

  NOTE_A4, 2, REST, 8, NOTE_A4, 8, NOTE_A4, 8, NOTE_GS4, 8,  //9
  NOTE_G4, 2, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8,
  NOTE_E4, 2, NOTE_E4, 8, NOTE_G4, 8, NOTE_E4, 8,
  NOTE_D4, 2, NOTE_D4, 8, NOTE_D4, 8, NOTE_F4, 8, NOTE_DS4, 8,

  NOTE_E4, 2  //13
};
const int buzzerPin = 26;
const int screen[ROWS][COLS] = {
  { 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 },
  { 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39 },
  { 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40 },
  { 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79 },
  { 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80 },
  { 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119 },
  { 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120 },
  { 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159 },
  { 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160 },
  { 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199 }
};
CRGB colors[ROWS][COLS];
CRGB leds[NUM_LEDS];

// Structure to track bullets
struct Bullet {
  int row;
  int col;
  bool active;
  unsigned long lastMoveTime;
};
struct Enemy {
  int type;
  int health;
  int row;
  int col;
  bool active;
  unsigned long lastMoveTime;
};
// array to store enemies
Enemy enemies[max_enemies];
// Array to store bullets
Bullet bullets[max_bullets];
//Timing variables Enemy
int difficultyFactor = 4;
int difficultyFactor1 = 22;
unsigned long EnemyMoveInterval = 600;  // Time between enemy movements in ms
unsigned long lastEnemySpawnTime = 0;
unsigned long enemyCooldown = 2500;  // Minimum time between spawning enemies

// Timing variables Bullet
const unsigned long bulletMoveInterval = 100;  // Time between bullet movements in ms
unsigned long lastBulletFiredTime = 0;         // Time when last bullet was fired
const unsigned long bulletCooldown = 10;       // Minimum time between firing bullets
int score = 0;
int maxhealth = 3;
void setLEDColors(CRGB colors[ROWS][COLS]) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      int led = pgm_read_word(&(screen[i][j]));
      leds[led] = colors[i][j];
    }
  }
}

// Button debouncing variables
unsigned long lastDebounceTimeLeft = 0;
unsigned long lastDebounceTimeRight = 0;
unsigned long lastDebounceTimeShoot = 0;
unsigned long debounceDelay = 50;
bool leftButtonActive = false;
bool rightButtonActive = false;

int lastLeftState = HIGH;
int lastRightState = HIGH;
int lastShootState = HIGH;
bool shootButtonPressed = false;  // Track if shoot button is being held down

void setup() {
  Serial.begin(9600);
  pinMode(scorePin , INPUT_PULLUP) ; 
  EEPROM.begin(EEPROM_SIZE) ; 

int initialHighScore = 0;
EEPROM.get(Score_Address, initialHighScore);
if (initialHighScore < 0 || initialHighScore >2) {
  initialHighScore = 0;
  EEPROM.put(Score_Address, initialHighScore);
  EEPROM.commit();
}

  pinMode(motorPin,OUTPUT) ; 
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  setupMusic();  // Initialize the music player
  startMusic();  // Start playing the music
  FastLED.show();
  pinMode(shoots, INPUT_PULLUP);
  pinMode(sleft, INPUT_PULLUP);
  pinMode(sright, INPUT_PULLUP);
  int x = random(1, 2343);
  int total = 0;
  for (int i = 15; i <= 23; i++)  // or up to A7 for many Arduino boards.
    total += (analogRead(i) + x);
  randomSeed(total);

  // Initialize all bullets as inactive
  for (int i = 0; i < max_bullets; i++) {
    bullets[i].active = false;
    enemies[i].active = false;
  }
  while (!gameStarted) {
    mainscreen();
    checkStartButton();
  }
}

void displayBinary(int num) {
  for (int i = 0; i < 10; i++) {
    if ((num >> i) & 1) {
      colors[i][19] = CRGB(255, 255, 255);
    }
  }
}

void mainscreen() {  //chamakta hua A.T.O.M
  static unsigned long lastChange = 0;
  if (millis() - lastChange > 700) {
    for (int i = 0; i < 10; i++) {
      int c1 = random(255);
      int c2 = random(255);
      int c3 = random(255);
      colors[2][0] = CRGB(c1, c2, c3);
      colors[3][0] = CRGB(c1, c2, c3);
      colors[4][0] = CRGB(c1, c2, c3);
      colors[5][0] = CRGB(c1, c2, c3);
      colors[6][0] = CRGB(c1, c2, c3);

      colors[2][2] = CRGB(c1, c2, c3);
      colors[3][2] = CRGB(c1, c2, c3);
      colors[4][2] = CRGB(c1, c2, c3);
      colors[5][2] = CRGB(c1, c2, c3);
      colors[6][2] = CRGB(c1, c2, c3);

      colors[2][6] = CRGB(c1, c2, c3);
      colors[3][6] = CRGB(c1, c2, c3);
      colors[4][6] = CRGB(c1, c2, c3);
      colors[5][6] = CRGB(c1, c2, c3);
      colors[6][6] = CRGB(c1, c2, c3);

      colors[2][10] = CRGB(c1, c2, c3);
      colors[3][10] = CRGB(c1, c2, c3);
      colors[4][10] = CRGB(c1, c2, c3);
      colors[5][10] = CRGB(c1, c2, c3);
      colors[6][10] = CRGB(c1, c2, c3);

      colors[2][13] = CRGB(c1, c2, c3);
      colors[3][13] = CRGB(c1, c2, c3);
      colors[4][13] = CRGB(c1, c2, c3);
      colors[5][13] = CRGB(c1, c2, c3);
      colors[6][13] = CRGB(c1, c2, c3);

      colors[2][15] = CRGB(c1, c2, c3);
      colors[3][15] = CRGB(c1, c2, c3);
      colors[4][15] = CRGB(c1, c2, c3);
      colors[5][15] = CRGB(c1, c2, c3);
      colors[6][15] = CRGB(c1, c2, c3);

      colors[2][19] = CRGB(c1, c2, c3);
      colors[3][19] = CRGB(c1, c2, c3);
      colors[4][19] = CRGB(c1, c2, c3);
      colors[5][19] = CRGB(c1, c2, c3);
      colors[6][19] = CRGB(c1, c2, c3);

      colors[2][1] = CRGB(c1, c2, c3);
      colors[4][1] = CRGB(c1, c2, c3);
      colors[2][4] = CRGB(c1, c2, c3);
      colors[2][5] = CRGB(c1, c2, c3);
      colors[2][7] = CRGB(c1, c2, c3);
      colors[2][8] = CRGB(c1, c2, c3);
      colors[2][11] = CRGB(c1, c2, c3);
      colors[2][12] = CRGB(c1, c2, c3);
      colors[6][11] = CRGB(c1, c2, c3);
      colors[6][12] = CRGB(c1, c2, c3);
      colors[4][16] = CRGB(c1, c2, c3);
      colors[4][17] = CRGB(c1, c2, c3);
      colors[4][18] = CRGB(c1, c2, c3);
      colors[3][16] = CRGB(c1, c2, c3);
      colors[3][18] = CRGB(c1, c2, c3);
      colors[5][17] = CRGB(c1, c2, c3);
      setLEDColors(colors);
      FastLED.show();
      lastChange = millis();
      updateMusic();
    }
    FastLED.show();
  }
}
//MOOOSIC
int tempo = 80;
int currentNote = 0;
int notesCount;
unsigned long previousNoteTime = 0;
int noteDuration = 0;
int pauseBetweenNotes = 0;
bool isPlaying = false;
bool isTonePlaying = false;

// Calculate the number of notes in the melody
const int noteCount = sizeof(melody) / sizeof(melody[0]) / 2;

// Configure whole note duration in milliseconds
const int wholenote = (60000 * 4) / tempo;

// Initialize the music player
void setupMusic() {
  notesCount = noteCount;
  isPlaying = true;
}

// Start or restart the music
void startMusic() {
  currentNote = 0;
  isPlaying = true;
  isTonePlaying = false;
  previousNoteTime = millis();
}

// Pause the music
void pauseMusic() {
  isPlaying = false;
  noTone(buzzerPin);
}

// Resume the music from where it left off
void resumeMusic() {
  isPlaying = true;
}

// Stop the music and reset to beginning
void stopMusic() {
  isPlaying = false;
  currentNote = 0;
  noTone(buzzerPin);
}

// Update music player - call this in your loop() function
void updateMusic() {
  if (!isPlaying) {
    return;
  }

  unsigned long currentTime = millis();

  // If we're playing a tone and it's time to stop it
  if (isTonePlaying && currentTime - previousNoteTime >= noteDuration) {
    noTone(buzzerPin);
    isTonePlaying = false;
    pauseBetweenNotes = noteDuration * 0.1;  // 10% pause between notes
    previousNoteTime = currentTime;
    return;
  }

  // If we're in the pause between notes
  if (!isTonePlaying && currentTime - previousNoteTime >= pauseBetweenNotes) {
    // Move to the next note
    if (currentNote >= notesCount * 2) {
      // Reached the end of the melody, restart
      currentNote = 0;
    }

    // Calculate note duration
    int divider = melody[currentNote + 1];
    if (divider > 0) {
      // Regular note
      noteDuration = wholenote / divider;
    } else if (divider < 0) {
      // Dotted note
      noteDuration = (wholenote / abs(divider)) * 1.5;
    }

    // Play the note (only if it's not a REST)
    if (melody[currentNote] != REST) {
      tone(buzzerPin, melody[currentNote], noteDuration * 0.9);
      isTonePlaying = true;
    } else {
      // For REST notes, just wait the duration
      isTonePlaying = true;
    }

    previousNoteTime = currentTime;
    currentNote += 2;  // Move to the next note-duration pair
  }
}

void drawspaceship() {
  CRGB colo;
  switch (maxhealth) {
    case 2:
      colo = CRGB(255, 248, 23);
      break;
    case 3:
      colo = CRGB(0, 0, 255);
      break;
    case 1:
      colo = CRGB(255, 0, 0);
      break;
    default:
      shipPhatna();
      delay(3000);
      for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
          colors[row][col] = CRGB::Black;
        }
      }
      setLEDColors(colors);
      FastLED.show();
      gameover();
      break;
  }
  for (int i = ssleft; i <= ssright; i++) {
    colors[i][0] = colo;
  }
  colors[ssleft + 1][1] = colo;
}
void shipPhatna() {
  for (int i = ssleft; i <= ssright; i++) {
    colors[i][0] = CRGB(255, 0, 0);
  }
  colors[ssleft + 1][1] = CRGB(255, 0, 0);
  colors[ssleft - 1][1] = CRGB(255, 64, 0);
  colors[ssleft - 2][2] = CRGB(255, 128, 0);
  colors[ssleft][1] = CRGB(255, 64, 0);
  colors[ssleft][2] = CRGB(255, 128, 0);
  colors[ssleft - 1][3] = CRGB(255, 64, 0);
  colors[ssleft - 2][4] = CRGB(255, 128, 0);
  colors[ssleft + 1][2] = CRGB(255, 64, 0);
  colors[ssleft + 1][3] = CRGB(255, 64, 0);
  colors[ssleft + 2][2] = CRGB(255, 128, 0);
  colors[ssleft + 2][1] = CRGB(255, 64, 0);
  colors[ssleft + 3][1] = CRGB(255, 128, 0);
  colors[ssleft + 3][2] = CRGB(255, 128, 0);
  setLEDColors(colors);
  FastLED.show();
}
void spawnEnemies() {
  unsigned long currentTime = millis();
  if (currentTime - lastEnemySpawnTime < enemyCooldown) {
    return;
  }

  int spawnCount = (random(1, 100) < 35) ? 2 : 1;
  for (int i = 0; i < max_enemies && spawnCount > 0; i++) {
    if (!enemies[i].active) {
      int rs = random(1, 100);
      if (rs > 4 && rs < 32) {
        enemies[i].type = 2;
        enemies[i].health = 2;
      } else if (rs <= 4) {
        enemies[i].type = 3;
        enemies[i].health = 10;
        spawnCount = 1;
      } else {
        enemies[i].type = 1;
        enemies[i].health = 1;
      }
      if (enemies[i].type == 3) {
        enemies[i].row = 3;
        enemies[i].col = 18;
      } else {
        enemies[i].row = random(1, 8);
        enemies[i].col = 18;
      }
      enemies[i].active = true;
      enemies[i].lastMoveTime = currentTime;
      lastEnemySpawnTime = currentTime;
      spawnCount--;
    }
  }
}


void fireBullet() {
  unsigned long currentTime = millis();

  // Add cooldown to prevent firing too rapidly
  if (currentTime - lastBulletFiredTime < bulletCooldown) {
    return;
  }

  // Find first inactive bullet
  for (int i = 0; i < max_bullets; i++) {
    if (!bullets[i].active) {
      bullets[i].row = ssleft + 1;  // Middle of ship
      bullets[i].col = 2;           // Just in front of ship
      bullets[i].active = true;
      bullets[i].lastMoveTime = currentTime;
      lastBulletFiredTime = currentTime;

      return;
    }
  }
}
void updateEnemies() {
  unsigned long currentTime = millis();
  int moveBoss = EnemyMoveInterval;
  for (int i = 0; i < max_enemies; i++) {
    if (enemies[i].active) {
      if (enemies[i].type == 1)
        colors[enemies[i].row][enemies[i].col] = CRGB(0, 255, 0);
      else if (enemies[i].type == 2) colors[enemies[i].row][enemies[i].col] = CRGB(255, 45, 241);
      else if (enemies[i].type == 3) {
        colors[enemies[i].row][enemies[i].col] = CRGB(150, 150, random(0, 255));
        colors[enemies[i].row + 1][enemies[i].col] = CRGB(150, 150, random(0, 255));
        colors[enemies[i].row + 2][enemies[i].col] = CRGB(150, 150, random(0, 255));
        colors[enemies[i].row + 1][enemies[i].col - 1] = CRGB(150, 150, random(0, 255));
        colors[enemies[i].row][enemies[i].col - 2] = CRGB(150, 150, random(0, 255));
        colors[enemies[i].row + 1][enemies[i].col - 2] = CRGB(150, 150, random(0, 255));
        colors[enemies[i].row + 2][enemies[i].col - 2] = CRGB(150, 150, random(0, 255));
      }
    }

    if (enemies[i].type == 3) {
      EnemyMoveInterval = 2000;
    }
    if (currentTime - enemies[i].lastMoveTime > EnemyMoveInterval) {
      enemies[i].col--;
      if (enemies[i].type != 3) {
        if (random(1, 100) < 25) {
          int new_row = enemies[i].row + random(-1, 2);
          if (new_row != 0 && new_row != ROWS - 1) {
            enemies[i].row = new_row;
          }
        }
        if (random(1, 100) < 20) {
          int new_row = enemies[i].row + random(-1, 2);
          if (new_row != 0 && new_row != ROWS - 1) {
            enemies[i].row = new_row;
          }
        }
      }

      enemies[i].lastMoveTime = currentTime;

      int er = enemies[i].row;
      int ec = enemies[i].col;

      bool shipCollision = false;
      if (enemies[i].type == 3) {
        if ((ssleft == er && 0 == enemies[i].col - 2) || (ssleft == er + 1 && 0 == enemies[i].col - 2) || (ssleft == er + 2 && 0 == enemies[i].col - 2))
          shipCollision = true;
      } else {
        if (ec == 0) {
          if (er == ssleft && enemies[i].active == true) {
            shipCollision = true;
          } else if (er == ssright && enemies[i].active == true) {
            shipCollision = true;
          }
        } else if (ec == 1 && er == ssleft + 1 && enemies[i].active == true) {
          shipCollision = true;
        }
      }

      if (shipCollision) {
        maxhealth--;
        enemies[i].active = false;
      }

      for (int j = 0; j < max_bullets; j++) {
        if (enemies[i].type == 3) {
          int br = bullets[j].row;
          int bc = bullets[j].col;
          if (bullets[j].active) {
            if ((br == er && bc == enemies[i].col - 2) || (br == er + 1 && bc == enemies[i].col - 2) || (br == er + 2 && bc == enemies[i].col - 2)) {
              enemies[i].health--;
              bullets[j].active = false;
              if (enemies[i].health == 0) {
                enemies[i].active = false;
                score++;
              }
              break;
            }
          }
        }

        else if (bullets[j].active && checkCollision(j, i)) {
          enemies[i].type--;
          enemies[i].health--;
          bullets[j].active = false;
          if (enemies[i].health == 0) {
            enemies[i].active = false;
            score++;
          }
          break;
        }
      }
    }
    if (enemies[i].type == 3 && enemies[i].col < 0 && enemies[i].active == true) {
      gameover();
    }
    if (enemies[i].col < 0 && enemies[i].active == true) {
      enemies[i].active = false;
      if (score > 0) score--;
    }
  }
  EnemyMoveInterval = moveBoss;
}

void updateBullets() {
  unsigned long currentTime = millis();

  for (int i = 0; i < max_bullets; i++) {
    if (bullets[i].active) {
      // Draw the bullet
      colors[bullets[i].row][bullets[i].col] = CRGB(255, 0, 0);

      // Check if it's time to move the bullet
      if (currentTime - bullets[i].lastMoveTime > bulletMoveInterval) {
        // Move bullet forward
        bullets[i].col++;
        bullets[i].lastMoveTime = currentTime;

        for (int j = 0; j < max_enemies; j++) {

          if (enemies[j].active && enemies[j].type == 3) {
            // Check all 7 parts of type 3 enemy
            if ((bullets[i].row == enemies[j].row && bullets[i].col == enemies[j].col) || (bullets[i].row == enemies[j].row + 1 && bullets[i].col == enemies[j].col) || (bullets[i].row == enemies[j].row + 2 && bullets[i].col == enemies[j].col) || (bullets[i].row == enemies[j].row + 1 && bullets[i].col == enemies[j].col - 1) || (bullets[i].row == enemies[j].row && bullets[i].col == enemies[j].col - 2) || (bullets[i].row == enemies[j].row + 1 && bullets[i].col == enemies[j].col - 2) || (bullets[i].row == enemies[j].row + 2 && bullets[i].col == enemies[j].col - 2)) {
              enemies[j].health--;
              bullets[i].active = false;
              if (enemies[j].health <= 0) {
                enemies[j].active = false;
                score++;
              }
              break;
            }
          } else if (enemies[j].active && checkCollision(i, j)) {
            if (enemies[j].type == 2) enemies[j].type--;
            enemies[j].health--;
            bullets[i].active = false;
            if (enemies[j].health == 0) {
              enemies[j].active = false;
              score++;
            }
            break;
          }
        }
        // Check if bullet reached the end of the screen
        if (bullets[i].col >= COLS) {
          bullets[i].active = false;
        }
      }
    }
  }
}

void checkStartButton() {
  int buttonState = digitalRead(shoots);
  if (buttonState == LOW) {
    delay(50);  // Simple debounce
    if (digitalRead(shoots) == LOW) {
      gameStarted = true;
      FastLED.clear();
      FastLED.show();
    }
  }
}

bool checkCollision(int i, int j) {
  if (!bullets[i].active || !enemies[j].active) {
    return false;
  }
  int er = enemies[j].row;
  int ec = enemies[j].col;
  int br = bullets[i].row;
  int bc = bullets[i].col;

  return (er == br && ec == bc);
}


void gameover() {
  digitalWrite(motorPin, HIGH); // Turn motor ON   


  colors[2][6] = CRGB(255, 0, 0);
  colors[3][6] = CRGB(255, 0, 0);
  colors[2][7] = CRGB(255, 0, 0);
  colors[3][7] = CRGB(255, 0, 0);
  colors[2][12] = CRGB(255, 0, 0);
  colors[3][12] = CRGB(255, 0, 0);
  colors[3][13] = CRGB(255, 0, 0);
  colors[2][13] = CRGB(255, 0, 0);

  //white face
  colors[0][4] = CRGB(255, 255, 255);
  colors[1][4] = CRGB(255, 255, 255);
  colors[2][4] = CRGB(255, 255, 255);
  colors[3][4] = CRGB(255, 255, 255);
  colors[4][4] = CRGB(255, 255, 255);
  colors[5][4] = CRGB(255, 255, 255);
  colors[6][4] = CRGB(255, 255, 255);
  colors[0][5] = CRGB(255, 255, 255);
  colors[0][6] = CRGB(255, 255, 255);
  colors[0][7] = CRGB(255, 255, 255);
  colors[0][8] = CRGB(255, 255, 255);
  colors[0][9] = CRGB(255, 255, 255);
  colors[0][10] = CRGB(255, 255, 255);
  colors[0][11] = CRGB(255, 255, 255);
  colors[0][12] = CRGB(255, 255, 255);
  colors[0][13] = CRGB(255, 255, 255);
  colors[0][14] = CRGB(255, 255, 255);
  colors[0][15] = CRGB(255, 255, 255);
  colors[1][5] = CRGB(255, 255, 255);
  colors[1][6] = CRGB(255, 255, 255);
  colors[1][7] = CRGB(255, 255, 255);
  colors[1][8] = CRGB(255, 255, 255);
  colors[1][9] = CRGB(255, 255, 255);
  colors[1][10] = CRGB(255, 255, 255);
  colors[1][11] = CRGB(255, 255, 255);
  colors[1][12] = CRGB(255, 255, 255);
  colors[1][13] = CRGB(255, 255, 255);
  colors[1][14] = CRGB(255, 255, 255);
  colors[1][15] = CRGB(255, 255, 255);
  colors[2][5] = CRGB(255, 255, 255);
  colors[3][5] = CRGB(255,255,255) ; 
  colors[2][8] = CRGB(255, 255, 255);
  colors[2][9] = CRGB(255, 255, 255);
  colors[2][10] = CRGB(255, 255, 255);
  colors[2][11] = CRGB(255, 255, 255);
  colors[3][9] = CRGB(255, 255, 255);
  colors[3][10] = CRGB(255, 255, 255);
  colors[3][11] = CRGB(255, 255, 255);
  colors[3][8] = CRGB(255, 255, 255);
  colors[3][14] = CRGB(255, 255, 255);
  colors[3][15] = CRGB(255, 255, 255);
  colors[2][14] = CRGB(255, 255, 255);
  colors[2][15] = CRGB(255, 255, 255);
  colors[4][5] = CRGB(255, 255, 255);
  colors[4][6] = CRGB(255, 255, 255);
  colors[4][7] = CRGB(255, 255, 255);
  colors[4][8] = CRGB(255, 255, 255);
  colors[4][11] = CRGB(255, 255, 255);
  colors[4][12] = CRGB(255, 255, 255);
  colors[4][13] = CRGB(255, 255, 255);
  colors[4][14] = CRGB(255, 255, 255);
  colors[4][15] = CRGB(255, 255, 255);
  colors[5][5] = CRGB(255, 255, 255);
  colors[5][6] = CRGB(255, 255, 255);
  colors[5][6] = CRGB(255, 255, 255);
  colors[5][8] = CRGB(255, 255, 255);
  colors[5][9] = CRGB(255, 255, 255);
  colors[5][10] = CRGB(255, 255, 255);
  colors[5][11] = CRGB(255, 255, 255);
  colors[5][12] = CRGB(255, 255, 255);
  colors[5][13] = CRGB(255, 255, 255);
  colors[5][14] = CRGB(255, 255, 255);
  colors[5][15] = CRGB(255, 255, 255);
  colors[6][5] = CRGB(255, 255, 255);
  colors[6][6] = CRGB(255, 255, 255);
  colors[6][7] = CRGB(255, 255, 255);
  colors[6][12] = CRGB(255, 255, 255);
  colors[6][13] = CRGB(255, 255, 255);
  colors[6][14] = CRGB(255, 255, 255);
  colors[6][15] = CRGB(255, 255, 255);
  colors[7][7] = CRGB(255, 255, 255);
  colors[7][12] = CRGB(255, 255, 255);
  colors[8][10] = CRGB(255, 255, 255);
  colors[8][11] = CRGB(255, 255, 255);
  colors[8][7] = CRGB(255, 255, 255);
  colors[8][8] = CRGB(255, 255, 255);
  colors[8][9] = CRGB(255, 255, 255);
  colors[8][12] = CRGB(255, 255, 255);
  colors[9][7] = CRGB(255, 255, 255);
  colors[9][8] = CRGB(255, 255, 255);
  colors[9][9] = CRGB(255, 255, 255);
  colors[9][10] = CRGB(255, 255, 255);
  colors[9][11] = CRGB(255, 255, 255);
  colors[9][12] = CRGB(255, 255, 255);
  colors[5][7] = CRGB(255,255,255) ;  


  //nose
  colors[4][9] = CRGB(4, 69, 217);
  colors[4][10] = CRGB(4, 69, 217);

  //teeth
  colors[6][8] = CRGB(25, 118, 3);
  colors[6][9] = CRGB(217, 181, 5);
  colors[6][10] = CRGB(25, 118, 3);
  colors[6][11] = CRGB(25, 118, 3);
  colors[7][8] = CRGB(25, 118, 3);
  colors[7][9] = CRGB(25, 118, 3);
  colors[7][10] = CRGB(25, 118, 3);
  colors[7][11] = CRGB(25, 118, 3);
  setLEDColors(colors);
  FastLED.show();
// circle dia 3.9mm, outer = 35.4 mm, inner = 24.3 mm, dist between mid-points of black parts = 2.1cm = 21 mm
  delay(9500);
  updateHighScore();
  esp_restart();
}
void displayBinaryHighScore() {
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      colors[row][col] = CRGB::Black;
    }
  }

  int num;
  EEPROM.get(Score_Address, num);
  
  // Temporary buffer to preserve game state
  CRGB tempColors[ROWS][COLS];
  memcpy(tempColors, colors, sizeof(colors));

  // Show score without blocking
  for (int i = 0; i < 10; i++) {
    if ((num >> i) & 1) {
      colors[i][10] = CRGB::White;
    }
  }
  setLEDColors(colors);
  FastLED.show();
  
  // Restore game state
  memcpy(colors, tempColors, sizeof(colors));
}

void updateHighScore() {
  static bool alreadyUpdated = false;
  if (!alreadyUpdated) {
    int currentHighScore;
    EEPROM.get(Score_Address, currentHighScore);
    if (score > currentHighScore) {
      EEPROM.put(Score_Address, score);
      EEPROM.commit();
    }
    alreadyUpdated = true;
  }
}
int reduceTime = 10;
int changeBG = 20;
void loop() {

// Replace the existing scorePin check with debounce logic
  static bool scoreDisplayActive = false;
  static unsigned long scoreDisplayStart = 0;

  if (digitalRead(scorePin) == LOW && !scoreDisplayActive) {
    scoreDisplayActive = true;
    scoreDisplayStart = millis();
    displayBinaryHighScore();
  }

  if (scoreDisplayActive && (millis() - scoreDisplayStart > 4000)) {
    scoreDisplayActive = false;
    // Clear score display
    FastLED.clear();
    FastLED.show();
  } 

  // Clear the display
  Serial.println(score);
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      if (score % reduceTime == 0) {
      colors[row][col] = CRGB::Black;
    reduceTime += 20;
  }
    }
  }

  // Read the current button states
  int currentstate_left = digitalRead(sleft);
  int currentstate_right = digitalRead(sright);
  int currentstate_shoot = digitalRead(shoots);
  int currentstate_score = digitalRead(score) ; 

  // Left button with debouncing
  if (currentstate_left != lastLeftState) {
    lastDebounceTimeLeft = millis();
  }

  if ((millis() - lastDebounceTimeLeft) > debounceDelay) {
    if (currentstate_left == LOW && !leftButtonActive && ssleft > 0) {
      // Button is newly pressed
      ssright--;
      ssleft--;
      leftButtonActive = true;
    } else if (currentstate_left == HIGH) {
      // Button is released
      leftButtonActive = false;
    }
  }
  lastLeftState = currentstate_left;

  // Right button with debouncing
  if (currentstate_right != lastRightState) {
    lastDebounceTimeRight = millis();
  }

  if ((millis() - lastDebounceTimeRight) > debounceDelay) {
    if (currentstate_right == LOW && !rightButtonActive && ssright < 9) {
      // Button is newly pressed
      ssright++;
      ssleft++;
      rightButtonActive = true;
    } else if (currentstate_right == HIGH) {
      // Button is released
      rightButtonActive = false;
    }
  }
  lastRightState = currentstate_right;

  // Shoot button with improved handling
  if (currentstate_shoot != lastShootState) {
    lastDebounceTimeShoot = millis();
  }

  if ((millis() - lastDebounceTimeShoot) > debounceDelay) {
    // Only fire when button is first pressed, not while held
    if (currentstate_shoot == LOW && !shootButtonPressed) {
      fireBullet();
      shootButtonPressed = true;

    } else if (currentstate_shoot == HIGH) {
      shootButtonPressed = false;
    }
  }
  lastShootState = currentstate_shoot;

  if (score % reduceTime == 0) {
    EnemyMoveInterval -= difficultyFactor * score;
    enemyCooldown -= difficultyFactor1 * score;
    reduceTime += 10;
  }

  // Update and draw active bullets
  drawspaceship();

  updateEnemies();

  // Draw spaceship
  spawnEnemies();
  updateBullets();
  displayBinary(score);

  // Update LED matrix
  setLEDColors(colors);
  FastLED.show();
  updateMusic();
  delay(20);  // Faster refresh rate for smoother movement
}