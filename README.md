# NoSA (No Springs Attached)
Calculates the normal modes of a multiple mass-spring system.

Requirements:-
  1. Armadillo version armadillo-6.700.7
  2. Python version 3.5
  3. numpy version
  4. pyQt version
  5. [recommended anaconda for OS X ]
  6. Cython version

Checklist [physics]:-
  1. Build ConnectedSystem class which can add and remove masses and springs [DONE]
  2. Working makefile [DONE]
  3. Integrate Armadillo linear algebra package [DONE]
  3. Build force matrix for example system and solve for eigenvalues and eigenvectors [DONE]
  4. Consistency check with simple analytical case [DONE]
  5. Implement solutions to produce a dynamic system [DONE]

Checklist [GUI & I/O]:-
  1. Integrate the consys class with GUI [DONE]
  2. Make objects scaleable [DONE]
  3. Make an interface panel [DONE]
  4. Build in animations [DONE]
  5. Make objects react to click events
