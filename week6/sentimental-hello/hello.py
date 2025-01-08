# Continually prompt user for name input
while (True):
    # Obtain user input and remove blank space
    user_name = input("What is your name? ").strip()

    # Check if empty
    if not user_name:
        print("Please enter a valid name\n")
    else:
        break

# Greet user
print(f"hello, {user_name}")
