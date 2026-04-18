# Tennis for Two (1958) Recreation

A recreation of the world's first video game, *Tennis for Two*, built with C++ and raylib.

## Features

- Realistic physics with gravity and air resistance
- Dual-knob angle control (A/D for left player, ←/→ for right player)
- Authentic oscilloscope-style phosphor trail effect
- Blue CRT monitor aesthetic with grid background
- Turn-based hit system with proper serve rotation
- Net collision and ground bounce with energy loss

## Controls

| Player | Angle Knob | Hit |
|--------|------------|-----|
| Left   | A / D     | SPACE |
| Right  | ← / →     | ENTER |

## How to Play

1. The serving player (indicated on screen) adjusts their angle with the knob keys
2. Press the hit button to serve
3. The ball must clear the net
4. The opponent can hit the ball **only after it crosses to their side**
5. Score a point when the opponent fails to return or hits the net

## Build Requirements

- C++17 or later
- [raylib](https://www.raylib.com/) 5.0 or later

## Building

```bash
g++ -o tennis42 main.cpp -lraylib -lopengl32 -lgdi32 -lwinmm
```

## Credits

Original game created by William Higinbotham in 1958 at Brookhaven National Laboratory.

## License

MIT