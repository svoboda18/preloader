"""
This module is used to delegate signature generation to HSM(Hardware Security Module)
If public key is given for signing instead of private key, we'll know that
we're trying to delegate signature to HSM. Then we look up key table created
here to find HSM parameters. Here public key is used only as id for HSM
parameters and won't act as a public key.
"""
import filecmp
import os
import lib.cert

class HsmParam(object):
    """
    Parameter for HSM
    """
    def __init__(self):
        # you can add parameter required by your HSM here
        self.m_prvk = None

def create_key_table():
    """
    create key table for public key to private key mapping
    """
    prvk_list = []
    pubk_list = []
    key_database_path = os.path.join(os.path.dirname(__file__), 'hsm_test_keys')
    keys = os.listdir(key_database_path)
    key_table = {}

    for key in keys:
        key_path = os.path.join(key_database_path, key)
        if lib.cert.is_prvk(key_path):
            prvk_list.append(key_path)
        elif lib.cert.is_pubk(key_path):
            pubk_list.append(key_path)

    for pubk in pubk_list:
        for prvk in prvk_list:
            tmp_pubk = os.path.join(os.path.dirname(__file__), 'tmp_pubk.pem')
            lib.cert.prvk_to_pubk(prvk, tmp_pubk)
            if filecmp.cmp(pubk, tmp_pubk, False) is True:
                key_table[pubk] = os.path.join(key_database_path, prvk)
                os.remove(tmp_pubk)
                break
            os.remove(tmp_pubk)

    return key_table

def query_key_table(key_table, key):
    """
    get private key from public key.
    In your implementation, you should convert input public
    key to parameter passed to HSM, so HSM knows how to sign
    message. Here as an example, we search public keys in a folder
    as public key data base, and use corresponding private key
    to sign message.
    """
    for pubk in key_table.keys():
        if filecmp.cmp(key, pubk, False) is True:
            return key_table[pubk]

    return None


def hsm_rsa_sign(data, key, padding, sig):
    """
    sign data with HSM
    """
    # note that key is pubk actually, use it as index for
    # HSM parameters such as key selection
    hsm_param_obj = HsmParam()
    key_table = create_key_table()
    hsm_param_obj.m_prvk = query_key_table(key_table, key)
    if hsm_param_obj.m_prvk is None:
        print 'not valid HSM parameter'
        return -1

    print "========================"
    print "HSM parameter:"
    print "    m_prvk  = " + hsm_param_obj.m_prvk
    print "========================"

    # place hsm request here -- start
    # we re-direct it to signing with private key to mimic HSM
    # data is not hashed here, you can hash data here to reduce
    # network usage
    lib.cert.sig_gen(data, hsm_param_obj.m_prvk, padding, sig)
    # place hsm request here -- end
    return 0
