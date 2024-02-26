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

    parser = argparse.ArgumentParser(description='MediaTek EFUSE XML Parser')
    parser.add_argument('--file', '-f',
                        required=True,
                        help='Provide the xml file')
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
    printAndLog("****************************** version 1.3.6.2 ****************************")
    printAndLog("***************************************************************************")

    printAndLog("Loading XML file: " + os.path.abspath(args.file))

    if os.path.isfile(args.output_bin_name) :
        os.remove(args.output_bin_name)
        printAndLog("Remove old image file: " + os.path.abspath(args.output_bin_name))
        printAndLog("-----------------------------------------------")

    if not os.path.isfile(args.file) :
        PrintError_RaiseException_StopBuild("XML file not exist!!")

    try :
        xml_file = minidom.parse(args.file)
    except Exception:
        printAndLog("[Error] ***** XML format is NOT CORRECT. Please check your XML input file. *****")
        printAndLog("[Error] ***** XML format is NOT CORRECT. Please check your XML input file. *****")
        PrintError_RaiseException_StopBuild("***** XML format is NOT CORRECT. Please check your XML input file. *****")

    #Parsing XML to variable
    printAndLog("Parsing XML file ...")

    #Parse String value
    EFUSE_KEY1 = parseXmlTagAndAttribute(xml_file, "magic-key", "key1", 8, True)
    EFUSE_KEY2 = parseXmlTagAndAttribute(xml_file, "magic-key", "key2", 8, True)
    EFUSE_ac_key = parseXmlTagInnerValue(xml_file, "ac-key", 32, True)
    EFUSE_usb_vid = parseXmlTagAndAttribute(xml_file, "usb-id", "vid", 4, True)
    EFUSE_usb_pid = parseXmlTagAndAttribute(xml_file, "usb-id", "pid", 4, True)

    #Parse Integer value
    EFUSE_Disable_NAND_boot_speedup = parseXmlTagAndAttribute(xml_file, "common-ctrl", "Disable_NAND_boot_speedup")
    EFUSE_USB_download_type = parseXmlTagAndAttribute(xml_file, "common-ctrl", "USB_download_type")
    EFUSE_Disable_NAND_boot = parseXmlTagAndAttribute(xml_file, "common-ctrl", "Disable_NAND_boot")
    EFUSE_Disable_EMMC_boot = parseXmlTagAndAttribute(xml_file, "common-ctrl", "Disable_EMMC_boot")
    EFUSE_Enable_SW_JTAG_CON = parseXmlTagAndAttribute(xml_file, "secure-ctrl", "Enable_SW_JTAG_CON")
    EFUSE_Enable_Root_Cert = parseXmlTagAndAttribute(xml_file, "secure-ctrl", "Enable_Root_Cert")
    EFUSE_Enable_ACC = parseXmlTagAndAttribute(xml_file, "secure-ctrl", "Enable_ACC")
    EFUSE_Enable_ACK = parseXmlTagAndAttribute(xml_file, "secure-ctrl", "Enable_ACK")
    EFUSE_Enable_SLA = parseXmlTagAndAttribute(xml_file, "secure-ctrl", "Enable_SLA")
    EFUSE_Enable_DAA = parseXmlTagAndAttribute(xml_file, "secure-ctrl", "Enable_DAA")
    EFUSE_Enable_SBC = parseXmlTagAndAttribute(xml_file, "secure-ctrl", "Enable_SBC")
    EFUSE_Disable_JTAG = parseXmlTagAndAttribute(xml_file, "secure-ctrl", "Disable_JTAG")
    EFUSE_Disable_DBGPORT_LOCK = parseXmlTagAndAttribute(xml_file, "secure-ctrl", "Disable_DBGPORT_LOCK")

    EFUSE_C_SEC_CTRL = parseXmlTagAndAttribute(xml_file, "cust-secure-ctrl", "c_sec_ctrl", 2, True)
    EFUSE_C_CTRL = parseXmlTagAndAttribute(xml_file, "cust-common-ctrl", "c_ctrl", 2, True)
    EFUSE_DISABLE_EFUSE = parseXmlTagAndAttribute(xml_file, "cust-common-ctrl", "DISABLE_EFUSE_BLOW")

    EFUSE_com_ctrl_lock = parseXmlTagAndAttribute(xml_file, "common-lock", "com_ctrl_lock")
    EFUSE_usb_id_lock = parseXmlTagAndAttribute(xml_file, "common-lock", "usb_id_lock")
    EFUSE_sec_attr_lock = parseXmlTagAndAttribute(xml_file, "secure-lock", "sec_attr_lock")
    EFUSE_ackey_lock = parseXmlTagAndAttribute(xml_file, "secure-lock", "ackey_lock")
    EFUSE_sbc_pubk_hash_lock = parseXmlTagAndAttribute(xml_file, "secure-lock", "sbc_pubk_hash_lock")

    if (int(EFUSE_C_CTRL, 16) & 0x80) != 0 :  #bit[7] is not masked
        PrintError_RaiseException_StopBuild("C_CTRL should only have bit[6:0]. The bit[7] should not be set.")

    printAndLog("Parsing XML file ... Done")

    #Pre-process value

    printAndLog("-----------------------------------------------")

    #[Important] Please make sure the value of "print" is the same as xml value

    printAndLog("EFUSE_ac_key = " + str(EFUSE_ac_key))
    printAndLog("EFUSE_usb_vid = " + str(EFUSE_usb_vid))
    printAndLog("EFUSE_usb_pid = " + str(EFUSE_usb_pid))
    printAndLog("EFUSE_Disable_NAND_boot_speedup = " + str(EFUSE_Disable_NAND_boot_speedup))
    printAndLog("EFUSE_USB_download_type = " + str(EFUSE_USB_download_type))
    printAndLog("EFUSE_Disable_NAND_boot = " + str(EFUSE_Disable_NAND_boot))
    printAndLog("EFUSE_Disable_EMMC_boot = " + str(EFUSE_Disable_EMMC_boot))
    printAndLog("EFUSE_Enable_SW_JTAG_CON = " + str(EFUSE_Enable_SW_JTAG_CON))
    printAndLog("EFUSE_Enable_Root_Cert = " + str(EFUSE_Enable_Root_Cert))
    printAndLog("EFUSE_Enable_ACC = " + str(EFUSE_Enable_ACC))
    printAndLog("EFUSE_Enable_ACK = " + str(EFUSE_Enable_ACK))
    printAndLog("EFUSE_Enable_SLA = " + str(EFUSE_Enable_SLA))
    printAndLog("EFUSE_Enable_DAA = " + str(EFUSE_Enable_DAA))
    printAndLog("EFUSE_Enable_SBC = " + str(EFUSE_Enable_SBC))
    printAndLog("EFUSE_Disable_JTAG = " + str(EFUSE_Disable_JTAG))
    printAndLog("EFUSE_Disable_DBGPORT_LOCK = " + str(EFUSE_Disable_DBGPORT_LOCK))
    printAndLog("EFUSE_C_SEC_CTRL = " + str(EFUSE_C_SEC_CTRL))
    printAndLog("EFUSE_DISABLE_EFUSE = " + str(EFUSE_DISABLE_EFUSE))
    printAndLog("EFUSE_C_CTRL = " + str(hex(int(EFUSE_C_CTRL, 16) & 0x7F))[2:].zfill(2).upper())  #Mask 0x7F
    printAndLog("EFUSE_com_ctrl_lock = " + str(EFUSE_com_ctrl_lock))
    printAndLog("EFUSE_usb_id_lock = " + str(EFUSE_usb_id_lock))
    printAndLog("EFUSE_sec_attr_lock = " + str(EFUSE_sec_attr_lock))
    printAndLog("EFUSE_ackey_lock = " + str(EFUSE_ackey_lock))
    printAndLog("EFUSE_sbc_pubk_hash_lock = " + str(EFUSE_sbc_pubk_hash_lock))

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

    printAndLog("EFUSE_SBC_PUBK_HASH = " + EFUSE_SBC_PUBK_HASH)

    printAndLog("-----------------------------------------------")

    #usb-vid and usb-pid are special cases (The priority is important! PUT IT AFTER PRINT)
    EFUSE_usb_vid = EFUSE_usb_vid[2:4] + EFUSE_usb_vid[0:2]
    EFUSE_usb_pid = EFUSE_usb_pid[2:4] + EFUSE_usb_pid[0:2]


    with open(args.output_bin_name, "wb") as f :
        writeHexStringToFile(f, EFUSE_KEY1)  #0x0
        writeHexStringToFile(f, EFUSE_KEY2)  #0x4
        writeBitValueToFile(f, [(0, 0)])  #0x8
        writeHexStringToFile(f, EFUSE_ac_key[0:8])  #0xC
        writeHexStringToFile(f, EFUSE_ac_key[8:16]) #0x10

        writeHexStringToFile(f, EFUSE_ac_key[16:24])  #0x14
        writeHexStringToFile(f, EFUSE_ac_key[24:32])  #0x18
        writeHexStringToFile(f, EFUSE_SBC_PUBK_HASH[0:8])  #0x1C
        writeHexStringToFile(f, EFUSE_SBC_PUBK_HASH[8:16]) #0x20

        writeHexStringToFile(f, EFUSE_SBC_PUBK_HASH[16:24])  #0x24
        writeHexStringToFile(f, EFUSE_SBC_PUBK_HASH[24:32])  #0x28
        writeHexStringToFile(f, EFUSE_SBC_PUBK_HASH[32:40])  #0x2C
        writeHexStringToFile(f, EFUSE_SBC_PUBK_HASH[40:48])  #0x30

        writeHexStringToFile(f, EFUSE_SBC_PUBK_HASH[48:56])  #0x34
        writeHexStringToFile(f, EFUSE_SBC_PUBK_HASH[56:64])  #0x38
        writeHexStringToFile(f, EFUSE_usb_pid)  #0x3C  (only 4 digits, auto padding to 8 digits in writeHexStringToFile)
        writeHexStringToFile(f, EFUSE_usb_vid)  #0x40  (only 4 digits, auto padding to 8 digits in writeHexStringToFile)

        writeBitValueToFile(f, [(0, EFUSE_Disable_EMMC_boot), (1, EFUSE_Disable_NAND_boot), (2, EFUSE_USB_download_type), (4, EFUSE_Disable_NAND_boot_speedup)])  #0x44
        writeBitValueToFile(f, [(0, EFUSE_Disable_JTAG), (1, EFUSE_Enable_SBC), (2, EFUSE_Enable_DAA), (3, EFUSE_Enable_SLA),(4, EFUSE_Enable_ACK), (5, EFUSE_Enable_ACC), (6, EFUSE_Enable_SW_JTAG_CON), (7, EFUSE_Enable_Root_Cert), (9, EFUSE_Disable_DBGPORT_LOCK)])  #0x48
        writeHexStringToFile(f, EFUSE_C_SEC_CTRL)  #0x4C    (only 2 digits, auto padding to 8 digits in writeHexStringToFile)

        if EFUSE_DISABLE_EFUSE == 1 :
            writeBitValueAndOneHexStringToFile(f, [(7, 1)], EFUSE_C_CTRL)  #0x50
        else :
            writeBitValueAndOneHexStringToFile(f, [(7, 0)], EFUSE_C_CTRL)  #0x50   #[IMPORTANT] You MUST still set bit 7 to value 0 as a mask because the priority of bit input(lstBitValue) is higher than hex input(hexString).

        writeBitValueToFile(f, [(0, EFUSE_sbc_pubk_hash_lock), (1, EFUSE_ackey_lock), (2, EFUSE_sec_attr_lock)])  #0x54
        writeBitValueToFile(f, [(1, EFUSE_usb_id_lock), (2, EFUSE_com_ctrl_lock)])  #0x58

        #extends to 512 bytes file siz
        for i in range(97) : #0x5C
            writeBitValueToFile(f, [(0, 0)])

    bin_file_size_before_hash = os.path.getsize(args.output_bin_name)

    printAndLog("")
    sha256_hash = get_file_sha256hexdigest(args.output_bin_name)
    printAndLog("Image file(" + str(bin_file_size_before_hash) + " bytes) sha256 hash: " + sha256_hash)

    with open(args.output_bin_name, "ab") as f :
        writeHexStringToFile(f, sha256_hash[0:8])  #0x1E0
        writeHexStringToFile(f, sha256_hash[8:16]) #0x1E4
        writeHexStringToFile(f, sha256_hash[16:24])  #0x1E8
        writeHexStringToFile(f, sha256_hash[24:32])  #0x1EC
        writeHexStringToFile(f, sha256_hash[32:40])  #0x1F0
        writeHexStringToFile(f, sha256_hash[40:48])  #0x1F4
        writeHexStringToFile(f, sha256_hash[48:56])  #0x1F8
        writeHexStringToFile(f, sha256_hash[56:64])  #0x1FC

    printAndLog("Append sha256 hash to bin: Done!")

    bin_file_size = os.path.getsize(args.output_bin_name)

    printAndLog("")
    printAndLog("[Success] Write to bin: " + os.path.abspath(args.output_bin_name) + " (size: " + str(bin_file_size) + " bytes)")
    printAndLog("")


if __name__ == '__main__':
    main()
