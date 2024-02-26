"""
This module checks preloader project setting in order to
avoid releasing incorrect configuration to customer.
"""
import os
import re
import shutil
import argparse
import sys

class CheckSetting(object):
    """
    CheckSetting class for checking project setting
    """
    def __init__(self):
        self.sboot_forbidden = "ATTR_SBOOT_DISABLE"
        self.usbdl_forbidden = "ATTR_SUSBDL_DISABLE"
        self.sboot_allow = "ATTR_SBOOT_ONLY_ENABLE_ON_SCHIP"
        self.usbdl_allow = "ATTR_SUSBDL_ONLY_ENABLE_ON_SCHIP"
        self.bypass_pattern = ["evb", "fpga", "tb"]

    def analyze_and_correct(self, in_path):
        """
        analyze and correct the project setting.
        """
        file_path = os.path.abspath(in_path)
        if not os.path.isfile(file_path):
            print "ERROR: " + in_path + " does not exist."
            sys.exit()

        file_name = os.path.basename(file_path)

        for elem in self.bypass_pattern:
            if re.search(elem, file_name):
                print "No need to check this project (Not phone project)."
                return

        file_dir = os.path.dirname(file_path)
        tmp_file = os.path.join(file_dir, file_name.split(".")[0] + "_temp." + file_name.split(".")[1])
        f_in = open(file_path,'r')
        f_out = open(tmp_file,'w')
        for line in f_in:
            if re.search(self.sboot_forbidden, line):
                f_out.write(line.replace(self.sboot_forbidden, self.sboot_allow))
                print "Replace " + line.strip() + " with " + line.replace(self.sboot_forbidden, self.sboot_allow).strip()
            elif re.search(self.usbdl_forbidden, line):
                f_out.write(line.replace(self.usbdl_forbidden, self.usbdl_allow))
                print "Replace " + line.strip() + " with " + line.replace(self.usbdl_forbidden, self.usbdl_allow).strip()
            else:
                f_out.write(line)

        f_in.close()
        f_out.close()
        os.remove(file_path)
        shutil.move(tmp_file, file_path)
        print "Check and correct successfully!!!"

def main():
    """
    entry point for secure boot config check
    """
    get_setting_str = lambda arg: 'Not Set' if arg is None else arg
    parser = argparse.ArgumentParser(description='Secure boot configuration check script.')
    parser.add_argument('-i',
                        dest='in_path',
                        help='configuration file path',
                        required = True)

    input_args = parser.parse_args()

    print "=========================================="
    print "in_path:" + get_setting_str(input_args.in_path)
    print "=========================================="

    check = CheckSetting()
    check.analyze_and_correct(input_args.in_path)

if __name__ == '__main__':
    main()
