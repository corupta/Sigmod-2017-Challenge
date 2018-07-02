# Sigmod - 2017 Challange
### Made for ACM SIGMOD 2017 Programming Contest

I've created this solution for the ACM Sigmod 2017 Programming Contest on Mar 22th, 2017.
I've not been able to finish the whole idea in my mind but as far as I remember the current version do run very fast but I've failed to submit due to some funny technical issues with the system. (My C reader did not work with their system but did run successfully on my system.) 

## Task 

An N-gram of words is a contiguous sequence of N words (https://en.wikipedia.org/wiki/N-gram). For this year's contest, the task is to filter a stream of documents using a set of N-grams of interest and, for each document, return where in it one of the N-grams is found The input to the task will have two parts: first, an initial set of N-grams, which may be processed and indexed; second, a series of queries (documents) and N-gram updates (insertions or deletions), arbitrarily interleaved. For each N-gram insertion or deletion, the set of N-grams of interest is updated accordingly. For each new query (document) arriving, the task is to return as fast as possible the N-grams of the currently up-to-date set that are found in the document. These should be presented in order of their first appearance in the document. If one N-gram is a prefix of another and the larger one is in the document, then the shorter one is presented first. Note that in answering each query, all insertions/deletions preceding the query must be taken into account.

For more details check out [ACM Sigmod 2017 Programming Contest](http://sigmod17contest.athenarc.gr/task.shtml)

## Test Dataset
Designed to be used on Linux.

Run "./compile.sh" to get binary.
copy it to test folder
Run "make" to get test harness binary
Run "./harness" to get instructions on how to make tests.
Test folder includes the small test dataset.
You can download this big dataset to further test my project. [Test Dataset](http://sigmod17contest.athenarc.gr/test-dataset.tar.bz2)

## Solution

1 - Team Name:      corupta

2 - Member Info:
    
    There's only one member. ( Me :P )

    Halit Özsoy (Halit Ozsoy)
    halitozsoy1584@gmail.com
    halit.ozsoy@boun.edu.tr
    corupta@corupta.net
    Bogazici University, Departmant of Engineering, Computer Engineering, 1st Year, 2nd Semester, Turkey

3 - No Advisors / Supervisors
    
    (I've studied for Olympiads In Informatics myself, back then. I've won two medals, only in the national olympiads, though.)
    (However, I got used to learning lots of crazy thing myself. So, no advisors. )

4 - A Brief Description of The Solution

    PS: I created a datatype, which I called Prefix Tree. It is very similar to a Suffix Tree in reverse, but not exactly the same. 
        Well, I haven't previously written a Suffix Tree before, and when trying to come up with a solution myself (not referencing any wiki), I came up with this, Prefix Tree.
        Then, I liked the idea of developing such data structure in C, to see how fast can it work. So, here's the final product.
        
        Send me an e-mail if you are interested in understanding how the Prefix Tree works.

    PS2: It's more of a Persistent Prefix Tree, I would say. Since, I could parallelize insertion / deletions with queries.

    PS3: I created this readme before actually finishing the code, so I hope the code will finish in time, with all the specs below.

    PS4: I think I have been coding for 16 hours of the last 20 hours, my total finished work for coding ~ 40 hours, and for thinking / designing in the beginning ~ 10 - 30 hours.
            Am I too slow? I think I should code for 16 more hours. (March 22nd, 2017, 05:54 GMT + 2 ) Linear Code is Almost Finished.

    To put very very simply:

    Insertion:
        Firstly, converts each word into an unsigned integer using a Trie. Obtains an NGram of N numbers.
        Secondly, puts the NGram into an NGramLibrary (if it does not exist), returns an id.
        Thirdly, inserts the NGram into a Prefix Tree with the id.

    Deletion:
        Basically, changes a boolean so that it can be checked before printing whether the NGram is deleted.

    Query:
        Breaks the input into tokens of unsigned integers formed by words existing in the Trie.
        I call each of these tokens a WordGroup.
        WordGroups are investigated in parallel, and matched nodes of the Prefix Tree are inserted to a Printer class. 
        (With lots of queues, because I realized the output must be sorted in (start_point, len) not (end_point, len) which I intially misunderstood.)
        (It is easier to calculate the (end_point,len) sorted list with the Prefix Tree.) 

   
    Parallelization:
        ( It is my first time, doing parallelization in C, so I would probably make a terrible implementation. )
        1 thread is used for Printer Class => 
            repeatedly
            reviews the inserted nodes in the right order, and sends the chosen ones ( that are printable, not deleted, etc. ) to FastPrintF Class (sends them to buffer)

        1 thread is used for FastPrintF Class =>
            repeatedly
            looks up in its buffer, and prints the characters inside.

        1 thread is used for Main / LineReading / Insertion - Deletion Operations
            reads next line
            if it's a query spawns a new thread for the query.
            if it's deletion / operation, this thread itself does the operation.

        Other Threads

            QueryRipper :P => Breaks a query into tokens, spawns threads to search NGrams inside each token. (1 thread for each token)

            NGram searcher => matches the input with nodes in the Prefix Tree. Then inserts those onto Printer class.



        1'st priority thread => LineReader (Reading more lines improve parallelization)
        2'nd priority thread => FastPrintF (Because printing is a very slow operation)
        3'rd priority thread => Printer (Because printing is a very slow operation)

        then, smaller NGram searcher are higher priority.
        then, smaller QueryRippers are higher priority.

        smaller = relating to  smaller Line / WordGroup number

5 - A list of the third party code and their licenses.
    
    3'rd Party ?? I'm stubborn enough to write everything from scratch. That's what speeds up the codes these days, aren't it?
    Well, I know it's not a great practice but I'm too much used to doing everything from scratch, that's why I've coded in C.

