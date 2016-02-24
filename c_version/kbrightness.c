#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/* Control the keyboard backlight on Samsung Series 7 Chronos laptops */


const char *BRIGHTNESS_PATH = "/sys/class/leds/samsung::kbd_backlight/brightness";
const char *VERSION = "1.0.0";


enum {
    SUCCESS,
    SUCCESS_E,
    FAILURE,
    MIN_BRIGHTNESS=48,
    MAX_BRIGHTNESS=56,
};


void usage(char *name)
{
	printf("\nUSAGE:\n");
	printf(" %s [options]\n\n", name);
	printf("Turn keyboard backlight brightness up or down\n\n");
	printf("Options\n");
	printf(" -d, --down\t turn the keyboard brightness up\n");
	printf(" -h, --help\t display this help and exit\n");
	printf(" -s, --set=VALUE set keyboard brightness to a value between 0 and 8\n");
	printf(" -u, --up\t turn the keyboard brightness up\n");
	printf(" -v, --verbose\t more information on keyboard brightness when it runs\n");
	printf(" -V, --version\t print version and exit\n");
}


/* Parse command line arguments. Returns SUCCESS if everything went smooth,
 * SUCCESS_E if everything was ok but can exit (help or version) or FAILURE if
 * no arguments or an invalid argument were given */
int parse_args(int argc, char *argv[], char **vars)
{ 
    int opt;
    char *name = argv[0];
    char *shortopts = "dhs:uvV";
    char *br = vars[0];
    char *br_set = vars[1];
    char *change = vars[2];
    char *verbose = vars[3];

    static struct option longopts[] = 
    {
        {"down",    no_argument,       0,   'd'},
        {"up",      no_argument,       0,   'u'},
        {"verbose", no_argument,       0,   'v'},
        {"help",    no_argument,       0,   'h'},
        {"set",     required_argument, 0,   's'},
        {"version", no_argument,       0,   'V'},
    };

    /* return if no arguments given */
    if (argc == 1) {
        usage(name);
        return FAILURE;
    }

    while ((opt = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
        switch(opt) {
        case 'd':
            *change = -1;
            break;
        case 'h':
            usage(name);
            return SUCCESS_E;
        case 's':
            *br = atoi(optarg)+MIN_BRIGHTNESS;
            *br_set = 1;
            break;
        case 'u':
            *change = 1;
            break;
        case 'v':
            *verbose = 1;
            break;
        case 'V':
            printf("%s %s\n", name, VERSION);
            return SUCCESS_E;
        default:
            /* invalid arg */
            usage(name);
            return FAILURE;
        }
    }

    return SUCCESS;
}


int main(int argc, char *argv[])
{
    FILE *fp;
    char brightness = 0;
    char brightness_set = 0;
    char change = 0;
    char verbose = 0;
    char *vars[4] = {&brightness, &brightness_set, &change, &verbose};
    int parse;

    parse = parse_args(argc, argv, vars);

    if (parse == FAILURE)
        return EXIT_FAILURE;
    else if (parse == SUCCESS_E)
        return EXIT_SUCCESS;

    /* open brightness file, exit if failure */
    if ((fp = fopen(BRIGHTNESS_PATH, "r+")) == NULL) {
        fprintf(stderr, "Error: %s: %s\n", BRIGHTNESS_PATH, strerror(errno));
        return EXIT_FAILURE;
    }

    if (change) {
        /* reads brightness from file, exit if failure */
        if ((brightness = fgetc(fp)) == EOF)
            return EXIT_FAILURE;

        brightness += change;
    } else if (brightness_set) {
        /* Brightness is set, makes sure it's within limits */
        if (brightness > MAX_BRIGHTNESS)
            brightness = MAX_BRIGHTNESS;
        else if (brightness < MIN_BRIGHTNESS)
            brightness = MIN_BRIGHTNESS;
    }

    if (brightness >= MIN_BRIGHTNESS && brightness <= MAX_BRIGHTNESS) {
        /* resets file position to 0 so old brightness value can
         * be overwritten */
        if (fseek(fp, 0, SEEK_SET) == -1)
            return EXIT_FAILURE;

        /* writes the new value to brightness file, exit if failure */
        if (fputc(brightness, fp) == EOF)
            return EXIT_FAILURE;
    }

    if (verbose) {
        /* resets file position back to 0  */
        if (fseek(fp, 0, SEEK_SET) == -1)
            return EXIT_FAILURE;

        /* Have to wait until everything is up to date after fseek */
        usleep(1);

        /* reads brightness from file, exit if failure */
        if ((brightness = fgetc(fp)) == EOF)
            return EXIT_FAILURE;

        printf("Current keyboard backlight brightness: %d\n", brightness-48);
    }

    if (fclose(fp) == EOF)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

