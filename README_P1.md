# bts67P1.cpp — COSC 1020 Project 1

## Overview
An interactive, single-transaction ticket purchasing system for the MCMT (community theatre). The program walks a user through buying tickets for one performance via console prompts and prints a formatted receipt.

## How It Works
1. **Day selection** — User enters F (Friday), S (Saturday), or U (Sunday). Friday is evening-only; Sunday is matinee-only; Saturday asks for a follow-up time choice (M/E).
2. **Ticket counts** — User enters the number of adult, senior, and student tickets.
3. **Accessible seating** — If requested (Y/N), the user specifies how many accessible seats are needed. These cannot exceed 50% of the total order.
4. **Cost calculation** — Prices differ by time (evening vs. matinee). Orders over 10 tickets receive a 7.5% group discount. A flat $0.50 processing fee is added per ticket.
5. **Receipt output** — Prints ticket counts, gross cost, discount, net cost, processing fees, and final total.

## Key Details
- Validates all inputs and exits with an error message on any invalid entry.
- Uses named constants for all prices, thresholds, and rates (no magic numbers).
- Formatting handled with `iomanip` (`setw`, `fixed`, `setprecision`).
- No file I/O — fully interactive, one order per run.
