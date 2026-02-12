# ESPHome component to monitor and control KingSong EUCs

## Denial of Responsibility

**Disclaimer:** Messing up with parameters could lead to EUC failure. Please make sure you understand what you are doing. This project is provided "as-is" without any warranty or support of any kind. By using this software, you agree that:

1. **No Warranty:** The project is provided without any warranty, expressed or implied. The entire risk of using the project is assumed by you, the user.
2. **No Support:** The author(s) of this project are under no obligation to provide support, updates, or maintenance. Issues and pull requests may be addressed at the sole discretion of the project contributors.
3. **Use at Your Own Risk:** The project may involve experimental features or third-party dependencies, and the user is responsible for ensuring its compatibility with their system. The author(s) are not responsible for any damage, data loss, or other consequences that may arise from the use of this project.
4. **No Guarantee of Compatibility:** The project may not be compatible with all environments, hardware, or software configurations. It is the user's responsibility to verify compatibility before use.
5. **Security Considerations:** The project may not have undergone a comprehensive security review. Users are advised to exercise caution and not use this project in critical or security-sensitive applications.
6. **Third-Party Dependencies:** This project may rely on third-party libraries, frameworks, or tools. The user is responsible for complying with the licenses and terms associated with these dependencies.

By using this project, you acknowledge and agree to these terms. If you do not agree with these terms, do not use or contribute to this project.

## Important note

This project is still in progress, there are some minor issues and bugs. Right now I was only able to test it with my S22 Pro Plus EUC, witch has only part of exposed features this component capable.

