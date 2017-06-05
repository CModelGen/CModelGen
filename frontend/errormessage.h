#ifndef __ERRORMESSAGE_H__
#define __ERRORMESSAGE_H__

#include "errorcode.h"

#define ERROR_STRING(NUM) \
(\
(NUM) == 100000 ? "InternalError" : \
(NUM) == 100001 ? "EnvironmentVariableNotSet" : \
(NUM) == 100002 ? "DirectoryAccessError" : \
(NUM) == 100003 ? "ModelGenPathCannotBeRelative" : \
(NUM) == 100004 ? "ExecutionOfCygpathFailed" : \
(NUM) == 100005 ? "CannotCreateTempFile" : \
(NUM) == 100006 ? "CannotCreateFile" : \
(NUM) == 100007 ? "InputNotExist" : \
(NUM) == 100008 ? "OptionCanOnlyBeSpecifiedOnce" : \
(NUM) == 100009 ? "InvalidOption" : \
(NUM) == 100010 ? "InvalidNameOption" : \
(NUM) == 100011 ? "NoInput" : \
(NUM) == 100012 ? "InvalidFileName" : \
(NUM) == 100013 ? "NMakeOnlyForVC" : \
(NUM) == 100014 ? "GMakeNotForCMD" : \
(NUM) == 100015 ? "SConstructNotForVCCygwin" : \
(NUM) == 100016 ? "LoadLibraryError" : \
(NUM) == 100017 ? "LoadSymbolFromLibraryError" : \
(NUM) == 100018 ? "CompilationFailed" : \
(NUM) == 101000 ? "DescriptionCannotBeOpened" : \
(NUM) == 101001 ? "SyntaxError" : \
(NUM) == 101002 ? "UnrecognizedToken" : \
(NUM) == 101003 ? "ASpaceMustExist" : \
(NUM) == 101004 ? "RedefineIsNotAllowed" : \
(NUM) == 101005 ? "FlagRedefineIsNotAllowed" : \
(NUM) == 101006 ? "FlagOverwriteIsNotAllowed" : \
(NUM) == 101007 ? "DoesNotAcceptFlag" : \
(NUM) == 101008 ? "DescSignalNotExist" : \
(NUM) == 101009 ? "DescNameNotExist" : \
(NUM) == 101010 ? "DescParameterCannotBeWritten" : \
(NUM) == 101011 ? "DescTopClassNotExist" : \
(NUM) == 101012 ? "DescMemberClassNotExist" : \
(NUM) == 101013 ? "DescClassNotExist" : \
(NUM) == 101014 ? "DescClassNotTop" : \
(NUM) == 101015 ? "DescClassNotSpecified" : \
(NUM) == 101016 ? "DescErrorInModuleOrSignalName" : \
(NUM) == 101017 ? "DescTopClassDifferentWithTheCurrent" : \
(NUM) == 101018 ? "CannotChangeAnOutputPortIntoAnInputPort" : \
(NUM) == 101019 ? "CannotChangeAnInputPortIntoAnOutputPort" : \
(NUM) == 101020 ? "MultipleDescClass" : \
(NUM) == 101021 ? "PortNameAlreadyUsed" : \
(NUM) == 101022 ? "PortCannotBeCreatedFromArray" : \
(NUM) == 101023 ? "PortDoesNotAcceptArrayImplementationFlags" : \
(NUM) == 101024 ? "SignalAlreadyAssignedToAPort" : \
(NUM) == 101025 ? "CreateAModelPortWithoutAName" : \
(NUM) == 101026 ? "AtLeastOneDirectionForPort" : \
(NUM) == 101027 ? "ImportDescriptionAfterModelDescription" : \
(NUM) == 101028 ? "VariadicArgumentMustBeTheLast" : \
(NUM) == 101029 ? "VariadicArgumentMustNotBeTheFirst" : \
(NUM) == 101030 ? "ArgumentsDoNotMatchTheDescription" : \
(NUM) == 101031 ? "PassByReferenceArgumentCannotBeExpression" : \
(NUM) == 101032 ? "InvalidReturnValueType" : \
(NUM) == 101033 ? "InvalidTaskArgumentDirectionModifier" : \
(NUM) == 101034 ? "InvalidTaskArgumentForDirection" : \
(NUM) == 102000 ? "EscapeCharShouldBeExactlyNCharacter" : \
(NUM) == 102001 ? "EscapeCharsShouldNotBeTheSame" : \
(NUM) == 102002 ? "EscapeCharMustBeInPairs" : \
(NUM) == 102003 ? "UnknownTokenInSpecs" : \
(NUM) == 102004 ? "UnknownParameter" : \
(NUM) == 102005 ? "FailedToEvaluateSpecs" : \
(NUM) == 102006 ? "SpecOnlyAcceptBinaryValues" : \
(NUM) == 102007 ? "ErrorInListExpansion" : \
(NUM) == 102008 ? "ParameterNameErrorNumberFirst" : \
(NUM) == 102009 ? "ParameterNameErrorNoEnglish" : \
(NUM) == 102010 ? "ParameterNameErrorReserved" : \
(NUM) == 102011 ? "ParameterNameErrorInvalidCharacter" : \
(NUM) == 103000 ? "HDLProcessingFailed" : \
(NUM) == 103001 ? "IcarusNoOutputFile" : \
(NUM) == 103002 ? "WarningMaskIllegal" : \
(NUM) == 103003 ? "FileCreationFailed" : \
(NUM) == 103004 ? "StringFormatError" : \
(NUM) == 103005 ? "StringFormatMissingOperand" : \
(NUM) == 103006 ? "UnsupportedSystemTask" : \
(NUM) == 103007 ? "UnsupportedSystemTaskUnknownPlace" : \
(NUM) == 103008 ? "SystemTaskCalledAsFunction" : \
(NUM) == 103009 ? "SystemTaskFunctionArgumentError" : \
(NUM) == 103010 ? "UnsupportedStatement" : \
(NUM) == 104000 ? "ImmediateUnsupportedState" : \
(NUM) == 104001 ? "UnsupportedStateGenerated" : \
(NUM) == 104002 ? "UnsupportedUnknownValueGenerated" : \
(NUM) == 104003 ? "UnsupportedUnknownValueGeneratedAt" : \
(NUM) == 105000 ? "RealNumber" : \
(NUM) == 105001 ? "WaitFound" : \
(NUM) == 105002 ? "RepeatFound" : \
(NUM) == 105003 ? "PowerError" : \
(NUM) == 105004 ? "Tri0Tri1" : \
(NUM) == 200000 ? "ModifyAlreadyConnectedToAPort" : \
(NUM) == 201000 ? "SystemFunctionCalledAsATask" : \
(NUM) == 201001 ? "DelayFound" : \
(NUM) == 201002 ? "DelayExpressionFound" : \
(NUM) == 201003 ? "WaitExpressionFound" : \
(NUM) == 201004 ? "DisableIgnored" : \
(NUM) == 300000 ? "WidthUnmatch" : \
(NUM) == 300001 ? "WidthNotIdentical" : \
(NUM) == 300002 ? "WriteOnlyVariableIsRead" : \
(NUM) == 300003 ? "ReadOnlyVariableIsWritten" : \
(NUM) == 300004 ? "VariableNeedsGreaterNoSV" : \
(NUM) == 300005 ? "BitUnrecognizable" : \
(NUM) == 300006 ? "OffsetTooLarge" : \
(NUM) == 300007 ? "EndSmallerThanBegin" : \
(NUM) == 300008 ? "TooManyFileDescriptorCreated" : \
(NUM) == 400000 ? "ErrorInFormatSpecification" : \
(NUM) == 400001 ? "FormatWidthTooLarge" : \
(NUM) == 400002 ? "FileCannotBeOpened" : \
(NUM) == 400003 ? "FileDescriptorIsNotValid" : \
(NUM) == 400004 ? "MultiChannelDescriptorIsNotValid" : \
(NUM) == 400005 ? "MultiChannelDescriptorTooShort" : \
(NUM) == 400006 ? "AllChannelsAreUsed" : \
(NUM) == 400007 ? "ErrorStringIsNotWideEnough" : \
(NUM) == 400008 ? "OperationInvalid" : \
(NUM) == 400009 ? "ReadMemFailedToOpenFile" : \
(NUM) == 400010 ? "ReadMemUnterminatedComment" : \
(NUM) == 400011 ? "ReadMemIllegalEntry" : \
(NUM) == 400012 ? "ReadMemOutOfBound" : \
(NUM) == 400013 ? "ReadMemOutOfBoundEntry" : \
(NUM) == 400014 ? "ReadMemStartOutOfBound" : \
(NUM) == 400015 ? "ReadMemTooFewElements" : \
NULL)
#define ERROR_MESSAGE_InternalError "An internal error has happened. Please report a bug to us."
#define ERROR_MESSAGE_EnvironmentVariableNotSet "The environment variable \"%s\" is not set."
#define ERROR_MESSAGE_DirectoryAccessError "An error has happened when accessing the directory %s. The directory must have read, write and access permissions."
#define ERROR_MESSAGE_ModelGenPathCannotBeRelative "The value of the environment variable CMODELGEN_PATH cannot be a relative path."
#define ERROR_MESSAGE_ExecutionOfCygpathFailed "The execution of a cygpath command has failed. The executed command is \"%s\"."
#define ERROR_MESSAGE_CannotCreateTempFile "Unable to create a temporary file in directory \"%s\"."
#define ERROR_MESSAGE_CannotCreateFile "Unable to create a file \"%s\"."
#define ERROR_MESSAGE_InputNotExist "The input file \"%s\" does not exist."
#define ERROR_MESSAGE_OptionCanOnlyBeSpecifiedOnce "The command-line option \"%s\" can only be specified once."
#define ERROR_MESSAGE_InvalidOption "The command-line option \"%s\" is invalid."
#define ERROR_MESSAGE_InvalidNameOption "A name must be provided after the option \"%s\"."
#define ERROR_MESSAGE_NoInput "At least one input file must be specified."
#define ERROR_MESSAGE_InvalidFileName "The file name \"%s\" is invalid."
#define ERROR_MESSAGE_NMakeOnlyForVC "Cannot generate an NMake-compatible makefile when the compiler is not Visual C++."
#define ERROR_MESSAGE_GMakeNotForCMD "Cannot generate a GMake-compatible makefile when the command-line system is Command Prompt."
#define ERROR_MESSAGE_SConstructNotForVCCygwin "Cannot generate a SCons build script when the use mode is VC-Cygwin."
#define ERROR_MESSAGE_LoadLibraryError "Unable to load the library \"%s\"."
#define ERROR_MESSAGE_LoadSymbolFromLibraryError "Unable to find the symbol \"%s\" from the library \"%s\"."
#define ERROR_MESSAGE_CompilationFailed "An error has happened during compilation."
#define ERROR_MESSAGE_DescriptionCannotBeOpened "Unable to open the model description file \"%s\"."
#define ERROR_MESSAGE_SyntaxError "[%s:%d] A syntax error has been found."
#define ERROR_MESSAGE_UnrecognizedToken "[%s:%d] The token \"%s\" is unrecognized."
#define ERROR_MESSAGE_ASpaceMustExist "[%s:%d] A space, tab or newline character must be inserted in between the ']' character and the token \"%s\"."
#define ERROR_MESSAGE_RedefineIsNotAllowed "[%s:%d] Redefining \"%s\" is not allowed. The previous definition is at %s:%d."
#define ERROR_MESSAGE_FlagRedefineIsNotAllowed "[%s:%d] The flag \"%s\" has been used more than once."
#define ERROR_MESSAGE_FlagOverwriteIsNotAllowed "[%s:%d] Overwritting a flag \"%s\" using a flag \"%s\" is not allowed."
#define ERROR_MESSAGE_DoesNotAcceptFlag "[%s:%d] The flag \"%s\" is not accepted in a %s definition statement."
#define ERROR_MESSAGE_DescSignalNotExist "[%s:%d] The variable, net or parameter \"%s\" is not found."
#define ERROR_MESSAGE_DescNameNotExist "[%s:%d] The scope named \"%s\" is not found."
#define ERROR_MESSAGE_DescParameterCannotBeWritten "[%s:%d] The parameter \"%s\" cannot be written."
#define ERROR_MESSAGE_DescTopClassNotExist "[%s:%d] The module named \"%s\" does not exist or is not a top-level module."
#define ERROR_MESSAGE_DescMemberClassNotExist "[%s:%d] The scope named \"%s\" cannot be found in the scope %s."
#define ERROR_MESSAGE_DescClassNotExist "[%s:%d] The module named \"%s\" does not exist."
#define ERROR_MESSAGE_DescClassNotTop "[%s:%d] The module named \"%s\" is not a top-level module."
#define ERROR_MESSAGE_DescClassNotSpecified "[%s:%d] A module name must be specified."
#define ERROR_MESSAGE_DescErrorInModuleOrSignalName "[%s:%d] The name \"%s\" is invalid."
#define ERROR_MESSAGE_DescTopClassDifferentWithTheCurrent "[%s:%d] Another top-level module \"%s\" is used in the model description construct of the top-level module \"%s\"."
#define ERROR_MESSAGE_CannotChangeAnOutputPortIntoAnInputPort "[%s:%d] Cannot create an input model port from an output port."
#define ERROR_MESSAGE_CannotChangeAnInputPortIntoAnOutputPort "[%s:%d] Cannot create an output model port from an input port."
#define ERROR_MESSAGE_MultipleDescClass "[%s:%d] Describing module \"%s\" multiple times is not allowed. The last description is at %s:%d."
#define ERROR_MESSAGE_PortNameAlreadyUsed "[%s:%d] The model port name \"%s\" is already used for \"%s\" at %s:%d."
#define ERROR_MESSAGE_PortCannotBeCreatedFromArray "[%s:%d] Cannot create a model port from an array \"%s\"."
#define ERROR_MESSAGE_PortDoesNotAcceptArrayImplementationFlags "[%s:%d] A port description statement does not accept the flag \"%s\"."
#define ERROR_MESSAGE_SignalAlreadyAssignedToAPort "[%s:%d] The variable, net or parameter named \"%s\" is already connected to a model port \"%s\" defined at %s:%d."
#define ERROR_MESSAGE_CreateAModelPortWithoutAName "[%s:%d] To create a model port from \"%s\", a name must be specified."
#define ERROR_MESSAGE_AtLeastOneDirectionForPort "[%s:%d] The direction of the model port \"%s\" is not specified."
#define ERROR_MESSAGE_ImportDescriptionAfterModelDescription "[%s:%d] The import statement must be moved to before the model description at %s:%d."
#define ERROR_MESSAGE_VariadicArgumentMustBeTheLast "[%s:%d] The variadic argument '...' must be the last argument."
#define ERROR_MESSAGE_VariadicArgumentMustNotBeTheFirst "[%s:%d] There must be at least one argument before the variadic argument '...'."
#define ERROR_MESSAGE_ArgumentsDoNotMatchTheDescription "[%s:%d] The argument(s) of the imported function/task \"%s\" do(es) not match the import description at %s:%d."
#define ERROR_MESSAGE_PassByReferenceArgumentCannotBeExpression "[%s:%d] An output/inout argument of the imported function/task \"%s\" cannot be an expression."
#define ERROR_MESSAGE_InvalidReturnValueType "[%s:%d] The return type \"%s\" of the function import statement is invalid."
#define ERROR_MESSAGE_InvalidTaskArgumentDirectionModifier "[%s:%d] The argument direction modifier \"%s\" of the task import statement is invalid."
#define ERROR_MESSAGE_InvalidTaskArgumentForDirection "[%s:%d] The argument type \"%s\" of the task import statement cannot be used when the direction modifier is \"%s\"."
#define ERROR_MESSAGE_EscapeCharShouldBeExactlyNCharacter "There should be exactly 3 escape characters. Modifying ESCAPECHARS into \"%s\" is not allowed."
#define ERROR_MESSAGE_EscapeCharsShouldNotBeTheSame "Escape characters '%c', '%c' and '%c' should be three different chracters."
#define ERROR_MESSAGE_EscapeCharMustBeInPairs "The '%c' escape characters should appear in pairs in the parameter \"%s=%s\"."
#define ERROR_MESSAGE_UnknownTokenInSpecs "Token \"%s\" is unknown in the parameter \"%s=%s\"."
#define ERROR_MESSAGE_UnknownParameter "The value of parameter \"%s\" is \"%s\". In this value, there is an unrecognized parameter \"%s\"."
#define ERROR_MESSAGE_FailedToEvaluateSpecs "Failed to evaluate the parameter \"%s=%s\"."
#define ERROR_MESSAGE_SpecOnlyAcceptBinaryValues "The value of the parameter \"%s\" can only be 0 or 1."
#define ERROR_MESSAGE_ErrorInListExpansion "An error has happened on evaluating the parameter \"%s\". The value of the parameter is \"%s\", and the error happens on expanding \"%s\"."
#define ERROR_MESSAGE_ParameterNameErrorNumberFirst "Cannot create a new parameter named \"%s\". The name must begin with an English character or an underscore."
#define ERROR_MESSAGE_ParameterNameErrorNoEnglish "Cannot create a new parameter named \"%s\". The name must contain at least an English character."
#define ERROR_MESSAGE_ParameterNameErrorReserved "Cannot create a new parameter named \"%s\". The name is reserved."
#define ERROR_MESSAGE_ParameterNameErrorInvalidCharacter "Cannot create a new parameter named \"%s\". The name must only consists of English characters, digits and underscores."
#define ERROR_MESSAGE_HDLProcessingFailed "An error has happened during HDL processing."
#define ERROR_MESSAGE_IcarusNoOutputFile "An output is not specified with the -o option."
#define ERROR_MESSAGE_WarningMaskIllegal "The warning mask \"%s\" is illegal. The legal form is 0xHHHHHHHH, while H is an upper hexdecimal number."
#define ERROR_MESSAGE_FileCreationFailed "Creating the output file \"%s\" has failed."
#define ERROR_MESSAGE_StringFormatError "[%s:%d] An error is found in the format string."
#define ERROR_MESSAGE_StringFormatMissingOperand "[%s:%d] The format string requires more operands than provided."
#define ERROR_MESSAGE_UnsupportedSystemTask "[%s:%d] The system task/function \"%s\" is not supported."
#define ERROR_MESSAGE_UnsupportedSystemTaskUnknownPlace "The system task/function \"%s\" is not supported."
#define ERROR_MESSAGE_SystemTaskCalledAsFunction "[%s:%d] The system task \"%s\" is called as a function."
#define ERROR_MESSAGE_SystemTaskFunctionArgumentError "[%s:%d] The system task/function \"%s\" is invoked with incompatible arguments."
#define ERROR_MESSAGE_UnsupportedStatement "[%s:%d] The statement is unsupported."
#define ERROR_MESSAGE_ImmediateUnsupportedState "[%s:%d] The immediate number contains an unsupported value \'%c\'. Please increase the NoSV to solve this problem."
#define ERROR_MESSAGE_UnsupportedStateGenerated "[%s:%d] The statement generates an unsupported value \'%c\'. Please increase the NoSV to solve this problem."
#define ERROR_MESSAGE_UnsupportedUnknownValueGenerated "An unsupported unknown value has been generated."
#define ERROR_MESSAGE_UnsupportedUnknownValueGeneratedAt "[%s:%d] An unsupported unknown value has been generated."
#define ERROR_MESSAGE_RealNumber "[%s:%d] The real data type is not supported."
#define ERROR_MESSAGE_WaitFound "[%s:%d] The wait statement is not supported."
#define ERROR_MESSAGE_RepeatFound "[%s:%d] The repeat statement is not supported."
#define ERROR_MESSAGE_PowerError "[%s:%d] The power expression is not supported unless its first operand is the immediate number 2."
#define ERROR_MESSAGE_Tri0Tri1 "[%s:%d] Declaring tri0 or tri1 nets is not supported."
#define ERROR_MESSAGE_ModifyAlreadyConnectedToAPort "At %s:%d: A model port is created from %s, therefore the modification to it is ignored."
#define ERROR_MESSAGE_SystemFunctionCalledAsATask "[%s:%d] The return value of the system function %s has not been used."
#define ERROR_MESSAGE_DelayFound "[%s:%d] The delay statement is ignored."
#define ERROR_MESSAGE_DelayExpressionFound "[%s:%d] The delay expression is ignored."
#define ERROR_MESSAGE_WaitExpressionFound "[%s:%d] The wait expression is ignored."
#define ERROR_MESSAGE_DisableIgnored "[%s:%d] The disable statement is ignored because it does not disable its enclosing block."
#define ERROR_MESSAGE_WidthUnmatch "Unmatching width detected in function \"%s\"."
#define ERROR_MESSAGE_WidthNotIdentical "The widths of the source cms_variables are not identical. Happened in function \"%s\"."
#define ERROR_MESSAGE_WriteOnlyVariableIsRead "A write-only cms_variable is read."
#define ERROR_MESSAGE_ReadOnlyVariableIsWritten "A read-only cms_variable is written."
#define ERROR_MESSAGE_VariableNeedsGreaterNoSV "The NoSV of a cms_variable should be at least %d. Happened in function %s."
#define ERROR_MESSAGE_BitUnrecognizable "Bit '%c' is unrecognizable. Happended in function %s."
#define ERROR_MESSAGE_OffsetTooLarge "The offset %d used to access a bit of a cms_variable is not smaller than the cms_variable's width %d. Happended in function %s."
#define ERROR_MESSAGE_EndSmallerThanBegin "The end offset %d is smaller than the begin offset %d. Happended in function %s."
#define ERROR_MESSAGE_TooManyFileDescriptorCreated "Too many file descriptors are created."
#define ERROR_MESSAGE_ErrorInFormatSpecification "An error has happened when parsing the character '%c' in the format specification \"%s\"."
#define ERROR_MESSAGE_FormatWidthTooLarge "The width in the format specification \"%s\" is too large."
#define ERROR_MESSAGE_FileCannotBeOpened "The file \"%s\" cannot be opened."
#define ERROR_MESSAGE_FileDescriptorIsNotValid "The file descriptor used in the function \"%s\" is invalid."
#define ERROR_MESSAGE_MultiChannelDescriptorIsNotValid "The multichannel descriptor used in the function \"%s\" is invalid."
#define ERROR_MESSAGE_MultiChannelDescriptorTooShort "The width of the destination multichannel descriptor used in the function \"%s\" should be no less than 32."
#define ERROR_MESSAGE_AllChannelsAreUsed "Cannot create a multichannel descriptor because all channels are in use."
#define ERROR_MESSAGE_ErrorStringIsNotWideEnough "In function \"%s\", the cms_variable pointed to by the third argument should has at least 640 bits."
#define ERROR_MESSAGE_OperationInvalid "In cms_$fseek, the value of the cms_variable pointed to by the operation argument must be 0, 1 or 2."
#define ERROR_MESSAGE_ReadMemFailedToOpenFile "In function \"%s\", opening the file named \"%s\" has failed."
#define ERROR_MESSAGE_ReadMemUnterminatedComment "In function \"%s\", the file \"%s\" ends with an unterminated comment."
#define ERROR_MESSAGE_ReadMemIllegalEntry "In function \"%s\", the file \"%s\" has an illegal entry at line %d."
#define ERROR_MESSAGE_ReadMemOutOfBound "In function \"%s\", the file \"%s\" has an out-of-bounds address at line %d."
#define ERROR_MESSAGE_ReadMemOutOfBoundEntry "In function \"%s\", the file \"%s\" has an entry with an out-of-bounds address at line %d."
#define ERROR_MESSAGE_ReadMemStartOutOfBound "In function \"%s\", the start address is out-of-bounds."
#define ERROR_MESSAGE_ReadMemTooFewElements "In function \"%s\", the file \"%s\" has too few elements than required."

