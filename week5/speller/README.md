# Spell Checker Efficiency Comparison

This assignment focused on developing a spell-checker program within a framework provided by Harvard CS50's staff. The primary challenge was to optimize the program's efficiency and compare its CPU time performance against the version developed by the Harvard staff.

## My Program's Performance

The challenge emphasized implementing and optimizing various aspects of the spell-checker, including:
- Dictionary loading (containing 143,091 words)
- Hashing
- Dictionary size check
- Spell-checking user-supplied text
- Dictionary unloading

To achieve maximum efficiency, I minimized overhead and redundancy, designing a lightweight and highly efficient hash function.

My program outperformed the Harvard staff's implementation in a majority of test cases. At its best, it achieved **200.00% comparable efficiency** and, at its worst, **92.11% efficiency**. 

For test cases with fewer than 9,622 lines (approximately 385 pages), my program consistently outperformed the Harvard implementation. However, with larger texts, it showed slightly reduced performance. For the largest test case, *shakespeare.txt* (124,453 lines, 4,979 pages), my program achieved **93.18% comparable efficiency**.

A significant portion of the efficiency gains with smaller texts comes from my implementation of unloading the dictionary from memory, which my program potentially performs up to **10 times faster** at constant time complexity, with respect to spell-checked text.

## Performance Results Table

Program performance was measured in seconds using the CPU time implementation provided by the Harvard staff. When run time was inconsistent, median time was taken from ~10 tests. Efficiency calculated as: **(Harvard Program Time) / (This Program Time)**.


| Test Case | Length (lines) | Pages (estimate) | This Program Time | Harvard Staff Solution Time | Efficiency Difference |
|---------------------------------------------------|----------------|------------------|---------|-----------------------------|-----------------------|
| cat.txt                                           | 1              | 1                | 0.02    | 0.04                        | 200.00%               |
| pneumonoultramicroscopicsilicovolcanoconiosis.txt | 4              | 1                | 0.02    | 0.04                        | 200.00%               |
| wordsworth.txt                                    | 31             | 2                | 0.02    | 0.03                        | 150.00%               |
| constitution.txt                                  | 872            | 35               | 0.02    | 0.03                        | 150.00%               |
| stein.txt                                         | 2,337          | 94               | 0.02    | 0.03                        | 150.00%               |
| lalaland.txt                                      | 4,751          | 191              | 0.04    | 0.05                        | 125.00%               |
| her.txt                                           | 6,094          | 244              | 0.04    | 0.05                        | 125.00%               |
| revenant.txt                                      | 5,524          | 221              | 0.05    | 0.06                        | 120.00%               |
| birdman.txt                                       | 6,222          | 249              | 0.05    | 0.06                        | 120.00%               |
| carroll.txt                                       | 3,736          | 150              | 0.06    | 0.07                        | 116.67%               |
| burnett.txt                                       | 3,209          | 129              | 0.08    | 0.09                        | 112.50%               |
| mansfield.txt                                     | 3,235          | 130              | 0.08    | 0.09                        | 112.50%               |
| rinehart.txt                                      | 3,557          | 143              | 0.10    | 0.11                        | 110.00%               |
| frankenstein.txt                                  | 9,302          | 373              | 0.10    | 0.11                        | 110.00%               |
| wells.txt                                         | 6,793          | 272              | 0.15    | 0.16                        | 106.67%               |
| grimm.txt                                         | 9,622          | 385              | 0.12    | 0.12                        | 100.00%               |
| xueqin1.txt                                       | 19,161         | 767              | 0.21    | 0.20                        | 95.45%                |
| federalist.txt                                    | 19,480         | 780              | 0.22    | 0.21                        | 95.45%                |
| surgery.txt                                       | 24,335         | 974              | 0.22    | 0.21                        | 95.45%                |
| homer.txt                                         | 26,176         | 1,048            | 0.22    | 0.21                        | 95.45%                |
| stoker.txt                                        | 16,624         | 665              | 0.18    | 0.17                        | 94.44%                |
| austen.txt                                        | 13,426         | 538              | 0.15    | 0.14                        | 93.33%                |
| whittier.txt                                      | 82,043         | 3,282            | 0.60    | 0.56                        | 93.33%                |
| shakespeare.txt                                   | 124,453        | 4,979            | 0.88    | 0.82                        | 93.18%                |
| tolstoy.txt                                       | 67,402         | 2,697            | 0.57    | 0.53                        | 92.98%                |
| xueqin2.txt                                       | 28,245         | 1,130            | 0.28    | 0.26                        | 92.86%                |
| aca.txt                                           | 58,485         | 2,340            | 0.40    | 0.37                        | 92.50%                |
| holmes.txt                                        | 115,633        | 4,626            | 1.14    | 1.05                        | 92.11%                |


## Examples (Left: My Program, Right: Harvard Staff Program)
Shortest Text: cat.txt (1 line, 1 page)

![cat_results](https://github.com/user-attachments/assets/f38d834d-3535-4ae5-9132-891ca311963c)

Longest Text: shakespeare.txt (124,453 lines, 4,979 pages)

![shakespeare_results](https://github.com/user-attachments/assets/8763aa7d-73f0-49dc-84a2-8b8d9cb25056)


