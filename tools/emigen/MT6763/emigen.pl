#!/usr/local/bin/perl
#
#*****************************************************************************
#  Copyright Statement:
#  --------------------
#  This software is protected by Copyright and the information contained
#  herein is confidential. The software may not be copied and the information
#  contained herein may not be used or disclosed except with the written
#  permission of MediaTek Inc. (C) 2008
#
#  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
#  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
#  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
#  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
#  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
#  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
#  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
#  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
#  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
#  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
#  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
#  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
#
#  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
#  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
#  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
#  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
#  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#
#  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
#  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
#  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
#  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
#  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
#
#****************************************************************************/
#*
#* Filename:
#* ---------
#*   emigen_sp.pl
#*
#* Project:
#* --------
#*
#*
#* Description:
#* ------------
#*   This script will
#*       1. parse custom_MemoryDevice.h to get memory device type and part number
#*       2. read a excel file to get appropriate emi setting based on the part number
#*       3. based on the emi settings, generate custom_EMI.c if not exist
#*       4. based on the emi settings, generate custom_EMI.h if not exist
#*
#* Author:
#* -------
#*
#*============================================================================
#*             HISTORY
#* Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
#*------------------------------------------------------------------------------
#* $Revision$
#* $Modtime$
#* $Log$
#*
#*
#*------------------------------------------------------------------------------
#* Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
#*============================================================================
#****************************************************************************/

#****************************************************************************
# Included Modules
#****************************************************************************


#   original design, but perl does not support array of structure, really?
#
#my $CustomChip = () ;
#
#
#       an array of following structure:
#
#       CustChip => NAND_ID
#                => CS0_PART_NUMBER
#                => CS1_PART_NUMBER
#
#
#
#
#
#
use File::Basename;

my $os = &OsName();
my $build_opt = $ARGV[5];
my $start_num;

BEGIN
{
  $LOCAL_PATH = dirname($0);
}

print "LOCAL_PATH: $LOCAL_PATH\n";
#use lib "$LOCAL_PATH/../Spreadsheet";
use lib "$LOCAL_PATH/../";
#require 'ParseExcel.pm';
use pack_dep_gen;

if ($os eq "windows")
{
    use strict;
    &gen_pm;
    require 'ForWindows.pm';
    $Win32::OLE::Warn = 3;
    $start_num = 1;
}
elsif ($os eq "linux")
{
    print "Os = linux\n";
    use lib "$LOCAL_PATH/../Spreadsheet";
    require 'ParseExcel.pm';
    $start_num = 0;
}
else
{
  die "unknow OS!\n";
}

#****************************************************************************
# PLATFORM EMI support matrix
#****************************************************************************
my %BBtbl_LPSDRAM =
(
	'MT6763'  => 1,
);

#****************************************************************************
# Constants
#****************************************************************************
my $EMIGEN_VERNO  = " V0.01";
                    # V0.01, Zhen Jiang, Porting emigen to DUMA project
                    #
my $DebugPrint    = 1; # 1 for debug; 0 for non-debug

my $COLUMN_VENDOR               = $start_num + 0;
my $COLUMN_PART_NUMBER	        = $COLUMN_VENDOR + 1 ;
my $COLUMN_TYPE	                = $COLUMN_PART_NUMBER + 1 ;
my $COLUMN_MODE	                = $COLUMN_TYPE + 1 ;
my $COLUMN_DENSITY	            = $COLUMN_MODE + 1 ;
my $COLUMN_BOARD_ID	            = $COLUMN_DENSITY + 1 ;
my $COLUMN_NAND_EMMC_ID	        = $COLUMN_BOARD_ID + 1 ;
my $COLUMN_FW_ID	        = $COLUMN_NAND_EMMC_ID + 1 ;
my $COLUMN_NAND_PAGE_SIZE       = $COLUMN_FW_ID + 1 ;
my $COLUMN_PLATFORM             = $COLUMN_NAND_PAGE_SIZE + 1 ;

my $CUSTOM_MEMORY_DEVICE_HDR  = $ARGV[0]; # src\custom\<project>, need full path for now
#my $MEMORY_DEVICE_LIST_XLS    = Win32::GetCwd()."\\memorydevicelist\\".$ARGV[1];
my $MEMORY_DEVICE_LIST_XLS    = $ARGV[1];
my $PLATFORM                  = $ARGV[2]; # MTxxxx
my $PROJECT               = $ARGV[3];
my $MTK_EMIGEN_OUT_DIR = $ARGV[4];
#my $MTK_MACH_TYPE = $ARGV[6]; # Darren

#my $MTK_EMIGEN_OUT_DIR = "$ENV{PRODUCT_OUT}/EMIGEN";


print "$CUSTOM_MEMORY_DEVICE_HDR\n$MEMORY_DEVICE_LIST_XLS\n$PLATFORM\n" if ($DebugPrint == 1);
print "$MTK_EMIGEN_OUT_DIR\n" if ($DebugPrint == 1);

# following parameters come from $CUSTOM_MEMORY_DEVICE_HDR
my $MEMORY_DEVICE_TYPE;

# data structure of $part_number if ($MEMORY_DEVICE_TYPE eq 'LPSDRAM')
#
# my $part_info =
# {
#    CS       => { "0" => { PART_NUMBER     => $part_number,
#                           EXCEL_ROW       => $excel_row,
#                           VENDOR          => $vendor,
my $part_info     = ();   # has different data structures for different $MEMORY_DEVICE_TYPE

my $bank_num = 0;         #  0: No memory is attached
                          #  1: 1 is attached
                          #  2: 2 are attached

# locations of output EMI settings
# src\custom\<project>\DRV\bootloader\EMI
my $CUSTOM_EMI_H = $CUSTOM_MEMORY_DEVICE_HDR;
#my $CUSTOM_EMI_C = $CUSTOM_MEMORY_DEVICE_HDR;
my $INFO_TAG = $CUSTOM_MEMORY_DEVICE_HDR;

if ($os eq "windows")
{
    $CUSTOM_EMI_H = "$MTK_EMIGEN_OUT_DIR/custom_emi.h";
#    $CUSTOM_EMI_C = "$MTK_EMIGEN_OUT_DIR/custom_emi.c";
    `mkdir output` unless (-d "output");
}
elsif ($os eq "linux")
{
    $CUSTOM_EMI_H = "$MTK_EMIGEN_OUT_DIR/custom_emi.h";
#    $CUSTOM_EMI_C = "$MTK_EMIGEN_OUT_DIR/custom_emi.c";
    $INFO_TAG     = "$MTK_EMIGEN_OUT_DIR/MTK_Loader_Info.tag";
}
# Pregen: after we load all perl module, it need to print all depend module
PrintDependModule($0);
print "$CUSTOM_EMI_H\n$INFO_TAG\n" if ($DebugPrint ==1);

# check existance of custom_EMI.h and custom_EMI.c
my $is_existed_h             = (-e $CUSTOM_EMI_H)?           1 : 0;
#my $is_existed_c             = (-e $CUSTOM_EMI_C)?           1 : 0;
#
#if ( ($is_existed_h == 1) && ($is_existed_c == 1) )
#{
#   print "\n\nALL custom_EMI\.h, custom_EMI\.c are existed!!!\n\n\n";
#   exit;
#}


#****************************************************************************
# parse custom_MemoryDevice.h to extract MEMORY_DEVICE_TYPE & PART_NUMBER
#****************************************************************************
open CUSTOM_MEMORY_DEVICE_HDR, "<$CUSTOM_MEMORY_DEVICE_HDR" or &error_handler("$CUSTOM_MEMORY_DEVICE_HDR: file error!", __FILE__, __LINE__);
#Pregen: print file name after we open a file which will be read
PrintDependency($CUSTOM_MEMORY_DEVICE_HDR);

# CustCS_CustemChips:
#  the number of part number which customer assigned
#  in mediatek/custom/$project/preloader/inc/custom_MemoryDevice.h
# TotalCustemChips:
#   because one part number may match multiple emmc/nand ID, the TotalCustemChips >= CustCS_CustemChips
#   the final number of part number which will use to create emi_setting
#   in mediatek/custom/$project/preloader/inc/custom_emi.h
#
my $CustCS_CustemChips = 0 ;
my $TotalCustemChips = 0 ;

#
#   arrays
#
#   this should be an array of structurs, but it is said perl does not support it.
#   these are input, except EMI_GEND
#

# CustCS_PART_NUMBER:
#  the content of part number which customer assigned
#  in mediatek/custom/$project/preloader/inc/custom_MemoryDevice.h
# Total_PART_NUMBER:
#   the final part number which will show
#   in mediatek/custom/$project/preloader/inc/custom_emi.h
my $CustCS_PART_NUMBER ;
my $Total_PART_NUMBER ;


######################################################################################
my $DEV_TYPE;
my $DEV_TYPE1;
my $DEV_TYPE2;
my $DEV_MODE;
my $NAND_EMMC_ID;
my $FW_ID ;
my $ID_String ;
my $FW_ID_String ;
my $Sub_version;
my $USE_EMMC_ID_LEN=9;
my $MAX_NAND_EMMC_ID_LEN=16;
my $MAX_FW_ID_LEN=8;
my $fw_id_len;
my $NAND_PAGE_SIZE;
my $MODE_EXTERN_VAL;
my $DIS_CBT_MODE_EXTERN_TYPE;
my $EMI_CONA_VAL;
my $CHN0_EMI_CONA_VAL;
my $CHN1_EMI_CONA_VAL;
my $EMI_CONF_VAL;
my $EMI_CONH_VAL;
#my $AC_TIME_EMI_DUMMY_01;
my $AC_TIME_EMI_FREQUENCY;
my $AC_TIME_EMI_TRAS;
my $AC_TIME_EMI_TRP;
my $AC_TIME_EMI_TRPAB;
my $AC_TIME_EMI_TRC;
my $AC_TIME_EMI_TRFC;
my $AC_TIME_EMI_TRFCPB;
my $AC_TIME_EMI_TXP;
my $AC_TIME_EMI_TRTP;
my $AC_TIME_EMI_TRCD;
my $AC_TIME_EMI_TWR;
my $AC_TIME_EMI_TWTR;
my $AC_TIME_EMI_TRRD;
my $AC_TIME_EMI_TFAW;
my $AC_TIME_EMI_TRTW_ODT_OFF;
my $AC_TIME_EMI_TRTW_ODT_ON;
my $AC_TIME_EMI_REFCNT;
my $AC_TIME_EMI_REFCNT_FR_CLK;
my $AC_TIME_EMI_TXREFCNT;
my $AC_TIME_EMI_TZQCS;
my $AC_TIME_EMI_TRTPD;
my $AC_TIME_EMI_TWTPD;
my $AC_TIME_EMI_TMRR2W_ODT_OFF;
my $AC_TIME_EMI_TMRR2W_ODT_ON;
my $AC_TIME_EMI_TRAS_05T;
my $AC_TIME_EMI_TRP_05T;
my $AC_TIME_EMI_TRPAB_05T;
my $AC_TIME_EMI_TRC_05T;
my $AC_TIME_EMI_TRFC_05T;
my $AC_TIME_EMI_TRFCPB_05T;
my $AC_TIME_EMI_TXP_05T;
my $AC_TIME_EMI_TRTP_05T;
my $AC_TIME_EMI_TRCD_05T;
my $AC_TIME_EMI_TWR_05T;
my $AC_TIME_EMI_TWTR_05T;
my $AC_TIME_EMI_TRRD_05T;
my $AC_TIME_EMI_TFAW_05T;
my $AC_TIME_EMI_TRTW_ODT_OFF_05T;
my $AC_TIME_EMI_TRTW_ODT_ON_05T;
my $AC_TIME_EMI_REFCNT_05T;
my $AC_TIME_EMI_REFCNT_FR_CLK_05T;
my $AC_TIME_EMI_TXREFCNT_05T;
my $AC_TIME_EMI_TZQCS_05T;
my $AC_TIME_EMI_TRTPD_05T;
my $AC_TIME_EMI_TWTPD_05T;
my $AC_TIME_EMI_TMRR2W_ODT_OFF_05T;
my $AC_TIME_EMI_TMRR2W_ODT_ON_05T;
#my $AC_TIME_EMI_XRTW2W;
#my $AC_TIME_EMI_XRTW2R;
#my $AC_TIME_EMI_XRTR2W;
#my $AC_TIME_EMI_XRTR2R;
#my $AC_TIME_EMI_DUMMY_02;
#my $AC_TIME_EMI_WRITE_LATENCY_0x41c;
#my $AC_TIME_EMI_WRITE_LATENCY_0x420;
#my $AC_TIME_EMI_WRITE_LATENCY_0x424;
#my $AC_TIME_EMI_WRITE_LATENCY_0x428;
#my $AC_TIME_EMI_WRITE_LATENCY_0x42c;
#my $AC_TIME_EMI_DQSINCTL_FOR_GATING;
#my $AC_TIME_EMI_DATLAT;
#my $AC_TIME_EMI_MODE_REG_WL;
#my $AC_TIME_EMI_MODE_REG_RL;
my $AC_TIME_VALUE;
my $DRAM_RANK0_SIZE;
my $DRAM_RANK1_SIZE;
my $Discrete_DDR = 0;
my $MCP_LPDDR2 = 0;
my $MCP_LPDDR3 = 0;
my $MCP_LPDDR4 = 0;
my $MCP_LPDDR4X = 0;
my $MCP_LPDDR4P = 0;
my $MCP_PCDDR3 = 0;
my $DIS_LPDDR2 = 0;
my $DIS_LPDDR3 = 0;
my $DIS_LPDDR4 = 0;
my $DIS_LPDDR4X = 0;
my $DIS_LPDDR4P = 0;
my $DIS_PCDDR3 = 0;

my $DDR1_2_3 ;

#discrete dram
my $DIS_TYPE;
my $DIS_CONA;
my $DIS_CONH;
my $DIS_AC_U00;
my $DIS_AC_U01;
my $DIS_AC_U02;
my $DIS_AC_U03;
my $DIS_AC_U04;
my $DIS_AC_U05;
my $DIS_AC_U06;
my $DIS_AC_U07;
my $DIS_CONF;
my $DIS_CH0_CONA;
my $DIS_CH1_CONA;
my $DIS_CBT_MODE_EXTERN;

#union
#1
#my $LPDDR2_MODE_REG1;
#my $LPDDR2_MODE_REG2;
#my $LPDDR2_MODE_REG3;
#my $LPDDR2_MODE_REG5;
#my $LPDDR2_MODE_REG10;
#my $LPDDR2_MODE_REG63;

#2
#my $DDR1_MODE_REG;
#my $DDR1_EXT_MODE_REG;

#3
#my $PCDDR3_MODE_REG0;
#my $PCDDR3_MODE_REG1;
#my $PCDDR3_MODE_REG2;
#my $PCDDR3_MODE_REG3;
#my $PCDDR3_MODE_REG4;
#my $PCDDR3_MODE_REG5;

#4
#my $LPDDR3_MODE_REG1;
#my $LPDDR3_MODE_REG2;
#my $LPDDR3_MODE_REG3;
my $LPDDR3_MODE_REG5;
my $LPDDR4_MODE_REG5;
my $LPDDR4X_MODE_REG5;
my $LPDDR4P_MODE_REG5;
#my $LPDDR3_MODE_REG10;
#my $LPDDR3_MODE_REG63;

my $HAVE_HW_TRAPPING = 1;
my $PIN_MUX_TYPE;

my $EMI_SETTINGS ;
#
# all above are arrays, each represents an user defined chip.
#
# this is the ID of the custom board.
my $CustBoard_ID ;

