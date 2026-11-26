#include "../drivers/frame_buffer.h"
#include "../drivers/interrupts.h"
#include "../drivers/hardware_interrupt_enabler.h"

/* sum of three function from chap 3 (add three num) */
int sum_of_three(int arg1, int arg2, int arg3)
{
    return arg1 + arg2 + arg3;
}

/* mult 2 num */
int multiply_two(int a, int b)
{
    return a * b;
}

/* sub 2 num */
int subtract_two(int a, int b)
{
    return a - b;
}

/* checks if two null-terminated strings are completely identical (returns 1 if equal, 0 if not) */
int str_equal(const char *s1, const char *s2)
{
    int i = 0;

    /* compare characters one by one until either string ends */
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) {
            return 0;
        }
        i++;
    }

    /* return 1 only if both strings reached '\0' at the exact same length */
    return (s1[i] == '\0' && s2[i] == '\0');
}

/* checks if string 'str' starts with 'prefix' (used for commands with arguments like "echo ") */
int starts_with(const char *str, const char *prefix)
{
    int i = 0;

    /* compare every character of prefix against the start of str */
    while (prefix[i] != '\0') {
        if (str[i] != prefix[i]) {
            return 0;
        }
        i++;
    }

    return 1;
}

/* c entry point called by loader.asm */
int kmain(void)
{
    int sum = sum_of_three(1, 2, 3);        /* 1 + 2 + 3 = 6 */
    int product = multiply_two(sum, 5);     /* 6 * 5 = 30 */
    int result = subtract_two(product, 4);  /* 30 - 4 = 26 */

    /* clear screen to black background and set cursor to origin coordinates */
    fb_set_color(FB_WHITE, FB_BLACK);
    fb_clear();

    /* print green welcome banner to top left */
    fb_set_color(FB_LIGHT_GREEN, FB_BLACK);
    fb_write_string("Hello! Welcome to my custom 32-bit Operating System...\n");

    /* move to column 4 and row 3 and print txt with a blue background */
    fb_move(4, 3);
    fb_set_color(FB_WHITE, FB_BLUE);
    fb_write_string(" 2D framebuffer driver active at (col 4, row 3) ");

    /* move to col 4 and row 5 and print calc results */
    fb_move(4, 5);
    fb_set_color(FB_LIGHT_CYAN, FB_BLACK);
    fb_write_string("result of the sum_of_three(1, 2, 3) calculation is = ");
    fb_set_color(FB_LIGHT_BROWN, FB_BLACK);
    fb_write_int(sum);

    fb_move(4, 6);
    fb_set_color(FB_LIGHT_CYAN, FB_BLACK);
    fb_write_string("final chained result is  = ");
    fb_set_color(FB_LIGHT_BROWN, FB_BLACK);
    fb_write_int(result);

    /* place blinking cursor neatly below the output */
    fb_move(0, 8);

    /* set up the IDT table, remap the PIC, and unmask keyboard IRQ1 */
    interrupts_install_idt();

    /* turn on CPU hardware interrupts so keypresses trigger interrupt 33 */
    enable_hardware_interrupts();

    /* 2. PASTE SHELL LOOP HERE (replacing the old test while loop) */
    /* buffer to hold each command entered by the user */
    char command[128];

    /* buffer to hold test text for the Task 2 readline demonstration */
    char test_line[128];

    /* print a hint showing the user how to list available commands */
    fb_set_color(FB_LIGHT_CYAN, FB_BLACK);
    fb_write_string("Type 'help' to see available shell commands.\n\n");

    /* main shell loop: display prompt, read command, and execute matching action */
    while (1) {
        /* print the shell prompt in light green */
        fb_set_color(FB_LIGHT_GREEN, FB_BLACK);
        fb_write_string("myos> ");
        fb_set_color(FB_WHITE, FB_BLACK);

        /* wait for the user to type a command and press Enter (uses Task 1 interrupts + Task 2 readline) */
        readline(command, 128);

        /* ignore empty lines when the user just presses Enter */
        if (command[0] == '\0') {
            continue;
        }
        /* help command: list all supported commands */
        else if (str_equal(command, "help")) {
            fb_set_color(FB_LIGHT_CYAN, FB_BLACK);
            fb_write_string("Available commands:\n");
            fb_set_color(FB_WHITE, FB_BLACK);
            fb_write_string("  help         - Show this help message\n");
            fb_write_string("  test         - Run Task 2 readline() input echo test\n");
            fb_write_string("  echo <text>  - Print <text> back to the screen\n");
            fb_write_string("  clear        - Clear the terminal screen\n");
            fb_write_string("  version      - Display OS version information\n");
        }
        /* test command: runs the Task 2 readline() prompt and echoes typed text back */
        else if (str_equal(command, "test")) {
            fb_set_color(FB_LIGHT_GREEN, FB_BLACK);
            fb_write_string("Please enter test text: ");
            fb_set_color(FB_WHITE, FB_BLACK);

            readline(test_line, 128);

            fb_set_color(FB_LIGHT_CYAN, FB_BLACK);
            fb_write_string("Thank you. You have typed: ");
            fb_set_color(FB_WHITE, FB_BLACK);
            fb_write_string(test_line);
            fb_newline();
        }
        /* clear command: wipe the screen and reset cursor to top-left (0, 0) */
        else if (str_equal(command, "clear")) {
            fb_clear();
        }
        /* version command: print kernel build info */
        else if (str_equal(command, "version")) {
            fb_set_color(FB_LIGHT_BROWN, FB_BLACK);
            fb_write_string("TinyOS v1.0 (32-bit x86 Bare-Metal Kernel)\n");
            fb_set_color(FB_WHITE, FB_BLACK);
        }
        /* echo command: print everything typed after "echo " (starting at index 5) */
        else if (starts_with(command, "echo ")) {
            fb_write_string(command + 5);
            fb_newline();
        }
        /* bare "echo" with no arguments: print a blank line */
        else if (str_equal(command, "echo")) {
            fb_newline();
        }
        /* unknown command fallback */
        else {
            fb_set_color(FB_LIGHT_BROWN, FB_BLACK);
            fb_write_string("Unknown command: ");
            fb_write_string(command);
            fb_write_string(" (type 'help' for commands)\n");
            fb_set_color(FB_WHITE, FB_BLACK);
        }
    }

    return result;
}