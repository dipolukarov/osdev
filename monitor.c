/**
 * monitor.c
 * @version $Id$
 */

#include "monitor.h"

// The VGA framebuffer starts at 0xB8000
u16int *video_memory = (u16int *)0xB8000;
// Stores the cursor position.
u8int cursor_x = 0;
u8int cursor_y = 0;

// Updates the hardware cursor.
static void move_cursor()
{
	// The screen is 80 characters wide...
	u16int cursorLocation = cursor_y * 80 + cursor_x;
	outb(0x3D4, 14);			// Tell the VGA controller we are setting the high cursor byte.
	outb(0x3D5, cursorLocation >> 8);	// Send the high cursor byte.
	outb(0x3D4, 15);			// Tell the VGA controller we are setting the low cursor byte.
	outb(0x3D5, cursorLocation);		// Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
static void scroll()
{
	// Get a space character with the default color attributes.
	u8int attributeByte = (0 /* black */ << 4) | (15 /* white */ & 0x0F);
	u16int blank = 0x20 /* space */ | (attributeByte << 8);

	// Row 25 is the end, this means we need to scroll up
	if (cursor_y >= 25) {
		// Move the current text chunk that makes up the screen
		// black in the buffer by a line
		int i;
		for (i = 0 * 80; i < 24 * 80; i++) {
			video_memory[i] = video_memory[i + 80];
		}

		// The last line should now be blank. Do this by writing
		// 80 spaces to it.
		for (i = 24 * 80; i < 25 * 80; i++) {
			video_memory[i] = blank;
		}

		// The cursor should now be on the last line
		cursor_y = 24;
	}
}

// Writes a single character out to the screen.
void monitor_put(char c)
{
	// The background color is black (0), the foreground is white (15).
	u8int backColor = 0;
	u8int foreColor = 15;

	// The attribute byte is made up of two nibbles - the lower being the
	// foreground color, and the upper the background color.
	u8int attributeByte = (backColor << 4) | (foreColor & 0x0F);

	// The attribute byte is the top 8 bits of the word we have to send to the
	// VGA controller.
	u16int attribute = attributeByte << 8;
	u16int *location;

	// Handle a backspace, by moving the cursor back one space
	if (c == 0x08 && cursor_x) {
		cursor_x--;
	} else if (c == 0x09) { // Handle a tab by increasing the cursor's X,
		// but only to a point where it's divisible by 8
		cursor_x = (cursor_x + 8) &  ~(8 - 1);
	} else if (c == '\r') { // Handle carriage return
		cursor_x = 0;
	} else if (c == '\n') { // Handle newline by moving cursor back to left and increasing the row
		cursor_x = 0;
		cursor_y++;
	} else if (c >= ' ') { // Handle any other printable character.
		location = video_memory + (cursor_y * 80 + cursor_x);
		*location = c | attribute;
		cursor_x++;
	}

	// Check if we need to insert a new line because we have reached the end
	// of the screen
	if (cursor_x >= 80) {
		cursor_x = 0;
		cursor_y++;
	}

	// Scroll the screen if needed.
	scroll();
	// Move the hardware cursor.
	move_cursor();
}

// Clears the screen, by copying lots of spaces th the framebuffer.
void monitor_clear()
{
	// Make an attribute byte for the default colors
	u8int attributeByte = (0 /* black */ << 4) | (15 /* white */ & 0x0F);
	u16int blank = 0x20 /* space */ | (attributeByte << 8);

	int i;
	for (i = 0; i < 80 * 25; i++) {
		video_memory[i] = blank;
	}

	// Move the hardware cursor back to the start.
	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

// Outputs a null-terminated ASCII string to the monitor.
void monitor_write(char *c)
{
	int i = 0;
	while (c[i]) {
		monitor_put(c[i++]);
	}
}