while (<CUSTOM_MEMORY_DEVICE_HDR>)
{
    # matching the following lines
    # "#define MEMORY_DEVICE_TYPE          LPSDRAM"
    # "#define CS0_PART_NUMBER             EDK1432CABH60"

    # error-checking
    if (/^#if|^#ifdef|^#ifndef|^#elif|^#else/)
    {
      &error_handler("$CUSTOM_MEMORY_DEVICE_HDR: Not allowed to set conditional keywords $_ in custom_MemoryDevice.h!", __FILE__, __LINE__)
          unless (/^#ifndef\s+__CUSTOM_MEMORYDEVICE__/);
    }
    if (/^#define\s+(\w+)\[(\d+)\]\s+\(([-\w]*)\)/ || /^#define\s+(\w+)\[(\d+)\]\s+([-\w]*)/ ||
        /^#define\s+(MEMORY_DEVICE_TYPE)\s+\((\w*)\)/ || /^#define\s+(MEMORY_DEVICE_TYPE)\s+(\w*)/ ||
        /^#define\s+(BOARD_ID)\s+\((\w*)\)/ || /^#define\s+(BOARD_ID)\s+(\w*)/)
    {

        if ($1 eq "BOARD_ID")
        {
            $CustBoard_ID = $2 ;
        }
        elsif ($1 eq "CS_PART_NUMBER")
        {
            print "\nCS0 $2, $3\n" ;
            $CustCS_PART_NUMBER[$2] = $3 ;
            $CustCS_CustemChips = $CustCS_CustemChips + 1 ;

            print "$CustCS_PART_NUMBER[$2]\n"
        }
    }

}
print "\n$CustCS_CustemChips\n" if ($DebugPrint ==1);
close CUSTOM_MEMORY_DEVICE_HDR;
#
#
# we now read in all the needed infomation form custom defination file,
# so close it.
#
#


#
#   check if data validate.
#
if ($CustCS_CustemChips > 10)
{
    die "\n[Error]CustCS_CustemChips($CustCS_CustemChips) > 10\n" ;
}
if ($CustCS_CustemChips == 0)
{
    die "\n[Error]CustCS_CustemChips($CustCS_CustemChips) = 0?\n" ;
}


#****************************************************************************
# read a excel file to get emi settings
#****************************************************************************
# get already active Excel application or open new
if ($os eq "windows")
{
    $Excel = Win32::OLE->GetActiveObject('Excel.Application')
        || Win32::OLE->new('Excel.Application', 'Quit');

    # copy the Excel file to a temp file and open it;
    # this will prevent error due to simultaneous Excel access
    $Book    = $Excel->Workbooks->Open($MEMORY_DEVICE_LIST_XLS);
}
else
{
    my $parser = Spreadsheet::ParseExcel->new();
    $Book = $parser->Parse($MEMORY_DEVICE_LIST_XLS);
}
#Pregen: print file name after we open a file which will be read
PrintDependency($MEMORY_DEVICE_LIST_XLS);
# select worksheet
my $Sheet;
my $eos_flag       = 5; # if this flag counts to '0', it means End Of Sheet
my $iter = 0 ;
my $CustCS_part_number_iter = 0;
my $total_part_number_iter = 0;

    while ($iter<$CustCS_CustemChips)
    {
        $eos_flag = 5;
        $total_part_number_iter = &DeviceListParser_LPSDRAM($iter,$total_part_number_iter);
        $iter = $iter + 1 ;
    }

    # if the value read from excel validate
    $iter = 0 ;
    my $Page_size = "0" ;
    print "TotalCustemChips:$TotalCustemChips\n";
    while ($iter < $TotalCustemChips)
    {

        # only one Discrete is allowed
        if ($DEV_TYPE1[$iter] eq "00" || $DEV_TYPE1[$iter] eq "0F")
        {
            # if all discrete dram are LPDDR2, LPDDR3, LPDDR4, LPDDR4X and LPDDR4P, it is allowed
            if ($DEV_TYPE2[$iter] eq "02" || $DEV_TYPE2[$iter] eq "03" || $DEV_TYPE2[$iter] eq "05" || $DEV_TYPE2[$iter] eq "06" || $DEV_TYPE2[$iter] eq "07")
            {
                my $iter_dis_dram;
                $iter_dis_dram = 0;
                while ($iter_dis_dram < $TotalCustemChips)
                {
		              if ($iter_dis_dram == $iter || !($DEV_TYPE1[$iter_dis_dram] eq "00" || $DEV_TYPE1[$iter_dis_dram] eq "0F"))
		              {
                    	$iter_dis_dram++;
		    	            next;
		              }
                  {
#comment to avoid build error for the same vendor ID
#=head
			                if ($DEV_TYPE2[$iter] eq "02")
			                {
				                # check the MODE_REG5(DRAM vendor_ID) are unique
				                # if have same MODE5 in the list, send the build error
				                if ( $LPDDR2_MODE_REG5[$iter] eq $LPDDR2_MODE_REG5[$iter_dis_dram])
				                {
                          print "[Error] MODE_REG5(DRAM vendor_ID) should not be the same in the Combo list, MODE_REG5($Total_PART_NUMBER[$iter])==MODE_REG6($Total_PART_NUMBER[$iter_dis_dram])\n" ;

					                die "[Error] MODE_REG5(DRAM vendor_ID) should not be the same in the Combo list, MODE_REG5($Total_PART_NUMBER[$iter])==MODE_REG5($Total_PART_NUMBER[$iter_dis_dram])\n" ;
				                }
			                }
			                elsif ($DEV_TYPE2[$iter] eq "03")
			                {
				                # check the MODE_REG5(DRAM vendor_ID) are unique
				                # if have same MODE5 in the list, send the build error
				            if ( $LPDDR3_MODE_REG5[$iter] eq $LPDDR3_MODE_REG5[$iter_dis_dram])
                                            {
				                if ((($DRAM_RANK0_SIZE[$iter] eq $DRAM_RANK0_SIZE[$iter_dis_dram]) && ($DRAM_RANK1_SIZE[$iter] eq $DRAM_RANK1_SIZE[$iter_dis_dram])))
				                #if ( $LPDDR3_MODE_REG5[$iter] eq $LPDDR3_MODE_REG5[$iter_dis_dram])
				                {
					                die "[Error] LPDDR3 DRAM_RANKx_SIZE should not be the same in the Combo list, DRAM_RANKx_SIZE($Total_PART_NUMBER[$iter])==DRAM_RANKx_SIZE($Total_PART_NUMBER[$iter_dis_dram])\n" ;
				                }
                                            }
			                }
			                elsif (($DEV_TYPE2[$iter] eq "05"))
			                {
				                # check the MODE_REG5(DRAM vendor_ID) are unique
				                # if have same MODE5 in the list, send the build error
				                #if ( $LPDDR4_MODE_REG5[$iter] eq $LPDDR4_MODE_REG5[$iter_dis_dram])
				                if ( $LPDDR4_MODE_REG5[$iter] eq $LPDDR4_MODE_REG5[$iter_dis_dram])
			                        {
				                    if ((($DRAM_RANK0_SIZE[$iter] eq $DRAM_RANK0_SIZE[$iter_dis_dram]) && ($DRAM_RANK1_SIZE[$iter] eq $DRAM_RANK1_SIZE[$iter_dis_dram])))
				                    {
					                    #die "[Error] MODE_REG5(DRAM vendor_ID) should not be the same in the Combo list, MODE_REG5($Total_PART_NUMBER[$iter])==MODE_REG5($Total_PART_NUMBER[$iter_dis_dram])\n" ;
					                    die "[Error] LPDDR4 DRAM_RANKx_SIZE should not be the same in the Combo list, DRAM_RANKx_SIZE($Total_PART_NUMBER[$iter])==DRAM_RANKx_SIZE($Total_PART_NUMBER[$iter_dis_dram])\n" ;
				                    }
			                        }
			                }
			                elsif ($DEV_TYPE2[$iter] eq "06")
			                {
				                # check the MODE_REG5(DRAM vendor_ID) are unique
				                # if have same MODE5 in the list, send the build error
				                if ( $LPDDR4X_MODE_REG5[$iter] eq $LPDDR4X_MODE_REG5[$iter_dis_dram])
				                {
				                    if ((($DRAM_RANK0_SIZE[$iter] eq $DRAM_RANK0_SIZE[$iter_dis_dram]) && ($DRAM_RANK1_SIZE[$iter] eq $DRAM_RANK1_SIZE[$iter_dis_dram])))
				                    {
					                    #die "[Error] MODE_REG5(DRAM vendor_ID) should not be the same in the Combo list, MODE_REG5($Total_PART_NUMBER[$iter])==MODE_REG5($Total_PART_NUMBER[$iter_dis_dram])\n" ;
					                    die "[Error] LPDDR4X DRAM_RANKx_SIZE should not be the same in the Combo list, DRAM_RANKx_SIZE($Total_PART_NUMBER[$iter])==DRAM_RANKx_SIZE($Total_PART_NUMBER[$iter_dis_dram])\n" ;
				                    }
				                }
			                }
			                elsif ($DEV_TYPE2[$iter] eq "07")
			                {
				                # check the MODE_REG5(DRAM vendor_ID) are unique
				                # if have same MODE5 in the list, send the build error
				                if ( $LPDDR4P_MODE_REG5[$iter] eq $LPDDR4P_MODE_REG5[$iter_dis_dram])
				                {
				                    if ((($DRAM_RANK0_SIZE[$iter] eq $DRAM_RANK0_SIZE[$iter_dis_dram]) && ($DRAM_RANK1_SIZE[$iter] eq $DRAM_RANK1_SIZE[$iter_dis_dram])))
				                    {
					                    #die "[Error] MODE_REG5(DRAM vendor_ID) should not be the same in the Combo list, MODE_REG5($Total_PART_NUMBER[$iter])==MODE_REG5($Total_PART_NUMBER[$iter_dis_dram])\n" ;
					                    die "[Error] LPDDR4P DRAM_RANKx_SIZE should not be the same in the Combo list, DRAM_RANKx_SIZE($Total_PART_NUMBER[$iter])==DRAM_RANKx_SIZE($Total_PART_NUMBER[$iter_dis_dram])\n" ;
				                    }
				                }
			                }
#=cut
                  }

                    $iter_dis_dram++;
                }
                $Discrete_DDR = $Discrete_DDR + 1 ;
            }
            else{
	    	# DDR1,DDR3 only support one discrete DRAM in the Combo List
		# if more then one discrete DRAM in the list, send build error
                if ($Discrete_DDR == 0)
                {
                    $Discrete_DDR = $Discrete_DDR + 1 ;
                    if ($TotalCustemChips > 1)
                    {
                        print "[Error]At most one discrete PCDDR3 DRAM is allowed in the Combo MCP list\n" ;
                        die "[Error]Combo discrete DRAM only support LPDDR2, LPDDR3, LPDDR4, LPDDR4X and LPDDR4P\n" ;
                    }

                }
                else
                {
                    die "[Error]more than 1 Discrete DDR used!\n" ;
                }
            }
        }
	else{

	}

        # NAND can't use different page size.
        if ($DEV_TYPE1[$iter] == "01")
        {
            if ($Page_size == "0")
            {
                $Page_size = $NAND_PAGE_SIZE[$iter] ;
            }
            else
            {
                if ($Page_size != $NAND_PAGE_SIZE[$iter])
                {
                    die "[Error]different MCP page size!$Page_size, $NAND_PAGE_SIZE[$iter]\n" ;
                }
            }
        }

        # Nand or eMMC's + FW ID must unique
        my $iter2 ;
        $iter2 = $iter + 1 ;
        while ($iter2 < $TotalCustemChips)
        {
            if ($DEV_TYPE1[$iter] != "00")
            {
                if ($NAND_EMMC_ID[$iter] eq $NAND_EMMC_ID[$iter2])
                {
                    if ( $Total_PART_NUMBER[$iter] ne $Total_PART_NUMBER[$iter2] )
                    {
                        die "[Error]Different part number:$Total_PART_NUMBER[$iter],$Total_PART_NUMBER[$iter2] with same NAND/eMMC ID:$NAND_EMMC_ID[$iter]"
                    }
                    else
                    { #$Total_PART_NUMBER[$iter] eq $Total_PART_NUMBER[$iter2] and $NAND_EMMC_ID[$iter] eq $NAND_EMMC_ID[$iter2] and $FW_ID[$iter] eq $FW_ID[$iter2]
                        if ($FW_ID[$iter] eq $FW_ID[$iter2])
                        {
                            die "[Error]Two same part number:$Total_PART_NUMBER[$iter],$Total_PART_NUMBER[$iter2] with same NAND/eMMC ID:$NAND_EMMC_ID[$iter] and same FW ID:$FW_ID[$iter]"
                        }
                    }
                }
            }
            $iter2 = $iter2 + 1 ;
        }

        $iter = $iter + 1 ;
    }

    if ($HAVE_HW_TRAPPING == 0 && $DIS_LPDDR2 > 0 && (($DIS_LPDDR3 > 0) || ($MCP_LPDDR3 > 0) || ($DIS_PCDDR3 > 0)))
    {
	    die "[Error] LPDDR2 and LPDDR3 are not allowed to be mixed in the Combo Discrete DRAM list.\n" ;
    }
    if ($HAVE_HW_TRAPPING == 0 && $DIS_LPDDR3 > 0 && (($DIS_LPDDR2 > 0) || ($MCP_LPDDR2 > 0) || ($DIS_PCDDR3 > 0) || ($MCP_LPDDR4X > 0) || ($DIS_LPDDR4X > 0) || ($MCP_LPDDR4 > 0) || ($DIS_LPDDR4 > 0)))
    {
           die "[Error] LPDDR2 and LPDDR3 are not allowed to be mixed in the Combo Discrete DRAM list.\n" ;
    }
    if ($HAVE_HW_TRAPPING == 0 && $DIS_LPDDR4 > 0 && (($DIS_LPDDR2 > 0) || ($DIS_LPDDR3 > 0) || ($MCP_LPDDR2 > 0) || ($MCP_LPDDR3 > 0) || ($DIS_PCDDR3 > 0)))
    {
           die "[Error] LPDDR2, LPDDR3 and LPDDR4/4X are not allowed to be mixed in the Combo Discrete DRAM list.\n" ;
    }
    if ($HAVE_HW_TRAPPING == 0 && $DIS_LPDDR4X > 0 && (($DIS_LPDDR2 > 0) || ($DIS_LPDDR3 > 0) || ($MCP_LPDDR2 > 0) || ($MCP_LPDDR3 > 0) || ($DIS_PCDDR3 > 0)))
    {
           die "[Error] LPDDR2, LPDDR3 and LPDDR4/4X are not allowed to be mixed in the Combo Discrete DRAM list.\n" ;
    }
    if ($HAVE_HW_TRAPPING == 0 && $DIS_LPDDR4P > 0 && (($DIS_LPDDR2 > 0) || ($DIS_LPDDR3 > 0) || ($MCP_LPDDR2 > 0) || ($MCP_LPDDR3 > 0) || ($DIS_PCDDR3 > 0)))
    {
           die "[Error] LPDDR2, LPDDR3 and LPDDR4/4X/4P are not allowed to be mixed in the Combo Discrete DRAM list.\n" ;
    }
    if ($DIS_PCDDR3 > 1)
    {
	    die "[Error] PCDDR3 not support Combo Discrete DRAM feature.\n" ;
    }
    if (($DIS_PCDDR3 == 1) && ($CustCS_CustemChips>1) )
    {
	    die "[Error] PCDDR3 not support Combo Discrete DRAM feature.\n" ;
    }


# close the temp Excel file
if ($os eq "windows")
{
    $Book->Close(1);
}


#&error_handler("$CUSTOM_MEMORY_DEVICE_HDR: Incorrect memory device type!", __FILE__, __LINE__) unless $MEMORY_DEVICE_TYPE;
#&error_handler("$CUSTOM_MEMORY_DEVICE_HDR: part number not supported!", __FILE__, __LINE__)    if ($is_part_found <= 0);

#&error_handler("$CUSTOM_MEMORY_DEVICE_HDR: Device not allowed to set NOR_RAM_MCP type!", __FILE__, __LINE__)     if (($MEMORY_DEVICE_TYPE eq 'NOR_RAM_MCP') && ($part_info->{CS}->{1}->{DRAM} eq 'YES'));
#&error_handler("$CUSTOM_MEMORY_DEVICE_HDR: Device not allowed to set NOR_LPSDRAM_MCP type!", __FILE__, __LINE__) if (($MEMORY_DEVICE_TYPE eq 'NOR_LPSDRAM_MCP') && ($part_info->{CS}->{1}->{DRAM} eq 'NO'));



#****************************************************************************
# generate custom_EMI.c
#****************************************************************************
#if ($is_existed_c == 0)
#{
#    if ($is_existed_c == 1)
#    {
#	unlink ($CUSTOM_EMI_C);
#    }

#    my $temp_path = `dirname $CUSTOM_EMI_C`;
#    `mkdir -p $temp_path`;

#    open (CUSTOM_EMI_C, ">$CUSTOM_EMI_C") or &error_handler("$CUSTOM_EMI_C: file error!", __FILE__, __LINE__);

#    print CUSTOM_EMI_C &copyright_file_header();
#    print CUSTOM_EMI_C &description_file_header(                      "custom_emi.c",
#          "This Module defines the EMI (external memory interface) related setting.",
#                                                 "EMI auto generator". $EMIGEN_VERNO);
#    print CUSTOM_EMI_C &custom_EMI_c_file_body();
#    close CUSTOM_EMI_C or &error_handler("$CUSTOM_EMI_C: file error!", __FILE__, __LINE__);

#    print "\n$CUSTOM_EMI_C is generated\n";
#} # if ($is_existed_c == 0)

#****************************************************************************
# generate custom_emi.h
#****************************************************************************
if ($build_opt == 0)
{
    if ($is_existed_h == 1)
    {
        unlink ($CUSTOM_EMI_H);
    }

    my $temp_path = `dirname $CUSTOM_EMI_H`;
    `mkdir -p $temp_path`;

    open (CUSTOM_EMI_H, ">$CUSTOM_EMI_H") or &error_handler("CUSTOM_EMI_H: file error!", __FILE__, __LINE__);

    print CUSTOM_EMI_H &copyright_file_header();
    print CUSTOM_EMI_H &description_file_header(                      "custom_emi.h",
          "This Module defines the EMI (external memory interface) related setting.",
                                                 "EMI auto generator". $EMIGEN_VERNO);
    print CUSTOM_EMI_H &custom_EMI_h_file_body();
    close CUSTOM_EMI_H or &error_handler("$CUSTOM_EMI_H: file error!", __FILE__, __LINE__);

    print "\n$CUSTOM_EMI_H is generated\n";
} # if ($build_opt == 0)

if ($build_opt == 1) # Build Preloader TAG!
{
    &write_tag($PROJECT);
}

exit;
#****************************************************************************
# subroutine:  trim
# input:       $string:  trim string
#****************************************************************************

sub trim($)
{
    my $string = shift;
    $string =~ s/^\s+//;
    $string =~ s/\s+$//;
    return $string;
}
#****************************************************************************
# subroutine:  error_handler
# input:       $error_msg:     error message
#****************************************************************************
sub error_handler
{
	   my ($error_msg, $file, $line_no) = @_;

	   my $final_error_msg = "[Error]EMIGEN ERROR: $error_msg at $file line $line_no\n";
	   print $final_error_msg;
	   die $final_error_msg;
}

#****************************************************************************
# subroutine:  copyright_file_header
# return:      file header -- copyright
#****************************************************************************
sub copyright_file_header
{
    my $template = <<"__TEMPLATE";
/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
__TEMPLATE

   return $template;
}

#****************************************************************************
# subroutine:  description_file_header
# return:      file header -- description
# input:       $filename:     filename
# input:       $description:  one line description
# input:       $author:       optional
#****************************************************************************
sub description_file_header
{
    my ($filename, $description, $author) = @_;
    my @stat_ar = stat $MEMORY_DEVICE_LIST_XLS;
    my ($day, $month, $year) = (localtime($stat_ar[9]))[3,4,5]; $month++; $year+=1900;
    my $template = <<"__TEMPLATE";
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $filename
 *
 * Project:
 * --------
 *   Android
 *
 * Description:
 * ------------
 *   $description
 *
 * Author:
 * -------
 *  $author
 *
 *   Memory Device database last modified on $year/$month/$day
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * \$Revision\$
 * \$Modtime\$
 * \$Log\$
 *
 *------------------------------------------------------------------------------
 * WARNING!!!  WARNING!!!   WARNING!!!  WARNING!!!  WARNING!!!  WARNING!!!
 * This file is generated by EMI Auto-gen Tool.
 * Please do not modify the content directly!
 * It could be overwritten!
 *============================================================================
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

__TEMPLATE

   return $template;
}
#****************************************************************************
# subroutine:  HeaderBody_for_lpsdram
# return:      content for custom_EMI.h
#****************************************************************************
#sub custom_EMI_h_file_body
#{
    ###
#    my $template = <<"__TEMPLATE";

#ifndef __CUSTOM_EMI__
#define __CUSTOM_EMI__

#define COMBO_LPDDR2 ($MCP_LPDDR2+$DIS_LPDDR2)
#define COMBO_LPDDR3 ($MCP_LPDDR3+$DIS_LPDDR3)
#define COMBO_LPDDR4 ($MCP_LPDDR4+$DIS_LPDDR4)
#define COMBO_LPDR4X ($MCP_LPDDR4X+$DIS_LPDDR4X)
#define COMBO_LPDR4P ($MCP_LPDDR4P+$DIS_LPDDR4P)
#define COMBO_PCDDR3 ($MCP_PCDDR3+$DIS_PCDDR3)

#endif /* __CUSTOM_EMI__ */


#__TEMPLATE

#    return $template;
#}

#****************************************************************************
# subroutine:  custom_EMI_h_file_body
# return:
#****************************************************************************
sub custom_EMI_h_file_body
{
    ###
    my $EMI_SETTINGS_string = "" ;
    my $ddr = -1 ;

    $iter = 0 ;

    for $iter (0..$TotalCustemChips-1)
    {
        if ($DEV_TYPE1[$iter] != "00")
	{
            $EMI_SETTINGS_string = $EMI_SETTINGS_string . $EMI_SETTINGS[$iter] ;
            $EMI_SETTINGS_string = $EMI_SETTINGS_string . " ," ;
	}
    }
    for $iter (0..$TotalCustemChips-1)
    {
        if ($DEV_TYPE1[$iter] == "00")
	{
            $EMI_SETTINGS_string = $EMI_SETTINGS_string . $EMI_SETTINGS[$iter] ;
            $EMI_SETTINGS_string = $EMI_SETTINGS_string . " ," ;
	}
    }
=head
previous version
    while ($iter<$TotalCustemChips)
    {
        if ($DEV_TYPE1[$iter] == "00")
        {
            $ddr = $iter ;
            print "Discrete ddr found $ddr \n" ;
            $iter = $iter + 1 ;
        }
        else
        {
            $EMI_SETTINGS_string = $EMI_SETTINGS_string . $EMI_SETTINGS[$iter] ;

            $iter = $iter + 1 ;
            if ($iter < $TotalCustemChips || $ddr != -1)
            {
                $EMI_SETTINGS_string = $EMI_SETTINGS_string . " ," ;
            }
        }
    }
# if we have discrete dram, we put them in the end
    if ($ddr != -1)
    {
        $EMI_SETTINGS_string = $EMI_SETTINGS_string . $EMI_SETTINGS[$ddr] ;
    }
=cut


	my $template = <<"__TEMPLATE";
#ifndef __CUSTOM_EMI__
#define __CUSTOM_EMI__

#include "dramc_pi_api.h"
#include "emi.h"

#define COMBO_LPDDR2 ($MCP_LPDDR2+$DIS_LPDDR2)
#define COMBO_LPDDR3 ($MCP_LPDDR3+$DIS_LPDDR3)
#define COMBO_LPDDR4 ($MCP_LPDDR4+$DIS_LPDDR4)
#define COMBO_LPDR4X ($MCP_LPDDR4X+$DIS_LPDDR4X)
#define COMBO_LPDR4P ($MCP_LPDDR4P+$DIS_LPDDR4P)
#define COMBO_PCDDR3 ($MCP_PCDDR3+$DIS_PCDDR3)
#define NUM_EMI_RECORD ($TotalCustemChips)

int num_of_emi_records = NUM_EMI_RECORD ;

EMI_SETTINGS default_emi_setting =
{
	0x0,            /* sub_version */
	$DIS_TYPE,         /* TYPE */
	0,              /* EMMC ID/FW ID checking length */
	0,              /* FW length */
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},              /* NAND_EMMC_ID */
	{0x00,0x0,0x0,0x0,0x0,0x0,0x0,0x0},             /* FW_ID */
	$DIS_CONA,		/* EMI_CONA_VAL */
	$DIS_CONH,		/* EMI_CONH_VAL */
	$DIS_AC_U00,		/* U 00 */
	$DIS_AC_U01,		/* U 01 */
	$DIS_AC_U02,		/* U 02 */
	$DIS_AC_U03,		/* U 03 */
	$DIS_AC_U04,		/* U 04 */
	$DIS_AC_U05,		/* U 05 */
	$DIS_AC_U06,		/* U 06 */
	$DIS_AC_U07,		/* U 07 */
	{0x20000000,0,0,0},		/* DRAM RANK SIZE */
	$DIS_CONF,		/* EMI_CONF_VAL */
	$DIS_CH0_CONA,		/* CHN0_EMI_CONA_VAL */
	$DIS_CH1_CONA,		/* CHN1_EMI_CONA_VAL */
	$DIS_CBT_MODE_EXTERN,	/* dram_cbt_mode_extern */
	{0,0,0,0,0,0},		/* reserved 6 */
	0x00000000,		/* LPDDR4_MODE_REG5 */
	0,		/* PIN_MUX_TYPE for tablet */
};

EMI_SETTINGS emi_settings[] =
{
     $EMI_SETTINGS_string
};

#endif /* __CUSTOM_EMI__ */
__TEMPLATE
    return $template ;
}


#****************************************************************************
# subroutine:  DeviceListParser_LPSDRAM
# input:       the number in array
# return:      string contain 1 set of EMI setting for input
#****************************************************************************
sub DeviceListParser_LPSDRAM
{
    my ($id) ;
    my ($CustCS_id) ;
    my ($PartNum) ;
    my ($iter);
    my ($num_part_found);


    $num_part_found = 0 ;
    $CustCS_id = $_[0]; # CustCS_CustemChips_iter
    $id = $_[1] ; # total_part_number_iter

    $PartNum = $CustCS_PART_NUMBER[$CustCS_id] ;
    print "\nCustCS ID num is $CustCS_id, part number:$CustCS_PART_NUMBER[$CustCS_id]\n" ;


    my $row        = $start_num + 0 ;                    # scan from row 2 when $MEMORY_DEVICE_TYPE eq 'LPSDRAM'
    my $col        = $COLUMN_PART_NUMBER ;               # scan column 2 for Part Number
    my $rows_part_found;                                 # stores the part numbers found in MDL


    $Sheet = get_sheet("mt6763") ;
    # find cell address of the specified Nand ID

    my $scan_idx = &xls_cell_value($Sheet, $row, $col) ;
    print "[Bike Check] scan_idx= $scan_idx eos_flag=$eos_flag\n";
    while (defined ($scan_idx) && ($eos_flag > 0))
    {
        ++$row ;
        $scan_idx = &xls_cell_value($Sheet, $row, $col) ;
        print "[Bike Check] scan_idx= $scan_idx\n";
        unless (defined $scan_idx)
        {
            print "[$row][scan_idx]No Value, $eos_flag\n" if $DebugPrint == 1 ;
            $eos_flag -- ;
            next ;
        }
        if ($scan_idx eq "")
        {
            print "[$row][scan_idx]EQ null, $eos_flag\n" if $DebugPrint == 1 ;
            $eos_flag -- ;
            next ;
        }
        $eos_flag   = 5 ;

        # remove leading and tailing spaces
        $scan_idx =~ s/^\s+// if $DebugPrint == 1 ;
        $scan_idx =~ s/\s+$// if $DebugPrint == 1 ;

		$scan_idx =~ s/^\s+// ;
		$scan_idx =~ s/\s+$// ;

		print "$scan_idx ?= $PartNum\n" ;

        if ($scan_idx eq $PartNum) # scan column 2 for Part Number

        {
            my $boardid ;
            $boardid = &xls_cell_value($Sheet, $row, $COLUMN_BOARD_ID) ;
            if ($CustBoard_ID eq $boardid)
            {
                $rows_part_found[$num_part_found] = $row;

                print "\nPartNum($PartNum==$scan_idx) found in row $row\n" ;
                $Total_PART_NUMBER[$TotalCustemChips] = $PartNum;
                $num_part_found += 1 ;
                $TotalCustemChips += 1;
            }
        }
    }
    print("[Bike Check]num_part_found $num_part_found");
    if ($num_part_found == 0)
    {
         print "\n[Error]unsupported part number $PartNum\n" ;
        die "\n[Error]unsupported part number $PartNum\n" ;
    }

    $iter = 0;
    while ($iter<$num_part_found)
    {

        $_ = $rows_part_found[$iter] ;
        $iter ++;

        $VENDOR[$id] = &xls_cell_value($Sheet, $_, $COLUMN_VENDOR) ;

        $DENSITY[$id] = &xls_cell_value($Sheet, $_, $COLUMN_DENSITY) ;


        # find the correct platform
        my $platform_scan_idx = $COLUMN_PLATFORM ; #First EMI controller

        my $tmp_platform = &xls_cell_value($Sheet, $start_num, $platform_scan_idx) ;
        while (!($tmp_platform =~ $PLATFORM))
        {
            $platform_scan_idx++;
            $tmp_platform = &xls_cell_value($Sheet, $start_num, $platform_scan_idx);
            if ($platform_scan_idx > 100)
            {
                die "[Error][Porting Error] It cannot find the right platform name.Please check platform name in XLS\n";
            }
        }
        &error_handler("$CUSTOM_MEMORY_DEVICE_HDR: $PLATFORM not support LPSDRAM!", __FILE__, __LINE__) if ($platform_scan_idx > $COLUMN_PLATFORM);

        $DEV_TYPE[$id]                       = &xls_cell_value($Sheet, $_, $COLUMN_TYPE) ;
        $DEV_MODE[$id]                       = &xls_cell_value($Sheet, $_, $COLUMN_MODE) ;
        $NAND_EMMC_ID[$id]                   = &xls_cell_value($Sheet, $_, $COLUMN_NAND_EMMC_ID) ;
        $FW_ID[$id]                          = &xls_cell_value($Sheet, $_, $COLUMN_FW_ID) ;
        $NAND_PAGE_SIZE[$id]                 = &xls_cell_value($Sheet, $_, $COLUMN_NAND_PAGE_SIZE) ;

        $EMI_CONA_VAL[$id]                   = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $CHN0_EMI_CONA_VAL[$id]              = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $CHN1_EMI_CONA_VAL[$id]              = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $EMI_CONF_VAL[$id]                   = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $EMI_CONH_VAL[$id]                   = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;

#        $AC_TIME_EMI_DUMMY_01[$id]           = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_FREQUENCY[$id]          = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRAS[$id]               = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRP[$id]                = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRPAB[$id]              = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRC[$id]                = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRFC[$id]               = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRFCPB[$id]             = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TXP[$id]                = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRTP[$id]               = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRCD[$id]               = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TWR[$id]                = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TWTR[$id]               = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRRD[$id]               = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TFAW[$id]               = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRTW_ODT_OFF[$id]       = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRTW_ODT_ON[$id]        = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_REFCNT[$id]             = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_REFCNT_FR_CLK[$id]      = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TXREFCNT[$id]           = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TZQCS[$id]              = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;

        $AC_TIME_EMI_TRTPD[$id]              = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TWTPD[$id]              = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TMRR2W_ODT_OFF[$id]     = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TMRR2W_ODT_ON[$id]      = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $CHIP_ID[$id]                        = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ; #Darren
        $AC_TIME_EMI_TRAS_05T[$id]           = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRP_05T[$id]            = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRPAB_05T[$id]          = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRC_05T[$id]            = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRFC_05T[$id]           = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRFCPB_05T[$id]         = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TXP_05T[$id]            = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRTP_05T[$id]           = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRCD_05T[$id]           = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TWR_05T[$id]            = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TWTR_05T[$id]           = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRRD_05T[$id]           = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TFAW_05T[$id]           = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRTW_ODT_OFF_05T[$id]   = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRTW_ODT_ON_05T[$id]    = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_REFCNT_05T[$id]         = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_REFCNT_FR_CLK_05T[$id]  = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_TXREFCNT_05T[$id]       = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_TZQCS_05T[$id]          = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TRTPD_05T[$id]          = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TWTPD_05T[$id]          = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TMRR2W_ODT_OFF_05T[$id] = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        $AC_TIME_EMI_TMRR2W_ODT_ON_05T[$id]  = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_XRTW2W[$id]             = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_XRTW2R[$id]             = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_XRTR2W[$id]             = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_XRTR2R[$id]             = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_DUMMY_02[$id]           = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_WRITE_LATENCY_0x41c[$id] = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_WRITE_LATENCY_0x420[$id] = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_WRITE_LATENCY_0x424[$id] = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_WRITE_LATENCY_0x428[$id] = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_WRITE_LATENCY_0x42c[$id] = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_DQSINCTL_FOR_GATING[$id] = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_DATLAT[$id]              = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_MODE_REG_WL[$id]        = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#        $AC_TIME_EMI_MODE_REG_RL[$id]        = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#
#        $DDR1_2_3[$id]                        = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
#
#        #  openoffice saved file workaround START
#
#        if ($DEV_TYPE[$id] =~ /DDR1/)
#        {
#            $DDR1_2_3[$id] = 'DDR1';
#        }
#        elsif ($DEV_TYPE[$id] =~ /LPDDR2/)
#        {
#            $DDR1_2_3[$id] = 'LPDDR2';
#        }
#        elsif ($DEV_TYPE[$id] =~ /PCDDR3/)
#        {
#            $DDR1_2_3[$id] = 'PCDDR3';
#        }
#        elsif ($DEV_TYPE[$id] =~ /LPDDR3/)
#        {
#            $DDR1_2_3[$id] = 'LPDDR3';
#        }
#        #  openoffice saved file workaround END

        if ($DEV_TYPE[$id] =~ /LPDDR4/)
        {
            $DDR1_2_3[$id] = 'LPDDR4';
            $DIS_TYPE = "0x0005";
            #$DIS_CONA = "0xf050f154";
            $DIS_CONA = "0x0000e050";
            $DIS_CONH = "0x00000000";
            $DIS_AC_U00 = "0x06090640";
            $DIS_AC_U01 = "0x113D1101";
            $DIS_AC_U02 = "0x0D060301";
            $DIS_AC_U03 = "0x99080307";
            $DIS_AC_U04 = "0x224E6561";
            $DIS_AC_U05 = "0x0B0B0C0C";
            $DIS_AC_U06 = "0x00054045";
            $DIS_AC_U07 = "0x00000000";
            $DIS_CONF = "0x00042100";
            #$DIS_CH0_CONA = "0x0400f050";
            #$DIS_CH1_CONA = "0x0400f050";
            $DIS_CH0_CONA = "0x0400e050";
            $DIS_CH1_CONA = "0x04000000";
            $DIS_CBT_MODE_EXTERN = "CBT_R0_R1_BYTE";
        }
        elsif ($DEV_TYPE[$id] =~ /LPDR4X/)
	{
            $DDR1_2_3[$id] = 'LPDR4X';
            $DIS_TYPE = "0x0006";
            #$DIS_CONA = "0xf050f154";
            $DIS_CONA = "0x0000e050";
            $DIS_CONH = "0x00000000";
            $DIS_AC_U00 = "0x06090640";
            $DIS_AC_U01 = "0x113D1101";
            $DIS_AC_U02 = "0x0D060301";
            $DIS_AC_U03 = "0x99080307";
            $DIS_AC_U04 = "0x224E6561";
            $DIS_AC_U05 = "0x0B0B0C0C";
            $DIS_AC_U06 = "0x00054045";
            $DIS_AC_U07 = "0x00000000";
            $DIS_CONF = "0x00042100";
            #$DIS_CH0_CONA = "0x0400f050";
            #$DIS_CH1_CONA = "0x0400f050";
            $DIS_CH0_CONA = "0x0400e050";
            $DIS_CH1_CONA = "0x04000000";
            $DIS_CBT_MODE_EXTERN = "CBT_R0_R1_BYTE";
	}
        elsif ($DEV_TYPE[$id] =~ /LPDR4P/)
	{
            $DDR1_2_3[$id] = 'LPDR4P';
            $DIS_TYPE = "0x0007";
            #$DIS_CONA = "0xf050f154";
            $DIS_CONA = "0x0000e050";
            $DIS_CONH = "0x00000000";
            $DIS_AC_U00 = "0x06090640";
            $DIS_AC_U01 = "0x113D1101";
            $DIS_AC_U02 = "0x0D060301";
            $DIS_AC_U03 = "0x99080307";
            $DIS_AC_U04 = "0x224E6561";
            $DIS_AC_U05 = "0x0B0B0C0C";
            $DIS_AC_U06 = "0x00054045";
            $DIS_AC_U07 = "0x00000000";
            $DIS_CONF = "0x00042100";
            #$DIS_CH0_CONA = "0x0400f050";
            #$DIS_CH1_CONA = "0x0400f050";
            $DIS_CH0_CONA = "0x0400e050";
            $DIS_CH1_CONA = "0x04000000";
            $DIS_CBT_MODE_EXTERN = "CBT_R0_R1_BYTE";
	}
	else
	{
            $DDR1_2_3[$id] = 'LPDDR3';
            $DIS_TYPE = "0x0003";
            #$DIS_CONA = "0x0000a0a2";
            $DIS_CONA = "0x0000a052";
            $DIS_CONH = "0x00000000";
            $DIS_AC_U00 = "0x070C03A5";
            $DIS_AC_U01 = "0x1F571501";
            $DIS_AC_U02 = "0x0D070301";
            $DIS_AC_U03 = "0x660F0407";
            $DIS_AC_U04 = "0x29656570";
            $DIS_AC_U05 = "0x00000B0A";
            $DIS_AC_U06 = "0x15055054";
            $DIS_AC_U07 = "0x00000000";
            $DIS_CONF = "0x04210000";
            #$DIS_CH0_CONA = "0x0000a0a2";
            $DIS_CH0_CONA = "0x0000a052";
            $DIS_CH1_CONA = "0x00000002";
            $DIS_CBT_MODE_EXTERN = "CBT_R0_R1_NORMAL";
	}


    if ($DEV_MODE[$id] eq "R0_NORMAL+R1_NORMAL")
	{
            $MODE_EXTERN_VAL[$id] = "CBT_R0_R1_NORMAL";
			$DIS_CBT_MODE_EXTERN_TYPE[$id] = "0x0";
	}
	elsif ($DEV_MODE[$id] eq "R0_BYTE+R1_BYTE")
	{
            $MODE_EXTERN_VAL[$id] = "CBT_R0_R1_BYTE";
			$DIS_CBT_MODE_EXTERN_TYPE[$id] = "0x1";
	}
	elsif ($DEV_MODE[$id] eq "R0_NORMAL+R1_BYTE")
	{
            $MODE_EXTERN_VAL[$id] = "CBT_R0_NORMAL_R1_BYTE";
			$DIS_CBT_MODE_EXTERN_TYPE[$id] = "0x2";
	}
	elsif ($DEV_MODE[$id] eq "R0_BYTE+R1_NORMAL")
	{
            $MODE_EXTERN_VAL[$id] = "CBT_R0_BYTE_R1_NORMAL";
			$DIS_CBT_MODE_EXTERN_TYPE[$id] = "0x3";
	}
	else
	{
            die "[Error]Wrong Mode !!!\n";
	}

#union
# LPDDR2
        if ($DDR1_2_3[$id] eq "LPDDR2")
        {
            die "[Error][EMIgen] no support LPDDR2.";
        }
# DDR1
        elsif ($DDR1_2_3[$id] eq "DDR1")
        {
	          die "[Error][EMIgen] no support DDR1.";
        }
# PCDDR3
        elsif ($DDR1_2_3[$id] eq "PCDDR3")
        {
            die "[Error][EMIgen] no support PCDDR3.";
        }
# LPDDR3
        elsif ($DDR1_2_3[$id] eq "LPDDR3")
        {
            $LPDDR3_MODE_REG5[$id]             = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        }
        elsif ($DDR1_2_3[$id] eq "LPDDR4")
        {
            $LPDDR4_MODE_REG5[$id]             = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        }
        elsif ($DDR1_2_3[$id] eq "LPDR4X")
        {
            $LPDDR4X_MODE_REG5[$id]             = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        }
        elsif ($DDR1_2_3[$id] eq "LPDR4P")
        {
            $LPDDR4P_MODE_REG5[$id]             = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;
        }

        $PIN_MUX_TYPE[$id] = &xls_cell_value($Sheet, $_, $platform_scan_idx++) ;

        if ($DENSITY[$id] eq "8192+8192")
        {
            $DRAM_RANK0_SIZE[$id] = "0x40000000";
            $DRAM_RANK1_SIZE[$id] = "0x40000000";
        }
        elsif ($DENSITY[$id] eq "16384+8192")
        {
            $DRAM_RANK0_SIZE[$id] = "0x80000000";
            $DRAM_RANK1_SIZE[$id] = "0x40000000";
        }
        elsif ($DENSITY[$id] eq "8192+4096")
        {
            $DRAM_RANK0_SIZE[$id] = "0x40000000";
            $DRAM_RANK1_SIZE[$id] = "0x20000000";
        }
        elsif ($DENSITY[$id] eq "4096+4096")
        {
            $DRAM_RANK0_SIZE[$id] = "0x20000000";
            $DRAM_RANK1_SIZE[$id] = "0x20000000";
        }
        elsif ($DENSITY[$id] eq "2048+2048")
        {
            $DRAM_RANK0_SIZE[$id] = "0x10000000";
            $DRAM_RANK1_SIZE[$id] = "0x10000000";
        }
        elsif ($DENSITY[$id] eq "4096+2048")
        {
            $DRAM_RANK0_SIZE[$id] = "0x20000000";
            $DRAM_RANK1_SIZE[$id] = "0x10000000";
        }
        elsif ($DENSITY[$id] eq "12288+12288")
        {
            $DRAM_RANK0_SIZE[$id] = "0x60000000";
            $DRAM_RANK1_SIZE[$id] = "0x60000000";
        }
        elsif ($DENSITY[$id] eq "6144+6144")
        {
            $DRAM_RANK0_SIZE[$id] = "0x30000000";
            $DRAM_RANK1_SIZE[$id] = "0x30000000";
        }
        elsif ($DENSITY[$id] eq "16384+16384")
        {
            $DRAM_RANK0_SIZE[$id] = "0x80000000";
            $DRAM_RANK1_SIZE[$id] = "0x80000000";
        }
        elsif ($DENSITY[$id] eq "24576+24576")
        {
            $DRAM_RANK0_SIZE[$id] = "0xC0000000";
            $DRAM_RANK1_SIZE[$id] = "0xC0000000";
        }
        elsif ($DENSITY[$id] eq "16384+32768")
        {
            $DRAM_RANK0_SIZE[$id] = "0x80000000";
            $DRAM_RANK1_SIZE[$id] = "0x100000000";
        }
        elsif ($DENSITY[$id] eq "32768")
        {
            $DRAM_RANK0_SIZE[$id] = "0x100000000";
            $DRAM_RANK1_SIZE[$id] = "0";
        }
        elsif ($DENSITY[$id] eq "24576")
        {
            $DRAM_RANK0_SIZE[$id] = "0xC0000000";
            $DRAM_RANK1_SIZE[$id] = "0";
        }
        elsif ($DENSITY[$id] eq "16384")
        {
            $DRAM_RANK0_SIZE[$id] = "0x80000000";
            $DRAM_RANK1_SIZE[$id] = "0";
        }
        elsif ($DENSITY[$id] eq "12288")
        {
            $DRAM_RANK0_SIZE[$id] = "0x60000000";
            $DRAM_RANK1_SIZE[$id] = "0";
        }
        elsif ($DENSITY[$id] eq "8192")
        {
            $DRAM_RANK0_SIZE[$id] = "0x40000000";
            $DRAM_RANK1_SIZE[$id] = "0";
        }
        elsif ($DENSITY[$id] eq "6144")
        {
            $DRAM_RANK0_SIZE[$id] = "0x30000000";
            $DRAM_RANK1_SIZE[$id] = "0";
        }
        elsif ($DENSITY[$id] eq "4096")
        {
            $DRAM_RANK0_SIZE[$id] = "0x20000000";
            $DRAM_RANK1_SIZE[$id] = "0";
        }
        elsif ($DENSITY[$id] eq "2048")
        {
            $DRAM_RANK0_SIZE[$id] = "0x10000000";
            $DRAM_RANK1_SIZE[$id] = "0";
        }
        else
        {
            die "[Error]Wrong Density size!!DENSITY:$DENSITY[$id], Please check Density in MDL.\n";
        }

#        if (!($CHIP_ID[$id] eq $MTK_MACH_TYPE))
        if (!($CHIP_ID[$id] eq "mt6763"))
        {
            die "[Error] Wrong Chip ID:$CHIP_ID[$id]"
        }

	#the rank size check algorithim should double check with EMI owner when porting.
	#rank size check START
        $hex_val = hex($EMI_CONA_VAL[$id]);
        $hex_val_ch0 = hex($CHN0_EMI_CONA_VAL[$id]);
        $hex_val_ch1 = hex($CHN1_EMI_CONA_VAL[$id]);
        if ($DENSITY[$id] =~ /\+/)
        {
            if (($DEV_TYPE[$id] =~ /LPDDR4/) || ($DEV_TYPE[$id] =~ /LPDR4X/) || ($DEV_TYPE[$id] =~ /LPDR4P/))
            {
                if (!($hex_val & 0x20000)) # dual rank
                {
                    die "[Error] LPDDR4 Wrong CONA value with dual rank:$DENSITY[$id],$EMI_CONA_VAL[$id]"
                }
                else
                {
                    if ($hex_val & 0x10000) #dual ch
                    {
                        if (!(($hex_val_ch0 & 0x1) && ($hex_val_ch1 & 0x1))) #dual rank
                        {
                            die "[Error] LPDDR4 Wrong Dual CHN CONA value with dual rank:$CHN0_EMI_CONA_VAL[$id],$CHN1_EMI_CONA_VAL[$id]"
                        }
                    }
                    else #single ch
                    {
                        if (!($hex_val_ch0 & 0x1))
                        {
                            die "[Error] LPDDR4 Wrong Single CHN CONA value with dual rank:$CHN0_EMI_CONA_VAL[$id]"
                        }
                    }
                }
            }
            else # LPDDR3
            {
                if (!($hex_val & 0x20000))
                {
                    die "[Error] LPDDR3 Wrong CONA value with dual rank:$DENSITY[$id],$EMI_CONA_VAL[$id]"
                }
            }
        }
        else # single rank
        {
            if (($DEV_TYPE[$id] =~ /LPDDR4/) || ($DEV_TYPE[$id] =~ /LPDR4X/) || ($DEV_TYPE[$id] =~ /LPDR4P/))
            {
                if ($hex_val & 0x20000)
                {
                    die "[Error] LPDDR4 Wrong CONA value with single rank:$DENSITY[$id],$EMI_CONA_VAL[$id]";
                }
                else
                {
                    if ($hex_val & 0x10000) #dual ch
                    {
                        if (($hex_val_ch0 & 0x1) || ($hex_val_ch1 & 0x1)) #dual ch
                        {
                            die "[Error] LPDDR4 Wrong Dual CHN CONA value with single rank:$CHN0_EMI_CONA_VAL[$id],$CHN1_EMI_CONA_VAL[$id]"
                        }
                    }
                    else # single ch
                    {
                        if (($hex_val_ch0 & 0x1))
                        {
                            die "[Error] LPDDR4 Wrong Single CHN CONA value with single rank:$CHN0_EMI_CONA_VAL[$id]"
                        }
                    }
                }
            }
            else # LPDDR3
            {
                if ($hex_val & 0x20000)
                {
                    die "[Error] LPDDR3 Wrong CONA value with single rank:$DENSITY[$id],$EMI_CONA_VAL[$id]";
                }
            }
        }
	#rank size check END

        if ($DEV_TYPE[$id] eq "Discrete DDR1")
        {
            $DEV_TYPE1[$id] = "00" ;
            $DEV_TYPE2[$id] = "01" ;
	          die "[Error][EMIgen] no support DDR1.";

	          $NAND_EMMC_ID[$id]="0x00";
	          $FW_ID[$id]="0x00"
        }
        elsif ($DEV_TYPE[$id] eq "Discrete LPDDR2")
        {
            $DEV_TYPE1[$id] = "00" ;
            $DEV_TYPE2[$id] = "02" ;
            die "[Error][EMIgen] no support LPDDR2.";

	          $NAND_EMMC_ID[$id]="";
	          $FW_ID[$id]=""
        }
        elsif ($DEV_TYPE[$id] eq "Discrete Tablet LPDDR2") # Denali Tablet (Darren)
        {
            $DEV_TYPE1[$id] = "0F" ;
            $DEV_TYPE2[$id] = "02" ;
            die "[Error][EMIgen] no support Discrete LPDDR2.";

	          $NAND_EMMC_ID[$id]="";
	          $FW_ID[$id]=""
        }
        elsif ($DEV_TYPE[$id] eq "Discrete LPDDR3")
        {
            $DEV_TYPE1[$id] = "00" ;
            $DEV_TYPE2[$id] = "03" ;
	          $DIS_LPDDR3 = $DIS_LPDDR3 + 1 ;

	          $NAND_EMMC_ID[$id]="";
	          $FW_ID[$id]=""
        }
        elsif ($DEV_TYPE[$id] eq "Discrete LPDDR4")
        {
            $DEV_TYPE1[$id] = "00" ;
            $DEV_TYPE2[$id] = "05" ;
	          $DIS_LPDDR4 = $DIS_LPDDR4 + 1 ;

	          $NAND_EMMC_ID[$id]="";
	          $FW_ID[$id]=""
        }
        elsif ($DEV_TYPE[$id] eq "Discrete LPDR4X")
        {
            $DEV_TYPE1[$id] = "00" ;
            $DEV_TYPE2[$id] = "06" ;
	          $DIS_LPDDR4X = $DIS_LPDDR4X + 1 ;

	          $NAND_EMMC_ID[$id]="";
	          $FW_ID[$id]=""
        }
        elsif ($DEV_TYPE[$id] eq "Discrete LPDR4P")
        {
            $DEV_TYPE1[$id] = "00" ;
            $DEV_TYPE2[$id] = "07" ;
	          $DIS_LPDDR4P = $DIS_LPDDR4P + 1 ;

	          $NAND_EMMC_ID[$id]="";
	          $FW_ID[$id]=""
        }
	elsif ($DEV_TYPE[$id] eq "Discrete PCDDR3")
        {
            $DEV_TYPE1[$id] = "00" ;
            $DEV_TYPE2[$id] = "04" ;
            die "[Error][EMIgen] no support Discrete PCDDR3.";
        }
        elsif ($DEV_TYPE[$id] eq "MCP(NAND+DDR1)")
        {
            $DEV_TYPE1[$id] = "01" ;
            $DEV_TYPE2[$id] = "01" ;
	          die "[Error][EMIgen] no support NAND+DDR1.";
        }
        elsif ($DEV_TYPE[$id] eq "MCP(NAND+LPDDR2)")
        {
            $DEV_TYPE1[$id] = "01" ;
            $DEV_TYPE2[$id] = "02" ;
	          die "[Error][EMIgen] no support NAND+LPDDR2.";
        }
        elsif ($DEV_TYPE[$id] eq "MCP(NAND+LPDDR3)")
        {
            $DEV_TYPE1[$id] = "01" ;
            $DEV_TYPE2[$id] = "03" ;
	          die "[Error][EMIgen] no support NAND+LPDDR3.";
        }
	      elsif ($DEV_TYPE[$id] eq "MCP(NAND+PCDDR3)")
        {
            $DEV_TYPE1[$id] = "01" ;
            $DEV_TYPE2[$id] = "04" ;
	          die "[Error][EMIgen] no support NAND+PCDDR3.";
        }
        elsif ($DEV_TYPE[$id] eq "MCP(eMMC+DDR1)")
        {
            $DEV_TYPE1[$id] = "02" ;
            $DEV_TYPE2[$id] = "01" ;
            die "[Error][EMIgen] no support DDR1.";
        }
        elsif ($DEV_TYPE[$id] eq "MCP(eMMC+LPDDR2)")
        {
            $DEV_TYPE1[$id] = "02" ;
            $DEV_TYPE2[$id] = "02" ;
	          die "[Error][EMIgen] no support LPDDR2.";
        }
        elsif ($DEV_TYPE[$id] eq "MCP(eMMC+LPDDR3)")
        {
            $DEV_TYPE1[$id] = "02" ;
            $DEV_TYPE2[$id] = "03" ;
	          $MCP_LPDDR3 = $MCP_LPDDR3 + 1 ;
        }
        elsif ($DEV_TYPE[$id] eq "MCP(eMMC+LPDDR4)")
        {
            $DEV_TYPE1[$id] = "02" ;
            $DEV_TYPE2[$id] = "05" ;
	          $MCP_LPDDR4 = $MCP_LPDDR4 + 1 ;
        }
        elsif ($DEV_TYPE[$id] eq "MCP(eMMC+LPDR4X)")
        {
            $DEV_TYPE1[$id] = "02" ;
            $DEV_TYPE2[$id] = "06" ;
	          $MCP_LPDDR4X = $MCP_LPDDR4X + 1 ;
        }
        elsif ($DEV_TYPE[$id] eq "MCP(eMMC+LPDR4P)")
        {
            $DEV_TYPE1[$id] = "02" ;
            $DEV_TYPE2[$id] = "07" ;
	          $MCP_LPDDR4P = $MCP_LPDDR4P + 1 ;
        }
        elsif ($DEV_TYPE[$id] eq "MCP(eMMC+PCDDR3)")
        {
            $DEV_TYPE1[$id] = "02" ;
            $DEV_TYPE2[$id] = "04" ;
	          die "[Error][EMIgen] no support PCDDR3.";
        }
        elsif ($DEV_TYPE[$id] eq "MCP(UFS+LPDR4)")
        {
            $DEV_TYPE1[$id] = "03" ;
            $DEV_TYPE2[$id] = "05" ;
	          $MCP_LPDDR4 = $MCP_LPDDR4 + 1 ;
        }
        elsif ($DEV_TYPE[$id] eq "MCP(UFS+LPDR4X)")
        {
            $DEV_TYPE1[$id] = "03" ;
            $DEV_TYPE2[$id] = "06" ;
	          $MCP_LPDDR4X = $MCP_LPDDR4X + 1 ;
        }
        else
        {
            die "[Error] unknown mcp type $DEV_TYPE[$id] \n" ;
        }

	      #LPDDR2 MODE_REG5 empty value check START
	      if (($DEV_TYPE2[$id] eq "02") && ($LPDDR2_MODE_REG5[$id] eq ''))
	      {
                 die "[Error](".$id.")MDL error, LPDDR2 but no LPDDR2_MODE_REG5 information, please update the MDL\n";
	      }
	      if (($DEV_TYPE2[$id] eq "03") && ($LPDDR3_MODE_REG5[$id] eq ''))
	      {
                 die "[Error](".$id.")MDL error, LPDDR3 but no LPDDR3_MODE_REG5 information, please update the MDL\n";
	      }
	      if (($DEV_TYPE2[$id] eq "05") && ($LPDDR4_MODE_REG5[$id] eq ''))
	      {
                 die "[Error](".$id.")MDL error, LPDDR4 but no LPDDR4_MODE_REG5 information, please update the MDL\n";
	      }
	      if (($DEV_TYPE2[$id] eq "06") && ($LPDDR4X_MODE_REG5[$id] eq ''))
	      {
                 die "[Error](".$id.")MDL error, LPDDR4X but no LPDDR4X_MODE_REG5 information, please update the MDL\n";
	      }
	      if (($DEV_TYPE2[$id] eq "07") && ($LPDDR4P_MODE_REG5[$id] eq ''))
	      {
                 die "[Error](".$id.")MDL error, LPDDR4P but no LPDDR4P_MODE_REG5 information, please update the MDL\n";
	      }
	      #LPDDR2 MODE_REG5 empty value check END

        print "NAND_EMMC_ID:$NAND_EMMC_ID[$id]\n";
        # To parse NAND_EMMC_ID, we only support 12 bytes ID
        if (length($NAND_EMMC_ID[$id]) % 2)
        {
            die "[Error] The wrong NAND_EMMC_ID: $NAND_EMMC_ID[$id] !! It is not byte align";
        }
	      if (length($FW_ID[$id]) % 2)
        {
            die "[Error] The wrong FW_ID: $FW_ID[$id] !! It is not byte align";
        }
        $ID_String[$id] = "{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}" ;
        $FW_ID_String[$id] = "{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}" ;
        $Sub_version[$id] = "0x1";
        $ID_Length[$id] = (length($NAND_EMMC_ID[$id])-2)/2;
        $FW_ID_Length[$id] = (length($FW_ID[$id])-2)/2;

	      if ($ID_Length[$id] < 0)
	      {
	      	$ID_Length[$id] = 0;
	      }
	      if ($FW_ID_Length[$id] < 0)
	      {
	      	$FW_ID_Length[$id] = 0;
	      }
	      $nand_emmc_id_len = $ID_Length[$id];
        print $ID_Length[$id].$FW_ID_Length[$id];
        if ($ID_Length[$id]> 0)
        {
            my @NAND_VECTOR = ($NAND_EMMC_ID[$id] =~ m/([\dA-Fa-f]{2})/gs);
            #create NAND_EMMC_ID string
            $last = pop(@NAND_VECTOR);
            $ID_String[$id] = "{";
            foreach $a (@NAND_VECTOR)
            {
                $ID_String[$id] .= "0x$a,";
            }
            $ID_String[$id] .= "0x$last";
            # To add ZERO in the end
            if ($nand_emmc_id_len < $MAX_NAND_EMMC_ID_LEN)
            {
                for($i = 0; $i <($MAX_NAND_EMMC_ID_LEN - $ID_Length[$id]); $i++)
                {
                    $ID_String[$id].= ",0x0"
                }
            }
            $ID_String[$id] .= "}";
        }
        else{
            $ID_Length[$id] = 0;
        }
        print "$ID_String[$id]\n" ;


        #create FW ID string
        if ($FW_ID_Length[$id] > 0)
        {
            my @FW_ID_VECTOR = ($FW_ID[$id] =~ m/([\dA-Fa-f]{2})/gs);
            $last = pop(@FW_ID_VECTOR);
            $FW_ID_String[$id] = "{";
            foreach $a (@FW_ID_VECTOR)
            {
                $FW_ID_String[$id] .= "0x$a,";
            }
            $FW_ID_String[$id] .= "0x$last";
            # To add ZERO in the end
            if ($fw_id_len < $MAX_FW_ID_LEN)
            {
                for($i = 0; $i <($MAX_FW_ID_LEN - $FW_ID_Length[$id]); $i++)
                {
                    $FW_ID_String[$id].= ",0x0"
                }
            }
            $FW_ID_String[$id] .= "}";
        }
        else{
            $FW_ID_Length[$id] = 0;
        }
        print "$FW_ID_String[$id]\n" ;

        $EMI_SETTINGS[$id] = "\n\t//$PartNum\n\t{\n\t\t" ;
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $Sub_version[$id] . ",\t\t/* sub_version */\n\t\t" ;
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . "0x" . $DEV_TYPE1[$id] . $DEV_TYPE2[$id] . ",\t\t/* TYPE */\n\t\t" ;
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $ID_Length[$id] . ",\t\t/* EMMC ID/FW ID checking length */\n\t\t" ;
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $FW_ID_Length[$id] . ",\t\t/* FW length */\n\t\t" ;
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $ID_String[$id] . ",\t\t/* NAND_EMMC_ID */\n\t\t" ;
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $FW_ID_String[$id] . ",\t\t/* FW_ID */\n\t\t" ;

        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $EMI_CONA_VAL[$id] . ",\t\t/* EMI_CONA_VAL */\n\t\t" ;
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $EMI_CONH_VAL[$id] . ",\t\t/* EMI_CONH_VAL */\n\t\t" ;

        $AC_TIME_VALUE[$id][0] = sprintf("0x%08X",($AC_TIME_EMI_FREQUENCY[$id] + ($AC_TIME_EMI_TRAS[$id] << 16) + ($AC_TIME_EMI_TRP[$id] << 24)));
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $AC_TIME_VALUE[$id][0] . ",\t\t/* U 00 */\n\t\t" ;

        $AC_TIME_VALUE[$id][1] = sprintf("0x%08X",(($AC_TIME_EMI_TRPAB[$id]) + ($AC_TIME_EMI_TRC[$id] << 8) + ($AC_TIME_EMI_TRFC[$id] << 16) + ($AC_TIME_EMI_TRFCPB[$id] << 24)));
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $AC_TIME_VALUE[$id][1] . ",\t\t/* U 01 */\n\t\t" ;

        $AC_TIME_VALUE[$id][2] = sprintf("0x%08X",(($AC_TIME_EMI_TXP[$id]) + ($AC_TIME_EMI_TRTP[$id] << 8) + ($AC_TIME_EMI_TRCD[$id] << 16) + ($AC_TIME_EMI_TWR[$id] << 24)));
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $AC_TIME_VALUE[$id][2] . ",\t\t/* U 02 */\n\t\t" ;

        $AC_TIME_VALUE[$id][3] = sprintf("0x%08X",(($AC_TIME_EMI_TWTR[$id]) + ($AC_TIME_EMI_TRRD[$id] << 8) + ($AC_TIME_EMI_TFAW[$id] << 16) + ($AC_TIME_EMI_TRTW_ODT_OFF[$id] << 24) + ($AC_TIME_EMI_TRTW_ODT_ON[$id] << 28)));
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $AC_TIME_VALUE[$id][3] . ",\t\t/* U 03 */\n\t\t" ;

        $AC_TIME_VALUE[$id][4] = sprintf("0x%08X",(($AC_TIME_EMI_REFCNT[$id]) + ($AC_TIME_EMI_REFCNT_FR_CLK[$id] << 8) + ($AC_TIME_EMI_TXREFCNT[$id] << 16) + ($AC_TIME_EMI_TZQCS[$id] << 24)));
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $AC_TIME_VALUE[$id][4] . ",\t\t/* U 04 */\n\t\t" ;

        $AC_TIME_VALUE[$id][5] = sprintf("0x%08X",(($AC_TIME_EMI_TRTPD[$id]) + ($AC_TIME_EMI_TWTPD[$id] << 8) + ($AC_TIME_EMI_TMRR2W_ODT_OFF[$id] << 16) + ($AC_TIME_EMI_TMRR2W_ODT_ON[$id] << 24)));
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $AC_TIME_VALUE[$id][5] . ",\t\t/* U 05 */\n\t\t" ;

        $AC_TIME_VALUE[$id][6] = sprintf("0x%08X",(($AC_TIME_EMI_TRAS_05T[$id]) + ($AC_TIME_EMI_TRP_05T[$id] << 2) + ($AC_TIME_EMI_TRPAB_05T[$id] << 4) +
                                                ($AC_TIME_EMI_TRC_05T[$id] << 6) + ($AC_TIME_EMI_TRFC_05T[$id] << 8) + ($AC_TIME_EMI_TRFCPB_05T[$id] << 10) +
                                                ($AC_TIME_EMI_TXP_05T[$id] << 12) + ($AC_TIME_EMI_TRTP_05T[$id] << 14) + ($AC_TIME_EMI_TRCD_05T[$id] << 16) +
                                                ($AC_TIME_EMI_TWR_05T[$id] << 18) + ($AC_TIME_EMI_TWTR_05T[$id] << 20) + ($AC_TIME_EMI_TRRD_05T[$id] << 22) +
                                                ($AC_TIME_EMI_TFAW_05T[$id] << 24) + ($AC_TIME_EMI_TRTW_ODT_OFF_05T[$id] << 26) + ($AC_TIME_EMI_TRTW_ODT_ON_05T[$id] << 28) +
                                                ($AC_TIME_EMI_TRTPD_05T[$id] << 30)));
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $AC_TIME_VALUE[$id][6] . ",\t\t/* U 06 */\n\t\t" ;

        $AC_TIME_VALUE[$id][7] = sprintf("0x%08X",(($AC_TIME_EMI_TWTPD_05T[$id]) + ($AC_TIME_EMI_TMRR2W_ODT_OFF_05T[$id] << 2) + ($AC_TIME_EMI_TMRR2W_ODT_ON_05T[$id] << 4)));
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $AC_TIME_VALUE[$id][7] . ",\t\t/* U 07 */\n\t\t" ;

#        $AC_TIME_VALUE[7] = sprintf("0x%X",(($AC_TIME_EMI_TXREFCNT_05T[$id]) + ($AC_TIME_EMI_TZQCS_05T[$id] << 2) + ($AC_TIME_EMI_XRTW2W[$id] << 4) + ($AC_TIME_EMI_XRTW2R[$id] << 8) +
#                                                ($AC_TIME_EMI_XRTR2W[$id] << 12) + ($AC_TIME_EMI_XRTR2R[$id] << 16) + ($AC_TIME_EMI_DUMMY_02[$id] << 20)));
#        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $AC_TIME_VALUE . ",\t\t/* U 06 */\n\t\t" ;
#
#        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $AC_TIME_EMI_WRITE_LATENCY_0x41c[$id] . ",\t\t/* U 07 */\n\t\t" ;
#        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $AC_TIME_EMI_WRITE_LATENCY_0x420[$id] . ",\t\t/* U 08 */\n\t\t" ;
#        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $AC_TIME_EMI_WRITE_LATENCY_0x424[$id] . ",\t\t/* U 09 */\n\t\t" ;
#        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $AC_TIME_EMI_WRITE_LATENCY_0x428[$id] . ",\t\t/* U 10 */\n\t\t" ;
#        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $AC_TIME_EMI_WRITE_LATENCY_0x42c[$id] . ",\t\t/* U 11 */\n\t\t" ;
#
#        $AC_TIME_VALUE = sprintf("0x%X",(($AC_TIME_EMI_DQSINCTL_FOR_GATING[$id]) + ($AC_TIME_EMI_DATLAT[$id] << 8) + ($AC_TIME_EMI_MODE_REG_WL[$id] << 16) + ($AC_TIME_EMI_MODE_REG_RL[$id] << 24)));
#        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $AC_TIME_VALUE . ",\t\t/* U 12 */\n\t\t" ;

        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . "{" . $DRAM_RANK0_SIZE[$id] . "," .  $DRAM_RANK1_SIZE[$id] . ",0,0},\t\t/* DRAM RANK SIZE */\n\t\t" ;
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $EMI_CONF_VAL[$id] . ",\t\t/* EMI_CONF_VAL */\n\t\t" ;
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $CHN0_EMI_CONA_VAL[$id] . ",\t\t/* CHN0_EMI_CONA_VAL */\n\t\t" ;
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $CHN1_EMI_CONA_VAL[$id] . ",\t\t/* CHN1_EMI_CONA_VAL */\n\t\t" ;
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $MODE_EXTERN_VAL[$id] . ",\t\t/* dram_cbt_mode_extern */\n\t\t" ;
        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . "{0,0,0,0,0,0},\t\t/* reserved 6 */\n\t\t" ;

#union
#1 LPDDR2
        if ($DDR1_2_3[$id] eq "LPDDR2")
        {
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $LPDDR2_MODE_REG1[$id] . ",\t\t/* LPDDR2_MODE_REG1 */\n\t\t" ;
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $LPDDR2_MODE_REG2[$id] . ",\t\t/* LPDDR2_MODE_REG2 */\n\t\t" ;
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $LPDDR2_MODE_REG3[$id] . ",\t\t/* LPDDR2_MODE_REG3 */\n\t\t" ;
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $LPDDR2_MODE_REG5[$id] . ",\t\t/* LPDDR2_MODE_REG5 */\n\t\t" ;
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $LPDDR2_MODE_REG10[$id] . ",\t\t/* LPDDR2_MODE_REG10 */\n\t\t" ;
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $LPDDR2_MODE_REG63[$id] . ",\t\t/* LPDDR2_MODE_REG63 */\n\t}" ;
        }
#2 DDR1
        elsif ($DDR1_2_3[$id] eq "DDR1")
        {
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $DDR1_MODE_REG[$id] . ",\t\t/* DDR1_MODE_REG */\n\t\t" ;
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $DDR1_EXT_MODE_REG[$id] . "\t\t/* DDR1_EXT_MODE_REG */\n\t}" ;
        }
#3 PCDDR3
        elsif ($DDR1_2_3[$id] eq "PCDDR3")
        {
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $PCDDR3_MODE_REG0[$id] . ",\t\t/* PCDDR3_MODE_REG0 */\n\t\t" ;
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $PCDDR3_MODE_REG1[$id] . ",\t\t/* PCDDR3_MODE_REG1 */\n\t\t" ;
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $PCDDR3_MODE_REG2[$id] . ",\t\t/* PCDDR3_MODE_REG2 */\n\t\t" ;
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $PCDDR3_MODE_REG3[$id] . ",\t\t/* PCDDR3_MODE_REG3 */\n\t\t" ;
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $PCDDR3_MODE_REG4[$id] . ",\t\t/* PCDDR3_MODE_REG4 */\n\t\t" ;
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $PCDDR3_MODE_REG5[$id] . ",\t\t/* PCDDR3_MODE_REG5 */\n\t}" ;
        }
#3 LPDDR3
        elsif ($DDR1_2_3[$id] eq "LPDDR3")
        {
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $LPDDR3_MODE_REG1[$id] . ",\t\t/* LPDDR3_MODE_REG1 */\n\t\t" ;
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $LPDDR3_MODE_REG2[$id] . ",\t\t/* LPDDR3_MODE_REG2 */\n\t\t" ;
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $LPDDR3_MODE_REG3[$id] . ",\t\t/* LPDDR3_MODE_REG3 */\n\t\t" ;
            $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $LPDDR3_MODE_REG5[$id] . ",\t\t/* LPDDR3_MODE_REG5 */\n\t\t" ;
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $LPDDR3_MODE_REG10[$id] . ",\t\t/* LPDDR3_MODE_REG10 */\n\t\t" ;
            #$EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $LPDDR3_MODE_REG63[$id] . ",\t\t/* LPDDR3_MODE_REG63 */\n\t}" ;
        }
#4 LPDDR4/4X/4P
        elsif ($DDR1_2_3[$id] eq "LPDDR4")
        {
            $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $LPDDR4_MODE_REG5[$id] . ",\t\t/* LPDDR4_MODE_REG5 */\n\t\t" ;
        }
        elsif ($DDR1_2_3[$id] eq "LPDR4X")
        {
            $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $LPDDR4X_MODE_REG5[$id] . ",\t\t/* LPDDR4X_MODE_REG5 */\n\t\t" ;
        }
        elsif ($DDR1_2_3[$id] eq "LPDR4P")
        {
            $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $LPDDR4P_MODE_REG5[$id] . ",\t\t/* LPDDR4P_MODE_REG5 */\n\t\t" ;
        }

        $EMI_SETTINGS[$id] = $EMI_SETTINGS[$id] . $PIN_MUX_TYPE[$id] . ",\t\t/* PIN_MUX_TYPE for tablet */\n\t}" ;
        print  $EMI_SETTINGS[$id] ;
        print "\n\n" ;
        $id ++;
    }

    &check_dram_rank_size_by_EMI_settings($id); #Darren+

    # return total_part_number_iter
    return $id;

}


#****************************************************************************************
# subroutine:  check_dram_rank_size_by_EMI_settings
# return:      which os this script is running
# input:       no input
#****************************************************************************************
sub check_dram_rank_size_by_EMI_settings {
  $hex_CONA = hex($EMI_CONA_VAL[$id]);
  $hex_CONH = hex($EMI_CONH_VAL[$id]);
  $shift_for_16bit = 1;

  #Is it 32-bit or 16-bit I/O
  if ($hex_CONA & 0x2)
  {
    $shift_for_16bit = 0;
  }

  $dram_rank0_size = 0;
  $dram_rank1_size = 0;

  $ch0_rank0_size = ($hex_CONH >> 16) & 0xf;
  $ch0_rank1_size = ($hex_CONH >> 20) & 0xf;
  $ch1_rank0_size = ($hex_CONH >> 24) & 0xf;
  $ch1_rank1_size = ($hex_CONH >> 28) & 0xf;

    if($ch0_rank0_size == 0)
    {
        #rank 0 setting
        $col_bit = (($hex_CONA >> 4) & 0x03) + 9;
        $row_bit = ((($hex_CONA >> 24) & 0x1) << 2) + (($hex_CONA >> 12) & 0x03) + 13;
        $dram_rank0_size = (1 << ($row_bit + $col_bit)) * (4 >> $shift_for_16bit) * 8; #4 or 2 byte * 8 banks
        print "[Debug] dram_rank0_size:  $dram_rank0_size\n";
    }
    else
    {
        $dram_rank0_size = ($ch0_rank0_size * 256 << 20);
    }

    if (0 != ($hex_CONA & (1 << 17)))   #rank 1 exist
    {
        if($ch0_rank1_size == 0)
        {
            $col_bit = (($hex_CONA >> 6) & 0x03) + 9;
            $row_bit = ((($hex_CONA >> 25) & 0x1) << 2) + (($hex_CONA >> 14) & 0x03) + 13;
            $dram_rank1_size = ((1 << ($row_bit + $col_bit)) * (4 >> $shift_for_16bit) * 8); #4 or 2 byte * 8 banks
            print "[Debug] dram_rank1_size:  $dram_rank1_size\n";
        }
        else
        {
            $dram_rank1_size = ($ch0_rank1_size * 256 << 20);
        }
    }

    if(0 != (($hex_CONA >> 8) & 0x01))     #channel 1 exist
    {
        if($ch1_rank0_size == 0)
        {
            #rank0 setting
            $col_bit = (($hex_CONA >> 20) & 0x03) + 9;
            $row_bit = ((($hex_CONH >> 4) & 0x1) << 2) + (($hex_CONA >> 28) & 0x03) + 13;
            $dram_rank0_size += ((1 << ($row_bit + $col_bit)) * (4 >> $shift_for_16bit) * 8); #4 or 2 byte * 8 banks
            print "[Debug] dram_rank0_size:  $dram_rank0_size\n";
        }
        else
        {
            $dram_rank0_size += ($ch1_rank0_size * 256 << 20);
        }

        if (0 != ($hex_CONA &  (1 << 16)))   #rank 1 exist
        {
            if($ch1_rank1_size == 0)
            {
                $col_bit = (($hex_CONA >> 22) & 0x03) + 9;
                $row_bit = ((($hex_CONH >> 5) & 0x1) << 2) + (($hex_CONA >> 30) & 0x03) + 13;
                $dram_rank1_size += ((1 << ($row_bit + $col_bit)) * (4 >> $shift_for_16bit) * 8); #4 or 2 byte * 8 banks
                print "[Debug] dram_rank1_size:  $dram_rank1_size\n";
            }
            else
            {
                $dram_rank1_size += ($ch1_rank1_size * 256 << 20);
            }
        }
    }

    if((($hex_CONA >> 8) & 0x01) >= 2) # for qual channel
    {
        $dram_rank0_size = $dram_rank0_size * 2;
        $dram_rank1_size = $dram_rank1_size * 2;
    }

    print "[Debug] DRAM rank0 size: $dram_rank0_size, DRAM rank1 size: $dram_rank1_size\n";
    $mdl_r0_size = hex($DRAM_RANK0_SIZE[$id]);
    $mdl_r1_size = hex($DRAM_RANK1_SIZE[$id]);
    print "[Debug] DRAM_RANK0_SIZE: $mdl_r0_size, DRAM_RANK1_SIZE: $mdl_r1_size\n";

    if (($mdl_r0_size != $dram_rank0_size) || ($mdl_r1_size != $dram_rank1_size))
    {
        die "[Error][DRAMC] DRAM size settings incorrect between EMI CONA/CONH and MDL Density !!!\n";
    }
}


#****************************************************************************************
# subroutine:  OsName
# return:      which os this script is running
# input:       no input
#****************************************************************************************
sub OsName {
  my $os = `set os`;
  if(!defined $os) {
    $os = "linux";
  }
  else {
    die "[Error]does not support windows now!!" ;
    $os = "windows";
  }
}
#*************************************************************************************************
# subroutine:  gen_pm
# return:      no return, but will generate a ForWindows.pm in "/perl/lib" where your perl install
#*************************************************************************************************
sub gen_pm {
  foreach (@INC) {
    if(/^.*:\/Perl\/lib$/) {
      open FILE, ">${_}\/ForWindows.pm";
      print FILE "package ForWindows;\n";
      print FILE "use Win32::OLE qw(in with);\n";
      print FILE "use Win32::OLE::Const 'Microsoft Excel';\n";
      print FILE "\$Win32::OLE::Warn = 3;\n";
      print FILE "1;";
      close(FILE);
      last;
    }
  }
}
#****************************************************************************************
# subroutine:  get_sheet
# return:      Excel worksheet no matter it's in merge area or not, and in windows or not
# input:       Specified Excel Sheetname
#****************************************************************************************
sub get_sheet {
  my $MEMORY_DEVICE_TYPE = $_[0];

  print $MEMORY_DEVICE_TYPE ;

  if ($os eq "windows") {
    return $Sheet     = $Book->Worksheets($MEMORY_DEVICE_TYPE);
  }
  else {
    return $Sheet     = $Book->Worksheet($MEMORY_DEVICE_TYPE);
  }
}


#****************************************************************************************
# subroutine:  xls_cell_value
# return:      Excel cell value no matter it's in merge area or not, and in windows or not
# input:       $Sheet:  Specified Excel Sheet
# input:       $row:    Specified row number
# input:       $col:    Specified column number
#****************************************************************************************
sub xls_cell_value {
  my ($Sheet, $row, $col) = @_;
  if ($os eq "windows") {
    return &trim(&win_xls_cell_value($Sheet, $row, $col));
  }
  else {
      return &trim(&lin_xls_cell_value($Sheet, $row, $col));
  }
}
sub win_xls_cell_value
{
    my ($Sheet, $row, $col) = @_;

    if ($Sheet->Cells($row, $col)->{'MergeCells'})
    {
        my $ma = $Sheet->Cells($row, $col)->{'MergeArea'};
        return ($ma->Cells(1, 1)->{'Value'});
    }
    else
    {
        return ($Sheet->Cells($row, $col)->{'Value'})
    }
}
sub lin_xls_cell_value
{
  my ($Sheet, $row, $col) = @_;
  my $cell = $Sheet->get_cell($row, $col);
  return "" unless (defined $cell);
  my $value = $cell->Value();

}

sub write_tag()
{
    my $project = lc($_[0]);
    my $filesize = 0x0 ;
    my $ddr = -1 ;

    if (-e $INFO_TAG)
    {
        unlink ($INFO_TAG);
    }
    my $temp_path = `dirname $INFO_TAG`;
    `mkdir -p $temp_path`;

    open FILE,">$INFO_TAG";

    print FILE pack("a24", "MTK_BLOADER_INFO_v30"); #MT6763
#    print FILE pack("a24", "MTK_BLOADER_INFO_v00");
    $filesize = $filesize + 24 ;
    seek(FILE, 0x1b, 0);
    $pre_bin = "preloader_${project}.bin";
    print "PROJECT = $project, pre_bin = $pre_bin\n";
    print FILE pack("a64", $pre_bin);
    $filesize = $filesize + 64 ;
    seek(FILE, 0x58, 0);
    print FILE pack("H8", 56313136);
    $filesize = $filesize + 4 ;
    print FILE pack("H8", 22884433);
    $filesize = $filesize + 4 ;
    print FILE pack("H8", "90007000");
    $filesize = $filesize + 4 ;
    print FILE pack("a8", "MTK_BIN");
    $filesize = $filesize + 8 ;

#    print FILE pack("H8", bc000000);


    seek(FILE,0x6c, 0);
    # 1.LPDDR2/LPDDR3/LPDDR4/LPDDR4X/LPDDR4P discrete dram number >= 2
    #[DEL] 2.LPDDR2 discrete dram > 0, LPDDR2 MCP > 0
    #[DEL] 3.LPDDR3 discrete dram > 0, LPDDR3 MCP > 0
    #[DEL] 4.LPDDR4 discrete dram > 0, LPDDR4 MCP > 0
    #[DEL] 5.LPDDR4X discrete dram > 0, LPDDR4X MCP > 0
    if (($Discrete_DDR >= 2))
    #if (($Discrete_DDR >= 2) || (($DIS_LPDDR2 > 0) && ($MCP_LPDDR2 > 0)) || (($DIS_LPDDR3 > 0) && ($MCP_LPDDR3 > 0)))
    {
    	    print "[EMIgen] Have multiple discrete dram\n";
	    print FILE pack("L", $TotalCustemChips+1);   # number of emi settings + 1 default value emi_settings.
    }else
    {
	    print FILE pack("L", $TotalCustemChips);     # number of emi settings.
    }
    $filesize = $filesize + 4 ;

    my $iter = 0 ;

    # 1.LPDDR2/LPDDR3/LPDDR4/LPDDR4X/LPDDR4P discrete dram number >= 2
    #[DEL] 2.LPDDR2 discrete dram > 0, LPDDR2 MCP > 0
    #[DEL] 3.LPDDR3 discrete dram > 0, LPDDR3 MCP > 0
    #[DEL] 4.LPDDR4 discrete dram > 0, LPDDR4 MCP > 0
    #[DEL] 5.LPDDR4X discrete dram > 0, LPDDR4X MCP > 0
    #[DEL] 6.LPDDR4P discrete dram > 0, LPDDR4P MCP > 0
    if (($Discrete_DDR >= 2))
    #if (($Discrete_DDR >= 2) || (($DIS_LPDDR2 > 0) && ($MCP_LPDDR2 > 0)) || (($DIS_LPDDR3 > 0) && ($MCP_LPDDR3 > 0)))
    {

	    if ($DEV_TYPE2[0] eq "02")
	    {
		    $filesize = $filesize + &write_tag_one_element_default_LPDDR2() ;
	    }
	    elsif ($DEV_TYPE2[0] eq "03")
	    {
		    $filesize = $filesize + &write_tag_one_element_default_LPDDR3() ;
	    }
	    elsif ($DEV_TYPE2[0] eq "05")
	    {
		    $filesize = $filesize + &write_tag_one_element_default_LPDDR4() ;
	    }
	    elsif ($DEV_TYPE2[0] eq "06")
	    {
		    $filesize = $filesize + &write_tag_one_element_default_LPDDR4X() ;
	    }
	    elsif ($DEV_TYPE2[0] eq "07")
	    {
		    $filesize = $filesize + &write_tag_one_element_default_LPDDR4P() ;
	    }
    }
    for $iter (0..$TotalCustemChips-1)
    {
    	# generate MCP dram
        if ($DEV_TYPE1[$iter] != "00")
        {
            $filesize = $filesize + &write_tag_one_element ($iter) ;
        }
    }
    for $iter (0..$TotalCustemChips-1)
    {
    	# generate discrete dram
        if ($DEV_TYPE1[$iter] == "00")
        {
            $filesize = $filesize + &write_tag_one_element ($iter) ;
        }
    }
#    $filesize = $filesize + 4 ;

#    print "2.file size is $filesize \n";

    print FILE pack("L", $filesize) ;

    close (FILE) ;
    print "$INFO_TAG is generated!\n";
    return ;
}

sub write_tag_one_element_default_LPDDR2()
{
    my $id = $_[0];
    my $fs = 0 ;
    print "in write_tag_one_element_default_LPDDR2";
    print FILE pack ("L", hex (lc("0x0"))) ;        # Sub_version checking for flash tool
    $fs = $fs + 4 ;

    if ($DEV_TYPE1[0] eq "0F")
    {
        print FILE pack("L", hex("0x0F02"));                           #type
    }
    else
    {
        print FILE pack("L", hex("0x0002"));                           #type
    }
    $fs = $fs + 4 ;

    print FILE pack ("L", scalar("0")) ;        # EMMC ID checking length
    $fs = $fs + 4 ;

    print FILE pack ("L", scalar("0")) ;        # FW ID checking length
    $fs = $fs + 4 ;

    $_ = "{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}" ;
    if (/(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+)/)
    {
        print FILE pack ("C", hex ($1)) ;            #id
        print FILE pack ("C", hex ($2)) ;
        print FILE pack ("C", hex ($3)) ;
        print FILE pack ("C", hex ($4)) ;
        print FILE pack ("C", hex ($5)) ;
        print FILE pack ("C", hex ($6)) ;
        print FILE pack ("C", hex ($7)) ;
        print FILE pack ("C", hex ($8)) ;
        print FILE pack ("C", hex ($9)) ;
        print FILE pack ("C", hex ($10)) ;
        print FILE pack ("C", hex ($11)) ;
        print FILE pack ("C", hex ($12)) ;
        print FILE pack ("C", hex ($13)) ;
        print FILE pack ("C", hex ($14)) ;
        print FILE pack ("C", hex ($15)) ;
        print FILE pack ("C", hex ($16)) ;
        $fs = $fs + 16 ;
    }
    $_= "{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}";
    if (/(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+)/)
    {
        print FILE pack ("C", hex ($1)) ;            #fw id
        print FILE pack ("C", hex ($2)) ;
        print FILE pack ("C", hex ($3)) ;
        print FILE pack ("C", hex ($4)) ;
        print FILE pack ("C", hex ($5)) ;
        print FILE pack ("C", hex ($6)) ;
        print FILE pack ("C", hex ($7)) ;
        print FILE pack ("C", hex ($8)) ;
        $fs = $fs + 8 ;
    }


    print FILE pack ("L", hex (lc("0x0000212E"))) ;         # EMI_CONA_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0xAA00AA00"))) ;    # DRAMC_DRVCTL0_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0xAA00AA00"))) ;    # DRAMC_DRVCTL1_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x44584493"))) ;      # DRAMC_ACTIM_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x01000000"))) ;  # DRAMC_GDDR3CTL1_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0xF0048683"))) ;      # DRAMC_CONF1_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0xA00632D1"))) ;    # DRAMC_DDR2CTL_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0xBF080401"))) ;    # DRAMC_TEST2_3_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0340633F"))) ;      # DRAMC_CONF2_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x51642342"))) ;    # DRAMC_PD_CTRL_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00008888"))) ;    # DRAMC_PADCTL3_VAL

    $fs = $fs + 4 ;
    print FILE pack ("L", hex (lc("0x88888888"))) ;     # DRAMC_DQODLY_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00000000"))) ;        # DRAMC_ADDR_OUTPUT_DLY
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00000000"))) ;        # DRAMC_CLK_OUTPUT_DLY
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x01000510"))) ;           # $DRAMC_ACTIM1_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x07800000"))) ;        # DRAMC_MISCTL0_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x04002600"))) ;           # $DRAMC_ACTIM05T_VAL
    $fs = $fs + 4 ;


    print FILE pack ("Q", hex ("0")) ;                                  #  DRAM_RANK_SIZE[4]
    print FILE pack ("Q", hex ("0")) ;
    print FILE pack ("Q", hex ("0")) ;
    print FILE pack ("Q", hex ("0")) ;
    $fs = $fs + 32 ;


    print FILE pack ("L", hex ("0")) ;                                  #reserved[10]
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    $fs = $fs + 40 ;

    if (1)
    {
        #ddr2
        print FILE pack ("L", hex (lc("0x00C30001"))) ;        # DDR2_MODE_REG1
        $fs = $fs + 4 ;

        print FILE pack ("L", hex (lc("0x00060002"))) ;        # DDR2_MODE_REG2
        $fs = $fs + 4 ;

        print FILE pack ("L", hex (lc("0x00020003"))) ;        # DDR2_MODE_REG3
        $fs = $fs + 4 ;

        print FILE pack ("L", hex (lc("0x0"))) ;               # DDR2_MODE_REG5
        $fs = $fs + 4 ;

        print FILE pack ("L", hex (lc("0x00FF000A"))) ;        # DDR2_MODE_REG10
        $fs = $fs + 4 ;

        print FILE pack ("L", hex (lc("0x0000003F"))) ;        # DDR2_MODE_REG63
        $fs = $fs + 4 ;

    }
