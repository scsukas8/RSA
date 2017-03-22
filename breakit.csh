#!/bin/csh
set user = $1
set Expected = HelloFrom.scsukas8.kYZvxGIgzYnVwbmLdwvDcvchQEdQXOLnecgZYTklt
set Actual = `./BreakRSA  741187370364926099 612783289038795421  137711107293349766 445570787035017270 2095320133394594 247184451386249146 440524455842200856 375680854268535412 606994550131259714 624289203942259084 39247189357030617 97376339791049702`
echo "expected is $Expected" 
echo "actal    is $Actual"
if ( "$Expected" == "$Actual" ) then
echo "Grade for user $user is 100"
else
echo "Grade for user $user is 50"
endif
