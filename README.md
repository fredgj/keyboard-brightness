Keyboard backlight brightness
=============================

This is a script that lets you controll the keyboard backlight brightness on 
Samsung Series 7 Chronos laptops. 

It's useful if you need to conigure the special keys on the keyboard (fn + fX) in order
to controll the keyboard backlight brightness.
In my case I use fn + f9 and fn + 10 for turning the backlight brightness up and down.
This can be done throught xbindkeys by adding the following lines to your .xbindkeysrcs file.

```
 "~/path/to/kbrightness.sh -d"    
      m:0x0 + c:237
 "~/path/to/kbrightness.sh -u"
      m:0x0 + c:238
```

Note that in order to make changes to the brightness file without being logged in as root, 
you need to alter the file permissions. The permissions need to be changed during 
initialization (systemd in most cases). This can be done by adding the following line to your /etc/rc.local file: chmod 666 /sys/class/leds/samsung\:\:kbd_backlight/brightness

If your system doesn't have a /etc/rc.local file either figure our how rc.local have been 
replaced or create a new rc.local file and a rc-local.service and add the service to systemd.
To do this you need to create a rc-local.service in /etc/systemd/system and add the following 
lines:
```
 [Unit]
 Description=/etc/rc.local Compatibility
 ConditionPathExists=/etc/rc.local
 
 [Service]
 Type=forking
 ExecStart=/etc/rc.local start
 TimeoutSec=0
 StandardOutput=tty
 RemainAfterExit=yes
 SysVStartPriority=99
 
 [Install]
 WantedBy=multi-user.target
```
Then run **sudo systemctl enable rc-local.service**, and now your keyboard backlight script 
should work.


Usage
=====

``` sh
$ ./kbrightness.sh -u       # turns the brightness up by 1 
$ ./kbrightness.sh -d        # turns the brightness down by 1 
$ ./kbrightness.sh -s 8     # sets the brightness to 8
```