#       print "1.file size is $fs \n";
    return $fs;

}
sub write_tag_one_element_default_LPDDR3()
{
    my $id = $_[0];
    my $fs = 0 ;

    print "in write_tag_one_element_default_LPDDR3";
    print FILE pack ("L", hex (lc("0x1"))) ;        # Sub_version checking for flash tool
    $fs = $fs + 4 ;

    print FILE pack("L", hex("0x0003"));                           #type
    $fs = $fs + 4 ;

    print FILE pack ("L", scalar("0")) ;        # EMMC ID checking length
    $fs = $fs + 4 ;

    print FILE pack ("L", scalar("0")) ;        # FW ID checking length
    $fs = $fs + 4 ;

    $_ = "{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}" ;
    if (/(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+)/)
    {
        print FILE pack ("C", hex ($1)) ;            #id
        print FILE pack ("C", hex ($2)) ;
        print FILE pack ("C", hex ($3)) ;
        print FILE pack ("C", hex ($4)) ;
        print FILE pack ("C", hex ($5)) ;
        print FILE pack ("C", hex ($6)) ;
        print FILE pack ("C", hex ($7)) ;
        print FILE pack ("C", hex ($8)) ;
        print FILE pack ("C", hex ($9)) ;
        print FILE pack ("C", hex ($10)) ;
        print FILE pack ("C", hex ($11)) ;
        print FILE pack ("C", hex ($12)) ;
        print FILE pack ("C", hex ($13)) ;
        print FILE pack ("C", hex ($14)) ;
        print FILE pack ("C", hex ($15)) ;
        print FILE pack ("C", hex ($16)) ;
        $fs = $fs + 16 ;
    }
    $_= "{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}";
    if (/(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+)/)
    {
        print FILE pack ("C", hex ($1)) ;            #fw id
        print FILE pack ("C", hex ($2)) ;
        print FILE pack ("C", hex ($3)) ;
        print FILE pack ("C", hex ($4)) ;
        print FILE pack ("C", hex ($5)) ;
        print FILE pack ("C", hex ($6)) ;
        print FILE pack ("C", hex ($7)) ;
        print FILE pack ("C", hex ($8)) ;
        $fs = $fs + 8 ;
    }


    print FILE pack ("L", hex (lc("0x0000a052"))) ;     # EMI_CONA_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00000000"))) ;     # EMI_CONH_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x070C03A5"))) ;     # AC Timing U 00
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x1F571801"))) ;     # AC Timing U 01
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0D070301"))) ;     # AC Timing U 02
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x060F0407"))) ;     # AC Timing U 03
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x28646570"))) ;     # AC Timing U 04
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x01055054"))) ;     # AC Timing U 05
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00000000"))) ;     # AC Timing U 06
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00000000"))) ;     # AC Timing U 07
    $fs = $fs + 4 ;

    print FILE pack ("Q", hex ("0")) ;                                  #  DRAM_RANK_SIZE[4]
    print FILE pack ("Q", hex ("0")) ;
    print FILE pack ("Q", hex ("0")) ;
    print FILE pack ("Q", hex ("0")) ;
    $fs = $fs + 32 ;

    print FILE pack ("L", hex (lc("0x00421000"))) ;     # EMI_CONF_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0000a052"))) ;     # CHN0_EMI_CONA_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00000002"))) ;     # CHN1_EMI_CONA_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("CBT_R0_R1_NORMAL"))) ;     # MODE_EXTERN_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex ("0")) ;					#reserved[6]
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    $fs = $fs + 24 ;

    print FILE pack ("L", hex (lc("0x0"))) ;               # DDR2_MODE_REG5
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0"))) ;               # pin-mux type for tablet
    $fs = $fs + 4 ;

#       print "1.file size is $fs \n";
    return $fs;

}

