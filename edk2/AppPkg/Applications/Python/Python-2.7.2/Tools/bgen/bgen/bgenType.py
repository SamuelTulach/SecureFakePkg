"""Type classes and a modest collection of standard types."""


from bgenOutput import *


class Type:

    """Define the various things you can do with a C type.

    Most methods are intended to be extended or overridden.
    """

    def __init__(self, typeName, fmt):
        """Call with the C name and getargs format for the type.

        Example: int = Type("int", "i")
        """
        self.typeName = typeName
        self.fmt = fmt

    def declare(self, name, reference=False):
        """Declare a variable of the type with a given name.

        Example: int.declare('spam') prints "int spam;"
        """
        for decl in self.getArgDeclarations(name, reference):
            Output("%s;", decl)
        for decl in self.getAuxDeclarations(name):
            Output("%s;", decl)

    def getArgDeclarations(self, name, reference=False, constmode=False, outmode=False):
        """Return the main part of the declarations for this type: the items
        that will be passed as arguments in the C/C++ function call."""
        if reference:
            ref = "&"
        else:
            ref = ""
        if constmode:
            const = "const "
        else:
            const = ""
        if outmode:
            out = "*"
        else:
            out = ""
        return ["%s%s%s%s %s" % (const, self.typeName, ref, out, name)]

    def getAuxDeclarations(self, name):
        """Return any auxiliary declarations needed for implementing this
        type, such as helper variables used to hold sizes, etc. These declarations
        are not part of the C/C++ function call interface."""
        return []

    def getargs(self):
        return self.getargsFormat(), self.getargsArgs()

    def getargsFormat(self):
        """Return the format for this type for use with PyArg_Parse().

        Example: int.getargsFormat() returns the string "i".
        (getargs is a very old name for PyArg_Parse, hence the name of this method).
        """
        return self.fmt

    def getargsArgs(self, name):
        """Return an argument for use with PyArg_Parse().

        Example: int.getargsArgs("spam") returns the string "&spam".
        """
        return "&" + name

    def getargsPreCheck(self, name):
        """Perform any actions needed before calling getargs().

        This could include declaring temporary variables and such.
        """

    def getargsCheck(self, name):
        """Perform any needed post-[new]getargs() checks.

        This is type-dependent; the default does not check for errors.
        An example would be a check for a maximum string length, or it
        could do post-getargs() copying or conversion."""

    def passInput(self, name):
        """Return an argument for passing a variable into a call.

        Example: int.passInput("spam") returns the string "spam".
        """
        return name

    def passOutput(self, name):
        """Return an argument for returning a variable out of a call.

        Example: int.passOutput("spam") returns the string "&spam".
        """
        return "&" + name

    def passReference(self, name):
        """Return an argument for C++ pass-by-reference.
        Default is to call passInput().
        """
        return self.passInput(name)

    def errorCheck(self, name):
        """Check for an error returned in the variable.

        This is type-dependent; the default does not check for errors.
        An example would be a check for a NULL pointer.
        If an error is found, the generated routine should
        raise an exception and return NULL.

        XXX There should be a way to add error clean-up code.
        """
        Output("/* XXX no err check for %s %s */", self.typeName, name)

    def mkvalue(self):
        return self.mkvalueFormat(), self.mkvalueArgs()

    def mkvalueFormat(self):
        """Return the format for this type for use with Py_BuildValue().

        This is normally the same as getargsFormat() but it is
        a separate function to allow future divergence.
        (mkvalue is a very old name for Py_BuildValue, hence the name of this
        method).
        """
        return self.getargsFormat()

    def mkvalueArgs(self, name):
        """Return an argument for use with Py_BuildValue().

        Example: int.mkvalueArgs("spam") returns the string "spam".
        """
        return name

    def mkvaluePreCheck(self, name):
        """Perform any actions needed before calling mkvalue().

        This could include declaring temporary variables and such.
        """

    def cleanup(self, name):
        """Clean up if necessary.

        This is normally empty; it may deallocate buffers etc.
        """
        pass

class ByAddressType(Type):
    "Simple type that is also passed by address for input"

    def passInput(self, name):
        return "&%s" % name



# Sometimes it's useful to define a type that's only usable as input or output parameter

