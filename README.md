# cityexplore
cityexplore, task given as test.

Used FLTK + Modern JSON by nlohmann

The following are the requirements: 

Please perform the following tasks in C++11 or newer.

Having a data scheme (global_data_scheme):
1. Propose and implement a text utility for displaying this data in very ergonomic way (please propose your own design)
 or
2. Implement filtering engine that given a list of filters like in example_filters.json will find and print all entities
   that match provided filtering criteria.
    a. path to a filtered element is provided with a simple json-pointer syntax
    b. numerical types must support following filters
      * eq - equal to
      * ge - greater or equal
      * le - less or equal
      * lt - less than
      * gt - greater than
      * in - within a <min, max> bounds    
    c. Make the tool ergonomic and easy to use

3. User input errors should be handled in an ergonomic way
4. Errors in input data should be handled
5. Provide a cmake file that will compile the project with any 3rd party dependencies that were used
6. Any external dependency should be downloaded automatically by cmake file, or manually by provided script (please provide documentation).
 


General Notes:
* Results should be submitted within 10 days.
* Tasks do not have to be 100% complete, but both have to be largely accomplished.
* Please treat the tasks as if the recipient was a person with negligible technical skills. Please put emphasis on ergonomic aspects. 
* We care about the candidate's own invention in solving the problems he has defined.
* After receiving the assignments we will check them and get back to you within a week.
* In case of negative feedback, the candidate will receive a brief descriptive assessment of the assignment.
* If we evaluate the task positively, we will invite the candidate for an interview (skype).

