# Non-Arduino implementation of RadioLib for STM32H5

Untested for other STM32 boards, but as long as the HAL syntax remains consistent, it should be no problem.
Tested only RFM69, but I don't see why it wouldn't work for other SPI modules supported by RadioLib.

---

## Limitations

* Requires some configuration in CubeMX:

  * SPI is to be configured in CubeMX and the SPI handle/peripheral is to be intialized in `main.c` before being passed to the RadioLib HAL.
  * External interrupts (GPIO interrupts) are **NOT** to be configured in CubeMX, and that includes different pins that may trigger the same EXTI.

    * Meaning: for example, if PA13 is to be used as external interrupt (DIO0), CubeMX should **NOT** have PA13, PB13, PC13... etc configured as a GPIO_EXTI.
* Supports only some pins for GPIO functionality currently.

  * The list of supported pins are given in:
    `/Core/Inc/STM32H5_PinMap.h`
* Does not support `tone()` and `noTone()`.

---

## Implementation and Usage

### `/Core/Inc` contains the following headers:

| File                      | Description                                                                                                           |
| ------------------------- | --------------------------------------------------------------------------------------------------------------------- |
| `STM32H5_EXTI_Handlers.h` | External interrupt handlers for each supported GPIO pin. Used to support `attachInterrupt()` and `detachInterrupt()`. |
| `STM32H5_PinMap.h`        | Map of pin indices to GPIO ports and pin numbers.                                                                     |
| `STM32H5_MyHal.h`         | Implementation of RadioLib HAL functions that may be useful for user code as well.                                    |
| `STM32H5_RadioLibHal.h`   | Implementation of `STM32H5_RadioLibHal`, inheriting from the `RadioLibHal` class.                                     |
| `radiolib_bridge.h`       | Definitions used in the `radiolib_bridge.cpp` file.                                                                   |

---

### `/Core/Src` contains:

| File                  | Description                                                                                                                                                                                |
| --------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `radiolib_bridge.cpp` | Since `main.c` is not a C++ file, it cannot use the classes defined by RadioLib. To get around this, this file acts as a C++ bridge that takes care of all RadioLib functions and classes. |

---

## Changes to root `CMakeLists.txt`

* Added C++ standard selection
* Enabled support for CXX language
* Added RadioLib subdirectory and linked library
* Added `radiolib_bridge.cpp` to the executable source list