#ifndef CMSL_INTERNAL_H

#define ERRORCODE_InternalError 100000
#define ERROR_InternalError()\
{\
fprintf(stderr, "Error 100000: An internal error has happened. Please report a bug to us.\n"); \
exit(100000);}

#define ERRORCODE_EnvironmentVariableNotSet 100001
#define ERROR_EnvironmentVariableNotSet(ARG0)\
{\
fprintf(stderr, "Error 100001: The environment variable \"%s\" is not set.\n", ARG0); \
exit(100001);}

#define ERRORCODE_DirectoryAccessError 100002
#define ERROR_DirectoryAccessError(ARG0)\
{\
fprintf(stderr, "Error 100002: An error has happened when accessing the directory %s. The directory must have read, write and access permissions.\n", ARG0); \
exit(100002);}

#define ERRORCODE_ModelGenPathCannotBeRelative 100003
#define ERROR_ModelGenPathCannotBeRelative()\
{\
fprintf(stderr, "Error 100003: The value of the environment variable CMODELGEN_PATH cannot be a relative path.\n"); \
exit(100003);}

#define ERRORCODE_ExecutionOfCygpathFailed 100004
#define ERROR_ExecutionOfCygpathFailed(ARG0)\
{\
fprintf(stderr, "Error 100004: The execution of a cygpath command has failed. The executed command is \"%s\".\n", ARG0); \
exit(100004);}

