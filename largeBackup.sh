# Usage: ./largeBackup.sh

#!/bin/bash
touch -d '-3 months' $HOME/.cloudbuddy/input/timeStampMarker
backup no-label
