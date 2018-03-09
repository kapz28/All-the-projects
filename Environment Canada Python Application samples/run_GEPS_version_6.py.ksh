#! /usr/bin/ksh
# run_verify_strfw_fcastHr_acc_watrouteAF1F2_BAMS_WAT_strfw.py.ksh

# This script launches verify_strfw_fcastHr_acc_watrouteAF1F2_BAMS_WAT_strfw.py

# Written by Dorothy Durnford, November 2015

#####################################################################

#date_str=20151104  # Verification period for Vincent's presentation of Feb 12, 2016
#date_end=20160131  # Verification period for Vincent's presentation of Feb 12, 2016

datestr_str=20160521   # Start of forcing with RDPS v5
dateend_str=20160522
#dir_out=~afsgdod/cetus3/Kapilan/test_dump
dir_out=~afsgkas/diskspace/test_dump
in_dir=~afsgkas/diskspace/operation.ensemble.prog.ens.glbmodel
gridtemplate=GEPS_grid_template.fst

set +x
if [ ! -d $dir_out ] ; then mkdir -p $dir_out ; fi
echo -e "desire(-1,['^^','>>'])" | editfst -s  ~afsgkas/diskspace/rarc/operation.ensemble.prog.ens.glbmodel/2016060100_024_001 -d ${dir_out}/${gridtemplate} -e
echo ""
echo ""
echo "Launching the script using:"


set -x
python  GEPS_version_6.py  $datestr_str $dateend_str $dir_out $in_dir $gridtemplate
set +x

# convert $outfile.png $outfile.pdf
