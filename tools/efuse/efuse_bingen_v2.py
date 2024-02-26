import binascii
import os.path
import sys
from xml.dom import minidom
import argparse
import re
import base64
import subprocess
import os
import hashlib
import logging

log_file_path = ""

def convert_to_int_value(val, tag_name, attr_name=None) :
    if (val == "") or (val == 0) or (val == "0") :
        return 0
    if (val == 1) or (val == "1") :
        return 1
    if isinstance(val, basestring) :    #check if "val" is string type (only string has lower() method)
        if val.lower() == "true" :
            return 1
        if val.lower() == "false" :
            return 0

    if attr_name : #has Atribute
        PrintError_RaiseException_StopBuild("Attribute Name: " + attr_name + " => I got value: \"" + str(val) + "\". The value should be: \"1\" or \"0\" or \"true\" or \"false\" or empty(=false)")
    else :
        PrintError_RaiseException_StopBuild("Tag Name: " + tag_name + " => I got value: \"" + str(val) + "\". The value should be: \"1\" or \"0\" or \"true\" or \"false\" or empty(=false)")


def isReverseEndianEnabled(val) :
    if isinstance(val, basestring) :    #check if "val" is string type (only string has lower() method)
        if val.lower() == "true" :
            return True
        if val.lower() == "false" :
            return False
        PrintError_RaiseException_StopBuild("[definition] \"reverse_endian\" should be in value \"true\" or \"false\".")
    else :
        PrintError_RaiseException_StopBuild("[definition] \"reverse_endian\" should be in String(\"true\" or \"false\") format.")


def reverseEndian(val, enableReverse=False) :
    if enableReverse :
        if not isinstance(val, basestring) :
            PrintError_RaiseException_StopBuild("The type of input value of \"reverseEndian\" should be String")
        if (len(val) == 4) :
            return val[2:4] + val[0:2]
        elif (len(val) == 8) :
            return val[6:8] + val[4:6] + val[2:4] + val[0:2]
    else :
        return val

    PrintError_RaiseException_StopBuild("The input value of \"reverseEndian\" should be the multiply of 4")


def writeBitValueAndOneHexStringToFile(fstream, lstBitValue, hexString) :
    """
        Notice: The priority of bit input(lstBitValue) is higher than hex input(hexString) if hexString overlaps lstBitValue
    """

    result = 0;
    priority_mask = 0xFFFFFFFF
    for bit_number, value in lstBitValue :
        if value == 1 :
            result = result + (2 ** bit_number) #transform bit to decimal
        priority_mask = priority_mask & ~(2 ** bit_number)

    hexString2Decimal = int(hexString, 16)  #transform hex to decimal
    result = (hexString2Decimal & priority_mask) | result #merge

    plain_hex_string = str(hex(result))[2:]  #result is decimal, transform to hex
    plain_hex_string = plain_hex_string.zfill(8)  #padding 0 at right to 8 digits
    lstHexArr = re.findall('..', plain_hex_string)

    fstream.write(chr(int(lstHexArr[3], 16)))
    fstream.write(chr(int(lstHexArr[2], 16)))
    fstream.write(chr(int(lstHexArr[1], 16)))
    fstream.write(chr(int(lstHexArr[0], 16)))


def getBitValueAndOneHexString(lstBitValue, hexString) :
    """
        Notice: The priority of bit input(lstBitValue) is higher than hex input(hexString) if hexString overlaps lstBitValue
                This function is the same as function "writeBitValueAndOneHexStringToFile" except it return the merged value rather than writing to file
    """

    result = 0;
    priority_mask = 0xFFFFFFFF
    for bit_number, value in lstBitValue :
        if value == 1 :
            result = result + (2 ** bit_number) #transform bit to decimal
        priority_mask = priority_mask & ~(2 ** bit_number)

    hexString2Decimal = int(hexString, 16)  #transform hex to decimal
    result = (hexString2Decimal & priority_mask) | result #merge

    plain_hex_string = str(hex(result))[2:]  #result is decimal, transform to hex
    plain_hex_string = plain_hex_string.zfill(len(hexString))  #padding 0 at right to 8 digits

    return plain_hex_string.upper()


def writeBitValueToFile(fstream, lstBitValue) :
    result = 0;
    for bit_number, value in lstBitValue :
        if value == 1 :
            result = result + (2 ** bit_number)

    plain_hex_string = str(hex(result))[2:]  #result is decimal
    plain_hex_string = plain_hex_string.zfill(8)
    lstHexArr = re.findall('..', plain_hex_string)

    fstream.write(chr(int(lstHexArr[3], 16)))
    fstream.write(chr(int(lstHexArr[2], 16)))
    fstream.write(chr(int(lstHexArr[1], 16)))
    fstream.write(chr(int(lstHexArr[0], 16)))


def get_file_sha256hexdigest(file_name):

    hash_result = ""

    with open(file_name) as f:
        m = hashlib.sha256()
        m.update(f.read())
        hash_result = m.hexdigest()

    return hash_result.upper()


