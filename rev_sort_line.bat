rev line_sorted.txt > line_reversed.txt
rem sed -e 's/\/n\/r/''/g' line_reversed.txt>line_reversed.txt
rem sed -e 's/\/r\/n/\/n\/r/g' line_reversed > line_reversed
rem sed -e 's/\/n\/r/\/r\/n/g' line_reversed > line_reversed
rem sed -e 's/\n\r/g' line_reversed.txt > line_reversed
sort line_reversed.txt /o line_reversed_sort.txt
rev line_reversed_sort.txt > line_sorted.txt