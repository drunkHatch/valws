The program consists of three sub programs: valgrind, studnet program and gnuplot.

Basic design:

  There are 2 ways to send messages between these programs:
    1, write and load from disk
    2, write and load via pipe

  It's obvious that tansfer via disk is the second slowliset way(Only faster than Internet), 
  therefore we would avoid accessing to disk as much as possible.
  To save our program's runtime, we decide to send messages via the pipe way.
  So we need to generate two pipes:
    1, from valgrind to student program
    2, from student program

  The way we create first pipe is from eclass: "|" in command line
  For the second pipe we have another solution which create the pipe in our 
  student program and send data to the pipe via fprint. All gnu commands and  
  values go through this pipe.

Deailed design:
  Window implementation: sliding window
    Window is a linked list to delete node obeying FIFO
    But we do not count unique pages here.

  Hash table implementation: bucket number = window size / 10, linked list
    Since window size is the exact number of pages we need to calculate unique,
    we divide the whole window into one tenth of window size and store each piece in 
    one bucket.
    Each bucket is a smaller window(linked list)

    And the unique pages is counted every time a new node is added to table or 
    oldest node is removed from table.

  Stream:
    valgrind -(generate one memory use report)->
      valws379 -(update by report then update unique)->
        gnuplot -(get new unique then update diagram)->
    When the stream ends, gnuplot generate plots we want via the stream




