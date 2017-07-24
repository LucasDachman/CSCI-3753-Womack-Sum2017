#!/bin/bash
if [ $# -ne 1 ] 
then
	echo "USAGE: $0 <output dir>"
	exit 0
fi
dir=$1
mkdir $dir

sched_other_opfn="$dir/sched-other-output"
sched_fifo_opfn="$dir/sched-fifo-output"
sched_rr_opfn="$dir/sched-rr-output"
table_data_fn="$dir/table-data"

sudo ./sched_test SCHED_OTHER 100 $sched_other_opfn"-100"
sudo ./sched_test SCHED_OTHER 1000 $sched_other_opfn"-1000"
sudo ./sched_test SCHED_OTHER 10000 $sched_other_opfn"-10000"

sudo ./sched_test SCHED_FIFO 100 $sched_fifo_opfn"-100"
sudo ./sched_test SCHED_FIFO 1000 $sched_fifo_opfn"-1000"
sudo ./sched_test SCHED_FIFO 10000 $sched_fifo_opfn"-10000"

sudo ./sched_test SCHED_RR 100 $sched_rr_opfn"-100"
sudo ./sched_test SCHED_RR 1000 $sched_rr_opfn"-1000"
sudo ./sched_test SCHED_RR 10000 $sched_rr_opfn"-10000"

# compile data in table file

# 100 processes
echo "100 processes:" >> $table_data_fn
tail -n 19 $sched_other_opfn'-100' >> $table_data_fn
tail -n 19 $sched_fifo_opfn'-100' >> $table_data_fn
tail -n 19 $sched_rr_opfn'-100' >> $table_data_fn

# 1000 processes
echo "1000 processes:\n" >> $table_data_fn
tail -n 19 $sched_other_opfn'-1000' >> $table_data_fn
tail -n 19 $sched_fifo_opfn'-1000' >> $table_data_fn
tail -n 19 $sched_rr_opfn'-1000' >> $table_data_fn

# 10000 processes
echo "10000 processes:\n" >> $table_data_fn
tail -n 19 $sched_other_opfn'-10000' >> $table_data_fn
tail -n 19 $sched_fifo_opfn'-10000' >> $table_data_fn
tail -n 19 $sched_rr_opfn'-10000' >> $table_data_fn


