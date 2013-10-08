
# Xively EuroClojure Demo

```
+---------+     +---------+     +---------+     +--------------+
| Twitter | --> | Clojure | --> | Clojure | --> | Agent        |
|         |     | Heroku  |     | Xively  |     | Electric Imp |
+---------+     +---------+     +---------+     +-------+------+
                                                        |
  ------------------------------------------------------+
  |
+-+------------+     +---------+      LEDs
| Device       | --> | C       | --> +-+-+-+-+
| Electric Imp |     | Arduino |     |x|o|x|x|
+--------------+     +---------+     +-+-+-+-+
```

## folders

* arduino

The arduino sources (gets data from IMP and controls the LED)

* heroku

Clojure app that counts tweets and sends data to Xively

* imp

Electic Imp agent and device code

## submodules

* Adafruit_Neopixel

Arduino library for the LED array