def writeHexStringToFile(fstream, hexString) :
    plain_hex_string = hexString.ljust(8, '0')  #padding 0 at right to 8 digits
    lstHexArr = re.findall('..', plain_hex_string)

    fstream.write(chr(int(lstHexArr[0], 16)))
    fstream.write(chr(int(lstHexArr[1], 16)))
    fstream.write(chr(int(lstHexArr[2], 16)))
    fstream.write(chr(int(lstHexArr[3], 16)))


def checkLessThan32BitsHexStringLength(tag_name, hexString, min_index, max_index, attribute_name="") :
    if (max_index <= min_index) :
        PrintError_RaiseException_StopBuild("[Coding Error] Maximum index should be bigger than minimum index!")

    plain_hex_string = hexString.ljust(8, '0')  #padding 0 at right to 8 digits
    lstHexArr = re.findall('..', plain_hex_string)

    result = 0;
    mask = 0xFFFFFFFF
    for bit_number in range(0, 32) :
        if (min_index <= bit_number <= max_index) :
            mask = mask & ~(2 ** bit_number)

    input_length_mask = int(lstHexArr[3] + lstHexArr[2] + lstHexArr[1] + lstHexArr[0], 16)
    forbidden_field_mask = mask

    # print(hex(input_length_mask))
    # print(hex(forbidden_field_mask))

    if (input_length_mask & forbidden_field_mask) != 0 :
        if attribute_name == "" :
            PrintError_RaiseException_StopBuild("Tag Name: " + tag_name + " (Wrong value length. The length of value should be \"" + tag_name + "[" + str(max_index) + ":" + str(min_index) + "]\")")
        else :
            PrintError_RaiseException_StopBuild("Tag Name: " + tag_name + " (Wrong value length. The length of value should be \"" + attribute_name + "[" + str(max_index) + ":" + str(min_index) + "]\")")


def parseXmlTagAndAttribute(xml_file, tag_name, attr_name, inputValueLengthLimit=1, inputIsStringType=False) :
    if inputIsStringType :
        retVal = '0' *  inputValueLengthLimit
    else :
        retVal = 0

    try:

        if not xml_file.getElementsByTagName(tag_name) :
            raise KeyError
        tag_number = len(xml_file.getElementsByTagName(tag_name))
        if tag_number > 1 :
            raise ValueError("Duplicated tag name. It appears " + str(tag_number) + " times!!")

        tmp_parse_value = xml_file.getElementsByTagName(tag_name)[0].attributes[attr_name].value

        if inputIsStringType :
            if (tmp_parse_value == "") :
                return retVal
            if (inputValueLengthLimit <> len(tmp_parse_value)) :
                raise ValueError("Wrong value length. The length of value should be: " + str(inputValueLengthLimit))
            if isinstance(tmp_parse_value, basestring) :  #check if "tmp_parse_value" is string type (only string has upper() method)
                if not isValidHexString(tmp_parse_value) :  #only string can be processed by regex
                    raise ValueError("Wrong hex value type! The value should be within [0-9|A-F]")
                return tmp_parse_value.upper()
            return tmp_parse_value
        else : #not string
            return convert_to_int_value(tmp_parse_value, tag_name, attr_name)
    except IndexError:
        #Tag or Attribute not exist
        #printAndLog("[Warning][Not Exist] Tag Name: " + tag_name + ", Attribute Name: " + attr_name + " => Set to default value: " + str(retVal))
        return retVal
    except KeyError:
        #Tag or Attribute not exist
        #printAndLog("[Warning][Not Exist] Tag Name: " + tag_name + ", Attribute Name: " + attr_name + " => Set to default value: " + str(retVal))
        return retVal
    except ValueError as err:
        PrintError_RaiseException_StopBuild("Tag Name: " + tag_name + ", Attribute Name: " + attr_name + " (" + str(err) + ")")


def parseXmlTagInnerValue(xml_file, tag_name, inputValueLengthLimit=1, inputIsStringType=False) :
    if inputIsStringType :
        retVal = '0' *  inputValueLengthLimit
    else :
        retVal = 0

    try:

        if not xml_file.getElementsByTagName(tag_name) :
            raise KeyError
        tag_number = len(xml_file.getElementsByTagName(tag_name))
        if tag_number > 1 :
            raise ValueError("Duplicated tag name. It appears " + str(tag_number) + " times!!")

        tmp_parse_value = xml_file.getElementsByTagName(tag_name)[0].childNodes[0].data

        if inputIsStringType :
            if (tmp_parse_value == "") :
                return retVal
            if (inputValueLengthLimit <> len(tmp_parse_value)) :
                raise ValueError("Wrong value length. The length of value should be: " + str(inputValueLengthLimit))
            if isinstance(tmp_parse_value, basestring) :  #check if "tmp_parse_value" is string type (only string has upper() method)
                if not isValidHexString(tmp_parse_value) :  #only string can be processed by regex
                    raise ValueError("Wrong hex value type! The value should be within [0-9|A-F]")
                return tmp_parse_value.upper()
            return tmp_parse_value
        else : #not string
            return convert_to_int_value(tmp_parse_value, tag_name)
    except IndexError:
        #Tag or Attribute not exist
        #printAndLog("[Warning][Not Exist] Tag Name: " + tag_name + " => Set to default value: " + str(retVal))
        return retVal
    except KeyError:
        #Tag or Attribute not exist
        #printAndLog("[Warning][Not Exist] Tag Name: " + tag_name + " => Set to default value: " + str(retVal))
        return retVal
    except ValueError as err:
        PrintError_RaiseException_StopBuild("Tag Name: " + tag_name + " (" + str(err) + ")")


