#! /bin/bash

# Control the keyboard backlight brightess on Samsung Series 7 Chronos laptops

version=1.0.0


function usage {
	echo $'\nUSAGE:'
	echo $0 $'[options]\n'
	echo $'Turn keyboard backlight brightness up or down\n'
	echo $'Options:'
	echo $' -d, --down\t turn keybord brightness up'
	echo $' -h, --help\t display this help and exit'
	echo $' -s, --set=VALUE sets keyboard brightness to a value between 0 and 8'
	echo $' -u, --up\t turn keyboard brightness down'
	echo $' -v, --verbose\t more information on keyboard brightness when it runs'
	echo $' -V, --version\t print version and exit'
	
}


function echo_version {
	echo $0 $version
}


# adds command line options
args=$(getopt -o dhs:uvV -l down,help,set:,up,verbose,Version -n 'kbrightness.sh' -- "$@")
eval set -- "$args"


change=0


# parses command line options
while true; do
	case "$1" in
		-d|--down)
			change=-1 ; shift ;;
		-h|--help) 
            usage ; exit 0 ;;
		-s|--set)
			brightness=$2 ; shift 2 ;;
		-u|--up)
			change=1 ; shift ;;
		-v|--verbose) 
			verbose=true ; shift ;;
		-V|--version) 
			echo_version ; exit 0 ;;
		--) shift ; break ;;
        *) usage ; exit 1 ;;
    esac
done


# if brightness is already set with the -s/--set option, make
# sure it's within its limits, otherwise set it to min or max
if [ $brightness ]; then
	if [ $brightness -gt 8 ]; then
		brightness=8
	elif [ $brightness -lt 0 ]; then
		brightness=0
	fi
else	
	# reads current brightness and adds new value to it 
	brightness=$(cat /sys/class/leds/samsung\:\:kbd_backlight/brightness)
	brightness=$((brightness+change))
fi

if [ $brightness -ge 0 -a $brightness -le 8 ]; then
	# writes new value to brightness file
	echo $brightness > /sys/class/leds/samsung\:\:kbd_backlight/brightness
fi


if [ $verbose ]; then
	brightness=$(cat /sys/class/leds/samsung\:\:kbd_backlight/brightness)
	echo "Current keyboard backlight brightness:" $brightness
fi


exit 0