sub write_tag_one_element_default_LPDDR4()
{
    my $id = $_[0];
    my $fs = 0 ;

    print "in write_tag_one_element_default_LPDDR4";
    print FILE pack ("L", hex (lc("0x1"))) ;        # Sub_version checking for flash tool
    $fs = $fs + 4 ;

    print FILE pack("L", hex("0x0005"));                           #type
    $fs = $fs + 4 ;

    print FILE pack ("L", scalar("0")) ;        # EMMC ID checking length
    $fs = $fs + 4 ;

    print FILE pack ("L", scalar("0")) ;        # FW ID checking length
    $fs = $fs + 4 ;

    $_ = "{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}" ;
    if (/(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+)/)
    {
        print FILE pack ("C", hex ($1)) ;            #id
        print FILE pack ("C", hex ($2)) ;
        print FILE pack ("C", hex ($3)) ;
        print FILE pack ("C", hex ($4)) ;
        print FILE pack ("C", hex ($5)) ;
        print FILE pack ("C", hex ($6)) ;
        print FILE pack ("C", hex ($7)) ;
        print FILE pack ("C", hex ($8)) ;
        print FILE pack ("C", hex ($9)) ;
        print FILE pack ("C", hex ($10)) ;
        print FILE pack ("C", hex ($11)) ;
        print FILE pack ("C", hex ($12)) ;
        print FILE pack ("C", hex ($13)) ;
        print FILE pack ("C", hex ($14)) ;
        print FILE pack ("C", hex ($15)) ;
        print FILE pack ("C", hex ($16)) ;
        $fs = $fs + 16 ;
    }
    $_= "{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}";
    if (/(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+)/)
    {
        print FILE pack ("C", hex ($1)) ;            #fw id
        print FILE pack ("C", hex ($2)) ;
        print FILE pack ("C", hex ($3)) ;
        print FILE pack ("C", hex ($4)) ;
        print FILE pack ("C", hex ($5)) ;
        print FILE pack ("C", hex ($6)) ;
        print FILE pack ("C", hex ($7)) ;
        print FILE pack ("C", hex ($8)) ;
        $fs = $fs + 8 ;
    }


    print FILE pack ("L", hex (lc("0xf050f154"))) ;     # EMI_CONA_VAL
#    print FILE pack ("L", hex (lc("0x0000f050"))) ;     # EMI_CONA_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00000000"))) ;     # EMI_CONH_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x06090640"))) ;     # AC Timing U 00
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x113D1101"))) ;     # AC Timing U 01
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0D060301"))) ;     # AC Timing U 02
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x99080307"))) ;     # AC Timing U 03
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x224E6561"))) ;     # AC Timing U 04
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0B0B0C0C"))) ;     # AC Timing U 05
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00054045"))) ;     # AC Timing U 06
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00000000"))) ;     # AC Timing U 07
    $fs = $fs + 4 ;

    print FILE pack ("Q", hex ("0")) ;                                  #  DRAM_RANK_SIZE[4]
    print FILE pack ("Q", hex ("0")) ;
    print FILE pack ("Q", hex ("0")) ;
    print FILE pack ("Q", hex ("0")) ;
    $fs = $fs + 32 ;

    print FILE pack ("L", hex (lc("0x00042100"))) ;     # EMI_CONF_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0400f050"))) ;     # CHN0_EMI_CONA_VAL
    $fs = $fs + 4 ;

#    print FILE pack ("L", hex (lc("0x0400f050"))) ;     # CHN1_EMI_CONA_VAL
    print FILE pack ("L", hex (lc("0x04000000"))) ;     # CHN1_EMI_CONA_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("CBT_R0_R1_BYTE"))) ;     # MODE_EXTERN_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex ("0")) ;					#reserved[6]
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    $fs = $fs + 24 ;

    print FILE pack ("L", hex (lc("0x0"))) ;               # DDR2_MODE_REG5
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0"))) ;               # pin-mux type for tablet
    $fs = $fs + 4 ;

#       print "1.file size is $fs \n";
    return $fs;

}