#define ERRORCODE_CannotCreateTempFile 100005
#define ERROR_CannotCreateTempFile(ARG0)\
{\
fprintf(stderr, "Error 100005: Unable to create a temporary file in directory \"%s\".\n", ARG0); \
exit(100005);}

#define ERRORCODE_CannotCreateFile 100006
#define ERROR_CannotCreateFile(ARG0)\
{\
fprintf(stderr, "Error 100006: Unable to create a file \"%s\".\n", ARG0); \
exit(100006);}

#define ERRORCODE_InputNotExist 100007
#define ERROR_InputNotExist(ARG0)\
{\
fprintf(stderr, "Error 100007: The input file \"%s\" does not exist.\n", ARG0); \
exit(100007);}

#define ERRORCODE_OptionCanOnlyBeSpecifiedOnce 100008
#define ERROR_OptionCanOnlyBeSpecifiedOnce(ARG0)\
{\
fprintf(stderr, "Error 100008: The command-line option \"%s\" can only be specified once.\n", ARG0); \
exit(100008);}

#define ERRORCODE_InvalidOption 100009
#define ERROR_InvalidOption(ARG0)\
{\
fprintf(stderr, "Error 100009: The command-line option \"%s\" is invalid.\n", ARG0); \
exit(100009);}

#define ERRORCODE_InvalidNameOption 100010
#define ERROR_InvalidNameOption(ARG0)\
{\
fprintf(stderr, "Error 100010: A name must be provided after the option \"%s\".\n", ARG0); \
exit(100010);}

