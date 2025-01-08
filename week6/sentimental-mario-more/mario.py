def main():
    height = obtain_height()
    print_pyramids(height)


def obtain_height():
    """Obtain user input for half-pyramid height"""
    while (True):
        # Validate integer input
        try:
            height = int(input("Height: "))
        except ValueError:
            print("Please enter a valid integer height")
        else:
            # Ensure height within range 1-8, inclusive
            if height >= 1 and height <= 8:
                return height
            else:
                print("Height must be between 1 and 8, inclusive")


def print_pyramids(height):
    """Print two symmetrical pyramids of specified height"""
    for i in range(1, height + 1):
        print_blocks(height - i, i)


def print_blocks(spaces, blocks):
    """Print symmetrical spaces/blocks with a small gap in between"""
    for _ in range(spaces):
        print(" ", end="")
    for _ in range(blocks):
        print("#", end="")

    # Print spaces in-between
    print("  ", end="")

    for _ in range(blocks):
        print("#", end="")

    # Line break
    print()


if __name__ == "__main__":
    main()
