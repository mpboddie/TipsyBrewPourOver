# TipsyBrew Pour Over Robot (POR) MkII

### NOTE: Until I say otherwise, MkII is a work in progress. If you start purchasing or printing parts to work along with me, I welcome you. However, until hardware is finalized I can't guarantee that money or effort won't be wasted.

This code is used on an Arduino MEGA 2560 with integrated ESP8266, like this one https://robotdyn.com/mega-wifi-r3-atmega2560-esp8266-flash-32mb-usb-ttl-ch340g-micro-usb.html.

## Preheat Feature
I see this feature being used when you want to start the heater remotely so the robot is closer to a cup of coffee once you get to putting grounds in. Once the preheat feature starts it will heat up to the temp in settings which is typically around 80&#8451;. The optimal brew temp is in the range of 90-96&#8451;, so you want to be sure the preheat is just a bit lower than that.

PID control was not used for Preheat because the kettle is controlled with it's native lever depression control. Switching it on and off with high frequency would just wear it out quickly and make a bunch of noise.

Feedback is provided by a DS18B20 temperature sensor that I put in a stainless steel thermowell just below the minimum fill line of the kettle.

Preheat has a few safety features as well. First, the mechanism that presses the lever always goes to a neutral position allowing the kettle to spring to the off position if it needs to. Second, the preheat function will only run for a limited set of time, which helps prevent a dry boil if someone were to start the preheat and forget about it (Olle knows who he is). Third, if the temperature continues to raise when the kettle is off or drop when the kettle is on for a period of time defined in settings, then the preheat feature will throw an error and stop.
