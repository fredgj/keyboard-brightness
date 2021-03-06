Keyboard backlight brightness
=============================

This is a script that lets you controll the keyboard backlight brightness on 
Samsung Series 7 Chronos laptops. 

It's useful if you need to configure the special keys on the keyboard (fn + fX) in order
to control the keyboard backlight brightness.
In my case I use Fn + f9 and Fn + 10 for turning the backlight brightness up and down.
This can be done through xbindkeys by adding the following lines to your .xbindkeysrc file.

```
 "~/path/to/kbrightness.sh -d"    
      m:0x0 + c:237
 "~/path/to/kbrightness.sh -u"
      m:0x0 + c:238
```

Note that in order to write to the the brightness file without being logged in as root you 
need to alter the file permissions. The permissions need to be changed during 
initialization (systemd in most cases). This can be done by adding the following line 
to your */etc/rc.local* file: **chmod 666 /sys/class/leds/samsung\:\:kbd_backlight/brightness**

If your system doesn't have a */etc/rc.local* file either figure out how *rc.local* has been 
replaced or create a new *rc.local* file and a *rc-local.service* and add the service to systemd.
To do this you need to create a new *rc-local.service* in */etc/systemd/system* and add the 
following lines:

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
Then run **sudo systemctl enable rc-local.service** to enable the service at bootup, and now 
your keyboard backlight script should work after the next reboot.


Usage
=====

``` sh
$ ./kbrightness.sh -u       # turns the brightness up by 1 
$ ./kbrightness.sh -d        # turns the brightness down by 1 
$ ./kbrightness.sh -s 8     # sets the brightness to 8
```

