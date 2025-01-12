""" Original program framework provided by CS50"""

import os
import json

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Load in NASDAQ stock symbols
"""Provided by rreichel3 via Github (https://github.com/rreichel3/US-Stock-Symbols)"""
with open("static/nasdaq_full_tickers.json", "r") as f:
    nasdaq_data = json.load(f)


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    purchase_summary = db.execute(
        "SELECT symbol, SUM(quantity) as quantity FROM purchases WHERE user_id = ? GROUP BY symbol", user_id)

    # Build portfolio summary
    portfolio = []
    portfolio_value = 0

    for entry in purchase_summary:
        new_summary = dict()

        symbol = entry["symbol"]

        new_summary["symbol"] = symbol
        new_summary["quantity"] = entry["quantity"]
        # Fetch current price, formatting for summary
        price = lookup(symbol)["price"]
        new_summary["price"] = usd(price)
        # Calculate stock total value and add to portfolio value
        stock_value = float(price) * new_summary["quantity"]
        new_summary["total_value"] = usd(stock_value)
        portfolio_value += stock_value

        # Add entry to portfolio summary
        portfolio.append(new_summary)

    # Retrieve current user cash balance
    user_balance = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]["cash"]

    # Calculate total holdings
    total_holdings = user_balance + portfolio_value

    # Display table summary of portfolio
    return render_template("index.html", portfolio=portfolio, user_balance=usd(user_balance), portfolio_value=usd(portfolio_value), total_holdings=usd(total_holdings))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Parse and validate user/form information
        user_id = session["user_id"]

        symbol = request.form.get("symbol")
        stock = lookup(symbol)
        if not stock:
            return apology("invalid symbol")

        price = stock["price"]

        try:
            quantity = int(request.form.get("shares"))
        except:
            return apology("share quantity must be a positive whole number")
        if not isinstance(quantity, int) or quantity <= 0:
            return apology("share quantity must be a positive whole number")

        # Calculate total price
        total_price = price * quantity

        # Retrieve current user cash balance
        user_balance = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]["cash"]

        # Ensure necessary balance on hand
        if user_balance < total_price:
            return apology(f"inadequate cash balance: {usd(user_balance)}\ntotal price: {usd(total_price)}")

        try:
            # Purchase stocks, updating user balance
            db.execute("UPDATE users SET cash = ? WHERE id = ?",
                       user_balance - total_price, user_id)

            # Add purchase to log
            db.execute("INSERT INTO purchases (user_id, symbol, price, quantity) VALUES(?, ?, ?, ?)",
                       user_id, symbol, price, quantity)
        except:
            return apology("error updating purchase database")

        # Return back to homepage
        return redirect("/")
    else:
        return render_template("buy.html", stocks=nasdaq_data)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]

    # Retrieve user transaction history
    transactions = db.execute(
        "SELECT * FROM purchases WHERE user_id = ? ORDER BY timestamp DESC", user_id)

    # Build formatted history
    user_history = []

    for entry in transactions:
        new_entry = dict()
        new_entry["timestamp"] = entry["timestamp"]
        new_entry["symbol"] = entry["symbol"]
        price = entry["price"]
        new_entry["price"] = usd(price)
        quantity = entry["quantity"]
        new_entry["quantity"] = quantity
        new_entry["balance_change"] = usd(price * quantity * -1)
        user_history.append(new_entry)

    # Pass data into rendered HTML
    return render_template("history.html", user_history=user_history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # Attempt to lookup stock price
        stock = lookup(request.form.get("symbol"))
        if not stock:
            return apology("invalid symbol")

        # Display stock prices for requested symbol
        return render_template("quoted.html", name=stock["name"], price=usd(stock["price"]), symbol=stock["symbol"])
    else:
        return render_template("quote.html", stocks=nasdaq_data)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure each field submitted
        if not username:
            return apology("must provide username", 400)
        elif not password:
            return apology("must provide password", 400)
        elif not confirmation:
            return apology("must provide password confirmation", 400)

        # Ensure password matches confirmation
        if password != confirmation:
            return apology("password confirmation failed", 400)

        # Encrypt password
        password = generate_password_hash(password)

        # Attempt to insert new credentials, rejected if duplicate username
        try:
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, password)
        except ValueError:
            return apology("username already exists", 400)

        # Redirect to login, if successful
        return redirect("/")

    # User reached route via GET (by clicking "Register")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Parse and validate user/form information
        user_id = session["user_id"]

        symbol = request.form.get("symbol")
        stock = lookup(symbol)
        if not stock:
            return apology("invalid symbol")

        price = stock["price"]

        try:
            quantity = int(request.form.get("shares"))
        except:
            return apology("share quantity must be a positive whole number")
        if not isinstance(quantity, int) or quantity <= 0:
            return apology("share quantity must be a positive whole number")

        # Retrieve currently-held share quantity
        held_quantity = db.execute(
            "SELECT symbol, SUM(quantity) FROM purchases WHERE user_id = ? AND symbol = ? GROUP BY symbol HAVING SUM(quantity) > 0", user_id, symbol)[0]["SUM(quantity)"]

        # Prevent selling more stocks than currently held
        if quantity > held_quantity:
            return apology("unsufficient stock quantity currently held")

        # Calculate total price
        total_price = price * quantity

        # Retrieve current user cash balance
        user_balance = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]["cash"]

        try:
            # Sell stock, updating user balance
            db.execute("UPDATE users SET cash = ? WHERE id = ?",
                       user_balance + total_price, user_id)

            # Add sale to log
            db.execute("INSERT INTO purchases (user_id, symbol, price, quantity) VALUES(?, ?, ?, ?)",
                       user_id, symbol, price, quantity*-1)  # negative qty for sale
        except:
            return apology("error updating purchase database")

        # Return back to homepage
        return redirect("/")
    else:
        # Obtain currently held stocks
        user_id = session["user_id"]

        held_stocks = db.execute(
            "SELECT symbol, SUM(quantity) FROM purchases WHERE user_id = ? GROUP BY symbol HAVING SUM(quantity) > 0", user_id)

        # Render template to select stocks to sell
        return render_template("sell.html", held_stocks=held_stocks)