#define ERRORCODE_NoInput 100011
#define ERROR_NoInput()\
{\
fprintf(stderr, "Error 100011: At least one input file must be specified.\n"); \
exit(100011);}

#define ERRORCODE_InvalidFileName 100012
#define ERROR_InvalidFileName(ARG0)\
{\
fprintf(stderr, "Error 100012: The file name \"%s\" is invalid.\n", ARG0); \
exit(100012);}

#define ERRORCODE_NMakeOnlyForVC 100013
#define ERROR_NMakeOnlyForVC()\
{\
fprintf(stderr, "Error 100013: Cannot generate an NMake-compatible makefile when the compiler is not Visual C++.\n"); \
exit(100013);}

#define ERRORCODE_GMakeNotForCMD 100014
#define ERROR_GMakeNotForCMD()\
{\
fprintf(stderr, "Error 100014: Cannot generate a GMake-compatible makefile when the command-line system is Command Prompt.\n"); \
exit(100014);}

#define ERRORCODE_SConstructNotForVCCygwin 100015
#define ERROR_SConstructNotForVCCygwin()\
{\
fprintf(stderr, "Error 100015: Cannot generate a SCons build script when the use mode is VC-Cygwin.\n"); \
exit(100015);}

#define ERRORCODE_LoadLibraryError 100016
#define ERROR_LoadLibraryError(ARG0)\
{\
fprintf(stderr, "Error 100016: Unable to load the library \"%s\".\n", ARG0); \
exit(100016);}

#define ERRORCODE_LoadSymbolFromLibraryError 100017
#define ERROR_LoadSymbolFromLibraryError(ARG0,ARG1)\
{\
fprintf(stderr, "Error 100017: Unable to find the symbol \"%s\" from the library \"%s\".\n", ARG0, ARG1); \
exit(100017);}

#define ERRORCODE_CompilationFailed 100018
#define ERROR_CompilationFailed()\
{\
fprintf(stderr, "Error 100018: An error has happened during compilation.\n"); \
exit(100018);}