sub write_tag_one_element_default_LPDDR4X()
{
    my $id = $_[0];
    my $fs = 0 ;

    print "in write_tag_one_element_default_LPDDR4X";
    print FILE pack ("L", hex (lc("0x1"))) ;        # Sub_version checking for flash tool
    $fs = $fs + 4 ;

    print FILE pack("L", hex("0x0006"));                           #type
    $fs = $fs + 4 ;

    print FILE pack ("L", scalar("0")) ;        # EMMC ID checking length
    $fs = $fs + 4 ;

    print FILE pack ("L", scalar("0")) ;        # FW ID checking length
    $fs = $fs + 4 ;

    $_ = "{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}" ;
    if (/(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+)/)
    {
        print FILE pack ("C", hex ($1)) ;            #id
        print FILE pack ("C", hex ($2)) ;
        print FILE pack ("C", hex ($3)) ;
        print FILE pack ("C", hex ($4)) ;
        print FILE pack ("C", hex ($5)) ;
        print FILE pack ("C", hex ($6)) ;
        print FILE pack ("C", hex ($7)) ;
        print FILE pack ("C", hex ($8)) ;
        print FILE pack ("C", hex ($9)) ;
        print FILE pack ("C", hex ($10)) ;
        print FILE pack ("C", hex ($11)) ;
        print FILE pack ("C", hex ($12)) ;
        print FILE pack ("C", hex ($13)) ;
        print FILE pack ("C", hex ($14)) ;
        print FILE pack ("C", hex ($15)) ;
        print FILE pack ("C", hex ($16)) ;
        $fs = $fs + 16 ;
    }
    $_= "{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}";
    if (/(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+)/)
    {
        print FILE pack ("C", hex ($1)) ;            #fw id
        print FILE pack ("C", hex ($2)) ;
        print FILE pack ("C", hex ($3)) ;
        print FILE pack ("C", hex ($4)) ;
        print FILE pack ("C", hex ($5)) ;
        print FILE pack ("C", hex ($6)) ;
        print FILE pack ("C", hex ($7)) ;
        print FILE pack ("C", hex ($8)) ;
        $fs = $fs + 8 ;
    }

    print FILE pack ("L", hex (lc("0x0000f050"))) ;     # EMI_CONA_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00000000"))) ;     # EMI_CONH_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x06090640"))) ;     # AC Timing U 00
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x113D1101"))) ;     # AC Timing U 01
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0D060301"))) ;     # AC Timing U 02
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x99080307"))) ;     # AC Timing U 03
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x224E6561"))) ;     # AC Timing U 04
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0B0B0C0C"))) ;     # AC Timing U 05
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00054045"))) ;     # AC Timing U 06
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00000000"))) ;     # AC Timing U 07
    $fs = $fs + 4 ;

    print FILE pack ("Q", hex ("0")) ;                                  #  DRAM_RANK_SIZE[4]
    print FILE pack ("Q", hex ("0")) ;
    print FILE pack ("Q", hex ("0")) ;
    print FILE pack ("Q", hex ("0")) ;
    $fs = $fs + 32 ;

    print FILE pack ("L", hex (lc("0x00042100"))) ;     # EMI_CONF_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0400f050"))) ;     # CHN0_EMI_CONA_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x04000000"))) ;     # CHN1_EMI_CONA_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("CBT_R0_R1_NORMAL"))) ;     # MODE_EXTERN_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex ("0")) ;					#reserved[6]
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    $fs = $fs + 24 ;

    print FILE pack ("L", hex (lc("0x0"))) ;               # DDR2_MODE_REG5
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0"))) ;               # pin-mux type for tablet
    $fs = $fs + 4 ;

#       print "1.file size is $fs \n";
    return $fs;

}

