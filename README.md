# Design and Analysis of Operating Systems
* **Course:** [CSCI 3753, Design and Analysis of Operating Systems](https://moodle.cs.colorado.edu/course/view.php?id=164)
* **Semester:** Summer C Session - June 5th to July 28th
* **Instructor:** Chris Womack, [christopher.womack@colorado.edu](mailto:christopher.womack@colorado.edu)
* **Office Hours:** Fridays after recitation 10:30 to 12:30
   * [Piazza forum](https://piazza.com/colorado/summer2017/csci3573/home) for questions and discussion 
   * [Email](mailto:christopher.womack@colorado.edu) for 1-on-1 help, or to set up a time to meet
* **Office Location:** CSEL or ECCS 112C
* **Class Time:** Monday through Friday from 9:15 to 10:35 (recitation is the Friday class)
* **Class Location:** ECCS 112C in CSEL

## Course Overview

This course introduces students to important Operating Systems concepts. The course will cover key aspects of how an operating systems functions on a modern computer.  The following major topics will be covered:

* Basic System Structure
* Processes and Threads
* Virtual Memory
* File Systems
* Security
* Networking and Distributed (File) Systems
* Virtual Machines

In addition, the class will gain familiarity with important software tools such as debuggers, compilers, editors, kernel modules, and virtual machines.

### Prerequisites
* CSCI 2270 and either CSCI 2400 or ECEN 3350 (all minimum grade C-)
* Understanding of basic data structures and computer memory concepts
* Strong knowledge of C/C++ style coding

These won't be enforced by the instructor, but you will be pretty lost without understanding those concepts.

### Textbook

**Required:** Operating System Concepts, 9th Edition, Abraham Silberschatz, Peter Galvin, Greg Gagne (electronic or hardcover)

**Suggested:** [Combined Volume Set of Intel® 64 and IA-32 Architectures Software Developer’s Manuals](http://www.intel.com/content/www/us/en/processors/architectures-software-developer-manuals.html)

### Course Website / Resources

Please enroll ASAP in the [Moodle course web page]((https://moodle.cs.colorado.edu/course/view.php?id=164)). All of your class material will be available through Moodle, and all exams will be administered here as well. The enrollment key is: **kernel**

Also enroll in [Piazza forum](https://piazza.com/colorado/summer2017/csci3573/home) for this course. This will be the first point of contact regarding assignment questions and discussions.

As moodle can be a little tedious to use for downloading all class material, I will be maintaining a repo for this class with the following content:

- Lecture Slides
- Problem Sets
- Programming Assignments
- Demo Code from Class

There is also a corresponding [website](https://chris-womack.github.io/CSCI-3753-Womack-Sum2017/) that will help you get setup for the semester.

### Course Schedule

| Week  | Topic                                                                                             | Reading      |
|:-----:|:-------------------------------------------------------------------------------------------------:|:------------:|
| 1     | Intro, Kernel Mode, System Calls, I/O, Interrupts, DMA                                            | Ch. 1, 2, 13 |
| 2     | Processes, IPC, Synchronization, Mutual Exclusion                                                 |  Ch. 3, 5 |
| 3     | Monitors, Conditional Variables, Deadlock, Deadlock Detection, Avoidance, Scheduling              | Ch. 5, 6, 7 |
| 4     | Scheduling, Memory Managment                                                                      | Ch. 6, 7, 8 |
| 5     | Paging, Page Replacement                                                                          | Ch. 8, 9 |
| 6     | Memory Allocation, File Systems, File Allocation, Free Space Managment                            | Ch. 9, 10, 11, 12 |
| 7     | Disk Scheduling, Flash File Systems, RAID, Authorization, Symmetric Key Crypto, Public Key Crypto | Ch. 10, 11, 12, 14, 15 |
|8      | Networking, Virtual Machines                                                                      | Ch. 16, 17 |

## Grading

* Final Exam - 25%
* Midterm - 15%
* Programming Assignments - 50%
* Problem Sets - 10%

## Grading Standards
**Programming Assignments:** Grading for programming assignments (PAs) will be based on interview-style grading, where 40% of the grade will be based on the code submitted (does it compile, does it execute the required functions) and 60% of the grade will be based on answering questions from the instructor in the interview (questions may be based on explaining the code and/or explaining software concepts that the PAs cover), unless otherwise noted. 

No teams.  Each student is responsible for doing their own work.

   * *Code*: Programming assignments must be submitted by uploading your code to the moodle site by their due date, unless otherwise noted. All PAs must be written in C and compiled for execution in the specified environment for that PA, unless otherwise noted. No late submissions will be accepted. If you submit your assignment late, you will recieve a zero for the code portion of the assignment. I highly encourage you to submit even partially completed programs to recieve partial credit.

   * *Interview:* Randomized interviews will be assigned after the due date of each assignment. Each student who is selected must arrange an individual grading interview with the instructor for each PA. Interview time slots will be posted on the moodle.  You may bring your own laptop to these grading sessions to demonstrate your program to the instructor and answer questions. Even if your PA code is submitted in time, you must attend your grading meeting to be given a grade for the interview portion of the PA. Any missed meetings (without notifying your instructor ahead of time with a suitable reason) may result in a zero grade for the interview portion of the assignment. The instructor is under no obligation to reschedule your appointment if you miss your meeting, so write down your meeting times, and don't forget them!

**Problem Sets:** All problem sets must be submitted by the deadline.  No late submissions are allowed.

## Statements on Plagiarism

All students of the University of Colorado at Boulder are responsible for knowing and adhering to the [Academic Integrity Policy](http://www.colorado.edu/policies/academic-integrity-policy) of this institution. Violations of this policy may include: cheating, plagiarism, aid of academic dishonesty, fabrication, lying, bribery, and threatening behavior. All incidents of academic misconduct shall be reported to the Honor Code Council (honor@colorado.edu; 303-735-2273). Students who are found to be in violation of the academic integrity policy will be subject to both academic sanctions from the faculty member and non-academic sanctions (including but not limited to university probation, suspension, or expulsion). Additional information regarding the [Honor Code Policy](http://www.colorado.edu/honorcode/sites/default/files/attached-files/honor_code_policies_and_procedures_2017_0.pdf) can be found online and at the [Honor Code Office](http://www.colorado.edu/honorcode/).

In this class, each student is expected to submit his/her own original work for each assignment, whether it is a lab, quiz, problem set, exam, or other assignment, where the only exceptions to this policy will be noted by the instructor.

Violations of this plagiarism policy, the University Honor Code, and/or the Honor Code pledge shown below will be grounds for one or more of the following: grade reduction, a failing grade, and/or potentially other punitive academic actions, in accordance with the University of Colorado's policy on plagiarism and Honor Code.

<!--
### Instructor

Reuse and building upon ideas or code are major parts of modern software development. As a professional programmer you will never write anything from scratch. This class is structured such that many of the solutions are public. You are encouraged to learn from the work of your peers. I won't hunt down people who are simply copying-and-pasting solutions, because without challenging themselves, they are simply wasting their time and money taking this class.

Please respect the terms of use and/or license of any code you find, and if you reimplement or duplicate an algorithm or code from elsewhere, credit the original source with an inline comment.
-->

## Disability

If you qualify for accommodations because of a disability, please submit to your professor a letter from Disability Services in a timely manner (for exam accommodations provide your letter at least one week prior to the exam) so that your needs can be addressed. Disability Services determines accommodations based on documented disabilities. Contact Disability Services at 303-492-8671 or by e-mail at dsinfo@colorado.edu. 

If you have a temporary medical condition or injury, see Temporary Injuries guidelines under the Quick Links at the [Disability Services Website](http://www.colorado.edu/disabilityservices/) and discuss your needs with your professor.

## Religious Observances

Campus policy regarding religious observances requires that faculty make every effort to deal reasonably and fairly with all students who, because of religious obligations, have conflicts with scheduled exams, assignments or required attendance.

In this class, every effort will be made to accommodate all students who, because of religious obligations, have conflicts with scheduled exams, assignments, or other required attendance, provided they notify the professor well in advance of the scheduled conflict. Whenever possible, students should notify faculty at least two weeks in advance of the conflict to request special accommodation. 

See the [Religious Observances Policy](http://www.colorado.edu/policies/observance-religious-holidays-and-absences-classes-andor-exams) for full details.

## Discrimination and Sexual Harassment

The University of Colorado Boulder (CU-Boulder) is committed to maintaining a positive learning, working, and living environment. CU-Boulder will not tolerate acts of discrimination or harassment based upon Protected Classes or related retaliation against or by any employee or student. For purposes of this CU-Boulder policy, "Protected Classes" refers to race, color, national origin, sex, pregnancy, age, disability, creed, religion, sexual orientation, gender identity, gender expression, veteran status, political affiliation or political philosophy. Individuals who believe they have been discriminated against should contact the Office of Institutional Equity and Compliance (OIEC) at 303-492-2127 or the Office of Student Conduct and Conflict Resolution (OSC) at 303-492-5550. Information about the OIEC, the above referenced policies, and the campus resources available to assist individuals regarding discrimination or harassment can be found at the [OIEC Website](http://www.colorado.edu/institutionalequity/). The full [Policy on Discrimination and Harassment](http://www.colorado.edu/policies/discrimination-and-harassment-policy-and-procedures) contains additional information. 

## Classroom Behavior

Students and faculty each have responsibility for maintaining an appropriate learning environment. Those who fail to adhere to such behavioral standards may be subject to discipline. Professional courtesy and sensitivity are especially important with respect to individuals and topics dealing with differences of race, color, culture, religion, creed, politics, veteran’s status, sexual orientation, gender, gender identity and gender expression, age, disability, and nationalities. Class rosters are provided to the instructor with the student's legal name. I will gladly honor your request to address you by an alternate name or gender pronoun. Please advise me of this preference early in the semester so that I may make appropriate changes to my records. For more information, see the [Policies on Classroom Behavior](http://www.colorado.edu/policies/student-classroom-and-course-related-behavior) and the [Student Code of Conduct](http://www.colorado.edu/osccr/sites/default/files/attached-files/student_code_of_conduct_2016-2017.pdf).


## Syllabus Changes

The instructor reserves the right to modify this syllabus as needed during the semester. Should any changes be necessary, the instructor will inform students of the change and post and updated copy of the syllabus to Moodle.
