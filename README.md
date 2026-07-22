# Arduino — Embedded Systems Roadmap

Structured Arduino projects and exercises built as Stage 3 of a self-directed 
embedded systems roadmap. Target stack: STM32/ARM Cortex-M, FreeRTOS, bare-metal 
firmware engineering.

Currently progressing toward STM32 (Stage 4) covering register-level GPIO, 
timers, DMA, and communication protocols.

---

## Structure

| Folder | Contents |
|--------|----------|
| [Fundamentals/](./Fundamentals/) | Foundational exercises — FSM LED blink, button debounce, PWM dimmer |
| [Projects/](./Projects/) | Real projects — sensor logging |

---

## Roadmap Progress

- [x] Fundamentals — FSM pattern, non-blocking timing, PWM
- [x] Temperature & humidity logger
- [x] Ultrasonic distance sensor with I2C display
- [x] PIR motion alarm with FSM
- [ ] Mini CSV data logger
- [ ] STM32 (Stage 4) — coming soon

---

## Environment

- **Platform:** Arduino Uno R3
- **IDE:** PlatformIO (VS Code)
- **OS:** Linux (Fedora)
