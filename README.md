# Nxamf - NX Automation Meta Firmware Library

Meta firmware for NX automation.

## Overview

With the Nintendo Switch (codename: NX) now equipped with the widely-used USB interface, several automated operation solutions using MCUs have come to light. The prevalent method currently involves two wiring paths: PC->MCU->NX and NX->HDMI Video Capture->PC. Through these paths, video feedback is obtained and processed on the PC side, after which operations are transmitted accordingly.

This library primarily focuses on the MCU aspect mentioned above. Regardless of the microcontroller or protocol used, its firmware will be abstracted to: receive byte arrays through communication, convert them into a data structure representing the next state of the gamepad, and reflect this to the NX.

## Usage

By implementing each method of `nxamf_buffer_interface_t`, a specific communication protocol is defined. Implementations for the [NX Macro Controller v2](https://blog.bzl-web.com/entry/2020/01/20/165719) and [Poke-Controller Modified](https://github.com/Moi-poke/Poke-Controller-Modified) are included as examples.

Below is a conceptual class diagram.

```mermaid
classDiagram
  class nxamf_buffer_interface_t
  <<interface>> nxamf_buffer_interface_t
  nxamf_buffer_interface_t : +append(uint8_t d) void
  nxamf_buffer_interface_t : +deserialize(nxamf_gamepad_state_t *out) bool
  nxamf_buffer_interface_t : +clear() void

  class nxmc2_buffer_t
  nxmc2_buffer_t ..|> nxamf_buffer_interface_t

  class pokecon_buffer_t
  pokecon_buffer_t ..|> nxamf_buffer_interface_t

  class nxamf_multi_buffer_manager_t
  nxamf_multi_buffer_manager_t : -nxamf_buffer_interface_t bufs[]
  nxamf_multi_buffer_manager_t : +get_last_deserialized_index() size_t

  nxamf_multi_buffer_manager_t ..|> nxamf_buffer_interface_t
  nxamf_buffer_interface_t ..* nxamf_multi_buffer_manager_t

  class nxamf_gamepad_state_t
  nxamf_gamepad_state_t : nxamf_button_t y
  nxamf_gamepad_state_t : nxamf_button_t b
  nxamf_gamepad_state_t : nxamf_button_t a
  nxamf_gamepad_state_t : nxamf_button_t x
  nxamf_gamepad_state_t : nxamf_button_t l
  nxamf_gamepad_state_t : nxamf_button_t r
  nxamf_gamepad_state_t : nxamf_button_t zl
  nxamf_gamepad_state_t : nxamf_button_t zr
  nxamf_gamepad_state_t : nxamf_button_t minus
  nxamf_gamepad_state_t : nxamf_button_t plus
  nxamf_gamepad_state_t : nxamf_button_t l_click
  nxamf_gamepad_state_t : nxamf_button_t r_click
  nxamf_gamepad_state_t : nxamf_button_t home
  nxamf_gamepad_state_t : nxamf_button_t capture
  nxamf_gamepad_state_t : nxamf_hat_t hat
  nxamf_gamepad_state_t : nxamf_stick_t l_stick
  nxamf_gamepad_state_t : nxamf_stick_t r_stick
  nxamf_gamepad_state_t : uint8_t extension[16]

  nxamf_buffer_interface_t <.. nxamf_gamepad_state_t
```

## Dependency

- [kokke/tiny-regex-c](https://github.com/kokke/tiny-regex-c) included (public domain).
