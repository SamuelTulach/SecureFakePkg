## @file
# process Version section generation
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
from .Ffs import SectionSuffix
import Common.LongFilePathOs as os
from .GenFdsGlobalVariable import GenFdsGlobalVariable
from CommonDataClass.FdfClass import VerSectionClassObject
from Common.LongFilePathSupport import OpenLongFilePath as open
from Common.DataType import SUP_MODULE_SEC

## generate version section
#
#
class VerSection (VerSectionClassObject):

    ## The constructor
    #
    #   @param  self        The object pointer
    #
    def __init__(self):
        VerSectionClassObject.__init__(self)

    ## GenSection() method
    #
    #   Generate version section
    #
    #   @param  self        The object pointer
    #   @param  OutputPath  Where to place output file
    #   @param  ModuleName  Which module this section belongs to
    #   @param  SecNum      Index of section
    #   @param  KeyStringList  Filter for inputs of section generation
    #   @param  FfsInf      FfsInfStatement object that contains this section data
    #   @param  Dict        dictionary contains macro and its value
    #   @retval tuple       (Generated file name, section alignment)
    #
    def GenSection(self, OutputPath, ModuleName, SecNum, KeyStringList, FfsInf=None, Dict={}, IsMakefile = False):
        #
        # Prepare the parameter of GenSection
        #
        if FfsInf:
            self.Alignment = FfsInf.__ExtendMacro__(self.Alignment)
            self.BuildNum = FfsInf.__ExtendMacro__(self.BuildNum)
            self.StringData = FfsInf.__ExtendMacro__(self.StringData)
            self.FileName = FfsInf.__ExtendMacro__(self.FileName)

        OutputFile = os.path.join(OutputPath,
                                  ModuleName + SUP_MODULE_SEC + SecNum + SectionSuffix.get('VERSION'))
        OutputFile = os.path.normpath(OutputFile)

        # Get String Data
        StringData = ''
        if self.StringData:
            StringData = self.StringData
        elif self.FileName:
            FileNameStr = GenFdsGlobalVariable.ReplaceWorkspaceMacro(self.FileName)
            FileNameStr = GenFdsGlobalVariable.MacroExtend(FileNameStr, Dict)
            FileObj = open(FileNameStr, 'r')
            StringData = FileObj.read()
            StringData = '"' + StringData + '"'
            FileObj.close()
        GenFdsGlobalVariable.GenerateSection(OutputFile, [], 'EFI_SECTION_VERSION',
                                             Ver=StringData, BuildNumber=self.BuildNum, IsMakefile=IsMakefile)
        OutputFileList = []
        OutputFileList.append(OutputFile)
        return OutputFileList, self.Alignment
