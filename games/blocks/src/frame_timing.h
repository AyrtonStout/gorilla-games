#ifndef HELLO_FRAME_TIMING_H
#define HELLO_FRAME_TIMING_H

const int FRAMES_TO_SLIDE = 4; // There is an additional frame after the slide before blocks "light up"
const int FRAMES_TO_DROP = 1; // How many frames it takes a block to go a full square when dropping
const int FRAMES_TO_START_FALLING = 10; // When you slide a block over an empty column, how long before it starts falling
const int FRAMES_OF_LIGHT = 44; // The block will alternate back and forth between lit and unlit every frame
const int FRAMES_OF_FACE = 25; // How long ALL blocks in a block group show a face before they pop
const int FRAME_PER_POP = 9; // How long after a block pops before the next block pops
const int FRAME_DROP_DELAY = 12; // How long after a pop before the blocks above them fall down

// When a block lands after a fall, it squishes, jumps up, then normalizes
const int FRAME_SQUISH = 3; // How long a block looks squished
const int FRAME_BOUNCE = 3; // How long a block image bounces up to the top
const int FRAME_REBOUND = 3; // This is between the "bounce" and the "normal" animations

#endif //HELLO_FRAME_TIMING_H
