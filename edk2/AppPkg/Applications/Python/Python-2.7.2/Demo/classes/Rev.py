'''
A class which presents the reverse of a sequence without duplicating it.
From: "Steven D. Majewski" <sdm7g@elvis.med.virginia.edu>

It works on mutable or inmutable sequences.

>>> chars = list(Rev('Hello World!'))
>>> print ''.join(chars)
!dlroW olleH

The .forw is so you can use anonymous sequences in __init__, and still
keep a reference the forward sequence. )
If you give it a non-anonymous mutable sequence, the reverse sequence
will track the updated values. ( but not reassignment! - another
good reason to use anonymous values in creating the sequence to avoid
confusion. Maybe it should be change to copy input sequence to break
the connection completely ? )

>>> nnn = range(3)
>>> rnn = Rev(nnn)
>>> for n in rnn: print n
...
2
1
0
>>> for n in range(4, 6): nnn.append(n)   # update nnn
...
>>> for n in rnn: print n     # prints reversed updated values
...
5
4
2
1
0
>>> nnn = nnn[1:-1]
>>> nnn
[1, 2, 4]
>>> for n in rnn: print n     # prints reversed values of old nnn
...
5
4
2
1
0

#
>>> WH = Rev('Hello World!')
>>> print WH.forw, WH.back
Hello World! !dlroW olleH
>>> nnn = Rev(range(1, 10))
>>> print nnn.forw
[1, 2, 3, 4, 5, 6, 7, 8, 9]
>>> print nnn.back
[9, 8, 7, 6, 5, 4, 3, 2, 1]

>>> rrr = Rev(nnn)
>>> rrr
<1, 2, 3, 4, 5, 6, 7, 8, 9>

'''

class Rev:
    def __init__(self, seq):
        self.forw = seq
        self.back = self

    def __len__(self):
        return len(self.forw)

    def __getitem__(self, j):
        return self.forw[-(j + 1)]

    def __repr__(self):
        seq = self.forw
        if isinstance(seq, list):
            wrap = '[]'
            sep = ', '
        elif isinstance(seq, tuple):
            wrap = '()'
            sep = ', '
        elif isinstance(seq, str):
            wrap = ''
            sep = ''
        else:
            wrap = '<>'
            sep = ', '
        outstrs = [str(item) for item in self.back]
        return wrap[:1] + sep.join(outstrs) + wrap[-1:]

def _test():
    import doctest, Rev
    return doctest.testmod(Rev)

if __name__ == "__main__":
    _test()
