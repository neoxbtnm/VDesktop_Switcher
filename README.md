# VDesktop_Switcher
Легковесная (~1mb озу) cpp реализация VirtualDesktopSwitcher изначально написанной на ahk с использованием rust библиотеки 

Основной репозиторий библиотеки virtual-desktop-accessor.dll
(должен быть в папке с .exe программы или в env PATH)

Для windows 10 самый первый релиз!
https://github.com/Ciantic/VirtualDesktopAccessor

Начиная с версии 0.6a можно изменять комбинации клавиш в конфигурационном файле, на данный момент реализована поддержка простых буквенных клавиш в разделе key для некоторых действий и возможность выбирать несколько модификаторов (ALT, CTRL, SHIFT и WIN) получая из них комбинации. Для корректной работы программы рекомендую строго придерживаться формы из дефолтного конфига (модификаторы пропысывать соблюдая json, не забывайте ставить запятые если используете несколько модификаторов)

```
{
"close window": {
        "enabled": 1,
        "key": "Q",
        "modifiers": [
            "ALT",
            "SHIFT"
        ]
    },
    "swap desktops": {
        "enabled": 1,
        "modifiers": [
            "ALT"
        ]
    },
    "swap window to desktop": {
        "enabled": 1,
        "modifiers": [
            "ALT",
            "SHIFT"
        ]
    },
    "terminate window": {
        "enabled": 0,
        "key": "Q",
        "modifiers": [
            "ALT",
            "CTRL",
            "SHIFT"
        ]
    }
}
```

**Поддерживаемые модификаторы:**
```
"ALT"
"SHIFT"
"CTRL"
"WIN"
```

# VDesktop_Switcher ENG
Lightweight (~1MB RAM) CPP implementation of VirtualDesktopSwitcher, originally written in ahk using the Rust library.

Main library repository: virtual-desktop-accessor.dll
(Must be in the program's .exe folder or in the environment variable PATH)

First release for Windows 10!
https://github.com/Ciantic/VirtualDesktopAccessor

Starting with version 0.6a, you can change key combinations in the configuration file. Currently, support for simple letter keys in the key section is implemented for some actions, as well as the ability to select multiple modifiers (ALT, CTRL, SHIFT, and WIN) to create combinations. For proper operation, I recommend strictly adhering to the default configuration (specify modifiers using JSON, and remember to include commas when using multiple modifiers).

```
{
"close window": {
        "enabled": 1,
        "key": "Q",
        "modifiers": [
            "ALT",
            "SHIFT"
        ]
    },
    "swap desktops": {
        "enabled": 1,
        "modifiers": [
            "ALT"
        ]
    },
    "swap window to desktop": {
        "enabled": 1,
        "modifiers": [
            "ALT",
            "SHIFT"
        ]
    },
    "terminate window": {
        "enabled": 0,
        "key": "Q",
        "modifiers": [
            "ALT",
            "CTRL",
            "SHIFT"
        ]
    }
}
```
**Supported modifiers:**
```
"ALT"
"SHIFT"
"CTRL"
"WIN"
```
