Nintendo 64 controller made using a due board. Controller inputs are calculated by hand at compile time, which are "repeated" at runtime. Basically, the user programs what the MCU should do, and the MCU repeats it.

The console-controller communication flow goes like:
1. Console sends command to controller (always 1 byte)
2. Controller responds to command (varies - sending what buttons are pressed is 4 bytes)

The protocol is similar to UART, but there are no start/stop bits between bytes. For clarity, the microcontroller receives data from, and sends data to, the console. It is acting as the controller and is wired directly to the console's controller port.

# RX
Receiving one byte from the console using UART is impossible because it lacks a stop bit. However, setting the receiver's baud rate high enough so that it samples one whole byte per console's bit does work. Determining if the console sent a 1 or a 0 is done by reading one of the bits in the middle of the received byte. At the moment, it only responds to one command correctly: the "poll" command (I do not have a game that uses any other command).

# TX
Transmitting is done via PWM with a 250kHz frequency, and it mimics the protocol by varying the duty cycle at the end of each period (the protocol resembles UART). The duty cycles are stored in an array of bytes, so sending a "1" or a "0" is a matter of changing its corresponding value in the array of duty cycles. Instead of turning the PWM off, the duty cycle is set to 0.

# Sending controller data (4 bytes)

The lower 16 bits of the sent data correspond to buttons. "1" means the button is pressed, "0" means it is not. Bits 16-23 store the analog stick's X-position (as a signed byte, with 0 being centered) and bits 24-31 store the the stick's Y-position (as a signed byte, with 0 being centered).


# Storing instructions

Controller data needs to be sent every time a new frame is displayed (30 times per second), so storing instructions for individual frames will take up a lot of space. Instead, instructions are stored as: {[buttons/stick data], [number of frames to repeat]}. Creating these instructions by hand is horribly tedious and time consuming (the signed bytes are sent LSB first, so after doing the trig to calculate X and Y given an angle, it needs to be reversed), so a helper program is used to create the instructions (https://github.com/17parker/due64_instruction_maker/).

# Other
The microcontroller displays what buttons are being "pressed," as well as how many instructions it has sent in total, on a screen. Commands and data are sent to the display controller (ILI9341) using direct memory access (DMA). This requires either an array or linked list, with a few *hundred* items. I opted for linked list, because large arrays really bog down Visual Studio.

A dot on an 8x8 LED dot matrix is moved after every instruction sent. The matrix display's controller uses SPI, and this was only included to demonstrate SPI with DMA.

The instructions currently programmed in collect a "power star" in the game "Super Mario 64". Upon powering the console up, the MCU navigates the menu, starts a new game, and navigates through the level.