#define ERRORCODE_DescriptionCannotBeOpened 101000
#define ERROR_DescriptionCannotBeOpened(ARG0)\
{\
fprintf(stderr, "Error 101000: Unable to open the model description file \"%s\".\n", ARG0); \
exit(101000);}

#define ERRORCODE_SyntaxError 101001
#define ERROR_SyntaxError(ARG0,ARG1)\
{\
fprintf(stderr, "Error 101001: [%s:%d] A syntax error has been found.\n", ARG0, ARG1); \
exit(101001);}

#define ERRORCODE_UnrecognizedToken 101002
#define ERROR_UnrecognizedToken(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101002: [%s:%d] The token \"%s\" is unrecognized.\n", ARG0, ARG1, ARG2); \
exit(101002);}

#define ERRORCODE_ASpaceMustExist 101003
#define ERROR_ASpaceMustExist(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101003: [%s:%d] A space, tab or newline character must be inserted in between the ']' character and the token \"%s\".\n", ARG0, ARG1, ARG2); \
exit(101003);}

#define ERRORCODE_RedefineIsNotAllowed 101004
#define ERROR_RedefineIsNotAllowed(ARG0,ARG1,ARG2,ARG3,ARG4)\
{\
fprintf(stderr, "Error 101004: [%s:%d] Redefining \"%s\" is not allowed. The previous definition is at %s:%d.\n", ARG0, ARG1, ARG2, ARG3, ARG4); \
exit(101004);}

#define ERRORCODE_FlagRedefineIsNotAllowed 101005
#define ERROR_FlagRedefineIsNotAllowed(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101005: [%s:%d] The flag \"%s\" has been used more than once.\n", ARG0, ARG1, ARG2); \
exit(101005);}

#define ERRORCODE_FlagOverwriteIsNotAllowed 101006
#define ERROR_FlagOverwriteIsNotAllowed(ARG0,ARG1,ARG2,ARG3)\
{\
fprintf(stderr, "Error 101006: [%s:%d] Overwritting a flag \"%s\" using a flag \"%s\" is not allowed.\n", ARG0, ARG1, ARG2, ARG3); \
exit(101006);}

#define ERRORCODE_DoesNotAcceptFlag 101007
#define ERROR_DoesNotAcceptFlag(ARG0,ARG1,ARG2,ARG3)\
{\
fprintf(stderr, "Error 101007: [%s:%d] The flag \"%s\" is not accepted in a %s definition statement.\n", ARG0, ARG1, ARG2, ARG3); \
exit(101007);}

#define ERRORCODE_DescSignalNotExist 101008
#define ERROR_DescSignalNotExist(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101008: [%s:%d] The variable, net or parameter \"%s\" is not found.\n", ARG0, ARG1, ARG2); \
exit(101008);}

#define ERRORCODE_DescNameNotExist 101009
#define ERROR_DescNameNotExist(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101009: [%s:%d] The scope named \"%s\" is not found.\n", ARG0, ARG1, ARG2); \
exit(101009);}

#define ERRORCODE_DescParameterCannotBeWritten 101010
#define ERROR_DescParameterCannotBeWritten(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101010: [%s:%d] The parameter \"%s\" cannot be written.\n", ARG0, ARG1, ARG2); \
exit(101010);}

#define ERRORCODE_DescTopClassNotExist 101011
#define ERROR_DescTopClassNotExist(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101011: [%s:%d] The module named \"%s\" does not exist or is not a top-level module.\n", ARG0, ARG1, ARG2); \
exit(101011);}

#define ERRORCODE_DescMemberClassNotExist 101012
#define ERROR_DescMemberClassNotExist(ARG0,ARG1,ARG2,ARG3)\
{\
fprintf(stderr, "Error 101012: [%s:%d] The scope named \"%s\" cannot be found in the scope %s.\n", ARG0, ARG1, ARG2, ARG3); \
exit(101012);}

#define ERRORCODE_DescClassNotExist 101013
#define ERROR_DescClassNotExist(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101013: [%s:%d] The module named \"%s\" does not exist.\n", ARG0, ARG1, ARG2); \
exit(101013);}

#define ERRORCODE_DescClassNotTop 101014
#define ERROR_DescClassNotTop(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101014: [%s:%d] The module named \"%s\" is not a top-level module.\n", ARG0, ARG1, ARG2); \
exit(101014);}

#define ERRORCODE_DescClassNotSpecified 101015
#define ERROR_DescClassNotSpecified(ARG0,ARG1)\
{\
fprintf(stderr, "Error 101015: [%s:%d] A module name must be specified.\n", ARG0, ARG1); \
exit(101015);}

#define ERRORCODE_DescErrorInModuleOrSignalName 101016
#define ERROR_DescErrorInModuleOrSignalName(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101016: [%s:%d] The name \"%s\" is invalid.\n", ARG0, ARG1, ARG2); \
exit(101016);}

#define ERRORCODE_DescTopClassDifferentWithTheCurrent 101017
#define ERROR_DescTopClassDifferentWithTheCurrent(ARG0,ARG1,ARG2,ARG3)\
{\
fprintf(stderr, "Error 101017: [%s:%d] Another top-level module \"%s\" is used in the model description construct of the top-level module \"%s\".\n", ARG0, ARG1, ARG2, ARG3); \
exit(101017);}

#define ERRORCODE_CannotChangeAnOutputPortIntoAnInputPort 101018
#define ERROR_CannotChangeAnOutputPortIntoAnInputPort(ARG0,ARG1)\
{\
fprintf(stderr, "Error 101018: [%s:%d] Cannot create an input model port from an output port.\n", ARG0, ARG1); \
exit(101018);}

#define ERRORCODE_CannotChangeAnInputPortIntoAnOutputPort 101019
#define ERROR_CannotChangeAnInputPortIntoAnOutputPort(ARG0,ARG1)\
{\
fprintf(stderr, "Error 101019: [%s:%d] Cannot create an output model port from an input port.\n", ARG0, ARG1); \
exit(101019);}

#define ERRORCODE_MultipleDescClass 101020
#define ERROR_MultipleDescClass(ARG0,ARG1,ARG2,ARG3,ARG4)\
{\
fprintf(stderr, "Error 101020: [%s:%d] Describing module \"%s\" multiple times is not allowed. The last description is at %s:%d.\n", ARG0, ARG1, ARG2, ARG3, ARG4); \
exit(101020);}

#define ERRORCODE_PortNameAlreadyUsed 101021
#define ERROR_PortNameAlreadyUsed(ARG0,ARG1,ARG2,ARG3,ARG4,ARG5)\
{\
fprintf(stderr, "Error 101021: [%s:%d] The model port name \"%s\" is already used for \"%s\" at %s:%d.\n", ARG0, ARG1, ARG2, ARG3, ARG4, ARG5); \
exit(101021);}

#define ERRORCODE_PortCannotBeCreatedFromArray 101022
#define ERROR_PortCannotBeCreatedFromArray(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101022: [%s:%d] Cannot create a model port from an array \"%s\".\n", ARG0, ARG1, ARG2); \
exit(101022);}

#define ERRORCODE_PortDoesNotAcceptArrayImplementationFlags 101023
#define ERROR_PortDoesNotAcceptArrayImplementationFlags(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101023: [%s:%d] A port description statement does not accept the flag \"%s\".\n", ARG0, ARG1, ARG2); \
exit(101023);}

