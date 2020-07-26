#include <Debouncer.h>
#include <Encoder.h>
#include <HID-Project.h>

#define ENC_PIN1 5
#define ENC_PIN2 6
#define SW_PIN 4
#define DEBOUNCE_MS 22


Debouncer debouncer(SW_PIN, DEBOUNCE_MS);
Encoder encoder(ENC_PIN1, ENC_PIN2);
long position = 0;
boolean muted = false;


void setup() {
  Serial.begin(115200);
  pinMode(SW_PIN, INPUT_PULLUP);
  Consumer.begin();
}

void loop() {
  debouncer.update();

  long newPosition = encoder.read() / 4;

  if (newPosition != position) {
    if (newPosition < position) {
      Serial.print(F("Left"));
      Consumer.write(MEDIA_VOLUME_DOWN);
    }

    if (newPosition > position) {
      Serial.print(F("Right"));
      Consumer.write(MEDIA_VOLUME_UP);
    }

    Serial.print(F(" ["));
    Serial.print(position);
    Serial.print(F(" => "));
    Serial.print(newPosition);
    Serial.println(F("]");

    position = newPosition;
  }

  if (debouncer.edge()) {
    if (debouncer.falling()) {
      muted = !muted;
      Consumer.write(MEDIA_VOLUME_MUTE);

      Serial.print(F("Click ["));

      if (muted) {
        Serial.print(F("muted"));
      }
      else {
        Serial.print(F("unmuted"));
      }

      Serial.println(F("]"));
    }
  }

  Serial.flush();
}
