"""
pbp is a tool that signs/re-signs bootloader and generate data for root public key authentication.
"""
import os
import shutil
import argparse
from lib import gfh
from lib import cert


def get_file_sizeb(file_path):
    """
    Get size of binary file
    """
    if not os.path.isfile(file_path):
        return 0
    file_handle = open(file_path, "rb")
    file_handle.seek(0, 2)
    file_size = file_handle.tell()
    file_handle.close()
    return file_size


def concatb(file1_path, file2_path):
    """
    Concatenate binary files
    """
    file2_size = get_file_sizeb(file2_path)
    file1 = open(file1_path, "ab+")
    file2 = open(file2_path, "rb")
    file1.write(file2.read(file2_size))
    file2.close()
    file1.close()


class Bl(object):
    """
    Bl, which stands for preloader in Mediatek solution.
    Mediatek preloader is loaded/verified by BootROM and its format is determined by BootROM
    and is different from other images due to several reasons.
    It has basic format as follows:
    =======================
    GFH
    =======================
    Preloader_NO_GFH.bin
    =======================
    Sig
    =======================
    Where Preloader_NO_GFH.bin is converted from preloader.elf.
    """
    def __init__(self, out_path, in_bootloader_file_path, out_bootloader_file_path):
        self.m_gfh = gfh.ImageGFH()
        self.m_out_path = out_path
        if not os.path.exists(self.m_out_path):
            os.makedirs(self.m_out_path)
        self.m_in_bl_file_path = in_bootloader_file_path
        self.m_out_bl_file_path = out_bootloader_file_path
        self.m_bl_is_signed = False
        self.m_bl_content_offset = 0
        # initialize content size to bl file size
        self.m_bl_content_length = get_file_sizeb(self.m_in_bl_file_path)
        self.m_bl_sig_size = 0
        # generate file path for bl without gfh and signature
        bl_path = os.path.splitext(in_bootloader_file_path)
        self.m_bl_no_gfh_file_path = bl_path[0] + "_plain.bin"
        self.m_sig_ver = 0
        self.m_sw_ver = 0
        self.m_root_prvk_path = ""
        self.m_img_prvk_path = ""
        self.m_ac_key = 0
        self.m_sig_handler = None

    def is_signed(self):
        """
        GFH and signature are added after bootloader image has been processed by pbp.
        We use this fact to determine whether bootloader image is signed.
        """
        if self.m_in_bl_file_path:
            bl_file = open(self.m_in_bl_file_path, "rb")
            gfh_hdr_obj = gfh.GFHHeader()
            gfh_hdr_size = gfh_hdr_obj.get_size()
            gfh_hdr_buf = bl_file.read(gfh_hdr_size)
            self.m_bl_is_signed = gfh_hdr_obj.is_gfh(gfh_hdr_buf)
            bl_file.close()
        return self.m_bl_is_signed

    def parse(self):
        """
        If image is signed, we remove GFH and signature. Removed GFH is parsed and
        stored. Stored GFH will be used later if GFH ini file is not given.
        """
        print "===parse bootloader==="
        # image will be decomposed if it's signed
        if self.is_signed():
            gfh_total_size = self.m_gfh.parse(self.m_in_bl_file_path)
            self.m_bl_content_offset = gfh_total_size
            self.m_bl_content_length -= gfh_total_size
            self.m_bl_content_length -= self.m_gfh.get_sig_size()
            self.m_bl_sig_size = self.m_gfh.get_sig_size()
            in_file = open(self.m_in_bl_file_path, "rb")
            out_file = open(self.m_bl_no_gfh_file_path, "wb")
            in_file.seek(self.m_bl_content_offset)
            out_file.write(in_file.read(self.m_bl_content_length))
            out_file.close()
            in_file.close()
        else:
            shutil.copyfile(self.m_in_bl_file_path, self.m_bl_no_gfh_file_path)
        print "bootloader content size = " + hex(self.m_bl_content_length)

    def create_gfh(self, gfh_config):
        """
        GFH creation. GFH may be created from parsed/stored GFH config or from GFH config file
        provided by user.
        """
        self.parse()
        if gfh_config:
            if self.is_signed():
                del self.m_gfh.gfhs[:]
            self.m_gfh.load_ini(gfh_config)
        elif not self.is_signed():
            print "GFH_CONFIG.ini does not exist!!"
            return -1
        # self.m_gfh.dump()
        return 0

    def sign(self, key_ini_path, key_cert_path, content_config_file_path):
        """
        Sign bootloader according to its signature type, which is stored in GFH.
        """
        self.m_gfh.finalize(self.m_bl_content_length, key_ini_path)
        # create tbs_bootloader.bin
        tbs_bl_file_path = os.path.join(self.m_out_path, "tbs_preloader.bin")
        tbs_bl_file = open(tbs_bl_file_path, "wb")
        tbs_bl_file.write(self.m_gfh.pack())
        bl_no_gfh_file = open(self.m_bl_no_gfh_file_path, "rb")
        tbs_bl_file.write(bl_no_gfh_file.read(self.m_bl_content_length))
        bl_no_gfh_file.close()
        tbs_bl_file.close()
        print "===sign==="
        if self.m_gfh.get_sig_type() == "CERT_CHAIN":
            self.m_sig_handler = cert.CertChainV2()
            # create key cert if key cert does not exist
            if key_cert_path == "":
                key_cert_path = os.path.join(self.m_out_path, "key_cert.bin")
            if not os.path.isfile(key_cert_path):
                key_cert_file_name = os.path.basename(os.path.abspath(key_cert_path))
                self.m_sig_handler.create_key_cert(key_ini_path,
                                                   self.m_out_path,
                                                   key_cert_file_name)
                key_cert_path = os.path.join(self.m_out_path, key_cert_file_name)
            else:
                self.m_sig_handler.set_key_cert(key_cert_path)
            # create content cert
            content_cert_name = "content_cert.bin"
            self.m_sig_handler.create_content_cert(content_config_file_path,
                                                   tbs_bl_file_path,
                                                   self.m_out_path,
                                                   content_cert_name)
            # create final cert chain
            sig_name = "preloader.sig"
            sig_file_path = os.path.join(self.m_out_path, sig_name)
            self.m_sig_handler.output(self.m_out_path, sig_name)
            # output final cert chain size
            sig_size_name = "sig_size.txt"
            sig_size_file_path = os.path.join(self.m_out_path, sig_size_name)
            sig_size_file = open(sig_size_file_path, 'w')
            sig_size_file.write(hex(get_file_sizeb(sig_file_path)))
            sig_size_file.close()
            # create final preloader image
            if os.path.isfile(self.m_out_bl_file_path):
                os.remove(self.m_out_bl_file_path)
            concatb(self.m_out_bl_file_path, tbs_bl_file_path)
            concatb(self.m_out_bl_file_path, sig_file_path)
            # clean up
            os.remove(os.path.join(self.m_out_path, content_cert_name))
        elif self.m_gfh.get_sig_type() == "SINGLE_AND_PHASH":
            self.m_sig_handler = cert.SigSingleAndPhash(self.m_gfh.get_pad_type())
            self.m_sig_handler.set_out_path(self.m_out_path)
            self.m_sig_handler.create(key_ini_path, tbs_bl_file_path)
            # signature generation
            self.m_sig_handler.sign()
            sig_name = "preloader.sig"
            sig_file_path = os.path.join(self.m_out_path, sig_name)
            self.m_sig_handler.output(self.m_out_path, sig_name)
            # output signature size
            sig_size_name = "sig_size.txt"
            sig_size_file_path = os.path.join(self.m_out_path, sig_size_name)
            sig_size_file = open(sig_size_file_path, 'w')
            sig_size_file.write(hex(get_file_sizeb(sig_file_path)))
            sig_size_file.close()
            # create final preloader image
            if os.path.isfile(self.m_out_bl_file_path):
                os.remove(self.m_out_bl_file_path)
            concatb(self.m_out_bl_file_path, tbs_bl_file_path)
            concatb(self.m_out_bl_file_path, sig_file_path)
        else:
            print "unknown signature type"
        # clean up
        os.remove(self.m_bl_no_gfh_file_path)
        os.remove(tbs_bl_file_path)
        os.remove(sig_file_path)
        return