#define ERRORCODE_SignalAlreadyAssignedToAPort 101024
#define ERROR_SignalAlreadyAssignedToAPort(ARG0,ARG1,ARG2,ARG3,ARG4,ARG5)\
{\
fprintf(stderr, "Error 101024: [%s:%d] The variable, net or parameter named \"%s\" is already connected to a model port \"%s\" defined at %s:%d.\n", ARG0, ARG1, ARG2, ARG3, ARG4, ARG5); \
exit(101024);}

#define ERRORCODE_CreateAModelPortWithoutAName 101025
#define ERROR_CreateAModelPortWithoutAName(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101025: [%s:%d] To create a model port from \"%s\", a name must be specified.\n", ARG0, ARG1, ARG2); \
exit(101025);}

#define ERRORCODE_AtLeastOneDirectionForPort 101026
#define ERROR_AtLeastOneDirectionForPort(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101026: [%s:%d] The direction of the model port \"%s\" is not specified.\n", ARG0, ARG1, ARG2); \
exit(101026);}

#define ERRORCODE_ImportDescriptionAfterModelDescription 101027
#define ERROR_ImportDescriptionAfterModelDescription(ARG0,ARG1,ARG2,ARG3)\
{\
fprintf(stderr, "Error 101027: [%s:%d] The import statement must be moved to before the model description at %s:%d.\n", ARG0, ARG1, ARG2, ARG3); \
exit(101027);}

#define ERRORCODE_VariadicArgumentMustBeTheLast 101028
#define ERROR_VariadicArgumentMustBeTheLast(ARG0,ARG1)\
{\
fprintf(stderr, "Error 101028: [%s:%d] The variadic argument '...' must be the last argument.\n", ARG0, ARG1); \
exit(101028);}

#define ERRORCODE_VariadicArgumentMustNotBeTheFirst 101029
#define ERROR_VariadicArgumentMustNotBeTheFirst(ARG0,ARG1)\
{\
fprintf(stderr, "Error 101029: [%s:%d] There must be at least one argument before the variadic argument '...'.\n", ARG0, ARG1); \
exit(101029);}

#define ERRORCODE_ArgumentsDoNotMatchTheDescription 101030
#define ERROR_ArgumentsDoNotMatchTheDescription(ARG0,ARG1,ARG2,ARG3,ARG4)\
{\
fprintf(stderr, "Error 101030: [%s:%d] The argument(s) of the imported function/task \"%s\" do(es) not match the import description at %s:%d.\n", ARG0, ARG1, ARG2, ARG3, ARG4); \
exit(101030);}

#define ERRORCODE_PassByReferenceArgumentCannotBeExpression 101031
#define ERROR_PassByReferenceArgumentCannotBeExpression(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101031: [%s:%d] An output/inout argument of the imported function/task \"%s\" cannot be an expression.\n", ARG0, ARG1, ARG2); \
exit(101031);}

#define ERRORCODE_InvalidReturnValueType 101032
#define ERROR_InvalidReturnValueType(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101032: [%s:%d] The return type \"%s\" of the function import statement is invalid.\n", ARG0, ARG1, ARG2); \
exit(101032);}

#define ERRORCODE_InvalidTaskArgumentDirectionModifier 101033
#define ERROR_InvalidTaskArgumentDirectionModifier(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 101033: [%s:%d] The argument direction modifier \"%s\" of the task import statement is invalid.\n", ARG0, ARG1, ARG2); \
exit(101033);}

#define ERRORCODE_InvalidTaskArgumentForDirection 101034
#define ERROR_InvalidTaskArgumentForDirection(ARG0,ARG1,ARG2,ARG3)\
{\
fprintf(stderr, "Error 101034: [%s:%d] The argument type \"%s\" of the task import statement cannot be used when the direction modifier is \"%s\".\n", ARG0, ARG1, ARG2, ARG3); \
exit(101034);}

#define ERRORCODE_EscapeCharShouldBeExactlyNCharacter 102000
#define ERROR_EscapeCharShouldBeExactlyNCharacter(ARG0)\
{\
fprintf(stderr, "Error 102000: There should be exactly 3 escape characters. Modifying ESCAPECHARS into \"%s\" is not allowed.\n", ARG0); \
exit(102000);}

#define ERRORCODE_EscapeCharsShouldNotBeTheSame 102001
#define ERROR_EscapeCharsShouldNotBeTheSame(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 102001: Escape characters '%c', '%c' and '%c' should be three different chracters.\n", ARG0, ARG1, ARG2); \
exit(102001);}

#define ERRORCODE_EscapeCharMustBeInPairs 102002
#define ERROR_EscapeCharMustBeInPairs(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 102002: The '%c' escape characters should appear in pairs in the parameter \"%s=%s\".\n", ARG0, ARG1, ARG2); \
exit(102002);}

#define ERRORCODE_UnknownTokenInSpecs 102003
#define ERROR_UnknownTokenInSpecs(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 102003: Token \"%s\" is unknown in the parameter \"%s=%s\".\n", ARG0, ARG1, ARG2); \
exit(102003);}

#define ERRORCODE_UnknownParameter 102004
#define ERROR_UnknownParameter(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 102004: The value of parameter \"%s\" is \"%s\". In this value, there is an unrecognized parameter \"%s\".\n", ARG0, ARG1, ARG2); \
exit(102004);}

#define ERRORCODE_FailedToEvaluateSpecs 102005
#define ERROR_FailedToEvaluateSpecs(ARG0,ARG1)\
{\
fprintf(stderr, "Error 102005: Failed to evaluate the parameter \"%s=%s\".\n", ARG0, ARG1); \
exit(102005);}

#define ERRORCODE_SpecOnlyAcceptBinaryValues 102006
#define ERROR_SpecOnlyAcceptBinaryValues(ARG0)\
{\
fprintf(stderr, "Error 102006: The value of the parameter \"%s\" can only be 0 or 1.\n", ARG0); \
exit(102006);}

#define ERRORCODE_ErrorInListExpansion 102007
#define ERROR_ErrorInListExpansion(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 102007: An error has happened on evaluating the parameter \"%s\". The value of the parameter is \"%s\", and the error happens on expanding \"%s\".\n", ARG0, ARG1, ARG2); \
exit(102007);}

#define ERRORCODE_ParameterNameErrorNumberFirst 102008
#define ERROR_ParameterNameErrorNumberFirst(ARG0)\
{\
fprintf(stderr, "Error 102008: Cannot create a new parameter named \"%s\". The name must begin with an English character or an underscore.\n", ARG0); \
exit(102008);}

#define ERRORCODE_ParameterNameErrorNoEnglish 102009
#define ERROR_ParameterNameErrorNoEnglish(ARG0)\
{\
fprintf(stderr, "Error 102009: Cannot create a new parameter named \"%s\". The name must contain at least an English character.\n", ARG0); \
exit(102009);}

#define ERRORCODE_ParameterNameErrorReserved 102010
#define ERROR_ParameterNameErrorReserved(ARG0)\
{\
fprintf(stderr, "Error 102010: Cannot create a new parameter named \"%s\". The name is reserved.\n", ARG0); \
exit(102010);}

#define ERRORCODE_ParameterNameErrorInvalidCharacter 102011
#define ERROR_ParameterNameErrorInvalidCharacter(ARG0)\
{\
fprintf(stderr, "Error 102011: Cannot create a new parameter named \"%s\". The name must only consists of English characters, digits and underscores.\n", ARG0); \
exit(102011);}

