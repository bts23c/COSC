# bts67P4.cpp — COSC 1020 Project 4

## Overview
A refactored version of the P3 menu-driven ticket system. The core change is replacing all parallel vectors with a single `vector<TicketOrder>`, where `TicketOrder` is a fully defined class encapsulating all order data and cost logic.

## The `TicketOrder` Class
Each order is represented as a single object with the following design:

**Private members:** `productionTitle`, `performanceDay`, `performanceTime`, `adultCount`, `seniorCount`, `studentCount`, `accessibility`, `accessibleCount`, `contactDetails`

**Constructors:** Default, parameterized, and copy constructor are all implemented. An assignment operator with self-assignment guard is also provided.

**Cost methods:**
- `getTicketCost()` — base ticket subtotal by time (evening/matinee pricing)
- `getDiscount()` — 7.5% group discount if total tickets exceed 10
- `getProcessingFee()` — flat $0.50 per ticket
- `getTotalCost()` — combines the above three

**Operator overloads:**
- `operator>>` — reads a record from an input stream directly into the object
- `operator<<` — prints a formatted order summary to an output stream

## Menu (same structure as P3)

| Key | Action |
|-----|--------|
| U   | Upload a file; production title inferred from filename |
| A   | Display all loaded orders via `operator<<` |
| D   | Revenue summary by performance day |
| T   | Revenue summary by production title |
| C   | Clear the vector |
| Q   | Quit |

## Key Differences from P3
- All nine parallel vectors are replaced by `vector<TicketOrder>`.
- Data and behavior are co-located inside the class rather than spread across free functions and multiple vectors.
- `operator>>` and `operator<<` are declared as `friend` functions for direct private member access.
- Function signatures are dramatically simpler — functions receive the single vector, not nine separate parameters.
