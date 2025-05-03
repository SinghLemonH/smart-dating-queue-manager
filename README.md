# Smart Dating Queue Manager

A smart dating queue management system implemented in C as part of the CPE112 Data Structures course. This system helps users efficiently manage their romantic connections by prioritizing partners, scheduling dates, and optimizing travel plans using advanced data structures and algorithms.

## Features

- **User Authentication**: Secure login and registration using a hash table for efficient user management.
- **Partner Management**:
  - Add, view, and delete partners with priority-based ranking.
  - Partners are ranked based on relationship scores using a priority queue.
- **Date Scheduling**:
  - Calculate optimal schedules using Dijkstra's algorithm for shortest path calculation.
  - Prevent scheduling conflicts and double bookings.
  - Edit and view schedules in a user-friendly table format.
- **Data Management**:
  - Save and load partner data and schedules using CSV files for persistence.
  - Clear all data when logging out.
- **Dynamic Updates**:
  - Adjust schedules dynamically based on changing priorities or availability.

## Data Structures Used

- **Hash Table**: For efficient user authentication and management.
- **Priority Queue**: For ranking partners based on relationship scores.
- **Linked List**: For managing the queue of partners and schedules.
- **Graph**: For representing locations and calculating shortest paths using Dijkstra's algorithm.

## Requirements

- GCC version 13 or higher
- Make utility
- Operating System:
  - Windows
  - Linux 
  - macOS 

## How to Run

1. Clone the repository:

```bash
git clone https://github.com/your-repo/smart-dating-queue-manager.git
```
2. Navigate to the project directory:

```bash
cd smart-dating-queue-manager
```
3. Compile and run:

- On Windows:

```bash
gcc -o main.exe src/main.c src/auth.c src/dataManage.c libs/hashTable.c libs/priorityQueue.c libs/dijkstras.c -Iinclude
./main
```

- On Linux or macOS:

```bash
gcc -o main src/main.c src/auth.c src/dataManage.c libs/hashTable.c libs/priorityQueue.c libs/dijkstras.c -Iinclude
./main
```
## How It Works
1. User Authentication:

    - Users log in or register using a secure hash table.
    - Each user is assigned a unique ID (UID).

2. Partner Management:

    - Users can add partners with a relationship score.
    - Partners are stored in a priority queue, sorted by their scores.

3. Date Scheduling:

    - Users can schedule dates by selecting locations.
    - The system calculates the shortest travel path using Dijkstra's algorithm.
    - Prevents scheduling conflicts and allows users to edit schedules.

4. Data Management:

    - All partner and schedule data is saved to CSV files for persistence.
    - Data is cleared upon logout.

## Members
- Ms. Thanaporn Bunlusilp (yok) 67070503416
- Mr. Wichitchai Suwanno (Singh) 67070503439
- Ms. Pilaiwan Churdchu (Chomphu) 67070503492