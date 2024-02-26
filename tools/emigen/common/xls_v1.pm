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
package xls_v1;

use strict;
use File::Basename;

my $LOCAL_PATH;

BEGIN {
	$LOCAL_PATH = dirname($0);
}

use lib "$LOCAL_PATH/../";
use pack_dep_gen;

# remove windows support
use lib "$LOCAL_PATH/../Spreadsheet";
require 'ParseExcel.pm';

our @ISA= qw( Exporter );
our @EXPORT_OK = qw( xls_open get_sheet xls_cell_value );
our @EXPORT    = qw( xls_open get_sheet xls_cell_value );

#****************************************************************************
# read a excel file to get emi settings
#****************************************************************************
# get already active Excel application or open new
sub xls_open {
	my $file = shift;

	my $parser = Spreadsheet::ParseExcel->new();

	return $parser->Parse($file);
}

#****************************************************************************
# subroutine:  trim
# input:       $string:  trim string
#****************************************************************************

sub trim {
	my $string = shift;

	$string =~ s/^\s+//;
	$string =~ s/\s+$//;
	return $string;
}

#****************************************************************************************
# subroutine:  get_sheet
# return:      Excel worksheet no matter it's in merge area or not, and in windows or not
# input:       Specified Excel Sheetname
#****************************************************************************************
sub get_sheet {
	my $book = shift;
	my $MEMORY_DEVICE_TYPE = shift;

	print $MEMORY_DEVICE_TYPE ;

	return $book->Worksheet($MEMORY_DEVICE_TYPE);
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
	return &trim(&lin_xls_cell_value($Sheet, $row, $col));
}

sub win_xls_cell_value {
	my ($Sheet, $row, $col) = @_;

	if ($Sheet->Cells($row, $col)->{'MergeCells'}) {
		my $ma = $Sheet->Cells($row, $col)->{'MergeArea'};
		return ($ma->Cells(1, 1)->{'Value'});
	} else {
		return ($Sheet->Cells($row, $col)->{'Value'})
	}
}

sub lin_xls_cell_value {
	my ($Sheet, $row, $col) = @_;
	my $cell = $Sheet->get_cell($row, $col);
	return "" unless (defined $cell);
	my $value = $cell->Value();

}

1;
