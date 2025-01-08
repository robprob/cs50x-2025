import re
import sys


def main():
    # Obtain user card
    card_num = obtain_card()

    # Perform Luhn's Algorithm to catch invalid checksum
    if luhns_algorithm(card_num) != 0:
        sys.exit("INVALID")

    # Validate issuer id against length of card, printing respective issuer
    validate_issuer(card_num)


def obtain_card():
    """Continually prompts user for card number as string, validating initial input"""
    while True:
        card_num = str(input("Number: "))
        if not valid_input(card_num):
            sys.exit("INVALID")
        else:
            return card_num


def valid_input(card_num):
    """Validate initial numeric input between 13 and 16 digits long"""
    regex = r'^\d{13,16}$'
    return re.match(regex, card_num)


def luhns_algorithm(card_num):
    """Perform Luhn's Algorithm on credit card number, returning checksum result"""
    checksum = 0
    # Step 1
    for i in range(len(card_num) - 2, -1, -2):
        product = int(card_num[i]) * 2
        for digit in str(product):
            checksum += int(digit)
    # Step 2
    for i in range(len(card_num) - 1, -1, -2):
        checksum += int(card_num[i])
    # Step 3
    return checksum % 10


def validate_issuer(card_num):
    """Validate issuer id against length of card, printing respective issuer if valid"""
    issuer_id = card_num[:2]
    card_length = len(card_num)
    if int(issuer_id) in [34, 37] and card_length == 15:
        print("AMEX")
    elif int(issuer_id) in range(51, 56) and card_length == 16:
        print("MASTERCARD")
    elif int(issuer_id[0]) == 4 and card_length in [13, 16]:
        print("VISA")
    else:
        print("INVALID")


if __name__ == "__main__":
    main()
