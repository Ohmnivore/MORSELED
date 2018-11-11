# This script generates the pattern array declarations for our Arduino script.
# I tried doing this in the C++ code directly using macros and even explored constexpr.
# But I didn't manage to get anything decent, so here's a Python script.

# From https://en.wikipedia.org/wiki/Morse_code
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
OUT_LINE = 'const morse_pattern MORSE_LETTER_{0} = 0b{1:08b};' # Write pattern as zero-padded 8-bit binary
OUT_DOT = 'MORSE_DOT'
OUT_DASH = 'MORSE_DASH'

# Pack each letter into a byte, right-aligned with a leading 1 to denote the beginning of the pattern
# Dot = 0, dash = 1
for letter in alphabet:
    pattern = alphabet[letter]

    # Leading 1
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