def isValidHexString(hex_input) :
    if re.match(r"^[0-9A-F]*$", hex_input, re.IGNORECASE) :
        return True
    return False


def isValidNumberString(number_input) :
    if re.match(r"^[0-9]*$", number_input, re.IGNORECASE) :
        return True
    return False


def isValidRegisterLengthString(number_input) :
    if re.match(r"^[0-9]*$", number_input, re.IGNORECASE) :
        if 1 <= int(number_input) <= 32 :
            return True
    return False


def isValidRegisterIndexString(index_input) :
    if re.match(r"^[0-9]*$", index_input, re.IGNORECASE) :
        if 0 <= int(index_input) <= 31 :
            return True
    return False


def printAndLog(msg, criticalLevel=False):
    print(msg)

    global log_file_path
    if (log_file_path) :
        logging.basicConfig(format='[%(asctime)s] %(message)s', filename=log_file_path, level=logging.DEBUG)
        if criticalLevel :
            logging.critical(msg)
        else :
            logging.info(msg)


def PrintError_RaiseException_StopBuild(err) :
    printAndLog("[Error] " + err, criticalLevel=True)
    raise Exception("[Error] " + err)


def main():

    WRITE_TYPE_BIT = 1
    WRITE_TYPE_STRING = 2
    WRITE_TYPE_BIT_AND_STRING = 3

    parser = argparse.ArgumentParser(description='MediaTek EFUSE XML Parser')
    parser.add_argument('--file', '-f',
                        required=True,
                        help='Provide the EFUSE blowing xml file')
    parser.add_argument('--definition_file', '-d',
                        required=True,
                        help='Provide the EFUSE definition file')
    parser.add_argument('--output_bin_name', '-o',
                        required=False,
                        default='xml_output.bin',
                        help='Provide output file name')
    parser.add_argument('--key_hash', '-k',
                        required=False,
                        help='Provide the file name path of key hash')
    parser.add_argument('--log_output_file', '-l',
                        required=False,
                        help='Provide the log output file name')

    args = parser.parse_args()

    if (args.log_output_file) :
        if os.path.isfile(args.log_output_file) :
            try :
                os.remove(args.log_output_file)
            except :
                pass

        global log_file_path
        log_file_path = args.log_output_file

    printAndLog("***************************************************************************")
    printAndLog("**************** MediaTek EFUSE XML Parser ([MTK_XML2BIN]) ****************")
    printAndLog("****************************** version 2.0.2 ******************************")
    printAndLog("***************************************************************************")

    printAndLog("Loading XML file: " + os.path.abspath(args.file))

    if os.path.isfile(args.output_bin_name) :
        os.remove(args.output_bin_name)
        printAndLog("Remove old image file: " + os.path.abspath(args.output_bin_name))
        printAndLog("-----------------------------------------------")

    if not os.path.isfile(args.file) :
        PrintError_RaiseException_StopBuild("XML file not exist!!")

    if not os.path.isfile(args.definition_file) :
        PrintError_RaiseException_StopBuild("EFUSE definition file not exist!!")

    try :
        xml_file = minidom.parse(args.file)
    except Exception:
        printAndLog("[Error] ***** XML format is NOT CORRECT. Please check your XML input file. *****")
        printAndLog("[Error] ***** XML format is NOT CORRECT. Please check your XML input file. *****")
        PrintError_RaiseException_StopBuild("***** XML format is NOT CORRECT. Please check your XML input file. *****")

    try :
        definition_file = minidom.parse(args.definition_file)
    except Exception:
        printAndLog("[Error] ***** EFUSE Definition XML format is NOT CORRECT. Please check your XML input file. *****")
        printAndLog("[Error] ***** EFUSE Definition XML format is NOT CORRECT. Please check your XML input file. *****")
        PrintError_RaiseException_StopBuild("***** EFUSE Definition XML format is NOT CORRECT. Please check your XML input file. *****")

    efuse_writer_tag = definition_file.getElementsByTagName("efuse_writer")[0]
    if len(definition_file.getElementsByTagName("efuse_writer")) > 1 :
        PrintError_RaiseException_StopBuild("[definitions] Should only have one \"efuse_writer\" tag.")

    efuse_writer_chip = efuse_writer_tag.getAttribute("chip").strip()
    if (efuse_writer_chip == "") :
        PrintError_RaiseException_StopBuild("[definitions] chip name should not be empty in definition file.")

    printAndLog("Definition Target Platform: " + efuse_writer_chip)

    efuse_writer_output_binary_size = efuse_writer_tag.getAttribute("output_bin_size").strip()
    if (efuse_writer_output_binary_size == "") :
        PrintError_RaiseException_StopBuild("[definitions] output_bin_size should not be empty in definition file.")
    if not isValidNumberString(efuse_writer_output_binary_size) :
        PrintError_RaiseException_StopBuild("[definitions] output_bin_size is not a valid number.")
    try :
        MAX_OUTPUT_OFFSET_DECIMAL = int(efuse_writer_output_binary_size) - 32
    except ValueError :
        PrintError_RaiseException_StopBuild("[definitions] " + efuse_writer_output_binary_size + " is not a valid type of number.")

    printAndLog("Definition Expected Output Binary Size: " + efuse_writer_output_binary_size + " bytes")

    #Parsing XML to variable
    printAndLog("Parsing XML file ...")

    dict_definition_inner_value = {}
    dict_definition_inner_text = {}
    dict_definition_boolean = {}
    dict_definition_external = {}

    printAndLog("-----------------------------------------------")

    if not definition_file.getElementsByTagName("definitions") :
        PrintError_RaiseException_StopBuild("[definitions] No \"definitions\" tag found in EFUSE definition file.")

    if len(definition_file.getElementsByTagName("definitions")) > 1 :
        PrintError_RaiseException_StopBuild("[definitions] Should only have one \"definitions\" tag.")

    definition_alllist = definition_file.getElementsByTagName("definitions")[0]
    definition_inner_value_list = definition_alllist.getElementsByTagName("inner_value")
    definition_inner_text_list = definition_alllist.getElementsByTagName("inner_text")
    definition_boolean_list = definition_alllist.getElementsByTagName("boolean")

    definition_merge_inner_text_and_value_list = definition_inner_value_list + definition_inner_text_list

    for tag in definition_merge_inner_text_and_value_list :
        tag_type = tag.tagName
        tag_name = tag.getAttribute("tag").strip()
        tag_attribute = ""

        if tag_name == "" :
            PrintError_RaiseException_StopBuild("[definitions] Type: " + tag_type + ", \"tag\" value should not be null.")

        if tag_type == "inner_value" :
            tag_attribute = tag.getAttribute("attribute").strip()
            if tag_attribute == "" :
                PrintError_RaiseException_StopBuild("[definitions] Type: " + tag_type + ", \"attribute\" value should not be null.")

        suppress_log = tag.getElementsByTagName("suppress_log")
        log_display = True
        if suppress_log :
            log_display = False

        if tag_type == "inner_value" :
            prepared_error_msg = "Type: " + tag_type + " => tag: " + tag_name + ", attribute: " + tag_attribute

            if tag_name not in dict_definition_inner_value :
                dict_definition_inner_value[tag_name] = {}

            if tag_attribute in dict_definition_inner_value[tag_name] :
                PrintError_RaiseException_StopBuild("[definitions] Duplicated attribute name for the same tag name. (" + prepared_error_msg + ")")

        else : # tag_type == "inner_text"
            prepared_error_msg = "Type: " + tag_type + " => tag: " + tag_name

            if tag_name in dict_definition_inner_text :
                PrintError_RaiseException_StopBuild("[definitions] Duplicated tag name. (" + prepared_error_msg + ")")


        require_conditions = tag.getElementsByTagName("require")
        dict_tmp_require_conditions = {"length": None,
                                       "valid_start_bit": None,
                                       "valid_end_bit": None}

        for condition in require_conditions :
            for condition_name in dict_tmp_require_conditions :
                tmp_condition_value = condition.getAttribute(condition_name).strip()
                if (tmp_condition_value != "") :
                    if (dict_tmp_require_conditions[condition_name] is not None) :
                        PrintError_RaiseException_StopBuild("[definitions] Duplicated \"" + condition_name + "\" declaration. (" + prepared_error_msg + ")")
                    dict_tmp_require_conditions[condition_name] = tmp_condition_value

        if dict_tmp_require_conditions["length"] == None :
            PrintError_RaiseException_StopBuild("[definitions] Length is required. (" + prepared_error_msg + ")")

        #value check and transform string to integer
        if not isValidRegisterLengthString(dict_tmp_require_conditions["length"]) :
            PrintError_RaiseException_StopBuild("[definitions] Length is not a valid number(should be 1-32). (" + prepared_error_msg + ")")
        dict_tmp_require_conditions["length"] = int(dict_tmp_require_conditions["length"])

        if tag_type == "inner_value" :
            tag_value = parseXmlTagAndAttribute(xml_file, tag_name, tag_attribute, dict_tmp_require_conditions["length"], True)
        else : # tag_type == "inner_text"
            tag_value = parseXmlTagInnerValue(xml_file, tag_name, dict_tmp_require_conditions["length"], True)

        if (dict_tmp_require_conditions["valid_start_bit"] != None) or (dict_tmp_require_conditions["valid_end_bit"] != None) :
            if (dict_tmp_require_conditions["valid_start_bit"] == None) or (dict_tmp_require_conditions["valid_end_bit"] == None) :
                PrintError_RaiseException_StopBuild("[definitions] valid_start_bit and valid_end_bit should appear at the same time or not. (" + prepared_error_msg + ")")

            #value check and transform string to integer
            if (not isValidRegisterIndexString(dict_tmp_require_conditions["valid_start_bit"])) :
                PrintError_RaiseException_StopBuild("[definitions] valid_start_bit is not a valid index(should be 0-31). (" + prepared_error_msg + ")")
            if (not isValidRegisterIndexString(dict_tmp_require_conditions["valid_end_bit"])) :
                PrintError_RaiseException_StopBuild("[definitions] valid_end_bit is not a valid index(should be 0-31). (" + prepared_error_msg + ")")
            dict_tmp_require_conditions["valid_start_bit"] = int(dict_tmp_require_conditions["valid_start_bit"])
            dict_tmp_require_conditions["valid_end_bit"] = int(dict_tmp_require_conditions["valid_end_bit"])

            checkLessThan32BitsHexStringLength(tag_name, tag_value, dict_tmp_require_conditions["valid_start_bit"], dict_tmp_require_conditions["valid_end_bit"], tag_attribute);

        if tag_type == "inner_value" :
            dict_definition_inner_value[tag_name][tag_attribute] = tag_value
        else : # tag_type == "inner_text"
            dict_definition_inner_text[tag_name] = tag_value

        if log_display :
            printAndLog("EFUSE_" + tag_name + " = " + tag_value)

    #print(dict_definition_inner_text)
    #print(dict_definition_inner_value)

    for tag in definition_boolean_list :
        tag_name = tag.getAttribute("tag").strip()
        tag_attribute = tag.getAttribute("attribute").strip()

        if (tag_name == "") or (tag_attribute == "") :
            PrintError_RaiseException_StopBuild("[definitions] boolean type, tag or attribute value should not be null.")

        suppress_log = tag.getElementsByTagName("suppress_log")
        log_display = True
        if suppress_log :
            log_display = False

        if tag_name not in dict_definition_boolean :
            dict_definition_boolean[tag_name] = {}

        if tag_attribute in dict_definition_boolean[tag_name] :
            PrintError_RaiseException_StopBuild("[definitions] Duplicated attribute name. (boolean type, tag: " + tag_name + ", attribute: " + tag_attribute + ")")

        tag_value = parseXmlTagAndAttribute(xml_file, tag_name, tag_attribute)

        dict_definition_boolean[tag_name][tag_attribute] = tag_value

        if log_display :
            printAndLog("EFUSE_" + tag_attribute + " = " + str(tag_value))

    #print(dict_definition_boolean)

    printAndLog("-----------------------------------------------")

    EFUSE_SBC_PUBK_HASH = '0' * 64
    if args.key_hash :
        printAndLog("[Info] Loading SBC_PUBK_HASH from key hash file: " + os.path.abspath(args.key_hash))

        if os.path.isfile(args.key_hash) :
            try:
                with open(args.key_hash, 'r') as f:
                    EFUSE_SBC_PUBK_HASH = f.read()
            except Exception:
                PrintError_RaiseException_StopBuild("***** Error while reading key hash file *****")

            EFUSE_SBC_PUBK_HASH = EFUSE_SBC_PUBK_HASH.strip()
            if EFUSE_SBC_PUBK_HASH == "" :
                PrintError_RaiseException_StopBuild("SBC_PUBK_HASH is empty and not generated")

            if len(EFUSE_SBC_PUBK_HASH) <> 64 :
                PrintError_RaiseException_StopBuild("SBC_PUBK_HASH is not in length 64. Current length of SBC_PUBK_HASH is: " + str(len(EFUSE_SBC_PUBK_HASH)))

            EFUSE_SBC_PUBK_HASH = EFUSE_SBC_PUBK_HASH.upper()

            if not isValidHexString(EFUSE_SBC_PUBK_HASH) :
                PrintError_RaiseException_StopBuild("SBC_PUBK_HASH contains invalid hex string(s)! The value should be within [0-9|A-F]")

        else :
            PrintError_RaiseException_StopBuild(args.key_hash + " is not generated from getKeyHash.sh for SBC_Key_Hash!!")

    else :
        printAndLog("[Info] SBC_PUBK_HASH is not loaded from key hash file.")
        EFUSE_SBC_PUBK_HASH = '0' * 64

    dict_definition_external["SBC_PUBK_HASH"] = EFUSE_SBC_PUBK_HASH

    printAndLog("EFUSE_SBC_PUBK_HASH = " + EFUSE_SBC_PUBK_HASH)

    printAndLog("-----------------------------------------------")

    dict_definition_output = {}
    lst_supported_output_type = ["inner_value", "inner_text", "external", "boolean", "mix_type"]
    lst_supported_output_mix_type = ["inner_value", "inner_text", "external", "boolean"]

    if not definition_file.getElementsByTagName("blow_list") :
        PrintError_RaiseException_StopBuild("[definitions] No \"blow_list\" tag found in EFUSE definition file.")

    if len(definition_file.getElementsByTagName("blow_list")) > 1 :
        PrintError_RaiseException_StopBuild("[definitions] Should only have one \"blow_list\" tag.")

    definition_blow_list = definition_file.getElementsByTagName("blow_list")[0]
    definition_blow_efuse_items = definition_blow_list.getElementsByTagName("efuse")


    for tag in definition_blow_efuse_items :
        tag_type = tag.getAttribute("type").strip()
        tag_offset = tag.getAttribute("offset").strip()
        offset_int_decimal = 0

        if (tag_offset == "") :
            PrintError_RaiseException_StopBuild("[definitions] \"efuse\" tag => \"tag_offset\" should not be null.")

        if (tag_type == "") :
            PrintError_RaiseException_StopBuild("[definitions] \"efuse\" tag => \"tag_type\" should not be null.")

        if tag_type not in lst_supported_output_type :
            PrintError_RaiseException_StopBuild("[definitions] Output type \"" + tag_type + "\" is not supported.")

        try :
            offset_int_decimal = int(tag_offset, 16)

            if (offset_int_decimal % 4) != 0 :
                PrintError_RaiseException_StopBuild("[definitions] offset \"" + tag_offset + "\" should be 4bytes align.")
        except ValueError :
            PrintError_RaiseException_StopBuild("[definitions] offset \"" + tag_offset + "\" is not a valid hex.")

        if offset_int_decimal in dict_definition_output :
            PrintError_RaiseException_StopBuild("[definitions] offset \"" + tag_offset + "\" is duplicated.")

        if (offset_int_decimal >= MAX_OUTPUT_OFFSET_DECIMAL) :
            PrintError_RaiseException_StopBuild("[definitions] Since the biggest size of output binary file is 512 bytes, the offset \"" + str(offset_int_decimal) + "\" in decimal should not exceeds " + str(MAX_OUTPUT_OFFSET_DECIMAL) + ".")

        dict_definition_output[offset_int_decimal] = {}

        input_data_items = tag.getElementsByTagName("input")

        #value checking
        if input_data_items.length == 0 :
            PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => No input declaration")
        if (tag_type not in ["mix_type", "boolean"]) :
            if input_data_items.length > 1 :
                PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => This type only allows one declaration")

        if (tag_type == "mix_type") :
            dict_definition_output[offset_int_decimal]["write_data"] = ""  #set the default empty string value because it will append the string for this kind of type

        tmp_dict_input_bit_duplicated_counter = {}

        for input_field in input_data_items :

            dict_tmp_input_fields = {"key": None,
                                     "tag": None,
                                     "attribute": None,
                                     "bit": None,
                                     "start_index": None,
                                     "end_index": None,
                                     "reverse_endian": None,
                                     "type": None}

            for attribute_name_in_input_tag in input_field.attributes.keys() : #loop attribute and attribute value
                if (attribute_name_in_input_tag != "") :
                    attribute_value_in_input_tag = input_field.getAttribute(attribute_name_in_input_tag).strip()

                    if attribute_name_in_input_tag not in dict_tmp_input_fields :
                        PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => The input attribute \"" + attribute_name_in_input_tag + "\" is not supported.")

                    if (dict_tmp_input_fields[attribute_name_in_input_tag] is not None) :
                        PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => Duplicated attribute: \"" + attribute_name_in_input_tag + "\".")

                    if attribute_value_in_input_tag == "" :
                        PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => Attribute: \"" + attribute_name_in_input_tag + "\" value cannot be null.")

                    if attribute_name_in_input_tag == "key" :
                        if attribute_value_in_input_tag not in dict_definition_external :
                            PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => Key: \"" + attribute_value_in_input_tag + "\" cannot be found in external storage (current external storage: " + str(dict_definition_external) + ").")
                        dict_tmp_input_fields[attribute_name_in_input_tag] = attribute_value_in_input_tag

                    elif attribute_name_in_input_tag == "type" :
                        if tag_type != "mix_type" :
                            PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => You must set the overall \"type\" to \"mix_type\" first if you want to specify the \"type\" attribute in each input field.")
                        if attribute_value_in_input_tag not in lst_supported_output_mix_type :
                            PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => input type: \"" + attribute_value_in_input_tag + "\" is not supported.")
                        dict_tmp_input_fields[attribute_name_in_input_tag] = attribute_value_in_input_tag

                    elif attribute_name_in_input_tag == "bit" :
                        if not isValidRegisterIndexString(attribute_value_in_input_tag) :
                            PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => Attribute: \"" + attribute_name_in_input_tag + "\" should be in valid index range(0-31).")
                        dict_tmp_input_fields["bit"] = int(attribute_value_in_input_tag)

                    elif attribute_name_in_input_tag == "reverse_endian" :
                        if (isReverseEndianEnabled(attribute_value_in_input_tag)) :
                            dict_tmp_input_fields["reverse_endian"] = reverseEndian(attribute_value_in_input_tag)

                    elif (attribute_name_in_input_tag == "start_index") or (attribute_name_in_input_tag == "end_index") :
                        if not isValidNumberString(attribute_value_in_input_tag) :
                            PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => Attribute: \"" + attribute_name_in_input_tag + "\" should be a valid number.")
                        if (attribute_name_in_input_tag == "start_index") :
                            dict_tmp_input_fields["start_index"] = int(attribute_value_in_input_tag)
                        elif (attribute_name_in_input_tag == "end_index") :
                            dict_tmp_input_fields["end_index"] = int(attribute_value_in_input_tag)

                    else :
                        dict_tmp_input_fields[attribute_name_in_input_tag] = attribute_value_in_input_tag

            #print(dict_tmp_input_fields)

            need_slice_string = False
            query_tag = dict_tmp_input_fields["tag"]
            query_attribute = dict_tmp_input_fields["attribute"]
            query_key = dict_tmp_input_fields["key"]
            query_start_index = dict_tmp_input_fields["start_index"]
            query_end_index = dict_tmp_input_fields["end_index"]
            query_reverse_endian = dict_tmp_input_fields["reverse_endian"]
            query_tag_type = dict_tmp_input_fields["type"]

            if (query_tag_type == None) :
                query_tag_type = tag_type

            if (query_start_index != None) or (query_end_index != None) :
                if (query_start_index is None) or (query_end_index is None) :
                    PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => \"start_index\" and \"end_index\" should appear at the same time or not.")
                if (query_end_index <= query_start_index) :
                    PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => \"end_index\" should be bigger than \"start_index\".")
                if (query_end_index - query_start_index) > 7 :
                    PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => \"start_index\" or \"end_index\" is not within the correct range.")
                need_slice_string = True

            #check: (1)start_index and end_index (2)end_index-start_index<=7 (3)xxx_index should be valid

            if (query_tag_type == "inner_value") :
                if (dict_tmp_input_fields["tag"] is None) or (dict_tmp_input_fields["attribute"] is None) :
                    PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => \"tag\" and \"attribute\" are necessary for this type.")

                if query_tag not in dict_definition_inner_value :
                    PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => You must declare the tag \"" + query_tag + "\" first.")
                if query_attribute not in dict_definition_inner_value[query_tag] :
                    PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => You must declare the attribute \"" + query_attribute + "\" in tag \"" + query_tag + "\" first.")

                dict_definition_output[offset_int_decimal]["write_type"] = WRITE_TYPE_STRING

                if need_slice_string :
                    tmp_length = len(dict_definition_inner_value[query_tag][query_attribute])
                    if (tmp_length < (query_end_index + 1)) or (tmp_length < (query_start_index + 1)) :
                        PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => \"end_index\" exceeds the maximum length \"" + str(tmp_length) + "\" defined in declaration area.")
                    tmp_write_data = reverseEndian(dict_definition_inner_value[query_tag][query_attribute][query_start_index:query_end_index + 1], query_reverse_endian)
                else :
                    tmp_write_data = reverseEndian(dict_definition_inner_value[query_tag][query_attribute], query_reverse_endian)

                if (tag_type == "mix_type") :
                    dict_definition_output[offset_int_decimal]["write_data"] = dict_definition_output[offset_int_decimal]["write_data"] + tmp_write_data
                else :
                    dict_definition_output[offset_int_decimal]["write_data"] = tmp_write_data

                # dict_definition_output[offset_int_decimal]["write_data"]

            elif (query_tag_type == "inner_text") :
                if (dict_tmp_input_fields["tag"] is None) :
                    PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => \"tag\" is necessary for this type.")

                if query_tag not in dict_definition_inner_text :
                    PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => You must declare the tag \"" + query_tag + "\" first.")

                dict_definition_output[offset_int_decimal]["write_type"] = WRITE_TYPE_STRING

                if need_slice_string :
                    tmp_length = len(dict_definition_inner_text[query_tag])
                    if (tmp_length < (query_end_index + 1)) or (tmp_length < (query_start_index + 1)) :
                        PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => \"end_index\" exceeds the maximum length \"" + str(tmp_length) + "\" defined in declaration area.")
                    tmp_write_data = reverseEndian(dict_definition_inner_text[query_tag][query_start_index:query_end_index + 1], query_reverse_endian)
                else :
                    tmp_write_data = reverseEndian(dict_definition_inner_text[query_tag], query_reverse_endian)

                if (tag_type == "mix_type") :
                    dict_definition_output[offset_int_decimal]["write_data"] = dict_definition_output[offset_int_decimal]["write_data"] + tmp_write_data
                else :
                    dict_definition_output[offset_int_decimal]["write_data"] = tmp_write_data

            elif (query_tag_type == "external") :
                if (dict_tmp_input_fields["key"] is None) :
                    PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => \"key\" is necessary for this type.")

                dict_definition_output[offset_int_decimal]["write_type"] = WRITE_TYPE_STRING

                if query_key not in dict_definition_external :
                    PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => You must declare the external key \"" + query_key + "\" in Python code first.")

                dict_definition_output[offset_int_decimal]["write_type"] = WRITE_TYPE_STRING

                if need_slice_string :
                    tmp_length = len(dict_definition_external[query_key])
                    if (tmp_length < (query_end_index + 1)) or (tmp_length < (query_start_index + 1)) :
                        PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => \"end_index\" exceeds the maximum length \"" + str(tmp_length) + "\" defined in declaration area.")
                    tmp_write_data = reverseEndian(dict_definition_external[query_key][query_start_index:query_end_index + 1], query_reverse_endian)
                else :
                    tmp_write_data = reverseEndian(dict_definition_external[query_key], query_reverse_endian)

                if (tag_type == "mix_type") :
                    dict_definition_output[offset_int_decimal]["write_data"] = dict_definition_output[offset_int_decimal]["write_data"] + tmp_write_data
                else :
                    dict_definition_output[offset_int_decimal]["write_data"] = tmp_write_data

            elif (query_tag_type == "boolean") :
                if (dict_tmp_input_fields["tag"] is None) or (dict_tmp_input_fields["attribute"] is None) or (dict_tmp_input_fields["bit"] is None) :
                    PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => \"tag\" and \"attribute\" and \"bit\" are necessary for this type.")

                tmp_current_bit = dict_tmp_input_fields["bit"]

                if tmp_current_bit in tmp_dict_input_bit_duplicated_counter :
                    PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => Duplicated \"bit\" index value \"" + str(tmp_current_bit) + "\" for the same efuse field.")

                if query_tag not in dict_definition_boolean :
                    PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => You must declare the tag \"" + query_tag + "\" first.")

                if query_attribute not in dict_definition_boolean[query_tag] :
                    PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => You must declare the attribute \"" + query_attribute + "\" in tag \"" + query_tag + "\" first.")

                tmp_dict_input_bit_duplicated_counter[tmp_current_bit] = dict_definition_boolean[query_tag][query_attribute]
                #later summarize in outer loop

            else :
                if (tag_type == "mix_type") :
                    PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => The \"type\" of the input field should not be empty if you use mix_type.")
                PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => This type is not supported.")


        if (tag_type == "boolean") :

            tmp_lst_all_input_bits_in_this_efuse_field = []
            for idx in tmp_dict_input_bit_duplicated_counter :
                tmp_lst_all_input_bits_in_this_efuse_field.append( (idx, tmp_dict_input_bit_duplicated_counter[idx]) )

            dict_definition_output[offset_int_decimal]["write_data"] = tmp_lst_all_input_bits_in_this_efuse_field
            dict_definition_output[offset_int_decimal]["write_type"] = WRITE_TYPE_BIT

        elif (tag_type == "mix_type") :

            if not tmp_dict_input_bit_duplicated_counter :
                PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => You should not use \"mix_type\" type because you do not set the type of any input field to \"boolean\".")

            tmp_lst_all_input_bits_in_this_efuse_field = []
            for idx in tmp_dict_input_bit_duplicated_counter :
                tmp_lst_all_input_bits_in_this_efuse_field.append( (idx, tmp_dict_input_bit_duplicated_counter[idx]) )

            if (len(dict_definition_output[offset_int_decimal]["write_data"]) > 8) :
                PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => If you want to use \"mix_type\" type, the total length of \"inner_value\" and \"inner_text\" input type of field should not exceed 8.")

            if (len(dict_definition_output[offset_int_decimal]["write_data"]) == 0) :
                PrintError_RaiseException_StopBuild("[definitions] Output offset at \"" + tag_offset + "\", type \"" + tag_type + "\" => If you want to use \"mix_type\" type, you should add at least one \"inner_value\" or one \"inner_text\" input type of field.")

            dict_definition_output[offset_int_decimal]["write_data2"] = tmp_lst_all_input_bits_in_this_efuse_field
            dict_definition_output[offset_int_decimal]["write_type"] = WRITE_TYPE_BIT_AND_STRING


    #print(dict_definition_output)

    #dic key: offset
    #dic value: type, final_value

    current_offset = 0

    with open(args.output_bin_name, "wb") as f :
        while (current_offset < MAX_OUTPUT_OFFSET_DECIMAL) :
            if current_offset in dict_definition_output :
                write_data = dict_definition_output[current_offset]["write_data"]
                write_type = dict_definition_output[current_offset]["write_type"]

                if write_type == WRITE_TYPE_BIT :
                    writeBitValueToFile(f, write_data)
                elif write_type == WRITE_TYPE_STRING :
                    writeHexStringToFile(f, write_data)
                elif write_type == WRITE_TYPE_BIT_AND_STRING :
                    write_data_bits = dict_definition_output[current_offset]["write_data2"]
                    writeBitValueAndOneHexStringToFile(f, write_data_bits, write_data)
                else :
                    writeBitValueToFile(f, [(0, 0)])
                    printAndLog("[definition] Unsupported write type in offset decimal: " + str(current_offset) + ".")
            else :
                writeBitValueToFile(f, [(0, 0)])

            current_offset = current_offset + 4


    bin_file_size_before_hash = os.path.getsize(args.output_bin_name)

    printAndLog("")
    sha256_hash = get_file_sha256hexdigest(args.output_bin_name)
    printAndLog("Image file(" + str(bin_file_size_before_hash) + " bytes) sha256 hash: " + sha256_hash)

    with open(args.output_bin_name, "ab") as f :
        writeHexStringToFile(f, sha256_hash[0:8]) #0x1E0 if 512bytes in size
        writeHexStringToFile(f, sha256_hash[8:16])
        writeHexStringToFile(f, sha256_hash[16:24])
        writeHexStringToFile(f, sha256_hash[24:32])
        writeHexStringToFile(f, sha256_hash[32:40])
        writeHexStringToFile(f, sha256_hash[40:48])
        writeHexStringToFile(f, sha256_hash[48:56])
        writeHexStringToFile(f, sha256_hash[56:64])

    printAndLog("Append sha256 hash to bin: Done!")

    bin_file_size = os.path.getsize(args.output_bin_name)

    printAndLog("")
    printAndLog("[Success] Write to bin: " + os.path.abspath(args.output_bin_name) + " (size: " + str(bin_file_size) + " bytes)")
    printAndLog("")


if __name__ == '__main__':
    main()
