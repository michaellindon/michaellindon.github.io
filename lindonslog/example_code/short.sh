#!/bin/bash
# short.sh: a short discovery job

printf "Start time: "; /bin/date
printf "Job is running on node: "; /bin/hostname
printf "Job running as user: "; /usr/bin/id

echo "Environment:"
/bin/env | /bin/sort

echo "Dramatic pause..."
sleep ${1-15}    # Sleep 15 seconds, or however much we're told to sleep
echo "Et voila!"
