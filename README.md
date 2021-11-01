# Election Manipulation

Carlo Augusto Vitellio (carloaugusto.vitellio@mail.polimi.it) APSC project.

The project consists of the implementation of the election manipulation problem in the framework of unknown preferences.
First, it is implemented the actual Social Network Creator and, then, the proper manipulation phase.

For what concerns the creation of the Social Network it is used a plugin architecture for letting the user the choice of what type of Graph to study. In fact, the user may specify in an input-file read at run-time which Graph Creator method to use and to point the corresponding dynamic library to be loaded.
Furthermore, the user may choose the way the Network is populated with people.
In particular, he/she may decide which distributions are used for generating each Person in the Graph.

The manipulation phase is characterised by a manipulator that exploits an [Information cascade](https://en.wikipedia.org/wiki/Information_cascade) for influencing the Voters in the network.

For futher details, please read the report.pdf

## Libraries used
* [Boost Graph Library](https://www.boost.org/)
  * Mainly, it was used the template only part of the library. This means that no compilation is needed for most of the code.
  * For implementing the GraphCreatorInputFile class two other libraries were also used. In particular, "boost_graph" and "boost_regex" were used.
  
  For these reasons, it suggested to install the Boost library following [getting_started](https://www.boost.org/doc/libs/1_77_0/more/getting_started/index.html) or look to the section "Installing Boost" below
* [MuParser](https://beltoforion.de/en/muparser/)
  
  For reading and parsing probability distributions from text file.
  For installing it, follow https://beltoforion.de/en/muparser/building.php
* [GetPot](http://getpot.sourceforge.net/)
  
  It is actually a single header file and it has already been included in the include/ExternalUtilities folder
* Some utilities implemented by Prof. Luca Formaggia
  
  Those utilities have been proposed as examples in his APSC classes at Politecnico di Milano and they are available at https://github.com/pacs-course/pacs-examples

It may also be needed to have installed [graphviz](https://graphviz.org/download/).

Once installed both the Boost and MuParser library, the file Makefile.inc should be modified in order to link to the proper folders containing the libraries.
In particular, the user should modify the variables *ROOT*, *mkBoostInc*, *MUPARSER_INC_DIR* and *MUPARSER_LIB_DIR*.

## Make instructions
After having modified the variables listed above in the Makefile.inc file, it is possible to build the project.

* With *make help* the user may see the help of the make
* With *make all* it is generated a dependency file, dynamic libraries are built and the executable is made
* With *make doc* documentation is generated via [Doxygen](https://www.doxygen.nl/index.html)

## Executable instructions
After having made the executable, it may be asked its help via *./main -h*

## Installing Boost on Unix variants
This is not intended to replace the [Boost Documentation](https://www.boost.org/doc/libs/1_77_0/more/getting_started/index.html), but it simply tries to help the user.

1. download the library from [link](https://www.boost.org/users/history/version_1_77_0.html)
1. In the terminal write
  1. tar --bzip2 -xf /path/to/boost_1_77_0.tar.bz2
  1. cd /path/to/boost_1_77_0.tar.bz2
  1. ./bootstrap.sh --with-libraries=graph,regex

You may need write permission in the directory where you decide to store the library.
