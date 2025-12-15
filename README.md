CS-300: Data Structures and Algorithms – Portfolio Reflection
Student: David Silva
Course: CS-300 Analysis and Design
Instructor: Saba Jamalian
December 15, 2025


Project Overview
The projects in CS-300 focused on designing and implementing an efficient course planning system using different data structures. The goal was to store course information, retrieve individual course details quickly, and display an ordered list of courses. Project One emphasized analyzing the runtime and memory trade-offs of multiple data structures, while Project Two required implementing a working solution that sorts and prints course data in alphanumeric order.

Problem Solved
The primary problem addressed was determining the most effective data structure for managing and searching course information. The system needed to load course data from a file, allow fast lookups by course number, and print all courses in sorted order. This required balancing performance, readability, and maintainability.

Approach and Use of Data Structures
I approached the problem by comparing three data structures: vectors, hash tables, and binary search trees. Each structure was evaluated based on loading time, search efficiency, and printing performance. This comparison highlighted why understanding data structures is critical to designing efficient software. Based on the analysis, a hash table was selected for the final implementation due to its constant-time average lookup performance.

Overcoming Challenges
One of the main challenges was managing data parsing and ensuring consistent formatting when loading course information from a CSV file. I addressed this by implementing input validation, trimming whitespace, and converting course numbers to a standard format. Another challenge was sorting data stored in a hash table, which required collecting all elements into a vector and applying a sorting algorithm before printing.

Growth in Software Design
Working on these projects expanded my understanding of how design decisions directly impact performance and usability. Instead of focusing only on getting a program to work, I learned to evaluate how well it works under different conditions. This experience reinforced the importance of selecting appropriate data structures early in the design process.

Maintainability and Readability
These projects improved the way I write maintainable and readable code. I focused on clear function separation, meaningful variable names, and in-line comments to explain logic. This structured approach makes the program easier to understand, debug, and extend in the future.

Conclusion
Through analyzing and implementing multiple data structures, I gained a deeper understanding of algorithm efficiency and system design. The hash table implementation provided the best balance of performance and usability for the Course Planner system. Overall, this course strengthened my ability to design efficient, scalable, and well-structured software solutions.
