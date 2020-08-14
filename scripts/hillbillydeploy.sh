#/usr/bin/bash

set -eu
HOSTS="pi1.wlan.kruemel.home pi2.wlan.kruemel.home pi3.wlan.kruemel.home"

for h in ${HOSTS}
do
    echo "Deploy to $h"
    ssh pi@$h <<EOF
if [ ! -d yellowfish ]
then
    git clone https://github.com/RafaelOstertag/yellowfish.git
    mkdir yellowfish/build
fi

cd yellowfish
git pull

cd build
rm -rf *
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j 4
sudo make install
sudo shutdown -r +1
EOF
    echo "DONE $h"
done
