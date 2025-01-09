-- Keep a log of any SQL queries you execute as you solve the mystery.

-- July 28
-- Humphrey Street

-- Locate crime_scene_reports on date and place of crime
SELECT description
FROM crime_scene_reports
WHERE month = 7
AND day = 28
AND year = 2024
AND street LIKE '%Humphrey%';

/*
Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
*/

-- Read interview transcripts from the three witnesses
SELECT *
FROM interviews
WHERE month = 7
AND day = 28
AND year = 2024;

/*
Ruth, id 161
Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.

Eugene, id 162
I don't know the thief's name, but it was someone I recognized.
Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.

Raymond, id 163
As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
The thief then asked the person on the other end of the phone to purchase the flight ticket.
*/

-- Check bakery security logs within 10 minutes of 10:15am
CREATE TEMPORARY VIEW suspect_security_logs AS
SELECT *
FROM bakery_security_logs
WHERE month = 7
AND day = 28
AND year = 2024
AND hour = 10
AND minute BETWEEN 15 AND 25;

-- Check Leggett ATM logs and corresponding bank account info
CREATE TEMPORARY VIEW suspect_accounts AS
SELECT *
FROM bank_accounts
WHERE account_number IN (
    SELECT account_number
    FROM atm_transactions
    WHERE month = 7
    AND day = 28
    AND year = 2024
    AND atm_location LIKE '%Leggett%'
    AND transaction_type = 'withdraw'
);

-- Check flights out of Fiftyville the morning after
CREATE TEMPORARY VIEW suspect_flights AS
SELECT *
FROM flights
WHERE origin_airport_id IN (
    SELECT id
    FROM airports
    WHERE city LIKE '%Fifty%'
)
AND month = 7
AND day = 29
AND year = 2024
ORDER BY hour;
-- Earliest flight id = 36, destination airport 4
-- Locate desination
SELECT *
FROM airports
WHERE id IN (
    SELECT destination_airport_id
    FROM suspect_flights
);
-- Destination: New York City (LaGuardia Airport)

-- Check passenger info of suspect flight by matching passport number
CREATE TEMPORARY VIEW suspects AS
SELECT *
FROM people
WHERE passport_number IN (
    SELECT passport_number
    FROM passengers
    WHERE flight_id = (
        SELECT id
        FROM suspect_flights
    )
);

-- Narrow suspects by matching suspect license plates
CREATE TEMPORARY VIEW suspects_plate_matched AS
SELECT *
FROM suspects
WHERE license_plate IN (
    SELECT license_plate
    FROM suspect_security_logs
);

-- Further narrow suspects by matching ATM log info
CREATE TEMPORARY VIEW suspects_atm_matched AS
SELECT *
FROM suspects_plate_matched
WHERE id IN (
    SELECT person_id
    FROM suspect_accounts
);

-- Trace phone calls less than 1 minute
CREATE TEMPORARY VIEW suspect_calls AS
SELECT *
FROM phone_calls
WHERE month = 7
AND day = 28
AND year = 2024
AND duration <= 60;

-- Pull person info for suspect callers
SELECT id
FROM people
WHERE phone_number IN (
    SELECT caller
    FROM suspect_calls
);

-- Further narrow suspects by matching suspect caller number
CREATE TEMPORARY VIEW suspect_caller AS
SELECT *
FROM suspects_atm_matched
WHERE id IN (
    SELECT id
    FROM people
    WHERE phone_number IN (
        SELECT caller
        FROM suspect_calls
    )
);
-- Suspect: Bruce (686048)

-- Determine accomplice whom suspect called
CREATE TEMPORARY VIEW suspect_receiver AS
SELECT *
FROM people
WHERE phone_number = '(375) 555-8161';
-- Suspect Accomplice: Robin (864400)

SELECT *
FROM people
WHERE name IN ('Bruce', 'Luca');
