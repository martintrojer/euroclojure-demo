
# Xively EuroClojure Demo

+---------+     +---------+     +---------+     +--------------+
| Twitter | --> | Clojure | --> | Clojure | --> | Agennt       |
|         |     | Heroku  |     | Xively  |     | Electric Imp |
+---------+     +---------+     +---------+     +-------+------+
                                                        |
  ------------------------------------------------------+
  |
+-+------------+     +---------+     +-+-+-+
| Device       | --> | C       | --> |x|o|x| LEDs
| Electric Imp |     | Adruino |     +-+-+-+
+--------------+     +---------+

## submodules

* Adafruit_Neopixel
Arduino library for the LED array

* hammer-time
iOS HTML app to simulate 'funpark hammers' (not used in the demo, but still cool)

## folders

* arduino
The arduino sources (gets data from IMP and controls the LED)

* heroku
Clojure app that counts tweets and sends data to Xively

* imp
Electic Imp agent and device code

* iOS
iOS sourcers
