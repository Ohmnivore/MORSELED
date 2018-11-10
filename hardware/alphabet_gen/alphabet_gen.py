# This script generates the pattern array declarations for our Arduino script
# I tried doing this in the script itself using macros and even explored constexpr
# Didn't manage to get anything decent, so here's a Python script

alphabet = {
    'A': '.-',
    'B': '-...',
    'C': '-.-.',
    'D': '-..',
    'E': '.',
    'F': '..-.',
    'G': '--.',
    'H': '....',
    'I': '..',
    'J': '.---',
    'K': '-.-',
    'L': '.-..',
    'M': '--',
    'N': '-.',
    'O': '---',
    'P': '.--.',
    'Q': '--.-',
    'R': '.-.',
    'S': '...',
    'T': '-',
    'U': '..-',
    'V': '...-',
    'W': '.--',
    'X': '-..-',
    'Y': '-.--',
    'Z': '--..',
    '0': '-----',
    '1': '.----',
    '2': '..---',
    '3': '...--',
    '4': '....-',
    '5': '.....',
    '6': '-....',
    '7': '--...',
    '8': '---..',
    '9': '----.',
}

CHAR_DOT = '.'
CHAR_DASH = '-'

# Config
OUT_LINE = 'const morse_pattern MORSE_LETTER_{0} = 0b{1:08b};'
OUT_DOT = 'MORSE_DOT'
OUT_DASH = 'MORSE_DASH'

for letter in alphabet:
    pattern = alphabet[letter]

    pattern_int = 1

    for char in pattern:
        pattern_int <<= 1

        if char == CHAR_DOT:
            pass
        elif char == CHAR_DASH:
            pattern_int += 1
        else:
            raise Exception('Unknown character {0} in letter {1}'.format(char, letter))

    statement = OUT_LINE.format(letter, pattern_int)
    print(statement)
