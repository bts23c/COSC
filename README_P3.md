# bts67P3.cpp — COSC 1020 Project 3

## Overview
A menu-driven, multi-file ticket management system for MCMT. Extends the batch processing from P2 by introducing a persistent interactive menu and the ability to load, display, and clear data from multiple production files across a session.

## How It Works
The program loops on a menu with five options:

| Key | Action |
|-----|--------|
| U   | Upload a ticket data file (validates records, appends valid ones to memory) |
| A   | Display all loaded orders in a formatted table |
| D   | Display a revenue summary grouped by performance day/time |
| T   | Display a revenue summary grouped by production title |
| C   | Clear all loaded data from memory |
| Q   | Quit |

Multiple files can be uploaded in sequence; data accumulates across uploads until cleared. The production title (Curtains, Oklahoma, Seussical, Urinetown, or Other) is inferred from the filename.

## Data Storage
All order data is stored in **parallel vectors** — one vector per field (day, time, adult count, etc.) — all maintained in `main()` and passed by reference to each function. This is the defining structural characteristic of P3, which P4 later refactors into a class.

## Key Details
- Validation logic mirrors P2 (day/time combos, ticket counts, accessibility rules).
- `calculateCost()` is a standalone helper used by both `uploadFile` and `allDetails`.
- Summary functions accumulate into fixed-size arrays indexed by category.
- Input is normalized to uppercase via `toupper()` throughout.
