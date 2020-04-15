# 42

This is a simple **Arduino**-written Brainfuck interpreter. Supports liquid crystal **I2C** displays and any keypads 4x4.

It requires **LiquidCrystal_I2C** and **Keypad** libraries.

## Button mapping.

|   |   |   |   |
|-  |-  |-  |-  |
|`+`|`-`|`<`|`>`|
|`.`|`,`|`[`|`]`|
|`_`|`_`|`_`|`_`|
|`*`|`C`|`#`|`_`|

The `_` keys are silent ones - they have no use (as for now).

`-` `*` launches code.

`-` `C` resets **Arduino** board.

`-` `#` erases one character.