sub write_tag_one_element_default_LPDDR4P()
{
    my $id = $_[0];
    my $fs = 0 ;

    print "in write_tag_one_element_default_LPDDR4P";
    print FILE pack ("L", hex (lc("0x1"))) ;        # Sub_version checking for flash tool
    $fs = $fs + 4 ;

    print FILE pack("L", hex("0x0007"));                           #type
    $fs = $fs + 4 ;

    print FILE pack ("L", scalar("0")) ;        # EMMC ID checking length
    $fs = $fs + 4 ;

    print FILE pack ("L", scalar("0")) ;        # FW ID checking length
    $fs = $fs + 4 ;

    $_ = "{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}" ;
    if (/(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+)/)
    {
        print FILE pack ("C", hex ($1)) ;            #id
        print FILE pack ("C", hex ($2)) ;
        print FILE pack ("C", hex ($3)) ;
        print FILE pack ("C", hex ($4)) ;
        print FILE pack ("C", hex ($5)) ;
        print FILE pack ("C", hex ($6)) ;
        print FILE pack ("C", hex ($7)) ;
        print FILE pack ("C", hex ($8)) ;
        print FILE pack ("C", hex ($9)) ;
        print FILE pack ("C", hex ($10)) ;
        print FILE pack ("C", hex ($11)) ;
        print FILE pack ("C", hex ($12)) ;
        print FILE pack ("C", hex ($13)) ;
        print FILE pack ("C", hex ($14)) ;
        print FILE pack ("C", hex ($15)) ;
        print FILE pack ("C", hex ($16)) ;
        $fs = $fs + 16 ;
    }
    $_= "{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}";
    if (/(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+)/)
    {
        print FILE pack ("C", hex ($1)) ;            #fw id
        print FILE pack ("C", hex ($2)) ;
        print FILE pack ("C", hex ($3)) ;
        print FILE pack ("C", hex ($4)) ;
        print FILE pack ("C", hex ($5)) ;
        print FILE pack ("C", hex ($6)) ;
        print FILE pack ("C", hex ($7)) ;
        print FILE pack ("C", hex ($8)) ;
        $fs = $fs + 8 ;
    }

    print FILE pack ("L", hex (lc("0x0000f050"))) ;     # EMI_CONA_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00000000"))) ;     # EMI_CONH_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x06090640"))) ;     # AC Timing U 00
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x113D1101"))) ;     # AC Timing U 01
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0D060301"))) ;     # AC Timing U 02
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x99080307"))) ;     # AC Timing U 03
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x224E6561"))) ;     # AC Timing U 04
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0B0B0C0C"))) ;     # AC Timing U 05
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00054045"))) ;     # AC Timing U 06
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x00000000"))) ;     # AC Timing U 07
    $fs = $fs + 4 ;

    print FILE pack ("Q", hex ("0")) ;                                  #  DRAM_RANK_SIZE[4]
    print FILE pack ("Q", hex ("0")) ;
    print FILE pack ("Q", hex ("0")) ;
    print FILE pack ("Q", hex ("0")) ;
    $fs = $fs + 32 ;

    print FILE pack ("L", hex (lc("0x00042100"))) ;     # EMI_CONF_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0400f050"))) ;     # CHN0_EMI_CONA_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x04000000"))) ;     # CHN1_EMI_CONA_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("CBT_R0_R1_NORMAL"))) ;     # MODE_EXTERN_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex ("0")) ;					#reserved[6]
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    $fs = $fs + 24 ;

    print FILE pack ("L", hex (lc("0x0"))) ;               # DDR2_MODE_REG5
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc("0x0"))) ;               # pin-mux type for tablet
    $fs = $fs + 4 ;

