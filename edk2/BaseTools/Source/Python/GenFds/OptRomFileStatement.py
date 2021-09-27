## @file
# process OptionROM generation from FILE statement
#
#  Copyright (c) 2007 - 2018, Intel Corporation. All rights reserved.<BR>
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#

##
# Import Modules
#
from __future__ import absolute_import
import Common.LongFilePathOs as os

from .GenFdsGlobalVariable import GenFdsGlobalVariable
##
#
#
class OptRomFileStatement:
    ## The constructor
    #
    #   @param  self        The object pointer
    #
    def __init__(self):
        self.FileName = None
        self.FileType = None
        self.OverrideAttribs = None

    ## GenFfs() method
    #
    #   Generate FFS
    #
    #   @param  self        The object pointer
    #   @param  Dict        dictionary contains macro and value pair
    #   @retval string      Generated FFS file name
    #
    def GenFfs(self, Dict = {}, IsMakefile=False):

        if self.FileName is not None:
            self.FileName = GenFdsGlobalVariable.ReplaceWorkspaceMacro(self.FileName)

        return self.FileName