class PbpArgs(object):
    """
    PbpArgs is used to pass parameter to pbp.
    This structure is both used when user executes this python script directly or imports
    this module and use exported method.
    """
    def __init__(self):
        self.op = None
        self.padding = None
        self.key_ini_path = None
        self.key_path = None
        self.gfh_cfg_ini_path = None
        self.cnt_cfg_ini_path = None
        self.key_cert_path = None
        self.input_bl_path = None
        self.tmp_output_path = None
        self.output_path = None
    def reset(self):
        self.__init__()
    def dump(self):
        """
        dump parameters.
        """
        f = lambda arg: 'Not Set' if arg is None else arg
        print "op = " + f(self.op)
        print "padding = " + f(self.padding)
        print "key_ini_path = " + f(self.key_ini_path)
        print "key_path = " + f(self.key_path)
        print "gfh_cfg_ini_path = " + f(self.gfh_cfg_ini_path)
        print "cnt_cfg_ini_path = " + f(self.cnt_cfg_ini_path)
        print "key_cert_path = " + f(self.key_cert_path)
        print "input_bl_path = " + f(self.input_bl_path)
        print "tmp_output_path = " + f(self.tmp_output_path)
        print "output_path = " + f(self.output_path)

def _op_sign(args):
    """
    Sign/re-sign operation
    """
    bl_obj = Bl(args.tmp_output_path, args.input_bl_path, args.output_path)
    bl_obj.create_gfh(args.gfh_cfg_ini_path)
    bl_obj.sign(args.key_ini_path, args.key_cert_path, args.cnt_cfg_ini_path)
    return 0

