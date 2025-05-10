# C Threaded Event Queue

A simple, thread-safe event queue implementation in C, using POSIX threads (pthreads). allowing multiple producer and consumer threads safely enqueue and dequeue event messages via a shared queue, synchronized with a mutex and condition variable.

## Features

- **Thread safety**  
  Uses `pthread_mutex_t` for mutual exclusion and `pthread_cond_t` to signal waiting consumers when new events arrive.
- **FIFO ordering**  
  Events are dequeued in the same order they were enqueued.
- **Lightweight**  
  Minimal dependencies—just standard C and pthreads.
- **Easy integration**  
  Drop the header and source into your project and link with `-lpthread`.
  
## Requirements

- A POSIX-compliant system (Linux, macOS, BSD)
- GCC or any standards-compliant C compiler
- pthreads library

## Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/your-username/c-threaded-event-queue.git
   cd c-threaded-event-queue