#define ERRORCODE_HDLProcessingFailed 103000
#define ERROR_HDLProcessingFailed()\
{\
fprintf(stderr, "Error 103000: An error has happened during HDL processing.\n"); \
exit(103000);}

#define ERRORCODE_IcarusNoOutputFile 103001
#define ERROR_IcarusNoOutputFile()\
{\
fprintf(stderr, "Error 103001: An output is not specified with the -o option.\n"); \
exit(103001);}

#define ERRORCODE_WarningMaskIllegal 103002
#define ERROR_WarningMaskIllegal(ARG0)\
{\
fprintf(stderr, "Error 103002: The warning mask \"%s\" is illegal. The legal form is 0xHHHHHHHH, while H is an upper hexdecimal number.\n", ARG0); \
exit(103002);}

#define ERRORCODE_FileCreationFailed 103003
#define ERROR_FileCreationFailed(ARG0)\
{\
fprintf(stderr, "Error 103003: Creating the output file \"%s\" has failed.\n", ARG0); \
exit(103003);}

#define ERRORCODE_StringFormatError 103004
#define ERROR_StringFormatError(ARG0,ARG1)\
{\
fprintf(stderr, "Error 103004: [%s:%d] An error is found in the format string.\n", ARG0, ARG1); \
exit(103004);}

#define ERRORCODE_StringFormatMissingOperand 103005
#define ERROR_StringFormatMissingOperand(ARG0,ARG1)\
{\
fprintf(stderr, "Error 103005: [%s:%d] The format string requires more operands than provided.\n", ARG0, ARG1); \
exit(103005);}

#define ERRORCODE_UnsupportedSystemTask 103006
#define ERROR_UnsupportedSystemTask(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 103006: [%s:%d] The system task/function \"%s\" is not supported.\n", ARG0, ARG1, ARG2); \
exit(103006);}

#define ERRORCODE_UnsupportedSystemTaskUnknownPlace 103007
#define ERROR_UnsupportedSystemTaskUnknownPlace(ARG0)\
{\
fprintf(stderr, "Error 103007: The system task/function \"%s\" is not supported.\n", ARG0); \
exit(103007);}

#define ERRORCODE_SystemTaskCalledAsFunction 103008
#define ERROR_SystemTaskCalledAsFunction(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 103008: [%s:%d] The system task \"%s\" is called as a function.\n", ARG0, ARG1, ARG2); \
exit(103008);}

#define ERRORCODE_SystemTaskFunctionArgumentError 103009
#define ERROR_SystemTaskFunctionArgumentError(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 103009: [%s:%d] The system task/function \"%s\" is invoked with incompatible arguments.\n", ARG0, ARG1, ARG2); \
exit(103009);}

#define ERRORCODE_UnsupportedStatement 103010
#define ERROR_UnsupportedStatement(ARG0,ARG1)\
{\
fprintf(stderr, "Error 103010: [%s:%d] The statement is unsupported.\n", ARG0, ARG1); \
exit(103010);}

#define ERRORCODE_ImmediateUnsupportedState 104000
#define ERROR_ImmediateUnsupportedState(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 104000: [%s:%d] The immediate number contains an unsupported value \'%c\'. Please increase the NoSV to solve this problem.\n", ARG0, ARG1, ARG2); \
exit(104000);}

#define ERRORCODE_UnsupportedStateGenerated 104001
#define ERROR_UnsupportedStateGenerated(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 104001: [%s:%d] The statement generates an unsupported value \'%c\'. Please increase the NoSV to solve this problem.\n", ARG0, ARG1, ARG2); \
exit(104001);}

#define ERRORCODE_UnsupportedUnknownValueGenerated 104002
#define ERROR_UnsupportedUnknownValueGenerated()\
{\
fprintf(stderr, "Error 104002: An unsupported unknown value has been generated.\n"); \
exit(104002);}

#define ERRORCODE_UnsupportedUnknownValueGeneratedAt 104003
#define ERROR_UnsupportedUnknownValueGeneratedAt(ARG0,ARG1)\
{\
fprintf(stderr, "Error 104003: [%s:%d] An unsupported unknown value has been generated.\n", ARG0, ARG1); \
exit(104003);}

#define ERRORCODE_RealNumber 105000
#define ERROR_RealNumber(ARG0,ARG1)\
{\
fprintf(stderr, "Error 105000: [%s:%d] The real data type is not supported.\n", ARG0, ARG1); \
exit(105000);}

#define ERRORCODE_WaitFound 105001
#define ERROR_WaitFound(ARG0,ARG1)\
{\
fprintf(stderr, "Error 105001: [%s:%d] The wait statement is not supported.\n", ARG0, ARG1); \
exit(105001);}

#define ERRORCODE_RepeatFound 105002
#define ERROR_RepeatFound(ARG0,ARG1)\
{\
fprintf(stderr, "Error 105002: [%s:%d] The repeat statement is not supported.\n", ARG0, ARG1); \
exit(105002);}

#define ERRORCODE_PowerError 105003
#define ERROR_PowerError(ARG0,ARG1)\
{\
fprintf(stderr, "Error 105003: [%s:%d] The power expression is not supported unless its first operand is the immediate number 2.\n", ARG0, ARG1); \
exit(105003);}

#define ERRORCODE_Tri0Tri1 105004
#define ERROR_Tri0Tri1(ARG0,ARG1)\
{\
fprintf(stderr, "Error 105004: [%s:%d] Declaring tri0 or tri1 nets is not supported.\n", ARG0, ARG1); \
exit(105004);}

#define WARNINGCODE_ModifyAlreadyConnectedToAPort 200000
#define WARNING_ModifyAlreadyConnectedToAPort(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Warning 200000: At %s:%d: A model port is created from %s, therefore the modification to it is ignored.\n", ARG0, ARG1, ARG2); \
}

#define WARNINGCODE_SystemFunctionCalledAsATask 201000
#define WARNING_SystemFunctionCalledAsATask(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Warning 201000: [%s:%d] The return value of the system function %s has not been used.\n", ARG0, ARG1, ARG2); \
}

#define WARNINGCODE_DelayFound 201001
#define WARNING_DelayFound(ARG0,ARG1)\
if (glb_Warning & (WARNING_CODE_IGNORED) ) {\
fprintf(stderr, "Warning 201001: [%s:%d] The delay statement is ignored.\n", ARG0, ARG1); \
}

#define WARNINGCODE_DelayExpressionFound 201002
#define WARNING_DelayExpressionFound(ARG0,ARG1)\
if (glb_Warning & (WARNING_CODE_IGNORED) ) {\
fprintf(stderr, "Warning 201002: [%s:%d] The delay expression is ignored.\n", ARG0, ARG1); \
}

#define WARNINGCODE_WaitExpressionFound 201003
#define WARNING_WaitExpressionFound(ARG0,ARG1)\
if (glb_Warning & (WARNING_CODE_IGNORED) ) {\
fprintf(stderr, "Warning 201003: [%s:%d] The wait expression is ignored.\n", ARG0, ARG1); \
}

#define WARNINGCODE_DisableIgnored 201004
#define WARNING_DisableIgnored(ARG0,ARG1)\
if (glb_Warning & (WARNING_CODE_IGNORED) ) {\
fprintf(stderr, "Warning 201004: [%s:%d] The disable statement is ignored because it does not disable its enclosing block.\n", ARG0, ARG1); \
}

