# bsadlerP2.cpp — COSC 1020 Project 2

## Overview
A batch ticket processing system for MCMT. Instead of one interactive order, this program reads an entire CSV-style data file, validates each record, prints a formatted line-by-line report, and outputs a summary table grouped by performance slot.

## How It Works
1. **File input** — Prompts the user for a file path, skips the header line, then reads each record (day, time, adult/senior/student counts, accessibility flag, accessible seat count, contact info).
2. **Validation** — Each record is checked against multiple rules: valid day/time combos (no Friday matinees, no Sunday evenings), positive ticket totals, valid accessibility flag, and accessible seat count within 50% of the order.
3. **Cost output** — Valid records have their total cost calculated and printed inline. Invalid records display all associated error messages and a contact line for follow-up.
4. **Summary table** — After all records are processed, totals (ticket counts by category + revenue) are printed for each of the four performance slots: Friday Evening, Saturday Matinee, Saturday Evening, and Sunday Matinee.

## Key Details
- Uses a `CategoryTotals` struct to cleanly accumulate per-slot running totals.
- All pricing/fee constants are defined at file scope to avoid magic numbers.
- A helper lambda handles uniform summary row formatting.
- Reports both valid and invalid record counts at the end.
