# Input files
## Graph_input.getpot
Most of the options of the manipulation process may be chosen at run-time since they are read from this file (or similar if correctly pointed executing the program).

Under *[Output]* the user indicates whether to output the final social network in a file in the out folder or not and may also choose to output the influence metrics in the aforementioned directory.

Under *[Graph_option]* are included all the characteristics of the Graph to be implemented. Please look at the documentation of the corresponding class for the correct interpretation of each parameter.

Under *[Person_option]* the distributions used to generate each Person in the network may be chosen.

Under *[InfluenceOption]* the options for the influence process are chosen.

## txt files
The two text files (resistanceDistrib.txt and VotingDistrib.txt) have been used to test the ParsedDistribution classes. They are in fact parsed with MuParser.

In resistanceDistrib.txt, beyond the standard operations, it is also available a factorial function.

## other files
The other files are graphs saved in different format. They have been used to test the GraphCreatorInputFile class.