extern unsigned int glb_Warning;

#else // CMSL_INTERNAL_H

#define ERRORCODE_WidthUnmatch 300000
#define ERROR_WidthUnmatch(ARG0)\
{\
fprintf(stderr, "Error 300000: Unmatching width detected in function \"%s\".\n", ARG0); \
exit(300000);}

#define ERRORCODE_WidthNotIdentical 300001
#define ERROR_WidthNotIdentical(ARG0)\
{\
fprintf(stderr, "Error 300001: The widths of the source cms_variables are not identical. Happened in function \"%s\".\n", ARG0); \
exit(300001);}

#define ERRORCODE_WriteOnlyVariableIsRead 300002
#define ERROR_WriteOnlyVariableIsRead()\
{\
fprintf(stderr, "Error 300002: A write-only cms_variable is read.\n"); \
exit(300002);}

#define ERRORCODE_ReadOnlyVariableIsWritten 300003
#define ERROR_ReadOnlyVariableIsWritten()\
{\
fprintf(stderr, "Error 300003: A read-only cms_variable is written.\n"); \
exit(300003);}

#define ERRORCODE_VariableNeedsGreaterNoSV 300004
#define ERROR_VariableNeedsGreaterNoSV(ARG0,ARG1)\
{\
fprintf(stderr, "Error 300004: The NoSV of a cms_variable should be at least %d. Happened in function %s.\n", ARG0, ARG1); \
exit(300004);}

#define ERRORCODE_BitUnrecognizable 300005
#define ERROR_BitUnrecognizable(ARG0,ARG1)\
{\
fprintf(stderr, "Error 300005: Bit '%c' is unrecognizable. Happended in function %s.\n", ARG0, ARG1); \
exit(300005);}

#define ERRORCODE_OffsetTooLarge 300006
#define ERROR_OffsetTooLarge(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 300006: The offset %d used to access a bit of a cms_variable is not smaller than the cms_variable's width %d. Happended in function %s.\n", ARG0, ARG1, ARG2); \
exit(300006);}

#define ERRORCODE_EndSmallerThanBegin 300007
#define ERROR_EndSmallerThanBegin(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Error 300007: The end offset %d is smaller than the begin offset %d. Happended in function %s.\n", ARG0, ARG1, ARG2); \
exit(300007);}

#define ERRORCODE_TooManyFileDescriptorCreated 300008
#define ERROR_TooManyFileDescriptorCreated()\
{\
fprintf(stderr, "Error 300008: Too many file descriptors are created.\n"); \
exit(300008);}

#define WARNINGCODE_ErrorInFormatSpecification 400000
#define WARNING_ErrorInFormatSpecification(ARG0,ARG1)\
{\
fprintf(stderr, "Warning 400000: An error has happened when parsing the character '%c' in the format specification \"%s\".\n", ARG0, ARG1); \
}

#define WARNINGCODE_FormatWidthTooLarge 400001
#define WARNING_FormatWidthTooLarge(ARG0)\
{\
fprintf(stderr, "Warning 400001: The width in the format specification \"%s\" is too large.\n", ARG0); \
}

#define WARNINGCODE_FileCannotBeOpened 400002
#define WARNING_FileCannotBeOpened(ARG0)\
{\
fprintf(stderr, "Warning 400002: The file \"%s\" cannot be opened.\n", ARG0); \
}

#define WARNINGCODE_FileDescriptorIsNotValid 400003
#define WARNING_FileDescriptorIsNotValid(ARG0)\
{\
fprintf(stderr, "Warning 400003: The file descriptor used in the function \"%s\" is invalid.\n", ARG0); \
}

#define WARNINGCODE_MultiChannelDescriptorIsNotValid 400004
#define WARNING_MultiChannelDescriptorIsNotValid(ARG0)\
{\
fprintf(stderr, "Warning 400004: The multichannel descriptor used in the function \"%s\" is invalid.\n", ARG0); \
}

#define WARNINGCODE_MultiChannelDescriptorTooShort 400005
#define WARNING_MultiChannelDescriptorTooShort(ARG0)\
{\
fprintf(stderr, "Warning 400005: The width of the destination multichannel descriptor used in the function \"%s\" should be no less than 32.\n", ARG0); \
}

#define WARNINGCODE_AllChannelsAreUsed 400006
#define WARNING_AllChannelsAreUsed()\
{\
fprintf(stderr, "Warning 400006: Cannot create a multichannel descriptor because all channels are in use.\n"); \
}

#define WARNINGCODE_ErrorStringIsNotWideEnough 400007
#define WARNING_ErrorStringIsNotWideEnough(ARG0)\
{\
fprintf(stderr, "Warning 400007: In function \"%s\", the cms_variable pointed to by the third argument should has at least 640 bits.\n", ARG0); \
}

#define WARNINGCODE_OperationInvalid 400008
#define WARNING_OperationInvalid()\
{\
fprintf(stderr, "Warning 400008: In cms_$fseek, the value of the cms_variable pointed to by the operation argument must be 0, 1 or 2.\n"); \
}

#define WARNINGCODE_ReadMemFailedToOpenFile 400009
#define WARNING_ReadMemFailedToOpenFile(ARG0,ARG1)\
{\
fprintf(stderr, "Warning 400009: In function \"%s\", opening the file named \"%s\" has failed.\n", ARG0, ARG1); \
}

#define WARNINGCODE_ReadMemUnterminatedComment 400010
#define WARNING_ReadMemUnterminatedComment(ARG0,ARG1)\
{\
fprintf(stderr, "Warning 400010: In function \"%s\", the file \"%s\" ends with an unterminated comment.\n", ARG0, ARG1); \
}

#define WARNINGCODE_ReadMemIllegalEntry 400011
#define WARNING_ReadMemIllegalEntry(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Warning 400011: In function \"%s\", the file \"%s\" has an illegal entry at line %d.\n", ARG0, ARG1, ARG2); \
}

#define WARNINGCODE_ReadMemOutOfBound 400012
#define WARNING_ReadMemOutOfBound(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Warning 400012: In function \"%s\", the file \"%s\" has an out-of-bounds address at line %d.\n", ARG0, ARG1, ARG2); \
}

#define WARNINGCODE_ReadMemOutOfBoundEntry 400013
#define WARNING_ReadMemOutOfBoundEntry(ARG0,ARG1,ARG2)\
{\
fprintf(stderr, "Warning 400013: In function \"%s\", the file \"%s\" has an entry with an out-of-bounds address at line %d.\n", ARG0, ARG1, ARG2); \
}

#define WARNINGCODE_ReadMemStartOutOfBound 400014
#define WARNING_ReadMemStartOutOfBound(ARG0)\
{\
fprintf(stderr, "Warning 400014: In function \"%s\", the start address is out-of-bounds.\n", ARG0); \
}

#define WARNINGCODE_ReadMemTooFewElements 400015
#define WARNING_ReadMemTooFewElements(ARG0,ARG1)\
{\
fprintf(stderr, "Warning 400015: In function \"%s\", the file \"%s\" has too few elements than required.\n", ARG0, ARG1); \
}

#endif // CMSL_INTERNAL_H

#endif // __ERRORMESSAGE_H__
