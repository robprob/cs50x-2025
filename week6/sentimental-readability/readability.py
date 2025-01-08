def main():
    user_text = obtain_text()
    reading_index = coleman_liau(user_text)
    print_reading_level(reading_index)


def obtain_text():
    """Obtain input text passage from the user"""
    while True:
        user_text = input("Text: ")
        # Ensure text is not blank
        if user_text:
            return user_text
        else:
            print("Please provide some text")


def coleman_liau(user_text):
    """Calculate Coleman-Liau index using user text input"""
    # 0.0588 * L - 0.296 * S - 15.8
    # L = letters/100 words
    # S = sentences/100 words
    letter_count = 0
    word_count = 0
    sentence_count = 0

    # Iterate characters in text
    for char in user_text:
        if char.isalpha():
            letter_count += 1
        elif char == " ":
            word_count += 1
        elif char in [".", "!", "?"]:
            sentence_count += 1
    # Add last word to count
    word_count += 1
    # If single sentence with no ending punctuation, sentence count = 1
    if sentence_count == 0:
        sentence_count = 1

    # Calculate input values and resulting reading index
    colemanL = 100 * letter_count / word_count
    colemanS = 100 * sentence_count / word_count
    return 0.0588 * colemanL - 0.296 * colemanS - 15.8


def print_reading_level(reading_index):
    """Prints reading level given Coleman-Liau index as input"""
    if reading_index >= 16:
        print("Grade 16+")
    elif reading_index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {round(reading_index)}")


if __name__ == "__main__":
    main()
