/*
  Big goal timer - Reach a time target with a big display
  1. Push start button to start the timer
  2. LEDs flash until specific intervals are met and then stay on
  3. Pushing reset 3 times in a row resets the timer (prevents accidental resetting)
  
  TODO: 
  1. Add interval audio beep
  2. Add goal melody (random?)
  3. Make timer settable ( hours/minutes buttons )
  4. Rename to procrastaclock?
  5. Put on a real board, fabricate PCB and mount in box
  6. Hook up battery, look at power savings
  7. Write count to eeprom (write 0 on reset) - this way we can continue from power failure.
  8. Make much more efficient.. lots of checks are being done at impossible times.
 */

const unsigned int goal = 3600 * 1;    // Target time to count to

const int greenLED = 12;       // Pin out for green LED
const int yellowLED = 11;      // Pin out for yellow LED
const int redLED = 10;         // Pin out for red LED

const int start_button = 2;    // Pin in for start/pause button
const int reset_button = 3;    // Pin in for start/pause button

int flashstate = LOW;          // Initial state for flashing LEDs
    
int timerstate = 0;            // Global timer state: 0 = stopped, 1 = running
long sec = 1000;               // 1 second in milliseconds
long last_uptime = 0;          // Used for clock()

unsigned int interval_size = goal / 3;  // Interval between LEDs lighting up in seconds
unsigned int last_clock = 0;            // 0 seconds for last clock time
unsigned int clock_count = 0;           // 0 seconds start for clock

int reset_confirm = 0;         // Reset confirm value - must be 2 to reset the timer

void setup()
{
  Serial.begin(9600);
  Serial.println(goal);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  
  pinMode(start_button, INPUT);
  pinMode(reset_button, INPUT);
}


void loop()
{
  // Check to see if start/pause button pressed
  if (digitalRead(start_button) == HIGH) {
    if (timerstate == 0) {
      timerstate = 1;
    } else {
      timerstate = 0; 
    }
    delay(300); // Give user time to lift finger off button :)
  }
  
  // Reset and stop the timer
  // TODO: Add some logic to reset the reset_confirm value if 5 seconds or so pass, making it even more fool proof
  if (digitalRead(reset_button) == HIGH) {
    Serial.println(reset_confirm);
    if (reset_confirm >= 2) {
      reset_stop();
    } else {
      reset_confirm++; 
    }
    delay(300); // Give user time to lift finger off button :)
  }
  
  
  if (timerstate == 1) {
    interval_leds(); // Handle time interval LEDs
  }
  
  clock(); // Update the clock, starts and stops via state of timerstate 
}

void reset_stop()
{
  /* Reset and stop the timer, turn off LEDs */
  clock_count = timerstate = reset_confirm = last_clock = 0; // Reset everything!

  flashstate == LOW;
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);  
}


void clock()
{
  /* Set number of seconds the timer has been in the running state */ 
  unsigned long uptime = millis();
 
  if(uptime - last_uptime > sec && timerstate) {
    Serial.println(clock_count);
    last_uptime = uptime;
    clock_count++;
  }
}

void interval_leds()
{
  /* Light up the different interval LEDs */
  
  if (clock_count < interval_size) {
    flash_led(redLED);
  }
  
  if (clock_count >= interval_size && clock_count <= interval_size * 2) {
    digitalWrite(redLED, HIGH);
    flash_led(yellowLED);
  }
  
  if (clock_count > interval_size * 2) {
    digitalWrite(yellowLED, HIGH);
    flash_led(greenLED);
  }
  
  if (clock_count == interval_size * 3) {
    digitalWrite(greenLED, HIGH);
    timerstate = 0; // Stop the timer, goal reached!
  }
}

void flash_led(int led)
{
  /* Flash an LED on and off at clock speed */
  
  if (last_clock < clock_count) {
    last_clock = clock_count;
    if (flashstate == LOW) {
      flashstate = HIGH;
    } else {
      flashstate = LOW;      
    }
    digitalWrite(led, flashstate);
  }
}
