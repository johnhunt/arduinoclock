arduinoclock
============

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