Make sure you are not overloading your configuration with redundant entities as it can lead to lack of memory, especially if you add additional components to your project. You can use [`free`](https://esphome.io/components/debug.html#sensor) sensor of `debug` component to check free memory. I recommend to disable components like `ota`, `captive_portal`, `logger`, etc on production.

> Please note that for some reason my setup (ESP32-C3 super mini) doesn't connect with EUC directly. I'm using BT proxy feature of [eucWatch](https://github.com/enaon/eucWatch). I believe this feature is also available in some custom EUC apps. I'm trying to solve this issue, maybe that is due to hardware that I use, I already ordered another ESP32 dev board to check.
>
> If you have solution or further investigation on this, please create PR or contact me.
>
> Upd: actually it occasionally works with ESP32-S3. Sometimes it connects and can get notfication with state of entities that requires separate request, but does not receive notifications for other entities state change.

### Minimal configuration

```yaml
external_components:
  source: github://lufton/esphome-kingsong-euc@main

ble_client:
  - id: euc_client
    mac_address: 00:00:00:00:00:00

kingsong_euc:
  ble_client_id: euc_client

sensor:
  - platform: kingsong_euc
    current:
      name: Current
    odometer:
      name: Odometer
    speed:
      name: Speed
    voltage:
      name: Voltage
    ...
```

## Entity parameters

### report_interval

`report_interval` entity parameter defines how often component is going to report entity's state downstream even if state has not changed. Changing entity's state immediately reports new state downstream (see `hysteresis`). `Defaults to: 10s`.

```yaml
sensor:
  - platform: kingsong_euc
    uptime:
      name: Uptime
      report_interval: 1m
```

### update_interval

`update_interval` entity parameter defines how often component is going to poll entity's state from EUC. This parameter is only relevant to entities that receive their state via response for special state request that is performed once when EUC is connected and then every `update_interval`. Note that every time state for such entities is requested EUC will do a beep. So when EUC is connected you may here these beep several times depending on number of entities. `Defaults to: never`.

```yaml
sensor:
  - platform: kingsong_euc
    uptime:
      name: Uptime
      hysteresis: 60
      report_interval: 1m
```

### hysteresis

`hysteresis` sensor parameter defines how much new value should be different for the new state to be reported downstream. This parameter is helpful for entities that change their state to often and therefor generate noise. Change that is smaller than `hysteresis` is ignored and does not trigger actual state change. `Defaults to: 4294967295`.

## Available entities

### Binary sensors

| ID | Description | Requestable |
| - | - | - |
| `charging` | whether UEC is currently charging (see `charging_status` text sensor) | no |
| `fan` | whether fan is currently working | no |
| `gyroscope_error` | whether there is a gyroscope error | no |
| `hall_sensor_error` | whether there is a hall sensor error | no |
| `phase_short_circuit` | whether there is phase short circuit detected | no |

### Buttons

| ID | Description |
| - | - |
| `beep` | beeps |
| `horn` | horns |
| `power_off` | powers off EUC |

### Locks

| ID | Description | Requestable |
| - | - | - |
| `lock` | EUC lock | yes |

### Numbers

| ID | Description | Min | Max | Unit | Requestable |
| - | - | - | - | - | - |
| `alarm_1` | speed for first alarm beep | 0 | 70 | km/h | yes |
| `alarm_2` | speed for second alarm beep | 0 | 70 | km/h | yes |
| `alarm_3` | speed for third alarm beep | 0 | 70 | km/h | yes |
| `command` | manual command request | 0 | 255 | N/A | yes |
| `standby_delay` | delay before EUC turns off after being inactive | 0 | 14400 | sec | yes |
| `tilt_back` | speed for tilt back | 0 | 70 | km/h | yes |

### Selects

| ID | Description | Options | Requestable |
| - | - | - | - |
| `magic_light_mode` | magic light mode | `Annular 1`, `Annular 2`, `Annular 3`, `Annular 4` | yes |
| `main_light_mode` | main light mode | `On`, `Off`, `Auto` | no |
| `ride_mode` | ride mode | `Hard`, `Medium`, `Soft` | no |
| `spectrum_light_mode` | spectrum light mode | `Automatic`, `Beating`, `Flashing`, `Alternate` | yes |
| `voice_language` | voice language | `English`, `Chinese` | yes |

### Sensors

| ID | Description | Units | Requestable |
| - | - | - | - |
| `bms_N_cell_M_voltage` | voltage of M-th cell of N-th BMS | V | no |
| `bms_N_current` | current of N-th BMS | A | no |
| `bms_N_factory_capacity` | factory battery capacity of N-th BMS | mAh | no |
| `bms_N_full_cycles` | number of full charging cycles of N-th BMS | N/A | no |
| `bms_N_mosfet_temperature` | MOSFET temperature of N-th BMS | °C | no |
| `bms_N_remaining_capacity` | remaining battery capacity of N-th BMS | mAh | no |
| `bms_N_soc` | state of charge of N-th BMS, calculated as `bms_N_remaining_capacity` / `bms_N_factory_capacity` * 100 | % | no |
| `bms_N_temperature_1` | value of temperature sensor 1 of N-th BMS | °C | no |
| `bms_N_temperature_2` | value of temperature sensor 2 of N-th BMS | °C | no |
| `bms_N_temperature_3` | value of temperature sensor 3 of N-th BMS | °C | no |
| `bms_N_temperature_4` | value of temperature sensor 4 of N-th BMS | °C | no |
| `bms_N_temperature_5` | value of temperature sensor 5 of N-th BMS | °C | no |
| `bms_N_temperature_6` | value of temperature sensor 6 of N-th BMS | °C | no |
| `bms_N_voltage` | voltage of N-th BMS | V | no |
| `cpu_load` | CPU load | % | no |
| `error_code` | error code (see `error_description` text sensor) | N/A | no |
| `current` | discharging current (negative when charging) | A | no |
| `mosfet_temperature` | MOSFET temperature | °C | no |
| `motor_temperature` | motor temperature | °C | no |
| `odometer` | odometer (total distance) | km | no |
| `power` | discharging power, calculated as `voltage` * `current` (negative when charging) | W | no |
| `pwm` | PWM | % | no |
| `ride_time` | total ride time | sec | no |
| `speed` | speed | km/h | no |
| `speed_limit` | current speed limit | km/h | no |
| `trip_distance` | current trip distance | km | no |
| `trip_max_speed` | current trip maximum speed | km/h | no |
| `uptime` | time since EUC was turned on | sec | no |
| `voltage` | battery voltage | V | no |

### Switches

| ID | Description | Requestable |
| - | - | - |
| `circle_light` | circle light switch | yes |
| `lift_sensor` | lift sensor switch | yes |
| `music_bluetooth` | music bluetooth switch | yes |
| `spectrum_light` | spectrum light switch | yes |
| `strobe` | strobe switch | yes |
| `voice` | voice switch | yes |

### Text sensors

| ID | Description | Requestable |
| - | - | - |
| `bms_N_firmware` | firmware version of N-th BMS | yes |
| `bms_N_manufacture_date` | manufacture date of N-th BMS | yes |
| `bms_N_serial_number` | serial number of N-th BMS | yes |
| `charging_status` | charging status (`Not charging`, `Charging`, `Discharged`, `Overcharged (charger)`, `Overcharged (regeneration)` | no |
| `error_description` | error description (taken from https://support.euco.us/article/319-kingsong-error-codes) | no |
| `model` | EUC model | yes |
| `serial_number` | UEC serial number | yes |

## Contribution

Please feel free to contribute. If you have improvement ideas, bug-fixes or even confirmation that this project worked for you, please create relevant issue.

> I spent almost a month developing just a proof of concept of this project, if you appreciate my work please consider [donating or subscribe via PayPal](https://www.paypal.com/donate/?hosted_button_id=RULD2P56VXB4W).
