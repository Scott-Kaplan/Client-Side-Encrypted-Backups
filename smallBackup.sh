# Usage: sudo ./smallBackup.sh

#!/bin/bash
touch -d '-1 min' $HOME/.cloudbuddy/input/timeStampMarker
touch README.md
backup no-label