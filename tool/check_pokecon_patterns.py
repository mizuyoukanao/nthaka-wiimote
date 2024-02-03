import re


def main():
    valid_pattern_str = "(\\d{1,4}|(0x)?\\d{4}) 0( (0x)?\\d{1,2} (0x)?\\d{1,2}( (0x)?\\d{1,2} (0x)?\\d{1,2})?)?\\r?\\n"
    valid_pattern = re.compile(valid_pattern_str)

    # "(\\d{1,4}|(0x)?\\d{4}) 0"
    #
    # "0 0"
    # "00 0"
    # "000 0"
    # "0000 0"
    # "0x0000 0"
    btns_hat = [3, 4, 5, 6, 8]

    # " (0x)?\\d{1,2}"

    # " 0"
    # " 00"
    # " 0x0"
    # " 0x00"
    single_stick = set([_0 + _1 for _0 in [2, 3, 4, 5] for _1 in [2, 3, 4, 5]])
    both_stick = set([_0 + _1 for _0 in single_stick for _1 in single_stick])
    sticks = set([0, *single_stick, *both_stick])

    # "\\n"
    # "\\r\\n"
    eol = [1, 2]

    valid_length = set([_0 + _1 + _2 for _0 in btns_hat for _1 in sticks for _2 in eol])

    print(f"Valid pattern: {valid_pattern_str}")
    print(f"Valid length: {valid_length}")
    print()

    while True:
        raw = input("Enter any valid sequence: ")
        s = raw.replace("\\r", "\r")
        s = s.replace("\\n", "\n")

        print(f'  input: "{raw}"')
        is_valid = re.match(valid_pattern, s) is not None
        print(f"  is_valid: {is_valid}")
        if is_valid:
            length = len(s)
            print(f"  length: {length} -> {length in valid_length}")
        print()


if __name__ == "__main__":
    main()