#       print "1.file size is $fs \n";
    return $fs;

}

sub write_tag_one_element()
{
    my $id = $_[0];
    my $type = "0x$DEV_TYPE1[$id]$DEV_TYPE2[$id]" ;
    my $fs = 0 ;
    print FILE pack ("L", hex (lc($Sub_version[$id]))) ;        # Sub_version checking for flash tool
    $fs = $fs + 4 ;

    print FILE pack("L", hex($type));                           #type
    $fs = $fs + 4 ;

    print FILE pack ("L", scalar($ID_Length[$id])) ;        # EMMC ID checking length
    $fs = $fs + 4 ;

    print FILE pack ("L", scalar($FW_ID_Length[$id])) ;        # FW ID checking length
    $fs = $fs + 4 ;

    $_ = $ID_String[$id] ;
    if (/(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+)/)
    {
        print FILE pack ("C", hex ($1)) ;            #id
        print FILE pack ("C", hex ($2)) ;
        print FILE pack ("C", hex ($3)) ;
        print FILE pack ("C", hex ($4)) ;
        print FILE pack ("C", hex ($5)) ;
        print FILE pack ("C", hex ($6)) ;
        print FILE pack ("C", hex ($7)) ;
        print FILE pack ("C", hex ($8)) ;
        print FILE pack ("C", hex ($9)) ;
        print FILE pack ("C", hex ($10)) ;
        print FILE pack ("C", hex ($11)) ;
        print FILE pack ("C", hex ($12)) ;
        print FILE pack ("C", hex ($13)) ;
        print FILE pack ("C", hex ($14)) ;
        print FILE pack ("C", hex ($15)) ;
        print FILE pack ("C", hex ($16)) ;
        $fs = $fs + 16 ;
    }
    $_= $FW_ID_String[$id];
    if (/(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+)/)
    {
        print FILE pack ("C", hex ($1)) ;            #fw id
        print FILE pack ("C", hex ($2)) ;
        print FILE pack ("C", hex ($3)) ;
        print FILE pack ("C", hex ($4)) ;
        print FILE pack ("C", hex ($5)) ;
        print FILE pack ("C", hex ($6)) ;
        print FILE pack ("C", hex ($7)) ;
        print FILE pack ("C", hex ($8)) ;
        $fs = $fs + 8 ;
    }


    print FILE pack ("L", hex (lc($EMI_CONA_VAL[$id]))) ;         # EMI_CONA_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc($EMI_CONH_VAL[$id]))) ;         # EMI_CONH_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc($AC_TIME_VALUE[$id][0]))) ;         # AC Timing U 00
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc($AC_TIME_VALUE[$id][1]))) ;         # AC Timing U 01
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc($AC_TIME_VALUE[$id][2]))) ;         # AC Timing U 02
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc($AC_TIME_VALUE[$id][3]))) ;         # AC Timing U 03
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc($AC_TIME_VALUE[$id][4]))) ;         # AC Timing U 04
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc($AC_TIME_VALUE[$id][5]))) ;         # AC Timing U 05
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc($AC_TIME_VALUE[$id][6]))) ;         # AC Timing U 06
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc($AC_TIME_VALUE[$id][7]))) ;         # AC Timing U 07
    $fs = $fs + 4 ;

    print FILE pack ("Q", hex (lc($DRAM_RANK0_SIZE[$id]))) ;                                  #  DRAM_RANK_SIZE[4]
    print FILE pack ("Q", hex (lc($DRAM_RANK1_SIZE[$id]))) ;
    print FILE pack ("Q", hex ("0")) ;
    print FILE pack ("Q", hex ("0")) ;
    $fs = $fs + 32 ;

    print FILE pack ("L", hex (lc($EMI_CONF_VAL[$id]))) ;         # EMI_CONF_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc($CHN0_EMI_CONA_VAL[$id]))) ;         # CHN0_EMI_CONA_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex (lc($CHN1_EMI_CONA_VAL[$id]))) ;         # CHN1_EMI_CONA_VAL
    $fs = $fs + 4 ;

    print FILE pack ("L", hex ($DIS_CBT_MODE_EXTERN_TYPE[$id])) ;     # DIS_CBT_MODE_EXTERN
    $fs = $fs + 4 ;

    print FILE pack ("L", hex ("0")) ;                                  #reserved[6]
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    print FILE pack ("L", hex ("0")) ;
    $fs = $fs + 24 ;

    if ($DEV_TYPE2[$id] == "03")
    {
        #ddr3
        print FILE pack ("L", hex (lc($LPDDR3_MODE_REG5[$id]))) ;        # LPDDR3_MODE_REG5
        $fs = $fs + 4 ;
    }
    elsif ($DEV_TYPE2[$id] == "05")
    {
	#ddr4
        print FILE pack ("L", hex (lc($LPDDR4_MODE_REG5[$id]))) ;        # LPDDR4_MODE_REG5
        $fs = $fs + 4 ;
    }
    elsif ($DEV_TYPE2[$id] == "06")
    {
	#ddr4x
        print FILE pack ("L", hex (lc($LPDDR4X_MODE_REG5[$id]))) ;        # LPDDR4X_MODE_REG5
        $fs = $fs + 4 ;
    }
    elsif ($DEV_TYPE2[$id] == "07")
    {
	#ddr4p
        print FILE pack ("L", hex (lc($LPDDR4P_MODE_REG5[$id]))) ;        # LPDDR4P_MODE_REG5
        $fs = $fs + 4 ;
    }

    print FILE pack ("L", hex (lc($PIN_MUX_TYPE[$id]))) ;         # pin mux type for tablet
    $fs = $fs + 4 ;

#       print "1.file size is $fs \n";
    return $fs;

}