class InputOnlyMixIn:

    "Mix-in class to boobytrap passOutput"

    def passOutput(self, name):
        raise RuntimeError, "Type '%s' can only be used for input parameters" % self.typeName

class InputOnlyType(InputOnlyMixIn, Type):

    "Same as Type, but only usable for input parameters -- passOutput is boobytrapped"

class OutputOnlyMixIn:

    "Mix-in class to boobytrap passInput"

    def passInput(self, name):
        raise RuntimeError, "Type '%s' can only be used for output parameters" % self.typeName

class OutputOnlyType(OutputOnlyMixIn, Type):

    "Same as Type, but only usable for output parameters -- passInput is boobytrapped"


# A modest collection of standard C types.
void = None
char = Type("char", "c")
short = Type("short", "h")
unsigned_short = Type("unsigned short", "H")
int = Type("int", "i")
long = Type("long", "l")
unsigned_long = Type("unsigned long", "l")
float = Type("float", "f")
double = Type("double", "d")


# The most common use of character pointers is a null-terminated string.
# For input, this is easy.  For output, and for other uses of char *,
# see the module bgenBuffer.
stringptr = InputOnlyType("char*", "s")
unicodestringptr = InputOnlyType("wchar_t *", "u")


# Some Python related types.
objectptr = Type("PyObject*", "O")
stringobjectptr = Type("PyStringObject*", "S")
# Etc.


class FakeType(InputOnlyType):

    """A type that is not represented in the Python version of the interface.

    Instantiate with a value to pass in the call.
    """

    def __init__(self, substitute):
        self.substitute = substitute
        self.typeName = None    # Don't show this argument in __doc__ string

    def getArgDeclarations(self, name, reference=False, constmode=False, outmode=False):
        return []

    def getAuxDeclarations(self, name, reference=False):
        return []

    def getargsFormat(self):
        return ""

    def getargsArgs(self, name):
        return None

    def passInput(self, name):
        return self.substitute


class OpaqueType(Type):

    """A type represented by an opaque object type, always passed by address.

    Instantiate with the type name and the names of the new and convert procs.
    If fewer than three arguments are passed, the second argument is used
    to derive the new and convert procs by appending _New and _Convert; it
    defaults to the first argument.
    """

    def __init__(self, name, arg = None, extra = None):
        self.typeName = name
        if extra is None:
             # Two arguments (name, usetype) or one (name)
            arg = arg or name
            self.new = arg + '_New'
            self.convert = arg + '_Convert'
        else:
            # Three arguments (name, new, convert)
            self.new = arg
            self.convert = extra

    def getargsFormat(self):
        return "O&"

    def getargsArgs(self, name):
        return "%s, &%s" % (self.convert, name)

    def passInput(self, name):
        return "&%s" % name

    def mkvalueFormat(self):
        return "O&"

    def mkvalueArgs(self, name):
        return "%s, &%s" % (self.new, name)


class OpaqueByValueType(OpaqueType):

    """A type represented by an opaque object type, on input passed BY VALUE.

    Instantiate with the type name, and optionally an object type name whose
    New/Convert functions will be used.
    """

    def passInput(self, name):
        return name

    def mkvalueArgs(self, name):
        return "%s, %s" % (self.new, name)

class OpaqueByRefType(OpaqueType):
    """An opaque object type, passed by reference.

    Instantiate with the type name, and optionally an object type name whose
    New/Convert functions will be used.
    """

    def passInput(self, name):
        return name

#    def passOutput(self, name):
#        return name

    def mkvalueFormat(self):
        return "O"

    def mkvalueArgs(self, name):
        return "%s(%s)" % (self.new, name)

class OpaqueByValueStructType(OpaqueByValueType):
    """Similar to OpaqueByValueType, but we also pass this to mkvalue by
    address, in stead of by value.
    """

    def mkvalueArgs(self, name):
        return "%s, &%s" % (self.new, name)


class OpaqueArrayType(OpaqueByValueType):

    """A type represented by an opaque object type, with ARRAY passing semantics.

    Instantiate with the type name, and optional an object type name whose
    New/Convert functions will be used.
    """

    def getargsArgs(self, name):
        return "%s, %s" % (self.convert, name)

    def passOutput(self, name):
        return name
