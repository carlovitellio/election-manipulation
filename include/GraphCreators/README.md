## *Adding new GraphCreator*
For implementing a new GraphCreator, it is requested to follow the
GraphCreatorBase class and inherits from it.
If new GraphCreator are implemented, they have to be added to the corresponding
factory. For doing so, it is reccomended to create a dynamic library and to add
in the corresponding source file a function using the *constructor* attribute
calling '<addRuleToFactory>' function in *GraphCreatorFactory.hpp* with the
new method.