def _op_keybin(args):
    """
    Generate root key data structure for root public key authentication.
    """
    key = cert.CtKey(args.padding)
    key.create(args.key_path)
    key_bin = key.pack()
    out_file = open(args.output_path, "wb")
    out_file.write(key_bin)
    out_file.close()
    return 0

def _op_keybin_pss(args):
    """
    Root key data structures are different for different padding. Here we handles pss padding.
    """
    args.padding = 'pss'
    return _op_keybin(args)

def _op_keybin_legacy(args):
    """
    Root key data structures are different for different padding. Here we handles legacy padding.
    """
    args.padding = 'legacy'
    return _op_keybin(args)

def _op_keyhash(args):
    """
    Generate hash of root key data structure, which is dependent on padding used.
    """
    key = cert.CtKey(args.padding)
    key.create(args.key_path)
    key_bin = key.pack()
    tmp_key_bin_path = os.path.join(args.tmp_output_path, "tmp_keybin.bin")
    out_file = open(tmp_key_bin_path, "wb")
    out_file.write(key_bin)
    out_file.close()
    cert.hash_gen(tmp_key_bin_path, args.output_path)
    os.remove(tmp_key_bin_path)
    return 0

def _op_keyhash_pss(args):
    """
    Root key data struture hash for pss padding.
    """
    args.padding = 'pss'
    return _op_keyhash(args)

def _op_keyhash_legacy(args):
    """
    Root key data struture hash for legacy padding.
    """
    args.padding = 'legacy'
    return _op_keyhash(args)

def pbp_op(args):
    """
    Handles and dispatches all operations supported by pbp.
    """
    supported_ops = {
        'sign': _op_sign,
        'keybin_pss': _op_keybin_pss,
        'keybin_legacy': _op_keybin_legacy,
        'keyhash_pss': _op_keyhash_pss,
        'keyhash_legacy': _op_keyhash_legacy
    }

    if args.output_path is None:
        print "output path is not given!"
        return -1

    if args.op is None:
        print "op is not given!"
        return -1

    if args.op == 'sign':
        if not args.input_bl_path:
            print "bootloader path is not given!"
            return -1
        if (args.key_ini_path is None) and (args.key_cert_path is None):
            print "key path is not given!"
            return -1
    else:
        if (args.key_ini_path is None) and (args.key_path is None):
            print "key path is not given!"
            return -1

    args.tmp_output_path = os.path.dirname(os.path.abspath(args.output_path))
    if not os.path.exists(args.tmp_output_path):
        os.makedirs(args.tmp_output_path)

    op_f = supported_ops.get(args.op)
    return op_f(args)


def main():
    """
    Main function for pbp, which is used when pbp.py is executed directly.
    Note that we changed input bootloader parameter to -in_bl $BL_PATH.
    Please remember to add -in_bl if you're migrating from previous version.
    """
    parser = argparse.ArgumentParser(description='pbp tool for preloader gfh \
creation/replacement and signing/re-signing')
    parser.add_argument('-i', dest='key_ini_path', help='key configuartion path')
    parser.add_argument('-j', dest='key_path', help='key path (with pem format)')
    parser.add_argument('-g', dest='gfh_cfg_ini_path', help='gfh(generaic file header) \
configuration path')
    parser.add_argument('-c', dest='cnt_cfg_ini_path', help='content certificate \
configuration path')
    parser.add_argument('-k', dest='key_cert_path', help='key certificate path')
    parser.add_argument('-func', dest='op', help='operation to be performed', required=True)
    parser.add_argument('-o', dest='output_path', help='output file path')
    parser.add_argument('input_bl_path', nargs='?', help='input file path')

    pbp_args = parser.parse_args()
    return pbp_op(pbp_args)


if __name__ == '__main__':
    main()
